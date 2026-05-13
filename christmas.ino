#include <Adafruit_NeoPixel.h>

#define PIN        0
#define NUM_LEDS   16

Adafruit_NeoPixel pixels(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// Modes
enum Mode {
  TWINKLE = 0,
  SPIN_RG,
  CHASE,
  SPIN_RW,
  FADE,
  NUM_MODES
};

Mode mode = TWINKLE;

// Timing intervals
const uint32_t modeInterval    = 10000; // 10 seconds per mode
const uint32_t sparkleInterval = 60;
const uint32_t chaseInterval   = 150;
const uint32_t spinInterval    = 150;
const uint32_t fadeInterval    = 20;

// Last update times
uint32_t lastModeSwitch = 0;
uint32_t lastSparkle    = 0;
uint32_t lastChase      = 0;
uint32_t lastSpinRG     = 0;
uint32_t lastSpinRW     = 0;
uint32_t lastFade       = 0;

// Offsets
uint8_t chasePos       = 0;
uint8_t offsetRG       = 0;
uint8_t offsetRW       = 0;

// Fade variables
uint8_t fadeValue = 0;
int fadeDir = 1;

// Colors
const uint32_t RED   = pixels.Color(255, 0, 0, 0);
const uint32_t GREEN = pixels.Color(0, 255, 0, 0);
const uint32_t WHITE = pixels.Color(255, 255, 255, 0);
const uint32_t BLUE  = pixels.Color(0, 0, 255, 0);

// Christmas colors array
const uint32_t XmasColors[4] = { RED, GREEN, WHITE, BLUE };

// ---------------------------------------------------------------------------
void setup() {
  pixels.begin();
  pixels.setBrightness(60);
  pixels.clear();
}

// ---------------------------------------------------------------------------
void loop() {
  uint32_t now = millis();

  // Automatic mode switching
  if (now - lastModeSwitch >= modeInterval) {
    mode = static_cast<Mode>((mode + 1) % NUM_MODES);
    pixels.clear();
    pixels.show();
    lastModeSwitch = now;

    // Reset offsets
    chasePos = offsetRG = offsetRW = 0;
    fadeValue = 0;
    fadeDir = 1;

    lastSparkle = lastChase = lastSpinRG = lastSpinRW = lastFade = now;
  }

  switch(mode) {

    // -------------------------------------------------
    // MODE 0 — TWINKLE with very dark glow
    // -------------------------------------------------
    case TWINKLE:
      if (now - lastSparkle >= sparkleInterval) {
        // Dark subtle glow
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(5, 5, 5, 0)); // very low glow
        }
        // Sparkles on top
        for (uint8_t j = 0; j < 4; j++) { // 4 sparkles
          uint8_t i = random(NUM_LEDS);
          uint8_t r = random(4);
          uint32_t c = (r == 0) ? RED :
                       (r == 1) ? GREEN :
                       (r == 2) ? WHITE : BLUE;
          pixels.setPixelColor(i, c);
        }
        pixels.show();
        lastSparkle = now;
      }
      break;

    // -------------------------------------------------
    // MODE 1 — SPIN RED/GREEN
    // -------------------------------------------------
    case SPIN_RG:
      if (now - lastSpinRG >= spinInterval) {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
          pixels.setPixelColor(i, ((i + offsetRG) & 1) ? RED : GREEN);
        }
        pixels.show();
        offsetRG ^= 1;
        lastSpinRG = now;
      }
      break;

    // -------------------------------------------------
    // MODE 2 — CHASE (1 LED per color)
    // -------------------------------------------------
    case CHASE:
      if (now - lastChase >= chaseInterval) {
        pixels.clear();
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
          uint8_t idx = (i + chasePos) % 4; // 1 LED per color
          pixels.setPixelColor(i, XmasColors[idx]);
        }
        pixels.show();
        chasePos = (chasePos + 1) % NUM_LEDS;
        lastChase = now;
      }
      break;

    // -------------------------------------------------
    // MODE 3 — SPIN RED/WHITE
    // -------------------------------------------------
    case SPIN_RW:
      if (now - lastSpinRW >= spinInterval) {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
          pixels.setPixelColor(i, ((i + offsetRW) & 1) ? RED : WHITE);
        }
        pixels.show();
        offsetRW ^= 1;
        lastSpinRW = now;
      }
      break;

    // -------------------------------------------------
    // MODE 4 — FADE
    // -------------------------------------------------
    case FADE:
      if (now - lastFade >= fadeInterval) {
        fadeValue += fadeDir;
        if (fadeValue >= 255) fadeDir = -1;
        else if (fadeValue <= 0) fadeDir = 1;

        for (uint8_t i = 0; i < NUM_LEDS; i++) {
          uint8_t idx = i % 4;
          uint32_t c = (idx == 0) ? pixels.Color(fadeValue, 0, 0, 0) :
                       (idx == 1) ? pixels.Color(0, fadeValue, 0, 0) :
                       (idx == 2) ? pixels.Color(fadeValue, fadeValue, fadeValue, 0) :
                                    pixels.Color(0, 0, fadeValue, 0);
          pixels.setPixelColor(i, c);
        }
        pixels.show();
        lastFade = now;
      }
      break;
  }
}
