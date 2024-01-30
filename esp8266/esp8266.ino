// THIS IS THE ARDUINO IDE VERSION

#include <ESP8266WiFi.h>
#include <vector>
//#include <algorithm>
//#include "NetDiscovery.h"
#include "uMQTTBroker.h"

const bool become_access_point = 1;

const char* ssid = "sloughnet";
const char* password = "homebase";
//const char* ssid = "CenturyLink8158";
//const char* password = "r5hpj9j9st5bbr";

// Set up a web server on port 80
WiFiServer server(80);

// HTTP Variable to store requests
String header;


///////////// EASY MQTT //////////

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
      String dataString = (String)data_str;

      Serial.println("received topic '"+topic+"' with data '"+(String)data_str+"'");
      //printClients();

      if (topic == "beat") {
        if (dataString == "kick") {
           Serial.println("blue");
           pins.get(0).turnOff();
           pins.get(1).turnOff();
           pins.get(2).turnOn();
           delay(1000);
           pins.get(2).turnOff();
        }
      }
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


myMQTTBroker myBroker; // TODO rename mqttBroker

////// EASY ACCESS POINT //////

char ap_ssid[] = "beatnet";     // your network SSID (name)
char ap_pass[] = "password"; // your network password

void startWiFiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

////////////// EASY IO ///////////////

// TODO class PinArray
// set transition speed
// crawl
// ant walk
// christmas lights

// TODO rename Pin
class PinInfo {
public:
  // Constructor // TODO rename pinNumber to just number // TODO should pin number and pin mode be uint8_t ???
  PinInfo(String name, int number, int inputOutputMode, boolean isDigitalPin) {
    pinName = name;
    pinNumber = number;
    isLow = true;
    isOutput = (inputOutputMode == OUTPUT);
    isDigital = isDigitalPin;
    id = random(0, 1000);
    initialize();
  }

  // Static method to create a new PinInfo instance
  static PinInfo create(String name, int number, int inputOutputMode, boolean isDigitalPin) {
    return PinInfo(name, number, inputOutputMode, isDigitalPin);
  }

  void initialize() {
    if (isOutput) {
      pinMode(pinNumber, OUTPUT);
    } else {
      pinMode(pinNumber, INPUT);
    }
    if (isDigital) {
      turnOn();
    } else {

    }
  }

  // Member function to update the state of the pin
  //void updateState() {
  //  digitalRead(pinNumber);
  //}

  // Member function to get the pin name
  String getPinName() {
    return pinName;
  }

  boolean isOn() {
    return !isLow;
  }

  boolean isOff() {
    return isLow;
  }

  // TODO keep track if analog pin
  void turnOn() {
    Serial.println("Pin (" + String(id) + ") " + pinName + " (gpio " + pinNumber + ") on");
    // TODO if (isDigital) { } else if (analiog) { analogWrite() }
    digitalWrite(pinNumber, HIGH);
    isLow = false;
  }

  // TODO keep track if analog pin
  void turnOff() {
    Serial.println("Pin (" + String(id) + ") " + pinName + " (gpio " + pinNumber + ") off");
    digitalWrite(pinNumber, LOW);
    isLow = true;
  }

  void toggle() {
    if (isOn()) {
      turnOff();
    } else {
      turnOn();
    }
  }

  void setValue(int analogVal) {
    Serial.println("Pin " + pinName + "(gpio " + pinNumber + ") set to " + analogVal);
    analogWrite(pinNumber, analogVal);
    if (analogVal > 0) { // TODO single line this statement
      isLow = false;
    } else {
      isLow = true;
    }
    //return 0;
  }

  String getName() {
    return pinName;
  }

  int getNumber() {
    return pinNumber;
  }

private:
  String pinName;
  int pinNumber;
  boolean isLow;
  boolean isOutput;
  boolean isDigital;
  int value;
  int id;
};

class PinArray {
public:
    
  std::vector<PinInfo> pins;
  std::vector<int> states;

  PinArray() {
      
  }

  void addPin(const PinInfo& pin) {
      pins.push_back(pin);
      states.push_back(0);
  }

  int size() {
    int sizeOfPinArray = pins.size();
    return sizeOfPinArray;
  }

  PinInfo get(int position) {
    if (position < size()) {
        return pins[position];
    }
    // TODO handle the case where the position is out of bounds
    // or return a default PinInfo object in such cases.
    return PinInfo::create("", 0, 0, true);
  }
  
  void push(const PinInfo& newPin) {
    addPin(newPin);
  }
  
  PinArray getPinsNear(int position, int numberOfPins, boolean loopAround) {
    PinArray pinsNearPosition = PinArray();

    for (int i=0; i<numberOfPins; i++) {
      int positionOfPinToFetch = (position+i) % pins.size(); // TODO possible off by one error (might need to be size - 1)
      PinInfo pinToFetech = pins[positionOfPinToFetch];
      pinsNearPosition.addPin(pinToFetech);
    }

    return pinsNearPosition;
  }

private:
  int currentPosition = 0;
};



// TODO rename RandomPinEffect
class RandomPinEffect {
  public:
    void doEffect(PinArray pins, int step) {
      // TODO for safety later
      // int randomNumber = min(RAND_MAX, pins.size());
      int randomNumber = random(1, pins.size());
      PinInfo randomPin = pins.get(randomNumber);
      randomPin.turnOn();
    }
};

// TODO rename RandomPinEffect
class RainbowAntEffect {
  int defaultEffectSize = 5;
  public:
    // void selection(); // aka get pins
    // void transition(); // aka get old pins (step-1)
    // void apply(); // aka change color
    void doEffect(PinArray pins, int step) {
      int effectSize = min(defaultEffectSize+1, pins.size()); // +1 for last pin to turn off
      
      if (effectSize < 1) {
        return;
      }

      PinArray pinArray = pins.getPinsNear(step, effectSize, true);
      pinArray.get(0).turnOff();
      for (int i=1; i<effectSize; i++) {
        pinArray.get(i).turnOn();
      }
    }
};


PinArray pins = PinArray();


// TODO is this still needed?
const int led14 = 14;  // D14?

// TODO put this inside a 
int getParameterValue(String queryString, String parameterName) {
  // Find the position of the parameter name in the query string
  int paramPos = queryString.indexOf(parameterName);

  // Check if the parameter is found in the query string
  if (paramPos != -1) {
    // Find the position of the equal sign after the parameter name
    int equalPos = queryString.indexOf("=", paramPos);

    // Find the position of the ampersand or end of the string after the equal sign
    int ampersandPos = queryString.indexOf("&", equalPos);

    // Extract the substring between the equal sign and the ampersand (or end of the string)
    String paramValue = queryString.substring(equalPos + 1, ampersandPos);

    // Convert the parameter value to an integer
    return paramValue.toInt();
  } else {
    // Return a default value if the parameter is not found
    return -1;
  }
}

/*
// Declare the number of pins
const int NUM_PINS = 7;

// Declare the array of PinInfo objects globally
// Pins used to control relays
//const int relay1 = 5;  // D1
//const int relay2 = 4;  // D2
//const int relay3 = 0;  // D3
//const int relay4 = 2;  // D4
PinInfo pinArray[NUM_PINS] = {
  PinInfo("relay1", 5, OUTPUT, true),
  PinInfo("relay2", 4, OUTPUT, true),
  PinInfo("relay3", 0, OUTPUT, true),
  PinInfo("relay4", 2, OUTPUT, true),
  PinInfo("Pin12", 12, OUTPUT, true),
  PinInfo("Pin13", 13, OUTPUT, true),
  PinInfo("Pin16", 16, OUTPUT, true)
};
*/

// Global Variables
//NetDiscoverer discoverer = NetDiscoverer();

// TODO WebServer.init()
void initWebserver() {
  // Connect to WiFi
  Serial.print("Connecting to WiFi network");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  // We will connect with the entered name and password
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  //IP address will be shown

  // Start the webserver
  server.begin();
}

// TODO WebServer.handleRequest()
void handleWebRequests() {
  // get client requests
  WiFiClient client = server.available();  // Web server starts

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        header += c;
        if (c == '\n') {

          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //The PIN will be activated according to the received requests
            for (int i = 0; i < pins.size(); i++) {
              PinInfo pin = pins.get(i);
              String pinName = pin.getName();
              String pinNum = String(pin.getNumber());
              if (header.indexOf("GET /" + pinName + "/on") >= 0) {
                Serial.println("*** " + pinNum + " ON ***");
                pin.turnOn();
              } else if (header.indexOf("GET /" + pinName + "/off") >= 0) {
                Serial.println("*** " + pinNum + " OFF ***");
                pin.turnOff();
              } else if (header.indexOf("GET /favicon.ico") >= 0) {
                Serial.println("Ignoring Favicon");
              } else if (header.indexOf("GET /broker") >= 0) {
                 Serial.println("setting broker");
                 //String brokerIP = getParameterValue(header, "ip")
              } else {
                Serial.println("*** Unrecognized API route ***");
              }
            }

            // HTML Page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>WebServer based Device Controller</title>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");

            // You can customize the values ​​according to your needs
            client.println("<style>html, body {background-color: #e0ebcd; font-family: Helvetica; display: block; color: #1336dd; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #30f040; color: black; padding: 12px 40px;");
            client.println("text-decoration: none; font-size: 20px; margin: auto; cursor: pointer;}");
            client.println(".button2 {background-color: red; color: black; padding: 12px 40px;}");
            client.println(".textbox {width: 80px; padding: 16px 20px 0px 24px; background-image: linear-gradient(180deg, #fff, #ddd 40%, #ccc);}");
            client.println(".mytext {font-size: 16px; font-weight:bold; font-family:Arial ; text-align: center;}");
            client.println(".headertext{ font-weight:bold; font-family:Arial ; text-align: center; color: brown ;}");
            client.println("#container {width: 100%; height: 100%; margin-left: 5px; margin-top: 20px; padding: 10px; display: -webkit-flex; -webkit-justify-content: center; display: flex; justify-content: center;} ");
            client.println("</style></head>");
            client.println("<body><h1><u><div class = \"headertext\"> WebServer based Device Controller</u></h1>");

            // The elements inside the web page are defined
            for (int i = 0; i < pins.size(); i++) {
              client.println("<div id=\"container\">");
              int deviceNum = i + 1;
              client.print("<p><div class=\"textbox mytext\">DEVICE ");
              client.print(deviceNum);
              client.println("</div>");

              PinInfo pin = pins.get(i);
              String pinName = pin.getName();
              boolean pinIsOn = pin.isOn();

              if (pinIsOn) {
                client.print("<a href=\"/");
                client.print(pinName);
                client.println("/on\"><button class=\"button\">TURN ON</button></a></p>");
              } else {
                client.print("<a href=\"/");
                client.print(pinName);
                client.println("/off\"><button class=\"button button2\">TURN OFF</button></a></p>");
              }

              client.println("</div>");
            }

            client.println("</body></html>");

            client.println();

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

void setup() {
  // this board's serial speed???
  Serial.begin(115200);

  //initWebserver();

  // Initialize the PinInfo objects
  pins.push(PinInfo::create("Pin1", 5, OUTPUT, true));
  pins.push(PinInfo::create("Pin2", 4, OUTPUT, true));
  pins.push(PinInfo::create("Pin3", 0, OUTPUT, true));
  pins.push(PinInfo::create("Pin4", 2, OUTPUT, true));
  pins.push(PinInfo::create("Pin12", 12, OUTPUT, true));
  pins.push(PinInfo::create("Pin13", 13, OUTPUT, true));
  pins.push(PinInfo::create("Pin16", 16, OUTPUT, true));

  // initiate discovery of devices
  //discoverer.initDiscoveryListening(); //TODO delete this line
  if (become_access_point) {
     startWiFiAP();
  } else {
     initWebserver();
  }
  myBroker.init();
  myBroker.subscribe("#");
}

void loop() {
    // led
  //pinMode(led14, OUTPUT);
  // led
  //analogWrite(led14, 20);

  //handleWebRequests();

  delay(5000);
  client_status();
  delay(4000);
  
  // // white
  // Serial.println("white");
  // pins.get(0).turnOn();
  // pins.get(1).turnOn();
  // pins.get(2).turnOn();
  // delay(1000);

  // // off
  // Serial.println("off");
  // pins.get(0).turnOff();
  // pins.get(1).turnOff();
  // pins.get(2).turnOff();
  // delay(1000);

  // //blue
  // Serial.println("blue");
  // pins.get(0).turnOff();
  // pins.get(1).turnOff();
  // pins.get(2).turnOn();
  // delay(1000);

  // // red
  // Serial.println("red");
  // pins.get(0).turnOn();
  // pins.get(1).turnOff();
  // pins.get(2).turnOff();
  // delay(1000);

  // // green
  // Serial.println("blue");
  // pins.get(0).turnOff();
  // pins.get(1).turnOn();
  // pins.get(2).turnOff();
  // delay(1000);
}



void client_status() {
  unsigned char number_client;
  struct station_info *stat_info;
  
  struct ip_addr *IPaddress;
  IPAddress address;
  int i=1;
  
  number_client= wifi_softap_get_station_num();
  stat_info = wifi_softap_get_station_info();
  
  Serial.print(" Total Connected Clients are = ");
  Serial.println(number_client);
  
    while (stat_info != NULL) {
      //Serial.print(" IP adress is = ");
      //Serial.print(&stat_info->ip);
      //Serial.print("end ip");
      IPaddress = (struct ip_addr*)&stat_info->ip;
      address = IPaddress->addr;
      
      Serial.print("client= ");
      
      Serial.print(i);
      Serial.print(" IP adress is = ");
      Serial.print((address));
      Serial.print(" with MAC adress is = ");
      Serial.print(stat_info->bssid[0],HEX);
      Serial.print(" ");
      Serial.print(stat_info->bssid[1],HEX);
      Serial.print(" ");
      Serial.print(stat_info->bssid[2],HEX);
      Serial.print(" ");
      Serial.print(stat_info->bssid[3],HEX);
      Serial.print(" ");
      Serial.print(stat_info->bssid[4],HEX);
      Serial.print(" ");
      Serial.print(stat_info->bssid[5],HEX);
      Serial.print(" ");

      stat_info = STAILQ_NEXT(stat_info, next);
      i++;
      Serial.println();

      /*
      IPaddress = &stat_info->ip;
      address = IPaddress->addr;
      
      Serial.print("client= ");
      
      Serial.print(i);
      Serial.print(" IP adress is = ");
      Serial.print((address));
      Serial.print(" with MAC adress is = ");
      
      Serial.print(stat_info->bssid[0],HEX);Serial.print(" ");
      Serial.print(stat_info->bssid[1],HEX);Serial.print(" ");
      Serial.print(stat_info->bssid[2],HEX);Serial.print(" ");
      Serial.print(stat_info->bssid[3],HEX);Serial.print(" ");
      Serial.print(stat_info->bssid[4],HEX);Serial.print(" ");
      Serial.print(stat_info->bssid[5],HEX);Serial.print(" ");
      
      stat_info = STAILQ_NEXT(stat_info, next);
      i++;
      Serial.println();
      */
    }
    
  delay(500);
}
