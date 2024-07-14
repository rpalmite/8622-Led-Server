// PinArray.h
#ifndef PIN_ARRAY_H
#define PIN_ARRAY_H

#include <vector>
#include "PinInfo.h"

// TODO rename PinSet
class PinArray {
public:
    std::vector<PinInfo> pins;
    std::vector<int> states;

    PinArray();

    void addPin(const PinInfo &pin);
    int size();
    PinInfo get(int position);
    void push(const PinInfo &newPin);
    PinArray getPinsNear(int position, int numberOfPins, boolean loopAround);

private:
    int currentPosition = 0;
};

#endif // PIN_ARRAY_H
