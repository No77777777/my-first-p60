/**
  @file policyman_rf.c

  @brief  RAT and RF band information
*/

/*
    Copyright (c) 2013-2019,2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_rf.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "ref_cnt_obj.h"
#include "mre_engine.h"
#include "mre_namedobj.h"
#include "policyman_cfgitem.h"
#include "policyman_device_config.h"
#include "policyman_diag.h"
#include "policyman_plmn.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_rat_capability.h"
#include "policyman_rf.h"
#include "policyman_state.h"
#include "policyman_subs.h"
#include "policyman_tech.h"
#include "policyman_util.h"
#include "policyman_xml.h"

#include "event.h"
#include "trm.h"
#include "rfm.h"
#include "rcevt.h"
#include "mcs_wrappers.h"  // MCS wrappers for 'critical' rcevt_* functions


#define HAS_CAPABILITY(mode)  \
  (mcs_modem_has_capability(MCS_MODEM_CAPABILITY_FEATURE_##mode) != MCS_MODEM_CAP_UNAVAILABLE)


struct policyman_rf_info_t
{
  policyman_rf_device_info_t  overall;    /* this is the composite of all devices */
};

/*-------- rf_band_item_t --------*/
struct rf_bands_item_t
{
  POLICYMAN_ITEM;

  rf_efs_bands_t    rfBands;
  mre_set_t        *pRestrictions;
};

typedef enum
{
  GW_BANDS,
  LTE_BANDS,
  TDS_BANDS
} band_selector_t;

typedef struct
{
  POLICYMAN_ACTION_BASE;

  char const                *pListName;   /* Used to indicate which named band list to use */
  rf_bands_t                *pBands;
  mre_named_obj_namespace_t  ns;
  lookup_method_t            lookup_method;
} rf_bands_action_t;


static policyman_rf_info_t device_rf_info;

/*-------- policyman_rf_hw_capability_supports_cdma --------*/
boolean policyman_rf_hw_capability_supports_cdma(
  void
)
{
  uint32  hwMask;
  boolean hasCdma = FALSE;

#ifdef FEATURE_CDMA
  hwMask  = device_rf_info.overall.ratMask;
  hasCdma = ((hwMask & SYS_SYS_MODE_MASK_CDMA) != 0);
#endif

  return hasCdma;
}

/*-------- policyman_rf_bands_item_dtor --------*/
STATIC void policyman_rf_bands_item_dtor(
  void  *pObj
)
{
  rf_bands_item_t *pMe = (rf_bands_item_t *)pObj;

  MRE_RELEASE_IF(pMe->pRestrictions);
  mre_simple_dtor(pObj);
}

/*-------- policyman_rf_bands_item_new --------*/
policyman_item_t * policyman_rf_bands_item_new(
  rf_efs_bands_t  *pBands,
  mre_set_t              *pRestrictionSet,
  sys_modem_as_id_e_type  subsId
)
{
  rf_bands_item_t            *prfb;
  policyman_rf_device_info_t *pOverall;

  prfb = POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_RF_BANDS,
                                rf_bands_item_t,
                                policyman_rf_bands_item_dtor,
                                subsId );

  // mask with HW RF bands
  pOverall = policyman_rf_get_overall_info();

  prfb->rfBands.bands.gwBands  = pBands->bands.gwBands & pOverall->gwBands;
  prfb->rfBands.bands.lteBands = SYS_LTE_BAND_MASK_COMMON(&pBands->bands.lteBands, &pOverall->lteBands);
  prfb->rfBands.bands.tdsBands = pBands->bands.tdsBands & pOverall->tdsBands;

  // update BST bands
  prfb->rfBands.excludeFromBst.gwBands  = pBands->excludeFromBst.gwBands & pOverall->gwBands;
  prfb->rfBands.excludeFromBst.lteBands = SYS_LTE_BAND_MASK_COMMON(&pBands->excludeFromBst.lteBands, &pOverall->lteBands);
  prfb->rfBands.excludeFromBst.tdsBands = pBands->excludeFromBst.tdsBands & pOverall->tdsBands;

  // update VOLTE bands
  prfb->rfBands.excludeForVolte.gwBands  = pBands->excludeForVolte.gwBands & pOverall->gwBands;
  prfb->rfBands.excludeForVolte.lteBands = SYS_LTE_BAND_MASK_COMMON(&pBands->excludeForVolte.lteBands, &pOverall->lteBands);
  prfb->rfBands.excludeForVolte.tdsBands = pBands->excludeForVolte.tdsBands & pOverall->tdsBands;

  prfb->pRestrictions = pRestrictionSet;
  ref_cnt_obj_add_ref(pRestrictionSet);
  
  return (policyman_item_t *) prfb;
}

/*=============================================================================
  Actions related to RF bands
=============================================================================*/

/*-----------------------------------------------------------------------------
  Helper functions for RF bands.
-----------------------------------------------------------------------------*/


/*-------- policyman_rf_bands_dtor --------*/
STATIC void policyman_rf_bands_dtor(
  void  *pObj
)
{
  rf_bands_t  *pBands = (rf_bands_t *) pObj;

  POLICYMAN_MEM_FREE_IF(pBands->pName);
  policyman_mem_free(pBands);
}


/*-------- policyman_rf_band_action_dtor --------*/
STATIC void policyman_rf_band_action_dtor(
  void  *pObj
)
{
  rf_bands_action_t  *pAction = (rf_bands_action_t *) pObj;

  POLICYMAN_MEM_FREE_IF(pAction->pListName);
  MRE_RELEASE_IF(pAction->pBands);

  policyman_action_dtor(pAction);
}


/*-------- policyman_rf_bands_parse --------*/
STATIC boolean policyman_rf_bands_parse(
  char const          *pStr,
  sys_band_mask_type  *pMask
)
{
  char                token[32];
  int                 bandNum;
  sys_band_mask_type  mask     = 0;

  while (policyman_get_token(&pStr, token, sizeof(token)))
  {
    bandNum = atoi(token);

    if (bandNum < 0 || bandNum > 63)
    {
      POLICYMAN_UT_MSG_1("Invalid band in RF bands specification: %s", token);
      return FALSE;
    }

    mask |= (1LL << bandNum);
  }

  *pMask = mask;

  return TRUE;
}

/*-------- policyman_rf_lte_bands_parse --------*/
STATIC boolean policyman_rf_lte_bands_parse(
  char const               *pStr,
  sys_lte_band_mask_e_type *pLteBandMask
)
{
  char                      token[32];
  int                       bandNum;
  sys_lte_band_mask_e_type  tempLteBandMask;

  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&tempLteBandMask);

  while (policyman_get_token(&pStr, token, sizeof(token)))
  {
    bandNum = atoi(token);

    if ( (bandNum < 0) || (bandNum > 255) )
    {
      POLICYMAN_MSG_HIGH_1("Invalid band in LTE RF bands specification: %d", bandNum);
      POLICYMAN_UT_MSG_1("Invalid band in LTE RF bands specification: %s", token);
      return FALSE;
    }

    SYS_LTE_BAND_MASK_ADD_BAND(&tempLteBandMask, bandNum);
  }

  *pLteBandMask = tempLteBandMask;

  return TRUE;
}

/*-------- rf_band_mask_read --------*/
STATIC boolean rf_band_mask_read(
  char const      *pStr,
  rf_band_set_t   *pBandSet,
  band_selector_t  whichBands
)
{
  boolean succeeded;

  switch (whichBands)
  {
    case LTE_BANDS:
      succeeded = policyman_rf_lte_bands_parse(pStr, &pBandSet->lteBands);
      break;

    case GW_BANDS:
      succeeded = policyman_rf_bands_parse(pStr, &pBandSet->gwBands);
      break;

    case TDS_BANDS:
      succeeded = policyman_rf_bands_parse(pStr, &pBandSet->tdsBands);
      break;

    default:
      succeeded = FALSE;
  }

  return succeeded;
}

/*-------- policyman_rf_band_set_base --------*/
STATIC void policyman_rf_band_set_base(
  rf_bands_t       *pBands,
  band_selector_t   whichBands,
  policyman_base_t  base
)
{
  switch (whichBands)
  {
    case GW_BANDS:
      pBands->bandsBase.gwBandsBase = base;
      break;
  
    case LTE_BANDS:
      pBands->bandsBase.lteBandsBase = base;
      break;
  
    default:
      pBands->bandsBase.tdsBandsBase = base;
  }
}

/*-------- policyman_rf_band_mask_read --------*/
STATIC boolean policyman_rf_band_mask_read(
  mre_xml_element_t const       *pElem,
  rf_bands_t                    *pBands,
  band_selector_t                whichBands
)
{
  boolean           succeeded;
  size_t            nChildren;
  size_t            numChild;
  policyman_base_t  base;

  succeeded = policyman_util_get_base(pElem, &base);

  if (succeeded)
  {
    policyman_rf_band_set_base(pBands, whichBands, base);

    // if base none or current then clear the bands
    if (   base == POLICYMAN_BASE_NONE
        || base == POLICYMAN_BASE_CURRENT)
    {
      switch (whichBands)
      {
        case GW_BANDS:
          pBands->bands.gwBands  = 0;
          break;

        case LTE_BANDS:
          SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&pBands->bands.lteBands);
          break;

        default:
          pBands->bands.tdsBands = 0;
      }
    }
  }

  /*  Get the number of child elements.
   */
  nChildren = policyman_xml_num_children(pElem);
  if (nChildren == 0)
  {
    goto Done;
  }

  for (numChild = 0 ; succeeded && numChild < nChildren ; ++numChild)
  {
    char const                    *pStr;
    mre_xml_element_t const       *pChild;

    pChild = policyman_xml_get_child(pElem, numChild);

    pStr = policyman_xml_get_text(pChild);

    if (pStr == NULL)
    {
      POLICYMAN_UT_MSG_0("No Content for rf band");
      continue;
    }

    if (policyman_xml_tag_is(pChild, "include"))
    {
      succeeded = rf_band_mask_read(pStr, &pBands->bands, whichBands);
    }
    else if (policyman_xml_tag_is(pChild, "exclude"))
    {
      succeeded = rf_band_mask_read(pStr, &pBands->exclude, whichBands);
    }
    else if (policyman_xml_tag_is(pChild, "exclude_for_bst"))
    {
      succeeded = rf_band_mask_read(pStr, &pBands->excludeFromBst, whichBands);
    }
    else if (policyman_xml_tag_is(pChild, "exclude_for_volte"))
    {
      succeeded = rf_band_mask_read(pStr, &pBands->excludeForVolte, whichBands);
    }
    else
    {
      POLICYMAN_UT_MSG_1("Invalid tag in RF bands specification: %s", pStr);
      succeeded = FALSE;
    }
  }

Done:
  return succeeded;
}

/*-------- policyman_rf_band_list_read --------*/
boolean policyman_rf_band_list_read(
  mre_xml_element_t const       *pElem,
  rf_bands_t                    *pBands
)
{
  policyman_status_t          succeeded = TRUE;
  size_t                      nChildren;
  size_t                      i;
  policyman_rf_device_info_t *pRfInfo;
  band_selector_t             whichBands;

  /*  Initialize band information to hardware settings, so that the XML
   *  only needs to specify band masks that differ from the hardware.
   */
  pRfInfo                         = policyman_rf_get_overall_info();
  pBands->bands.gwBands           = pRfInfo->gwBands;
  pBands->bands.lteBands          = pRfInfo->lteBands;
  pBands->bands.tdsBands          = pRfInfo->tdsBands;

  pBands->exclude.gwBands         = 0;
  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&pBands->exclude.lteBands);
  pBands->exclude.tdsBands        = 0;

  pBands->excludeFromBst.gwBands  = 0;
  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&pBands->excludeFromBst.lteBands);
  pBands->excludeFromBst.tdsBands = 0;

  pBands->excludeForVolte.gwBands  = 0;
  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&pBands->excludeForVolte.lteBands);
  pBands->excludeForVolte.tdsBands = 0;

  pBands->bandsBase.gwBandsBase    = POLICYMAN_BASE_HARDWARE;
  pBands->bandsBase.lteBandsBase   = POLICYMAN_BASE_HARDWARE;
  pBands->bandsBase.tdsBandsBase   = POLICYMAN_BASE_HARDWARE;

  nChildren = policyman_xml_num_children(pElem);
  if (nChildren == 0)
  {
    goto Done;
  }

  for (i = 0 ; succeeded && i < nChildren ; ++i)
  {
    mre_xml_element_t const *pChild;

    pChild = policyman_xml_get_child(pElem, i);

    // read the inline tags
    if (policyman_xml_tag_is(pChild, "gw_bands"))
    {
      whichBands = GW_BANDS;
    }
    else if (policyman_xml_tag_is(pChild, "lte_bands"))
    {
      whichBands = LTE_BANDS;
    }
    else if (policyman_xml_tag_is(pChild, "tds_bands"))
    {
      whichBands = TDS_BANDS;
    }
    else
    {
      succeeded = FALSE;
      break;
    }

    succeeded = policyman_rf_band_mask_read(pChild, pBands, whichBands);
  }

  if (succeeded)
  {
    /*  Remove any bands to be excluded from the base bands.
     */
    pBands->bands.gwBands  &= ~pBands->exclude.gwBands;
    pBands->bands.tdsBands &= ~pBands->exclude.tdsBands;
    SYS_LTE_BAND_MASK_REMOVE_MASK(&pBands->bands.lteBands, &pBands->exclude.lteBands);
  }

Done:
  return succeeded;
}


/*-------- policyman_rf_restriction_compare --------*/
STATIC boolean policyman_rf_restriction_compare(
  void const *pElem1,
  void const *pElem2
)
{
  rf_restrictions_t  restriction1 = *((rf_restrictions_t *)pElem1);
  rf_restrictions_t  restriction2 = *((rf_restrictions_t *)pElem2);

  return (   restriction1.policy_num  == restriction2.policy_num
          && restriction1.policy_type == restriction2.policy_type
          && restriction1.policy_ver  == restriction2.policy_ver);
}


/*-------- policyman_rf_bands_clone --------*/
policyman_item_t * policyman_rf_bands_clone(
  policyman_item_t const *pItem
)
{
  rf_bands_item_t const *pRfbItem       = (rf_bands_item_t const *)pItem;
  rf_bands_item_t       *pRfbItemClone;

  pRfbItemClone = POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_RF_BANDS,
                                         rf_bands_item_t,
                                         policyman_rf_bands_item_dtor,
                                         pRfbItem->asubs_id );

  pRfbItemClone->rfBands.bands           = pRfbItem->rfBands.bands;
  pRfbItemClone->rfBands.excludeFromBst  = pRfbItem->rfBands.excludeFromBst;
  pRfbItemClone->rfBands.excludeForVolte = pRfbItem->rfBands.excludeForVolte;
  pRfbItemClone->pRestrictions           = mre_set_clone(pRfbItem->pRestrictions);

  return (policyman_item_t *)pRfbItemClone;
}


/*-----------------------------------------------------------------------------
  Named RF band list actions
-----------------------------------------------------------------------------*/

/*-------- execute_rf_band_list --------*/
STATIC boolean execute_rf_band_list(
  policyman_action_t const      *pAction,
  void                          *pCtx,
  sys_modem_as_id_e_type         subsId
)
{
  rf_bands_action_t  *pBandAction = (rf_bands_action_t *) pAction;
  mre_policy_t       *pPolicy     = EXECUTE_CTX_POLICY(pCtx);

  if (pPolicy == NULL)
  {
    POLICYMAN_MSG_ERROR_0("Internal error - executing RF band list action with no policy");
    return FALSE;
  }

  POLICYMAN_MSG_HIGH_1(SRC_LINE "executing <rf_band_list>", LINE_NUM(pBandAction));

  return mre_named_object_add( (mre_named_object_t *) pBandAction->pBands,
                               pBandAction->ns,
                               pPolicy );
}

/*-------- policyman_rf_band_list_action_execute --------*/
STATIC boolean policyman_rf_band_list_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const  *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_rf_band_list);
}

/*-------- policyman_rf_band_list_action_new --------*/
mre_status_t policyman_rf_band_list_action_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_action_t             **ppAction
)
{
  mre_status_t       status  = MRE_STATUS_ERR_MALFORMED_XML;
  rf_bands_action_t *pAction = NULL;
  char const        *pName;

  /*  Get the name to be associated with this list of bands.
   */
  pName = policyman_xml_get_attribute(pElem, "name");
  if (pName == NULL)
  {
    POLICYMAN_UT_MSG_0("<rf_band_list>: must have a 'name' attribute");
    goto Done;
  }

  /*  Allocate and initialize the action itself.
   */
  pAction = POLICYMAN_ACTION_NEW( rf_bands_action_t,
                                  pPolicy,
                                  policyman_rf_band_list_action_execute,
                                  policyman_rf_band_action_dtor,
                                  SUBS_THIS,
                                  pElem );

  /*  Allocate and initialize the bands object, giving it the name.
   */
  pAction->pBands = MRE_NAMED_OBJECT_NEW( rf_bands_t,
                                          policyman_rf_bands_dtor,
                                          MRE_NAMED_RF_BANDS,
                                          pName );
  /*  Read the bands
   */
  if (!policyman_rf_band_list_read(pElem, pAction->pBands))
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<rf_band_list>: invalid band specification", POLICY_SUBS(pPolicy));
    MRE_RELEASE_IF(pAction);
    goto Done;
  }

  pAction->ns = mre_named_object_get_namespace(pElem);

  *ppAction = (mre_action_t *) pAction;
  status = MRE_STATUS_SUCCESS;

Done:
  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "<rf_band_list> returned status %d",
                        POLICY_SUBS(pPolicy),
                        status );
  return status;
}


/*-----------------------------------------------------------------------------
  RF bands action
-----------------------------------------------------------------------------*/
/*-------- policyman_get_rf_bands_item --------*/
STATIC rf_bands_item_t * policyman_get_rf_bands_item(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  rf_bands_item_t **ppRfItem;
  rf_bands_item_t  *pRfItem;

  ppRfItem = (rf_bands_item_t **)policyman_cfgitem_find_item_in_itemset( pItemSet,
                                                                         POLICYMAN_ITEM_RF_BANDS,
                                                                         subsId );
  if (NON_NULL(ppRfItem) && NON_NULL(*ppRfItem))
  {
    pRfItem = *ppRfItem;
  }
  else
  {
    pRfItem = (rf_bands_item_t *) policyman_cfgitem_get_item( POLICYMAN_ITEM_RF_BANDS,
                                                              subsId );
  }

  return pRfItem;
}


/*-------- policyman_rf_bands_combine --------*/
STATIC rf_efs_bands_t policyman_rf_bands_combine(
  rf_efs_bands_t    *pCurrent,
  rf_restrictions_t *pNew
)
{
  rf_efs_bands_t rfBands;

  memset(&rfBands, 0x00, sizeof(rfBands));

  // create band object to use
  if (pNew->bandsBase.gwBandsBase == POLICYMAN_BASE_CURRENT)
  {
    rfBands.bands.gwBands  = pCurrent->bands.gwBands;
    rfBands.bands.gwBands |= pNew->bands.gwBands;
    rfBands.bands.gwBands &= ~pNew->exclude.gwBands;

    rfBands.excludeForVolte.gwBands  = pCurrent->excludeForVolte.gwBands;
    rfBands.excludeForVolte.gwBands |= pNew->excludeForVolte.gwBands;

    rfBands.excludeFromBst.gwBands  = pCurrent->excludeFromBst.gwBands;
    rfBands.excludeFromBst.gwBands |= pNew->excludeFromBst.gwBands;
  }
  else
  {
    rfBands.bands.gwBands = pNew->bands.gwBands;
    rfBands.excludeForVolte.gwBands = pNew->excludeForVolte.gwBands;
    rfBands.excludeFromBst.gwBands = pNew->excludeFromBst.gwBands;
  }

  if (pNew->bandsBase.lteBandsBase == POLICYMAN_BASE_CURRENT)
  {
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.bands.lteBands, &pCurrent->bands.lteBands);
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.bands.lteBands, &pNew->bands.lteBands);
    SYS_LTE_BAND_MASK_REMOVE_MASK(&rfBands.bands.lteBands, &pNew->exclude.lteBands);

    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.excludeForVolte.lteBands, &pCurrent->excludeForVolte.lteBands);
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.excludeForVolte.lteBands, &pNew->excludeForVolte.lteBands);

    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.excludeFromBst.lteBands, &pCurrent->excludeFromBst.lteBands);
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.excludeFromBst.lteBands, &pNew->excludeFromBst.lteBands);
  }
  else
  {
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.bands.lteBands, &pNew->bands.lteBands);
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.excludeForVolte.lteBands, &pNew->excludeForVolte.lteBands);
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.excludeFromBst.lteBands, &pNew->excludeFromBst.lteBands);
  }

  if (pNew->bandsBase.tdsBandsBase == POLICYMAN_BASE_CURRENT)
  {
    rfBands.bands.tdsBands  = pCurrent->bands.tdsBands;
    rfBands.bands.tdsBands |= pNew->bands.tdsBands;
    rfBands.bands.tdsBands &= ~pNew->exclude.tdsBands;

    rfBands.excludeForVolte.tdsBands  = pCurrent->excludeForVolte.tdsBands;
    rfBands.excludeForVolte.tdsBands |= pNew->excludeForVolte.tdsBands;

    rfBands.excludeFromBst.tdsBands  = pCurrent->excludeFromBst.tdsBands;
    rfBands.excludeFromBst.tdsBands |= pNew->excludeFromBst.tdsBands;
  }
  else
  {
    rfBands.bands.tdsBands = pNew->bands.tdsBands;
    rfBands.excludeForVolte.tdsBands = pNew->excludeForVolte.tdsBands;
    rfBands.excludeFromBst.tdsBands = pNew->excludeFromBst.tdsBands;
  }

  return rfBands;
}


/*-------- policyman_rf_hardware_band_item_new --------*/
rf_bands_item_t *policyman_rf_hardware_band_item_new(
  sys_modem_as_id_e_type  subsId
)
{
  policyman_rf_device_info_t  *pOverall = policyman_rf_get_overall_info();
  rf_bands_item_t             *prfb;

  // Get RF band Item with all bands
  {
    rf_efs_bands_t  rfBands;
    mre_set_t      *pRfRestrictions;

    rfBands.bands.gwBands            = pOverall->gwBands;
    rfBands.bands.tdsBands           = pOverall->tdsBands;
    rfBands.excludeFromBst.gwBands   = 0;
    rfBands.excludeFromBst.tdsBands  = 0;
    rfBands.excludeForVolte.gwBands  = 0;
    rfBands.excludeForVolte.tdsBands = 0;

    SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&rfBands.bands.lteBands);
    SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&rfBands.excludeFromBst.lteBands);
    SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&rfBands.excludeForVolte.lteBands);
    SYS_LTE_BAND_MASK_ADD_MASK(&rfBands.bands.lteBands, &pOverall->lteBands);

    pRfRestrictions = mre_set_new( sizeof(rf_restrictions_t),
                                   3,
                                   MRE_SET_NO_MAX_SIZE,
                                   policyman_rf_restriction_compare,
                                   NULL );

    prfb = (rf_bands_item_t *) policyman_rf_bands_item_new(&rfBands, pRfRestrictions, subsId);

    MRE_RELEASE_IF(pRfRestrictions);
  }

  return prfb;
}


/*-------- policyman_rf_set_full_bands --------*/
void policyman_rf_set_full_bands(
  policyman_state_t  *pState,
  mre_set_t          *pItemSet
)
{
  size_t                     numSim;
  rf_bands_item_t           *pRf;
  sys_modem_as_id_e_type     subsId;

  (void)policyman_get_current_num_sim(&numSim);

  for (subsId = SYS_MODEM_AS_ID_1; subsId < numSim; subsId++)
  {
    if ( policyman_subs_has_location(pState, subsId) )
    {
      pRf           = policyman_rf_hardware_band_item_new(subsId);

      mre_set_replace(pItemSet, &pRf);
      ref_cnt_obj_release(pRf);
    }
  }
}


/*-------- policyman_ensure_and_get_rf_bands_item --------*/
STATIC rf_bands_item_t * policyman_ensure_and_get_rf_bands_item(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  rf_bands_item_t **ppRfItem;
  rf_bands_item_t  *pRfItem;

  ppRfItem = (rf_bands_item_t **)policyman_cfgitem_find_item_in_itemset( pItemSet,
                                                                         POLICYMAN_ITEM_RF_BANDS,
                                                                         subsId );
  if (NON_NULL(ppRfItem) && NON_NULL(*ppRfItem))
  {
    pRfItem = *ppRfItem;
  }
  else
  {
    pRfItem = policyman_rf_hardware_band_item_new(subsId);
  }

  return pRfItem;
}


/*-------- policyman_rf_bands_merge_with_current_config --------*/
STATIC rf_efs_bands_t policyman_rf_bands_merge_with_current_config(
  mre_set_t              *pItemSet,
  rf_restrictions_t      *pRfRestriction,
  sys_modem_as_id_e_type  subsId
)
{
  uint32            config_mask = 0;
  rf_bands_item_t  *pRfItem;
  rf_efs_bands_t    rfBands;
  rf_efs_bands_t    rfBandsTemp;

  // retrieve current
  pRfItem = policyman_ensure_and_get_rf_bands_item(pItemSet, subsId);

  rfBandsTemp = pRfItem->rfBands;
  ref_cnt_obj_release(pRfItem);

  rfBands = policyman_rf_bands_combine(&rfBandsTemp, pRfRestriction);

  return rfBands;
}


/*-------- policyman_rf_bands_get_current_restriction_set --------*/
STATIC mre_set_t * policyman_rf_bands_get_current_restriction_set(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  uint32             config_mask = 0;
  rf_bands_item_t   *pRfItem;
  mre_set_t         *pRestrictionSet;

  // retrieve item
  pRfItem = policyman_get_rf_bands_item(pItemSet, subsId);

  if (NON_NULL(pRfItem))
  {
    pRestrictionSet = pRfItem->pRestrictions;
    ref_cnt_obj_add_ref(pRestrictionSet);
    ref_cnt_obj_release(pRfItem);
  }
  else
  {
    pRestrictionSet = mre_set_new(sizeof(rf_restrictions_t),
                                  3,
                                  MRE_SET_NO_MAX_SIZE,
                                  policyman_rf_restriction_compare,
                                  NULL);
  }

  return pRestrictionSet;
}


/*-------- policyman_rf_bands_populate_restriction --------*/
STATIC void policyman_rf_bands_populate_restriction(
  mre_policy_t           *pPolicy,
  rf_bands_t             *pBands,
  rf_restrictions_t      *pRfRestriction
)
{
  pRfRestriction->policy_num  = MRE_POLICY_NUMBER(pPolicy);
  pRfRestriction->policy_type = MRE_POLICY_TYPE(pPolicy);
  pRfRestriction->policy_ver  = MRE_POLICY_VERSION(pPolicy);

  pRfRestriction->bands           = pBands->bands;
  pRfRestriction->exclude         = pBands->exclude;
  pRfRestriction->excludeForVolte = pBands->excludeForVolte;
  pRfRestriction->excludeFromBst  = pBands->excludeFromBst;
  pRfRestriction->bandsBase       = pBands->bandsBase;
}


/*-------- policyman_rf_util_get_rf_bands_from_action --------*/
STATIC rf_bands_t * policyman_rf_util_get_rf_bands_from_action(
  rf_bands_action_t *pRfAction,
  mre_policy_t      *pPolicy
)
{
  rf_bands_t *pRfBands;

  pRfBands = pRfAction->pBands;

  if (IS_NULL(pRfBands) && NON_NULL(pRfAction->pListName))
  {
    pRfBands = (rf_bands_t *) mre_named_object_find( pRfAction->pListName,
                                                     POLICYMAN_NAMED_RF_BANDS,
                                                     pPolicy );
  }

  return pRfBands;
}

/*-------- policyman_rf_bands_filter_bandset_with_hardware --------*/
STATIC void policyman_rf_bands_filter_bandset_with_hardware(
  rf_band_set_t *pBandSet
)
{
  rf_band_set_t               bandSet  = *pBandSet;
  policyman_rf_device_info_t *pOverall;

  pOverall = policyman_rf_get_overall_info();

  bandSet.gwBands      &= pOverall->gwBands;
  bandSet.tdsBands     &= pOverall->tdsBands;
  bandSet.lteBands      = SYS_LTE_BAND_MASK_COMMON(&bandSet.lteBands, &pOverall->lteBands);
  *pBandSet = bandSet;
}


/*-------- policyman_rf_util_filter_rf_efs_bands_with_hardware --------*/
STATIC void policyman_rf_util_filter_rf_efs_bands_with_hardware(
  rf_efs_bands_t  *pBands
)
{
  policyman_rf_bands_filter_bandset_with_hardware(&pBands->bands);
  policyman_rf_bands_filter_bandset_with_hardware(&pBands->excludeFromBst);
  policyman_rf_bands_filter_bandset_with_hardware(&pBands->excludeForVolte);
}


/*-------- policyman_rf_band_clone_from_itemset --------*/
STATIC rf_bands_item_t   * policyman_rf_band_clone_from_itemset(
  mre_set_t                *pItemSet,
  sys_modem_as_id_e_type    subsId)
{

  policyman_item_t  *pItem;
  rf_bands_item_t   *pRfBands;

  pItem = (policyman_item_t *)policyman_get_rf_bands_item(pItemSet, subsId);

  if(NON_NULL(pItem))
  {
    pRfBands = (rf_bands_item_t *) policyman_rf_bands_clone(pItem);
  }
  else
  {
    pRfBands = policyman_rf_hardware_band_item_new(subsId);
  }
  MRE_RELEASE_IF(pItem);
  return pRfBands;
}


/*-------- execute_rf_bands --------*/
STATIC boolean execute_rf_bands(
  policyman_action_t const *pAction,
  void                     *pCtx,
  sys_modem_as_id_e_type    subsId
)
{
  rf_bands_action_t *prfbAction = (rf_bands_action_t *) pAction;
  mre_policy_t      *pPolicy    = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t         *pItemSet   = POLICY_EXECUTE_ITEMSET(pCtx);
  rf_bands_item_t   *pRfBands = NULL;
  boolean            succeeded  = FALSE;
  rf_bands_t        *pBands;

  pBands = policyman_rf_util_get_rf_bands_from_action(prfbAction, pPolicy);

  if (pBands != NULL)
  {
    rf_restrictions_t  rfRestriction;

    succeeded = TRUE;
    memset(&rfRestriction, 0x00, sizeof(rfRestriction));
    
    policyman_rf_bands_populate_restriction(pPolicy, pBands, &rfRestriction);

    pRfBands = policyman_rf_band_clone_from_itemset(pItemSet, subsId);
    mre_set_replace(pRfBands->pRestrictions, &rfRestriction);

    pRfBands->rfBands = policyman_rf_bands_merge_with_current_config(pItemSet, &rfRestriction, subsId);

    policyman_rf_util_filter_rf_efs_bands_with_hardware(&pRfBands->rfBands);

    if ( ((pRfBands->rfBands.bands.gwBands | pRfBands->rfBands.bands.tdsBands) == 0)
         && SYS_LTE_BAND_MASK_CHK_IF_EMPTY(&(pRfBands->rfBands.bands.lteBands)) )
    {
      POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "<rf_bands> none attempted, action not taken", subsId);
    }
    else
    {

      mre_set_replace(pItemSet, &pRfBands);

      POLICYMAN_MSG_HIGH_2( SUBS_PREFIX SRC_LINE "action <rf_bands> set:",
                            pRfBands->asubs_id,
                            LINE_NUM(prfbAction) );

      policyman_rf_print_bands( pRfBands->asubs_id,
                                pRfBands->rfBands.bands.gwBands,
                                pRfBands->rfBands.bands.lteBands,
                                pRfBands->rfBands.bands.tdsBands);
    }
  }
  MRE_RELEASE_IF(pRfBands);

  return succeeded;
}


/*-------- policyman_rf_bands_action_execute --------*/
STATIC boolean policyman_rf_bands_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_rf_bands);
}


/*-------- execute_rf_bands_if --------*/
STATIC boolean execute_rf_bands_if(
  policyman_action_t const *pAction,
  void                     *pCtx,
  sys_modem_as_id_e_type    subsId
)
{
  mre_policy_t       *pPolicy         = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t          *pItemSet        = POLICY_EXECUTE_ITEMSET(pCtx);
  rf_bands_action_t  *pRfBandsAction  = (rf_bands_action_t *)pAction;
  boolean             succeeded       = TRUE;
  boolean             item_is_present;

  item_is_present = policyman_cfgitem_item_is_present_based_on_lookup_method( pRfBandsAction->lookup_method,
                                                                              pItemSet,
                                                                              POLICYMAN_ITEM_RF_BANDS,
                                                                              subsId );

  if (item_is_present)
  {
    POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "<rf_bands_if pmsubs=%d> skipping update - item present",
                        POLICY_SUBS(pPolicy),
                        LINE_NUM(pRfBandsAction),
                        pRfBandsAction->pmSubsId );
  }
  else
  {
    succeeded = policyman_rf_bands_action_execute((mre_action_t const  *)pAction, pCtx);
  }

  return succeeded;
}


/*-------- policyman_rf_bands_if_action_execute --------*/
STATIC boolean policyman_rf_bands_if_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  return policyman_execute_action((policyman_action_t const *)pAction,
                                   pCtx,
                                   execute_rf_bands_if);
}


/*-------- policyman_rf_bands_create_new_action --------*/
mre_status_t policyman_rf_bands_create_new_action(
  mre_xml_element_t const        *pElem,
  policyman_policy_t             *pPolicy,
  mre_action_t                  **ppAction,
  pfn_execute_t                   executefn
)
{
  mre_status_t             status  = MRE_STATUS_ERR_MALFORMED_XML;
  rf_bands_action_t       *pAction = NULL;
  char const              *pStr;
  policyman_subs_t         pm_subs;

  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  /*  Allocate and initialize the action itself.
   */
  pAction = POLICYMAN_ACTION_NEW( rf_bands_action_t,
                                  (mre_policy_t *)pPolicy,
                                  executefn,
                                  policyman_rf_band_action_dtor,
                                  pm_subs,
                                  pElem );

  //  If this action uses a named bands list, save the name and we're done.
  pStr = policyman_xml_get_attribute(pElem, "list");
  if (pStr != NULL)
  {
    pAction->pListName = policyman_str_dup(pStr);
    status = MRE_STATUS_SUCCESS;
    goto Done;
  }

  /*  Otherwise, read the inline band list.
   */
  pAction->pBands = (rf_bands_t *) policyman_mem_alloc(sizeof(rf_bands_t));
  ref_cnt_obj_init(pAction->pBands, policyman_rf_bands_dtor);
  if (!policyman_rf_band_list_read(pElem, pAction->pBands))
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<rf_bands> invalid specification", POLICY_SUBS(pPolicy));
    goto Done;
  }

  // Search method lookup
  pStr = policyman_xml_get_attribute(pElem, "method");
  pAction->lookup_method = policyman_util_str_to_lookup_method(pStr);

  status = MRE_STATUS_SUCCESS;

Done:
  if (MRE_SUCCEEDED(status))
  {
    *ppAction = (mre_action_t *) pAction;
  }
  else
  {
    MRE_RELEASE_IF(pAction);
  }

  return status;
}


/*-------- policyman_rf_bands_action_new --------*/
mre_status_t policyman_rf_bands_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  return policyman_rf_bands_create_new_action( pElem,
                                               (policyman_policy_t *) pPolicy,
                                               ppAction,
                                               policyman_rf_bands_action_execute );
}

/*-------- policyman_rf_bands_if_action_new --------*/
mre_status_t policyman_rf_bands_if_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  return policyman_rf_bands_create_new_action( pElem,
                                               (policyman_policy_t *) pPolicy,
                                               ppAction,
                                               policyman_rf_bands_if_action_execute );
}


/*=============================================================================
  Methods for the RF band configuration item
=============================================================================*/

/*-------- policyman_rf_bands_get_default_from_efs --------*/
policyman_status_t policyman_rf_bands_get_default_from_efs(
  cfgitem_execute_ctx_t *pCfgCtx
)
{
  rf_efs_bands_t       retrievedBands;
  mre_set_t           *pRfRestrictions;
  rf_bands_item_t     *prfb      = NULL;
  policyman_status_t   status    = POLICYMAN_STATUS_ERR;

  retrievedBands.bands.gwBands  = 0;
  retrievedBands.bands.tdsBands = 0;

  retrievedBands.excludeForVolte.gwBands  = 0;
  retrievedBands.excludeForVolte.tdsBands = 0;

  retrievedBands.excludeFromBst.gwBands  = 0;
  retrievedBands.excludeFromBst.tdsBands = 0;

  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&retrievedBands.bands.lteBands);
  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&retrievedBands.excludeFromBst.lteBands);
  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&retrievedBands.excludeForVolte.lteBands);

  pRfRestrictions = mre_set_new(sizeof(rf_restrictions_t),
                                3,
                                MRE_SET_NO_MAX_SIZE,
                                policyman_rf_restriction_compare,
                                NULL);

  // read EFS for given subscription
  if (policyman_retrieve_rats_bands(pCfgCtx->subsId, NULL, &retrievedBands, pRfRestrictions))
  {
    prfb = (rf_bands_item_t *)policyman_rf_bands_item_new(&retrievedBands, pRfRestrictions, pCfgCtx->subsId);

    pCfgCtx->pItem = (policyman_item_t *)prfb;
    status = POLICYMAN_STATUS_SUCCESS;
  }

  MRE_RELEASE_IF(pRfRestrictions);
  return status;
}

/*-------- policyman_rf_bands_get_default --------*/
policyman_item_t * policyman_rf_bands_get_default(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  rf_bands_item_t             *prfb;

  prfb = policyman_rf_hardware_band_item_new(subsId);

  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Default RF bands:", subsId);
  policyman_rf_print_bands( subsId,
                            prfb->rfBands.bands.gwBands,
                            prfb->rfBands.bands.lteBands,
                            prfb->rfBands.bands.tdsBands);

  return (policyman_item_t *) prfb;
}

/*-------- policyman_rf_bands_compare --------*/
boolean policyman_rf_bands_compare(
  policyman_item_t  *pi1,
  policyman_item_t  *pi2
)
{
  rf_bands_item_t *prf1 = (rf_bands_item_t *) pi1;
  rf_bands_item_t *prf2 = (rf_bands_item_t *) pi2;

  return  policyman_cfgitem_id(pi1) == POLICYMAN_ITEM_RF_BANDS
          &&  policyman_cfgitem_id(pi2) == POLICYMAN_ITEM_RF_BANDS
          &&  prf1->rfBands.bands.gwBands == prf2->rfBands.bands.gwBands
          &&  SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&prf1->rfBands.bands.lteBands, &prf2->rfBands.bands.lteBands)
          &&  prf1->rfBands.bands.tdsBands == prf2->rfBands.bands.tdsBands;

}

/*-------- policyman_rf_print_bands --------*/
void policyman_rf_print_lte_bands(
  sys_modem_as_id_e_type    subsId,
  sys_lte_band_mask_e_type  lteBands
)
{
#if defined(LTE_BAND_NUM) && (LTE_BAND_NUM == 256)
  POLICYMAN_MSG_HIGH_9( SUBS_PREFIX "LTE-256-MSB 0x%08x%08x.%08x%08x\t"
                "LTE-256-LSB 0x%08x%08x.%08x%08x",
                        subsId,
                        (int)(lteBands.bits_193_256 >> 32),
                        (int)(lteBands.bits_193_256),
                        (int)(lteBands.bits_129_192 >> 32),
                        (int)(lteBands.bits_129_192),
                        (int)(lteBands.bits_65_128 >> 32),
                        (int)(lteBands.bits_65_128),
                        (int)(lteBands.bits_1_64 >> 32),
                        (int)(lteBands.bits_1_64) );
#else
  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "LTE-64      0x%08x.%08x",
                        subsId,
                        (int)(lteBands >> 32),
                        (int)(lteBands ) );
#endif
}


/*-------- policyman_rf_print_bands --------*/
void policyman_rf_print_bands(
  sys_modem_as_id_e_type    subsId,
  sys_band_mask_type        gwBands,
  sys_lte_band_mask_e_type  lteBands,
  sys_band_mask_type        tdsBands
)
{

  policyman_rf_print_lte_bands(subsId, lteBands);
  POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "GW-64      0x%08x.%08x  TDS-64      0x%08x.%08x",
                        subsId,
                        (int)(gwBands >> 32),
                        (int)(gwBands), 
                        (int)(tdsBands >> 32),
                        (int)(tdsBands) );
}


/*-------- policyman_rf_bands_display --------*/
void policyman_rf_bands_display(
  policyman_item_t  *pItem
)
{
  rf_bands_item_t *prfb = (rf_bands_item_t *) pItem;

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "RF bands: (from item 0x%08x, seqno %d)",
                        pItem->asubs_id,
                        pItem,
                        pItem->seqno );

  policyman_rf_print_bands( pItem->asubs_id,
                            prfb->rfBands.bands.gwBands,
                            prfb->rfBands.bands.lteBands,
                            prfb->rfBands.bands.tdsBands);
}


/*-------- policyman_rf_bands_on_update --------*/
void policyman_rf_bands_on_update(
  policyman_item_t  *pItem
)
{
  rf_bands_item_t                 *pRfBands = (rf_bands_item_t *)pItem;
  policyman_diag_event_rf_bands_t  event_rf_bands;

  event_rf_bands.gw_bands       = pRfBands->rfBands.bands.gwBands;
  event_rf_bands.lte_band_count = sizeof(sys_lte_band_mask_e_type) / sizeof(uint64);
#if defined(LTE_BAND_NUM) && (LTE_BAND_NUM == 256)
  event_rf_bands.lte_bands_1_64    = pRfBands->rfBands.bands.lteBands.bits_1_64;
  event_rf_bands.lte_bands_65_128  = pRfBands->rfBands.bands.lteBands.bits_65_128;
  event_rf_bands.lte_bands_129_192 = pRfBands->rfBands.bands.lteBands.bits_129_192;
  event_rf_bands.lte_bands_193_256 = pRfBands->rfBands.bands.lteBands.bits_193_256;
#else
  event_rf_bands.lte_bands_1_64    = pRfBands->rfBands.bands.lteBands;
  event_rf_bands.lte_bands_65_128  = 0;
  event_rf_bands.lte_bands_129_192 = 0;
  event_rf_bands.lte_bands_193_256 = 0;
#endif
  event_rf_bands.tds_bands      = pRfBands->rfBands.bands.tdsBands;

  event_report_payload( EVENT_PM_RF_BANDS,
                        sizeof(event_rf_bands),
                        &event_rf_bands );

  return;
}


/*-------- policyman_rf_get_rats --------*/
STATIC uint32 policyman_rf_get_rats(
  sys_band_mask_type       gwBands,
  sys_lte_band_mask_e_type lteBands,
  sys_band_mask_type       tdsBands
)
{
  uint32  ratMask = 0;

  if ((gwBands & ALL_GSM_BANDS) && HAS_CAPABILITY(GSM))
  {
    ratMask |= SYS_SYS_MODE_MASK_GSM;
  }

  if ((gwBands & ALL_WCDMA_BANDS) && HAS_CAPABILITY(WCDMA) && policyman_tech_is_loaded(CLSID_WCDMA))
  {
    ratMask |= SYS_SYS_MODE_MASK_WCDMA;
  }

  if ((gwBands & ALL_CDMA_BANDS) && HAS_CAPABILITY(1X))
  {
    ratMask |= SYS_SYS_MODE_MASK_CDMA;
  }

  if ((gwBands & ALL_HDR_BANDS) && HAS_CAPABILITY(DO))
  {
    ratMask |= SYS_SYS_MODE_MASK_HDR;
  }

  if ( (!SYS_LTE_BAND_MASK_CHK_IF_EMPTY(&lteBands)) && HAS_CAPABILITY(LTE))
  {
    ratMask |= SYS_SYS_MODE_MASK_LTE;
  }

  if ((tdsBands & ALL_TDS_BANDS) && HAS_CAPABILITY(TDSCDMA) && policyman_tech_is_loaded(CLSID_TDSCDMA))
  {
    ratMask |= SYS_SYS_MODE_MASK_TDS;
  }

  return ratMask;
}


/*-------- policyman_get_hardware_rats_bands --------*/
void policyman_get_hardware_bands_all_techs(
  sys_band_mask_type       *pGwBands,
  sys_lte_band_mask_e_type *pLteBands,
  sys_band_mask_type       *pTdsBands
)
{

  /*  Get the GW band information for all devices.
   */
  if ( pGwBands != NULL )
  {
    *pGwBands = rfm_get_aggregated_band_mask();
#ifndef  FEATURE_CDMA
    *pGwBands &= ~(ALL_CDMA_BANDS);
#endif
  }

  /*  LTE bands only if enabled.
   */
  if ( pLteBands != NULL )
  {
    SYS_LTE_BAND_MASK_CLR_ALL_BANDS(pLteBands);
#ifdef  FEATURE_LTE
    *pLteBands = rfm_get_lte_band_mask();
#endif
  }

  /*  TDS bands only on device 0, if enabled, and if loaded.
   */
  if ( pTdsBands != NULL )
  {
    *pTdsBands = 0;
#ifdef  FEATURE_TDSCDMA
    *pTdsBands = rfm_get_tds_band_mask(RFCOM_TRANSCEIVER_0);
#endif
  }
}

/*-------- policyman_get_hardware_rats_bands --------*/
void policyman_get_hardware_rats_bands(
  uint32                   *pRatMask,
  sys_band_mask_type       *pGwBands,
  sys_lte_band_mask_e_type *pLteBands,
  sys_band_mask_type       *pTdsBands
)
{
  sys_band_mask_type       gwBands;
  sys_band_mask_type       tdsBands;
  sys_lte_band_mask_e_type lteBands;
  static boolean           isRfmInitDone = FALSE;

  if (!isRfmInitDone)
  {
    mcs_rcevt_wait_name("RFM_INIT_COMPLETE");
    isRfmInitDone = TRUE;
  }

  policyman_get_hardware_bands_all_techs( &gwBands, &lteBands, &tdsBands );

  if (!policyman_tech_is_loaded(CLSID_WCDMA))
  {
    gwBands &= ~ALL_WCDMA_BANDS;
  }

  if (!policyman_tech_is_loaded(CLSID_TDSCDMA))
  {
    tdsBands = 0;
  }

  if ( pGwBands != NULL )
  {
    *pGwBands = gwBands;
  }
  if ( pLteBands != NULL )
  {
    *pLteBands = lteBands;
  }
  if ( pTdsBands != NULL )
  {
    *pTdsBands = tdsBands;
  }

  /*  Combine the information from all devices into the overall device info.
   */
  if ( pRatMask != NULL )
  {
    *pRatMask = policyman_rf_get_rats(gwBands, lteBands, tdsBands);
  }
}


/*-------- policyman_rf_determine_volte_restricted_e911_rats --------*/
/**
@brief  Return the RATs allowed for e911 calls when volte is enabled.
        This is a synchronous call that can be made at any time.

        Should only be called when volte is enabled and appropriate
        boolean is true.  Assumes these checks have been
        done by caller.

@return
  No return value. The supplied rat mask is modified as necessary.
*/
void policyman_rf_determine_volte_restricted_e911_rats(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  uint32                  *ratMask
)
{
  sys_mcc_type       mcc;
  boolean            mcc_is_in_japan;
  boolean            csim_is_enabled;

  mcc              = policyman_state_get_subs_mcc(pState, subsId);
  mcc_is_in_japan  = policyman_plmn_mcc_is_volte_only(mcc, subsId);
  csim_is_enabled  = (MMGSDI_APP_CSIM == policyman_uim_get_cdma_sim_type(pState, subsId));

  if (mcc_is_in_japan && csim_is_enabled)
  {
    // Hardware minus C H T
    *ratMask &= ~(SYS_SYS_MODE_MASK_CDMA | SYS_SYS_MODE_MASK_HDR | SYS_SYS_MODE_MASK_TDS);
  }
  else if (csim_is_enabled)
  {
    // Hardware minus T
    *ratMask &= ~(SYS_SYS_MODE_MASK_TDS);
  }
  else
  {
    // Hardware minus C H
    *ratMask &= ~(SYS_SYS_MODE_MASK_CDMA | SYS_SYS_MODE_MASK_HDR);
  }

  return;
}

/*-------- policyman_rf_determine_restricted_e911_rats --------*/
/**
@brief  Return the RATs allowed for e911 calls .
        This is a synchronous call that can be made at any time.

        Should only be called when L+L  is allowed.

@return
  No return value. The supplied rat mask is modified as necessary.
*/
void policyman_rf_determine_restricted_e911_rats(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  uint32                  *ratMask
)
{
  sys_modem_as_id_e_type ddsSub = policyman_subs_get_dds_subs(pState);

  if (subsId == ddsSub)
  {
    *ratMask &= ~(SYS_SYS_MODE_MASK_HDR);
  }
  else
  {
    *ratMask &= ~(SYS_SYS_MODE_MASK_HDR | SYS_SYS_MODE_MASK_TDS | SYS_SYS_MODE_MASK_LTE);
  }
}

/*-------- policyman_get_e911_rat_mask --------*/
/**
@brief  Return the RATs allowed for e911 calls.
        This is a synchronous call that can be made at any time.

@return
  ratMask  The allowed RAT mask.
*/
uint32 policyman_get_e911_rat_mask(
  sys_modem_as_id_e_type  subsId
)
{
  boolean                      mmcp_e911_volte = FALSE;
  boolean                      volte_is_enabled;
  policyman_rf_device_info_t   hwRatsAndBands;
  uint32                       ratMask;
  policyman_state_t           *pState;

  (void)policyman_get_subs_boolean_value("mmcp:e911_volte", subsId, &mmcp_e911_volte);

  policyman_state_enter_crit_section();

  pState           = policyman_state_get_state();
  volte_is_enabled = policyman_ph_get_volte_is_enabled(pState, subsId);

  // Fetch hardware rats and bands
  policyman_get_hardware_rats_bands( &hwRatsAndBands.ratMask,
                                     &hwRatsAndBands.gwBands,
                                     &hwRatsAndBands.lteBands,
                                     &hwRatsAndBands.tdsBands );

  // Start with hardware RATs
  ratMask = hwRatsAndBands.ratMask;

  if ( mmcp_e911_volte && volte_is_enabled )
  {
    policyman_rf_determine_volte_restricted_e911_rats(pState, subsId, &ratMask);
  }

  policyman_rf_determine_restricted_e911_rats(pState, subsId, &ratMask);

  policyman_state_leave_crit_section();

  POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "e911_rat_mask: hw 0x%04x, ret 0x%04x, e911_volte (T/F): %d, volte_is_enabled %d",
                        subsId,
                        hwRatsAndBands.ratMask,
                        ratMask,
                        mmcp_e911_volte,
                        volte_is_enabled );

  return ratMask;
}

/*-------- policyman_rf_init --------*/
void policyman_rf_init(
  void
)
{
  policyman_rf_device_info_t  *pOverall;

  pOverall = &device_rf_info.overall;
  policyman_get_hardware_rats_bands( &pOverall->ratMask,
                                     &pOverall->gwBands,
                                     &pOverall->lteBands,
                                     &pOverall->tdsBands );

  POLICYMAN_MSG_HIGH_1("Default RAT mask: 0x%04x", pOverall->ratMask);
  POLICYMAN_MSG_HIGH_0("Default RF bands:");
  policyman_rf_print_bands(SYS_MODEM_AS_ID_NONE, pOverall->gwBands, pOverall->lteBands, pOverall->tdsBands);

  if (pOverall->ratMask == SYS_SYS_MODE_MASK_NONE)
  {
    policyman_cfgitem_set_limited_capabilities(POLICYMAN_CFG_ERROR_DEVICE_IS_NOT_CALIBRATED);
  }
}


/*-------- policyman_rf_deinit --------*/
void policyman_rf_deinit(
  void
)
{
}


/*-------- policyman_rf_get_overall_info --------*/
policyman_rf_device_info_t * policyman_rf_get_overall_info(
  void
)
{
  policyman_rf_device_info_t  *pOverall;

  pOverall = &device_rf_info.overall;

  return pOverall;
}


/*-------- policyman_rf_get_overall_rat_mask --------*/
uint32 policyman_rf_get_overall_rat_mask(
  void
)
{
  policyman_rf_device_info_t *pOverall;

  pOverall = policyman_rf_get_overall_info();

  return pOverall->ratMask;
}


/*-------- policyman_validate_rf_bands_item --------*/
STATIC policyman_status_t policyman_validate_rf_bands_item(
  rf_bands_item_t *prf
)
{
  if (prf == NULL)
  {
    POLICYMAN_MSG_ERROR_0("get_rf_bands returned err POLICYMAN_STATUS_ERR_INVALID_ARGS");
    return POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }

  if (policyman_cfgitem_id((policyman_item_t *) prf) != POLICYMAN_ITEM_RF_BANDS)
  {
    POLICYMAN_MSG_ERROR_0("get_rf_bands returned err POLICYMAN_STATUS_ERR_INVALID_ITEM_ID");
    return POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
  }

  return POLICYMAN_STATUS_SUCCESS;
}


/*-------- policyman_get_rf_bands --------*/
policyman_status_t policyman_get_rf_bands(
  policyman_item_t const    *pItem,
  sys_band_mask_type        *pGWbands,
  sys_lte_band_mask_e_type  *pLTEbands,
  sys_band_mask_type        *pTDSbands
)
{
  policyman_status_t   status = POLICYMAN_STATUS_SUCCESS;
  rf_bands_item_t     *prf   = (rf_bands_item_t *) pItem;

  status = policyman_validate_rf_bands_item(prf);

  if (POLICYMAN_SUCCEEDED(status))
  {
    if (pGWbands != NULL)
    {
      *pGWbands = prf->rfBands.bands.gwBands;
    }
    if (pLTEbands != NULL)
    {
      *pLTEbands = prf->rfBands.bands.lteBands;
    }
    if (pTDSbands != NULL)
    {
      *pTDSbands = prf->rfBands.bands.tdsBands;
    }

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "get_rf_bands (from item 0x%08x, seqno %d) RF bands:",
                          pItem->asubs_id,
                          pItem,
                          pItem->seqno );

    policyman_rf_print_bands( pItem->asubs_id,
                              prf->rfBands.bands.gwBands,
                              prf->rfBands.bands.lteBands,
                              prf->rfBands.bands.tdsBands);
  }

  return status;
}


/*-------- policyman_rf_bands_get_restriction_set --------*/
policyman_status_t policyman_rf_bands_get_restriction_set(
  policyman_item_t const  *pItem,
  mre_set_t              **ppRestrictionSet
)
{
  policyman_status_t   status = POLICYMAN_STATUS_SUCCESS;
  rf_bands_item_t     *prf   = (rf_bands_item_t *) pItem;

  status = policyman_validate_rf_bands_item(prf);

  if (POLICYMAN_SUCCEEDED(status))
  {
    *ppRestrictionSet = prf->pRestrictions;
    ref_cnt_obj_add_ref(prf->pRestrictions);
  }

  return status;
}


/*-------- policyman_get_rf_bands_force --------*/
policyman_status_t policyman_get_rf_bands_force(
  policyman_item_t const  *pItem,
  boolean                 *pForce
)
{
  policyman_status_t   status = POLICYMAN_STATUS_SUCCESS;
  rf_bands_item_t     *prf    = (rf_bands_item_t *) pItem;

  status = policyman_validate_rf_bands_item(prf);

  if (POLICYMAN_STATUS_SUCCESS == status)
  {
    *pForce = FALSE;
  }

  return status;
}

/*-------- policyman_get_rf_scan_bands --------*/
policyman_status_t policyman_get_rf_scan_bands(
  policyman_item_t const    *pItem,
  sys_band_mask_type        *pGWbands,
  sys_lte_band_mask_e_type  *pLTEbands,
  sys_band_mask_type        *pTDSbands
)
{
  policyman_status_t        status       = POLICYMAN_STATUS_SUCCESS;
  rf_bands_item_t          *prf          = (rf_bands_item_t *) pItem;
  sys_lte_band_mask_e_type  lteScanBand;

  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&lteScanBand);
  status = policyman_validate_rf_bands_item(prf);

  if (POLICYMAN_SUCCEEDED(status))
  {
    if (pGWbands != NULL)
    {
      *pGWbands = prf->rfBands.bands.gwBands & ~prf->rfBands.excludeFromBst.gwBands;
    }
    if (pLTEbands != NULL)
    {
      *pLTEbands = prf->rfBands.bands.lteBands;
      SYS_LTE_BAND_MASK_REMOVE_MASK(pLTEbands, &prf->rfBands.excludeFromBst.lteBands);
      lteScanBand = *pLTEbands;
    }
    if (pTDSbands != NULL)
    {
      *pTDSbands = prf->rfBands.bands.tdsBands & ~prf->rfBands.excludeFromBst.tdsBands;
    }

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "get_rf_scan_bands (from item 0x%08x, seqno %d) RF bands:",
                          pItem->asubs_id,
                          pItem,
                          pItem->seqno );

    policyman_rf_print_bands( pItem->asubs_id,
                              prf->rfBands.bands.gwBands & ~prf->rfBands.excludeFromBst.gwBands,
                              lteScanBand,
                              prf->rfBands.bands.tdsBands & ~prf->rfBands.excludeFromBst.tdsBands);
  }

  return status;
}

/*-------- policyman_subs_is_in_excluded_location --------*/
STATIC boolean policyman_subs_is_in_excluded_location(
  sys_modem_as_id_e_type    subsId
)
{
  mre_plmn_mcc_set_t *pMccSet;
  boolean             result;
  policyman_state_t  *pState;

  policyman_enter_namespace_crit_sect();

  pMccSet = (mre_plmn_mcc_set_t *) mre_named_object_find_with_subs( PM_EXCLUDE_BAND_MCCS,
                                                                    POLICYMAN_NAMED_MCC_SET,
                                                                    subsId,
                                                                    policyman_get_namespace_info());
  if (NON_NULL(pMccSet))
  {
    ref_cnt_obj_add_ref(pMccSet);
  }

  policyman_leave_namespace_crit_sect();

  if (IS_NULL(pMccSet))
  {
    sys_mcc_type  japan_mcc[] = {440, 441};

    pMccSet  =  policyman_create_mcc_set(subsId, japan_mcc, ARR_SIZE(japan_mcc));
  }

  pState = policyman_state_get_state();

  result = location_is_in_mcc_set(pMccSet->pPolicy, pState, subsId, pMccSet);

  MRE_RELEASE_IF(pMccSet);
  return result;
}


/*-------- policyman_get_unexcluded_volte_bands --------*/
policyman_status_t policyman_get_unexcluded_volte_bands(
  policyman_item_t const    *pItem,
  sys_lte_band_mask_e_type  *pLTEbands
)
{
  policyman_status_t        status;
  rf_bands_item_t          *prf = (rf_bands_item_t *) pItem;

  status = policyman_validate_rf_bands_item(prf);

  if (POLICYMAN_SUCCEEDED(status))
  {
    *pLTEbands = prf->rfBands.bands.lteBands;

    if (policyman_subs_is_in_excluded_location(pItem->asubs_id) )
    {
      SYS_LTE_BAND_MASK_ADD_BAND(&prf->rfBands.excludeForVolte.lteBands, SYS_SBAND_LTE_EUTRAN_BAND41);
      SYS_LTE_BAND_MASK_REMOVE_MASK(pLTEbands, &prf->rfBands.excludeForVolte.lteBands);
    }

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "get_unexcluded_volte_bands (from item 0x%08x, seqno %d) RF Bands:",
                          pItem->asubs_id,
                          pItem,
                          pItem->seqno );

    policyman_rf_print_lte_bands(pItem->asubs_id, *pLTEbands);
  }
  return status;
}

/*-------- policyman_get_bands_for_named_object --------*/
void policyman_get_bands_for_named_object(
  rf_bands_t         *pBands,
  rf_bands_t         *pGetBands
)
{
  pGetBands->bands.gwBands  = pBands->bands.gwBands;
  pGetBands->bands.lteBands = pBands->bands.lteBands;
  pGetBands->bands.tdsBands = pBands->bands.tdsBands;

  pGetBands->bandsBase.gwBandsBase  = pBands->bandsBase.gwBandsBase;
  pGetBands->bandsBase.lteBandsBase = pBands->bandsBase.lteBandsBase;
  pGetBands->bandsBase.tdsBandsBase = pBands->bandsBase.tdsBandsBase;
}


/*-------- policyman_rf_bands_filter_based_on_rat --------*/
void policyman_rf_bands_filter_based_on_rat(
  rf_bands_item_t  *prfb,
  uint32            ratMask
)
{
  if ( (ratMask & (SYS_SYS_MODE_MASK_WCDMA|SYS_SYS_MODE_MASK_TDS)) == (SYS_SYS_MODE_MASK_WCDMA|SYS_SYS_MODE_MASK_TDS))
  {
    POLICYMAN_MSG_MED_0("RAT mask has WCDMA and TDS: bands not filtered");
  }
  else if ( (ratMask & SYS_SYS_MODE_MASK_WCDMA) == SYS_SYS_MODE_MASK_WCDMA)
  {
    POLICYMAN_MSG_MED_0("RAT mask has WCDMA: removing TDS bands");
    prfb->rfBands.bands.tdsBands = 0;
  }
}

/*-------- policyman_rf_bands_fill_subs_diag_log --------*/
void policyman_rf_bands_fill_subs_diag_log(
  policyman_item_t const  *pItem,
  LOG_PM_SUBS_INFO_type   *log_ptr
)
{
  rf_bands_item_t     *pRfBands = (rf_bands_item_t *)pItem;

  log_ptr->gw_bands       = pRfBands->rfBands.bands.gwBands;
  log_ptr->lte_band_count = sizeof(sys_lte_band_mask_e_type) / sizeof(uint64);
#if defined(LTE_BAND_NUM) && (LTE_BAND_NUM == 256)
  log_ptr->lte_bands_0    = pRfBands->rfBands.bands.lteBands.bits_1_64;
  log_ptr->lte_bands_1    = pRfBands->rfBands.bands.lteBands.bits_65_128;
  log_ptr->lte_bands_2    = pRfBands->rfBands.bands.lteBands.bits_129_192;
  log_ptr->lte_bands_3    = pRfBands->rfBands.bands.lteBands.bits_193_256;
#else
  log_ptr->lte_bands_0    = pRfBands->rfBands.bands.lteBands;
  log_ptr->lte_bands_1    = 0;
  log_ptr->lte_bands_2    = 0;
  log_ptr->lte_bands_3    = 0;
#endif
  log_ptr->tds_bands      = pRfBands->rfBands.bands.tdsBands;
}

/*-------- policyman_rf_bands_mdump --------*/
QSH_MDUMP_FN_ATTR_PM void policyman_rf_bands_mdump(
  policyman_item_t const *pItem
)
{
  policyman_qsh_dump_collect((void *)pItem, sizeof(rf_bands_item_t));
}
