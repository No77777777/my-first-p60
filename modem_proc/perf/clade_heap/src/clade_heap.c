/*!
  @file
  clade_heap.c

  @brief
  Implementation of Heap in CLADE dynamic region.
  
*/

/*===========================================================================

  Copyright (c) 2019 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/perf.mpss/4.4.2/clade_heap/src/clade_heap.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/11/19   xx      Initial Version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include <comdef.h>           /* Definition for basic types and macros */
#include "clade_heap.h"        /* External interface to modem_mem.c */
#include "memheap.h"

#ifdef FEATURE_CLADE_HEAP

#ifdef FEATURE_MEM_DEBUG
#include "rcinit.h"
#endif

mem_heap_type clade_mem_heap = {0xff};
extern mem_heap_type  modem_mem_heap;
volatile unsigned int  is_clade_heap_initialized=0;


#ifdef FEATURE_MEM_DEBUG
#ifndef CLADE_HEAP_CALLER_LEVEL
#define CLADE_HEAP_CALLER_LEVEL     (0)
#endif

#if defined(FEATURE_QDSP6)
 #define  MEM_HEAP_CALLER_ADDRESS() ((void *)__builtin_return_address(CLADE_HEAP_CALLER_LEVEL));
#else
 #define MEM_HEAP_CALLER_ADDRESS() ((void *) __return_address())
#endif

#endif

#ifndef CLADE_HEAP_SIZE
#define CLADE_HEAP_SIZE (512 * 1024)
#endif

#define BOUNDARY_CHECK_SECTIONS(theBlock, heap_ptr) ((theBlock >= ((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)theBlock)->section_index].start_addr) && (theBlock < (mem_block_header_type *)((char*)(((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)theBlock)->section_index].start_addr) + ((mem_heap_type*)heap_ptr)->sections[((mem_block_header_type *)theBlock)->section_index].size)))


uint8 clade_heap_static_buffer[CLADE_HEAP_SIZE] __attribute__((aligned (64)));

volatile unsigned int clade_heap_size = CLADE_HEAP_SIZE;

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/


#ifdef FEATURE_MEM_DEBUG
/* AMSS memory debug functions */
extern void memdebug_updatecallerptr(mem_heap_type *heap_ptr,void *ptr,void *caller_ptr, size_t size);
memheap_task_stats_type clade_mem_heap_task_stats[MEMHEAP_MAX_THREADS];
extern uint32 memheap_global_error;
#endif  /* FEATURE_MEM_DEBUG */


void cladeheap_malloc_failed
(
  struct mem_heap_struct *heap_ptr, /* Heap on which allocation is attempted */
  size_t size                       /* Size of failed request */
);


void cladeheap_malloc_failed
(
  struct mem_heap_struct *heap_ptr, /* Heap on which allocation is attempted */
  size_t size                       /* Size of failed request */
)
{

 MSG_ERROR("clade heap allocation failed total bytes --> %u ,used bytes --> u ,failed Request size --> %u",heap_ptr->total_bytes,
            heap_ptr->heap_used_bytes,size);
}

void clade_init_heap(void)
{
  clade_heap_size = CLADE_HEAP_SIZE;
  memset(&clade_mem_heap, 0, sizeof(mem_heap_type));
  mem_init_heap(&clade_mem_heap,clade_heap_static_buffer,CLADE_HEAP_SIZE,cladeheap_malloc_failed);
  #ifdef FEATURE_MEM_DEBUG
   if(MEM_TLS_CREATE_ERR != memheap_global_error)
   {
      clade_mem_heap.heapStatistics = &clade_mem_heap_task_stats[0];
   }
   clade_mem_heap.mem_get_quota_fnc_ptr = NULL;
#endif

  return;
}


  void clade_check_heap_initialized(void)
{
  /* Immediately return if the heap is initialied; except for the very 
  ** first function call, it will return from here
  */
  if (is_clade_heap_initialized) 
  {
    return;
  }

  
  if (!(is_clade_heap_initialized))
  {
    clade_init_heap();
	is_clade_heap_initialized=1;
  }
  return;
}

void* clade_heap_alloc(
  size_t                size       /*!< Number of bytes to allocate */
)
{
  void *ptr = NULL;

  /*-----------------------------------------------------------------------*/

  if (!size) 
  {
    return NULL;  /* Zero size request - return NULL immediately */
  }

  /*-----------------------------------------------------------------------*/

  clade_check_heap_initialized();

  ptr=mem_malloc(&clade_mem_heap,size);
  

#ifdef FEATURE_MEM_DEBUG
  {
    void *caller_ptr=MEM_HEAP_CALLER_ADDRESS();
    memdebug_updatecallerptr(&clade_mem_heap,ptr, caller_ptr,size);
  }
#endif
  return ptr;
 
}


void* clade_heap_calloc(
  size_t                elt_count,
  size_t 			    elt_size

)
{
  void *ptr=NULL;
  clade_check_heap_initialized();
  ptr=mem_calloc(&clade_mem_heap,elt_count, elt_size);
#ifdef FEATURE_MEM_DEBUG
  {
    void *caller_ptr=MEM_HEAP_CALLER_ADDRESS();
    memdebug_updatecallerptr(&clade_mem_heap,ptr, caller_ptr,elt_count * elt_size);
  }
#endif
  return ptr;
} /* clade_heap_calloc() */

void *clade_heap_realloc
(
  void   *ptr,
  size_t size
)
{

  clade_check_heap_initialized();
  ptr = mem_realloc( &clade_mem_heap, ptr, size );
  #ifdef FEATURE_MEM_DEBUG
  {
     void *caller_ptr=MEM_HEAP_CALLER_ADDRESS();
     memdebug_updatecallerptr(&clade_mem_heap,ptr, caller_ptr,size);
  }
  #endif
  return ptr;
}



 void clade_heap_free(
  void                *ptr        /*!< Memory to free */
)
{
 
   mem_free(&clade_mem_heap,ptr);
 
}

#endif // FEATURE_CLADE_HEAP
