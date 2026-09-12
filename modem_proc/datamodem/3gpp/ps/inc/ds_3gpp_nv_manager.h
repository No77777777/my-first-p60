/*!
  @file
  ds_3gpp_nv_manager.h

  @brief
  Manages NV/EFS items related to 3GPP MH

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2019 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/inc/ds_3gpp_nv_manager.h#10 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/14   sb      Initial version.
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#ifndef DS3GPPNVMANAGER_H
#define DS3GPPNVMANAGER_H

#include "datamodem_variation.h"
#ifdef FEATURE_DATA_3GPP
#include "dstask_v.h"
#include "ds_3gpp_hdlr.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_pdn_throttle_sm.h"
#include "ds_3gpp_bearer_context.h"
#include "ds_eps_pdn_context.h"
#include "ds_3gpp_rmsm_ip_common.h"
#include "cm.h"
#include "ps_iface.h"
#include "dsm_queue.h"
#include "dsumtspdpreg.h"
#include "ps_ppp.h"
#include "ds3gmgr.h"
#include "ds_3gppi_utils.h"
#include "sys.h"
#include "ps_iface_defs.h"
#include "cm_gw.h"
#include "ps_comp_logging_helper.h"
#include "ps_wmk_logging.h"
#include "ds3gpp_api.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

#ifdef FEATURE_QXDM_WM_TRACING
#include "fc_wm.h"
#endif /* FEATURE_QXDM_WM_TRACING */
#include "ds_3gpp_data_retry.h"

#include "pstimer.h"
#include "ds_3gpp_flow_control.h"
#include "ds_3gpp_nv_manager_ex.h"


#define NV_MANAGER_COOKIE 0xABCA0
#define DS_3GPP_NV_ENABLED    1

/*------------------------------------------------------------------------------ 
  Read the  NAS EFS
  ------------------------------------------------------------------------------*/
#define DS_3GPP_EFNAS_CONFIG_SIZE 120
/*------------------------------------------------------------------------------ 
  when teardown_unused_pdn_after_irat_bmask&DS_TEARDOWN_PDN_IRAT_LTE_TO_GWT is valid
  it means need to tear down pdn after irat from lte to GWT
  ------------------------------------------------------------------------------*/
#define DS_TEARDOWN_PDN_IRAT_LTE_TO_GWT  0x01

/*----------------------------------------------------------------------------
 The struct is defined because the default retry timer for lte is 24 hours,
 for other modes, it would be 12 min.
----------------------------------------------------------------------------*/
typedef struct
{
  uint32     def_retry_lte_timer;
  uint32     def_retry_umts_timer;
}ds_3gpp_nv_efnas_failure_def_retry_timer_info;

typedef struct
{
  uint32                                              cookie;
  sys_modem_as_id_e_type                              subs_id;
  boolean                                             apm_apn_switching;
  uint16                                              mtu_size;
  boolean                                             nv_rel10_throttling_per_plmn;
  boolean                                             ds_3gpp_redial_for_no_cause_code;
  boolean                                             ds_3gpp_reg_pdn_throttling;
  boolean                                             allow_multi_pdn_same_apn;
  ds_bearer_cntxt_tput_opt_struct_type                ds_bearer_cntxt_tput_opt_params;
  ds_bearer_cntxt_um_wm_threshold_struct_type         ds_bearer_cntxt_um_wm_threshold;
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type     ds_bearer_cntxt_tds_um_wm_threshold;
  boolean                                             is_tput_opt_params_present_in_nv;
  ds_3gpp_fcf_bidir_struct_type                       ds_3gpp_fcf_bidir_params;
  uint32                                              max_rab_reestab_req_retry_cnt;
  boolean                                             ds_eps_always_connect_def_pdn_flag;
  boolean                                             ds_eps_remove_unused_pdn;
  boolean                                             ds_eps_allow_def_pdn_teardown;
  boolean                                             ds_eps_allow_infinite_throt;
  boolean                                             ds_eps_switch_apn_on_reject;
  boolean                                             ds_3gpp_send_ps_data_avail_on_power_save;
  boolean                                             ds_3gpp_enforce_ps_data_avail;
  boolean                                             nbns_enabled;
  ds_3gpp_global_throttle_s                           global_throttling_nv;
  boolean                                             emergency_pdn_throttling_nv;
  uint32                                              ser_req_throt_sm_nv;
  uint8                                               auth_fb_enabled;
  boolean                                             ds_3gpp_disable_apn_matching;
  boolean                                             apn_param_chg_cfg;
  uint32                                              dl_tput_t_accumulate;
  boolean                                             enable_pdn_throt_per_rat;
  boolean                                             check_ehplmn_list;
  boolean                                             ignore_ui_data_throttling;
  uint32                                              uldl_tput_t_tmeas;
  boolean                                             null_apn_support_for_non_attach_pdn;
  byte                                               *efsnas_config;
  uint32                                              uplink_wcdma_taccum;
  uint32                                              uplink_lte_taccum;
  ds_eps_ca_display_threshold_s                       lte_ca_display_threshold;
  uint8                                               mnc_len;
  boolean                                             ds_3gpp_disable_mtu_request;
  ds_3gpp_iot_throttle_s                              iot_throttling_nv;
  uint8                                               rab_retry_timer;
  ds_3gpp_nv_efnas_failure_def_retry_timer_info       efnas_read_failure_def_retry_timer;
  ds_3gpp_dun_call_config_type     ds_3gpp_allow_single_ip_dun_call_dual_ip_profile;
  boolean                                             modem_def_profile_support;
  boolean                                             nw_override_enhancement;
  boolean                                             attach_with_nw_override;
#ifdef FEATURE_DATA_4_5G
  uint8                                               ui_display_4_5g_support;
#endif
  uint32                                              ds_dsd_apm_attach_action_type;
  boolean                                             ps_data_off_feature_enable;
  boolean                                             generic_throt_skip_sm_retry;
  ds_3gpp_ran_bit_data_rate_support_config           *ran_bit_rate_support;
  ds3gtputmgr_w2l_redir_config_s                      w2l_redir_config;
  boolean                                             ignore_preced_conflict_on_same_bearer;
  boolean                                             enable_default_pdn_throttling;  
  boolean                                             lte_attach_pending;  
  /*indicate user name and password in profile is case sensitive or not*/
  boolean                                             usr_pwd_case_sensitive;
  /*Indicate if it needs to keep KAMGR on regardless of WIfi or UI setting*/
  boolean                                             kamgr_always_on;
  boolean                                             ignore_ims_setting;
/*different bit stands for different irat. for example,bit 0 is 1 means enable tear down
    ununsed pdn after irat from 4G to 3G*/
  uint8                                               teardown_unused_pdn_after_irat_bmask;
  boolean                                             nw_override_cleanup;
boolean                                             force_disconnect;
}ds_3gpp_nv_per_subs_info_type;

extern ds_3gpp_nv_per_subs_info_type*  ds_3gpp_nv_per_subs_info_p[DS3GSUBSMGR_SUBS_ID_MAX];

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_qsh_mdump_collect
  
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
void ds_3gpp_nv_manager_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_INIT

DESCRIPTION
  This function initializes the nv manager module. This function is called
  during boot up. This function is responsible for intializing the nv info
  for primary subscription.
 
	PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_init(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_init_per_device_nv

DESCRIPTION
  This function is called during boot up. This function is responsible for 
  intializing the nv info for device specific nv
 
	PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_nv_manager_init_per_device_nv(void);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_INIT_PER_SUBS_ID

DESCRIPTION
  This function is responsible for intializing the nv info for each
	subscription.
 
  PARAMETERS
  subs_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_init_per_subs_id
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_VALIDATE_NV_SUBS_INFO

DESCRIPTION
  This function is used to reset the nv info structure per subscription
 
  PARAMETERS
  subs_id    subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_nv_manager_reset_per_subs_id
(
  sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_VALIDATE_NV_SUBS_INFO

DESCRIPTION
  This function is used to validate the nv info pointer.
 
  PARAMETERS
  nv_validation_info- nv validation info pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_nv_manager_validate_nv_subs_info
(
   ds_3gpp_nv_per_subs_info_type* nv_validation_info
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_rab_retry_timer

DESCRIPTION
  This function reads the rab_retry_timer_nv item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/

void ds_3gpp_nv_manager_read_rab_retry_timer
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_rab_retry_timer

DESCRIPTION
  This function gets the rab_retry_timer nv value for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Return RAB retry timer val

SIDE EFFECTS
===========================================================================*/

uint8 ds_3gpp_nv_manager_get_rab_retry_timer
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_NV_SUBS_INFO

DESCRIPTION
  This function is used to return the nv validation info pointer.for the
  subscription passed
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  ds_3gpp_nv_per_subs_info_type - nv validation info pointer

SIDE EFFECTS
  None.

===========================================================================*/
ds_3gpp_nv_per_subs_info_type* ds_3gpp_nv_manager_get_nv_subs_info
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_READ_MAX_RAB_REESTAB_REQ_RETRY_CNT

DESCRIPTION
  This function is used to read mx rab reestablishment count nv item
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_max_rab_reestab_req_retry_cnt
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_READ_BACK_PRESSURE_PARAMS

DESCRIPTION
  This function is used to read back pressure parameters nv item
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_back_pressure_params
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_READ_TCP_THRESHOLD_ACK_CNT

DESCRIPTION
  This function is used to read tcp threshold acknowledgement count per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_tcp_threshold_ack_cnt
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_READ_UM_WM_THRESHOLD

DESCRIPTION
  This function is used to read um watermark threshold information per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_um_wm_threshold
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_READ_PS_DATA_AVAIL_ON_POWER_SAVE

DESCRIPTION
  This function is used to read ps data available on power save nv item
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_ps_data_avail_on_power_save
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_READ_SER_REQ_THOR_SM_NV

DESCRIPTION
  This function is used to read service request throttle info nv item
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  ds_3gpp_nv_manager_read_ser_req_thor_sm_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_AUTH_FALL_BACK

DESCRIPTION
  This function is used to read auth fall back from nv item per subscription

  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_nv_manager_read_auth_fallback
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_AUTH_FALLBACK

DESCRIPTION
  This function is used to get auth fall back from nv item per subscription

  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  uint8 - auth fallback infor

SIDE EFFECTS
  None.

===========================================================================*/

uint8 ds_3gpp_nv_manager_get_auth_fallback
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_GET_BTPUT_OPT_PARAMS_PRESENT_IN_NV

DESCRIPTION
  This function is used to retrieve whether the throughput optimization
  parameters are present per subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  boolean -present or not present

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_nv_get_tput_opt_params_present_in_nv
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_GET_BACK_PRESSURE_PARAMS

DESCRIPTION
  This function is used to retrieve the back pressure parameters per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  ds_bearer_cntxt_tput_opt_struct_type -back pressure parameters information

SIDE EFFECTS
  None.

===========================================================================*/
ds_bearer_cntxt_tput_opt_struct_type ds_3gpp_nv_get_back_pressure_params
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_GET_TDS_WM_THRESHOLD

DESCRIPTION
  This function is used to retrieve tds watermark threshold values per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type - td wm info

SIDE EFFECTS
  None.

===========================================================================*/
ds_bearer_cntxt_tds_um_wm_threshold_struct_type  ds_3gpp_nv_get_tds_wm_threshold
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_GET_UM_WM_THRESHOLD

DESCRIPTION
  This function is used to retrieve um watermark threshold values per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  ds_bearer_cntxt_um_wm_threshold_struct_type - um wm info

SIDE EFFECTS
  None.

===========================================================================*/

ds_bearer_cntxt_um_wm_threshold_struct_type  ds_3gpp_nv_get_um_wm_threshold
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_GET_MAX_RAB_REESTAB_RETRY_CNT

DESCRIPTION
  This function is used to retrieve umax rab reestablishment retry count per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  uint32 -max rab reestab count

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_3gpp_nv_get_max_rab_reestab_retry_cnt
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_GET_PS_DATA_AVAIL_ON_POWER_SAVE

DESCRIPTION
  This function is used to retrieve ps data available on power save per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_nv_manager_get_ps_data_avail_on_power_save
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_GET_enforce_ps_data_avail

DESCRIPTION
  This function is used to retrieve ps data available on power save for ims
  PDN per subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_nv_manager_get_enforce_ps_data_avail
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_READ_ENFORCE_PS_DATA_AVAIL

DESCRIPTION
  This function is used to retrieve ps data available on power save for ims
  PDN per subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_enforce_ps_data_avail
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_GET_SER_REQ_THOR_SM_NV

DESCRIPTION
  This function is used to retrieve service request throttle info per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  uint32. - Service request throttling info

SIDE EFFECTS
  None.

===========================================================================*/
uint32  ds_3gpp_nv_manager_get_ser_req_thor_sm_nv
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_MTU_FROM_NV

DESCRIPTION
  This function is used to read mtu value from nv item per subscription

  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_mtu_from_nv 
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANGER_READ_REDIAL_NO_CAUSE_CODE_FROM_NV

DESCRIPTION
  This function reads the redial for no cause from NV item
  NV item per subscription: 72563
 
PARAMETERS
  subs_id  subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None

SIDE EFFECTS

===========================================================================*/


void ds_3gpp_nv_manager_read_redial_no_cause_code_from_nv
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_AUTH_FALLBACK

DESCRIPTION
  This function is used to get auth fall back from nv item per subscription

  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  uint8 - auth fallback infor

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_nv_manager_read_apm_apn_switching
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
  FUNCTION ds_3gpp_nv_manager_READ_NAS_UMTS_REL_VERSION
  
  DESCRIPTION
  This function is is called in 3gpp nv manager init.It reads the NAS
  Complaince NV item and updates the global variable ds_3gpp_nv_umts_rel_version
  accordingly for umts specific throttling scenerios.
    
  PARAMETERS  
  None.
      
  DEPENDENCIES 
  None.
    
  RETURN VALUE 
  None.
     
  SIDE EFFECTS 
  None.
    
===========================================================================*/
void ds_3gpp_nv_manager_read_nas_umts_rel_version
(
#ifdef FEATURE_MSIM_L_PLUS_L
  sys_modem_as_id_e_type subs_id
#endif
);
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_bcm_info_validation_nv

DESCRIPTION
  This function gets the BCM validation NV setting.

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE /FALSE

SIDE EFFECTS

===========================================================================*/
boolean ds_3gpp_nv_manager_get_bcm_info_validation_nv(void);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_iot_throttling_nv

DESCRIPTION
  This function reads the iot throttling NV item for each
  subscription
 
PARAMETERS
  subs_id -  subscription id

DEPENDENCIES
  None.

RETURN VALUE  

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_iot_throttling_nv
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_multi_pdn_dun_nv

DESCRIPTION
  This function gets the multi pdn dun nv item

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE /FALSE

SIDE EFFECTS

===========================================================================*/

boolean ds_3gpp_nv_manager_get_multi_pdn_dun_nv(void);
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_l_to_w_qos_continuity_nv

DESCRIPTION
  This function gets the L to W QOS continuity NV item

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE/FALSE

SIDE EFFECTS

===========================================================================*/
boolean ds_3gpp_nv_manager_get_l_to_w_qos_continuity_nv(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_umts_nw_init_qos_support_nv

DESCRIPTION
  This function gets the umts_nw_init_qos_support NV item
NV: 70331
modem/data/3gpp/umts_nw_initiated_qos_support

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE  
  1 - If NV enabled
  0 - otherwise

SIDE EFFECTS
  None

===========================================================================*/
uint8 ds_3gpp_nv_manager_get_umts_nw_init_qos_support_nv(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_umts_ue_init_qos_support_nv

DESCRIPTION
  This function gets the umts_nw_init_qos_support NV item
  NV: 66044
  modem/data/3gpp/umtsps/qos_support

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE  
  1 - If NV enabled
  0 - otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_umts_ue_init_qos_support_nv(void);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_EFS_3GPP_REL_VERSION

DESCRIPTION 
  This function reads the 3GGP_REL_VERSION NV and saves for caching 
 
PARAMETERS
  None
DEPENDENCIES
  None.

RETURN VALUE
  None
SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_nv_manager_read_efs_3gpp_rel_version(void);


/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_3gpp_rel_version

DESCRIPTION 
  This function returns 3GGP_REL_VERSION from cache  
 
PARAMETERS
  None
DEPENDENCIES
  None.

RETURN VALUE
  cached 3GPP_REL_VERSION NV value
SIDE EFFECTS
  None.
===========================================================================*/
uint8 ds_3gpp_nv_manager_get_3gpp_rel_version(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_l_to_w_qos_continuity

DESCRIPTION
		This function reads the nv l_to_w qos continuity
	
PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/

void ds_3gpp_nv_manager_read_l_to_w_qos_continuity(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_umts_nw_init_qos_support_nv(

DESCRIPTION
 This function gets the umts_nw_init_qos_support NV item
NV: 70331
modem/data/3gpp/umts_nw_initiated_qos_support
	
PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_umts_nw_init_qos_support_nv(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_umts_ue_init_qos_support_nv

DESCRIPTION
 This function gets the ds_3gpp_nv_umts_ue_init_qos_support NV item
 NV: 66044
  modem/data/3gpp/umtsps/qos_support

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_umts_ue_init_qos_support_nv(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_bcm_validation

DESCRIPTION
  This function reads the nv bcm validation.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_bcm_validation(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_emergency_pdn_throttle_nv

DESCRIPTION
  This function reads the emergency PDN throttle NV item.

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_emergency_pdn_throttle_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_f3_enable

DESCRIPTION
  This function reads the f3 supress parameter nv.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_f3_enable();

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_multi_pdn_dun_nv

DESCRIPTION
  This function reads the multi pdn dun nv.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_multi_pdn_dun_nv(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_check_ehplmn_list_from_nv

DESCRIPTION
  This function reads the check_ehplmn_list NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
===========================================================================*/
void ds_3gpp_nv_manager_read_check_ehplmn_list_from_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_mnc_len_from_nv

DESCRIPTION
  This function reads the check_ehplmn_list NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
===========================================================================*/
void ds_3gpp_nv_manager_read_mnc_len_from_nv
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_ignore_ui_data_throttling_from_nv

DESCRIPTION
  This function reads the  ignore_ui_data_throttling NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
===========================================================================*/
void ds_3gpp_nv_manager_read_ignore_ui_data_throttling_from_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_enable_pdn_throt_per_rat_from_nv

DESCRIPTION
  This function reads the enable_pdn_throt_per_rat NV item for each
  subscription

PARAMETERS
  subs_id: subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
===========================================================================*/
void ds_3gpp_nv_manager_read_enable_pdn_throt_per_rat_from_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_rel_10_throttling_per_plmn__from_nv

DESCRIPTION
  This function reads the rel_10_throttling_per_plmn from NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/

void ds_3gpp_nv_manager_read_rel_10_throttling_per_plmn_from_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_regular_pdn_throttling_from_nv

DESCRIPTION
  This function reads the regular pdn throttling  NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_regular_pdn_throttling_from_nv
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_multi_pdn_same_apn_nv

DESCRIPTION
  This function reads the multi pdn same apn NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/

void   ds_3gpp_nv_manager_read_multi_pdn_same_apn_nv
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_global_throttling_nv

DESCRIPTION
  This function reads the global throttling NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/


void ds_3gpp_nv_manager_read_global_throttling_nv
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_multi_pdn_same_apn

DESCRIPTION
  This function gets the multi pdn same apn nv value for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether Multi Pdn same apn nv is enabled.

SIDE EFFECTS
===========================================================================*/

boolean ds_3gpp_nv_manager_get_multi_pdn_same_apn
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_global_throttling

DESCRIPTION
  This function gets the global throttling nv value for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether global throttling  nv is enabled.

SIDE EFFECTS
===========================================================================*/

boolean ds_3gpp_nv_manager_get_global_throttling
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_emergency_pdn_throttling

DESCRIPTION
  This function gets the emergency throttling nv value for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether emergency throttling  nv is enabled.

SIDE EFFECTS
===========================================================================*/

boolean ds_3gpp_nv_manager_get_emergency_pdn_throttling
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_reg_pdn_throttling 

DESCRIPTION
  This function gets the Regular PDN Throttling Flag for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether Regular PDN Throttling is enabled.

SIDE EFFECTS
===========================================================================*/

boolean ds_3gpp_nv_manager_get_reg_pdn_throttling 
(
  sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_SET_REG_PDN_THROTTLING

DESCRIPTION
  This function sets the Regular PDN Throttling Flag for each subscription

PARAMETERS
  Boolean Flag
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
===========================================================================*/
void ds_3gpp_nv_manager_set_reg_pdn_throttling 
(
  boolean flag,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_DISABLE_APN_MATCHING_FROM_NV

DESCRIPTION
  This function reads the disable_apn_matching NV(73529 ) item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_disable_apn_matching_from_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_APN_PARAM_CHG_CFG_FROM_NV

DESCRIPTION
  This function reads the apn_param_chg_cfg NV(73603 ) item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_apn_param_chg_cfg_from_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_ULDL_TPUT_T_TMEAS_FROM_NV

DESCRIPTION
  This function reads the t_meas for uldl throughput reporting
  NV(73656) item for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/

void ds_3gpp_nv_manager_read_uldl_tput_t_tmeas_from_nv
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_DL_TPUT_T_ACCUMULATE_FROM_NV

DESCRIPTION
  This function reads the t_accumuate NV(73587) item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_dl_tput_t_accumulate_from_nv
(
   sys_modem_as_id_e_type    subs_id
);
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_mtu_size

DESCRIPTION
  This function gets the mtu size for the nv item for each subscriprion

PARAMETERS
  subs_id subscription id
  
DEPENDENCIES
  None.

RETURN VALUE  
  uint16  mtu value

SIDE EFFECTS

===========================================================================*/

uint16 ds_3gpp_nv_manager_get_mtu_size
( 
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_force_disconnect_nv

DESCRIPTION
  This function reads the force_disconnect NV for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE 
  None 

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_nv_manager_read_force_disconnect_nv
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_force_disconnect_nv

DESCRIPTION
  This function is used to get the network override enhancement NV value 

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - NV set to 1
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_force_disconnect_nv
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_is_no_cause_code_redial_set

DESCRIPTION
  This function checks  whether ds_3gpp_redial_for_no_cause_code is set for
  each subscription

PARAMETERS
  subs_id subscription id

  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE- if successfull
  FALSE- invalid PDN ptr/invalid SUBS_ID

SIDE EFFECTS

===========================================================================*/
boolean ds_3gpp_nv_manager_get_is_no_cause_code_redial_set
(
  sys_modem_as_id_e_type        subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_apm_apn_switching

DESCRIPTION
  This function returns true ;if the nv item corresponding to the 
  apm apn switching per subscription is set to true


PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/


boolean ds_3gpp_nv_manager_get_apm_apn_switching
(
  sys_modem_as_id_e_type        subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_check_ehplmn_list

DESCRIPTION
  This function returns true if check_ehplmn_list NV item is set to TRUE

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_check_ehplmn_list
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_mnc_len

DESCRIPTION
  This function gets mnc len for imsi from mnc len NV item

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  uint8 - mnc length

SIDE EFFECTS
  None

===========================================================================*/
uint8 ds_3gpp_nv_manager_get_mnc_len
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_enable_pdn_throt_per_rat

DESCRIPTION
  This function returns the value of enable_pdn_throt_per_rat NV item

PARAMETERS
  subs_id: subscription id

DEPENDENCIES
  None.

RETURN VALUE
  Value of enable_pdn_throt_per_rat NV item

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_enable_pdn_throt_per_rat
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_ignore_ui_data_throttling

DESCRIPTION
  This function returns the value of ignore_ui_data_throttling
  NV item

PARAMETERS
  subs_id: subscription id

DEPENDENCIES
  None.

RETURN VALUE
  Value of ignore_ui_data_throttling NV item

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_ignore_ui_data_throttling
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_nv_rel_10_throttling_per_plmn

DESCRIPTION
  This function returns true ;if the nv item corresponding to the 
  release 10 throttling per plmn is set to true


PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/

boolean ds_3gpp_nv_manager_get_nv_rel_10_throttling_per_plmn
(
   sys_modem_as_id_e_type        subs_id
);


/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_nv_umts_rel_version

DESCRIPTION
  This Function reads the NAS Complaince NV item and updates the global variable 
  ds_3gpp_nv_umts_rel_version accordingly for umts specific scenerios.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  ue_nas_rel_compliance_type enum type

SIDE EFFECTS
  None

===========================================================================*/
ue_nas_rel_compliance_type ds_3gpp_nv_manager_get_nv_umts_rel_version
(
  sys_modem_as_id_e_type subs_id  
);


/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_nv_lte_rel_10

DESCRIPTION
  This function returns true if the nv item corresponding to the file
  /nv/item_files/modem/lte/lte_3gpp_release_ver is LTE_3GPP_REL10
  and release 10 throttling is enabled

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/


boolean ds_3gpp_nv_manager_get_nv_lte_rel_10
(
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_DISABLE_APN_MATCHING

DESCRIPTION
  This function returns true if the nv item corresponding to the file
  /nv/item_files/modem/data/3gpp/lteps/disable_apn_matching 

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_disable_apn_matching
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_APN_PARAM_CHG_CFG

DESCRIPTION
  This function returns true if the nv item corresponding to the file
  /nv/item_files/modem/data/3gpp/lteps/disable_apn_matching 

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_apn_param_chg_cfg
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_DL_TPUT_T_TMEAS

DESCRIPTION
  This function retreieves the uldl tmeas value from the nv

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  Value in the efs or default value

SIDE EFFECTS
  None

===========================================================================*/


uint32 ds_3gpp_nv_manager_get_uldl_tput_t_tmeas
(
   sys_modem_as_id_e_type subs_id
);
#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_UPLINK_TACCUM_WCDMA

DESCRIPTION
This function retreieves the uplink taccum value for wcdma mode

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  Value in the efs or default value

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_nv_manager_get_uplink_tput_tacccum_wcdma
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_UPLINK_TACCUM_LTE

DESCRIPTION
  This function retreieves the uplink taccum value for lte mode

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  Value in the efs or default value

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_nv_manager_get_uplink_tput_tacccum_lte
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_UPLINK_WCDMA_TPUT_TACCUM_FROM_NV

DESCRIPTION
  This function reads the taccum for uplink throughput reporting
  for each subscription on WCDMA


PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_uplink_wcdma_tput_taccum_from_nv
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_UPLINK_LTE_TPUT_TACCUM_FROM_NV


DESCRIPTION
  This function reads the taccum for uplink throughput reporting
  for each subscription on LTE


PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_uplink_lte_tput_taccum_from_nv
(
   sys_modem_as_id_e_type    subs_id
);
#endif
/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_DL_TPUT_T_ACCUMULATE

DESCRIPTION
  This function returns true if the nv item corresponding to the file
  /nv/item_files/modem/data/3gpp/lteps/t_accumulate  (73587)

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  Value in the efs or default value

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_nv_manager_get_dl_tput_t_accumulate
(
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_nv_umts_rel_version

DESCRIPTION
  This Function returns the NAS Complaince NV item  read during nv manager
  initialization

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  boolean - True -> Umts Nas release version 10 or above
          - False -> Otherwise

SIDE EFFECTS
  None

===========================================================================*/


boolean ds_3gpp_nv_manager_is_rel_10_or_above_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_nbns_enabled

DESCRIPTION
  This Function reads the  nbns nv item per subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/


void ds_3gpp_nv_manager_read_nbns_enabled
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_nbns_enabled

DESCRIPTION
  This Function returns true; if nbns nv item is enabled

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  boolean true-  if nbns nv item is enabled
          false   if nbns nv item is disabled

SIDE EFFECTS
  None

===========================================================================*/


boolean ds_3gpp_nv_manager_get_nbns_enabled
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_call_orig_before_ps_attach

DESCRIPTION
  This function returns true if call orig is allowed before ps attach.
..This value is read per subscription

PARAMETERS 
  subs_id subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - if call orig is allowed before ps attach
  False - 
 
SIDE EFFECTS
  None.
===========================================================================*/

boolean ds_3gpp_nv_manager_get_call_orig_before_ps_attach
(
  sys_modem_as_id_e_type subs_id
);
#ifdef FEATURE_DATA_LTE

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_always_connect_def_pdn_flag

DESCRIPTION
  This function reads the always connect default pdn NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/

void ds_3gpp_nv_manager_read_always_connect_def_pdn_flag
(
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_allow_default_pdn_teardown

DESCRIPTION
  This function reads the always  default pdn tear down  NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/


void ds_3gpp_nv_manager_read_allow_default_pdn_teardown
(
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_switch_apn_on_reject

DESCRIPTION
  This function reads the apn switch on reject  NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/

void ds_3gpp_nv_manager_read_switch_apn_on_reject
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_allow_infinite_throttling

DESCRIPTION
  This function reads the always  infinite throttling  NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/

void ds_3gpp_nv_manager_read_allow_infinite_throttling
(
   sys_modem_as_id_e_type subs_id
);



/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_remove_unused_pdn

DESCRIPTION
  This function reads the remove unused pdn NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_remove_unused_pdn
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_allow_def_pdn_teardown

DESCRIPTION
  This function informs whether default LTE PDN can be torn down based on the
   nv value per subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/


boolean ds_3gpp_nv_manager_get_allow_def_pdn_teardown
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_allow_infinite_throttling
DESCRIPTION
  This function informs whether infinite throttling is enabled based on the
   nv value per subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_nv_manager_get_allow_infinite_throttling
(
   sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_switch_apn_on_reject
DESCRIPTION
  This function informs whether apn switch on reject is enabled based on the
   nv value per subscription

PARAMETERS
  subs_id subscription id
 
 
DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/


boolean ds_3gpp_nv_manager_get_switch_apn_on_reject
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_always_connect_def_pdn_flag
 
DESCRIPTION
  This function informs whether always connect on default pdn is enabled based on the
   nv value per subscription

PARAMETERS
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_nv_manager_get_always_connect_def_pdn_flag
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_remove_unused_pdn

DESCRIPTION
  This function gets the value of NV 70334 REMOVE_UNUSED_PDN per subscription

PARAMETERS 
  subs_id subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - REMOVE_UNUSED_PDN =1.
  False - REMOVE_UNUSED_PDN =0.
 
SIDE EFFECTS
  None.
===========================================================================*/

boolean ds_3gpp_nv_manager_get_remove_unused_pdn
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_ignore_inactivity_timer

DESCRIPTION
  This function returns true if ignore inactivity timer nv is set to true

PARAMETERS 
  subs_id subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - if call orig is allowed before ps attach
  False - 
 
SIDE EFFECTS
  None.
===========================================================================*/
uint8  ds_3gpp_nv_manager_get_ignore_inactivity_timer
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_nv_lte_rel_version

DESCRIPTION
  This function returns  the LTE release version, read during pdn initialization.

PARAMETERS
  void

DEPENDENCIES
  None.

RETURN VALUE
  lte_3gpp_release_ver_e enum type

SIDE EFFECTS
  None

===========================================================================*/
lte_3gpp_release_ver_e ds_3gpp_nv_manager_get_nv_lte_rel_version
(
  sys_modem_as_id_e_type subs_id
);
/* ds_3gpp_nv_manager_get_nv_lte_rel_version */

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_ENABLE_SEND_TRM_PRIORITY_NV

DESCRIPTION
  This function reads the ENABLE_SEND_TRM_PRIORITY NV(73562) item for each
  subscription

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE  
  None

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_enable_send_trm_priority_nv(void);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_ENABLE_SEND_TRM_PRIORITY

DESCRIPTION
  This function gets the value of NV 73562 ENABLE_SEND_TRM_PRIORITY per
  subscription

PARAMETERS 
  None
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - ENABLE_SEND_TRM_PRIORITY = 1.
  False - ENABLE_SEND_TRM_PRIORITY = 0.
 
SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_nv_manager_get_enable_send_trm_priority(void);


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_NULL_APN_SUPPORT_FOR_NON_ATTACH_PDN_FROM_NV

DESCRIPTION
  This function reads null_apn_support_for_non_attach_pdn NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_null_apn_support_for_non_attach_pdn_from_nv
(
   sys_modem_as_id_e_type    subs_id
);/*ds_3gpp_nv_manager_read_null_apn_support_for_non_attach_pdn_from_nv*/


/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_NULL_APN_SUPPORT_FOR_NON_ATTACH_PDN

DESCRIPTION
  This function returns true if the nv item corresponding to the file
  /nv/item_files/modem/data/3gpp/lteps/null_apn_support_for_non_attach_pdn

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - NV set to 1
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_null_apn_support_for_non_attach_pdn
(
   sys_modem_as_id_e_type subs_id
);/*ds_3gpp_nv_manager_get_null_apn_support_for_non_attach_pdn*/


#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_read_efnas_config

DESCRIPTION
  This function reads the NAS configuration NV

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_efnas_config
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_efnas_config

DESCRIPTION
  This function gets the NAS configuration

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
byte  *ds_3gpp_nv_manager_get_efnas_config
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_DATA_SHARK_NV

DESCRIPTION
  This function reads the DS_3GPP_DATA_SHARK_ENABLE NV() item

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS 
  None. 

===========================================================================*/
void ds_3gpp_nv_manager_read_data_shark_nv(void);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_DATA_SHARK

DESCRIPTION
  This function gets the value of NV DS_3GPP_DATA_SHARK_ENABLE

PARAMETERS 
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - DS_3GPP_DATA_SHARK_ENABLE = 1.
  False - DS_3GPP_DATA_SHARK_ENABLE = 0.
 
SIDE EFFECTS
  None.
 
===========================================================================*/
boolean ds_3gpp_nv_manager_get_data_shark(void);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_ENABLE_DYNAMIC_WCDMA_WM_TUNING

DESCRIPTION
  This Function reads the Enable Dynamic WCDMA WM Tuning EFS item (NV# 73689)

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_read_enable_dynamic_wcdma_wm_tuning( void );

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_enable_dynamic_wcdma_wm_tuning 

DESCRIPTION
  This function informs returns Enable dynamic WCMDA WM EFS item (NV# 73689)

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_nv_manager_get_enable_dynamic_wcdma_wm_tuning( void );

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_REL12_QCI_SUPPORTED

DESCRIPTION
  This Function reads the Release 12 QCI Support Nv item 

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_read_rel12_qci_support( void );

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_REL12_QCI_SUPPORT

DESCRIPTION
  This function gets the value of NV Release 12 QCI Support Nv item

PARAMETERS 
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - Release 12 qci supported
  False - Release 12 qci not supported
 
SIDE EFFECTS
  None.
 
===========================================================================*/
boolean ds_3gpp_nv_manager_get_rel12_qci_support(void);

#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_LTE_CA_DISPLAY_THRESHOLD

DESCRIPTION
  This function reads LTE_CA_DISPLAY_THRESHOLD NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_lte_ca_display_threshold
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_LTE_CA_DISPLAY_THRESHOLD

DESCRIPTION
  This function returns the value of LTE_CA_DISPLAY_THRESHOLD configuration.

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  ds_eps_ca_display_threshold_s*

SIDE EFFECTS
  None

===========================================================================*/
ds_eps_ca_display_threshold_s* ds_3gpp_nv_manager_get_lte_ca_display_threshold
(
  sys_modem_as_id_e_type  subs_id
);
#endif /* FEATURE_DATA_LTE */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_PERSISTENT_PROFILE_OPTIMIZATION

DESCRIPTION
  This Function reads persistent profile optimization NV

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_read_persistent_profile_optimization( void );

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_PERSISTENT_PROFILE_OPTIMIZATION

DESCRIPTION
  This Function returns the value of persistent profile optimization NV

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_persistent_profile_optimization( void );

/*=========================================================================

FUNCTION  DS_3GPP_NV_MANAGER_GET_DISABLE_ATTACH_ON_PWR_UP

DESCRIPTION
  This function returns disable_attach_on_pwr_up

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_disable_attach_on_pwr_up(void);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_ENABLE_LPM_ON_PWR_UP

DESCRIPTION
  This function returns enable_lpm_on_pwr_up

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_enable_lpm_on_pwr_up(void);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_PRE_ATTACH_SRV_DOMAIN_PREF

DESCRIPTION
  This function is used to retrieve pre attach srv domain pref
 
  PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  uint8. - pre attach srv domain pref

SIDE EFFECTS
  None.

===========================================================================*/
uint8  ds_3gpp_nv_manager_get_pre_attach_srv_domain_pref(void);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_PRE_ATTACH_MODE_PREF

DESCRIPTION
  This function is used to retrieve pre attach mode pref
 
  PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  uint8. - pre attach mode pref

SIDE EFFECTS
  None.

===========================================================================*/
uint8  ds_3gpp_nv_manager_get_pre_attach_mode_pref(void);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_MANAGE_ATTACH_ON_PWR_UP

DESCRIPTION
  This Function reads manage attach on power up

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_read_manage_attach_on_pwr_up( void );

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_platform_config_nv

DESCRIPTION
  This function reads the device platform config.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_platform_config_nv(void);

/*===========================================================================
  FUNCTION ds_3gpp_nv_manager_get_platform_config

  DESCRIPTION
    This function gets the platform config value.

  PARAMETERS
    None

  DEPENDENCIES
    None.

  RETURN VALUE  
    TRUE /FALSE

  SIDE EFFECTS

===========================================================================*/
ds_3gpp_platform_config_e_type ds_3gpp_nv_manager_get_platform_config(void);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_switch_attach_apn_over_dun

DESCRIPTION
  This function reads the device platform config.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_switch_attach_apn_over_dun(void);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_switch_attach_apn_over_dun

DESCRIPTION
  This function gets the value of NV Switch Attach APN over DUN Nv item

PARAMETERS 
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - Switch Attach APN over DUN supported
  False - Switch Attach APN over DUN not supported
 
SIDE EFFECTS
  None.
 
===========================================================================*/
boolean ds_3gpp_nv_manager_get_switch_attach_apn_over_dun(void);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_DISABLE_MTU_REQUEST

DESCRIPTION
  This function is used to read disable mtu request value from nv item
  per subscription

  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_read_disable_mtu_request
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_DISABLE_MTU_REQUEST

DESCRIPTION
  This function is used to retrieve disable mtu request value per subscription

  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  boolean-disable mtu request

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_nv_manager_get_disable_mtu_request
(
   sys_modem_as_id_e_type    subs_id
);
/*===========================================================================
FUNCTION ds_3gpp_nv_manager_read_allow_single_ip_dun_call_dual_ip_pf

DESCRIPTION
  This function reads the NV item # 74047 which contains parameters for 
  allowing single IP DUN call when profile is Dual IP type.

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
..None.

SIDE EFFECTS
 .None
===========================================================================*/
void ds_3gpp_nv_manager_read_allow_single_ip_dun_call_dual_ip_pf
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_ppp_wait_timer

DESCRIPTION
  This function gets the timer value of ppp_up_wait_timer_val present in
  NV item single_ip_dun_call_in_dual_ip_profile for each subscription

PARAMETERS 
  subs_id subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  uint16 timer value in ms.
 
SIDE EFFECTS
  None.
 
===========================================================================*/
uint16 ds_3gpp_nv_manager_get_ppp_wait_timer
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_nv_manager_get_allow_single_IP_call_dualIP_pf

DESCRIPTION
  This function gets the NV value of ds_3gpp_allow_single_ip_dun_call_dual_ip_profile
  for each subscription

PARAMETERS 
  subs_id subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  Boolean Flag indicating whether allow_single_ip_dun_call_dual_ip_profile nv is enabled.
 
SIDE EFFECTS
  None.
 
===========================================================================*/
boolean ds_3gpp_nv_manager_get_allow_single_IP_call_dualIP_pf
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_iot_throttling

DESCRIPTION
  This function gets the iot throttling nv value for each subscription

PARAMETERS
  subs_id -  subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether global throttling  nv is enabled.

SIDE EFFECTS
===========================================================================*/
boolean ds_3gpp_nv_manager_get_iot_throttling
(
  sys_modem_as_id_e_type subs_id
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK  */

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_set_iot_throttling_timer

DESCRIPTION
  This function sets the iot throttling timer in nv cache for specified subscription

PARAMETERS
  subs_id         - Subscription id
  throttle_timer  - Throttle timer value in hours
DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether global throttling  nv is enabled.

SIDE EFFECTS
===========================================================================*/

boolean ds_3gpp_nv_manager_set_iot_throttling_timer
(
  sys_modem_as_id_e_type subs_id,
  uint8                  throttle_timer
);  

/*===========================================================================
FUNCTION ds_3gpp_nv_manager_get_iot_throttling_timer

DESCRIPTION
  This function gets the iot throttling timer in nv cache for 
  specified subscription

PARAMETERS
  subs_id  -  Subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Value of the timer.

SIDE EFFECTS
===========================================================================*/

uint8 ds_3gpp_nv_manager_get_iot_throttling_timer
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_EFNAS_FAILURE_DEF_RETRY_TIMER 
 
DESCRIPTION
  This Function reads efnas failure default retry timer

PARAMETERS
  subs_id: The subscription for which the timer is being read

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_read_efnas_failure_def_retry_timer 
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_EFNAS_READ_FAILURE_DEF_RETRY_TIMER

DESCRIPTION
  This Function reads efnas_read_failure_def_retry_timer

PARAMETERS
  subs_id: The subscription for which the timer is being read
  mode:    The system mode.

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_nv_manager_get_efnas_read_failure_def_retry_timer
(
   sys_modem_as_id_e_type   subs_id,
   sys_sys_mode_e_type      mode
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_MODEM_DEF_PROFILE_SUPPORT
 
DESCRIPTION
  This Function reads the NV which tells if modem_def_profile
  creation is supported or not

PARAMETERS
  subs_id: The subscription for which the NV is being read.

DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_read_modem_def_profile_support
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_MODEM_DEF_PROFILE_SUPPORT

DESCRIPTION
  This Function gets the NV support for modem_def_profile creation

PARAMETERS
  subs_id: The subscription for which the NV is being get

DEPENDENCIES
  None.

RETURN VALUE
  TRUE : if NV is turned on
  FALSE : otherwise.
 
SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_modem_def_profile_support
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_NW_OVERRIDE_ENHANCEMENT_FROM_NV

DESCRIPTION
  This function reads the NV value to determine whether network override
  enhancement is enabled or not

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_nw_override_enhancement_from_nv
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_NW_OVERRIDE_ENHANCEMENT_NV

DESCRIPTION
  This function is used to get the network override enhancement NV value 

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - NV set to 1
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_nw_override_enhancement_nv
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_ATTACH_WITH_NW_OVERRIDE_NV

DESCRIPTION
  This function reads the NV value to determine whether attach with network
  override enhancement is enabled or not

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_attach_with_nw_override_nv
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_ATTACH_WITH_NW_OVERRIDE_NV

DESCRIPTION
  This function is used to get the attach with network override NV value 

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - NV set to 1
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_attach_with_nw_override_nv
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_PSDATAOFF_FEATURE_STATUS

DESCRIPTION
  This function is used to update the PS data off feature enable/disable
  status.

PARAMETERS
  subs_id:        subscription
  feature_status: boolean indicating whether the feature is on or off
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_set_psdataoff_feature_status
(
  sys_modem_as_id_e_type subs_id,
  boolean                feature_status
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_PSDATAOFF_FEATURE_STATUS

DESCRIPTION
  This function is used to get the PS Data Off Feature Status NV value 

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - PS data Off feature is enabled
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_psdataoff_feature_status
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_RAN_BIT_RATE_CONFIG

DESCRIPTION
  This function is used to read the NV value to determine the configuration
  for RAN bit rate support.
  The valid configurations are:
  rec_bit_data_rate_support   query_bit_data_rate_support
      FALSE                       FALSE
      TRUE                        TRUE
      TRUE                        FALSE
 
PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - NV set to 1
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_nv_manager_read_ran_bit_rate_config
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_RAN_BIT_RATE_CONFIG

DESCRIPTION
  This function is used to get the RAN bit rate configuration.
 
PARAMETERS
  subs_id  subscription 
  *config  populate the configuration
    
DEPENDENCIES
  None.

RETURN VALUE
  ds_3gpp_ran_bit_data_rate_support_config

SIDE EFFECTS
  None
===========================================================================*/
errno_enum_type ds_3gpp_nv_manager_get_ran_bit_rate_config 
(
  sys_modem_as_id_e_type subs_id,
  ds_3gpp_ran_bit_data_rate_support_config *config
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_READ_W2L_REDIR_CONFIG_NV

DESCRIPTION
  This function reads the values of NV parameters about WCDMA to LTE redirection
  configuration

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None

RETURN VALUE  
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_nv_manager_read_w2l_redir_config_nv
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_W2L_REDIR_CONFIG_NV

DESCRIPTION
  This function is used to get the WCDMA to LTE redirection configuration

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - NV set to 1
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
ds3gtputmgr_w2l_redir_config_s ds_3gpp_nv_manager_get_w2l_redir_config_nv
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_IGNORE_PREC_CONF_ON_SAME_BEARER

DESCRIPTION
  This function reads the ignore precedence conflict NV that determines
  whether to ignore the precedence conflict on same bearer

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS 
  None. 

===========================================================================*/
void ds_3gpp_nv_manager_read_ignore_prec_conf_on_same_bearer
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_IGNORE_PREC_CONF_ON_SAME_BEARER

DESCRIPTION
  This function is used to get the ignore precedence conflict on same bearer
  NV value.

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - ignore precedence conflict on same bearer
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_ignore_prec_conf_on_same_bearer
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_ATTACH_PENDING_ENABLED

DESCRIPTION
  This function reads attach pending is enabled/disabled
 
PARAMETERS
  subs_id -  subscription id

DEPENDENCIES
  None.

RETURN VALUE  

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_read_attach_pending_enabled
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_ATTACH_PENDING_ENABLED

DESCRIPTION
  This function is used to get the attach pending(enabled/disabled)

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE- enabled
  FALSE - disabled

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_attach_pending_enabled
(
   sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_GET_TEARDOWN_UNUSED_PDN_AFTER_IRAT_NV

DESCRIPTION
  This function is used to get the teardown_unused_pdn_after_irat_bmask NV value 

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - teardown_unused_pdn_after_irat_bmask feature is enabled
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_get_teardown_unused_pdn_after_irat_nv
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_TEARDOWN_UNUSED_PDN_AFTER_IRAT_NV

DESCRIPTION
  This function reads the  NV that determines
  whether the feature is on or off

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS 
  None. 

===========================================================================*/
void ds_3gpp_nv_manager_read_teardown_unused_pdn_after_irat_nv
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_NW_OVERRIDE_CLEANUP_NV

DESCRIPTION
  This function reads the network override cleanup NV for each
  subscription
  The NV enable/disable NW override cleanup when bearer is deactivated by NW.

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE 
  None 

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_nv_manager_read_nw_override_cleanup_nv
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_NW_OVERRIDE_CLEANUP

DESCRIPTION
  This function is used to get the network override cleanup NV.
  The NV enable/disable NW override cleanup when bearer is deactivated by NW.

  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE   -- default
  FALSE  -- if NW override cleanup NV is set as FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_nv_manager_get_nw_override_cleanup
(
  sys_modem_as_id_e_type subs_id
);

#endif /* FEATURE_DATA_3GPP */
#endif /* DS3GPPNVMANAGER_H */
