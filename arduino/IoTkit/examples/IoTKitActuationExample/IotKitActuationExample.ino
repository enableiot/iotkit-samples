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

//This example reacts for default actuator component if registered on device
//LED.v1.0 command is used
//When executed from Control section on dashboard with value 0, LED light is turned off
//When executed from Control section on dashboard with value 1, LED light is turned on
//iotkit-agent must use MQTT connection (not REST) for actuation to work.
//It also sends 1 as power when agent is started.

#include <IoTkit.h>    // include IoTkit.h to use the Intel IoT Kit
#include <Ethernet.h>  // must be included to use IoTkit
#include <aJSON.h>
#include <stdio.h>

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
    Serial.println("recieved invalid JSON");
    return;
  }
   
  aJsonObject* component = aJson.getObjectItem(parsed, "component");
  aJsonObject* command = aJson.getObjectItem(parsed, "command"); 
  aJsonObject* argv = aJson.getObjectItem(parsed, "argv");
  aJsonObject* argvArray = argv->child;
  aJsonObject* name = argvArray->child; // name : on
  aJsonObject* value = name->next; // value: 1/0
  
  if ((component != NULL)) {
    if (strcmp(component->valuestring, "actuator") == 0) {
      if ((command != NULL)) {
        if (strcmp(command->valuestring, "light") == 0 && strcmp(value->valuestring, "0") == 0) {
          Serial.println("Light Off!");
          pinMode(13, OUTPUT);
          digitalWrite(13, false);
        }
        if (strcmp(command->valuestring, "light") == 0 && strcmp(value->valuestring, "1") == 0) {
          Serial.println("Light on!");
          pinMode(13, OUTPUT);
          digitalWrite(13, true);
        }
      }
    }
  }
}
