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

uint8_t currentIndex [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};


int fadingUpDumb[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int fadingUp[14][2] = {
  {-1,0},
  {-1,0},
  {-1,0},
  {-1,0},
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
int twinkling[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int stripWeAreOn = 0;
int doneStrips[] = {-1,-1,-1,-1,-1};
int myBrainHurts = 0;



CRGB leds[NUM_STRIPS][14]; //this might work?


uint8_t colorIndex[14];

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

CRGBPalette16 redWave(redWave_gp);
CRGBPalette16 smolder(smolder_gp);

OneButton btn = OneButton(BTN_PIN, true, true);
OneButton rst = OneButton(RST_PIN, true, true);

void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 3>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(leds[2], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds[3], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds[4], 14);


  FastLED.setBrightness(255);

  Serial.begin(57600);


  btn.attachClick(nextPattern);
  rst.attachClick(patOff);

  pinMode(RELAY_ONE_PIN, OUTPUT);
  pinMode(RELAY_TWO_PIN, OUTPUT);
  

  //Fill the colorIndex array with random numbers
 for (int i = 0; i < 14; i++) {
   colorIndex[i] = random8();
 }

// for(int x = 0; x < checkIf16(stripWeAreOn); x++){  //checks if an led is at 255 and if so, set it to the twinkle array.
//      Serial.println(x);
//      Serial.println("This is checking the checkif16");
//    }
//    delay(5000);
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

void nextPattern(){ //this happend when the next button is pressed.

  pattern++;

}

int checkIf16(int strip){ //This will check if we are on strip #4 and return 16 if we are. 10 if we are not.
  if (strip == 4){
    return 14;
  } else {
    return 10;  
    }
  }

void patOff() {  //This happens when the reset button is pressed
  reset = true;
  digitalWrite(RELAY_ONE_PIN, LOW);
  digitalWrite(RELAY_TWO_PIN, LOW);
}

void debug() {  //this runs the first time we power on and after a reset and after power on until the next button is pressed.

  if (firstOn == true){ //if it's the first time we are on, be blue for 1 second.
    Serial.println("First On!");
      for (int i = 0; i < NUM_STRIPS; i++){
          fill_solid(leds[i], checkIf16(i), CRGB::Blue); 
            }
     delay(1000);
    firstOn = false; 
    }
    
  reset = false;
  
  for (int i = 0; i < NUM_STRIPS; i++){
    fill_solid(leds[i], checkIf16(i), CRGB::Black); //makes everything black.

    }
        
  myBrainHurts = 0; //starts resetting variables.
  stripWeAreOn= 0;
    
  for( int x =0; x < 14; x++){
            fadingUpDumb[x] = -1;
            fadingUp[x][0] = -1;
            fadingUp[x][-1] = 0;
            twinkling[x] = 0;
            //Serial.println("we reset from debug");
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


 

//Serial.println("we got to 1");
     
 EVERY_N_SECONDS(1){ //Add a pixel to the list of pixels fading up.
    found = false;
    //Serial.println("we got to the add pixel part");
    for (int i = 0;  i < checkIf16(stripWeAreOn); i++){
      if (found == false){
        if (fadingUpDumb[i] == -1){
          if (twinkling[i] == 0){
            fadingUpDumb[i] = 0;
            fadingUp[i][0] =  0;  //adds it to the fading up list
            Serial.println(i); 
            Serial.println("we got to the add pixel part farts farts farts"); 
            found = true;
            }
          }

        }
    }
  }

//Serial.println("we got to 2");

  EVERY_N_MILLISECONDS(10){ //fade up each pixel that wants to fade up
    //Serial.println("we got to the fade pixel part"); //confimed we get here
    for(int x = 0; x < checkIf16; x++){
      if (fadingUp[x][0] == 0){
        leds[stripWeAreOn][x] = ColorFromPalette( redWave, fadingUp[x][1]);
        if(fadingUp[x][1] < 254){
          fadingUp[x][1]++;
        }else{
          twinkling[x] = 1;
          Serial.println(x);
          Serial.println("is at 255");
          //Serial.println("we got to the fade pixel part farts farts farts");
        }
      }
    }
  }


//Serial.println("we got to 3");

//  for(int x = 0; x < checkIf16(stripWeAreOn); x++){  //checks if an led is at 255 and if so, set it to the twinkle array.
//    if (fadingUp[x][1] == 255){
//      twinkling[x] = 1;
//      Serial.println(x);
//      Serial.println("we added soemthing to twinkle");
//    }  //right now, the whole strip will stay at the end of red wave until the entire strip is there and then it will twinkle
// 
//    }

//Serial.println("we got to 4");
    
  int myBrainHurts = 0; //this should have it be either 10 or 16
  for( int x = 0; x < checkIf16(stripWeAreOn); x++){
    myBrainHurts = myBrainHurts + twinkling[x]; //if everything is twinkling, it should be either 10 or 16.
    }

   
      if (myBrainHurts == checkIf16(stripWeAreOn)){ // if the strip is done with the wave, flag it as done, move onto the next strip, and reset the twinking and fading.
        doneStrips[stripWeAreOn] = 0;

        Serial.println("we reset and advanced");
        for( int x =0; x < checkIf16(stripWeAreOn); x++){
          fadingUpDumb[x] = -1;
          twinkling[x] = 0;
          fadingUp[x][0] = -1;
          fadingUp[x][-1] = 0;
          }
         stripWeAreOn++;
        }
  }

 // Serial.println("we got to 5");
  
  // Color each pixel from the palette using the index from colorIndex[]
  for (int x = 0; x < NUM_STRIPS; x++){
    if (doneStrips[x] == 0)
    for (int i = 0; i < checkIf16(x); i++) {
      leds[x][i] = ColorFromPalette(smolder, currentIndex[i]);
    }
  }
  EVERY_N_MILLISECONDS(10){
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      colorIndex[i]++;
      currentIndex[i]++;
      if(currentIndex[i] != colorIndex[i]){
        currentIndex[i]++; //This is a dirty trick to get the smoulder to not jump to the index value, but fade to it.
      }
    }
  }
  FastLED.show();
}
