var mqtt = require('mqtt');

// Message endpoint variables
var BROKER_HOST = 'data.enableiot.com';
var BROKER_PORT = 8884;
var BROKER_DATA_TOPIC = "data/washington/#";
var BROKER_OPTS = {
   keyPath: process.env.BROKER_HOST_KEY || '~/enableiot/certs/client.key',
   certPath: process.env.BROKER_HOST_CERT || '~/enableiot/certs/client.crt',
   username: process.env.BROKER_HOST_USR || 'username',
   password: process.env.BROKER_HOST_PSW || 'password',
   keepalive: 30000
}

var broker = mqtt.createSecureClient(BROKER_PORT, BROKER_HOST, BROKER_OPTS);

// subscribe and process inbound messages 
console.log('Subscribing to broker...');
broker.subscribe(BROKER_DATA_TOPIC)
      .on('message', function (topic, message) {

      if (!topic) throw "Null topic";
      console.log('topic: ' + topic);
      console.log('message: ' + message);

}); // end subscribe





















