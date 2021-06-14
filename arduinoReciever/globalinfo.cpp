#include "globalinfo.h"
#include "hamming.h"
namespace global
{
    char sendBuffer[maxBufferSize + 8];
    char sendMessage[maxBufferSize];
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

        syncTime -= recievingBitIndex % 16 == 0;

        return flashes > (sum >> 1);  // HAHA, LOOK AT ME! IM C PROGRAMMER
    }

    char arduinoRecieveByte()
    {
        char result = 0;

        for (char i = byteSize - 1; i >= 0; i--)
        {
            bool r = arduinoRecieveBit();
            result |= r * (1 << i);
        }

        return result;
    }

    void sendPcInfo(char length)
    {
        for (int i = 1; i < length + 1; i++)
            Serial.write(sendMessage[i]);
    }

    void receiveLength(char *length)
    {
        char encodedLength[] = { 0, 0 };
        encodedLength[0] = arduinoRecieveByte();
        encodedLength[1] = arduinoRecieveByte();
        getHammingMessage(encodedLength, length, 4, 8);
    }

    void receivePackage(char onePackage[])
    {
        for (char *ptr = onePackage; ptr < ptr + PACKAGE_SIZE; ptr++)
        {
            *ptr = arduinoRecieveByte();
        }
    }

    void arduinoRecieveLength(char &lengthRecieved, char &length, char &trueLength)
    {
        receiveLength(&lengthRecieved);
        length = lengthRecieved == 0 ? (maxBufferSize - 1) : lengthRecieved;
        trueLength = length;
        if ((length % 7) != 0)
            trueLength = ((length / 7) + 1) * 7;
        trueLength += trueLength / 7;
    }

    void arduinoRecieveInfo(char trueLength)
    {
        char onePackage[PACKAGE_SIZE];
        char *ptr = sendMessage;
        for (char i = 0; i < trueLength; i += PACKAGE_SIZE)
        {
            receivePackage(onePackage);
            getHammingMessage(onePackage, ptr, 7, 64);
            ptr += PACKAGE_SIZE - 1;
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