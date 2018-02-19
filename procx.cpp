#ifndef _PROCX_CPP
#define _PROCX_CPP

#include "procx.h"


int ProcessClass::Init(int, char *[])
{
 return 0;
}

/******************************************************************************/

void ProcessClass::Work(void)
{
}

/******************************************************************************/

ProcessClass::ProcessClass(void)
{
 State = STATE_ON; /* Na początku procedura działa */
 PrevBuf = NextBuf = NULL; /* Na początku nie ma powiązań */
}

/******************************************************************************/

ProcessClass::~ProcessClass(void)
{
 /* Koniec działania procedury */
}

/******************************************************************************/

void ProcessClass::SignalEndOfData(void)
{
 /* Koniec działania procedury */
 if(NextBuf != NULL)
 {
  NextBuf->SignalEndOfData(); /* Powiadom następny bufor o końcu danych */
 }
 State = EOF;
}

/******************************************************************************/

/* Informuje, że proces już nie będzie żądał danych */
int ProcessClass::StateEOF(void)
{
 return (State == EOF) ? 1 : 0;
}

/******************************************************************************/

#endif
