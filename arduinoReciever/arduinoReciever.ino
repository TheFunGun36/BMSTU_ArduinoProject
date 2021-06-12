
#include "globalinfo.h"

bool buffer[1000];

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
            digitalWrite(LED_BUILTIN, HIGH);
            global::arduinoRecieveInfo();
            global::sendPcInfo();
        }

        digitalWrite(LED_BUILTIN, LOW);
    }
}
