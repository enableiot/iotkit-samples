var mqtt = require('mqtt'),
    os = require("os"),
    fs =  require("fs");

// Agent 
var broker_endpoint = "localhost";
var broker_topic = "data/temp-sensor";
var broker_port = 1883;
var broker_sec_opt = {
     clientId: os.hostname(),
     keepalive: 30000
}

// Sample temperature 
var getTemp = function(){
    var temp = 0;
    var TEMP_SRC = "/sys/class/thermal/thermal_zone0/temp";
    if (fs.existsSync(TEMP_SRC)){
        var t = fs.readFileSync(TEMP_SRC);
        temp = ((t/1000).toPrecision(3));
    }
    return temp;
}

// Create sample MQTT Client
var client = mqtt.createClient(broker_port, broker_endpoint, broker_sec_opt);

// Create a Temperature message
var msg = '{"metric": "temp", "value": 0}';
    msg.value = getTemp();

// Publish the message to the local agent
client.publish(broker_topic, JSON.stringify(msg));

console.log("Done");
