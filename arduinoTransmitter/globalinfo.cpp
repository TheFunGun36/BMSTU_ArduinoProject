#include "globalinfo.h"

namespace global
{
    char sendBuffer[maxBufferSize + 8]; // 8 - память под контрольные байты. 

    void arduinoSendByte(char byte)
    {
        for (int i = byteSize - 1; i >= 0; i--)
        {
            digitalWrite(outputPin, ((byte >> i) & 1));
            delay(bitLengthMilliseconds);
        }
    }

    void recievePcInfo(bool &isLastTransmission, int &length)
    {
        sendBuffer[0] = Serial.read();
        isLastTransmission = sendBuffer[0] != 0;
        length = sendBuffer[0] ? sendBuffer[0] : (maxBufferSize - 1);
        Serial.readBytes(sendBuffer + 1, length);
    }

    void arduinoSendInfo(int length)
    {
        for (int i = 0; i < length + 1; i++)
        {
            arduinoSendByte(sendBuffer[i]);
        }
        digitalWrite(outputPin, LOW);
    }

    void otherArduinoSync()
    {
        digitalWrite(outputPin, HIGH);
        delay(4000);
        digitalWrite(outputPin, LOW);
        delay(1000);
    }
}
