/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             L 1    D E D    I S R

GENERAL DESCRIPTION
   This module handles the dedicated mode isr processing.

EXTERNALIZED FUNCTIONS
  L1_control_dedicated
    Procedure to service the tdma interrupt when in dedicated mode.

  L1_change_channel
    Procedure to control the change of the dedicated channel

  L1_change_FN
    This function controls the change of frame timebase for
    synchronising to a new cell

  L1_change_timeslot
    This function controls the change of timeslot

  L1_notify_start
    This function sends an appropriate confirm message to L1 that
    the channel is established


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_ded.c#2 $
  $DateTime: 2020/04/29 10:32:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
20/08/18   sk        CR2181447 Opt for fading cells reconf scheduling in dedicated mode
09/04/18   sk        CR2217923 To limit the CR2212372 fix to Blind HO scenarios
27/03/18   sk        CR2212372 Ensure negative qb_lag adjusted properly for HO
20/03/17   mk        CR2020999 Prx Drx power difference needs to be calculated & applied to NCELLs before REP_PERIOD_IND to avoid delays
21/02/17   bg        CR2007570 Use PRx power for DRx power instead of default gain (-70dBm) when RxD enabled
11/08/17   rc        CR2138578  ML logging - GL1 Changes
30/01/17   mk        CR1113152 Keep RxD always enabled in FTM mode based on RxDForcedON NV
16/02/17   cc        CR1010614 G2L idle time transfer and MEAS only support
05/08/16   tsk       CR1048165 Reset GL1 and Gdrivers flag to remain in sync after RxD exit.
08/12/16   mk        CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
08/12/16   mk        CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
02/05/16   mk        CR1010068 Add DTF events in dedicated mode
08/03/16   dg        CR975456 Send tx on to RF after frame adjustment has happened for X2G Handover case
27/11/15   hd        CR924654 SDCCH operation before starting time should be performed if before parameters are valid
18/11/15   jk        CR940372:Remove statements related to VAMOS 2 code which are outside switch case
14/10/15   snjv      CR919955 FR38046:QSH Call End Data Collection
29/04/15   dg        CR829595 In dedicated mode,don't start G2X IRAT measurements on a frame on which TS adjustment is happening
10/06/15   sjv       CR818672 GL1 will send HANDOVER_IND to RR with Error code as OTHER_ERROR when acq. has failed attempting blind handover 
04/06/15   jk        CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
23/02/15   br        CR794803:UE not moving to a new  CS channel after recieving MAC_L1_DTM_CHANNEL_ASSIGN_REQ  which has cs reconfiguration.
17/12/14   br        CR779053 UE is not acknowledging Packet Assignment message leading to DTM call drop
16/12/14   jk        CR753423:LIF for Idle,voice and data
20/05/14   dg        CR613013 Long Term fix on GL1 for TS 34.123 & TC 8.3.11.12 GCF Testcase
                     failure due to RACH failure on GSM cell
03/09/14   pjr       CR718235 Check current sub is not hopping when moving to idle
20/08/14   pjr       CR711604 Check connected mode hopping is not in progress when moving to idle mode
10/07/14   jk        CR688587:GL1 changes to support LIF feature
24/07/14   og        CR684296. Sanity check the gas id in dedicated isr and SCE.
29/04/14   pjr       CR600612 Dedicated WTR Hop initial code drop
19/02/14   jk        CR608290:GERAN RxD ON/OFF adaptive algorithm
22/01/13   ws        CR602047 - Allow MVS/GHDI to operate on SUB2 if VS driver is not present
24/01/14   cah       CR598552: enable sleep for DSDS GSTMR+VSTMR
19/12/13   jk        CR503359 - GL1 changes to support Rx diversity
28/11/13   cja       CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
24/09/13   jk        CR549227:Shifted SACCH changes for VAMOS2
24/07/13   jk        CR464982:Changes to support GEA5_4 ciphering
21/05/13   cs        TSTS Updates
16/04/12   ws        CR473890 - rewind frame number if total qbit offset when syncing to target cell
                     is greater than 5000 Qbits
20/09/12   pg        CR401909 Call Tx band change during frame-slamming frame for inta-cell HO
10/01/12   cja       CR327967 Add request MCPM AEq on for FR
29/06/11   nt        DSDS CR289454: ASYNC should ignore timing adjustments for CS and PS connections
22/03/11   ap        CR279430 DSDS Timing value wraps casusing frame misalignment
29/03/11   cja       CR 281211 In DTM timeslot change always adjust all NCell offsets
07/01/11   ap        CR 269153 Incorrect frame alignment when GL1 goes from CS TS 7 to DTM CS TS0
20/05/10   ap        CR 238752  fix problem with DTM frame inc and reconnect
03/03/10   ab        Code Featurization for FEATURE_GSM_DTM and FEATURE_GSM_EDTM
15/02/10   cja       Re-enable GSAC clock when dedicated started if cipher enabled.
24/07/09   ws        CR 180476 - Improve performance of Dedicated AGC
26/02/09   ap        Correction to CR 172492
17/02/09   ap        CR 172492 Dont reset the data valid flags for a DTM assignment
03/12/08   ws        Added FEATURE_GSM_DYNAMIC_SAIC
12/01/08   gj        CR165617 - mainline FEATURE_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING feature.
27/03/08   ap        CR 139993 Introduce flag to indicate measurement reporting period has been interrupted
15/02/08   nt        CR136979:- Advance the starting time by one more frame to compensate for
                     the ISR overrun fix.
09/11/07   nf        Added FEATURE_GSM_R_SACCH
31/08/07   ws        Fixed Klocwork warnings
20/07/07   nt        CR123397 - Allows L2 Ack to be sent for CMM rather than deleting
                     it when moving channels.
16/05/07   ws        Added FEATURE_GSM_L1_CONTROL_SET_BAND CR 114613
09/05/07   nt        CR118109:Delay sending of the Handover failure notification
                     until connecting back onto the old channels.
05/03/07   cs        Making CR112989 calculation consistent with sc updates and CR99129
28/02/07   nt        CR112989: Turned on FEATURE_GSM_TRANSFER_DED_SERVING_CELL_INFO, by
                     removing switch from around the code.
07/12/06   og        Fix for CR99129, increment NCELL QB adjustement by 5000.
27/11/06   og        Remove un-necessary arithmetic modulo operations.
27/10/06   ws        Fixed Lint Warnings
07/09/06   og        EDTM updates following testing on Agilent 8960.
24/08/06   ws        Fix for CR 100103 subtract an extra 5000 qbits if we are current slammed
                     on tn or 7 and we want to return to old cell
14/08/06   ws        Allow dedicated to be timeslot slammed to lowest received tn
                     and also to place the CS timeslot on TN 2
10/07/06   dv        Change in calc. for a particular condition in L1_DTM_change_timeslot
01/03/06   ip        Removed camped_in_prev_frame flag, IOT Ericsson, CR84986
11/16/05   ip        Changed handover functionality for DTM slamming
09/21/05   dp        Fix to reset measurements on channel startup
08/23/05   ws        Added FEATURE_GSM_DTM code in gl1_include_in_rxlev() since sub_state
                     are different in DTM
08/16/05   dp        Fix to not zero out the entire l1_dedicated_data struct
                     and thus wipe out the ACS when reconnecting to GSM
08/12/05   ws        Added initial FEATURE_GSM_DTM support
06/24/05   dp        Lint cleanup
06/24/05   dp        Fix to exclude BCCH carrier from RXLEV average if its in
                     the hopping list and the PWRC bit is set
05/25/05   dp        Added l1_init_dedicated_data
01/07/05   dp        Moved sending MPH_IMMED_ASSIGN_CNF to ISR toprevent race
                     condition with RR
01/05/05   dp        Merged dedicated timeline changes for audio optimizations
                     to mainline
06/04/04   sv        Removed all references to XXX_mframe_power_XXX.
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
03/22/04   gfr       Detect if too many SACCH decodes fail and RR does not react.
03/18/04   gsc       Some more lint clean up.
02/25/04   gw        Some lint clean up.
07/31/03   dp        Fix for measurement rpt issue reported by Samsung
07/23/03   dp        Fix to handle ncell offset = 0 in L1_sync_to_current.
                     Fixes 26.10.2.4 failure at Cetecom
07/15/03   gw        Changed call to l1_sci_monscan_go() to l1_sci_go().
07/11/03   gfr       Lint cleanup.
06/24/03   dp        Start up physical channels 2 frames before start time.
                     This fixes 26.6.5.1 M=3 DCS failure at Cetecom
05/12/03   ATM       Moved Frame Number management entirely inside GTMRS
05/02/03   gw        Merged in power transfer changes from branch.
04/02/03   gw        Changed calls to monscan functions to reflect new names.
03/10/03   dp        Temp patch for the memory corruption problem in
                     14.7.1 at Cetecom.  Store off starting_time at channel
                     initialization
02/25/03   dp        Added HR support featurized under
                     FEATURE_GSM_HALF_RATE and enabled by default
01/13/03   dp        Fixed bug in handling elapsed starting time
01/10/03   gw        Removed all references to l1_sc_set_sync_dedi()
                     replaced with l1_sc_set_sync().
11/22/02   dp        Fix to call l1_sc_set_sync_dedi even when the timeslot is
                     unchanged.  This is to handle Ncell offset going out
                     of bounds on a SDCCH to TCH transition
10/16/02   dp        Added support to pass in a flag to L1_control_full_rate
                     to inform the TCH if a freq redefinition
10/01/02   ATM       Moved central get and set FN routines to l1_utils
09/19/02   dp        Added L1_is_FN_after_freq_redef_start_time to handle freq
                     redef in the middle of a block
09/12/02   dp        Cleaned up unused code
09/04/02   plm       Added extern for l1_sc_set_sync_dedi() to eliminate compiler warning.
07/23/02   ATM       Added check for null to prevent crash in L1_change_channel
07/23/02   ATM       Removed "static" from L1_sync_to_() functions so they can
                     be shared by l1_acq for InterRat Handovers.
06/14/02   dp        Adjusted starting_time_advanced to account for channel
                     startup changes in l1_tch
06/10/02   dp        Added fixes to start at the specified starting time. Added
                     function L1_is_FN_after_start_time. Support to stop sending
                     SACCH meas rpts after channel release
05/23/02   dp        Fixed issue with not sending RANDOM_ACCESS_CNF if
                     start time was specified but with the time elapsed
04/24/02   dp        Fixed bug in transitioning from the BEFORE to AFTER
                     channel.  We were stuck in the BEFORE channel
03/20/02   dp        Added support for power control in steps of 2dB
02/20/02   dp        Added support for handovers
02/15/02   mk        Using gl1_msg_correct_timing() to change timeslot.
12/18/01   dp        Changes to support SC functionality on SDCCH
12/10/01   thh       Hw_*() functions are obsolete.  Use the new interface
                     gl1_hw*() functions.
12/07/01   mk        Updated to return idle status in tch and sdcch8
                     modes.
11/27/01   mk        Modified L1_DEDICATED_STOP to work with SDCCH/4 and FACCH
                     signaling.
10/15/01   JC        Added MPH_CHANNEL_MODE_MODIFY_CNF support in
                     L1_notify_start().
09/20/01   jc        Added message layer for ciphering.
08/31/01   JC        Removed unnecessary compile switch and added QCT std
                     fcn hdr templates.
05/29/01   JC        Cleanup of SPINCO_SIMULATION.
04/24/01   jc        Initial version .
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1_drx.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_sc.h"
#include "l1i.h"
#include "l1_sc_int.h"
#include "msg.h"
#include "l1_log.h"     /* For logging */
#include "l1_utils.h"   /* for ADD_FN and SUB_FN macros */
#include "gtmrs.h"
#include "gtmrs_g.h"      /* for GSTMR_INC_FN_BY_ONE() */
#include "err.h"        /* For ERR_FATAL macro */
#include "gl1_hw_clk_ctl.h"
#include "gl1_msg_wtr_hop.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "geran_dual_sim_g.h"

#ifdef FEATURE_QSH_EVENT_METRIC
#include "l1_log_qsh.h"
#endif /*FEATURE_QSH_EVENT_METRIC*/

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

/*Used to advance the starting time*/
#define START_TIME_FRAME_ADVANCE 2

static void L1_invalidate_tx_buffers ( dedicated_data_T *ded_dtr_ptr );
void L1_sync_to_target  ( l1_sc_bs_T *current_cell, l1_sc_bs_T *target_cell,gas_id_t gas_id );
void L1_sync_to_current ( l1_sc_bs_T *current_cell, gas_id_t gas_id );
/*
 * Flag to indicate if a full set of rxqual values have been taken
 * during a reporting period.
 */
boolean meas_report_valid[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( FALSE ) };

/*Frame number on which TS adjustment has happend*/
uint32 ded_gstmr_cha_fn[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE(GL1_DEFS_INVALID_FN) };


static uint32 starting_time[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };

extern dedicated_data_T l1_dedicated_data[];

/* Count number of sequential failed SACCH blocks to make sure RR is
   paying attention */
int l1_bad_sacch_block_cnt[NUM_GERAN_DATA_SPACES];

extern void gl1_hw_set_rxdiv_wait_for_block_start(boolean  block_start,
                                           gas_id_t gas_id);

boolean l1_ded_drx_agc_init[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

#ifdef FEATURE_GSM_DTM
/* Used in l1_ded_store_ftn_info() and l1_ded_get_ftn_info() */
#define GL1_LOCAL_FTN_INIT  { FALSE, FALSE, FALSE, \
                              FALSE, FALSE, FALSE, \
                              FALSE, FALSE, FALSE }

static gl1_msg_frame_tick_type local_ftn_type[NUM_GERAN_DATA_SPACES] =
  {INITIAL_VALUE(GL1_LOCAL_FTN_INIT)};
#endif /* end of FEATURE_GSM_DTM */


/*===========================================================================

FUNCTION  l1_ded_set_l1_diversity_status

DESCRIPTION
  This function sets the l1 diverity flag status.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_ded_set_l1_diversity_status(boolean status, gas_id_t gas_id)
{
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  l1_dedicated_data_ptr->l1_enable_rxdiversity = status;
}

/*===========================================================================

FUNCTION  L1_CONTROL_DEDICATED

DESCRIPTION
  This function controls dedicated mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_control_dedicated(gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

    /* locals */
    word                starting_time_advanced;
    boolean             apply_freq_redef = FALSE;
    channel_type_T      physical_channel_type;
    boolean             idle = FALSE;
    l2_channel_type_T   logical_channel_type;
    ARFCN_T             serving_BCCH;
    sys_band_mask_type  band_mask;
#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    gas_id = check_gas_id(gas_id);


    /*
      Advance starting time by START_TIME_FRAME_ADVANCE
      frames to account for delay in channel startup
    */
    starting_time_advanced = ADD_MOD( frame_counters[gas_id].FNmod42432, START_TIME_FRAME_ADVANCE, 42432 );

    /*
        Reset starting time once it elapses.  We use this to determine if the
       starting time has elapsed or not in L1_is_FN_after_start_time
    */
    if ( l1_dedicated_data_ptr->starting_time )
    {
        if ( L1_determine_time(frame_counters[gas_id].FNmod42432, l1_dedicated_data_ptr->starting_time) )
        {
            starting_time[gas_id] = 0;
        }
    }

    /* Frequency redefinition  imminent */

    if ((l1_dedicated_data_ptr->freq_redef_imminent)&&
        (L1_determine_time(frame_counters[gas_id].FNmod42432,l1_dedicated_data_ptr->freq_redef_starting_time)) )
    {
        /* It is now time to change the channel info */

        switch (l1_tskisr_blk->sub_state)
        {
        case L1_DED_BEFORE:
            l1_dedicated_data_ptr->channel_specification.channel_info_1_before =
            l1_dedicated_data_ptr->new_channel_info;
            break;

        case L1_DED_AFTER:
            l1_dedicated_data_ptr->channel_specification.channel_info_1_after =
            l1_dedicated_data_ptr->new_channel_info;
            break;

        default:
            break;
        }
        l1_dedicated_data_ptr->freq_redef_imminent = FALSE;

        /*
            This flag is enabled for just a frame and informs the TCH that
            the frequencies are to be switched in this frame because the
            frequency redefinition start time has elapsed
        */
        apply_freq_redef = TRUE;
    }

    switch (l1_tskisr_blk->sub_state)
    {
    case L1_DED_SUSPEND:
        break;

    case L1_DED_INIT:

        /*if ded_int is for X2G HO, turn tx on now*/
        if(l1_check_for_x2g_ho(gas_id))
        {
          MSG_GERAN_HIGH_0_G("For X2G IRAT HO. Turn TX ON now");
          gl1_drx_off( TRUE, gas_id );
        }

        l1_indicate_x2g_ho(FALSE,gas_id);

        /*check if RF TX enable cnf is not received from RF, then retun from this function */
        if(l1_rf_tx_enable_cnf_rcv[gas_id] == FALSE)
        {
          MSG_GERAN_HIGH_0_G("RF not enabled. SO, return");
          return(idle);
        }
        /*
           Store starting time just once during channel initialization.
           This is a temp patch for the memory corruption issue at
           Cetecom in 14.7.1 where l1_dedicated_data_ptr->starting_time gets corrupted.
           This way we don't care if l1_dedicated_data_ptr->starting_time gets
           corrupted somewhere down the line
        */
        starting_time[gas_id] = l1_dedicated_data_ptr->starting_time;

        /* Enable SACCH meas reporting when starting up a new channel.
           L2 disables it later before releasing the channel
        */
        l1_dedicated_data_ptr->stop_meas_rpt = FALSE;
        /* Overwrite dedicated data with our parameters if we're moving to dedicated
           through our test code
        */

        /* Reset the bad sacch block count */
        l1_bad_sacch_block_cnt[gas_id] = 0;

        /* initialise agc averaging over 4 bursts
         * this may be overwritten to 2 bursts later
         * if we get assigned Half rate
         */
         sys_algo_agc_avg_init(FOUR_BURST_AVG, gas_id);
         l1_ded_drx_agc_init[gas_id] = TRUE;

#if defined (FEATURE_DUAL_SIM) && !defined (FEATURE_GSM_VS_DRIVER)
         gl1_msg_set_dedicated_gas_id(gas_id);
#endif
         l1_dedicated_data_ptr->schedule_fading_cells = FALSE;

#ifdef FEATURE_GSM_DYNAMIC_SAIC
#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
    gl1_hw_set_rxdiv_wait_for_block_start(FALSE, gas_id);

    if(gl1_rxd_ctl_flag.enableRxd)
      {
      l1_dedicated_data_ptr->l1_enable_rxdiversity = TRUE;
      l1_dedicated_data_ptr->prev_rxqual = RXD_INVALID_RXQUAL;
      gl1_set_enable_rxdiversity(TRUE,gas_id);
      }

    /*In DRx only mode enalble AEQ always in dedicated mode*/
    if((gl1_rxd_ctl_flag.enableRxd) && (gl1_rxd_ctl_flag.enableDrx)
      && (!gl1_rxd_ctl_flag.enablePrx))
      {
      l1_dedicated_data_ptr->l1_saic_dis_ena_state = TRUE;
      }
    else
#endif /*FEATURE_GSM_RX_DIVERSITY*/
    {
        l1_dedicated_data_ptr->l1_saic_dis_ena_state = FALSE;
    }
        /* Ensure SAIC is enabled since we may had a Handover from a previous cell with it off */
        gl1_hw_cfg_saic( TRUE, gas_id );
        gl1_hw_mcpm_amr_aeq_update(TRUE, gas_id);
#endif


/* Enable cipher clock for mDSP */
        if (gl1_msg_is_ciphering_enabled(gas_id))
        {
           gl1_hw_cipher_clock_required(TRUE, gas_id);
        }


        /* Find out whether before or after parameters are to be used */
        if (( l1_dedicated_data_ptr->channel_specification.starting_time_valid) &&
            ( l1_dedicated_data_ptr->channel_specification.before_parameters_valid) &&
            ( L1_determine_time(starting_time_advanced,
                                l1_dedicated_data_ptr->starting_time) == FALSE))
        {

            /*  use before parameters */
            physical_channel_type = l1_dedicated_data_ptr->channel_specification.
                                    channel_info_1_before.channel_type;


            if ((physical_channel_type == SDCCH_4)||
                (physical_channel_type == SDCCH_8))
            {
                /* Start surround cell engine */
                l1_sc_start(L1SCModeDediSDCCh, gas_id);
                logical_channel_type = SDCCH;
            }
            else
            {
                /* Start surround cell engine */
                l1_sc_start(L1SCModeDediTCh, gas_id);

                if ( physical_channel_type == FULL_RATE_TRAFFIC )
                {
                    logical_channel_type = FACCH_F;
                }
                else
                {
                    logical_channel_type = FACCH_H;
                }
            }

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_BEFORE_PARAM_VALID);
#endif

            l1_tskisr_blk->sub_state = L1_DED_BEFORE;
            l1_tskisr_blk->sub_sub_state = L1_DED_INIT;
            L1_change_channel(l1_dedicated_data_ptr,
                              &(l1_dedicated_data_ptr->channel_specification.
                                channel_info_1_before), FALSE, gas_id);

        }

        else
        {
            if (( l1_dedicated_data_ptr->channel_specification.starting_time_valid == FALSE)
                || (L1_determine_time(starting_time_advanced,
                                      l1_dedicated_data_ptr->starting_time) == TRUE))
            {
                /* User after parameters */
                physical_channel_type = l1_dedicated_data_ptr->channel_specification.
                                        channel_info_1_after.channel_type;

                if ((physical_channel_type == SDCCH_4) ||
                    (physical_channel_type == SDCCH_8))
                {
                    /* Start surround cell engine */
                    l1_sc_start(L1SCModeDediSDCCh, gas_id);
                    logical_channel_type = SDCCH;
                }
                else
                {
                    if ( physical_channel_type ==  FULL_RATE_TRAFFIC )
                    {
                        logical_channel_type = FACCH_F;
                    }
                    else
                    {
                        logical_channel_type = FACCH_H;
                    }
                    /* Start surround cell engine */
                    l1_sc_start(L1SCModeDediTCh, gas_id);
                }

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_AFTER_PARAM_VALID);
#endif

                l1_tskisr_blk->sub_state = L1_DED_AFTER;
                l1_tskisr_blk->sub_sub_state = L1_DED_INIT;
                L1_change_channel(l1_dedicated_data_ptr,
                                  &(l1_dedicated_data_ptr->channel_specification.
                                    channel_info_1_after), FALSE, gas_id);

            }
            else
            {
                /* This is the case of starting_time_valid = TRUE but
                 * before_parameters_valid = FALSE.  Wait here till
                 * start time
                 */
                MSG_GERAN_LOW_2_G("Curr time %d  Waiting for %d",starting_time_advanced,l1_dedicated_data_ptr->starting_time);
                break;
            }
        }


        /* Reset measurement variables for the new channel */
        l1_serving_cell_meas_ptr->rx_power               = 0;
        l1_serving_cell_meas_ptr->no_of_measurements     = 0;
        l1_serving_cell_meas_ptr->rx_power_sub           = 0;
        l1_serving_cell_meas_ptr->no_of_measurements_sub = 0;
        l1_serving_cell_meas_ptr->rx_power_val           = 0;
        l1_serving_cell_meas_ptr->no_of_measurements_val = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY
        l1_serving_cell_meas_ptr->divrx_power            = 0;
        l1_serving_cell_meas_ptr->divrx_power_sub        = 0;
        l1_serving_cell_meas_ptr->divrx_power_val        = 0;
        l1_serving_cell_meas_ptr->is_divrx_disabled_in_sacch_period = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

        gl1_msg_bep_init( gas_id );

        /* Reset for next reporting period */
        gl1_msg_rxqual_init( gas_id );

        /* get serving cel BCCH */
        serving_BCCH = gl1_get_serving_cell_arfcn(gas_id);

        /* If we are about to camp on a 1800 band cell ensure we are running at
         * low clock speed, otherwise set some other band to clock at high speed
         */
        if(serving_BCCH.band == SYS_BAND_DCS_1800)
        {
          band_mask = SYS_BAND_MASK_GSM_DCS_1800;
        }else
        {
          band_mask = SYS_BAND_MASK_GSM_850;
        }

        gl1_set_band_group(band_mask, gas_id);

        MSG_GERAN_HIGH_1_G(" DED ARFCN %d",serving_BCCH.num);
        /* No break after DED_INIT.  This saves us a frame */
        /* Start the SCE monscan process. */
        l1_sci_go(gas_id);

        L1_notify_start(physical_channel_type,
                        logical_channel_type,
                        l1_dedicated_data_ptr->L1_confirm_msg,
                        NO_ERROR,gas_id);
        /*  measurement reporting period has potentially been interrupted */
        meas_report_valid[gas_id] = FALSE;

#ifdef FEATURE_QSH_EVENT_METRIC
        MSG_GERAN_MED_0_G("GL1_QSH: Resetting QSH Params");
        /*INit the QSH Cache for TCH Rx Params*/
        gl1_qsh_cache_tch_rx_params (0, 0,
                                     0, 0,
                                     0, 0,
                                     gas_id);
#endif /*FEATURE_QSH_EVENT_METRIC*/

    case L1_DED_BEFORE:                             /*lint !e616 */
        if (( l1_dedicated_data_ptr->channel_specification.starting_time_valid) &&
            ( l1_dedicated_data_ptr->channel_specification.before_parameters_valid) &&
            ( L1_determine_time(starting_time_advanced,l1_dedicated_data_ptr->starting_time)
              == FALSE))
        {
            MSG_GERAN_HIGH_2_G("DED_BEFORE %d %d",starting_time_advanced,l1_dedicated_data_ptr->starting_time);

            if (!l1_dedicated_data_ptr->channel_specification.channel_info_1_before.hopping_flag)
            {
              geran_eng_mode_data_write(ENG_MODE_DED_OR_PDCH_ARFCN, &(l1_dedicated_data_ptr->channel_specification.channel_info_1_before.frequency_list.channel[0]),TRUE, gas_id);
            }
            else
            {
              geran_eng_mode_data_write(ENG_MODE_HOPPING_FREQUENCY_LIST,&(l1_dedicated_data_ptr->channel_specification.channel_info_1_before.frequency_list),TRUE, gas_id);
            }

#ifdef FEATURE_GSM_WTR_HOP
            gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif

            switch (l1_dedicated_data_ptr->channel_specification.channel_info_1_before.
                    channel_type)
            {
            case FULL_RATE_TRAFFIC:
                idle = L1_control_full_rate( l1_dedicated_data_ptr,
                                             &(l1_dedicated_data_ptr->channel_specification.channel_info_1_before),
                                             apply_freq_redef, gas_id);

                break;

            case HALF_RATE_TRAFFIC:
                idle = L1_control_half_rate(l1_dedicated_data_ptr,
                                            &(l1_dedicated_data_ptr->channel_specification.
                                              channel_info_1_before),
                                            apply_freq_redef, gas_id );

                break;

            case SDCCH_4:
                idle = L1_control_SDCCH_4(l1_dedicated_data_ptr,
                                          &(l1_dedicated_data_ptr->channel_specification.
                                            channel_info_1_before), gas_id);
                break;

            case SDCCH_8:
                idle = L1_control_SDCCH_8(l1_dedicated_data_ptr,
                                          &(l1_dedicated_data_ptr->channel_specification.
                                            channel_info_1_before), gas_id);
                break;

            default:

                break;

            }

#ifdef FEATURE_GSM_WTR_HOP
            gl1_msg_wtr_hop_state_machine(&l1_dedicated_data_ptr->channel_specification.channel_info_1_before, gas_id);

            /* Check that any hop in progress is completed/aborted before 
             * leaving dedicated mode 
             * */
            if (idle && 
                gl1_msg_wtr_hop_get_status() != WTR_HOP_INACTIVE &&
                gl1_msg_wtr_hop_get_gas_to_hop() == gas_id)
            {
              idle = FALSE;
            }
#endif

            break;
        }
        else
        {
            MSG_GERAN_HIGH_2_G("DED_AFTER %d %d",starting_time_advanced,l1_dedicated_data_ptr->starting_time);
            l1_tskisr_blk->sub_state = L1_DED_AFTER;

        }

        /* No break - Just fall through to after.  This ensures a seamless
           transition between before and after
        */

    case L1_DED_AFTER:
        if (!l1_dedicated_data_ptr->channel_specification.channel_info_1_after.hopping_flag)
        {
          geran_eng_mode_data_write(ENG_MODE_DED_OR_PDCH_ARFCN, &(l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list.channel[0]), TRUE, gas_id);
        }
        else
        {
          geran_eng_mode_data_write(ENG_MODE_HOPPING_FREQUENCY_LIST, &(l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list), TRUE, gas_id);
        }
#ifdef FEATURE_GSM_WTR_HOP
        gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif

        switch (l1_dedicated_data_ptr->channel_specification.channel_info_1_after.
                channel_type)
        {
        case FULL_RATE_TRAFFIC:
            idle = L1_control_full_rate(l1_dedicated_data_ptr,
                                        &(l1_dedicated_data_ptr->channel_specification.
                                          channel_info_1_after),
                                        apply_freq_redef, gas_id);

            break;

        case HALF_RATE_TRAFFIC:
            idle = L1_control_half_rate(l1_dedicated_data_ptr,
                                        &(l1_dedicated_data_ptr->channel_specification.
                                          channel_info_1_after),
                                        apply_freq_redef, gas_id );


            break;
        case SDCCH_4:
            idle = L1_control_SDCCH_4(l1_dedicated_data_ptr,
                                      &(l1_dedicated_data_ptr->channel_specification.
                                        channel_info_1_after), gas_id);

            break;

        case SDCCH_8:
            idle = L1_control_SDCCH_8(l1_dedicated_data_ptr,
                                      &(l1_dedicated_data_ptr->channel_specification.
                                        channel_info_1_after), gas_id);
            break;

        default:

            break;

        }

#ifdef FEATURE_GSM_WTR_HOP
        gl1_msg_wtr_hop_state_machine(&l1_dedicated_data_ptr->channel_specification.channel_info_1_after, gas_id);

        /* Check that any hop in progress is completed/aborted before 
         * leaving dedicated mode 
         * */
        if (idle && 
            gl1_msg_wtr_hop_get_status() != WTR_HOP_INACTIVE &&
            gl1_msg_wtr_hop_get_gas_to_hop() == gas_id)
        {
          idle = FALSE;
        }
#endif
        break;

    default:
        MSG_GERAN_ERROR_1_G("Unexpected state %d",l1_tskisr_blk->sub_state);
        break;

    } /* end of switch */

    /* Execute any commands that can occur during dedicated mode */

    if (l1_tskisr_blk->command == L1_SYNCH_TO_TARGET_COMMAND)
    {
        L1_sync_to_target(&l1_dedicated_data_ptr->current_cell_details,
                          &l1_dedicated_data_ptr->target_cell_details, gas_id);

        l1_tskisr_blk->command = L1_NULL_COMMAND;
    }

    if (  l1_tskisr_blk->command == L1_SYNCH_TO_CURRENT_COMMAND)
    {
        L1_sync_to_current(&l1_dedicated_data_ptr->current_cell_details, gas_id);

        l1_tskisr_blk->command = L1_NULL_COMMAND;
    }

    if ((  l1_tskisr_blk->command == L1_SET_CIPHERING_COMMAND)||
        (  l1_tskisr_blk->command == L1_SET_HANDVR_CIPH_COMMAND))
    {
        /*enable or disable ciphering*/
        l1_set_cch_ciphering_flag(l1_dedicated_data_ptr->channel_specification.cipher_flag, gas_id);
        if (l1_get_cch_ciphering_flag(gas_id))
        {
            gl1_defs_cipher_mode mode;

            mode = (gl1_defs_cipher_mode) l1_dedicated_data_ptr->channel_specification.cipher_algorithm;
      if((mode == A5_1)||(mode == A5_2)||(mode == A5_3))
      {
              gl1_msg_enable_ciphering( l1_dedicated_data_ptr->cipher_key, mode, gas_id);
      }
      else
      {
        gl1_msg_enable_ciphering(l1_dedicated_data_ptr->cipher_key_128, mode, gas_id);
      }

            MSG_GERAN_MED_0_G("L1: Ciphering enabled");
        }
        else
        {
            gl1_msg_disable_ciphering(gas_id);
            MSG_GERAN_MED_0_G("L1: Ciphering disabled");
        }


        if ( l1_tskisr_blk->command == L1_SET_CIPHERING_COMMAND)
        {
            /*send confirmation to RR*/

            /*
             * This won't actually send the message, as it is sent directly 2 lines
             * below, but it is used to signal some other stuff...
             */

            l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_SET_CIPHERING_CNF;

            L1isr_send_MPH_SET_CIPHERING_CNF( NO_ERROR, gas_id );
        }

        l1_tskisr_blk->command = L1_NULL_COMMAND;
    }
    if (idle == TRUE)
    {
        /*Reset R-SACCH params after leaving Dedicated Mode*/
        l1_rsacch_init_params(FALSE, gas_id);
    }
    if (l1_tskisr_blk->main_command == L1_DEDICATED_COMMAND)
    {
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
    }

    return( idle );

}

/*===========================================================================

FUNCTION  L1_CHANGE_CHANNEL

DESCRIPTION
  This function controls the change of dedicated channel

DEPENDENCIES
  pointer to the dedicated data array
  pointer to channel 1 info
  pointer to channel 2 info

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_change_channel ( dedicated_data_T       *ded_dtr_ptr,
                         channel_information_T  *chnl1_info_ptr,
                         boolean                dtm_assignment,
                         gas_id_t gas_id)

{
    /* Stop any activity on the old channel */
    /* Stop any timing waveforms */

    /* Stop any synthesiser transfers */

    /* Reset the DSPs */

    if (!ded_dtr_ptr || !chnl1_info_ptr)
    {
        MSG_GERAN_ERROR_2_G("Rcvd NULL ded_dtr_ptr %d or chnl1_info_ptr %d",
                  ded_dtr_ptr, chnl1_info_ptr);
        return;
    }

    if (NULL != ded_dtr_ptr)
    {
       MSG_GERAN_HIGH_3_G("DCCH valid %d SACCH valid %d confirm msg 0x%02X ",
                     ded_dtr_ptr->l1_dcch_s0_buffer_ptr->data_valid,
                     ded_dtr_ptr->l1_sacch_s0_buffer_ptr->data_valid,
                     ded_dtr_ptr->L1_confirm_msg);
    }

#ifdef FEATURE_GSM_DTM
    if(dtm_assignment == TRUE)
    {
      if(dtm_assign_cs_present[gas_id] == TRUE)
	  {
            dtm_assign_cs_present[gas_id] = FALSE;
	    /* clear the L2 transmit buffers if required */
            L1_invalidate_tx_buffers(ded_dtr_ptr);
	  }
      /* With DTM Assignments our CS TN will always be on TN 2 */
      MSG_GERAN_HIGH_1_G("Slam CS to rel 2 [%d]", gl1_get_FN( gas_id ));

      /* change timeslot so that the CS slot lies on relative TN 2 */
      L1_dtm_change_timeslot( ( (chnl1_info_ptr->TN - 2 + 8) % 8), chnl1_info_ptr, gas_id );

      /* Inform Message layer to Rx and Tx all CS chanels on TN 2 */
      gl1_msg_set_cs_timeslot(GL1_DEFS_TN_2, gas_id);
    }
    else
    {
      /* clear the L2 transmit buffers if required */
      L1_invalidate_tx_buffers(ded_dtr_ptr);

      if(ded_dtr_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST)
      {
        MSG_GERAN_HIGH_1_G("Slam CS to lowest [%d]",gl1_get_FN( gas_id ));
        L1_change_timeslot( chnl1_info_ptr->TN, gas_id);

        /* Inform Message layer to Rx and Tx all CS chanels on TN 0 */
        gl1_msg_set_cs_timeslot(GL1_DEFS_TN_0, gas_id );

      }else if (ded_dtr_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
      {

        MSG_GERAN_HIGH_1_G("Slam CS to rel 2 [%d]",gl1_get_FN( gas_id ));
        /* change timeslot so that the CS slot lies on relative TN 2 */
        L1_dtm_change_timeslot( ( (chnl1_info_ptr->TN - 2 + 8) % 8), chnl1_info_ptr, gas_id );

        /* Inform Message layer to Rx and Tx all CS chanels on TN 2 */
        gl1_msg_set_cs_timeslot(GL1_DEFS_TN_2, gas_id);
      }

    }
#else
    /* clear the L2 transmit buffers if required */
    L1_invalidate_tx_buffers(ded_dtr_ptr);

    /* change timeslot */
    L1_change_timeslot( chnl1_info_ptr->TN, gas_id);
#endif
    /* initiate RF Tx band change - only gets actioned if band has actually changed */
    gl1_hw_rf_set_tx_band(
        gl1_hw_arfcn_to_rfgsm_band(chnl1_info_ptr->frequency_list.channel[0]),
        gas_id           );
}

/*===========================================================================

FUNCTION  L1_CHANGE_TIMESLOT

DESCRIPTION
  This function controls the change of timeslot

DEPENDENCIES
  new timeslot number

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void L1_change_timeslot ( byte new_ts, gas_id_t gas_id )
{
    /* Locals */
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    sword qbit_decrease;
    /* In transfer so gasid must be sync sim */

    MSG_GERAN_HIGH_2_G("Change TS Old TS=%d New TS=%d",l1_get_current_timeslot(gas_id),new_ts);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

    qbit_decrease = (sword)((new_ts - l1_get_current_timeslot(gas_id)) * 625);
    gl1_msg_update_g2l_slot_change((int32)qbit_decrease, gas_id);

    if (qbit_decrease < 0)
    {
        qbit_decrease += 5000;
    }

    if ( qbit_decrease )
    {
        gl1_msg_correct_timing( (int32) qbit_decrease, gas_id );
    }

    if ( new_ts < l1_get_current_timeslot(gas_id) )
    {
          /* The above line of code should shorten the next frame
          but instead lengthens it by adding 8. thus we have
          lost a frame and must compensate by incrementing the
          frame counters an extra time */
          GSTMR_INC_FN_BY_ONE(gas_id);

        /* update frame counter */

        /*tell surrounding cell engine that timeslot has changed*/
        l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

        if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
        {

          /*Set skip G2X ded mode flag*/
          l1_sc_wcdma_data_ptr->l1_skip_g2x_ded_mode = TRUE;

          /*copy Gstmr fn to temp variable*/
          ded_gstmr_cha_fn[gas_id]= GSTMR_GET_FN_GERAN(gas_id);

          /* Transfer serving cell info separately */
          l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                 , gas_id );
        }

        l1_set_current_timeslot(new_ts, gas_id);   /* update current TS */
    }

    else if ( new_ts > l1_get_current_timeslot(gas_id) )
    {
        /*tell surrounding cell engine that timeslot has changed*/
        l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);


        if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
        {

        /*Set skip G2X ded mode flag*/
        l1_sc_wcdma_data_ptr->l1_skip_g2x_ded_mode = TRUE;

        /*copy Gstmr fn to temp variable*/
        ded_gstmr_cha_fn[gas_id]= GSTMR_GET_FN_GERAN(gas_id);

        /* Transfer serving cell info separately */
        l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                               , gas_id );
        }

        l1_set_current_timeslot(new_ts, gas_id);   /* update current TS  */

    }
    else
    {
        /*
          Even if timeslot hasn't changes, check if Ncell offset is
          within bounds or if it needs to be wrapped around.  Especially
          for a SDCCH to TCH transition since the Ncell acq windows are
          different
        */
        l1_sc_set_sync(0, 0, gas_id);

        if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
        {
          /* Transfer serving cell info separately */
          l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, 0, gas_id);
        }

    }
}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION  L1_DTM_CHANGE_TIMESLOT

DESCRIPTION
  This function controls the change of timeslot during DEDICATED and
  DTM modes the the CS slot exists on relative TN 2

DEPENDENCIES
  new timeslot number

RETURN VALUE
  none

SIDE EFFECTS
  None

The following tables show whether frame inc
is required or not when changing timeslots.
This is only valid for DTM timebase reference
of timeslot 2.
N = no change in frame number
x = condition not valid
I = increment frame number

new_ts > current_ts
new_ts  |   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
---------------------------------------------
curr_ts | 0 | x | N | N | N | N | N | I | I |
        | 1 | x | x | N | N | N | N | I | I |
        | 2 | x | x | x | N | N | N | I | I |
        | 3 | x | x | x | x | N | N | I | I |
        | 4 | x | x | x | x | x | N | I | I |
        | 5 | x | x | x | x | x | x | I | I |
        | 6 | x | x | x | x | x | x | x | N |
        | 7 | x | x | x | x | x | x | x | x |

new_ts < current_ts
new_ts  |   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
---------------------------------------------
curr_ts | 0 | X | X | X | X | X | X | X | X |
        | 1 | I | X | X | X | X | X | X | X |
        | 2 | I | I | X | X | X | X | X | X |
        | 3 | I | I | N | X | X | X | X | X |
        | 4 | I | I | N | N | X | X | X | X |
        | 5 | I | I | I | I | N | X | X | X |
        | 6 | N | N | N | N | N | N | X | X |
        | 7 | N | N | N | N | N | N | I | X |


N = no change in frame number
x = condition not valid
I = increment frame number
new_ts  |   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
---------------------------------------------
curr_ts | 0 | N | N | N | N | N | N | I | I |
cs on 0 | 1 | I | N | N | N | N | N | I | I |
        | 2 | I | I | N | N | N | N | I | I |
        | 3 | I | I | I | N | N | N | I | I |
        | 4 | I | I | I | I | N | N | I | I |
        | 5 | I | I | I | I | I | N | I | I |
        | 6 | I | I | I | I | I | I | I | I |
        | 7 | 2I| I | I | I | I | I | I | I |

===========================================================================*/
void L1_dtm_change_timeslot ( byte new_ts, channel_information_T  *chnl1_info_ptr, gas_id_t gas_id )
{
    /* Locals */
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    sword qbit_decrease;

    MSG_GERAN_HIGH_2_G("Change TS Old TS=%d New TS=%d ",l1_get_current_timeslot(gas_id),new_ts);

    qbit_decrease = (sword)((new_ts - l1_get_current_timeslot(gas_id)) * 625);
    gl1_msg_update_g2l_slot_change((int32)qbit_decrease, gas_id);

    if (qbit_decrease < 0)
    {
        qbit_decrease += 5000;
    }

    if ( qbit_decrease )
    {
        gl1_msg_correct_timing( (int32) qbit_decrease, gas_id );
    }

    if ( new_ts < l1_get_current_timeslot(gas_id) )
    {
        /* Don't increment the FN if we are currently camped on the
         * previous frame due to the CS slot exisiting in TN 0 or 1
         * since we increased the FN when we previously camped
         *
         * if we are slammed to lowest tn in dedicated mode on 6 or 7
         * then ensure we increment the FN as we are not camped in previous
         * frame
         */
        if ( ( (l1_get_current_timeslot(gas_id) >= 6) && (new_ts < 6) ) &&
                (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2) )
        {
          MSG_GERAN_HIGH_1_G("DTM don't inc FN [%d]", gl1_get_FN( gas_id ));

          /*tell surrounding cell engine that timeslot has changed*/
          /* If camped in previous frame (frame increased ), then timeslot change will decrement
             frame number when not necessary, so inc frame number !!
             only applies here as for ts 0 and 1 it was frame incremented */
          l1_sc_set_sync(1, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

          /* Transfer serving cell info separately */
          l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 1, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                 , gas_id);

        }
        else
        if ( ( (l1_get_current_timeslot(gas_id) == 7) && (new_ts == 6) ) &&
                (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST) )
        {
           /*tell surrounding cell engine that timeslot has changed*/
           l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

           /* Transfer serving cell info separately */
           l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                  , gas_id );

           /* The above line of code should shorten the next frame
            but instead lengthens it by adding 8. thus we have
            lost a frame and must compensate by incrementing the
            frame counters an extra time Increment the MS TDMA frame by two and update
           ncell information.
            */
           MSG_GERAN_HIGH_1_G("DTM frame inc by 2, FN now = [%d]", gl1_get_FN( gas_id ));

           GSTMR_ADVANCE_FN_GERAN(2,gas_id);
        }else
        {
          /*tell surrounding cell engine that timeslot has changed*/
          l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

          /* Transfer serving cell info separately */
          l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                 , gas_id );

          /* The above line of code should shorten the next frame
          but instead lengthens it by adding 8. thus we have
          lost a frame and must compensate by incrementing the
          frame counters an extra time */
          MSG_GERAN_HIGH_1_G("DTM frame inc, FN now = [%d]",gl1_get_FN( gas_id ));

          GSTMR_INC_FN_BY_ONE(gas_id);
        }

        l1_set_current_timeslot(new_ts, gas_id);   /* update current TS */
    }
    else if ( new_ts > l1_get_current_timeslot(gas_id) )
    {
      boolean  increment_by_one_frame = FALSE;

      if ((l1_get_current_timeslot(gas_id) <= 5) && (new_ts >= 6))
      {
          GSTMR_INC_FN_BY_ONE(gas_id);
          MSG_GERAN_HIGH_1_G("DTM frame inc, FN now = [%d]",gl1_get_FN( gas_id ));

          increment_by_one_frame = TRUE;
      }
      if (new_ts >=6)
      {
          if ( (new_ts == 7) && (l1_get_current_timeslot(gas_id) == 6) && (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST))
          {
            GSTMR_INC_FN_BY_ONE(gas_id);
            MSG_GERAN_HIGH_1_G("DTM frame inc, FN now = [%d]",gl1_get_FN( gas_id ));

            increment_by_one_frame = TRUE;
          }
          /*tell surrounding cell engine that timeslot has changed*/
          if( FALSE == increment_by_one_frame )
          {
            l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

            /* Transfer serving cell info separately */
            l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                   , gas_id);
          }
          else
          {
            l1_sc_set_sync( 0, (int16)( (( new_ts - l1_get_current_timeslot(gas_id) ) * 625) - 5000), gas_id);

            /* Transfer serving cell info separately */
            l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)( (( new_ts - l1_get_current_timeslot(gas_id) ) * 625) - 5000)
                                   , gas_id);
          }
      }
      else
      {
          /*tell surrounding cell engine that timeslot has changed*/
          l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

          /* Transfer serving cell info separately */
          l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                 , gas_id );
      }
      l1_set_current_timeslot(new_ts, gas_id);   /* update current TS  */
    }
    else
    {   /* Take into account the transitions from TS6(7)
         * on lowest to TS6(7) on relative 2.
         * Increment the MS TDMA frame by one and update
         * ncell information.
         */
        if( (5 < l1_get_current_timeslot(gas_id)) && (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST))
        {
          GSTMR_INC_FN_BY_ONE(gas_id);
          MSG_GERAN_HIGH_1_G("DTM frame inc, FN now = [%d]",gl1_get_FN( gas_id ));
          l1_sc_set_sync(0, (int16)5000, gas_id);
        }else
        {
        /*
          Even if timeslot hasn't changes, check if Ncell offset is
          within bounds or if it needs to be wrapped around.  Especially
          for a SDCCH to TCH transition since the Ncell acq windows are
          different
        */

          l1_sc_set_sync(0, 0, gas_id);
        }
        /* Transfer serving cell info separately */
        l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0 , 0, gas_id);
    }
}

#endif /* FEATURE_GSM_DTM */
/*===========================================================================

FUNCTION  L1_NOTIFY_START

DESCRIPTION
  This function sends an appropriate confirm message to L1 when
  the channel is established.

DEPENDENCIES
  physical_channel_type (SDCCH_4, SDCCH_8, etc.. )
  logical_channel_type type (SDCCH,SACCH or FACCH)
  message type
  error code

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_notify_start (channel_type_T physical_channel_type,
                      l2_channel_type_T  logical_channel_type,
                      byte message_type,
                      byte error_code, gas_id_t gas_id )
{
    /* Locals */
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    /* code */
    switch ( message_type )
    {
    case MPH_HANDOVER_IND:
        /* Tell RR we failed, 
           If BSIC is known then Timing advance is out of range
           else Acquisition failed at L1, so Lower Layer Failed
           RR will send appro. error cause based on the enum sent by L1 */
        if (l1_dedicated_data_ptr->target_cell_details.bsic_known)
        {
        L1_send_MPH_HANDOVER_IND(HANDOVER_OUT_OF_RANGE, gas_id);
        }
        else
        {
           L1_send_MPH_HANDOVER_IND(OTHER_ERROR, gas_id);
        }

        L1isr_send_PH_READY_TO_SEND_IND(logical_channel_type, gas_id);
        L1isr_send_PH_READY_TO_SEND_IND(SACCH, gas_id);
        break;

    case MPH_IMMED_ASSIGN_CNF:

        L1_send_MPH_IMMED_ASSIGN_CNF(error_code, gas_id);
        if ( error_code == NO_ERROR)
        {
            L1isr_send_PH_CONNECT_IND(physical_channel_type, gas_id);

        }
        break;


    case MPH_CHANNEL_ASSIGN_CNF:
        L1isr_send_MPH_CHANNEL_ASSIGN_CONF(error_code, gas_id);
        if ( error_code == NO_ERROR)
        {
            L1isr_send_PH_READY_TO_SEND_IND(logical_channel_type, gas_id);
            L1isr_send_PH_READY_TO_SEND_IND(SACCH, gas_id);
        }
        break;

    case MPH_RECONNECT_CHANNEL_CNF:
        L1isr_send_MPH_RECONNECT_CHANNEL_CNF(gas_id);
        L1isr_send_PH_READY_TO_SEND_IND(logical_channel_type, gas_id);
        L1isr_send_PH_READY_TO_SEND_IND(SACCH, gas_id);

        break;
    case MPH_SET_CIPHERING_CNF:

        /* This is now sent earlier */

        /*    L1isr_send_MPH_SET_CIPHERING_CNF(NO_ERROR); */

        L1isr_send_PH_READY_TO_SEND_IND(logical_channel_type, gas_id);
        L1isr_send_PH_READY_TO_SEND_IND(SACCH, gas_id);

        /* Annul all stored data for Tx apb */
        l1_dedicated_data_ptr->l1_dcch_s0_buffer_ptr->data_valid = FALSE;
        l1_dedicated_data_ptr->l1_sacch_s0_buffer_ptr->data_valid = FALSE;
        break;

    case MPH_CHANNEL_MODE_MODIFY_CNF:
        L1_send_MPH_CHANNEL_MODE_MODIFY_CNF(CHANNEL_MODE_CHANGED, gas_id);
        break;


#ifdef FEATURE_GSM_DTM

    case MPH_DTM_CHANNEL_ASSIGN_CNF:
        L1isr_send_MPH_DTM_CHANNEL_ASSIGN_CNF( gas_id);

        if ( l1_tskisr_blk->current_params.L1Data.pDtm_data->l1_dtm_init_mode
                                                                      != L1_DTM_PS_ONLY )
        {
          /* Only send this to L2 if it has been suspended
           * as there was a new CS assignment
           */
#ifdef  FEATURE_GSM_EDTM
          if( l1_tskisr_blk->sub_state == L1_EDTM_INIT )
          {
            L1isr_send_PH_CONNECT_IND(physical_channel_type, gas_id);
          }
          else
          {
#endif

            L1isr_send_PH_READY_TO_SEND_IND(logical_channel_type, gas_id);
            L1isr_send_PH_READY_TO_SEND_IND(SACCH, gas_id);

#ifdef  FEATURE_GSM_EDTM
          }
#endif
        }
        break;

#endif /* End of FEATURE_GSM_DTM */
    default:
        break;
    }

    if(message_type == PH_RANDOM_ACCESS_CNF)
    {
      L1_send_PH_RANDOM_ACCESS_CNF(l1_dedicated_data_ptr->access_burst_content,
                                     frame_counters[gas_id].FNmod42432, gas_id);
    }

}

/*===========================================================================

FUNCTION  L1_SYNC_TO_TARGET

DESCRIPTION
  This function controls the change of frame timebase for synchronising
  to a new cell.

DEPENDENCIES
  Current and target cell details
RETURN VALUE

SIDE EFFECTS
  Frame number and frame tick are synchronized to target cell

===========================================================================*/
void L1_sync_to_target ( l1_sc_bs_T *current_cell, l1_sc_bs_T *target_cell, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = l1_tskisr_blk->current_params.L1Data.pDedi_data;
    int16 ncell_qbit_lag, total_qbit_lag;
    sword ts_qbit_lag;
    uint8 new_ts;

#ifdef FEATURE_GSM_DTM
    if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
    {
      new_ts = ((l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN - 2 + 8) % 8);
    }else
    {
      new_ts = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN;
    }
#else
    new_ts = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN;
#endif

    if (!current_cell || !target_cell)
    {
        MSG_GERAN_ERROR_2_G("Rcvd NULL current_cell %d or target_cell %d",
                  current_cell, target_cell);
        return;
    }

    if (target_cell->bsic_known)
    {
        /*save old timeslot number*/
        current_cell->TN = l1_get_current_timeslot(gas_id);

        if (target_cell->qbit_lag < 0 )
        {
            ncell_qbit_lag = 5000 + target_cell->qbit_lag;

            /*
               Change FN to that of target cell using fn lag
               Store offset and fn lag to current cell to revert
               back to original cell if handover fails
            */
            current_cell->fn_lag = L1_change_FN( target_cell->fn_lag, gas_id );
            current_cell->qbit_lag = -target_cell->qbit_lag;

            /* As we are aligning the negative qbit lag w.r.t next frame tick, 
               we need to offset the FN increment */
            if ( l1_tskisr_blk->acq_for_blind_ho )
            {
              GSTMR_INC_FN_BY_ONE(gas_id);
              MSG_GERAN_HIGH_0_G("FN increment for negative qbit lag during blind HO");	  
            }

            /* Translate Ncell information w.r.t new cell */
            l1_sc_set_sync( ADD_FN(target_cell->fn_lag,1), target_cell->qbit_lag, gas_id);

            if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
            {

              /* Transfer serving cell info separately */
              l1_sc_ba_cell_set_sync( &l1_dedicated_data_ptr->previous_cell_details,
                                      ADD_FN(target_cell->fn_lag,1),
                                      target_cell->qbit_lag, gas_id );
            }
        }
        else
        {
            ncell_qbit_lag = target_cell->qbit_lag;

            /*
               Change FN to that of target cell using fn lag
               Store offset and fn lag to current cell to revert
               back to original cell if handover fails
            */
            current_cell->fn_lag = L1_change_FN( target_cell->fn_lag, gas_id );
            current_cell->qbit_lag = 5000 - target_cell->qbit_lag;

            /* Translate Ncell information w.r.t new cell */
            l1_sc_set_sync( target_cell->fn_lag, target_cell->qbit_lag, gas_id );

            if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
            {
              /* Transfer serving cell info separately */
              l1_sc_ba_cell_set_sync( &l1_dedicated_data_ptr->previous_cell_details,
                                      target_cell->fn_lag,
                                      target_cell->qbit_lag, gas_id );
            }
        }

        l1_tskisr_blk->acq_for_blind_ho = FALSE;

        MSG_GERAN_HIGH_2_G("Sync Tgt Old TS=%d New TS=%d",l1_get_current_timeslot(gas_id),new_ts);

        /*
           The surround cell offset is based on the surround cell's
           tone and SCH which are in timeslot 0.  So we've synced up
           to timeslot 0 after we slam to ncell_qbit_lag.  So we have
           to compute ts_qbit_lag relative to TS 0
        */
        l1_set_current_timeslot(0, gas_id);

        ts_qbit_lag = (sword)((new_ts - l1_get_current_timeslot(gas_id)) * 625);

        if (ts_qbit_lag < 0)
        {
            ts_qbit_lag += 5000;
        }

#ifdef FEATURE_GSM_DTM
       if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
       {
          if ( new_ts < l1_get_current_timeslot(gas_id) )
          {
              /* The above line of code should shorten the next frame
                 but instead lengthens it, thus we have
                 lost a frame and must compensate by incrementing the
                 frame counters an extra time */
              GSTMR_INC_FN_BY_ONE(gas_id);

              /*tell surrounding cell engine that timeslot has changed*/
              l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

              if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
              {
                /* Transfer serving cell info separately */
                l1_sc_ba_cell_set_sync( &l1_dedicated_data_ptr->previous_cell_details, 0,
                                        (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                        , gas_id );
              }

              l1_set_current_timeslot(new_ts, gas_id);   /* update current TS */
          }
          else if ( new_ts > l1_get_current_timeslot(gas_id) )
          {
              /*tell surrounding cell engine that timeslot has changed*/
              if ( new_ts >= 6)
              {
                  GSTMR_INC_FN_BY_ONE(gas_id);
                  l1_sc_set_sync(0, (int16)(( new_ts - 8 ) * 625), gas_id);

                  if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
                  {
                    /* Transfer serving cell info separately */
                    l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - 8 ) * 625)
                                           , gas_id);
                  }
              }
              else
              {
                l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

                if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
                {
                  /* Transfer serving cell info separately */
                  l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                         , gas_id );
                }

              }
              l1_set_current_timeslot(new_ts, gas_id);   /* update current TS  */
          }
          else
          {
              /*
                 Even if timeslot hasn't changes, check if Ncell offset is
                 within bounds or if it needs to be wrapped around.  Especially
                 for a SDCCH to TCH transition since the Ncell acq windows are
                 different
              */
              l1_sc_set_sync(0, 0, gas_id);

              if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
              {
                /* Transfer serving cell info separately */
                l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, 0, gas_id );
              }

#ifdef FEATURE_GSM_DTM
              /* current_timeslot = 0;  */
              l1_set_current_timeslot(new_ts, gas_id);   /* update current TS  */
#endif
          }
       }else
       {
         /****
          ****  We are slammed to our lowest receive timeslot
          ****/

         if ( new_ts < l1_get_current_timeslot(gas_id) )
         {
             /* The above line of code should shorten the next frame
                but instead lengthens it, thus we have
                lost a frame and must compensate by incrementing the
                frame counters an extra time */
             GSTMR_INC_FN_BY_ONE(gas_id);

             /*tell surrounding cell engine that timeslot has changed*/
             l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

             if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
             {
               /* Transfer serving cell info separately */
               l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                      , gas_id );
             }

             l1_set_current_timeslot(new_ts, gas_id);   /* update current TS */
         }
         else if ( new_ts > l1_get_current_timeslot(gas_id) )
         {
           /*tell surrounding cell engine that timeslot has changed*/
           l1_sc_set_sync(0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625), gas_id);

           if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
           {
             /* Transfer serving cell info separately */
             l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, (int16)(( new_ts - l1_get_current_timeslot(gas_id) ) * 625)
                                    , gas_id );
           }

           l1_set_current_timeslot(new_ts, gas_id);   /* update current TS  */
         }
         else
         {
             /*
                Even if timeslot hasn't changes, check if Ncell offset is
                within bounds or if it needs to be wrapped around.  Especially
                for a SDCCH to TCH transition since the Ncell acq windows are
                different
             */
            l1_sc_set_sync(0, 0, gas_id);

            if ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
            {
              /* Transfer serving cell info separately */
              l1_sc_ba_cell_set_sync(&l1_dedicated_data_ptr->previous_cell_details, 0, 0, gas_id );
            }

         }
       }  /* End of else if l1_current_slam_mode == L1_CS_TN_SLAM_TN2 */
#endif

        total_qbit_lag = ncell_qbit_lag + ts_qbit_lag;

        MSG_GERAN_HIGH_3_G("Slamming by %d + %d = %d",ncell_qbit_lag,ts_qbit_lag,total_qbit_lag);

        /* Adjust for both offsets together instead of in two stages earlier
         *  ncell_qbit_lag syncs up to timeslot 0 of new cell and ts_qbit_lag
         * then syncs up to new timeslot on new cell
         */
        if ( total_qbit_lag )
        {
            gl1_msg_correct_timing((int32) total_qbit_lag, gas_id );
        }

        /* If total lag >=5000, adjust frame number accordingly */
        if ( total_qbit_lag >= 5000 )
        {
            GSTMR_REWIND_FN ( 1 ,gas_id );
        }
    }
    else
    {
        MSG_GERAN_ERROR_0_G("Target cell details unavailable");
    }
}

/*===========================================================================

FUNCTION  L1_SYNC_TO_CURRENT

DESCRIPTION
  This function controls the change of frame timebase for synchronising
  to revert back to current cell

DEPENDENCIES
  Current and target cell details
RETURN VALUE

SIDE EFFECTS
  Frame number and frame tick are synchronized to current cell

===========================================================================*/
void L1_sync_to_current ( l1_sc_bs_T *current_cell, gas_id_t gas_id )
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    int  ts_qbit_change;
    int  total_qbit_change;
    int  real_qbit_lag;
    int  frame_lag;
    uint32 fn_lag;

    if ( !current_cell )
    {
        MSG_GERAN_ERROR_0_G("Rcvd NULL current_cell ptr");
        return;
    }

    if (l1_get_current_timeslot(gas_id) != 0)
    {
        /* Find qbit lag to timeslot 0 */
        ts_qbit_change = 5000 - ((sword)l1_get_current_timeslot(gas_id) * 625);
    }
    else
    {
        ts_qbit_change = 0;
    }

    MSG_GERAN_HIGH_2_G("Sync curr Old TS=%d New TS=%d",l1_get_current_timeslot(gas_id),current_cell->TN);

    /* ts_qbit_change will sync up to timeslot 0 on current cell
       (which is the cell we've handed off to) and and qbit_lag
       will then sync up to the target cell(which is the old cell
       now) since qbit_lag is the offset between timeslot 0 on
       target cell and original timeslot of the old cell
    */
    total_qbit_change = ts_qbit_change + current_cell->qbit_lag;

    if ( total_qbit_change >= 5000)
    {
        total_qbit_change -= 5000;
    }

    /* Calculate the total lag accounting for the time slot we're on */
    real_qbit_lag = ((sword)l1_get_current_timeslot(gas_id) * 625) + 5000 + total_qbit_change;

    /* current_cell->qbit_lag == 5000 => Ncell offset = 0.  In this case
       we don't slam for the Ncell offset, so subtract a frame (5000)
    */

    /* In DTM we can be slammed to timeslot 6 or 7 in the previous frame
       if our CS timeslot is allocated on timeslot 0 or 1. so when returning
       back to our old cell subtract a frame from real_qbit_lag calculation
     */
#ifdef FEATURE_GSM_DTM
    if ( ( current_cell->qbit_lag == 5000 ) ||
         ( (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2) &&
           (l1_get_current_timeslot(gas_id) >= 6)
         ) )
    {
        real_qbit_lag -= 5000;
        MSG_GERAN_HIGH_3_G("reduced qbit lag %d current tn %d old tn %d",
                                      real_qbit_lag,l1_get_current_timeslot(gas_id),current_cell->TN);
    }
#endif /* FEATURE_GSM_DTM */

    /*
       This give us the total lag in terms of number of frames -
       essentially the number of frame ticks we'd miss by lengthening
       the frame to slam it
    */
    frame_lag = (int)(real_qbit_lag/5000);

    /*Delay frame tick by this lag */
    if ( total_qbit_change )
    {
        gl1_msg_correct_timing( (int32) total_qbit_change, gas_id );
    }

    /* Reduce fn_lag to compensate for the missed frame ticks */
    fn_lag = (uint32)SUB_FN((int)current_cell->fn_lag, frame_lag);

    (void)L1_change_FN( fn_lag, gas_id );

    /* Translate Ncell information w.r.t new cell */
    l1_sc_set_sync( fn_lag,
                    total_qbit_change % 5000, gas_id
                  );

    l1_set_current_timeslot(current_cell->TN, gas_id);
}

/*===========================================================================

FUNCTION  L1_change_FN

DESCRIPTION
  This function controls the change of frame timebase for synchronising to
  a new cell

DEPENDENCIES
  frame lag

RETURN VALUE
  The inverse, i.e. the number to pass to this function again if
  resynchronising with the original cell.

SIDE EFFECTS
  None

===========================================================================*/
dword L1_change_FN ( dword FN_lag , gas_id_t gas_id)
{
    /* Decrement FN by FN_lag */
    GSTMR_REWIND_FN( FN_lag, gas_id );

    /* Return inverse of FN_lag to allow counter-adjustment */
    return(FRAMES_PER_HYPERFRAME - FN_lag);
}

/*===========================================================================

FUNCTION  L1_IS_FN_AFTER_START_TIME

DESCRIPTION
  This function determines if the specified FN is after the start time or not

DEPENDENCIES
  None

RETURN VALUE
  current FN

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_is_FN_after_start_time( uint32 fn, gas_id_t gas_id )
{
    /*starting_time = 0 => no start time or start time elapsed */
    if ( !starting_time[gas_id] )
    {
        return( TRUE );
    }
    else
    {
        return( L1_determine_time ( fn%42432, (uint16)starting_time[gas_id] ) );
    }
}

/*===========================================================================

FUNCTION  L1_IS_FN_AFTER_FREQ_REDEF_START_TIME

DESCRIPTION
  This function determines if frequency redefinition is in progress and if so
  if the passed in FN is after the start time

DEPENDENCIES
  None

RETURN VALUE
  current FN

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_is_FN_after_freq_redef_start_time( uint32 fn ,gas_id_t gas_id)
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    /*
       Return TRUE only if freq redef is in progress and if the passed in FN
       is after the start time
    */
    return( (l1_dedicated_data_ptr->freq_redef_imminent)&&
            (L1_determine_time( fn%42432, l1_dedicated_data_ptr->freq_redef_starting_time)) );
}

/******************************************************************************
 *  *
 *   *  Function name:  l1_init_dedicated_data
 *    *  -----------------------------------------------
 *     *
 *      *  Description:
 *       *  ------------
 *        *  Initializes idle data values
 *         *
 *          *  Parameters:
 *           *  -----------
 *            *  NONE
 *             *
 *              *  Return Value:
 *               *  --------
 *                *  NONE
 *                 *****************************************************************************/

void l1_init_dedicated_data( gas_id_t gas_id )
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
#if 0
   /* Zero out l1_idle_data */
   memset ( &l1_dedicated_data, NULL, sizeof ( dedicated_data_T) );
#endif

   /* This is a temp fix because zeroing out the entire dedicated data
    * struct wiped out the ACS when reconnecting back to GSM after a
    * G2W HO failure.   We're resetting just this variable for now
    * to address an earlier CR63134 where L1 kept RACHing because the below
    * variable was somehow set.  Need to optimize this fix to save off the
    * AMR ACS separately during a GtW HO
    */
    l1_dedicated_data_ptr->no_of_ab_to_send = 0;
}

/*===========================================================================

FUNCTION  GL1_INCLUDE_IN_RXLEV

DESCRIPTION
  This function determines if the RSSI measurement of the passed in
  ARFCN should be included in the RXLEV based on whether it is the
  carrier frequency and if the PWRC bit is set

DEPENDENCIES
  None

RETURN VALUE
  TRUE

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_include_in_rxlev ( ARFCN_T arfcn, gas_id_t gas_id )
{
   idle_data_T               *l1_idle_data = &l1_idle_data_store[gas_id];
   volatile ISRTIM_CMD_BLK*  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
   channel_information_T     *channel_info;
   ARFCN_T                   serving_cell_arfcn;

#ifdef FEATURE_GSM_DTM
   if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
   {
     switch (l1_tskisr_blk->sub_state)
     {
        case L1_DED_BEFORE:
           channel_info = &l1_dedicated_data_ptr->channel_specification.channel_info_1_before;
        break;

        case L1_DED_AFTER:
           channel_info = &l1_dedicated_data_ptr->channel_specification.channel_info_1_after;
        break;

        default:
           MSG_GERAN_ERROR_1_G("Invalid DED state %d",l1_tskisr_blk->sub_state);
           return ( TRUE );
     }
   }else
   {
     /* We will always use After Channel params in DTM */
     channel_info = &l1_dedicated_data_ptr->channel_specification.channel_info_1_after;
   }

#else
    switch (l1_tskisr_blk->sub_state)
     {
        case L1_DED_BEFORE:
           channel_info = &l1_dedicated_data_ptr->channel_specification.channel_info_1_before;
        break;

        case L1_DED_AFTER:
           channel_info = &l1_dedicated_data_ptr->channel_specification.channel_info_1_after;
        break;

        default:
           MSG_GERAN_ERROR_1_G("Invalid DED state %d",l1_tskisr_blk->sub_state);
           return ( TRUE );
     }
#endif

   serving_cell_arfcn =  gl1_get_serving_cell_arfcn(gas_id);

   /* If any of the hopping frequencies is on the BCCH carrier and the
    * PWRC bit is set, the RSSI measurement on that ARFCN should be excluded
    * from RXLEV
    */
   return ( !( channel_info->hopping_flag &&
            l1_idle_data->campedon_cell_data.PWRC &&
            ARFCNS_EQUAL( serving_cell_arfcn, arfcn ) ) );
}

/*===========================================================================

FUNCTION  L1_INVALIDATE_TX_BUFFERS

DESCRIPTION
       Annul all stored data for Tx unless this is a channel reconfig for a
       Channel Mode Modify, as this is transparent to L2

DEPENDENCIES
  None

RETURN VALUE
None

SIDE EFFECTS
  None

===========================================================================*/
static void L1_invalidate_tx_buffers(dedicated_data_T *ded_dtr_ptr)
{
  if (NULL != ded_dtr_ptr)
  {
    if (ded_dtr_ptr->L1_confirm_msg != (byte)MPH_CHANNEL_MODE_MODIFY_CNF)
    {
      if (ded_dtr_ptr->l1_dcch_s0_buffer_ptr != NULL)
      {
        ded_dtr_ptr->l1_dcch_s0_buffer_ptr->data_valid = FALSE;
      }

      if (ded_dtr_ptr->l1_sacch_s0_buffer_ptr != NULL)
      {
        ded_dtr_ptr->l1_sacch_s0_buffer_ptr->data_valid = FALSE;
      }
    }
  }

  return;
}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION  L1_DED_STORE_FTN_INFO

DESCRIPTION
  This function Stores the frame tick notification info for DTM operation.

DEPENDENCIES
  None

RETURN VALUE
None

SIDE EFFECTS
  None

===========================================================================*/
void l1_ded_store_ftn_info(gl1_msg_frame_tick_type ftn_type, gas_id_t gas_id)
{

  local_ftn_type[gas_id] = ftn_type;
}


/*===========================================================================

FUNCTION  L1_DED_GET_FTN_INFO

DESCRIPTION
This function Retrieves DEDICATED mode frame tick notification info for
use in DTM mode operation

DEPENDENCIES
  None

RETURN VALUE
  local_ftn_type - frame tick notification structure set in DTM mode

SIDE EFFECTS
  None

==========================================================================*/


gl1_msg_frame_tick_type l1_ded_get_ftn_info(gas_id_t gas_id)
{
  return (local_ftn_type[gas_id]);
}

/*===========================================================================

FUNCTION  l1_ded_store_bad_sacch_block_cnt

DESCRIPTION
  This function Stores the counter for bad sacch blocks

DEPENDENCIES
  None

RETURN VALUE
None

SIDE EFFECTS
  None

===========================================================================*/
void l1_ded_store_bad_sacch_block_cnt(int value, gas_id_t gas_id)
{
  l1_bad_sacch_block_cnt[gas_id] = value;
}


/*===========================================================================

FUNCTION  l1_ded_get_bad_sacch_block_cnt

DESCRIPTION
  This function Retrieves the counter for bad sacch blocks

DEPENDENCIES
  None

RETURN VALUE
  local_ftn_type - frame tick notification structure set in DTM mode

SIDE EFFECTS
  None

==========================================================================*/


int l1_ded_get_bad_sacch_block_cnt(gas_id_t gas_id)
{
  return l1_bad_sacch_block_cnt[gas_id];
}
#endif /* FEATURE_GSM_DTM */

/*===========================================================================

FUNCTION  if_starting_time_elapsed_or_valid

DESCRIPTION
  This function TRUE whether starting time time is in future and before parameters are valid or starting time has already elapsed

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - if starting time time is in future and before parameters are valid or starting time has already elapsed
  FALSE - Otherwise

SIDE EFFECTS
  None

==========================================================================*/
boolean if_starting_time_elapsed_or_valid(dedicated_data_T *ded_data, gas_id_t gas_id)
{
  if(!starting_time[gas_id])
  {
    return TRUE;
  }

  return(((L1_determine_time(frame_counters[gas_id].FNmod42432,starting_time[gas_id]) == FALSE) &&
          (ded_data->channel_specification.before_parameters_valid == TRUE)) ||
         (L1_determine_time(frame_counters[gas_id].FNmod42432,starting_time[gas_id]) == TRUE));
}
