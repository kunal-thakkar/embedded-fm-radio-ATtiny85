#include <Wire.h>
#include "RDA5807.h"

#define CH_BUTTON 3
#define V_PLUS    4
#define V_MINUS   5

RADIO radio;

void setup()
{
  pinMode(CH_BUTTON, INPUT_PULLUP);
  pinMode(V_PLUS, INPUT_PULLUP);
  pinMode(V_MINUS, INPUT_PULLUP);
  radio.init();
  radio.next_channel();
}

void loop()
{
  if(digitalRead(CH_BUTTON) == LOW){
    radio.next_channel();
    while(digitalRead(CH_BUTTON) == LOW);
  }
  if(digitalRead(V_PLUS) == LOW){
    radio.vol_up();
    while(digitalRead(V_PLUS) == LOW);
  }
  if(digitalRead(V_MINUS) == LOW){
    radio.vol_down();
    while(digitalRead(V_MINUS) == LOW);
  }
}
