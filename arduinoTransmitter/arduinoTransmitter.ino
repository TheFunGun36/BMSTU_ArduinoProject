#include "globalinfo.h"

constexpr int pinLed = 2;

void setup() {
    Serial.begin(9600);
    pinMode(pinLed, OUTPUT);
}

void loop() {
    if (Serial.available() > 0)
    {
        bool isLastTransmission = 0;

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
}
