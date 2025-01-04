#include <FastLED.h>

#define DATA_PIN 5
#define NUM_LEDS 10

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void flashColor(CRGB color, int delayTime) {
  // Turn on the LEDs with the specified color
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
  delay(delayTime);

  // Turn off the LEDs
  FastLED.clear();
  FastLED.show();  // Ensure the strip updates
  delay(delayTime);
}

void loop() {
  // Flash red three times
  for (int i = 0; i < 3; i++) {
    flashColor(CRGB::Red, 500);
  }

  // Flash green three times
  for (int i = 0; i < 3; i++) {
    flashColor(CRGB::Green, 500);
  }

  // Flash blue three times
  for (int i = 0; i < 3; i++) {
    flashColor(CRGB::Blue, 500);
  }
}
