// SPDX-FileCopyrightText: 2017 Phillip Burgess for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUM_LEDS   16
#define MODE_COUNT 3

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN);

bool up = true;
uint8_t  mode   = 0,        // Current animation effect
         offset = 0;        // Position of spinner animation
uint32_t color  = 0xFF8000; // Starting color = amber
uint32_t prevTime;          // Time of last animation mode switch

void setup() {
  pixels.begin();
  pixels.setBrightness(60); // ~1/3 brightness
  prevTime = millis();      // Starting time
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch(mode) {

   case 1: // Random sparkles - just one LED on at a time!
    i = random(NUM_LEDS);           // Choose a random pixel
    pixels.setPixelColor(i, color); // Set it to current color
    pixels.show();                  // Refresh LED states
    // Set same pixel to "off" color now but DON'T refresh...
    // it stays on for now...both this and the next random
    // pixel will be refreshed on the next pass.
    pixels.setPixelColor(i, 0);
    delay(10);                      // 10 millisecond delay
    break;
 
   case 2: // Spinny wheel (4 LEDs on at a time)
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

    case 3: // Alternating
      bool shift = false;
      for (i = 0; i < NUM_LEDS; i++)
        pixels.setPixelColor(i, i % 2 == shift ? 1 : 0 ? color : 0);
      pixels.show();
      delay(500);
      shift = !shift;
      break;
      
    case 4: // Build up
      i = random(NUM_LEDS);
      if (up) {
        while (pixels.getPixelColor(i) != 0)
          i = random(NUM_LEDS);
        pixels.setPixelColor(i, color);
        if (++offset == 15)
          up = false;
      } else {
        while (pixels.getPixelColor(i) == 0)
          i = random(NUM_LEDS);
        pixels.setPixelColor(i, 0);
        if (--offset == 0)
          up = true;
      }
      pixels.show();
      delay(250);
      break;
      
    case 5: // B  
      // uint16_t temp = offset;
      if (up) {
        offset = (offset + 1) * 2 - 1;
        if (offset == 255)
          up = false;
      } else {
        offset = (offset + 1) / 2 - 1;
        if (offset == 1)
          up = true;
      }
      if (pixels.getPixelColor(0) == 0)
        pixels.fill(color, 0);
      pixels.setBrightness(offset);
      pixels.show();
      delay(50);
      break;
  }

  t = millis();                    // Current time in milliseconds
  if((t - prevTime) > 4000) {      // Every 4 seconds...
    offset = 0;
    up = true;
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
