/*==========================================================================
 * FILE:         dlpager_q6zip_iface.c
 *
 * SERVICES:     DL PAGER Q6ZIP INTERFACE
 *
 * DESCRIPTION:  This file implements functionality for scheduling compressions/decompression with q6zip module
 *
 * Copyright (c) 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who          what, where, why
--------   --------     ----------------------------------------------------------
12/09/14   rc,rr,cp     First version
===========================================================================*/

#include <q6zip_clk.h>
#include <q6zip_iface.h>
#include <q6zip_ipa.h>
#include <q6zip_sw.h>
#include <q6zip_module.h>
#include <q6zip_context.h>
#include <q6zip_worker.h>
#include <dlpager.h>
#include <dlpager_params.h>
#include <dlpager_types.h>
#include <dlpager_log.h>
#include <dlpager_meta.h>
#include <dlpager_pagesm.h>
#include <dlpager_swappool.h>
#include <dlpager_waitlist.h>
#include <dlpager_stats.h>
#include <dlpager_handlers.h>
#include <dlpager_debug.h>
#if defined( ENABLE_Q6ZIP_IPA )
#include <mcfg_mmu.h>
#endif
#include <q6zip_request_private.h>

//TODO: should we depend on interface or implementation?
#include <q6zip_uncompress.h>
#include <assert.h>


struct {
  int initialized;
  qurt_sem_t outstanding_transactions;  
  qurt_sem_t icache_clean_completed;  
}dlpager_q6zip;

/** @brief Q6ZIP implementation to use */
static q6zip_iface_t const * q6zip_impl;

void dlpager_q6zip_schedule_decompression(unsigned int fault_page_va, unsigned int thread_id)
{
  q6zip_request_t * request;
  dlpager_swappool_page_info_t fault_page_info;
  dlpager_iovec_t src_info;
  q6zip_error_t error;
  q6zip_iface_t const * actual_q6zip_impl = q6zip_impl;

  while(! dlpager_q6zip.initialized);

  request = actual_q6zip_impl->get_request();

  if ( !request )
  {
      /* Try SW */
      request = Q6ZIP_SW.get_request();
      actual_q6zip_impl = &Q6ZIP_SW;
  }

  ASSERT(request != NULL);
  
  src_info = dlpager_meta_get_compressed_block_addr(fault_page_va);
  dlpager_swappool_lookup(fault_page_va, &fault_page_info);
   
  request->src.ptr      = (void *)src_info.addr;
  #if defined( DLPAGER_PROVIDES_PHYS_ADDR_TO_Q6ZIP )
  request->src.phys_ptr = 0;
  #endif
  request->src.len      = src_info.len;
  request->dst.ptr      = (void *)fault_page_info.va_swap_rw;
  #if defined( DLPAGER_PROVIDES_PHYS_ADDR_TO_Q6ZIP )
  request->dst.phys_ptr = (void *)fault_page_info.pa;
  #endif
  request->dst.len      = PAGE_SIZE;

  if (dlpager_meta_is_addr_in_text_or_rodata_range(fault_page_va))
     request->algo = Q6ZIP_ALGO_UNZIP_RO;
   else if (dlpager_meta_is_addr_in_rwdata_range(fault_page_va))
     request->algo = Q6ZIP_ALGO_UNZIP_RW;
   else ASSERT(0);

  request->priority = Q6ZIP_FIXED_PRIORITY_HIGH;

  request->user.fault_addr = fault_page_va;
  request->user.thread_id = thread_id;
  
  request->hint = Q6ZIP_ALGO_HINT_NONE;
  if( dlpager_meta_is_addr_in_text_range(fault_page_va) )
  {
    request->hint = Q6ZIP_ALGO_HINT_UNZIP_RO_TEXT;
  }

  qurt_sem_up(&dlpager_q6zip.outstanding_transactions);
  error = actual_q6zip_impl->submit( &request );
  ASSERT( !error );
}

void dlpager_q6zip_schedule_compression(unsigned int src_page_va, dlpager_iovec_t rw_buffer, dlpager_priority_t prio)
{
  q6zip_request_t * request;
  dlpager_swappool_page_info_t src_page_info;
  q6zip_error_t error;
  q6zip_iface_t const * actual_q6zip_impl = q6zip_impl;

  #if defined( DLPAGER_RWC_IN_Q6ZIP_SW )
  /* In this path, *ALL* compressions are forced to use Q6Zip SW adapter */
  actual_q6zip_impl = &Q6ZIP_SW;
  #else
  /* In this path, Q6Zip adpater selection depends on priority of request */
  if ( prio == HIGH_PRIORITY )
  {
    /* High priority requests always uses the currently selected Q6Zip
       adapter implementation */
    actual_q6zip_impl = q6zip_impl;
  }
  else
  {
    /* Low priority requests *ALWAYS* uses Q6Zip SW. This is so that if
       Q6Zip IPA is being used, we dont delay a high priority request
       due to Q6Zip IPA handling a low priority ahead of it */
    actual_q6zip_impl = &Q6ZIP_SW;
  }
  #endif

  /* First attempt to get a Q6Zip request from the actual implementation that
     was selected above */
  request = actual_q6zip_impl->get_request();
  if ( Q6ZIP_UNLIKELY( !request ) )
  {
    /* Since it failed, try to get Q6Zip request from Q6Zip SW adapter */
    request = Q6ZIP_SW.get_request();
    actual_q6zip_impl = &Q6ZIP_SW;
  }

  /* @warning We should always get a request! If the assertion (below) fails
     it is likely that Q6Zip request buffers are not adequately sized */
  ASSERT(request != NULL);
  
  dlpager_swappool_lookup(src_page_va, &src_page_info);
  
  request->src.ptr      = (void *)src_page_info.va_swap_rw;
  #if defined( DLPAGER_PROVIDES_PHYS_ADDR_TO_Q6ZIP )
  request->src.phys_ptr = (void *)src_page_info.pa;
  #endif
  request->src.len      = PAGE_SIZE;
  request->dst.ptr      = (void *)rw_buffer.addr;
  #if defined( DLPAGER_PROVIDES_PHYS_ADDR_TO_Q6ZIP )
  request->dst.phys_ptr = 0;
  #endif
  request->dst.len      = rw_buffer.len;
  request->algo         = Q6ZIP_ALGO_ZIP_RW;
  request->priority = prio == HIGH_PRIORITY? Q6ZIP_FIXED_PRIORITY_HIGH : Q6ZIP_FIXED_PRIORITY_LOW;

  request->user.fault_addr = src_page_va;
  qurt_sem_up(&dlpager_q6zip.outstanding_transactions);

  error = actual_q6zip_impl->submit( &request );
  ASSERT( !error );
}

void dlpager_q6zip_complete_cb(q6zip_request_t * request, q6zip_response_t const * response)
{
  dlpager_event_t event;
  unsigned int page_va;
  unsigned int recompress_count_index;

  ASSERT( !response->error ); 
  
  event = INVALID_EVENT;
  page_va = request->user.fault_addr;

  switch(request->algo)
  {
    case Q6ZIP_ALGO_UNZIP_RO:
       dlpager_stats.rox_decompressions++;  
       event = DECOMPRESSION_COMPLETE;
       #if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
       dlpager_stats_ipa_to_resume_start( 0 );
       #endif
       break;
    case Q6ZIP_ALGO_UNZIP_RW:
       dlpager_stats.rw_decompressions++;
       event = DECOMPRESSION_COMPLETE;
       #if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
       dlpager_stats_ipa_to_resume_start( 0 );
       #endif
       break;
    case Q6ZIP_ALGO_ZIP_RW:
       dlpager_stats.rw_compressions++;
       dlpager_waitlist_set_rwbuffer_len(page_va, response->len); 
       event = COMPRESSION_COMPLETE;
       if(dlpager_osam_debug_enabled)
       {
          recompress_count_index = (page_va - ((unsigned int)&__swapped_segments_rwdata_start__)) >> PAGE_SHIFT;
          dlpager_stats.recompress_count[recompress_count_index]++;
       }
       break;
    default:
     ASSERT(0); 
  }  

  /* For decompression, update the loader that was used */
//  if ( event == DECOMPRESSION_COMPLETE )
//  {
//      q6zip_context_t const * context = q6zip_request_private_get_context( request );

      /* @warning request->user.other is the DL pager tasklog index associated
         with this request. It is set in dlpager_q6zip_schedule_decompression()*/
//      dlpager_tasklog_set_loader( 
//          (unsigned int) request->user.other,
//          context->impl == Q6ZIP_IMPL_SW ? DLP_LOADER_Q6ZIP_SW : DLP_LOADER_Q6ZIP_IPA );
//  }
  
  dlpager_pagesm_handle_event( event, page_va, THREAD_NULL );
  q6zip_request_free(&request);

  qurt_sem_down(&dlpager_q6zip.outstanding_transactions);
  if( 0 == qurt_sem_get_val(&dlpager_q6zip.outstanding_transactions) )
  {
    dlpager_swappool_perform_mru();
    dlpager_stats.lru_pages_be++;
  }
}

void dlpager_q6zip_initialized_cb(boolean initialized)
{
   dlpager_q6zip.initialized++;
   q6zip_impl = &Q6ZIP_SW;
}

void dlpager_q6zip_ipa_initialized_cb( boolean initialized )
{
    /* @warning This callback will *NEVER* be invoked if ENABLE_Q6ZIP_IPA is
       *NOT* defined */
#if defined( ENABLE_Q6ZIP_IPA )
    #if !defined( DLPAGER_PROVIDES_PHYS_ADDR_TO_Q6ZIP )
    /* @todo anandj 07/07/2016 Remove this once Q6Zip IPA adapter is modified
       to use the PA provided by DL Pager. */
    qurt_addr_t               vaddr_base;
    unsigned int              size;
    dlpager_swapmem_t const * swapmem;
    unsigned int              num_swapmem;
    unsigned int              i;
    #endif

    ASSERT( initialized );


    #if !defined( DLPAGER_PROVIDES_PHYS_ADDR_TO_Q6ZIP )
    /* @todo anandj 07/07/2016 Remove this once Q6Zip IPA adapter is modified
       to use the PA provided by DL Pager. */

    /* For each swap-pool, find out the swap-memory's starting address (VA) and
       its size and let MCFG know about it. This enables faster translations
       from IPA. This assumes the underlying physical memory is contiguous. */
    dlpager_swappool_rx_get_swapmem( &swapmem, &num_swapmem );
    for ( i = 0; i < num_swapmem; i++ )
    {
        vaddr_base = dlpager_swapmem_get_base_virtaddr( &swapmem[ i ] );
        size = dlpager_swapmem_get_size( &swapmem[ i ] );
        mcfg_mmu_add_entry( vaddr_base, size );
    }

    dlpager_swappool_rw_get_swapmem( &swapmem, &num_swapmem );
    for ( i = 0; i < num_swapmem; i++ )
    {
        vaddr_base = dlpager_swapmem_get_base_virtaddr( &swapmem[ i ] );
        size = dlpager_swapmem_get_size( &swapmem[ i ] );
        mcfg_mmu_add_entry( vaddr_base, size );
    }
    #endif

    /* Switch to HW accelerated Q6ZIP */
    q6zip_impl = &Q6ZIP_IPA;
#endif
}

int dlpager_q6zip_init(void)
{
    char * ro_dictionary;
    
    dlpager_q6zip.initialized = 0;
   /* init semaphore that tracks outstanding transactions with q6zip */
    qurt_sem_init_val(&dlpager_q6zip.outstanding_transactions, 0); 

    /* init semaphore that tracks whether icache cleans are performed on pages
       submitted for decompression
       icache clean must be completed before a task on the faulting page is resumed*/
    qurt_sem_init_val(&dlpager_q6zip.icache_clean_completed, 0);

    q6zip_module_init();
    ro_dictionary = dlpager_meta_q6zip_dictionary();
    Q6ZIP_SW.init( 
        ro_dictionary,
        0,
        dlpager_q6zip_initialized_cb,
        dlpager_q6zip_complete_cb );

    #if defined( ENABLE_Q6ZIP_IPA ) && !defined( ENABLE_Q6ZIP_TEST_FRAMEWORK )
    /* Initialize Q6ZIP implementation in IPA */
    Q6ZIP_IPA.init(
        ro_dictionary,
        ( 1 << DICT1_BITS ) * 4,
        dlpager_q6zip_ipa_initialized_cb,
        dlpager_q6zip_complete_cb );
    #endif

  return 0;
}

boolean dlpager_q6zip_has_outstanding_operations (void)
{
    return qurt_sem_get_val( &dlpager_q6zip.outstanding_transactions ) > 0;
}
