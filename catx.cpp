#ifndef _CATX_CPP
#define _CATX_CPP

#include "catx.h"

int CatClass::Init(int, char *[])
{
 return 1;
}

/******************************************************************************/

void CatClass::Work(void)
{
 int ile;
 while((ile = PrevBuf->GetByteArea(LocalBuf, BUF_DEFSIZE)) > 0)
 {
  NextBuf->PutByteArea(LocalBuf, ile);
 }
 if(ile == EOF)
 {
  SignalEndOfData();
 }
}

/******************************************************************************/

int Yes13Class::Init(int, char *[])
{
 return 1;
}

/******************************************************************************/

void Yes13Class::Work(void)
{
 int ile;
 int i;
 Byte * src, * dst;
 while((ile = PrevBuf->GetByteArea(LocalBuf, BUF_DEFSIZE >> 1)) > 0)
 {
  src = LocalBuf + ile;
  dst = LocalBuf + BUF_DEFSIZE;
  for(i = 0; i < ile; i++)
  {
   if((*--dst = *--src) == '\n')
   {
    *--dst = '\r'; /* Dodanie kodu 13 */
   }
  }
  NextBuf->PutByteArea(dst, LocalBuf + BUF_DEFSIZE - dst);
 }
 if(ile == EOF)
 {
  SignalEndOfData();
 }
}

/******************************************************************************/

int No13Class::Init(int, char *[])
{
 return 1;
}

/******************************************************************************/

void No13Class::Work(void)
{
 int ile, i;
 Byte * src, * dst;
 Byte tmp;

 while((ile = PrevBuf->GetByteArea(LocalBuf, BUF_DEFSIZE)) > 0)
 {
  src = dst = LocalBuf;
  for(i = 0; i < ile; i++)
  {
   if((tmp = *src++) != '\r')
   {
    *dst++ = tmp;
   }
  }
  NextBuf->PutByteArea(LocalBuf, dst - LocalBuf);
 }
 if(ile == EOF)
 {
  SignalEndOfData();
 }
}

#endif
