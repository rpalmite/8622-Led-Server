#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Adafruit_NeoPixel.h>

class LEDStrip {
  private:
    Adafruit_NeoPixel *strip;

  public:
    // Constructor
    LEDStrip(int numLEDs, int pin) {
      strip = new Adafruit_NeoPixel(numLEDs, pin, NEO_GRB + NEO_KHZ800);
    }

    // Initialize the LED strip
    void begin() {
      strip->begin();
      strip->show();  // Initialize all pixels to 'off'
    }

    // Set the color of a specific pixel
    void setPixelColor(int index, uint32_t color) {
      strip->setPixelColor(index, color);
    }

    // Show the changes on the strip
    void show() {
      strip->show();
    }

    // Get the number of pixels in the strip
    int numPixels() {
      return strip->numPixels();
    }

    // Create a color from RGB values
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
      return strip->Color(r, g, b);
    }
};

#endif
