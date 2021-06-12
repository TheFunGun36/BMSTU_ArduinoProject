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
    constexpr uint8_t inputPin = 3;

    inline bool isLedActive();
    char arduinoRecieveByte();
    void sendPcInfo();
    void arduinoRecieveInfo();
    bool otherArduinoSync();
}

#endif

