#ifndef TDSDLOLPC_H
#define TDSDLOLPC_H
/*============================================================================
                            D L O L P C . H
DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2003-2010 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlolpc.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
03/15/11    xq      Created the file.
===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsl1rrcif.h"
#include "tfw_sw_intf_msg.h"
#include "mcaltds_mgr.h"
#include "mcaltds_dec.h"

#ifndef MSGR_TDSCDMA_L1_DL
#define MSGR_MODULE_L1_DL   0x03
#define MSGR_TDSCDMA_L1_DL  MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_L1_DL)
#endif

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* power offset value between DPCCH and DPDCH received from RRC in the
 * format defined in spec (int value 0..24 offset varying in steps of
 * 0.25dB from 0dB to 6dB)
 */
extern uint16 tdsdlolpc_dl_olpc_target_ebnt_hist[8];

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* Outer loop power control related macros */
/* --------------------------------------- */

#define TDSDLOLPC_BLER_QUAL_VAL_MAX 0
#define TDSDLOLPC_BLER_QUAL_VAL_MIN -63
#define TDSDLOLPC_BLER_QUAL_VAL_NA  -127
#define TDSDLOLPC_OLPC_TGT_EBNT_MAX_VAL          0xFFFF

#define TDSDLOLPC_MA_MAX_HIST 1025
#define TDSDLOLPC_ERR_MAX_HIST 5

#define TDSDLOLPC_MAX_NUM_BLKS 33

#define TDSDLOLPC_SIR_TGT_DB_UP_STEP_5_Q25     (16777216*2)   /* 0.5*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_UP_STEP_4_Q25     (13421773*2)   /* 0.4*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_UP_STEP_3_Q25     (10066330*2)   /* 0.3*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_UP_STEP_2_Q25      (6710886*2)   /* 0.2*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_UP_STEP_1_Q25      (3355443*2)   /* 0.1*2 dB */

#define TDSDLOLPC_SIR_TGT_DB_DOWN_STEP_5_Q25   (16777216*2)   /* 0.5*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_DOWN_STEP_4_Q25   (13421773*2)   /* 0.4*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_DOWN_STEP_3_Q25   (10066330*2)   /* 0.3*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_DOWN_STEP_2_Q25    (6710886*2)   /* 0.2*2 dB */
#define TDSDLOLPC_SIR_TGT_DB_DOWN_STEP_1_Q25    (3355443*2)   /* 0.1*2 dB */

#define TDSDLOLPC_MAX_BLER_VAL        64
#define TDSDLOLPC_MAX_DOWN_STEP_VAR   6
#define TDSDLOLPC_CURR_BLER_COLUMN    0
#define TDSDLOLPC_DOWN_STEP_COLUMN_5  1
#define TDSDLOLPC_DOWN_STEP_COLUMN_4  2
#define TDSDLOLPC_DOWN_STEP_COLUMN_3  3
#define TDSDLOLPC_DOWN_STEP_COLUMN_2  4
#define TDSDLOLPC_DOWN_STEP_COLUMN_1  5

// BLER filter sensitivity in IDLE/ACTIVE modes
#define TDSDLOLPC_FILTER_SENS_IDLE    8
#define TDSDLOLPC_FILTER_SENS_ACTIVE  9
#define TDSDLOLPC_MA_LEN_RANGE_1      128
#define TDSDLOLPC_MA_LEN_RANGE_2      256
#define TDSDLOLPC_MA_LEN_RANGE_3      1024
#define TDSDLOLPC_FIX_POINT_M         22
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* outer loop power control related structure definition */
/* ----------------------------------------------------- */

typedef struct
{
  /* this indicate that if the outer loop power control is applicable
     to this TrCh */
  boolean valid;
  /*trch id*/
  uint8 trch_id;
  /* BLER quality target reported by RRC or default value set by L1 code.
     This number ranges from 0 to -63. This is log10 of the real BLER
     quality target. */
  int8 bler_quality_target;
  /* Scaled SIR target in dB */
  int32 sir_target_db_q25;

  /* Down step size (scaled value */
  uint32 down_step_size_q25;
  boolean tr_blk_recvd;
  boolean dormant;
  uint16  inactivity_timer;
  int32   old_max_target_db_q25;
  uint8   parole_mode_timer;
  uint8   TTI_err_counter;

  int32     bler_filter_val_q25; // Y(k)
  int32     bler_prev_filter_val_q25; // Y(k-1)
  uint32    dl_olpc_blk_counter;
  uint16    dl_olpc_bler_row;
  uint16    dl_olpc_n;
  // Total number of blocks needed to fill bler filter
  uint32    dl_olpc_filter_filler;
  uint8     dl_olpc_filt_crc_hist[TDSDLOLPC_MA_MAX_HIST];
  uint8     dl_olpc_filt_err_hist[TDSDLOLPC_MA_MAX_HIST];
  uint32    dl_olpc_filt_crc_sum;
  uint32    dl_olpc_filt_err_sum;
  uint16    dl_olpc_ma_hist_idx;
  boolean   dl_olpc_err_hist[TDSDLOLPC_ERR_MAX_HIST];
  uint16    dl_olpc_err_hist_idx;
  uint16    dl_olpc_ma_len;

  boolean   is_dcch;
  boolean   in_panic_mode;

  tdsl1_tti_enum_type tti_value;
  /* Coding type (enumerated). */
  tdsl1_dl_cctrchcfg_coding_enum_type        coding_type;
  /** CRC length (enum or number of bits). */
  tdsl1_crc_length_enum_type crc_length;
  /** Number of transport format. */
  uint8 num_tf;

  /** Pointer to the array of transport format. */
  mcaltds_dec_tf_info_struct_type *tf_info_array[TDSL1_DL_MAX_TF];
} tdsdlolpc_trch_vars_struct_type;
typedef struct
{
  /* outer loop power control variable for each trch */
  tdsdlolpc_trch_vars_struct_type trch_vars[TDSL1_DL_MAX_TRCH];
} tdsdlolpc_vars_struct_type;

extern  tdsdlolpc_vars_struct_type  *tdsdlolpc_vars_ptr;
/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================
FUNCTION tdsdlolpc_olpc_init

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsdlolpc_olpc_init
(
  /* CcTrCh table index */
  uint8 cctrcht_idx,
  /* CCTrCh info structure info pointer */
  const tdsl1_dl_cctrchcfg_cctrch_info_struct_type *cctrch_info
#ifdef FEATURE_TDSCDMA_L1_TFCI_BASED_OLPC
  /* TFC info structure info pointer */
  ,const tdsl1_dl_cctrchcfg_ctfc_info_struct_type   *ctfc_info
#endif
);

/*===========================================================================
FUNCTION tdsdlolpc_olpc_enable

DESCRIPTION
  Enables OLPC.
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsdlolpc_olpc_enable(void);

/*===========================================================================
FUNCTION tdsdlolpc_olpc_disable

DESCRIPTION
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsdlolpc_olpc_disable(void);
/*===========================================================================
FUNCTION tdsdlolpc_olpc_in_reconfig

DESCRIPTION
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsdlolpc_olpc_in_reconfig(void);

/*===========================================================================
FUNCTION tdsdlolpc_olpc_report_channel_crc_status

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsdlolpc_olpc_report_channel_crc_status
(
  uint8 trch_idx,     /* TrCh index */
  uint8 tf_idx,       /* TF index */
  uint8 num_crc,      /* number of the CRC received */
  uint8 num_err,       /* number of CRC errors */
  boolean sb_detected_status/*special burst detected status*/
);

/*===========================================================================
FUNCTION tdsdlolpc_olpc_update_sir_target

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsdlolpc_olpc_update_sir_target(void);

/*===========================================================================
FUNCTION tdsdlolpc_update_trch_sir_tgt

DESCRIPTION

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdlolpc_update_trch_sir_tgt
(
  uint8 trch_idx,     /* TrCh index */
  uint8 num_crc,      /* number of the CRC received */
  uint8 num_err,       /* number of CRC errors */
  boolean sb_detected_status/*special burst detected status*/
);

/*===========================================================================
FUNCTION dl_olpc_find_trch_dormancy_status

DESCRIPTION
  This function returns TRUE in case of the transport channel is under dormancy or if .
  Trch dormancy  Non_dormant_trch_exist  ret_val
         FALSE               FALSE                      TRUE (This case should never happen)
         FALSE               TRUE                       TRUE
         TRUE                FALSE                      TRUE
         TRUE                TRUE                       FALSE  
    
DEPENDENCIES
  None

RETURN VALUE
  Boolean TRUE or FALSE

SIDE EFFECTS
None
===========================================================================*/

extern boolean tdsdlolpc_olpc_find_dormancy_criteria(uint8 cctrch_index, uint8 trch_index);

/*===================================================================
FUNCTION tdsdlolpc_get_target_sir

DESCRIPTION
  return current sir target.

DEPENDENCIES
  None

RETURN VALUE
  int32

SIDE EFFECTS
  New estimate of bler for the given tr ch is computed
===================================================================*/
extern int32 tdsdlolpc_get_target_sir(void);
/*===================================================================
FUNCTION tdsdlolpc_update_sirtarget_dueTo_rxdivTurnoff

DESCRIPTION
  increase sirTarget of all the TrCH due to RX diversity is turned off.

DEPENDENCIES
  
RETURN VALUE

SIDE EFFECTS

===================================================================*/

extern void tdsdlolpc_update_sirtarget_dueTo_rxdivTurnoff(int8 set_point);

#endif

