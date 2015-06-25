#!/usr/bin/env node
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
 
/*
 This script opens a secure connection directly to the IoT Analytics server
 using the device credentials and requests the server health status 
*/
var mqtt = require('mqtt');

var client;
/*
 Get device/account settings from the iotkit-agent configuration file 
 (data/device.json) and fill in the variables below
 */
var device_id = "<device ID>";
var device_token = "<device token>"
var account_id = "<account ID>";
var comp_id = "<component_id>";

certs = [ "/usr/lib/node_modules/iotkit-agent/certs/AddTrust_External_Root.pem" ];
broker_host = "broker.us.enableiot.com";
broker_port = 8883;

// Device authentication
var mypass = {
        "username": device_id,
        "password": device_token,
        "keepalive": 60,
        "ca": certs,
        "rejectUnauthorized": true
}

// Define MQTT message and topics for server-health request
var msg = { "detail": "mqtt" };
ptopic = 'server/devices/'+device_id+'/health';
stopic = 'device/'+device_id+'/health';

// Open secure MQTT connection using iotkit credentials
client = mqtt.createSecureClient(broker_port, broker_host, mypass);

client.on('connect', function () {
   console.log('Subscribing to: ' + stopic);
   client.subscribe(stopic, function () {
      client.on('message', function (topic, message) {
         console.log(message.toString());
         client.end();
      });
   });
   console.log('Publishing: ' + ptopic);
   client.publish(ptopic, JSON.stringify(msg), mypass);
});
