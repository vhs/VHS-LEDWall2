#include "FastSPI_LED2.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// RGB Calibration code
//
// Use this sketch to determine what the RGB ordering for your chipset should be.  Steps for setting up to use:

// * Uncomment the line in setup that corresponds to the LED chipset that you are using.  (Note that they
//   all explicitly specify the RGB order as RGB)
// * Define DATA_PIN to the pin that data is connected to.
// * (Optional) if using software SPI for chipsets that are SPI based, define CLOCK_PIN to the clock pin
// * Compile/upload/run the sketch 

// You should see six leds on.  If the RGB ordering is correct, you should see 1 red led, 2 green 
// leds, and 3 blue leds.  If you see different colors, the count of each color tells you what the 
// position for that color in the rgb orering should be.  So, for example, if you see 1 Blue, and 2
// Red, and 3 Green leds then the rgb ordering should be BRG (Blue, Red, Green).  

// You can then test this ordering by setting the RGB ordering in the addLeds line below to the new ordering
// and it should come out correctly, 1 red, 2 green, and 3 blue.
//
//////////////////////////////////////////////////

#define NUM_LEDS_PER_PANEL 64
#define NUM_PANEL_PER_PIN   4
#define NUM_PINS            3

#define NUM_LEDS           ( NUM_PANEL_PER_PIN * NUM_LEDS_PER_PANEL * NUM_PINS )
#define NUM_LEDS_PER_STRIP ( NUM_PANEL_PER_PIN * NUM_LEDS_PER_PANEL ) 

#define NUM_CHANNELS        (NUM_LEDS*3)

// Data pin that led data will be written out over
#define DATA_PIN_SET_ONE   5
#define DATA_PIN_SET_TWO   6
#define DATA_PIN_SET_THREE 9
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8

CRGB leds[NUM_LEDS];
int canvas[NUM_LEDS];

char serial_buffer[64];
char serial_pos;


void setup() {
        Serial.begin(115200);
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);
   
        FastLED.addLeds<WS2811, DATA_PIN_SET_ONE  >(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
        FastLED.addLeds<WS2811, DATA_PIN_SET_TWO  >(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
        FastLED.addLeds<WS2811, DATA_PIN_SET_THREE>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

      // Uncomment one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      //LEDS.showColor(CRGB(0,0,0));


      for( int x = 0 ; x < NUM_LEDS ; x++ ) {
        leds[x] = CRGB(0,0,0);
      }
      FastLED.show();
      
      serial_pos=0;
      Serial.println(">");
}

void loop() {
  if(Serial.available()) {
    char c = Serial.read();
    Serial.print(c);
    
    if((c!='\n' && c!='\r')) {
      if(serial_pos<64) {
        serial_buffer[serial_pos++]=c;
      }
    } else {
      Serial.print("\n");
      serial_buffer[serial_pos]=0;
      serial_pos=0;
      int pixel_id=(int)atof(serial_buffer);
      if(pixel_id<0)pixel_id=0;
      if(pixel_id>NUM_LEDS) pixel_id=NUM_LEDS;
      
      for( int x = 0 ; x < NUM_LEDS ; x++ ) {
        leds[x] = CRGB(0,0,0);
      }
      leds[pixel_id]=CRGB(255,255,255);
      FastLED.show();

      Serial.print("lighting only ");
      Serial.println(pixel_id);
      Serial.println(">");
    }
  }
}
