#ifndef MEMHEAP_V_H
#define MEMHEAP_V_H
/**
  @file memheap_v.h
  @brief 
    Contains the heap manager internal interface.

*/
/*===========================================================================

  Copyright (c) 2014-2017 Qualcomm Technologies, Inc. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc.and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/utils/src/memheap_v.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/30/17   nk       Added Support to log multiple callers
11/08/16   nk       Fixed Integer overflow in memheap_lite sanity check macro
06/09/16   nk       Added check to avoid 16Byte Block creation in MBA image
02/11/16   nk       Changes to remove 16 byte blocks
09/08/15   ps       Changes to move amss and modem heap memory from static pools
04/07/14   rks      Initial Version
===========================================================================*/


/*=============================================================================

                           INCLUDE FILES

=============================================================================*/

#include "qurt.h"
#include "memheap.h"
#include <comdef.h> 
#include "tms_rcinit.h"

/*=============================================================================

                        MACRO DEFINITIONS

=============================================================================*/

#ifdef MEMHEAP_LOG_MULTIPLE_CALLERS 
#define MEMHEAP_CALLER_2 2
#define MEMHEAP_CALLER_3 3
#define MEMHEAP_CALLER_4 4
#define MEMHEAP_CALLER_5 5
#endif

#define OVERFLOW_CHECK(elt_count, elt_size) (!(elt_count >= (1U<<10) || elt_size >= (1U<<22)) || ((((uint64)elt_count * (uint64)elt_size) >> 32) == 0))

#define BOUNDARY_CHECK_SECTIONS(theBlock, heap_ptr) ((theBlock >= ((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)theBlock)->section_index].start_addr) && (theBlock < (mem_block_header_type *)((char*)(((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)theBlock)->section_index].start_addr) + ((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)theBlock)->section_index].size)))

#define FRD_OFFSET_CHECK(block, heap_ptr) (((((mem_block_header_type *)block)->forw_offset + (char *)block) > (char *)block)\
   && (((mem_block_header_type *)block)->forw_offset + (char *)block) <= ((((char*)((mem_heap_type*)heap_ptr)->first_block) + ((mem_heap_type*)heap_ptr)->total_bytes)))


#define FRD_OFFSET_CHECK_SECTIONS(block, heap_ptr) (((((mem_block_header_type *)block)->forw_offset + (char *)block) > (char *)block)\
   && ((((mem_block_header_type *)block)->forw_offset + (char *)block) <= ((((char*)((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *) (block))->section_index].start_addr) + ((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)(block))->section_index].size))))

#define FOOTER_FRD_OFFSET_CHECK_SECTIONS(block, heap_ptr, footer_offset) (((char *)block > ((char*)(((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)block)->section_index].start_addr)))\
   && (footer_offset <= ((char *)block - ((char*)(((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)block)->section_index].start_addr)))))

/* XOR based Guard byte calculations and restore */
#define INTEGRITY_CHECK_ON_USED_HEADER(magic_num_used, block) \
            (block[0]^block[1]^block[2]^block[3]^magic_num_used^((unsigned short)block)^((unsigned short)(((unsigned int)block)>> 16)))


#define INTEGRITY_CHECK_ON_FREE_HEADER(magic_num_free, block) \
            (block[0]^block[1]^block[2]^block[3]^block[4]^block[5]^magic_num_free^((unsigned short)block)^((unsigned short)(((unsigned int)block)>> 16)))


#define ADD_GUARD_BYTES_TO_USED_HEADER(magic_num_used, block) \
            (block[0] = block[1]^block[2]^block[3]^magic_num_used^((unsigned short)block)^((unsigned short)(((unsigned int)block)>> 16)))


#define ADD_GUARD_BYTES_TO_FREE_HEADER(magic_num_free, block) \
            (block[0] = block[1]^block[2]^block[3]^block[4]^block[5]^magic_num_free^((unsigned short)block)^((unsigned short)(((unsigned int)block)>> 16)))


#define MIN_HEAP_SIZE_FOR_BINS 0x20000 

#define MEMHEAP2_BINSIZE_OVERHEAD (sizeof(mem_block_header_type))
#define NEXT_BLOCK_SIZE(temp) ((mem_block_header_type*)(temp->nextPtr))->forw_offset

#ifdef FEATURE_MEM_DEBUG
#define MEMHEAP_MAX_EXTRA  31 // since it can be  (kMinBlockSize-1s)
#else
#define MEMHEAP_MAX_EXTRA  39 // since it can be  (kMinBlockSize - 1 +(kMinChunkSize - sizeof(mem_block_header_type)))
#endif

#define MEMHEAP2_BINSIZE0         (0x00000020)
#define MEMHEAP2_BINSIZE1         (0x00000030)
#define MEMHEAP2_BINSIZE2         (0x00000040)
#define MEMHEAP2_BINSIZE3         (0x00000050)
#define MEMHEAP2_BINSIZE4         (0x00000060)
#define MEMHEAP2_BINSIZE5         (0x00000080)
#define MEMHEAP2_BINSIZE6         (0x00000090)
#define MEMHEAP2_BINSIZE7         (0x000000C0)
#define MEMHEAP2_BINSIZE8         (0x000000E0)
#define MEMHEAP2_BINSIZE9         (0x00000100)
#define MEMHEAP2_BINSIZE10        (0x00000120)
#define MEMHEAP2_BINSIZE11        (0x00000150)
#define MEMHEAP2_BINSIZE12        (0x00000180)
#define MEMHEAP2_BINSIZE13        (0x00000200)
#define MEMHEAP2_BINSIZE14        (0x00000250)
#define MEMHEAP2_BINSIZE15        (0x00000300)
#define MEMHEAP2_BINSIZE16        (0x00000400)
#define MEMHEAP2_BINSIZE17        (0x00000600)
#define MEMHEAP2_BINSIZE18        (0x00000800)
#define MEMHEAP2_BINSIZE19        (0x00000C00)
#define MEMHEAP2_BINSIZE20        (0x00001000)
#define MEMHEAP2_BINSIZE21        (0x00001800)
#define MEMHEAP2_BINSIZE22        (0x00002000)
#define MEMHEAP2_BINSIZE23        (0x00003000)
#define MEMHEAP2_BINSIZE24        (0x00004000)
#define MEMHEAP2_BINSIZE25        (0x00006000)
#define MEMHEAP2_BINSIZE26        (0x00008000)
#define MEMHEAP2_BINSIZE27        (0x0000C000)
#define MEMHEAP2_BINSIZE28        (0x00010000)
#define MEMHEAP2_BINSIZE29        (0x00018000)
#define MEMHEAP2_BINSIZE30        (0x00020000)
#define MEMHEAP2_BINSIZE31        (0xFFFFFFFF)

typedef struct frd_Offset_info_type{
  unsigned long pad;
  unsigned long freeBlock_frdOff;
}frd_Offset_info;





#ifndef MEMHEAP_V_INFO_FREEBLOCKS
#define MEMHEAP_V_INFO_FREEBLOCKS  8
#endif

/* Structure used to log heap allocation failures */
typedef struct memheap_v_info_s
{
  struct mem_heap_struct *heap_ptr;
  size_t size;
  mem_heap_totals_type totals;
  unsigned int freeblocks[MEMHEAP_V_INFO_FREEBLOCKS];
} memheap_v_info_s_type;


/*=============================================================================

                             FUNCTION DEFINITIONS

=============================================================================*/


/** 
Create a heap region of requested size 

  @param[in] size         size of the region to be cearted .
  @param[in] mem_region   mem_region variable refrence

@return
returns the heap region address if successful or  NULL if failure.

@dependencies
None.
*/
   void* heap_create_region(uint32 size, qurt_mem_pool_t mem_pool, qurt_mem_region_t *mem_region);

/*===========================================================================
FUNCTION MEM_INIT_HEAP_ALLOC


DESCRIPTION
  This function is to be called when the client is not allocating the memory 
  for the heap and is expecting the heap memory to come from a pre-initialized
  heap region.After allocating a memory chunk this function calls mem_init_heap()

===========================================================================*/
void mem_init_heap_alloc(
   mem_heap_type                 *heap_ptr,
      /* Statically allocated heap structure
      */
   unsigned long                  heap_mem_size,
      /* The size in bytes of the memory pointed to by heap_mem_ptr
      */
   mem_allocator_failed_proc_type fail_fnc_ptr,
      /* Function to call when allocation fails, can be NULL
      */
   qurt_mem_pool_t                mem_pool
);


#endif /* MEMHEAP_V_H */
