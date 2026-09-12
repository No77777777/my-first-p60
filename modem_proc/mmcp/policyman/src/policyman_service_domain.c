/**
  @file policyman_service_domain.c

  @brief
*/

/*
    Copyright (c) 2015-2017,2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_service_domain.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_efs.h"

#include "policyman_cfgitem.h"
#include "policyman_cm.h"
#include "policyman_diag.h"
#include "policyman_efs.h"
#include "policyman_policies.h"
#include "policyman_service_domain.h"
#include "policyman_serving_system.h"
#include "policyman_subs.h"
#include "policyman_util.h"
#include "policyman_task.h"
#include "policyman_xml.h"

#include <stringl/stringl.h>

/*=============================================================================
 Data Structure for Service Domain Info
==============================================================================*/
struct policyman_service_domain_item_t
{
  POLICYMAN_ITEM;

  sys_srv_domain_e_type  service_domain;
};

/*=============================================================================
  Service Domain APIs
============================================================================*/

typedef struct
{
 POLICYMAN_ACTION_BASE;

 sys_srv_domain_e_type service_domain;
} service_domain_action_t;

/*-------- policyman_service_domain_get_default --------*/
policyman_item_t* policyman_service_domain_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
)
{
  return (policyman_item_t *)policyman_service_domain_item_new(SYS_SRV_DOMAIN_CS_PS, subsId);
}

/*-------- policyman_service_domain_clone --------*/
policyman_item_t * policyman_service_domain_clone(
  policyman_item_t const *pItem
)
{
  policyman_service_domain_item_t const *pDomain       = (policyman_service_domain_item_t const *)pItem;
  policyman_service_domain_item_t       *pDomainClone;

  pDomainClone = policyman_service_domain_item_new( pDomain->service_domain,
                                                    pDomain->asubs_id );

  return (policyman_item_t *)pDomainClone;
}

/*-------- policyman_service_domain_compare --------*/
boolean policyman_service_domain_compare(
  policyman_item_t  *pItem1,
  policyman_item_t  *pItem2
)
{
  policyman_service_domain_item_t *pDomain1 = (policyman_service_domain_item_t *) pItem1;
  policyman_service_domain_item_t *pDomain2 = (policyman_service_domain_item_t *) pItem2;

  return pDomain1->service_domain == pDomain2->service_domain;
}

/*-------- policyman_get_service_domain --------*/
policyman_status_t policyman_get_service_domain(
  policyman_item_t const *pItem,
  sys_srv_domain_e_type  *pServiceDomain
)
{
  policyman_service_domain_item_t *pDomain = (policyman_service_domain_item_t *) pItem;

  if (pDomain == NULL || pServiceDomain == NULL)
  {
    POLICYMAN_MSG_ERROR_0("policyman_get_service_domain returned error POLICYMAN_STATUS_ERR_INVALID_ARGS");
    return POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }
  if (policyman_cfgitem_id(pItem) != POLICYMAN_ITEM_SERVICE_DOMAIN)
  {
    POLICYMAN_MSG_ERROR_0("policyman_get_service_domain returned error POLICYMAN_STATUS_ERR_INVALID_ITEM_ID");
    return POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
  }

  *pServiceDomain = pDomain->service_domain;

  POLICYMAN_CL_MSG_LOW_4( SUBS_PREFIX "get_service_domain (from item 0x%08x, seqno %d) returned service_domain %d",
                          pItem->asubs_id,
                          pItem,
                          pItem->seqno,
                          pDomain->service_domain );

  return POLICYMAN_STATUS_SUCCESS;
}

/*-------- policyman_str_to_service_domain --------*/
sys_srv_domain_e_type policyman_str_to_service_domain(
  char const  *pStr
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("CS",    SYS_SRV_DOMAIN_CS_ONLY),
    MRE_STR_VALUE_ENTRY("PS",    SYS_SRV_DOMAIN_PS_ONLY),
    MRE_STR_VALUE_ENTRY("CSPS",  SYS_SRV_DOMAIN_CS_PS),
    MRE_STR_VALUE_ENTRY("CAMP",  SYS_SRV_DOMAIN_CAMPED),
  };

  char                  token[32];
  sys_srv_domain_e_type domain = SYS_SRV_DOMAIN_NONE;

  if (policyman_get_token(&pStr, token, sizeof(token)))
  {
    mre_status_t  status;
    
    MRE_LOOKUP_STR_VALUE(map, token, sys_srv_domain_e_type, &domain);
    if (MRE_FAILED(status))
    {
      POLICYMAN_UT_MSG_1("Invalid value for <sys_domain_pref>: %s", token);
    }
  }

  return domain;
}

/*-------- policyman_get_service_domain_for_subs --------*/
policyman_status_t policyman_get_service_domain_for_subs(
  sys_modem_as_id_e_type  subsId,
  sys_srv_domain_e_type  *pServiceDomain
)
{
  policyman_status_t      status = POLICYMAN_STATUS_ERR_NOT_PRESENT;
  policyman_item_t const *pItem;

  pItem = policyman_cfgitem_get_item(POLICYMAN_ITEM_SERVICE_DOMAIN, subsId);

  if (pItem != NULL)
  {
    status = policyman_get_service_domain(pItem, pServiceDomain);
  }

  policyman_item_release(pItem);
  return status;
}

/*-------- policyman_service_domain_display --------*/
void policyman_service_domain_display(
  policyman_item_t *pItem
)
{
  policyman_service_domain_item_t *pDomain = (policyman_service_domain_item_t *)pItem;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "service domain %d",
                        pDomain->asubs_id,
                        pDomain->service_domain );
}

/*-------- policyman_service_domain_item_new --------*/
policyman_service_domain_item_t * policyman_service_domain_item_new(
  sys_srv_domain_e_type  service_domain,
  sys_modem_as_id_e_type subsId
)
{
  policyman_service_domain_item_t  *pItem;

  pItem = POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_SERVICE_DOMAIN,
                                 policyman_service_domain_item_t,
                                 policyman_simple_dtor,
                                 subsId );

  pItem->service_domain = service_domain;

  return pItem;
}


/*-------- execute_service_domain --------*/
static boolean execute_service_domain(
  policyman_action_t const *pAction,
  void                     *pCtx,
  sys_modem_as_id_e_type    subsId
)
{
  service_domain_action_t         *pServiceDomainAction = (service_domain_action_t *)pAction;
  mre_set_t                       *pItemSet             = POLICY_EXECUTE_ITEMSET(pCtx);
  policyman_service_domain_item_t *pSrvDomain;

  pSrvDomain = policyman_service_domain_item_new(pServiceDomainAction->service_domain, subsId);
  mre_set_replace(pItemSet, &pSrvDomain);

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "action <service_domain> set to %d",
                        pSrvDomain->service_domain,
                        subsId );

  ref_cnt_obj_release(pSrvDomain);

  return TRUE;
}


/*-------- policyman_service_domain_execute --------*/
static boolean policyman_service_domain_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_service_domain);
}


/*-------- policyman_service_domain_create_new_action --------*/
mre_status_t policyman_service_domain_create_new_action(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  mre_status_t             status         = MRE_STATUS_ERR_INVALID_ACTION;
  service_domain_action_t *pAction        = NULL;
  policyman_subs_t         pm_subs        = SUBS_THIS;
  char const              *pStr;
  sys_srv_domain_e_type    serviceDomain;

  pStr = policyman_xml_get_text(pElem);
  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_0("<service_domain>: requires a service domain");
    goto Done;
  }

  serviceDomain = policyman_str_to_service_domain(pStr);
  if (serviceDomain == SYS_SRV_DOMAIN_NONE)
  {
    goto Done;
  }

  /* Does this action indicate the subscription?
   * Default is subscription from policy. */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pAction = POLICYMAN_ACTION_NEW( service_domain_action_t,
                                  pPolicy,
                                  policyman_service_domain_execute,
                                  policyman_action_dtor,
                                  pm_subs,
                                  pElem );

  pAction->service_domain = serviceDomain;

  *ppAction = (mre_action_t *)pAction;

  status = MRE_STATUS_SUCCESS;

Done:
  POLICYMAN_MSG_HIGH_1( SUBS_PREFIX "action_new <service_domain> returned status %d", status );
  return status;
}

/*-----------------------------------------------------------------------------
  Service Domain Pref
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  sys_srv_domain_e_type service_domain_pref;
} policyman_service_domain_pref_condition_t;

/*-------- evaluate_service_domain_has --------*/
STATIC boolean evaluate_service_domain_has(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_service_domain_pref_condition_t const *pCond    = (policyman_service_domain_pref_condition_t *) pCondition;
  policyman_state_t                               *pState   = POLICY_EXECUTE_STATE(pCtx);
  boolean                                          result   = FALSE;

  result = policyman_serving_system_service_domain_has(pState, subsId, pCond->service_domain_pref);

  POLICYMAN_UT_MSG_1("condition <service_domain_has> returns %d", result);
  return result;
}


/*-------- policyman_condition_service_domain_has_evaluate --------*/
STATIC boolean policyman_condition_service_domain_has_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const *pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_service_domain_has);
}


/*-------- policyman_condition_service_domain_has_new --------*/
mre_status_t policyman_condition_service_domain_has_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                               status      = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_subs_t                           pm_subs     = SUBS_THIS;
  policyman_service_domain_pref_condition_t *pCondition;
  char const                                *pStr;
  sys_srv_domain_e_type                      domainPref;

  pStr = policyman_xml_get_text(pElem);
  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_0("<service_domain_has>: must specify a domain preference");
    goto Done;
  }

  /* Does this action indicate the subscription
   * Default is subscription from policy. */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs );
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  domainPref= policyman_str_to_service_domain(pStr);
  if (domainPref == SYS_SRV_DOMAIN_NONE)
  {
    POLICYMAN_UT_MSG_0("<service_domain_has>: invalid domain preferences");
    goto Done;
  }

  pCondition = POLICYMAN_CONDITION_NEW( policyman_service_domain_pref_condition_t,
                                        pPolicy,
                                        policyman_condition_service_domain_has_evaluate,
                                        policyman_condition_dtor,
                                        pm_subs,
                                        pElem );

  pCondition->service_domain_pref = domainPref;

  *ppCondition = (mre_condition_t *)pCondition;

  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}

/*-------- policyman_service_domain_mdump --------*/
QSH_MDUMP_FN_ATTR_PM void policyman_service_domain_mdump(
  policyman_item_t const *pItem
)
{
  policyman_qsh_dump_collect((void *)pItem, sizeof(policyman_service_domain_item_t));
}
