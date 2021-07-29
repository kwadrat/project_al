#ifndef _HEXX_CPP
#define _HEXX_CPP


#include "hexx.h"

#include <string.h>
#include <stdlib.h>

int HexClass::Init(int, char *[])
{
    IleBuf = 0; /* Na razie w "Buf" nie ma żadnych znaków */
    LiczBajtow = 0; /* Nie przekonwertowaliśmy jeszcze żadnych bajtów */
    return 1;
}

void HexClass::Work(void)
{
    int ile;

    while((ile = PrevBuf->GetByteArea((Byte *)(Buf_in + IleBuf),
                   HEXX_SZER - IleBuf)) > 0)
    {
        IleBuf += ile;
        if(IleBuf > HEXX_SZER || IleBuf < 0)
        {
            SygError("Zła liczba danych w buforze!");
            exit(0);
        }
        else
        {
            if(IleBuf == HEXX_SZER) /* Czy mamy już pełny bufor */
            {
                /* Zaczynamy wydruk treści bufora w trybie szesnastkowym */
                LineOut();
            }
        }
    }
    if(ile == EOF)
    {
        if(IleBuf != 0) /* Czy coś jest w buforze do wydruku */
        {
            LineOut(); /* Wyświetl ostatnią linijkę bufora */
        }
        SignalEndOfData();
    }
}

/* Drukuje w porządnym formacie linijkę tekstu */
void HexClass::LineOut(void)
{
    int i;
    Byte b;
    /*
    - drukuj numer bajtu
    - drukuj kolejne wartości w trybie HEX
    - drukuj wartości w trybie ASCII
    - koniec linii
    - zwiększ liczbę pokazanych bajtów
    */
    memset(Linia, ' ', HEXX_CPR);
    sprintf(Pcs, "%08x  ", LiczBajtow);
    strncpy(Linia, Pcs, HEX_OFFSET);
    for(i = 0; i < IleBuf; i++)
    {
        sprintf(Pcs, "%02x", (Byte) Buf_in[i]);
        strncpy(Linia + HEX_OFFSET + 3 * i, Pcs, 2);
    }
    for(i = 0; i < IleBuf; i++)
    {
        b = (Byte) Buf_in[i];
        Linia[TXT_OFFSET + i] = ((b < 32) || (b > 126)) ? '.' : (char) b;
    }
    Linia[NEWLINE_OFFSET] = '\n';
    NextBuf->PutByteArea((Byte *)Linia, ZERO_OFFSET); /* Wydruk aż do znaku LF */
    LiczBajtow += IleBuf;
    IleBuf = 0;
}

int UnHexClass::Init(int, char *[])
{
    IleWLinii = 0; /* Na początku bufor jest pusty */
    return 1;
}

/* Funkcja przekształcająca linię ASCII w plik binarny, zgodnie z "hex" */
void UnHexClass::Work(void)
{
    int ile;
    while((ile = PrevBuf->GetByteArea((Byte *)(Linia + IleWLinii),
    (IleWLinii == 0) ? 76 : 1)) > 0)
    {
        IleWLinii += ile;
        if(IleWLinii >= HEXX_CPR) /* Czy przekraczamy pojemność bufora */
        {
            SygError("Przekroczenie wielkości bufora Linia.");
            exit(0);
        }
        else
        {
            if(Linia[IleWLinii - 1] == '\n') /* Czy osiągnęliśmy koniec linii */
            {
                DecodeLine(); /* Wydrukuj linię */
            }
        }
    }
    if(ile == EOF)
    {
        if(IleWLinii > 0)
        {
            DecodeLine(); /* Próbujemy dekodować ostatnie znaki */
        }
        else
        {
            if(IleWLinii < 0)
            {
                SygError("Ujemna liczba znaków w buforze !");
            }
        }
        SignalEndOfData();
    }
}

/* Funkcja próbuje dekodować linię */
void UnHexClass::DecodeLine(void)
{
    int value;
    int i;

    for(i = 0; i < 16; i++)
    {
        Linia[HEX_OFFSET + 3 * i + 2] = '\0'; /* Zakończ napis - nie ma więcej znaków */
        if(sscanf(Linia + HEX_OFFSET + 3 * i, "%x", & value) == 1)
        {
            /* Tu mamy już zdekodowaną liczbę, teraz ją wysyłamy */
            Buf_out[i] = (char) value;
        }
        else
        {
            break; /* Koniec wczytywania - napotkaliśmy dziwny znak */
        }
    }
    NextBuf->PutByteArea((Byte *) Buf_out, i); /* Wyślij zebraną linię */
    IleWLinii = 0; /* Nie mamy już znaków w linii */
    memset(Linia, '\0', HEXX_CPR); /* Czyścimy pamięć */
}

void HexClass::DescribeColumns(void)
{
    Linia[0] = '\n';
    NextBuf->PutByteArea((Byte *)Linia, 1); /* Wydruk pustej linii oddzielającej nagłówek od danych */
}

#endif

