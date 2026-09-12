/**
  @file policyman_fullrat_config.c

  @brief
*/

/*
    Copyright (c) 2015-2018 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_fullrat_config.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_efs.h"
#include "mre_engine.h"
#include "mre_rules.h"
#include "mre_set.h"
#include "mre_xml.h"

#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_fullrat_config.h"
#include "policyman_phone_events.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_rules.h"
#include "policyman_serving_system.h"
#include "policyman_timer.h"
#include "policyman_svc_mode.h"

#include "event.h"
#include <stringl/stringl.h>

#define FULLRAT_TIMER_ID 1000
#define FULLRAT_TIMER_DEFAULT 120
#define ACQFAIL_CNT_DEFAULT 1

struct fullrat_config_t
{
  mre_policy_t           *pPolicy;
  policyman_timer_t      *pTimer;
  uint8                   scan_cnt;
  sys_modem_as_id_e_type  subsId;
  mre_set_t              *pActionSet;
  boolean                 timerExpired;
};

STATIC fullrat_config_t g_FullRatConfigs[POLICYMAN_NUM_SUBS];
STATIC fullrat_config_t g_PostFullRatConfigs[POLICYMAN_NUM_SUBS];

/*-------- policyman_fullrat_config_get_carrier --------*/
fullrat_config_t *policyman_fullrat_config_get_carrier(
  sys_modem_as_id_e_type subsId
)
{
  POLICYMAN_UT_MSG_1(SUBS_PREFIX "Normal full rat config being used", subsId);
  return &g_FullRatConfigs[subsId];
}

/*-------- policyman_fullrat_config_get_post --------*/
fullrat_config_t *policyman_fullrat_config_get_post(
  sys_modem_as_id_e_type subsId
)
{
  POLICYMAN_UT_MSG_1(SUBS_PREFIX "Post full rat config being used", subsId);
  return &g_PostFullRatConfigs[subsId];
}


/*-------- policyman_fullrat_config_init_post_per_subs --------*/
STATIC void policyman_fullrat_config_init_post_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfg;

  pFullRatCfg = policyman_fullrat_config_get_post(subsId);

  pFullRatCfg->pTimer     = NULL;
  pFullRatCfg->scan_cnt   = ACQFAIL_CNT_DEFAULT;
  pFullRatCfg->subsId     = subsId;
  pFullRatCfg->pActionSet = NULL;
}

/*-------- policyman_fullrat_config_init_carrier_per_subs --------*/
void policyman_fullrat_config_init_carrier_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfg;

  pFullRatCfg = policyman_fullrat_config_get_carrier(subsId);

  pFullRatCfg->pTimer       = NULL;
  pFullRatCfg->scan_cnt     = ACQFAIL_CNT_DEFAULT;
  pFullRatCfg->subsId       = subsId;
  pFullRatCfg->pActionSet   = NULL;
  pFullRatCfg->timerExpired = FALSE;
}


/*-------- policyman_fullrat_config_timer_stop_per_subs --------*/
void policyman_fullrat_config_timer_stop_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfg;
  
  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Stop Timer", subsId);
  pFullRatCfg = policyman_fullrat_config_get_carrier(subsId);
  policyman_timer_stop(pFullRatCfg->pTimer);
}


/*-------- policyman_fullrat_config_timer_stop_post_per_subs --------*/
void policyman_fullrat_config_timer_stop_post_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfgPost;

  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Post Stop Timer", subsId);
  pFullRatCfgPost = policyman_fullrat_config_get_post(subsId);
  policyman_timer_stop(pFullRatCfgPost->pTimer);
}


/*-------- policyman_fullrat_config_reset_timer_expired_per_subs --------*/
void policyman_fullrat_config_reset_timer_expired_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfg;
  
  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "resetting Timer expired", subsId);
  pFullRatCfg = policyman_fullrat_config_get_carrier(subsId);
  pFullRatCfg->timerExpired = FALSE;
}


/*------- policyman_fullrat_config_reset_timer_expired_post_per_subs ------*/
void policyman_fullrat_config_reset_timer_expired_post_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfgPost;
  
  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "resetting post-Timer-expired", subsId);
  pFullRatCfgPost = policyman_fullrat_config_get_post(subsId);
  pFullRatCfgPost->timerExpired = FALSE;
}

/*-------- policyman_fullrat_config_init_per_subs --------*/
void policyman_fullrat_config_init_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  policyman_fullrat_config_init_carrier_per_subs(subsId);
  policyman_fullrat_config_init_post_per_subs(subsId);
}


/*-------- policyman_fullrat_config_deinit_per_subs --------*/
STATIC void policyman_fullrat_config_deinit_post_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfg;

  pFullRatCfg = policyman_fullrat_config_get_post(subsId);

  policyman_timer_stop(pFullRatCfg->pTimer);
  MRE_RELEASE_IF(pFullRatCfg->pTimer);
  MRE_RELEASE_IF(pFullRatCfg->pActionSet);

}

/*-------- policyman_fullrat_config_deinit_per_subs --------*/
void policyman_fullrat_config_deinit_per_subs(
  sys_modem_as_id_e_type subsId
)
{
  fullrat_config_t *pFullRatCfg;

  pFullRatCfg = policyman_fullrat_config_get_carrier(subsId);

  policyman_timer_stop(pFullRatCfg->pTimer);
  MRE_RELEASE_IF(pFullRatCfg->pTimer);
  MRE_RELEASE_IF(pFullRatCfg->pActionSet);

}

/*-------- policyman_fullrat_config_deinit --------*/
void policyman_fullrat_config_deinit(
  void
)
{
  size_t            subsIndex;

  for (subsIndex = 0; subsIndex < POLICYMAN_NUM_SUBS; subsIndex++)
  {
    policyman_fullrat_config_deinit_per_subs(subsIndex);
    policyman_fullrat_config_deinit_post_per_subs(subsIndex);
  }
}

/*-------- policyman_scancnt_exceeded_config --------*/
STATIC boolean policyman_scancnt_exceeded_config(
  policyman_state_t *pState,
  fullrat_config_t  *pFullRatCfg
)
{
  boolean limitWasReached = FALSE;

  if (pFullRatCfg->scan_cnt > 0)
  {
    limitWasReached = (policyman_ss_get_min_acq_fail_count(pState, pFullRatCfg->subsId) >= pFullRatCfg->scan_cnt);
  }

  return limitWasReached;
}

/*-------- policyman_fullrat_conditions_are_met --------*/
STATIC boolean policyman_fullrat_conditions_are_met(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  fullrat_config_t *pFullRatCfg;
  boolean           have_location;
  boolean           oos_scancnt_exceeded;
  boolean           hysteresis_expired;
  boolean           conditions_met;
  location_info_t   locationInfo;
  boolean           inCall;

  pFullRatCfg          = policyman_fullrat_config_get_carrier(subsId);
  oos_scancnt_exceeded = policyman_scancnt_exceeded_config(pState, pFullRatCfg);
  inCall               = policyman_cmcall_subs_is_in_call(pState, subsId);

  policyman_ss_get_location_info(pState, &locationInfo);

  PRINT_LOCATION_INFO(locationInfo);

  have_location =     policyman_ss_have_location_from_user_or_subs(&locationInfo, subsId)
                   || policyman_ss_have_location_from_subs(&locationInfo, OTHER_SUB(subsId))
                   || policyman_ss_have_location_from_hlos(&locationInfo);

  hysteresis_expired   = pFullRatCfg->timerExpired || oos_scancnt_exceeded;
  conditions_met       = (!have_location && !inCall && hysteresis_expired);

  POLICYMAN_UT_MSG_6( SUBS_PREFIX "Fullrat conditions met: %d (have_location %d, inCall %d, timer_expired %d, oos_scancnt_exceeded %d)",
                      subsId,
                      conditions_met,
                      have_location,
                      inCall,
                      pFullRatCfg->timerExpired,
                      oos_scancnt_exceeded );

  // Send FULL RAT event
  {
    policyman_diag_event_full_rat_mode_eval_t event_full_rat;
    timetick_type                             interval;

    interval = policyman_timer_get_interval(pFullRatCfg->pTimer);

    event_full_rat.subs                  = (int8)subsId;
    event_full_rat.conditions_met        = conditions_met;
    event_full_rat.have_location         = have_location;
    event_full_rat.timer_expired         = pFullRatCfg->timerExpired;
    event_full_rat.oos_scan_cnt_exceeded = oos_scancnt_exceeded;
    event_full_rat.timer_interval        = interval;

    event_report_payload( EVENT_PM_FULL_RAT_EVAL,
                          sizeof(event_full_rat),
                          &event_full_rat );
  }

  return (conditions_met);
}

/*-------- policyman_fullrat_execute_actions --------*/
STATIC void policyman_fullrat_execute_actions(
  policy_execute_ctx_t *pCtx,
  fullrat_get_fn_t     pfnGetConfig
)
{
  fullrat_config_t *pFullRatCfg;

  pFullRatCfg = pfnGetConfig(pCtx->subsId);
  
  if (NULL != pFullRatCfg && NULL != pFullRatCfg->pActionSet)
  {
    int8 subs_id;
    policyman_svc_mode_item_t *pItem;

    POLICYMAN_UT_MSG_1(SUBS_PREFIX "entering Full Rat mode", pCtx->subsId);
    pItem = policyman_svc_mode_item_new(POLICYMAN_SVC_MODE_FULL_RAT, pCtx->subsId);
    mre_set_replace(pCtx->pItemSet, &pItem);
    ref_cnt_obj_release(pItem);


    subs_id = pCtx->subsId;
    event_report_payload( EVENT_PM_ENTER_FULL_RAT,
                          sizeof(subs_id),
                          &subs_id);

    MRE_POLICY_STOPEVAL(pFullRatCfg->pPolicy) = FALSE;
    MRE_POLICY_DESCEND_LEVEL(pFullRatCfg->pPolicy);
    mre_actionset_execute(pFullRatCfg->pActionSet, pCtx);
    MRE_POLICY_ASCEND_LEVEL(pFullRatCfg->pPolicy);
  }
}

/*-------- policyman_get_fullrat_timer --------*/
policyman_timer_t * policyman_get_fullrat_timer(
  sys_modem_as_id_e_type  subsId
)
{
  fullrat_config_t *pFullRatCfg;

  pFullRatCfg = policyman_fullrat_config_get_carrier(subsId);

  return pFullRatCfg->pTimer;
}

/*-------- policyman_fullrat_enter_check --------*/
STATIC void policyman_fullrat_enter_check(
  policy_execute_ctx_t  *pCtx,
  fullrat_get_fn_t       pfnGetConfig
)
{
  if (policyman_fullrat_conditions_are_met(pCtx->pState, pCtx->subsId))
  {
    policyman_fullrat_execute_actions(pCtx, pfnGetConfig);
  }
}

/*-------- policyman_fullrat_config_stop_timer_per_subs --------*/
void policyman_fullrat_config_stop_timer_per_subs(
  sys_modem_as_id_e_type  subsId
)
{
  policyman_timer_t      *pTimer;

  POLICYMAN_MSG_HIGH_1("Stopping  Sub: %d full RAT timers", subsId);

  pTimer = policyman_get_fullrat_timer(subsId);
  policyman_timer_stop(pTimer);
}

/*-------- policyman_fullrat_config_evaluate_per_subs --------*/
void policyman_fullrat_config_evaluate_per_subs(
  policy_execute_ctx_t   *pCtx,
  sys_modem_as_id_e_type  subsId,
  fullrat_get_fn_t        pfnGetConfig
)
{
  if (!policyman_ue_is_online(pCtx->pState))
  {
    POLICYMAN_MSG_HIGH_0("Not evaluating full RAT conditions; UE is not ONLINE");
    policyman_fullrat_config_stop_timer_per_subs(subsId);
  }
  else
  {
    POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Evaluating full RAT conditions", subsId);
    pCtx->subsId = subsId;
    policyman_fullrat_enter_check(pCtx, pfnGetConfig);
  }
}

/*-------- policyman_fullrat_config_evaluate --------*/
void policyman_fullrat_config_evaluate(
  policy_execute_ctx_t   *pCtx,
  fullrat_get_fn_t        pfnGetConfig
)
{
  size_t                 numSim = 0;
  sys_modem_as_id_e_type subsId;

  (void)policyman_get_current_num_sim(&numSim);

  for (subsId = SYS_MODEM_AS_ID_1; subsId < numSim; subsId++)
  {
    pCtx->pPolicy = (pfnGetConfig(subsId))->pPolicy;
    policyman_fullrat_config_evaluate_per_subs(pCtx, subsId, pfnGetConfig);
  }
}

/*-------- policyman_fullrat_config_notify_service --------*/
void policyman_fullrat_config_notify_service(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  fullrat_config_t       *pFullRatConfig;
  fullrat_config_t       *pFullRatConfigPost;
  boolean                 haveService;

  pFullRatConfig     = policyman_fullrat_config_get_carrier(subsId);
  pFullRatConfigPost = policyman_fullrat_config_get_post(subsId);
  haveService        = policyman_ss_subs_is_in_service(pState, subsId);

  if (!haveService && policyman_ue_is_online(pState))
  {
    policyman_timer_start(pFullRatConfig->pTimer);
  }
  else
  {
    policyman_fullrat_config_stop_timer_per_subs(subsId);
  }

  if (haveService)
  {
    pFullRatConfig->timerExpired = FALSE;
    pFullRatConfigPost->timerExpired = FALSE;
  }
}

/*-------- policyman_fullrat_timer_expired --------*/
STATIC void policyman_fullrat_timer_expired(
  policyman_timer_t *pTimer
)
{
  policy_execute_ctx_t    policyExecCtx;
  fullrat_config_t       *pFullRatConfig;
  fullrat_config_t       *pFullRatConfigPost;

  policyExecCtx.pPolicy  = (mre_policy_t *)policyman_timer_get_policy(pTimer);
  policyExecCtx.pState   = policyman_state_get_state();
  policyExecCtx.pItemSet = policyman_itemset_new();
  policyExecCtx.subsId   = policyman_timer_get_subs(pTimer);

  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "policyman_fullrat_timer_expired", policyExecCtx.subsId);

  policyman_ss_set_subs_serving_info_validity(policyExecCtx.pState, policyExecCtx.subsId, FALSE);

  pFullRatConfig = policyman_fullrat_config_get_carrier(policyExecCtx.subsId);
  pFullRatConfig->timerExpired = TRUE;

  policyman_fullrat_enter_check(&policyExecCtx, policyman_fullrat_config_get_carrier);

  pFullRatConfigPost = policyman_fullrat_config_get_post(policyExecCtx.subsId);
  if (NON_NULL(pFullRatConfigPost->pActionSet))
  {
    policyExecCtx.pPolicy = (mre_policy_t *)pFullRatConfigPost->pPolicy;
    POLICY_SUBS(pFullRatConfigPost->pPolicy) = policyExecCtx.subsId;
    pFullRatConfigPost->timerExpired = TRUE;

    policyman_fullrat_enter_check(&policyExecCtx, policyman_fullrat_config_get_post);

    POLICY_SUBS(pFullRatConfigPost->pPolicy) = SYS_MODEM_AS_ID_NONE;
  }

  policyman_cfgitem_update_items(policyExecCtx.pState, policyExecCtx.pItemSet, NULL);
  ref_cnt_obj_release(policyExecCtx.pItemSet);

  policyman_timer_start(pTimer);
}

/*=============================================================================
  <define_fullrat_config>
=============================================================================*/
typedef struct
{
  POLICYMAN_ACTION_BASE;

  timetick_type      timer_secs;
  uint8              scan_cnt;
  mre_set_t         *pActionSet;
  boolean            isPost;
} fullrat_config_action_t;

/*-------- policyman_named_fullrat_action_dtor --------*/
STATIC void policyman_fullrat_action_dtor(
  void  *pObj
)
{
  fullrat_config_action_t *pAction = (fullrat_config_action_t *) pObj;

  MRE_RELEASE_IF(pAction->pActionSet);
  policyman_action_dtor(pAction);
}

/*-------- execute_fullrat_config --------*/
STATIC boolean execute_fullrat_config(
  policyman_action_t const     *pAction,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  fullrat_config_action_t  *pFRAction   = (fullrat_config_action_t *)pAction;
  mre_policy_t             *pPolicy      = EXECUTE_CTX_POLICY(pCtx);
  fullrat_config_t         *pFullRatCfg;
  policyman_timer_t        *pTimer;

  POLICYMAN_MSG_HIGH_1(SRC_LINE "executing <define_full_rat_config>", LINE_NUM(pFRAction));

  if (pFRAction->isPost)
  {
    pFullRatCfg = policyman_fullrat_config_get_post(pPolicy->subs);
  }
  else
  {
    pFullRatCfg = policyman_fullrat_config_get_carrier(pPolicy->subs);

    /* release Timer and actionset in full rat config if already exists.
          we are now going to update with recently set configuration */
    policyman_fullrat_config_deinit_per_subs(pPolicy->subs);
    
  }

  pTimer = policyman_timer_create_fixed_timer( NULL,
                                               pPolicy->subs,
                                               pPolicy,
                                               FULLRAT_TIMER_ID,
                                               pFRAction->timer_secs,
                                               policyman_fullrat_timer_expired );

  pFullRatCfg->pPolicy    = pPolicy;
  pFullRatCfg->pTimer     = pTimer;
  pFullRatCfg->scan_cnt   = pFRAction->scan_cnt;
  pFullRatCfg->pActionSet = pFRAction->pActionSet;
  ref_cnt_obj_add_ref(pFullRatCfg->pActionSet);

  return TRUE;
}

/*-------- policyman_fullrat_config_execute --------*/
STATIC boolean policyman_fullrat_config_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const  *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_fullrat_config);
}

/*-------- policyman_fullrat_config_new --------*/
mre_status_t policyman_fullrat_config_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_action_t             **ppAction
)
{
  fullrat_config_action_t *pAction;
  char const              *pStr;
  mre_status_t             status;

  pAction = POLICYMAN_ACTION_NEW( fullrat_config_action_t,
                                  pPolicy,
                                  policyman_fullrat_config_execute,
                                  policyman_fullrat_action_dtor,
                                  SUBS_THIS,
                                  pElem );

  pStr = mre_xml_get_attribute(pElem, "is_post");
  if (NON_NULL(pStr))
  {
    mre_str_to_boolean(pStr, &pAction->isPost);
  }

  pStr = mre_xml_get_attribute(pElem, "timer_secs");
  if (NON_NULL(pStr))
  {
    pAction->timer_secs = atoi(pStr);
  }

  if (0 == pAction->timer_secs)
  {
    pAction->timer_secs = FULLRAT_TIMER_DEFAULT;
  }

  pStr = mre_xml_get_attribute(pElem, "scan_fail_cnt");
  if (NON_NULL(pStr))
  {
    pAction->scan_cnt = atoi(pStr);
  }
  else
  {
    pAction->scan_cnt = ACQFAIL_CNT_DEFAULT;
  }

  status = mre_rule_get_actionset(pElem, pPolicy, &pAction->pActionSet);
  if (MRE_SUCCEEDED(status))
  {
    *ppAction = (mre_action_t *) pAction;
  }
  else
  {
    ref_cnt_obj_release(pAction);
  }

  return status;
}

/*=============================================================================
  <enter_fullrat>
=============================================================================*/
typedef struct
{
  POLICYMAN_ACTION_BASE;

  boolean  isPost;
} fullrat_enter_action_t;


/*-------- execute_fullrat_enter --------*/
STATIC boolean execute_fullrat_enter(
  policyman_action_t const      *pAction,
  void                          *pCtx,
  sys_modem_as_id_e_type         subsId
)
{
  fullrat_enter_action_t  *pFRAction   = (fullrat_enter_action_t *)pAction;
  mre_policy_t            *pPolicy = EXECUTE_CTX_POLICY(pCtx);


  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX SRC_LINE "executing <fullrat_enter>",
                        POLICY_SUBS(pPolicy),
                        LINE_NUM(pAction) );

  if (pFRAction->isPost)
  {
    policyman_fullrat_execute_actions(pCtx, policyman_fullrat_config_get_post);
  }
  else
  {
    policyman_fullrat_execute_actions(pCtx, policyman_fullrat_config_get_carrier);
  }

  return TRUE;
}

/*-------- policyman_fullrat_enter_execute --------*/
STATIC boolean policyman_fullrat_enter_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const  *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_fullrat_enter);
}

/*-------- policyman_fullrat_enter_new --------*/
mre_status_t policyman_fullrat_enter_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  fullrat_enter_action_t  *pAction;
  char const              *pStr;

  pAction = POLICYMAN_ACTION_NEW( fullrat_enter_action_t,
                                  pPolicy,
                                  policyman_fullrat_enter_execute,
                                  policyman_action_dtor,
                                  SUBS_THIS,
                                  pElem );

  pStr = mre_xml_get_attribute(pElem, "is_post");
  if (NON_NULL(pStr))
  {
    mre_str_to_boolean(pStr, &pAction->isPost);
  }

  *ppAction = (mre_action_t *)pAction;

  return MRE_STATUS_SUCCESS;
}
