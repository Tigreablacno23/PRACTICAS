#include <stdint.h>

// Definir pines para los LEDs
uint8_t red_led = 2;   // Pin del LED rojo
uint8_t green_led = 4; // Pin del LED verde

void setup() {
  // Inicializamos el puerto serie para mostrar por pantalla
  Serial.begin(9600);

  // Configurar los pines como salida
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
}

void loop() {
  // Encender LED rojo y apagar LED verde
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, LOW);
  delay(1000); // Esperar 1 segundo

  // Encender LED verde y apagar LED rojo
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, HIGH);
  delay(1000); // Esperar 1 segundo
}