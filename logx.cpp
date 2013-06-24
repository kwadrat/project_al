#include "logx.h"
#include "inoutx.h"
#include "procx.h"
#include "masterx.h"
#include "catx.h"
#include "plx.h"
#include "hexx.h"

#include <string.h>

extern MasterClass TopMember;


#define StrCmp strcmp /* To dla możliwości wprowadzania "not case sensitive" */
#define IleArgumentow (n - index) /* Liczba pozostałych argumentów */

/******************************************************************************/

/* Funkcja do przetwarzania wprowadzanych z klawiatury i powtarzających
   się sekwencji analizy napisu wejściowego. Wartość zwracana: 1 - OK,
   0 - wystąpił błąd, który nie pozwala na kontynuację działania */
int CreateChainLink(void * ogniwo, const char * NazwaTypu, int * index,
                    int n, char * s[])
{
 int status;
 ProcessClass * tmp; /* Tu wstawiamy kolejno utworzone ogniwa */
 static char Komunikat[80];
 int IleArg; /* Liczba argumentów zużywanych przez ogniwo */

 status = 1;
 tmp = (ProcessClass *) ogniwo;
 if(tmp != NULL)
 {
  IleArg = tmp->Init((n - (*index)), s + (*index));
  if(IleArg != STATE_ERROR)
  {
   if( ! TopMember.AddProcess(tmp))
   {
    status = 0;
   }
   (*index) += IleArg;
  }
  else
  {
   status = 0;
  }
 }
 else
 {
  sprintf(Komunikat, "Błąd tworzenia obiektu %s.", NazwaTypu);
  SygError(Komunikat);
  status = 0; /* Błąd przetwarzania */
 }
 return status;
}

/******************************************************************************/

int CreateChainLogic(int n, char * s[])
{
 int status; /* Zerowanie oznacza, że wystąpił błąd przetwarzania */
 int IleArg; /* Liczba przetworzonych argumentów */
 int index; /* Numer aktualnie przetwarzanego parametru */
 int BylO, BylIO; /* Sygnalizują wystąpienie kodu "o" i "io" */
 ProcessClass * tmp;

 status = 1; /* Kod statusu oznaczający - na razie wszystko OK */
 BylO = BylIO = 0; /* Na początku nie było żadnego kodu */
 index = 0; /* Początkowy indeks elementu */
 while(index < n && status)
 {
  if(index == 0) /* To tylko dla pierwszego elementu ciągu */
  {
   if( ! StrCmp(s[index], "i") || ! StrCmp(s[index], "io"))
   {
    if(IleArgumentow >= 2)
    {
     tmp = (ProcessClass *) new ProcIFileClass;
     if(tmp != NULL)
     {
      IleArg = tmp->Init(IleArgumentow, s + index);
      if(IleArg != STATE_ERROR)
      {
       if( ! StrCmp(s[index], "io"))
       {
        TopMember.NazwaPlikuIO = s[index + 1];
        BylIO = 1; /* Był plik, który był plikiem wejściowym i wyjściowym */
       }
       if( ! TopMember.AddProcess(tmp)) /* Tu dodajemy proces do potoku */
       {
        status = 0; /* Nie udało się dodać procesu do łańcucha */
       }
       index += IleArg;
      }
      else
      {
       status = 0; /* Błąd przetwarzania */
      }
     }
     else
     {
      SygError("Błąd tworzenia obiektu ProcIFileClass.");
      status = 0; /* Błąd przetwarzania */
     }
    }
    else /* Nie ma argumentu - nazwy pliku wejściowego */
    {
     SygError("Brak nazwy pliku dla opcji \"i\".");
    }
   }
   else /* Otwieramy standardowy strumień wejściowy */
   {
    tmp = (ProcessClass *) new ProcIClass;
    if(tmp != NULL)
    {
     IleArg = tmp->Init(IleArgumentow, s + index);
     if(IleArg != STATE_ERROR)
     {
      if( ! TopMember.AddProcess(tmp)) /* Dodaj proces do potoku */
      {
       status = 0;
      }
      /* index += IleArg; */ /* To nie było brane z argumentów */
     }
     else /* Init zwrócił błąd */
     {
      status = 0; /* Błąd */
     }
    }
    else
    {
     SygError("Błąd przy tworzeniu obiektu ProcIClass.");
    }
   }
  }
  if(index < n)
  {
   if( ! StrCmp(s[index], "o"))
   {
    if(BylO == 0)
    {
     BylO = 1;
     if(BylIO == 0)
     {
      tmp = (ProcessClass *) new ProcOFileClass;
      if(tmp != NULL)
      {
       IleArg = tmp->Init(IleArgumentow, s + index);
       if(IleArg != STATE_ERROR)
       {
        if( ! TopMember.AddProcess(tmp))
        {
         status = 0; /* Błąd przy dodawaniu procesu do potoku */
        }
        index += IleArg; /* To nie jest z argumentów */
       }
       else
       {
        status = 0; /* Nie udała się inicjalizacja */
       }
      }
      else
      {
       SygError("Błąd tworzenia obiektu ProcOFileClass.");
       status = 0;
      }
     }
     else
     {
      SygError("Napotkano jednocześnie \"io\" jak i \"o\".");
     }
    }
    else
    {
     SygError("Drugie wywołanie opcji \"o\".");
    }
   }
   else if( ! StrCmp(s[index],"cat"))
   {
    status = CreateChainLink(new CatClass, "CatClass", & index, n, s);
   }
   else if( ! StrCmp(s[index],"Yes13"))
   {
    status = CreateChainLink(new Yes13Class, "Yes13Class", & index, n, s);
   }
   else if( ! StrCmp(s[index],"No13"))
   {
    status = CreateChainLink(new No13Class, "No13Class", & index, n, s);
   }
   else if( ! StrCmp(s[index],"pl"))
   {
    status = CreateChainLink(new PlClass, "PlClass", & index, n, s);
   }
   else if( ! StrCmp(s[index],"hex"))
   {
    status = CreateChainLink(new HexClass, "HexClass", & index, n, s);
   }
   else if( ! StrCmp(s[index],"unhex"))
   {
    status = CreateChainLink(new UnHexClass, "UnHexClass", & index, n, s);
   }
   else
   {
    char tab[200];
    printf("Index = %d, tekst = %s\n", index, s[index]);
    sprintf(tab, "Nierozpoznana opcja \"%s\".", s[index]);
    SygError(tab);
    status = 0; /* To nie została rozpoznana opcja */
   }
  }
 }
 if(BylIO == 0 && BylO == 0) /* Nie było frazy "out", więc sami otwieramy
                                standardowe wyjście */
 {
  tmp = (ProcessClass *) new ProcOClass;
  if(tmp != NULL)
  {
   IleArg = tmp->Init(IleArgumentow, s + index);
   if(IleArg != STATE_ERROR)
   {
    if( ! TopMember.AddProcess(tmp))
    {
     status = 0; /* Błąd przy dodawaniu procesu do potoku */
    }
   }
   else
   {
    status = 0; /* Błąd inicjalizacji */
   }
  }
  else
  {
   SygError("Błąd tworzenia obiektu ProcOClass.");
   status = 0;
  }
 }
 else /* Spodziewamy się, że była opcja IO i należy utworzyć końcówkę */
 {
  if(BylIO != 0)
  {
   /* Musimy utworzyć końcówkę */
   tmp = (ProcessClass *) new ProcIOFileClass;
   if(tmp != NULL)
   {
    IleArg = tmp->Init(IleArgumentow, s + index);
    if(IleArg != STATE_ERROR)
    {
     if( ! TopMember.AddProcess(tmp))
     {
      status = 0; /* Błąd przy dodawaniu procesu do potoku */
     }
     index += IleArg; /* To nie jest z argumentów */
    }
    else
    {
     status = 0; /* Nie udała się inicjalizacja */
    }
   }
   else
   {
    SygError("Błąd tworzenia obiektu ProcOFileClass.");
    status = 0;
   }
  }
 }
 return status;
}

/******************************************************************************/

/*
Do zrobienia:
- końcowa obsługa pliku wyjściowego "io"

 */
