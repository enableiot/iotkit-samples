/*
Copyright (c) 2013, Intel Corporation

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

#include <IoTkit.h>
#include <EthernetUdp.h>

IoTkit::IoTkit()  
{
  _udp = new EthernetUDP();
  _ip = IPAddress(127, 0, 0, 1);    
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

int IoTkit::registerMeasurement(const char* source, const char* type, const char * uom)
{
   _udp->beginPacket(_ip, 41234);
  
  _udp->write("{\"s\":\"");
  _udp->write(source);
  _udp->write("\",\"t\":\"");
  _udp->write(type);
  _udp->write("\",\"u\":\"");
  _udp->write(uom);
  _udp->write("\"}");
  
  _udp->endPacket();
}

int IoTkit::send(const char* source, int value)
{

  char buffer[128];
  int len = snprintf(buffer, 1024, "%d", value);
  send(source, buffer);
}

int IoTkit::send(const char* source, double value)
{
  char buffer[128];
  int len = snprintf(buffer, 1024, "%f", value);
  send(source, buffer);
}

int IoTkit::send(const char* source, char * value)
{
  // since the value could be any length, don't use the buffer. 
  // Instead use udp.write to write the value.

  _udp->beginPacket(_ip, 41234);
  
  _udp->write("{\"s\":\"");
  _udp->write(source);
   _udp->write("\",\"v\":");
  _udp->write(value);
  _udp->write("}");
  
  _udp->endPacket();
}
