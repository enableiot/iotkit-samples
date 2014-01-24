// Store the cloud id and password onto the SD card.
// This only needs to be done once for each SD card

#include <Ethernet.h>	// must be included to use IoTkit
#include <IoTkit.h>		// include IoTkit.h to use the Intel IoT Kit

void setup() {
  IoTkit::storeCredentials("the-username", "the-password");
}

void loop() {
}