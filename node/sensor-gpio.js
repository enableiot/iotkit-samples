var gpio = require("gpio");

gpioObj = gpio.export(12, {
   direction: "out",
   ready: function() {
      gpioObj.on("change", function(val) {
         console.log('Changed: ' + val);
      });
   }
});
