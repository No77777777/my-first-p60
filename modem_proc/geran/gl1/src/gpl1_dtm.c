/*=========================================================================
L 1   D U A L  T R A N S F E R  M O D E  I S R  S C H E D U L E R



GENERAL DESCRIPTION
   This module contains the procedures to handle the ISR level processing
   for NPL1 message layer of DUAL TANSFER MODE

EXTERNALIZED FUNCTIONS
 gpl1_dtm_control_cs_ps()

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2005-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dtm.c#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
26/08/16   km       CR1051091: In case of DTM, do not increment the sched_msg when there is no UL data to be txed or                       when coding_scheme(cs) is invalid
18/05/16   sn       CR1017349  Disable first TX schedule only if UL config is chnaged.
18/11/15   jk       CR940372:Remove statements related to VAMOS 2 code which are outside switch case
11/08/15   dg       CR885170 Don't leave DTM/EDTM mode if G2X IRAT is ongoing
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
21/04/15   shr      CR806878 GPRS DSDS Power Control and WWCoex Power Info do not log the Tx Power in use
01/04/15   zc       CR802323: Reset USF Detect Flag if FT handler hasn't been added in EXT_UTBF_NO_DATA
12/02/15   ab       CR773792: Perform header decodes only during DL tbf release, reduces TX Blanking
07/11/14   pjr      CR751950 Ensure wtr hopping is inactive when state changes from transfer to idle.
20/06/14   pjr      DATA+MMS feature - USF utilisation calculation
30/05/14   ws       CR657674 Data + MMS Phase 1
29/04/14   pjr      CR641721 Enable connected mode WTR hopping during DTM
08/05/14   br       CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
21/11/13   cja      CR455527 Mainline or remove features to tidy up code.
24/09/13   jk       CR549227:Shifted SACCH changes for VAMOS2
10/09/13   nm       CR488624:Clear UL PACCH buffers when aborting DTM mode.
21/06/13   ws       CR498325 - Fix gas ID with grlc_service_block_timers() and
                    grlc_dl_set_oper_win_size()
15/08/13   sk       CR524039 GBTA bringup changes
26/07/13   sai      CR514338: Handle race condition between CS+PS abort complete at
                    MPH_DTM_CHANNEL_RELEASE_REQ, and UL/DL PDTCH block start.
06/03/12   pg       CR457891: Trigger Tx band change earlier during transfer init and reconfig
14/12/12   ab       CR432106 -L1 to delete Control messages queue when going from DTM to Dedicated
18/05/12   ab       CR#: 352933 L1 - MAC is out of sync after reception of Down ass
                    during DL TBF release.
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
24/11/11   ab       CR321179 - During DTM/EDTM in with EGPRS DL - L1 does not disable IR.
11/01/11   cgc      Fix compiler warning add L1_DTM_NONE  #1553
02/12/09   ws       Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                    Improvements
11/03/09   og       CR173804 - Ignore downlink mac mode for Rel 6.
12/02/09   ap       Fix lint error
12/02/09   ap       CR169099 When leaving DTM to Dedicated. Correctly set the
                    AFC/TT mode to Dedicated.
28/01/09   og       Do not apply the Enhanced Ext TBF criterea in single block
                    TBF. Resolves CR170320.
24/11/08   og       FEATURE_GSM_GPRS_MSC33 GPL1 changes.
17/03/08   og       Fix the support of extended uplink TBF in DTM.
                    Resolves CR141215.
17/01/08   cs       Lint fixes
24/09/07   og       Handle the error fatal case where EGPRS DL TBF is released
                    but queued nodes still exist.
16/03/07   og       Abort DL PTCCH when entering EDTM_INIT state.
06/02/07   cs       Fix compiler feature for non-EDTM build
26/01/07   og       Implement UL ctrl msg deletion in L1.
23/01/07   og       Resolve RVCT2.1 compiler warnings, merge from the
                    gpl1_hummer.10.00.43.01 branch.
28/11/06   dv       Lint cleanup
14/11/06   cs       CR102226 Fix the Uplink Power Logging to use unslammed slot
10/10/06   og       Set the correct clock setting in EDTM INIT depending on the
                    timeslot slam mode.
17/08/06   og       Basic featured-off EDTM code drop.
14/08/06   ws       Allow dedicated to be timeslot slammed to lowest received tn
                    and also to place the CS timeslot on TN 2
31/07/06   av       CR85810 NB AB TSC, and TA fix.
10/07/06   og       Use DTM multislot parameters in DTM mode. Resolves
                    CR94827.
19/07/06   nt       CR93487 - L1 ignoring release from MAC. Have stopped the pending_alloc from
                    getting set to NULL_ALLOC when in reconfiguring and sending a RRBP
                    which has FAI set.
29/03/06   og       Take wrap around cases into account when add/sub camp timeslot
                    to/from absolute timeslots, resolves CR 90960.
17/03/06   og       Take downlink assignment mac mode into account. This is
                    required by release 4 ETSI spec.
16/03/06   og       Re-introduce the use of the buffering of camp timeslot by NPL1.
06/02/06   ws       Added suppport for Ext. Dynamic allocation in DTM for MSC 11
26/01/06   og       Neighbour cell acquisition changes to handle extended dynamic.
15/11/05   og       Fix to USF granularity handling, resolves CR 82339, 82341.
03/11/05   nt       changed values in the array IS_PDCH_CHAN_ABORTED so that the index
                    cannot equal the return to prevent aborts in one thread.
02/11/05   nt       new function to reset static flags/data gpl1_dtm_data_init()
19/09/05   nt       Added code to L1_TRANSFER_RECONFIG state in DTM, to reconfig.
15/09/05   ws       Added dummy function to avoid warning when FEATURE_GSM_DTM
                    is not defined
06/09/05   og       Correctly handle Tra/Trb clashes.
05/09/05   og       Multislot class 12 support.
01/09/05   ws       Linted and code cleanup for FEATURE_GSM_DTM
01/09/05   ws       Moved disabling EGPRS until 4th frame of radio block
26/08/05   og       Removal of DEBUG_TRANSFER compiler switch and associated
                    code.
22/08/05   og       Add the setting of mac mode to NPL1.
23/09/05   ws       Corrected ftn functionality so that PS sets up correct
                    block_start flags
19/08/05   og       Removal of downlink block FIFO.
12/08/05   ws       Updated for initial DTM release
08/08/05   og       DTM code changes.
05/07/05   ws       Initial Revision.

===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_DTM

#ifdef  FEATURE_GSM_EDTM

#include "gl1_msg_g.h"

#endif

#include "gl1_msg_pdch.h"

#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_utils.h" /* prototype for gl1_get_FN() */
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_ptcch.h"
#include "grlcl1.h" /* l1 <-> rlc api */
#include "geran_eng_mode_info.h"
#include "gpl1_dual_sim.h"
#include "gl1_msg_wtr_hop.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* Provides client with the FN mod13 when it is safe to schedule
 * potentially conflicting activities. i.e. when the PDCH chanels
 * have been aborted.
 *
 * In: FN mod 13 Return: mod 13 FN
 */
extern dedicated_data_T l1_dedicated_data[];

/*Make sure that the frame number passed in can never be that returned
therefore

IS_PDCH_CHAN_ABORTED[3] = 3 should be
IS_PDCH_CHAN_ABORTED[3] = 4

IS_PDCH_CHAN_ABORTED[7] = 7 should be
IS_PDCH_CHAN_ABORTED[7] = 8,

IS_PDCH_CHAN_ABORTED[12] = 12 should be
IS_PDCH_CHAN_ABORTED[12] = 0

This reason is so that the abort can never happen on the first call to this check,
it will always be at least on the next frame preventing the call to abort the PDTCH
and the call to  gl1_msg_set_egprs_mode(FALSE); in the same thread.
*/
static const uint8 IS_PDCH_CHAN_ABORTED[13] =
{
  /* current FNmod13 : 0, 1, 2, 3 */
                       3, 3, 3, 4,
  /* current FNmod13 : 4, 5, 6, 7 */
                       7, 7, 7, 8,
  /* current FNmod13 : 8, 9, 10, 11,12 */
                      12, 12, 12, 12, 0
};

  /* Setup Npl1s rx\tx structures containing function addresses */
static const  gl1_defs_rx_pkt_cb_type  dynamic_rx_cb =
{
    gpl1_gprs_rx_dyn_data_cb,
    gpl1_gprs_rx_dyn_metrics_cb,
    (gl1_defs_rx_pkt_discarded_callback)  NULL,
    (gl1_defs_rx_pkt_log_callback)        NULL
};

static const  gl1_msg_tx_pkt_cb_type   dynamic_tx_cb =
{
    (gl1_msg_scheduled_data_callback)  gpl1_gprs_tx_dyn_rrbp_sched_cb,
    (gl1_msg_discarded_data_callback)  gpl1_gprs_tx_dyn_rrbp_disc_cb,
    (gl1_msg_usf_utilisation_callback) gpl1_gprs_tx_dyn_rrbp_usf_util_cb

};


static boolean cs_idle[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
static boolean ps_idle[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

/* Used to cancel RX/TX for two block period boundaries */
static uint8  tra_trb_cancel_ts_alloc[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };

static uint32 abort_fnmod13[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0xffffffff) };

/* Controls the release process allowing the setup of variable for only
the first pass through the releasing loop */
static boolean abort_setup_once[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(TRUE) };

#if defined ( FEATURE_GSM_EDTM )
/* Instantiate structure to invalid */
#define _MAC_QUEUE_INIT { FALSE, { { 0, 0, 0, 0 }, 0, 0} }
static q_mac_l1_delete_ul_ctrl_msg_t gpl1_queued_mac_l1_delete_ul_ctrl_msg[NUM_GERAN_DATA_SPACES] =
    { INITIAL_VALUE(_MAC_QUEUE_INIT)};

#undef _MAC_QUEUE_INIT
/* This function queues the mac l1 delete ul ctrl msg */
void  gpl1_queue_mac_l1_delete_ul_ctrl_msg( uint8  message_type, gas_id_t gas_id)
{
  /* Copy in the message type */
  gpl1_queued_mac_l1_delete_ul_ctrl_msg[gas_id].msg.message_type = message_type;

  /* Set gas_id */
  gpl1_queued_mac_l1_delete_ul_ctrl_msg[gas_id].msg.gas_id = gas_id;

  /* Set the valid flag to TRUE */
  gpl1_queued_mac_l1_delete_ul_ctrl_msg[gas_id].valid = TRUE;

#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
}

/* This function removes the mac l1 delete ul ctrl msg */
void  gpl1_remove_mac_l1_delete_ul_ctrl_msg( gas_id_t gas_id )
{
  /* Reset the validity boolean */
  gpl1_queued_mac_l1_delete_ul_ctrl_msg[gas_id].valid = FALSE;

#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
}

const q_mac_l1_delete_ul_ctrl_msg_t*  gpl1_read_mac_l1_delete_ul_ctrl_msg( gas_id_t gas_id )
{
  return &( gpl1_queued_mac_l1_delete_ul_ctrl_msg[gas_id] );
}
#endif /* FEATURE_GSM_EDTM */

/*===========================================================================

FUNCTION  gpl1_dtm_send_ftn

DESCRIPTION
  This function configures a set of counter's and flags for formatting
  the parameters for sending the function gl1_frame_tick_notification() to NPL1.



DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_dtm_send_ftn(
                         uint8 *pacch_pdtch_ul_blk_start_cnt,
                         uint8 *pacch_pdtch_dl_blk_start_cnt, gas_id_t gas_id)

{
  volatile ISRTIM_CMD_BLK            *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /*-------------------------------------------------------------------------------------
     Initialise ftn structure with block start's setup
     in L1_DEDICATED_MODE
    -----------------------------------------------------------------------------------*/
  gl1_msg_frame_tick_type  ftn = l1_ded_get_ftn_info(gas_id);

  transfer_data_T *transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;



  if (*pacch_pdtch_ul_blk_start_cnt == 0)
  {
    ftn.next_frame_is_ul_pdch_block_start = TRUE;

    /* set counter to invalid as block start has elapsed */
    *pacch_pdtch_ul_blk_start_cnt = GPL1_INVALID;
  }
  else
  {
    ftn.next_frame_is_ul_pdch_block_start = FALSE;
  }

  if (*pacch_pdtch_dl_blk_start_cnt == 0)
  {
    ftn.next_frame_is_dl_pdch_block_start = TRUE;

    /* set counter to invalid as block start has elapsed */
    *pacch_pdtch_dl_blk_start_cnt = GPL1_INVALID;
  }
  else
  {
    ftn.next_frame_is_dl_pdch_block_start = FALSE;
  }


  /* Clear block starts */
  gl1_set_pwr_meas_block_start(FALSE,gas_id);
  gl1_set_ncell_block_start(FALSE,gas_id);

  /*  Perform serving cell measurements */
  /* If ftn.next_frame_is_acq_ncell_block_start has been set to true, then this
      frame is an idle slot and may or may not be being used. So if this flag set
      to false over write with outcome of gpl1_gprs_serving_cell_meas. If set
      to true only overwrite if frame available for interference measurements.  */

  /*  Still need to run function, but do not set flag  */
  (void)gpl1_gprs_serving_cell_meas(0,gas_id);

  gl1_msg_frame_tick_notification(&ftn,gas_id);

  /*
   * Decrement all block start counters
   */

  if (*pacch_pdtch_ul_blk_start_cnt != GPL1_INVALID)
    (*pacch_pdtch_ul_blk_start_cnt)--;

  if (*pacch_pdtch_dl_blk_start_cnt != GPL1_INVALID)
    (*pacch_pdtch_dl_blk_start_cnt)--;


  gpl1_gprs_handle_tbf_complete(transfer_data_ptr, gas_id);

}   /* End of function */



/*===========================================================================

FUNCTION  GPL1_DTM_CONTROL_CS_PS()

DESCRIPTION
 Schedules NPL1 for Dual Transfer Mode
DEPENDENCIES


RETURN VALUE
  idle - TRUE Drivers have aborted
         FALSE Drivers not aborted

SIDE EFFECTS
  None

===========================================================================*/
l1_dtm_mode_T gpl1_dtm_control_cs_ps(dtm_data_T   *dtm_data, boolean apply_freq_redef, gas_id_t gas_id)
{

  idle_data_T                  *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK      *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  dedicated_data_T    *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  gl1_defs_coding_type  txed_coding_scheme[GL1_DEFS_MAX_ASSIGNED_UL_TS] = {0};

  l1_dtm_mode_T idle =     L1_DTM_NONE;

  transfer_data_T          *transfer_data_ptr =
                                     l1_tskisr_blk->current_params.L1Data.pTransfer_data;


  channel_information_T    *freq_info_ptr =
                                   &l1_dedicated_data_ptr->channel_specification.channel_info_1_after;
  uint32 FN =              gl1_get_FN(gas_id);
  uint8    i;
  const gpl1_ps_store_T    *gpl1_store_ptr = NULL;

  uint8    pacch_pdtch_ul_blk_start_cnt = GPL1_INVALID;
  uint8    pacch_pdtch_dl_blk_start_cnt = GPL1_INVALID;

  static downlink_timeslot_info_t  downlink_ts_info[NUM_GERAN_DATA_SPACES] =
                            { INITIAL_VALUE_S(GPL1_INVALID_TN) };


  /* Reconfigure PS GSPRS/EGPRS TBF's */

  if(  ( l1_tskisr_blk->sub_state == L1_TRANSFER_RECONFIG )

#ifdef  FEATURE_GSM_EDTM

/* @@OG: EDTM design */
     ||( l1_tskisr_blk->sub_state == L1_EDTM_INIT )

#endif

    )
  {
   /* Setup the usf allocation array */
    gl1_defs_rx_pkt_alloc_type  alloc_type[ GL1_DEFS_MAX_ASSIGNED_DL_TS ]=
    {
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 }
    };

    uint8                     dl_timeslot_alloc = 0;

#ifdef  FEATURE_GSM_EDTM

    if( L1_EDTM_INIT == l1_tskisr_blk->sub_state )
    {
      MSG_GERAN_HIGH_0_G("Perform gpl1_dtm_control_cs_ps L1_EDTM_INIT");
    }
    else
    {
#endif

      MSG_GERAN_HIGH_0_G("Perform gpl1_dtm_control_cs_ps L1_TRANSFER_RECONFIG");

#ifdef  FEATURE_GSM_EDTM
    }
#endif


    /* Setup egprs incremental redundancy.
     * This function updates the rlc mode.
     */
    gpl1_reconfig_egprs_incr_redundancy( transfer_data_ptr, FN,gas_id );

    /* Set the loopback test mode to OFF */
    transfer_data_ptr->l1_test_mode = L1_TEST_MODE_OFF;

    /* configure NPL1s tx handler */
    gl1_msg_cfg_ul_pacch_pdtch( freq_info_ptr->TSC,
                                TS0, /* Access Burst TSC */
                                l1_idle_data->campedon_cell_BSIC,
                                &dynamic_tx_cb,gas_id );

    if( NULL != transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr )
    {
      dl_timeslot_alloc = transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
    }

    /* Avoid de-referencing a NULL pointer */
    if( NULL != transfer_data_ptr->dynamic_tbf_ptr )
    {

      /* Take a copy of mac_mode */
      uint8 mac_mode = transfer_data_ptr->dynamic_tbf_ptr->mac_mode;

      /* If we are connecting a downlink assignment then its mac_mode
       * must be ignored (Rel 6).
       */

      /* Check the supplied mac mode */
      switch( mac_mode )
      {
      case  MAC_L1_MAC_MODE_DYNAMIC:

        /* Set the mac mode into l1 transfer data */
        l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_DYNAMIC;

        /* Inform NPL1 */
        gl1_msg_set_medium_access_mode( GL1_DEFS_DYNAMIC_ALLOC,gas_id );

        MSG_GERAN_MED_0_G(" mac mode DYNAMIC ");
        break;

      case  MAC_L1_MAC_MODE_EXTENDED_DYNAMIC:

        /* Set the mac mode into l1 transfer data */
        l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_EXTENDED_DYNAMIC;

        /* Inform NPL1 */
        gl1_msg_set_medium_access_mode( GL1_DEFS_EXT_DYNAMIC_ALLOC,gas_id );

        MSG_GERAN_MED_0_G(" mac mode EXT DYNAMIC ");
        break;

      default:
        /* The mac mode is invalid/unavailable, don't
         * do anything.
         */

        break;
      }
    }  /* End of if dynamic tbf ptr is not NULL */
    else
    {
      MSG_GERAN_ERROR_0_G(" Attempt to dereference NULL dynamic tbf ptr ");
    }

#ifdef  FEATURE_GSM_EDTM

    if( L1_EDTM_INIT == l1_tskisr_blk->sub_state )
    {
      /* Configure Message layer for EGPRS or GPRS mode */
      if( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
      {

        if(l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST)
        {
          gl1_msg_set_egprs_mode( TRUE,  /* egprs mode */
                                  TRUE,  /* register clock */
                                  gas_id
                                );

          l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_TN2;
        }else
        {
          /* been into DTM before so we are already running at 122Mhz */
          gl1_msg_set_egprs_mode( TRUE,  /* egprs mode */
                                  FALSE,  /* register clock */
                                  gas_id
                                );
        }

      }
      else
      {
        if(l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST)
        {
          l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_TN2;
        }
      }
    }

#endif

    /* Work out the timeslot information resulting
     * from the received assignment message.
     */

    gpl1_calc_dyn_dl_ts_info( dl_timeslot_alloc,                  /* In  */
                              &( transfer_data_ptr->dynamic_tbf_ptr->usf_value[0] ), /* In  */
                              &( downlink_ts_info[gas_id] ),              /* Out */
                              &( alloc_type[0] ), gas_id );               /* Out */

    /* Configure NPL1s rx handler */
    gl1_msg_cfg_dl_pacch_pdtch( &( alloc_type[ 0 ] ),
                                downlink_ts_info[gas_id].num_dl_pdch,
                                freq_info_ptr->TSC,
                                transfer_data_ptr->dynamic_tbf_ptr->usf_granularity,
                                &dynamic_rx_cb,
                                gas_id);

#ifdef PL1_FW_SIM
    /* Configure uplink and/or downlink tbf */
    MSG_GERAN_MED_0_G("PL1: DYNAMIC reconfig ");
#endif

    /* Update the FN variable after the gpl1_gprs_change_timeslot( ) is called
     * incase lowest DL timeslot decreased and caused the frame counters
     * of the MS to be incremented by one. This ensures FN is the
     * most up-to-date value.
     */
    FN = gl1_get_FN(gas_id);

    gpl1_gprs_config_ta_param( FALSE,
                               freq_info_ptr->TSC,
                               downlink_ts_info[gas_id].lowest_dl_timeslot, gas_id );

#ifdef  FEATURE_GSM_EDTM
    if( L1_EDTM_INIT == l1_tskisr_blk->sub_state )
    {
      /* We want to the update the ptcch information to the relevant
       * modules but also we need to switch off the msg layer dl handler.
       * There is not i/f by msg layer currently to abort the ul ptcch handler.
       */
      gl1_msg_abort_dl_ptcch(gas_id );
    }
#endif

    gpl1_store_ptr = gpl1_read_xfer_isr_store(gas_id);

    gl1_msg_set_afc_tt_mode(gpl1_store_ptr->gpl1_afc_tt_mode, gas_id);



    /* Configure AGC for dl power control ( if present) */
    gpl1_gprs_config_agc(gas_id);

    /* Set AGC valid on first RX block */
    gpl1_gprs_update_first_rx(TRUE, gas_id);

    /* initiate RF Tx band change - only gets actioned if band has actually changed */
    gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]), gas_id);

    /* disable the first tx from being scheduled only if there is change in UL config*/
    if( (!transfer_data_ptr->starting_time_elapsed) &&
     ( (transfer_data_ptr->tbf_est_type == UPLINK_TBF_ESTABLISHED) ||
       (transfer_data_ptr->tbf_est_type == UPLINK_DOWNLINK_TBF_ESTABLISHED)
     ) )
    {
     GPL1_DISABLE_TX( gas_id);
    }


    /* @@@@@@@@@ Maybe wait for DTM release 0.2 to code this @@@@@@@@@@ */
    /* finished reconfiguring TBF, change ss and schedule */
    l1_tskisr_blk->sub_state = L1_DTM_RUNNING;
#ifdef FEATURE_GPRS_GBTA
    SET_GBTA_STATUS(GRM_GBTA_NONE);
    /* For this SUB we are not interested in IDLE state vars */
    grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);
#endif  /*FEATURE_GPRS_GBTA */
  } /* End of if( ss = TRANSFER_RECONFIG || L1_EDTM_INIT */


  MSG_GERAN_HIGH_2_G("gpl1_dtm_control_cs_ps: GFN:%d, l1_tskisr_blk->sub_state:%d",
                     gl1_get_FN(gas_id),l1_tskisr_blk->sub_state);

  switch (l1_tskisr_blk->sub_state)
  {

    case L1_DTM_INIT:
    {

      /* Do we need to setup PS channels ? */
      if ( (dtm_data->l1_dtm_init_mode == L1_DTM_CS_PS) ||
           (dtm_data->l1_dtm_init_mode == L1_DTM_PS_ONLY) )
      {
                        /* Setup the usf allocation array */
        gl1_defs_rx_pkt_alloc_type  alloc_type[ GL1_DEFS_MAX_ASSIGNED_DL_TS ]=
        {
          { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
          { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
          { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
          { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 }
        };

        uint8    dl_timeslot_alloc  = 0;
        uint8    tx_timeslot = GL1_DEFS_SLOTS_IN_FRAME;
        boolean  *tx_ts_ptr   = &( gprs_serv_cell_meas_ptr->tx_in_progress
                                  [ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );

        /* Initialise the ta_meas_in_progress, tx_in_progress
         * which is used by the serving cell measurement
         * module to avoid scheduling interference
         * measurements on timeslots that clash with
         * the ptcch timeslot, rrbp, usf ul.
         */
        gprs_serv_cell_meas_ptr->ta_meas_in_prog = NO_PTCCH;

        while( tx_timeslot-- > 0 )
        {
          *( tx_ts_ptr-- ) = FALSE;
        }
        tx_ts_ptr++;

        GPL1_RESET_USF_DETECT( gas_id );

        /* Avoid de-referencing a NULL pointer */
        if( NULL != transfer_data_ptr->dynamic_tbf_ptr )
        {
          /* Take a copy of mac_mode */
          uint8 mac_mode = transfer_data_ptr->dynamic_tbf_ptr->mac_mode;

          /* If we are connecting a downlink assignment then its mac_mode
           * must be ignored (Rel 6).
           */

          /* Initialise mac mode to invalid */
          l1_transfer_data[gas_id].mac_mode = L1_MAC_MODE_INVALID;

          /* Check the supplied mac mode */
          switch( mac_mode )
          {
            case  MAC_L1_MAC_MODE_DYNAMIC:

              /* Set the mac mode into l1 transfer data */
              l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_DYNAMIC;

              /* Inform NPL1 */
              gl1_msg_set_medium_access_mode( GL1_DEFS_DYNAMIC_ALLOC,gas_id );

              MSG_GERAN_MED_0_G(" mac mode DYNAMIC ");

              break;

            case  MAC_L1_MAC_MODE_EXTENDED_DYNAMIC:

              /* Set the mac mode into l1 transfer data */
              l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_EXTENDED_DYNAMIC;

              /* Inform NPL1 */
              gl1_msg_set_medium_access_mode( GL1_DEFS_EXT_DYNAMIC_ALLOC,gas_id );

              MSG_GERAN_MED_0_G(" mac mode Ext DYNAMIC ");

              break;

            default:
              /* The mac mode is invalid/unavailable, don't
               * do anything.
               */
            break;
          } /* End of switch on mac mode */

        }  /* End of if dynamic tbf ptr is not NULL */
        else
        {
            MSG_GERAN_ERROR_0_G(" Attempt to dereference NULL dynamic tbf ptr ");
        }


        /* Configure Message layer for EGPRS or GPRS mode */
        if( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
        {

          if(l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST)
          {
            gl1_msg_set_egprs_mode( TRUE,TRUE,gas_id );

            l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_TN2;

          }else
          {
            /* been into DTM before so we are already running at 122Mhz */
            gl1_msg_set_egprs_mode( TRUE,FALSE,gas_id);
          }

        }
        else
        {
          if(l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST)
          {
            l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_TN2;
          }
        }

        /* Setup egprs incremental redundancy.
         * This function sets the rlc mode.
         */
        gpl1_init_egprs_incr_redundancy( transfer_data_ptr, FN,gas_id );

        /* Set the loopback test mode to OFF */
        transfer_data_ptr->l1_test_mode = L1_TEST_MODE_OFF;

        MSG_GERAN_MED_1_G(" Test mode OFF FNmod13 %u",frame_counters[gas_id].FNmod13);

        /* configure NPL1s tx handler */
        gl1_msg_cfg_ul_pacch_pdtch( freq_info_ptr->TSC,
                                    TS0, /* Access Burst TSC */
                                    l1_idle_data->campedon_cell_BSIC,
                                    &dynamic_tx_cb,gas_id );

        if( NULL != transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr )
        {
            dl_timeslot_alloc =
                           transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
        }

        /* Work out the timeslot information resulting
         * from the received assignment message.
         */
        gpl1_calc_dyn_dl_ts_info
                      (
                        dl_timeslot_alloc,                                     /* In  */
                        &( transfer_data_ptr->dynamic_tbf_ptr->usf_value[0] ), /* In  */
                        &( downlink_ts_info[gas_id] ),                                 /* Out */
                        &( alloc_type[0] ) , gas_id                                    /* Out */
                      );

        /* Configure NPL1s rx handler */
        gl1_msg_cfg_dl_pacch_pdtch( &( alloc_type[ 0 ] ),
                                    downlink_ts_info[gas_id].num_dl_pdch,
                                    freq_info_ptr->TSC,
                                    transfer_data_ptr->dynamic_tbf_ptr->usf_granularity,
                                    &dynamic_rx_cb,
                                    gas_id);

        /* configure message layer which AFC/TT mode to use PDCH or DCH */

        /* @@@@@@@ Need to work if lowest TN is PS or CS @@@@@@@@@@ */
        /* Fetch current value of num_rx_tn */
        gpl1_store_ptr = gpl1_read_xfer_isr_store(gas_id);

        gl1_msg_set_afc_tt_mode(gpl1_store_ptr->gpl1_afc_tt_mode, gas_id);

        /* Initialise AFC/TT for Packet transfer based on the frequency band */
        gl1_msg_pdch_afc_tt_init(freq_info_ptr->frequency_list.channel[0],gas_id);

        /* Configure PS AGC for dl power control ( if present) */
        gpl1_gprs_config_agc(gas_id);

        /* Set AGC valid on first RX block */
        gpl1_gprs_update_first_rx(TRUE, gas_id);
        /* initiate RF Tx band change - only gets actioned if band has actually changed */
        gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]), gas_id);

        /***
         *** General Initialisation
         ***/

        /* setup metrics for downlink */
        gpl1_gprs_init_metrics_cb_store( gas_id );

        /*Reset rrbp tx counter */
        gpl1_gprs_reset_num_rrbp_msg_txd(gas_id );

        l1_transfer_data[gas_id].confirm_pending = SENT;

        /* Initialise abort callbacks */
        GPL1_RESET_ABORT_CB( gas_id );

        l1_transfer_data[gas_id].abort_receives = FALSE;

        /* Initialize count of scheduled Tx messages
         * Used to control scheduling of PACCH_PDTCH
         * messages to NPL1
         */
        transfer_data_ptr->dynamic_tbf_ptr->sched_msgs = 0;

        /* disable the first tx from being scheduled */
        GPL1_DISABLE_TX( gas_id);

        /* used to indicate that the pca has been transmitted
         * and therefore the tbf can be released
         */
        transfer_data_ptr->pca_sent = FALSE;

        /* Initialsise first_tx_block. Used to send first rlc_data_block_ind
         * to mac */
        transfer_data_ptr->first_tx_block=TRUE;

        /* initialise downlink buffer pointers to begining of fifo */
        gpl1_gprs_init_dl_fifo(gas_id);

        /* initialise buffer for deferring blocked RLC UL FIFO activities */
        GPL1_RESET_DEFER_BUFF( gas_id );

        /* preset wait for pca vars */
        transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
        transfer_data_ptr->wait_for_pca_valid = FALSE;


        /* Set the usf granularity information */
        gpl1_gprs_set_usf_gran_info( 0,      /* granularity block count */
                                     TRUE,
                                     gas_id ); /* start granularity block count */

#ifdef FEATURE_GPRS_GBTA
        SET_GBTA_STATUS(GRM_GBTA_NONE);
        /* For this SUB we are not interested in IDLE state vars */
        grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);
#endif  /*FEATURE_GPRS_GBTA */

      }
      else if (dtm_data->l1_dtm_init_mode == L1_DTM_CS_ONLY)
      {
        /* @@@@@@ Case to handle Channel Mode modify  @@@@@@ */

        /* Only CS channels are aborted PS continues to run */
      }
      else
      {
        MSG_GERAN_ERROR_1_G("dtm_init_mode is bad %d",dtm_data->l1_dtm_init_mode);
      }

      /* finished initialising, change ss and schedule */
      l1_tskisr_blk->sub_state = L1_DTM_RUNNING;

      /* no break, fall through */

    } /* End of case L1_DTM_INIT */

    /*lint -fallthrough*/
    case L1_DTM_RUNNING:
    {

      /* Schedule CS activities as per L1_DEDICATED API */

#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif

      switch (l1_dedicated_data_ptr->channel_specification.channel_info_1_after.
                channel_type)
      {
        case FULL_RATE_TRAFFIC:
        {
          cs_idle[gas_id] = L1_control_full_rate(l1_dedicated_data_ptr,
                                         &(l1_dedicated_data_ptr->channel_specification.
                                         channel_info_1_after),
                                         apply_freq_redef, gas_id);

          break;
        }

        case HALF_RATE_TRAFFIC:
        {
          cs_idle[gas_id] = L1_control_half_rate(l1_dedicated_data_ptr,
                                         &(l1_dedicated_data_ptr->channel_specification.
                                         channel_info_1_after),
                                         apply_freq_redef, gas_id );


          break;
        }
        default:
          {
            MSG_GERAN_ERROR_2_G("Wrong channel type %d",
                       l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type,
                       0);
             break;
          }
      }


      switch( frame_counters[gas_id].FNmod13 )
      {
        case 0:
        {
          if( 0 == frame_counters[gas_id].FNmod104 )
          {
              /* adjust AFC/TT every 104 frames */
              gl1_msg_pdch_tt_adjust(gas_id );
          }

          /* Setup new window info if we have a DL EGPRS TBF for RLC ACK mode */
          if ((transfer_data_ptr->dl_tbf_ptr) &&
             (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS) &&
             (transfer_data_ptr->rlc_mode == RLC_ACK) )
          {
            e_grlc_dl_window_info_t win_info;

            /* IR is enabled for the EGPRS DL TBF so setup WS */
            if(e_grlc_dl_get_window_info(gas_id, &win_info))
            {
              gl1_msg_IR_set_active_window(win_info.start_bsn, win_info.end_bsn,gas_id);
              MSG_GERAN_HIGH_3_G("update dl win st_bsn %d end_bsn %d [%d]",win_info.start_bsn,
                                                                 win_info.end_bsn,
                                                                 gl1_get_FN( gas_id ));

            }else
            {
              /* Setup default params as RLC hasn't been confiured for DL TBF yet
               * No changes required for MSC30 - 34.
               */
              gl1_msg_IR_set_active_window(0,1024,gas_id);

              MSG_GERAN_HIGH_0_G("No win info from RLC");
            }

          } /* end of if ((dynamic_tbf_ptr->dl_tbf_ptr) && ... */

          /* No break follow through */

        }
        /*lint -fallthrough*/
        case 4:
        case 8:
        {
          if (transfer_data_ptr->rlc_svc_timer_active)
          {
            /* RLC has data so kick service block timer on frame 1
            * of each radio block
            */
#ifdef PL1_FW_SIM
            MSG_GERAN_LOW_0_G("PL1: sending RLC service block timer");
#endif
            grlc_service_block_timers(gas_id);

          }
          break;
        }


        case 3:   /* Schedule for Block 1 */
        case 7:   /* schedule for Block 2 */
        case 12:  /* schedule for Block 3 */
        {

#if defined ( FEATURE_GSM_EDTM )
          const q_mac_l1_delete_ul_ctrl_msg_t*  msg_ptr = gpl1_read_mac_l1_delete_ul_ctrl_msg( gas_id );

          if( NULL != msg_ptr )
          {
            /* Do we have a pending pacch fifo delete event */
            if( FALSE != msg_ptr->valid )
            {
              /* Search for the msg type in the PACCH FIFO and remove them
               * This is safe at this point because scheduled messages should
               * be zero and only pending messages are set.
               * Hence no interaction with lower layers to worry about at this
               * point.
               */
              uint8 csn_msg_type = msg_ptr->msg.message_type;

              MSG_GERAN_MED_2_G(" ul ctrl del FN %u type %u ", FN, csn_msg_type);

              /* Search through the pacch fifo and remove pending messages */
              gpl1_pacch_fifo_delete_msg( csn_msg_type, gas_id );

              /* Search through the RRBP queue and remove pending messages
               * Be careful of removing messages which the pending release is
               * awaiting transmission of.
               */
              gpl1_rrbp_delete_msg( csn_msg_type,gas_id );

              /* Reset queued message to invalid when completed */
              gpl1_remove_mac_l1_delete_ul_ctrl_msg( gas_id );
            } /* End of if queued delete msg is valid */
          }
          else
          {
            /* Print an error message */
            MSG_GERAN_ERROR_1_G(" NULL msg ptr FNmod13 %u ", frame_counters[gas_id].FNmod13);
          }
#endif

          /* Are we attempting to abort PS activities */

          if((dtm_data->l1_dtm_abort_mode == L1_DTM_CS_ONLY) ||
            (dtm_data->l1_dtm_abort_mode == L1_DTM_NONE))
          {

            /* Used to cancel RX/TX for this block boundary */
            uint8    l1_cancel_ts_alloc = 0x0;

            /* Used to task receives for downlink tfi timeslots */
            uint8  dl_tn_alloc    = 0;

            /* Initialise index into metrics/log/data/ buffer */
            uint8  rlc_mac_dl_info_index = 0;



            /***
            *** Schedule DL PACCH_PDTCH TN(s)
            ***/

            /***********************************************************************
            ***
            *** schedule Allocated DL TN's
            ***
            ***********************************************************************/

            L1_get_ARFCNS ( 1, 1, 4, l1_get_gprs_arfcns(gas_id),
                         &l1_dedicated_data_ptr->channel_specification.channel_info_1_before,
                         &l1_dedicated_data_ptr->channel_specification.channel_info_1_after,
                         &l1_dedicated_data_ptr->new_channel_info, gas_id );

            /* if an additional downlink allocation is configured
            * then assign timeslot allocation
            */
            if (transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr)
            {
              dl_tn_alloc =
                       transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
            }

            /*********************************************************
             ***
             *** Check if there is a  clash of RRBP and Rx Tn's
             *** (1) If RRBP is PDAN and it classhes, delete PDAN (04.60 sec 8.1.2.2)
             *** (2) If RRBP is PCA and it clashes, remove clashed Tn.
             ***
             *********************************************************/

            /* Fetch current value of num_rx_tn */
            gpl1_store_ptr = gpl1_read_xfer_isr_store(gas_id);

            if( 0x0 != tra_trb_cancel_ts_alloc[gas_id] )
            {
              /* This is the second block boundary after the flag
               * was set so reset to FALSE.
               */
              l1_cancel_ts_alloc |= tra_trb_cancel_ts_alloc[gas_id];

              /* preset clash_tn_found for detection of pdtch and RRBP */
              tra_trb_cancel_ts_alloc[gas_id] = 0x0;
            }

            gpl1_gprs_clean_rrbp_enteries( FN,
                                           gpl1_store_ptr->num_rx_tn,
                                           &( tra_trb_cancel_ts_alloc[gas_id] ),
                                           &( l1_cancel_ts_alloc ), gas_id );

            if( 0x0 != l1_cancel_ts_alloc )
            {
              GPL1_DISABLE_TX( gas_id);
            }

            /* Update the camp timeslot */
            gl1_msg_pdch_set_camp_tn( (gl1_defs_tn_type) l1_get_current_timeslot(gas_id),gas_id );

            for (i=0;i< MAX_TN;i++)
            {
              /* We schedule all dl timeslots every block therefore we should
               * have at least one tn either from the DL tbf and/or UL TBF
               */
              if( (  ( transfer_data_ptr->dynamic_tbf_ptr->usf_value[i] != GPL1_INVALID )
                   ||( 0 != ( dl_tn_alloc & 0x1 ) )
                  )
                &&( 0 == ( ( l1_cancel_ts_alloc >> i ) & 0x01 ) )
                )
              {
                /* Setup DL data buffer */
                gpl1_gprs_init_data_cb_store
                                        (
                                        rlc_mac_dl_info_index,
                                        transfer_data_ptr->rlc_mac_dl_buff.in_ptr,
                                        gas_id
                                        );

                /* Schedule Downlink PDCH */
                gl1_msg_rx_pacch_pdtch
                           ( (gl1_defs_tn_type) SUB_MOD( i,l1_get_current_timeslot(gas_id), GL1_DEFS_SLOTS_IN_FRAME),
                              l1_get_gprs_arfcns(gas_id),
                              &transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                              &transfer_data_ptr->packet_transfer_div_agc,
#endif
                              &gpl1_store_ptr->rlc_mac_dl_info[rlc_mac_dl_info_index++],
                              gas_id
                           );

                /* Subtract one to account for the cs timeslot which is always
                 * assumed to have been tasked before the packet timeslot.
                 */
                if( rlc_mac_dl_info_index > ( PL1_DTM_MULTISLOT_RX - 1 ) )
                {
                  /* too many TS allocated, so shout */
                  MSG_GERAN_ERROR_0_G("PL1: Dynamic MS class failure");
                }

                /* Increment in pointer to dl fifo for next dl schedule
                 * reset to beginning if end of fifo is reached
                 */

                if (transfer_data_ptr->rlc_mac_dl_buff.in_ptr >=
                            &(gpl1_store_ptr->gpl1_gprs_dyn_dl_data[MAX_NUM_DL_MSGS]) )
                {
                  transfer_data_ptr->rlc_mac_dl_buff.in_ptr =
                                          (gl1_defs_rx_pkt_data_type*)
                                          & (gpl1_store_ptr->gpl1_gprs_dyn_dl_data[0]);
                }

              }  /* end of if(tn_alloc.. */

              /* move DL alloc onto the next timeslot */
              dl_tn_alloc>>=1;
            } //end of for(i.. MAX_TN

            /* DL BLOCK start for PACCH or PDTCH to be set in 1 frame */
            pacch_pdtch_dl_blk_start_cnt = 0;

            /****************************************
            ***
            *** Check if we need to transmit an RRBP
            ***
            ****************************************/

            /* invalidate which RRBP message to tx
             * in case no RRBP are in the upcomming block
             */
            i = SIZE_OF_RRBP_BUFF;

            /* check if rrbp is comming up in the next block */
            if (gpl1_gprs_get_num_rrbp_msg(gas_id))
            {
              uint32  fn_num = ADD_FN( FN, NPL1_FN_OFFSET );

              while ( (i = gpl1_gprs_get_matching_rrbp(fn_num, fn_num, GPL1_INVALID_TN, gas_id) )
                       < SIZE_OF_RRBP_BUFF)
              {
                /* Store away the message type of the RRBP we about to transmit to help
                *  us in deciding which message to transmit if our next RRBP has a clash
                */

                transfer_data_ptr->last_rrbp_msg_type = gpl1_gprs_get_rrbp_msg_type(i, gas_id);

                /* We are about to TX an RRBP, NPL1 should schedule the RRBP and save
                * the pdch if the USF was detected in the previous block and transmit
                * it when the usf is detected again.
                *
                * Need to check if RLC data pointer is deleted for the RRBP tn or
                * if the pointer is saved until the next USF is detected
                */


                /* Check if we are sending a PDAN with FAI=1 for the first time
                * or for DL unack mode we are sending a PCA with FAI=1
                */
                switch( transfer_data_ptr->last_rrbp_msg_type )
                {
                  case  PDAN_MSG_WITH_FAI:
                  case  PCA_DL_FAI_MSG:

                  /* start timer 3192 */
                  grlc_dl_start_t3192(gas_id);
                  /* just started T3192 so clear out the flag */
                  transfer_data_ptr->first_t3192_start = FALSE;
                  MSG_GERAN_MED_0_G("PL1: START_T3192");

#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
                  gpl1_gprs_reset_tfi_when_T3192_running(gas_id);
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
                  break;

                default:
                  /* No action required here */
                  break;
                } /* End of switch statement on last rrbp msg type */

                /* Only schedule RRBP if we have a valid ta for txing
                 * normal bursts or access bursts
                 */
                transfer_data_ptr->last_rrbp_msg_type = gpl1_gprs_transfer_tx_rrbp(i, gas_id);

                pacch_pdtch_ul_blk_start_cnt = 0;

              } /* (i < SIZE_OF_RRBP_BUFF) && ...*/

              /* Check to see if there is a release pending */
              if ( gpl1_gprs_awaiting_release(gas_id) )
              {

                /* Check if the routes to release have resulted in the approriate RRBP
                * msgs being txd.
                * if ul is being release have all ul rrbp been txd
                * if ul and dl is being released have all rrbp been txd
                * Partial releases do not result in a return to idle mode
                * Full releases result in a return to idle
                */
                if ((transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL) ||
                    (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
                    (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL))
                {

                  MSG_GERAN_LOW_1_G("UL TBF DEAD [%d]",FN);
                  /*  Reset ul_desc pointer so any previous READS discarded. */
                  memset(&transfer_data_ptr->rlc_ul_status,0,sizeof(rlc_ul_status_T) );

                  transfer_data_ptr->rlc_ul_status.ul_desc_ptr =
                                            &transfer_data_ptr->rlc_ul_status.ul_desc[0];

                  /* clear the deffered buffer flag */
                  GPL1_RESET_DEFER_BUFF( gas_id );

                  /* check that RLC FIFO is disabled, it is done when
                   * the UL Release is rcvd
                   */
                  if (transfer_data_ptr->rlc_ul_data_available)
                  {
                    MSG_GERAN_MED_1_G("RLC UL FIFO still enabled!! [%d]",FN);
                    transfer_data_ptr->rlc_ul_data_available = FALSE;
                  }

                } /* (transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL) || */

                /* Have we just scheduled our last RRBP for the TBF*/
                if (!pacch_pdtch_ul_blk_start_cnt)
                {
                  /* Ensure we only set the  pca fn once!! */
                  if (!transfer_data_ptr->wait_for_pca_valid)
                  {
                    /* wait for any new assignments on the PACCH
                     * last block to be processed is block when PCA is sent
                     */
                    transfer_data_ptr->wait_for_pca_fn =  ADD_FN(FN,10);

                    transfer_data_ptr->wait_for_pca_valid = TRUE;

                    MSG_GERAN_LOW_2_G("OLD leaving idle FN [%d] leave fn %d",
                                                FN,transfer_data_ptr->wait_for_pca_fn);
                  } else
                  {
                    MSG_GERAN_LOW_1_G("**** pca_valid still true [%d]",FN);
                  }
                }

              } /*(transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL) && */

            } /* gpl1_gprs_get_num_rrbp_msg() */

            /***
            *** Schedule UL PACCH_PDTCH TN(s)
            ***/

            /* disable sending tx's for dynamic allocation
            * for first block as USF's haven't been detected
            * yet so the tx's will just be discarded
            *
            * Also if dynamic UL tbf is not present then the
            * number of timeslots (nts) will be zero so don't
            * schedule dynamic UL
            *
            * Finally check that we have a valid ta otherwise we can't tx
            * normal bursts
            */

            /* Fetch current value of enable_tx */
            gpl1_store_ptr = gpl1_read_xfer_isr_store(gas_id);

            if (   ( gpl1_store_ptr->enable_tx )
                 &&( transfer_data_ptr->dynamic_tbf_ptr->nts )
                 &&( !l1_transfer_data[gas_id].wait_for_pca_valid ) )
            {
              boolean               update_sig_info = FALSE;
              gl1_defs_coding_type  cs              = GL1_DEFS_CS1_CODING;
              uint8                 sched_tx_count  = 0;

              /* schedule usf specific msgs based on the nts
              * available less any rrbp mesgs which are scheduled
              */

              /* Determine what data to transmit and
              * which coding scheme to use
              */

              while(
                     transfer_data_ptr->dynamic_tbf_ptr->sched_msgs <
                     (
                       transfer_data_ptr->dynamic_tbf_ptr->nts -
                       gpl1_gprs_get_num_rrbp_msg_txd(gas_id)
                     )
                   )
              {
                /* Determine what data to transmit and
                 * which coding scheme to use
                 */
                uint8* payload_data[3];
                geran_pdu_priority_t priority;

                gl1_defs_tx_pkt_data_type  pkt_data;
                gl1_defs_puncturing_type   ps[2] = { GL1_DEFS_PUNCTURING_1,
                                                     GL1_DEFS_PUNCTURING_1 };

                /* Schedule EGPRS TBF MCS 1 - 9 or GPRS CS 1-4
                 * if we have RLC data.
                 * If not schedule UL Dummy CS1.
                 */

                priority = gpl1_gprs_get_egprs_ul_data( &cs,
                                             ps,
                                             payload_data,
                                             gas_id,
                                             FALSE        /* single block alloc */
                                           );

                /* Schedule dynamic UL pdch */

                /* for GPRS write tx_data to msg_data item 0
                 * for EGPRS write header to msg_data item 0
                 * write msg1 to imsg_data item 1
                 * and write msg2 to msg_data item 2
                 */
#ifdef DEBUG_SHOW_PL1_BSN
                #error code not present
#endif
                pkt_data.egprs.hdr           = payload_data[0];
                pkt_data.egprs.data[0]       = payload_data[1];
                pkt_data.egprs.data[1]       = payload_data[2];
                pkt_data.egprs.puncturing[0] = ps[0];
                pkt_data.egprs.puncturing[1] = ps[1];

                if( cs > GL1_DEFS_MCS9_CODING )
                {
                  MSG_GERAN_ERROR_2_G(" cs error %d [%d]",cs,gl1_get_FN( gas_id ));

                }
                else if(payload_data[0] == NULL)
                {
                  /* For EGPRS and GPRS ensure at least Payload_data[0]
                   * is valid as this is either the EGPRS header pointer
                   * or the GPRS data pointer
                   */
                  MSG_GERAN_HIGH_1_G("No data to send [%d]",gl1_get_FN( gas_id ));
                }
                else
                {

                  gl1_msg_tx_pacch_pdtch_dynamic( &pkt_data,
                                                  cs,priority,gas_id );
                  txed_coding_scheme[sched_tx_count] = cs;
                  sched_tx_count++;
                  /* Allow signal info to be called */
                  update_sig_info = TRUE;
                }

                transfer_data_ptr->dynamic_tbf_ptr->sched_msgs++;
              } /* End of while loop */

              
              /*If FT handler hasn't been added, and ext_utbf_nodata fllag has been set, then there will be no TxMsgs to Transmit, 
              so to avoid T3180 timer expiry, L1 must reset the USF_DETECT flag*/
              if(!update_sig_info && (l1_idle_data->layer1_extra_gprs_sys_info.ext_utbf_nodata == TRUE))
              {
                GPL1_RESET_USF_DETECT(gas_id);
              }

    	      /* Ensure that sched_msgs relates to number of
               * Tx scheduled.
               * There may be a difference if the coding scheme
               * is out of the range cs1 < cs > MCS9 or there is not
               * data to send
               */
              transfer_data_ptr->dynamic_tbf_ptr->sched_msgs = sched_tx_count;
              
              /***
              *** update tx power for each assigned timeslot
              ***/
              if (update_sig_info)
              {
                /* temp place holder for TX PWR to write Eng Mode Info */
                int16 tmp_tx_pwr[8] = {0};

                /* Initialise pdch count */
                uint8  num_pdch = 0;
                gl1_msg_ul_pacch_pdtch_signal_type  signal_info[4] =
                {
                  { GL1_DEFS_TN_0, 0x7fff },
                  { GL1_DEFS_TN_0, 0x7fff },
                  { GL1_DEFS_TN_0, 0x7fff },
                  { GL1_DEFS_TN_0, 0x7fff }
                };

                /* loop around all 8 timeslots */
                for (i=0;i< MAX_TN;i++)
                {
                  /* a timeslot allocation has been found */
                  if (transfer_data_ptr->dynamic_tbf_ptr->usf_value[i]!=GPL1_INVALID)
                  {
                    /*  Serving Cell Measurements */
                    /*  Power control value */

                    uint8  slammed_slot = gpl1_gprs_ts_conv(i, gas_id);
                    signal_info[num_pdch].TXLEV = (int16)gprs_serv_cell_meas_ptr->pch[i];

                    switch ( frame_counters[gas_id].FNmod13 )
                    {
                    case  7:
                      gpl1_gprs_scell_disable_int_meas( (gl1_defs_tn_type) i , gas_id);
                      break;
                    default:
                      break;
                    }

                    signal_info[num_pdch].tn = (gl1_defs_tn_type) slammed_slot;
                    num_pdch++;
                  }

                } /* End of for loop */

                /* Subtract one to account for the cs timeslot which is always
                 * assumed to have been tasked before the packet timeslot.
                 */
                if(  ( num_pdch == 0 )
                   ||( num_pdch > ( PL1_DTM_MULTISLOT_TX - 1 ) )
                  )
                {
                  /* Report an F3 error message */
                  MSG_GERAN_ERROR_1_G(" PL1 dtm: invalid num ul pdch %u ",num_pdch);

                  /* Return from the dynamic control function */
                  return  idle;
                }

                /* Configure initial power of frequency values for assigned pdch's
                *  Need to investigate this more, do we need to specify power and arfcn
                * every time new data is set to NPL1 ????
                *
                * This should be the place where we update power and frequency
                * for the comming radio block
                */
                /*lint -save -e645 fixed by above return */

                gl1_msg_set_pacch_pdtch_dynamic_signal_info
                                              (
                                                 signal_info,
                                                 transfer_data_ptr->dynamic_tbf_ptr->nts,
                                                 l1_get_gprs_arfcns (gas_id),
                                                 gas_id
                                              );

                for (i=0; (i<num_pdch &&
                           i<transfer_data_ptr->dynamic_tbf_ptr->nts &&
                           i<transfer_data_ptr->dynamic_tbf_ptr->sched_msgs);
                           i++)
                {
                  /* Report using original TS allocation and not slammed */
                  gpl1_log_gprs_power_control( GL1_MSG_UL_PACCH_PDTCH,
                                               (gl1_defs_tn_type) gpl1_gprs_ts_conv_unslam( signal_info[i].tn ,gas_id),
                                               signal_info[i].TXLEV,
                                               txed_coding_scheme[i], gas_id );
                }

                for (i=0; (i<num_pdch &&
                     i<transfer_data_ptr->dynamic_tbf_ptr->nts &&
                     i<transfer_data_ptr->dynamic_tbf_ptr->sched_msgs);
                     i++)
                {
                  tmp_tx_pwr[i] = signal_info[i].TXLEV;
                }
                geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, gas_id);

                /*lint -restore */
                /* UL BLOCK start for PACCH or PDTCH to be set in 1 frame */
                pacch_pdtch_ul_blk_start_cnt = 0;

              } /* end of if(update_sig_info) .. */

            } /* End of if (   ( gpl1_store_ptr->enable_tx ) .... */

            /* first block has passed so allow transmissions */
            GPL1_ENABLE_TX( gas_id );

          } /* End of disable_rx_tx == FALSE */
          else
          {
              MSG_GERAN_HIGH_1_G(" PS is aborting FNmod13 %u",frame_counters[gas_id].FNmod13);
          }

          break;
        } /* end of case */


        default:
          break;

      } /* End of switch (FNmod13) */



      /**********************************************
        Does PS, CS, or CS and PS require aborting
       *********************************************/
      if(dtm_data->l1_dtm_abort_mode != L1_DTM_NONE)
      {

        switch(dtm_data->l1_dtm_abort_mode)
        {
          case L1_DTM_PS_ONLY:
          case L1_DTM_CS_PS:
          {
            if(abort_setup_once[gas_id])
            {
              /* Stop the tx callbacks from being actioned*/
              GPL1_SET_ABORT_CB( gas_id );

              /* Clear wait for pca vars */
              transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
              transfer_data_ptr->wait_for_pca_valid = FALSE;

              /* L1 leaves packet transfer mode after transmitting the
               * Packet Control Acknowledgement hence it is safer to
               * empty the rrbp queue on dropping back into idle as
               * opposed to when starting packet transfer.
               */
              gpl1_gprs_reset_rrbp_enteries( gas_id);

              /* save FN where PS abort was first called */
              abort_fnmod13[gas_id] = frame_counters[gas_id].FNmod13;
              abort_setup_once[gas_id] = FALSE;

              /* Abort the PS channels */
              gl1_msg_abort_dl_pacch_pdtch(gas_id);

              /* clear out the PACCH UL FIFO if we have any msgs remaining from transfer */
              gpl1_gprs_reset_pacch_fifo(gas_id);
            }

            /* Check if the PS has aborted */
            if( IS_PDCH_CHAN_ABORTED[abort_fnmod13[gas_id]] == frame_counters[gas_id].FNmod13 )
            {
              ps_idle[gas_id] = TRUE;

              /* Returning back to idle, so if we are doing an EGPRS TBF
               * then turn EGPRS off
               */
              if (dtm_data->l1_dtm_abort_mode == L1_DTM_CS_PS)
              {

                if(l1_transfer_data[gas_id].egprs_or_gprs_tbf == TBF_MODE_EGPRS)
                {
                  /* turn off EGPRS */
                  gl1_msg_set_egprs_mode(FALSE,FALSE,gas_id);
                  MSG_GERAN_HIGH_0_G("EGPRS-DTM Leave clk at 122Mhz");

                }else
                {
                  /* returning to idle from DTM-GPRS so just de-register EDGE clock */
                  MSG_GERAN_HIGH_0_G("GPRS-DTM leave clk at 122 Mhz");

                }
              }else if(dtm_data->l1_dtm_abort_mode == L1_DTM_PS_ONLY)
              {
                if(l1_transfer_data[gas_id].egprs_or_gprs_tbf == TBF_MODE_EGPRS)
                {
                  gl1_msg_set_egprs_mode(FALSE,FALSE,gas_id);
                  MSG_GERAN_HIGH_0_G("EGPRS-DTM leave clk @ 122Mhz");
                }else
                {
                  MSG_GERAN_HIGH_0_G("GPRS-DTM leave clk @ 122Mhz");
                }
              }


            }

            MSG_GERAN_HIGH_3_G("Aborting PDCH [%d] cs_idle %d mode %d",gl1_get_FN( gas_id ),
                                         cs_idle[gas_id],dtm_data->l1_dtm_abort_mode);

            MSG_GERAN_HIGH_2_G("Aborting PDCH, ps_idle:%d, TBF mode:%d",ps_idle[gas_id],l1_transfer_data[gas_id].egprs_or_gprs_tbf );


            /*************************************************************************/
            /* Check the abort modes against what has actually aborted, to know when */
            /* the abort has fully completed and what to return to the calling       */
            /* when the abort is fully or partially complete.                        */
            /* Check if wtr hop is in progress and abort hop. CS+PS can not be       */
            /* aborted while there is a hop in progress                              */
            /*************************************************************************/
            if( (dtm_data->l1_dtm_abort_mode == L1_DTM_CS_PS) &&
#ifdef FEATURE_GSM_WTR_HOP
                gl1_msg_wtr_hop_abort(FALSE, gas_id) &&
#endif
                cs_idle[gas_id] && ps_idle[gas_id] )
            {
              /*Don't change abort mode to none if G2X search is ongoing*/
              if (!l1_sc_wcdma_srch_active(gas_id)	)
              {
              /* Abort has FULLY completed for CS and PS */
              idle = L1_DTM_CS_PS;
              /* Reset static and global abort flags */
              cs_idle[gas_id] = FALSE;
              ps_idle[gas_id] = FALSE;
              abort_setup_once[gas_id] = TRUE;
              dtm_data->l1_dtm_abort_mode = L1_DTM_NONE;
              }
#if defined ( FEATURE_GSM_EDTM )
              gpl1_remove_mac_l1_delete_ul_ctrl_msg( gas_id );
#endif
            }
            else
            if( (dtm_data->l1_dtm_abort_mode == L1_DTM_PS_ONLY) && !cs_idle[gas_id] && ps_idle[gas_id] )
            {
              /*Don't change abort mode to none if G2X search is ongoing*/
              if (!l1_sc_wcdma_srch_active(gas_id))
              {
              /* Abort has FULLY completed for PS only condition */
              idle = L1_DTM_PS_ONLY;
              /* Reset static and global abort flags */
              cs_idle[gas_id] = FALSE;
              ps_idle[gas_id] = FALSE;
              abort_setup_once[gas_id] = TRUE;
              dtm_data->l1_dtm_abort_mode = L1_DTM_NONE;
              }
              gl1_msg_set_afc_tt_mode(AFC_TT_MODE_DCH, gas_id);

#if defined ( FEATURE_GSM_EDTM )
              gpl1_remove_mac_l1_delete_ul_ctrl_msg( gas_id );
#endif
            }
            else
            if( (dtm_data->l1_dtm_abort_mode == L1_DTM_CS_PS) && cs_idle[gas_id] && !ps_idle[gas_id] )
            {
              /*Don't change abort mode to none if G2X search is ongoing*/
              if (!l1_sc_wcdma_srch_active(gas_id))
              { 
              /* Abort PARTIALLY completed for CS and PS, notify of cs only complete */
              idle = L1_DTM_CS_ONLY;
              }
            }
            else
            if( (dtm_data->l1_dtm_abort_mode == L1_DTM_CS_PS) && !cs_idle[gas_id] && ps_idle[gas_id] )
            {
              /*Don't change abort mode to none if G2X search is ongoing*/
              if (!l1_sc_wcdma_srch_active(gas_id))
              { 
              /* Abort PARTIALLY completed for CS and PS, notify of cs only complete */
              idle = L1_DTM_PS_ONLY;
              }
#if defined ( FEATURE_GSM_EDTM )
              gpl1_remove_mac_l1_delete_ul_ctrl_msg( gas_id );
#endif
            }

            break;
          }
          case L1_DTM_CS_ONLY:
          {
            /*-------------------------------------------------------------------------
              wait for CS channel abort to return success.
              main_command = L1_DEDICATED_STOP event is used to terminate CS channels
              -------------------------------------------------------------------------*/
            if(cs_idle[gas_id] == TRUE)
            {
              /*Don't change abort mode to none if G2X search is ongoing*/
              if (!l1_sc_wcdma_srch_active(gas_id))
              { 
              /* Abort has FULLY completed for CS only condition */
              idle = L1_DTM_CS_ONLY;
              /* Reset static and global abort flags */
              cs_idle[gas_id] = FALSE;
              ps_idle[gas_id] = FALSE;
              abort_setup_once[gas_id] = TRUE;
              dtm_data->l1_dtm_abort_mode = L1_DTM_NONE;
              }
            }

            break;
          }
          case L1_DTM_NONE:
          default:
          {
           /* No action required here */
          }
          break;
        }

      } /* end of l1_dtm_abort_mode */

      /* Tell the L1 ISR to send the release confirmation
       * msg to MAC since the waiting period ( ~10 tdma
       * frames ) passed following PCA/PDAN transmission
       * tasking to NPL1.
       *
       * Allow for the case where the pca fn occurs 1 tdma
       * frame after the start time of a high->low camp
       * timeslot assignment. This is done by checking
       * for the long frame in the radio block gap.
       */
      if( ( FALSE != transfer_data_ptr->wait_for_pca_valid )
        &&( ( FN == transfer_data_ptr->wait_for_pca_fn )
          ||( FN == ADD_FN( transfer_data_ptr->wait_for_pca_fn, 1 ) )
          )
        )
      {
        transfer_data_ptr->pca_sent      = TRUE;
        l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

        switch( l1_transfer_data[gas_id].release_pending )
        {
            case L1_FULL_RELEASE_UL:
            case L1_PARTIAL_RELEASE_UL:

                /* Reset the UL ST */
                l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
                break;

            default:
                /* No action required here */
                break;
        } /* End of switch statement */
      }


      /*********************************************************
       ** Send Frame tick notification to NPL1
       **
       ********************************************************/

      /* If DTM abort is completed, reset blk start cnts to ensure
      ** that no Tx/Rx cmd is issued in the current frame
      */
      if((dtm_data->l1_dtm_abort_mode == L1_DTM_CS_PS)
         && (idle == L1_DTM_CS_PS)
        )
      {
        MSG_GERAN_HIGH_0_G("gpl1_dtm_control_cs_ps: Clearing PDTCH start cnt vars");
        pacch_pdtch_ul_blk_start_cnt = GPL1_INVALID;
        pacch_pdtch_dl_blk_start_cnt = GPL1_INVALID;
      }

      gpl1_dtm_send_ftn( &pacch_pdtch_ul_blk_start_cnt,
                         &pacch_pdtch_dl_blk_start_cnt, gas_id );


      if ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
      {
        /* Local fn value store, used to help identify a long frame
         * due to the lowest dl timeslot decreasing.
         */
        static uint32  previous_fn[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(GPL1_INVALID_FN) };

        /* Make sure a valid DL TBF is currently active */
        if( NULL != transfer_data_ptr->dl_tbf_ptr )
        {
          /* Take a copy of the downlink tbf timeslot allocation */
          const uint8  dl_timeslot_alloc =
            transfer_data_ptr->dl_tbf_ptr->ts_allocation;

          /* Inform the queued nodes of the frame tick.
           * Check if any of them are due to be converted
           * quality reports on this frame.
           * Warning: Use the function call to get the latest FN
           * value incase this is a long frame since FN
           * variable is set before any potential timing
           * changes take effect.
           */
          gpl1_egprs_search_qual_meas_queue( dl_timeslot_alloc, gas_id );

          if ( 2 == SUB_FN( FN, previous_fn[gas_id] ) )
          {
            /* This is a long frame because the timing
             * device omitted a frame interrupt.
             * Simulate the lost frame interrupt by
             * searching the queue again.
             */
            gpl1_egprs_search_qual_meas_queue( dl_timeslot_alloc, gas_id );
          }
        }
        else
        {
          /* Prevent EPDAN requests, from the previous DL TBF,
           * from being generated incase a new DL TBF is assigned.
           */
          gpl1_egprs_empty_qual_meas_queue(gas_id);
        }

        if ( (ps_idle[gas_id] == TRUE) || (idle == L1_DTM_PS_ONLY) )
        {
          /* We are leaving packet transfer mode,
           * configure the qual meas module to switch off
           * the bep and interference measurements.
           */
          gpl1_egprs_serv_cell_qual_meas_reset( gas_id);

          /* Clear any possibly remaining queued quality
           * measurement nodes.
           */
          gpl1_egprs_empty_qual_meas_queue(gas_id );

          /* Need to make sure to avoid searching twice
           * on normal-length frame
           */
          previous_fn[gas_id] = GPL1_INVALID_FN;

          transfer_data_ptr->l1_test_mode = L1_TEST_MODE_OFF;

          MSG_GERAN_MED_1_G(" Test mode OFF FNmod13 %u",frame_counters[gas_id].FNmod13);

          /* Turn OFF Incremental Redundancy when
           * leaving egprs pkt transfer.
           */
          gl1_msg_IR_enable( FALSE ,gas_id);

          /* Reset the FULL BSN range for DL IR */
          gl1_msg_IR_clear_memory(0,2048,gas_id);

          MSG_GERAN_HIGH_1_G("IR OFF %d mem cleared",FN);

          transfer_data_ptr->rlc_mode = RLC_MODE_INVALID;

          /* clear out the PACCH FIFO as we have not sent it */
          gpl1_gprs_reset_pacch_fifo(gas_id);

        } else
        {
          /* Update copy of current fn for comparison
           * purposes in the frames to follow.
           */
          previous_fn[gas_id] = FN;
        }
      }

#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_state_machine(
          &l1_dedicated_data_ptr->channel_specification.channel_info_1_after,
          gas_id);
#endif

      break;
    } /* End of case L1_DTM_RUNNING */

    default:
      break;
  } /* end of switch ((l1_tskisr_blk->sub_state) */

  MSG_GERAN_HIGH_1_G("gpl1_dtm_control_cs_ps: ret_val:%d ",idle);

  /* Return the idle enum */
  return idle;
}

/*===========================================================================

FUNCTION gpl1_dtm_data_init

DESCRIPTION
  Resets any flags and variables critical to dtm transfer operation.

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void gpl1_dtm_data_init(gas_id_t gas_id)
{
  abort_setup_once[gas_id] = TRUE;
}


#else

/* Dummy function to avoid warning when FEATURE_GSM_DTM is NOT defined */
void gpl1_dtm_dummy(void)
{
}

#endif /* FEATURE_GSM_DTM */

