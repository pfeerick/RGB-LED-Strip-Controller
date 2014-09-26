#define DEBUG

#define IR_INCREASE_BRIGHTNESS 0xF700FF
#define IR_DECREASE_BRIGHTNESS 0xF7807F

#define IR_ON  0xF7C03F
#define IR_OFF 0xF740BF

#define IR_RED  0xF720DF
#define IR_GREEN 0xF7A05F
#define IR_BLUE  0xF7609F
#define IR_WHITE 0xF7E01F

#define IR_ORANGE_DK  0xF710EF
#define IR_ORANGE_MED 0xF730CF
#define IR_ORANGE_LT  0xF708F7
#define IR_YELLOW     0xF728D7

#define IR_GREEN_LT   0xF7906F
#define IR_BLUE_LT    0xF7B04F
#define IR_BLUE_MED   0xF78877
#define IR_BLUE_DK    0xF7A857

#define IR_BLUE_ALT   0xF750AF
#define IR_PURPLE_DK  0xF7708F
#define IR_PURPLE_MED 0xF748B7
#define IR_PURPLE_LT  0xF76897

#define IR_MODE_FLASH  0xF7D02F
#define IR_MODE_STROBE 0xF7F00F
#define IR_MODE_FADE   0xF7C837
#define IR_MODE_SMOOTH 0xF7E817


/*
 * RGB LED Strip Controller - control a common anode 12v RGB led light strip with a IR remote
 * Version 0.1, September, 2014
 * Copyright 2014 Peter Feerick
  */

#include <IRremote.h>

#define RECV_PIN A3
#define mRED_PIN 5
#define mGREEN_PIN 6
#define mBLUE_PIN 9

IRrecv irrecv(RECV_PIN);

decode_results results;

boolean powerFlg = false; // current power state, defaults to false
int MODE = 0; // defaults to off
int LAST_COLOUR_SET[3] = {255,255,255}; //defaults to white

int ledPins[] = {mRED_PIN, mGREEN_PIN, mBLUE_PIN}; 

const byte COLOUR_PRIMARY_RED[] = {255,0,0};
const byte COLOUR_PRIMARY_GREEN[] = {0,255,0};
const byte COLOUR_PRIMARY_BLUE[] = {0,0,255};
const byte COLOUR_WHITE[] = {255,255,255};
  
const byte COLOUR_ORANGE[] = {237,120,6};
const byte COLOUR_MOSS_GREEN[] = {16,192,16};
const byte COLOUR_BRILLIANT_BLUE[] = {0,129,184};

const byte COLOUR_PURPLE[] = {128,0,255};

const byte COLOUR_LIGHT_SKY_BLUE[] = {42,165,85};
const byte COLOUR_SKY_BLUE[] = {0,145,212};
const byte COLOUR_BRIGHT_BLUE[] = {0,121,192};

const byte COLOUR_YELLOW[] = {255,255,0};

void setup()
{
  #ifdef DEBUG
  Serial.begin(9600);
  #endif DEBUG
  pinMode(13,OUTPUT);
  pinMode(RECV_PIN, INPUT);
  pinMode(mRED_PIN,OUTPUT);
  pinMode(mBLUE_PIN,OUTPUT);
  pinMode(mGREEN_PIN,OUTPUT);
  
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    #ifdef DEBUG
    Serial.println(results.value, HEX);
    #endif
    switch(results.value)
    {
      case IR_ON:
        if (powerFlg != true) setPower(true);
        break;
      
      case IR_OFF:
        if (powerFlg != false) setPower(false);
        break;
      
      case IR_RED:
    #ifdef DEBUG
    Serial.println("RED");
    #endif    
      
        setColour(ledPins,COLOUR_PRIMARY_RED);
        break;
      
      case IR_GREEN:
    #ifdef DEBUG
    Serial.println("GREEN");
    #endif    
      
        setColour(ledPins,COLOUR_PRIMARY_GREEN);      
        break;

      case IR_BLUE:
    #ifdef DEBUG
    Serial.println("BLUE");
    #endif    
      
        setColour(ledPins,COLOUR_PRIMARY_BLUE);
        break;
        
      case IR_WHITE:
    #ifdef DEBUG
    Serial.println("WHITE");
    #endif    
      
        setColour(ledPins,COLOUR_WHITE);
        break;
      
      default:
    #ifdef DEBUG
    Serial.println("Unknown");
    #endif    
      
      break;      
    }
    
    digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(13,LOW);
    delay(50);
    
    irrecv.resume(); // Receive the next value
  }
}

void setPower(boolean state)
{
  if (state == true) //power on
  {
    powerFlg = true;
    #ifdef DEBUG
    Serial.println("ON");
    #endif    
    analogWrite(mRED_PIN,LAST_COLOUR_SET[0]);
    analogWrite(mGREEN_PIN,LAST_COLOUR_SET[1]);
    analogWrite(mBLUE_PIN,LAST_COLOUR_SET[2]);      
  }
  else //power off
  {
    #ifdef DEBUG
    Serial.println("OFF");
    #endif    
    
    powerFlg = false;
    digitalWrite(mRED_PIN,0);
    digitalWrite(mGREEN_PIN,0);
    digitalWrite(mBLUE_PIN,0);
  }
}

// on, off, fade, smooth, blink,etc
void setMode(int mode)
{
  switch(mode)
  {
  case 0: //NORMAL
    break;

  case 1: //FLASH
    break;

  case 2: //STROBE
    break;

  case 3: //FADE
    break;

  case 4: //SMOOTH
    break;

  default:
    break;
  }
}

// needs RGB value
void setColour(int* led, byte* colour)
{
  if (powerFlg == true)
  {
    //iterate through each of the three pins (red green blue)  
    for(int i = 0; i < 3; i++)
    {           
      analogWrite(led[i], colour[i]);
      LAST_COLOUR_SET[i] = colour[i];
    }
  }
}

void setColour(int* led, const byte* colour)
{
  byte tempByte[] = {
    colour[0], colour[1], colour[2]  }; 
  setColour(led, tempByte);
}

// needs either positive no. for increase or negative for decrease
void incBrightness()
{
  if (LAST_COLOUR_SET[0] >= 245)
  {
    LAST_COLOUR_SET[0] = 255;
  }
  else
  {
    LAST_COLOUR_SET[0] += 10;
  }

  if (LAST_COLOUR_SET[1] >= 245)
  {
    LAST_COLOUR_SET[1] = 255;
  }
  else
  {
    LAST_COLOUR_SET[1] += 10;
  }

  if (LAST_COLOUR_SET[2] >= 245)
  {
    LAST_COLOUR_SET[2] = 255;
  }
  else
  {
    LAST_COLOUR_SET[2] += 10;     
  }

  if (powerFlg == true)
  {
    analogWrite(mRED_PIN,LAST_COLOUR_SET[0]);
    analogWrite(mGREEN_PIN,LAST_COLOUR_SET[1]);
    analogWrite(mBLUE_PIN,LAST_COLOUR_SET[2]);
  }
}


void decBrightness()
{
  if (LAST_COLOUR_SET[0] <= 10)
  {
    LAST_COLOUR_SET[0] = 0;
  }
  else
  {
    LAST_COLOUR_SET[0] -= 10;
  }

  if (LAST_COLOUR_SET[1] <= 10)
  {
    LAST_COLOUR_SET[1] = 0;
  }
  else
  {
    LAST_COLOUR_SET[1] -= 10;
  }

  if (LAST_COLOUR_SET[2] <= 10)
  {
    LAST_COLOUR_SET[2] = 0;  
  }
  else
  {
    LAST_COLOUR_SET[2] -= 10;
  }

  if (powerFlg == true)
  {
    analogWrite(mRED_PIN,LAST_COLOUR_SET[0]);
    analogWrite(mGREEN_PIN,LAST_COLOUR_SET[1]);
    analogWrite(mBLUE_PIN,LAST_COLOUR_SET[2]);
  }  
}
