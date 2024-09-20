#include <EEPROM.h>

#define boton1      2  
#define boton2      3
#define TIME        1000
#define BAUDRATE    9600
#define MAXVOL      10 
#define MINVOL      0     
#define I_EEPROMP   0       
#define ledRojo     11
#define ledVerde    12
#define ledAzul     13

uint8_t volumen = EEPROM.read(I_EEPROMP);
bool flagSubir   = false;        
bool flagBajar = false;   
bool flagReset = false;             

unsigned long lasttime1 = 0;
unsigned long lasttime2 = 0;
const unsigned long debounceDelay = 60; 

void setup() {
    pinMode(BOTON1, INPUT_PULLUP); 
    pinMode(BOTON2, INPUT_PULLUP);
    
    attachInterrupt(digitalPinToInterrupt(boton1),Subir, FALLING);
    attachInterrupt(digitalPinToInterrupt(boton2),Bajar, FALLING);
    pinMode(ledRojo, OUTPUT);
    pinMode(ledVerde, OUTPUT);
    pinMode(ledAzul, OUTPUT);

    Serial.begin(BAUDRATE);
    Serial.print("Volumen inicio: ");
    Serial.println(volumen);
}

void loop() {
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= TIME) {
        Serial.print("Volumen actual: ");
        Serial.println(volumen);
        previousMillis = currentMillis;
    }
    if (flagSubir) {
        if (millis() - lasttime1 > debounceDelay) { 
            if (volumen < MAXVOL) {
                volumen++;
                EEPROM.write(I_EEPROMP, volumen); 
                Serial.print("VOLUMEN(+): ");
                Serial.println(volumen);
            }
            lasttime1 = millis();
        }
        flagSubir = false;
    }

    if (flagBajar) {
        if (millis() - lasttime2 > debounceDelay) { 
            if (volumen > MINVOL) {
                volumen--;
                EEPROM.write(I_EEPROMP, volumen); 
                Serial.print("VOLUMEN(-): ");
                Serial.println(volumen);
            }
            lasttime2 = millis();
        }
        flagBajar = false;
    }
    if (flagReset) {
        volumen = 0;
        EEPROM.write(I_EEPROMP, volumen); 
        Serial.println("Volumen en 0");
        flagReset = false; 
    }

    if (digitalRead(boton1) == LOW && digitalRead(boton2) == LOW) {
        ledVerde = true;
        ledAzul delay(10000)
        flagReset = true;
        delay(500);
    }
}
void Subir() {
    flagSubir = true; 
}

void Bajar() {
    flagBajar = true; 
}
