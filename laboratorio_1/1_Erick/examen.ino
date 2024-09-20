#include <EEPROM.h>  // Incluir la librería EEPROM

#define BTN1 2
#define BTN2 3
#define LED1 1
#define LED2 4
#define LED3 5

unsigned long BAUDRATE = 9600;

unsigned int MAXVAL = 10;
unsigned int MINVAL = 1;

int TiempoCurado=0;
int direccion = 0;
int PASO = 1;

volatile unsigned long lasttime = 0;
volatile unsigned long debounceDelay = 50;

void setup() {
  // put your setup code here, to run once:
Serial.begin(BAUDRATE);
TiempoCurado = EEPROM.read(0);

pinMode(BTN1, INPUT);
pinMode(BTN2, INPUT);
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);

attachInterrupt(digitalPinToInterrupt(BTN1), SUMAR, FALLING);
attachInterrupt(digitalPinToInterrupt(BTN2), RESTAR, FALLING);


}

void loop() {
  // put your main code here, to run repeatedly:
  Curar();
}

void SUMAR() {
  if (millis() - lasttime > debounceDelay) {
    if (TiempoCurado < MAXVAL) {
      TiempoCurado += PASO;
      lasttime = millis();
      EEPROM.write(direccion, TiempoCurado);
      Serial.println(TiempoCurado);
    }
  digitalWrite(LED1, HIGH);
  }
}

void RESTAR() {
  if (millis() - lasttime > debounceDelay) {
    if (TiempoCurado > MINVAL) {
      TiempoCurado -= PASO;
      lasttime = millis();
      EEPROM.write(direccion, TiempoCurado);
      Serial.println(TiempoCurado);
    }
  }
}

void CURAR(){
  if (millis() - lasttime > debounceDelay) {
    if (digitalRead(BTN1) == 0 && digitalRead(BTN2)== 0) {
      lasttime = millis();
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);   
          digitalWrite(LED3, HIGH); 
          delay(5000);   
              digitalWrite(LED3, LOW);
              delay(5000);     
      Serial.println(TiempoCurado);
    }
  }
  }
