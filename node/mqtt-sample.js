var mqtt = require('mqtt'),
    os = require("os"),
    fs =  require("fs");

// Sample data, replace it with sensor call results
var msg = { 
    "s": "temp-sensor", 
    "m": "air-temp", 
    "v": 26.7 
};

// Create sample MQTT Client
var client = mqtt.createClient(1883);

// Publish the message to the local agent
client.publish("data", JSON.stringify(msg));

console.log("Done");
