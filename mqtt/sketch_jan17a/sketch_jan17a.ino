/*
 * uMQTTBroker demo for Arduino (C++-style)
 * 
 * The program defines a custom broker class with callbacks, 
 * starts it, subscribes locally to anything, and publishs a topic every second.
 * Try to connect from a remote client and publish something - the console will show this as well.
 */

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
#include <ESP8266WebServer.h>

/*
 * Your WiFi config here
 */
char ssid[] = "sloughnet";     // your network SSID (name)
char pass[] = "homebase"; // your network password


char ap_ssid[] = "basenet";
char ap_pass[] = "house";

bool WiFiAP = true;      // Do yo want the ESP as AP?

/*
 * Custom broker class with overwritten callback functions
 */
class myMQTTBroker: public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }

    virtual void onDisconnect(IPAddress addr, String client_id) {
      Serial.println(addr.toString()+" ("+client_id+") disconnected");
    }

    virtual bool onAuth(String username, String password, String client_id) {
      Serial.println("Username/Password/ClientId: "+username+"/"+password+"/"+client_id);
      return true;
    }
    
    virtual void onData(String topic, const char *data, uint32_t length) {
      char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      
      Serial.println("received topic '"+topic+"' with data '"+(String)data_str+"'");
      //printClients();
    }

    // Sample for the usage of the client info methods

    virtual void printClients() {
      for (int i = 0; i < getClientCount(); i++) {
        IPAddress addr;
        String client_id;
         
        getClientAddr(i, addr);
        getClientId(i, client_id);
        Serial.println("Client "+client_id+" on addr: "+addr.toString());
      }
    }
};

myMQTTBroker myBroker;
ESP8266WebServer server(80);

/*
 * WiFi init stuff
 */
void startWiFiClient()
{
  Serial.println("Connecting to "+(String)ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWiFiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Start WiFi
  if (WiFiAP)
    startWiFiAP();
  else
    startWiFiClient();

  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();

/*
 * Subscribe to anything
 */
  myBroker.subscribe("#");
}

int counter = 0;

void loop()
{
  /*
  * Publish the counter value as String
  */
  if (counter % 20 == 0) {
    myBroker.publish("broker/counter", (String)counter);
    myBroker.publish("test/slow", (String)counter);
  }
  if (counter % 10 == 0) {
    myBroker.publish("test/medium", (String)counter);
  }
  if (counter % 5 == 0) {
    myBroker.publish("test/fast", (String)counter);
  }
  if (counter % 3 == 0) {
    myBroker.publish("test/superfast", (String)counter);
  }
  counter++;

  // wait a second
  delay(1000);
}