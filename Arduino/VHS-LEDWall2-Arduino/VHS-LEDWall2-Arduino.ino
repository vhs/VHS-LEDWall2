#include "FastSPI_LED2.h"

int heartbeatled = 13;

#define DEBUG 0

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

#define FPS (15.0)
#define MS_PER_FRAME (1000.0/FPS)
int pixel_count=0;
long frame_count=0;
int r,g,b;

int buffer[NUM_CHANNELS];
int buff_count = 0;
int counter = 0;

void setup() {
  Serial.begin(512000);
//  Serial.begin(256000);
//  Serial.begin(115200);
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(heartbeatled, OUTPUT);     
   
  FastLED.addLeds<WS2811, DATA_PIN_SET_ONE  >(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, DATA_PIN_SET_TWO  >(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, DATA_PIN_SET_THREE>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  
  for( int x = 0 ; x < NUM_LEDS ; x++ ) {
    leds[x] = CRGB(0,0,0);
  }

  if( DEBUG )
    Serial.print( "Initiating buffer..." );
    
  for( int i = 0 ; i < NUM_CHANNELS ; i++ ) {  
    buffer[i] = (int) 0;
    digitalWrite(heartbeatled, HIGH);   // turn the LED on (HIGH is the voltage level)
//    delay(50);               // wait for a second
    digitalWrite(heartbeatled, LOW);    // turn the LED off by making the voltage LOW
//    delay(50);               // wait for a second
  }
  if( DEBUG )
    Serial.println( "done!" );

  digitalWrite(heartbeatled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);               // wait for a second
  digitalWrite(heartbeatled, LOW);    // turn the LED off by making the voltage LOW
  delay(250);               // wait for a second
  digitalWrite(heartbeatled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);               // wait for a second
  digitalWrite(heartbeatled, LOW);    // turn the LED off by making the voltage LOW
  delay(250);               // wait for a second
  digitalWrite(heartbeatled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);               // wait for a second
  digitalWrite(heartbeatled, LOW);    // turn the LED off by making the voltage LOW
  delay(250);               // wait for a second
  digitalWrite(heartbeatled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);               // wait for a second
  digitalWrite(heartbeatled, LOW);    // turn the LED off by making the voltage LOW
  delay(250);               // wait for a second
  Serial.println( "Online" );
}

void loop() {
  Serial.println("*");
  while( buff_count < NUM_CHANNELS ) {
    if( DEBUG )
      Serial.print(".");
    if( counter >= 100 ) {
      if( DEBUG )
        Serial.println( buff_count );
      counter=0;
    } else {
      counter++;
    }
    while( Serial.available() ) {
      if( DEBUG )
        Serial.print("o");
      int inp = Serial.read();
      if( inp != -1 ) {
        if( DEBUG )
          Serial.print("O");
        buffer[buff_count] = inp;
        buff_count++;
      }
    }
//    delay( 50 );
  }
  if( DEBUG ) {
      Serial.print( ":" );
      Serial.print( buff_count );
  }
  Serial.println( "#" );
    
  if( buff_count >= NUM_CHANNELS ) {
    Serial.println( ">>" );
    buff_count = 0;

    for(int x=0;x<NUM_LEDS;x++) {
//      Serial.print( "<<<" );
      int r_pos=x*3;
      int g_pos=r_pos+1;
      int b_pos=g_pos+1;
      leds[x]=CRGB(buffer[r_pos],buffer[g_pos],buffer[b_pos]);
//      Serial.print( "<<<[" );
//      Serial.print( r_pos );
//      Serial.print( "/" );
//      Serial.print( g_pos );
//      Serial.print( "/" );
//      Serial.print( b_pos );
//      Serial.println( "]" );
     }
     Serial.println( "!" );
  }

  FastLED.show();
  digitalWrite(heartbeatled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                         // wait for a second
  digitalWrite(heartbeatled, LOW);    // turn the LED off by making the voltage LOW
  Serial.println( "$" );
}
