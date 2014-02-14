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

  // register the measurement. This only needs to be done once per board
  // (unless the agent is updated), but there is no harm in calling it
  // each time the sketch starts
  // parm1 - the name of the metric. Calls to send must use the same value
  // parm2 - the type of the metric "int", "float" or "string"
  // parm3 - the unit of measure to be diplayed on the charts
  iotkit.registerMetric("temp", "float", "Celsius");
}

void loop() {
}