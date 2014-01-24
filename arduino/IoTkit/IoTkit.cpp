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
  char * str = "{\n\t\"username\": \"%s\",\n\t\"password\": \"%s\"\n}";
  snprintf(buffer, 1024, str, username, password);

  FILE *fp;
  fp = fopen("/home/root/iotkit-agent/config.json", "w");
  fputs(buffer, fp);
  fclose(fp);
}

int IoTkit::setDateTimeUtc(int year, int month, int day, int hour, int minute, int second)
{
  char buffer[128];
  snprintf(buffer, 1024, "date %d.%d.%d-%d:%d:%d", year, month, day, hour, minute, second);
  system(buffer);
}

void IoTkit::begin(unsigned int localport)
{
  _udp->begin(localport);
}

int IoTkit::send(const char* source, const char* measurement, int value)
{

  char buffer[128];
  int len = snprintf(buffer, 1024, "%d", value);
  send(source, measurement, buffer);
}

int IoTkit::send(const char* source, const char* measurement, double value)
{
  char buffer[128];
  int len = snprintf(buffer, 1024, "%f", value);
  send(source, measurement, buffer);
}

int IoTkit::send(const char* source, const char* measurement, char * value)
{
  // since the value could be any length, don't use the buffer. 
  // Instead use udp.write to write the value.

  _udp->beginPacket(_ip, 41234);
  
  _udp->write("{\"s\":\"");
  _udp->write(source);
  _udp->write("\",\"m\":\"");
  _udp->write(measurement);
  _udp->write("\",\"v\":");
  _udp->write(value);
  _udp->write("}");
  
  _udp->endPacket();
}
