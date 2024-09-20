#include <EEPROM.h>

#define PUL2 2
#define PUL1 3
#define LED1 13  
#define BAURENT 9600
#define DEBOUNCE_DELAY 50
#define MIN 0
#define MAX 100
unsigned long lastDebounceTime1 = 0;  
unsigned long lastDebounceTime2 = 0;  
int direccion = 0;
unsigned int CONT = 0;
unsigned int PASO = 1;

void setup() {
  Serial.begin(BAURENT);
  EEPROM.get(direccion, CONT); 
  if (CONT > MAX) {
    CONT = 0; 
  }

  Serial.print("Contador inicial desde EEPROM: ");
  Serial.println(CONT);
  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
  pinMode(LED1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PUL1), sumar, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUL2), restar, FALLING);
}

void loop() {
  delay(100);  
}

void sumar() {
  if (CONT >= MAX) {
    Serial.println("El contador ha alcanzado el valor máximo.");
  } else {
    unsigned long currentTime = millis();  
    if (currentTime - lastDebounceTime1 > DEBOUNCE_DELAY) {
      CONT += PASO;  
      EEPROM.put(direccion, CONT);  
      Serial.print("Pulsaciones (sumar): ");
      Serial.println(CONT);
      lastDebounceTime1 = currentTime;  
    }
  }
}

void restar() {
  if (CONT <= MIN) {
    Serial.println("El contador ya está en 0.");
  } else {
    unsigned long currentTime = millis();  
    if (currentTime - lastDebounceTime2 > DEBOUNCE_DELAY) {
      CONT -= PASO;  
      EEPROM.put(direccion, CONT);  
      Serial.print("Pulsaciones (restar): ");
      Serial.println(CONT);
      lastDebounceTime2 = currentTime;  
    }
  }
}
