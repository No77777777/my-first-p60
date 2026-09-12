/*==========================================================================
 * FILE:         dlpager_handlers.c
 *
 * SERVICES:     DL PAGER HANDLERS
 *
 * DESCRIPTION:  This file implements handlers for the dlpager page state machine 
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
#include <comdef.h>
#include <assert.h>
#include <qurt.h>
#include <dlpager.h>
#include <dlpager_params.h>
#include <dlpager_swappool.h> 
#include <dlpager_pagesm.h>
#include <dlpager_meta.h>
#include <dlpager_waitlist.h>
#include <dlpager_q6zip_iface.h>
#include <dlpager_log.h>
#include <dlpager_rwbuffer.h>
#include <dlpager_stats.h>
#include <dlpager_debug.h>

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/
static void resume_task( qurt_thread_t task_id )
{
   int result;
   
   DLPAGER_TASKLOG_WRITE_L1( DLP_LOG_TASK_RESUME, task_id, 0 );
   result = qurt_thread_resume( task_id );
   ASSERT( QURT_EOK == result );
   #if DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING
   dlpager_stats_faultresumetimer_stop( task_id );
   #endif
   #if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
   dlpager_stats_ipa_to_resume_stop( task_id );
   #endif
}

static void resume_tasks_from_waitlist(unsigned int page_va, dlpager_event_t event)
{
   dlpager_task_idx_pair_t task_idx_pair;
   unsigned int num_pending_tasks = 0;

   do
   {
      num_pending_tasks = dlpager_waitlist_get_task( page_va, event, &task_idx_pair );
      if( task_idx_pair.task_id != THREAD_NULL )
      {
         resume_task( task_idx_pair.task_id );
      }
   } while( num_pending_tasks > 0 );
}

static boolean dlpager_handler_clean_page( unsigned int page_va, dlpager_priority_t prio )
{
   void *rw_buffer;
   unsigned int page_pa;
   unsigned int rc;
   dlpager_iovec_t rw_iovec;

   rw_buffer = dlpager_rwbuffer_alloc( prio );
   if( !rw_buffer )
   {    
      return 0; /* failure */
   }
   rw_iovec.addr = (unsigned int)rw_buffer;  
   rw_iovec.len = RWBUFFER_SIZE;

   dlpager_waitlist_add_rwbuffer( page_va, rw_iovec );
   dlpager_swappool_rw_get_pa( page_va, &page_pa );
   rc = qurt_mapping_remove( page_va, page_pa, PAGE_SIZE );
   ASSERT(rc == QURT_EOK);

   dlpager_q6zip_schedule_compression( page_va, rw_iovec, prio );

   return 1; /* success */
}

// static void dlpager_handler_try_set_bkpt(unsigned int page_va, unsigned int page_pa)
// {
  // FIXME cporter: need to test this map/unmap approach
  // Three-step process:
  //  1. Map the RX page as RW for the purpose of writing the bkpt
  //  2. Write the breakpoint(s) (if one exists for this page)
  //  3. Remove the mapping and clean
  // unsigned int rc = qurt_mapping_create(page_va, page_pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ | QURT_PERM_WRITE);
  // ASSERT(rc == QURT_EOK);
  // T32_Pager(page_va, 0 /* space */); // write 0 or more bkpts
  // rc = qurt_mapping_remove(page_va, page_pa, PAGE_SIZE);
  // XXX we could avoid flushing and invalidating the entire page by modifying dlpager_t32.c.
  //     from T32_Pager, we could invalidate just the address/es that was/were modified
  // qurt_mem_cache_clean((qurt_addr_t) page_va,
                        // PAGE_SIZE,
                        // QURT_MEM_CACHE_FLUSH_INVALIDATE,
                        // QURT_MEM_DCACHE);
  // ASSERT(rc == QURT_EOK);
// }

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------ 
   Event handlers for UNMAPPED_CLEAN state.
   
   An UNMAPPED_CLEAN page is a page that is not in physical memory.  So, UNMAPPED
   and CLEAN are redundant. There are only 3 events that can happen to an 
   UNMAPPED page, the three TLB misses, eXecute, Read, or Write. Once software
   tries to access the unmapped page, we come here to load the page into memory
   by scheduling a decompression. Once the decompression completes, the back end
   will resume the faulted task in the context of the IPA interrupt handler.
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_x_at_unmapped_clean( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int evicted_pa;
   void * token;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_X_AT_UNMAPPED_CLEAN, task_id, fault_addr );
   evicted_pa = dlpager_swappool_rx_evict_page( &token );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   dlpager_swappool_rx_insert_mapping( fault_addr, evicted_pa, token );
   #if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
   dlpager_stats_fault_to_ipa_stop( task_id );
   #endif
   dlpager_q6zip_schedule_decompression( fault_addr, task_id );
} //Page becomes UNMAPPED_CLEAN_DECOMPRESSING

void dlpager_handler_tlb_miss_r_at_unmapped_clean( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int evicted_pa;
   void * token;

   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_UNMAPPED_CLEAN, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   
   if (dlpager_meta_is_addr_in_rwdata_range(fault_addr)) 
   {
      evicted_pa = dlpager_swappool_rw_evict_page( &token );
      dlpager_swappool_rw_insert_mapping( fault_addr, evicted_pa, token );
   } 
   else 
   {
      evicted_pa = dlpager_swappool_rx_evict_page( &token );
      dlpager_swappool_rx_insert_mapping( fault_addr, evicted_pa, token );
   }
   
   #if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
   dlpager_stats_fault_to_ipa_stop( task_id );
   #endif
   dlpager_q6zip_schedule_decompression( fault_addr, task_id );
   if (dlpager_meta_is_addr_in_rwdata_range(fault_addr)) 
   {
       dlpager_swappool_rw_initiate_soft_clean();
       dlpager_swappool_rw_decrement_clean_page_count();
   }
} //Page becomes UNMAPPED_CLEAN_DECOMPRESSING

void dlpager_handler_tlb_miss_w_at_unmapped_clean( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int evicted_pa;
   void * token;

   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_UNMAPPED_CLEAN, task_id, fault_addr );
   evicted_pa = dlpager_swappool_rw_evict_page( &token );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   dlpager_swappool_rw_insert_mapping( fault_addr, evicted_pa, token );
   #if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
   dlpager_stats_fault_to_ipa_stop( task_id );
   #endif
   dlpager_q6zip_schedule_decompression( fault_addr, task_id );
   dlpager_swappool_rw_initiate_soft_clean();
   dlpager_swappool_rw_decrement_clean_page_count();
} //Page becomes UNMAPPED_DIRTY_DECOMPRESSING


/*------------------------------------------------------------------------------ 
   Event handlers for UNMAPPED_BSS state.
   
   An UNMAPPED_BSS page is a BSS page that has never been loaded into physical
   memory. We come here the first time software accesses the BSS page. This is
   an optimization that takes advantage of the fact that a BSS page is all zeroes.
   We simply clear the page, map it, and resume the task. 

------------------------------------------------------------------------------*/
static void dlpager_handler_tlb_miss_at_unmapped_bss( unsigned int fault_addr, qurt_thread_t task_id, unsigned int perms ) 
{
   dlpager_swappool_page_info_t swappool_page_info;
   unsigned int evicted_pa;
   unsigned int rc;
   unsigned int addr;
   void * token;
   
   dlpager_stats.bss_allocations++;
   evicted_pa = dlpager_swappool_rw_evict_page( &token );
   dlpager_swappool_rw_insert_mapping( fault_addr, evicted_pa, token );
   dlpager_swappool_lookup( fault_addr, &swappool_page_info );
   addr = swappool_page_info.va_swap_rw;
   for( unsigned int i = 0; i < (PAGE_SIZE/32); i++ )
   {
      asm volatile ("dczeroa(%[addr])" : : [addr] "r" (addr) : "memory" );
      addr += 32;
   }
   //qurt_mem_cache_clean( (qurt_addr_t)swappool_page_info.va_swap_rw, 4096, QURT_MEM_CACHE_FLUSH_INVALIDATE, QURT_MEM_DCACHE );
   rc = qurt_mapping_create( fault_addr, evicted_pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, perms );
   ASSERT( QURT_EOK == rc );

   /* @warning Apply any outstanding writes to this faulted page *AFTER* it
      it has been loaded and mapped! */
   if( dlpager_osam_debug_enabled )
   {
      dlpager_debug_apply_outstanding_writes( fault_addr );
   }      

   resume_task( task_id );
   dlpager_swappool_rw_initiate_soft_clean();
}

void dlpager_handler_tlb_miss_r_at_unmapped_bss( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_UNMAPPED_BSS, task_id, fault_addr );
   dlpager_handler_tlb_miss_at_unmapped_bss(fault_addr, task_id, QURT_PERM_READ);
}  // page becomes MAPPED_CLEAN

void dlpager_handler_tlb_miss_w_at_unmapped_bss( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_UNMAPPED_BSS, task_id, fault_addr );
   dlpager_handler_tlb_miss_at_unmapped_bss(fault_addr, task_id, QURT_PERM_READ|QURT_PERM_WRITE);
   dlpager_swappool_rw_initiate_soft_clean();
   dlpager_swappool_rw_decrement_clean_page_count();
} //Page becomes MAPPED_DIRTY


/*------------------------------------------------------------------------------ 
   Event handlers for UNMAPPED_CLEAN_DECOMPRESSING state.
   
   An UNMAPPED_CLEAN_DECOMPRESSING page is a text/ro or rw page that is being
   decompressed. If another TLB miss eXecute or Read/Write occurs then just add that
   software task to the list waiting for decompression to complete for the page
   under decompression. The more interesting event is the actual completion of
   the decompression.
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_x_at_unmapped_clean_decompressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_X_AT_UNMAPPED_CLEAN_DECOMPRESSING, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   dlpager_swappool_rx_mru_page( fault_addr );
}  //Page remains UNMAPPED_CLEAN_DECOMPRESSING

void dlpager_handler_tlb_miss_r_at_unmapped_clean_decompressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_UNMAPPED_CLEAN_DECOMPRESSING, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   if (dlpager_meta_is_addr_in_rwdata_range(fault_addr))
   {
      dlpager_swappool_rw_mru_page( fault_addr );
   }
   else 
   {
      dlpager_swappool_rx_mru_page( fault_addr );
   }
}  //Page remains UNMAPPED_CLEAN_DECOMPRESSING

void dlpager_handler_tlb_miss_w_at_unmapped_clean_decompressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_UNMAPPED_CLEAN_DECOMPRESSING, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   dlpager_swappool_rw_mru_page( fault_addr );  
}  //Page becomes UNMAPPED_DIRTY_DECOMPRESSING

void dlpager_handler_decompression_complete_at_unmapped_clean_decompressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int perms = QURT_PERM_READ;
   unsigned int pa = 0;
   unsigned int rc;

   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_DECOMPRESSION_COMPLETE_AT_UNMAPPED_CLEAN_DECOMPRESSING, fault_addr, 0 );
   if( dlpager_meta_is_addr_in_text_range(fault_addr) )
   {
      perms = ( QURT_PERM_READ | QURT_PERM_EXECUTE );
   }
      
   if (dlpager_meta_is_addr_in_rwdata_range(fault_addr)) {
      dlpager_swappool_rw_get_pa( fault_addr, &pa );
   }
   else 
   {
      dlpager_swappool_rx_get_pa( fault_addr, &pa );
   }
   
   // if( dlpager_osam_debug_enabled )
   // {
      // dlpager_debug_apply_outstanding_writes( fault_addr );
      // if( dlpager_meta_is_addr_in_text_range(fault_addr) && T32_PagerTable[0].action )
      // {
         // dlpager_handler_try_set_bkpt( fault_addr, pa );
      // }
   // }
   rc = qurt_mapping_create( fault_addr, pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, perms );
   ASSERT( QURT_EOK == rc );
   resume_tasks_from_waitlist( fault_addr, DECOMPRESSION_COMPLETE );
} //Page becomes MAPPED_CLEAN



/*------------------------------------------------------------------------------ 
   Event handlers for UNMAPPED_DIRTY_DECOMPRESSING state.
   
   An UNMAPPED_DIRTY_DECOMPRESSING page is a RW page that has already taken a TLB
   write miss and is being decompressed. Another TLB read/write miss just adds that
   software task to the wait for decompression to complete. Otherwise, the only
   valid event is decompression complete.
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_r_at_unmapped_dirty_decompressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_UNMAPPED_DIRTY_DECOMPRESSING, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   dlpager_swappool_rw_mru_page( fault_addr );
} //Page remains UNMAPPED_DIRTY_DECOMPRESSING

void dlpager_handler_tlb_miss_w_at_unmapped_dirty_decompressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_UNMAPPED_DIRTY_DECOMPRESSING, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, DECOMPRESSION_COMPLETE, task_id );
   dlpager_swappool_rw_mru_page( fault_addr );
} //Page remains UNMAPPED_DIRTY_DECOMPRESSING

void dlpager_handler_decompression_complete_at_unmapped_dirty_decompressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int pa = 0;
   unsigned int rc;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_DECOMPRESSION_COMPLETE_AT_UNMAPPED_DIRTY_DECOMPRESSING, fault_addr, 0 );
   (void)task_id;
   if( dlpager_osam_debug_enabled )
   {
      dlpager_debug_apply_outstanding_writes( fault_addr );
   }
   dlpager_swappool_rw_get_pa( fault_addr, &pa );
   rc = qurt_mapping_create( fault_addr, pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, (QURT_PERM_READ|QURT_PERM_WRITE) );
   ASSERT( QURT_EOK == rc );
   resume_tasks_from_waitlist( fault_addr, DECOMPRESSION_COMPLETE );
} //Page becomes MAPPED_DIRTY



/*------------------------------------------------------------------------------ 
   Event handlers for UNMAPPED_DIRTY_COMPRESSING_ALLOCATED state.
   
   An UNMAPPED_DIRTY_COMPRESSING_ALLOCATED page is a dirty RW page that is in
   physical memory, is being compressed, but is not mapped in the page table.

------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_r_at_unmapped_dirty_compressing_allocated( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int rc;
   unsigned int page_pa;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_UNMAPPED_DIRTY_COMPRESSING_ALLOCATED, task_id, fault_addr );
   dlpager_swappool_rw_mru_page( fault_addr );
   dlpager_swappool_rw_get_pa( fault_addr, &page_pa );
   rc = qurt_mapping_create( fault_addr, page_pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ); 
   ASSERT(rc == QURT_EOK);
   resume_task( task_id );
   dlpager_swappool_rw_initiate_soft_clean();
} //Page becomes MAPPED_CLEAN_COMPRESSING

void dlpager_handler_tlb_miss_w_at_unmapped_dirty_compressing_allocated( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int rc;
   unsigned int page_pa;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_UNMAPPED_DIRTY_COMPRESSING_ALLOCATED, task_id, fault_addr );
   dlpager_swappool_rw_mru_page( fault_addr );
   dlpager_swappool_rw_get_pa( fault_addr, &page_pa );
   rc = qurt_mapping_create( fault_addr, page_pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ | QURT_PERM_WRITE); 
   ASSERT(rc == QURT_EOK);
   resume_task( task_id );
   dlpager_swappool_rw_initiate_soft_clean();
} //Page becomes MAPPED_DIRTY_COMPRESSING

void dlpager_handler_compression_complete_at_unmapped_dirty_compressing_allocated( unsigned int fault_addr, qurt_thread_t task_id )
{
   dlpager_iovec_t rw_buffer;

   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_COMPRESSION_COMPLETE_AT_UNMAPPED_DIRTY_COMPRESSING_ALLOCATED, fault_addr, 0 );
   (void)task_id;
   rw_buffer = dlpager_waitlist_get_rwbuffer( fault_addr );
   dlpager_meta_set_compressed_block( fault_addr, rw_buffer );
   dlpager_rwbuffer_free( (void *)rw_buffer.addr, LOW_PRIORITY );
   dlpager_swappool_rw_increment_clean_page_count();
} //Page becomes UNMAPPED_CLEAN_ALLOCATED

void dlpager_handler_soft_clean_failed_at_unmapped_dirty_compressing_allocated( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_SOFT_CLEAN_FAILED_AT_UNMAPPED_DIRTY_COMPRESSING_ALLOCATED, fault_addr, 0 );
   (void)fault_addr;
   (void)task_id;
   --dlpager_stats.soft_cleans;
   ++dlpager_stats.soft_cleans_failed;  
} //Page becomes MAPPED_DIRTY


/*------------------------------------------------------------------------------ 
   Event handlers for UNMAPPED_CLEAN_ALLOCATED state.
   
   An UNMAPPED_CLEAN_ALLOCATED page is a clean RW page that is in physical memory,
   but is not mapped..
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_r_at_unmapped_clean_allocated( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int rc;
   unsigned int pa;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_UNMAPPED_CLEAN_ALLOCATED, task_id, fault_addr );
   dlpager_swappool_rw_mru_page( fault_addr );
   dlpager_swappool_rw_get_pa( fault_addr, &pa );
   rc = qurt_mapping_create( fault_addr, pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ );
   ASSERT(rc == QURT_EOK);
   resume_task( task_id );
   dlpager_swappool_rw_initiate_soft_clean();
   dlpager_swappool_rw_decrement_clean_page_count();
} //Page becomes MAPPED_CLEAN

void dlpager_handler_tlb_miss_w_at_unmapped_clean_allocated( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int rc;
   unsigned int pa;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_UNMAPPED_CLEAN_ALLOCATED, task_id, fault_addr );
   dlpager_swappool_rw_mru_page( fault_addr );
   dlpager_swappool_rw_get_pa( fault_addr, &pa );
   rc = qurt_mapping_create( fault_addr, pa, PAGE_SIZE, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ | QURT_PERM_WRITE );
   ASSERT(rc == QURT_EOK);
   resume_task( task_id );
   dlpager_swappool_rw_initiate_soft_clean();
   dlpager_swappool_rw_decrement_clean_page_count();
} //Page becomes MAPPED_DIRTY

void dlpager_handler_evict_page_at_unmapped_clean_allocated( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_EVICT_PAGE_AT_UNMAPPED_CLEAN_ALLOCATED, fault_addr, 0 );
   (void)task_id;
   dlpager_swappool_rw_remove_mapping( fault_addr );
} //Page becomes UNMAPPED_CLEAN


/*------------------------------------------------------------------------------ 
   Event handlers for UNMAPPED_HARD_CLEAN_COMPRESSING state.
   
   An UNMAPPED_HARD_CLEAN_COMPRESSING page is an rw page that is in physical memory,
   is not mapped, and is being compressed.
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_r_at_unmapped_hard_clean_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_UNMAPPED_HARD_CLEAN_COMPRESSING, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, COMPRESSION_COMPLETE, task_id );
} //Page remains UNMAPPED_HARD_CLEAN_COMPRESSING

void dlpager_handler_tlb_miss_w_at_unmapped_hard_clean_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_UNMAPPED_HARD_CLEAN_COMPRESSING, task_id, fault_addr );
   dlpager_waitlist_add_task( fault_addr, COMPRESSION_COMPLETE, task_id );
} //Page remains UNMAPPED_HARD_CLEAN_COMPRESSING

void dlpager_handler_compression_complete_at_unmapped_hard_clean_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   dlpager_iovec_t rw_buffer;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_COMPRESSION_COMPLETE_AT_UNMAPPED_HARD_CLEAN_COMPRESSING, fault_addr, 0 );
   (void)task_id;
   rw_buffer = dlpager_waitlist_get_rwbuffer( fault_addr );
   dlpager_meta_set_compressed_block( fault_addr, rw_buffer );
   dlpager_rwbuffer_free( (void *)rw_buffer.addr, HIGH_PRIORITY );
   dlpager_swappool_rw_remove_mapping( fault_addr );
   dlpager_swappool_rw_increment_clean_page_count();
   resume_tasks_from_waitlist( fault_addr, COMPRESSION_COMPLETE );
} //Page becomes UNMAPPED_CLEAN


/*------------------------------------------------------------------------------ 
   Event handlers for MAPPED_CLEAN state.
   
   A MAPPED_CLEAN page is a text or RO/RW page that is in physical memory and is
   mapped in the page table. A TLB miss X or R is due to a race condition on the
   front end and back end resume.  Just resume the faulted task. For a TLB W to the
   RW page, remap it to READ|WRITE permisiion 
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_x_at_mapped_clean( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_X_AT_MAPPED_CLEAN, task_id, fault_addr );
   resume_task( task_id );
} //Page remains MAPPED_CLEAN

void dlpager_handler_tlb_miss_r_at_mapped_clean( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_MAPPED_CLEAN, task_id, fault_addr );
   resume_task( task_id );
} //Page remains MAPPED_CLEAN

void dlpager_handler_tlb_miss_w_at_mapped_clean( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int page_pa;
   unsigned int rc;
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_MAPPED_CLEAN, task_id, fault_addr );
   dlpager_swappool_rw_get_pa( fault_addr, &page_pa );
   rc = qurt_mapping_remove( fault_addr, page_pa, PAGE_SIZE );
   ASSERT(rc == QURT_EOK);

   rc = qurt_mapping_create(fault_addr, page_pa, 4096, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ|QURT_PERM_WRITE); 
   ASSERT(rc == QURT_EOK);     
   
   resume_task( task_id );   
   dlpager_swappool_rw_mru_page( fault_addr );
   dlpager_swappool_rw_initiate_soft_clean();
} //Page becomes MAPPED_DIRTY

void dlpager_handler_evict_page_at_mapped_clean( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int rc;
   dlpager_swappool_page_info_t page_info;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_EVICT_PAGE_AT_MAPPED_CLEAN, fault_addr, 0 );
   (void)task_id;
   dlpager_swappool_lookup( fault_addr, &page_info );
   rc = qurt_mapping_remove( fault_addr, page_info.pa, PAGE_SIZE );
   ASSERT( QURT_EOK == rc );
   if (dlpager_meta_is_addr_in_rwdata_range(fault_addr) )
   {
      dlpager_swappool_rw_remove_mapping( fault_addr );
      dlpager_swappool_rw_increment_clean_page_count();
   }
   else
   {
      dlpager_swappool_rx_remove_mapping( fault_addr );
   }
 
   if( dlpager_meta_is_addr_in_text_range(fault_addr) )
   {
      qurt_mem_cache_clean( (qurt_addr_t)page_info.va_swap_rx, PAGE_SIZE, QURT_MEM_CACHE_INVALIDATE, QURT_MEM_ICACHE );
   }
   else
   {
      /* Page must be in .rodata. and Q6ZIP does data cache invalidate. */
   }
} //Page becomes UNMAPPED_CLEAN


/*------------------------------------------------------------------------------ 
   Event handlers for MAPPED_DIRTY state.
   
   A MAPPED_DIRTY page is an RW page that is in physical memory and is mapped.
   For a TLB miss R/W, just resume the faulted task.
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_r_at_mapped_dirty( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_MAPPED_DIRTY, task_id, fault_addr );
   resume_task( task_id );
} //Page remains MAPPED_DIRTY

void dlpager_handler_tlb_miss_w_at_mapped_dirty( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_MAPPED_DIRTY, task_id, fault_addr );
   resume_task( task_id );
} //Page remains MAPPED_DIRTY

void dlpager_handler_soft_clean_page_at_mapped_dirty( unsigned int fault_addr, qurt_thread_t task_id )
{
   boolean success = 0;

   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_SOFT_CLEAN_PAGE_AT_MAPPED_DIRTY, fault_addr, 0 );
   ++dlpager_stats.soft_cleans; // decremented later if soft fails
   success = dlpager_handler_clean_page( fault_addr, LOW_PRIORITY );
   if( !success )
   {
      dlpager_pagesm_handle_event( SOFT_CLEAN_FAILED, fault_addr, task_id );
   }
} // Page becomes UNMAPPED_DIRTY_COMPRESSING_ALLOCATED

void dlpager_handler_hard_clean_page_at_mapped_dirty( unsigned int fault_addr, qurt_thread_t task_id ) 
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_HARD_CLEAN_PAGE_AT_MAPPED_DIRTY, fault_addr, 0 );
   (void)task_id;
   ++dlpager_stats.hard_cleans;
   dlpager_handler_clean_page( fault_addr, HIGH_PRIORITY );
} //Page becomes UNMAPPED_HARD_CLEAN_COMPRESSING


/*------------------------------------------------------------------------------ 
   Event handlers for MAPPED_DIRTY_COMPRESSING state.
   
   A MAPPED_DIRTY_COMPRESSING page is an RW page that is in physical memory, is
   mapped, and is being compressed. 
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_r_at_mapped_dirty_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_MAPPED_DIRTY_COMPRESSING, task_id, fault_addr );
   resume_task( task_id );
} //Page remains MAPPED_DIRTY_COMPRESSING

void dlpager_handler_tlb_miss_w_at_mapped_dirty_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_MAPPED_DIRTY_COMPRESSING, task_id, fault_addr );
   resume_task( task_id );
} //Page remains MAPPED_DIRTY_COMPRESSING

void dlpager_handler_compression_complete_at_mapped_dirty_compressing( unsigned int fault_addr, qurt_thread_t task_id ) 
{
   dlpager_iovec_t rw_buffer;

   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_COMPRESSION_COMPLETE_AT_MAPPED_DIRTY_COMPRESSING, fault_addr, 0 );
   (void)task_id;

   /* Throw away the compression result because we got here due to a write to this page
      in parallel to the compression. So, the compression was performed with stale data. */
   rw_buffer = dlpager_waitlist_get_rwbuffer( fault_addr );
   dlpager_rwbuffer_free( (void *)rw_buffer.addr, LOW_PRIORITY );
} //Page becomes MAPPED_DIRTY

/*------------------------------------------------------------------------------ 
   Event handlers for MAPPED_CLEAN_COMPRESSING state.
   
   A MAPPED_CLEAN_COMPRESSING page is an RW page that is in physical memory, is
   mapped, and is being compressed due to soft clean event. 
------------------------------------------------------------------------------*/

void dlpager_handler_tlb_miss_r_at_mapped_clean_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_MAPPED_CLEAN_COMPRESSING, task_id, fault_addr );
   resume_task( task_id );
} //Page remains MAPPED_CLEAN_COMPRESSING

void dlpager_handler_tlb_miss_w_at_mapped_clean_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int rc;
   unsigned int page_pa;
   
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_MAPPED_CLEAN_COMPRESSING, task_id, fault_addr );
   
   // remap as READ|WRITE
   dlpager_swappool_rw_get_pa( fault_addr, &page_pa );
   rc = qurt_mapping_remove( fault_addr, page_pa, PAGE_SIZE );
   ASSERT(rc == QURT_EOK);

   rc = qurt_mapping_create(fault_addr, page_pa, 4096, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ|QURT_PERM_WRITE); 
   ASSERT(rc == QURT_EOK);     
   
   resume_task( task_id );
   dlpager_swappool_rw_mru_page( fault_addr );
} // Page becomes MAPPED_DIRTY_COMPRESSING

void dlpager_handler_compression_complete_at_mapped_clean_compressing( unsigned int fault_addr, qurt_thread_t task_id )
{
   dlpager_iovec_t rw_buffer;

   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_COMPRESSION_COMPLETE_AT_MAPPED_CLEAN_COMPRESSING, fault_addr, 0 );
   (void)task_id;
   rw_buffer = dlpager_waitlist_get_rwbuffer( fault_addr );
   dlpager_meta_set_compressed_block( fault_addr, rw_buffer );
   dlpager_rwbuffer_free( (void *)rw_buffer.addr, LOW_PRIORITY );
   // dlpager_swappool_rw_increment_clean_page_count();
} //Page becomes MAPPED_CLEAN

/*------------------------------------------------------------------------------ 
   Event handlers for MAPPED_CLEAN_BSS state.
   
   A MAPPED_CLEAN_BSS page is an bss page that is in physical memory, is
   mapped. For a TLB miss R, just resume the task, For TLB W miss, Reamp the page as 
   READ|WRITE and resume the task   
------------------------------------------------------------------------------*/
void dlpager_handler_tlb_miss_r_at_mapped_clean_bss( unsigned int fault_addr, qurt_thread_t task_id )
{
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_R_AT_MAPPED_CLEAN_BSS, task_id, fault_addr );
   dlpager_swappool_rw_mru_page( fault_addr );
   resume_task( task_id );    
}

void dlpager_handler_tlb_miss_w_at_mapped_clean_bss( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int page_pa;
   unsigned int rc;
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_MISS_W_AT_MAPPED_CLEAN_BSS, task_id, fault_addr );
   
   // remap as READ|WRITE
   dlpager_swappool_rw_get_pa( fault_addr, &page_pa );
   rc = qurt_mapping_remove( fault_addr, page_pa, PAGE_SIZE );
   ASSERT(rc == QURT_EOK);

   rc = qurt_mapping_create(fault_addr, page_pa, 4096, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ|QURT_PERM_WRITE); 
   ASSERT(rc == QURT_EOK);     
   
   resume_task( task_id );
   dlpager_swappool_rw_mru_page( fault_addr );
   dlpager_swappool_rw_initiate_soft_clean();
   dlpager_swappool_rw_decrement_clean_page_count();
}

void dlpager_handler_evict_page_at_mapped_clean_bss( unsigned int fault_addr, qurt_thread_t task_id )
{
   unsigned int page_pa;
   unsigned int rc;
   DLPAGER_TASKLOG_WRITE_L2( DLP_LOG_EVICT_PAGE_AT_MAPPED_CLEAN_BSS, fault_addr, fault_addr );
   (void)task_id;
   
   dlpager_swappool_rw_get_pa( fault_addr, &page_pa );
   rc = qurt_mapping_remove( fault_addr, page_pa, PAGE_SIZE );
   ASSERT(rc == QURT_EOK);
   
   dlpager_swappool_rw_remove_mapping( fault_addr );
}
