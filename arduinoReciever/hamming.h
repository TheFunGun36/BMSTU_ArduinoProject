// globalinfo.h

#ifndef _HAMMING_h
#define _HAMMING_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <math.h>
#define MESSAGE_LENGTH 64
#define CTRL_BITS_NUMBER 7
#define TRUE_LENGTH (MESSAGE_LENGTH + CTRL_BITS_NUMBER)
#define PACKAGE_SIZE 9

void getCtrlBits(char message[], bool receivedCtrlBits[], int ctrlBitsNumber);
void calculateCtrlBits(char message[], bool calculatedCtrlBits[], int ctrlBitsNumber, int trueLength);
void getTrueMessage(char message[], char trueMessage[], int messageLength, int trueLength);
void getHammingMessage(char message[], char trueMessage[], int ctrlBitsNumber, int messageLength);

#endif // HAMMING_H
