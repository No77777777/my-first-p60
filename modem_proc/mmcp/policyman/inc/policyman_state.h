#ifndef _POLICYMAN_STATE_H_
#define _POLICYMAN_STATE_H_

/**
  @file policyman_state.h

  @brief
*/

/*
    Copyright (c) 2013-2018 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_state.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/
#include "lte_rrc_ext_msg.h"

#include "policyman_i.h"
#include "policyman_call_events.h"
#include "policyman_diag.h"
#include "policyman_cfgitem.h"
#include "policyman_network_events.h"
#include "policyman_phone_events.h"
#include "policyman_rf.h"
#include "policyman_serving_system.h"
#include "policyman_subs.h"
#include "policyman_uim.h"

#define NO_MCC 1665              // 1665 is 0xFFF in BCD encoding and considered invalid MCC

typedef struct
{
  policyman_subs_info_t *pSubsInfo;
} policyman_device_info_t;

typedef struct
{
  sys_modem_as_id_e_type    asubs_id;
  sys_mcc_type              current_mcc;
  boolean                   imsiVerified;
  boolean                   simlockIsEnabled;
  lte_rrc_embms_plus_xrat_mask_t embmsRatMask;
  boolean                   embmsIsActive;
  policyman_ss_info_t      *pSsInfo;
  policyman_cmph_info_t    *pCmPhInfo;
  policyman_uim_info_t     *pUimInfo;
  policyman_cmcall_info_t  *pCmCallInfo;
  policyman_network_info_t *pNetworkInfo;
} policyman_subs_state_t;

typedef struct
{
  sys_mcc_type         mcc;
  size_t               confidenceLevel;
  size_t               status;
  boolean              allow_1x_without_subs;
} policyman_global_info_t;

struct policyman_state_t
{
  policyman_device_info_t     *pDeviceInfo;
  policyman_subs_state_t      *pSubsState[POLICYMAN_NUM_SUBS];
  policyman_global_info_t     *pGlobalInfo;
  policyman_cmcall_history_t  *pCmcallHistory;
  policyman_cmss_history_t    *pCmssHistory;
  policyman_cmph_history_t    *pCmphHistory;
  policyman_notify_history_t  *pNotifyHistory;
};


/*=============================================================================
  General APIs
=============================================================================*/


/*-------- policyman_state_init --------*/
/**
@brief

@param

@return

*/
void policyman_state_init(
  void
);


/*-------- policyman_state_static_init --------*/
/**
@brief

@param

@return

*/
void policyman_state_static_init(
  void
);


/*-------- policyman_state_deinit --------*/
/**
@brief

@param

@return

*/
void policyman_state_deinit(
  void
);


/*-------- policyman_state_static_deinit --------*/
/**
@brief

@param

@return

*/
void policyman_state_static_deinit(
  void
);


/*-------- policyman_state_enter_crit_section --------*/
/**
@brief

@param

@return

*/
void policyman_state_enter_crit_section(
  void
);


/*-------- policyman_state_leave_crit_section --------*/
/**
@brief

@param

@return

*/
void policyman_state_leave_crit_section(
  void
);


/*-------- policyman_state_cleanup_after_policy_run --------*/
void policyman_state_cleanup_after_policy_run(
  policyman_state_t      *pState
);


/*-------- policyman_state_clone --------*/
/**
@brief

@param

@return

*/
policyman_state_t * policyman_state_clone(
  void
);


/*-------- policyman_state_delete_clone --------*/
/**
@brief

@param

@return

*/
void policyman_state_delete_clone(
  policyman_state_t *pState
);


/*-------- policyman_state_get_state --------*/
/**
@brief  Get a pointer to the policyman state.  This will in general only be
        called by unit tests - most use of policyman_state will be through
        parameters passed to functions.

@param

@return

*/
policyman_state_t * policyman_state_get_state(
  void
);


/*-------- policyman_state_get_subs_state --------*/
/**
@brief

@param

@return

*/
policyman_subs_state_t * policyman_state_get_subs_state(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
);

/*-------- policyman_state_get_device_info --------*/
policyman_device_info_t * policyman_state_get_device_info(
  policyman_state_t *pState
);


/*-------- policyman_state_get_subs_info --------*/
/**
@brief  Get a pointer to the policyman subs state.

@param

@return

*/
policyman_subs_info_t * policyman_state_get_subs_info(
  policyman_state_t *pState
);



/*-------- policyman_state_handle_update --------*/
/**
@brief  The policyman state has been udpated; policies should be processed to
        determine whether there is a policy update.

@return
  Nothing.
*/
void policyman_state_handle_update(
  policyman_sandbox_t *pSandbox
);

/*=============================================================================
  RF related APIs
=============================================================================*/


/*=============================================================================
  Serving System related APIs
=============================================================================*/

/*-------- policyman_state_get_serving_system_info --------*/
policyman_ss_info_t *policyman_state_get_serving_system_info(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId
);


/*=============================================================================
  CM Phone Event related APIs
=============================================================================*/

/*===========================================================================
  FUNCTION POLICYMAN_STATE_UPDATE_PH_OPRT_MODE()

  DESCRIPTION
    Update operating mode into PM Phone state

  PARAMETERS
    oprt_mode : operating mode to be set
===========================================================================*/
void policyman_state_update_ph_oprt_mode(
  policyman_sandbox_t    *pSandbox,
  sys_oprt_mode_e_type    oprt_mode
);


/*===========================================================================
  FUNCTION POLICYMAN_STATE_CMPH_NV_INIT()

  DESCRIPTION
    Read NVs related to CM PH state for a subscription

  PARAMETERS
    asubs_id : subscription to read NVs for
===========================================================================*/
void policyman_state_cmph_nv_init(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
);


/*-------- policyman_state_get_cm_ph_state --------*/
policyman_cmph_info_t * policyman_state_get_cm_ph_state(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
);

/*=============================================================================
  UIM related APIs
=============================================================================*/

/*-------- policyman_state_get_uim_info --------*/
policyman_uim_info_t * policyman_state_get_uim_info(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
);

/*=============================================================================
  Call related APIs
=============================================================================*/

/*-------- policyman_state_get_call_info --------*/
policyman_cmcall_info_t * policyman_state_get_call_info(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);


/*=============================================================================
  Hardware related APIs
=============================================================================*/

/*-------- policyman_state_get_imei --------*/
char const * policyman_state_get_imei(
  void
);


/*-------- policyman_state_get_raw_imei --------*/
byte * policyman_state_get_raw_imei(
  void
);


/*=============================================================================
  Network Scan related APIs
=============================================================================*/

/*-------- policyman_state_update_network_scan_result --------*/
void policyman_state_update_network_scan_result(
  policyman_sandbox_t *pSandbox,
  cm_ph_info_s_type *pInfo
);

/*-------- policyman_state_get_network_info --------*/
policyman_network_info_t * policyman_state_get_network_info(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);

/*=============================================================================
  HLOS MCC related APIs
=============================================================================*/

/*-------- policyman_state_update_hlos_mcc --------*/
void policyman_state_update_hlos_mcc(
  policyman_state_t *pState,
  sys_mcc_type       hlos_mcc,
  size_t             confidenceLevel,
  size_t             status
);

/*-------- policyman_state_clear_hlos_mcc --------*/
void policyman_state_clear_hlos_mcc(
  policyman_state_t *pState
);

/*-------- policyman_state_get_hlos_mcc --------*/
sys_mcc_type policyman_state_get_hlos_mcc(
  policyman_state_t *pState
);

/*-------- policyman_state_set_subs_mcc --------*/
void policyman_state_set_subs_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  sys_mcc_type            mcc
);

/*-------- policyman_state_update_subs_mcc --------*/
void policyman_state_update_subs_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  sys_mcc_type            mcc
);

/*-------- policyman_state_get_subs_mcc --------*/
sys_mcc_type policyman_state_get_subs_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_state_get_imsi_verified --------*/
boolean policyman_state_get_imsi_verified(
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_state_update_imsi_verified --------*/
void policyman_state_update_imsi_verified(
  sys_modem_as_id_e_type  subsId,
  boolean                 verified
);

/*-------- policyman_state_set_imsi_verified --------*/
void policyman_state_set_imsi_verified(
  sys_modem_as_id_e_type  subsId,
  boolean                 verified
);

/*=============================================================================
  allow_1x_without_subs related APIs
=============================================================================*/

/*-------- policyman_state_get_1x_allowed_without_subs --------*/
boolean policyman_state_get_1x_allowed_without_subs(
  policyman_state_t *pState
);

/*=============================================================================
  Internal functions exported to unit test code.
=============================================================================*/

/*-------- policyman_state_ss_init --------*/
void policyman_state_ss_init(
  policyman_state_t *pState
);

/*-------- policyman_state_uim_init --------*/
boolean policyman_state_uim_init(
  policyman_state_t *pState
);

/*-------- policyman_state_cmph_init --------*/
boolean policyman_state_cmph_init(
  policyman_state_t *pState
);

/*-------- policyman_state_subs_init --------*/
boolean policyman_state_subs_init(
  policyman_state_t *pState
);

/*-------- policyman_state_cmcall_init --------*/
boolean policyman_state_cmcall_init(
  policyman_state_t *pState
);

/*-------- policyman_state_network_init --------*/
boolean policyman_state_network_init(
  policyman_state_t *pState
);


/*-------- policyman_state_fill_diag_subs_info --------*/
void policyman_state_fill_diag_subs_info(
  sys_modem_as_id_e_type  asubs_id,
  pm_dump_subs_info_t    *dump_subs_info
);

/*-------- policyman_state_set_simlock_enabled --------*/
boolean policyman_state_set_simlock_enabled(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  boolean                 enableSimlock
);

/*-------- policyman_state_simlock_is_enabled --------*/
boolean policyman_state_simlock_is_enabled(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);
/*-------- policyman_state_update_embms_status --------*/
void policyman_state_update_embms_status(
  msgr_hdr_s             *pMsg,
  void                   *pContext,
 sys_modem_as_id_e_type   subsId
);

/*-------- policyman_state_get_embms_rat_mask --------*/
lte_rrc_embms_plus_xrat_mask_t policyman_state_get_embms_rat_mask(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_state_embms_is_enabled --------*/
boolean policyman_state_embms_is_enabled(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_state_embms_rat_is_restricted --------*/
boolean policyman_state_embms_rat_is_restricted(
  policyman_state_t     *pState,
  sys_sys_mode_e_type    ratToCheck,
  sys_modem_as_id_e_type ddsSub
);

/*-------- policyman_condition_embms_is_active_new --------*/
mre_status_t policyman_condition_embms_is_active_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_condition_t          **ppCondition
);

/*-------- policyman_state_update_time_in_lpm --------*/
void policyman_state_update_time_in_lpm(
  policyman_sandbox_t  *pSandbox,
  uint32                timeInLPM
);

/*-------- policyman_mdump_state --------*/
void policyman_state_mdump(
  void
);
#endif /* _POLICYMAN_STATE_H_ */
