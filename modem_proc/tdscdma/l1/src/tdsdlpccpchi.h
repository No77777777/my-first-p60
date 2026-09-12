#ifndef TDSDLPCCPCH_I_H
#define TDSDLPCCPCH_I_H

/*===========================================================================
                               T D S D L P C C P C H I . H


GENERAL DESCRIPTION

  This file contains the declarations and definitions that are internal to
  dlbcchmgr.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright(c)2007 - 2008 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlpccpchi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------

===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "tdsl1const.h"
#include "tdsdlchmgr.h"
#include "tdsdlphch.h"
#include "tdsl1std.h"
#include "tdsdlpccpch.h"

/*--------------------------------------------------------------------------
                      Included Header Files
--------------------------------------------------------------------------*/

#define TDSDLPCCPCH_AVOID_SUB_FRAME_CX8           0x4050
#define TDSDLPCCPCH_AVOID_SUB_FRAME_CX1           0x80A
#define TDSDLPCCPCH_SLAM_LINE_CX8_START           0x8800
#define TDSDLPCCPCH_SLAM_LINE_CX8_END             0x10


/*------------------GLOBAl VARIABLES DEFINITION BEGIN------------------------------------*/
typedef struct
{
  uint16 chipx8;
  uint16 subsfn;
  uint32 counter;  
}tdsdlpccpch_time_counter_struct_type;
  
typedef struct
{
  /*chipx8 and subsfn value of wall time from FW*/
  tdsdlpccpch_time_counter_struct_type wt_from_fw;
  /*wall time value from stmr dump*/
  tdsdlpccpch_time_counter_struct_type wt_from_dump;
  /*rx time value from stmr dump*/
  tdsdlpccpch_time_counter_struct_type rx_from_dump;  
  /*when wall time is this wall time target value, seq triggers slam operation*/
  tdsdlpccpch_time_counter_struct_type wt_target;
  /*subsfn and chipx8 value of slam value for rx counter*/
  tdsdlpccpch_time_counter_struct_type rx_slam;
   /*offset value of wall time and rx time*/
  tdsdlpccpch_time_counter_struct_type offset_wt_rx;  
   /*offset value of frequency*/
  int16 freq_offset;
}tdsdlpccpch_counter_struct_type;
  
/* This complicated macro looks at the SIB schedule received from RRC,  It
** uses the current SFN to determine the next SFN with SIB data.  In other
** words it figures out where we are in the SIB schedule. */


/* The number of bits in a single element of the bitmask. 
** Used to speed up searching the bitmask. */
#define TDSDLPCCPCH_BITMASK_ELEM_SIZE  8

#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
/*define maximum time waiting for pccpch decode result in shallow mode*/
/* Defined as 1s, need system to confirm */
#define TDSDLPCCPCH_SHALLOW_DETECTION_TIMER 45

/*define maximum time waiting for pccpch decode result in deep mode*/
/* Defined as 1s, need system to confirm */
#define TDSDLPCCPCH_DEEP_DETECTION_TIMER 45

#else
/*define maximum time waiting for pccpch decode result in shallow mode*/
/* Defined as 1s, need system to confirm */
#define TDSDLPCCPCH_SHALLOW_DETECTION_TIMER 160

/*define maximum time waiting for pccpch decode result in deep mode*/
/* Defined as 1s, need system to confirm */
#define TDSDLPCCPCH_DEEP_DETECTION_TIMER 160

#endif

/*define pccpch slam delta time value when receive FW isr*/
#define TDSDLPCCPCH_SLAM_DELTA 8192  // changed for Trion 3.0.1 only: slow HW

#define TDSLPCCPCH_CHIPX8_SUB(Y,Z) \
        (Y >= Z)? (Y-Z) : (TDSL1_CX8_PER_SUB_FRAME + Y -Z)

#define TDSLPCCPCH_SUBFRAME_SUB(Y,Z) \
        (Y >= Z)? (Y-Z) : (TDSL1_MAX_SUB_FRAME_NUM + Y -Z)

/*Macro to indicate bcch manager invalid dynamic db index*/
#define TDSDLPCCPCH_INVALID_DYN_DB_IDX (TDSDLCHMGR_INVALID_CH_SPEC_IDX)

/*Macro to indicate the minimum energy a bcch block should have to be 
considered to be passed*/
#define TDSDLPCCPCH_DOB_ENERGY_THRESHOLD 0x100

/*Macro to check whether a bcch static db index is invalid*/
#define TDSDLPCCPCH_IS_STATIC_DB_IDX_INVALID(static_db_idx)  \
  (static_db_idx >= TDSDLPCCPCH_STAT_DB_SIZE)

/*Macro to do a preliminary check on a bcch crc status command
before running the crc state machine*/
#define TDSDLPCCPCH_CRC_CMD_PRELIM_CHECK_FAILED(cmd)    \
  ((cmd->eng < TDSDLPCCPCH_DOB_ENERGY_THRESHOLD) || (cmd->sfn == 0))    \

#define TDSDLPCCPCH_IS_CFG_DB_IDX_NOT_IN_USE()    \
  (tdsdlpccpch_cfg_db.dyn_db.in_use == FALSE )

#define TDSDLPCCPCH_IS_CFG_DB_IDX_IN_USE    \
  (tdsdlpccpch_cfg_db.dyn_db.in_use == TRUE )

#define TDSDLPCCPCH_GET_STATIC_DB_IDX()    \
    (tdsdlpccpch_cfg_db.dyn_db.static_db_idx)

#define TDSDLPCCPCH_IS_CCTRCH_IDX_INVALID(cctrch_idx)    \
  (TDSDLCHMGR_IS_CCTRCH_IDX_INVALID(cctrch_idx))

#define TDSDLPCCPCH_IS_BCCH_DB_IDX_INVALID(cfg_db_idx)    \
  (cfg_db_idx == TDSDLPCCPCH_INVALID_DYN_DB_IDX)

#define TDSDLPCCPCH_DOES_MAC_NEEDS_BCH_FROM_CFG_DB_IDX()    \
    (tdsdlpccpch_cfg_db.stat_db.turn_bch_on_to_mac)

#define TDSDLPCCPCH_SHOULD_CHANNEL_BE_SLAMMED_FROM_CFG_DB_IDX()    \
    (tdsdlpccpch_cfg_db.stat_db.reslam_needed) //jintaoh

#define TDSDLPCCPCH_GET_PHCH_HDLE(cfg_db_idx)    \
    (TDSDLCHMGR_GET_PHCH_HDLE_FROM_CFG_DB_IDX(TDSDLPCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLPCCPCH_GET_CRC_STATUS()    \
    (tdsdlpccpch_cfg_db.dyn_db.crc_status)

#define TDSDLPCCPCH_GET_SHALLOW_TIME()    \
    (tdsdlpccpch_cfg_db.stat_db.max_detection_time_shallow)

#define TDSDLPCCPCH_GET_DEEP_TIME()    \
      (tdsdlpccpch_cfg_db.stat_db.max_detection_time_deep)

#define TDSDLPCCPCH_GET_CCTRCH_IDX_FROM_CFG_DB_IDX()    \
    (TDSDLCHMGR_GET_CCTRCH_IDX_FROM_CFG_DB_IDX(TDSDLPCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLPCCPCH_GET_DRX_HDLE_FROM_CFG_DB_IDX()    \
    (tdsdlpccpch_cfg_db.dyn_db.drx_handle)

#define TDSDLPCCPCH_IS_DRX_HDLE_INVALID_FROM_CFG_DB_IDX()    \
    (TDSDLPCCPCH_GET_DRX_HDLE_FROM_CFG_DB_IDX() == TDSDRXMGR_ENTITY_HANDLE_INVALID)

#define TDSDLPCCPCH_GET_DRX_ENTITY_TYPE_FROM_CFG_DB_IDX()   \
    (tdsdlpccpch_cfg_db.stat_db.drx_type)

#define TDSDLPCCPCH_GET_DRX_CB_FUNC_FROM_CFG_DB_IDX()    \
    (tdsdlpccpch_cfg_db.stat_db.drx_cb_functions)

#define TDSDLPCCPCH_GET_CHANNEL_ADD_STATUS()  \
     ((TDSDLCHMGR_GET_CURR_ADD_STATE_FROM_CFG_DB_IDX(TDSDLPCCPCH_GET_CHMGR_CFG_DB_IDX()))== TDSDLPHCH_STATE_ACTIVE)

#define TDSDLPCCPCH_SET_CHANNEL_ADD_STATUS(status)  \
     ((TDSDLCHMGR_GET_CURR_ADD_STATE_FROM_CFG_DB_IDX(TDSDLPCCPCH_GET_CHMGR_CFG_DB_IDX()))= status)

#define TDSDLPCCPCH_GET_AWAKE_STATUS()    \
((TDSDLCHMGR_GET_SUSPEND_RESUME_STATE_FROM_CFG_DB_IDX(TDSDLPCCPCH_GET_CHMGR_CFG_DB_IDX()) == TDSDLCHMGR_RESUMED_STATUS)  || \
    (TDSDLCHMGR_GET_SUSPEND_RESUME_STATE_FROM_CFG_DB_IDX(TDSDLPCCPCH_GET_CHMGR_CFG_DB_IDX()) == TDSDLCHMGR_RESUME_ONGOING_STATUS))

/*Structure that maps a bcch type from chmgr static db type to bcch manager static db type*/
typedef struct {
tdsdlchmgr_static_db_idx_type dlchmgr_static_db_idx;

tdsdlpccpch_static_db_idx_enum_type bcch_static_db_idx;
} tdsdlpccpch_stat_idx_map_struct_type;

//static tdsdrxmgr_channel_cb_struct_type  tdsdlpccpch_drx_cb_functions;

/*------------------GLOBAl VARIABLES DEFINITION END------------------------------------*/

/*------------------STATIC FUNCTIONS DECLARATION BEGIN------------------------------------*/

/*===========================================================================
FUNCTION        tdsdlpccpch_prep_db

DESCRIPTION     This function resets an entry in bcch manager dynamic db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
//commented out for q6 build, owen
//void tdsdlpccpch_prep_db(void);

/*===========================================================================
FUNCTION        tdsdlpccpch_find_stat_db_idx

DESCRIPTION     This function finds the ch-spec manager dynamic db index 
                associated with the channel identified by the CCTrCh.

DEPENDENCIES    None

RETURN VALUE
                ch-spec-db-idx: The ch-specific db index of the channel identified by the input parameter.

SIDE EFFECTS    None
===========================================================================*/
uint8 tdsdlpccpch_get_cfg_db_idx(uint8 cctrch_idx);

/*===========================================================================
FUNCTION        tdsdlpccpch_crc_post_process

DESCRIPTION     This function does all the post-processing after the CRC 
                state machine is over.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
//commented out for q6 build, owen
//void tdsdlpccpch_crc_post_process(boolean pass, tdsdlpccpch_bch_crc_status_type *cmd);

/*===========================================================================
FUNCTION        tdsdlpccpch_find_stat_db_idx

DESCRIPTION     This function finds the static db index associated with the channel identified by 
                the input parameter.

DEPENDENCIES    None

RETURN VALUE
                static-db-idx: The static db index of the channel identified by the input parameter.

SIDE EFFECTS    None
===========================================================================*/
//commented out for q6 build, owen
//tdsdlpccpch_static_db_idx_enum_type tdsdlpccpch_find_stat_db_idx
//  (tdsdlchmgr_cfg_db_idx_type chmgr_dyn_db_idx);

/*===========================================================================
FUNCTION        tdsdlpccpch_slam

DESCRIPTION     This function handles post-reslam operations.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
//commented out for q6 build, owen
//boolean tdsdlpccpch_slam(tdsdlpccpch_bch_crc_status_type *cmd);

/*===========================================================================
FUNCTION        dl_bcch_dealloc_dyn_db_idx

DESCRIPTION     This function frees entry in the bcch manager dynamic db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
void tdsdlpccpch_dealloc_cfg_db_idx(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);


/*===========================================================================
FUNCTION        dl_bcch_alloc_dyn_db_idx

DESCRIPTION     This function finds a free entry in the bcch manager dynamic 
                db and allocates it to the caller.

DEPENDENCIES    None

RETURN VALUE
                cfg-db-idx: The allotted index in bcch manager dynamic db.

SIDE EFFECTS    None.
===========================================================================*/
tdsdlchmgr_dlch_spec_cfg_db_idx_type tdsdlpccpch_alloc_cfg_db_idx(tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx);


/*===========================================================================
FUNCTION        tdsdlpccpch_pre_add_cb

DESCRIPTION     This function does any processing needed before the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-add operations were successful.
                FALSE: At least one pre-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlpccpch_pre_add_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx,
  tdsl1_dl_phycfg_phychan_db_struct_type *phychan_db);

/*===========================================================================
FUNCTION        tdsdlpccpch_post_add_cb

DESCRIPTION     This function does any processing needed after the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-add operations were successful.
                FALSE: At least one post-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlpccpch_post_add_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpccpch_pre_drop_cb

DESCRIPTION     This function does any processing needed before the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-drop operations were successful.
                FALSE: At least one pre-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlpccpch_pre_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);
/*===========================================================================
FUNCTION        tdsdlpccpch_post_drop_cb

DESCRIPTION     This function does any processing needed after the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-drop operations were successful.
                FALSE: At least one post-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlpccpch_post_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);
/*===========================================================================
FUNCTION        tdsdlpccpch_post_resume_cb

DESCRIPTION     This function does any processing needed after the resumption 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-resumption operations were successful.
                FALSE: At least one post-resumption operation failed.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlpccpch_post_resume_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpccpch_post_suspend_cb

DESCRIPTION     This function does any processing needed after the suspension 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-suspension operations were successful.
                FALSE: At least one post-suspension operation failed.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlpccpch_post_suspend_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION   DL_BCCH_NEXT_FRAME_WANTED_CB

DESCRIPTION
  DRX Manager invokes this callback routine to find out how long a BCCH wants
  to be active or sleeping, whichever the case may be.  So this function
  effectively implements the scheduling of inter-SIB sleep.

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE on success (always returns TRUE).
  
SIDE EFFECTS
  None.
===========================================================================*/
boolean tdsdlpccpch_next_frame_wanted_cb(
  tdsdrxmgr_entity_handle_type          entity_handle,
  tdsdrxmgr_sleep_struct_type  *sleep_info_ptr
);

//static tdsdlpccpch_static_db_idx_enum_type tdsdlpccpch_find_stat_db_idx
//  (tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx);
//static void tdsdlpccpch_prep_db(void);
//static boolean tdsdlpccpch_slam(tsdl1dec_bch_status_info_struct_type *bch_status_info);

/*===========================================================================
FUNCTION        tdsdlpccpch_get_bitmask_cb

DESCRIPTION     This function calculate the bitmask for pccpch.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
tdsdlchmgr_spec_tdm_bitmask_type tdsdlpccpch_get_bitmask_cb(uint16 actviation_time);

/*===========================================================================
FUNCTION        tdsdlpccpch_add_status

DESCRIPTION     This function return add status.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
tdsdlphch_state_enum_type tdsdlpccpch_add_status(uint16 actviation_time);

#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
/*===========================================================================
FUNCTION        tdsdlpccpch_submit_int_pccpch_status_logpkt

FILE NAME       

DESCRIPTION     

DEPENDENCIES                

RETURN VALUE   

SIDE EFFECTS   
 
===========================================================================*/
LOCAL void tdsdlpccpch_submit_int_pccpch_status_logpkt(void);
#endif

/*------------------STATIC FUNCTIONS DECLARATION END------------------------------------*/
#endif

