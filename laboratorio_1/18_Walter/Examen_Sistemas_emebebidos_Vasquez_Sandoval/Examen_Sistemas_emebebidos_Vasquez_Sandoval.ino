#include <EEPROM.h>

#define boton1 2
#define boton2 3

#define led1 9
#define led2 10
#define ledTimer 11

int tiempoCurado = 1;          
int tiempoAlmacenado = 0;      
const int tiempoMinimo = 1;
const int tiempoMaximo = 10;

volatile bool curadoEnProgreso = false;  
unsigned long tiempoPrevio = 0;          
const long intervalo = 10;            
int tiempoRestante = 0;                  

void setup() {

  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(ledTimer, OUTPUT);
  

  Serial.begin(9600);


  attachInterrupt(digitalPinToInterrupt(boton1), boton1Presionado, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton2), boton2Presionado, FALLING);

  
  tiempoAlmacenado = EEPROM.read(0);
  if (tiempoAlmacenado >= tiempoMinimo && tiempoAlmacenado <= tiempoMaximo) {
    tiempoCurado = tiempoAlmacenado;
  } else {
    tiempoCurado = 1;  
  }

 
  digitalWrite(led1, HIGH);
  Serial.print("Tiempo de curado configurado: ");
  Serial.println(tiempoCurado);
}

void loop() {

  bool estadoBoton1 = digitalRead(boton1) == LOW;
  bool estadoBoton2 = digitalRead(boton2) == LOW;
  
  
  if (estadoBoton1 && estadoBoton2 && !curadoEnProgreso) {
    iniciarCurado();
  }


  if (estadoBoton1 && !estadoBoton2) {
    aumentarTiempo();
  } else if (!estadoBoton1 && estadoBoton2) {
    disminuirTiempo();
  }

  if (curadoEnProgreso) {
    actualizarCurado();
  }
}


void boton1Presionado() {
  // No hacemos nada en la interrupción para evitar retardo
}

void boton2Presionado() {
  // No hacemos nada en la interrupción para evitar retardo
}

void iniciarCurado() {
  curadoEnProgreso = true;
  tiempoRestante = tiempoCurado;  
  tiempoPrevio = millis();        


  digitalWrite(led1, LOW);   
  digitalWrite(led2, HIGH);  
  Serial.println("Curado iniciado...");
}

void aumentarTiempo() {
  if (tiempoCurado < tiempoMaximo) {
    tiempoCurado++;
    EEPROM.write(0, tiempoCurado);  
    Serial.print("Tiempo de curado configurado: ");
    Serial.println(tiempoCurado);
    delay(300);  
  }
}

void disminuirTiempo() {
  if (tiempoCurado > tiempoMinimo) {
    tiempoCurado--;
    EEPROM.write(0, tiempoCurado); 
    Serial.print("Tiempo de curado configurado: ");
    Serial.println(tiempoCurado);
    delay(300); 
  }
}

void actualizarCurado() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoPrevio >= intervalo) {
    tiempoPrevio = tiempoActual;

    digitalWrite(ledTimer, !digitalRead(ledTimer));

    tiempoRestante--;  
    Serial.print("Tiempo restante: ");
    Serial.println(tiempoRestante);

    if (tiempoRestante <= 0) {
      finalizarCurado();
    }
  }
}

void finalizarCurado() {
  curadoEnProgreso = false;

  digitalWrite(led2, LOW);
  digitalWrite(ledTimer, LOW);

  digitalWrite(led1, HIGH);

  tiempoCurado = EEPROM.read(0);
  Serial.println("Curado completado");
}