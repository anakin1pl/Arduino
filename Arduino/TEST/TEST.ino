
#define Power     64
#define key_1     32
#define key_2     160
#define key_3     96
#define key_4     16
#define key_5     144
#define key_6     80
#define key_7     48
#define key_8     176
#define key_9     112
#define key_0     136
#define vol_up    0
#define vol_down  128
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
  pinMode(0,OUTPUT);
  digitalWrite(1,LOW);
  digitalWrite(0,HIGH);

  
}

void loop(void) {

  if(sended == 1)
{
  if(Data_back == 144) 
  {
     digitalWrite(0,LOW);
  }
  else if(Data_back == 192) 
  {
     digitalWrite(0,HIGH);
  }
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
