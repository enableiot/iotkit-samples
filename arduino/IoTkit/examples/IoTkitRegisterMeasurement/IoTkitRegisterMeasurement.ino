// Register a measurement
// This only needs to be done once for each measurement taht will be
// sent from the board. When sending measurements, it's important that
// the source name matches the name used in the source here.

#include <Ethernet.h>	// must be included to use IoTkit
#include <IoTkit.h>		// include IoTkit.h to use the Intel IoT Kit

// create an object of the IoTkit class
IoTkit iotkit;

void setup() {
  // call begin on the IoTkit object before calling any other methods
  iotkit.begin();

  // register the temp source 
  iotkit.registerMeasurement("temp", "float", "Celsius");
}

void loop() {
}