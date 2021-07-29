#ifndef _HEXX_H
#define _HEXX_H

#include "procx.h"

#define HEXX_SZER HEX_SIZE /* Liczba bajtów dekodowanych w jednej linii */
#define HEX_OFFSET 10 /* Numer kolumny z wartościami hex */
#define TXT_OFFSET ((HEX_OFFSET) + 3 * (HEXX_SZER) + 1) /* Numer kolumny z tekstową reprezentacją */
#define NEWLINE_OFFSET (TXT_OFFSET + HEXX_SZER)
#define ZERO_OFFSET (NEWLINE_OFFSET + 1)
#define HEXX_CPR (ZERO_OFFSET + 1) /* Wielkość buforu dla jednej linii */


class HexClass : public ProcessClass
{
 char Linia[HEXX_CPR]; /* Tu tworzymy obraz linii */
 char Buf_in[HEXX_SZER]; /* Tu zbieramy kawałki jednej linii */
 char Pcs[HEX_OFFSET + 1]; /* Pieces - obszar roboczy do konstrukcji napisów */
 int RunOnce; /* Dla jednokrotnego wypisania nagłówka */
 int IleBuf; /* Liczba znaków już zgromadzonych w buforze Buf */
 int LiczBajtow; /* Ile bajtów już wydrukowaliśmy */
 void LineOut(void); /* Wydrukuj treść bufora */
 void DescribeColumns(void);
 virtual int Init(int, char *[]);
 virtual void Work(void);
};

class UnHexClass : public ProcessClass
{
 char Linia[HEXX_CPR]; /* Tu tworzymy obraz linii */
 char Buf_out[HEXX_SZER]; /* Tu zbieramy znaki linii wyjściowej */
 int IleWLinii; /* Liczba znaków zgromadzonych w linii */
 void DecodeLine(void); /* Zamiana z ASCII na kod binarny */
 virtual int Init(int, char *[]);
 virtual void Work(void);
};

#endif
