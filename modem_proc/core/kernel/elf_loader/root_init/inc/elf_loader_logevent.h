/*#============================================================================
#  Name:                                                                     
#    elf_loader_logevent.h 
#
#  Description:                                                              
#    PD related info used by elf loader/dynamic loader to log the failure scenarios. 
#                                                                            
# Copyright (c) 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.        
#============================================================================
*/

#ifndef ELF_LOADER_LOGEVENT_H
#define ELF_LOADER_LOGEVENT_H


#define DEBUG_MSG_MAX_LEN 128

#define ELF_LOADER_DEBUG(a,b,c) ELF_LOADER_DEBUG_TABLE_entry((int)(a),(int)(b),(char*)(c))

#define ELF_LOADER_TABLE_MAX_INDEX 63     // Must be one less than a power of two; with this value, we use 4K of debug buffer space

//for mode//
#define ERR_MODE_ELF_LOADER 1
#define ERR_MODE_FS_GATEWAY 2
#define ERR_MODE_LIBDL 3

#define ERR_ELF_LOADER_MEM_ALLOC (-39)
#define ERR_ELF_LOADER_THREAD_CREAT (-40)
#define ERR_ELF_LOADER_MEM_ALLOC_SERVREG_CB (-41)
#define ERR_ELF_LOADER_MEM_ALLOC_FSTAT_BUF (-42)
#define ERR_ELF_LOADER_DALTIMER_ATTACH_FAIL (-43)
#define ERR_ELF_LOADER_WLAN_IMG_NOT_FOUND (-44)
#define ERR_ELF_LOADER_RFS_SERVER_ERR (-45)


typedef struct elf_loader_debug {
   unsigned long long pcycle;
   unsigned char mode;
   int err;
   char msg[DEBUG_MSG_MAX_LEN]; //failed msg in some cases
}elf_loader_debug_info_t;

extern elf_loader_debug_info_t ELF_LOADER_DEBUG_TABLE[ELF_LOADER_TABLE_MAX_INDEX+1];

extern unsigned volatile elf_loader_debug_index;

static inline void ELF_LOADER_DEBUG_TABLE_entry(int a, int b, char* c)
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
                : "=&r" (j) , "=&r" (nj), "+m" (elf_loader_debug_index), "=&r" (pcycle)
                : "r" (&elf_loader_debug_index), "r" (ELF_LOADER_TABLE_MAX_INDEX)
                : "p0");

   ELF_LOADER_DEBUG_TABLE[j].pcycle = pcycle;
   ELF_LOADER_DEBUG_TABLE[j].mode = a;  //mode
   ELF_LOADER_DEBUG_TABLE[j].err = b;  //err code
   strlcpy(ELF_LOADER_DEBUG_TABLE[j].msg , c , DEBUG_MSG_MAX_LEN); //err msg   
}

#endif