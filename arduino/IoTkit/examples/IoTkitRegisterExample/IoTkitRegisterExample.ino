// READ ALL COMMENTS BEFORE RUNNING AND SET REQUIRED VALUES IN COMMANDS RUN
// This sketch helps register your device and any sensors you would like to declare.
// Open your Serial monitor to view the output of the commands.
// This will walk you through the iotkit-agent and iotkit-samples readmes
// The registration needs to be performed only once for each new sensor

#include <IoTkit.h>    // include IoTkit.h to use the Intel IoT Kit
#include <Ethernet.h>  // must be included to use IoTkit

IoTkit iotkit;

void setup() {
    Serial.begin(115200);
    // Additional time to open Arduino Serial Monitor
    delay(10000);
}

void loop() {
    // Check connectivity
    system("iotkit-admin test > /dev/ttyGS0 2>&1");
    // Replace YOUR_DEVICE_ID with your value
    system("iotkit-admin set-device-id YOUR_DEVICE_ID > /dev/ttyGS0 2>&1");
    // Setting data-directory used. Value provided is default location when installed globally, using npm.
    // Change it if your agent is installed in other location or you have moved data directory
    system("iotkit-admin set-data-directory /usr/lib/node_modules/iotkit-agent/data > /dev/ttyGS0 2>&1");
    // Replace values with your proxy host and port or comment out if proxy is not used
    //system("iotkit-admin proxy http://PROXY-HOST PORT > /dev/ttyGS0 2>&1");
    // Find your device activation key on the IoT web account and replace XXXXXXXX with it
    system("iotkit-admin activate XXXXXXXX > /dev/ttyGS0 2>&1");
    // After activation we should restart iotkit-agent service
    // If your installation is local, execute scripts start-agent.sh and stop-agent.sh instead providing full path to them
    system("systemctl stop iotkit-agent > /dev/ttyGS0 2>&1");
    delay(5000);
    system("systemctl start iotkit-agent > /dev/ttyGS0 2>&1");
    for(int i = 0; i < 3; i++) {
      delay(5000);
      system("systemctl status iotkit-agent > /dev/ttyGS0 2>&1");
    }
    
    //now we will register sensor names
    iotkit.begin();
    Serial.println("Registering components");
    // feel free to change/add more sensors you will be sending data from
	iotkit.send("{ \"n\": \"temperature\", \"t\": \"temperature.v1.0\"}\n");
	iotkit.send("{ \"n\": \"humidity\", \"t\": \"humidity.v1.0\"}\n");
	iotkit.send("{ \"n\": \"power\", \"t\": \"powerswitch.v1.0\"}\n");
	//
	// Where:
	//
	// n - the sensor name ("Temperature", "Humidity", "Weight", "Force", etc.)
	// t - is the sensor type of data this source generates 
	// (This should be one of the Component Type defined in your account Catalog available in the iotkit-dashboard)
	// stopping loop, go ahead and load another sketch! Try IoTkitSimpleExample
	Serial.println("done");
	while(1) {
	  delay(60000);
	}
}
