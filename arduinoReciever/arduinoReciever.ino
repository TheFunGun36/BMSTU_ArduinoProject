
#include "globalinfo.h"

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    if (Serial.available() > 0)
    {
        byte ping = Serial.read();
        
        if (ping == '\xc0')
            Serial.write('\xc0');
    }
    if (global::isLedActive())
    {
        if (global::otherArduinoSync())
        {
            Serial.write('\xcb');
            digitalWrite(LED_BUILTIN, HIGH);

            byte recievedLength, messageLength, encodedMessageLength;

            global::arduinoRecieveLength(recievedLength, messageLength, encodedMessageLength);
            Serial.write(recievedLength);
            global::arduinoRecieveInfo(encodedMessageLength);
            global::sendPcInfo(messageLength);

            while (recievedLength == 0)
            {
                global::arduinoRecieveLength(recievedLength, messageLength, encodedMessageLength);
                Serial.write(recievedLength);
                global::arduinoRecieveInfo(encodedMessageLength);
                global::sendPcInfo(messageLength);
            }

            if (recievedLength == 0)
            {
                Serial.write('\xce');
            }
        }

        digitalWrite(LED_BUILTIN, LOW);
    }
}
