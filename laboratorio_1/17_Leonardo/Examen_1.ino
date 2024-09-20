#include          <EEPROM.h>
#define           BR            9600
#define           BTN1          2
#define           BTN2          3
#define           L1            7     //Defino el led 1
#define           L2            8     //Defino el led 2
#define           L3            9     //Defino el led 3
#define           MAX           10
#define           MIN           1
#define           TIME          500          
const int         b;
uint8_t           cont = 1;
unsigned long     tiempoActual = 0;
unsigned long     tiempoAnterior = 0;
const long        intervalo = 150; 

void setup() {Serial.begin(BR);
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  pinMode(BTN1,INPUT);
  pinMode(BTN2,INPUT);
  attachInterrupt(digitalPinToInterrupt(BTN1), sumar, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2), restar, FALLING);
  Serial.println("EL valor del tiempo es:");
  Serial.println(EEPROM.read(0));
  digitalWrite(L1,1);
  digitalWrite(L2,0);
  digitalWrite(L3,0);
}

void loop() {
  if(digitalRead(BTN1) == 1 && digitalRead(BTN2) ==1){
    digitalWrite(L1,0);
    digitalWrite(L2,1);
    for(int i = 0; i<= cont; i++){
      digitalWrite(L3,1);
      delay(TIME);
      digitalWrite(L3,0);
      delay(TIME);
      Serial.println("Se comienza a descontar: ");
      cont--;
      Serial.println(cont);
    }
    digitalWrite(L1,1);
    digitalWrite(L2,0);
  }
  

}

void sumar(){tiempoActual = millis();
  if(tiempoActual-tiempoAnterior > intervalo){
      if(cont < MAX){
        cont++;
      }
    Serial.println(cont);
    tiempoAnterior = millis();
  }
  EEPROM.write(cont,0);
}

void restar(){tiempoActual = millis();
  if(tiempoActual-tiempoAnterior > intervalo){
      if(cont > MIN){
        cont--;
      }  
    Serial.println(cont);
    tiempoAnterior = millis();
  }
  EEPROM.write(cont,0);
}