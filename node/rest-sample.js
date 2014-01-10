var http = require('http'),
    os = require("os"),
    fs =  require("fs");


// Sample data, replace it with sensor call results
var msg = { 
    "s": "temp-sensor", 
    "m": "air-temp", 
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




