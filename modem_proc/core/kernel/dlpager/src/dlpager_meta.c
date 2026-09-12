/*==========================================================================
 * FILE:         dlpager_meta.c
 *
 * SERVICES:     DL PAGER METADATA
 *
 * DESCRIPTION:  This file provides functionality for managing and maintaining metadata 
                 generated at build time.
 *
 * Copyright (c) 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/
/*===========================================================================

            EDIT HISTORY FOR MODULE


===========================================================================*/
/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include <assert.h>
#include <dlpager_types.h>
#include <dlpager_meta.h>
#include <q6zip_uncompress.h>
#include <memheap.h>
#include <dlpager_stats.h>
#include <dlpager_debug.h>
#include <stringl.h>

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/
/** @brief Entry for an RW (re)compressed page
    @warning Crashscope depends on this data structure to decompress RW
     compressed pages. Please *DONOT* change composition, ordering and types in
    this data structure without coordinating with QCT's Crashscope team */
typedef struct 
{
  void *buffer;
  unsigned short length_used;
  unsigned short length_allocated;
}buf_entry_t;

/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/
extern unsigned int __attribute__ ((weak)) __dlpager_heap_start__;
extern unsigned int __attribute__ ((weak)) __dlpager_heap_end__;

/** @brief These symbols are poked by elfManipulator.py and used by DL Pager.
           They *MUST BE* placed in .data section */
unsigned int __attribute__((section(".data"))) start_va_compressed_text = 0;
unsigned int __attribute__((section(".data"))) end_va_compressed_text = 0;
unsigned int __attribute__((section(".data"))) start_va_compressed_rw = 0;
unsigned int __attribute__((section(".data"))) end_va_compressed_rw = 0;

/** @brief These symbols are poked by elfManipulator.py and used by
           off-target scripts. They *MUST BE* placed in .data section */
unsigned int __attribute__((section(".data"))) start_va_uncompressed_text = 0;
unsigned int __attribute__((section(".data"))) end_va_uncompressed_text = 0;
unsigned int __attribute__((section(".data"))) start_va_uncompressed_rw = 0;
unsigned int __attribute__((section(".data"))) end_va_uncompressed_rw = 0;

/** @brief Convenient variable that points to heap for RW compressed pages
           for easy access from Trace32 to run heap-walker script */
mem_heap_type const * dlpager_heap = NULL;

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/
/* RO metadata generated at build time */
struct __attribute__ ((__packed__)) dlpager_metadata_rx
{
  unsigned short total_blocks;  /* number of uncompressed pages of text */
  char unused[2];
  unsigned int dictionary[Q6ZIP_DICT_SIZE];
  unsigned int compressed_va[1]; /* contains n_blocks of uint32 addresses
                                mapping uncompressed text to compressed text */
} *dlpager_metadata_rx_p;


/* RW metadata generated at build time */
struct __attribute__ ((__packed__)) _dlpager_metadata_rw
{
  unsigned short total_blocks;  /* number of uncompressed pages of text */
  char unused[2];
  unsigned int compressed_va[1]; /* contains n_blocks of uint32 addresses
                                mapping uncompressed rw data to compressed rw data */
}*_dlpager_metadata_rw_p;

/** @brief RW metadata at run time
    @warning Crashscope depends on this data structure to decompress RW
     compressed pages. Please *DONOT* change composition, ordering and types in
    this data structure without coordinating with QCT's Crashscope team */
struct dlpager_metadata_rw
{
  mem_heap_type heap;
  void *heap_buffer;
  unsigned int heap_size;

  unsigned int num_rw_blocks;
  unsigned int num_bss_blocks;
  unsigned int total_blocks;
  buf_entry_t *compressed_va;
}dlpager_metadata_rw;

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/
boolean dlpager_meta_is_addr_in_text_range(unsigned int va)
{
  return ((va >= (unsigned int)&__swapped_segments_text_start__) &&
          (va <  (unsigned int)&__swapped_segments_text_end__)); 
}

boolean dlpager_meta_is_addr_in_rodata_range(unsigned int va)
{
  return ((va >= (unsigned int)&__swapped_segments_rodata_start__) &&
          (va <  (unsigned int)&__swapped_segments_rodata_end__)); 
}

boolean dlpager_meta_is_addr_in_text_or_rodata_range(unsigned int va)
{
  return (dlpager_meta_is_addr_in_text_range(va) 
           || dlpager_meta_is_addr_in_rodata_range(va));
}

/* For the outside world, rwdata includes rw and bss sections */
boolean dlpager_meta_is_addr_in_rwdata_range(unsigned int va)
{
   return ((va >= (unsigned int)&__swapped_segments_rwdata_start__) &&
           (va <  (unsigned int)&__swapped_segments_rwdata_end__))
		  || ((va >= (unsigned int)&__swapped_segments_bss_start__) &&
			 (va <  (unsigned int)&__swapped_segments_bss_end__));    
}

inline boolean rwdata_section_exists()
{
  return ((unsigned int)&__swapped_segments_rwdata_start__ != 0)
		  && ((unsigned int)&__swapped_segments_rwdata_start__ != (unsigned int)&__swapped_segments_rwdata_end__);
}

inline boolean bss_section_exists()
{
  return ((unsigned int)&__swapped_segments_bss_start__ != 0)
		  && ((unsigned int)&__swapped_segments_bss_start__ != (unsigned int)&__swapped_segments_bss_end__);
}

inline unsigned int rwdata_section_size()
{
  return ((unsigned int)&__swapped_segments_rwdata_end__ - (unsigned int)&__swapped_segments_rwdata_start__);
}

inline unsigned int bss_section_size()
{
  return ((unsigned int)&__swapped_segments_bss_end__ - (unsigned int)&__swapped_segments_bss_start__);
}

//TODO: assumes 4K page size, ok/not ok?
dlpager_iovec_t dlpager_meta_get_compressed_block_addr( unsigned int va )
{
  unsigned int block;
  dlpager_iovec_t ret_blk;

  if(dlpager_meta_is_addr_in_text_or_rodata_range(va))
  {
     block = (va - (unsigned int)&__swapped_segments_start__) >> 12;
     ASSERT(block < dlpager_metadata_rx_p->total_blocks);
     ret_blk.addr = dlpager_metadata_rx_p->compressed_va[block];
    
    if(block+1 != dlpager_metadata_rx_p->total_blocks) 
      ret_blk.len = dlpager_metadata_rx_p->compressed_va[block+1] - dlpager_metadata_rx_p->compressed_va[block];
   else
     ret_blk.len = end_va_compressed_text - dlpager_metadata_rx_p->compressed_va[block];

     return ret_blk;
  }
  else if(dlpager_meta_is_addr_in_rwdata_range(va))
  {
     block = (va - (unsigned int)&__swapped_segments_rwdata_start__) >> 12;
     ASSERT(block < dlpager_metadata_rw.total_blocks);
     ret_blk.addr = (unsigned int)dlpager_metadata_rw.compressed_va[block].buffer;
     ret_blk.len = dlpager_metadata_rw.compressed_va[block].length_used;
     return ret_blk;
  }

  /* we should not reach here */
  ASSERT(0);

  return ret_blk;  
}

void dlpager_meta_set_compressed_block(unsigned int va, dlpager_iovec_t new_buffer)
{
  unsigned int block;
  void *current_buffer;
  unsigned int current_buf_sz;

  ASSERT(dlpager_meta_is_addr_in_rwdata_range(va));

  block = (va - (unsigned int)&__swapped_segments_rwdata_start__) >> 12;
  ASSERT(block < dlpager_metadata_rw.total_blocks);
  current_buffer = dlpager_metadata_rw.compressed_va[block].buffer;
  current_buf_sz = dlpager_metadata_rw.compressed_va[block].length_allocated;

  if(current_buf_sz < new_buffer.len)
  {
    /* free current buffer
       allocate a new one, update size */
    if ( current_buffer )
    {
      mem_free(&dlpager_metadata_rw.heap, current_buffer); 
    }
    current_buffer = mem_malloc(&dlpager_metadata_rw.heap, new_buffer.len+4);
    ASSERT(current_buffer!=NULL);
    dlpager_metadata_rw.compressed_va[block].buffer = current_buffer;
    dlpager_metadata_rw.compressed_va[block].length_allocated = (unsigned short)new_buffer.len;
  }

  /* copy new buffer to existing buffer */
  memscpy(current_buffer, new_buffer.len, (void *)new_buffer.addr, new_buffer.len);
  dlpager_metadata_rw.compressed_va[block].length_used = (unsigned short)new_buffer.len;

  /* flush compressed buffer to DDR for HW to use for subsequent decompression of the block*/
  qurt_mem_cache_clean ((qurt_addr_t)current_buffer, new_buffer.len, QURT_MEM_CACHE_FLUSH_INVALIDATE, QURT_MEM_DCACHE);
}

char * dlpager_meta_q6zip_dictionary(void)
{
  return((char *) &dlpager_metadata_rx_p->dictionary);
}

unsigned int dlpager_meta_init()
{
  unsigned int total_blocks;
  int i=0;

  /* Verify that sizes of candidate Q6Zip sections specified by linker-assigned
     and elfManipulator.py-poked variables match. This also has a nice side-effect
     of ensuring that the elfManipulator.py-poked variables are not optimized-out! */
  ASSERT( ( (unsigned int)&__swapped_segments_rw_start__  - (unsigned int)&__swapped_segments_start__ ) == ( end_va_uncompressed_text - start_va_uncompressed_text ) );
  ASSERT( ( (unsigned int)&__swapped_segments_end__ - (unsigned int)&__swapped_segments_rw_start__ ) == ( end_va_uncompressed_rw - start_va_uncompressed_rw ) );
 
   /* init ROX */
   dlpager_metadata_rx_p = (struct dlpager_metadata_rx *) start_va_compressed_text;

  /* Initialize heap */
  if ( ( &__dlpager_heap_end__ - &__dlpager_heap_start__ ) > 0 )
  {
      dlpager_metadata_rw.heap_buffer =(void*)&__dlpager_heap_start__;
      dlpager_metadata_rw.heap_size = (unsigned int)&__dlpager_heap_end__ - (unsigned int)&__dlpager_heap_start__;

      /* Must use the lite API as PRNG HW is not available this early in boot. */
      mem_init_heap_lite(&dlpager_metadata_rw.heap, dlpager_metadata_rw.heap_buffer, dlpager_metadata_rw.heap_size, NULL);
  }

  /* init datastructures with defaults */
  if(rwdata_section_exists() || bss_section_exists())
  {
     /* initialization common to rw and bss */
     _dlpager_metadata_rw_p = (struct _dlpager_metadata_rw *) start_va_compressed_rw;
     /* init rw heap */

     dlpager_heap = &dlpager_metadata_rw.heap;
           
    /* allocate memory to hold rw + bss if non-empty */
    total_blocks = (rwdata_section_size() + bss_section_size()) >> 12;
    dlpager_metadata_rw.num_rw_blocks = 0; 
    dlpager_metadata_rw.num_bss_blocks = 0;
    dlpager_metadata_rw.total_blocks = total_blocks;

    dlpager_metadata_rw.compressed_va = mem_malloc(&dlpager_metadata_rw.heap, sizeof(buf_entry_t) * total_blocks);
    ASSERT(dlpager_metadata_rw.compressed_va != NULL);
  }   

  if(rwdata_section_exists())
  {
    /* rw init */
    _dlpager_metadata_rw_p = (struct _dlpager_metadata_rw *) start_va_compressed_rw;

    /* allocate memory to hold compressed rw blocks from the rw heap */
    dlpager_metadata_rw.num_rw_blocks = _dlpager_metadata_rw_p->total_blocks; 


    /*copy each compressed rw block over */   
    for (i = 0; i < dlpager_metadata_rw.num_rw_blocks; i++)
      {
         void * buffer;
         unsigned short len;

         buffer = (void *)_dlpager_metadata_rw_p->compressed_va[i];
         len = (i + 1 == dlpager_metadata_rw.num_rw_blocks) ?
               (end_va_compressed_rw - _dlpager_metadata_rw_p->compressed_va[i]) :
               (_dlpager_metadata_rw_p->compressed_va[i+1]- _dlpager_metadata_rw_p->compressed_va[i]);

         dlpager_metadata_rw.compressed_va[i].buffer = mem_malloc(&dlpager_metadata_rw.heap, len+4); 
         ASSERT(dlpager_metadata_rw.compressed_va[i].buffer != NULL);
         dlpager_metadata_rw.compressed_va[i].length_allocated = len;
                  
         memscpy(dlpager_metadata_rw.compressed_va[i].buffer, len, (void *)buffer, len);
         dlpager_metadata_rw.compressed_va[i].length_used = len;

    }

  }

  if(bss_section_exists())
  {
    /* bss init */
    dlpager_metadata_rw.num_bss_blocks = bss_section_size() >> 12;
      /* set the bss blocks to null */
      for(; i < dlpager_metadata_rw.total_blocks; i++)
      {
         dlpager_metadata_rw.compressed_va[i].buffer = NULL;
         dlpager_metadata_rw.compressed_va[i].length_allocated = 0;
         dlpager_metadata_rw.compressed_va[i].length_used = 0;
      }
   }

   /* now that total_blocks is known, safe to alloc a buf for recompress_counts for all rw pages */
   //if(dlpager_osam_debug_enabled)
   /* @warning Here RW heap is assumed to exist even though RW compression
      may be disabled
      @todo rraghave Should memory for recompress_count be coming out of RW 
      heap? Please investigate and fix (if deemed to be a bug) */
   dlpager_stats.recompress_count = (unsigned char *) mem_malloc(&dlpager_metadata_rw.heap, sizeof(unsigned char) * dlpager_metadata_rw.total_blocks); 
   ASSERT( dlpager_stats.recompress_count != NULL);
   return 0;
}
