/**
  @file heap_mem_reclaim.c

  @brief  
*/
/*
  Copyright (c) 2013 QUALCOMM Technologies Incorporated.
          All Rights Reserved.
    Qualcomm Confidential and Proprietary
*/

/*===========================================================================
                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/perf.mpss/4.4.2/heap_mgt/src/heap_mem_reclaim.c#1 $
  $DateTime: 2022/10/12 17:54:20 $


  when        who     what, where, why
  ---------   ---     ---------------------------------------------------------------------------
  04/21/2016   skotalwa Added generic api to reclaim memory into heap
  05/11/2016   skotalwa added new file
 
===========================================================================*/
#include "comdef.h"
#include "heap_mem_reclaim.h"
#include "memheap.h"


struct reclaim_dbg_buffer_t reclaim_dbg_buffer[12] ={0};
extern mem_heap_type modem_mem_heap;
static int debug_cnt = 0;


/********************************************************
 FUNCTION: reclaim_memory_to_heap(void* base_address, uint32 size)
 This API will reclaim memory into heap.
 
 @param[in]
  - start_address of the section.  
  - size of section to be reclaimed into heap.
 
 @return
  - return 0 on success else 1.
 ********************************************************/

int reclaim_memory_to_heap(void* base_address, uint32 size)
{
  
  uint32 reclaim_size;
  void* start_address;
  
  reclaim_size = size;
  start_address = base_address;
  
  reclaim_dbg_buffer[debug_cnt].memory_start_address = start_address;
  reclaim_dbg_buffer[debug_cnt].memory_size = reclaim_size;
  
  if(start_address  == NULL)
  {
    reclaim_dbg_buffer[debug_cnt].error_msg = HEAP_MGT_RECLAIM_INVALID_START_ADDRESS;
    return 1;
  }
  
  if(reclaim_size == NULL)
  {
    reclaim_dbg_buffer[debug_cnt].error_msg = HEAP_MGT_RECLAIM_INVALID_MEM_SIZE;
  }
  
  if(size < MIN_MEMORY_SECTION_SIZ)
  {
    reclaim_dbg_buffer[debug_cnt].error_msg = HEAP_MGT_RECLAIM_INVALID_MEM_SIZE;
    return 1;
  }
  
  mem_heap_add_section(&modem_mem_heap, (void *)start_address, (unsigned long)(reclaim_size));
  
  reclaim_dbg_buffer[debug_cnt].error_msg = HEAP_MGT_RECLAIM_SUCCESS;

  debug_cnt++;

   return 0;  
}

