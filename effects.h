//   Graphical effects to run on the RGB Shades LED array
//   Each function should have the following components:
//    * Must be declared void with no parameters or will break function pointer array
//    * Check effectInit, if false then init any required settings and set effectInit true
//    * Set effectDelay (the time in milliseconds until the next run of this effect)
//    * All animation should be controlled with counters and effectDelay, no delay() or loops
//    * Pixel data should be written using leds[XY(x,y)] to map coordinates to the RGB Shades layout

// Triple Sine Waves
void threeSine() {

  static byte sineOffset = 0; // counter for current position of sine waves

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 20;
  }

  // Draw one frame of the animation into the LED array
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {

      // Calculate "sine" waves with varying periods
      // sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
      byte sinDistanceR = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 9 + x * 16)), 2);
      byte sinDistanceG = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 10 + x * 16)), 2);
      byte sinDistanceB = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 11 + x * 16)), 2);

      leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
    }
  }

  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle

}


// RGB Plasma
void plasma() {

  static byte offset  = 0; // counter for radial color wave motion
  static int plasVector = 0; // counter for orbiting plasma center

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 10;
  }

  // Calculate current center of plasma pattern (can be offscreen)
  int xOffset = cos8(plasVector / 256);
  int yOffset = sin8(plasVector / 256);

  // Draw one frame of the animation into the LED array
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x - 7.5) * 10 + xOffset - 127) + sq(((float)y - 2) * 10 + yOffset - 127)) + offset);
      leds[XY(x, y)] = CHSV(color, 255, 255);
    }
  }

  offset++; // wraps at 255 for sin8
  plasVector += 16; // using an int for slower orbit (wraps at 65536)

}


// Scanning pattern left/right, uses global hue cycle
void rider() {

  static byte riderPos = 0;

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 5;
    riderPos = 0;
  }

  // Draw one frame of the animation into the LED array
  for (byte x = 0; x < kMatrixWidth; x++) {
    int brightness = abs(x * (256 / kMatrixWidth) - triwave8(riderPos) * 2 + 127) * 3;
    if (brightness > 255) brightness = 255;
    brightness = 255 - brightness;
    CRGB riderColor = CHSV(cycleHue, 255, brightness);
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)] = riderColor;
    }
  }

  riderPos++; // byte wraps to 0 at 255, triwave8 is also 0-255 periodic

}


// Shimmering noise, uses global hue cycle
void glitter() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 15;
  }

  // Draw one frame of the animation into the LED array
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)] = CHSV(cycleHue, 255, random8(5) * 63);
    }
  }

}


// Fills saturated colors into the array from alternating directions
void colorFill() {

  static byte currentColor = 0;
  static byte currentRow = 0;
  static byte currentDirection = 0;

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 45;
    currentColor = 0;
    currentRow = 0;
    currentDirection = 0;
    currentPalette = RainbowColors_p;
  }

  // test a bitmask to fill up or down when currentDirection is 0 or 2 (0b00 or 0b10)
  if (!(currentDirection & 1)) {
    effectDelay = 45; // slower since vertical has fewer pixels
    for (byte x = 0; x < kMatrixWidth; x++) {
      byte y = currentRow;
      if (currentDirection == 2) y = kMatrixHeight - 1 - currentRow;
      leds[XY(x, y)] = currentPalette[currentColor];
    }
  }

  // test a bitmask to fill left or right when currentDirection is 1 or 3 (0b01 or 0b11)
  if (currentDirection & 1) {
    effectDelay = 20; // faster since horizontal has more pixels
    for (byte y = 0; y < kMatrixHeight; y++) {
      byte x = currentRow;
      if (currentDirection == 3) x = kMatrixWidth - 1 - currentRow;
      leds[XY(x, y)] = currentPalette[currentColor];
    }
  }

  currentRow++;

  // detect when a fill is complete, change color and direction
  if ((!(currentDirection & 1) && currentRow >= kMatrixHeight) || ((currentDirection & 1) && currentRow >= kMatrixWidth)) {
    currentRow = 0;
    currentColor += random8(3, 6);
    if (currentColor > 15) currentColor -= 16;
    currentDirection++;
    if (currentDirection > 3) currentDirection = 0;
    effectDelay = 300; // wait a little bit longer after completing a fill
  }


}

// Emulate 3D anaglyph glasses
void threeDee() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 50;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      if (x < 7) {
        leds[XY(x, y)] = CRGB::Blue;
      } else if (x > 8) {
        leds[XY(x, y)] = CRGB::Red;
      } else {
        leds[XY(x, y)] = CRGB::Black;
      }
    }
  }

  leds[XY(6, 0)] = CRGB::Black;
  leds[XY(9, 0)] = CRGB::Black;

}

// Random pixels scroll sideways, uses current hue
#define rainDir 0
void sideRain() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 30;
  }

  scrollArray(rainDir);
  byte randPixel = random8(kMatrixHeight);
  for (byte y = 0; y < kMatrixHeight; y++) leds[XY((kMatrixWidth - 1) * rainDir, y)] = CRGB::Black;
  leds[XY((kMatrixWidth - 1)*rainDir, randPixel)] = CHSV(cycleHue, 255, 255);

}

// Pixels with random locations and random colors selected from a palette
// Use with the fadeAll function to allow old pixels to decay
void confetti() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 10;
    selectRandomPalette();
  }

  // scatter random colored pixels at several random coordinates
  for (byte i = 0; i < 4; i++) {
    leds[XY(random16(kMatrixWidth), random16(kMatrixHeight))] = ColorFromPalette(currentPalette, random16(255), 255); //CHSV(random16(255), 255, 255);
    random16_add_entropy(1);
  }

}


// Draw slanting bars scrolling across the array, uses current hue
void slantBars() {

  static byte slantPos = 0;

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 5;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)] = CHSV(cycleHue, 255, quadwave8(x * 32 + y * 32 + slantPos));
    }
  }

  slantPos -= 4;

}


#define NORMAL 0
#define RAINBOW 1
#define charSpacing 2
// Scroll a text string
void scrollText(byte message, byte style, CRGB fgColor, CRGB bgColor) {
  static byte currentMessageChar = 0;
  static byte currentCharColumn = 0;
  static byte paletteCycle = 0;
  static CRGB currentColor;
  static byte bitBuffer[16] = {0};
  static byte bitBufferPointer = 0;

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 55; //SCROLL TEXT SPEED
    currentMessageChar = 0;
    currentCharColumn = 0;
    selectFlashString(message);
    loadCharBuffer(loadStringChar(message, currentMessageChar));
    currentPalette = RainbowColors_p;
    for (byte i = 0; i < kMatrixWidth; i++) bitBuffer[i] = 0;
  }

  paletteCycle += 15;

  if (currentCharColumn < 5) { // characters are 5 pixels wide
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = charBuffer[currentCharColumn]; // character
  } else {
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = 0; // space
  }

  CRGB pixelColor;
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < 5; y++) { // characters are 5 pixels tall
      if (bitRead(bitBuffer[(bitBufferPointer + x) % kMatrixWidth], y) == 1) {
        if (style == RAINBOW) {
          pixelColor = ColorFromPalette(currentPalette, paletteCycle + y * 16, 255);
        } else {
          pixelColor = fgColor;
        }
      } else {
        pixelColor = bgColor;
      }
      leds[XY(x, y)] = pixelColor;
    }
  }

  currentCharColumn++;
  if (currentCharColumn > (4 + charSpacing)) {
    currentCharColumn = 0;
    currentMessageChar++;
    char nextChar = loadStringChar(message, currentMessageChar);
    if (nextChar == 0) { // null character at end of strong
      currentMessageChar = 0;
      nextChar = loadStringChar(message, currentMessageChar);
    }
    loadCharBuffer(nextChar);
  }

  bitBufferPointer++;
  if (bitBufferPointer > 15) bitBufferPointer = 0;

}


void scrollTextZero() {
  scrollText(0, NORMAL, CRGB::Red, CRGB::Black);
}

void scrollTextOne() {
  scrollText(1, RAINBOW, 0, CRGB::Black);
}

void scrollTextTwo() {
  scrollText(2, NORMAL, CRGB::Green, CRGB(0, 0, 8));
}

void scrollTextThree() {
  scrollText(3, NORMAL, CRGB::Red, CRGB::Black);
}

void scrollTextFour() {
  scrollText(4, RAINBOW, 0, CRGB::Black);
}

void scrollTextFive() {
  scrollText(5, NORMAL, CRGB(100, 100, 0), CRGB::Black);
}
void scrollTextSix() {
  scrollText(6, NORMAL, CRGB::Green, CRGB::Black);
}

void PizzaTime() {

  if (effectInit == false) {
    effectInit = true;
    effectDelay = 50;
  }

  for (byte PizzaTimex = 0; PizzaTimex < kMatrixWidth; PizzaTimex++) {
    for (byte PizzaTimey = 0; PizzaTimey < kMatrixHeight; PizzaTimey++) {
      leds[XY(PizzaTimex, PizzaTimey)] = CHSV(cycleHue, 255, random8(5) * 63);
    }
  }
  //leds[XY(x, y)] = CRGB(Red,Green,Blue);
  leds[XY(2, 0)] = CRGB(95.3, 64.3, 0);
  leds[XY(3, 0)] = CRGB(95.3, 64.3, 0);
  leds[XY(4, 0)] = CRGB(95.3, 64.3, 0);
  leds[XY(11, 0)] = CRGB(95.3, 64.3, 0);
  leds[XY(12, 0)] = CRGB(95.3, 64.3, 0);
  leds[XY(13, 0)] = CRGB(95.3, 64.3, 0);
  leds[XY(1, 1)] = CRGB(95.3, 64.3, 0);
  leds[XY(5, 1)] = CRGB(95.3, 64.3, 0);
  leds[XY(10, 1)] = CRGB(95.3, 64.3, 0);
  leds[XY(14, 1)] = CRGB(95.3, 64.3, 0);
  leds[XY(1, 2)] = CRGB(95.3, 64.3, 0);
  leds[XY(5, 2)] = CRGB(95.3, 64.3, 0);
  leds[XY(10, 2)] = CRGB(95.3, 64.3, 0);
  leds[XY(14, 2)] = CRGB(95.3, 64.3, 0);
  leds[XY(1, 3)] = CRGB(95.3, 64.3, 0);
  leds[XY(5, 3)] = CRGB(95.3, 64.3, 0);
  leds[XY(10, 3)] = CRGB(95.3, 64.3, 0);
  leds[XY(14, 3)] = CRGB(95.3, 64.3, 0);
  leds[XY(2, 4)] = CRGB(95.3, 64.3, 0);
  leds[XY(3, 4)] = CRGB(95.3, 64.3, 0);
  leds[XY(4, 4)] = CRGB(95.3, 64.3, 0);
  leds[XY(11, 4)] = CRGB(95.3, 64.3, 0);
  leds[XY(12, 4)] = CRGB(95.3, 64.3, 0);
  leds[XY(13, 4)] = CRGB(95.3, 64.3, 0);

  leds[XY(2, 1)] = CRGB(100, 100, 0);
  leds[XY(4, 1)] = CRGB(100, 100, 0);
  leds[XY(2, 2)] = CRGB(100, 100, 0);
  leds[XY(3, 2)] = CRGB(100, 100, 0);
  leds[XY(3, 3)] = CRGB(100, 100, 0);
  leds[XY(4, 3)] = CRGB(100, 100, 0);
  leds[XY(11, 1)] = CRGB(100, 100, 0);
  leds[XY(13, 1)] = CRGB(100, 100, 0);
  leds[XY(11, 2)] = CRGB(100, 100, 0);
  leds[XY(12, 2)] = CRGB(100, 100, 0);
  leds[XY(13, 2)] = CRGB(100, 100, 0);
  leds[XY(12, 3)] = CRGB(100, 100, 0);

  leds[XY(3, 1)] = CRGB(100, 0, 0);
  leds[XY(4, 2)] = CRGB(100, 0, 0);
  leds[XY(2, 3)] = CRGB(100, 0, 0);
  leds[XY(12, 1)] = CRGB(100, 0, 0);
  leds[XY(11, 3)] = CRGB(100, 0, 0);
  leds[XY(13, 3)] = CRGB(100, 0, 0);
}

void micLevels() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 5;
  }
  int micReading;
  int micRGB;
  int micPin;
  int mic0;
  int mic1;
  int mic2;
  int mic3;
  int mic4;
  //int micRGB;\

  //CHSV(cycleHue, 255, random8(5) * 63)
  //leds[XY(2, 0)] = CRGB(100,100,100);
  micReading = analogRead(micPin);
  micRGB = map(micReading, 0, 255, 0, 15);
  mic4 = mic3;
  mic3 = mic2;
  mic2 = mic1;
  mic1 = mic0;
  mic0 = micRGB;

  for (int micFor = 0; micFor < 4; micFor++) {

    for (int micFor2 = 0; micFor2 < mic[micFor]; micFor2++) {
      leds[XY(micFor2, micFor)] = CHSV(cycleHue, 255, random8(5) * 63);
    }

  }
}
void BaseballEyes() {

  if (effectInit == false) {
    effectInit = true;
    effectDelay = 50;
  }

  for (byte BaseballEyesx = 0; BaseballEyesx < kMatrixWidth; BaseballEyesx++) {
    for (byte BaseballEyesy = 0; BaseballEyesy < kMatrixHeight; BaseballEyesy++) {
      leds[XY(BaseballEyesx, BaseballEyesy)] = CHSV(cycleHue, 255, random8(5) * 63);
    }
  }


  leds[XY(2, 0)] = CRGB(100, 100, 100);
  leds[XY(3, 0)] = CRGB(100, 100, 100);
  leds[XY(4, 0)] = CRGB(100, 0, 0); //
  leds[XY(11, 0)] = CRGB(100, 100, 100);
  leds[XY(12, 0)] = CRGB(100, 0, 0); //
  leds[XY(13, 0)] = CRGB(100, 100, 100);
  leds[XY(1, 1)] = CRGB(100, 0, 0); //
  leds[XY(5, 1)] = CRGB(100, 0, 0); //
  leds[XY(10, 1)] = CRGB(100, 100, 100);
  leds[XY(14, 1)] = CRGB(100, 100, 100);
  leds[XY(1, 2)] = CRGB(100, 100, 100);
  leds[XY(5, 2)] = CRGB(100, 0, 0); //
  leds[XY(10, 2)] = CRGB(100, 100, 100);
  leds[XY(14, 2)] = CRGB(100, 0, 0); //
  leds[XY(1, 3)] = CRGB(100, 100, 100);
  leds[XY(5, 3)] = CRGB(100, 100, 100);
  leds[XY(10, 3)] = CRGB(100, 0, 0); //
  leds[XY(14, 3)] = CRGB(100, 100, 100);
  leds[XY(2, 4)] = CRGB(100, 0, 0); //
  leds[XY(3, 4)] = CRGB(100, 100, 100);
  leds[XY(4, 4)] = CRGB(100, 100, 100);
  leds[XY(11, 4)] = CRGB(100, 0, 0); //
  leds[XY(12, 4)] = CRGB(100, 100, 100);
  leds[XY(13, 4)] = CRGB(100, 100, 100);

  leds[XY(2, 1)] = CRGB(100, 0, 0); //
  leds[XY(4, 1)] = CRGB(100, 100, 100);
  leds[XY(2, 2)] = CRGB(100, 100, 100);
  leds[XY(3, 2)] = CRGB(100, 0, 0); //
  leds[XY(3, 3)] = CRGB(100, 0, 0); //
  leds[XY(4, 3)] = CRGB(100, 100, 100);
  leds[XY(11, 1)] = CRGB(100, 100, 100);
  leds[XY(13, 1)] = CRGB(100, 100, 100);
  leds[XY(11, 2)] = CRGB(100, 100, 100);
  leds[XY(12, 2)] = CRGB(100, 100, 100);
  leds[XY(13, 2)] = CRGB(100, 0, 0); //
  leds[XY(12, 3)] = CRGB(100, 100, 100);
  leds[XY(3, 1)] = CRGB(100, 100, 100);
  leds[XY(4, 2)] = CRGB(100, 100, 100);
  leds[XY(2, 3)] = CRGB(100, 100, 100);
  leds[XY(12, 1)] = CRGB(100, 0, 0); //
  leds[XY(11, 3)] = CRGB(100, 100, 100);
  leds[XY(13, 3)] = CRGB(100, 100, 100);
}

