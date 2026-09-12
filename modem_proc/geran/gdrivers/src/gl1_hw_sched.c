/*===========================================================================

        IMPLEMENTATION OF FRAME LAYER SCHEDULER FUNCTIONS

GENERAL DESCRIPTION
   This file implements the GSM/GPRS frame layer scheduler responsible for
   scheduling power measures, checking for conflicts, and validating time
   tracking.

EXTERNALIZED FUNCTIONS
   gl1_hw_schedule_init
   gl1_hw_schedule_frame_tick
   gl1_hw_schedule_tt_ok
   gl1_hw_schedule_rx
   gl1_hw_schedule_tx
   gl1_hw_schedule_int_meas
   gl1_hw_schedule_pwr_meas
   gl1_hw_schedule_async_rx
   gl1_hw_schedule_sync_rx
   gl1_hw_schedule_sync_tx
   gl1_hw_schedule_start_acq
   gl1_hw_schedule_stop_acq
   gl1_hw_schedule_rf_sample_ram


INITIALIZATION AND SEQUENCING REQUIREMENTS
   The following is the desired order in which functions should be called

   gl1_hw_schedule_init
   gl1_hw_schedule_frame_tick


Copyright (c) 2005 - 2015 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_sched.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when        who     what, where, why
--------    ---     --------------------------------------------------------
11/05/17   rc        CR2042018:Ensure to send StartGSM Mode CNF after resume init.
25/07/17    og      CR2047915. Fix HMSC power meas scheduling.
09/04/15    dp      CR819956 Thor 2.x RF API change, G2W use MSGR
27/02/15    ab      CR750511: GSM Timeline reporting for WLAN COEX
27/01/15    npt     CR697236 Mainline FEATURE_GERAN_VSTMR
17/11/14    dp      CR700052 Move g2w drivers into W filetree
18/09/14    ws      CR 726622 Compile wartning fix for unitialised variable
12/09/14    nm      CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
05/09/14    ws      CR 718881 Removing unused rfgsm_core_types.h
06/08/14    jj      CR 705509 reducing scell meaurement when BA list is empty in dedicated mode
14/04/14    pjr     CR641577 Replaced FN with Seq Number in cxm log table
04/08/14    mc      CR703283 Insane no-zone calculation triggers gfw_burst_scheduler.cc:801
                    Assertion (0 <= burstRxTxInfo.lastRx && burstRxTxInfo.lastRx <= TS7) failed
28/07/14    pjr     CR700772 Changed mdsp_notify_power_mon_info to support two no_zones
11/07/14    dp      CR674070 - Use new G2W drivers interface (phase 1)
14/07/14    cs      CR693967 Get DR-DSDS mode during startup
19/06/14    ws      DATA+MMS feature - command FW to schedule monitors if GL1 can't find a space
22/05/14    cah     CR651795 Bolt 2.0: FEATURE_GERAN_DRDSDS
05/05/14    npt     CR656269 Do not postpone power measurements on first wake-up frame
04/04/14    cah     CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
05/03/14    mc      CR.583282 : Limit the number of PSCAN monitors per frame to that which can be COEX protected in X+G DSDA
10/02/14    br      CR612488 increment the FREE EVENTS properly while decremnting null events.
03/02/14    br      CR610693: Handle scheduler when NULL event is added.
13/12/13    mc      CR.583042 : COEX : DSDA CS + PS - PS call drops in RxTx overlap scenario
06/12/13    sp      CR585446: Skip decreasing time offsets at gl1_hw_schedule_frame_tick if num_events is 0.
10/12/13    ws      CR 587586 - Remove DUAL_SIM featurisation in API
04/11/13    mc      CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
28/11/13    cja     CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13    cja     CR455527 Remove featurisation from files in API directory
23/10/13   sp       CR563626: Disable acq_in_progress in next frame after Stop_Acq is issued
14/10/13    pa      CR539505:Bolt: Added support for Dynamic Power Scheduling
11/18/13    pjr     CR563534 Fix incorrect channel type shown in CXM logs packets
17/09/13    pjr     CR538893 Changes to enable CXM logging
21/08/13    npt     CR531633 - Remove FEATURE_DIME_MODEM dependencies
15/08/13    sk      CR524039 GBTA bringup changes
14/08/13    npt     CR528491 - Use mod4096 instead of mode4 for interactions with FW
29/07/13    sk      CR519663 Partial QBTA bringup changes
24/04/13    pg      CR459509: Don't schedule pwr meas after crossing frame boundary
05/04/13    mc      CR470155 : COEX Support - GERAN arbitrator framework - PS integration
13/03/13    mc      CR459487 : COEX Support - GERAN arbitrator framework
29/10/12    cja     CR403253 Modify test for power measure in same FW timeslot.
13/09/12    cja     CR386695 Use RFI API for Rx alpha
07/09/12    cja     CR388608 Complete fix for CR365347.
04/09/12    og      CR395649. T2G Acquisition and BSIC decode enhancements.
16/08/12    zf      CR390228: Featurizing Opt19.
06/07/12    jj      CR375928 Avoid HMSC pwr meas schedule beyond 5000 QS
03/07/12    zf      CR341842: Opt19: Increase power monitor number to 5 per PCH frame.
29/06/12    jj      CR374950 GL1 changes for 38 symbol HMSC monitor
11/06/12    dv      CR369356 Crash in G2T due to bug in scheduler
30/05/12    cja     CR365748 Resume power measurements on g2x clean-up
29/05/12    og      CR363662. Take the async interrupt into account when booking x2g
                    events.
29/05/12    cja     CR365347 Allow 1 slot margin after stop acq at start of frame
03/05/12    og      Ensure the X2G gap event conflict is only checked against
                    similar events. CR358363.
12/04/12    og      Add gl1_hw_schedule_remove_x2g_startup( ).
03/04/12    ws      CR348574 Fix compile error for targets not supporting TDS
02/04/12    og      CR348574. Update the gl1_hw_schedule[gas_id] to support X2G IRAT.
26/03/12    jj      CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
10/02/12    ws      CR335103 - add dummy var to allow black list to detect HMSC
10/02/12    ws      CR335103 - Reset medium access mode to Fixed allocation when leaving
                    idle mode. Avoids false HMSC mode detects in dedicated mode.
23/02/12    jj      CR338861 HMSC error when scheduling power measures during PDP activation testing
15/02/12    jj      CR337139  correction & optimisation in HMSC monitor schedule
01/02/12    cja     CR333103 Ensure max 1 monitor per timeslot.
27/01/12    jj      HMSC power monitor
20/10/11    cja     CR313887 Correct G2W internal scheduler times
10/10/11    cja     CR313006 Check time before scheduling pwr measure to prevent ISR overrun.
18/09/11    cja     CR307155  Replace RF timing definitions with RF API
16/08/11    jj      CR299789 Reduced the pwr meas length to 48 symbol
09/05/11    cja     Nikel add RF API for timing info
28/04/11    pg      Fix compiler warnings
28/02/11    pg      Avoid generating false scheduling conflict errors in HMSC
23/02/10    pg      Updates for NCell FCCH when HMSC enabled
14/02/11    cja     CR273625 Revert HMSC Rx alpha to old value
24/11/10    cja     Integrate HMSC
04/10/10    cja     CR258464. Correct selection of DSP scheduled pwr meas.
15/09/10    cja     CR255616. Ensure power measure start offset is in current frame
13/09/10    cja     Add debug for CR251982 and CR251985
01/09/10    cja     Correct merge of CR248465 with CR250867.
26/08/10    cja     CR248465 Only have DSP schedule monitor if GL1 can not schedule it
23/08/10    cja     CR250867. Add extra guard period to pwr meas scheduling for BB Rx clock gating
05/08/09    ws      Removed FEATURE_GSM_MDSP_DTM it's now mainlined
18/01/09    cs      Fix the conflict debug printing for Mustang
24/11/08    og      Let the fw only schedule pwr monitors on Tra and only when
                    five transmits are requested by host.
09/10/08    cs      Allow the RX Sweep to schedule shorter monitor duration
11/09/08    cs      Reduced the guard period for RX Sweep to 10 as no complex setups
10/09/08    agv     CR156708 - If a Async SCH Rx is in progress, don't allow
                    Power Monitor to conflict.
28/08/08    cs      Implement the Fast Rx Sweep functionality for FTM
05/06/08    ps      Lint cleanup
19/07/07    og      UOOS changes, reduce number of measured samples.CR119147.
16/03/06    og      Take timeslot gaps into account when deciding on Tta/Trb
                    or Ttb/Tra. Resolves CR 88935 and CR 88975.
02/06/06    agv     Changed GL1_HW_SCHEDULE_GUARD_PERIOD for CR85687 and 84522
10/12/05    gfr     Make gl1_hw_schedule_free_events static and add sanity check
                    Latch the mac mode on the frame tick
09/22/05    gfr     Removed FEATURE_SBI_MDSP_CONTROLLER
09/07/05    gfr     Only use mDSP scheduled pwr meas when we have 4 tx and at
                    least one receive, all other cases we can schedule
08/26/05    gfr     Use the correct offset for mDSP scheduled power measures
                    Use mDSP scheduled pwr meas whenever num rx + num tx > 5
08/19/05    gfr     QLint cleanup
07/29/05    gfr     DTM support
07/27/05    gfr     Increase guard period to compensate for running the mDSP
                    at 61 MHz instead of 100 MHz.
06/27/05    gfr     Do not schedule power measures that stop on the start
                    offset of the next event
06/21/05    gfr     Initial revision pulled from gl1_hw_gprs.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hwi.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "vtmrs_g.h"
#include "l1_drx.h"
#include "l1_sc.h"

#include "msg.h"
#include "stdio.h" /* for sprintf */
#include "gl1_msg_pdch.h"
#include "gl1_hw_arbitration.h"
#include "l1_log.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gpl1_dual_sim.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_G2W_DRIVER_IN_WL1
#include "wl1x2wsrchapi.h"
#else
extern uint32 g2w_srch_startup_time[];
extern uint32 g2w_srch_cleanup_time[];
extern uint16 g2w_srch_pll_settling_time[];
#endif

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/
/* This defines the maximum number of outstanding events */
#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
#define GL1_HW_SCHEDULE_EVENT_Q_SIZE  (17U)
#else
#ifdef FEATURE_GSM_DYNMC_PWR_MEAS
/* Max 12 (MAX_MEAS_PER_FRAME) pwr measurements events can be created in one frame,
   if there are no other events schedule in the same frame.*/
#define GL1_HW_SCHEDULE_EVENT_Q_SIZE  (12U)
#else
#define GL1_HW_SCHEDULE_EVENT_Q_SIZE  (9U)
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
#endif

#define MINIMUM_FRAME_TICK_TO_SCHEDULE_HMSC 500
/* Type of activity scheduled */
typedef enum
{
   GL1_HW_SCHED_TX,
   GL1_HW_SCHED_RX,
   GL1_HW_SCHED_ASYNC_RX,
   GL1_HW_SCHED_ACQ,
   GL1_HW_SCHED_INT_MEAS,
   GL1_HW_SCHED_PWR_MEAS,
   GL1_HW_SCHED_FRAME_DELAY
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || \
    defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
   ,GL1_HW_SCHED_G2W_GAP
#ifdef FEATURE_GSM_TDS
   ,GL1_HW_SCHED_G2X_GAP
#endif
#endif
#ifdef FEATURE_GSM_TDS
   ,GL1_HW_SCHED_X2G_GAP
#endif
   ,GL1_HW_SCHED_MAX
} gl1_hw_schedule_type;


static const char *gl1_hw_schedule_type_to_str[GL1_HW_SCHED_MAX] =
{
   "TX", "RX",
   "ASYNC RX", "ACQ",
   "INT MEAS", "PWR MEAS", "FRAME_DELAY"
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || \
    defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
   , "WCDMA"
#ifdef FEATURE_GSM_TDS
   , "G2X GAP"
#endif
#endif
#ifdef FEATURE_GSM_TDS
   ,"X2G GAP"
#endif
};


/* Structure containing information about a single event */
typedef struct
{
   gl1_hw_schedule_type  type;
   int16                 sub_type;
   int32                 start_offset;
   int32                 stop_offset;
} gl1_hw_schedule_event;

/* Main structure containing all the schedule information, it works like this:
   "event_store" is the actual data structures for the events, and "events"
   is an array of pointers sorted in order of start offset of currently
   active events (rx, tx...).  "free_events" is an array of pointers to
   unallocated events.  When a new event is created a pointer to a free
   event structure is pulled from the "free_events" array and inserted
   into the "events" array based on its start offset.  A linked list is
   not used as the number of active events is always small so the overhead
   in list management versus just shifting relevant array entries is not
   worth it. */
typedef struct
{
   uint32                 timestamp;
   uint32                 current_FN;
   gl1_hw_schedule_event  event_store[GL1_HW_SCHEDULE_EVENT_Q_SIZE];
   gl1_hw_schedule_event  *events[GL1_HW_SCHEDULE_EVENT_Q_SIZE];
   gl1_hw_schedule_event  *free_events[GL1_HW_SCHEDULE_EVENT_Q_SIZE];
   uint32                 num_events;
   uint32                 num_free_events;
   uint32                 max_pwr_meas;
   boolean                allow_late_pwr_meas;
   gl1_defs_access_mode_type mac_mode;  /* Store MAC mode */
   gl1_defs_access_mode_type pending_mac_mode;
   uint32                    num_rx;    /* Keep count for EDA */
   uint32                    num_tx;    /* Keep count for EDA */
   boolean                conflict;     /* Flag if a conflict occurred */
   boolean                acq_in_progress;
   int32                  acq_start_offset;
   boolean                async_rx_in_progress; /* Conflict check */

#ifdef FEATURE_GSM_TDS
   boolean                x2g_in_progress;
   int32                  x2g_gap_start_offset;
#endif

} gl1_hw_schedule_struct;


/* Stored schedule, will be reset in gl1_msg_init */
static gl1_hw_schedule_struct gl1_hw_schedule[NUM_GERAN_DATA_SPACES];

extern gl1_hw_arbitration_t gl1_hw_arbitration[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                             LOCAL MACROS

===========================================================================*/

/* Keep events around that fall in the last slot of the previous frame */
#define GL1_HW_SCHEDULE_MIN_STORED_OFFSET (-QS_PER_SLOT)

/* The guard period to leave between scheduled activity in qs */
/* AGV REM changed 40 to 60 to fix CR85687 and 84522 */
/* CJA Reduced to 56 to allow BB Rx clock gating (CR250867)*/
#define GL1_HW_SCHEDULE_GUARD_PERIOD 56

/* These should be in the rf driver, but are not */
#define RX_BURST_LENGTH_QS (150 * 4)
#define INT_MEAS_LENGTH_QS (MDSP_INT_MEAS_LEN * 4)
#define PWR_MEAS_LENGTH_QS (MDSP_PWR_MEAS_LEN * 4)
/* Definition for power measurement lengths, reduced for HMSC */
#define PWR_MEAS_REDUCED_LENGTH_QS  (MDSP_PWR_MEAS_REDUCED_LEN * 4)

/* Guard period required between end of monitor SBI and start of Tx */
#define PWR_MEAS_END_GUARD_PERIOD   20

/* Time allowed for power measure scheduling (in qs) */
#define PWR_MEAS_SCHEDULE_PROCESS_TIME_MAX   2500

/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/


/*===========================================================================

                                FUNCTIONS

===========================================================================*/


/*=============================================================================
FUNCTION gl1_hw_schedule_reset_events

DESCRIPTION
   Reset the schedule check store data.  Should be static, but FTM is
   calling it.

============================================================================= */
void gl1_hw_schedule_reset_events (gas_id_t gas_id)
{
   uint32 i;

   /* Create the free event queue */
   for (i = 0; i < GL1_HW_SCHEDULE_EVENT_Q_SIZE; i++)
   {
      gl1_hw_schedule[gas_id].events[i] = NULL;
      gl1_hw_schedule[gas_id].free_events[i] = &gl1_hw_schedule[gas_id].event_store[i];
   }

   gl1_hw_schedule[gas_id].num_events = 0;
   gl1_hw_schedule[gas_id].num_free_events = GL1_HW_SCHEDULE_EVENT_Q_SIZE;
   gl1_hw_schedule[gas_id].async_rx_in_progress = FALSE;
}



/*=============================================================================
FUNCTION gl1_hw_schedule_init

DESCRIPTION
   Init all scheduler data, should only be called from gl1_msg_init or
   the mac mode could potentially get out of sync.

============================================================================= */
void gl1_hw_schedule_init (gas_id_t gas_id)
{
   /* Reset events */
   gl1_hw_schedule_reset_events(gas_id);

   /* Set mac mode to the default */
   gl1_hw_schedule[gas_id].pending_mac_mode = GL1_DEFS_FIXED_ALLOC;

   /* We better not start out with an acq in progress! */
   gl1_hw_schedule[gas_id].acq_in_progress = FALSE;

   /* Initially set Ncell acq in progress to FALSE. */
   gl1_hw_schedule[gas_id].async_rx_in_progress = FALSE;

}

/*=============================================================================
FUNCTION gl1_hw_schedule_set_medium_access_mode

DESCRIPTION
   Informs the scheduler of the current medium access mode.  The scheduler
   latches this value on the frame tick, so the normal usage would be to call
   this function in frame 2 of the block (i.e. frame that decode flag is set)
   so that it applies at the start of the next block.

============================================================================= */
void gl1_hw_schedule_set_medium_access_mode
(
   gl1_defs_access_mode_type mode,
   gas_id_t                  gas_id
)
{
   gl1_hw_schedule[gas_id].pending_mac_mode = mode;
}

/*=============================================================================
FUNCTION gl1_hw_schedule_free_events

DESCRIPTION
   This function removes the given number of events from the start of the
   list and and puts them back onto the free event list.

============================================================================= */
static void gl1_hw_schedule_free_events (uint32 count,gas_id_t gas_id)
{
   uint32 i;
   /* Needed to make sure the Async is done. */
   gl1_hw_schedule_event *event;


   /* Sanity check */
   if (gl1_hw_schedule[gas_id].num_events < count)
   {
      MSG_GERAN_ERROR_2_G("Cannot free %d events (num=%d)!",count,gl1_hw_schedule[gas_id].num_events);
      return;
   }

   /* Add to free queue */
   for (i = 0; i < count; i++)
   {
      /* Return the event so we know to Reset the Async Rx. */
      event = gl1_hw_schedule[gas_id].free_events[gl1_hw_schedule[gas_id].num_free_events++] =
        gl1_hw_schedule[gas_id].events[i];

      /* If the Ncell Rx is set; clear it here for next Async scheduling event. */
      if (gl1_hw_schedule[gas_id].async_rx_in_progress && event->type == GL1_HW_SCHED_ASYNC_RX)
      {
        gl1_hw_schedule[gas_id].async_rx_in_progress = FALSE;
      }
   }

   /* Remove from event queue */
   for (i = 0; i < gl1_hw_schedule[gas_id].num_events - count; i++)
   {
      gl1_hw_schedule[gas_id].events[i] = gl1_hw_schedule[gas_id].events[i + count];
   }
   gl1_hw_schedule[gas_id].num_events -= count;
}



/*=============================================================================
FUNCTION gl1_hw_schedule_frame_tick

DESCRIPTION
   Advance the store schedule info by a frame

============================================================================= */
void gl1_hw_schedule_frame_tick (uint32 timestamp,gas_id_t gas_id)
{
   int32 i, min_stored_offset = GL1_HW_SCHEDULE_MIN_STORED_OFFSET;

   /* If the timestamp is not sequential then reset */
   if (timestamp != gl1_hw_schedule[gas_id].timestamp + 1)
   {
      gl1_hw_schedule_reset_events(gas_id);
   }
   else
   {
      /* First reduce any pending acquisition */
      if (gl1_hw_schedule[gas_id].acq_in_progress)
      {
         gl1_hw_schedule[gas_id].acq_start_offset -= QS_PER_FRAME;
         min_stored_offset = gl1_hw_schedule[gas_id].acq_start_offset;
      }

      /* Now go through active events and reduce offsets by a frame, once
         we find the first stale event we discard it and all previous
         events.  This assumes that the events are ordered by stop offset,
         which will be true as long as there have not been conflicts. */
      if (gl1_hw_schedule[gas_id].num_events)
      {
         for (i = (int32)gl1_hw_schedule[gas_id].num_events - 1; i >= 0; i--)
         {

            if( gl1_hw_schedule[gas_id].events[i] == NULL )
            {
               /* Reset events */
              gl1_hw_schedule_reset_events(gas_id);
              MSG_GERAN_ERROR_0_G("Events ptr NULL");
              break;
            }

            gl1_hw_schedule[gas_id].events[i]->start_offset -= QS_PER_FRAME;
            gl1_hw_schedule[gas_id].events[i]->stop_offset -= QS_PER_FRAME;

            /* If this is the first event that is stale, we are done */
            if (gl1_hw_schedule[gas_id].events[i]->stop_offset < min_stored_offset)
            {
               gl1_hw_schedule_free_events((uint32)(i + 1),gas_id);
               break;
            }
         }
       }
   }

   gl1_hw_schedule[gas_id].mac_mode = gl1_hw_schedule[gas_id].pending_mac_mode;
   gl1_hw_schedule[gas_id].max_pwr_meas = MAX_MEAS_PER_FRAME;
   gl1_hw_schedule[gas_id].allow_late_pwr_meas = FALSE;
   gl1_hw_schedule[gas_id].timestamp = timestamp;
   gl1_hw_schedule[gas_id].current_FN = GSTMR_GET_FN_GERAN(gas_id);
   gl1_hw_schedule[gas_id].num_rx = 0;
   gl1_hw_schedule[gas_id].num_tx = 0;
   gl1_hw_schedule[gas_id].conflict = FALSE;
}



/*=============================================================================
FUNCTION gl1_hw_schedule_check_conflict

DESCRIPTION
   Display a schedule conflict error message.

============================================================================= */
static void gl1_hw_schedule_check_conflict
(
   gl1_hw_schedule_event *new_event,
   gl1_hw_schedule_event *old_event,
   gas_id_t               gas_id
)
{
   if ( !new_event || !old_event )
   {
     MSG_GERAN_ERROR_3_G( "Null schedule conflict check new_event:0x%x old_event:0x%x FN:%d",
                new_event, old_event, GSTMR_GET_FN_GERAN(gas_id) );

     return;
   }

   /* Check if the events overlap */
   if (new_event->stop_offset < old_event->start_offset ||
       new_event->start_offset > old_event->stop_offset)
   {
      /* No overlap - normal case */
      return;
   }

   /* In extended dynamic allocation receives can overlap with transmits -
      the mdsp will cancel receives.  Note that receives are scheduled before
      txs in a given frame.  If the new event is a tx and conflicts with an rx
      and the start of the receive is before the start of the tx we are ok. */
   if (gl1_hw_schedule[gas_id].mac_mode == GL1_DEFS_EXT_DYNAMIC_ALLOC &&
       old_event->type == GL1_HW_SCHED_RX &&
       new_event->type == GL1_HW_SCHED_TX &&
       old_event->start_offset < new_event->start_offset)
   {
      return;
   }

   /* SCH can conflict with PDTCHU, the MDSP will cancel the PDTCHU as
      long as the PDTCHU lies before or during the SCH burst */
   if ((old_event->type == GL1_HW_SCHED_ASYNC_RX &&
        old_event->sub_type == (int16)GL1_HW_SCH &&
        new_event->type == GL1_HW_SCHED_TX) ||
       (new_event->type == GL1_HW_SCHED_ASYNC_RX &&
        new_event->sub_type == (int16)GL1_HW_SCH &&
        old_event->type == GL1_HW_SCHED_TX))
   {
      MSG_GERAN_HIGH_1_G("Transmit discarded for SCH burst FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   /* If we schedule an acq that conflicts with a transmit the mdsp
      will discard the transmit */
   if (new_event->type == GL1_HW_SCHED_ACQ &&
       old_event->type == GL1_HW_SCHED_TX)
   {
      MSG_GERAN_HIGH_1_G("Transmit discarded for ACQ FN=%d", GSTMR_GET_FN_GERAN(gas_id));
      return;
   }
#ifdef FEATURE_GSM_TDS
   if( ( new_event->type == GL1_HW_SCHED_X2G_GAP &&
        old_event->type != GL1_HW_SCHED_X2G_GAP
     )
     ||( old_event->type == GL1_HW_SCHED_X2G_GAP &&
        new_event->type != GL1_HW_SCHED_X2G_GAP
     )
   )
   {
      return;
   }
#endif // FEATURE_GSM_TDS

   /* In HMSC mode, we get false scheduling clashes detected between power measurements scheduled in TS7
      and the subsequent Rx in TS0. In actuality there is no scheduling issue, so detect this specific
      clash type and avoid the unnecessary error messages */
   if (new_event->type == GL1_HW_SCHED_RX &&
     old_event->type == GL1_HW_SCHED_PWR_MEAS && old_event->sub_type == 1)
   {
      return;
   }

   /* Flag a conflict */
   gl1_hw_schedule[gas_id].conflict = TRUE;

   /* Prepare the conflict report, leave params for MSG_ERROR as it's faster */
   /* Use MSG_SPRINTF so that err_msg is explicitly copied */
   MSG_SPRINTF_4( MSG_SSID_GSM_L1, MSG_LEGACY_ERROR,
                  "Conflict: %s (%d) [%d:%d] vs",
                  gl1_hw_schedule_type_to_str[new_event->type],
                  new_event->sub_type, new_event->start_offset, new_event->stop_offset );

   MSG_SPRINTF_6( MSG_SSID_GSM_L1, MSG_LEGACY_ERROR,
                  "          %s (%d) [%d:%d] Total events:%d FN:%d",
                  gl1_hw_schedule_type_to_str[old_event->type],
                  old_event->sub_type, old_event->start_offset, old_event->stop_offset,
                  gl1_hw_schedule[gas_id].num_events, GSTMR_GET_FN_GERAN(gas_id) );
}



/*=============================================================================
FUNCTION gl1_hw_schedule_add_event

DESCRIPTION
   Adds an event to the list of current events, also checks for conflicts if
   required.

============================================================================= */
static void gl1_hw_schedule_add_event (gl1_hw_schedule_event *new_event,gas_id_t gas_id)
{
   gl1_hw_schedule_event *event;
   int32 i, insertion_index = 0;

   /* Go through list and find offset */
   for (i = 0; i < (int32)gl1_hw_schedule[gas_id].num_events; i++)
   {
      event = gl1_hw_schedule[gas_id].events[i];

      if ( !event )
      {
         MSG_GERAN_ERROR_2_G( "Null schedule add event number:%d FN:%d",
                    i, GSTMR_GET_FN_GERAN(gas_id));

         gl1_hw_schedule[gas_id].num_events--;
     gl1_hw_schedule[gas_id].num_free_events++;
         if(gl1_hw_schedule[gas_id].num_free_events > GL1_HW_SCHEDULE_EVENT_Q_SIZE)
         {
           gl1_hw_schedule[gas_id].num_free_events = GL1_HW_SCHEDULE_EVENT_Q_SIZE;
         }
         return;
      }

      /* Check for conflict */
      gl1_hw_schedule_check_conflict(new_event, event,gas_id);

#ifdef FEATURE_GSM_TDS
    if( ( new_event->type == GL1_HW_SCHED_X2G_GAP )
      &&( gl1_hw_schedule[gas_id].conflict )
    )
    {
      /* don't book it */
        return;
    }
#endif

      /* If the new event came before the current event, we are done */
      if (new_event->start_offset < event->start_offset) break;
   }
   insertion_index = i;

   /* Add the event */
   for (i = (int32)gl1_hw_schedule[gas_id].num_events; i > insertion_index; i--)
   {
      gl1_hw_schedule[gas_id].events[i] = gl1_hw_schedule[gas_id].events[i - 1];
   }
   gl1_hw_schedule[gas_id].events[insertion_index] = new_event;
   gl1_hw_schedule[gas_id].num_events++;
}



/*=============================================================================
FUNCTION gl1_hw_schedule_create_event

DESCRIPTION
   Handles allocating an event, filling in the fields, and adding it to the
   list.

============================================================================= */
static void gl1_hw_schedule_create_event
(
   gl1_hw_schedule_type type,
   int16 sub_type,
   int32 start_offset,
   int32 stop_offset,
   gas_id_t gas_id
)
{
   gl1_hw_schedule_event *event;

   /* Check if any events are free */
   if (gl1_hw_schedule[gas_id].num_free_events == 0)
   {
      MSG_GERAN_ERROR_0_G("GSM HW Scheduler: out of events!");
      return;
   }

   /* Get a free event structure */
   event = gl1_hw_schedule[gas_id].free_events[--gl1_hw_schedule[gas_id].num_free_events];

   /* Fill in the event information and add it to the list */
   event->type = type;
   event->sub_type = sub_type;
   event->start_offset = start_offset;
   event->stop_offset = stop_offset;

#ifdef DEBUG_GSM_SCHEDULER
   {
      MSG_GERAN_MED_3_G("Schedule add_event, start_offset %d, stop_offset %d type %d",
                         start_offset,stop_offset,type);
   }
#endif
   gl1_hw_schedule_add_event(event,gas_id);
}



/*=============================================================================
FUNCTION calc_frame_delay_offsets

DESCRIPTION
   Calculates the start and stop offsets for a frame delay event.

============================================================================= */
static void calc_frame_delay_offsets
(
   int16 delay,
   int16 *start_offset,
   int16 *stop_offset,
   gas_id_t gas_id
)
{
   /* If the delay is positive then we are lengthening the frame, in which
      case conflicts will only occur if another event stradles the tick.
      We always use the end of a normal frame as the stop offset since no
      event could be scheduled in the extended frame area. */
   if (delay > 0) delay = 0;

   /* Delay is negative, and must be adjusted by the setup margin since we
      store offets relative to timeslot 0, but the adjustment will occur
      relative to the real gstmr tick. */
   *start_offset = (delay - FRAME_TICK_SETUP_MARGIN_QS);
   *stop_offset = (-FRAME_TICK_SETUP_MARGIN_QS);

   /* If the current_FN and GSTMR FN are not the same it means that this
      was called before gl1_hw_schedule_frame_tick.  So the stored event
      offsets are off by a frame.  Add a frame to compensate */
   if (gl1_hw_schedule[gas_id].current_FN != GSTMR_GET_FN_GERAN(gas_id))
   {
      *start_offset += QS_PER_FRAME;
      *stop_offset += QS_PER_FRAME;
   }
}



/*=============================================================================
FUNCTION gl1_hw_schedule_frame_delay

DESCRIPTION
   Fill in the delay with an event.
   delay: offset in QS of how much we are advancing or delaying the GSTMR

============================================================================= */
void gl1_hw_schedule_frame_delay
(
   int16 delay,
   gas_id_t gas_id
)
{
   int16 start_offset;
   int16 stop_offset;
   uint32 i;
   gl1_hw_schedule_event *event;

   /* Calculate the offsets */
   calc_frame_delay_offsets(delay, &start_offset, &stop_offset,gas_id);

   /* Check if a frame delay already exists in this frame, if we find one we
      just need to modify the start offset instead of adding a new event. */
   for (i = 0; i < gl1_hw_schedule[gas_id].num_events; i++)
   {
      event = gl1_hw_schedule[gas_id].events[i];

      /* Frame delay events in the same frame have the same stop offset */
      if (event->type == GL1_HW_SCHED_FRAME_DELAY &&
          event->stop_offset == stop_offset)
      {
         /* Calculate new start_offset, ensuring start is still shorter than
            the stop offset - important since positive delays are treated
            as zero length. */
         event->start_offset += delay;
         if (event->start_offset > event->stop_offset)
         {
            event->start_offset = event->stop_offset;
         }

         /* Re-check previous event for conflict if we shortened further */
         if (i > 0 && delay < 0)
         {
            gl1_hw_schedule_check_conflict(event, gl1_hw_schedule[gas_id].events[i - 1],gas_id);
         }

         /* We are done now, no need to add another event */
         return;
      }
   }

   /* Create the event since we did not find a previous frame delay */
   gl1_hw_schedule_create_event
    (
      GL1_HW_SCHED_FRAME_DELAY,
      0,  /* sub_type */
      start_offset,
      stop_offset,
      gas_id
    );
}



/*=============================================================================
FUNCTION gl1_hw_schedule_tt_ok

DESCRIPTION
   Check if a give time tracking adjustment would collide with a previously
   scheduled activity.
   delay: offset in QS of how much we are advancing or delaying the GSTMR

============================================================================= */
boolean gl1_hw_schedule_tt_ok
(
   int16 delay,
   gas_id_t gas_id
)
{
   int16 start_offset;
   int16 stop_offset;
   gl1_hw_schedule_event *event;
   uint32 i;

   /* If an acquisition is in progress then not ok */
   if (gl1_hw_schedule[gas_id].acq_in_progress) return FALSE;

   /* Calculate the offsets */
   calc_frame_delay_offsets(delay, &start_offset, &stop_offset,gas_id);

   /* Go through list and check for conflicts */
   for (i = 0; i < gl1_hw_schedule[gas_id].num_events; i++)
   {
      event = gl1_hw_schedule[gas_id].events[i];

      if ( !event )
      {
         MSG_GERAN_ERROR_3_G( "NULL event:%d num_events:%d FN:%d",
                    i, gl1_hw_schedule[gas_id].num_events, GSTMR_GET_FN_GERAN(gas_id) );

         return FALSE;
      }

      /* Check for conflict - technically if another frame delay is in this
         frame we should combine the delays and check the combined event,
         but this is quite complicated for no real gain. */
      if (event->type == GL1_HW_SCHED_FRAME_DELAY ||
          stop_offset < event->start_offset ||
          start_offset > event->stop_offset)
      {
         /* If we do not conflict and the next event is after this one
            we are done */
         if (start_offset < event->start_offset) break;
      }
      else
      {
         /* A conflict */
         return FALSE;
      }
   }

   /* No conflicts */
   return TRUE;
}

/*=============================================================================
FUNCTION gl1_hw_schedule_dtm_rx

DESCRIPTION
   Validate the DTM rx schedule.

============================================================================= */
void gl1_hw_schedule_dtm_rx
(
  const mdsp_dtm_rx_params_type *params,
  gas_id_t gas_id
)
{
   int16 sw;
   int32 start_offset, stop_offset;
   int32 rx_alpha_qs;

#ifdef FEATURE_GPRS_GBTA
   /* for PTCCH index 0 as only 1 TS */
   gl1_hw_channel_type rx_chan_type = gl1_hw_dtm_get_rx_channel_type(params->slot_params[0].timeslot,gas_id);
#endif /* FEATURE_GPRS_GBTA */

   rx_alpha_qs = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Create the event */
   sw = (int16)(mdsp_get_sync_rx_search_width(gas_id) * 4 / 2);
   start_offset = (int16)params->slot_params[0].timeslot * QS_PER_SLOT
                          - rx_alpha_qs
                          - sw;
   stop_offset  = (int16)params->slot_params[params->num_slots - 1].timeslot * QS_PER_SLOT
                          + RX_BURST_LENGTH_QS
                          + sw;
   gl1_hw_schedule_create_event
    (
      GL1_HW_SCHED_RX,
      (int16)params->slot_params[0].channel_type,
      start_offset,
      stop_offset,
      gas_id
    );

#ifdef DEBUG_GSM_SCHEDULER
   MSG_GERAN_MED_3_G("Schedule Rx Tn %d to Tn %d (%d slots)", params->slot_params[0].timeslot, params->slot_params[params->num_slots - 1].timeslot, params->num_slots);
#endif

   /* Keep track of the number of rx for EDA power measure scheduling */
   gl1_hw_schedule[gas_id].num_rx = params->num_slots;

#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
#ifdef FEATURE_GSM_POWER_MONITOR_SCHEDULE_OPTION19
   gl1_hw_schedule[gas_id].max_pwr_meas = MIN(gl1_hw_schedule[gas_id].max_pwr_meas, (MAX_MEAS_PER_FRAME - params->num_slots));
#else
   /* Rx burst uses the fifth mon spot so we can do at most 4 pwr measures */
   gl1_hw_schedule[gas_id].max_pwr_meas = MIN(gl1_hw_schedule[gas_id].max_pwr_meas, 4);
#endif /* FEATURE_GSM_POWER_MONITOR_SCHEDULE_OPTION19 */

#ifdef FEATURE_GPRS_GBTA
   if( rx_chan_type == GL1_HW_PTCCHD)
   {
     gl1_hw_schedule[gas_id].max_pwr_meas = MIN(gl1_hw_schedule[gas_id].max_pwr_meas,5);
   }
#endif /* FEATURE_GPRS_GBTA  */
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/

   /* If no Rx in tn 0 and not last burst of block then pwr meas can overlap tn 0 of next frame */
   if ((params->slot_params[0].timeslot != MDSP_TN_0) && (params->slot_params[0].burst_num != 3))
   {
     gl1_hw_schedule[gas_id].allow_late_pwr_meas = TRUE;
   }
}


/*=============================================================================
FUNCTION gl1_hw_schedule_dtm_tx

DESCRIPTION
   Validate the DTM tx schedule.

============================================================================= */
void gl1_hw_schedule_dtm_tx
(
  const mdsp_dtm_tx_params_type *params,
  gas_id_t gas_id
)
{
   int32 start_offset, stop_offset;

   /* index 0 is required for PTCCH */
   gl1_hw_channel_type  tx_chan_type = gl1_hw_dtm_get_tx_channel_type(params->timeslots[0],gas_id);

   /* Create the event */
   start_offset = ((int16)params->timeslots[0] + 3) * QS_PER_SLOT
                   - mdsp_get_tx_burst_offset(gas_id)
                   - (params->timing_advance * 4)
                   - params->tx_alpha;
   stop_offset = ((int16)params->timeslots[params->num_slots - 1] + 4) * QS_PER_SLOT
                   - (params->timing_advance * 4);
   gl1_hw_schedule_create_event
    (
      GL1_HW_SCHED_TX,
      (int16)tx_chan_type,
      start_offset,
      stop_offset,
      gas_id
    );

   MSG_GERAN_MED_3_G("Schedule Tx Tn %d to Tn %d (%d slots)", params->timeslots[0] + 3, params->timeslots[params->num_slots - 1] + 3, params->num_slots);

   /* Keep track of the number of tx for EDA power measure scheduling */
   if( GL1_DEFS_EXT_DYNAMIC_ALLOC == gl1_hw_schedule[gas_id].mac_mode )
   {
     gl1_hw_schedule[gas_id].num_tx = params->num_ul_timeslots;
   }
   else
   {
     gl1_hw_schedule[gas_id].num_tx = params->num_slots;
   }

#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
   /* Tx burst uses MON_BURST_4 so we can do at most 3 pwr measures */
#ifdef FEATURE_GPRS_GBTA
   if( tx_chan_type == GL1_HW_PTCCHU_ACC ||
       tx_chan_type == GL1_HW_PTCCHU_EXT )
   {
     gl1_hw_schedule[gas_id].max_pwr_meas = MIN(gl1_hw_schedule[gas_id].max_pwr_meas,5);
   }
   else
#endif /* FEATURE_GPRS_GBTA  */
   {
     /* Tx burst uses MON_BURST_4 so we can do at most 3 pwr measures */
     gl1_hw_schedule[gas_id].max_pwr_meas = MIN(gl1_hw_schedule[gas_id].max_pwr_meas, 3);
    }
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
}



/*=============================================================================
FUNCTION gl1_hw_schedule_int_meas

DESCRIPTION
   Validate the interference measure schedule.

============================================================================= */
boolean gl1_hw_schedule_int_meas
(
  const gl1_hw_meas_signal_type *signal_info,
  uint32                        num_freq,
  gas_id_t gas_id

)
{
   int16 start_offset, stop_offset;
   uint32 si, ei; /* start index, end index */
   int32 rx_alpha_qs;

   rx_alpha_qs = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Interference measures may be non-contiguous, but schedule contiguous
      slots as a single event */
   si = ei = 0;
   while (si < num_freq)
   {
      while (ei < num_freq - 1 &&
             (int16)signal_info[ei].tn + 1 == (int16)signal_info[ei + 1].tn)
      {
         ei++;
      }

      /* Calculate the offsets */
      start_offset = ((int16)signal_info[si].tn * QS_PER_SLOT) - rx_alpha_qs;
      stop_offset = ((int16)signal_info[ei].tn) * QS_PER_SLOT + INT_MEAS_LENGTH_QS;

      /* For some reason we need lots of extra space after an interference
         measure before scheduling a power measure, so add in another 40 qs
         here if such a scenario is possible. */
      if (signal_info[ei].tn == GL1_DEFS_TN_7) stop_offset += 40;

      /* Create the event */
      gl1_hw_schedule_create_event
       (
         GL1_HW_SCHED_INT_MEAS,
         0,
         start_offset,
         stop_offset,
         gas_id
       );

      ei++;
      si = ei;
   }

   /* No pwr measures if we are doing interference */
   gl1_hw_schedule[gas_id].max_pwr_meas = 0;

   /* If a conflict occurred, let the caller know */
   if (gl1_hw_schedule[gas_id].conflict) return FALSE;

   return TRUE;
}


/*=============================================================================
FUNCTION gl1_hw_schedule_hmsc_pwr_meas

DESCRIPTION
   Validate the power measure schedule for HMSC.

============================================================================= */
static uint32 gl1_hw_schedule_hmsc_pwr_meas
(
  uint16       sched_offset[ MAX_MEAS_PER_FRAME ],  /* output */
  gas_id_t     gas_id
)
{
  /* Start with the assumption that the power monitor scheduling is called after
   * the TX scheduling.
   */
  uint32 tx_event = gl1_hw_schedule[gas_id].num_events - 1;
  uint32  possible_async_event = 0;
  int32  tx_stop_offset;
  int32  last_tx_slot;
  uint32 num_meas = 0;
  int32  pwr_meas_start_offset;

  const int32  rf_pll_tune_time = 180;  // this constant must come from RF
  const int32  pwr_meas_req_time = rf_pll_tune_time + PWR_MEAS_REDUCED_LENGTH_QS;


  /* Try and find a possible async event */
  if( tx_event != 0 )
  {
    possible_async_event = tx_event - 1;
  }
  else
  {
    possible_async_event = 0;
  }

  /* The async event gets booked after the TX but the insertion is done based on the start time.
   * Although the tx event is in num_events - 1, the tx event finishes before the async event.
   *
   *  | ********* ASYNC EVENT ******************|         index = num events - 2 booked at t - 1
   *    | ---------  TX EVENT   ----------|               index = num events - 1 booked at t - 2
   *
   * Assuming that a previous event's end time will always be behind the current event's end time is
   * wrong hence we need the && part of the if( ) statement.
   * If we don't take the end time into account then the power monitor will clash with the async event.
   */
  if( ( GL1_HW_SCHED_TX == gl1_hw_schedule[gas_id].events[tx_event]->type )
    &&(   ( GL1_HW_SCHED_ASYNC_RX != gl1_hw_schedule[gas_id].events[possible_async_event]->type )
       || ( gl1_hw_schedule[gas_id].events[tx_event]->stop_offset >  gl1_hw_schedule[gas_id].events[possible_async_event]->stop_offset )
      )
    )
  {
    /* Tx found, schedule HMSC monitor immediately after it. No need to */
    /* check if it fits, it must fit in the worst case (TA = 0) */

    /* Set the monitor scheduling offset ( + 1 is to avoid scheduling conflict) */
    tx_stop_offset = gl1_hw_schedule[gas_id].events[tx_event]->stop_offset;

   /* There is a scenario where false trigger to schedule hmsc monitor when acquisition
     started , resulting  a hmsc monitor schedule in previous frame .To avoid this,
     schedule  HMSC only if   last "tx_stop_offset" is a positive  means last tx should
     be in the current frame */
   if(tx_stop_offset > MINIMUM_FRAME_TICK_TO_SCHEDULE_HMSC)
   {
    pwr_meas_start_offset = tx_stop_offset + 1 + rf_pll_tune_time;

    /* Now we need to make sure that the monitor and last Tx are not in */
    /* the same slot. If they are then move the monitor to the next slot */
    last_tx_slot = (tx_stop_offset - 1)/QS_PER_SLOT;
    if(last_tx_slot < 7)/* to avoid HMSC pwr meas schedule beyond 5000 QS*/
    {
    if( pwr_meas_start_offset/QS_PER_SLOT == last_tx_slot )
    {
      pwr_meas_start_offset = (last_tx_slot + 1)*QS_PER_SLOT;
    }

    /* Set the output */
    sched_offset[num_meas++] = (uint16)pwr_meas_start_offset;
    MSG_MED( "Schedule HMSC Pwr Meas: pwr_meas_start_offset = %d tx_stop_offset = %d last_tx_slot = %d",
      pwr_meas_start_offset, tx_stop_offset, last_tx_slot );

    /* Now remove RF PLL tune time and create event */
    pwr_meas_start_offset -= rf_pll_tune_time;
    gl1_hw_schedule_create_event
     (
      GL1_HW_SCHED_PWR_MEAS,                         /* schedule type    */
      1,                                             /* sub type 1: HMSC */
      pwr_meas_start_offset,                         /* start offset     */
      pwr_meas_start_offset + pwr_meas_req_time,      /* stop offset,      */
         gas_id
     );

    }
    }
  }

  return num_meas;
}



/*=============================================================================
FUNCTION gl1_hw_schedule_pwr_meas

DESCRIPTION
   Validate the power measure schedule

============================================================================= */
uint32 gl1_hw_schedule_pwr_meas
(
   ARFCN_T*     arfcns,
   uint32       max_num_meas,
   uint16       sched_offset[ MAX_MEAS_PER_FRAME ],   /* output */
   mdsp_gprs_monitor_coex_params coex_params[MAX_MEAS_PER_FRAME], /* output */
   boolean      *mdsp_scheduled ,   /* output */
   boolean      *hmsc_enable,
   gas_id_t      gas_id
)
{

   uint32 gstmr_qs;

   uint32 i = 0;

   int32 freespace, offset, prev_offset = 0;

   int32 pwr_meas_req_time, pwr_meas_max_time, fw_offset_to_meas_start;

   uint32 event;

   uint8 mon_timeslot;
#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
   uint8 previous_mon_timeslot = 0xF0;    /* Set to invalid timeslot number initially */
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
#ifdef FEATURE_GSM_COEX
   int32 coex_fallback_offset[GL1_DEFS_SLOTS_IN_FRAME];
   uint8 coex_fallback_offset_count = 0;
#ifdef FEATURE_DUAL_DATA
   boolean pwr_mon_info_valid = FALSE;
#endif
#endif

#ifdef __DONT_STRIP_NEEDED_FOR_BLACKLISTING
#ifdef  FEATURE_GSM_GPRS_MSC33
   boolean hmsc_dummy = TRUE;

   /* To allow black list to pickup HMSC */
   hmsc_dummy = hmsc_dummy;
#endif
#endif

   *hmsc_enable = FALSE;
   *mdsp_scheduled = FALSE;


   /* Check that we are not trying to do too much */
   max_num_meas = MIN(max_num_meas, gl1_hw_schedule[gas_id].max_pwr_meas);

   if (max_num_meas == 0)
   {
     MSG_GERAN_MED_0_G("Max_num_meas is 0");
     return (0);
   }

   /* If an Ncell Rx acquisition is active we cannot do power measures */
   if (gl1_hw_schedule[gas_id].async_rx_in_progress)
   {
     MSG_GERAN_MED_0_G("Async_rx is 1");
     return (0);
   }

   /* If an acquisition is active we cannot do power measures */
   if (gl1_hw_schedule[gas_id].acq_in_progress)
   {
     MSG_GERAN_MED_0_G("acq_in_progress is 1");
     return (0);
   }

   /* Check enough processing time for power measure (to prevent ISR overrun) */
   gstmr_qs = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));
   if ((gstmr_qs > (QS_PER_FRAME - PWR_MEAS_SCHEDULE_PROCESS_TIME_MAX))
        || (get_vstmr_FN_modMaxFN(geran_map_gas_id_to_nas_id(gas_id)) != vstmr_rd_modMaxFN_frame_num(gas_id))
      )
   {
      if (!gl1_drx_first_awake_frame(gas_id))
      {
         MSG_GERAN_ERROR_1_G("No enough time to schedule pwr meas, qs=%d", gstmr_qs);
         return (0);
      }
   }

   gl1_hw_arbitration[gas_id].monitors.count = 0;

   fw_offset_to_meas_start = gl1_hw_get_rf_gsm_mon_setup_time(gas_id) + GL1_HW_SCHEDULE_GUARD_PERIOD;

   /* The total time required by a power measure including tuning and BB Rx turn off */
   pwr_meas_req_time = fw_offset_to_meas_start + PWR_MEAS_LENGTH_QS;
#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
   if( max_num_meas >= (8) )
   {
      pwr_meas_max_time = 9 * QS_PER_SLOT;
   }
   else
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
   {
      /* Maximum offset at which a power measure can finish and not conflict with */
      /* a possible receive in slot 0 of the next frame */
      pwr_meas_max_time = QS_PER_FRAME - PWR_MEAS_END_GUARD_PERIOD - gl1_hw_get_rf_gsm_rx_setup_time(gas_id);
      /* If no Rx in slot 0 of next frame can schedule 1 slot later */
      if (gl1_hw_schedule[gas_id].allow_late_pwr_meas)
      {
         pwr_meas_max_time += QS_PER_SLOT;
      }
   }
#ifdef FEATURE_GSM_DED_SCELL_MONITOR  
      if (arfcns[i].num == DUMMY_ARFCN_FOR_SC_MEAS)
      {
#ifdef DEBUG_FEATURE_GSM_DED_SCELL_MONITOR
        MSG_GERAN_HIGH_1_G("return DUMMY_ARFCN_FOR_SC_MEAS %d", arfcns[i].num);
#endif /* DEBUG_FEATURE_GSM_DED_SCELL_MONITOR*/
         return (DUMMY_ARFCN_FOR_SC_MEAS);
      }
#endif /*FEATURE_GSM_DED_SCELL_MONITOR */

#ifdef FEATURE_GSM_COEX
   /* Initialise the list of monitor ARFCNs to be arbitrated */
   for(i=0;i<max_num_meas;i++)
   {
      gl1_hw_arbitration[gas_id].monitors.monitor[i].arfcn = arfcns[i];
   }

   /* Initialise the number of monitor ARFCNs to be arbitrated */
   gl1_hw_arbitration[gas_id].monitors.count = max_num_meas;

   /* Perform arbitration of activities prior to configuring RF and sending mDSP commands */
   gl1_hw_arbitration[gas_id].notify_event(ARBITRATE_MONITORS, gas_id);

   gl1_hw_arbitration[gas_id].monitors.count = 0;

   /* The number of monitors are restricted to the max permitted by the arbitrator*/
   if(max_num_meas > gl1_hw_arbitration[gas_id].monitors.max_per_frame) max_num_meas = gl1_hw_arbitration[gas_id].monitors.max_per_frame;
#endif

   /* Find space before each event and schedule power measures */
   for (event = 0;
        event <= gl1_hw_schedule[gas_id].num_events &&  gl1_hw_arbitration[gas_id].monitors.count < max_num_meas;
        event++)
   {
      /* Determine start of freespace */
      if (event == 0 || gl1_hw_schedule[gas_id].events[event - 1]->stop_offset < 0)
      {
         offset = 0;
      }
      else
      {
         offset = gl1_hw_schedule[gas_id].events[event - 1]->stop_offset;
      }
      /* Add 1 slot margin (required for FW processing) if after acq stop */
      if (event != 0 && gl1_hw_schedule[gas_id].events[event - 1]->type == GL1_HW_SCHED_ACQ)
      {
         offset += QS_PER_SLOT;
      }

      /* Catch the case when a conflict occurred and the stop offset of the
         previous event is before the stop offset of this event */
      offset = MAX(offset, prev_offset);
      prev_offset = offset;

      /* Determine size of freespace */
      if (event == gl1_hw_schedule[gas_id].num_events)
      {
         freespace = pwr_meas_max_time - offset;
      }
      else
      {
         freespace = gl1_hw_schedule[gas_id].events[event]->start_offset - offset - PWR_MEAS_END_GUARD_PERIOD;
      }

      /* Go through scheduling monitors until we are out of space */
#ifdef FEATURE_GSM_DYNMC_PWR_MEAS
      /* If free available space is equal to the pwr meas time, then schedule pwr measurement as
         PWR_MEAS_END_GUARD_PERIOD (20qs) takes care of the delay between two events (pwr meas event and event X)
         In a case, where there is no event schedule in the frame then use all pwr meas max time as PWR_MEAS_END_GUARD_PERIOD
         is also considered in calculating pwr meas max time.*/
      while ((freespace >= pwr_meas_req_time) && (gl1_hw_arbitration[gas_id].monitors.count < max_num_meas))
#else
      while ((freespace > pwr_meas_req_time) && (gl1_hw_arbitration[gas_id].monitors.count < max_num_meas))
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
      {
#ifdef FEATURE_GSM_COEX
          /* The placement of monitors needs to avoid any non-zones specified by the arbitrator */
          for(i=0;i<gl1_hw_arbitration[gas_id].monitors.nozone_count;i++)
          {
             if( !( (gl1_hw_arbitration[gas_id].monitors.nozone_end[i]   <  offset) ||
                    (gl1_hw_arbitration[gas_id].monitors.nozone_start[i] > (offset + pwr_meas_req_time) ) ) )
             {
                /* The next candidate monitor position violates a no-zone. The aim is to position all
                 * monitors without having to violate the no-zone. This may not always be possible, so in
                 * the event that the minimum number of monitors per frame could not be positioned we will
                 * fall back to positioning inside the no-zone.
                 */
                 if(coex_fallback_offset_count < GL1_DEFS_SLOTS_IN_FRAME) { coex_fallback_offset[coex_fallback_offset_count++] = offset; }

                /* Move the offset to the end of the no-zone and adjust the freespace. If there isn't
                 * sufficient space for a monitor between the end of the no-zone and end of the freespace
                 * then we'll bail out of the current iteration and attempt to find the next freespace.
                 */
                 freespace = freespace - (gl1_hw_arbitration[gas_id].monitors.nozone_end[i] - offset);
                 offset    = gl1_hw_arbitration[gas_id].monitors.nozone_end[i];
             }
          }
#endif

          /* There is space here for the power measure, but adjust so only one monitor per timeslot for FW */
          /* Need to adjust to FW offsets to check timeslot clash */
           mon_timeslot = (offset + fw_offset_to_meas_start) / QS_PER_SLOT;
           /* FW shall support multiple monitors in one ts*/
       /* Checking of mon_timeslot and prev_timeslot is present in the Dime but not in the Triton.
          The check ensures that GL1 should not schedule two pwr measurement events in the same
          slot. It looks like this scenario never occured as the max limit is 7. Don't knwow the
          history of this code. But better to keep this code as a safe check.
          */
#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
       if (mon_timeslot == previous_mon_timeslot)
       {
        mon_timeslot += 1;
        freespace += offset;
        offset = (mon_timeslot* QS_PER_SLOT) - fw_offset_to_meas_start;
        freespace -= offset;
       }
       previous_mon_timeslot = mon_timeslot;
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
           freespace -= pwr_meas_req_time;

           /* If there is space add power measruement, but ensure Rx does not start in next frame */
           if ((freespace > 0) && (mon_timeslot < GL1_DEFS_SLOTS_IN_FRAME))
           {
              gl1_hw_arbitration[gas_id].monitors.monitor[gl1_hw_arbitration[gas_id].monitors.count].arfcn = arfcns[gl1_hw_arbitration[gas_id].monitors.count];
              gl1_hw_arbitration[gas_id].monitors.monitor[gl1_hw_arbitration[gas_id].monitors.count].qs_position= (uint16)offset;
              gl1_hw_arbitration[gas_id].monitors.count++;
              offset += pwr_meas_req_time;
           }
      }
   }

#ifdef FEATURE_GSM_COEX
    /* Fall back to positioning the monitors within the no-zone a minimum of one monitors could not be scheduled with the no-zone enforced */
    if( (gl1_hw_arbitration[gas_id].monitors.count==0) && (coex_fallback_offset_count!=0) )
    {
       gl1_hw_arbitration[gas_id].monitors.monitor[0].arfcn = arfcns[0];
       gl1_hw_arbitration[gas_id].monitors.monitor[0].qs_position= coex_fallback_offset[0];
       gl1_hw_arbitration[gas_id].monitors.count = 1;

#ifdef FEATURE_DUAL_DATA
       /* Can't find a place to put the monitor so let GFW have a go and finding a space */
       /* GFW bureaucracy checkpoint - requesting auto monitor positioning without SYNC RX is a crime punishable by CRASH */
       if( ( gl1_hw_schedule[gas_id].num_events > 0 ) && (gl1_hw_schedule[gas_id].events[0]->type == GL1_HW_SCHED_RX ) )
       {
          pwr_mon_info_valid = TRUE;
       }
#endif
    }
#endif

#ifdef DEBUG_GSM_SCHEDULER
   for (i = 0; i < gl1_hw_arbitration[gas_id].monitors.count; i++)
   {
       MSG_GERAN_MED_2_G("Schedule Pwr Meas monitor_placements , num_placements %d, Start Offset %d",
             gl1_hw_arbitration[gas_id].monitors.count,gl1_hw_arbitration[gas_id].monitors.monitor[i].qs_position);
   }
#endif

   /* Add power measure events */
   for (i = 0; i < gl1_hw_arbitration[gas_id].monitors.count; i++)
   {
      offset = (int32)gl1_hw_arbitration[gas_id].monitors.monitor[i].qs_position;
      sched_offset[i] = offset + fw_offset_to_meas_start;
      coex_params[i].coex_desense_id = gl1_hw_arbitration[gas_id].monitors.monitor[i].desense_id;
      coex_params[i].coex_priority   = gl1_hw_arbitration[gas_id].monitors.monitor[i].priority;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_drdsds_enabled( gas_id ) )
      {
        coex_params[i].coex_skip = gl1_hw_arbitration[gas_id].monitors.monitor[i].skip;
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
        coex_params[i].coex_skip = FALSE;
      }

      gl1_hw_schedule_create_event
       (
         GL1_HW_SCHED_PWR_MEAS,                         /* schedule type */
         0,                                             /* sub type      */
         (offset + GL1_HW_SCHEDULE_GUARD_PERIOD),       /* start offset  */
         (offset + pwr_meas_req_time),                  /* stop offset   */
          gas_id
       );

#ifdef FEATURE_GSM_LOG_COEX
      /* Log power measurement burst in cxm_log_table */
      l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), arfcns[i].num, CXM_LOG_MONITOR,
                           offset + pwr_meas_req_time - 96, CXM_LOG_UNKNOWN_VALUE, 0, 0, 0, 
                           CXM_LOG_ACTIVITY_RX, gas_id);
#endif

#ifdef DEBUG_GSM_SCHEDULER
      MSG_GERAN_MED_3_G("Schedule Pwr Meas %d, Start Offset %d, Stop Offset %d",
               i+1, (offset + GL1_HW_SCHEDULE_GUARD_PERIOD), (offset + pwr_meas_req_time));
#endif
   }

 if( 0 == gl1_hw_arbitration[gas_id].monitors.count )
  {

#ifndef  FEATURE_GSM_GPRS_MSC33

   /* In EDA class 12 we can let the mDSP schedule a power measure if GL1 has not*/
   if( ( gl1_hw_schedule[gas_id].mac_mode == GL1_DEFS_EXT_DYNAMIC_ALLOC )
     &&( gl1_hw_schedule[gas_id].num_tx == (4) )
     &&( gl1_hw_schedule[gas_id].num_rx != 0 )
     &&( gl1_hw_arbitration[gas_id].monitors.count == 0) )
   {
      /* Allow at most one mdsp scheduled power measure.  The offset in this
         case is the offset from the start of the last receive burst to the
         start of the power measure.  In other words we need to add the
         rx burst length, guard period, and mon alpha. */
      sched_offset[0] = (uint16)(RX_BURST_LENGTH_QS
                                 + (mdsp_get_sync_rx_search_width(gas_id) * 4)
                                 + fw_offset_to_meas_start);

      /* The firmware schedules the power monitor */
      *mdsp_scheduled = TRUE;

      gl1_hw_arbitration[gas_id].monitors.count = 1;

#ifdef FEATURE_GSM_COEX
      coex_params[0].coex_desense_id = gl1_hw_arbitration[gas_id].monitors.monitor[0].desense_id;
      coex_params[0].coex_priority   = gl1_hw_arbitration[gas_id].monitors.monitor[0].priority;

#ifdef FEATURE_GSM_LOG_COEX
      l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), arfcns[0].num, CXM_LOG_MONITOR,
                           sched_offset[0] + pwr_meas_req_time - 96,
                           CXM_LOG_UNKNOWN_VALUE, 0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
#endif
   }
#else

  gl1_hw_arbitration[gas_id].monitors.count = gl1_hw_schedule_hmsc_pwr_meas( sched_offset, gas_id );

#ifdef FEATURE_GSM_COEX
  for (i = 0; i < gl1_hw_arbitration[gas_id].monitors.count; i++)
  {
    coex_params[i].coex_desense_id = gl1_hw_arbitration[gas_id].monitors.monitor[i].desense_id;
    coex_params[i].coex_priority   = gl1_hw_arbitration[gas_id].monitors.monitor[i].priority;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( gl1_drdsds_enabled( gas_id ) )
    {
      coex_params[i].coex_skip = gl1_hw_arbitration[gas_id].monitors.monitor[i].skip;
    }
    else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    {
      coex_params[i].coex_skip = FALSE;
    }

#ifdef FEATURE_GSM_LOG_COEX
    /* Log power measurement burst in cxm_log_table */
    l1_log_cxm_new_entry(mdsp_get_current_seq_num(gas_id), arfcns[i].num, CXM_LOG_MONITOR,
                         sched_offset[i] + 96, CXM_LOG_UNKNOWN_VALUE,
                         0, 0, 0, CXM_LOG_ACTIVITY_RX, gas_id);
#endif
  }

#endif
 *hmsc_enable = TRUE;
#endif /* FEATURE_GSM_GPRS_MSC33*/

 }
   /* No more pwr measures this frame */
   gl1_hw_schedule[gas_id].max_pwr_meas = 0;

#ifdef FEATURE_DUAL_DATA
   mdsp_notify_power_mon_info( pwr_mon_info_valid,
                               (uint16)gl1_hw_arbitration[gas_id].monitors.nozone_start[0],
                               (uint16)gl1_hw_arbitration[gas_id].monitors.nozone_end[0],
                               (uint16)gl1_hw_arbitration[gas_id].monitors.nozone_start[1],
                               (uint16)gl1_hw_arbitration[gas_id].monitors.nozone_end[1],
                               gas_id
                             );
#endif
   return gl1_hw_arbitration[gas_id].monitors.count;
}

/*=============================================================================
FUNCTION gl1_hw_pwr_meas_req_time

DESCRIPTION
  Returns the total time required by a power measure including tuning and BB Rx turn off

============================================================================= */
int32 gl1_hw_pwr_meas_req_time( gas_id_t gas_id )
{
  /* The total time required by a power measure including tuning and BB Rx turn off */
  return ( gl1_hw_get_rf_gsm_mon_setup_time(gas_id) + GL1_HW_SCHEDULE_GUARD_PERIOD + PWR_MEAS_LENGTH_QS );
}

/*=============================================================================
FUNCTION gl1_hw_pwr_meas_req_time

DESCRIPTION
  Returns therf_gsm_mon_setup_time by a power measure including GL1_HW_SCHEDULE_GUARD_PERIOD

============================================================================= */

int32 gl1_mon_set_up_with_sched_guard_period( gas_id_t gas_id )
{
   return ( gl1_hw_get_rf_gsm_mon_setup_time(gas_id) + GL1_HW_SCHEDULE_GUARD_PERIOD );
}

/*=============================================================================
FUNCTION gl1_hw_schedule_async_rx

DESCRIPTION
   Validate the rx schedule.

============================================================================= */
void gl1_hw_schedule_async_rx
(
  gl1_hw_channel_type  chan_type,
  uint16               timing_offset,
  gas_id_t             gas_id
)
{
   int16 sw;
   int32 rx_alpha_qs;

   rx_alpha_qs = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Skip the check if we are not in gsm/gprs mode */
   if (!gl1_hw_mode_gsm[gas_id]) return;

   /* Create the event */
   sw = (int16)(mdsp_get_async_rx_search_width(gas_id) * 4 / 2);
   gl1_hw_schedule_create_event
    (
      GL1_HW_SCHED_ASYNC_RX,
      (int16)chan_type,
      (int16)timing_offset - sw - rx_alpha_qs,
      (int16)timing_offset + RX_BURST_LENGTH_QS + sw
#ifndef DEBUG_HW_SELECT_STAGE3_OUTPUT_CHANNEL_FILTER
       + (chan_type == GL1_HW_SCH ? 0 : 40)
#endif
      ,gas_id
    );

#ifdef  FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    if ((chan_type != GL1_HW_BCCH) && (chan_type != GL1_HW_PCH))
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
    {
      /* Set the flag to indicate Schedulling of Ncell Rx in progress. */
      gl1_hw_schedule[gas_id].async_rx_in_progress = TRUE;
    }

#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
   /* Async rx uses PWR_BURST (MON_BURST_2) so we can do at most 1 measure */
   gl1_hw_schedule[gas_id].max_pwr_meas = MIN(gl1_hw_schedule[gas_id].max_pwr_meas, 1);
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
}

/*=============================================================================
FUNCTION gl1_hw_schedule_start_acq

DESCRIPTION
   Validates the start acq.

============================================================================= */
void gl1_hw_schedule_start_acq
(
   uint16   timing_offset,
   gas_id_t gas_id
)
{
   int32 rx_alpha_qs;

   rx_alpha_qs = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Skip the check if we are not in gsm/gprs mode */
   if (!gl1_hw_mode_gsm[gas_id]) return;

   /* Save the acq parameter and mark as in progress */
   gl1_hw_schedule[gas_id].acq_start_offset = (int16)timing_offset - rx_alpha_qs;
   gl1_hw_schedule[gas_id].acq_in_progress = TRUE;

   /* Start acq uses MON_BURST_1 so we cannot do any power measures */
   gl1_hw_schedule[gas_id].max_pwr_meas = 0;
}



/*=============================================================================
FUNCTION gl1_hw_schedule_stop_acq

DESCRIPTION
   Schedules the stop acq.

============================================================================= */
void gl1_hw_schedule_stop_acq
(
   uint16   timing_offset,
   gas_id_t gas_id
)
{
   /* Skip the check if we are not in gsm/gprs mode */
   if (!gl1_hw_mode_gsm[gas_id]) return;

   /* Sanity check that an acquisition was started at some point */
   if (!gl1_hw_schedule[gas_id].acq_in_progress)
   {
      MSG_GERAN_ERROR_1_G("Acq stop when no acq in progress, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
   }
   else
   {
      /* Check if we are aborting before getting the tick for this frame,
         and if so add another frame to the offset to compensate */
      if (gl1_hw_schedule[gas_id].current_FN != GSTMR_GET_FN_GERAN(gas_id))
      {
         timing_offset += QS_PER_FRAME;
      }

      /* Now create the complete acq event */
      gl1_hw_schedule_create_event
       (
         GL1_HW_SCHED_ACQ,
         0,
         gl1_hw_schedule[gas_id].acq_start_offset,
         timing_offset,
         gas_id
       );
      gl1_hw_schedule[gas_id].acq_in_progress = FALSE;
   }

#ifndef FEATURE_GSM_DYNMC_PWR_MEAS
   /* Stop acq uses the sixth spot so we can only do at most 5 pwr measures */
   gl1_hw_schedule[gas_id].max_pwr_meas = MIN(gl1_hw_schedule[gas_id].max_pwr_meas, 5);
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/
}

/*=============================================================================
FUNCTION gl1_hw_update_acq_in_progress

DESCRIPTION
   Update acq_in_progress, which is mainly used as a check before allowing to schedule any power monitors.

============================================================================= */
void gl1_hw_update_acq_in_progress(boolean status, gas_id_t gas_id )
{
  gl1_hw_schedule[gas_id].acq_in_progress = status;
}


#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || \
    defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
/*=============================================================================
FUNCTION gl1_hw_schedule_rf_sample_ram

DESCRIPTION
   Schedules a wcdma rf sample

============================================================================= */
void gl1_hw_schedule_rf_sample_ram(
   #ifdef FEATURE_GSM_TO_WCDMA_MSGR
   uint16 start_offset,
   uint16 stop_offset,
   #else
   uint16 timing_offset,
   #endif
   gas_id_t gas_id)
{
#ifdef FEATURE_GSM_TO_WCDMA_MSGR

  gl1_hw_schedule_create_event(GL1_HW_SCHED_G2W_GAP,
                               0,
                               (int32)start_offset,
                               (int32)stop_offset,
                               gas_id);

#else /* FEATURE_GSM_TO_WCDMA_MSGR */
  wl1_x2w_srch_rf_timing_params_struct_type rf_timing_params;

  wl1_x2w_srch_get_rf_timing_params(&rf_timing_params, gas_id);

  /* Create the event */
  gl1_hw_schedule_create_event(GL1_HW_SCHED_G2W_GAP,
                               0,
                               (int32)(timing_offset),
                               (int32)(timing_offset + rf_timing_params.startup_time + 
                                       rf_timing_params.pll_settling_time + 
                                       MDSP_FILL_SAMPLE_RAM_DURATION + 
                                       rf_timing_params.cleanup_time),
                               gas_id );
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}
#ifdef FEATURE_GSM_TDS
/*=============================================================================
FUNCTION gl1_hw_schedule_g2x_gap

DESCRIPTION
   Schedules a wcdma rf sample

============================================================================= */
void gl1_hw_schedule_g2x_gap (uint32 timing_offset,gas_id_t gas_id)
{
   /* Create the event */
   gl1_hw_schedule_create_event
    (
      GL1_HW_SCHED_G2X_GAP,
      0,
      0,
      (int32)(timing_offset + QS_PER_SLOT),
      gas_id
    );
}
#endif /*FEATURE_GSM_TDS*/
#endif

/*=============================================================================
FUNCTION gl1_hw_schedule_get_num_slots

DESCRIPTION
   Interface function for NCell FCCH scheduling to get the total number of used
   slots.

============================================================================= */
uint32 gl1_hw_schedule_get_num_slots(gas_id_t gas_id)
{
   return (gl1_hw_schedule[gas_id].num_tx + gl1_hw_schedule[gas_id].num_rx);
}


#ifdef FEATURE_GSM_TDS

void  gl1_hw_schedule_remove_x2g_startup( gas_id_t gas_id )
{
  gl1_hw_schedule[gas_id].x2g_in_progress = FALSE;

}

boolean  gl1_hw_schedule_x2g_gap_startup( uint16  timing_offset,gas_id_t gas_id  )
{
  boolean  schedule_ok = TRUE;

  if( FALSE == gl1_hw_schedule[gas_id].x2g_in_progress )
  {
    gl1_hw_schedule[gas_id].x2g_in_progress      = TRUE;
    gl1_hw_schedule[gas_id].x2g_gap_start_offset = timing_offset;
  }
  else
  {
    /* Double startup */
    MSG_GERAN_ERROR_0_G(" two successive startup commands ");

    schedule_ok = FALSE;
  }

  return  schedule_ok;
}

boolean  gl1_hw_schedule_x2g_gap_cleanup( uint16  timing_offset,
                                                     boolean  pre_tick,
                                                     gas_id_t gas_id
)
{
  boolean schedule_ok = TRUE;

  if( gl1_hw_schedule[gas_id].x2g_in_progress )
  {
    int32  time_offset = timing_offset;

    gl1_hw_schedule[gas_id].x2g_in_progress = FALSE;

  if( pre_tick )
  {
      time_offset -= QS_PER_FRAME;
      gl1_hw_schedule[gas_id].x2g_gap_start_offset -= QS_PER_FRAME;

  }

    gl1_hw_schedule_create_event( GL1_HW_SCHED_X2G_GAP,
                                  0,
                                  gl1_hw_schedule[gas_id].x2g_gap_start_offset,
                                  time_offset,
                                  gas_id);

#if 0 /* Disabled for now and enabled the conflict events printout */
  if( gl1_hw_schedule[gas_id].conflict )
  {
      /* Event was not booked */

    /* Indicate an error to caller */
    schedule_ok = FALSE;
  }
#endif

  }
  else
  {
    MSG_GERAN_ERROR_0_G(" cleanup with no corresponding startup ");

  schedule_ok = FALSE;
  }

  return  schedule_ok;
}

#endif


