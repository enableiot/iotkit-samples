/*
Copyright (c) 2013, Intel Corporation

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

#include <IoTkit.h>
#include <Ethernet.h>
#include <aJSON.h>
#include <utility/pgmspace.h>
#include <signal.h>  

IoTkit::IoTkit()  
{
  _udp = new EthernetUDP();
  _ip = IPAddress(IOTKIT_IP);    
  byte _mac[] = {ARDUINO_MAC};

}

void IoTkit::begin(unsigned int localport)
{

  Ethernet.begin(_mac, _ip);
  _udp->begin(localport);
  signal(SIGPIPE, SIG_IGN);  

}

// Now using https://github.com/interactive-matter/aJson

int IoTkit::send(const char* metric, int value)
{
  char buffer[128];
  int len = snprintf(buffer, 128, "%d", value);
  return send(metric, buffer);
}

int IoTkit::send(const char* metric, double value)
{
  char buffer[128];
  int len = snprintf(buffer, 128, "%f", value);
  return send(metric, buffer);
}

int IoTkit::send(const char* metric, const char * value) {
  //convert to json
  int val;
  aJsonObject* root = aJson.createObject();
  if (root == NULL) {
    return -1;
  }
  aJson.addItemToObject(root, "n", aJson.createItem(metric));
  aJson.addItemToObject(root, "v", aJson.createItem(value));
  val = send(aJson.print(root));
  aJson.deleteItem(root);
  return val;
}

int IoTkit::send(char* json)
{
  // Use udp.write to write the value.
  if (!checkJSON(json)) {
    Serial.println("Malformed JSON, not sending");
    return 1;
  }
  _udp->beginPacket(_ip, 41234);
  _udp->write(json);
  _udp->endPacket();
  return 0;
}

bool IoTkit::checkJSON(char* json) {
  aJsonObject* parsed = aJson.parse(json);
  if (parsed == NULL) {
    // invalid or empty JSON
    return false;
  }
  aJson.deleteItem(parsed);
  return true;
}

int IoTkit::checkPacket(const char *json)
{
    return (json && *json) ? 0 : 1;
}

int IoTkit::receive(void (*f)(char*)) {
  char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
  int packetSize = _udp->parsePacket();
  while (packetSize) {
    // read the packet into packetBufffer
    _udp->read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[packetSize] = NULL;
    char *done = packetBuffer;
    if (checkPacket(done) == 0) {
      if (checkJSON(done)) {
        Serial.print("Good JSON Command from Server: ");
        Serial.println(done);
        (*f)(done);
      }
      else {
        Serial.println("Bad JSON Command from server.");
      }
    }
    else {
      Serial.println("No \\n at the end of command from server, or exceeded buffer limit. ");
      Serial.println(packetSize);
    }
    packetSize = _udp->parsePacket();
  }
}

int IoTkit::receive() {
  receive(((void (*)(char*))(&IoTkit::incomingEnact)));
}

// for "set", the values are:
//
// 0: INPUT
// 1: OUTPUT
// 2: INPUT_PULLUP
//
// here is a sample JSON that (1) sets a pin to OUTPUT and (2) turns the LED on
// 
// {"set":{"pin":13,"value":1},"digital":{"write":{"pin":13,"value":true}}}  
//

void IoTkit::incomingEnact(char* json) {
  aJsonObject* parsed = aJson.parse(json);
  if (&parsed == NULL) {
    // invalid or empty JSON
    Serial.println("recieved invalid JSON");
    return;
  }
  aJsonObject* digital = aJson.getObjectItem(parsed, "digital");
  aJsonObject* analog = aJson.getObjectItem(parsed, "analog");
  aJsonObject* set = aJson.getObjectItem(parsed, "set");
  if ((set != NULL) && set->type != aJson_NULL && aJson.getObjectItem(set, "value") != NULL && aJson.getObjectItem(set, "pin") != NULL && aJson.getObjectItem(set, "pin")->valueint == 13) {
    if (aJson.getObjectItem(set, "value")->valueint == 0) {
      pinMode(aJson.getObjectItem(set, "pin")->valueint, INPUT);
    }
    if (aJson.getObjectItem(set, "value")->valueint == 1) {
      pinMode(aJson.getObjectItem(set, "pin")->valueint, OUTPUT);
    }
    if (aJson.getObjectItem(set, "value")->valueint == 2) {
      pinMode(aJson.getObjectItem(set, "pin")->valueint, INPUT_PULLUP);
    }
  }
  if ((digital != NULL) && digital->type == aJson_Object) {
    aJsonObject* write = aJson.getObjectItem(digital, "write"); //digitalWrite
    aJsonObject* read = aJson.getObjectItem(digital, "read"); //digitalRead
    if ((write != NULL) && write->type != aJson_NULL && aJson.getObjectItem(write, "pin") != NULL && aJson.getObjectItem(write, "value") != NULL && aJson.getObjectItem(write, "pin")->valueint == 13) {
      digitalWrite(aJson.getObjectItem(write, "pin")->valueint, aJson.getObjectItem(write, "value")->valuebool);
    }
    if ((read != NULL) && read->type != aJson_NULL && aJson.getObjectItem(read, "pin") != NULL) {
      digitalRead(aJson.getObjectItem(read, "pin")->valueint);
    }
  }
  if ((analog != NULL) && analog->type == aJson_Object) {
    aJsonObject* awrite = aJson.getObjectItem(analog, "write"); // analogWrite
    aJsonObject* aread = aJson.getObjectItem(analog, "read"); // analogRead
    if ((awrite != NULL) && awrite->type != aJson_NULL && aJson.getObjectItem(awrite, "pin") != NULL && aJson.getObjectItem(awrite, "value") != NULL  && aJson.getObjectItem(awrite, "pin")->valueint == 13) {
      analogWrite(aJson.getObjectItem(awrite, "pin")->valueint, aJson.getObjectItem(awrite, "value")->valueint);
    }
    if ((aread != NULL) && aread->type != aJson_NULL && aJson.getObjectItem(aread, "pin") != NULL) {
      analogRead(aJson.getObjectItem(aread, "pin")->valueint);
    }
  }
}
