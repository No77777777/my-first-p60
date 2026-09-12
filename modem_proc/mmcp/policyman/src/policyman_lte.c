/**

  @file policy_lte.c

  @brief  Policy which reads the ca_combo and makes it available
          to the rest of modem.
*/

/*
    Copyright (c) 2015-2018,2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_lte.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_efs.h"

#include "policyman_cfgitem.h"
#include "policyman_device_config.h"
#include "policyman_efs.h"
#include "policyman_lte_i.h"
#include "policyman_plmn.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_rules.h"
#include "policyman_serving_system.h"
#include "policyman_subs.h"
#include "policyman_util.h"
#include "policyman_xml.h"


#include <stringl/stringl.h>
#include <string.h>


typedef struct
{
  POLICYMAN_ACTION_BASE;

  char  const  *pCombos;
  char  const  *pPlmn;
} ca_combo_action_t;

typedef struct
{
  MRE_NAMED_OBJECT_BASE;

  char const *pCombos;
} named_ca_combo_t;

/*=============================================================================
 Data Structure for CA
==============================================================================*/
struct policyman_ca_combo_item_t
{
  POLICYMAN_ITEM;

  char  const  *pCombos;
};


/*-------- policyman_ca_dtor --------*/
STATIC void policyman_ca_dtor(
  void  *pObj
)
{
  policyman_ca_combo_item_t *pMe = (policyman_ca_combo_item_t *)pObj;

  POLICYMAN_MEM_FREE_IF(pMe->pCombos);
  mre_simple_dtor(pObj);
}


/*-------- policyman_named_config_dtor --------*/
STATIC void policyman_named_ca_dtor(
  void  *pObj
)
{
  named_ca_combo_t *pMe = (named_ca_combo_t *) pObj;

  POLICYMAN_MEM_FREE_IF(pMe->pCombos);
  mre_named_object_dtor(pObj);
}


/*-------- policyman_ca_combo_item_new --------*/
policyman_ca_combo_item_t* policyman_ca_combo_item_new(
  char  const             *pCombos,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_ca_combo_item_t  *pItem;

  pItem = (policyman_ca_combo_item_t *)policyman_cfgitem_new_item( POLICYMAN_ITEM_CA_BAND_COMBOS,
                                                                   sizeof(policyman_ca_combo_item_t),
                                                                   policyman_ca_dtor,
                                                                   subsId);
  pItem->pCombos   = policyman_str_dup(pCombos);

  return pItem;
}

/*-------- policyman_ca_combo_clone --------*/
policyman_item_t * policyman_ca_combo_clone(
  policyman_item_t const *pItem
)
{
  policyman_ca_combo_item_t const *pCaCombo       = (policyman_ca_combo_item_t const *)pItem;
  policyman_ca_combo_item_t       *pCaComboClone;

  pCaComboClone = policyman_ca_combo_item_new(pCaCombo->pCombos, pCaCombo->asubs_id);

  return (policyman_item_t *)pCaComboClone;
}

/*-------- policyman_ca_combo_get_default --------*/
policyman_item_t* policyman_ca_combo_get_default(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_ca_combo_item_t   *pItem;

  pItem = policyman_ca_combo_item_new(NULL, subsId);
  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "ca_combo default is NULL", subsId);

  return (policyman_item_t *)pItem;
}


/*-------- policyman_ca_combo_compare --------*/
boolean policyman_ca_combo_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
)
{
  policyman_ca_combo_item_t  *pCombo1 = (policyman_ca_combo_item_t *)pData1;
  policyman_ca_combo_item_t  *pCombo2 = (policyman_ca_combo_item_t *)pData2;
  boolean                     status  = TRUE;


  if ((pCombo1 == NULL) || (pCombo2 == NULL))
  {
    status = FALSE;
    goto Done;
  }

  // In case both the stings are NULL. There is no updation
  if ((pCombo1->pCombos == NULL) && (pCombo2->pCombos == NULL))
  {
    status = TRUE;
    goto Done;
  }

  // In case one string is NULL. Need to notify clients
  if ((pCombo1->pCombos == NULL) || (pCombo2->pCombos == NULL))
  {
    status = FALSE;
    goto Done;
  }

  status = (strcmp(pCombo1->pCombos, pCombo2->pCombos) == 0);

Done:
  POLICYMAN_UT_MSG_1("ca_combo_compare status: %d", status);
  return status;
}


/*-------- policyman_ca_combo_create_named --------*/
STATIC boolean policyman_ca_combo_create_named(
  char const         *pPlmn,
  char const         *pBandCombos
)
{
  named_ca_combo_t     *pNamedCombo;
  mre_namespace_info_t *pNamespace;
  boolean               succeeded;

  policyman_enter_namespace_crit_sect();
  pNamespace = policyman_get_namespace_info();

  pNamedCombo = (named_ca_combo_t *) policyman_mem_alloc(sizeof(named_ca_combo_t));
  ref_cnt_obj_init(pNamedCombo, policyman_named_ca_dtor);

  mre_named_object_init( (mre_named_object_t *)pNamedCombo,
                               pPlmn,
                               POLICYMAN_NAMED_CA_BAND );

  pNamedCombo->pCombos = policyman_str_dup(pBandCombos);

  succeeded = mre_named_object_add_in_set( (mre_named_object_t *)pNamedCombo,
                                           &pNamespace->pGlobalNamedObjs );

  ref_cnt_obj_release(pNamedCombo);
  policyman_leave_namespace_crit_sect();

  return succeeded;
}


/*-------- policyman_ca_combo_create_named_from_plmn --------*/
STATIC boolean policyman_ca_combo_create_named_from_plmn(
  sys_plmn_id_s_type *pPlmn,
  char const         *pBandCombos
)
{
  char objectName[8] = {0};

  policyman_plmn_to_ascii(pPlmn, objectName, sizeof(objectName));
  return policyman_ca_combo_create_named(objectName, pBandCombos);
}


/*-------- policyman_ca_combo_retrieve_named --------*/
STATIC named_ca_combo_t * policyman_ca_combo_retrieve_named(
  sys_plmn_id_s_type *pPlmn
)
{
  named_ca_combo_t     *pNamedCombo;
  mre_namespace_info_t *pNamespace;
  char                  objectName[8] = {0};

  policyman_plmn_to_ascii(pPlmn, objectName, sizeof(objectName));

  policyman_enter_namespace_crit_sect();
  pNamespace = policyman_get_namespace_info();

  pNamedCombo = (named_ca_combo_t *) mre_named_object_find_in_set( objectName,
                                                                   MRE_NAMED_CA_BAND,
                                                                   pNamespace->pGlobalNamedObjs );
  policyman_leave_namespace_crit_sect();
  return pNamedCombo;
}

/*-------- policyman_ca_combo_read_from_efs --------*/
STATIC mre_status_t policyman_ca_combo_read_from_efs(
  sys_plmn_id_s_type  *pPlmn,
  char const         **ppBandCombos,
  size_t              *pComboLen
)
{
  mre_efs_status_t efs_status    = MRE_EFS_STATUS_SUCCESS;
  mre_status_t     status        = MRE_STATUS_SUCCESS;
  uint32           mcc;
  uint32           mnc;
  boolean          mnc_includes_pcs_digit;
  char             fileName[128] = {0};
  size_t           size;
  char const      *pCaBands      = NULL;
  char             plmnStr[8];

  policyman_plmn_get_mcc_mnc(pPlmn, &mnc_includes_pcs_digit, &mcc, &mnc);

  if (mnc_includes_pcs_digit)
  {
    snprintf(plmnStr, sizeof(plmnStr), "%03d_%03d", (int)mcc, (int)mnc);
  }
  else
  {
    snprintf(plmnStr, sizeof(plmnStr), "%03d_%02d", (int)mcc, (int)mnc);
  }

  strlcpy(fileName, POLICYMAN_CA_COMBO_ROOT_DIR"/ca_bands_", sizeof(fileName));
  strlcat(fileName, plmnStr, sizeof(fileName));

  efs_status = mre_efs_get_file( fileName,
                                 (void **)&pCaBands,
                                 &size,
                                 SYS_MODEM_AS_ID_NONE );

  if (MRE_EFS_STATUS_SUCCESS != efs_status)
  {
    status = MRE_STATUS_ERR_NOT_PRESENT;
    goto Done;
  }

  *ppBandCombos = pCaBands;
  *pComboLen    = size;

Done:
  return status;
}


/*-------- policyman_ca_combo_persist_to_efs --------*/
STATIC mre_efs_status_t policyman_ca_combo_persist_to_efs(
  sys_plmn_id_s_type *pPlmn,
  char const         *pBandCombos
)
{
  mre_efs_status_t efs_status    = MRE_EFS_STATUS_SUCCESS;
  uint32           mcc;
  uint32           mnc;
  boolean          mnc_includes_pcs_digit;
  char             fileName[128] = {0};
  char             plmnStr[8];

  policyman_plmn_get_mcc_mnc(pPlmn, &mnc_includes_pcs_digit, &mcc, &mnc);

  if (mnc_includes_pcs_digit)
  {
    snprintf(plmnStr, sizeof(plmnStr), "%03d_%03d", (int)mcc, (int)mnc);
  }
  else
  {
    snprintf(plmnStr, sizeof(plmnStr), "%03d_%02d", (int)mcc, (int)mnc);
  }

  strlcpy(fileName, POLICYMAN_CA_COMBO_ROOT_DIR"/ca_bands_", sizeof(fileName));
  strlcat(fileName, plmnStr, sizeof(fileName));

  efs_status = mre_efs_put_file( fileName,
                                 (void *)pBandCombos,
                                 strlen(pBandCombos) + 1,
                                 TRUE,
                                 SYS_MODEM_AS_ID_NONE );

  return efs_status;
}


/*-------- policyman_ca_combo_display --------*/
void policyman_ca_combo_display(
  policyman_item_t  *pItem
)
{
  policyman_ca_combo_item_t  *eCombo = (policyman_ca_combo_item_t *)pItem;

  POLICYMAN_MSG_S2( SUBS_PREFIX "CA band combo '%s'",
                    eCombo->asubs_id,
                    eCombo->pCombos == NULL ? "" : eCombo->pCombos );
}

/*-------- execute_ca_combo --------*/
static boolean execute_ca_combo(
  policyman_action_t const     *pAction,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  ca_combo_action_t           *pCaComboAction =  (ca_combo_action_t *)pAction;
  mre_policy_t                *pPolicy        = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t                   *pItemSet       = POLICY_EXECUTE_ITEMSET(pCtx);
  policyman_ca_combo_item_t   *pCaComboItem;

  POLICYMAN_MSG_HIGH_1(SRC_LINE "executing <ca_band_combos> with no PLMN", LINE_NUM(pCaComboAction));
  pCaComboItem = policyman_ca_combo_item_new(pCaComboAction->pCombos, POLICY_SUBS(pPolicy));

  mre_set_replace(pItemSet, &pCaComboItem);
  ref_cnt_obj_release(pCaComboItem);

  return TRUE;
}

/*-------- policyman_ca_combo_execute --------*/
STATIC boolean policyman_ca_combo_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const  *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_ca_combo);
}


/*-------- policyman_ca_combo_with_plmn_execute --------*/
STATIC boolean execute_ca_combo_with_plmn(
  policyman_action_t const  *pAction,
  void                      *pCtx,
  sys_modem_as_id_e_type     subsId
)
{
  ca_combo_action_t      *pCaComboAction =  (ca_combo_action_t *)pAction;
  char const             *pStr = NULL;
  size_t                  len;
  sys_plmn_id_s_type      plmn;
  mre_status_t            status = MRE_STATUS_FAILED;

  if (NULL == pCaComboAction->pCombos)
  {
    pCaComboAction->pCombos = policyman_mem_alloc(sizeof(char));
  }

  if (mre_parse_ascii_plmn(pCaComboAction->pPlmn, &plmn))
  {
    status = policyman_ca_combo_read_from_efs(&plmn, &pStr, &len);

    if (status == MRE_STATUS_SUCCESS)
    {
      POLICYMAN_MEM_FREE_IF(pCaComboAction->pCombos);
      pCaComboAction->pCombos = pStr;
    }
    else
    {
      policyman_ca_combo_persist_to_efs(&plmn, pCaComboAction->pCombos);
    }
  }
  else
  {
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX SRC_LINE "Invalid PLMN in ca_band_combo, could not persist to EFS",
                          subsId,
                          LINE_NUM(pCaComboAction) );
  }

  POLICYMAN_MSG_HIGH_1(SRC_LINE "executing <ca_band_combos> for PLMN", LINE_NUM(pCaComboAction));
  policyman_ca_combo_create_named(pCaComboAction->pPlmn, pCaComboAction->pCombos);

  POLICYMAN_MEM_FREE_IF(pCaComboAction->pCombos);
  POLICYMAN_MEM_FREE_IF(pCaComboAction->pPlmn);

  return TRUE;
}

/*-------- policyman_ca_combo_with_plmn_execute --------*/
STATIC boolean policyman_ca_combo_with_plmn_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const  *pAct = (policyman_action_t const *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_ca_combo_with_plmn);
}

/*-------- ca_combo_action_dtor --------*/
STATIC void ca_combo_action_dtor(
  void  *pObj
)
{
  ca_combo_action_t  *pAction = (ca_combo_action_t *) pObj;

  mre_mem_free((void *)pAction->pCombos);
  policyman_action_dtor(pObj);
}

/*-------- policyman_ca_combo_action_new --------*/
mre_status_t policyman_ca_combo_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t            status   = MRE_STATUS_ERR_INVALID_ACTION;
  ca_combo_action_t      *pAction  = NULL;
  pfn_execute_t           pExecute = policyman_ca_combo_execute;
  char const             *pStr;
  char const             *pPlmn;

  pStr  = policyman_xml_get_attribute(pElem, "string");
  pPlmn = policyman_xml_get_attribute(pElem, "plmn");

  if (pPlmn != NULL)
  {
    pExecute = policyman_ca_combo_with_plmn_execute;
  }

  pAction = POLICYMAN_ACTION_NEW( ca_combo_action_t,
                                  pPolicy,
                                  pExecute,
                                  ca_combo_action_dtor,
                                  SUBS_THIS,
                                  pElem );

  pAction->pCombos   = policyman_str_dup(pStr);
  pAction->pPlmn     = policyman_str_dup(pPlmn);

  *ppAction = (mre_action_t *)pAction;

  status = MRE_STATUS_SUCCESS;

  return status;
}


/*=============================================================================
  Accessor function for CA Combo
=============================================================================*/


/*-------- policyman_ca_band_combos_get_string --------*/
policyman_status_t policyman_ca_band_combos_get_string(
  policyman_item_t const  *pItem,
  char const             **ppCombo
)
{
  policyman_ca_combo_item_t  *pCaItem = (policyman_ca_combo_item_t *) pItem;
  policyman_status_t          status;

  if (pCaItem == NULL)
  {
    status = POLICYMAN_STATUS_ERR_INVALID_ARGS;
    goto Done;
  }

  if (policyman_cfgitem_id(pItem) != POLICYMAN_ITEM_CA_BAND_COMBOS)
  {
    status = POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
    goto Done;
  }

  *ppCombo = pCaItem->pCombos;

  POLICYMAN_UT_MSG_S2( SUBS_PREFIX "policyman_ca_band_combos_get_string() returned ca_combo '%s'",
                       pItem->asubs_id,
                       pCaItem->pCombos );

  status = POLICYMAN_STATUS_SUCCESS;

Done:
  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman_ca_band_combos_get_string() status %d",
                        pItem == NULL ? SYS_MODEM_AS_ID_NONE : pItem->asubs_id,
                        status );
  return status;
}


/*-------- ca_band_combos_update --------*/
boolean ca_band_combos_update(
  sys_plmn_id_s_type      *pPlmn,
  char const              *pBandCombos
)
{
  boolean            succeeded = FALSE;
  mre_efs_status_t   mre_status;
  named_ca_combo_t  *pNamedCombo;
  policyman_sandbox_t *pSandbox;

  if (NULL == pBandCombos)
  {
    pBandCombos = "";
  }

  mre_status = policyman_ca_combo_persist_to_efs(pPlmn, pBandCombos);

  if (MRE_EFS_STATUS_SUCCESS == mre_status)
  {
    pNamedCombo = policyman_ca_combo_retrieve_named(pPlmn);

    if (pNamedCombo != NULL)
    {
      POLICYMAN_MEM_FREE_IF(pNamedCombo->pCombos);
      pNamedCombo->pCombos = policyman_str_dup(pBandCombos);
      succeeded = TRUE;
    }
    else
    {
      succeeded = policyman_ca_combo_create_named_from_plmn(pPlmn, pBandCombos);
    }
  }

  pSandbox = policyman_get_global_sandbox();
  policyman_state_handle_update(pSandbox);

  return succeeded;
}


/*-------- ca_band_combos_get --------*/
size_t ca_band_combos_get(
  sys_plmn_id_s_type      *pPlmn,
  char                    *pBandCombos,
  size_t                   bandCombosLen
)
{
  size_t             actualLen = 0;
  mre_status_t       mre_status;
  char const        *pCombos   = NULL;
  named_ca_combo_t  *pNamedCombo;

  // Search named objects
  pNamedCombo = policyman_ca_combo_retrieve_named(pPlmn);

  if (pNamedCombo != NULL)
  {
    actualLen = strlen(pNamedCombo->pCombos) + 1;

    strlcpy(pBandCombos, pNamedCombo->pCombos, bandCombosLen);

    goto Done;
  }

  // Search EFS.  If found, create named object for future lookups
  mre_status = policyman_ca_combo_read_from_efs(pPlmn, &pCombos, &actualLen);

  if (MRE_SUCCEEDED(mre_status))
  {
    actualLen = strlen(pCombos) + 1;
    strlcpy(pBandCombos, pCombos, bandCombosLen);
    policyman_ca_combo_create_named_from_plmn(pPlmn, pCombos);
  }
  else
  {
    actualLen = 0;
  }

  POLICYMAN_MEM_FREE_IF(pCombos);

Done:
  return actualLen;
}


/*-------- policyman_ca_band_combos_is_present  --------*/
STATIC boolean policyman_ca_band_combos_is_present(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  boolean           item_is_present;
  policyman_item_t *pItem            = NULL;

  item_is_present = policyman_cfgitem_is_present_in_itemset( pItemSet,
                                                             POLICYMAN_ITEM_CA_BAND_COMBOS,
                                                             subsId,
                                                             &pItem );

  REF_CNT_OBJ_RELEASE_IF(pItem);

  return item_is_present;
}


/*-------- policyman_get_ca_band_combos --------*/
char const * policyman_get_ca_band_combos(
  sys_plmn_id_s_type plmn
)
{
  mre_status_t       mre_status;
  char const        *pCombos = NULL;
  size_t             len;
  named_ca_combo_t  *pNamedCombo;

  // Search named objects
  pNamedCombo = policyman_ca_combo_retrieve_named(&plmn);

  if (pNamedCombo != NULL)
  {
    pCombos = policyman_str_dup(pNamedCombo->pCombos);
    goto Done;
  }

  // Search EFS.  If found, create named object for future lookups
  mre_status = policyman_ca_combo_read_from_efs(&plmn, &pCombos, &len);

  if (MRE_SUCCEEDED(mre_status))
  {
    policyman_ca_combo_create_named_from_plmn(&plmn, pCombos);
  }

Done:
  return pCombos;
}


/*-------- policyman_ca_band_combos_evaluate_per_subs --------*/
STATIC void policyman_ca_band_combos_evaluate_per_subs(
  policyman_state_t       *pState,
  mre_set_t               *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  sys_plmn_id_s_type plmn;
  boolean            subHas3GPPMcc;
  boolean            haveCaBandPresentInItemset;

  haveCaBandPresentInItemset = policyman_ca_band_combos_is_present(pItemSet, subsId);
  subHas3GPPMcc              = policyman_ss_get_3GPP_plmn_from_subs(pState, subsId, &plmn);

  if (!haveCaBandPresentInItemset && subHas3GPPMcc)
  {
    char const                *pCaBandCombo;
    policyman_ca_combo_item_t *pCaBandComboItem;

    pCaBandCombo = policyman_get_ca_band_combos(plmn);

    if (NULL != pCaBandCombo)
    {
      POLICYMAN_MSG_S2( SUBS_PREFIX "policyman_ca_band_combos_update with %s",
                        subsId,
                        pCaBandCombo );
    }
    else
    {
      POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "policyman_ca_band_combos_update with empty string", subsId);
    }

    pCaBandComboItem = policyman_ca_combo_item_new(pCaBandCombo, subsId);

    mre_set_append(pItemSet, &pCaBandComboItem);

    ref_cnt_obj_release(pCaBandComboItem);
    POLICYMAN_MEM_FREE_IF(pCaBandCombo);
  }
}


/*-------- policyman_ca_band_combos_evaluate --------*/
void policyman_ca_band_combos_evaluate(
  policy_execute_ctx_t  *pCtx
)
{
  policyman_state_t       *pState     = POLICY_EXECUTE_STATE(pCtx);
  mre_set_t               *pItemSet   = POLICY_EXECUTE_ITEMSET(pCtx);
  sys_modem_as_id_e_type   subsId     = POLICY_EXECUTE_SUBS(pCtx);
  size_t                   numSim = 0;
  policyman_status_t       status;

  status = policyman_get_current_num_sim(&numSim);
  for (subsId = SYS_MODEM_AS_ID_1; subsId < numSim && POLICYMAN_SUCCEEDED(status); subsId++)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "policyman_ca_band_combos_evaluate", subsId);
    policyman_ca_band_combos_evaluate_per_subs(pState, pItemSet, subsId);
  }
}

/*-------- policyman_ca_combo_mdump --------*/
QSH_MDUMP_FN_ATTR_PM void policyman_ca_combo_mdump(
  policyman_item_t const *pItem
)
{
  policyman_qsh_dump_collect((void *)pItem, sizeof(policyman_ca_combo_item_t));
}

/*-------- policyman_ca_band_copy_from_database --------*/
STATIC void policyman_ca_band_copy_from_database(
  sys_modem_as_id_e_type  subsId,
  sys_plmn_id_s_type     *pPlmn,
  char                   *pBandCombos,
  size_t                  bandCombosLen,
  size_t                 *pCaComboLen
)
{
  policyman_ca_combo_item_t *pCaCombo;
  
  pCaCombo = (policyman_ca_combo_item_t *) policyman_cfgitem_get_item(POLICYMAN_ITEM_CA_BAND_COMBOS,
                                                                      subsId);
  if (IS_NULL(pCaCombo->pCombos))
  {
    *pCaComboLen = 0;
    pBandCombos = NULL;
  }
  else
  {
    strlcpy(pBandCombos, pCaCombo->pCombos, bandCombosLen);
    *pCaComboLen = strlen(pCaCombo->pCombos)+1;
  }
  
  POLICYMAN_MSG_PLMN_1( SUBS_PREFIX "policyman_ca_band_combos_get_string_for_plmn: returning database string for current service PLMN: ",
                        subsId,
                        *pPlmn );
  POLICYMAN_MSG_S1( "CA band combo '%s'",
                    pBandCombos == NULL ? "NULL" : pBandCombos );
  
  MRE_RELEASE_IF(pCaCombo);
}


/*-------- check_call_validity --------*/
STATIC policyman_status_t check_call_validity(
  sys_modem_as_id_e_type  subsId,
  sys_plmn_id_s_type     *pPlmn
)
{
  policyman_status_t status = POLICYMAN_STATUS_SUCCESS;
  
  if (   (SYS_MODEM_AS_ID_1  > subsId)
      || (POLICYMAN_NUM_SUBS < subsId)
      || IS_NULL(pPlmn))
  {
    POLICYMAN_MSG_ERROR_0("policyman_ca_band_combos_get_string_for_plmn: Invalid subsId or plmn");
    status = POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }
  else if (policyman_is_initializing())
  {
    POLICYMAN_MSG_HIGH_0("policyman_ca_band_combos_get_string_for_plmn: initializing, no string available");
    status = POLICYMAN_STATUS_ERR_NOT_READY;
  }

  return status;
}


/*-------- policyman_ca_band_sandbox_plmn --------*/
STATIC void policyman_ca_band_sandbox_plmn(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  sys_plmn_id_s_type     *pPlmn
)
{
  policyman_sandbox_t *pSandbox;

  pSandbox = policyman_create_sandbox();

  policyman_ss_set_serving_plmn_rat(pSandbox, subsId, pPlmn, SYS_SYS_MODE_LTE);

  POLICYMAN_MSG_HIGH_1( "executing get_string_for_plmn with subs %d", subsId );

  policyman_policies_run_policy_check(pSandbox, pItemSet);

  ref_cnt_obj_release(pSandbox);
}


STATIC void get_ca_band_combos(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  char                   *pBandCombos,
  size_t                  bandCombosLen,
  size_t                 *pCaComboLen
)
{
  policyman_ca_combo_item_t **ppCaCombo;

  ppCaCombo = (policyman_ca_combo_item_t **) policyman_cfgitem_find_item_in_itemset(
                                                pItemSet,
                                                POLICYMAN_ITEM_CA_BAND_COMBOS,
                                                subsId );

  if (IS_NULL(ppCaCombo) || IS_NULL(*ppCaCombo))
  {
    *pCaComboLen = 0;
    pBandCombos = NULL;
  }
  else
  {
    if (IS_NULL((*ppCaCombo)->pCombos))
    {
      *pCaComboLen = 0;
      pBandCombos = NULL;
    }
    else
    {
      *pCaComboLen = strlcpy(pBandCombos, (*ppCaCombo)->pCombos, bandCombosLen);
    }

    POLICYMAN_MSG_S1( "CA band combo '%s'",
                      pBandCombos == NULL ? "NULL" : pBandCombos );

    ref_cnt_obj_release(*ppCaCombo);
  }
}


/*-------- policyman_lte_feature_item_t --------*/
struct policyman_lte_feature_item_t
{
  POLICYMAN_ITEM;

  uint64 features;
};


/*-------- get_current_features --------*/
STATIC uint64 get_current_features(
  mre_set_t               *pItemSet,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_lte_feature_item_t *pCurrentItem;
  uint64                        features;
    
  pCurrentItem = (policyman_lte_feature_item_t *) policyman_cfgitem_find_item_in_itemset_or_database(
                                                      pItemSet,
                                                      POLICYMAN_ITEM_LTE_FEATURE_MASK,
                                                      subsId );

  features = NON_NULL(pCurrentItem)? pCurrentItem->features
                                   : LTE_PM_CAP_ALL_FEATURES_ENABLED;

  REF_CNT_OBJ_RELEASE_IF(pCurrentItem);
  return features;
}


#define LOW_32(l)   ((uint32) ((l) & 0xffffffff))
#define HIGH_32(l)  ((uint32) ((l) >> 32))


/*-------- policyman_get_lte_capabilities_for_plmn --------*/
policyman_status_t policyman_get_lte_capabilities_for_plmn(
  sys_modem_as_id_e_type  subsId,
  sys_plmn_id_s_type     *pPlmn,
  char                   *pBandCombos,
  size_t                  bandCombosLen,
  size_t                 *pCaComboLen,
  uint64                 *pFeatureMask
)
{
  
  policyman_status_t  status;
  mre_set_t          *pItemSet = NULL;

  {
    boolean includesPcsDigit;
    uint32  mcc;
    uint32  mnc;

    policyman_plmn_get_mcc_mnc(pPlmn, &includesPcsDigit, &mcc, &mnc);
    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX
                          "policyman_get_lte_capabilities_for_plmn "
                          "called with MCC %d MNC %d ",
                          subsId,
                          mcc,
                          mnc );
  }
                      

  status = check_call_validity(subsId, pPlmn);
  if (POLICYMAN_SUCCEEDED(status))
  {
    pItemSet = policyman_itemset_new();

    policyman_ca_band_sandbox_plmn(pItemSet, subsId, pPlmn);

    get_ca_band_combos(pItemSet, subsId, pBandCombos, bandCombosLen, pCaComboLen);
    if (NON_NULL(pFeatureMask))
    {
      *pFeatureMask = get_current_features(pItemSet,  subsId);
    }

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "policyman_get_lte_capabilities_for_plmn returns LTE features: 0x%08x%08x",
                          subsId,
                          HIGH_32(*pFeatureMask),
                          LOW_32(*pFeatureMask) );

    ref_cnt_obj_release(pItemSet);
  }
  
  return status;
}



/******************************************************************************
  LTE feature support
******************************************************************************/

/*-------- lte_feature_action_t --------*/
typedef struct
{
  POLICYMAN_ACTION_BASE;

  uint64      feature;
  mre_set_t  *pConditions;
} lte_feature_action_t;


/*-------- policyman_get_lte_features --------*/
policyman_status_t policyman_get_lte_features(
  policyman_item_t const  *pItem,
  uint64                  *pFeatureMask
)
{
  policyman_status_t  status = POLICYMAN_STATUS_ERR_INVALID_ARGS;
  
  if (NON_NULL(pItem) && NON_NULL(pFeatureMask))
  {
    if (policyman_cfgitem_id(pItem) == POLICYMAN_ITEM_LTE_FEATURE_MASK)
    {
      policyman_lte_feature_item_t  *pFeatureItem = (policyman_lte_feature_item_t *) pItem;

      *pFeatureMask = pFeatureItem->features;
      status = POLICYMAN_STATUS_SUCCESS;
    }
    else
    {
      status = POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
    }

  }

  return status;
}


/*-------- policyman_lte_feature_get_default --------*/
policyman_item_t *policyman_lte_feature_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
)
{
  return (policyman_item_t *) policyman_lte_feature_item_new(LTE_PM_CAP_ALL_FEATURES_ENABLED, subsId);
}


/*-------- policyman_lte_feature_clone --------*/
policyman_item_t *policyman_lte_feature_clone(
  policyman_item_t const *pItem
)
{
  ref_cnt_obj_add_ref((policyman_item_t *) pItem);

  return (policyman_item_t *) pItem;
}


/*-------- policyman_lte_feature_item_new --------*/
policyman_lte_feature_item_t *policyman_lte_feature_item_new(
  uint64                  features,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_lte_feature_item_t  *pItem;

  pItem = (policyman_lte_feature_item_t *) policyman_cfgitem_new_item(
                                                POLICYMAN_ITEM_LTE_FEATURE_MASK,
                                                sizeof(policyman_lte_feature_item_t),
                                                mre_simple_dtor,
                                                subsId);

  pItem->features = features;

  return pItem;
}


/*-------- policyman_lte_feature_compare --------*/
boolean policyman_lte_feature_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
)
{
  policyman_lte_feature_item_t *pFeatures1 = (policyman_lte_feature_item_t *) pData1;
  policyman_lte_feature_item_t *pFeatures2 = (policyman_lte_feature_item_t *) pData2;

  return pFeatures1->features == pFeatures2->features;
}


/*-------- policyman_lte_feature_display --------*/
void policyman_lte_feature_display(
  policyman_item_t  *pItem
)
{
  policyman_lte_feature_item_t *pFeatures = (policyman_lte_feature_item_t *) pItem;

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "LTE features: 0x%08x%08x",
                        pFeatures->asubs_id,
                        HIGH_32(pFeatures->features),
                        LOW_32(pFeatures->features) );
}


/*-------- get_feature_mask --------*/
STATIC uint64 get_feature_mask(
  mre_xml_element_t const  *pElem
)
{
  uint64      feature = 0;
  char const *pName;
    
  pName = policyman_xml_get_attribute(pElem, "name");
  if (NON_NULL(pName))
  {
    static MRE_STR_VALUE64_TABLE(map) =
    {
      MRE_STR_VALUE64_ENTRY("NONDDS_APS", LTE_PM_CAP_FEATURE_MASK_APS),
    };

    MRE_LOOKUP_STR_VALUE64_NO_STATUS(map, pName, uint64, &feature);
  }

  return feature;
}


/*-------- lte_feature_action_dtor --------*/
STATIC void lte_feature_action_dtor(
  void  *pObj
)
{
  lte_feature_action_t  *pAction = (lte_feature_action_t *) pObj;

  REF_CNT_OBJ_RELEASE_IF(pAction->pConditions);
  policyman_action_dtor(pObj);
}


/*-------- execute_lte_feature --------*/
STATIC boolean execute_lte_feature(
  policyman_action_t const     *pAction,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  lte_feature_action_t          *pFeatureAct  = (lte_feature_action_t *) pAction;
  mre_set_t                     *pItemSet;
  policyman_lte_feature_item_t  *pFeatureItem;
  uint64                         features;

  pItemSet = POLICY_EXECUTE_ITEMSET(pCtx);
  features = get_current_features(pItemSet, subsId);

  if (mre_condset_evaluate(pFeatureAct->pConditions, pCtx))
  {
    features |= pFeatureAct->feature;
  }
  else
  {
    features &= ~pFeatureAct->feature;
  }
  
  pFeatureItem = policyman_lte_feature_item_new(features, subsId);
  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "<lte_feature> setting LTE features to 0x%08x%08x",
                        subsId,
                        HIGH_32(pFeatureItem->features),
                        LOW_32(pFeatureItem->features) );

  mre_set_replace(pItemSet, &pFeatureItem);
  ref_cnt_obj_release(pFeatureItem);

  return TRUE;
}


/*-------- policyman_lte_feature_execute --------*/
STATIC boolean policyman_lte_feature_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  policyman_action_t const  *pAct = (policyman_action_t const *) pAction;

  return policyman_execute_action(pAct, pCtx, execute_lte_feature);
}


/*-------- policyman_lte_feature_action_new --------*/
mre_status_t policyman_lte_feature_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t  status      = MRE_STATUS_ERR_MALFORMED_XML;
  mre_set_t    *pConditions = NULL;
  policyman_subs_t  pm_subs;
  uint64        feature;

  feature     = get_feature_mask(pElem);
  pConditions = mre_collect_conditions(pElem, pPolicy);
  status      = policyman_util_get_subs(pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (0 != feature && NON_NULL(pConditions) && POLICYMAN_SUCCEEDED(status))
  {
    lte_feature_action_t *pAction;

    pAction = POLICYMAN_ACTION_NEW( lte_feature_action_t,
                                    pPolicy,
                                    policyman_lte_feature_execute,
                                    lte_feature_action_dtor,
                                    pm_subs,
                                    pElem );
    pAction->pConditions  = pConditions;
    pAction->feature      = feature;

    *ppAction = (mre_action_t *) pAction;

    pConditions = NULL;
    status = MRE_STATUS_SUCCESS;
  }

  REF_CNT_OBJ_RELEASE_IF(pConditions);
  return status;
}


/*-------- policyman_get_lte_features_from_set_or_db --------*/
boolean policyman_get_lte_features_from_set_or_db(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  uint64                 *pFeatures
)
{
  policyman_item_t *pFeatureItem = NULL;
  boolean           featuresPresentInItemset = FALSE;

  featuresPresentInItemset = policyman_cfgitem_is_present_in_itemset(
                                            pItemSet,
                                            POLICYMAN_ITEM_LTE_FEATURE_MASK,
                                            subsId,
                                            &pFeatureItem );
  if (!featuresPresentInItemset)
  {
    policyman_item_id_t id = POLICYMAN_ITEM_LTE_FEATURE_MASK;
    
    policyman_cfgitem_get_items( &id,
                                 1,
                                (policyman_item_t const **) &pFeatureItem );
  }
  if (NON_NULL(pFeatureItem))
  {
    policyman_get_lte_features(pFeatureItem, pFeatures);
    ref_cnt_obj_release(pFeatureItem);
  }

  return featuresPresentInItemset;
}


/*-------- policyman_is_lte_feature_enabled --------*/
boolean policyman_is_lte_feature_enabled(
  uint64  features,
  uint64  whichFeature
)
{
  return (features & whichFeature) != 0;
}


/*-------- policyman_lte_feature_enable --------*/
uint64 policyman_lte_feature_enable(
  uint64  features,
  uint64  whichFeature
)
{
  return features | whichFeature;
}


/*-------- policyman_lte_feature_disable --------*/
uint64 policyman_lte_feature_disable(
  uint64  features,
  uint64  whichFeature
)
{
  return features & ~whichFeature;
}
/*-------- policyman_lte_feature_mdump --------*/
QSH_MDUMP_FN_ATTR_PM void policyman_lte_feature_mdump(
  policyman_item_t const *pItem
)
{
  policyman_qsh_dump_collect((void *)pItem, sizeof(policyman_lte_feature_item_t));
}
