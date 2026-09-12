/*=========================================================================
G P L 1  G P R S  S H A R E D  T R A N S F E R



GENERAL DESCRIPTION
   This module contains the procedures that are common to GPRS/EGPRS
   processing in both DTM and packet transfer modes
EXTERNALIZED FUNCTIONS
 gpl1_control_dtm_cs_ps()

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2005-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_shared_transfer.c#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
01/02/18   km       CR2092516 Removal of misleading or redundant prints
16/08/16   bg       CR1047602 Updating frequency offset value for invalid bursts from the valid bursts in PDTCH case
24/05/16   sn       CR1016717:Removing Excess F3 logging for packet transfer
13/05/16   db       CR937805. DL BLER for GPRS and EGPRS DL TBF
21/12/15   sk       CR938638 Reducing no of F3s
28/09/15   pg       CR909305 Update dynamic RxD switching and remove pwr_meas suspension  
17/09/15   jk       CR901965:Correcting logging issues when PDTCH bursts are cancelled
09/09/15   hv       CR904692 GRLC Static declarations of variables which are not Dual data spaced
18/08/15   ra       CR879738 Error Recovery due to DSM exhaustion
03/07/15   shr      CR864001 Correct frame number logged for the early payload decode log packet
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/05/15   ws       CR834838 Correct as_id,gas_id calling fn parameter bugs
12/01/15   sp       CR779071: Dont ignore Packet TBF release when close to TBF reconfiguration.
15/02/15   sp       CR729492 Remove unimportant logging
12/02/15   ab       CR773792: Perform header decodes only during DL tbf release, reduces TX Blanking
30/01/15   pa       CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
08/08/14   nk       CR705945 Check for GERAN_ACCESS_STRATUM_ID_UNDEFINED to avoid memory read crash
11/11/14   ws       CR702510 Add support for gprs message metrics E log packet
29/10/14   ab       CR745429 - Featurization correction for PCHXFER for SS / CR merges.
01/10/14   nm       CR732131 Return gbta_not_allowed correclt in case null ptr check fails
20/08/14   sk       CR708206 Ensure GBTA is not allowed in the frame where lowest time slot changes
23/09/14   cja      CR728745 Always clear IR mem when enable IR
15/09/14   sp       CR681169 Additional logging of ignored mac control blks.
23/07/14   pjr      CR682764 TP IR Feature - implement IR mem Full handling from GFW to GRLC
13/06/14   am       CR679910 NULL check while accessing pTransfer_data
15/07/14   pjr      CR694682 Reduce MSC when TX blanking average reach 70%, increase average length
20/06/14   pjr      DATA+MMS feature - USF utilisation calculation
30/05/14   ws       CR657674 Data + MMS Phase 1
21/03/14   js       CR623622: Dont ignore data blocks with unmatched TFI so that SRB is turned ON
21/10/13   jk       CR564213/CR615619: Fixing KW error
26/11/13   ab       CR581579: DSDS GBTA: Do now allow GBTA in Two Phase
                    as not fully transitioned to Transfer
28/11/13   jj       CR 582720  fix merge error
24/10/13   jj       CR 497390 add LOG_GPRS_RECEIVE_BURST_METRICS_E_C
18/10/13   jj       CR 563546 KW array overflow warning fix
24/09/13   sp       CR482765: New logpacket for Edge Enhanced info from GFW
24/09/13   sp       CR449651: Use new logPacket for GPRS Burst metrics
27/09/13   sp       CR551580: Add NV logging of outgoing L1 messages
15/08/13   sk       CR524039 GBTA bringup changes
07/16/13   zc       CR470197 Replace memcpy with memscpy
21/06/13   cs       Major Triton TSTS Syncup
16/05/13   ab       CR487396 - Support EGPRS IR Reduced Window in GFW
01/05/13   ab       CR481750:Incorrect frame number comparison when frame number wraps
15/02/12   ws       CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
14/02/13   og       CR452911. NULL transfer data pointer.
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
28/09/11   ab       CR325130 : L1 host now passes TFI in EGPRS in UNACK MODE to GFW
28/09/11   ab       CR299297 : Incorrect Handling of Control messages during TBF
                    reconfiguration boundary
27/07/2011 ab       CR298128 Enhancement of SNR reselection in Transfer mode.
28/06/11   ab       CR293769:L1 incorrectly ignores DL blocks when Frame number recycles
21/03/11   ky       CR277664:Ignoring the DL data block received with correct DL TFI but
                    on a different timeslot which is not allocated in the current downlink tbf
18/01/11   pg       CR270330 Add FEATURE_GSM_DYNAMIC_AEQ_8PSK
28/04/11   pg       Fix compiler warnings
10/11/10   ws       CR 254488 - Use higher timeslots for AFC in packet transfer mode
                    if lowest timesot has poor SNR
30/11/10   ab       Support EGPRS WS REDUCTION during allocation of dynamic memory in start
                    gsm mode request when FEATURE_GSM_EGPRS_IR_OPT is defined.
06/09/10   ab       ENH Add TEST MODE B Reconfig and add asymetric TS support.
17/09/10   ws       CR 254345 - Always AGC on Control blocks received, remove gating
                    based on USF detection
07/09/10   ws       CR 252093 - Eng mode Rxqual fixes for PTM and enabling in dedicated
14/06/10   ab       CR: 235571 - compile error correction for previous check in
14/06/10   ab       CR: 235571 - 3GPP: Radio blocks for channel quality reporting
06/05/10   dv       CR237642 - Add ASRB funtionality
05/05/10   ws       CR232901 - further changes as previous change did not work
28/04/10   dv       CR232901 - DL FTP data Stall while switching
                    from CS 2 to CS 3 cell reselect
18/03/10   ab       CR#: 228650-chk downlink rrbp ts with assigned dl ts alloc
23/02/10   pg       CR168644 reverse order of GPRS metrics logging
20/01/10   cs       CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
11/12/09   ab       CR: 218950 - added timeslot check when decoding downlink data block.
02/12/09   ws       Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                    Improvements
04/09/09   ap       CR 193147 Incorrect handling of TFI
09/09/09   cs       Fix Klocwork possible NULL pointer reference error
20/07/09   ap       CR 189567 UE is not ignoring the FBI bit in SRB mode
09/07/09   ab       CR 183589 Incorrect handling of RLC mode change request
                    during extend UL TBF operation
29/05/09   ws       FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
12/03/09   ws       CR 173134 Added Test mode A to L1 interface and calculate
                    RLA_P for test modes from PDCH
26/01/09   og       Ensure rxqual calculations ignore CS4 radio blocks. Resolves
                    CR170214.
19/01/09   nt       CR151077 - Enhancement to SNR based reselection
24/11/08   og       FEATURE_GSM_GPRS_MSC33 GPL1 changes.
20/08/08   ab       CR 143307 Layer 1 should not perform an ABORT in cases when the
                    GET has been deferred.
20/08/08   og       CR155903 Open SRB loopback as late as possible - TC21.8 / 21.9 BER fix.
23/05/08   ap       Klocwork fixes
29/02/08   ab       Corrections for UL FIFO access to avoid page fault in CR138476
15/01/08   ws       Added support for ESIAC logging and SAIC AFC in PTM
11/01/08   og       Complete the solution to CR125334 by not returning from the
                    tx cb routine when the abort callbacks boolean is TRUE.
12/12/07   ws       CR 125334 Ensure RLC UL FIFO abort happens before any possible
                    PUAN is received in RLC.
07/02/07   cs       Fix compile if EDTM build
26/01/07   og       Implement UL ctrl msg deletion in L1.
23/01/07   og       Resolve RVCT2.1 compiler warnings, merge from the
                    gpl1_hummer.10.00.43.01 branch.
28/11/06   dv       Lint cleanup.
07/04/06   nt       Changes for required host test support/compilation made within
                    gpl1_gprs_tx_dyn_rrbp_disc_cb to the MSG_ERROR call
29/03/06   og       Take wrap around cases into account when add/sub camp timeslot
                    to/from absolute timeslots, resolves CR 90960.
16/03/06   og       Re-introduce the use of the buffering of camp timeslot by NPL1.
01/02/06   ws       Support for SRB and Test mode B ACK mode
26/01/06   og       Neighbour cell acquisition changes to handle extended dynamic.
17/01/06   og       Change the number of timeslots F3 debug to mention rx instead
                    of dl.
07/12/05   og       Protect against TS7 wrap around case, resolves CR 84496.
23/11/05   og       Update tx call back routines to provide more information.
15/11/05   og       Fix to USF granularity handling, resolves CR 82339, 82341.
24/1-/05   ws       GPRS data_cb. If we can't send dsm_ptr to rlc then free the dsm item.
21/09/05   og       Fix to the restructuring of the rlc deferred blocks handling
                    in PL1.
20/09/05   ws       Added DTM multislot class 9
01/09/05   ws       Linted and code cleanup for FEATURE_GSM_DTM
22/08/05   og       Add the setting of mac mode to NPL1.
19/08/05   og       Removal of downlink block FIFO.
17/08/05   og       Removal of dummy ctrl block dual buffer.
12/08/05   ws       Updated for initial DTM release
08/08/05   og       Initial Revision
                    Restructuring PL1 before addition of DTM.

===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"



#include "l1i.h"
#include "l1_isr.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_test.h"
#include "gpl1_gprs_utils.h"
#include "l1_utils.h"     /* gl1_get_FN() prtotype */
#include "l1_task.h"      /* l1_transfer_data extern */
#include "grlcl1.h"       /* l1 -> rlc inter task message API */
#include "grlcdlfifo.h"   /* GRLC downlink FIFO API */
#include "grlculfifo.h"   /* GRLC uplink FIFO API */
#include "gmacl1.h"       /* l1 -> gmac inter task message API */
#include "gmactimers.h"
#include "gprs_pdu.h"
#include "gl1_sys_algo_gprs.h" /* GPRS AGC API */
#include "gl1_msg_pdch.h" /* NPL1 message layer API */
#include "ms.h"           /* for inter-task Message set's */

#include <stringl/stringl.h>

#include "l1_sc.h"
#include "geran_eng_mode_info.h"
#include "gpl1_gprs_ptcch.h"

#ifdef FEATURE_GSM_EGPRS_IR_WS_REDUCTION
#include "gl1_msg_g.h"
#include "grlcl1.h"
#endif /* FEATURE_GSM_EGPRS_IR_WS_REDUCTION */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/* Local storage of the accumulated PDTCH SNR values */
static struct{
  boolean valid;
  sdword accumulator;
  sword accumulator_count;
}pdtch_snr[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE_S( 0 ) };


/* Used to extract the usf value from the first
 * octet of the downlink block header.
 */
#define  USF_BITMASK  0x07

/* Used to add debug for when Txing UL PACCH data */
#define DEBUG_UL_PACCH

/* Used to define the size of the fn reduction array */
#define  MOD13_FN_COUNTER  13

/* Used in extended dynamic allocation to remove the polling
 * off rlc data blocks received on timeslot common between
 * downlink and uplink tbf.
 * This macro acts on the first octet of the header.
 */
#define  L1_CLEAR_RLC_HDR_POLL_BIT(a) ((a) & (0xF7))
/* The previous blocks decode results are available 2 frames
 * into the start of the current block period. At a maximum
 * it could be 3 frames.
 */
#define  MAX_DECODE_FN_DIFF  3

static gpl1_ps_store_T  gpl1_ps_isr_store[NUM_GERAN_DATA_SPACES];
;


/* Used in the receive data callback function to provide
 * the frame offset to the first burst tdma frame.
 */
static const uint8 GPL1_EGPRS_RRBP_FN_REDUCTION[ MOD13_FN_COUNTER ]=
{
  5, 6, 7, 8, 4, 5, 6, 7, 4, 5, 6, 7, 4
};


static void  gpl1_gprs_extract_mcs_hdr_params( uint16             *mcs_hdr,
                                               pl1_mcs_hdr_info_T *pl1_mcs_hdr_store );

static void  gpl1_gprs_handle_mcs_data_cb( gl1_defs_rx_pkt_data_type *msg_data,
                                           dBx16_T                   *pr_value,
                                           boolean                   *message_valid,
                                           usf_dl_tfi_info_t *usf_dl_tfi_info,
                                           gas_id_t gas_id );

static uint8 gpl1_get_control_msg_type(uint8 * ctrl_msg);

/* Time slot mask , TS is mapped in reversed order */
const uint8 tn_mask[MAX_TN] = {1,2,4,8,16,32,64,128};


#ifdef  FEATURE_GSM_GPRS_MSC33
#define MAX_GPRS_BURST_METRICS   5
#else
#define MAX_GPRS_BURST_METRICS   4
#endif

typedef void (*gpl1_log_gprs_burst_metrics_x_type) ( gl1_msg_dl_chn_type channel,uint32 fn,
                                              const gl1_defs_rx_pkt_metrics_type *rpt_ptr, gas_id_t gas_id );

/* Note: gpl1_log_gprs_burst_metrics_e not currently supported */
static gpl1_log_gprs_burst_metrics_x_type gpl1_log_gprs_burst_metrics_x[MAX_GPRS_BURST_METRICS] = {gpl1_log_gprs_burst_metrics_a,
                                                                              gpl1_log_gprs_burst_metrics_b,
                                                                              gpl1_log_gprs_burst_metrics_c,
                                                                              gpl1_log_gprs_burst_metrics_d,
                                                                              gpl1_log_gprs_burst_metrics_e};

static gpl1_log_gprs_burst_metrics_x_type gpl1_log_gprs_burst_metrics_ver2_x[MAX_GPRS_BURST_METRICS-1] = {gpl1_log_gprs_burst_metrics_a_ver2,
                                                                              gpl1_log_gprs_burst_metrics_b_ver2,
                                                                              gpl1_log_gprs_burst_metrics_c_ver2,
                                                                              gpl1_log_gprs_burst_metrics_d_ver2};

extern dedicated_data_T l1_dedicated_data[];

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

static boolean gl1_ir_mem_full[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
/*===========================================================================

              EXTERNALISED FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION  gpl1_gprs_read_xfer_store

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

const gpl1_ps_store_T *  gpl1_read_xfer_isr_store(gas_id_t gas_id)
{

  /* Return pointer to local store */
  return(&gpl1_ps_isr_store[gas_id]);
}

/*===========================================================================

FUNCTION  gpl1_gprs_update_first_rx

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_first_rx( boolean  first_rx, gas_id_t gas_id )
{
  gpl1_ps_isr_store[gas_id].first_rx = first_rx;
}

/*===========================================================================

FUNCTION  gpl1_gprs_update_usf_det

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_usf_det( boolean  usf_detected, gas_id_t gas_id )
{
  gpl1_ps_isr_store[gas_id].usf_detected = usf_detected;
}

/*===========================================================================

FUNCTION  gpl1_gprs_update_enable_tx

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_enable_tx( boolean  enable_tx, gas_id_t gas_id )
{
  gpl1_ps_isr_store[gas_id].enable_tx = enable_tx;
}
/*===========================================================================
FUNCTION  gpl1_gprs_update_deferred_buff

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_deferred_buff(rlc_ul_status_T * rlc_buf,
                                    boolean abort_flag,
                                    gpl1_defer_mode_t mode,
                                    gas_id_t gas_id)
{
  switch(mode)
  {
    case UPDATE_ABORT_FLAG:
    {
      gpl1_ps_isr_store[gas_id].defer_abort_flag = abort_flag;

      break;
    }
    case UPDATE_BUFFER:
    {
      gpl1_ps_isr_store[gas_id].deferred_get_buff = *rlc_buf;
      break;
    }

    case RESET_BUFFER:
    {
       memset( &gpl1_ps_isr_store[gas_id].deferred_get_buff,
               0,
               sizeof(gpl1_ps_isr_store[gas_id].deferred_get_buff)
             );
       break;
    }
    default:

      MSG_GERAN_ERROR_1("PL1: wrong defer buff access %d",mode);
      break;
  }
}

/*===========================================================================
FUNCTION  gpl1_gprs_update_num_rx_tn

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_num_rx_tn( uint8  num_rx_tn, gas_id_t gas_id )
{
  gpl1_ps_isr_store[gas_id].num_rx_tn = num_rx_tn;
}

/*===========================================================================
FUNCTION  gpl1_gprs_update_abort_callbacks

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_abort_callbacks( boolean  abort_callbacks, gas_id_t gas_id )
{
  gpl1_ps_isr_store[gas_id].abort_callbacks = abort_callbacks;
}

/*===========================================================================
FUNCTION  gpl1_gprs_update_afc_tt_mode

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_GSM_DTM
void gpl1_gprs_update_afc_tt_mode(gl1_msg_afc_tt_mode afc_tt_mode, gas_id_t gas_id)
{
  gpl1_ps_isr_store[gas_id].gpl1_afc_tt_mode = afc_tt_mode;
}
#endif

/*===========================================================================
FUNCTION  gpl1_gprs_init_metrics_cb_store

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_init_metrics_cb_store(gas_id_t gas_id)
{
  int16 i,j=0;

  /* setup metrics store for downlink receives */
#ifdef  FEATURE_GSM_GPRS_MSC33
  for (i=0;i<PL1_MULTISLOT_RX;i++)
#else
  for (i=0;i<4;i++)
#endif
  {

#ifdef  FEATURE_GSM_GPRS_MSC33
    for (j=0;j<GL1_DEFS_FRAMES_IN_BLK_PERIOD;j++)
#else
    for (j=0;j<4;j++)
#endif
    {

      gpl1_ps_isr_store[gas_id].rlc_mac_dl_info[i].metrics_data_mem[j] =
                            &gpl1_ps_isr_store[gas_id].gpl1_gprs_dyn_dl_metrics[i][j];
    }
  }

  gpl1_ps_isr_store[gas_id].usf_utilisation_stats.frame_num = L1_GPRS_INVALID_FN;
  gpl1_ps_isr_store[gas_id].usf_utilisation_stats.total_usfs = 0;
  gpl1_ps_isr_store[gas_id].usf_utilisation_stats.total_blocks_blanked = 0;
  gpl1_ps_isr_store[gas_id].usf_utilisation_stats.num_consecutive_blanked_blocks = 0;
  gpl1_ps_isr_store[gas_id].usf_utilisation_stats.blanking_percentage_average = 0;
  gpl1_ps_isr_store[gas_id].usf_utilisation_blanking_average = 0;
}

/*===========================================================================
FUNCTION  gpl1_gprs_init_data_cb_store

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_init_data_cb_store(uint8 index, gl1_defs_rx_pkt_data_type *buff_ptr, gas_id_t gas_id)
{
  if(buff_ptr== NULL)
  {
    MSG_GERAN_ERROR_0("data_cb_ptr NULL !!");
    return;
  }
  /* Setup DL data buffer */
  gpl1_ps_isr_store[gas_id].rlc_mac_dl_info[index].msg_data_mem = buff_ptr++;

}

/*===========================================================================
FUNCTION  gpl1_gprs_init_dl_fifo

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_init_dl_fifo(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T  *transfer_data_ptr =
                                     l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  /* initialise downlink buffer pointers to begining of fifo */
  transfer_data_ptr->rlc_mac_dl_buff.in_ptr =
                                    &gpl1_ps_isr_store[gas_id].gpl1_gprs_dyn_dl_data[0];

}

/*===========================================================================

FUNCTION gp1l_gprs_abort_ul_fifo_access

DESCRIPTION

This function interfaces with RLC layer to abort any pending messages awaiting USF
detection for succesful transmission.

The calling of this function implies 1 or more RLC PDU's haven't been transmitted
 due to USF allocation

DEPENDENCIES

RETURN VALUE
  none
SIDE EFFECTS
  None

===========================================================================*/
static void gp1l_gprs_abort_ul_fifo_access(gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    grlc_ul_fifo_access_block_t   rlc_config_params;
    transfer_data_T    *transfer_data_ptr =
                      (transfer_data_T *)l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  /* Check if there are any RLC messages that have been scheduled  but not transmitted
   * if so abort RLC, assuming that access to the UL_FIFO is allowed, i.e no CS or TLLI
   * change is in progress
   */
  if (transfer_data_ptr->rlc_ul_status.num_msgs)
  {
    /* Abort RLC UL FIFO here as scheduled tx has not been transmitted*/
    rlc_config_params.access_mode = ABORT_MODE;

    switch ( grlc_ul_fifo_access_block(gas_id, &rlc_config_params) )
    {
      case UL_FIFO_BUSY:
        /* defer abort until next block */
        GPL1_SET_ABORT_FLAG( gas_id );
        break;

      case UL_FIFO_ACCESS_OK:

        /* disable abort flag as the abort has been sucessful */
        GPL1_CLEAR_ABORT_FLAG( gas_id );

        /* Clear out pending RLC Tx count as we don't want to
         * Abort more than once per Radio block interval
         */
        transfer_data_ptr->rlc_ul_status.num_msgs = 0;

        break;

      default:
        MSG_GERAN_ERROR_0_G("PL1:ABORT MODE on RLC ULFIFO failed");
        break;
    }

  } /* end of if(rlc_ul_status.num_msgs.. */

}

/*===========================================================================
FUNCTION  gpl1_gprs_set_usf_gran_info

DESCRIPTION

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_gprs_set_usf_gran_info( uint8    usf_gran_count,
                                  boolean  start_usf_gran_count,
                                  gas_id_t gas_id )
{
  gpl1_ps_store_T*  ps_store_ptr = &( gpl1_ps_isr_store[gas_id] );

  ps_store_ptr->usf_gran_count       = usf_gran_count;
  ps_store_ptr->start_usf_gran_count = start_usf_gran_count;
}


/*===========================================================================

FUNCTION  gpl1_calc_dyn_dl_ts_info

DESCRIPTION
  This function works out the lowest downlink timeslot resulting from the
  received assignment/release message. It also works out the number of
  assigned timeslots and the highest relative timeslot.

DEPENDENCIES
  None


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_calc_dyn_dl_ts_info
               (
                 uint8                       dl_tn_alloc,
                 const uint8                 usf_value[GL1_DEFS_SLOTS_IN_FRAME],
                 downlink_timeslot_info_t*   downlink_ts_info_ptr,
                 gl1_defs_rx_pkt_alloc_type  alloc_type[ GL1_DEFS_MAX_ASSIGNED_DL_TS ],
                 gas_id_t gas_id
               )
{
  /* Local variables */
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  uint32   i;
  uint8    num_pdch            = 0;
  uint8    lowest_dl_timeslot  = GPL1_INVALID;
  uint8    highest_dl_timeslot = GPL1_INVALID; /* Added for debug purposes only */
#ifdef FEATURE_GSM_DTM
  gl1_msg_afc_tt_mode afc_tt_mode = AFC_TT_MODE_PDCH;
#endif


  boolean  lowest_dl_ts_found = FALSE;

  /* Pointer to read the serving cell measurement
   * structure
   */
  gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr =gprs_serv_cell_meas_ptr;

  /*  Clear the lowest timeslot valid flag so it gets set with this assignment  */
  gprs_pl1_scell_meas_ptr->lowest_tn.valid = FALSE;

  /* save previous highest tn */
  gprs_pl1_scell_meas_ptr->prev_highest_tn = gprs_pl1_scell_meas_ptr->highest_tn;

  /* setup timeslot's and USF values */
  for (i=0;i< MAX_TN ;i++)
  {
    if (num_pdch < GL1_DEFS_MAX_ASSIGNED_DL_TS)
    {
      /* check if timeslot is allocated and setup alloc_type struct */
      if( usf_value[i] != GPL1_INVALID )
      {
        if( FALSE == lowest_dl_ts_found )
        {
            /* Set up lowest numbered timeslot */
            gprs_pl1_scell_meas_ptr->prev_lowest_tn  =
                                                   gprs_pl1_scell_meas_ptr->lowest_tn.tn;

            gprs_pl1_scell_meas_ptr->lowest_tn.tn    = (uint8) i;
            gprs_pl1_scell_meas_ptr->lowest_tn.valid = TRUE;

            lowest_dl_ts_found = TRUE;
            lowest_dl_timeslot = (uint8) i;
        }

        /* Fill in the relative timeslot and associated usf value */
        alloc_type[num_pdch].tn  = (gl1_defs_tn_type) (i - lowest_dl_timeslot);
        alloc_type[num_pdch].usf = (gl1_defs_usf_type) usf_value[i];

        /* Increment number of assigned TN's */
        num_pdch++;

        /* Keep track of the highest timeslot */
        highest_dl_timeslot = (uint8) i;
      }
      else if( 0 != (dl_tn_alloc & 0x1) )
      {
        if( FALSE == lowest_dl_ts_found )
        {
          /* Set up lowest numbered timeslot */
          gprs_pl1_scell_meas_ptr->prev_lowest_tn  =
                                              gprs_pl1_scell_meas_ptr->lowest_tn.tn;
          gprs_pl1_scell_meas_ptr->lowest_tn.tn    = (uint8) i;
          gprs_pl1_scell_meas_ptr->lowest_tn.valid = TRUE;

          lowest_dl_ts_found = TRUE;
          lowest_dl_timeslot = (uint8) i;
        }

        /* Fill in the relative timeslot and associated usf value */
        alloc_type[num_pdch].tn  = (gl1_defs_tn_type)( i - lowest_dl_timeslot);
        alloc_type[num_pdch].usf = GL1_DEFS_PDCH_ALLOCATED;

        /* Increment number of assigned TN's */
        num_pdch++;

        /* Keep track of the highest timeslot */
        highest_dl_timeslot = (uint8) i;
      }

      /* Move onto next timeslot */
      dl_tn_alloc >>= 1;
    }
    else
    {
      /* Maximum number of timeslots for this multislot
       * class, exit the loop.
       */
      break;
    }
  } /* End of for(i=0...MAX_TN) */

  /* Set num_rx which is used to avoid clashes between
   * RRBP TX and Dynamic RX/TX.
   */
  gpl1_gprs_update_num_rx_tn(num_pdch, gas_id);

  downlink_ts_info_ptr->num_dl_pdch         = num_pdch;
  downlink_ts_info_ptr->lowest_dl_timeslot  = lowest_dl_timeslot;

  /* highest receive timeslot */
  downlink_ts_info_ptr->highest_dl_timeslot = highest_dl_timeslot;

  gprs_pl1_scell_meas_ptr->lowest_tn.tn = 0;

  /* Calculate the highest relative rx timeslot */
  gprs_pl1_scell_meas_ptr->highest_tn   = highest_dl_timeslot - lowest_dl_timeslot;

  MSG_GERAN_MED_3_G(" RX num %u lowest %u highest %u",
          num_pdch,
          lowest_dl_timeslot,
          highest_dl_timeslot );

#ifdef FEATURE_GSM_DTM
/* @@@@ may need to adjust highest tn, lowest tn to account for CS slot @@@@@@@ */

  if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
  {

    uint8    camped_to_low_tn_off = GPL1_INVALID;
    uint8    cs_tn = GPL1_INVALID;



    /* Fetch the current CS TN */
    cs_tn = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN;

    /* Adjust absoulute highest and lowest TN when the CS Tn is
     * the highest allocated timeslot
     */
    if(cs_tn > highest_dl_timeslot)
    {
      /* Does our PS TN lie on the camped TN ?
       * Use macro to handle wrap around case on TS7.
       */
      camped_to_low_tn_off = SUB_MOD( lowest_dl_timeslot,
                          l1_get_current_timeslot(gas_id),
                                      GL1_DEFS_SLOTS_IN_FRAME );

      /* absoloute highest TN */
      downlink_ts_info_ptr->highest_dl_timeslot = highest_dl_timeslot = cs_tn;

      /* abs lowest is unchanged */

      /* relative lowest 0 + camped offset */
      gprs_pl1_scell_meas_ptr->lowest_tn.tn =
                         ( gprs_pl1_scell_meas_ptr->lowest_tn.tn + camped_to_low_tn_off);

      /* relative highest ?? */
      gprs_pl1_scell_meas_ptr->highest_tn =
                      ( highest_dl_timeslot - lowest_dl_timeslot + camped_to_low_tn_off);
    }

    /* Adjust absoulute highest and lowest TN when the CS Tn is
     * the lowest allocated timeslot
     */
    if(cs_tn < lowest_dl_timeslot)
    {

      /* absoloute highest unchanged*/

      /* absoloute lowest */
      downlink_ts_info_ptr->lowest_dl_timeslot =  lowest_dl_timeslot = cs_tn;

      /* relative_lowest */
      gprs_pl1_scell_meas_ptr->lowest_tn.tn =
                        ( gprs_pl1_scell_meas_ptr->lowest_tn.tn + 2);

      /* relative highest */
      gprs_pl1_scell_meas_ptr->highest_tn =
                     ( highest_dl_timeslot - lowest_dl_timeslot + 2);

      /* as CS TN is lower than PS TN setup AFC TT mode */
      afc_tt_mode = AFC_TT_MODE_DCH;

      /* increment offset to step over CS slot to first PS slot */
      camped_to_low_tn_off = 2 + 1;
    }


    /* Adjust absoulute highest and lowest TN when the CS Tn is
     * in between the highest and lowest allocated PS timeslot
     */
    if( (cs_tn < highest_dl_timeslot) &&
        (cs_tn > lowest_dl_timeslot) )
    {
      /***
       *** Highest and Lowest absoloute and relative remain unchanged
       *** since PS allocation is discontinuous and CS slot sits in tthe gap
       ***/

      /* as PS TN is lower than CS TN setup AFC TT mode */
      afc_tt_mode = AFC_TT_MODE_PDCH;

      /* This will always be 1 since the CS timeslot will always
       * be slammed to TN 2 so the lowest PS slot will be 1 less
       * Use macro to handle wrap around case on TS7.
       */
      camped_to_low_tn_off = SUB_MOD( lowest_dl_timeslot,
                          l1_get_current_timeslot(gas_id),
                                      GL1_DEFS_SLOTS_IN_FRAME );

       /* relative lowest 0 + camped offset */
      gprs_pl1_scell_meas_ptr->lowest_tn.tn =
                        ( gprs_pl1_scell_meas_ptr->lowest_tn.tn + camped_to_low_tn_off);

      /* relative highest timeslot */
      gprs_pl1_scell_meas_ptr->highest_tn =
                     ( highest_dl_timeslot - lowest_dl_timeslot + camped_to_low_tn_off);

    }


    /*** NOTE these values include the CS TN ***/

    if(camped_to_low_tn_off != 0)
    {
      /* Adjust allocation based on current timeslot */

      for (i=0;i < GL1_DEFS_MAX_ASSIGNED_DL_TS;i++)
      {
        /* if tn is allocated then adjust based
         * on camp offset
         */
        if(alloc_type[i].usf <= GL1_DEFS_PDCH_ALLOCATED)
        {
          alloc_type[i].tn =
          (gl1_defs_tn_type)( camped_to_low_tn_off + (uint8) alloc_type[i].tn);
        }

      }

    }

    MSG_GERAN_MED_3_G("DTM Ass. CS tn %d lowest %d highest %d",cs_tn,
                                                     lowest_dl_timeslot,
                                                     highest_dl_timeslot);

  } /* end of if(L1_DTM_MODE... */

  /* Update AFC TT MODE */
  gpl1_gprs_update_afc_tt_mode(afc_tt_mode, gas_id);

#endif /* FEATURE_GSM_DTM */

}

/*===========================================================================

FUNCTION  gpl1_get_control_msg_type

DESCRIPTION
  This function decodes the message type from control block.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_get_control_msg_type(uint8 *ctrl_msg)
{
   uint8 rbsn_bit  = 0;
   uint8 fs_bit = 0;
   uint8 ac_bit = 0;
   uint8 msg_type;

   rbsn_bit = (ctrl_msg[1] >> 7);
   fs_bit =   ((ctrl_msg[1] >> 1) & 0x01);

   if((rbsn_bit == 0)  && (fs_bit == 1))
   {
     /*****************************************************************
     *  not a segmented dl msg, but has optional rlc/mac header       *
     *  octet present                                                 *
     ******************************************************************/

     /*check ac_bit to see if second optional octet is present.*/
     ac_bit = ctrl_msg[1] & 0x01;
     if (ac_bit) /* AC bit present */
       msg_type = ((ctrl_msg[3] & 0xFC) >> 2);
     else
       msg_type = ((ctrl_msg[2] & 0xFC) >> 2);
   }
   else
   /* message must be segmented more than one blocks.
     can be ignored as it gets complex.
     default msg_type to unknown so that it can be ignored
     broadcast messages are never segmented
   */
   {
     msg_type = 0x1;
   }
   return msg_type;
} /* gpl1_get_control_msg_type */


/*===========================================================================

FUNCTION  gpl1_gprs_extract_mcs_hdr_params

DESCRIPTION
  This function is called by gpl1_gprs_rx_dynamic_pdtch_data_cb
  and is used to extract es/p, rrbp, tfi and pr from the mcs header

  Passes back the message data and the msg_hdr which descrbes the
  received data.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gpl1_gprs_extract_mcs_hdr_params( uint16             *mcs_hdr,
                                               pl1_mcs_hdr_info_T *pl1_mcs_hdr_store )
{

  pl1_mcs_hdr_store->e_sp = (uint8)((*mcs_hdr & 0x18)>> 3);
  pl1_mcs_hdr_store->rrbp = (uint8)((*mcs_hdr & 0x60)>> 5);
  pl1_mcs_hdr_store->dl_tfi = (uint8)((*mcs_hdr & 0xF80)>> 7);
  pl1_mcs_hdr_store->pr = (uint8)((*mcs_hdr & 0x3000)>> 12);
  pl1_mcs_hdr_store->bsn1 =
          (uint16)(((mcs_hdr[0] & 0xC000)>> 14) | ((mcs_hdr[1] & 0x1FF) << 2));
}

/*===========================================================================

FUNCTION  gpl1_gprs_handle_mcs_data_cb

DESCRIPTION
  This function is called by gpl1_gprs_rx_dynamic_pdtch_data_cb
  when a MCS 1..9 EGPRS data block is received.


  Passes back the message data and the msg_hdr which descrbes the
  received data.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gpl1_gprs_handle_mcs_data_cb( gl1_defs_rx_pkt_data_type  *msg_data,
                                           dBx16_T                    *pr_value,
                                           boolean                    *message_valid,
                                           usf_dl_tfi_info_t          *usf_dl_tfi_info,
                                           gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  pl1_mcs_hdr_info_T pl1_mcs_hdr_store  = { 0x0, 0xFF, 0xFF, 0xFF, 0xFF};
  uint8 data_cnt;  /* required for test_mode_b */

  transfer_data_T    *transfer_data_ptr =
                (transfer_data_T *)l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  uint8                     *payload_ptr;
  uint8                     *tmp_ptr;
  dsm_item_type             *dsm_msg_1_ptr = NULL;
  dsm_item_type             *dsm_msg_2_ptr = NULL;
  uint8                     fbi_e_1 = 0;
  uint8                     fbi_e_2 = 0;
  boolean                   crc_1 = FALSE;
  boolean                   crc_2 = FALSE;
  boolean                   dl_tfi_matched = FALSE;
  e_pl1_radio_block_t       egprs_dl_fifo;
  grlc_dl_fifo_op_result_t  res;

  static const  uint8 pl1_cs_to_msg_length_lut[] =
  {
   GL1_DEFS_CS1_BYTES,        /* GL1_DEFS_CS1_CODING */
   GL1_DEFS_CS2_BYTES,
   GL1_DEFS_CS3_BYTES,
   GL1_DEFS_CS4_BYTES,
   GL1_DEFS_ACCESS_BYTES,     /* GL1_DEFS_ACCESS_CODING */
   GL1_DEFS_EXT_ACCESS_BYTES, /* GL1_DEFS_EXT_ACCESS_CODING */
   GL1_DEFS_MCS1_BYTES,       /* GL1_DEFS_MCS1_CODING */
   GL1_DEFS_MCS2_BYTES,
   GL1_DEFS_MCS3_BYTES,
   GL1_DEFS_MCS4_BYTES,
   GL1_DEFS_MCS5_BYTES,
   GL1_DEFS_MCS6_BYTES,
   GL1_DEFS_MCS7_BYTES,
   GL1_DEFS_MCS8_BYTES,
   GL1_DEFS_MCS9_BYTES
};

  /* Variables required for edge quality measurement reporting */
  gpl1_egprs_decode_metrics_T  egprs_decode_metrics;

  /* This pointer is used read to mdsp beps */
  uint16  *src_bep_ptr  =
  &( msg_data->egprs.bep[(GL1_DEFS_FRAMES_IN_BLK_PERIOD - 1)] );

  /* This pointer is used to write gpl1 beps ( format: 1/65535 units ) */
  uint16 *dest_bep_ptr =
  &( egprs_decode_metrics.bep[(GL1_DEFS_FRAMES_IN_BLK_PERIOD - 1)] );

  /* This pointer is used read to mdsp schedule_status (converted to schedule_error) */
  boolean  *schedule_error_ptr  =
  &( msg_data->egprs.schedule_error[(GL1_DEFS_FRAMES_IN_BLK_PERIOD - 1)] );

  uint8         dl_tfi;
  /* Get the current absolute tdma frame number from the GSM timer */
  const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id );

  /* Initialise RLC EGPRS DL_FIFO structure */
  memset(&egprs_dl_fifo,0,sizeof(e_pl1_radio_block_t) );

  /*****************************************************************************
   *** To get here an EGPRS DL TBF is setup and we have received MCS 1 ... 9 ***
   ****************************************************************************/

  /* First check if the RLC/MAC header CRC has passed */
  if (msg_data->egprs.hdr_crc_passed)
  {
    uint8 dl_ts_allocation = GPL1_INVALID_TN;

    /* Store header for passing up in RLC DL FIFO later */

    /* Extract parameters from header needed by this handler */
    gpl1_gprs_extract_mcs_hdr_params(msg_data->egprs.hdr,&pl1_mcs_hdr_store);


    /* calculate if we are potentially receiving the decoded block from the previous tbf.
     * If so this block needs to be compared with the previous tbf's TFI
     */
    if((transfer_data_ptr->dl_tbf_ptr->dl_tfi_st != L1_GPRS_INVALID_FN) &&
       (! gpl1_gprs_is_frame_num_later(current_fn,ADD_FN(transfer_data_ptr->dl_tbf_ptr->dl_tfi_st,MAX_DECODE_FN_DIFF))) &&
       ( GPL1_TFI_INVALID != transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi))
    {
      dl_tfi = transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi;
      dl_ts_allocation = transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc;
      MSG_GERAN_MED_2_G("using previous - dl tfi %d ts_alloc %0x",dl_tfi,dl_ts_allocation);
    }
    else
    {
      dl_tfi = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
      dl_ts_allocation	= transfer_data_ptr->dl_tbf_ptr->ts_allocation;
      transfer_data_ptr->dl_tbf_ptr->dl_tfi_st = L1_GPRS_INVALID_FN;
    }


    /* check if this mcs data block is addressed to us */
    dl_tfi_matched =
            ( (pl1_mcs_hdr_store.dl_tfi == dl_tfi)
              ? TRUE : FALSE);

    if ( (dl_tfi_matched) &&
          (!(dl_ts_allocation & tn_mask[msg_data->tn]))
       )
    {
      /* if dl tfi matched and the block received on the allocated timeslots then accept it other wise it should
        ** be discarded.
      */
      dl_tfi_matched = FALSE;
      MSG_GERAN_HIGH_2_G( "GPL1 ignoring dl data blk rvcd on incorrect ts=%0x ,prev alloc %0x",
                transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation,dl_ts_allocation);
    }

    if ( (dl_tfi_matched) &&
         (GPL1_TFI_INVALID == transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi) &&
         (!(transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation & tn_mask[msg_data->tn]))
       )
    {
      /* if new downlink is being assigned then the previous_dl_tfi should be invalid
      ** , and if the data block is decoded with the previous ul_ts allocation ,
      ** then it should be discarded.
      */
      dl_tfi_matched = FALSE;
      MSG_GERAN_HIGH_3_G("GPL1 previous dl tfi=%d,block is invalid a new dl tbf, dyn_ts_alloc=%x msg_tn=%d ",
                transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi,
                transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation,
                msg_data->tn);
    }

    usf_dl_tfi_info->dl_tfi_match = dl_tfi_matched;

    if (dl_tfi_matched)
    {
      MSG_GERAN_LOW_3_G("gl1_hw_get_rx_data enhEdgeDecHdrLog 0x%x, mcs_0 = 0x%x, mcs_1 0x%x",
                           msg_data->egprs.enhEdgeLogPkt.logPktHdr.enhEdgeDecHdrLog,
                           msg_data->egprs.enhEdgeLogPkt.logPktMsg[0].mcs,
                           msg_data->egprs.enhEdgeLogPkt.logPktMsg[1].mcs);
#ifdef FEATURE_DATA_PDU_DEBUG
      if (geran_nv_data_pdu_debug_enabled(GERAN_DATA_PDU_DEBUG_GRLC_DL))
      {
        MSG_GERAN_HIGH_0_G("DL TFI MATCHED");

        /* Copy the header and the 1st 12 octets of both payloads for RLC debugging needs */
        e_grlc_dl_copy_pld_for_dump(
          (uint16*)msg_data->egprs.hdr,
          (uint8*)msg_data->msg[0].data,
          (uint8*)msg_data->msg[1].data,
          gas_id
        );
      }
#endif /* FEATURE_DATA_PDU_DEBUG */

      /******************************************
       *** Next process first or only payload ***
       *****************************************/

#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
      if (grlc_dl_t3192_status(gas_id) == TMR_ACTIVE)
       {
         transfer_data_ptr->t3192_tfi_match_count ++;
         MSG_GERAN_MED_3_G("DL_REL_HDR_DEC_ONLY %d TFI %d tfi_match_count %d ",pl1_mcs_hdr_store.bsn1,pl1_mcs_hdr_store.dl_tfi,transfer_data_ptr->t3192_tfi_match_count);
       }
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */

      /* Setup crc status */
      crc_1 = (msg_data->msg[0].crc_passed);

      /* This MCS data block is for us so let agc know */
      *message_valid = TRUE;

      /* assign pr value so that agc can be informed
       * if downlink power control is enabled
       */
      if (transfer_data_ptr->dl_power_control.params_valid)
      {
        *pr_value = pl1_mcs_hdr_store.pr;
      } else
      {
        *pr_value = 0;
      }

      /* Has payload 1 CRC passed */
      if (crc_1)
      {
        /* Deduct PADDING_OCTET_LEN from MCS_6 or MCS_3 Block if it's a retx. MCS_8 block
         *
         * does the data block need padding to be removed
         */
        tmp_ptr=(uint8*)msg_data->egprs.hdr; // required for Klocwork
        if (e_grlc_dl_is_padding_removal_required
                                  (
                                    tmp_ptr,
                                    ((uint32)msg_data->coding_scheme - MSC_CS_OFFSET ))
                                  )
        {
          payload_ptr = (msg_data->msg[0].data + PADDING_OCTET_LEN);

          /* Extract FBI and E bits from payload */
          fbi_e_1 = (*payload_ptr & 0x3);

          /* Align remaing payload to start of octet boundary */
          gpl1_gprs_shift_out_fbi_e(
                                     payload_ptr,
                                     (msg_data->msg[0].msg_len - PADDING_OCTET_LEN)
                                   );

#ifdef FEATURE_DATA_PDU_DEBUG
          if (geran_nv_data_pdu_debug_enabled(GERAN_DATA_PDU_DEBUG_GL1))
          {
            MSG_GERAN_HIGH_0_G("printing 1st BSN after FBI SHIFT");
            gl1_print_data_block(msg_data->msg[0].data, msg_data->coding_scheme, gas_id);
          }
#endif /* FEATURE_DATA_PDU_DEBUG */

          /* Sanity check MCS length */
          if(msg_data->msg[0].msg_len !=
                                    pl1_cs_to_msg_length_lut[msg_data->coding_scheme] )
          {
            MSG_GERAN_ERROR_3_G(" mcs length wrong mcs %d npl1 len %d calc len %d",
                                     msg_data->coding_scheme,
                                     msg_data->msg[0].msg_len,
                                     pl1_cs_to_msg_length_lut[msg_data->coding_scheme] );
          }

          /* create dsm item chain for the payload */
          dsm_msg_1_ptr = gl1_pdu_construct
                                    (
                                      payload_ptr,
                                      (msg_data->msg[0].msg_len - PADDING_OCTET_LEN - 1)
                                    );

        } else
        {
          payload_ptr = (msg_data->msg[0].data);

          /* Extract FBI and E bits from payload */
          fbi_e_1 = (*payload_ptr & 0x3);

          /* Align remaing payload to start of octet boundary */
          gpl1_gprs_shift_out_fbi_e(payload_ptr,msg_data->msg[0].msg_len);

#ifdef FEATURE_DATA_PDU_DEBUG
          if (geran_nv_data_pdu_debug_enabled(GERAN_DATA_PDU_DEBUG_GL1))
          {
            MSG_GERAN_HIGH_0_G("printing 1st BSN after FBI SHIFT");
            gl1_print_data_block(msg_data->msg[0].data, msg_data->coding_scheme, gas_id);
          }
#endif /* FEATURE_DATA_PDU_DEBUG */

          /* Sanity check MCS length */
          if(msg_data->msg[0].msg_len !=
                                      pl1_cs_to_msg_length_lut[msg_data->coding_scheme] )
          {
            MSG_GERAN_ERROR_3_G(" mcs length wrong mcs %d npl1 len %d calc len %d",
                                    msg_data->coding_scheme,
                                    msg_data->msg[0].msg_len,
                                    pl1_cs_to_msg_length_lut[msg_data->coding_scheme] );
          }

          /* create dsm item chain for the payload */
          dsm_msg_1_ptr = gl1_pdu_construct(payload_ptr,(msg_data->msg[0].msg_len - 1));
        }

        if ( dsm_msg_1_ptr == NULL )
        {
          MSG_GERAN_ERROR_0_G("Out of DSM Resources");
        }

      } else
      {

        /***************************
        *** Payload 1 crc failed ***
        ***************************/

        /* check if the rlc mode is anacknowledged mode */
        if (transfer_data_ptr->rlc_mode == RLC_UNACK)
        {
         /* Deduct PADDING_OCTET_LEN from MCS_6 or MCS_3 Block
          * if it's a retx. MCS_8 block
          *
          * does the data block need padding to be removed
          */
          tmp_ptr=(uint8*)msg_data->egprs.hdr; // required for Klocwork
          if (e_grlc_dl_is_padding_removal_required
                             (
                                tmp_ptr,
                                ( (uint32)msg_data->coding_scheme - MSC_CS_OFFSET) )
                             )
          {
            payload_ptr = (msg_data->msg[0].data + PADDING_OCTET_LEN);

            /* Set FBI and E bits from payload to zero as crc has failed */
            fbi_e_1 = 0;

            /* Align remaing payload to start of octet boundary */
            gpl1_gprs_shift_out_fbi_e(
                                        payload_ptr,
                                        (msg_data->msg[0].msg_len - PADDING_OCTET_LEN)
                                     );

            /* create dsm item chain for the payload */
            dsm_msg_1_ptr = gl1_pdu_construct
                                     (
                                       payload_ptr,
                                       (msg_data->msg[0].msg_len - PADDING_OCTET_LEN - 1)
                                     );

          } else
          {
            payload_ptr = (msg_data->msg[0].data);

            /* Set FBI and E bits from payload to zero as crc has failed */
            fbi_e_1 = 0;

            /* Align remaing payload to start of octet boundary */
            gpl1_gprs_shift_out_fbi_e(payload_ptr,msg_data->msg[0].msg_len);

            /* create dsm item chain for the payload */
            dsm_msg_1_ptr = gl1_pdu_construct
                                          (
                                            payload_ptr,
                                            (msg_data->msg[0].msg_len - 1)
                                          );
          }

          if ( dsm_msg_1_ptr == NULL )
          {
            MSG_GERAN_ERROR_0_G("Out of DSM Resources");
          }
        } else
        {
          /* Ignore this payload when the rlc mode is acknowledged mode */
        }
      } /* end of if/else(crc_passed ... */

      /***********************************************
      *** Next process second payload if it exists ***
      ***********************************************/

      if (IS_MSG_DUAL_PAYLOAD(msg_data->coding_scheme))
      {
        /* Setup crc status */
        crc_2 = (msg_data->msg[1].crc_passed);

        /* Has payload 1 CRC passed */
        if (crc_2)
        {
          /* Need not check for padding removal in case of dual payload
          ** Payload removal is only for MCS_6 or MCS_3
          */

          payload_ptr = (msg_data->msg[1].data);

          /* Extract FBI and E bits from payload */
          fbi_e_2 = (*payload_ptr & 0x3);

          /* Align remaing payload to start of octet boundary */
          gpl1_gprs_shift_out_fbi_e(payload_ptr,msg_data->msg[1].msg_len);

#ifdef FEATURE_DATA_PDU_DEBUG
          if (geran_nv_data_pdu_debug_enabled(GERAN_DATA_PDU_DEBUG_GL1))
          {
            MSG_GERAN_HIGH_0_G("printing 2nd BSN after FBI SHIFT");
            gl1_print_data_block(msg_data->msg[1].data, msg_data->coding_scheme, gas_id);
          }
#endif /* FEATURE_DATA_PDU_DEBUG */

          if(msg_data->msg[1].msg_len !=
                                      pl1_cs_to_msg_length_lut[msg_data->coding_scheme] )
          {
            MSG_GERAN_ERROR_3_G(" mcs length wrong mcs %d npl1 len %d calc len %d",
                                    msg_data->coding_scheme,
                                    msg_data->msg[1].msg_len,
                                    pl1_cs_to_msg_length_lut[msg_data->coding_scheme] );
          }

          /* create dsm item chain for the payload */
          dsm_msg_2_ptr = gl1_pdu_construct(payload_ptr, (msg_data->msg[1].msg_len -1));

          if ( dsm_msg_2_ptr == NULL )
          {
            MSG_GERAN_ERROR_0_G("Out of DSM Resources");
          }

        } else
        {

          /***************************
          *** Payload 1 crc failed ***
          ***************************/

          /* check if the rlc mode is anacknowledged mode */
          if (transfer_data_ptr->rlc_mode == RLC_UNACK)
          {
            /* Need not check for padding removal in case of dual payload
            ** Payload removal is only for MCS_6 or MCS_3
            */

            payload_ptr = (msg_data->msg[1].data);

            /* Set FBI and E bits from payload to zero as crc has failed */
            fbi_e_2 = 0;

            /* Align remaing payload to start of octet boundary */
            gpl1_gprs_shift_out_fbi_e(payload_ptr,msg_data->msg[1].msg_len);

            /* create dsm item chain for the payload */
            dsm_msg_2_ptr = gl1_pdu_construct
                                           (
                                             payload_ptr,
                                             (msg_data->msg[1].msg_len - 1)
                                           );

            if ( dsm_msg_2_ptr == NULL )
            {
              MSG_GERAN_ERROR_0_G("Out of DSM Resources");
            }

          } else
          {
            /* Ignore this payload when the rlc mode is acknowledged mode */
          }
        } /* end of if/else(crc_passed ... */

      } /* end of if DUAL_PAYLOAD.. */


       /* Populate RLC downlink FIFO structure */
      egprs_dl_fifo.crc_1 = crc_1;
      egprs_dl_fifo.crc_2 = crc_2;
      egprs_dl_fifo.fbi_e_1 = fbi_e_1;
      egprs_dl_fifo.fbi_e_2 = fbi_e_2;
      egprs_dl_fifo.payload_1_ptr = dsm_msg_1_ptr;
      egprs_dl_fifo.payload_2_ptr = dsm_msg_2_ptr;


      /* Pass up RRBP related parameters to be relayed
       * to MAC for PCA tx's in Unack mode
       */
      egprs_dl_fifo.rrbp = pl1_mcs_hdr_store.rrbp;
      egprs_dl_fifo.frame_number =
                           SUB_FN( current_fn, GPL1_EGPRS_RRBP_FN_REDUCTION[ frame_counters[gas_id].FNmod13 ] );
      egprs_dl_fifo.time_slot = (uint8)msg_data->tn;
      egprs_dl_fifo.esp = pl1_mcs_hdr_store.e_sp;


      /* Copy header into structure */
      memscpy(egprs_dl_fifo.hdr_arr,sizeof(egprs_dl_fifo.hdr_arr),msg_data->egprs.hdr,sizeof(egprs_dl_fifo.hdr_arr));


      /* Map Coding Scheme received from NPl1 to GSM 04.60 values */
      egprs_dl_fifo.mcs  = ((uint32)msg_data->coding_scheme - MSC_CS_OFFSET);


      if (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_B)
      {
        /* get data_cnt to know if blocks needs discarding or number of block
        * required to generate based on number of ul ts
        */
        data_cnt = gpl1_gprs_is_rlc_dl_data_disc(msg_data->tn, gas_id);

        /* If we are not looping back this timeslot and the FBI bit is Not set,
         * discard the data block. data_cnt = 0 means disc the block.
         */
        if ((data_cnt == 0) && ((fbi_e_1 & 0x2) == 0))
        {
          MSG_GERAN_HIGH_3_G(" TMB: ERLC block disc tn %d bsn %d cnt=%d",
                                      msg_data->tn,
                                      pl1_mcs_hdr_store.bsn1,
                                      data_cnt);
          egprs_dl_fifo.loop_dl_blk = 0;
        }else
        {
          if ((data_cnt == 0) && (((fbi_e_1 & 0x2) >> 1) == 1))
          {
            data_cnt = 1;

            MSG_GERAN_HIGH_3_G(" TMB: ERLC block accept FBI_1=1 tn %d bsn %d data_cnt %d",
                                msg_data->tn,
                                pl1_mcs_hdr_store.bsn1,
                                data_cnt);
          }
          egprs_dl_fifo.loop_dl_blk = data_cnt;

          MSG_GERAN_LOW_3_G(" TMB: ERLC block accept tn %d bsn %d data_cnt %d",
                      msg_data->tn,
                      pl1_mcs_hdr_store.bsn1,
                      data_cnt);
        }
        /* Pass EGPRS RLC data block(s) To RLC for processing */
        res = e_grlc_dl_fifo_put_block(gas_id, &egprs_dl_fifo);

        if (res != DL_FIFO_PUT_OK)
        {
          MSG_GERAN_ERROR_1_G("EGPRS DL FIFO put error %d",res);
        }

        /* Send ph_data_ind every data block in Test Mode B */
        grlc_send_ph_data_ind(gas_id);

      }
      else
      {
        if (( L1_TEST_MODE_SRB_OFF == l1_transfer_data[gas_id].l1_test_mode ) ||
           ( L1_TEST_MODE_SRB_ON == l1_transfer_data[gas_id].l1_test_mode ))
        {
          /* always have the FBI set to 0 during SRB loopback */
          egprs_dl_fifo.fbi_e_1 = 0;
          egprs_dl_fifo.fbi_e_2 = 0;
        }
        /* Pass EGPRS RLC data block(s) To RLC for processing */
        res = e_grlc_dl_fifo_put_block(gas_id, &egprs_dl_fifo);

        if (res == DL_FIFO_PUT_OK)
        {
          /* DL FIFO put success, check if RLC needs context */
          if (  e_grlc_is_rlc_dl_required_context
                                              ( gas_id,
                                                crc_1,
                                                crc_2,
                                                fbi_e_1,
                                                fbi_e_2,
                                                pl1_mcs_hdr_store.e_sp
                                              ) )
          {
            grlc_send_ph_data_ind(gas_id);
          }

        } else
        {
          MSG_GERAN_ERROR_1_G("EGPRS DL FIFO put error %d",res);

          /* Free dsm items's as RLC DL FIFO is full */
          if (dsm_msg_1_ptr)
          {
            GPRS_PDU_FREE(&dsm_msg_1_ptr);
          }

          if (dsm_msg_2_ptr)
          {
            GPRS_PDU_FREE(&dsm_msg_2_ptr);
          }
        }
      }

    } else
    {
      /* Let agc know not to use this message for AGC, ensure pr is zero */
      *message_valid = FALSE;
      *pr_value = 0;

      MSG_GERAN_MED_3_G("Pl1: DL TFI does not match rcvd %d alloc %d FN=%d",
                                    pl1_mcs_hdr_store.dl_tfi,
                                    transfer_data_ptr->dl_tbf_ptr->dl_tfi,
                                    gl1_get_FN( gas_id ));
    } /* End of if dl tfi matched */

    /* Store the bep measurements in the local bep array.
     * Do this for header crc ok case since bep module
     * ignores bep values when header crc is bad.
     */
    *dest_bep_ptr       = *src_bep_ptr;
    *(dest_bep_ptr - 1) = *(src_bep_ptr - 1);
    *(dest_bep_ptr - 2) = *(src_bep_ptr - 2);
    *(dest_bep_ptr - 3) = *(src_bep_ptr - 3);

#if defined (FEATURE_GSM_DYNAMIC_AEQ_8PSK) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
    gl1_msg_calc_aeq_8psk_mean_bep(src_bep_ptr, schedule_error_ptr, gas_id);
#endif
  } else
  {
    /* Header crc has failed therefore discard the message */

    /* Let agc know not to use this message for AGC, ensure pr is zero */
    *message_valid = FALSE;
    *pr_value = 0;
  } /* End of if header crc successful */

  /* Report raw ber measurements for quality measurement reporting purposes */

#if  EGPL1_OPTIMISE_MEAN_BEP

  egprs_decode_metrics.first_payload_error  = (boolean) !crc_1;
  egprs_decode_metrics.second_payload_error = (boolean) !crc_2;

#endif

  egprs_decode_metrics.decode_error = (boolean) !(msg_data->egprs.hdr_crc_passed);
  egprs_decode_metrics.coding_type  = msg_data->coding_scheme;

  /* Pass the data block decode metrics to the quality measurement module */
  gpl1_egprs_serv_cell_data_cb( msg_data->tn,
                                &( egprs_decode_metrics ),
                                dl_tfi_matched,
                                usf_dl_tfi_info, gas_id );

  gl1_ir_mem_full[gas_id] = (msg_data->egprs.IR_mem_full[0] || msg_data->egprs.IR_mem_full[1]);
  /* Check the polling bit and queue a qual meas node
   * incase it is set.
   */
  if( ( 0 != pl1_mcs_hdr_store.e_sp )
    &&( FALSE != dl_tfi_matched ) )

  {
    /* If we are assigned an UNACK mode DL TBF. Don't send SEND PDAN REQ
     * to RLC when either FBI bit's are set as it will be discarded since
     * a PCA is sent instead
     */

    if ((transfer_data_ptr->rlc_mode == RLC_UNACK)&&
       ((egprs_dl_fifo.fbi_e_1 & 0x2) || (egprs_dl_fifo.fbi_e_2 & 0x2)) )
    {
      MSG_GERAN_LOW_1_G(" Not Sending L1_RLC_PDAN_REQ [%d]",gl1_get_FN( gas_id ));
    }
    else
    {
      /* Check for extended dynamic && concurrency and common timeslots */

      if( !(
             ( MAC_L1_MAC_MODE_EXTENDED_DYNAMIC == l1_transfer_data[gas_id].mac_mode ) &&
             ( L1_DYNAMIC_DOWN_ALLOC == transfer_data_ptr->current_alloc_type ) &&
             ( GPL1_INVALID ==
                          transfer_data_ptr->dynamic_tbf_ptr->usf_value[ msg_data->tn ] )
           )
        )
      {
          /* A check for payload type is required ! */
          gpl1_egprs_create_qual_meas_node( msg_data->tn,
                                            pl1_mcs_hdr_store.rrbp,
                                            pl1_mcs_hdr_store.e_sp,
                                            pl1_mcs_hdr_store.bsn1, gas_id );
      }
      else
      {
          /* Ignore the polling as an MS operating in extended dynamic
           * allocation need only respond to data block polling only
           * when received on a pdch common for reception and transmission.
           */
          MSG_GERAN_MED_1_G(" Polling ignored on timeslot %u ", msg_data->tn);
      }
    }

  }
}

/*===========================================================================

FUNCTION  gpl1_init_egprs_incr_redundancy( )

DESCRIPTION
  This function is called in the transfer init state to setup
  incremental redundancy i.e. configure NPL1.

  Sets the rlc mode in the transfer data structure.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_init_egprs_incr_redundancy( transfer_data_T *transfer_data_ptr,
                                              uint32           FN,
                                              gas_id_t gas_id )
{
  /* Initialise variable, used to store the rlc mode,
   * included in the init msg to invalid.
   */
  mac_rlc_mode_T  config_rlc_mode = RLC_MODE_INVALID;

  /* @@Warning: This assumes that the control_dynamic_tbf
   * function is passed current dynamic tbf ptr when it
   * is called in control_tbf_transfer( ). This is currently
   * true and hence there is no problem.
   */
  const dynamic_tbf_information_T* dynamic_tbf_ptr =
       transfer_data_ptr->dynamic_tbf_ptr;

  if ( transfer_data_ptr->current_alloc_type == L1_DOWNLINK_ALLOC)
  {
    if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
    {
      if( RLC_MODE_INVALID != dynamic_tbf_ptr->dl_tbf_ptr->rlc_mode )
      {
         config_rlc_mode = dynamic_tbf_ptr->dl_tbf_ptr->rlc_mode;
      }
    }
  }

  if (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS)
  {
    if( RLC_MODE_INVALID != config_rlc_mode )
    {
      if( RLC_ACK == config_rlc_mode )
      {
        /* Reset the FULL BSN range for DL IR */
        gl1_msg_IR_clear_memory(0,2048,gas_id);

        MSG_GERAN_HIGH_0_G(" Clear DL IR window 0 - 2048");

        /* This is ACK mode and we are establishing
         * Downlink TBF so turn on Incremental
         * Redundancy.
        */
        gl1_msg_IR_enable( TRUE ,gas_id);

#ifdef FEATURE_GSM_EGPRS_IR_WS_REDUCTION
        /* Set opertaional window size used by GRLC to take descision
        ** to set ms_out_mem
        */
        grlc_dl_set_oper_win_size(gas_id,gl1_msg_get_ir_oper_win_size(gas_id));
#endif /* FEATURE_GSM_EGPRS_IR_WS_REDUCTION */

        MSG_GERAN_HIGH_1_G(" IR ON %d",FN);

        if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
        {
          if( FALSE != dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi_present )
          {
            MSG_GERAN_MED_3_G(" EGPRS IR DL tfi %u ",
                                         dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,
                                         0,
                                         0 );

            /* Tell NPL1 about the valid value */
             gl1_msg_IR_set_tfi( dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,gas_id );
#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
             transfer_data_ptr->t3192_tfi_match_count = 0;
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
          }
        }
      }
      else
      {
        /* RLC mode is UNACK so turn OFF
        * Incremental Redundancy.
        */
        gl1_msg_IR_enable( FALSE,gas_id );

        /* Reset the FULL BSN range for DL IR */
        gl1_msg_IR_clear_memory(0,2048,gas_id);

        MSG_GERAN_HIGH_1_G("IR OFF (UNACK) %d mem cleared",FN);

        /* Let FW know tfi to avoid message decode when there thre is a TFI mismatch */
        if( (NULL != dynamic_tbf_ptr->dl_tbf_ptr) &&
            (FALSE != dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi_present ))
        {
          MSG_GERAN_MED_3_G(" EGPRS IR DL tfi %u ",
                      dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,
                      0,
                      0 );
          gl1_msg_IR_set_tfi( dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,gas_id );
#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
          transfer_data_ptr->t3192_tfi_match_count = 0;
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
        }
      }
    } /* End of if RLC ACK mode */
    else
    {
      /* This is an uplink assignment
      ** so turn OFF Incremental Redundancy
      */
      gl1_msg_IR_enable(FALSE,gas_id);

      /* Reset the FULL BSN range for DL IR */
      gl1_msg_IR_clear_memory(0,2048,gas_id);

      MSG_GERAN_HIGH_1_G("IR OFF (UL) %d mem cleared",FN);
    } /* End of else rlc mode is invalid */
  } /* End of if tbf mode is EGPRS */

  /* Set the rlc mode in the transfer structure */
  if( RLC_MODE_INVALID != config_rlc_mode )
  {
    /* Don't set it unless it is valid to prevent
     * partial ul/dl/pdch release messages from
     * erroroneously changing rlc mode.
    */
    transfer_data_ptr->rlc_mode = config_rlc_mode;

    /* Printout debug */
    if( RLC_ACK == config_rlc_mode )
    {
      MSG_GERAN_MED_0_G(" RLC ACK mode ");
    }
    else
    {
      MSG_GERAN_MED_0_G(" RLC UnACK mode ");
    }
  }

} /* gpl1_init_egprs_incr_redundancy */

/*===========================================================================

FUNCTION  gpl1_reconfig_egprs_incr_redundancy( )

DESCRIPTION
  This function is called in the transfer reconfig state to setup
  incremental redundancy i.e. configure NPL1.

  Updates the rlc mode in the transfer data structure.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_reconfig_egprs_incr_redundancy( transfer_data_T *transfer_data_ptr,
                                          uint32           FN,
                                          gas_id_t         gas_id )
{
  /* Initialise variable, used to store the rlc mode,
   * included in the init msg to invalid.
   */
  mac_rlc_mode_T  config_rlc_mode = RLC_MODE_INVALID;

  /* @@Warning: This assumes that the control_dynamic_tbf
   * function is passed current dynamic tbf ptr when it
   * is called in control_tbf_transfer( ). This is currently
   * true and hence there is no problem.
   */
  const dynamic_tbf_information_T* dynamic_tbf_ptr =
     transfer_data_ptr->dynamic_tbf_ptr;

  if ( (transfer_data_ptr->tbf_est_type == DOWNLINK_TBF_ESTABLISHED)  ||
       (transfer_data_ptr->tbf_est_type == UPLINK_DOWNLINK_TBF_ESTABLISHED)
     )
  {
    if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
    {
      if( RLC_MODE_INVALID != dynamic_tbf_ptr->dl_tbf_ptr->rlc_mode )
      {
        config_rlc_mode = dynamic_tbf_ptr->dl_tbf_ptr->rlc_mode;
      }
    }
  }

  /* If the egprs reconfig is occuring due to a DL
   * assignment then track the dl tfi value.
   */
  if( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
  {
    if( RLC_MODE_INVALID != config_rlc_mode )
    {
      if( RLC_ACK == config_rlc_mode )
      {
        MSG_GERAN_HIGH_2_G("IR ON (%d) %d",gl1_msg_is_IR_enabled(gas_id),FN);
        if (gl1_msg_is_IR_enabled(gas_id) == FALSE)
        {
           /* Reset the FULL BSN range for DL IR before turning IR on */
           gl1_msg_IR_clear_memory(0,2048,gas_id);

           /* Turn ON Incremental Redundancy */
           gl1_msg_IR_enable( TRUE,gas_id );

        }

#ifdef FEATURE_GSM_EGPRS_IR_WS_REDUCTION
        /* Set opertaional window size used by GRLC to take descision
        ** to set ms_out_mem
        */
        grlc_dl_set_oper_win_size(gas_id,gl1_msg_get_ir_oper_win_size(gas_id));
#endif /* FEATURE_GSM_EGPRS_IR_WS_REDUCTION */

        if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
        {
            if( FALSE != dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi_present )
            {
                MSG_GERAN_MED_3_G(" EGPRS IR DL tfi %u ",
                        dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,
                        0,
                        0 );

                /* Tell NPL1 about the change in tfi value */
                gl1_msg_IR_set_tfi( dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,gas_id );
#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
                transfer_data_ptr->t3192_tfi_match_count = 0;
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
            }
        }
      }
      else
      {
        /* Turn OFF Incremental Redundancy when
         * a DL assignment is received that changes
         * RLC mode from ACK o UNACK.
         */
        gl1_msg_IR_enable( FALSE,gas_id );

        /* Reset the FULL BSN range for DL IR */
        gl1_msg_IR_clear_memory(0,2048,gas_id);

        /* Let FW know tfi to avoid message decode when there thre is a TFI mismatch */
        if( (NULL != dynamic_tbf_ptr->dl_tbf_ptr) &&
            (FALSE != dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi_present ))
        {
          MSG_GERAN_MED_3_G(" EGPRS IR DL tfi %u ",
                      dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,
                      0,
                      0 );
          gl1_msg_IR_set_tfi( dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,gas_id );
#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
          transfer_data_ptr->t3192_tfi_match_count = 0;
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
        }

        MSG_GERAN_HIGH_1_G("IR OFF %d mem cleared",FN);
      }
    }
    else
    {
      /* This is one of the following:
       * pdch dl release.
       * If this is a dl release msg then
       * switch off Incremental Redundancy.
       */
      if( ( RLC_ACK == transfer_data_ptr->rlc_mode )
        &&( L1_DYNAMIC_ALLOC == transfer_data_ptr->current_alloc_type ) )
      {
        /* Turn OFF Incremental Redundancy when
         * a DL assignment is received that changes
         * RLC mode from ACK o UNACK.
         */
        gl1_msg_IR_enable( FALSE,gas_id );

        /* Reset the FULL BSN range for DL IR */
        gl1_msg_IR_clear_memory(0,2048,gas_id);

        MSG_GERAN_HIGH_1_G("IR OFF %d mem cleared",FN);
      }
    } /* End of else rlc mode is invalid */
  } /* End of if tbf mode is EGPRS */

    /* Set the rlc mode in the transfer structure */
  if( RLC_MODE_INVALID != config_rlc_mode )
  {
    if( transfer_data_ptr->rlc_mode != config_rlc_mode )
    {
      /* Report the change in rlc while in pkt xfer since
       * this should not really happen.
       */
      MSG_GERAN_HIGH_0_G(" RLC mode changed during pkt xfer ");
    }

    /* Don't set it unless it is valid to prevent
     * partial ul/dl/pdch release messages from
     * erroroneously changing rlc mode.
     */
    transfer_data_ptr->rlc_mode = config_rlc_mode;

    /* Printout debug */
    if( RLC_ACK == config_rlc_mode )
    {
      MSG_GERAN_MED_0_G(" RLC ACK mode ");
    }
    else
    {
      MSG_GERAN_MED_0_G(" RLC UnACK mode ");
    }
  }
} /* gpl1_reconfig_egprs_incr_redundancy */

#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
/*===========================================================================

FUNCTION  gpl1_gprs_reset_tfi_when_T3192_running

DESCRIPTION
  Function Resets te GFW TFI to invalid value during DL Release when T3192
  is started, such that when doing   haeader decode TFI check can fail ,
  prevening attempted to deocde payload.

  This serves two purpose
  1. Power saving
  2. In MultiSim DSDA solutionms reduces TX blanking.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_reset_tfi_when_T3192_running(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T  *transfer_data_ptr =
                                     l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  if( (TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf) &&
      (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_OFF)  &&
      (grlc_dl_t3192_status(gas_id) == TMR_ACTIVE)  )
  {
    const dynamic_tbf_information_T* dynamic_tbf_ptr =
                                   transfer_data_ptr->dynamic_tbf_ptr;

     if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
     {
        if( FALSE != dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi_present )
        {
           MSG_GERAN_MED_3_G(" DL_REL_HDR_DEC_ONLY old %u invalid_tfi %u t3192_tfi_match_count %d",
                                        dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi,
                                        dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi == 0 ? 31 : 0,
                                        transfer_data_ptr->t3192_tfi_match_count );

           /* Tell NPL1 about the change in tfi value */
           /* provide invalid TFI in range to disable payload decode in DL Release*/
           /* This helps to shut of RF early , and heps X+G DSDA desence */
           gl1_msg_IR_set_tfi( dynamic_tbf_ptr->dl_tbf_ptr->dl_tfi == 0 ? 31 : 0,gas_id );
           transfer_data_ptr->t3192_tfi_match_count = 0;
        }
     }
   }
 }
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */

/*===========================================================================

FUNCTION  gpl1_gprs_send_L1_MAC_USF_DETECTED

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST.

 This function posts the L1_MAC_USF_DETECTED message on the MAC
 or TEST queue to inform the MAC layer that at least one of the
 allocated USF's has been received.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_l1_mac_usf_detected(gas_id_t gas_id)
{

  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = L1_MAC_USF_DETECTED;
  message->msg.usf_detected.gas_id = gas_id;

  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);

}


/*===========================================================================

FUNCTION  gpl1_gprs_tx_dynamic_pdtch_disc_cb

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
/*lint -save -e715
 * vars msgs_discarded and num_msg_txd  not used unless firmware sim is defined
 */

void gpl1_gprs_tx_dyn_rrbp_disc_cb( gl1_msg_tx_discarded_data_type  msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
                                    uint8                           num_msg_txd,
                                    gas_id_t                        gas_id)
{

#ifdef PL1_FW_SIM
  if (num_msg_txd)
    MSG_GERAN_ERROR_0_G(" PDTCH DISCARDED CALLBACK");
#endif

}

/*lint -restore*/

/*===========================================================================

FUNCTION  gpl1_gprs_tx_dyn_rrbp_usf_util_cb

DESCRIPTION
  This function is called by Non Portable Layer 1 when a scheduled PACCH or
  PDTCH has been marked as scheduled by firmware. The function updates the
  total number of usf counter and the total number of blanked blocks.


DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_tx_dyn_rrbp_usf_util_cb( gl1_msg_usf_utilisation_data_type usf_utilisation,
                                        gas_id_t                          gas_id)
{
  uint32 average, blanked_ratio;
  gpl1_ps_store_T* gpl1_store_ptr;

  gpl1_store_ptr = &gpl1_ps_isr_store[gas_id];

  gpl1_store_ptr->usf_utilisation_stats.frame_num = GSTMR_GET_FN_GERAN(gas_id);
  gpl1_store_ptr->usf_utilisation_stats.total_usfs += usf_utilisation.num_scheduled_blocks;
  gpl1_store_ptr->usf_utilisation_stats.total_blocks_blanked += usf_utilisation.num_blanked_blocks;
  gpl1_store_ptr->usf_utilisation_stats.num_consecutive_blanked_blocks = usf_utilisation.num_consecutive_blanked_blocks;


#define USF_UTILISATION_AVERAGE_LENGTH  500
#define USF_UTILISATION_SCALING_FACTOR  10000      /* 10000->100% of blocks are blanked */
#define USF_UTILISATION_MAX_BLANKING    7000

  /* Calculate average blanked/scheduled blocks */
  if (usf_utilisation.num_scheduled_blocks != 0)
  {
    blanked_ratio = (usf_utilisation.num_blanked_blocks * USF_UTILISATION_SCALING_FACTOR) / usf_utilisation.num_scheduled_blocks;
  }
  else
  {
    blanked_ratio = 0;
  }

  average = gpl1_store_ptr->usf_utilisation_blanking_average;
  gpl1_store_ptr->usf_utilisation_blanking_average = average - (average / USF_UTILISATION_AVERAGE_LENGTH) +
                                                     (blanked_ratio / USF_UTILISATION_AVERAGE_LENGTH);

  MSG_GERAN_DEBUG_OPT_4_G("USF DEBUG, usf utilisation, scheduled_blocks: %d, blanked_blocks: %d, consecutive_blanked_blocks: %d, blanking average: %d %%",
      usf_utilisation.num_scheduled_blocks,
      usf_utilisation.num_blanked_blocks,
      usf_utilisation.num_consecutive_blanked_blocks,
      gpl1_store_ptr->usf_utilisation_blanking_average / 100);


  gpl1_store_ptr->usf_utilisation_stats.blanking_percentage_average = gpl1_store_ptr->usf_utilisation_blanking_average / 100;

#ifdef FEATURE_DUAL_DATA
  /* Check if TX blanking is above threshold and request RR to reduce
   * number of UL slots to 1 */
  if (gpl1_store_ptr->usf_utilisation_blanking_average > USF_UTILISATION_MAX_BLANKING)
  {
    MSG_GERAN_HIGH_0_G("USF DEBUG, blanking average exceeded threshold, Request RR to reduce MSC to 1 UL");
    L1_send_MPH_COEX_MSC_CHANGE_IND(1, 0, gas_id);
  }
#endif /* FEATURE_DUAL_DATA */
}


/*===========================================================================

FUNCTION  gpl1_gprs_test_b_sched_cb

DESCRIPTION
  This function is called by Portable Layer 1 when a scheduled PACCH or
  PDTCH is sucessfully transmitted while setup for test mode B



DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_test_b_sched_cb(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T               *transfer_data_ptr;



  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  GPL1_RESET_USF_DETECT( gas_id );

  if (transfer_data_ptr->dynamic_tbf_ptr->sched_msgs)
  {

    /* NPL1 clears out any messages which are not Txd, so
     * no abort needed
     */

    /*  Reset total scheduled msg count to zero */
    transfer_data_ptr->dynamic_tbf_ptr->sched_msgs = 0;

  }

  /*Reset rrbp tx counter */
  gpl1_gprs_reset_rrbp_msgs_txd(gas_id);

}

/*===========================================================================

FUNCTION  gpl1_gprs_tx_dynamic_pdtch_sched_cb

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
void gpl1_gprs_tx_dyn_rrbp_sched_cb( gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                     uint8                           num_msg_txd, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T               *transfer_data_ptr;
  grlc_ul_fifo_access_block_t   rlc_config_params;
  uint8                         i,sched_msgs;
  uint8                         deferred_msgs = 0;
  uint8                         pacch_pdtch_msg_cnt;
  boolean                       found_msg_ptr=FALSE;
  l1_mac_sig_t                  message_buffer,*message;
  rlc_ul_status_T               local_rlc_buf;

  const gpl1_ps_store_T*        gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  /* Get pointer to ISR store */

  if(gpl1_store_ptr == NULL)
  {
    MSG_GERAN_ERROR_0_G("isr store pointer invalid");
    return;
  }

  /* Check this frame is the second in the radio block
   * preceding the IDLE frame.
   */
  switch( frame_counters[gas_id].FNmod52 )
  {
  case  22:
  case  48:
  {
    /* Initialise highest transmit timeslot to an invalid
     * value before it is set.
     */
    uint8   highest_tx_timeslot = 0xFF;

    uint32  msg = 0; /* Used to loop through transmitted msgs */

    const gl1_msg_tx_scheduled_data_type*  msg_ptr =
      &( msgs_txd[ 0 ] );

    /* Check the transmitted blocks, including RRBP ones
     * and convey the highest transmit timeslot information
     * to the neighbour cell scheduling routines.
     */
    while( msg < num_msg_txd )
    {
      if(  ( 0xFF == highest_tx_timeslot )
         ||( highest_tx_timeslot < ADD_MOD( (uint8)msg_ptr->camp_tn,
                                            (uint8)msg_ptr->tn,
                                            GL1_DEFS_SLOTS_IN_FRAME )
           )
        )
      {
        highest_tx_timeslot = ADD_MOD( (uint8)msg_ptr->camp_tn,
                                       (uint8)msg_ptr->tn,
                                       GL1_DEFS_SLOTS_IN_FRAME );
      }

      msg++;
      msg_ptr++;
    }

    /* Inform the Ncell scheduling routines of the highest transmit
     * timeslot.
     */
    gpl1_pkt_xfer_set_highest_tx_timeslot( highest_tx_timeslot, gas_id );
  }
    break;

  default:
    break;
  }

  /* copy defer_get_buffer into local to work on */
  local_rlc_buf = gpl1_store_ptr->deferred_get_buff;

  /* scan through rrbp scheduled list to determine if an RRBP has been transmitted */
  gpl1_gprs_clean_rrbp_txd_info(num_msg_txd, msgs_txd, gas_id);

  if (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_B)
  {
    /* Call test mode B Tx sched CB handler */
    gpl1_gprs_test_b_sched_cb(gas_id);

    return;
  }
  /* handle any deferred gets from previous block if fifo were busy  and check that
   * access to the fifo is allowed i.e. no CS or TLLI change
   */
  if ((local_rlc_buf.num_msgs) && (transfer_data_ptr->rlc_ul_data_available))
  {
    for (i=0;i< (local_rlc_buf.num_msgs);i++)
    {
      /* Action get mode's on transmitted RLC pointer */
      rlc_config_params.access_mode=GET_MODE;
      rlc_config_params.get_type = GET_WITH_PREVIOUS_READ;
      rlc_config_params.ack_state = local_rlc_buf.ul_desc[i].ack_state;
      rlc_config_params.blk_ptr = local_rlc_buf.ul_desc[i].blk_ptr;

      if (grlc_ul_fifo_access_block(gas_id, &rlc_config_params) != UL_FIFO_ACCESS_OK)
      {
        MSG_GERAN_ERROR_1_G("deffered RLC Get failed [%d]",gl1_get_FN( gas_id ));
      }
    }
    /* Clear out deferred buffer */
    GPL1_RESET_DEFER_BUFF( gas_id );

    /* Clear out the local rlc buffer
     * @@Warning: This is important since deferred blocks
     * are stored momentarly in this buffer before getting
     * copied to the main deffered buffer.
     */
    memset( &local_rlc_buf,
            0,
            sizeof(local_rlc_buf) );

  }

  if (gpl1_store_ptr->usf_detected)
  {
    /*********************************************************
     ***  USF detected on one or all Allocated Timeslots
     *********************************************************/


    /* Inform MAC that an assigned USF's has been detected */
#ifdef DEBUG_RLC
    #error code not present
#endif

    /* calculate and test for total pacch, dummy and RLC messages */
    pacch_pdtch_msg_cnt = (num_msg_txd - gpl1_gprs_get_num_rrbp_msg_txd(gas_id));
    if ( pacch_pdtch_msg_cnt )
    {
      /* Decrement scheduled message count while ignoring any
       * RRBP messages which may have been txd. This is so new
       * messages can be scheduled for the next block
       */
      transfer_data_ptr->dynamic_tbf_ptr->sched_msgs-= pacch_pdtch_msg_cnt;

      /* Scan through pacch/pdtch txd msgs to find any RLC or MAC message pointers */
      for (i=0;i< num_msg_txd;i++)
      {
        /* initialise vars */
        sched_msgs=0;
        found_msg_ptr=FALSE;

        /*  scan through each element in the rlc buffer */
        while ((sched_msgs < GL1_DEFS_MAX_UL_MSGS) && (!found_msg_ptr) )
        {
          /* if RLC data has been transmitted and the ULFIFO is available,
           * i.e. no CS or TLLI change
           */
          if (transfer_data_ptr->rlc_ul_status.ul_desc[sched_msgs].blk_ptr->
                                                           elm.gprs.block == msgs_txd[i].msg_data )
          {

            /************************************************************************
             ****  RLC DATA TXD
             ************************************************************************/

            /* Make sure PL1 is in Uplink TBF */
            switch( transfer_data_ptr->current_alloc_type )
            {
            case  L1_DYNAMIC_ALLOC:
            case  L1_DYNAMIC_DOWN_ALLOC:
                /* Increment the number of rlc data blocks
                 * transmitted.
                 * This is done for log packet generation purposes.
                 */
                gpl1_increment_uplink_tbf_data_blk_count( gas_id );
                break;

            default:
                break;
            }
            /* Check if this is the first RLC block to send be sent */
            if (transfer_data_ptr->first_tx_block)
            {
              /* send L1_MAC_1ST_DATA_BLOCK_SENT message */

              gmac_start_t3166(gas_id);


              message = &message_buffer;

              /* Set up the message header */
              message->task_id = MS_MAC_L1;
              message->sig_id = L1_MAC_1ST_DATA_BLOCK_SENT;
              message->msg.first_data_block_sent.gas_id = gas_id;

              /* Send the message, NULL used for MAC_PH_DATA_IND */
              L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);

              transfer_data_ptr->first_tx_block = FALSE;
            }



            /* Action get mode's on transmitted RLC pointer */
            rlc_config_params.access_mode=GET_MODE;
            rlc_config_params.get_type = GET_WITH_PREVIOUS_READ;
            rlc_config_params.ack_state =
                          transfer_data_ptr->rlc_ul_status.ul_desc[sched_msgs].ack_state;
            rlc_config_params.blk_ptr =
                            transfer_data_ptr->rlc_ul_status.ul_desc[sched_msgs].blk_ptr;
#ifdef DEBUG_RLC
            #error code not present
#endif

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
#endif
            switch ( grlc_ul_fifo_access_block(gas_id, &rlc_config_params) )
            {
            case UL_FIFO_BUSY:
              /* defer gets until next block, assume we won't be denied
               * access for more than 1 block
               */
#ifdef  FEATURE_GSM_GPRS_MSC33

              if (++(local_rlc_buf.num_msgs) > PL1_MULTISLOT_TX)
#else
              if (++(local_rlc_buf.num_msgs) > 4)
#endif
                MSG_GERAN_ERROR_0_G("PL1: Defered get buffer overflow!!");

              local_rlc_buf.ul_desc[deferred_msgs].ack_state =
                                                             rlc_config_params.ack_state;
              local_rlc_buf.ul_desc[deferred_msgs].blk_ptr = rlc_config_params.blk_ptr;
              deferred_msgs++;

              MSG_GERAN_MED_0_G("PL1: RLC GET Deferred");

              break;

            case UL_FIFO_ACCESS_OK:
#ifdef PL1_FW_SIM
              /* success case - get access performed */
              MSG_GERAN_MED_0_G("PL1: RLC GET SUCCESS");
#endif
              break;

            default:
              MSG_GERAN_ERROR_0_G("PL1:GET MODE on RLC ULFIFO failed");
              break;
            }

            transfer_data_ptr->rlc_ul_status.num_msgs--;

            /* stop search for pacch or RLC pointer as we have found it */
            found_msg_ptr=TRUE;

            /****
            **** Determine if PH_READY_TO_SEND requires sending based on the max number
            **** of messages that would need to be scheduled for the next block
            ****/
#ifdef DEBUG_RLC
            #error code not present
#endif

            switch (rlc_config_params.ack_state)
            {
            case TXA_NACK:
            case TXA_PENDING_PACK:
            case TXA_PTX:

              if (grlc_ul_fifo_get_nack_cnt(gas_id) <= transfer_data_ptr->dynamic_tbf_ptr->nts)
              {
                /* Send RLC_PH_READY_TO_SEND */
                grlc_send_ph_ready_to_send_ind(gas_id);
              }
              break;

            case TXA_PACK:
              if (
                   (grlc_ul_fifo_get_ptx_cnt(gas_id) <=
                                             transfer_data_ptr->dynamic_tbf_ptr->nts) ||
                   (grlc_ul_fifo_get_nack_cnt(gas_id) <=
                                             transfer_data_ptr->dynamic_tbf_ptr->nts)
                 )
              {
                /* Send RLC_PH_READY_TO_SEND */
                grlc_send_ph_ready_to_send_ind(gas_id);
              }

              break;

            default:
              /* All other ack states's are unsused so report error and continue */
              MSG_GERAN_ERROR_1_G("RLC UL ACK STATE incorrect %d",rlc_config_params.ack_state);
              break;
            } /* End of switch */

          }
#if ((defined FEATURE_GSM_DTM) && (defined FEATURE_GSM_EDTM) )
          else if( NULL != transfer_data_ptr->mac_ul_ctrl_buff_ptr )
#else
          else if (transfer_data_ptr->mac_ul_ctrl_buff_ptr->
                                    pacch_data[sched_msgs].control_block == msgs_txd[i].msg_data )
#endif
          {

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
            if(  ( FALSE != transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[sched_msgs].valid )
               &&( msgs_txd[i].msg_data == transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[sched_msgs].control_block )
              )
            {
#endif
              /****
               **** a MAC UL PACCH msg has been transmitted so update pacch tx'd pointer
               ****/
#ifdef DEBUG_UL_PACCH
              MSG_GERAN_HIGH_3_G("PACCH/U txd [%d] %X %X",
                                    gl1_get_FN( gas_id ),
                                    msgs_txd[i].msg_data[0],
                                    msgs_txd[i].msg_data[1]);

              MSG_GERAN_HIGH_3_G("PACCH/U txd  %d %X %X",
                                    msgs_txd[i].msg_data[2],
                                    msgs_txd[i].msg_data[3],
                                    msgs_txd[i].msg_data[4]);
#endif

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
#endif

              /* The 2nd octet of UL control mag contains the
               * UL msg type in the 6 msb bits
               */
              if( ((msgs_txd[i].msg_data[1] & 0xfc) >>2) == 0x5)
              {
                /*UL control msg is PRR*/
                gpl1_gprs_send_l1_mac_single_block_sent(gas_id);
              }

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
              /* Set the boolean to invalid since the block has been
               * transmitted correctly.
               */
              transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[sched_msgs].valid = FALSE;
#endif

              /* Set the last txd pointer */
              transfer_data_ptr->mac_ul_ctrl_buff_ptr->last_pacch_txd_ptr =
                &( transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data[sched_msgs] );

              /* decrement scheduled pacch msgs count as a PACCH has been transmitted */
              transfer_data_ptr->mac_ul_ctrl_buff_ptr->sched_msgs--;

              /* stop search for pacch or RLC pointer as we have found it */
              found_msg_ptr=TRUE;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
            } /* end of msg is valid and ctr blk ptrs match */
            else
            {
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
            }
#endif
          }   /* end of else */
#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
          else
          {

#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
          }
#endif

          /* move on to next message */
          sched_msgs++;

        }  /* end of while(sched_msgs=0.. */

        /* Is this a dummy control block ?
         * Check both memory locations where dummy
         * control blocks are stored in PL1.
         */
        if( transfer_data_ptr->dummy_control_block == msgs_txd[ i ].msg_data )
        {
            /* Make sure PL1 is in Uplink TBF */
            switch( transfer_data_ptr->current_alloc_type )
            {
            case  L1_DYNAMIC_ALLOC:
            case  L1_DYNAMIC_DOWN_ALLOC:
              /* Increment the number of rlc data blocks
               * transmitted.
               * This is done for log packet generation purposes.
               */
                gpl1_increment_uplink_tbf_dummy_blk_count( gas_id );
                break;
            default:
                break;
            }
        }

      }    /* end of for(i=0.. */
    }      /* end of if (pacch_pdtch_msgs=.... */

    GPL1_RESET_USF_DETECT( gas_id );

  }  /* End of if usf_detected */

  /* Now check to see if there are any scheduled messages which
   * have not been transmitted
   */
  if (transfer_data_ptr->dynamic_tbf_ptr->sched_msgs)
  {

    /* NPL1 clears out any messages which are not Txd, so
     * no abort needed
     */

    /*  Reset total scheduled msg count to zero */
    transfer_data_ptr->dynamic_tbf_ptr->sched_msgs = 0;

    /* Check if any pacch msgs have been scheduled */
    if( NULL != transfer_data_ptr->mac_ul_ctrl_buff_ptr )
    {
      if (transfer_data_ptr->mac_ul_ctrl_buff_ptr->sched_msgs)
      {
        /* align transmitted pacch buffer pointer with pacch_out_ptr */
        transfer_data_ptr->mac_ul_ctrl_buff_ptr->pacch_data_out_ptr =
        transfer_data_ptr->mac_ul_ctrl_buff_ptr->last_pacch_txd_ptr;


        /* increment the pending msg count by the number of previously scheduled msgs */
        transfer_data_ptr->mac_ul_ctrl_buff_ptr->pending_msgs+=
        transfer_data_ptr->mac_ul_ctrl_buff_ptr->sched_msgs;

        /* set schedule count to zero as any scheduled messages are now placed back
         * in the FIFO to be scheduled on the next frame
         */
        transfer_data_ptr->mac_ul_ctrl_buff_ptr->sched_msgs=0;

        /* scheduled msgs now placed back into the fifo so inform PL1 that pacch
         * msgs are now available
         */
        transfer_data_ptr->mac_ul_ctrl_available            = TRUE;
        transfer_data_ptr->mac_ul_ctrl_buff_ptr->data_valid = TRUE;

        /* report exception if too many msgs are in pacch fifo */
        if (transfer_data_ptr->mac_ul_ctrl_buff_ptr->pending_msgs > MAX_NUM_UL_PACCH_MSGS)
        {
          MSG_GERAN_ERROR_0_G("PL1: ERROR MAC UL PACCH BUFFER OVERFLOW");
        }

      } /* end of if pacch_msgs ..*/
    }

    /* If we haven't transmitted all our scheduled RLC PDU's
     * then inform RLC to abort them
    */

    /* If a GET_MODE done above gets deferred , L1 should not perform an abort
     * as this would make RLC counters go out of sync.
    */
    if ( 0 == deferred_msgs)
    {
      gp1l_gprs_abort_ul_fifo_access(gas_id);
    }

  }/* end of if(sched_msgs.. */


  /* Write back defered RLC buffer to isr ps store */
  gpl1_gprs_update_deferred_buff(&local_rlc_buf,
                                 FALSE,
                                 UPDATE_BUFFER,
                                 gas_id);

  /* Clear out the RLC Read buffer */
  memset(&transfer_data_ptr->rlc_ul_status,0,sizeof(rlc_ul_status_T) );

  /* Set rlc status pointer to point to the begining of the buffer for
   * next block
   */
  transfer_data_ptr->rlc_ul_status.ul_desc_ptr  =
                                            &transfer_data_ptr->rlc_ul_status.ul_desc[0];

  /*Reset rrbp tx counter */
  gpl1_gprs_reset_rrbp_msgs_txd(gas_id);

}

/*===========================================================================

FUNCTION  gpl1_gprs_rx_dynamic_pdtch_data_cb

DESCRIPTION
  This function is called by Non Portable Layer 1 when a scheduled PACCH or
  PDTCH is received.


  Passes back the message data and the msg_hdr which descrbes the
  received data.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_rx_dyn_data_cb(gl1_defs_rx_pkt_data_type  *msg_data, boolean cb_valid, gas_id_t gas_id)

{
  volatile ISRTIM_CMD_BLK    *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T            *transfer_data_ptr;
  pl1_radio_block_header_t   rlc_data_hdr;
  grlc_dl_fifo_op_result_t   res;
  dsm_item_type              *dsm_ptr;
  dBx16_T                    pr_value=0;
  boolean                    message_valid=FALSE;
  static uint8               data_cb_calls[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
  boolean                    dl_tfi_matched = FALSE;
  uint8                      data_cnt; /* for test_mode_b */

  gprs_agc_info_t            gprs_agc_info;
  dword                      new_fn = gl1_get_FN( gas_id );
  static dword               old_fn[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( L1_GPRS_INVALID_FN ) };
  uint8                      unslamed_ts =0;
  uint8                      dl_tfi;
  boolean                    tfi_matched = FALSE;
  uint8                      usf_received;

  /* Get the current absolute tdma frame number from the GSM timer */
  const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id );

  /* Get pointer to ISR store */
  const gpl1_ps_store_T*  gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

  usf_dl_tfi_info_t          usf_dl_tfi_info =
      { FALSE, FALSE, 0xffff, 0xffff, 0xffff, GL1_DEFS_PDCH_UNALLOCATED };

  if(gpl1_store_ptr == NULL)
  {
    MSG_GERAN_ERROR_0_G("isr store pointer invalid");
    return;
  }

  /* if TRANSFER_STOP sub state has been entered then abort callbacks */
  if ((gpl1_store_ptr->abort_callbacks) || (l1_transfer_data[gas_id].abort_receives))
  {
    return;
  }

  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;


  /* Workout the absolute timeslot this message was received
   * on. This relies on the relative and camp timeslot values
   * reported by NPL1.
   */
  unslamed_ts  = (uint8) ADD_MOD( (uint8) (msg_data->tn),
                                  (uint8) (msg_data->camp_tn),
                                  GL1_DEFS_SLOTS_IN_FRAME ) ;

  msg_data->tn = (gl1_defs_tn_type) unslamed_ts;

#ifdef PL1_FW_SIM
  MSG_GERAN_LOW_0_G("pl1: DYNAMIC ALLOC -> DATA CALLBACK RCVD");
#endif


  /* Check to see if this data callback contains valid data */
  if (cb_valid)
  {


    /* Check to ensure we are only processing CS1.. CS4 for GPRS RLC/MAC API */
    if( msg_data->coding_scheme <= GL1_DEFS_CS4_CODING )
    {
    
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
       #error code not present
#endif
      /* Check if message was decoded correctly */
      /* Add mask here to check for CRC status only */
      if (msg_data->msg[0].crc_passed)
      {
        uint8  payload_type = CHECK_PAYLOAD_TYPE( msg_data->msg[0].data );

        if( ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
          &&( GL1_DEFS_CS1_CODING == msg_data->coding_scheme ) )
        {
            switch( transfer_data_ptr->l1_test_mode )
            {
            case  L1_TEST_MODE_SRB_OFF:

                switch( payload_type )
                {
                case  L1_DATA_BLOCK:
					MSG_GERAN_MED_0_G(" test_mode: L1_TEST_MODE_SRB_OFF, payload_type: L1_DATA_BLOCK ");					
                    usf_received = (gl1_defs_usf_type)(msg_data->msg[0].data[0] & USF_BITMASK);
                    tfi_matched = CHECK_RLC_TFI( msg_data->msg[0].data, transfer_data_ptr->dl_tbf_ptr->dl_tfi);

                    if( (tfi_matched == FALSE) && ( (byte)usf_received !=
                                                    transfer_data_ptr->dynamic_tbf_ptr->usf_value[ msg_data->tn] ))
                    {
                      MSG_GERAN_HIGH_0_G(" TFI and USF not matching, Ignoring CS-1 Data block ");
                      break;
                    }

                case  L1_CTRL_BLOCK_PLUS_OPTIONS:			
                    /* Warning: Do we have to wait for receiving a block
                     * before tasking DSP for transmission ?
                     */
                    MSG_GERAN_MED_1_G("test_mode: L1_TEST_MODE_SRB_OFF, payload_type: L1_CTRL_BLOCK_PLUS_OPTIONS, SRB ON FNmod13: %u ",frame_counters[gas_id].FNmod13);

                    transfer_data_ptr->l1_test_mode = L1_TEST_MODE_SRB_ON;
                    gl1_srb_set_ul_mod_mode(transfer_data_ptr->dynamic_tbf_ptr->srb_mode,gas_id);
                    gl1_msg_pdch_set_loopback( TRUE,gas_id );
                    break;

                default:
                    /* No action required */
                    break;
                }

                break;

            case  L1_TEST_MODE_SRB_ON:

                switch( payload_type )
                {
                case  L1_CTRL_BLOCK:
                case  L1_RESERVED_BLOCK:			
                    MSG_GERAN_MED_1_G("test_mode: L1_TEST_MODE_SRB_ON, payload_type: L1_CTRL_BLOCK/L1_RESERVED_BLOCK, SRB OFF FNmod13: %u ",frame_counters[gas_id].FNmod13);
                    /* Delay Opening SRB loop until the frame
                     * before the next Radio block
                     */
                    gl1_egprs_open_srb_loopback( gas_id );
                    break;

                default:
                    /* No action required */
                    break;
                } /* End of switch on payload */

                break;

            default:
                /* No special functionality required for
                 * L1_TEST_MODE_B / L1_TEST_MODE_OFF
                 */
                break;
            } /* End of switch on l1_test_mode */
        }     /* End of if statement */
        /* check payload type of incoming message and that a downlink tbf
         * has been allocated
         */

        if( (payload_type == L1_DATA_BLOCK) &&
            (transfer_data_ptr->dl_tbf_ptr) )
        {
          uint8 dl_ts_allocation = GPL1_INVALID_TN;
         /* calculate if we are potentially receiving the decoded block from the previous tbf.
          * If so this block needs to be compared with the previous tbf's TFI
          */
          if( (transfer_data_ptr->dl_tbf_ptr->dl_tfi_st != L1_GPRS_INVALID_FN) &&
              (! gpl1_gprs_is_frame_num_later(current_fn,ADD_FN(transfer_data_ptr->dl_tbf_ptr->dl_tfi_st,MAX_DECODE_FN_DIFF))) &&
              ( GPL1_TFI_INVALID != transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi))
          {
            dl_tfi = transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi;
            dl_ts_allocation = transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc;
            MSG_GERAN_MED_2_G("using previous - dl tfi %d ts_alloc %0x",dl_tfi,dl_ts_allocation);
          }
          else
          {
            dl_tfi = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
            dl_ts_allocation = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
            transfer_data_ptr->dl_tbf_ptr->dl_tfi_st = L1_GPRS_INVALID_FN;
          }

          /* check if this mcs data block is addressed to us */
          dl_tfi_matched = CHECK_RLC_TFI( msg_data->msg[0].data, dl_tfi);
          if ( (dl_tfi_matched) &&
               (!(dl_ts_allocation & tn_mask[msg_data->tn]))
             )
          {
            /* if dl tfi matched and the block received on the allocated timeslots then accept it other wise it should
            ** be discarded.
            */
            dl_tfi_matched = FALSE;
            MSG_GERAN_HIGH_2_G( "GPL1 ignoring dl data blk rvcd on incorrect ts=%0x , prev alloc %0x",
                      transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation,dl_ts_allocation);
          }

          if ( (dl_tfi_matched) &&
               (GPL1_TFI_INVALID == transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi) &&
               (!(transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation & tn_mask[msg_data->tn]))
             )
          {
            /* if new downlink is being assigned then the previous_dl_tfi should be invalid
            ** , and if the data block is decoded with the previous ul_ts allocation ,
            ** then it should be discarded.
            */
            dl_tfi_matched = FALSE;
            MSG_GERAN_HIGH_3_G("GPL1 previous_dl_tfi=%d is invalid a new dl tbf, dl_ts_alloc=%x msg_tn=%d ",
                      transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi,
                      transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation,
                      msg_data->tn);
          }

          /* check downlink TFI to see if data block belongs to the MS
           * if it belongs then send to RLC */

          if (
                dl_tfi_matched
             )
          {

            usf_dl_tfi_info.dl_tfi_match = TRUE;

#ifdef PL1_FW_SIM

            MSG_GERAN_MED_1_G("rlc data Rcvd tfi ok tn %d",msg_data->tn);
#endif

#ifdef DEBUG_SHOW_PL1_BSN
            #error code not present
#endif
            /* This message is for us so set valid flag for updating agc*/
            message_valid = TRUE;

            if (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_B)
            {
              /* First create rlc data header*/
              rlc_data_hdr.frame_number =
                         SUB_FN( current_fn, GPL1_EGPRS_RRBP_FN_REDUCTION[ frame_counters[gas_id].FNmod13 ] );
              rlc_data_hdr.time_slot    = (uint8)msg_data->tn;
              rlc_data_hdr.cs           = (uint8)msg_data->coding_scheme;

              /* get data_cnt to know if blocks needs discarding or number of block
               * required to generate based on number of ul ts
              */
              data_cnt = gpl1_gprs_is_rlc_dl_data_disc(msg_data->tn, gas_id);

              /* If we are not looping back this timeslot and
               * the FBI bit is Not set, discard the data block
               */
              if( (data_cnt == 0 )&&
                ( ( (msg_data->msg[0].data[1] & 0x1) == 0) ) )
              {
                MSG_GERAN_HIGH_1_G(" RLC block disc tn %d",msg_data->tn);
                rlc_data_hdr.loop_dl_blk = 0;
              }else
              {
                if( (data_cnt == 0 )&&
                    (((msg_data->msg[0].data[1] & 0x1) == 1) ) )
                {
                  data_cnt = 1;
                  MSG_GERAN_HIGH_2_G(" TMB: RLC block accept FBI=1 tn %d  data_cnt %d",
                                  msg_data->tn,
                                  data_cnt);
                }

                MSG_GERAN_LOW_2_G(" TMB: RLC block accept tn %d data_cnt %d",
                                      msg_data->tn,
                                      data_cnt);

                rlc_data_hdr.loop_dl_blk = data_cnt;
              }
              /* Check for extended dynamic && concurrency and common timeslots
               * remove the polling from the header if the check matches.
               */
              if(  (MAC_L1_MAC_MODE_EXTENDED_DYNAMIC == l1_transfer_data[gas_id].mac_mode )  &&
                   (L1_DYNAMIC_DOWN_ALLOC == transfer_data_ptr->current_alloc_type ) &&
                   ( GPL1_INVALID ==
                        transfer_data_ptr->dynamic_tbf_ptr->usf_value[ msg_data->tn ] )
                )
              {
                  /* Clear the Bit4 from the first octet of the header */
                  msg_data->msg[0].data[0] =
                                 L1_CLEAR_RLC_HDR_POLL_BIT( msg_data->msg[0].data[0] );

                  /* Printout a debug message */
                  MSG_GERAN_MED_1_G(" RLC data blk poll bit clrd FNmod13 %u ", frame_counters[gas_id].FNmod13);
              }

              /* create dsm item chain for the data payload */
              dsm_ptr =
                    gl1_pdu_construct(msg_data->msg[0].data,msg_data->msg[0].msg_len);

              if ( dsm_ptr != NULL )
              {
                /* inform RLC of the new dsm item chain */
                res = grlc_dl_fifo_put_block(gas_id, &rlc_data_hdr, dsm_ptr);

                if (res == DL_FIFO_PUT_OK)
                {

                  /* send ph_data_ind every block for test mode B */
                  grlc_send_ph_data_ind(gas_id);

                }else
                {
                  /* Report error and free dsm item as RLC DL FIFO is not available */
                  MSG_GERAN_ERROR_1_G("EGPRS DL FIFO put error %d",res);

                  GPRS_PDU_FREE(&dsm_ptr);
                }

              }else
              {
                MSG_GERAN_ERROR_0_G("Out of DSM Resources");
              }



            }else
            {

              /* TFI matches so convert to dsm item and inform RLC
               * Do this only if MS is in gprs tbf to protect against
               * cases where MS egprs tbf tfi matches the gprs tfi of
               * another MS.
               */
              if( TBF_MODE_GPRS == transfer_data_ptr->egprs_or_gprs_tbf  )
              {
                /* First create rlc data header*/
                rlc_data_hdr.frame_number =
                           SUB_FN( current_fn, GPL1_EGPRS_RRBP_FN_REDUCTION[ frame_counters[gas_id].FNmod13 ] );
                rlc_data_hdr.time_slot    = (uint8)msg_data->tn;
                rlc_data_hdr.cs           = (uint8)msg_data->coding_scheme;

                /* create dsm item chain for the data payload */
                dsm_ptr =
                      gl1_pdu_construct(msg_data->msg[0].data,msg_data->msg[0].msg_len);

                /* Check for extended dynamic && concurrency and common timeslots
                 * remove the polling from the header if the check matches.
                 */
                if( ( MAC_L1_MAC_MODE_EXTENDED_DYNAMIC == l1_transfer_data[gas_id].mac_mode ) &&
                    ( L1_DYNAMIC_DOWN_ALLOC == transfer_data_ptr->current_alloc_type ) &&
                    ( GPL1_INVALID ==
                          transfer_data_ptr->dynamic_tbf_ptr->usf_value[ msg_data->tn ] )
                  )
                {
                    /* Clear the Bit4 from the first octet of the header */
                    msg_data->msg[0].data[0] =
                                   L1_CLEAR_RLC_HDR_POLL_BIT( msg_data->msg[0].data[0] );

                    /* Printout a debug message */
                    MSG_GERAN_MED_1_G(" RLC data blk poll bit clrd FNmod13 %u ", frame_counters[gas_id].FNmod13);
                }

                if ( dsm_ptr != NULL )
                {
                  /* inform RLC of the new dsm item chain */
                  res = grlc_dl_fifo_put_block(gas_id, &rlc_data_hdr, dsm_ptr);

                  if (res == DL_FIFO_PUT_OK)
                  {

                    /* now check if RLC needs the RLC_PH_DATA_IND to be sent */
                    if (grlc_is_rlc_dl_required_context(gas_id, msg_data->msg[0].data) )
                    {
                      grlc_send_ph_data_ind(gas_id);
                    }

                  }else
                  {
                    /* Report error and free dsm item as RLC DL FIFO is not available */
                    MSG_GERAN_ERROR_1_G("EGPRS DL FIFO put error %d",res);

                    GPRS_PDU_FREE(&dsm_ptr);
                  }

                } else
                {
                  MSG_GERAN_ERROR_0_G("Out of DSM Resources");
                }
              }/* End of if tbf mode is GPRS */
              else
              {
                message_valid = FALSE;
              }
            }


          } else
          {
            /********* TFI does NOT match ********/

            /* This message is not for us so unset valid flag for updating agc*/
            message_valid = FALSE;

            usf_dl_tfi_info.dl_tfi_match = FALSE;


          }

        } else if( ( payload_type != L1_DATA_BLOCK)
                 &&(msg_data->coding_scheme == GL1_DEFS_CS1_CODING))
        {
          /* if a control block is received during tbf reconfig boundary,it belongs
              to timefrasme before start time but decoded after start time,and should
              be ignored - MAC may aceept it incorrectly , resulting in
              incorrect TBF reconfiguration , leading to protocol timer expirys and
              other abnormal releases.
          */
          boolean ignore_ctrl_blk = FALSE;

          /* DO MAC PROCESSING */
#ifdef PL1_FW_SIM
          MSG_GERAN_LOW_0_G("MAC CTRL BLOCK RECEIVED");
#endif
          gl1_print_control_block(msg_data->msg[0].data, gas_id);

          /****
           **** Test mode B handling of control block reception
           ****/
          if (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_B)
          {
            /* Mark reception of control block if received on uplink timeslot */
            if (unslamed_ts == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
            {
              gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status = 0x1;
            }

            if (unslamed_ts == gpl1_gprs_tm_b_store[gas_id].ul_tn[1])
            {
              gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status |= 0x2;
            }

#ifdef FEATURE_GSM_GPRS_MSC33
            if (unslamed_ts == gpl1_gprs_tm_b_store[gas_id].ul_tn[2])
            {
              gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status |= 0x4;
              MSG_GERAN_MED_2_G(" rx ctrl tn_2 %d ctrl_s %d",
                        gpl1_gprs_tm_b_store[gas_id].ul_tn[2],
                        gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status);
            }
#endif /* FEATURE_GSM_GPRS_MSC33 */

            MSG_GERAN_LOW_3_G(" rx ctrl tn_0 %d tn_1 %d ctrl_s %d",
                                              gpl1_gprs_tm_b_store[gas_id].ul_tn[0],
                                              gpl1_gprs_tm_b_store[gas_id].ul_tn[1],
                                              gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status);
          }

          /* Always AGC on control blocks, GCF 14.16.1 will change power with
           * DL TBF only assigned while only sending CS1 control blocks on PACCH
           */
          message_valid = TRUE;

         /* check for ctrl block rcvd during block boundary and ignore it */
         if ( (transfer_data_ptr->dl_tbf_ptr != NULL) &&
              (transfer_data_ptr->dl_tbf_ptr->dl_tfi_st != L1_GPRS_INVALID_FN) &&
              (! gpl1_gprs_is_frame_num_later(current_fn,ADD_FN(transfer_data_ptr->dl_tbf_ptr->dl_tfi_st,MAX_DECODE_FN_DIFF)))
            )
         {
           MSG_GERAN_MED_2_G("GL1 ctrl blk ignored before reconfig boundary curr_fn %d , dl_tfi_st %d",
                                  current_fn,transfer_data_ptr->dl_tbf_ptr->dl_tfi_st);
           ignore_ctrl_blk = TRUE;
         }
         else
         {
           if (transfer_data_ptr->dl_tbf_ptr != NULL)
           {
             /* reset the starttime such that we dont ignore ctrl blk in next boundary */
             transfer_data_ptr->dl_tbf_ptr->dl_tfi_st = L1_GPRS_INVALID_FN;
           }
         }

         /* check for ctrl block rcvd during block boundary and ignore it */
         if ( (transfer_data_ptr->dynamic_tbf_ptr != NULL) &&
              (transfer_data_ptr->dynamic_tbf_ptr->ul_tfi_st != L1_GPRS_INVALID_FN) &&
              (! gpl1_gprs_is_frame_num_later(current_fn,ADD_FN(transfer_data_ptr->dynamic_tbf_ptr->ul_tfi_st,MAX_DECODE_FN_DIFF)))
            )
         {
           MSG_GERAN_MED_2_G("GL1 ctrl blk ignored before reconfig boundary curr_fn %d , ul_tfi_st %d",
                                  current_fn,transfer_data_ptr->dynamic_tbf_ptr->ul_tfi_st);
           ignore_ctrl_blk = TRUE;
         }
         else
         {
           if (transfer_data_ptr->dynamic_tbf_ptr != NULL)
           {
             /* reset the starttime such that we dont ignore ctrl blk in next boundary */
             transfer_data_ptr->dynamic_tbf_ptr->ul_tfi_st = L1_GPRS_INVALID_FN;
           }
         }

         if ( TRUE == ignore_ctrl_blk)
         {
           /* get message type and pass on broadcast message to MAC
              reject mobile specfic message like PUA,PDA,PTR etc
           */
           uint8 msg_type;

           if( payload_type == L1_CTRL_BLOCK_PLUS_OPTIONS )
           {
             /* message type needs to decoded as option octets present */
             msg_type =  gpl1_get_control_msg_type(msg_data->msg[0].data);
           }
           else
           {
             msg_type = ((msg_data->msg[0].data[1] & 0xFC) >> 2);
           }

           switch (msg_type)
           {
              /* broadcast message types */
              case 8:  /*Pkt TBF REL*/
              case 9:  /*PUAN*/
              case 35 : /* Pkt PDCH REL */
              case 36 : /* Pkt PRACH Params */
              case 37 : /* PDDCB */
              case 40 : /* PSI 16 */
              case 48 :  /*PSI 6 */
              case 49 :  /*PSI 1 */
              case 50 :  /*PSI 2 */
              case 51 :  /*PSI 3 */
              case 52 :  /*PSI 3 bis */
              case 53 :  /*PSI 4 */
              case 54 :  /*PSI 5 */
              case 55 :  /*PSI 13 */
              case 56 :  /*PSI 7 */
              case 57 :  /*PSI 8 */
              case 58 :  /*PSI 14 */
              case 60 :  /*PSI 3 ter */
              case 61 :  /*PSI 3 quater */
              case 62 :  /*PSI 15 */
                ignore_ctrl_blk = FALSE;

                MSG_GERAN_MED_3_G("GL1 ignore ctrl block cancelled msg_type %d payload_type %d data1 %0x",msg_type,payload_type,msg_data->msg[0].data[1]);
                break;

              default :
                MSG_GERAN_HIGH_3_G("GL1 ignore ctrl block msg_type %d payload_type %d data1 %0x ",msg_type,payload_type,msg_data->msg[0].data[1]);
                break ;
           } /* switch (msg_type) */
         } /* if ( TRUE = ignore_ctrl_blk) */

         if ( FALSE == ignore_ctrl_blk )
         {
          /* send mac_ph_data_ind message */
          gpl1_gprs_send_mac_ph_data_ind
                              (
                                SUB_FN( current_fn,
                                        GPL1_EGPRS_RRBP_FN_REDUCTION[ frame_counters[gas_id].FNmod13 ] ),
                                (uint8)msg_data->tn,
                                msg_data->msg[0].data, gas_id
                              );
         } /* FALSE == ignore_ctrl_blk  */
        } else
        {
          /* Payload type is RLC but we don't have a downlink tbf currently active,
           * i.e. we don't have a DL TFI to ascertain if the received data is for
           * us or not. This means we are receiving downlink data for another mobile
           */

          /* Check  to see if our USF is allocated on this RLC data block
           * Let AGC include the RXLEV if our USF is detected
           */

          MSG_GERAN_HIGH_3_G("PT %d CS %d [%d]",payload_type,msg_data->coding_scheme,gl1_get_FN( gas_id ));



          if((transfer_data_ptr->dynamic_tbf_ptr != NULL)&& ((msg_data->msg[0].data[0]  & USF_BITMASK) ==
                         (transfer_data_ptr->dynamic_tbf_ptr->usf_value[ msg_data->tn])))
          {
            /* Use the Rxlev sample for AGC */
            message_valid=TRUE;
          }else
          {
            message_valid=FALSE;
          }


          MSG_GERAN_LOW_0_G("PL1: Payload error");
        }

      } else
      {
        /* This message is not for us so unset valid flag for updating agc*/
        message_valid = FALSE;

      } /* end of if(crc_passed .. */

      if ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
      {
        /* Instantiate structure with maximum bep values */
        gpl1_egprs_decode_metrics_T  egprs_decode_metrics =
        { { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, GL1_DEFS_CS1_CODING, FALSE

#if  EGPL1_OPTIMISE_MEAN_BEP
            ,FALSE, FALSE
#endif
        };

        /* Report raw ber measurements for quality measurement reporting purposes */
        egprs_decode_metrics.decode_error =
        (boolean) !(msg_data->msg[0].crc_passed);

        egprs_decode_metrics.coding_type = msg_data->coding_scheme;

       /* As per spec 10.2.3.2.1  */
       /* The quality parameters shall be, for the radio blocks intended for this MS only
       ** (i.e. at least radio blocks where the TFI identifying this MS can be decoded from
       ** the RLC/MAC header and radio blocks where the TFI identifying this MS can be decoded
       ** from the RLC/MAC control block header
       ** Payload check for control block part of check_rlc_tfi_in_ctrl_blk macro
       */
        if( (transfer_data_ptr->dl_tbf_ptr) &&
            (FALSE == egprs_decode_metrics.decode_error) &&
            (GL1_DEFS_CS1_CODING == msg_data->coding_scheme)
          )
        {
          /* check if this cs 1 ctrl block is addressed to us */
          dl_tfi_matched = CHECK_RLC_TFI_IN_CTRL_BLK( msg_data->msg[0].data,
                                                      transfer_data_ptr->dl_tbf_ptr->dl_tfi);
        }
        else
        {
          dl_tfi_matched = FALSE;
        }

        gpl1_egprs_serv_cell_data_cb( msg_data->tn,
                                      &( egprs_decode_metrics ),
                                      dl_tfi_matched,
                                      &usf_dl_tfi_info, gas_id );
      }
    }  /* end of if(coding_scheme => CS1 and <=CS4 .. */

    /* otherwise check if this data is received on a DL EGPRS TBF and
     * its MCS 1.. 9
     */
    else if ((transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS) &&
             (transfer_data_ptr->dl_tbf_ptr) &&
             (msg_data->coding_scheme >= GL1_DEFS_MCS1_CODING) &&
             (msg_data->coding_scheme <= GL1_DEFS_MCS9_CODING))
    {
 #ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
        #error code not present
#endif
      gpl1_gprs_handle_mcs_data_cb(msg_data,&pr_value,&message_valid,&usf_dl_tfi_info, gas_id);

    } else
    {
      /* Instantiate structure with maximum bep values */
      gpl1_egprs_decode_metrics_T  egprs_decode_metrics =
        { { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, GL1_DEFS_CS1_CODING, FALSE
#if  EGPL1_OPTIMISE_MEAN_BEP
            ,FALSE, FALSE
#endif
        };

        /* Report raw ber measurements for quality measurement reporting purposes */
        egprs_decode_metrics.decode_error =
                       (boolean) !(msg_data->egprs.hdr_crc_passed);

        egprs_decode_metrics.coding_type = msg_data->coding_scheme;

        gpl1_egprs_serv_cell_data_cb( msg_data->tn,
                                      &( egprs_decode_metrics ),
                                      FALSE,                   /* tfi match */
                                      &usf_dl_tfi_info, gas_id );


      /* check if our usf has been detected on an MCS block when we have
       * no downlink TBF allocated
       */
      if (
           (msg_data->egprs.hdr_crc_passed) &&
           (
              ( msg_data->egprs.hdr[0]  & USF_BITMASK) ==
              (transfer_data_ptr->dynamic_tbf_ptr->usf_value[ msg_data->tn] )
           )
         )
      {
         /* Use the Rxlev sample for AGC */
         message_valid=TRUE;
      }else
      {
        message_valid=FALSE;
      }
    }




    /* Read USF value out of EGPRS header or GPRS payload to determine if
     * the USF matches our allocated value
     */

    if ((msg_data->coding_scheme >= GL1_DEFS_MCS1_CODING) &&
       (msg_data->coding_scheme <= GL1_DEFS_MCS9_CODING))
    {
      if (msg_data->egprs.hdr_crc_passed)
      {
        usf_dl_tfi_info.usf = (gl1_defs_usf_type)( msg_data->egprs.hdr[0] & USF_BITMASK);
      }
    }else


    if(msg_data->coding_scheme <= GL1_DEFS_CS4_CODING)
    {
      if (msg_data->msg[0].crc_passed)
      {
        usf_dl_tfi_info.usf =
                             (gl1_defs_usf_type)(msg_data->msg[0].data[0] & USF_BITMASK);
      }
    }


    /* Calculate if the received USF matches our allocated one ( if it exists) */
	if(transfer_data_ptr->dynamic_tbf_ptr != NULL)
	 {
      usf_dl_tfi_info.usf_match =
          ( (byte)usf_dl_tfi_info.usf ==
                          transfer_data_ptr->dynamic_tbf_ptr->usf_value[ msg_data->tn] );
	}

    /*Eng Mode ENG_MODE_RX_QUAL Reporting, only reporting CS1..4, RXQUAL is not used for MCS1..9*/

    if ( (msg_data->msg[0].crc_passed) && (msg_data->coding_scheme <= GL1_DEFS_CS4_CODING))
    {
      uint8 rx_qual = gpl1_gprs_calc_rxqual_lev( msg_data->msg[0].rx_qual, 1 ,gas_id);
      geran_eng_mode_data_write(ENG_MODE_RX_QUAL, &rx_qual, TRUE, gas_id);
    }

    /* log the message metrics */
    if (old_fn[gas_id] != new_fn)
    {
      data_cb_calls[gas_id] = 1;
      old_fn[gas_id] = new_fn;

      if ((msg_data->coding_scheme >= GL1_DEFS_MCS1_CODING) &&
         (msg_data->coding_scheme <= GL1_DEFS_MCS9_CODING))
      {
        gpl1_log_edge_message_metrics(GL1_MSG_DL_PACCH_PDTCH, old_fn[gas_id], (byte)msg_data->tn, &msg_data->egprs.enhEdgeLogPkt,gas_id);
      }

      gpl1_log_gprs_message_metrics_a_ver2(GL1_MSG_DL_PACCH_PDTCH,
                                           old_fn[gas_id],
                                           msg_data,
                                           &usf_dl_tfi_info, gas_id);

    } else
    {
      if ((msg_data->coding_scheme >= GL1_DEFS_MCS1_CODING) &&
         (msg_data->coding_scheme <= GL1_DEFS_MCS9_CODING))
      {
        gpl1_log_edge_message_metrics(GL1_MSG_DL_PACCH_PDTCH, old_fn[gas_id], (byte)msg_data->tn, &msg_data->egprs.enhEdgeLogPkt,gas_id);
      }

      data_cb_calls[gas_id]++;
      if (data_cb_calls[gas_id] == 2)
      {

        gpl1_log_gprs_message_metrics_b(GL1_MSG_DL_PACCH_PDTCH,old_fn[gas_id],msg_data, gas_id);

        gpl1_log_gprs_message_metrics_b_ver2(GL1_MSG_DL_PACCH_PDTCH,
                                             old_fn[gas_id],
                                             msg_data,
                                             &usf_dl_tfi_info, gas_id);

      } else if (data_cb_calls[gas_id] == 3)
      {
        gpl1_log_gprs_message_metrics_c(GL1_MSG_DL_PACCH_PDTCH,old_fn[gas_id],msg_data, gas_id);

        gpl1_log_gprs_message_metrics_c_ver2(GL1_MSG_DL_PACCH_PDTCH,
                                             old_fn[gas_id],
                                             msg_data,
                                             &usf_dl_tfi_info, gas_id);

      } else if (data_cb_calls[gas_id] == 4)
      {
        gpl1_log_gprs_message_metrics_d(GL1_MSG_DL_PACCH_PDTCH,old_fn[gas_id],msg_data, gas_id);

        gpl1_log_gprs_message_metrics_d_ver2(GL1_MSG_DL_PACCH_PDTCH,
                                             old_fn[gas_id],
                                             msg_data,
                                             &usf_dl_tfi_info, gas_id);
      }
#ifdef  FEATURE_GSM_GPRS_MSC33
      else if (data_cb_calls[gas_id] == 5)
      {
        gpl1_log_gprs_message_metrics_e_ver2(GL1_MSG_DL_PACCH_PDTCH,
                                             old_fn[gas_id],
                                             msg_data,
                                             &usf_dl_tfi_info, gas_id);
      }
#endif
    }




    /* Extract the PR value from received data if packet
     * is for us and we received CS1 .. CS4
     */
    if( (message_valid) &&
         (msg_data->coding_scheme <= GL1_DEFS_CS4_CODING)
      )
    {
      /* check if downlink power control is enabled */
      if (transfer_data_ptr->dl_power_control.params_valid)
      {

        pr_value = gpl1_gprs_get_pr_value(msg_data->msg[0].data);
      } else
      {
        pr_value = 0;
      }
    }

    /* Update receiver gain tracking loop.
     * Warning: Pass in the relative timeslot value
     * not the absolute one.
     */

    /* Update AGC on first block after TBF reconfigure or start of PTM */

    sys_algo_agc_process_pdtch(&transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                               &transfer_data_ptr->packet_transfer_div_agc,
#endif
                               (gl1_defs_tn_type) SUB_MOD( (uint8) (msg_data->tn),
                                                           (uint8) (msg_data->camp_tn),
                                                           GL1_DEFS_SLOTS_IN_FRAME ),
                                                           pr_value,
                                                           message_valid, gas_id);




    if (message_valid)
    {
      /***** Construct the AGC log packet *****/

      gprs_agc_info.gprs_channel = (uint8)GL1_MSG_DL_PACCH_PDTCH;

      /* assign currently allocated p0 value */
      gprs_agc_info.p0 = transfer_data_ptr->dl_power_control.po;
      {
        gprs_agc_info.pb =0;
      }

      gprs_agc_info.pr_a = pr_value;

      /* currently not used */
      gprs_agc_info.pr_b =0;
      gprs_agc_info.pr_c =0;
      gprs_agc_info.pr_d =0;
      /* initialise tfi_usf */
      gprs_agc_info.tfi_usf =0;
      /* construct usf/tfi list MSB = TN 0 LSB = TN7*/
      gprs_agc_info.tfi_usf  |= (0x01 << (7 - (uint8)msg_data->tn));

      gpl1_log_agc(&transfer_data_ptr->packet_transfer_agc,&gprs_agc_info, gas_id);
    }




    /*  Serving Cell Measurements */
    /*  Both PBCCH and PCCCH measurements can be taken, measurements taken in
     *  metrics, indicate here if burst correctly decoded, or if not discard
     *  C_Value measurements.
     */
    if ( TBF_MODE_GPRS == transfer_data_ptr->egprs_or_gprs_tbf )
    {
      gpl1_gprs_decode_metrics_T  gprs_decode_metrics;
      gprs_decode_metrics.coding_type        = msg_data->coding_scheme;
      gprs_decode_metrics.rxqual             = msg_data->msg[0].rx_qual;
      gprs_decode_metrics.header_decode_ok   = msg_data->msg[0].crc_passed;
      if( NULL != transfer_data_ptr->dl_tbf_ptr )
      {
        gprs_decode_metrics.tfi_match          = dl_tfi_matched;
      }
      else
      {
        gprs_decode_metrics.tfi_match          = FALSE;
      }
      gprs_decode_metrics.data_payload_error = !msg_data->msg[0].crc_passed;
        gpl1_scell_pkt_transfer_data_cb( msg_data->tn,
                                       &gprs_decode_metrics,
                                       gas_id );
    }
  } /* End of if msg is valid */
}

/*===========================================================================

FUNCTION gpl1_gprs_rx_dyn_metrics_cb

DESCRIPTION
  This function is called by Non Portable Layer 1, 4 times per radio block
  and Passes back the metrics for each frame

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_rx_dyn_metrics_cb(
                                gl1_defs_rx_pkt_metrics_type *md[GL1_DEFS_MAX_DL_MSGS ],
                                uint8                              num_msgs,
                                uint8                              burst_num,
                                gas_id_t gas_id
                                )
{

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    const gpl1_ps_store_T *gpl1_store_ptr;

    boolean first_valid_burst_found = FALSE;

    uint16 i = 0;
    uint16 j = 0;

    /* read usf_detected out of local store */
    gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

    /* if TRANSFER_STOP sub state has been entered then abort callbacks */
    if( FALSE == gpl1_store_ptr->abort_callbacks )
    {
      /* Set up a pointer to the transfer data structure */
      transfer_data_T*  transfer_data_ptr =
                                    l1_tskisr_blk->current_params.L1Data.pTransfer_data;

      /* Get the current absolute frame number */
      const uint32  FN = gl1_get_FN( gas_id  );

      /* loop variable */
      boolean usf_detected_local;

      gl1_power_meas_type pwr_meas_type = PRX_MEAS;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
      dBx16_T  divrx_rssi = GL1_INVALID_POWER_LEVEL;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

      usf_detected_local = gpl1_store_ptr->usf_detected;

#ifdef PL1_FW_SIM
      MSG_GERAN_LOW_0_G("Metrics received for Dynamic alloc Rx");
#endif


      if((num_msgs > 0) && (num_msgs <= MAX_GPRS_BURST_METRICS))
      {
          /*
          Store away all the data available that has SNR values. This is used as a backup if the PTCCH is
          signalling that the PTCCH SNR is bad. Clear out the values at the start of every 104 frame
          multiframe to just store one multiframes worth of data.
          */
          if ( frame_counters[gas_id].FNmod104 == 0 )
          {
            gpl1_reset_pdtch_snr_variables(gas_id);
          }

          {
            uint16 count;
            for ( count=0; count < num_msgs; count++)
            {
              pdtch_snr[gas_id].valid = TRUE;
              /*
              ** use post burst snr as pre mimo snr can be much lower
              ** under interferance to avoid unnecessary reselection trigger.
              */
              pdtch_snr[gas_id].accumulator += gl1_msg_SNR_to_dBx16( md[count]->snr );
              pdtch_snr[gas_id].accumulator_count++;
            }
          }
      }
      else
      {
         /* num_msgs should always be a number between 1 and MAX_GPRS_BURST_METRICS */
         MSG_GERAN_ERROR_1_G("Invalid number of GPRS burst metrics: %d",num_msgs);
      }

      /* Check the metrics validity flag */
      if(md[0]->valid != FALSE)
      {
#ifdef FEATURE_GSM_MDSP_ESAIC
      /* If ESAIC is used in a burst the mDSP sets bit 14 of */
      /* the misc_flag true. Additionally, for AFC tracking */
      /* the flag selects the specific GMSK SAIC threshold. */
      if ( gl1_msg_saic_in_burst( md[0]->misc_flags ) )
      {
        MSG_GERAN_LOW_3_G("SAIC detected mimo_snr %d mod %d freq_off %d",
                md[0]->pre_mimo_trained_complete_snr,md[0]->modulation,md[0]->freq_offset);

        /* Update the time tracking loop with lowest timeslot burst timing offset.
                       * Don't adjust.*/
        gl1_msg_pdch_tt_update ( (int16) md[0]->timing_offset,
                                 gl1_msg_gprs_saic_tt_threshold_snr( md[0] ),
                                 md[0]->modulation,gas_id );

      }
      else
#endif /* FEATURE_GSM_MDSP_ESAIC */
      {
         MSG_GERAN_LOW_3_G("SAIC NOT detected snr %d mod %d freq_off %d",
                 md[0]->snr,md[0]->modulation,md[0]->freq_offset);

         gl1_msg_pdch_tt_update ( (int16) md[0]->timing_offset, md[0]->snr,
                                  md[0]->modulation,gas_id );

      }
      }
      else
      {
         MSG_GERAN_ERROR_0_G(" Invalid pdch burst metrics for lowest time slot");
      }

      gl1_msg_pdch_afc_update(num_msgs,burst_num,md,gas_id);
      /* Log burst metrics */
      if((num_msgs > 0) && (num_msgs <= MAX_GPRS_BURST_METRICS))
      {
        for(i=0; i<num_msgs; i++)
        {
          if((md[i]->valid) && (md[i]->schedule_status & GL1_DEFS_RX_SCHEDULED))
          {
            md[i]->tn = (gl1_defs_tn_type) ADD_MOD( (uint8) md[i]->tn, (uint8) md[i]->camp_tn, GL1_DEFS_SLOTS_IN_FRAME );
#ifdef  FEATURE_GSM_GPRS_MSC33
            /* gpl1_log_gprs_burst_metrics_e not currently supported */
            if(i<(MAX_GPRS_BURST_METRICS-1))
#endif
            {
              gpl1_log_gprs_burst_metrics_ver2_x[i]( GL1_MSG_DL_PACCH_PDTCH, FN, md[i],gas_id );
            }
            gpl1_log_gprs_burst_metrics_x[i]( GL1_MSG_DL_PACCH_PDTCH, FN, md[i] ,gas_id);
          }
          else
          {
            MSG_GERAN_LOW_3_G("Burst %d metrics valid %d schedule_status:%d",md[i]->tn,md[i]->valid,md[i]->schedule_status);
          }
        }
      }
      else if(!((num_msgs > 0) && (num_msgs <= MAX_GPRS_BURST_METRICS)))
      {
        /* num_msgs should always be a number between 1 and MAX_GPRS_BURST_METRICS */
        MSG_GERAN_ERROR_1_G("Invalid number of GPRS burst metrics: %d",num_msgs);
      }
      gl1_msg_pdch_afc_adjust(gas_id);

      /* check 4th metrics to see if one of our allocated USF has been detected */
      if( L1_BURST_4 == burst_num )
      {
        if( 0 != num_msgs )
        {
          uint32  tx_msgs;

          /* Scan through detected usf's */
          for (tx_msgs =0; tx_msgs< num_msgs;tx_msgs++)
          {
            /* initialize num_tn */
            uint32 num_tn = 0;

            while ((!usf_detected_local) && (num_tn < MAX_TN) )
            {
              /* Has one of the allocated USF's been detected */
              if ( (uint8)(md[tx_msgs]->usf) ==
                                transfer_data_ptr->dynamic_tbf_ptr->usf_value[num_tn++] )
              {
                usf_detected_local = TRUE;

                /* Send MAC a usf detected message */
                gpl1_gprs_send_l1_mac_usf_detected(gas_id);

                MSG_GERAN_LOW_2_G("USF DET 1[%d] %d", FN, gpl1_store_ptr->usf_gran_count);

                if (transfer_data_ptr->dynamic_tbf_ptr->usf_granularity)
                {
                  if (gpl1_store_ptr->start_usf_gran_count)
                  {
                    gpl1_ps_isr_store[gas_id].usf_gran_count = 4;
                  }
                }
              }

            } /* End of while loop */
          }   /* End of for loop   */
        }     /* End of if number of msgs is not zero */


        /* Test for USF GRANULARITY */
        if (transfer_data_ptr->dynamic_tbf_ptr->usf_granularity)
        {

          if (gpl1_store_ptr->usf_gran_count)
          {
            MSG_GERAN_LOW_2_G("USF DET 2[%d] %d", FN, gpl1_store_ptr->usf_gran_count);

            usf_detected_local   = TRUE;
            gpl1_ps_isr_store[gas_id].start_usf_gran_count = FALSE;

            gpl1_ps_isr_store[gas_id].usf_gran_count--;
          }

          if (!gpl1_store_ptr->usf_gran_count)
          {
            /* Re-start USF granularity at next usf detected */
            gpl1_ps_isr_store[gas_id].start_usf_gran_count = TRUE;
          }
        }

        /* In the case where we scheduled RLC Tx on FN12 (FN%13) RLC may receive
         * PUAN control message before L1 aborts unsuccessful TX. This is because
         * handling of RLC FIFO API is done in the tx schedule callback handler
         * which in this case happens 1 frame later than the PUAN is received.
         *
         * In all other cases the RLC FIFO API will be called on the SAME frame as
         * when the PUAN control block would be received
         */
        if(usf_detected_local == FALSE)
        {
          grlc_ul_fifo_access_block_t rlc_config_params;
          rlc_ul_status_T             local_rlc_buf;
          uint8                       i;

          /* Perform Deferred GET and Abort */

          /* copy defer_get_buffer into local to work on */
          local_rlc_buf = gpl1_store_ptr->deferred_get_buff;

          /* handle any deferred gets from previous block if fifo were busy  and check that
           * access to the fifo is allowed i.e. no CS or TLLI change
           */
          if ((local_rlc_buf.num_msgs) && (transfer_data_ptr->rlc_ul_data_available))
          {
            for (i=0;i< (local_rlc_buf.num_msgs);i++)
            {
              /* Action get mode's on transmitted RLC pointer */
              rlc_config_params.access_mode=GET_MODE;
              rlc_config_params.get_type = GET_WITH_PREVIOUS_READ;
              rlc_config_params.ack_state = local_rlc_buf.ul_desc[i].ack_state;
              rlc_config_params.blk_ptr = local_rlc_buf.ul_desc[i].blk_ptr;

              if (grlc_ul_fifo_access_block(gas_id, &rlc_config_params) != UL_FIFO_ACCESS_OK)
              {
                MSG_GERAN_ERROR_1_G("deffered RLC Get failed [%d]",gl1_get_FN( gas_id ));
              }
            }

            /* Clear out deferred buffer */
            GPL1_RESET_DEFER_BUFF( gas_id );

                  /* Clear out the local rlc buffer
                   * @@Warning: This is important since deferred blocks
                   * are stored momentarly in this buffer before getting
                   * copied to the main deffered buffer.
                   */
                  memset( &local_rlc_buf,
                          0,
                          sizeof(local_rlc_buf) );

          }

          gp1l_gprs_abort_ul_fifo_access(gas_id);
        }

      } /* End of if burst number == Burst 4 */

      first_valid_burst_found = FALSE;
      i = 0;

      if((num_msgs > 0) && (num_msgs <= MAX_GPRS_BURST_METRICS))
      {
         /*Find the position of the first valid burst*/
         for(j=0;((j<num_msgs) &&(first_valid_burst_found == FALSE));j++)
         {
            if((md[j]->valid) && (md[j]->schedule_status & GL1_DEFS_RX_SCHEDULED))
            {
               first_valid_burst_found = TRUE;
               i = j;
               MSG_GERAN_LOW_1_G("first_valid_burst_found at:%d",i);
            }
         }
      }
      else
      {
         /* num_msgs should always be a number between 1 and MAX_GPRS_BURST_METRICS */
         MSG_GERAN_ERROR_1_G("Invalid number of GPRS burst metrics: %d",num_msgs);
      }


      if(first_valid_burst_found == TRUE)
      {
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
        if (gl1_get_MSRD_ctl_nv(gas_id)) 
        {
           pwr_meas_type = PRX_DIVRX_MEAS;
           if(md[i]->pwr_dBm_x16_divrx != GL1_INVALID_POWER_LEVEL)
           {
              divrx_rssi = md[i]->pwr_dBm_x16_divrx;
           }
           else
           {
              /* DivRX pwr is invalid, use PRX power to keep running average counters */
              /* variables in sync. */
              divrx_rssi = md[i]->pwr_dBm_x16;
           }
         }
         else if (gl1_get_IMRD_ctl_nv(gas_id)) 
         {
            pwr_meas_type = PRX_DIVRX_MEAS;
            divrx_rssi = md[i]->pwr_dBm_x16;
         }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

        /*  Serving Cell Measurements */
        gpl1_scell_pkt_transfer_metrics_cb( md[i]->arfcn.num,
                                            burst_num,
                                            md[i]->pwr_dBm_x16, 
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
                                            divrx_rssi,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                            pwr_meas_type,
                                            gas_id );
      }
      else
      {
        MSG_GERAN_ERROR_0_G("No valid bursts found");
      }

      /* write back usf_detected to isr store */
      gpl1_gprs_update_usf_det(usf_detected_local, gas_id);
    } /* End of if abort callbacks is FALSE */
}

/*===========================================================================

FUNCTION  gpl1_gprs_config_agc

DESCRIPTION
  This function configure's and reconfigure's the parameters required for
  setting up the agc parameters for GPRS.

DEPENDENCIES
  pointer to dynamic allocation parameters


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_config_agc(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T    *transfer_data_ptr;
  dBx16_T p0;
  dBx16_T pb;
  boolean dl_pwr_ctrl_valid;
  boolean pbcch_valid;

  /* assign shortcut to general transfer_data_params */
  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  if (transfer_data_ptr->dl_power_control.params_valid)
  {
    p0 = l1_compute_P0(transfer_data_ptr->dl_power_control.po);
    dl_pwr_ctrl_valid= TRUE;
  } else
  {
    /* p0 not present, therefore downlink power control not available */
    p0 = 0;
    dl_pwr_ctrl_valid = FALSE;
  }
    /* No PBBCH, therefore pb parameter not present */
    pb = 0;
    pbcch_valid = FALSE;

  /* configure AGC with pb, pr values */
  sys_algo_gprs_agc_cfg(pb,p0,pbcch_valid,dl_pwr_ctrl_valid,gas_id);
  MSG_GERAN_LOW_3_G("cfg agc pb %d po %d [%d]",pb,p0,gl1_get_FN( gas_id ));
}

#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION gpl1_gprs_reconfig_progress

DESCRIPTION
   Check Reonfig in progress anmd disallow activity if TRUE

DEPENDENCIES

RETURN VALUE
  boolean release_allowed TRUE/FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_reconfig_progress( uint32 frames_required,gas_id_t gas_id )
{
  boolean gbta_not_allowed = FALSE;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  uint32 start_frame;
  uint32 tbf_suspend_end;

  transfer_data_T*  transfer_data_ptr =
          l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  if( GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id )
  {
    return TRUE;
  }


 if((GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id) || (transfer_data_ptr == NULL))
 {
    MSG_GERAN_ERROR_1_G("gpl1_gprs_reconfig_progress - Invalid gas id %d or tranfer data ptr NULL", gas_id);
    return TRUE;
 }

  start_frame = trans[gas_id].gbta_current_pch_suspension_FN;
  tbf_suspend_end = ADD_FN(start_frame,frames_required);

#ifdef FEATURE_GPRS_GBTA_DEBUG
  MSG_GERAN_MED_3_G("GL1 GBTA reconfig_progress start_frame %d suspend_end %d pch_fn %d",
                      start_frame,tbf_suspend_end,trans[gas_id].gbta_current_pch_suspension_FN);
#endif /* FEATURE_GPRS_GBTA_DEBUG */

  /* We could be in Two Phase Access , Waiting for Starttime to expire, but not
     fully transitioned to Transfer
  */
  if (trans[gas_id].gbta_current_pch_suspension_FN == L1_GPRS_INVALID_FN)
  {
    MSG_GERAN_HIGH_0_G("GL1 GBTA gbta_current_pch_suspension_FN invalid : GBTA not allowed");
    return TRUE;
  }

  if(l1_tskisr_blk->time_slot_change_pending)
  {
    MSG_GERAN_HIGH_0_G("GL1 GBTA lowest timeslot change in progress, do not allow GBTA for this frame");
    return TRUE;
  }

  /* Setup pointer to access the current transfer data parameters */

  /* Check pending uplink assignment */
  if(  ( transfer_data_ptr->ul_st != L1_GPRS_INVALID_FN )
     &&( transfer_data_ptr->ul_st >= start_frame )
     &&( transfer_data_ptr->ul_st <= tbf_suspend_end ) )
  {
      gbta_not_allowed = TRUE;

      MSG_GERAN_MED_3_G(" apprchin ul assign blks tbf susp FN=%d start_frame=%d tbf_suspend_end=%d",
                gl1_get_FN( gas_id ),start_frame,tbf_suspend_end );
  }

  /* Check pending downlink assignment */
  if(  ( transfer_data_ptr->dl_st != L1_GPRS_INVALID_FN )
     &&( transfer_data_ptr->dl_st >= start_frame )
     &&( transfer_data_ptr->dl_st <= tbf_suspend_end ) )
  {
      gbta_not_allowed = TRUE;

      MSG_GERAN_MED_3_G(" apprchin dl assign blks tbf susp FN=%d start_frame=%d tbf_suspend_end=%d",
                gl1_get_FN( gas_id ),start_frame,tbf_suspend_end );
  }

  return  gbta_not_allowed;
}
#endif /* FEATURE_GPRS_GBTA */

/*===========================================================================

FUNCTION gpl1_gprs_leaving_transfer

DESCRIPTION
  Prohibits GBTA when Transfer SUb is releasing TBF.

DEPENDENCIES

RETURN VALUE
  boolean release_allowed TRUE/FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_leaving_transfer( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
  transfer_data_T *transfer_data_ptr;

  if( GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id )
  {
    return TRUE;
  }

  l1_tskisr_blk= &l1_tsk_buffer[gas_id];
  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  if (transfer_data_ptr != NULL)
  {
    if ( (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
         (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL)  ||
         (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL) )
    {
      MSG_GERAN_MED_1_G("GBTA ptm sub release pending %d ",transfer_data_ptr->release_pending);
      return TRUE;
    }
  }

  return FALSE;
}

/*===========================================================================

FUNCTION gpl1_gprs_awaiting_release

DESCRIPTION
  Decides if L1 can leave transfer mode or not, depending upon what is being
  released. For example if UL_TBF_NORMAL release then make sure that the last
  RRBP message sent out was a PCA_UL_FAI_MSG.

DEPENDENCIES
  transfer_data_ptr->release_pending
  transfer_data_ptr->tbf_release_req_type
  transfer_data_ptr->last_rrbp_msg_type

RETURN VALUE
  boolean release_allowed TRUE/FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_awaiting_release( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean release_allowed = FALSE;
  transfer_data_T *transfer_data_ptr =
                              l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  switch ( transfer_data_ptr->release_pending )
  {
  case L1_PARTIAL_RELEASE_UL:
  case L1_FULL_RELEASE_UL:

    if ( !gpl1_gprs_get_ul_num_rrbp_msg(gas_id) )
    {
      release_allowed = TRUE;
    }
    break;

  case L1_FULL_RELEASE_DL:
  case L1_PARTIAL_RELEASE_DL:

    if ( !gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id) )
    {
      release_allowed = TRUE;
    }
    break;


  case L1_FULL_RELEASE_UL_DL:

    if ( !gpl1_gprs_get_ul_num_rrbp_msg(gas_id) && !gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id) )
    {
      release_allowed = TRUE;
    }
    break;

  default:
      ;
    /* Do nothing there is no release pending */
  }

  return release_allowed;

}

/*===========================================================================

FUNCTION gpl1_is_pdtch_snr_bad

DESCRIPTION
  This function checks the PDTCH SNR to evaluate if it is below the current
  defined SNR threshold. The boolean is returned to report the result to
  the calling function.

DEPENDENCIES

RETURN VALUE
  boolean TRUE= SNR is below threshold

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_is_pdtch_snr_bad( gas_id_t gas_id  )
{
  #define PDTCH_SNR_BAD_THRESHOLD 96 /* 6 dB * 16*/
  boolean pdtch_snr_is_bad = FALSE;

  if ( pdtch_snr[gas_id].valid && pdtch_snr[gas_id].accumulator_count)
  {
    sword average_snr = (pdtch_snr[gas_id].accumulator/pdtch_snr[gas_id].accumulator_count);

    if ( average_snr < PDTCH_SNR_BAD_THRESHOLD )
    {
      pdtch_snr_is_bad = TRUE;
      MSG_GERAN_ERROR_3_G("++PDTCH avgSNR=%d accumulator=%d count=%d"
        ,average_snr
        ,pdtch_snr[gas_id].accumulator
        ,pdtch_snr[gas_id].accumulator_count);
    }
    else
    {
      MSG_GERAN_LOW_3_G("++PDTCH avgSNR=%d accumulator=%d count=%d"
        ,average_snr
        ,pdtch_snr[gas_id].accumulator
        ,pdtch_snr[gas_id].accumulator_count);
    }

  }

  /* Clear out all values on a call to this function */
  gpl1_reset_pdtch_snr_variables(gas_id);

  return pdtch_snr_is_bad;
}

/*===========================================================================

FUNCTION gpl1_reset_pdtch_snr_variables

DESCRIPTION
  This function clears out the store that calculates the SNR average on PDTCH

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_reset_pdtch_snr_variables( gas_id_t gas_id  )
{

  /*reset the accumulation for the start of this 104 frame PTCCH multiframe */
  MSG_GERAN_MED_3_G("reset the SNR accumulation FN(mod104):%d cnt:%d acc:%d"
           ,frame_counters[gas_id].FNmod104
           ,pdtch_snr[gas_id].accumulator_count
           ,pdtch_snr[gas_id].accumulator);

  pdtch_snr[gas_id].accumulator_count = 0;
  pdtch_snr[gas_id].accumulator = 0;
  pdtch_snr[gas_id].valid = FALSE;
}

/*===========================================================================

FUNCTION l1_get_usf_utilisation_info

DESCRIPTION
  This function return the usf utilisation info

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_get_usf_utilisation_info(l1_usf_utilisation_info_t* usf_utilisation_info,
                                 gas_id_t gas_id)
{
  const gpl1_ps_store_T* gpl1_store_ptr = gpl1_read_xfer_isr_store(gas_id);

  *usf_utilisation_info = gpl1_store_ptr->usf_utilisation_stats;

  MSG_GERAN_HIGH_4_G("USF DEBUG, l1_get_usf_utilisation_info, FN: %d, total USFs: %d, total blanked blocks: %d, num_consecutive_blanked_blocks: %d",
      usf_utilisation_info->frame_num,
      usf_utilisation_info->total_usfs,
      usf_utilisation_info->total_blocks_blanked,
      usf_utilisation_info->num_consecutive_blanked_blocks);
}

 /*===========================================================================

FUNCTION l1_get_number_consecutive_blanked_blocks

DESCRIPTION
  This function return the number of consecutive blocks that were blanked
  by cxm

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint32 l1_get_number_consecutive_blanked_blocks(gas_id_t gas_id)
{
  const gpl1_ps_store_T* gpl1_store_ptr = gpl1_read_xfer_isr_store(gas_id);

  return gpl1_store_ptr->usf_utilisation_stats.num_consecutive_blanked_blocks;
}

/*===========================================================================

FUNCTION gpl1_get_egprs_ir_mem_status

DESCRIPTION
  This function returns the Incremental redundancy memory status.

  TRUE =  MEMORY FULL
  FALSE = MEMORY NOT FULL

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_get_egprs_ir_mem_status(gas_id_t gas_id)
{
  if(gl1_ir_mem_full[gas_id]!= FALSE)
  {
    MSG_GERAN_HIGH_1_G("GL1 EGPRS: IR Mem full %d",gl1_ir_mem_full[gas_id]);
  }
  return (gl1_ir_mem_full[gas_id]);
}
