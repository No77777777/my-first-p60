#ifndef VS_MEMMGR_H
#define VS_MEMMGR_H
/*===========================================================================

DESCRIPTION
  A simple sub-allocator to manage memory allocations and deallocations
  using a Next Fit strategy.

  Note that these routines are FULLY re-entrant.  Furthermore, while
  performing memory allocation/deallocation calls on one heap, the routines
  may be interrupted to perform memory allocation/deallocation calls on
  different heaps without blocking or corruption.  However, should an
  interrupting task attempt to perform memory allocation/deallocation
  on the same heap that had a critical section interrupted, it will block
  allowing the first call to finish.  All this is accomplished by giving
  each heap its own semaphore.

  FEATURE_APR_MEMMGR_MT
    Activates multi threading support for the heap

  Usage Notes - Avoiding Fragmentation

  1. If all bytes in the heap are freed, the heap is guaranteed to be returned
     to the same state as after a call to vs_memmgr_init_heap, except that the
     values of maxUsed & maxRequest are not reset.  The order in which blocks
     are freed is irrelevant so long as they ALL are freed.

  2. After n consecutive malloc/calloc calls, in a heap with non-fragmented
     free space (and no intervening realloc calls that return a new pointer),
     calling free in the reverse order on the n blocks guarantees that the heap
     will be returned to the exact same state as before the n alloc calls.

  3. FEATURE_APR_MEMMGR_HEAP_SMALLER_OVERHEAD == 0
     (2) still applies, except the calls to free may be made in any order and
     after all n blocks are freed, the heap is guaranteed to be returned to the
     exact same state as prior to the n alloc calls.

  4. FEATURE_APR_MEMMGR_HEAP_SMALLER_OVERHEAD == 1
     The overhead per heap block is 8 bytes.  However it's not possible to backup
     the next-block-to-start-searching-for-free-space pointer, so the heap MAY
     become more fragmented, but it MAY NOT -- it depends on the alloc/free
     calling patterns.

  5. FEATURE_APR_MEMMGR_HEAP_SMALLER_OVERHEAD == 0
     The overhead per heap block is 12 bytes.  Backing up is possible, so some
     fragmention that otherwise MIGHT occur can be prevented.  There is very little
     performance cost for this, the big expense is the additional 4 bytes of overhead
     per heap block.

Copyright (C) 1997-2009 by QUALCOMM Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                                 Edit History

$PVCSPath: O:/src/asw/COMMON/vcs/memheap.h_v   1.1   07 Mar 2002 18:48:56   rajeevg  $
$Header: //components/rel/avs.mpss/9.0.c51/vsd/utils/inc/protected/vs_memmgr.h#1 $
===========================================================================*/

/* ------------------------------------------------------------------------
** Includes
** ------------------------------------------------------------------------ */

#include <stddef.h>
#include "mmdefs.h"
#include "memheap.h"

/* ------------------------------------------------------------------------
** Defines
** ------------------------------------------------------------------------ */

/* This defines the amount of opaque data needed to be statically assigned
 * for the heap header
*/
#define VS_MEMMGR_OPAQUE_DATASIZE ( 2048 )

#define VS_MEM_ALIGN_64(x) ( (uint32_t)((char_t*)x + 7) & (~7) )

/* ------------------------------------------------------------------------
** Types
** ------------------------------------------------------------------------ */

/**
  Opaque heap structure. The internal heap mgr stores
  data in this structure. 
  @note Please treat this as a read only structure. 
  @note There is nothing in this structure 
        that can be read by a client
*/
typedef struct vs_memmgr_type{
    char opaque_data[VS_MEMMGR_OPAQUE_DATASIZE];
}vs_memmgr_type;

/* Allocation Failed Procedure
**   This function is called when there is not enough room in ioHeap to satisfy
**   a request of inRequest bytes.  This routine should free up any memory it
**   can before returning.  The request will then be tried again.  There are
**   no restrictions on what this function may do including memory allocation/
**   deallocation calls.  Note that before calling this function, calls to
**   the allocator failed proc on ioHeap are disabled.  After this function
**   returns, calls are re-enabled.  This does NOT affect other heaps, so if
**   multiple heaps share the same allocator failed proc, it should be
**   re-entrant.
*/

typedef void (*vs_memmgr_allocator_failed_proc_type)(
   struct vs_memmgr_type *heap_ptr,
      /* Heap on which allocation is attempted
      */
   size_t                    request_size
      /* Size of failed request
      */
);


/* Each heap can have its own function that is used to lock the heap and
** free the heap.  mem_lock_fnc_type and mem_free_fnc_type are the types
** of these functions.
*/
/**
  Provides an abstraction so each function can have its own function to 
  lock the heap.

  @param[in] ptr Pointer to what is to be locked.

  @return
  None.

  @dependencies
  None.
*/
typedef void (*vs_memmgr_lock_fnc_type)( void * ptr);

/**
  Provides an abstraction so each function can have its own function to 
  free the heap.

  @param[in] ptr Pointer to what is to be freed.

  @return
  None.

  @dependencies
  None.
*/
typedef void (*vs_memmgr_free_fnc_type)( void * ptr);

/* ------------------------------------------------------------------------
** Functions
** ------------------------------------------------------------------------ */

#ifdef __cplusplus
   extern "C"
   {
#endif

/*===========================================================================
FUNCTION VS_MEMMGR_HEAP_INIT

DESCRIPTION
  Initializes the ioHeap object and sets up inMemoryChunk for use with the
  ioHeap object.  inMemoryChunk may be aligned on any boundary.  Beginning
  bytes will be skipped until a paragraph boundary is reached.  Do NOT pass
  in NULL pointers.  inFailProc may be NULL in which case no function will
  be called if vs_memmgr_malloc or vs_memmgr_calloc is about to fail.  If inFailProc
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
);

/*===========================================================================
FUNCTION VS_MEMMGR_CALLOC

DESCRIPTION
  Allocates enough space for elt_count elements each of elt_size bytes
  from the heap and initializes the space to nul bytes.  If
  heap_ptr is NULL or elt_count or elt_size is 0, the NULL pointer will
  be silently returned.

  Returns a pointer to the newly allocated block, or NULL if the block
  could not be allocated.
===========================================================================*/
/*lint -sem(apr_memmgr_calloc,1p,2n>=0&&3n>=0&&(@p==0||@P==2n*3n)) */
#if 0
#ifdef FEATURE_APR_MEMMGR_DEBUG
   VS_INTERNAL void* apr_memmgr_calloc_debug(
      apr_memmgr_type *heap_ptr,
         /* Heap from which to allocate
         */
      size_t           elt_count,
         /* Number of elements to allocate
         */
      size_t           elt_size,
         /* Size of each element
         */
      char            *file_name,
         /* Name of file from which apr_memmgr_calloc was called
         */
      unsigned int     line_number
         /* Line number corresponding to apr_memmgr_calloc call
         */
   );
#else
   APR_INTERNAL void* apr_memmgr_calloc(
      apr_memmgr_type *heap_ptr,
         /* Heap from which to allocate
         */
      size_t           elt_count,
         /* Number of elements to allocate
         */
      size_t           elt_size
         /* Size of each element
         */
   );
#endif

#ifdef FEATURE_APR_MEMMGR_DEBUG
/*lint -save -e683 */
   #define  apr_memmgr_calloc(heap_ptr, elt_count, elt_size)\
      apr_memmgr_calloc_debug(heap_ptr,\
                              elt_count,\
                              elt_size,\
                              (char *) __FILENAME__,\
                              (unsigned int) __LINE__)

/*lint -restore */
#endif
#endif
/*===========================================================================
FUNCTION APR_MEMMGR_MALLOC

DESCRIPTION
  Allocates a block of size bytes from the heap.  If heap_ptr is NULL
  or size is 0, the NULL pointer will be silently returned.

  Returns a pointer to the newly allocated block, or NULL if the block
  could not be allocated.
===========================================================================*/
/*lint -sem(apr_memmgr_malloc,1p,2n>=0&&(@p==0||@P==2n)) */
#ifdef FEATURE_APR_MEMMGR_DEBUG
    void* vs_memmgr_malloc_debug(
      vs_memmgr_type *heap_ptr,
         /* Heap from which to allocate
         */
      size_t           size,
         /* Number of bytes to allocate
         */
      char            *file_name,
         /* Name of file from which apr_memmgr_malloc was called
         */
      unsigned int     line_number
         /* Line number corresponding to apr_memmgr_malloc call
         */
   );
#else
    void* vs_memmgr_malloc(
      vs_memmgr_type *heap_ptr,
         /* Heap from which to allocate
         */
      size_t           size
         /* Number of bytes to allocate
         */
   );
#endif

#ifdef FEATURE_APR_MEMMGR_DEBUG
/*lint -save -e683 */
   #define  vs_memmgr_malloc(heap_ptr, size)\
      vs_memmgr_malloc_debug(heap_ptr,\
                              size,\
                              (char *) __FILENAME__,\
                              (unsigned int) __LINE__)
/*lint -restore */
#endif

/*===========================================================================
FUNCTION APR_MEMMGR_REALLOC

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
/*lint -sem(apr_memmgr_realloc,1p,3n>=0&&(@p==0||@P==3n)) */
#ifdef FEATURE_APR_MEMMGR_DEBUG
    void* vs_memmgr_realloc_debug(
      vs_memmgr_type *heap_ptr,
         /* Heap from which to allocate
         */
      void            *ptr,
         /* A block previously allocated from heap_ptr
         */
      size_t           size,
         /* Number of bytes to allocate
         */
      char            *file_name,
         /* Name of file from which apr_memmgr_realloc was called
         */
      unsigned int     line_number
         /* Line number corresponding to apr_memmgr_realloc call
         */
   );
#else
    void *vs_memmgr_realloc(
      vs_memmgr_type *heap_ptr,
         /* Heap in which to re-allocate
         */
      void            *ptr,
         /* A block previously allocated from heap_ptr
         */
      size_t           size
         /* New size (in bytes) of the ptr block of memory
         */
   );
#endif

#ifdef FEATURE_APR_MEMMGR_DEBUG
/*lint -save -e683 */
   #define  vs_memmgr_realloc(heap_ptr, ptr, size)\
      vs_memmgr_realloc_debug(heap_ptr,\
                               ptr,\
                               size,\
                               (char *) __FILENAME__,\
                               (unsigned int) __LINE__)
/*lint -restore */
#endif

/*===========================================================================
FUNCTION APR_MEMMGR_FREE

DESCRIPTION
  Deallocates the ptr block of memory.  If ptr is NULL, heap_ptr is NULL or
  ptr is outside the range of memory managed by heap_ptr, then this function
  call does nothing (and is guaranteed to be harmless).  This function will
  ASSERT if it can detect an attempt to free an already freed block.  (This
  is not always reliable though, so it might not catch it.)
===========================================================================*/
/*lint -sem(apr_memmgr_free,1p) */
#ifdef FEATURE_APR_MEMMGR_DEBUG
    void vs_memmgr_free_debug(
      vs_memmgr_type *heap_ptr,
         /* Heap from which to allocate
         */
      void            *ptr,
         /* Memory to free
         */
      char            *file_name,
         /* Name of file from which apr_memmgr_free was called
         */
      unsigned int     line_number
         /* Line number corresponding to apr_memmgr_free call
         */
   );
#else
    void vs_memmgr_free(
      vs_memmgr_type *heap_ptr,
         /* Heap in which to free memory
         */
      void            *ptr
         /* Memory to free
         */
   );
#endif

#ifdef FEATURE_APR_MEMMGR_DEBUG
/*lint -save -e683 */
   #define  vs_memmgr_free(heap_ptr, ptr)\
      vs_memmgr_free_debug(heap_ptr,\
                            ptr,\
                            (char *) __FILENAME__,\
                            (unsigned int) __LINE__)
/*lint -restore */
#endif

/*===========================================================================
FUNCTION VS_MEMMGR_HEAP_SET_LOCK

DESCRIPTION
  This function sets up the specified heap to use (interrupt) locking and
  freeing at its locking mechanism.
  Returns 1 on success and 0 on failure.
===========================================================================*/
int vs_memmgr_heap_set_lock(
   vs_memmgr_type          *heap_ptr,
   vs_memmgr_lock_fnc_type  lock_fnc_ptr,
   vs_memmgr_free_fnc_type  free_fnc_ptr
);

/*===========================================================================
FUNCTION VS_MEMMGR_HEAP_SET_NO_LOCK

DESCRIPTION
  This function sets up the specified heap to use no locking. This implies
  that this heap should only be used from the context of one task.
  Returns 1 on success and 0 on failure.
===========================================================================*/
 int vs_memmgr_heap_set_no_lock( vs_memmgr_type *heap_ptr );

#ifdef __cplusplus
   }
#endif

#endif /* VS_MEMMGR_H */

