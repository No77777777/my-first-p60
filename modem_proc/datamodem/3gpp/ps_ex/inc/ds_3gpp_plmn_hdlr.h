#ifndef DS_3GPP_PLMN_HDLR_H
#define DS_3GPP_PLMN_HDLR_H

/*!
  @file
  ds_3gpp_plmn_hdlr.h

  @brief
  PLMN related functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009 - 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_plmn_hdlr.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/14   vm      Initial version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#include "sys_v.h"
#include "reg_sim.h"
#include "sys_eplmn_list.h"
#include "cm.h"
#include "ds_3gpp_srv_req_throt_sm.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif
/*===========================================================================

                         EXTERNAL TYPE DECLRATIONS

===========================================================================*/

#define DS_3GPP_SYS_SEL_RETRY_MAX_VAL 16
/*------------------------------------------------------ 
  Max retrial value, after which AP has to be notified 
  a action_conf_failure if it is awaiting a response
  -------------------------------------------------------*/
#define DS_3GPP_SYS_SEL_RETRY_MAX_RETRIAL_VAL 60

typedef enum
{
  DS_3GPP_NO_SYS_SEL_RETRY,
  DS_3GPP_LTE_ENABLE_RETRY,
  DS_3GPP_LTE_DISABLE_RETRY,
  DS_3GPP_LTE_PLMN_BLOCK_RETRY,
  DS_3GPP_LTE_PLMN_UNBLOCK_RETRY,
  DS_3GPP_LTE_DETACH_ONLY
}ds_3gpp_plmn_sys_sel_retry_e_type;

/*-------------------------------------------------------------------------- 
  This structure is used to determine the retry timer and at what intervals
  should the retry occur
  --------------------------------------------------------------------------*/
typedef struct
{
  uint16                                 retry_timer_val;
  uint32                                 total_retry_time_elapsed;
  rex_timer_type                         sys_sel_retry_timer; 
  cm_plmn_blocking_info_s_type           plmn_blocking_info;
  cm_sys_sel_pref_params_s_type          sys_sel_pref_params;
  ds_3gpp_plmn_sys_sel_retry_e_type      retry_type;
  ds_3gpp_srv_req_throt_event_type       svc_event_type;
}ds_3gpp_plmn_sys_sel_retry_sm_s;


extern ds_3gpp_plmn_sys_sel_retry_sm_s ds_3gpp_plmn_sys_sel_retry_sm[SYS_MODEM_AS_ID_MAX];

/*===========================================================================

                      EXTERNAL FUNCTION DECLRATIONS

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_plmn_qsh_mdump_collect
  
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
void ds_3gpp_plmn_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION      DS_3GPP_GET_EPLMN_LIST_PER_SUB

DESCRIPTION   This function returns the EPLMN List per subs id.
              Note: memory has to be allocated by user.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_get_eplmn_list_per_sub
(
  sys_modem_as_id_e_type  subs_id,
  sys_plmn_list_s_type   *eplmn_list_p,
  sys_plmn_id_s_type     *rplmn_p
);

/*===========================================================================
FUNCTION      DS_3GPP_GET_EHPLMN_LIST_PER_SUB

DESCRIPTION   This function returns the EHPLMN List per subs id.
              Note: memory has to be allocated by user.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_get_ehplmn_list_per_sub
(
  sys_modem_as_id_e_type     subs_id,
  reg_sim_plmn_list_s_type  *ehplmn_list_p
);

/*===========================================================================
FUNCTION      DS_3GPP_IS_PLMN_IN_EHPLMN_LIST

DESCRIPTION   This function checks whether the given PLMN is in EHPLMN List

DEPENDENCIES  None

RETURN VALUE  TRUE, if PLMN is in EHPLMN List
              FALSE, otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_is_plmn_in_ehplmn_list
(
  sys_plmn_id_s_type      plmn_id,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_GET_HPLMN_ID_PER_SUB

DESCRIPTION   This function returns the HPLMN ID per subs id.
              Note: memory has to be allocated by user.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_get_hplmn_id_per_sub
(
  sys_modem_as_id_e_type   subs_id,
  sys_plmn_id_s_type      *hplmn_id_p
);

/*===========================================================================
FUNCTION DS_3GPP_PLMN_SYS_SEL_PREF_NO_ERR_CMD_HDLR
 
DESCRIPTION
  This function handles when sys selection enable/disable has resulted in
  an no error, This function clears all the retry/timer values
  
PARAMETERS   : 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_plmn_sys_sel_pref_no_err_cmd_hdlr
(
  void              *client_data_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_PLMN_SYS_SEL_PREF_ERR_CMD_HDLR
 
DESCRIPTION
  This function handles when sys selection enable/disable has resulted in
  an error, It establishes a retry mechanism to enable/disable LTE
  
PARAMETERS   : 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_plmn_sys_sel_pref_err_cmd_hdlr
(
  void              *client_data_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_RETRY_TIMER_EXPIRY_CMD_HDLR

DESCRIPTION
  This function would retry to enable/disable sys selection

PARAMETERS
  subs_id      : The subsciprtion ID
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_plmn_sys_sel_retry_timer_expiry_cmd_hdlr
(
   sys_modem_as_id_e_type          subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_CLR_RETRY_SM

DESCRIPTION
  This function clears sys_sel_retry_sm 

PARAMETERS
   void            
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_plmn_sys_sel_retry_timer_expiry
(
 unsigned long callback_data  
);

/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_CLR_RETRY_SM

DESCRIPTION
  This function clears sys_sel_retry_sm 

PARAMETERS
   void            
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_plmn_sys_sel_clr_retry_sm
(
  sys_modem_as_id_e_type          subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_SET_RETRY_TYPE

DESCRIPTION
  This function sets the retry type

PARAMETERS
  ds_3gpp_throt_sys_sel_retry_e_type     retry_type           
  sys_modem_as_id_e_type                subs_id
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_plmn_sys_sel_set_retry_type
(
  ds_3gpp_plmn_sys_sel_retry_e_type     retry_type,
  sys_modem_as_id_e_type                subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_GET_RETRY_TYPE

DESCRIPTION
  This function gets the retry type for particular sub

PARAMETERS
  ds_3gpp_throt_sys_sel_retry_e_type     retry_type
  sys_modem_as_id_e_type                subs_id
 
DEPENDENCIES
  None.

RETURN VALUE 
  retry_type 

SIDE EFFECTS
  None.

===========================================================================*/
ds_3gpp_plmn_sys_sel_retry_e_type ds_3gpp_plmn_sys_sel_get_retry_type
(
  sys_modem_as_id_e_type                subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_SET_BLK_INFO

DESCRIPTION
  This function sets the PLMN BLK INFO, This is needed to 
  recalculate the retry time when a disable fails.

PARAMETERS 
  PLMN blocking info BLOB 
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_plmn_sys_sel_set_blk_info
(
   cm_plmn_blocking_info_s_type *plmn_blocking_info_p
);

/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_SET_SYS_SEL_PREF_PARAMS

DESCRIPTION
  This function sets the sys_sel_pref_params, This is needed to 
  send out the LTE disable retry request

PARAMETERS 
  sys_sel_pref_params_p 
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_plmn_sys_sel_set_sys_sel_pref_params
(
   cm_sys_sel_pref_params_s_type *sys_sel_pref_params_p,
   sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PLMN_SYS_SEL_SET_blk_int
 
DESCRIPTION
  This function sets the blocking interval, Only used for test cases

PARAMETERS
  blocking interval time           
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_plmn_sys_sel_set_blk_int
(
   uint16                  blocking_interval,
   sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PLMN_PREP_FOR_LTE_BLOCKING_AND_LOG

DESCRIPTION
  This function prepares all the necessary variables, throttle retry sm's
  before calling ds3gmshif function to actually block/disable LTE
 
PARAMETERS  
  sys_plmn_id_s_type      plmn_id,
  uint32                  timer_val,
  sys_modem_as_id_e_type  subs_id,
  uint8                   failure_count
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE                    If the function succeeds
  FALSE                   Otherwise
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_plmn_prep_for_lte_blocking_and_log
(
  sys_plmn_id_s_type             plmn_id,
  uint32                         timer_val,
  sys_modem_as_id_e_type         subs_id,
  uint8                          failure_count,
  boolean                        detach,
  sys_block_plmn_cause_e_type    cause
);

void ds_3gpp_plmn_init
(
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_SRV_REQ_THROT_SET_EV_INFO

DESCRIPTION
  This function sets the svc throttle info necessary for logging 

PARAMETERS 
  
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_plmn_set_sys_sel_ev_info
(
  void *svc_throt_ev_info
);

#endif /* DS_3GPP_PLMN_HDLR_H */
