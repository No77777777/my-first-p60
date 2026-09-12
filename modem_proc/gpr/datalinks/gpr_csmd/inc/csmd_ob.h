/*========================================================================*/
/**
@file csmd_ob.h

@brief Shared Memory API: This file contains the prototypes of the functions/APIs exposed by the
CSMD to the GPR client and are primarily invoked during out of band transfers.
*/
/*========================================================================
Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */
/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\inc\csmd_ob.h

when       who        what, where, why
--------   ---       --------------------------------------------------
05/04/18   abangnar      Created file.
========================================================================== */
#ifndef CSMD_OB_H
#define CSMD_OB_H
/* =======================================================================
INCLUDE FILES
========================================================================== */
#include "ar_osal_types.h"
#include "ar_osal_error.h"
#include "ar_osal_heap.h"
#include "ar_types.h"
/*--------------------------------------------------------------*/
/* Type Declarations                                          */
/* -------------------------------------------------------------*/
#ifndef WINSOCK_TRANSPORT
typedef struct memorymap_shm_region
{
   uint32_t shm_addr_lsw;
   /**< Lower 32 bits of the shared memory address of the memory region to
        map. */

   uint32_t shm_addr_msw;
   /**< Upper 32 bits of the shared memory address of the memory region to
        map.

        The 64-bit number formed by shm_addr_lsw and shm_addr_msw word must be
        contiguous memory, and it must be 4 KB aligned.

        @values
        - For a 32-bit shared memory address, this field must be set to 0.
        - For a 36-bit shared memory address, bits 31 to 4 must be set to 0.
        - For a 64-bit shared memory address, any 32 bit value.
        @tablebulletend */

   uint32_t mem_size;
   /**< Size of the shared memory region.

        Number of bytes in the shared memory region.

        @values Multiples of 4 KB

        The aDSP always maps the regions as virtual contiguous memory, but
        the memory size must be in multiples of 4 KB to avoid gaps in the
        virtually contiguous mapped memory. @newpagetable */

} memorymap_shm_region_t;

/* LL Node Information structure declaration*/
struct mem_map_node
{
   uint32_t     index;      /*<Indexes the node starting with num_regions -1 down to 0 */
   uint32_t     addr_remote; /*<OB Chunk start address on the remote host*/
   void *       ptr_local;  /*<OB Chunk start address on the local_host*/
   uint32_t     size;       /*<Size of the OB memory chunk*/
   bool_t       is_offset_map; /*<to indicate if the mapping  will be queried by offset later*/
   bool_t       is_va_map; /*<to indicate if the mapping is virtual (or physical)*/
   struct mem_map_node *next;       /*<Pointer to the next node in the LL*/
};

typedef struct mem_map_node mem_map_node_t;

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/
/*----------------------------------------------------------------------------
* Function Prototypes
* ------------------------------------------------------------------------- */
/*to allocate the mirror memory and map the local pointer with the
      remote pointer and maintain a list with this info*/
uint32_t csmd_malloc_map_region(uint32_t num_regions, void *regions, bool_t is_offset_map, bool_t is_va_map);
/*to compute and return the virtual address to be accessed in the local domain*/
uint32_t csmd_get_virt_addr(uint32_t *handle, void *ptr1_remote);
/*to emulate the OB memory of the remote as a mirror on the local domain*/
ar_result_t csmd_mem_invalidate(uint32_t virt_addr, uint32_t mem_size);
/*to emulate the OB memory on the remote as a mirror of the local domain*/
ar_result_t csmd_mem_flush(uint32_t virt_addr, uint32_t mem_size);
/*to free the mirror memory and the associated book keepting records*/
ar_result_t csmd_free_unmap_region(uint32_t *handle);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  // WINSOCK_TRANSPORT
/*to send the requested portion of data from or receive an update to OB mem the remote client*/
ar_result_t honour_ob_req(void *recv_buf, uint32_t recv_size);
#endif // WINSOCK_TRANSPORT
#endif // CSMD_OB_H
