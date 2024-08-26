#ifndef PATTERN_H
#define PATTERN_H

#include "LEDStrip.h"

// Base Pattern class (abstract)
class Pattern {
  protected:
    LEDStrip **strips;  // Array of pointers to LEDStrip objects
    int numStrips;      // Number of strips in the array

  public:
    // Constructor
    Pattern(LEDStrip **stripsPointer, int numStrips)
      : strips(stripsPointer), numStrips(numStrips) {}

    // Method that loops over all strips and calls the virtual applyPattern method
    void run(int wait) {
      for (int s = 0; s < numStrips; s++) {
        applyPattern(strips[s], wait);
      }
    }

    // Pure virtual method to apply the pattern to a single strip
    virtual void applyPattern(LEDStrip *strip, int wait) = 0;
};

// MyColor class for handling color
class MyColor {
  public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    MyColor(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}

    uint32_t toUint32() {
      return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
    }
};

// Derived class for Rainbow Cycle Pattern
class RainbowCyclePattern : public Pattern {
  public:
    RainbowCyclePattern(LEDStrip **stripsPointer, int numStrips)
      : Pattern(stripsPointer, numStrips) {}

    void applyPattern(LEDStrip *strip, int wait) override;

  private:
    uint32_t wheel(byte WheelPos, LEDStrip *strip);
};

// Derived class for Color Chase Pattern
class ColorChasePattern : public Pattern {
  private:
    MyColor color;

  public:
    ColorChasePattern(LEDStrip **stripsPointer, int numStrips, MyColor c)
      : Pattern(stripsPointer, numStrips), color(c) {}

    void applyPattern(LEDStrip *strip, int wait) override;
};

// Derived class for Theater Chase Pattern
class TheaterChasePattern : public Pattern {
  private:
    MyColor color;

  public:
    TheaterChasePattern(LEDStrip **stripsPointer, int numStrips, MyColor c)
      : Pattern(stripsPointer, numStrips), color(c) {}

    void applyPattern(LEDStrip *strip, int wait) override;
};

#endif
