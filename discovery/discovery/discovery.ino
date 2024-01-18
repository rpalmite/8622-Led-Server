/*
 Simple multicast test - receiver, which is listening for other device to announce their presence
 This code would be on the server (hub), for example

 Reference: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "NetDiscovery.h"
//#include "common.h"
#include <array>

// *** COMMON.H ***

#define SSID           "<enter SSID here>"
#define PASSWORD       "<enter password here>"
//#define DEBUG 3                  // define as minimum desired debug level, or comment out to disable debug statements

#ifdef DEBUG
#define DEBUG_MSG(L, H, M)	       if ((L) <= DEBUG) {Serial.print("DEBUG> "); Serial.print(H); Serial.print(": "); Serial.println(M);}
#else
#define DEBUG_MSG(...)            ;
#endif


NetDiscoverer discoverer = NetDiscoverer();

void setup()
{
	Serial.begin(115200);
	WiFi.begin(SSID, PASSWORD);
	while ( WiFi.status() != WL_CONNECTED ){
		delay(500);
		Serial.print(".");
	}
	Serial.println();
  
  discoverer.initDiscoveryListening();
}




/*
 simple test where the packet contains just the IP address and an ID
 loops forever - your application logic would need to determine how the loop is terminated
*/
void loop()
{
  std::array<bool, 256> senderACK;
	DEBUG_MSG(1, F("RECEIVER listening"), "");
	senderACK.fill(false);
	while ( true ) {
		discoverer.listenForDiscovery(senderACK);
		yield();
		delay(1000);
	}
}