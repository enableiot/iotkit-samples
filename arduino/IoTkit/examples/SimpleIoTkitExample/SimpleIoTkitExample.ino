// Example to send an observation to the Intel IoTkit cloud

#include <Ethernet.h>	// must be included to use IoTkit
#include <IoTkit.h>		// include IoTkit.h to use the Intel IoT Kit

// create an object of the IoTkit class
IoTkit iotkit;	

void setup() {
  // call begin on the IoTkit object before calling any other methods
  iotkit.begin();
}

void loop() {
  // call send to generate one observation
  iotkit.send("gateway", "temp", 26.42);	

  delay(60000);
}