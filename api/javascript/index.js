/*
 Copyright (c) 2015, Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
"use strict";

// imports
var logger = require("./lib/logger").init(),
    util = require("util"),
    request = require("request");

// configs
var config = null,
    base_url = null;

// helpers
var process_response = function(res, body, callback) {
    var data = null;
    if (res.statusCode === 200 || res.statusCode === 201) {
        if (res.headers['content-type'] &&
            res.headers['content-type'].indexOf('application/json') > -1) {
            data = body;
        } else {
            data = null;
        }
    } else if (res.statusCode === 204) {
        data = { status: "Done" };
    }
    return  callback(data);
}

var make_request = function(options, callback){
  request(options, function (error, response, body) {
    if (!error && (response.statusCode === 200 ||
                   response.statusCode === 201 ||
                   response.statusCode === 204)) {
        process_response(response, body, function(json_data) {
             return callback(null, json_data);
        });
    } else {
        error = error || body;
        return callback(error);
    }
  });
}

var make_options = function(url, method, data, token){
  var options = {
    "url": url,
    "proxy": null,
    "method": method,
    "json": true,
    "followAllRedirects": true,
    "strictSSL": false,
    "body": data || {},
    "headers": {}
  }
  if (process.env.PROXY) {
    // logger.debug("Proxy: %s", process.env.PROXY);
    options.proxy = process.env.PROXY;
    options.strictSSL = false;
  }
  if (token) options.headers = { "Authorization": "Bearer " + token };
  return options;
}

var get_token = function(callback){
  var url = util.format("%s/auth/token", base_url);
  var data = {
    "username": config.target.username,
    "password": config.target.password
  };
  make_request(make_options(url, "POST", data), callback);
}

var get_user = function(token, callback){
  var url = util.format("%s/auth/tokenInfo", base_url);
  make_request(make_options(url, "GET", {}, token), callback);
}

var get_account = function(token, user_id, callback){
  var url = util.format("%s/users/%s", base_url, user_id);
  make_request(make_options(url, "GET", null, token), callback);
}

var get_account_guid = function(token, account_id, callback){
  var url = util.format("%s/accounts/%s", base_url, account_id);
  make_request(make_options(url, "GET", null, token), callback);
}

var parse_account_id = function(obj){
  var result = null;
  Object.keys(obj).forEach(function(account_id) {
    var account_args = obj[account_id];
    Object.keys(account_args).forEach(function(arg) {
      var arg_val = account_args[arg];
      //logger.debug(util.format("K: %s V: %s", arg, arg_val));
      if (arg == "name" && arg_val == config.target.account){
        result = account_id;
      }
    });
  });
  return result;
}

var make_device = function(token, account_id, callback){
  var url = util.format("%s/accounts/%s/devices", base_url, account_id);
  make_request(make_options(url, "POST", config.source, token), callback);
}

var activate_device = function(token, account_id, callback){
  var url = util.format("%s/accounts/%s/activationcode/refresh", base_url, account_id);
  make_request(make_options(url, "PUT", config.source, token), function(err, data){
    if (err) callback(err);
    if (data) {
      var activ_code = data.activationCode;
      logger.debug("Device Activation Code: " + data.activationCode);
      url = util.format("%s/accounts/%s/devices/%s/activation", base_url, account_id, config.source.deviceId);
      data = { "activationCode": activ_code };
      make_request(make_options(url, "PUT", data, token), function(err, data){
        if (err) callback(err);
        if (data) callback(null, data.deviceToken);
      });
    }
  });
}

var make_component = function(token, account_id, device_id, callback){
  var url = util.format("%s/accounts/%s/devices/%s/components", base_url, account_id, device_id);
  make_request(make_options(url, "POST", config.component, token), callback);
}

var submit_observation = function(config, data, callback){
  if (!callback){
    callback = function(err){ if (err) throw err; }
  }
  var url = util.format("%s/data/%s", base_url, config.device);
  var observation =  {
    "on": (new Date).getTime(),
    "accountId": config.account,
    "data": data
  }
  //logger.debug(observation);
  make_request(make_options(url, "POST", observation, config.token), callback);
}

var setup = function(in_config, callback){

  if (!in_config) throw "Null config";
  if (!callback) {
    callback = function(err){ if (err) throw err; }
  }

  config = in_config;
  base_url = util.format("https://%s%s", config.target.host, config.target.api_root);

  get_token(function(err, data){
    if (err) callback(err);
    if (data) {
      var token = data.token;
      logger.debug("Token: " + token);

      // 1. get user id from the server
      get_user(token, function(err, data){
        if (err) callback(err);
        if (data) {
            var user_id = data.payload.sub;
            logger.debug("User Id: " + user_id);

            // 2. get account id using user id
            get_account(token, user_id, function(err, data){
              if (err) callback(err);
              if (data) {
                  var account_id = parse_account_id(data.accounts);
                  logger.debug("Account Id: " + account_id);

                  // 3. make a new device
                  make_device(token, account_id, function(err, data){
                    if (err) callback(err);
                    if (data) {
                      var device_id = data.deviceId;
                      logger.debug("Device Id: " + device_id);

                      // 4. activate new device
                      activate_device(token, account_id, function(err, data){
                        if (err) callback(err);
                        if (data) {
                          var device_token = data;
                          logger.debug("Device Token: " + device_token);

                          // 5. create a component 
                          make_component(device_token, account_id, device_id, function(err, data){
                            if (err) callback(err);
                            if (data) {
                              var component_id = data.cid;
                              logger.debug("Component Id: " + component_id);

                              //TODO: Return all necessary data elements to submit observation 
                              var state = {
                                "token": device_token,
                                "component": component_id,
                                "account": account_id,
                                "device": device_id
                              }

                              callback(null, state);

                            }
                          }); // component
                        }
                      }); // device activation
                    }
                  }); // device 
              }
            }); // account
        }
      }); // user
    }
  }); // token

}


module.exports = {
    setup: setup,
    submitObservations: submit_observation
};





