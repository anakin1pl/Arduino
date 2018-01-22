#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN 9
#define CSN_PIN 10
  
// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
const int servo1 = 9; // first servo
const int servo2 = 10; // second servo
int servoVal; // variable to read the value from the analog pin

Servo myservo1; // create servo1
Servo myservo2; // create servo2
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[6];  // 6 element array holding Joystick readings

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  delay(1000);
  myservo1.attach(servo1); // attaches the servo1 
  myservo2.attach(servo2); // attaches the servo2
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();;
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while (!done)
    {
      done = radio.read( joystick, sizeof(joystick[2]) );
      
      // the four button variables from joystick array
      int upbut = joystick[2];
      int rightbut = joystick[3];
      int downbut = joystick[4];
      int lefttbut = joystick[5];
      
     
      Serial.print("X = ");
      
      Serial.print(joystick[0]);
      Serial.print(" Y = ");  
      
      Serial.print(joystick[1]);
      Serial.print(" Up = ");
      
      Serial.print(joystick[2]);
      Serial.print(" Right = ");  
      
      Serial.print(joystick[3]);
      Serial.print(" Down = ");
      
      Serial.print(joystick[4]);
      Serial.print(" Left = ");  
      
      Serial.println(joystick[5]);
     
     delay(15); // waits for the servo to get there
    }
  }
  else
  {    
      Serial.println("No radio available");
  }

}//--(end main loop )---
