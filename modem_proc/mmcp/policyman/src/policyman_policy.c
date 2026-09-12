/**
  @file policyman_policy.c

  @brief Public interface to Policy Manager
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_policy.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre.h"
#include "mre_efs.h"
#include "mre_engine.h"
#include "mre_set.h"
#include "mre_util.h"

#include "policyman_call_events.h"
#include "policyman_call_mode.h"
#include "policyman_cfgitem.h"
#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_efs.h"
#include "policyman_frequency_list.h"
#include "policyman_fullrat_config.h"
#include "policyman_network_events.h"
#include "policyman_plmn.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_rat_capability.h"
#include "policyman_rat_order.h"
#include "policyman_scan_optimization.h"
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


/*-------- policyman_policy_get_subs --------*/
sys_modem_as_id_e_type  policyman_policy_get_subs(
  mre_policy_t *pPolicy
)
{
  policyman_policy_t *pSubsPolicy;

  pSubsPolicy = (policyman_policy_t *)pPolicy;
  return pSubsPolicy->subs;
}

/*-------- policyman_policy_dtor --------*/
void policyman_policy_dtor(
  void  *pObj
)
{
  mre_policy_dtor(pObj);
}


/*-------- policy_disable--------*/
STATIC void policy_disable(
  policyman_policy_t              *pPolicy,
  policyman_cfg_error_cause        reason
)
{
  MRE_POLICY_ENABLED((mre_policy_t *) pPolicy) = FALSE;
  POLICYMAN_POLICY_ERROR_CAUSE(pPolicy) = reason;
}



/*-------- policyman_policy_initialize --------*/
/**
@brief  Read the policy_info file for this policy and use it to initialize
        the policy.

@param[in]  pPolicy   Pointer to the policy.

@return
  TRUE if policy is initialized
*/
boolean policyman_policy_initialize(
  policyman_policy_t  *pPolicy,
  policyman_state_t   *pState,
  mre_set_t           *pItemSet,
  boolean              keepInitSet
)
{
  policy_execute_ctx_t   ctx;
  policyman_cfg_error_cause errorCause = POLICYMAN_CFG_ERROR_NONE;

  /*  If we're passed a NULL pointer for the state, we're just running
   *  an XML validation test.  Finish here.
   */
  if (pState == NULL)
  {
    goto Done;
  }

  /*  Configure the policy, setting any initial configuration items.
   */
  ctx.pPolicy  = (mre_policy_t *)pPolicy;
  ctx.pItemSet = pItemSet;
  ctx.pState   = pState;
  ctx.subsId   = pPolicy->subs;

  policyman_enter_namespace_crit_sect();

  if (keepInitSet)
  {
    mre_policy_init_and_keep_initset( (mre_policy_t *)pPolicy,
                                      policyman_get_namespace_info(),
                                      (mre_policy_execute_ctx_t *)&ctx );
  }
  else
  {
    mre_policy_init( (mre_policy_t *)pPolicy,
                     policyman_get_namespace_info(),
                     (mre_policy_execute_ctx_t *)&ctx );
  }

  policyman_leave_namespace_crit_sect();

  errorCause = policyman_get_limited_error_cause();

  if (errorCause == POLICYMAN_CFG_ERROR_NONE)
  {
    if (!policyman_subs_id_is_configured(pPolicy->subs))
    {
      errorCause = POLICYMAN_CFG_ERROR_CARRIER_ON_INVALID_SUB;
    }
    else if (!policyman_feature_is_supported_by_hardware(pItemSet))
    {
      POLICYMAN_MSG_ERROR_1( SUBS_PREFIX "========== Mismatch in XML and device capabilities ========",
                             POLICY_SUBS(pPolicy) );

      errorCause = POLICYMAN_CFG_ERROR_POLICY_FEATURE_MISMATCH;
    }
    else if (!policyman_config_is_supported_by_hardware())
    {
      POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "========== Unsupported DEVICE CONFIG ========", POLICY_SUBS(pPolicy) );
      errorCause = POLICYMAN_CFG_ERROR_INVALID_DEVICE_CONFIG;
    }
  }

  if (errorCause != POLICYMAN_CFG_ERROR_NONE)
  {
    policy_disable(pPolicy, errorCause);

    policyman_cfgitem_set_limited_capabilities(errorCause);
  }

Done:

  return TRUE;
}

/*-------- policyman_policy_new --------*/
mre_policy_t * policyman_policy_new(
  char const              *pPolicyName,
  mre_xml_element_t const *pPolicyElem
)
{
  policyman_policy_t  *pPolicy;


  pPolicy = policyman_mem_alloc(sizeof(policyman_policy_t));
  ref_cnt_obj_init(pPolicy, policyman_policy_dtor);

  POLICYMAN_MSG_HIGH_0("Policyman: created policy object");

  return (mre_policy_t *) pPolicy;
}

/*-------- policy_execute --------*/
STATIC void policy_execute(
  policyman_policy_t    *pPolicy,
  policy_execute_ctx_t  *pCtx
)
{
  POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "----------- Started executing policy version %d.%d.%d (%d) -----------",
                        POLICY_SUBS(pPolicy),
                        MRE_POLICY_NUMBER((mre_policy_t *)pPolicy),
                        MRE_POLICY_TYPE((mre_policy_t *)pPolicy),
                        MRE_POLICY_VERSION((mre_policy_t *)pPolicy),
                        MRE_POLICY_CHANGELIST((mre_policy_t *)pPolicy) );

  if (POLICYMAN_POLICY_ERROR_CAUSE(pPolicy) == POLICYMAN_CFG_ERROR_POLICY_FEATURE_MISMATCH)
  {
    POLICYMAN_MSG_ERROR_1( SUBS_PREFIX "Policyman feature does not match capabilities",
                           POLICY_SUBS(pPolicy) );
  }

  mre_policy_execute((mre_policy_t *) pPolicy, (mre_policy_execute_ctx_t *)pCtx);

  POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "----------- Finished executing policy %d.%d.%d (%d ticks) -----------",
                        POLICY_SUBS(pPolicy),
                        MRE_POLICY_NUMBER((mre_policy_t *)pPolicy),
                        MRE_POLICY_TYPE((mre_policy_t *)pPolicy),
                        MRE_POLICY_VERSION((mre_policy_t *)pPolicy),
                        pPolicy->elapsed_timeticks );
}

/*-------- policyman_policy_run --------*/
boolean policyman_policy_run(
  void  *pElem,
  void  *pCtx
)
{
  policyman_policy_t  *pPolicy = *((policyman_policy_t **) pElem);
  policy_execute_ctx_t *pPolicyCtx  = (policy_execute_ctx_t *)pCtx;

  pPolicyCtx->pPolicy  = (mre_policy_t *)pPolicy;
  pPolicyCtx->subsId   = POLICY_SUBS(pPolicy);

  policy_execute(pPolicy, pPolicyCtx);

  return TRUE;
}

/*-------- policyman_policy_run_post --------*/
boolean policyman_policy_run_post(
  void  *pElem,
  void  *pCtx
)
{
  policyman_policy_t     *pPolicy = *((policyman_policy_t **) pElem);
  policy_execute_ctx_t   *pPolicyCtx  = (policy_execute_ctx_t *)pCtx;
  sys_modem_as_id_e_type  subs;
  size_t                  nSim;
  policyman_status_t      status;

  status = policyman_get_current_num_sim(&nSim);

  for (subs = SYS_MODEM_AS_ID_1; subs < nSim && POLICYMAN_SUCCEEDED(status); ++subs)
  {
    POLICY_SUBS(pPolicy) = subs;
    pPolicyCtx->pPolicy  = (mre_policy_t *)pPolicy;
    pPolicyCtx->subsId   = subs;

    policy_execute(pPolicy, pPolicyCtx);

    policyman_fullrat_config_evaluate_per_subs(pPolicyCtx, subs, policyman_fullrat_config_get_post);
  }

  POLICY_SUBS(pPolicy) = SYS_MODEM_AS_ID_NONE;

  return TRUE;
}

