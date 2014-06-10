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

var net = require('net');
var client = new net.Socket();

var day = 86400000;
// Sample data, replace it desired values
var data = [{
    sensorName : "temp-sensor",
    sensorType: "temperature.v1.0",
    observations: [{
        on: new Date().getTime() - (day * 3),
        value: "10"
    },{
        on: new Date().getTime() - (day * 2),
        value: "20"
    },{
        on: new Date().getTime() - (day),
        value: "30"
    }]
},{
    sensorName : "hum-sensor",
    sensorType: "humidity.v1.0",
    observations: [{
        on: new Date().getTime() - (day * 3),
        value: "90"
    },{
        on: new Date().getTime() - (day * 2),
        value: "50"
    },{
        on: new Date().getTime() - (day),
        value: "80"
    }]
}];

// TCP Options
var options = {
    host : 'localhost',
    port : 7070
};

function registerNewSensor(name, type, callback){
    var msg = JSON.stringify({
        n: name,
        t: type
    });

    var sentMsg = msg.length + "#" + msg;
    console.log("Registering sensor: " + sentMsg);
    client.write(sentMsg);
    callback();
};

function sendObservation(name, value, on){
    var msg = JSON.stringify({
        n: name,
        v: value,
        on: on
    });

    var sentMsg = msg.length + "#" + msg;
    console.log("Sending observation: " + sentMsg);
    client.write(sentMsg);
}

client.connect(options.port, options.host, function() {
    console.log('Connected');

    data.forEach(function(item){
       registerNewSensor(item.sensorName, item.sensorType, function(){
           item.observations.forEach(function(observation){
               setTimeout(function(){
                   sendObservation(item.sensorName, observation.value, observation.on);
               }, 3000);
           });
       });
    });
});