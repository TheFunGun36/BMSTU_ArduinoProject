#include "globalinfo.h"

void setup() {
    Serial.begin(9600);
    pinMode(global::outputPin, OUTPUT);
}

void loop() {
    if (Serial.available() > 0)
    {
        if (static_cast<char>(Serial.peek()) == '\xc0')
        {
            Serial.write('\xc0');
            Serial.read();
        }
        else
        {
            bool isLastTransmission = 0;

            do
            {
                int length;
                global::recievePcInfo(isLastTransmission, length);
                global::otherArduinoSync();
                global::arduinoSendInfo(length);

                if (!isLastTransmission)
                    Serial.write('\xcc');
            }
            while (!isLastTransmission);

            Serial.write('\xcd');
        }
    }
    digitalWrite(global::outputPin, HIGH);
    delay(500);
    digitalWrite(global::outputPin, LOW);
    delay(500);
}
