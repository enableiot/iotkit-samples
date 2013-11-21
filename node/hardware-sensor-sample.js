var mqtt = require('mqtt'),
    os = require("os"),
    fs =  require("fs"),
    mac = require("getmac");

// Orchestration variables 
var is_debug = true;
var send_freq = (is_debug) ? 18000 : 60000; //1 min

// Message endpoint variables
var root_topic = "data";
var account_id = process.env.IOT_ACCOUNT_ID || "a-101-sample";
var device_id = process.env.IOT_DEVICE_ID || "d-123456-sample";
var broker_endpoint = process.env.IOT_BROKER_URI || "broker.enableiot.com";
var broker_port = 1883;

// Endpoint configuration
var broker_topic = "/" + root_topic + "/" + account_id + "/" + device_id
var broker_sec_opt = {
     clientId: os.hostname(),
     keepalive: 30000
}

// Local functions 
var getTimestamp = function(){
    return new Date().getTime();
}

var cloneObject = function(obj1){
    return JSON.parse(JSON.stringify(obj1));
}

var getTemp = function(){

    var temp = 0;
    var TEMP_SRC = "/sys/class/thermal/thermal_zone0/temp";

    if (fs.existsSync(TEMP_SRC)){
        var t = fs.readFileSync(TEMP_SRC);
        temp = ((t/1000).toPrecision(3));
    }

    return temp;

}

var makeItem = function(name, value){

    var item_template = { 
        "name": "",
        "sample": [ { "value": 0, "timestamp": 0 } ]
    }

    var item = cloneObject(item_template);
        item.name = name;
        item.sample[0].value = value;
        item.sample[0].timestamp = getTimestamp();

    return item;

}


// MQTT Client
var client = mqtt.createClient(broker_port, broker_endpoint, broker_sec_opt);

// Main function
var get_metrics = function(){

    // get MAC address
    mac.getMac(function(err, macAddress){

        // array to hold the metrics
        var metrics = [];

        // clone message, new instance 
        var msg = {
            "msg_type": "metrics_msg",
            "sender_id": os.hostname(),
            "account_id": account_id,
            "timestamp": getTimestamp(),
            "data_source": [
                {
                    "name": "gateway_data_source",
                    "metrics": []
                }
            ]
        };

        if (err) console.log(err);
        else msg.sender_id = macAddress; 

        // info about the CPU
        var cpus = os.cpus();

        // CPU
        for (var i in cpus) {
            metrics.push(makeItem("cpu_" + i + "_model",cpus[i].model));
            metrics.push(makeItem("cpu_" + i + "_speed",cpus[i].speed));
            metrics.push(makeItem("cpu_" + i + "_time_user",cpus[i].times.user));
            metrics.push(makeItem("cpu_" + i + "_time_sys",cpus[i].times.sys));
            metrics.push(makeItem("cpu_" + i + "time_idle",cpus[i].times.idle));
        }

        // Temp
        metrics.push(makeItem("temp", getTemp()));

        // Memory Total
        metrics.push(makeItem("total_memory", os.totalmem()));

        // Memory Free
        metrics.push(makeItem("free_memory", os.freemem()));

        // Uptime
        metrics.push(makeItem("uptime", os.uptime()));


        // Load averages
        /*
            The load average is a very UNIX-y concept; 
            On Windows platforms this function always returns [0, 0, 0] 
        */
        var loadavgs = os.loadavg();

        // 1, 5, 15 load avg min
        metrics.push(makeItem("min1_load_avg", loadavgs[0]));
        metrics.push(makeItem("min5_load_avg", loadavgs[1]));
        metrics.push(makeItem("min15_load_avg", loadavgs[2]));


        // Add metrics to the message
        msg.data_source[0].metrics = metrics;

        // If in debug than print to console, else send to broker
        if (is_debug) 
            console.log(JSON.stringify(msg)); 
        else
            client.publish(broker_topic, JSON.stringify(msg));
        
        console.log("OK: " + new Date());

        // Wash, rinse and repeat 
        setTimeout(get_metrics, send_freq);

    });

}


// Invoke main function
get_metrics();


