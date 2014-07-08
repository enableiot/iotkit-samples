// READ ALL COMMENTS BEFORE RUNNING
// This sketch helps register your device and any sensors you would like to declare.
// Open your Serial monitor to view the output of the commands.
// This will walk you through the iotkit-agent and iotkit-samples readmes
// The registration needs to be performed only once for each new sensor

#include <IoTkit.h>    // include IoTkit.h to use the Intel IoT Kit
#include <Ethernet.h>  // must be included to use IoTkit

IoTkit iotkit;

void setup() {
    Serial.begin(115200);
}

void loop() {
  
    delay(15000);
    // open Arduino Serial Monitor within 15 seconds!
	Serial.println("Find your device activation key on the IoT web account.");
    // next time you run this sketch, comment this while(1) line:
    // and paste API key from website instead of XXXXXXXX
    while(1) {}
    // now we will activate
    // all json quotes are escaped, see IoTkitSimpleExample for more details
    system("iotkit-admin activate XXXXXXXX > /dev/ttyGS0 2>&1");
    //look for success
    
    //now we will register sensor names
    iotkit.begin();
    // feel free to change/add more sensors you will be sending data from
	iotkit.send("{ \"n\": \"temperature sensor\", \"t\": \"temperature.v1.0\"}\n");
	iotkit.send("{ \"n\": \"humidity sensor\", \"t\": \"humidity.v1.0\"}\n");
	//
	// Where:
	//
	// n - the sensor name ("Temperature", "Humidity", "Weight", "Force", etc.)
	// t - is the sensor type of data this source generates 
	// (This should be one of the Component Type defined in your account Catalog available in the iotkit-dashboard)

	// stopping loop, go ahead and load another sketch! Try IoTkitSimpleExample
	Serial.println("done");
	while(1) {}
}