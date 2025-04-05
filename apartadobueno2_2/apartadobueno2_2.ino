#include <stdint.h>
#define pinLedR 18  
#define pinLedG 19  
#define pinLedB 23  
#define fotoresistor 4  

int luzValue = 0;
const int umbral = 2000;  

void setup() {
    Serial.begin(115200);
    pinMode(pinLedR, OUTPUT);
    pinMode(pinLedG, OUTPUT);
    pinMode(pinLedB, OUTPUT);
}

void loop() {
    luzValue = analogRead(fotoresistor);
    Serial.println(luzValue);

    if (luzValue >= umbral) {  
        digitalWrite(pinLedR, 255);  
        digitalWrite(pinLedG, 255);  
        digitalWrite(pinLedB, 255);  
    } else {  
        digitalWrite(pinLedR, 0);
        digitalWrite(pinLedG, 0);
        digitalWrite(pinLedB, 0);
    }
    delay(500);
}
