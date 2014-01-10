var five = require("johnny-five");

five.Board().on("ready", function() {
	// change the pin value per your configuration 
	var sensor = new five.Sensor("A5");

  	sensor.on("data", function() {
   	var voltage = this.value * 0.004882814;
   	var celsius = (voltage - 0.5) * 100;
   	var fahrenheit = celsius * (9 / 5) + 32;

   	console.log(celsius + "°C", fahrenheit + "°F");
  	});
});