
// NEVER DEFINE BOTH
#define BE_CLIENT
//#define BE_SERVER

#ifdef BE_CLIENT

#include <SPI.h>
//#include <WiFi101.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "sloughnet";
const char pass[] = "homebase";

//WiFiClient net;
MQTTClient client;

unsigned int lastMillis = 0;

void connect() {
 

  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  Serial.print("\nconnecting to broker...");
  while (!client.connect("arduino", "", "")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("public.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
}

#endif // BE_CLIENT

#ifdef BE_SERVER

// MainArduinoSketch.ino
#include <ESP8266WiFi.h>
// https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
#include <vector>
#include "easymqtt.h"
#include "PinInfo.h"
#include "PinArray.h"
#include "MQTT.h"
//#include "wifi.h"

const bool become_access_point = 1;

const char *ssid = "beatnet";
const char *password = "password";

WiFiServer server(80);
String header;

myMQTTBroker myBroker;

char ap_ssid[] = "beatnet";
char ap_pass[] = "password";

//MQTT_Client mqttClient;

void startWiFiAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

PinArray pins;

int blue = 0;
int green = 1;
int red = 2;
int color = 0;

void onBeat(String topic, String data) {
  Serial.println("$$$$ blue $$$$");
  if (color == blue) {
    pins.get(0).turnOff();
    pins.get(1).turnOff();
    pins.get(2).turnOn();
    color++;
  } else if (color == green) {
    pins.get(0).turnOn();
    pins.get(1).turnOff();
    pins.get(2).turnOff();
    color++;
  } else if (color == red) {
    pins.get(0).turnOff();
    pins.get(1).turnOn();
    pins.get(2).turnOff();
    color++;
  } else {
    pins.get(0).turnOff();
    pins.get(1).turnOff();
    pins.get(2).turnOff();
    color = 0;
  }

  //delay(1000);
  //pins.get(2).turnOff();
}

///// MQTT CLIENT //////

#define MQTT_KEEPALIVE    120  /*second*/
#define MQTT_CLEAN_SESSION 1
MQTT_Client mqttClient;

static void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status)
{
  if (status == STATION_GOT_IP) {
    MQTT_Connect(&mqttClient);
  } else {
    MQTT_Disconnect(&mqttClient);
  }
}

static void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  Serial.println("MQTT: Published");
}

void mqqtClientSetup() {
  //MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT, DEFAULT_SECURITY);
  uint8_t ipAddress[] = {192, 168, 4, 1};
  Serial.println("Initiating MQTT connetion");
  MQTT_InitConnection(&mqttClient, ipAddress, 1880, 0);
  Serial.println("Initiated MQTT connetion");

  //uint8_t clientID[] = {192, 168, 4, 1};
  // Define a static uint8_t array with the ASCII values of "CLIENT_1234"
  uint8_t clientID[] = {67, 76, 73, 69, 78, 84, 95, 49, 50, 51, 52};
  uint8_t user[] = {};
  uint8_t pass[] = {};
  if ( !MQTT_InitClient(&mqttClient, clientID, user, pass, MQTT_KEEPALIVE, MQTT_CLEAN_SESSION) )
  {
    Serial.println("Failed to initialize properly. Check MQTT version.\r\n");
    return;
  } else {
    Serial.println("I think I connected to the MQTT broker");
    MQTT_Publish(&mqttClient, "hello", "this is my hello", 16, 0, 0);
  }

  uint8_t lwtArray[] = {47, 108, 119, 116};
  uint8_t offlineArray[] = {111, 102, 102, 108, 105, 110, 101};
  Serial.println("Init LWT");

  MQTT_InitLWT(&mqttClient, lwtArray, offlineArray, 0, 0);
  //MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
  //MQTT_OnConnected(&mqttClient, mqttConnectedCb);
  //MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
  Serial.println("Setting up onpublish");

  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  //MQTT_OnData(&mqttClient, mqttDataCb);

  //WIFI_Connect(ssid, password, wifiConnectCb);
  MQTT_Publish(&mqttClient, "hello", "this is my hello", 16, 0, 0);

}

void mqqtClientLoop() {
  Serial.println("publishing {\"value\":\"100\"}...");

  MQTT_Publish(&mqttClient, "/beat", "{\"value\":\"100\"}", 15, 0, 0);
}

//////////////////////

void setup() {
  Serial.begin(115200);

  Serial.write("MQTT SERVER/BROKER STARTING");

  pins.push(PinInfo::create("Pin1", 5, OUTPUT, true));
  pins.push(PinInfo::create("Pin2", 4, OUTPUT, true));
  pins.push(PinInfo::create("Pin3", 0, OUTPUT, true));
  pins.push(PinInfo::create("Pin4", 2, OUTPUT, true));
  pins.push(PinInfo::create("Pin12", 12, OUTPUT, true));
  pins.push(PinInfo::create("Pin13", 13, OUTPUT, true));
  pins.push(PinInfo::create("Pin16", 16, OUTPUT, true));

  if (become_access_point) {
    startWiFiAP();
    myBroker.init();
    myBroker.subscribe("#");
    myBroker.addSubscription("#", onBeat);
  } else {
    mqqtClientSetup();
    initWebserver();
  }
}

void loop() {
  delay(500);
  client_status();
  if (!become_access_point) {
    //mqqtClientLoop();
  }
  delay(500);
}

void client_status() {
  // Your client status code here
}

void initWebserver() {
  Serial.print("Connecting to WiFi network ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  handleWebRequests();
}

void handleWebRequests() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            handleAPIRequests();
            handleHTMLPage();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void handleAPIRequests() {
  for (int i = 0; i < pins.size(); i++) {
    PinInfo pin = pins.get(i);
    String pinName = pin.getName();
    if (header.indexOf("GET /" + pinName + "/on") >= 0) {
      Serial.println("*** " + pinName + " ON ***");
      pin.turnOn();
    } else if (header.indexOf("GET /" + pinName + "/off") >= 0) {
      Serial.println("*** " + pinName + " OFF ***");
      pin.turnOff();
    } else if (header.indexOf("GET /favicon.ico") >= 0) {
      Serial.println("Ignoring Favicon");
    } else if (header.indexOf("GET /broker") >= 0) {
      Serial.println("setting broker");
    } else {
      Serial.println("*** Unrecognized API route ***");
    }
  }
}

void handleHTMLPage() {
  // Your HTML page handling code here
}



////////// MQTT CLIENT /////////////

/*DEFAULT CONFIGURATIONS*/

//#define MQTT_HOST     "192.168.0.101" //or "mqtt.yourdomain.com"
//#define MQTT_PORT     1883
//#define MQTT_BUF_SIZE   1024


//#define MQTT_CLIENT_ID    "CLIENT_1234"
//#define MQTT_USER     "USER"
//#define MQTT_PASS     "PASS"

//#define MQTT_KEEPALIVE 120

//#define STA_SSID "SSID"
//#define STA_PASS "PASS"

//#define MQTT_RECONNECT_TIMEOUT  5 /*second*/

//#define DEFAULT_SECURITY  0
//#define QUEUE_BUFFER_SIZE       2048



/*
#include "ets_sys.h"
//#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"

static void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status)
{
  if (status == STATION_GOT_IP) {
    MQTT_Connect(&mqttClient);
  } else {
    MQTT_Disconnect(&mqttClient);
  }
}
static void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Connected\r\n");
  MQTT_Subscribe(client, "/mqtt/topic/0", 0);
  MQTT_Subscribe(client, "/mqtt/topic/1", 1);
  MQTT_Subscribe(client, "/mqtt/topic/2", 2);

  MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);
  MQTT_Publish(client, "/mqtt/topic/1", "hello1", 6, 1, 0);
  MQTT_Publish(client, "/mqtt/topic/2", "hello2", 6, 2, 0);

}

static void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Disconnected\r\n");
}

static void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Published\r\n");
}

static void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
  char *topicBuf = (char*)os_zalloc(topic_len + 1),
        *dataBuf = (char*)os_zalloc(data_len + 1);

  MQTT_Client* client = (MQTT_Client*)args;
  os_memcpy(topicBuf, topic, topic_len);
  topicBuf[topic_len] = 0;
  os_memcpy(dataBuf, data, data_len);
  dataBuf[data_len] = 0;
  INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
  os_free(topicBuf);
  os_free(dataBuf);
}

void ICACHE_FLASH_ATTR print_info()
{
  INFO("\r\n\r\n[INFO] BOOTUP...\r\n");
  INFO("[INFO] SDK: %s\r\n", system_get_sdk_version());
  INFO("[INFO] Chip ID: %08X\r\n", system_get_chip_id());
  INFO("[INFO] Memory info:\r\n");
  system_print_meminfo();

  INFO("[INFO] -------------------------------------------\n");
  INFO("[INFO] Build time: %s\n", BUID_TIME);
  INFO("[INFO] -------------------------------------------\n");

}


static void ICACHE_FLASH_ATTR app_init(void)
{
  uart_init(BIT_RATE_115200, BIT_RATE_115200);
  print_info();
  MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT, DEFAULT_SECURITY);
  //MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

  if ( !MQTT_InitClient(&mqttClient, MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS, MQTT_KEEPALIVE, MQTT_CLEAN_SESSION) )
  {
    INFO("Failed to initialize properly. Check MQTT version.\r\n");
    return;
  }
  //MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
  MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
  MQTT_OnConnected(&mqttClient, mqttConnectedCb);
  MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  MQTT_OnData(&mqttClient, mqttDataCb);

  WIFI_Connect(STA_SSID, STA_PASS, wifiConnectCb);
}
void user_init(void)
{
  system_init_done_cb(app_init);
}
*/

#endif // BE_SERVER