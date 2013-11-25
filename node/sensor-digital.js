var arduino = require('duino'),
    board, sensor;

board = new arduino.Board({
  debug: true
});

sensor = new arduino.Sensor({
  board: board,
  pin: 'A5'
});

sensor.on('read', function(err, value) {
  value = +value;
  // |value| is the raw sensor output
  console.log( value );
});