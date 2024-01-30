// PinInfo.h
#ifndef PIN_INFO_H
#define PIN_INFO_H

#include <Arduino.h>

class PinInfo {
public:
    PinInfo(String name, int number, int inputOutputMode, boolean isDigitalPin);
    static PinInfo create(String name, int number, int inputOutputMode, boolean isDigitalPin);

    void initialize();
    String getPinName();
    boolean isOn();
    boolean isOff();
    void turnOn();
    void turnOff();
    void toggle();
    void setValue(int analogVal);
    String getName();
    int getNumber();

private:
    String pinName;
    int pinNumber;
    boolean isLow;
    boolean isOutput;
    boolean isDigital;
    int value;
    int id;
};

#endif // PIN_INFO_H
