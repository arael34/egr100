// SPDX-FileCopyrightText: 2017 Phillip Burgess for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUM_LEDS   16
#define MODE_COUNT 5

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN);

bool     shift = true;
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

   case 1: // breathing
    int brightness = pixels.getBrightness() + (shift ? 10 : -10);
    pixels.setBrightness(brightness);
    if (brightness > 100 || brightness < 20) shift = !shift;
    pixels.show();
    delay(50);
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

    case 3: // alternating
      for (i = 0; i < NUM_LEDS; i++)
        pixels.setPixelColor(i, i % 2 == (shift ? 1 : 0) ? color : 0);
      pixels.show();
      delay(500);
      shift = !shift;
      break;
      
    case 4: // build up
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
    
    case 5: // Long spin
      for (int i = 0; i < 16; i++) {
        uint32_t color = pixels.getPixelColor(i);

        int w = ((color >> 24) & 0xFF) * 0.75; // extract W value
        int r = ((color >> 16) & 0xFF) * 0.75; // extract R value
        int g = ((color >> 8) & 0xFF) * 0.75;  // extract G value
        int b = (color & 0xFF) * 0.75;         // extract B value

        pixels.setPixelColor(i, (w << 24) | (r << 16) | (g << 8) | b);
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
