#include <Adafruit_NeoPixel.h>
#include <Adafruit_FreeTouch.h>

#define TOUCH_PIN 0 // I assume 0 is fine
#define PIXEL_PIN 1
#define NUM_LEDS 16 // find out actual pixel count

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIXEL_PIN);
// read docs for args
Adafruit_FreeTouch touchSensor = Adafruit_FreeTouch(
  CAPTOUCH_PIN, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE
);

uint8_t state;
uint32_t prevTime;          // Time of last animation mode switch

// For calibration: change this variable to something between
// your capacitive touch serial readouts for on and off
// without proper output this will maybe have to be guessed
int touch = 650;

// Modes --------------------------------------------------------------
void gradient() {
  
}
// --------------------------------------------------------------------

// ran once at the start
void setup() {
  state = 0;
  prevTime = millis();
  pixels.begin();
  pixels.setBrightness(60); // ~1/3 brightness
  if (!touchSensor.begin()) {
    // error handling ig
  }
}

// loop code
void loop() {
  // touch logic 
}
