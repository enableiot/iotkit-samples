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

//This example reads the temperature sensor on the Galileo's
//onboard ADC, AD7298, using the iio (Industrial I/O) subsystem and 
//sends the observed data to the Intel IoTkit Cloud

//Prework:
//You need iotkit-agent installed and running.
//Device should be activated. (by running: iotkit-admin activate ACTIVATION_CODE)
//Following components should be registered (by running: iotkit-admin register NAME TYPE):
//temperature of type temperature.v1.0
//power of type powerswitch.v1.0
//You can also use IoTkitRegisterExample to activate and register new components

#include <IoTkit.h>    // include IoTkit.h to use the Intel IoT Kit
#include <Ethernet.h>  // must be included to use IoTkit
#include <aJSON.h>

// create an object of the IoTkit class
IoTkit iotkit;        
int temp;


void setup() {
  Serial.begin(115200);
  // call begin on the IoTkit object before calling any other methods
  iotkit.begin();

  // If you are receiving incoming commands, listen for them with receive
  // If you have your own custom json-parsing receive function, pass as argument
  // such as iotkit.receive(callback);
  // It must follow this prototype, but any name: void callback(char* json)
  //
  iotkit.receive(callback);
}

void loop() {
  Serial.println("Reading temperature");
  temp = getADCTemp();
  Serial.print("Temperature is ");
  Serial.print(temp);
  Serial.println(" degrees celcius.");
  
  // call send to generate one observation.
  // parm1 - the name of the measurement. It must have been previously registered.
  // parm2 - the value to send as the measurement observation
  // you can also generate your own JSON and send multiple keys and values
  // in this format:
  //
  // {
  //   “n”: “temperature”,
  //   “v”: “27.2"
  // }
  //
  // you need to escape the quotations to pass it directly to iotkit.send:
  // iotkit.send("{\"n\": \"temperature\",\"v\":\"27.2\"}");
  //
  // you can also send a full JSON string with your own variables:
  //
  // aJsonObject* root = aJson.createObject();
  // if (root != NULL) {
  //    aJson.addItemToObject(root, "n", aJson.createItem(metric));
  //    aJson.addItemToObject(root, "v", aJson.createItem(value));
  //    iotkit.send(aJson.print(root)); // this sends your full json
  //    aJson.deleteItem(root);
  // }
  //
  iotkit.send("temperature", temp);
  delay(2000);
}

// this is an example callback that parses a user-created JSON in the form
// {
//    "component": "LED01",
//    "command": "1"
// }
// and turns off LED at pin 13 hard-coded
//
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
  if ((component != NULL)) {
    if (strcmp(component->valuestring, "LED") == 0) {
      if ((command != NULL)) {
        if (strcmp(command->valuestring, "0") == 0) {
          pinMode(13, OUTPUT);
          digitalWrite(13, false);
        }
        if (strcmp(command->valuestring, "1") == 0) {
          pinMode(13, OUTPUT);
          digitalWrite(13, true);
        }
      }
    }
  }
}

// reads hardware temp sensor
int getADCTemp(){
  bool successful = true;
  char scale[4];
  char raw[4];
  char offset[4];
  
  int raw_i;
  int scale_i;
  int offset_i;

  //read the values from scale, raw and offset files.
  //we need all three values, because the formula for
  //calulating the actual temperature in milli-degrees Celcius
  //is: TEMP = (RAW + OFFSET) * SCALE
  FILE *fp_raw;
  fp_raw = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_raw", "r");
  if(fp_raw != NULL) {
    fgets(raw, 4, fp_raw);
    fclose(fp_raw);
  } else {
    Serial.println("Cannot open file /sys/bus/iio/devices/iio:device0/in_temp0_raw for reading.");
    Serial.println("Try another sensors readings in this directory");
    successful = false;
  }


  FILE *fp_scale;
  fp_scale = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_scale", "r");
  if(fp_scale != NULL) {
    fgets(scale, 4, fp_scale);
    fclose(fp_scale);
  } else {
    Serial.println("Cannot open file /sys/bus/iio/devices/iio:device0/in_temp0_raw for reading.");
    Serial.println("Try another sensors readings in this directory");
    successful = false;
  }

  
  FILE *fp_offset;
  fp_offset = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_offset", "r");
  if(fp_offset != NULL) {
    fgets(offset, 4, fp_offset);
    fclose(fp_offset);
  } else {
    Serial.println("Cannot open file /sys/bus/iio/devices/iio:device0/in_temp0_raw for reading.");
    Serial.println("Try another sensors readings in this directory");
    successful = false;
  }

  if(successful) {
    raw_i = atoi(raw);         //we have the values now, but they are in ASCII form-
    scale_i = atoi(scale);     //we need them as integers so we can use them for calculations.
    offset_i = atoi(offset);

    int temp = (raw_i + /*offset_i*/ 0) * scale_i;  //Calculate temperature in milli-degrees celcius
    temp /= 1000;                                   //divide by 1000 to convert to degrees celcius
    temp = raw_i;
    return temp;
  }
  return 0;
}
