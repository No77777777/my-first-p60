#ifndef WL1W2GLOG_H
#define WL1W2GLOG_H

/*===========================================================================
                WL1 W2G log packet header file

DESCRIPTION
  This file is WL1 W2G log packet header file that supports 0x41DC and 0x41DD log packet
  implementations. This file contains all declarations and definitions necessary for WL1
  to implement the log packets.

Copyright (c) 2006 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1w2glog.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who     what, where, why
---------------------------------------------------------------------------------------------------------
12/09/15   gn      Created this file to add new log packets for W2G
===========================================================================*/

#include "comdef.h"
#include "qw.h"
#include "log.h"
#include "log_codes_wcdma_int.h"
#include "srchgsmdrv.h"

/* =======================================================================
**                        Macros
** ======================================================================= */
typedef enum
{
   WL1_W2G_IDLE_GAP,
   WL1_W2G_FACH_GAP,
   WL1_W2G_CM_GAP
}srchcm_w2g_ue_state_enum_type;

typedef enum
{
   SCH_BURST_NOT_RECEIVED,
   SCH_BURST_RECEIVED_BUT_BSIC_ID_DECODE_FAILED,
   BSIC_ID_DECODE_PASSED_NOT_RECONFIRMED,
   BSIC_ID_DECODE_PASSED_AND_RECONFIRMED
}wl1_w2g_bsic_id_result_enum_type;

/* Max inter-RAT cells */
#define W2G_IRAT_GSM_CELL_MAX 32
/* Max Log Buffer size */
#define WL1_W2G_LOG_MAX_BUF_SIZE 1000
/* Max number of entities that can be buffered in W2G Gap Information Log Packet */
#define WL1_W2G_GAP_MAX_ENTITIES 50


/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================
FUNCTION wl1w2glog_gap_info_logging

DESCRIPTION
This function is to fill the W2G Gap information log packet structure and submit the log.

DEPENDENCIES
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void wl1w2glog_gap_info_logging( uint8 gap_length_slots,
                                        uint8 gap_start_slot_num,
                                        srchgsm_meas_info_struct_type *meas_info,
                                        uint8 srch_type,
                                        uint8 ue_state,
                                        boolean cm_freeze,
                                        boolean cm_suspend,
                                        wsub_id_e_type  wsub_id );

/*===========================================================================
FUNCTION wl1w2glog_meas_info_logging

DESCRIPTION
This function is to fill the W2G Measurement results log packet structure and submit the log.

DEPENDENCIES
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void wl1w2glog_meas_info_logging( void  *results_ptr,
                                         boolean result,
                                         uint8 num_results,
                                         uint16 arfcn,
                                         uint8 srch_type,
                                         uint8 pwr_scan_idx,
                                         wsub_id_e_type  wsub_id );


/* Header definition since the header definition is defined in log.c */
typedef PACKED struct PACKED_POST
{
  uint16 len;  /* Specifies the length, in bytes of 
                 the entry, including this header. */

  uint16 code; /* Specifies the log code for the 
                  entry as enumerated above.       
                  Note: This is specified as word 
                  to guarantee size.               */

  qword ts;    /* The system timestamp for the log entry. The upper 48 bits
                  represent elapsed time since 6 Jan 1980 00:00:00 
                  in 1.25 ms units. The low order 16 bits represent elapsed 
                  time since the last 1.25 ms tick in 1/32 chip units 
                  (this 16 bit counter wraps at the value 49152).          */
} wl1w2glog_log_hdr_struct_type;

/*--------------------------------------------------------------------------
                 LOG PACKET: WL1_X2W_SRCH_STEP1_LOG_PACKET
                     
This structure is used to log search step 1 information. These packets occur
upon every id search in idle mode.          
--------------------------------------------------------------------------*/

#ifndef WL1_W2G_GAP_LOG_PKT
#define WL1_W2G_GAP_LOG_PKT L1_WCDMA_W2G_GAP_0x41DC_LOG_PKT
#else
#error redefinition of LOG command code: WL1_W2G_GAP_LOG_PKT
#endif

typedef PACKED struct PACKED_POST
{
  /* WCDMA L1 UE state
     0 - Idle State /
     1 - FACH State / / 
     2 - CM State */
  uint8 ue_state;
  /*(0) RSSI/
    (1) FCCH detection/
    (2) SCH decode/
    (3) SCH reconfirmation */
  uint8 srch_type;
  /* CFN value at the time of freeze, if CM is freezed  */
  uint8 cm_freeze;
  /* CFN value at the time of suspend, if CM is suspended */
  uint8 cm_suspend;
  /* WCDMA frame number when W2G GAP starts */
  uint8 gap_start_cfn;
  /* TGSN of Gap in WCDMA Frame */
  uint8 start_slot;
  /* Length of the slot gap */
  uint8 slot_len;
  /* WCDMA frame number when W2G GAP ends  */
  uint8 gap_end_cfn;
  /* GSM frame number when W2G GAP starts */
  uint8 gap_start_gfn;
  /* Offset from GSM frame number to actual gap start position.  */
  uint16 gap_start_qs_offset;
  /* GSM frame number when W2G GAP starts */
  uint8 gap_end_gfn;
  /* Gap length in QS */
  uint16 gap_len_qs;

} wl1w2glog_gap_log_info_struct_type;

LOG_RECORD_DEFINE(WL1_W2G_GAP_LOG_PKT)

  /* Pkt Version */
  uint8 pkt_ver;
  /* Number of entities in the log packet */
  uint8 num_entities;
  /* W2G Gap information for each entity */
  wl1w2glog_gap_log_info_struct_type w2g_gap_log_info[WL1_W2G_GAP_MAX_ENTITIES];


LOG_RECORD_END

/*--------------------------------------------------------------------------
                 LOG PACKET: WL1_X2W_SRCH_STEP1_LOG_PACKET
                     
This structure is used to log search step 1 information. These packets occur
upon every id search in idle mode.          
--------------------------------------------------------------------------*/

#ifndef WL1_W2G_MEAS_LOG_PKT
#define WL1_W2G_MEAS_LOG_PKT L1_WCDMA_W2G_MEAS_0x41DD_LOG_PKT
#else
#error redefinition of LOG command code: WL1_W2G_MEAS_LOG_PKT
#endif

typedef PACKED struct PACKED_POST
{
  /* Carrier RF channel number */
  uint16 carrier_arfcn;
  /* Burst start GSM QS offset */
  uint16 burst_start_qs_offset;
  /* LNA gain range choosen */
  uint8 gsm_lna_state;
  /* RSSI dB*/
  int16 rssi_db;
} wl1w2glog_rssi_meas_log_info_struct_type;

LOG_RECORD_DEFINE(WL1_W2G_MEAS_LOG_PKT)

  /* Pkt Version */
  uint8 pkt_ver;
  /*(0) RSSI/
    (1) FCCH detection/
    (2) SCH decode/
    (3) SCH reconfirmation */
  uint8 srch_type;
  /* union of srch schedule time based on srch_type */
  PACKED union PACKED_POST
  {
    /* RSSI measurement info */
    PACKED struct PACKED_POST
    {
      /* number of GSM RSSI scans */
      uint8 num_bursts;
      /* Frequency at which RSSI burst is initiated */
      uint32 rssi_detection_timer;
      /* RSSI information for each burst */
      wl1w2glog_rssi_meas_log_info_struct_type rssi_meas_log_info[W2G_IRAT_GSM_CELL_MAX];
    } rssi_meas;

    /* FCCH detection info */
    PACKED struct PACKED_POST
    {
      /* Carrier RF channel number */
      uint16 carrier_arfcn;
      /* GSM frame number when ACQ starts*/
      uint8 acq_start_gfn;
      /* ACQ start GSM QS offset */
      uint16 acq_start_qs_offset;
      /* number of FCCH tones detected */
      uint8 num_fcch_tone_detected;
      /* Tone QS offset from ACQ start offset */
      int32 fcch_tone_qs_offset;
      /* Frequency offset value for the tone returned from FCCH result*/
      int32 freq_offset;
      /* FCCH detection result */
      uint8 fcch_result;
      /* SNR estimate of the tone */
      uint16 fcch_snr;
      /* Number of gaps to find FCCH/Parallel SCH */
      uint16 bsic_identify_count;
    } fcch_meas;

    /* SCH decode or SCH reconfirmation info */
    PACKED struct PACKED_POST
    {
      /* Carrier RF channel number */
      uint16 carrier_arfcn;
      /* Describes how SCH decode was obtained /
         0 - Legacy Mechanism /
         1 - Parallel SCH  */
      uint8 parallel_sch;
      /* GSM frame number at which SCH decoded */
      uint8 sch_decode_gfn;
      /* Timing offset result returned by GFW */
      int32 acq_time_offset;
      /* Frequency offset result returned by GFW */
      int32 freq_offset;
      /* GSM QS offset to the center of the burst at which SCH is decoded */
      uint16 sch_burst_qs_offset;
      /* indicate the BSIC verification status and CRC pass status */
      uint16 sch_bsic_verify_result;
      /* Decoded SCH data */
      uint32 sch_decoded_data;
      /* SCH frame nunber 1, 11, 12, 21, 22, 31, 32, 41, 42 */
      uint8 sch_decode_frame_number;
      /* ncc value of BSIC_ID */
      uint8 bsic_id_ncc;
      /* bcc value of BSIC_ID */
      uint8 bsic_id_bcc;
      /* CRC_PASS information which is returned from GFW result */
      uint8 crc_pass;
      /* Metrics_valid result which is returned from GFW result */
      uint8 metrics_valid;
      /* RSSI dBmx16  */
      int16 rssi;
      /* SNR value */
      uint16 sch_snr;
      /* Time difference between two consecutive BSIC ID/Reconfirmations */
      uint32 bsic_id_periodicity;
    } sch_meas;
  } w2g_meas_info;

LOG_RECORD_END

#define WL1_W2G_MEAS_LOG_PKT_HEADER_LEN (sizeof(wl1w2glog_log_hdr_struct_type))
#define WL1_W2G_MEAS_LOG_PKT_BASE_LEN (2 + WL1_W2G_MEAS_LOG_PKT_HEADER_LEN)
#define WL1_W2G_MEAS_LOG_PKT_RSSI_LEN(num_cells) (5+ (sizeof(wl1w2glog_rssi_meas_log_info_struct_type) * num_cells))
#define WL1_W2G_MEAS_LOG_PKT_FCCH_LEN 19
#define WL1_W2G_MEAS_LOG_PKT_SCH_LEN 33
#define WL1_W2G_GAP_PKT_BASE_ENTITY_LEN 14
#define WL1_W2G_GAP_LOG_PKT_LEN(num_entities) (2+ ((num_entities) * WL1_W2G_GAP_PKT_BASE_ENTITY_LEN))

typedef struct
{
  /* Number of log packet entities buffered */
  uint8 gap_log_num_entities;
  /* Carrier RF channel number */
  uint16 carrier_arfcn;
  /* number of FCCH tones detected */
  uint8 num_fcch_tone_detected;
  /* number of bsic_id done for the current cell in search */
  uint16 bsic_identify_count;
  /* Length of W2G Meas Log packet */
  uint8 w2g_meas_log_pkt_len;
  /* W2G Gap information log packet*/
  WL1_W2G_GAP_LOG_PKT_type w2g_gap_log_pkt;
  /* W2G measurement result log packet*/
  WL1_W2G_MEAS_LOG_PKT_type w2g_meas_log_pkt;

} wl1w2glog_ext_cntrl_params_type;

extern wl1w2glog_ext_cntrl_params_type wl1w2glog_ext_cntrl_params[WCDMA_NUM_SUBS];

#endif /* WL1W2GLOG_H */

