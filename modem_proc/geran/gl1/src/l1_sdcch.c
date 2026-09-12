/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          L 1   D E D I C A T E D - C O N T R O L   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures to handle the ISR level processing
   of the dedicated  control channels.

EXTERNALIZED FUNCTIONS
  L1_control_SDCCH_4
    This function controls SDCCH/4.

  L1_control_SDCCH_8
    This function controls SDCCH/8.

  L1_send_dedicated_report
    This function sends  reports to RR and should be called at the end
    of the reporting period.


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:  L:/src/asw/GSM/L1/vcs/l1_sdcch.c_v   1.17   14 Jun 2002 20:24:52   dpandian  $
  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sdcch.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
24/05/16   mk       CR1010623: Add 1 Frame delay in SDCCH channel while moving from DED_INIT to DED_RUNNING 
08/12/16   mk       CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
27/11/15   hd       CR924654 SDCCH operation before starting time should be performed if before parameters are valid
18/11/15   jk       CR940372:Remove statements related to VAMOS 2 code which are outside switch case
04/06/15   jk       CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
13/03/15   mc       CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
17/11/14   dg       CR758006 IRAT improvements to ease debugging
16/12/14   jk       CR753423:LIF for Idle,voice and data
10/07/14   jk       CR688587:GL1 changes to support LIF feature
02/07/14   mc       CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
29/04/14   jk       CR656825:Call gl1_hw_mcpm_amr_aeq_update( ) when AEQ is ON during SDCCH
13/03/14   mc       CR602368: COEX : Support for Frequency Hopping over large ARFCN lists
19/02/14   jk       CR608290:GERAN RxD ON/OFF adaptive algorithm
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
29/11/13   js       CR574402 : COEX : VBATT : Accurate prediction of N+1 frame Tx power for CS (TCH and SDCCH)
02/10/13   pjr      CR552448 DDR Frequency plan implementation - l1_arbitration modification
04/11/13   mc       CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
24/09/13   jk       CR549227:Shifted SACCH changes for VAMOS2
22/08/13   cja      CR523987 Add FEATURE_GSM_DED_SCELL_MONITOR for extra SCell monitor in voice call
15/08/13   sk        CR524039 GBTA bringup changes
13/03/13   mc        CR 459487 : COEX Support - GERAN arbitrator framework
28/11/12   br       Boundary checks for subchannel Klockwork error
13/03/12   ws        CR 330862 - Change dedicated mode F3 level's and add sacch burst
                     metrics and compact message metrics log packets
30/01/12   cgc      CR332648 removed extern l1_get_current_rr_l1_msg_id(), now in l1i.h
06/10/10   ap       Correction to CR: 235190 - 3GPP: Enhanced control of maximum output power in
            a common BCCH cell
13/09/10   cja      CR251985. If returning to idle from SDCCH do not run frame handlers.
14/06/10   ap       CR: 235190 - 3GPP: Enhanced control of maximum output power in
                      a common BCCH cell
15/02/10   cja      Re-enable GSAC clock when dedicated started if cipher enabled.
20/01/10   cs       CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
05/01/10   cs       CR 218984 GCF 13.3.4.1 failure on high bands TX lev wrongly
                    reported
03/12/08   ws       Added FEATURE_GSM_DYNAMIC_SAIC
12/01/08   gj       CR165617 - mainline FEATURE_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING feature.
19/05/08   og       Fix for SDCCH8 lockup after the handover request is received. CR146010.
27/03/08   ap       CR142716 When on SDCCH update the RXqual full on this callback
02/10/07   og       Clean up of unused code.
31/08/07   agv      CR127468 Fix for SAIC TT handling in SDCCH.
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
08/06/07   nt       CR118485: Removed previous changes in preparation for re-work
23/05/07   nt       CR118485: Stops the SDCCH rx/tx whilst aborting, and alligns
                    on block boundary.
05/03/07   ws        Fixed filtered lint warnings
28/11/06   dv       Lint cleanup. Removed sacch51_rx_in_progress &
                    sacch51_tx_in_progress as redundant
06/04/06   av       Feature SAIC MIMO elements added to the burst metrics.
08/17/05   gfr      Use gl1_msg_init_frame_tick_info to init the ftn structure
07/29/05   gfr      Support for NPL1 DTM interface changes
06/24/05   dp       Fix to exclude BCCH carrier from RXLEV average if its in
                    the hopping list and the PWRC bit is set
03/15/05   dp       Added support for enhanced measurement reporting
03/11/05   sv       Mainlined FEATURE_SCE.
01/24/05   pp       Added transmit burst metrics logging
06/22/04   dp       Lint Clean up
06/21/04   gsc      Lint Clean up
03/18/04   gsc      Lint Clean Up
11/05/03   gfr      Support for quad-band.
08/15/03   dp       Moved decrementing of number of access bursts outside
                    L1_more_abs_to_send to fix issues with incorrcet number
                    of access bursts
07/29/03   dp       Fall through from DED_INIT to DED_RUNNING.  This
                    fixes start time issues with 26.6.5.4.2 at Cetecom
07/16/03   bk       Removed error messages from a default statement in case statements
                    as the default is part of normal operation.
07/11/03   gfr      Lint cleanup.
05/02/03   gw       Merged in power transfer changes from branch.
05/01/03   mk       Added support for AFC/TT.
04/14/03   ATM      Cleaup unused variable: DTX_used
04/10/03   dp       Fixed logging bug in sdcch_rx_callback
04/02/03   gw       Changes to reflect modifications in monscan module.
02/25/03   dp       Changed L1_get_ARFCNs interface for half rate
01/02/03   gw       Added calls to setup up callback to be called at the end
                    of each measurement period.
10/01/02   ATM      Removed compiler warning
09/18/02   dp       Passing in frequency redef parameters to L1_get_ARFCNS
                    to guarantee a seamless transition between the before and
                    after channels in the case of the start time lying in the
                    middle of a block
08/29/02   JC       Obsoleted call to L1_map_txlev_to_power_index(); buried
                    mapping in msg layer to support multi-band operation.
                    Also, Modified usage of gl1_msg_tx_rach().
08/21/02   dp       Fix to not schedule monitors when aborting
08/23/02   plm      Removed static declaration for rpt that was not used to
                    eliminate compiler warning.
                    Added externs for l1_log_burst_metrics() to eliminate compiler warnings.
07/26/02   dp       Support to avg measurements ovr 2 SACCH periods for
                    ranking cells
06/14/02   dp       Changes to meet the 20ms constraint between last burst of
                    last complete block on old channel and first burst on new
                    channel
06/10/02   dp       Support to stop sending measurement reports when commanded
                    by L2
06/10/02   dp       Passing in before and after freq parameters to L1_get_ARFCNS
                    to guarantee a seamless transition between the before and
                    after channels in the case of the start time lying in the
                    middle of a block
06/06/02   mk       Support for aborts on all channels.
05/08/02   JC       Divied up metrics logging s.t. burst and message metrics
                    are logged separately for reliability.  Also changed
                    MSG_ levels so that we rely more on run-time CFA.
04/30/02   JC       Added sdcch_metrics_callback() for enhancing logging.
04/24/02   dp       Fix for TXLEV reporting on uplink SACCH.  We now report the
                    TXLEV of the last burst of the previous reporting period
04/21/02   dp       Fix to start sending out handover access bursts only at
                    the beginning of a SDCCH block
04/02/02   dp       Fixed bug in SACCH metrics callback not being registered
                    on the SDCCH8
03/28/02   JC/dp    Temp put in l1_sci_do_monscan_ded till we consolidate with idle
03/20/02   dp       Added support for power control in steps of 2dB on SDCCH4
                    and SDCCH8.  TXLEV to power index changes.  Enabled SACCH
                    metrics on SDCCH to enable measurement reports on the SDCCH
03/06/02   dp       Support to frequency hop access bursts
02/27/02   gw       Removed #define FEATURE_SCE.
02/20/01   dp       Added support for handovers
02/14/02   gw       Added calls to gl1_msg_reset() to support AGC.
                    Changed DED_SIG_SC to FEATURE_SCE.
02/13/02   gw       Fixed bug that erevented transition out of SDCCH8.  Fixed
                    bug in SDCCH8 that could result in -ve number of monitors
                    being requested.
02/07/02   gw       Improved starting and stopping of SCE.  Removed some unused
                    SCE code.
02/05/02   JC       Added support for AGC.
12/21/01   gw       Made l1_sci_tick_dedi_sdcch run every frame.  This is
                    required to support DRX scheduling.
12/07/01   mk       Updated sdcch8 control state machine to match sdcch4.
11/27/01   mk       Added L1_DED_INIT, L1_DED_RUNNING substates to initialize
                    message callbacks, added L1_DEDICATED_STOP support for
                    sdcch4 signaling. Misc cleanup.
11/9/01    bk       Updated NPL1 FTN block info to conform to the interface change
10/31/01   dp       Incorporated changes due to new CCH decode_rpt format
10/16/01   gw       Used OLD_SCE to featurize references to old style power
                    measurement functions that have been removed.
10/10/01   gw       Added support for message/frame layer Ncell reception.
09/25/01   mk       Non-supported SCE code featurized using OLD_SCE.
08/31/01   JC       Modified ARFCN_T variables to uint16 type and general
                    cleanup. Added QCT function header templates.
05/29/01   JC       Modified Code to Receive and Transmit SDCCH/SACCH.
                    Also, temporarily commented out RACH TX.
05/13/01   JC       Replaced Frame Number variables.
04/24/01   jc       Initial version (Sina emulaton board)

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_log.h"           /* for l1_log_message_metrics() */
#include "l1_sc.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "msg.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "l1_sacch.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "geran_dual_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

typedef struct
{
  /* decoded data goes here sacch and sdcch can share the same rpt since they occur far apart */
  cch_decode_rpt        sdcch_rpt;

/* Support for SACCH metrics - to send up meas rpts on the SDCCH */
  gl1_msg_metrics_rpt   sacch_metrics_rpts[4];
  gl1_msg_rx_rpt        s_rpt;
  boolean               first_time_on_sdcch_4;
  boolean               first_time_on_sdcch_8;
  boolean               first_access_burst_sent_4;
  boolean               first_access_burst_sent_8;

} l1_sdcch_data_t;

static l1_sdcch_data_t l1_sdcch_data[NUM_GERAN_DATA_SPACES];



// metrics and log are NULL for now
//extern uint8    jump_to_sdcch;
extern dedicated_data_T l1_dedicated_data[];

void sdcch_init_data_struct( void );
static void sdcch_tx_callback( gas_id_t gas_id );
static void sdcch_rx_callback(gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void sdcch_metrics_callback(gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id);

#ifdef FEATURE_GSM_COEX_SW_CXM
static void l1_sdcch_get_arfcns(int16 start, uint16 increment, uint16 num_arfcns, ARFCN_T arfcns[], gas_id_t gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

extern void gl1_store_block_rx_power ( gl1_msg_metrics_rpt *rpt[], uint8 bursts_per_block, gas_id_t gas_id );
extern void gl1_update_emr_parameters (gl1_defs_rx_hdr_struct *rpt_hdr, boolean is_sid_fn, boolean compute_bep, gas_id_t gas_id );

extern boolean gl1_include_in_rxlev ( ARFCN_T arfcn, gas_id_t gas_id );

#define MONITORS_PER_REPORTING_PERIOD 100


/*===========================================================================

FUNCTION  L1_sdcch_init_data_struct

DESCRIPTION
  This function initialiase the data structures used by l1_sdcch

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Initialise l1_sdcch_data

===========================================================================*/
void L1_sdcch_init_data_struct( gas_id_t gas_id )
{
  int j;

  l1_sdcch_data[gas_id].s_rpt.data = (gl1_msg_decode_rpt *) &l1_sdcch_data[gas_id].sdcch_rpt;
  for (j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD; j++)
    l1_sdcch_data[gas_id].s_rpt.metrics[j] = &l1_sdcch_data[gas_id].sacch_metrics_rpts[j];
}

/*===========================================================================

FUNCTION  L1_control_SDCCH_4

DESCRIPTION
  This function controls dedicated mode (SDCCH/4)

DEPENDENCIES
  pointer to dedicated mode data
  pointer to channel 1 data
  pointer to channel 2 data

RETURN VALUE
  TRUE  - processing is complete and mode may be changed
  FALSE - this function should be called again on the next frame tick


SIDE EFFECTS
  None

===========================================================================*/
boolean L1_control_SDCCH_4( dedicated_data_T      *ded_data,
                            channel_information_T *chnl_1_data, gas_id_t gas_id
                          )

{
  /* Locals */
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  ded_command_T              command;
  boolean                    idle = FALSE;
  uint8                      mod102;
  uint8                      FNrptperiod;
  uint32                     gap_to_next_activity = 0;
  uint16                     data_len;

  ARFCN_T                    arfcns[4];
  int16                      txlev[4];
  gl1_msg_frame_tick_type    ftn_type;
  boolean                    dl_block_start;
  boolean                    ul_block_start;
  boolean                    access_burst_scheduled = FALSE;

  sys_algo_agc_T             *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
  sys_algo_agc_T             *agc_ptr_divrx;
#endif

  if (!ded_data || !chnl_1_data)
  {
     MSG_GERAN_ERROR_2_G("Rcvd NULL ded_data %d or chnl_1_data %d",
       ded_data, chnl_1_data);
     return FALSE;
  }

  agc_ptr = &ded_data->c0_agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
  agc_ptr_divrx = &ded_data->agc_divrx;
#endif

  switch ( l1_tskisr_blk->sub_sub_state )
  {
  case L1_DED_INIT :
    {
      gl1_msg_tx_cb_type     tx_cb;
      gl1_msg_rx_cb_type     rx_cb;

      /* Configure the callback pointers */
      gl1_msg_reset(gas_id);

      tx_cb.data_cb    = sdcch_tx_callback;
      tx_cb.metrics_cb = NULL;
      tx_cb.log_cb     = NULL;
      tx_cb.abort_cb   = NULL;
      rx_cb.data_cb    = sdcch_rx_callback;
      rx_cb.metrics_cb = sdcch_metrics_callback;
      rx_cb.log_cb     = NULL;
      rx_cb.abort_cb   = NULL;

      gl1_msg_cfg_ul_sdcch(chnl_1_data->TSC, &tx_cb,gas_id );
      gl1_msg_cfg_dl_sdcch(chnl_1_data->TSC, &rx_cb,gas_id );

      gl1_msg_cch_afc_tt_init( gl1_get_serving_cell_arfcn(gas_id), gas_id );

      tx_cb.data_cb    = sacch_tx_callback;
      tx_cb.abort_cb   = NULL;
      rx_cb.data_cb    = sacch_rx_callback;
      rx_cb.abort_cb   = NULL;
      rx_cb.metrics_cb = sacch51_metrics_callback;

      gl1_msg_cfg_ul_sacch(chnl_1_data->TSC, &tx_cb, gas_id);

      gl1_msg_cfg_dl_sacch(chnl_1_data->TSC, &rx_cb, gas_id);

      l1_sdcch_data[gas_id].first_time_on_sdcch_4 = TRUE;
      l1_sdcch_data[gas_id].first_access_burst_sent_4 = FALSE;

      /* Precompute TXLEVS for first reporting period.  Transmit at the
         command power level immediately as soon as we jump onto the new
         channel. ms_power_level_to_use is initially set to ms_power_level_in_use
         ,so L1_precompute_txlev will not step up/step down the power
         The 2dB step algo kicks in only after the next power
         level change commanded by the base station
      */
      L1_precompute_txlev( ded_data->ms_power_level_to_use,
                           ded_data->ms_power_level_in_use,
                           102,
                           chnl_1_data->frequency_list.channel[0],
                           gas_id
                         );

      /* Install callback to trigger serving cell measurement reporting. */
      l1_sci_monscan_set_next_period_ded_cb( L1_send_dedicated_report, gas_id );

      MSG_GERAN_MED_1_G("Sdcch4 init FNmod42432=%d", frame_counters[gas_id].FNmod42432);

      l1_tskisr_blk->sub_sub_state = L1_DED_RUNNING;

/* For Poseidon enable cipher clock for mDSP */
      if (gl1_msg_is_ciphering_enabled(gas_id))
      {
        /* mDSP now requires the clock */
        gl1_hw_cipher_clock_required(TRUE, gas_id);
      }

    }

#ifdef FEATURE_GSM_COEX_SW_CXM
    /* Notify the arbitrator of the channel assignment */
    garb_intf_notify_sdcch_assignment( chnl_1_data->frequency_list, FALSE, chnl_1_data->subchannel, l1_sdcch_get_arfcns, gas_id);
#endif

    break;

  case L1_DED_RUNNING:
    /* allow for a 1 frame shift in the tables */

    /* Chatlani, the tables provide the commands two frames in
    ** advance
    */
    if ( frame_counters[gas_id].FNmod102 == 0 )
    {
      mod102 = 101;
    }
    else
    {
      mod102 = frame_counters[gas_id].FNmod102 - 1;
    }
    //boundary check for subchannel to fix klockwork error
    if (chnl_1_data->subchannel > 3)
    {
      chnl_1_data->subchannel = 3;
    }

    /*check if any commmand on this frame */
    command = ded_sdcch_4_table[mod102][chnl_1_data->subchannel][0];

    /*indicate start of new measurement period to SC engine */
    /*
        Start 100(should be 102 but set to 100 now since the power meas
        handler cannot handle back to back measurement sequences - needs
        to be changed) measurements at the end of each reqporting period
    */

    /* If its the very first time on the SDCCH, schedule only as many monitors as
       upto the next reporting period
    */

    if ( (l1_tskisr_blk->main_command != L1_DEDICATED_STOP) )
    {
       if (l1_sdcch_data[gas_id].first_time_on_sdcch_4)
       {
          uint8 num;

          l1_sdcch_data[gas_id].first_time_on_sdcch_4 = FALSE;
          /*
           Number of monitors is currently reduced by 2 due to the fact that the power
           measure handler needs 2 frames between back to back measurement sequences
           - To fix
          */
          if (mod102 < L1_SC_SDCCH4_FRAME_REPORTING )
          {
             num =  L1_SC_SDCCH4_FRAME_REPORTING - mod102 - 1 - 2;
          }
          else
          {
             num =  102 - (mod102 - L1_SC_SDCCH4_FRAME_REPORTING) - 2;
          }

          if (num > 0)
          {
             l1_sci_monscan_do_ded( num, 1, FALSE, gas_id );
          }
       }
       else
       {
          if ( mod102 == L1_SC_SDCCH4_FRAME_REPORTING )
          {
             l1_sci_monscan_next_period_ded(gas_id);
             l1_sci_monscan_do_ded(MONITORS_PER_REPORTING_PERIOD,1, FALSE, gas_id);
          }
       }
    }

    /* Ordered TA and TXLEV is applied only at the start of the next reporting period.
       We precompute TXLEV for the entire reporting period and then rtrieve it later
       for each transmitted block.  We do this in reporting_period - 4 because the
       precomputation takes some time and its better to do it in oneof the idle frames
       to prevent frame tick overruns
    */
    if ( frame_counters[gas_id].FNmod102 == L1_SC_SDCCH4_FRAME_REPORTING - 2 )
    {
        L1_get_txlev ( 98, 1, txlev, gas_id);

        /* Update ms_power_level_in_use with the last block in the
           previous reporting period */
        ded_data->ms_power_level_in_use = (byte)txlev[3];
        ded_data->ms_power_level_to_report = (byte)txlev[3];

        /*Update the power level and TA to use with that commanded by the base station */
        ded_data->ms_power_level_to_use =  ded_data->ordered_ms_power_level;
        ded_data->timing_advance_to_use =  ded_data->ordered_timing_advance;

        L1_precompute_txlev( ded_data->ordered_ms_power_level,
                             ded_data->ms_power_level_in_use,
                             102,
                             chnl_1_data->frequency_list.channel[0],
                             gas_id
                           );

        ded_data->timing_advance_in_use =
          L1_change_ta( ded_data->timing_advance_to_use,
                        ded_data->timing_advance_in_use,gas_id );

    }

    /* FNrptperiod is the FN relative to the start of the reporting period
       This is used to index into the precomputed TXLEVs since they are computed
       with reference to the start of the reporting period
    */
    FNrptperiod = SUB_MOD( frame_counters[gas_id].FNmod102, L1_SC_SDCCH4_FRAME_REPORTING, 102);

    L1_set_reporting_period(FNrptperiod, 102, gas_id);

    if ((l1_tskisr_blk->main_command != L1_DEDICATED_STOP) &&
        if_starting_time_elapsed_or_valid(ded_data, gas_id))
    {


      switch ( command )
      {
      /* ----------------------------------------------------------------
      **              Schedule RX Operations
      ** ----------------------------------------------------------------
      */
      case L1_DED_READ_SDCCH:
        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        gl1_msg_rx_sdcch(arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                         agc_ptr_divrx,
#endif
                         &l1_sdcch_data[gas_id].s_rpt,
                         gas_id);
        break;

      case L1_DED_READ_SACCH:

        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        gl1_msg_rx_sacch51(arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                           agc_ptr_divrx,
#endif
			               &l1_sdcch_data[gas_id].s_rpt,
			               gas_id);
        gl1_msg_cch_tt_adjust(gas_id);
        gl1_msg_cch_afc_adjust(gas_id);
        break;


        /* ----------------------------------------------------------------
        **              Schedule TX Operations
        ** ----------------------------------------------------------------
        */
      case L1_DED_SEND_SDCCH:

        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        /* Convert txlevs to power index before passing to message layer */
        L1_get_txlev ( FNrptperiod, 1, txlev, gas_id );
        ded_data->ms_power_level_in_use = (byte)txlev[3];

        /*
         If the flag is set for testing, don't tx anything to avoid bad syncs.
         We're on the SDCCH when the base station is not expecting us to be
        */
        if (L1isr_get_jump_to_sdcch(gas_id))
        {
        }
        /* Schedule access burst if we're doing a handover */
        else if ( L1_more_abs_to_send(ded_data, DCCH, gas_id) )
        {
           uint8 i = 0;

           /*
               Transmit handover access bursts. Single burst, so just
               pass in the first ARFCN in the block
           */

           /* Decrement # of handover acccess bursts to send */
           ded_data->no_of_ab_to_send--;

           gl1_msg_tx_rach
                        (  arfcns[0],
                           l1_dedicated_data_ptr->ms_power_level_to_use,
                           0,
                           l1_dedicated_data_ptr->access_burst_content,
                           gas_id
                        );


           access_burst_scheduled = TRUE;
           /* Indicates that the access burst was transmitted on the first
              burst of the SDCCH block.  This flag is to prevent the first
              access burst from being transmitted  from the middle of a
              SDCCH block
           */
           l1_sdcch_data[gas_id].first_access_burst_sent_4 = TRUE;

           for ( i = 0 ; i < 4; i++ )
           {
              txlev[i] =  l1_dedicated_data_ptr->ms_power_level_to_use;
           }

           /* Log tx burst metrics */
           l1_log_transmit_burst_metrics
           ( GL1_MSG_RACH,
             1, 1, 1,
             arfcns,
             txlev,
             chnl_1_data->TSC,
             0, gas_id );
        }
        else
        {
          uint8  tx_data[N201_MAX];

          (void) L1_get_CCH_message ( TRUE, SDCCH, ded_data, tx_data, &data_len, gas_id);

          gl1_msg_tx_sdcch ( arfcns, txlev, tx_data, gas_id);

          /* Log tx burst metrics */
          l1_log_transmit_burst_metrics
          ( GL1_MSG_SDCCH,
            1, 1, 4,
            arfcns,
            txlev,
            chnl_1_data->TSC,
            ded_data->timing_advance_in_use, gas_id );
        }
        break;

      case L1_DED_SEND_ACCESS_BURST:

         /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
         L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        /*
           Don't transmit if the first burst was not sent out on the first
           SDCCH burst
        */
         if ( l1_sdcch_data[gas_id].first_access_burst_sent_4)
         {
            if ( L1_more_abs_to_send(ded_data, DCCH, gas_id) )
            {
               uint8 i = 0;

               /* Decrement # of handover acccess bursts to send */
               ded_data->no_of_ab_to_send--;

                /*
                    Transmit handover access bursts. Single burst, so just
                    pass in the first ARFCN in the block
                */
                gl1_msg_tx_rach
                        (  arfcns[0],
                           l1_dedicated_data_ptr->ms_power_level_to_use,
                           0,
                           l1_dedicated_data_ptr->access_burst_content,
                           gas_id
                        );
                access_burst_scheduled = TRUE;

                for ( i = 0 ; i < 4; i++ )
                {
                   txlev[i] =  l1_dedicated_data_ptr->ms_power_level_to_use;
                }

                /* Log tx burst metrics */
                l1_log_transmit_burst_metrics
                ( GL1_MSG_RACH,
                  1, 1, 1,
                  arfcns,
                  txlev,
                  chnl_1_data->TSC,
                  0 , gas_id);
            }
         }
         else
         {
            l1_sdcch_data[gas_id].first_access_burst_sent_4 = FALSE;
         }
         break;

      case L1_DED_SEND_SACCH:
        {
          uint8  tx_data[N201_MAX];

          /*
           If the flag is set for testing, don't tx anything to avoid bad syncs.
           We're on the SDCCH when the base station is not expecting us to be
          */
          if (!L1isr_get_jump_to_sdcch(gas_id))
          {
            /* Get the next block of ARFCN's and signal strengths */
            /* Passing in before and after freq parameters to L1_get_ARFCNS
               to guarantee a seamless transition between the before and
               after channels in the case of the start time lying in the
               middle of a block.  Not very elegant but the least complicated
               solution for now
            */

            L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

            /* Convert txlevs to power index before passing to message layer */
            L1_get_txlev ( FNrptperiod, 1, txlev, gas_id );
            ded_data->ms_power_level_in_use = (byte)txlev[3];

            (void) L1_get_CCH_message( TRUE, SACCH, ded_data, tx_data, &data_len, gas_id);

            /* Don't send meas rpt if L2 has asked us to stop */
            if ( !ded_data->stop_meas_rpt )
            {
                gl1_msg_tx_sacch51 ( arfcns, txlev, tx_data,gas_id);

                /* Log tx burst metrics */
                l1_log_transmit_burst_metrics
                ( GL1_MSG_SACCH,
                  1, 1, 4,
                  arfcns,
                  txlev,
                  chnl_1_data->TSC,
                  ded_data->timing_advance_in_use, gas_id );
            }
          }
          break;
        }

      default:
       /* This is encountered during idle frames as part of normal operations */
       break;
      }

    }
    else
    {
        /* If we're stopping, store the last tx power, in case we
           need to reconnect as in handovers
        */
        L1_get_txlev ( FNrptperiod, 1, txlev, gas_id );
        ded_data->ms_power_level_in_use = (byte)txlev[0];
    }

    /********************************************
    *check gap to next block for SC engine      *
    *********************************************/
    if ( access_burst_scheduled || l1_tskisr_blk->main_command == L1_DEDICATED_STOP)
    {
      gap_to_next_activity = 0;
    }

    else
    {
      gap_to_next_activity = ded_sdcch_4_table[mod102][chnl_1_data->subchannel][1];
    }

    /*tell SC engine available gap before next Rx or Tx block*/
    l1_sci_tick_dedi_sdcch(gap_to_next_activity, (uint8)(l1_tskisr_blk->main_command == L1_DEDICATED_STOP), gas_id);

    /*indicate start of new measurement period to SC engine */
    /*
        Start 100(should be 102 but set to 100 now since the power meas
        handler cannot handle back to back measurement sequences - needs
        to be changed) measurements at the end of each reqporting period
    */

    /* The frame before downlink blocks are 1,5,11,15,21,25,31,35,41,45 */
    if ( (frame_counters[gas_id].FNmod51%10 == 1) || (frame_counters[gas_id].FNmod51%10 == 5) )
    {
      dl_block_start = TRUE;
    }
    else
    {
      dl_block_start = FALSE;
    }

    /* The frame before uplink blocks are 50,5,9,36,40,46 */
    switch ( frame_counters[gas_id].FNmod51 )
    {
    case 50:
    case 5:
    case 9:
    case 36:
    case 40:
    case 46:
      ul_block_start = TRUE;
      break;
    default:
      ul_block_start = FALSE;
    }

    gl1_msg_init_frame_tick_info(&ftn_type);
    ftn_type.next_frame_is_dl_block_start             = dl_block_start;
    ftn_type.next_frame_is_ul_block_start             = ul_block_start || access_burst_scheduled;
    ftn_type.next_frame_is_acq_ncell_block_start      = gl1_is_ncell_block_start(gas_id) && !access_burst_scheduled;
    ftn_type.next_frame_is_pwr_meas_block_start       = gl1_is_pwr_meas_block_start(gas_id);


    idle = FALSE;

    if ( l1_tskisr_blk->main_command == L1_DEDICATED_STOP )
    {
      /* Poll SDCCH abort.  This aborts return TRUE only after
       the tx block has been completed.  This is required to
       satisfy the 20 ms constraint - i.e the time interval between the
       last burst on the old channel and the first burst on the new channel
       should be <= 20ms.  The 20ms window applies to the last complete
       tx block which the network sees, so an incomplete block increases
       this window causing us to fail this 11.10 test case
      */
       /* Abort Tx TCH */
      /* Abort SDCCH Tx */
      idle = gl1_msg_abort_tx_sdcch(gas_id);
      /* Abort SACCH51 Tx */
      gl1_msg_abort_tx_sacch51(gas_id);
      /* Abort SDCCH Rx */
      gl1_msg_abort_rx_sdcch(gas_id);
      /* Abort SACCH51 Rx */
      gl1_msg_abort_rx_sacch51(gas_id);

      /*
      Clean up surround cell rx and power measurements before quitting
      */
      idle = (idle && !l1_sci_is_op_in_progress(gas_id) );

      /*Check if GL1 is stuck in this state because of unfinished IRAT activity*/
      gl1_check_if_irat_state_stuck(gas_id);

      MSG_GERAN_MED_2_G("Sdcch4 abort, idle %d rr_l1_msg %d", idle, l1_get_current_rr_l1_msg_id(gas_id));

    }

    /* Run frame tick handlers unless returning to idle and re-starting dedicated in same frame */
    if ( (idle == FALSE) ||
          ( (l1_get_current_rr_l1_msg_id(gas_id) != MPH_RECONNECT_CHANNEL_REQ)
          && (l1_get_current_rr_l1_msg_id(gas_id) != MPH_CHANNEL_ASSIGN_REQ)
          && (l1_get_current_rr_l1_msg_id(gas_id) != MPH_CHANNEL_MODE_MODIFY_REQ)) )
    {
       gl1_msg_frame_tick_notification ( &ftn_type,gas_id );
    }

    /* Clear out block starts */
    gl1_set_ncell_block_start(FALSE,gas_id);
    gl1_set_pwr_meas_block_start(FALSE,gas_id);


    break;

  default:
    break;

  } /* end of switch */

  return(idle);
}


/*===========================================================================

FUNCTION  L1_control_SDCCH_8

DESCRIPTION
  This function controls dedicated mode (SDCCH/8)

DEPENDENCIES
  pointer to dedicated mode data
  pointer to channel 1 data
  pointer to channel 2 data

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_control_SDCCH_8( dedicated_data_T      *ded_data,
                            channel_information_T *chnl_1_data, gas_id_t gas_id
                          )

{
  /* Locals */
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  ded_command_T   command;
  int             lookup_FN;
  uint8           fn_mod_102;
  uint8           FNrptperiod;
  uint16          data_len;
  boolean         idle=FALSE;
  uint32          gap_to_next_activity = 0;
  ARFCN_T                    arfcns[4];
  int16                      txlev[4];
  gl1_msg_frame_tick_type    ftn_type;
  boolean                    dl_block_start;
  boolean                    ul_block_start;
  boolean                    access_burst_scheduled = FALSE;
  sys_algo_agc_T             *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
  sys_algo_agc_T             *agc_ptr_divrx;
#endif

  if (!ded_data || !chnl_1_data)
  {
     MSG_GERAN_ERROR_2_G("Rcvd NULL ded_data %d or chnl_1_data %d",
       ded_data, chnl_1_data);
     return FALSE;
  }

  agc_ptr = &ded_data->c0_agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
  agc_ptr_divrx = &ded_data->agc_divrx;
#endif

  switch ( l1_tskisr_blk->sub_sub_state )
  {
  case L1_DED_INIT :
    {
      gl1_msg_tx_cb_type     tx_cb;
      gl1_msg_rx_cb_type     rx_cb;

      /* Configure the callback pointers */
      gl1_msg_reset(gas_id);

      tx_cb.data_cb    = sdcch_tx_callback;
      tx_cb.metrics_cb = NULL;
      tx_cb.log_cb     = NULL;
      tx_cb.abort_cb   = NULL;
      rx_cb.data_cb    = sdcch_rx_callback;
      rx_cb.metrics_cb = sdcch_metrics_callback;
      rx_cb.log_cb     = NULL;
      rx_cb.abort_cb   = NULL;

      gl1_msg_cfg_ul_sdcch(chnl_1_data->TSC, &tx_cb,gas_id );
      gl1_msg_cfg_dl_sdcch(chnl_1_data->TSC, &rx_cb,gas_id );

      gl1_msg_cch_afc_tt_init( gl1_get_serving_cell_arfcn(gas_id), gas_id );

      tx_cb.data_cb    = sacch_tx_callback;
      tx_cb.abort_cb   = NULL;
      rx_cb.data_cb    = sacch_rx_callback;
      rx_cb.abort_cb   = NULL;
      rx_cb.metrics_cb = sacch51_metrics_callback;

      gl1_msg_cfg_ul_sacch(chnl_1_data->TSC, &tx_cb, gas_id);

      gl1_msg_cfg_dl_sacch(chnl_1_data->TSC, &rx_cb, gas_id);

      l1_sdcch_data[gas_id].first_time_on_sdcch_8 = TRUE;
      l1_sdcch_data[gas_id].first_access_burst_sent_8 = FALSE;

    /* Precompute TXLEVS for first reporting period.  Transmit at the
         command power level immediately as soon as we jump onto the new
         channel. ms_power_level_to_use is initially set to ms_power_level_in_use
         ,so L1_precompute_txlev will not step up/step down the power
         The 2dB step algo kicks in only after the next power
         level change commanded by the base station
      */
      L1_precompute_txlev( ded_data->ms_power_level_to_use,
                           ded_data->ms_power_level_in_use,
                           102,
                           chnl_1_data->frequency_list.channel[0],
                           gas_id
                         );

      /* Install callback to trigger serving cell measurement reporting. */
      l1_sci_monscan_set_next_period_ded_cb( L1_send_dedicated_report, gas_id );

      MSG_GERAN_MED_1_G("Sdcch8 init FNmod42432=%d", frame_counters[gas_id].FNmod42432);

      l1_tskisr_blk->sub_sub_state = L1_DED_RUNNING;

/* For Poseidon enable cipher clock for mDSP */
      if (gl1_msg_is_ciphering_enabled(gas_id))
      {
        /* mDSP now requires the clock */
        gl1_hw_cipher_clock_required(TRUE, gas_id);
      }

    }

#ifdef FEATURE_GSM_COEX_SW_CXM
   /* Notify the arbitrator of the channel assignment */
   garb_intf_notify_sdcch_assignment( chnl_1_data->frequency_list, TRUE, chnl_1_data->subchannel, l1_sdcch_get_arfcns, gas_id);
#endif

    break;


  case L1_DED_RUNNING:

    /* controlled by the mod51 frame number */
    /* The table has an entry for sub-channels 3 and 7.
       subchannels 0, 1, and 2, are similar to subchannel 3,
       subchannels 4, 5, and 6  are similar to subchannel 7.

       To get the frame number corresponding to subchannels 3 or 7
       add 12 to the FN for subchannels 0 and 4,
       add  8 to the FN for subchannels 1 and 5,
       add  4 to the FN for subchannels 2 and 6. */

    /* Also allow that the FNs have been advanced by 1 */

    if ( frame_counters[gas_id].FNmod102 == 0 )
    {
      fn_mod_102 = 101;
    }

    else
    {
      fn_mod_102 = frame_counters[gas_id].FNmod102 - 1;
    }

    lookup_FN = (fn_mod_102 + (((7 - chnl_1_data->subchannel) % 4) << 2)) % 102;

   //boundary check for subchannel to fix klockwork error
    if (chnl_1_data->subchannel > 7)
  {
      chnl_1_data->subchannel = 7;
  }

    command = ded_sdcch_8_table[lookup_FN][chnl_1_data->subchannel >> 2][0];

    if ( (l1_tskisr_blk->main_command != L1_DEDICATED_STOP) )
    {
       if (l1_sdcch_data[gas_id].first_time_on_sdcch_8)
       {
          uint8 num;

          l1_sdcch_data[gas_id].first_time_on_sdcch_8 = FALSE;

          /*
             Number of monitors is currently reduced by 2 sue to the fact that the power
             measure handler needs 2 frames between back to back measurement sequences
           - To fix
           */
          if (fn_mod_102 < L1_SC_SDCCH8_FRAME_REPORTING - 2 )
          {
             num = L1_SC_SDCCH8_FRAME_REPORTING - 2 - fn_mod_102 - 1;
          }
          else
          {
             num = 102 - (fn_mod_102 - L1_SC_SDCCH8_FRAME_REPORTING) - 4;
          }

          if (num > 0)
          {
             l1_sci_monscan_do_ded( (uint8)num, 1, FALSE, gas_id );
          }
          else
          {
             MSG_GERAN_ERROR_1_G("SDCCH8: Negative num measurements %d",num);
          }
       }
       else
       {
          if ( fn_mod_102 == L1_SC_SDCCH8_FRAME_REPORTING )
          {
             l1_sci_monscan_next_period_ded(gas_id);
             l1_sci_monscan_do_ded(100,1, FALSE, gas_id);
          }
       }
    }

    /* Ordered TA and TXLEV is applied only at the start of the next reporting period.
       We precompute TXLEV for the entire reporting period and then rtrieve it later
       for each transmitted block.  We do this in reporting_period - 4 because the
       precomputation takes some time and its better to do it in oneof the idle frames
       to prevent frame tick overruns
    */
    if ( frame_counters[gas_id].FNmod102 == L1_SC_SDCCH8_FRAME_REPORTING - 2 )
    {
        /* Update ms_power_level_in_use with the last block in the
           previous reporting period */
        L1_get_txlev ( 98, 1, txlev, gas_id );
        ded_data->ms_power_level_in_use = (byte)txlev[3];
        ded_data->ms_power_level_to_report = (byte)txlev[3];

        /*Update the power level and TA to use with that commanded by the base station */
        ded_data->ms_power_level_to_use =  ded_data->ordered_ms_power_level;
        ded_data->timing_advance_to_use =  ded_data->ordered_timing_advance;

        /*

        */
        L1_precompute_txlev( ded_data->ordered_ms_power_level,
                             ded_data->ms_power_level_in_use,
                             102,
                             chnl_1_data->frequency_list.channel[0],
                             gas_id
                           );

        ded_data->timing_advance_in_use =
          L1_change_ta( ded_data->timing_advance_to_use,
                        ded_data->timing_advance_in_use,gas_id );

    }

    /* FNrptperiod is the FN relative to the start of the reporting period
       This is used to index into the precomputed TXLEVs since they are computed
       with reference to the start of the reporting period
    */
    FNrptperiod = SUB_MOD( frame_counters[gas_id].FNmod102, L1_SC_SDCCH8_FRAME_REPORTING, 102);

    L1_set_reporting_period(FNrptperiod, 102, gas_id);

    if ((l1_tskisr_blk->main_command != L1_DEDICATED_STOP) &&
        if_starting_time_elapsed_or_valid(ded_data, gas_id))
    {

      switch ( command )
      {
      /* ----------------------------------------------------------------
      **              Schedule RX Operations
      ** ----------------------------------------------------------------
      */
      case L1_DED_READ_SDCCH:
        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        gl1_msg_rx_sdcch(arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                         agc_ptr_divrx,
#endif
			             &l1_sdcch_data[gas_id].s_rpt,
			             gas_id);
        break;

      case L1_DED_READ_SACCH:

        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        gl1_msg_rx_sacch51(arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                           agc_ptr_divrx,
#endif
			               &l1_sdcch_data[gas_id].s_rpt,
			               gas_id);
        gl1_msg_cch_tt_adjust(gas_id);
        gl1_msg_cch_afc_adjust(gas_id);
        break;


        /* ----------------------------------------------------------------
        **              Schedule TX Operations
        ** ----------------------------------------------------------------
        */
      case L1_DED_SEND_SDCCH:

        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        /* Convert txlev to power index before passing to msg layer */
        L1_get_txlev ( FNrptperiod, 1, txlev, gas_id );
        ded_data->ms_power_level_in_use = (byte)txlev[3];

        /*
         If the flag is set for testing, don't tx anything to avoid bad syncs.
         We're on the SDCCH when the base station is not expecting us to be
        */
        if (L1isr_get_jump_to_sdcch(gas_id))
        {
        }
        /* Schedule access burst if we're doing a handover */
        else if ( L1_more_abs_to_send(ded_data, DCCH, gas_id) )
        {
           uint8 i = 0;
           /* Decrement # of handover acccess bursts to send */
           ded_data->no_of_ab_to_send--;

           /*
               Transmit handover access bursts. Single burst, so just
               pass in the first ARFCN in the block
           */
           gl1_msg_tx_rach
                        (  arfcns[0],
                           l1_dedicated_data_ptr->ms_power_level_to_use,
                           0,
                           l1_dedicated_data_ptr->access_burst_content,
                           gas_id
                        );


           access_burst_scheduled = TRUE;

           /* Indicates that the access burst was transmitted on the first
              burst of the SDCCH block.  This flag is to prevent the first
              access burst from being transmitted  from the middle of a
              SDCCH block
           */
           l1_sdcch_data[gas_id].first_access_burst_sent_8 = TRUE;

           for ( i = 0 ; i < 4; i++ )
           {
              txlev[i] =  l1_dedicated_data_ptr->ms_power_level_to_use;
           }

           /* Log tx burst metrics */
           l1_log_transmit_burst_metrics
           ( GL1_MSG_RACH,
             1, 1, 1,
             arfcns,
             txlev,
             chnl_1_data->TSC,
             0 , gas_id);
        }
        else
        {
          uint8  tx_data[N201_MAX];

          (void) L1_get_CCH_message ( TRUE, SDCCH, ded_data, tx_data, &data_len, gas_id);

          gl1_msg_tx_sdcch ( arfcns, txlev, tx_data, gas_id);

          /* Log tx burst metrics */
          l1_log_transmit_burst_metrics
          ( GL1_MSG_SDCCH,
            1, 1, 4,
            arfcns,
            txlev,
            chnl_1_data->TSC,
            ded_data->timing_advance_in_use, gas_id );
        }
        break;

      case L1_DED_SEND_ACCESS_BURST:
        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        /* Handover access bursts */
        /*
           Don't transmit if the first burst was not sent out on the first
           SDCCH burst
        */
        if ( l1_sdcch_data[gas_id].first_access_burst_sent_8)
        {
           if ( L1_more_abs_to_send(ded_data, DCCH, gas_id) )
           {
               uint8 i = 0;
               /* Decrement # of handover acccess bursts to send */
               ded_data->no_of_ab_to_send--;

               /*
                   Transmit handover access bursts. Single burst, so just
                   pass in the first ARFCN in the block
               */
               gl1_msg_tx_rach
                        (  arfcns[0],
                           l1_dedicated_data_ptr->ms_power_level_to_use,
                           0,
                           l1_dedicated_data_ptr->access_burst_content,
                           gas_id
                        );


               access_burst_scheduled = TRUE;

               for ( i = 0 ; i < 4; i++ )
               {
                  txlev[i] =  l1_dedicated_data_ptr->ms_power_level_to_use;
               }

               /* Log tx burst metrics */
               l1_log_transmit_burst_metrics
               ( GL1_MSG_RACH,
                 1, 1, 1,
                 arfcns,
                 txlev,
                 chnl_1_data->TSC,
                 0, gas_id );
           }
        }
        else
        {
            l1_sdcch_data[gas_id].first_access_burst_sent_8 = FALSE;
        }
        break;

      case L1_DED_READ_SACCH | L1_DED_SEND_ACCESS_BURST:

        /* Get the next block of ARFCN's and signal strengths */
        /* Passing in before and after freq parameters to L1_get_ARFCNS
           to guarantee a seamless transition between the before and
           after channels in the case of the start time lying in the
           middle of a block.  Not very elegant but the least complicated
           solution for now
        */
        L1_get_ARFCNS ( 1, 1, 4, arfcns,
                        &ded_data->channel_specification.channel_info_1_before,
                        &ded_data->channel_specification.channel_info_1_after,
                        &ded_data->new_channel_info, gas_id);

        gl1_msg_rx_sacch51(arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                           agc_ptr_divrx,
#endif
                           &l1_sdcch_data[gas_id].s_rpt,
                           gas_id);
        gl1_msg_cch_tt_adjust(gas_id);
        gl1_msg_cch_afc_adjust(gas_id);

        /* Handover access bursts */
        /*
           Don't transmit if the first burst was not sent out on the first
           SDCCH burst
        */
        if ( l1_sdcch_data[gas_id].first_access_burst_sent_8)
        {
           if ( L1_more_abs_to_send(ded_data, DCCH, gas_id) )
           {
               uint8 i = 0;
               /* Decrement # of handover acccess bursts to send */
               ded_data->no_of_ab_to_send--;

               /*
                   Transmit handover access bursts. Single burst, so just
                   pass in the first ARFCN in the block
               */
               gl1_msg_tx_rach
                       (  arfcns[0],
                          l1_dedicated_data_ptr->ms_power_level_to_use,
                          0,
                          l1_dedicated_data_ptr->access_burst_content,
                          gas_id
                       );

               access_burst_scheduled = TRUE;

               for ( i = 0 ; i < 4; i++ )
               {
                  txlev[i] =  l1_dedicated_data_ptr->ms_power_level_to_use;
               }

               /* Log tx burst metrics */
               l1_log_transmit_burst_metrics
               ( GL1_MSG_RACH,
                 1, 1, 1,
                 arfcns,
                 txlev,
                 chnl_1_data->TSC,
                 0, gas_id );
           }
        }
        else
        {
            l1_sdcch_data[gas_id].first_access_burst_sent_8 = FALSE;
        }
        break;


      case L1_DED_SEND_SACCH:
        {
          uint8  tx_data[N201_MAX];

          if ( !L1isr_get_jump_to_sdcch(gas_id) )
          {
            /* Get the next block of ARFCN's and signal strengths */
            /* Passing in before and after freq parameters to L1_get_ARFCNS
               to guarantee a seamless transition between the before and
               after channels in the case of the start time lying in the
               middle of a block.  Not very elegant but the least complicated
               solution for now
            */
            L1_get_ARFCNS ( 1, 1, 4, arfcns,
                          &ded_data->channel_specification.channel_info_1_before,
                          &ded_data->channel_specification.channel_info_1_after,
                          &ded_data->new_channel_info, gas_id);

            /* Convert txlev to power index before passing to msg layer */
            L1_get_txlev ( FNrptperiod, 1, txlev, gas_id );
            ded_data->ms_power_level_in_use = (byte)txlev[3];

            (void) L1_get_CCH_message ( TRUE, SACCH, ded_data, tx_data, &data_len, gas_id);

            /* Don't send meas rpt if L2 has asked us to stop */
            if ( !ded_data->stop_meas_rpt )
            {
              gl1_msg_tx_sacch51 ( arfcns, txlev, tx_data,gas_id);

              /* Log tx burst metrics */
              l1_log_transmit_burst_metrics
              ( GL1_MSG_SACCH,
                1, 1, 4,
                arfcns,
                txlev,
                chnl_1_data->TSC,
                ded_data->timing_advance_in_use, gas_id );
            }
          }
        }
        break;

      default:
        /* This is encountered during idle frames as part of normal operations */
        break;
      }

    }
    else
    {
        /* If we're stopping, store the last tx power, in case we
           need to reconnect as in handovers
        */
        L1_get_txlev ( FNrptperiod, 1, txlev, gas_id );
        ded_data->ms_power_level_in_use = (byte)txlev[0];
    }

    /********************************************
    *check gap to next block for SC engine      *
    *********************************************/
    if (access_burst_scheduled || l1_tskisr_blk->main_command == L1_DEDICATED_STOP)
    {
      gap_to_next_activity = 0;
    }

    else
    {
      gap_to_next_activity = ded_sdcch_8_table[lookup_FN][chnl_1_data->subchannel >> 2][1];
    }

    /*tell SC engine available gap before next Rx or Tx block*/
    l1_sci_tick_dedi_sdcch(gap_to_next_activity, (uint8)(l1_tskisr_blk->main_command == L1_DEDICATED_STOP), gas_id);

    /* The frame before downlink blocks are 3,7,11,15,19,23,27,31,35,39,43,47, 50 */
    if ( (frame_counters[gas_id].FNmod51%4 == 3) || (frame_counters[gas_id].FNmod51 == 50) )
    {
      dl_block_start = TRUE;
    }
    else
    {
      dl_block_start = FALSE;
    }

    /* The frame before uplink blocks are 50,3,7,14,18,22,26,30,34,38,42,46,50 */
    ul_block_start = FALSE;
    if ( (frame_counters[gas_id].FNmod51 == 3) || (frame_counters[gas_id].FNmod51 == 7) )
    {
      ul_block_start = TRUE;
    }
    else if ( frame_counters[gas_id].FNmod51 > 13 )
    {
      uint8    mod51;

      mod51 = frame_counters[gas_id].FNmod51-14;
      if ( mod51%4 == 0 )
      {
        ul_block_start = TRUE;
      }

    }

    gl1_msg_init_frame_tick_info(&ftn_type);
    ftn_type.next_frame_is_dl_block_start             = dl_block_start;
    ftn_type.next_frame_is_ul_block_start             = ul_block_start || access_burst_scheduled;
    ftn_type.next_frame_is_acq_ncell_block_start      = gl1_is_ncell_block_start(gas_id);
    ftn_type.next_frame_is_pwr_meas_block_start       = gl1_is_pwr_meas_block_start(gas_id);


    idle = FALSE;

    if ( l1_tskisr_blk->main_command == L1_DEDICATED_STOP )
    {
      boolean  sci_in_progress;

      /* Poll SDCCH abort.  This aborts return TRUE only after
       the tx block has been completed.  This is required to
       satisfy the 20 ms constraint - i.e the time interval between the
       last burst on the old channel and the first burst on the new channel
       should be <= 20ms.  The 20ms window applies to the last complete
       tx block which the network sees, so an incomplete block increases
       this window causing us to fail this 11.10 test case
      */
      /* Abort SDCCH Tx */
      idle = gl1_msg_abort_tx_sdcch(gas_id);
      /* Abort SACCH51 Tx */
      gl1_msg_abort_tx_sacch51(gas_id);
      /* Abort SDCCH Rx */
      gl1_msg_abort_rx_sdcch(gas_id);
      /* Abort SACCH51 Rx */
      gl1_msg_abort_rx_sacch51(gas_id);

      sci_in_progress = l1_sci_is_op_in_progress(gas_id);

      /*Check if GL1 is stuck in this state because of unfinished IRAT activity*/
      gl1_check_if_irat_state_stuck(gas_id);

      MSG_GERAN_MED_3_G("Sdcch8 abort idle %d sci %d rr_l1_msg %d", idle, sci_in_progress, l1_get_current_rr_l1_msg_id(gas_id));

      /*
      Clean up surround cell rx and power measurements before quitting
      */
      idle = (idle && !sci_in_progress);

    }

    /* Run frame tick handlers unless returning to idle and re-starting dedicated in same frame */
    if ( (idle == FALSE) ||
          ( (l1_get_current_rr_l1_msg_id(gas_id) != MPH_RECONNECT_CHANNEL_REQ)
          && (l1_get_current_rr_l1_msg_id(gas_id) != MPH_CHANNEL_ASSIGN_REQ)
          && (l1_get_current_rr_l1_msg_id(gas_id) != MPH_CHANNEL_MODE_MODIFY_REQ)) )
    {
       gl1_msg_frame_tick_notification ( &ftn_type,gas_id );
    }

    /* Clear out block starts */
    gl1_set_ncell_block_start(FALSE,gas_id);
    gl1_set_pwr_meas_block_start(FALSE,gas_id);


    break;

  default:
    break;

  } /* end of switch */


    return(idle);
  }


/*===========================================================================

FUNCTION  L1_SEND_DEDICATED_REPORT

DESCRIPTION
  This function sends  reports to L3 at the end of the Dedicated
  channel reporting period

DEPENDENCIES
  DTX_used, Dedicated data pointer

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
  void L1_send_dedicated_report (gas_id_t gas_id)
  {
#ifdef COMMENT
    #error code not present
#endif
    l1_set_serv_pwr_meas_permitted(FALSE, gas_id);

    L1isr_send_L1_REP_PERIOD_IND(gas_id);
  }




/*===========================================================================

FUNCTION sdcch_tx_callback

DESCRIPTION
  This function is called in the first frame of a SDCCH block (ie 4 frames).
  Currently no activity is needed here.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static  void sdcch_tx_callback( gas_id_t gas_id )
{
    /* nothing to do */
}




/*===========================================================================

FUNCTION sdcch_rx_callback

DESCRIPTION
  This function is called when the data from the SDCCH has been decoded. It
  sends a message to L2 indicating if message is good or not.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void sdcch_rx_callback(gl1_msg_decode_rpt *rpt_i, gas_id_t gas_id )
  {
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    cch_decode_rpt *rpt =  (cch_decode_rpt *)rpt_i;
#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_rxd_control_type gl1_rxd_ctl_flag;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    if (!rpt)
    {
       MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
       return;
    }

    /* Do RXLEV_VAL processing */
    gl1_update_emr_parameters ( &(rpt->hdr), FALSE, TRUE, gas_id );

    /*Update the RXqual full on this callback*/
    gl1_msg_rxqual_full_update( (uint16)rpt->hdr.rx_qual, gas_id );

#ifdef FEATURE_GSM_DYNAMIC_SAIC

    /* If SAIC NV is disabled then don't run SAIC disable code */
    if(gl1_hw_feature_disabled_saic(gas_id) != TRUE)
    {
      MSG_GERAN_HIGH_3_G(" RXQUAL %d saic d/e %d state %d",rpt->hdr.rx_qual,gl1_msg_get_saic_state(gas_id),l1_dedicated_data_ptr->l1_saic_dis_ena_state);
      /* If We receive a block with less than perfect RXQUAL when SAIC is OFF then enable SAIC */
      if( (rpt->hdr.rx_qual < L1_PERFECT_RXQUAL) &&
          (gl1_msg_get_saic_state(gas_id) == FALSE) &&
          (l1_dedicated_data_ptr->l1_saic_dis_ena_state == FALSE) )
      {
        /* Turn SAIC ON */
        gl1_hw_cfg_saic(TRUE, gas_id);
        gl1_hw_mcpm_amr_aeq_update(TRUE, gas_id);

        /* Set SAIC DISABLE/ENABLE STATE to stop Flip-Flopping of SAIC state */
        l1_dedicated_data_ptr->l1_saic_dis_ena_state = TRUE;
      }

    }
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
    if(gl1_rxd_ctl_flag.enableRxd) 
	  {
	  MSG_GERAN_HIGH_2_G("TCH SDCCH Hard RxQUAL:%d,FN:%d",rpt->hdr.rx_qual,gl1_get_FN(gas_id));
	  if((rpt->hdr.rx_qual < RXD_HARD_RXQUAL_THRESHOLD) &&
             (l1_dedicated_data_ptr->l1_enable_rxdiversity == FALSE) )
            {
            l1_dedicated_data_ptr->l1_enable_rxdiversity = TRUE;
            gl1_set_enable_rxdiversity(TRUE,gas_id);
            l1_ded_drx_agc_init[gas_id] = TRUE;
            }
	  }
#endif

    if ( rpt->hdr.good_data )
    {
      L1_send_PH_DATA_IND(TRUE,
                          SDCCH,
                          FALSE,
                          NULL_ARFCN,         /* ARFCN is not needed for SDCCH */
                          rpt->data,
                          N201_MAX, gas_id);
      MSG_GERAN_LOW_0_G("SDCCH decoded");
    }
    else
    {
      /* Bad Block received */
      L1_send_PH_DATA_IND(FALSE,
                          SDCCH,
                          FALSE,
                          NULL_ARFCN,         /* ARFCN is not needed for SDCCH */
                          NULL,
                          0, gas_id);
      MSG_GERAN_ERROR_0_G("SDCCH decode failed");
    }

    l1_log_message_metrics ( GL1_MSG_SDCCH, (gl1_msg_decode_rpt*)rpt, gas_id );
    l1_log_message_metrics_compact(GL1_MSG_SDCCH, (gl1_msg_decode_rpt*)rpt, gas_id);
  }


/******************************************************************************
 *
 *  Function name:  L1_set_to_sdcch_mode
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  This function sets up the parameters to allow a jump to SDCCH from the ISR
 *  context.
 *
 *
 *  Parameters:
 *  -----------
 *
 *
 *  Returns:
 *  --------
 *
 *
 *****************************************************************************/
void L1_set_to_sdcch_mode( gas_id_t gas_id )
{
     idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
     dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
     ARFCN_T sdcch8_chn = { 30, SYS_BAND_EGSM_900 };
     uint8 sdcch8_tn = 2;

     l1_dedicated_data_ptr->channel_specification.number_of_ded_channels = 0;
     l1_dedicated_data_ptr->channel_specification.DTX_indicator= FALSE;
     l1_dedicated_data_ptr->channel_specification.power_level=0;
     l1_dedicated_data_ptr->channel_specification.starting_time_valid=FALSE;
     l1_dedicated_data_ptr->starting_time=0;
     l1_dedicated_data_ptr->channel_specification.before_parameters_valid=FALSE;
     l1_dedicated_data_ptr->channel_specification.cipher_flag=FALSE;
     l1_dedicated_data_ptr->channel_specification.cipher_algorithm=0;
     l1_dedicated_data_ptr->channel_specification.channel_mode_1=0;
     l1_dedicated_data_ptr->channel_specification.channel_mode_2=0;
     l1_dedicated_data_ptr->channel_specification.channel_info_1_after.subchannel = 0;
     l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TSC =  (l1_idle_data->campedon_cell_BSIC & 0x07);
     l1_dedicated_data_ptr->channel_specification.channel_info_1_after.hopping_flag = FALSE;
     l1_dedicated_data_ptr->channel_specification.channel_info_1_after.MAIO = 0;
     l1_dedicated_data_ptr->channel_specification.channel_info_1_after.HSN = 0;
     l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list.no_of_items = 1;

     if (L1isr_get_jump_to_sdcch(gas_id) == 1)
     {
         l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type= 2;
         l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN = 0;
         l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list.channel[0] = l1_idle_data->campedon_cell_ARFCN;
         MSG_GERAN_MED_1_G("Switching to SDCCH/4 Channel %d TN %d", l1_idle_data->campedon_cell_ARFCN.num);
     }
     else if (L1isr_get_jump_to_sdcch(gas_id) == 2)
     {
         l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type= 3;
         l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN = sdcch8_tn;
         l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list.channel[0] = sdcch8_chn;
         MSG_GERAN_MED_2_G("Switching to SDCCH/8 Channel %d TN %d", sdcch8_chn.num,sdcch8_tn);
     }
}


static void sdcch_metrics_callback(gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id)
{
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
  int i;

  if (!rpt)
  {
     MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
     return;
  }

  /* Store rx power over the current blocks for RXLEV_VAL computation later */
  gl1_store_block_rx_power ( rpt, 4, gas_id );

  for (i=0; i<4; i++)
  {
     /* If carrier is on the hopping sequence, exclude it from RXLEV */
     if ( gl1_include_in_rxlev ( rpt[i]->arfcn, gas_id ) )
     {
        /* All bursts get accumulated here for RXLEV_FULL and SUB */
        l1_serving_cell_meas_ptr->rx_power     += rpt[i]->pwr_dBm_x16;
        l1_serving_cell_meas_ptr->rx_power_sub += rpt[i]->pwr_dBm_x16;
        l1_serving_cell_meas_ptr->no_of_measurements++;
        l1_serving_cell_meas_ptr->no_of_measurements_sub++;
#ifdef FEATURE_GSM_RX_DIVERSITY
        if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
        {
           /*If RxD is off at burst level, then use PRx RSSI for maintaining DRx RSSI*/
           if(rpt[i]->pwr_dBm_x16_divrx == 0)
           {
              l1_serving_cell_meas_ptr->divrx_power     += rpt[i]->pwr_dBm_x16;
              l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16;
           }
           else
           {
              l1_serving_cell_meas_ptr->divrx_power     += rpt[i]->pwr_dBm_x16_divrx;
              l1_serving_cell_meas_ptr->divrx_power_sub += rpt[i]->pwr_dBm_x16_divrx;
           }
        }
        else
        {
           l1_serving_cell_meas_ptr->divrx_power = 0;
           l1_serving_cell_meas_ptr->divrx_power_sub  = 0;
        }
#endif
     }
     else
     {
#ifdef FEATURE_GSM_RX_DIVERSITY
        if(gl1_get_rxd_ctl_nv(gas_id) == TRUE)
         {
         MSG_GERAN_LOW_3_G("Excluding ARFCN %d RSSI %d from PRx_RXLEV %d DRx_RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4,rpt[i]->pwr_dBm_x16_divrx>>4);
         }
#else
        MSG_GERAN_LOW_2_G("Excluding ARFCN %d RSSI %d from RXLEV",rpt[i]->arfcn.num,rpt[i]->pwr_dBm_x16>>4);
#endif
     }
  }

  /* Process TT, and AFC here */
  for ( i=0; i<4; i++ )
  {
    /* If SAIC is used in a burst the mDSP sets bit 14 of */
    /* the misc_flag true. Additionally, for AFC/TT track */
    /* the flag selects the specific GMSK SAIC threshold. */
    if ( gl1_msg_saic_in_burst( rpt[i]->misc_flags ) )
    {
      gl1_msg_cch_tt_update ( rpt[i]->timing_offset,
                              gl1_msg_saic_tt_threshold_snr( rpt[i] ), TRUE ,gas_id);

      gl1_msg_cch_afc_update( rpt[i]->freq_offset,
                              gl1_msg_saic_afc_threshold_snr( rpt[i] ), TRUE,gas_id );
    }
    else
    {
      gl1_msg_cch_tt_update ( rpt[i]->timing_offset, rpt[i]->snr, FALSE ,gas_id);

      gl1_msg_cch_afc_update( rpt[i]->freq_offset, rpt[i]->snr, FALSE ,gas_id);
    }
  }

  l1_log_burst_metrics ( GL1_MSG_SDCCH, rpt, gas_id  );

}

#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION  l1_sdcch_get_arfcns

DESCRIPTION
  This function determines the ARFCNs to be used on the specified frames 
  according to the defined frequency hopping parameters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_sdcch_get_arfcns(int16 start, uint16 increment, uint16 num_arfcns, ARFCN_T arfcns[], gas_id_t gas_id)
{
  dedicated_data_T *ded_dtr_ptr = &l1_dedicated_data[gas_id];

  L1_get_ARFCNS ( start, increment, num_arfcns, arfcns,
                  &ded_dtr_ptr->channel_specification.channel_info_1_before,
                  &ded_dtr_ptr->channel_specification.channel_info_1_after,
                  &ded_dtr_ptr->new_channel_info, gas_id );
}
#endif /*FEATURE_GSM_COEX_SW_CXM */
