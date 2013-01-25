#ifndef _CAT_H
#define _CAT_H

#include "procx.h"
class CatClass : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 virtual int Init(int, char *[]);
 virtual void Work(void);
};

class Yes13Class : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 virtual int Init(int, char *[]);
 virtual void Work(void);
};

class No13Class : public ProcessClass
{
 Byte LocalBuf[BUF_DEFSIZE];
 virtual int Init(int, char *[]);
 virtual void Work(void);
};


#endif
