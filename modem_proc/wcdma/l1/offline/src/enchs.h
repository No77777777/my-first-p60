#ifndef ENCHS_H
#define ENCHS_H

/*==========================================================================
             WCDMA L1 HS encoder operation related header file

DESCRIPTION
  This file contains definition, declaration required for HSDPA encoder
  related operations

  Copyright (c) 2004 - 2011 by Qualcomm Technologies, Inc.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/enchs.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/19/15    ac      W + W datapath init cleanup
01/26/15   hdk      Hslog global reorg for W+W
01/26/15    ac      Initial checkin for X+W
===========================================================================*/

#include "wcdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "hslog.h"

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Number of beta table buffer. Set to 2 to double buffer these */
#define ENCHS_NUM_BETA_TABLE_BUF 2
/* HS encoder beta table invalid table index identifier */
#define ENCHS_TABLE_IDX_INVALID 0xFF

/* Avg CQI Info invalid identifier */
#define ENCHS_AVG_CQI_INVALID 0xFF
/* Num Samples for Avg CQI Info invalid identifier */
#define ENCHS_INVALID_NUM_SAMPLE_FOR_AVG_CQI 0xFFFF;

/* Number of Rhs values 0..8  and 9 for DTX */
#define ENCHS_NUM_RHS_VAL (HS_DPCCH_DELTA_VAL_MAX + 2)
/* Rhs value index that corresponds to DTX */
#define ENCHS_RHS_DTX_VAL_IDX (ENCHS_NUM_RHS_VAL - 1)

/* Total number of Beta pairs */
#define ENCHS_NUM_BETA_PAIRS 30

/* Maximum beta value */
#define ENCHS_MAX_BETA_VAL   15

/* Max values based on bit width and then scaled */

#define ENCHS_BETA_PRIME_BW          15
#define ENCHS_BETA_PRIME_MAX_VAL     17841

/* There are 5 scaling values for Rhs
   index 0: 1/1 For normal mode
   index 1: 6/5 Following are for compressed mode
   index 1: 6/4 
   index 1: 5/4
   index 1: 5/3 */
#define ENCHS_NUM_MAX_RHS_SCALINGS 5
/* Scaling value index corresponding to no scaling (normal mode */
#define ENCHS_RHS_INDEX_NO_SCALING 0
/* For a given DPCCH slot format only 2 compressed mode scalings are possible.
   So total of 3 scalings including normal mode */
#define ENCHS_RHS_NUM_RHS_SCALE_BETA_TABLE 3

/* Maximum length of test pattern */
#define ENCHS_TEST_MODE_ACK_NACK_PATTERN_LEN_MAX WFW_TX_HS_ACK_ORIDE_BUF_SIZE
#define ENCHS_TEST_MODE_CQI_PATTERN_LEN_MAX      WFW_TX_HS_CQI_ORIDE_BUF_SIZE

/* Override bit ask in mDSP */
#define ENCHS_TEST_MODE_ORIDE_ACK_NACK_PATTERN_BMSK 0x0001
#define ENCHS_TEST_MODE_ORIDE_CQI_PATTERN_BMSK      0x0002

/* ACK/NACK/DTX values in mDSP */
#define ENCHS_TEST_MODE_ACK_VAL   0
#define ENCHS_TEST_MODE_NACK_VAL  1
#define ENCHS_TEST_MODE_DTX_VAL  -1


/*Number of new HS Types added with MIMO*/
#define ENCHS_NUM_MIMO_RHS_TYPES 5

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Call back function type declaration */
/* ----------------------------------- */

/* Enum type declaration */
/* --------------------- */

/* Different Rhs values types. These corresponds to various type of
   infomration transmitted */
typedef enum
{
  ENCHS_RHS_VAL_TYPE_BC,
  ENCHS_RHS_VAL_TYPE_BD,
  ENCHS_RHS_VAL_TYPE_BHS,
  ENCHS_RHS_VAL_TYPE_RA,
  ENCHS_NUM_RHS_VAL_TYPE
} enchs_rhs_val_type_enum_type;

/* This enum defines the Index for the Single and Dual Carrier HSDPA for 
 * the B_hs value for T/P calculations for EDCH  */
typedef enum 
{
  ENCHS_SC_HSDPA_IDX,
  ENCHS_DC_HSDPA_IDX,
  ENCHS_MAX_HSDPA_CARRIER_IDX
} enchs_carr_idx_enum_type;

/* Structure type declaration */
/* -------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */






/* Variabled to store average CQI and Num Samples */
extern uint16 enchs_avg_cqi_submit_num_sample;

typedef struct
{
  /* Periodic Average CQI info */
  uint32 enchs_avg_cqi_val;

  /* Number of samples */
  uint16 enchs_avg_cqi_num_sample;
}enchs_avg_cqi_info_struct_type;

typedef struct
{
  boolean enchs_ul_dpcch_cqi_override_pending;
  uint32 dpcch_est_counter;
  /* Indicate pending ACK/NACK or CQI override request */
  boolean enchs_ul_dpcch_ack_nack_override_pending;
  uint32 debug_dpcch_est_counter;
}enchs_ext_cntrl_params_type;

extern enchs_ext_cntrl_params_type enchs_ext_cntrl_params;

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================
FUNCTION enchs_get_avg_cqi_info

DESCRIPTION
  This function returns avg CQI information

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern uint32 enchs_get_avg_cqi_info(void);

/*===========================================================================
FUNCTION enchs_get_avg_cqi_info_for_ui_display

DESCRIPTION
  This function populates avg CQI and total Num Sample information 
  for UI display

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void enchs_get_avg_cqi_info_for_ui_display(
  /* structure pointer to get the avg CQI and Num sample */
  enchs_avg_cqi_info_struct_type *avg_cqi_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_set_cqi_average_window_seconds

DESCRIPTION
  This function sets total Num Sample information 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void enchs_set_cqi_average_window_seconds(
  /* Scaling factor for avg CQI calculation */
  uint8 cqi_averaging_window, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION enchs_init

DESCRIPTION
  This function initialize encode HS beta table related variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_init(void);

/*===========================================================================
FUNCTION enchs_create_beta_table

DESCRIPTION
  This function computes and fill new Beta table. It assumes that UL DPCCH
  slot format has already been set as per new configuration. This should be
  TRUE because HS config happens after all DPCH related configuration.
  Beta  prime (Bc') is computed as following
  
  Beta table is highly dimentional table. Following is the structure of beta
  table.
  
  Bc,Bd |      DTX         |       ACK        |       NACK       |      CQI
        |------------------|------------------|------------------|------------------
        | Normal CM_A CM_B | Normal CM_A CM_B | Normal CM_A CM_B | Normal CM_A CM_B
        |  |               
        | \/               
        | Bc' Bd' Bhs' RA  
  ----------------------------------------------------------------------------------
  15, 0 |
  15, 1 |
  .....
  15,15 |
   1,15 |
   2,15 |
  .....
  14,15 |
  
  There are 50 Beta pairs
  Each beta pair has 4 Rhs types
  Each Rhs type has 3 scaling (1 normal + 2 compressed mode)
  Each scaling has 4 vales (Bc', Bd', Bhs', RA)
        
DEPENDENCIES
  Any UL DPCH config must be done before calling this function

RETURN VALUE
  None

SIDE EFFECTS
  Beta table is computed and updated at ordered beta table index
===========================================================================*/

extern void enchs_create_beta_table(
  /* delta CQI (0..8) */
  uint8 delta_cqi,
  /* delta ACK (0..8) */
  uint8 delta_ack,
  /* delta NACK (0..8) */
  uint8 delta_nack
 #if defined(FEATURE_WCDMA_MIMO) || defined(FEATURE_WCDMA_DC_HSDPA)
  /* delta ACK_ACK (0..8) */
  , uint8 delta_ack_ack
  /* delta ACK_NACK (0..8) */
  , uint8 delta_ack_nack
  /* delta NACK_ACK (0..8) */
  , uint8 delta_nack_ack
  /* delta NACK_NACK (0..8) */
  , uint8 delta_nack_nack
  /* delta cqi (0..8) */
  , uint8 cqi_mimo_delta
 #endif
  , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_switch_beta_table_index

DESCRIPTION
  This function set current beta table index to ordered beta table index
  and ordred beta table is reset to INVALID.
  This is done with WCDMA_INTLOCK as usage of beta table index can be in UL
  interrupt context.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Beta table is computed and updated at ordered beta table index
===========================================================================*/

extern void enchs_switch_beta_table_index( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_upload_beta_table_to_mdsp

DESCRIPTION
  This function uploads beta table for DTX, ACK, NACK and CQI Rhs types for
  all values of Bc'. Bd', Bhs' and RA. It finds the table to upload from
  already prepared beta table based on Bc,Bd pair, and compressed mode.
  
DEPENDENCIES
  mDSP beta table index is assumed to be available one. It is responsibility
  of call to provide appropriate beta table index

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_upload_beta_table_to_mdsp(
  /* mDSP beta table index where to upload table */
  uint8 mdsp_beta_table_index,
  /* Beta C value */
  uint8 bc,
  /* Beta C value */
  uint8 bd,
  /* Rate adjust in value over head */
  int16 rate_adj_in_overhead,
  /* Indicate frame compressed or not */
  boolean frame_compressed,
  /* indicate if compressed then which slot format to use */
  boolean cm_slot_format_a, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_set_ack_nack_override

DESCRIPTION
  This function set ACK/NACK test mode for UL HS DPCCH. It checks override
  status in mDSP. If mDSP doesn't have override eanabled then it programs
  mDSP with override immediatey.
  If mDSP has already override enabled then it sets up pending flag and UL
  maintainance event will manage uprating pending override.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_set_ack_nack_override(
  /* indicate override by ACK/NACK test pattern.
     TRUE override enabled, FALSE is DISABLED */
  boolean override,
  /* length of ACK/NACK pattern. Max 8 */
  uint8 ack_nack_pattern_len,
  /* Array pointer to ACK/NACK/DTX pattern */
  int8 *ack_nack_test_pattern,
  int8 *ack_nack_test_pattern_dc,
  int8 *ack_nack_test_pattern_3c
  , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_load_ack_nack_test_pattern

DESCRIPTION
  This function load and set ACK/NACK override data to mDSP
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_load_ack_nack_test_pattern( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_set_cqi_override

DESCRIPTION
  This function set CQI test mode for UL HS DPCCH. It checks override
  status in mDSP. If mDSP doesn't have override eanabled then it programs
  mDSP with override immediatey.
  If mDSP has already override enabled then it sets up pending flag and UL
  maintainance event will manage uprating pending override.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_set_cqi_override(
  /* indicate override by CQI test pattern.
     TRUE override enabled, FALSE is DISABLED */
  boolean override,
  /* CQI repetition cycle repeat count. Must be minimum 1 for CQI test
     pattern to be effective */
  int16 cqi_repetition_cycle_repeat_count,
  /* length of ACK/NACK pattern. Max 8 */
  uint8 cqi_pattern_len,
  /* Array pointer to CQI pattern */
  uint8 *cqi_test_pattern,
    /* length of ACK/NACK pattern. Max 8 */
  uint8 cqi_pattern_len_dc,
  /* Array pointer to CQI pattern */
  uint8 *cqi_test_pattern_dc,
    /* length of ACK/NACK pattern. Max 8 */
  uint8 cqi_pattern_len_3c,
  /* Array pointer to CQI pattern */
  uint8 *cqi_test_pattern_3c
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_load_cqi_test_pattern

DESCRIPTION
  This function load and set CQI override data to mDSP
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_load_cqi_test_pattern( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_maintain_test_pattern_override

DESCRIPTION
  This function is called in UL maintanance event. It checks for pending
  override update flags. If there is any flag pending then it acts upon
  that.
  Pending request is disable override mode:
    It disable override bit in mDSP and is done with request
  Pending request is enable override mode:
    If mDSP existing mode is already override then it disables mDSP override
    and DON'T clear pending flag. This is to make UL wait for one frame roll
    over to make disable effective in mDSP. In next frame event, if enables
    override with new data and is done with pending request.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_maintain_test_pattern_override( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION enchs_override_cqi

DESCRIPTION
  This function sets a flag as well as the CQI override value to the MDSP. 
  mDSP reads the flag at the R99 frame boundary and if set, overrides the 
  CQI with the value written by L1.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void enchs_override_cqi(
  /* flag to override CQI value */
  boolean flag, 
  /* the CQI value to be overwritten */
  uint8 val, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  ENCHS_GET_MAX_SIGNALED_DELTA_HS

DESCRIPTION
  This function returns the maximum signaled delta HS scaled for next frame's CM.

DEPENDENCIES
  This function can only be called in DCH state by EDPCH module during EDPCH
  event that is after UL TX_GEN_GET_DATA event.

RETURN VALUE
  uint16 as A_hsx15
  if HS not configured, return 0
  if HS not present in frame, return 0

SIDE EFFECTS
  None
===========================================================================*/
uint16 enchs_get_max_signaled_delta_hs
(
  uint8   dpcch_slt_fmt_nm,  /* 0 ~ 3 */
  boolean compressed,       /* CM or normal */
  boolean cm_slot_format_a  /* CM A or B */
, wsub_id_e_type wsub_id);

#endif /* ENCHS_H */

