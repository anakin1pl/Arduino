
#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
  {
   Serial.begin(9600);
   irrecv.enableIRIn(); // Start the receiver
   pinMode(13, OUTPUT);
  }

void loop() {
  
    if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);
    irrecv.resume();
    
    
    
    switch (results.value) {


      case 719194275:
      
  Serial.println("1");
  digitalWrite(13, HIGH);   
  delay(1000);              
  digitalWrite(13, LOW);  
  delay(1000); 
  break;
  
      case 2137078331:
      
  Serial.println("2");
  digitalWrite(13, HIGH);  
  delay(1000);              
  digitalWrite(13, LOW);   
  delay(1000);   
  break;
  
  
      case 440841023:
      
  Serial.println("3");
  digitalWrite(13, HIGH);  
  delay(1000);               
  digitalWrite(13, LOW);    
  delay(1000);   
  break;
 
      

      
                           }
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
  }
}
