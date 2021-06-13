
#include "globalinfo.h"

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    if (global::isLedActive())
    {
        if (global::otherArduinoSync())
        {
            Serial.write('\xcb');
            digitalWrite(LED_BUILTIN, HIGH);

            bool isLastTransmission;
            int length;

            global::arduinoRecieveInfo(isLastTransmission, length);
            global::sendPcInfo(length);

            while (!isLastTransmission && global::otherArduinoSync())
            {
                global::arduinoRecieveInfo(isLastTransmission, length);
                global::sendPcInfo(length);
            }

            if (!isLastTransmission)
            {
                Serial.write('\xce');
            }
        }

        digitalWrite(LED_BUILTIN, LOW);
    }
}
