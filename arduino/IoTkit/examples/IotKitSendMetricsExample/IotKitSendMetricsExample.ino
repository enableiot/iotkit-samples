//This example reads the cpu load, memory usage, voltage and uptime on Edison board
//and sends the data to the Intel IoTkit Cloud
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
  int voltage=getVolt();
  Serial.println("Voltage is: ");
  Serial.print(voltage);
  Serial.println(".");

  iotkit.send("voltage", voltage);  
  sendProc();
  sendMem();
  sendUptime();

  delay(5000);
}

// reads hardware temp sensor
int getVolt(){
  char scale[4];
  char raw[4];
  char offset[4];
  
  int raw_i;
  int scale_i;
  int offset_i;

  FILE *fp_raw;
  fp_raw = fopen("/sys/bus/iio/devices/iio:device1/in_voltage0_raw", "r");  //read the values from scale, raw and offset files.
  fgets(raw, 4, fp_raw);                                                    //we need all three values, because the formula for
  fclose(fp_raw);                                                           //calulating the actual temperature in milli-degrees Celcius
                                                                            //is: TEMP = (RAW + OFFSET) * SCALE 
  FILE *fp_scale;
  fp_scale = fopen("/sys/bus/iio/devices/iio:device1/in_voltage_scale", "r");
  fgets(scale, 4, fp_scale);
  fclose(fp_scale);
  
  raw_i = atoi(raw);         //we have the values now, but they are in ASCII form-                                                       
  scale_i = atoi(scale);     //we need them as integers so we can use them for calculations.
  //offset_i = atoi(offset);
  
  float temp = raw_i  * scale_i;  //Calculate temperature in milli-degrees celcius
                                  //divide by 1000 to convert to degrees celcius
  return temp;
}
