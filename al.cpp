#include "typy.h"
#include "bufx.h"
#include "procx.h"
#include "logx.h"
#include "masterx.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define THIS_PROGRAM_VERSION "1.03"

/* Sat Dec 13 10:27:51 1997 */

MasterClass TopMember; /* Tu jest definicja jedynego członka klasy TopClass */

/* Funkcja sprawdza, czy dwa ciągi są równe, nie brać pod uwagę wielkości
   liter */
int CaseStrcmp(char * a, const char * b)
{
 while(*a && *b)
 {
  if(toupper(*a) == toupper(*b))
  {
   a++;
   b++;
  }
  else
  {
   break; /* Znaleźliśmy różne znaki */
  }
 }
 return (*a == *b) ? 0 : 1;
}

/******************************************************************************/

void ShowHelp(int argc, char * argv[])
{
 if(argc == 2)
 {
  puts(
"Projekt AL v" THIS_PROGRAM_VERSION " (kwadrat@polsl.gliwice.pl)\n"
"Program służy do przetwarzania strumienia danych. Możliwe są filtry:\n"
"i \"plik\" - wczytanie danych z pliku,\n"
"o \"plik\" - zapis danych do pliku,\n"
"io \"plik\" - odczyt danych z pliku i zapis do tego samego pliku (to\n"
"   rozwiązanie wykorzystuje tymczasowy plik docelowy),\n"
"No13 - usuwa wszystkie znaki CR (o kodzie 13),\n"
"Yes13 - dodaje znak CR (kod 13) przed każdym znakiem LF (kod 10),\n"
"hex - zamiana formatu binarnego na format monitora szesnastkowego,\n"
"unhex - zamiana formatu monitora szesnastkowego na format binarny,\n"
"unman - przetwarza dokumenty \"man\" na tekst ASCII,\n"
"pl f1 f2 - konwertuje standard polskich liter z \"f1\" na \"f2\",\n"
"cat - przepisuje tekst bez zmian (dla celów testowych).\n\n"
"   Program czyta dane ze standardowego wejścia, chyba że jako pierwszą\n"
"opcję podano \"i\" lub \"io\" - wtedy dane są czytane ze podanego pliku.\n"
"   Dane są wypisywane na standardowym wyjściu, chyba że podano na końcu\n"
"opcję \"o\" lub na początku była opcja \"io\" - wtedy dane są zapisywane\n"
"do podanego pliku.\n"
"   Przykład użycia programu:\n"
"                  al i plik1 No13 pl iso a o plik2\n"
"Ten przykład usunie znaki CR, skonwertuje standard polskich\n"
"liter ISOLatin2->AmigaPL i zapisze dane do pliku \"plik2\".\n"
"   Podaj nazwę którejś opcji, aby się dowiedzieć czegoś więcej na ten temat."
  );
 }
 else /* Mamy więcej niż 1 parametr - nazwę interesującej opcji */
 {
  printf("Opis funkcji \"%s\":\n\n", argv[2]);
  if( ! CaseStrcmp(argv[2], "i"))
  {
   puts(
"   Opcja \"i\" wymaga podania nazwy pliku, z którego będą\n"
"wczytywane dane do szeregu procesów. Ta opcja musi być\n"
"używana jako pierwsze ogniwo łańcucha procesów przetwarzających."
   );
  }
  else if( ! CaseStrcmp(argv[2], "o"))
  {
   puts(
"   Opcja \"o\" wymaga podania nazwy pliku, do którego będą zapisywane\n"
"dane generowane przez łańcuch procesów. Ta opcja musi być użyta jako\n"
"ostatnie ogniwo w łańcuchu procesów."
   );
  }
  else if( ! CaseStrcmp(argv[2], "io"))
  {
   puts(
"   Opcja \"io\" wymaga podania nazwy pliku, który będzie przetwarzany.\n"
"Dane są czytane z podanego pliku, przetwarzane i zapisywane w tymczasowym\n"
"pliku. Następnie zawartość pliku tymczasowego jest kopiowana z powrotem\n"
"do podanego pliku, a następnie plik tymczasowy jest kasowany. Ta opcja\n"
"musi być użyta jako pierwsze ogniwo w łańcuchu procesów i nie może\n"
"być używana z opcjami \"i\" i \"o\"."
   );
  }
  else if( ! CaseStrcmp(argv[2], "No13"))
  {
   puts(
"   Opcja \"No13\" powoduje usunięcie znaków CR (Carriage Return\n"
"- o kodzie 13), przez co następuje konwersja końców linii\n"
"ze standardu MS-DOS na standard UNIX/Amiga."
   );
  }
  else if( ! CaseStrcmp(argv[2], "Yes13"))
  {
   puts(
"   Opcja \"Yes13\" powoduje dołożenie przed każdym znakiem LF (Line Feed\n"
"- o kodzie 10) znaku CR (Carriage Return - o kodzie 13), przez co\n"
"następuje konwersja znaków końców linii ze standardu UNIX/Amiga\n"
"na standard MS-DOS."
   );
  }
  else if( ! CaseStrcmp(argv[2], "hex"))
  {
   puts(
"   Opcja \"hex\" powoduje wyświetlenie danych w postaci liczb\n"
"szesnastkowych oraz w postaci znaków ASCII (jeśli znaki\n"
"wejściowe nie mają graficznej reprezentacji w ASCII, jest\n"
"drukowana \".\"). Każda linijka rozpoczyna się od szesnastkowej liczby\n"
"określającej aktualnego numer bajtu od początku pliku."
   );
  }
  else if( ! CaseStrcmp(argv[2], "unhex"))
  {
   puts(
"   Opcja \"unhex\" przyjmuje dane wygenerowane przez \"hex\" i odtwarza\n"
"bajty przez zdekodowanie kolejnych liczb zapisanych w kodzie\n"
"szesnastkowym. Pozwala to na dokonywanie poprawek w pliku binarnym\n"
"przy użyciu zwykłego edytora."
   );
  }
  else if( ! CaseStrcmp(argv[2], "unman"))
  {
   puts(
"   Opcja \"unman\" powoduje przekształcenie formatu plików wygenerowanych\n"
"przez polecenie \"man\" w systemie UNIX na zwykłe pliki ASCII. Jest to\n"
"dokonywane przez interpretację kodów BS (BackSpace - kod 8) i usuwanie\n"
"znaków znajdujących się bezpośrednio przed kodami BS."
   );
  }
  else if( ! CaseStrcmp(argv[2], "pl"))
  {
   puts(
"   Opcja \"pl\" pozwala na konwersję standardów polskich znaków i wymaga\n"
"podania jako parametrów identyfikatorów dwóch standardów, źródłowego\n"
"i docelowego. Można podać:\n"
" l2 - IBM Latin 2 (Code Page 852),\n"
" 1250 - Code Page 1250 (stosowany w MS Windows),\n"
" m - Mazovia,\n"
" a - AmigaPL,\n"
" iso - ISO Latin 2 (ISO 8859-2), stosowany w sieci Internet,\n"
" tex - standard polskich liter z wykorzystaniem znaku \"/\" (slash), czyli\n"
"konwencji \"ciachowej\". \n"
" mex - standard polskich liter stosowanych w MeX/PL (polska wersja TeX'a).\n"
" ascii - zwykły plik ASCII, są usuwane kody polskich liter.\n"
"Nie można podawać dwóch identycznych formatów, format ASCII nie może\n"
"być formatem źródłowym."
   );
  }
  else if( ! CaseStrcmp(argv[2], "cat"))
  {
   puts(
"   Opcja \"cat\" jest funkcją testową - jej działanie polega na\n"
"drukowaniu na wyjściu tego, co zostało wczytane z wejścia."
   );
  }
  else
  {
   printf("Opcja \"%s\" nie jest znana lub jest niezaimplementowana.\n",
          argv[2]);
  }
 }
}

/******************************************************************************/

/* Funkcja decyduje, czy użtkownik chce informacji na temat programu */
int WantHelp(char * s)
{
 int status = 0;
 if( ! CaseStrcmp(s, "-h") ||
     ! CaseStrcmp(s, "--help") ||
     ! CaseStrcmp(s, "/h") ||
     ! CaseStrcmp(s, "/?") ||
     ! CaseStrcmp(s, "-?") ||
     ! CaseStrcmp(s, "help") ||
     ! CaseStrcmp(s, ""))
 {
  status = 1; /* Użytkownik czegoś chce się dowiedzieć */
 }
 return status;
}

/******************************************************************************/

int main(int argc, char * argv[])
{
 /* Na początku musimy dokonać przetworzenia parametrów */
 if(argc > 1) /* Wymagamy jakichkolwiek parametrów do poprawnej pracy */
 {
  if( WantHelp(argv[1])) /* Sprawdź, czy użytkownik nie potrzebuje pomocy */
  {
   ShowHelp(argc, argv);
  }
  else /* Nie chcemy pomocy - od razu wyświetl pomoc */
  {
   if(CreateChainLogic(argc - 1, argv + 1))
   {
#if DEBUG
    SygError("DEBUG::: Zaczynam pracę...");
#endif
    TopMember.RunProcesses();
#if DEBUG
    SygError("DEBUG::: Kończę pracę.");
#endif
   }
   else
   {
    SygError("Kończę z powodu błędu przetwarzania.");
   }
  }
 }
 else
 {
  SygError("Brak parametrów - nie wiem, co mam robić!");
 }
 return 0;
}

/******************************************************************************/
/*
 Struktura plików:
 typy.h - główne definicje i dodatki plików nagłówkowych
 al.cpp - główny moduł programu
 bufx.cpp, bufx.h - klasa buforów
 procx.cpp, procx.h - główna klasa procesów
 logx.cpp. logx.h - klasa logiki tworząca łańcuch procesów
 masterx.cpp, masterx.h - klasa związana z globalnymi zmiennymi
 inoutx.cpp, inoutx.h - klasa zajmująca się czytaniem/pisaniem do pliku

Realizowane funkcje w ramach "al":
i <nazwa pliku> - wczytuje dane z pliku
o <nazwa pliku> - zapis danych do pliku
no13 - obcina CR
yes13 - dodaje CR
identpl - sprawdza polskie litery
checkpl <std> - sprawdza, czy plik jest ASCII + <std>
pl <from> <to> - konwersja z standardu na standard
unman - konwersja według unman
compr10 - kompresja Enterów



Standardy:
a - AmigaPL
ascii - ASCII
l2 - IBM Latin 2 (CodePage 852)
m - Mazovia
1250 - CodePage 1250 (Windows)
TeX - Konwencja z TeX'a - z "/"

*/
/******************************************************************************/
