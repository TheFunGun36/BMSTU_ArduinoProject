// globalinfo.h

#ifndef _GLOBALINFO_h
#define _GLOBALINFO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace global
{
    constexpr uint16_t threshold = 1000;
    constexpr int baudRate = 9600;
    constexpr int maxBufferSize = 64;
    constexpr int bitLengthMilliseconds = 100;
    constexpr int byteSize = 8;
    constexpr int hammingPackageSize = 64;

    inline bool isLedActive(int pin);
    char arduinoRecieveByte(int pin);
    void sendPcInfo(bool &isLastTransmission);
    void arduinoRecieveInfo(int pin);
    bool otherArduinoSync(int pin);
}

#endif

