/**
  @file mre_namedobj.c

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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/mre_namedobj.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_dbg.h"
#include "mre_engine.h"
#include "mre_lang.h"
#include "mre_namedobj.h"
#include "mre_set.h"
#include "mre_util.h"
#include "mre_util_i.h"
#include "mre_xml.h"

#include "sys.h"



/*=============================================================================
  Named object APIs
=============================================================================*/


/*-------- mre_named_object_get_namespace --------*/
mre_named_obj_namespace_t mre_named_object_get_namespace(
  mre_xml_element_t const *pElem
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("policy",    MRE_NAMESPACE_POLICY),
    MRE_STR_VALUE_ENTRY("subs",      MRE_NAMESPACE_SUBS),
    MRE_STR_VALUE_ENTRY("global",    MRE_NAMESPACE_GLOBAL),
  };

  char const                *pNamespace;
  mre_named_obj_namespace_t  ns = MRE_NAMESPACE_NONE;

  pNamespace = mre_xml_get_attribute(pElem, "ns");
  if (NULL != pNamespace)
  {
    MRE_LOOKUP_STR_VALUE_NO_STATUS(map, pNamespace, mre_named_obj_namespace_t, &ns);
  }

  return ns;
}


/*-------- mre_named_object_matches --------*/
boolean mre_named_object_matches(
  mre_named_object_t      *pObj,
  char const              *pName,
  mre_named_object_type_t  objType
)
{
  return    pObj != NULL
         && pObj->objType == objType
         && pObj->pName != NULL
         && strcasecmp(pObj->pName, pName) == 0;
}


/*-------- mre_named_object_dtor --------*/
void mre_named_object_dtor(
  void  *pObj
)
{
  mre_named_object_t  *pNamedObj = (mre_named_object_t *) pObj;

  MRE_MEM_FREE_IF(pNamedObj->pName);
  mre_mem_free(pNamedObj);
}



/*-------- mre_named_object_init --------*/
void mre_named_object_init(
  mre_named_object_t      *pObj,
  char const              *pName,
  mre_named_object_type_t  objType
)
{
  if (pObj != NULL && pName != NULL)
  {
    pObj->pName = mre_str_dup(pName);
    pObj->objType = objType;
  }
}


/*-------- mre_named_object_new --------*/
mre_named_object_t * mre_named_object_new(
  size_t                   objectSize,
  destructor_ptr           pDtor,
  mre_named_object_type_t  objType,
  char const              *pName
)
{
  mre_named_object_t  *pNamedObj;
  destructor_ptr       pDestruct  = mre_named_object_dtor;

  pNamedObj = (mre_named_object_t *)mre_mem_alloc(objectSize);

  if (NON_NULL(pDtor))
  {
    pDestruct = pDtor;
  }

  ref_cnt_obj_init(pNamedObj, pDestruct);

  mre_named_object_init( pNamedObj,
                         pName,
                         objType );

  pNamedObj->pPolicy = NULL;  // notused?

  return pNamedObj;
}


/*-------- mre_ensure_obj_set --------*/
STATIC void mre_ensure_obj_set(
  mre_set_t **ppSet
)
{
  if (NULL == *ppSet)
  {
    *ppSet = mre_set_refcnt_new(5, MRE_SET_NO_MAX_SIZE, NULL);
  }
}


/*-------- mre_named_object_add --------*/
boolean mre_named_object_add(
  mre_named_object_t        *pObj,
  mre_named_obj_namespace_t  ns,
  mre_policy_t              *pPolicy
)
{
  boolean                success = FALSE;
  mre_set_t            **ppNamedObjs = NULL;
  mre_namespace_info_t  *pNamespaceInfo = MRE_POLICY_NAMESPACE_INFO(pPolicy);

  pObj->pPolicy = pPolicy;

  /*  If no namespace is specified, use the default for the policy.
   */
  if ((MRE_NAMESPACE_NONE == ns) && (NULL != pNamespaceInfo))
  {
    ns = pNamespaceInfo->defaultNS;
  }

  /*  Get a pointer to storage for the pointer to the named object set.
   */
  switch (ns)
  {
    case MRE_NAMESPACE_POLICY:
    default:
      ppNamedObjs = &MRE_POLICY_PNAMEDOBJS(pPolicy);
      break;

    case MRE_NAMESPACE_SUBS:
      if (NULL != pNamespaceInfo)
      {
        sys_modem_as_id_e_type  subs = SYS_MODEM_AS_ID_1;
        if (NULL != pNamespaceInfo->pfnGetPolicySubs)
        {
          subs = pNamespaceInfo->pfnGetPolicySubs(pPolicy);
        }
        ppNamedObjs = &pNamespaceInfo->perSubsNamedObjs[(size_t) subs];
      }
      break;

    case MRE_NAMESPACE_GLOBAL:
      if (NULL != pNamespaceInfo)
      {
        ppNamedObjs = &pNamespaceInfo->pGlobalNamedObjs;
      }
      break;
  }

  /*  Create the set if it's not present, then add the object to that set.
   */
  if (NULL != ppNamedObjs)
  {
    mre_ensure_obj_set(ppNamedObjs);
    success = mre_set_add(*ppNamedObjs, &pObj);
  }

  return success;
}


/*-------- mre_named_object_add_in_set --------*/
boolean mre_named_object_add_in_set(
  mre_named_object_t        *pObj,
  mre_set_t                **ppSet
)
{
  boolean success = FALSE;

  if (NULL != ppSet)
  {
    mre_ensure_obj_set(ppSet);
    success = mre_set_add(*ppSet, &pObj);
  }

  return success;
}


/*-------- mre_named_object_find_in_set --------*/
mre_named_object_t *mre_named_object_find_in_set(
  char const              *pName,
  mre_named_object_type_t  objType,
  mre_set_t               *pSet
)
{
  mre_named_object_t  *pObj = NULL;

  if (NULL != pSet && NULL != pName)
  {
    pObj = mre_set_find_named_object(pSet, pName, objType);
  }

  return  pObj;
}


/*-------- mre_named_object_find --------*/
mre_named_object_t *mre_named_object_find(
  char const              *pName,
  mre_named_object_type_t  objType,
  mre_policy_t            *pPolicy
)
{
  mre_named_object_t     *pObj           = NULL;
  mre_namespace_info_t   *pNamespaceInfo = MRE_POLICY_NAMESPACE_INFO(pPolicy);
  sys_modem_as_id_e_type  subs           = SYS_MODEM_AS_ID_1;

  if (NULL == pName)
  {
    goto Done;
  }

  /*  Look in policy first...
   */
  pObj = mre_named_object_find_in_set(pName, objType, MRE_POLICY_PNAMEDOBJS(pPolicy));
  if (NULL != pObj)
  {
    goto Done;
  }

  if (NULL != pNamespaceInfo)
  {
    if (NULL != pNamespaceInfo->pfnGetPolicySubs)
    {
      subs = pNamespaceInfo->pfnGetPolicySubs(pPolicy);
    }

    pObj = mre_named_object_find_with_subs(pName, objType, subs, pNamespaceInfo);
  }

Done:
  return pObj;
}


/*-------- mre_named_object_find_with_subs --------*/
mre_named_object_t * mre_named_object_find_with_subs(
  char const              *pName,
  mre_named_object_type_t  objType,
  sys_modem_as_id_e_type   subs,
  mre_namespace_info_t    *pNamespaceInfo
)
{
  mre_named_object_t     *pObj = NULL;

  if ((NULL == pName) || (NULL == pNamespaceInfo))
  {
    goto Done;
  }

  /*  Look in the subscription namespace...
   */
  if (subs != SYS_MODEM_AS_ID_NONE)
  {
    pObj = mre_named_object_find_in_set( pName,
                                         objType,
                                         pNamespaceInfo->perSubsNamedObjs[(size_t) subs] );
  }

  if (NULL != pObj)
  {
    goto Done;
  }

  /*  ...then in the globals
   */
  pObj = mre_named_object_find_in_set( pName,
                                       objType,
                                       pNamespaceInfo->pGlobalNamedObjs );

Done:
  return pObj;
}


/*-------- object_types_match --------*/
STATIC boolean object_types_match(
  void const *pElem,
  void const *pCtx
)
{
  mre_named_object_t      *pObj    = *(mre_named_object_t **)pElem;
  mre_named_object_type_t  objType = *((mre_named_object_type_t *)pCtx);

  return pObj->objType == objType;
}


/*-------- mre_named_object_iterate_over_type --------*/
void mre_named_object_iterate_over_type(
  mre_named_object_type_t  objType,
  mre_policy_t            *pPolicy,
  elem_iter_fn_t           pfnType,
  void                    *pCtx
)
{
  mre_set_t              *pFilteredSet;
  mre_set_t              *pSetToFilter;
  mre_namespace_info_t   *pNamespaceInfo = MRE_POLICY_NAMESPACE_INFO(pPolicy);
  sys_modem_as_id_e_type  subsId         = SYS_MODEM_AS_ID_1;

  // Find policy level named objects
  if (NULL != MRE_POLICY_PNAMEDOBJS(pPolicy))
  {
    pSetToFilter = MRE_POLICY_PNAMEDOBJS(pPolicy);

    pFilteredSet = mre_set_filter(pSetToFilter, object_types_match, &objType);
    mre_set_iterate(pFilteredSet, pfnType, pCtx);

    ref_cnt_obj_release(pFilteredSet);
  }

  // Find subs level named objects
  if (NULL != pNamespaceInfo)
  {
    if (NULL != pNamespaceInfo->pfnGetPolicySubs)
    {
      subsId = pNamespaceInfo->pfnGetPolicySubs(pPolicy);
    }

    pSetToFilter = pNamespaceInfo->perSubsNamedObjs[(size_t) subsId];
    pFilteredSet = mre_set_filter(pSetToFilter, object_types_match, &objType);
    mre_set_iterate(pFilteredSet, pfnType, pCtx);

    ref_cnt_obj_release(pFilteredSet);
  }
}

/*=============================================================================
  Named set APIs
=============================================================================*/


/*-------- mre_named_set_dtor --------*/
void  mre_named_set_dtor(
  void  *pObj
)
{
  mre_named_set_t *pNamedSet = (mre_named_set_t *) pObj;

  MRE_MEM_FREE_IF(pNamedSet->pName);
  REF_CNT_OBJ_RELEASE_IF(pNamedSet->pSet);
  mre_mem_free(pNamedSet);
}


/*-------- mre_named_set_new_with_type --------*/
mre_named_set_t *mre_named_set_new_with_type(
  size_t                  namedObjectCTypeSize,
  char const             *pName,
  mre_named_object_type_t type,
  size_t                  elemSize,
  size_t                  nInitElems,
  size_t                  maxElems,
  elem_compare_fn_t       pfnCompare,
  destructor_ptr          pfnDtor
)
{
  mre_named_set_t *pNamedSet;
  mre_set_t       *pSet;

  pNamedSet = (mre_named_set_t *)mre_named_object_new( namedObjectCTypeSize,
                                                       mre_named_set_dtor,
                                                       type,
                                                       pName );

  pNamedSet->pSet = mre_set_new(elemSize, nInitElems, maxElems, pfnCompare, pfnDtor);

  return pNamedSet;
}


/*-------- mre_named_set_new --------*/
mre_named_set_t *mre_named_set_new(
  char const             *pName,
  mre_named_object_type_t type,
  size_t                  elemSize,
  size_t                  nInitElems,
  size_t                  maxElems,
  elem_compare_fn_t       pfnCompare,
  destructor_ptr          pfnDtor
)
{
  mre_named_set_t *pNamedSet;

  pNamedSet = MRE_NAMED_SET_NEW( mre_named_set_t,
                                 pName,
                                 type,
                                 elemSize,
                                 nInitElems,
                                 maxElems,
                                 pfnCompare,
                                 pfnDtor );

  return pNamedSet;
}


/*-------- mre_named_set_add --------*/
boolean mre_named_set_add(
  mre_named_set_t *pNamedSet,
  void            *pElem
)
{
  return mre_set_add(pNamedSet->pSet, pElem);
}


/*-------- mre_named_set_contains --------*/
boolean mre_named_set_contains(
  mre_named_set_t const *pNamedSet,
  void const            *pElem
)
{
  return mre_set_contains(pNamedSet->pSet, pElem);
}


/*-------- mre_named_set_num_elems --------*/
size_t mre_named_set_num_elems(
  mre_named_set_t const *pNamedSet
)
{
  return mre_set_num_elems(pNamedSet->pSet);
}


/*-------- mre_named_object_clone --------*/
boolean mre_named_object_clone(
  void  *pElem,
  void  *pCtx
)
{
  mre_named_object_t   *pNamedObj    = *((mre_named_object_t **)pElem);
  mre_set_t           **ppCloneSet   = (mre_set_t **)pCtx;

  if (pNamedObj->objType == MRE_NAMED_BOOLEAN)
  {
    pNamedObj = mre_named_boolean_clone(pNamedObj);
  }
  else
  {
    ref_cnt_obj_add_ref(pNamedObj);
  }

  mre_named_object_add_in_set( pNamedObj,
                               ppCloneSet);

  MRE_RELEASE_IF(pNamedObj);

  return TRUE;
}


/*-------- mre_named_objects_clone --------*/
mre_set_t * mre_named_objects_clone(
  mre_set_t *pNamedObjSet
)
{
  mre_set_t *pNamedObjSetClone = NULL;

  if (NON_NULL(pNamedObjSet))
  {
    pNamedObjSetClone = mre_set_refcnt_new( mre_set_num_elems(pNamedObjSet),
                                            MRE_SET_NO_MAX_SIZE,
                                            NULL );

    mre_set_iterate(pNamedObjSet, mre_named_object_clone, (void **)&pNamedObjSetClone);
  }

  return pNamedObjSetClone;
}


/*-------- mre_plmn_mcc_set_is_valid --------*/
boolean mre_plmn_mcc_set_is_valid(
  mre_plmn_mcc_set_t    *pPlmnMccSet
)
{
  return    NON_NULL(pPlmnMccSet)
         && (  (mre_set_num_elems(pPlmnMccSet->pSet) != 0)
            || (pPlmnMccSet->includeHPLMN)
            || (pPlmnMccSet->includeEHPLMN) );
}


/*-------- mre_plmn_list_contains --------*/
boolean mre_plmn_list_contains(
  mre_policy_t             *pPolicy,
  mre_plmn_mcc_set_t const *pPlmnSet,
  sys_plmn_id_s_type const *pElem
)
{
  boolean elemIsFound;

  elemIsFound = mre_named_set_contains((mre_named_set_t const *)pPlmnSet, pElem);

  if (!elemIsFound && NON_NULL(pPolicy))
  {
    if (pPlmnSet->includeHPLMN)
    {
      elemIsFound = mre_plmns_are_equal((sys_plmn_id_s_type const *)&pPolicy->hplmn, pElem);
      MRE_MSG_HIGH_1("PLMN found in HPLMN: %d", elemIsFound);
    }

    if (!elemIsFound)
    {
      elemIsFound =    pPlmnSet->includeEHPLMN
                    && NON_NULL(pPolicy->pEhPlmnSet)
                    && mre_set_contains(pPolicy->pEhPlmnSet, pElem);

      MRE_MSG_HIGH_1("PLMN found in EHPLMN: %d", elemIsFound);
    }
  }

  return elemIsFound;
}


/*-------- element_is_this_mcc --------*/
STATIC boolean element_is_this_mcc(
  void *pPlmn,
  void *pCtx
)
{
  sys_mcc_type listMcc;
  sys_mcc_type *pMccToCheck = (sys_mcc_type *)pCtx;

  sys_plmn_get_mcc(*(sys_plmn_id_s_type *)pPlmn, &listMcc);

  return listMcc == *pMccToCheck;
}


/*-------- mre_mcc_list_contains --------*/
boolean mre_mcc_list_contains(
  mre_policy_t             *pPolicy,
  mre_plmn_mcc_set_t const *pMccSet,
  sys_mcc_type       const *pMccToCheck
)
{
  boolean elemIsFound;

  elemIsFound = mre_named_set_contains((mre_named_set_t const *)pMccSet, pMccToCheck);

  if (!elemIsFound && NON_NULL(pPolicy))
  {
    // check HPLMN first
    if (!elemIsFound && pMccSet->includeHPLMN)
    {
      sys_mcc_type   mcc;

      sys_plmn_get_mcc(pPolicy->hplmn, &mcc);
      elemIsFound = (mcc == *pMccToCheck);

      MRE_MSG_HIGH_1("MCC found in HPLMN: %d", elemIsFound);
    }

    // check EHPLMN list next
    if (  !elemIsFound
       && pMccSet->includeEHPLMN
       && NON_NULL(pPolicy->pEhPlmnSet) )
    {
      elemIsFound = mre_util_plmn_set_has_mcc(pPolicy->pEhPlmnSet, *pMccToCheck);
      MRE_MSG_HIGH_1("MCC found in EHPLMN: %d", elemIsFound);
    }
  }

  return elemIsFound;
}
