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
        for (int i = 0; i < sendBuffer[0]; i++)
            arduinoSendByte(pin, sendBuffer[i]);
        digitalWrite(pin, LOW);
    }

    void otherArduinoSync(int pinLed)
    {
        digitalWrite(pinLed, HIGH);
        delay(4000);
        digitalWrite(pinLed, LOW);
        delay(1000);
    }
}
