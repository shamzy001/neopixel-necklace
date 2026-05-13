#include <Adafruit_NeoPixel.h>

#define PIN        0
#define NUM_LEDS   16

Adafruit_NeoPixel pixels(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// Modes
enum Mode {
  SPARKLE = 0,
  CHECKER_STATIC,
  CHECKER_SPIN,
  EIGHT_STATIONARY,
  EIGHT_SPINNING,
  NUM_MODES
};

Mode mode = SPARKLE;

// Timing
const uint32_t modeInterval = 10000; // 10 seconds per mode
uint32_t lastModeSwitch = 0;

const uint32_t sparkleInterval = 20;
const uint32_t spinInterval = 150;
uint32_t lastSparkle = 0;
uint32_t lastSpin = 0;

// Spinning offsets
uint8_t offset = 0;
uint8_t spinPos8LED = 0;

// Colors
const uint32_t SCARLET_RED  = pixels.Color(200, 0, 0, 0);
const uint32_t BUCKEYE_GRAY = pixels.Color(5, 5, 5, 0);

// Flag to track static mode updates
bool staticModeUpdated = false;

void setup() {
  pixels.begin();
  pixels.setBrightness(60);
}

void loop() {
  uint32_t now = millis();

  // ----------------------------
  // Automatic mode switching
  // ----------------------------
  if (now - lastModeSwitch >= modeInterval) {
    mode = static_cast<Mode>((mode + 1) % NUM_MODES);
    pixels.clear();
    staticModeUpdated = false; // reset static mode flag
    lastModeSwitch = now;
    // Reset spinning offsets
    offset = 0;
    spinPos8LED = 0;
    lastSpin = now;
    lastSparkle = now;
  }

  // ----------------------------
  // Run current mode
  // ----------------------------
  switch(mode) {

    case SPARKLE:
      if (now - lastSparkle >= sparkleInterval) {
        int i = random(NUM_LEDS);
        pixels.setPixelColor(i, (random(2) == 0) ? SCARLET_RED : BUCKEYE_GRAY);
        pixels.show();
        pixels.setPixelColor(i, 0);
        lastSparkle = now;
      }
      break;

    case CHECKER_STATIC:
      if (!staticModeUpdated) {
        for (int i = 0; i < NUM_LEDS; i++)
          pixels.setPixelColor(i, (i % 2 == 0) ? SCARLET_RED : BUCKEYE_GRAY);
        pixels.show();
        staticModeUpdated = true;
      }
      break;

    case CHECKER_SPIN:
      if (now - lastSpin >= spinInterval) {
        for (int i = 0; i < NUM_LEDS; i++)
          pixels.setPixelColor(i, ((i + offset) % 2 == 0) ? SCARLET_RED : BUCKEYE_GRAY);
        pixels.show();
        offset = (offset + 1) % 2;
        lastSpin = now;
      }
      break;
      
    case EIGHT_STATIONARY:
      if (!staticModeUpdated) {
        // Define top LEDs (red) and bottom LEDs (gray) explicitly
        uint8_t topLEDs[8]    = {7,8,9,10,11,12,13,14};   // top half
        uint8_t bottomLEDs[8] = {15,0,1,2,3,4,5,6}; // bottom half
    
        for (int i = 0; i < 8; i++) {
          pixels.setPixelColor(topLEDs[i], SCARLET_RED);
          pixels.setPixelColor(bottomLEDs[i], BUCKEYE_GRAY);
        }
        pixels.show();
        staticModeUpdated = true;
      }
      break;

    case EIGHT_SPINNING:
      if (now - lastSpin >= spinInterval) {
        pixels.clear();
        int half = NUM_LEDS / 2;
        int blockSize = 8;

        for (int i = 0; i < blockSize; i++) {
          pixels.setPixelColor((spinPos8LED + i) % NUM_LEDS, SCARLET_RED);
          pixels.setPixelColor((spinPos8LED + half + i) % NUM_LEDS, BUCKEYE_GRAY);
        }
        pixels.show();
        spinPos8LED = (spinPos8LED + 1) % NUM_LEDS;
        lastSpin = now;
      }
      break;
  }
}
