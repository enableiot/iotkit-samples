/*
Copyright (c) 2012, Intel Corporation

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

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

var Netcat = require('node-netcat');
var client = Netcat.udpClient(41234, '127.0.0.1');
var _ = require('underscore');
var ninjaBlocks = require('ninja-blocks');

client.on('open', function () {  console.log('open'); });
client.once('error', function (err) {  console.error('err'); });
client.once('close', function () { console.log('client, closed'); });


var ninja = ninjaBlocks.app({user_access_token:"YOUR_API_TOKEN_HERE"});


ninja.devices({ device_type: 'temperature' }, function(err, devices) {
    _.each(devices, function(device,guid){
        ninja.device(guid).last_heartbeat(function(err, data) {
            var data = '{"s": "Temperature", "v": ' + data.DA + '}';
            console.dir(data);
            client.send(data);
        })
    })
});


ninja.devices({ device_type: 'humidity'}, function(err, devices) {
    _.each(devices, function(device,guid){
        ninja.device(guid).last_heartbeat(function(err, data) {
          var data = '{"s": "Humidity", "v": ' + data.DA + '}';
          console.dir(data);
          client.send(data);
        })
    })
});

setTimeout(function () {
  console.log("waiting to close...");
  client.close();
}, 6000);