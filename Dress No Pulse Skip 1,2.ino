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
bool what = false;

uint8_t currentIndex [5][14] = {
  {128,128,128,128,128,128,128,128,128,128,128,128,128,128},
  {128,128,128,128,128,128,128,128,128,128,128,128,128,128},
  {128,128,128,128,128,128,128,128,128,128,128,128,128,128},
  {128,128,128,128,128,128,128,128,128,128,128,128,128,128},
  {128,128,128,128,128,128,128,128,128,128,128,128,128,128}
  };


int fadingUpDumb[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; //Please don't jusge me. I'm a python boy and static arrays are scary.
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
int stripWeAreOn = 2;
int doneStrips[] = {0,0,-1,-1,-1,0};
int myBrainHurts = 0;

int veryStupid14[14] = {13,12,11,10,9,8,7,6,5,4,3,2,1,0} ; 
int veryStupid10[10] = {9,8,7,6,5,4,3,2,1,0} ; 



CRGB leds[NUM_STRIPS][14]; 


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
  


 for (int i = 0; i < 14; i++) {   //Fill the colorIndex array with random numbers
   colorIndex[i] = random8();
 }
}

void loop() {    //checks what step we are on.
  
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

int flippityDoo(int _number, int strip){
  if (checkIf16(strip) == 14){
    return (veryStupid14[_number]);

  }else{
    return (veryStupid10[_number]);
  }



}

int checkIf16(int strip){ //This will check if we are on strip #5 and return 14 if we are. 10 if we are not.
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
  Serial.println("Reset Pressed.");
}

void debug() {  //This runs the first time we power on and after a reset and after power on until the next button is pressed.

  if (firstOn == true){    //if it's the first time we are on, be blue for .5 seconds.
    Serial.println("First On!");
      for (int i = 0; i < NUM_STRIPS; i++){
      fill_solid(leds[i], checkIf16(i), CRGB::Blue); 

    }
    FastLED.show();
     delay(500);
    firstOn = false; 
    }
    
  reset = false;
  
  for (int i = 0; i < NUM_STRIPS; i++){
    fill_solid(leds[i], checkIf16(i), CRGB::Black); //makes everything black.

    }
        
  myBrainHurts = 0; //starts resetting variables. (I think this one is no longer needed...)
  stripWeAreOn = 2;
    
  for( int x =0; x < 14; x++){
    fadingUpDumb[x] = -1;
    fadingUp[x][0] = -1;
    fadingUp[x][-1] = 0;
    twinkling[x] = 0;

    }
            
          
  for (int x = 0; x < NUM_STRIPS; x++){
    doneStrips[x] = -1;
    for(int i = 0; i < 14; i++){
      currentIndex[x][i] = 128;
      }
    }
  doneStrips[0] = 0;
  doneStrips[1] = 0;
  
  }

void stepOne() {

  int allDone = 0;
  for( int x = 0; x < NUM_STRIPS; x++){  //Checks if all the strips are done. Skips it if they are.
  allDone = allDone + doneStrips[x];
  }
  if (allDone < 0){


     
 EVERY_N_MILLISECONDS(100){ //Add a pixel to the list of pixels fading up. Changing this time makes the wave advance faster or slower.
    
    found = false; //Look, this FEELS not needed, but here it is.
    for (int i = 0 ;  i < checkIf16(stripWeAreOn); i++){


      int x = flippityDoo(i, stripWeAreOn); 


      if (found == false){
        if (fadingUpDumb[x] == -1){ //Is this already in the fading list?
          if (twinkling[x] == 0){ //Is this already in the twinkling list?
            fadingUpDumb[x] = 0;
            fadingUp[x][0] =  0;  //adds it to the fading up list
            found = true;
            }
          }

        }
    }
  }



  EVERY_N_MILLISECONDS(1){ //fade up each pixel that wants to fade up. Changing this time makes the pixels go through the pallet faster or slower.

    for(int x = 0; x < checkIf16(stripWeAreOn); x++){
      int i = flippityDoo(x, stripWeAreOn);
      if (fadingUp[i][0] == 0){
        leds[stripWeAreOn][i] = ColorFromPalette( redWave, fadingUp[i][1]);
        if(fadingUp[i][1] < 254){
          fadingUp[i][1]++;
          }else{
            twinkling[i] = 1;
            fadingUp[i][0] = -1;

        }
      }
    }
  }
   
  int myBrainHurts = 0; //this should have it be either 10 or 16
  for( int x = 0; x < checkIf16(stripWeAreOn); x++){
    myBrainHurts = myBrainHurts + twinkling[x]; //if everything is twinkling, it should be either 10 or 14.
    }

    if (myBrainHurts == checkIf16(stripWeAreOn)){ // if the strip is done with the wave, flag it as done, move onto the next strip, and reset the twinking and fading.
      what = true;
      Serial.println("we reset and advanced");
      Serial.println(stripWeAreOn);
      for( int x =0; x < checkIf16(stripWeAreOn); x++){
        fadingUpDumb[x] = -1;
        twinkling[x] = 0;
        fadingUp[x][0] = -1;
        fadingUp[x][-1] = 0;
        }
      }

  if(what == true){  //Look, I KNOW this is stupid, but there was some really weird behavior and this helped debbug and now if it ain't broke, dont fix it.
    doneStrips[stripWeAreOn] = 0;
    stripWeAreOn++;
    what = false;  
  }
}

//Below here is the smouldering stuff.

for (int x = 0; x < 5; x++){
  if (doneStrips[x] == 0)
  for (int i = 0; i < checkIf16(x); i++) {
    leds[x][i] = ColorFromPalette(smolder, currentIndex[x][i]);
  }
}
EVERY_N_MILLISECONDS(10){  //Changing this time makes the smoulder faster or slower.
  
  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    colorIndex[i]++;
    for (int x  = 0; x < NUM_STRIPS; x++){
      if (doneStrips[x] == 0){
        
        if (currentIndex[x][i] < colorIndex[i]){
            currentIndex[x][i]++;
        }
        if (currentIndex[x][i] > colorIndex[i]){
            currentIndex[x][i]--;
      }
    }
    
    
  }

   EVERY_N_MILLISECONDS(80){  //Changing this time changes how quickly the strip moved into the smouldering effect after the wave.
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      for (int x  = 0; x < NUM_STRIPS; x++){
        if (doneStrips[x] == 0){
          if(currentIndex[x][i] != colorIndex[i]){
            if (currentIndex[x][i] < colorIndex[i]){
            currentIndex[x][i]++;
            }
            if (currentIndex[x][i] > colorIndex[i]){
                currentIndex[x][i]--;
            }
        }
      }
    
      }
   }
   }
  }
}
  FastLED.show();
}
