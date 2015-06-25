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
 using the device credentials and submits data value. 
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

// Get current timestamp
var now = new Date().getTime();

// Device authentication
var mypass = {
        "username": device_id,
        "password": device_token,
        "keepalive": 60,
        "ca": certs,
        "rejectUnauthorized": true
}

// Define MQTT message to submit data observation(s)
var data_value = 34.5;
var msg = {
    "accountId": account_id,
    "did": device_id,
    "on": now,
    "count": 1,
    "data": [{
        "on": now,
        "value": data_value,
        "cid": comp_id
    }]
}
console.log('Establishing connection', mypass.ca)
client = mqtt.createSecureClient(broker_port, broker_host, mypass);
console.log('Publishing message')
topic = 'server/metric/'+account_id+'/'+device_id;
// Submit data
client.publish(topic, JSON.stringify(msg), mypass);
console.log('Published');
console.log('Closing connection');
client.end();