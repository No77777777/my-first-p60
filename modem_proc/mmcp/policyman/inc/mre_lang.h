#ifndef _MRE_LANG_H_
#define _MRE_LANG_H_

/**
  @file mre_lang.h

  @brief  Code for the "language" used in the Modem Rules Engine XML.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/mre_lang.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_xml.h"


/*=============================================================================
  Conditions
=============================================================================*/

mre_status_t mre_condition_true_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);


mre_status_t mre_boolean_test_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);


mre_status_t mre_condition_not_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);


mre_status_t mre_condition_or_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);

mre_status_t mre_condition_and_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);


/*=============================================================================
  Actions
=============================================================================*/

/*-------- mre_execute_action --------*/
boolean mre_execute_action(
  mre_action_t  *pAction,
  void          *pCtx
);

/*-----------------------------------------------------------------------------
  Logging functions
-----------------------------------------------------------------------------*/
void clear_action_logging_info(
  mre_action_t  *pAction
);

void update_action_eval_count(
  mre_action_t  *pAction
);

void update_action_eval_time(
  mre_action_t  *pAction
);


/*-----------------------------------------------------------------------------
  This is defined in _lang.c but used in _rules.c.
-----------------------------------------------------------------------------*/
// TODO: Refactor _rules.c - most everything there should probably be in either _lang.c or _engine.c

boolean mre_if_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
);


/*-----------------------------------------------------------------------------
  Action creators
-----------------------------------------------------------------------------*/
mre_action_t * mre_action_new_with_vtbl(
  size_t                    actionSize,
  mre_policy_t             *pPolicy,
  pfn_execute_t             pExecute,
  destructor_ptr            pDtor,
  mre_xml_element_t const  *pElem,
  mre_logging_vtbl_t       *pVtbl
);

/*-------- MRE_ACTION_NEW_WITH_VTBL --------*/
#define MRE_ACTION_NEW_WITH_VTBL(action_type, p_policy, p_exec, p_dtor, p_elem, p_vtbl) \
  (action_type *)mre_action_new_with_vtbl( sizeof(action_type), \
                                           (p_policy), \
                                           (p_exec), \
                                           (p_dtor), \
                                           (p_elem), \
                                           (p_vtbl) )

mre_status_t mre_continue_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);


mre_status_t mre_boolean_define_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);


mre_status_t mre_boolean_set_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

mre_status_t mre_if_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

mre_status_t  mre_select_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

mre_status_t  mre_stop_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- mre_boolean_get_value_for_subs --------*/
mre_status_t mre_boolean_get_value_for_subs(
  char const             *pName,
  boolean                *pValue,
  sys_modem_as_id_e_type  subsId,
  mre_namespace_info_t   *pNamespaceInfo
);

/*-------- mre_boolean_get_value_from_set --------*/
mre_status_t mre_boolean_get_value_from_set(
  char const *pName,
  boolean    *pValue,
  mre_set_t  *pSet
);

/*-------- mre_tristate_define_new --------*/
mre_status_t mre_tristate_define_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- mre_tristate_test_new --------*/
mre_status_t mre_tristate_test_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);

/*-------- mre_tristate_set_new --------*/
mre_status_t mre_tristate_set_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- mre_tristate_reset_all_new --------*/
mre_status_t mre_tristate_reset_all_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);

boolean mre_parse_ascii_plmn(
  char const          *plmnStr,
  sys_plmn_id_s_type  *pPlmn
);

mre_status_t mre_plmn_list_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

mre_status_t mre_mcc_list_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

mre_named_object_t * mre_named_boolean_clone(
  mre_named_object_t   *pBoolean
);

#endif /* _MRE_LANG_H_ */
