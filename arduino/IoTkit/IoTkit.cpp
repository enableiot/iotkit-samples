#include <IoTkit.h>
#include <EthernetUdp.h>

IoTkit::IoTkit()  
{
  _udp = new EthernetUDP();
  _ip = IPAddress(127, 0, 0, 1);    
}


int IoTkit::storeCredentials(const char* username, const char* password)
{
  char buffer[1024];
  char * str = "printf \"{\n\t\\\"username\\\": \\\"%s\\\",\n\t\\\"password\\\": \\\"%s\\\"\n}\" > /home/root/iotkit-agent/config.json";
  int len = snprintf(buffer, 1024, str, username, password);

  system(buffer);
}

void IoTkit::begin(unsigned int localport)
{
  _udp->begin(localport);
}

int IoTkit::send(const char* source, const char* measurement, int value)
{

  char buffer[1024];
  int len = snprintf(buffer, 1024, "{ \"s\": \"%s\", \"m\": \"%s\", \"v\": %d }", 
                      source, measurement, value);

  _udp->beginPacket(_ip, 41234);
  _udp->write((const uint8_t *)buffer, len); 
  _udp->endPacket();
}

int IoTkit::send(const char* source, const char* measurement, double value)
{

  char buffer[1024];
  int len = snprintf(buffer, 1024, "{ \"s\": \"%s\", \"m\": \"%s\", \"v\": %f }",
                      source, measurement, value);

  _udp->beginPacket(_ip, 41234);
  _udp->write((const uint8_t *)buffer, len); 
  _udp->endPacket();
}

int IoTkit::send(const char* source, const char* measurement, char * value)
{
  // since the value could be any length, don't use the buffer. 
  // Instead use udp.write to write the value.

  char buffer[1024];
  int len = snprintf(buffer, 1024, "{ \"s\": \"%s\", \"m\": \"%s\", \"v\": \"",
                      source, measurement);

  _udp->beginPacket(_ip, 41234);
  _udp->write((const uint8_t *)buffer, len); 
  _udp->write(value); 
  _udp->write("\"}");
  _udp->endPacket();
}
