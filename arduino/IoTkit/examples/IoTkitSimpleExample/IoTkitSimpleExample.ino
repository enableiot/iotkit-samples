//This example reads the temperature sensor on the Galileo's 
//onboard ADC, AD7298, using the iio (Industrial I/O) subsystem and 
//sends the observed data to the Intel IoTkit Cloud

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
}

void loop() {
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
  //   “n”: “Temp.Sensor”,
  //   “v”: “27.2"
  // }
  //
  // you need to escape the quotations to pass it directly to iotkit.send:
  // iotkit.send("{\"n\": \"Temp.Sensor\",\"v\":\"27.2\"}");
  //

  iotkit.send("Temp.Sensor", temp);  

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

  // If you are receiving incoming commands, listen for them with receive
  // If you have your own custom json-parsing receive function, pass as argument
  // such as iotkit.receive(callback);
  // It must follow this prototype, but any name: void callback(char* json)
  //

  //iotkit.receive();
  iotkit.receive(callback);

  delay(1500);
}

// this is an example callback that parses a user-created JSON in the form
// {
//    "component": "led",
//    "command": "off"
// }
// and turns off LED at pin 13 hard-coded
//

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
  if ((component != NULL)) {
    if (strcmp(component->valuestring, "led") == 0) {
      if ((command != NULL)) {
        if (strcmp(command->valuestring, "off") == 0) {
          pinMode(13, OUTPUT);
          digitalWrite(13, false);
        }
        if (strcmp(command->valuestring, "on") == 0) {
          pinMode(13, OUTPUT);
          digitalWrite(13, true);
        }
      }
    }
  }
}

// reads hardware temp sensor
int getADCTemp(){
  char scale[4];
  char raw[4];
  char offset[4];
  
  int raw_i;
  int scale_i;
  int offset_i;

    
  FILE *fp_raw;
  fp_raw = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_raw", "r");     //read the values from scale, raw and offset files.
  fgets(raw, 4, fp_raw);                                                    //we need all three values, because the formula for
  fclose(fp_raw);                                                           //calulating the actual temperature in milli-degrees Celcius
                                                                            //is: TEMP = (RAW + OFFSET) * SCALE 
  FILE *fp_scale;
  fp_scale = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_scale", "r");
  fgets(scale, 4, fp_scale);
  fclose(fp_scale);
  
  FILE *fp_offset;
  fp_offset = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_offset", "r");
  fgets(offset, 4, fp_offset);
  fclose(fp_offset);
   
  raw_i = atoi(raw);         //we have the values now, but they are in ASCII form-                                                       
  scale_i = atoi(scale);     //we need them as integers so we can use them for calculations.
  offset_i = atoi(offset);
  
  int temp = (raw_i + offset_i) * scale_i;  //Calculate temperature in milli-degrees celcius
  temp /= 1000;                         //divide by 1000 to convert to degrees celcius
  return temp;  
  
}