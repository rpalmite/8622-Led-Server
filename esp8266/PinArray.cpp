// PinArray.cpp
#include "PinArray.h"

PinArray::PinArray() {
    // Constructor implementation
}

void PinArray::addPin(const PinInfo &pin) {
    pins.push_back(pin);
    states.push_back(0);
}

int PinArray::size() {
    return pins.size();
}

PinInfo PinArray::get(int position) {
    if (position < size()) {
        return pins[position];
    }
    // Handle the case where the position is out of bounds
    // or return a default PinInfo object in such cases.
    return PinInfo::create("", 0, 0, true);
}

void PinArray::push(const PinInfo &newPin) {
    addPin(newPin);
}

PinArray PinArray::getPinsNear(int position, int numberOfPins, boolean loopAround) {
    PinArray pinsNearPosition = PinArray();

    for (int i = 0; i < numberOfPins; i++) {
        int positionOfPinToFetch = (position + i) % pins.size();
        PinInfo pinToFetech = pins[positionOfPinToFetch];
        pinsNearPosition.addPin(pinToFetech);
    }

    return pinsNearPosition;
}
