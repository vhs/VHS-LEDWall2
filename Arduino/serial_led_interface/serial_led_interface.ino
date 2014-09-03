#include "FastSPI_LED2.h"

#define NUM_LEDS_PER_PANEL 64
#define NUM_PANEL_PER_PIN   4
#define NUM_PINS            3

#define NUM_LEDS           ( NUM_PANEL_PER_PIN * NUM_LEDS_PER_PANEL * NUM_PINS )
#define NUM_LEDS_PER_STRIP ( NUM_PANEL_PER_PIN * NUM_LEDS_PER_PANEL ) 

#define NUM_CHANNELS        (NUM_LEDS*3)

#define PANEL_WIDTH    8
#define PANEL_HEIGHT   8

#define WALL_WIDTH     (PANEL_WIDTH * NUM_PANEL_PER_PIN)
#define WALL_HEIGHT    (PANEL_HEIGHT * NUM_PINS)

// Data pin that led data will be written out over
#define DATA_PIN_SET_ONE   5
#define DATA_PIN_SET_TWO   6
#define DATA_PIN_SET_THREE 9
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8

CRGB leds[NUM_LEDS];
int canvas[NUM_LEDS];


void setup() {
  Serial.begin(250000);
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
   
  FastLED.addLeds<WS2811, DATA_PIN_SET_ONE  >(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, DATA_PIN_SET_TWO  >(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, DATA_PIN_SET_THREE>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  
  for( int x = 0 ; x < NUM_LEDS ; x++ ) {
    leds[x] = CRGB(0,0,0);
  }
  FastLED.show();
}

#define FPS (15)
#define MS_PER_FRAME (1000.0/FPS)

void loop() {
  Serial.print("*");
  long start = millis();
  int x=0;
  while(millis() - start < MS_PER_FRAME) {
    if(Serial.available()) {
      char r=Serial.read();
      char g=Serial.read();
      char b=Serial.read();
      leds[x++]=CRGB(r,g,b);
    }
  }
  FastLED.show();
}
