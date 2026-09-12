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

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_log.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
25/04/18   rv       CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
15/02/18   nsa     CR2189790 Logging pch burst even it is yielded.
30/08/16   tsk     CR1026504: Update to 0x5096 RxD log packet with additional details.
17/04/17   nm      CR2032010 make rf power class variable dual data spaced
19/10/16   rv      CR1068205: Fix for - GL1 is filling the info for sub1 when Gl1 API l1_log_signal_info is called for sub2
23/08/16   bg      CR1045621 Updating Rx Div Ant parameters in Engineering mode
30/06/16   sm      CR1035750 Added common event to logging all failure events.
23/02/16   sk      CR973190 Excessive F3 logging reduction
04/12/15   sjw     CR947229: code improvements to traffic msg logging
11/09/15   sjw     CR905858: avoid high volume of logging for MPH_ACQUISITION_STATE_IND
12/08/15   pa      CR887572: RxD: Schedule DRX wakeup after Rx/Tx and request TRM in task context. 
26/08/15   ws      CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
21/08/15   am      CR892913 Update WLAN with SACCH burst SNR in voice call
14/08/14   ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
22/06/15   mko     CR854924: Correting the Cxm logging issue, ChannelType seen as "Invalid" in SCH burst
27/05/15   msa     CR844425 Incorporate new diag services API for logging events with subscription id
28/05/15   akh     CR822523 (FR26957) Modem power tx stats - don't update tx stats twice
07/05/15   npt     CR821672 GERAN power logging update  
21/04/15   shr     CR806878 GPRS DSDS Power Control and WWCoex Power Info do not log the Tx Power in use
09/04/15   cgc     CR794901 deprecated MPH_SURROUND_WCDMA_DEDICATED_MEAS_IND
21/04/15   shr     CR806878 GPRS DSDS Power Control and WWCoex Power Info do not log the Tx Power in use
20/02/15   ab      CR773948 GL1 Support for provding Dumps and Metrics for QSH
24/03/15   npt     CR806839 Properly initialize version and padding fields in GERAN power logging packet  
12/03/15   pjr     CR802823 Ensure correct TS is printed in CXM logs
11/03/15   cs/tw   CR805703 Fix QTF builds with included array definition
04/03/15   br      CR763774 GL1 logging improvement to ease debugging
04/03/15   og      CR792350. Log packet 0x5A7C is not showing the correct ARFCN.
27/02/15   am      CR646584 WLAN coex
19/01/15   fm      CR780869 If a neighbor has the same ARFCN and BSIC as the serving one, GL1 checks if it works on the right TS
12/12/14   cs      CR754853 Add support for Parallel SCH detection handling
03/12/14   npt     CR759467 Fix compiler warnings
20/11/14   hd      CR760078 Add byte padding value to size parameter while calling log_alloc()
24/03/14   dg      CR626860 - New log packets by adding timeslot information
17/11/14   npt     CR642426 - Added power reporting log packet
29/10/14   ab      CR745429 - Featurization correction for PCHXFER for SS / CR merges.
14/10/14   br      CR732289 Change in TCH/HS bfi algorithm to consider prev hard rx_qual
19/08/14   pjr     CR711506 Support for AMR Partial Decode and DTX detection FR19667
17/09/14   pjr     CR725173 Added Chain ID and action type to CXM log packets
09/09/14   jk      CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14   nm      CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
14/04/14   pjr     CR641577 Replaced FN with Seq Number in cxm log table
11/07/14   pjr     CR692177 Added support for RFACCH 2.0
06/08/14   df      CR536059 Allow Asynchronous sleep for all idle activities
25/03/14   pjr     CR628670 Fix cxm log packets for async activities
18/06/14   cs/ch   CR651795 TRM Updated changes for DR-DSDS
17/04/14   br      CR647012  Exclude L1_DO_TIME_PWR_AVG and L1_FUNC_CALL_IN_TASK from logging
28/03/14   sjw     CR637346: Handle new_procedure flag in MPH_SET_PRIORITY_IND
07/03/14   pjr     CR623983 Fixed CXM logs for Acquisition messagges
06/02/14   cgc     CR598057 Exclude PH_DATA_IND and MPH_SURROUND_IRAT_MEAS_IND from logging
07/11/13   pjr     CR571992 modified the handling of gstmr info when logging in CXM log table
11/18/13   pjr     CR563534 Fix incorrect channel type shown in CXM logs packets
05/11/13   ws      CR571077 - Fixed regression with CR449651 with calling
                   gsmdiag_log_commit() instead of log_commit()
24/09/13   sp      CR 449651: Use new logPacket for Burst metrics.
04/10/13   pjr     CR536133 Removed unecessary debug message
04/10/13   nm      CR552803 tighten power scan threshold from -107dBm to -102dBm for background power scan only
27/09/13   sp      CR551580: Add NV logging of outgoing L1 messages
17/09/13   pjr     CR538893 Changes to enable CXM logging
02/07/13   pjr     CR507542 added data type and function for cxm logging
17/07/13   gk      CR514244: Remove NV logging of TRM commands
07/16/13   zc      CR470197 Replace memcpy with memscpy
28/11/12   br      CR383165 Included header file gl1_msgi.h to avoid compilation warnings
31/05/12   ab      CR 415136 RX and TX Time Power averaging update using msg router intf.
31/05/12   ab      CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
17/05/12   cgc     CR332796  add support for Tx power pa info api
03/05/12   rn      CR 330862 - Added version field to sacch burst
                   metrics and compact message metrics log packets.
13/03/12   ws      CR 330862 - Change dedicated mode F3 level's and add sacch burst
                   metrics and compact message metrics log packets
14/12/09   ws      Merged PACKED_POST changes from MUSTANG to resolve packed
                   struct alignment issues with loging
26/05/09   cs      CR167848 fix for expanded afc_freq I/F to mDSP
29/05/09   ps      Zero Initialise memory relocation to Heap for selected data structure
14/10/08   ap      CR160278 Serving cell ARFCN is reported incorrectly while frequency
                   hopping is being used.
02/09/08   cs      CR155904 remove setting of INVALID_BAND in HR to avoid l1_log errors
20/05/08   cs      Fix Klocwork array bounds check
01/05/07   dv      Changed RFACCH logging words from boolean to uint8
27/03/07   dv      Added FEATURE_GSM_R_FACCH
05/03/07   ws      Fixed filtered lint warnings
03/02/06   ws      Merged RVDS 2.1 fixes to casting of logging types
23/01/06   og      Fix qualifier type mismatch, removes remainder of Qlint
                   high level warnings.
09/07/05   pp      Initialize status variable in l1_log_message_metrics_amr()
                   before using it. Fixes AMR message metrics logging issue.
07/29/05   gfr     Support for NPL1 DTM interface changes
06/13/05   yh      Implementing qsort.
01/24/05   pp      Added support for AMR RATSCCH request/response event logging.
                   Changed l1_log_transmit_burst_metrics function
01/05/05   dp      Merged dedicated timeline changes for audio optimizations
                   to mainline. Lint cleanup
07/16/04   gfr     Moved ncell BCCH/SCH logging into this file.
06/25/04   gfr     Support for logging CBCH message metrics.
06/22/04   sv      Added L1 NV logging mechanism on Error Fatals.
05/26/04   sv      Lint Cleanup.
02/25/04   gw      Lint clean up.
01/30/04   gfr     Support for the firecode status bit
01/28/04   gw      Changes to support reselection optimizations.
11/05/03   gfr     Support for quad-band.
10/17/03   gfr     Increased MAX_PSCAN_LIST_SIZE from 124 to 220
05/12/03   ATM     Moved Frame Number management entirely inside GTMRS
05/07/03   gsc     Added AMR logging support.
04/10/03   dp      Added HR metrics logging
02/24/03   atm     Disabled logging L1_STATE event due to non-isr friendly
                   change made to in event logging
01/24/03   atm     Logging cleanup (Added TXLEV/TA Logging)
01/13/03   rrr/mk  Added suport for new message metrics w/ both hard and soft
                   RxQual.
12/04/02   ws      Updated GPRS burst metrics log packet def's
11/25/02   mk      Updated GPRS baseline.
11/25/02   ws      Added gprs agc log packets
11/18/02   mk      Updated GPRS baseline with release 2.0 functionality.
11/18/02   ATM     Bugfix: Use SUB_FN to ensure non-negative FN values
11/05/02   ws      Added gprs burst and message metrics
10/08/02   ATM     Relocated Frame Number Adjustment logging to GTMRS
10/01/02   ATM     Added central get and set FN routines
09/15/02   ATM     Added support for AGC info subcommand
07/15/02   ATM     Added FCCH/SCH decode packets (preliminary).
07/11/02   ATM     Added NCELL acq events.
07/11/02   gw      Changed references to power_dBm_x16 in metrics reports to
                   pwr_dBm_x16.
05/29/02   JC      Added BCCH message metrics support.
05/14/02   ATM     Added DTX_Indicator to Serving Cell Power Info packet
05/08/02   ATM     Split Rx_Metrics into Message/Burst packets
05/02/02   JC      Changed log_pscan() to use
                   l1_Frequency_List.num_above_sort_threshold_criteria
04/30/02   ATM     Fixed logical channel mapping.
04/30/02   ATM     Reversed store burst/msg and classed FACCH as Traffic
04/30/02   ATM     Added Serving Cell Power Info packet & Power Scan
04/19/02   ATM     Split recieve metrics into BURST/MESSAGE pieces
04/17/02   ATM     Changed "Status" to a composite GOOD_DATA/TAF/BFI
04/11/02   ATM     Adding LOG_GSM_RECEIVE_METRICS_C packet
04/03/02   ATM     TRUE vs true
04/02/02   ATM     BUGFIX - rx_power should be dBx16_T (sint15, not uint8)
03/29/02   ATM     Added NCELL packets
02/12/02   atm     Added LOG_ON_DEMAND functionality.
                   also Utilized the LOG_RECORD_DEFINE macros.
02/07/02   gw      Removed MSG_ERROR messages.
01/10/02   atm     Initial Revision.

===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "geran_variation.h"
#include "ms.h"
#include "l1_log.h"         // This module's header file
#include "log.h"            // For log_commit() and log_alloc() and log codes
#include "diag.h"           // Needed for DIAG_DIAGVER prior to calling event.h
#include "event.h"          // For logging RAR event and event codes
#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"  // For GSM log codes

#include "l1i.h"            // For TSKISR struct (for l1_state)
#include "l1_sc.h"          // for l1_sc_globals
#include "gl1_sys_params.h" // for SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16
#include <string.h>         // for memset
#include "gtmrs.h"
#include "gtmrs_g.h"        // For GSTMR_GET_FN_GERAN( gas_id) & SUB_FN()

#include "gpl1_gprs_log.h"  // for convert_txlev_to_pwr_dBm()
#include "l1_utils.h"
#include "gl1_msgi.h"
#include <stringl/stringl.h>

#include "l1_task.h"

#include "geran_eng_mode_info.h"
#include "l1_isr.h"

#include "geran_dual_sim.h"
#include "gl1_hw_g.h"
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
#include "vtmrs_g.h"
#endif
#include "gsmdiag.h"

extern volatile ISRTIM_CMD_BLK   l1_tsk_buffer[];  // For access to l1_state (in l1_task.c)
extern l1_serving_cell_meas_T   l1_serving_cell_meas[];


/*===========================================================================
                        TYPE DEFINITIONS FOR EVENTS
===========================================================================*/
#define LOG_GSM_RECEIVE_MSG_METRICS_RFACCH_C_VERSION 1
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#define GL1_QSH_TIM_ERROR_THRESHOLD 8
#define GL1_QSH_FREQ_ERROR_THRESHOLD 200
#define GL1_QSH_RXLEV_DIFF_PRI_DIV_THRESHOLD 240 /* (16 * 15 ) =  dbx16 * 15 =(240)5db */
#define GL1_QSH_DEBUG_BIG_TE_SNR_THRESHOLD 256 /* same as TT_SNR_THRESH */
#endif


LOG_RECORD_DEFINE(LOG_GSM_RECEIVE_MSG_METRICS_RFACCH_C)
    uint8                   version;
    uint32                  FN;
    uint8                   subID;
    GfwRfacchLogType        GfwRfacchLog;
LOG_RECORD_END


// -----------RECEIVE METRICS----------------
LOG_RECORD_DEFINE(LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C)
    uint32                  FN;
    byte                    chan;
    /* MESSAGE METRICS */
    uint16                  rx_qual;
    byte                    status;    /* SID:bit3,4/CRC:bit2/FireCode:bit1/BFI:bit0 */
    uint16                  msg_len;
    uint16                  rx_qual_s;
#ifdef FEATURE_GSM_AMR
    /* New for AMR */
    uint8                   codec_mode;
    uint16                  amr_status;
    uint8                   norm_c_over_i;
    uint8                   dl_acs;
    uint8                   ul_acs;
#endif /* FEATURE_GSM_AMR */
    uint8                   r_facch_storage_status;
    uint8                   r_facch_recomb_status;
LOG_RECORD_END

#define LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_VERSION 1

LOG_RECORD_DEFINE(LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C)
    uint8                   version;
    uint32                  FN;
    byte                    chan;
    /* MESSAGE METRICS */
    uint16                  rx_qual;
    byte                    status;    /* SID:bit3,4/CRC:bit2/FireCode:bit1/BFI:bit0 */
    uint16                  msg_len;
    uint16                  rx_qual_s;
#ifdef FEATURE_GSM_AMR
    /* New for AMR */
    uint8                   codec_mode;
#endif /* FEATURE_GSM_AMR */
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GSM_RECEIVE_BURST_METRICS_C)
    byte                    chan;
    /* BURST METRICS x 4 */
    l1_log_burst_metrics_T  burst[4];
LOG_RECORD_END

#define LOG_GSM_RECEIVE_BURST_METRICS_VERSION 4
LOG_RECORD_DEFINE(LOG_GSM_RECEIVE_BURST_METRICS_VER2_C)
    byte                    version;
    byte                    chan;
    /* BURST METRICS x 4 */
    l1_log_burst_metrics_v2_T  burst[4];
LOG_RECORD_END


#define LOG_GSM_RECEIVE_SACCH_BURST_METRICS_C_VERSION 1

LOG_RECORD_DEFINE(LOG_GSM_RECEIVE_SACCH_BURST_METRICS_C)
    uint8 version;
    /* BURST METRICS x 4 */
    l1_log_sacch_burst_metrics_T  burst[4];
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GSM_TRANSMIT_BURST_METRICS_C)
    byte               chan;
    byte               tsc;
    byte               ta;
    /* BURST METRICS x 4 */
    l1_log_transmit_burst_metrics_T  burst[4];
LOG_RECORD_END

#define LOG_GSM_TRANSMIT_BURST_METRICS_VERSION 1
LOG_RECORD_DEFINE(LOG_GSM_TRANSMIT_BURST_METRICS_VER1_C)
    uint8              version;
    byte               chan;
    byte               tsc;
    byte               ta;
    /* BURST METRICS x 4 */
    l1_log_transmit_burst_metrics_v1_T  burst[4];
LOG_RECORD_END

// -----------PAGING DATA----------------
LOG_RECORD_DEFINE(LOG_GSM_PAGING_DATA_C)
        byte Paging_Multiframe;
        byte Paging_Block_Index;
        byte Paging_Data_Valid;
        byte All_Paging_Blocks;
        byte Paging_Mode;
        byte CCCH_Group;
        byte Paging_Group;
        byte Pwr_Av_Freq;
LOG_RECORD_END

// -----------NEIGHBOR CELL MODE----------------
LOG_RECORD_DEFINE(LOG_GSM_NCELL_ENGINE_MODE_C)
        byte Mode;
LOG_RECORD_END

// -----------NEIGHBOR CELL LIST----------------
// Below is a subset of l1_sc_bs_T
typedef PACKED struct PACKED_POST {
    word    arfcn;
    dBx16_T rx_power;          /* last averaged power level */
    boolean bsic_known;
    byte    bsic;              /* only valid if bsic_known == TRUE */
    dword   fn_lag;            /* only valid if bsic_known == TRUE */
    int16   qbit_lag;          /* only valid if bsic_known == TRUE */
} l1_log_ncell_info_T;

#define MAX_NCELL_LIST_SIZE  32

LOG_RECORD_DEFINE(LOG_GSM_NCELL_BA_LIST_C)
  byte                Cell_Count;
  l1_log_ncell_info_T Cell[MAX_NCELL_LIST_SIZE];
LOG_RECORD_END

// -----------NEIGHBOR CELL ACQ----------------
LOG_RECORD_DEFINE(LOG_GSM_NCELL_ACQ_C)
    byte    mode;
    uint8   rank;
    word    arfcn;
    dBx16_T rx_power;  /* Only relavent for START/LOST */
LOG_RECORD_END

// -----------SERVING CELL INFO----------------
LOG_RECORD_DEFINE(LOG_GSM_SERVING_CELL_POWER_C)
    word    arfcn;
    dBx16_T rx_power;
    byte    tx_lev;
    byte    ta;
    boolean dtx;
LOG_RECORD_END

// -----------POWER SCAN LIST----------------
typedef PACKED struct PACKED_POST {
    word    arfcn;
    dBx16_T power;
} l1_log_pscan_info_T;

#define MAX_PSCAN_LIST_SIZE  220

LOG_RECORD_DEFINE(LOG_GSM_POWER_SCAN_C)
  word                count;
  dBx16_T             threshold;
  word                list_size;
  l1_log_pscan_info_T freq_list[MAX_PSCAN_LIST_SIZE];
LOG_RECORD_END

// -----------TXLEV/TA MODE----------------
LOG_RECORD_DEFINE(LOG_GSM_TXLEV_TA_C)
        word arfcn;
        byte TxLev;
        byte TA;
LOG_RECORD_END

// ---------AUXILIARY MEASUEMENTS ---------

LOG_RECORD_DEFINE(LOG_GSM_SERVING_AUX_MEAS_C)
   dBx16_T    rx_power_instant;
   boolean    SNR_is_bad;
LOG_RECORD_END

typedef PACKED struct PACKED_POST {
   uint16  arfcn;
   dBx16_T rx_power_instant;
} l1_log_ncell_aux_meas_T;

LOG_RECORD_DEFINE(LOG_GSM_NCELL_AUX_MEAS_C)
   byte                    cell_count;
   l1_log_ncell_aux_meas_T ncells[MAX_NCELL_LIST_SIZE];
LOG_RECORD_END


/* -----------NCELL BCCH BURST METRICS---------------- */
LOG_RECORD_DEFINE(LOG_GSM_BCCH_BURST_METRICS_C)
    uint32              FN;
    word                arfcn;
    uint32              rssi;
    dBx16_T             pwr_dBm_x16;
    int16               dc_offset_i;
    int16               dc_offset_q;
    int16               freq_offset;
    int16               timing_offset;
    uint16              snr;
    byte                gain_state;
LOG_RECORD_END

/* -----------NCELL BCCH MSG METRICS---------------- */
LOG_RECORD_DEFINE(LOG_GSM_BCCH_MSG_METRICS_C)
    uint32                   FN;
    uint16                   arfcn;
    byte                     data[sizeof(gl1_defs_rx_hdr_struct)];
LOG_RECORD_END

/* -----------NCELL SCH BURST METRICS---------------- */
LOG_RECORD_DEFINE(LOG_GSM_SCH_BURST_METRICS_C)
    uint32              FN;
    word                arfcn;
    uint32              rssi;
    dBx16_T             pwr_dBm_x16;
    int16               dc_offset_i;
    int16               dc_offset_q;
    int16               freq_offset;
    int16               timing_offset;
    uint16              snr;
    byte                gain_state;
LOG_RECORD_END

/* -----------NCELL SCH MSG METRICS---------------- */
LOG_RECORD_DEFINE(LOG_GSM_SCH_MSG_METRICS_VER2_C)
    uint32                   FN;
    uint16                   arfcn;
    uint16                   crc;
    uint8                    data[MAX_SCH_OCTETS];
LOG_RECORD_END

/* -----------COEX MANAGER PARAMETERS---------------- */
LOG_RECORD_DEFINE(LOG_GSM_COEX_PARAMS_C)
  uint8                version;
  uint8                gas_id;
  uint32               FN;
  uint8                num_entries;
  l1_cxm_result_info_T cxm_result_info[10];
LOG_RECORD_END

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
typedef PACKED struct PACKED_POST
{
   uint32                  FN;
   uint8                   channel_type;
   uint8                   esaic_snr;
   uint32                  rssi;
   uint16                  snr;
   uint8                   arfcn_equal_bcch          : 1;
   uint8                   gl1_dtx_ind               : 1;
   uint8                   gl1_amr_pd_ind            : 1;
   uint8                   gfw_dtx_cxm_reg           : 1;
   uint8                   gfw_amr_pd_cxm_reg        : 1;
   uint8                   gfw_dtx_rx_cancel         : 1;
   uint8                   gfw_amr_pd_rx_cancel      : 1;
   uint8                   gfw_amr_pd_desense_status : 1;
} amr_pd_dtx_burst_metrics_t;

LOG_RECORD_DEFINE(LOG_GSM_AMR_PD_DTX_BURST_METRICS_C)
   uint8                      version;
   uint8                      gas_id;
   amr_pd_dtx_burst_metrics_t burst[4];
LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_GSM_AMR_PD_DTX_MESSAGE_METRICS_C)
   uint8                   version;
   uint8                   gas_id;
   uint32                  FN;
   uint8                   channel_type;
   uint8                   gfw_dtx_ind_at_decode                :1;
   uint8                   gfw_amr_pd_ind_at_decode             :1;
   uint8                   gfw_gl1_amr_pd_cancel_ind_at_decode  :1;
   uint16                  hard_rx_qual;
   uint16                  mean_snr;
   uint8                   codec_mode;
LOG_RECORD_END
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
/* -------------------------------------------------
   GERAN   GERAN physical layer power info    0x1879
   ------------------------------------------------- */
#define LOG_GERAN_PHYSICAL_LAYER_POWER_INFO_VERSION 0

LOG_RECORD_DEFINE(LOG_GERAN_PHYSICAL_LAYER_POWER_INFO_C)
  uint8      version;
  uint8      gas_id;
  uint8      padding[6];          /* Ensure byte alignment of ustmr */
  uint64     ustmr;               /* Time when first sample in packet was collected */
  uint8      num_active_dl_slots; /* Number of active DL slots over the past 20 frames.*/
  uint8      num_active_ul_slots; /* Number of active UL slots over the past 20 frames.*/
  uint8      aeq_status;          /* TRUE if AEQ is used during the 20-frame duration.*/
  uint8      power_monitor;       /* Count of power monitor measurements made during the past 20 frames.*/
  uint8      uplink_power;        /* Latest non-zero Tx power used, in 0.5dBm steps.*/
  uint8      rxd;                 /* TRUE if RxD is enabled any point over the past 20 frames.*/
LOG_RECORD_END

static l1_power_info_T l1_power_info[NUM_GERAN_DATA_SPACES];
#endif /* FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO */

/*===========================================================================
                         DATA DECLARATIONS
===========================================================================*/


// Used by UMTS_AGC_INFO_F subcommand
l1_log_burst_metrics_T l1_log_gsm_agc_info;

l1_log_transmit_burst_metrics_T l1_log_gsm_transmit_info;

typedef struct
{
  // Used for LOG_ON_DEMAND registry
  boolean l1_log_initialized;
} l1_log_data_t;

static l1_log_data_t l1_log_data = {0};

#ifdef FEATURE_GSM_LOG_COEX
static cxm_log_table_T cxm_log_table[NUM_GERAN_DATA_SPACES];
#endif
extern dedicated_data_T l1_dedicated_data[];

// used for logging L1 debug data into NVM
l1_NVLog_struct_T l1_nv_log[NUM_GERAN_DATA_SPACES];

/*===========================================================================
                         FUNCTION DECLARATIONS
===========================================================================*/
log_on_demand_status_enum_type l1_log_on_demand(log_code_type log_code);

#ifdef FEATURE_GSM_LOG_COEX
static int8 l1_log_cxm_get_entry_index(uint16 seq_num, int16 gstmr_start, int16 gstmr_stop,
                                       uint16 arfcn, uint8 activity, GfwChanType chan_type, gas_id_t gas_id);
#endif
/*===========================================================================
FUNCTION  arfcn_to_word

DESCRIPTION
  This function converts an arfcn structure into a word for logging.

RETURN VALUE
  uint16 arfcn:
  -Lower 12 bits represents the ARFCN
  -Upper 4 bits numerically represents the band
    0000 -> current 900/1800 setting if no quadband
    1000 -> GSM 900
    1001 -> DCS
    1010 -> PCS
    1011 -> GSM 850
    1100 -> GSM 450  (not supported)

SIDE EFFECTS
  None
===========================================================================*/
uint16 arfcn_to_word ( ARFCN_T arfcn )
{
   uint16 ret_arfcn;

   ret_arfcn = arfcn.num;

   switch (arfcn.band)
   {
      case SYS_BAND_PGSM_900:
      case SYS_BAND_EGSM_900:
         ret_arfcn |= 0x8000;
         break;
      case SYS_BAND_DCS_1800:
         ret_arfcn |= 0x9000;
         break;
      case SYS_BAND_PCS_1900:
         ret_arfcn |= 0xA000;
         break;
      case SYS_BAND_CELL_850:
         ret_arfcn |= 0xB000;
         break;
      case SYS_BAND_NONE:
         /* This is valid when we have cleared the metrics in HR ! */
         break;
      default:
         MSG_GERAN_ERROR_2("Invalid band %d for ARFCN %d",(int)arfcn.band,(int)ret_arfcn);
   }

   return ret_arfcn;
}

/*===========================================================================

FUNCTION  l1_log_packet_init

DESCRIPTION
  Initialises a gsmdiag_pkt_info structure ready for use with logging functions

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_packet_init(gsmdiag_pkt_info_t *info_ptr, log_code_type pkt_id, unsigned int pkt_size, gas_id_t gas_id)
{
  if (info_ptr != NULL)
  {
    info_ptr->pkt_id          = pkt_id;
    info_ptr->pkt_size        = pkt_size;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    info_ptr->dual_sim_active = TRUE;
    info_ptr->active_sub_id   = ( 1 + GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id) );
    info_ptr->pkt_version_id  = GSMDIAG_PKT_VERSION_DEFAULT;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  }

  return;
}

/*===========================================================================
FUNCTION  l1_log_serving_cell_power

DESCRIPTION
  This function should be called when L1 serving cell power needs to be logged

RETURN VALUE
  boolean - was the packet sent or not (used by LOG_ON_DEMAND)

SIDE EFFECTS
  None
===========================================================================*/
boolean  l1_log_serving_cell_power ( gas_id_t gas_id )
{
  gsmdiag_pkt_info_t                  pkt_info;
  LOG_GSM_SERVING_CELL_POWER_C_type*  log_ptr;    // pointer to log buffer


  switch( l1_tsk_buffer[gas_id].l1_state )
  {
  case L1_INACTIVE_MODE:
  case L1_NULL_MODE:
  case L1_SUSPEND_MODE:
  case L1_RESET_MODE:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  case L1_MULTI_SIM_INACTIVE:
  case L1_MULTI_SIM_NULL:
#endif
    /* Check the other gas id */
    if( GERAN_ACCESS_STRATUM_ID_1 == gas_id )
    {
      gas_id = GERAN_ACCESS_STRATUM_ID_2;
    }
    else
    {
      gas_id = GERAN_ACCESS_STRATUM_ID_1;
    }

      switch( l1_tsk_buffer[gas_id].l1_state )
      {
      case L1_INACTIVE_MODE:
      case L1_NULL_MODE:
      case L1_SUSPEND_MODE:
      case L1_RESET_MODE:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      case L1_MULTI_SIM_INACTIVE:
      case L1_MULTI_SIM_NULL:
#endif
        /* Both gas id inactive, abort */
        return  FALSE;
        break;

      default:

      break;
      }

    break;

    default:

      break;
  }


  l1_log_init(); // Self-initialize

  l1_log_packet_init( &pkt_info,
                      LOG_GSM_SERVING_CELL_POWER_C,
                      sizeof(LOG_GSM_SERVING_CELL_POWER_C_type),
                      gas_id
                    );

  /* Try to allocate memory for the log packet */
  log_ptr = (LOG_GSM_SERVING_CELL_POWER_C_type *)gsmdiag_log_alloc(&pkt_info);

  if ( NULL != log_ptr)
  {
    dBx16_T                  rx_power_average = 0;
    l1_serving_cell_meas_T*  l1_serving_cell_meas_ptr = &l1_serving_cell_meas[gas_id];

    // Fill packet and commit it
    if ( 0 != l1_serving_cell_meas_ptr->no_of_measurements )
    {
      rx_power_average   =      (dBx16_T)(l1_serving_cell_meas_ptr->rx_power / l1_serving_cell_meas_ptr->no_of_measurements);
    }

    log_ptr->rx_power  =       rx_power_average;

    if( l1_tsk_buffer[gas_id].l1_state < L1_RANDOM_ACCESS_MODE )
    {
      /* IDLE */
      idle_data_T*  l1_idle_data = &l1_idle_data_store[gas_id];

      log_ptr->arfcn     =       arfcn_to_word(l1_idle_data->campedon_cell_ARFCN);
      log_ptr->tx_lev    =       255;
      log_ptr->ta        =       255;
      log_ptr->dtx       =       255;
    }
    else
    {
      /* TRANSMITTING */
      dedicated_data_T*  l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

      log_ptr->arfcn     =       arfcn_to_word(gl1_get_serving_cell_arfcn(gas_id));
      log_ptr->tx_lev    =       l1_dedicated_data_ptr->ms_power_level_in_use & 0x1F;
      log_ptr->ta        =       l1_dedicated_data_ptr->timing_advance_in_use & 0x7F;
      log_ptr->dtx       =       l1_dedicated_data_ptr->channel_specification.DTX_indicator;
    }

    gsmdiag_log_commit(&pkt_info);

    return  TRUE;
  }
  else
  {
    MSG_GERAN_ERROR_0_G("NULL log_serving_cell_power ptr ");
  }

  return  FALSE;
}

/*===========================================================================

FUNCTION  l1_log_init

DESCRIPTION
  This function initializes the log_on_demand packets.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_init(void)
{
  /* Call this only once */
  if (FALSE == l1_log_data.l1_log_initialized)
  {
    l1_log_data.l1_log_initialized = TRUE;
    (void)log_on_demand_register( LOG_GSM_PAGING_DATA_C,        l1_log_on_demand );
    (void)log_on_demand_register( LOG_GSM_NCELL_ENGINE_MODE_C,  l1_log_on_demand );
    (void)log_on_demand_register( LOG_GSM_SERVING_CELL_POWER_C, l1_log_on_demand );
  }
  return;
}

/*===========================================================================

FUNCTION  l1_log_on_demand

DESCRIPTION
  This function calls relevant log packet routine when the DM demands it.

RETURN VALUE
  Status of the call (should always be good)

SIDE EFFECTS
  None

===========================================================================*/
log_on_demand_status_enum_type l1_log_on_demand(log_code_type log_code)
{
    gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
    boolean retVal =  FALSE;

    switch(log_code){
       case LOG_GSM_PAGING_DATA_C:
          retVal = l1_log_paging_data(gas_id);
          break;

       case LOG_GSM_NCELL_ENGINE_MODE_C:
          retVal = l1_log_ncell_engine_mode(gas_id);
          break;

       case LOG_GSM_SERVING_CELL_POWER_C:
          retVal = l1_log_serving_cell_power(gas_id);
          break;

       default:
          MSG_GERAN_ERROR_1_G("Unexpected log code %d", log_code);
          break;
    }/* end switch */

    if( retVal )
    {
        return LOG_ON_DEMAND_SENT_S;
    }

    return LOG_ON_DEMAND_FAILED_ATTEMPT_S;
}

/*===========================================================================
FUNCTION  l1_log_paging_data

DESCRIPTION
  This function should be called when L1 paging data is available.
  It will send a log packet to the DM.

RETURN VALUE
  boolean - was the packet sent or not (used by LOG_ON_DEMAND)

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_log_paging_data    ( gas_id_t gas_id )
{
    LOG_GSM_PAGING_DATA_C_type  *log_ptr;    // pointer to log buffer
    idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    gsmdiag_pkt_info_t pkt_info;

    l1_log_init(); // Self-initialize

    l1_log_packet_init(&pkt_info, LOG_GSM_PAGING_DATA_C, sizeof(LOG_GSM_PAGING_DATA_C_type), gas_id);
    log_ptr = (LOG_GSM_PAGING_DATA_C_type *)gsmdiag_log_alloc(&pkt_info);

    /* Try to allocate memory for the log packet */
    if ( NULL != log_ptr )
    {   // Fill packet and commit it
        log_ptr->Paging_Multiframe  =       l1_idle_data->paging_data.paging_multiframe;
        log_ptr->Paging_Block_Index =       l1_idle_data->paging_data.paging_block_index;
        log_ptr->Paging_Data_Valid  = (byte)l1_idle_data->paging_data.paging_data_valid;
        log_ptr->All_Paging_Blocks  = (byte)l1_idle_data->paging_data.all_paging_blocks;
        log_ptr->Paging_Mode        =       l1_idle_data->paging_data.paging_mode;
        log_ptr->CCCH_Group         =       l1_idle_data->paging_data.ccch_group;
        log_ptr->Paging_Group       =       l1_idle_data->paging_data.paging_group;
        log_ptr->Pwr_Av_Freq        =       l1_idle_data->paging_data.pwr_av_freq;
        gsmdiag_log_commit(&pkt_info);
        return TRUE;
    }//endif log_ptr
    return FALSE;
}

/*===========================================================================
FUNCTION  l1_log_ncell_engine_mode

DESCRIPTION
  This function should be called when L1 paging data is available.
  It will send a log packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_log_ncell_engine_mode    ( gas_id_t gas_id )
{
    LOG_GSM_NCELL_ENGINE_MODE_C_type  *log_ptr;    // pointer to log buffer
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    gsmdiag_pkt_info_t pkt_info;

    l1_log_init(); // Self-initialize

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_NCELL_ENGINE_MODE_C, sizeof(LOG_GSM_NCELL_ENGINE_MODE_C_type), gas_id);
    log_ptr = (LOG_GSM_NCELL_ENGINE_MODE_C_type *)gsmdiag_log_alloc(&pkt_info);


    if ( log_ptr != NULL )
    {   // Fill packet and commit it
        log_ptr->Mode = l1_sc_globals_ptr->mode;
        gsmdiag_log_commit(&pkt_info);

        return TRUE;
    }//endif log_ptr
    return FALSE;
}

/*===========================================================================
FUNCTION  l1_log_tx_lev_and_ta

DESCRIPTION
  This function should be called when L1
  It will send a log packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_log_tx_lev_and_ta ( byte tx_lev, byte ta , gas_id_t gas_id)
{
    LOG_GSM_TXLEV_TA_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    idle_data_T      *l1_idle_data          = &l1_idle_data_store[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_TXLEV_TA_C, sizeof(LOG_GSM_TXLEV_TA_C_type), gas_id);
    log_ptr = (LOG_GSM_TXLEV_TA_C_type *)gsmdiag_log_alloc(&pkt_info);


    if ( log_ptr != NULL )
    {   // Fill packet and commit it
        switch (l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].l1_state)
        {
           case L1_INACTIVE_MODE:
           case L1_NULL_MODE:
           case L1_FIND_BCCH:
           case L1_FIND_SCH:
           case L1_IDLE_MODE:
           case L1_GPRS_IDLE_MODE:
              /* IDLE */
              log_ptr->arfcn = arfcn_to_word(l1_idle_data->campedon_cell_ARFCN);
              break;

           case L1_RANDOM_ACCESS_MODE:
           case L1_PACKET_ACCESS_MODE:
           case L1_DEDICATED_MODE:
           case L1_TRANSFER_MODE:
           default:
              /* TRANSMITTING */
              log_ptr->arfcn = arfcn_to_word(l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list.channel[0]);
              break;
        }

        log_ptr->TxLev  =  tx_lev;
        log_ptr->TA     =  ta;
        gsmdiag_log_commit(&pkt_info);

    }//endif log_ptr
}

/*===========================================================================
FUNCTION  l1_log_ncell_ba_list

DESCRIPTION
  This function should be called when L1 paging data is available.
  It will send a log packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_log_ncell_ba_list    ( gas_id_t gas_id )
{
    LOG_GSM_NCELL_BA_LIST_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

    byte   n;
    uint32 log_size;
    byte   count   = l1_sc_globals_ptr->sorted_cell_count;

    // Limit cell count to MAX_NCELL_LIST_SIZE
    if (count > MAX_NCELL_LIST_SIZE)
    {
        count = MAX_NCELL_LIST_SIZE;
    }

    log_size = sizeof(LOG_GSM_NCELL_BA_LIST_C_type) - ((MAX_NCELL_LIST_SIZE - count)*sizeof(l1_log_ncell_info_T));

    l1_log_packet_init(&pkt_info, LOG_GSM_NCELL_BA_LIST_C, log_size, gas_id);
    log_ptr = (LOG_GSM_NCELL_BA_LIST_C_type *)gsmdiag_log_alloc(&pkt_info);

    /* Try to allocate memory for the log packet */
    if ( NULL != log_ptr )
    {
        byte  current_ts = 0;
        int16 ts_qbit = 0;
        int16 delta_qbit = 0;
        ARFCN_T serving_arfcn = {0};
        uint8 serving_bsic = 0;
        serving_arfcn = gl1_get_serving_cell_arfcn(gas_id);
        serving_bsic = gl1_get_serving_cell_BSIC(gas_id);
        current_ts = l1_get_current_timeslot(gas_id);

        // Fill packet and commit it
        log_ptr->Cell_Count  =  count;
        for(n=0;n<count;++n)
        {
            log_ptr->Cell[n].arfcn      = arfcn_to_word(l1_sc_globals_ptr->sorted_cells[n]->bs.arfcn);
            log_ptr->Cell[n].rx_power   = l1_sc_globals_ptr->sorted_cells[n]->bs.rx_power;
            log_ptr->Cell[n].bsic_known = l1_sc_globals_ptr->sorted_cells[n]->bs.bsic_known;
            log_ptr->Cell[n].bsic       = l1_sc_globals_ptr->sorted_cells[n]->bs.bsic;
            log_ptr->Cell[n].qbit_lag   = l1_sc_globals_ptr->sorted_cells[n]->bs.qbit_lag;
            log_ptr->Cell[n].fn_lag     = l1_sc_globals_ptr->sorted_cells[n]->bs.fn_lag;

            /* In China, NW usually insert the C0 of serving cell into BA list.
            Hence if a neighbor has the same ARFCN and BSIC as the serving cell,
            we can treat them as the same and check if MS works on the right
            TS by checking the relevant Qbit lag */
            if (ARFCNS_EQUAL(serving_arfcn, l1_sc_globals_ptr->sorted_cells[n]->bs.arfcn)
                && (l1_sc_globals_ptr->sorted_cells[n]->bs.bsic == serving_bsic))
            {
                ts_qbit = ((GL1_DEFS_SLOTS_IN_FRAME - current_ts) * QS_PER_SLOT) % QS_PER_FRAME;

                if (l1_sc_globals_ptr->sorted_cells[n]->bs.qbit_lag < 0)
                {
                    l1_sc_globals_ptr->sorted_cells[n]->bs.qbit_lag += QS_PER_FRAME;
                }

                if (ts_qbit >= l1_sc_globals_ptr->sorted_cells[n]->bs.qbit_lag)
                {
                    delta_qbit = ts_qbit - l1_sc_globals_ptr->sorted_cells[n]->bs.qbit_lag;
                }
                else
                {
                    delta_qbit = l1_sc_globals_ptr->sorted_cells[n]->bs.qbit_lag - ts_qbit;
                }

                /* if the actual qbit offset is far away from the expected one,
                   it implies MS works on the wrong TS */
                if (delta_qbit > 50)
                {
                  MSG_GERAN_ERROR_5("TS error may happen on ARFCN %u, BSIC %u, TS %u, expected Qbit lag %d, actual Qbit lag %d!",
                      serving_arfcn.num, serving_bsic, current_ts, ts_qbit, l1_sc_globals_ptr->sorted_cells[n]->bs.qbit_lag);
                }
            }
        }
        gsmdiag_log_commit(&pkt_info);
    }//endif log_ptr
}
/*===========================================================================
FUNCTION  l1_log_ncell_acq

DESCRIPTION
  This function should be called when L1 paging data is available.
  It will send a log packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_log_ncell_acq ( byte mode, uint8 rank, ARFCN_T arfcn, dBx16_T rx_power, gas_id_t gas_id )
{
    LOG_GSM_NCELL_ACQ_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_NCELL_ACQ_C, sizeof(LOG_GSM_NCELL_ACQ_C_type), gas_id);
    log_ptr = (LOG_GSM_NCELL_ACQ_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {   // Fill packet and commit it
        log_ptr->mode     =  mode;
        log_ptr->rank     =  rank;
        log_ptr->arfcn    =  arfcn_to_word(arfcn);
        log_ptr->rx_power =  rx_power;
        gsmdiag_log_commit(&pkt_info);
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  l1_log_sacch_burst_metrics

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void l1_log_sacch_burst_metrics (gl1_msg_metrics_rpt *rpt_ptr[],gas_id_t gas_id )
{
    LOG_GSM_RECEIVE_SACCH_BURST_METRICS_C_type  *log_ptr;    // pointer to log buffer
    int n;
    uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);

    for(n=0;n<4;++n)
    {
      /* burst level averaging */
      if(rpt_ptr[n]->valid )
      {
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_update_rx_pwr_avg(rpt_ptr[n]->pwr_dBm_x16,gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
        gpl1_update_snr_avg(rpt_ptr[n]->snr,gas_id);
#endif
      }
    }


    /* Try to allocate memory for the log packet */
    log_ptr = (LOG_GSM_RECEIVE_SACCH_BURST_METRICS_C_type  *)log_alloc( LOG_GSM_RECEIVE_SACCH_BURST_METRICS_C, sizeof(LOG_GSM_RECEIVE_SACCH_BURST_METRICS_C_type) );

    if ( log_ptr )
    {
        log_ptr->version = LOG_GSM_RECEIVE_SACCH_BURST_METRICS_C_VERSION;
        /* Fill packet and commit it */
        for(n=0;n<4;++n)
        {
            if ( rpt_ptr[n]->valid )
            {
              log_ptr->burst[n].FN              = SUB_FN( this_FN, rpt_ptr[n]->offset_to_sob );
              log_ptr->burst[n].arfcn           = arfcn_to_word(rpt_ptr[n]->arfcn);
              log_ptr->burst[n].pwr_dBm_x16     = rpt_ptr[n]->pwr_dBm_x16;
              /* Magnatude of dcI dcQ = I^2 + Q^2 */
              log_ptr->burst[n].dc_offset       = ((rpt_ptr[n]->dc_offset_i * rpt_ptr[n]->dc_offset_i ) + (rpt_ptr[n]->dc_offset_q * rpt_ptr[n]->dc_offset_q));
              log_ptr->burst[n].freq_offset     = rpt_ptr[n]->freq_offset;
              log_ptr->burst[n].timing_offset   = rpt_ptr[n]->timing_offset;
              log_ptr->burst[n].snr             = rpt_ptr[n]->snr;
              log_ptr->burst[n].gain_state      = (byte) rpt_ptr[n]->rx_gain_range;
            }
            else
            {
              log_ptr->burst[n].FN              = 0;
              log_ptr->burst[n].arfcn           = 0;
              log_ptr->burst[n].pwr_dBm_x16     = 0;
              log_ptr->burst[n].dc_offset       = 0;
              log_ptr->burst[n].freq_offset     = 0;
              log_ptr->burst[n].timing_offset   = 0;
              log_ptr->burst[n].snr             = 0;
              log_ptr->burst[n].gain_state      = 0;
            }
        }
        log_commit((void *)log_ptr);
    }

}

/*===========================================================================
FUNCTION  l1_log_burst_metrics

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void l1_log_burst_metrics ( gl1_msg_chan_type    channel,
                            gl1_msg_metrics_rpt *rpt_ptr[], gas_id_t gas_id )
{
    LOG_GSM_RECEIVE_BURST_METRICS_C_type  *log_ptr;    // pointer to log buffer
    int n;
    uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
    gsmdiag_pkt_info_t pkt_info;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    boolean big_fe = FALSE;
    boolean big_te = FALSE;
    boolean big_rxlev_dif_pri_div = FALSE;
    dBx16_T rxlev_dif_pri_div = GL1_INVALID_POWER_LEVEL;
#endif

    for(n=0;n<4;++n)
    {
      /* burst level averaging */
      if(rpt_ptr[n]->valid && !rpt_ptr[n]->yield )
      {
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_update_rx_pwr_avg(rpt_ptr[n]->pwr_dBm_x16,gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

      }
    }


    /* Save a copy for AGC_INFO regardless of log_mask */
    l1_log_gsm_agc_info.FN            = SUB_FN( this_FN, rpt_ptr[0]->offset_to_sob);
    l1_log_gsm_agc_info.arfcn         = arfcn_to_word(rpt_ptr[0]->arfcn);
    l1_log_gsm_agc_info.rssi          = rpt_ptr[0]->rssi;
    l1_log_gsm_agc_info.pwr_dBm_x16   = rpt_ptr[0]->pwr_dBm_x16;
    l1_log_gsm_agc_info.dc_offset_i   = rpt_ptr[0]->dc_offset_i;
    l1_log_gsm_agc_info.dc_offset_q   = rpt_ptr[0]->dc_offset_q;
    l1_log_gsm_agc_info.freq_offset   = rpt_ptr[0]->freq_offset;
    l1_log_gsm_agc_info.timing_offset = rpt_ptr[0]->timing_offset;
    l1_log_gsm_agc_info.snr           = rpt_ptr[0]->snr;
    l1_log_gsm_agc_info.gain_state    = (byte) rpt_ptr[0]->rx_gain_range;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_RECEIVE_BURST_METRICS_C, sizeof(LOG_GSM_RECEIVE_BURST_METRICS_C_type), gas_id);
    log_ptr = (LOG_GSM_RECEIVE_BURST_METRICS_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {   // Fill packet and commit it
        log_ptr->chan = channel;
        for(n=0;n<4;++n)
        {
            if ( rpt_ptr[n]->valid && !rpt_ptr[n]->yield)
            {
                log_ptr->burst[n].FN              = SUB_FN( this_FN, rpt_ptr[n]->offset_to_sob );
                log_ptr->burst[n].arfcn           = arfcn_to_word(rpt_ptr[n]->arfcn);
                log_ptr->burst[n].rssi            = rpt_ptr[n]->rssi;
                log_ptr->burst[n].pwr_dBm_x16     = rpt_ptr[n]->pwr_dBm_x16;
                log_ptr->burst[n].dc_offset_i     = rpt_ptr[n]->dc_offset_i;
                log_ptr->burst[n].dc_offset_q     = rpt_ptr[n]->dc_offset_q;
                log_ptr->burst[n].freq_offset     = rpt_ptr[n]->freq_offset;
                log_ptr->burst[n].timing_offset   = rpt_ptr[n]->timing_offset;
                log_ptr->burst[n].snr             = rpt_ptr[n]->snr;
                log_ptr->burst[n].gain_state      = (byte) rpt_ptr[n]->rx_gain_range;
            }
            else
            {
                log_ptr->burst[n].FN              = 0;
                log_ptr->burst[n].arfcn           = 0;
                log_ptr->burst[n].rssi            = 0;
                log_ptr->burst[n].pwr_dBm_x16     = 0;
                log_ptr->burst[n].dc_offset_i     = 0;
                log_ptr->burst[n].dc_offset_q     = 0;
                log_ptr->burst[n].freq_offset     = 0;
                log_ptr->burst[n].timing_offset   = 0;
                log_ptr->burst[n].snr             = 0;
                log_ptr->burst[n].gain_state      = 0;
            }
        }
        gsmdiag_log_commit(&pkt_info);
    }//endif log_ptr

    l1_log_burst_metrics_ver2(channel,rpt_ptr, gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    for(n=0;n<4;++n)
    {
      if (rpt_ptr[n]->valid && !rpt_ptr[n]->yield)
      {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
        if(gl1_get_IMRD_ctl_nv(gas_id))
        {
          if(rpt_ptr[n]->pwr_dBm_x16_divrx != GL1_INVALID_POWER_LEVEL)
          {
            if (rpt_ptr[n]->pwr_dBm_x16_divrx > rpt_ptr[n]->pwr_dBm_x16)
            {
              rxlev_dif_pri_div = rpt_ptr[n]->pwr_dBm_x16_divrx - rpt_ptr[n]->pwr_dBm_x16;
            }
            else
            {
              rxlev_dif_pri_div = rpt_ptr[n]->pwr_dBm_x16 - rpt_ptr[n]->pwr_dBm_x16_divrx;
            }

            if (rxlev_dif_pri_div > GL1_QSH_RXLEV_DIFF_PRI_DIV_THRESHOLD)
            {
              big_rxlev_dif_pri_div = TRUE;
            }   
          }
        }
#endif
      
        if ( (rpt_ptr[n]->snr > GL1_QSH_DEBUG_BIG_TE_SNR_THRESHOLD) &&
             ((rpt_ptr[n]->freq_offset > GL1_QSH_FREQ_ERROR_THRESHOLD) || (rpt_ptr[n]->freq_offset < -GL1_QSH_FREQ_ERROR_THRESHOLD)) 
           )
        {
          big_fe = TRUE;
        }

        if ( (rpt_ptr[n]->snr > GL1_QSH_DEBUG_BIG_TE_SNR_THRESHOLD) &&
             ((rpt_ptr[n]->timing_offset > GL1_QSH_TIM_ERROR_THRESHOLD) || (rpt_ptr[n]->timing_offset < -GL1_QSH_TIM_ERROR_THRESHOLD)) 
           )
        {
          big_te = TRUE;
        }

      }
    }

    if (big_te)
    {
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_RX_HIGH_TIMING_ERROR);
    }

    if (big_fe)
    {
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_RX_HIGH_FREQ_ERROR);
    }

    if (big_rxlev_dif_pri_div)
    {
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_HIGH_RXLEV_DIFF_PRI_DIV);
    }

#endif

}

/*===========================================================================
FUNCTION  l1_log_burst_metrics_ver2

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void l1_log_burst_metrics_ver2 ( gl1_msg_chan_type    channel,
                            gl1_msg_metrics_rpt *rpt_ptr[],
                            gas_id_t gas_id )
{
    LOG_GSM_RECEIVE_BURST_METRICS_VER2_C_type  *log_ptr;    // pointer to log buffer
    gsmdiag_pkt_info_t pkt_info;
    int n;
    uint32 this_FN = GSTMR_GET_FN_GERAN(gas_id);

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
    for(n=0;n<4;++n)
    {
      /* burst level averaging */
      if(rpt_ptr[n]->valid && !rpt_ptr[n]->yield)
      {
        gpl1_update_rx_pwr_avg(rpt_ptr[n]->pwr_dBm_x16,gas_id );
      }
    }
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

    /* Save a copy for AGC_INFO regardless of log_mask */
    l1_log_gsm_agc_info.FN            = SUB_FN( this_FN, rpt_ptr[0]->offset_to_sob);
    l1_log_gsm_agc_info.arfcn         = arfcn_to_word(rpt_ptr[0]->arfcn);
    l1_log_gsm_agc_info.rssi          = rpt_ptr[0]->rssi;
    l1_log_gsm_agc_info.pwr_dBm_x16   = rpt_ptr[0]->pwr_dBm_x16;
    l1_log_gsm_agc_info.dc_offset_i   = rpt_ptr[0]->dc_offset_i;
    l1_log_gsm_agc_info.dc_offset_q   = rpt_ptr[0]->dc_offset_q;
    l1_log_gsm_agc_info.freq_offset   = rpt_ptr[0]->freq_offset;
    l1_log_gsm_agc_info.timing_offset = rpt_ptr[0]->timing_offset;
    l1_log_gsm_agc_info.snr           = rpt_ptr[0]->snr;
    l1_log_gsm_agc_info.gain_state    = (byte) rpt_ptr[0]->rx_gain_range;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_RECEIVE_BURST_METRICS_VER2_C, sizeof(LOG_GSM_RECEIVE_BURST_METRICS_VER2_C_type), gas_id);
    log_ptr = (LOG_GSM_RECEIVE_BURST_METRICS_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {   // Fill packet and commit it
        log_ptr->version = LOG_GSM_RECEIVE_BURST_METRICS_VERSION;
        log_ptr->chan = channel;
        for(n=0;n<4;++n)
        {
            if ( rpt_ptr[n]->valid && !rpt_ptr[n]->yield)
            {
                log_ptr->burst[n].FN              = SUB_FN( this_FN, rpt_ptr[n]->offset_to_sob );
                log_ptr->burst[n].arfcn           = arfcn_to_word(rpt_ptr[n]->arfcn);
                log_ptr->burst[n].rssi            = rpt_ptr[n]->rssi;
                log_ptr->burst[n].pwr_dBm_x16     = rpt_ptr[n]->pwr_dBm_x16;
                log_ptr->burst[n].dc_offset_i     = rpt_ptr[n]->dc_offset_i;
                log_ptr->burst[n].dc_offset_q     = rpt_ptr[n]->dc_offset_q;
                log_ptr->burst[n].freq_offset     = rpt_ptr[n]->freq_offset;
                log_ptr->burst[n].timing_offset   = rpt_ptr[n]->timing_offset;
                log_ptr->burst[n].snr             = rpt_ptr[n]->snr;
                log_ptr->burst[n].gain_state      = (byte) rpt_ptr[n]->rx_gain_range;
                log_ptr->burst[n].aci_filter_used = (byte) rpt_ptr[n]->aci_filter_used;
                log_ptr->burst[n].reliabFactorQ16 = (uint32)rpt_ptr[n]->reliabFactorQ16;
                log_ptr->burst[n].timeslot        = ((gl1_defs_tn_type)l1_get_current_timeslot(gas_id));
                log_ptr->burst[n].jdet_reading    = rpt_ptr[n]->jdet_value;
                log_ptr->burst[n].WBEE            = 0;
                log_ptr->burst[n].HLinLLinState   = rpt_ptr[n]->SawlessHLLLState;
                log_ptr->burst[n].aqpsk_detected  = (byte) rpt_ptr[n]->aqpsk_detected;
            }
            else
            {
                log_ptr->burst[n].FN              = 0;
                log_ptr->burst[n].arfcn           = 0;
                log_ptr->burst[n].rssi            = 0;
                log_ptr->burst[n].pwr_dBm_x16     = 0;
                log_ptr->burst[n].dc_offset_i     = 0;
                log_ptr->burst[n].dc_offset_q     = 0;
                log_ptr->burst[n].freq_offset     = 0;
                log_ptr->burst[n].timing_offset   = 0;
                log_ptr->burst[n].snr             = 0;
                log_ptr->burst[n].gain_state      = 0;
                log_ptr->burst[n].aci_filter_used = 0;
                log_ptr->burst[n].reliabFactorQ16 = 0;
                log_ptr->burst[n].timeslot        = 0;
                log_ptr->burst[n].jdet_reading    = 0;
                log_ptr->burst[n].WBEE            = 0;
                log_ptr->burst[n].HLinLLinState   = 0;
                log_ptr->burst[n].aqpsk_detected  = 0;
            }
        }
        gsmdiag_log_commit(&pkt_info);
    }//endif log_ptr
}

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
/*===========================================================================
FUNCTION  l1_log_amr_pd_dtx_burst_metrics

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void l1_log_amr_pd_dtx_burst_metrics ( gl1_msg_chan_type      channel,
                                         gl1_defs_metrics_rpt  *rpt_ptr[],
                                         gas_id_t               gas_id )
{
  LOG_GSM_AMR_PD_DTX_BURST_METRICS_C_type  *log_ptr;    // pointer to log buffer
  uint32 i;

  uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
  gsmdiag_pkt_info_t pkt_info;

  uint16 serving_cell_arcfn = l1_idle_data_store[gas_id].campedon_cell_ARFCN.num;

  /* Try to allocate memory for the log packet */
  l1_log_packet_init(&pkt_info, LOG_GSM_AMR_PD_DTX_BURST_METRICS_C, sizeof(LOG_GSM_AMR_PD_DTX_BURST_METRICS_C_type), gas_id);
  log_ptr = (LOG_GSM_AMR_PD_DTX_BURST_METRICS_C_type *)gsmdiag_log_alloc(&pkt_info);

  if ( log_ptr )
  {
    for (i = 0; i < 4; i++)
    {
      log_ptr->version                            = 1;
      log_ptr->gas_id                             = gas_id;
      log_ptr->burst[i].FN                        = SUB_FN( this_FN, rpt_ptr[i]->offset_to_sob );
      log_ptr->burst[i].channel_type              = (uint8)channel;
      log_ptr->burst[i].esaic_snr                 = rpt_ptr[i]->esaic_metrics;
      log_ptr->burst[i].rssi                      = rpt_ptr[i]->rssi;
      log_ptr->burst[i].snr                       = rpt_ptr[i]->snr;
      log_ptr->burst[i].arfcn_equal_bcch          = rpt_ptr[i]->arfcn.num == serving_cell_arcfn;
      log_ptr->burst[i].gl1_dtx_ind               = gl1_msg_dtx_detection_get_status(gas_id) ? 1 : 0;
      log_ptr->burst[i].gl1_amr_pd_ind            = gl1_msg_amr_pd_get_status(gas_id)        ? 1 : 0;
      log_ptr->burst[i].gfw_dtx_cxm_reg           = rpt_ptr[i]->dtx_cxm_reg          ? 1 : 0;
      log_ptr->burst[i].gfw_amr_pd_cxm_reg        = rpt_ptr[i]->amr_pd_cxm_reg       ? 1 : 0;
      log_ptr->burst[i].gfw_dtx_rx_cancel         = rpt_ptr[i]->dtx_rx_cancel        ? 1 : 0;
      log_ptr->burst[i].gfw_amr_pd_rx_cancel      = rpt_ptr[i]->amr_pd_rx_cancel     ? 1 : 0;
      log_ptr->burst[i].gfw_amr_pd_desense_status = rpt_ptr[i]->amr_pd_desense       ? 1 : 0;
    }

    gsmdiag_log_commit(&pkt_info);
  }
}
/*===========================================================================
FUNCTION  l1_log_amr_pd_dtx_message_metrics

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.
RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void l1_log_amr_pd_dtx_message_metrics ( gl1_msg_decode_rpt *rpt_ptr,
                                         uint8              codec_mode,
                                         gas_id_t           gas_id )
{
  LOG_GSM_AMR_PD_DTX_MESSAGE_METRICS_C_type  *log_ptr;    // pointer to log buffer

  uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
  gsmdiag_pkt_info_t pkt_info;

  /* Try to allocate memory for the log packet */
  l1_log_packet_init(&pkt_info, LOG_GSM_AMR_PD_DTX_MESSAGE_METRICS_C, sizeof(LOG_GSM_AMR_PD_DTX_MESSAGE_METRICS_C_type), gas_id);
  log_ptr = (LOG_GSM_AMR_PD_DTX_MESSAGE_METRICS_C_type *)gsmdiag_log_alloc(&pkt_info);

  if ( log_ptr )
  {
    log_ptr->version                             = 1;
    log_ptr->gas_id                              = gas_id;
    log_ptr->FN                                  =  SUB_FN( this_FN, rpt_ptr->tch_facch.offset_to_sob );
    log_ptr->channel_type                        = (byte)rpt_ptr->tch_facch.hdr.chan;
    log_ptr->gfw_dtx_ind_at_decode               = rpt_ptr->tch_facch.hdr.gfw_dtx_ind_dec        ? 1 : 0;
    log_ptr->gfw_amr_pd_ind_at_decode            = rpt_ptr->tch_facch.hdr.gfw_amr_pd_ind_dec     ? 1 : 0;
    log_ptr->gfw_gl1_amr_pd_cancel_ind_at_decode = rpt_ptr->tch_facch.hdr.gfw_gl1_disable_amr_pd ? 1 : 0;
    log_ptr->hard_rx_qual                        = rpt_ptr->tch_facch.hdr.rx_qual;
    log_ptr->mean_snr                            = 0;
    log_ptr->codec_mode                          = codec_mode;

    gsmdiag_log_commit(&pkt_info);
  }
}
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

/*===========================================================================
FUNCTION  l1_log_transmit_burst_metrics

DESCRIPTION
  This function should be called when transmit_burst metrics are available
  It will send a log packet.

  channel       - channel type. Eg. GL1_MSG_RACH, Gl1_MSG_SACCH, etc
  fn_offset     - no. of frames after the current frame, when the bursts will
                  start transmitting
  burst_spacing - no. of frames between consecutive transmit bursts. Eg. for
                  SACCH during dedicated mode TCH = 26, for TCH/F = 1, TCH/H = 2
  num_bursts    - total no. of bursts being transmitted. Eg. for SACCH during
                  dedicated mode TCH = 4
  arfcn         - arfcns corresponding to the bursts
  txlev         - transmit level for the bursts
  tsc           - training sequecne code for the bursts
  ta            - timing advance for the bursts

RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void l1_log_transmit_burst_metrics ( gl1_msg_chan_type    channel,
                                     uint8 fn_offset,
                                     uint8 burst_spacing,
                                     uint8 num_bursts,
                                     ARFCN_T arfcn[4],
                                     int16 txlev[4],
                                     uint8  tsc,
                                     uint8  ta ,
                                     gas_id_t gas_id)
{
    LOG_GSM_TRANSMIT_BURST_METRICS_C_type  *log_ptr;   // pointer to log buffer
    uint8 n;
    uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
    gsmdiag_pkt_info_t pkt_info;
    int16 tmp_txlev[4];

    for(n=0;n<num_bursts;++n)
    {
      tmp_txlev[n] = txlev[n];
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
      gpl1_update_tx_pwr_avg(&tmp_txlev[n],arfcn[n].band, GL1_DEFS_CS1_CODING, gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
    }

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_TRANSMIT_BURST_METRICS_C, sizeof(LOG_GSM_TRANSMIT_BURST_METRICS_C_type), gas_id);
    log_ptr = (LOG_GSM_TRANSMIT_BURST_METRICS_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {   // Fill packet and commit it
        log_ptr->chan = channel;
        log_ptr->tsc  = tsc;
        log_ptr->ta   = ta;

        for(n=0;n<num_bursts;++n)
        {
           log_ptr->burst[n].FN              = ADD_FN( this_FN, (fn_offset + n * burst_spacing ) );
           log_ptr->burst[n].arfcn           = arfcn_to_word(arfcn[n]);
           log_ptr->burst[n].txlev           = tmp_txlev[n];
        }

        for(n=num_bursts;n<4;++n)
        {
           log_ptr->burst[n].FN              = 0;
           log_ptr->burst[n].arfcn           = 0;
           log_ptr->burst[n].txlev           = 0;
        }
        gsmdiag_log_commit(&pkt_info);
    }//endif log_ptr

    l1_log_transmit_burst_metrics_ver1(channel,
                                     fn_offset,
                                     burst_spacing,
                                     num_bursts,
                                     arfcn,
                                     tmp_txlev,
                                     tsc,
                                     ta,
                                     gas_id);
}

/*===========================================================================
FUNCTION  l1_log_transmit_burst_metrics_ver1

DESCRIPTION
  This function should be called when transmit_burst metrics are available
  It will send a log packet.

  channel       - channel type. Eg. GL1_MSG_RACH, Gl1_MSG_SACCH, etc
  fn_offset     - no. of frames after the current frame, when the bursts will
                  start transmitting
  burst_spacing - no. of frames between consecutive transmit bursts. Eg. for
                  SACCH during dedicated mode TCH = 26, for TCH/F = 1, TCH/H = 2
  num_bursts    - total no. of bursts being transmitted. Eg. for SACCH during
                  dedicated mode TCH = 4
  arfcn         - arfcns corresponding to the bursts
  txlev         - transmit level for the bursts
  tsc           - training sequecne code for the bursts
  ta            - timing advance for the bursts

RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void l1_log_transmit_burst_metrics_ver1 ( gl1_msg_chan_type    channel,
                                     uint8 fn_offset,
                                     uint8 burst_spacing,
                                     uint8 num_bursts,
                                     ARFCN_T arfcn[4],
                                     int16 txlev[4],
                                     uint8  tsc,
                                     uint8  ta ,
                                     gas_id_t gas_id)
{
    LOG_GSM_TRANSMIT_BURST_METRICS_VER1_C_type  *log_ptr;   // pointer to log buffer
    uint8 n;
    uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
    gsmdiag_pkt_info_t pkt_info;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_TRANSMIT_BURST_METRICS_VER1_C, sizeof(LOG_GSM_TRANSMIT_BURST_METRICS_VER1_C_type), gas_id);
    log_ptr = (LOG_GSM_TRANSMIT_BURST_METRICS_VER1_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr )
    {   // Fill packet and commit it
        log_ptr->chan = channel;
        log_ptr->tsc  = tsc;
        log_ptr->ta   = ta;

        for(n=0;n<num_bursts;++n)
        {
           log_ptr->burst[n].FN              = ADD_FN( this_FN, (fn_offset + n * burst_spacing ) );
           log_ptr->burst[n].arfcn           = arfcn_to_word(arfcn[n]);
           log_ptr->burst[n].txlev           = txlev[n];
           log_ptr->burst[n].TN              = ((gl1_defs_tn_type)l1_get_current_timeslot(gas_id));
        }

        for(n=num_bursts;n<4;++n)
        {
           log_ptr->burst[n].FN              = 0;
           log_ptr->burst[n].arfcn           = 0;
           log_ptr->burst[n].txlev           = 0;
           log_ptr->burst[n].TN              = 0;
        }
        gsmdiag_log_commit(&pkt_info);
    }//endif log_ptr

}

/*===========================================================================
FUNCTION  l1_log_message_metrics_compact

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  l1_log_message_metrics_compact  ( gl1_msg_chan_type   channel,
                                        gl1_msg_decode_rpt *rpt_ptr, gas_id_t gas_id )
{
    LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_type  *log_ptr;    // pointer to log buffer
    uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);

    /* Sanity check */
    if ( !rpt_ptr ) return;

    /* Try to allocate memory for the log packet */
    log_ptr = (LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_type  *)log_alloc( LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C, sizeof(LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_type) );
    if (log_ptr == NULL) return;

    log_ptr->version = LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_VERSION;
    log_ptr->chan    =  channel;
    log_ptr->status  =  0;

    /* NOTE: CHAN seems to tell how to decode the next three */
    if ( GL1_MSG_CCCH == channel ||
         GL1_MSG_BCCH == channel ||
         GL1_MSG_CBCH == channel )
    {
        log_ptr->chan    =  (byte)rpt_ptr->ccch.hdr.chan;
        log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->ccch.offset_to_sob );
        log_ptr->rx_qual =  rpt_ptr->ccch.hdr.rx_qual;
        log_ptr->rx_qual_s = rpt_ptr->ccch.hdr.rx_qual_s;
        if (rpt_ptr->ccch.hdr.good_data)  /* CRC */
        {
            log_ptr->status |= 0x4; // bit 2
        }
        if (rpt_ptr->ccch.hdr.firecode_used)
        {
            log_ptr->status |= 0x2; // bit 1
        }

        if (rpt_ptr->ccch.hdr.bfi)
        {
            log_ptr->status |= 0x1; // bit 0
        }
        log_ptr->msg_len =  rpt_ptr->ccch.hdr.len;
    }
    else if ( GL1_MSG_TCH   == channel  ||
              GL1_MSG_TCH_HS == channel ||
              GL1_MSG_FACCH == channel )
    {
        log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->tch_facch.offset_to_sob );
        log_ptr->chan    =  (byte)rpt_ptr->tch_facch.hdr.chan;
        log_ptr->rx_qual =  rpt_ptr->tch_facch.hdr.rx_qual;
        log_ptr->rx_qual_s = rpt_ptr->tch_facch.hdr.rx_qual_s;
        if (rpt_ptr->tch_facch.hdr.good_data)
        {
            log_ptr->status |= 0x4; // bit 2
        }
        if (rpt_ptr->tch_facch.hdr.firecode_used)
        {
            log_ptr->status |= 0x2; // bit 1
        }

        log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.sid << 3 ); //bit 3,4

        if (rpt_ptr->tch_facch.hdr.ufi)
        {
           log_ptr->status |= 0x20; // bit 5
        }

        log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.voicing_mode << 6 ); //bit 6,7

        if (rpt_ptr->tch_facch.hdr.bfi)
        {
            log_ptr->status |= 0x1; // bit 0
        }
        log_ptr->msg_len =  rpt_ptr->tch_facch.hdr.len;

    }
    else if ( GL1_MSG_SACCH == channel ||
              GL1_MSG_SDCCH == channel  )
    {
        log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->cch.offset_to_sob );
        log_ptr->rx_qual =  rpt_ptr->cch.hdr.rx_qual;
        log_ptr->rx_qual_s = rpt_ptr->cch.hdr.rx_qual_s;
        if (rpt_ptr->cch.hdr.good_data)
        {
            log_ptr->status |= 0x4; // bit 2
        }
        if (rpt_ptr->cch.hdr.firecode_used)
        {
            log_ptr->status |= 0x2; // bit 1
        }

        if (rpt_ptr->cch.hdr.bfi)
        {
            log_ptr->status |= 0x1; // bit 0
        }
        log_ptr->msg_len =  rpt_ptr->cch.hdr.len;
    }
    else
    {
        log_ptr->chan    =  (byte)channel;
        log_ptr->rx_qual =  0;
        log_ptr->status  =  0;
        log_ptr->msg_len =  0;
        log_ptr->rx_qual_s = 0;
        log_ptr->FN      =  0;
    }
#ifdef FEATURE_GSM_AMR
    /* Set these to N/A */
    log_ptr->codec_mode = 0xFF;
#endif

    log_commit((void *)log_ptr);
}

/*===========================================================================
FUNCTION  l1_log_message_metrics

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void    l1_log_message_metrics  ( gl1_msg_chan_type   channel,
                                  gl1_msg_decode_rpt *rpt_ptr, gas_id_t gas_id )
{
    LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C_type  *log_ptr;    // pointer to log buffer
    uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
    gsmdiag_pkt_info_t pkt_info;

    /* Sanity check */
    if ( !rpt_ptr ) return;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C, sizeof(LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C_type), gas_id);
    log_ptr = (LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C_type *)gsmdiag_log_alloc(&pkt_info);


    if (log_ptr == NULL) return;

    log_ptr->chan    =  channel;
    log_ptr->status  =  0;

    /* NOTE: CHAN seems to tell how to decode the next three */
    if ( GL1_MSG_CCCH == channel ||
         GL1_MSG_BCCH == channel ||
         GL1_MSG_CBCH == channel )
    {
        log_ptr->chan    =  (byte)rpt_ptr->ccch.hdr.chan;
        log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->ccch.offset_to_sob );
        log_ptr->rx_qual =  rpt_ptr->ccch.hdr.rx_qual;
        log_ptr->rx_qual_s = rpt_ptr->ccch.hdr.rx_qual_s;
        if (rpt_ptr->ccch.hdr.good_data)  /* CRC */
        {
            log_ptr->status |= 0x4; // bit 2
        }
        if (rpt_ptr->ccch.hdr.firecode_used)
        {
            log_ptr->status |= 0x2; // bit 1
        }

        if (rpt_ptr->ccch.hdr.bfi)
        {
            log_ptr->status |= 0x1; // bit 0
        }
        log_ptr->msg_len =  rpt_ptr->ccch.hdr.len;
    }
    else if ( GL1_MSG_TCH    == channel  ||
              GL1_MSG_TCH_HS == channel  ||
              GL1_MSG_FACCH  == channel )
    {
        log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->tch_facch.offset_to_sob );
        log_ptr->chan    =  (byte)rpt_ptr->tch_facch.hdr.chan;
        log_ptr->rx_qual =  rpt_ptr->tch_facch.hdr.rx_qual;
        log_ptr->rx_qual_s = rpt_ptr->tch_facch.hdr.rx_qual_s;
        if (rpt_ptr->tch_facch.hdr.good_data)
        {
            log_ptr->status |= 0x4; // bit 2
        }
        if (rpt_ptr->tch_facch.hdr.firecode_used)
        {
            log_ptr->status |= 0x2; // bit 1
        }

        log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.sid << 3 ); //bit 3,4

        if (rpt_ptr->tch_facch.hdr.ufi)
        {
           log_ptr->status |= 0x20; // bit 5
        }

        log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.voicing_mode << 6 ); //bit 6,7

        if (rpt_ptr->tch_facch.hdr.bfi)
        {
            log_ptr->status |= 0x1; // bit 0
        }
        log_ptr->msg_len =  rpt_ptr->tch_facch.hdr.len;
        log_ptr->r_facch_storage_status  = rpt_ptr->tch_facch.hdr.rfacch_store_status;
        log_ptr->r_facch_recomb_status   = rpt_ptr->tch_facch.hdr.rfacch_comb_status;
    }
    else if ( GL1_MSG_SACCH == channel ||
              GL1_MSG_SDCCH == channel  )
    {
        log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->cch.offset_to_sob );
        log_ptr->rx_qual =  rpt_ptr->cch.hdr.rx_qual;
        log_ptr->rx_qual_s = rpt_ptr->cch.hdr.rx_qual_s;
        if (rpt_ptr->cch.hdr.good_data)
        {
            log_ptr->status |= 0x4; // bit 2
        }
        if (rpt_ptr->cch.hdr.firecode_used)
        {
            log_ptr->status |= 0x2; // bit 1
        }

        if (rpt_ptr->cch.hdr.bfi)
        {
            log_ptr->status |= 0x1; // bit 0
        }
        log_ptr->msg_len =  rpt_ptr->cch.hdr.len;
    }
    else
    {
        log_ptr->chan    =  (byte)channel;
        log_ptr->rx_qual =  0;
        log_ptr->status  =  0;
        log_ptr->msg_len =  0;
        log_ptr->rx_qual_s = 0;
        log_ptr->FN      =  0;
    }

#ifdef FEATURE_GSM_AMR
    /* Set these to N/A */
    log_ptr->codec_mode = 0xFF;
    log_ptr->amr_status = 0xFFFF;
    log_ptr->dl_acs     = 0xFF;
    log_ptr->ul_acs     = 0xFF;
    log_ptr->norm_c_over_i = 0xFF;
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
    gl1_hw_set_HardRxQual(log_ptr->rx_qual, gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY */

    gsmdiag_log_commit(&pkt_info);
}

/*===========================================================================
FUNCTION  l1_log_rfacch_metrics

DESCRIPTION
  This function should be called when rfacch metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void l1_log_rfacch_metrics(gl1_msg_decode_rpt *rpt_ptr, gas_id_t gas_id )
{
  LOG_GSM_RECEIVE_MSG_METRICS_RFACCH_C_type  *log_ptr;    // pointer to log buffer
    uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
    gsmdiag_pkt_info_t pkt_info;

    /* Sanity check */
    if ( !rpt_ptr ) return;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_RECEIVE_MSG_METRICS_RFACCH_C, sizeof(LOG_GSM_RECEIVE_MSG_METRICS_RFACCH_C_type), gas_id);
    log_ptr = (LOG_GSM_RECEIVE_MSG_METRICS_RFACCH_C_type *)gsmdiag_log_alloc(&pkt_info);


    if (log_ptr == NULL) return;

    log_ptr->version    =  LOG_GSM_RECEIVE_MSG_METRICS_RFACCH_C_VERSION;
    log_ptr->subID = (uint8)gas_id;
    log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->tch_facch.offset_to_sob );
    log_ptr->GfwRfacchLog = rpt_ptr->tch_facch.hdr.rfacchLogBuff;
    gsmdiag_log_commit(&pkt_info);
}


#ifdef FEATURE_GSM_AMR

/*===========================================================================
FUNCTION  l1_log_message_metrics_amr

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  l1_log_message_metrics_amr  (gl1_msg_decode_rpt   *rpt_ptr,
           uint8                codec_mode,
           uint16               amr_status,
           uint8                dl_acs,
           uint8                ul_acs,
           uint16               norm_c_over_i, gas_id_t gas_id)
{
  LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C_type  *log_ptr;
  uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);
  gsmdiag_pkt_info_t pkt_info;

  if ( !rpt_ptr )
  {
      return;
  }

  /* Try to allocate memory for the log packet */
  l1_log_packet_init(&pkt_info, LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C, sizeof(LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C_type), gas_id);
  log_ptr = (LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C_type *)gsmdiag_log_alloc(&pkt_info);

  if ( log_ptr )
  {
      log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->tch_facch.offset_to_sob );
      log_ptr->chan    =  (byte)rpt_ptr->tch_facch.hdr.chan;
      log_ptr->rx_qual =  rpt_ptr->tch_facch.hdr.rx_qual;
      log_ptr->rx_qual_s = rpt_ptr->tch_facch.hdr.rx_qual_s;

      log_ptr->status = 0;

      if (rpt_ptr->tch_facch.hdr.good_data)
      {
          log_ptr->status |= 0x4; // bit 2
      }
      if (rpt_ptr->tch_facch.hdr.firecode_used)
      {
          log_ptr->status |= 0x2; // bit 1
      }

      if (rpt_ptr->tch_facch.hdr.ufi)
     {
         log_ptr->status |= 0x20; // bit 5
     }

      log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.sid << 3 ); //bit 3,4
      log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.voicing_mode << 6 ); //bit 6,7

      if (rpt_ptr->tch_facch.hdr.bfi)
     {
         log_ptr->status |= 0x1; // bit 0
     }

      log_ptr->msg_len =  rpt_ptr->tch_facch.hdr.len;

      log_ptr->codec_mode = codec_mode;

      log_ptr->amr_status = amr_status;

      log_ptr->dl_acs     = dl_acs;

      log_ptr->ul_acs     = ul_acs;

      log_ptr->norm_c_over_i = (uint8) norm_c_over_i;
      log_ptr->r_facch_storage_status  = rpt_ptr->tch_facch.hdr.rfacch_store_status;
      log_ptr->r_facch_recomb_status   = rpt_ptr->tch_facch.hdr.rfacch_comb_status;

     gsmdiag_log_commit(&pkt_info);
  }
}

/*===========================================================================
FUNCTION  l1_log_message_metrics_compact_amr

DESCRIPTION
  This function should be called when message metrics are available
  It will send a log packet.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  l1_log_message_metrics_compact_amr  (gl1_msg_decode_rpt   *rpt_ptr,
                                   uint8                codec_mode, gas_id_t gas_id
                                  )
{
  LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_type  *log_ptr;
  gsmdiag_pkt_info_t pkt_info;
  uint32 this_FN = GSTMR_GET_FN_GERAN( gas_id);

  if ( !rpt_ptr )
  {
      return;
  }

  /* Try to allocate memory for the log packet */
  l1_log_packet_init(&pkt_info, LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C, sizeof(LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_type), gas_id);
  log_ptr = (LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_type *)gsmdiag_log_alloc(&pkt_info);

  if ( log_ptr )
  {
      log_ptr->version = LOG_GSM_RECEIVE_MSG_METRICS_COMPACT_C_VERSION;
      log_ptr->FN      =  SUB_FN( this_FN, rpt_ptr->tch_facch.offset_to_sob );
      log_ptr->chan    =  (byte)rpt_ptr->tch_facch.hdr.chan;
      log_ptr->rx_qual =  rpt_ptr->tch_facch.hdr.rx_qual;
      log_ptr->rx_qual_s = rpt_ptr->tch_facch.hdr.rx_qual_s;

      log_ptr->status = 0;

      if (rpt_ptr->tch_facch.hdr.good_data)
      {
          log_ptr->status |= 0x4; // bit 2
      }
      if (rpt_ptr->tch_facch.hdr.firecode_used)
      {
          log_ptr->status |= 0x2; // bit 1
      }

      if (rpt_ptr->tch_facch.hdr.ufi)
     {
         log_ptr->status |= 0x20; // bit 5
     }

      log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.sid << 3 ); //bit 3,4
      log_ptr->status |= (byte)( rpt_ptr->tch_facch.hdr.voicing_mode << 6 ); //bit 6,7

      if (rpt_ptr->tch_facch.hdr.bfi)
    {
      log_ptr->status |= 0x1; // bit 0
    }

      log_ptr->msg_len =  rpt_ptr->tch_facch.hdr.len;

      log_ptr->codec_mode = codec_mode;


      gsmdiag_log_commit(&pkt_info);
  }
}
#endif /* FEATURE_GSM_AMR */

/*===========================================================================
FUNCTION  l1_log_power_scan

DESCRIPTION
  This function should be called when L1 power scan results are available
  and sorted. It will send a log packet to the DM.
  NOTE: We are only going to send power info for cells above the threshold

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern  Frequency_List_T *l1_Frequency_List_Store[];

void l1_log_power_scan ( gas_id_t gas_id )
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
    LOG_GSM_POWER_SCAN_C_type  *log_ptr;    /* pointer to log buffer */

    byte   n;
    uint32 log_size;
    gsmdiag_pkt_info_t pkt_info;
    uint16 count = l1_Frequency_List->num_above_sort_threshold_criteria;

    /* Limit cell count to MAX_PSCAN_LIST_SIZE */
    if ( count > MAX_PSCAN_LIST_SIZE )
    {
        count = MAX_PSCAN_LIST_SIZE;
    }

    /*
     * LOG_RECORD_DEFINE(LOG_GSM_POWER_SCAN_C)
     *   word                count;
     *   dBx16_T             threshold;
     *   word                list_size;
     *   l1_log_pscan_info_T freq_list[MAX_PSCAN_LIST_SIZE];
     */
    log_size = ( sizeof(log_hdr_type) + sizeof(word) + sizeof(dBx16_T) + sizeof(word) + ( count * sizeof(l1_log_pscan_info_T) ) );

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_POWER_SCAN_C, log_size, gas_id);
    log_ptr = (LOG_GSM_POWER_SCAN_C_type *)gsmdiag_log_alloc(&pkt_info);

    /* Try to allocate memory for the log packet */
    if ( NULL != log_ptr )
    {   // Fill packet and commit it
        log_ptr->count      =  l1_Frequency_List->length;                 // # in list
        log_ptr->threshold  =  L1_pscan_get_threshold(gas_id);
        log_ptr->list_size  =  count;                                    // # above thresh

        for ( n = 0; n < count; n++ )
        {                                                                // for each above thresh:
           log_ptr->freq_list[n].arfcn      = arfcn_to_word(l1_Frequency_List->ARFCN[l1_Frequency_List->sorted_array_indexes[n]]);          // channel
           log_ptr->freq_list[n].power      = l1_Frequency_List->power_dBm_x16[l1_Frequency_List->sorted_array_indexes[n]];  // power
        }

        gsmdiag_log_commit(&pkt_info);
    }//endif log_ptr
}

/*===========================================================================
FUNCTION  l1_log_

DESCRIPTION
  This function should be called when
  It will send a log packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_log_agc_params             ( void ){}
void l1_log_afc_params             ( void ){}
void l1_log_measurement_report     ( void ){}
void l1_log_surrounding_cell_info  ( void ){}
void l1_log_calibration_values     ( void ){}

/*===========================================================================

FUNCTION  l1_log_state

DESCRIPTION
  This function should be called when L1 state info changes.
  It will send an event packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_state ( gas_id_t gas_id )
{
#ifdef DEBUG_DISABLE_L1STATE_EVENT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    uint8  l1_state;
    volatile ISRTIM_CMD_BLK* gl1_l1_isrtsk_blk_ptr = &l1_tsk_buffer[gas_id];
    l1_state = gl1_l1_isrtsk_blk_ptr->l1_state;
#else
    uint8  l1_state = l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].l1_state;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

    // There is a problem with latest event core code -- not ISR friendly
    GERAN_EVENT_REPORT_PAYLOAD( EVENT_GSM_L1_STATE, 1, &( l1_state ) );
#endif
}

#ifdef FEATURE_GSM_AMR

/*===========================================================================

FUNCTION  l1_log_ratscch_in_dtx

DESCRIPTION
  This function should be called when a RATSCCH message is to be transmitted
  on the U/L in DTX mode. It will send an event packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_ratscch_in_dtx ( gas_id_t gas_id )
{
  GERAN_EVENT_REPORT( EVENT_GSM_RATSCCH_IN_DTX );
}

/*===========================================================================

FUNCTION  l1_log_facch_in_dtx

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_facch_in_dtx ( gas_id_t gas_id )
{
  GERAN_EVENT_REPORT( EVENT_GSM_FACCH_IN_DTX );
}

/*===========================================================================

FUNCTION  l1_log_facch_and_ratscch_collision

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_facch_and_ratscch_collision ( gas_id_t gas_id )
{
  GERAN_EVENT_REPORT( EVENT_GSM_FACCH_AND_RATSCCH_COLLISION );
}

/*===========================================================================

FUNCTION  l1_log_facch_and_sid_update_collision

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_facch_and_sid_update_collision ( gas_id_t gas_id )
{
  GERAN_EVENT_REPORT( EVENT_GSM_FACCH_AND_SID_UPDATE_COLLISION);
}

/*===========================================================================

FUNCTION  l1_log_ratscch_and_sid_update_collision

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_ratscch_and_sid_update_collision ( gas_id_t gas_id )
{
  GERAN_EVENT_REPORT(EVENT_GSM_RATSCCH_AND_SID_UPDATE_COLLISION);
}

/*===========================================================================

FUNCTION  l1_log_amr_state_change

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_amr_state_change(l1_log_amr_state_change_T *data, gas_id_t gas_id )
{
  if ( data )
    {
      GERAN_EVENT_REPORT_PAYLOAD(EVENT_GSM_AMR_STATE_CHANGE,
         sizeof(l1_log_amr_state_change_T),
         data );
    }
}


/*===========================================================================

FUNCTION  l1_log_amr_cmi_phase_change

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_amr_cmi_phase_change(l1_log_amr_cmi_phase_change_T *data, gas_id_t gas_id )
{
  if ( data )
    {
      GERAN_EVENT_REPORT_PAYLOAD( EVENT_GSM_RATSCCH_CMI_PHASE_CHANGE,
          sizeof(l1_log_amr_cmi_phase_change_T),
          data );
    }
}


/*===========================================================================

FUNCTION  l1_log_amr_req_act_timer_expiry

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_amr_req_act_timer_expiry
(l1_log_amr_req_act_timer_expiry_T *data, gas_id_t gas_id)
{
  if ( data )
    {
      GERAN_EVENT_REPORT_PAYLOAD( EVENT_GSM_RATSCCH_REQ_ACT_TIMER_EXPIRY,
          sizeof(l1_log_amr_req_act_timer_expiry_T),
          data );
    }
}

/*===========================================================================

FUNCTION  l1_log_amr_ack_act_timer_expiry

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_amr_ack_act_timer_expiry
(l1_log_amr_ack_act_timer_expiry_T *data, gas_id_t gas_id )
{
  if ( data )
    {
      GERAN_EVENT_REPORT_PAYLOAD(EVENT_GSM_RATSCCH_ACK_ACT_TIMER_EXPIRY,
         sizeof (l1_log_amr_ack_act_timer_expiry_T),
         data );
    }
}

/*===========================================================================

FUNCTION  l1_log_amr_ratscch_req

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_amr_ratscch_req
(l1_log_amr_ratscch_req_T *req, gas_id_t gas_id )
{
  if ( req )
    {
      GERAN_EVENT_REPORT_PAYLOAD(EVENT_GSM_AMR_RATSCCH_REQ,
         sizeof (byte),
         req );
    }
}

/*===========================================================================

FUNCTION  l1_log_amr_ratscch_rsp

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_amr_ratscch_rsp( uint8 rsp, gas_id_t gas_id )
{
  GERAN_EVENT_REPORT_PAYLOAD(EVENT_GSM_AMR_RATSCCH_RSP,
       sizeof (rsp),
       &rsp );
}

/*===========================================================================

FUNCTION  l1_log_amr_cmc_turnaround_time

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_log_amr_cmc_turnaround_time ( uint16 elapsed_frames, gas_id_t gas_id )
{
  GERAN_EVENT_REPORT_PAYLOAD( EVENT_GSM_AMR_CMC_TURNAROUND_TIME,
      sizeof (elapsed_frames),
      &elapsed_frames );
}
#endif /* FEATURE_GSM_AMR */

/*===========================================================================
FUNCTION  l1_log_serving_aux_meas

DESCRIPTION
  Log serving cell auxiliary measurements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_log_serving_aux_meas ( gas_id_t gas_id )
{
    l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

    LOG_GSM_SERVING_AUX_MEAS_C_type *log_ptr;
    gsmdiag_pkt_info_t pkt_info;

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_SERVING_AUX_MEAS_C, sizeof(LOG_GSM_SERVING_AUX_MEAS_C_type), gas_id);
    log_ptr = (LOG_GSM_SERVING_AUX_MEAS_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr != NULL  )
    {
       /* Fill packet and commit it */
       log_ptr->rx_power_instant = l1_serving_cell_meas_ptr->rx_power_instant;
       log_ptr->SNR_is_bad       = l1_serving_cell_meas_ptr->SNR_is_bad;
       gsmdiag_log_commit(&pkt_info);
    }
}

/*===========================================================================
FUNCTION  l1_log_ncell_aux_meas

DESCRIPTION
  Log neighbor cell auxiliary measurements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_log_ncell_aux_meas ( gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    LOG_GSM_NCELL_AUX_MEAS_C_type *log_ptr;
    byte    n;
    uint32  log_size;
    byte    count = l1_sc_globals_ptr->sorted_cell_count;
    gsmdiag_pkt_info_t pkt_info;

    /* Limit cell count to MAX_NCELL_LIST_SIZE */
    if (count > MAX_NCELL_LIST_SIZE)
    {
        count = MAX_NCELL_LIST_SIZE;
    }

    log_size = sizeof(LOG_GSM_NCELL_AUX_MEAS_C_type) - ((MAX_NCELL_LIST_SIZE - count)*sizeof(l1_log_ncell_aux_meas_T));

    /* Try to allocate memory for the log packet */
    l1_log_packet_init(&pkt_info, LOG_GSM_NCELL_AUX_MEAS_C, log_size, gas_id);
    log_ptr = (LOG_GSM_NCELL_AUX_MEAS_C_type *)gsmdiag_log_alloc(&pkt_info);

    if ( log_ptr != NULL )
    {
       /* Fill packet and commit it */
       log_ptr->cell_count = count;

       for(n=0; n<count; ++n)
       {
          log_ptr->ncells[n].arfcn            = arfcn_to_word(l1_sc_globals_ptr->sorted_cells[n]->bs.arfcn);
          log_ptr->ncells[n].rx_power_instant = l1_sc_globals_ptr->sorted_cells[n]->bs.rx_power_instant;
       }
       gsmdiag_log_commit(&pkt_info);
    }
}

/*===========================================================================
FUNCTION l1_log_ncell_bcch_burst_metrics

DESCRIPTION
  Helper function to log BURST metrics for this neighbor cell broadcast
  channel decode.

RETURN VALUE
  None
===========================================================================*/
void l1_log_ncell_bcch_burst_metrics (gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id)
{
   LOG_GSM_BCCH_BURST_METRICS_C_type  *log_ptr;
   int i;
   uint32 current_FN;
   gsmdiag_pkt_info_t pkt_info;

   for (i = 0; i < 4; i++)
   {
      /* Only log valid metrics */
      if (!rpt[i]->valid) continue;

      /* Try to allocate memory for the log packet */
      l1_log_packet_init(&pkt_info, LOG_GSM_BCCH_BURST_METRICS_C, sizeof(LOG_GSM_BCCH_BURST_METRICS_C_type), gas_id);
      log_ptr = (LOG_GSM_BCCH_BURST_METRICS_C_type *)gsmdiag_log_alloc(&pkt_info);

      /* If we are not logging then skip the rest */
      if ( log_ptr == NULL) break;

      current_FN = GSTMR_GET_FN_GERAN( gas_id);

      /* Fill in the log */
      log_ptr->FN            =  SUB_FN(current_FN, rpt[i]->offset_to_sob);
      log_ptr->arfcn         =  arfcn_to_word(rpt[i]->arfcn);
      log_ptr->rssi          =  rpt[i]->rssi;
      log_ptr->pwr_dBm_x16   =  rpt[i]->pwr_dBm_x16;
      log_ptr->dc_offset_i   =  rpt[i]->dc_offset_i;
      log_ptr->dc_offset_q   =  rpt[i]->dc_offset_q;
      log_ptr->freq_offset   =  rpt[i]->freq_offset;
      log_ptr->timing_offset =  rpt[i]->timing_offset;
      log_ptr->snr           =  rpt[i]->snr;
      log_ptr->gain_state    =  (int)rpt[i]->rx_gain_range;

      gsmdiag_log_commit(&pkt_info);
   }
}

/*===========================================================================

FUNCTION l1_log_ncell_bcch_msg_metrics

DESCRIPTION
  Helper function to log MSG metrics for this neighbor cell broadcast
  channel decode.

===========================================================================*/
void l1_log_ncell_bcch_msg_metrics (ccch_decode_rpt *rpt, gas_id_t gas_id)
{
   LOG_GSM_BCCH_MSG_METRICS_C_type  *log_ptr;
   uint32 current_FN;
   gsmdiag_pkt_info_t pkt_info;

   /* Try to allocate memory for the log packet */
   l1_log_packet_init(&pkt_info, LOG_GSM_BCCH_MSG_METRICS_C, sizeof(LOG_GSM_BCCH_MSG_METRICS_C_type), gas_id);
   log_ptr = (LOG_GSM_BCCH_MSG_METRICS_C_type *)gsmdiag_log_alloc(&pkt_info);

   if (log_ptr != NULL)
   {
      current_FN = GSTMR_GET_FN_GERAN( gas_id);
      log_ptr->FN     =  SUB_FN(current_FN, rpt->offset_to_sob);
      log_ptr->arfcn  =  arfcn_to_word(rpt->arfcn);
      memscpy((void*)log_ptr->data,sizeof(log_ptr->data), &rpt->hdr, sizeof(gl1_defs_rx_hdr_struct));

      gsmdiag_log_commit(&pkt_info);
   }
}

/*===========================================================================
FUNCTION l1_log_ncell_sch_burst_metrics

DESCRIPTION
  Helper function to log BURST metrics for this neighbor cell synch channel
  decode.

RETURN VALUE
  None
===========================================================================*/
void l1_log_ncell_sch_burst_metrics (gl1_msg_metrics_rpt *rpt, gas_id_t gas_id)
{
   LOG_GSM_SCH_BURST_METRICS_C_type  *log_ptr;
   uint32 current_FN;
   gsmdiag_pkt_info_t pkt_info;

   /* Try to allocate memory for the log packet */
   l1_log_packet_init(&pkt_info, LOG_GSM_SCH_BURST_METRICS_C, sizeof(LOG_GSM_SCH_BURST_METRICS_C_type), gas_id);
   log_ptr = (LOG_GSM_SCH_BURST_METRICS_C_type *)gsmdiag_log_alloc(&pkt_info);

   if (log_ptr != NULL)
   {
      current_FN = GSTMR_GET_FN_GERAN( gas_id);
      log_ptr->FN            =  SUB_FN( current_FN, rpt->offset_to_sob);
      log_ptr->arfcn         =  arfcn_to_word(rpt->arfcn);
      log_ptr->rssi          =  rpt->rssi;
      log_ptr->pwr_dBm_x16   =  rpt->pwr_dBm_x16;
      log_ptr->dc_offset_i   =  rpt->dc_offset_i;
      log_ptr->dc_offset_q   =  rpt->dc_offset_q;
      log_ptr->freq_offset   =  rpt->freq_offset;
      log_ptr->timing_offset =  rpt->timing_offset;
      log_ptr->snr           =  rpt->snr;
      log_ptr->gain_state    =  (int)rpt->rx_gain_range;

      gsmdiag_log_commit(&pkt_info);
   }
}

/*===========================================================================

FUNCTION l1_log_ncell_sch_msg_metrics

DESCRIPTION
  Helper function to log MSG metrics for this neighbor cell synch channel
  decode.

===========================================================================*/
void l1_log_ncell_sch_msg_metrics (sch_decode_rpt *rpt, gas_id_t gas_id)
{
   LOG_GSM_SCH_MSG_METRICS_VER2_C_type  *log_ptr;
   uint32  current_FN;
   gsmdiag_pkt_info_t pkt_info;

   /* Try to allocate memory for the log packet */
   l1_log_packet_init(&pkt_info, LOG_GSM_SCH_MSG_METRICS_VER2_C, sizeof(LOG_GSM_SCH_MSG_METRICS_VER2_C_type), gas_id);
   log_ptr = (LOG_GSM_SCH_MSG_METRICS_VER2_C_type *)gsmdiag_log_alloc(&pkt_info);

   if (log_ptr != NULL)
   {
      current_FN      = GSTMR_GET_FN_GERAN( gas_id);
      log_ptr->FN     =  SUB_FN( current_FN, rpt->offset_to_sob);
      log_ptr->arfcn  =  arfcn_to_word(rpt->arfcn);
      log_ptr->crc    =  rpt->hdr.good_data;
      COPY_SCH_DATA( &log_ptr->data, &rpt->data );
      gsmdiag_log_commit(&pkt_info);
   }
}

#ifdef L1_MSG_TRAFFIC_LOGGING
/*===========================================================================
FUNCTION  l1_log_msg_traffic

DESCRIPTION
  Log Layer1 message traffic to\from other tasks into NV.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void l1_log_msg_traffic(uint8 msg_set, uint8 msg_ID, uint32 FN, boolean incoming_message, gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
   l1_NVLog_struct_T *l1_nv_log_ptr = &l1_nv_log[gas_id];

    if (( msg_set == MS_L1_L1 ) &&
        (( msg_ID == L1_GRM_REQ_AND_NOTIFY_ENH ) ||
         ( msg_ID == L1_GRM_RELEASE )            ||
         ( msg_ID == L1_GRM_DIVRX_RELEASE )      ||
         ( msg_ID == L1_GRM_RESERVE_AT )         ||
         ( msg_ID == L1_GRM_REQUEST )            ||
#ifdef  FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef  FEATURE_DUAL_SIM
         ( msg_ID == L1_GRM_RESERVE_AT_CCCH )    ||
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
#ifdef  FEATURE_QSH_EVENT_METRIC
         ( msg_ID == L1_QSH_METRIC_IND )         ||
#endif /* FEATURE_QSH_EVENT_METRIC */
         ( msg_ID == L1_SC_CALL_IN_TASK_CNF )    ||
         ( msg_ID == L1_MULTI_SIM_FW_RF_LOAD )   ||
         ( msg_ID == L1_DO_TIME_PWR_AVG )        ||
         ( msg_ID == L1_FUNC_CALL_IN_TASK ))
       )
    {
      /* ignore these Layer 1 internal messages */
      return;
    }

    if ((msg_set == MS_L2_L1) &&
        (msg_ID  == PH_DATA_IND))
    {
      /* ignore downlink data for Layer 2 */
      return;
    }

    if ((msg_set == MS_MAC_L1)&&
        ((msg_ID == L1_MAC_USF_DETECTED)     ||
         (msg_ID == L1_MAC_QUAL_MEASURMENTS) ||
         (msg_ID == MAC_PH_DATA_IND)))
    {
      /* ignore downlink data and measurements for GMAC */
      return;
    }

    /* RR messages can be filtered, or logged into a separate trace buffer */
    if (msg_set == MS_RR_L1)
    {
      if ((msg_ID == MPH_BLOCK_QUALITY_IND)            ||
          (msg_ID == MPH_SET_PRIORITY_IND)             ||
          (msg_ID == MPH_DSC_THRESHOLD_IND)            ||
          (msg_ID == MPH_ACQUISITION_STATE_IND)        ||
          (msg_ID == MPH_SURROUND_MEAS_IND)            ||
          (msg_ID == MPH_SERVING_DED_MEAS_IND)         ||
          (msg_ID == MPH_SERVING_IDLE_MEAS_IND)        ||
          (msg_ID == MPH_SURROUND_MEAS_IND)            ||
          (msg_ID == MPH_SURROUND_WCDMA_IDLE_MEAS_IND) ||
          (msg_ID == MPH_SERVING_AUX_MEAS_IND)         ||
          (msg_ID == MPH_SURROUND_IRAT_MEAS_IND))
      {
        /* ignore these RR messages */;
      }
      else
      {
        /* ensure the RR trace buffer index is bounded correctly before use */
        if (l1_nv_log_ptr->l1_RR_log_msg_index >= L1_MSG_RR_L1_LOG_LEN)
        {
          l1_nv_log_ptr->l1_RR_log_msg_index = 0;
        }
        l1_nv_log_ptr->l1_RR_log_msg[l1_nv_log_ptr->l1_RR_log_msg_index].FN               = FN;
        l1_nv_log_ptr->l1_RR_log_msg[l1_nv_log_ptr->l1_RR_log_msg_index].msg_set          = msg_set;
        l1_nv_log_ptr->l1_RR_log_msg[l1_nv_log_ptr->l1_RR_log_msg_index].msg_ID           = msg_ID;
        l1_nv_log_ptr->l1_RR_log_msg[l1_nv_log_ptr->l1_RR_log_msg_index].incoming_message = incoming_message;
        l1_nv_log_ptr->l1_RR_log_msg_index++;
      }

      /* no further action for any RR message */
      return;
    }

    /* ensure the trace buffer index is bounded correctly before use */
    if (l1_nv_log_ptr->l1_log_msg_index >= L1_MSG_LOG_LEN)
    {
      l1_nv_log_ptr->l1_log_msg_index = 0;
    }
    l1_nv_log_ptr->l1_log_msg[l1_nv_log_ptr->l1_log_msg_index].FN               = FN;
    l1_nv_log_ptr->l1_log_msg[l1_nv_log_ptr->l1_log_msg_index].msg_set          = msg_set;
    l1_nv_log_ptr->l1_log_msg[l1_nv_log_ptr->l1_log_msg_index].msg_ID           = msg_ID;
    l1_nv_log_ptr->l1_log_msg[l1_nv_log_ptr->l1_log_msg_index].incoming_message = incoming_message;
    l1_nv_log_ptr->l1_log_msg_index++;
  }
  else
  {
    MSG_GERAN_ERROR_1("BAD GAS ID 0x%02X", (int)gas_id);
  }

  return;
}
#endif /* L1_MSG_TRAFFIC_LOGGING */

#ifdef FEATURE_GL1_VAR_LOGGING

/*===========================================================================
FUNCTION  l1_var_log
DESCRIPTION
  Create a log of the L1 varibles each time it is changed
  log line number, frame number and file name from where it is changed.
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void l1_var_log(uint8 var,uint32 linenum, l1_fname_T fname,uint16 var_id,gas_id_t gas_id)
{
   switch(var_id)
   {
    case L1_VAR_RECEIVE_IN_PROGRESS:
      l1_rcv_log(var,linenum,fname,gas_id);
      break;
    case L1_VAR_MONSCAN_IN_PROGRESS:
      l1_monscan_log(var,linenum,fname,gas_id);
      break;
    case L1_VAR_W_ID_SRCH_IN_PROGRESS:
      l1_wsrch_id_log(var,linenum,fname,gas_id);
      break;
    case L1_VAR_W_SRCH_IN_PROGRESS:
      l1_wsrch_log(var,linenum,fname,gas_id);
      break;
    case L1_VAR_IDLE_MONITORS_IN_PROGRESS:
      l1_idle_monitors_log(var,linenum,fname,gas_id);
      break;
    default:
      break;
   }
}


/*===========================================================================

FUNCTION  l1_rcv_log

DESCRIPTION
  Create a log of the L1 varible recive in progress each time it is changed
  log line number, frame number and file name from where it is changed.
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/

void l1_rcv_log( uint8 recive_in_progress,uint32 linenum,l1_fname_T fname, gas_id_t gas_id)
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   static uint8 prev_value[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

   if (recive_in_progress!= prev_value[gas_id])
   {
      prev_value[gas_id] = recive_in_progress;
      l1_nv_log_ptr->l1_rcv_log_buf[l1_nv_log_ptr->l1_rcv_log_index].FN          = GSTMR_GET_FN_GERAN( gas_id);
      l1_nv_log_ptr->l1_rcv_log_buf[l1_nv_log_ptr->l1_rcv_log_index].rcv         = recive_in_progress;
      l1_nv_log_ptr->l1_rcv_log_buf[l1_nv_log_ptr->l1_rcv_log_index].linenum     = linenum;
      l1_nv_log_ptr->l1_rcv_log_buf[l1_nv_log_ptr->l1_rcv_log_index].filename    = fname;
      if (++l1_nv_log_ptr->l1_rcv_log_index >= L1_VAR_LOG_LEN)
      {
        l1_nv_log_ptr->l1_rcv_log_index = 0;
      }
   }
}

/*===========================================================================

FUNCTION  l1_monscan_log

DESCRIPTION
  Create a log of the L1 varible monscan in progress each time it is changed
  log line number, frame number and file name from where it is changed..
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void l1_monscan_log( uint8 monscan_in_progress,uint32 linenum,l1_fname_T fname, gas_id_t gas_id)
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   static uint8 prev_value[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

   if (monscan_in_progress!= prev_value[gas_id])
   {
      prev_value[gas_id] = monscan_in_progress;
      l1_nv_log_ptr->l1_monscan_log_buf[l1_nv_log_ptr->l1_monscan_log_index].FN          = GSTMR_GET_FN_GERAN( gas_id);
      l1_nv_log_ptr->l1_monscan_log_buf[l1_nv_log_ptr->l1_monscan_log_index].mon_scan    = monscan_in_progress;
      l1_nv_log_ptr->l1_monscan_log_buf[l1_nv_log_ptr->l1_monscan_log_index].linenum     = linenum;
      l1_nv_log_ptr->l1_monscan_log_buf[l1_nv_log_ptr->l1_monscan_log_index].filename    = fname;
      if (++l1_nv_log_ptr->l1_monscan_log_index >= L1_VAR_LOG_LEN)
      {
        l1_nv_log_ptr->l1_monscan_log_index = 0;
      }
   }
}


/*===========================================================================

FUNCTION  l1_wcdma_id_search_in_progress_log
DESCRIPTION
  Create a log of the L1 varible l1_wcdma_id_search_in_progress_ each time it is changed
  log line number, frame number and file name from where it is changed..
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void l1_wsrch_id_log( uint8 wcdma_srch_id_progress ,uint32 linenum,l1_fname_T fname, gas_id_t gas_id)
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   static uint8 prev_value[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

   if (wcdma_srch_id_progress!= prev_value[gas_id])
   {
      prev_value[gas_id] = wcdma_srch_id_progress;
      l1_nv_log_ptr->l1_wsrch_id_log_buf[l1_nv_log_ptr->l1_wsrch_id_log_index].FN          = GSTMR_GET_FN_GERAN( gas_id);
      l1_nv_log_ptr->l1_wsrch_id_log_buf[l1_nv_log_ptr->l1_wsrch_id_log_index].wsrch_id    = wcdma_srch_id_progress;
      l1_nv_log_ptr->l1_wsrch_id_log_buf[l1_nv_log_ptr->l1_wsrch_id_log_index].linenum     = linenum;
      l1_nv_log_ptr->l1_wsrch_id_log_buf[l1_nv_log_ptr->l1_wsrch_id_log_index].filename    = fname;
      if (++l1_nv_log_ptr->l1_wsrch_id_log_index >= L1_VAR_LOG_LEN)
      {
        l1_nv_log_ptr->l1_wsrch_id_log_index = 0;
      }
   }
}

/*===========================================================================

FUNCTION  l1_wcdma_srch_in_progress_log
DESCRIPTION
  Create a log of the L1 varible l1_wcdma_srch_in_progress each time it is changed
  log line number, frame number and file name from where it is changed..
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void l1_wsrch_log( uint8 wcdma_srch_in_progress,uint32 linenum,l1_fname_T fname, gas_id_t gas_id)
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   static uint8 prev_value[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

   if (wcdma_srch_in_progress!= prev_value[gas_id])
   {
      prev_value[gas_id] = wcdma_srch_in_progress;
      l1_nv_log_ptr->l1_wsrch_log_buf[l1_nv_log_ptr->l1_wsrch_log_index].FN          = GSTMR_GET_FN_GERAN( gas_id);
      l1_nv_log_ptr->l1_wsrch_log_buf[l1_nv_log_ptr->l1_wsrch_log_index].wsrch       = wcdma_srch_in_progress;
      l1_nv_log_ptr->l1_wsrch_log_buf[l1_nv_log_ptr->l1_wsrch_log_index].linenum     = linenum;
      l1_nv_log_ptr->l1_wsrch_log_buf[l1_nv_log_ptr->l1_wsrch_log_index].filename    = fname;
      if (++l1_nv_log_ptr->l1_wsrch_log_index >= L1_VAR_LOG_LEN)
      {
        l1_nv_log_ptr->l1_wsrch_log_index = 0;
      }
   }
}


/*===========================================================================

FUNCTION  l1_idle_moniters_log

DESCRIPTION
  Create a log of the L1 varible l1_idle_moniters_in_progress each time it is changed
  log line number, frame number and file name from where it is changed..
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void l1_idle_monitors_log( uint8 idle_monitors,uint32 linenum,l1_fname_T fname, gas_id_t gas_id)
{
   l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
   static uint8 prev_value[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

   if (idle_monitors!= prev_value[gas_id])
   {
      prev_value[gas_id] = idle_monitors;
      l1_nv_log_ptr->l1_monitors_log_buf[l1_nv_log_ptr->l1_monitors_msg_index].FN                  = GSTMR_GET_FN_GERAN( gas_id);
      l1_nv_log_ptr->l1_monitors_log_buf[l1_nv_log_ptr->l1_monitors_msg_index].idle_monitors       = idle_monitors;
      l1_nv_log_ptr->l1_monitors_log_buf[l1_nv_log_ptr->l1_monitors_msg_index].linenum             = linenum;
      l1_nv_log_ptr->l1_monitors_log_buf[l1_nv_log_ptr->l1_monitors_msg_index].filename            = fname;
      if (++l1_nv_log_ptr->l1_monitors_msg_index >= L1_VAR_LOG_LEN)
      {
        l1_nv_log_ptr->l1_monitors_msg_index = 0;
      }
   }
}

#endif
/*===========================================================================
FUNCTION  l1_log_signal_info

DESCRIPTION
  Store Rx and Tx signal info for current active ARFCN in Eng Mode structure

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void l1_log_signal_info(geran_eng_mode_signal_info_t *signal_info_ptr, gas_id_t gas_id)
{
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

   int32 rx_pwr,divrx_power;
   uint8 prx,drx;
   gl1_rxd_control_type gl1_rxd_ctl_flags = gl1_get_rxd_control_flags(gas_id);

   prx = drx = FALSE;


   /* initialise key fields, which use a different invalid value */
   signal_info_ptr->band = ENG_MODE_BAND_INVALID_VALUE;
   signal_info_ptr->channel = ENG_MODE_CHANNEL_INVALID_VALUE;
   signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].rx_pwr = ENG_MODE_SIGNAL_INFO_INVALID_VALUE;
   signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].is_radio_tuned = FALSE;
   signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_1].rx_pwr = ENG_MODE_SIGNAL_INFO_INVALID_VALUE;
   signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_1].is_radio_tuned = FALSE;
   signal_info_ptr->rx_power.rx_diversity_indicator = ENG_MODE_RX_CHAIN_UNAVAIL;

   /*Checking the enabling status of DRX,PRX and updating parameters accordingly*/
   if(gl1_rxd_ctl_flags.enableRxd && gl1_rxd_ctl_flags.enablePrx && gl1_rxd_ctl_flags.enableDrx)
   {
     signal_info_ptr->rx_power.rx_diversity_indicator = ENG_MODE_RX_CHAIN_0_AVAIL;
     signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].is_radio_tuned = TRUE;
     prx = TRUE;
     if(gl1_hw_get_rxd_enable_status(gas_id))
       {         
         /* RxD enabled */
         signal_info_ptr->rx_power.rx_diversity_indicator = ENG_MODE_RX_CHAIN_ALL_AVAIL;
         signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_1].is_radio_tuned = TRUE;
         drx = TRUE;
       }
   }
   else if(gl1_rxd_ctl_flags.enablePrx) /* PRx only enabled */
   {
     signal_info_ptr->rx_power.rx_diversity_indicator = ENG_MODE_RX_CHAIN_0_AVAIL;
     signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].is_radio_tuned = TRUE;
     prx = TRUE;
   }
   else if(gl1_rxd_ctl_flags.enableDrx) /* DRx only enabled */
   {
     signal_info_ptr->rx_power.rx_diversity_indicator = ENG_MODE_RX_CHAIN_1_AVAIL;
     signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_1].is_radio_tuned = TRUE;
     drx = TRUE;
   }  

   /* if GSM is not in idle, dedicated, transfer or dtm, send invalid report */
   if(l1_tsk_buffer[gas_id].l1_state < L1_IDLE_MODE)
   {
      /* set as not tuned, as GSM not in active state */
      signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].is_radio_tuned = FALSE;
   }
   else if(l1_tsk_buffer[gas_id].l1_state < L1_DEDICATED_MODE)
   {  /* fill response packet for serving cell info when in idle */
      signal_info_ptr->band = (int32)l1_idle_data->campedon_cell_ARFCN.band;
      signal_info_ptr->channel = (int32)l1_idle_data->campedon_cell_ARFCN.num;

      if ( 0 != l1_serving_cell_meas_ptr->no_of_measurements )
      {
        if(prx)
        {
          /* get Rx power in dBm*16 */
          rx_pwr = (l1_serving_cell_meas_ptr->rx_power / l1_serving_cell_meas_ptr->no_of_measurements);
          /* convert to dBm*10 */
          signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].rx_pwr = (rx_pwr*10) >> 4;
        }
        if(drx)
        {
          /* get Rx power in dBm*16 */
          divrx_power = (l1_serving_cell_meas_ptr->divrx_power / l1_serving_cell_meas_ptr->no_of_measurements);
          /* convert to dBm*10 */
          signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_1].rx_pwr = (divrx_power*10) >> 4;
        }
      }
   }
   else
   {  /* fill response packet for TCH/PDCH cell info if in dedicated, transfer or dtm */
      /* get Rx power in dBm*16 and convert to dBm*10 */
     if(prx)
     {
       signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].rx_pwr = (gl1_msg_get_dtm_expected_power(gas_id)*10)>>4;
     }
     if(drx)
     {
       signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_1].rx_pwr = (gl1_msg_get_dtm_expected_power_divrx(gas_id)*10)>>4;
     }
      signal_info_ptr->band = gl1_msg_get_dtm_band(gas_id);
      signal_info_ptr->channel = gl1_msg_get_dtm_arfcn(gas_id);

      /* Tx pwr info is now is populated in the caller func geran_eng_mode_data_read() */
   }
   MSG_GERAN_HIGH_5_G("Prx enabled:%d, Drx enabled:%d, rx_diversity_indicator:%d, Prx_power(dBm*10):%d, Drx_power(dBm*10):%d",
                       prx,drx,signal_info_ptr->rx_power.rx_diversity_indicator,
                       signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_0].rx_pwr,
                       signal_info_ptr->rx_power.rx_pwr_chain[ENG_MODE_GSM_RX_CHAIN_1].rx_pwr);
}

/*===========================================================================

FUNCTION geran_eng_get_pa

DESCRIPTION

  retrieve the current PA gain state from Rf for the most recent Tx

PARAMETERS
  gas_id

RETURN VALUE
  uint8

SIDE EFFECTS


===========================================================================*/
uint8  l1_log_get_TxPa_state( gas_id_t gas_id )
{
   /*
   ** new api feature to read  PA gain state.
   */
   uint8 tx_pa = rfm_get_current_pa_state();

   return tx_pa;
}

/*===========================================================================

FUNCTION l1_log_convert_txlev_to_pwr_dBm

DESCRIPTION

  wrapper function for convert_txlev_to_pwr_dBm()

PARAMETERS

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void l1_log_convert_txlev_to_pwr_dBm(eng_mode_sys_band_T band, uint8 txlev, uint8 *power_out, gas_id_t gas_id)
{
   convert_txlev_to_pwr_dBm(  (sys_band_T)band, txlev, power_out, gas_id);
}

#ifdef FEATURE_GSM_LOG_COEX
/*===========================================================================
FUNCTION l1_log_coex_params

DESCRIPTION
  Function to log coexistence manager information. The log packet is build
  from the cxm log message received from firmware and the channel type and
  other information is recovered from cxm_log_table

RETURN VALUE
  None
===========================================================================*/
void l1_log_coex_params(volatile GfwCxmLogPacketBuff* cxm_log_buf_ptr, gas_id_t gas_id)
{
  LOG_GSM_COEX_PARAMS_C_type *log_ptr;
  gsmdiag_pkt_info_t pkt_info;
  volatile GfwCxmResultStruct* gfw_results = &cxm_log_buf_ptr->cxmResult[0];
  uint32 num_entries = cxm_log_buf_ptr->numEntries;

  if (num_entries > 10)
    num_entries = 10;

#ifdef FEATURE_GERAN_LOG_OPT
  if ( gl1_hw_disable_conditional_logging()== FALSE )
  {
    uint8 count = 0;
    boolean cxm_no_conflict = TRUE;

    /* Log CxM data only if there is a conflict */
    for ( count = 0; count < num_entries; count++)
    {
      if ( ( (uint8)gfw_results[count].cxmStatus == GFW_CXM_PROCEED ) ||
           ( (uint8)gfw_results[count].cxmStatus == GFW_CXM_YIELD ) )
      {
        cxm_no_conflict = FALSE ;
        break;
      }	
    }

    if ( cxm_no_conflict )
    {
      return;
    }
  }
#endif

  /* Try to allocate memory for the log packet */
  /* only allocate memory for the ammount of entries */
  l1_log_packet_init(&pkt_info, LOG_GSM_COEX_PARAMS_C,
                     sizeof(LOG_GSM_COEX_PARAMS_C_type) -
                     (10 - num_entries ) * sizeof(l1_cxm_result_info_T),
                     gas_id);

  log_ptr = (LOG_GSM_COEX_PARAMS_C_type *)gsmdiag_log_alloc(&pkt_info);

  if (log_ptr != NULL)
  {
    int i;
    int8 table_index;
    log_ptr->version = 2;
    log_ptr->gas_id = (uint8)gas_id;
    log_ptr->FN = cxm_log_buf_ptr->gsmFrameNo;
    log_ptr->num_entries = num_entries;

    for (i = 0; i < log_ptr->num_entries; i++)
    {
      /* Populate log entries received from FW */
      log_ptr->cxm_result_info[i].gstmr_start         = gfw_results[i].startGstmrQs;
      log_ptr->cxm_result_info[i].gstmr_end           = gfw_results[i].endGstmrQs;
      log_ptr->cxm_result_info[i].ustmr_start         = (uint32)gfw_results[i].startUstmr;
      log_ptr->cxm_result_info[i].ustmr_end           = (uint32)gfw_results[i].endUstmr;
      log_ptr->cxm_result_info[i].activity_type       = (uint8)gfw_results[i].activity_type;
      log_ptr->cxm_result_info[i].cxm_priority        = gfw_results[i].priority;
      log_ptr->cxm_result_info[i].cxm_freq_id         = gfw_results[i].freq_id;
      log_ptr->cxm_result_info[i].cxm_status          = (uint8)gfw_results[i].cxmStatus;
      log_ptr->cxm_result_info[i].arfcn               = gfw_results[i].arfcn;
      log_ptr->cxm_result_info[i].channel_id          = gfw_results[i].channel_id;
      log_ptr->cxm_result_info[i].action              = gfw_results[i].action;
      log_ptr->cxm_result_info[i].timeslot            = 0;
      log_ptr->cxm_result_info[i].tx_desense_back_off = 0;
      log_ptr->cxm_result_info[i].tx_vbatt_back_off   = 0;
      log_ptr->cxm_result_info[i].power               = 0;

      switch (gfw_results[i].gfwCxmRegType)
      {
        case GFW_CXM_REG_TYPE_NORMAL:
          /* Try to find matching entry in gl1 cxm log table */
          table_index = l1_log_cxm_get_entry_index(gfw_results[i].seqNum, gfw_results[i].startGstmrQs,
                                                   gfw_results[i].endGstmrQs, gfw_results[i].arfcn,
                                                   gfw_results[i].activity_type, gfw_results[i].chanType, gas_id);

          /* If a valid entry is found then retrieve values */
          if (table_index >= 0)
          {
            uint8 channel_type, timeslot;
            uint16 arfcn, tx_desense_back_off, tx_vbatt_back_off, power;

            l1_log_cxm_get_entry(table_index, &arfcn, &channel_type, &timeslot,
                                 &tx_desense_back_off, &tx_vbatt_back_off, &power, gas_id);

            log_ptr->cxm_result_info[i].channel_type = channel_type;
            log_ptr->cxm_result_info[i].timeslot = timeslot;
            log_ptr->cxm_result_info[i].tx_desense_back_off = tx_desense_back_off;
            log_ptr->cxm_result_info[i].tx_vbatt_back_off = tx_vbatt_back_off;
            log_ptr->cxm_result_info[i].power = power;
          }
          else
          {
            log_ptr->cxm_result_info[i].channel_type = CXM_LOG_INVALID;
#ifdef ABORT_F3_REDUCTION         
            MSG_GERAN_HIGH_1_G("Invalid CXM Log, Firmware Channel: %d", gfw_results[i].chanType);
#endif
          }
          break;

        case GFW_CXM_REG_TYPE_ACQ:
          log_ptr->cxm_result_info[i].channel_type = CXM_LOG_ACQUISITION;
          break;

        case GFW_CXM_REG_TYPE_GAP:
          log_ptr->cxm_result_info[i].channel_type = CXM_LOG_QBTA_GAP;
          break;

        default:
          log_ptr->cxm_result_info[i].channel_type = CXM_LOG_INVALID;
      }
    }
    gsmdiag_log_commit(&pkt_info);
  }
}

/*===========================================================================
FUNCTION l1_log_cxm_table_init

DESCRIPTION
  Initialiase cxm_log_table to 0

RETURN VALUE
  None
===========================================================================*/
void l1_log_cxm_table_init(gas_id_t gas_id)
{
  memset(&cxm_log_table[gas_id], 0, sizeof(cxm_log_table_T));
}

/*===========================================================================
FUNCTION l1_log_cxm_new_entry

DESCRIPTION
  Function to log a new entry in cxm_log_table.

RETURN VALUE
  None
===========================================================================*/
void l1_log_cxm_new_entry(uint16 seq_num, uint16 arfcn, uint8 channel_type,
                          int16 gstmr_center, int8 tn,
                          uint16 tx_desense_back_off, uint16 vbatt_back_off,
                          uint16 power, uint8 activity, gas_id_t gas_id)
{
  cxm_log_table_T* cxm_log_ptr = &cxm_log_table[gas_id];

  /* Check if write index is within range and wrap around */
  if (cxm_log_ptr->write_index >= CXM_LOG_TABLE_SIZE)
  {
    cxm_log_ptr->write_index = 0;
  }

  /* If gstmr info is not valid try to workout based on tn */
  if (gstmr_center == CXM_LOG_UNKNOWN_VALUE && tn < 8)
  {
    gstmr_center = QS_PER_SLOT * tn + QS_PER_SLOT / 2;
  }

  /* If tn is not available try to workout based on gstmr */
  if (tn == CXM_LOG_UNKNOWN_VALUE && gstmr_center != CXM_LOG_UNKNOWN_VALUE)
  {
    tn = gstmr_center / QS_PER_SLOT;
  }

  if( gstmr_center >= QS_PER_FRAME )
  {
    gstmr_center -= QS_PER_FRAME;
  }

  /* Write info and increment write index */
  cxm_log_ptr->logs[cxm_log_ptr->write_index].seq_number = seq_num;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].arfcn = arfcn;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].channel_type = channel_type;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].gstmr_center = (uint16)gstmr_center;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].tn = (uint8)tn;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].tx_desense_back_off = tx_desense_back_off;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].vbatt_back_off = vbatt_back_off;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].power = power;
  cxm_log_ptr->logs[cxm_log_ptr->write_index].activity_type = activity;

  cxm_log_ptr->write_index++;
}


/*===========================================================================
FUNCTION l1_log_cxm_get_entry_index

DESCRIPTION
  Function to search an entry in cxm_log_table. If the frame number, arfcn
  and the gstmr values match an entry in the table, the index of the entry
  is returned. If no matching entry is found, return -1

RETURN VALUE
  Index of matching entry or -1
===========================================================================*/
static int8 l1_log_cxm_get_entry_index(uint16 seq_num, int16 gstmr_start, int16 gstmr_stop,
                                       uint16 arfcn, uint8 activity, GfwChanType chan_type, gas_id_t gas_id)
{
  cxm_log_table_T* cxm_log_ptr = &cxm_log_table[gas_id];
  uint32 i;

  // Check if we are in 8th slot. FW GSTMR roll over
  // at the end of the 7th slot
  if (gstmr_stop < FRAME_TICK_SETUP_MARGIN_QS)
  {
    gstmr_stop += QS_PER_FRAME;
  }

  // If activity start after FW GSTMR has rolled over
  // remove roll over effer
  if (gstmr_start < FRAME_TICK_SETUP_MARGIN_QS)
  {
    gstmr_start += QS_PER_FRAME;
  }

  gstmr_start -= FRAME_TICK_SETUP_MARGIN_QS;
  gstmr_stop -= FRAME_TICK_SETUP_MARGIN_QS;

  // When burst length exceeds 1000 QS the 
  // GSTMR start time can be before the center 
  // of the previous burst. 
  // In this scenario the wrong entry can
  // be retrieved from the log table.
  // If the burst length is longer than 700 QS
  // change the start time so that the burst length
  // is capped to 700 QS.
  if ( (gstmr_stop - gstmr_start > 700) &&
       ( (chan_type >= GFW_PDTCHU_CS1 && chan_type <= GFW_PTCCHU_EXT) ||
         (chan_type >= GFW_PDTCHU_MCS1 && chan_type <= GFW_PDTCHU_MCS9) ) )
  {
    gstmr_start = gstmr_stop - 700;
  }

  // Search for an activity that matches in the cxm_log
  // table
  for (i = 0; i < CXM_LOG_TABLE_SIZE; i++)
  {
    if (cxm_log_ptr->logs[i].seq_number == seq_num &&
        cxm_log_ptr->logs[i].arfcn == arfcn &&
        cxm_log_ptr->logs[i].activity_type == activity)

    {
      // Normal scenario when burst start and finish in
      // same frame
      if (gstmr_stop > gstmr_start &&
          cxm_log_ptr->logs[i].gstmr_center >= gstmr_start &&
          cxm_log_ptr->logs[i].gstmr_center <= gstmr_stop)
      {
        return i;
      }

      // Scenario where activity happens between tn 7 and tn 0
      // for example if an activity is programed in tn 7 it's
      // gstmr_stop is likely to be in tn 0
      // This also cover the case of async activities scheduled
      // between tn 7 and tn 0.
      // In these scenarios gstmr start can be anywhere between
      // 4375-5000 and gstmr_stop between 0-625
      if (gstmr_stop < gstmr_start &&
          (cxm_log_ptr->logs[i].gstmr_center >= gstmr_start ||
          cxm_log_ptr->logs[i].gstmr_center <= gstmr_stop))
      {
        return i;
      }
    }
  }
  return -1;
}


/*===========================================================================
FUNCTION l1_log_cxm_get_entry

DESCRIPTION
  Function to return the an entry from the cxm_log_table located at the
  given index

RETURN VALUE
  None
===========================================================================*/
void l1_log_cxm_get_entry(int8 index, uint16 *arfcn,
                          uint8 *channel_type, uint8 *tn,
                          uint16 *tx_desense_back_off, uint16 *vbatt_back_off,
                          uint16 *power, gas_id_t gas_id)
{
  cxm_log_table_T* cxm_log_ptr = &cxm_log_table[gas_id];

  *arfcn               = cxm_log_ptr->logs[index].arfcn;
  *channel_type        = cxm_log_ptr->logs[index].channel_type;
  *tn                  = cxm_log_ptr->logs[index].tn;
  *tx_desense_back_off = cxm_log_ptr->logs[index].tx_desense_back_off;
  *vbatt_back_off      = cxm_log_ptr->logs[index].vbatt_back_off;
  *power               = cxm_log_ptr->logs[index].power;
}
#endif /* FEATURE_GSM_LOG_COEX */

#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
/*===========================================================================
FUNCTION  l1_physical_layer_power_data

DESCRIPTION
  Helper function to collect power logging data

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_physical_layer_power_data( l1_power_info_data param, uint8 value, gas_id_t gas_id )
{
   l1_power_info_T *l1_power_data = &l1_power_info[gas_id];

   if ( l1_power_data->reset )
   {
      l1_power_data->ustmr = VSTMR_XO_READ_FULL();
      l1_power_data->reset = FALSE;
   }

   switch (param)
   {
      case NUM_ACTIVE_DL_SLOTS:
         l1_power_data->num_active_dl_slots += value;
         break;
      case NUM_ACTIVE_UL_SLOTS:
         l1_power_data->num_active_ul_slots += value;
         break;
      case AEQ_STATUS:
         l1_power_data->aeq_status = value;
         break;
      case NUM_POWER_MONITORS:
         l1_power_data->power_monitor += value;
         break;
      case UPLINK_POWER:
         l1_power_data->uplink_power = value;
         break;
      case RXD_STATUS:
         l1_power_data->rxd = value;
         break;
      default:
          MSG_GERAN_ERROR_1_G("Invalid power info parameter %d",param);
         break;
   }
}

/*===========================================================================
FUNCTION  l1_log_physical_layer_power_info

DESCRIPTION
  This function should be called when L1 power info needs to be sent.
  It will send a log packet to the DM.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_log_physical_layer_power_info( boolean immediate, gas_id_t gas_id )
{
    LOG_GERAN_PHYSICAL_LAYER_POWER_INFO_C_type  *log_ptr;
    l1_power_info_T *l1_power_data = &l1_power_info[gas_id];
    gsmdiag_pkt_info_t pkt_info;

    if ( !immediate )
    {
       if ((get_vstmr_FN_modMaxFN(gas_id)%20)!=0)
       {
          return;
       }
    }

    l1_log_packet_init(&pkt_info, LOG_GERAN_PHYSICAL_LAYER_POWER_INFO_C, sizeof(LOG_GERAN_PHYSICAL_LAYER_POWER_INFO_C_type), gas_id);
    log_ptr = (LOG_GERAN_PHYSICAL_LAYER_POWER_INFO_C_type *)gsmdiag_log_alloc(&pkt_info);

    /* Try to allocate memory for the log packet */
    if ( log_ptr )
    {
      // Fill packet and commit it
      memset(&log_ptr->padding[0],0,sizeof(log_ptr->padding));
      log_ptr->version             = LOG_GERAN_PHYSICAL_LAYER_POWER_INFO_VERSION;
      log_ptr->gas_id              = (uint8)gas_id;
      if (l1_power_data->ustmr == 0) 
      {
         log_ptr->ustmr               = VSTMR_XO_READ_FULL();
      }
      else
      {
         log_ptr->ustmr               = l1_power_data->ustmr;
      }
      log_ptr->num_active_dl_slots = l1_power_data->num_active_dl_slots;
      log_ptr->num_active_ul_slots = l1_power_data->num_active_ul_slots;
      log_ptr->aeq_status          = l1_power_data->aeq_status;
      log_ptr->power_monitor       = l1_power_data->power_monitor;
      log_ptr->uplink_power        = l1_power_data->uplink_power;
      log_ptr->rxd                 = l1_power_data->rxd;

      gsmdiag_log_commit(&pkt_info);
    }
    else
    {
      MSG_GERAN_LOW_0_G( "Not sending power info" );
    }

    l1_power_data->ustmr = 0;
    l1_power_data->num_active_dl_slots = 0;
    l1_power_data->num_active_ul_slots = 0;
    l1_power_data->aeq_status = 0;
    l1_power_data->power_monitor = 0;
    l1_power_data->uplink_power = 0;
    l1_power_data->rxd = 0;
    l1_power_data->reset = TRUE;
}
#endif
/*  EOF */
