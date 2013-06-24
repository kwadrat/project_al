#include "masterx.h"

MasterClass::MasterClass(void)
{
 HeadPtr = NULL;
 NazwaPlikuIO = NULL; /* Inna wartość oznacza, że pracujemy w trybie "io" */
 NazwaPlikuTymczasowego = NULL; /* Tu nazwa pliku "tmp" dla trybu "io" */
}

/******************************************************************************/

MasterClass::~MasterClass(void)
{

}

/******************************************************************************/

/* Znajduje wskaźnik do ostatniego procesu w łańcuchu */
ProcessClass * MasterClass::FindLastProc(void)
{
 ProcessClass * proc;
 BufClass * buf;
 proc = HeadPtr;
 if(proc != NULL)
 {
  while((buf = proc->NextBuf) != NULL)
  {
   if((proc = buf->NextProcess) == NULL)
   {
    SygError("W łańcuchu po buforze nie ma procesu.");
    break;
   }
  }
 }
 return proc;
}

/******************************************************************************/

/* Tworzy i dodaje na końcu łańcucha bufor */
/* Wartość zwrotna: NULL - wystąpił błąd, != NULL - wskaźnik do utworzonego
   bufora */
BufClass * MasterClass::AddBuffer(void)
{
 BufClass * buf;
 ProcessClass * proc;

 buf = new BufClass;
 if(buf != NULL)
 {
  if(HeadPtr != NULL)
  {
   proc = FindLastProc();
   if(proc != NULL)
   {
    buf->PrevProcess = proc;
    proc->NextBuf = buf;
   }
   else /* Kłopoty przy znajdowaniu ostatniego procesu w łańcuchu */
   {
    SygError("Błąd podczas szukania ostatniego procesu w łańcuchu.");
    buf = NULL;
   }
  }
  else
  {
   SygError("Nie mogę dodać bufora do pustego łańcucha procesów.");
   buf = NULL;
  }
 }
 else
 {
  SygError("Błąd tworzenia obiektu BufClass.");
 }
 return buf;
}

/******************************************************************************/

/* Dopisuje do tablicy procesów kolejny proces */
/* Zwraca 1 - OK, 0 - błąd */
int MasterClass::AddProcess(ProcessClass * proc)
{
 int status;
 BufClass * buf;

 status = 0; /* Zakładamy, że się pojawi błąd */
 if(HeadPtr == NULL)
 {
  HeadPtr = proc;
  status = 1;
 }
 else
 {
  if((buf = AddBuffer()) != NULL)
  {
   buf->NextProcess = proc;
   proc->PrevBuf = buf;
   status = 1; /* Tu już wszystko w porządku */
  }
  else
  {
   SygError("Błąd dodania bufora do łańcucha procesów");
  }
 }
 return status;
}

/******************************************************************************/

void MasterClass::RunProcesses(void)
{
 if(HeadPtr != NULL)
 {
  HeadPtr->Work();
 }
 else
 {
  SygError("Brak procesów w ciągu");
 }
}

/******************************************************************************/

