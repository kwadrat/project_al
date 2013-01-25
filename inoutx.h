#ifndef _INOUTX_H
#define _INOUTX_H

#include "procx.h"

class ProcIClass : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 int Init(int, char *[]);
 void Work(void);
};

/******************************************************************************/

class ProcOClass : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 int Init(int, char *[]);
 void Work(void);
};

/******************************************************************************/

class ProcIFileClass : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 FILE * plik;
 int Init(int, char *[]);
 void Work(void);
};

/******************************************************************************/

class ProcOFileClass : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 FILE * plik;
 int Init(int, char *[]);
 void Work(void);
};

/******************************************************************************/

class ProcIOFileClass : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 FILE * plik;
 int Init(int, char *[]);
 size_t IleBajtow;
 void Work(void);
};

/******************************************************************************/

#endif
