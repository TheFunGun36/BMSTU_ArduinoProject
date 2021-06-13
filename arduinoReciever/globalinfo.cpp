#include "globalinfo.h"

namespace global
{
    char sendBuffer[maxBufferSize + 8];
    unsigned long syncTime = 0;
    unsigned int recievingBitIndex = 1;

    inline bool isLedActive()
    {
        return analogRead(inputPin) < threshold;
    }

    bool arduinoRecieveBit()
    {
        unsigned long timestamp = millis();
        unsigned int sum = 0;
        unsigned int flashes = 0;

        while (millis() < syncTime + recievingBitIndex * bitLengthMilliseconds)
        {
            sum++;
            flashes += isLedActive();
        }

        recievingBitIndex++;

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

    void sendPcInfo(int length)
    {
        for (int i = 0; i < length + 1; i++)
            Serial.write(sendBuffer[i]);
    }

    void arduinoRecieveInfo(bool &isLastTransmission, int &length)
    {
        sendBuffer[0] = arduinoRecieveByte();
        isLastTransmission = sendBuffer[0] != 0;
        length = isLastTransmission ? sendBuffer[0] : (maxBufferSize - 1);

        for (char *ptr = sendBuffer + 1; ptr < sendBuffer + length + 1; ptr++)
        {
            *ptr = arduinoRecieveByte();
        }
    }

    bool otherArduinoSync()
    {
        recievingBitIndex = 1;
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
        syncTime = millis();
        return true;
    }
}