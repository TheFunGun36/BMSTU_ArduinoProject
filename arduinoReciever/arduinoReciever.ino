
#include "globalinfo.h"
constexpr uint8_t inputPin = 3;

bool buffer[1000];

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

    if (global::isLedActive(inputPin))
    {
        if (global::otherArduinoSync(inputPin))
        {
            digitalWrite(LED_BUILTIN, HIGH);
            char result = global::arduinoRecieveByte(inputPin);
            Serial.write(result);
        }

        digitalWrite(LED_BUILTIN, LOW);
    }
}
