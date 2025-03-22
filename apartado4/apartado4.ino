#include <stdint.h>

const int ledPin = 13;  // Pin del LED
const int p1 = 2;       // Pin del pulsador P1
const int p2 = 3;       // Pin del pulsador P2

bool ledEncendido = false;  
unsigned long tiempoInicio = 0;  
const unsigned long tiempoEspera = 10000; // 10 segundos

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(p1, INPUT_PULLUP);
    pinMode(p2, INPUT_PULLUP);
}

void loop() {
    if (digitalRead(p1) == LOW) {  // Si se presiona P1
        digitalWrite(ledPin, HIGH);
        ledEncendido = true;
        tiempoInicio = millis();  // Reinicia el temporizador
        delay(200); // Pequeña pausa para evitar rebotes
    }

    if (digitalRead(p2) == LOW) {  // Si se presiona P2, apaga el LED
        digitalWrite(ledPin, LOW);
        ledEncendido = false;
        delay(200); // Pequeña pausa para evitar rebotes
    }

    if (ledEncendido && (millis() - tiempoInicio >= tiempoEspera)) {
        digitalWrite(ledPin, LOW); // Apaga el LED tras 10s
        ledEncendido = false;
    }
}