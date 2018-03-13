#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

uint8_t data[3] ; 
const uint8_t buffer_size = sizeof(data);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };//two pipes communication 

const int analogInPinX = A0;// 
const int analogInPinY= A1;


int Str_value1 = 0;
int Str_value2 = 0;  


void setup(void)
{
  Serial.begin(9600);
  pinMode(analogInPinX,INPUT );
  pinMode(analogInPinY,INPUT );

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.stopListening();

}

void loop(void)
{

  Str_value1 = analogRead(analogInPinX);
  Str_value2 = analogRead(analogInPinY); 

  Str_value1 = Str_value1/10;//scaling Str_value1
  data[0] = Str_value1;
  Str_value2 = Str_value2/10;//scaling Str_value2
  data[1] = Str_value2;
  /////////////////////////////////////////////////////////////////////////
  radio.stopListening();

  bool ok = radio.write(  data ,sizeof(data) );
  delay(30);
  radio.startListening();
  delay(20);
  if (ok){
    Serial.print("data[0]=");
    Serial.print(data[0]);
    Serial.print(" data[1]=");
    Serial.print(data[1]);
    Serial.print(" data[2]=");
    Serial.println(data[2]);
  }
  else
    Serial.println("Failed");

}//void loop()
