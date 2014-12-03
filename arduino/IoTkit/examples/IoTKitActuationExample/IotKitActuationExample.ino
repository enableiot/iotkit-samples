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
