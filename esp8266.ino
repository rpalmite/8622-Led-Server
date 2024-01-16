#include <ESP8266WiFi.h>
#include <vector>

const char* ssid = "sloughnet";
const char* password = "homebase";
/*
const char* ssid = "CenturyLink8158";
const char* password = "r5hpj9j9st5bbr";
*/

// Set up a web server port
WiFiServer server(80);

// HTTP Variable to store requests
String header;

// TODO class PinArray
// set transition speed
// crawl
// ant walk
// christmas lights
// 

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

    PinArray() {
        
    }

    void addPin(const PinInfo& pin) {
        pins.push_back(pin);
    }

    void step() {
      
    }

};






const int led14 = 14;  // D14?


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

void setup() {
  Serial.begin(115200);
  //pinSetup();

  // led
  pinMode(led14, OUTPUT);

  // Set the relays to on by default
  //digitalWrite(relay1, HIGH);
  //digitalWrite(relay2, HIGH);
  //digitalWrite(relay3, HIGH);
  //digitalWrite(relay4, HIGH);

  // led
  analogWrite(led14, 100);


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

  // Initialize the PinInfo objects
  // pinArray[0] = PinInfo::create("Pin1", 2, OUTPUT, true);
  // pinArray[1] = PinInfo::create("Pin2", 3, INPUT, true);
  // pinArray[2] = PinInfo::create("Pin3", 4, OUTPUT, true);
  // pinArray[3] = PinInfo::create("Pin4", 5, INPUT, true);
  // pinArray[4] = PinInfo::create("Pin12", 12, OUTPUT, true);
  // pinArray[5] = PinInfo::create("Pin13", 13, OUTPUT, true);
  // pinArray[6] = PinInfo::create("Pin16", 16, OUTPUT, true);
  // pinArray[7] = PinInfo::create("Pin4", 5, OUTPUT, true);
}

void loop() {
  // Declare the number of pins
  //const int NUM_PINS = 8;

  // Declare the array of PinInfo objects
  /*PinInfo pinArray[NUM_PINS] = { }; // String name, int number, int inputOutputMode, boolean isDigitalPin
  pinArray[0] = PinInfo::create("Pin1", 2, OUTPUT, true);
  pinArray[1] = PinInfo::create("Pin2", 3, INPUT, true);
  pinArray[2] = PinInfo::create("Pin3", 4, OUTPUT, false);
  pinArray[3] = PinInfo::create("Pin4", 5, INPUT, false);

  PinInfo pinArray[NUM_PINS] = {
    PinInfo("Pin1", 2, OUTPUT, true),
    PinInfo("Pin2", 3, INPUT, true),
    PinInfo("Pin3", 4, OUTPUT, true),
    PinInfo("Pin4", 5, INPUT, true)
  };
  */
  /*
  PinInfo pinArray[NUM_PINS] = {
    PinInfo("Pin1", 2, OUTPUT, true),
    PinInfo("Pin2", 3, INPUT, true),
    PinInfo("Pin3", 4, OUTPUT, true),
    PinInfo("Pin4", 5, INPUT, true),
    PinInfo("Pin12", 12, OUTPUT, true),
    PinInfo("Pin13", 13, OUTPUT, true),
    PinInfo("Pin16", 16, OUTPUT, true),
    PinInfo("Pin4", 5, OUTPUT, true)
  };
  */

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
            for (int i = 0; i < NUM_PINS; i++) {
              PinInfo pin = pinArray[i];
              String pinName = pin.getName();
              String pinNum = String(pin.getNumber());
              if (header.indexOf("GET /" + pinName + "/on") >= 0) {
                // if (i==3) {
                //   pin.setValue(100);
                // } else {
                //   pin.turnOn();
                // }
                Serial.println("*** " + pinNum + " ON ***");
                pin.turnOn();
              } else if (header.indexOf("GET /" + pinName + "/off") >= 0) {
                // if (i==3) {
                //   pin.setValue(10);
                // } else {
                //   pin.turnOff();
                // }
                Serial.println("*** " + pinNum + " OFF ***");
                pin.turnOff();
              } else if (header.indexOf("GET /favicon.ico") >= 0) {
                Serial.println("Ignoring Favicon");
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
            for (int i = 0; i < NUM_PINS; i++) {
              client.println("<div id=\"container\">");
              int deviceNum = i + 1;
              client.print("<p><div class=\"textbox mytext\">DEVICE ");
              client.print(deviceNum);
              client.println("</div>");

              PinInfo pin = pinArray[i];
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
