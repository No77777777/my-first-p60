/**
  @file mcfg_mmu.h

  @brief  Definitions used in mcfg_mmu API.
*/
/*
  Copyright (c) 2013 Qualcomm Technologies Incorporated.
          All Rights Reserved.
    Qualcomm Confidential and Proprietary
*/

/*===========================================================================
                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/api/mcfg_mmu.h#1 $
  $DateTime: 2021/04/08 00:04:19 $


  when        who     what, where, why
  ---------   ---     ---------------------------------------------------------------------------
  06/01/17    skotalwa CR2055351 Framework for SBL reclaim
  04/28/15    hzhi    CR 827861: Added support for client to insert new entry into mcfg_mmu_table[] for fast VA<-->PA look up.
  07/25/14    skotalwa Updated for CR 699655: optimize MMU_table lookup API to fix MPPS performance hit.
  06/10/14    hzhi    Created for CR 670990: removing unnecessary dependency of mmu update on segment loading feature.

===========================================================================*/

#ifndef MCFG_MMU_H
#define MCFG_MMU_H

#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#include "ref_cnt_obj.h"
#endif
#include "comdef.h"
#include "IxErrno.h"

#include "qurt_memory.h"

/*------------------------------------------------------------------*
 * symbols defined to represent main memory region VA and PA start. * 
 *------------------------------------------------------------------*/
extern unsigned int image_pstart __attribute__((section(".data")));
extern unsigned int image_vstart __attribute__((section(".data")));

/*------------------------------------------------------------------*
 * local data types to manage mcfg MMU table to translate        *
 * heap section VA to PA upon query.                                *
 *------------------------------------------------------------------*/
#define MCFG_MMU_ENTRY_MAX_NUMBER 32
#define MCFG_MMU_VA_STEP_SIZE (256*1024)

/*-----------------------------------------------------------------------------
  Definition of return types for mcfg_mmu_update functions. 
-----------------------------------------------------------------------------*/
typedef enum
{
  MCFG_MMU_UPDATE_SUCCESS           = 0,
  MCFG_MMU_UPDATE_TRANSLATION_FAIL  = 1,
  MCFG_MMU_UPDATE_ENTRY_OVERFLOW_FAIL = 2,
  MCFG_MMU_UPDATE_MALLOC_FAIL = 3
}mcfg_mmu_update_status_t;
/*-----------------------------------------------------------------------------
  Definition of return types for FS3 RECLAIM. 
-----------------------------------------------------------------------------*/
#ifdef FEATURE_FS3_RECLAIM
typedef enum
{
  MCFG_FS3_EFS_SYNC_SUCCESS                         = 0,
  MCFG_FS3_XPU_LOCK_FAILED                          = 1,
  MCFG_FS3_QURT_POOL_CREATION_FAILED                = 2,
  MCFG_FS3_QURT_MEM_REGION_CREATION_FAILED          = 3,
  MCFG_FS3_RECLAIM_INIT                             = 4
}mcfg_FS3_reclaim_status_t;
#endif
/*-----------------------------------------------------------------------------
  Definition of return types for SBL_MBA RECLAIM. 
-----------------------------------------------------------------------------*/
#ifdef FEATURE_SBL_MBA_RECLAIM
typedef enum
{
  MCFG_MBA_XPU_LOCK_FAILED                          = 0,
  MCFG_QURT_POOL_CREATION_FAILED                    = 1,
  MCFG_QURT_MEM_REGION_CREATION_FAILED              = 3,
  MCFG_RECLAIM_SUCCESS                              = 4,
  MCFG_RECLAIM_INIT                                 = 5
}mcfg_reclaim_status_t;
#endif
/*------------------------------------------------------------------*
 * mcfg_MMU_table entry data type.                               *
 *------------------------------------------------------------------*/
typedef struct
{
  qurt_addr_t va_start;
  qurt_addr_t va_end;
  qurt_paddr_t pa_start;
  qurt_paddr_t pa_end;
  unsigned int offset;
}mcfg_MMU_entry_t, *mcfg_MMU_entry_ptr_t;

/*------------------------------------------------------------------*
 * MMU table to manage address translation translate for heap       *
 * sections added from reclaimed memory region.                     *
 *------------------------------------------------------------------*/
extern mcfg_MMU_entry_t mcfg_MMU_table[MCFG_MMU_ENTRY_MAX_NUMBER];

/*------------------------------------------------------------------*
 * mcfg_mmu_table_last_entry points to last non-zero entry in MMU.  *
 *------------------------------------------------------------------*/
extern mcfg_MMU_entry_t *mcfg_mmu_table_last_entry;

/*===========================================================================

FUNCTION mcfg_VA_to_PA

DESCRIPTION
  This function returns accurate Physical Address of given Virtual Address 
  for heap sections added from reclaimed memory region. 
  *Usage is limited to address fails into heap and modem_mem region.*
  *Not including device I/O, smem, EFS simulation region.*

DEPENDENCIES
  None. 

RETURN VALUE
  Physical Address of input Virtual Address. 

SIDE EFFECTS
  None.

===========================================================================*/
static __inline qurt_paddr_t mcfg_VA_to_PA(qurt_addr_t va)
{
#if !defined(T_WINNT) && !defined(T_UNIX)
  mcfg_MMU_entry_ptr_t ptr = mcfg_mmu_table_last_entry;

  while(va < ptr->va_start || va > ptr->va_end)
  {
    ptr--;
  }
  return (qurt_paddr_t)(ptr->offset + va);
#else /*T_WINNT || T_UNIX*/
  return (qurt_paddr_t)va;
#endif /*T_WINNT || T_UNIX*/
}


/*===========================================================================

FUNCTION mcfg_PA_to_VA

DESCRIPTION
  This function returns accurate Virtual Address of given Physical Address 
  for heap sections added from reclaimed memory region. 
  *Usage is limited to address fails into heap and modem_mem region.*
  *Not including device I/O, smem, EFS simulation region.*

DEPENDENCIES
  None. 

RETURN VALUE
  Virtual Address of input Physical Address. 

SIDE EFFECTS
  None.

===========================================================================*/
static __inline qurt_addr_t mcfg_PA_to_VA(qurt_paddr_t pa)
{
#if !defined(T_WINNT) && !defined(T_UNIX)
  mcfg_MMU_entry_ptr_t ptr = mcfg_mmu_table_last_entry;

  while(pa < ptr->pa_start || pa > ptr->pa_end)
  {
    ptr--;
  }
  return (qurt_addr_t)(pa - ptr->offset);
#else /*T_WINNT || T_UNIX*/
  return (qurt_addr_t)pa;
#endif /*T_WINNT || T_UNIX*/
}


/*===========================================================================

FUNCTION mcfg_mmu_add_entry

DESCRIPTION
  This function allows user to request adding extra entry into mcfg_mmu_table.
  If requested entry is already covered by current table, no change will be 
  made; otherwise new table entry will be created.

DEPENDENCIES
  corebsp supports modem mem heap enumeration api modem_mem_get_section_info(). 

LIMITATIONS
  current version does not support adding mixed various-offset address ranges or
  address range smaller than one page, please make sure the v_start pssed in
  belong to the same contiguous memory chunk. 

RETURN VALUE
  0 - on success.
  1 - on error.

SIDE EFFECTS
  This API is NOT SAFE in multi-threading environment. This is intentional due 
  to performance concern. 
  Please use it with carefullness and do not add plenty small memory chunks 
  with this API. 

===========================================================================*/
int mcfg_mmu_add_entry(qurt_addr_t v_start, qurt_size_t v_size);

/*===========================================================================

FUNCTION mcfg_reclaim_mem_pool

DESCRIPTION
  This function reclaims MBA buffer during boot up and adds this memory into heap. 

DEPENDENCIES
  None. 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
#ifdef FEATURE_SHARED_BOOT_MODEM_MEMORY_RECLAIM
void mcfg_reclaim_mem_pool(void);
#endif

/*===========================================================================
FUNCTION mcfg_mmu_update

DESCRIPTION
  This function update mcfg_MMU_table[] after segment loading is done. 
  Every heap section is supposed to be added into this MMU table as 
  a separate entry, and for mcfg_VA_to_PA() and mcfg_PA_to_VA() api to
  query on. 
  
DEPENDENCIES
  corebsp supports modem mem heap enumeration api modem_mem_get_section_info(). 
  
RETURN VALUE
  MCFG_MMU_UPDATE_SUCCESS             --> mcfg_MMU_table[] updated.
  MCFG_MMU_UPDATE_TRANSLATION_FAIL    --> given VA cannot be translated to
                                          leagal PA by qurt_lookup_physaddr().
  MCFG_MMU_UPDATE_ENTRY_OVERFLOW_FAIL --> too many heap sections according to
                                          enumeration result. 
  MCFG_MMU_UPDATE_MALLOC_FAIL         --> cannot allocate enough memory for
                                          section_info[] ADT.

SIDE EFFECTS
  None.
===========================================================================*/
mcfg_mmu_update_status_t mcfg_mmu_update(void);

#endif /*MCFG_MMU_H*/
