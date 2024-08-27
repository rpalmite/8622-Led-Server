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
    void run() {
      Serial.println("Running pattern");
      for (int s = 0; s < numStrips; s++) {
        Serial.print("Strip #");
        Serial.println(s);
        applyPattern(strips[s]);
        delay(50);
      }
    }

    // Pure virtual method to apply the pattern to a single strip
    virtual void applyPattern(LEDStrip *strip) = 0;

    // Utility method to generate a random color
    virtual uint32_t getColor() {
      uint8_t r = random(0, 256);
      uint8_t g = random(0, 256);
      uint8_t b = random(0, 256);
      return (r << 16) | (g << 8) | b;  // Combine into uint32_t color
    }

    virtual void wait(int millesecs) {
      // TODO make wait time variable
      delay(millesecs);  // Internal delay for this pattern
    }

    // virtual wheel(int WheelPos) {
    //   WheelPos = 255 - WheelPos;
    //     if (WheelPos < 85) {
    //       return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
    //     }
    //     if (WheelPos < 170) {
    //       WheelPos -= 85;
    //       return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
    //     }
    //     WheelPos -= 170;
    //     return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    // }
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

    void applyPattern(LEDStrip *strip) override;

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

    void applyPattern(LEDStrip *strip) override;
};

// Derived class for Theater Chase Pattern
class TheaterChasePattern : public Pattern {
  private:
    MyColor color;

  public:
    TheaterChasePattern(LEDStrip **stripsPointer, int numStrips, MyColor c)
      : Pattern(stripsPointer, numStrips), color(c) {}

    void applyPattern(LEDStrip *strip) override;
};

// Derived class for Cylon Bounce Pattern
class CylonBouncePattern : public Pattern {
  private:
    MyColor color;
    int eyeSize;
    int speedDelay;
    int returnDelay;

  public:
    CylonBouncePattern(LEDStrip **stripsPointer, int numStrips, MyColor c, int eyeSize, int speedDelay, int returnDelay)
      : Pattern(stripsPointer, numStrips), color(c), eyeSize(eyeSize), speedDelay(speedDelay), returnDelay(returnDelay) {}

    void applyPattern(LEDStrip *strip) override;
};

// Derived class for Bouncing Line Pattern
class BouncingLinePattern : public Pattern {
  private:
    int lineSize;

  public:
    BouncingLinePattern(LEDStrip **stripsPointer, int numStrips, int lineSize)
      : Pattern(stripsPointer, numStrips), lineSize(lineSize) {}

    void applyPattern(LEDStrip *strip) override;

  private:
    uint32_t modifyColor(uint32_t color, int redMod, int greenMod, int blueMod);
    uint32_t getColor();
};

class RandomSpotsPattern : public Pattern {
  private:
    int size;

  public:
    RandomSpotsPattern(LEDStrip **stripsPointer, int numStrips, int size)
      : Pattern(stripsPointer, numStrips), size(size) {}

    void applyPattern(LEDStrip *strip) override;

  private:
    //uint32_t getColor();
    //int getPauseSpeed();
};


class RandomClusteredDotsPattern : public Pattern {
  private:
    int density;  // Controls how frequently clusters appear
    int clusterSize;  // Number of consecutive pixels in each cluster

  public:
    RandomClusteredDotsPattern(LEDStrip **stripsPointer, int numStrips, int density, int clusterSize)
      : Pattern(stripsPointer, numStrips), density(density), clusterSize(clusterSize) {}

    void applyPattern(LEDStrip *strip) override;

  private:
    uint32_t getColor();
    int getPauseSpeed();
};

#endif
