#ifndef _PROCX_H
#define _PROCX_H

class ProcessClass;

#include "typy.h"
#include "bufx.h"

class ProcessClass
{
protected:
 int State;

public:
 BufClass * PrevBuf;
 BufClass * NextBuf;
 
 virtual void Work(void);  /* Pompa do wtłaczania danych w potok */
 virtual int Init(int, char *[]); /* Tu początek danych */
 void SignalEndOfData(void); /* Proces kończy działanie */
 int StateEOF(void);
 ProcessClass(void);
 ~ProcessClass(void);
};


#endif
