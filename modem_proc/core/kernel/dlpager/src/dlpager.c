/*==========================================================================
 * FILE:         dlpager.c
 *
 * SERVICES:     DL PAGER
 *
 * DESCRIPTION:  This file provides the implementation of pager service
 *               initialization and creation of pager thread. It servers as an
 *               interface to Kernel.
 *
 * Copyright (c) 2010-2016 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/

/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stringl.h>
#include <assert.h>
#include <qurt.h>
#include <dlpager.h>
#include <dlpager_log.h>
#include <dlpager_params.h>
#include <dlpager_main.h>
#include <dlpager_meta.h>
#include <dlpager_pagesm.h>
#include <dlpager_q6zip_iface.h>
#include <dlpager_swappool.h>
#include <dlpager_types.h>
#include <dlpager_waitlist.h>
#include <dlpager_rwbuffer.h>
#include <dlpager_stats.h>
#include <dlpager_api.h>
#include <qurt_event_restricted.h>
#if defined( ENABLE_Q6ZIP_TEST_FRAMEWORK )
#include <q6zip_test.h>
#endif

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/
#define MAIN_STACK_SIZE 1024

#if defined( DLPAGER_TEST_RESET_RX_SWAP )
#define DLPAGER_TEST_FRONT_END_LOCK_INIT qurt_pimutex_init( &dlpager_test_front_end_mutex )
#define DLPAGER_TEST_FRONT_END_LOCK qurt_pimutex_lock( &dlpager_test_front_end_mutex )
#define DLPAGER_TEST_FRONT_END_UNLOCK qurt_pimutex_unlock( &dlpager_test_front_end_mutex )
#else
#define DLPAGER_TEST_FRONT_END_LOCK_INIT
#define DLPAGER_TEST_FRONT_END_LOCK
#define DLPAGER_TEST_FRONT_END_UNLOCK
#endif

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/
/* Needs to be global for QSH/Q6ZIP coexistence module, CR923057. */
qurt_thread_t dlpager_tid;

/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/
/* Version required by CrashScope. Changing variable name will break CrashScope.*/
static volatile char dlpager_version[] = DLPAGER_VERSION;

#if defined( DLPAGER_TEST_RESET_RX_SWAP )
/** @brief Mutex to serialize DL pager front-end
    @warning For *TESTING PURPOSES* only! */
qurt_mutex_t dlpager_test_front_end_mutex;
#endif

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/
static dlpager_event_t to_dlpager_event( unsigned int ssr_cause, unsigned int fault_addr )
{
   dlpager_event_t event = INVALID_EVENT;

   switch( ssr_cause )
   {
      case 0x60:
      case 0x61:
         if( dlpager_meta_is_addr_in_text_range(fault_addr) )
         {
            event = TLB_MISS_X;
         }
         break;

      case 0x70:
         if( dlpager_meta_is_addr_in_text_or_rodata_range(fault_addr) ||
             dlpager_meta_is_addr_in_rwdata_range(fault_addr) )
         {
            event = TLB_MISS_R;
         }
         break;

      case 0x71:
      case 0x23:
         if( dlpager_meta_is_addr_in_rwdata_range(fault_addr) )
         {
            event = TLB_MISS_W;
         }
         break;

      default:
         event = INVALID_EVENT;
         break;
   }

   ASSERT( INVALID_EVENT != event );
   return event;
}

static void dlpager_main( void *arg0 )
{
   qurt_sysevent_pagefault_t pf_data;
   dlpager_event_t event;
   unsigned int rtn_val;

   while( 1 )
   {
      rtn_val = qurt_exception_wait_pagefault( &pf_data );
      ASSERT( QURT_EOK == rtn_val );

      DLPAGER_TEST_FRONT_END_LOCK;

      /* Doing this with the knowledge that kernel returns ssr_cause as (ssr & 0xFF)
         and the upper bits of thread_id have not been used by paged tasks, yet. */
      DLPAGER_TASKLOG_WRITE_L1( DLP_LOG_FAULT, ((pf_data.ssr_cause << 24) | pf_data.thread_id), pf_data.fault_addr );
      #if DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING
      dlpager_stats_faultresumetimer_start( pf_data.thread_id );
      #endif
      #if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
      dlpager_stats_fault_to_ipa_start( pf_data.thread_id );
      #endif

      event = to_dlpager_event( pf_data.ssr_cause, pf_data.fault_addr );
      if( INVALID_EVENT != event )
      {
         dlpager_pagesm_handle_event( event, DLPAGER_ALIGN_DOWN(pf_data.fault_addr, PAGE_SIZE), pf_data.thread_id );
      }

      DLPAGER_TEST_FRONT_END_UNLOCK;
   }
}

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/
int dlpager_init( void )
{
   qurt_thread_attr_t thread_attr;
   int status, i;
   void *pStack;
  
   static unsigned int dlpager_initted_flag = 0;
   if( dlpager_initted_flag )
   {
      return 0;
   }
   else
   {
      dlpager_initted_flag++;
   }

   if( &__swapped_segments_start__ == &__swapped_segments_end__ )
   {
      return 0;
   }

   /* Dummy memscpy to ensure compiler optimizations do not remove symbol. */
   i = strlen( DLPAGER_VERSION );
   memscpy( (void*)&dlpager_version, i, DLPAGER_VERSION, i );

   DLPAGER_TEST_FRONT_END_LOCK_INIT;

   /* meta init must happen before swappool init if we want to reclaim rw compressed section 
     for swappool */
   (void)dlpager_meta_init();
   (void)dlpager_pagesm_init();  
   (void)dlpager_q6zip_init();
   (void)dlpager_swappool_init();
   (void)dlpager_waitlist_init();
   (void)dlpager_rwbuffer_init();
   #if defined( ENABLE_Q6ZIP_TEST_FRAMEWORK )
   q6zip_test_init();
   #endif


   /* Creates the main demand paging thread */
   qurt_thread_attr_init( &thread_attr );
   pStack = malloc( MAIN_STACK_SIZE );
   ASSERT( pStack != NULL );
   qurt_thread_attr_set_name( &thread_attr, "DLPager_main" );
   qurt_thread_attr_set_stack_addr( &thread_attr, pStack );
   qurt_thread_attr_set_stack_size( &thread_attr, MAIN_STACK_SIZE );
   qurt_thread_attr_set_priority( &thread_attr, DLPAGER_MAIN_PRIO );
   status = qurt_thread_create( &dlpager_tid, &thread_attr, dlpager_main, (void*)0 );
   ASSERT( 0 == status );
  
   return 0;
}

static inline void dlpager_reset_rx_i (dlpager_swap_reset_result_t * result)
{
    unsigned long long start_tick;
    start_tick = qurt_sysclock_get_hw_ticks();
    result->num_pages_reset = dlpager_swappool_rx_reset();
    dlpager_pagesm_reset_rx();
    result->duration = qurt_sysclock_get_hw_ticks() - start_tick;
}

void dlpager_reset_rx (void)
{
    dlpager_swap_reset_result_t unused;
    dlpager_reset_rx_i( &unused );
}

#if defined( DLPAGER_TEST_RESET_RX_SWAP )
void dlpager_test_reset_rx (dlpager_swap_reset_result_t * result)
{
    DLPAGER_TEST_FRONT_END_LOCK;

    /* Wait for outstanding Q6Zip operations to complete */
    while ( dlpager_q6zip_has_outstanding_operations() )
    {
        ;
    }

    dlpager_reset_rx_i( result );

    DLPAGER_TEST_FRONT_END_UNLOCK;
}
#endif
