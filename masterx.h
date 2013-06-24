#ifndef _MASTERX_H
#define _MASTERX_H

#include "typy.h"
#include "procx.h"

class MasterClass
{
 ProcessClass * HeadPtr;
 char * NazwaPlikuTymczasowego;
 BufClass * AddBuffer(void);
 ProcessClass * FindLastProc(void);

public:
 char * NazwaPlikuIO;
 MasterClass(void);
 ~MasterClass(void);
 int AddProcess(ProcessClass *);
 void RunProcesses(void);
};

#endif
