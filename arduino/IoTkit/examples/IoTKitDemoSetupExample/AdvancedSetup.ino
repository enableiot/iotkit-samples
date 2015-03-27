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

/*
SETUP INSTRUCTIONS

1. Get library from https://github.com/Seeed-Studio/Sketchbook_Starter_Kit_V2.0/tree/master/libraries/Grove_LCD_RGB_Backlight
2. Script assumes that you have successfully executed Setup.ino sketch and connected temperature, LCD display and led light as mentioned in Setup.ino setup instructions.
3. Connect light sensor to A1 pin, rotary angle sensor to A2.
4. Connect temperature sensor to A0 pin and LED light to D3 pin.
5. Connect LCD Groove display to I2C port.

*/

#include <Wire.h>
#include "rgb_lcd.h" //library from https://github.com/Seeed-Studio/Sketchbook_Starter_Kit_V2.0/tree/master/libraries/Grove_LCD_RGB_Backlight
#include <IoTkit.h>
#include <Ethernet.h>
#include <aJSON.h>

int outPinLed = 3;
int inPinLightSensor = A1;
int inPinTemperatureSensor = A0;
int inPinRotaryAngleSensor = A2;

IoTkit iotkit;
rgb_lcd lcd;

int TemperatureValue = 0;
int LightSensorValue = 0;
int RotaryValue = 0;

int mytime=0;
int delayTime=100;

void setup() {
  //Register powerswitch
  Serial.begin(115200);
  pinMode(outPinLed, OUTPUT);
  lcd.begin(16, 2);
  lcd.setRGB(0,174,239);
  lcd.print("Intel Inside");
  lcd.setCursor(0,1);
  lcd.print("Edison");
  iotkit.begin();
}

void loop() {
  readAllSensors();
  turnOnOfLed();
  iotkit.receive(callback);
  sendDataEachSec(10); //each 10 sec
  delay(delayTime) ;
}

void readAllSensors(){
  int temp =  analogRead(inPinTemperatureSensor);
  float resistance = (float)(1023-temp)*10000/temp;
  TemperatureValue= 1/(log(resistance/10000)/3975+1/298.15)-273.15;


  LightSensorValue =  analogRead(inPinLightSensor);
  RotaryValue = analogRead(inPinRotaryAngleSensor);
}

void sendDataEachSec(int secTime){
  int secWait = 1000 * secTime / delayTime;
  mytime++;
  if(mytime > secWait){
    mytime = 0;
    iotkit.send("temperature", TemperatureValue);
  }
}

void turnOnOfLed(){
  if(LightSensorValue < RotaryValue){
    digitalWrite(outPinLed, HIGH);
  } else {
    digitalWrite(outPinLed, LOW);
  }
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
  Serial.println("On Alarm:");
  OnAlarm(command->valuestring, value->valuestring);
}
void lcdClearRow(int row){
  lcd.setCursor(0,row);
  lcd.print("                ");
}
void OnAlarm(String component, String command){
  lcdClearRow(0);
  lcdClearRow(1);


  lcd.setCursor(0,0);
  lcd.print("command: "+component);
  lcd.setCursor(0,1);
  lcd.print("value: "+command);

  for(int i=0;i<6;i++){
   if(i%2==0){
     lcd.setRGB(255,0,0);
   } else {
     lcd.setRGB(0,0,255);
   }
     delay(1000);
   }

}
