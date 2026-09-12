/*!
  @file
  ds_3gpp_cc_sp_throt_sm.h

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_cc_sp_throt_sm.h#3 $

when         who     what, where, why
--------     ---     ------------------------------------------------------
1/07/16      vb      Handle cc_sp throttling 
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/


#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#include "ds_3gpp_pdn_context.h"
#include "dsutil.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================

                           MACRO DEFINITIONS

===========================================================================*/
#define DS_3GPP_CC_SP_THROT_MAX_FAIL_COUNT 3
#define DS_3GPP_CC_SP_THROT_2_HOURS_TIMER  7200*1000  /*in milliseconds*/  
#define DS_3GPP_CC_SP_THROT_TIMER          10
#define DS_3GPP_CC_SP_THROT_ATTACH_TIMER   45*1000

typedef enum
{
  DS_3GPP_CC_SP_MIN_TIMER = 0,
  DS_3GPP_CC_SP_PDN_THROT_2_HOURS_TIMER = 1,
  DS_3GPP_CC_SP_APN_THROT_TIMER = 2,
  DS_3GPP_CC_SP_ATTACH_DEF_TIMER = 3,
  DS_3GPP_CC_SP_PDN_THROT_UMTS_2_HOURS_TIMER,
  DS_3GPP_CC_SP_TIMER_UNKNOWN
}ds_3gpp_cc_sp_exp_tmr_cb_data_enum_type;

#ifdef FEATURE_DATA_LTE 

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_throt_sm_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_3gpp_throt_sm_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION DS_3GPP_CC_SP_INIT

DESCRIPTION
  This function does the powerup initialization for the CC SP module.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_cc_sp_init
(
   void
);/*ds_3gpp_cc_sp_init*/

/*===========================================================================
FUNCTION DS_3GPP_THROT_ENABLE_CC_SP_THROTTLING

DESCRIPTION
  This function enables cc_sp_throttling

PARAMETERS  
  Subs Id
  Flag - TRUE / FALSE
 
DEPENDENCIES 
  None.
  
RETURN VALUE 
boolean
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_throt_sm_set_cc_sp_throttling
(
  sys_modem_as_id_e_type  subs_id,
  boolean                 value
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_IS_CC_SP_THROTTLING_ENABLED

DESCRIPTION
  This function returns the NV item value of the cc_sp_throttling

PARAMETERS  
None
DEPENDENCIES 
  None.
  
RETURN VALUE 
boolean
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_throt_sm_is_cc_sp_throttling_enabled
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_CC_SP_INFO_TBL_PER_SUBS_INIT

DESCRIPTION
  This function initializes throttle info tbl to get timer value per sub

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_cc_sp_info_tbl_per_subs_init
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_CC_SP_RULE_ALLOW_SPEC_THROTTLING

DESCRIPTION
  This function validates if spec defined throttling is needed or not for
  cc_sp depending on whether UE is in HOME or ROAM

PARAMETERS  
None
DEPENDENCIES 
  None.
  
RETURN VALUE 
 FALSE = If UE is in HOME and cc_sp throt NV is enabled, follow cc_sp throttling
         (i.e ignore spec throttling)
 TRUE = if UE in ROAM/cc_sp NV disabled, follow spec throttling
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_cc_sp_rule_allow_spec_throttling
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLR_CC_SP_T3402_TIMER

DESCRIPTION
  This function is used to clear T3402 timer running for CC SP
  

PARAMETERS: 
  subs_id.
 
DEPENDENCIES
  None.

RETURN VALUE
 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_cc_sp_clr_timer
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_CLEAR_CC_SP_THROT_STATE_FOR_APN_ON_CURRENT_PLMN

DESCRIPTION
  This function is used to clear CC SP throttling of the matching APN
  on the current PLMN
  

PARAMETERS: 
  subs_id.
 
DEPENDENCIES
  None.

RETURN VALUE
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_clear_cc_sp_throt_state_for_apn_on_current_plmn
(
  byte                   *apn, 
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SET_CCSP_PLMN_UNBLOCK_TIMER

DESCRIPTION
  This function is used to set 2 HR PLMN blocking timer.
  This is called in UMTS when a PLMN is blocked on LTE and on UMTS
  infinite throttling occurs.

PARAMETERS: 
  subs_id.
 
DEPENDENCIES
  None.

RETURN VALUE
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_cc_sp_throt_set_plmn_unblock_timer
(
  sys_modem_as_id_e_type subs_id,
  uint32                 throt_tmr_val
);

/*===========================================================================
FUNCTION DS_EPS_PDN_THROT_HANDLE_PDN_CONN_REJ
 
DESCRIPTION
  This function gets called when attach is rejected and given cause code is present in EFS configuration file
  
PARAMETERS   : 
  1. rejected_pdn_cntx_p 
  2. Subs Id
  
DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_cc_sp_throt_handle_pdn_conn_rej
( 
  ds_pdn_context_s                *rejected_pdn_cntx_p, 
  esm_cause_enum_T                 esm_cause,
  ps_iface_net_down_reason_type    down_reason,
  sys_modem_as_id_e_type           subs_id
);


/*===========================================================================
FUNCTION  DS_EPS_PDN_THROT_T3402_TIMER_EXP_CB

DESCRIPTION
  This function posts a cmd to take specific action based on tr3402 timer expiry

PARAMETERS
   call back data ptr - which points to t3402 timer table            
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_pdn_throt_t3402_timer_exp_cb
(
     unsigned long callback_data 
);

/*===========================================================================
FUNCTION  DS_EPS_PDN_THROT_T3402_TIMER_EXPIRY_CMD_HDLR

DESCRIPTION
  This function resets the sm on timer expiry

PARAMETERS
   void            
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_cc_sp_throt_t3402_timer_expiry_cmd_hdlr
(
  unsigned long client_data_ptr
);

/*============================================================================
FUNCTION      DS_3GPP_CC_SP_RESET_THROTTLING

DESCRIPTION
  This function takes care of clearing CC_SP throttling information as
  part of timer expiry/RAU/TAU update. It also takes care of informing
  external clients with the ublocked APNs
  
PARAMETERS  
  callback_data - points to the  throttling state machine.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None
  
===========================================================================*/
void ds_3gpp_cc_sp_reset_throttling
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_IS_ANY_APN_THROTTLED_FOR_MAX_FAILURE

DESCRIPTION
  This function is used to check of any APN is throttled for max count 
  

PARAMETERS: 
  subs_id.
 
DEPENDENCIES
  None.

RETURN VALUE
  true - if any APN is throttled and fail count reached maximum
   flase - otherwise.
 
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_cc_sp_throt_is_any_apn_throttled_for_max_failure
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_CC_SP_IS_PLMN_UNBLOCK_TIMER_RUNNING

DESCRIPTION
  This function is used to check if plmn unblock timer is running

PARAMETERS: 
  subs_id.
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_cc_sp_is_plmn_unblock_timer_running
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_CC_SP_THROT_IS_ATTACH_DEF_THROTTLED

DESCRIPTION
  This function is used to check of attach APN is throttled for max count or
  not for cc_sp_throttle

PARAMETERS: 
  subs_id.
 
DEPENDENCIES
  None.

RETURN VALUE
  true - if attach APN is throttled and fail count has not reached MAX
  false - otherwise.
 
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_cc_sp_throt_is_attach_apn_def_throttled
(
  sys_modem_as_id_e_type subs_id
);
#endif/*FEATURE_LTE*/
