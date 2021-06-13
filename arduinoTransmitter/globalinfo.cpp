#include "globalinfo.h"

namespace global
{
    char sendBuffer[maxBufferSize + 8]; // 8 - ������ ��� ����������� �����. 

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
        global::initializeTestPackage();
        //sendBuffer[0] = Serial.read();
        //isLastTransmission = sendBuffer[0] != 0;
        //Serial.readBytes(sendBuffer + 1, (sendBuffer[0]) ? sendBuffer[0] : (maxBufferSize - 1));
    }

    void calculateCtrlBits(char message[], bool calculatedCtrlBits[], int ctrlBitsNumber, int trueLength)
    {
        int i, bitPosition = 0, t;
        bool bit;
        for (i = 0; i < ctrlBitsNumber; i++)
        {
            t = (int)pow(2, i);
            bitPosition = t- 1;
            while (bitPosition < trueLength*8)
            {
                int j = 0;
                while (j < t && bitPosition < trueLength*8)
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

    void getTrueTrueMessage(char trueMessage[], int trueLength, bool calculatedCtrlBits[], int ctrlBitsNumber)
    {
        int number = 1, i, j = 0;
        for (i = 0; i < trueLength*8 && j < ctrlBitsNumber;i++)
        {
            if (i == number - 1)
            {
                number *= 2;

                if (calculatedCtrlBits[j] == 1)
                {
                    trueMessage[i / 8] |= (1 << (7 - i % 8));
                }
                j++;
            }
        }
    }

    void getTrueMessage(char message[], char trueMessage[], int messageLength, int trueLength)
    {
        int number = 1, i, j = 0;
        for (i = 0; i < trueLength*8 && j < trueLength*8;)
        {
            if (j == number - 1)
            {
                number *= 2;

                trueMessage[j / 8] &= ~(1 << (7 - j % 8));

                j++;
            }
            else if ((message[i / 8]) & (1 << (7 - i % 8)))
            {
                trueMessage[j / 8] |= (1 << (7 - j % 8));
                i++;
                j++;
            }
            else
            {
                trueMessage[j / 8] &= ~(1 << (7 - j % 8));
                i++;
                j++;
            }
        }
    }

    int hem(char *str, int size_str, char *str_out, int size_str_out)
    {
        int ctrlBitsNumber = 7;
        bool calculatedCtrlBits[ctrlBitsNumber];
        for (int i = 0; i < ctrlBitsNumber; i++)
        {
            calculatedCtrlBits[i] = 0;
        }
        for (int i = 0; i < size_str_out; i++)
        {
            str_out[i] = 0;
        }

        getTrueMessage(str, str_out, size_str, size_str_out);

        calculateCtrlBits(str_out, calculatedCtrlBits, ctrlBitsNumber, size_str_out);
        
        getTrueTrueMessage(str_out, size_str_out, calculatedCtrlBits, ctrlBitsNumber);

        return 0;
    }

    void codeInfo()
    {
        int size_sendBuffer = int(sendBuffer[0]);

        char code_sendBuffer[maxBufferSize + 8];
        int i_code_sendBuffer = 0;

        char miniBuffer[8], code_miniBuffer[9];
        int j = 0;

        for (int i = 1; i < size_sendBuffer; i++)
        {
            if (i % 8 != 0)
            {
                miniBuffer[j] = sendBuffer[i];
            }
            else
            {
                hem(miniBuffer, j, code_miniBuffer, j + 1);
                for (int k = 0; k < j && i_code_sendBuffer < size_sendBuffer + 8; k++)
                {
                    code_sendBuffer[i_code_sendBuffer++] = code_miniBuffer[k];
                }
                j = 0;
            }
        }

        for (int i = 1; i < size_sendBuffer; i++)
        {
            sendBuffer[i] = code_sendBuffer[i - 1];
        }
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
