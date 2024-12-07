// Pines de conexión
const int stepPin = 9;           // Pin de STEP del controlador del motor
const int dirPin = 8;            // Pin de DIR del controlador del motor
const int enablePin = 10;        // Pin de ENABLE del controlador del motor (opcional)
const int sensorPin1 = A0;       // Pin analógico para el primer sensor de presión
const int sensorPin2 = A1;       // Pin analógico para el segundo sensor de presión

// Parámetros del motor
const int stepsToClose = 200;    // Pasos para cerrar el gripper
const int stepsToOpen = 200;     // Pasos para abrir el gripper
const int delayTime = 800;       // Control de velocidad del motor

// Umbrales y tiempos
const int pressureThreshold = 300;  // Umbral de presión para detectar el disco
const unsigned long detectionTime = 2000; // Tiempo mínimo para detectar presión (2 segundos)
const unsigned long holdTime = 10000;     // Tiempo para mantener el gripper cerrado (10 segundos)

void setup() {
  // Configuración de pines
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // Habilitar el driver del motor
  
  Serial.begin(9600);
  Serial.println("Sistema iniciado con dos sensores.");
}

void loop() {
  // Cerrar el gripper
  Serial.println("Cerrando el gripper...");
  cerrarGripper();

  unsigned long startTime = millis(); // Tiempo de inicio para la detección
  bool detected = false;

  // Monitorear la presión durante el cierre
  while (millis() - startTime < detectionTime) {
    int sensorValue1 = analogRead(sensorPin1);
    int sensorValue2 = analogRead(sensorPin2);
    Serial.print("Sensor 1: ");
    Serial.print(sensorValue1);
    Serial.print(" | Sensor 2: ");
    Serial.println(sensorValue2);

    if (sensorValue1 > pressureThreshold && sensorValue2 > pressureThreshold) {
      detected = true; // Presión detectada por ambos sensores
      Serial.println("Presión detectada en ambos sensores. Continuando...");
      break;
    }
  }

  if (detected) {
    // Mantener cerrado si ambos sensores detectaron presión
    Serial.println("Disco detectado. Manteniendo gripper cerrado.");
    delay(holdTime); // Mantener cerrado por 10 segundos
  } else {
    // Si no se detecta presión, abrir y volver a intentar
    Serial.println("No se detectó disco en ambos sensores. Abriendo gripper y reintentando.");
    abrirGripper();
  }

  delay(1000); // Esperar antes de volver a intentar
}

// Función para cerrar el gripper
void cerrarGripper() {
  digitalWrite(dirPin, HIGH); // Configura la dirección para cerrar
  for (int i = 0; i < stepsToClose; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
}

// Función para abrir el gripper
void abrirGripper() {
  digitalWrite(dirPin, LOW); // Configura la dirección para abrir
  for (int i = 0; i < stepsToOpen; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
}
