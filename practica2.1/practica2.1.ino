#include <stdint.h>
#define pinLed1 18   // Led RGB - RED
#define pinLed2 19    // Led RGB - GREEN
#define pinLed3 23   // Led RGB - BLUE
#define potenciometro 4  
const int pinBuzzer = 5;  // Pin del buzzer

int portValue = 0;
int frequency = 0; // Variable para la frecuencia del buzzer

void setup() {
    pinMode(pinLed1, OUTPUT);
    pinMode(pinLed2, OUTPUT);
    pinMode(pinLed3, OUTPUT);
    pinMode(pinBuzzer, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    // Leer el valor del potenciómetro
    portValue = analogRead(potenciometro);
    Serial.println(portValue);

    // Control de LEDs según el valor del potenciómetro
    digitalWrite(pinLed1, portValue >= 1365 ? HIGH : LOW);
    digitalWrite(pinLed2, portValue >= 2730 ? HIGH : LOW);
    digitalWrite(pinLed3, portValue >= 4095 ? HIGH : LOW);

    // Si el potenciómetro está en 0, apagar el buzzer
    if (portValue == 0) {
        noTone(pinBuzzer);
    } else {
        // Convertir el valor del potenciómetro en una frecuencia audible (200Hz - 2000Hz)
        frequency = map(portValue, 0, 4095, 200, 2000); 
        tone(pinBuzzer, frequency);
    }

    delay(100); // Pequeña pausa para suavizar cambios
}
