/*==========================================================================
 * FILE:         dlpager_pagesm.c
 *
 * SERVICES:     DL PAGER STATE MACHINE
 *
 * DESCRIPTION:  This file implements page state lookup/transitions for the async dlpager
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
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <qurt.h>
#include <atomic_ops.h>
#include <dlpager.h>
#include <dlpager_types.h>
#include <dlpager_params.h>
#include <dlpager_pagesm.h>
#include <dlpager_handlers.h>
#include <dlpager_stats.h>
#include <dlpager_debug.h>
#include <dlpager_meta.h>

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/
#define PAGESM_LOG_ENABLE 1
#define PAGESM_LOG_ENTRIES_BITS 8
#define PAGESM_LOG_ENTRIES_MAX (1 << PAGESM_LOG_ENTRIES_BITS)
#define PAGESM_LOG_ENTRIES_MASK (PAGESM_LOG_ENTRIES_MAX - 1)

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/
/* Actual page state machine */
typedef struct
{
  dlpager_page_state_t *table;
  unsigned int count; 
  qurt_mutex_t mutex; 
} dlpager_pagesm_t;

typedef void (*dlpager_handler_fptr_t)( unsigned int, qurt_thread_t );

/* A mutex for a page in transit*/
typedef struct
{
  qurt_mutex_t mutex;
  unsigned int page_va;
  unsigned short ref_count;  
  unsigned short valid;
} page_mutex_t;

typedef struct
{
  unsigned int num_allocd;
  unsigned int max_allocd;
  page_mutex_t table[MAX_PENDING_PAGES];
} page_mutexes_t;

typedef struct
{
   unsigned int ticks:24;
   dlpager_event_t event:8;
   unsigned int page:16;
   dlpager_page_state_t start_state:8;
   dlpager_page_state_t end_state:8;
} pagesm_log_entry_t;

/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/
static dlpager_pagesm_t dlpager_pagesm;
static page_mutexes_t page_mutexes;
#if PAGESM_LOG_ENABLE
static pagesm_log_entry_t dlpager_pagesm_log[PAGESM_LOG_ENTRIES_MAX]  __attribute__((aligned(32)));
static atomic_word_t dlpager_pagesm_log_idx = ATOMIC_INIT(-1);
#endif

static const dlpager_page_state_t next_state[MAX_STATES][MAX_EVENTS] =
{
                                             /*TLB_MISS_X,                   TLB_MISS_R,                       TLB_MISS_W,                          EVICT_PAGE,       DECOMPRESSION_COMPLETE, COMPRESSION_COMPLETE,       SOFT_CLEAN_PAGE,                      HARD_CLEAN_PAGE,                  SOFT_CLEAN_FAILED */
  /* UNMAPPED_CLEAN */                       { UNMAPPED_CLEAN_DECOMPRESSING, UNMAPPED_CLEAN_DECOMPRESSING,     UNMAPPED_DIRTY_DECOMPRESSING,        INVALID_STATE,    INVALID_STATE,          INVALID_STATE,              INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* UNMAPPED_BSS */                         { INVALID_STATE,                MAPPED_CLEAN_BSS,                 MAPPED_DIRTY,                        INVALID_STATE,    INVALID_STATE,          INVALID_STATE,              INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* UNMAPPED_CLEAN_DECOMPRESSING */         { UNMAPPED_CLEAN_DECOMPRESSING, UNMAPPED_CLEAN_DECOMPRESSING,     UNMAPPED_DIRTY_DECOMPRESSING,        INVALID_STATE,    MAPPED_CLEAN,           INVALID_STATE,              INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* UNMAPPED_DIRTY_DECOMPRESSING */         { INVALID_STATE,                UNMAPPED_DIRTY_DECOMPRESSING,     UNMAPPED_DIRTY_DECOMPRESSING,        INVALID_STATE,    MAPPED_DIRTY,           INVALID_STATE,              INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* UNMAPPED_DIRTY_COMPRESSING_ALLOCATED */ { INVALID_STATE,                MAPPED_CLEAN_COMPRESSING,         MAPPED_DIRTY_COMPRESSING,            INVALID_STATE,    INVALID_STATE,          UNMAPPED_CLEAN_ALLOCATED,   INVALID_STATE,                        INVALID_STATE,                    MAPPED_DIRTY     },
  /* UNMAPPED_CLEAN_ALLOCATED */             { INVALID_STATE,                MAPPED_CLEAN,                     MAPPED_DIRTY,                        UNMAPPED_CLEAN,   INVALID_STATE,          INVALID_STATE,              INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* UNMAPPED_HARD_CLEAN_COMPRESSING */      { INVALID_STATE,                UNMAPPED_HARD_CLEAN_COMPRESSING,  UNMAPPED_HARD_CLEAN_COMPRESSING,     INVALID_STATE,    INVALID_STATE,          UNMAPPED_CLEAN,             INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* MAPPED_CLEAN */                         { MAPPED_CLEAN,                 MAPPED_CLEAN,                     MAPPED_DIRTY,                        UNMAPPED_CLEAN,   INVALID_STATE,          INVALID_STATE,              INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* MAPPED_DIRTY */                         { INVALID_STATE,                MAPPED_DIRTY,                     MAPPED_DIRTY,                        INVALID_STATE,    INVALID_STATE,          INVALID_STATE,              UNMAPPED_DIRTY_COMPRESSING_ALLOCATED, UNMAPPED_HARD_CLEAN_COMPRESSING,  INVALID_STATE    },
  /* MAPPED_DIRTY_COMPRESSING */             { INVALID_STATE,                MAPPED_DIRTY_COMPRESSING,         MAPPED_DIRTY_COMPRESSING,            INVALID_STATE,    INVALID_STATE,          MAPPED_DIRTY,               INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* MAPPED_CLEAN_COMPRESSING */             { INVALID_STATE,                MAPPED_CLEAN_COMPRESSING,         MAPPED_DIRTY_COMPRESSING,            INVALID_STATE,    INVALID_STATE,          MAPPED_CLEAN,               INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
  /* MAPPED_CLEAN_BSS */                     { INVALID_STATE,                MAPPED_CLEAN_BSS,                 MAPPED_DIRTY,                        UNMAPPED_BSS,     INVALID_STATE,          INVALID_STATE,              INVALID_STATE,                        INVALID_STATE,                    INVALID_STATE    },
};

static const dlpager_handler_fptr_t dlpager_handlers[MAX_STATES][MAX_EVENTS] =
{
   { dlpager_handler_tlb_miss_x_at_unmapped_clean, dlpager_handler_tlb_miss_r_at_unmapped_clean, dlpager_handler_tlb_miss_w_at_unmapped_clean, 0, 0, 0, 0, 0, 0 },
   { 0, dlpager_handler_tlb_miss_r_at_unmapped_bss, dlpager_handler_tlb_miss_w_at_unmapped_bss, 0, 0, 0, 0, 0, 0 },
   { dlpager_handler_tlb_miss_x_at_unmapped_clean_decompressing, dlpager_handler_tlb_miss_r_at_unmapped_clean_decompressing, dlpager_handler_tlb_miss_w_at_unmapped_clean_decompressing, 0, dlpager_handler_decompression_complete_at_unmapped_clean_decompressing, 0, 0, 0, 0 },
   { 0, dlpager_handler_tlb_miss_r_at_unmapped_dirty_decompressing, dlpager_handler_tlb_miss_w_at_unmapped_dirty_decompressing, 0, dlpager_handler_decompression_complete_at_unmapped_dirty_decompressing, 0, 0, 0, 0 },
   { 0, dlpager_handler_tlb_miss_r_at_unmapped_dirty_compressing_allocated, dlpager_handler_tlb_miss_w_at_unmapped_dirty_compressing_allocated, 0, 0, dlpager_handler_compression_complete_at_unmapped_dirty_compressing_allocated, 0, 0, dlpager_handler_soft_clean_failed_at_unmapped_dirty_compressing_allocated },
   { 0, dlpager_handler_tlb_miss_r_at_unmapped_clean_allocated, dlpager_handler_tlb_miss_w_at_unmapped_clean_allocated, dlpager_handler_evict_page_at_unmapped_clean_allocated, 0, 0, 0, 0, 0 },
   { 0, dlpager_handler_tlb_miss_r_at_unmapped_hard_clean_compressing, dlpager_handler_tlb_miss_w_at_unmapped_hard_clean_compressing, 0, 0, dlpager_handler_compression_complete_at_unmapped_hard_clean_compressing, 0, 0, 0 },
   { dlpager_handler_tlb_miss_x_at_mapped_clean, dlpager_handler_tlb_miss_r_at_mapped_clean, dlpager_handler_tlb_miss_w_at_mapped_clean, dlpager_handler_evict_page_at_mapped_clean, 0, 0, 0, 0, 0 },
   { 0, dlpager_handler_tlb_miss_r_at_mapped_dirty, dlpager_handler_tlb_miss_w_at_mapped_dirty, 0, 0, 0, dlpager_handler_soft_clean_page_at_mapped_dirty, dlpager_handler_hard_clean_page_at_mapped_dirty, 0 },
   { 0, dlpager_handler_tlb_miss_r_at_mapped_dirty_compressing, dlpager_handler_tlb_miss_w_at_mapped_dirty_compressing, 0, 0, dlpager_handler_compression_complete_at_mapped_dirty_compressing, 0, 0, 0 }, 
   { 0, dlpager_handler_tlb_miss_r_at_mapped_clean_compressing, dlpager_handler_tlb_miss_w_at_mapped_clean_compressing, 0, 0, dlpager_handler_compression_complete_at_mapped_clean_compressing, 0, 0, 0 },
   { 0, dlpager_handler_tlb_miss_r_at_mapped_clean_bss, dlpager_handler_tlb_miss_w_at_mapped_clean_bss, dlpager_handler_evict_page_at_mapped_clean_bss, 0, 0, 0, 0, 0 },
};

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/
#if PAGESM_LOG_ENABLE
static unsigned int get_qtimer_32( void )
{
   unsigned int ret;
   asm volatile (" %0 = c30 " : "=r"(ret));
   return ret;
}

static void pagesm_log_write( unsigned int page, dlpager_event_t event, dlpager_page_state_t start_state, dlpager_page_state_t end_state )
{
   unsigned int log_idx = 0;
   
   log_idx = atomic_inc_return( &dlpager_pagesm_log_idx ) & PAGESM_LOG_ENTRIES_MASK;
   
   dlpager_pagesm_log[log_idx].ticks = get_qtimer_32();
   dlpager_pagesm_log[log_idx].event = event;
   dlpager_pagesm_log[log_idx].page = page;
   dlpager_pagesm_log[log_idx].start_state = start_state;
   dlpager_pagesm_log[log_idx].end_state = end_state;
}
#endif

static int pagesm_pagemutex_get( unsigned int fault_addr )
{
   int free_entry = -1;
   int matched_entry = -1;
   int retval = -1;

   qurt_pimutex_lock( &dlpager_pagesm.mutex );
   for( int iter = 0; iter < MAX_PENDING_PAGES; iter++ )
   {
      if( (page_mutexes.table[iter].valid == 1) && (page_mutexes.table[iter].page_va == fault_addr) )
      {
         matched_entry = iter;
         break;
      }
      /* Must go through all MAX_PENDING_PAGES. This is a sparse array. A match could be at last []. */
      if( (page_mutexes.table[iter].valid == 0) && (free_entry == -1) )
      {
         free_entry = iter;
      }
   }

   if( matched_entry != -1 )
   {
      page_mutexes.table[matched_entry].ref_count++;
      retval = matched_entry;
   }
   else if( free_entry != -1 )
   {
      page_mutexes.table[free_entry].valid = 1;
      page_mutexes.table[free_entry].page_va = fault_addr;
      page_mutexes.table[free_entry].ref_count = 1;
      page_mutexes.num_allocd ++;
      if( page_mutexes.max_allocd < page_mutexes.num_allocd )
      {
         page_mutexes.max_allocd = page_mutexes.num_allocd;
      }
      retval = free_entry;
   }         
   else
   {
      ASSERT(0);    
   }
   qurt_pimutex_unlock(&dlpager_pagesm.mutex);
  
   return retval;
}

static inline void pagesm_pagemutex_lock(int iter)
{
  qurt_pimutex_lock(&page_mutexes.table[iter].mutex);
}

static inline void pagesm_pagemutex_unlock(int iter)
{
  qurt_pimutex_unlock(&page_mutexes.table[iter].mutex);
}

static inline void pagesm_pagemutex_free(int matched_entry)
{
  qurt_pimutex_lock(&dlpager_pagesm.mutex);
  /* ensure matched entry is valid*/
  ASSERT(page_mutexes.table[matched_entry].valid == 1);
  
  page_mutexes.table[matched_entry].ref_count--;

  if(page_mutexes.table[matched_entry].ref_count == 0)
  {
    page_mutexes.table[matched_entry].valid = 0;
    page_mutexes.table[matched_entry].page_va = 0; 
    page_mutexes.num_allocd --;   
  }  
  qurt_pimutex_unlock(&dlpager_pagesm.mutex);
}

static void pagesm_pagemutex_init()
{
   int i;
   page_mutexes.num_allocd = 0;
   page_mutexes.max_allocd = 0;

   for( i = 0; i < MAX_PENDING_PAGES; i++ )
   {
     page_mutexes.table[i].page_va = 0;
     page_mutexes.table[i].ref_count = 0;
     qurt_pimutex_init( &page_mutexes.table[i].mutex );
     page_mutexes.table[i].valid = 0;
   }
}

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/
unsigned int dlpager_pagesm_is_page_evictable( unsigned int va )
{
   unsigned int idx;

   /* RX pages are always evictable. We should only be here to check RW pages. */
   ASSERT( ((unsigned int)&__swapped_segments_rwdata_start__ <= va) && (va < (unsigned int)&__swapped_segments_end__) );
  
   idx = (va - (unsigned int)&__swapped_segments_start__) >> PAGE_SHIFT;   

   return ( UNMAPPED_CLEAN_ALLOCATED == dlpager_pagesm.table[idx] || 
            MAPPED_CLEAN_BSS == dlpager_pagesm.table[idx] ||
            MAPPED_CLEAN == dlpager_pagesm.table[idx]);
}

unsigned int dlpager_pagesm_is_page_dirty( unsigned int va )
{
   unsigned int idx = 0;

   if( 0 == va )
   {
      /* this page is unmapped, already clean. Not dirty. */
      return 0;
   }

   ASSERT(((va >= (unsigned int)&__swapped_segments_start__)
       && (va <= (unsigned int)&__swapped_segments_end__)));

   idx = ( va - (unsigned int)&__swapped_segments_start__ ) >> PAGE_SHIFT;   
   return MAPPED_DIRTY == dlpager_pagesm.table[idx];
}

void dlpager_pagesm_handle_event( dlpager_event_t event, unsigned int fault_addr, qurt_thread_t task_id )
{
   dlpager_page_state_t state_begin = INVALID_STATE;
   dlpager_page_state_t state_end   = INVALID_STATE;
   unsigned int page_idx;
   int page_mutex;

   page_idx = ( fault_addr - (unsigned int)&__swapped_segments_start__ ) >> PAGE_SHIFT;

   page_mutex = pagesm_pagemutex_get( fault_addr );
   pagesm_pagemutex_lock( page_mutex );

   state_begin = dlpager_pagesm.table[page_idx];
   if (state_begin < INVALID_STATE) {
      state_end = next_state[state_begin][event];
   }
   #if PAGESM_LOG_ENABLE
   pagesm_log_write( page_idx, event, state_begin, state_end );
   #endif
   ASSERT( state_begin < INVALID_STATE );
   ASSERT( state_end < INVALID_STATE );
   
   dlpager_debug_handle_start_state( state_begin );
   dlpager_pagesm.table[page_idx] = state_end;
   dlpager_handlers[state_begin][event]( fault_addr, task_id );

   pagesm_pagemutex_unlock( page_mutex );
   pagesm_pagemutex_free( page_mutex );

   dlpager_debug_handle_end_state( state_end );
   dlpager_stats.page_counts[state_begin]--;
   dlpager_stats.page_counts[state_end]++;
   dlpager_stats.transitions[state_begin][state_end]++;
}

int dlpager_pagesm_init(void)
{
   unsigned int table_size;
   unsigned int segment_size, num_global_pages;
   unsigned int i,j, count=0;

   segment_size = (unsigned int)&__swapped_segments_end__ - (unsigned int)&__swapped_segments_start__;
   num_global_pages = segment_size >> PAGE_SHIFT;

   ASSERT(num_global_pages != 0);

   table_size = num_global_pages * sizeof(dlpager_page_state_t);
   dlpager_pagesm.table = (dlpager_page_state_t *) malloc(table_size);
   ASSERT(dlpager_pagesm.table != NULL);
   dlpager_pagesm.count = num_global_pages;

   /* RO and RW pages start as UNMAPPED_CLEAN */
   for(i = (unsigned int)&__swapped_segments_start__, j = 0; i < (unsigned int)&__swapped_segments_bss_start__; i+= PAGE_SIZE, j++)
   {
      dlpager_pagesm.table[j] =  UNMAPPED_CLEAN;
   }
   dlpager_stats.page_counts[UNMAPPED_CLEAN] = j;
   count = j;

   /* BSS pages start as UNMAPPED_BSS */
   for(;i < (unsigned int)&__swapped_segments_end__; i+= PAGE_SIZE, j++)
   {
      dlpager_pagesm.table[j] =  UNMAPPED_BSS;
   }
   dlpager_stats.page_counts[UNMAPPED_BSS] = j-count;
  
   qurt_pimutex_init( &dlpager_pagesm.mutex );
   pagesm_pagemutex_init();
 
   return 0;
}

void dlpager_pagesm_reset_rx (void)
{
    unsigned int i, j;

    /* @warning No need to lock page state-machine as this function is expected
       to be invoked in single threaded mode (STM) */

    /* Clear-out RX pages */
    for ( i = (unsigned int)&__swapped_segments_text_start__, j = 0; i < (unsigned int)&__swapped_segments_rodata_end__; i += PAGE_SIZE, j++ )
    {
       dlpager_pagesm.table[j] =  UNMAPPED_CLEAN;
    }
}
