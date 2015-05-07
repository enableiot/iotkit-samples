/*
Copyright (c) 2015, Intel Corporation

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

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

//This example reacts for default actuator component if registered on device
//Prequisites:
// 1. Device must be activated
// 2. A component named "power" of actuator type with number data type
//This example sends back numeric value to an actuator when actuation is received

#include <IoTkit.h>    // include IoTkit.h to use the Intel IoT Kit
#include <Ethernet.h>  // must be included to use IoTkit
#include <aJSON.h>
#include <stdio.h>
#include <stdlib.h>

// create an object of the IoTkit class
IoTkit iotkit;
int temp;
char buf[112];

void setup() {
  Serial.begin(115200);
  // call begin on the IoTkit object before calling any other methods
  iotkit.begin();
  delay(10500);
  iotkit.send("power", 1);
}

void loop() {
  iotkit.receive(callback);
  delay(5000);
}

void callback(char* json) {
  Serial.println(json);
  aJsonObject* parsed = aJson.parse(json);
  if (&parsed == NULL) {
    // invalid or empty JSON
    Serial.println("received invalid JSON");
    return;
  }

  aJsonObject* component = aJson.getObjectItem(parsed, "component");
  aJsonObject* command = aJson.getObjectItem(parsed, "command");
  aJsonObject* argv = aJson.getObjectItem(parsed, "argv");
  aJsonObject* argvArray = argv->child;
  aJsonObject* name = argvArray->child; // name : on
  aJsonObject* value = name->next; // value: string

  if ((component != NULL)) {    
      if ((command != NULL)) {
        Serial.println("Sending actuation value to cloud");
        iotkit.send("power", atoi(value->valuestring));
      }
  }
}
