/*=========================================================================
L 1   T R A N S F E R   F I X E D   A L L O C A T I O N  L O G I C A L

C H A N N E L S

GENERAL DESCRIPTION
   This module contains the procedures to handle the ISR level processing
   of the single block allocation locaical channels

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_single.c#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
10/06/17   sn       CR2118472 Starting SCE before TS adjustment is done in 2 Phase access
20/07/16   sp        CR1040702: Allow TA during 2Phase access
08/09/17   sn       CR1061730 FR43082: TA during PS Procedures- GL1 changes
13/09/17   nm       CR2108286 Do not allow GBTA from single block if tuneaway data is not init
08/08/17   nm       CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
18/5/16    sn        CR1007565 PACKET Transfer DTF Events/Handlers Phase1
13/05/16   db        CR937805. DL BLER for GPRS and EGPRS DL TBF
23/12/15   sp      CR954806: In SingleBlk tx transmission, calculate arfcns to use only on block start.
07/12/15   og       CR945690. Ensure the frame number comparison takes into account
                    the negative range when transitioning out of G2X_TA_DISABLE_RXTX_NULL.
16/09/15   cjl      CR908527 improve system procedures used for TRM access
02/09/15   zf       CR905869: Set trm_status to NULL after grm release
21/07/15   cjl      CR876903 Use system procedure types for TRM subreasons 
21/07/15   cjl      CR876741 Replace TRM subreasons with system procedure types
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
03/07/15   pa       CR865455: Don't clear grm frequency info during Single SIM initialisation. 
30/06/15   cjl      CR863267 Treat GL1_TRM_RETAINED_FOR_ACCESS as GL1_TRM_GRANTED in *_TRM_WAIT states
22/06/15   smd      CR857943 GL1 to release and request diversity chain at GTA and RACH tune away
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
07/05/15   ggu      CR822105 Correct wrongly decreased NCELL FN lag when back to back TS slam
30/04/15   ap       CR811057 FR22272 - TRM Unification  - Logging Changes
04/02/14   nk       CR767734 Allowing TA during 2-phase access
17/02/15   am       CR768900 Enable F3 when SB is decoded
30/01/15   pa       CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
15/12/14   pg       CR630281 implementation of RxD support for data/MSRD
25/11/14   sp       CR762520: Send failure indication to MAC, if single block decoded isnt control block.
07/11/14   pjr      CR751950 Ensure wtr hopping is inactive when state changes from transfer to idle.
10/09/14   pjr      CR719548 Add camp_tn to tn  in function sb_dl_metrics_cb to ensure that log 
                    shows over the air timeslot instead of slammed tn
11/09/14   ws       CR723099 Resolve GL1 compile warnings
06/08/14   hd       CR703953 Move to idle after sending the MAC_L1_SINGLE_BLOCK_CONFIG
29/07/14   ap       CR695234: Changes for GL1's New Rach interface
02/07/14   mc       CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
20/06/14   pjr      DATA+MMS feature - USF utilisation calculation
30/05/14   ws       CR657674 Data + MMS Phase 1
08/05/14   br       CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
02/10/13   pjr      CR552448 DDR Frequency plan implementation - l1_arbitration modification
24/09/13   sp       CR449651: Use new logPacket for Burst metrics
18/10/13   gk       CR553692: DSDA Register Freq with CXM during Single block Packet Access
27/09/13   sp       CR551580: Add NV logging of outgoing L1 messages
29/07/13   sk       CR519663 Partial QBTA bringup changes
06/06/13   mc       CR492352 : COEX Support : Per-Slot Channel Indication (for RRBP) + COEX params for monitors
06/03/12   pg       CR457891: Trigger Tx band change earlier during transfer init
15/02/13   ws       CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
03/01/13   cja      CR435212 Delay sending l1 mac single block sent until in idle state.
28/11/12   br       CR383165 Included header file gl1_msgi.h and array intialised to remove compilation warnings
24/08/11   kb       CR302696 - Added Initialization function to initialise grprs single data values.
25/05/11   ap       DSDS CR:289094 - Signal release of the Single block after the mDSP
                    complete has began to terminate
17/03/11   cja      CR279518 Add single slot FTM power store for GPRS/EDGE
05/05/10   ws       CR232901 - further changes as previous change did not work
28/04/10   dv       CR232901 - DL FTP data Stall while switching
                    from CS 2 to CS 3 cell reselect
11/12/09   ab       CR 220464 Add REL_IND/REL_CNF to DL_SINGLE_BLOCK to prevent L1 and MAC
                    go out of sync.
02/12/09   ws       Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                    Improvements
28/01/09   og       Do not apply the Enhanced Ext TBF criterea in single block
                    TBF. Resolves CR170320.
26/01/09   og       Ensure rxqual calculations ignore CS4 radio blocks. Resolves
                    CR170214.
26/01/07   og       Implement UL ctrl msg deletion in L1.
28/11/06   dv       Lint cleanup
14/11/06   cs       CR102226 Fix the Uplink Power Logging to use unslammed slot
31/07/06   av       CR85810 NB AB TSC, and TA fix.
06/01/06   ws       Added Burst and message metrics log packet to single block Rx's
15/12/05   og       Fix the initialisation of abort_once_only boolean. Resolves
                    T3168 lockup scenario, Boca Raton - Florida.
06/12/05   og       Merge change 267041 from raven branch, resolves CR 82938.
23/11/05   og       Update tx call back routines to provide more information.
18/11/05   og       Correction to suppression of QLint 8.0 errors.
02/11/05   nt       new function to reset static flags/data gpl1_single_block_data_init()
08/08/05   og       Restructuring PL1 before addition of DTM.
07/29/05   gfr      Support for NPL1 DTM interface changes
04/07/05   og       Make use of the NPL1 camp timeslot buffering scheme.
02/06/05   og       Removal of header-header includes.
01/06/05   og       Add validity flag check to the metrics callback routines.
17/01/05   ws       Lint updates
10/01/05   ws       If allocated DL Single block allocation and CRC fails then
                    send L1_MAC_51_SB_FAILURE. Fix for CR 54831
13/12/04   og       Removal of single block flag from gprs change timeslot function.
2/11/04    og       Addition of egprs srb test mode capability.
28/10/04   ws       Lint tidy up
25/10/04   og       Re-arrangement of private/public functions.
13/10/04   kf       Changes for time slot slamming in transfer.
12/10/04   ws       Added Multiblock Allocation for EGPRS
17/09/04   ws       Setup power for single block tx based on GAMMA/ALPHA rather
                    than PMAX
26/08/04   kf       changes to ncell sync's for ts slamming.
26/05/04   sv       Changed "l1_isrtsk_blk" structure parameters for better
                    code readability.
23/06/04   og        Egprs Qual measurements handling.
12/05/04   ws       Fixed corruption of PBCCH_PCCCH buffer when single block
                    decode fails
05/05/04   kf       Changes for ts slamming.
05/05/04   og       Removal of Lint warnings.
28/04/04   rm       Replaced RRBP tx codes with function calls to RRBP module. Also
                    cleaned up lint errors.
15/03/04   ws       Added additional check for txing RRBP to ensure they are valid
11/03/04   ws       Added payload filter to data callback to stop RLC data entering MAC
27/11/03   tl       Addition of gpl1_gprs_send_l1_mac_single_block_sent()
26/11/03   ws       Lint cleanup
29/10/03   bk       Fix for decrementing the num of rrbp msgs
14/10/03   npr      Set TXLEV to pmax for single block transmit.
10/10/03   npr      Removed fixed power index of 5 for RRBP tx - now Max CCH Pwr.
15/08/03   pjr      Removed fixed power for transmit bursts, always use measured
                    value.
12/08/03   ws       Removed ncell_meas_fixed from gpl1_gprs_transfer_send_ftn()
10/07/03   bk       Changed the call to gpl1_gprs_setup_start_idle to be called from
                    the task context.
26/06/03   ws       wait until PCA is txd before releasing TBF
25/06/03   gw       Use gpl1_gprs_setup_start_idle() to start Idle Mode.
06/19/03   gw       Changes for starting/stopping SCE going in/out of
                    52 Idle Mode.
19/06/03   gw       Changes to starting and stopping of SCE.
18/06/03   ws       Added missing bcch ARFCN from single data callback
17/06/03   ws       Added returning to idle via sending PCA
07/05/03   ws       Fixes for Single Ul and DL for TC_41_2_4_2
09/04/03   pjr      Added sending of RRBP to single block receive.
26/02/03   npr      Changed instances of INVALID to GPL1_INVALID
21/02/03   pjr      Modified the 2-phase access process to remain on the PDTCH
                    once the PRR has been sent, regardless of whether the PBCCH
                    is present.
14/02/03   pjr      Removed redundant agc_params parameter.
05/02/03   ws       Moved scheduling of UL and DL channels to frame 4 of RB
06/12/02   ws       Added gl1_msg_reset() in case of dl single block allocation
04/12/02   DLH      Modified gpl1_gprs_calculate_arfcns api.
25/11/02   pjr      Minor modification to debug message.
25/11/02   ws       Stopped release confirm being sent for single ul and single dl
                    which are not 2 phase assignments
04/11/02   pjr      Merge Neighbour cell measurements under feature switch
01/11/02   pjr      Added serving cell measurements under feature switch
01/11/02   WS       Added L1_MODE_RUN test around rx scedules
10/21/02   He       Replaced FN by gl1_get_FN(), tx_signal_strength by TXLEV
28/10/02   DLH      Added gprs_l1_page_group_flag to PH_DATA_IND
09/10/02   ws       Added DL siingle block handling
27/09/02   ws       If moving back to 51 mfrm only abort on frame 2
23/09/02   ws       Added setting of timing advance via gl1_msg_store_ta()
06/09/02   pjr      Added gpl1_gprs_adjust_mod_num function where FN was being
                    modified.
05/09/02   pjr      Greatly modified after integration, including adding of data
                    and metrics callbacks.
03/09/02   pjr      Modified handling of prach states.
05/08/02   ws       Changed FEATURE_GPRS to FEATURE_GSM_GPRS_L1
24/07/02   pjr      Added setting of prach sub_state.
                    Added setting of idle sub_sub_state
07/19/02   DLH      Added call to gprs freq. hopping function

05/02/02   pjr      Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_msg_g.h"
#include "gl1_msg_pdch.h"
#include "gl1_sys_algo_gprs.h"
#include "l1_isr.h"
#include "l1_task.h" /* Used for passing BSIC for access burst tx's */
#include "l1_drx.h"
#include "gpl1_gprs_isr.h"
#include "msg.h"
#include "ms.h"
#include "gmacl1.h"
#include "l1_utils.h" /* gl1_get_FN() */
#include "l1_sc_drv.h" /* l1_scdrv_call_in_task */
#include "gl1_msgi.h"


#include "gpl1_gprs_log.h"


#include "geran_eng_mode_info.h"

#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_GSM_WTR_HOP
#include "gl1_msg_wtr_hop.h"
#endif

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
#include "geran_dual_sim_g.h"
#include "l1_sc_irat.h"
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* #defines */
#define FN_MODULO_13  13U

/*  Structure to preserve data for PBCCH and PCCCH data ind, added complication
    is that consequective blocks can contain PCCCH so this information needs to
    be stored in some form of fifo */
typedef struct
{
  uint32  fn;
  uint8   tn;

} sb_dl_block_int_t;

struct
{
  sb_dl_block_int_t instance_1;
/*  sb_dl_block_int_t instance_2; Keep lint happy about unreferenced local struct element */
  sb_dl_block_int_t instance_3;
  sb_dl_block_int_t *instance_ptr_in;
  sb_dl_block_int_t *instance_ptr_out;

} sb_dl_l1_pbcch_pccch;

typedef struct
{
uint8  pacch_pdtch_ul_blk_start_cnt ;
uint8  pacch_pdtch_dl_blk_start_cnt ;
gl1_defs_rx_pkt_data_type            msg_data_mem[MAX_NUM_DL_MSGS]; /* buffer for storing DL PACCH PDTCH data */
gl1_defs_rx_pkt_metrics_type         metrics_data_mem[4]; /* buffer for storing DL metrics */
gl1_defs_rx_pkt_mem_type             sb_dl_data; // holds ptr's to metrics, log and data buffer for rx's

/* Controls the release process allowing the setup of variable for only
the first pass through the releasing loop */
boolean  abort_once_only ;

boolean              enable_single_block_receive ;
boolean              enable_single_block_transmit ;
boolean              enable_sb_dl_callbacks_setup ;
boolean              do_single_rx_once;
boolean              rrbp_to_transmit ;
uint32               rrbp_end_fn ;
uint8    abort_fn ;
}gpl1_gprs_single_data_t;


/* This array is used to supply the number of frames required before
 * the single block TBF can be safely aborted and the relevant timebase
 * change to PCCCH/CCCH can be applied.
 */
static const uint8  IS_PDCH_CHAN_ABORTED[ FN_MODULO_13 ] =
{
  /* 0, 1, 2, 3, 4, 5, 6, 7, 8,   9, 10, 11, 12 */
     4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12, 0
};

static gpl1_gprs_single_data_t  gpl1_gprs_single_data[NUM_GERAN_DATA_SPACES];
// static gpl1_gprs_single_data_t *gpl1_gprs_single_data_ptr = gpl1_gprs_single_data;

#ifdef FEATURE_GTA_2PHASE_ACCESS
#define TBF_MIN_DURATION_MS 40
gpl1_g2x_ta_disable_rx_tx_state g2t_ta_seq_trm_state[NUM_GERAN_DATA_SPACES] = {G2X_TA_DISABLE_RXTX_NULL,G2X_TA_DISABLE_RXTX_NULL};
static byte fw_rf_setting_up[NUM_GERAN_DATA_SPACES]                         = { INITIAL_VALUE(RF_FW_SETUP_FRAME_DELAY) };
static byte suspension_timer[NUM_GERAN_DATA_SPACES]                         = { INITIAL_VALUE(SUSPENSION_FRAME_DELAY) };
#define G2X_TUNEAWAY_FN_DIFF_THRESHOLD     20
#define G2X_TUNEAWAY_2PHASE_ACCESS_TIMEOUT 18  // 85 ms
/*===========================================================================

FUNCTION gpl1_g2x_trm_unlock_for_X

DESCRIPTION
             State machine for GTA gap during 2 Phase Access

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void gpl1_g2x_trm_unlock_for_X(gas_id_t gas_id);

/*===========================================================================
FUNCTION gpl1_gprs_check_susp_FN_gta_for_tds

DESCRIPTION
             This function compares suspension FN with current FN to decide when its time to start aborting
             and prepare for GTA gap.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE none

===========================================================================*/
void gpl1_gprs_check_susp_FN_gta_for_tds(gas_id_t gas_id);
/*===========================================================================
FUNCTION gpl1_gprs_reset_G2T_2phase_state_machine

DESCRIPTION
             This function reset variables related to G2T 2 Phase Access.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE boolean

===========================================================================*/
void gpl1_gprs_reset_G2T_2phase_state_machine(gas_id_t gas_id);


#endif /*FEATURE_GTA_2PHASE_ACCESS*/

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
extern void (*gpl1_g2x_ta_seq[NUM_GERAN_DATA_SPACES])(gas_id_t);
extern boolean        stop_sc_tick_trans[NUM_GERAN_DATA_SPACES];
extern trans_T  trans[NUM_GERAN_DATA_SPACES];
extern boolean gpl1_g2x_ta_register_seq(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id);
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

/*===========================================================================

FUNCTION init_gpl1_gprs_single_data

DESCRIPTION
             Initializes grprs single data values .

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void init_gpl1_gprs_single_data( gas_id_t gas_id )
{
  memset ( &gpl1_gprs_single_data[gas_id], 0 , sizeof ( gpl1_gprs_single_data_t) );
  gpl1_gprs_single_data[gas_id].pacch_pdtch_dl_blk_start_cnt = GPL1_INVALID;
  gpl1_gprs_single_data[gas_id].pacch_pdtch_ul_blk_start_cnt = GPL1_INVALID;
  gpl1_gprs_single_data[gas_id].abort_once_only = TRUE;
  gpl1_gprs_single_data[gas_id].enable_single_block_receive = FALSE;
  gpl1_gprs_single_data[gas_id].enable_single_block_transmit = FALSE;
  gpl1_gprs_single_data[gas_id].enable_sb_dl_callbacks_setup = TRUE;
  gpl1_gprs_single_data[gas_id].do_single_rx_once = FALSE;
  gpl1_gprs_single_data[gas_id].rrbp_to_transmit = FALSE;
  gpl1_gprs_single_data[gas_id].rrbp_end_fn = L1_GPRS_INVALID_FN;
}


/*===========================================================================

FUNCTION gpl1_gprs_send_mac_51_sb_failure

DESCRIPTION
  This function sends gpl1_gprs_send_mac_51_sb_failure to MAC

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_gprs_send_mac_51_sb_failure(void *dummy_param, gas_id_t gas_id)
{

  /*  Inform MAC  */
  l1_mac_sig_t            message_buffer,*message;


  /* Align the pointers */
  message = &message_buffer;

  NOTUSED(dummy_param);

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = L1_MAC_51_SB_FAILURE;
  message->msg.sb_51_failure.gas_id = gas_id;

  /* Send the message, NULL used for MAC_PH_DATA_IND */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
}

/*===========================================================================

FUNCTION  gpl1_gprs_control_single_tbf

DESCRIPTION
  This function controls transfer mode for single block allocation.

DEPENDENCIES
  pointer to dedicated mode data

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean gpl1_gprs_control_single_blk(single_blk_information_T *single_blk_ptr,
                                  frequency_information_T *freq_info_ptr,
                                  l1_mode_command_T    mode, gas_id_t gas_id)
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gpl1_gprs_single_data_t *gpl1_gprs_single_data_ptr = &gpl1_gprs_single_data[gas_id];

  transfer_data_T               *transfer_data_ptr;
  gl1_msg_tx_pkt_cb_type             tx_callback;
  gl1_msg_ul_pacch_pdtch_signal_type signal_info;

  uint8                *payload_data[3];
  gl1_defs_tx_pkt_data_type pkt_data;

  boolean              idle = FALSE;
  gl1_defs_coding_type cs = GL1_DEFS_CS1_CODING;
  uint32               fn_num;
  uint8                rrbp_index = SIZE_OF_RRBP_BUFF;
  dword                       FN = gl1_get_FN( gas_id );
  uint8               slammed_slot =0;

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
  gpl1_g2x_ta_priority_t pri_fire = G2X_TA_PRI_RESERVED;
  static boolean	abort_pending[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
  l1_serving_cell_meas_T	 *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
#ifdef FEATURE_GPRS_GBTA
  boolean gbta_go_idle = FALSE;
#endif 
#endif/*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

  l1_transfer_data[gas_id].send_l1_mac_single_block_sent = FALSE;
  gpl1_gprs_calculate_arfcns(l1_get_gprs_arfcns(gas_id), freq_info_ptr, NORM,gas_id);

  /* assign shortcut pointer's for use in function */
  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      if(single_blk_ptr->two_phase_access)
      {
       gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_2PHASE_ACESS);
      }
#endif


  if(l1_tskisr_blk->sub_state == L1_TRANSFER_RECONFIG)
  {
    /* Network may require ARAC message to be sent again on a single block
     * allocation if it did receive it the first time from multiblock allocation
     */
     tx_callback.discarded_callback = (gl1_msg_discarded_data_callback) gpl1_gprs_tx_single_pdtch_disc_cb;
    tx_callback.scheduled_callback = (gl1_msg_scheduled_data_callback) gpl1_gprs_tx_single_pdtch_sched_cb;
    tx_callback.usf_utilisation_callback = (gl1_msg_usf_utilisation_callback) gpl1_gprs_tx_dyn_rrbp_usf_util_cb;

    /* Configure NPL1 for Uplink; added AB_TSC */
    gl1_msg_cfg_ul_pacch_pdtch(freq_info_ptr->tsc,TS0,l1_idle_data->campedon_cell_BSIC,&tx_callback,gas_id);

    /* Setup timing advance for the single block */
    gl1_msg_store_ta(single_blk_ptr->timing_advance_params.ta_value_ie.ta_value,gas_id);

    /***
    *** General Initialisation
    ***/




    /*  Initialize the sb dl bcch/ccch parameters  */
    sb_dl_l1_pbcch_pccch.instance_ptr_in = &(sb_dl_l1_pbcch_pccch.instance_1);
    sb_dl_l1_pbcch_pccch.instance_ptr_out = &(sb_dl_l1_pbcch_pccch.instance_1);

    if (single_blk_ptr->sb_ul_dl_type == SINGLE_UL_BLOCK)
    {
      gpl1_gprs_single_data_ptr->enable_single_block_receive = FALSE;
      gpl1_gprs_single_data_ptr->enable_single_block_transmit = TRUE;
      gpl1_gprs_single_data_ptr->enable_sb_dl_callbacks_setup = FALSE;
      l1_tskisr_blk->sub_state = L1_SINGLE_BLK_TX;


    }else
    {
      MSG_GERAN_ERROR_1_G(" single block alloc type incorrect %d",
                                     single_blk_ptr->sb_ul_dl_type);
    }
    l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

    /* preset wait for pca vars */
    transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
    transfer_data_ptr->wait_for_pca_valid = FALSE;

    /* Ensure no rlc data can be transmitted during single block */
    transfer_data_ptr->rlc_ul_data_available = FALSE;

    /* Slam timebase to the lowest received timeslot */
    gpl1_gprs_change_timeslot( single_blk_ptr->ts_number, gas_id );

    slammed_slot = gpl1_gprs_ts_conv(single_blk_ptr->ts_number,gas_id);


  }



  if(l1_tskisr_blk->sub_state == L1_TRANSFER_INIT)
  {
    /* Configure AGC
     *
     * Currently fixed to no PBCCH present and no dl power control
     */
#ifdef FEATURE_GTA_2PHASE_ACCESS
    MSG_GERAN_LOW_0_G("2PHASE_ACCESS: Reset Disable Rx");
    gta_2p_susp_info[gas_id].disable_rx = FALSE;
#endif
    sys_algo_gprs_agc_cfg(0,0,FALSE,FALSE,gas_id);

    /* Set AGC valid on first RX block */
    gpl1_gprs_update_first_rx(TRUE, gas_id);
        /* initiate RF Tx band change - only gets actioned if band has actually changed */
    gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]),gas_id);

    tx_callback.discarded_callback = (gl1_msg_discarded_data_callback) gpl1_gprs_tx_single_pdtch_disc_cb;
    tx_callback.scheduled_callback = (gl1_msg_scheduled_data_callback) gpl1_gprs_tx_single_pdtch_sched_cb;
    tx_callback.usf_utilisation_callback = (gl1_msg_usf_utilisation_callback) gpl1_gprs_tx_dyn_rrbp_usf_util_cb;

    /* Configure NPL1 for Uplink; added AB_TSC */
    gl1_msg_cfg_ul_pacch_pdtch(freq_info_ptr->tsc,TS0,l1_idle_data->campedon_cell_BSIC,&tx_callback,gas_id);

    /* Setup timing advance for the single block */
    gl1_msg_store_ta(single_blk_ptr->timing_advance_params.ta_value_ie.ta_value,gas_id);

    /***
    *** General Initialisation
    ***/

   gl1_hw_set_ftm_power_store_init(TRUE,gas_id);

    /* Initialise the abort once flag to TRUE */
    gpl1_gprs_single_data_ptr->abort_once_only = TRUE;

    /*  Initialize the sb dl bcch/ccch parameters  */
    sb_dl_l1_pbcch_pccch.instance_ptr_in = &(sb_dl_l1_pbcch_pccch.instance_1);
    sb_dl_l1_pbcch_pccch.instance_ptr_out = &(sb_dl_l1_pbcch_pccch.instance_1);

    if (single_blk_ptr->sb_ul_dl_type == SINGLE_UL_BLOCK)
    {
      gpl1_gprs_single_data_ptr->enable_single_block_receive = FALSE;
      gpl1_gprs_single_data_ptr->enable_single_block_transmit = TRUE;
      gpl1_gprs_single_data_ptr->enable_sb_dl_callbacks_setup = FALSE;
      l1_tskisr_blk->sub_state = L1_SINGLE_BLK_TX;
      l1_transfer_data[gas_id].starting_time_valid = FALSE;

    }else if (single_blk_ptr->sb_ul_dl_type == SINGLE_DL_BLOCK)
    {
      gpl1_gprs_single_data_ptr->enable_single_block_receive = FALSE;
      gpl1_gprs_single_data_ptr->enable_single_block_transmit = FALSE;
      gpl1_gprs_single_data_ptr->enable_sb_dl_callbacks_setup = FALSE;
      l1_tskisr_blk->sub_state = L1_SINGLE_BLK_RX;
      l1_transfer_data[gas_id].starting_time_valid = FALSE;
      gpl1_gprs_single_data_ptr->do_single_rx_once = TRUE;
      single_blk_ptr->sb_crc_fail = FALSE;
      /* Reset frame layer as we have just come from 51 MFRM Idle */
      gl1_msg_reset(gas_id);
    }
    l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

    /* preset wait for pca vars */
    transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
    transfer_data_ptr->wait_for_pca_valid = FALSE;

    /* Ensure no rlc data can be transmitted during single block */
    transfer_data_ptr->rlc_ul_data_available = FALSE;
   
#ifdef  FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
    if(single_blk_ptr->two_phase_access)
    {
       MSG_GERAN_HIGH_0_G("2PHASE_ACCESS:Start SCE in Init");
       l1_sc_start( L1SCModeGprsTrans, gas_id );
    }
#endif /* FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/


    /* Slam to lowest received timeslot */
    gpl1_gprs_change_timeslot( single_blk_ptr->ts_number, gas_id );
    slammed_slot = gpl1_gprs_ts_conv(single_blk_ptr->ts_number,gas_id);

    MSG_GERAN_MED_3_G(" SINGLE BLK ts num %d slam ts %d curr ts %d",l1_transfer_data[gas_id].single_block_tbf_1.ts_number,slammed_slot,l1_get_current_timeslot(gas_id));

#ifdef FEATURE_GSM_COEX_SW_CXM
   garb_intf_notify_twophase_access(freq_info_ptr->frequency_list,gas_id );
#endif /* FEATURE_GSM_COEX_SW_CXM*/
  }


  switch(l1_tskisr_blk->sub_state)
  {
/*lint -save -e616 No break from previous case acceptible */
    case L1_SINGLE_BLK_TX:
    {
/*lint -restore*/
      switch(frame_counters[gas_id].FNmod13)
      {
        case 3:   /* Schedule for Block 1 */
        case 7:   /* schedule for Block 2 */
        case 12:  /* schedule for Block 3 */
        {
          /* schedule Block  (fn mod 4) = 3 */

          gpl1_gprs_calculate_arfcns(l1_get_gprs_arfcns(gas_id), freq_info_ptr, NORM,gas_id);

          if (gpl1_gprs_single_data_ptr->enable_single_block_transmit)
          {
            if( mode == L1_MODE_RUN )
            {
              /* temp place holder for TX PWR to write Eng Mode Info */
              int16 tmp_tx_pwr[8] = {0};

              /***
              *** Schedule UL PACCH_PDTCH TN(s)
              ***/
              /*  Set power and timeslot  */
              signal_info.tn = (gl1_defs_tn_type)slammed_slot;
              signal_info.TXLEV =(int16)gprs_serv_cell_meas_ptr->pch[l1_transfer_data[gas_id].single_block_tbf_1.ts_number];


              /* Determine what data to transmit and
              * which coding scheme to use.
              * Either PACCH control data or UL DUMMY
              */
              (void)gpl1_gprs_get_egprs_ul_data( &cs,
                                           NULL,
                                           payload_data,
                                           gas_id,
                                           TRUE        /* single block alloc */
                                         );

              pkt_data.gprs.data     = payload_data[0];

              /* Schedule fixed UL pdch PS = NULL*/
              gl1_msg_tx_pacch_pdtch_fixed(&pkt_data, &signal_info,l1_get_gprs_arfcns(gas_id),cs,GL1_DEFS_FIXED_PACCH,gas_id);

              /* Report using original TS allocation and not slammed */
              gpl1_log_gprs_power_control( GL1_MSG_UL_PACCH_PDTCH,
                                           (gl1_defs_tn_type) gpl1_gprs_ts_conv_unslam( signal_info.tn,gas_id ),
                                           signal_info.TXLEV,
                                           cs, gas_id );

              tmp_tx_pwr[0] = signal_info.TXLEV;
              geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, gas_id);

              /* UL BLOCK start for PACCH or PDTCH to be set in 1 frame */
              gpl1_gprs_single_data_ptr->pacch_pdtch_ul_blk_start_cnt = 0;

              /* Have we transmitted our last allocated radio block ? */
              if(!(--single_blk_ptr->num_rb_alloc))
              {
                /*  Set enable transmit to ensure this is the last block which is sent */
                gpl1_gprs_single_data_ptr->enable_single_block_transmit = FALSE;
              }
            }
            else
            {
              MSG_GERAN_ERROR_1_G(" Single blk tx not tasked FNmod13 %u ", frame_counters[gas_id].FNmod13);
            }
          }

          break;

        } /* End of case */

        default:
          break;
        /* do nothing no actions on this frame*/


      } /* end of switch */

      break;
    } /*  end of L1_FIXED case  */


    case L1_SINGLE_BLK_RX:
    {
      /*  Receive functionality required in dl single block Mode.
       *   Receive one DL block on the PDTCH
       */
      if (!gpl1_gprs_single_data_ptr->enable_sb_dl_callbacks_setup)
      {
        uint8                       i;
        gl1_defs_rx_pkt_alloc_type  alloc_type[GL1_DEFS_MAX_ASSIGNED_DL_TS];
        gl1_defs_rx_pkt_cb_type     sb_dl_callbacks;

        /*  Configure receive callbacks for single block mode */
        gpl1_gprs_single_data_ptr->sb_dl_data.msg_data_mem = &gpl1_gprs_single_data_ptr->msg_data_mem[0];

        for (i=0; i<4; i++)
        {
          gpl1_gprs_single_data_ptr->sb_dl_data.metrics_data_mem[i] = &gpl1_gprs_single_data_ptr->metrics_data_mem[i];
        }

        sb_dl_callbacks.data_callback = sb_dl_data_cb;
        sb_dl_callbacks.metrics_callback = sb_dl_metrics_cb;
        sb_dl_callbacks.discarded_callback = NULL;
        sb_dl_callbacks.log_callback     = NULL;

        /* preset alloc struct to unallocated */
        for(i=0;i<GL1_DEFS_MAX_ASSIGNED_DL_TS;i++)
        {
          alloc_type[i].usf = GL1_DEFS_PDCH_UNALLOCATED;
          alloc_type[i].tn = GL1_DEFS_TN_0;
        }

        /* Configure NPL1 for rx handler */
        gl1_msg_cfg_dl_pacch_pdtch(
                                alloc_type,
                                1,
                                freq_info_ptr->tsc,
                                FALSE,
                                &sb_dl_callbacks,
                                gas_id
                              );

        /*  Set flag to ensure this only happens the once   */
        gpl1_gprs_single_data_ptr->enable_sb_dl_callbacks_setup = TRUE;

      } /* end of if enable_sb_dl_callbacks_setup */

      /*  The following switch statement determines the receive actions
          to be taken for the specified frame number. The action has to
          be actioned by PL1 in frame 4 of the block before the
          block the action is specified for. e.g. B1 is actioned in frame 3,
          mod13 in this switch statement.  */
      if (gpl1_gprs_single_data_ptr->do_single_rx_once)
      {
        switch (frame_counters[gas_id].FNmod13)
        {
          /*  Read all blocks.  */
          case 3:     /* B1, B4, B7, B10 */
          case 7:     /* B2, B5, B8, B11 */
          case 12:    /* B0, B3, B6, B9 */
          {
            /*  Determine arfcn whether or not implementing
                frequency hopping    */
            gpl1_gprs_calculate_arfcns(l1_get_gprs_arfcns(gas_id), freq_info_ptr, NORM,gas_id);

            /* Update the camp timeslot */
            gl1_msg_pdch_set_camp_tn( (gl1_defs_tn_type) l1_get_current_timeslot(gas_id) ,gas_id);

            gl1_msg_rx_pacch_pdtch((gl1_defs_tn_type)slammed_slot,
                                    l1_get_gprs_arfcns(gas_id),
                                    &transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                                    &transfer_data_ptr->packet_transfer_div_agc,
#endif
                                    &gpl1_gprs_single_data_ptr->sb_dl_data,gas_id);

            /*  Set up the frame number the dl block start should be set    */
            gpl1_gprs_single_data_ptr->pacch_pdtch_dl_blk_start_cnt = 0;

            /*  Setup Frame number and timeslot for PH_DATA_IND message  */
            sb_dl_l1_pbcch_pccch.instance_ptr_in->fn =
                                (uint32)gpl1_gprs_adjust_mod_num(FN, 1, FRAMES_IN_HYPERFRAME);
            sb_dl_l1_pbcch_pccch.instance_ptr_in->tn = (uint8)single_blk_ptr->ts_number;
            /*  Manage pointer  */
            (sb_dl_l1_pbcch_pccch.instance_ptr_in == &(sb_dl_l1_pbcch_pccch.instance_3)) ?
                  (sb_dl_l1_pbcch_pccch.instance_ptr_in = &(sb_dl_l1_pbcch_pccch.instance_1)) :
                  (sb_dl_l1_pbcch_pccch.instance_ptr_in++);

            gpl1_gprs_single_data_ptr->do_single_rx_once = FALSE;
            break;
          } /*  End of block start case */

          default:
            break;

         } /* end of switch */

      }

      break;
    }


    default:
      MSG_GERAN_ERROR_0_G("PL1: bad state");
      break;

  } /* end of switch for l1tskisr_blk->sub_state*/

  /*  If enabled receive on all blocks  */
  if (gpl1_gprs_single_data_ptr->enable_single_block_receive)
  {
    uint8 i;
    /*  Receive functionality required in single block Mode.
        Receive all blocks on the PDTCH used to send the single block.  */
    if (!gpl1_gprs_single_data_ptr->enable_sb_dl_callbacks_setup)
    {
      gl1_defs_rx_pkt_alloc_type  alloc_type[GL1_DEFS_MAX_ASSIGNED_DL_TS];
      gl1_defs_rx_pkt_cb_type     sb_dl_callbacks;

      /*  Configure receive callbacks for single block mode */
      gpl1_gprs_single_data_ptr->sb_dl_data.msg_data_mem = &gpl1_gprs_single_data_ptr->msg_data_mem[0];

      for (i=0; i<4; i++)
      {
        gpl1_gprs_single_data_ptr->sb_dl_data.metrics_data_mem[i] = &gpl1_gprs_single_data_ptr->metrics_data_mem[i];
      }

      sb_dl_callbacks.data_callback = sb_dl_data_cb;
      sb_dl_callbacks.metrics_callback = sb_dl_metrics_cb;
      sb_dl_callbacks.discarded_callback = NULL;
      sb_dl_callbacks.log_callback     = NULL;


      /* preset alloc struct to unallocated */
      for(i=0;i<GL1_DEFS_MAX_ASSIGNED_DL_TS;i++)
      {
        alloc_type[i].usf = GL1_DEFS_PDCH_UNALLOCATED;
        alloc_type[i].tn = GL1_DEFS_TN_0;
      }

      /* Configure NPL1 for rx handler */
      gl1_msg_cfg_dl_pacch_pdtch(
                              alloc_type,
                              1,
                              freq_info_ptr->tsc,
                              FALSE,
                              &sb_dl_callbacks,
                              gas_id
                            );

      /*  Set flag to ensure this only happens the once   */
      gpl1_gprs_single_data_ptr->enable_sb_dl_callbacks_setup = TRUE;

    } /* end of if enable_sb_dl_callbacks_setup */

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS

    //G2X_TA scheduling mechanism below
    pri_fire = gpl1_g2x_ta_fire_sequencer(gas_id);

    if(G2X_TA_PRI_RESERVED == pri_fire )
    {
      MSG_GERAN_LOW_1_G("G2X_TA FIRE SEQUENCER: NOTHING TO DO,TBF FN: %d",gl1_get_FN(gas_id));
    }
    else
    {
      MSG_GERAN_HIGH_2_G("G2X_TA_2P_ACCESS FIRE SEQUENCER: FIRE SEQ:%d ,TBF FN: %d", pri_fire, gl1_get_FN(gas_id));

      if(gpl1_g2x_ta_register_seq(pri_fire, gas_id))
      {
        gpl1_g2x_ta_set_ongoing(pri_fire, gas_id);
      }
    }
    gpl1_g2x_ta_seq[gas_id](gas_id);

#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

#ifdef FEATURE_GTA_2PHASE_ACCESS
    gpl1_gprs_check_susp_FN_gta_for_tds(gas_id);
    if(gta_2p_susp_info[gas_id].tick_susp_state_machine)
    {
      MSG_GERAN_LOW_0_G("2PHASE_ACCESS: Tick G2X state machine");
      gpl1_g2x_trm_unlock_for_X(gas_id);
    }
#endif
    /*  The following switch statement determines the receive actions
        to be taken for the specified frame number. The action has to
        be actioned by PL1 in frame 2 or frame 3 of the block before the
        block the action is specified for. e.g. B1 is actioned in frame 2,
        mod52 in this switch statement.  */
    switch (frame_counters[gas_id].FNmod13)
    {
      /*  Read all blocks.  */
      case 3:     /* B1, B4, B7, B10 */
      case 7:     /* B2, B5, B8, B11 */
      case 12:    /* B0, B3, B6, B9 */
      {
        /*  Determine arfcn whether or not implementing
            frequency hopping    */
        gpl1_gprs_calculate_arfcns(l1_get_gprs_arfcns(gas_id), freq_info_ptr, NORM,gas_id);
#ifdef FEATURE_GTA_2PHASE_ACCESS
        MSG_GERAN_HIGH_1_G("2PHASE_ACCESS: Check for disable_rx: %d", gta_2p_susp_info[gas_id].disable_rx);
        if(FALSE == gta_2p_susp_info[gas_id].disable_rx)
#endif

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
        if (!transfer_data_ptr->disable_rx_tx)
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

        {

          if (l1_transfer_data[gas_id].starting_time_valid)
          {

            /* Used to stop scheduling RX's if we are within 7 frames from a new assignmnet */
            if (!(((uint32)gpl1_gprs_adjust_mod_num(l1_transfer_data[gas_id].starting_time, -(int32)FN,
                                FRAMES_IN_HYPERFRAME) <= 7) ||
                        ((uint32)gpl1_gprs_adjust_mod_num(FN, -(int32)(l1_transfer_data[gas_id].starting_time),
                                FRAMES_IN_HYPERFRAME) <= 7)))
            {
              if(mode == L1_MODE_RUN)
              {
                MSG_GERAN_LOW_3_G(" sched rx ts %d  slammed %d [%d]",single_blk_ptr->ts_number,slammed_slot,gl1_get_FN( gas_id ));

                /* Update the camp timeslot */
                gl1_msg_pdch_set_camp_tn( (gl1_defs_tn_type) l1_get_current_timeslot(gas_id),gas_id );

                gl1_msg_rx_pacch_pdtch( (gl1_defs_tn_type)slammed_slot,
                                        l1_get_gprs_arfcns(gas_id),
                                        &transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                                        &transfer_data_ptr->packet_transfer_div_agc,
#endif
                                        &gpl1_gprs_single_data_ptr->sb_dl_data,gas_id);


              }
            }

          }
          else
          {
            if(mode == L1_MODE_RUN)
            {
              MSG_GERAN_MED_3_G(" sched rx ts %d  slammed %d [%d]",single_blk_ptr->ts_number,slammed_slot,gl1_get_FN( gas_id ));

              /* Update the camp timeslot */
              gl1_msg_pdch_set_camp_tn( (gl1_defs_tn_type) l1_get_current_timeslot(gas_id),gas_id );

              gl1_msg_rx_pacch_pdtch( (gl1_defs_tn_type)slammed_slot,
                                      l1_get_gprs_arfcns(gas_id),
                                      &transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                                      &transfer_data_ptr->packet_transfer_div_agc,
#endif
                                      &gpl1_gprs_single_data_ptr->sb_dl_data,gas_id);


            }
          }

        }
        /*  Set up the frame number the dl block start should be set    */
        gpl1_gprs_single_data_ptr->pacch_pdtch_dl_blk_start_cnt = 0;

        /*  Setup Frame number and timeslot for PH_DATA_IND message  */
        sb_dl_l1_pbcch_pccch.instance_ptr_in->fn =
                            (uint32)gpl1_gprs_adjust_mod_num(FN, 1, FRAMES_IN_HYPERFRAME);
        sb_dl_l1_pbcch_pccch.instance_ptr_in->tn = (uint8)single_blk_ptr->ts_number;
        /*  Manage pointer  */
        (sb_dl_l1_pbcch_pccch.instance_ptr_in == &(sb_dl_l1_pbcch_pccch.instance_3)) ?
              (sb_dl_l1_pbcch_pccch.instance_ptr_in = &(sb_dl_l1_pbcch_pccch.instance_1)) :
              (sb_dl_l1_pbcch_pccch.instance_ptr_in++);


        break;
      } /*  End of block start case */

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS

#ifdef FEATURE_GPRS_GBTA
      case  11 :
      {
        gas_id_t idle_gas_id;
        
        {
          switch( frame_counters[gas_id].FNmod52 )
          {
            case 11:
            case 37:
            {
              /* Checks other gas_id Event status and it is not leaving Idle */
              if(gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_ACTIVE,gas_id,&idle_gas_id))
              {
                idle_gas_id =	check_gas_id(idle_gas_id);
                if (l1_sc_get_monscan_active(idle_gas_id))  // Referring to IDLE SUB l1_sc
                {
                  gl1_gbta_scheduling_idle_event(GBTA_EV_IDLE_PWR_MON,idle_gas_id);
                  
                  /* change state only Transfer sub managed to load the handler */  
                  if(l1_get_idle_monitors_in_progress(idle_gas_id))
                  {
                    MSG_GERAN_MED_2_G("G2X_TA_2P_ACCESS: GL1 GBTA Idle Power monitor scheduled FN %d idle_gas_id %d",FN,idle_gas_id);
                    gl1_set_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED,idle_gas_id);
                  }
                  else
                  {
                    MSG_GERAN_HIGH_3_G("G2X_TA_2P_ACCESS:GL1 GBTA Idle Power monitor abandoned FN %d idle_gas_id %d IS_SUB_LEAVING_IDLE %d",
                    FN,idle_gas_id,IS_SUB_LEAVING_IDLE(idle_gas_id));			 
                    /* Reset when moving to idle */
                    grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                  }
                }
                else
                {
                  MSG_GERAN_HIGH_2_G("G2X_TA_2P_ACCESS:GL1 GBTA Idle Power monitor abandoned moving to IDLE FN %d idle_gas_id %d ", FN,idle_gas_id);		
                  /* Reset when moving to idle  */
                  grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                }
              }
            }
            break;
            
            /* Idle FRAME */
            case 24 :
            case 50 :
            {
              /* check if idle frame free and mon canbe scheduled */
              if (gprs_serv_cell_meas_ptr->idle_frame_free)
              {
                gpl1_gbta_schedule_idle_pwr_mon(gas_id);
                
                if( gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_ACTIVE,gas_id,&idle_gas_id))
                {
                  idle_gas_id =  check_gas_id(idle_gas_id);
                  if (l1_sc_get_monscan_active(idle_gas_id)) // Referring to IDLE SUB l1_sc
                  {
                    gl1_gbta_scheduling_idle_event(GBTA_EV_IDLE_PWR_MON,idle_gas_id);
                    
                    /* change state only Transfer sub managed to load the handler */ 
                    if(l1_get_idle_monitors_in_progress(idle_gas_id))
                    { 
                      gl1_set_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED,idle_gas_id);
                      MSG_GERAN_MED_2_G("G2X_TA_2P_ACCESS:GL1 GBTA Idle Power monitor scheduled in IDLE FRAME FN %d idle_gas_id %d", FN,idle_gas_id);
                      gprs_serv_cell_meas_ptr->idle_frame_free  = FALSE;			  
                    }
                    else
                    {
                      MSG_GERAN_HIGH_3_G("GL1 GBTA Idle Power monitor abandoned FN %d idle_gas_id %d IS_SUB_LEAVING_IDLE %d",
                      FN,idle_gas_id,IS_SUB_LEAVING_IDLE(idle_gas_id));		
                      /* Reset when moving to idle  */
                      grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                    }
                  }
                  else
                  {
                    MSG_GERAN_HIGH_2_G("G2X_TA_2P_ACCESS:GL1 GBTA Idle Power monitor abandoned moving to IDLE FN %d idle_gas_id %d ", FN,idle_gas_id);		  
                    /* Reset when moving to idle */
                    grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                  }
                }
              }
            }
            break;
            
            default :
            /* do nothing */
            break;
          } /* End of case 11, case 37 */
          
        }/*end */
        
        break;
      } /* End of case 11 */
#endif /*   FEATURE_GPRS_GBTA */
      
      case 25:
      case 51:
      gl1_hw_rf_idle_frame_processing(gas_id);
      break;

#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
      default:
        break;

    } /* end of switch */

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS

#ifdef FEATURE_G2X_TUNEAWAY
        if ( gl1_msg_get_multi_sim_mode() )
        {
          /* In Dual sim operation only perform the frame tick when firmware has been downloaded
          this implies that the lock is granted too */
          if ( l1_tskisr_blk->firmware_loaded == TRUE )
          {
            /*	Do Neighbour cell measurement and sync processing */
            gpl1_gprs_measurements(mode, gas_id);
          }
          else
          {
            MSG_GERAN_HIGH_1_G("G2X_TA_2P_ACCESS: No FW gprs_measurements FN %d",FN);
          }
        }
        else
#endif /* FEATURE_G2X_TUNEAWAY */
        /*	Do Neighbour cell measurement and sync processing */
        {
          gpl1_gprs_measurements(mode , gas_id);
        }
              
        /* During the radio block period preceding the IDLE frame,
              * need to wait till the transmit callback has been trigerred
              * NPL1 so as to convey the transmit timeslot information to
              * the neighbour cell scheduling routines.
              *
              * This needs to happen on the second tdma frame of the block
              * since this is the frame at which transmit callback happens
              * and at which the voting on the next neighbour cell to be
              * acquired occurs.
              *
              * Note: Order is vital, must happen after the frame tick
              * notification to the lower layers. This funcionality is
              * extracted out of the gpl1_gprs_measurements( ) function.
              */
        switch( frame_counters[gas_id].FNmod52 )
        {
          /*1 frames before calling trans tick, check if next index to be scheduled is LTE one*/ 
          case 21:
          case 47:
          {
            if (mode == L1_MODE_RUN)
            {
              if (!stop_sc_tick_trans[gas_id])
              {
                /*Check if Gl1 is going to schedule G2L search at next frame*/
                l1_sc_check_if_next_cm_search_is_lte(gas_id);
              }
            }
          }
          break;
              
#ifdef FEATURE_G2X_TUNEAWAY
          case 9:
          case 35:
          if ( !gl1_msg_get_multi_sim_mode() )
          {
            /* Do nothing here in single sim mode when FNmod52 equals 9 or 35 */
            break;
          }
          /* fall through */
#endif
          
          case 22:
          case 48:
          {
            if (mode == L1_MODE_RUN)
            {
              if (!stop_sc_tick_trans[gas_id])
              {
#ifdef FEATURE_G2X_TUNEAWAY
                if ( gl1_msg_get_multi_sim_mode() )
                {
                  if( gl1_check_any_other_subs_l1_state( gas_id, L1_SUSPEND_MODE ) )
                  {
                  MSG_GERAN_HIGH_0_G("Don't Abort TBF in GtoW mode");
                  }
                  else
                  {
                  gpl1_gprs_sc_trans_tick(gas_id);
                  }
                }else
#endif
                {
                  gpl1_gprs_sc_trans_tick(gas_id);
                }
              
              } else
              {
                /* Need to preserve prediction of next NCELL SCH */
                gpl1_gprs_predict_next_sch(gas_id);
                MSG_GERAN_MED_1_G("SCE stopped [%d]",gl1_get_FN( gas_id ));
              }
              
              /*Below check allows GBTA for scenario when transfer init happened
              *before other G sub actualy moved from INACTIVE_MODE to NULL/Idle but 
              *moved to IDLE after that*/
#ifdef FEATURE_GPRS_GBTA
              if( gl1_msg_get_multi_sim_standby_mode() &&
                 IS_GBTA_INACTIVE() && 
                 GBTA_ALLOWED_CAN_BE_SET(gas_id) &&
                 gl1_gbta_get_nv_gbta_support(gas_id) &&
                 (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) &&
                 (!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id))) && // TRM now has tech specfic dr status
                 (trans[gas_id].tuneaway_and_ncell_data_initialized == TRUE)
              )
              {
                SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
                
                /* For this SUB we are not interested in IDLE state vars */
                grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);
                
                MSG_GERAN_MED_0_G("G2X_TA_2P_ACCESS: GL1 GBTA : GBTA ALLOWED other G sub came out of INACTIVE_MODE");
              }
#endif  /*FEATURE_GPRS_GBTA */
              
            }
          }
          break;
          
          case 23:
          case 49:
            
            /* Reset flag to activate G2X  */
            /* Reason for resetting flag here is functions which schedules G2X activity
            l1_sc_wcdma_xfer_ready called multiple times at FN = 22 */
            /* FNmod26 =22, GL1 schedules IRAT, reinitialise l1_tds_modified_ts_fn on next frame*/
            
            l1_sc_set_skip_g2x_pkt_txfr(FALSE,gas_id);	/* allow G2X after TS is reconfigured */
            
            break;
            default:
            break;
         }
         
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

    /*  Check if any RRBP has to be sent  */
    fn_num = gpl1_gprs_adjust_mod_num(FN,1,FRAMES_IN_HYPERFRAME);

    rrbp_index = gpl1_gprs_get_matching_rrbp(fn_num, fn_num, GPL1_INVALID_TN, gas_id);

      /*  RRBP has been requested, command NPL1 */
    if (rrbp_index < SIZE_OF_RRBP_BUFF)
    {
       MSG_GERAN_MED_2_G("RRBP %d, %d", fn_num, rrbp_index);

       l1_transfer_data[gas_id].last_rrbp_msg_type = gpl1_gprs_single_tx_rrbp(rrbp_index,freq_info_ptr, gas_id);

      /*  Set ul_block to next frame  */
      gpl1_gprs_single_data_ptr->pacch_pdtch_ul_blk_start_cnt = 0;

      /* Have we just sent our last pca ? */
      if ((transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) &&
         (!gpl1_gprs_get_ul_num_rrbp_msg(gas_id)))
      {

        if (!gpl1_gprs_single_data_ptr->pacch_pdtch_ul_blk_start_cnt)
        {
          /* Ensure we only set the fn for the end of the pca once !! */
          if (!transfer_data_ptr->wait_for_pca_valid)
          {
            uint32 temp_fn = gl1_get_FN( gas_id );
            transfer_data_ptr->wait_for_pca_fn =  ADD_FN(temp_fn,6);
            transfer_data_ptr->wait_for_pca_valid = TRUE;
            MSG_GERAN_LOW_2_G("OLD leaving idle FN [%d] leave fn %d",gl1_get_FN( gas_id ),transfer_data_ptr->wait_for_pca_fn);
          }
          else
          {
            MSG_GERAN_LOW_1_G("**** pca_valid still true [%d]",gl1_get_FN( gas_id ));
          }

        }
      }



    }

  } /*  End of if enable_single_block_receive */

  if(mode == L1_MODE_STOP)
  {
    if(l1_tskisr_blk->sub_state != L1_SINGLE_BLK_RX)
    {

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
      static boolean sce_idle[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
#endif

      /* To avoid Tx's clashing with RX CCCH's in Idle only abort dl on frames 2,3 or 4
       * and then wait for 2 frames before returning to idle
       */
      switch (frame_counters[gas_id].FNmod13)
      {
        case 0:
        case 1:
        case 5:
        case 9:
          break;

        default:

          if( FALSE != gpl1_gprs_single_data_ptr->abort_once_only )
          {

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
            abort_pending[gas_id] = TRUE;
#endif
            /* Issue the abort to any receive activities */
            gl1_msg_abort_dl_pacch_pdtch(gas_id);
            gl1_msg_abort_dl_ptcch(gas_id);
            gl1_msg_abort_int_meas(gas_id);

            MSG_GERAN_MED_3_G("Abort DL PDTCH FN[%d][%d][%d]",
                    FN, frame_counters[gas_id].FNmod52,frame_counters[gas_id].FNmod13);

            /* Store the FN when abort is supposed to occur */
            gpl1_gprs_single_data_ptr->abort_fn        = frame_counters[gas_id].FNmod13;
            gpl1_gprs_single_data_ptr->abort_once_only = FALSE;

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
            /*Give SCE a chance to abort any receives in progress. */
            l1_sci_tick_receive(gas_id);
#endif

          }
          break;
      }


#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS

      if (abort_pending[gas_id])
      {
#ifdef FEATURE_GPRS_GBTA
        gas_id_t idle_gas_id;
#endif /* FEATURE_GPRS_GBTA*/
      
        sce_idle[gas_id] = !l1_sci_is_op_in_progress(gas_id);
      
#ifdef FEATURE_GPRS_GBTA
        gbta_go_idle = TRUE;
        /* First Sub */
        /* Idle Sub has set Event for Transfer Sub for Power Monitor*/
        if(gl1_check_any_other_subs_idle_evt_power_mon(gas_id,&idle_gas_id))
        {
          /* If power monitors of idle sub is not scheduled then clear the monitor event.
          Otherwise it will be cleared during pwr meas callback.*/
          idle_gas_id =	check_gas_id(idle_gas_id);
          if (! IS_IDLE_EV_PWR_MON_SCHEDULED(idle_gas_id))
          {
            MSG_GERAN_HIGH_1_G("GL1 GBTA IDLE Power monitor abandoned , Transfer Sub going to Idle. idle_sub %d",idle_gas_id);
            /* Reset , otherwise when the same sub goes in transfer it would start scheduling the Mon when when not required */ 	  
            grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
            gbta_go_idle = TRUE;
          }
          else
          {
            gbta_go_idle = FALSE; 
            MSG_GERAN_HIGH_1_G("GL1 GBTA IDLE Power monitor in progress , Transfer Sub going to Idle. idle_sub %d",idle_gas_id);
          }
        }
#endif /* FEATURE_GPRS_GBTA */
      
        /*Check if GL1 is stuck in this state because of unfinished IRAT activity*/
        gl1_check_if_irat_state_stuck(gas_id);
          /*
          Check to see if the SCE has aborted all operations and if a valid abort FN
          has been reached on this frame. If so then the abort is complete and L1 can
          transition to IDLE
          */
      
      if (( FALSE == gpl1_gprs_single_data_ptr->abort_once_only ) &&
          (sce_idle[gas_id] == TRUE) &&
#ifdef FEATURE_GPRS_GBTA
          (gbta_go_idle == TRUE)&&
#endif 
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_abort(FALSE, gas_id) &&
#endif
         (IS_PDCH_CHAN_ABORTED[gpl1_gprs_single_data_ptr->abort_fn] == frame_counters[gas_id].FNmod13))
      {
        GPL1_SET_ABORT_CB( gas_id );
        abort_pending[gas_id] = FALSE;
        
        /*If measurements have been taken in transfer, update the 51 idle value.
        this overcomes the problem with being in transfer for along time and
        the idle serving cell measurement being out of date.   */
        if ((l1_running_avg_get_avg(&(gprs_serv_cell_meas_ptr->rla_p_buffer))))
        {
          l1_serving_cell_meas_ptr->rx_power = l1_running_avg_get_avg(&(gprs_serv_cell_meas_ptr->rla_p_buffer));
          l1_serving_cell_meas_ptr->no_of_measurements = 1;
        }
        idle = TRUE;
        sce_idle[gas_id] = FALSE;
        
        gpl1_transfer_service_pending_sysinfo_reqs(gas_id );
        
        gpl1_gprs_single_data_ptr->abort_once_only = TRUE;
        
        /* turn the PA off in 2 frames to ensure Single block is txd */
        gl1_drx_require_TX(2, gas_id);
        
        if (l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND)
        {
          l1_transfer_data[gas_id].send_l1_mac_single_block_sent = TRUE;
        }
        else
        {
          /* Inform MAC that UL Single block has been sent.
          * This is required since PL1 releases the single block tbf without
          * any interaction between MAC and PL1
          */
          gpl1_gprs_send_l1_mac_single_block_sent(gas_id);
        }
      }
      
    }


#else /* FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

      /* Check if wtr hop is in progress and abort hop. Transfer mode can      */
      /* not move to idle while there is a hop in progress                     */
      if( ( FALSE == gpl1_gprs_single_data_ptr->abort_once_only ) &&
#ifdef FEATURE_GSM_WTR_HOP
            gl1_msg_wtr_hop_abort(FALSE, gas_id) &&
#endif
          ( IS_PDCH_CHAN_ABORTED[ gpl1_gprs_single_data_ptr->abort_fn ] == frame_counters[gas_id].FNmod13 )
        )
      {
        /* Stop the tx callbacks from being actioned*/
        GPL1_SET_ABORT_CB( gas_id );

        /* inform isr that transfer isr activity has stopped */
        idle = TRUE;

        gpl1_gprs_single_data_ptr->abort_once_only = TRUE;

        /* turn the PA off in 2 frames to ensure Single block is txd */
        gl1_drx_require_TX(2, gas_id);

        if (l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND)
        {
          l1_transfer_data[gas_id].send_l1_mac_single_block_sent = TRUE;
        }
        else
        {
           /* Inform MAC that UL Single block has been sent.
            * This is required since PL1 releases the single block tbf without
            * any interaction between MAC and PL1
            */
           gpl1_gprs_send_l1_mac_single_block_sent(gas_id);
        }
      }

#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

    }
    else
    {
      idle = TRUE;
      MSG_GERAN_MED_3_G("L1_SINGLE_BLK_RX Abort DL PDTCH FN[%d][%d][%d]"
              ,FN, frame_counters[gas_id].FNmod52,frame_counters[gas_id].FNmod13);
    }
  } /* End of mode stop */



  /* check if this is the FN where the last PCA of the TBF
   * has been transmitted
   */
  if ((transfer_data_ptr->wait_for_pca_valid) &&
      (gl1_get_FN( gas_id ) == transfer_data_ptr->wait_for_pca_fn))
  {
      /* Clear wait for pca vars */
      transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
      transfer_data_ptr->wait_for_pca_valid = FALSE;

      transfer_data_ptr->pca_sent = TRUE;
      l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

  }

  /* Send Frame tick notification to NPL1 */
  gpl1_gprs_transfer_send_ftn(
                          &gpl1_gprs_single_data_ptr->pacch_pdtch_ul_blk_start_cnt,
                          &gpl1_gprs_single_data_ptr->pacch_pdtch_dl_blk_start_cnt,
                          mode, gas_id );


#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    if (single_blk_ptr->two_phase_access == TRUE && 
    (trans[gas_id].tuneaway_and_ncell_data_initialized == TRUE) &&
    (mode != L1_MODE_STOP))
    {
      gpl1_schedule_pch_in_transfer(gas_id);
    }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
    
    if(((uint32)gpl1_gprs_adjust_mod_num(transfer_data_ptr->starting_time, (NPL1_FN_OFFSET + TEMP_DELAY_OS), FRAMES_IN_HYPERFRAME)
          == FN) && (l1_tskisr_blk->sub_state != L1_SINGLE_BLK_RX) && (mode != L1_MODE_STOP))
    {
      if (single_blk_ptr->two_phase_access)
      {
        MSG_GERAN_HIGH_0_G("2PHASE_ACCESS:Change priority and register CB");
        
        l1_sci_go(gas_id);
        
        gpl1_gprs_init_tuneaway_and_ncell_data(gas_id);
      }
  }

#else /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

#ifdef FEATURE_GTA_2PHASE_ACCESS
  if(((uint32)gpl1_gprs_adjust_mod_num(transfer_data_ptr->starting_time, (NPL1_FN_OFFSET + TEMP_DELAY_OS), FRAMES_IN_HYPERFRAME)
                                                  == FN) && (l1_tskisr_blk->sub_state != L1_SINGLE_BLK_RX))
  {
    if ((single_blk_ptr->two_phase_access) && (gl1_msg_get_multi_sim_mode()))
    {
      MSG_GERAN_HIGH_0_G("2PHASE_ACCESS: Change priority and register CB");
      grm_change_priority(l1_tskisr_blk->client_id, 
                          TRM_ACCESS,
                          gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id), 
                          gas_id);
      grm_retain_lock( l1_tskisr_blk->client_id,
                       (grm_unlock_callback_t)&grm_gta_2phase_access_preemption_callback,
                       gas_id );
    }
  }
#endif /*FEATURE_G2T_2PHASE_ACCESS*/

#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

  /*  Ensure block has been sent */
  if (((uint32)gpl1_gprs_adjust_mod_num(transfer_data_ptr->starting_time, NPL1_FN_OFFSET, FRAMES_IN_HYPERFRAME)
                                                  == FN) && (l1_tskisr_blk->sub_state != L1_SINGLE_BLK_RX))
  {
    /*  then move state to either IDLE or ACCESS
        depending on whether doing two phase access */
    if (single_blk_ptr->two_phase_access)
    {
      /*  Remain in this state and start receiving. */
      /*  Enable single block receive and remain in state.  */
      gpl1_gprs_single_data_ptr->enable_single_block_receive = TRUE;
      l1_transfer_data[gas_id].l1_called_from_sngl_blk = TRUE;

    }
    else
    {
      if((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
         (l1_transfer_data[gas_id].release_pending == L1_NO_RELEASE))
	    {
          /****  Not two phase so return to idle ****/

          /* no release needed so set release cause to sent */
          l1_transfer_data[gas_id].confirm_pending = SENT;
		}

      /*CR822105: Need to set leave_trans_adjust_sync as TRUE when transfer from L1SCModeGprsTrans to L1SCModeIdle.
      * It will increase one frame when slam TS back. Fix one mised scenario of CR789023
      */
      l1_sc_stop(gas_id);

      /* SCE start is CPU intensive, hence call it in the task context */
      l1_scdrv_call_in_task( gpl1_gprs_setup_start_idle, (void *)0, gas_id );

    }
  }else if (((uint32)gpl1_gprs_adjust_mod_num(transfer_data_ptr->starting_time,4, FRAMES_IN_HYPERFRAME) < FN) &&
             (l1_tskisr_blk->sub_state == L1_SINGLE_BLK_RX))
  {
    /* DL control has been received */
    uint8 num_msg;
    boolean release_required=FALSE;

    if(single_blk_ptr->sb_crc_fail != TRUE)
    {
      if(!gpl1_gprs_single_data_ptr->rrbp_to_transmit)
      {
        /* no UL RRBP msg to tranmit */
        num_msg = gpl1_gprs_get_ul_num_rrbp_msg(gas_id);

        if(num_msg)
        {
            /* UL RRBP msg to transmit */
            gpl1_gprs_single_data_ptr->rrbp_to_transmit=TRUE;
            /*  Search rrbp array and set starting time accordingly */
            rrbp_index= gpl1_gprs_check_for_rrbp_msg(TRUE,gas_id);

            MSG_GERAN_MED_2_G("num_msg to tx from sn bk rx=%d rrbp_idx %d", num_msg, rrbp_index);

            if (rrbp_index < SIZE_OF_RRBP_BUFF)
            {
               /* calculate frame to allow drop into idle */
               gpl1_gprs_single_data_ptr->rrbp_end_fn = gpl1_gprs_get_rrbp_frame(rrbp_index, gas_id);
               gpl1_gprs_single_data_ptr->rrbp_end_fn = ADD_FN(gpl1_gprs_single_data_ptr->rrbp_end_fn,2);

            }else
            {
               /* Drop into idle */
               release_required = TRUE;
               MSG_GERAN_HIGH_1_G("L1_SINGLE_BLK_RX release_required,invalid rrbp idx %d", rrbp_index);
            }
        } /* end of if(num_msg) */
      }else if ( gpl1_gprs_single_data_ptr->rrbp_end_fn == FN )
      {
        /* RRBP msg has been transmitted. Drop into idle , in the same tick */
        /* will drop at the time of reception of MAC_L1_REL_REQ and sending REL_CNF */
        MSG_GERAN_MED_1_G("L1_SINGLE_BLK_RX release_required,rrbp tx FN=%u", FN);
        gpl1_gprs_single_data_ptr->rrbp_to_transmit = FALSE;

        l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
        l1_scdrv_call_in_task( gpl1_gprs_setup_start_idle, (void *)0, gas_id);
      }else if ( SUB_FN(gpl1_gprs_single_data_ptr->rrbp_end_fn,4) == FN )
      {
        /* there is an RRBP msg to schedule */
        fn_num=SUB_FN(gpl1_gprs_single_data_ptr->rrbp_end_fn,2);
        rrbp_index = gpl1_gprs_get_matching_rrbp(fn_num, fn_num, GPL1_INVALID_TN, gas_id);

        /*  RRBP has been requested, command NPL1 */
        if (rrbp_index < SIZE_OF_RRBP_BUFF)
        {
           MSG_GERAN_MED_2_G("L1_SINGLE_BLK_RX Schedule RRBP %d, %d", FN, rrbp_index);

           l1_transfer_data[gas_id].last_rrbp_msg_type = gpl1_gprs_single_tx_rrbp(rrbp_index,freq_info_ptr, gas_id);
           /*  Set ul_block to next frame  */
           gpl1_gprs_single_data_ptr->pacch_pdtch_ul_blk_start_cnt = 0;
        }
      }
    }else
    {
      MSG_GERAN_HIGH_0_G("L1_SINGLE_BLK_RX release_required,SB_FAILURE");
      release_required = TRUE;
    }

    /* only done in error case and single block failure */
    if(release_required)
    {
       /* Set release cause to sent */
       l1_transfer_data[gas_id].confirm_pending = SENT;
       /* SCE start is CPU intensive, hence call it in the task context */
       l1_scdrv_call_in_task( gpl1_gprs_setup_start_idle, (void *)0, gas_id);

       gpl1_gprs_single_data_ptr->rrbp_to_transmit=FALSE;
    }

    /* delayed single block failure ensures that L1 is in idle. */
    if (single_blk_ptr->sb_crc_fail == TRUE)
    {
      l1_transfer_data[gas_id].current_alloc_type = L1_NULL_ALLOC;
      single_blk_ptr->sb_crc_fail = FALSE;
      l1_scdrv_call_in_task( gpl1_gprs_send_mac_51_sb_failure, (void *)0, gas_id);
    }
  }


#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
  if(trans[gas_id].tick_wcdma)
  {
    l1_sci_tick_receive(gas_id);
    l1_sci_tick_wcdma_id_in_tbf(30,gas_id);
  }
#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

  return(idle);
} /* end of function */

/*===========================================================================

FUNCTION  gpl1_gprs_tx_single_pdtch_disc_cb

DESCRIPTION
  This function is called by Non Portable Layer 1 when a scheduled PACCH or
  PDTCH is discarded due to higher priority of another PDCH being scheduled
  or the message buffer in the mDSP is full.


  Passes back the pointer to the discarded message.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_tx_single_pdtch_disc_cb( gl1_msg_tx_discarded_data_type  msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
                                        uint8                           num_msg_txd )
{
  MSG_GERAN_MED_1("PL1: Single Block -> DISCARDED CALLBACK Rcvd = 0x%X",*(msgs_discarded[0].msg_data));
} /*lint !e715 : Lint complains about un-referenced symbols */


/*===========================================================================

FUNCTION  gpl1_gprs_tx_single_pdtch_sched_cb

DESCRIPTION
  This function is called by Non Portable Layer 1 when a scheduled PACCH or
  PDTCH is sucessfully transmitted.


  Passes back an array of pointers to the scheduled messages and the number
  of pointers in the array.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_tx_single_pdtch_sched_cb( gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                         uint8                           num_msg_txd, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T               *transfer_data_ptr;
  uint8                         sched_msgs = 0;
  boolean                       found_msg_ptr = FALSE;

  NOTUSED(num_msg_txd);

  MSG_GERAN_LOW_0_G("PL1: Single Block -> SCHEDULED CALLBACK Rcvd");

  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  /*  scan through each element in the mac buffer */
  while((sched_msgs < GL1_DEFS_MAX_UL_MSGS) && (!found_msg_ptr) )
  {

    if(  ( transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[sched_msgs].control_block == msgs_txd[0].msg_data )

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM
       &&( FALSE != transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[sched_msgs].valid )
#endif
#endif
      )

    {
      /****
       **** a MAC UL PACCH msg has been transmitted so update pacch tx'd pointer
       ****/

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM
      /* */
      transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[sched_msgs].valid = FALSE;

#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif


#endif
#endif

      if (++transfer_data_ptr->mac_ul_ctrl_buff_ptr->last_pacch_txd_ptr >
         &(transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[MAX_NUM_UL_PACCH_MSGS -1]) )
      {

        transfer_data_ptr->mac_ul_ctrl_buff_ptr->last_pacch_txd_ptr =
            &transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[0];
      }

      found_msg_ptr = TRUE;

    }
    sched_msgs++;

  }
}



/*===========================================================================

FUNCTION  SB_DL_METRICS_CALLBACK

DESCRIPTION
  This function is the callback function called for each burst, when in Access
  mode and the PCCCH or PBCCH is being read.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sends signal to MAC

===========================================================================*/
void sb_dl_metrics_cb( gl1_defs_rx_pkt_metrics_type *md[GL1_DEFS_MAX_DL_MSGS ],
                       uint8  num_msgs,
                       uint8  burst_num,
                       gas_id_t gas_id )
{
  if( FALSE != md[0]->valid )
  {
    gl1_power_meas_type pwr_meas_type = PRX_MEAS;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
    dBx16_T  divrx_rssi = GL1_INVALID_POWER_LEVEL;

    if (gl1_get_MSRD_ctl_nv(gas_id)) 
    {
      pwr_meas_type = PRX_DIVRX_MEAS;

      if(md[0]->pwr_dBm_x16_divrx != GL1_INVALID_POWER_LEVEL)
      {
        divrx_rssi = md[0]->pwr_dBm_x16_divrx;
      }
      else
      {
        /* DivRX pwr is invalid, use PRX power to keep running average counters/
           variables in sync. */
        divrx_rssi = md[0]->pwr_dBm_x16;
      }
    }
    else if (gl1_get_IMRD_ctl_nv(gas_id)) 
    {
      pwr_meas_type = PRX_DIVRX_MEAS;
      divrx_rssi = md[0]->pwr_dBm_x16;
    }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
    /*  Serving Cell Measurements */
    gpl1_scell_pkt_transfer_metrics_cb( md[0]->arfcn.num,
                                        burst_num,
                                        md[0]->pwr_dBm_x16, 
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
                                        divrx_rssi,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                        pwr_meas_type,
                                        gas_id );

    /* Calculate the absolute timeslot for logging */
    md[0]->tn = (gl1_defs_tn_type) gpl1_gprs_ts_conv_unslam( (uint8)md[0]->tn, gas_id );

    /* Log burst metrics */
    gpl1_log_gprs_burst_metrics_a(GL1_MSG_DL_PACCH_PDTCH,gl1_get_FN( gas_id ),md[0],gas_id);
    gpl1_log_gprs_burst_metrics_a_ver2(GL1_MSG_DL_PACCH_PDTCH,gl1_get_FN( gas_id ),md[0],gas_id);
  }
  else
  {
    MSG_GERAN_ERROR_1_G(" Invalid pdch burst %u metrics ", burst_num);
  }

} /*lint !e715 num_msgs un-referenced */

/*===========================================================================

FUNCTION  SB_DL_DATA_CB

DESCRIPTION
  This function is the callback function called when in Access mode and the
  PCCCH or PBCCH has been recieved and decoded.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sends signal to MAC

===========================================================================*/
void sb_dl_data_cb (gl1_defs_rx_pkt_data_type  *msg_data,
                    boolean                    cb_valid, gas_id_t gas_id)
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_mac_sig_t      message_buffer,*message;
  dBx16_T           pr_value=0;
  boolean           message_valid = FALSE;
  dword             FN = gl1_get_FN( gas_id );
  uint8             unslamed = 0;
  transfer_data_T   *transfer_data_ptr;
  usf_dl_tfi_info_t  usf_dl_tfi_info =
      { FALSE, FALSE, 0xffff, 0xffff, 0xffff, GL1_DEFS_PDCH_UNALLOCATED };
  gpl1_gprs_decode_metrics_T  gprs_decode_metrics;

  /* assign shortcut pointer's for use in function */
  message = &message_buffer;

  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  unslamed = (uint8) (msg_data->tn) + (uint8) (msg_data->camp_tn);

  msg_data->tn = (gl1_defs_tn_type) unslamed;

  message->msg.data_ind.timeslot = unslamed;

  /*  Check message has been decoded correctly, by checking CRC bit  */
  if (msg_data->msg[0].crc_passed)
  {
    MSG_GERAN_HIGH_3_G("SINGLE decode successful, received Data Block FN %u, header:%2x, cs=%d", 
                        FN,
                        msg_data->msg[0].data,
                        msg_data->coding_scheme);

    /* check the payload and coding scheme to ensure only PACCH control data is sent to MAC */
    if (( CHECK_PAYLOAD_TYPE(msg_data->msg[0].data) != L1_DATA_BLOCK) &&(msg_data->coding_scheme == GL1_DEFS_CS1_CODING))
    {
      MSG_GERAN_LOW_1_G("SINGLE decode OK FN %u", FN);

      /* Set up the message header */
      message->task_id = MS_MAC_L1;
      message->sig_id = MAC_PH_DATA_IND;

      /* Frame number PBCCH data on */
      message->msg.data_ind.frame_no = sb_dl_l1_pbcch_pccch.instance_ptr_out->fn;

      /* Timeslot number PBCCH data on */
      message->msg.data_ind.timeslot = sb_dl_l1_pbcch_pccch.instance_ptr_out->tn;

      message->msg.data_ind.gprs_l1_page_group_flag = FALSE;

      message->msg.data_ind.bcch_arfcn = l1_idle_data->campedon_cell_ARFCN;

      message->msg.data_ind.gas_id = gas_id;

      /* Set up the data in the message body */
      L1_send_gprs_gmac_message (message,msg_data->msg[0].data, gl1_get_FN(gas_id), gas_id);

      /* To update agc, set valid flag as this message is for us */
      message_valid = TRUE;

      /* Populate Power Reduction from decoded message if downlink power control
       * is enabled
       */
      if (transfer_data_ptr->dl_power_control.params_valid)
      {

        pr_value = gpl1_gprs_get_pr_value(msg_data->msg[0].data);

      } else
      {

        pr_value = 0;

      }

    }else
    {
      /* Decoded RLC data, no DL TBF present so ignore msg */
      message_valid = FALSE;

      MSG_GERAN_HIGH_1_G("SINGLE decode CRC failure sb_ul_dl_type %d",transfer_data_ptr->single_block_tbf_ptr->sb_ul_dl_type);

      if (transfer_data_ptr->single_block_tbf_ptr->sb_ul_dl_type == SINGLE_DL_BLOCK)
      {
         /* Defer the single block failure and send it before going to idle,
            ** this is done to prevent MAC sending RACH_REQ before L1 going to idle.
         */
         transfer_data_ptr->single_block_tbf_ptr->sb_crc_fail = TRUE;
         MSG_GERAN_ERROR_3_G("SINGLE decode unsuccessful -- received Data Block FN %u, header:%2x, cs=%d", 
                   FN,
                   msg_data->msg[0].data,
                   msg_data->coding_scheme);
      }
    }
  }
  else
  {
    MSG_GERAN_HIGH_1_G("SINGLE decode unsuccessful FN %u", FN);

    /* To update agc unset valid flag as this message is for us */
    message_valid = FALSE;

    if (transfer_data_ptr->single_block_tbf_ptr->sb_ul_dl_type == SINGLE_DL_BLOCK)
    {
      /* Defer the single block failure and send it before going to idle,
      ** this is done to prevent MAC sending RACH_REQ before L1 going to idle.
      */
      transfer_data_ptr->single_block_tbf_ptr->sb_crc_fail = TRUE;
    }

  }


  /*  Manage pointer  */
  (sb_dl_l1_pbcch_pccch.instance_ptr_out == &(sb_dl_l1_pbcch_pccch.instance_3)) ?
             (sb_dl_l1_pbcch_pccch.instance_ptr_out = &(sb_dl_l1_pbcch_pccch.instance_1)) :
             (sb_dl_l1_pbcch_pccch.instance_ptr_out++);

  /* Update AGC on first block after start of PTM */
  sys_algo_agc_process_pdtch(&transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                             &transfer_data_ptr->packet_transfer_div_agc,
#endif
                             (gl1_defs_tn_type)gpl1_gprs_ts_conv((uint8)msg_data->tn,gas_id),
                             pr_value,
                             message_valid, gas_id);




  gprs_decode_metrics.coding_type        = msg_data->coding_scheme;
  gprs_decode_metrics.rxqual             = msg_data->msg[0].rx_qual;
  gprs_decode_metrics.header_decode_ok   = msg_data->msg[0].crc_passed;
  gprs_decode_metrics.tfi_match          = FALSE;

  gprs_decode_metrics.data_payload_error = !(msg_data->msg[0].crc_passed);

  /*  Serving Cell Measurements */
  gpl1_scell_pkt_transfer_data_cb( msg_data->tn,
                                   &gprs_decode_metrics,
                                   gas_id );

  /* Log Message metrics */
  gpl1_log_gprs_message_metrics_a_ver2(GL1_MSG_DL_PACCH_PDTCH,gl1_get_FN( gas_id ),msg_data, &(usf_dl_tfi_info), gas_id);

} /*lint !e715: Symbol cb_valid not referenced */
  /* End of function sb_dl_data_cb */


/*===========================================================================

FUNCTION  gpl1_gprs_send_L1_MAC_SINGLE_BLOCK_SENT

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST.

 This function posts the L1_MAC_SINGLE_BLOCK_SENT message on the MAC
 or TEST queue to inform the MAC layer that the Single block message
 i.e. PCCF or PMR has been transmitted

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_l1_mac_single_block_sent(gas_id_t gas_id)
{

  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = L1_MAC_SINGLE_BLOCK_SENT;
  message->msg.sb_sent.gas_id = gas_id;

  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);

}

/*===========================================================================

FUNCTION gpl1_single_block_data_init

DESCRIPTION
  Resets any flags and variables critical to single block transfer operation.

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void gpl1_single_block_data_init( gas_id_t gas_id )
{
  gpl1_gprs_single_data_t *gpl1_gprs_single_data_ptr = &gpl1_gprs_single_data[gas_id];
  gpl1_gprs_single_data_ptr->abort_once_only = TRUE;
}

#ifdef FEATURE_GTA_2PHASE_ACCESS
/*===========================================================================

FUNCTION gpl1_g2x_trm_unlock_for_T

DESCRIPTION
             State machine for G2T GTA gap during 2 Phase Access

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void gpl1_g2x_trm_unlock_for_X(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
 // transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  idle_data_T *idle_data_ptr      = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gpl1_gprs_single_data_t *gpl1_gprs_single_data_ptr = &gpl1_gprs_single_data[gas_id];
  static int8 inactivity_timer[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };
  boolean allowed_to_go = TRUE;
  boolean geran_tuneaway_allowed = FALSE;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  MSG_GERAN_LOW_2_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X at FN=%d, sub_state:%d",gl1_get_FN(gas_id), g2t_ta_seq_trm_state[gas_id]);
  switch( g2t_ta_seq_trm_state[gas_id] )
  {
    case G2X_TA_DISABLE_RXTX_NULL:
    {
      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      if ( (trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
      {
          if ((TRUE == gl1_is_geran_tuneaway_allowed(gas_id)) &&
              (FALSE == idle_data_ptr->non_updated_lai_in_nmo1))
          {
            geran_tuneaway_allowed = TRUE;
          }

          MSG_GERAN_HIGH_2_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X veto=%d, drx_mode:%d",!gl1_is_geran_tuneaway_allowed(gas_id), idle_data_ptr->mm_non_drx_mode);

          if ( FALSE != geran_tuneaway_allowed )
          {
              if (l1_transfer_data[gas_id].starting_time_valid)
              {
                uint32  FN_diff = SUB_FN( l1_transfer_data[gas_id].dynamic_tbf_1.starting_time, gta_2p_susp_info[gas_id].pch_suspension_FN );

                if( ( FN_diff > G2X_TUNEAWAY_FN_DIFF_THRESHOLD )
					&& ( FN_diff < (FRAMES_PER_HYPERFRAME/2) )
				  )
                {
                    suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY;
                    gprs_serv_cell_meas_ptr->suspended_tbf = TRUE;
                    gl1_msg_abort_dl_pacch_pdtch(gas_id);
                    gl1_msg_abort_dl_ptcch(gas_id);
                    gl1_msg_abort_int_meas(gas_id);
                    gta_2p_susp_info[gas_id].disable_rx = TRUE;
                    g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_SUSPENDING;

                    MSG_GERAN_MED_3_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X G2X_TA_DISABLE_RXTX_NULL exit at FN=%d st valid %d fn diff %u ",
				                      gl1_get_FN(gas_id),
					              l1_transfer_data[gas_id].starting_time_valid,
									  FN_diff
								     );
                }
                else
                {
                    gta_2p_susp_info[gas_id].disable_rx = FALSE;
                    gta_2p_susp_info[gas_id].pch_suspension_FN = L1_GPRS_INVALID_FN;
                    gta_2p_susp_info[gas_id].tick_susp_state_machine = FALSE;

                    MSG_GERAN_HIGH_2_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X ST: %d, SuspFn: %d", l1_transfer_data[gas_id].starting_time, gta_2p_susp_info[gas_id].pch_suspension_FN);
                }
              }
              else
              {
                  suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY;
                  gprs_serv_cell_meas_ptr->suspended_tbf = TRUE;
                  gl1_msg_abort_dl_pacch_pdtch(gas_id);
                  gl1_msg_abort_dl_ptcch(gas_id);
                  gl1_msg_abort_int_meas(gas_id);
                  gta_2p_susp_info[gas_id].disable_rx = TRUE;
                  g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_SUSPENDING;

                  MSG_GERAN_MED_2_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X G2X_TA_DISABLE_RXTX_NULL exit at FN=%d st valid %d ",
				                    gl1_get_FN(gas_id),
								    l1_transfer_data[gas_id].starting_time_valid
								   );
              }

          } /* if ( FALSE != geran_tuneaway_allowed ) */
      } /*if ( (l1_tskisr_blk->trm_status == TRM_GRANTED) || (l1_tskisr_blk->trm_status == TRM_RETAINED_FOR_ACCESS) )*/
   }
   break;
   case G2X_TA_DISABLE_RXTX_SUSPENDING:
   {
      MSG_GERAN_MED_1_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X G2X_TA_DISABLE_RXTX_SUSPENDING at FN=%d",gl1_get_FN(gas_id));
      if (--suspension_timer[gas_id] == 0)
      {
          if (!gl1_is_geran_tuneaway_allowed(gas_id) || (idle_data_ptr->non_updated_lai_in_nmo1))
          {
            MSG_GERAN_HIGH_0_G("MULTISIM: G2X is blocked by upper layers");
            allowed_to_go = FALSE;
          }

          if( allowed_to_go )
          {
            grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
            grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;
            
            gl1_hw_schedule_reset_events(gas_id);
            
            inactivity_timer[gas_id] = G2X_TUNEAWAY_2PHASE_ACCESS_TIMEOUT; // 85 ms
            
            grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;
            grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
            grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(TBF_MIN_DURATION_MS, T_MSEC); /*To get the lock as soon as TDS releases*/
            grm_request_and_notify_enhanced_data_p->reason    = TRM_ACCESS;
            
            grm_request_and_notify_enhanced_data_p->resource  =
            gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );
            
            /*using the tbf supend tag as fo now as it only sets the trm status to granted and doesn nothing else on callback*/
            grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_TBF_SUSPEND; 
            
            grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);
            
            gl1_hw_multi_sim_deact_g2x(gas_id);
            
            /*release the diversity chain too*/
            grm_release_rx_sec(grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,gas_id), gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_TA_IN_2PHASE_ACCESS);
#endif

            grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p->client_id,
                                             grm_request_and_notify_enhanced_data_p->resource,
                                             grm_request_and_notify_enhanced_data_p->duration,
                                             grm_request_and_notify_enhanced_data_p->reason,
                                             gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id),
                                             grm_request_and_notify_enhanced_data_p->grm_tag );
            
            l1_tskisr_blk->firmware_loaded = FALSE;

            g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_TRM_WAIT;
            
             MSG_GERAN_HIGH_1_G("2PHASE_ACCESS L1_DS_DISABLE_RXTX_SUSPENDED go Wait for TRM %d",gl1_get_FN(gas_id));
          }
          else
          {
            g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
            gta_2p_susp_info[gas_id].disable_rx = FALSE;
            gta_2p_susp_info[gas_id].pch_suspension_FN = L1_GPRS_INVALID_FN;
            gta_2p_susp_info[gas_id].tick_susp_state_machine = FALSE;
            MSG_GERAN_HIGH_1_G("2PHASE_ACCESS Abort TBF Suspension %d",gl1_get_FN(gas_id));
          }
        }
        else
        {
          MSG_GERAN_HIGH_1_G("suspension timer running %d",gl1_get_FN(gas_id));
        }
   }
   break;
   case G2X_TA_DISABLE_RXTX_TRM_WAIT:
   {
      MSG_GERAN_MED_1_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_T G2X_TA_DISABLE_RXTX_TRM_WAIT at FN=%d",gl1_get_FN(gas_id));
      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      if((trm_status == GL1_TRM_GRANTED)||(trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
      {
        if( l1_tskisr_blk->firmware_loaded == FALSE )
        {
            g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_FW_LOAD_WAIT;
            fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
            gl1_multi_sim_fw_rf_load(l1_tskisr_blk->client_id, gas_id);
            break;
        }
      }
      else if( --inactivity_timer[gas_id] <= 0)
      {
          gta_2p_susp_info[gas_id].pch_suspension_FN = L1_GPRS_INVALID_FN;
          gta_2p_susp_info[gas_id].tick_susp_state_machine = FALSE;
          g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL;
          MSG_GERAN_HIGH_2_G("L1 send RR_DS_ABORT_TIMEDOUT fn %d inact timer %d", gl1_get_FN( gas_id ),inactivity_timer[gas_id]);
          L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,200);
          gpl1_gprs_single_data_ptr->abort_once_only = FALSE;
          call_in_task_grm_release( l1_tskisr_blk->client_id );
      }
   }
   break;
   case G2X_TA_DISABLE_RXTX_FW_LOAD_WAIT:
   {
      MSG_GERAN_MED_1_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X G2X_TA_DISABLE_RXTX_FW_LOAD_WAIT at FN=%d",gl1_get_FN(gas_id));
      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      if ( fw_rf_setting_up[gas_id] > 0 )
      {
          fw_rf_setting_up[gas_id]--;
      }
      if( l1_tskisr_blk->firmware_load_status == FW_LOAD_IN_PROGRESS )
      {
          MSG_GERAN_HIGH_0_G("FW_LOAD_IN_PROGRESS");
      }
      else if( trm_status == GL1_TRM_ABORTING ||
               l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
      {
          if( fw_rf_setting_up[gas_id] == 0 )
          {
              g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
              call_in_task_grm_release( l1_tskisr_blk->client_id );
              MSG_GERAN_HIGH_0_G("DS TBF CCCH Process Aborted");
          }
          else
          {
              MSG_GERAN_HIGH_1_G("Delay trm release FN = %d",gl1_get_FN( gas_id ));
          }
      } 
      else if( l1_tskisr_blk->firmware_loaded == TRUE )
      {
          gl1_hw_set_idle_mode(FALSE,gas_id);
      }
      gl1_hw_turn_tx_on(gas_id);
      MSG_GERAN_HIGH_0_G("waiting for rf/fw warmup time");
      g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
      gta_2p_susp_info[gas_id].pch_suspension_FN= L1_GPRS_INVALID_FN;
      gta_2p_susp_info[gas_id].tick_susp_state_machine = FALSE;
      gta_2p_susp_info[gas_id].disable_rx = FALSE;
      grm_retain_lock( l1_tskisr_blk->client_id,
                       (grm_unlock_callback_t)&grm_gta_2phase_access_preemption_callback,
                       gas_id );
      
   }
   break;
   default:
      MSG_GERAN_MED_1_G("2PHASE_ACCESS gpl1_g2x_trm_unlock_for_X INVALID state at FN=%d",gl1_get_FN(gas_id));
   break;
  }
  gl1_drx_require_next_tick(gas_id);
}
/*===========================================================================

FUNCTION gpl1_gprs_check_susp_FN_gta_for_tds

DESCRIPTION
             This function compares suspension FN with current FN to decide when its time to start aborting
             and prepare for GTA gap.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE boolean

===========================================================================*/
void gpl1_gprs_check_susp_FN_gta_for_tds(gas_id_t gas_id)
{
    if(gta_2p_susp_info[gas_id].pch_suspension_FN != L1_GPRS_INVALID_FN && (IS_FRAME_NUM_LATER(gta_2p_susp_info[gas_id].pch_suspension_FN, gl1_get_FN(gas_id))))
    {
        if(SUB_FN( gta_2p_susp_info[gas_id].pch_suspension_FN, NPL1_FN_OFFSET ) == GSTMR_GET_FN_GERAN(gas_id))
        {        
            gta_2p_susp_info[gas_id].tick_susp_state_machine = TRUE;
            MSG_GERAN_HIGH_1_G("Start ticking suspension state machine, FN: %d", GSTMR_GET_FN_GERAN(gas_id));
        }
    }
}

/*===========================================================================
FUNCTION gpl1_gprs_reset_G2T_2phase_state_machine

DESCRIPTION
             This function reset variables related to G2T 2 Phase Access.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE boolean

===========================================================================*/
void gpl1_gprs_reset_G2T_2phase_state_machine(gas_id_t gas_id)
{
    MSG_GERAN_HIGH_1_G("2PHASE_ACCESS: Resetting G2x variables, FN: %d", GSTMR_GET_FN_GERAN(gas_id));

   gta_2p_susp_info[gas_id].pch_suspension_FN = L1_GPRS_INVALID_FN;
   gta_2p_susp_info[gas_id].tick_susp_state_machine = FALSE;
	g2t_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL;
}

/*===========================================================================

FUNCTION gpl1_gprs_get_single_block_receive

DESCRIPTION
             This function return the value of enable_single_block_receive flag.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE boolean

===========================================================================*/

boolean gpl1_gprs_get_single_block_receive (gas_id_t gas_id)
{
    gpl1_gprs_single_data_t *gpl1_gprs_single_data_ptr = &gpl1_gprs_single_data[gas_id];
    return gpl1_gprs_single_data_ptr->enable_single_block_receive;
}

#endif /*FEATURE_GTA_2PHASE_ACCESS*/
