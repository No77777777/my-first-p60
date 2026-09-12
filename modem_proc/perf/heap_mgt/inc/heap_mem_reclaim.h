/********************************************//**
*\file heap_mem_reclaim.h

*\copyright (c) 2006 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/perf.mpss/4.4.2/heap_mgt/inc/heap_mem_reclaim.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/12/16   skotalwa  skotalwa Added generic api to reclaim memory into heap
==========================================================================*/



extern int reclaim_memory_to_heap(void* base_address, uint32 size);

/*Minimum size requried for reclaim*/
#define MIN_MEMORY_SECTION_SIZ        20000

/*debug info for heap reclaim api*/ 
typedef enum 
 {
  HEAP_MGT_RECLAIM_INVALID                         = 0,      
  HEAP_MGT_RECLAIM_SUCCESS                         = 1,
  HEAP_MGT_RECLAIM_FAILED                          = 2,
  HEAP_MGT_RECLAIM_INVALID_START_ADDRESS           = 3,
  HEAP_MGT_RECLAIM_INVALID_MEM_SIZE                = 4
  }reclaim_err_t;
 
 
  struct reclaim_dbg_buffer_t
 {
  void* memory_start_address;
  uint32 memory_size;
  reclaim_err_t error_msg;
 } ;