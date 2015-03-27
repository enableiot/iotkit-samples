/*
 Copyright (c) 2012, Intel Corporation

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

var dgram = require('dgram');
var client = dgram.createSocket('udp4');

var day = 86400000;
// Sample data, replace it desired values
var data = [{
    sensorName : "actuator1",
    sensorType: "control.v1.0",
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
    sensorName : "hum-sensor1",
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

// UDP Options
var options = {
    host : '127.0.0.1',
    port : 41234
};

function registerNewSensor(name, type, callback){
    var msg = JSON.stringify({
        n: name,
        t: type
    });

    var sentMsg = new Buffer(msg);
    console.log("Registering sensor: " + sentMsg);
    client.send(sentMsg, 0, sentMsg.length, options.port, options.host, callback);
};

function sendObservation(name, value, on){
    var msg = JSON.stringify({
        n: name,
        v: value,
        on: on
    });

    var sentMsg = new Buffer(msg);
    console.log("Sending observation: " + sentMsg);
    client.send(sentMsg, 0, sentMsg.length, options.port, options.host);
};

client.on("message", function(mesg, rinfo){
    console.log('UDP message from %s:%d', rinfo.address, rinfo.port);
    var a = JSON.parse(mesg);
    console.log(" m ", JSON.parse(mesg));

    if (a.b == 5) {
        client.send(message, 0, message.length, PORT, HOST, function(err, bytes) {
            if (err) throw err;
            console.log('UDP message sent to ' + HOST +':'+ PORT);
            // client.close();

        });
    }
});


data.forEach(function(item) {
    registerNewSensor(item.sensorName, item.sensorType, function () {
        item.observations.forEach(function (observation) {
            setTimeout(function () {
                sendObservation(item.sensorName, observation.value, observation.on);
            }, 5000);
        });
    });
});

