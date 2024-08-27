// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 140 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int LED_PATTERN = 0;

void setup() {
  Serial.begin(115200);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  Serial.println("Setup");

  int pattern = pickPattern();

  Serial.print("LED PATTERN = ");
  Serial.print(pattern);
  Serial.println("");
}

void loop() {
  Serial.println("New Loop");
  clear(); 

  int pattern = getPattern();

  if (pattern == 0) {
    bouncingLine();
  } else if (pattern == 1) {
    growingLine();
  } else if (pattern == 2) {
    sparkle();
    // CylonBounce(0xff, 0, 0, 4, 10, 50);
  } else if (pattern == 3) {
    rainbow();
    // RGBLoop();
  } else if (pattern == 4) {
    cluster();
    // FadeInOut(0xff, 0x00, 0x00); // red
    // FadeInOut(0xff, 0xff, 0xff); // white
    // FadeInOut(0x00, 0x00, 0xff); // blue
  } else if (pattern == 5) {
    growingUp();
    // Strobe(0xff, 0xff, 0xff, 10, 50, 1000);
  } else if (pattern == 6) {
    growingUp();
    //growingIn();
    // runningLine(getColor(), 5, 100);  // White running line
  } else if (pattern == 7) {
    // rainbow();
  } else {
    bouncingLine();
  }
}


// BASIC FUNCTIONS //

void wait() {
  int waitTime = 100;
  delay(waitTime);
}

void wait(int waitTime) {
  delay(waitTime);
}

void show() {
  pixels.show();
}

// Set all pixel colors to 'off'
void clear() {
  pixels.clear(); 
}

uint32_t getColor() {
  int randomR = random(0, 244);
  int randomG = random(0, 244);
  int randomB = random(0, 244);
  uint32_t color = getColor(randomR, randomG, randomB);
  return color;
}

uint32_t getColor(uint8_t red, uint8_t green, uint8_t blue) {
  return pixels.Color(red, green, blue);
}

uint32_t colorWheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return getColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return getColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return getColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
void setPixelColor(int index, uint8_t red, uint8_t green, uint8_t blue) {
  pixels.setPixelColor(index, pixels.Color(red, green, blue));
}

void setPixelColor(int index, uint32_t color) {
  pixels.setPixelColor(index, color);
}

uint16_t numPixels() {
   return pixels.numPixels();
}

int getPattern() {
  return LED_PATTERN;
}

int pickPattern() {
  int NUMBER_OF_PATTERNS = 6;
  int randomNumber = random(0, NUMBER_OF_PATTERNS);
  LED_PATTERN = randomNumber;
  LED_PATTERN = NUMBER_OF_PATTERNS-1;
  Serial.print("LED PATTERN = ");
  Serial.println(LED_PATTERN);
  return LED_PATTERN;
}

// PATTERNS //

// Growing Line

void growingLine() {
  uint32_t color = getColor();

  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    if (i % 25 == 0) {
      color = getColor();
    }

    setPixelColor(i, color);

    show();   // Send the updated pixel colors to the hardware.

    wait(500);
  }
}

void bouncingLine() {
  int lineSize = 20;
  uint32_t color = getColor();
  for (int i = 0; i < numPixels(); i++) {
    clear();
    for (int j=i; j < i+lineSize; j++) {
      // for (int z=j; z>0; z--) {
      //   if (z % i == 0) {

      //   }
      // }
      setPixelColor(j, color);
    }
    show();
    wait();
  }
  color = getColor();
  for (int i = numPixels()-1; i > 0 ; i--) {
    clear();
    for (int j=i; j < i+lineSize; j++) {
      setPixelColor(j, color);
    }
    show();
    wait();
  }
}

void sparkle() {
  int size = 2; // spotSize
  uint32_t color = getColor();

  clear();
  for (int i = 0; i < numPixels(); i++) {
    int randomNumber = random(0, size);
    if (randomNumber % size) {
      setPixelColor(i, color);
    }
  }
  wait(100);
  show();
}

void rainbow() {
  for (int j = 0; j < 256 * 5; j++) {  // 5 cycles of all colors on the wheel
    for (int i = 0; i < numPixels(); i++) {
      setPixelColor(i, colorWheel((i * 256 / numPixels() + j) & 255));
    }
    show();
    wait(20);  // Internal delay for this pattern
  }
}


void cluster() {
  int density = 5;

  int clusterSize = 4;
  uint32_t color = getColor();
  Serial.print("Color=");
  Serial.println(color);

  clear();
  Serial.println("strip cleared");

  Serial.print("looping over this many pixels: ");
  Serial.print(numPixels());
  Serial.println();

  for (int i = 0; i < numPixels(); i++) {
    //Serial.println("1");

    // Determine if a cluster starts here based on density
    if (random(0, density) == 0) {
      //Serial.println("2");

      // Light up a cluster of consecutive pixels
      for (int j = 0; j < clusterSize && (i + j) < numPixels(); j++) {
        //Serial.println("3");
        setPixelColor(i + j, color);
        //Serial.println("4");
      }
      // Skip over the pixels in the cluster
      i += clusterSize - 1;
    }
  }

  //Serial.println("5");

  show();  // Display the result
  Serial.println("RandomClusteredDotsPattern applied");

  wait();  // Internal delay for this pattern
}

void growingUp() {
  Serial.println("Growing Up");

  uint32_t color = getColor();
  int middle = floor((numPixels()/2));

  Serial.print("Middle = ");
  Serial.println(middle);

  setPixelColor(middle, color);

  for(int i=0; i<middle; i++) { // For each pixel...

    if (i % 25 == 0) {
      color = getColor();
    }

    int distanceUpFromMiddle = middle + i;
    //Serial.print("Up From Middle = ");
    //Serial.println(distanceUpFromMiddle);
    setPixelColor(distanceUpFromMiddle, color);

    int distanceDownFromMiddle = middle - i;
    //Serial.print("Down From Middle = ");
    //Serial.println(distanceDownFromMiddle);
    setPixelColor(distanceDownFromMiddle, color);

    show();   // Send the updated pixel colors to the hardware.

    wait(500);
  }

  for(int i=0; i<middle; i++) { // For each pixel...

    if (i % 25 == 0) {
      color = getColor();
    }

    int distanceUpFromMiddle = middle + i;
    //Serial.print("Up From Middle = ");
    //Serial.println(distanceUpFromMiddle);
    setPixelColor(distanceUpFromMiddle, 0);

    int distanceDownFromMiddle = middle - i;
    //Serial.print("Down From Middle = ");
    //Serial.println(distanceDownFromMiddle);
    setPixelColor(distanceDownFromMiddle, 0);

    show();   // Send the updated pixel colors to the hardware.

    wait(500);
  }
}

void growingIn() {

}


