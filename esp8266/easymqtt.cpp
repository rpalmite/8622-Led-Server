// uMQTTBroker.cpp
#include "easymqtt.h"

bool myMQTTBroker::onConnect(IPAddress addr, uint16_t client_count) {
    Serial.println(addr.toString() + " connected");
    return true;
}

void myMQTTBroker::onDisconnect(IPAddress addr, String client_id) {
    Serial.println(addr.toString() + " (" + client_id + ") disconnected");
}

bool myMQTTBroker::onAuth(String username, String password, String client_id) {
    Serial.println("Username/Password/ClientId: " + username + "/" + password + "/" + client_id);
    return true;
}

void myMQTTBroker::onData(String topic, const char *data, uint32_t length) {
    char data_str[length + 1];
    os_memcpy(data_str, data, length);
    data_str[length] = '\0';
    String dataString = (String)data_str;

    Serial.println("received topic '" + topic + "' with data '" + (String)data_str + "'");
    // printClients();

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

void myMQTTBroker::printClients() {
    for (int i = 0; i < getClientCount(); i++) {
        IPAddress addr;
        String client_id;

        getClientAddr(i, addr);
        getClientId(i, client_id);
        Serial.println("Client " + client_id + " on addr: " + addr.toString());
    }
}
