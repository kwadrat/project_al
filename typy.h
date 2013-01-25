#ifndef _TYPY_H
#define _TYPY_H

#include <stdio.h>

#define DEBUG 0 /* Brak wykrywania błędów */


/* Domyślna wielkość bufora - Amiga/SAS C nie lubi buforów > 512 bajtów */
#define BUF_DEFSIZE 512
#define STATE_ON 1
#define STATE_ERROR -1


#define SygError(x) fprintf(stderr, "!!! Plik \"%s\" Linia %d, \"%s\"\n",\
                             __FILE__, __LINE__, (x))



typedef unsigned char Byte;

#endif
