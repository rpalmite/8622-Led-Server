// PinInfo.cpp
#include "PinInfo.h"

PinInfo::PinInfo(String name, int number, int inputOutputMode, boolean isDigitalPin) {
    pinName = name;
    pinNumber = number;
    isLow = true;
    isOutput = (inputOutputMode == OUTPUT);
    isDigital = isDigitalPin;
    id = random(0, 1000);
    initialize();
}

PinInfo PinInfo::create(String name, int number, int inputOutputMode, boolean isDigitalPin) {
    return PinInfo(name, number, inputOutputMode, isDigitalPin);
}

void PinInfo::initialize() {
    if (isOutput) {
        pinMode(pinNumber, OUTPUT);
    } else {
        pinMode(pinNumber, INPUT);
    }
    if (isDigital) {
        turnOn();
    }
}

String PinInfo::getPinName() {
    return pinName;
}

boolean PinInfo::isOn() {
    return !isLow;
}

boolean PinInfo::isOff() {
    return isLow;
}

void PinInfo::turnOn() {
    Serial.println("Pin (" + String(id) + ") " + pinName + " (gpio " + pinNumber + ") on");
    digitalWrite(pinNumber, HIGH);
    isLow = false;
}

void PinInfo::turnOff() {
    Serial.println("Pin (" + String(id) + ") " + pinName + " (gpio " + pinNumber + ") off");
    digitalWrite(pinNumber, LOW);
    isLow = true;
}

void PinInfo::toggle() {
    if (isOn()) {
        turnOff();
    } else {
        turnOn();
    }
}

void PinInfo::setValue(int analogVal) {
    Serial.println("Pin " + pinName + "(gpio " + pinNumber + ") set to " + analogVal);
    analogWrite(pinNumber, analogVal);
    isLow = (analogVal <= 0);
}

String PinInfo::getName() {
    return pinName;
}

int PinInfo::getNumber() {
    return pinNumber;
}
