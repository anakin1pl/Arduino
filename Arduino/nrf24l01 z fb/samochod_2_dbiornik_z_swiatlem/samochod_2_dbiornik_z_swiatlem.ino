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
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include <Servo.h>

Servo myservo;
// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins CE 8, CSN 9
// This is for UNO Adapter, pin 10 is Vcc
RF24 radio(8, 9);

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.

typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;                                              // The role of the current running sketch

// A single byte to keep track of the data being sent back and forth
byte counter = 1;

unsigned char tablica[21];
byte liczba_danych = 20;

//byte przycisk_gora = 3;
//byte przycisk_dol = 1;
int difv;
int jazda;
int jazd;
void setup() {


  // pinMode(3,OUTPUT);
  myservo.attach(3);

  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);




  role = role_pong_back;

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
  radio.setPayloadSize(liczba_danych);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipes[1]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void) {

  // Pong back role.  Receive each packet, dump it out, and send it back

  if ( role == role_pong_back ) {
    byte pipeNo;
    byte gotByte;                                       // Dump the payloads until we've gotten everything
    while ( radio.available(&pipeNo)) {

      radio.read( &tablica[0], liczba_danych );

      printf("Odebrano %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d  \n\r", tablica[0], tablica[1], tablica[2], tablica[3], tablica[4], tablica[5], tablica[6], tablica[7], tablica[8], tablica[9], tablica[10], tablica[11], tablica[12], tablica[13], tablica[14], tablica[15], tablica[16], tablica[17], tablica[18], tablica[19]);

      radio.writeAckPayload(pipeNo, &tablica[0], liczba_danych );
      //analogWrite(3, LOW);
      //np przycisk w gore

      // myservo.write(90);


      difv = tablica[5];
      difv = map(difv, 0, 255, 0, 180);
      myservo.write(difv);
      if (tablica[5] == 128 )
      {
        myservo.write(90);
      }
      /* else
        {if (tablica[4] > 127){

         poz0 = 90;

        }*/
      if (tablica[2] == 128)
      {
        // jazda = tablica[2];
        // jazd = map(jazda, 0, 128, 0, 255);
        analogWrite(5, 0);
        // analogWrite(5, 255);
      }
      if (tablica[2] < 128)
      {
        //jazda = tablica[2];
        jazd = map(tablica[2], 127, 0, 0, 255);
        analogWrite(5, jazd);
        digitalWrite(4, HIGH);

        // analogWrite(5, 255);
      }
      else digitalWrite(4, LOW);
      if (tablica[2] > 128)
      {
        //jazda = tablica[2];
        jazd = map(tablica[2], 129, 255, 0, 255);
        analogWrite(5, jazd);
        digitalWrite(7, HIGH);
        //  digitalWrite(5, HIGH);

      }
      else digitalWrite(7, LOW);


      if (tablica[0] == 255 )
      {
        digitalWrite(6, HIGH);
      }
      if (tablica[1] == 255 )
      {
        digitalWrite(6, LOW);
      }

      if (tablica[15] == 1 )
      {
        digitalWrite(A1, HIGH);
      }
      if (tablica[14] == 1 )
      {
        digitalWrite(A1, LOW);
      }
       if (tablica[12] == 1 )
      {
        digitalWrite(A0, HIGH);
      }
      if (tablica[11] == 1 )
      {
        digitalWrite(A0, LOW);
      }
        if (tablica[19] == 1 )
      {
        digitalWrite(A2, HIGH);
      }
      if (tablica[17] == 1 )
      {
        digitalWrite(A2, LOW);
      }
       if (tablica[16] == 1 )
      {
        digitalWrite(A3, HIGH);
      }
      if (tablica[18] == 1 )
      {
        digitalWrite(A3, LOW);
      }
      

    }



  }
}

