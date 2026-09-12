/**
  @file policyman_policies.c

  @brief 1. enumerate and manage loaded policies
            2. Manage suspend resume framework
*/

/*
    Copyright (c) 2015-2018,2020,2022 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_policies.c#2 $
  $DateTime: 2022/02/15 22:29:00 $
  $Author: pwbldsvc $
*/

#include "mre.h"
#include "mre_efs.h"
#include "mre_set.h"
#include "mre_rules.h"
#include "mre_util.h"
#include "mre_engine.h"

#include "policyman_call_events.h"
#include "policyman_call_mode.h"
#include "policyman_cfgitem.h"
#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_efs.h"
#include "policyman_frequency_list.h"
#include "policyman_fullrat_config.h"
#include "policyman_ims_config.h"
#include "policyman_lte_i.h"
#include "policyman_network_events.h"
#include "policyman_plmn.h"
#include "policyman_policy.h"
#include "policyman_policies.h"
#include "policyman_rat_capability.h"
#include "policyman_rat_order.h"
#include "policyman_scan_optimization.h"
#include "policyman_service_domain.h"
#include "policyman_state.h"
#include "policyman_subs.h"
#include "policyman_svc_mode.h"
#include "policyman_task.h"
#include "policyman_tech.h"
#include "policyman_timer.h"
#include "policyman_ue_mode.h"
#include "policyman_uim.h"
#include "policyman_util.h"
#include "policyman_voice_domain_pref.h"
#include "policyman_volte.h"
#include "policyman_xml.h"

#include <stringl/stringl.h>
#include <qurt_timer.h>

/*=============================================================================
  Mappings between condition tags and the functions to create a new condition
  from the XML document.
=============================================================================*/

STATIC mre_condition_map_element_t const  policyman_condition_map[] =
{
  /*---------------------------------------------------------------------------
    Serving system conditions
  ---------------------------------------------------------------------------*/

  {"service_status_in",       policyman_condition_service_status_new,             PM_COND_ID_SERVICE_STATUS_IN },
  {"srv_domain_has",          policyman_condition_service_domain_has_new,         PM_COND_ID_SRV_DOMAIN_HAS },
  {"serving_mcc_in",          policyman_condition_serving_mcc_new,                PM_COND_ID_SERVING_MCC_IN },
  {"serving_plmn_in",         policyman_condition_serving_plmn_new,               PM_COND_ID_SERVING_PLMN_IN },
  {"serving_rat_in",          policyman_condition_serving_rat_new,                PM_COND_ID_SERVING_RAT_IN },
  {"serving_band_in",         policyman_condition_serving_rf_band_new,            PM_COND_ID_SERVING_BAND_IN },
  {"reg_reject_cause_in",     policyman_condition_reg_reject_new,                 PM_COND_ID_REG_REJECT_CAUSE_IN },
  {"oos_scan_cnt",            policyman_condition_oos_scan_cnt_new,               PM_COND_ID_OOS_SCAN_CNT },
  {"location_mcc_in",         policyman_condition_location_mcc_new,               PM_COND_ID_LOCATION_MCC_IN },
  {"have_service",            policyman_condition_have_service_new,               PM_COND_ID_HAVE_SERVICE },
  {"have_location",           policyman_condition_have_location_new,              PM_COND_ID_HAVE_LOCATION },
  {"have_serving_system",     policyman_condition_have_serving_system_new,        PM_COND_ID_HAVE_SERVING_SYSTEM },
  {"subphone_cap_has",        policyman_condition_subphone_cap_has_new,           PM_COND_ID_SUBPHONE_CAP_HAS },
  {"current_mcc_in",          policyman_condition_current_mcc_in_new,             PM_COND_ID_CURRENT_MCC_IN },

  /*---------------------------------------------------------------------------
    UIM conditions
  ---------------------------------------------------------------------------*/
  {"imsi_plmn_in",            policyman_condition_imsi_plmn_new,                  PM_COND_ID_IMSI_PLMN_IN },
  {"imsi_mcc_in",             policyman_condition_imsi_mcc_new,                   PM_COND_ID_IMSI_MCC_IN },
  {"sim_type",                policyman_condition_sim_type_new,                   PM_COND_ID_SIM_TYPE },
  {"imei",                    policyman_condition_imei_new,                       PM_COND_ID_IMEI },
  {"sim_refreshed",           policyman_condition_sim_refresh_new,                PM_COND_ID_SIM_REFRESHED },
  {"simlock_enabled",         policyman_condition_simlock_enabled_new,            PM_COND_ID_SIMLOCK_ENABLED },
  {"have_imsi",               policyman_condition_have_imsi_new,                  PM_COND_ID_HAVE_IMSI },

  /*---------------------------------------------------------------------------
    Current state conditions
  ---------------------------------------------------------------------------*/
  {"ue_mode_is",              policyman_condition_ue_mode_new,                    PM_COND_ID_UE_MODE_IS },
  {"svc_mode_is",             policyman_condition_svc_mode_new,                   PM_COND_ID_SVC_MODE_IS },
  {"num_subs",                policyman_condition_num_subs_new,                   PM_COND_ID_NUM_SUBS },
  {"enforce_full_rat",        policyman_condition_enforce_full_rat_new,           PM_COND_ID_ENFORCE_FULL_RAT },
  {"is_subs_dds",             policyman_condition_is_subs_dds_new,                PM_COND_ID_IS_SUBS_DDS },
  {"subs_is_active",          policyman_condition_subs_is_active_new,             PM_COND_ID_SUBS_IS_ACTIVE },

  /*---------------------------------------------------------------------------
    User preference conditions
  ---------------------------------------------------------------------------*/
  {"user_domain_pref",        policyman_condition_ph_domain_pref_new,             PM_COND_ID_USER_DOMAIN_PREF },
  {"user_mode_pref_contains", policyman_condition_ph_rat_pref_new,                PM_COND_ID_USER_MODE_PREF_CONTAINS },
  {"user_mcc_in",             policyman_condition_ph_user_mcc_new,                PM_COND_ID_USER_MCC_IN },
  {"network_selection_mode",  policyman_condition_ph_network_selection_mode_new,  PM_COND_ID_NETWORK_SELECTION_MODE },
  {"voice_domain_pref_is",    policyman_condition_voice_domain_pref_new,          PM_COND_ID_VOICE_DOMAIN_PREF_IS },
  {"call_mode_is",            policyman_condition_call_mode_new,                  PM_COND_ID_CALL_MODE_IS },
  {"service_domain_has",      policyman_condition_service_domain_has_new,         PM_COND_ID_SERVICE_DOMAIN_HAS },

  /*---------------------------------------------------------------------------
    Timer conditions
  ---------------------------------------------------------------------------*/
  {"timer_expired",           policyman_condition_timer_expired_new,              PM_COND_ID_TIMER_EXPIRED },
  {"timer_is_running",        policyman_condition_timer_is_running,               PM_COND_ID_TIMER_IS_RUNNING },

  /*---------------------------------------------------------------------------
    Phone Operating Mode conditions
  ---------------------------------------------------------------------------*/
  {"phone_operating_mode",    policyman_condition_ph_operating_mode_new,          PM_COND_ID_PHONE_OPERATING_MODE },
  {"time_in_lpm",             policyman_condition_time_in_lpm_new,                PM_COND_ID_TIME_IN_LPM },

  /*---------------------------------------------------------------------------
    Technology conditions
  ---------------------------------------------------------------------------*/
  {"tech_loaded",             policyman_condition_tech_loaded_new,                PM_COND_ID_TECH_LOADED },


  /*---------------------------------------------------------------------------
    Network Search conditions
  ---------------------------------------------------------------------------*/
  {"scan_results",            policyman_condition_scan_results_new,               PM_COND_ID_SCAN_RESULTS },
  {"network_scan_active",     policyman_condition_network_scan_active_new,        PM_COND_ID_NETWORK_SCAN_ACTIVE },

  /*---------------------------------------------------------------------------
    Special conditions
  ---------------------------------------------------------------------------*/
  {"volte_enabled",           policyman_condition_volte_enabled_new,              PM_COND_ID_VOLTE_ENABLED },
  {"embms_is_active",         policyman_condition_embms_is_active_new,            PM_COND_ID_EMBMS_IS_ACTIVE },

  {"fdn_status",              policyman_condition_fdn_status_new,                 PM_COND_ID_FDN_STATUS },
  {"fdn_has",                 policyman_condition_fdn_has_new,                    PM_COND_ID_FDN_HAS },

  /*---------------------------------------------------------------------------
    Terminator
  ---------------------------------------------------------------------------*/
  {NULL,                      NULL,                                               PM_COND_ID_UNDEF}
};

/*=============================================================================
  Mappings between action tags and the functions to create a new action
  from the XML document.
=============================================================================*/

STATIC mre_action_map_element_t const  policyman_action_map[] =
{
  /*  Configuration item actions
   */
  {"device_configuration",    policyman_device_config_action_new,         PM_ACT_ID_DEVICE_CONFIGURATION },
  {"device_configuration_if", policyman_device_config_if_action_new,      PM_ACT_ID_DEVICE_CONFIGURATION_IF },
  {"define_config",           policyman_device_config_define_new,         PM_ACT_ID_DEFINE_CONFIG },
  {"use_config",              policyman_device_config_use_new,            PM_ACT_ID_USE_CONFIG },
  {"use_config_if",           policyman_device_config_use_if_new,         PM_ACT_ID_USE_CONFIG_IF },
  {"rat_capability",          policyman_rat_capability_action_new,        PM_ACT_ID_RAT_CAPABILITY },
  {"rat_capability_if",       policyman_rat_capability_if_action_new,     PM_ACT_ID_RAT_CAPABILITY_IF },
  {"ue_mode",                 policyman_ue_mode_action_new,               PM_ACT_ID_UE_MODE },
  {"ue_mode_if",              policyman_ue_mode_if_action_new,            PM_ACT_ID_UE_MODE_IF },
  {"rf_bands",                policyman_rf_bands_action_new,              PM_ACT_ID_RF_BANDS },
  {"rf_bands_if",             policyman_rf_bands_if_action_new,           PM_ACT_ID_RF_BANDS_IF },
  {"svc_mode",                policyman_svc_mode_action_new,              PM_ACT_ID_SVC_MODE },
  {"feature",                 policyman_device_config_feature_new,        PM_ACT_ID_FEATURE },
  {"voice_domain_pref",       policyman_voice_domain_pref_action_new,     PM_ACT_ID_VOICE_DOMAIN_PREF },
  {"rat_order",               policyman_rat_order_action_new,             PM_ACT_ID_RAT_ORDER },
  {"call_mode",               policyman_call_mode_action_new,             PM_ACT_ID_CALL_MODE },
  {"call_mode_if",            policyman_call_mode_if_action_new,          PM_ACT_ID_CALL_MODE_IF },
  {"volte",                   policyman_volte_action_new,                 PM_ACT_ID_VOLTE },
  {"scan_optimization",       policyman_scan_opt_action_new,              PM_ACT_ID_SCAN_OPTIMIZATION },
  {"freq_list",               policyman_freq_list_action_new,             PM_ACT_ID_FREQ_LIST },
  {"freq_list_if",            policyman_freq_list_if_action_new,          PM_ACT_ID_FREQ_LIST_IF },
  {"select_config",           policyman_device_config_select_config_new,  PM_ACT_ID_SELECT_CONFIG },
  {"ca_band_combos",          policyman_ca_combo_action_new,              PM_ACT_ID_CA_BAND_COMBOS },
  {"service_domain",          policyman_service_domain_create_new_action, PM_ACT_ID_SERVICE_DOMAIN },
  {"lte_feature",             policyman_lte_feature_action_new,           PM_ACT_ID_LTE_FEATURE_MASK },

  /*  Actions to define named objects
   */
  {"rf_band_list",            policyman_rf_band_list_action_new,          PM_ACT_ID_RF_BAND_LIST },
  {"define_fullrat_config",   policyman_fullrat_config_new,               PM_ACT_ID_DEFINE_FULLRAT_CONFIG },
  {"fullrat_enter",           policyman_fullrat_enter_new,                PM_ACT_ID_FULLRAT_ENTER },

  /*---------------------------------------------------------------------------
    Timer actions
  ---------------------------------------------------------------------------*/
  {"define_timer",            policyman_named_timer_action_new,           PM_ACT_ID_DEFINE_TIMER },
  {"expired_timer_handled",   policyman_expired_timer_handled_new,        PM_ACT_ID_EXPIRED_TIMER_HANDLED },
  {"timer_start",             policyman_timer_start_new,                  PM_ACT_ID_TIMER_START },
  {"timer_stop",              policyman_timer_stop_new,                   PM_ACT_ID_TIMER_STOP },

  /*  Actions that affect state of system
   */
  {"exclude_tech",            policyman_tech_exclude_new,                 PM_ACT_ID_EXCLUDE_TECH },

  /* Network Scan actions
   */
  {"network_scan",            policyman_network_scan_action_new,          PM_ACT_ID_NETWORK_SCAN },

  /* IMS Actions
  */
  {"ims_config",              policyman_ims_config_action_new,            PM_ACT_ID_IMS_CONFIG },

  /*---------------------------------------------------------------------------
    Terminator
  ---------------------------------------------------------------------------*/
  {NULL,                      NULL,                                       PM_ACT_ID_UNDEF}
};

STATIC  mre_set_t             *g_pPolicySet;
STATIC  mre_set_t             *g_pGlobalPolicySet;
STATIC  mre_set_t             *g_pSubsPolicySet;
STATIC  mre_set_t             *g_pPostPolicySet;
STATIC  boolean                g_policy_is_initialized;
STATIC  mre_namespace_info_t  *g_pNamespaceInfo;
STATIC  policyman_crit_sect_t  g_suspendResumeCS;
STATIC  mre_set_t             *g_pSuspendClientSet;
STATIC  policyman_crit_sect_t  policyman_policies_crit_sect;
STATIC  policyman_crit_sect_t  policyman_namespace_crit_sect;

/*-----------------------------------------------------------------------------
  Policyman suspend/resume framework
-----------------------------------------------------------------------------*/

/*-------- compare_clients --------*/
STATIC boolean compare_clients(
  void const *pElem1,
  void const *pElem2
)
{
  policyman_client_t  client1 = *((policyman_client_t *) pElem1);
  policyman_client_t  client2 = *((policyman_client_t *) pElem2);

  return client1 == client2;
}

/*-------- policyman_init_suspend_resume_framework --------*/
STATIC void policyman_init_suspend_resume_framework(
  void
)
{
  policyman_init_crit_sect(&g_suspendResumeCS);

  policyman_enter_crit_sect(&g_suspendResumeCS);
  g_pSuspendClientSet = mre_set_new(  sizeof(policyman_client_t),
                                      5,
                                      MRE_SET_NO_MAX_SIZE,
                                      compare_clients,
                                      NULL );
  policyman_leave_crit_sect(&g_suspendResumeCS);
}

/*-------- policyman_deinit_suspend_resume_framework --------*/
STATIC void policyman_deinit_suspend_resume_framework(
  void
)
{
  policyman_deinit_crit_sect(&g_suspendResumeCS);
  MRE_RELEASE_IF(g_pSuspendClientSet);
}

/*-------- policyman_get_suspend_client_set --------*/
mre_set_t* policyman_get_suspend_client_set(
  void
)
{
  return g_pSuspendClientSet;
}

/*-------- policyman_is_suspended --------*/
boolean policyman_is_suspended(
  policyman_sandbox_t *pSandbox
)
{
  return !mre_set_is_empty(pSandbox->pSuspendClientSet);
}

/*-------- try_suspend --------*/
STATIC boolean  try_suspend(
  policyman_client_t  client
)
{
  policyman_dbupdate_state     state;
  boolean                      suspended;

  policyman_enter_crit_sect(&g_suspendResumeCS);

  state = policyman_dbupdate_state_get();
  suspended =     (POLICYMAN_DBUPDATE_STATE_SUSPENDED == state)
              ||  policyman_dbupdate_try_enter_suspend_state() ;

  if (suspended)
  {
    mre_set_add(g_pSuspendClientSet, &client);
  }

  policyman_leave_crit_sect(&g_suspendResumeCS);

  return suspended;
}

/*-------- policyman_suspend_by_client --------*/
void policyman_suspend_by_client(
  policyman_client_t  client
)
{
  size_t  numClients;

  // return if in test mode
  if (policyman_any_test_mode_is_enabled())
  {
    POLICYMAN_MSG_HIGH_0("policyman_suspend_by_client.. test mode is enabled ..not suspending");
    return;
  }

  /* Block the client if Policyman DB update is in progress
     Wait till DB update completes and try_suspend returns true
  */
  {
    boolean suspended = FALSE;

    while (!suspended)
    {
      suspended = try_suspend(client);
      if (!suspended)
      {
        qurt_timer_sleep(2 * QURT_TIMER_MIN_DURATION);  // 200 us
      }
    }

  numClients = mre_set_num_elems(g_pSuspendClientSet);

  POLICYMAN_MSG_HIGH_2( "policyman suspended by client %d; %d clients have suspended",
                        client,
                        numClients );
  }
}

/*------- policyman_execute_process_resume --------*/
STATIC void policyman_execute_process_resume(
  policyman_cmd_t *pCmd
)
{
  policyman_state_handle_update(pCmd->pSandbox);
}

/*-------- policyman_resume_internal --------*/
size_t policyman_resume_internal(
  policyman_client_t  client
)
{
  size_t           numClients;

  policyman_enter_crit_sect(&g_suspendResumeCS);
  mre_set_remove(g_pSuspendClientSet, &client);
  numClients = mre_set_num_elems(g_pSuspendClientSet);
  policyman_leave_crit_sect(&g_suspendResumeCS);

  if (numClients > 0)
  {
    POLICYMAN_MSG_HIGH_2( "policyman resumed by client %d; still suspended by %d clients",
                          client,
                          numClients );
  }
  else
  {
    POLICYMAN_MSG_HIGH_1("policyman resumed by client %d", client);
  }

  return numClients;
}

/*-------- policyman_resume_by_client --------*/
void policyman_resume_by_client(
  policyman_client_t  client
)
{
  size_t           numClients;
  policyman_cmd_t *pCmd = NULL;

  if (policyman_any_test_mode_is_enabled())
  {
    POLICYMAN_MSG_HIGH_0("policyman_resume_by_client : test mode is enabled");
    return;
  }

  numClients = policyman_resume_internal(client);

  if (numClients == 0)
  {
    pCmd = POLICYMAN_CMD_NEW( policyman_cmd_t,
                              policyman_execute_process_resume,
                              NULL,
                              NULL );

    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    ref_cnt_obj_release(pCmd);

    policyman_dbupdate_enter_idle_state( );
  }
}

/*-------- policyman_suspend --------*/
void policyman_suspend(
  void
)
{
  policyman_suspend_by_client(POLICYMAN_CLIENT_CM);
}

/*-------- policyman_resume--------*/
void policyman_resume(
  void
)
{
  policyman_resume_by_client(POLICYMAN_CLIENT_CM);
}


/*-------- policyman_namespace_info_dtor --------*/
void policyman_namespace_info_dtor(
  void *pObj
)
{
  mre_namespace_info_t *pNamespaceInfo = (mre_namespace_info_t *)pObj;
  size_t                subs;

  for (subs = 0; subs < ARR_SIZE(pNamespaceInfo->perSubsNamedObjs); ++subs)
  {
    MRE_RELEASE_IF(pNamespaceInfo->perSubsNamedObjs[subs]);
  }

  MRE_RELEASE_IF(pNamespaceInfo->pGlobalNamedObjs);

  mre_mem_free(pNamespaceInfo);
}


/*-------- policyman_namespace_info_deinit --------*/
STATIC void policyman_namespace_info_deinit(
  void
)
{
  policyman_enter_crit_sect(&policyman_namespace_crit_sect);

  MRE_RELEASE_IF(g_pNamespaceInfo);

  policyman_leave_crit_sect(&policyman_namespace_crit_sect);
  policyman_deinit_crit_sect(&policyman_namespace_crit_sect);
}


/*-------- policyman_namespace_clone --------*/
mre_namespace_info_t * policyman_namespace_clone(
  mre_namespace_info_t *pNamespaceInfo
)
{
  mre_namespace_info_t *pNamespaceInfoClone;
  size_t                subs;

  pNamespaceInfoClone = (mre_namespace_info_t *) mre_mem_alloc(sizeof(mre_namespace_info_t));

  policyman_enter_crit_sect(&policyman_namespace_crit_sect);

  *pNamespaceInfoClone = *pNamespaceInfo;

  ref_cnt_obj_init(pNamespaceInfoClone, policyman_namespace_info_dtor);

  for (subs = 0; subs < MAX_AS_IDS; ++subs)
  {
    pNamespaceInfoClone->perSubsNamedObjs[subs] = mre_named_objects_clone(pNamespaceInfo->perSubsNamedObjs[subs]);
  }

  pNamespaceInfoClone->pGlobalNamedObjs = mre_named_objects_clone(pNamespaceInfo->pGlobalNamedObjs);

  policyman_leave_crit_sect(&policyman_namespace_crit_sect);

  return pNamespaceInfoClone;
}

/*-------- policyman_enter_namespace_crit_sect --------*/
void policyman_enter_namespace_crit_sect(
  void
)
{
  policyman_enter_crit_sect(&policyman_namespace_crit_sect);
}


/*-------- policyman_leave_namespace_crit_sect --------*/
void policyman_leave_namespace_crit_sect(
  void
)
{
  policyman_leave_crit_sect(&policyman_namespace_crit_sect);
}

/*-------- policyman_get_namespace_info --------*/
mre_namespace_info_t * policyman_get_namespace_info(
  void
)
{
  return g_pNamespaceInfo;
}


/*-------- policyman_namespace_info_init --------*/
void policyman_namespace_info_init(
  void
)
{
  policyman_init_crit_sect(&policyman_namespace_crit_sect);

  g_pNamespaceInfo = (mre_namespace_info_t *)mre_mem_alloc(sizeof(mre_namespace_info_t));

  ref_cnt_obj_init(g_pNamespaceInfo, policyman_namespace_info_dtor);

  g_pNamespaceInfo->defaultNS         = MRE_NAMESPACE_SUBS;
  g_pNamespaceInfo->pfnGetPolicySubs  = policyman_policy_get_subs;
}


/*-------- policyman_namespace_info_per_subs_deinit --------*/
STATIC void policyman_namespace_info_per_subs_deinit(
  sys_modem_as_id_e_type subsId
)
{
  policyman_enter_crit_sect(&policyman_namespace_crit_sect);

  MRE_RELEASE_IF(g_pNamespaceInfo->perSubsNamedObjs[subsId]);

  policyman_leave_crit_sect(&policyman_namespace_crit_sect);
}


typedef struct
{
  mre_namespace_info_t *pNamespaceInfo;
  mre_set_t            *pCloneSet;
} clone_policy_ctx;


/*-------- policyman_policy_clone --------*/
policyman_policy_t * policyman_policy_clone(
  policyman_policy_t    *pPolicy,
  mre_namespace_info_t  *pNamespaceInfo
)
{
  policyman_policy_t *pPolicyClone;

  pPolicyClone = (policyman_policy_t *)mre_policy_clone((mre_policy_t *)pPolicy, pNamespaceInfo);

  pPolicyClone->error_cause = pPolicy->error_cause;

  return pPolicyClone;
}


/*-------- policyman_clone_policy_set --------*/
STATIC boolean policyman_clone_policy_set(
  void *pElem,
  void *pCtx
)
{
  policyman_policy_t  **ppPolicy         = (policyman_policy_t **)pElem;
  clone_policy_ctx     *pClonePolicyCtx = (clone_policy_ctx *)pCtx;
  mre_policy_t         *pPolicyClone;

  pPolicyClone = (mre_policy_t *)policyman_policy_clone(*ppPolicy, pClonePolicyCtx->pNamespaceInfo);

  mre_set_add(pClonePolicyCtx->pCloneSet, &pPolicyClone);

  MRE_RELEASE_IF(pPolicyClone);

  return TRUE;
}


/*-------- policyman_clone_global_policy_set --------*/
mre_set_t * policyman_clone_global_policy_set(
  mre_namespace_info_t *pNamespaceInfo
)
{
  mre_set_t         *pPolicySet;
  mre_set_t         *pGlobalPolicySet;
  mre_set_t         *pSubsPolicySet;
  mre_set_t         *pPostPolicySet;
  clone_policy_ctx   clonePolicyCtx;

  pPolicySet        = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
  pGlobalPolicySet  = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
  pSubsPolicySet    = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
  pPostPolicySet    = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);

  clonePolicyCtx.pNamespaceInfo = pNamespaceInfo;
  clonePolicyCtx.pCloneSet      = pGlobalPolicySet;

  policyman_enter_crit_sect(&policyman_policies_crit_sect);

  mre_set_iterate(g_pGlobalPolicySet, policyman_clone_policy_set, &clonePolicyCtx);

  clonePolicyCtx.pCloneSet = pSubsPolicySet;

  mre_set_iterate(g_pSubsPolicySet, policyman_clone_policy_set, &clonePolicyCtx);

  clonePolicyCtx.pCloneSet = pPostPolicySet;

  mre_set_iterate(g_pPostPolicySet, policyman_clone_policy_set, &clonePolicyCtx);

  policyman_leave_crit_sect(&policyman_policies_crit_sect);

  mre_set_add(pPolicySet, &pGlobalPolicySet);
  mre_set_add(pPolicySet, &pSubsPolicySet);
  mre_set_add(pPolicySet, &pPostPolicySet);

  MRE_RELEASE_IF(pGlobalPolicySet);
  MRE_RELEASE_IF(pSubsPolicySet);
  MRE_RELEASE_IF(pPostPolicySet);

  return pPolicySet;
}


/*-------- policyman_get_global_policy_set --------*/
mre_set_t* policyman_get_global_policy_set(
  void
)
{
  return g_pPolicySet;
}

/*-------- policyman_get_policy --------*/
STATIC policyman_policy_t * policyman_get_policy(
  size_t n
)
{
  policyman_policy_t  **ppPolicy = NULL;
  size_t                globalPolicyCount;
  size_t                subsPolicyCount;
  size_t                postPolicyCount;

  globalPolicyCount = (NON_NULL(g_pGlobalPolicySet)) ? mre_set_num_elems(g_pGlobalPolicySet) : 0;
  subsPolicyCount   = (NON_NULL(g_pSubsPolicySet)) ? mre_set_num_elems(g_pSubsPolicySet)   : 0;
  postPolicyCount   = (NON_NULL(g_pPostPolicySet)) ? mre_set_num_elems(g_pPostPolicySet)   : 0;

  if (n < globalPolicyCount)
  {
    ppPolicy = (policyman_policy_t **) mre_set_elem_ptr(g_pGlobalPolicySet, n);
    goto Done;
  }

  n -= globalPolicyCount;
  if (n < subsPolicyCount)
  {
    ppPolicy = (policyman_policy_t **) mre_set_elem_ptr(g_pSubsPolicySet, n);
    goto Done;
  }

  n -= subsPolicyCount;
  if (n < postPolicyCount)
  {
    ppPolicy = (policyman_policy_t **) mre_set_elem_ptr(g_pPostPolicySet, n);
  }

Done:
  return (IS_NULL(ppPolicy)) ? NULL : *ppPolicy;
}

/*-------- policyman_global_policy_files --------*/
STATIC char const  *policyman_global_policy_files[] =
{
  POLICYMAN_DEVICE_CONFIG_XML_NAME,
};

/*-------- policyman_subs_policy_files --------*/
STATIC char const  *policyman_subs_policy_files[] =
{
  POLICYMAN_CARRIER_POLICY_XML_NAME,
  POLICYMAN_RESTRICTIONS_XML_NAME,
  POLICYMAN_SEGMENT_LOADING_XML_NAME,
};

STATIC char const  *policyman_post_policy_files[] =
{
  POLICYMAN_POST_POLICY_XML_NAME,
};

/*-------- policyman_policies_file_enum_init --------*/
void policyman_policies_file_enum_init(
  policyman_policy_enum_ctx *pCtx
)
{
  *pCtx = 0;
}

/*-------- policyman_policy_global_file_enum_next --------*/
boolean policyman_policy_global_file_enum_next(
  policyman_policy_enum_ctx *pCtx,
  char                      *pFileName,
  size_t                     nameLength
)
{
  policyman_policy_enum_ctx index     = *pCtx;
  size_t                    arraySize = ARR_SIZE(policyman_global_policy_files);
  boolean                   havePath  = FALSE;

  POLICYMAN_UT_MSG_1("policyman_policies_global_file_enum_next: processing file index %d", index);

  if (index < arraySize)
  {
    snprintf(pFileName, nameLength, POLICYMAN_EFS_ROOT_DIR"/%s", policyman_global_policy_files[index]);
    havePath = TRUE;
  }

  *pCtx = index + 1;
  return havePath;
}

/*-------- policyman_policies_file_enum_next --------*/
boolean policyman_policies_file_enum_next(
  policyman_policy_enum_ctx *pCtx,
  char                      *pFileName,
  size_t                     nameLength
)
{
  policyman_policy_enum_ctx index     = *pCtx;
  size_t                    arraySize = ARR_SIZE(policyman_subs_policy_files);
  boolean                   havePath  = FALSE;

  POLICYMAN_UT_MSG_1("policyman_policies_file_enum_next: processing file index %d", index);

  if (index < arraySize)
  {
    snprintf(pFileName, nameLength, POLICYMAN_EFS_ROOT_DIR"/%s", policyman_subs_policy_files[index]);
    havePath = TRUE;
  }

  *pCtx = index + 1;
  return havePath;
}

/*-------- policyman_policy_post_file_enum_next --------*/
boolean policyman_policy_post_file_enum_next(
  policyman_policy_enum_ctx *pCtx,
  char                      *pFileName,
  size_t                     nameLength
)
{
  policyman_policy_enum_ctx index     = *pCtx;
  size_t                    arraySize = ARR_SIZE(policyman_post_policy_files);
  boolean                   havePath  = FALSE;

  POLICYMAN_UT_MSG_1("policyman_policies_post_file_enum_next: processing file index %d", index);

  if (index < arraySize)
  {
    snprintf(pFileName, nameLength, POLICYMAN_EFS_ROOT_DIR"/%s", policyman_post_policy_files[index]);
    havePath = TRUE;
  }

  *pCtx = index + 1;
  return havePath;
}

/*-------- policyman_policy_load_global --------*/
policyman_status_t policyman_policy_load_global(
  policyman_state_t        *pState,
  mre_set_t                *pItemSet
)
{
  policyman_policy_enum_ctx  fileCtx;
  char                       fileName[128] = {0};
  policyman_status_t         status = POLICYMAN_STATUS_ERR;

  if (policyman_any_test_mode_is_enabled())
  {
    return status;
  }

  if (g_policy_is_initialized)
  {
    return POLICYMAN_STATUS_SUCCESS;
  }

  policyman_policies_file_enum_init(&fileCtx);

  while (policyman_policy_global_file_enum_next(&fileCtx, fileName, sizeof(fileName)))
  {
    mre_status_t         policyStatus;
    policyman_policy_t  *pPolicy = NULL;

    policyStatus = mre_policy_create_legacy(fileName,
                                            SYS_MODEM_AS_ID_NONE,
                                            policyman_condition_map,
                                            policyman_action_map,
                                            policyman_policy_new,
                                            (mre_policy_t **) &pPolicy );

    if (MRE_SUCCEEDED(policyStatus))
    {
      POLICYMAN_UT_MSG_S1( "Loaded global policy file %s", fileName );

      mre_set_add(g_pGlobalPolicySet, &pPolicy);
      ref_cnt_obj_release(pPolicy);

      policyman_policy_initialize(pPolicy, pState, pItemSet, FALSE);
    }
    else
    {
      POLICYMAN_UT_MSG_S2( "Failed (error %d) loading global policy file %s",
                        policyStatus,
                        fileName );
    }

    // map MRE status to Policyman status
    switch (policyStatus)
    {
      case MRE_STATUS_SUCCESS:
        status = POLICYMAN_STATUS_SUCCESS;
        break;
      case MRE_STATUS_ERR_NOT_PRESENT:
        status = POLICYMAN_STATUS_ERR_NOT_PRESENT;
        break;
      default:
        status = POLICYMAN_STATUS_ERR;
    }
  }

  return status;
}

/*-------- policyman_policy_load_subs --------*/
void policyman_policy_load_subs(
  policyman_state_t        *pState,
  sys_modem_as_id_e_type    policySubsId,
  mre_set_t                *pItemSet
)
{
  policyman_policy_enum_ctx  fileCtx;
  char                       fileName[128] = {0};

  policyman_policies_file_enum_init(&fileCtx);

  while (policyman_policies_file_enum_next(&fileCtx, fileName, sizeof(fileName)))
  {
    mre_status_t         status;
    policyman_policy_t  *pPolicy = NULL;

    status = mre_policy_create_legacy(fileName,
                                      policySubsId,
                                      policyman_condition_map,
                                      policyman_action_map,
                                      policyman_policy_new,
                                      (mre_policy_t **) &pPolicy );

    if (MRE_SUCCEEDED(status))
    {
      POLICYMAN_UT_MSG_S2( SUBS_PREFIX "Loaded policy file %s",
                        policySubsId,
                        fileName );

      policyman_policy_initialize(pPolicy, pState, pItemSet, FALSE);

      mre_set_add(g_pSubsPolicySet, &pPolicy);
      ref_cnt_obj_release(pPolicy);
    }
    else
    {
      POLICYMAN_UT_MSG_S3( SUBS_PREFIX "Failed (error %d) loading subs policy file %s",
                        policySubsId,
                        status,
                        fileName );
    }
  }

}


/*-------- policyman_policy_load_post --------*/
void policyman_policy_load_post(
  policyman_state_t        *pState,
  mre_set_t                *pItemSet
)
{
  policyman_policy_enum_ctx  fileCtx;
  char                       fileName[128] = {0};
  size_t                     nSim          = 0;

  if (   policyman_any_test_mode_is_enabled()
      || g_policy_is_initialized)
  {
    return;
  }

  policyman_policies_file_enum_init(&fileCtx);

  while (policyman_policy_post_file_enum_next(&fileCtx, fileName, sizeof(fileName)))
  {
    mre_status_t         policyStatus;
    policyman_policy_t  *pPolicy = NULL;

    policyStatus = mre_policy_create_legacy(fileName,
                                            SYS_MODEM_AS_ID_NONE,
                                            policyman_condition_map,
                                            policyman_action_map,
                                            policyman_policy_new,
                                            (mre_policy_t **) &pPolicy );

    if (MRE_SUCCEEDED(policyStatus))
    {
      sys_modem_as_id_e_type subsId;

      POLICYMAN_UT_MSG_S1( "Loaded post policy file %s",
                           fileName );

      mre_set_add(g_pPostPolicySet, &pPolicy);
      ref_cnt_obj_release(pPolicy);

      (void) policyman_get_current_num_sim(&nSim);

      for (subsId = SYS_MODEM_AS_ID_1; subsId < nSim; subsId++)
      {
        POLICY_SUBS(pPolicy) = subsId;
        policyman_policy_initialize(pPolicy, pState, pItemSet, TRUE);
      }

      POLICY_SUBS(pPolicy) = SYS_MODEM_AS_ID_NONE;
      mre_policy_release_initset((mre_policy_t *)pPolicy);
    }
    else
    {
      POLICYMAN_UT_MSG_S2( "Failed (error %d) loading global policy file %s",
                        policyStatus,
                        fileName );
    }
  }

  return;
}


/*-------- policyman_policies_init --------*/
void policyman_policies_init(
  void
)
{
  policyman_init_suspend_resume_framework();

  policyman_init_crit_sect(&policyman_policies_crit_sect);

  /*  Create the sets to hold the policies.
   */
  g_pPolicySet       = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
  g_pGlobalPolicySet = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
  g_pSubsPolicySet   = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
  g_pPostPolicySet   = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
}

/*-------- policyman_set_policy_initialized --------*/
void policyman_set_policy_initialized(
  boolean  is_initialized
)
{
  g_policy_is_initialized = is_initialized;
}

/*-------- policyman_policy_init_is_complete --------*/
void policyman_policy_init_is_complete(
  void
)
{
  if(!g_policy_is_initialized)
  {
    mre_set_add(g_pPolicySet, &g_pGlobalPolicySet);
    mre_set_add(g_pPolicySet, &g_pSubsPolicySet);
    mre_set_add(g_pPolicySet, &g_pPostPolicySet);
    g_policy_is_initialized = TRUE;
  }
}

/*-------- policyman_sub_policies_init --------*/
void policyman_sub_policies_init(
  policyman_state_t      *pState,
  mre_set_t              *pItemSet
)
{
  sys_modem_as_id_e_type  subsId;

  if (g_policy_is_initialized)
  {
    return;
  }

  for (subsId = SYS_MODEM_AS_ID_1; policyman_subs_id_is_valid(subsId); ++subsId)
  {
    policyman_fullrat_config_init_per_subs(subsId);
    policyman_policies_per_subs_init(pState, pItemSet, subsId);
  }
}

/*-------- policyman_policies_per_subs_init --------*/
void  policyman_policies_per_subs_init(
  policyman_state_t      *pState,
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  if (policyman_any_test_mode_is_enabled())
  {
    return;
  }

  if (!g_policy_is_initialized)
  {
    policyman_policy_load_subs(pState, subsId, pItemSet);
  }
}

/*-------- policyman_policies_deinit --------*/
void policyman_policies_deinit(
  void
)
{
  g_policy_is_initialized = FALSE;
  policyman_deinit_suspend_resume_framework();
  policyman_namespace_info_deinit();

  policyman_enter_crit_sect(&policyman_policies_crit_sect);

  MRE_RELEASE_IF(g_pGlobalPolicySet);
  MRE_RELEASE_IF(g_pSubsPolicySet);
  MRE_RELEASE_IF(g_pPostPolicySet);
  MRE_RELEASE_IF(g_pPolicySet);

  policyman_leave_crit_sect(&policyman_policies_crit_sect);

  policyman_deinit_crit_sect(&policyman_policies_crit_sect);
}

/*-------- policyman_policy_enter_ftm --------*/
void policyman_policy_enter_ftm(
  void
)
{
  /* FTM could be due to new calibration. Delete all
       persisted config items which will be obsolete in that case.
   */
  policyman_efs_obliterate_subs_ext(SYS_MODEM_AS_ID_NONE, FALSE);

  // Set FTM NV so that no rule execution can happen
  policyman_set_FTM(TRUE);
}


/*-------- reset_policies_for_ftm_handling --------*/
STATIC void reset_policies_for_ftm_handling(
  void
)
{
  policyman_policies_deinit();
  policyman_fullrat_config_deinit();

  policyman_set_FTM(FALSE);

  policyman_namespace_info_init();
  policyman_policies_init();
}


/*-------- policyman_policy_leave_ftm --------*/
void policyman_policy_leave_ftm(
  void
)
{
  /* Initialize Policy when already in FTM and disable FTM
        For test_mode, we should contiue to be running without policy
    */
  if (   policyman_FTM_is_enabled()
      && !policyman_test_mode_is_enabled()
     )
  {
    mre_set_t           *pItemSet;
    policyman_status_t   status;
    policyman_state_t   *pState;

    pItemSet = policyman_itemset_new();

    reset_policies_for_ftm_handling();
    policyman_global_sandbox_refresh_policies();

    pState = policyman_state_get_state();
    status = policyman_policy_load_global(pState, pItemSet);

    // global policy read is a success if file not present
    if (  status == POLICYMAN_STATUS_SUCCESS
       || status == MRE_STATUS_ERR_NOT_PRESENT
       )
    {
      policyman_sub_policies_init(pState, pItemSet);
      policyman_policy_load_post(pState, pItemSet);
    }

    policyman_policy_init_is_complete();

    // Update the database and send client notification.
    policyman_cfgitem_update_items(pState, pItemSet, NULL);

    policyman_cfgitem_display_all();
    MRE_RELEASE_IF(pItemSet);
  }
}


/*=====================================
  Policy EHPLMN update
=======================================*/

typedef struct
{
  sys_modem_as_id_e_type   subsId;
  mre_set_t               *pEhPlmnSet;
  mmgsdi_data_type        *pData;
  mmgsdi_data_type        *pAdData;
} policy_ehplmn_ctx_t;

/*-------- update_hplmn_in_policy --------*/
STATIC boolean update_hplmn_in_policy(
  void  *pElem,
  void  *pCtx
)
{
  policyman_policy_t   *pPolicy    = *((policyman_policy_t **) pElem);
  policy_ehplmn_ctx_t  *pHplmnCtx  = (policy_ehplmn_ctx_t *)pCtx;

  if (pHplmnCtx->subsId == pPolicy->subs)
  {
    mre_status_t status;

    status = mre_policy_set_hplmn( (mre_policy_t *)pPolicy,
                                    pHplmnCtx->pData,
                                    pHplmnCtx->pAdData );
  }

  return TRUE;
}


/*-------- policyman_policy_update_hplmn --------*/
void policyman_policy_update_hplmn(
  sys_modem_as_id_e_type   subsId,
  mmgsdi_data_type        *pHplmnData,
  mmgsdi_data_type        *pAdData
)
{
  policy_ehplmn_ctx_t  hplmnCtx;

  hplmnCtx.subsId     = subsId;
  hplmnCtx.pEhPlmnSet = NULL;
  hplmnCtx.pData      = pHplmnData;
  hplmnCtx.pAdData    = pAdData;

  mre_set_iterate(g_pSubsPolicySet, update_hplmn_in_policy, &hplmnCtx);
}


/*-------- update_ehplmn_in_policy --------*/
STATIC boolean update_ehplmn_in_policy(
  void  *pElem,
  void  *pCtx
)
{
  policyman_policy_t   *pPolicy    = *((policyman_policy_t **) pElem);
  policy_ehplmn_ctx_t  *pEhplmnCtx = (policy_ehplmn_ctx_t *)pCtx;

  if (pEhplmnCtx->subsId == pPolicy->subs)
  {
    mre_status_t status;

    status = mre_policy_add_ehplmn_set( (mre_policy_t *)pPolicy,
                                         pEhplmnCtx->pEhPlmnSet);

    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "ehplmn set updated in policy with status %d ",
                          pEhplmnCtx->subsId,
                          MRE_SUCCEEDED(status) );
  }

  return TRUE;
}


/*-------- policyman_policy_update_ehplmn_set --------*/
void policyman_policy_update_ehplmn_set(
  sys_modem_as_id_e_type  subsId,
  mre_set_t              *pEhPlmnSet
)
{
  policy_ehplmn_ctx_t  ehplmnCtx;

  ehplmnCtx.subsId     = subsId;
  ehplmnCtx.pEhPlmnSet = pEhPlmnSet;
  ehplmnCtx.pData      = NULL;
  ehplmnCtx.pAdData    = NULL;

  mre_set_iterate(g_pSubsPolicySet, update_ehplmn_in_policy, &ehplmnCtx);
}

/*-------- delete_hplmn_in_policy --------*/
STATIC boolean delete_hplmn_in_policy(
  void  *pElem,
  void  *pCtx
)
{
  mre_policy_t           *pPolicy = *((mre_policy_t **) pElem);
  sys_modem_as_id_e_type  subsId  = *(sys_modem_as_id_e_type *)pCtx;

  if (subsId == pPolicy->subs)
  {
    mre_policy_delete_hplmn(pPolicy);
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "hplmn deleted in policy 0x%x",
                          subsId,
                          pPolicy );
  }

  return TRUE;
}


/*-------- policyman_policy_delete_hplmn --------*/
void policyman_policy_delete_hplmn(
  sys_modem_as_id_e_type  subsId
)
{
  mre_set_iterate(g_pSubsPolicySet, delete_hplmn_in_policy, &subsId);
}

/*-------- release_ehplmn_set --------*/
STATIC void release_ehplmn_set(
 mre_policy_t * pPolicy
)
{
  MRE_RELEASE_IF(pPolicy->pEhPlmnSet);
  mre_policy_add_ehplmn_set(pPolicy, NULL);
}


/*-------- release_ehplmn_in_policy --------*/
STATIC boolean release_ehplmn_in_policy(
  void  *pElem,
  void  *pCtx
)
{
  mre_policy_t           *pPolicy = *((mre_policy_t **) pElem);
  sys_modem_as_id_e_type  subsId  = *(sys_modem_as_id_e_type *)pCtx;

  if (subsId == pPolicy->subs)
  {
    release_ehplmn_set(pPolicy);
  }

  return TRUE;
}


/*-------- policyman_policy_release_ehplmn_set --------*/
void policyman_policy_release_ehplmn_set(
  sys_modem_as_id_e_type  subsId
)
{
  mre_set_iterate(g_pSubsPolicySet, release_ehplmn_in_policy, &subsId);
}


/*=====================================
  Policy remove
=======================================*/

typedef struct
{
  sys_modem_as_id_e_type  subsId;
  mre_set_t              *pItemSet;
} policy_remove_ctx_t;

/*-------- policyman_policy_to_remove --------*/
STATIC boolean policyman_policy_to_remove(
  void  *pElem,
  void  *pCtx
)
{
  policyman_policy_t  *pPolicy    = *((policyman_policy_t **) pElem);
  policy_remove_ctx_t *pRemoveCtx = (policy_remove_ctx_t *) pCtx;

  if (pRemoveCtx->subsId == pPolicy->subs)
  {
    mre_set_add(pRemoveCtx->pItemSet, &pPolicy);
  }

  return TRUE;
}

/*-------- policyman_policy_remove --------*/
STATIC boolean policyman_policy_remove(
  void  *pElem,
  void  *pCtx
)
{
  policyman_policy_t *pPolicy    = *((policyman_policy_t **) pElem);
  mre_set_t          *pPolicySet = (mre_set_t *) pCtx;

  mre_set_remove(pPolicySet, &pPolicy);

  release_ehplmn_set((mre_policy_t *)pPolicy);

  return TRUE;
}

/*-------- policyman_policies_per_subs_deinit --------*/
void policyman_policies_per_subs_deinit(
  sys_modem_as_id_e_type subsId
)
{
  policy_remove_ctx_t   ctx;
  mre_set_t            *pRemoveSet;

  g_policy_is_initialized = FALSE;

  /*  Release any subs specific named object namespaces that were created.
   */
  policyman_namespace_info_per_subs_deinit(subsId);

  pRemoveSet    = mre_set_refcnt_new(3, MRE_SET_NO_MAX_SIZE, NULL);
  ctx.subsId    = subsId;
  ctx.pItemSet  = pRemoveSet;

  /* Remove subs specific policies from the set
   */
  mre_set_iterate(g_pSubsPolicySet, policyman_policy_to_remove, &ctx);
  mre_set_iterate(pRemoveSet, policyman_policy_remove, g_pSubsPolicySet);

  MRE_RELEASE_IF(pRemoveSet);
}

/*-------- policyman_can_policy_execute --------*/
boolean policyman_can_policy_execute(
  policyman_sandbox_t *pSandbox
)
{
  boolean ftmIsEnabled;
  boolean testModeIsEnabled;
  boolean policymanIsSuspended;
  boolean inLimitedCapability;
  policyman_cfg_error_cause errorCause;

  ftmIsEnabled         = policyman_FTM_is_enabled();
  testModeIsEnabled    = policyman_test_mode_is_enabled();
  policymanIsSuspended = policyman_is_suspended(pSandbox);
  inLimitedCapability  = policyman_cfgitem_limited_capabilities_is_enabled();
  errorCause           = policyman_get_limited_error_cause();

  // Return FALSE if in FTM, test mode, suspended, or in limited capabilities.
  if (ftmIsEnabled || testModeIsEnabled || policymanIsSuspended || inLimitedCapability)
  {
    POLICYMAN_MSG_ERROR_5( "Policies not executed while in: FTM %d, test_mode %d, suspended %d, limited capabilities %d with errorCause %d",
                           ftmIsEnabled,
                           testModeIsEnabled,
                           policymanIsSuspended,
                           inLimitedCapability,
                           errorCause);
    return FALSE;
  }

  return TRUE;
}

typedef struct
{
  policy_execute_ctx_t  *pCtx;
  elem_iter_fn_t         pfn;
} policy_set_iterate_ctx_t;

/*-------- policyman_policy_set_iterate --------*/
STATIC boolean policyman_policy_set_iterate(
  void  *pElem,
  void  *pCtx
)
{
  mre_set_t                 *pPolicySet = *((mre_set_t **) pElem);
  policy_set_iterate_ctx_t  *pIterCtx   = (policy_set_iterate_ctx_t *) pCtx;

  mre_set_iterate(pPolicySet, pIterCtx->pfn, pIterCtx->pCtx);

  return TRUE;
}

/*-------- policyman_policy_fill_policy_statistics() --------*/
void policyman_policy_fill_policy_statistics(
  policyman_policy_t      *pPolicy,
  pm_dump_policy_info_t   *pPolicyInfo
)
{
  // Get the policy statistics from MRE
  mre_rules_gather_policy_statistics( (mre_policy_t *) pPolicy,
                                      (mre_dump_policy_info_t *) &pPolicyInfo->policy_stats );

  // Add policyman-specific stats
  pPolicyInfo->suspend_count   = mre_set_num_elems(g_pSuspendClientSet);
  pPolicyInfo->is_policy_init  = g_policy_is_initialized;
}

/*-------- policyman_policy_send_diag_statistics --------*/
boolean policyman_policy_send_diag_statistics(
  void  *pElem,
  void  *pCtx
)
{
  policyman_policy_t            *pPolicy = *((policyman_policy_t **) pElem);
  LOG_PM_POLICY_STATS_INFO_type *log_ptr;

  log_ptr = LOG_PACKET_NEW( LOG_PM_POLICY_STATS_INFO_C,
                            LOG_PM_POLICY_STATS_INFO_type);

  if (NULL != log_ptr)
  {
    log_ptr->version      = LOG_PM_POLICY_STATS_INFO_LOG_PACKET_VERSION;

    memset(&log_ptr->policy_info, 0, sizeof(log_ptr->policy_info));

    policyman_policy_fill_policy_statistics( pPolicy,
                                             &log_ptr->policy_info );

    log_commit( (void *)log_ptr );
  }

  return TRUE;
}

/*-------- policyman_policies_send_diag_logs --------*/
void policyman_policies_send_diag_logs(
  void
)
{
  policy_execute_ctx_t      ctx;
  policy_set_iterate_ctx_t  iterCtx;

  ctx.pPolicy     = NULL;
  ctx.pState      = NULL;
  ctx.pItemSet    = NULL;
  ctx.subsId      = SYS_MODEM_AS_ID_NONE;
  iterCtx.pCtx    = &ctx;
  
  iterCtx.pfn     = policyman_policy_send_diag_statistics;

  mre_set_iterate(g_pPolicySet, policyman_policy_set_iterate, &iterCtx);
}

/*-------- policyman_policies_run_policy_check --------*/
/**
@brief  Run a policy check and collect updates into the itemset passed.

@param

@return

*/
void policyman_policies_run_policy_check(
  policyman_sandbox_t  *pSandbox,
  mre_set_t            *pItemSet
)
{
  if (policyman_can_policy_execute(pSandbox))
  {
    policy_execute_ctx_t      policyExecCtx;
    policyman_state_t        *pState;
    mre_set_iterator_t       *pIterator;
    mre_set_t                *pPolicySet;

    pState = pSandbox->pState;

    policyExecCtx.pPolicy  = NULL;
    policyExecCtx.pState   = pState;
    policyExecCtx.pItemSet = pItemSet;
    policyExecCtx.subsId   = SYS_MODEM_AS_ID_NONE;

    policyman_rf_set_full_bands(pSandbox->pState, policyExecCtx.pItemSet);

    pIterator = mre_set_get_iterator(pSandbox->pPolicySet);

    // run device level policies
    pPolicySet = *((mre_set_t **)mre_set_iterator_next_element(pIterator));

    if (NON_NULL(pPolicySet))
    {
      mre_set_iterate(pPolicySet, policyman_policy_run, &policyExecCtx);
    }

    // subs level
    pPolicySet = *((mre_set_t **)mre_set_iterator_next_element(pIterator));

    if (NON_NULL(pPolicySet))
    {
      mre_set_iterate(pPolicySet, policyman_policy_run, &policyExecCtx);
    }

    policyman_fullrat_config_evaluate(&policyExecCtx, policyman_fullrat_config_get_carrier);
    policyman_ca_band_combos_evaluate(&policyExecCtx);
    policyman_device_config_update_cdma_subphone_config(pState, pItemSet);

    // post level
    pPolicySet = *((mre_set_t **)mre_set_iterator_next_element(pIterator));

    if (NON_NULL(pPolicySet))
    {
      mre_set_iterate(pPolicySet, policyman_policy_run_post, &policyExecCtx);
    }

    policyman_state_cleanup_after_policy_run(pState);
    ref_cnt_obj_release(pIterator);
  }
}

/*-------- policyman_evaluate_cond_for_this_subs --------*/
boolean policyman_evaluate_cond_for_this_subs(
  policyman_condition_t const *pCond,
  void                        *pCtx,
  pfnEvaluateCondForSubs       pfnEvaluate
)
{
  return pfnEvaluate(pCond, pCtx, POLICY_EXECUTE_SUBS(pCtx));
}

/*-------- policyman_evaluate_cond_for_other_subs --------*/
boolean policyman_evaluate_cond_for_other_subs(
  policyman_condition_t const *pCond,
  void                        *pCtx,
  pfnEvaluateCondForSubs       pfnEvaluate
)
{
  sys_modem_as_id_e_type  subs;
  size_t                  nSim;
  policyman_status_t      status;

  status = policyman_get_current_num_sim(&nSim);

  for (subs = SYS_MODEM_AS_ID_1; subs < nSim && POLICYMAN_SUCCEEDED(status); ++subs)
  {
    if (   subs != POLICY_EXECUTE_SUBS(pCtx)
        && pfnEvaluate(pCond, pCtx, subs)
       )
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*-------- policyman_evaluate_cond_for_all_subs --------*/
boolean policyman_evaluate_cond_for_all_subs(
  policyman_condition_t const *pCond,
  void                        *pCtx,
  pfnEvaluateCondForSubs       pfnEvaluate
)
{
  return    policyman_evaluate_cond_for_this_subs(pCond, pCtx, pfnEvaluate)
        &&  policyman_evaluate_cond_for_other_subs(pCond, pCtx, pfnEvaluate);
}

/*-------- policyman_evaluate_cond_for_any_subs --------*/
boolean policyman_evaluate_cond_for_any_subs(
  policyman_condition_t const *pCond,
  void                        *pCtx,
  pfnEvaluateCondForSubs       pfnEvaluate
)
{
  return    policyman_evaluate_cond_for_this_subs(pCond, pCtx, pfnEvaluate)
        ||  policyman_evaluate_cond_for_other_subs(pCond, pCtx, pfnEvaluate);
}

/*-------- policyman_evaluate_cond --------*/
boolean policyman_evaluate_cond(
  policyman_condition_t const *pCond,
  void                        *pCtx,
  pfnEvaluateCondForSubs       pfnEvaluate
)
{
  switch (pCond->pmSubsId)
  {
    case SUBS_THIS:
      return policyman_evaluate_cond_for_this_subs(pCond, pCtx, pfnEvaluate);
      break;

    case SUBS_OTHER:
      return policyman_evaluate_cond_for_other_subs(pCond, pCtx, pfnEvaluate);
      break;

    case SUBS_ALL:
      return policyman_evaluate_cond_for_all_subs(pCond, pCtx, pfnEvaluate);
      break;

    case SUBS_ANY:
      return policyman_evaluate_cond_for_any_subs(pCond, pCtx, pfnEvaluate);
      break;

    default:
      return FALSE;
      break;
  }
}

/*-------- policyman_execute_action_for_this_subs --------*/
boolean policyman_execute_action_for_this_subs(
  policyman_action_t const *pAction,
  void                     *pCtx,
  pfnExecuteActionForSubs   pfnExecute
)
{
  return pfnExecute(pAction, pCtx, POLICY_EXECUTE_SUBS(pCtx));
}

/*-------- policyman_execute_action_for_other_subs --------*/
boolean policyman_execute_action_for_other_subs(
  policyman_action_t const *pAction,
  void                     *pCtx,
  pfnExecuteActionForSubs   pfnExecute
)
{
  sys_modem_as_id_e_type  subsId;
  size_t                  nSim;
  policyman_status_t      status;

  status = policyman_get_current_num_sim(&nSim);
  for ( subsId = SYS_MODEM_AS_ID_1;
        (POLICYMAN_SUCCEEDED(status) && subsId < nSim);
        subsId++)
  {
    if (   subsId != POLICY_EXECUTE_SUBS(pCtx)
        && pfnExecute(pAction, pCtx, subsId)
       )
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*-------- policyman_execute_action_for_all_subs --------*/
boolean policyman_execute_action_for_all_subs(
  policyman_action_t const *pAction,
  void                     *pCtx,
  pfnExecuteActionForSubs   pfnExecute
)
{
  return    policyman_execute_action_for_this_subs(pAction, pCtx, pfnExecute)
        &&  policyman_execute_action_for_other_subs(pAction, pCtx, pfnExecute);
}

/*-------- policyman_execute_action --------*/
boolean policyman_execute_action(
  policyman_action_t const *pAction,
  void                     *pCtx,
  pfnExecuteActionForSubs   pfnExecute
)
{
  switch (pAction->pmSubsId)
  {
    case SUBS_THIS:
      return policyman_execute_action_for_this_subs(pAction, pCtx, pfnExecute);
      break;

    case SUBS_OTHER:
      return policyman_execute_action_for_other_subs(pAction, pCtx, pfnExecute);
      break;

    case SUBS_ALL:
      return policyman_execute_action_for_all_subs(pAction, pCtx, pfnExecute);
      break;

    case SUBS_ANY:  // Can't run action on unspecified subs
    default:
      return FALSE;
      break;
  }

}

/*-------- policyman_condition_new --------*/
policyman_condition_t * policyman_condition_new(
  size_t                   condSize,
  mre_policy_t            *pPolicy,
  pfn_evaluate_t           pEvaluate,
  destructor_ptr           pDtor,
  policyman_subs_t         pmSubsId,
  mre_xml_element_t const *pElem
)
{
  policyman_condition_t *pCondition;

  pCondition = (policyman_condition_t *)mre_condition_new( condSize,
                                                           (mre_policy_t *)pPolicy,
                                                           pEvaluate,
                                                           pDtor,
                                                           pElem );
  pCondition->pmSubsId = pmSubsId;

  return pCondition;
}

/*-------- policyman_action_new --------*/
policyman_action_t * policyman_action_new(
  size_t                   actionSize,
  mre_policy_t            *pPolicy,
  pfn_execute_t            pExecute,
  destructor_ptr           pDtor,
  policyman_subs_t         pmSubsId,
  mre_xml_element_t const *pElem
)
{
  policyman_action_t *pAction;

  pAction = (policyman_action_t *)mre_action_new( actionSize,
                                                  (mre_policy_t *)pPolicy,
                                                  pExecute,
                                                  pDtor,
                                                  pElem );
  pAction->pmSubsId = pmSubsId;

  return pAction;
}

/*-------- policyman_policies_gather_statistics --------*/
void policyman_policies_gather_statistics(
  size_t                 policy_idx,
  pm_dump_policy_info_t *pPolicyInfo
)
{
  policyman_policy_t   *pPolicy;

  pPolicy = policyman_get_policy(policy_idx);

  if ( (NULL != pPolicy) && (NULL != pPolicyInfo) )
  {
    // Get the policy statistics from MRE
    policyman_policy_fill_policy_statistics( pPolicy,
                                             pPolicyInfo );
  }
}

QSH_MDUMP_FN_ATTR_PM boolean policyman_policy_mdump(
  void *pElem,
  void *ctx
)
{
  policyman_policy_t *ptrPolicy = *((policyman_policy_t**)pElem);

  policyman_qsh_dump_collect((void *)ptrPolicy, sizeof(policyman_policy_t));
  policyman_qsh_dump_collect((void *)ptrPolicy->pInfo, sizeof(mre_policy_info_t));
  
  return TRUE;
}

QSH_MDUMP_FN_ATTR_PM void policyman_policies_mdump(
  void
)
{
  mre_set_iterate(g_pGlobalPolicySet, policyman_policy_mdump, NULL);
  mre_set_iterate(g_pSubsPolicySet, policyman_policy_mdump, NULL);
  mre_set_iterate(g_pPostPolicySet, policyman_policy_mdump, NULL);
}
