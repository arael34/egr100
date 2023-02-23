// SPDX-FileCopyrightText: 2017 Phillip Burgess for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUM_LEDS   16
#define MODE_COUNT 4

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN);

bool     shift = true;
uint8_t  mode   = 1,        // Current animation effect
         offset = 0;        // Position of spinner animation
uint32_t color  = 0xFF8000; // Starting color = amber
uint32_t prevTime;          // Time of last animation mode switch

uint32_t changeBrightness(uint32_t pc, bool up) {
  int w = (pc >> 24) & 0xFF; // extract W value
  int r = (pc >> 16) & 0xFF; // extract R value
  int g = (pc >> 8) & 0xFF;  // extract G value
  int b = pc & 0xFF;         // extract B value

  if (up) {
    w *= 1.25;
    r *= 1.25;
    g *= 1.25;
    b *= 1.25;
  } else {
    w *= 0.75;
    r *= 0.75;
    g *= 0.75;
    b *= 0.75;
  }

  return (w << 24) | (r << 16) | (g << 8) | b;
}

void setup() {
  pixels.begin();
  pixels.setBrightness(60); // ~1/3 brightness
  prevTime = millis();      // Starting time
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch(mode) {
    case 1: // alternating
      for (i = 0; i < NUM_LEDS; i++)
        pixels.setPixelColor(i, i % 2 == (shift ? 1 : 0) ? color : 0);
      pixels.show();
      delay(500);
      shift = !shift;
      break;
      
    case 2: // build up
      if (!shift) {
        pixels.setPixelColor(offset++, color);
        shift = offset == 16;
      } else {
        pixels.setPixelColor(offset--, 0);
        shift = !(offset == 0);
      }

      pixels.show();
      delay(100);
      break;

    case 3: // Spinny wheel (4 LEDs on at a time)
      for(i=0; i<NUM_LEDS; i++) {    // For each LED...
        uint32_t c = 0;              // Assume pixel will be "off" color
        if(((offset + i) & 7) < 2) { // For each 8 pixels, 2 will be...
          c = color;                 // ...assigned the current color
        }
        pixels.setPixelColor(i, c);  // Set color of pixel 'i'
      }
      pixels.show();                 // Refresh LED states
      delay(50);                     // 50 millisecond delay
      offset++;                      // Shift animation by 1 pixel on next frame
      break;
    
    case 4: // Long spin
      for (i = 0; i < NUM_LEDS; i++) {
        uint32_t color = pixels.getPixelColor(i);
        pixels.setPixelColor(i, changeBrightness(color, false));
      }
      pixels.setPixelColor(offset++, color);
      offset > 15 && (offset = 0);

      pixels.show();
      delay(50);
      break;
  }

  t = millis();                    // Current time in milliseconds
  if((t - prevTime) > 4000) {      // Every 4 seconds...
    offset = 0;
    shift = false;
    pixels.setBrightness(60);
    mode++;                        // Advance to next animation mode
    if(mode > MODE_COUNT) {                 // End of modes?
      mode = 1;                    // Start over from beginning
      color >>= 8;                 // And change color
      if(!color) color = 0xFF8000; // preiodically reset to amber
    }
    pixels.clear();                // Set all pixels to 'off' state
    prevTime = t;                  // Record the time of the last mode change
  }
}
