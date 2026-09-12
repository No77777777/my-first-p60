/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     L1 ISR DUAL SIM MODE FIND BCCH STATE PROCESSING

GENERAL DESCRIPTION


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dual_sim_find_bcch.c#4 $
$DateTime: 2020/02/06 02:36:07 $ $Author: pwbldsvc $

when          who    what, where, why
--------   --------  ----------------------------------------------------------
04/02/20   nv		CR2615257 Reverting the hack made to handle L1_TIME_LINE_DELAY scenario in HA as part of FR:51554
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
11/6/18       nsa    CR2255905 Removing setting TRM status to NULL in multisim reselection state machine
09/12/16      akm    CR1097283 No TRM reservation for BCCH for SI decode
11/04/17      rv     CR2031827 Excessive F3 log reduction for Oppo
10/03/17      ksb    CR2016325 Allow change state only after QTA cleanup is done 
24/01/17      ksb    CR1113403 Changing the prio to CM_INV for 4th attempt of BCCH decode so that it will have better chance to win TRM during BCCHlist decode 
31/10/17      sp     CR2125524 Prio Change to CHM_INV if bcch_attempt_count > 4
03/10/17      sp     CR2115832 Avoid MCPM State transitions in FIND_BCCH if EMBMS is Active on other sub (to utilize available gaps effectively)
12/01/18      rv     CR2170840: Fix for GSM TRM priority is lower then 1x priority during ACQUISITION
27/01/17   cws       CR1115987 Delay the turn tx off when transitioning from DTM to find BCCH in multisim
19/08/16   dp       CR1055018 Make FIND_BCCH_MODE QTA's check for invalid device granted from TRM
20/12/17      dp     CR1037929 make FIND_SCH mode use low MCPM power when waiting for TRM
15/07/16      sp     CR1038329: use correct freq in bcch_stop state RNN and clear freq. before adding more
16/06/16      sp     CR1030232: Use correct band for reserve at in find bcch states.
22/03/16      nm     CR905791 TRM U changes for Atlas(use unified TRM reasons)
08/12/16      ksb    CR1008155 Change TRM reason to CM during BCCH list decode after 2 attempts so that it has a better chance to get/retain lock when other SUB's idle activities are colliding with it
06/09/15      br     CR1063442 FR 38050: AsDiv in Acquisition for CSFB in multisimode.
13/06/16      mn     CR1025218 Release TRM before leaving the FIND_BCCH state and returning back to NULL.
16/05/16      mn     CR988646 Performing reservation for demod page inv when GL1 in reselection trm wait.
16/03/16      sk     CR990166 Request TRM with same sub-reason as in TRM reserve for bcch_decode during reselection
24/03/16      mk     CR970891 Reverting CR961866 & CR965869
23/3/16       sk     CR993161 Don't set frame duration & unlock callback in PSCAN & DECODE_BCCH_LIST STOPPED state 
                     if DS_ABORT has already set them to COMPLETE_PROCEDURE_DURATION & NULL respectively.
26/02/16      js     CR980305 Issue X2G startup while configuring RX/ Sending blcok IND to RR when TRM is not granted
19/02/16      bg     CR979066 No need band checking, when QTA for BCCH read has triggered in FIND BCCH state
18/02/16      bg     CR978003 Break after TRM wait state during reselection
12/02/16      sk     CR972980 GRM release in multi_sim_acq_bcch_list suspend mode if acquisition is already stopped
01/02/15      js     CR909145 FR 27391: GL1 should allow QBTA/QTA gaps in find_BCCH state to avoid LTA gaps
14/01/16      js     CR961866: Check for internal GRM status while handling DS_ABORT
15/12/15      dp     CR949655 Prevent state transitions out of FIND_BCCH state when in low power mode
03/12/15      dp     CR929648 reduce power usage when waiting for TRM during find_bcch
27/11/15      cjl    CR942511 Check TRM available before extending BCCH activity
10/09/15      sk     CR899847 GERAN changes to trigger QSH events and handler for debugging
22/09/15      mko    CR908272 Stores the sub trm reason while doing reservation for BCCH
16/09/15      cjl    CR908527 improve system procedures used for TRM access
02/09/15      zf     CR905869 Set trm_status to NULL after grm release
24/08/15      cjl    CR872907 Notify GRR of serving cell BCCH TRM denials
20/08/15      ap     CR849948 Race condition between handling MPH_SET_PRIORITY_IND and TRM grant callback 
07/08/15      cjl    CR877672 Handle priority inversion for TRM_RESELECT use cases
07/08/15      zf     CR887176 Handle trm_status NULL in PSCAN TRM_WAIT state
07/08/15      am     CR886694 Remove some part of CR864046
22/07/15      nk     CR876691 Avoid performing any TRM reservations through multisim reselection state machine if TRM resource lock is ON
21/07/15      cjl    CR876903 Use system procedure types for TRM subreasons 
24/07/15      am     CR878969 Assign pri when trm is reserved for FIND_BCCH
30/06/15      cjl    CR864046 Use L1_MULTISIM_MINIMUM_FRAMES_FOR_BCCH_READ for bcch scheduling
21/07/15      cjl    CR876741 Replace TRM subreasons with system procedure types
21/07/15      nk     CR876776 Remove "else" condition placed as part of CR872598 to get X2G reserlections working
15/07/15      hd     CR872598 While doing multi sim reselection, treat GL1_TRM_RETAINED_FOR_ACCESS as GL1_TRM_GRANTED
24/06/15      ap     CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR 
30/06/15      cjl    CR863267 Treat GL1_TRM_RETAINED_FOR_ACCESS as GL1_TRM_GRANTED in *_TRM_WAIT states
10/06/15      cjl    CR851285 Fix RACH from idle state machine during RR procedure lock
26/05/15      am     CR840857 Ensure GL1 is awake to schedule SYS INFO REQ
06/01/15      js     CR822603  Don't update frame duration in pscan_trm_wait
29/05/15      smd    CR845859: Use the correct TRM reason to request lock for BCCH.
11/05/15      zf     CR836035: In FIND_BCCH TRM wait state, using TRM granted frames for frames duration.
11/05/15      pjr    CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
06/05/15      cjl    CR831599 Resolve persistent BCCH denial issue
30/04/15      ap     CR811057 FR22272 - TRM Unification  - Logging Changes
12/03/15      pa     CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
20/02/15      sjw    CR793380: adaptive TRM durations for acquisition mode
19/02/15      mko    CR776903 Don't do FW load if main command is L1_DEACTIVATE_COMMAND in PSCAN_STOPPED state
27/01/15      zf     CR787277: Initiatize frame_duration for ACQ_BCCH_LIST
24/01/15      mko    CR785110 GL1 must not register unlock callback if DS_ABORT is received during the FW_LOAD state
18/02/15      am     CR785921 Do TRM reserve & request for bcch readings in cell reselection state
17/02/15      am     CR793750 Correcting compilation warning 
06/01/15      am     CR390109 Improved DSDS cell reselection
12/12/14      pa     CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14      ws     CR768376 Q6 compile warning fixes
12/11/14      sjv    CR748612 Update band info for RnNE for reselection 
12/05/14      sk     CR662094 Ensure RANE doesnt get triggered when FW release pending
06/08/14      df     CR536059 Allow Asynchronous sleep for all idle activities
29/07/14      ap     CR695234: Changes for GL1's New Rach interface
04/07/14      cs     CR688003 GRM functions moved to gpl1_grm_intf.c
20/05/14      smd    CR662851: Release and request RF device again for RACH in SGLTE+G
16/06/14      cs/ch  CR651795 TRM Updated changes for DR-DSDS
29/05/14      cs/pa  CR660831 Use new trm_status definitions specific to GL1
08/05/14      sp     CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
08/05/14      nk     CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status. Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
10/05/14      rc     CR662550: Back out changes made in CR655548
06/05/14      smd    CR655548: Release and request RF device again for RACH in SGLTE+G
28/04/14      sk     CR654330 Ensure GL1 doesnt get stuck on TRM NULL while waiting for grant in find SCH state
08/04/14      br     CR644504 While aborting reselection procedure for rach,
                     don't Release the TRM if the main command is ACCESS command
15/04/14      sp     CR647343 Increase Clk speed for Power Scan
26/03/14      ab     CR638353 Vote for TX on when Transition from  FIND_BCCH to RACH
21/03/14      nm     CR635095 Fix the merge error bug went into CR619199
19/03/14      ws     CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
04/04/14      cah    CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
03/03/14      am     CR630494 Don't release FW in the same frame as FW setup
28/02/14      sjw    CR620569: adaptive unlock callback for acquisition
18/02/13      br     CR619199 Handle TRM_ABORTING  for multi sim TRM_WAIT state.
17/12/13      mko    CR586179 Applying the timing error reported in SCH reconfirm during reselection
05/12/13      am     CR581771 Ensure frequency correction to re-selection cell is always applied
29/11/13      sp     CR581776: Correct Suspension timer counter and release of trm in PowerScan.
08/01/14      cah    CR597414: fix for missing handling of L1_GSTMR_DEREG_SWAP_TO_VSTMR
13/12/13      ap     CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
20/11/13      nk     CR578950 Updating sub_sub_state when ENTER_NULL_COMMAND is processed
19/11/13      sp     CR574816 Correct Suspension timer counter and release of trm in BCCH list state.
28/11/13      sjw    CR578873: Priority Inversion Management for Multi-SIM (PIMMS)
11/20/13      nm     CR555931 Check for FW load status while starting Pscan even if TRM status is retained for access
06/10/13      js     CR552679 Handle TRM retained for access state in L1_MULTI_SIM_FIND_SCH_TRM_WAIT
17/10/13      sp     CR562765: Handle few main commands in DUAL_SIM_FIND_BCCH TRM_WAIT states.
18/10/13      js     CR549651 Dont release TRM if retained for Aceess already in L1_MULTI_SIM_ACQUIRING_RUNNING
05/10/13      js     CR539807:Added handling of TRM release in ACQUIRING_ABORT state
05/10/13      js     CR461049 Cell Reselection Failure when 3 KHz frequency drift is applied on N cell
10/09/13      sp     CR539352: Update trm_status at start of dual_sim_bcch activities
14/08/13      sp     CR529521: Move req_notify_enh_data from static to local
21/08/13      npt    CR531633 - Remove FEATURE_DIME_MODEM dependencies
21/06/13      cs     Major Triton TSTS Syncup
20/05/13      cs     TSTS Updates
20/03/12      ab     DSDS CR461102 - When retain lock make the TRM status as GRANTED.
15/04/13      am     CR473834 Avoid TRM release while FW loading is in progress
20/03/12      sjw    DSDS CR345480 - Use the current inversion setting for acquisition priority
07/12/11      mc     CR.323352 - when a TRM abort is seen during get bcch, main command should only be
                     overwritten with L1_MULTI_SIM_TRM_ABORTING_COMMAND if its initial value was L1_NULL_COMMAND.
14/02/13      pjr    CR452272 added support to set multisim mode at activation time
16/11/11      cs     CR319119 Always reset the message layer before configuring new pscan
05/10/11      ap     CR 308923: DSDS GL1 Task can be stuck waiting on signal after MPH_DS_L1_ABORT_IND
21/09/11      ap     DSDS CR306482 - Ensure ISR runs and actions MPH_L1_DS_ABORT_IND before
                     continuing in task
26/09/11      nt     DSDS CR307781 - Added extra check to ensure firmware is loaded before l1_bcch()
16/05/11      njt    DSDS CR278852 - Upon completing a list decode and return to NULL don't use
                     the L1_MULTI_SIM_ACQ_BCCH_LIST_SUSPENDING state.
16/05/11      njt    DSDS Added suspension state when L1_multi_sim_get_bcch_list_fcch_sch_bcch is
                     terminated frm running by a main_command other than the DEACTIVATE
13/04/11      njt    DSDS CR281997 - stack did not respond to CM_STOP_MODE_REQ, Bad L1 sub-state
22/03/11      ap     CR279430 DSDS Timing value wraps casusing frame misalignment
25/03/11      njt    DSDS CR280372 - mdsp busy after cell reselection
28/03/11      njt    DSDS CR278796 - Add check for TRM_ABORTING in reselection state
02/03/11      njt    DSDS CR276931 - BPLMN suspension changes
25/01/11      nt     Added suspension state to L1_multi_sim_acquiring.
17/01/11      nt     Pscan fix to prevent suspending unstarted pscan.
11/01/11      nt     CR269703 For BCCH search terminated by MPH_SELECT_SPECIFIC_BCCH_REQ
                     keep the lock and FW.
30/06/10      nt     CR244872 Changed the position of the setting of the suspend state.
22/06/10      nt     CR243756 - Allow for coincidence of power scan end and suspension
04/06/10      nt     CR237053 - Allow RANE to be preemptable within min duration
12/05/10      nt     Change to the RESELECTION_DURATION_MS
===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "l1_drx.h"
#include "l1i.h"
#include "l1_isr.h"
#include "rr_l2.h"
#include "ms.h"
#include "msg.h"
#include "err.h"
#include "l1_task.h"
#include "l1_sc.h"
#include "l1_utils.h"  /* for l1_state_change */
#include "l1_fm.h"

#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_task.h"
#include "rex.h"
#include "l1_os.h"

#ifdef PL1_FW_SIM
#include "pl1_test_harness.h"
#else
#include "gmactask.h"
#endif

#ifdef DEBUG_GSM_GPRS_LED_TEST
#error code not present
#endif /* DEBUG_GSM_GPRS_LED_TEST */


#include "gl1_hw.h"
#include "gl1_hw_g.h"

#include "gl1_hw_tcxo_mgr3.h"

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#include "gl1_msg_g.h"
#endif

#include "gl1_msgi.h"

#include  "l1_log.h"  /* For L1 NV logging */

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "gpl1_gprs_utils.h"

#include "geran_eng_mode_info.h"
#include "gpl1_dual_sim.h"

#include "gpl1_grm_intf.h"

#include "gl1_hw_sleep_ctl.h" /* for gfw sleep control during find-bcch modes */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif 

/*===========================================================================

                     LOCAL DEFINES

===========================================================================*/
/* Minimum frames to do TRM reserve and get TRM and do FW/RF init and read BCCH  */
#define L1_MULTISIM_MINIMUM_FRAMES_FOR_BCCH_READ           2
#define THIRD_ATTEMPT_OF_BCCH_DECODE                       2
#define FOURTH_ATTEMPT_OF_BCCH_DECODE                      3

/*
===========================================================================
  LOCAL FUNCTIONS
===========================================================================
*/
boolean L1_multi_sim_find_sch( gas_id_t gas_id );
void l1_isr_handle_multi_sim_find_bcch_command(gas_id_t gas_id);
boolean L1_multi_sim_reselection( gas_id_t gas_id );
boolean L1_multi_sim_get_bcch_list_fcch_sch_bcch( gas_id_t gas_id );
boolean L1_multi_sim_acquiring( gas_id_t gas_id );
static boolean L1_multi_sim_power_scan ( gas_id_t gas_id );
static void L1_multi_sim_bcch_reserve_at( uint32 required_frame_nbr, gas_id_t gas_id );
void L1_multi_sim_bcch_reserve_at_priority_change( gas_id_t gas_id );


/*
===========================================================================
  EXTERNAL FUNCTIONS
===========================================================================
*/
extern void L1_pscan_resume ( gas_id_t gas_id );
extern void L1_pscan_suspend ( void* dummy, gas_id_t gas_id );
extern boolean get_sch_decoded( gas_id_t gas_id );

extern void l1_sch_adjust_timing_error(gas_id_t gas_id );

extern void gl1_msgi_pwr_meas_init( gas_id_t gas_id );

/*
===========================================================================
  LOCAL MODULE DATA
===========================================================================
*/
typedef struct
{
  uint32 bcch_req_frame;
  boolean trm_request_notify;
} l1_reselection_data_t;

/* These flags can only be used by a client holding the lock so are safe from dual dataspacing */
static byte fw_rf_setting_up[NUM_GERAN_DATA_SPACES] =
              { INITIAL_VALUE( RF_FW_SETUP_FRAME_DELAY ) };
static byte rf_fw_warmup_timer[NUM_GERAN_DATA_SPACES] =
              { INITIAL_VALUE( RF_FW_WARMUP_FRAME_DELAY ) };
static byte suspension_timer[NUM_GERAN_DATA_SPACES] =
              { INITIAL_VALUE( SUSPENSION_FRAME_DELAY ) };
static boolean suspension_state_exit_to_abort[NUM_GERAN_DATA_SPACES] =
              { INITIAL_VALUE( FALSE ) };
static l1_reselection_data_t l1_reselection_data[NUM_GERAN_DATA_SPACES] =
              { INITIAL_VALUE_S( 0 ) };

/*===========================================================================

FUNCTION  l1_isr_handle_multi_sim_find_sch_command

DESCRIPTION
   Handle the main command mentioned in the function name in various modes.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_handle_multi_sim_find_sch_command( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gl1_trm_state_t trm_status = GL1_TRM_NULL;


  switch (l1_tskisr_blk->command)
  {
    case L1_DECODE_SCH_COMMAND:
      /*set up to reselect new serving cell*/

      l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
      l1_tskisr_blk->command       = L1_DS_RESELECTION_APPLY_TIME_CHANGE;
      L1_transfer_new_data( gas_id );

      l1_tskisr_blk->sub_state     = L1_MULTI_SIM_FIND_SCH;

      l1_tskisr_blk->sub_sub_state = L1_SCH_START;
      l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
      l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_STOPPED;

      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      /* if the lock is already granted then change the timing here */
      if ( (trm_status == GL1_TRM_GRANTED) || 
           (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
      {
        if( l1_tskisr_blk->firmware_loaded && ( l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE ))
        {
          (void)L1_change_FN(l1_tskisr_blk->current_params.frame_lag, gas_id);

          /* slam frame tick */
          gl1_msg_correct_timing((int32)l1_tskisr_blk->current_params.QB_lag,gas_id);
          if ( l1_idle_data->reselection_apply_freq_correction )
          {
            l1_sc_reselection_apply_freq_correction( l1_tskisr_blk->current_params.ARFCN, gas_id);
            l1_idle_data->reselection_apply_freq_correction = FALSE;
          }
          /* we will start out on timeslot 0 */
          l1_set_current_timeslot(0, gas_id);

          l1_sc_set_sync(l1_tskisr_blk->current_params.frame_lag, /*FN*/
                         l1_tskisr_blk->current_params.QB_lag,     /*qbit_offset*/
                         gas_id);

          l1_tskisr_blk->command = L1_NULL_COMMAND;
        }
      }

      break;

    default:
      MSG_GERAN_HIGH_1_G("Bad L1 command %d.", l1_tskisr_blk->command);
      break;
  }
}

/*===========================================================================

FUNCTION  l1_isr_handle_multi_sim_find_bcch_command

DESCRIPTION
 used for dual sim acqusition activities and reselection
DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_handle_multi_sim_find_bcch_command(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gl1_trm_state_t trm_status = GL1_TRM_NULL;

   switch (l1_tskisr_blk->command)
   {
      case L1_DECODE_BCCH_COMMAND:
         /*set up to reselect new serving cell*/
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
         l1_tskisr_blk->sub_state     = L1_MULTI_SIM_RESELECTION;
         l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_tskisr_blk->command       = L1_DS_RESELECTION_APPLY_TIME_CHANGE;
         l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;

         trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

         /* If we already have the lock then this timing change can be applied here */
         if ( (trm_status == GL1_TRM_GRANTED) || 
              (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
         {
           if( l1_tskisr_blk->firmware_loaded && ( l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE ))
           {
             /* change frame number */
             (void)L1_change_FN(l1_tskisr_blk->current_params.frame_lag, gas_id);

             gl1_msg_correct_timing((int32)l1_tskisr_blk->current_params.QB_lag,gas_id);
             if( l1_idle_data->reselection_apply_freq_correction )
             {
               l1_sc_reselection_apply_freq_correction( l1_tskisr_blk->current_params.ARFCN, gas_id);
               l1_idle_data->reselection_apply_freq_correction = FALSE;
             }
             /* we will start out on timeslot 0 */
             l1_set_current_timeslot(0, gas_id);

             l1_sc_set_sync(l1_tskisr_blk->current_params.frame_lag, /*FN*/
                            l1_tskisr_blk->current_params.QB_lag,     /*qbit_offset*/
                            gas_id);

             l1_tskisr_blk->command = L1_NULL_COMMAND;

             /* Send a reselection cnf if necessary */
             if (l1_tskisr_blk->current_params.reselection_cnf_reqd )
             {
               L1_send_MPH_RESELECTION_CNF( TRUE, gas_id );
               l1_tskisr_blk->current_params.reselection_cnf_reqd = FALSE;
               l1_tskisr_blk->next_params.reselection_cnf_reqd = FALSE;
             }
           }
         }
         break;

      case L1_ACQUIRE_COMMAND:
      case L1_ACQUIRE_DURING_RESELECTION_COMMAND:
         /* change mode */
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
         l1_tskisr_blk->sub_state     = L1_MULTI_SIM_AQUIRING;
         l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_STOPPED;
         break;

      case L1_ACQUIRE_BCCH_LIST_COMMAND:
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_tskisr_blk->sub_state     = L1_MULTI_SIM_AQUIRE_BCCH_LIST;
         l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED;
         l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;
         break;

      case L1_SCAN_COMMAND:
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
         l1_tskisr_blk->sub_state     = L1_MULTI_SIM_POWER_SCAN;
         l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_STOPPED;
         l1_tskisr_blk->pscan_state = L1_SCAN_START;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         l1_pscan_mcpm_change(TRUE,gas_id);

         break;

      default:
         MSG_GERAN_HIGH_1_G("Bad L1 command %d.", l1_tskisr_blk->command);
         break;
   }

   /* Cannot asume we won't sleep - the other GSM client may be ready to sleep here */
   gl1_drx_require_next_tick(gas_id);
}

/*===========================================================================

FUNCTION  l1_isr_find_bcch

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_multi_sim_find_bcch( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   boolean abort_find_bcch = FALSE;

   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
   /*Inform MCPM about critical scenario now if needed*/
   if((timeline_error_recovery_needed[gas_id] !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE)
       &&(gl1_hw_mcpm_get_state(gas_id) == GL1_MCPM_GERAN_IDLE))
   {
     gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
     critical_scn_updated_to_mcpm[gas_id] = TRUE;
   }
#endif

   switch (l1_tskisr_blk->sub_state)
   {
      case L1_MULTI_SIM_POWER_SCAN:
      /* This state is used to drive the power scan.
        * It is transitioned to after reception of
        * MPH_POWER_SCAN_REQ
        */
        abort_find_bcch = L1_multi_sim_power_scan(gas_id);
        break;

      case L1_MULTI_SIM_AQUIRING:
      /* This state is used to drive an aquisition of FCCH/SCH
        * after reception of MPH_SELECT_SPECIFIC_BCCH_REQ
        * or MPH_GPRS_RESELECTION_REQ
        */
        abort_find_bcch = L1_multi_sim_acquiring(gas_id);
        break;

      case L1_MULTI_SIM_FIND_SCH:
      /* This state is used to decode SCH before a cell reselction
        * MPH_GPRS_RESELECTION_REQ
        * only if SYNC flag is true and bsic known
        */
        abort_find_bcch = L1_multi_sim_find_sch(gas_id);
        break;

      case L1_MULTI_SIM_RESELECTION:
      /* This state is used to decode the specified BCCH for
        * MPH_GPRS_RESELECTION_REQ or MPH_SELECT_SPECIFIC_BCCH_REQ
        * This is usually driven by the reception of FCCH/SCH via
        * L1_AQUIRING
        */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_RESEL);
#endif	  
        abort_find_bcch = L1_multi_sim_reselection(gas_id);
        break;

      case L1_MULTI_SIM_AQUIRE_BCCH_LIST:
      /* This state is used to decode the specified group of SCH's and BCCH's
        * for a decode MPH_DECODE_BCCH_LIST_REQ
        */
        abort_find_bcch =  L1_multi_sim_get_bcch_list_fcch_sch_bcch(gas_id);
#ifdef FEATURE_GL1_ASDIV_ACQ
         gl1_asdiv_acq_state_tick(gas_id);
         MSG_GERAN_HIGH_1_G("AsDiv: FT handler reg: %d", gl1_msgi_check_ft_handler_registered(gas_id ,GL1_MSG_FT_HDLR_PWR_MEAS));
#endif
        break;
#ifdef FEATURE_GL1_ASDIV_ACQ 
     /*New Sub State for AsDiv in Acquisition*/
      case L1_ASDIV_ACQ_SWITCH:
               gl1_asdiv_acq_state_tick(gas_id);
               abort_find_bcch = TRUE;
               break;
#endif /*FEATURE_GL1_ASDIV_ACQ*/

      case L1_MULTI_SIM_DECOUPLING:
        

        if (l1_fm_get_suspend_trm_release(gas_id) == 0)
        {
          MSG_GERAN_HIGH_0_G("Move to MULTI_SIM_DECOUPLED");
          l1_change_state(L1_MULTI_SIM_DECOUPLED, gas_id);
          l1_tskisr_blk->multi_sim_decoupled_state = L1_MULTI_SIM_DECOUPLED_WAIT_FOR_TRM_GRANT;
          gl1_drx_require_next_tick(gas_id);
          L1_send_PH_READY_TO_SEND_IND(L2_RACH, gas_id);
        }

        break;

      default:
         MSG_GERAN_HIGH_1_G( "Bad L1 sub-state %d", l1_tskisr_blk->sub_state);
         break;
   }

   /******************************************************************************/
   if( abort_find_bcch) 
   { 
       if(gl1_hw_qta_gap_active(gas_id)) 
       { 
          abort_find_bcch = FALSE; 
          MSG_GERAN_HIGH_0_G( "QTA still active - delay bcch abort to next frame"); 
       } 
   } 

   if (l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND )
   {
      if ( abort_find_bcch )
      {
         l1_change_state( L1_MULTI_SIM_NULL, gas_id );
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;

         /*
            This should not affect anything since we're merely
            transitioning to NULL mode
          */
         L1_transfer_new_data( gas_id );

         /* Send MPH_DECODE_BCCH_LIST_CNF to RR if specified */
         if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_DECODE_BCCH_LIST_CNF )
         {
            /* reset to a NULL_CNF so that we dont send the BCCH_LIST_CNF twice */
            l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
            l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
      l1_tskisr_blk->sub_sub_state = L1_ACQ_END;
            L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE, gas_id);
         }
         else if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_POWER_SCAN_CNF )
         {
           /* reset to a NULL_CNF so that we dont send the MPH_POWER_SCAN_CNF twice */

           l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
           l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;

           L1_send_MPH_POWER_SCAN_CNF(FALSE, gas_id);

         }

      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_IDLE_COMMAND )
   {
      if ( abort_find_bcch )
      {
         idle_data_T  *idle_data_ptr;

         L1_transfer_new_data( gas_id );
         l1_change_state( L1_MULTI_SIM_IDLE, gas_id );
         l1_tskisr_blk->sub_state = L1_IDLE_INIT;
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         L1_send_MPH_START_IDLE_MODE_CNF(gas_id);

         /*set timeslot according to CCCH_GROUP*/
         idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

         L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND )
   {
      if ( abort_find_bcch )
      {
         /* The way to FIND_BCCH is always through NULL MODE.  Got to
            NULL mode and branch based on the specifid command
         */
         l1_change_state( L1_MULTI_SIM_NULL, gas_id );
      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
   {
      if ( abort_find_bcch )
      {
         l1_change_state( L1_INACTIVE_MODE, gas_id );
      }
   }
   /******************************************************************************/
   else if (l1_tskisr_blk->main_command == L1_GPRS_IDLE_COMMAND )
   {
      if ( abort_find_bcch )
      {
         L1_transfer_new_data( gas_id );
         l1_change_state( L1_SUSPEND_MODE, gas_id );
         l1_tskisr_blk->sub_sub_state = L1_GPRS_IDLE_INIT;
         l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         L1_send_MPH_START_IDLE_MODE_CNF(gas_id);
      }
   }
   else if (l1_tskisr_blk->main_command == L1_DECOUPLE_MULTI_SIM_COMMAND)
   {
     if (abort_find_bcch)
     {
       /* Need to wait for TRM release and FW unload */
       l1_tskisr_blk->sub_state = L1_MULTI_SIM_DECOUPLING;
   }

   }
               
   else if (l1_tskisr_blk->main_command != L1_NULL_COMMAND)
   {
      MSG_GERAN_HIGH_1_G("Unexpected L1 main_command: %d",l1_tskisr_blk->main_command);
   }
}

/*===========================================================================

FUNCTION gl1_get_find_bcch_subreason

DESCRIPTION
 Return the subreason to use for TRM activities

DEPENDENCIES
 None

RETURN VALUE
  sys_proc_type_e_type - TRM subreason

SIDE EFFECTS
  None

===========================================================================*/
static sys_proc_type_e_type gl1_get_find_bcch_subreason( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  sys_proc_type_e_type subreason;

  if (l1_tskisr_blk->x2g_resel_in_progress)
  {
    subreason = SYS_PROC_TYPE_IRAT_CELL_RESELECTION;
  }
  else if (l1_tskisr_blk->x2g_cco_in_progress)
  {
    subreason = SYS_PROC_TYPE_CCO;
  }
  else
  {
    subreason = gl1_get_specific_sys_proc(GERAN_ACQ_PROCEDURE, gas_id);
  }
  return subreason;
}

/*===========================================================================

FUNCTION L1_multi_sim_acquiring

DESCRIPTION
 State machine for acquiring in multi-SIM mode

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_multi_sim_acquiring( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean l_abort_find_bcch=FALSE;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  boolean mcpm_ongoing;

  switch ( l1_tskisr_blk->multi_sim_acquiring_state )
  {

    case L1_MULTI_SIM_ACQUIRING_STOPPED:
      {
        sys_proc_type_e_type subreason = gl1_get_find_bcch_subreason(gas_id);

        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if(l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
        {
          MSG_GERAN_HIGH_0_G("FW Load Status: Release pending, wait for another tick for issuing RANE");
        }
        else if ( (trm_status == GL1_TRM_GRANTED) || 
                  (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
        {
          grm_change_priority( l1_tskisr_blk->client_id, TRM_CHANNEL_MAINTENANCE, subreason, gas_id );

          grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

          /* we got here from another mode e.g.dedicated/findSCH/initACQ and already have the lock
          check to see if we need to load the firmware and move to the correct state based upon this */
          if ( l1_tskisr_blk->firmware_loaded == TRUE )
          {
            l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_RUNNING;
          }
          else
          {
            l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_TRM_WAIT;
          }
        }
        else
        if ( trm_status == GL1_TRM_ABORTING )
        {
          call_in_task_grm_release( l1_tskisr_blk->client_id );
        }
        else
        {
          /* Send GFW sleep cmd, then fws_suspend so that we can call MCPM 
             INIT_REQ, which calls fws_sleep. This way we use less power 
             while waiting for TRM grant cb. */

          if(gl1_hw_sleep_try_fw_sleep(gas_id))
          {
          grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
          grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

            /* TRM Request(s)... */

          grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

            /* Ask for the amount of time required for a complete reselection */
            grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
            if (grm_get_acquisition_priority(gas_id) == GERAN_PRIORITY_INVERTED)
            {
              grm_request_and_notify_enhanced_data_p->reason  = TRM_CHANNEL_MAINTENANCE_INV;
            }
            else
            {
              grm_request_and_notify_enhanced_data_p->reason    = TRM_CHANNEL_MAINTENANCE;
            }
            grm_request_and_notify_enhanced_data_p->sub_reason = subreason;
            grm_request_and_notify_enhanced_data_p->resource  =
              gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );
            grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk( RESELECTION_DURATION_MS, T_MSEC );

          grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_RESELECTION;
          /*Add bands to freq_info*/
            grm_clear_freq_info(l1_tskisr_blk->client_id, gas_id);
          grm_add_trm_band_to_freq_input_type(gl1_map_gsm_band_to_sys_band(l1_tskisr_blk->next_params.ARFCN.band),
                                              l1_tskisr_blk->client_id,
                                              gas_id); 

          grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);

          call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );

            /* MCPM Request... */
            MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_STOPPED: MCPM_GERAN_INIT_REQ!");
            gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_INIT_REQ, GL1_MCPM_GERAN_INIT, gas_id);

            l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_TRM_WAIT;
          }
          else
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_STOPPED: Waiting for FW to sleep");
        }
        }

        break;
      }

    case L1_MULTI_SIM_ACQUIRING_TRM_WAIT:
      {
        boolean fall_through = FALSE;

        /* This state will conditionally send an MCPM_GERAN_IDLE_REQ based on 
           gl1_hw_mcpm_get_state(), but we need to be sure there are no ongoing
           configs before reading that function. */
        if(gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id))
        {
          MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_TRM_WAIT: MCPM ongoing");
          break;
        }

        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if(trm_status == GL1_TRM_NOT_GRANTED ||
           trm_status == GL1_TRM_ABORTING ||
           l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
           l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND ||
           l1_tskisr_blk->main_command == L1_IDLE_COMMAND ||
           l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND ||
           l1_tskisr_blk->main_command == L1_GPRS_IDLE_COMMAND)
        {
          /* Trm has denied the lock, or we need to give up */

          MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_TRM_WAIT: ABORT!");

          /* Undo our power saving steps so we're back in original MCPM/MCVS state */
          if(gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
          {
            gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);
          }

          l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_MCPM_WAIT_ABORT;
        }
        else if(trm_status == GL1_TRM_GRANTED ||
                trm_status == GL1_TRM_RETAINED_FOR_ACCESS)
        {
          if(gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_TRM_WAIT: MCPM_GERAN_IDLE_REQ!");
            gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);
          }
          else
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_TRM_WAIT: MCPM_GERAN_IDLE_REQ already done.");
            fall_through = TRUE;
          }
          l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_MCPM_WAIT;
        }
        else
        {
          MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_ACQUIRING_TRM_WAIT (trm_status=%d)",(int)(trm_status));
        }

        if(!fall_through)
        {
          break;
        } 
      }

    case L1_MULTI_SIM_ACQUIRING_MCPM_WAIT:
      {
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if(trm_status == GL1_TRM_NOT_GRANTED                    ||
           trm_status == GL1_TRM_ABORTING                       ||
             l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
             l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND ||
             l1_tskisr_blk->main_command == L1_IDLE_COMMAND ||
             l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND ||
             l1_tskisr_blk->main_command == L1_GPRS_IDLE_COMMAND )
        {
          /* Trm has denied the lock, or we need to give up */

          MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_MCPM_WAIT: ABORT!");
          l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_MCPM_WAIT_ABORT;

          /* we will fall through to L1_MULTI_SIM_ACQUIRING_MCPM_WAIT_ABORT */
        }
        else
        {
          mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
          if(mcpm_ongoing ||
             gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
          {
            /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */

            MSG_GERAN_HIGH_3_G("GL1_PWR: ACQUIRING_MCPM_WAIT: wait another frame (trm_status %d MCPM ongoing %d MCPM status %d)",
                               (int)(trm_status), mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));
        }
        else 
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_MCPM_WAIT: loading FW/RF!");
  
            if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
        {
              MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
            }
  
          /* Need to load the RF and FW now, and only move to runnning once ready */
          /* Time is now ticking keep watch for an abort */
          /* as this may happen at any time during this state too */
          /* Need to load the RF and FW now, and only move to runnning once ready, this will be
            indicated by the flag l1_tskisr_blk->firmware_loaded */
  
          gl1_multi_sim_fw_rf_load( l1_tskisr_blk->client_id, gas_id );

          fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
          rf_fw_warmup_timer[gas_id] = RF_FW_WARMUP_FRAME_DELAY;
          l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_FW_LOAD_WAIT;
        }
          break;
        }

        /* can fall through */
      }

    case L1_MULTI_SIM_ACQUIRING_MCPM_WAIT_ABORT:
    {
      /* Wait in this state until MCPM_GERAN_IDLE_REQ has completed. Only then can we proceed with the abort. */
      mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
      if(mcpm_ongoing ||
         gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
      {
        /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */
      
        MSG_GERAN_HIGH_2_G("GL1_PWR: ACQUIRING_MCPM_WAIT_ABORT: wait another frame (MCPM ongoing %d MCPM status %d)",
                           mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));
      }
      else
      {
        /* Undo FW Sleep */
        if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
        {
          MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
        }

        /* Release TRM lock (if holding it) */
        call_in_task_grm_release(l1_tskisr_blk->client_id);

            l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_STOPPED;
            l_abort_find_bcch = TRUE;
        MSG_GERAN_HIGH_0_G("GL1_PWR: ACQUIRING_MCPM_WAIT_ABORT: Now aborting acquiring");
        }
        break;
      }

    case L1_MULTI_SIM_ACQUIRING_FW_LOAD_WAIT:
      {
        if ( fw_rf_setting_up[gas_id] > 0 )
        {
          fw_rf_setting_up[gas_id]--;
        }

        if( l1_tskisr_blk->firmware_load_status == FW_LOAD_IN_PROGRESS )
        {
          MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_ACQUIRING_FW_LOAD_WAIT FN = %d",gl1_get_FN( gas_id ));
        }
        /* Time is now ticking keep watch for an abort */
        /* as this may happen at any time during this state too */
        else
        if (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING ||
            l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
        {
          if( fw_rf_setting_up[gas_id] == 0 )
          {
            l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_STOPPED;
            call_in_task_grm_release( l1_tskisr_blk->client_id );
            l_abort_find_bcch = TRUE;
            MSG_GERAN_HIGH_0_G("Multi-SIM Acquiring Process Aborted");
          }
          else
          {
            MSG_GERAN_HIGH_1_G("Delay grm release FN = %d",gl1_get_FN( gas_id ));
          }
        }
        else
        if ( l1_tskisr_blk->firmware_loaded == TRUE )
        {
          /* add in some RF FW settling time to prevent any RSSI=0 issues */
          if ( rf_fw_warmup_timer[gas_id] == 0 )
          {
            l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_RUNNING;
          }
          else
          {
            rf_fw_warmup_timer[gas_id]--;
            MSG_GERAN_HIGH_0_G("waiting for rf/fw warmup time");
          }
        }
        break;
      }

    case L1_MULTI_SIM_ACQUIRING_RUNNING:
      /* the controlling function l1_initial_acq() will be ticked every frame.
       * This control function when it has completed its task will move to the
       * next state directed by the setting within L1_ACQ_END within l1_initial_acq() .
       * To do this the function will also need changes to move to the correct
       * dual sim state depending upon the success/failure, and the command setting.
       * This state shall move back to TRM_WAIT.
      */
      if ( l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
      {
        /* Still need to tick over the handler */
        (void)L1_initial_acq(gas_id);

        l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_SUSPENDING;
        suspension_timer[gas_id] = ACQ_SUSPENSION_FRAME_DELAY;
        MSG_GERAN_HIGH_0_G("GL1_TRM_ABORTING");
      }
      else if (L1_initial_acq(gas_id))
      {
        /* fcch_sch_idle == TRUE*/
        /* Finished first attempt try again */
        if (l1_tskisr_blk->sub_sub_state == L1_ACQ_INITIALISE )
        {
          l1_tskisr_blk->sub_state     = L1_MULTI_SIM_AQUIRING;
          L1_transfer_new_data( gas_id );
        }
        /* Finished search completed ok move to reselection */
        else if(l1_tskisr_blk->sub_sub_state == L1_BCCH_START )
        {
          l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
          /* this is correct however we have been granted trm and then relaese it again where we want to really carry
          on with trm maybe add a hold on indefintae trm here */
          l1_tskisr_blk->sub_state     = L1_MULTI_SIM_RESELECTION;
          l1_tskisr_blk->command       = L1_NULL_COMMAND;
          L1_transfer_new_data( gas_id );

          /* GL1_TRM_ABORTING is set so needs to be honoured here, as the actual
          function L1_initial_acq cannot respond to anything but a main command. This means
          the acquisition must continue until completion. */
          if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING )
          {
            call_in_task_grm_release( l1_tskisr_blk->client_id );
          }

        }
        /* Search aborted */
        else if ( l1_tskisr_blk->main_command != L1_NULL_COMMAND )
        {
          l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_ABORT;
          /*dont release trm if RACH has already retained it*/
          if((l1_tskisr_blk->main_command != L1_ACCESS_COMMAND ) &&
             (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_RETAINED_FOR_ACCESS))
          {
            call_in_task_grm_release( l1_tskisr_blk->client_id );
          }
          else
          {
            MSG_GERAN_HIGH_0_G("Dont rel TRM, retained for Access");
          }
        }
        /* The search is at an end and has failed release the lock via suspension here as TRM abort is set */
        else if ( ( l1_tskisr_blk->sub_sub_state == L1_SCH_END ) &&
                  ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING ) &&
                  ( l1_tskisr_blk->command == L1_NULL_COMMAND)
                )
        {
          l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_ABORT;

          call_in_task_grm_release( l1_tskisr_blk->client_id );
        }

        /*call to trm hold on indefinate*/
        l_abort_find_bcch=TRUE;

      }
      break;
    case L1_MULTI_SIM_ACQUIRING_ABORT :
      l_abort_find_bcch=TRUE;

      /*If GL1_TRM_ABORTING flag is set then release the TRM*/
      if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING )
      {
        call_in_task_grm_release( l1_tskisr_blk->client_id );

        l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_STOPPED;
      }

      break;

   case L1_MULTI_SIM_ACQUIRING_SUSPENDING:
     {
       if ( suspension_timer[gas_id] > 0)
       {
         suspension_timer[gas_id]--;
       }

       /* Stay here to allow the suspension to provide the msg/hw layers with
          "suspension_timer" frames to terminate */
       if( L1_initial_acq(gas_id) && ( suspension_timer[gas_id] == 0 ) )
       {
         call_in_task_grm_release( l1_tskisr_blk->client_id );

         l1_tskisr_blk->multi_sim_acquiring_state = L1_MULTI_SIM_ACQUIRING_ABORT;

         l_abort_find_bcch = TRUE;

         /* go back to Dual Sim Null state and wait for further commands */
         MSG_GERAN_HIGH_0_G("Dual SIM acq suspended");
       }
       else
       {
         l_abort_find_bcch = FALSE;
         MSG_GERAN_HIGH_0_G("L1_initial_acq() suspension timer running");
       }
     }
     break;

    default:
      MSG_GERAN_HIGH_0_G("Unexpected dsim acquiring state.");
      break;

  }

  gl1_drx_require_next_tick(gas_id);

  return(l_abort_find_bcch);
}


/*===========================================================================

FUNCTION

DESCRIPTION


DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_multi_sim_find_sch( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gl1_trm_state_t           trm_status = GL1_TRM_NULL;

  /* locals */
  boolean find_sch_complete = TRUE;
  boolean mcpm_ongoing;

  /* Code starts */
  switch ( l1_tskisr_blk->multi_sim_find_sch_state)
  {

    case L1_MULTI_SIM_FIND_SCH_STOPPED:
      {
        sys_proc_type_e_type subreason = gl1_get_find_bcch_subreason(gas_id);

        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if(l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
        {
          MSG_GERAN_HIGH_0_G("FW Load Status: Release pending, wait for another tick for issuing RANE");
        }
        else if ( (trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
        {
          grm_change_priority( l1_tskisr_blk->client_id, TRM_CHANNEL_MAINTENANCE, subreason, gas_id );

          grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

          /* we got here from another mode e.g.dedicated/findSCH/initACQ and already have the lock
          check to see if we need to load the firmware and move to the correct state based upon this */
          if ( l1_tskisr_blk->firmware_loaded == TRUE )
          {
            if ( l1_idle_data->reselection_apply_freq_correction )
            {
              l1_sc_reselection_apply_freq_correction( l1_tskisr_blk->current_params.ARFCN, gas_id);
              l1_idle_data->reselection_apply_freq_correction = FALSE;
            }

            l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_RUNNING;
          }
          else
          {
            l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_TRM_WAIT;
          }
        }
        else
        if ( trm_status == GL1_TRM_ABORTING )
        {
          call_in_task_grm_release( l1_tskisr_blk->client_id );
        }
        else
        {
          grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
          grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

          /* Send GFW sleep cmd, then fws_suspend so that we can call MCPM 
             INIT_REQ, which calls fws_sleep. This way we use less power 
             while waiting for TRM grant cb. */

          if(!gl1_hw_sleep_try_fw_sleep(gas_id))
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_STOPPED: Waiting for FW to sleep");
          }
          else
          {
            grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;
    
            /* Ask for the amount of time required for a complete reselection */
            grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
            if (grm_get_acquisition_priority(gas_id) == GERAN_PRIORITY_INVERTED)
            {
              grm_request_and_notify_enhanced_data_p->reason    = TRM_CHANNEL_MAINTENANCE_INV;
            }
            else
            {
              grm_request_and_notify_enhanced_data_p->reason    = TRM_CHANNEL_MAINTENANCE;
            }
            grm_request_and_notify_enhanced_data_p->sub_reason = subreason;
            grm_request_and_notify_enhanced_data_p->resource  =
              gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );
            grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk( RESELECTION_DURATION_MS, T_MSEC );
    
            grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_RESELECTION;
            /*Add bands to freq_info*/
            grm_clear_freq_info(l1_tskisr_blk->client_id, gas_id);
            grm_add_trm_band_to_freq_input_type(gl1_map_gsm_band_to_sys_band(l1_tskisr_blk->next_params.ARFCN.band),
                                                l1_tskisr_blk->client_id,
                                                gas_id); 
            /* reordered these statements */
            grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);

            call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );

            /* MCPM Request(s)... (will all get queued up and run in DPC context) */
            MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_STOPPED: MCPM_GERAN_INIT_REQ!");
            gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_INIT_REQ, GL1_MCPM_GERAN_INIT, gas_id);

            find_sch_complete = FALSE;
            
            l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_TRM_WAIT;
          }
        }
        break;
      }

    case L1_MULTI_SIM_FIND_SCH_TRM_WAIT:
      {
        /* This state will conditionally send an MCPM_GERAN_IDLE_REQ based on 
           gl1_hw_mcpm_get_state(), but we need to be sure there are no ongoing
           MCPM configs before reading that function. */

        find_sch_complete = FALSE;

        if(gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id))
        {
          MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_TRM_WAIT: MCPM ongoing");
          break;
        }

        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if(trm_status == GL1_TRM_NOT_GRANTED ||
           trm_status == GL1_TRM_ABORTING ||
           trm_status == GL1_TRM_NULL ||
           l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
           l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND)
        {
          /* Trm has denied the lock, or we otherwise need to give up */    
          MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_TRM_WAIT: ABORT!");
    
          /* Undo our power saving steps so we're back in original MCPM/MCVS state */
          MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_TRM_WAIT: MCPM_GERAN_IDLE_REQ!");
          gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);

          l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_MCPM_WAIT_ABORT;
        }
        else
        if ((trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
        {
          MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_TRM_WAIT: MCPM_GERAN_IDLE_REQ!");
          gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);
          l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_MCPM_WAIT;
        }
        else
        {
          MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_FIND_SCH_TRM_WAIT (trm_status=%d)",(int)(trm_status));
        }
        break;
      }

    case L1_MULTI_SIM_FIND_SCH_MCPM_WAIT:
      {
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if (trm_status == GL1_TRM_NOT_GRANTED ||
            trm_status == GL1_TRM_ABORTING ||
            trm_status == GL1_TRM_NULL ||
            l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
            l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND )
        {
          /* Trm has denied the lock, or we need to give up */
          
          MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_MCPM_WAIT: ABORT!");
          l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_MCPM_WAIT_ABORT;
          
          find_sch_complete = FALSE;
          /* we will fall through to L1_MULTI_SIM_FIND_SCH_MCPM_WAIT_ABORT */
        }
        else
        {
          mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
          if(mcpm_ongoing ||
            gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
          {
            /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */

            MSG_GERAN_HIGH_3_G("GL1_PWR_DEBUG: FIND_SCH_MCPM_WAIT: wait another frame (trm_status %d MCPM ongoing %d MCPM status %d)",
                               (int)(trm_status), mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));

            find_sch_complete = FALSE;
          }
          else
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_MCPM_WAIT: loading FW/RF!");
           
            /* wake fw */
            if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
            {
              MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
            }
  
            /* Need to load the RF and FW now, and only move to runnning once ready, this will be
               indicated by the flag l1_tskisr_blk->firmware_loaded */
            gl1_multi_sim_fw_rf_load( l1_tskisr_blk->client_id, gas_id );

            fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
            rf_fw_warmup_timer[gas_id] = RF_FW_WARMUP_FRAME_DELAY;
            l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_FW_LOAD_WAIT;
          }
          break;
        }

        /* can fall through */
      }

    case L1_MULTI_SIM_FIND_SCH_MCPM_WAIT_ABORT:
      {
        /* Wait in this state until MCPM_GERAN_IDLE_REQ has completed. Only then can we proceed with the abort. */

        mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
        if(mcpm_ongoing ||
           gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
        {
          /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */

          MSG_GERAN_HIGH_2_G("GL1_PWR_DEBUG: FIND_SCH_MCPM_WAIT_ABORT: wait another frame (MCPM ongoing %d MCPM status %d)",
                             mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));

          find_sch_complete = FALSE;
        }
        else
        {
          /* Undo FW Sleep */
          if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
          {
            MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
          }

          /* Release TRM lock (if holding it) */
          call_in_task_grm_release(l1_tskisr_blk->client_id);

          l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_STOPPED;

          MSG_GERAN_HIGH_0_G("GL1_PWR_DEBUG: FIND_SCH_MCPM_WAIT_ABORT: Now aborting pscan");
        }
        break;
      }

    case L1_MULTI_SIM_FIND_SCH_FW_LOAD_WAIT:
      {
        if ( fw_rf_setting_up[gas_id] > 0 )
        {
          fw_rf_setting_up[gas_id]--;
        }

        /* Need to load the RF and FW now, and only move to runnning once ready */
        if( l1_tskisr_blk->firmware_load_status == FW_LOAD_IN_PROGRESS )
        {
          MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_FIND_SCH_FW_LOAD_WAIT FN = %d",gl1_get_FN( gas_id ));
        }
        /* Time is now ticking keep watch for an abort */
        /* as this may happen at any time during this state too */
        else
        if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_ABORTING ||
             l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
        {
          if( fw_rf_setting_up[gas_id] == 0 )
          {
            call_in_task_grm_release( l1_tskisr_blk->client_id );

            l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_STOPPED;

            MSG_GERAN_HIGH_0_G("Multi-SIM Find SCH Process Aborted");
          }
          else
          {
            MSG_GERAN_HIGH_1_G("Delay grm release FN = %d",gl1_get_FN( gas_id ));
          }
        }
        else
        if ( l1_tskisr_blk->firmware_loaded == TRUE )
        {

          /* perform any timing adjustments here just once */
          if ( l1_tskisr_blk->command == L1_DS_RESELECTION_APPLY_TIME_CHANGE )
          {

            (void)L1_change_FN(l1_tskisr_blk->current_params.frame_lag, gas_id);
            /* slam frame tick */
            gl1_msg_correct_timing((int32)l1_tskisr_blk->current_params.QB_lag,gas_id);
            if ( l1_idle_data->reselection_apply_freq_correction )
            {
              l1_sc_reselection_apply_freq_correction( l1_tskisr_blk->current_params.ARFCN, gas_id);
              l1_idle_data->reselection_apply_freq_correction = FALSE;
            }
            /* we will start out on timeslot 0 */
            l1_set_current_timeslot(0, gas_id);

            l1_sc_set_sync(l1_tskisr_blk->current_params.frame_lag, /*FN*/
                           l1_tskisr_blk->current_params.QB_lag,     /*qbit_offset*/
                           gas_id);
            l1_tskisr_blk->command = L1_NULL_COMMAND;
          }

          /* add in some RF FW settling time to prevent any RSSI=0 issues */
          if ( rf_fw_warmup_timer[gas_id] == 0 )
          {
            l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_RUNNING;
          }
          else
          {
            rf_fw_warmup_timer[gas_id]--;
            MSG_GERAN_HIGH_0_G("waiting for rf/fw warmup time");
          }
        }
        break;
      }

    case L1_MULTI_SIM_FIND_SCH_RUNNING:
      {
        /* The controlling function l1_get_sch() will be ticked every frame.
         * This control function when it has completed its task will move to the next state.
         * To do this the function will also need changes to move to the correct dual sim state
         * which should be RESELECTION. This always seems to be the case even if the SCH
         * decode failed. The state within this function that will need these changes will be L1_SCH_END.
         * The l1_get_sch() function will also need to be changed to use the timing offset for the ASYNC_SIM.
         * This is currently assumed as zero, as the adjustment will have been made prior to the call and
         * within l1_isr_handle_find_sch_command().
         */
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( ( trm_status == GL1_TRM_ABORTING ) ||
             ( l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ) )
        {

          L1_get_sch(gas_id);

          if ( l1_tskisr_blk->sub_sub_state == L1_SCH_END )
          {
            /* re-assert the DSDS state here as this may have been overwritten in L1_get_sch() */
            l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
            L1_send_MPH_RESELECTION_CNF( get_sch_decoded(gas_id), gas_id );

            /* If this is not a deact but a reselction move on to the next state and
            re-request the lock again */
            if ( trm_status == GL1_TRM_ABORTING )
            {
              l1_tskisr_blk->sub_state = L1_MULTI_SIM_RESELECTION;
              l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
              l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
            }

            /* release the lock here */
            call_in_task_grm_release( l1_tskisr_blk->client_id );

            l1_tskisr_blk->multi_sim_find_sch_state = L1_MULTI_SIM_FIND_SCH_STOPPED;

          }
          else
          {
            find_sch_complete = FALSE;
          }

          MSG_GERAN_HIGH_0_G("GL1_TRM_ABORTING");
        }
        else
        {
          L1_get_sch(gas_id);
          if ( l1_tskisr_blk->sub_sub_state == L1_SCH_END )
          { /* fcch_sch_idle == TRUE*/
            l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
            l1_tskisr_blk->sub_state = L1_MULTI_SIM_RESELECTION;
            l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
            l1_tskisr_blk->sub_sub_state = L1_BCCH_START;

            if(get_sch_decoded(gas_id))
            {
              l1_sch_adjust_timing_error(gas_id);
            }
            /* Send reselection confirmation */
            L1_send_MPH_RESELECTION_CNF( get_sch_decoded(gas_id), gas_id );
          }
        }
      break;
      }

    case L1_MULTI_SIM_FIND_SCH_ABORT:
      /* Stay here whilst everthing is finished to prevent any inproper calls to TRM */
    default:
      MSG_GERAN_HIGH_0_G("Unexpected l1 sub sub state.");
      break;
  } /* of switch */

  gl1_drx_require_next_tick(gas_id);

  return find_sch_complete;
}

/*===========================================================================

FUNCTION  L1_multi_sim_reselection

DESCRIPTION
          Handles reselection mode. There is no frame duration, we ask for
          a time and will hold the lock until this has completed regardless.
          Current time is 3secs.

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_multi_sim_reselection( gas_id_t gas_id )
{
  /* locals */
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_reselection_data_t     *l1_reselection_data_p = &l1_reselection_data[gas_id];
  sys_proc_type_e_type      subreason = gl1_get_find_bcch_subreason(gas_id);

  boolean l_abort_find_bcch=FALSE;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  gl1_trm_state_t tx_trm_status = GL1_TRM_NULL;
  boolean leaving_bcch;
  uint32 gap_to_next_bcch = GL1_DEFS_INVALID_FN;
  uint32 ms_reselection_frames_until_active = 0;

  switch ( l1_tskisr_blk->multi_sim_reselection_state )
  {
    case L1_MULTI_SIM_RESELECTION_STOPPED:
      {
	   tx_trm_status = grm_get_trm_status(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id),gas_id);

	   if((tx_trm_status == GL1_TRM_GRANTED) || (tx_trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
	   {
	     gl1_hw_turn_tx_off(TRUE, gas_id);
	   }
	   
       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

       l1_reselection_data_p->bcch_req_frame = GL1_DEFS_INVALID_FN;

       if(l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
       {
         MSG_GERAN_HIGH_0_G("FW Load Status: Release pending, wait for another tick for issuing RANE");
       }
        else if ( l1_tskisr_blk->firmware_loaded && 
                 ( (trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) ) )
       {
         if ( l1_tskisr_blk->firmware_unload_in_progress == TRUE )
         {
           MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_RESELECTION_STOPPED unload in progress");
           return l_abort_find_bcch;
         }

         grm_change_priority( l1_tskisr_blk->client_id, TRM_CHANNEL_MAINTENANCE,subreason, gas_id );

         grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

         if ( l1_idle_data->reselection_apply_freq_correction )
         {
           l1_sc_reselection_apply_freq_correction( l1_tskisr_blk->current_params.ARFCN, gas_id);
           l1_idle_data->reselection_apply_freq_correction = FALSE;
         }
         /* we got here from another mode e.g.dedicated/findSCH/initACQ and already have the lock */
         l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_RUNNING;
         if ( L1_get_bcch(&ms_reselection_frames_until_active, gas_id))
         {
           l_abort_find_bcch=TRUE;

           l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;

           call_in_task_grm_release( l1_tskisr_blk->client_id );
         }
       }
       else if ( trm_status == GL1_TRM_ABORTING )
       {
         call_in_task_grm_release( l1_tskisr_blk->client_id );
       }
       else
       {
         /* get TRM immediately for change in timings */
         if ( l1_tskisr_blk->command == L1_DS_RESELECTION_APPLY_TIME_CHANGE )
         {
         grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
         grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

         grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

         /* Ask for the amount of time required for a complete reselection */
         grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
         if (grm_get_acquisition_priority(gas_id) == GERAN_PRIORITY_INVERTED)
         {
           grm_request_and_notify_enhanced_data_p->reason    = TRM_CHANNEL_MAINTENANCE_INV;
         }
         else
         {
           grm_request_and_notify_enhanced_data_p->reason    = TRM_CHANNEL_MAINTENANCE;
         }
         grm_request_and_notify_enhanced_data_p->sub_reason = subreason;
         grm_request_and_notify_enhanced_data_p->resource  =
         gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );

           /* trm duration required is the minimum as every bcch read is doing trm reservation */
           grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk( BCCH_DURATION_MS, T_MSEC );

         grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_RESELECTION;
         /*Add bands to freq_info*/
         grm_clear_freq_info(l1_tskisr_blk->client_id, gas_id);
          grm_add_trm_band_to_freq_input_type(gl1_map_gsm_band_to_sys_band(l1_tskisr_blk->next_params.ARFCN.band),
                                              l1_tskisr_blk->client_id,
                                              gas_id); 
         grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);
         l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_TRM_WAIT;

         call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );
           l1_reselection_data_p->trm_request_notify = TRUE;
         }
         else
         {
           /* do the reservaton for next bcch occasion and wait */
#if defined (FEATURE_QTA_IN_FINDBCCH)
            MSG_GERAN_HIGH_2_G("L1_MULTI_SIM_RESELECTION_STOPPED calculate gap to next bcch, bcch mask 0x%x, ext mask 0x%x", l1_idle_data->BCCH_mask, l1_tskisr_blk->current_params.extended_BCCH_mask);
            gap_to_next_bcch = L1_gap_to_next_bcch( gl1_get_FN( gas_id ), l1_idle_data->BCCH_mask, 
                                                    l1_tskisr_blk->current_params.extended_BCCH_mask, gas_id );
#else         
           gap_to_next_bcch = L1_gap_to_next_bcch( gl1_get_FN( gas_id ), SERV_BCCH_MASK_ALL, l1_tskisr_blk->current_params.extended_BCCH_mask, gas_id );
#endif

           if (gap_to_next_bcch != GL1_DEFS_INVALID_FN)
           {
             l1_reselection_data_p->bcch_req_frame = ADD_FN(gl1_get_FN( gas_id ),gap_to_next_bcch);
             l1_reselection_data_p->trm_request_notify = FALSE;

             /* check for the minimum gap left for bcch read */
             if (gap_to_next_bcch >= L1_MULTISIM_MINIMUM_FRAMES_FOR_BCCH_READ)
             {
               L1_multi_sim_bcch_reserve_at( l1_reselection_data_p->bcch_req_frame, gas_id );
             }
             else
             {
               /* If there are not enough frames to reserve trm, state L1_MULTI_SIM_RESELECTION_TRM_WAIT
                * will request try to request TRM immediately or make a new reservation for the next BCCH required */
               MSG_GERAN_HIGH_1_G("not sufficient frames left to read next bcch %d, move to TRM wait state", gap_to_next_bcch);
             }
           }

           l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_TRM_WAIT;

         }
         MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_RESELECTION_STOPPED");
       }

       break;
      }

    case L1_MULTI_SIM_RESELECTION_TRM_WAIT:
      {
#if defined (FEATURE_QTA_IN_FINDBCCH)
        uint32 next_scheduled_bcch;
        next_scheduled_bcch = ADD_FN( gl1_get_FN( gas_id ), L1_gap_to_next_bcch( gl1_get_FN( gas_id ), l1_idle_data->BCCH_mask, l1_tskisr_blk->current_params.extended_BCCH_mask, gas_id ) );
#endif        
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        MSG_GERAN_HIGH_3_G("RESELECTION_TRM_WAIT bcch_req_frame %d trm_status %d firmware_loaded %d",
                           l1_reselection_data_p->bcch_req_frame,trm_status,l1_tskisr_blk->firmware_loaded);


        if ( trm_status == GL1_TRM_NOT_GRANTED ||
             l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
             l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND ||
             l1_tskisr_blk->main_command == L1_IDLE_COMMAND ||
             l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND ||
             l1_tskisr_blk->main_command == L1_GPRS_IDLE_COMMAND )
        {
          /* give up here - call release in case no response has come back */
          call_in_task_grm_release( l1_tskisr_blk->client_id );
          l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
          l_abort_find_bcch=TRUE;
#if defined (FEATURE_QTA_IN_FINDBCCH)
          break;
#endif
        }
#if defined (FEATURE_QTA_IN_FINDBCCH)
        /* Case where a BCCH read was re scheduled, ie next_scheduled_bcch != bcch_req_frame */
        else if ( l1_reselection_data_p->bcch_req_frame != GL1_DEFS_INVALID_FN &&
                  next_scheduled_bcch != l1_reselection_data_p->bcch_req_frame )
        {
          call_in_task_grm_release( l1_tskisr_blk->client_id );
          l1_reselection_data_p->bcch_req_frame = GL1_DEFS_INVALID_FN;
          l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
          break;
        }
#endif
        /* Case where we reached the frame number to request trm */
        else if ( ( l1_reselection_data_p->bcch_req_frame != GL1_DEFS_INVALID_FN  &&
                    gl1_get_FN( gas_id ) == l1_reselection_data_p->bcch_req_frame ) || 
                  trm_status == GL1_TRM_GRANTED || trm_status == GL1_TRM_RETAINED_FOR_ACCESS ) 

        {
          MSG_GERAN_HIGH_1_G("Read BCCH trm_status %d",trm_status);

          /* trm status is NULL for trm reserve only */
          if( trm_status == GL1_TRM_NULL )
          {
            trm_grant_return_enum_type grm_grant_status;
            trm_reason_enum_t  reason_for_find_bcch;
            const uint32 duration_for_find_bcch = L1_SC_DURATION_BCCH;


            if ( gpl1_sys_info_trm_priority_inverted (gas_id) )
            {
              /* Need to use inverted priority to request TRM lock */
              reason_for_find_bcch = TRM_CHANNEL_MAINTENANCE_INV;
            }
            else
            {
              reason_for_find_bcch = TRM_CHANNEL_MAINTENANCE;
            }

            MSG_GERAN_HIGH_1_G("Read BCCH request TRM lock with reason %d",reason_for_find_bcch);

            grm_grant_status = 
                 grm_request( l1_tskisr_blk->client_id,
                              gpl1_multi_sim_convert_frames_to_sclcks(duration_for_find_bcch),
                              reason_for_find_bcch,
                              SYS_PROC_TYPE_SYSTEM_INFO_READING,
                              gl1_ms_get_trm_resource( gas_id, TRM_CHANNEL_MAINTENANCE ),
                              GRM_DIVRX_TRM_REQ_INVALID );

            grm_set_trm_grant_status(grm_grant_status, l1_tskisr_blk->client_id, gas_id);

            if ( grm_grant_status != TRM_DENIAL )
            {
              trm_status = GL1_TRM_GRANTED;
              grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);


              grm_set_trm_grant_fn(gl1_get_FN(gas_id), l1_tskisr_blk->client_id, gas_id);
              grm_set_trm_grant_frames(duration_for_find_bcch, l1_tskisr_blk->client_id, gas_id);

            }
            else
            {

              /* Inform GRR that we were blocked while attempting to read BCCH using block quality UNKNOWN */
              L1_send_MPH_BLOCK_QUALITY_IND(BCCH, GL1_BLOCK_QUALITY_UNKNOWN, gas_id);
              grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER, l1_tskisr_blk->client_id, gas_id);
              grm_set_trm_grant_frames(0, l1_tskisr_blk->client_id, gas_id);

            }
            }
#if defined (FEATURE_QTA_IN_FINDBCCH)
          if( trm_status == GL1_TRM_GRANTED && !l1_tskisr_blk->firmware_loaded )
          {
            l1_tskisr_blk->firmware_loaded = TRUE;
#ifdef FEATURE_QTA
            if ( l1_tskisr_blk->qta_in_progress )
            {
              /*X+G scenario:Issue X2G startup when RX is configured*/
            }
#endif /* FEATURE_QTA*/
	    else
            {
              /* Issue the call to init the firmware and RF */
              gl1_hw_multi_sim_fw_rf_init(TRUE,
                                          l1_task_tcb_read(gas_id),
                                          gsm_l1_wait,
                                          L1_WAIT_HW_SIG,
                                          L1_TIMEOUT_HW_SIG,
                                          GL1_HW_INIT_RX_ONLY,
                                          l1_tskisr_blk->client_id,
                                          FALSE,
                                          gas_id);
            }
            if( l1_tskisr_blk->firmware_loaded)
            {
              l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_RUNNING;
            }
          }
         else if( ( trm_status == GL1_TRM_GRANTED ) && l1_tskisr_blk->firmware_loaded )
         {
           //l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_FW_LOAD_WAIT;
           l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_RUNNING;
         }
#else
          if( ( trm_status == GL1_TRM_GRANTED ) || ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS ) )
          {
            if (l1_tskisr_blk->firmware_loaded)
            {
              l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_FW_LOAD_WAIT;
            }
            else
            {
              /* Issue the call to init the firmware and RF */
              gl1_hw_multi_sim_fw_rf_init(TRUE,
                                          l1_task_tcb_read(gas_id),
                                          gsm_l1_wait,
                                          L1_WAIT_HW_SIG,
                                          L1_TIMEOUT_HW_SIG,
                                          GL1_HW_INIT_RX_ONLY,
                                          l1_tskisr_blk->client_id,
                                          FALSE,
                                          gas_id);
              l1_tskisr_blk->firmware_loaded = TRUE;

              fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
              rf_fw_warmup_timer[gas_id] = RF_FW_WARMUP_FRAME_DELAY;
              l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_FW_LOAD_WAIT;

            }
          }
#endif
          else
          {
            MSG_GERAN_HIGH_2_G("RESELECTION_TRM_WAIT trm_status %d firmware_loaded %d",trm_status,l1_tskisr_blk->firmware_loaded);
            break;
          }
        }
        else if ( trm_status == GL1_TRM_ABORTING )
         {
            call_in_task_grm_release( l1_tskisr_blk->client_id );

            l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
            l_abort_find_bcch=TRUE;
            MSG_GERAN_HIGH_0_G("Multi-SIM RESELECTION Process Aborted");
#if defined (FEATURE_QTA_IN_FINDBCCH)
          break;
#endif
         }

        /* Case where we missed the required frame */
        else if ( trm_status == GL1_TRM_NULL &&  /* trm status is NULL for trm reserve only */
                  l1_reselection_data_p->bcch_req_frame != GL1_DEFS_INVALID_FN && 
                  IS_FRAME_NUM_LATER( gl1_get_FN( gas_id ), l1_reselection_data_p->bcch_req_frame ) )
        {
          /* now just need the next bcch occasion and do the reservaton and wait */
#if defined (FEATURE_QTA_IN_FINDBCCH)
          gap_to_next_bcch = L1_gap_to_next_bcch( gl1_get_FN( gas_id ), l1_idle_data->BCCH_mask, l1_tskisr_blk->current_params.extended_BCCH_mask, gas_id );
#else
          gap_to_next_bcch = L1_gap_to_next_bcch( gl1_get_FN( gas_id ), SERV_BCCH_MASK_ALL, l1_tskisr_blk->current_params.extended_BCCH_mask, gas_id );
#endif
          /* check for the minimum, as when other sub is not there then trm wil be granted immediately */
          if( ( gap_to_next_bcch!= GL1_DEFS_INVALID_FN ) && ( gap_to_next_bcch >= L1_MULTISIM_MINIMUM_FRAMES_FOR_BCCH_READ ) )
          {
            l1_reselection_data_p->bcch_req_frame = ADD_FN(gl1_get_FN( gas_id ),(gap_to_next_bcch));
            l1_reselection_data_p->trm_request_notify = FALSE;

            /* do TRM reservation and wait */
            L1_multi_sim_bcch_reserve_at( l1_reselection_data_p->bcch_req_frame, gas_id );

            /* stay here untill gap to next bcch is > 2 */
            l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_TRM_WAIT;
          }
          else
          {
            MSG_GERAN_HIGH_1_G("not sufficient frames for reservation %d",gap_to_next_bcch);
          }
#if defined (FEATURE_QTA_IN_FINDBCCH)
          break;
#endif
        }
        else
        {
          break;
        }
      }

#if !defined (FEATURE_QTA_IN_FINDBCCH)
    case L1_MULTI_SIM_RESELECTION_FW_LOAD_WAIT:
      {
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( fw_rf_setting_up[gas_id] > 0 )
        {
          fw_rf_setting_up[gas_id]--;
        }
        /* Add some debug F3 to investigate BCCH scheduling timing */
        MSG_GERAN_HIGH_1_G("RESELECTION_FW_LOAD_WAIT FNmod51=%d",
                           frame_counters[gas_id].FNmod51);

        /* Time is now ticking keep watch for an abort */
        /* as this may happen at any time during this state too */
        if ( trm_status == GL1_TRM_ABORTING ||
             l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
        {
          if( fw_rf_setting_up[gas_id] == 0 )
          {
            call_in_task_grm_release( l1_tskisr_blk->client_id );

            l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
            l_abort_find_bcch=TRUE;

            MSG_GERAN_HIGH_0_G("Multi-SIM RESELECTION Process Aborted");
          }
          else
          {
            MSG_GERAN_HIGH_1_G("Delay grm release FN = %d",gl1_get_FN( gas_id ));
          }
        }
        else
        if ( l1_tskisr_blk->firmware_loaded == TRUE )
        {
          /* This call is only to now make sure that this client allows pre-emption to take place
          as this doesn't take place within the minimum duration time specified using the RANE */
          grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

          /* retain lock for the case of req&notify */
          if ( l1_reselection_data_p->trm_request_notify )
          {
            grm_retain_lock( l1_tskisr_blk->client_id,
                           (grm_unlock_callback_t)&grm_abort_callback,
                           gas_id );
          }

          /* now we have the lock apply the timing change once only here */
          if ( l1_tskisr_blk->command == L1_DS_RESELECTION_APPLY_TIME_CHANGE )
          {
            /* change frame number */
            (void)L1_change_FN(l1_tskisr_blk->current_params.frame_lag, gas_id);

            gl1_msg_correct_timing((int32)l1_tskisr_blk->current_params.QB_lag,gas_id);
            if ( l1_idle_data->reselection_apply_freq_correction )
            {
              l1_sc_reselection_apply_freq_correction( l1_tskisr_blk->current_params.ARFCN, gas_id);
              l1_idle_data->reselection_apply_freq_correction = FALSE;
            }
            /* we will start out on timeslot 0 */
            l1_set_current_timeslot(0, gas_id);

            l1_sc_set_sync(l1_tskisr_blk->current_params.frame_lag, /*FN*/
                           l1_tskisr_blk->current_params.QB_lag,     /*qbit_offset*/
                           gas_id);

            l1_tskisr_blk->command = L1_NULL_COMMAND;

            /* Send a reselection cnf if necessary */
            if (l1_tskisr_blk->current_params.reselection_cnf_reqd )
            {
              L1_send_MPH_RESELECTION_CNF( TRUE, gas_id );
              l1_tskisr_blk->current_params.reselection_cnf_reqd = FALSE;
              l1_tskisr_blk->next_params.reselection_cnf_reqd = FALSE;
            }
          }
          else
          {
            if (rf_fw_warmup_timer[gas_id] != 0)
            {
			  /* for bcch read go to RUNNING state immediately */
              rf_fw_warmup_timer[gas_id]--;
            }
          }

          /* add in some RF FW settling time to prevent any RSSI=0 issues */
          if ( rf_fw_warmup_timer[gas_id] == 0 )
          {
            l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_RUNNING;
          }
          else
          {
            MSG_GERAN_HIGH_0_G("waiting for rf/fw warmup time");
          }
        }
        break;
      }
#endif
    case L1_MULTI_SIM_RESELECTION_RUNNING:
      {
      /* If TRM is forcing an abort the main command must be set to something other than L1_NULL_COMMAND
    to abort the L1_get_bcch state machine. If however the main command is anything other than L1_NULL_COMMAND
    (for e.g. L1_IDLE_COMMAND if we are about to abort find bcch to move into idle) then there is no need to do
    this - the TRM lock will be released when we move through the L1_MULTI_SIM_RESELECTION_SUSPENDING state. In
    this case we need ensure the main command is preserved such that it is actioned when moving out of find bcch */
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

        if (trm_status == GL1_TRM_ABORTING && l1_tskisr_blk->main_command == L1_NULL_COMMAND)
        {
          l1_tskisr_blk->main_command = L1_MULTI_SIM_TRM_ABORTING_COMMAND;
          MSG_GERAN_HIGH_0_G("GL1_TRM_ABORTING force abort to take place");
        }
#if defined (FEATURE_QTA_IN_FINDBCCH)

        else
        {
           /* Add some debug F3 to investigate BCCH scheduling timing */
            MSG_GERAN_HIGH_2_G("RESELECTION_RUNNING FNmod51=%d active in %d",
                               frame_counters[gas_id].FNmod51,
                               ms_reselection_frames_until_active);
        }
#endif
        /* Tick the state machine - any change of main command will terminate this. So
        for a reselection_power_scan or deactivate this will terminate */
        leaving_bcch = L1_get_bcch(&ms_reselection_frames_until_active,gas_id);

        if ( leaving_bcch )
        {
          /* The main command has changed or the search is completed */
          suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY;

          l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_SUSPENDING;
        }
        /* if L1_get_bcch() returns a large gap here, do TRM reservation */
        else if ( ms_reselection_frames_until_active != GL1_DEFS_INVALID_FN &&
                  ms_reselection_frames_until_active >= L1_MULTISIM_MINIMUM_FRAMES_FOR_BCCH_READ 
                  && (FALSE == grm_get_resource_lock_state(gas_id))
                )
        {
          l1_reselection_data_p->trm_request_notify = FALSE;
          l1_reselection_data_p->bcch_req_frame = ADD_FN(gl1_get_FN( gas_id ),(ms_reselection_frames_until_active ));

          /* do TRM reservation and wait */
          L1_multi_sim_bcch_reserve_at( l1_reselection_data_p->bcch_req_frame, gas_id );

          /* no need for a suspension delay here because the FW should be inactive already */
          l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_TRM_WAIT;

          MSG_GERAN_HIGH_1_G("switch to RESELECTION_TRM_WAIT bcch_req_frame %d",l1_reselection_data_p->bcch_req_frame);
       }
       break;
     }

   case L1_MULTI_SIM_RESELECTION_SUSPENDING:
     {
       if ( suspension_timer[gas_id] > 0)
       {
         suspension_timer[gas_id]--;
       }

       /* Stay here to allow the suspension to provide the msg/hw layers with
          "suspension_timer" frames to terminate */
       if( L1_get_bcch( &ms_reselection_frames_until_active, gas_id ) && ( suspension_timer[gas_id] == 0 ) )
       {

         /* This main command is used simply to abort the L1_get_bcch state machine */
         if ( l1_tskisr_blk->main_command == L1_MULTI_SIM_TRM_ABORTING_COMMAND )
         {
           l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         }

         l_abort_find_bcch=TRUE;

         if(l1_tskisr_blk->main_command != L1_ACCESS_COMMAND)
         {
           call_in_task_grm_release( l1_tskisr_blk->client_id );
         }

         l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;

         /* go back to Dual Sim Null state and wait for further commands */
         MSG_GERAN_HIGH_0_G("Multi-SIM L1_multi_sim_reselection() suspended");
       }
       else
       {
         MSG_GERAN_HIGH_0_G("L1_multi_sim_reselection() suspension timer running");
       }
     }
     break;

    case L1_MULTI_SIM_RESELECTION_ABORT:
      {
        if (L1_get_bcch(&ms_reselection_frames_until_active, gas_id))
        {
          l_abort_find_bcch=TRUE;
          l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
          call_in_task_grm_release( l1_tskisr_blk->client_id );
        }
        break;
      }

    default:
      MSG_GERAN_HIGH_0_G("Unexpected dsim reselection state.");
      break;

  } /* of switch */

  gl1_drx_require_next_tick(gas_id);
  return(l_abort_find_bcch);
}
/*===========================================================================

FUNCTION  L1_MULTI_SIM_POWER_SCAN

DESCRIPTION
  This function handles the L1 dual sim power scan

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean L1_multi_sim_power_scan ( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  boolean abort_allowed = TRUE;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  sys_proc_type_e_type subreason = gl1_get_find_bcch_subreason(gas_id);
  boolean mcpm_ongoing;
  boolean embms_active = grm_is_embms_active(gas_id);

   switch ( l1_tskisr_blk->multi_sim_pscan_state )
   {

   case L1_MULTI_SIM_PSCAN_STOPPED:
     {
       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
       if(l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
       {
         MSG_GERAN_HIGH_0_G("FW Load Status: Release pending, wait for another tick for issuing RANE");
       }
       else if ( (trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
       {	 
         if(l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND)
         {
           /* Cancel the trm request, and clear down */
           call_in_task_grm_release( l1_tskisr_blk->client_id );
         }
         else
         {
         /*need to load the fw if not already loaded*/
         if(l1_tskisr_blk->firmware_load_status == FW_UNLOADED)
         {
            gl1_multi_sim_fw_rf_load( l1_tskisr_blk->client_id, gas_id );
         }
         /* we got here from another mode e,g, dedicated and already have the lock and FW so tell TRM that
         we want to extend the duration of the lock and provide it the oppertunity to deny with an abort
         callback. Set a default frame duration for initial use */
         l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT;
         rf_fw_warmup_timer[gas_id] = RF_FW_WARMUP_FRAME_DELAY;
         grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
         fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
   
         /*GL1 must not set frame duration & register unlock callback if DS_ABORT is received during or before the L1_MULTI_SIM_PSCAN_STOPPED state
             When DS_ABORT received frame duration is set to COMPLETE_PROCEDURE_DURATION & unlock_callback to NULL*/
         if (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) > COMPLETE_PROCEDURE_DURATION/2 )
         {
           /* don't retain the lock, unlock callback is set to NULL in DS_ABORT*/
           MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_PSCAN_STOPPED don't set frame duration or unlock callback, frame_duration: %d", grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
         }
         else
         {
            grm_set_frame_duration(RETAIN_LOCK_DEFAULT_FRAME_DURATION, l1_tskisr_blk->client_id, gas_id);        
            /* retain the lock here but do not respond to UNLOCK_REQUIRED events yet */
            grm_retain_lock_for_acquisition(l1_tskisr_blk->client_id, GRM_IGNORE_UNLOCK_REQUIRED,TRM_ACQUISITION,subreason,gas_id);
         }
        }
       }
       else
       if ( trm_status == GL1_TRM_ABORTING )
       {
         call_in_task_grm_release( l1_tskisr_blk->client_id );
       }
       else
       /* If we're going to transition out of L1 FIND_BCCH STATE in this frame,
          do not Request and Notify Enhanced. It is possible for a deactivate
          to cross over here when we kick off a power scan.  Check for this
          before asking TRM for the lock otherwise this can cause a lock up */
       if(l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND &&
          l1_tskisr_blk->main_command != L1_ENTER_NULL_COMMAND)
       {
         /* Send GFW sleep cmd, then fws_suspend so that we can call MCPM 
            INIT_REQ, which calls fws_sleep. This way we use less power 
            while waiting for TRM grant cb. */

         if(gl1_hw_sleep_try_fw_sleep(gas_id))
       {
         grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
         grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

           /* TRM Request... */

         grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

         /* Ask for the amount of time required for a complete reselection */
         grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
         grm_request_and_notify_enhanced_data_p->reason    = TRM_ACQUISITION;
         grm_request_and_notify_enhanced_data_p->sub_reason = subreason;
         grm_request_and_notify_enhanced_data_p->resource  =
           gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );
         grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(grm_get_acquisition_duration(gas_id), T_MSEC);

         grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_ACQUISITION;

         grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);

         call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );

           /* MCPM Request(s)... (will all get queued up and run in DPC context) */
             if(embms_active == FALSE)
             {
           /* make sure we release MCVS requests before MCPM_GERAN_INIT_REQ */
           l1_pscan_mcpm_change(FALSE, gas_id);

           MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_STOPPED: MCPM_GERAN_INIT_REQ!");
           gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_INIT_REQ, GL1_MCPM_GERAN_INIT, gas_id);
              }
           abort_allowed = FALSE;

           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_TRM_WAIT;
       }
       else
       {
           MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_STOPPED: Waiting for FW to sleep");
         }
       }
       else
       {
         MSG_GERAN_HIGH_0_G("Deactivation has prevented this MS PSCAN");
       }
       break;
     }

   case L1_MULTI_SIM_PSCAN_TRM_WAIT:
     {
      abort_allowed = FALSE;

      /* This state will conditionally send an MCPM_GERAN_IDLE_REQ based on 
         gl1_hw_mcpm_get_state(), but we need to be sure there are no ongoing
         configs before reading that function. */
      if(gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id))
      {
        MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_TRM_WAIT: MCPM ongoing");
        break;
      }

       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
       if ( trm_status == GL1_TRM_NOT_GRANTED ||
            trm_status == GL1_TRM_ABORTING ||
            trm_status == GL1_TRM_NULL ||
            l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
            l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND )
       {
        /* Trm has denied the lock, or we need to give up */

        /* Undo our power saving steps so we're back in original MCPM/MCVS state */
        MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_TRM_WAIT: ABORT!");

        if(gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
        {
          gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);
        }
  
        l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_MCPM_WAIT_ABORT;
      }
      else if(trm_status == GL1_TRM_GRANTED || trm_status == GL1_TRM_RETAINED_FOR_ACCESS)
      {
        MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_TRM_WAIT: MCPM_GERAN_IDLE_REQ!");
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);
        l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_MCPM_WAIT;
      }
      else
      {
        MSG_GERAN_LOW_1_G("L1_MULTI_SIM_PSCAN_TRM_WAIT (trm_status=%d)",(int)(trm_status));
      }
      break;
    }

   case L1_MULTI_SIM_PSCAN_MCPM_WAIT:
     {
       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
       if ( trm_status == GL1_TRM_NOT_GRANTED ||
            trm_status == GL1_TRM_ABORTING ||
            trm_status == GL1_TRM_NULL ||
            l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
            l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND )
       {
         /* Trm has denied the lock, or we need to give up */

         MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_MCPM_WAIT: ABORT!");
         l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_MCPM_WAIT_ABORT;

         abort_allowed = FALSE;
         /* we will fall through to L1_MULTI_SIM_PSCAN_MCPM_WAIT_ABORT */
       }
       else
       {
         mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
         if(mcpm_ongoing ||
            gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
         {
           /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */

           MSG_GERAN_HIGH_3_G("GL1_PWR: PSCAN_MCPM_WAIT: wait another frame (trm_status %d MCPM ongoing %d MCPM status %d)",
                              (int)(trm_status), mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));
           abort_allowed = FALSE;
       }
       else
         {
           if (l1_tskisr_blk->firmware_loaded == FALSE)
           {
           MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_MCPM_WAIT: loading FW/RF!");
           /* once we've moved back to MCPM_IDLE state, make our MCVS request for PSCAN */
           l1_pscan_mcpm_change(TRUE, gas_id);

             if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
             {
               MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
             }             
           }
  
           if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
       {
             MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
           }
  
           /* Need to load the RF and FW now, and only move to runnning once ready, this will be
           indicated by the flag l1_tskisr_blk->firmware_loaded */
           gl1_multi_sim_fw_rf_load( l1_tskisr_blk->client_id, gas_id );

           fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
           rf_fw_warmup_timer[gas_id] = RF_FW_WARMUP_FRAME_DELAY;
           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT;
           if (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) > COMPLETE_PROCEDURE_DURATION/2 )
           {
             /* don't retain the lock, unlock callback is set to NULL in DS_ABORT*/
             MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT don't set unlock callback frame_duration %d", grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
           }
           else
           {
             /* retain the lock and respond to UNLOCK_REQUIRED events from now on */
             grm_retain_lock_for_acquisition(l1_tskisr_blk->client_id, GRM_ALLOWS_UNLOCK_REQUIRED, TRM_ACQUISITION,subreason,gas_id);
           }
        }
         break;
       }

       /* can fall through */
     }

     case L1_MULTI_SIM_PSCAN_MCPM_WAIT_ABORT:
     {
        /* Wait in this state until MCPM_GERAN_IDLE_REQ has completed. Only then can we proceed with the abort. */

        mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
        if(mcpm_ongoing ||
           gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
        {
          /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */

          abort_allowed = FALSE;

          MSG_GERAN_HIGH_2_G("GL1_PWR: PSCAN_MCPM_WAIT_ABORT: wait another frame (MCPM ongoing %d MCPM status %d)",
                             mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));
        }
       else
       {
          /* Undo MCVS change */
          l1_pscan_mcpm_change(TRUE, gas_id);

          /* Undo FW Sleep */
          if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
          {
            MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
       }

          /* Release TRM lock (if holding it) */
          call_in_task_grm_release(l1_tskisr_blk->client_id);

          l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_STOPPED;

          MSG_GERAN_HIGH_0_G("GL1_PWR: PSCAN_MCPM_WAIT_ABORT: Now aborting pscan");
        }
       break;
     }

   case L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT:
     {
       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( fw_rf_setting_up[gas_id] > 0 )
        {
          fw_rf_setting_up[gas_id]--;
        }

       if (trm_status == GL1_TRM_NOT_GRANTED) 
       {
          MSG_GERAN_HIGH_0_G("Retire PSCAN_FW_LOAD_WAIT, TRM Not Granted");
          grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);
          l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_STOPPED;
       }
       else
       if( l1_tskisr_blk->firmware_load_status == FW_LOAD_IN_PROGRESS )
       {
         MSG_GERAN_HIGH_2_G("L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT FN = %d frame_duration = %d",
                            gl1_get_FN( gas_id ),
                            grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
       }
       /* Time is now ticking by decrement the frame count and keep watch for an abort */
       /* as this may happen at any time during this state too */
       else
       if ((grm_get_post_decrement_frame_duration(l1_tskisr_blk->client_id, gas_id) == 0) || 
           (trm_status == GL1_TRM_ABORTING) ||
           (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
          )
       {
         if( fw_rf_setting_up[gas_id] == 0 )
         {
           call_in_task_grm_release( l1_tskisr_blk->client_id );

           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_STOPPED;
         }
         else
         {
           MSG_GERAN_HIGH_1_G("Delay grm release FN = %d",gl1_get_FN( gas_id ));
         }
       }
       else
       if ( l1_tskisr_blk->firmware_loaded == TRUE )
       {
         grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

         /* add in some RF FW settling time to prevent any RSSI=0 issues */
         if ( rf_fw_warmup_timer[gas_id] == 0 )
         {
           L1_pscan_resume(gas_id);
           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_RUNNING;

           /*GL1 must not register unlock callback if DS_ABORT is received during the FW_LOAD state
                   When DS_ABORT received frame duration is set to COMPLETE_PROCEDURE_DURATION & unlock_callback to NULL*/
           if (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) > COMPLETE_PROCEDURE_DURATION/2 )
           {
             /* don't retain the lock, unlock callback is set to NULL in DS_ABORT*/
             MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT don't set unlock callback frame_duration %d", grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
           }
           else
           {
             /* retain the lock and respond to UNLOCK_REQUIRED events from now on */
             grm_retain_lock_for_acquisition(l1_tskisr_blk->client_id, GRM_ALLOWS_UNLOCK_REQUIRED,TRM_ACQUISITION,subreason,gas_id);
           }
         }
         else
         {
           rf_fw_warmup_timer[gas_id]--;
           /* Clear any remaining message layer pscan configs */
           gl1_msgi_pwr_meas_init(gas_id);

           MSG_GERAN_HIGH_0_G("waiting for rf/fw warmup time");
         }
       }
       else
       {
         MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT %d",
                            grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
       }
       break;
     }

   case L1_MULTI_SIM_PSCAN_RUNNING:
     {
    /* decrement allowed frames if no frames are left then the power scan must be suspended
     * otherwise run as normal. This must also happen if TRM calls the Abort callback */
       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
       if ((grm_get_post_decrement_frame_duration(l1_tskisr_blk->client_id, gas_id) == 0) || 
           (trm_status == GL1_TRM_ABORTING) ||
           (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
           )
       {

         if ( l1_tskisr_blk->pscan_state != L1_SCAN_START )
         {
           L1_pscan_suspend(0,gas_id);
           L1_power_scan(gas_id);

           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_SUSPENDING;
           suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY;
           abort_allowed = FALSE;
         }
         else
         {
           /* The pscan hasn't had a chance to start yet, can be aborted immediately */
           call_in_task_grm_release( l1_tskisr_blk->client_id );

           MSG_GERAN_HIGH_0_G("Multi-SIM PSCAN Process Not yet started");
           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_STOPPED;
         }

       }
       else
       {
         L1_power_scan(gas_id);

         if ( l1_tskisr_blk->pscan_state == L1_SCAN_END )
         {
           /* The scan is completed finish by cleaning up here */
           if(grm_get_resource_lock_state(gas_id) == FALSE)
             {
               call_in_task_grm_release( l1_tskisr_blk->client_id );
             }

           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_ABORT;

           /* go back to Dual Sim Null state and wait for further commands */
           MSG_GERAN_HIGH_0_G("Multi-SIM PSCAN Process completed");
         }
       }

    break;
     }

   case L1_MULTI_SIM_PSCAN_ABORT:
     /* Stay here whilst everything is finished to prevent any inproper calls to TRM */
    break;

   case L1_MULTI_SIM_PSCAN_SUSPENDING:
     {
       if(suspension_timer[gas_id] > 0)
       {
          suspension_timer[gas_id]--;
       }
       /* Stay here to allow the suspension to provide the msg/hw layers with a couple of frames to terminate */
       if(suspension_timer[gas_id] == 0)
       {
         call_in_task_grm_release( l1_tskisr_blk->client_id );

         if ( l1_tskisr_blk->pscan_state == L1_SCAN_END )
         {
           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_ABORT;
           MSG_GERAN_HIGH_0_G("Multi-SIM PSCAN Process completed");
         }
         else
         {
           l1_tskisr_blk->multi_sim_pscan_state = L1_MULTI_SIM_PSCAN_STOPPED;
         }

         /* go back to Dual Sim Null state and wait for further commands */
         MSG_GERAN_HIGH_0_G("Multi-SIM PSCAN Process suspended");
       }
       else
       {
         abort_allowed = FALSE;
         MSG_GERAN_HIGH_0_G("pscan suspension timer running");
       }

       L1_power_scan(gas_id);
     }
     break;

   default:
      MSG_GERAN_HIGH_0_G("Unexpected l1 sub sub state.");
      break;

   } /* of switch */

  gl1_drx_require_next_tick(gas_id);

  return abort_allowed;
}

/*===========================================================================

FUNCTION  L1_multi_sim_get_bcch_list_fcch_sch_bcch

DESCRIPTION


DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_multi_sim_get_bcch_list_fcch_sch_bcch( gas_id_t gas_id )
{
   /* locals */
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   boolean l_abort_find_bcch = FALSE;
   gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
   sys_proc_type_e_type subreason = gl1_get_find_bcch_subreason(gas_id);
   boolean mcpm_ongoing;
   static boolean priority_bumped_up[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
   uint16 gap_before_bcch_FN =0;
   uint32 gap_before_suspension=0xFFFFFFFF;
   boolean embms_active = grm_is_embms_active(gas_id);

   if(trm_status == GL1_TRM_GRANTED)
   {
      /*If TRM Status is granted, then decrement a frame duration as we are running the isr now*/
      (void)grm_get_post_decrement_frame_duration(l1_tskisr_blk->client_id, gas_id);
   }

   /* Code starts */
   switch ( l1_tskisr_blk->multi_sim_acq_bcch_list_state)
   {
     case L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED:
      {
       priority_bumped_up[gas_id] = FALSE;        
       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
       if(l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
       {
         MSG_GERAN_HIGH_0_G("FW Load Status: Release pending, wait for another tick for issuing RANE");
       }
       else if ( (trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
       {
         /* we got here from another mode e,g, dedicated and already have the lock and FW so tell TRM that
         we want to extend the duration of the lock and provide it the oppertunity to deny with an abort
         callback. Set a default frame duration for initial use */
         if( l1_tskisr_blk->firmware_loaded )
         {
           l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_FW_LOAD_WAIT;
         }
         else
         {
           l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_TRM_WAIT;

           MSG_GERAN_HIGH_0_G(" L2G CSFB REDIR goto L1_MULTI_SIM_ACQ_BCCH_LIST_TRM_WAIT ");
         }
         rf_fw_warmup_timer[gas_id] = RF_FW_WARMUP_FRAME_DELAY;
         grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
         fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;

         /*GL1 must not set frame duration & register unlock callback if DS_ABORT is received during or before the L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED state
             When DS_ABORT received frame duration is set to COMPLETE_PROCEDURE_DURATION & unlock_callback to NULL*/
         if (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) > COMPLETE_PROCEDURE_DURATION/2 )
         {
            /* Don't accept callbacks, unlock callback is set to NULL in DS_ABORT*/
            MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED don't set frame duration or unlock callback, frame_duration %d", grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
         }
         else
         {
            grm_set_frame_duration(RETAIN_LOCK_DEFAULT_FRAME_DURATION, l1_tskisr_blk->client_id, gas_id);
            /* retain the lock here but do not respond to UNLOCK_REQUIRED events yet */
            grm_retain_lock_for_acquisition(l1_tskisr_blk->client_id, GRM_IGNORE_UNLOCK_REQUIRED,TRM_ACQUISITION,subreason, gas_id);
         }
       }
       else
       if ( trm_status == GL1_TRM_ABORTING )
       {
         call_in_task_grm_release( l1_tskisr_blk->client_id );
       }
       else
       {
         /* Send GFW sleep cmd, then fws_suspend so that we can call MCPM 
            INIT_REQ, which calls fws_sleep. This way we use less power 
            while waiting for TRM grant cb. */

         if(gl1_hw_sleep_try_fw_sleep(gas_id))
         {
         grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
         grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

           /* TRM Request... */

         grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

         /* Ask for the amount of time required for a complete reselection */
         grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
         grm_request_and_notify_enhanced_data_p->reason    = TRM_ACQUISITION;
         grm_request_and_notify_enhanced_data_p->sub_reason = subreason;
         grm_request_and_notify_enhanced_data_p->resource  =
           gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );
         grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(grm_get_acquisition_duration(gas_id), T_MSEC);

         grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_ACQUISITION;

         /* provide abort call back to TRM to allow pre-emption */
         grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);

         /* call TRM for a period of time to be allocated */
         call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );

           /* MCPM Request(s)... (will all get queued up and run in DPC context) */

            /* Skip moving to MCPM_GERAN_INIT_REQ if embms is active*/
            if (embms_active == FALSE)
            {
           l1_find_bcch_mcpm_change(FALSE, gas_id);
           MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_STOPPED: MCPM_GERAN_INIT_REQ!");
           gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_INIT_REQ, GL1_MCPM_GERAN_INIT, gas_id);
            }
            else
            {
              MSG_GERAN_HIGH_2_G("Skip MCPM Init, embms_active:%d, mcpm_state:%d",embms_active, gl1_hw_mcpm_get_state(gas_id));
            }
           l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_TRM_WAIT;

       }
         else
         {
           MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_STOPPED: Waiting for FW to sleep");
         }
       }
        break;
      }

     case L1_MULTI_SIM_ACQ_BCCH_LIST_TRM_WAIT:
      {
        boolean fall_through = FALSE;

        /* This state will conditionally send an MCPM_GERAN_IDLE_REQ based on 
           gl1_hw_mcpm_get_state(), but we need to be sure there are no ongoing
           configs before reading that function. */
        if(gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id))
        {
          MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_TRM_WAIT: MCPM ongoing");
          break;
        }

        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( trm_status == GL1_TRM_NOT_GRANTED ||
             trm_status == GL1_TRM_ABORTING ||
             l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
             l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND ||
             l1_tskisr_blk->main_command == L1_IDLE_COMMAND ||
             l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND ||
             l1_tskisr_blk->main_command == L1_GPRS_IDLE_COMMAND )
        {
          /* Trm has denied the lock, or we need to give up */

          MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_TRM_WAIT: ABORT!");

          /* Undo our power saving steps so we're back in original MCPM/MCVS state */
          if(gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
          {
            gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);
          }

          l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_MCPM_WAIT_ABORT;
        }
        else if(trm_status == GL1_TRM_GRANTED || 
                trm_status == GL1_TRM_RETAINED_FOR_ACCESS)
        {
          if(gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_TRM_WAIT: MCPM_GERAN_IDLE_REQ!");
            gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_IDLE_REQ, GL1_MCPM_GERAN_IDLE, gas_id);
          }
          else
          {
            MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_TRM_WAIT: MCPM_GERAN_IDLE_REQ already done.");
            fall_through = TRUE;
          }
          l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_MCPM_WAIT;
        }
        else
        {
          MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_ACQ_BCCH_LIST_TRM_WAIT (trm_status=%d)",(int)(trm_status));
        }

        if(!fall_through)
        {
          break;
        }
      }
     
      case L1_MULTI_SIM_ACQ_BCCH_LIST_MCPM_WAIT:
      {
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( trm_status == GL1_TRM_NOT_GRANTED ||
             trm_status == GL1_TRM_ABORTING ||
             l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ||
             l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND ||
             l1_tskisr_blk->main_command == L1_IDLE_COMMAND ||
             l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND ||
             l1_tskisr_blk->main_command == L1_GPRS_IDLE_COMMAND )
        {
          /* Trm has denied the lock, or we need to give up */

          MSG_GERAN_HIGH_0_G("GL1_PWR: ACQ_BCCH_LIST_MCPM_WAIT: ABORT!");
          l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_MCPM_WAIT_ABORT;

          /* we will fall through to L1_MULTI_SIM_ACQ_BCCH_LIST_MCPM_WAIT_ABORT */
        }
        else
        {
          mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
          if(mcpm_ongoing ||
             gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
          {
            /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */

            MSG_GERAN_HIGH_3_G("GL1_PWR: BCCH_LIST_MCPM_WAIT: wait another frame (trm_status %d MCPM ongoing %d MCPM status %d)",
                               (int)(trm_status), mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));
        }
        else
          {
              if ( l1_tskisr_blk->firmware_loaded == FALSE )
              {
            MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_MCPM_WAIT: loading FW/RF!");
            /* once we've moved back to MCPM_IDLE state, make our MCVS request for BCCH_LIST */
            l1_find_bcch_mcpm_change(TRUE, gas_id);
  
            if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
        {
              MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
            }
         }
  
            /* Need to load the RF and FW now, and only move to running once ready, this will be
            indicated by the flag l1_tskisr_blk->firmware_loaded */
            gl1_multi_sim_fw_rf_load( l1_tskisr_blk->client_id, gas_id );

            fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
            rf_fw_warmup_timer[gas_id] = RF_FW_WARMUP_FRAME_DELAY;
            l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_FW_LOAD_WAIT;
        }
          break;
        }

        /* can fall through */
      }

      case L1_MULTI_SIM_ACQ_BCCH_LIST_MCPM_WAIT_ABORT:
      {
        /* Wait in this state until MCPM_GERAN_IDLE_REQ has completed. Only then can we proceed with the abort. */
        mcpm_ongoing = gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id);
        if(mcpm_ongoing ||
           gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_IDLE)
        {
          /* We still need to wait for the MCPM_GERAN_IDLE_REQ to complete (so do nothing) */

          MSG_GERAN_HIGH_2_G("GL1_PWR: BCCH_LIST_MCPM_WAIT_ABORT: wait another frame (MCPM ongoing %d MCPM status %d)",
                             mcpm_ongoing, gl1_hw_mcpm_get_state(gas_id));
        }
      else
        {
          /* Undo MCVS change */
          l1_find_bcch_mcpm_change(TRUE, gas_id);
      
          /* Undo FW Sleep */
          if(!gl1_hw_sleep_try_fw_wakeup(gas_id))
          {
            MSG_GERAN_FATAL_0_G("Failed to wake up GFW");
          }
      
          /* Release TRM lock (if holding it) */
          call_in_task_grm_release( l1_tskisr_blk->client_id );
      
          l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED;
          l_abort_find_bcch = TRUE;

          MSG_GERAN_HIGH_0_G("GL1_PWR: BCCH_LIST_MCPM_WAIT_ABORT: Now aborting find_bcch");
        }
        break;
      }

     case L1_MULTI_SIM_ACQ_BCCH_LIST_FW_LOAD_WAIT:
      {
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( fw_rf_setting_up[gas_id] > 0 )
        {
          fw_rf_setting_up[gas_id]--;
        }

        if (trm_status == GL1_TRM_NOT_GRANTED) 
        {
          MSG_GERAN_HIGH_0_G("Retire ACQ_BCCH_LIST_FW_LOAD_WAIT, TRM Not Granted");
          grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);
          l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED;
        }
        else
        if( l1_tskisr_blk->firmware_load_status == FW_LOAD_IN_PROGRESS )
        {
          MSG_GERAN_HIGH_2_G("L1_MULTI_SIM_PSCAN_FW_LOAD_WAIT FN = %d frame_duration = %d",
                             gl1_get_FN( gas_id ),
                             grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
        }
        /* Time is now ticking by decrement the frame count and keep watch for an abort */
        /* as this may happen at any time during this state too */
        else
        if ((grm_get_post_decrement_frame_duration(l1_tskisr_blk->client_id, gas_id) == 0) || 
            (trm_status == GL1_TRM_ABORTING) ||
            (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
            )
        {
          if( fw_rf_setting_up[gas_id] == 0 )
          {
            call_in_task_grm_release( l1_tskisr_blk->client_id );
            l_abort_find_bcch = TRUE;
            l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED;
          }
          else
          {
            MSG_GERAN_HIGH_1_G("Delay grm release FN = %d",gl1_get_FN( gas_id ));
          }
        }
        else
        if ( l1_tskisr_blk->firmware_loaded == TRUE )
        {
          grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

          /* add in some RF FW settling time to prevent any RSSI=0 issues */
          if ( rf_fw_warmup_timer[gas_id] == 0 )
          {
            L1_bcch_list_acq_bcch_resume(gas_id);
            l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_RUNNING;

           /*GL1 must not register unlock callback if DS_ABORT is received during the FW_LOAD state
                   When DS_ABORT received frame duration is set to COMPLETE_PROCEDURE_DURATION & unlock_callback to NULL*/
           if (grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) > COMPLETE_PROCEDURE_DURATION/2 )
           {
             /* Don't accept callbacks, unlock callback is set to NULL in DS_ABORT*/
              MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_ACQ_BCCH_LIST_FW_LOAD_WAIT don't set unlock callback frame_duration %d", grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id));
           }
           else
           {
              /* retain the lock and respond to UNLOCK_REQUIRED events from now on */
              grm_retain_lock_for_acquisition(l1_tskisr_blk->client_id, GRM_ALLOWS_UNLOCK_REQUIRED, TRM_ACQUISITION, subreason, gas_id);
            }
          }
          else
          {
            rf_fw_warmup_timer[gas_id]--;
            MSG_GERAN_HIGH_0_G("waiting for rf/fw warmup time");
          }
        }
        break;
      }

     case L1_MULTI_SIM_ACQ_BCCH_LIST_RUNNING:
     {
       trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
       if ((grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) <= SUSPENSION_FRAME_DELAY) || 
           (trm_status == GL1_TRM_ABORTING) ||
           (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
           )
       {

         /* Change the state here as this may be referenced within L1_get_bcch_list_fcch_sch_bcch */
         l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_SUSPENDING;

         L1_bcch_list_acq_bcch_suspend(0, gas_id);
        (void)L1_get_bcch_list_fcch_sch_bcch(gas_id);

         suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY;
         l_abort_find_bcch = FALSE;
         suspension_state_exit_to_abort[gas_id] = FALSE;
       }
       else
       {
         /* check to see if the state has changed, indicating
          * the completion of the BCCH decoding */
         l_abort_find_bcch = L1_get_bcch_list_fcch_sch_bcch(gas_id);

         if((l_abort_find_bcch) || ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_NULL ))
         {
           if ( ( l1_tskisr_blk->main_command != L1_FIND_BCCH_COMMAND ) && ( l1_tskisr_blk->l1_state != L1_MULTI_SIM_NULL ) )
           {
             /* Change the state here as this may be referenced within L1_get_bcch_list_fcch_sch_bcch */
             l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_SUSPENDING;

             suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY;
             l_abort_find_bcch = FALSE;

             suspension_state_exit_to_abort[gas_id] = TRUE;

             MSG_GERAN_MED_0_G("L1_MULTI_SIM_ACQ_BCCH_LIST_RUNNING to L1_MULTI_SIM_ACQ_BCCH_LIST_SUSPENDING");
           }
           else
           {
             /* Release the lock here if leaving the state and returning back to NULL. This implies the list
             decoding has completed but L1 has not been given any instruction to camp on any cells it may have
             found. Or perhaps no cells can be found in a poor service area. */
             call_in_task_grm_release( l1_tskisr_blk->client_id );

             l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED;

             MSG_GERAN_MED_0_G("L1_MULTI_SIM_ACQ_BCCH_LIST_RUNNING to L1_MULTI_SIM_NULL");
           }
         }
         else
         {
          /*should happen in the case of non abort only ?? */
           
           if( l1_BCCH_List[gas_id].bcch_blocked_cnt[l1_BCCH_List[gas_id].current_bcch_index] >=2)
            {
              /* we might enter this loop before scheduling bcch ~non block start or just when decode finished*/

              /* calculate the no. of frames for the bcch frame from the current frame number*/
               if(IS_FRAME_NUM_LATER(l1_BCCH_List[gas_id].next_bcch_fn[l1_BCCH_List[gas_id].current_bcch_index],gl1_get_FN( gas_id)))
                  gap_before_bcch_FN = SUB_FN(l1_BCCH_List[gas_id].next_bcch_fn[l1_BCCH_List[gas_id].current_bcch_index],gl1_get_FN( gas_id));

               /*to avoid doing trm operations in the next subsequent iterations before the bcch is scheduled */
               /* if already the priority is bumped-it must be in after third attempt*/
               if(((priority_bumped_up[gas_id]&&(grm_gl1_get_current_trm_reason(l1_tskisr_blk->client_id,gas_id)==TRM_CHANNEL_MAINTENANCE)&&(l1_BCCH_List[gas_id].bcch_blocked_cnt[l1_BCCH_List[gas_id].current_bcch_index]==FOURTH_ATTEMPT_OF_BCCH_DECODE))) ||
	   	((!priority_bumped_up[gas_id]) && !l1_BCCH_List[gas_id].bcch_decoded[l1_BCCH_List[gas_id].current_bcch_index]))
                {
                   gap_before_suspension =grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id);
                   /*consider only the bcch frames which are min of 15 frames before the suspennsion frame or max of 10 frames after the suspension frame   */
                   if (((gap_before_bcch_FN>=gap_before_suspension)&&(gap_before_bcch_FN-gap_before_suspension<=10) )||((gap_before_suspension>=gap_before_bcch_FN)&&(gap_before_suspension-gap_before_bcch_FN<=15)))
                   {
                       if(l1_BCCH_List[gas_id].bcch_blocked_cnt[l1_BCCH_List[gas_id].current_bcch_index]==THIRD_ATTEMPT_OF_BCCH_DECODE)
					 	{
                      MSG_GERAN_MED_2_G("Change prio to Channel Maintanance for bcch index %d, arfcn %d ",l1_BCCH_List[gas_id].current_bcch_index,l1_BCCH_List[gas_id].arfcn[l1_BCCH_List[gas_id].current_bcch_index].num);
                      priority_bumped_up[gas_id] = grm_change_priority( l1_tskisr_blk->client_id, TRM_CHANNEL_MAINTENANCE, subreason,gas_id );
       }
                     else if(l1_BCCH_List[gas_id].bcch_blocked_cnt[l1_BCCH_List[gas_id].current_bcch_index]==FOURTH_ATTEMPT_OF_BCCH_DECODE)
					 	{
					     MSG_GERAN_MED_2_G("Change prio to Channel MaintananceINV for bcch index %d, arfcn %d ",l1_BCCH_List[gas_id].current_bcch_index,l1_BCCH_List[gas_id].arfcn[l1_BCCH_List[gas_id].current_bcch_index].num);
                        priority_bumped_up[gas_id] = grm_change_priority( l1_tskisr_blk->client_id, TRM_CHANNEL_MAINTENANCE_INV, subreason,gas_id );
                     	}

                   }
                } 
                else if (l1_BCCH_List[gas_id].bcch_decoded[l1_BCCH_List[gas_id].current_bcch_index] == TRUE)
                {
                    MSG_GERAN_MED_2_G("BCCH FN %d, gap %d", gl1_get_FN(gas_id), gap_before_bcch_FN); 
                    /*bcch successfully decoded case  */
                    /*it covers the case where the above if loop is executed but still we have bcch decode failure for reasons like poor SNR*/
           
                    (void)grm_change_priority( l1_tskisr_blk->client_id, grm_get_acquisition_reason(TRM_ACQUISITION, gas_id), subreason,gas_id );
                    priority_bumped_up[gas_id] = FALSE;
                 }
            }
          }         
        } 
       break;
     }

    case L1_MULTI_SIM_ACQ_BCCH_LIST_SUSPENDING:
      {
        /* Stay here whilst everthing is finished to prevent any inproper calls to TRM */
        /* Stay here to allow the suspension to provide the msg/hw layers with a couple of frames to terminate */

        if(suspension_timer[gas_id] > 0)
        {
          suspension_timer[gas_id]--;
        }

        if ( gpl1_multi_sim_bplmn_decode_bcch_inactive(gas_id) && ((suspension_timer[gas_id] == 0) ||(l1_tskisr_blk->acq_bcch_state == L1_BCCH_LIST_INIT)))
        {
          priority_bumped_up[gas_id] = FALSE;
          call_in_task_grm_release( l1_tskisr_blk->client_id );

          if ( suspension_state_exit_to_abort[gas_id] )
          {
            /* go back to L1_MULTI_SIM_ACQ_BCCH_LIST_ABORT state and wait for further commands */
            l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_ABORT;

            MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_ACQ_BCCH_LIST suspended move to L1_MULTI_SIM_ACQ_BCCH_LIST_ABORT");
          }
          else
          {
            /* go back to L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED state and re-request the lock */
            l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED;

            MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_ACQ_BCCH_LIST suspended move to L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED");
          }
          l_abort_find_bcch = TRUE;

          /* Don't allow call to L1_get_bcch_list_fcch_sch_bcch() this may call frame_tick_notification causing crash */
          break;
        }
        else
        {
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

          l_abort_find_bcch = FALSE;

           MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_ACQ_BCCH_LIST suspension timer running");

           if( l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND )
           {
#if 0
              /* If we are stuck aborting then force an exit */
         if( gl1_msg_is_ft_hdlr_table_empty(gas_id) )
         {
               if ( suspension_state_exit_to_abort[gas_id] )
               {
                 /* go back to L1_MULTI_SIM_ACQ_BCCH_LIST_ABORT state and wait for further commands */
                 l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_ABORT;

                 MSG_GERAN_HIGH_0_G("Multi-SIM ACQ_BCCH_LIST suspended move to L1_MULTI_SIM_ACQ_BCCH_LIST_ABORT");
               }
               else
               {
                 /* go back to L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED state and re-request the lock */
                 l1_tskisr_blk->multi_sim_acq_bcch_list_state = L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED;

                 MSG_GERAN_HIGH_0_G("Multi-SIM ACQ_BCCH_LIST suspended move to L1_MULTI_SIM_ACQ_BCCH_LIST_STOPPED");

                 l_abort_find_bcch       = TRUE;
               }
         }
#endif
         }
       }
       (void)L1_get_bcch_list_fcch_sch_bcch(gas_id);
      }
      break;

    case L1_MULTI_SIM_ACQ_BCCH_LIST_ABORT:
   /* Stay here whilst everthing is finished to prevent any inproper calls to TRM */
       break;

    default:
      MSG_GERAN_HIGH_1_G("Unexpected l1 multi_sim_acq_bcch_list_state %d", l1_tskisr_blk->multi_sim_acq_bcch_list_state);
      break;
   } /* of switch */

   gl1_drx_require_next_tick(gas_id);

   return(l_abort_find_bcch);
}

/*===========================================================================

FUNCTION  L1_multi_sim_bcch_reserve_at

DESCRIPTION
          Does the TRM reservation for BCCH with TRM_CHANNEL_MAINTENANCE reason

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_multi_sim_bcch_reserve_at( uint32 required_frame_nbr, gas_id_t gas_id )
{
  grm_reserve_at_data_t grm_reserve_at_data;

  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* calculate the required parameters and pass in to TRM in the reservation request */
  grm_reserve_at_data.frames_required   = required_frame_nbr;
  grm_reserve_at_data.frame_duration    = L1_SC_DURATION_BCCH;
  if ( gpl1_sys_info_trm_priority_inverted (gas_id) )
  {
    grm_reserve_at_data.reason          = TRM_CHANNEL_MAINTENANCE_INV;
  }
  else
  {
    grm_reserve_at_data.reason          = TRM_CHANNEL_MAINTENANCE;
  }
#if defined (FEATURE_QTA_IN_FINDBCCH)
  grm_reserve_at_data.pri               = FM_PRI_NCELL_BCCH;
#else
  grm_reserve_at_data.pri               = FM_PRI_MAX;
#endif
  grm_reserve_at_data.sub_reason        = SYS_PROC_TYPE_ACQUISITION;
  grm_reserve_at_data.client_id         = l1_tskisr_blk->client_id;
  grm_reserve_at_data.gas_id            = gas_id;
  grm_reserve_at_data.resource          = gl1_ms_get_trm_resource( gas_id, grm_reserve_at_data.reason );
  grm_reserve_at_data.duration          = gpl1_multi_sim_convert_frames_to_sclcks( L1_SC_DURATION_BCCH );

  /*Add bands to freq_info*/
  grm_clear_freq_info(l1_tskisr_blk->client_id, gas_id);
  grm_add_trm_band_to_freq_input_type(gl1_map_gsm_band_to_sys_band(l1_tskisr_blk->next_params.ARFCN.band),
                                      l1_tskisr_blk->client_id,
                                      gas_id); 

  gpl1_gbta_reset_active_state(gas_id);
  call_in_task_grm_reserve_at( grm_reserve_at_data );
  gl1_store_grm_sub_reason(grm_reserve_at_data.sub_reason,gas_id);
}
/*===========================================================================

FUNCTION  L1_multi_sim_bcch_get_reservation_band

DESCRIPTION
  Returns the band that was registered with TRM for the reserve-at (or RANE) call.

DEPENDENCIES
  Only works if a search is currently active
   (before next_params are transfered to current_params)

RETURN VALUE
  sys_band_T

SIDE EFFECTS
  None

===========================================================================*/
sys_band_T L1_multi_sim_bcch_get_reservation_band(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  return l1_tskisr_blk->next_params.ARFCN.band;
}


/*===========================================================================

FUNCTION  L1_multi_sim_bcch_reserve_priority_change

DESCRIPTION
          Does the TRM reservation for BCCH with TRM_RESELECT reason.when chnage priority indication is recieved for Demode page.

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
void L1_multi_sim_bcch_reserve_at_priority_change(gas_id_t gas_id )
{
	l1_reselection_data_t	  *l1_reselection_data_p = &l1_reselection_data[gas_id];

	if (l1_reselection_data_p->bcch_req_frame != GL1_DEFS_INVALID_FN )
	{
		L1_multi_sim_bcch_reserve_at( l1_reselection_data_p->bcch_req_frame, gas_id );
	}

}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
