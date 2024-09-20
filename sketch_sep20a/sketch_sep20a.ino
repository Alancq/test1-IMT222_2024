#include <EEPROM.h>
#define LED1 13
#define LED2 12
#define LED3 11
#define PUL2 2
#define PUL1 3
#define BAURENT 9600
#define REINICIO 0
#define DEBOUNCE_DELAY 50  // Tiempo de retardo para evitar rebote
const int btn1 = 2;  // Incrementar tiempo
const int btn2 = 3;  // Decrementar tiempo
const int led1 = 13;  // LED 1: Configuración
const int led2 = 12;  // LED 2: Curado en Proceso
const int led3 = 11; // LED 3: Temporizador

// Variables para la configuración de tiempo
int CURADO = 5;     // Tiempo inicial por defecto
const int MIN = 1;   // Tiempo mínimo
const int MAX = 10;  // Tiempo máximo

// Variables para los pulsadores
bool btn1State = LOW;
bool btn2State = LOW;
bool lastBtn1State = LOW;
bool lastBtn2State = LOW;
volatile bool flag = false;

unsigned long lastDebounceTime = 0;

// Variables para temporización
unsigned long previousMillis = 0;
const long Tled = 1000; // Intervalo de parpadeo para LED3 (1 segundo)
int countdown = 0;          // Cuenta regresiva

void setup() {
  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Leer tiempo almacenado en EEPROM
  curadoTime = EEPROM.read(0);
  if (curadoTime < minTime || curadoTime > maxTime) {
    curadoTime = 5;  // Valor predeterminado si los datos en EEPROM no son válidos
  }

  // Iniciar comunicación serial
  Serial.begin(9600);
  Serial.print("Tiempo de curado inicial: ");
  Serial.println(curadoTime);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPU);

  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(PUL1), sumar, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUL2), restar, FALLING);  
  digitalWrite(led1Pin, HIGH);
  
}

void loop() {
  if (btn1State == HIGH && btn2State == HIGH) {
    iniciarCurado();
  }

  lastBtn1State = reading1;
  lastBtn2State = reading2;
}

void sumar() {
  if (CONT > MAX) {
    Serial.println("El contador ha alcanzado el valor máximo.");
  } else {
    unsigned long currentTime = millis();  // Obtener el tiempo actual
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
      flag = !flag;
      if (flag) {
        CURADO = CURADO + PASO;  // Incrementar el contador
        flag = false;
        // Guardar el nuevo valor de CONT en la EEPROM
        EEPROM.put(direccion, CONT);

        Serial.print("Pulsaciones (sumar): ");
        Serial.println(CONT);
      }
      lastDebounceTime = currentTime;  // Actualizar el tiempo del último cambio
    }
  }
}
void restar() {
  if (CURADO < MIN) {
    Serial.println("El contador ha alcanzado el valor minimo");
  } else {
    unsigned long currentTime = millis();  // Obtener el tiempo actual
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
      flag = !flag;
      if (flag) {
        CURADO = CURADO - PASO;  // Incrementar el contador
        flag = false;
        // Guardar el nuevo valor de CONT en la EEPROM
        EEPROM.put(direccion, CONT);

        Serial.print("Pulsaciones (sumar): ");
        Serial.println(CURADO);
      }
      lastDebounceTime = currentTime;  // Actualizar el tiempo del último cambio
    }
  }
}
void iniciarCurado() {
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, HIGH);
  countdown = curadoTime;

  while (countdown > 0) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= Tled) {
      previousMillis = currentMillis;

      // Parpadeo del LED 3 y cuenta regresiva
      digitalWrite(led3Pin, !digitalRead(led3Pin));  // Parpadeo
      countdown--;
      Serial.print("Tiempo restante: ");
      Serial.println(countdown);
    }
  }

  // Finalización del curado
  Serial.println("CURADO COMPLETADO");
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
  digitalWrite(led1Pin, HIGH);  // Volver al modo de configuración
}
