/*
Copyright (c) 2013 - 2015, Intel Corporation

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

#ifndef IOTKIT_H
#define IOTKIT_H
#define IOTKIT_JSON_SIZE 1024
#define IOTKIT_IP 127,0,0,1
#define ARDUINO_MAC  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED 

#include "Arduino.h"
#include <EthernetUdp.h>
#include <aJSON.h>
#include <utility/pgmspace.h>

class IoTkit
{
public:
	IoTkit();

	void begin(unsigned int localport = 41235);

	// send a value for a metric. The source must have been previously registered
	int send(const char* metric, int value);
	int send(const char* metric, double value);
	int send(char* json);
	int receive(void (*f)(char*));
	int receive();
	bool checkJSON(char* json);
	int checkPacket(const char *json);

private:

		void incomingEnact(char* json); //default callback function
		int send(const char* metric, const char * value);
		
protected:

	EthernetUDP *_udp;
	IPAddress _ip;		
	byte _mac[];
	char packetBuffer;

};

#endif