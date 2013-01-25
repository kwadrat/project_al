#ifndef _BUFX_H
#define _BUFX_H

class BufClass;

#include "typy.h"
#include "procx.h"

class BufClass
{
 int ch;
#if 0
 void PutByte(int);
 int GetByte(void);
#endif
 int InputState; /* 1 - working, EOF - stopped - było EOF */

public:
 ProcessClass * PrevProcess;
 ProcessClass * NextProcess;
 Byte * BufPtr; /* Wskaźnik do początku bufora */
 int BufLen; /* Długość bufora */
 Byte * BufEnd; /* Wskaźnik do końca bufora */
 Byte * FirstDataPtr; /* Wskaźnik do pierwszego zajętego bajtu */
 Byte * FirstFreePtr; /* Wskaźnik do pierwszego wolnego bajtu */
 int DataAmount; /* Liczba danych w buforze */
 int GetByteArea(Byte *, int); /* Odczytuje z bufora ciąg bajtów */
 void PutByteArea(Byte *, int); /* Dopisuje do bufora bajty */
 void SignalEndOfData(void); /* Sygnalizacja, że już nie będzie danych */
 BufClass(void);
 ~BufClass(void);
private:
 int FreeBytes, ToEnd, Zapis, Part1, Part2;
};

#endif
