/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            L1 Logging Facilities

GENERAL DESCRIPTION
   This module contains functions for logging L1 packets.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_log.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
17/04/17   nm      CR2032010 make rf power class variable dual data spaced
21/04/15   shr       CR806878 GPRS DSDS Power Control and WWCoex Power Info do not log the Tx Power in use
26/03/15   npt     CR813733 Change version of GSM EDGE Enhanced Log packet 
27/02/15   am      CR646584 WLAN coex
11/11/14   ws      CR702510 Add support for gprs message metrics E log packet
09/09/14   jk      CR697308:GL1 changes to support HLin LLin sawless feature
30/05/14   ws      CR657674 Data + MMS Phase 1
24/10/13   jj      CR 497390 add LOG_GPRS_RECEIVE_BURST_METRICS_E_C
05/11/13   ws      CR571077 - Fixed regression with CR482765 with calling 
                   gsmdiag_log_commit() instead of log_commit()
24/09/13   sp      CR482765 - New Log Packet - EDGE Enhanced Info.
24/09/13   sp      CR449651: Define new log packets for burst metrics
07/16/13   zc      CR470197 Replace memcpy with memscpy
21/06/13   cs      Major Triton TSTS Syncup
10/04/13   ap      CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
04/15/13   ss      CR475600 srb_hard_decisions log packet commit changes
31/05/12   ab      CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
09/09/11   ab      CR303214 Request for Configurable option for changing GSM Power Class value
02/07/10   dv      Lint cleanup
01/07/10   ws      Changes to support IQ logging for QDSP6 DSP
06/05/10   dv      CR237642 - Add ASRB funtionality
04/03/10   dv      Merge Gobi Source to 2H09 modem pkg
28/01/10   da      Fix use of incorrect NSAPI values in log packet 0x5231
09/09/09   cs      Fix for Klocwork bounds checking on array indices
03/04/09   ap      CR177998 Incorrect log packet Status Bit setting in
                   GPRS Receive Message Metrics
30/03/09   cs      Remove unnecessary extern gprs_serv_cell_meas for Lint
06/12/08   cs      Include gl1_hw_debug.h for new split gl1_hw.h
29/04/08   cs      Add the TSC and channel to the IQ log data
07/04/08   cs      Fix for eSAIC variable size and correct logging of IQ
14/01/08   cs      Allow for separate IQ logging and saving
17/09/07   agv     Added I/Q logging to code base.
12/02/07   og      Addition of pointer address safety checks for
                   gprs_air_interface_summary and gprs_transfer_summary log packets.
01/02/07   og      gprs transfer log summary.
24/07/06   zyu     Added new log packet LOG_EGPRS_SRB_HARD_DECISIONS_C
17/01/06   og      Merge change 280538 from the raven 3.0 branch. Resolves
                   CR 86191 / SR 791121.
22/11/05   og      Changes to logging interfaces.
21/11/05   og      Addition of missing log packet type casts.
04/08/05   og      Merge change 225338 from the raven branch.
23/06/05   og      Lint error/warnings removal.
23/06/05   og      Code changes to accomodate the uplink tbf data block count
                   log packet.
02/06/05   og      Removal of header-header includes.
28/10/04   ws        Lint tidy up
28/10/04   ws      Added EGPRS logging support to burst and messgae_metrics
15/10/04   og      Removed Fixed TBF allocation related source code.
17/08/04   ws      Cleaned up EGPRS code and linted
28/04/04   rm      Adding TA interface and cleaned up some of Lint errors
23/04/04   ws      Corrected Lint Error's and Warninggs
01/30/04   gfr     Support for the firecode status bit
01/12/03   ws      Lint Cleanup
10/08/03   gfr     Support for quad-band.
17/09/03   ws      Updates for air interface summary
08/09/03   ws      Added ptcch/d logging packets
04/09/03   ws      Updates for gpl1_log_gprs_air_interface_summary packet
07/07/03   ws      Added message metrics v2 log packet
01/05/03   ws      Added missing customer.h for building with GPRS
30/04/03   dl      Fixes for building GPRS related s/w without GPRS features enabled
26/02/03   npr     Changed instances of INVALID to GPL1_INVALID
07/02/03   ws      Renamed gpl1_log.h to gpl1_gprs_log.h in include path
04/02/03   ws      downgraded MSG_HIGH's to MSG_LOW and removed l1_log revision history
03/02/03   rm      Initial Version

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "gpl1_gprs_log.h"   // This module's header file
#include "log.h"            // For log_commit() and log_alloc() and log codes
#include "diag.h"           // Needed for DIAG_DIAGVER prior to calling event.h
#include "event.h"          // For logging RAR event and event codes
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"

#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"  // For GSM log codes

#include "gllc.h"
#include "gllc_v.h"
#include "gllc_v_g.h"
#include "gsndcp.h"
#include "gsni.h"
#include "l1_task.h"
#include "l1_utils.h"

#include "grlclog.h"
#include "gpl1_gprs_ptcch.h"

#include "l1_isr.h"
#include "gpl1_gprs_serving_cell.h"

#include "gl1_msg_pdch.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_task.h"
#include <stringl/stringl.h>

#ifdef FEATURE_GOBI
/* Defined in rr_sys_info_main.c */
extern ARFCN_T rr_gprs_get_scell_arfcn(void);
/* Defined in cmss.c */
extern void cmss_update_active_gprs_pdtch(const ARFCN_T *);
#endif /* FEATURE_GOBI */

/* RF power class per band */
extern byte   gl1_rf_power_class_per_band[NUM_GERAN_DATA_SPACES][5];
extern uint8  gl1_max_pcl_pwr_class[5][5][2];

/*===========================================================================
                        TYPE DEFINITIONS FOR EVENTS
===========================================================================*/

/* -----------GPRS RECEIVE METRICS---------------- */

/*lint -save -e754 */

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_A_C)
    byte                    chan;
    /* BURST METRICS  */
    gpl1_log_gprs_burst_metrics_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_B_C)
    byte                    chan;
    /* BURST METRICS  */
    gpl1_log_gprs_burst_metrics_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_C_C)
    byte                    chan;
    /* BURST METRICS */
    gpl1_log_gprs_burst_metrics_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_D_C)
    byte                    chan;
    /* BURST METRICS  */
    gpl1_log_gprs_burst_metrics_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_E_C)
    byte                    chan;
    /* BURST METRICS  */
    gpl1_log_gprs_burst_metrics_T  burst;
LOG_RECORD_END


#define LOG_GPRS_RECEIVE_BURST_METRICS_C_VER 2
LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_A_VER2_C)
    byte                    version;
    byte                    chan;
    /* BURST METRICS  */
    gpl1_log_gprs_burst_metrics_ver2_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_B_VER2_C)
    byte                    version;
	byte                    chan;
    /* BURST METRICS  */
    gpl1_log_gprs_burst_metrics_ver2_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_C_VER2_C)
    byte                    version;
	byte                    chan;
    /* BURST METRICS */
    gpl1_log_gprs_burst_metrics_ver2_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_BURST_METRICS_D_VER2_C)
    byte                    version;
	byte                    chan;
    /* BURST METRICS  */
    gpl1_log_gprs_burst_metrics_ver2_T  burst;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_B_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_C_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_D_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
    byte                    usf;
    byte                    ir_status;
    byte                    mod_detected;
    uint16                  blk_mean_bep_tslot;
    uint16                  blk_cv_bep_tslot;
    uint16                  filtered_mean_bep_tslot;
    uint16                  filtered_cv_bep_tslot;
    uint16                  reliability;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
    byte                    usf;
    byte                    ir_status;
    byte                    mod_detected;
    uint16                  blk_mean_bep_tslot;
    uint16                  blk_cv_bep_tslot;
    uint16                  filtered_mean_bep_tslot;
    uint16                  filtered_cv_bep_tslot;
    uint16                  reliability;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
    byte                    usf;
    byte                    ir_status;
    byte                    mod_detected;
    uint16                  blk_mean_bep_tslot;
    uint16                  blk_cv_bep_tslot;
    uint16                  filtered_mean_bep_tslot;
    uint16                  filtered_cv_bep_tslot;
    uint16                  reliability;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
    byte                    usf;
    byte                    ir_status;
    byte                    mod_detected;
    uint16                  blk_mean_bep_tslot;
    uint16                  blk_cv_bep_tslot;
    uint16                  filtered_mean_bep_tslot;
    uint16                  filtered_cv_bep_tslot;
    uint16                  reliability;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_RECEIVE_MSG_METRICS_E_VER2_C)
    uint32                  FN;
    byte                    TN;
    byte                    chan;
    /* MESSAGE METRICS */
    byte                    coding_scheme;
    uint16                  rx_qual;
    byte                    status;    /* TAF:bit2/BFI:bit1/GOOD_DATA:bit0 */
    uint16                  msg_len;
    byte                    usf;
    byte                    ir_status;
    byte                    mod_detected;
    uint16                  blk_mean_bep_tslot;
    uint16                  blk_cv_bep_tslot;
    uint16                  filtered_mean_bep_tslot;
    uint16                  filtered_cv_bep_tslot;
    uint16                  reliability;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_AIR_INTERFACE_SUMMARY_C)
    uint32                  FN;
    byte                    band_ind;
    byte                    dl_timeslots;
    byte                    ul_timeslots;
    dBx16_T                 rx_power;
    byte                    tx_power_timeslot[4];
    byte                    timing_advance;
    uint32                  rlc_dl_rx;
    uint32                  rlc_ul_tx;
    uint32                  rlc_ul_retx;
    uint32                  llpdu_tx_sum;
    uint32                  llpdu_retx;
    uint32                  llpdu_rx_sum;
    uint32                  llpdu_rx_err;
    uint32                  user_oct_dl;
    uint32                  user_oct_ul;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_AGC_C)
    uint16                  ARFCN;
    uint32                  FN;
    byte                    channel;
    dBx16_T                 rx_p_ref_bcch;
    dBx16_T                 pr_a;
    dBx16_T                 pr_b;
    dBx16_T                 pr_c;
    dBx16_T                 pr_d;
    dBx16_T                 p0;
    dBx16_T                 pb;
    byte                    tfi_usf;
    boolean                 srl;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_TIMING_ADVANCE_C)
    byte                   ta_index;
    byte                   ta_timeslot;
    byte                   ta_assigned_value;
    byte                   ta_continous_value;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_POWER_CONTROL_C)
    byte                   tx_channel;
    byte                   tx_tn;
    dBx16_T                tx_pwr;
    byte                   coding_scheme;
    uint8                  gamma_tn;
    dBx16_T                gamma_band;   //Frequency band related PC parameter
    byte                   alpha;       //System parameter. The 8960 has the alpha parameter hard wired to 0.
    byte                   derivedC;    //Normalised Rx Signal at MS
    byte                   pmax;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GPRS_TRANSFER_SUMMARY_C)
    byte                   band_ind;
    byte                   current_alloc;
    byte                   ul_ptcch_timeslot;
    byte                   dl_ptcch_timeslot;
    byte                   timing_advance;
    byte                   usf_granularity;
    byte                   ul_bitmap_tn;
    byte                   dl_bitmap_tn;
LOG_RECORD_END

/* Uplink tbf data/dummy block statistics */
LOG_RECORD_DEFINE(LOG_UPLINK_TBF_DATA_BLOCK_COUNT_C)
    uint32  number_of_rlc_data_blocks;
    uint32  number_of_dummy_ctrl_blocks;
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_EGPRS_SRB_HARD_DECISIONS_C)                 /* 0x522E */
    uint32  frame_number;
    uint16  mcs;
    uint16  hd_348bits[22]; /*348 bits HD form 22 words (16bits)*/
LOG_RECORD_END

#define LOG_EGPRS_MESSAGE_METRICS_C_VER 3  
LOG_RECORD_DEFINE(LOG_GPRS_EDGE_ENHANCED_C)
  byte                    version;
  byte                    chan;
  uint32                  FN;
  byte                    TN;
  GfwEnhancedEdgeLogPacket edge_log_packet;
LOG_RECORD_END  

/* This should be for both GSM,  and GPRS  (EGPRS) */
/* Define here for similarity to bit stream logging */
#ifdef FEATURE_GSM_LOG_IQ_SAMPLES

/*
 * These mDSP sizes are arranged as size of data + data itself
 * eg 3 1 2 3 is data of length 3 with 1 2 3
 * Clearly any changes to this data format needs reflecting here
 * the best mechanism os for the mDSP to export some values so we
 * always remain in-sync
 */
#define IQ_CIR_SIZE         ( 1 + 14 )

/* NB: The biggest it can be for F/W that has eSAIC and reports wrong size */
#define IQ_SAIC_SIZE        ( 1 + ( 4 * ( 1 + 8 ) ) )

#define IQ_DATA_SIZE        ( 1 + 648 )
#define IQ_CORR_ENERGY_SIZE ( 1 + 14 )

LOG_RECORD_DEFINE(LOG_MDSP_IQ_SAMPLES_C)     /* 0x522F */
    uint32 FN;
    uint16 TSC;
    uint16 channel;
    int16  CIR[IQ_CIR_SIZE];        /* CIR                  */
    int16  SAIC[IQ_SAIC_SIZE];      /* SAIC                 */
    int16  IQ[IQ_DATA_SIZE];        /* IQ info              */
    int16  CE[IQ_CORR_ENERGY_SIZE]; /* Correlation Energies */
LOG_RECORD_END
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */
/* lint -restore */

/*===========================================================================
                         DATA DECLARATIONS
===========================================================================*/

/* Used for LOG_ON_DEMAND registry */
static boolean gpl1_log_initialized   = FALSE;

static dBx16_T rx_pwr;

/*===========================================================================
                         FUNCTION DECLARATIONS
===========================================================================*/

#ifdef FEATURE_GSM_LOG_IQ_SAMPLES
static uint16 gpl1_collect_iq_log_data( uint16 *out_data_p, uint16 *in_data_p,
                                        uint16 max_data_length );



#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */

/*===========================================================================

FUNCTION  gpl1_log_init

DESCRIPTION
  This function initializes the log_on_demand packets.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_log_init()
{
    if (gpl1_log_initialized)
    {
        return;
    }
    else
    {   /* Call this only once */
        gpl1_log_initialized = TRUE;
    }
}


/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_a

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void gpl1_log_gprs_burst_metrics_a ( gl1_msg_dl_chn_type channel,uint32 fn,
                                     const gl1_defs_rx_pkt_metrics_type *rpt_ptr, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_A_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    rx_pwr = rpt_ptr->pwr_dBm_x16;

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16,gas_id );
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_A_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_A_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_A_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {
      // Fill packet and commit it
      log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16)rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;
      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_a...");
    }//endif log_ptr
#ifdef FEATURE_GOBI
    if (channel == GL1_MSG_DL_PACCH_PDTCH && rpt_ptr->arfcn.num != rr_gprs_get_scell_arfcn().num)
    {
      cmss_update_active_gprs_pdtch(&(rpt_ptr->arfcn));
    }
#endif /* FEATURE_GOBI */
}

/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_a_ver2

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
  
RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void gpl1_log_gprs_burst_metrics_a_ver2 ( gl1_msg_dl_chn_type channel,uint32 fn,
                                          const gl1_defs_rx_pkt_metrics_type *rpt_ptr,
                                          gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_A_VER2_C_type  *log_ptr;    // pointer to log buffer 
    gsmdiag_pkt_info_t pkt_info;
       
    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    rx_pwr = rpt_ptr->pwr_dBm_x16;

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16,gas_id );
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_A_VER2_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_A_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_A_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);


    if ( log_ptr )
    { 
      // Fill packet and commit it
	  log_ptr->version               = LOG_GPRS_RECEIVE_BURST_METRICS_C_VER;
      log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16)rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)(rpt_ptr->brstCancel << 4) | (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;
      log_ptr->burst.reliabFactorQ16          = (uint32)(rpt_ptr->reliabFactorQ16);
      log_ptr->burst.jdet_reading          = rpt_ptr->jdet_value;
      log_ptr->burst.WBEE                  = 0;
      log_ptr->burst.HLinLLinState         = rpt_ptr->SawlessHLLLState;


      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_a...");
    }//endif log_ptr
#ifdef FEATURE_GOBI
    if (channel == GL1_MSG_DL_PACCH_PDTCH && rpt_ptr->arfcn.num != rr_gprs_get_scell_arfcn().num)
    {
      cmss_update_active_gprs_pdtch(&(rpt_ptr->arfcn));
    }
#endif /* FEATURE_GOBI */
}  


/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_a_ver2

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_a_ver2  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                              const gl1_defs_rx_pkt_data_type *rpt_ptr,
                                              const usf_dl_tfi_info_t         *usf_dl_tfi_info
                                               , gas_id_t gas_id)
{
    LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    uint16 blk_mean_bep_tslot = 0;
    uint16 blk_cv_bep_tslot = 0;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);
      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;

      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS7_CODING)
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len + rpt_ptr->msg[1].msg_len;
      }else
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len;
      }
      log_ptr->status        =  0;
      log_ptr->status       |= 0x3; /* bit 0 and 1 not used */

      /* Bit 2. Used as GPRS CRC for CS 1 - 4 */
      if ((rpt_ptr->msg[0].crc_passed) && (rpt_ptr->coding_scheme <= GL1_DEFS_CS4_CODING))
      {
        log_ptr->status     |=  0x4;
      }

      /* Bit 5 N/A */
      /* Bit 6 used for USF matched */
      if (usf_dl_tfi_info->usf_match)
      {
       log_ptr->status |= 0x40;
      }
      /* Bit 7 used for DL TFI matched */
      if (usf_dl_tfi_info->dl_tfi_match)
      {
        log_ptr->status |= 0x80;
      }


      /* bit 2. used as EGPRS header CRC */
      if ((uint8)rpt_ptr->egprs.hdr_crc_passed)
      {
        log_ptr->status     |=  0x4;
      }

       /* Bit 3 Used for EGPRS message 1 CRC */
      if ((rpt_ptr->msg[0].crc_passed) &&  (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING) )
      {
        log_ptr->status     |=  0x8;
      }

      /* Bit 4 Used when a dual payload EGPRS message is present */
      if ((uint8)rpt_ptr->msg[1].crc_passed)
      {
        log_ptr->status     |=  0x10;
      }

      log_ptr->ir_status = 0;

      /* bit 0 indicated IR used for first payload */
      if (rpt_ptr->egprs.IR_used[0])
      {
        log_ptr->ir_status |= 0x1;
      }

      /* Bit 1 used when IR used for second payload */
      if (rpt_ptr->egprs.IR_used[1])
      {
        log_ptr->ir_status |= 0x2;
      }


      /* Calculate mean bep and cv bep for MCS decodes. Log filtered ones*/
      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING)
      {
        blk_cv_bep_tslot = egpl1_calc_block_statistical_params((uint16*)rpt_ptr->egprs.bep,
                                                               &blk_mean_bep_tslot, gas_id);

        log_ptr->blk_cv_bep_tslot = blk_cv_bep_tslot;
        log_ptr->blk_mean_bep_tslot = blk_mean_bep_tslot;

        log_ptr->filtered_mean_bep_tslot = usf_dl_tfi_info->mean_bep_tn;
        log_ptr->filtered_cv_bep_tslot = usf_dl_tfi_info->cv_bep_tn;
        log_ptr->reliability = usf_dl_tfi_info->r;

      }else
      {
        log_ptr->blk_cv_bep_tslot = 0;
        log_ptr->blk_mean_bep_tslot = 0;
      }

      /* not currently setup, (may become redundant) */
      log_ptr->mod_detected = 0;



      /* USF now extracted from 1st 3 bits of EGPRS header or GPRS payload
       * since MCS 7 - 9 data callback later than 4th metrics
       */
      log_ptr->usf  = (uint8)usf_dl_tfi_info->usf;
      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_a_ver2");
    }//endif log_ptr
}
/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_b

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_burst_metrics_b ( gl1_msg_dl_chn_type channel,uint32 fn,
                                     const gl1_defs_rx_pkt_metrics_type *rpt_ptr, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_B_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16 ,gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_B_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_B_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_B_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16) rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_b...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_b

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_burst_metrics_b_ver2 ( gl1_msg_dl_chn_type channel,uint32 fn,
                                          const gl1_defs_rx_pkt_metrics_type *rpt_ptr,
                                          gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_B_VER2_C_type  *log_ptr;    // pointer to log buffer 
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16,gas_id );
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_B_VER2_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_B_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_B_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->version               = LOG_GPRS_RECEIVE_BURST_METRICS_C_VER;
	  log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16) rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)(rpt_ptr->brstCancel << 4) | (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;
      log_ptr->burst.reliabFactorQ16          = (uint32)(rpt_ptr->reliabFactorQ16);
      log_ptr->burst.jdet_reading             = rpt_ptr->jdet_value;
      log_ptr->burst.WBEE                     = 0;
      log_ptr->burst.HLinLLinState            = rpt_ptr->SawlessHLLLState;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_c...");
    }//endif log_ptr    
}
      
/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_b

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_b  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                         const gl1_defs_rx_pkt_data_type *rpt_ptr, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_MSG_METRICS_B_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_B_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_B_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_B_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )

    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);
      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;
      log_ptr->msg_len       =  rpt_ptr->msg[0].msg_len;
      log_ptr->status        =  0;
      log_ptr->status       |= 0x1; /* bit 0 not used */

      if ((uint8)rpt_ptr->msg[0].crc_passed)
      {
        log_ptr->status     |=  0x4;
      }
      if (!(uint8)rpt_ptr->msg[0].crc_passed)
      {
        log_ptr->status     |=  0x2;
      }

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_b...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_b_ver2

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_b_ver2  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                              const gl1_defs_rx_pkt_data_type *rpt_ptr,
                                              const usf_dl_tfi_info_t         *usf_dl_tfi_info,
                                              gas_id_t                         gas_id)
{
    LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    uint16 blk_mean_bep_tslot = 0;
    uint16 blk_cv_bep_tslot = 0;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);

      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;

      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS7_CODING)
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len + rpt_ptr->msg[1].msg_len;
      }else
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len;
      }
      log_ptr->status        =  0;
      log_ptr->status       |= 0x3; /* bit 0 and 1 not used */

      /* Bit 2. Used as GPRS CRC for CS 1 - 4 */
      if ((rpt_ptr->msg[0].crc_passed) && (rpt_ptr->coding_scheme <= GL1_DEFS_CS4_CODING))
      {
        log_ptr->status     |=  0x4;
      }

      /* Bit 5 N/A */
      /* Bit 6 used for USF matched */
      if (usf_dl_tfi_info->usf_match)
      {
       log_ptr->status |= 0x40;
      }
      /* Bit 7 used for DL TFI matched */
      if (usf_dl_tfi_info->dl_tfi_match)
      {
        log_ptr->status |= 0x80;
      }


      /* bit 2. used as EGPRS header CRC */
      if ((uint8)rpt_ptr->egprs.hdr_crc_passed)
      {
        log_ptr->status     |=  0x4;
      }

       /* Bit 3 Used for EGPRS message 1 CRC */
      if ((rpt_ptr->msg[0].crc_passed) &&  (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING) )
      {
        log_ptr->status     |=  0x8;
      }

      /* Bit 4 Used when a dual payload EGPRS message is present */
      if ((uint8)rpt_ptr->msg[1].crc_passed)
      {
        log_ptr->status     |=  0x10;
      }

      log_ptr->ir_status = 0;

      /* bit 0 indicated IR used for first payload */
      if (rpt_ptr->egprs.IR_used[0])
      {
        log_ptr->ir_status |= 0x1;
      }

      /* Bit 1 used when IR used for second payload */
      if (rpt_ptr->egprs.IR_used[1])
      {
        log_ptr->ir_status |= 0x2;
      }


      /* Calculate mean bep and cv bep for MCS decodes */
      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING)
      {
        blk_cv_bep_tslot = egpl1_calc_block_statistical_params((uint16*)rpt_ptr->egprs.bep,
                                                               &blk_mean_bep_tslot, gas_id);

        log_ptr->blk_cv_bep_tslot = blk_cv_bep_tslot;
        log_ptr->blk_mean_bep_tslot = blk_mean_bep_tslot;

        log_ptr->filtered_mean_bep_tslot = usf_dl_tfi_info->mean_bep_tn;
        log_ptr->filtered_cv_bep_tslot = usf_dl_tfi_info->cv_bep_tn;
        log_ptr->reliability = usf_dl_tfi_info->r;


      }else
      {
        log_ptr->blk_cv_bep_tslot = 0;
        log_ptr->blk_mean_bep_tslot = 0;
      }

      /* not currently setup, (may become redundant) */
      log_ptr->mod_detected = 0;



      /* USF now extracted from 1st 3 bits of EGPRS header or GPRS payload
       * since MCS 7 - 9 data callback later than 4th metrics
       */
      log_ptr->usf  = (uint8)usf_dl_tfi_info->usf;
      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_b_ver2...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_c

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_burst_metrics_c ( gl1_msg_dl_chn_type channel,uint32 fn,
                                     const gl1_defs_rx_pkt_metrics_type *rpt_ptr, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_C_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16,gas_id );
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_C_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_C_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_C_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {
      // Fill packet and commit it
      log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16)rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_c...");
    }//endif log_ptr
}
/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_c

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_burst_metrics_c_ver2 ( gl1_msg_dl_chn_type channel,uint32 fn,
                                          const gl1_defs_rx_pkt_metrics_type *rpt_ptr,
                                          gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_C_VER2_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;
    
    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16,gas_id );
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_C_VER2_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_C_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_C_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->version               = LOG_GPRS_RECEIVE_BURST_METRICS_C_VER;
	  log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16) rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)(rpt_ptr->brstCancel << 4) | (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;
      log_ptr->burst.reliabFactorQ16          = (uint32)(rpt_ptr->reliabFactorQ16);
      log_ptr->burst.jdet_reading             = rpt_ptr->jdet_value;
      log_ptr->burst.WBEE                     = 0;
      log_ptr->burst.HLinLLinState            = rpt_ptr->SawlessHLLLState;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_c...");
    }//endif log_ptr    
}    

/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_c

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_c  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                         const gl1_defs_rx_pkt_data_type *rpt_ptr, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_MSG_METRICS_C_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_C_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_C_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_C_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);
      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;
      log_ptr->msg_len       =  rpt_ptr->msg[0].msg_len;
      log_ptr->status        =  0;
      log_ptr->status       |= 0x1; /* bit 0 not used */

      if ((uint8)rpt_ptr->msg[0].crc_passed)
      {
        log_ptr->status     |=  0x4;
      }
      if (!(uint8)rpt_ptr->msg[0].crc_passed)
      {
        log_ptr->status     |=  0x2;
      }

      gsmdiag_log_commit(&pkt_info);

    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_c...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_c_ver2

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_c_ver2  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                              const gl1_defs_rx_pkt_data_type *rpt_ptr,
                                              const usf_dl_tfi_info_t         *usf_dl_tfi_info, gas_id_t gas_id)
{
    LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    uint16 blk_mean_bep_tslot = 0;
    uint16 blk_cv_bep_tslot = 0;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);

      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;

      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS7_CODING)
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len + rpt_ptr->msg[1].msg_len;
      }else
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len;
      }
      log_ptr->status        =  0;
      log_ptr->status       |= 0x3; /* bit 0 and 1 not used */

      /* Bit 2. Used as GPRS CRC for CS 1 - 4 */
      if ((rpt_ptr->msg[0].crc_passed) && (rpt_ptr->coding_scheme <= GL1_DEFS_CS4_CODING))
      {
        log_ptr->status     |=  0x4;
      }

      /* Bit 5 N/A */
      /* Bit 6 used for USF matched */
      if (usf_dl_tfi_info->usf_match)
      {
       log_ptr->status |= 0x40;
      }
      /* Bit 7 used for DL TFI matched */
      if (usf_dl_tfi_info->dl_tfi_match)
      {
        log_ptr->status |= 0x80;
      }


      /* bit 2. used as EGPRS header CRC */
      if ((uint8)rpt_ptr->egprs.hdr_crc_passed)
      {
        log_ptr->status     |=  0x4;
      }

       /* Bit 3 Used for EGPRS message 1 CRC */
      if ((rpt_ptr->msg[0].crc_passed) &&  (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING) )
      {
        log_ptr->status     |=  0x8;
      }

      /* Bit 4 Used when a dual payload EGPRS message is present */
      if ((uint8)rpt_ptr->msg[1].crc_passed)
      {
        log_ptr->status     |=  0x10;
      }

      log_ptr->ir_status = 0;

      /* bit 0 indicated IR used for first payload */
      if (rpt_ptr->egprs.IR_used[0])
      {
        log_ptr->ir_status |= 0x1;
      }

      /* Bit 1 used when IR used for second payload */
      if (rpt_ptr->egprs.IR_used[1])
      {
        log_ptr->ir_status |= 0x2;
      }


      /* Calculate mean bep and cv bep for MCS decodes */
      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING)
      {
        blk_cv_bep_tslot = egpl1_calc_block_statistical_params((uint16*)rpt_ptr->egprs.bep,
                                                               &blk_mean_bep_tslot, gas_id);

        log_ptr->blk_cv_bep_tslot = blk_cv_bep_tslot;
        log_ptr->blk_mean_bep_tslot = blk_mean_bep_tslot;

        log_ptr->filtered_mean_bep_tslot = usf_dl_tfi_info->mean_bep_tn;
        log_ptr->filtered_cv_bep_tslot = usf_dl_tfi_info->cv_bep_tn;
        log_ptr->reliability = usf_dl_tfi_info->r;

      }else
      {
        log_ptr->blk_cv_bep_tslot = 0;
        log_ptr->blk_mean_bep_tslot = 0;
      }

      /* not currently setup, (may become redundant) */
      log_ptr->mod_detected = 0;



      /* USF now extracted from 1st 3 bits of EGPRS header or GPRS payload
       * since MCS 7 - 9 data callback later than 4th metrics
       */
      log_ptr->usf  = (uint8)usf_dl_tfi_info->usf;
      gsmdiag_log_commit(&pkt_info);

    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_c_ver2...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_burst_metrics_d

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_burst_metrics_d ( gl1_msg_dl_chn_type channel,uint32 fn,
                                     const gl1_defs_rx_pkt_metrics_type *rpt_ptr, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_D_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16 ,gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_D_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_D_C_type),gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_D_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {   // Fill packet and commit it
        log_ptr->chan                  = (byte)channel;
        log_ptr->burst.FN              = fn;
        log_ptr->burst.TN              = (byte)rpt_ptr->tn;
        log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
        log_ptr->burst.rssi            = rpt_ptr->rssi;
        log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
        log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
        log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
        log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
        log_ptr->burst.timing_offset   = (uint16)rpt_ptr->timing_offset;
        log_ptr->burst.usf             = (byte)rpt_ptr->usf;
        log_ptr->burst.snr             = rpt_ptr->snr;
        log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
        log_ptr->burst.mod_detected    = (byte)rpt_ptr->modulation;
        log_ptr->burst.filter_used      = (byte)rpt_ptr->aci_filter_used;
        log_ptr->burst.aci_pwr_n200_khz = (uint16)rpt_ptr->aci_pwr_low;
        log_ptr->burst.aci_pwr_0_hz     = (uint16)rpt_ptr->aci_pwr_center;
        log_ptr->burst.aci_pwr_p200_khz = (uint16)rpt_ptr->aci_pwr_high;

        gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_d...");
    }//endif log_ptr
}
/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_e

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void gpl1_log_gprs_burst_metrics_e ( gl1_msg_dl_chn_type channel,uint32 fn,
                                     const gl1_defs_rx_pkt_metrics_type *rpt_ptr, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_E_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    rx_pwr = rpt_ptr->pwr_dBm_x16;

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16,gas_id );
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_E_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_E_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_E_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {
      // Fill packet and commit it
      log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16)rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;
      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_a...");
    }//endif log_ptr
#ifdef FEATURE_GOBI
    if (channel == GL1_MSG_DL_PACCH_PDTCH && rpt_ptr->arfcn.num != rr_gprs_get_scell_arfcn().num)
    {
      cmss_update_active_gprs_pdtch(&(rpt_ptr->arfcn));
    }
#endif /* FEATURE_GOBI */
}
/*===========================================================================
FUNCTION  gpl1_log_gprs_burst_metrics_b

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_burst_metrics_d_ver2 ( gl1_msg_dl_chn_type channel,uint32 fn,
                                          const gl1_defs_rx_pkt_metrics_type *rpt_ptr,
                                          gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_BURST_METRICS_D_VER2_C_type  *log_ptr;    // pointer to log buffer 
    gsmdiag_pkt_info_t pkt_info;
    
    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    gpl1_update_rx_pwr_avg(rpt_ptr->pwr_dBm_x16,gas_id );
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
    gpl1_update_snr_avg(rpt_ptr->snr,gas_id);
#endif
    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_BURST_METRICS_D_VER2_C, sizeof(LOG_GPRS_RECEIVE_BURST_METRICS_D_VER2_C_type),gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_BURST_METRICS_D_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->version               = LOG_GPRS_RECEIVE_BURST_METRICS_C_VER;
	  log_ptr->chan                  = (byte)channel;
      log_ptr->burst.FN              = fn;
      log_ptr->burst.TN              = (byte)rpt_ptr->tn;
      log_ptr->burst.arfcn           = arfcn_to_word(rpt_ptr->arfcn);
      log_ptr->burst.rssi            = rpt_ptr->rssi;
      log_ptr->burst.pwr_dBm_x16     = rpt_ptr->pwr_dBm_x16;
      log_ptr->burst.dc_offset_i     = (uint16)rpt_ptr->dc_offset_i;
      log_ptr->burst.dc_offset_q     = (uint16)rpt_ptr->dc_offset_q;
      log_ptr->burst.freq_offset     = (uint16)rpt_ptr->freq_offset;
      log_ptr->burst.timing_offset   = (uint16) rpt_ptr->timing_offset;
      log_ptr->burst.usf             = (byte)rpt_ptr->usf;
      log_ptr->burst.snr             = rpt_ptr->snr;
      log_ptr->burst.gain_state      = (byte)rpt_ptr->gain_range;
      log_ptr->burst.mod_detected          = (byte)(rpt_ptr->brstCancel << 4) | (byte)rpt_ptr->modulation;
      log_ptr->burst.filter_used           = (byte)rpt_ptr->aci_filter_used;
      log_ptr->burst.aci_pwr_n200_khz      = (uint16)rpt_ptr->aci_pwr_low;
      log_ptr->burst.aci_pwr_0_hz          = (uint16)rpt_ptr->aci_pwr_center;
      log_ptr->burst.aci_pwr_p200_khz      = (uint16)rpt_ptr->aci_pwr_high;
      log_ptr->burst.reliabFactorQ16          = (uint32)(rpt_ptr->reliabFactorQ16);
      log_ptr->burst.jdet_reading             = rpt_ptr->jdet_value;
      log_ptr->burst.WBEE                     = 0;
      log_ptr->burst.HLinLLinState            = rpt_ptr->SawlessHLLLState;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_burst_metrics_d...");
    }//endif log_ptr    
}      
/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_d

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_d  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                         const gl1_defs_rx_pkt_data_type    *rpt_ptr , gas_id_t gas_id)
{
    LOG_GPRS_RECEIVE_MSG_METRICS_D_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_D_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_D_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_D_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);
      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;
      log_ptr->msg_len       =  rpt_ptr->msg[0].msg_len;
      log_ptr->status        =  0;
      log_ptr->status       |= 0x1; /* bit 0 not used */

      if ((uint8)rpt_ptr->msg[0].crc_passed)
      {
        log_ptr->status     |=  0x4;
      }
      if (!(uint8)rpt_ptr->msg[0].crc_passed)
      {
        log_ptr->status     |=  0x2;
      }

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_d...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_d_ver2

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_d_ver2  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                              const gl1_defs_rx_pkt_data_type *rpt_ptr,
                                              const usf_dl_tfi_info_t         *usf_dl_tfi_info, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    uint16 blk_mean_bep_tslot = 0;
    uint16 blk_cv_bep_tslot = 0;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);

      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;

      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS7_CODING)
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len + rpt_ptr->msg[1].msg_len;
      }else
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len;
      }
      log_ptr->status        =  0;
      log_ptr->status       |= 0x3; /* bit 0 and 1 not used */

      /* Bit 2. Used as GPRS CRC for CS 1 - 4 */
      if ((rpt_ptr->msg[0].crc_passed) && (rpt_ptr->coding_scheme <= GL1_DEFS_CS4_CODING))
      {
        log_ptr->status     |=  0x4;
      }

      /* Bit 5 N/A */
      /* Bit 6 used for USF matched */
      if (usf_dl_tfi_info->usf_match)
      {
       log_ptr->status |= 0x40;
      }
      /* Bit 7 used for DL TFI matched */
      if (usf_dl_tfi_info->dl_tfi_match)
      {
        log_ptr->status |= 0x80;
      }


      /* bit 2. used as EGPRS header CRC */
      if ((uint8)rpt_ptr->egprs.hdr_crc_passed)
      {
        log_ptr->status     |=  0x4;
      }

       /* Bit 3 Used for EGPRS message 1 CRC */
      if ((rpt_ptr->msg[0].crc_passed) &&  (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING) )
      {
        log_ptr->status     |=  0x8;
      }

      /* Bit 4 Used when a dual payload EGPRS message is present */
      if ((uint8)rpt_ptr->msg[1].crc_passed)
      {
        log_ptr->status     |=  0x10;
      }

      log_ptr->ir_status = 0;

      /* bit 0 indicated IR used for first payload */
      if (rpt_ptr->egprs.IR_used[0])
      {
        log_ptr->ir_status |= 0x1;
      }

      /* Bit 1 used when IR used for second payload */
      if (rpt_ptr->egprs.IR_used[1])
      {
        log_ptr->ir_status |= 0x2;
      }


      /* Calculate mean bep and cv bep for MCS decodes */
      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING)
      {
        blk_cv_bep_tslot = egpl1_calc_block_statistical_params((uint16*)rpt_ptr->egprs.bep,
                                                               &blk_mean_bep_tslot, gas_id);

        log_ptr->blk_cv_bep_tslot = blk_cv_bep_tslot;
        log_ptr->blk_mean_bep_tslot = blk_mean_bep_tslot;

        log_ptr->filtered_mean_bep_tslot = usf_dl_tfi_info->mean_bep_tn;
        log_ptr->filtered_cv_bep_tslot = usf_dl_tfi_info->cv_bep_tn;
        log_ptr->reliability = usf_dl_tfi_info->r;

      }else
      {
        log_ptr->blk_cv_bep_tslot = 0;
        log_ptr->blk_mean_bep_tslot = 0;
      }

      /* not currently setup, (may become redundant) */
      log_ptr->mod_detected = 0;



      /* USF now extracted from 1st 3 bits of EGPRS header or GPRS payload
       * since MCS 7 - 9 data callback later than 4th metrics
       */
      log_ptr->usf  = (uint8)usf_dl_tfi_info->usf;
      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_d_ver2...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_gprs_message_metrics_e_ver2

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_message_metrics_e_ver2  ( gl1_msg_dl_chn_type channel, uint32 fn,
                                              const gl1_defs_rx_pkt_data_type *rpt_ptr,
                                              const usf_dl_tfi_info_t         *usf_dl_tfi_info, gas_id_t gas_id )
{
    LOG_GPRS_RECEIVE_MSG_METRICS_E_VER2_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    uint16 blk_mean_bep_tslot = 0;
    uint16 blk_cv_bep_tslot = 0;

    if ( !rpt_ptr )
    {   // Sanity Check
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_RECEIVE_MSG_METRICS_E_VER2_C, sizeof(LOG_GPRS_RECEIVE_MSG_METRICS_E_VER2_C_type), gas_id);
    log_ptr = (LOG_GPRS_RECEIVE_MSG_METRICS_E_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)rpt_ptr->tn;
      log_ptr->coding_scheme =  ((byte)rpt_ptr->coding_scheme + 1);

      log_ptr->rx_qual       =  rpt_ptr->msg[0].rx_qual;

      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS7_CODING)
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len + rpt_ptr->msg[1].msg_len;
      }else
      {
        log_ptr->msg_len = rpt_ptr->msg[0].msg_len;
      }
      log_ptr->status        =  0;
      log_ptr->status       |= 0x3; /* bit 0 and 1 not used */

      /* Bit 2. Used as GPRS CRC for CS 1 - 4 */
      if ((rpt_ptr->msg[0].crc_passed) && (rpt_ptr->coding_scheme <= GL1_DEFS_CS4_CODING))
      {
        log_ptr->status     |=  0x4;
      }

      /* Bit 5 N/A */
      /* Bit 6 used for USF matched */
      if (usf_dl_tfi_info->usf_match)
      {
       log_ptr->status |= 0x40;
      }
      /* Bit 7 used for DL TFI matched */
      if (usf_dl_tfi_info->dl_tfi_match)
      {
        log_ptr->status |= 0x80;
      }


      /* bit 2. used as EGPRS header CRC */
      if ((uint8)rpt_ptr->egprs.hdr_crc_passed)
      {
        log_ptr->status     |=  0x4;
      }

       /* Bit 3 Used for EGPRS message 1 CRC */
      if ((rpt_ptr->msg[0].crc_passed) &&  (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING) )
      {
        log_ptr->status     |=  0x8;
      }

      /* Bit 4 Used when a dual payload EGPRS message is present */
      if ((uint8)rpt_ptr->msg[1].crc_passed)
      {
        log_ptr->status     |=  0x10;
      }

      log_ptr->ir_status = 0;

      /* bit 0 indicated IR used for first payload */
      if (rpt_ptr->egprs.IR_used[0])
      {
        log_ptr->ir_status |= 0x1;
      }

      /* Bit 1 used when IR used for second payload */
      if (rpt_ptr->egprs.IR_used[1])
      {
        log_ptr->ir_status |= 0x2;
      }


      /* Calculate mean bep and cv bep for MCS decodes */
      if (rpt_ptr->coding_scheme >= GL1_DEFS_MCS1_CODING)
      {
        blk_cv_bep_tslot = egpl1_calc_block_statistical_params((uint16*)rpt_ptr->egprs.bep,
                                                               &blk_mean_bep_tslot, gas_id);

        log_ptr->blk_cv_bep_tslot = blk_cv_bep_tslot;
        log_ptr->blk_mean_bep_tslot = blk_mean_bep_tslot;

        log_ptr->filtered_mean_bep_tslot = usf_dl_tfi_info->mean_bep_tn;
        log_ptr->filtered_cv_bep_tslot = usf_dl_tfi_info->cv_bep_tn;
        log_ptr->reliability = usf_dl_tfi_info->r;

      }else
      {
        log_ptr->blk_cv_bep_tslot = 0;
        log_ptr->blk_mean_bep_tslot = 0;
      }

      /* not currently setup, (may become redundant) */
      log_ptr->mod_detected = 0;



      /* USF now extracted from 1st 3 bits of EGPRS header or GPRS payload
       * since MCS 7 - 9 data callback later than 4th metrics
       */
      log_ptr->usf  = (uint8)usf_dl_tfi_info->usf;
      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_message_metrics_e_ver2...");
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  gpl1_log_srb_hard_decisions

DESCRIPTION
  This function should be called either at point FW passes the DL hard decisions
  to L1, or L1 sends the UL hard decisions for SRB lookback. Please make sure this
  is for SRB lookback only (the hard decision is not obtained with the help of
  channel coding).

  The MCS field has 11 as LSB reserved and the rest of 14 bits reserved as well. They can be used for other channel
  bit stream logging.


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_srb_hard_decisions  ( uint32 FN, uint16* HD_ptr, uint16 mcs, gas_id_t gas_id )//mcs can be either 16 or 8 bits, which suit lint.
{

    uint8 i;
    LOG_EGPRS_SRB_HARD_DECISIONS_C_type *log_ptr;
    gsmdiag_pkt_info_t pkt_info;

    if ( !HD_ptr )
    {   /* Sanity Check */
        return;
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_EGPRS_SRB_HARD_DECISIONS_C, sizeof(LOG_EGPRS_SRB_HARD_DECISIONS_C_type), gas_id);
    log_ptr = (LOG_EGPRS_SRB_HARD_DECISIONS_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      log_ptr->frame_number = FN;//store FN now in this field
      log_ptr->mcs = mcs & 0x03; /* 2-bit number, assign it to 16 bit. Reserve the rest for future. */
 
   
      (void)memscpy((void *) log_ptr->hd_348bits, sizeof(log_ptr->hd_348bits),(void *)HD_ptr, 44 /*byte len of HD*/ );


/*GMSK, only 116 bits (2 x 58), occupying 15 bytes.

int16     |16   |   0       1        2       3        4        5        6       7       8
brst_ct  |mcs | 1  0   3   2   5   4   7  6    9   8  11  10 13 12 15 14 17 16
0x51 23 00 00 CF 5C D7 C4 F8 21 08 AD AE E2 82 56 8E 75 8B C9 47 39 88 44 00 A5 F8 99 1A AF 69 F2 79 79 89 65 FB 97 58 D5 90 6B 0B 9F 8C A1 7F 45
                      xx xx xx xx............................................... 00 xx 00 00 00 00...

*/
      if ((log_ptr->mcs) == 0)
      {
        for (i=8; i<22; i++)
        {
          log_ptr->hd_348bits[i] = 0;
        }

        log_ptr->hd_348bits[7] &= 0xFFF0;
      }
      else if((log_ptr->mcs) == 1) /*8PSK 348 bits */
      {
        log_ptr->hd_348bits[21] &= 0xFFF0;
      }
      else
      {
        MSG_GERAN_LOW_0_G("unknown mod (mcs>=2) in LOG_EGPRS_SRB_HARD_DECISIONS.");
      }

      gsmdiag_log_commit(&pkt_info);

    }
    else
    {
         MSG_GERAN_LOW_0_G("Log packet LOG_EGPRS_SRB_HARD_DECISIONS.allocation failed.");
    }

}

/*===========================================================================
FUNCTION  gpl1_log_edge_message_metrics

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.
  
RETURN VALUE
  None

SIDE EFFECTS
  None                               
===========================================================================*/
void  gpl1_log_edge_message_metrics  ( gl1_msg_dl_chn_type channel, uint32 fn, byte tn,
                                       const GfwEnhancedEdgeLogPacket *edge_data,
                                       gas_id_t gas_id  )
{
    LOG_GPRS_EDGE_ENHANCED_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GPRS_EDGE_ENHANCED_C, sizeof(LOG_GPRS_EDGE_ENHANCED_C_type), gas_id);
    log_ptr = (LOG_GPRS_EDGE_ENHANCED_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr)
    {
      // Fill packet and commit it
      log_ptr->version       =   LOG_EGPRS_MESSAGE_METRICS_C_VER;
      log_ptr->chan          =  (byte)channel;
      log_ptr->FN            =  fn;
      log_ptr->TN            =  (byte)tn;
      log_ptr->edge_log_packet = *edge_data;
      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_LOW("Not sending edge_message_metrics...", 0, 0, 0);
    }//endif log_ptr
}




#ifdef FEATURE_GSM_LOG_IQ_SAMPLES
/*===========================================================================
FUNCTION  gpl1_log_mdsp_debug_buffer

DESCRIPTION
  This function should be called for mainly IQ analysis with GMSK or 8PSK
  modulations. The MDSP debug buffer will have CIR, soft decisions and the
  format is defined in MDSP_DEBUG_BUFFER of GSM Modem DSP Interface_latest.doc
  (the doc is out of date, update is needed). The code below is following
  the actual structure in the FW).

  Any change of the interface will have impact to this logging.

  Due to 2 kB for each burst, this log packet should be used with care,
  and the right level of log configuration need to be tried before formal
  log collection.

RETURN VALUE
  None

SIDE EFFECTS
  Check the dropped messages and impact to the ISR overrun.
===========================================================================*/
void gpl1_log_mdsp_debug_buffer( uint16 *MDSP_buffer_p,      uint32   FN,
                                 uint16 TSC, uint16 channel, gas_id_t gas_id )
{
  LOG_MDSP_IQ_SAMPLES_C_type *log_ptr;
  uint16                      data_offset = 3;
  gsmdiag_pkt_info_t pkt_info;

  if ( !MDSP_buffer_p )
  {
    MSG_GERAN_ERROR_0_G( "Log packet MDSP_buffer_p NULL");

    return;
  }

  /* Try to allocate memory for the log packet */
  l1_log_packet_init(&pkt_info, LOG_MDSP_IQ_SAMPLES_C, sizeof(LOG_MDSP_IQ_SAMPLES_C_type), gas_id);
  log_ptr = (LOG_MDSP_IQ_SAMPLES_C_type *)gsmdiag_log_alloc(&pkt_info);

  if ( log_ptr )
  {
    /* Allow for frame offset from mDSP to Host */
    log_ptr->FN      = SUB_FN( FN, 1 );
    log_ptr->TSC     = TSC;
    log_ptr->channel = channel;

    /* Set data offset to 0 as we only collect IQ data for QDSP6 */
    memset(log_ptr->CIR,NULL,sizeof(log_ptr->CIR));
    log_ptr->CIR[0] = IQ_CIR_SIZE-1;

    memset(log_ptr->SAIC,NULL,sizeof(log_ptr->SAIC));
    log_ptr->SAIC[0] = IQ_SAIC_SIZE-1;

    /* Collect IQ data */
    data_offset = 0;
    gpl1_collect_iq_log_data( (uint16 *)log_ptr->IQ, ( MDSP_buffer_p + data_offset ), IQ_DATA_SIZE );

    memset(log_ptr->CE,NULL,sizeof(log_ptr->CE));
    log_ptr->CE[0] = IQ_CORR_ENERGY_SIZE-1;

    gsmdiag_log_commit(&pkt_info);
  }
  else
  {
    MSG_GERAN_LOW_1_G( "Log packet LOG_MDSP_IQ_SAMPLES allocation failed FN %d", GSTMR_GET_FN_GERAN( gas_id));
  }
}


/*===========================================================================
FUNCTION  gpl1_collect_iq_log_data

DESCRIPTION
  Helper function to populate output logging buffer with data from the
  input mDSP debug buffer

RETURN VALUE
  The new offset to the next element in the input debug buffer

SIDE EFFECTS
  None
===========================================================================*/
static uint16 gpl1_collect_iq_log_data( uint16 *out_data_p, uint16 *in_data_p,
                                        uint16 max_data_length )
{
 uint16 data_size = max_data_length;

  /* Needed for old F/W reporting wrong data length */
  if ( data_size > max_data_length )
  {
    MSG_GERAN_ERROR_2_G( "IQ log data size from mDSP is too large reported %d max %d",
               data_size, max_data_length);

    data_size = max_data_length;
  }

  /* Collect metrics */
  memscpy( (void *)out_data_p,( data_size * sizeof( int16 ) ),(void *)in_data_p, ( data_size * sizeof( int16 ) ) );
     
  /* To be used to update the input data location for subsequent copies */
  return ( data_size );
}
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */

/*===========================================================================
FUNCTION  gpl1_log_gprs_air_interface_summary

DESCRIPTION
  Creates, populates, and sends a "GPRS Air Interface Summary" log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_gprs_air_interface_summary( boolean transfer_ind, gas_id_t gas_id )
{
  /* Try to allocate memory for the log packet
   * - will fail if the log mask is not set appropriately
   */
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gsmdiag_pkt_info_t pkt_info;
  LOG_GPRS_AIR_INTERFACE_SUMMARY_C_type *log_ptr;



  /* Try to allocate memory for the log packet */
  l1_log_packet_init(&pkt_info, LOG_GPRS_AIR_INTERFACE_SUMMARY_C, sizeof(LOG_GPRS_AIR_INTERFACE_SUMMARY_C_type), gas_id);
  log_ptr = (LOG_GPRS_AIR_INTERFACE_SUMMARY_C_type *)gsmdiag_log_alloc(&pkt_info);

  if( NULL != log_ptr )
  {
    const rlc_log_rfblks_cnts_t*  grlc_rfblks_cnts_ptr;

    uint32  local_llpdu_tx_sum = 0;
    uint32  local_llpdu_retx   = 0;
    uint32  local_llpdu_rx_sum = 0;
    uint32  local_llpdu_rx_err = 0;

    uint32  local_dl_octet = 0;
    uint32  local_ul_octet = 0;

    uint8 nsapi;
    uint8 sapi = 0;

    /* Initialise tx power array content */
    log_ptr->tx_power_timeslot[0] = 0;
    log_ptr->tx_power_timeslot[1] = 0;
    log_ptr->tx_power_timeslot[2] = 0;
    log_ptr->tx_power_timeslot[3] = 0;

    /* Populate the log packet here */

    for (sapi=1; sapi<=11; sapi+=2)
    {
      const gllc_diag_pdu_stat_rsp_t*  gllc_pdu_ptr = gllc_diag_get_pdu_stats(gas_id, sapi ); /* Pointer to stats inside LLC */

      if (gllc_pdu_ptr != NULL)
      {
        local_llpdu_tx_sum = local_llpdu_tx_sum + gllc_pdu_ptr->llpdu_tx_cnt;
        local_llpdu_retx   = local_llpdu_retx   + gllc_pdu_ptr->ack_l3pdu_retx_cnt;
        local_llpdu_rx_sum = local_llpdu_rx_sum + gllc_pdu_ptr->llpdu_rx_cnt;

        local_llpdu_rx_err = local_llpdu_rx_err +
                             gllc_pdu_ptr->llpdu_fcs_err_cnt +
                             gllc_pdu_ptr->llpdu_frm_rej_cnt +
                             gllc_pdu_ptr->llpdu_tlli_err_cnt +
                             gllc_pdu_ptr->llpdu_addr_err_cnt +
                             gllc_pdu_ptr->llpdu_short_err_cnt;
      }
    }

    log_ptr->llpdu_tx_sum   = local_llpdu_tx_sum;
    log_ptr->llpdu_retx     = local_llpdu_retx;
    log_ptr->llpdu_rx_sum   = local_llpdu_rx_sum;
    log_ptr->llpdu_rx_err   = local_llpdu_rx_err;

    /* SNDCP NSAPI range is 5 to 15 inclusive. */
    for (nsapi = GSN_EXTERN_NSAPI((uint8)GSN_NSAPI_5); nsapi < GSN_EXTERN_NSAPI((uint8)GSN_NSAPI_MAX); nsapi++)
    {
      const gsndcp_diag_pdu_stat_rsp_t*  sndcp_oct_ptr = gsndcp_diag_get_pdu_stats( nsapi ); /* Pointer to stats inside SNDCP */

       if (sndcp_oct_ptr != NULL)
       {
         local_dl_octet = local_dl_octet + sndcp_oct_ptr->snpdu_octet_rx_cnt + sndcp_oct_ptr->npdu_octet_rx_cnt;
         local_ul_octet = local_ul_octet + sndcp_oct_ptr->snpdu_octet_tx_cnt + sndcp_oct_ptr->npdu_octet_tx_cnt;
       }
    }

    log_ptr->user_oct_dl    = local_dl_octet;
    log_ptr->user_oct_ul    = local_ul_octet;

    log_ptr->timing_advance = gl1_msg_pdch_retrieve_ta(gas_id);

    grlc_rfblks_cnts_ptr = rlc_log_rfblks_cnts(gas_id);

    if( NULL != grlc_rfblks_cnts_ptr )
    {
      log_ptr->rlc_dl_rx    = grlc_rfblks_cnts_ptr->dl_rx;
      log_ptr->rlc_ul_tx    = grlc_rfblks_cnts_ptr->ul_tx;
      log_ptr->rlc_ul_retx  = grlc_rfblks_cnts_ptr->ul_retx;
    }

    log_ptr->FN = gl1_get_FN(gas_id);

    log_ptr->band_ind    = (byte)gprs_l1_idle_data->gprs_campedon_cell_arfcn.band;

    log_ptr->dl_timeslots = 0;
    log_ptr->ul_timeslots = 0;

    if( FALSE == transfer_ind )
    {
      log_ptr->rx_power = 0;

      gsmdiag_log_commit(&pkt_info);

      return;
    }

    log_ptr->rx_power = rx_pwr >> 4;

    switch( l1_transfer_data[gas_id].current_alloc_type )
    {
    case  L1_DYNAMIC_ALLOC:
    {
      if(  ( l1_transfer_data[gas_id].dynamic_tbf_ptr == &( l1_transfer_data[gas_id].dynamic_tbf_1 ) )
         ||( l1_transfer_data[gas_id].dynamic_tbf_ptr == &( l1_transfer_data[gas_id].dynamic_tbf_2 ) )
        )
      {
        uint32  i;
        uint32  loop_num = 0;

        log_ptr->ul_timeslots = l1_transfer_data[gas_id].dynamic_tbf_ptr->nts;
        log_ptr->dl_timeslots = l1_transfer_data[gas_id].dynamic_tbf_ptr->nts;

        for (i=0; i<8; i++)
        {
          if (l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[i] != GPL1_INVALID)
          {
            convert_txlev_to_pwr_dBm( (sys_band_T)  log_ptr->band_ind,
                                       (byte)       gprs_serv_cell_meas_ptr->pch[i],
                                       (uint8 *)    &(log_ptr->tx_power_timeslot[loop_num]),
                                       gas_id
                                     );

            if ( loop_num <
                  ( ( sizeof( log_ptr->tx_power_timeslot ) / sizeof( log_ptr->tx_power_timeslot[0] ) - 1 ) ) )
            {
               loop_num++;
            }
            else
            {
               break;
            }
          }
        }
      }
      else
      {
        MSG_GERAN_ERROR_1_G(" Invalid dyn tbf ptr 0x%p ", l1_transfer_data[gas_id].dynamic_tbf_ptr);
      }
    }
    break;

    case  L1_SINGLE_BLOCK_ALLOC:
       {
         if(  ( l1_transfer_data[gas_id].single_block_tbf_ptr == &( l1_transfer_data[gas_id].single_block_tbf_1 ) )
            ||( l1_transfer_data[gas_id].single_block_tbf_ptr == &( l1_transfer_data[gas_id].single_block_tbf_2 ) )
           )
         {
           if (l1_transfer_data[gas_id].single_block_tbf_ptr->sb_ul_dl_type == SINGLE_UL_BLOCK)
           {
             log_ptr->ul_timeslots = 1<<l1_transfer_data[gas_id].single_block_tbf_ptr->ts_number;
             log_ptr->dl_timeslots = 0;

             convert_txlev_to_pwr_dBm( (sys_band_T) log_ptr->band_ind,
                                       (byte)       gprs_serv_cell_meas_ptr->pch[ l1_transfer_data[gas_id].single_block_tbf_ptr->ts_number ],
                                       (uint8 *)    ( &log_ptr->tx_power_timeslot[0] ),
                                       gas_id
                                     );
           }
           else if (l1_transfer_data[gas_id].single_block_tbf_ptr->sb_ul_dl_type == SINGLE_DL_BLOCK)
           {
             log_ptr->dl_timeslots = 1<<l1_transfer_data[gas_id].single_block_tbf_ptr->ts_number;
             log_ptr->ul_timeslots = 0;
           }
         }
         else
         {
           MSG_GERAN_ERROR_1_G(" Invalid sngl blk tbf ptr 0x%p ", l1_transfer_data[gas_id].single_block_tbf_ptr);
         }
       }
       break;

     case  L1_DOWNLINK_ALLOC:
       {
         if(  ( l1_transfer_data[gas_id].dl_tbf_ptr == &( l1_transfer_data[gas_id].dl_tbf_1 ) )
            ||( l1_transfer_data[gas_id].dl_tbf_ptr == &( l1_transfer_data[gas_id].dl_tbf_2 ) )
           )
         {
           log_ptr->dl_timeslots = l1_transfer_data[gas_id].dl_tbf_ptr->nts;
           log_ptr->ul_timeslots = 0;
         }
         else
         {
           MSG_GERAN_ERROR_1_G(" Invalid dl tbf ptr 0x%p ", l1_transfer_data[gas_id].dl_tbf_ptr);
         }
       }
     break;

     case  L1_DYNAMIC_DOWN_ALLOC:
       {
         if(  ( l1_transfer_data[gas_id].dynamic_tbf_ptr == &( l1_transfer_data[gas_id].dynamic_tbf_1 ) )
            ||( l1_transfer_data[gas_id].dynamic_tbf_ptr == &( l1_transfer_data[gas_id].dynamic_tbf_2 ) )
           )
         {
           if(  ( l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr == &( l1_transfer_data[gas_id].dl_tbf_1 ) )
              ||( l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr == &( l1_transfer_data[gas_id].dl_tbf_2 ) )
             )
           {
             uint32  i;
             uint32  loop_num = 0;

             log_ptr->ul_timeslots = l1_transfer_data[gas_id].dynamic_tbf_ptr->nts;
             log_ptr->dl_timeslots = l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr->nts;

             for (i=0; i<8; i++)
             {
               if (l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[i] != GPL1_INVALID)
               {
                 convert_txlev_to_pwr_dBm( (sys_band_T)  log_ptr->band_ind,
                                           (byte)        gprs_serv_cell_meas_ptr->pch[i],
                                           (uint8 *)     &(log_ptr->tx_power_timeslot[loop_num]),
                                           gas_id
                                         );

                 if ( loop_num <
                        ( ( sizeof( log_ptr->tx_power_timeslot ) / sizeof( log_ptr->tx_power_timeslot[0] ) - 1 ) ) )

                 {
                   loop_num++;
                 }
                 else
                 {
                   break;
                 }
               }
             }
           }
           else
           {
             MSG_GERAN_ERROR_3_G(" Invalid dl tbf ptr 0x%p ",
                       l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr,
                       0,
                       0 );
           }
         }
         else
         {
           MSG_GERAN_ERROR_3_G(" Invalid dyn tbf ptr 0x%p ",
                     l1_transfer_data[gas_id].dynamic_tbf_ptr,
                     0,
                     0 );
         }
       }
      break;

      default:
        break;
    } /* End of switch on current alloc type */

    gsmdiag_log_commit(&pkt_info);
  }
  else
  {
    MSG_GERAN_LOW_0_G(" NULL log ptr ");
  }
}

/*===========================================================================
FUNCTION  gpl1_log_agc

DESCRIPTION
  Creates, populates, and sends a "GPRS AGC" log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_agc(const sys_algo_agc_T *agc_ptr, const gprs_agc_info_t *gprs_agc_info, gas_id_t gas_id)
{
    LOG_GPRS_AGC_C_type *log_ptr;
    gsmdiag_pkt_info_t pkt_info;

    /* Try to allocate memory for the log packet            */
    /* - will fail if the log mask is not set appropriately */
    l1_log_packet_init(&pkt_info, LOG_GPRS_AGC_C, sizeof(LOG_GPRS_AGC_C_type), gas_id);
    log_ptr = (LOG_GPRS_AGC_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr != NULL)
    {
      /* Populate the log packet here */
      log_ptr->ARFCN = arfcn_to_word(agc_ptr->arfcn);
      log_ptr->FN = agc_ptr->FN;
      log_ptr->channel = gprs_agc_info->gprs_channel;
      log_ptr->rx_p_ref_bcch = agc_ptr->pwr_dBm_x16;
      log_ptr->pr_a = gprs_agc_info->pr_a;
      log_ptr->pr_b = gprs_agc_info->pr_b;
      log_ptr->pr_c = gprs_agc_info->pr_c;
      log_ptr->pr_d = gprs_agc_info->pr_d;
      log_ptr->p0 = gprs_agc_info->p0;
      log_ptr->pb = gprs_agc_info->pb;
      log_ptr->tfi_usf = gprs_agc_info->tfi_usf;
      log_ptr->srl = agc_ptr->srl_enabled;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending AGC...");
    }//endif log_ptr

}

/*===========================================================================
FUNCTION  gpl1_log_timing_advance

DESCRIPTION
  Creates, populates, and sends a "GPRS Timing Advance" log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_timing_advance( const ta_description_T* active_ta_params,
                              uint8                   ta_value, gas_id_t gas_id )
{
    LOG_GPRS_TIMING_ADVANCE_C_type *log_ptr;
    gsmdiag_pkt_info_t pkt_info;

    /* Try to allocate memory for the log packet            */
    /* - will fail if the log mask is not set appropriately */
    l1_log_packet_init(&pkt_info, LOG_GPRS_TIMING_ADVANCE_C, sizeof(LOG_GPRS_TIMING_ADVANCE_C_type), gas_id);
    log_ptr = (LOG_GPRS_TIMING_ADVANCE_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr != NULL)
    {
      /* Populate the log packet here */
      log_ptr->ta_continous_value = ta_value;

      if ((active_ta_params->ta_param_ptr != NULL) &&
          (active_ta_params->ta_param_ptr->present))
      {
        log_ptr->ta_index = active_ta_params->ta_param_ptr->tai;
        log_ptr->ta_timeslot = active_ta_params->ta_param_ptr->ta_timeslot;
      }
      else
      {
        log_ptr->ta_index = 0;
        log_ptr->ta_timeslot = 0;
      }

      if (active_ta_params->tav.present)
      {
        log_ptr->ta_assigned_value = active_ta_params->tav.ta_value;
      }
      else
      {
        log_ptr->ta_assigned_value = 0;
      }

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending timing_advance...");
    }//endif log_ptr

}

/*===========================================================================
FUNCTION  gpl1_log_gprs_power_control

DESCRIPTION
  Creates, populates, and sends a "GPRS Power Control Parameters" log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_power_control(gl1_msg_ul_chn_type channel,
                                 gl1_defs_tn_type tn,
                                 dBx16_T tx_pwr,
                                 gl1_defs_coding_type cs, gas_id_t gas_id)
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  dBx16_T tmp_tx_pwr = tx_pwr;

    LOG_GPRS_POWER_CONTROL_C_type *log_ptr;
    gsmdiag_pkt_info_t pkt_info;
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    uint8 n,num_bursts = 1;

    if ( channel == GL1_MSG_UL_PACCH_PDTCH)
    {
      num_bursts = 4;
    }

    for(n=0;n<num_bursts;++n)
    {
      /* burst level averaging */
      gpl1_update_tx_pwr_avg(&tmp_tx_pwr,gprs_serv_cell_meas_ptr->ul_power_band, cs, gas_id );
    }
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

    /* Try to allocate memory for the log packet            */
    /* - will fail if the log mask is not set appropriately */
    l1_log_packet_init(&pkt_info, LOG_GPRS_POWER_CONTROL_C, sizeof(LOG_GPRS_POWER_CONTROL_C_type), gas_id);
    log_ptr = (LOG_GPRS_POWER_CONTROL_C_type *)gsmdiag_log_alloc(&pkt_info);

    if (log_ptr != NULL)
    {
      /* Populate the log packet here */
      log_ptr->tx_channel    = (byte)channel;
      log_ptr->tx_tn         = (byte)tn;
      log_ptr->tx_pwr        = tmp_tx_pwr;
      log_ptr->coding_scheme = (byte)++cs;
      log_ptr->gamma_tn      = gprs_serv_cell_meas_ptr->gamma_tn[tn];
      log_ptr->gamma_band    = (byte)(gprs_serv_cell_meas_ptr->gamma_gsm/16);
      log_ptr->alpha         = gprs_serv_cell_meas_ptr->alpha;
      log_ptr->derivedC      = (byte)gprs_serv_cell_meas_ptr->reported_rxlev;
      log_ptr->pmax          = gprs_serv_cell_meas_ptr->pmax;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G("Not sending gprs_power_control...");
    }//endif log_ptr

}

/*===========================================================================
FUNCTION  gpl1_log_gprs_transfer_summary

DESCRIPTION
  Creates, populates, and sends a "GPRS Air Interface Summary" log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_log_gprs_transfer_summary(gas_id_t gas_id)
{
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);

  byte i;
  uint8 ta_timeslot;

  LOG_GPRS_TRANSFER_SUMMARY_C_type *log_ptr;
  gsmdiag_pkt_info_t pkt_info;

  l1_log_packet_init(&pkt_info, LOG_GPRS_TRANSFER_SUMMARY_C, sizeof(LOG_GPRS_TRANSFER_SUMMARY_C_type), gas_id);
  log_ptr = (LOG_GPRS_TRANSFER_SUMMARY_C_type *)gsmdiag_log_alloc(&pkt_info);

  if (log_ptr != NULL)
  {
     ta_timeslot = gpl1_gprs_get_global_ta_timeslot(gas_id);

     log_ptr->band_ind          = (byte)gprs_l1_idle_data->gprs_campedon_cell_arfcn.band;
     log_ptr->ul_ptcch_timeslot = ((ta_timeslot + 3) & 7);
     log_ptr->dl_ptcch_timeslot = ta_timeslot;
     log_ptr->timing_advance    = gl1_msg_pdch_retrieve_ta(gas_id);

     log_ptr->current_alloc     = (byte)l1_transfer_data[gas_id].current_alloc_type;
     log_ptr->usf_granularity   = 0;
     log_ptr->ul_bitmap_tn      = 0;
     log_ptr->dl_bitmap_tn      = 0;

     if (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_ALLOC)
     {
       if(  ( &( l1_transfer_data[gas_id].dynamic_tbf_1 ) == l1_transfer_data[gas_id].dynamic_tbf_ptr )
          ||( &( l1_transfer_data[gas_id].dynamic_tbf_2 ) == l1_transfer_data[gas_id].dynamic_tbf_ptr )
         )
       {
         log_ptr->usf_granularity = l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_granularity;

         for (i=0; i<8; i++)
         {
           if (l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[i] != GPL1_INVALID)
           {
             log_ptr->ul_bitmap_tn |= (1<<i);
           }
         }

         log_ptr->dl_bitmap_tn = log_ptr->ul_bitmap_tn;
       }
       else
       {
         MSG_GERAN_ERROR_3_G(" Attempt to de-ref 0x%p dynamic_tbf_ptr ",
                   l1_transfer_data[gas_id].dynamic_tbf_ptr,
                   0,
                   0 );
       }
     }
     else if (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC)
     {
#if 0
       if (l1_transfer_data[gas_id].single_block_tbf.sb_ul_dl_type == SINGLE_UL_BLOCK)
       {
         log_ptr->ul_bitmap_tn = 1<<l1_transfer_data[gas_id].single_block_tbf.ts_number;
         log_ptr->dl_bitmap_tn = 0;
       }
       else if (l1_transfer_data[gas_id].single_block_tbf.sb_ul_dl_type == SINGLE_DL_BLOCK)
       {
         log_ptr->dl_bitmap_tn = 1<<l1_transfer_data[gas_id].single_block_tbf.ts_number;
         log_ptr->ul_bitmap_tn = 0;
       }
#endif
     }
     else if (l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC)
     {
       if(  ( &( l1_transfer_data[gas_id].dl_tbf_1 ) == l1_transfer_data[gas_id].dl_tbf_ptr )
          ||( &( l1_transfer_data[gas_id].dl_tbf_2 ) == l1_transfer_data[gas_id].dl_tbf_ptr )
         )
       {
         log_ptr->dl_bitmap_tn = l1_transfer_data[gas_id].dl_tbf_ptr->ts_allocation;
       }
       else
       {
         MSG_GERAN_ERROR_3_G(" Attempt to de-ref 0x%p dl_tbf_ptr ",
                   l1_transfer_data[gas_id].dl_tbf_ptr,
                   0,
                   0 );
       }

       log_ptr->ul_bitmap_tn = 0;
     }
     else if (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
     {
       if(  ( l1_transfer_data[gas_id].dynamic_tbf_ptr == &( l1_transfer_data[gas_id].dynamic_tbf_1 ) )
          ||( l1_transfer_data[gas_id].dynamic_tbf_ptr == &( l1_transfer_data[gas_id].dynamic_tbf_2 ) )
         )
       {
         if(  ( l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr == &( l1_transfer_data[gas_id].dl_tbf_1 ) )
            ||( l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr == &( l1_transfer_data[gas_id].dl_tbf_2 ) )
           )
         {
           log_ptr->usf_granularity = l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_granularity;

           for (i=0; i<8; i++)
           {
             log_ptr->dl_bitmap_tn = l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;

             if (l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[i] != GPL1_INVALID)
             {
               log_ptr->ul_bitmap_tn |= (1<<i);
               log_ptr->dl_bitmap_tn |= (1<<i);
             }
           }
         }
         else
         {
           MSG_GERAN_ERROR_3_G(" Invalid dl tbf ptr 0x%p ",
                     l1_transfer_data[gas_id].dynamic_tbf_ptr->dl_tbf_ptr,
                     0,
                     0 );
         }
       }
       else
       {
         MSG_GERAN_ERROR_3_G(" Attempt to de-ref 0x%p dynamic_tbf_ptr ",
                   l1_transfer_data[gas_id].dynamic_tbf_ptr,
                   0,
                   0 );
       }
     }

     gsmdiag_log_commit(&pkt_info);
   }
   else
   {
     MSG_GERAN_LOW_0_G("Not sending gprs_transfer_summary...");
   }
}


/*===========================================================================

FUNCTION convert_txlev_to_pwr_dBm

DESCRIPTION
  This function maps the TXLEV based on RF band information to a power
  index used by the HW layer.
  NOTE: The power index is mapped in revers order from TXLEV.  It is
  also offset by the min TXLEV for the power class.  So for power class 4
  in GSM900 the power index is offset by 5 such that power index 0
  is TXLEV 5.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void convert_txlev_to_pwr_dBm(sys_band_T band, uint8 txlev, uint8 *power_out, gas_id_t gas_id)
{
  byte convert_pmax_900[] =  {5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,39,39};
  byte convert_pmax_1800[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36};
  byte convert_band_index900[] = {14,18,16,14,12}; /* 33dbm,39,37,33,29 */

  switch (band)
  {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_CELL_850:
    {
      /* Limit the PCL as per Power Class for each band */
      if ( txlev  < gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1] )
      {
        *power_out  = convert_pmax_900[ convert_band_index900[gl1_rf_power_class_per_band[gas_id][band]] ];
      } else if( txlev  < 19 )
      {
        *power_out  = convert_pmax_900[19 - txlev];
      } else
      { /* txlev (19..31) */
        *power_out  = convert_pmax_900[0];                   /* Min dBm Power, 5dBm   */
      }
    }
    break;

    case SYS_BAND_DCS_1800:
    case SYS_BAND_PCS_1900:
    {
      /* Power Class 1 Mobile, TXLEV = 0 ==> 30 dBm */
      if( txlev  < 15 )
      {
        *power_out  = convert_pmax_1800[15 - txlev];
      } else if( txlev < 28 )
      {
        *power_out = convert_pmax_1800[0];                   /* Min dBm Power, 0dBm   */
      } else
      { /* txlev (29,30,31) */

        if ( gl1_rf_power_class_per_band[gas_id][band] == 0 ) /* class 1 */
          *power_out  = convert_pmax_1800[15];    /* Max dBm Power, 30 dBm */
        else if ( gl1_rf_power_class_per_band[gas_id][band] == 1 ) /* class 2 */
          *power_out  = convert_pmax_1800[12];    /* Max dBm Power, 24 dBm */
      }
    }
    break;

    default:
    {
      MSG_GERAN_ERROR_1("Unsupported band class: %d", band);
      *power_out = 0;
    }
  }
}


/*===========================================================================

FUNCTION gpl1_log_uplink_tbf_data_block_count

DESCRIPTION
  This function generates the uplink tbf data/dummy block
  statistics log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_log_uplink_tbf_data_block_count( const ul_data_blk_count_t*  ul_data_blk_count_ptr , gas_id_t gas_id)
{

  gsmdiag_pkt_info_t pkt_info;

    /* Setup a log packet pointer */
  LOG_UPLINK_TBF_DATA_BLOCK_COUNT_C_type*  log_ptr;

    l1_log_packet_init(&pkt_info, LOG_UPLINK_TBF_DATA_BLOCK_COUNT_C, sizeof(LOG_UPLINK_TBF_DATA_BLOCK_COUNT_C_type), gas_id);
    log_ptr = (LOG_UPLINK_TBF_DATA_BLOCK_COUNT_C_type *)gsmdiag_log_alloc(&pkt_info);


    MSG_GERAN_MED_3_G("PL1: UL TBF stats. Num data blks %d Num dummy blks %d [%d]",
                  ul_data_blk_count_ptr->number_rlc_data_blks,
                  ul_data_blk_count_ptr->number_dummy_ctrl_blks,
                  gl1_get_FN(gas_id) );

    /* Make sure the pointer address is not NULL */
    if( NULL != log_ptr )
    {
        /* Fill in the log packet contents */
        log_ptr->number_of_rlc_data_blocks =
            ul_data_blk_count_ptr->number_rlc_data_blks;

        log_ptr->number_of_dummy_ctrl_blocks =
            ul_data_blk_count_ptr->number_dummy_ctrl_blks;

        /* Log packet is filled, call the commit function */
        gsmdiag_log_commit(&pkt_info);
    }
    else
    {
        /* Generate an error message */
        MSG_GERAN_LOW_0_G(" Failed to generate ul block stats log pkt memory ");
    }
}

