#include "globalinfo.h"

namespace global
{
    char sendBuffer[maxBufferSize + 8]; // 8 - память под контрольные байты. 

    void arduinoSendByte(int pin, char byte)
    {
        for (int i = byteSize - 1; i >= 0; i--)
        {
            digitalWrite(pin, ((byte >> i) & 1));
            delay(bitLengthMilliseconds);
        }
    }

    void recievePcInfo(bool &isLastTransmission)
    {
        sendBuffer[0] = Serial.read();
        isLastTransmission = sendBuffer[0] != 0;
        Serial.readBytes(sendBuffer + 1, (sendBuffer[0]) ? sendBuffer[0] : (maxBufferSize - 1));
    }

    void arduinoSendInfo(int pin)
    {
        Serial.println(static_cast<int>(sendBuffer[0]));
        for (int i = 0; i < sendBuffer[0] + 1; i++)
        {
            arduinoSendByte(pin, sendBuffer[i]);
            Serial.println(sendBuffer[i]);
        }
        digitalWrite(pin, LOW);
    }

    void otherArduinoSync(int pinLed)
    {
        digitalWrite(pinLed, HIGH);
        delay(4000);
        digitalWrite(pinLed, LOW);
        delay(1000);
    }

    void initializeTestPackage()
    {
        sendBuffer[0] = 13;
        sendBuffer[1] = 'H';
        sendBuffer[2] = 'e'; 
        sendBuffer[3] = 'l';
        sendBuffer[4] = 'l';
        sendBuffer[5] = 'o';
        sendBuffer[6] = ',';
        sendBuffer[7] = ' ';
        sendBuffer[8] = 'w';
        sendBuffer[9] = 'o';
        sendBuffer[10] = 'r';
        sendBuffer[11] = 'l';
        sendBuffer[12] = 'd';
        sendBuffer[13] = '!';
    }
}
