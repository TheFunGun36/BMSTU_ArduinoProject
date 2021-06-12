#include "globalinfo.h"

namespace global
{
    inline bool isLedActive(int pin)
    {
        return analogRead(pin) < threshold;
    }

    inline bool arduinoRecieveBit(int pin)
    {
        unsigned int timestamp = millis();
        unsigned int sum = 0;
        unsigned int flashes = 0;

        while (millis() - timestamp < byteSize)
        {
            sum++;
            flashes += isLedActive(pin);
        }

        return flashes > (sum >> 1);  // HAHA, LOOK AT ME! IM C PROGRAMMER
    }

    char arduinoRecieveByte(int pin)
    {
        char result = static_cast<char>(0);

        for (int i = byteSize - 1; i >= 0; i--)
        {
            result |= arduinoRecieveBit(pin) * (1 << i);
        }

        return result;
    }

    void sendPcInfo(bool &isLastTransmission)
    {

    }

    void arduinoRecieveInfo(int pin)
    {

    }

    bool otherArduinoSync(int pin)
    {
        unsigned long timeStamp = millis();

        while (millis() - timeStamp < 3000)
        {
            bool ledActive = isLedActive(pin);

            if (!ledActive)
                return false;
        }

        while (isLedActive(pin));
        delay(1000);
        return true;
    }
}