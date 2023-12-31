#include "FastLED.h"
#include <OneButton.h>

#define NUM_STRIPS 5
#define NUM_LEDS_PER_STRIP 10
#define BTN_PIN   13
#define RST_PIN   12
#define RELAY_ONE_PIN A0
#define RELAY_TWO_PIN A1


bool reset = false;
bool useOrange = false;
int pattern = 0;
int wavePasssed = 0;
bool firstOn = true;

CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

uint8_t colorIndex[NUM_LEDS_PER_STRIP];
uint8_t whichPalette = 0;

DEFINE_GRADIENT_PALETTE( redWave_gp ) { //TODO: Make this actually a red Wave
    0,    0,  194,  255,     //light blue
   46,    3,    0,  246,     //dark blue
  176,   55,  222,   70,     //bright green
  255,    0,  194,  255      //light blue
};

DEFINE_GRADIENT_PALETTE( orangeWave_gp ) {  //TODO: Make this actually an orange Wave
    0,  255,  100,    0,     //orange
   90,  255,    0,  255,     //magenta
  150,  255,  100,    0,     //orange
  255,  255,  100,    0      //orange
};

DEFINE_GRADIENT_PALETTE( redOrange_gp ) {  //TODO: Make this actually a red/ orange Wave
    0,    6,  255,    0,     //green
   71,    0,  255,  153,     //bluegreen
  122,  200,  200,  200,     //gray
  181,  110,   61,    6,     //brown
  255,    6,  255,    0      //green
};

CRGBPalette16 currentPalette(orangeWave_gp);
CRGBPalette16 targetPalette(redWave_gp);
CRGBPalette16 redOrange(redOrange_gp);

OneButton btn = OneButton(BTN_PIN, true, true);
OneButton rst = OneButton(RST_PIN, true, true);

void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 3>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(leds[2], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds[3], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds[4], NUM_LEDS_PER_STRIP);


  FastLED.setBrightness(50);

  Serial.begin(57600);


  btn.attachClick(nextPattern);
  rst.attachClick(patOff);

  pinMode(RELAY_ONE_PIN, OUTPUT);
  pinMode(RELAY_TWO_PIN, OUTPUT);
  
}


void loop() {
  if (reset == true){
    pattern = 0;
  }
  switch (pattern) {
    case 0:
      debug(); 
      break;
    case 1:
      stepOne();
      break;
    case 2:
      stepOne();
      digitalWrite(RELAY_ONE_PIN, HIGH);
      break;
    case 3:
      stepOne();
      digitalWrite(RELAY_TWO_PIN, HIGH);
      break;
  }

  btn.tick();
  rst.tick();
  FastLED.show();
}

void nextPattern(){

  pattern++;

}

void patOff() {
  reset = true;
  digitalWrite(RELAY_ONE_PIN, LOW);
  digitalWrite(RELAY_TWO_PIN, LOW);
}

void debug() {

  if (firstOn == true){
for (int i = 0; i < NUM_STRIPS; i++){
    for (int x = 0; x < NUM_LEDS_PER_STRIP; x++){
      fill_solid(leds[i], NUM_LEDS_PER_STRIP, CRGB::Blue); //Just a Placeholder;

          }
        }
FastLED.show();
delay(100);
firstOn = false;
}
  reset = false;
  for (int i = 0; i < NUM_STRIPS; i++){
    for (int x = 0; x < NUM_LEDS_PER_STRIP; x++){
      fill_solid(leds[i], NUM_LEDS_PER_STRIP, CRGB::Black); //Just a Placeholder;


          }
        }
}

void stepOne() {
  for (int i = 0; i < NUM_STRIPS; i++){
    for (int x = 0; x < NUM_LEDS_PER_STRIP; x++){
      fill_solid(leds[i], NUM_LEDS_PER_STRIP, CRGB::Red); //Just a Placeholder;
            }
        }


  for (int x = 0; x < NUM_STRIPS; x++){         // this will come after the wave and just twinkle in the red/ orange pallette
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = ColorFromPalette(redOrange, colorIndex[i]);
    }
  }



  FastLED.show();
}

void twinkleOrangRed(){

  for (int x = 0; x < NUM_STRIPS; x++){
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      colorIndex[i] = random8();
    }
  }

  for (int x = 0; x < NUM_STRIPS; x++){         // this will come after the wave and just twinkle in the red/ orange pallette
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = ColorFromPalette(redOrange, colorIndex[i]);
    }
  }

  EVERY_N_MILLISECONDS(5){
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {  //We need an x for the first part of the array?
      colorIndex[i]++;
    }
  }
}

void stepTwo(){

}
  //hi matt
