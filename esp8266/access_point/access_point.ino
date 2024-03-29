/*


*** WORKING AP WITH MQTT BROKER ***


   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "uMQTTBroker.h"

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

uMQTTBroker mqttBroker;

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  String ipStr = WiFi.softAPIP().toString();
  String responseBody = "<h1>You are connected</h1><h3>WTF My IP Address is " + ipStr + "</h3>";
  Serial.print("My IP address: ");
  Serial.print(ipStr);
  Serial.print("Body: " + responseBody);
  // Add a custom HTTP header with the IP address
  server.sendHeader("X-BROKER-IP", ipStr);
  server.send(200, "text/html", "<h1>You are connected</h1><h3>My IP Address is </h3>");
}



// ACESS POINT CLASS
void initAccessPoint() {
  Serial.println();
  Serial.print("Configuring access point (v1)...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

// TODO rename
void startAccessPoint() {
  server.handleClient();
}

void setup() {
  delay(1000);
  Serial.begin(115200);
 
  initAccessPoint();

  // Start the broker
  Serial.println("Starting MQTT broker");
  mqttBroker.init();
}

int counter = 0;

void loop() {
  startAccessPoint();

  /*
  * Publish the counter value as String
  */
  if (counter % 20 == 0) {
    mqttBroker.publish("test/slow", (String)counter);
  }
  if (counter % 10 == 0) {
    mqttBroker.publish("test/medium", (String)counter);
  }
  if (counter % 5 == 0) {
    mqttBroker.publish("test/fast", (String)counter);
  }
  if (counter % 3 == 0) {
    mqttBroker.publish("test/superfast", (String)counter);
  }
  counter++;

  // wait a second
  delay(1000);
}
