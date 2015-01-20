/*
Copyright (c) 2015, Intel Corporation

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
    // Additional time to open Arduino Serial Monitor
       delay(10000);
}

void loop() {
    system("iotkit-admin test > /dev/ttyGS0 2>&1"); // Check connectivity
    system("iotkit-admin set-device-id YOUR_DEVICE_ID > /dev/ttyGS0 2>&1");   // Replace YOUR_DEVICE_ID with your value
    system("iotkit-admin set-data-directory /usr/lib/node_modules/iotkit-agent/data > /dev/ttyGS0 2>&1");
    system("iotkit-admin proxy http://PROXY-HOST PORT > /dev/ttyGS0 2>&1"); // Replace values with your proxy host and port or comment if proxy is not used
    system("iotkit-admin activate XXXXXXXX > /dev/ttyGS0 2>&1");    // Find your device activation key on the IoT web account and replace XXXXXXXX
    // After activation we should restart iotkit-agent service
    system("systemctl stop iotkit-agent > /dev/ttyGS0 2>&1");
    delay(5000);
    system("systemctl start iotkit-agent > /dev/ttyGS0 2>&1");
    for(int i = 0; i < 3; i++) {
      delay(5000);
      system("systemctl status iotkit-agent > /dev/ttyGS0 2>&1");
    }
    
    //now we will register sensor names
    iotkit.begin();
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
