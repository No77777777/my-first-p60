#ifndef DS_3GPP_APN_SWITCH_MGR_H
#define DS_3GPP_APN_SWITCH_MGR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
         
                       D A T A   S E R V I C E S 
                        DS EPS APN SWITCH MGR HEADER FILE

GENERAL DESCRIPTION

  All the declarations and definitions necessary to support APN Switch MGR file

 Copyright (c) 2015-2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_apn_switch_mgr.h#3 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/27/15    pvb     Created Module 

===========================================================================*/

/*===========================================================================
                              INCLUDE FILES
===========================================================================*/
#include "ds_dsd_apm_ext_i.h"
#include "ds_3gpp_pdn_context.h"
#include "dsumts_rmsmi.h"
#include "queue.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
typedef struct
{
  boolean                          payload_valid;
  ds_dsd_apm_ind_type              apm_ind;
  ds_dsd_apm_ind_pay_load          apm_ind_payload;
}ds_3gpp_apn_switch_mgr_payload_info_type;

/*---------------------------------------------------------------------------
APN: 
CID1 ->Common APN supporting only IPv4 for internet connecting stored
CID9 -> APN supporting only IPv4 for attach procedure stored
CID10 -> APN supporting only IPv4 for FOTA connecting stored
Other CID ->Null
ID,Pass: 
For CID1 ->Null
For CID9 -> Common ID,PASS for internet connecting stored
For CID10 -> NullFor other CID -> Null
---------------------------------------------------------------------------*/
#define APN_SWITCH_CID1_PROFILE_ID 1       /*the number is only for debug currently*/
#define APN_SWITCH_CID9_PROFILE_ID 4       /*the number is only for debug currently*/
#define APN_SWITCH_INVALID_PROFILE_ID 0  /*the number is only for debug currently*/


/*---------------------------------------------------------------------------
  Enumeration of states of the RmSm IP Common State Machine.
---------------------------------------------------------------------------*/
typedef enum
{
  /*Initial State*/
  DS_3GPP_APN_SWITCH_NULL_STATE = 0, 
  DS_3GPP_APN_SWITCH_DEACTIVATE_IN_PROGRESS_STATE,
  DS_3GPP_APN_SWITCH_ACTIVATE_IN_PROGRESS_STATE,
  DS_3GPP_APN_SWITCH_ACTIVATE_SUCCESS_STATE
}ds_3gpp_apn_switch_hdlr_state_type;

/*--------------------------------------------------------------------------- 
  Function ptr that needs to be called to start the dial up procedure
  cb data   ----> cb data passed by the client
  cb_result ----> TRUE/FALSE indication if apn switch was successful or not.
  ---------------------------------------------------------------------------*/
typedef void (*ds_3gpp_apn_switch_hdlr_cb_fn_ptr)
(
  void     *cb_data,
  boolean   cb_result
);

/*--------------------------------------------------------------------- 
  Structure when to handle all apn switch hdlr data like
  timers, cb function ptrs
  --------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_apn_switch_hdlr_cb_fn_ptr     cb_fn_ptr; /*sent to  the caller*/
  void                                                 *cb_data;
  rex_timer_type                                  guard_timer;
  boolean                                            cb_pending;
  ds_3gpp_apn_switch_hdlr_state_type   state;
  sys_modem_as_id_e_type                   subs_id;
  void                                                 *user_data;
  uint16                                          user_profile_id;
  void                                           *user_apn;
  boolean                                            call_disconnected;
}ds_3gpp_apn_switch_hdlr_data_type;

#ifdef FEATURE_DATA_LTE
/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_CHECK_IF_APN_SWITCH_NEEDED

DESCRIPTION    This function extracts APM rule block entry for a given 
               profile ID and compares the cause code with APN Switching
               cause codes to decide APN Switch is needed or not
 
PARAMETERS     subs_id: Currently active subscription for Packet services 
               profile_id: Profile of the PDN that is brought down
               cause_type: Detach/Reject Network cause
               call_info_ptr: call info passed by lower layers
               apn_switch_is_needed_p: Pointer to be filled with value that
                                       tells whether APN Switch is needed or 
                                       not
 
DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_check_if_apn_switch_needed
(
  cm_call_mode_info_u_type         *call_info_ptr,
  boolean                          *apn_switch_is_needed_p,
  sys_modem_as_id_e_type            subs_id,
  uint16                            profile_id,
  sys_sys_mode_e_type               call_mode,
  dsd_apm_rule_param_name_enum_type cause_type
);

/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_GET_DETACH_SWITCHING_ACTION_IF_ANY

DESCRIPTION    This function gets the flag which tells whether APN switch 
               is needed for the detach 
 
PARAMETERS     subs_id: Currently active subscription for Packet services 
 
DEPENDENCIES   NONE

RETURN VALUE   switching_action_payload: Switching action related info per subs

SIDE EFFECTS   NONE
===========================================================================*/
ds_3gpp_apn_switch_mgr_payload_info_type * 
                        ds_3gpp_apn_switch_mgr_get_switching_action_payload
(
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_RESET_SWITCHING_ACTION_PAYLOAD

DESCRIPTION    This function resets the switching action payload per subs
 
PARAMETERS     subs_id: Currently active subscription for Packet services 
 
DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_reset_switching_action_payload
(
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_SEND_IND_TO_APM

DESCRIPTION    This function retrieves the switching action payload per 
               subs and indicates it to APM 
 
PARAMETERS     subs_id: Currently active subscription for Packet services 
 
DEPENDENCIES   NONE

RETURN VALUE   ds_dsd_ext_error_code: Error code after posting ind to APM

SIDE EFFECTS   NONE
===========================================================================*/
ds_dsd_ext_error_code ds_3gpp_apn_switch_mgr_send_ind_to_apm
(
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_POPULATE_SWITCHING_PAYLOAD_PER_RAT

DESCRIPTION    This function extracts APM rule block entry for a given 
               profile ID and compares the cause code with APN Switching
               cause codes to decide APN Switch is needed or not
 
PARAMETERS     bearer_call_mode: Call mode of the bearer 
               cause_type: DETACH/REJECT NETWORK cause type
               profile_id: Profile of the PDN detached or rejected
               PDN State: State of PDN Context
               call_info_ptr: Call info passed by lower layers
               subs_id: Current active packet service subscription
 
DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_populate_switching_payload_per_rat
(
  uint64                               bearer_call_mode,
  uint16                               cause_type,
  uint16                               profile_id,
  ds_pdn_context_state_e               pdn_state,
  cm_call_mode_info_u_type            *call_info_ptr,
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_WRITE_ATTACH_PROF_INFO_TO_EFS

DESCRIPTION    This function calls APM API to write attach profile/rule 
               info from cache to EFS 
 
PARAMETERS     subs_id: Current active PS subs
 
DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_write_attach_prof_info_to_efs
(
  ds_dsd_apm_detach_info_per_subs_type   *detach_info_ptr
);


#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_INIT

DESCRIPTION
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_apn_switch_hdlr_init
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_PROCESS_SYS_EVENT

DESCRIPTION
  This function process the DSD pref sys info and take corresponding action
 
PARAMETERS
  cmd_payload_ptr:                    pref sys info reported by DSD

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_apn_switch_hdlr_process_sys_event
(
  void  *cmd_payload_ptr
);

/*===========================================================================
FUNCTION       ds_3gpp_apn_switch_hdlr_proc_start

DESCRIPTION    This function start DUN call processing on LTE and W/G/T mode

PARAMETERS     

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_3gpp_apn_switch_hdlr_proc_start
(
  uint16                                           profile_num,
  ds_3gpp_apn_switch_hdlr_cb_fn_ptr  apn_switch_cb_fn_ptr,
  void                                              *cb_data,  
  char                                              passwd[],
  boolean                                         *cb_ev_active,
  void                                              *info_ptr,     
  sys_modem_as_id_e_type                subs_id
);


/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_HDLR_DUN_CALL_DISCONNECT

DESCRIPTION    This function will switch CID9 back to CID1 for LTE in dun call disconnect

PARAMETERS     NONE

DEPENDENCIES   NONE

RETURN VALUE   

SIDE EFFECTS   NONE
===========================================================================*/

boolean ds_3gpp_apn_switch_hdlr_dun_call_disconnect
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_ENABLED

DESCRIPTION
  If apn switch hdlr NV is set to TRUE, return TRUE
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_apn_switch_hdlr_enabled
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_GET_USER_DATA

DESCRIPTION
   Get user profile id from apn switch hdlr
 
PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void *ds_3gpp_apn_switch_hdlr_get_user_data
(
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_GET_USER_APN

DESCRIPTION
   Get user apn from apn switch hdlr
 
PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void *ds_3gpp_apn_switch_hdlr_get_user_apn
(
  sys_modem_as_id_e_type         subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_GET_USER_PROFILE_ID

DESCRIPTION
   Get user profile id from apn switch hdlr
 
PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
uint16 ds_3gpp_apn_switch_hdlr_get_user_profile_id
(
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_DISPATCH_CB
 
DESCRIPTION
  This function Clear the guard timer and dispatch the cb fn only if cb is pending
  
PARAMETERS   : 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_apn_switch_hdlr_dispatch_cb
(
  boolean cb_val
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWICH_HDLR_IS_SYS_MODE_LTE
 
DESCRIPTION
  This function is used to judge if the current network mode is LTE
  
PARAMETERS   : 
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: current network mode is LTE
  FALSE: current network mode is WCDMA or TDSCDMA or GSM.
 
SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_apn_switch_hdlr_is_sys_mode_lte
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_PERFORM_DETACH_OR_DISCONNECT_ACTION

DESCRIPTION    Called as part of dsd_apm_set_attach_pdn_list to perform 
               corresponding Action.

DEPENDENCIES   This function is only processed if an action is pending to 
               be taken (ds_dsd_apm_is_detach_or_disconnect_pdn_action_cnf_pending)
               else we bail out.
 
               DS_DSD_APM_ACTION_DISCONNECT_ATTACH_PDN_ONLY can be done in any
               3GPP RAT , if the PDN is UP
     
               DS_DSD_APM_ACTION_PERFORM_DETACH , can only be performed if
               we are on LTE
 
PARAMETERS     subs_id : Subscription ID

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_perform_detach_or_disconnect_action
(
  sys_modem_as_id_e_type      subs_id
);
/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_PERFORM_CLEANUP_ON_DETACH_RETRY_ERR

DESCRIPTION    Performs cleanup if CM rejects LTE DETACH requested as 
               part of attach action due to an ongoing voice call.
               This clears the AP cache and notifies AP of CONF FAILURE

DEPENDENCIES   This function is only processed if an action is pending to 
               be taken (ds_dsd_apm_is_detach_or_disconnect_pdn_action_cnf_pending)
               else we bail out.
 
PARAMETERS     client_data_ptr (which is the subs_id in this case)

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_perform_cleanup_on_detach_retry_err
(
  void                           *client_data_ptr
);
/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_DEL_TMP_ATTACH_PROFILE_AND_CACHE

DESCRIPTION    Deletes the temp atatch profile file if created adn the 
               temp attach prof cache 

DEPENDENCIES   NONE 
 
PARAMETES      subs_id : Subscription id 

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_del_tmp_attach_profile_and_cache
(
  sys_modem_as_id_e_type      subs_id
);
/*===========================================================================
FUNCTION       DS_3GPP_APN_SWITCH_MGR_DETACH_IF_ATTACH_LIST_MODIFIED_BY_AP

DESCRIPTION    Triggeres PS detach if AP has modified the attach profile list 
               by sending PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST while the
               UE is in the middle of attach process.
               This internally calls ds_eps_pdn_cntxt_send_detach_req to do the
               detach.

DEPENDENCIES   NONE 
 
PARAMS         subs_id : Subscription id for which the action needs to be taken 
 

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_apn_switch_mgr_detach_if_attach_list_modified_by_ap
(
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_GET_CB_DATA

DESCRIPTION
   When this api is called the client's cb data is returned to the caller
   For EX : For DUN calls sm_ptr is stored is returned
 
   The callback fn ptr should match only then the call back data is given
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void *ds_3gpp_apn_switch_hdlr_get_cb_data
(
  ds_3gpp_apn_switch_hdlr_cb_fn_ptr  cb_fn_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_ALLOWED

DESCRIPTION
  Function to check if the APN switch hdlr feature is allowed in DUN
  call or in other words if the APN switch to the new dialled string
  profile would happen. Currently it considers the profile ID, APN & 
  pdp type parameter and returns a boolean value accordingly.
  
PARAMETERS
  subscription id
  profile_num
  rmsmi_info_ptr
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: if dial string profile is different from attached profile.
  FALSE: otherwise.

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_apn_switch_hdlr_allowed
(
  sys_modem_as_id_e_type subs_id,
  uint16                 profile_num,
  dsumts_rmsmi_info_type *rmsmi_info_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_APN_SWITCH_HDLR_DETACH_OR_DISCONNECT

DESCRIPTION

  This function is called when PS receives 
  PS_SYS_IOCTL_3GPP_SET_LTE_ATTACH_PDN_LIST.

  In order to avoid race condition with 
  ds_eps_pdn_cntx_attach_complete_ind_hdlr.

PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_apn_switch_hdlr_detach_or_disconnect
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_EXTRACT_APN_FROM_USERID

DESCRIPTION
  Function to extract apn info from auth userid parameter. The userid
  format is expected to be in the form username@apn and component after
  the '@' needs to be extracted and stored in the apn info.
  
PARAMETERS
  rmsmi_info_ptr
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_extract_apn_from_userid
(
  ppp_auth_info_type  *auth_info_ptr,
  byte                *apn
);
#endif /* DS_3GPP_APN_SWITCH_MGR_H*/
