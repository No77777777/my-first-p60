/*
 * Copyright (c) 2012-2013, 2020 QUALCOMM Technologies Inc. All Rights Reserved.
 * Qualcomm Technologies Confidential and Proprietary
 *
 */
#ifndef VERIFY_H
#define VERIFY_H

#include <string.h>

//Debug logging
#define DEBUG_MSG_MAX_LEN 32

#define FS_GATEWAY_DEBUG(a,b) fs_gateway_debug_log((int)(a),(char*)(b))

#define FS_GATEWAY_TABLE_MAX_INDEX 15        // Must be one less than a power of two;


#define FS_GATEWAY_MEM_ALLOC (-39)
#define FS_GATEWAY_THREAD_CREAT (-40)
#define FS_GATEWAY_MEM_ALLOC_SERVREG_CB (-41)
#define FS_GATEWAY_MEM_ALLOC_FSTAT_BUF (-42)
#define FS_GATEWAY_DALTIMER_ATTACH_FAIL (-43)
#define FS_GATEWAY_WLAN_IMG_NOT_FOUND (-44)
#define FS_GATEWAY_RFS_SERVER_ERR (-45)


typedef struct fs_gateway_debug {
   unsigned long long pcycle;
   int err;
   char msg[DEBUG_MSG_MAX_LEN]; //failed msg in some cases
}fs_gateway_debug_info_t;

extern fs_gateway_debug_info_t FS_GATEWAY_DEBUG_TABLE[FS_GATEWAY_TABLE_MAX_INDEX+1];

extern unsigned volatile fs_gateway_debug_index;

static inline void fs_gateway_debug_log(int a, char* b)
{
   unsigned long long pcycle;
   int nj;
   int j;

   asm volatile("   %3 = c31:30\n"
                "1: %0 = memw_locked(%4)\n"
                "   %1 = add(%0,#1)\n"
                "   %1 = and(%1,%5)\n"
                "   memw_locked(%4, p0) = %1\n"
                "   if (!p0) jump 1b\n"
                : "=&r" (j) , "=&r" (nj), "+m" (fs_gateway_debug_index), "=&r" (pcycle)
                : "r" (&fs_gateway_debug_index), "r" (FS_GATEWAY_TABLE_MAX_INDEX)
                : "p0");

   FS_GATEWAY_DEBUG_TABLE[j].pcycle = pcycle;
   FS_GATEWAY_DEBUG_TABLE[j].err = a;  //err code
   strlcpy(FS_GATEWAY_DEBUG_TABLE[j].msg , b , DEBUG_MSG_MAX_LEN); //err msg   
}

#define CATCH(exception) exception##bail: if (exception != 0)

#define CATCHC(exception, errcode) exception##bail: if (exception != 0)

#define VERIFY(exception, val) \
  do { \
    if(0 == (val)) { \
      exception = exception == 0 ? -1 : exception; \
	  FS_GATEWAY_DEBUG(exception , "Assertion " #val " failed" ); \
      goto exception##bail; \
    } \
  } while (0)

#define VERIFYC(exception, val, errcode) \
  do { \
    if(0 == (val)) { \
      exception = exception == 0 ? errcode : exception; \
      FS_GATEWAY_DEBUG(errcode , "Assertion " #val " failed"); \
      goto exception##bail; \
    } \
  } while (0)

#endif //VERIFY_H

