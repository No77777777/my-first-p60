#ifndef TDSENCHS_H
#define TDSENCHS_H

/*==========================================================================
             WCDMA L1 HS encoder operation related header file

DESCRIPTION
  This file contains definition, declaration required for HSDPA encoder
  related operations

  Copyright (c) 2010-2011 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsenchs.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/12/11   owen     cleanup and new TDS RRC/L1 IF
05/09/11   owen     cleanup more for logging
===========================================================================*/

#include "tdscdma_variation.h"
#include "tdsuecomdef.h"
#include "customer.h"


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
#define TDSENCHS_NUM_BETA_TABLE_BUF 2
/* HS encoder beta table invalid table index identifier */
#define TDSENCHS_TABLE_IDX_INVALID 0xFF


/* Rhs value index that corresponds to DTX */
#define TDSENCHS_RHS_DTX_VAL_IDX (TDSENCHS_NUM_RHS_VAL - 1)

/* Total number of Beta pairs */
#define TDSENCHS_NUM_BETA_PAIRS 30

/* Maximum beta value */
#define TDSENCHS_MAX_BETA_VAL   15

/* Max values based on bit width and then scaled */
/* Modified beta maximum bit width */
#define TDSENCHS_BETA_PRIME_BW          11
#define TDSENCHS_BETA_PRIME_MAX_VAL ((((1 << TDSENCHS_BETA_PRIME_BW) - 1)/30) * 24)

/* There are 5 scaling values for Rhs
   index 0: 1/1 For normal mode
   index 1: 6/5 Following are for compressed mode
   index 1: 6/4 
   index 1: 5/4
   index 1: 5/3 */
#define TDSENCHS_NUM_MAX_RHS_SCALINGS 5
/* Scaling value index corresponding to no scaling (normal mode */
#define TDSENCHS_RHS_INDEX_NO_SCALING 0
/* For a given DPCCH slot format only 2 compressed mode scalings are possible.
   So total of 3 scalings including normal mode */
#define TDSENCHS_RHS_NUM_RHS_SCALE_BETA_TABLE 3


#ifdef FEATURE_HSDPA_L1_STATS

/* Number of HS decode status buffer */
#define TDSENCHS_LOG_NUM_BUF 2

/* Number of entries in mDSP logging buffer per sub frame */
#define TDSENCHS_LOG_INFO_NUM_W16_PER_ENTRY 4

/* Various fields in mDSP UL HS DPCCH log buffer for a sub-frame */

//#ifdef FEATURE_HSDPA_HARQ_PREAMBLE
#define TDSENCHS_MDSP_LOG_DPCCH_ACK_NACK_STATUS_BMSK 0x3800
//#else
//#define TDSENCHS_MDSP_LOG_DPCCH_ACK_NACK_STATUS_BMSK 0x1800
//#endif
#define TDSENCHS_MDSP_LOG_DPCCH_ACK_NACK_STATUS_REG  0
#define TDSENCHS_MDSP_LOG_DPCCH_ACK_NACK_STATUS_SHFT 11

#define TDSENCHS_MDSP_LOG_DPCCH_BETA_HS_ACK_BMSK 0x07FF
#define TDSENCHS_MDSP_LOG_DPCCH_BETA_HS_ACK_REG  0
#define TDSENCHS_MDSP_LOG_DPCCH_BETA_HS_ACK_SHFT 0

#define TDSENCHS_MDSP_LOG_DPCCH_CQI_NUM_BMSK 0xF800
#define TDSENCHS_MDSP_LOG_DPCCH_CQI_NUM_REG  1
#define TDSENCHS_MDSP_LOG_DPCCH_CQI_NUM_SHFT 11

#define TDSENCHS_MDSP_LOG_DPCCH_BETA_HS_CQI_BMSK 0x07FF
#define TDSENCHS_MDSP_LOG_DPCCH_BETA_HS_CQI_REG  1
#define TDSENCHS_MDSP_LOG_DPCCH_BETA_HS_CQI_SHFT 0

#define TDSENCHS_MDSP_LOG_DPCCH_TX_AGC_PA_ACK_BMSK 0x1FFF
#define TDSENCHS_MDSP_LOG_DPCCH_TX_AGC_PA_ACK_REG  2
#define TDSENCHS_MDSP_LOG_DPCCH_TX_AGC_PA_ACK_SHFT 0

#define TDSENCHS_MDSP_LOG_DPCCH_TX_AGC_PA_CQI_BMSK 0x1FFF
#define TDSENCHS_MDSP_LOG_DPCCH_TX_AGC_PA_CQI_REG  3
#define TDSENCHS_MDSP_LOG_DPCCH_TX_AGC_PA_CQI_SHFT 0

/* This macro gets the value form mDSP decode status buffer (W16 array)
   for the given field name */
#define TDSENCHS_MDSP_LOG_FIELD_VAL(buffer, field) \
  (((buffer)[TDSENCHS_MDSP_LOG_##field##_REG] & TDSENCHS_MDSP_LOG_##field##_BMSK) >> \
   TDSENCHS_MDSP_LOG_##field##_SHFT)
   
#endif /* FEATURE_HSDPA_L1_STATS */

/*Number of new HS Types added with MIMO*/
#define TDSENCHS_NUM_MIMO_RHS_TYPES 5

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

#ifdef FEATURE_HSDPA_L1_STATS

/* This enum defines various actions for HS UL DPCCH logging */
typedef enum 
{
  TDSENCHS_LOG_NOOP,
  TDSENCHS_LOG_START,
  TDSENCHS_LOG_RESTART,
  TDSENCHS_LOG_STOP
} tdsenchs_log_action_enum_type;

#endif /* FEATURE_HSDPA_L1_STATS */

/* Call back function type declaration */
/* ----------------------------------- */

/* Enum type declaration */
/* --------------------- */

/* Different Rhs values types. These corresponds to various type of
   infomration transmitted */
typedef enum
{
  TDSENCHS_RHS_VAL_TYPE_BC,
  TDSENCHS_RHS_VAL_TYPE_BD,
  TDSENCHS_RHS_VAL_TYPE_BHS,
  TDSENCHS_RHS_VAL_TYPE_RA,
  TDSENCHS_NUM_RHS_VAL_TYPE
} tdsenchs_rhs_val_type_enum_type;

/* Structure type declaration */
/* -------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
/*This flag is used to indicate to flush the eul dpch logpkt because of HS
 recfg/stop*/
extern boolean  tdsenchs_eul_dpch_logpkt_flush_hs_data;
#endif


/* =======================================================================
**                        Function Declarations
** ======================================================================= */


/*===========================================================================
FUNCTION tdsenchs_init

DESCRIPTION
  This function initialize encode HS beta table related variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsenchs_init(void);

/*===========================================================================
FUNCTION tdsenchs_create_beta_table

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

extern void tdsenchs_create_beta_table(
  /* delta CQI (0..8) */
  uint8 delta_cqi,
  /* delta ACK (0..8) */
  uint8 delta_ack,
  /* delta NACK (0..8) */
  uint8 delta_nack
  );

/*===========================================================================
FUNCTION tdsenchs_switch_beta_table_index

DESCRIPTION
  This function set current beta table index to ordered beta table index
  and ordred beta table is reset to INVALID.
  This is done with TDS_INTLOCK as usage of beta table index can be in UL
  interrupt context.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Beta table is computed and updated at ordered beta table index
===========================================================================*/

extern void tdsenchs_switch_beta_table_index(void);

#ifdef FEATURE_HSDPA_L1_STATS

/* ---------------------------------------------------------- */
/* Following functions handle UL HS DPCCH information logging */
/* ---------------------------------------------------------- */

/*===========================================================================
FUNCTION tdsenchs_logging_init

DESCRIPTION
  This function initializes the variables required for accumulation 
  and logging of HS UL logging. It initialize the following variables
  
  tdsenchs_log_accum_active is set to FALSE to indicate inactive
  tdsenchs_log_action is set to NOOP
  All HSDPA UL log buffers are set to available
  Log to submit request id set to FALSE
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  All above mentioned variables in descriptions are inited
===========================================================================*/

extern void tdsenchs_logging_init(void);

/*===========================================================================
FUNCTION tdsenchs_init_log_buffer

DESCRIPTION
  This function is called at the start of HS channel bringup or at the start
  of logging. this functions allocates a buffer to accumulate the log samples
  and populates the log header.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  One of the log buffer is marked unavailable.
===========================================================================*/

extern void tdsenchs_init_log_buffer(uint8 this_cfn);

#endif /* FEATURE_HSDPA_L1_STATS */

#ifdef  FEATURE_TDSCDMA_L1_LOGGING_ENABLED 
/*===========================================================================
FUNCTION     ENCHS_GET_EUL_DPCH_LOG_PKT_HS_INFO

DESCRIPTION  This function populates the hs info for every sample in eul dpch 
             log packet 

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None
===========================================================================*/
extern void tdsenchs_update_eul_dpch_logpkt_hs_info(void);
/*===========================================================================
FUNCTION     tdsenchs_eul_dpch_logpkt_do_hs_action

DESCRIPTION  This function changes EUL DPCH logpkt's HS UL logging mode. 
             It updates the cfnx5 offset ,logging action and stop sub fr number 
             so that the hs logging uses the info properly during start/reconfig
             and stop
  
DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None
===========================================================================*/
extern void tdsenchs_eul_dpch_logpkt_do_hs_action(  
/* Action for HS UL logging in eul dpch logpkt */
  tdsenchs_log_action_enum_type log_action,
  /* Sub frame number at which to start or restart */
  uint16 start_sub_fn,
  /* start CFN to start or restart logging */
  uint16 start_cfn,
  /* mDSP info table index from where to read the information */
  uint8  info_table_index,
  /* Last sub frame number after which information is submitted for logging
     then either restarted ot stopped based on action */
  uint16 final_sub_fn);
/*===========================================================================
FUNCTION     tdsenchs_eul_dpch_logpkt_hs_logging_init

DESCRIPTION  This function initializes the variables required for  logging of 
             EUL dpch HS UL logging. 

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS All athe variables used in eul dpch HS logging are initialized
===========================================================================*/
extern void tdsenchs_eul_dpch_logpkt_hs_logging_init(void);
#endif /*end of   FEATURE_TDSCDMA_L1_LOGGING_ENABLED */


#endif /* TDSENCHS_H */

