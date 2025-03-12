#include <stdint.h>

// Definir pines para los LEDs
uint8_t red_led = 2;   // Pin del LED rojo
uint8_t button = 23;   // Pin del botón

bool led_state = false;

void setup() {
    // Configurar los pines como salida
    Serial.begin(9600);
    pinMode(red_led, OUTPUT);  
    pinMode(button, INPUT_PULLUP); // Inicializamos el pin botón como entrada con pull-up
    // Pullup significa que 1 es en reposo y 0 es pulsando

    digitalWrite(red_led, LOW); // Inicializamos el LED rojo como apagado
}

void loop() {
    if (digitalRead(button) == LOW) { // Si el botón está a 0, es decir, presionado
        delay(200);  // Pequeño retraso para evitar rebotes
        led_state = !led_state; // Intercambiamos el estado del LED
        digitalWrite(red_led, led_state); // Encendemos o apagamos el LED según el estado
    }
}