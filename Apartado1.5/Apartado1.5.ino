#include <stdint.h>

// Definir pines para los LEDs de ambos semáforos
uint8_t red_led1 = 2;    // Semáforo 1 - LED rojo
uint8_t yellow_led1 = 4; // Semáforo 1 - LED amarillo
uint8_t green_led1 = 5;  // Semáforo 1 - LED verde
uint8_t red_led2 = 18;   // Semáforo 2 - LED rojo
uint8_t yellow_led2 = 19; // Semáforo 2 - LED amarillo
uint8_t green_led2 = 23;  // Semáforo 2 - LED verde

// Variables para temporización
uint64_t previous_millis = 0;
uint64_t state_interval = 3000; // Intervalo de cambio de estado (3s por fase)
uint8_t state = 0; // Estado del semáforo

// Funciones para controlar los estados
void semaforo1_rojo() {
    digitalWrite(red_led1, HIGH);
    digitalWrite(yellow_led1, LOW);
    digitalWrite(green_led1, LOW);
}

void semaforo1_amarillo() {
    digitalWrite(red_led1, LOW);
    digitalWrite(yellow_led1, HIGH);
    digitalWrite(green_led1, LOW);
}

void semaforo1_verde() {
    digitalWrite(red_led1, LOW);
    digitalWrite(yellow_led1, LOW);
    digitalWrite(green_led1, HIGH);
}

void semaforo2_rojo() {
    digitalWrite(red_led2, HIGH);
    digitalWrite(yellow_led2, LOW);
    digitalWrite(green_led2, LOW);
}

void semaforo2_amarillo() {
    digitalWrite(red_led2, LOW);
    digitalWrite(yellow_led2, HIGH);
    digitalWrite(green_led2, LOW);
}

void semaforo2_verde() {
    digitalWrite(red_led2, LOW);
    digitalWrite(yellow_led2, LOW);
    digitalWrite(green_led2, HIGH);
}

void setup() {
    Serial.begin(9600);
    pinMode(red_led1, OUTPUT);
    pinMode(yellow_led1, OUTPUT);
    pinMode(green_led1, OUTPUT);
    pinMode(red_led2, OUTPUT);
    pinMode(yellow_led2, OUTPUT);
    pinMode(green_led2, OUTPUT);

    // Inicializar LEDs apagados
    digitalWrite(red_led1, LOW);
    digitalWrite(yellow_led1, LOW);
    digitalWrite(green_led1, LOW);
    digitalWrite(red_led2, LOW);
    digitalWrite(yellow_led2, LOW);
    digitalWrite(green_led2, LOW);
}

void loop() {
    uint64_t actual_millis = millis(); // Obtener el tiempo actual

    // Cambio de estado cada 'state_interval' milisegundos
    if (actual_millis - previous_millis >= state_interval) {
        previous_millis = actual_millis;
        state = (state + 1) % 4; // Ciclo de 4 estados alternando ambos semáforos
    }

    // Control de los LEDs según el estado del semáforo
    if (state == 0) {
        semaforo1_rojo();
        semaforo2_verde();
    } else if (state == 1) {
        semaforo1_amarillo();
        semaforo2_rojo();
    } else if (state == 2) {
        semaforo1_verde();
        semaforo2_rojo();
    } else if (state == 3) {
        semaforo1_rojo();
        semaforo2_amarillo();
    }
}
