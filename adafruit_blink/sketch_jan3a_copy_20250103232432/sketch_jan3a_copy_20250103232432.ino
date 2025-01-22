#include <Adafruit_NeoPixel.h>

#define PIN        5
#define NUMPIXELS 16

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
  pixels.begin();
}

void blink(int blinks) {
  for (int i=0;i<=blinks;i++) {
    // green
    pixels.clear();
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    }
    pixels.show();
    delay(DELAYVAL);

    // clear
    pixels.clear();
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(DELAYVAL);
  
    i++;
  }
  
}

void loop() {
  blink(2);
  delay(999999);
}