#ifndef _GLOBALINFO_h
#define _GLOBALINFO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace global
{
    constexpr int baudRate = 9600;
    constexpr int maxBufferSize = 57;
    constexpr int bitLengthMilliseconds = 50;
    constexpr int byteSize = 8;
    constexpr int hammingPackageSize = 64;
    constexpr int outputPin = 2;


    void codeInfo();
    void arduinoSendByte(char byte);
    void recievePcInfo(bool &isLastTransmission, int &length);
    void arduinoSendInfo(int length);
    void otherArduinoSync();
}

#endif
