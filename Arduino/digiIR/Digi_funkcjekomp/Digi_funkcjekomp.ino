/*************************************
* This code is written by Laurens Wuyts
* For questions: laurens.wuyts@gmail.com
* 
* 
* Microcontroller: ATtiny85
* Dev board:       Digispark
* 
*************************************/

/**** Define Remote control keys ****/
#define Power     64
#define key_1     32
#define key_2     160
#define key_3     96
#define key_4     16
#define key_5     14
#define key_6     80
#define key_7     48
#define key_8     176
#define key_9     112
#define key_0     136
#define vol_up    192 
#define vol_down  144
#define ch_up     72
#define ch_down   8
#define mute      240
#define next      172
#define prev      164
#define up        6
#define down      134
#define left      166
#define right     70
#define playpause 156
#define key_stop  180
/************************************/

#include "TrinketHidCombo.h"

uint8_t situation = 0;
uint8_t START = 0;
uint8_t x = 0;
uint8_t BIT = 0;
uint8_t Id = 0;
uint8_t Id_inv = 0;
uint8_t Data = 0;
uint8_t Data_back = 0;
uint8_t Data_inv = 0;
uint8_t Repeat = 0;
uint8_t sended = 0;

uint16_t Time_old = 0;
uint16_t Time = 0;
uint16_t TimeDelta = 0;
 
void setup(void) {
  /* Use INT0(P2) on the Digispark */
  attachInterrupt(0, IR_Read, FALLING);
  
  pinMode(1,OUTPUT);
  digitalWrite(1,LOW);

  TrinketHidCombo.begin();
}
 
void loop(void) {
  if(sended == 1) {
    /* Assign functions to the buttons */
    if(Data_back == vol_up) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
    } else if(Data_back == vol_down) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
    } else if(Data_back == next) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_NEXT_TRACK);
    } else if(Data_back == prev) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_PREV_TRACK);
    } else if(Data_back == key_stop) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_STOP);
    } else if(Data_back == playpause) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE);
    } else if(Data_back == mute) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_MUTE);
    } else if(Data_back == Power) {
      TrinketHidCombo.pressSystemCtrlKey(SYSCTRLKEY_SLEEP);
    } else if(Data_back == key_0) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_0);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_1) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_1);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_2) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_2);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_3) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_3);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_4) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_4);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_5) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_5);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_6) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_6);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_7) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_7);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_8) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_8);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == key_9) {
      TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_9);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == up) {
      TrinketHidCombo.pressKey(0, KEYCODE_ARROW_UP);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == down) {
      TrinketHidCombo.pressKey(0, KEYCODE_ARROW_DOWN);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == left) {
      TrinketHidCombo.pressKey(0, KEYCODE_ARROW_LEFT);
      TrinketHidCombo.pressKey(0, 0);
    } else if(Data_back == right) {
      TrinketHidCombo.pressKey(0, KEYCODE_ARROW_RIGHT);
      TrinketHidCombo.pressKey(0, 0);
    }
    sended = 0;
  } else {
      TrinketHidCombo.poll();
  }
}

/* Read the IR code */
void IR_Read(void) {
  digitalWrite(1,HIGH);
  Time = micros();
  if (Time_old != 0) {
    TimeDelta = Time - Time_old;
    if ((TimeDelta > 12000)&&(TimeDelta < 14000)) {
      START = 1;
      x = 0;
      situation = 1;
      Id = 0;
      Id_inv = 0;
      Data = 0;
      Data_inv = 0;
    } else if ((TimeDelta > 10000)&&(TimeDelta < 12000)) {
      situation = 2; // repeat
    } else if ((TimeDelta > 1500)&&(TimeDelta < 2500)) {
      situation = 3; //"1"
      BIT = 1;
    } else if ((TimeDelta > 1000)&&(TimeDelta < 1500)) {
      situation = 3; //"0"
      BIT = 0;
    } else situation = 5; 
    if (situation == 3) {
      if (x < 8) {
        Id |= BIT;
        if (x < 7) Id <<= 1;  
        x++;
      } else if (x < 16) {
        Id_inv |= BIT;
        if (x < 15) Id_inv <<= 1;
        x++;
      } else if (x < 24) {
        Data |= BIT;
        if (x < 23) Data <<= 1;
        x++;
      } else if (x < 32) {
        Data_inv |= BIT;
        if (x < 31) {
          Data_inv <<= 1;
        } else {
          
          /* DO SOMETHING HERE */
          sended = 1;
          
          Data_back = Data;
          Repeat = 0;
        }
        x++;
      }
    } else if (situation == 2) {
      if(Repeat == 1) {
        
        /* DO SOMETHING HERE */
        sended = 1;
        
      } else {
        Repeat = 1;
      }
    }
  }
  Time_old = Time;
  digitalWrite(1,LOW);
}
