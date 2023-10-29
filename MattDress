#include "FastLED.h"
#include <OneButton.h>

#define NUM_STRIPS 5
#define NUM_LEDS_PER_STRIP 10
#define BTN_PIN   12
#define RST_PIN   13
#define RELAY_ONE_PIN A0
#define RELAY_TWO_PIN A1


bool reset = false;
bool redOrange = false;
int pattern = 0;
int wavePasssed = 0;
bool firstOn = true;
bool found = false;


int fadingUpDumb[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int fadingUp[10][2] = {
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0}
  } ; //the first value is the position, the second is the pallette index.
int twinkling[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int stripWeAreOn = 0;
int doneStrips[] = {-1,-1,-1,-1,-1};
int myBrainHurts = 0;



CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];


uint8_t colorIndex[NUM_LEDS_PER_STRIP];
uint8_t colorIndex2[NUM_LEDS_PER_STRIP];
uint8_t whichPalette = 0;

DEFINE_GRADIENT_PALETTE( redWave_gp ) {
  0,    0,  0,  0,     //Black
  10,   255,  25,  0,     //Orange
  255,  150,  0,   0,     //Low Orange

};

DEFINE_GRADIENT_PALETTE( smolder_gp ) {
    0,   255,  15,  0,     //orange
  128,   50,  0,  0,     //red
  255,   255,  25,  0,     //orange

};

CRGBPalette16 currentPalette(redWave_gp);
CRGBPalette16 redWave(redWave_gp);
CRGBPalette16 smolder(smolder_gp);

OneButton btn = OneButton(BTN_PIN, true, true);
OneButton rst = OneButton(RST_PIN, true, true);

void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 3>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(leds[2], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds[3], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds[4], NUM_LEDS_PER_STRIP);


  FastLED.setBrightness(255);

  Serial.begin(57600);


  btn.attachClick(nextPattern);
  rst.attachClick(patOff);

  pinMode(RELAY_ONE_PIN, OUTPUT);
  pinMode(RELAY_TWO_PIN, OUTPUT);
  

  //Fill the colorIndex array with random numbers
 for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
   colorIndex[i] = random8();
 }
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
    Serial.print("First On!");
for (int i = 0; i < NUM_STRIPS; i++){
    for (int x = 0; x < NUM_LEDS_PER_STRIP; x++){
      fill_solid(leds[i], NUM_LEDS_PER_STRIP, CRGB::Blue); //Just a Placeholder;

          }
        }
delay(1000);
firstOn = false;
}
  reset = false;
  for (int i = 0; i < NUM_STRIPS; i++){
    for (int x = 0; x < NUM_LEDS_PER_STRIP; x++){
      fill_solid(leds[i], NUM_LEDS_PER_STRIP, CRGB::Black); //Just a Placeholder;

          }
        }
myBrainHurts = 0;
stripWeAreOn= 0;
        for( int x =0; x < NUM_LEDS_PER_STRIP; x++){
          fadingUpDumb[x] = -1;
          twinkling[x] = -1;
          fadingUp[x][0] = -1;
          fadingUp[x][-1] = 0;
          Serial.print("we reset from debug");
          }
for (int x = 0; x < NUM_STRIPS; x++){
 doneStrips[x] = -1;
}

}

void stepOne() {

int allDone = 0;
for( int x = 0; x < NUM_STRIPS; x++){
allDone = allDone + doneStrips[x];
}

if (allDone < 0){
 
  
  EVERY_N_SECONDS(1){ //Add a pixel to the list of pixels fading up.
    found = false;
    Serial.print("we got to the add pixel part");
    for (int i = 0;  i < NUM_LEDS_PER_STRIP; i++){
      if (found == false){
        if (fadingUpDumb[i] == -1){
          if (twinkling[i] == -1){
            fadingUpDumb[i] = i;
            fadingUp[i][0] =  i;  //adds it to the fading up list
            Serial.print("we got to the add pixel part farts farts farts");
            found = true;
            }
          }

        }
    }
  }

EVERY_N_MILLISECONDS(10){ //fade up each pixel that wants to fade up
  //Serial.print("we got to the fade pixel part"); //confimed we get here
  for(int x = 0; x < NUM_LEDS_PER_STRIP; x++){
    if (fadingUp[x][0] != -1){
      leds[stripWeAreOn][x] = ColorFromPalette( redWave, fadingUp[x][1]);
      if(fadingUp[x][1] < 254){
        fadingUp[x][1]++;
        Serial.print("we got to the fade pixel part farts farts farts");
      }
    }
  }
}


  for(int x = 0; x < NUM_LEDS_PER_STRIP; x++){
    if (fadingUp[x][1] == 254){
      twinkling[x] = x;
      myBrainHurts++;
      Serial.print("we added soemthing to twinkle");
    }  //right now, the whole strip will stay at the end of red wave until the entire strip is there and then it will twinkle
 
    }

//check if the whole strip is 255



   
      if (myBrainHurts >= NUM_LEDS_PER_STRIP){ // if the strip is done with the wave, flag it as done, move onto the next strip, and reset the twinking and fading.
        doneStrips[stripWeAreOn] = 0;
        stripWeAreOn++;
        myBrainHurts = 0;
        Serial.print("we reset and advanced");
        for( int x =0; x < NUM_LEDS_PER_STRIP; x++){
          fadingUpDumb[x] = -1;
          twinkling[x] = -1;
          fadingUp[x][0] = -1;
          fadingUp[x][-1] = 0;
          
          } 
        }
}
  // Color each pixel from the palette using the index from colorIndex[]
  for (int x = 0; x < NUM_STRIPS; x++){
    if (doneStrips[x] == 0)
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = ColorFromPalette(smolder, colorIndex[i]);
    }
  }
  EVERY_N_MILLISECONDS(10){
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      colorIndex[i]++;
    }
  }
  FastLED.show();
}

