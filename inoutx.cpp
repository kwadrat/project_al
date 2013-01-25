#ifndef _INOUT_CPP
#define _INOUT_CPP

#include "inoutx.h"
#include "masterx.h"
#include <stdio.h>
#include <stdlib.h>


extern MasterClass TopMember;


int ProcIClass::Init(int argc, char * argv[])
{
 return 1;
}

/******************************************************************************/

void ProcIClass::Work(void)
{
 int ile;

 while((ile = (int) fread(LocalBuf, 1, BUF_DEFSIZE, stdin)) > 0)
 {
  NextBuf->PutByteArea(LocalBuf, ile);
 }
 SignalEndOfData();
}

/******************************************************************************/

int ProcIFileClass::Init(int argc, char * argv[])
{
 int status;
 status = STATE_ERROR;
 if(argc >= 2)
 {
  plik = fopen(argv[1], "rb");
  if(plik != NULL)
  {
   status = 2; /* Zużyto dwa elementy */
  }
  else
  {
   SygError("Błąd otwarcia pliku wejściowego do odczytu.");
  }
 }
 else
 {
  SygError("Brak nazwy pliku wejściowego.");
 }
 return status;
}

/******************************************************************************/

void ProcIFileClass::Work(void)
{
 int ile;

 while((ile = (int) fread(LocalBuf, 1, BUF_DEFSIZE, plik)) > 0)
 {
  NextBuf->PutByteArea(LocalBuf, ile);
 }
 fclose(plik); /* Najpierw zamykamy plik, a potem dajemy znać innym */
 SignalEndOfData();
}

/******************************************************************************/

int ProcOClass::Init(int argc, char * argv[])
{
 return 1;
}

/******************************************************************************/

void ProcOClass::Work(void)
{
 int ile;
 while((ile = PrevBuf->GetByteArea(LocalBuf, BUF_DEFSIZE)) > 0)
 {
  fwrite(LocalBuf, 1, (size_t) ile, stdout);
 }
 if(ile == EOF)
 {
  State = EOF;
 }
}

/******************************************************************************/

int ProcOFileClass::Init(int argc, char * argv[])
{
 int status;
 status = STATE_ERROR;
 if(argc >= 2)
 {
  plik = fopen(argv[1], "wb");
  if(plik != NULL)
  {
   status = 2; /* Zużyto dwa elementy */
  }
  else
  {
   SygError("Błąd otwarcia pliku wyjściowego do zapisu.");
  }
 }
 else
 {
  SygError("Brak nazwy pliku wyjściowego.");
 }
 return status;
}

/******************************************************************************/

void ProcOFileClass::Work(void)
{
 int ile;
 while((ile = PrevBuf->GetByteArea(LocalBuf, BUF_DEFSIZE)) > 0)
 {
  fwrite(LocalBuf, 1, (size_t) ile, plik);
 }
 if(ile == EOF)
 {
  if(plik != NULL)
  {
   fclose(plik);
   plik = NULL;
  }
  State = EOF;
 }
}

/******************************************************************************/

int ProcIOFileClass::Init(int argc, char * argv[])
{
 int status;
 status = STATE_ERROR;
 if(argc < 2)
 {
  plik = tmpfile(); /* Otwieramy tymczasowy plik do zapisu i odczytu */
  if(plik != NULL)
  {
   IleBajtow = 0; /* Na razie nie zapisano żadnego bajtu */
   status = 1; /* Zużyto jeden element, ale to już nieważne */
  }
  else
  {
   SygError("Błąd otwarcia pliku wyjściowego do zapisu.");
  }
 }
 else
 {
  printf("Liczba argumentów wynosi = %d, np. \"%s\".\n", argc, argv[0]);
  SygError("Tu w ogóle nie powinno być argumentów.");
 }
 return status;
}

/******************************************************************************/

void ProcIOFileClass::Work(void)
{
 int ile;
 size_t zapisano;
 while((ile = PrevBuf->GetByteArea(LocalBuf, BUF_DEFSIZE)) > 0)
 {
  zapisano = fwrite(LocalBuf, 1, (size_t) ile, plik);
  if(zapisano >= 0)
  {
   IleBajtow += zapisano;
  }
  else /* Czyżby błąd podczas zapisu */
  {
   SygError("Błąd podczas zapisu do pliku.\n");
  }
 }
 if(ile == EOF)
 {
  if(plik != NULL)
  {
   FILE * out;
   size_t ile;
   size_t ile2;
   
   /* Przed zamknięciem pliku dokonamy przepisania treści pliku do oryginału */
   if(TopMember.NazwaPlikuIO != NULL)
   {
    out = fopen(TopMember.NazwaPlikuIO, "wb");
    if(out != NULL)
    {
     fseek(plik, 0, SEEK_SET); /* Początek pliku */
     zapisano = IleBajtow; /* Ile mamy do zapisania */
     while((ile = ((zapisano < BUF_DEFSIZE) ?
                   (zapisano) : (BUF_DEFSIZE))) > 0)
     {
      ile2 = fread(LocalBuf, 1, ile, plik);
      if(ile2 != ile)
      {
       SygError("Nie odczytano właściwej liczby bajtów.");
       exit(0);
      }
      ile2 = fwrite(LocalBuf, 1, ile, out);
      if(ile2 != ile)
      {
       SygError("Nie zapisano właściwej liczby bajtów.");
       exit(0);
      }
      zapisano -= ile;
     }
     fclose(out);
    }
    else
    {
     SygError("Nie udało się otworzyć pliku \"io\" do zapisu.");
    }
   }
   else
   {
    SygError("Nazwa \"TopMember.NazwaPlikuIO\" jest pusta.");
   }
   fclose(plik);
   plik = NULL;
  }
  State = EOF;
 }
}

/******************************************************************************/

#endif
