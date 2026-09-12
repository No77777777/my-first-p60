/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                 SCE: RECEIVE

GENERAL DESCRIPTION
   This SCE module contains state machine receive functions.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS



Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_rcv.c#2 $
$DateTime: 2020/02/10 05:49:45 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  --------------------------------------------------------- 
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
10/06/16   nm        CR1025785 Set ignore_xcch in case ncell ACQ on PTM Yields PCH on idle sub
19/10/15   br        CR926522 GL1 to Pass the frequency offset to rotator to correct in GBTA.
18/09/15   nm        CR908307 Back out CR888325
20/08/15   nm        CR888325 Improve Debug F3 if idle sub page collides with transfer sub Ncell SCH/FCCH
16/07/15   sp        CR840930: Dont reset the rx state to Aborting if already aborted + CR710841
30/06/15   ab        CR869795 : SR-DSDS : Perform GBTA with Transfer SUB FCCH/SCH using COEX.
29/06/15   am        CR862998 Restrict CR771762 to some max time limit
30/03/15   cja       CR800650 Optimise irat state machine
03/04/15   br        CR763774 GL1 logging improvement to ease debugging
25/02/14   cah       CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
17/02/15   br        CR776829 Update sce rx.abort properly while leaving dedicated mode
12/02/15   am        CR771762 BCCH Decode Failures at Good RSSI when the NCell is drifted by 1.5KHz
12/01/15   sjv       CR756154 Enhance Band Registration for Activities   
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
03/12/14   npt       CR759467 Fix compiler warnings
11/11/14   sjv       CR754470 If receive_in_progress then do not perform band reg. for SC_BCCH 
30/10/14   sjv       CR748415 Do not perform band registrations for SCH/FCH if receive in progress 
10/10/14   nm        CR735664 Reset start_wcdma_receive after TDS srch done
17/10/14   pa        CR739626: In G2X TA, perform trm band registration on actual scheduling of BCCH.
24/09/14   hd        CR725334: Check l1_sci_monscan_meas_in_progress while SCE operation in progress
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
19/05/14   br        CR667202 Issuing ACQ command in G2L measurment gap in PTM.
06/03/14   br        CR626780, RACE condition between TRANSFER G sub and IDLE G sub, issuing SCH in G2x gap.
27/01/14   ss        CR605762 Call abort_receive if stale boooking detected in frame manager
01/11/13   ss        CR569849: Declare l1_sc_wcdma_ded_tick_abort_stuck_srch in a header file
05/09/13   sp        CR538279 Add F3s to debug rcv_in_progress state
15/08/13   sk        CR524039 GBTA bringup changes
08/08/14   nk        CR705945 Check for GERAN_ACCESS_STRATUM_ID_UNDEFINED to avoid memory read crash
25/06/13   cs        TSTS Updates
28/11/12   br        CR383165 variable type cast to fix compilation warnings
21/03/12   cgc       Cr344065 wait for LTE_CPHY_IRAT_MEAS_CLEANUP_CNF
                     add wcdma_id_srch_in_progress to l1_sci_is_op_in_progress()
21/06/11   cs        Initialise the aborted state in l1_sc_receive_init()
11/01/11   cgc       Fix compiler warning add cast #379,#441
13/07/09   dv        CR187784 - Repeated Idle aborts when Fast RACH requested
                     after PKT Transfer Mode
28/07/98   hv        Added KxMutex support
19/05/08   og        Fix for SDCCH8 lockup after the handover request is received. CR146010.
08/03/05   sv        Allow Cell ID searches to schedule after higher priority
                     neighbor activities are done.
05/25/05   sv        Fix to correctly schedule Ncell ACQ which was causing Ncell
                     BCCHs to fail.
05/23/05   sv        Move the Frame Manager ticker out of SCE tick function.
04/21/05   sv        Remove the unused "power_scan_in_progress" variable.
01/29/05   bm        Abort any stuck WCDMA DED searches
01/26/05   sv        Add support for Frame Manager.
10/20/04   sv        Optimization of Ncell SCE awake time
07/22/04   gw        Set rx.done to NULL in receive_callback so that it can't be
                     called more than once per receive.
06/22/04   sv        Added L1 NV logging mechanism on Error Fatals.
06/21/04   gsc       Call l1_sci_clear_required_frames in l1_sci_receive_done and
                     l1_sc_abort_receive_callback. API changes for l1_sci_tick_idle
                     and l1_sci_check_available_frames. Removed
                     l1_sci_ok_to_schedule_cbch.
06/02/04   gw        Still tick some handlers in the abort state.  This is
                     required to allow these handlers to keep L1 awake.
05/26/04   sv        Lint Cleanup.
05/26/04   hg        Added missing featurization around some GtoW code.
05/24/04   gw        Change to aborting state after calling abort_drivers().
05/14/04   gw        Yet another refinement of the aborting mechanism.
04/22/04   dnn/hg    Added missing featurization around some GtoW code.
03/24/04   gw        Added id to handle case where abort receive comes after
                     task callback has been put in queue.  Added debug code to
                     log rx actvity. Added code in l1_sci_receive_done() to
                     handle this function beingcalled after abort due to FTN
                     being called to early in Dedicated Mode.
03/19/04   gw        Moved aborting back into ISR context to avoid writing to
                     mDSP from the task context.
03/16/04   gw        Modified receive module design to be compatible with
                     background PLMN search.
02/25/04   gw        Lint clean up.
02/09/04   dp        Added support for background HPLMN searching
08/01/03   ws        Added StateSBTrans and StateFCBTrans to l1_sci_tick_receive() for GPRS
                     due to modifications in aborting SB driver
10/14/03   gw        Added l1_sci_ok_to_schedule_cbch() to support CBCH.
09/12/03   tb        Added handling of StateWcdmaSrch in l1_sci_tick_receive
                     to fix a problem with searching while on SDCCH.
08/28/03   gw        Fixed typo in a MSG_GERAN_ERROR_3_G().
08/21/03   gw        Minor modifications to aborting mechanism to support aborts
                     that take more than one frame, e.g. ncell BCCH.
30/07/03   kf        Removed l1_sc_abort_receive_callback_gprs.
07/14/03   gw        Removed ASSERTs.
07/11/03   gfr       Lint cleanup.
07/11/03   gw        Changes to speed up reselection.
07/08/03   gw        GRR/L1 SCE cleanup. Changes to ncell BCCH receiving.
06/19/03   gw        Added debug function to print state.
06/06/03   tb        Modifications to support dedicated mode gtow searching.
05/02/03   gw        Merged in power transfer changes from branch.
04/16/03   sw        Added initial changes for WCDMA neighbour cell search
                     during packet idle on PCCCH
04/15/03   tb        Added use of l1_sc_globals.wcdma_list_srch_in_progress
                     to fix problems with aborting idle mode.
04/04/03   gw        Removed obsolete debug code.
04/02/03   gw        Changes to reflect modifications in monscan module.
09/04/03   ws        Merged code from samsund 3.0 branch and set rx.state to
                     NULL in receive abort callback
03/26/02   pjr       Added RequestBCChGprs case to rx.request switch
02/07/02   ag        Updates for FEATURE_GPRS_TBF_SUSPEND
12/03/02   mk        General GPRS featurization cleanup.
12/02/02   ws        Updated abort handling of sb and fcb bursts for GPRS
11/25/02   mk        Updated GPRS baseline.
11/22/02   ag        General improvements to NCELL BCCH reading under switch
                     FEATURE_GPRS_TBF_SUSPEND
11/21/02   mk        Deleted unsupported code under FEATURE_GSM_ALTERNATE_PLMN_SEARCH.
11/15/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/12/02   ag        Added PL1 GPRS TBF suspension for NCELL BCCH reading code
                     hooks under switch FEATURE_GPRS_TBF_SUSPEND.
11/03/02   tb        Added support for INTERRAT_GTOW
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
08/12/02   gw        Featurized code for alternate PLMN searching.
06/24/02   dp        Changed receive_callback to invoke rx.done only if
                     receiving_cell is not NULL to handle race conditions with
                     SC aborts
05/21/02   gw        Removed reference to bs.expected_power.
02/20/02   dp        Changed abort_drivers to switch on rx.request to fix a bug
                     in aborting the TCH
02/07/02   gw        Improved abort mechanism.  Moved code for FCB and SB rx to
                     l1_sc_acq.c.
02/05/02   JC        Temporary changes to support first cut AGC.
02/01/02   mk        Updated to support latest afc/tt algorithms.
12/21/01   gw        Changes to support DRX scheduling.
12/18/01   dp        Changes to support SDCCH SC functionality and SC aborts
10/24/01   dp        Implemented surround cell functionality for the TCH
10/16/01   gw        Mainlined FEATURE_FINDBCCH.  Used OLD_SCE to featurize
                     references to old SB reception code that has been
                     removed.  Added support for pscan using message layer
                     power measurement functions.
10/10/01   gw        Added support for message layer power measurements and
                     Ncell reception.
09/25/01   mk/gw     General SCE cleanup. Mainlined MONITOR_DEBUG, added
                     PScan/Ncell support using FINDBCCH and MULTIMON.
09/14/01   JC        Added some std QCT template and fixed include file list.
08/20/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
08/20/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "l1_drx.h"
#include "l1_sc.h"
#include "l1_sc_int.h"
#include "l1_log.h"     /* For L1 NV logging */
#include "l1_utils.h"   /* For gl1_get_FN() */
#include "l1_fm.h"

#include "geran_dual_sim.h"
#include "gpl1_grm_intf.h"

#include "l1_task.h"
#include "gpl1_gprs_utils.h"

#ifdef DEBUG_SCE_RX_LOGGING
#include "rex.h"
#endif


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
#define MIN_QBIT_DRIFT_UPDATE                     4
#define MAX_QBIT_DRIFT_UPDATE                     100
#define MAX_FRAME_DURATION                        13002 /* 60 seconds */

/*===========================================================================

                     Prototypes for External Functions

===========================================================================*/

/*===========================================================================

                     Types and Local Storage

===========================================================================*/

typedef enum
{
   StateNull,
   StateFCB,
   StateSB,
   StateBCCh,
   StateFCBTCh,
   StateSBTCh,
   StateFCBTrans,
   StateSBTrans,
   StateBCChTrans,
   StateCallbackPending,
   StateAborting
} receive_state_T;

typedef struct
{

struct
{
   void            (* done )( cell_T*, gas_id_t gas_id );
   receive_state_T   state;
   receive_state_T   aborted_state;
   boolean           abort;
   uint32            id;
   boolean           start_wcdma_receive;
} rx;

boolean ncell_block_start;

}l1_sc_rcv_data_t;


#ifdef FEATURE_GPRS_GBTA
extern boolean ignore_xcch;
#endif /* FEATURE_GPRS_GBTA */


static l1_sc_rcv_data_t  l1_sc_rcv_data[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                    Prototypes for Local Functions

===========================================================================*/
static void    receive_callback( void* id, gas_id_t gas_id );
static boolean start_receive( cell_T* cell, void(*done)(cell_T*, gas_id_t gas_id),gas_id_t gas_id );


/*===========================================================================

                     EXTERNALLY AVAILABLE FUNCTIONS

===========================================================================*/
void l1_sc_receive_init( gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   l1_sc_globals_ptr->receive_in_progress                = FALSE;
   l1_sc_rcv_data_ptr->rx.state                          = StateNull;
   l1_sc_rcv_data_ptr->rx.done                           = NULL;
   l1_sc_rcv_data_ptr->rx.abort                          = FALSE;
   l1_sc_rcv_data_ptr->rx.id                             = 0;
   l1_sc_rcv_data_ptr->rx.aborted_state                  = l1_sc_rcv_data_ptr->rx.state;
   l1_sc_rcv_data_ptr->rx.start_wcdma_receive            = FALSE;

#ifdef DEBUG_SCE_RX_LOGGING
   sce_rx_log_event( EVE_INIT, 0,0,0,gas_id );
#endif
}

void l1_sc_receive_shutdown( void )
{
}

void l1_sci_tick_receive( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   ASSERT_INT();

   if (l1_sc_rcv_data_ptr->rx.abort)
   {
      abort_receive(gas_id);
      l1_sc_rcv_data_ptr->rx.aborted_state = l1_sc_rcv_data_ptr->rx.state;
      /*If already aborted and reset to StateNull in abort_receive, then dont overwrite this to Aborting*/
      if (l1_sc_rcv_data_ptr->rx.state != StateNull)
      {
        l1_sc_rcv_data_ptr->rx.state       = StateAborting;
      }

      l1_sc_rcv_data_ptr->rx.abort         = FALSE;
      return;
   }

   if (l1_sc_rcv_data_ptr->rx.start_wcdma_receive)
   {
      l1_sci_start_wcdma_srch(gas_id);
      l1_sc_rcv_data_ptr->rx.start_wcdma_receive = FALSE;
      return;
   }

   if (l1_sc_globals_ptr->wcdma_srch_in_progress)
   {
       //Call the tick function to abort any stuck searches
       l1_sc_wcdma_ded_tick_abort_stuck_srch(gas_id);
   }

   switch ( l1_sc_rcv_data_ptr->rx.state )
   {
      case StateNull:
      case StateCallbackPending:
         break;

      case StateFCB:
         l1_sci_tick_fcb(gas_id);
         break;

      case StateFCBTCh:
         l1_sci_tick_fcb_tch(gas_id);
         break;

      case StateFCBTrans:
         gpl1_gprs_sci_tick_fcb_trans(gas_id);
         break;

      case StateSB:
         l1_sci_tick_sb(gas_id);
         break;

      case StateSBTCh:
         l1_sci_tick_sb_tch(gas_id);
         break;

      case StateSBTrans:
         gpl1_gprs_sci_tick_sb_trans(gas_id);
         break;

      case StateBCCh:
         l1_sci_tick_bcch(gas_id);
         break;

      case StateBCChTrans:
         gpl1_gprs_sci_tick_bcch_trans(gas_id);
         break;

      case StateAborting:
         /* Some receive state machines need to
          * be ticked while aborting to let them
          * keep L1 awake.
          */
         switch (l1_sc_rcv_data_ptr->rx.aborted_state)
         {
            case StateFCB:
               l1_sci_tick_fcb(gas_id);
               break;

            case StateSB:
               l1_sci_tick_sb(gas_id);
               break;

            case StateBCCh:
               l1_sci_tick_bcch(gas_id);
               break;

            default:
               /* do nothing */
               break;
         }
         break;

      default:
         MSG_GERAN_ERROR_1_G("Unknown state %d", l1_sc_rcv_data_ptr->rx.state);
         break;
   }
}

void l1_sci_post_tick_receive( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   ASSERT_INT();

   switch ( l1_sc_rcv_data_ptr->rx.state )
   {
      case StateFCB:
         l1_sci_post_tick_fcb(gas_id);
         break;

      case StateSB:
         l1_sci_post_tick_sb(gas_id);
         break;

      case StateBCCh:
         l1_sci_post_tick_bcch(gas_id);
         break;

      case StateCallbackPending:
         if (l1_sc_globals_ptr->mode == L1SCModeIdle)
         {
             receive_callback((void*)(int32)(l1_sc_rcv_data_ptr->rx.id ), gas_id );


             /* The receive callback function could schedule another receive operation.
                For example, after an Ncell SB has been received for 1 cell, it might be
                time for scheduling Ncell BCCH for another cell. As such, state transitions
                may happen in the above call to receive_callback. We check here for the
                state change and call appropriate functions.
             */
             switch(l1_sc_rcv_data_ptr->rx.state)
             {
                case StateSB:
                   l1_sci_tick_sb(gas_id);
                   break;

                case StateBCCh:
                   l1_sci_tick_bcch(gas_id);
                   break;

                default:
                   break;
             }
         }
         else
         {
             MSG_GERAN_ERROR_1_G("Function called in the wrong SCE mode: %d",
                                         l1_sc_globals_ptr->mode);
         }
         break;

      default:
         /* Do nothing here */
         break;
    }

}
void l1_sci_receive_done( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

#ifdef DEBUG_SCE_RX_LOGGING
   sce_rx_log_event( EVE_DONE_ISR,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)l1_sc_rcv_data_ptr->rx.id,
                     (uint32)l1_sc_rcv_data_ptr->rx.start_wcdma_receive,gas_id );
#endif


   /* Cover the case where Dedicated Mode calls
    * FTN before l1_sci_tick_receive().
    *
    * TODO - Fix Dedicated Mode.
    */
   if ( (l1_sc_rcv_data_ptr->rx.abort) || (l1_sc_rcv_data_ptr->rx.state == StateAborting) )
   {
      MSG_GERAN_ERROR_0_G("SCE rx done after abort.");
      l1_sc_globals_ptr->receive_in_progress = FALSE;
      L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
      l1_sc_rcv_data_ptr->rx.state = StateNull;
      l1_sc_rcv_data_ptr->rx.abort = FALSE;
   }
   else
   {
      l1_sc_rcv_data_ptr->rx.state = StateCallbackPending;
      if (l1_sc_globals_ptr->mode != L1SCModeIdle)
      {
         l1_scdrv_call_in_task( receive_callback, (void*)(int32)(l1_sc_rcv_data_ptr->rx.id ), gas_id );
      }
   }

}

void l1_sc_abort_receive( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   ASSERT_TASK();

  GL1_ISR_LOCK(gas_id);

#ifdef DEBUG_SCE_RX_LOGGING
   sce_rx_log_event( EVE_ABORT,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)l1_sc_rcv_data_ptr->rx.id,
                     (uint32)l1_sc_rcv_data_ptr->rx.start_wcdma_receive,gas_id );
#endif

   switch (l1_sc_rcv_data_ptr->rx.state)
   {
      case StateNull:
         l1_sc_globals_ptr->receive_in_progress = FALSE;
         L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
         MSG_GERAN_HIGH_1_G("Aborting SCE rx in unexpected state %d.", l1_sc_rcv_data_ptr->rx.state);
         break;

      case StateAborting:
        l1_sc_globals_ptr->receive_in_progress = FALSE;
        L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
        MSG_GERAN_ERROR_0_G("Multiple aborts.");
        break;

      case StateCallbackPending:
         /* Receive is done but task hasn't
          * processed callback yet.
          */
         l1_sc_globals_ptr->receive_in_progress = FALSE;
         L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
         l1_sc_rcv_data_ptr->rx.state = StateNull;
         MSG_GERAN_HIGH_0_G("Aborting SCE rx with callback pending.");
         break;

      default:
         l1_sc_rcv_data_ptr->rx.abort = TRUE;
         break;
   }

   l1_sc_rcv_data_ptr->rx.id++;

   l1_sc_rcv_data_ptr->rx.start_wcdma_receive = FALSE;

  GL1_ISR_UNLOCK(gas_id);
}

void l1_sc_abort_receive_callback(gas_id_t gas_id)
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

#ifdef DEBUG_SCE_RX_LOGGING
   sce_rx_log_event( EVE_ABORT_CB,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)l1_sc_rcv_data_ptr->rx.id,
                     (uint32)l1_sc_rcv_data_ptr->rx.start_wcdma_receive,gas_id );
#endif

   if ( (l1_sc_rcv_data_ptr->rx.state != StateAborting) && (l1_sc_rcv_data_ptr->rx.abort != TRUE) )
   {
      MSG_GERAN_ERROR_1_G("Unexpected state %d.", l1_sc_rcv_data_ptr->rx.state);
   }

   l1_sc_rcv_data_ptr->rx.state         = StateNull;
   l1_sc_rcv_data_ptr->rx.aborted_state = StateNull;
   l1_sc_globals_ptr->receive_in_progress = FALSE;
   L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
}

void l1_sc_receive_fcb( cell_T* cell, void ( *done )( cell_T*, gas_id_t gas_id ), gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

  GL1_ISR_LOCK(gas_id);
   if (start_receive( cell, done, gas_id ))
   {
      l1_sc_rcv_data_ptr->rx.state = StateFCB;
      l1_sc_start_fcb( cell, gas_id );
   }
  GL1_ISR_UNLOCK(gas_id);
}

void l1_sc_receive_fcb_tch( cell_T* cell, void ( *done )( cell_T*,gas_id_t gas_id ),gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_scenario_status_e status;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

  /* FR54762 Inform MCPM about this scenario and check if we can go ahead with scheduling Ncell FCB in TCH */
  gl1_concman_config_params.type = CMGR_TRANSIENT_START;
  gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
  gl1_concman_config_params.update_scenario = 0;
  gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
  gl1_concman_config_params.start_timestamp = 0;
  gl1_concman_config_params.duration_msec = 0;
  gl1_concman_config_params.flow_ctrl_time_msec = 0;
  status = gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
  MSG_GERAN_HIGH_1_G("***sending scenario request to MCPM : status : %d", status);


  GL1_ISR_LOCK(gas_id);
   if(status == CMGR_ACCEPTED)
  {
   if (start_receive( cell, done, gas_id ))
   {
      l1_sc_rcv_data_ptr->rx.state = StateFCBTCh;
      l1_sc_start_fcb_tch( cell, gas_id );
   }
  }
  else
  MSG_GERAN_HIGH_1_G("NCELL Tone denied by MCPM for ARFCN: %d", cell->bs.arfcn.num);
  GL1_ISR_UNLOCK(gas_id);
}


void gpl1_gprs_sc_receive_fcb_trans( cell_T* cell, void ( *done )( cell_T*,gas_id_t gas_id ),gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];
   boolean trm_band_req_ok = TRUE;
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_scenario_status_e status;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);
   
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   idle_data_T* l1_idle_data = get_ptr_to_idle_data( gas_id);
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   sys_band_T serving_cell_band = gl1_get_band(l1_idle_data->campedon_cell_ARFCN);
#endif

   /* FR54762 Inform MCPM about this scenario and check if we can go ahead with scheduling Ncell FCB in TCH */
   gl1_concman_config_params.type = CMGR_TRANSIENT_START;
   gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
   gl1_concman_config_params.update_scenario = 0;
   gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
   gl1_concman_config_params.start_timestamp = 0;
   gl1_concman_config_params.duration_msec = 0;
   gl1_concman_config_params.flow_ctrl_time_msec = 0;
   status = gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
   MSG_GERAN_HIGH_1_G("***sending scenario request to MCPM : status : %d", status);

   /*For DRDSDS any non-serving cell band sc activities must be requested through TRM*/
   /*Only send the request if the neighbour cell band is different to the serving cell band.*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*This same check is done inside start_receive() function
    Actual activity will not start without this condition being met*/
  if (!(l1_sc_globals_ptr->receive_in_progress))
  {
    if ( gl1_drdsds_enabled( gas_id ) &&
         (serving_cell_band != cell->bs.arfcn.band)
       )
    {
           MSG_GERAN_MED_0_G(GRM_TM_HDR"Attempting band Reg for FCH");
         /*FCH read is on another band so band must be registered with TRM for DR-DSDS.
           TRM band denial FCH failure.*/
         trm_band_req_ok = grm_check_ifs_band_reg(SC_FCH_SCH, cell->bs.arfcn.band, GRM_FRM_REQ_FCH,DIVERSITY_FALSE, gas_id );

    }
#endif

    if (trm_band_req_ok)
    {
       GL1_ISR_LOCK(gas_id);
       if(status == CMGR_ACCEPTED)
       {
       if (start_receive( cell, done, gas_id ))
       {
          l1_sc_rcv_data_ptr->rx.state = StateFCBTrans;
          gpl1_gprs_sc_start_fcb_trans( cell, gas_id );

#ifdef FEATURE_GPRS_GBTA
          ignore_xcch = TRUE;
#endif /* FEATURE_GPRS_GBTA */

       }
       }
	   else
	   MSG_GERAN_HIGH_1_G("Ncell Tone detect denied by MCPM for  ARFCN = %d", cell->bs.arfcn.num);
       GL1_ISR_UNLOCK(gas_id);
    }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    else
    {
#ifdef DEBUG_DRDSDS
       MSG_GERAN_MED_2_G(GL1_DRDS_DBG_HDR"Ncell Tone detect denied by TRM for  ARFCN = %d, BAND %d",
                        cell->bs.arfcn.num, cell->bs.arfcn.band);
#endif
    }
  }/*if !l1_sc_globals_ptr->receive_in_progress*/
  else
  {
#ifdef DEBUG_SCE_RX_LOGGING
     sce_rx_log_event( EVE_START_RX,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)done,
                     (uint32)l1_sc_globals_ptr->receive_in_progress,gas_id );
#endif
     MSG_GERAN_ERROR_1_G("SCE rx in progress %d", l1_sc_rcv_data_ptr->rx.state);
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
}


void l1_sc_receive_sb( cell_T* cell, void ( *done )( cell_T*,gas_id_t gas_id ),gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

  GL1_ISR_LOCK(gas_id);
   if (start_receive( cell, done, gas_id ))
   {
      l1_sc_rcv_data_ptr->rx.state = StateSB;
      l1_sc_start_sb( cell, gas_id );
   }
  GL1_ISR_UNLOCK(gas_id);
}

void l1_sc_receive_sb_tch( cell_T* cell, void ( *done )( cell_T*,gas_id_t gas_id ),gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_scenario_status_e status;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

   /* FR54762 Inform MCPM about this scenario and check if we can go ahead with scheduling Ncell FCB in TCH */
   gl1_concman_config_params.type = CMGR_TRANSIENT_START;
   gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
   gl1_concman_config_params.update_scenario = 0;
   gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
   gl1_concman_config_params.start_timestamp = 0;
   gl1_concman_config_params.duration_msec = 0;
   gl1_concman_config_params.flow_ctrl_time_msec = 0;
   status = gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
   MSG_GERAN_HIGH_1_G("***sending scenario request to MCPM : status : %d", status);

  GL1_ISR_LOCK(gas_id);
  if(status == CMGR_ACCEPTED)
  {
   if (start_receive( cell, done, gas_id ))
   {
      l1_sc_rcv_data_ptr->rx.state = StateSBTCh;
      l1_sc_start_sb_tch( cell, gas_id );
   }
  }
  else
  MSG_GERAN_HIGH_1_G("Ncell SCH denied by MCPM for ARFCN : %d", cell->bs.arfcn.num);
  GL1_ISR_UNLOCK(gas_id);
}

void gpl1_gprs_sc_receive_sb_trans( cell_T* cell, void ( *done )( cell_T*,gas_id_t gas_id ),gas_id_t gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   idle_data_T* l1_idle_data = get_ptr_to_idle_data( gas_id);
   sys_band_T serving_cell_band = gl1_get_band(l1_idle_data->campedon_cell_ARFCN);
#endif
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];
   boolean trm_band_req_ok =  TRUE;
  

   /*Any planned FCH or SCH activities must be check against serving cell band.
     If the band is different then BAND must be registred with TRM. If the BAND request is denied then
     the NCell activity can not proceed in the idle frame*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*This same check is done inside start_receive() function
    Actual activity will not start without this condition being met*/
   if (!(l1_sc_globals_ptr->receive_in_progress))
   {
     /*Only send the request if the neighbour cell band is different to the serving cell band.*/
     if ( gl1_drdsds_enabled( gas_id ) &&
          ( serving_cell_band != cell->bs.arfcn.band )
        )
     {
        MSG_GERAN_MED_0_G(GRM_TM_HDR"Attempting band Reg for SCH");
        /*BCCH read is on another band so band must be registered with TRM for DR-DSDS.
          TRM band denial will result in TBF suspend cancelation.*/
        trm_band_req_ok = grm_check_ifs_band_reg(SC_FCH_SCH, cell->bs.arfcn.band, GRM_FRM_REQ_SCH, DIVERSITY_FALSE, gas_id );

     }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

     if ( trm_band_req_ok )
     {
       GL1_ISR_LOCK(gas_id);
       if (start_receive( cell, done, gas_id ))
       {
           l1_sc_rcv_data_ptr->rx.state = StateSBTrans;
           gpl1_gprs_sc_start_sb_trans( cell, gas_id );
       }
       GL1_ISR_UNLOCK(gas_id);
     }
     else
     {
#ifdef DEBUG_DRDSDS
       MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR" SCH schedule denied by TRM for arfcn %d band %d",
                        cell->bs.arfcn.num, cell->bs.arfcn.band);
#endif
     }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   }
   else
   {
#ifdef DEBUG_SCE_RX_LOGGING
     sce_rx_log_event( EVE_START_RX,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)done,
                     (uint32)l1_sc_globals_ptr->receive_in_progress,gas_id );
#endif
      MSG_GERAN_ERROR_1_G("SCE rx in progress %d", l1_sc_rcv_data_ptr->rx.state);
   }
#endif/* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

void l1_sc_receive_bcch( cell_T* cell, void ( *done )( cell_T*,gas_id_t gas_id ),gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

  GL1_ISR_LOCK(gas_id);
   if (start_receive( cell, done, gas_id ))
   {
      l1_sc_rcv_data_ptr->rx.state = StateBCCh;
      l1_sc_start_bcch( cell, gas_id );
   }
  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  gpl1_gprs_sc_receive_bcch_trans

DESCRIPTION
  Receives BCCH/CCCH block in the transfer mode.

DEPENDENCIES
  None

PARAMETERS

RETURN VALUE
  True  : if start receving block

SIDE EFFECTS
  None
===========================================================================*/
boolean gpl1_gprs_sc_receive_bcch_trans( cell_T* cell,
                                         void ( *done )( cell_T*, gas_id_t gas_id ),
                                         gl1_msg_chan_type chan_type,
                                         async_ccch_type_t async_ccch_type,
                                         gas_id_t gas_id )
{
  l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];
  boolean start_bcch = TRUE;
  
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  sys_band_T bcch_band = gl1_get_band(cell->bs.arfcn);
  idle_data_T* l1_idle_data = get_ptr_to_idle_data( gas_id);
  sys_band_T serving_cell_band = gl1_get_band(l1_idle_data->campedon_cell_ARFCN);
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

  /*if receive in progress then, gpl1_gprs_sc_start_bcch_trans will not be called 
    so cb's which release the bands will not be there*/
  if ( !(l1_sc_globals_ptr->receive_in_progress) )
  {
  
    /* Called for both channel type : CCCH or BCCH
       BCCH: Perform band registration. */
    if (gl1_drdsds_enabled( gas_id ) &&
        (GL1_MSG_BCCH == chan_type) &&
        (serving_cell_band != bcch_band)
        ) 
    {
      /*BCCH read is on another band so band must be registered with TRM for DR-DSDS. 
        TRM band denial will result in TBF suspend cancelation. */
       start_bcch = grm_check_ifs_band_reg( SC_BCCH,
                                            bcch_band, 
                                            data_ptr->table[G2X_TA_PRI_SERV_BCCH].duration, 
                                            DIVERSITY_FALSE,
                                            gas_id );
    }
#endif  /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

    if (TRUE == start_bcch) 
    {
      GL1_ISR_LOCK(gas_id); 
      if(start_receive(cell, done, gas_id))
      {
        l1_sc_rcv_data_ptr->rx.state = StateBCChTrans;
        gpl1_gprs_sc_start_bcch_trans( cell, chan_type,async_ccch_type,gas_id );
      }
      else
      {
        start_bcch = FALSE;
      }
      GL1_ISR_UNLOCK(gas_id);
    }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  }/*if l1_sc_globals_ptr->receive_in_progress*/
  else
  {
    start_bcch = FALSE;
  }
#endif
  return start_bcch;
}

void l1_sci_tick_idle(gas_id_t gas_id )
{
   ASSERT_INT();

   l1_sci_tick_receive(gas_id);

   l1_sci_tick_wcdma(gas_id);

}

void l1_sci_post_tick_idle(gas_id_t gas_id)
{
   ASSERT_INT();

   l1_sci_post_tick_receive(gas_id);

   l1_sci_post_tick_wcdma(gas_id);


}

void gpl1_gprs_sci_tick_idle(uint32 available_frames_gprs, gas_id_t gas_id )
{
   ASSERT_INT();

   l1_fm_tick(available_frames_gprs, gas_id);
   l1_sci_tick_receive(gas_id);

}


/*===========================================================================

FUNCTION  gl1_is_ncell_block_start

DESCRIPTION
  This function allows the logical channel code to determine if the
  surround cell engine or any other module has set the block start
  indicator for the ncell handler

DEPENDENCIES
  None

PARAMETERS

RETURN VALUE
  True  : if block start indicator set
  False : if block start indicator has not been set

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_is_ncell_block_start(gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

    return (l1_sc_rcv_data_ptr->ncell_block_start);
}

/*===========================================================================

FUNCTION  gl1_set_ncell_block_start

DESCRIPTION
  This function sets the block start
  indicator for the ncell handler

DEPENDENCIES
  None

PARAMETERS
    Value which ncell_block_start should be set to

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_set_ncell_block_start( boolean value,gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   l1_sc_rcv_data_ptr->ncell_block_start = value;
}

/*===========================================================================

FUNCTION l1_sci_is_op_in_progress

DESCRIPTION
  Use to check if the SCE has an operation in progress.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE  - SCE has operation in progress (don't change L1 state)
  FALSE - SCE is idle (OK to change L1 state)

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sci_is_op_in_progress( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   boolean monscan_meas_in_progress      = l1_sci_monscan_meas_in_progress(gas_id);

   MSG_GERAN_HIGH_3_G(" mon %d wsrch %d rcv %d ",
           (l1_sc_globals_ptr->monscan_in_progress || monscan_meas_in_progress),
           (l1_sc_globals_ptr->wcdma_srch_in_progress),
            l1_sc_globals_ptr->receive_in_progress
           );

   return ( l1_sc_globals_ptr->monscan_in_progress    ||
            monscan_meas_in_progress  ||
            l1_sc_globals_ptr->wcdma_srch_in_progress ||
            l1_sc_globals_ptr->receive_in_progress    );
}


/*===========================================================================

FUNCTION l1_sci_is_op_in_progress_gbta

DESCRIPTION
  Use to check if the SCE has an operation in progress. without Monitors

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE  - SCE has operation in progress (don't change L1 state)
  FALSE - SCE is idle (OK to change L1 state)

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sci_is_op_in_progress_gbta( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr;
   if( GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id )
   {
     return TRUE;
   }
   l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);
   MSG_GERAN_MED_2_G(" wcdma_srch_in_progress %d rcv %d ",
            l1_sc_globals_ptr->wcdma_srch_in_progress,
            l1_sc_globals_ptr->receive_in_progress
           );

   return ( l1_sc_globals_ptr->wcdma_srch_in_progress);
} //l1_sci_is_op_in_progress_gbta

/*===========================================================================

FUNCTION l1_sci_meas_in_progress_gbta

DESCRIPTION
  Returns TRUE if meas_in_progress is due to BCCH Read

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean l1_sci_meas_in_progress_gbta( gas_id_t gas_id )
{
  l1_sc_rcv_data_t *l1_sc_rcv_data_ptr;
  boolean meas_in_progress = FALSE;
  if( GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id )
  {
    return TRUE;
  }
  l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

  if(l1_sc_rcv_data_ptr->rx.state != StateNull)
  {
    MSG_GERAN_HIGH_1_G( "PTM NCELL state: %d" ,l1_sc_rcv_data_ptr->rx.state);

#ifdef FEATURE_GPRS_GBTA
    if(l1_sc_rcv_data_ptr->rx.state == StateFCBTrans)
    {
      ignore_xcch = TRUE;
    }
#endif /* FEATURE_GPRS_GBTA */

  }
  if(l1_sc_rcv_data_ptr->rx.state == StateBCChTrans)
  {
    return TRUE;
  }
  return(meas_in_progress);
}

/*===========================================================================

FUNCTION  l1_sci_rcv_state_debug_msg()

DESCRIPTION
  Print a debug message showing the state of the receive module.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_rcv_state_debug_msg( gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   MSG_GERAN_ERROR_1_G("l1_sc_rcv_data_ptr->rx: state=%d", l1_sc_rcv_data_ptr->rx.state);
}

void l1_sc_start_wcdma_receive( gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   l1_sc_rcv_data_ptr->rx.start_wcdma_receive = TRUE;
}

void l1_sc_stop_wcdma_receive( gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   l1_sc_rcv_data_ptr->rx.start_wcdma_receive = FALSE;
}


/*===========================================================================

                             LOCAL FUNCTIONS

===========================================================================*/
static void receive_callback( void* id, gas_id_t gas_id )
{
   void            (* done )( cell_T*, gas_id_t gas_id );
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   ASSERT_TASK();

#ifdef DEBUG_SCE_RX_LOGGING
   sce_rx_log_event( EVE_DONE_TASK,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)l1_sc_rcv_data_ptr->rx.id,
                     (uint32)id,gas_id );
#endif

  GL1_ISR_LOCK(gas_id);
   /* Prevent rx.done from being called more than once per receive.
    * Need to do it this way since rx.done function may itself set
    * new value for rx.done.
    */
   done     = l1_sc_rcv_data_ptr->rx.done;
   l1_sc_rcv_data_ptr->rx.done  = NULL;

   l1_sc_rcv_data_ptr->rx.state = StateNull;
  GL1_ISR_UNLOCK(gas_id);

   /* Only process callback if id's match. (If id's don't
    * match the receive must have been aborted.)
    */
   if ((uint32)id == l1_sc_rcv_data_ptr->rx.id)
   {
      l1_sc_globals_ptr->receive_in_progress = FALSE;
      L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);

      /* Check for NULL pointers */
      if (done && l1_sc_globals_ptr->receiving_cell )
      {
         done( l1_sc_globals_ptr->receiving_cell,gas_id );
      }
      else
      {
         MSG_GERAN_ERROR_2_G("Not calling rx.done(). %d %d", done, l1_sc_globals_ptr->receiving_cell);
      }
   }
}

static boolean start_receive( cell_T* cell, void(*done)(cell_T*, gas_id_t gas_id),gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

#ifdef DEBUG_SCE_RX_LOGGING
   sce_rx_log_event( EVE_START_RX,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)done,
                     (uint32)l1_sc_globals_ptr->receive_in_progress,gas_id );
#endif

   if ( l1_sc_globals_ptr->receive_in_progress )
   {
      MSG_GERAN_ERROR_1_G("SCE rx in progress %d", l1_sc_rcv_data_ptr->rx.state);
      return FALSE;
   }

   l1_sc_globals_ptr->receive_in_progress = TRUE;
   L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
   l1_sc_globals_ptr->receiving_cell      = cell;

   l1_sc_rcv_data_ptr->rx.abort = FALSE;
   l1_sc_rcv_data_ptr->rx.done  = done;

   MSG_GERAN_HIGH_3_G("start_receive: cell:%d, rcv %d rcv_state %d",
                         l1_sc_globals_ptr->receiving_cell->bs.arfcn.num, 
                         l1_sc_globals_ptr->receive_in_progress ,
                         l1_sc_rcv_data_ptr->rx.state);
   return TRUE;
}

void abort_receive( gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

#ifdef DEBUG_SCE_RX_LOGGING
   sce_rx_log_event( EVE_ABORT_ISR,
                     (uint32)l1_sc_rcv_data_ptr->rx.state,
                     (uint32)l1_sc_rcv_data_ptr->rx.id,
                     (uint32)l1_sc_rcv_data_ptr->rx.start_wcdma_receive,gas_id );
#endif

   switch ( l1_sc_rcv_data_ptr->rx.state )
   {
      case StateNull:
      case StateCallbackPending:
      case StateAborting:
         MSG_GERAN_ERROR_1_G("Aborting SCE rx when none in progress. %d", l1_sc_rcv_data_ptr->rx.state);
         break;

      case StateFCB:
         l1_sci_abort_fcb(gas_id);
         break;

      case StateFCBTrans:
      case StateFCBTCh:
         l1_sci_abort_fcb_tch(gas_id);
         break;

      case StateSB:
         l1_sci_abort_sb(gas_id);
         break;

      case StateSBTrans:
      case StateSBTCh:
         l1_sci_abort_sb_tch(gas_id);
         break;

      case StateBCChTrans:
      case StateBCCh:
         l1_sci_abort_bcch(gas_id);
         break;

      default:
         MSG_GERAN_ERROR_1_G("Unknown state %d", l1_sc_rcv_data_ptr->rx.state);
         break;
   }
}

#ifdef DEBUG_SCE_RX_LOGGING
void sce_rx_log_event( rx_event_T event,
                              uint32     data1,
                              uint32     data2,
                              uint32     data3,
                              gas_id_t   gas_id )
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   l1_nv_log_ptr->sce_rx_log_buf[l1_nv_log_ptr->sce_rx_log_buf_index].FN     = gl1_get_FN( gas_id );
   l1_nv_log_ptr->sce_rx_log_buf[l1_nv_log_ptr->sce_rx_log_buf_index].in_ISR = rex_is_in_irq_mode();
   l1_nv_log_ptr->sce_rx_log_buf[l1_nv_log_ptr->sce_rx_log_buf_index].event  = event;
   l1_nv_log_ptr->sce_rx_log_buf[l1_nv_log_ptr->sce_rx_log_buf_index].data1  = data1;
   l1_nv_log_ptr->sce_rx_log_buf[l1_nv_log_ptr->sce_rx_log_buf_index].data2  = data2;
   l1_nv_log_ptr->sce_rx_log_buf[l1_nv_log_ptr->sce_rx_log_buf_index].data3  = data3;

   if (++l1_nv_log_ptr->sce_rx_log_buf_index >= SCE_RX_LOG_LENGTH)
   {
      l1_nv_log_ptr->sce_rx_log_buf_index = 0;
   }
}
#endif
/*===========================================================================

FUNCTION l1_sci_meas_in_progress

DESCRIPTION
  Returns TRUE if meas_in_progress is due to BCCH Read,NCELL SCH/FCCH

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean l1_sci_meas_in_progress( gas_id_t gas_id )
{
  l1_sc_rcv_data_t *l1_sc_rcv_data_ptr;
  boolean meas_in_progress = FALSE;
  if( GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id )
  {
    return TRUE;
  }
  l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];

   if((l1_sc_rcv_data_ptr->rx.state == StateSBTrans)||(l1_sc_rcv_data_ptr->rx.state == StateFCBTrans)||(l1_sc_rcv_data_ptr->rx.state == StateBCChTrans)
     ||(l1_sc_rcv_data_ptr->rx.state == StateAborting))
  {
    MSG_GERAN_HIGH_1_G( "PTM NCELL state: %d" ,l1_sc_rcv_data_ptr->rx.state);
    return TRUE;
  }
  return(meas_in_progress);
}


/*===========================================================================

FUNCTION l1_sci_reset_state_machine

DESCRIPTION
 Reset rx.state and recive in progress

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void l1_sci_reset_state_machine( gas_id_t gas_id )
{
   l1_sc_rcv_data_t *l1_sc_rcv_data_ptr = &l1_sc_rcv_data[gas_id];
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   l1_sc_rcv_data_ptr->rx.state = StateNull;
   l1_sc_globals_ptr->receive_in_progress = FALSE;
   L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_RCV,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
}

/*===========================================================================

FUNCTION l1_sc_apply_timing_drift

DESCRIPTION
 Calculate the qbit drift based on the below formula and apply
 this correction to the given neighbor cell
 Qs = ([XO(SCELL) - XO(NCELL)] *Frame_diff * 5000)/(1000 * 1000000)

PARAMS
  None

DEPENDENCIES

RETURN VALUE
  TRUE - if qbit lag has been updated
  FALSE - if lag is not updated

===========================================================================*/
boolean l1_sc_apply_timing_drift( gas_id_t gas_id )
{
  boolean drift_applied = FALSE;
#ifdef FEATURE_GSM_NCELL_BCCH_DECODE_IMPROVEMENT
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  if ( GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_DISABLE_FR) )
  {
    MSG_GERAN_HIGH_0_G("FR disabled");
    return drift_applied;
  }

  if ( l1_sc_globals_ptr->receiving_cell )
  {
    cell_T* cell = l1_sc_globals_ptr->receiving_cell;

    if ( cell->afc_info.valid )
    {
      int32 abs_qs_to_correct, qs_to_correct;
      int32 frame_duration = SUB_FN( gl1_get_FN( gas_id ), cell->bs.fn_when_read );
      int32 xo_freq_difference = cell->bs.scell_freq_err - cell->afc_info.afc_freq;
	  
      MSG_GERAN_HIGH_2_G("Serving XO %d ncell ncell afc_freq %d freq_error %d",cell->bs.scell_freq_err , cell->afc_info.afc_freq );
	  
      /* Qs = ( XO_diff * Frame_diff * 5000)/(1000 * 1000000) */
      qs_to_correct = ((xo_freq_difference * frame_duration * 5)/1000000);

      abs_qs_to_correct = abs(qs_to_correct);

      MSG_GERAN_HIGH_3_G("frame_diff %d XO_dif %d qs_to_correct %d",frame_duration, xo_freq_difference, qs_to_correct);

      /* don't update if qbit drift is too large and time elapsed is beyond 60 seconds */
      if ( ( frame_duration < MAX_FRAME_DURATION ) 
           && ( abs_qs_to_correct > MIN_QBIT_DRIFT_UPDATE ) && ( abs_qs_to_correct < MAX_QBIT_DRIFT_UPDATE ) 
          )
      {
        int16 old_lag = cell->bs.qbit_lag;

        l1_sc_adjust_lag( &(cell->bs), qs_to_correct );
        cell->bs.fn_when_read = gl1_get_FN( gas_id );
        drift_applied = TRUE;

        MSG_GERAN_HIGH_4_G("Adjusted %dqs for cell %d: updated qbit_lag from %d to %d",
                                    qs_to_correct, cell->bs.arfcn.num, old_lag, cell->bs.qbit_lag);
      }
      /* something is wrong, qbit drift should not be this high, re-set the frame */
      else if ( abs_qs_to_correct >= MAX_QBIT_DRIFT_UPDATE )
      {
        cell->bs.fn_when_read = gl1_get_FN( gas_id );
      }
    }
  }
#endif /* FEATURE_GSM_NCELL_BCCH_DECODE_IMPROVEMENT */

  return drift_applied;
}





