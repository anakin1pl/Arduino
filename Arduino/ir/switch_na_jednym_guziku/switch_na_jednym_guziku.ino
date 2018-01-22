
#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
  {
   Serial.begin(9600);
   irrecv.enableIRIn(); // Start the receiver
   pinMode(4, OUTPUT);
  }

void loop() {
  
    if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);
    irrecv.resume();
    
    
    
    switch (results.value) {


      case 719194275:
      
  Serial.println("1");
  if(digitalRead(4)==HIGH)
  {       
  digitalWrite(4, LOW);
  }
  else 
  {
  (digitalWrite(4, HIGH));
  }
  
 
  break;
  
      case 2137078331:
      
  Serial.println("1");
  if(digitalRead(3)==HIGH)
  {       
  digitalWrite(3, LOW);
  }
  else 
  {
  (digitalWrite(3, HIGH));
  }
  
  
      case 440841023:
      
 Serial.println("1");
  if(digitalRead(5)==HIGH)
  {       
  digitalWrite(5, LOW);
  }
  else 
  {
  (digitalWrite(5, HIGH));
  };
 
      

      
                           }
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
  }
}
