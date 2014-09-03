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

char serial_buffer[64];
char serial_pos;
int counter=0;


void setup() {
        Serial.begin(115200);
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);
   
        FastLED.addLeds<WS2811, DATA_PIN_SET_ONE  >(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
        FastLED.addLeds<WS2811, DATA_PIN_SET_TWO  >(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
        FastLED.addLeds<WS2811, DATA_PIN_SET_THREE>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

      for( int x = 0 ; x < NUM_LEDS ; x++ ) {
        leds[x] = CRGB(0,0,0);
      }
      FastLED.show();
      
      serial_pos=0;
      Serial.print("w=");
      Serial.println(WALL_WIDTH);
      Serial.print("h=");
      Serial.println(WALL_HEIGHT);
}

int led_map(int input) {
  int row = input / NUM_LEDS_PER_STRIP;
  input %= NUM_LEDS_PER_STRIP;
  
  int y = input / ( PANEL_WIDTH * NUM_PANEL_PER_PIN );
  int x = input % ( PANEL_WIDTH * NUM_PANEL_PER_PIN );
  
  if(x%2) {
    y = 7-y;
  }
  
  int output = row * NUM_LEDS_PER_STRIP
             + x * PANEL_HEIGHT
             + y;
  return output;
}

int led_map(int x,int y) {
  return led_map(y * PANEL_WIDTH * NUM_PANEL_PER_PIN + x);
}


void set_wall(CRGB color) {
  for( int x = 0 ; x < NUM_LEDS ; x++ ) {
    leds[x] = color;
  }
}


void test_linear_mapping() {
  for(counter=0;counter<NUM_LEDS;++counter) {
    set_wall(CRGB(0,0,0));
    leds[led_map(counter)]=CRGB(255,255,255);
    FastLED.show();
  }
}

void test_xy_mapping() {
  for(int x=0;x<WALL_WIDTH;++x) {
    set_wall(CRGB(0,0,0));
    leds[led_map(x,0)]=CRGB(255,255,255);
    FastLED.show();
  }
  for(int y=0;y<WALL_HEIGHT;++y) {
    set_wall(CRGB(0,0,0));
    leds[led_map(WALL_WIDTH-1,y)]=CRGB(255,255,255);
    FastLED.show();
  }
  for(int x=WALL_WIDTH-1;x>=0;--x) {
    set_wall(CRGB(0,0,0));
    leds[led_map(x,WALL_HEIGHT-1)]=CRGB(255,255,255);
    FastLED.show();
  }
  for(int y=WALL_HEIGHT-1;y>=0;--y) {
    set_wall(CRGB(0,0,0));
    leds[led_map(0,y)]=CRGB(255,255,255);
    FastLED.show();
  }
}

float px=0;
float py=0;
float dx=0.5;
float dy=0.5;

void test_rainbow_circle() {
  px+=dx;
  py+=dy;
  if(px>WALL_WIDTH) {
    px=WALL_WIDTH;
    dx=-dx;
  } else if(px<0) {
    px=0;
    dx=-dx;
  }
  if(py>WALL_HEIGHT) {
    py=WALL_HEIGHT;
    dy=-dy;
  } else if(py<0) {
    py=0;
    dy=-dy;
  }
  
  int x,y;
  for(y=0;y<WALL_HEIGHT;++y) {
    for(x=0;x<WALL_WIDTH;++x) {
      float nx = (float)(px-x);
      float ny = (float)(py-y);
      float v = sqrt(nx*nx + ny*ny);
      
      // color map v to RGB?
      int r=cos(v)*127 + 127;
      int g=sin(v)*127 + 127;
      int b=(cos(v)*sin(v))*127 + 127;
      leds[led_map(x,y)] = CRGB(r,g,b);
    }
  }
  FastLED.show();
  //delay(10);
}


void loop() {
  //test_linear_mapping();
  //test_xy_mapping();
  test_rainbow_circle();
}
