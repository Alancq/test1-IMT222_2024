#include <EEPROM.h>
#define BTN1 15
#define BTN2 4
#define LED1 3
#define LED2 6
#define LED3 7

const int TMIN = 0;
const int TMAX = 10;
int tiempo = 0;

const int tiempoCurado=1;
const int tiempoGuardado=0;
const int tiempoEEPROM;
volatile bool banderaAumentar=false;
volatile bool banderaDisminuir=false;
volatile bool enCurado = false;
unsigned long lastInterruptTime = 0;
const unsigned long deboucedelay = 60;

void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  tiempoEEPROM = EEPROM.read(0);
  if(tiempoEEPROM>=1 &&tiempoEEPROM<=10){
    tiempoCurado=tiempEEPROM;
  }else{
    EEPROM.write(0,tiempoCurado);
  }
  Serial.print("Timepo de curado inicial: ");
  Serial.println(tiempoCuarado);
  digitalWrite(LED1,HIGH);
  }
  attachInterrupt(digitalPinToInterrupt(BTN1), botonAumentarPulsado, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2), botonDisminuirPulsado, FALLING);

  Serial.begin(115200);
}
void loop() {
  
  if (banderaAumentar && banderaDisminuir) {
    tiempo = tiempoCurado;  
    EEPROM.write(direccionEEPROM, tiempo);
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,HIGH);
;
    Serial.println("Se inicia el proceso de curado por UV");
    resetearBanderas();  
  }
  else if (banderaAumentar) {
   
    if (tiempo < TMAX) {
      tiempo++;
      EEPROM.write(direccionEEPROM, tiempo);
      EEPROM.commit();
      Serial.print("Tiempo: ");
      Serial.println(tiempo);
    }
    resetearBanderas();
  }
  else if (banderaDisminuir) {
   
    if (tiempo > TMIN) {
      tiempo--;
      EEPROM.write(direccionEEPROM, tiempo);
      EEPROM.commit();
      Serial.print("Tiempo: ");
      Serial.println(tiempo);
    }
    resetearBanderas();
  }

  delay(500); 
}

void botonAumentarPulsado() {
  unsigned long interruptTime = millis();
  
  if (interruptTime - lastInterruptTime > deboucedelay) {
    banderaAumentar = true; 
    lastInterruptTime = interruptTime;
  }
}

void botonDisminuirPulsado() {
  unsigned long interruptTime = millis();
  
  if (interruptTime - lastInterruptTime > deboucedelay) {
    banderaDisminuir = true; 
    lastInterruptTime = interruptTime;
  }
}

void resetearBanderas() {
  banderaAumentar = false;
  banderaDisminuir = false;
}