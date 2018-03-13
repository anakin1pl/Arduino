#include <Servo.h> 
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };//2 pipes communication
uint8_t received_data[3];
uint8_t num_received_data =sizeof(received_data);

const int servo1 = 8;//  servo

Servo myservo;  // create servo object to control a servo
int servoval;  // variable to read the value from the analog pin


////
const int IN1 = 5;
const int IN2 = 4;
const int IN3 = 6;
const int IN4 = 7;

const int ENA = 3;
////

void setup(void)
{
  Serial.begin(9600);
  delay(300); //wait until the esc starts in case of Arduino got power first
  myservo.attach(servo1);  // attaches the servo on pin 3 to the servo object 
  myservo.write(90);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();

  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);

}//void setup

void loop(void)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  ///////////////////////////////Radio reading///////////////////////////////////////////////////////     
  if ( radio.available() )
  {
    bool done = false;
    while (!done)
    {
    done = radio.read(&received_data, num_received_data  );
    delay(30);
    }// while (!done)
   
    Serial.print(received_data[0]);
    Serial.print ("---"); 
   // Serial.print(received_data[1]);
   // Serial.print ("---"); 
   // Serial.print(received_data[2]);

    radio.stopListening();

    radio.startListening();
   
 /****************************************steering servo*********************************************************************/
    servoval = received_data[0];
   // servoval = servoval*10;//scale back
    servoval = map(servoval, 0, 102, 0, 200);//steering servo
    Serial.println(servoval);
    analogWrite(ENA, servoval);
    delay(30);
    

  }// if ( radio.available() )
}//void loop(void)
