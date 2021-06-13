#include "globalinfo.h"
#include "hamming.h"
namespace global
{
    byte sendBuffer[maxBufferSize + 8];
    byte sendMessage[maxBufferSize];
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

    byte arduinoRecieveByte()
    {
        byte result = 0;

        for (byte i = byteSize - 1; i >= 0; i--)
        {
            bool r = arduinoRecieveBit();
            result |= r * (1 << i);
        }

        return result;
    }

    void sendPcInfo(byte length)
    {
        for (int i = 1; i < length + 1; i++)
            Serial.write(sendMessage[i]);
    }

    void receiveLength(byte *length)
    {
        byte encodedLength[] = { 0, 0 };
        encodedLength[0] = arduinoRecieveByte();
        encodedLength[1] = arduinoRecieveByte();
        getHammingMessage(encodedLength, length, 4, 8);
    }

    void receivePackage(byte onePackage[])
    {
        for (byte *ptr = onePackage; ptr < ptr + PACKAGE_SIZE; ptr++)
        {
            *ptr = arduinoRecieveByte();
        }
    }

    void arduinoRecieveLength(byte &lengthRecieved, byte &length, byte &trueLength)
    {
        receiveLength(&lengthRecieved);
        length = lengthRecieved == 0 ? (maxBufferSize - 1) : sendBuffer[0];
        trueLength = length;
        if ((length % 8) != 0)
            trueLength = ((length / 8) + 1) * 8;
        trueLength += trueLength / 8;
    }

    void arduinoRecieveInfo(byte trueLength)
    {
        byte onePackage[PACKAGE_SIZE];
        byte *ptr = sendMessage;
        for (byte i = 0; i < trueLength; i += PACKAGE_SIZE)
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