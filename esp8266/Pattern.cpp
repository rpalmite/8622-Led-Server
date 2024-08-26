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

void CylonBouncePattern::applyPattern(LEDStrip *strip, int wait) {
  // Forward pass
  for(int i = 0; i < strip->numPixels() - eyeSize - 2; i++) {
    // Turn off all LEDs
    for (int k = 0; k < strip->numPixels(); k++) {
      strip->setPixelColor(k, strip->Color(0, 0, 0));
    }

    // Dimmed leading and trailing LED
    strip->setPixelColor(i, strip->Color(color.red / 10, color.green / 10, color.blue / 10));
    
    // The 'eye' of the Cylon
    for(int j = 1; j <= eyeSize; j++) {
      strip->setPixelColor(i + j, strip->Color(color.red, color.green, color.blue));
    }

    // Dimmed trailing LED after the 'eye'
    strip->setPixelColor(i + eyeSize + 1, strip->Color(color.red / 10, color.green / 10, color.blue / 10));

    // Show the changes
    strip->show();
    delay(speedDelay);
  }

  // Delay before returning
  delay(returnDelay);

  // Backward pass
  for(int i = strip->numPixels() - eyeSize - 2; i > 0; i--) {
    // Turn off all LEDs
    for (int k = 0; k < strip->numPixels(); k++) {
      strip->setPixelColor(k, strip->Color(0, 0, 0));
    }

    // Dimmed leading and trailing LED
    strip->setPixelColor(i, strip->Color(color.red / 10, color.green / 10, color.blue / 10));

    // The 'eye' of the Cylon
    for(int j = 1; j <= eyeSize; j++) {
      strip->setPixelColor(i + j, strip->Color(color.red, color.green, color.blue));
    }

    // Dimmed trailing LED after the 'eye'
    strip->setPixelColor(i + eyeSize + 1, strip->Color(color.red / 10, color.green / 10, color.blue / 10));

    // Show the changes
    strip->show();
    delay(speedDelay);
  }

  // Delay before next cycle
  delay(returnDelay);
}

// BouncingLinePattern //

void BouncingLinePattern::applyPattern(LEDStrip *strip, int wait) {
  uint32_t color = getColor();

  // Forward bouncing line
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->show();  // Show initial state
    strip->clear(); // Clear the strip

    for (int j = i; j < i + lineSize && j < strip->numPixels(); j++) {
      if (j == i) {
        strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim first pixel
      } else if (j == i + lineSize - 1) {
        strip->setPixelColor(j, color);  // Full color on the last pixel
      } else {
        strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim middle pixels
      }
    }

    strip->show();
    delay(wait);
  }

  color = getColor();  // Update color for the backward pass

  // Backward bouncing line
  for (int i = strip->numPixels() - 1; i >= 0; i--) {
    strip->clear(); // Clear the strip

    for (int j = i; j < i + lineSize && j < strip->numPixels(); j++) {
      if (j == i) {
        strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim first pixel
      } else if (j == i + lineSize - 1) {
        strip->setPixelColor(j, color);  // Full color on the last pixel
      } else {
        strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim middle pixels
      }
    }

    strip->show();
    delay(wait);
  }
}

uint32_t BouncingLinePattern::modifyColor(uint32_t color, int redMod, int greenMod, int blueMod) {
  // Break down color into components
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  // Modify each component with the provided mod values
  r = constrain(r + redMod, 0, 255);
  g = constrain(g + greenMod, 0, 255);
  b = constrain(b + blueMod, 0, 255);

  return (r << 16) | (g << 8) | b;  // Recombine into uint32_t color
}

uint32_t BouncingLinePattern::getColor() {
  // Generate or retrieve a color; for now, let's return a random color for demonstration
  uint8_t r = random(0, 256);
  uint8_t g = random(0, 256);
  uint8_t b = random(0, 256);
  return (r << 16) | (g << 8) | b;  // Combine into uint32_t color
}