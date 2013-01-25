#ifndef _PLX_H
#define _PLX_H

#include "procx.h"

class PlClass : public ProcessClass
{
 int SrcCode, DstCode; /* Kody standardów źródłowych i docelowych */
 int Slash; /* Znacznik, czy był znak "/" - przy konwersji z TeX'a */
 Byte LocalBuf[BUF_DEFSIZE];
 Byte TransTab[256]; /* Tabela translacji */
 virtual int Init(int, char *[]);
 virtual void Work();
};

#endif
