# NeoPixel Necklace

Adafruit Gemma + NeoPixel Ring 16 RGBW firmware for a wearable LED pendant necklace, made as a Christmas gift.

Two sketches:
- `osu.ino` — Ohio State colors (scarlet & gray), 5 animation modes
- `christmas.ino` — Christmas colors (red, green, white, blue), 5 animation modes

## Hardware

- Adafruit Gemma
- [Adafruit NeoPixel Ring 16 RGBW Warm White](https://www.adafruit.com/product/2854)
- Small LiPo battery (350mAh or similar)
- Gold bail for hanging on a chain

## Setup

1. Install the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) in Arduino IDE
2. Select **Adafruit Gemma** as the board
3. Flash either sketch to the Gemma via USB

Both sketches use `millis()`-based timing — no `delay()` calls anywhere, so mode switching stays responsive throughout.

## Write-up

[I Made Beth a Necklace for Christmas](https://www.shahzangeneh.com/neopixel-necklace)
