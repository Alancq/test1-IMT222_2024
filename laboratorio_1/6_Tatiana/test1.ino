#include <EEPROM.h> 
#define BOTON1      2 
#define BOTON2      3 
#define TIME        5000
#define BAUDRATE    9600 
#define I_EEPROMP   0
#define LED1        8
#define LED2        11
#define LED3        13
#define MAXTIME     10
#define MINTIME     1

unint8__t tiempo=EEPROM.read(I_EEPROMP);
bool flag = false;
volatile unsigned long lasttime=0;
volatile unsigned long debounce=50;
volatile unsigned long lasttime2=0;

void setup(){
    if(digitalRead(BOTON1)){
        digitalWrite(LED1,55);
        digitalWrite(LED2,0);
        digitalWrite(LED3,0);

    } else{
        digitalWrite(LED1,0);
    }
    pinmode(BOTON1, INPUT_PULLUP);
    pinmode(BOTON2, INPUT_PULLUP);
    attatchInterrupt(digitalPinToInterrupt(BOTON1), up, FALLING);
    attatchInterrupt(digitalPinToInterrupt(BOTON2), down, FALLING);
}

void loop(){
    // Si ambos botones están presionados, empieza el tiempo de curado
    if (flag1==true && flag2==true) {
        tiempo = 0;
        EEPROM.write(I_EEPROMP, time);
        bool flag1 = false;
        bool flag2 = false;
    }

    delay(TIME);
}


void up(){
    if(millis()-lasttime>debounce){ 
        flag =true;
        if(time<MAXTIME){
            time++;
            serial.println (time);
            EEPROM.write(I_EEPROMP,value)

        }
        lasttime=millis();
    }
}

void down(){
   if(millis()-lasttime<debounce){
        flag =true;
        if(time>MINTIME){
            time--;
            serial.println (time);
        }
    lasttime=millis();
   }
}