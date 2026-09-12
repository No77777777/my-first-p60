/**
  @file mre_lang.c

  @brief  Code for the "language" used in policyman XML.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/mre_lang.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_dbg.h"
#include "mre_engine.h"
#include "mre_lang.h"
#include "mre_rules.h"
#include "mre_set.h"
#include "mre_util_i.h"
#include "policyman_diag.h"
#include "sys_v.h"

#include "mmgsdisessionlib_v.h"
#include <stringl/stringl.h>


struct mre_boolean_t
{
  MRE_NAMED_OBJECT_BASE;

  boolean  value;
  boolean  isWritable;
};


typedef struct
{
  MRE_NAMED_OBJECT_BASE;

  mre_tristate_state_t  state;
} mre_tristate_t;


/*=============================================================================
  Public utility functions
=============================================================================*/

/*-------- mre_boolean_get_value_from_set --------*/
mre_status_t mre_boolean_get_value_from_set(
  char const          *pName,
  boolean             *pValue,
  mre_set_t           *pSet
)
{
  mre_status_t   status = MRE_STATUS_ERR_NOT_PRESENT;
  mre_boolean_t *pBoolean;
  boolean        value = FALSE;

  pBoolean = (mre_boolean_t *) mre_named_object_find_in_set( pName,
                                                             MRE_NAMED_BOOLEAN,
                                                             pSet );
  if (pBoolean != NULL)
  {
    value  = pBoolean->value;
    status = MRE_STATUS_SUCCESS;
  }

  *pValue = value;
  return status;
}

/*-------- mre_boolean_get_value_for_subs --------*/
mre_status_t mre_boolean_get_value_for_subs(
  char const             *pName,
  boolean                *pValue,
  sys_modem_as_id_e_type  subsId,
  mre_namespace_info_t   *pNamespaceInfo
)
{
  mre_status_t   status = MRE_STATUS_ERR_NOT_PRESENT;
  mre_boolean_t *pBoolean;

  pBoolean = (mre_boolean_t *) mre_named_object_find_with_subs(
                                      pName,
                                      MRE_NAMED_BOOLEAN,
                                      subsId,
                                      pNamespaceInfo );
  if (pBoolean != NULL)
  {
    status = MRE_STATUS_SUCCESS;
    *pValue = pBoolean->value;
  }

  return status;
}



/*-------- mre_boolean_is_writable --------*/
boolean mre_boolean_is_writable(
  mre_boolean_t          *pBoolean
)
{
  return pBoolean->isWritable;
}


/*-------- mre_boolean_set_value --------*/
mre_status_t mre_boolean_set_value(
  mre_boolean_t          *pBoolean,
  boolean                 value
)
{
  mre_status_t   status = MRE_STATUS_ERR_NOT_WRITABLE;

  if (mre_boolean_is_writable(pBoolean))
  {
    pBoolean->value = value;
    status = MRE_STATUS_SUCCESS;
  }

  return status;
}


/*-------- mre_boolean_get_value --------*/
mre_status_t mre_boolean_get_value(
  char const          *pName,
  boolean             *pValue,
  mre_policy_t        *pPolicy
)
{
  mre_status_t   status = MRE_STATUS_ERR_NOT_PRESENT;
  mre_boolean_t *pBoolean;

  pBoolean = (mre_boolean_t *) mre_named_object_find( pName,
                                                      MRE_NAMED_BOOLEAN,
                                                      pPolicy );
  if (NULL != pBoolean)
  {
    status = MRE_STATUS_SUCCESS;
    *pValue = (boolean) pBoolean->value;
  }

  return status;
}


/*=============================================================================
  Conditions
=============================================================================*/

/*-------- mre_condition_new --------*/
mre_condition_t * mre_condition_new(
  size_t                    condSize,
  mre_policy_t             *pPolicy,
  pfn_evaluate_t            pEvaluate,
  destructor_ptr            pDtor,
  mre_xml_element_t const  *pElem
)
{
  mre_condition_t    *pCondition;
  destructor_ptr      pDestruct  = mre_condition_dtor;

  pCondition = (mre_condition_t *)mre_mem_alloc(condSize);

  if (pDtor != NULL)
  {
    pDestruct = pDtor;
  }

  ref_cnt_obj_init(pCondition, pDestruct);

  pCondition->isTrue           = pEvaluate;
  pCondition->lineNum          = mre_xml_get_line_number(pElem);
  pCondition->last_eval_result = MRE_COND_EVAL_NEVER;
  pCondition->cond_id          = MRE_COND_ID_UNDEF;

  return pCondition;
}

/*-----------------------------------------------------------------------------
  <true>
-----------------------------------------------------------------------------*/

typedef struct
{
  MRE_CONDITION_BASE;
} mre_true_condition_t;

STATIC boolean mre_condition_true_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mre_policy_t *pPolicy = EXECUTE_CTX_POLICY(pCtx);

  MRE_UT_MSG_3( SUBS_PREFIX CLIENT SRC_LINE "condition <true> returns 1",
                POLICY_SUBS(pPolicy),
                POLICY_CLIENT(pPolicy),
                LINE_NUM(pCondition) );

  return TRUE;
}


mre_status_t mre_condition_true_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  *ppCondition = (mre_condition_t *)MRE_CONDITION_NEW( mre_true_condition_t,
                                                       pPolicy,
                                                       mre_condition_true_evaluate,
                                                       NULL,
                                                       pElem );

  MRE_MSG_HIGH_2( SUBS_PREFIX CLIENT "mre_condition_true_new returned SUCCESS",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy) );

  return MRE_STATUS_SUCCESS;
}


/*-----------------------------------------------------------------------------
  <boolean_is_true>
-----------------------------------------------------------------------------*/


typedef struct
{
  MRE_CONDITION_BASE;

  char const  *pBooleanName;
} mre_boolean_test_t;


/*-------- mre_boolean_test_dtor --------*/
STATIC void mre_boolean_test_dtor(
  void  *pObj
)
{
  mre_boolean_test_t  *pMe = (mre_boolean_test_t *) pObj;

  MRE_MEM_FREE_IF(pMe->pBooleanName);
  mre_action_dtor(pMe);
}


/*-------- mre_boolean_test_evaluate --------*/
STATIC boolean mre_boolean_test_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mre_boolean_test_t  *pCond = (mre_boolean_test_t *) pCondition;
  mre_policy_t        *pPolicy   = EXECUTE_CTX_POLICY(pCtx);
  mre_boolean_t       *pBoolean;
  boolean              result;


  pBoolean = (mre_boolean_t *) mre_named_object_find( pCond->pBooleanName,
                                                      MRE_NAMED_BOOLEAN,
                                                      pPolicy );
  result =    pBoolean != NULL
          &&  pBoolean->value != 0;

  MRE_MSG_HIGH_4( SUBS_PREFIX CLIENT SRC_LINE "<boolean_test> returns %d",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pCond),
                  result );
  MRE_UT_MSG_S4(  SUBS_PREFIX SRC_LINE "<boolean_test name='%s'> returns %d",
                  POLICY_SUBS(pPolicy),
                  LINE_NUM(pCond),
                  pCond->pBooleanName,
                  result );

  return result;
}


/*-------- mre_boolean_test_new --------*/
mre_status_t mre_boolean_test_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mre_status_t         status       = MRE_STATUS_ERR_INVALID_CONDITION;
  mre_boolean_test_t  *pCondition   = NULL;
  char const          *pStr;
  char                *pBooleanName = NULL;

  /*  Get the name of the boolean to test.
   */
  pStr = mre_xml_get_attribute(pElem, "name");
  if (pStr == NULL)
  {
    MRE_UT_MSG_1(SUBS_PREFIX "<boolean_test> requires a 'name' attribute", POLICY_SUBS(pPolicy));
    goto Done;
  }

  pBooleanName = mre_str_dup(pStr);

  /*  Create the condition.
   */
  pCondition = MRE_CONDITION_NEW( mre_boolean_test_t,
                                  pPolicy,
                                  mre_boolean_test_evaluate,
                                  mre_boolean_test_dtor,
                                  pElem );

  pCondition->pBooleanName = pBooleanName;

  status = MRE_STATUS_SUCCESS;

Done:
  *ppCondition = (mre_condition_t *) pCondition;

  return status;
}



/*=============================================================================
  Compound conditions (those that modify other conditions)
=============================================================================*/

/*-----------------------------------------------------------------------------
  Utility functions for compound conditions
-----------------------------------------------------------------------------*/


/*-------- mre_collect_conditions --------*/
mre_set_t *mre_collect_conditions(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy
)
{
  size_t     nConds;
  policy_set_ctx_t  ctx;
  mre_status_t      status = MRE_STATUS_ERR_MALFORMED_XML;

  ctx.pPolicy = pPolicy;
  ctx.pSet    = NULL;

  nConds = mre_xml_num_children(pElem);
  if (0 != nConds)
  {
    ctx.pSet = mre_set_refcnt_new(nConds, nConds, NULL);

    status = mre_xml_process_children(pElem, mre_rule_add_condition, &ctx);
    if (MRE_FAILED(status))
    {
      MRE_RELEASE_IF(ctx.pSet);
    }
  }
  else
  {
    MRE_UT_MSG_S2( SUBS_PREFIX "<%s> must have sub-elements",
                   POLICY_SUBS(pPolicy),
                   mre_xml_get_tag(pElem));
  }

  return ctx.pSet;
}


/*-------- mre_compound_condition_new --------*/
STATIC mre_status_t mre_compound_condition_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  pfn_evaluate_t            pfnEvaluate,
  mre_condition_t         **ppCondition
)
{
  mre_status_t     status     = MRE_STATUS_ERR_INVALID_CONDITION;
  mre_condition_t *pCondition = NULL;
  mre_set_t       *pCondSet;

  /*  Get the subelements of this condition.
   */
  pCondSet = mre_collect_conditions(pElem, pPolicy);

  if (pCondSet == NULL)
  {
    MRE_UT_MSG_S2( SUBS_PREFIX "<%s> unable to collect conditions",
                   POLICY_SUBS(pPolicy),
                   mre_xml_get_tag(pElem) );
    goto Done;
  }

  /*  Create the condition
   */
  pCondition = MRE_CONDITION_NEW( mre_condition_t,
                                  pPolicy,
                                  pfnEvaluate,
                                  mre_condition_dtor,
                                  pElem );

  pCondition->pCondSet = pCondSet;

  status = MRE_STATUS_SUCCESS;

Done:
  *ppCondition = (mre_condition_t *) pCondition;
  MRE_UT_MSG_S3( SUBS_PREFIX "mre_condition_%s_new status %d",
                 POLICY_SUBS(pPolicy),
                 mre_xml_get_tag(pElem),
                 status );

  return status;
}


/*-----------------------------------------------------------------------------
  <not>
-----------------------------------------------------------------------------*/

/*-------- mre_condition_not_evaluate --------*/
STATIC boolean mre_condition_not_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mre_policy_t *pPolicy = EXECUTE_CTX_POLICY(pCtx);
  boolean           result;

  result = !mre_condset_evaluate(pCondition->pCondSet, pCtx);

  MRE_UT_MSG_4( SUBS_PREFIX CLIENT SRC_LINE "condition <not> returns %d",
                POLICY_SUBS(pPolicy),
                POLICY_CLIENT(pPolicy),
                LINE_NUM(pCondition),
                result);
  return result;
}


/*-------- mre_condition_not_new --------*/
mre_status_t mre_condition_not_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  return mre_compound_condition_new( pElem,
                                     pPolicy,
                                     mre_condition_not_evaluate,
                                     ppCondition );
}


/*-----------------------------------------------------------------------------
  <or>
-----------------------------------------------------------------------------*/

typedef struct
{
  boolean condsetIsTrue;
  boolean evaluateAll;
  void    *pCtx;
} or_condition_ctx;


/*-------- or_condition_test --------*/
STATIC boolean or_condition_test(
  void  *pElem,
  void  *pCtx
)
{
  mre_condition_t const *pCond = *((mre_condition_t const **) pElem);
  or_condition_ctx      *pOrCtx = (or_condition_ctx *) pCtx;

  /*  If the condition is true, we set the result of the test to TRUE, but
   *  return FALSE to stop execution.
   */
  if (pCond->isTrue(pCond, pOrCtx->pCtx))
  {
    pOrCtx->condsetIsTrue = TRUE;
    return pOrCtx->evaluateAll;
  }

  /*  Otherwise, return TRUE to continue the iteration.
   */
  return TRUE;
}


/*-------- mre_condset_any_true --------*/
boolean mre_condset_any_true(
  mre_set_t const *pCondSet,
  void            *pCtx,
  boolean          evaluateAll
)
{
  or_condition_ctx  ctx;

  ctx.condsetIsTrue = FALSE;
  ctx.pCtx          = pCtx;
  ctx.evaluateAll   = evaluateAll;

  mre_set_iterate(pCondSet, or_condition_test, &ctx);

  return ctx.condsetIsTrue;
}



/*-------- mre_condition_or_evaluate --------*/
STATIC boolean mre_condition_or_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mre_policy_t *pPolicy = EXECUTE_CTX_POLICY(pCtx);
  boolean       result;

  result = mre_condset_any_true(pCondition->pCondSet, pCtx, pCondition->evaluateAll);

  MRE_UT_MSG_4( SUBS_PREFIX CLIENT SRC_LINE "condition <any_of> returns %d",
                POLICY_SUBS(pPolicy),
                POLICY_CLIENT(pPolicy),
                LINE_NUM(pCondition),
                result );
  return result;
}


/*-------- mre_condition_or_new --------*/
mre_status_t mre_condition_or_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_condition_t        **ppCondition
)
{
  mre_status_t  status;
  char const   *pStr;
  boolean       evaluateAll = FALSE;

  pStr = mre_xml_get_attribute(pElem, "evaluate_all");
  if (NON_NULL(pStr))
  {
    evaluateAll = (strcasecmp(pStr, "true") == 0);
  }
  
  status = mre_compound_condition_new( pElem,
                                       pPolicy,
                                       mre_condition_or_evaluate,
                                       ppCondition );

  if (MRE_SUCCEEDED(status))
  {
    (*ppCondition)->evaluateAll = evaluateAll;
  }

  return status;
}


/*-----------------------------------------------------------------------------
  <and>
-----------------------------------------------------------------------------*/

/*-------- mre_condition_and_evaluate --------*/
STATIC boolean mre_condition_and_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mre_policy_t *pPolicy      = EXECUTE_CTX_POLICY(pCtx);
  boolean result;

  result = mre_condset_evaluate(pCondition->pCondSet, pCtx);

  MRE_UT_MSG_4( SUBS_PREFIX CLIENT SRC_LINE "condition <all_of> returns %d",
                POLICY_SUBS(pPolicy),
                POLICY_CLIENT(pPolicy),
                LINE_NUM(pCondition),
                result );
  return result;
}


/*-------- mre_condition_and_new --------*/
mre_status_t mre_condition_and_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  return mre_compound_condition_new( pElem,
                                     pPolicy,
                                     mre_condition_and_evaluate,
                                     ppCondition );
}


/*=============================================================================
  Actions
=============================================================================*/


/*-------- mre_execute_action --------*/
boolean mre_execute_action(
  mre_action_t  *pAction,
  void          *pCtx
)
{
  UPDATE_EVAL_COUNT(pAction);
  UPDATE_EVAL_TIME(pAction);
  UPDATE_ACTION_TIME(pAction);

  return pAction->execute(pAction, pCtx);
}


/*-------- clear_action_logging_info --------*/
void clear_action_logging_info(
  mre_action_t  *pAction
)
{
  pAction->last_eval_time = 0;
  pAction->eval_count     = 0;
}


/*-------- update_action_eval_count --------*/
void update_action_eval_count(
  mre_action_t  *pAction
)
{
  ++pAction->eval_count;
}


/*-------- update_action_eval_time --------*/
void update_action_eval_time(
  mre_action_t  *pAction
)
{
  pAction->last_eval_time = mre_util_get_time();
}


/*-------- update_action_action_time --------*/
STATIC void update_action_action_time(
  mre_action_t  *pAction
)
{
}


/*-------- update_action_else_time --------*/
STATIC void update_action_else_time(
  mre_action_t *pRule
)
{
}


/*-------- gather_action_rule_info --------*/
STATIC void gather_action_rule_info(
  mre_action_t            *pAction,
  mre_dump_ruleset_info_t *pRulesetInfo,
  mre_dump_rule_info_t    *pRuleInfo
)
{
}


/*-------- action_logging_vtbl --------*/
STATIC mre_logging_vtbl_t action_logging_vtbl =
{
  clear_action_logging_info,
  update_action_eval_count,
  update_action_eval_time,
  update_action_action_time,
  update_action_else_time,
  gather_action_rule_info
};

/*-------- mre_action_new_with_vtbl --------*/
mre_action_t * mre_action_new_with_vtbl(
  size_t                    actionSize,
  mre_policy_t             *pPolicy,
  pfn_execute_t             pExecute,
  destructor_ptr            pDtor,
  mre_xml_element_t const  *pElem,
  mre_logging_vtbl_t       *pVtbl
)
{
  mre_action_t       *pAction;
  destructor_ptr      pDestruct = mre_action_dtor;

  pAction = (mre_action_t *)mre_mem_alloc(actionSize);

  if (pDtor != NULL)
  {
    pDestruct = pDtor;
  }

  ref_cnt_obj_init(pAction, pDestruct);

  pAction->execute  = pExecute;
  pAction->lineNum  = mre_xml_get_line_number(pElem);
  pAction->pLogVtbl = pVtbl;

  CLEAR_LOGGING_INFO(pAction);

  return pAction;
}


/*-------- mre_action_new --------*/
mre_action_t  *mre_action_new(
  size_t                    actionSize,
  mre_policy_t             *pPolicy,
  pfn_execute_t             pExecute,
  destructor_ptr            pDtor,
  mre_xml_element_t const  *pElem
)
{
  return mre_action_new_with_vtbl(actionSize,
                                  pPolicy,
                                  pExecute,
                                  pDtor,
                                  pElem,
                                  &action_logging_vtbl );
}


/*-----------------------------------------------------------------------------
  <continue>
-----------------------------------------------------------------------------*/

/*-------- mre_continue_execute --------*/
STATIC boolean mre_continue_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  mre_policy_t *pPolicy = EXECUTE_CTX_POLICY(pCtx);

  mre_rules_continue_evaluation(pPolicy);
  MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT SRC_LINE "<continue> encountered - continuing rule evaluation",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pAction) );
  return TRUE;
}


/*-------- mre_continue_new --------*/
mre_status_t mre_continue_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  *ppAction = MRE_ACTION_NEW( mre_action_t,
                              pPolicy,
                              mre_continue_execute,
                              mre_action_dtor,
                              pElem );

  MRE_MSG_HIGH_2( SUBS_PREFIX CLIENT "mre_continue_new returned status 1",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy) );

  return MRE_STATUS_SUCCESS;
}


/*-----------------------------------------------------------------------------
  <boolean_define>
-----------------------------------------------------------------------------*/


typedef struct
{
  MRE_ACTION_BASE;

  char const                 *pBooleanName;  /* Name of the boolean to create */
  mre_named_obj_namespace_t   ns;
  boolean                     initialValue;
  boolean                     isWritable;
} boolean_define_action_t;


/*-------- mre_define_boolean_execute --------*/
STATIC boolean mre_define_boolean_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  boolean_define_action_t *pBooleanAction = (boolean_define_action_t *) pAction;
  mre_policy_t            *pPolicy        = EXECUTE_CTX_POLICY(pCtx);
  mre_boolean_t           *pBoolean;
  boolean                  succeeded;

  if (pPolicy == NULL)
  {
    MRE_MSG_ERROR_0("Internal error - executing <define_boolean> with no policy");
    return FALSE;
  }

  pBoolean = MRE_NAMED_OBJECT_NEW( mre_boolean_t,
                                   NULL,
                                   MRE_NAMED_BOOLEAN,
                                   pBooleanAction->pBooleanName );

  pBoolean->value = pBooleanAction->initialValue;
  pBoolean->isWritable = pBooleanAction->isWritable;

  succeeded = mre_named_object_add( (mre_named_object_t *) pBoolean,
                                    pBooleanAction->ns,
                                    pPolicy );
  ref_cnt_obj_release(pBoolean);

  return succeeded;
}


/*-------- mre_define_boolean_dtor --------*/
STATIC void mre_define_boolean_dtor(
  void  *pObj
)
{
  boolean_define_action_t  *pMe = (boolean_define_action_t *) pObj;

  MRE_MEM_FREE_IF(pMe->pBooleanName);
  mre_action_dtor(pMe);
}


/*-------- mre_boolean_define_new --------*/
mre_status_t mre_boolean_define_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t               status = MRE_STATUS_ERR_MALFORMED_XML;
  char const                *pName;
  mre_named_obj_namespace_t  ns;
  char const                *pInitial;
  char const                *pWritable;
  boolean                    initial = FALSE;
  boolean                    isWritable  = FALSE;
  boolean_define_action_t   *pAction;

  pName = mre_xml_get_attribute(pElem, "name");
  if (pName == NULL)
  {
    MRE_UT_MSG_1(SUBS_PREFIX "<boolean_define> 'name' attribute required", POLICY_SUBS(pPolicy));
    goto Done;
  }

  ns = mre_named_object_get_namespace(pElem);

  pInitial = mre_xml_get_attribute(pElem, "initial");
  status = mre_str_to_boolean(pInitial, &initial);
  if (MRE_FAILED(status))
  {
    MRE_UT_MSG_1(SUBS_PREFIX "<boolean_define> valid values for 'initial' attribute are 'true' and 'false'", POLICY_SUBS(pPolicy));
    goto Done;
  }

  pWritable = mre_xml_get_attribute(pElem, "writable");

  if (NON_NULL(pWritable))
  {
    status = mre_str_to_boolean(pWritable, &isWritable);
    if (MRE_FAILED(status))
    {
      MRE_UT_MSG_S3(SUBS_PREFIX SRC_LINE "<boolean_define> invalid 'writable' attribute value %s",
                    POLICY_SUBS(pPolicy),
                    mre_xml_get_line_number(pElem),
                    pWritable);
      goto Done;
    }
  }

  /*  Allocate and initialize the timer create action.
   */
  pAction = MRE_ACTION_NEW( boolean_define_action_t,
                            pPolicy,
                            mre_define_boolean_execute,
                            mre_define_boolean_dtor,
                            pElem );

  pAction->pBooleanName = mre_str_dup(pName);
  pAction->ns           = ns;
  pAction->initialValue = initial;
  pAction->isWritable   = isWritable;

  MRE_UT_MSG_S3(SUBS_PREFIX SRC_LINE "value for boolean %s",
                POLICY_SUBS(pPolicy),
                mre_xml_get_line_number(pElem),
                pInitial);

  *ppAction = (mre_action_t*) pAction;
  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}



/*-----------------------------------------------------------------------------
  <boolean_set>
-----------------------------------------------------------------------------*/

typedef struct
{
  MRE_ACTION_BASE;

  char const  *pBooleanName;  /* Name of the boolean to set */
  boolean      value;
  mre_set_t   *pConditions;
} boolean_set_action_t;


/*-------- mre_boolean_set_action_dtor --------*/
void mre_boolean_set_action_dtor(
  void  *pObj
)
{
  boolean_set_action_t  *pAction = (boolean_set_action_t *) pObj;

  MRE_MEM_FREE_IF(pAction->pBooleanName);
  MRE_RELEASE_IF(pAction->pConditions);
  mre_action_dtor(pAction);
}


/*-------- mre_boolean_set_execute --------*/
STATIC boolean mre_boolean_set_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  boolean_set_action_t  *pAct = (boolean_set_action_t *) pAction;
  mre_policy_t          *pPolicy   = EXECUTE_CTX_POLICY(pCtx);
  mre_boolean_t         *pBoolean;

  pBoolean = (mre_boolean_t *) mre_named_object_find( pAct->pBooleanName,
                                                      MRE_NAMED_BOOLEAN,
                                                      pPolicy );
  if (pBoolean == NULL)
  {
    MRE_UT_MSG_S3( SUBS_PREFIX SRC_LINE "<boolean_set> no boolean named %s",
                   POLICY_SUBS(pPolicy),
                   LINE_NUM(pAct),
                   pAct->pBooleanName );
    MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT SRC_LINE "<boolean_set> unable to find boolean",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy),
                    LINE_NUM(pAct) );
    return FALSE;
  }

  if (pAct->pConditions != NULL)
  {
    MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT SRC_LINE "<boolean_set> evaluating conditions for boolean",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy),
                    LINE_NUM(pAct) );
    pBoolean->value = mre_condset_evaluate(pAct->pConditions, pCtx);
  }
  else
  {
    pBoolean->value = pAct->value;
  }

  MRE_UT_MSG_S4( SUBS_PREFIX SRC_LINE "<boolean_set> boolean %s set to %d",
                 POLICY_SUBS(pPolicy),
                 LINE_NUM(pAct),
                 pBoolean->pName,
                 pBoolean->value );
  MRE_MSG_HIGH_4( SUBS_PREFIX CLIENT SRC_LINE "<boolean_set> boolean set to %d",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pAct),
                  pBoolean->value );

  return TRUE;
}


/*-------- mre_boolean_set_new --------*/
mre_status_t mre_boolean_set_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t           status = MRE_STATUS_ERR_INVALID_ACTION;
  char const            *pName;
  char const            *pValue;
  boolean                value = FALSE;
  size_t                 nConds;
  mre_set_t             *pCondSet = NULL;
  boolean_set_action_t  *pAction = NULL;

  pName = mre_xml_get_attribute(pElem, "name");
  if (pName == NULL)
  {
    MRE_UT_MSG_0("<boolean_set>: 'name' attribute required");
    goto Done;
  }

  /*  See if there are children of this element (conditions).
   */
  nConds = mre_xml_num_children(pElem);

  /*  see if there is a "value" attribute on this element.
   */
  pValue = mre_xml_get_attribute(pElem, "value");

  /*  You can't have both a value and conditions (children).
   */
  if (nConds != 0 && pValue != NULL)
  {
    MRE_MSG_HIGH_2( SUBS_PREFIX CLIENT "<boolean_set> can't have both a 'value' attribute and conditions",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy) );
    goto Done;
  }

  /*  You must have either a value or conditions (children).
   */
  if (nConds == 0 && pValue == NULL)
  {
    MRE_MSG_HIGH_2( SUBS_PREFIX CLIENT "<boolean_set> must have either a 'value' attribute or conditions",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy) );
    goto Done;
  }

  /*  If we have conditions, collect them; otherwise, get the value.
   */
  if (nConds != 0)
  {
    MRE_UT_MSG_S2( SUBS_PREFIX "<boolean_set> collecting conditions for boolean %s",
                   POLICY_SUBS(pPolicy),
                   pName );

    pCondSet = mre_collect_conditions(pElem, pPolicy);
    if (pCondSet == NULL)
    {
      MRE_UT_MSG_1(SUBS_PREFIX "<boolean_set> unable to collect conditions", POLICY_SUBS(pPolicy));
      goto Done;
    }
  }
  else
  {
    status = mre_str_to_boolean(pValue, &value);
    if (MRE_FAILED(status))
    {
      MRE_UT_MSG_2( SUBS_PREFIX "<boolean_set> 'value' attribute of 'true' or 'false' required for boolean",
                    POLICY_SUBS(pPolicy),
                    pName );
      goto Done;
    }
  }

  pAction = MRE_ACTION_NEW( boolean_set_action_t,
                            pPolicy,
                            mre_boolean_set_execute,
                            mre_boolean_set_action_dtor,
                            pElem );

  pAction->pBooleanName = mre_str_dup(pName);
  pAction->value = value;
  pAction->pConditions = pCondSet;

  *ppAction = (mre_action_t *) pAction;
  status = MRE_STATUS_SUCCESS;

Done:
  MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT "mre_boolean_set_new returned status %d",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  status );
  return status;
}


/*=============================================================================
  Action to run an embedded rule.
=============================================================================*/


/*-------- mre_condition_is_true --------*/
STATIC boolean mre_condition_is_true(
  mre_condition_t *pCond,
  void            *pCtx
)
{

  /*  If the condition is true, we continue execution; otherwise we stop
   *  evaluating conditions in the set.
   */

  pCond->last_eval_result = pCond->isTrue(pCond, pCtx);

  return pCond->last_eval_result;
}


/*-------- condition_is_true --------*/
STATIC boolean condition_is_true(
  void  *pElem,
  void  *pCtx
)
{
  mre_condition_t *pCond = *((mre_condition_t **) pElem);

  return mre_condition_is_true(pCond, pCtx);
}



/*-------- mre_condset_evaluate --------*/
boolean mre_condset_evaluate(
  mre_set_t const   *pCondSet,
  void              *pCtx
)
{
  return mre_set_iterate((mre_set_t *) pCondSet, condition_is_true, pCtx);
}


/*-------- mre_execute_if --------*/
STATIC void mre_execute_if(
  mre_rule_t        *pRule,
  mre_policy_t      *pPolicy,
  void              *pCtx
)
{
  boolean condsTrue;

  MRE_POLICY_CONTEVAL(pPolicy) = MRE_POLICY_CONTINUEBYDEFAULT(pPolicy);

  condsTrue = mre_condition_is_true(pRule->pCond, pCtx);

  if (condsTrue)
  {
    MRE_UT_MSG_2( CLIENT SRC_LINE "executing actions for <if>/<rule>",
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pRule) );
    UPDATE_ACTION_TIME(pRule);

    MRE_POLICY_DESCEND_LEVEL(pPolicy);
    execute_action(&pRule->pThenAction, pCtx);
    MRE_POLICY_ASCEND_LEVEL(pPolicy);
  }
  else if (NULL != pRule->pElseAction)
  {
    MRE_UT_MSG_2( CLIENT SRC_LINE "executing 'else' actions for <if>/<rule>",
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pRule) );
    UPDATE_ELSE_TIME(pRule);

    MRE_POLICY_DESCEND_LEVEL(pPolicy);
    execute_action(&pRule->pElseAction, pCtx);
    MRE_POLICY_ASCEND_LEVEL(pPolicy);
  }
  else if (MRE_POLICY_AT_TOP_LEVEL(pPolicy))
  {
    MRE_POLICY_CONTEVAL(pPolicy) = TRUE;
  }
}


/*-------- mre_if_action_execute --------*/
boolean mre_if_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  if_action_t   *pAct       = (if_action_t *)pAction;
  mre_policy_t  *pPolicy    = EXECUTE_CTX_POLICY(pCtx);
  boolean       contEval;

  MRE_UT_MSG_2( CLIENT SRC_LINE "evaluating conditions for <if>/<rule>",
                POLICY_CLIENT(pPolicy),
                LINE_NUM(pAct) );

  /*  Embedded 'if' actions are just like rules, except for one thing: the fact
   *  that the actions are executed does *not* mean that rule evaluation should
   *  stop.  If the enclosing top-level rule has a <continue />, an 'if' action
   *  should not override this.  On the other hand, if the 'if' does a <continue />
   *  *that* should be honored.
   *
   *  This is enforced by checking the 'continue' result after evaluating the
   *  'if'.  If it is TRUE, we leave it as-is.  If it is FALSE, we restore the
   *  state prior to evaluating the 'if'.
   */

  contEval = MRE_POLICY_AT_TOP_LEVEL(pPolicy) ? MRE_POLICY_CONTINUEBYDEFAULT(pPolicy)
                                              : MRE_POLICY_CONTEVAL(pPolicy);

  mre_execute_if(pAct, pPolicy, pCtx);

  if (!MRE_POLICY_CONTEVAL(pPolicy))
  {
    MRE_POLICY_CONTEVAL(pPolicy) = contEval;
  }

  /*  Now set whether execution is to continue based on whether a <stop> has
   *  executed; otherwise, use the "continue evaluating" setting.
   */
  if (MRE_POLICY_STOPEVAL(pPolicy))
  {
    MRE_POLICY_CONTEVAL(pPolicy) = FALSE;
    contEval = FALSE;
  }
  else
  {
    contEval = MRE_POLICY_AT_TOP_LEVEL(pPolicy)? MRE_POLICY_CONTEVAL(pPolicy) : TRUE;
  }

  return contEval;
}

/*-------- mre_if_action_new --------*/
mre_status_t mre_if_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  boolean        succeeded;
  mre_rule_t    *pRule = NULL;
  if_action_t   *pAct;

  /*  Create the rule from the node.
   */
  succeeded = mre_rule_create(pPolicy, pElem, &pRule);
  if (succeeded)
  {
    *ppAction = (mre_action_t *) pRule;
  }

Done:
  return succeeded ? MRE_STATUS_SUCCESS : MRE_STATUS_ERR_MALFORMED_XML;
}


/*=============================================================================
  <select> action
=============================================================================*/

/*-------- mre_select_case_t --------*/
typedef struct
{
  _REF_CNT_OBJ;

  mre_condition_t *pCond;
  mre_action_t    *pAction;
  size_t           lineNum;
} mre_select_case_t;


/*-------- select_case_dtor --------*/
STATIC void select_case_dtor(
  void  *pObj
)
{
  mre_select_case_t *pMe = (mre_select_case_t *) pObj;

  MRE_RELEASE_IF(pMe->pCond);
  MRE_RELEASE_IF(pMe->pAction);
  mre_mem_free(pMe);
}

/*-------- mre_select_action_t --------*/
typedef struct
{
  MRE_ACTION_BASE;

  mre_set_t *pCaseSet;
} mre_select_action_t;


/*-------- select_action_dtor --------*/
STATIC void select_action_dtor(
  void  *pObj
)
{
  mre_select_action_t *pMe = (mre_select_action_t *) pObj;

  MRE_RELEASE_IF(pMe->pCaseSet);
  mre_action_dtor(pMe);
}


/*-------- case_execute --------*/
STATIC boolean case_execute(
  void  *pElem,
  void  *pCtx
)
{
  mre_select_case_t *pCase            = *((mre_select_case_t **) pElem);
  mre_policy_t      *pPolicy          = EXECUTE_CTX_POLICY(pCtx);
  boolean            caseWasSelected;


  MRE_UT_MSG_3( SUBS_PREFIX CLIENT SRC_LINE "Evaluating <select> case",
                POLICY_SUBS(pPolicy),
                POLICY_CLIENT(pPolicy),
                LINE_NUM(pCase) );
  caseWasSelected = mre_condition_is_true(pCase->pCond, pCtx);
  if (caseWasSelected)
  {
    MRE_UT_MSG_3( SUBS_PREFIX CLIENT SRC_LINE "Executing <select> case",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pCase) );
    mre_execute_action(pCase->pAction, pCtx);
  }

  return !caseWasSelected;
}


/*-------- select_action_execute --------*/
STATIC boolean select_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  mre_select_action_t const *pAct = (mre_select_action_t const *) pAction;
  mre_policy_t              *pPolicy          = EXECUTE_CTX_POLICY(pCtx);
  boolean                    noCaseWasSelected;
  boolean                    stopWasExecuted;

  noCaseWasSelected = mre_set_iterate(pAct->pCaseSet, case_execute, pCtx);
  stopWasExecuted   = MRE_POLICY_STOPEVAL(pPolicy);

  return noCaseWasSelected || (!stopWasExecuted && MRE_POLICY_CONTINUEBYDEFAULT(pPolicy));
}


/*-------- is_valid_case --------*/
STATIC boolean is_valid_case(
  mre_xml_element_t const *pElem
)
{
  /*  Case statements must have 2 children
   */
  {
    size_t                   nChildren;

    nChildren = mre_xml_num_children(pElem);
    if (nChildren != 2)
    {
      MRE_MSG_HIGH_2( SRC_LINE "In <case>: expected condition and action, but got %d elements instead",
                      mre_xml_get_line_number(pElem),
                      nChildren );
      return FALSE;
    }
  }

  /*  Case statements are named "case".
   */
  {
    char const  *pStr;

    pStr = mre_xml_get_tag(pElem);
    if (0 != strcasecmp(pStr, "case"))
    {
      MRE_MSG_HIGH_1( SRC_LINE "<select> statement can only contain <case> elements",
                      mre_xml_get_line_number(pElem) );
      return FALSE;
    }
  }

  return TRUE;
}


/*-------- get_case --------*/
STATIC mre_status_t get_case(
  mre_xml_element_t const  *pElem,
  void                    *pCtx
)
{
  mre_status_t             status = MRE_STATUS_ERR_MALFORMED_XML;
  policy_set_ctx_t        *pCaseCtx = (policy_set_ctx_t *) pCtx;
  boolean                  succeeded = FALSE;
  mre_select_case_t       *pCase = NULL;
  mre_xml_element_t const *pChild;

  if (!is_valid_case(pElem))
  {
    goto Done;
  }

  pCase = (mre_select_case_t *) mre_mem_alloc(sizeof(*pCase));
  ref_cnt_obj_init(pCase, select_case_dtor);
  LINE_NUM(pCase) = mre_xml_get_line_number(pElem);

  /*  First child of a case is the condition
   */
  pChild = mre_xml_get_child(pElem, 0);
  succeeded = mre_rule_get_condition(pChild, pCaseCtx->pPolicy, &pCase->pCond);
  if (!succeeded)
  {
    goto Done;
  }

  /*  Second child of a case is the action
   */
  pChild = mre_xml_get_child(pElem, 1);
  succeeded = mre_rule_get_single_action(pChild, pCaseCtx->pPolicy, &pCase->pAction);
  if (!succeeded)
  {
    goto Done;
  }

  mre_set_append(pCaseCtx->pSet, &pCase);
  status = MRE_STATUS_SUCCESS;

Done:
  MRE_RELEASE_IF(pCase);
  return status;
}


/*-------- gather_select_case_condition_statistics --------*/
boolean gather_select_case_condition_statistics(
  void  *pElem,
  void  *pCtx
)
{
  mre_select_case_t const *pCase        = *((mre_select_case_t const **) pElem);
  mre_dump_ruleset_info_t *pRulesetInfo = (mre_dump_ruleset_info_t *)pCtx;
  mre_dump_rule_info_t    *pRuleInfo;

  pRuleInfo = &pRulesetInfo->rule_info[pRulesetInfo->num_rules];

  pRuleInfo->num_conditions = 0;
  pRuleInfo->eval_count     = 0;
  pRuleInfo->act_id         = MRE_ACT_ID_CASE; // no official table entry

  gather_condition_statistics( (void *)&pCase->pCond,
                               pRuleInfo );

  // Each case is logged as a separate rule (case looks like 'if')
  pRulesetInfo->num_rules++;

  return (pRulesetInfo->num_rules < MRE_DIAG_NUM_RULE_INFO);
}

/*-------- gather_select_rule_info --------*/
STATIC void gather_select_rule_info(
  mre_action_t            *pAction,
  mre_dump_ruleset_info_t *pRulesetInfo,
  mre_dump_rule_info_t    *pRuleInfo
)
{
  mre_select_action_t const  *pSelAction = (mre_select_action_t const *) pAction;
  mre_condition_t   *pCond;

  pRuleInfo->num_conditions   = mre_set_num_elems(pSelAction->pCaseSet);
  pRuleInfo->last_eval_time   = pSelAction->last_eval_time;
  pRuleInfo->eval_count       = pSelAction->eval_count;
  pRuleInfo->act_id           = pSelAction->act_id;

  // Select is logged as a rule by itself (cases follow as separate rules)
  pRulesetInfo->num_rules++;

  mre_set_iterate( pSelAction->pCaseSet,
                   gather_select_case_condition_statistics,
                   (void *)pRulesetInfo );
}

/*-------- action_logging_vtbl --------*/
STATIC mre_logging_vtbl_t select_action_logging_vtbl =
{
  clear_action_logging_info,
  update_action_eval_count,
  update_action_eval_time,
  update_action_action_time,
  update_action_else_time,
  gather_select_rule_info     // select
};

/*-------- mre_select_action_new --------*/
mre_status_t  mre_select_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t         status = MRE_STATUS_ERR_MALFORMED_XML;
  mre_select_action_t *pAction = NULL;
  size_t               numCases;
  policy_set_ctx_t     ctx;

  numCases = mre_xml_num_children(pElem);
  if (0 == numCases)
  {
    MRE_MSG_ERROR_2(CLIENT SRC_LINE "No cases in <select> statement", POLICY_CLIENT(pPolicy), mre_xml_get_line_number(pElem));
    goto Done;
  }

  pAction = MRE_ACTION_NEW_WITH_VTBL( mre_select_action_t,
                                      pPolicy,
                                      select_action_execute,
                                      select_action_dtor,
                                      pElem,
                                      &select_action_logging_vtbl );

  pAction->pCaseSet = mre_set_refcnt_new(numCases, numCases, NULL);
  ctx.pPolicy  = pPolicy;
  ctx.pSet = pAction->pCaseSet;

  status = mre_xml_process_children(pElem, get_case, &ctx);

  *ppAction = (mre_action_t *) pAction;

Done:
  if (MRE_FAILED(status))
  {
    MRE_RELEASE_IF(pAction);
  }

  return status;
}


/*=============================================================================
  <stop> action
=============================================================================*/

/*-------- stop_action_execute --------*/
STATIC boolean stop_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  mre_policy_t *pPolicy  = EXECUTE_CTX_POLICY(pCtx);

  MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT SRC_LINE "stopping policy execution",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pAction) );

  MRE_POLICY_STOPEVAL(pPolicy) = TRUE;

  return FALSE;
}

/*-------- mre_stop_action_new --------*/
mre_status_t  mre_stop_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  *ppAction = MRE_ACTION_NEW( mre_action_t,
                              pPolicy,
                              stop_action_execute,
                              mre_action_dtor,
                              pElem );

  return MRE_STATUS_SUCCESS;
}

/*=============================================================================
  tristate related functions
=============================================================================*/

/*-------- mre_str_to_tristate --------*/
mre_status_t mre_str_to_tristate(
  char const           *pStr,
  mre_tristate_state_t *pTristate
)
{
  mre_status_t  status = MRE_STATUS_SUCCESS;

  if      (strcasecmp(pStr, "undef") == 0)
  {
    *pTristate = MRE_TRISTATE_UNDEF;
  }
  else if (strcasecmp(pStr, "true") == 0)
  {
    *pTristate = MRE_TRISTATE_TRUE;
  }
  else if (strcasecmp(pStr, "false") == 0)
  {
    *pTristate = MRE_TRISTATE_FALSE;
  }
  else
  {
    status = MRE_STATUS_ERR_MALFORMED_XML;
  }

  return status;
}

/*-------- tristate_get_xml_value --------*/
mre_status_t tristate_get_xml_value(
  mre_xml_element_t const *pElem,
  mre_tristate_state_t    *pValue
)
{
  mre_status_t  status = MRE_STATUS_SUCCESS;
  const char   *pStr;

  pStr = mre_xml_get_attribute(pElem, "value");
  if (NON_NULL(pStr) && NON_NULL(pValue))
  {
    status = mre_str_to_tristate(pStr, pValue);
    if (MRE_FAILED(status))
    {
      MRE_UT_MSG_0("<tristate_*> valid values for 'value' attribute are 'undef', 'true' and 'false'");
    }
  }
  else
  {
    MRE_MSG_ERROR_0("Failed to get tristate attribute 'value'");
  }

  return status;
}


/*-------- mre_tristate_get_value --------*/
mre_status_t mre_tristate_get_value(
  char const           *pName,
  mre_policy_t         *pPolicy,
  mre_tristate_state_t *pValue
)
{
  mre_status_t    status = MRE_STATUS_ERR_NOT_PRESENT;
  mre_tristate_t *pTristate;

  pTristate = (mre_tristate_t *) mre_named_object_find( pName,
                                                        MRE_NAMED_TRISTATE,
                                                        pPolicy );

  if (NULL != pTristate)
  {
    status  = MRE_STATUS_SUCCESS;
    *pValue = pTristate->state;
  }

  return status;
}


/*-------- mre_tristate_set --------*/
mre_status_t mre_tristate_set(
  char const           *pName,
  mre_policy_t         *pPolicy,
  mre_tristate_state_t  value
)
{
  mre_status_t    status = MRE_STATUS_ERR_NOT_PRESENT;
  mre_tristate_t *pTristate;

  pTristate = (mre_tristate_t *) mre_named_object_find( pName,
                                                        MRE_NAMED_TRISTATE,
                                                        pPolicy );

  if (NULL != pTristate)
  {
    MRE_MSG_MED_1("setting a tristate to value %d", value);
    MRE_UT_MSG_S2("setting tristate %s to value %d", pName, value);

    status           = MRE_STATUS_SUCCESS;
    pTristate->state = value;
  }

  return status;
}

/*=============================================================================
  <tristate_define>
=============================================================================*/

typedef struct
{
  MRE_ACTION_BASE;

  char const                 *pTristateName;  /* Name of the boolean to create */
  mre_named_obj_namespace_t   ns;
  mre_tristate_state_t        initialValue;
} tristate_define_action_t;


/*-------- mre_tristate_define_execute --------*/
STATIC boolean mre_tristate_define_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  tristate_define_action_t *pAct      = (tristate_define_action_t *) pAction;
  mre_policy_t             *pPolicy   = EXECUTE_CTX_POLICY(pCtx);
  mre_tristate_t           *pTristate;
  boolean                   succeeded = FALSE;

  pTristate = MRE_NAMED_OBJECT_NEW( mre_tristate_t,
                                    mre_named_object_dtor,
                                    MRE_NAMED_TRISTATE,
                                    pAct->pTristateName );

  pTristate->state = pAct->initialValue;

  succeeded = mre_named_object_add( (mre_named_object_t *) pTristate,
                                    pAct->ns,
                                    pPolicy );
  ref_cnt_obj_release(pTristate);

  return succeeded;
}


/*-------- mre_tristate_define_dtor --------*/
STATIC void mre_tristate_define_dtor(
  void  *pObj
)
{
  tristate_define_action_t  *pMe = (tristate_define_action_t *) pObj;

  MRE_MEM_FREE_IF(pMe->pTristateName);
  mre_action_dtor(pMe);
}


/*-------- mre_tristate_define_new --------*/
mre_status_t mre_tristate_define_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t               status = MRE_STATUS_ERR_MALFORMED_XML;
  char const                *pName;
  mre_named_obj_namespace_t  ns;
  mre_tristate_state_t       initial = MRE_TRISTATE_UNDEF;
  tristate_define_action_t  *pAction;

  pName = mre_xml_get_attribute(pElem, "name");
  if (NULL == pName)
  {
    MRE_UT_MSG_1(SUBS_PREFIX "<tristate_define> 'name' attribute required", POLICY_SUBS(pPolicy));
    goto Done;
  }

  ns = mre_named_object_get_namespace(pElem);

  status = tristate_get_xml_value(pElem, &initial);
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pAction =  MRE_ACTION_NEW( tristate_define_action_t,
                             pPolicy,
                             mre_tristate_define_execute,
                             mre_tristate_define_dtor,
                             pElem );

  pAction->pTristateName = mre_str_dup(pName);
  pAction->ns            = ns;
  pAction->initialValue  = initial;

  *ppAction = (mre_action_t*) pAction;
  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}


/*-----------------------------------------------------------------------------
  <tristate_set>
-----------------------------------------------------------------------------*/

typedef struct
{
  MRE_ACTION_BASE;

  char const  *pTristateName;  /* Name of the tristate to set */
  uint32       state;
  mre_set_t   *pConditions;
} tristate_set_action_t;


/*-------- mre_tristate_set_action_dtor --------*/
void mre_tristate_set_action_dtor(
  void  *pObj
)
{
  tristate_set_action_t  *pAction = (tristate_set_action_t *) pObj;

  MRE_MEM_FREE_IF(pAction->pTristateName);
  MRE_RELEASE_IF(pAction->pConditions);
  mre_action_dtor(pAction);
}


/*-------- mre_tristate_set_execute --------*/
STATIC boolean mre_tristate_set_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  tristate_set_action_t *pAct = (tristate_set_action_t *) pAction;
  mre_policy_t          *pPolicy   = EXECUTE_CTX_POLICY(pCtx);
  mre_tristate_t        *pTristate;

  pTristate = (mre_tristate_t *) mre_named_object_find( pAct->pTristateName,
                                                      MRE_NAMED_TRISTATE,
                                                      pPolicy );
  if (NULL == pTristate)
  {
    MRE_UT_MSG_S3( SUBS_PREFIX SRC_LINE "<tristate_set> no tristate named %s",
                   POLICY_SUBS(pPolicy),
                   LINE_NUM(pAct),
                   pAct->pTristateName );
    MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT SRC_LINE "<tristate_set> unable to find tristate",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy),
                    LINE_NUM(pAct) );
    return FALSE;
  }

  if (NULL != pAct->pConditions)
  {
    MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT SRC_LINE "<tristate_set> evaluating conditions for tristate",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy),
                    LINE_NUM(pAct) );
    pTristate->state = mre_condset_evaluate(pAct->pConditions, pCtx);
  }
  else
  {
    pTristate->state = pAct->state;
  }

  MRE_UT_MSG_S4( SUBS_PREFIX SRC_LINE "<tristate_set> tristate %s set to %d",
                 POLICY_SUBS(pPolicy),
                 LINE_NUM(pAct),
                 pTristate->pName,
                 pTristate->state );
  MRE_MSG_HIGH_4( SUBS_PREFIX CLIENT SRC_LINE "<tristate_set> tristate set to %d",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pAct),
                  pTristate->state );

  return TRUE;
}


/*-------- mre_tristate_set_new --------*/
mre_status_t mre_tristate_set_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t           status = MRE_STATUS_ERR_INVALID_ACTION;
  char const            *pName;
  char const            *pValue;
  mre_tristate_state_t   value = MRE_TRISTATE_UNDEF;
  size_t                 nConds;
  mre_set_t             *pCondSet = NULL;
  tristate_set_action_t *pAction = NULL;

  pName = mre_xml_get_attribute(pElem, "name");
  if (NULL == pName)
  {
    MRE_UT_MSG_0("<tristate_set>: 'name' attribute required");
    goto Done;
  }

  /*  See if there are children of this element (conditions).
   */
  nConds = mre_xml_num_children(pElem);

  /*  see if there is a "value" attribute on this element.
   */
  pValue = mre_xml_get_attribute(pElem, "value");

  /*  You can't have both a value and conditions (children).
   */
  if (nConds != 0 && pValue != NULL)
  {
    MRE_MSG_HIGH_2( SUBS_PREFIX CLIENT "<tristate_set> can't have both a 'value' attribute and conditions",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy) );
    goto Done;
  }

  /*  You must have either a value or conditions (children).
   */
  if (nConds == 0 && pValue == NULL)
  {
    MRE_MSG_HIGH_2( SUBS_PREFIX CLIENT "<tristate_set> must have either a 'value' attribute or conditions",
                    POLICY_SUBS(pPolicy),
                    POLICY_CLIENT(pPolicy) );
    goto Done;
  }

  /*  If we have conditions, collect them; otherwise, get the value.
   */
  if (nConds != 0)
  {
    MRE_UT_MSG_S2( SUBS_PREFIX "<tristate_set> collecting conditions for tristate %s",
                   POLICY_SUBS(pPolicy),
                   pName );

    pCondSet = mre_collect_conditions(pElem, pPolicy);
    if (pCondSet == NULL)
    {
      MRE_UT_MSG_1(SUBS_PREFIX "<tristate_set> unable to collect conditions", POLICY_SUBS(pPolicy));
      goto Done;
    }
  }
  else
  {
    status = mre_str_to_tristate(pValue, &value);
    if (MRE_FAILED(status))
    {
      MRE_UT_MSG_2( SUBS_PREFIX "<tristate_set> 'value' attribute of 'true' or 'false' required for tristate",
                    POLICY_SUBS(pPolicy),
                    pName );
      goto Done;
    }
  }

  pAction = MRE_ACTION_NEW( tristate_set_action_t,
                            pPolicy,
                            mre_tristate_set_execute,
                            mre_tristate_set_action_dtor,
                            pElem );

  pAction->pTristateName = mre_str_dup(pName);
  pAction->state         = value;
  pAction->pConditions   = pCondSet;

  *ppAction = (mre_action_t *) pAction;
  status = MRE_STATUS_SUCCESS;

Done:
  MRE_MSG_HIGH_3( SUBS_PREFIX CLIENT "mre_tristate_set_new returned status %d",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  status );
  return status;
}


/*-----------------------------------------------------------------------------
  <tristate_test>
-----------------------------------------------------------------------------*/


typedef struct
{
  MRE_CONDITION_BASE;

  char const           *pTristateName;
  mre_tristate_state_t  state;
} mre_tristate_test_t;


/*-------- mre_tristate_test_dtor --------*/
STATIC void mre_tristate_test_dtor(
  void  *pObj
)
{
  mre_tristate_test_t  *pMe = (mre_tristate_test_t *) pObj;

  MRE_MEM_FREE_IF(pMe->pTristateName);
  mre_action_dtor(pMe);
}


/*-------- mre_tristate_test_evaluate --------*/
STATIC boolean mre_tristate_test_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mre_tristate_test_t *pCond   = (mre_tristate_test_t *) pCondition;
  mre_policy_t        *pPolicy = EXECUTE_CTX_POLICY(pCtx);
  mre_tristate_t      *pTristate;
  boolean              result;


  pTristate = (mre_tristate_t *) mre_named_object_find( pCond->pTristateName,
                                                        MRE_NAMED_TRISTATE,
                                                        pPolicy );
  result =    pTristate != NULL
          &&  pTristate->state == pCond->state;

  MRE_MSG_HIGH_4( SUBS_PREFIX CLIENT SRC_LINE "<tristate_test> returns %d",
                  POLICY_SUBS(pPolicy),
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pCond),
                  result );
  MRE_UT_MSG_S4(  SUBS_PREFIX SRC_LINE "<tristate_test name='%s'> returns %d",
                  POLICY_SUBS(pPolicy),
                  LINE_NUM(pCond),
                  pCond->pTristateName,
                  result );

  return result;
}


/*-------- mre_tristate_test_new --------*/
mre_status_t mre_tristate_test_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mre_status_t           status     = MRE_STATUS_ERR_INVALID_CONDITION;
  mre_tristate_test_t   *pCondition = NULL;
  char const            *pName      = NULL;
  mre_tristate_state_t   value      = MRE_TRISTATE_TRUE;

  /*  Get the name of the tristate to test.
   */
  pName = mre_xml_get_attribute(pElem, "name");
  if (NULL == pName)
  {
    MRE_UT_MSG_1(SUBS_PREFIX "<tristate_test> requires a 'name' attribute", POLICY_SUBS(pPolicy));
    goto Done;
  }

  status = tristate_get_xml_value(pElem, &value);
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  /*  Create the condition.
   */
  pCondition = MRE_CONDITION_NEW( mre_tristate_test_t,
                                  pPolicy,
                                  mre_tristate_test_evaluate,
                                  mre_tristate_test_dtor,
                                  pElem );

  pCondition->pTristateName = mre_str_dup(pName);
  pCondition->state         = value;

  *ppCondition = (mre_condition_t *) pCondition;

  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}


/*=============================================================================
  <tristate_reset_all>
=============================================================================*/

typedef struct
{
  MRE_CONDITION_BASE;

  boolean condReturn;
} mre_tristate_reset_all_t;


/*-------- mre_tristate_reset_all_dtor --------*/
STATIC boolean reset_tristate(
  void  *pElem,
  void  *pCtx
)
{
  mre_tristate_t  *pTristate  = *(mre_tristate_t **) pElem;

  if (NULL != pTristate)
  {
    pTristate->state = MRE_TRISTATE_UNDEF;
    MRE_UT_MSG_S1("<tristate_reset_all> resetting tristate %s", pTristate->pName);
  }

  return TRUE;
}


/*-------- mre_tristate_reset_all_evaluate --------*/
STATIC boolean mre_tristate_reset_all_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mre_tristate_reset_all_t  *pCond   = (mre_tristate_reset_all_t *) pCondition;
  mre_policy_t              *pPolicy = EXECUTE_CTX_POLICY(pCtx);

  mre_named_object_iterate_over_type( MRE_NAMED_TRISTATE,
                                      pPolicy,
                                      reset_tristate,
                                      pCtx );

  return pCond->condReturn;
}


/*-------- tristate_reset_get_return --------*/
mre_status_t tristate_reset_get_return(
  mre_xml_element_t const *pElem,
  boolean                 *pReturn
)
{
  mre_status_t  status = MRE_STATUS_SUCCESS;
  const char   *pStr;

  pStr = mre_xml_get_attribute(pElem, "return");

  if (NULL != pStr)
  {
    status = mre_str_to_boolean(pStr, pReturn);
    if (MRE_FAILED(status))
    {
      MRE_UT_MSG_0("<tristate_reset_all> valid values for 'return' attribute are 'true' and 'false'");
    }
  }

  return status;
}


/*-------- mre_tristate_reset_all_new --------*/
mre_status_t mre_tristate_reset_all_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mre_status_t               status     = MRE_STATUS_ERR_INVALID_CONDITION;
  mre_tristate_reset_all_t  *pCondition = NULL;
  boolean                    condReturn = TRUE;

  status = tristate_reset_get_return(pElem, &condReturn);
  if (MRE_SUCCEEDED(status))
  {
    pCondition = MRE_CONDITION_NEW( mre_tristate_reset_all_t,
                                    pPolicy,
                                    mre_tristate_reset_all_evaluate,
                                    mre_condition_dtor,
                                    pElem );

    pCondition->condReturn = condReturn;

    *ppCondition = (mre_condition_t *) pCondition;
    status = MRE_STATUS_SUCCESS;
  }

  return status;
}


/*=============================================================================
   PLMN and MCC named lists
=============================================================================*/

/*-----------------------------------------------------------------------------
  Generic support for named list actions
-----------------------------------------------------------------------------*/

/*  This is the action to execute to make a named list for a policy.
 */
typedef struct
{
  MRE_ACTION_BASE;

  mre_named_obj_namespace_t  ns;
  mre_plmn_mcc_set_t        *pList;
} mre_list_action_t;


/*  Type of the function that reads the text inside the list element
 *  and returns a set that contains the appropriate members.
 */
typedef mre_set_t * (*pfn_read_list_t)(
  mre_xml_element_t const  *pElem,
  char              const  *pStr
);


/*-------- mre_named_list_action_dtor --------*/
/**
@brief  Destructor for a named list action.
*/
static void mre_named_list_action_dtor(
  void  *pObj
)
{
  mre_list_action_t  *pAction = (mre_list_action_t *) pObj;

  MRE_RELEASE_IF(pAction->pList);
  mre_action_dtor(pAction);
}


/*-------- mre_named_list_action_execute --------*/
static boolean mre_named_list_action_execute(
  mre_action_t const      *pAction,
  void                    *pCtx
)
{
  mre_list_action_t *pListAction = (mre_list_action_t *) pAction;
  mre_policy_t      *pPolicy     = EXECUTE_CTX_POLICY(pCtx);

  if (pPolicy == NULL)
  {
    MRE_MSG_ERROR_0("Internal error - executing named list action with no policy");
    return FALSE;
  }

  MRE_MSG_HIGH_2( CLIENT SRC_LINE "executing <plmn_list> or <mcc_list>",
                  POLICY_CLIENT(pPolicy),
                  LINE_NUM(pListAction) );

  return mre_named_object_add( (mre_named_object_t *)pListAction->pList,
                               pListAction->ns,
                               pPolicy );
}


/*-------- mre_named_list_action_new --------*/
/**
@brief  Function to create a new named list action.  This takes a pointer
        to the function to execute for this
*/
STATIC mre_action_t *mre_named_list_action_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_plmn_mcc_set_t        *pSet
)
{
  mre_list_action_t         *pAction;
  mre_named_obj_namespace_t  ns;

  ns = mre_named_object_get_namespace(pElem);

  pAction = MRE_ACTION_NEW( mre_list_action_t,
                            pPolicy,
                            mre_named_list_action_execute,
                            mre_named_list_action_dtor,
                            pElem );

  pAction->pList = pSet;
  pAction->ns    = ns;

  return (mre_action_t *) pAction;

}


/*-------- mre_read_mmgsdi_file --------*/
mre_status_t mre_read_mmgsdi_file(
  mmgsdi_session_id_type   sessionId,
  mmgsdi_file_enum_type    fileName,
  mmgsdi_data_type        *pRead_buffer
)
{
  mre_status_t              status        = MRE_STATUS_SUCCESS;
  mmgsdi_return_enum_type   mmgsdiStatus;
  mmgsdi_len_type           fileSize = 0;
  mmgsdi_data_type          fileDataSink = {0, NULL};

  mmgsdiStatus = mmgsdi_session_read_cache_file_size( sessionId,
                                                      fileName,
                                                      &fileSize );

  if (MMGSDI_SUCCESS == mmgsdiStatus)
  {
    fileDataSink.data_ptr = (uint8 *)mre_mem_alloc(fileSize);
    fileDataSink.data_len = fileSize;

    mmgsdiStatus = mmgsdi_session_read_cache( sessionId,
                                              fileName,
                                              fileDataSink );
  }

  if (MMGSDI_SUCCESS == mmgsdiStatus)
  {
    pRead_buffer->data_len = fileDataSink.data_len;
    pRead_buffer->data_ptr = fileDataSink.data_ptr;
  }
  else
  {
    MRE_MSG_ERROR_3( "mmgsdi_read_cache for file %d failed (mmgsdi_status %d), size %d",
                     fileName,
                     mmgsdiStatus,
                     fileSize );

    MRE_MEM_FREE_IF(fileDataSink.data_ptr);
    status = MRE_STATUS_FAILED;
  }

  return status;
}


/*-------- mre_named_list_get_includes --------*/
STATIC void mre_named_list_get_includes(
  mre_xml_element_t const *pElem,
  mre_plmn_mcc_set_t     **ppSet
)
{
  char const *pName;

  pName = mre_xml_get_attribute(pElem, "include");

  if (NULL != pName)
  {
    char token[10];

    while( mre_get_token(&pName, token, sizeof(token)))
    {
      if (strcasecmp(token, "hplmn") == 0)
      {
        (*ppSet)->includeHPLMN = TRUE;
      }
      else if (strcasecmp(token, "ehplmn") == 0)
      {
        (*ppSet)->includeEHPLMN = TRUE;
      }
    }
  }

  MRE_MSG_HIGH_2( "named list includes HPLMN %d, included eHPLMN list %d",
                  (*ppSet)->includeHPLMN,
                  (*ppSet)->includeEHPLMN );
}


/*-------- mre_named_mcc_plmn_set_new --------*/
mre_status_t  mre_named_mcc_plmn_set_new(
  char const                   *pName,
  mre_xml_element_t const      *pElem,
  mre_named_object_type_t       type,
  pfn_read_named_list_t         pFnRead,
  mre_plmn_mcc_set_t          **ppSet
)
{
  mre_status_t         status;
  size_t               elemSize = sizeof(sys_plmn_id_s_type);
  mre_plmn_mcc_set_t  *pPlmnMccSet;

  if (MRE_NAMED_MCC_SET == type)
  {
    elemSize = sizeof(sys_mcc_type);
  }

  pPlmnMccSet = MRE_NAMED_SET_NEW( mre_plmn_mcc_set_t,
                                   pName,
                                   type,
                                   elemSize,
                                   10,
                                   MRE_SET_NO_MAX_SIZE,
                                   NULL,
                                   NULL );

  status = pFnRead(pElem, (mre_named_set_t *)pPlmnMccSet);

  if (MRE_SUCCEEDED(status))
  {
    mre_named_list_get_includes(pElem, &pPlmnMccSet);
  }

  if (mre_plmn_mcc_set_is_valid(pPlmnMccSet))
  {

    *ppSet = pPlmnMccSet;
    MRE_MSG_HIGH_5( SUBS_PREFIX SRC_LINE "PLMN list created with %d items, includes HPLMN %d and EHPLMN list %d",
                    pPlmnMccSet->subsId,
                    mre_xml_get_line_number(pElem),
                    mre_set_num_elems(pPlmnMccSet->pSet),
                    pPlmnMccSet->includeHPLMN,
                    pPlmnMccSet->includeEHPLMN );
  }
  else
  {
    status = MRE_STATUS_ERR_MALFORMED_XML;
    MRE_RELEASE_IF(pPlmnMccSet);
  }

  return status;
}


/*-----------------------------------------------------------------------------
  PLMN list support
-----------------------------------------------------------------------------*/


/*-------- mre_plmn_list_read --------*/
mre_status_t mre_plmn_list_read(
  mre_xml_element_t const *pElem,
  mre_named_set_t         *pSet
)
{
  char const          *pStr;
  char                 plmnStr[10];
  sys_plmn_id_s_type   plmn;
  boolean              gotPlmn = FALSE;

  pStr = mre_xml_get_text(pElem);
  if (NON_NULL(pStr))
  {
    gotPlmn = TRUE;
    while (mre_get_token(&pStr, plmnStr, sizeof(plmnStr)))
    {
      if (!mre_parse_ascii_plmn(plmnStr, &plmn))
      {
        gotPlmn = FALSE;
        break;
      }

      mre_named_set_add(pSet, &plmn);
    }
  }

  MRE_MSG_HIGH_2( SRC_LINE "PLMN list created with %d items",
                  mre_xml_get_line_number(pElem),
                  mre_named_set_num_elems(pSet));

  return ( gotPlmn ? MRE_STATUS_SUCCESS
                   : MRE_STATUS_ERR_MALFORMED_XML );
}


/*-------- mre_plmn_list_new --------*/
STATIC mre_status_t  mre_plmn_list_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_plmn_mcc_set_t      **ppSet
)
{
  mre_status_t       status = MRE_STATUS_ERR_MALFORMED_XML;
  char const        *pName;

  pName = mre_xml_get_attribute(pElem, "name");
  if (NON_NULL(pName))
  {
    status = mre_named_mcc_plmn_set_new( pName,
                                         pElem,
                                         MRE_NAMED_PLMN_SET,
                                         mre_plmn_list_read,
                                         ppSet );
  }

  return status;
}


/*-------- mre_plmn_list_action_new --------*/
mre_status_t mre_plmn_list_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t        status;
  mre_plmn_mcc_set_t *pSet;

  status = mre_plmn_list_new(pElem, pPolicy, &pSet);

  if (MRE_SUCCEEDED(status))
  {
    *ppAction = mre_named_list_action_new(pElem, pPolicy, pSet);
  }

  return status;
}


/*-----------------------------------------------------------------------------
  MCC list support
-----------------------------------------------------------------------------*/

/*-------- mre_mcc_list_read --------*/
mre_status_t mre_mcc_list_read(
  mre_xml_element_t const *pElem,
  mre_named_set_t         *pSet
)
{
  mre_status_t  status = MRE_STATUS_ERR_MALFORMED_XML;
  char const   *pStr;
  char          mccStr[4];
  uint32        mcc;

  pStr = mre_xml_get_text(pElem);

  if (NULL == pStr)
  {
    goto Done;
  }

  /*  Add the MCCs to the set
   */
  while (mre_get_token(&pStr, mccStr, sizeof(mccStr)))
  {
    mcc = atoi(mccStr);
    if (mcc == 0)
    {
      goto Done;
    }

    mre_named_set_add(pSet, &mcc);
  }

  status = MRE_STATUS_SUCCESS;
  MRE_MSG_HIGH_2( SRC_LINE "MCC list created with %d items",
                  mre_xml_get_line_number(pElem),
                  mre_named_set_num_elems(pSet) );

Done:
  return status;
}


/*-------- mre_mcc_list_new --------*/
STATIC mre_status_t mre_mcc_list_new(
  mre_xml_element_t const     *pElem,
  mre_policy_t                *pPolicy,
  mre_plmn_mcc_set_t         **ppSet
)
{
  mre_status_t       status = MRE_STATUS_ERR_MALFORMED_XML;
  char const        *pName;

  pName = mre_xml_get_attribute(pElem, "name");
  if (NULL != pName)
  {
    status = mre_named_mcc_plmn_set_new( pName,
                                         pElem,
                                         MRE_NAMED_MCC_SET,
                                         mre_mcc_list_read,
                                         ppSet );
  }

  return status;
}


/*-------- mre_mcc_list_action_new --------*/
mre_status_t mre_mcc_list_action_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_action_t           **ppAction
)
{
  mre_status_t            status;
  mre_plmn_mcc_set_t     *pSet;

  status = mre_mcc_list_new(pElem, pPolicy, &pSet);
  if (MRE_SUCCEEDED(status))
  {
    *ppAction = mre_named_list_action_new(pElem, pPolicy, pSet);
  }

  return status;
}


/*-------- mre_named_boolean_clone --------*/
mre_named_object_t * mre_named_boolean_clone(
  mre_named_object_t *pBoolean
)
{
  mre_boolean_t *pBooleanClone;

  pBooleanClone = MRE_NAMED_OBJECT_NEW( mre_boolean_t,
                                        mre_named_object_dtor,
                                        MRE_NAMED_BOOLEAN,
                                        pBoolean->pName );

  pBooleanClone->value = ((mre_boolean_t *)pBoolean)->value;

  return (mre_named_object_t *)pBooleanClone;
}
