#ifndef _SS_MINIDUMP_H
#define _SS_MINIDUMP_H

/*=============================================================================
                        MINIDUMP driver source code

GENERAL DESCRIPTION
  This header file contains the subsystem minidump specific APIs.

    Copyright 2017  - 2018 by QUALCOMM Technologies, Inc.  All Rights Reserved.
=============================================================================*/

/*=============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who          what, where, why
--------   ---          --------------------------------------------------
23/01/18     MarthaM       Moved stubbed APIs to products stub source file.
16/12/17     MarthaM       Initial revision for sdm670

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "qurt_types.h"
#include "DALSysTypes.h"
#include "DALSys.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#define MD_GLOBAL_MDTOC_PTR_OFFSET       (0xB88)
#define MD_GLOBAL_MDTOC_SIMEM_ADDR 	     (IMEM_BASE + MD_GLOBAL_MDTOC_PTR_OFFSET)

#define MD_SS_STATUS_SLEEP       (0xffff0001)

/*===========================================================================

FUNCTION  md_ss_check_if_in_minidump_mode

DESCRIPTION
This function shall return a boolean indicating current execution mode: cold boot or  minidump mode.

This API is mainly used by the CLOCKs,CRYPTO and BAM drivers to avoid calling/stubbing unsupported APIs while
running in Minidump Mode. 
Note DAL, QURT, Mutex and CLOCK APIs..etc does not work in Minidump Mode.

DEPENDENCIES
  None.
 
PARAMETERS
  None
 
RETURN VALUE
   TRUE - If running in minidump mode.
   FALSE - If running in cold boot mode.
    
SIDE EFFECTS
  None.

==============================================================================*/
boolean md_ss_check_if_in_minidump_mode(void);

/*===========================================================================

FUNCTION  md_ss_is_minidump_enabled

DESCRIPTION
This function shall return the subsystem minidump enable or disable status.

DEPENDENCIES
  None.
 
PARAMETERS
  None
 
RETURN VALUE
   TRUE-Secure Minidump enabled.
   FALSE-Secure Minidump disabled.
    
SIDE EFFECTS
  None.

==============================================================================*/
boolean md_ss_is_minidump_enabled(void);

/*===========================================================================

FUNCTION  md_ss_enable_abnormal_reset_debug_path

DESCRIPTION
This function shall enable the subsystem abnormal reset debug by setting bit RMB_SHADOW3[0]=1.
So that after Q6 debug reset by PIL(SSR) or SDI(Soc reset) we enter into minidump path.

This API called by SLEEP driver while sleep exit (for cases which it woke from PBL).

MSS Minidump Flow: 
(PBL)->(Subsystem Image Start Address RMB_SHADOW3[31-1])->(QURT Abnormal Reset Handler)->
(Minidump Bridge Entry Handler)->(Minidump Debug Reset Main)
                     

DEPENDENCIES
  None.
 
PARAMETERS
  None
 
RETURN VALUE
   None
    
SIDE EFFECTS
  None.

==============================================================================*/
void md_ss_enable_abnormal_reset_debug_path(void);

/*===========================================================================

FUNCTION  md_ss_disable_abnormal_reset_debug_path

DESCRIPTION
This function shall disable the subsystem abnormal reset debug by setting bit RMB_SHADOW3[0]=0.
So that after Q6 debug reset by PIL(SSR)/SDI(Soc reset) we won't enter minidump path.

This API called by SLEEP driver while sleep entry(for cases which it woke from PBL).

Minidump Flow: 
(PBL)->(Subsystem Image Start Address)->(QURT Abnormal Reset Handler)->
(Minidump Bridge Entry Handler)->(Minidump Debug Reset Main)
                     

DEPENDENCIES
  None.
 
PARAMETERS
  None
 
RETURN VALUE
   None
    
SIDE EFFECTS
  None.

==============================================================================*/

void md_ss_disable_abnormal_reset_debug_path(void);

/*===========================================================================

FUNCTION  md_ss_malloc

DESCRIPTION
This function allocates the memory of requested size from Minidump Heap Memory,
and returns allocated address with 8-byte aligned.
8-byte alignment is the basic requirement for BAM/CRYPTO operations in minidump mode.

This API should be used only in minidump mode.

DEPENDENCIES
  None.
 
PARAMETERS
    size - Size of memory to be allocate in bytes.
 
RETURN VALUE
   void * - Pointer to allocated memory.
    FALSE - If allocation faild, or NO memory.
    
SIDE EFFECTS
  None.

===========================================================================*/


void * md_ss_malloc(uint32 size);

/*===========================================================================

FUNCTION  md_ss_free

DESCRIPTION
This function frees the memory chunk allocated in Minidump Heap.
The status flag is_available in memory_chunk pointer set to FREE.
This API should be used only in minidump mode.

DEPENDENCIES
  None.
 
PARAMETERS
    void* - Pointer to be freed.
 
RETURN VALUE
   None
    
SIDE EFFECTS
  Pointer must be allocated from Minidump heap.

===========================================================================*/

void md_ss_free(void *pmem);

/*===========================================================================

FUNCTION  md_ss_heap_get_paddr

DESCRIPTION
This function returns the physical address of allocated buffer in Minidump Heap.
it should be called in minidump mode only.


DEPENDENCIES
  None.
 
PARAMETERS
    void* - buffer pointer allocated in minidump heap.
 
RETURN VALUE
   void * - physical address of buffer pointer.
    
SIDE EFFECTS
  None.

===========================================================================*/
void *md_ss_heap_get_paddr(void* vaddr);

void md_ss_memorybarrier(void);

int md_ss_map_memory_range
( 
	uint32 region_paddr, 
	uint32 region_vaddr, 
	uint32 region_size, 
    qurt_mem_cache_mode_t cache_attribs,
    unsigned int *entry, 
    uint8 remap, 
    uint32 *vadd_out_param 
);
							
/**Minidump specific DAL APIs. Supported only in minidump mode.*/
DALResult md_ss_DALSYS_MemRegionAlloc
(
	uint32 dwAttribs,
	DALSYSMemAddr VirtualAddr,
    DALSYSMemAddr PhysicalAddr, 
    uint32 dwLen, 
    DALSYSMemHandle *phMem,
    DALSYSMemObj *pObj
);

DALResult md_ss_DALSYS_MemInfo(DALSYSMemHandle hMem, DALSYSMemInfo *pMemInfo);
#endif /* _SS_MINIDUMP_H */
