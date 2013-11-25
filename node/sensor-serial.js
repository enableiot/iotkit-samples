var arduino = require("arduino");
var board = arduino.connect("/dev/tty5");

board.pinMode(5, arduino.OUTPUT);

console.log("L: " + arduino.LOW);
console.log("H: " + arduino.HIGH);