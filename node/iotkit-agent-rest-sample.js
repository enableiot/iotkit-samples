var http = require('http'),
    os = require("os"),
    fs =  require("fs");


// Sample temperature 
var getTemp = function(){
    var temp = 26.5;
    // TODO: Do your magic here to populate the temp value
    return temp;
}

// Message
var msg = {
    "metric": "temp", 
    "value": getTemp()
}

// Message string
jsonObj = JSON.stringify(msg);
console.info('Reading:');
console.info(jsonObj);

// HTTP Headers
var putHeaders = {
    'Content-Type' : 'application/json',
    'Content-Length' : Buffer.byteLength(jsonObj, 'utf8')
};

// HTTP Options 
var putOpts = {
    host : 'localhost',
    port : 8080,
    path : '/temp-sensor',
    method : 'PUT',
    headers : putHeaders
};

console.info('Options:');
console.info(putOpts);


// do the POST call
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




