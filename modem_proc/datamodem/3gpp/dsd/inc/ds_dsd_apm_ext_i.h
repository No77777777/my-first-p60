#ifndef DS_DSD_APM_EXT_I_H
#define DS_DSD_APM_EXT_I_H
/*===========================================================================

                      DS_DSD_APM_EXT_I_H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2012-2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/inc/ds_dsd_apm_ext_i.h#3 $
  $Author: pwbldsvc $ $DateTime: 2019/12/19 04:39:41 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/18/13   fjia   Add iWLAN mobility support
05/08/12    sk    Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"

#include "ds_dsd_ext_i.h"
#include "ps_iface_defs.h"
#include "ds_profile_3gpp.h"
#include "sys.h"
#include "ps_sys_ioctl.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================
                          COMMON TO REQUEST AND INDICATION
===========================================================================*/
typedef enum
{
  DS_DSD_APM_SUCCESS,                /* Success operation                      */
  DS_DSD_APM_INVALID_APM,            /* APM is not valid                       */
  DS_DSD_APM_NO_PAY_LOAD,            /* No payload defined for the event       */
  DS_DSD_APM_NO_MEMORY_SUPPLIED,     /* no memory allocated to writeback       */
  DS_DSD_APM_INVALID_PROF_ID,        /* Client provided profile not valid      */
  DS_DSD_APM_INVALID_RULE_TYPE,      /* Client provided rule type is not valid */
  DS_DSD_APM_INVALID_ATTACH_TYPE,    /* Client provided attach type not valid  */
  DS_DSD_APM_FIELD_MASK_NOT_SET,     /* Client does not set field mask         */
  DS_DSD_APM_ALL_ATTACH_APN_BLOCKED, /* All attach APN requested is blocked    */
  DS_DSD_APM_ATTACH_NOT_ALLOWED,     /* Current LTE attach is not allowed      */
  DS_DSD_APM_UNSEPCIFIED             /* Unspecified                            */
}ds_dsd_ext_error_code;

/*-------------------------------------------------------------------------- 
  Enum to represent various action items
  --------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_APM_ACTION_PERFORM_DETACH = 0,
  DS_DSD_APM_ACTION_DISCONNECT_ATTACH_PDN_ONLY,
  DS_DSD_APM_ACTION_MAX /*This should be the last enum always*/
} ds_dsd_apm_new_attach_list_action_e_type;

/*-------------------------------------------------------------------------- 
  Enum to represent the conf type to AP
  --------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_APM_AP_ACTION_CONF_FAILURE = 0,
  DS_DSD_APM_AP_ACTION_CONF_SUCCESS
} ds_dsd_apm_ap_action_conf_e_type;

/*---------------------------------------------------------------------------
   Field MASK to indicate the available field  in payload
---------------------------------------------------------------------------*/
#define DS_DSD_APM_IP_V4_FIELD_MASK  0x01
#define DS_DSD_APM_IP_V6_FIELD_MASK  0x02

#define ATTACH_APN_MAX  2

typedef struct
{
  /*-------------------------------------------------------------------------
    Field mask indicates the validity of the following fields.
  -------------------------------------------------------------------------*/
  uint8     field_mask;

  /*-------------------------------------------------------------------------
    TRUE - if IP address available
    FALSE - if address not available
  -------------------------------------------------------------------------*/
  boolean   v4_address;
  boolean   v6_address;

  /*-------------------------------------------------------------------------
    Indicate reason of address failure
  -------------------------------------------------------------------------*/
  ps_iface_net_down_reason_type v4_reason;
  ps_iface_net_down_reason_type v6_reason;
}ds_dsd_apm_ip_type;

/*===========================================================================
                          INDICATION
===========================================================================*/

/*-------------------------------------------------------------------------
Pay load field mask to indicate the available field in payload
--------------------------------------------------------------------------*/
#define DS_DSD_IND_PROFILE_ID_FIELD_MASK            0x0001
#define DS_DSD_IND_PCSCF_FIELD_MASK                 0x0002
#define DS_DSD_IND_CAUSE_CODE_FIELD_MASK            0x0004
#define DS_DSD_IND_IP_ADDRESS_FIELD_MASK            0x0008
#define DS_DSD_IND_PLMN_FIELD_MASK                  0x0010
#define DS_DSD_IND_SYS_FIELD_MASK                   0x0020
#define DS_DSD_IND_ATTACH_TYPE_MASK                 0x0040
#define DS_DSD_IND_APN_SWITCH_MASK                  0x0080
#define DS_DSD_IND_DNS_ADDR_FIELD_MASK              0x0100
#define DS_DSD_IND_CNTXT_TRANSFER_PROGRESS_MASK     0x0200

/*---------------------------------------------------------------------------
  The INDICATIONS that the client is passing.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_APM_ATTACH_INIT_IND,      /* UE start LTE attach procedure            */
  DS_DSD_APM_ATTACH_COMPLETE_IND,  /* NW attach completes and L2 ACK received  */
  DS_DSD_APM_ATTACH_REJECT_IND,    /* NW reject attach request                 */
  DS_DSD_APM_DETACH_IND,           /* N/W detaches                             */
  DS_DSD_APM_IP_ADDRESS_IND,       /* IP address field changes                 */
  DS_DSD_APM_PLMN_CHANGED_IND,     /* how to know if there is true PLMN change */
  DS_DSD_APM_SYS_CHANGED_IND,      /* If the UE moves to system other than LTE */
  DS_DSD_APM_RPM_INIT_IND,         /* RPM supported SIM is inserted            */
  DS_DSD_APM_T3402_CHG_IND,        /* T3402 Timer val has changed              */
  DS_DSD_APM_RESET_ATTACH_APN_IND, /* Reset Attach APN to Class 1              */
  DS_DSD_APM_TLB_MODE_ACT_IND,     /* TLB Mode A Activate Ind                  */
  DS_DSD_APM_ATTACH_PDP_TYPE_IND,  /* PDP type used for attach Ind             */
  DS_DSD_APM_TRANSFER_IND,         /* UE is in transfer progress to LTE        */ 
  DS_DSD_APM_MAX_IND               /* Max value of the ENUM                    */
}ds_dsd_apm_ind_type;

/*---------------------------------------------------------------------------
   Indication Pay load data structure
---------------------------------------------------------------------------*/
typedef struct
{
  /*-------------------------------------------------------------------------
  Field mask indicates the validity of the fields.
  -------------------------------------------------------------------------*/
  uint16                          ind_field_mask;

  /*-------------------------------------------------------------------------
  This API is called using theattach profile ID as parameter. So that,
  if same API is called (e.g. IPv4 address complete for other profiles),
  it can be seperated out.
  -------------------------------------------------------------------------*/
  uint16                          profile_id;

  /*-------------------------------------------------------------------------
  Availability of IP address is notified.
  -------------------------------------------------------------------------*/
  ds_dsd_apm_ip_type              ip_address;

  /*-------------------------------------------------------------------------
  Availability of P-CSCF is notified.
  -------------------------------------------------------------------------*/
  ds_dsd_apm_ip_type              pcscf_address;

  /*-------------------------------------------------------------------------
  Availability of DNS Address is notified.
  -------------------------------------------------------------------------*/
  ds_dsd_apm_ip_type              dns_address;

  /*-------------------------------------------------------------------------
  Indicate normal LTE attach procedure or GW to LTE IRAT
  -------------------------------------------------------------------------*/
  ds_dsd_attach_complete_type     attach_type;

  /*-------------------------------------------------------------------------
  The actual PLMN ID value is passed by client.
  Is it relevant to pass 1xRTT, eHRPD and compare with LTE?? TBD
  -------------------------------------------------------------------------*/
  sys_plmn_id_s_type              plmn_id;

  /*-------------------------------------------------------------------------
  The system. This serves two purpose:
    - If the system goes OOS
    - If the system acquires a system (e.g. LTE, eHRPD)
  -------------------------------------------------------------------------*/
  sys_sys_mode_e_type             system;

  /*-------------------------------------------------------------------------
  Subscription ID on which this payload information is provided
  -------------------------------------------------------------------------*/
  sys_modem_as_id_e_type          subs_id;

  /*-------------------------------------------------------------------------
  EMM DETACH and REJECT are notified through this cause code. The semantics
  of reason code is as follows:
  if (EMM !=19)
  {
    internal error; //no switching
  }
  else
  {
    if (ESM cause code = 54)
    {
       internal error; //no action on switching
    }
    else
    {
      external error; // switching
    }
  }

  Note: ESM  cause code is valid only if EMM cause code = 19. This logic is
  embedded in NAS layer.
  -------------------------------------------------------------------------*/
  ps_iface_net_down_reason_type   cause_code;

  /*-------------------------------------------------------------------------
  PLMN Blocking Interval Timer value supplied by MH to APM which it gets
  from NAS
  -------------------------------------------------------------------------*/
  uint32                          t3402_timer_val;

  /*-------------------------------------------------------------------------
  Flag that informs APM whether APN switching needs to be performed on
  receiving an ATTACH REJECT IND
  -------------------------------------------------------------------------*/
  boolean                         switch_apn;

  /*-------------------------------------------------------------------------
  Flag that informs APM whether APN switching needs to be performed on
  receiving an ATTACH REJECT IND
  -------------------------------------------------------------------------*/
  boolean                         tlb_mode_active;

  /*-------------------------------------------------------------------------
    MH notifies APM regarding Attach PDP type
  -------------------------------------------------------------------------*/
  ds_profile_3gpp_pdp_type_enum_type  attach_pdp_type;

  /*---------------------------------------------------------------------- 
    UE is in transfer progress. The target rat is LTE
    --------------------------------------------------------------------*/
  boolean                             is_transfer_progress;

}ds_dsd_apm_ind_pay_load;


/*===========================================================================
                          REQUEST
===========================================================================*/
/*---------------------------------------------------------------------------
  Pay load field mask to indicate the available field in payload
---------------------------------------------------------------------------*/
#define DS_DSD_REQ_PROFILE_ID_FIELD_MASK        0x0001
#define DS_DSD_REQ_PCSCF_FIELD_MASK             0x0002
#define DS_DSD_REQ_IP_ADDRESS_FIELD_MASK        0x0004
#define DS_DSD_REQ_DNS_ADDRESS_FIELD_MASK       0x0008
#define DS_DSD_REQ_PCSCF_DNS_ADDR_FIELD_MASK    0x0010

/*---------------------------------------------------------------------------
  A client's REQUEST to get information
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_APM_ATTCH_REQ,                      /* Client requesting attach profile id          */
  DS_DSD_APM_MANDATORY_PCSCF_REQ,            /* Client requesting mandatory P-CSCFs          */
  DS_DSD_APM_MANDATORY_IP_REQ,               /* Client requesting mandatory IPs              */
  DS_DSD_APM_ATTACH_APN_AVAIL_REQ,           /* Client requesting if attach APN is available */
  DS_DSD_APM_MANDATORY_DNS_ADDR_REQ,         /* Client requesting mandatory DNS              */
  DS_DSD_APM_MANDATORY_PCSCF_OR_DNS_ADDR_REQ /* Client requesting mandatory PCSCF or DNS     */
}ds_dsd_apm_req_type;

typedef enum
{
  ipv4_mandatory_and_ipv6_donotcare = 1,
  ipv6_mandatory_and_ipv4_donotcare = 2,
  ipv4_mandatory_and_ipv6_mandatory = 3,
  ipv4_dontcare_and_ipv6_donotcare  = 4,
  ipv4_mandatory_or_ipv6_mandatory  = 5
}ipRequiredType;

typedef enum
{
  DS_DSD_APM_ATTACH_RULE,
  DS_DSD_APM_ON_DEMAND_RULE
}rule_category_type;

typedef enum
{
  DS_DSD_APM_INTER_ERR            = 0,
  DS_DSD_APM_ATTACH_APN_AVAILABLE = 1,
  DS_DSD_APM_ATTACH_APN_BLOCKED   = 2,
  DS_DSD_APM_EFS_CONFIG_ERROR     = 3,
  DS_DSD_APM_PROF_CONFIG_ERROR    = 4
}attach_apn_availability_info;

typedef struct
{
  /*-------------------------------------------------------------------------
  Field mask indicates the validity of the fields.
  -------------------------------------------------------------------------*/
  uint16                        req_field_mask;

  /*-------------------------------------------------------------------------
  The attach profile ID
  -------------------------------------------------------------------------*/
  uint16                        profile_id;

  rule_category_type            rule_type;

  ipRequiredType                mandatory_ip;

  ipRequiredType                mandatory_pcscf;

  ipRequiredType                mandatory_dns_addr;

  ipRequiredType                mandatory_pcscf_or_dns_addr;

  /*-------------------------------------------------------------------------
  Attach APN availability
  -------------------------------------------------------------------------*/
  attach_apn_availability_info  attach_apn_avail;

  /*-------------------------------------------------------------------------
  Subscription ID on which this payload information is provided
  -------------------------------------------------------------------------*/
  sys_modem_as_id_e_type        subs_id;

}ds_dsd_apm_req_pay_load;

#define DS_DSD_APM_MAX_APN_STRING_LEN  101

/*-------------------------------------------------------------------------
  Indicate the blocked IP type
-------------------------------------------------------------------------*/
#define DS_DSD_APM_IP_V4_BLOCKED_MASK  0x01
#define DS_DSD_APM_IP_V6_BLOCKED_MASK  0x02

typedef struct
{
  boolean                 attach_allowed;
  uint16                  ip_type_blocked;
  uint16                  apn_len;
  char                    apn_name[DS_DSD_APM_MAX_APN_STRING_LEN];
  sys_modem_as_id_e_type  subs_id;
}ds_dsd_apm_apn_block_info_type;

/*-------------------------------------------------------------------------
  The caller needs to allocate and free memory
-------------------------------------------------------------------------*/
typedef struct
{
  ds_dsd_apm_apn_block_info_type  *pdn_block_info;
  uint16                           num_apn;
}ds_dsd_apm_block_list_type;

/*---------------------------------------------------------------------------
  Enum for parameter names specified in the Rules file in EFS.
  Any update to this enum requires a corresponding update to
    dsd_apm_rule_param_name_id.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_APM_RULE_PARAM_ID_MIN = 0,
  DS_DSD_APM_APN_STRING = DS_DSD_APM_RULE_PARAM_ID_MIN,
  DS_DSD_APM_ATTACH_TYPE_RULES,
  DS_DSD_APM_ON_DEMAND_TYPE_RULES,
  DS_DSD_APM_PCSCF_REQUIRED,
  DS_DSD_APM_IPADDRESS_REQUIRED,
  DS_DSD_APM_CAUSE_CODE_RULES,
  DS_DSD_APM_DETACH_NETWORK_CAUSE,
  DS_DSD_APM_REJECT_NETWORK_CAUSE,
  DS_DSD_APM_DNSADDRESS_REQUIRED,
  DS_DSD_APM_PCSCF_OR_DNSADDRESS_REQUIRED,
  DS_DSD_APM_RULE_PARAM_ID_MAX = DS_DSD_APM_PCSCF_OR_DNSADDRESS_REQUIRED
}dsd_apm_rule_param_name_enum_type;

/*------------------------------------------------------------------------
Define a struct for cause code Queue block entry
------------------------------------------------------------------------*/
typedef struct
{
  boolean  cause_code_rule_valid;
  uint16   config_valid_flag;
  q_type   cause_code_info_q;
}ds_dsd_apm_efs_cause_code_info_type;

/*------------------------------------------------------------------------
struct that will be passed once AP completes setting attach prof info
------------------------------------------------------------------------*/
typedef struct
{
  uint8                                num_entries;
  boolean                              detach_required;
  boolean                              apm_rules_update;
  sys_modem_as_id_e_type               subs_id;
  ps_sys_attach_rules_info_type        attach_rules_arr[ATTACH_APN_MAX];
  ds_dsd_apm_efs_cause_code_info_type  cause_code_info;
  ps_sys_3gpp_lte_attach_pdn_list_type del_attach_list;
}ds_dsd_apm_detach_info_per_subs_type;

/*------------------------------------------------------------------------
 struct to record RS/RA status for Attach PDN
 rs_ra_in_progress: is RS/RA in progress?
 cm_queried_rs_ra_status: did CM query the RS/RA status?
------------------------------------------------------------------------*/
typedef struct
{
  boolean  rs_ra_in_progress;
  boolean  cm_queried_rs_ra_status;
}ds_dsd_apm_rs_ra_progress_type;

/*------------------------------------------------------------------------
Save the attach action from AP side, for now only DETACH is applied;
For PDN disconnect, we just tear down attach PDN and return CNF to AP, 
action shall be relegated to future if the needs arise;
For DETACH, we use this flag to handle the case attach is in progress,
it will be reset until detach rejected or attach rejected or attached with 
the latest attach profile (tmp becomes current)
------------------------------------------------------------------------*/
typedef enum
{
  ATTACH_ACTION_INVALID   = -1,
  ATTACH_ACTION_DETACH   = 0,
  ATTACH_ACTION_PDN_DIS       ,  
  ATTACH_ACTION_NONE
} attach_action_info_type ;

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_dsd_apm_qsh_mdump_collect
  
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
void ds_dsd_apm_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION       DS_DSD_APM_REQ_EX

DESCRIPTION    This is the external wrapper function called by external clients
               to request for the passed sub:
                - ATTACH PROFILE_ID.
                - Mandatory P-CSCF
                - Mandatory IP
                - ATTACH PROFILE AVAILABILITY

               The client supplies the memory to write the content.

DEPENDENCIES   NONE

RETURN VALUE   error codes as defined

SIDE EFFECTS   NONE
===========================================================================*/
ds_dsd_ext_error_code ds_dsd_apm_req_ex
(
  ds_dsd_apm_req_type       apm_req_event,
  ds_dsd_apm_req_pay_load  *pay_load,
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_REQ

DESCRIPTION    This is the external function called by external clients to
               request:
                - ATTACH PROFILE_ID.
                - Mandatory P-CSCF
                - Mandatory IP
                - ATTACH PROFILE AVAILABILITY

               The client supplies the memory to write the content.

DEPENDENCIES   NONE

RETURN VALUE   error codes as defined

SIDE EFFECTS   NONE
===========================================================================*/
ds_dsd_ext_error_code ds_dsd_apm_req
(
  ds_dsd_apm_req_type       apm_req_event,
  ds_dsd_apm_req_pay_load  *pay_load
);


/*===========================================================================
FUNCTION       DS_DSD_APM_IND

DESCRIPTION    This is the external function called by external clients.
               The clients have to notify the DSD APM module.

DEPENDENCIES   NONE

RETURN VALUE   error codes as defined

SIDE EFFECTS   NONE
===========================================================================*/
ds_dsd_ext_error_code ds_dsd_apm_ind
(
  ds_dsd_apm_ind_type       apm_ind_event,
  ds_dsd_apm_ind_pay_load  *pay_load
);

/*===========================================================================
FUNCTION        DS_DSD_APM_INIT

DESCRIPTION     DSD APM Module Initialization entry point. This function
                is called from dsd_init function during power up.

DEPENDENCIES    NONE

RETURN VALUE    NONE

SIDE EFFECTS    All the variables are initialized.
===========================================================================*/
void ds_dsd_apm_init( void );

/*===========================================================================
FUNCTION      DS_DSD_APM_INIT_PER_SUBS_ID

DESCRIPTION   External Wrapper for DSD APM Module Initialization entry point.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  All the variables are initialized. .
===========================================================================*/
void ds_dsd_apm_init_per_subsid
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_GET_NEXT_ATTACH_PROFILE_ID

DESCRIPTION    This utility function returns next attach profile id

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
uint16 ds_dsd_apm_get_next_attach_profile_id
(
  sys_modem_as_id_e_type  subs_id,
  uint16                  prev_prof_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_FORBIDDEN_APN_LIST_IND

DESCRIPTION    Indicate the list of blocked APN name and IP type by 3GPP MH.
               3GPP MH will call this function whenever blocked APN list
               changed.
               If there are APNs blocked at power up, 3GPP MH will call this
               function before ds_dsd_apm_ind(DS_DSD_APM_ATTACH_INIT_IND).

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_forbidden_apn_list_ind
(
  sys_modem_as_id_e_type       subs_id,
  ds_dsd_apm_block_list_type  *forbidden_apn_list
);

/*===========================================================================
FUNCTION      DS_DSD_DSC_COMMAND_PROCESS

DESCRIPTION   Put DSC CMD to DS task

DEPENDENCIES  None.

RETURN VALUE

SIDE EFFECTS  Commands CM module
===========================================================================*/
void ds_dsd_dsc_command_process
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================

FUNCTION DS_DSD_APM_UNSET_IP_RULE_REQUIRED

DESCRIPTION
  This is a external wrapper function for a utility unset rule required
  for current attach PDN.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_dsd_apm_unset_ip_rule_required
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_APN_CHANGE_CMD_PROCESS

DESCRIPTION    External wrapper used fro Processing of DSD APM APN change
               command

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_apn_change_cmd_process
(
  ds_cmd_type  *cmd_ptr
);


/*===========================================================================
FUNCTION       DS_DSD_APM_APN_DISABLE_CMD_PROCESS

DESCRIPTION    Processing of DSD APM APN disable command

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_apn_disable_cmd_process
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION       DS_DSD_APM_APN_DISABLE_CMD_PROCESS

DESCRIPTION    Processing of DSD APM delete attach profiles process

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_del_attach_profiles_cmd_process
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION       DS_DSD_APM_IS_ATTACH_PROFILE_LIST_UPDATE_NEEDED

DESCRIPTION    This function compares the new attach profile list sent by 
               AP to the current active attach profile list and determines if
               attach profile list update is needed or not.

DEPENDENCIES   NONE

RETURN VALUE   TRUE - if attach profile list update is needed 
               FALSE - if update is not needed 

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_is_attach_profile_list_update_needed
(
  ps_sys_3gpp_lte_attach_pdn_list_type  *new_attach_list_ptr,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_IS_DETACH_OR_DISCONNECT_PDN_ACTION_CNF_PENDING

DESCRIPTION    Checks if AP has yet to be notified of the action confirmation 
               for the action requested 

DEPENDENCIES   NONE 
 
PARAMS         subs_id : Subscription id for which the cnf is to be checked
 
RETURN VALUE   TRUE  if action conf is pending to be sent to AP 
               FALSE : if no action conf is pending

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_is_detach_or_disconnect_pdn_action_cnf_pending
(
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_NOTIFY_DETACH_OR_DISCONNECT_PDN_ACTION_COMPLETED

DESCRIPTION    Utility fucntion to send confirmaion to AP that the action 
               requested has been completed. It also unsets the action_cnf_pending
               flag

DEPENDENCIES   NONE 
 
PARAMS         subs_id : Subscription id for which conf is to be sent 
 
RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_notify_detach_or_disconnect_pdn_action_completed
(
  ds_dsd_apm_ap_action_conf_e_type   conf_type,
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_SET_AP_ACTION_PENDING_TYPE

DESCRIPTION    set AP action type

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_set_ap_action_pending_type
(
  attach_action_info_type          action_type,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_GET_AP_ACTION_PENDING_TYPE

DESCRIPTION    get AP action type

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
attach_action_info_type ds_dsd_apm_get_ap_action_pending_type
(
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_APM_EFS_DEL_TEMP_ATTACH_PROFILE_FILE

DESCRIPTION   This function clears all the temp attach profile file from EFS

DEPENDENCIES  None.

RETURN VALUE  void
===========================================================================*/
void ds_dsd_apm_efs_del_temp_attach_profile_file
(
  sys_modem_as_id_e_type  subs_id
);
/*===========================================================================
FUNCTION       DS_DSD_APM_SET_DETACH_OR_DISCONNECT_PDN_ACTION_CONF_FLAG

DESCRIPTION    Utility fucntion to set or unset the global action conf flag. 
               Set it to TRUE when action response is pending
               Set to FALSE when PS has been notified that action is completed.
 
DEPENDENCIES   NONE 
 
PARAMS         subs_id : Subscription id for which conf is to be sent 
               conf_flag : Value to which it is to be set 
 
RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_set_detach_or_disconnect_pdn_action_conf_flag
(
  boolean                            conf_flag,
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================

FUNCTION DS_DSD_APM_GET_CURRENT_PLMN_ID

DESCRIPTION
  This external wrapper function gets the current_plmn_id.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_dsd_apm_get_current_plmn_id
(
  sys_modem_as_id_e_type   subs_id,
  sys_plmn_id_s_type      *current_plmn_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_ATTACH_COMPLETED

DESCRIPTION    This is the external wrapper based utility function to check
               if ATTACH COMPLETED from APM modules perspective

DEPENDENCIES   NONE

RETURN VALUE   TRUE/FALSE

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_attach_completed
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_SET_ATTACH_PDN_LIST

DESCRIPTION    To set attach PDN list

DEPENDENCIES   NONE

RETURN VALUE   0: succeeds
              -1: fails

SIDE EFFECTS   NONE
===========================================================================*/
int ds_dsd_apm_set_attach_pdn_list
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno,
  void                    *reset_succ_cb_fptr
);

/*===========================================================================
FUNCTION       DS_DSD_APM_GET_ATTACH_PDN_LIST

DESCRIPTION    To get attach PDN list

DEPENDENCIES   NONE

RETURN VALUE   0: succeeds
              -1: fails

SIDE EFFECTS   NONE
===========================================================================*/
int ds_dsd_apm_get_attach_pdn_list
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION       DS_DSD_APM_GET_ATTACH_PDN_LIST_EX

DESCRIPTION    To get current and pending attach PDN list

DEPENDENCIES   NONE

RETURN VALUE   0: succeeds
              -1: fails

SIDE EFFECTS   NONE
===========================================================================*/
int ds_dsd_apm_get_attach_pdn_list_ex
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION       DS_DSD_APM_GET_MAX_ATTACH_PROFILE_NUM

DESCRIPTION    To get current maximum number of attach profiles configured

DEPENDENCIES   NONE

RETURN VALUE   0: succeeds
              -1: fails

SIDE EFFECTS   NONE
===========================================================================*/
int ds_dsd_apm_get_max_attach_profile_num
(
  void                   *arg_val_ptr,
  sys_modem_as_id_e_type  subs_id,
  int16                  *ps_errno
);

/*===========================================================================
FUNCTION       DS_DSD_APM_APN_SWITCH_STATUS_CHANGE

DESCRIPTION    IOCTL for disable/enable apn switch

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
int ds_dsd_apm_apn_switch_status_change
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION       DS_DSD_APM_SET_APN_SWITCH_STATUS

DESCRIPTION    to disable/enable apn switch temperarily, this will not write
               to the NV

DEPENDENCIES   NONE

RETURN VALUE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_set_apn_switch_status
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION       DS_DSD_APM_GET_APN_SWITCH_STATUS

DESCRIPTION    IOCTL for disable/enable apn switch

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
int ds_dsd_apm_get_apn_switch_status
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION       DS_DSD_APM_IS_APN_SWITCH_SUPPORT

DESCRIPTION    Indicate if APN switch is supported or not

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_is_apn_switch_support
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_IS_APN_IN_ATTACH_PDN_LIST

DESCRIPTION    This function checks whether a given APN is an attach APN

PARAMETERS     apn_name: APN which has to be checked

DEPENDENCIES   NONE

RETURN VALUE   TRUE: IF APN passed is Attach APN
               FALSE: Otherwise

SIDE EFFECTS   None
===========================================================================*/
boolean ds_dsd_apm_is_apn_in_attach_pdn_list
(
  sys_modem_as_id_e_type  subs_id,
  char                   *apn_name
);

/*===========================================================================
FUNCTION       DS_DSD_APM_CMD_PROCESS

DESCRIPTION    This function processes APM commands.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void ds_dsd_apm_cmd_process
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION       DS_DSD_APM_MATCH_CAUSE_CODES

DESCRIPTION    This function compares the cause_type & cause code provided by
               lowers layers with APN Switching cause codes in rule block

PARAMETERS     subs_id: Current active PS subs
               profile_id: Profile whose rule block entry is needed
               cause_type: Detach/Reject Network cause
               int_cause: Cause code passed by lower layers

DEPENDENCIES   NONE

RETURN VALUE   TRUE: If the cause returned by lower layers is present in APM
                     Rule block corresponding to the passed profile ID
               FALSE: If the cause code passed is not found in APM Rule block

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_match_cause_codes
(
  uint16                             profile_id,
  sys_modem_as_id_e_type             subs_id,
  uint16                             int_cause,
  dsd_apm_rule_param_name_enum_type  cause_type
);

/*===========================================================================
FUNCTION       DS_DSD_APM_WRITE_ATTACH_PROF_INFO_TO_EFS

DESCRIPTION    Processing of DSD APM Write attach profile info to EFS

DEPENDENCIES   NONE

RETURN VALUE   TRUE: If write was successful
               FALSE: Otherwise

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_write_attach_prof_info_to_efs
(
  ds_dsd_apm_detach_info_per_subs_type  *detach_info_ptr
);

/*===========================================================================
FUNCTION       DS_DSD_APM_SET_ATTACH_PROFILE_INFO

DESCRIPTION    This utility function is invoked via PS_SYS IOCTL when AP
               invokes the QMI API to set attach profile info

PARAMETERS     attach_prof_info_ptr: Attach profile info provided by AP
               subs_id: Active PS subs
               ps_errno: Error code if any to be returned to PS

DEPENDENCIES   NONE

RETURN VALUE   0: If Operation was successful
               -1: If for some reason the input params are invalid

SIDE EFFECTS   NONE
===========================================================================*/
int ds_dsd_apm_set_attach_profile_info
(
  void                    *attach_prof_info_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION       DS_DSD_DELETE_ALL_ATTACH_PROFILES

DESCRIPTION    Read the configuration to get the attach profile list
               If the profile is default     --> Reset it
               If the profile is non default --> Delete it

DEPENDENCIES   NONE

PARAMS         subs_id

RETURN VALUE   void

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_delete_all_attach_profiles
(
  sys_modem_as_id_e_type  as_subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_APN_CHANGE_CB_HDLR

DESCRIPTION    Callback handler to process apn_change

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_apn_change_cb_hdlr
(
   uint16                  profile_id,
   sys_modem_as_id_e_type  subs_id,
   void                   *data_ptr
);
/*===========================================================================
FUNCTION       DS_DSD_APM_FREE_ATTACH_PROF_CACHE_Q

DESCRIPTION    This function clears the cached attach profiles which are newly 
               receieved from AP 
 
PARAMETERS     subs_id: Current Active PS subs 

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_free_attach_prof_cache_q 
(
  sys_modem_as_id_e_type        subs_id
);
/*===========================================================================
FUNCTION       DS_DSD_AMP_CLEAR_TEMP_ATTACH_CONFIG

DESCRIPTION    Clear all the temp configuration
               1. Temp attach efs
               2. Temp rule efs
               3. Temp attach profile cache

DEPENDENCIES   NONE 
 
PARAMS         subs_id 

RETURN VALUE   NONE 

SIDE EFFECTS   NONE
===========================================================================*/

void ds_dsd_apm_clear_temp_attach_config
(
  sys_modem_as_id_e_type  as_subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_RS_RA_PROGRESS

DESCRIPTION    Record rs_ra_in_progress and post DS_3GPP_CM_RS_RA_DONE_IND to CM

DEPENDENCIES   NONE 
 
PARAMS         in_progress: if RS_RA is in progress
               subs_id: active PS subs

RETURN VALUE   TRUE: msg is sent successfully
               FALSE: msg fails to be sent

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_rs_ra_progress
(
  boolean                in_progress, 
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_CM_IS_RS_RA_PENDING_EX

DESCRIPTION    This is internal API to ds3gex_cm_is_rs_ra_pending(), which is 
               called by CM to check if IPv6 RS/RA procedure is complete for 
               Attach PDN

DEPENDENCIES   NONE 
 
PARAMS         subid: subscription id 

RETURN VALUE   TRUE  - RS/RA pending
               FALSE - RS/RA not pending 

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_cm_is_rs_ra_pending_ex
(
  sys_modem_as_id_e_type subid
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================
FUNCTION       DS_DSD_APM_GET_PROFILE_FROM_Q

DESCRIPTION    To get profiles from the q
 
DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_get_profile_from_q
(
   q_type                                 *q,
   ps_sys_3gpp_lte_attach_pdn_list_type   *attach_list
);

/*===========================================================================
FUNCTION       DS_DSD_APM_IS_ATTACH_PROFILE_LIST_MATCH

DESCRIPTION    This function compares the new attach profile list with active 
               or pending attach profile list. 

DEPENDENCIES   NONE

RETURN VALUE   TRUE - if attach profile list matches 
               FALSE - otherwise

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_is_attach_profile_list_match
(
  ps_sys_3gpp_lte_attach_pdn_list_type  *new_attach_list_ptr,
  ps_sys_3gpp_lte_attach_pdn_list_type  *attach_list_ptr,
  sys_modem_as_id_e_type                 subs_id
);
#endif /* DS_DSD_APM_EXT_I_H */
