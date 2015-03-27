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

//This example reads the cpu load, memory usage, voltage and uptime on Edison board
//and sends the data to the Intel IoTkit Cloud
//It also sends 1 as power when agent is started.

//Prework:
//You need iotkit-agent installed and running.
//Device should be activated. (by running: iotkit-admin activate ACTIVATION_CODE)
//Following components should be registered (by running: iotkit-admin register NAME TYPE):
//uptime, cpu_proc, mem_proc, voltage of type e.g. temperature.v1.0
//power of type powerswitch.v1.0

#include <IoTkit.h>    // include IoTkit.h to use the Intel IoT Kit
#include <Ethernet.h>  // must be included to use IoTkit
#include <aJSON.h>
#include <stdio.h>

// create an object of the IoTkit class
IoTkit iotkit;        
int voltage;
char buf[112];

void setup() {
  Serial.begin(115200);
  // call begin on the IoTkit object before calling any other methods
  iotkit.begin();
  delay(10500);
  iotkit.send("power", 1);
}

void sendUptime(){
  FILE *lsofFile_p = popen("uptime | awk '{gsub(/:/,\"\"); print $1}'", "r");

  if (lsofFile_p)
  {
    char buffer[1024];
    char *line_p = fgets(buffer, sizeof(buffer), lsofFile_p);

    Serial.print("uptime:");
    Serial.println(buffer);
    iotkit.send("uptime", atof(buffer));
  }
  pclose(lsofFile_p);
}

void sendProc(){
  FILE *lsofFile_p = popen("grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage }'", "r");

  if (lsofFile_p)
  {
    char buffer[1024];
    char *line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
 
    Serial.print("Cpu:");
    Serial.println(buffer);
    iotkit.send("cpu_proc", atof(buffer));
  }
  pclose(lsofFile_p);
}

void sendMem(){
 
  FILE *lsofFile_p = popen(" free | awk 'FNR == 3 {print $3/($3+$4)*100}'", "r");

  if (lsofFile_p)
  {
    char buffer[1024];
    char *line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
 
    Serial.print("Mem:");
    Serial.println(buffer);
    iotkit.send("mem_proc",atof(buffer));
  }
  pclose(lsofFile_p);
}

void logDevice(){
  system("top -n 1 | grep 'CPU\|Mem' | head -n 2 >> /home/root/logagent.log");
  system("top -n 1 | grep 'iot\|sket' | head -n 2 >> /home/root/logagent.log");
  system("curl -ILs 'http://www.google.com' | head -n 1  >> /home/root/logagent.log");
  system("uptime >> /home/root/logagent.log");
}

void loop() {
  int voltage = getVolt();
  Serial.println("Voltage is: ");
  Serial.print(voltage);
  Serial.println(".");

  iotkit.send("voltage", voltage);
  sendProc();
  sendMem();
  sendUptime();

  delay(5000);
}

// reads hardware voltage sensor
int getVolt(){
  char scale[4];
  char raw[4];
  char offset[4];
  
  int raw_i;
  int scale_i;
  int offset_i;

  FILE *fp_raw;
  fp_raw = fopen("/sys/bus/iio/devices/iio:device1/in_voltage0_raw", "r");  //read the values from scale and raw
  fgets(raw, 4, fp_raw);                                                    //the formula for calulating the actual voltage
  fclose(fp_raw);                                                           //is: VOLTAGE = RAW * SCALE 
                                                                            
  FILE *fp_scale;
  fp_scale = fopen("/sys/bus/iio/devices/iio:device1/in_voltage_scale", "r");
  fgets(scale, 4, fp_scale);
  fclose(fp_scale);
  
  raw_i = atoi(raw);         //we have the values now, but they are in ASCII form-                                                       
  scale_i = atoi(scale);     //we need them as integers so we can use them for calculations.
  
  float voltage = raw_i  * scale_i;  //Calculate voltage

  return voltage;
}
