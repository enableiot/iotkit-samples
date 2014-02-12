// Set the date and time (UTC)
// This only needs to be done each time the board is powered on, 
// unless power is applied to the COIN header

#include <Ethernet.h>	// must be included to use IoTkit
#include <IoTkit.h>		// include IoTkit.h to use the Intel IoT Kit

void setup() {
  // set the date and time
  // parms - year, month, day, hour, minute, second
  IoTkit::setDateTimeUtc(2013, 2, 15, 11, 0, 0);
}

void loop() {
}