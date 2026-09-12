/* =========================================================================

Copyright (C) 2019 by QUALCOMM Technologies, Inc.  All Rights Reserved.
============================================================================ */

/* =========================================================================

                             Edit History

$Header: //components/rel/avs.mpss/9.0.c51/vsd/utils/src/vs_memmgr.c#1 $ */

/* ------------------------------------------------------------------------
** Includes
** ------------------------------------------------------------------------ */
#include "vs_memmgr.h"
/* ------------------------------------------------------------------------
** Externs
** ------------------------------------------------------------------------ */

/* defaults to NULL.  If not NULL, function will be called
   whenever an allocation has failed due to an out of heap
   space condition and is about to return NULL to the caller */
extern vs_memmgr_allocator_failed_proc_type vs_memmgr_allocator_failed_hook;

/* ------------------------------------------------------------------------
** Types
** ------------------------------------------------------------------------ */
typedef mem_heap_block_iterator_type vs_memmgr_heap_block_iterator_type;

typedef mem_heap_totals_type vs_memmgr_heap_totals_type;

typedef mem_block_header_type vs_memmgr_block_header_type;

typedef mem_heap_type vs_memheap_type;
/* ------------------------------------------------------------------------
** Defines
** ------------------------------------------------------------------------ */


#define TCB_TYPE    int
#define TCB_SELF()  0

#define VS_MEMMGR_ASSERT( xx_exp ) \
  if( !(xx_exp) ) \
  { \
      ERR( "In task 0x%x, Assertion " #xx_exp " failed", \
       (unsigned long) TCB_SELF(), 0, 0 ); \
  }

typedef void (*vs_memheap_allocator_failed_proc_type)(
   struct mem_heap_struct *heap_ptr,
   size_t                  request_size
   );

/*===========================================================================
FUNCTION VS_MEMMGR_INIT_HEAP

DESCRIPTION
  Initializes the heap_ptr object and sets up inMemoryChunk for use with the
  heap_ptr object.  inMemoryChunk may be aligned on any boundary.  Beginning
  bytes will be skipped until a paragraph boundary is reached.  Do NOT pass
  in NULL pointers.  infail_fnc_ptr may be NULL in which case no function will
  be called if apr_memmgr_malloc or apr_memmgr_calloc is about to fail.  If infail_fnc_ptr
  is provided, it will be called once and then the allocation will be
  attempted again.  See description of my_allocator_failed_proc for details.
  There is no protection for initializing a heap more than once.  If a heap
  is re-initialized, all pointers previously allocated from the heap are
  immediately invalidated and their contents possibly destroyed.  If that's
  the desired behavior, a heap may be initialized more than once.
===========================================================================*/
/*lint -sem(apr_memmgr_init_heap,1p,2p,2P>=3n) */
void vs_memmgr_init_heap(
   vs_memmgr_type                       *heap_ptr,
      /* Statically allocated heap structure
      */
   void                                  *heap_mem_ptr,
      /* Pointer to contiguous block of memory used for this heap
      */
   unsigned long                          heap_mem_size,
      /* The size in bytes of the memory pointed to by heap_mem_ptr
      */
   vs_memmgr_allocator_failed_proc_type  fail_fnc_ptr
      /* Function to call when allocation fails, can be NULL
      */
)
{
   void *heap_wrapper_ptr = (void *)VS_MEM_ALIGN_64(heap_ptr);	
   VS_MEMMGR_ASSERT(heap_wrapper_ptr);   
   mem_init_heap((vs_memheap_type *)heap_wrapper_ptr,heap_mem_ptr,heap_mem_size,(vs_memheap_allocator_failed_proc_type)fail_fnc_ptr);

} /* END vs_memmgr_init_heap */

/*===========================================================================
FUNCTION VS_MEMMGR_MALLOC

DESCRIPTION
  Allocates a block of size bytes from the heap.  If heap_ptr is NULL
  or size is 0, the NULL pointer will be silently returned.

  Returns a pointer to the newly allocated block, or NULL if the block
  could not be allocated.
===========================================================================*/
void* vs_memmgr_malloc(
   vs_memmgr_type *heap_ptr,
   /* Heap from which to allocate
         */
   size_t           size
   /* Number of bytes to allocate
         */
   )
{
	return mem_malloc((vs_memheap_type *)VS_MEM_ALIGN_64(heap_ptr),size);

} /* END vs_memmgr_malloc */


/*===========================================================================
FUNCTION VS_MEMMGR_FREE

DESCRIPTION
  Deallocates the ptr block of memory.  If ptr is NULL, heap_ptr is NULL or
  ptr is outside the range of memory managed by heap_ptr, then this function
  call does nothing (and is guaranteed to be harmless).  This function will
  ASSERT if it can detect an attempt to free an already freed block.  (This
  is not always reliable though, so it might not catch it.)
===========================================================================*/
void vs_memmgr_free(
   vs_memmgr_type *heap_ptr,
      /* Heap in which to free memory*/
   void            *ptr
      /* Memory to free*/
   )
{
	mem_free((vs_memheap_type *)VS_MEM_ALIGN_64(heap_ptr),ptr);
} /* END vs_memmgr_free */


/*===========================================================================
FUNCTION VS_MEMMGR_REALLOC

DESCRIPTION
  Resizes the ptr block of memory to be size bytes while preserving the
  block's contents.  If the block is shortened, bytes are discarded off the
  end.  If the block is lengthened, the new bytes added are not initialized
  and will have garbage values.  If heap_ptr is NULL, the contents of ptr are
  unchanged and the function silently returns NULL.  If ptr is NULL, the
  functions behaves exactly like apr_memmgr_malloc (see above).  If ptr is not NULL
  and size is 0, the function behaves exactly like apr_memmgr_free (see below).  If
  the block cannot be resized, ptr is not NULL and size is not 0, then NULL
  is returned and the original block is left untouched.  If the ptr block IS
  successfully resized and the returned value is different from the ptr value
  passed in, the old block passed in must be considered deallocated and no
  longer useable (i.e. do NOT try to apr_memmgr_free it!)  This function will ASSERT
  if it can detect a bad pointer or a pointer to a free block within the
  range of memory managed by heap_ptr.  However, it's not always possible to
  do this.  Passing in a ptr block that is outside the memory managed by
  heap_ptr will result in a silent return of NULL with the contents of ptr
  unchanged.  This function will call apr_memmgr_malloc if it can't grow the block
  in place, so the allocater failed function WILL be called if the heap is
  out of room to grow the block.

  Returns a pointer to the beginning of the resized block of memory (which
  may be different than ptr) or NULL if the block cannot be resized.
===========================================================================*/
void *vs_memmgr_realloc(
    vs_memmgr_type *heap_ptr,
       /* Heap in which to re-allocate */
    void            *ptr,
       /* A block previously allocated from heap_ptr */
    size_t           size
       /* New size (in bytes) of the ptr block of memory*/
   )
{
	return mem_realloc((vs_memheap_type *)VS_MEM_ALIGN_64(heap_ptr),ptr,size);
} /* END vs_memmgr_realloc */

