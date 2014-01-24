// IoTkit.h
// Intel IoTKit
// Libray to submit observatons to the Intel IoT cloud


#ifndef IOTKIT_H
#define IOTKIT_H

#include "Arduino.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

class IoTkit
{
public:
	IoTkit();

	// store the username and password into the config file. This only needs to be called once.
	static int storeCredentials(const char* username, const char* password);
	static int setDateTimeUtc(int year, int month, int day, int hour, int minute, int second = 0);

	void begin(unsigned int localport = 8080);
	int send(const char* source, const char* measurement, int value);
	int send(const char* source, const char* measurement, double value);
	int send(const char* source, const char* measurement, char* value);

protected:

	EthernetUDP *_udp;
	IPAddress _ip;		
};

#endif