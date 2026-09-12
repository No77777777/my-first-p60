/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file contains the logging section of the GL1 frame layer.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
   Call gl1_hw_log_reset to reset internal state.


Copyright (c) 2005 - 2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_log.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
11/04/17   rv       CR2031827 Excessive F3 log reduction for Oppo
10/08/16   cws      CR1044687 Use ARFCN from pwr sm directly for pwr monitor log packet
30/08/16   tsk      CR1058305: Update to 0x5096 RxD log packet with additional details.
01/06/16   hd       CR1022267 In gl1_hw_log_lif_metrics check maximum no of TS correctly
12/05/16   hd       CR1014598 LIF logging should support maximum 5 PS TSs and 1 CS TSs
03/11/15   sp       CR916957: Update CR909073, log with 0's instead of no logging when yield
26/08/15   npt      CR892374 Generate monitor log when leaving GERAN 
04/08/15   ws       CR884404 add Subcription ID to GSM_RxD_METRICS log packet
01/04/15   jj       CR 816472 DFT log packet  5095 v2 
11/05/15   ws       CR814717 Populate Multisim LOG_GSM_MON_BURST_C log packet using gsmdiag API
17/03/15   cs       CR804196 Use new passthrough versioned FCCH/SCH log packet
02/03/15   ak       CR796460 Apex L1 Monitor Burst Metrics - RSSI does not match ARFCN after power monitor abort
23/02/15   jk       CR793098:LIF additional logging changes
18/02/15   am       CR777094 Keep mon write and read logging-index aligned after mon is aborted
19/01/15   pg       CR783271 Update RxD log packet version when supporting MSRD
15/12/14   cs       CR754853 Add support for Parallel SCH detection handling
16/12/14   jk       CR753423:LIF for Idle,voice and data
10/11/14   ws       CR 740688  Enable DSDS log packet support for L1 FCCH Acquisition,
                    L1 SCH Acquisition and L1 Neighbor Cell Acquisition
23/09/14   jj       CR 728792  Discrete Fourier transform (DFT) spur metric log packet
07/10/14   pg       CR730132 F3 cleanup
06/06/14   jk       CR668058:Adding EPD Log pakcet
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
11/07/14   ws       CR690995 Hybrid AEQ log packet definition and implementation
06/08/14   jj       CR 705509 reducing scell meaurement when BA list is empty in dedicated mode
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
10/07/14   jk       CR688587:GL1 changes to support LIF feature
21/07/14   jk       CR660801:Support for logging VAMOS log packet 0x5099
18/06/14   cos      CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF and FEATURE_GSM_GPRS_QDSP6
19/12/13    jk      CR503359 - GL1 changes to support Rx diversity
22/11/13   cja      CR455527 Remove featurisation from files in API directory
14/10/13   pa       CR539505:Bolt: Added support for Dynamic Power Scheduling
7/16/13    zc       CR470197 Replace memcpy with memscpy
03/07/13   mko      CR503337 Multi-SIM versions of the existing eSAIC metrics log packets
17/05/13   cs       TSTS Updates
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
22/11/11   jj       CR319573 incorporate with review comments
16/11/11   jj       CR 319573   changes for  profiling the modem performance
21/06/11   cja      Featurise 32-bit FCCH freq interface to MDSP
15/06/11   cja      Correct merge error
17/03/11   cja      CR279813 Correct Acquisition SNR logging
27/01/11   cja      Changes for RxLM and TxLM for NikeL
02/03/11   cja      CR276826 For logging 32-bit freq values, saturate to 16-bit
28/02/11   cja      CR276160 Correct test for frequency logging values > 16-bit
12/01/11   cs       CR264690 Acquisition frequency capture range larger
14/10/10   pg       Fix compiler warnings
20/09/10   ws       Fixed Compile error for FEATURE_GSM_MDSP_AEQ_SCH disabled case
06/09/10   ws       Corrected FN offset for AEQ log packets As an offset of 2
                    is only needed for packet transfer mode.
03/09/10   ws       Added FEATURE_GSM_MDSP_AEQ_SCH for QDSP6 targets
05/05/10   pg       Add AEQ metrics logging for QDSP6
04/02/09   ws       Removed leagcy Q4 MDSP services and image for Q6 targets
14/12/09   ws       Merged PACKED_POST changes from MUSTANG to resolve packed
                    struct alignment issues with loging
08/02/10   cja      XO Update
26/05/09   cs       CR167848 fix for expanded afc_freq I/F to mDSP
23/03/09   cs       Updated for new XO/TCXO common 32-bit mDSP F/W
25/02/09   cs       Add in an Ncell asynch rx log packet
08/12/08   cs       Add in the new split gl1_hw_debug.h from gl1_hw.h
22/04/08   cs       CR141866 Add in a new special LPM for 7500
19/05/08   cs       Fix for Klocwork array out of bounds issue
15/01/08   ws       Added suport for ESIAC logging and SAIC AFC in PTM
31/10/07   og       Modify the gsm monitor log packet to accomodate 7 monitors
                    per TDMA frame.
05/03/07   ws       Resolved filtered lint warnings
28/11/06   ws       High Lint warning fixes
01/14/06   gfr      Added three more SAIC metrics, and fixed SAIC FN
12/20/05   gfr      Support for logging SAIC metrics
11/29/05   gfr      Cast log packets to correct types to fix compiler warning
11/04/05   gfr      Change byte to uint8 and word to uint16
08/19/05   gfr      QLint cleanup
04/12/05   gfr      Initial revision created by moving code from gl1_hw_gsm.c
                    and gl1_hw.c
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "gl1_hwi.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "rfm.h"

#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_intf_shared_vars.h"
#include "msg.h"            /* Diagnostics MSGs  */
#include "log.h"            /* LOG PACKET call   */
#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"  /* For GSM log codes */

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "l1_log.h"
#include "l1_sc.h"
#include "l1_sc_int.h"
#include "l1_task.h"
#include "gsmdiag_v.h"
#include "gsmdiag.h"
#include "stringl.h"
#include "gfw_sw_intf.h"
#include "gl1_hw_rxdiversity.h"
#include "l1_utils.h"

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/

#define GL1_LOG_ALLOC( gL1lOGtYPE ) \
    ( gL1lOGtYPE ## _type * )log_alloc( gL1lOGtYPE, \
                                        sizeof( gL1lOGtYPE ## _type ) )

/* -----------FCCH DECODE---------------- */
LOG_RECORD_DEFINE( LOG_GSM_FCCH_DECODE_C )
    uint16                       arfcn;
    mdsp_fcch_log_results_struct data;
LOG_RECORD_END

/* New versioned log packet */
#define LOG_GSM_FCCH_DATA_C_VERSION 1

LOG_RECORD_DEFINE( LOG_GSM_FCCH_DECODE_V_C )
    uint8               version;
    uint8               gas_id;
    uint16              arfcn;
    GfwFcchResultStruct data;
LOG_RECORD_END

/* -----------SCH DECODE---------------- */
LOG_RECORD_DEFINE(LOG_GSM_SCH_DECODE_C)
    uint16                   arfcn;
    GfwSchResultStruct       data;
LOG_RECORD_END

/* New versioned log packet */
#define LOG_GSM_SCH_DATA_C_VERSION 1

LOG_RECORD_DEFINE( LOG_GSM_SCH_DECODE_V_C )
    uint8              version;
    uint8              gas_id;
    uint16             arfcn;
    GfwSchResultStruct data;
LOG_RECORD_END

/* -----------PARALLEL SCH DECODE---------------- */
#define LOG_GSM_PARALLEL_SCH_DATA_C_VERSION 1

LOG_RECORD_DEFINE( LOG_GSM_PARALLEL_SCH_DATA_C )
    uint8              version;
    uint8              gas_id;
    uint16             arfcn;
    GfwPllelSchLogPckt data;
LOG_RECORD_END

/* -----------COMBINED ACQ---------------- */
/* NOTE: Reuse the NCELL_ACQ code for now  */
#define LOG_GSM_COMBINED_ACQ_C LOG_GSM_NCELL_ACQ_C
LOG_RECORD_DEFINE(LOG_GSM_COMBINED_ACQ_C)
    uint8   mode;
    uint8   rank;
    uint16  arfcn;
    dBx16_T rx_power;  /* Only relavent for START/LOST */
LOG_RECORD_END


/* -----------MONITOR---------------- */
typedef PACKED struct PACKED_POST
{
    uint32           FN;
    uint16           arfcn;
    dBx16_T          rx_power;
    uint32           rssi_value;
    rf_gain_range_T  rx_gain_range;
} gl1_hw_monitor_record_T;


#define MONITOR_BLOCKING_FACTOR  (16)
LOG_RECORD_DEFINE(LOG_GSM_MON_BURST_C)
    uint32                   num_records;
    gl1_hw_monitor_record_T  record[MONITOR_BLOCKING_FACTOR];
LOG_RECORD_END


#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
#error code not present
#endif


/* -----------FREQ ADJ---------------- */
LOG_RECORD_DEFINE(LOG_GSM_AFC_ADJUST_C)
    uint32  FN;
    uint8   type;         /* gl1_log_afc_adjust_type */
    int32   pdm_adjust;   /* PDM */
    int32   pdm_value;    /* PDM */
    int32   rot_adjust;   /* Hz */
    int32   rot_value;    /* Hz */
LOG_RECORD_END


/* -----------ASYNC RX--------------- */
LOG_RECORD_DEFINE(LOG_GSM_NCELL_ASYNC_METRICS_C)
   uint16           arfcn;
   uint32           rssi;
   dBx16_T          pwr_dBm_x16;
   rf_measurement_T powerStatus;
   rf_gain_range_T  rx_gain_range;
   int16            dc_offset_i;
   int16            dc_offset_q;
   int16            freq_offset;
   int16            timing_offset;
   uint16           snr;
LOG_RECORD_END


typedef PACKED struct PACKED_POST
{
   uint32  FN;
   uint8   cmd;
} cmd_diag_struct;

#define CELL_CMDS_COUNT 24
LOG_RECORD_DEFINE(LOG_GSM_GL1_HW_CMD_C)
   uint32                     num_cmds;
   cmd_diag_struct   cmds[CELL_CMDS_COUNT];
LOG_RECORD_END


/* Log packet to log the complete mdsp debug buffer - very big! */
#define MDSP_DEBUG_BUFFER_SIZE 512
LOG_RECORD_DEFINE(LOG_GSM_MDSP_DEBUG_BUFFER_C)
   uint32  FN;
   uint8   TSC;
   uint16  debug_buffer[MDSP_DEBUG_BUFFER_SIZE];
LOG_RECORD_END

/* -----------GSTMR_FRAME_NUMBER_ADJ-------- */
LOG_RECORD_DEFINE(LOG_GSM_FN_ADJ_C)
    uint32          oldFN;
    uint32          newFN;
LOG_RECORD_END

/* Log packet to log SAIC metrics from the mDSP debug buffer */
LOG_RECORD_DEFINE(LOG_GSM_DFT_SPUR_METRICS_C)
 uint8 version; // 1 byte
 uint8 gasid;   // 1 byte
 uint16 tTN;    // 2 bytes
 uint32 fFN;    // 4 bytes
 GfwDftSpurAciMetricsStruct DftSpurAciMetrics;
LOG_RECORD_END

#define LOG_GSM_SAIC_METRICS_V2_C_VERSION 1

LOG_RECORD_DEFINE(LOG_GSM_SAIC_METRICS_V2_C)
    uint8                   version;
   uint32 FN;
    uint8                   subID;
   uint16 num_zero_soft_decisions;

   /* The order of the next 8 fields must remain the same as the order
      in the mDSP debug buffer */
   uint16 pre_mimo_complete_snr_iter0;
   uint16 post_mimo_complete_snr_iter0;
   uint16 pre_mimo_complete_svanr_iter1;
   uint16 post_mimo_complete_svanr_iter1;
   uint16 misc_flags;
   uint16 pre_mimo_tsc_snr0;
   uint16 post_mimo_tsc_snr0;
   uint16 reserved;
   uint16 RSSEburstSnr;
   uint16 AEQburstSnr;
   uint16 unused1;
   uint16 unused2;
   uint16 unused3;
   uint16 unused4;
LOG_RECORD_END

#ifdef FEATURE_GSM_MDSP_ESAIC

#define LOG_GPRS_ESAIC_METRICS_V2_C_VERSION 1

LOG_RECORD_DEFINE(LOG_GPRS_ESAIC_METRICS_V2_C)
    uint8                   version;
   uint32 FN;
    uint8                   subID;
   uint16 TN;
   uint16 num_zero_soft_decisions;

   /* The order of the next 8 fields must remain the same as the order
      in the mDSP debug buffer */
   uint16 pre_mimo_complete_snr_iter0;
   uint16 post_mimo_complete_snr_iter0;
   uint16 pre_mimo_complete_svanr_iter1;
   uint16 post_mimo_complete_svanr_iter1;
   uint16 misc_flags;
   uint16 pre_mimo_tsc_snr0;
   uint16 post_mimo_tsc_snr0;
   uint16 reserved;
   uint16 RSSEburstSnr;
   uint16 AEQburstSnr;
   uint16 unused1;
   uint16 unused2;
   uint16 unused3;
   uint16 unused4;
LOG_RECORD_END


#endif

#ifdef FEATURE_GSM_MDSP_AEQ_SCH

#define LOG_GSM_AEQ_SCH_METRICS_V2_C_VERSION 1

LOG_RECORD_DEFINE(LOG_GSM_AEQ_SCH_METRICS_V2_C)
    uint8                   version;
   uint32 FN;
    uint8                   subID;
   uint16 num_zero_soft_decisions;

   /* The order of the next 8 fields must remain the same as the order
      in the mDSP debug buffer */
   uint16 pre_mimo_complete_snr_iter0;
   uint16 post_mimo_complete_snr_iter0;
   uint16 pre_mimo_complete_svanr_iter1;
   uint16 post_mimo_complete_svanr_iter1;
   uint16 misc_flags;
   uint16 pre_mimo_tsc_snr0;
   uint16 post_mimo_tsc_snr0;
   uint16 reserved;
   uint16 RSSEburstSnr;
   uint16 AEQburstSnr;
   uint16 unused1;
   uint16 unused2;
   uint16 unused3;
   uint16 unused4;
LOG_RECORD_END

#endif
/* Number of SAIC metrics to read from the debug buffer */
#define NUM_SAIC_METRICS_WORDS 8

#ifdef FEATURE_GSM_RX_DIVERSITY
/* -----------RxD log packet---------------- */
LOG_RECORD_DEFINE(LOG_GSM_RxD_METRICS_C)
   uint8                 version;
   uint32                FN;
   uint8                 subID;
   gl1_log_chan_type     chan_type;
   dBx16_T               pwr_dBm_x16_prx;   //new field
   dBx16_T               pwr_dBm_x16_divrx;
   rf_gain_range_T       rx_gain_range_prx; //new field
   rf_gain_range_T       rx_gain_range_divrx;
   rf_measurement_T      powerstatus_divrx;
   uint16                jdet_reading_divrx;
   uint32                WBEE_divrx;
   boolean               HLinLLinState_divrx;
   GfwRxdLogPacket       data;  //modified structure
   gl1RxDLogAddnl_t      gl1RxdAddnlData; /* RxD Additional logging details */
LOG_RECORD_END
#endif


#ifdef FEATURE_AEQ_PHASE4
/* -----------AEQ4 log packet---------------- */
LOG_RECORD_DEFINE(LOG_GSM_AEQ4_METRICS_C)
   uint8                 version;
   uint32                FN;
   GfwAeq4LogPacket    data;  //modified structure
LOG_RECORD_END
#endif /*FEATURE_AEQ_PHASE4*/

#ifdef FEATURE_VAMOS_II
/* -----------RxD log packet---------------- */
LOG_RECORD_DEFINE(LOG_GSM_VAMOS_METRICS_C)
   uint8                 version;
   uint32                FN;
   uint8                 UEVamosCapability;
   boolean               IsShiftedSACCH;
   GfwVamosLogType       data;
LOG_RECORD_END
#endif


#ifdef GERAN_L1_ENHANCED_RX
LOG_RECORD_DEFINE(LOG_GSM_LIF_METRICS_C)
   uint8                   version;
   uint8                   gas_id;
   byte                    NoOfSlots;
   gl1_log_lif_params_t    SlotMetrics[MAX_NUMBER_OF_RX_SLOTS];
LOG_RECORD_END
#endif

/* -----------EPD log packet---------------- */
LOG_RECORD_DEFINE(LOG_GSM_EPD_METRICS_C)
   uint8                 version;
   uint32                FN;
   uint8                 gas_id;
   GfwEpdLogPacket       data;
LOG_RECORD_END

#ifdef GERAN_L1_HLLL_LNASTATE
typedef PACKED struct PACKED_POST
{
   uint32           FN;
   uint16           arfcn;
   dBx16_T          rx_power;
   uint16           snr;
   uint16           jdet_reading;
   uint32           WBEE;
   boolean          HLinLLinState;
} gl1_hw_hlll_metrics_T;

#define MAX_NUMBER_OF_CHANNELS   12

LOG_RECORD_DEFINE(LOG_GSM_METRICS_HLLL_STATE_C)
    uint8             version;
    uint32                   num_metrics;
    gl1_hw_hlll_metrics_T    metrics[MAX_NUMBER_OF_CHANNELS];
LOG_RECORD_END
#endif /*GERAN_L1_HLLL_LNASTATE*/

/*===========================================================================

                             LOCAL MACROS

===========================================================================*/


/*===========================================================================

                             LOCAL STORAGE

===========================================================================*/
#ifdef FEATURE_GSM_DYNMC_PWR_MEAS
#define MON_FREQ_LIST_SIZE  (MAX_MEAS_PER_FRAME * GL1_HW_MAX_PWR_MEAS_FRAMES * GL1_HW_PWR_MEAS_STATE_MACHINES)
#else
#if (MAX_MEAS_PER_FRAME==(7))
/* Make sure that storing of arfcns does not overlap the retrieved power
 * monitor values thus corrupting the log packet contents.
 */
#define MON_FREQ_LIST_SIZE  (25)
#else
#define MON_FREQ_LIST_SIZE  (16)
#endif
#endif /*FEATURE_GSM_DYNMC_PWR_MEAS*/


#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
#error code not present
#endif

static LOG_GSM_GL1_HW_CMD_C_type gl1_hw_cmd_log_pkt[NUM_GERAN_DATA_SPACES];

static LOG_GSM_MON_BURST_C_type gl1_hw_mon_pkt[NUM_GERAN_DATA_SPACES];


/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/

void gl1_hw_log_detected_fcch(
      ARFCN_T              arfcn,
      GfwFcchResultStruct *fcch_data_p,
      gas_id_t             gas_id );

void gl1_hw_log_decoded_sch_data(
       ARFCN_T             arfcn,
       GfwSchResultStruct *sch_data_p,
       gas_id_t            gas_id );

void gl1_hw_log_parallel_sch_data(
       ARFCN_T             arfcn,
       GfwPllelSchLogPckt *par_sch_data_p,
       gas_id_t            gas_id );

void gl1_hw_log_gfn_adjust( uint32 old_FN, uint32 FN_diff );

/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION  gl1_hw_log_decoded_sch

DESCRIPTION
  Logs a decoded SCH packet.

===========================================================================*/
void gl1_hw_log_decoded_sch(
       ARFCN_T             arfcn,
       GfwSchResultStruct *sch_data_p,
       gas_id_t            gas_id )
{
   LOG_GSM_SCH_DECODE_C_type  *log_ptr;
   gsmdiag_pkt_info_t         pkt_info;

   l1_log_packet_init(&pkt_info, LOG_GSM_SCH_DECODE_C, sizeof(LOG_GSM_SCH_DECODE_C_type), gas_id);

   /* Try to allocate memory for the log packet */
   log_ptr = (LOG_GSM_SCH_DECODE_C_type  *)gsmdiag_log_alloc(&pkt_info);

   if ( log_ptr  )
   {
      log_ptr->arfcn  =  arfcn_to_word(arfcn);
      log_ptr->data   =  *sch_data_p;
      gsmdiag_log_commit(&pkt_info);
   }
}

/*===========================================================================

FUNCTION  gl1_hw_log_decoded_sch_data

DESCRIPTION
  Logs a decoded SCH packet.

===========================================================================*/
void gl1_hw_log_decoded_sch_data(
       ARFCN_T             arfcn,
       GfwSchResultStruct *sch_data_p,
       gas_id_t            gas_id )
{
  LOG_GSM_SCH_DECODE_V_C_type *log_sch_p;

  if ( !sch_data_p )
  {
    return;
  }

  log_sch_p = GL1_LOG_ALLOC( LOG_GSM_SCH_DECODE_V_C );

  if ( log_sch_p )
  {
    log_sch_p->version = LOG_GSM_SCH_DATA_C_VERSION;
    log_sch_p->gas_id  = gas_id;

    log_sch_p->arfcn   = arfcn_to_word( arfcn );

    log_sch_p->data    = *sch_data_p;

    log_commit( log_sch_p );
  }
}

/*===========================================================================

FUNCTION  gl1_hw_log_decoded_parallel_sch

DESCRIPTION
  Logs a decoded Parallel SCH packet.

===========================================================================*/
void gl1_hw_log_parallel_sch_data(
       ARFCN_T             arfcn,
       GfwPllelSchLogPckt *par_sch_data_p,
       gas_id_t            gas_id )
{
  LOG_GSM_PARALLEL_SCH_DATA_C_type *log_par_sch_p;

  if ( !par_sch_data_p )
  {
    return;
  }

  log_par_sch_p = GL1_LOG_ALLOC( LOG_GSM_PARALLEL_SCH_DATA_C );

  if ( log_par_sch_p )
  {
    log_par_sch_p->version = LOG_GSM_PARALLEL_SCH_DATA_C_VERSION;
    log_par_sch_p->gas_id  = gas_id;

    log_par_sch_p->arfcn   = arfcn_to_word( arfcn );

    log_par_sch_p->data    = *par_sch_data_p;

    log_commit( log_par_sch_p );
  }
}

/*===========================================================================

FUNCTION  gl1_hw_log_async_rx_data

DESCRIPTION
  Logs an async rx metrics packet.

===========================================================================*/
void gl1_hw_log_async_rx_data( gl1_defs_metrics_rpt *rpt,gas_id_t gas_id  )
{
   LOG_GSM_NCELL_ASYNC_METRICS_C_type *log_ptr;

   if ( !rpt )
   {
    return;
   }

   /* Log the result */
   log_ptr = GL1_LOG_ALLOC( LOG_GSM_NCELL_ASYNC_METRICS_C );

   if ( log_ptr )
   {
     log_ptr->arfcn         = arfcn_to_word( rpt->arfcn );
     log_ptr->rssi          = rpt->rssi;
     log_ptr->pwr_dBm_x16   = rpt->pwr_dBm_x16;
     log_ptr->powerStatus   = rpt->powerStatus;
     log_ptr->rx_gain_range = rpt->rx_gain_range;
     log_ptr->dc_offset_i   = rpt->dc_offset_i;
     log_ptr->dc_offset_q   = rpt->dc_offset_q;
     log_ptr->freq_offset   = rpt->freq_offset;
     log_ptr->timing_offset = rpt->timing_offset;
     log_ptr->snr           = rpt->snr;

     log_commit( (void *)log_ptr );
   }
}

/*===========================================================================

FUNCTION  gl1_hw_log_detected_tone

DESCRIPTION
  This function sends a detected tone log packet.

===========================================================================*/
void gl1_hw_log_detected_tone(
      ARFCN_T              arfcn,
      GfwFcchResultStruct *fcch_data_p,
      gas_id_t             gas_id )
{
   LOG_GSM_FCCH_DECODE_C_type *log_ptr;
   gsmdiag_pkt_info_t pkt_info;

   if ( !fcch_data_p )
   {
    return;
   }

   l1_log_packet_init(&pkt_info, LOG_GSM_FCCH_DECODE_C, sizeof(LOG_GSM_FCCH_DECODE_C_type), gas_id);

   /* Try to allocate memory for the log packet */
   log_ptr = (LOG_GSM_FCCH_DECODE_C_type  *)gsmdiag_log_alloc(&pkt_info);

   if (log_ptr != NULL)
   {
      log_ptr->arfcn            = arfcn_to_word(arfcn);
      log_ptr->data.id          = fcch_data_p->toneCount;
      log_ptr->data.offset_h    = ((fcch_data_p->toneStartPosition >> 16) & 0xffff);
      log_ptr->data.offset_l    = fcch_data_p->toneStartPosition & 0xffff ;

      /*
       * As we only have 16-bits to log then use largest change
       * which will not be exactly accurate but will work
       * until we get QXDM/APEX parser updated
       */
      if (fcch_data_p->coarseFreqEstimate > MAX_INT16)
      {
         log_ptr->data.coarse_freq    = MAX_INT16;
      }
      else
      {
         if (fcch_data_p->coarseFreqEstimate < (0 - MAX_INT16))
         {
            log_ptr->data.coarse_freq    = MAX_NEG_UINT16;
         }
         else
         {
            log_ptr->data.coarse_freq = (int16)(fcch_data_p->coarseFreqEstimate & 0xFFFF);
         }
      }
      /*
       * As we only have 16-bits to log then use largest change
       * which will not be exactly accurate but will work
       * until we get QXDM/APEX parser updated
       */
      if (fcch_data_p->fineFreqEstimate > MAX_INT16)
      {
         log_ptr->data.fine_freq    = MAX_INT16;
      }
      else
      {
         if (fcch_data_p->fineFreqEstimate < (0 - MAX_INT16))
         {
            log_ptr->data.fine_freq    = MAX_NEG_UINT16;
         }
         else
         {
            log_ptr->data.fine_freq = (int16)(fcch_data_p->fineFreqEstimate & 0xFFFF);
         }
      }

      /*
       * As we only have 16-bits to log then use largest change
       * which will not be exactly accurate but will work
       * until we get QXDM/APEX parser updated
       */
      if (fcch_data_p->afcFreqEst > MAX_INT16)
      {
         log_ptr->data.afc_freq    = MAX_INT16;
      }
      else
      {
         if (fcch_data_p->afcFreqEst < (0 - MAX_INT16))
         {
            log_ptr->data.afc_freq    = MAX_NEG_UINT16;
         }
         else
         {
            log_ptr->data.afc_freq = (int16)(fcch_data_p->afcFreqEst & 0xFFFF);
         }
      }

      log_ptr->data.snr         = fcch_data_p->snrEst;
      gsmdiag_log_commit(&pkt_info);
   }
}

/*===========================================================================

FUNCTION  gl1_hw_log_detected_fcch

DESCRIPTION
  This function sends a detected FCCH log packet.

===========================================================================*/
void gl1_hw_log_detected_fcch(
      ARFCN_T              arfcn,
      GfwFcchResultStruct *fcch_data_p,
      gas_id_t             gas_id )
{
  LOG_GSM_FCCH_DECODE_V_C_type *log_fcch_p;

  if ( !fcch_data_p )
  {
    return;
  }

  log_fcch_p = GL1_LOG_ALLOC( LOG_GSM_FCCH_DECODE_V_C );

  if ( log_fcch_p )
  {
    log_fcch_p->version = LOG_GSM_FCCH_DATA_C_VERSION;
    log_fcch_p->gas_id  = gas_id;

    log_fcch_p->arfcn   = arfcn_to_word( arfcn );

    log_fcch_p->data    = *fcch_data_p;

    log_commit( log_fcch_p );
  }
}

/*===========================================================================

FUNCTION gl1_hw_log_monitor

DESCRIPTION
  Logs monitor bursts.

===========================================================================*/
void gl1_hw_log_monitor
(
   dBx16_T          rx_power,
   uint32           rssi_value,
   rf_gain_range_T  rx_gain_range,
   ARFCN_T          arfcn,
   boolean          rx_yield,   
   boolean          immediate_log,
   gas_id_t         gas_id
)
{
   gl1_hw_monitor_record_T *rec;
   uint32 current_FN = GSTMR_GET_FN_GERAN(gas_id);
   ARFCN_T arfcn_to_log;
   gsmdiag_pkt_info_t pkt_info;
   LOG_GSM_MON_BURST_C_type *log_ptr;


   /* Sanity check */
   if ( gl1_hw_mon_pkt[gas_id].num_records >= MONITOR_BLOCKING_FACTOR )
   {
      MSG_GERAN_ERROR_1_G("Crazy value!! mon_pkt.num_recs=%u",gl1_hw_mon_pkt[gas_id].num_records);
      gl1_hw_mon_pkt[gas_id].num_records = 0;
   }

#ifdef FEATURE_GSM_DED_SCELL_MONITOR

   if ( (rx_power == 0)&&(rssi_value ==0)&& (ba_list_empty))
   {
       return;
   }
#endif /*  #define FEATURE_GSM_DED_SCELL_MONITOR */

   /* Get a pointer to the next record */
   rec = &gl1_hw_mon_pkt[gas_id].record[gl1_hw_mon_pkt[gas_id].num_records];
   arfcn_to_log = arfcn;
   /* Record sample data in the log buffer */
   rec->FN            = SUB_FN(current_FN, 1);
   rec->arfcn         = arfcn_to_word(arfcn_to_log);
   if (!rx_yield)	   
   {
	   rec->rx_power	  = rx_power;
	   rec->rssi_value	  = rssi_value;
	   rec->rx_gain_range = rx_gain_range;
	}
   else
	{  
	   MSG_GERAN_HIGH_1_G("Monitor yield, invalidate RSSI arfcn:%d", arfcn_to_log.num);
	   rec->rx_power	  = 0;
	   rec->rssi_value	  = 0;
	   rec->rx_gain_range = GAIN_RANGE_1;
	}

#ifdef DEBUG_FEATURE_GSM_DED_SCELL_MONITOR
   MSG_GERAN_MED_3_G("Pwr meas log pkt arfcn %d power %d FN %d", arfcn_to_log.num  , rec->rx_power , rec->FN );
#endif /*DEBUG_FEATURE_GSM_DED_SCELL_MONITOR */
   /* increment log counter & submit if it's the last sample */
   gl1_hw_mon_pkt[gas_id].num_records++;
   if ((gl1_hw_mon_pkt[gas_id].num_records == MONITOR_BLOCKING_FACTOR) || (immediate_log == TRUE))
   {
      l1_log_packet_init(&pkt_info, LOG_GSM_MON_BURST_C, sizeof(LOG_GSM_MON_BURST_C_type), gas_id);

      /* Try to allocate memory for the log packet */
      log_ptr = (LOG_GSM_MON_BURST_C_type  *)gsmdiag_log_alloc(&pkt_info);
      if (log_ptr !=NULL)
      {
        log_ptr->num_records = gl1_hw_mon_pkt[gas_id].num_records;
        memscpy(&(log_ptr->record),sizeof(log_ptr->record),&(gl1_hw_mon_pkt[gas_id].record),sizeof(gl1_hw_mon_pkt[gas_id].record));
        gsmdiag_log_commit(&pkt_info);
      }

      /* Reset log counter */
      gl1_hw_mon_pkt[gas_id].num_records = 0;
   }
}

/*===========================================================================

FUNCTION gl1_hw_log_comb_acq

DESCRIPTION
  Logs combined acquisition results

===========================================================================*/
void gl1_hw_log_comb_acq (uint8 mode, ARFCN_T arfcn, dBx16_T rx_power,gas_id_t gas_id)
{
   LOG_GSM_COMBINED_ACQ_C_type  *log_ptr;
   gsmdiag_pkt_info_t           pkt_info;

   l1_log_packet_init(&pkt_info, LOG_GSM_COMBINED_ACQ_C, sizeof(LOG_GSM_COMBINED_ACQ_C_type), gas_id);

   /* Try to allocate memory for the log packet */
   log_ptr = (LOG_GSM_COMBINED_ACQ_C_type  *)gsmdiag_log_alloc(&pkt_info);

   if (log_ptr)
   {
      log_ptr->mode     =  mode;
      log_ptr->rank     =  0;         /* Not known or applicable */
      log_ptr->arfcn    =  arfcn_to_word(arfcn);
      log_ptr->rx_power =  rx_power;
      gsmdiag_log_commit(&pkt_info);
   }
}

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
#error code not present
#endif

/*===========================================================================
FUNCTION gl1_log_afc_adjust

DESCRIPTION
  This function is called to log any adjustments made through AFC.

===========================================================================*/
void gl1_log_afc_adjust
(
  int32 pdm_adjust,
  int32 rot_adjust,
  gl1_log_afc_adjust_type type,
  gas_id_t gas_id
)
{
   LOG_GSM_AFC_ADJUST_C_type *log_ptr;

   /* Do not process empty logs */
   if ( ( pdm_adjust == 0 ) && ( rot_adjust == 0 ) )
   {
     return;
   }

   log_ptr = GL1_LOG_ALLOC( LOG_GSM_AFC_ADJUST_C );

   if ( log_ptr )
   {
      log_ptr->FN         =  GSTMR_GET_FN_GERAN(gas_id);
      log_ptr->type       =  (uint8)type;

      /* In XO we highjack the PDM field for logging */
      log_ptr->pdm_adjust =  pdm_adjust;
      log_ptr->pdm_value  =  gl1_hw_get_xo_acc_freq_err(gas_id);

      log_ptr->rot_adjust =  gl1_hw_phasetohz( rot_adjust );
      log_ptr->rot_value  =  gl1_hw_phasetohz( gl1_hw_get_rotator_val(gas_id) );

      log_commit((void * )log_ptr);
   }
}

/*===========================================================================
FUNCTION gl1_log_cell_cmds_to_diag

DESCRIPTION
  This function is called to log any adjustments made through AFC.

===========================================================================*/
void gl1_hw_log_cell_cmds_to_diag( gl1_hw_cmd_types cmd_type, boolean dump,gas_id_t gas_id )
{
   /* Stamp packet header during the first sample */
   if ( gl1_hw_cmd_log_pkt[gas_id].num_cmds == 0 )
   {
      log_set_length   ( &gl1_hw_cmd_log_pkt[gas_id], sizeof( LOG_GSM_GL1_HW_CMD_C_type ) );
      log_set_code     ( &gl1_hw_cmd_log_pkt[gas_id], LOG_GSM_GL1_HW_CMD_C );
      log_set_timestamp( &gl1_hw_cmd_log_pkt[gas_id] );
   }

   gl1_hw_cmd_log_pkt[gas_id].cmds[gl1_hw_cmd_log_pkt[gas_id].num_cmds].cmd = (uint8)cmd_type;
   gl1_hw_cmd_log_pkt[gas_id].cmds[gl1_hw_cmd_log_pkt[gas_id].num_cmds].FN  = GSTMR_GET_FN_GERAN(gas_id);
   gl1_hw_cmd_log_pkt[gas_id].num_cmds++;

   if ( dump || ( gl1_hw_cmd_log_pkt[gas_id].num_cmds >= CELL_CMDS_COUNT ) )
   {
      (void)log_submit( (void *)&gl1_hw_cmd_log_pkt[gas_id] );
      gl1_hw_cmd_log_pkt[gas_id].num_cmds = 0;
   }
}

/*===========================================================================

FUNCTION gl1_hw_log_mdsp_debug_buffer

DESCRIPTION
   Grabs and logs the mdsp debug buffer.

===========================================================================*/
void gl1_hw_log_mdsp_debug_buffer (uint8 TSC,gas_id_t gas_id)
{
   LOG_GSM_MDSP_DEBUG_BUFFER_C_type *log_ptr;

   log_ptr = GL1_LOG_ALLOC( LOG_GSM_MDSP_DEBUG_BUFFER_C );

   if ( log_ptr )
   {
      log_ptr->FN = GSTMR_GET_FN_GERAN(gas_id);
      log_ptr->TSC = TSC;
      (void) gl1_hw_get_debug_data
              (
                (uint16 *)log_ptr->debug_buffer,
                0,  /* offset */
                MDSP_DEBUG_BUFFER_SIZE,
                gas_id
              );
      log_commit(log_ptr);
   }
}

/*===========================================================================

FUNCTION gl1_hw_log_gfn_adjust

DESCRIPTION
   Logging for GFN adjustment
===========================================================================*/
void gl1_hw_log_gfn_adjust( uint32 old_FN, uint32 FN_diff )
{
   LOG_GSM_FN_ADJ_C_type *log_ptr = GL1_LOG_ALLOC( LOG_GSM_FN_ADJ_C );

   if ( log_ptr )
   {
      log_ptr->oldFN = old_FN;
      log_ptr->newFN = FN_diff;

      log_commit( (void *)log_ptr );
   }

   return;
}

/*===========================================================================

FUNCTION gl1_hw_log_saic_metrics

DESCRIPTION
   Grabs and logs the mdsp SAIC metrics.  Should be called during burst
   metrics processing.

===========================================================================*/
void gl1_hw_log_saic_metrics (gas_id_t gas_id)
{
   LOG_GSM_SAIC_METRICS_V2_C_type *log_ptr;
   gsmdiag_pkt_info_t pkt_info;
   uint32 current_fn;

   /* Try to allocate memory for the log packet */
   l1_log_packet_init(&pkt_info, LOG_GSM_SAIC_METRICS_V2_C, sizeof(LOG_GSM_SAIC_METRICS_V2_C_type), gas_id);
   log_ptr = (LOG_GSM_SAIC_METRICS_V2_C_type *)gsmdiag_log_alloc(&pkt_info);

   if ( !log_ptr )
   {
     return;
   }

   current_fn = GSTMR_GET_FN_GERAN(gas_id);

   log_ptr->version = LOG_GSM_SAIC_METRICS_V2_C_VERSION;
   /* Subtract 1 from the FN to get the frame when the burst happened.
      Note this will need to change for GPRS since the offset is not 1. */

   log_ptr->FN = SUB_FN(current_fn, 1);
   log_ptr->subID = (uint8)gas_id;


   /* Send the log packet */
   gsmdiag_log_commit(&pkt_info);
}

#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION gl1_hw_log_gprs_esaic_metrics

DESCRIPTION
   Grabs and logs the mdsp SAIC metrics.  Should be called during burst
   metrics processing.

===========================================================================*/
void gl1_hw_log_gprs_esaic_metrics (uint16 timeslot,gas_id_t gas_id)
{
   LOG_GPRS_ESAIC_METRICS_V2_C_type *log_ptr;
   gsmdiag_pkt_info_t pkt_info;
   uint32 current_fn;

   /* Try to allocate memory for the log packet */
   l1_log_packet_init(&pkt_info, LOG_GPRS_ESAIC_METRICS_V2_C, sizeof(LOG_GPRS_ESAIC_METRICS_V2_C_type), gas_id);
   log_ptr = (LOG_GPRS_ESAIC_METRICS_V2_C_type *)gsmdiag_log_alloc(&pkt_info);

   if ( !log_ptr )
   {
     return;
   }

   current_fn = GSTMR_GET_FN_GERAN(gas_id);
   log_ptr->version = LOG_GPRS_ESAIC_METRICS_V2_C_VERSION;

   /* Subtract 2 from the FN to get the frame when the burst happened. */

   log_ptr->FN = SUB_FN(current_fn, 2);
   log_ptr->subID = (uint8)gas_id;
   log_ptr->TN = timeslot;


   /* Send the log packet */
   gsmdiag_log_commit(&pkt_info);
}

#endif /* FEATURE_GSM_MDSP_ESAIC */

/*===========================================================================

FUNCTION gl1_hw_log_aeq_metrics

DESCRIPTION
   Grabs and logs the mdsp AEQ metrics.  Should be called during burst
   metrics processing.

===========================================================================*/
void gl1_hw_log_aeq_metrics (uint16 timeslot, uint16 tag,boolean packet_mode,gas_id_t gas_id)
{
   /* Need to change to ESAIC, or preferably a new AEQ log packet */
   LOG_GSM_SAIC_METRICS_V2_C_type *log_ptr_cs;
   LOG_GPRS_ESAIC_METRICS_V2_C_type *log_ptr_ps;
   uint32 current_fn;
   uint16 buf_index, buf_num;
   volatile GfwMetricsStruct *gfwMetricsBuffer = NULL;
   volatile GfwAeqLogPacket *gfwAeqLogPacket = NULL;

   current_fn = GSTMR_GET_FN_GERAN(gas_id);

   if (tag != MDSP_INVALID_TAG)
   {
      /* Calculate where to find the data */
#ifdef FEATURE_GSM_GPRS_MSC33
      buf_num =   tag / MDSP_MAX_DL_TS;
      buf_index = tag % MDSP_MAX_DL_TS;
#else
      buf_num =   tag / 4;
      buf_index = tag % 4;
#endif

     gfwMetricsBuffer = (volatile GfwMetricsStruct*)(mdsp_intf_shared_var_read(GFWMETRICSBUFFERS, gas_id)) + buf_num;
     gfwAeqLogPacket = &(gfwMetricsBuffer->gfwBurstMetricsBuffers[buf_index].aeqLogPacket);
   }

   /* check which log to use based on if we are in packet transfer or not */
   if(packet_mode)
   {

     log_ptr_ps = GL1_LOG_ALLOC( LOG_GPRS_ESAIC_METRICS_V2_C );

     if ( !log_ptr_ps )
     {
       return;
     }

     log_ptr_ps->version = LOG_GPRS_ESAIC_METRICS_V2_C_VERSION;

     /* Subtract 2 from the FN to get the frame when the burst happened. */
     log_ptr_ps->FN = SUB_FN(current_fn, 2);
     log_ptr_ps->subID = (uint8)gas_id;
     log_ptr_ps->TN = timeslot;

     if (gfwAeqLogPacket != NULL)
     {
       memscpy(&log_ptr_ps->pre_mimo_complete_snr_iter0,
               (sizeof(LOG_GPRS_ESAIC_METRICS_V2_C_type) - sizeof(log_ptr_ps->version) - sizeof(log_ptr_ps->FN)- sizeof(log_ptr_ps->subID) - sizeof(log_ptr_ps->TN) - sizeof(log_ptr_ps->num_zero_soft_decisions)),
               (void *)gfwAeqLogPacket,
               sizeof(GfwAeqLogPacket));
     }

     /* Send the log packet */
     log_commit(log_ptr_ps);

   }else
   {

     log_ptr_cs = GL1_LOG_ALLOC( LOG_GSM_SAIC_METRICS_V2_C );

     if ( !log_ptr_cs )
     {
       return;
     }

     log_ptr_cs->version = LOG_GSM_SAIC_METRICS_V2_C_VERSION;
     /* Subtract 1 from the FN to get the frame when the burst happened. */
     log_ptr_cs->FN = SUB_FN(current_fn, 1);
     log_ptr_cs->subID = (uint8)gas_id;
     if (gfwAeqLogPacket != NULL)
     {
        memscpy (&log_ptr_cs->pre_mimo_complete_snr_iter0,
                (sizeof(LOG_GSM_SAIC_METRICS_V2_C_type) - sizeof(log_ptr_cs->version) - sizeof(log_ptr_cs->FN) - sizeof(log_ptr_cs->subID) - sizeof(log_ptr_cs->num_zero_soft_decisions)),
                 (void *)gfwAeqLogPacket,
                 sizeof(GfwAeqLogPacket));
     }

     /* Send the log packet */
     log_commit(log_ptr_cs);
   }

}

#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================
FUNCTION gl1_hw_set_rxqual_full

DESCRIPTION
  Set RxQualhist value using quantized rxqual_full in gl1RxdAddnlData.
  rxqual_full = 0 means good quality.
  rxqual_full = 7 means bad quality.
===========================================================================*/
void gl1_hw_set_rxqual_full(uint8 rxqual_full, gas_id_t gas_id)
{
  gl1RxDLogAddnl_t *gl1RxdAddnlDataPtr = &gl1RxdAddnlData[gas_id];
  gl1RxdAddnlDataPtr->RxQualhist = rxqual_full;
}

/*===========================================================================
FUNCTION gl1_hw_set_HardRxQual

DESCRIPTION
  Set HardRxQual value using raw HardRxQual in gl1RxdAddnlData.
  HardRxQual = 32767 means good quality.
===========================================================================*/
void gl1_hw_set_HardRxQual(uint16 HardRxQual, gas_id_t gas_id)
{
  gl1RxDLogAddnl_t *gl1RxdAddnlDataPtr = &gl1RxdAddnlData[gas_id];
  gl1RxdAddnlDataPtr->HardRxQual = HardRxQual;
}

/*===========================================================================
FUNCTION gl1_hw_set_mean_bep

DESCRIPTION
  Set BEP value using mean BEP in gl1RxdAddnlData.
===========================================================================*/
void gl1_hw_set_mean_bep(uint32 mean_bep, gas_id_t gas_id)
{
  gl1RxDLogAddnl_t *gl1RxdAddnlDataPtr = &gl1RxdAddnlData[gas_id];
  gl1RxdAddnlDataPtr->BEP = mean_bep;
}

/*===========================================================================
FUNCTION gl1_hw_set_fwakeup_rxd_log

DESCRIPTION
  Set BEP value using mean BEP in gl1RxdAddnlData.
===========================================================================*/
void gl1_hw_set_fwakeup_rxd_log(boolean forcedwakeup, gas_id_t gas_id)
{
 gl1RxDLogAddnl_t *gl1RxdAddnlDataPtr = &gl1RxdAddnlData[gas_id];
 gl1RxdAddnlDataPtr->IdleforceRet = forcedwakeup;
}

/*===========================================================================
FUNCTION gl1_hw_get_addln_rxd_metrics

DESCRIPTION
   Populates additional RxD log data structure that will be used in RxD
   log packet 0x5096.

===========================================================================*/
gl1RxDLogAddnl_t *gl1_hw_get_addln_rxd_metrics(gas_id_t gas_id)
{
  static uint16 prev_rxqual = RXD_INVALID_RXQUAL;
  uint16 rxqual_full = RXD_INVALID_RXQUAL;
  gl1_rxd_control_type rxdNv;
  gl1RxDLogAddnl_t *gl1RxdAddnlDataPtr = &gl1RxdAddnlData[gas_id];

  /* Check if the pointer is not  NULL and update individual fields. */
  if (gl1RxdAddnlDataPtr != NULL)
  {
    gl1RxdAddnlDataPtr->xCCHTimerVal =  l1_idle_saic_get_timer_remaining(gas_id);
    rxdNv = gl1_get_rxd_control_flags(gas_id);
    memscpy (&gl1RxdAddnlDataPtr->RxDNVValues, sizeof(uint8), &rxdNv, sizeof(uint8));
    rxqual_full = gl1RxdAddnlDataPtr->RxQualhist & 0xFF;
    gl1RxdAddnlDataPtr->RxQualhist = (uint16)((prev_rxqual & 0xFF) << 0x8) | rxqual_full;

    /* Keep track of last rxqual to update into the log packet */
    if ((prev_rxqual != rxqual_full)
        && (rxqual_full != RXD_INVALID_RXQUAL)
        && (prev_rxqual != RXD_INVALID_RXQUAL))
    {
      prev_rxqual = rxqual_full;
    }
  }

  return gl1RxdAddnlDataPtr;
}

/*===========================================================================

FUNCTION gl1_hw_log_rxd_metrics

DESCRIPTION
   Grabs and logs the mdsp Rx Diversity metrics.  Should be called during burst
   metrics processing.

===========================================================================*/
void gl1_hw_log_rxd_metrics(dBx16_T pwr_dBm_x16_prx,dBx16_T pwr_dBm_x16_rxd,
                                 rf_gain_range_T rx_gain_range_prx,rf_gain_range_T rx_gain_range_rxd,
                                 rf_measurement_T powerstatus_rxd, GfwRxdLogPacket *gfwRxdLogPacket,
                                 gl1_log_chan_type log_chan_type, gas_id_t gas_id)
{
   LOG_GSM_RxD_METRICS_C_type *log_ptr_cs;
   uint32 current_fn;

   log_ptr_cs = GL1_LOG_ALLOC( LOG_GSM_RxD_METRICS_C );

   if ( !log_ptr_cs )
   {
     return;
   }

   current_fn = GSTMR_GET_FN_GERAN(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   log_ptr_cs->version             = 5;
#else
   log_ptr_cs->version             = 4;
#endif
   log_ptr_cs->subID = (uint8) gas_id;

#ifdef FEATURE_GSM_RX_DIVERSITY_ADDL
   log_ptr_cs->version             = 6;
#endif

   /* Subtract 1 from the FN to get the frame when the burst happened. */
   log_ptr_cs->FN                  = SUB_FN(current_fn, 1);
   log_ptr_cs->chan_type           = log_chan_type;
   log_ptr_cs->pwr_dBm_x16_prx     = pwr_dBm_x16_prx;
   log_ptr_cs->pwr_dBm_x16_divrx   = pwr_dBm_x16_rxd;
   log_ptr_cs->rx_gain_range_prx   = rx_gain_range_prx;
   log_ptr_cs->rx_gain_range_divrx = rx_gain_range_rxd;
   log_ptr_cs->powerstatus_divrx   = powerstatus_rxd;
   log_ptr_cs->jdet_reading_divrx  = 0;
   log_ptr_cs->WBEE_divrx          = 0;
   log_ptr_cs->HLinLLinState_divrx = 0;

   if (gfwRxdLogPacket != NULL)
     {
     log_ptr_cs->data = *gfwRxdLogPacket;
     }

   log_ptr_cs->gl1RxdAddnlData = *gl1_hw_get_addln_rxd_metrics(gas_id);
 //  log_ptr_cs->data.rxdMisc.aeq4Enabled = gl1_hw_get_saic_state(gas_id);
   /* Send the log packet */
   log_commit(log_ptr_cs);

}
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef GERAN_L1_HLLL_LNASTATE
/*===========================================================================

FUNCTION gl1_hw_log_HLin_LLin_metrics

DESCRIPTION
  Logs monitor bursts.

===========================================================================*/
void gl1_hw_log_HLin_LLin_metrics( gl1_defs_metrics_lnastate *gl1_metrics_lnastate,uint8 *SawlessHLLLState, gas_id_t gas_id)
{
   LOG_GSM_METRICS_HLLL_STATE_C_type *log_ptr_cs;
   uint32 current_fn;
   uint32 num_channels = 5;
   uint32 i = 0;

   if ( !gl1_metrics_lnastate || !SawlessHLLLState )
   {
     return;
   }

   if(gl1_metrics_lnastate->NoOfChannels > MAX_NUMBER_OF_CHANNELS)
    {
    MSG_GERAN_ERROR_2_G("HLinLLin Logging NoOfChannels:%d greater than max number of channels supported:%d",gl1_metrics_lnastate->NoOfChannels,
                        MAX_NUMBER_OF_CHANNELS);
    return;
    }

   log_ptr_cs = GL1_LOG_ALLOC( LOG_GSM_METRICS_HLLL_STATE_C );

   if ( !log_ptr_cs )
   {
       MSG_GERAN_HIGH_0_G("LOG_GSM_METRICS_HLLL_STATE_C MEMORY NOT ASSIGNED");
       return;
   }

   current_fn = GSTMR_GET_FN_GERAN(gas_id);

   log_ptr_cs->version             = 1;
   log_ptr_cs->num_metrics         = gl1_metrics_lnastate->NoOfChannels;

   for(i=0;i<gl1_metrics_lnastate->NoOfChannels;i++)
    {
    /* Subtract 1 from the FN to get the frame when the burst happened. */
    log_ptr_cs->metrics[i].FN                  = SUB_FN(current_fn, 1);
    log_ptr_cs->metrics[i].arfcn               = arfcn_to_word(gl1_metrics_lnastate->gsm_metrics[i].arfcn);
    log_ptr_cs->metrics[i].rx_power            = gl1_metrics_lnastate->gsm_metrics[i].rx_lvl_dbm;
    log_ptr_cs->metrics[i].snr                 = gl1_metrics_lnastate->gsm_metrics[i].snr;
    log_ptr_cs->metrics[i].jdet_reading        = gl1_metrics_lnastate->gsm_metrics[i].jdet_value;
    log_ptr_cs->metrics[i].HLinLLinState       = SawlessHLLLState[i];
    log_ptr_cs->metrics[i].WBEE                = 0;
    }

   for(i=num_channels;i<MAX_NUMBER_OF_CHANNELS;i++)
    {
    /* Subtract 1 from the FN to get the frame when the burst happened. */
    log_ptr_cs->metrics[i].FN                  = 0;
    log_ptr_cs->metrics[i].arfcn               = 0;
    log_ptr_cs->metrics[i].rx_power            = 0;
    log_ptr_cs->metrics[i].snr                 = 0;
    log_ptr_cs->metrics[i].jdet_reading        = 0;
    log_ptr_cs->metrics[i].HLinLLinState       = 0;
    log_ptr_cs->metrics[i].WBEE                = 0;
    }

   /* Send the log packet */
   log_commit(log_ptr_cs);
}
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
/*===========================================================================

FUNCTION gl1_hw_log_lif_metrics

DESCRIPTION
   Grabs and logs the lif metrics Should be called during burst
   metrics processing.

===========================================================================*/
void  gl1_hw_log_lif_metrics(gl1_log_lif_metrics_t *log_lif_metrics,gas_id_t gas_id)
{
    LOG_GSM_LIF_METRICS_C_type *log_ptr_cs = GL1_LOG_ALLOC( LOG_GSM_LIF_METRICS_C );
    uint32 current_fn;
    uint8 i =0;

    if ( !log_lif_metrics )
    {
      return;
    }

    if( log_lif_metrics->NoOfSlots > MAX_NUMBER_OF_RX_SLOTS)
    {
      MSG_GERAN_ERROR_2_G("LIF Logging NoOfSlots:%d greater than max number of slots supported:%d",log_lif_metrics->NoOfSlots,
                                                                                                   MAX_NUMBER_OF_RX_SLOTS);
      return;
    }


    if ( !log_ptr_cs )
    {
      return;
    }

    current_fn = GSTMR_GET_FN_GERAN(gas_id);

    log_ptr_cs->version             = 3;
    log_ptr_cs->gas_id              = gas_id;
    log_ptr_cs->NoOfSlots           = log_lif_metrics->NoOfSlots;

    for(i=0;i<log_ptr_cs->NoOfSlots;i++)
    {
      memset(&log_ptr_cs->SlotMetrics[i],0,sizeof(gl1_log_lif_params_t));
    /* Subtract 2 from the FN in case packet channel and 1 otherwise to get the frame when the burst happened. */
      log_ptr_cs->SlotMetrics[i] = log_lif_metrics->gl1_log_lif_params[i];

      if( (log_lif_metrics->gl1_log_lif_params[i].channel_type == GL1_MSG_PS_PACCH_PDTCH) ||
          (log_lif_metrics->gl1_log_lif_params[i].channel_type == GL1_MSG_PS_PTCCH) )
      {
        log_ptr_cs->SlotMetrics[i].FN = SUB_FN(current_fn, 2);
      }
      else
      {
        log_ptr_cs->SlotMetrics[i].FN = SUB_FN(current_fn, 1);
      }
    }

    if(i < MAX_NUMBER_OF_RX_SLOTS)
    {
      memset(&log_ptr_cs->SlotMetrics[i],0,sizeof(gl1_log_lif_params_t) * (MAX_NUMBER_OF_RX_SLOTS - i));
    }

    /* Send the log packet */
    log_commit(log_ptr_cs);
}
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef FEATURE_VAMOS_II
void gl1_hw_log_vamos2_metrics(uint8 gl1_vamos_support,boolean ShiftedSacchEn,GfwVamosLogType *VamosLogPacket,gas_id_t gas_id)
{
    LOG_GSM_VAMOS_METRICS_C_type *log_ptr_cs;
    uint32 current_fn;

    log_ptr_cs = GL1_LOG_ALLOC( LOG_GSM_VAMOS_METRICS_C );

    if ( !log_ptr_cs )
    {
      MSG_GERAN_LOW_0_G("LOG_GSM_VAMOS_METRICS_C NULL pointer");
      return;
    }

    current_fn = GSTMR_GET_FN_GERAN(gas_id);

    log_ptr_cs->version             = 1;
    /* Subtract 1 from the FN to get the frame when the burst happened. */
    log_ptr_cs->FN                  = SUB_FN(current_fn, 1);
    log_ptr_cs->UEVamosCapability   = gl1_vamos_support;
    log_ptr_cs->IsShiftedSACCH      = ShiftedSacchEn;
    if(VamosLogPacket != NULL)
     {
     log_ptr_cs->data = *VamosLogPacket;
     }

    /* Send the log packet */
    log_commit(log_ptr_cs);
}
#endif /*FEATURE_VAMOS_II*/

/*===========================================================================

FUNCTION gl1_log_epd_metrics

DESCRIPTION
   Log EPD metrics if paging channel was decoded using Single burst decode.

===========================================================================*/
void gl1_log_epd_metrics(GfwEpdLogPacket *gfwEPDLogPacket,gas_id_t gas_id)
{
   LOG_GSM_EPD_METRICS_C_type *log_ptr_cs;
   uint32 current_fn;

   if ( !gfwEPDLogPacket )
   {
    return;
   }

   log_ptr_cs = GL1_LOG_ALLOC( LOG_GSM_EPD_METRICS_C );
   current_fn = GSTMR_GET_FN_GERAN(gas_id);

   if ( !log_ptr_cs )
   {
      MSG_GERAN_LOW_0_G("EPD Log Packet not assigned");
      return;
   }

   log_ptr_cs->version  = 1;
   /* Subtract 1 from the FN to get the frame when the burst happened. */
   log_ptr_cs->FN     = SUB_FN(current_fn, 1);
   log_ptr_cs->gas_id = (uint8)gas_id;
   if (gfwEPDLogPacket != NULL)
     {
      log_ptr_cs->data = *gfwEPDLogPacket;
     }
   /* Send the log packet */
   log_commit(log_ptr_cs);
}

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
/*===========================================================================

FUNCTION gl1_hw_log_aeq_sch_metrics

DESCRIPTION
   Grabs and logs the mdsp AEQ metrics.  Should be called during burst
   metrics processing.

===========================================================================*/
void gl1_hw_log_aeq_sch_metrics (GfwBurstMetricsStruct *metrics,gas_id_t gas_id)
{
   /* Need to change to ESAIC, or preferably a new AEQ log packet */
   LOG_GSM_AEQ_SCH_METRICS_V2_C_type *log_ptr;
   uint32 current_fn;

   log_ptr = GL1_LOG_ALLOC( LOG_GSM_AEQ_SCH_METRICS_V2_C );

   if ( !log_ptr )
   {
     return;
   }

   current_fn = GSTMR_GET_FN_GERAN(gas_id);

   log_ptr->version = LOG_GSM_AEQ_SCH_METRICS_V2_C_VERSION;

   /* Subtract 1 from the FN to get the frame when the burst happened. */
   log_ptr->FN = SUB_FN(current_fn, 1);
   log_ptr->subID = (uint8)gas_id;

   memscpy (&log_ptr->pre_mimo_complete_snr_iter0,
            (sizeof(LOG_GSM_AEQ_SCH_METRICS_V2_C_type) - sizeof(log_ptr->version) - sizeof(log_ptr->FN) - - sizeof(log_ptr->subID) - sizeof(log_ptr->num_zero_soft_decisions)),
            &metrics->aeqLogPacket.aeqGmskLogPacket,
            sizeof(GfwAeqGmskLogPacket));


   /* Send the log packet */
   log_commit(log_ptr);
}
#endif /* FEATURE_GSM_MDSP_AEQ_SCH */

#ifdef FEATURE_AEQ_PHASE4
/*===========================================================================

FUNCTION gl1_hw_log_aeq4_metrics

DESCRIPTION
   Grabs and logs the mdsp AEQ4 metrics.  Should be called during data
   metrics processing.


===========================================================================*/
void gl1_hw_log_aeq4_metrics (GfwAeq4LogPacket *datarcvd,gas_id_t gas_id)
{
   LOG_GSM_AEQ4_METRICS_C_type *log_ptr_cs;
   uint32 current_fn;

   log_ptr_cs = GL1_LOG_ALLOC( LOG_GSM_AEQ4_METRICS_C );

   if ( !log_ptr_cs )
   {
    return;
   }

   current_fn = GSTMR_GET_FN_GERAN(gas_id);

   log_ptr_cs->version    = 1;
   log_ptr_cs->FN          = SUB_FN(current_fn, 1);
   log_ptr_cs->data        = *datarcvd;
   /* Send the log packet */
   log_commit(log_ptr_cs);

}
#endif /* FEATURE_AEQ_PHASE4 */

#ifdef FEATURE_GSM_ISR_PROFILING
void l1_log_ncell_bcch_burst_metrics (gl1_msg_metrics_rpt *rpt[],gas_id_t gas_id);
//extern uint16 time_result_buffer[NUM_GERAN_DATA_SPACES][50];
void log_profiling(gas_id_t gas_id);

void log_profiling(gas_id_t gas_id)
{
int i=0;
gl1_msg_metrics_rpt da1 ,da2, da3,da4,da5,da6,da7,da8;


da1.valid =1;
da1.rssi           = 0;
da1.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[0];
da1.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[1];
da1.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[2];
da1.freq_offset    = gtmrs_data[gas_id].time_result_buffer[3];
da1.timing_offset  = gtmrs_data[gas_id].time_result_buffer[4];
da1.snr            = gtmrs_data[gas_id].time_result_buffer[5];



da2.valid =1;
da2.rssi           = 6;
da2.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[6];
da2.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[7];
da2.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[8];
da2.freq_offset    = gtmrs_data[gas_id].time_result_buffer[9];
da2.timing_offset  = gtmrs_data[gas_id].time_result_buffer[10];
da2.snr            = gtmrs_data[gas_id].time_result_buffer[11];


da3.valid =1;
da3.rssi           = 12;
da3.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[12];
da3.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[13];
da3.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[14];
da3.freq_offset    = gtmrs_data[gas_id].time_result_buffer[15];
da3.timing_offset  = gtmrs_data[gas_id].time_result_buffer[16];
da3.snr            = gtmrs_data[gas_id].time_result_buffer[17];


da4.valid =1;
da4.rssi           = 18;
da4.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[18];
da4.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[19];
da4.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[20];
da4.freq_offset    = gtmrs_data[gas_id].time_result_buffer[21];
da4.timing_offset  = gtmrs_data[gas_id].time_result_buffer[22];
da4.snr            = gtmrs_data[gas_id].time_result_buffer[23];

da5.valid =1;
da5.rssi           = 24;
da5.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[24];
da5.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[25];
da5.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[26];
da5.freq_offset    = gtmrs_data[gas_id].time_result_buffer[27];
da5.timing_offset  = gtmrs_data[gas_id].time_result_buffer[28];
da5.snr            = gtmrs_data[gas_id].time_result_buffer[29];



da6.valid =1;
da6.rssi           = 30;
da6.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[30];
da6.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[31];
da6.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[32];
da6.freq_offset    = gtmrs_data[gas_id].time_result_buffer[33];
da6.timing_offset  = gtmrs_data[gas_id].time_result_buffer[34];
da6.snr            = gtmrs_data[gas_id].time_result_buffer[35];


da7.valid =1;
da7.rssi           = 36;
da7.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[36];
da7.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[37];
da7.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[38];
da7.freq_offset    = gtmrs_data[gas_id].time_result_buffer[39];
da7.timing_offset  = gtmrs_data[gas_id].time_result_buffer[40];
da7.snr            = gtmrs_data[gas_id].time_result_buffer[41];


da8.valid =1;
da8.rssi           = 42;
da8.pwr_dBm_x16    = gtmrs_data[gas_id].time_result_buffer[42];
da8.dc_offset_i    = gtmrs_data[gas_id].time_result_buffer[43];
da8.dc_offset_q    = gtmrs_data[gas_id].time_result_buffer[44];
da8.freq_offset    = gtmrs_data[gas_id].time_result_buffer[45];
da8.timing_offset  = gtmrs_data[gas_id].time_result_buffer[46];
da8.snr            = gtmrs_data[gas_id].time_result_buffer[47];



gl1_msg_metrics_rpt *data1[4] , *data2[4];
data1[0]=&da1;
data1[1]=&da2;
data1[2]=&da3;
data1[3]=&da4;

data2[0] = &da5;
data2[1] = &da6;
data2[2] = &da7;
data2[3] = &da8;


l1_log_ncell_bcch_burst_metrics(&data1[0],gas_id);
l1_log_ncell_bcch_burst_metrics(&data2[0],gas_id);
/* clear the log buffer after each log */

for(i=0;i<50;i++)
{
gtmrs_data[gas_id].time_result_buffer[i]=0;
}

}
#endif /*FEATURE_GSM_ISR_PROFILING */

/*===========================================================================

FUNCTION gl1_log_gsm_dft_spur_metric

DESCRIPTION
   Grabs and logs the DFT spur metric.  Should be called during
   metrics processing.


===========================================================================*/
void gl1_log_gsm_dft_spur_metric ( GfwDftSpurAciMetricsStruct SpurAciMetrics, uint16 time_slot,gas_id_t gas_id )
{
  LOG_GSM_DFT_SPUR_METRICS_C_type  *log_ptr;
  log_ptr = GL1_LOG_ALLOC( LOG_GSM_DFT_SPUR_METRICS_C );

  if ( log_ptr )
  {
    log_ptr->fFN   = GSTMR_GET_FN_GERAN(gas_id);
    log_ptr->tTN   = time_slot;
    log_ptr->gasid = gas_id ;
    log_ptr->version = 2;
    memscpy ( &log_ptr->DftSpurAciMetrics,
              (sizeof(LOG_GSM_DFT_SPUR_METRICS_C_type) - sizeof(log_ptr->fFN)- sizeof( log_ptr->gasid)- sizeof(log_ptr->version)-sizeof(log_ptr->tTN)),
              &SpurAciMetrics,
              sizeof(GfwDftSpurAciMetricsStruct));
    log_commit(log_ptr );
  }
}




