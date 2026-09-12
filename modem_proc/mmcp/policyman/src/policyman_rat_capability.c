/**
  @file policy_rat_capability.c

  @brief  Policy to modify the RAT capability of the device based on the IIN
          of the current SIM.
*/

/*
    Copyright (c) 2013-2018,2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_rat_capability.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_efs.h"

#include "policyman_rat_capability.h"
#include "policyman_cfgitem.h"
#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_diag.h"
#include "policyman_efs.h"
#include "policyman_i.h"
#include "policyman_lte_i.h"
#include "policyman_policies.h"
#include "policyman_state.h"
#include "policyman_subs.h"
#include "policyman_svc_mode.h"
#include "policyman_task.h"
#include "policyman_ue_mode.h"
#include "policyman_uim.h"
#include "policyman_util.h"
#include "policyman_xml.h"

#include "event.h"
#include "sys.h"
#include <stringl/stringl.h>


#define RAT_CAPABILITY_TAG         "rat_capability"
#define RAT_CAPABILITY_IF_TAG      "rat_capability_if"

typedef struct
{
  BASE_EXECUTE_CTX;

  sys_modem_as_id_e_type  cdmaSubs;
  sys_modem_as_id_e_type  ddsSubs;
  boolean                includeHdrOnDds;
  boolean                includeTdsOnDds;
  boolean                excludeLteForTempDds;
} rat_concurrency_ctx_t;


/*=============================================================================
  Rat capability methods for cfgitem
=============================================================================*/

/*-------- policyman_rat_capability_item_new --------*/
policyman_item_t* policyman_rat_capability_item_new(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  uint32                  mask
)
{
  rat_capability_t *pRatItem;
  uint32            subphoneMask;

  pRatItem = POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_RAT_CAPABILITY,
                                    rat_capability_t,
                                    mre_simple_dtor,
                                    subsId );

  // Filter the RATs based on subphone capabilities.
  subphoneMask = policyman_rat_capability_get_max_config(pItemSet, subsId);
  if (subphoneMask == 0)
  {
    subphoneMask = policyman_rf_get_overall_rat_mask();
  }

  pRatItem->ratMask     = mask;
  pRatItem->subphoneCap = subphoneMask;

  return (policyman_item_t *) pRatItem;
}


/*-------- policyman_rat_capability_get_default_from_efs --------*/
policyman_status_t policyman_rat_capability_get_default_from_efs(
  cfgitem_execute_ctx_t *pCfgCtx
)
{
  policyman_item_t         *pRatItem;
  uint32                    ratMask;
  policyman_status_t        status     = POLICYMAN_STATUS_ERR;

  // read EFS for RAT/BAND capability
  if (policyman_retrieve_rats_bands(pCfgCtx->subsId, &ratMask, NULL, NULL))
  {
    status         = POLICYMAN_STATUS_SUCCESS;
    pCfgCtx->pItem = policyman_rat_capability_item_new(pCfgCtx->pItemSet, pCfgCtx->subsId, ratMask);
  }

  return status;
}


/*-------- policyman_rat_capability_get_default --------*/
policyman_item_t * policyman_rat_capability_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
)
{
  uint32                mask;
  rat_capability_t *pRatItem;

  mask = policyman_rf_get_overall_rat_mask();

  pRatItem = (rat_capability_t *)policyman_rat_capability_item_new(pItemSet, subsId, mask);

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "Default RAT capability mask 0x%04x",
                        subsId,
                        pRatItem->ratMask );

  return (policyman_item_t *)pRatItem;
}


/*-------- policyman_rat_capability_get_base_rats --------*/
STATIC uint32 policyman_rat_capability_get_base_rats(
  policyman_item_t const *pItem
)
{
  rat_capability_t  *prc  = (rat_capability_t *)pItem;

  return prc->ratMask;
}


/*-------- policyman_rat_capability_get_effective_rats --------*/
STATIC uint32 policyman_rat_capability_get_effective_rats(
  policyman_item_t *pItem
)
{
  uint32             ratCap;
  rat_capability_t  *prc     = (rat_capability_t *)pItem;

  ratCap  = prc->ratMask | prc->ratIncludeMask;
  ratCap &= ~ prc->ratExcludeMask;
  ratCap &= prc->subphoneCap;

  return ratCap;
}


/*-------- policyman_rat_capability_compare --------*/
boolean policyman_rat_capability_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
)
{
  return  (policyman_rat_capability_get_effective_rats(pData1) == policyman_rat_capability_get_effective_rats(pData2));
}

/*-------- policyman_rat_capability_display --------*/
void policyman_rat_capability_display(
  policyman_item_t  *pItem
)
{
  rat_capability_t  *prc = (rat_capability_t *)pItem;

  POLICYMAN_MSG_HIGH_7( SUBS_PREFIX "Rat capability: (from item 0x%08x seqno %d), Rat cap 0x%04x, Base 0x%04x, subphone cap 0x%04x, exclude mask 0x%04x",
                        prc->asubs_id, 
                        pItem,
                        pItem->seqno,                        
                        policyman_rat_capability_get_effective_rats(pItem),
                        prc->ratMask,
                        prc->subphoneCap,
                        prc->ratExcludeMask );
}


/*-------- policyman_rat_capability_on_update --------*/
void policyman_rat_capability_on_update(
  policyman_item_t  *pItem
)
{
  rat_capability_t                *prc           = (rat_capability_t *)pItem;
  policyman_diag_event_rat_cap_t   event_rat_cap;

  event_rat_cap.ratMask  = policyman_rat_capability_get_effective_rats(pItem);
  event_rat_cap.subs     = (int8)prc->asubs_id;

  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Sending EVENT_PM_RAT_CHANGE", prc->asubs_id);

  event_report_payload( EVENT_PM_RAT_CHANGE,
                        sizeof(event_rat_cap),
                        &event_rat_cap );

  return;
}


/*-------- policyman_get_rat_capability_base_for_subs --------*/
STATIC policyman_status_t policyman_get_rat_capability_base_for_subs(
  sys_modem_as_id_e_type  subsId,
  uint32                 *pRatMask
)
{
  size_t                nItems;
  policyman_status_t    status  = POLICYMAN_STATUS_ERR;
  policyman_item_t     *pItem   = NULL;
  policyman_item_id_t   id[]    = { POLICYMAN_ITEM_RAT_CAPABILITY };

  nItems = policyman_cfgitem_get_items_per_subs( (policyman_item_id_t const *)&id,
                                                 ARR_SIZE(id),
                                                 (policyman_item_t const **)&pItem,
                                                 subsId );
  if (nItems == ARR_SIZE(id))
  {
    *pRatMask = policyman_rat_capability_get_base_rats(pItem);
     status  = POLICYMAN_STATUS_SUCCESS;
  }

  MRE_RELEASE_IF(pItem);
  return status;
}

/*-------- policyman_rat_capability_clone_from_db --------*/
STATIC policyman_item_t * policyman_rat_capability_clone_from_db(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  uint32                currentMask = 0;
  policyman_item_t     *pRatItem    = NULL;
  policyman_status_t    status;

  // get current RAT capabilities for subs and create a new object
  status = policyman_get_rat_capability_base_for_subs(subsId, &currentMask);
  if (POLICYMAN_SUCCEEDED(status))
  {
    pRatItem = policyman_rat_capability_item_new(pItemSet, subsId, currentMask);
  }

  return pRatItem;
}


/*-------- policyman_rat_capability_clone --------*/
policyman_item_t * policyman_rat_capability_clone(
  policyman_item_t const *pItem
)
{
  rat_capability_t const *pRatItem       = (rat_capability_t const *)pItem;
  rat_capability_t       *pRatItemClone;

  pRatItemClone = POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_RAT_CAPABILITY,
                                         rat_capability_t,
                                         mre_simple_dtor,
                                         pRatItem->asubs_id );

  *pRatItemClone = *pRatItem;

  // Only one reference on this object
  pRatItemClone->_rcobj_.ref_count.value = 1;

  return (policyman_item_t *)pRatItemClone;
}


/*-------- policyman_ensure_rat_capability_for_each_subs --------*/
STATIC void policyman_ensure_rat_capability_for_each_subs(
  rat_concurrency_ctx_t *pCtx
)
{
  size_t             nSim;
  size_t             subsId;
  policyman_status_t status;
  boolean            item_is_present;

  status = policyman_get_current_num_sim(&nSim);

  // For each subs add RAT capability to itemset if not present
  for (subsId = SYS_MODEM_AS_ID_1; POLICYMAN_SUCCEEDED(status) && subsId < nSim; subsId++)
  {
    policyman_item_t *pRatItem = NULL; 

    item_is_present  = policyman_cfgitem_is_present_in_itemset( pCtx->pItemSet, 
                                                                POLICYMAN_ITEM_RAT_CAPABILITY, 
                                                                subsId, 
                                                                &pRatItem);

    if (!item_is_present)
    {
      pRatItem = policyman_rat_capability_clone_from_db(pCtx->pItemSet, subsId);

      if (NON_NULL(pRatItem))
      {
        mre_set_add(pCtx->pItemSet, &pRatItem);
      }
    }
    else
    {
      // Update subphone capability with latest device config value
      ((rat_capability_t *)pRatItem)->subphoneCap = policyman_rat_capability_get_max_config(pCtx->pItemSet, subsId);
    }

    REF_CNT_OBJ_RELEASE_IF(pRatItem);
  }
}

/*-------- ue_has_3gpp2_subscription --------*/
STATIC boolean ue_has_3gpp2_subscription(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId
)
{
  cm_rtre_control_e_type  rtre_control;
  boolean                 sim_has_3gpp2_subs;
  boolean                 nv_has_3gpp2_subs;
  boolean                 cdma_allowed_without_subs;
  boolean                 card_is_3gpp2;

  card_is_3gpp2 = policyman_uim_sim_is_3gpp2( pState,
                                              subsId,
                                              &sim_has_3gpp2_subs,
                                              &nv_has_3gpp2_subs );

  cdma_allowed_without_subs = policyman_state_get_1x_allowed_without_subs(pState);

  POLICYMAN_MSG_MED_4( SUBS_PREFIX "card has 3GPP2 (sim %d, NV %d), cdma_allowed_without_subs %d",
                       subsId,
                       sim_has_3gpp2_subs,
                       nv_has_3gpp2_subs,
                       cdma_allowed_without_subs );

  return (card_is_3gpp2 || cdma_allowed_without_subs);
}


/*-------- include_tds_on_this_sub --------*/
STATIC boolean include_tds_on_this_sub(
  uint32                 effectiveMask,
  sys_modem_as_id_e_type subsId
)
{
  return    ((effectiveMask & SYS_SYS_MODE_MASK_TDS) != 0)
         && policyman_get_subs_boolean_with_default(PM_ALLOW_TDS_ON_THIS_SUB,
                                                    subsId,
                                                    FALSE);
}


/*-------- determine_concurrency_parameters --------*/
STATIC boolean determine_concurrency_parameters(
  void  *pElem,
  void  *pCtx
)
{
  policyman_item_t      *pItem    = *((policyman_item_t **) pElem);
  rat_concurrency_ctx_t *pRatCtx  = (rat_concurrency_ctx_t *) pCtx;

  if (policyman_cfgitem_id(pItem) == POLICYMAN_ITEM_RAT_CAPABILITY)
  {   
    policyman_state_t      *pState;
    sys_modem_as_id_e_type  subsId;
    uint32                  effectiveMask;

    subsId        = pItem->asubs_id;
    pState        = pRatCtx->pState;
    effectiveMask = policyman_rat_capability_get_effective_rats(pItem);

    if (pItem->asubs_id == pRatCtx->ddsSubs)
    {
      // HDR present on DDS SUBS only if it is a 3GPP2 card
      pRatCtx->includeHdrOnDds =   ue_has_3gpp2_subscription(pState, subsId)
                                && ((effectiveMask & SYS_SYS_MODE_MASK_HDR) != 0);

      pRatCtx->includeTdsOnDds = include_tds_on_this_sub(effectiveMask, subsId);

      pRatCtx->excludeLteForTempDds =   policyman_get_subs_boolean_with_default(PM_EXCLUDE_LTE_FOR_TEMP_DDS,
                                                                                pRatCtx->ddsSubs,
                                                                                FALSE);


      POLICYMAN_MSG_HIGH_4( SUBS_PREFIX "includeHdrOnDds %d, includeTdsOnDds %d pRatCtx->excludeLteForTempDds %d",
                            subsId,
                            pRatCtx->includeHdrOnDds,
                            pRatCtx->includeTdsOnDds,
                            pRatCtx->excludeLteForTempDds );
    }

    // Save the SUBS ID where CDMA is actually present
    if ( (effectiveMask & SYS_SYS_MODE_MASK_CDMA) != 0
       && ue_has_3gpp2_subscription(pState, subsId)
       )
    {
      pRatCtx->cdmaSubs = pItem->asubs_id;
    }
  }

  return TRUE;
}


/*-------- set_masks_for_non_dds_subs --------*/
STATIC void set_masks_for_non_dds_subs(
  rat_concurrency_ctx_t  *pRatCtx,
  rat_capability_t       *prc
)
{
  boolean embmsEnabled;
  boolean  subCSInvalid;
  uint32   effectiveMask;
  boolean  volteIsEnabled;

  embmsEnabled = policyman_state_embms_is_enabled(pRatCtx->pState, pRatCtx->ddsSubs);
  volteIsEnabled          = policyman_ph_get_volte_is_enabled(pRatCtx->pState, prc->asubs_id);

  prc->ratExcludeMask |= SYS_SYS_MODE_MASK_HDR;

  // remove TDS only for permanent changes to support legacy behavior
  if (policyman_subs_dds_is_permanent(pRatCtx->pState))
  {
    prc->ratExcludeMask |= SYS_SYS_MODE_MASK_TDS;
  }

  if (embmsEnabled)
  {
    if (policyman_state_embms_rat_is_restricted(pRatCtx->pState, SYS_SYS_MODE_LTE, pRatCtx->ddsSubs))
    {
      prc->ratExcludeMask |= SYS_SYS_MODE_MASK_LTE;
    }

    if (policyman_state_embms_rat_is_restricted(pRatCtx->pState, SYS_SYS_MODE_WCDMA, pRatCtx->ddsSubs))
    {
      prc->ratExcludeMask |= SYS_SYS_MODE_MASK_WCDMA;
    }

    if (policyman_state_embms_rat_is_restricted(pRatCtx->pState, SYS_SYS_MODE_CDMA, pRatCtx->ddsSubs))
    {
      prc->ratExcludeMask |= SYS_SYS_MODE_MASK_CDMA;
    }
  }

  if (prc->removeLteWhenNotUseful && !volteIsEnabled)
  {
    prc->ratExcludeMask |= SYS_SYS_MODE_MASK_LTE;
  }

  // remove GW if LTE capable but CS invalid 
  subCSInvalid  = ( policyman_ss_get_sim_state(pRatCtx->pState, prc->asubs_id) == SYS_SIM_STATE_CS_INVALID );
  effectiveMask = policyman_rat_capability_get_effective_rats((policyman_item_t *)prc);

  if ( (effectiveMask & SYS_SYS_MODE_MASK_LTE) != 0
     &&  subCSInvalid)
  {
    prc->ratExcludeMask |= (SYS_SYS_MODE_MASK_GSM | SYS_SYS_MODE_MASK_WCDMA);
  }
}


/*-------- set_masks_for_dds_subs --------*/
STATIC void set_masks_for_dds_subs(
  rat_concurrency_ctx_t  *pRatCtx,
  rat_capability_t       *prc
)
{
  if (pRatCtx->excludeLteForTempDds
      &&(!policyman_subs_dds_is_permanent(pRatCtx->pState)))
  {
     prc->ratExcludeMask |=  SYS_SYS_MODE_MASK_LTE;
  }

  if (!pRatCtx->includeHdrOnDds)
  {
    prc->ratExcludeMask |= SYS_SYS_MODE_MASK_HDR;
  }
  if (!pRatCtx->includeTdsOnDds) 
  {
    prc->ratExcludeMask |= SYS_SYS_MODE_MASK_TDS;
  }

}


/*-------- update_concurrency_masks --------*/
STATIC boolean update_concurrency_masks(
  void  *pElem,
  void  *pCtx
)
{
  policyman_item_t      *pItem   = *((policyman_item_t **)pElem);
  rat_concurrency_ctx_t *pRatCtx = (rat_concurrency_ctx_t *)pCtx;

  // work only on RAT capability item
  if (policyman_cfgitem_id(pItem) == POLICYMAN_ITEM_RAT_CAPABILITY)
  {
    sys_modem_as_id_e_type  ddsSubs = pRatCtx->ddsSubs;
    rat_capability_t       *prc     = (rat_capability_t *)pItem;

    prc->ratIncludeMask = 0;
    prc->ratExcludeMask = 0;

    if (pRatCtx->cdmaSubs == SYS_MODEM_AS_ID_NONE)
    {
      prc->ratExcludeMask |= SYS_SYS_MODE_MASK_CDMA;
    }

    if (pItem->asubs_id == ddsSubs)
    {
      set_masks_for_dds_subs(pRatCtx, prc);
    }
    else
    {
      set_masks_for_non_dds_subs(pRatCtx, prc);
    }
  }

  return TRUE;
}


/*-------- policyman_get_rat_capability_item --------*/
rat_capability_t * policyman_get_rat_capability_item(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  rat_capability_t **ppRatCapItem;
  rat_capability_t  *pRatCapItem;

  ppRatCapItem = (rat_capability_t **)policyman_cfgitem_find_item_in_itemset( pItemSet,
                                                                              POLICYMAN_ITEM_RAT_CAPABILITY,
                                                                              subsId );
  if (NON_NULL(ppRatCapItem) && NON_NULL(*ppRatCapItem))
  {
    pRatCapItem = *ppRatCapItem;
  }
  else
  {
    pRatCapItem = (rat_capability_t *) policyman_cfgitem_get_item( POLICYMAN_ITEM_RAT_CAPABILITY,
                                                                   subsId);
  }

  return pRatCapItem;
}


/*-------- subs_rats_contain --------*/
STATIC boolean subs_rats_contain(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  uint32                  ratMask
)
{
  policyman_item_t  *pItem;
  uint32             effectiveMask = 0;

  pItem = (policyman_item_t *) policyman_get_rat_capability_item(pItemSet, subsId );

  if (NON_NULL(pItem))
  {
    effectiveMask = policyman_rat_capability_get_effective_rats(pItem);
    ref_cnt_obj_release(pItem);
  }

  return (ratMask & effectiveMask) != 0;
}


/*-------- policyman_set_rat_capability_concurrency_masks --------*/
void policyman_set_rat_capability_concurrency_masks(
  policyman_state_t *pState,
  mre_set_t         *pItemSet
)
{
  rat_concurrency_ctx_t ctx;

  ctx.pState    = pState;
  ctx.pItemSet = pItemSet;
  ctx.subsId    = SYS_MODEM_AS_ID_1;
  ctx.cdmaSubs = SYS_MODEM_AS_ID_NONE;
  ctx.ddsSubs   = policyman_subs_get_dds_subs(ctx.pState);

  // ensure itemset has RAT capability for all SUBS
  policyman_ensure_rat_capability_for_each_subs(&ctx);

  // get a context with info on required concurrent RATs on all SUBS
  mre_set_iterate(pItemSet, determine_concurrency_parameters, &ctx);

  // update concurrency masks for all RAT capability items
  mre_set_iterate(pItemSet, update_concurrency_masks, &ctx);
}


/*-----------------------------------------------------------------------------
  rat_capability action
-----------------------------------------------------------------------------*/


typedef struct
{
  POLICYMAN_ACTION_BASE;

  rat_capability_info_t rat_info;
  lookup_method_t       lookup_method;
  boolean               remove_lte_when_not_useful;
} rat_capability_action_t;


#define INVALID_RAT_MASK  0xffffffff


/*-------- policyman_get_rat_capability --------*/
policyman_status_t policyman_get_rat_capability(
  policyman_item_t const  *pItem,
  uint32                  *pRatCfg
)
{
  if (pItem == NULL || pRatCfg == NULL)
  {
    POLICYMAN_MSG_ERROR_0("get_rat_capability returned err POLICYMAN_STATUS_ERR_INVALID_ARGS");
    return POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }

  if (policyman_cfgitem_id(pItem) != POLICYMAN_ITEM_RAT_CAPABILITY)
  {
    POLICYMAN_MSG_ERROR_0("get_rat_capability returned err POLICYMAN_STATUS_ERR_INVALID_ITEM_ID");
    return POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
  }

  *pRatCfg = policyman_rat_capability_get_effective_rats((policyman_item_t *)pItem);

  POLICYMAN_MSG_HIGH_4( SUBS_PREFIX "get_rat_capability (from item 0x%08x, seqno %d) returned 0x%04x",
                        pItem->asubs_id,
                        pItem,
                        pItem->seqno,
                        *pRatCfg );

  return POLICYMAN_STATUS_SUCCESS;
}


/*-------- policyman_get_rat_capability_for_subs --------*/
policyman_status_t policyman_get_rat_capability_for_subs(
  sys_modem_as_id_e_type  subsId,
  uint32                 *pRatMask
)
{
  size_t                nItems;
  policyman_status_t    status  = POLICYMAN_STATUS_ERR;
  policyman_item_t     *pItem   = NULL;
  policyman_item_id_t   id[]    = { POLICYMAN_ITEM_RAT_CAPABILITY };

  nItems = policyman_cfgitem_get_items_per_subs( (policyman_item_id_t const *)&id,
                                                 ARR_SIZE(id),
                                                 (policyman_item_t const **)&pItem,
                                                 subsId );
  if (nItems == ARR_SIZE(id))
  {
    status = policyman_get_rat_capability(pItem, pRatMask);
  }

  MRE_RELEASE_IF(pItem);
  return status;
}


/*-------- policyman_rat_capability_str_to_rat --------*/
uint32 policyman_rat_capability_str_to_rat(
  const char  *pRatName
)
{
  uint32   mask;

  mask = policyman_util_str_to_rat_mask(pRatName);

  return (0 == mask)? INVALID_RAT_MASK : mask;
}


/*-------- policyman_rat_config_parse_rats --------*/
boolean policyman_rat_config_parse_rats(
  char const  *pStr,
  uint32      *pMask
)
{
  char    token[32];
  boolean succeeded = TRUE;
  uint32  mask;

  while (policyman_get_token(&pStr, token, sizeof(token)))
  {
    mask = policyman_rat_capability_str_to_rat(token);
    if (mask == INVALID_RAT_MASK)
    {
      POLICYMAN_UT_MSG_1("invalid RAT specified: %s", token);
      succeeded = FALSE;
      break;
    }
    *pMask |= mask;
  }

  return succeeded;
}


/*-------- policyman_rat_capability_read_info --------*/
policyman_status_t policyman_rat_capability_read_info(
  mre_xml_element_t const       *pElem,
  rat_capability_info_t         *pInfo
)
{
  policyman_status_t status    = POLICYMAN_STATUS_ERR_INVALID_ACTION;
  size_t             nChildren;
  size_t             i;

  pInfo->base = POLICYMAN_BASE_NONE;
  pInfo->includeMask = 0;
  pInfo->excludeMask = 0;

  if (!policyman_util_get_base(pElem, &pInfo->base))        { goto Done; }

  nChildren = policyman_xml_num_children(pElem);
  if (nChildren == 0)
  {
    status = POLICYMAN_STATUS_SUCCESS;
    goto Done;
  }

  for (i = 0 ; i < nChildren ; ++i)
  {
    mre_xml_element_t const       *pChild;
    char const                    *pStr;

    pChild = policyman_xml_get_child(pElem, i);
    if (pChild == NULL)
    {
      POLICYMAN_MSG_ERROR_1("Internal error - no element #%d", i);
      goto Done;
    }

    pStr = policyman_xml_get_text(pChild);
    if      (policyman_xml_tag_is(pChild, "include"))
    {
      if (!policyman_rat_config_parse_rats(pStr, &pInfo->includeMask)) { goto Done; }
    }
    else if (policyman_xml_tag_is(pChild, "exclude"))
    {
      if (!policyman_rat_config_parse_rats(pStr, &pInfo->excludeMask)) { goto Done; }
    }
    else
    {
      goto Done;
    }
  }

  status = POLICYMAN_STATUS_SUCCESS;

Done:
  return status;
}

/*-------- policyman_rat_capability_get_max_config --------*/
uint32 policyman_rat_capability_get_max_config(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  uint32                   config_mask = 0;
  device_configuration_t  *pDcItem    = NULL;

  pDcItem = policyman_get_device_config(pItemSet);
  
  if (NULL != pDcItem)
  {
    config_mask = device_config_get_current_config(pDcItem, (size_t)subsId);
    ref_cnt_obj_release(pDcItem);
  }
  else
  {
    policyman_cfgitem_set_limited_capabilities(POLICYMAN_CFG_ERROR_NO_DEVICE_XML);
  }

  return config_mask;
}


/*-------- policyman_rat_capability_get_current_config --------*/
uint32 policyman_rat_capability_get_current_config(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  uint32             config_mask = 0;
  rat_capability_t  *pRatCapItem;

  pRatCapItem = policyman_get_rat_capability_item(pItemSet, subsId);

  if (NON_NULL(pRatCapItem))
  {
    policyman_status_t status;

    status = policyman_get_rat_capability((policyman_item_t *)pRatCapItem, &config_mask);
    ref_cnt_obj_release(pRatCapItem);

    if (POLICYMAN_SUCCEEDED(status))
    {
      POLICYMAN_MSG_MED_2(SUBS_PREFIX "Using base of current rats %d", subsId, config_mask);
    }
    else
    {
      POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "No rat capability item found for base=current", subsId);
    }
  }

  return config_mask;
}


/*-------- policyman_rat_capability_evaluate --------*/
uint32 policyman_rat_capability_evaluate(
  rat_capability_info_t  *pInfo,
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  uint32  mask = 0;

  switch (pInfo->base)
  {
    case POLICYMAN_BASE_NONE:
      mask = 0;
      break;

    case POLICYMAN_BASE_CONFIG:
      mask = policyman_rat_capability_get_max_config(pItemSet, subsId);
      break;

    case POLICYMAN_BASE_HARDWARE:
      mask = policyman_rf_get_overall_rat_mask();
      break;

    case POLICYMAN_BASE_CURRENT:
      mask = policyman_rat_capability_get_current_config(pItemSet, subsId);
      break;

    default:
      POLICYMAN_MSG_ERROR_1("invalid base type: %d", pInfo->base);
      break;
  }

  mask |= pInfo->includeMask;
  mask &= ~(pInfo->excludeMask);

  return mask;
}


/*-------- execute_rat_capability --------*/
STATIC boolean execute_rat_capability(
  policyman_action_t const *pAction,
  void                     *pCtx,
  sys_modem_as_id_e_type    subsId
)
{
  rat_capability_action_t *pRatAction = (rat_capability_action_t *) pAction;
  mre_set_t               *pItemSet   = POLICY_EXECUTE_ITEMSET(pCtx);
  rat_capability_t        *pRatItem;
  uint32                   mask;

  mask = policyman_rat_capability_evaluate(&pRatAction->rat_info, pItemSet, subsId);

  /* if none rat capability is attempted, enable GSM */
  if (mask == SYS_SYS_MODE_MASK_NONE)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "<rat_capability> none attempted, action not taken", subsId);
    return TRUE;
  }

  pRatItem = (rat_capability_t *)policyman_rat_capability_item_new(pItemSet, subsId, mask);
  pRatItem->removeLteWhenNotUseful = pRatAction->remove_lte_when_not_useful;

  mre_set_replace(pItemSet, &pRatItem);

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX SRC_LINE "action <rat_capability> set Base to 0x%04x",
                        subsId,
                        LINE_NUM(pRatAction),
                        pRatItem->ratMask );

  ref_cnt_obj_release(pRatItem);
  return TRUE;
}


/*-------- policyman_rat_capability_execute --------*/
STATIC boolean policyman_rat_capability_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_rat_capability);
}


/*-------- execute_rat_capability_if --------*/
STATIC boolean execute_rat_capability_if(
  policyman_action_t const       *pAction,
  void                           *pCtx,
  sys_modem_as_id_e_type          subsId
)
{
  rat_capability_action_t *pratAction       = (rat_capability_action_t *)pAction;
  mre_set_t               *pItemSet         = POLICY_EXECUTE_ITEMSET(pCtx);
  boolean                  succeeded        = TRUE;
  boolean                  item_is_present;

  item_is_present = policyman_cfgitem_item_is_present_based_on_lookup_method( pratAction->lookup_method,
                                                                              pItemSet,
                                                                              POLICYMAN_ITEM_RAT_CAPABILITY,
                                                                              subsId );

  if (item_is_present)
  {
    POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "<rat_capability_if pmsubs=%d> skipping update - item present",
                        subsId,
                        LINE_NUM(pratAction),
                        pratAction->pmSubsId );
  }
  else
  {
    succeeded = policyman_rat_capability_execute((mre_action_t const  *)pAction, pCtx);
  }

  return succeeded;
}

/*-------- policyman_rat_capability_if_execute --------*/
STATIC boolean policyman_rat_capability_if_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  return policyman_execute_action((policyman_action_t const *)pAction, pCtx, execute_rat_capability_if);
}


/*-------- policyman_rat_capability_create_new_action --------*/
mre_status_t policyman_rat_capability_create_new_action(
  mre_xml_element_t const  *pElem,
  policyman_policy_t             *pPolicy,
  mre_action_t                  **ppAction,
  pfn_execute_t                   executefn
)
{
  mre_status_t             status  = MRE_STATUS_ERR_INVALID_ACTION;
  rat_capability_action_t *pAction = NULL;
  policyman_subs_t         pm_subs;
  char const              *pStr;
  char const              *pRemoveLteWhenNotUseful;

  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pAction = POLICYMAN_ACTION_NEW( rat_capability_action_t,
                                  (mre_policy_t *)pPolicy,
                                  executefn,
                                  policyman_action_dtor,
                                  pm_subs,
                                  pElem );

  status = policyman_rat_capability_read_info(pElem, &pAction->rat_info);

  // Search method lookup
  pStr = policyman_xml_get_attribute(pElem, "method");
  pAction->lookup_method = policyman_util_str_to_lookup_method(pStr);

  pRemoveLteWhenNotUseful = policyman_xml_get_attribute(pElem, "remove_lte_when_not_useful");

  if (pRemoveLteWhenNotUseful == NULL || strcmp(pRemoveLteWhenNotUseful, "false") == 0)
  {
    pAction->remove_lte_when_not_useful = FALSE;
  }
  else
  {
    pAction->remove_lte_when_not_useful = TRUE;
  }

Done:
  if (MRE_SUCCEEDED(status))
  {
    *ppAction = (mre_action_t *) pAction;
  }
  else
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<rat_capability>: invalid XML", POLICY_SUBS(pPolicy));
    MRE_RELEASE_IF(pAction);
  }

  return status;
}


/*-------- policyman_rat_capability_action_new --------*/
mre_status_t policyman_rat_capability_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  return policyman_rat_capability_create_new_action( pElem,
                                                     (policyman_policy_t *) pPolicy,
                                                     ppAction,
                                                     policyman_rat_capability_execute );
}

/*-------- policyman_rat_capability_if_action_new --------*/
mre_status_t policyman_rat_capability_if_action_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
 return policyman_rat_capability_create_new_action( pElem,
                                                    (policyman_policy_t *) pPolicy,
                                                    ppAction,
                                                    policyman_rat_capability_if_execute );
}

typedef struct
{
  POLICYMAN_CMD_HDR;

  sys_ue_mode_e_type    ue_mode;
  policyman_svc_mode_t  svc_mode;
} policyman_full_rat_mode_cmd_t;


/*-------- policyman_process_original_rats_bands_efs --------*/
void policyman_process_original_rats_bands_efs(
  rat_capability_efs_t      *pBuffer,
  uint32                    *pRatMask,
  rf_efs_bands_t            *pRetrievedBands
)
{
  if (NON_NULL(pRatMask))
  {
    *pRatMask = pBuffer->rat_mask;
  }
  
  if (NON_NULL(pRetrievedBands))
  {
    pRetrievedBands->bands.gwBands  = pBuffer->gw_band_mask;
    pRetrievedBands->bands.tdsBands = pBuffer->tds_band_mask;
    pRetrievedBands->bands.lteBands = pBuffer->lte_band_mask;
  
    // update BST bands
    pRetrievedBands->excludeFromBst.gwBands  = pBuffer->gw_bst_mask;
    pRetrievedBands->excludeFromBst.tdsBands = pBuffer->tds_bst_mask;
    pRetrievedBands->excludeFromBst.lteBands = pBuffer->lte_bst_mask;
  }
}


/*-------- policyman_process_versioned_rats_bands_efs --------*/
mre_efs_status_t policyman_process_versioned_rats_bands_efs(
  rat_capability_efs_v1_t   *pBuffer,
  uint32                    *pRatMask,
  rf_efs_bands_t            *pRetrievedBands,
  mre_set_t                 *pRestrictionSet
)
{
  mre_efs_status_t efs_status = MRE_EFS_STATUS_ERROR;
  
  if ((pBuffer->version & POLICYMAN_RAT_CAPABILITY_VERSION_TAG) != POLICYMAN_RAT_CAPABILITY_VERSION_TAG)
  {
    // Version marker not present
    POLICYMAN_MSG_ERROR_1( "Invalid rat capability and rf bands from EFS; vers 0x%08x",
                           pBuffer->version );

    goto Done;
  }

  pBuffer->version &= POLICYMAN_RAT_CAPABILITY_VERSION_MASK; // Remove tag and leave version
  if (pBuffer->version != POLICYMAN_RAT_CAPABILITY_VERSION_NUM)
  {
    POLICYMAN_MSG_ERROR_1( "Invalid rat capability/rf bands version from EFS; vers %d",
                           pBuffer->version );

    goto Done;
  }

  if (pBuffer->num_policy_restrictions > POLICYMAN_RAT_CAPABILITY_MAX_RESTRICTIONS)
  {
    POLICYMAN_MSG_ERROR_1( "Invalid rat capability/ rf bands from EFS; number of stored rf restrictions %d exceeds max",
                           pBuffer->num_policy_restrictions );

    goto Done;
  }

  if (NON_NULL(pRatMask))
  {
    *pRatMask = pBuffer->rat_mask;
  }
  
  if (NON_NULL(pRetrievedBands))
  {
    pRetrievedBands->bands.gwBands  = pBuffer->gw_band_mask;
    pRetrievedBands->bands.tdsBands = pBuffer->tds_band_mask;
    pRetrievedBands->bands.lteBands = pBuffer->lte_band_mask;
  
    // update BST bands
    pRetrievedBands->excludeFromBst.gwBands  = pBuffer->gw_bst_mask;
    pRetrievedBands->excludeFromBst.tdsBands = pBuffer->tds_bst_mask;
    pRetrievedBands->excludeFromBst.lteBands = pBuffer->lte_bst_mask;
  }

  if (NON_NULL(pRestrictionSet))
  {
    size_t num_res;

    for (num_res = 0; num_res < pBuffer->num_policy_restrictions; num_res++)
    {
      rf_restrictions_t rf_restriction;

      rf_restriction = pBuffer->rfRestrictions[num_res];
      mre_set_add(pRestrictionSet, &rf_restriction);
    }
  }

  efs_status = MRE_EFS_STATUS_SUCCESS;

Done:
  return efs_status;
}


/*-------- policyman_retrieve_rats_bands --------*/
boolean policyman_retrieve_rats_bands(
  sys_modem_as_id_e_type     subsId,
  uint32                    *pRatMask,
  rf_efs_bands_t            *pRetrievedBands,
  mre_set_t                 *pRestrictionSet
)
{
  mre_efs_status_t          efsStatus      = MRE_EFS_STATUS_ERROR;
  rat_capability_efs_v1_t  *rat_cap_buffer = NULL;
  size_t                    rat_file_size  = 0;

  efsStatus = mre_efs_get_file( POLICYMAN_RAT_MASK_NV_PATH,
                                (void **)&rat_cap_buffer,
                                &rat_file_size,
                                subsId );

  if (MRE_EFS_STATUS_SUCCESS == efsStatus)
  {
    if (rat_file_size == sizeof(rat_capability_efs_t))
    {
      rat_capability_efs_t *old_rat_cap_efs = (rat_capability_efs_t *)rat_cap_buffer;
      policyman_process_original_rats_bands_efs(old_rat_cap_efs, pRatMask, pRetrievedBands);

      POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "---------- RATs and bands from EFS; RAT mask: 0x%04x",
                            subsId,
                            old_rat_cap_efs->rat_mask );

      policyman_rf_print_bands( subsId,
                                old_rat_cap_efs->gw_band_mask,
                                old_rat_cap_efs->lte_band_mask,
                                old_rat_cap_efs->tds_band_mask );
    }
    else
    {
      efsStatus = policyman_process_versioned_rats_bands_efs(rat_cap_buffer, pRatMask, pRetrievedBands, pRestrictionSet);

      POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "---------- RATs and bands from EFS; RAT mask: 0x%04x",
                            subsId,
                            rat_cap_buffer->rat_mask );

      policyman_rf_print_bands( subsId,
                                rat_cap_buffer->gw_band_mask,
                                rat_cap_buffer->lte_band_mask,
                                rat_cap_buffer->tds_band_mask );
    }
  }

  POLICYMAN_MEM_FREE_IF(rat_cap_buffer);

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "policyman_retrieve_rats_bands: status %d, filesize %d",
                        subsId,
                        efsStatus,
                        rat_file_size );

  return (efsStatus == MRE_EFS_STATUS_SUCCESS);
}


/*-------- policyman_rats_bands_persist --------*/
STATIC boolean policyman_rats_bands_persist(
  sys_modem_as_id_e_type     subsId,
  uint32                    *pRatMask,
  rf_efs_bands_t            *pWriteBands,
  mre_set_t                 *pRestrictionSet
)
{
  boolean                   write_status;
  rat_capability_efs_v1_t  *pRatCapBuffer;
  size_t                    idx;

  pRatCapBuffer = (rat_capability_efs_v1_t*)policyman_mem_alloc(sizeof(*pRatCapBuffer));

  pRatCapBuffer->version          = POLICYMAN_RAT_CAPABILITY_VERSION_TAG + POLICYMAN_RAT_CAPABILITY_VERSION_NUM;

  pRatCapBuffer->rat_mask      = *pRatMask;
  pRatCapBuffer->gw_band_mask  = pWriteBands->bands.gwBands;
  pRatCapBuffer->lte_band_mask = pWriteBands->bands.lteBands;
  pRatCapBuffer->tds_band_mask = pWriteBands->bands.tdsBands;

  // update BST bands
  pRatCapBuffer->gw_bst_mask  = pWriteBands->excludeFromBst.gwBands;
  pRatCapBuffer->lte_bst_mask = pWriteBands->excludeFromBst.lteBands;
  pRatCapBuffer->tds_bst_mask = pWriteBands->excludeFromBst.tdsBands;

  pRatCapBuffer->num_policy_restrictions = (mre_set_num_elems(pRestrictionSet) < POLICYMAN_RAT_CAPABILITY_MAX_RESTRICTIONS) ? 
                                           mre_set_num_elems(pRestrictionSet) : POLICYMAN_RAT_CAPABILITY_MAX_RESTRICTIONS;

  for (idx = 0; idx < pRatCapBuffer->num_policy_restrictions; idx++)
  {
    pRatCapBuffer->rfRestrictions[idx] = *(rf_restrictions_t *)mre_set_elem_ptr(pRestrictionSet, idx);
  }

  write_status = policyman_cfgitem_persist_item( POLICYMAN_RAT_MASK_NV_PATH,
                                                 (void *)pRatCapBuffer,
                                                 sizeof(*pRatCapBuffer),
                                                 subsId );

  POLICYMAN_MSG_HIGH_4( SUBS_PREFIX "Wrote RAT mask to EFS: base 0x%04x, status %d, filesize %d",
                        subsId,
                        *pRatMask,
                        write_status,
                        sizeof(*pRatCapBuffer) );

  policyman_mem_free(pRatCapBuffer);

  return write_status;
}


/*-------- policyman_retrieve_rats_bands_per_subs --------*/
policyman_status_t policyman_retrieve_rats_bands_per_subs(
  size_t                    subs,
  uint32                   *pRatMask,
  rf_efs_bands_t           *pRetrievedBands,
  mre_set_t               **ppRestrictionSet
)
{
  size_t               nItems;
  policyman_status_t   status   = POLICYMAN_STATUS_ERR;
  policyman_item_t    *pRatcfg  = NULL;
  policyman_item_id_t  ratId    = POLICYMAN_ITEM_RAT_CAPABILITY;
  policyman_item_t    *pBandcfg = NULL;
  policyman_item_id_t  rfId     = POLICYMAN_ITEM_RF_BANDS;

  nItems = policyman_cfgitem_get_items_per_subs( &ratId,
                                                 1,
                                                 (policyman_item_t const **)&pRatcfg,
                                                 subs );
  if (nItems != 1)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "policyman_get_items failed for RAT capability", subs);
    goto Done;
  }

  *pRatMask = policyman_rat_capability_get_base_rats((policyman_item_t const *)pRatcfg);

  nItems = policyman_cfgitem_get_items_per_subs( &rfId,
                                                 1,
                                                 (policyman_item_t const **)&pBandcfg,
                                                 subs );
  if (nItems != 1)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "policyman_get_items failed for Band capability", subs);
    goto Done;
  }
  status = policyman_get_rf_bands( (policyman_item_t const *)pBandcfg,
                                   &pRetrievedBands->bands.gwBands,
                                   &pRetrievedBands->bands.lteBands,
                                   &pRetrievedBands->bands.tdsBands);

  (void)policyman_rf_bands_get_restriction_set(pBandcfg, ppRestrictionSet);

Done:

  MRE_RELEASE_IF(pRatcfg);
  MRE_RELEASE_IF(pBandcfg);
  return status;
}


/*-------- policyman_update_rats_bands_to_efs --------*/
void policyman_update_rats_bands_to_efs(
  uint8 subsMask
)
{
  policyman_status_t        status;
  boolean                   write_status;
  uint32                    ratMask;
  rf_efs_bands_t            bandsToWrite;
  mre_set_t                *pRestrictionSet = NULL;

  size_t                    subs;
  size_t                    nSim       = 0;

  (void)policyman_get_current_num_sim(&nSim);

  for (subs = 0; subs < nSim; subs++)
  {
    status = policyman_retrieve_rats_bands_per_subs(subs, &ratMask, &bandsToWrite, &pRestrictionSet);
    if (!POLICYMAN_SUCCEEDED(status))
    {
      POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "Failed to retrieve rat/band information, skipping EFS write", subs);
      continue;
    }

    /* Write data to EFS file
    */
    write_status = policyman_rats_bands_persist( (sys_modem_as_id_e_type)subs,
                                                 &ratMask,
                                                 &bandsToWrite,
                                                 pRestrictionSet );

    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "update_rats_bands_to_efs: write_status %d",
                          subs,
                          write_status );

    MRE_RELEASE_IF(pRestrictionSet);
  }
}


/*-------- policyman_update_rats_bands_to_efs_per_tech --------*/
void policyman_update_rats_bands_to_efs_per_tech(
  uint32  rat_include
)
{
  policyman_status_t        status;
  boolean                   write_status;
  uint32                    ratMask;
  rf_efs_bands_t            bandsToWrite;
  mre_set_t                *pRestrictionSet = NULL;
  sys_band_mask_type        hwGwBand;
  sys_band_mask_type        hwTdsBand;
  sys_modem_as_id_e_type    subs;

  subs = SYS_MODEM_AS_ID_1;

  policyman_get_hardware_bands_all_techs( &hwGwBand, NULL, &hwTdsBand );

  status = policyman_retrieve_rats_bands_per_subs( subs, &ratMask, &bandsToWrite, &pRestrictionSet);
  if (!POLICYMAN_SUCCEEDED(status))
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "Failed to retrieve rat/band information, skipping EFS write", subs);
    return;
  }

  if ( rat_include & SYS_SYS_MODE_MASK_WCDMA )
  {
    ratMask |= SYS_SYS_MODE_MASK_WCDMA;
    bandsToWrite.bands.gwBands  |= hwGwBand & ALL_WCDMA_BANDS;
  }
  else
  {
    ratMask = ratMask & ~SYS_SYS_MODE_MASK_WCDMA;
    bandsToWrite.bands.gwBands  = bandsToWrite.bands.gwBands & ~ALL_WCDMA_BANDS;
  }

  if ( rat_include & SYS_SYS_MODE_MASK_TDS )
  {
    ratMask |= SYS_SYS_MODE_MASK_TDS;
    bandsToWrite.bands.tdsBands = hwTdsBand;
  }
  else
  {
    ratMask = ratMask & ~SYS_SYS_MODE_MASK_TDS;
    bandsToWrite.bands.tdsBands = 0;
  }

  /* Write data to EFS file
   */
  write_status = policyman_rats_bands_persist(subs, &ratMask, &bandsToWrite, pRestrictionSet);

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "update_rats_bands_to_efs_per_tech: write_status %d",
                        subs,
                        write_status );

  MRE_RELEASE_IF(pRestrictionSet);
}


/*-------- policyman_rat_capability_fill_subs_diag_log --------*/
void policyman_rat_capability_fill_subs_diag_log(
  policyman_item_t const  *pItem,
  LOG_PM_SUBS_INFO_type   *log_ptr
)
{
  rat_capability_t  *pRatCap = (rat_capability_t *)pItem;

  log_ptr->rat_mask = pRatCap->ratMask;
  log_ptr->subphone_cap = pRatCap->subphoneCap;
  log_ptr->exclude_mask = pRatCap->ratExcludeMask;
  log_ptr->base_mask    = policyman_rat_capability_get_effective_rats((policyman_item_t *)pItem);
}


/*-------- policyman_rat_capability_get_limited_item --------*/
void policyman_rat_capability_get_limited_item(
  mre_set_t             *pItemSet,
  sys_modem_as_id_e_type subsId
)
{
  rat_capability_t *pRatItem;

  pRatItem =  POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_RAT_CAPABILITY,
                                     rat_capability_t,
                                     policyman_simple_dtor,
                                     subsId );
  
  // A rat Mask of None ensures no acqusition attempts in lower layers
  pRatItem->ratMask     = 0;
  pRatItem->subphoneCap = policyman_rat_capability_get_max_config(pItemSet, subsId);

  mre_set_add(pItemSet, &pRatItem);
  ref_cnt_obj_release(pRatItem);
}

/*-------- policyman_rat_capability_mdump --------*/
QSH_MDUMP_FN_ATTR_PM void policyman_rat_capability_mdump(
  policyman_item_t const *pItem
)
{
  policyman_qsh_dump_collect((void *)pItem, sizeof(rat_capability_t));
}
