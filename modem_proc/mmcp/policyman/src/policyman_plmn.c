/**
  @file policyman_plmn.c

  @brief
*/

/*
    Copyright (c) 2013-2017 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_plmn.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_engine.h"
#include "mre_xml.h"
#include "policyman_plmn.h"
#include "policyman_util.h"
#include "policyman_rules.h"
#include "policyman_xml.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_dbg.h"



/*=============================================================================
  PLMN APIs
=============================================================================*/


/*-------- policyman_plmns_are_equal --------*/
boolean policyman_plmns_are_equal(
  sys_plmn_id_s_type  *pPlmn1,
  sys_plmn_id_s_type  *pPlmn2
)
{
  return mre_plmns_are_equal( (sys_plmn_id_s_type const *)pPlmn1, 
                              (sys_plmn_id_s_type const *)pPlmn2 );
}


/*-------- policyman_plmn_clear --------*/
void policyman_plmn_clear(
  sys_plmn_id_s_type  *pPlmn
)
{
  memset(pPlmn, 0xFF, sizeof(sys_plmn_id_s_type));
}


/*-------- policyman_plmn_get_mcc --------*/
uint32 policyman_plmn_get_mcc(
  sys_plmn_id_s_type  *pPlmn
)
{
  return mre_plmn_get_mcc(pPlmn);
}


/*-------- policyman_plmn_get_mcc_mnc --------*/
void policyman_plmn_get_mcc_mnc(
  sys_plmn_id_s_type  *pPlmn,
  boolean             *mnc_includes_pcs_digit_ptr,
  uint32              *mcc_ptr,
  uint32              *mnc_ptr
)
{
  boolean               plmn_is_undefined;

  sys_plmn_get_mcc_mnc(*pPlmn, &plmn_is_undefined, mnc_includes_pcs_digit_ptr, mcc_ptr, mnc_ptr);
}

/*-------- policyman_plmn_to_ascii --------*/
void policyman_plmn_to_ascii(
  sys_plmn_id_s_type  *pPlmn,
  char                *pPlmnStr,
  size_t               plmnStrLen
)
{
  uint32   mcc;
  uint32   mnc;
  boolean  mnc_includes_pcs_digit;

  policyman_plmn_get_mcc_mnc(pPlmn, &mnc_includes_pcs_digit, &mcc, &mnc);

  if (mnc_includes_pcs_digit)
  {
    snprintf(pPlmnStr, plmnStrLen, "%03d-%03d", (int)mcc, (int)mnc);
  }
  else
  {
    snprintf(pPlmnStr, plmnStrLen, "%03d-%02d", (int)mcc, (int)mnc);
  }
}


/*-------- policyman_get_set_from_condition --------*/
mre_plmn_mcc_set_t *policyman_get_set_from_condition(
  policyman_set_condition_t *pCond,
  mre_policy_t              *pPolicy,
  mre_named_object_type_t    objType
)
{
  mre_plmn_mcc_set_t *pSet;
  
  pSet = pCond->pSet;
  if (NULL == pSet && NULL != pCond->pSetName)
  {
    pSet = (mre_plmn_mcc_set_t *) mre_named_object_find( pCond->pSetName,
                                                         objType,
                                                         pPolicy );
  }

  return pSet;
}

/*-------- policyman_set_condition_dtor --------*/
void policyman_set_condition_dtor(
  void  *pObj
)
{
  policyman_set_condition_t *pCondition = (policyman_set_condition_t *)pObj;

  MRE_RELEASE_IF(pCondition->pSet);
  POLICYMAN_MEM_FREE_IF(pCondition->pSetName);
  policyman_condition_dtor(pCondition);
}


/*-------- policyman_plmn_mcc_get_list --------*/
mre_status_t policyman_plmn_mcc_get_list(
  mre_xml_element_t const    *pElem,
  mre_policy_t               *pPolicy,
  mre_named_object_type_t     type,
  pfn_read_named_list_t       pfnRead,
  policyman_set_condition_t  *pCondition
)
{
  char const           *pStr        = NULL;
  char                 *pSetName    = NULL;
  mre_plmn_mcc_set_t   *pPlmnMccSet = NULL;
  mre_status_t          status      = MRE_STATUS_SUCCESS;

  //  See if there is a named MCC list to use for this condition.
  pStr = mre_xml_get_attribute(pElem, "list");

  if (NON_NULL(pStr))
  {
    pSetName = mre_str_dup(pStr);
  }
  else
  {
    //  No named set; try and get an inline list.
    pStr = mre_xml_get_text(pElem);

    if (NON_NULL(pStr))
    {
      status = mre_named_mcc_plmn_set_new( NULL, 
                                           pElem, 
                                           type,
                                           pfnRead,
                                           &pPlmnMccSet);
    }
  }

  if (MRE_SUCCEEDED(status))
  {
    pCondition->pSet     = pPlmnMccSet;
    pCondition->pSetName = pSetName;
  }

  return status;
}


/*-------- policyman_plmn_mcc_new --------*/
mre_status_t policyman_plmn_mcc_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_named_object_type_t         type,
  pfn_read_named_list_t           pFnRead,
  mre_condition_t               **ppCondition,
  pfn_evaluate_t                  evaluate
)
{
  mre_status_t               status     = MRE_STATUS_ERR_INVALID_CONDITION;
  policyman_set_condition_t *pCondition = NULL;
  char const                *pStr;
  policyman_subs_t           pm_subs    = SUBS_NONE;
  char                      *pSetName   = NULL;

  status = policyman_util_get_subs(pElem, POLICY_SUBS(pPolicy), &pm_subs);
  if (MRE_SUCCEEDED(status))
  {
    pCondition = POLICYMAN_CONDITION_NEW( policyman_set_condition_t,
                                          pPolicy,
                                          evaluate,
                                          policyman_set_condition_dtor,
                                          pm_subs,
                                          pElem );

    policyman_plmn_mcc_get_list( pElem,
                                 pPolicy,
                                 type,
                                 pFnRead,
                                 pCondition );
  }

  //  If there is neither an inline or named list, bail.
  if (NON_NULL(pCondition) && IS_NULL(pCondition->pSet) && IS_NULL(pCondition->pSetName))
  {
     MRE_RELEASE_IF(pCondition);
     status = MRE_STATUS_ERR_INVALID_CONDITION;
   }
   else
   {
     *ppCondition = (mre_condition_t *)pCondition;
     status = MRE_STATUS_SUCCESS;
   }

  return status;
}

/*-------- policyman_plmn_mcc_is_volte_only --------*/
boolean policyman_plmn_mcc_is_volte_only(
  sys_mcc_type            mcc,
  sys_modem_as_id_e_type  subsId
)
{
  mre_named_set_t *pMccSet;
  boolean          result  = FALSE;

  policyman_enter_namespace_crit_sect();
  pMccSet = (mre_named_set_t *) mre_named_object_find_with_subs( PM_VOLTE_ONLY_MCCS,
                                                                 MRE_NAMED_MCC_SET,
                                                                 subsId,
                                                                 policyman_get_namespace_info() );
  if (NON_NULL(pMccSet))
  {
    ref_cnt_obj_add_ref(pMccSet);
  }
  policyman_leave_namespace_crit_sect();

  if (pMccSet != NULL)
  {
    result = mre_named_set_contains(pMccSet, &mcc);
  }

  MRE_RELEASE_IF(pMccSet);
  POLICYMAN_MSG_HIGH_4( SUBS_PREFIX "mcc_is_volte_only check for mcc %d, set 0x%x, result %d",
                        subsId,
                        mcc,
                        pMccSet,
                        result );

  return result;
}
