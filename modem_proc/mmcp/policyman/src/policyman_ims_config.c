/**
  @file policyman_ims_config.c

  @brief Configuration item for IMS status.
*/

/*
    Copyright (c) 2016-2018,2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_ims_config.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_efs.h"
#include "mre_util.h"

#include "policyman_cfgitem.h"
#include "policyman_cm.h"
#include "policyman_device_config.h"
#include "policyman_diag.h"
#include "policyman_efs.h"
#include "policyman_ims_config.h"
#include "policyman_policies.h"
#include "policyman_serving_system.h"
#include "policyman_subs.h"
#include "policyman_util.h"
#include "policyman_task.h"
#include "policyman_xml.h"

#include <stringl/stringl.h>

/*=============================================================================
 Data Structure for IMS Config Item Info
==============================================================================*/
struct policyman_ims_config_item_t
{
  POLICYMAN_ITEM;

  size_t  maxInstances;
  boolean imsAllowed;
};

/*=============================================================================
  IMS Status APIs
============================================================================*/

typedef struct
{
 POLICYMAN_ACTION_BASE;

 boolean imsAllowed;
} ims_config_action_t;

/*-------- policyman_ims_config_check_parameters --------*/
STATIC policyman_status_t policyman_ims_config_check_parameters (
  policyman_ims_config_item_t *pItem,
  void                        *pParam
)
{
  policyman_status_t  status = POLICYMAN_STATUS_SUCCESS;
  policyman_item_id_t pItemId;

  if (pItem == NULL || pParam == NULL)
  {
    POLICYMAN_MSG_ERROR_0("Invalid parameter passed");
    status = POLICYMAN_STATUS_ERR_INVALID_ARGS;
    goto Done;
  }

  pItemId = policyman_cfgitem_id((policyman_item_t const *)pItem);
  if (pItemId != POLICYMAN_ITEM_IMS_CONFIG)
  {
    POLICYMAN_MSG_ERROR_2("Wrong policy item passed. Expected Item ID %d, Received Item ID %d", 
                          POLICYMAN_ITEM_IMS_CONFIG, 
                          pItemId);

    status = POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
  }

Done:
  return status;
}


/*-------- policyman_ims_config_get_max_ims_instances --------*/
policyman_status_t policyman_ims_config_get_max_ims_instances(
  policyman_item_t const *pItem,
  size_t                 *pMaxInstances
)
{
  policyman_ims_config_item_t *pImsConfig      = (policyman_ims_config_item_t *)pItem;
  policyman_status_t           status;

  status = policyman_ims_config_check_parameters(pImsConfig, pMaxInstances);
  if(POLICYMAN_SUCCEEDED(status))
  {
    *pMaxInstances = pImsConfig->maxInstances;

  POLICYMAN_CL_MSG_HIGH_2( SUBS_PREFIX "policyman_ims_config_get_max_ims_instances returned max_instances %d",
                           pItem->asubs_id,
                           *pMaxInstances );
  }

  return status;
}


/*-------- policyman_ims_config_get_ims_allowed --------*/
policyman_status_t policyman_ims_config_get_ims_allowed(
  policyman_item_t const *pItem,
  boolean                *pImsIsAllowed
)
{
  policyman_ims_config_item_t *pImsConfig = (policyman_ims_config_item_t *)pItem;
  policyman_status_t           status;

  status = policyman_ims_config_check_parameters(pImsConfig, pImsIsAllowed);
  if (POLICYMAN_SUCCEEDED(status))
  {
    *pImsIsAllowed = pImsConfig->imsAllowed;

    POLICYMAN_CL_MSG_HIGH_2( SUBS_PREFIX "policyman_ims_config_get_ims_allowed returned ims_allowed %d",
                             pItem->asubs_id,
                             pImsConfig->imsAllowed );
  }

  return status;
}

/*-------- policyman_get_ims_config_for_subs --------*/
policyman_status_t policyman_get_ims_config_for_subs(
  sys_modem_as_id_e_type  subsId,
  boolean                *pImsConfig
)
{
  size_t                nItems;
  policyman_status_t    status     = POLICYMAN_STATUS_ERR;
  policyman_item_t     *pImsItem   = NULL;
  policyman_item_id_t   id[]       = { POLICYMAN_ITEM_IMS_CONFIG };

  nItems = policyman_cfgitem_get_items_per_subs( (policyman_item_id_t const *)&id,
                                                 ARR_SIZE(id),
                                                 (policyman_item_t const **)&pImsItem,
                                                 subsId );

  if (nItems == ARR_SIZE(id))
  {
    status = policyman_ims_config_get_ims_allowed(pImsItem, pImsConfig);
  }

  MRE_RELEASE_IF(pImsItem);
  return status;
}

/*-------- policyman_ims_config_display --------*/
void policyman_ims_config_display(
  policyman_item_t  *pItem
)
{
  policyman_ims_config_item_t *pImsConfig = (policyman_ims_config_item_t *)pItem;


  POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "ims_allowed %d max_instances %d (item 0x%08x, seqno %d)",
                        pImsConfig->asubs_id,
                        pImsConfig->imsAllowed,
                        pImsConfig->maxInstances,
                        pItem,
                        pItem->seqno );
}

/*-------- policyman_ims_config_compare --------*/
boolean policyman_ims_config_compare(
  policyman_item_t  *pItem1,
  policyman_item_t  *pItem2
)
{
  policyman_ims_config_item_t *pImsConfig1 = (policyman_ims_config_item_t *) pItem1;
  policyman_ims_config_item_t *pImsConfig2 = (policyman_ims_config_item_t *) pItem2;

  return ( (pImsConfig1->imsAllowed == pImsConfig2->imsAllowed)
           && (pImsConfig1->maxInstances == pImsConfig2->maxInstances) );
}

/*-------- update_ims_allowed --------*/
STATIC void update_ims_allowed(
  policyman_state_t           *pState,
  mre_set_t                   *pItemSet,
  policyman_ims_config_item_t *pImsItem
)
{
  boolean subIsActive         = FALSE;
  boolean subphoneSupportsLte = FALSE;
  boolean imsOnlyFromXml;

  imsOnlyFromXml        = policyman_get_subs_boolean_with_default(PM_IMS_ONLY_FROM_XML, SYS_MODEM_AS_ID_NONE, FALSE);
  if (!imsOnlyFromXml)
  {
    subIsActive           = policyman_is_cm_subs_active(pState, pImsItem->asubs_id);
    subphoneSupportsLte   = policyman_device_config_lte_is_present(pState, pItemSet, pImsItem->asubs_id);

    pImsItem->imsAllowed = TRUE;

    if (!subIsActive || !subphoneSupportsLte)
    {
      pImsItem->imsAllowed = FALSE;
    }
  }

  POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "IMS Allowed %d (sub is active %d, subphone has LTE %d IMS only from XML %d)",
                        pImsItem->asubs_id,
                        pImsItem->imsAllowed,
                        subIsActive,
                        subphoneSupportsLte,
                        imsOnlyFromXml );
}

/*-------- policyman_ims_config_clone_db --------*/
STATIC policyman_ims_config_item_t* policyman_ims_config_clone_db(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_ims_config_item_t *pImsConfig   = NULL;
  boolean                      imsIsAllowed = FALSE;
  policyman_status_t           status;

  status = policyman_get_ims_config_for_subs(subsId, &imsIsAllowed);
  if (POLICYMAN_SUCCEEDED(status))
  {
    pImsConfig = policyman_ims_config_item_new(pItemSet, subsId, imsIsAllowed);
  }

  return pImsConfig;
}


/*-------- policyman_ims_config_item_clone --------*/
policyman_item_t * policyman_ims_config_item_clone(
  policyman_item_t const *pItem
)
{
  policyman_ims_config_item_t const *pImsConfig       = (policyman_ims_config_item_t const *)pItem;
  policyman_ims_config_item_t       *pImsConfigClone;

  pImsConfigClone = POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_IMS_CONFIG,
                                           policyman_ims_config_item_t,
                                           policyman_simple_dtor,
                                           pImsConfig->asubs_id );

  pImsConfigClone->maxInstances = pImsConfig->maxInstances;
  pImsConfigClone->imsAllowed   = pImsConfig->imsAllowed;
  pImsConfigClone->asubs_id     = pImsConfig->asubs_id;

  return (policyman_item_t *)pImsConfigClone;
}


/*-------- policyman_ims_config_ensure_items_for_subs --------*/
void policyman_ims_config_ensure_items_for_subs(
  policyman_state_t *pState,
  mre_set_t         *pItemSet
)
{
  size_t             nSim;
  size_t             subsId;
  policyman_status_t status;

  status = policyman_get_current_num_sim(&nSim);

  for (subsId = SYS_MODEM_AS_ID_1; POLICYMAN_SUCCEEDED(status) && subsId < nSim; subsId++)
  {
    policyman_ims_config_item_t *pImsItem        = NULL;
    boolean                      item_is_present;

    item_is_present = policyman_cfgitem_is_present_in_itemset( pItemSet,
                                                               POLICYMAN_ITEM_IMS_CONFIG,
                                                               subsId,
                                                               (policyman_item_t**)&pImsItem);

    if (!item_is_present)
    {
      // Get IMS allowed field and add to set
      pImsItem = policyman_ims_config_clone_db(pItemSet, subsId);
    }

    // Ensure Item exists.
    if (NON_NULL(pImsItem))
    {
      update_ims_allowed(pState, pItemSet, pImsItem);

      // Add to itemset only when item is not present
      if (!item_is_present)
      {
        mre_set_add(pItemSet, &pImsItem);
      }

      ref_cnt_obj_release(pImsItem);
    }
  }
}

/*-------- policyman_ims_config_get_default --------*/
policyman_item_t* policyman_ims_config_get_default(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_ims_config_item_t *pImsConfig;

  pImsConfig = policyman_ims_config_item_new(pItemSet, subsId, FALSE);

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "Default IMS allowed %d",
                        pImsConfig->asubs_id,
                        pImsConfig->imsAllowed );

  return (policyman_item_t *)pImsConfig;
}

/*-------- policyman_ims_config_item_new --------*/
policyman_ims_config_item_t* policyman_ims_config_item_new(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  boolean                 imsAllowed
)
{
  policyman_ims_config_item_t *pItem;
  policyman_state_t           *pState;
  boolean                      lteIsOnAllSubs = FALSE;

  pItem = POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_IMS_CONFIG,
                                 policyman_ims_config_item_t,
                                 policyman_simple_dtor,
                                 subsId );

  pState = policyman_state_get_state();

  lteIsOnAllSubs = policyman_device_config_lte_is_on_multiple_subs(pState, pItemSet);

  pItem->maxInstances = lteIsOnAllSubs ? 2 : 1;
  pItem->imsAllowed = imsAllowed;
  pItem->asubs_id   = subsId;

  return pItem;
}

/*-------- execute_ims_config --------*/
STATIC boolean execute_ims_config(
  policyman_action_t const *pAction,
  void                     *pCtx,
  sys_modem_as_id_e_type    subsId
)
{
  ims_config_action_t         *pImsConfigAction  = (ims_config_action_t *)pAction;
  mre_set_t                   *pItemSet     = POLICY_EXECUTE_ITEMSET(pCtx);
  policyman_ims_config_item_t *pImsConfig;

  pImsConfig = policyman_ims_config_item_new(pItemSet, subsId, pImsConfigAction->imsAllowed);
  mre_set_replace(pItemSet, &pImsConfig);

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX SRC_LINE "action <ims_config> set IMS allowed to %d",
                        subsId,
                        LINE_NUM(pImsConfigAction),
                        pImsConfig->imsAllowed );

  ref_cnt_obj_release(pImsConfig);

  return TRUE;
}

/*-------- policyman_ims_config_execute --------*/
STATIC boolean policyman_ims_config_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_ims_config);
}

/*-------- policyman_ims_config_action_new --------*/
mre_status_t policyman_ims_config_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  ims_config_action_t *pAction      = NULL;
  char const          *pStr         = NULL;
  policyman_subs_t     pm_subs      = SUBS_THIS;
  boolean              imsAllowed   = FALSE;
  mre_status_t         status       = MRE_STATUS_ERR_INVALID_ACTION;

  pStr = mre_xml_get_attribute(pElem, "allowed");
  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<ims_config>: 'allowed' attribute required.", POLICY_SUBS(pPolicy));
    goto Done;
  }

  status = mre_str_to_boolean(pStr, &imsAllowed);
  if (MRE_FAILED(status))
  {
    MRE_UT_MSG_1(SUBS_PREFIX "<ims_config> valid values for 'allowed' attribute are 'true' and 'false'", POLICY_SUBS(pPolicy));
    goto Done;
  }

  /* Does this action indicate the subscription?
   * Default is subscription from policy. */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pAction = POLICYMAN_ACTION_NEW( ims_config_action_t,
                                  pPolicy,
                                  policyman_ims_config_execute,
                                  policyman_action_dtor,
                                  pm_subs,
                                  pElem );

  pAction->imsAllowed = imsAllowed;

  *ppAction = (mre_action_t *) pAction;

  status = MRE_STATUS_SUCCESS;

Done:
  POLICYMAN_MSG_HIGH_2(SUBS_PREFIX "action_new <ims_config> returned status %d", pm_subs, status);
  return status;
}

/*-------- policyman_ims_config_mdump --------*/
QSH_MDUMP_FN_ATTR_PM void policyman_ims_config_mdump(
  policyman_item_t const *pItem
)
{
  policyman_qsh_dump_collect((void *)pItem, sizeof(policyman_ims_config_item_t));
}

