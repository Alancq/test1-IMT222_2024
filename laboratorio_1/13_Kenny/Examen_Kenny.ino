#include <EEPROM.h>

#define L1 7 //R
#define L2 8 //G
#define L3 9 //B

#define B1  2
#define B2  3

#define BAUD_RATE 9600

// TIME LIMIT PROCESS//
#define MIN 1
#define MAX 10
//  DEBOUNCING  //
#define DB  170
// BLINK TIME
#define BT  500
unsigned long last_time=0;
// 
uint8_t set_time;

void setup() { Serial.begin(BAUD_RATE);
  pinMode (L1,OUTPUT);
  pinMode (L2,OUTPUT);
  pinMode (L3,OUTPUT);
  pinMode (B1,INPUT);
  pinMode (B2,INPUT);
  attachInterrupt(digitalPinToInterrupt(B1), up  , FALLING);
  attachInterrupt(digitalPinToInterrupt(B2), down, FALLING);
  if (EEPROM.read(0)!=0){set_time=EEPROM.read(0);}
  else{set_time=MIN;}
  Serial.print("Tiempo de Curado: ");
  Serial.println(EEPROM.read(0));
}

void loop() {
  if (digitalRead(B1)==0 && digitalRead(B2)==0){EEPROM.write(0,set_time);
      digitalWrite(L1, 0);
      digitalWrite(L2, 1);
      Serial.print("INICIALIZANDO CURADO EN: ");
      Serial.println(EEPROM.read(0));
      while (set_time>MIN){
        digitalWrite(L3, 1);
        set_time--;
        Serial.println(set_time);
        delay(BT);
        digitalWrite(L3, 0);
        delay(BT);
      } Serial.println("CURADO FINALIZADO");
      Serial.print("Tiempo de Curado: ");
      Serial.println(EEPROM.read(0));
    }
    else{
          digitalWrite(L1, 1);
          digitalWrite(L2, 0);
          digitalWrite(L3, 0);
        }
}

void up(){
  if (millis()-last_time > DB){
    if (set_time<MAX && digitalRead(B2)==1){
      set_time++;
      Serial.println(set_time);
      
    }
    last_time = millis(); 
  }
}
void down(){
  if (millis()-last_time > DB){
    if (set_time>MIN && digitalRead(B1)==1){
      set_time--;
      Serial.println(set_time);
    }
    last_time = millis(); 
  }
}
