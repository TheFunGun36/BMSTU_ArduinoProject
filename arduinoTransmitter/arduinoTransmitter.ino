#include "globalinfo.h"

constexpr int pinLed = LED_BUILTIN;

void setup() {
    Serial.begin(9600);
    pinMode(pinLed, OUTPUT);
}

void loop() {
    if (Serial.available() > 0)
    {
        bool isLastTransmission;

        do
        {
            global::recievePcInfo(isLastTransmission);
            global::otherArduinoSync(pinLed);
            global::arduinoSendInfo(pinLed);

            if (!isLastTransmission)
                Serial.write('\xcc');
        }
        while (!isLastTransmission);
    }
    delay(1000);
}
