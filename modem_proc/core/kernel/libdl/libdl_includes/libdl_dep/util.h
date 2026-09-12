/*=============================================================================

FILE:       util.h

SERVICES:   Utilities for for authenticating dynamic modules using RSA signatures and
            an X509 certificate chain embedded in the dynmod.

==============================================================================
        Copyright (c) 2013 Qualcomm Technologies Incorporated.
                   All Rights Reserved.
            QUALCOMM Proprietary and Confidential
==============================================================================*/
#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>
#include <memory.h>
#include "AEEstd.h"
#include "stringl.h"
#include "com_dtypes.h"

#define PL_SHA256_HASH_LEN    32

#define NSIZEOF(x)      ((int)sizeof(x))
#define ARRAY_SIZE(a)   ((int)(sizeof(a) / sizeof(a[0])))
#define CSTRLEN(s)      (NSIZEOF(s)-1)
#define ZEROAT(p)       PL_MEMSET(p, 0, sizeof(*p))

#define PL_MEMCPY          memcpy
#define PL_MEMSCPY         memscpy
#define PL_MEMCMP          memcmp
#define PL_MEMSET          memset
#define MALLOC          malloc
#define FREE            free

#define ERRMALLOC_REC(type,pp)   ErrMalloc(NSIZEOF(type),(void**)pp)


void DumpMem(void *pMem, int nSize);
void DumpMemEx(void *pMem, int nSize, boolean bShowAddress, boolean bShowAscii);


// guarantees zero initialization
static __inline int ErrMalloc(int size, void **pp)
{
   void *pMem = MALLOC(size);
   if (!pMem) {
      return -1;
   }
   PL_MEMSET(pMem, 0, size);
   *pp = pMem;
   return 0;
}


#define STD_MIN(a,b)   ((a)<(b)?(a):(b))


////////////////////////////////////////////////////////////////////////////////
// DMALLOC - debug malloc

void* __dmalloc(size_t size, const char *file, int line);
void __dfree(void *p, const char *file, int line);

#ifdef _DEBUG
#define DMALLOC(s)   __dmalloc((s),__FILENAME__,__LINE__)
#define DFREE(p)     __dfree((p),__FILENAME__,__LINE__)
#else
#define DMALLOC      malloc
#define DFREE        free
#endif


#endif   //__UTIL_H__
