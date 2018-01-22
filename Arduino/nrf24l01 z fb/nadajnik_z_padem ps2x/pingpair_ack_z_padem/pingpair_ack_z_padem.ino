/*
  // March 2014 - TMRh20 - Updated along with High Speed RF24 Library fork
  // Parts derived from examples by J. Coliz <maniacbug@ymail.com>
*/
/**
   Example for efficient call-response using ack-payloads

   This example continues to make use of all the normal functionality of the radios including
   the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well.
   This allows very fast call-response communication, with the responding radio never having to
   switch out of Primary Receiver mode to send back a payload, but having the option to if wanting
   to initiate communication instead of respond to a commmunication.
*/



#include <SPI.h>
#include <PS2X_lib.h>  //for v1.6
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins CE 8, CSN 9
// This is for UNO Adapter, pin 10 is Vcc
RF24 radio(8, 9);

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.

typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_ping_out;                                              // The role of the current running sketch

// A single byte to keep track of the data being sent back and forth
byte counter = 1;
PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;
byte silnik = 0;
// const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 3; // Analog output pin that the LED is attached to
// value output to the PWM (analog out)

int sensorValue = vibrate;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

unsigned char tablica[21];

byte liczba_danych = 20;

void setup() {
  ps2x.config_gamepad(5, 3, 2, 4, true, true); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error


  // Simple codes for UNO nRF adapter that uses pin 10 as Vcc
  // pinMode(10,OUTPUT);
  // digitalWrite(10,HIGH);
  // delay(500);


  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r", role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // Setup and configure rf radio

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipes[1]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void) {
  {




    ps2x.read_gamepad(false, vibrate);  //read controller and set large motor to spin at 'vibrate' speed


    if (ps2x.Button(PSB_START))                  //will be TRUE as long as button is pressed
    { //Serial.println("Start is being held");
      tablica[0] = 255;
    } else {
      tablica[0] = 0;
    }

    if (ps2x.Button(PSB_SELECT))
    {
      //Serial.println("Select is being held");
      //val_1= (ps2x.Button(PSB_SELECT)
      tablica[1] = 255;
    } else {
      tablica[1] = 0;
    }

    /* if (ps2x.Button(PSB_PAD_UP)) {        //will be TRUE as long as button is pressed
       //Serial.print("Up held this hard: ");
       //Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);

      }
      if (ps2x.Button(PSB_PAD_RIGHT)) {

       //Serial.print("Right held this hard: ");
       //Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);

      }
      if (ps2x.Button(PSB_PAD_LEFT)) {

       //Serial.print("LEFT held this hard: ");
       //Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);

      //  tablica[2] = ps2x.Analog(PSAB_PAD_LEFT);

      //val_1 = ps2x.Button(PSB_PAD_UP);
      //msg[1] = val_1;
      //radio.write(msg,1);
      }else{

       //tablica[2]=128;
      }
      if (ps2x.Button(PSB_PAD_DOWN)) {

       //Serial.print("DOWN held this hard: ");
       //Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);

      }




      if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
      {



       if (ps2x.Button(PSB_L3))
         //Serial.println("L3 pressed");
       if (ps2x.Button(PSB_R3))
         //Serial.println("R3 pressed");
       if (ps2x.Button(PSB_L2))
         //Serial.println("L2 pressed");
       if (ps2x.Button(PSB_R2))
         //Serial.println("R2 pressed");
       if (ps2x.Button(PSB_GREEN))
         //Serial.println("Triangle pressed");

      }


      if (ps2x.ButtonPressed(PSB_RED))            //will be TRUE if button was JUST pressed
       Serial.println("Circle just pressed");

      if (ps2x.ButtonReleased(PSB_PINK))            //will be TRUE if button was JUST released
       Serial.println("Square just released");

      if (ps2x.NewButtonState(PSB_BLUE))           //will be TRUE if button was JUST pressed OR released
       Serial.println("X just changed");


      if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
      {
       Serial.print("Stick Values:");
       Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
       Serial.print(",");

       Serial.print(ps2x.Analog(PSS_LX), DEC);
       Serial.print(",");
       Serial.print(ps2x.Analog(PSS_RY), DEC);
      // Serial.print(",");
       Serial.println(ps2x.Analog(PSS_RX), DEC);


      }
    */
    tablica[2] = ps2x.Analog(PSS_LY);
    tablica[3] = ps2x.Analog(PSS_LX);
    tablica[4] = ps2x.Analog(PSS_RY);
    tablica[5] = ps2x.Analog(PSS_RX);

    tablica[6] = ps2x.Button(PSB_PAD_UP);
    tablica[7] = ps2x.Button(PSB_PAD_RIGHT);
    tablica[8] = ps2x.Button(PSB_PAD_DOWN);
    tablica[9] = ps2x.Button(PSB_PAD_LEFT);
    tablica[10] = ps2x.Button(PSB_L3);
    tablica[11] = ps2x.Button(PSB_L2);
    tablica[12] = ps2x.Button(PSB_L1);
    tablica[13] = ps2x.Button(PSB_R3);
    tablica[14] = ps2x.Button(PSB_R2);
    tablica[15] = ps2x.Button(PSB_R1);
    tablica[16] = ps2x.Button(PSB_GREEN);
    tablica[17] = ps2x.Button(PSB_RED);
    tablica[18] = ps2x.Button(PSB_BLUE);
    tablica[19] = ps2x.Button(PSB_PINK);


    if (role == role_ping_out) {

      radio.stopListening();                                  // First, stop listening so we can talk.

     printf("Nr : %d, Wyslano %d, %d  \n\r", counter , tablica[0], tablica[1]);
      if (!radio.write( &tablica, liczba_danych )) {
        printf("failed.\n\r");
      } else {
        byte gotByte;
        if (!radio.available()) {
          printf("Blank Payload Received\n\r");
        } else {
          // while(radio.available() ){
          //   radio.read( &gotByte, 1 );
          //   printf("Got response %d,\n\r",gotByte);
          counter++;
          // }
        }

      }

      delay(10);
    }




  }
}
