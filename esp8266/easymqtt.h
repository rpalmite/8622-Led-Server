// easymqtt.h
#ifndef U_MQTT_BROKER_H
#define U_MQTT_BROKER_H

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"

class myMQTTBroker : public uMQTTBroker {
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count);
    virtual void onDisconnect(IPAddress addr, String client_id);
    virtual bool onAuth(String username, String password, String client_id);
    virtual void onData(String topic, const char *data, uint32_t length);
    virtual void printClients();
};

#endif // U_MQTT_BROKER_H
