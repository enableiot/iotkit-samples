var mqtt = require('mqtt');


var broker_endpoint = 'data.enableiot.com';
var broker_port = 8884;
var broker_sec_opt = {
	 keyPath: '/Users/markchma/Dropbox/Code/enableiot/tools/certs/client.key',
	 certPath: '/Users/markchma/Dropbox/Code/enableiot/tools/certs/client.crt',
	 username: 'mark',
	 password: 'password1234',
	 keepalive: 30000
}
var broker_topic = "data/mark/device-laptop"
var msg_num = 100;
var msg = "{'name': 'test', 'value': 1.23}"

var client = mqtt.createSecureClient(broker_port, broker_endpoint, broker_sec_opt);

client.publish(broker_topic, JSON.stringify(msg));