#include "Pattern.h"

// Implementation of RainbowCyclePattern
void RainbowCyclePattern::applyPattern(LEDStrip *strip) {
  for (int j = 0; j < 256 * 5; j++) {  // 5 cycles of all colors on the wheel
    for (int i = 0; i < strip->numPixels(); i++) {
      strip->setPixelColor(i, wheel((i * 256 / strip->numPixels() + j) & 255, strip));
    }
    strip->show();
    wait(20);  // Internal delay for this pattern
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
void ColorChasePattern::applyPattern(LEDStrip *strip) {
  Serial.println("Running pattern ColorChasePattern");

  uint32_t colorValue = getColor();
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, colorValue);
    strip->show();
    wait(50);  // Internal delay for this pattern
    strip->setPixelColor(i, 0);  // Turn off the pixel to create a chase effect
  }
}

// Implementation of TheaterChasePattern
void TheaterChasePattern::applyPattern(LEDStrip *strip) {
  uint32_t colorValue = color.toUint32();
  for (int j = 0; j < 10; j++) {  // Do 10 cycles of the chase pattern
    for (int q = 0; q < 3; q++) { // Cycle through 3 different phases
      for (int i = 0; i < strip->numPixels(); i += 3) {
        strip->setPixelColor(i + q, colorValue);  // Set every third pixel
      }
      strip->show();
      wait(100);  // Internal delay for this pattern
      for (int i = 0; i < strip->numPixels(); i += 3) {
        strip->setPixelColor(i + q, 0);  // Turn off every third pixel
      }
    }
  }
}

// Implementation of CylonBouncePattern
void CylonBouncePattern::applyPattern(LEDStrip *strip) {
  // Forward pass
  for (int i = 0; i < strip->numPixels() - eyeSize - 2; i++) {
    strip->clear();

    // Dimmed leading LED
    strip->setPixelColor(i, strip->Color(color.red / 10, color.green / 10, color.blue / 10));

    // The 'eye' of the Cylon
    for (int j = 1; j <= eyeSize; j++) {
      strip->setPixelColor(i + j, strip->Color(color.red, color.green, color.blue));
    }

    // Dimmed trailing LED
    strip->setPixelColor(i + eyeSize + 1, strip->Color(color.red / 10, color.green / 10, color.blue / 10));

    strip->show();
    wait(speedDelay);  // Internal delay for this pattern
  }

  // Delay before returning
  wait(returnDelay);

  // Backward pass
  for (int i = strip->numPixels() - eyeSize - 2; i > 0; i--) {
    strip->clear();

    // Dimmed leading LED
    strip->setPixelColor(i, strip->Color(color.red / 10, color.green / 10, color.blue / 10));

    // The 'eye' of the Cylon
    for (int j = 1; j <= eyeSize; j++) {
      strip->setPixelColor(i + j, strip->Color(color.red, color.green, color.blue));
    }

    // Dimmed trailing LED
    strip->setPixelColor(i + eyeSize + 1, strip->Color(color.red / 10, color.green / 10, color.blue / 10));

    strip->show();
    wait(speedDelay);  // Internal delay for this pattern
  }

  // Delay before next cycle
  wait(returnDelay);
}

// BouncingLinePattern //

void makeLine(LEDStrip *strip, int start, int stop, uint32_t color, int lineSize) {
    // turn on the line
    for (int j = start; j < start + lineSize && j < stop; j++) {
      strip->setPixelColor(j, color);  // Full color on the last pixel

      // if (j == i) {
      //   //strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim first pixel
      // } else if (j == i + lineSize - 1) {
      //   strip->setPixelColor(j, color);  // Full color on the last pixel
      // } else {
      //   //strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim middle pixels
      // }
    }
}

// Implementation of BouncingLinePattern
void BouncingLinePattern::applyPattern(LEDStrip *strip) {
  Serial.println("BouncingLine");

  uint32_t color = getColor();

  int miniStripFactor = 30;

  // over alll the pixels
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->clear();  // Clear the strip

    makeLine(strip, i, strip->numPixels(), color, lineSize);
    //makeLine(strip, i+30, strip->numPixels(), color, lineSize);

    Serial.print("Bounce ");
  //  Serial.print(LED_PATTERN);
    Serial.println("");

    strip->show();
    wait(50);  // Internal delay for this pattern
  }

  color = getColor();  // Update color for the backward pass

  // Backward bouncing line
  for (int i = strip->numPixels() - 1; i >= 0; i--) {
    strip->clear();  // Clear the strip

    for (int j = i; j < i + lineSize && j < strip->numPixels(); j++) {
      if (j == i) {
        //strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim first pixel
        strip->setPixelColor(j, color);  // Full color on the last pixel
      } else if (j == i + lineSize - 1) {
        strip->setPixelColor(j, color);  // Full color on the last pixel
      } else {
        //strip->setPixelColor(j, modifyColor(color, -10, -10, -10));  // Dim middle pixels
        strip->setPixelColor(j, color);  // Full color on the last pixel
      }
    }

    strip->show();
    wait(50);  // Internal delay for this pattern
  }
}

uint32_t BouncingLinePattern::modifyColor(uint32_t color, int redMod, int greenMod, int blueMod) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  r = constrain(r + redMod, 0, 255);
  g = constrain(g + greenMod, 0, 255);
  b = constrain(b + blueMod, 0, 255);

  return (r << 16) | (g << 8) | b;
}

uint32_t BouncingLinePattern::getColor() {
  uint8_t r = random(0, 256);
  uint8_t g = random(0, 256);
  uint8_t b = random(0, 256);
  return (r << 16) | (g << 8) | b;
}


// RandomSpotsPattern

void RandomSpotsPattern::applyPattern(LEDStrip *strip) {
  int wait = 100;
  uint32_t color = getColor();

  strip->clear();  // Clear the strip

  for (int i = 0; i < strip->numPixels(); i++) {
    int randomNumber = random(0, size);
    if (randomNumber % size == 0) {
      strip->setPixelColor(i, color);
    }
  }

  strip->show();  // Display the result
  delay(wait);  // Internal delay for this pattern
}

void RandomClusteredDotsPattern::applyPattern(LEDStrip *strip) {
  Serial.println("Applying pattern RandomClusteredDotsPattern");

  int wait = getPauseSpeed();
  uint32_t color = getColor();
  Serial.print("Color=");
  Serial.println(color);

  strip->clear();  // Clear the strip
  Serial.println("strip cleared");

  Serial.print("looping over this many pixels: ");
  Serial.print(strip->numPixels());
  Serial.println();

  for (int i = 0; i < strip->numPixels(); i++) {
    Serial.println("1");

    // Determine if a cluster starts here based on density
    if (random(0, density) == 0) {
      Serial.println("2");

      // Light up a cluster of consecutive pixels
      for (int j = 0; j < clusterSize && (i + j) < strip->numPixels(); j++) {
        Serial.println("3");
        strip->setPixelColor(i + j, color);
        Serial.println("4");
      }
      // Skip over the pixels in the cluster
      i += clusterSize - 1;
    }
  }

  Serial.println("5");

  strip->show();  // Display the result
  Serial.println("RandomClusteredDotsPattern applied");

  delay(wait);  // Internal delay for this pattern
}

uint32_t RandomClusteredDotsPattern::getColor() {
  Serial.println("RandomClusteredDotsPattern::getting color");

  // Generate or retrieve a color; for now, let's return a random color for demonstration
  uint8_t r = random(0, 256);
  uint8_t g = random(0, 256);
  uint8_t b = random(0, 256);
  return (r << 16) | (g << 8) | b;  // Combine into uint32_t color
}

int RandomClusteredDotsPattern::getPauseSpeed() {
  Serial.println("RandomClusteredDotsPattern::pause speed");

  // Return a random pause speed between 100 and 500 ms for demonstration purposes
  return random(100, 500);
}