#ifndef _HEXX_H
#define _HEXX_H

#include "procx.h"

#define HEXX_SZER 16 /* Liczba bajtów dekodowanych w jednej linii */
#define HEXX_CPR 80 /* Wielkość buforów dla jednej linii */
#define HEX_OFFSET 10 /* Numer kolumny z wartościami hex */


class HexClass : public ProcessClass
{
 char Linia[HEXX_CPR]; /* Tu tworzymy obraz linii */
 char Buf[HEXX_SZER]; /* Tu zbieramy kawałki jednej linii */
 char Pcs[HEXX_SZER]; /* Pieces - obszar roboczy do konstrukcji napisów */
 int IleBuf; /* Liczba znaków już zgromadzonych w buforze Buf */
 int LiczBajtow; /* Ile bajtów już wydrukowaliśmy */
 void LineOut(void); /* Wydrukuj treść bufora */
 virtual int Init(int, char *[]);
 virtual void Work(void);
};

class UnHexClass : public ProcessClass
{
 char Linia[HEXX_CPR]; /* Tu tworzymy obraz linii */
 char Buf[HEXX_SZER]; /* Tu zbieramy znaki linii wyjściowej */
 int IleWLinii; /* Liczba znaków zgromadzonych w linii */
 void DecodeLine(void); /* Zamiana z ASCII na kod binarny */
 virtual int Init(int, char *[]);
 virtual void Work(void);
};

#endif
