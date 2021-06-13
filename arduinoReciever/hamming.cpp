#include <math.h>
#include "hamming.h"

#define MESSAGE_LENGTH 64
#define CTRL_BITS_NUMBER 7
#define TRUE_LENGTH (MESSAGE_LENGTH + CTRL_BITS_NUMBER)


void getCtrlBits(char message[], bool receivedCtrlBits[], int ctrlBitsNumber);
void calculateCtrlBits(char message[], bool calculatedCtrlBits[], int ctrlBitsNumber, int trueLength);
void getTrueMessage(char message[], char trueMessage[], int messageLength, int trueLength);

void getHammingMessage(char message[], char trueMessage[], int ctrlBitsNumber, int messageLength)
{
    bool receivedCtrlBits[CTRL_BITS_NUMBER] = { 0 };
    bool calculatedCtrlBits[CTRL_BITS_NUMBER] = { 0 };
    getCtrlBits(message, receivedCtrlBits);
    calculateCtrlBits(message, calculatedCtrlBits);
    bool isRight = true;
    int i, indexOfWrongBit = - 1;
    for (i = 0; i < ctrlBitsNumber; i++)
        if (receivedCtrlBits[i] != calculatedCtrlBits[i])
        {
            isRight = false;
            indexOfWrongBit += (int) pow(2, i);
        }
    if (!isRight)
        message[indexOfWrongBit / 8] ^= (1 << (indexOfWrongBit % 8));
    getTrueMessage(message, trueMessage);
}

void getCtrlBits(char message[], bool receivedCtrlBits[], int ctrlBitsNumber)
{
    int i = 0, ctrlBitNumber = 0, index = 0;
    while (i < ctrlBitsNumber)
    {
        while (pow(2, ctrlBitNumber) - 8 * index - 1 < 8)
        {
            int p = ((int) pow(2, ctrlBitNumber) - 8 * index - 1);
            p = bool((1 << (7 - p)) & message[index]);
            receivedCtrlBits[i] = p;
            (ctrlBitNumber)++;
            i++;
        }
        index++;
    }
}

void calculateCtrlBits(char message[], bool calculatedCtrlBits[], int ctrlBitsNumber, int trueLength)
{
    int i, bitPosition = 0, t;
    bool bit;
    for (i = 0; i < ctrlBitsNumber; i++)
    {
        t = (int)pow(2, i);
        bitPosition = t- 1;
        while (bitPosition < trueLength)
        {
            int j = 0;
            while (j < t && bitPosition < trueLength)
            {
                if (bitPosition == t - 1)
                    bit = 0;
                else
                    bit = bool(message[bitPosition / 8] & (1 << (7 - (bitPosition % 8))));
                if ((calculatedCtrlBits[i] + bit) == 1)
                    calculatedCtrlBits[i] = 1;
                else
                    calculatedCtrlBits[i] = 0;
                bitPosition++;
                j++;
            }
            bitPosition = bitPosition + t;
        }
        //calculatedCtrlBits[i] = !calculatedCtrlBits[i]; - если нужно наоборот: четная сумма 1, нечётная 0
    }
}

void getTrueMessage(char message[], char trueMessage[], int messageLength, int trueLength)
{
    int number = 4, i = 2, j = 0;
    for (i = 2; i < trueLength && j < trueLength; i++)
    {
        if (i % (number - 1) == 0)
            number *= 2;
        else if ((message[i / 8]) & (1 << (7 - i % 8)))
        {
            trueMessage[j / 8] |= (1 << (7 - j % 8));
            j++;
        }
        else
        {
            trueMessage[j / 8] &= ~(1 << (7 - j % 8));
            j++;
        }
    }
}
