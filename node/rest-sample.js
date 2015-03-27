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

var http = require('http'),
    os = require("os"),
    fs =  require("fs");


// Sample data, replace it with sensor call results
var msg = { 
    "s": "temp-sensor", 
    "v": 26.7 
};


// HTTP Headers
var putHeaders = {
    'Content-Type' : 'application/json',
    'Content-Length' : Buffer.byteLength(jsonObj, 'utf8')
};

// HTTP Options 
var putOpts = {
    host : '127.0.0.1',
    port : 8080,
    path : '/data',
    method : 'PUT',
    headers : putHeaders
};

// Do the POST call
var putReq = http.request(putOpts, function(res) {
    console.log("statusCode: ", res.statusCode);
    res.on('data', function(d) {
        console.info('PUT result:\n');
        process.stdout.write(d);
        console.info('\n\nPUT completed');
    });
});

// Write JSON data
putReq.write(jsonObj);
putReq.end();
putReq.on('error', function(e) {
    console.error(e);
});




