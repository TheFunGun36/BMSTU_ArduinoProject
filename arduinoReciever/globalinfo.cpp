#include "globalinfo.h"

namespace global
{
    char sendBuffer[maxBufferSize + 8];

    inline bool isLedActive()
    {
        return analogRead(inputPin) < threshold;
    }

    bool arduinoRecieveBit()
    {
        unsigned long timestamp = millis();
        unsigned int sum = 0;
        unsigned int flashes = 0;

        while (millis() - timestamp < bitLengthMilliseconds)
        {
            sum++;
            flashes += isLedActive();
        }

        return flashes > (sum >> 1);  // HAHA, LOOK AT ME! IM C PROGRAMMER
    }

    char arduinoRecieveByte()
    {
        char result = static_cast<char>(0);

        for (int i = byteSize - 1; i >= 0; i--)
        {
            bool r = arduinoRecieveBit();
            result |= r * (1 << i);
        }

        return result;
    }

    void sendPcInfo()
    {
        Serial.print(sendBuffer);
        Serial.println("");
    }

    void arduinoRecieveInfo()
    {
        sendBuffer[0] = arduinoRecieveByte();
        int length = sendBuffer[0] == 0 ? maxBufferSize : sendBuffer[0];

        for (char *ptr = sendBuffer + 1; ptr < sendBuffer + length + 1; ptr++)
        {
            *ptr = arduinoRecieveByte();
        }
    }

    bool otherArduinoSync()
    {
        unsigned long timeStamp = millis();

        digitalWrite(LED_BUILTIN, HIGH);

        while (millis() - timeStamp < 3800)
        {
            bool ledActive = isLedActive();

            if (!ledActive)
                return false;
        }

        digitalWrite(LED_BUILTIN, LOW);
        while (isLedActive());

        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        return true;
    }
}