//Transmitter1 sketch
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);
const uint64_t pipes[6] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL, 0xF0F0F0F0C3LL,0xF0F0F0F0C4LL,0xF0F0F0F0C5LL,0xF0F0F0F0C6LL };

byte GloveOne[3]; 
byte val1;
byte val2;
byte G1 = 1;

void setup(void){
Serial.begin(57600);
radio.begin();
radio.openWritingPipe(pipes[1]);//write to rec
//radio.openReadingPipe(1,pipes[2]);//read from rec
}

void loop(void){
    GloveOne[0] = G1;
  val1 = analogRead(A0);   
/* or use   "val1 = (analogRead(15) >> 3);" for each analogue input to convert from 0-1023 val to 0-127 val for midi send*/
  GloveOne[1] = val1;
  val2 = analogRead(A1);
  GloveOne[2] = val2;
  radio.write( GloveOne, sizeof(GloveOne) );
}
