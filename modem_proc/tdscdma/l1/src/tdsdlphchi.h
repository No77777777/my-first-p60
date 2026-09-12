#ifndef TDSDLPHCHI_H
#define TDSDLPHCHI_H

/*===========================================================================
                 TDSCDMA L1 DOWNLINK PHYSICAL CHANNEL CONFIGURATION

GENERAL DESCRIPTION
  This file contains the code for handling validation, configuration and
  management of downlink physical channels 
  (PCCPCH, SCCPCH,  DPCH and PICH).

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2010 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/dlcmd.c_v   1.109   02 Jul 2002 18:11:42   cemani  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlphchi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------
11/22/10   YH      Revise this file from UMTS to TDSCDMA.
7/5/11      YH      Cleanup code for TD product.
===========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "tdsdlphychanlog.h"

#include "tdsl1rrcif.h"
#include "tdsl1const.h"
#include "mcaltds_dl.h"
#include "tdsdlphch.h"

/*+TDSDLPHCH Development-----------------------------------------------------------------------*/
/* ==========================================================================
** Macro Definitions
** ========================================================================*/
/* -----------------------------------------------------------------------
** Macro constant Definitions
** ----------------------------------------------------------------------*/
#define TDSDLPHCH_MAX_SLOT_FORMAT 25
#define TDSDLPHCH_MAX_N_PI 3
#define TDSDLPHCH_MAX_N_PICH 9
#define TDSDLPHCH_MAX_PI_LEN 3
#define TDSDLPHCH_MAX_CHANCODE 16
#define TDSDLPHCH_SF1_CHANCODE_BITMAP 0
#define TDSDLPHCH_SF1_CHANCODE 1
#define TDSDLPHCH_NO_TFCI_BIT 0
#define TDSDLPHCH_NO_SLOT_FORMAT 0
/* -----------------------------------------------------------------------
** Action mask Definitions
** ----------------------------------------------------------------------*/
#define TDSDLPHCH_ACTIONS_ALLOCATE_CFG_DB_BMSK                               0x00000001
#define TDSDLPHCH_ACTIONS_DEALLOCATE_CFG_DB_BMSK                             0x00000002
#define TDSDLPHCH_ACTIONS_ISSUE_MCAL_ADD_BMSK                                0x00000004
#define TDSDLPHCH_ACTIONS_ISSUE_MCAL_DROP_BMSK                               0x00000008
#define TDSDLPHCH_ACTIONS_ISSUE_MCAL_RECFG_BMSK                              0x00000010
#define TDSDLPHCH_ACTIONS_ISSUE_MCAL_DROP_OLD_CFG_BMSK                       0x00000020
#define TDSDLPHCH_ACTIONS_ENABLE_CCTRCH_BMSK                                 0x00000040
#define TDSDLPHCH_ACTIONS_DISABLE_CCTRCH_BMSK                                0x00000080
#define TDSDLPHCH_ACTIONS_FREE_CCTRCH_BMSK                                   0x00000100
#define TDSDLPHCH_ACTIONS_FREE_OLD_CCTRCH_N_DEALLOCATE_OLDCFG_DB_BMSK        0x00000200
#define TDSDLPHCH_ACTIONS_DEALLOCATE_OLDCFG_DB_BMSK                          0x00000400
#define TDSDLPHCH_ACTIONS_MCAL_OP (TDSDLPHCH_ACTIONS_ISSUE_MCAL_ADD_BMSK| \
                                   TDSDLPHCH_ACTIONS_ISSUE_MCAL_DROP_BMSK| \
                                   TDSDLPHCH_ACTIONS_ISSUE_MCAL_RECFG_BMSK| \
                                   TDSDLPHCH_ACTIONS_ISSUE_MCAL_DROP_OLD_CFG_BMSK)
                                                              
/* ==========================================================================
** Constant Definitions
** ========================================================================*/
const uint8 tdsdlphch_Npi_bits[TDSDLPHCH_MAX_N_PI] = 
{
  88,
  44,
  22
};

const uint8 tdsdlphch_Npich_bits[TDSDLPHCH_MAX_N_PICH] = 
{
  2,
  2,
  4,
  2,
  4,
  2,
  4,
  2,
  4
};


/* ==========================================================================
** Macro condition Definitions
** ========================================================================*/
#define TDSDLPHCH_IS_ACTION_NEEDED(bmsk, phch_cfg_db_idx) \
  (((tdsdlphch_cfg_db[phch_cfg_db_idx].actions_bmsk&bmsk) != 0)?TRUE:FALSE)
 
/* Not done yet */
#define TDSDLPHCH_IS_MCALOP_NEEDED(phch_cfg_db_idx) \
  (((tdsdlphch_cfg_db[phch_cfg_db_idx].actions_bmsk&TDSDLPHCH_ACTIONS_MCAL_OP) != 0)?TRUE:FALSE)

#define TDSDLPHCH_ACTION_SET(bmsk, actions_bmsk) \
  (actions_bmsk|= bmsk)

#define TDSDLPHCH_ACTION_SET_DB(bmsk, phch_cfg_db_idx) \
  (tdsdlphch_cfg_db[phch_cfg_db_idx].actions_bmsk|= bmsk)

#define TDSDLPHCH_ACTION_RESET(bmsk, phch_cfg_db_idx) \
  (tdsdlphch_cfg_db[phch_cfg_db_idx].actions_bmsk &= ~bmsk)
                                                                                             
#define TDSDLPHCH_ACTION_RESET_ALL(phch_cfg_db_idx) \
  (tdsdlphch_cfg_db[phch_cfg_db_idx].actions_bmsk = 0x00)
                                                                                             
#define TDSDLPHCH_ACTION_RESET_ALL_PRE_MCAL(phch_cfg_db_idx) \
  (tdsdlphch_cfg_db[phch_cfg_db_idx].actions_bmsk &= 0xFFFFFF80)
                                                                                             
#define TDSDLPHCH_ACTION_RESET_MCALOPS(phch_cfg_db_idx) \
  (tdsdlphch_cfg_db[phch_cfg_db_idx].actions_bmsk &= ~TDSDLPHCH_ACTIONS_MCAL_OP)
                                                              
#define TDSDLPHCH_IS_OPERATION_GRACEFUL(operation, phch_cfg_db_idx)   \
(((operation == TDSDLPHCH_NOTIFY_DROP) && (tdsdlphch_cfg_db[phch_cfg_db_idx].state == TDSDLPHCH_STATE_INACTIVE)) || \
 ((operation == TDSDLPHCH_NOTIFY_SUSPEND) && (tdsdlphch_cfg_db[phch_cfg_db_idx].state == TDSDLPHCH_STATE_SUSPEND)) || \
 ((operation == TDSDLPHCH_NOTIFY_RESUME) && (tdsdlphch_cfg_db[phch_cfg_db_idx].state == TDSDLPHCH_STATE_ACTIVE)))\

#define TDSDLPHCH_IS_OPERATION_NOT_ALLOWED(operation, phch_cfg_db_idx) \
(((operation == TDSDLPHCH_NOTIFY_ADD) && ((tdsdlphch_cfg_db[phch_cfg_db_idx].state == TDSDLPHCH_STATE_ACTIVE) || (tdsdlphch_cfg_db[phch_cfg_db_idx].state == TDSDLPHCH_STATE_SUSPEND))) ||\
  (((operation == TDSDLPHCH_NOTIFY_RECFG) || (operation == TDSDLPHCH_NOTIFY_SUSPEND) || (operation == TDSDLPHCH_NOTIFY_RESUME)) && (tdsdlphch_cfg_db[phch_cfg_db_idx].state == TDSDLPHCH_STATE_INACTIVE)))\

#define TDSDLPHCH_IS_TFCI_BIT_INVALIDE(tfci_bits)    \
  ((tfci_bits != 0) && (tfci_bits != 4) && (tfci_bits != 8) && (tfci_bits != 16) && (tfci_bits != 32))

#define TDSDLPHCH_IS_SF_INVALIDE(sf)    \
  ((sf != TDSL1_SF_16) && (sf != TDSL1_SF_1))

#define TDSDLPHCH_IS_SF16_CHANCODE_BIT_VALID(chancode_bitmap)    \
 (chancode_bitmap&0x1)  

#define TDSDLPHCH_GET_PI_BITS(pi_length) tdsdlphch_pi_bits[(pi_length)]

#define TDSDLPHCH_GET_PICH_BITS(repetition_period_length) tdsdlphch_pich_bits[(repetition_period_length)]

#define TDSDLPHCH_GET_PI_LEN_VAL(pi_length) tdsdlphch_pi_length[(pi_length)]
/* ==========================================================================
** Variable Definitions
** ========================================================================*/



/* ==========================================================================
** Data structure Definitions
** ========================================================================*/
/*+Yongh add for idle timslot development***********************/
#define TDSDLPHCH_TIEMSLOT_USED_BY_RX 2
typedef struct{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint8 repetition_period;
  uint64 chan_present_mask;
}tdsmeas_dpchdl_config_struct_type;

typedef struct{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint8 repetition_period;
  uint64 chan_present_mask;
}tdsmeas_sccpch_config_struct_type;

typedef struct{
  uint16 idleslot_config_time;
  boolean pccpch_on;
}tdsmeas_pccpch_config_struct_type;

typedef mcaltds_dl_pccpch_cfg_struct_type tdsdlphch_dl_pccpch_cfg_parms_struct_type;
typedef mcaltds_dl_sccpch_cfg_struct_type tdsdlphch_dl_sccpch_cfg_parms_struct_type;
typedef mcaltds_dl_dpch_cfg_struct_type tdsdlphch_dl_dpch_cfg_parms_struct_type;
typedef mcaltds_dl_pich_cfg_struct_type tdsdlphch_dl_pich_cfg_parms_struct_type;
/*-Yongh add for idle timslot development***********************/
/* ==========================================================================
** Function Definitions
** ========================================================================*/
/*--------------------------------------------------------------------------
                 DL PHCH Notification Handler Functions
                 1. tdsdlphch_add_notify
                 2. tdsdlphch_recfg_notify
                 3. tdsdlphch_drop_notify
                 4. tdsdlphch_suspend_notify
                 5. tdsdlphch_resume_notify
--------------------------------------------------------------------------*/

/*===========================================================================
FUNCTION tdsdlphch_add_notify

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdlphch_add_notify(
  /* phychan cfg index for which add notification process needed */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx,

  /* status if the add operation is successful or not */
  boolean status);

/*===========================================================================
FUNCTION tdsdlphch_recfg_notify

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdlphch_recfg_notify(
  /* phychan cfg index for which add notification process needed */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx, 

 /* status if the add operation is successful or not */
  boolean status);

/*===========================================================================
FUNCTION tdsdlphch_drop_notify

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdlphch_drop_notify(
  /* phychan cfg index for which add notification process needed */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx, 

 /* status if the add operation is successful or not */
  boolean status);

/*===========================================================================
FUNCTION tdsdlphch_suspend_notify

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdlphch_suspend_notify(
  /* phychan cfg index for which add notification process needed */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx, 

  /* status if the add operation is successful or not */
  boolean status);

/*===========================================================================
FUNCTION tdsdlphch_resume_notify

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdlphch_resume_notify(
  /* phychan cfg index for which add notification process needed */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx, 

  /* status if the add operation is successful or not */
  boolean status);

/*===========================================================================
FUNCTION tdsdlphch_notify

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsdlphch_notify(
  /* access to physical channel database */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx, 

  /* status for notification */
  boolean status,

  /* notification function ptr */
  TDSDLPHCH_CFG_DONE_CB_FUNC_TYPE *cfg_done_cb);


/*--------------------------------------------------------------------------
                 DL PHCH Database Update Functions
                 1. tdsdlphch_copy_pccpch_info
                 2. tdsdlphch_copy_sccpch_info
                 3. tdsdlphch_copy_dpch_info
                 4. tdsdlphch_copy_pich_info
--------------------------------------------------------------------------*/
/*===========================================================================
FUNCTION tdsdlphch_copy_pccpch_info

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsdlphch_copy_pccpch_info(
   /* access to physical channel database */
   tdsdlphch_cfg_handle_type phch_cfg_db_idx, 

   /*phychan confguration pointer */
  tdsdlphch_add_type *phychan_db_ptr );
/*===========================================================================
FUNCTION tdsdlphch_copy_sccpch_info

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsdlphch_copy_sccpch_info(
  /* to which cfg db location the information should be copied */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx,
  
  /*phychan confguration pointer */
  tdsdlphch_add_type *phychan_db_ptr );

/*===========================================================================
FUNCTION tdsdlphch_copy_dpch_info

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsdlphch_copy_dpch_info(
  /* to which cfg db location the information should be copied */
  tdsdlphch_cfg_handle_type phch_cfg_db_idx,

   /*phychan confguration pointer */
  tdsdlphch_add_type *phychan_db_ptr );


/*===========================================================================
FUNCTION tdsdlphch_copy_pich_info

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsdlphch_copy_pich_info(
  /* to which cfg db location the information should be copied */
  tdsdlphch_cfg_handle_type cfg_db_idx,
  
   /*phychan confguration pointer */
  tdsdlphch_add_type *phychan_db_ptr );




/*--------------------------------------------------------------------------
                 DL PHCH Database Maintenance Functions
                 1. tdsdlphch_allocate_cfg_db
                 2. tdsdlphch_allocate_rl_db
                 3. tdsdlphch_deallocate_cfg_db
                 4. tdsdlphch_deallocate_rl_db
--------------------------------------------------------------------------*/

/*===========================================================================
FUNCTION tdsdlphch_allocate_cfg_db

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
tdsdlphch_cfg_handle_type tdsdlphch_allocate_cfg_db(void);

/*===========================================================================
FUNCTION tdsdlphch_deallocate_cfg_db

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

void tdsdlphch_deallocate_cfg_db(tdsdlphch_cfg_handle_type phch_cfg_db_idx);

/*===========================================================================
FUNCTION dl_phch_configure

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsdlphch_configure(
    tdsdlphch_cfg_handle_type phch_cfg_db_idx,
    tdsdlphch_notify_enum_type operation,
    tdsdlphch_state_enum_type phch_next_state,
    TDSDLPHCH_CFG_DONE_CB_FUNC_TYPE *phch_done_cb_ptr );

/*===========================================================================
FUNCTION tdsdlphch_configure_undo

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdlphch_configure_undo(uint32 bmsk, tdsdlphch_cfg_handle_type phch_cfg_db_idx);

/*===========================================================================
FUNCTION tdsdlphch_configure_ops

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsdlphch_configure_ops(
  tdsdlphch_cfg_handle_type phch_cfg_db_idx,
  tdsdlphch_notify_enum_type operation,
  TDSDLPHCH_CFG_DONE_CB_FUNC_TYPE *phch_done_cb_ptr );

/*===========================================================================
FUNCTION tdsdlphch_update_actions_mask

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern uint32 tdsdlphch_update_actions_mask( 
  tdsdlphch_notify_enum_type operation,
  tdsdlphch_state_enum_type phch_cur_state,
  tdsdlphch_state_enum_type phch_next_state );

/*===========================================================================
FUNCTION        tdsdlphch_hho_save_phch_params_info

DESCRIPTION     This function saves necessary phch params which may be used for
                resuming back to old config upon HHO failure.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void tdsdlphch_hho_save_phch_params_info(void);

/*===========================================================================
FUNCTION tdsdlphch_cfg_db_init

DESCRIPTION
  This function initializes all the information of the physical channle config database of the given index

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsdlphch_cfg_db_init(uint8 index);

/*===========================================================================
FUNCTION tdsdlphch_copy_ts_info

DESCRIPTION
  Allocates one location from pool of tdsdlphch_cfg_db_tmp if it is available

DEPENDENCIES
  None

RETURN VALUE
  handle to access the tdsdlphch_cfg_db_tmp
  
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsdlphch_copy_ts_info(
  /* to which cfg db location the information should be copied */
  mcaltds_dl_ts_cfg_parms_struct_type *ts_cfg_ptr, 
  
  /* tfci code bits */
  uint8 tfci_coding,
  
  /* num of ts */
  uint8 num_ts,
  
  /* physical channel configuration information from RRC */
  tdsl1_dl_phycfg_individual_ts_info_struct_type *individual_ts_ptr );

/*===========================================================================
FUNCTION        tdsdlphch_calc_slot_format

FILE NAME       dl_phch.c

DESCRIPTION     This function computes the slot format index as needed by
                the mDSP interface for setting up the SCCPCH, DPCH, and
                PDSCH physical channels.

DEPENDENCIES    None

RETURN VALUE    The slot format index based on the channel type, numboer
                of or existence of pilot bits, tfci existence, and the
                spreading factor.

SIDE EFFECTS    None.
===========================================================================*/
uint8 tdsdlphch_calc_slot_format(
  /* tfci code bits */
  tdsl1_sf_enum_type sf,

  uint8 tfci_bits,

  tdsl1_ss_tpc_symb_enum_type tpc_ss_symbols );
/*===========================================================================
FUNCTION        tdsdlphch_calc_tpc_ss_bits

DESCRIPTION     This function computes the slot format for the SCCPCH as
                needed by the mDSP interface.

DEPENDENCIES    None

RETURN VALUE    The slot format index

SIDE EFFECTS    None
===========================================================================*/
uint8 tdsdlphch_calc_tpc_ss_bits( 
  tdsl1_sf_enum_type sf, 
  tdsl1_ss_tpc_symb_enum_type tpc_ss_symbols );
/*===========================================================================
FUNCTION tdsdlphch_idleslot_configure_for_measurement

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsdlphch_configure_idleslot_for_measurement(
  tdsdlphch_cfg_handle_type phch_cfg_db_idx, 
  boolean flag);

/*===========================================================================
FUNCTION tdsdlphch_configure_sccpch_idleslot_for_measurement

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsdlphch_configure_sccpch_idleslot_for_measurement(
  tdsdlphch_dl_sccpch_cfg_parms_struct_type *sccpch_cfg_ptr, 
  boolean flag);
/*===========================================================================
FUNCTION tdsdlphch_configure_dpch_idleslot_for_measurement

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsdlphch_configure_dpch_idleslot_for_measurement(
  tdsdlphch_dl_dpch_cfg_parms_struct_type *dpch_cfg_ptr, 
  boolean flag,
  uint16 actionTime);
/*===========================================================================
FUNCTION tdsdlphch_configure_sccpch_idleslot_for_measurement

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsdlphch_configure_pccpch_idleslot_for_measurement(
  tdsdlphch_dl_pccpch_cfg_parms_struct_type *pccpch_cfg_ptr,
  boolean flag);
/*===========================================================================
FUNCTION tdsdlphch_configure_pich_idleslot_for_measurement

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsdlphch_configure_pich_idleslot_for_measurement(
  tdsdlphch_dl_pich_cfg_parms_struct_type *pich_cfg_ptr,
  boolean flag);
/*===========================================================================
FUNCTION tdsdlphch_update_db_info

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsdlphch_update_db_info(
  tdsdlphch_cfg_handle_type phch_cfg_db_idx,
  tdsdlphch_update_parms_struct_type *phch_parms_ptr);

/*===========================================================================
FUNCTION tdsdlphch_get_cctrch_idx

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
uint8 tdsdlphch_get_cctrch_idx(
       tdsdlphch_cfg_handle_type phch_cfg_db_idx);

/*-TDSDLPHCH Development-----------------------------------------------------------------------*/

#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
/*===========================================================================
FUNCTION        tdsdlphch_submit_dpch_cfg_logpkt

FILE NAME       

DESCRIPTION     

DEPENDENCIES                

RETURN VALUE   

SIDE EFFECTS   
 
===========================================================================*/
LOCAL void tdsdlphch_submit_dpch_cfg_logpkt( uint8 action_type );

/*===========================================================================
FUNCTION        mcaltds_dl_submit_common_channel_cfg_logpkt

FILE NAME       

DESCRIPTION     

DEPENDENCIES                

RETURN VALUE   

SIDE EFFECTS   
 
===========================================================================*/
LOCAL void tdsdlphch_submit_common_phychan_cfg_logpkt( 
           mcaltds_dl_phch_cfg_parms_struct_type *phch_cfg_parms_ptr );

#endif
#endif /* DLPHCHI_H */

