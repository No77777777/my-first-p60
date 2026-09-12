/*==========================================================================
 * FILE:         dlpager_log.c
 *
 * SERVICES:     DL PAGER LOG
 *
 * DESCRIPTION:  This file provides the implementation of loggin service for
                 dlpager
 *
 * Copyright (c) 2010-2013 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


==============================================================================*/
/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include <atomic_ops.h>
#include <dlpager_log.h>

/*------------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *----------------------------------------------------------------------------*/
#define DLPAGER_LOG_ENTRIES_BITS  11
#define DLPAGER_LOG_ENTRIES_MASK  ((1<<DLPAGER_LOG_ENTRIES_BITS)-1)
#define DLPAGER_LOG_ENTRIES_MAX   (1<<DLPAGER_LOG_ENTRIES_BITS) /**< No. of log entries */

#if DLPAGER_LOG_VERBOSE_LEVEL > 0

/*------------------------------------------------------------------------------
 * Type Declarations
 *----------------------------------------------------------------------------*/
/*
   These types are chosen simply for readability/formatting to the table within T32.
   wArg1 is an unsigned int so that T32 does not try to display the symbol name of the
   values in that column. wArg2 is a void* for the opposite reason. We want symbol names
   shown for those values because wArg2 is the last column.
*/
typedef struct
{
   unsigned long long dwTicks:56;
   dlpager_tasklog_event_t wEvent:8;
   unsigned int wArg1;
   void *wArg2; //void* allows T32 to display symbol name.
} dlpager_tasklog_entry_t;

/*------------------------------------------------------------------------------
 * Global Data Definitions
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/
static dlpager_tasklog_entry_t dlpager_tasklog[DLPAGER_LOG_ENTRIES_MAX]  __attribute__((aligned(32)));

 /* Roll over to 0 will happen on first atomic increment. */
static atomic_word_t dlpager_tasklog_idx = ATOMIC_INIT(-1);

/*------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *----------------------------------------------------------------------------*/
static unsigned long long get_qtimer( void )
{
   unsigned long long ret;
   asm volatile (" %0 = c31:30 " : "=r"(ret));
   return ret;
}

/*------------------------------------------------------------------------------
 * Externalized Function Definitions
 *----------------------------------------------------------------------------*/
void dlpager_tasklog_write( dlpager_tasklog_event_t wLogEvent, unsigned wArg1, unsigned wArg2 )
{
   unsigned int log_idx = 0;
   unsigned int addr = 0;

   log_idx = atomic_inc_return( &dlpager_tasklog_idx ) & DLPAGER_LOG_ENTRIES_MASK;
   
   dlpager_tasklog[log_idx].dwTicks = get_qtimer();
   dlpager_tasklog[log_idx].wEvent = wLogEvent;
   dlpager_tasklog[log_idx].wArg1 = wArg1;
   dlpager_tasklog[log_idx].wArg2 = (void*)wArg2; //void* allows T32 to display symbol name.

   /* We never read from the log. To be nice to others I will try to clean the cache line
      when I think we are done with it. Eventually, the log buffer needs to be made
      non-cacheable. TODO: Buffer needs to be non-cached somehow. */
   addr = (unsigned int)(&dlpager_tasklog[log_idx]);
   if( 0 != (addr & 0x1F) )
   {
      asm volatile ("dccleaninva(%[addr])" : : [addr] "r" (addr) : "memory" );
   }

   return;
}

#endif /* #if DLPAGER_LOG_VERBOSE_LEVEL > 0 */
