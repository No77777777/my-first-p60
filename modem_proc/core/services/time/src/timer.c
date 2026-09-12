/*=============================================================================

                  T I M E R   S E R V I C E   S U B S Y S T E M

GENERAL DESCRIPTION
  Implements time-keeping functions using the slow clock.

  Slow Clock Frequency          Granularity     Timer Range
    30.000kHz (lower limit)      33.3 us         39.7 hrs
    32.768kHz (nominal freq)     30.5 us         36.4 hrs
    60.000kHz (upper limit)      16.6 us         19.8 hrs


REGIONAL FUNCTIONS 

  timer_init()
    Initializes the timer subsytem.  Call only from time_init( ).


EXTERNALIZED FUNCTIONS

  timer_def(timer, group, tcb, sigs, func, data)
    Defines and initializes a timer.

  timer_def_priority(timer, group, tcb, sigs, func, data, timer_prio)
    Same as timer_def, but with an option to defnie the timer as a priority timer also.

  timer_def2(timer, group, func, data)
    Defines and initializes a timer, with the clock callback semantics.

  timer_def2_priority(timer, group, func, data, timer_prio)
    Same as timer_def2, but with an option to defnie the timer as a priority timer also.

  timer_def_osal(timer,group,sigs_obj,sigs,func,data)
    Defines and initializes a timer. Provision of using a signal object for notification.

  timer_def_osal_priority(timer,group,sigs_obj,sigs,func,data,timer_prio)
    Same as timer_def_osal(), with added option to specify the timer priority.

  timer_reg(timer,func,data,time_ms,reload_ms)
    Complimentary function to timer_def2. Sets the timer as per the passed params.
    
  timer_set(timer, ticks, reload, unit)
    Sets an inactive timer to expire after a given period of time, or changes
    an active timer to expire after a given period of time.
    Optionally, specifies the timer to repeatly expire with a given period.

  timer_set_64(timer, ticks, reload, unit)
    Same as timer_set, but this API also supports the 64 bit timetick values @ 19.2 Mhz as an input.

  timer_set_absolute(timer,time)
    Sets an inactive timer to expire at the given time, or changes an active timer to expire at that time.
  
  timer_get(timer, unit)
    Get number of ticks before timer expires

  timer_get_64(timer, unit)
    Same as timer_get(), but with support for 64 bit timetick values @ 19.2 Mhz.

  timer_get_params(timer,timer_attrib_struct_ptr)
    Fills up the passed timer attribute structure with timer parameters.

  timer_set_params(timer,to_set,timer_attrib_struct_ptr)
    Updates the timer parameter based on the passed arguments.

  timer_is_active(timer)
    Returns boolean TRUE / FALSE based on the timer active status.

  timer_expires_at_64(timer)
    Returns the 19.2 Mhz, 64 bit timetick val for the timer's expiry if it is active 

  timer_expires_at(timer)
    Returns the slow clock based timetick val for the timer's expiry if it is active

  timer_get_start_64(timer)
    Return the 19.2 Mhz based 64 bit timetick value when the timer was started.

  timer_get_start(timer)
    Return the slow clock based timetick value when the timer was started.

  timer_pause(timer)
    Suspends an active timer

  timer_resume(timer)
    Resumes a previously "paused" active timer.

  timer_clr(timer, unit)
    Stops an active timer and return the remaining slow clock based timeticks before expiry.

  timer_clr_64(timer, unit)
    Stops an active timer and return the remaining 19.2 Mhz based 64 bit timeticks before expiry.

  timer_stop(timer,unit,rem_time)
    Stops an active timer and returns the remaining ticks before expiry in the passed argument.

  timer_undef(timer)
    Stops an active timer and also deallocates any internal memory reserved for the timer.
      
  timer_clr_task_timers(tcb)
    Stops all active timers which belong to the specified task.

  timer_clr_osal_thread_timers(thread_id)
    Stops all active timers which belong to the specified thread.
   
  timer_group_enable(timer_group)
    Enables a timer group.  Timers in the timer group which expired during the
    diabled period will have their expiry processing executed.

  timer_group_disable(timer_group)
    Disables a timer group.  Timers in the timer group will continue to count
    down, but they will not expire.

  timer_group_set_deferrable(pgroup,condition)
    Sets the passed group property as deferrable or non-deferrable



INITIALIZATION AND SEQUENCING REQUIREMENTS

    timer_init() must be called once before any other timer functions.

    timer_def() must be called, or the TIMER_DEF( ) macro used, for each
      timer, before other timer functions are used with that timer.

    A valid sclk estimate is needed before timers can be set.


Copyright (c) 2003 - 2017 by QUALCOMM Technologies Incorporated.  All Rights Reserved.

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/time/src/timer.c#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
22/02/17   abh     Initialize malloc memory for chunks to avoid corruption.
09/18/15   abh    Optimization changes in chunks traversal logic.
05/20/14   abh    Added changes to support MultiPD on MPSS
08/30/13   abh    New API additions and adding error return types to the existing ones, and some changes.
04/26/11   din     Fixed compiler warnings.
03/11/11   ab      Removed compiler warnings.
03/10/11   ab      Fix a corner case when a small expiry timer is inserted 
                   immediately after a large expiry timer.
11/12/10   din     Added function to get time to next occuring non-deferrable 
                   timer expiry.
08/25/10   din     Removed call to timetick_get_sclk_offset in timer_expires_in.
04/27/10   din     Fixed compiler warnings.
12/18/09   din     Adding Timer List debugging feature.
12/10/09   din     Timer expiry now caters to changing sclk_delta
09/30/09   sri     Enhanced QXDM strings on timer_group NULL
05/22/09   din     Added missing refernce of zero in timer_defer_match_interrupt 
05/22/09   din     Moved implementation of timer_set_next_interrupt to 
                   timetick_set_next_interrupt.
05/22/09   din     Removed FEATURE_TIMER_TASK. Removed Function timer_isr.
01/16/09   pbi     Moved MIN_REARM_SCLK definition to timetick.c
01/07/09   pbi     sclk_delta removed from timers_type
01/07/09   pbi     Removed timer_set_sclk_offset
03/25/09   din     In timer_expire(), cached callback functions from timer 
                   structure before first callback is made.
03/04/09   din     timer_defer_match_interrupt returns zero when expiry of first 
                   non deferrable timers is in past.
02/23/09   din     Consoldation of time.
01/21/09   din     Forced timer_undefer_match_interrupt() to update MATCH_VAL
                   register. Updated timer_process_active_timers() to prevent
                   overflow in timer expiry past the active timer list zero.
01/16/09   din     Added missing reset of last_set_time in timer_restart().
01/13/09   din     Removed timer_delay_expiry_excluding_groups() and
                   timer_undelay_expiry().
01/12/09   din     Added deferrable attribute to timer_group. Also added 
                   lightweight API's to enable/ disable timer groups. API's
                   timer_defer_match_interrupt(), timer_undefer_match_interrupt
                   & timer_group_set_deferrable().
01/05/09   din     Consolidation of time sources. Added 
                   FEATURE_TIME_UMTS_TARGETS. timer_set_next_interrupt was
           made uniform acroos different targets. Also Ensured that 
           timer_get() doesnot return 0.
12/23/08   tev     Remove timer_v.h inclusion.  No longer required now that
                   timer_v.h is obsolete.
12/15/08   tev     Explicitly include newly created private header file
                   timer_v.h
11/20/08   din     Always set set_time when match_value is updated in 
                   timer_set_next_interrupt(). Every attempt to set is recorded 
                   in last_set_time.
11/14/08   tev     Amended control for timer_restart to
                   defined(FEATURE_TIME_POWER_COLLAPSE) &&
                   !defined(FEATURE_TIME_REMOVE_TIMER_RESTART)
11/10/08   tev     Tentatively restored timer_restart under control of
                   FEATURE_TIME_POWER_COLLAPSE symbol definition.
10/17/08   din     Fixed initialization of do_reprogram_isr
10/16/08   din     Added lightweight mechanism to enable and disable timer groups 
                   during sleep. For use by the sleep subsystem only! APIs are
                   timer_delay_expiry_excluding_groups(), and timer_undelay_expiry()
                   (renamed timer_restart().) 
09/19/08   trc     timer_bin_find_first_exceeding() returns NULL upon failure
06/10/08   trc     Upon re-enabling a timer group, only nudge the active timer
                   list zero point as far back as the earliest expired timer
11/13/07   jhs     Removed timer_always_on_group.
08/31/07   jhs     Support for AMSS RESTART
05/29/07   jhs     Added in support to rewrite MATCH when writing match over a
                   tick boundary if FEATURE_TIME_REQUIRES_ODD_WRITE is defined.
04/19/07   gfr     Support for timer tracing.
01/12/07   trc     Increase MIN_REARM_SCLK from 6 to 9 slow clock cycles
12/04/06   trc     Bump up the # of timer bins from 64 to 128
10/11/06   trc     Reset match_value and set_value when sclk_delta is set
                   coming out of power collapse since they are no longer valid.
08/19/06   gb      Changed FEATURE_POWER_COLLAPSE to FEATURE_TIME_POWER_COLLAPSE. Made
                   changes for modem power collapse code.
08/15/06   jhs     Clear out match_value and set_value when sclk_delta is set
                   because they are no longer valid.
04/10/06   ajn     Prevent rounding nudge from creating a negative runtime.
04/07/06   gfr     Remove unneeded header file (msm_drv.h)
03/01/06   kap     Only unbin when timer->bin field is non-null
02/22/06   ajn     Add offset btw modem & MPM sclk counter for power collapse
02/10/06   kap/ajn Added Timer Optimization Changes (double linking/binning)
12/20/05   ejv     Added T_QSC60X0.
11/03/05   ajn     Zero-initialize the timer.cache fields in timer_define()
10/27/05   ajn     Always set set_time in timer_set_next_interrupt().
08/12/05   jhs     Moved apps processor to its own caching assembly function
                   for writing to MATCH_VAL.
08/05/05   ajn     MATCH_VAL written from caching assembly function.
07/29/05   ajn     Repeat MATCH_VAL writes until the write latches.
07/22/05   ajn     Don't write new MATCH_VAL until last write has latched.
07/06/05   ajn     Added timer_set_modem_app_sclk_offset (7500 APPS)
06/24/05   ajn     Include partial 7500 support
06/09/05   ajn     Corrected non-timer_task initialization.
05/19/05   ajn     Timer op's don't signal task or call process_active_timers.
05/09/05   ajn     Don't write to TIMETICK_MATCH_VAL if value is unchanged.
                   Fixed O(N^2) list insertion in timer_group_enable().
05/06/05   ajn     Cache time/reload values to avoid recalculating if unchanged
03/29/05   ajn     Replaced timer_ptr with timer_ptr_type, for L4 project.
03/16/05   ajn     Update next timer expiry interrupt w/o processing timers
                   when timers are removed from the active list.
03/16/05   ajn     Add ms rounding to following expiry in repeating timers.
12/06/04   lcc     In timer_process_active_timers, added reading of timetick
                   count to make sure it's non-zero before existing.  This is
                   needed for QUASar's timetick hw problem.
10/20/04   ajn     Moved timer expiry out of ISR context into a Timer task.
09/28/04   ajn     Catch use of timers w/o call to timer_define().
09/27/04   ajn     Force timer in the NULL timer group into an actual group.
08/30/04   ajn     Added "get next interrupt" and "interrupt early" functions
                   for TCXO shutdown.
07/06/04   ajn     timer_define() ERR's (not ERR_FATAL's) on active timer.
06/23/04   ajn     Corrected re-entrancy when processing active timers.
01/29/04   ajn     Corrected possible NULL pointer dereference.
01/09/04   ajn     Optimized the expiry-time advance of auto-reloading timers.
11/14/03   ajn     Added sclk timestamps for profiling timer_isr latency.
11/11/03   ajn     Added checks to detect timer_def( ) calls on active timers
10/03/03   ajn     Call added to process_active_timers() in group_disable, to
                   ensure next active timer's interrupt occurs at proper time.
08/11/03   ajn     Added timer_pause/timer_resume for rex_set_timer replacement
08/08/03   ajn     Updated for coding standards
08/06/03   ajn     Added timer_def2(), for clk_reg() replacement
07/23/03   ajn     Moved _{to,from}_sclk functions to timetick.c
07/17/03   ajn     File created.

=============================================================================*/



/*=============================================================================


  *** NOTE *** NOTE *** NOTE *** NOTE *** NOTE *** NOTE *** NOTE *** NOTE ***

  
  All comparisons in this module are done with respect to a moving origin,
  or "zero" point, to handle wrap-around of the 32-bit slow clock counter.
  
  For example, the conditional statement "if ( now >= expiry )" is wrong,
  since "now" could be 0xFFFFFF00, and "expiry" could be "0x00000100", ...
  "0x200" counts in the "future".
  
  The correct comparison would be "if ( now - zero >= expiry - zero )".
  This compares the current time (measured from some point in the past) to
  the expiry time (measured with respect to the same point in the past).
  If the "zero" origin was "0xFFFFFE00", with the above values, "now - zero"
  would be "0x100" and "expiry - zero" would be "0x300", making the entire
  expression FALSE; the expiry point has not been reached.

  If the expression "now - zero >= expiry - zero" returns TRUE, then it is
  safe to subtract "now - expiry" to determine how much time has passed since
  the expiry point.  If "now - zero >= expiry - zero" returns FALSE, the
  reverse subtraction is valid: "expiry - now" indicates how much time
  remains until the expiry point.

=============================================================================*/


/*=============================================================================

                           INCLUDE FILES

=============================================================================*/

#include "comdef.h"
#include "ats_v.h"
#include "timer_v.h"
#include "err.h"
#include "assert.h"
#include "stdint.h"
#include "qurt_signal2.h"
#include "DALSys.h"
#include "rlc.h"

#ifndef FEATURE_TIMER_USE_QURT_SYSCLOCK
#include "DDIInterruptController.h"
#include "DALStdErr.h"
#include "DALDeviceId.h"
#include "DalDevice.h"
#endif /*FEATURE_TIMER_USE_QURT_SYSCLOCK*/
#ifdef FEATURE_TIME_QDSP6
#include "qurt.h"
#endif /*FEATURE_TIME_QDSP6*/

#if defined(FEATURE_TIMER_APQ8064)
#define TRAMP_SLP_TICK_ISR TRAMP_GP_TIMER_IRQ
#endif

#include <stringl/stringl.h>
#include "timer_fn_map.h"
#include "rcevt.h"
#include "rcinit.h"
#include "rcinit_rex.h"
#include "queue.h"

#include "timer_slaves.h"
#include "DDITimetick.h"

#include "npa.h"
#include "ClockDefs.h"
#include "voltage_level.h"

/*timer debug variables can be mainlined based on profiling studies
 done. The latency introduced due to these features are less than a 
 tick on an average */
#define FEATURE_TIMER_TRACE
#define FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
#define FEATURE_TIMER_API_LOG

/*as of now this feature is disabled by default. The debug team can either
 uncomment this line or define this feature in .builds file*/
/*#define FEATURE_DEBUG_OFFENDING_TIMERS_ISOLATION*/

/*we still do not want to mainline FEATURE_TIMER_LIST_DEBUG 
  since it has not been profiled as of yet */
/*#define FEATURE_TIMER_LIST_DEBUG*/

/*=============================================================================

                             CONFIGURATION

=============================================================================*/


/*-----------------------------------------------------------------------------
 Select edge-triggered timer interrupt, if supported
-----------------------------------------------------------------------------*/

#if defined( T_MSM7500 ) || defined( T_QSC60X0 ) || defined (FEATURE_TIMER_APQ8064)


  #define TIMETICK_INT_EDGE_TRIGGERED

  /* All interrupts are already edge-triggered */
  #define TIMETICK_INT_EDGE_TRIGGERED_INIT() /* No-op */


#elif defined( T_MSM6700 ) || defined( T_MSM6275 )


  #define TIMETICK_INT_EDGE_TRIGGERED

  #define TIMETICK_INT_EDGE_TRIGGERED_INIT() \
    HWIO_INT_DET_REG_1_OUTM( \
      HWIO_INT_DET_REG_1_SLEEP_TIME_TICK_INT_BMSK, \
      HWIO_INT_DET_REG_1_SLEEP_TIME_TICK_INT_BMSK )


#elif defined( T_MSM6550 )


  #define TIMETICK_INT_EDGE_TRIGGERED

  /*lint -emacro(570,TIMETICK_INT_EDGE_TRIGGERED_INIT) */
  #define TIMETICK_INT_EDGE_TRIGGERED_INIT() \
    HWIO_INT_DETECT_CTL_0_OUTM( \
      HWIO_INT_DETECT_CTL_0_SLP_XTAL_TIMETICK_INT_BMSK, \
      HWIO_INT_DETECT_CTL_0_SLP_XTAL_TIMETICK_INT_BMSK )

#endif /* T_MSMxxxx */


/*=============================================================================

                           DATA DEFINITIONS

=============================================================================*/

#define TIMER_MAX_BACKOFF		 (1920) //100us in xo ticks

/*-----------------------------------------------------------------------------
  Signals used by the timer task.
-----------------------------------------------------------------------------*/

/* Signal for a timer expiring */
#define TIMER_EXPIRY_SIG          0x00000001

/* Signal for a change in the active timer list */
#define TIMER_CHANGE_SIG          0x00000002

/* Signal mask to indicate timer task to allocate new client timer chunk */
#define TIMER_ALLOCATE_CHUNK_SIG  0x00000004

/* Signals which must cause the active timer list to be processed */
#define TIMER_PROCESS_SIGS        (TIMER_EXPIRY_SIG | TIMER_CHANGE_SIG | TIMER_ALLOCATE_CHUNK_SIG)

/* Signals mask for timer_sleep API Stub  */
#define TIMER_SLEEP_CB_SIG_MASK 0x80000000

/*-----------------------------------------------------------------------------
  Timer Bin Configuration
   - Modify these values to improve binning performance
-----------------------------------------------------------------------------*/

/* Number of bins available. This must be large enough to accomodate all the
   timers in the system, or an err_fatal will be reached */ 
#define TIMER_BIN_NUM             128 

/* The range (in timeticks) of each timer bin 
    - This must be (2^n - 1) to work (for bitwise optimizations)
    - 8191 = 0.25 sec bins */
#define TIMER_BIN_RANGE           TIMER_FREQ_VALUE/4
#define MAGIC_INITIALIZER         0xDEADBEEF
#define MAGIC_INITIALIZER_64      0xDEADBEEFD00DFEEDuLL

/* Factor to divide 19.2MHz clock into 32kHz */
#define XO_TO_SCLK_CONV_FACTOR 586

/* This range is a result of the following calculation...
In general we can say that the oscillator has a +/- 100% swing in freq
Therefore lower bound on Latency(Ticks/ms) = 2*f/1000 
If 5ms is max then TIMER_SCLK_LATENCY = (2*f/1000)*5 = f/100 
*/
#define TIMER_SCLK_LATENCY        TIMER_FREQ_VALUE/100

#ifdef FEATURE_OS_MEMORY_PROTECTION
extern byte * A11S_CSR_BASE;
#endif

/*-----------------------------------------------------------------------------
  Pool of timer bins
-----------------------------------------------------------------------------*/
#ifdef TIMER_BIN_REMOVED
timer_bin_type              timer_bin_pool[ TIMER_BIN_NUM ];
#endif


/* Initialize the timers structure with some default values */
static timers_type                timers =
{
  /* Faux timer group contain active timers & list of timer groups */
  {
    0,
	
    /* Active timers */
    {
      /* Slow clock "zero" base count for active timers list */
      NULL,

      /* Linked list of active timers */
      NULL
    },

    /* Disabled flag for the Faux Active Timer group.
       This group is always enabled.  When timers are disabled, they are moved
       off of this list */
    FALSE,

    /* Deferrable Flag */
    FALSE,

    /* Linked list of timer groups which have been disabled at some time. */
    &timers.active,
    
    /* Process index */    
    0xFF,	

    /* Group index */
    0xFF
  },


  /* Faux timer group contain p0 active timers & list of timer groups */
  {
    /* External self */
    0,
		
    /* Active timers */
    {
      /* Slow clock "zero" base count for active timers list */
      0,

      /* Linked list of p0 active timers */
      NULL
    },

    /* Disabled flag for the Faux Active Timer group.
       This group is always enabled.  When timers are disabled, they are moved
       off of this list */
    FALSE,

    /* Deferrable Flag */
    FALSE,

    /* Linked list of timer groups which have been disabled at some time. */
    &timers.p0_active
  },

  /* Timestamp when call to _set_next_interrupt updates match_value */
  0,

  /* Timestamp of last attempt to update match value */
  0,

  /* Value passed to _set_next_interrupt */
  0,

  /* Last timer expiration value */
  0,

  /* Value written to match count */
  0,

  /* Timestamp when timer isr occurred at */
  0,

  /* Previous learned timestamp when timer isr occurred at */
  0,

  /* timers_process_active_timers is not executing */
  FALSE,

  /* Allow timer_isr to be set */
  TRUE,

  /* Number of active timers */
  0,

  /* Number of paused timers */
  0,

  #ifdef TIMER_BIN_REMOVED
  /* Active timer bin list */
  {
    /* First bin in active bin list */
    NULL,

    /* Last bin in active bin list */
    NULL,

    /* Number of bins in list */
    0
  },

  /* Free timer bin list */
  {
    /* First bin in free bin list */
    NULL,

    /* Last bin in free bin list */
    NULL,

    /* Number of bins in list */
    0
  },
  #endif /*endif TIMER_BIN_REMOVED*/

  /* First Non-Deferrable Timer Pointer */
/* Timer Internal Pointer */
  NULL,

  #ifndef FEATURE_TIME_QDSP6
  /* Dal Device Handle */
  /* Dal Interrupt Handle */
  NULL,
  #endif /*FEATURE_TIME_QDSP6*/

  /* Backoff time */
  0

};

/* Since there are only 8 bits are for group idx, the max can be only 256. */
#define TIMER_MAX_GROUPS 256
static timer_group_t *timer_internal_groups[TIMER_MAX_GROUPS] = {NULL};
static uint32 timer_next_free_group_idx = 0;
static uint32 timer_internal_groups_allocated = 0;

/* Max elements in the timer array. */
#define MAX_TIMER_BUFFER_NUMBER   256
#define MIN_TIMER_BUFFER_NUMBER    -1

/* Buffer of timer_internal_type */
static timer_ptr_internal_type timer_buffer_ptr = NULL;

/* Client timers chunks type */
typedef struct timer_client_buffer_s
{
  timer_client_ptr_type           buffer;     /**< Chunk of memory for 
                                                               client timers */
  uint32                          buffer_idx; /** Index to next timer, probably free */

  uint16                          num_timers; /**< Number of timers in the chunk */
  
  uint16                          allocated_cnt; /**< Count of free timers in the chunk. */
} timers_client_buffer_type;

/* Threshold for minimun numbers of client timers in a chunk to create another new chunk */
#define TIMERS_MIN_CLIENT_TIMERS_FOR_NEW_CHUNK   15

/* Max Chunks possible 256 */
#define TIMER_CLIENT_CHUNKS_MAX 12

/* Client timer chunks static allocation */
timers_client_buffer_type timer_client_chunks[TIMER_CLIENT_CHUNKS_MAX];

uint32 timer_client_chunks_allocated = 0;
uint32 timer_client_curr_chunk_idx = 0;

/* Count of total number of free client timers in in all the chunks */
uint32 timer_free_client_timers_cnt = 0;

/* Max 2D array elements for the static allocation of client timers */
#define TIMER_CLIENT_BUFFER_INIT_ARRAY_SIZE  8

/* Static array of client timers for the 1st chunk */
static timer_client_type timer_client_buffer[TIMER_CLIENT_BUFFER_INIT_ARRAY_SIZE][MAX_TIMER_BUFFER_NUMBER];

/* Global vars for storing timer chunk sizes */
uint32 timer_client_chunk_total_num_chunks = 0;

/* Queue to store free timer internal strcuture */
static q_type                     timer_free_q;

/* Indicated if timer_free_q in initialized */ 
static uint8 timer_q_init_status = 0;

#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
static uint32 timer_while_cnt = 0;
#endif

static DalDeviceHandle *hTimerHandle = NULL;

static rex_tcb_type *timer_tcb_ptr = NULL;

/* Variables required for Handling timers of remote process */
static timer_process_cb_handler timer_remote_callback_handler = NULL;
static uint32 timer_curr_process_idx = 0;

/* Notification functions for indicating non-defer timer expiry changes */
typedef struct
{
  timer_notify_type         notify_type;
  time_osal_notify_obj_ptr  notify_obj;
  time_osal_notify_data     notify_mask;
  uint32                    process_idx;
} timer_non_defer_notification_client_type;

#define TIMER_NON_DEFER_NOTIFICATION_MOCK_PTR ((timer_ptr_type)0x88888888)
#define TIMER_NON_DEFER_EXPIRY_NOTIFY_CLIENTS_MAX_CNT 4
static timer_non_defer_notification_client_type timer_non_defer_expiry_notify_clients[TIMER_NON_DEFER_EXPIRY_NOTIFY_CLIENTS_MAX_CNT];
static uint32 timer_non_defer_expiry_notify_clients_cnt = 0;

/*-----------------------------------------------------------------------------
  Internal timer priority group
-----------------------------------------------------------------------------*/

/* Priority 0 group for timers defined with timer_def_priority API */
timer_group_type                  timer_prio0_group;

/*-----------------------------------------------------------------------------
  Timer trace data
-----------------------------------------------------------------------------*/

/* Stores number of timers present in timer_buffer*/
static uint32 timer_count_buffer_allocated = 0;

/* Stores the static function pointers for timer dependencies */
static timer_fn_map_fp_type timer_fn_map;

#ifdef FEATURE_TIMER_TRACE

/* Type of timer events to log */
typedef enum
{
  TIMER_TRACE_ISR,
  TIMER_TRACE_PROCESS_START,
  TIMER_TRACE_PROCESS_END,
  TIMER_TRACE_MATCH_VALUE_PROG,
} timer_trace_event_type;

/* A single timer event trace */
typedef struct
{
  timer_trace_event_type  event;
  time_timetick_type      ts;
  time_timetick_type      match_value_programmed;

} timer_trace_data_type;


/* Structure of all timer log events */
#define TIMER_TRACE_LENGTH  128
typedef struct
{
  uint32                index;
  timer_trace_data_type events[TIMER_TRACE_LENGTH];
} timer_trace_type;

/* Declare the log structure */
static timer_trace_type timer_trace;

#endif /* FEATURE_TIMER_TRACE */

#ifdef FEATURE_TIMER_API_LOG

typedef enum
{
  TIMER_API_LOG_TIMER_INIT = 0x1,
  TIMER_API_LOG_TIMER_DEF,
  TIMER_API_LOG_TIMER_DEF_OSAL,
  TIMER_API_LOG_TIMER_DEF2,
  TIMER_API_LOG_TIMER_SET,
  TIMER_API_LOG_TIMER_SET_ABSOLUTE,
  TIMER_API_LOG_TIMER_GET,
  TIMER_API_LOG_TIMER_GET_PARAMS,
  TIMER_API_LOG_TIMER_SET_PARAMS,
  TIMER_API_LOG_TIMER_CLR,
  TIMER_API_LOG_TIMER_STOP,
  TIMER_API_LOG_TIMER_UNDEF,
  TIMER_API_LOG_TIMER_CLR_TASK_TIMERS,
  TIMER_API_LOG_TIMER_CLR_OSAL_THREAD_TIMERS,
  TIMER_API_LOG_TIMER_GROUP_ENABLE,
  TIMER_API_LOG_TIMER_GROUP_DISABLE,
  TIMER_API_LOG_TIMER_GROUP_SET_DEFERRABLE,
  TIMER_API_LOG_TIMER_GET_TIME_TILL_FIRST_NON_DEFERRABLE_TIMER,
  TIMER_API_LOG_TIMER_DEFER_MATCH_INTERRUPT,
  TIMER_API_LOG_TIMER_UNDEFER_MATCH_INTERRUPT,
  TIMER_API_LOG_TIMER_RESTART,
  TIMER_API_LOG_TIMER_REG,
  TIMER_API_LOG_TIMER_GET_SCLK_TILL_EXPIRY,
  TIMER_API_LOG_TIMER_SET_WAKEUP,
  TIMER_API_LOG_TIMER_GET_QURT_SIG_REGISTRATION_ID,
  TIMER_API_LOG_TIMER_INIT_QURT_SIGNAL,
  TIMER_API_LOG_TIMER_UPDATE_ISR_TIMER,
  TIMER_API_LOG_TIMER_EXPIRES_AT,
  TIMER_API_LOG_TIMER_GET_START,
  TIMER_API_LOG_TIMER_GET_GROUP,
  TIMER_API_LOG_TIMER_GET_EXPIRY_TYPE,
  TIMER_API_LOG_TIMER_PAUSE,
  TIMER_API_LOG_TIMER_RESUME,
  TIMER_API_LOG_TIMER_IS_ACTIVE,
  TIMER_API_LOG_TIMER_IS_PERIODIC,
} timer_api_type;

typedef struct
{
  /* API type that was called */
  timer_api_type    		api;
  /* Pointer to calling function */
  //timer_cb_data_type      *timer_cf;
  /* Pointer to TCB */
  rex_tcb_type            	*tcb;
  /* Time Stamp */
  time_timetick_type		ts;
  /* Timer Pointer */
  timer_ptr_type			timer;
} timer_api_log_type;


#define MAX_TIMER_API_CALLS 64
static timer_api_log_type timer_api_calls[MAX_TIMER_API_CALLS];
static int timer_api_log_cnt = 0;
static void timer_api_log
(
  timer_api_type		event, 
  timer_ptr_type 		timer,  
  time_timetick_type	timetick
);

#endif /* FEATURE_TIMER_API_LOG */

#define TIMER_UNDEFINED_DURATION 0xFFFFFFFFFFFFFFFFuLL

/* Macros to perform logging */
#ifdef FEATURE_TIMER_TRACE
  #define TIMER_TRACE(event)              timer_trace_event(TIMER_TRACE_##event, TIMER_UNDEFINED_DURATION)
  #define TIMER_TRACE_TS(event,ts)        timer_trace_event(TIMER_TRACE_##event, ts)
#else
  #define TIMER_TRACE(event)
  #define TIMER_TRACE_TS(event,ts)
#endif

#ifdef FEATURE_TIMER_API_LOG
  #define TIMER_API_LOG(api,timer,timetick) timer_api_log(api,timer,timetick)
#else
  #define TIMER_API_LOG(api,timer,timetick)  
#endif /* FEATURE_TIMER_API_LOG */


static void timer_active_list_check(void);

/**
Post initialization routine for timer
*/
void timer_post_init(void);


/*For ARM9*/
#ifndef FEATURE_TIME_QDSP6
#define ATS_INTLOCK() INTLOCK()
#define ATS_INTFREE() INTFREE()

#define ATS_DECLARE_ISAV(X) dword X = 0
#define ATS_INTLOCK_SAV(x) INTLOCK_SAV(x)
#define ATS_INTFREE_SAV(x) INTFREE_SAV(x)      

#define TIMER_TRACE_MUTEX_LOCK() INTLOCK()
#define TIMER_TRACE_MUTEX_UNLOCK() INTFREE()
#define TIMER_PC_MASK_MUTEX_LOCK() INTLOCK()
#define TIMER_PC_MASK_MUTEX_UNLOCK() INTFREE()
#define TIMER_OS_EARLY_TOL 0

#define TIMER_MAX_EXPIRY 0xFFFFFF00000000uLL

#else /*else for Q6*/

#define TIMER_QURT_SIG_MASK 0x2

#define TIMER_QURT_TASK_SIGNAL_MASK (TIMER_QURT_SIG_MASK | TIMER_ALLOCATE_CHUNK_SIG)

/*registration id obtained for timer signal registered with QURT. This
 id will be used by timetick to program the match values for timers*/
static int timer_qurt_sig_registration_id = -1;

/*Timer signal registered with QURT. QURT will set this signal to
 TIMER_QURT_SIG_MASK when the match value == timetick value*/
qurt_anysignal_t        timer_q6_signal;

/* Pointer to the mutex object for protecting critical-section of timer 
   code*/
qurt_mutex_t timer_qemu_mutex;
static qurt_mutex_t timer_trace_mutex;
static qurt_mutex_t timer_pc_mask_mutex;

/*function protect the critical section of timer code*/
void timer_pimutex_init(void);
void timer_trace_mutex_init(void);
void timer_pc_mutex_init(void);
/*MACRO for locking/unlocking the pimutex*/
#define ATS_INTLOCK()  \
  qurt_pimutex_lock(&timer_qemu_mutex); \
  
#define ATS_INTFREE()  \
  qurt_pimutex_unlock(&timer_qemu_mutex); \

#define ATS_DECLARE_ISAV(X)

#define ATS_INTLOCK_SAV(X) \
  qurt_pimutex_lock(&timer_qemu_mutex); \
  
#define ATS_INTFREE_SAV(X)   \
  qurt_pimutex_unlock(&timer_qemu_mutex); \

#define TIMER_TRACE_MUTEX_LOCK()  \
  qurt_pimutex_lock(&timer_trace_mutex); \
  
#define TIMER_TRACE_MUTEX_UNLOCK()  \
  qurt_pimutex_unlock(&timer_trace_mutex); \

#define TIMER_PC_MASK_MUTEX_LOCK()  \
  qurt_pimutex_lock(&timer_pc_mask_mutex); \

#define TIMER_PC_MASK_MUTEX_UNLOCK()  \
  qurt_pimutex_unlock(&timer_pc_mask_mutex); \

#define TIMER_OS_EARLY_TOL 1920

#define TIMER_MAX_EXPIRY QURT_TIMER_MAX_DURATION_TICKS

#endif /*FEATURE_TIME_QDSP6*/

/*This enum will be used to specify the event
 responsible for setting the match value*/
typedef enum
{
  TIMER_MVS_TIMER_PROCESSED = 0x1,
  TIMER_MVS_TIMER_SET,
  TIMER_MVS_DEFER_MATCH_INT,
  TIMER_MVS_UNDEFER_MATCH_INT,
  TIMER_MVS_GROUP_DISABLED,
  TIMER_MVS_GROUP_ENABLED,
  TIMER_MVS_TIMER_CLEARED,
  TIMER_MVS_TIMER_PAUSED,
  TIMER_MVS_TIMER_RESUMED,
  TIMER_MVS_TIMER_CLEAR_TASK_TIMERS,
  TIMER_MVS_SET_WAKEUP,
  TIMER_MVS_TIMER_UNDEFINED,
}timer_match_interrupt_setter_type;

/*These are timer debug variables that were useful to catch many 
 bugs. As of now they are featurized, so save space*/
#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES

typedef struct timer_debug_var_1
{
  uint64 timer_now_actual;
  uint64 timer_match_value_actual_set;
  timer_ptr_internal_type timer_address;
  uint64 timer_now_supplied;
  uint64 match_value_supplied;
  timer_match_interrupt_setter_type  mv_setter:8;  
}timer_match_value_record_type;
#define MAX_MATCH_REC 32
static timer_match_value_record_type record_match_val[MAX_MATCH_REC];
static int record_match_val_counter = 0;

typedef struct timer_debug_var_2
{
  timer_ptr_type 			timer_to_be_set;
  timer_ptr_internal_type 	int_timer;
  time_timetick_type 		ts;
  time_timetick_type 		expiry;
  rex_tcb_type *			tcb_ptr;
}timer_set_type;

#define MAX_TIMER_SET 256
static timer_set_type timer_sets[MAX_TIMER_SET];
static int timer_set_cnt = 0;
static int timer_set_cnt_global = 0;

typedef struct timer_clr_dbg_var
{
  timer_ptr_type ext_timer;
  timer_ptr_internal_type int_timer;
  time_timetick_type ts;
  timer_error_type ret_val;
  timer_state_enum_type timer_state;
  rex_tcb_type * tcb_ptr;
}timer_clr_log_type;
#define MAX_TIMER_CLR_LOG 128
static timer_clr_log_type timer_clr_log[MAX_TIMER_CLR_LOG];
static uint32 timer_clr_log_cnt = 0;

typedef struct timer_expired_debug_struct
{
  uint32 whileloopcnt;
  timer_ptr_type timer_exp_ptr;
  timer_ptr_internal_type int_timer;
  timer_client_ptr_type clnt_timer;
  time_timetick_type ts;
  time_timetick_type expiry;
  time_timetick_type start;
  void * tcb_or_func;
  timer_properties_type info;
}timer_expired_debug_type; // 40 bytes
#define MAX_TIMER_EXPIRED_DEBUG 256
static timer_expired_debug_type timer_expired_debug[MAX_TIMER_EXPIRED_DEBUG];
static uint32 timer_expired_debug_cnt = 0;

typedef struct timer_debug_var_3
{
  timer_ptr_type          timer_expired;
  timer_ptr_internal_type int_timer;
  time_timetick_type      expiry_value; /* copy the expiry value of the timer */
  time_timetick_type      processing_started;
  time_timetick_type      processing_time;
  void *                  tcb_sigs_obj;
  timer_properties_type   info;
}timer_expired_type;


#define MAX_TIMER_EXPIRED 128
static timer_expired_type timers_expired[MAX_TIMER_EXPIRED];
static int timer_expired_cnt = 0;
static int timer_expired_cnt_global = 0;

/* For Remote process Timers */
#define MAX_TIMER_REMOTE_EXPIRED 32
static timer_expired_type timers_remote_expired[MAX_TIMER_REMOTE_EXPIRED];
static int timer_remote_expired_cnt = 0;
int timer_remote_expired_cnt_global = 0;

#endif /* FEATURE_ENABLE_TIMER_DEBUG_VARIABLES*/

#ifdef FEATURE_DEBUG_OFFENDING_TIMERS_ISOLATION

typedef struct timer_debug_var_4
{
  time_timetick_type processing_time;
  timer_ptr_internal_type timer_expired;  
  uint8  call_back_type;  
  time_timetick_type processing_started;
  uint32 thresh_exceed_count; /* number of times we have exceeded the threshold */
  uint32 fn_address; /* duplciate - not needed */
  uint64 data; /* duplciate - not needed */
}timer_isolate_type;
#define MAX_TIMER_ISOLATED 64
static uint32 timers_cnt_assert_threshold = 10;
static timer_isolate_type timers_isolated[MAX_TIMER_ISOLATED];
static int timer_isolated_cnt = 0;
static int timer_isolated_cnt_global = 0;

#define MAX_TIMER_ASSERT_COMPARE_LIST 16
uint32 timer_assert_compare_value_count;
uint32 timer_assert_compare_value_list[MAX_TIMER_ASSERT_COMPARE_LIST];

#endif /* FEATURE_DEBUG_OFFENDING_TIMERS_ISOLATION */

/*These values are set high to avoid the annoying F3 messages
 and an error fatal in case cb processing time is high. 
 1 sec = 32768 ticks. 
 Therefore, 32 ticks = 1 msec 
 */
volatile static uint64 timer_processing_time_warning_threshold = 19200;

#ifndef TIMER_ERROR_FATAL_THRESHOLD_ZEROED_OUT
volatile static uint64 timer_processing_time_err_fatal_threshold=38400000;
#else
volatile static uint64 timer_processing_time_err_fatal_threshold=0;
#endif

#ifdef FEATURE_TIMER_USE_QURT_SYSCLOCK
/*this function will be called in Q6 for updating the timer
 variables*/
static void timer_update_isr_time
(
  void
);
#endif /* FEATURE_TIMER_USE_QURT_SYSCLOCK */

static void timer_get_timetick_handle(void);

static void timer_init_qurt_signal(void);

timer_error_type timer_create_new_chunk(uint32 max_timers);

typedef struct
{
  time_timetick_type timer_undefer_match_interrupt_now;
  time_timetick_type timer_undefer_match_interrupt_first;
}timer_undefer_match_interrupt_log_type;
static timer_undefer_match_interrupt_log_type timer_undefer_match_interrupt_log; 

static boolean timer_slave_processing_enabled = FALSE; 

static int timer_after_pc_mask[5]={0,0,0,0,0};
#define TIMER_PC_MASK_INDEX 0
#define TIMER_PC_DEFER_UNDEFER_CNT_INDEX 4
static int timer_cnt_cpu_max=0;
static int timer_cnt_cpu_down=0;

typedef struct 
{
  int reference_count;
  timer_match_interrupt_setter_type function_called;
  time_timetick_type ts;
  time_timetick_type undefer_entry_ts;  /* Only needed for the undefer call */
}timer_defer_undefer_reference_count_log_type;

#define MAX_TIMER_REFERNCE_COUNT_LOG 8
#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
static timer_defer_undefer_reference_count_log_type timer_defer_undefer_reference_count_log[MAX_TIMER_REFERNCE_COUNT_LOG];
static int timer_defer_undefer_reference_count_log_cnt = 0;
static int timer_defer_undefer_reference_count =0;
#endif /*FEATURE_ENABLE_TIMER_DEBUG_VARIABLES*/

/* Log of timers having multiple notifications */
#define MAX_TIMER_MULTI_NOTIFY 4
typedef struct {
rex_tcb_type * tcb_ptr;
timer_ptr_type  timer_ptr;
}
timer_multi_notify_type;

static uint32 timer_multi_notify_cnt = 0;
static timer_multi_notify_type timer_multi_notify[MAX_TIMER_MULTI_NOTIFY];



/* Log for the API timer_set_params */
#define MAX_TIMER_SET_PARAMS_LOG 16

typedef struct {
timer_ptr_type timer;
time_timetick_type ts;
timer_state_enum_type timer_state;
timer_notify_type notification;
} timer_set_params_log_type ;

static uint32 timer_set_params_log_cnt = 0;
static timer_set_params_log_type timer_set_params_log[MAX_TIMER_SET_PARAMS_LOG];

/*=============================================================================

                             MACRO DEFINITIONS

=============================================================================*/

/* Invalid Handle */
#define TIMER_BUFFER_INVALID_HANDLE   -1

#define INDEX_TO_HANDLE(x)   ( (x) +1 )
#define HANDLE_TO_INDEX(x)   ( (x) -1 )
#define TIMER_HANDLE_CONSTANT 0xC3
#define GROUP_HANDLE_CONSTANT 0xC3C3

inline uint32 timer_index_to_handle(uint32 chunk_idx, uint32 timer_idx)
{
   uint32 handle;
   uint8 cid, tid1, tid0;
   
   cid = chunk_idx & 0xFF;
   tid1 = (timer_idx >> 8) & 0xFF;
   tid0 = timer_idx & 0xFF;
   
   handle = (uint32)(((cid ^ tid1 ^ tid0 ^ TIMER_HANDLE_CONSTANT) & 0xFF) << 24) | (uint32)((cid) << 16) | (uint32)((tid1) << 8) | (uint32)(tid0);
   return handle;
}
#define TIMER_INDEX_TO_HANDLE(cid, tid)   ( timer_index_to_handle(cid, tid) )

inline boolean timer_handle_to_index(uint32 handle, uint32 *chunk_idx, uint32 *timer_idx)
{
   uint8 temp, cid, tid1, tid0;
   
   if(chunk_idx == NULL || timer_idx == NULL)
      return FALSE;
   
   temp = (handle >> 24) & 0xFF;
   cid  = (handle >> 16) & 0xFF;
   tid1 = (handle >> 8) & 0xFF;
   tid0 = handle & 0xFF;
   
   if((cid ^ tid1 ^ tid0 ^ TIMER_HANDLE_CONSTANT) != temp)
      return FALSE;

   *chunk_idx = cid;
   *timer_idx = handle & 0xFFFF;
   return TRUE;
}
#define TIMER_HANDLE_TO_INDEX(h, cid_ptr, tid_ptr) (timer_handle_to_index(h, cid_ptr, tid_ptr))


inline uint32 group_index_to_handle(uint32 group_idx)
{
   uint32 handle;
   handle = ((group_idx & 0xFFFF) ^ GROUP_HANDLE_CONSTANT)  << 16 | (group_idx & 0xFFFF);
   return handle;
}
#define GROUP_INDEX_TO_HANDLE(gid)   ( group_index_to_handle(gid) )

inline boolean group_handle_to_index(uint32 handle, uint32 *group_idx)
{
   uint16 temp, gid;
   
   if(group_idx == NULL)
      return FALSE;
   
   temp = (handle >> 16) & 0xFFFF;
   gid  = handle & 0xFFFF;
   
   if((gid ^ GROUP_HANDLE_CONSTANT) != temp)
      return FALSE;

   *group_idx = gid;
   return TRUE;
}
#define GROUP_HANDLE_TO_INDEX(h, gid_ptr) (group_handle_to_index(h, gid_ptr))

#define GROUP_INVALID_HANDLE 0
#define TIMER_INVALID_HANDLE 0

static rlc_handle_ptr rlc_handle = NULL;
static rlc_external_attribute_type rlc_attribute_learn;
static rlc_external_attribute_type rlc_attribute_correct;
/*=============================================================================

                             FUNCTION DEFINITIONS

=============================================================================*/
static void timer_set_backoff(uint32 cluster)
{	uint32 num=0;
	num=rlc_get_cluster_location(rlc_attribute_correct, cluster, 0);
	ATS_INTLOCK();
	if(num > TIMER_MAX_BACKOFF)
	{
		timers.backoff = TIMER_MAX_BACKOFF;
	}
	else
	{
		timers.backoff = num;
	}
	ATS_INTFREE();
    //ATS_ERR_1("ATS: New backoff: %d\n", timers.backoff);
}



void timer_reset_check_pc_mask_if_set(unsigned int index, unsigned int decrease_count)
{

  TIMER_PC_MASK_MUTEX_LOCK();
 
  if( timer_after_pc_mask[index] > 0)
  {
    timer_after_pc_mask[index]= timer_after_pc_mask[index]-decrease_count;
    if( (timer_after_pc_mask[0] <= 0) &&
        (timer_after_pc_mask[1] <= 0) &&
        (timer_after_pc_mask[2] <= 0) &&
        (timer_after_pc_mask[3] <= 0) )
    {
      // Release the cpu, expected to be executed once per powercollapse.
      if( timer_fn_map.timer_npa_clk_client )
      { /* client/resource exists so use can make MAX requests to the clock */
        if(timer_fn_map.timer_npa_complete_request)
        {  
          timer_fn_map.timer_npa_complete_request(timer_fn_map.timer_npa_clk_client);
        }
        timer_cnt_cpu_down++;
      }else
      {
        ATS_ERR("NPA Client is null for Timer to bring down the cpu");
      }

    }
  }
  TIMER_PC_MASK_MUTEX_UNLOCK();
}

static void timer_reset_check_pc_mask(unsigned int index)
{

  TIMER_PC_MASK_MUTEX_LOCK();
  timer_after_pc_mask[index]--;
  if( (timer_after_pc_mask[0] <= 0) &&
      (timer_after_pc_mask[1] <= 0) &&
      (timer_after_pc_mask[2] <= 0) &&
      (timer_after_pc_mask[3] <= 0) )
  {
    // Release the cpu, expected to be executed once per powercollapse.
    if( timer_fn_map.timer_npa_clk_client )
    { /* client/resource exists so use can make MAX requests to the clock */
      if( timer_fn_map.timer_npa_complete_request )
      {
        timer_fn_map.timer_npa_complete_request(timer_fn_map.timer_npa_clk_client);
      }
      timer_cnt_cpu_down++;
    }else
    {
      ATS_ERR("NPA Client is null for Timer to bring down the cpu");
    }

  }
  
  TIMER_PC_MASK_MUTEX_UNLOCK();
}

void timer_clk_cpu_callback( void *context,
                             unsigned int event_type, 
                             void *data,       
                             unsigned int data_size)
{
  npa_query_type queryResult;
  uint32 voltage_mode = RAIL_VOLTAGE_LEVEL_LOW_SVS;

  if (NULL != timer_fn_map.timer_npa_clk_client)
  { /* client already created */
    return;
  }

  for (;voltage_mode < RAIL_VOLTAGE_LEVEL_NUM_LEVELS; voltage_mode++)
  {
    if(NPA_QUERY_SUCCESS == npa_query_by_name("/clk/cpu",
  	                                  CLOCK_NPA_QUERY_CPU_MAX_KHZ_AT_CORNER + voltage_mode,
  	                                  &queryResult))
    {
	  /* Update the time_fn_map structure with the updated SVS MAX freq */
      timer_fn_map.timer_npa_max_state = queryResult.data.value;  
      break;
    }
      
  }


  /* reached the max and still could not find a valid voltage mode */
  if ( RAIL_VOLTAGE_LEVEL_NUM_LEVELS == voltage_mode )
  {
    ATS_ERR("NPA failed to get the MAX frequncy in SVS mode");
  }
  else
  {
    /*only if a valid voltage mode is found then try to create a client*/
    if( timer_fn_map.timer_npa_create_sync_client )
    {
      timer_fn_map.timer_npa_clk_client = 
        timer_fn_map.timer_npa_create_sync_client( "/clk/cpu", 
                                                   "timer_clk_client",
                                                   timer_fn_map.timer_npa_client_required,
                                                   0,
                                                   NULL );
    }

    if (NULL == timer_fn_map.timer_npa_clk_client)
    { 
      /* client creation failed */
      ATS_ERR("NPA failed creation of timer_clk_client for Timer");
    }

  }/*end if-else*/
}

void timer_register_npa_client(void)
{
  if( timer_fn_map.timer_npa_resource_available_cb )
  {
    timer_fn_map.timer_npa_resource_available_cb( "/clk/cpu",
                                                  timer_clk_cpu_callback,
                                                  NULL );
  }
}

/*=============================================================================

FUNCTION TIMER_GET_TIMETICK_HANDLE

DESCRIPTION
 This function is called from timer_init to pass timetick handle to timer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void timer_get_timetick_handle(void)
{

#ifdef FEATURE_TIMER_SLAVE1
  /* We want slave task to return the handle since slave needs the ptr too */
  timer_slaves_get_timetick_handle((void**)&hTimerHandle);
#else /* FEATURE_TIMER_SLAVE1 */
  DalTimetick_Attach("SystemTimer", &hTimerHandle);
  if (hTimerHandle == NULL)
  	{
  	  ERR_FATAL("timer_get_timetick_handle: DalTimetick_Attach failed",0,0,0);
  	}
#endif /* FEATURE_TIMER_SLAVE1 */

  /* Enable system timer */
  DalTimetick_Enable(hTimerHandle,1);
  DalTimetick_InitTimetick64(hTimerHandle);

  /* Rex TCB pointer needed for task synchronization */
  timer_tcb_ptr = rcinit_lookup_rextask("timer");
}

#ifdef FEATURE_TIMER_TRACE
/*=============================================================================

FUNCTION TIMER_TRACE_EVENT

DESCRIPTION
  This function saved a timer trace event packet.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/

static void timer_trace_event
(
  timer_trace_event_type event,
  uint64 ts
)
{
  /* Pointer to the trace structure to fill in */
  timer_trace_data_type *trace;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  TIMER_TRACE_MUTEX_LOCK();
  /* Get the next trace structure */
  trace = &timer_trace.events[timer_trace.index];

  /* Fill in the common fields */
  trace->event = event;

  /* Determine whether or not we need to get a new timestamp */
  if (ts == TIMER_UNDEFINED_DURATION)
  {
    /* Get a new timestamp */
  DalTimetick_GetTimetick64(hTimerHandle, &trace->ts);
  }
  else
  {
    /* Use a passed in timestamp */
    trace->ts = ts;
  }

  /*record the match value in the structure*/
  if(event == TIMER_TRACE_MATCH_VALUE_PROG)
  {
    trace->match_value_programmed = timers.match_value; 
  }
  else
  {
    trace->match_value_programmed = 0;
  }

  /* Go to the next index */
  timer_trace.index = (timer_trace.index + 1) & (TIMER_TRACE_LENGTH - 1);

  TIMER_TRACE_MUTEX_UNLOCK();

} /* timer_trace_event */

#endif /* FEATURE_TIMER_TRACE */

#ifdef FEATURE_TIMER_API_LOG
/*=============================================================================

FUNCTION TIMER_API_LOG

DESCRIPTION
  This function logs all of the api calls along with
  the thread ID.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/

static void timer_api_log
(
  timer_api_type event,
  timer_ptr_type  timer,
  time_timetick_type timetick  
)
{
  /* Record Event Type */
  timer_api_calls[timer_api_log_cnt].api = event;
  /* Record ptr to TCB */
  timer_api_calls[timer_api_log_cnt].tcb = rex_self();

  /* Record timetick value */
  timer_api_calls[timer_api_log_cnt].ts = timetick;
  
  /* Record ptr to timer */
  timer_api_calls[timer_api_log_cnt].timer = timer;

  //timer_api_calls[timer_api_log_cnt].timer_cf =  /* Figure out how to get R14 */

  timer_api_log_cnt = (timer_api_log_cnt + 1) & ( MAX_TIMER_API_CALLS - 1 );
  
} /* timer_api_log */

#endif /* FEATURE_TIMER_API_LOG */

/** 
@brief 
API to mark the dependency between timers of two thread

@return
None

@param[in]                                                               
thread1_name Name of first thread
thread2_name Name of second thread
 
@dependencies  
None
 
@sideeffects 
None
*/
timer_error_type timer_drv_mark_interdependence_by_tid
(
   qurt_thread_t        tid1,
   qurt_thread_t        tid2
)
{
	int priority1 = 0,priority2 = 0;
	/* Setting dependency bit based on thread's priority 
       in global variable */
	priority1 = qurt_thread_get_priority(tid1);
	priority2 = qurt_thread_get_priority(tid2);

	if(priority1 == 0)
	   return TE_FAIL;
	else if(priority2 == 0)
	   return TE_FAIL;

	if(timer_slave_set_dependency_bit(priority1) != TRUE)
	   return TE_FAIL;		
	else if(timer_slave_set_dependency_bit(priority2) != TRUE)
	   return TE_FAIL;

    return TE_SUCCESS;	

}

/** 
@brief 
API to mark the dependency between timers of two thread

@return
None

@param[in]                                                               
thread1_name Name of first thread
thread2_name Name of second thread
 
@dependencies  
None
 
@sideeffects 
None
*/
timer_error_type timer_drv_mark_interdependence_by_name
( 
   char      *thread1_name ,
   char      *thread2_name
)
{
	int priority1 = 0,priority2 = 0;
	/* Setting dependency bit based on thread's priority 
       in global variable */
	priority1 = rcinit_lookup_prio(thread1_name);
	priority2 = rcinit_lookup_prio(thread2_name);
	
	if(priority1 == 0)
	   return TE_FAIL;
	else if(priority2 == 0)
	   return TE_FAIL;
	 
	if(timer_slave_set_dependency_bit(priority1) != TRUE)
	   return TE_FAIL;		
	else if(timer_slave_set_dependency_bit(priority2) != TRUE)
	   return TE_FAIL;
    
    return TE_SUCCESS;	

}

/*=============================================================================

FUNCTION TIMER_NOTIFY_FIRST_NON_DEFER_TIMER_EXPIRY_CHANGE

DESCRIPTION
  Calls the client's callbacks to notify the changed first non-deferrable timer 
  expiry

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/

static void timer_notify_first_non_defer_timer_expiry_change(void)
{
   int i;
   timer_notify_type         notify_type;
   time_osal_notify_obj_ptr  notify_obj;
   time_osal_notify_data     notify_mask;
   uint32                    process_idx;
   timer_client_type         loal_clnt_timer = { 0 };
   
   for(i=0; i<timer_non_defer_expiry_notify_clients_cnt; i++)
   {
   
    notify_type = timer_non_defer_expiry_notify_clients[i].notify_type;
    notify_obj = timer_non_defer_expiry_notify_clients[i].notify_obj;
    notify_mask = timer_non_defer_expiry_notify_clients[i].notify_mask;
    process_idx = timer_non_defer_expiry_notify_clients[i].process_idx;

    if(notify_obj == NULL)         
      ERR_FATAL("Notify_obj NULL means corruption happened; i=%d", i, 0, 0);     
	 
    /* If there are signals to be set, and a task / signal object to set the signals to,
            then set those signals in that task's task control block or the passed signal object */
	if (process_idx == timer_curr_process_idx )
	{
		if ( notify_type == TIMER_NATIVE_OS_SIGNAL_TYPE )
		{
		  #ifdef FEATURE_TIME_QDSP6
		   qurt_anysignal_set((qurt_anysignal_t *)notify_obj, notify_mask);
		  #else
		   ERR_FATAL("Signal type cannot be NATIVE OS type ; timer = 0x%x",timer->timer_ext,0,0);
		  #endif /* FEATURE_TIME_QDSP6 */
		}
     	else if ( notify_type == TIMER_REX_TCB_SIGNAL_TYPE )
     	{
		  #ifdef FEATURE_TIMER_REX_IS_AVAILABLE
		   (void) rex_set_sigs( notify_obj, notify_mask );
		  #else
		   ERR_FATAL("Signal type cannot be rex type ; timer = 0x%x",timer->timer_ext,0,0);
		  #endif /* FEATURE_TIMER_REX_IS_AVAILABLE */
		}
		else if ( notify_type == TIMER_NATIVE_OS_SIGNAL2_TYPE )
		{
		  #ifdef FEATURE_TIME_QDSP6
		   qurt_signal2_set((qurt_signal2_t *)notify_obj, notify_mask);
		  #else
		   ERR_FATAL("Signal type cannot be NATIVE OS type ; timer = 0x%x",timer->timer_ext,0,0);
		  #endif /* FEATURE_TIME_QDSP6 */
		}
		else if ( notify_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE )
		{
		  timer_dalsys_sig_set(notify_obj);
		}
		else
		{
		  ERR_FATAL("Invalid Notify type 0x%x, may be corruption",notify_type,0,0);
		}
	}
	else
	{
	  /* Pass the notification details to the client proc callback handler by mimicking a timer notification */
	  loal_clnt_timer.info.callback_type	= notify_type;
	  loal_clnt_timer.tcb					= (rex_tcb_type *)notify_obj;
	  loal_clnt_timer.sigs					= notify_mask;
  	  loal_clnt_timer.info.process_idx		= process_idx;
	  loal_clnt_timer.timer_client_ptr		= TIMER_NON_DEFER_NOTIFICATION_MOCK_PTR;
      if(timer_remote_callback_handler == NULL)
        {
          ERR_FATAL("No Shim Cb func. Call timer_set_remote_process_timer_handler()", 0, 0, 0);
          return;
      	}
	  else
	  	{
          timer_remote_callback_handler(&loal_clnt_timer , 0);	  	
	  	}
	}
   } /* for() */
     
   return;
} /* timer_notify_first_non_defer_timer_expiry_change */



/*=============================================================================

FUNCTION TIMER_GET_FIRST_ACTIVE_NON_DEFERRABLE_TIMER

DESCRIPTION
  This function returns the first non-deferrable timer on the active
  timer list.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the pointer to the first non-deferrable internal timer.
  If there are no non-deferrable timers on the active list, the return NULL. 

SIDE EFFECTS
  None.

=============================================================================*/

static void timer_update_first_active_non_deferrable_timer(void)
{
  timer_ptr_internal_type ptimer;
  timer_ptr_internal_type ptimer_p0 = timers.p0_active.list.first;
  time_timetick_type p1_expiry = TIMER_MAX_EXPIRY;
  time_timetick_type p0_expiry = TIMER_MAX_EXPIRY;
  time_timetick_type old_expiry = TIMER_MAX_EXPIRY;
  time_timetick_type new_expiry = TIMER_MAX_EXPIRY;

  if(timers.first_non_deferrable)
  	old_expiry = timers.first_non_deferrable->expiry;  

  if (ptimer_p0)
  {
    p0_expiry = ptimer_p0->expiry;
  }

  for( ptimer = timers.active.list.first; ptimer != NULL; ptimer = ptimer->next )
  {
    /* If timer belongs to a group, which is non-deferrable */
    if (timer_internal_groups[ptimer->info.group_idx]->deferrable == FALSE )
    {
      /* Found the first non-deferrable timer on the active list */
      p1_expiry = ptimer->expiry;
      break;
    }
  }

  /* If there was a non-deferrable timer on the active list, then check the expiry value */
  if (p0_expiry == TIMER_MAX_EXPIRY &&
      p1_expiry == TIMER_MAX_EXPIRY)
  {
    /* There are no active non-deferrable timers */
    timers.first_non_deferrable = NULL;
  }
  else if (p1_expiry < p0_expiry)
  {
    /* First non-deferrable timer is on the active list */
    timers.first_non_deferrable = ptimer;
	new_expiry = p1_expiry;
  }
  else
  {
    /* First non-deferrable timer is on the p0 active list */
    timers.first_non_deferrable = ptimer_p0;
	new_expiry = p0_expiry;
  }

  if(old_expiry != new_expiry)
  	timer_notify_first_non_defer_timer_expiry_change();

  
}/* timer_update_first_active_non_deferrable_timer */

/*=============================================================================

FUNCTION TIMER_DETERMINE_NEXT_TIMER_EXPIRY

DESCRIPTION
  To determine which earliest timer from the two timer list.
  This will require us to iterate through the two list to check
  the earlier of the two timers and return that timer.

DEPENDENCIES
  This function should be called from critical section.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
time_timetick_type timer_determine_next_timer_expiry
(
  timer_ptr_internal_type *head_timer
)
{
  /*initialize the next_timer_expiry to max value*/
  time_timetick_type first = TIMER_MAX_EXPIRY;
    
  if(timers.p0_active.list.first != NULL) 
  {
    first = timers.p0_active.list.first->expiry;
    *head_timer = timers.p0_active.list.first;
  }

  if ( timers.active.list.first != NULL )
  {
    if(timers.active.list.first->expiry < first) 
    {
      first = timers.active.list.first->expiry;
      *head_timer = timers.active.list.first;
    }   
  }
  return first;
}

/*=============================================================================

FUNCTION TIMER_TASK_ISR

DESCRIPTION
  This function detects a timer interrupt, and forwards it to the timer task
  for processing.

DEPENDENCIES
  Must only be called from tramp.

RETURN VALUE
  NULL pointer (No return value).

SIDE EFFECTS
  Uninstalls the timer ISR.
  Signals the timer task to process the expiring timer, which can have
  numerous side effects, depending on the expiring timer.

=============================================================================*/
static const void * timer_task_isr( void* input )
{
  /* Timer sclk time-stamp values */
  #ifdef TIMER_DEBUG_LATE_TIMER_ISR
  time_timetick_type                   set_time, set_value;

  /* Latency in ISR handling */
  time_timetick_type                   latency_sclk;
  #endif /*TIMER_DEBUG_LATE_TIMER_ISR*/

  time_timetick_type                   now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Disable interrupt since QTMR irq remains high */
  /* We need to disable the interrupt before we signal in order to avoid race cond'n */
  DalTimetick_Enable(hTimerHandle,0);

  #ifdef FEATURE_TIME_QDSP6
  /* Signal the timer task of the timer interrupt event */
  (void) qurt_anysignal_set(&timer_q6_signal, TIMER_QURT_SIG_MASK);
  #else /*FEATURE_TIME_QDSP6*/
  /* Signal the timer task of the timer interrupt event */
  (void) rex_set_sigs( timer_tcb_ptr, TIMER_EXPIRY_SIG );
  #endif /*FEATURE_TIME_QDSP6*/

  ATS_INTLOCK( );

  DalTimetick_GetTimetick64(hTimerHandle, &now);  

  /* Record when timer isr actually was handled */
  timers.isr_time = now;

  /* Capture timer set values, for messaging outside of ATS_INTLOCK. */
  #ifdef TIMER_DEBUG_LATE_TIMER_ISR
  set_time  = timers.set_time;
  set_value = timers.set_value;
  #endif /*TIMER_DEBUG_LATE_TIMER_ISR*/

  timers.set_value_last = timers.match_value;
  
  ATS_INTFREE();

  #ifdef TIMER_DEBUG_LATE_TIMER_ISR
  if ( now - set_time >= set_value - set_time)
  {
    /* Determine the difference between when the interrupt was programmed to
       occur at, and when the interrupt was actually handled. */
    latency_sclk = now - set_value;

    if ( latency_sclk > TIMER_SCLK_LATENCY )
    {
      /* Timer interrupt was handled over btw 5 (38MHz) to 10ms (19MHz) late.
         (Timers may expire much later than 5-10ms, if they are in a disabled
         group, or if multiple timers expire due to the same interrupt.
         This is only a measure of the timer isr latency.) */

      ATS_ERR_3("Late Timer ISR: ST=%d SV=%d IT-SV=%d",
                set_time, set_value, latency_sclk);
    }
  }
  else
  {
    ATS_MSG_3("Early Timer ISR: ST=%d SV=%d SV-IT=%d",
             set_time, set_value, set_value - now);
  }
  #endif /*TIMER_DEBUG_LATE_TIMER_ISR*/

  /* Save a trace packet */
  TIMER_TRACE_TS(ISR, now);

  return NULL;
} /* timer_task_isr */


/*=============================================================================

FUNCTION TIMER_SET_NEXT_INTERRUPT

DESCRIPTION
  Program the SLEEP_XTAL_TIMETICK to generate an interrupt at the given value

DEPENDENCIES
  Internal timer_ function.
  Must be called from ATS_INTLOCKED context.

RETURN VALUE
  None

SIDE EFFECTS
  Interrupt when slow clock counter reaches given value

=============================================================================*/

static void timer_set_next_interrupt
(
  /* Slow clock count at which next interrupt will occur */
  time_timetick_type                   match_count,

  /* Current slow clock count */
  time_timetick_type                   ticks_now,

  /* If TRUE, forces MATCH_VAL register to be updated */
  boolean                         force,

  uint8                           caller,

  timer_ptr_internal_type         head_timer
)
{                               

  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Don't re-write the same value to the MATCH_VAL register. 
    Only if the timer_undefer_reference_count is zero, it means
    that NPA is done with its job and we can start programming the interrupt.*/
  if ((force == TRUE || ( match_count != timers.set_value  &&  
         match_count != timers.match_value ))
       && timer_defer_undefer_reference_count == 0)       
  {
    #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    record_match_val[record_match_val_counter].match_value_supplied = match_count;
    record_match_val[record_match_val_counter].timer_address = head_timer;
    record_match_val[record_match_val_counter].timer_match_value_actual_set= MAGIC_INITIALIZER_64;
    record_match_val[record_match_val_counter].timer_now_supplied = ticks_now;
    record_match_val[record_match_val_counter].timer_now_actual = MAGIC_INITIALIZER_64;
    record_match_val[record_match_val_counter].mv_setter = caller;
    #endif

    DalTimetick_SetNextInterrupt64(hTimerHandle,
                                   (DalTimetickTime64Type)match_count - (DalTimetickTime64Type)timers.backoff,
                                   (DalTimetickTime64Type)ticks_now,
                                   &timers.match_value);
    /* Featurize since this function is called in case of sysclock and without */  
    #ifndef FEATURE_TIMER_USE_QURT_SYSCLOCK
    /* Re-enable interrupts */
    DalTimetick_Enable(hTimerHandle,1);
    #endif /*FEATURE_TIMER_USE_QURT_SYSCLOCK*/

    #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    record_match_val[record_match_val_counter].timer_match_value_actual_set = timers.match_value;
    DalTimetick_GetTimetick64(hTimerHandle, &record_match_val[record_match_val_counter].timer_now_actual);
    record_match_val_counter = (record_match_val_counter+1)&(MAX_MATCH_REC - 1);
    /* This is a dummy statement to remove compiler warning */
    record_match_val[record_match_val_counter].timer_now_supplied = \
                  record_match_val[record_match_val_counter].timer_now_supplied;
    #endif

    /* Save a trace packet with the match value */
    TIMER_TRACE(MATCH_VALUE_PROG);     
    
    /* Record when this value was actually written */
    timers.set_time    = ticks_now ;

    /* Record when next timer should expire at */
    timers.set_value = match_count;
  }
  
  /* Record when this match value was logically written, even if we don't
     bother to write the value if it is unchanged. */
  timers.last_set_time = ticks_now ;

} /* timer_set_next_interrupt */


/*=============================================================================

FUNCTION TIMER_UPDATE_TIMER_INTERRUPT

DESCRIPTION
  If the head of the timer list is modified, update the timer match value.
  Will cause an interrupt to be generated immediately if there is an
  expired timer.

DEPENDENCIES
  Internal timer_ function.
  Must be called from ATS_INTLOCKED context.

RETURN VALUE
  None

SIDE EFFECTS
  Interrupt when slow clock counter reaches the first timer's expiry point

=============================================================================*/

static void timer_update_timer_interrupt
(
  uint8 caller
)
{
  /* Current slow clock count */
  time_timetick_type                   now = 0;

  /* Time of first expiring timer */
  time_timetick_type                   first=TIMER_MAX_EXPIRY;
  timer_ptr_internal_type                       head_timer=NULL;

  /* Minimum re-arm provided by DalTimetick call */
  DalTimetickTime32Type                minReArm = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /* NOTE: This function has many early returns. */

  /* Skip updating the interrupts if not allowed */
  if ( FALSE == timers.do_reprogram_isr )
  {
    return;
  }

  /* If a timer is being altered inside "timer_process_active_timers" ... */
  if ( timers.processing )
  {
    /* ... don't bother updating the timer match value!  Caller will. */
    return;
  }



  /* The following two tests deal with timers.set_value, which is the time
     of the Already Programmed timer interrupt, which may have no relation to
     the first timer on the active timer list. */


  /* Get the current time */
  DalTimetick_GetTimetick64(hTimerHandle, &now);


  /* If the timer interrupt has already occurred ... */
  if ( timers.set_value - timers.set_time <= now - timers.set_time )
  {
    /* ... let the Timer ISR/Task update the next timer match value */
    return;
  }


  /* If the timer interrupt is just a little in the future ... */
  DalTimetick_GetMinRearmSclk(hTimerHandle,&minReArm);
  if ( timers.set_value - now <= minReArm )
  {
    /* ... we can't stop it from been generated.  Don't try to. */
    return;
  }

  first = timer_determine_next_timer_expiry(&head_timer);

  /* If the first expiring timer matches the timer.set_value, ... */
  if ( first == timers.set_value )
  {
    /* ... then it is already properly programmed in TIMETICK_MATCH */
    return;
  }

  /* If the first expiring timer is still in the past ... */
  if ( first < now  )
  {
    /* Set the timer for "as soon as possible" (eg, "now") */
    first = now;
  }


  /* Set the next interrupt match value (unless it is unchanged) */
  timer_set_next_interrupt( first, now, FALSE, caller,head_timer );

} /* timer_update_timer_interrupt */


/*=============================================================================

                           TIMER BIN FUNCTIONS

=============================================================================*/


/*=============================================================================

FUNCTION TIMER_BIN_LIST_INSERT_BETWEEN

DESCRIPTION
  Inserts a bin into the specified list between the two specified bin positions 

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_bin_list_insert_between
(
  /* Bin list to insert bin into */
  timer_bin_list_ptr              list, 

  /* Bin to insert in bin list */
  timer_bin_ptr                   bin,

  /* Which node bin should be inserted after */
  timer_bin_ptr                   ptr1,

  /* Which node bin should be inserted before */
  timer_bin_ptr                   ptr2
)
{

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Update our bin's prev/next ptrs to point at correct bin structs */
  bin->prev_bin = ptr1; 
  bin->next_bin = ptr2;


  /* Update surrounding prev/next ptrs (if necessary) to point to our 
     newly inserted bin */

  if( ptr1 != NULL )
  {
    ptr1->next_bin = bin;
  }
  else
  {
    list->first = bin; /* we have a new start of list, update first ptr */
  } 

  if( ptr2 != NULL )
  {
    ptr2->prev_bin = bin;
  }
  else
  {
    list->last = bin; /* we have a new end of list, update last ptr */
  }

  list->count++;

} /* timer_bin_list_insert_between */



/*=============================================================================

FUNCTION TIMER_BIN_LIST_REMOVE

DESCRIPTION
  Removes a bin from the specified list

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  The popped timer_bin_type

SIDE EFFECTS
  None

=============================================================================*/

static void timer_bin_list_remove
(
  /* Bin list to removed bin from */
  timer_bin_list_ptr              list, 

  /* Bin to remove from bin list */
  timer_bin_ptr                   bin
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( bin->prev_bin != NULL )
  {
    bin->prev_bin->next_bin = bin->next_bin;
  }
  else
  {
    /* This was the head, so update first ptr */
    list->first = bin->next_bin;
  }

  if( bin->next_bin != NULL )
  {
    bin->next_bin->prev_bin = bin->prev_bin;
  }
  else
  {
    /* This was the tail, so updated last ptr */
    list->last = bin->prev_bin;
  }

  /* Update list's count */
  list->count--;

} /* timer_bin_list_remove */



/*=============================================================================

FUNCTION TIMER_BIN_LIST_POP

DESCRIPTION
  Pops a bin off the head of the specified list and returns it 

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  The popped timer_bin_type

SIDE EFFECTS
  None

=============================================================================*/

static timer_bin_ptr timer_bin_list_pop
(
  /* Bin list to remove the head bin from */
  timer_bin_list_ptr              list
) 
{
  /* Bin at head of bin list */
  timer_bin_ptr                   bin = list->first;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( bin != NULL )
  {
    timer_bin_list_remove( list, bin );
  }

  return bin;

} /* timer_bin_list_pop */



/*=============================================================================

FUNCTION TIMER_BIN_LIST_PUSH

DESCRIPTION
  Pushes a bin onto the head of the specified list 

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_bin_list_push
(
  /* Bin list to push bin onto */
  timer_bin_list_ptr              list, 

  /* Bin to push onto bin list */
  timer_bin_ptr                   bin
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  timer_bin_list_insert_between( list, bin, NULL, list->first );

} /* timer_bin_list_push */



/*=============================================================================

FUNCTION TIMER_BIN_CREATE_BETWEEN

DESCRIPTION
  Takes a new bin from free bin pool and adds it between the specified bins
  on the active bin list 

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  The created timer_bin_type

SIDE EFFECTS
  None

=============================================================================*/

static timer_bin_ptr timer_bin_create_between
(
  /* Bin to create new bin after */
  timer_bin_ptr                   bin1, 

  /* Bin to create new bin before */
  timer_bin_ptr                   bin2,

  /* Timer which will exist in the bin (used to define bin range) */
  timer_ptr_internal_type         timer
) 
{ 
  /* Newly created bin */
  timer_bin_ptr                   new_bin;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Ensure we have a free bin in the pool */
  if (timers.bins_free.count <= 0)
  	{
  	 ERR_FATAL("timer_bin_create_between: No free bins",0,0,0);
  	}

  /* Pop it off */
  new_bin = timer_bin_list_pop( &timers.bins_free );

  /* Initialize the bin's info */
  new_bin->first  = NULL;
  new_bin->last   = NULL;
  new_bin->count  = 0;

  /* Determine the range of this new bin */
  new_bin->lower = timer->expiry & ~(TIMER_BIN_RANGE); 
  new_bin->upper = new_bin->lower  |  (TIMER_BIN_RANGE); 

  /* finally, compare against prev and next bins to ensure no overlap */

  if( bin1 != NULL )
  {
    /* is the bottom portion of the bin overlapping? */
    if( bin1->upper >= new_bin->lower )
    {
      /* yes, use the upper bound of the prev bin as our upper bound */
      new_bin->lower = bin1->upper + 1;
    }
  }
  if( bin2 != NULL )
  {
    /* is the top portion of the bin overlapping? */
    if( bin2->lower <= new_bin->upper )
    {
      /* yes, use the lower bound of the next bin as our upper bound */
      new_bin->upper = bin2->lower - 1;
    }
  }
        
  timer_bin_list_insert_between( &timers.bins, new_bin, bin1, bin2 );  

  return new_bin;

} /* timer_bin_create_between */



/*=============================================================================

FUNCTION TIMER_BIN_FIND_CREATE

DESCRIPTION
  Searches the active bin list for a bin corresponding to the specified timer  
  If none exists, create it and link it into the bin list

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  The timer_bin_type whose range the timer fits into  

SIDE EFFECTS
  None

=============================================================================*/

static timer_bin_ptr timer_bin_find_create
( 
  /* Timer for which a bin is needed */
  timer_ptr_internal_type        timer
)
{
  /* Pointer to a bin.  Used for searching the list of active timer bins */
  timer_bin_ptr                   bin;

  /* Timer's expiry. */
  time_timetick_type                   expiry;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /* NOTE: This function has many early returns. */

  if (timer->list == NULL)
  	{
  	  ERR_FATAL("timer_bin_find_create: For timer=0x%x, list is NULL",timer,0,0);
  	}

  bin    = timers.bins.first;
  expiry = timer->expiry; 

  while( bin != NULL )
  {
    /* Is this bin able to hold this timer? */

    if ( expiry >= bin->lower  &&  expiry <= bin->upper )
    {
      /* Timer fits in this bin, and it doesn't straddle zero, return it */
      return bin;
    }


    /* Should we create a new bin before this bin? */

    if( expiry < bin->lower )
    {
      /* If it fits between this bin and the prev, or if the prev is NULL
         then create one */

      if( bin->prev_bin == NULL  ||  expiry > bin->prev_bin->upper )
      {
        return timer_bin_create_between( bin->prev_bin, bin, timer );
      }
    }

    /* Doesnt fit in this bin, or before this bin, so iterate to next */
    bin = bin->next_bin;

  } /* while( bin != NULL ) */


  /* If we've gotten to the end, create a bin after the last bin */
  return timer_bin_create_between( timers.bins.last, NULL, timer );

} /* timer_bin_find_create */



/*=============================================================================

FUNCTION TIMER_BIN_FIND_FIRST_EXCEEDING

DESCRIPTION
  Returns the first timer on the bin list exceeding the specified 
  timer's range. If none are found, NULL is returned 

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  The timer meeting the description's criteria.  

SIDE EFFECTS
  None

=============================================================================*/

static timer_ptr_internal_type timer_bin_find_first_exceeding
(
  /* Timer used to determine bin range */
  timer_ptr_internal_type         timer
)
{
  /* First timer in bins */
  timer_ptr_internal_type         first;

  /* Timer bin under consideration */
  timer_bin_ptr                   bin;

  /* Expiry of timer under consideration */
  time_timetick_type                   expiry = timer->expiry;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /* NOTE: This function has many early returns. */


  /* Search the bins until we find one with a timer exceeding this timer's
     expiry */

  /* Skip passed all the unbinned expired timers at the head, if any */

  /*For priority timers, do not insert the timer into the 
  bin. Just return NULL*/
  if(TIMER_PRIORITY_0 == timer->info.timer_priority) 
  {
    return NULL;
  }
  else
  {
    first   = timers.active.list.first;
  }

  while( first != NULL )
  {
    if( first->bin != NULL )
    {
      break;
    }
    first = first->next;
  }

  /* Start with the bin associated with the first on the active list */
  if( first == NULL )
  {
    return NULL;
  }

  for( bin = first->bin;  bin != NULL;  bin = bin->next_bin )
  {
    /* Does this timer fit before the bin's upper bound? */

    if( expiry <= bin->upper )
    {
      /* return the first pointer */ 
      return bin->first;
    }
  }

  /* now continue at the beginning of the bin list and
     progress until we hit the bin of the first timer on the active list */
  for( bin = timers.bins.first; bin != first->bin; bin = bin->next_bin )
  {
    /* Does this timer fit before the bin's upper bound? */

    if( expiry  <= bin->upper )
    {
      /* return the first pointer */ 
      return bin->first;
    }
  }
 
  /* If we've gotten here, return NULL */
  return NULL;

} /* timer_bin_find_first_exceeding */



/*=============================================================================

FUNCTION TIMER_BIN_INSERT

DESCRIPTION
  Inserts the specified timer into the bin.
  This (potentially) involves updating the bin's first/last pointer.

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_bin_insert
(
  /* Bin to insert timer in */
  timer_bin_ptr                   bin,

  /* Timer to insert in bin */
  timer_ptr_internal_type         timer 
)
{
  /* Lowest expiry time in first bin */
  time_timetick_type                   bin_lowest;

  /* Highest expiry time in last bin */
  time_timetick_type                   bin_highest;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Since it is known that a bin does not straddle a counter loop,
     these compares can be done without a zero reference */


  /* Calculate the lowest timer expiry in the bin */

  if( bin->first != NULL )
  {
    bin_lowest  = bin->first->expiry;

    /* do we need to update the first pointer? */
    if( timer->expiry < bin_lowest )
    {
      /* We have a new first pointer */

      /* Note: if the expiries are equal, the first pointer remains the same
         to match the behavior of the timer list */ 
      bin->first = timer;
    } 

  }
  else
  {
    /* If no first pointer, update first pointer with new timer */
    bin->first = timer;
  } 

  /* Calculate the highest timer expiry in the bin */
  if( bin->last != NULL )
  {
    bin_highest = bin->last->expiry;

    /* Do we need to update the last pointer? */
    if( timer->expiry >= bin_highest )
    {
      /* We have a new last pointer */

      /* Note: if the expiries are equal, the last pointer is updated 
         to match the behavior of the timer list */ 
      bin->last = timer;
    }

  }
  else
  {
    /* If no last pointer, update last pointer with new timer */
    bin->last = timer;
  }

  /* Record the bin in the timer struct */
  timer->bin = bin; 

  /* Increment the bin's count */
  bin->count++;

} /* timer_bin_insert */



/*=============================================================================

FUNCTION TIMER_BIN_REMOVE

DESCRIPTION
  Remove the specified timer from the bin.
  This (potentially) involves updating the bin's first/last pointer.

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_bin_remove
(
  /* Timer to remove from bin list */
  timer_ptr_internal_type         timer
)
{
  /* Bin timer belongs to */
  timer_bin_ptr                   bin = timer->bin;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( bin == NULL )
  	{
  	  ERR_FATAL("timer_bin_remove: timer(0x%x)->bin is NULL",timer,0,0);
  	}  

  /* Do we now have an empty bin? */

  if( bin->count == 1 )
  {
    /* empty bin */
    bin->first = NULL;
    bin->last  = NULL;
  }
  else if( bin->count > 1 )
  {
    /* Check if we have a new bin first */
    if( timer == bin->first )
    { 
      bin->first = timer->next;
    }

    /* Check if we have a new bin last */ 
    if( timer == bin->last )
    {
      bin->last = timer->prev;
    }
  }

  /* Unbin the timer */ 
  timer->bin = NULL;

  /* Decrement the bin's count */
  bin->count--;  

  if( bin->count == 0 )
  {
    /* If this bin is now empty, then remove it from the active bin list
       and return it to the free list */
    timer_bin_list_remove( &timers.bins, bin );
    timer_bin_list_push( &timers.bins_free, bin);
  }

} /* timer_bin_remove */



/*=============================================================================

FUNCTION TIMER_INSERT_BETWEEN

DESCRIPTION
  Places 'timer' before 'ptr1' and after 'ptr2' in 'list'.

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void timer_insert_between
(
  /* List to insert timer into */
  timer_list_ptr                  list,

  /* Timer to insert into list */
  timer_ptr_internal_type         timer, 

  /* Timer to insert after */
  timer_ptr_internal_type         ptr1,

  /* Timer to insert before */
  timer_ptr_internal_type         ptr2
)
{

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Update our timer's prev/next ptrs to point at correct timer structs */

  timer->prev = ptr1; 
  timer->next = ptr2;


  /* Update surrounding prev/next ptrs (if necessary) to point to our 
     newly inserted timer */

  if( ptr1 != NULL )
  {
    ptr1->next = timer;
  }
  else
  {
    list->first = timer; /* We have a new start of list, update first ptr */
  } 

  if( ptr2 != NULL )
  {
    ptr2->prev = timer;
  }
  else
  {
    list->last = timer; /* We have a new end of list, update last ptr */
  }
 
} /* timer_insert_between */



/*=============================================================================

FUNCTION TIMER_INSERT

DESCRIPTION
  Inserts a timer into the sorted list of timers.
  Timers are sorted according to increasing expiration times

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_insert
(
  /* List timer is to be inserted into */
  timer_list_ptr                  list,

  /* Timer to be inserted into list of active timers */
  timer_ptr_internal_type         timer
)
{
  /* Pointer to a timer. Used for walking list of timers */
  timer_ptr_internal_type         ptr;

  /* Pointer to a timer. Determines where on the list to start looking */
  timer_ptr_internal_type         start=NULL;
  
  /* Current slow clock count */
  time_timetick_type                   now = 0;
  
  /* bin timetick bounds */
  time_timetick_type                   bin_lower, bin_upper;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  DalTimetick_GetTimetick64(hTimerHandle, &now);

  /* First set the list field of the timer */
  timer->list = list;

  /* if this timer is to be inserted on the active list 
   For priority list, as of now there will be no binning*/
  if( timer->list == &timers.active.list)
  {

    /* If this timer is going on the active list we can use binning 
       to help us cut down on the list search time */

     /* Obtain the first timer from the bin exceeding this timer's expiry */
    start = timer_bin_find_first_exceeding( timer ); 

    /* Increment the number of timers */
    timers.active_timers_count++; 
  }
  else /*it can be disabled or p0 timer*/
  {
    /* If this is not an active timer, do not optimize the search,
       just search from the start of the list */
    start = list->first;

    /*increment the # of active timers if its a priority list*/
    if(timer->list == &timers.p0_active.list) 
    {
      timers.active_timers_count++;
    }
  }

  /*do  not attempt to find bin for priority timers*/
  if(start != NULL && start->bin != NULL)
  {
     /*Debug*/
     if(TIMER_PRIORITY_0 == timer->info.timer_priority) 
     {
       ERR_FATAL("1- Priority timers should not have been binned",0,0,0);
     }
     /*end debug*/

     bin_lower = start->bin->lower;
     bin_upper = start->bin->upper;
     
     /* Ignore the bin's first timer to insert new timer into timers list
        if the 'now is in between bin's upper and lower boundary' and first  
        timer of the bin is before now. In such case, Search from start of
        timers list to insert new timer. */
     if(now > bin_lower && now < bin_upper && start->expiry < now) 
        start = NULL;
  }
  
  /* Initialize the start position */
  if( start != NULL )
  {
    ptr = start;
  }
  else
  {
    ptr = list->first;
  }

  /* Search for appropriate list location to insert timer */
  while ( ptr != NULL &&  ptr->expiry <= timer->expiry )
  {
    ptr = ptr->next;
  }

  /* Insert the timer into the list */
  timer_insert_between( list, timer, ptr ? ptr->prev : list->last, ptr ); 

  /* Only bin this timer if we are inserting into the active list 
   Do not insert the priority timer into the bin*/
  if( timer->list == &timers.active.list)
  {
    /* Insert thie timer into the appropriate timer list bin */
    timer_bin_insert( timer_bin_find_create(timer), timer );
  }

  /* Update the first non-deferrable timer */
  if (timer_internal_groups[timer->info.group_idx]->deferrable == 0 &&
      (timers.first_non_deferrable == NULL ||
       timers.first_non_deferrable->expiry > timer->expiry))
  {
    /* Set this timer as the first non-deferrable
       (regardless of if it's p0 or not) */
    timers.first_non_deferrable = timer;

	/* Notify the clients who are interested in non deferrable timer change */
	timer_notify_first_non_defer_timer_expiry_change();
  }

  /* Throw an error fatal if timer list is corrupted */
  timer_active_list_check();


  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timer_sets[timer_set_cnt].timer_to_be_set = timer->timer_ext->timer_client_ptr;
  timer_sets[timer_set_cnt].int_timer = timer;
  timer_sets[timer_set_cnt].ts = timer->start;
  timer_sets[timer_set_cnt].expiry = timer->expiry;
  timer_sets[timer_set_cnt].tcb_ptr = (rex_tcb_type *)rex_self();
  timer_set_cnt = (timer_set_cnt + 1) & (MAX_TIMER_SET - 1);
  timer_set_cnt_global++;
  
  /* This is a dummy statement to remove compiler warnings */
  timer_sets[timer_set_cnt].ts = timer_sets[timer_set_cnt].ts;
  #endif 

} /* timer_insert */



/*=============================================================================

FUNCTION TIMER_INSERT_LIST

DESCRIPTION
  Inserts a sorted list of timers into the sorted list of timers.
  Timers are sorted according to increasing expiration times

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_insert_list
(
  /* List timers are to be inserted into */
  timer_list_ptr                  dst_list,

  /* Linked list of timers to be inserted into the first list */
  timer_ptr_internal_type         src_list
)
{
  /* A timer from the second list, to be inserted in the first */
  timer_ptr_internal_type         timer;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Repeat for all timers on the source list */
  while ( src_list != NULL )
  {
    /* Retrieve a timer from the start of the source list */
    timer    = src_list;
    src_list = src_list->next;

    timer_insert( dst_list, timer );
  }

} /* timer_insert_list */



/*=============================================================================

FUNCTION TIMER_CONCATENATE_LISTS

DESCRIPTION
  Concatenates the src_list and the dst_list with the combined list taking on
  the identity of the dst_list.  src_list is cleared.

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_concatenate_lists
(
  /* Linked list of timers to be inserted into the first list */
  timer_list_ptr                  src_list,

  /* List timers are to be inserted into */
  timer_list_ptr                  dst_list
)
{
  /* timer iteration pointer */
  timer_ptr_internal_type         timer;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( src_list == NULL || dst_list == NULL )
  	{
  	  ERR_FATAL("timer_concatenate_lists: NULL src_list=0x%x or dst_list=0x%x",
	  	         src_list,dst_list,0);
  	}

  /* Check if the source list is empty */
  if( src_list->first == NULL )
  {
    return;
  }

  /* Stitch the two lists together */

  /* Is the destination list empty? */
  if( dst_list->first != NULL )
  { 
    /* No, so update the head's previous pointer */
    dst_list->first->prev = src_list->last;

    /* Update the source list's last next pointer */
    src_list->last->next = dst_list->first; 

  }
  else
  {
    /* Yes, so update the destination list's last pointer */
    dst_list->last = src_list->last;
  }

  /* Update the destination lists's first pointer */
  dst_list->first = src_list->first;
 
  /* Now, for each of the src list items, update their structures */
  timer = src_list->first;
  while( timer != NULL )
  {
    /* Update the list membership */
    timer->list = dst_list;

    /* If this is the last of the src list, we are done */
    if( timer == src_list->last )
    {
      break;
    }

    /* Proceed to next timer */
    timer = timer->next; 
  }

  /* Finally update the source list */
  src_list->first = NULL;
  src_list->last  = NULL;
    
} /* timer_concatenate_lists */



/*=============================================================================

FUNCTION TIMER_UNLINK

DESCRIPTION
  Simply unlinks a timer from its current list of timers.
  Updates the associated list first/last ptrs

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_unlink
(
  /* Timer to remove from its current list */
  timer_ptr_internal_type        timer
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( timer->prev != NULL )
  {
    timer->prev->next = timer->next;
  }
  else
  {
    /* This was the head, so update first ptr */
    timer->list->first = timer->next;
  }

  if( timer->next != NULL )
  {
    timer->next->prev = timer->prev;
  }
  else
  {
    /* This was the tail, so updated last ptr */
    timer->list->last = timer->prev;
  }

} /* timer_unlink */

/*=============================================================================

FUNCTION TIMER_GET_INTERNAL_TIMER

DESCRIPTION
  This function returns pointer to internal timer.

DEPENDENCIES
  None.

RETURN VALUE
  Returns relevant pointer to internal timer structure. 
  If handle is invalid returns NULL.

SIDE EFFECTS
  This function should be called inside an intlock or a mutex.

=============================================================================*/

static timer_ptr_internal_type timer_get_internal_timer
(
  timer_client_ptr_type   timer
)
{
  int32 temp_index;
  timer_ptr_internal_type temp_int = NULL;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if(timer == NULL)
     return temp_int;
  
  temp_index = HANDLE_TO_INDEX( timer->timer_int_handle );

  if ( temp_index == TIMER_BUFFER_INVALID_HANDLE )
  {
    return temp_int;
  }

  if ( temp_index > MIN_TIMER_BUFFER_NUMBER
       && temp_index < MAX_TIMER_BUFFER_NUMBER )
  {
    temp_int = &(timer_buffer_ptr[temp_index]);    
  }
  return temp_int;
} /* timer_get_internal_timer */

/*=============================================================================

FUNCTION TIMER_FREE_INTERNAL_TIMER

DESCRIPTION
  1. Puts the timer back in free q
  
DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void timer_free_internal_timer
(
  timer_ptr_internal_type timer  
)
{
  /* Temporary index */
  int32                          temp_index;

   if ( timer->info.node_state == NODE_IS_FREE )
   { 
     ERR_FATAL ( "Removing int_timer which is not active",0,0,0);
     return;
   }
   
   /* Save index value in a temp var */
   temp_index = timer->index;

   /* Clear expired timer buffer */
   memset( timer, 0, sizeof( timer_internal_type ));

   /* Restore timer index*/
   timer->index = temp_index ;

   timer->info.node_state = NODE_IS_FREE;
  
   /* Link timer_buffer element back to timer_free_q */
   (void) q_link( timer, &(timer->link) );

   /* Enqueue node in timer_free_q */
   q_put( &timer_free_q, &(timer->link) );   

   timer_count_buffer_allocated = MAX_TIMER_BUFFER_NUMBER - q_cnt(&timer_free_q);
} /* timer_free_internal_timer */



/*=============================================================================

FUNCTION TIMER_REMOVE

DESCRIPTION
  Removes a timer from the list of timers.

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_remove
(
  /* Timer to be removed from list of active timers */
  timer_ptr_internal_type		  timer
)
{

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( timer == NULL || timer->list == NULL )
  {
    return;
  }

  if( timer->list == &timers.active.list 
      || timer->list == &timers.p0_active.list )
  {
    /* Decrement the number of timers if timer belongs to active list */
    timers.active_timers_count--; 
  }

  /* if this timer is to be removed from the active list */
  if( timer->bin != NULL )
  {

    /*debug*/
    if(TIMER_PRIORITY_0 == timer->info.timer_priority) 
    {
      ERR_FATAL("2- Priority timers should not have been binned",0,0,0);
    }
    /*end debug*/

    /* Unbin this timer */
    timer_bin_remove( timer );
  }

  /* Fix up links/list around this node */

  if ( timer->prev )
  {
    timer->prev->next = timer->next;
  }
  else
  {
    timer->list->first = timer->next;
  }

  if ( timer->next )
  {
    timer->next->prev = timer->prev;
  }                                 
  else
  {
    timer->list->last = timer->prev;
  }


  timer->next = NULL;
  timer->prev = NULL;
  timer->list = NULL;

  /* Update the first non-deferrable timer */
  if (timers.first_non_deferrable == timer)
  {
    /* Since the first non-deferrable timer was removed,
       we need to get the next non-deferrable timer */
    timer_update_first_active_non_deferrable_timer();
  }

  /* Throw an error fatal if timer list is corrupted */
  timer_active_list_check();

} /* timer_remove */




/*=============================================================================

FUNCTION TIMER_REMOVE_HEAD

DESCRIPTION
  Removes the head of the specified timer list.

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_remove_head
(
  /* List to chop head off of */
  timer_list_ptr                  list
)
{
  /* Head of the timer list */
  timer_ptr_internal_type         head = list->first;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( list == &timers.active.list
      || list == &timers.p0_active.list )
  {
    /* Decrement the number of timers if timer belongs to active list */
    timers.active_timers_count--; 
  }

  /* New front of list */
  list->first = head->next;

  /* Fix up links/list around this node */
  if( list->first != NULL )
  {
    /* Update next node's prev ptr */
    list->first->prev = NULL;
  }
  else
  {
    /* Empty list */
    list->last = NULL;
  }


  /* NULL out deleted node's link fields */
  head->next = NULL;
  head->list = NULL;

  /* Update the first non-deferrable timer */
  if (timers.first_non_deferrable == head)
  {
    /* Since the first non-deferrable timer was removed,
       we need to get the next non-deferrable timer */
    timer_update_first_active_non_deferrable_timer();
  }

  /* Throw an error fatal if timer list is corrupted */
  timer_active_list_check();

} /* timer_remove_head */



/*=============================================================================

FUNCTION TIMER_APPEND

DESCRIPTION
  Appends the list with a timer
  The appended timer's list member is updated

DEPENDENCIES
  Internal timer_ function.
  Must be called from inside ATS_INTLOCK.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_append
(
  /* List to add timer to */
  timer_list_ptr                  list,

  /* Timer to add to list */
  timer_ptr_internal_type         timer
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Update timer's links */
  timer->prev = list->last;
  timer->next = NULL;

  /* Update the old tail to point to this new timer */
  if( list->last != NULL )
  {
    list->last->next = timer;
  }
  else
  {
    /* Empty list, have a new head */
    list->first = timer;
  } 
  
  /* Always have a new tail */ 
  list->last = timer;
 
  /* We have a new list for timer */ 
  timer->list = list;

} /* timer_append */

/*=============================================================================
FUNCTION TIMER_DRV_SET_REMOTE_PROCESS_CB_HANDLER

Sets the API that needs to be called when a remote process timer expires.
We will provide to this callback function, the timer callback details
=============================================================================*/
void timer_drv_set_remote_process_cb_handler(timer_process_cb_handler shim_cb_func)
{
   timer_remote_callback_handler = shim_cb_func;
} /* timer_drv_set_remote_process_cb_handler */

/*=============================================================================
FUNCTION TIMER_DRV_SET_LOCAL_PROCESS_IDX

Local Process Index needs to be set by timer shim layer so that remote timers
can be differentiated from local process timers
=============================================================================*/
void timer_drv_set_local_process_idx(uint32 local_process_index)
{
   timer_curr_process_idx = local_process_index;
} /* timer_drv_set_local_process_idx */

/*=============================================================================

FUNCTION TIMER_REMOTE_EXPIRE

DESCRIPTION
  Calls Shim layer callback function which which will push client timer's callback
   details into that process pipe/queue according to its implementation.

DEPENDENCIES
  Internal timer_function()

RETURN VALUE
  None

SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/
static void timer_remote_expire
(
  /* Expiring timer to be processed */
  timer_client_ptr_type           timer,

  /* Duration timer ran for. */
  time_timetick_type              run_time_ms,
  
  time_timetick_type*             time_now
)
{
  /*to logs when the cb processing starts*/
  time_timetick_type cb_processing_started = 0;

  /*to log when the cb processing ends*/
  time_timetick_type cb_processing_ended = 0;

  /*to log the processing time*/
  time_timetick_type cb_processing_time = MAGIC_INITIALIZER_64;
  
  /*fn address*/
  timer_t1_cb_type  fn_address_of_timer = timer->cbfn.func1;

  /* Last set value of timer */
  uint32 setlastval = 0;

  /* Last set value of timer */
  uint32 curr_latency = 0;
  rlc_error_type rlc_error=RLC_SUCCESS;

  #ifdef FEATURE_TIMER_SLAVE1
  timer_slave_post_cmd_params_type slave_cmd;
  memset(&slave_cmd, 0, sizeof(timer_slave_post_cmd_params_type));
  #endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  //#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timers_remote_expired[timer_remote_expired_cnt].timer_expired  	= timer->timer_client_ptr;
  timers_remote_expired[timer_remote_expired_cnt].int_timer       = timer_get_internal_timer(timer);
  timers_remote_expired[timer_remote_expired_cnt].info	        = timer->info;
  timers_remote_expired[timer_remote_expired_cnt].processing_started = 0;
  timers_remote_expired[timer_remote_expired_cnt].processing_time	= MAGIC_INITIALIZER_64;
  timers_remote_expired[timer_remote_expired_cnt].tcb_sigs_obj	= timer->tcb;
  #endif

  /*log the time when processing started*/
  cb_processing_started = *time_now;

  //#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  /*Log the time to indicate the start of processing of this timer*/
  timers_remote_expired[timer_remote_expired_cnt].processing_started = cb_processing_started;
  #endif

  /* Call Shim callback function Initially which should be 
     called back for expiring other process related timers */
  if(timer_remote_callback_handler == NULL)
  {
     ERR_FATAL("No Shim Cb func. Call timer_set_remote_process_timer_handler()", 0, 0, 0);
     return;
  }
  
  if ( timer->info.callback_type == TIMER_NATIVE_OS_SIGNAL_TYPE   ||
       timer->info.callback_type == TIMER_NATIVE_OS_SIGNAL2_TYPE  ||
       timer->info.callback_type == TIMER_REX_TCB_SIGNAL_TYPE     ||
       timer->info.callback_type == TIMER_SIGS_FUNC_DUAL_NOTIFY  ||
       timer->info.callback_type == TIMER_FUNC1_CB_TYPE           ||
       timer->info.callback_type == TIMER_FUNC2_CB_TYPE)
  {
    /* Call Shim layer callback which will push callback details into process pipe/queue */
    timer_remote_callback_handler(timer, run_time_ms);
  }
  else if( timer->info.callback_type != TIMER_NO_NOTIFY_TYPE )
  {
     ERR_FATAL("callback type seems to be corrupted, timer: 0x%x, callback_type: %d, func obj: 0x%x", 
                                                                        timer->timer_client_ptr, 
                                                                        timer->info.callback_type, 
                                                                        fn_address_of_timer);
  }
  
  /*log the cb ended time*/
  DalTimetick_GetTimetick64(hTimerHandle, &cb_processing_ended);
  *time_now = cb_processing_ended;
  
  /*to take care of the wrap around case*/
  if(cb_processing_ended >= cb_processing_started)
  {
    cb_processing_time = cb_processing_ended - cb_processing_started;       
  }
  else /*if it is a wrap around case*/
  {  
    cb_processing_time = 0xFFFFFFFFFFFFFFuLL - 
                        (cb_processing_started - cb_processing_ended)+1;
  }

  //#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timers_remote_expired[timer_remote_expired_cnt].processing_time = cb_processing_time;

  {
    timer_remote_expired_cnt = (timer_remote_expired_cnt+1)&(MAX_TIMER_REMOTE_EXPIRED-1);
    timer_remote_expired_cnt_global++;
  }
  #endif /* FEATURE_ENABLE_TIMER_DEBUG_VARIABLES */

  /* generate a warning or error fatal out if the processing time > threshold*/
  #if (defined FEATURE_ENABLE_TIMER_DEBUG_VARIABLES)
  if(cb_processing_time > timer_processing_time_warning_threshold)
  {
    #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    ATS_ERR_3("callback fn = 0x%x took %d ticks, max = %d ticks",
               fn_address_of_timer,
               cb_processing_time,
               timer_processing_time_warning_threshold);          
    #endif    
  }
  

  if(timer_processing_time_err_fatal_threshold != 0 && 
     cb_processing_time > timer_processing_time_err_fatal_threshold)
  {
    #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    ERR_FATAL("callback fn = 0x%x took %d ticks, err thre = %d ticks",
               fn_address_of_timer,
               cb_processing_time,
               timer_processing_time_err_fatal_threshold);          
    #endif 
   
  }

  #endif /*(defined FEATURE_ENABLE_TIMER_DEBUG_VARIABLES)*/

 if(timers.prev_learned_isr_time != timers.isr_time)
  {
    ATS_INTLOCK();
    setlastval = timers.set_value_last;
    timers.prev_learned_isr_time = timers.isr_time;
    ATS_INTFREE();

	curr_latency = ( cb_processing_started - setlastval);
	
    if ( ( cb_processing_started >= setlastval ) &&
         ( ( timers.backoff == 0 ) ||
		   ( curr_latency > ( timers.backoff + ( timers.backoff / 8 ) ) ) ||
		   ( curr_latency < (timers.backoff - (timers.backoff / 8 ) ) ) ) )
    {
      rlc_error = rlc_learn_data(rlc_attribute_learn, RLC_LEARN_ASYNC_MODE | RLC_LEARN_SYNC_MODE, 1, cb_processing_started - setlastval); 

      if(rlc_error != RLC_SUCCESS && rlc_error != RLC_FULL && rlc_error != RLC_LEARNING)
      {
        ATS_ERR_1("Error returned from RLC! Error: 0x%x\n", rlc_error);
      }
    }
  }

} /* timer_remote_expire */


/*=============================================================================

FUNCTION TIMER_EXPIRE

DESCRIPTION
  Processes an expiring timer, according to the timer's definition.

DEPENDENCIES
  Internal timer_ function()
  Must be called from inside ATS_INTLOCK & TASKLOCK (or interrupt) contexts

RETURN VALUE
  None

SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/

static void timer_expire
(
  /* Expiring timer to be processed */
  timer_client_ptr_type           timer,

  /* Duration timer ran for. */
  time_timetick_type              run_time_ms,
  
  time_timetick_type*             time_now
)
{
  /* Timer 'type 1' calllback function */
  timer_t1_cb_type                cb1;

  /* Timer 'type 2' calllback function */
  timer_t2_cb_type                cb2;

  /*to logs when the cb processing starts*/
  time_timetick_type cb_processing_started = 0;

  /*to log when the cb processing ends*/
  time_timetick_type cb_processing_ended = 0;

  /*to log the processing time*/
  time_timetick_type cb_processing_time = MAGIC_INITIALIZER_64;
  
  /*fn address*/
  uint32  fn_address_of_timer = 0;

  /*fn callback type*/
  uint8 fn_cb_type = 0;
    
  /* Last set value of timer */
  uint32 setlastval = 0;

  /* Last set value of timer */
  uint32 curr_latency = 0;
  rlc_error_type rlc_error=RLC_SUCCESS;

  #ifdef FEATURE_TIMER_SLAVE1
  timer_slave_post_cmd_params_type slave_cmd;
  memset(&slave_cmd, 0, sizeof(timer_slave_post_cmd_params_type));
  #endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Cache information from timer structure since it could get deleted any
  time after the first callback is made */
  fn_cb_type = (uint8)timer->info.callback_type;
  cb1 = timer->cbfn.func1;
  cb2 = timer->cbfn.func2;

  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timers_expired[timer_expired_cnt].timer_expired  	= timer->timer_client_ptr;
  timers_expired[timer_expired_cnt].int_timer       = timer_get_internal_timer(timer);
  timers_expired[timer_expired_cnt].info	        = timer->info;
  timers_expired[timer_expired_cnt].processing_time	= MAGIC_INITIALIZER_64;
  timers_expired[timer_expired_cnt].tcb_sigs_obj	= timer->tcb;
  #endif

  /*log the time when processing started*/
  cb_processing_started = *time_now;

  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  /*Log the time to indicate the start of processing of this timer*/
  timers_expired[timer_expired_cnt].processing_started = cb_processing_started;
  #endif



  /* If there are signals to be set, and a task / signal object to set the signals to,
     then set those signals in that task's task control block or the passed signal object */

  if ( fn_cb_type == TIMER_NATIVE_OS_SIGNAL_TYPE )
   {
     #ifdef FEATURE_TIME_QDSP6
       qurt_anysignal_set((qurt_anysignal_t *)timer->tcb, (time_osal_notify_data)timer->sigs);
	 #else
	   ERR_FATAL("Signal type cannot be NATIVE OS type ; timer = 0x%x",timer->timer_ext,0,0);
	 #endif /* FEATURE_TIME_QDSP6 */	 
   }
  else if ( fn_cb_type == TIMER_NATIVE_OS_SIGNAL2_TYPE )
  {
     #ifdef FEATURE_TIME_QDSP6
       qurt_signal2_set((qurt_signal2_t *)timer->tcb, (time_osal_notify_data)timer->sigs);
	 #else
	   ERR_FATAL("Signal type cannot be NATIVE OS type ; timer = 0x%x",timer->timer_ext,0,0);
	 #endif /* FEATURE_TIME_QDSP6 */
  }  
  else if ( fn_cb_type == TIMER_REX_TCB_SIGNAL_TYPE ||
  	        fn_cb_type == TIMER_SIGS_FUNC_DUAL_NOTIFY )
  {
    #ifdef FEATURE_TIMER_REX_IS_AVAILABLE
      (void) rex_set_sigs( timer->tcb, timer->sigs );
	#else
	  ERR_FATAL("Signal type cannot be rex type ; timer = 0x%x",timer->timer_ext,0,0);
	#endif
  }

   /* Release the MUTEX which was acquired in 
   timer_process_active_timers. The idea is
   to hold this mutex only till the expiry
   signalling notification is completed. */
   ATS_INTFREE_SAV( isave );
 
   /* Dalsys event signalling is done outside of the mutex 
   since it can trigger a DalSys callback event as well */
  if ( fn_cb_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE)  
  {      
    timer_dalsys_sig_set(timer->tcb);  
  }  

  /* If the timer was defined with a type-1 style function callback,
     call the function, or post the function as an asynchronous call */
  if ( (fn_cb_type == TIMER_FUNC1_CB_TYPE || 
  	    fn_cb_type == TIMER_SIGS_FUNC_DUAL_NOTIFY) && cb1 != NULL )
  {
    fn_address_of_timer = (uint32)cb1;
    
      /*queue it to slave task only if it's not priority timer*/
      if(TIMER_PRIORITY_0 != timer->info.timer_priority)
      { 
        /* Can't queue APC -- call it ourselves */
        #ifdef FEATURE_TIMER_SLAVE1
        slave_cmd.timer_ext = timer->timer_client_ptr;
        slave_cmd.cb_type = TIMER_SLAVE_CB_TYPE1;
        slave_cmd.cb1 = cb1;
        slave_cmd.data = timer->data;
        slave_cmd.timer_slave_task_num = timer->info.slave_task_idx;      
        if (1==timer_after_pc_mask[TIMER_PC_MASK_INDEX]) 
        {                                                
           TIMER_PC_MASK_MUTEX_LOCK();                    
           timer_after_pc_mask[slave_cmd.timer_slave_task_num+1]++;  
           TIMER_PC_MASK_MUTEX_UNLOCK();               
        }                                                  
        timer_slave_post_cmd(&slave_cmd);
        #else
        cb1( timer->data );
        #endif
      }
      else
      {
        cb1(timer->data);
      }
  } /* timer->func1 != NULL */

  /* If the timer was defined with a type-2 style function callback,
     (clock callback style), call the function with the time (in
     milliseconds) between timer start and now. */
  else if ( fn_cb_type == TIMER_FUNC2_CB_TYPE  && cb2 != NULL )
  {
    fn_address_of_timer = (uint32)cb2;

    if(TIMER_PRIORITY_0 != timer->info.timer_priority) 
    {
    /* Call the callback function */
    #ifdef FEATURE_TIMER_SLAVE1
     slave_cmd.timer_ext = timer->timer_client_ptr;
     slave_cmd.cb_type = TIMER_SLAVE_CB_TYPE2;
     slave_cmd.cb2 = cb2;
     slave_cmd.run_time = (time_timetick_type)run_time_ms;
     slave_cmd.data = timer->data;
     slave_cmd.timer_slave_task_num = timer->info.slave_task_idx;
    if (1==timer_after_pc_mask[TIMER_PC_MASK_INDEX]) 
    {                                                
       TIMER_PC_MASK_MUTEX_LOCK();                  
       timer_after_pc_mask[slave_cmd.timer_slave_task_num+1]++;  
       TIMER_PC_MASK_MUTEX_UNLOCK();                
    }                                                  
    timer_slave_post_cmd(&slave_cmd);
    #else
     cb2( (time_timetick_type) run_time_ms, timer->data);
    #endif
    }
    else
    {
      cb2( (int32) run_time_ms, timer->data);
    }
  }

 #if 0 // Currently this check will need a lot of conditions to parse
 /* Check if this timers has no notification types or if it is invalid */
  else if (fn_cb_type == TIMER_NO_NOTIFY_TYPE)
  	{
  	  // ATS_MSG_2("Timer = 0x%x expired; External timer = 0x%x",timer,timer->timer_ext);
  	}
  else
  	{
  	  ERR_FATAL("Timer = 0x%x : INVALID NOTIFICATION TYPE = %d",timer,timer->info.callback_type,0);
  	}
 #endif

  /*log the cb ended time*/
  DalTimetick_GetTimetick64(hTimerHandle, &cb_processing_ended);
  
  /*to take care of the wrap around case*/
  if(cb_processing_ended >= cb_processing_started)
  {
    cb_processing_time = cb_processing_ended - cb_processing_started;       
  }
  else /*if it is a wrap around case*/
  {  
    cb_processing_time = 0xFFFFFFFFFFFFFFuLL - 
                        (cb_processing_started - cb_processing_ended)+1;
  }

  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timers_expired[timer_expired_cnt].processing_time = cb_processing_time;

  /*if the timer slave processing is not enabled, then 
    unconditionally increment the counter.
    if the timer slave processing is enabled, then increment 
    only if the signals were set*/
  if((FALSE == timer_slave_processing_enabled)
     || (timer_slave_processing_enabled && ( (fn_cb_type == TIMER_REX_TCB_SIGNAL_TYPE) ||
                                             (fn_cb_type == TIMER_NO_NOTIFY_TYPE) ||
                                             (fn_cb_type == TIMER_NATIVE_OS_SIGNAL2_TYPE) ||
                                             (fn_cb_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE) ||                                             
                                             (fn_cb_type == TIMER_NATIVE_OS_SIGNAL_TYPE) )))
  {
  timer_expired_cnt = (timer_expired_cnt+1)&(MAX_TIMER_EXPIRED-1);
  timer_expired_cnt_global++;
  }
  #endif

  /*generate a warning or error fatal out if the processing time > threshold*/
  #if (defined FEATURE_ENABLE_TIMER_DEBUG_VARIABLES)
  if(cb_processing_time > timer_processing_time_warning_threshold)
  {
    #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    ATS_ERR_3("Processing of callback fn = 0x%x took 0x%x ticks, threshold = 0x%x ticks",
               fn_address_of_timer,
               cb_processing_time,
               timer_processing_time_warning_threshold);          
    #endif    
  }

  if(timer_processing_time_err_fatal_threshold != 0 && 
     cb_processing_time > timer_processing_time_err_fatal_threshold)
  {
    #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    ERR_FATAL("Processing of callback fn = 0x%x took 0x%x ticks, threshold = 0x%x ticks",
               fn_address_of_timer,
               cb_processing_time,
               timer_processing_time_err_fatal_threshold);          
    #endif 
   
  }

  #endif /*(defined FEATURE_ENABLE_TIMER_DEBUG_VARIABLES)*/

 if(timers.prev_learned_isr_time != timers.isr_time)
  {
    ATS_INTLOCK();
    setlastval = timers.set_value_last;
    timers.prev_learned_isr_time = timers.isr_time;
    ATS_INTFREE();

	curr_latency = ( cb_processing_started - setlastval);
	
    if ( ( cb_processing_started >= setlastval ) &&
         ( ( timers.backoff == 0 ) ||
		   ( curr_latency > ( timers.backoff + ( timers.backoff / 8 ) ) ) ||
		   ( curr_latency < (timers.backoff - (timers.backoff / 8 ) ) ) ) )
    {
      rlc_error = rlc_learn_data(rlc_attribute_learn, RLC_LEARN_ASYNC_MODE | RLC_LEARN_SYNC_MODE, 1, cb_processing_started - setlastval); 

      if(rlc_error != RLC_SUCCESS && rlc_error != RLC_FULL && rlc_error != RLC_LEARNING)
      {
        ATS_ERR_1("Error returned from RLC! Error: 0x%x\n", rlc_error);
      }
    }
  }

} /* timer_expire */

/*=============================================================================

FUNCTION TIMER_NEXT_TIMER_TO_EXPIRE

DESCRIPTION
  Determines the next timer to expire from the active list of timers

DEPENDENCIES
  Internal timer_ function()
  Must be called from inside ATS_INTLOCK & TASKLOCK (or interrupt) contexts

RETURN VALUE
  Pointer to the next timer eligible for expiry

SIDE EFFECTS
  None

=============================================================================*/
static timer_ptr_internal_type timer_next_timer_to_expire
(
  time_timetick_type ticks_now
)
{
  time_timetick_type tolerance = ticks_now + TIMER_OS_EARLY_TOL;

  /*first check the p0 list*/
  if(timers.p0_active.list.first
     && (timers.p0_active.list.first->expiry <= tolerance)) 
  {
    return timers.p0_active.list.first;
  }
  else if(timers.active.list.first
     && (timers.active.list.first->expiry <= tolerance)) 
  {
    return timers.active.list.first;
  }
  else
  {
    return NULL; 
  }
}


/*=============================================================================

FUNCTION TIMER_PROCESS_ACTIVE_TIMERS

DESCRIPTION
  Process timers, decrementing elapsed time from each one.

DEPENDENCIES
  Internal timer_ function()
  Must be called from inside ATS_INTLOCK & TASKLOCK (or interrupt) contexts, to
  prevent a large delay between reading the current timetick count, and
  writing the new timetick match value.

RETURN VALUE
  None

SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/

static void timer_process_active_timers( void )
{
  /* Current slow clock count */
  time_timetick_type                   ticks_now = 0;

  /* Timer being processed */
  timer_ptr_internal_type         timer;

  /* Length of time the timer ran for */
  time_timetick_type                   run_time;
  
    /* Duration timer ran for, in milliseconds */
  time_timetick_type                   run_time_ms = 0;

  /* Duration timer ran for, in milliseconds */
  time_timetick_type                   run_time_sclk;


  /* Minimum advance required for reloading timer */
  time_timetick_type                   min_advance;

  /* Interrupt locking status */
  ATS_DECLARE_ISAV(isave);

  /* Temporary value to compute the new expiry point */
  time_timetick_type                   new_expiry;

  /* Temporary value to compute the next expiry point */
  time_timetick_type                   next_timer_expiry;
  
  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  /* Temporary value to compute the new expiry point */
  time_timetick_type                   local_timer_expiry;
  #endif

  /* Pointer  to the head timer from the active list for next expiry */
  timer_ptr_internal_type              head_timer=NULL;

  /* Client timer ptr */
  timer_client_ptr_type                clnt_timer;
  
  /* Temp client timer object */
  timer_client_type                    temp_clnt_timer;

  timer_list_ptr                       current_list = NULL;

  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Lock interrupts while testing & manipulating the active timer list */
  ATS_INTLOCK_SAV( isave );

  /* Record the fact that timers are being processed, to prevent re-entry
     into this function, and unnecessary modification of TIMETICK_MATCH. */
  timers.processing = TRUE;

  /* Get current slow clock count */
  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);

  /* Save a trace packet */
  TIMER_TRACE_TS(PROCESS_START,ticks_now);


  /* Check to determine if the timer at head of the active timer list has
     past its expiry point.  If it has, remove it, expire the timer, and
     repeat with the new timer at the head of active timer list.
       
     Note: a timer's expiry can change timers.active.list.zero, so no local
     variable should hold a calculated value involving it from one iteration
     to the next. */

  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timer_while_cnt = 0;
  #endif  
   
  while ( NULL != (timer = timer_next_timer_to_expire(ticks_now)))          
  {
    /* Get Client Timer obj */
    clnt_timer = timer->timer_ext;
    if(clnt_timer->timer_state != TIMER_SET_FLAG && clnt_timer->timer_state != TIMER_RESUMED_FLAG)
       ERR_FATAL("Timer corruption. state: 0x%x", clnt_timer->timer_state, 0, 0);

	#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
	timer_while_cnt++;
    #endif

	/*populate the current list*/
    current_list = timer->list;
 
	
    if( timer->bin != NULL )
    {
      /*debug*/
      if(TIMER_PRIORITY_0 == timer->info.timer_priority) 
      {
        ERR_FATAL("3- Priority timers should not have been binned",0,0,0);
      }

      /* Unbin this timer */
      timer_bin_remove( timer );
    }

#ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    
    timer_expired_debug[timer_expired_debug_cnt].whileloopcnt = timer_while_cnt;
    timer_expired_debug[timer_expired_debug_cnt].timer_exp_ptr = clnt_timer->timer_client_ptr;
	timer_expired_debug[timer_expired_debug_cnt].clnt_timer = clnt_timer;
	timer_expired_debug[timer_expired_debug_cnt].int_timer = timer;
    timer_expired_debug[timer_expired_debug_cnt].ts = ticks_now;
    timer_expired_debug[timer_expired_debug_cnt].start = timer->start;
    timer_expired_debug[timer_expired_debug_cnt].expiry = timer->expiry;
    timer_expired_debug[timer_expired_debug_cnt].info = clnt_timer->info;
    if( clnt_timer->tcb != NULL && clnt_timer->sigs != 0 )
    {
	  timer_expired_debug[timer_expired_debug_cnt].tcb_or_func = clnt_timer->tcb;
    }
    else
    {
      timer_expired_debug[timer_expired_debug_cnt].tcb_or_func = clnt_timer->cbfn.func1;
    }

    timer_expired_debug_cnt = (timer_expired_debug_cnt+1)&(MAX_TIMER_EXPIRED_DEBUG-1);
#endif /*FEATURE_ENABLE_TIMER_DEBUG_VARIABLES*/

    /* Remove expiring timer from active timer list */
    timer_remove_head( current_list );
      
    /* Compute length of time the timer ran for.  This may be longer than
       programmed due to sleep or other delays in expiry processing. */
    run_time = ticks_now - timer->start;

    /* Compute time (in milliseconds) timer has been running for.
       This may be larger than expected if the timer expires during sleep,
       or otherwise has its expiry processing delayed */
	DalTimetick_CvtFromTimetick64(hTimerHandle,run_time,T_MSEC,&run_time_ms);

    #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    local_timer_expiry = timer->expiry;
    #endif

    /* Reactivate timer, if required */
    if ( timer->reload > 0 )
    {
      /* Determine how late the timer expired; this is the minimum
         amount the timer must be advanced by for the next expiry. */
      min_advance = ticks_now - timer->expiry;
  
      new_expiry = timer->expiry + timer->reload;
      
      if(new_expiry  <= ticks_now)
      {
        /* Temporary value to compute the new expiry point */
        new_expiry = timer->expiry;
        
        /* Timer expired 1 or more reload period ago.  This can happen if
           the timer belongs to a timer group which gets disabled, such as
           the default timer groups during sleep. */

        /* Round min_advance up to the next multiple of reload periods. */
        min_advance += timer->reload - min_advance % timer->reload;

        /* Add the rounded-up minimum advance to the timer expiry */
        new_expiry += min_advance;
      }

        /* Check to make sure that the new expiry point is further in the future
           than the old one.  This prevents the cases where overflow in the
           calculation could occur or wrap around past the active timer list
           zero. */
        
        if (new_expiry > timer->expiry )
        {
          /* New expiry point is further in the future than the old one, use it */
          timer->expiry = new_expiry;
        }
        else
        {
          /* The recurring timer’s new expiry point is at a point so far in the 
             future that it exceeds timer dynamic range.  Because of this, move  
             the expiry to just fit the available range */

          /* Move the expiry point as far out as possible */
          timer->expiry = TIMER_MAX_EXPIRY;
      
          ATS_ERR("Timer expiry set to max value. Timer may never expire !");
        }

      /* Record the new start time for the next expiry */
      timer->start = ticks_now;
      
        /* If a timer is a reloading timer, the millisecond conversion will have
           a rounding error.  We can adjust the start time to account for the
           residual.  This is done before signalling the timer expiry, since
           the timer expiry might alter the timer data */
      if ( clnt_timer->info.callback_type == TIMER_FUNC2_CB_TYPE)
      {
        if ( timer->list != NULL )
        {
          /* Timer is still on the list.  It is a reloading timer. */

          /* Difference btw run_time & run_time_sclk is rounding error (+/-.5ms) */
          DalTimetick_CvtToTimetick64(hTimerHandle,run_time_ms,T_MSEC,&run_time_sclk);

          /* Nudge start time, so the next expiry includes this rounding error */
          if ( run_time >= run_time_sclk )
          {
            /* Rounded down: move start backwards, so next duration is longer */
            timer->start -= run_time - run_time_sclk;
          }
          else if ( timer->expiry - timer->start > run_time_sclk - run_time )
          {
            /* Rounded up: move start forward, so next duration is shorter */
            timer->start += run_time_sclk - run_time;
          }
          else
          {
            /* Rounded up, but next expiry is too close for rounding nudging. */
            timer->start = timer->expiry - 1;
          }
        }
      }
      
      /* Insert timer back in active list */
      timer_insert( current_list, timer );

      clnt_timer->timer_state      = TIMER_SET_FLAG; 
      clnt_timer->timer_state_at   = ticks_now; 
    }
    else
    {
      /* Free internal timer object */
      timer_free_internal_timer(timer);

      clnt_timer->timer_state      = TIMER_EXPIRED_FLAG; 
      clnt_timer->timer_state_at   = ticks_now; 
      clnt_timer->timer_int_handle = TIMER_BUFFER_INVALID_HANDLE;
    }
    
      /* After intfree() call, timer may get deleted or cleared. Therefore, backup func, sigs details before releasing lock */
      memscpy(&temp_clnt_timer, sizeof(timer_client_type), clnt_timer, sizeof(timer_client_type));
      

	/* Expire timer */
    if(temp_clnt_timer.info.remote_process == TIMER_FROM_REMOTE_PROCESS)
      {
 	    /* Release the MUTEX which was acquired in 
	    timer_process_active_timers. The idea is
	    to hold this mutex only till the expiry
	    signalling notification is completed. */
        ATS_INTFREE_SAV( isave );
		
        timer_remote_expire(&temp_clnt_timer, run_time_ms, &ticks_now);
      }
      else
      {
        #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
          timers_expired[timer_expired_cnt].expiry_value =  local_timer_expiry;
        #endif
        timer_expire( &temp_clnt_timer, run_time_ms, &ticks_now );
      }

    /* NOTE: The above call may run user code, which can do anything ...
       including installing and uninstall other timers.  This can cause
       the value of timers.active.list.zero to change at this point.
       We must use the new value in the next iteration, so any local
       variable which holds computations involving that value must
       be considered invalid after the above call.  */


    /* Lock interrupts in preparation of the next iteration of the loop,
       which tests & manipulates the active timer list */
    ATS_INTLOCK_SAV( isave );

    /* Get current slow clock count */
    DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);

  } /* while timers on timer.active.list are expiring */
  
  

  /* Timers that expire at and before "ticks_now" have been processed.
     Set interrupt for when next timer expires. */

  next_timer_expiry = timer_determine_next_timer_expiry(&head_timer);

    /* The first timer on the active list is the timer next to expire */
  timer_set_next_interrupt( next_timer_expiry,ticks_now, TRUE, 
                            TIMER_MVS_TIMER_PROCESSED, 
                            head_timer );



  /* Timer processing has completed */
  timers.processing = FALSE;

  /* Save a trace packet */
  TIMER_TRACE(PROCESS_END);
  
  
  /* We've finished manipulating the active timer list.  */
  ATS_INTFREE_SAV( isave );

  // Resetting and checking should happen in a mutex
  TIMER_PC_MASK_MUTEX_LOCK();
  if(timer_after_pc_mask[TIMER_PC_MASK_INDEX] == 1)
  {
    timer_reset_check_pc_mask(TIMER_PC_MASK_INDEX);
  }
  TIMER_PC_MASK_MUTEX_UNLOCK();

  #if defined(T_QUASAR) && !defined(T_QUASAR_X)
  /* Because of a hardware problem with QUASar implementation of this
     timer, do read on the timer count until it's not zero...
     The problem is that for some reason, the SLEEP_XTAL_TIMETICK_COUNT
     will read zero for a while (maybe a 10-20 us) if you read it right
     after the ..._VAL is written to.  This is an easy workaround, instead
     having to check for the actual condition of ..._VAL being written
     (_VAL is written in timer_set_next_interrupt above). */
  while ( MSM_R( SLEEP_XTAL_TIMETICK_COUNT ) == 0 )
  {
    /* Do nothing */
  }
  #endif /* T_QUASAR */

} /* timer_process_active_timers */



/*=============================================================================

FUNCTION TIMER_TASK

DESCRIPTION
  Initializes the Timer Task

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void timer_task
(
  /* Unused parameter - required to match rex_task_func_type prototype */
  dword                           ignored    /*lint -esym(715,ignored) */
)
{
  /* Signals returned from rex wait procedure */
  rex_sigs_type                   sigs;

  /* Stores possible result from registering ISR if we own the Interrupt */
  #ifndef FEATURE_TIMER_USE_QURT_SYSCLOCK
  int                             eRes;
  #endif /*FEATURE_TIMER_USE_QURT_SYSCLOCK*/

  /* Install the timer expiry ISR */
  /* was DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER  if we own the interrupt */
  #ifndef FEATURE_TIMER_USE_QURT_SYSCLOCK
  if (timer_fn_map.timer_register_isr)
  {
    eRes = timer_fn_map.timer_register_isr(timers.dal_int_handle,
                                           timer_fn_map.timer_interrupt_id,
                                           (DALISR)timer_task_isr,
                                           NULL,
                                           timer_fn_map.timer_interrupt_flag);
    if ( eRes != DAL_SUCCESS )
    {
      ERR_FATAL("Cannot register ISR",0,0,0);
    }
  }
  #endif /*FEATURE_TIMER_USE_QURT_SYSCLOCK*/

#ifndef FEATURE_TIME_QDSP6
  /* Signal the task in order to prime the timer processing mechanism */
  (void) rex_set_sigs( timer_tcb_ptr, TIMER_CHANGE_SIG );
#else
   /* Signal the task in order to prime the timer processing mechanism */
   (void) qurt_anysignal_set(&timer_q6_signal, TIMER_QURT_SIG_MASK);
#endif

  rcinit_handshake_startup();

  for(;;)
  {

#ifndef FEATURE_TIME_QDSP6
    /* Wait for some task signal */
    sigs = rex_wait( TIMER_PROCESS_SIGS );
#else
    sigs = qurt_anysignal_wait (&timer_q6_signal, TIMER_QURT_TASK_SIGNAL_MASK);
#endif

#ifndef FEATURE_TIME_QDSP6
    if ( sigs & TIMER_EXPIRY_SIG )
    {
      /* Clear the timer expiry signal */
      (void) rex_clr_sigs( timer_tcb_ptr, TIMER_EXPIRY_SIG );
	  
      /* Process expiring timer(s) */
      timer_process_active_timers();
    }

    if ( sigs & TIMER_ALLOCATE_CHUNK_SIG )
    {
      /* Clear the timer expiry signal */
      (void) rex_clr_sigs( timer_tcb_ptr, TIMER_ALLOCATE_CHUNK_SIG );
	  
      /* Allocate a new client timer chunk.
            * This is done to keep the chunk allocation inside timer task only.
            * Here we are allocating the chunk beforehand.
            */
      (void)timer_create_new_chunk(MAX_TIMER_BUFFER_NUMBER);
    }
#else
    if (sigs & TIMER_QURT_SIG_MASK) 
    {
      /*record the time at which the qurt signal came*/
      #ifdef FEATURE_TIMER_USE_QURT_SYSCLOCK
      timer_update_isr_time();
      #endif /*FEATURE_TIMER_USE_QURT_SYSCLOCK*/
       
      /* Clear the timer expiry signal */
      qurt_anysignal_clear (&timer_q6_signal, TIMER_QURT_SIG_MASK);

      /* Process expiring timer(s) */
      timer_process_active_timers();
    }

    if (sigs & TIMER_ALLOCATE_CHUNK_SIG) 
    {
      /* Clear this signal for creating new chunks */
      qurt_anysignal_clear (&timer_q6_signal, TIMER_ALLOCATE_CHUNK_SIG);
    
      /* Allocate a new client timer chunk.
            * This is done to keep the chunk allocation inside timer task only.
            * Here we are allocating the chunk beforehand.
            */
      (void)timer_create_new_chunk(MAX_TIMER_BUFFER_NUMBER);
    }	
#endif	
  }
} /* timer_task */


/* If FEATURE_TIMER_TASK is defined, then the time tick interrupt is
** serviced in the task context.  The timer task should be started
** before enabling the interrupt so that the task is running when an
** interrupt occurs.
** */

void timer_task_init(void) /* Time services */
{
  RCINIT_INFO handle = rcinit_lookup("timer");

  if (RCINIT_NULL != handle)
  {
    rcinit_initfn_spawn_task(handle, timer_task);
  }
}


/*=============================================================================

FUNCTION TIMER_INIT                                                    REGIONAL

DESCRIPTION
  Initializes the 32kHz slow clock based Timer Subsytem

DEPENDENCIES
  Must only be called from time_init()

RETURN VALUE
  None

SIDE EFFECTS
  May install an ISR for the 32-bit slow clock match register

=============================================================================*/

void timer_init( void )
{
  /* Current slow clock count */
  time_timetick_type              ticks_now = 0;

  /* Loop index for timer bin initialization */
  int                             i;

  /* Timer bin */
  timer_bin_ptr                   bin;

  /* RLC starting values */

  rlc_error_type rlc_error = RLC_FAILED;

  #ifndef FEATURE_TIMER_USE_QURT_SYSCLOCK
  /* Dal Result */
  DALResult                       eRes;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  eRes = DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER, &timers.dal_int_handle);
  if( eRes != DAL_SUCCESS || timers.dal_int_handle == NULL )
  {
    /* Error out, we should not start up timer task if we cannot set interrupts through DAL */
    ERR_FATAL("timer_init: Could not obtain a Dal Interrupt Handle",0,0,0);
  }
  #endif /*FEATURE_TIMER_USE_QURT_SYSCLOCK*/

  timer_pimutex_init();
  timer_trace_mutex_init();
  timer_pc_mutex_init();

  timer_fn_map_initialize_fp(&timer_fn_map);

  /*sequence dependency - slave cmd queues have to be initialized 
   before timer_init*/
  #ifdef FEATURE_TIMER_SLAVE1
  /*the timer slave1 command queue has to be initializer here, before 
   starting the timer task*/
  timer_slave_cmd_queue_init();
  #endif

  /*initialize the signal that will be used for signalling expiry of
   the smallest timer when match value == timetick value*/
  timer_get_timetick_handle();

  #ifdef FEATURE_TIME_QDSP6
  /* Use function map here */
  timer_init_qurt_signal();
  #endif /*FEATURE_TIME_QDSP6*/

  /* Get current slow clock count */
  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_INIT, NULL, ticks_now);

  /* Initialize the timer_bin_pool and free list */

  for(i = 0; i < TIMER_BIN_NUM; i++)
  {
    bin = &timer_bin_pool[i]; 

    /* init the bin's data */
    bin->first    = NULL;
    bin->last     = NULL;
    bin->lower    = 0;
    bin->upper    = 0;
    bin->count    = 0;

    /* push it on the free list */
    timer_bin_list_push( &timers.bins_free, bin);
  }

  /* Initialize the list of active timers to contain no timers */
  timers.active.list.first = NULL;
  timers.active.list.last  = NULL;
  timers.p0_active.list.first = NULL;
  timers.p0_active.list.last  = NULL;  

  /* Initialize the first non-deferrable timer to NULL */
  timers.first_non_deferrable = NULL;

  /* Initialize timers.set_value, set_time values & last_set_time for tests 
     in set_next_interrupt() and update_timer_interrupt */
  timers.set_value         = ticks_now - 2;
  timers.set_time          = ticks_now - 1;
  timers.last_set_time     = ticks_now - 1;

#ifndef FEATURE_TIME_QDSP6

  #ifdef TIMETICK_INT_EDGE_TRIGGERED
  
  /* Setup edge-sensitive interrupt, instead of having the interrupt
     asserted for 30us */
  TIMETICK_INT_EDGE_TRIGGERED_INIT();

  #endif /* TIMETICK_INT_EDGE_TRIGGERED */

#endif

  #ifdef FEATURE_TIMER_SLAVE1
  timer_slave_processing_enabled = TRUE;
  #endif

  /* Register rlc handle */
  rlc_error = rlc_register_handle(&rlc_handle, "Timer");
  if(rlc_error != RLC_SUCCESS)
  {
    ATS_ERR_1("There was an error registering rlc handle! Error: 0x%x\n", rlc_error);
    //Sleep(5000);
    return;
  }
  else
  {
    ATS_MSG("Added handle successfully!\n");
  }


  rlc_error = rlc_add_learning_attribute(rlc_handle,
                                                  RLC_LEARNING_CLUSTERING,
                                                  64,
                                                  1,
                                                  2,
                                                  &rlc_attribute_learn
                                                  );
  if(rlc_error != RLC_SUCCESS)
  {

    ATS_ERR_1("There was an error adding learning attribute! Error: 0x%x\n", rlc_error);
    return;
  }
  else
  {
  ATS_MSG("Learning attribute added successfully!\n");
  }

  rlc_error = rlc_add_correcting_attribute_sc(rlc_handle, RLC_CORR_SET_VAR_BC, timer_set_backoff, rlc_attribute_learn, &rlc_attribute_correct);
  if(rlc_error != RLC_SUCCESS)
  {
    ATS_ERR_1("There was an error adding correction attribute! Error: 0x%x\n", rlc_error);
    return;
  }
  else
  {
    ATS_ERR("Correction attribute added successfully!\n");
  }

} /* timer_init */


/*=============================================================================

FUNCTION TIMER_DRV_GROUP_DISABLE

DESCRIPTION
  Disables a timer group.  Timers in the timer group will continue to count
  down, but they will not expire until the group is enabled.

DEPENDENCIES
  Timer group must not be left disabled for more than 2^32 slow clock counts.

RETURN VALUE
   TE_INVALID_PARAMETERS  If passed timer_group pointer is NULL
   TE_INVALID_GROUP_HANDLE	If the group asked to be disabled is time_prio0_group
   TE_SUCCESS  If the operation completed successfully

SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/

timer_error_type timer_drv_group_disable
(
  /* Timer group to be disabled */
  timer_group_ptr                 group,

  /* For debug purposes only  */
  timer_group_ptr                 group_actual_addr
)
{
  timer_group_t_ptr               timer_group;
  uint32                          group_idx;
  
  /* Pointer to timers, for walking through lists of timers */
  timer_ptr_internal_type         ptr;

  time_timetick_type              now=0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  
  if(group == NULL || group->group_handle == GROUP_INVALID_HANDLE)
    return TE_INVALID_GROUP_HANDLE;

  #else
    
  if(group == NULL || *group == GROUP_INVALID_HANDLE)
    return TE_INVALID_GROUP_HANDLE;

  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &now);
  TIMER_API_LOG(TIMER_API_LOG_TIMER_GROUP_DISABLE,NULL,now);

  /*if timer_priority0_group, return as disabling of 
   timer_prio0_group is not allowed*/
  if(group == &timer_prio0_group) 
  {
    ATS_INTFREE();
    return TE_INVALID_GROUP_HANDLE;
  }

  /* Disable the timer group, if it is not already disabled */

  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined(TIMER_HANDLE_AS_STRUCTURE)
  if(!GROUP_HANDLE_TO_INDEX(group->group_handle, &group_idx))
  {
     ATS_INTFREE();
     return TE_INVALID_GROUP_HANDLE;
  }
  #else
  if(!GROUP_HANDLE_TO_INDEX(*group, &group_idx))
  {
     ATS_INTFREE();
     return TE_INVALID_GROUP_HANDLE;
  }
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  
  timer_group = timer_internal_groups[group_idx];

  if ( !timer_group->disabled )
  {
    /* Mark timer-group as disabled, and record zero-basis */
    timer_group->disabled  = TRUE;
	#ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
	group->disabled = TRUE;
	#endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

    /* Move given group's timers from active timer list to group timer list */
    ptr = timers.active.list.first;

    while (ptr != NULL)
    {
      /* Remember the next element on the active list, we are going
         to nuke it */
      timer_ptr_internal_type next = ptr->next;

      if ( ptr->info.group_idx == group_idx )
      {
        /* This is a timer in the given timer group */

        /* if it's binned, remove it from its bin */
        if( ptr->bin != NULL ) 
        {
            timer_bin_remove( ptr );
        }
       
        /* Unlink timer from active list */
        timer_unlink( ptr );
       
        /* Once the timer has been removed from the active
         timer list, we need to decrement the num of
         timers in the active list*/
        /* Decrement the number of timers  */
        timers.active_timers_count--; 
       
        /* Append to group timer list */
        timer_append( &timer_group->list, ptr );
      }

        /* Walk to next timer node */
      ptr = next;

    }

    /* Active timer list has changed - ensure next timer event is correct */
    timer_update_timer_interrupt(TIMER_MVS_GROUP_DISABLED);
  }

  /* Update the first non-deferrable timer in the active list */
  timer_update_first_active_non_deferrable_timer();

  ATS_INTFREE();
  return TE_SUCCESS;

} /* timer_drv_group_disable */



/*=============================================================================

FUNCTION TIMER_DRV_GROUP_ENABLE

DESCRIPTION
  Enables a timer group.  Timers in the timer group which expired during the
  diabled period will have their expiry processing executed.

DEPENDENCIES
  Timer group must not be left disabled for more than 2^32 slow clock counts.

RETURN VALUE
  TE_INVALID_PARAMETERS  If the passed timer_group pointer is invalid
  TE_SUCCESS  If the operation completed successfully

SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/

timer_error_type timer_drv_group_enable
(
  /* Timer group to be enabled */
  timer_group_ptr                 group,

  /* For debug purposes only	*/
  timer_group_ptr 				group_actual_addr
)
{
  uint32                          group_idx;
  timer_group_t_ptr               timer_group;
  
  /* Current slow clock count */
  time_timetick_type                   ticks_now = 0;

  /* Pointer to a timer in the list of timers */
  timer_ptr_internal_type         timer;


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
#if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  
  if(group == NULL || group->group_handle == GROUP_INVALID_HANDLE)
    return TE_INVALID_GROUP_HANDLE;

#else
    
  if(group == NULL || *group == GROUP_INVALID_HANDLE)
    return TE_INVALID_GROUP_HANDLE;

#endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */


  ATS_INTLOCK();

  /* Get current slow clock count */
  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GROUP_ENABLE,NULL,ticks_now);

  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  if(!GROUP_HANDLE_TO_INDEX(group->group_handle, &group_idx))
  {
     ATS_INTFREE();
     return TE_INVALID_GROUP_HANDLE;
  }
  #else
  if(!GROUP_HANDLE_TO_INDEX(*group, &group_idx))
  {
     ATS_INTFREE();
     return TE_INVALID_GROUP_HANDLE;
  }
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  
  timer_group = timer_internal_groups[group_idx];

  /* Enable the timer group, if it is disabled */

  if ( timer_group->disabled )
  {
    /* Walk through timers in timer group list.
       Stop when we get to a timer after the head of the active list */

    timer = timer_group->list.first;

    while( timer != NULL )
    { 
      if( timer->expiry >= ticks_now )
      {
        break; 
      }
      /* Increment the number of expired timers  */
      timers.active_timers_count++; 
      timer = timer->next;
    }

    /* Break timer list into two parts at this point */
    if( timer != NULL )
    {
      timer_group->list.last = timer->prev;

      if( timer->prev == NULL )
      {
        timer_group->list.first = NULL;
      }
      else
      {
        timer->prev->next = NULL;
      }

      timer->prev = NULL;
    }
        
    /* Expired timers are in list pointed to by "timer_group->list" */
    /* Non-expired timers are in linked list pointed to by "timer" */

    /* Updates timer_num in timer_insert for non-expired timers */
    /* Move non-expired timers into active timer list */
    timer_insert_list( &timers.active.list, timer );

    /* If there are any expired timers ... */
    if ( timer_group->list.first != NULL ) 
    {

      /* Move them to the front of the special expired list */
      timer_concatenate_lists( &timer_group->list, &timers.active.list ); 
 
    }

    /* Mark timer-group as enabled */
    timer_group->disabled = FALSE;

    /* Active timer list has changed - ensure next timer event is correct */
    timer_update_timer_interrupt(TIMER_MVS_GROUP_ENABLED);
  }

  ATS_INTFREE();
  return TE_SUCCESS;
} /* timer_drv_group_enable */

/*=============================================================================

FUNCTION TIMER_CREATE_NEW_GROUP

DESCRIPTION
  Creates new internal group and attaches handle to client group variable

DEPENDENCIES
  Should be called from an INTLOCK'd context

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
static timer_error_type timer_create_new_group(timer_group_ptr group, 
                                               timer_group_ptr group_actual_addr, /* Only for debugging log purposes */
                                               uint32 *pgroup_idx, 
                                               uint32 process_idx)
{
  uint32 group_idx;
  uint32 local_free_group_idx;

  if ( timer_internal_groups_allocated >= TIMER_MAX_GROUPS ||
  	   timer_next_free_group_idx >= TIMER_MAX_GROUPS )
   {
      ERR_FATAL("Maximum possible groups %d are over", TIMER_MAX_GROUPS, 0, 0);
      return TE_FAIL;
   }  

  group_idx = timer_next_free_group_idx;
  if(timer_internal_groups[group_idx] != NULL)
  {
     ERR_FATAL("Incorrect free group index : %d", group_idx, 0, 0);
     return TE_NO_FREE_GROUP;
  }
  
  /* Allocate memory and initialize to zero */
  timer_internal_groups[group_idx] = (timer_group_t *) qurt_calloc(sizeof(timer_group_t), 1);
  if(timer_internal_groups[group_idx] == NULL)
  {
     ATS_ERR_1("Couldnt alloc memory of %d bytes", sizeof(timer_group_t));
     return TE_MALLOC_FAILED;
  }

  timer_internal_groups_allocated++;
  timer_internal_groups[group_idx]->ext_self = group_actual_addr;
  timer_internal_groups[group_idx]->process_idx = process_idx;
  timer_internal_groups[group_idx]->group_idx = group_idx;
  
  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  group->group_handle= GROUP_INDEX_TO_HANDLE(group_idx);
  #else
  *group = GROUP_INDEX_TO_HANDLE(group_idx);
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  *pgroup_idx = group_idx;

  /* Update the next free group index */
  local_free_group_idx = (timer_next_free_group_idx + 1) % TIMER_MAX_GROUPS;
  while ( local_free_group_idx != timer_next_free_group_idx )
  	{
  	  if (timer_internal_groups[local_free_group_idx] == NULL )
  	  	{
  	  	 break;
  	  	}

	  local_free_group_idx = (local_free_group_idx + 1)%TIMER_MAX_GROUPS;
  	}

  timer_next_free_group_idx = local_free_group_idx;
  
  /*Placing newly created group onto the list*/
  timer_internal_groups[group_idx]->next = timers.active.next;
  timers.active.next = timer_internal_groups[group_idx];

  return TE_SUCCESS;
} /* timer_create_new_group */



/*=============================================================================

FUNCTION TIMER_DELETE_GROUP

DESCRIPTION
  Deletes an internal group and frees the memory associated with it.
  
DEPENDENCIES
  Should be called from an INTLOCK'd context

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
static void timer_delete_group(uint8 group_idx )
{
  /* Free the memory for the group */
  qurt_free(timer_internal_groups[group_idx]);

  /* Mark the group pointer as NULL */
  timer_internal_groups[group_idx] = NULL;

  /* Update the next free group index & total allocated groups*/
  timer_next_free_group_idx = group_idx;
  timer_internal_groups_allocated--;

} /* timer_delete_group */


/*=============================================================================

FUNCTION TIMER_DRV_GROUP_DELETE

DESCRIPTION
  Delete the timer group after undefining all timers belonging to this group

DEPENDENCIES
  None

RETURN VALUE
   TE_INVALID_PARAMETERS  If passed group pointer is NULL
   TE_INVALID_GROUP_HANDLE	If the operation is asked on a group for which it is not allowed.
   TE_ALL_TIMER_NOT_UNDEEF  If all timer belonging to this group are not undefined.
   TE_SUCCESS : If the operation completed successfully

SIDE EFFECTS
  None.

=============================================================================*/

timer_error_type timer_drv_group_delete( timer_group_ptr group,
                                         timer_group_ptr group_actual_addr,
                                         uint32 process_idx )
{
  /* Set the property of this timer group to indicate whether or not it's 
  deferrable, based on the argument passed in*/
  timer_group_t_ptr group_ptr;
  uint32 group_idx;
  timer_group_t_ptr prev_ptr=NULL,curr_ptr=NULL;
  boolean found=FALSE;
  
  if ( group == NULL )
  	{
  	  ERR_FATAL("timer_drv_group_delete: Passed group poiinter is NULL",0,0,0);
  	}  

  ATS_INTLOCK();
  
  if(
  	#if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  	 !GROUP_HANDLE_TO_INDEX(group->group_handle, &group_idx)
  	#else
	 !GROUP_HANDLE_TO_INDEX(*group, &group_idx)
	#endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  	)
   {   
		 ATS_INTFREE();
		 return TE_INVALID_GROUP_HANDLE;
   }

  group_ptr = timer_internal_groups[group_idx];
  
  /*Checking whether all timers belonging to a group
    has been undefined or not*/
  if(group_ptr->timer_cnt!= 0)
	{
         ATS_INTFREE();
         return TE_ALL_TIMER_NOT_UNDEEF;
	}
  
  /*Checking for the group which we want to delete from the 
     group list*/
  curr_ptr = timers.active.next;
  prev_ptr = &timers.active;

  /*Looking for the group in list*/
  while(curr_ptr->next != &timers.active)
  {
	 if(curr_ptr == group_ptr) 
	 {
        found = TRUE;
        break;
	 }
	 
	 prev_ptr = curr_ptr;
     curr_ptr = prev_ptr->next;	 
  }
  
  /*Checking whether group is at the lst of the list*/
  if(curr_ptr->next == &timers.active)
	{
      if(curr_ptr == group_ptr) 
          found = TRUE;
    }
		 
  if(found == TRUE)
  {	  
    prev_ptr->next = curr_ptr->next ;
    curr_ptr->next = NULL;
  }
  else
	ERR_FATAL("timer_drv_group_delete : group not found in list",0,0,0);

  /*deleting the group from the list*/ 
  timer_delete_group(group_idx );
  *group = 0;
  
  ATS_INTFREE();
  return TE_SUCCESS; 

} /* timer_drv_group_delete */


/*=============================================================================

FUNCTION TIMER_DRV_GROUP_SET_DEFERRABLE

DESCRIPTION
  Set timer group deferrable status.Timer groups default to being non-deferrable.

DEPENDENCIES
  None

RETURN VALUE
   TE_INVALID_PARAMETERS  If passed group pointer is NULL
   TE_INVALID_GROUP_HANDLE	If the operation is asked on a group for which it is not allowed, i.e.
												- cannot set timer_null_group to non-deferrable
												- cannot set timer_non_defer_group to deferrable
   TE_SUCCESS : If the operation completed successfully

SIDE EFFECTS
  None.

=============================================================================*/

timer_error_type timer_drv_group_set_deferrable( timer_group_ptr group,
                                                      timer_group_ptr group_actual_addr, /* For debug purposes only	*/
                                                      boolean condition,
                                                      uint32 process_idx )
{
  /* Set the property of this timer group to indicate whether or not it's 
  deferrable, based on the argument passed in*/
  timer_group_t_ptr group_ptr;
  uint32 group_idx;
  time_timetick_type now = 0;
  timer_error_type status;

  if ( group == NULL )
  	{
  	  ERR_FATAL("timer_drv_group_set_deferrable: Passed group poiinter is NULL",0,0,0);
  	}  

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GROUP_SET_DEFERRABLE,NULL,now);
  
  if(
  	#if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  	 !GROUP_HANDLE_TO_INDEX(group->group_handle, &group_idx)
  	#else
	 !GROUP_HANDLE_TO_INDEX(*group, &group_idx)
	#endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  	)
  {   /* Group is not present, Create it first */
	  if((status = timer_create_new_group(group, group_actual_addr, &group_idx, process_idx)) != TE_SUCCESS)
	  {
		 ATS_INTFREE();
		 return status;
	  }
  }
  
  group_ptr = timer_internal_groups[group_idx];
  group_ptr->deferrable = condition;

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  group->deferrable = condition;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  /* Update the first non-deferrable timer in the active list */
  timer_update_first_active_non_deferrable_timer();  

  ATS_INTFREE();
  return TE_SUCCESS; 

} /* timer_drv_group_set_deferrable */


/*=============================================================================

FUNCTION TIMER_DRV_GET_FIRST_NON_DEFERRABLE_TIMER_EXPIRY

DESCRIPTION
  Gets the first non-deferrable timer's expiry value.

DEPENDENCIES
  None.

RETURN VALUE
  Absolute expiry of first occuring non-deferrable timer.
  Returns TIMER_MAX_EXPIRY in case there is no non-deferrable timer present.

SIDE EFFECTS
  None

=============================================================================*/

time_timetick_type timer_drv_get_first_non_deferrable_timer_expiry( 
                                      timer_non_defer_data_ptr timer_data_ptr )
{
  /* Expiry for the 1st non-deferrable timer */
  time_timetick_type              future_expiry = TIMER_MAX_EXPIRY;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ATS_INTLOCK();

  //TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_FIRST_NON_DEFERRABLE_TIMER_EXPIRY,NULL,ticks_now);

  /* Return the expiry of the first non-deferrable timer */
  if (timers.first_non_deferrable != NULL)
  {
    future_expiry = timers.first_non_deferrable->expiry;
  }
 
  if(timer_data_ptr != NULL)
  {
    if(future_expiry == TIMER_MAX_EXPIRY)
     {
       timer_data_ptr->cb_type = TIMER_INVALID_NOTIFY_TYPE;
       timer_data_ptr->sigs_func_addr = NULL;
       timer_data_ptr->sigs_mask_data = 0;
     }
    else
     {
       timer_data_ptr->cb_type = timers.first_non_deferrable->timer_ext->info.callback_type;
       timer_data_ptr->sigs_func_addr = timers.first_non_deferrable->timer_ext->cbfn.func1;
       timer_data_ptr->sigs_mask_data = timers.first_non_deferrable->timer_ext->data; 
     }
  }

  ATS_INTFREE();

  return future_expiry;

} /* timer_drv_get_first_non_deferrable_timer_expiry */

#if 0 /* This Part is moved to timer_client.c */

/*=============================================================================

FUNCTION TIMER_GET_TIME_TILL_FIRST_NON_DEFERRABLE_TIMER_64

DESCRIPTION
  Gets time based on 19MHz clock to the next occurring non-deferrable  
  timer expiry value. 

DEPENDENCIES
  Must be called from an ATS_INTLOCK'd context.

RETURN VALUE
  Number of 19MHz ticks until the next timer expiry in a timer group which is 
  non-deferrable.

SIDE EFFECTS
  None

=============================================================================*/

time_timetick_type timer_drv_get_time_till_first_non_deferrable_timer_64( void )
{
  /* Current slow clock count */
  time_timetick_type              ticks_now = 0;
  time_timetick_type              future_expiry;
  time_timetick_type              tt_ret;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ATS_INTLOCK();

  /* Get the current time */
  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_TIME_TILL_FIRST_NON_DEFERRABLE_TIMER,NULL,ticks_now);

  /* Assume the worst - no timers on list matching our criteria set to expire */
  future_expiry = TIMER_MAX_EXPIRY;

  /* Return the expiry of the first non-deferrable timer */
  if (timers.first_non_deferrable != NULL)
  {
    future_expiry = timers.first_non_deferrable->expiry;
  }

  /* If future_expiry is in past */
  if (future_expiry  < ticks_now )
  {  
    tt_ret = 0;
  }
  else
  {  
    /* Return the difference between now and the above future expiry */
    tt_ret = (future_expiry - ticks_now);
  }

  ATS_INTFREE();

  return tt_ret;

} /* timer_get_time_till_first_non_deferrable_timer_64 */
#endif /* #if 0 */


/*=============================================================================

FUNCTION TIMER_DRV_DEFER_MATCH_INTERRUPT_64

DESCRIPTION
  Sets timer match interrupt to the next occurring non-deferrable timer expiry 
  value. Complimentary function is timer_undefer_match_interrupt(). 

DEPENDENCIES
  Must be called from an ATS_INTLOCK'd context.

RETURN VALUE
  Number of ticks in 19MHz domain until the next timer expiry in a timer  
  group which is non-deferrable.

SIDE EFFECTS
  Adjusts match value and prevents any subsequent timer calls from updating the
  match value, until timer_undefer_match_interrupt() is called.

=============================================================================*/

time_timetick_type timer_drv_defer_match_interrupt_64( void )
{
  /* Current slow clock count */
  time_timetick_type                   ticks_now = 0;
  time_timetick_type                   future_expiry;
  timer_ptr_internal_type              head_timer = NULL;
  ATS_DECLARE_ISAV(isave);


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  TIMER_PC_MASK_MUTEX_LOCK();
  timer_after_pc_mask[TIMER_PC_DEFER_UNDEFER_CNT_INDEX]++; 
  TIMER_PC_MASK_MUTEX_UNLOCK();


  ATS_INTLOCK_SAV(isave);

  /* Get the current time */
  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);
  
  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].ts = ticks_now;
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].undefer_entry_ts = 0;
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].reference_count= timer_defer_undefer_reference_count+1;
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].function_called= TIMER_MVS_DEFER_MATCH_INT;
  if (timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].ts > TIMER_MAX_EXPIRY)
  {
    ATS_ERR("Timer detects bad timetick value!");
  }
  timer_defer_undefer_reference_count_log_cnt = (timer_defer_undefer_reference_count_log_cnt+1)&(MAX_TIMER_REFERNCE_COUNT_LOG-1); 
  #endif  

  TIMER_API_LOG(TIMER_API_LOG_TIMER_DEFER_MATCH_INTERRUPT,NULL,ticks_now);

  /* Assume the worst - furthest point in future ~119 years */
  future_expiry = TIMER_MAX_EXPIRY;

  /* Return the expiry of the first non-deferrable timer */
  if (timers.first_non_deferrable != NULL)
  {
    future_expiry = timers.first_non_deferrable->expiry;
    head_timer = timers.first_non_deferrable;
  }

  /* If future_expiry is in past */
  if (future_expiry < ticks_now )
  {
    timer_defer_undefer_reference_count++;
    ATS_INTFREE_SAV(isave);
    return 0;
  }
  
  /* Set the timer interrupt to occur at the proper point in the future*/
  timer_set_next_interrupt( TIMER_MAX_EXPIRY, ticks_now, FALSE,TIMER_MVS_DEFER_MATCH_INT, head_timer);

  /*increase the reference count here AFTER calling the timer_set_next_interrupt()
   so that the first time around we program the timer interrupt. The decision to bypass 
   programming of the timer interrupt, the next time, is made inside 
   timer_set_next_interrupt() */
  timer_defer_undefer_reference_count++;

  /* From this point on timer interrupt  won’t be adjusted */
  timers.do_reprogram_isr = FALSE;

  ATS_INTFREE_SAV(isave);

  /* Return the difference between now and the above future expiry */
  return (future_expiry - ticks_now);

} /* timer_drv_defer_match_interrupt_64 */


/*=============================================================================

FUNCTION TIMER_DRV_UNDEFER_MATCH_INTERRUPT

DESCRIPTION
  Set the timer ISR to fire at the earliest expiring timer in the active
  list.  Meant to serve as a the complimentary function to
  timer_defer_match_interrupt()

DEPENDENCIES
  Must be called from an ATS_INTLOCK'd context.

RETURN VALUE
  None

SIDE EFFECTS
  Adjusts the match value.

=============================================================================*/
void timer_drv_undefer_match_interrupt( void )
{
  /* Current slow clock count */
  time_timetick_type                   now = 0;

  /* temp Timetick */
  time_timetick_type                   tmp_ticks = 0;

  /* Time of first expiring timer */
  time_timetick_type              first = TIMER_MAX_EXPIRY;

  timer_ptr_internal_type              head_timer=NULL;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Log timetick here also, before any operation is performed */
  DalTimetick_GetTimetick64(hTimerHandle, &tmp_ticks);

  TIMER_PC_MASK_MUTEX_LOCK();
  timer_after_pc_mask[TIMER_PC_MASK_INDEX]=1;

  /*timer_defer_undefer_reference_count is never suppose to go below 0 */
  if(timer_after_pc_mask[TIMER_PC_DEFER_UNDEFER_CNT_INDEX] == 0 ) 
  {
    ERR_FATAL("timer_undefer called more than timer defer = 0x%x",
              timer_after_pc_mask[TIMER_PC_DEFER_UNDEFER_CNT_INDEX],0,0);
  }

  if(timer_after_pc_mask[TIMER_PC_DEFER_UNDEFER_CNT_INDEX] > 0 ) 
  {
    if(--timer_after_pc_mask[TIMER_PC_DEFER_UNDEFER_CNT_INDEX] == 0) 
    {
      // SET THE CPU TO MAX USING NPA CLIENT
      if( timer_fn_map.timer_npa_clk_client )
      { /* client/resource exists so use can make MAX requests to the clock */
        timer_fn_map.timer_npa_issue_suppressible_request( timer_fn_map.timer_npa_clk_client,
                                                       timer_fn_map.timer_npa_max_state );
        timer_cnt_cpu_max++;
       }else
       {
         ATS_ERR("NPA Client is null for Timer to bring up the cpu");
        }
      }
  }

  
  TIMER_PC_MASK_MUTEX_UNLOCK();

  ATS_INTLOCK();

  timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_now = MAGIC_INITIALIZER_64;
  timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_first = MAGIC_INITIALIZER_64;

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_UNDEFER_MATCH_INTERRUPT,NULL,now);

  timer_defer_undefer_reference_count--;

  /*timer_defer_undefer_reference_count is never suppose to go below 0 */
  if(timer_defer_undefer_reference_count < 0 ) 
  {
    ERR_FATAL("timer_defer and undefer calls do not match, ref_cnt=0x%x",
              timer_defer_undefer_reference_count,0,0);
  }

  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].ts = now;
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].undefer_entry_ts = tmp_ticks;
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].reference_count= timer_defer_undefer_reference_count;
  timer_defer_undefer_reference_count_log[timer_defer_undefer_reference_count_log_cnt].function_called= TIMER_MVS_UNDEFER_MATCH_INT;
  timer_defer_undefer_reference_count_log_cnt = (timer_defer_undefer_reference_count_log_cnt+1)&(MAX_TIMER_REFERNCE_COUNT_LOG-1);
  #endif

  /*we can return safely here since we will not be programming 
   the timer interrupt anyways.*/
  if(timer_defer_undefer_reference_count > 0)
  {
    ATS_INTFREE();
    return;
  }

  timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_now = MAGIC_INITIALIZER_64;
  timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_first = MAGIC_INITIALIZER_64;
  timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_now = now;
 
  first = timer_determine_next_timer_expiry(&head_timer);

  /* If the first expiring timer is in the past ... */
  if ( first < now  )
  {
    /* Set the timer for "as soon as possible" (eg, "now") */
    first = now;
  }
   
  timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_first = first;
  
  //This is a dummy statement to remove compiler warning.
  timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_first = \
           timer_undefer_match_interrupt_log.timer_undefer_match_interrupt_first;
   
  /* Set the next interrupt match value. The function is called after power 
     collapse and the hardware might not contain right values . Hence force 
     MATCH_VAL register to be updated. */
  timer_set_next_interrupt( first, now, TRUE,TIMER_MVS_UNDEFER_MATCH_INT, head_timer );

  /* Allow timer isr programming again */
  timers.do_reprogram_isr = TRUE;

  ATS_INTFREE();

  } /* timer_drv_undefer_match_interrupt */


/*=============================================================================

FUNCTION TIMER_DRV_RESTART

DESCRIPTION
 This function is called after waking up. The timetick counter values 
 are readjusted to sync up with the base timetick counter when waking up from
 power collapse. The match value has to be adjusted accordingly.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  Adjusts the match value


=============================================================================*/

void timer_drv_restart(void)
{
  time_timetick_type                now = 0;

  DalTimetick_GetTimetick64(hTimerHandle, &now);
  TIMER_API_LOG(TIMER_API_LOG_TIMER_RESTART,NULL,now);
  /* Calling timer_undefer_match_interrupt */
  timer_undefer_match_interrupt();
} /* timer_drv_restart */


/*=============================================================================

FUNCTION TIMER_INIT_BUFFER_QUEUE

DESCRIPTION
  Iniitialize buffer queue where free timer buffers are kept. 

DEPENDENCIES
  Must be called from timer_def or timer_def2 before the first timer is 
  defined. It is not called from timer_init as some timers are defined 
  before call to timer_init.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_init_buffer_queue(void)
{
  /* local Iterator */
  uint32 i;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*Get memory from heap*/
  timer_buffer_ptr = (timer_ptr_internal_type)
                     qurt_malloc( sizeof(timer_internal_type) * MAX_TIMER_BUFFER_NUMBER );

  if(NULL == timer_buffer_ptr) 
  {
    ERR_FATAL("qurt_malloc returned NULL",0,0,0);
    return;
  }
  
  /*memset the allocated memory so that we do not have to memset it at timer_drv_set*/
  memset( timer_buffer_ptr, 0, sizeof(timer_internal_type) * MAX_TIMER_BUFFER_NUMBER );
  
  /* Initialize the timer_free_q */
  q_init( &timer_free_q );
  
  for ( i = 0 ; i < MAX_TIMER_BUFFER_NUMBER ; i++ )
  {
    timer_buffer_ptr[i].info.node_state = NODE_IS_FREE;
    timer_buffer_ptr[i].index           = i;
  
    /* Link the handles to the timer_free_q */
    q_link( &timer_buffer_ptr[i], &timer_buffer_ptr[i].link );
    
    /* Enqueue handles */
    q_put( &timer_free_q, &timer_buffer_ptr[i].link );

  }
}


/*=============================================================================
FUNCTION TIMER_CREATE_NEW_CHUNK

DESCRIPTION
  Creates new memory chunk for more timers

DEPENDENCIES
  Should be called from Intlock'd context

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
timer_error_type timer_create_new_chunk(uint32 max_timers)
{
   uint32 i;
   uint32 num_timers = max_timers;
   timer_client_ptr_type ptr = NULL;
   
   ATS_INTLOCK();
   
	/*
	* This is to check if some other task already allocated a new chunk,
	* while timer task didn't got a chance to handle the signal & call this
	* function from timer tasks context.
	*/
	if(timer_free_client_timers_cnt > TIMERS_MIN_CLIENT_TIMERS_FOR_NEW_CHUNK)
		{
		/* There is no need to create a new chunk. */
		ATS_INTFREE();
		return TE_SUCCESS;
		}
   
   if(timer_client_chunks_allocated == TIMER_CLIENT_CHUNKS_MAX)
   {
      /* Note that when this err_fatal is hit there will be some 15 client timers 
	  still free which shouldnt be serious case. The point that we reached at this
	  err_fatal indicates there is some leak issue with currently allocated timers */

      ERR_FATAL("Maximum possible chunks %d are over", TIMER_CLIENT_CHUNKS_MAX, 0, 0);
	  ATS_INTFREE();
      return TE_FAIL;
   }
   
   do{
      /* Get memory from heap */
      ptr = (timer_client_ptr_type)
                        qurt_malloc( sizeof(timer_client_type) * num_timers );

      if(ptr == NULL)
      {
         num_timers = num_timers/2;
      }
      else
      {
         break;
      }
   }while(num_timers != 0);
   
   if(ptr == NULL)
   {
      ATS_MSG_2("Couldnt malloc %d bytes for %d client timers", sizeof(timer_client_type) * num_timers, num_timers);
      //ERR_FATAL("Couldnt malloc %d bytes for client timers", sizeof(timer_client_type) * num_timers, 0, 0);
      ATS_INTFREE();
      return TE_FAIL;
   }

   /* Memset the timer client chunks memory */
   memset( ptr, 0x0, sizeof(timer_client_type) * num_timers );
   
   timer_client_chunks[timer_client_chunks_allocated].buffer     = ptr;
   timer_client_chunks[timer_client_chunks_allocated].buffer_idx = 0;
   timer_client_chunks[timer_client_chunks_allocated].num_timers = num_timers;
   timer_client_chunks[timer_client_chunks_allocated].allocated_cnt = 0;

   /* Update the total count of free client timers */
   timer_free_client_timers_cnt += num_timers;
     
   ptr = &timer_client_chunks[timer_client_chunks_allocated].buffer[0];
   for(i = 0; i < num_timers; ptr++, i++)
   {
      ptr->timer_state      = TIMER_DEFAULT_FLAG;
      /* For now checking only timer_state for knowing if a node free or not
         and to paint chunk memory with speed */
      ptr->info.node_state  = NODE_IS_FREE;   
   }
   
   /* Increment allocated chunks count */
   timer_client_chunks_allocated++;
   ATS_INTFREE();
   return TE_SUCCESS;
} /* timer_create_new_chunk */


/*=============================================================================
FUNCTION TIMER_INIT_CLIENT_TIMERS

DESCRIPTION
  Initialize client timer buffer memory and set them to default flag. 

DEPENDENCIES
  Must be called from timer_define before the first timer is defined.
  It is not called from timer_init as some timers are defined before
  call to timer_init.

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void timer_init_client_timers(void)
{
   uint32 i = 0, j = 0;
   timer_client_ptr_type ptr = NULL;


   /* Split the statically allocated client timer buffer into equally divided
    * chunks each of size MAX_TIMER_BUFFER_NUMBER.
    */
   for ( i = 0 ; 
         i < TIMER_CLIENT_BUFFER_INIT_ARRAY_SIZE && timer_client_chunks_allocated < TIMER_CLIENT_CHUNKS_MAX ; 
		 i++ )
   {
   /* Update the timer client chunk pointer to point to the static buffer of client timers */
     timer_client_chunks[timer_client_chunks_allocated].buffer = &timer_client_buffer[i][0];

   /* Buffer index pointing to the 1st free client timer from this chunk */
   timer_client_chunks[timer_client_chunks_allocated].buffer_idx = 0;

     /* Total remaining client timers allocated statically in this chunk */
     timer_client_chunks[timer_client_chunks_allocated].num_timers = MAX_TIMER_BUFFER_NUMBER;

   /* There are no allocated timers in this chunk, mark it so */
   timer_client_chunks[timer_client_chunks_allocated].allocated_cnt = 0;

   /* Update the count of free client timers */
     timer_free_client_timers_cnt += timer_client_chunks[timer_client_chunks_allocated].num_timers;
     
   /* Initialize the static client timer buffers */
   ptr = &timer_client_chunks[timer_client_chunks_allocated].buffer[0];

     for(j = 0; j < MAX_TIMER_BUFFER_NUMBER; ptr++, j++)
   {
      ptr->timer_state      = TIMER_DEFAULT_FLAG;
      /* For now checking only timer_state for knowing if a node free or not
         and to paint chunk memory with speed */
      ptr->info.node_state  = NODE_IS_FREE;   
   }
   
   /* Assign to current chunk and increment allocated chunks index */
	 timer_client_chunks_allocated++;

   }
   
   /* Assign the current chunk to be used as the 0th chunk */
   timer_client_curr_chunk_idx = 0;

   /* Assign the values to the global variables for timer client chunks */
   timer_client_chunk_total_num_chunks = TIMER_CLIENT_CHUNKS_MAX;

   return;
} /* timer_init_client_timers */


/*=============================================================================
FUNCTION TIMER_GET_NEW_CLIENT_TIMER

DESCRIPTION
  Get New client timer from timer_client_chunks. 

DEPENDENCIES
  Need to be called from internal function

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
timer_client_ptr_type timer_get_new_client_timer(timer_error_type *status, uint32 *handle)
{
   timer_client_ptr_type clnt_timer = NULL;
   uint32 i;
   uint32 chunk_idx = 0, timer_idx = 0;
   uint32 num_timers, found_timer = 0;
   
   /* Before procedding, check if there are any free client timers available or not */
   if (timer_client_chunks_allocated == TIMER_CLIENT_CHUNKS_MAX && timer_free_client_timers_cnt == 0)
   	{
   	  ERR_FATAL("Maximum possible chunks %d are over", TIMER_CLIENT_CHUNKS_MAX, 0, 0);
      return NULL;
   	}
   
   /* Input validation */
   if(status == NULL || handle == NULL)
   {
      ATS_ERR("Got Null inputs");
      return NULL;
   }
   
   /* Check if we need to create a new chunk before procedding */
   if (timer_free_client_timers_cnt == 0)
   	{
   	  /* Create a new chunk since we dont have any free client timers */
      *status = timer_create_new_chunk(MAX_TIMER_BUFFER_NUMBER);
      if(*status != TE_SUCCESS)
      {
         ATS_ERR("Couldnt create new memory chunk");
         return NULL;
      }
   	}
   
   *status = TE_FAIL;
   *handle = 0;
 
   chunk_idx = timer_client_curr_chunk_idx;
   found_timer = 0;
   
   do{
   	  if(timer_client_chunks[chunk_idx].allocated_cnt == timer_client_chunks[chunk_idx].num_timers)
      {
         /* Number of allocated timers in this chunk is equal to the total timers
                 * i.e. there are no free timers in this current chunk, no Need to traverse it.
                 */      
         chunk_idx = (chunk_idx + 1) % timer_client_chunks_allocated;
		 continue;
      }
	  else
      {
         /* Traverse the chunk starting from buffer_idx to find a free client timer */

         /* Initialize few variables before starting to traverse the chunk */
         timer_idx = timer_client_chunks[chunk_idx].buffer_idx;
		 num_timers = timer_client_chunks[chunk_idx].num_timers;
		 i = timer_idx;

         do
         	{
         	if(timer_client_chunks[chunk_idx].buffer[i].timer_state == TIMER_DEFAULT_FLAG)
         {
         		/* Found a free timer from the chunk, update variables & break out of this inner loop */
            timer_client_curr_chunk_idx = chunk_idx;
				timer_client_chunks[chunk_idx].buffer_idx = timer_idx = i;
            found_timer = 1;
            break;
         }

			/* Increment the index */          
			i++;

			/* Correction for overflow */
         if(i == num_timers)
         {
		 	i = 0;
		 	}
         
         	}while(i != timer_idx);
            
		 /* If timer was found, break out of this loop as well */
		 if (found_timer == 1)
         {
			  	break;
         }
         else
         {
         /*
                 * Getting here would mean that we didn't find any free 
                 * client timers in the current chunk, though we started
                 * the search only after checking for allocated_cnt, and
                 * we are supposed to successfully find a free client timer.
                 */
            ERR_FATAL("Corruption in the chunk, allocated_cnt = %d, num_timers=%d, timer_index=%d",
                       timer_client_chunks[chunk_idx].allocated_cnt,num_timers,timer_idx);
         }
      }
      chunk_idx = (chunk_idx + 1) % timer_client_chunks_allocated;
   }while(chunk_idx != timer_client_curr_chunk_idx);
   
   /* Get Client Timer */
   clnt_timer = &timer_client_chunks[chunk_idx].buffer[timer_idx];

   /* Increment the buffer_idx to point to next client timer from the current chunk */
   timer_client_chunks[chunk_idx].buffer_idx = (timer_client_chunks[chunk_idx].buffer_idx + 1) % timer_client_chunks[chunk_idx].num_timers;
   timer_client_chunks[chunk_idx].allocated_cnt++;

   /* Decrement the count of free client timers */
   timer_free_client_timers_cnt--;
   
   /* memset to zero before giving for new client */
   memset(clnt_timer, 0, sizeof(timer_client_type));
   clnt_timer->info.node_state = NODE_IS_NOT_FREE;
   clnt_timer->timer_state = TIMER_DEFAULT_FLAG;

   *handle = TIMER_INDEX_TO_HANDLE(chunk_idx, timer_idx);
   *status = TE_SUCCESS;

   /* Check to see if the number of free client timers in the 
       * current chunk is less than threshold, if so then allocate 
       * another chunk in advance.
       * Do this only if all the timer chunks are not used up, to
       * avoid hitting ERR_FATAL in cases when there are
       * TIMERS_MIN_CLIENT_TIMERS_FOR_NEW_CHUNK or less
       * free timers still available.
       */
   if ((timer_client_chunks_allocated < TIMER_CLIENT_CHUNKS_MAX) &&
   	   (timer_free_client_timers_cnt <= TIMERS_MIN_CLIENT_TIMERS_FOR_NEW_CHUNK ))
   {
   #ifndef FEATURE_TIME_QDSP6
     /* Signal in order to prime timer task to allocate a new chunk */
     (void) rex_set_sigs( timer_tcb_ptr, TIMER_ALLOCATE_CHUNK_SIG );
   #else
     /* Signal in order to prime timer task to allocate a new chunk */
     (void) qurt_anysignal_set(&timer_q6_signal, TIMER_ALLOCATE_CHUNK_SIG);
   #endif
   }
   
   return clnt_timer;
} /* timer_get_new_client_timer */


/*=============================================================================

FUNCTION TIMER_GET_CLIENT_TIMER

DESCRIPTION
  This function returns pointer to client timer.

DEPENDENCIES
  Should be called from Intlock'd context.. 

RETURN VALUE
  Returns relevant pointer to client timer structure. 
  If handle is invalid returns NULL.

SIDE EFFECTS
  This function should be called inside an intlock or a mutex.

=============================================================================*/
timer_client_ptr_type timer_get_client_timer
(
   timer_ptr_type    timer,
   timer_ptr_type    timer_actual_addr,
   uint32            process_idx,
   timer_error_type  *status
)
{
  //int32 temp_index;
  uint32 cid, tid;
  timer_client_ptr_type temp_clnt = NULL;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if(timer == NULL || status == NULL)
  {
     if(status)
       *status = TE_INVALID_PARAMETERS;  
     return NULL;
  }  
  
  *status = TE_INVALID_TIMER_HANDLE;
  
  if(
  	#if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  	 !TIMER_HANDLE_TO_INDEX(timer->timer_handle, &cid, &tid)
  	#else
  	 !TIMER_HANDLE_TO_INDEX(*timer, &cid, &tid)	
	#endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  	)
  {
     return NULL;
  }

  if(cid >= timer_client_chunks_allocated)
     return NULL;
     
  if(tid >= timer_client_chunks[cid].num_timers)
     return NULL;  

  temp_clnt = &timer_client_chunks[cid].buffer[tid];
  
  if(temp_clnt->timer_state < TIMER_DEFAULT_FLAG || temp_clnt->timer_state > TIMER_UNDEF_FLAG)
  {
     ERR_FATAL("Timer got corrupted. timer_state 0x%x, cid: 0x%x, tid: 0x%x ", temp_clnt->timer_state, cid, tid);
  }
  
  /* Only if the client timer is in use, otherwise it may be stale Handle */
  if(temp_clnt->info.node_state == NODE_IS_NOT_FREE 
     && temp_clnt->timer_state != TIMER_DEFAULT_FLAG
     && timer_actual_addr == temp_clnt->timer_client_ptr
     && temp_clnt->info.process_idx == process_idx)
  {
     *status = TE_SUCCESS;
  }
  else
  {
     temp_clnt = NULL;
  }
  
  return temp_clnt;
} /* timer_get_client_timer */


/*=============================================================================
FUNCTION TIMER_FREE_CLIENT_TIMER

DESCRIPTION
  Free up the client timer. 

DEPENDENCIES
  Should be called from Intlock'd Context
  
RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
timer_error_type timer_free_client_timer(timer_ptr_type timer)
{
   uint32 cid = 0;
   uint32 tid = 0;
   timer_client_ptr_type clnt_timer = NULL;
   timers_client_buffer_type *chunk_ptr = NULL;
   
   if(
  	   #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  	    !TIMER_HANDLE_TO_INDEX(timer->timer_handle, &cid, &tid)
  	   #else
  	    !TIMER_HANDLE_TO_INDEX(*timer, &cid, &tid)	
	   #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
	 )
   {
      return TE_INVALID_TIMER_HANDLE;
   }
   
   chunk_ptr = &timer_client_chunks[cid];
   clnt_timer = &chunk_ptr->buffer[tid];
  
   /* Intentionally not memsetting to zero as other details will be useful for debugging. */
   clnt_timer->timer_state            = TIMER_DEFAULT_FLAG;
   clnt_timer->info.node_state        = NODE_IS_FREE;
   
   /* If there are no previous free timers, keep buffer_idx as this tid */
   if(chunk_ptr->num_timers == chunk_ptr->allocated_cnt)
      chunk_ptr->buffer_idx = tid;
      
   /* Decrement the allocated clinet timer count for this chunk */
   chunk_ptr->allocated_cnt--;

   /* Increment the count of free client timers */
   timer_free_client_timers_cnt++;

   return TE_SUCCESS;
} /* timer_free_client_timer */


/*=============================================================================

FUNCTION TIMER_GET_CLNT_INT_TIMERS

DESCRIPTION
  Gets Client, Internal timers from Timer Handle. 
  If internal timer is not required, NULL can be passed as input

DEPENDENCIES
  Should be called from Intlock'd context.

RETURN VALUE
  None

SIDE EFFECTS


=============================================================================*/
timer_error_type timer_get_clnt_int_timers(timer_ptr_type timer,
                                           timer_ptr_type timer_actual_addr,
                                           uint32 process_idx,
                                           timer_client_ptr_type *clnt_timer, 
                                           timer_ptr_internal_type *int_timer)
{
  timer_error_type status = TE_SUCCESS;

  if ( timer == NULL )
  	{
  	  ERR_FATAL("timer_get_clnt_int_timers: timer is NULL",0,0,0);
  	}  
  
  if(timer == NULL || clnt_timer == NULL)
    return TE_INVALID_PARAMETERS;
  
  *clnt_timer = NULL;
  if(int_timer)
     *int_timer = NULL;
  
  if( 
  	#if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  	 timer->timer_handle == TIMER_INVALID_HANDLE
  	#else
  	 *timer == TIMER_INVALID_HANDLE	
	#endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  	)
    return TE_INVALID_TIMER_HANDLE;
  
  *clnt_timer = timer_get_client_timer(timer, timer_actual_addr, process_idx, &status);
  if(status == TE_SUCCESS && int_timer != NULL)
  {
     *int_timer = timer_get_internal_timer ( *clnt_timer );
     if ( *int_timer == NULL )
        return TE_TIMER_NOT_ACTIVE;
  }
  return status;
} /* timer_get_clnt_int_timers */


/*=============================================================================

FUNCTION TIMER_DEFINE

DESCRIPTION
  Defines a timer structure, initializing members to NULL, and associates the
  timer with the given group.

DEPENDENCIES
  Timer must not already be defined and on a timer list (active).

RETURN VALUE
  None

SIDE EFFECTS
  May link timer group into linked list of timer groups

=============================================================================*/

static timer_client_ptr_type timer_define
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,    

  /* Group timer will belong to (NULL=the "NULL" timer group) */
  timer_group_ptr                 group,

 /* Group Actual Addr. For debug purposes only */
  timer_group_ptr                 group_actual_addr,  

  /* Process Index */
  uint32						  process_idx,
  
  /* Returns status of this function */
  timer_error_type                *status,

  /* Timer priority information */
  timer_priority_enum_type        timer_prio
)
{
  timer_client_ptr_type clnt_timer = NULL;
  timer_group_t* group_ptr;
  
  uint32 group_idx;
  uint32 handle;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ( timer == NULL || status == NULL )
  	{
  	  ERR_FATAL("timer_define: NULL value in timer=ox%x or status=0x%x",
	  	         timer,status,0);
  	}
	
	*status = TE_FAIL;

  if( 
  	  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  	  (timer->timer_handle != TIMER_INVALID_HANDLE) &&  	
  	  #else
  	  (*timer != TIMER_INVALID_HANDLE) &&	  
	  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  	  (NULL != (clnt_timer = timer_get_client_timer(timer, timer_actual_addr, process_idx, status)))
  	)
  {
#ifdef FEATURE_TIMER_ERR_FATAL_FOR_DEFINED_TIMERS
        ERR_FATAL("Handle already in use: timer: 0x%x, Handle: 0x%x", timer_actual_addr, 
                   #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
                   timer->timer_handle, 				   
				   #else
                   *timer, 				   
				   #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
                   0);
#else
        ATS_ERR_2("Handle already in use: timer: 0x%x, Handle: 0x%x", timer_actual_addr, 
                   #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
                   timer->timer_handle
				   #else
                   *timer
				   #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
                 );
#endif
        *status = TE_HANDLE_IN_USE;
        return NULL;
  }

  /* Initialize the timer buffer if it is not initialized */
  if(!timer_q_init_status)  
  	{    
  	  timer_init_client_timers();
	  timer_init_buffer_queue();
	  timer_q_init_status = 1;  
	}  

  /*if the timer is of priority 0, then it needs to be 
   associated with a static timer group that will be 
   guaranteed to be non-deferrable*/
  if(TIMER_PRIORITY_0 == timer_prio) 
  {
    group = &timer_prio0_group;
  }
  
  if(
      #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
	   !GROUP_HANDLE_TO_INDEX(group->group_handle, &group_idx)
      #else
	   !GROUP_HANDLE_TO_INDEX(*group, &group_idx)
      #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  	)
  {  /* Group is not created previously, create it first */
      if((*status = timer_create_new_group(group, group_actual_addr, &group_idx, process_idx)) != TE_SUCCESS)
      {
         return NULL;
      }

  }
  
  group_ptr = timer_internal_groups[group_idx];
  
  /* If this timer group is not yet in the linked list of timer groups,
     add it to the list. */
  if( group_ptr->next == NULL )
  {
    /* As now groups are sequential array, we can remove pointers to next group.
       For now keeping it, later need to remove from other places too */
    group_ptr->next = timers.active.next;
    timers.active.next = group_ptr;
  }

   /* Get the new timer from client timers buffer */
   /* This function should memset to zero and return */
   clnt_timer = timer_get_new_client_timer(status, &handle);
   if(*status != TE_SUCCESS || clnt_timer == NULL)
   {
      return NULL;
   }

  /* Initialize timer structure */
  clnt_timer->timer_client_ptr    = timer_actual_addr;
  clnt_timer->timer_int_handle    = TIMER_BUFFER_INVALID_HANDLE;
  clnt_timer->info.group_idx      = group_idx;
  clnt_timer->info.timer_priority = timer_prio;
  clnt_timer->timer_state         = TIMER_DEF_FLAG;
  clnt_timer->tcb				= (rex_tcb_type *)NULL;
  clnt_timer->sigs				= (rex_sigs_type)0;
  clnt_timer->cbfn.func1		= (timer_t1_cb_type)NULL;
  clnt_timer->data				= (timer_cb_data_type)0;  
  //clnt_timer->timer_state_at      = ticks_now;

  /* Update the thread ID as per the caller */
  clnt_timer->thread_id = qurt_thread_get_id();
  
  /* Fill Process details in the timer */
  clnt_timer->info.process_idx    = process_idx;
  if(process_idx != timer_curr_process_idx)
    clnt_timer->info.remote_process = TIMER_FROM_REMOTE_PROCESS;  

  // debugging purpose
  clnt_timer->callstack_info[0]= __builtin_return_address(2); 
  clnt_timer->callstack_info[1]= __builtin_return_address(3);
  clnt_timer->callstack_info[2]= __builtin_return_address(4);
  clnt_timer->callstack_info[3]= __builtin_return_address(5);

  /* For now not using timer_state in info structure. See if it will be useful */
  //clnt_timer->info.timer_state = (TIMER_DEF_FLAG - TIMER_DEFAULT_FLAG) & 0xF;
  #ifdef FEATURE_TIMER_SLAVE1
  /*assign the slave task here*/
  if ( timer_prio != TIMER_PRIORITY_0 )
  	{
  	 clnt_timer->info.slave_task_idx = timer_assign_slave_task_to_timer();
  	}
  else
  	{
  	 clnt_timer->info.slave_task_idx = 0xF;
  	}

  #endif
  
  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  timer->timer_handle          = handle;
  #else
  *timer = handle;  
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  
  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->tcb		           = (rex_tcb_type *)NULL;
  timer->sigs 		           = (rex_sigs_type)0;
  timer->func1		           = (timer_t1_cb_type)NULL;
  timer->func2		           = (timer_t2_cb_type)NULL;
  timer->data 		           = (timer_cb_data_type)0;
  timer->notify_type           = TIMER_NO_NOTIFY_TYPE;
  timer->group                 = group;
  timer->timer_priority        = timer_prio;
  timer->last_expiry           = 0;
  timer->last_start            = 0;
  timer->timer_state           = TIMER_DEF_FLAG;  
  timer->slave_task_assignment = clnt_timer->info.slave_task_idx;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  group_ptr->timer_cnt++;
  *status = TE_SUCCESS;
  	
  /* Return the client timer structure address */
  return clnt_timer;
} /* timer_define */

boolean timer_is_notify_type_valid(timer_notify_type notify_type)
{
  if(notify_type != TIMER_NATIVE_OS_SIGNAL_TYPE &&
     notify_type != TIMER_NATIVE_OS_SIGNAL2_TYPE &&
   	 notify_type != TIMER_REX_TCB_SIGNAL_TYPE &&
   	 notify_type != TIMER_DALSYS_EVENT_SIGNAL_TYPE &&
  	 notify_type != TIMER_FUNC1_CB_TYPE &&
  	 notify_type != TIMER_FUNC2_CB_TYPE &&
  	 notify_type != TIMER_SIGS_FUNC_DUAL_NOTIFY &&
  	 notify_type != TIMER_NO_NOTIFY_TYPE)
     return FALSE;
  else
     return TRUE;
}

/*=============================================================================

FUNCTION TIMER_DRV_DEF_OSAL_PRIORITY

DESCRIPTION
  Defines and initializes a timer. Signal and function notifications are 
  exclusive. If both are provided at the same time, error will be returned.

  The timer expires when:\n
  - sigs is non-NULL. The signals are set to the task given by tcb.
  - func is non-NULL. The callback function with the argument data
    is called from the timer tasks.

DEPENDENCIES
  None

RETURN VALUE
  TE_SUCCESS  Operation completed successfully
  TE_INVALID_PARAMETERS  Passed params are incorrect for this API
  TE_INVALID_STATE_FOR_OP  If the timer is in paused state

  

SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_def_osal_priority
(
  /* Timer to define */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Group timer will belong to (NULL = the "NULL" timer group) */
  timer_group_ptr                 group,

  /* Group Actual Addr. For Debug purposes only */
  timer_group_ptr                 group_actual_addr,  

  /*ats timer callback type*/
  timer_notify_type               notify_type,

  /* Task to signal and/or task to queue APC call for */
  time_osal_notify_obj_ptr        sigs_func_addr,
  
  /* Task signals to set to the tcb task when timer expires */
  time_osal_notify_data           sigs_mask_data,

  /* timer priority information */
  timer_priority_enum_type        timer_prio,

  /* Process Index */
  unsigned int                    process_idx
)
{
  timer_error_type                status = TE_SUCCESS;
  timer_client_ptr_type           clnt_timer = NULL;
  time_timetick_type              now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Input Validation */
  if(timer == NULL ||
     timer_prio > TIMER_MAX_PRIORITY
	 )
    return TE_INVALID_PARAMETERS;

  if(!timer_is_notify_type_valid(notify_type))
    return TE_INVALID_PARAMETERS;

  if (notify_type == TIMER_SIGS_FUNC_DUAL_NOTIFY)
  	return TE_INVALID_PARAMETERS; /* Multiple notifications are not supported using this API */

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  ATS_INTLOCK();
  
  TIMER_API_LOG(TIMER_API_LOG_TIMER_DEF_OSAL,timer_actual_addr,now);

  /* Allocate new client timer object and provide handle in timer variable */
  clnt_timer = timer_define( timer, timer_actual_addr, group, group_actual_addr, process_idx, &status, timer_prio );
  if(status != TE_SUCCESS || clnt_timer == NULL)
  {
     ATS_INTFREE();
     return status;
  }
  
  /* Copy data to timer structure */

  /* Update the callback type */
  clnt_timer->info.callback_type = notify_type;
  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->notify_type = notify_type;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  
  /* Update the signal information, if any */ 
  if (notify_type == TIMER_REX_TCB_SIGNAL_TYPE ||
      notify_type == TIMER_NATIVE_OS_SIGNAL_TYPE ||
  	  notify_type == TIMER_NATIVE_OS_SIGNAL2_TYPE ||
  	  notify_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE)
  {
     clnt_timer->tcb				= (rex_tcb_type *)sigs_func_addr;;
     clnt_timer->sigs				= (rex_sigs_type)sigs_mask_data;
	 #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
     timer->tcb                     = (rex_tcb_type *)sigs_func_addr;
     timer->sigs		            = (rex_sigs_type)sigs_mask_data;
	 #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */	 
  }
  /* Update the callback information, if any */
  else if(notify_type == TIMER_FUNC1_CB_TYPE ||
  	      notify_type == TIMER_FUNC2_CB_TYPE )
  {
     clnt_timer->cbfn.func1			= (timer_t1_cb_type)sigs_func_addr;
     clnt_timer->data				= (timer_cb_data_type)sigs_mask_data;
	 #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
	 if (notify_type == TIMER_FUNC1_CB_TYPE)
      {
	  timer->func1		            = (timer_t1_cb_type)sigs_func_addr;
	  timer->data 		            = (timer_cb_data_type)sigs_mask_data;
      }
      else
      {
	  timer->func2		            = (timer_t2_cb_type)sigs_func_addr;
	  timer->data 		            = (timer_cb_data_type)sigs_mask_data;
      }	
	 #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  }

  ATS_INTFREE();
  return status;
} /* timer_drv_def_osal_priority */

/*=============================================================================

FUNCTION TIMER_DRV_DEF_PRIORITY
  
DESCRIPTION
  Defines and initializes a timer with specified priority
  When the timer expires:
    if sigs is non-null, those signals are set to the task given by tcb;
    if func is non-null, then
       if tcb is null, then func(data) is called from interrupt context,
       otherwise, func(data) is queued as an Asynchronous Procedure Call (APC)

  Timers may also be staticly declared via the TIMER_DEF() macro

DEPENDENCIES
  None

RETURN VALUE
  TE_INVALID_STATE_FOR_OP  If the timer is in paused state
  TE_SUCCESS  If the operation completed successfully


SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_def_priority
(
  /* Timer to set */
  timer_ptr_type                  timer,
  
  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,    

  /* Group timer will belong to (NULL=the "NULL" timer group) */
  timer_group_ptr                 group,

  /* Group Actual Addr. For Debug purposes only */
  timer_group_ptr                 group_actual_addr,    

  /* Task to signal and/or task to queue APC call for */
  rex_tcb_type                    *tcb,

  /* Task signals to set to the tcb task when timer expires */
  rex_sigs_type                   sigs,

  /* APC (tcb!=NULL) or callback (tcb==NULL) to queue/call at timer expiry */
  timer_t1_cb_type                func,

  /* Arbitrary data for APC or callback */
  timer_cb_data_type              data,

  timer_priority_enum_type        timer_prio,

  /* Process Index */
  uint32						  process_idx
)
{  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  timer_error_type                status = TE_SUCCESS;
  timer_client_ptr_type           clnt_timer = NULL;
  time_timetick_type              now = 0;

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_def_priority: timer is NULL",0,0,0);
  }

  if ( sigs != 0  &&  tcb == NULL )
  {
    ERR_FATAL("Can't set signals to NULL task. T=%x S=%x", timer, sigs, 0);
  }  

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  ATS_INTLOCK();
  
   TIMER_API_LOG(TIMER_API_LOG_TIMER_DEF,timer_actual_addr,now);

  /* Allocate new client timer object and provide handle in timer variable */
  clnt_timer = timer_define( timer, timer_actual_addr, group, group_actual_addr, process_idx, &status, timer_prio );
  if(status != TE_SUCCESS || clnt_timer == NULL )
  {
     ATS_INTFREE();
     return status;
  }
  
  /* Copy data to timer structure */
  clnt_timer->tcb				= tcb;
  clnt_timer->sigs           	= sigs;
  clnt_timer->cbfn.func1		= func;
  clnt_timer->data				= data;

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->tcb	   = tcb;
  timer->sigs	   = sigs; 	 
  timer->func1	   = func;
  timer->func2	   = NULL;
  timer->data	   = data;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  /* Update the callback type information */ 
  if(sigs != 0 && func != NULL)
  {
     clnt_timer->info.callback_type	= TIMER_SIGS_FUNC_DUAL_NOTIFY;
	 
	 /* Log this for multiple notification */
     timer_multi_notify[timer_multi_notify_cnt].tcb_ptr   = tcb;
     timer_multi_notify[timer_multi_notify_cnt].timer_ptr = timer;
     timer_multi_notify_cnt = (timer_multi_notify_cnt+1)&(MAX_TIMER_MULTI_NOTIFY-1);	 
  }
  else if(sigs != 0)
  {
    clnt_timer->info.callback_type	= TIMER_REX_TCB_SIGNAL_TYPE;
  }
  else if(func != NULL)
  {
    clnt_timer->info.callback_type	= TIMER_FUNC1_CB_TYPE;
  }  
  /* No specific notification type is used */
  else
  {
    clnt_timer->info.callback_type = TIMER_NO_NOTIFY_TYPE;
  }

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->notify_type = clnt_timer->info.callback_type;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */


  ATS_INTFREE();
  return status;
} /* timer_drv_def_priority */



/*=============================================================================

FUNCTION TIMER_DRV_DEF2_PRIORITY

DESCRIPTION
  Defines a 'clock callback' style timer and with an ability to set
  the timer priority.

  When the timer expires:
    if func(time_ms, data) is called from interrupt context,

  Timers may also be staticly declared via the TIMER_DEF2() macro

DEPENDENCIES
  None

RETURN VALUE
  TE_INVALID_STATE_FOR_OP  If the timer is in paused state
  TE_SUCCESS  If the operation completed successfully

SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_def2_priority
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Group timer will belong to (NULL=the "NULL" timer group) */
  timer_group_ptr                 group,

  /* Group Actual Addr. For Debug purposes only */
  timer_group_ptr                 group_actual_addr,    

  timer_priority_enum_type        timer_prio,

  /* Process Index */
  uint32						  process_idx
)
{
  time_timetick_type              now = 0;
  timer_error_type                status = TE_SUCCESS;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_def2_priority: timer pointer is NULL",0,0,0);
  }  

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  ATS_INTLOCK();

  TIMER_API_LOG(TIMER_API_LOG_TIMER_DEF2,timer_actual_addr,now);

  /* Initialize new timer structure */
  (void) timer_define( timer, timer_actual_addr, group, group_actual_addr, process_idx, &status, timer_prio );
  if(status != TE_SUCCESS)
  {
     ATS_INTFREE();
     return status;
  }

  ATS_INTFREE();
  return status;

  /* timer_reg() will fill in the timer members "func2" and "data" */


} /* timer_drv_def2 */



/*=============================================================================

FUNCTION TIMER_DRV_REG

DESCRIPTION
  Initializes a 'clock callback' style timer.

  When the timer expires:
    if func(time_ms, data) is called from interrupt context,

DEPENDENCIES
  Timer must have been defined using timer_def2() or TIMER_DEF2()

RETURN VALUE
  TE_INVALID_STATE_FOR_OP  If the timer is not in the right state (defined,cleared,set,etc..)
  TE_INVALID_PARAMETERS  Passed params are incorrect for this API
  TE_SUCCESS


SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_reg
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Function to call at timer expiry */
  timer_t2_cb_type                func,

  /* Arbitrary data for func(time_ms, data) */
  timer_cb_data_type              data,

  /* Time (in ms) until first timer expiry */
  timetick_type                   time_ms,

  /* Period (in ms) between repeated expiries (0 = not periodic) */
  timetick_type                   reload_ms,

  /* Process Index */
  uint32							process_idx
)
{
  timer_client_ptr_type clnt_timer;
  timer_error_type status;
  time_timetick_type              now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_reg: timer pointer is NULL",0,0,0);
  }  

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  /* Lock before updating the timer structure */
  ATS_INTLOCK();

  TIMER_API_LOG(TIMER_API_LOG_TIMER_REG,timer_actual_addr,now);

  if( ( (status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, NULL)) != TE_SUCCESS )
  	  || ( clnt_timer == NULL ) )
  {
     ATS_INTFREE();
     return status;
  }

  /* Copy function & function data to timer structure */
  clnt_timer->cbfn.func2			= func;
  clnt_timer->data					= data;
  clnt_timer->info.callback_type	= TIMER_FUNC2_CB_TYPE;

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->func2    = func;
  timer->func1    = NULL;
  timer->data     = data;
  timer->tcb      = NULL;
  timer->sigs     = (rex_sigs_type ) 0;
  timer->notify_type = TIMER_FUNC2_CB_TYPE;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  /* Start the timer */
  status = timer_drv_set_64(timer, timer_actual_addr, time_ms, reload_ms, T_MSEC, process_idx );

  /* Release the lock */
  ATS_INTFREE();

  return status;


} /* timer_drv_reg */



/*=============================================================================

FUNCTION TIMER_SET_GET_CMD_BUF

DESCRIPTION
  Gets a command buffer for a free timer and fills it out with timer info.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static inline timer_error_type timer_set_get_cmd_buf
(
  timer_ptr_type                   timer,
  timer_ptr_type                   timer_actual_addr,
  uint32                           process_idx,
  timer_client_ptr_type           *clnt_timer_in,
  timer_ptr_internal_type         *free_timer_in
)
{
#ifdef FEATURE_TIMER_DETECT_MEM_LEAKS 
  uint32 iter = 0;
#endif /*FEATURE_TIMER_DETECT_MEM_LEAKS*/
  int32  temp_index;
  /* Internal timer ptr */
  timer_ptr_internal_type free_timer = NULL;
  /* Client timer ptr */
  timer_client_ptr_type   clnt_timer = NULL;
  /* error status */
  timer_error_type        status = TE_SUCCESS;

  /* timetick val */
  time_timetick_type      ticks_now = 0;
  
  /* ---------------------------------------------- */
  *clnt_timer_in = NULL;
  *free_timer_in = NULL;
     
  status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &free_timer);
  if(clnt_timer == NULL)
  {
     return status;
  }
  
  if ( free_timer != NULL ) 
  {
     /* Remove from active list */
     timer_remove(free_timer);   
	 
     if(clnt_timer->timer_state == TIMER_PAUSED_FLAG)
        timers.paused_timers_count--;

     clnt_timer->timer_int_handle = TIMER_BUFFER_INVALID_HANDLE;
     
     /* Clear the internal timer from previous values */
     temp_index                   = free_timer->index;
     memset( free_timer, 0, sizeof( timer_internal_type ));
     free_timer->index            = temp_index ;
  }
  else
  {
     free_timer = (timer_ptr_internal_type) q_get( &timer_free_q );
     if(free_timer == NULL)
     {
      *free_timer_in = NULL;
      return TE_NO_FREE_INTERNAL_TIMER;
     }
     /* increase the count of timers in timer_buffer */
     timer_count_buffer_allocated = MAX_TIMER_BUFFER_NUMBER - q_cnt(&timer_free_q);
  }

  
  /* Todo: Do we require this still? */
#ifdef FEATURE_TIMER_DETECT_MEM_LEAKS 
  
  /* Iterate through timer_buffer to see if we have an active timer
     with an external memory and different handle. This is an error */
  for(iter = 0; iter < MAX_TIMER_BUFFER_NUMBER ; iter++ ) 
  {
    if( timer_buffer_ptr[iter].timer_ext == clnt_timer 
        && timer_buffer_ptr[iter].info.node_state == NODE_IS_NOT_FREE ) 
    {
       ERR_FATAL("Trying to set the same timer with different handle mem=0x%x, handle=0x%x",timer, iter+1,0);      
    }
  }
#endif /* FEATURE_TIMER_DETECT_MEM_LEAKS */

  temp_index                   = free_timer->index;
  memset( free_timer, 0, sizeof(timer_internal_type) );
  free_timer->index            = temp_index;

  /* Update the thread ID as per the caller */
  free_timer->thread_id = qurt_thread_get_id();

  free_timer->info.node_state  = NODE_IS_NOT_FREE;  
  /* Most fields are 0's or NULL's */
  /* Copy data to Internal timer structure */
  free_timer->info.group_idx      = clnt_timer->info.group_idx;
  free_timer->info.process_idx    = clnt_timer->info.process_idx;
  free_timer->info.remote_process = clnt_timer->info.remote_process;  
  free_timer->info.timer_priority = clnt_timer->info.timer_priority;
  free_timer->timer_ext           = clnt_timer;
  free_timer->info.slave_task_idx = clnt_timer->info.slave_task_idx;

  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);
  
  clnt_timer->timer_int_handle = INDEX_TO_HANDLE( temp_index ); 
  clnt_timer->timer_state      = TIMER_SET_FLAG;
  clnt_timer->timer_state_at   = ticks_now;

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->timer_state = TIMER_SET_FLAG;  
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */  
  
  *clnt_timer_in = clnt_timer;
  *free_timer_in = free_timer;
  
  return TE_SUCCESS;
} /* timer_set_get_cmd_buf */



/*=============================================================================

FUNCTION TIMER_DRV_SET_64

DESCRIPTION
  Sets an inactive timer to expire after a given period of time, or changes
  an active timer to expire after a given period of time.

  Optionally, specifies the timer to repeatly expire with a given period.
  If units are ticks, then ticks are supplied in 19MHz domain.

DEPENDENCIES
  None

RETURN VALUE
  TE_INVALID_STATE_FOR_OP  If the timer is not in the right state (defined,cleared,set,etc..)
  TE_INVALID_PARAMETERS  Passed params are incorrect for this API
  TE_SUCCESS


SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/

timer_error_type timer_drv_set_64
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Time (in units below) until first timer expiry */
  time_timetick_type                   time,

  /* Period (in units) between repeated expiries (0 = not periodic) */
  time_timetick_type                   reload,

  /* Unit to measure "ticks" in. */
  timer_unit_type                      unit,

  /* Process Index */
  uint32                              process_idx
  
)
{
   timer_error_type status;
   /* Client timer ptr */
   timer_client_ptr_type		  clnt_timer = NULL;

   /* Local internal timer ptr */
   timer_ptr_internal_type       free_timer = NULL;

   /* Temp Index */
   time_timetick_type           ticks_now = 0;

   #ifdef FEATURE_TIMER_DETECT_MEM_LEAKS
   int                           iter=0;
   #endif /*FEATURE_TIMER_DETECT_MEM_LEAKS*/

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_set_64: timer pointer is NULL",0,0,0);
  }     

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);
  TIMER_API_LOG(TIMER_API_LOG_TIMER_SET,timer_actual_addr,ticks_now);

  status = timer_set_get_cmd_buf(timer, timer_actual_addr, process_idx, &clnt_timer, &free_timer);
  if( status != TE_SUCCESS || clnt_timer == NULL || free_timer == NULL) 
  {
     ATS_INTFREE();
     return status;
  }

  /* Check if the current timer state is incorrect & return accordingly */
  if ( clnt_timer->timer_state < TIMER_DEF_FLAG ||
  	   clnt_timer->timer_state > TIMER_RESUMED_FLAG )
  	{
  	  ATS_ERR_2("Timer=0x%x not in correct state=0x%x to be passed to timer_set()", timer, clnt_timer->timer_state);
  	}

  /* Defensive programming. Clearing the signal that will
   be set when the timer expires.*/
  if ( clnt_timer->info.callback_type == TIMER_NATIVE_OS_SIGNAL_TYPE )
	  {
	  #ifdef FEATURE_TIME_QDSP6
		qurt_anysignal_clear( (qurt_anysignal_t *)clnt_timer->tcb, (time_osal_notify_data)clnt_timer->sigs );
	  #else
        ERR_FATAL("Signal cannot be of NATIVE OS type. timer: 0x%x", timer, 0, 0);
	  #endif /* FEATURE_TIME_QDSP6 */
	  }
  else if ( clnt_timer->info.callback_type == TIMER_REX_TCB_SIGNAL_TYPE || 
  	        clnt_timer->info.callback_type == TIMER_SIGS_FUNC_DUAL_NOTIFY )
  	{
      #ifdef FEATURE_TIMER_REX_IS_AVAILABLE
        (void) rex_clr_sigs( clnt_timer->tcb, clnt_timer->sigs );
      #else
	    ERR_FATAL("Signal cannot be of rex type. timer: 0x%x", timer, 0, 0);
      #endif /* FEATURE_TIMER_REX_IS_AVAILABLE */
  	}
  else if ( clnt_timer->info.callback_type == TIMER_NATIVE_OS_SIGNAL2_TYPE )
	  {
	  #ifdef FEATURE_TIME_QDSP6
		qurt_signal2_clear( (qurt_signal2_t *)clnt_timer->tcb, (time_osal_notify_data)clnt_timer->sigs );
	  #else
        ERR_FATAL("Signal cannot be of NATIVE OS type. timer: 0x%x", timer, 0, 0);
	  #endif /* FEATURE_TIME_QDSP6 */
	  } 
  else if (clnt_timer->info.callback_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE)
     {
        timer_dalsys_sig_reset(clnt_timer->tcb);
     }



  /* ... convert given duration into slow clocks, and save in cache */
  free_timer->cache.duration      = time;

  /* Convert the passed unit to XO ticks */
  DalTimetick_CvtToTimetick64(hTimerHandle,time,unit,&free_timer->cache.duration_sclk);

  /* Retrieve converted time duration from cache */
  time = free_timer->cache.duration_sclk;

      /* Convert and cache given reload interval into slow clocks */
  free_timer->cache.reload      = reload;
  DalTimetick_CvtToTimetick64(hTimerHandle,reload,unit,&free_timer->cache.reload_sclk);

    /* Retrieve converted reload interval from cache */
  reload = free_timer->cache.reload_sclk;

  /* Ensure cached unit value is correct */
  free_timer->cache.unit = unit;

  /* Determine when timer should expire, and set reload */
  free_timer->start  = ticks_now;
  free_timer->expiry = free_timer->start + time;
  free_timer->reload = reload;

  /* Insert timer in either active timer list, or group timer list if it
     belongs to a disabled timer group */
  if ( timer_internal_groups[clnt_timer->info.group_idx]->disabled )
  {
    /* Timer belongs to a disabled group - add it to that group's list */
    timer_insert( &timer_internal_groups[clnt_timer->info.group_idx]->list, free_timer );
  }
  else
  {
    if(TIMER_PRIORITY_0 == free_timer->info.timer_priority) 
    {
      /* Timer is not a member of a disabled group - add to active list */
      timer_insert( &timers.p0_active.list, free_timer);
    }
    else
    {
    /* Timer is not a member of a disabled group - add to active list */
    timer_insert( &timers.active.list, free_timer );
    }

    /* Active timer list has changed - ensure next timer event is correct */
    timer_update_timer_interrupt(TIMER_MVS_TIMER_SET);
  }

  /* Update whether the expiry is absolute or relative */
  clnt_timer->info.expiry_type = TIMER_RELATIVE_EXPIRY;

  ATS_INTFREE();

  return TE_SUCCESS;
} /* timer_drv_set_64 */


/*=============================================================================

FUNCTION TIMER_DRV_SET_ABSOLUTE

DESCRIPTION
  Sets an inactive timer to expire at a given time, or changes
  an active timer to expire at a given time.

DEPENDENCIES
  None

RETURN VALUE
  TE_INVALID_STATE_FOR_OP  If the timer is not in the right state (defined,cleared,set,etc..)
  TE_INVALID_PARAMETERS  Passed params are incorrect for this API
  TE_SUCCESS


SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/

timer_error_type timer_drv_set_absolute
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Time (in XOs) until first timer expiry */
  time_timetick_type              time,

  /* process index */
  unsigned int					process_idx
)
{
  timer_error_type              status = TE_SUCCESS;
  /* Client Timer */
  timer_client_ptr_type         clnt_timer = NULL;
  /* Local internal timer ptr */
  timer_ptr_internal_type       free_timer = NULL;
  /* Timeticks */
  time_timetick_type            preintlock_val = 0;
  time_timetick_type            postintlock_val = 0;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
  /* Specific to set_hard function... check to see if we pass expiry when acq lock */  
  DalTimetick_GetTimetick64(hTimerHandle, &preintlock_val);
  
 ATS_INTLOCK();

  TIMER_API_LOG(TIMER_API_LOG_TIMER_SET_ABSOLUTE, timer_actual_addr, preintlock_val);

  status = timer_set_get_cmd_buf(timer, timer_actual_addr, process_idx, &clnt_timer, &free_timer);
  if(status != TE_SUCCESS || clnt_timer == NULL || free_timer == NULL)
  {
     ATS_INTFREE();
     return status;
  }

  /* Defensive programming. Clearing the signal that will
   be set when the timer expires.*/
  if ( clnt_timer->info.callback_type == TIMER_NATIVE_OS_SIGNAL_TYPE )
	{
	  #ifdef FEATURE_TIME_QDSP6
	    qurt_anysignal_clear( (qurt_anysignal_t *)clnt_timer->tcb, (time_osal_notify_data)clnt_timer->sigs );
	  #else
	    ERR_FATAL("Signal cannot be of NATIVE OS type. timer: 0x%x", timer, 0, 0);
	  #endif /* FEATURE_TIME_QDSP6 */
	}
  else if ( clnt_timer->info.callback_type == TIMER_REX_TCB_SIGNAL_TYPE || 
  	        clnt_timer->info.callback_type == TIMER_SIGS_FUNC_DUAL_NOTIFY )
	{
	  #ifdef FEATURE_TIMER_REX_IS_AVAILABLE
	    (void) rex_clr_sigs( clnt_timer->tcb, clnt_timer->sigs );
	  #else
	    ERR_FATAL("Signal cannot be of rex type. timer: 0x%x", timer, 0, 0);
	  #endif /* FEATURE_TIMER_REX_IS_AVAILABLE */
	}
  else if ( clnt_timer->info.callback_type == TIMER_NATIVE_OS_SIGNAL2_TYPE )
	{
	  #ifdef FEATURE_TIME_QDSP6
		qurt_signal2_clear( (qurt_signal2_t *)clnt_timer->tcb, (time_osal_notify_data)clnt_timer->sigs );
	  #else
	    ERR_FATAL("Signal cannot be of NATIVE OS type. timer: 0x%x", timer, 0, 0);
	  #endif /* FEATURE_TIME_QDSP6 */
	} 
  else if (clnt_timer->info.callback_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE)
     {
        timer_dalsys_sig_reset(clnt_timer->tcb);
     }



  /* Get time just before calculations */
  DalTimetick_GetTimetick64(hTimerHandle, &postintlock_val);
  	
  if( postintlock_val - preintlock_val  > time - preintlock_val )
  {
    /* We will want to expire this timer right away */
    /* ... convert given duration into slow clocks, and save in cache */
    free_timer->cache.duration      = 0;
    free_timer->cache.duration_sclk = 0;
    
    /* No reload allowed for this */
    free_timer->cache.reload      = 0;
    free_timer->cache.reload_sclk = 0;
    
    /* Ensure cached unit value is correct */
    free_timer->cache.unit = T_TICK;
    
    /* Determine when timer should expire, and set reload */
    
    free_timer->start  = postintlock_val;
    free_timer->expiry = postintlock_val;
    free_timer->reload = 0;
  }
  else
  {  
   /* ... convert given duration into slow clocks, and save in cache */
   free_timer->cache.duration      = time - postintlock_val;
   free_timer->cache.duration_sclk = free_timer->cache.duration;
  
       /* No reload allowed for this */
   free_timer->cache.reload      = 0;
   free_timer->cache.reload_sclk = 0; /* Is this okay? */
  
   /* Ensure cached unit value is correct */
   free_timer->cache.unit = T_TICK;
  
   /* Determine when timer should expire, and set reload */
  
   free_timer->start  = postintlock_val;
   free_timer->expiry = time;
   free_timer->reload = 0;
  }
  
    /* Insert timer in either active timer list, or group timer list if it
     belongs to a disabled timer group */
  
  if ( timer_internal_groups[clnt_timer->info.group_idx]->disabled )
  {
    /* Timer belongs to a disabled group - add it to that group's list */
    timer_insert( &timer_internal_groups[clnt_timer->info.group_idx]->list, free_timer );
  }
  else
  {
    if(TIMER_PRIORITY_0 == free_timer->info.timer_priority) 
    {
      /* Timer is not a member of a disabled group - add to active list */
      timer_insert( &timers.p0_active.list, free_timer);
    }
    else
    {
      /* Timer is not a member of a disabled group - add to active list */
      timer_insert( &timers.active.list, free_timer );
    }
  
    /* Active timer list has changed - ensure next timer event is correct */
    timer_update_timer_interrupt(TIMER_MVS_TIMER_SET);
  }

  clnt_timer->info.expiry_type = TIMER_ABSOLUTE_EXPIRY;

 ATS_INTFREE();
 
  return TE_SUCCESS;
} /* timer_drv_set_absolute */

/*=============================================================================

FUNCTION TIMER_EXPIRES_IN

DESCRIPTION
  Get number of ticks before timer expires

DEPENDENCIES
  Timer must be active (on a timer list)
  Internal function, called from ATS_INTLOCK context.

RETURN VALUE
  Number of ticks before timer expires.
  Zero is returned for inactive timers, and timers that have expired or
  are expiring.

SIDE EFFECTS
  None

=============================================================================*/

time_timetick_type timer_expires_in
(
  /* Timer to get */
  timer_ptr_internal_type         timer
)
{
  /* Current slow clock tick count */
  time_timetick_type                   now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get the current slow clock count value */

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  /* Determine if the timer has expired or not */

  if ( timer->expiry >=  now  )
  {
    /* Timer has not expired - compute time till expiry. */

    return ( timer->expiry - now );
  }
  else
  {
    /* Timer has expired - return zero */

    return 0;
  }

} /* timer_expires_in */


/*=============================================================================

FUNCTION TIMER_DRV_UNDEF

DESCRIPTION
  Deletes an active timer

DEPENDENCIES
  None

RETURN VALUE
  TE_SUCCESS if success
  TE_INVALID_PARAMETERS if timer  pointer passed is invalid
  TE_TIMER_ALREADY_IN_SAME_STATE if timer is already deleted

SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_undef
(
  timer_ptr_type                  timer,
  timer_ptr_type                  timer_actual_addr,
  uint32                          process_idx
)
{
timer_error_type				status = TE_SUCCESS;

timer_client_ptr_type			clnt_timer = NULL;

time_timetick_type              ticks = 0;

/* Pointer to internal timer */
timer_ptr_internal_type 		timer_int = NULL;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

ATS_INTLOCK();

DalTimetick_GetTimetick64(hTimerHandle, &ticks);

TIMER_API_LOG(TIMER_API_LOG_TIMER_UNDEF, timer_actual_addr, ticks);

status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &timer_int);
if(clnt_timer == NULL)
{
  ATS_INTFREE();
  return TE_TIMER_ALREADY_IN_SAME_STATE;
}

/* Remove from active list if active, Free the internal timer */
if ( timer_int && timer_int->list != NULL )
{
  /* Timer is active - record remaining time and remove timer */
  
  timer_remove( timer_int);
  timer_free_internal_timer(timer_int);

  /* Active timer list has changed - ensure next timer event is correct */
  timer_update_timer_interrupt(TIMER_MVS_TIMER_UNDEFINED);
}
/*if the timer was paused, then it was removed from the active list
 or the disabled list, but it was not returned back to the free 
 queue. Hence, if a paused timer was cleared, return it back
 to the free queue*/
else if(timer_int && clnt_timer->timer_state == TIMER_PAUSED_FLAG) 
{
  timer_free_internal_timer(timer_int); 	 
  timers.paused_timers_count--;
}

timer_internal_groups[clnt_timer->info.group_idx]->timer_cnt--;
clnt_timer->timer_state = TIMER_UNDEF_FLAG;
clnt_timer->timer_state_at = ticks;
clnt_timer->timer_int_handle = TIMER_BUFFER_INVALID_HANDLE;

/* Free the client timer */
status = timer_free_client_timer(timer);

#ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  memset( timer, 0, sizeof(timer_type));
  timer->timer_state = TIMER_UNDEF_FLAG;
#endif

#if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  timer->timer_handle = TIMER_INVALID_HANDLE;
#else
  *timer = TIMER_INVALID_HANDLE;
#endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */


ATS_INTFREE();
return status;
} /* timer_drv_undef */


/*=============================================================================

FUNCTION TIMER_DRV_DELETE_PROCESS_TIMERS

DESCRIPTION
  Deletes timers related to a process

DEPENDENCIES
  None

RETURN VALUE
  TE_SUCCESS if success
  Otherwise according to error will returns one of timer_error_type
  

SIDE EFFECTS
  None

=============================================================================*/
timer_error_type timer_drv_delete_process_timers(uint32 process_idx)
{
   timer_client_ptr_type clnt_timer = NULL;
   timer_ptr_internal_type timer_int = NULL;
   timers_client_buffer_type *chunk_ptr = NULL;
   uint32 tid = 0;
   uint32 chunk_idx = 0;
   uint32 num_timers = 0;
   
   ATS_INTLOCK();
   
   for(chunk_idx = 0; chunk_idx < timer_client_chunks_allocated; chunk_idx++)
   {
      chunk_ptr = &timer_client_chunks[chunk_idx];
      clnt_timer = &timer_client_chunks[chunk_idx].buffer[0];
      num_timers = timer_client_chunks[chunk_idx].num_timers;
      
      for(tid=0; tid < num_timers; tid++, clnt_timer++)
      {
         if( (clnt_timer->info.process_idx == process_idx) &&
             (clnt_timer->info.node_state != NODE_IS_FREE) )
         {
            timer_int = timer_get_internal_timer ( clnt_timer );
            if ( timer_int != NULL )
            {
              /* Remove from active list if active, Free the internal timer */
              if ( timer_int->list != NULL )
              {
                /* Timer is active - record remaining time and remove timer */
                
                timer_remove( timer_int);
                timer_free_internal_timer(timer_int);
              }
              else if(clnt_timer->timer_state == TIMER_PAUSED_FLAG) 
              {
                timer_free_internal_timer(timer_int);      
                timers.paused_timers_count--;
              }
            }
            clnt_timer->timer_int_handle = TIMER_BUFFER_INVALID_HANDLE;
            clnt_timer->info.node_state  = NODE_IS_FREE;
            clnt_timer->timer_state      = TIMER_DEFAULT_FLAG;
            //clnt_timer->timer_state_at = timetick_get_tmp64();
            clnt_timer->timer_state_at = 0;  /* Get timetick call takes 10usecs around. Therefore for now setting zero */

            /* If there are no previous free timers, keep buffer_idx as this tid */
            if(chunk_ptr->allocated_cnt == chunk_ptr->num_timers)
            {
               chunk_ptr->buffer_idx = tid;
            }

			/* Decrement the allocated count for this chunk */
			chunk_ptr->allocated_cnt--;

            /* Increment the count of free client timers */
            timer_free_client_timers_cnt++;			
			
         } /* if */
      } /* for tid */
   } /* for chunk_idx*/

   /* Active timer list has changed - ensure next timer event is correct */
   timer_update_timer_interrupt(TIMER_MVS_TIMER_UNDEFINED);
   
   ATS_INTFREE();
   return TE_SUCCESS;
} /* timer_drv_delete_process_timers */


/*=============================================================================

FUNCTION TIMER_DRV_DELETE_PROCESS_GROUPS

DESCRIPTION
  Deletes groups related to a process

DEPENDENCIES
  None

RETURN VALUE
  TE_SUCCESS if success

SIDE EFFECTS
  None

=============================================================================*/
timer_error_type timer_drv_delete_process_groups(uint32 process_idx)
{
   timer_group_t_ptr group = NULL;
   timer_group_t_ptr prev_group = &timers.active;  
   timer_group_t_ptr group_to_free = NULL;  

   //ERR_FATAL("Intentionally doing it",0,0,0);

   group = prev_group->next;
   
   ATS_INTLOCK();
   do
   {
	 if ( group->process_idx == process_idx )
	 { 
       if (prev_group != NULL)
       	{
       	  prev_group->next = group->next;
       	}

       group_to_free = group;
	   group = group->next;
   
	   /* delete & free the internal timer group belonging to this PD */
	   timer_delete_group(group_to_free->group_idx);

	   continue;
	 }

	 /* Update the prev_group & group pointers */
     prev_group = group;
	 group = group->next;
	 
   }while ( group != &timers.active );



   ATS_INTFREE();
   return TE_SUCCESS;
} /* timer_drv_delete_process_groups */


/*=============================================================================

FUNCTION TIMER_DRV_STOP

DESCRIPTION
  Stops an active timer

DEPENDENCIES
  None

RETURN VALUE
  TE_INVALID_PARAMETERS
  TE_TIMER_ALREADY_IN_SAME_STATE
  TE_SUCCESS

SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_stop
(
  timer_ptr_type				   timer,
  timer_ptr_type                   timer_actual_addr,    
  timer_unit_type    	           unit,
  time_timetick_type			   *rem_time,
  uint32                           process_idx
)
{

  /* Ticks until timer expires */
  time_timetick_type              ticks = 0;

  time_timetick_type              ticks_now = 0;  

  /* Pointer to internal timer */
  timer_ptr_internal_type         timer_int = NULL;

  timer_client_ptr_type           clnt_timer = NULL;

  timer_internal_type             timer_internal_copy;

  timer_error_type                status = TE_SUCCESS;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ATS_DECLARE_ISAV(isave);

  ATS_INTLOCK_SAV(isave);
  
  if(rem_time != NULL)
     *rem_time = 0;

  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);
  TIMER_API_LOG(TIMER_API_LOG_TIMER_STOP,timer_actual_addr,ticks_now);

  status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &timer_int);

  /* Timer clr logging */
  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    timer_clr_log[timer_clr_log_cnt].ext_timer 		= timer_actual_addr;
    timer_clr_log[timer_clr_log_cnt].int_timer 		= timer_int;
    timer_clr_log[timer_clr_log_cnt].ts 			= ticks_now;
    timer_clr_log[timer_clr_log_cnt].ret_val 		= status;
    if (clnt_timer != NULL)
      timer_clr_log[timer_clr_log_cnt].timer_state 	= clnt_timer->timer_state;
    else
    	timer_clr_log[timer_clr_log_cnt].timer_state= TIMER_UNDEF_FLAG;
    timer_clr_log[timer_clr_log_cnt].tcb_ptr 		= (rex_tcb_type *)rex_self();
  #endif /* FEATURE_ENABLE_TIMER_DEBUG_VARIABLES */

  if(status != TE_SUCCESS || clnt_timer == NULL)
  {
     #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
       timer_clr_log_cnt = (timer_clr_log_cnt+1)&(MAX_TIMER_CLR_LOG-1);    
	 #endif /* FEATURE_ENABLE_TIMER_DEBUG_VARIABLES */

     ATS_INTFREE_SAV(isave);
     return status;
  }

  if ( timer_int->list != NULL )
  {
    /* Timer is active - record remaining time and remove timer */
	
	memscpy(&timer_internal_copy, sizeof(timer_internal_copy), timer_int, sizeof(timer_internal_type));
    timer_remove( timer_int);
    timer_free_internal_timer(timer_int);
	status = TE_SUCCESS;

    /* Active timer list has changed - ensure next timer event is correct */
    timer_update_timer_interrupt(TIMER_MVS_TIMER_CLEARED);

    /* Get the ticks before timers expiry */
    ticks = timer_expires_in( &timer_internal_copy );
  }
  /*if the timer was paused, then it was removed from the active list
   or the disabled list, but it was not returned back to the free 
   queue. Hence, if a paused timer was cleared, return it back
   to the free queue*/
  else if(clnt_timer->timer_state == TIMER_PAUSED_FLAG) 
  {
    timer_free_internal_timer(timer_int); 	 
    timers.paused_timers_count--;
    ticks = 0;
    status = TE_SUCCESS;
  }
  else
  {
    /* Timer was not active */
    ticks = 0;
    status = TE_TIMER_ALREADY_IN_SAME_STATE;
  }

  clnt_timer->timer_state = TIMER_CLEARED_FLAG;
  clnt_timer->timer_state_at = ticks_now;
  clnt_timer->timer_int_handle = TIMER_BUFFER_INVALID_HANDLE;

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->timer_state = TIMER_CLEARED_FLAG;
  timer->last_expiry = 0;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  /* Timer clr logging */
  #ifdef FEATURE_ENABLE_TIMER_DEBUG_VARIABLES
    timer_clr_log[timer_clr_log_cnt].ret_val = status;
    timer_clr_log_cnt = (timer_clr_log_cnt+1)&(MAX_TIMER_CLR_LOG-1); 
  #endif /* FEATURE_ENABLE_TIMER_DEBUG_VARIABLES */

  ATS_INTFREE_SAV(isave);

  if (rem_time != NULL)
  	{
	 DalTimetick_CvtFromTimetick64(hTimerHandle,ticks,unit,rem_time);
  
     /* Return at least 1 as the remaining time if the timer was cleared */
     if (status == TE_SUCCESS && *rem_time == 0)
     	*rem_time = 1;
  	}

  return status;
}/* timer_drv_stop */


/*=============================================================================

FUNCTION TIMER_DRV_PAUSE

DESCRIPTION
  Pauses an active timer.

DEPENDENCIES
  Should only be called for a timer that is running.

RETURN VALUE
  TE_INVALID_TIMER_HANDLE  If the timer is not active
  TE_SUCCESS  If the operation was success


SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_pause
(
  /* Timer to pause */
  timer_ptr_type                  timer,
  timer_ptr_type                  timer_actual_addr,  
  uint32                          process_idx  
)
{
  /* Pointer to internal timer */
  timer_ptr_internal_type         timer_int = NULL;
  ATS_DECLARE_ISAV(isave);
   time_timetick_type             now = 0;

  timer_error_type                status = TE_SUCCESS;

  /* Client timer */
  timer_client_ptr_type           clnt_timer = NULL;   
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_pause: timer pointer is NULL",0,0,0);
  }    


  ATS_INTLOCK_SAV(isave);

  DalTimetick_GetTimetick64(hTimerHandle, &now);
  TIMER_API_LOG(TIMER_API_LOG_TIMER_PAUSE,timer_actual_addr,now);

  status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &timer_int);
  if(clnt_timer == NULL || timer_int == NULL) 
  { /* Here timer_int is required, if its present, clnt_timer is also present */
     ATS_INTFREE_SAV(isave);
     return status;
  }

  if ( timer_int->list != NULL )
  {
    /* Timer is active - remove it, and record remaining time */
 
    timer_remove( timer_int);

    /* Active timer list has changed - ensure next timer event is correct */
    timer_update_timer_interrupt( TIMER_MVS_TIMER_PAUSED );

    timer_int->expiry = timer_expires_in( timer_int );

    timers.paused_timers_count++;
  }

  clnt_timer->timer_state    = TIMER_PAUSED_FLAG;
  clnt_timer->timer_state_at = now;

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->timer_state = TIMER_PAUSED_FLAG;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  
  ATS_INTFREE_SAV(isave);

  return TE_SUCCESS;
} /* timer_drv_pause */



/*=============================================================================

FUNCTION TIMER_DRV_RESUME

DESCRIPTION
  Resumes a previously paused timer.

  If a timer was paused 5 seconds before its expiry, and 30 seconds later
  timer_resume( ) is called, the timer will expire 5 seconds after that point.

DEPENDENCIES
  Must only be called for a timer that has been paused.

RETURN VALUE
TE_INVAID_TIMER_HANDLE - If passed timer does not have any internal timer (it is not paused)
TE_INVALID_STATE_FOR_OP - If the timer that was asked to resume is not in paused state.
TE_SUCCESS - If the operation was success


SIDE EFFECTS
  May generate signals to a task, which can cause task switches.
  May queue an Asynchronous Procedure Call.

=============================================================================*/

timer_error_type timer_drv_resume
(
  /* Timer to resume */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Process index */
  uint32                          process_idx
)
{
  /* Ticks until timer expires */
  time_timetick_type              now = 0;

  timer_error_type                status = TE_SUCCESS;

  /* Client timer ptr */
  timer_client_ptr_type           clnt_timer = NULL;  

  /* Pointer to internal timer */
  timer_ptr_internal_type         timer_int = NULL;
  ATS_DECLARE_ISAV(isave);
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_resume: timer pointer is NULL",0,0,0);
  }     
 
  ATS_INTLOCK_SAV(isave);

  DalTimetick_GetTimetick64(hTimerHandle, &now);
  TIMER_API_LOG(TIMER_API_LOG_TIMER_RESUME,timer_actual_addr,now);

  status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &timer_int);
  if(timer_int == NULL || clnt_timer == NULL) 
  { /* Here timer_int is required, if its present, clnt_timer is also present */
     ATS_INTFREE_SAV(isave);
     return status;
  }
  
  if(clnt_timer->timer_state != TIMER_PAUSED_FLAG)
  {
     ATS_INTFREE_SAV(isave);
     return TE_INVALID_STATE_FOR_OP;
  }
  
  if ( timer_int->list == NULL )
  {
    /* Determine when timer should expire */
    timer_int->expiry += now;

    /* Insert timer in either active timer list, or group timer list if it
       belongs to a disabled timer group */

    if ( timer_internal_groups[clnt_timer->info.group_idx]->disabled == 1)
    {
      /* Timer belongs to a disabled group - add it to that group's list */
      timer_insert( &timer_internal_groups[clnt_timer->info.group_idx]->list, timer_int );
    }
    else
    {
      if(TIMER_PRIORITY_0 == timer_int->info.timer_priority) 
      {
        /* Timer is not a member of a disabled group - add to active list */
        timer_insert( &timers.p0_active.list, timer_int );
      }
      else
      {
      /* Timer is not a member of a disabled group - add to active list */
      timer_insert( &timers.active.list, timer_int );
      }

      /* Active timer list has changed - ensure next timer event is correct */
      timer_update_timer_interrupt(TIMER_MVS_TIMER_RESUMED);
	  timers.active_timers_count++;
    }

    timers.paused_timers_count--;
  }
  
  clnt_timer->timer_state    = TIMER_RESUMED_FLAG;
  clnt_timer->timer_state_at = now;

  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
  timer->timer_state = TIMER_RESUMED_FLAG;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  ATS_INTFREE_SAV(isave);
  return TE_SUCCESS;
} /* timer_drv_resume */

/*=============================================================================

FUNCTION TIMER_DRV_CLR_OSAL_THREAD_TIMERS

DESCRIPTION
  Stops active timers belonging to the given task thread

DEPENDENCIES
  None

RETURN VALUE
  TE_SUCCESS
  TE_FAIL  If there is any corruption in the internal timer memory


SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_clr_osal_thread_timers
(
  /* Timers belonging to this task will be cleared (stopped). */
  time_osal_thread_id                    thread_id
)
{
  /* Pointer to a timer group.  Used for walking list of timer groups */
  timer_group_t *                 group = NULL;

  /* Pointer to a timer.  Used for walking list of timers */
  timer_ptr_internal_type         timer = NULL;

  /* Pointer to the next timer.  Used for walking list of timers */
  timer_ptr_internal_type         next = NULL;

  /* Timetick value for logging */
  time_timetick_type              now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_CLR_OSAL_THREAD_TIMERS,NULL,now);

  /* Walk list of timer groups, starting with the faux active group */

  group = &timers.active;

  do
  {
    /* Walk the list of timers in this group, and
       remove timers which belong to given task */

    for( timer = group->list.first;  timer != NULL;  timer = next)
    {
      /* Save next timer in list, in case this timer's next field in NULL'd */
      next = timer->next;

      if ( timer->thread_id == thread_id)
      {
        if ( timer->timer_ext == NULL )
          {
           ATS_INTFREE();
           ERR_FATAL (" Timer must have an ext timer, timer_ptr=0x%x timer_ext=0x%x",
                    timer,timer->timer_ext,0);
		   return TE_FAIL;
          }

        /* Remove the timer from the list */
        timer_remove( timer);
		timer_free_internal_timer(timer);

		timer->timer_ext->timer_state		  = TIMER_CLEARED_FLAG;
		timer->timer_ext->timer_state_at	  = now;
		timer->timer_ext->timer_int_handle    = TIMER_BUFFER_INVALID_HANDLE;		
      }
    }

    /* Determine the next timer group in the chain ... stopping when we
       return to the start of the list with the faux timer group */

    group = group->next;
  }
  while ( group != &timers.active );


  /* Active timer list may have changed - ensure next timer event is correct */
  timer_update_timer_interrupt(TIMER_MVS_TIMER_CLEAR_TASK_TIMERS );


  ATS_INTFREE();

  return TE_SUCCESS;

} /* timer_drv_clr_osal_thread_timers */



/*=============================================================================

FUNCTION TIMER_DRV_CLR_TASK_TIMERS

DESCRIPTION
  Stops active timers belonging to the given task

DEPENDENCIES
  None

RETURN VALUE
  TE_SUCCESS
  TE_FAIL  If there is any corruption in the internal timer memory


SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_clr_task_timers
(
  /* Timers belonging to this task will be cleared (stopped). */
  rex_tcb_type                    *tcb
)
{
  /* Pointer to a timer group.  Used for walking list of timer groups */
  timer_group_t *                 group = NULL;

  /* Pointer to a timer.  Used for walking list of timers */
  timer_ptr_internal_type         timer = NULL;

  /* Pointer to the next timer.  Used for walking list of timers */
  timer_ptr_internal_type         next = NULL;

  /* Timetick value for logging */
  time_timetick_type              now = 0;

  /* Client timer pointer */
  timer_client_ptr_type           clnt_timer = NULL;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_CLR_TASK_TIMERS,NULL,now);

  /* Walk list of timer groups, starting with the faux active group */

  group = &timers.active;

  do
  {
    /* Walk the list of timers in this group, and
       remove timers which belong to given task */

    for( timer = group->list.first;  timer != NULL;  timer = next)
    {
      /* Save next timer in list, in case this timer's next field in NULL'd */
      next = timer->next;
	  
      if ( timer->timer_ext != NULL )
          clnt_timer = timer->timer_ext;
      else
        {
          ERR_FATAL (" Timer must have an ext timer, internal_timer=0x%x",timer,0,0);
          ATS_INTFREE();
          return TE_FAIL;
        }	  

      if ( clnt_timer->tcb == tcb)
      {
        /* Remove the timer from the list */
		timer_remove( timer );
		timer_free_internal_timer(timer);
		  
		clnt_timer->timer_state		  = TIMER_CLEARED_FLAG;
		clnt_timer->timer_state_at	  = now;
		clnt_timer->timer_int_handle  = TIMER_BUFFER_INVALID_HANDLE;

      }
    }

    /* Determine the next timer group in the chain ... stopping when we
       return to the start of the list with the faux timer group */

    group = group->next;
  }
  while ( group != &timers.active );


  /* Active timer list may have changed - ensure next timer event is correct */
  timer_update_timer_interrupt(TIMER_MVS_TIMER_CLEAR_TASK_TIMERS );


  ATS_INTFREE();

  return TE_SUCCESS;

} /* timer_drv_clr_task_timers */




/*=============================================================================

FUNCTION TIMER_DRV_GET_64

DESCRIPTION
  Get number of ticks in 19MHz domain before timer expires

DEPENDENCIES
  None

RETURN VALUE
  Number of ticks in 19MHz domain before timer expires.
  Zero is returned for inactive timers, and timers that have expired or
  are expiring. Fractional value which might truncate to zero 
  depending on unit passed as parameter is rounded to 1.

SIDE EFFECTS
  None

=============================================================================*/

time_timetick_type timer_drv_get_64
(
  /* Timer to get */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,    

  /* Unit to measure "ticks before timer expires" in. */
  timer_unit_type                 unit,

  /* process index */
  uint32                          process_idx
)
{
  /* Ticks until timer expires */
  time_timetick_type              ticks = 0;
  /* Client timer ptr */
  timer_client_ptr_type           clnt_timer = NULL;
  /* Pointer to internal timer */
  timer_ptr_internal_type         timer_int = NULL;
  ATS_DECLARE_ISAV(isave);	
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_get_64: timer pointer is NULL",0,0,0);
  }    

  
  ATS_INTLOCK_SAV(isave);

  DalTimetick_GetTimetick64(hTimerHandle, &ticks);
  /* This API LOG should be before any possible early exit of the function */
  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET,timer_actual_addr,ticks);

  if(timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &timer_int) != TE_SUCCESS)
  {
     ATS_INTFREE_SAV(isave);
     return 0;
  }

  if ( timer_int->list != NULL )
  {
    /* Timer is active. Ensure non-zero tick count */

    ticks = timer_expires_in( timer_int );
    if ( ticks == 0 )
    {
      ticks = 1;
    }

  }
  else
  {
    /* Timer is not active */

    ticks = 0;
  }

  ATS_INTFREE_SAV(isave);

  /* If timer is active then ensure that return value is non-zero */
  if ( ticks != 0 )
  {
    DalTimetick_CvtFromTimetick64(hTimerHandle,ticks,unit,&ticks);
    if ( ticks == 0 )
    {
      ticks = 1;
    }
  }

  return ticks;

} /* timer_drv_get_64 */


/*=============================================================================

FUNCTION TIMER_DRV_GET_PARAMS

DESCRIPTION
  Updates the passed structure as per the timer parameters.

DEPENDENCIES
   None

RETURN VALUE
  TE_SUCCESS if success
  TE_INVALID_PARAMETERS if passed pointers are NULL


SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_get_params
(
  timer_ptr_type                    timer,
  timer_ptr_type                    timer_actual_addr,
  timer_attrib_struct_type *        timer_attrib_struct_ptr,
  uint32                            process_idx
)
{

  timer_error_type        status = TE_SUCCESS;
  timer_client_ptr_type   clnt_timer = NULL;
  time_timetick_type      ticks = 0;

  ATS_INTLOCK();
  
  DalTimetick_GetTimetick64(hTimerHandle, &ticks);
  
  /* This API LOG should be before any possible early exit of the function */
  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_PARAMS,timer_actual_addr,ticks);    
  
  status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, NULL);
  if(status != TE_SUCCESS || clnt_timer == NULL)
  {
     ATS_INTFREE();
     return status;
  }
  
  /* Update the passed timer attribute structure members */
  timer_attrib_struct_ptr->tcb_sigs_obj      = (void *)clnt_timer->tcb;
  timer_attrib_struct_ptr->sigs              = (uint32)clnt_timer->sigs;
  if (clnt_timer->info.callback_type == TIMER_FUNC2_CB_TYPE)
	{
	  timer_attrib_struct_ptr->func1             = NULL;
	  timer_attrib_struct_ptr->func2             = clnt_timer->cbfn.func2;
  	}
  else
  	{
	  timer_attrib_struct_ptr->func1             = clnt_timer->cbfn.func1;  
	  timer_attrib_struct_ptr->func2             = NULL;  	
  	}
  timer_attrib_struct_ptr->data              = clnt_timer->data;
  timer_attrib_struct_ptr->notify_type       = clnt_timer->info.callback_type;
  timer_attrib_struct_ptr->timer_state       = clnt_timer->timer_state;

  ATS_INTFREE();
  return TE_SUCCESS;
} /* timer_drv_get_params */


/*=============================================================================

FUNCTION TIMER_DRV_SET_PARAMS

DESCRIPTION
  Updates the timer as per the passed params.

DEPENDENCIES
  None

RETURN VALUE
  TE_SUCCESS if success
  TE_INVALID_PARAMETERS if passed pointers are NULL


SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_set_params
(
  timer_ptr_type                    timer,
  timer_ptr_type                    timer_actual_addr,
  timer_attrib_set_type             to_set,
  timer_attrib_struct_type *        timer_attrib_struct_ptr,
  uint32                            process_idx
)
{
  time_timetick_type       ticks = 0;
  timer_ptr_internal_type  int_timer = NULL;
  timer_client_ptr_type    clnt_timer = NULL;
  timer_error_type         status = TE_SUCCESS;

  ATS_INTLOCK();
  
  DalTimetick_GetTimetick64(hTimerHandle, &ticks);
  
  /* This API LOG should be before any possible early exit of the function */
  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_PARAMS,timer_actual_addr,ticks);  


  status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &int_timer);
  if(status != TE_SUCCESS && status != TE_TIMER_NOT_ACTIVE)
  {
     ATS_INTFREE();
     return status;
  }

  /* Update the timer notification attributes */
  if (to_set == TIMER_UPDATE_NOTIFICATION_PARAMS)
  	{
  	  clnt_timer->info.callback_type = timer_attrib_struct_ptr->notify_type;
	  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
	  timer->notify_type = timer_attrib_struct_ptr->notify_type;
	  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
	  
  	  if ( (timer_attrib_struct_ptr->notify_type == TIMER_NATIVE_OS_SIGNAL_TYPE) ||
           (timer_attrib_struct_ptr->notify_type == TIMER_NATIVE_OS_SIGNAL2_TYPE) ||
           (timer_attrib_struct_ptr->notify_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE) ||           
           (timer_attrib_struct_ptr->notify_type == TIMER_REX_TCB_SIGNAL_TYPE) ||
	  	   (timer_attrib_struct_ptr->notify_type == TIMER_SIGS_FUNC_DUAL_NOTIFY) )
  	  	{
  	  	  clnt_timer->tcb			= (rex_tcb_type *)timer_attrib_struct_ptr->tcb_sigs_obj;
		  clnt_timer->sigs			= (rex_sigs_type)timer_attrib_struct_ptr->sigs;
		  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
		  timer->tcb                = clnt_timer->tcb;
		  timer->sigs               = clnt_timer->sigs;
		  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */
  	  	}

	  if ( (timer_attrib_struct_ptr->notify_type == TIMER_FUNC1_CB_TYPE) ||
	  	   (timer_attrib_struct_ptr->notify_type == TIMER_SIGS_FUNC_DUAL_NOTIFY) )
	  	{
          clnt_timer->cbfn.func1	= timer_attrib_struct_ptr->func1;
		  clnt_timer->data			= timer_attrib_struct_ptr->data;
		  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
		  timer->func1              = timer_attrib_struct_ptr->func1;
		  timer->data               = timer_attrib_struct_ptr->data;
		  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */		  
	  	}
	  else if (timer_attrib_struct_ptr->notify_type == TIMER_FUNC2_CB_TYPE)
	  	{
          clnt_timer->cbfn.func2	= timer_attrib_struct_ptr->func2;
		  clnt_timer->data			= timer_attrib_struct_ptr->data;
		  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
		  timer->func2              = timer_attrib_struct_ptr->func2;
		  timer->data               = timer_attrib_struct_ptr->data;
		  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */		  
	  	}
	  else if (timer_attrib_struct_ptr->notify_type == TIMER_NO_NOTIFY_TYPE)
	  	{
  	  	  clnt_timer->tcb			= (rex_tcb_type *)NULL;
     	  clnt_timer->sigs			= (rex_sigs_type)0;
	      clnt_timer->cbfn.func1	= (timer_t1_cb_type)NULL;
	      clnt_timer->data			= (timer_cb_data_type)0;
		  #ifdef TIMER_SUPPORT_OLD_TIMER_STRUCTURE
		  timer->tcb                = (rex_tcb_type *)NULL;
		  timer->sigs               = (rex_sigs_type)0;
		  timer->func1              = (timer_t1_cb_type)NULL;
		  timer->func2              = (timer_t2_cb_type)NULL;		  
		  timer->data               = (timer_cb_data_type)0;	  
		  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */		  
	  	}
	  else
	  	{
	  	  ATS_INTFREE();
		  return TE_INVALID_PARAMETERS;
	  	}
 	}
  else 
  	{
  	ATS_INTFREE();
  	return TE_INVALID_PARAMETERS;
  	}

  /* Check if there is any internal timer for this external timer as well */
  if (int_timer != NULL)
  	{
     int_timer->info.callback_type = clnt_timer->info.callback_type;
  	}

  timer_set_params_log[timer_set_params_log_cnt].timer = timer_actual_addr;
  timer_set_params_log[timer_set_params_log_cnt].ts = ticks;
  timer_set_params_log[timer_set_params_log_cnt].timer_state = clnt_timer->timer_state;
  timer_set_params_log[timer_set_params_log_cnt].notification = timer_attrib_struct_ptr->notify_type;
  timer_set_params_log_cnt = (timer_set_params_log_cnt+1)&(MAX_TIMER_SET_PARAMS_LOG-1);

  ATS_INTFREE();

  /* Print message whenever somebody updates the timer structure */
  ATS_MSG_HIGH("timer_set_params called : timer = 0x%x; notification_type = 0x%x ; timetick = 0x%llx",
  	        timer,
  	        timer_attrib_struct_ptr->notify_type,
  	        ticks);
  
  return TE_SUCCESS;
} /* timer_drv_set_params */


/*=============================================================================

FUNCTION TIMER_DRV_GET_SCLK_TILL_EXPIRY_64

DESCRIPTION
  Returns the number of ticks, in 19MHz domain, until the expiry of
  next timer to expire.

DEPENDENCIES
  This function is provided for the exclusive use of SLEEP, for TCXO shutdown.
  Must be called from an ATS_INTLOCK'd context.

RETURN VALUE
  Number of ticks, in 19MHz domain, until the next timer expiry.

SIDE EFFECTS
  None

=============================================================================*/

time_timetick_type timer_drv_get_sclk_till_expiry_64( void )
{
  /* Current slow clock count */
  time_timetick_type                   ticks_now = 0;
  time_timetick_type                   tt_ret = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ATS_INTLOCK();

  /* Get the current time */
  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_SCLK_TILL_EXPIRY,NULL,ticks_now);

  if ( timers.set_value - timers.set_time > ticks_now - timers.set_time )
  {  
    /* Compute & return the time until the timer is set to expire */
    tt_ret = timers.set_value - ticks_now;
  }
  else
  {  
    /* The next timer interrupt is already pending! */
    tt_ret = 0;
  }

  ATS_INTFREE();

  return tt_ret;
} /* timer_drv_get_sclk_till_expiry_64 */



/*=============================================================================

FUNCTION TIMER_DRV_SET_WAKEUP_64

DESCRIPTION
  This function sets the timer interrupt to expire in advance of the
  the next expiring timer's expiry point, to allow for TCXO to turn back on.
  This function works in the 19MHz domain.

DEPENDENCIES
  This function is provided for the exclusive use of SLEEP, for TCXO shutdown.
  Must be called from an ATS_INTLOCK'd context.

RETURN VALUE
  TE_SUCCESS always currently

SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_set_wakeup_64
(
  /* Wakeup the CPU in the given number of sclk's. */
  time_timetick_type                   wakeup_sclks
)
{
  /* Current slow clock count */
  time_timetick_type                   ticks_now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get the current time2 */
  DalTimetick_GetTimetick64(hTimerHandle, &ticks_now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_SET_WAKEUP,NULL,ticks_now); 
  /* Program the timer */
  timer_set_next_interrupt( ticks_now + wakeup_sclks, ticks_now, FALSE, 
                            TIMER_MVS_SET_WAKEUP, NULL );

  return TE_SUCCESS;

}

/* timer_drv_set_wakeup_64 */

#ifdef FEATURE_TIME_QDSP6
/*=============================================================================

FUNCTION TIMER_PIMUTEX_INIT

DESCRIPTION
  This function initializes the pimutex that will be used for protecting
  the crit-section portion of timer code.

DEPENDENCIES
  This function is applicable only for QURT.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void timer_pimutex_init(void)
  {
  qurt_pimutex_init ( &timer_qemu_mutex );
  }

void timer_trace_mutex_init(void)
  {
  qurt_pimutex_init ( &timer_trace_mutex );
}
void timer_pc_mutex_init(void)
  {
  qurt_pimutex_init ( &timer_pc_mask_mutex );
}
#endif

/*=============================================================================

FUNCTION TIMER_GET_QURT_SIG_REGISTRATION_ID

DESCRIPTION
  This function returns the registration id that will be used to signal the 
  expiry of the first timer.

DEPENDENCIES
  This function is applicable only for QURT.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
int timer_get_qurt_sig_registration_id
(
  void
)
{
  time_timetick_type          now = 0;

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_QURT_SIG_REGISTRATION_ID,NULL,now);
#ifdef FEATURE_TIME_QDSP6
  return timer_qurt_sig_registration_id;
#else
  return TIMER_MAX_EXPIRY; 
#endif
}


/*=============================================================================

FUNCTION TIMER_INIT_QURT_SIGNAL

DESCRIPTION
  This function initializes QURT signal related to timer expiry. When
  the match value == timetick value, QURT will set this signal. 

  This function also initializes the mutex that will be used in crit-section
  portions of timer code.

DEPENDENCIES
  This function is applicable only for QURT.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
#ifdef FEATURE_TIME_QDSP6
static void timer_init_qurt_signal
(
  void
)
{
  #ifdef FEATURE_TIMER_USE_QURT_SYSCLOCK
  time_timetick_type            now = 0;
  #endif

  /*initialize the signal mask that we want to wait on*/
  qurt_anysignal_init (&timer_q6_signal);


  #ifdef FEATURE_TIMER_USE_QURT_SYSCLOCK
  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_INIT_QURT_SIGNAL,NULL,now);

  timer_qurt_sig_registration_id = 
        qurt_sysclock_register (&timer_q6_signal, TIMER_QURT_SIG_MASK);

  if (timer_qurt_sig_registration_id < 0) 
  {
    ERR_FATAL("sclk registration failed, id=0x%i",timer_qurt_sig_registration_id,0,0);
  }

  /*pass on the qurt registration id to the timetick layer*/
  DalTimetick_UpdateBlastSigId(hTimerHandle,(uint32)timer_qurt_sig_registration_id);
  #endif
}
#endif

#ifdef FEATURE_TIMER_USE_QURT_SYSCLOCK
/*=============================================================================

FUNCTION TIMER_UPDATE_ISR_TIME

DESCRIPTION
  This function is called in QURT, when the timer signal is set. In Q6, 
  there is no timer interrupt, but for backward compatibility, the variables
  timer.isr_time, etc. need to be populated.

DEPENDENCIES
  This function is applicable only for QURT.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void timer_update_isr_time
(
  void
)
{
  /* Timer sclk time-stamp values */
  time_timetick_type                   set_time, set_value;

  /* Latency in ISR handling */
  time_timetick_type                   latency_sclk;
  time_timetick_type                   now = 0;

  ATS_INTLOCK( );
  /* Record when timer isr actually was handled */
  DalTimetick_GetTimetick64(hTimerHandle, &now);
  timers.isr_time = now;

  TIMER_API_LOG(TIMER_API_LOG_TIMER_UPDATE_ISR_TIMER,NULL,now);  

  /* Capture timer set values, for messaging outside of ATS_INTLOCK. */
  set_time  = timers.set_time;
  set_value = timers.set_value;

  ATS_INTFREE();


  if ( now - set_time >= set_value - set_time)
  {
    /* Determine the difference between when the interrupt was programmed to
       occur at, and when the interrupt was actually handled. */
    latency_sclk = now - set_value;

    if ( latency_sclk > TIMER_SCLK_LATENCY )
    {
      /* Timer interrupt was handled over btw 5 (60kHz) to 10ms (30kHz) late.
         (Timers may expire much later than 5-10ms, if they are in a disabled
         group, or if multiple timers expire due to the same interrupt.
         This is only a measure of the timer isr latency.) */

      ATS_ERR_3("Late Timer ISR: ST=%d SV=%d IT-SV=%d",
                set_time, set_value, latency_sclk);
    }
  }
  else
  {
#ifndef FEATURE_TIME_QDSP6
    ATS_ERR_3("Early Timer ISR: ST=%d SV=%d SV-IT=%d",
             set_time, set_value, set_value - now);
#endif
  }

  /* Save a trace packet */
  TIMER_TRACE(ISR);
}
#endif /* FEATURE_TIMER_USE_QURT_SYSCLOCK */

/*=============================================================================

FUNCTION TIMER_GET_TIMER_PARAMETER

DESCRIPTION
  Get requested timer parameter value or state.
  param_val2 is not always mandatory.

DEPENDENCIES
  Must be called from ATS_INTLOCKED context.
  
RETURN VALUE

SIDE EFFECTS
  None

=============================================================================*/
timer_error_type timer_get_timer_parameter(
                           timer_ptr_type timer,
                           timer_ptr_type timer_actual_addr,
                           uint32 process_idx,
                           timer_param_type param,
                           uint64 *param_val1, 
                           uint64 *param_val2)
{
  timer_error_type                status = TE_SUCCESS;
  /* Client timer ptr */
  timer_client_ptr_type           clnt_timer = NULL;
  /* Pointer to internal timer */
  timer_ptr_internal_type         timer_int = NULL;
  
  if(param_val1 == NULL)
     return TE_INVALID_PARAMETERS;
    
  *param_val1 = 0;
  if(param_val2 != NULL)
     *param_val2 = 0;	 
	 

  status = timer_get_clnt_int_timers(timer, timer_actual_addr, process_idx, &clnt_timer, &timer_int);
  if(clnt_timer == NULL)
  {
    return status;
  }
  
  
  status = TE_SUCCESS;
  switch(param)
  {
     case TIMER_IS_ACTIVE: 
     {
        if(timer_int && timer_int->list != NULL)
           *param_val1 = (uint64)TRUE;
        else
           *param_val1 = (uint64)FALSE;
        break;
     }
     
     case TIMER_IS_PERIODIC:
     {
        *param_val1 = (uint64)FALSE;
        if(timer_int && timer_int->reload > 0)
        {
            *param_val1 = (uint64)TRUE;
            if(param_val2 != NULL)
               *param_val2 = timer_int->reload;
        }
        break;
     }
     
     case TIMER_EXPIRES_AT_PARAM:
     {
        if(timer_int) 
        {
           *param_val1 = timer_int->expiry;
        }
        break;
     }
     
     case TIMER_GET_START_64:
     {
        if(timer_int) 
        {
           *param_val1 = timer_int->start;
        }
        break;
     }
     
     case TIMER_GET_GROUP:
     {
        if(clnt_timer) 
        {
          *param_val1 = GROUP_INDEX_TO_HANDLE(clnt_timer->info.group_idx);
		}
        break;
     }
     
     case TIMER_GET_EXPIRY_TYPE:
     {
        if(clnt_timer) 
        {	 
          *param_val1 = clnt_timer->info.expiry_type;
		}
        break;
     }
     default: status = TE_INVALID_PARAMETERS;
  }
  
  return status;
} /* timer_get_timer_parameter */

/*=============================================================================

FUNCTION TIMER_DRV_IS_ACTIVE

DESCRIPTION
 This is function which queries whether a timer is active or not.        
 A timer which has been set to expire at a certain time - even if the   
 group it is a member of is disabled - is considered to be active.      
 A timer which is explicitly paused is not active, until it has been    
 resumed.

DEPENDENCIES
  None

RETURN VALUE
  boolean - TRUE if the timer is active.FALSE if the timer is not active.

SIDE EFFECTS
  None

=============================================================================*/
boolean timer_drv_is_active
(
  timer_ptr_type  timer,
  timer_ptr_type  timer_actual_addr,
  uint32          process_idx
)
{
	uint64						   timer_active = (uint64)FALSE;
    time_timetick_type             now = 0;              

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_is_active: timer pointer is NULL",0,0,0);
  }      

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_IS_ACTIVE,timer_actual_addr,now);
   
  (void)timer_get_timer_parameter(timer, timer_actual_addr, process_idx, TIMER_IS_ACTIVE, &timer_active, NULL);

  ATS_INTFREE();
  return (boolean)timer_active;

} /* timer_drv_is_active */

/*=============================================================================

FUNCTION TIMER_DRV_IS_PERIODIC_TIMER

DESCRIPTION
 Returns TRUE if specified timer is a periodic timer.

DEPENDENCIES
  None

RETURN VALUE
  boolean - TRUE if the timer is periodic. FALSE if the timer is not active.

SIDE EFFECTS
  None

=============================================================================*/
timer_error_type timer_drv_is_periodic_timer
(
  timer_ptr_type      timer,
  timer_ptr_type      timer_actual_addr,  
  boolean            *is_periodic,
  time_timetick_type *period,
  uint32              process_idx
)
{
  timer_error_type                status = TE_SUCCESS;
  uint64                          is_periodic_64 = (uint64)FALSE;
  uint64                          is_periodic_temp = 0;
  time_timetick_type              now = 0;
  
  if(is_periodic == NULL)
     return TE_INVALID_PARAMETERS;

  ATS_INTLOCK();
  
  *is_periodic = FALSE;

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_IS_PERIODIC,timer_actual_addr,now);  
  
  status = timer_get_timer_parameter(timer, timer_actual_addr, process_idx, TIMER_IS_PERIODIC, &is_periodic_64, (uint64 *)&is_periodic_temp);
  *is_periodic = (boolean)is_periodic_64;
  
  if(period)
     *period = (time_timetick_type)is_periodic_temp;
 
  ATS_INTFREE();
  return status;
} /* timer_drv_is_periodic_timer */

/*=============================================================================

FUNCTION TIMER_DRV_EXPIRES_AT_64

DESCRIPTION
 This returns the sclk counter value when the timer    
 will expire at. See: DalTimetick_GetTimetick64()  

DEPENDENCIES
  Timer must be running for the returned value to be meaningful.

RETURN VALUE
  time_timetick_type - XO counter value corresponding to timer expiry point.

SIDE EFFECTS
  None

=============================================================================*/
time_timetick_type timer_drv_expires_at_64
(
  timer_ptr_type  timer,
  timer_ptr_type  timer_actual_addr,  
  uint32          process_idx
)
{
  time_timetick_type              expires_in=0;
  time_timetick_type              now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_expires_at_64: timer pointer is NULL",0,0,0);
  }    

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_EXPIRES_AT,timer_actual_addr,now);
 
 (void) timer_get_timer_parameter(timer, timer_actual_addr, process_idx, TIMER_EXPIRES_AT_PARAM, (uint64 *)&expires_in, NULL);

  ATS_INTFREE();
  return expires_in;

  
} /* timer_drv_expires_at_64 */

/*=============================================================================

FUNCTION TIMER_DRV_GET_START_64

DESCRIPTION
 This returns the sclk counter value in the 19MHz domain when the timer    
 was set  

DEPENDENCIES
  Timer must be running for the returned value to be meaningful.

RETURN VALUE
  time_timetick_type - TCXO counter value corresponding to timer start point.

SIDE EFFECTS
  None

=============================================================================*/

time_timetick_type timer_drv_get_start_64
(
  timer_ptr_type  timer,
  timer_ptr_type  timer_actual_addr,
  uint32          process_idx
)
{
  time_timetick_type              timer_start=0;
  time_timetick_type              now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( timer == NULL )
  {
    ERR_FATAL("timer_drv_get_start_64: timer pointer is NULL",0,0,0);
  }      

  ATS_INTLOCK();

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_START,timer_actual_addr,now);
 
  (void) timer_get_timer_parameter(timer, timer_actual_addr, process_idx, TIMER_GET_START_64, (uint64 *)&timer_start, NULL);

  ATS_INTFREE();
  return timer_start;


} /* timer_drv_get_start_64 */

/*=============================================================================

FUNCTION TIMER_DRV_GET_GROUP

DESCRIPTION
 Gets group handle

DEPENDENCIES
 Timer should be existing
 
RETURN VALUE
  Group Handle

SIDE EFFECTS
  None

=============================================================================*/

timer_error_type timer_drv_get_group
(
  timer_ptr_type    timer,
  timer_ptr_type    timer_actual_addr,
  timer_group_type *group,
  uint32            process_idx
)
{
   timer_error_type              status = TE_SUCCESS;
   uint64                        timer_group = 0;
   time_timetick_type            now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if(group == NULL)
     return TE_INVALID_PARAMETERS;

  ATS_INTLOCK();
  
  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  group->group_handle = (unsigned int) 0;
  #else
  *group = (timer_group_type) 0;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_GROUP,timer_actual_addr,now);
  
  status = timer_get_timer_parameter(timer, timer_actual_addr, process_idx, TIMER_GET_GROUP, &timer_group, NULL);
  #if defined (TIMER_SUPPORT_OLD_TIMER_STRUCTURE) || defined (TIMER_HANDLE_AS_STRUCTURE)
  group->group_handle = (unsigned int) timer_group;
  #else
  *group = (timer_group_type) timer_group;
  #endif /* TIMER_SUPPORT_OLD_TIMER_STRUCTURE */

  ATS_INTFREE();
  return status;
} /* timer_drv_get_group */

/* =============================================================================

FUNCTION TIMER_DRV_GET_EXPIRY_TYPE

DESCRIPTION
 Returns the Expiry type of timer

DEPENDENCIES
  Timer must be set atleast once

RETURN VALUE

SIDE EFFECTS
  None

============================================================================= */

timer_error_type timer_drv_get_expiry_type
(
  timer_ptr_type     timer,
  timer_ptr_type     timer_actual_addr,
  timer_expiry_type *expiry_type,
  uint32             process_idx
)
{
   timer_error_type              status = TE_SUCCESS;
   uint64                        expiry_type_64 = (uint64)TIMER_RELATIVE_EXPIRY;
   time_timetick_type            now = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
  if(expiry_type == NULL)
     return TE_INVALID_PARAMETERS;

  ATS_INTLOCK();
  
  *expiry_type = TIMER_RELATIVE_EXPIRY;

  DalTimetick_GetTimetick64(hTimerHandle, &now);

  TIMER_API_LOG(TIMER_API_LOG_TIMER_GET_EXPIRY_TYPE,timer_actual_addr,now);  

  status = timer_get_timer_parameter(timer, timer_actual_addr, process_idx, TIMER_GET_EXPIRY_TYPE, &expiry_type_64, NULL);
  *expiry_type = (timer_expiry_type) expiry_type_64;

  ATS_INTFREE();  
  return status;
} /* timer_drv_get_expiry_type */


/*=============================================================================

FUNCTION TIMER_ACTIVE_LIST_CHECK

DESCRIPTION
  Throws an error fatat if timer list is corrupted

DEPENDENCIES
  FEATURE_TIMER_TRACE and FEATURE_TIMER_LIST_DEBUG should be turned on.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

static void timer_active_list_check(void)
{
  
  #if defined( FEATURE_TIMER_TRACE ) && defined( FEATURE_TIMER_LIST_DEBUG )
  
  /* Pointer to traverse timer list */
  timer_ptr_internal_type          ptimer1, ptimer2;

  /* Number of timers in the list starting from first timer */
  uint32 forward_traversal_count = 0;
  
  /* Number of timers in the list starting from last timer */
  uint32 reverse_traversal_count = 0;

  /* First & last timer respectively in the active list */
  timer_ptr_internal_type          first, last;

  /* timetick for logging this functions entry & exit */
  static time_timetick_type        timer_list_check_entry = 0;
  static time_timetick_type        timer_list_check_last_exit = 0;  

  boolean timer_top_of_list = TRUE;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  DalTimetick_GetTimetick64(hTimerHandle, &timer_list_check_entry);

  /* Return if active list is NULL */
  if ( timers.active.list.first == NULL )
    return;
  
  /* Spin thru the active timer list */
  ptimer1 = timers.active.list.first->next;
  
  if( ptimer1 != NULL )
    ptimer2 = ptimer1->next;
  else
    ptimer2 = NULL;

  /* Checks for Cirular loop in timers' list 
     Start with 2 pointers ( ptimer1  and ptimer2 ). 
     Advance ptimers1 with one node and ptimer2 with two nodes
     At any point if ptimer1 == ptimer2 then you have a loop, 
     otherwise ptimer2 will reach NULL in n/2 iterations. */
  
  while( ptimer1 != NULL && ptimer2 != NULL )
  {
    
    if (ptimer1 == ptimer2 )
    {
      ERR_FATAL(" Timer List Corrupted, ptimer1=0x%x ", ptimer1,0,0 );
    }

    ptimer1 = ptimer1->next;
    ptimer2 = ptimer2->next;

    if ( ptimer2 !=NULL )
    {
      ptimer2 = ptimer2->next; 
    }
    else
    {
      break;
    }
  }
  
  first      = timers.active.list.first;
  last       = timers.active.list.last;

  while( first != NULL )
  {
     /* To catch the timers that have not been expired though the expiry time
        has been crossed. A timer is expired if expiry - start of that timer is
        less than now - start. But if such a timer still exists in timers list,
        it means it been fired and thus need to error fatal */
    if( timers.processing == FALSE &&
		(first->expiry < timer_list_check_entry) )
    {
       if(timer_top_of_list == FALSE)
       {
           ERR_FATAL(" timer active list corrupted due to timers not being expired, timer=0x%x ",first,0,0); 
       }
    }
    else
    {
       /* Starting timers that have crossed expiry are over */
       timer_top_of_list = FALSE;
    }

  /* Check if the links of the doubly linked list are proper starting from the
     first timer and also count the number of timer */
    if( first->next != NULL )
    {
      if( first != first->next->prev )
      {
         ERR_FATAL(" timer active list corrupted, ptr1=0x%x ptr1->next->prev=0x%x",first,first->next->prev,0); 
      }

      /* Checking for expiries of the timers to be in sorted order for some corruption issues */
      if(first->expiry > first->next->expiry)
      {
         ERR_FATAL("Timer active list corrupted due to small timer being inserted after large timer, timer=0x%x",first,0,0); 
      }
    }
    first = first->next;
    forward_traversal_count++;
  }

  /* Count the number of timers in the list starting from last timer */
  while( last != NULL )
  {
    last = last->prev;
    reverse_traversal_count++;
  }

  /* List is corrupted if first_num is not same as last_num or is 
     not same as actual number of timers supposed to be present 
     at given point of time */

  if (forward_traversal_count != reverse_traversal_count)
    ERR_FATAL(" timer active list corrupted, fwd_traversal=0x%x reverse_traversal=0x%x",
              forward_traversal_count,reverse_traversal_count,0 );

  timer_list_check_last_exit = timer_list_check_entry ;

  #endif /* FEATURE_TIMER_TRACE && FEATURE_TIMER_LIST_DEBUG */

} /*  timer_active_list_check */

/*=============================================================================

FUNCTION TIMER_GET_TIMER_FREE_Q_CNT

DESCRIPTION
 This returns the timers allocated 
 in timer_buffer

DEPENDENCIES
  None

RETURN VALUE
  uint32 number of allocated timers in timer_buffer

SIDE EFFECTS
  None

=============================================================================*/
uint32 timer_get_timer_free_q_cnt
(
  void
)
{
  return timer_count_buffer_allocated;
}

/*=============================================================================

FUNCTION TIMER_POST_INIT

DESCRIPTION
 This function is called from time_init to pass timetick handle to timer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void timer_post_init(void)
{
  /*at this point, the dog task has started and hence we can register the slave tasks with
   the dog*/
  #ifdef FEATURE_DOG
  #ifdef FEATURE_TIMER_SLAVE1
  timer_slave_register_dog(TIMER_SLAVE_TASK1);
  #endif

  #ifdef FEATURE_TIMER_SLAVE2
  timer_slave_register_dog(TIMER_SLAVE_TASK2);
  #endif

  #ifdef FEATURE_TIMER_SLAVE3
  timer_slave_register_dog(TIMER_SLAVE_TASK3);
  #endif
  #endif /* FEATURE_DOG */

  timer_register_npa_client();
}

/*=============================================================================

FUNCTION TIMER_DRV_REGISTER_FOR_NON_DEFER_EXPIRY_CHANGE

DESCRIPTION
Registers for first non-deferrable timer expiry change notification.
Only signal type notifications are allowed, for others, error will be 
returned.

DEPENDENCIES
  Input objects should be initialized by client

RETURN VALUE
TE_INVALID_PARAMETERS   If passed sig obj addr is NULL or invalid notify type
TE_SUCCESS :            If the operation completed successfully

SIDE EFFECTS
  None

=============================================================================*/
timer_error_type timer_drv_register_for_non_defer_expiry_change
(
  timer_notify_type        notify_type,
  time_osal_notify_obj_ptr notify_obj,
  time_osal_notify_data    notify_mask,
  uint32                   process_idx
)
{
   int idx;
   
   if(!timer_is_notify_type_valid(notify_type))
      return TE_INVALID_PARAMETERS;
   
   /* This registration is allowed only for signal type notifications */
   if(notify_type == TIMER_FUNC1_CB_TYPE || 
      notify_type == TIMER_FUNC2_CB_TYPE ||
      notify_type == TIMER_SIGS_FUNC_DUAL_NOTIFY ||      
      notify_type == TIMER_NO_NOTIFY_TYPE)   
      return TE_INVALID_PARAMETERS;

   if(notify_obj == NULL)   
      return TE_INVALID_PARAMETERS;
      
   ATS_INTLOCK();
   
   if(timer_non_defer_expiry_notify_clients_cnt == TIMER_NON_DEFER_EXPIRY_NOTIFY_CLIENTS_MAX_CNT)
   {
      ATS_INTFREE();
      return TE_FAIL;
   }

   idx = timer_non_defer_expiry_notify_clients_cnt++;
   timer_non_defer_expiry_notify_clients[idx].notify_type = notify_type;
   timer_non_defer_expiry_notify_clients[idx].notify_obj = notify_obj;
   timer_non_defer_expiry_notify_clients[idx].notify_mask = notify_mask;
   timer_non_defer_expiry_notify_clients[idx].process_idx = process_idx;
   
   ATS_INTFREE();
   
   return TE_SUCCESS;  
} /* timer_drv_register_for_non_defer_expiry_change */

