#include "globalinfo.h"

void setup() {
    Serial.begin(9600);
    pinMode(global::outputPin, OUTPUT);
}

void loop() {
    if (Serial.available() > 0)
    {
        if (Serial.peek() == '\xc0')
            Serial.write('\xc0');
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
}
