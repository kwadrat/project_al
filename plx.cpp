#ifndef _PLX_CPP
#define _PLX_CPP

#include "plx.h"

/* Konwerter zestawów polskich liter. Rozpoznaje zestawy:
   CP 1250 - używany przez Windows,
   CP 852 - inaczej zwany IBM Latin 2,
   AmigaPL - standard propagowany przez Magazyn Amiga,
   Mazovia - standard wzięty z polskiego klonu IBM PC.
   ISO Latin-2 - standard w sieci Internet ISO 8859-2.
   Potrafi także konwertować na czyste zestawy znaków ASCII.
 */

/* ąćęłńóśź,ż.ĄĆĘŁŃÓŚŹ,Ż. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __hppa
/* Śmieszna definicja, by HP-UX zauważył funkcje, które czasami zapomina. */
#define _INCLUDE_POSIX_SOURCE
#endif
#include <fcntl.h>
#ifdef __MSDOS__
#include <alloc.h>
#include <io.h>
#endif

/* Liczba stablicowanych zestawów polskich znaków */
#define NUMBER_OF_CP 8

/* Zwykłe litery odpowiadające znakom ASCII bez konwersji */
#define CP_ASCII 0

/* AmigaPL */
#define CP_AMIGA 1

/* Latin-2 */
#define CP_852 2

/* Zestaw znaków używany czasami w Windowsach */
#define CP_1250 3

/* Mazovia */
#define CP_MAZOVIA 4

/* ISO Latin-2 (ISO 8859-2) */
#define CP_ISO 5

/* Standard MeX - układ liter PL z pakietu MeX/LaMeX */
#define CP_MEX 6

/* Standard TeX - wstępnie nie był wliczany w stablicowane standardy,
ale następnie dodany, aby móc odróżnić "z," od "z.". */
#define CP_TEX 7

/* Rozmiar kawałka pliku, który będzie jednorazowo konwertowany */
#define DATA_NIBBLE 100


#ifdef __MSDOS

#define OPEN_CZYTAJ O_RDONLY|O_BINARY
#define OPEN_PISZ O_WRONLY|O_CREAT|O_TRUNC|O_BINARY

#else

#define OPEN_CZYTAJ O_RDONLY
#define OPEN_PISZ O_WRONLY|O_CREAT|O_TRUNC

#endif



typedef unsigned char Byte;

Byte ConvTable[256];



/* ąćęłńóśź,ż.ĄĆĘŁŃÓŚŹ,Ż. */
Byte PolTab[NUMBER_OF_CP][18] =
{
 /* ASCII */
 {'a', 'c', 'e', 'l', 'n', 'o', 's', 'z', 'z',
  'A', 'C', 'E', 'L', 'N', 'O', 'S', 'Z', 'Z'
 },
 /* AmigaPL */
 {
  226, 234, 235, 238, 239, 243, 244, 250, 251,
  194, 202, 203, 206, 207, 211, 212, 218, 219
 },
 /* Latin-2 (CP 852) */
 {
  165, 134, 169, 136, 228, 162, 152, 171, 190,
  164, 143, 168, 157, 227, 224, 151, 141, 189
 },
 /* CP 1250 (Windows) */
 {
  185, 230, 234, 179, 241, 243, 156, 159, 191,
  165, 198, 202, 163, 209, 211, 140, 143, 175
 },
 /* Mazovia */
 {
  134, 141, 145, 146, 164, 162, 158, 166, 167,
  143, 149, 144, 156, 165, 163, 152, 160, 161
 },
 /* ISO Latin-2 */
 {
  177, 230, 234, 179, 241, 243, 182, 188, 191,
  161, 198, 202, 163, 209, 211, 166, 172, 175
 },
 /* MeX */
 {
  161, 162, 166, 170, 171, 243, 177, 185, 187,
  129, 130, 134, 138, 139, 211, 145, 153, 155
 },
 /* TeX - notacja ciachowa */
 {'a', 'c', 'e', 'l', 'n', 'o', 's', 'x', 'z',
  'A', 'C', 'E', 'L', 'N', 'O', 'S', 'X', 'Z'
 }
};



#if 0
/* Wyświetla opis używania programu */
/* prog - nazwa programu, który jest wykonywany */
void Usage(const char * prog)
{
 puts("");
 puts("Program służy do konwersji między różnymi standardami polskich liter.");
 puts("Wywołanie programu:");
 printf("     %s <standard_from> <standard_to> <source> <destination>", prog);
 puts("");
 puts("Standardy kodowania polskich znaków są oznaczane następująco:");
 puts("\"l2\" - IBM Latin 2,");
 puts("\"1250\" - Code Page 1250 (stosowany w MS Windows),");
 puts("\"m\" - Mazovia,");
 puts("\"a\" - AmigaPL,");
 puts("\"iso\" - ISO Latin-2 (ISO 8859-2),");
 puts("\"ascii\" - zwykłe kody ASCII - tracimy tu \"ogonki\".");
 puts("");
 puts("Standardu \"ascii\" nie można używać jako źródłowego.");
 puts("");
 puts("<source> - plik z danymi do skonwertowania");
 puts("<destination> - plik wynikowy");
}
#endif

/* Znajduje kod podanego alfabetu */
int PodajKodAlfabetu(const char * napis)
{
 int kod;
 if( ! strcmp(napis, "l2"))
 {
  kod = CP_852;
 }
 else if( ! strcmp(napis, "1250"))
 {
  kod = CP_1250;
 }
 else if( ! strcmp(napis, "m"))
 {
  kod = CP_MAZOVIA;
 }
 else if( ! strcmp(napis, "a"))
 {
  kod = CP_AMIGA;
 }
 else if( ! strcmp(napis, "ascii"))
 {
  kod = CP_ASCII;
 }
 else if( ! strcmp(napis, "iso"))
 {
  kod = CP_ISO;
 }
 else if( ! strcmp(napis, "tex"))
 {
  kod = CP_TEX;
 }
 else if( ! strcmp(napis, "mex"))
 {
  kod = CP_MEX;
 }
 else
 {
  printf("Napis \"%s\" nie jest jednym z znanych mi standardów.\n", napis);
  kod = -1;
 }
 return kod; /* Sygnalizuj kod błędu */
}

int PlClass::Init(int argc, char * argv[])
{
 int status;
 int i;

 status = STATE_ERROR;
 Slash = 0; /* Na początku nie było znaku "/" */
 if(argc >= 3)
 {
  SrcCode = PodajKodAlfabetu(argv[1]);
  if(SrcCode >= 0)
  {
   DstCode = PodajKodAlfabetu(argv[2]);
   if(DstCode >= 0)
   {
    if(SrcCode != CP_ASCII)
    {
     if(SrcCode != DstCode) /* Tylko wtedy, gdy mamy różne standardy */
     {
      status = 3; /* Zużywamy 3 elementy z listy parametrów */
      /* Ustawiamy tablicę konwersji na brak konwersji */
      for(i = 0; i < 256; i++)
      {
       TransTab[i] = (Byte) i;
      }
      if(SrcCode == CP_TEX) /* Czy będziemy kodować z konwencji TeX'a */
      {
       for(i = 0; i < 256; i++)
       {
        TransTab[i] = 0; /* Zerujemy tablicę */
       }
       for(i = 0; i < 18; i++)
       {
#define POPRAWKA 1
#if POPRAWKA
        TransTab[PolTab[CP_TEX][i]] = PolTab[DstCode][i];
#else
        TransTab[PolTab[CP_ASCII][i]] = PolTab[DstCode][i];
#endif
       }
       TransTab[(Byte) '/'] = (Byte) '/'; /* Zamiana podwójnego na pojedynczy */
      }
      else if(DstCode == CP_TEX) /* Czy będziemy zamieniać na konwencję TeX'a */
      {
       for(i = 0; i < 256; i++)
       {
        TransTab[i] = 0; /* Zerujemy tablicę */
       }
       for(i = 0; i < 18; i++)
       {
#if POPRAWKA
        TransTab[PolTab[SrcCode][i]] = PolTab[CP_TEX][i];
#else
        TransTab[PolTab[SrcCode][i]] = PolTab[CP_ASCII][i];
#endif
       }
       TransTab[(Byte) '/'] = (Byte) '/'; /* Zamiana podwójnego na pojedynczy */
      }
      else /* Mamy do czynienia z normalną konwersją */
      {
       for(i = 0; i < 18; i++)
       {
        TransTab[PolTab[SrcCode][i]] = PolTab[DstCode][i];
       }
      }
     }
     else
     {
      SygError("Standard wejściowy jest identyczny z wyjściowym.");
     }
    }
    else
    {
     SygError("Nie dopuszczamy standardu \"ascii\" jako źródłowego.");
    }
   }
   else
   {
    SygError("Błędny typ wyjściowego kodowania polskich liter.");
    fprintf(stderr, "Typ: \"%s\".\n", argv[2]);
   }
  }
  else
  {
   SygError("Błędny typ wejściowego kodowania polskich liter.");
   fprintf(stderr, "Typ: \"%s\".\n", argv[1]);
  }
 }
 else
 {
  SygError("Brak nazw formatów kodów wejściowych i wyjściowych.");
 }
 return status;
}

void PlClass::Work(void)
{
 int i, ile;
 int DstStart; /* Ilość znaków w buforze, zarezerwowanych na wejście */
 Byte * wsk;
 Byte * DstPoczPtr;
 Byte znak;
 if(SrcCode == CP_TEX)
 {
  DstStart = BUF_DEFSIZE / 2;
  while((ile = PrevBuf->GetByteArea(LocalBuf, DstStart)) > 0)
  {
   DstPoczPtr = wsk = LocalBuf + DstStart;
   for(i = 0; i < ile; i++)
   {
    /* Przetwarzamy kolejne znaki napływające z wejścia */
    if(Slash)
    {
     /* Poprzednio był "/" */
     znak = TransTab[LocalBuf[i]];
     if(znak != 0)
     {
      *wsk++ = znak;
     }
     else /* Znak był równy 0 - czyli przepisujemy oryginalny znak */
     {
      *wsk++ = LocalBuf[i];
     }
     Slash = 0; /* Prefix przestaje obowiązywać */
    }
    else /* Nie było poprzedzającego znaku "/" */
    {
     if((znak = LocalBuf[i]) == '/') /* Czy mamy znak "/" */
     {
      Slash = 1;
     }
     else
     {
      *wsk++ = znak; /* Przesyłamy nietknięty znak */
     }
    }
   }
   NextBuf->PutByteArea(DstPoczPtr, wsk - DstPoczPtr);
  }
 }
 else if(DstCode == CP_TEX)
 {
  DstStart = BUF_DEFSIZE / 3; /* Tylko 1/3 bufora na wejściowe znaki */
  while((ile = PrevBuf->GetByteArea(LocalBuf, DstStart)) > 0)
  {
   DstPoczPtr = wsk = LocalBuf + DstStart;
   for(i = 0; i < ile; i++)
   {
    /* Teraz przetwarzamy kolejne znaki napływające z wejścia */
    if((znak = TransTab[LocalBuf[i]]) != 0)
    {
     *wsk++ = '/'; /* Generujemy znak "/", poprzedzający znak specjalny */
     *wsk++ = znak;
    }
    else
    {
     *wsk++ = LocalBuf[i];
    }
   }
   NextBuf->PutByteArea(DstPoczPtr, wsk - DstPoczPtr);
  }
 }
 else /* Konwertujemy normalne zestawy znaków */
 {
  while((ile = PrevBuf->GetByteArea(LocalBuf, BUF_DEFSIZE)) > 0)
  {
   wsk = LocalBuf;
   for(i = 0; i < ile; i++)
   {
    *wsk = TransTab[*wsk];
    wsk++;
   }
   NextBuf->PutByteArea(LocalBuf, ile);
  }
 }
 if(ile == EOF)
 {
  SignalEndOfData();
 }
}


#endif
