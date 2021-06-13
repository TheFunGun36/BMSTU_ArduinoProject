
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

            bool isLastTransaction;
            int length;

            global::arduinoRecieveLength(isLastTransaction, length);
            Serial.write(length * isLastTransaction);
            global::arduinoRecieveInfo(length);
            global::sendPcInfo(length);

            while (!isLastTransaction && global::otherArduinoSync())
            {
                global::arduinoRecieveLength(isLastTransaction, length);
                Serial.write(length * isLastTransaction);
                global::arduinoRecieveInfo(length);
                global::sendPcInfo(length);
            }

            if (!isLastTransaction)
            {
                Serial.write('\xce');
            }
        }

        digitalWrite(LED_BUILTIN, LOW);
    }
}
