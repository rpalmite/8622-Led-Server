// Mqtt
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Led
#include <FastLED.h>

// LED config
//#define DATA_PIN 5
//#define NUM_LEDS 10
//CRGB leds[NUM_LEDS];

//void flashColor(CRGB color, int delayTime);

// WiFi settings
// const char *ssid = "Win Or Die";             // Replace with your WiFi name
// const char *password = "W!n0rdi3";   // Replace with your WiFi password
const char *ssid = "RussSpot";             // Replace with your WiFi name
const char *password = "russellfi";   // Replace with your WiFi password

// MQTT Broker settings
const char *mqtt_broker = "broker.hivemq.com";  // EMQX broker endpoint
const char *mqtt_topic = "python/mqtt";     // MQTT topic
const char *mqtt_username = "";  // MQTT username for authentication
const char *mqtt_password = "";  // MQTT password for authentication
const int mqtt_port = 1883;  // MQTT port (TCP)

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void connectToWiFi();

void connectToMQTTBroker();

void mqttCallback(char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);

    // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic);
            // Publish message upon successful connection
            mqtt_client.publish(mqtt_topic, "Roger that. From ESP8266 ^^");
            // Flash red three times
            // for (int i = 0; i < 3; i++) {
            //   flashColor(CRGB::Red, 500);
            // }
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
    // Flash blue three times
    // for (int i = 0; i < 3; i++) {
    //   flashColor(CRGB::Blue, 500);
    // }
}

void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }
    mqtt_client.loop();
}


// void flashColor(CRGB color, int delayTime) {
//   // Turn on the LEDs with the specified color
//   fill_solid(leds, NUM_LEDS, color);
//   FastLED.show();
//   delay(delayTime);

//   // Turn off the LEDs
//   FastLED.clear();
//   FastLED.show();  // Ensure the strip updates
//   delay(delayTime);
// }

