#ifndef MCALWCDMA_DECLOG_H
#define MCALWCDMA_DECLOG_H

/*============================================================================*/
/** @file  
 * This module has definition and declaration related to MCAL WCDMA decoder
 * log packet definition
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2006 - 2012 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/mcalwcdma_declog.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
04/22/15     ac      Datapath W+W changes
01/26/16     ac      Initial checkin for X+W
===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "customer.h"
#include "qw.h"
#include "log.h"
#include "mcalwcdma_dec.h"
#include "log_codes_wcdma_int.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


#ifndef DL_CCTRCH_LOG_PKT
#define DL_CCTRCH_LOG_PKT DL_CCTRCH_0x4171_LOG_PKT
#else
#error "Redefinition of the log command code: DL_CCTRCH_LOG_PKT"
#endif

#ifndef DL_TRCH_DEBUG_LOG_PKT
#define DL_TRCH_DEBUG_LOG_PKT DL_TRCH_DEBUG_0x4172_LOG_PKT
#else
#error "Redefinition of the log command code: DL_TRCH_DEBUG_LOG_PKT"
#endif

#ifndef DL_TFC_LOG_PKT
#define DL_TFC_LOG_PKT DL_TFC_0x4173_LOG_PKT
#else
#error "Redefinition of the log command code: DL_TFC_LOG_PKT"
#endif


#ifndef DL_TFCI_LOG_PKT
#define DL_TFCI_LOG_PKT DL_TFCI_0x4773_LOG_PKT
#else
#error "Redefinition of the log command code: DL_TFCI_LOG_PKT"
#endif

#ifndef DL_TBSET_STATUS_PKT
 #define DL_TBSET_STATUS_PKT DL_TBSET_STATUS_0x41BA_PKT
#else
#error "Redefinition of the log command code: DL_TBSET_STATUS_PKT"
#endif

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* ------------------------------------------------- */
/* DL TrCh debug info log packet related definitions */
/* ------------------------------------------------- */

/* This structure defines the TF debug information to log of each
   TF in a given TrCh. */
typedef PACKED struct PACKED_POST
{
  /* Rate matching parameters for TF of a TrCh */
  uint16 eplus_over_a;
  uint32 eminus_over_a;
  int32  delta_n_tti;
} dl_trch_dbg_log_tf_struct_type;

/* This structure defines the TrCh information to log for a TrCh in
   a given CcTrCh. This structure is included in the CCTrCh information
   log packet */
typedef PACKED struct PACKED_POST
{
  uint8 trch_id;          /* TrCh Id */
  uint8 num_tf;           /* number of TF */
  uint8 tf_info_base;     /* TF info base in the TF info array */
} dl_trch_dbg_log_trch_struct_type;

/* TrCh debug info log packet.
   Log Packet ID: 0x4112 */
LOG_RECORD_DEFINE(DL_TRCH_DEBUG_LOG_PKT)
  uint8 cctrch_id; /* CCTrCh Id of this CCTrCh */
  uint8 num_trch;  /* number of TrCh in this CCTrCh */
  
  /* This structure defines the TrCh debug info for the rate matching
    info of all TF in it */
  dl_trch_dbg_log_trch_struct_type trch_log_info[MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* TF information of all TF */
  dl_trch_dbg_log_tf_struct_type tf_log_info[DEC_TOTAL_TFT_PER_CCTRCH];
LOG_RECORD_END


/* --------------------------------------------- */
/* DL TBset debug log packet related definitions */
/* ----------------------------------------------*/

#define DL_MAX_TBSET_DUMP_SIZE_W32      400
#define DL_MAX_NUM_TBSIZE_DUMP          3

LOG_RECORD_DEFINE(DL_DEBUG_TBSET_PKT)
  uint8 cctrch_id; /* CCTrCh Id of this CCTrCh */
  uint8 trch_id;   /* TrCh ID */
  uint8 num_tb;    /* Number of TB in this TBset */
  uint16 tb_size;  /* TB size of the TBs */
  uint8 crc_length;   /* Number of CRC bits */
  boolean post_header; /* Header/Data read event */
  uint32 dob_data[DL_MAX_TBSET_DUMP_SIZE_W32];
LOG_RECORD_END

/* ---------------------------------------- */
/* DL CCTrCh log packet related definitions */
/* ---------------------------------------- */

/* bit mask for various log fields */
#define DL_CCTRCH_LOG_DRMB_START_ADDR_BMSK  0x7FFF
#define DL_CCTRCH_LOG_DRMB_START_ADDR_SHFT  0
#define DL_CCTRCH_LOG_DRMB_STOR_DIR_BMSK    0x8000
#define DL_CCTRCH_LOG_DRMB_STOR_DIR_SHFT    15

/* this macro sets field in variable that jas multiplexed field within
   same variable */
#define DL_CCTRCH_LOG_SET_FIELD(var, field, val) \
  var &= (~(DL_CCTRCH_LOG_DRMB_##field##_BMSK)); \
  var |= (((val) << (DL_CCTRCH_LOG_DRMB_##field##_SHFT)) & (DL_CCTRCH_LOG_DRMB_##field##_BMSK))

/* This structure defines the DRMB information to log in a given
   CcTrCh. This structure is included in the CCTrCh information
   log packet. */
typedef PACKED struct PACKED_POST
{
  /* Bits 0:14 - DRMB start address 
     Bit 15    - DRMB storage direction */
  uint16 drmb_info;

  /* DRM buffer size */
  uint16 drm_buf_size;
} dl_cctrch_log_drmb_info_struct_type;

/* This structure defines the TF information to log of each TrCh in a
   given CCTrCh. This is used in the TrCh info to log structure */
typedef PACKED struct PACKED_POST
{
  uint16 tb_size; /* Transport block (TB) size */
  uint8  num_tb;  /* Number of TB in this TF */
} dl_cctrch_log_tf_struct_type;

/* This structure defines the TrCh information to log for a TrCh in
   a given CcTrCh. This structure is included in the CCTrCh information
   log packet */
typedef PACKED struct PACKED_POST
{
  uint8 trch_id;          /* TrCh Id */
  uint8 trch_ref_type;    /* TrCh ref. type */
  uint8 coding_rate_type; /* Coding type */
  uint8 num_crc_type;     /* num CRC bits */
  uint8 tti_type;         /* TTI of this TrCh */
  uint16 rm_attribute;    /* RM attribute */
  uint8 num_tf;           /* number of TF */
  uint8 tf_info_base;     /* TF info base in the TF info array */
} dl_cctrch_log_trch_struct_type;

/* CCTrCh log packet.
   Log Packet ID: 0x4108 */
LOG_RECORD_DEFINE(DL_CCTRCH_LOG_PKT)
  uint8 cctrch_id; /* CCTrCh Id of this CCTrCh */
  uint8 num_trch;  /* number of TrCh in this CCTrCh */
  
  /* DRM Buffer information for this CcTrCh */
  dl_cctrch_log_drmb_info_struct_type drmb_log_info;

  /* TrCh information of all TrCh in this CcTrCh */
  dl_cctrch_log_trch_struct_type trch_log_info[MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* TF information of all TF */
  dl_cctrch_log_tf_struct_type tf_log_info[DEC_TOTAL_TFT_PER_CCTRCH];
LOG_RECORD_END


/* ------------------------------------- */
/* DL TFC log packet related definitions */
/* ------------------------------------- */

/* This structure defines the TFC info for a transport format
   combinition for a given CCTrCh */
typedef PACKED struct PACKED_POST
{
  uint8 valid;                 /* This CTFC/TFC valid or not */
  uint8 tf_index[MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH]; /* TF index all the TrCh in this CCTrCh */
} dl_cctrch_log_tfc_struct_type;

/* TFC log packet.
   Log Packet ID: 0x4115 */
LOG_RECORD_DEFINE(DL_TFC_LOG_PKT)
  uint8 cctrch_id;             /* CCTrCh Id to which the TFC set belongs */
  uint8 num_trch;              /* Number of TrCh in this CCTrCh */
  uint8 last_valid_ctfc_index; /* last valid TFC index in TCF info array */
  
  /* TFC info for all the TFC in this CCTrCh */
  dl_cctrch_log_tfc_struct_type tfc_log_info[DEC_TOTAL_CTFC_ENTRIES_PER_TABLE];
LOG_RECORD_END

/* -------------------------------------- */
/* DL TFCI log packet related definitions */
/* -------------------------------------- */
#define L1_TFCI_LOG_NUM_SAM       100
#define MCALWCDMA_DEC_NUM_TFCI_STATS_LOGPKT     2

typedef PACKED struct PACKED_POST
{
  uint8     cfn_tfci;
  uint8     cctrch_idx;
  uint16    decoded_tfci;
  uint16    max_enr;
  uint8     max_enr_info;
  uint16    max_enr2;
  uint8     max_enr_info2;
} dl_tfci_statistics_info_struct_type;

LOG_RECORD_DEFINE(DL_TFCI_LOG_PKT)
  uint8  version;
  uint8  num_sample;
  dl_tfci_statistics_info_struct_type tfci_log_info[L1_TFCI_LOG_NUM_SAM];
LOG_RECORD_END

/* ----------------------------------------- */
/* L1 DL TB set status log packet definition */
/* ----------------------------------------- */

#define MCALWCDMA_DEC_NUM_TBSET_LOGPKT  2

/* Number of Samples in TBset status (CRC) log packet */
#define DL_TBSET_STATUS_NUM_SAM 75

typedef PACKED struct PACKED_POST
{
  /* mDSP status FIFO info */
  uint16 mdsp_status_fifo_word[DEC_NUM_WORD16_PER_FIFO_ENTRY];
  /* DOB TBset status info */
  uint32 tbset_status_word[DEC_NUM_WORD32_PER_TB_SET_STATUS_CONV_CODING];
} dl_tbset_status_log_info_struct_type;

LOG_RECORD_DEFINE(DL_TBSET_STATUS_PKT)
   uint8 version;   
   uint8 num_sample;
   dl_tbset_status_log_info_struct_type tbset_status_log_info[DL_TBSET_STATUS_NUM_SAM];
LOG_RECORD_END

typedef struct
{
  /*************************/
  /** 0x4171 - CCTrCh log **/
  /*************************/
  DL_CCTRCH_LOG_PKT_type dl_cctrch_log_pkt;

  #ifdef FEATURE_L1_LOG_ON_DEMAND
  boolean dl_cctrch_log_pkt_valid;
  #endif
  /* Flag indicating if CCTrCh log packet is ready to be submitted */
  boolean mcalwcdma_dec_cctrch_waiting_for_submit;
  /* Flag indicating if packet been sent since last TBSET_STATUS_PKT */
  boolean mcalwcdma_dec_cctrch_log_pending;
  /* Number of TF used. These are used to optimized the log packet size */
  uint8 dl_log_num_tf_for_cctrch_info;

  /*************************/
  /**  0x4172 - TrCh log  **/
  /*************************/
  DL_TRCH_DEBUG_LOG_PKT_type dl_trch_debug_log_pkt;

  /* Num of TFs in the TrCh debug log buffer */
  uint8 dl_log_num_tf_for_trch_debug_info;
  /* Flag indicating if packet been sent since last TBSET_STATUS_PKT */
  boolean mcalwcdma_dec_trch_log_pending;
  /* Flag indicating if TrCh debug log packet is ready to be submitted */
  boolean mcalwcdma_dec_trch_debug_waiting_for_submit;

  /*************************/
  /**   0x4173 - TFC log  **/
  /*************************/
  DL_TFC_LOG_PKT_type dl_tfc_log_pkt;

  #ifdef FEATURE_L1_LOG_ON_DEMAND
  boolean dl_tfc_log_pkt_valid;
  #endif
  /* Flag indicating if packet been sent since last TBSET_STATUS_PKT */
  boolean mcalwcdma_dec_tfc_log_pending;
  /* Flag indicating if TFC log packet is ready to be submitted */
  boolean mcalwcdma_dec_tfc_waiting_for_submit;

  /****************************/
  /** 0x4773 - TFCI stats log */
  /****************************/
  DL_TFCI_LOG_PKT_type dl_tfci_statistics_log_pkt[MCALWCDMA_DEC_NUM_TFCI_STATS_LOGPKT];

  /* The buffer index in use currently */
  uint8 mcalwcdma_dec_tfci_logpkt_index_in_use;
  /* The buffer index to be submitted */
  uint8 mcalwcdma_dec_tfci_logpkt_index_to_submit;
  /* Log buffer availability status */
  boolean mcalwcdma_dec_tfci_logpkt_avail_buf[MCALWCDMA_DEC_NUM_TFCI_STATS_LOGPKT];
  /* Flag indicating if TFCI stat log packet is ready to be submitted */
  boolean mcalwcdma_dec_tfci_stat_waiting_for_submit;
  /* Flag indicating if target logging CFN is beyond combiner enable CFN*/
  boolean mcalwcdma_dec_skip_tfci_logging[MCALWCDMA_DEC_TOTAL_CCTRCH_TABLES];

  /******************************/
  /** 0x41BA - TBSet status log */
  /******************************/
  DL_TBSET_STATUS_PKT_type mcalwcdma_dec_tbset_status_log_pkt[MCALWCDMA_DEC_NUM_TBSET_LOGPKT];

  /* Flag indicating if TBset status log packet is ready to be submitted */
  boolean mcalwcdma_dec_tbset_stat_waiting_for_submit;
  /* The buffer index to be submitted */
  uint8 mcalwcdma_dec_tbset_logpkt_index_to_submit;
  /* Log buffer in use */
  uint8 mcalwcdma_dec_tbset_logpkt_index_in_use;
  /* Log buffer availability status */
  boolean mcalwcdma_dec_tbset_logpkt_avail_buf[MCALWCDMA_DEC_NUM_TBSET_LOGPKT];

  /******************************/
  /** 0x4234 - TBSet debug dump */
  /******************************/
  #ifdef FEATURE_WCDMA_TBSET_DEBUG_DUMP
  #error code not present
#endif
}mcalwcdma_mgr_ext_cntrl_params_type;

extern mcalwcdma_mgr_ext_cntrl_params_type mcalwcdma_mgr_ext_cntrl_params[WCDMA_NUM_SUBS];

/* Call back function type declaration */
/* ----------------------------------- */

/* Enum type declaration */
/* --------------------- */

/* Structure type declaration */
/* -------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

#endif /* MCALWCDMA_DECLOG_H */
