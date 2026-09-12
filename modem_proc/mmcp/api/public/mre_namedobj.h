#ifndef _MRE_NAMEDOBJ_H_
#define _MRE_NAMEDOBJ_H_

/**
  @file mre_namedobj.h

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

  $Header: //components/rel/mmcp.mpss/7.9.0/api/public/mre_namedobj.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/


#ifdef __cplusplus
extern "C" {
#endif

#include "mre.h"
#include "sys.h"


typedef struct mre_named_object_t mre_named_object_t;
typedef struct mre_named_set_t    mre_named_set_t;
typedef struct mre_plmn_mcc_set_t mre_plmn_mcc_set_t;


typedef enum
{
  MRE_UNNAMED_OBJ,

  MRE_NAMED_BOOLEAN,
  MRE_NAMED_PLMN_SET,
  MRE_NAMED_MCC_SET,

  MRE_NAMED_RF_BANDS,
  MRE_NAMED_TIMER,
  MRE_NAMED_CONFIG,
  MRE_NAMED_CA_BAND,
  MRE_NAMED_FEATURE_SET,

  MRE_NAMED_TRISTATE,

} mre_named_object_type_t;


#define MRE_NAMED_OBJECT_BASE       \
  _REF_CNT_OBJ;                     \
  mre_named_object_type_t  objType; \
  char const              *pName;   \
  mre_policy_t            *pPolicy


struct mre_named_object_t
{
  MRE_NAMED_OBJECT_BASE;
};

#define MRE_MAX_SUBS  3

typedef enum
{
  MRE_NAMESPACE_NONE,
  MRE_NAMESPACE_POLICY,
  MRE_NAMESPACE_SUBS,
  MRE_NAMESPACE_GLOBAL
} mre_named_obj_namespace_t;


typedef sys_modem_as_id_e_type  (*pfnGetPolicySubs_t)(
  mre_policy_t *pPolicy
);


struct mre_namespace_info_t
{
  _REF_CNT_OBJ;

  mre_named_obj_namespace_t  defaultNS;
  pfnGetPolicySubs_t         pfnGetPolicySubs;
  mre_set_t                 *perSubsNamedObjs[MAX_AS_IDS];
  mre_set_t                 *pGlobalNamedObjs;
};


/*-------- mre_named_object_get_namespace --------*/
mre_named_obj_namespace_t mre_named_object_get_namespace(
  mre_xml_element_t const *pElem
);


/*-------- mre_named_object_matches --------*/
boolean mre_named_object_matches(
  mre_named_object_t      *pObj,
  char const              *pName,
  mre_named_object_type_t  objType
);


/*-------- mre_named_object_dtor --------*/
/**
@brief

@param

@return

*/
void mre_named_object_dtor(
  void  *pObj
);


/*-------- mre_named_object_init --------*/
/**
@brief  Initialize the name and type of a named object.

@param[in]  pObj    Pointer to the named object.
@param[in]  pName   The name of the object.
@param[in]  objType The type of the object

@return
  Nothing.
*/
void mre_named_object_init(
  mre_named_object_t      *pObj,
  char const              *pName,
  mre_named_object_type_t  objType
);

/*-------- mre_named_object_new --------*/
/**
@brief  Create a new base named object.  Creates and fills
in the elements of a base MRE named object.

@param[in]  objectSize    Pointer to the named object.
@param[in]  pDtor         Destructor function (MRE default dtor if NULL)
@param[in]  objType       The type of the object.
@param[in]  pName         The name of the object.

@return
  A new allocated named object which can be used in a derived
  named object.
*/
mre_named_object_t * mre_named_object_new(
  size_t                   objectSize,
  destructor_ptr           pDtor,
  mre_named_object_type_t  objType,
  char const              *pName
);

/*-------- MRE_NAMED_OBJECT_NEW --------*/
#define MRE_NAMED_OBJECT_NEW(object_c_type, p_dtor, p_objtype, p_name) \
  (object_c_type *)mre_named_object_new( sizeof(object_c_type), \
                                         (p_dtor), \
                                         (p_objtype), \
                                         (p_name) )

/*-------- mre_named_object_add --------*/
/**
@brief  Add a named object.

@param[in]  pObj    Named object.
@param[in]  ns      Namespace in which object is to be added.
@param[in]  pPolicy Pointer to the policy to which this object applies.

@return
  TRUE if added, FALSE if not (too many named objects).
*/
boolean mre_named_object_add(
  mre_named_object_t        *pObj,
  mre_named_obj_namespace_t  ns,
  mre_policy_t              *pPolicy
);


/*-------- mre_named_object_add_in_set --------*/
/**
@brief  Replacement for mre_named_object_add when executing in another
        task's context and you don't have a pointer to the governing
        policy.  Requires that you know the address of the set.

@param[in]  pObj    Named object.
@param[in]  ppSet   Pointer to set of named objects to add to

@return
  TRUE if added, FALSE if not (too many named objects).
*/
boolean mre_named_object_add_in_set(
  mre_named_object_t        *pObj,
  mre_set_t                **ppSet
);


/*-------- mre_set_find_named_object --------*/
/**
@brief  Find a named object within this set with a given name and type.

NOTE: The set on which this is called must be a set of pointers to named object,
and those named objects should have their pName member pointing to the set name.

@param[in]  pSet    Pointer to the policyman set.
@param[in]  pName   The name of the set for which to search.
@param[in]  objType The type of the object which is being looked for.

@return
  Pointer to the object, or NULL if it is not present.
*/
mre_named_object_t * mre_set_find_named_object(
  mre_set_t const         *pSet,
  char const              *pName,
  mre_named_object_type_t  objType
);


/*-------- mre_named_object_find_in_set --------*/
/**
@brief  Replacement for mre_named_object_find when executing in another
        task's context and you don't have a pointer to the governing
        policy.  Requires that you know the address of the set.

@param

@return

*/
mre_named_object_t * mre_named_object_find_in_set(
  char const              *pName,
  mre_named_object_type_t  objType,
  mre_set_t               *pSet
);


/*-------- mre_named_object_find --------*/
/**
@brief  Get a pointer to a named object

@param[in]  pName   Pointer to a NUL terminated name of the object.
@param[in]  objType Type of object.
@param[in]  pPolicy Policy querying for this object.

@return
  Pointer to the named object, if present, NULL otherwise.
  NOTE: A reference has been added to the object before returning it to the caller.
  The caller MUST release this reference when it is done.
*/
mre_named_object_t * mre_named_object_find(
  char const              *pName,
  mre_named_object_type_t  objType,
  mre_policy_t            *pPolicy
);


/*-------- mre_named_object_find_with_subs --------*/
mre_named_object_t * mre_named_object_find_with_subs(
  char const              *pName,
  mre_named_object_type_t  objType,
  sys_modem_as_id_e_type   subs,
  mre_namespace_info_t    *pNamespaceInfo
);


/*-------- mre_named_object_iterate_over_type --------*/
/**
@brief  Calls a function on all objects in all namespaces with the specified type

@param[in]  objType Type of object to find.
@param[in]  pPolicy Policy querying for this object.
@param[in]  pfnType Function to call for objects of the specified type.
@param[in]  pCtx Context to pass to type function.

@return
  None
*/

void mre_named_object_iterate_over_type(
  mre_named_object_type_t  objType,
  mre_policy_t            *pPolicy,
  elem_iter_fn_t           pfnType,
  void                    *pCtx
);

/*-------- mre_boolean_get_value_for_subs --------*/
mre_status_t mre_boolean_get_value_for_subs(
  char const             *pName,
  boolean                *pValue,
  sys_modem_as_id_e_type  subsId,
  mre_namespace_info_t   *pNamespaceInfo
);


/*-------- mre_boolean_get_value --------*/
mre_status_t mre_boolean_get_value(
  char const          *pName,
  boolean             *pValue,
  mre_policy_t        *pPolicy
);


/*-------- mre_boolean_set_value --------*/
mre_status_t mre_boolean_set_value(
  mre_boolean_t          *pBoolean,
  boolean                 value
);


/*-------- mre_boolean_is_writable --------*/
boolean mre_boolean_is_writable(
  mre_boolean_t          *pBoolean
);


/*  Type of the function that reads the text inside the list element
 *  and appends elements from the text to the passed set.
 */
typedef mre_status_t (*pfn_read_named_list_t)(
   mre_xml_element_t const  *pElem,
   mre_named_set_t          *pSet
);


/*-------- mre_plmn_list_read --------*/
mre_status_t mre_plmn_list_read(
  mre_xml_element_t const *pElem,
  mre_named_set_t         *pSet
);

/*-------- mre_mcc_list_read --------*/
mre_status_t mre_mcc_list_read(
  mre_xml_element_t const *pElem,
  mre_named_set_t         *pSet
);

typedef enum
{
  MRE_TRISTATE_FALSE,
  MRE_TRISTATE_TRUE,
  MRE_TRISTATE_UNDEF,
} mre_tristate_state_t;

/*-------- mre_tristate_get_value --------*/
mre_status_t mre_tristate_get_value(
  char const           *pName,
  mre_policy_t         *pPolicy,
  mre_tristate_state_t *pValue
);

/*-------- mre_tristate_set --------*/
mre_status_t mre_tristate_set(
  char const           *pName,
  mre_policy_t         *pPolicy,
  mre_tristate_state_t  value
);

/*=============================================================================
  Named sets
=============================================================================*/

struct mre_named_set_t
{
  MRE_NAMED_OBJECT_BASE;

  mre_set_t   *pSet;
};


struct mre_plmn_mcc_set_t
{
  MRE_NAMED_OBJECT_BASE;

  mre_set_t              *pSet;
  sys_modem_as_id_e_type  subsId;
  boolean                 includeEHPLMN;
  boolean                 includeHPLMN;
};

/*-------- mre_named_set_new --------*/
mre_named_set_t *mre_named_set_new(
  char const             *pName,
  mre_named_object_type_t type,
  size_t                  elemSize,
  size_t                  nInitElems,
  size_t                  maxElems,
  elem_compare_fn_t       pfnCompare,
  destructor_ptr          pfnDtor
);

/*-------- mre_named_set_new_with_type --------*/
mre_named_set_t *mre_named_set_new_with_type(
  size_t                  setSize,
  char const             *pName,
  mre_named_object_type_t type,
  size_t                  elemSize,
  size_t                  nInitElems,
  size_t                  maxElems,
  elem_compare_fn_t       pfnCompare,
  destructor_ptr          pfnDtor
);


/*-------- MRE_NAMED_SET_NEW --------*/
#define MRE_NAMED_SET_NEW(named_set_c_type, pName, type, elemSize, nInitElems, maxElems, pfnCompare, pfnDtor) \
  (named_set_c_type *)mre_named_set_new_with_type( sizeof(named_set_c_type), \
                                                   (pName), \
                                                   (type), \
                                                   (elemSize), \
                                                   (nInitElems), \
                                                   (maxElems), \
                                                   (pfnCompare), \
                                                   (pfnDtor) )


/*-------- mre_named_set_add --------*/
boolean mre_named_set_add(
  mre_named_set_t *pNamedSet,
  void            *pElem
);


/*-------- mre_named_set_contains --------*/
boolean mre_named_set_contains(
  mre_named_set_t const *pNamedSet,
  void const            *pElem
);


/*-------- mre_named_set_num_elems --------*/
size_t mre_named_set_num_elems(
  mre_named_set_t const *pNamedSet
);

/*-------- mre_named_mcc_plmn_set_new --------*/
mre_status_t  mre_named_mcc_plmn_set_new(
  char const                   *pName,
  mre_xml_element_t const      *pElem,
  mre_named_object_type_t       type,
  pfn_read_named_list_t         pFnRead,
  mre_plmn_mcc_set_t          **ppSet
);


/*-------- mre_plmn_mcc_set_is_valid --------*/
boolean mre_plmn_mcc_set_is_valid(
  mre_plmn_mcc_set_t    *pPlmnMccSet
);


/*-------- mre_mcc_list_contains --------*/
boolean mre_mcc_list_contains(
  mre_policy_t             *pPolicy,
  mre_plmn_mcc_set_t const *pMccSet,
  sys_mcc_type       const *pMccToCheck
);


/*-------- mre_plmn_list_contains --------*/
boolean mre_plmn_list_contains(
  mre_policy_t             *pPolicy,
  mre_plmn_mcc_set_t const *pPlmnSet,
  sys_plmn_id_s_type const *pElem
);


#ifdef __cplusplus
}
#endif

#endif /* _MRE_NAMEDOBJ_H_ */
