/*
SETUP INSTRUCTIONS

1. Get library from https://github.com/Seeed-Studio/Sketchbook_Starter_Kit_V2.0/tree/master/libraries/Grove_LCD_RGB_Backlight
2. Register your account at https://dashboard.us.enableiot.com , go to account and get activation code
2.1 Replace "CHANGE_ME" with your activation code for char  * activationCode="CHANGE_ME";
2.1 Replace "CHANGE_ME" with your network settings, you have to know SSID
3. Script assumes that you have both rgb_lcd and IoTkit libraries under library directory for Arduino IDE
4. Connect temperature sensor to A0 pin and LED light to D3 pin.
5. Connect LCD Groove display to I2C port.

*/
#include <stdio.h>
#include <IoTkit.h>
#include <Ethernet.h>
#include <aJSON.h>
#include <rgb_lcd.h>  //library from https://github.com/Seeed-Studio/Sketchbook_Starter_Kit_V2.0/tree/master/libraries/Grove_LCD_RGB_Backlight
#include <Wire.h>
#include <stdio.h>

char* networkSSID="CHANGE_ME";
char* WIFI_identity="CHANGE_ME";
char* WIFI_password="CHANGE_ME";
char* activationCode="CHANGE_ME";

IoTkit iotkit;
int inPinTemperatureSensor = A0;
int outPinLed = 3;
rgb_lcd lcd;

float TemperatureValue;
float minTempBlue = 22; //0% value
float maxTempRed = 40;  //100% value

void setup() {
    Serial.begin(115200);
    iotkit.begin();
    pinMode(outPinLed, OUTPUT);
    lcd.begin(16, 2);
    lcd.setRGB(0,174,239);
    ShowInfoLog("Intel Inside","Edison");
    delay(30 *1000); //wait 30s for boot

    if(network_not_set()){
        if(setNetwork()){
            if(device_not_activated()){
                activateAndRegisterComponents();
            }
        }
    }
}

void loop() {
    readAllSensors();
    turnOnOfLed();
    Display();
    iotkit.send("temperature", TemperatureValue);
    delay(1000);
}

void readAllSensors(){
    //Read temperature sensor
    int temp =  analogRead(inPinTemperatureSensor);
    float resistance = (float)(1023-temp)*10000/temp;
    TemperatureValue= 1/(log(resistance/10000)/3975+1/298.15)-273.15;
}

void turnOnOfLed(){
    if(TemperatureValue > maxTempRed){
        digitalWrite(outPinLed, HIGH);
    } else {
        digitalWrite(outPinLed, LOW);
    }
}

void lcdClearRow(int row){
    lcd.setCursor(0,row);
    lcd.print("                ");
}

void Display(){
    //from blue to red
    float tempProcentValue = (TemperatureValue - minTempBlue) * (100 / (maxTempRed-minTempBlue));
    tempProcentValue = tempProcentValue < 0 ? 0 : tempProcentValue;
    lcd.setRGB(255*tempProcentValue/100,0,(255 * (100 - tempProcentValue))/100);

    char* formated = new char[512];
    sprintf(formated, "%2.2f C", TemperatureValue);

    ShowInfoLog("Temperature:",formated);
}

void ShowInfoLog(char * firstLine, char* secondLine){
    lcdClearRow(0);
    lcdClearRow(1);
    Serial.print(firstLine);
    Serial.println(secondLine);
    lcd.setCursor(0,0);
    lcd.print(firstLine);
    lcd.setCursor(0,1);
    lcd.print(secondLine);
}

boolean device_not_activated(){

    char* agentVersion = getProcessOutput("iotkit-admin -V");
    ShowInfoLog("Agent version", agentVersion);

    if(agentVersion[0]=='1'){
        if(agentVersion[2]=='5'){
            //Edison iotki-agent 1.5.0 tokens location are in /usr/share/iotkit-agent
            char* tokenIsSet  = getProcessOutput("grep 'deviceToken' /usr/share/iotkit-agent/certs/token.json | awk '/,/{gsub(/ /,\"\",$0);gsub(/\"deviceToken\":/,\"\",$0);print $0;}'");
            if(strstr(tokenIsSet, "false") != NULL){
                ShowInfoLog("Token not set","");
                return true;
            }
        }
    }
    ShowInfoLog("Token set","");
    return false;
}

void activateAndRegisterComponents(){

    char* formated = new char[512];
    sprintf(formated, "iotkit-admin activate %s > /dev/ttyGS0 2>&1", activationCode);
    ShowInfoLog("Activating with code", activationCode);
    system(formated);

    if(device_not_activated()){
        lcd.setRGB(255,0,0);
        ShowInfoLog("There was error","during activation");
    }
    else {
        lcd.setRGB(0,255,0);
        ShowInfoLog("Activation","completed.");
        system("iotkit-admin register temperature temperature.v1.0 > /dev/ttyGS0 2>&1");
        system("iotkit-admin register powerswitch powerswitch.v1.0 > /dev/ttyGS0 2>&1"); //for future actution
        system("systemctl start iotkit-agent");
        system("systemctl enable iotkit-agent");
    }
    delay(2500);
}

boolean network_not_set(){

    char* isSet = getProcessOutput("wpa_cli status | grep ^ssid | awk '{split($0,s,\"=\");print s[2]}'");

    if(isSet){
        if(strstr(isSet, networkSSID) != NULL){
            lcd.setRGB(0,255,0);
            ShowInfoLog("Network is set", networkSSID);
            delay(2500);
            return false;
        }
    }
    ShowInfoLog(networkSSID, " network not set");
    return true;
}

String scanNetwork(){
    system("systemctl stop hostapd && sleep 2 && systemctl start wpa_supplicant");
    ShowInfoLog("Scanning network...","");
    for(int i=0;i<6;i++){
        system("wpa_cli scan");
        delay(1000);
    }
    char* formated = new char[512];
    sprintf(formated, "wpa_cli scan_results | grep %s | awk 'NR==1{gsub(/\\[/,\"\",$0);gsub(/\\]/,\"\",$0);print $4}'", networkSSID);

    char* scanResult = getProcessOutput(formated);

    ShowInfoLog("Network type:",scanResult);
    delay(3000);

    if (scanResult){

        if(strstr(scanResult,"WPA2-EAP")!= NULL || strstr(scanResult,"WPA-EAP")!=NULL){
            return "EAP";
        } else if(strstr(scanResult,"WEP")!=NULL){
            return "WEP";
        } else if(strstr(scanResult,"OPEN")!=NULL){
            return "OPEN";
        }else if(strstr(scanResult,"WPA-PSK")!=NULL || strstr(scanResult,"WPA2-PSK")!=NULL){
            return "PSK";
        }
    }

    return "";
}

void WriteWpaConfig(String networkType){

    int len = 0;
    char* network_conf = new char[512];

    if(networkType== "EAP"){
        len = sprintf(network_conf,"network={\nssid=\"%s\"\n\nkey_mgmt=WPA-EAP\npairwise=CCMP TKIP\ngroup=CCMP TKIP WEP104 WEP40\neap=TTLS PEAP TLS\nidentity=\"%s\"\npassword=\"%s\"\nphase1=\"peaplabel=0\"\n}\n",networkSSID,WIFI_identity,WIFI_password);
    }
    if(networkType== "OPEN"){
        len = sprintf(network_conf,"network={\nssid=\"%s\"\n\nkey_mgmt=NONE\n}\n",networkSSID);
    }
    if(networkType== "PSK"){
        len = sprintf(network_conf,"network={\nssid=\"%s\"\n\nkey_mgmt=WPA-PSK\npairwise=CCMP TKIP\ngroup=CCMP TKIP WEP104 WEP40\neap=TTLS PEAP TLS\npsk=\"%s\"\n}\n",networkSSID,WIFI_password);
    }
    if(networkType== "WEP"){
        len = sprintf(network_conf,"network={\nssid=\"%s\"\n\nkey_mgmt=NONE\ngroup=WEP104 WEP40\nwep_key0=\"%s\"\n}\n",networkSSID,WIFI_password);
    }

    //write configuration to file
    FILE *file =  fopen("/etc/wpa_supplicant/wpa_supplicant.conf","a");
    fwrite(network_conf, sizeof(char),len,file);
    fclose(file);
}

void RestartNetworking(){
    ShowInfoLog("Restarting network","");
    system("systemctl stop hostapd");
    system("systemctl restart wpa_supplicant");
    system("systemctl enable wpa_supplicant");
}

void SelectNetwork(){
    char* numOfNetworks = getProcessOutput("wpa_cli list_networks | awk 'END{print NR - 3}'");
    char* networkSelect = new char[512];
    sprintf(networkSelect, "wpa_cli select_network %s", numOfNetworks);
    system(networkSelect);
}

boolean WaitForIp(int timeout){
    for(int i = 0;i < timeout;i++){
        char* timeLeft = new char[15];
        sprintf(timeLeft,"%i seconds left", (30-i));
        ShowInfoLog("Waiting for ip address...", timeLeft);
        char* ip = getProcessOutput("wpa_cli status | grep ^ip_address | awk '{split($0,s,\"=\");print s[2]}'");
        if(String(ip) != ""){
            ShowInfoLog("ip address: ", ip);
            return true;
        }
        delay(1000);
    }
    return false;
}

boolean setNetwork(){
    String networkType = scanNetwork();
    if(networkType!=""){
        WriteWpaConfig();
        RestartNetworking();
        delay(1000);
        SelectNetwork();
        //we wait max 30s for IP
        if(WaitForIp(30)){
            return true;
        }

    } else {
        ShowInfoLog("Network not detected","");
    }
    return false;
}

char* getProcessOutput(char* command){
    FILE *out  = popen(command ,"r");
    if(out){
        char buffer[512];
        buffer[0] = '\0';
        fgets(buffer, sizeof(buffer), out);
        return buffer;
    }
    return "";
}
