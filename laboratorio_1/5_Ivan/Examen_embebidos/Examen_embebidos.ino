
#include <EEPROM.h>
#define PUL2 2
#define PUL1 3
#define LED1 8
#define LED2 9
#define LED3 10
#define BAURENT 9600
#define REINICIO 0
#define DEBOUNCE_DELAY 50 
#define MIN 1
#define MAX 10
           // Tiempo de retardo para evitar rebote
unsigned long lastDebounceTime = 0;  // Tiempo en que se detectó el último cambio
int direccion = 0;
unsigned int CONT = 0;               // Inicializamos CONT, pero lo cargaremos desde la EEPROM
unsigned int TIME = 0;
unsigned int PASO = 1;
volatile bool flag = false;


unsigned long previousMillis = 0;  // Tiempo anterior 
unsigned long elapsedTime = 0;  // Tiempo total transcurrido en milisegundos


void setup() {
  Serial.begin(BAURENT);
  
  // Leer el valor almacenado en la EEPROM al iniciar
  EEPROM.get(direccion, CONT);
  
  // Si el valor leído no está en el rango esperado (por ejemplo, valores iniciales incorrectos), reiniciar CONT
  if (CONT > MAX) {
    CONT = 0; // Resetear si el valor es inválido
  }

  Serial.print("Contador inicial desde EEPROM: ");
  Serial.println(CONT);

  pinMode(PUL1, INPUT_PULLUP);
  pinMode(PUL2, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  // Configurar interrupciones para sumar y restar
  attachInterrupt(digitalPinToInterrupt(PUL1), sumar, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUL2), restar, FALLING);
}

void loop() {
    if (digitalRead(PUL1)==1 && digitalRead(PUL2)==1) {
    unsigned long currentMillis = millis();
  
    if (currentMillis - previousMillis >= 1000) {  // Actualizar cada segundo
      elapsedTime += 1000;  // Sumar 1 segundo al tiempo transcurrido
      previousMillis = currentMillis;
      flag = false;

      displayTime();
    }
  }
  delay(100);  // Pequeño retardo para no saturar el loop principal
  
}

void sumar() {
  if (CONT >= MAX) {
    Serial.println("El contador ha alcanzado el valor máximo.");
  } else {
    unsigned long currentTime = millis();  // Obtener el tiempo actual

    // Verificar si ha pasado el tiempo suficiente desde la última pulsación (antirrebote)
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
      flag = !flag;
      if (flag) {
        CONT = CONT + PASO;  // Incrementar el contador
        digitalWrite(LED1, HIGH);
        flag = false;

        
        // Guardar el nuevo valor de CONT en la EEPROM
        EEPROM.put(direccion, CONT);
        
        // Mostrar el contador en el monitor serial
        Serial.print("Pulsaciones (sumar): ");
        Serial.println(CONT);
      }
      lastDebounceTime = currentTime;  // Actualizar el tiempo del último cambio
    }
  }
}

void restar() {
  if (CONT <= MIN) {
    Serial.println("El contador ya está en 0.");
  } else {
    unsigned long currentTime = millis();  // Obtener el tiempo actual

    // Verificar si ha pasado el tiempo suficiente desde la última pulsación (antirrebote)
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
      flag = !flag;
      if (flag) {
        CONT = CONT - PASO;  // Decrementar el contador
        digitalWrite(LED1, HIGH);
        flag = false;
        
        
        // Guardar el nuevo valor de CONT en la EEPROM
        EEPROM.put(direccion, CONT);
        
        // Mostrar el contador en el monitor serial
        Serial.print("Pulsaciones (restar): ");
        Serial.println(CONT);
      }
      lastDebounceTime = currentTime;  // Actualizar el tiempo del último cambio
    }
  }
}
void displayTime() {
  unsigned int seconds = CONT;
  unsigned int minutes = 0;
  

  // Mostrar el tiempo en formato MM:SS
  Serial.print(minutes < 10 ? "0" : "");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds < 10 ? "0" : "");
  Serial.println(seconds);
}
