#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Adafruit_NeoPixel.h>

class LEDStrip {
  private:
    Adafruit_NeoPixel *strip;
    int offset;  // The starting LED index offset

  public:
    // Constructor with offset
    LEDStrip(int numLEDs, int pin, int startOffset = 0) 
      : offset(startOffset) {
      strip = new Adafruit_NeoPixel(numLEDs, pin, NEO_GRB + NEO_KHZ800);
    }

    // Initialize the LED strip
    void begin() {
      strip->begin();
      strip->show();  // Initialize all pixels to 'off'
    }

    // Set the color of a specific pixel, applying the offset
    void setPixelColor(int index, uint32_t color) {
      if (index + offset < strip->numPixels()) {
        strip->setPixelColor(index + offset, color);
      }
    }

    // Show the changes on the strip
    void show() {
      strip->show();
    }

    // Get the number of pixels in the strip
    int numPixels() {
      return strip->numPixels();
    }

    // Clear all LEDs (set them to off)
    void clear() {
      strip->clear();  // Apply the changes
    }

    // Create a color from RGB values
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
      return strip->Color(r, g, b);
    }
};

#endif
