#include "Pattern.h"

// Implementation of RainbowCyclePattern
void RainbowCyclePattern::applyPattern(LEDStrip *strip, int wait) {
  for (int j = 0; j < 256 * 5; j++) {  // 5 cycles of all colors on the wheel
    for (int i = 0; i < strip->numPixels(); i++) {
      strip->setPixelColor(i, wheel((i * 256 / strip->numPixels() + j) & 255, strip));
    }
    strip->show();
    delay(wait);
  }
}

// Helper function for rainbow wheel effect
uint32_t RainbowCyclePattern::wheel(byte WheelPos, LEDStrip *strip) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Implementation of ColorChasePattern
void ColorChasePattern::applyPattern(LEDStrip *strip, int wait) {
  uint32_t colorValue = color.toUint32();
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, colorValue);
    strip->show();
    delay(wait);
    strip->setPixelColor(i, 0); // Turn off the pixel to create a chase effect
  }
}

// Implementation of TheaterChasePattern
void TheaterChasePattern::applyPattern(LEDStrip *strip, int wait) {
  uint32_t colorValue = color.toUint32();
  for (int j = 0; j < 10; j++) {  // Do 10 cycles of the chase pattern
    for (int q = 0; q < 3; q++) { // Cycle through 3 different phases
      for (int i = 0; i < strip->numPixels(); i = i + 3) {
        strip->setPixelColor(i + q, colorValue);  // Set every third pixel
      }
      strip->show();
      delay(wait);
      for (int i = 0; i < strip->numPixels(); i = i + 3) {
        strip->setPixelColor(i + q, 0);  // Turn off every third pixel
      }
    }
  }
}
