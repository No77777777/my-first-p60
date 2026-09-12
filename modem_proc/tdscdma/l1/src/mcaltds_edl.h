#ifndef MCALTDS_EDL_H
#define MCALTDS_EDL_H

/*============================================================================*/
/** @file  
 * This module has definition and declaration related to MCAL WCDMA EDL (EUL Downlink)
 * module APIs
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2007 - 2008 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_edl.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------   
01/12/11    weijunz initial edit for TDSCDMA HSUPA
===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#ifdef FEATURE_MCAL_TDSCDMA
#include "customer.h"
#include "comdef.h"
#include "mcaltds_dl.h"



/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define MCALTDS_EDL_MAX_RL                      4

#define MCALTDS_EDL_MAX_EAGCH_CHAN_NUM          4
#define MCALTDS_EDL_MAX_EHICH_CHAN_NUM          4


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */







/*EUL channel action types*/
typedef enum 
{
  MCALTDS_EDL_CHAN_DISABLE,
  MCALTDS_EDL_CHAN_ENABLE,
  MCALTDS_EDL_CHAN_RECONFIG,
  MCALTDS_EDL_CHAN_NO_ACTION
} mcaltds_edl_chan_action_enum_type;

typedef enum
{
  MCALTDS_EDL_STATE_INACTIVE,
  MCALTDS_EDL_STATE_ACTIVE,
  MCALTDS_EDL_STATE_CONFIG_START,
  MCALTDS_EDL_STATE_CONFIG_RECFG,
  MCALTDS_EDL_STATE_CONFIG_STOP
} mcaltds_edl_state_enum_type;


typedef struct
{
  uint8 ts_num ;
  uint8 first_chan_code ;
  uint8 second_chan_code ; 

} mcaltds_edl_eagch_chan_code_struct_type ;

#if 0
typedef enum {
	MCALTDS_DL_DEFAULT_MIDAMBLE,
	MCALTDS_DL_COMMON_MIDAMBLE,
	MCALTDS_DL_UE_SPECIFIC_MIDAMBLE,
	MCALTDS_DL_INVALID_MIDAMBLE
} mcaltds_edl_midamble_alloc_mode_enum_type;

typedef enum {
	MCALTDS_DL_MIDAMBLE_CONFIG_2,
	MCALTDS_DL_MIDAMBLE_CONFIG_4,
	MCALTDS_DL_MIDAMBLE_CONFIG_6,
	MCALTDS_DL_MIDAMBLE_CONFIG_8,
	MCALTDS_DL_MIDAMBLE_CONFIG_10,
	MCALTDS_DL_MIDAMBLE_CONFIG_12,
	MCALTDS_DL_MIDAMBLE_CONFIG_14,
	MCALTDS_DL_MIDAMBLE_CONFIG_16,
	MCALTDS_DL_NUM_MIDAMBLE_CONFIG
}mcaltds_edl_midamble_config_enum_type;
#endif

typedef struct
{
    uint8 ts_num ;
    mcaltds_dl_midamble_alloc_mode_enum_type allocate_mode ;
    mcaltds_dl_midamble_config_enum_type mid_config ;
    uint8 code_shift ;

} mcaltds_edl_midamble_info_struct_type ;

typedef struct 
{
  /*To be Kept*/
  boolean                    new_cfg;

  /*To Be kept*/
  mcaltds_edl_chan_action_enum_type action;

  /*Added for TDSCDMA HSUPA*/
  boolean rdi_pre ;
  uint16 ernti ;
  uint16 start_sub_frame_num ;
  uint8 num_eagch ;
  mcaltds_edl_eagch_chan_code_struct_type code_info[MCALTDS_EDL_MAX_EAGCH_CHAN_NUM] ;
  mcaltds_edl_midamble_info_struct_type midamble_info[MCALTDS_EDL_MAX_EAGCH_CHAN_NUM] ;

} mcaltds_edl_agch_info_struct_type;

typedef struct
{
  uint8 ts_num ;
  uint8 chan_code ;
  uint8 ei ;
  
} mcaltds_edl_ehich_chan_code_struct_type ;

typedef struct 
{
  boolean new_cfg;
  
  mcaltds_edl_chan_action_enum_type action;
 
 /*TDS HSUPA*/
 uint8 n_ehich ;
 uint16 start_sub_frame_num ;
 uint8 num_ehich ;
 mcaltds_edl_ehich_chan_code_struct_type code_info[MCALTDS_EDL_MAX_EHICH_CHAN_NUM] ;
 mcaltds_edl_midamble_info_struct_type midamble_info[MCALTDS_EDL_MAX_EHICH_CHAN_NUM] ;

} mcaltds_edl_sched_hich_info_struct_type;


/*dl config db */
typedef struct 
{
 
  /*MCAL state machine isn't neede. To be removed */
  mcaltds_edl_state_enum_type state; 

  mcaltds_edl_agch_info_struct_type agch_info;

  mcaltds_edl_sched_hich_info_struct_type sched_hich_info;
 
} mcaltds_edl_channel_db_struct_type;

typedef struct
{
    boolean new_cfg ;
    mcaltds_edl_chan_action_enum_type action ;
    uint16 start_sub_fn ;
    /*minmum number of time slots between E-PUCH and E-HICH*/
    uint8 n_ehich;
    /*E-HICH signature group index*/
    uint8 sig_group_idx ;
    /*E-HICH time slot number*/
    uint8 ts_num ;
    /*non shceduled E-HICH chan code*/
    uint8 chan_code ;
    /*Non scheduled E-HICH middle amble info*/
    mcaltds_dl_midamble_alloc_mode_enum_type allocate_mode ;
    mcaltds_dl_midamble_config_enum_type mid_config ;
    uint8 code_shift ;
} mcaltds_edl_non_sched_hich_db_struct_type ;

typedef struct
{
  /** ID of the event used by the MCAL sequencer for channel configuration 
    * purposes */
  uint8 evt_id;

 
  mcaltds_edl_channel_db_struct_type *channel_db_ptr;
} mcaltds_edl_config_db_struct_type;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION MCALTDS_EDL_SEND_EAGCH_CFG_CMD

DESCRIPTION     This function is the API to be called by the EDL Host layer to
                 configure EDL EAGCH channels.

DEPENDENCIES     R99 DPCH channel must be already setup and the Layer-1 must
                 be aligned to that already.

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
boolean mcaltds_edl_send_eagch_cfg_cmd(mcaltds_edl_agch_info_struct_type *agch_mcal_db_ptr);

/*===========================================================================

FUNCTION MCALTDS_EDL_SEND_SCHED_EHICH_CFG_CMD

DESCRIPTION     This function is the API to be called by the EDL Host layer to
                 configure EDL Sched EHICH channels.

DEPENDENCIES     R99 DPCH channel must be already setup and the Layer-1 must
                 be aligned to that already.

RETURN VALUE    boolean

SIDE EFFECTS    None

===========================================================================*/
boolean mcaltds_edl_send_sched_ehich_cfg_cmd(mcaltds_edl_sched_hich_info_struct_type *sched_hich_mcal_db_ptr);

/*===========================================================================

FUNCTION MCALTDS_EDL_SEND_NON_SCHED_EHICH_CFG_CMD

DESCRIPTION     This function is the API to be called by the EDL Host layer to
                 configure EDL Non Sched EHICH channels.

DEPENDENCIES     R99 DPCH channel must be already setup and the Layer-1 must
                 be aligned to that already.

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
boolean mcaltds_edl_send_non_sched_ehich_cfg_cmd(mcaltds_edl_non_sched_hich_db_struct_type *non_sched_hich_mcal_db_ptr);


extern void mcaltds_eagch_decode_done_isr_handler_cb(void);

extern void mcaltds_schld_ehich_decode_done_isr_handler_cb(void);

extern void mcaltds_non_schld_ehich_decode_done_isr_handler_cb(void);



#endif /* FEATURE_MCAL_TDSCDMA */
#endif

