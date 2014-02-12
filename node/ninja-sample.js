var Netcat = require('node-netcat');
var client = Netcat.udpClient(41234, '127.0.0.1');
var _ = require('underscore');
var ninjaBlocks = require('ninja-blocks');

client.on('open', function () {  console.log('open'); });
client.once('error', function (err) {  console.error('err'); });
client.once('close', function () { console.log('client, closed'); });


var ninja = ninjaBlocks.app({user_access_token:"YOUR_API_TOKEN_HERE"});


ninja.devices({ device_type: 'temperature' }, function(err, devices) {
    _.each(devices, function(device,guid){
        ninja.device(guid).last_heartbeat(function(err, data) {
            var data = '{"s": "Temperature", "v": ' + data.DA + '}';
            console.dir(data);
            client.send(data);
        })
    })
});


ninja.devices({ device_type: 'humidity'}, function(err, devices) {
    _.each(devices, function(device,guid){
        ninja.device(guid).last_heartbeat(function(err, data) {
          var data = '{"s": "Humidity", "v": ' + data.DA + '}';
          console.dir(data);
          client.send(data);
        })
    })
});

setTimeout(function () {
  console.log("waiting to close...");
  client.close();
}, 6000);