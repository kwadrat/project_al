#include "bufx.h"
#include "typy.h"

#include <stdlib.h>
#include <string.h>

BufClass::BufClass(void)
{
 BufLen = BUF_DEFSIZE; /* Domyślna wielkość bufora */
 BufPtr = (Byte *) malloc(BufLen);
 if(BufPtr != NULL)
 {
  BufEnd = BufPtr + BufLen;
  FirstDataPtr = FirstFreePtr = BufPtr;
  DataAmount = 0; /* Na razie brak danych w buforze */
  InputState = STATE_ON; /* Bufor działa */ 
 }
 else /* Brakło pamięci na utworzenie bufora */
 {
  SygError("BufClass: Konstruktor -- brak pamięci.");
 }
 PrevProcess = NextProcess = NULL; /* Na początku nie ma powiązań */
}

/******************************************************************************/

BufClass::~BufClass(void)
{
 if(BufPtr != NULL) /* Usuwamy przydzielony obszar pamięci */
 {
  free(BufPtr);
  BufPtr = NULL;
 }
}

/******************************************************************************/

/* Dopisuje do bufora bajty */
void BufClass::PutByteArea(Byte * obszar, int ile)
{
 while(ile > 0)
 {
  FreeBytes = BufLen - DataAmount;
  ToEnd = BufEnd - FirstFreePtr;
  /* Spróbuj zapisać trochę danych do bufora */
  Zapis = (ile < FreeBytes) ? ile : FreeBytes;
  /* Próbujemy zapisać "Zapis" bajtów za tym razem */
  Part1 = (Zapis < ToEnd) ? Zapis : ToEnd;
  Part2 = Zapis - ToEnd; /* Liczba bajtów do zapisania w drugim buforze */
  /* Zapis części "Part1" */
  memcpy(FirstFreePtr, obszar, Part1);
  if(Part2 >= 0)
  {
   /* Zapis na pewno zapełnił pierwszy etap bufora */
   if(Part2 > 0) /* Jeśli są jeszcze jakieś dane, to je zapisujemy */
   {
    memcpy(BufPtr, obszar + Part1, Part2);
   }
   /* Poprawiamy wskaźniki */
   FirstFreePtr = BufPtr + Part2;
  }
  else /* Zapis składał się tylko z jednej części zapisu */
  {
   /* Poprawiamy wskaźniki */
   FirstFreePtr += Zapis;
  }
  obszar += Zapis;
  ile -= Zapis;
  DataAmount += Zapis;
  /* Jeśli zapełniliśmy do końca bufor, to próbujemy popchnąć dane dalej */
  if(DataAmount == BufLen)
  {
   NextProcess->Work();
  }
 }
}

/******************************************************************************/

/* Dopisuje do bufora bajty */
int BufClass::GetByteArea(Byte * obszar, int ile)
{
 Zapis = ile < DataAmount ? ile : DataAmount; /* Tyle będziemy zapisywać */
 if(Zapis > 0)
 {
  /* Tu mamy dane, które możemy przetransmitować do bufora */
  ToEnd = BufEnd - FirstDataPtr;
  Part1 = Zapis < ToEnd ? Zapis : ToEnd; /* Ile w pierwszej paczce */
  Part2 = Zapis - ToEnd;
  memcpy(obszar, FirstDataPtr, Part1);
  if(Part2 >= 0) /* Mamy przynajmniej trochę dwuczęściową transmisję */
  {
   if(Part2 > 0)
   {
    memcpy(obszar + Part1, BufPtr, Part2);
   }
   FirstDataPtr = BufPtr + Part2;
  }
  else /* Tu będzie transmisja tylko z jednego bloku */
  {
   FirstDataPtr += Zapis;
  }
  DataAmount -= Zapis;
 }
 else
 {
  if(Zapis < 0)
  {
   /* To jest wywoływane, gdy w buforze jest "x" bajtów (x < 0) lub
      gdy proces poprosił o "x" bajtów */
   SygError("To jakiś WIELKI BŁĄD !!! To się nie powinno zdarzyć!!!");
  }
  if(InputState == STATE_ON)
  {
   Zapis = 0; /* Brak chwilowo danych */
  }
  else
  {
   Zapis = EOF; /* Więcej już nie będzie danych */
  }
 }
 return Zapis;
}

/******************************************************************************/

/* Sygnalizacja, że już nie będzie danych */
void BufClass::SignalEndOfData(void)
{
 InputState = EOF;
 while( ! NextProcess->StateEOF())
 {
  NextProcess->Work();
 }
}

/******************************************************************************/
