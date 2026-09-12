#if !defined(_MRE_H_)
#define _MRE_H_

/**
  @file mre.h

  @brief
*/
/*
    Copyright (c) 2014-2018 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/api/public/mre.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "comdef.h"
#include <sys/types.h>
#include "mmgsdilib_common.h"
#include "ref_cnt_obj.h"
#include "sys.h"

#include "mre_set.h"


#if !defined(IS_NULL)
#define  IS_NULL(p)   (NULL == (p))
#endif

#if !defined(NON_NULL)
#define  NON_NULL(p)  (!IS_NULL(p))
#endif


typedef enum
{
  MRE_STATUS_SUCCESS = 0,               /**< call succeeded */

  MRE_STATUS_FAILED,
  MRE_STATUS_ERR_MALFORMED_XML,         /**< XML in config file for policy is not well-formed */
  MRE_STATUS_ERR_INVALID_ACTION,        /**< XML for action is well-formed but not correct */
  MRE_STATUS_ERR_INVALID_CONDITION,     /**< XML for condition is well-formed but not correct */
  MRE_STATUS_ERR_NOT_PRESENT,           /**< item not present */
  MRE_STATUS_ERR_NOT_WRITABLE,          /**< Item is not writable */
} mre_status_t;


#define MRE_SUCCEEDED(status) (MRE_STATUS_SUCCESS == (status))
#define MRE_FAILED(status)    (!MRE_SUCCEEDED(status))

#define MRE_COND_EVAL_NEVER (-1)

typedef struct mre_xml_element_t        mre_xml_element_t;
typedef struct mre_condition_t          mre_condition_t;
typedef struct mre_action_t             mre_action_t;
typedef struct mre_timer_t              mre_timer_t;
typedef struct mre_policy_t             mre_policy_t;
typedef struct mre_policy_info_t        mre_policy_info_t;
typedef struct mre_namespace_info_t     mre_namespace_info_t;
typedef struct mre_dump_ruleset_info_t  mre_dump_ruleset_info_t;
typedef struct mre_dump_rule_info_t     mre_dump_rule_info_t;
typedef struct mre_boolean_t            mre_boolean_t;


typedef enum
{
  MRE_CLIENT_POLICYMAN = 0,
  MRE_CLIENT_LTE_RRC,
  MRE_CLIENT_MCFG,
} mre_client_t;

/*=============================================================================
  Policy rule conditions
=============================================================================*/

/*-------- pfnNewCondition_t --------*/
/**
@brief  Function to create a new condition from an XML element.

@param[in]  pElem       Pointer to the XML element being parsed as a condition.
@param[in]  pPolicy     Pointer to the policy being parsed.
@param[out] ppCondition Pointer to storage for a pointer to the condition parsed
                        from the XML.

@return

*/
typedef mre_status_t  (*pfnNewCondition_t)(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);


/*-------- pfn_evaluate_t --------*/
/**
@brief  Evaluate a condition in the given context.

@param[in]  pCond   Pointer to the condition to evaluate.
@param[in]  pCtx    Pointer to the context in which to evaluate the condition.

@return
  TRUE or FALSE
*/
typedef boolean (*pfn_evaluate_t)(
  mre_condition_t const *pCond,
  void                  *pCtx
);


/*-------- MRE_CONDITION_BASE --------*/
/**
@brief  The basic elements of any condition.  Should be included as the first
        element of the definition of any condition.

@params _REF_CNT_OBJ  Conditions are reference-counted; this enables that.
@params pPolicy       Pointer to the policy that this condition is part of.
@params isTrue        Pointer to the function to evaluate the condition.
@params pCondSet      Pointer to a set of conditions to be evaluated by the
                      isTrue function.  This is used for compound conditions
                      like <all_of> or <any_of>.
*/
#define MRE_CONDITION_BASE          \
  _REF_CNT_OBJ;                     \
  pfn_evaluate_t   isTrue;          \
  mre_set_t       *pCondSet;        \
  size_t           lineNum;         \
  uint16           cond_id;         \
  uint8            last_eval_result;\
  boolean          evaluateAll



/*-------- mre_condition_t --------*/
/**
@brief  Base (underived) condition.
*/
struct mre_condition_t
{
  MRE_CONDITION_BASE;
};


/*-------- mre_condition_map_element_t --------*/
/**
@brief  Element in an array that maps XML condition tags to the corresponding
        functions to create conditions.

@param  pTag          Pointer to the NUL-terminated ASCII string for the
                      condition.
@param  newCondition  Pointer to a function to parse the condition from an XML
                      element.
*/
typedef struct
{
  char const        *pTag;
  pfnNewCondition_t  pfnNew;
  uint16             cond_id;
} mre_condition_map_element_t;




/*-------- mre_condition_dtor --------*/
/**
@brief  Destructor for the MRE_CONDITION_BASE portion of a condition.
*/
void mre_condition_dtor(
  void  *pObj
);

/*-------- MRE_COND_ID_BASE --------*/
/**
  Define the Condition tag ids for external use                                      .
*/
#define MRE_COND_ID_CUST_BASE(MODULE) \
  MODULE##_COND_ID_UNDEF          = 0,  \
  MODULE##_COND_ID_COND           = 1,  \
  MODULE##_COND_ID_CONDITIONS     = 2,  \
  MODULE##_COND_ID_ALL_OF         = 3,  \
  MODULE##_COND_ID_ANY_OF         = 4,  \
  MODULE##_COND_ID_TRUE           = 5,  \
  MODULE##_COND_ID_NOT            = 6,  \
  MODULE##_COND_ID_BOOLEAN_TEST   = 7,  \
  MODULE##_COND_ID_TRISTATE_TEST  = 8,  \
  MODULE##_COND_ID_TRISTATE_RESET = 9

typedef enum {
  MRE_COND_ID_CUST_BASE(MRE)
} mre_cond_id_enum_t;

/*-------- mre_condition_new --------*/
/**
@brief  Creation and initialization for the MRE_CONDITION_BASE portion of a condition.
*/
mre_condition_t * mre_condition_new(
  size_t                    condSize,
  mre_policy_t             *pPolicy,
  pfn_evaluate_t            pEvaluate,
  destructor_ptr            pDtor,
  mre_xml_element_t const  *pElem
);

/*-------- MRE_CONDITION_NEW --------*/
#define MRE_CONDITION_NEW(condition_c_type, p_policy, p_eval, p_dtor, p_elem) \
  (condition_c_type *)mre_condition_new( sizeof(condition_c_type), \
                                       (p_policy), \
                                       (p_eval), \
                                       (p_dtor), \
                                       (p_elem) )

/*-------- mre_collect_conditions --------*/
/**
@brief  Collects the children of the element as an mre_set of conditions.

@param[in]  pElem     Pointer to the XML element being parsed.
@param[in]  pPolicy   Pointer to the policy being parsed.

*/
mre_set_t *mre_collect_conditions(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy
);


/*-------- mre_condset_evaluate --------*/
/**
@brief  Evaluate a set of conditions.

@param[in]  pCondSet  Set of conditions to evaluate.
@param[in]  pCtx      Evaluation context.

@return
  
*/
boolean mre_condset_evaluate(
  mre_set_t const *pCondSet,
  void            *pCtx
);


/*=============================================================================
  Policy rule actions
=============================================================================*/


/*-------- pfnNewAction_t --------*/
/**
@brief  Function to create a new action from an XML element.

@param[in]  pElem     Pointer to the XML element being parsed as an action.
@param[in]  pPolicy   Pointer to the policy being parsed.
@param[out] ppAction  Pointer to storage for a pointer to the action parsed
                      from the XML.
@return

*/
typedef mre_status_t  (*pfnNewAction_t)(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- pfn_execute_t --------*/
/**
@brief  Execute an action in the given context.

@param[in]  pAction   Pointer to the action to execute.
@param[in]  pCtx      Pointer to the context in which to execute the action.

*/
typedef boolean (*pfn_execute_t)(
  mre_action_t const  *pAction,
  void                *pCtx
);


/*-------- pfn_update_logging_t --------*/
/**
@brief  Function that updates specific logging information for an action.
*/
typedef void    (*pfn_update_logging_t)(
  mre_action_t  *pAction
);


/*-------- pfn_gather_info_t --------*/
/**
@brief  Function to gather rule logging information for an action.
*/
typedef void    (*pfn_gather_info_t)(
  mre_action_t            *pAction,
  mre_dump_ruleset_info_t *pRulesetInfo,
  mre_dump_rule_info_t    *pRuleInfo
);


/*-------- mre_logging_vtbl_t --------*/
/**
@brief  Vtable of functions for logging actions.
*/
typedef struct
{
  pfn_update_logging_t  clear_info;
  pfn_update_logging_t  update_eval_count;
  pfn_update_logging_t  update_eval_time;
  pfn_update_logging_t  update_action_time;
  pfn_update_logging_t  update_else_time;
  pfn_gather_info_t     gather_rule_info;
} mre_logging_vtbl_t;


#define LOG_VTBL(pAction)             pAction->pLogVtbl
#define CLEAR_LOGGING_INFO(pAction)   LOG_VTBL(pAction)->clear_info((mre_action_t *) pAction)
#define UPDATE_EVAL_COUNT(pAction)    LOG_VTBL(pAction)->update_eval_count((mre_action_t *) pAction)
#define UPDATE_EVAL_TIME(pAction)     LOG_VTBL(pAction)->update_eval_time((mre_action_t *) pAction)
#define UPDATE_ACTION_TIME(pAction)   LOG_VTBL(pAction)->update_action_time((mre_action_t *) pAction)
#define UPDATE_ELSE_TIME(pAction)     LOG_VTBL(pAction)->update_else_time((mre_action_t *) pAction)
#define GATHER_RULE_INFO(a, rsi, ri)  LOG_VTBL(pAction)->gather_rule_info((mre_action_t *) a, rsi, ri)


/*-------- MRE_ACTION_BASE --------*/
/**
@params _REF_CNT_OBJ    Actions are reference-counted; this enables that.
@params pPolicy         Pointer to the policy that this actions is part of.
@params execute         Pointer to the function to call to execute the condition.
@params pLogVtbl        Pointer to logging vtable for this action.
@params last_eval_time  Time at which this action was last executed.
@params eval_count      Number of times this actions has been executed.
*/
#define MRE_ACTION_BASE                 \
  _REF_CNT_OBJ;                         \
  pfn_execute_t        execute;         \
  mre_logging_vtbl_t  *pLogVtbl;        \
  uint32               last_eval_time;  \
  size_t               eval_count;      \
  size_t               lineNum;         \
  uint16               act_id



/*-------- mre_action_t --------*/
/**
@brief  Base (underived) action.
*/
struct mre_action_t
{
  MRE_ACTION_BASE;
};


/*-------- mre_action_map_element_t --------*/
/**
@brief  Element in an array that maps XML action tags to the corresponding
        functions to create actions.

@param  pTag        Pointer to the NUL-terminated ASCII string for the
                    action.
@param  newAction   Pointer to a function to parse the action from an XML
                    element.
*/
typedef struct
{
  char const      *pTag;
  pfnNewAction_t   pfnNew;
  uint16           act_id;
} mre_action_map_element_t;


/*-------- mre_action_dtor --------*/
/**
@brief  Destructor for the MRE_ACTION_BASE portion of an action.
*/
void mre_action_dtor(
  void  *pObj
);

/*-------- mre_action_new --------*/
/**
@brief  Creation and initialization for the MRE_ACTION_BASE portion of an action.
*/
mre_action_t * mre_action_new(
  size_t                    actionSize,
  mre_policy_t             *pPolicy,
  pfn_execute_t             pExecute,
  destructor_ptr            pDtor,
  mre_xml_element_t const  *pElem
);

/*-------- MRE_ACTION_NEW --------*/
#define MRE_ACTION_NEW(action_c_type, p_policy, p_exec, p_dtor, p_elem) \
  (action_c_type *)mre_action_new( sizeof(action_c_type), \
                                 (p_policy), \
                                 (p_exec), \
                                 (p_dtor), \
                                 (p_elem) )


/*-------- MRE_ACT_ID_BASE --------*/
/**
  Define the Action tag ids for external use.
  NOTE: CASE is not an official action, but stats needs an ID for it.
*/
#define MRE_ACT_ID_CUST_BASE(MODULE)    \
  MODULE##_ACT_ID_UNDEF           = 0,  \
  MODULE##_ACT_ID_IF              = 1,  \
  MODULE##_ACT_ID_RULE            = 2,  \
  MODULE##_ACT_ID_ACTIONS         = 3,  \
  MODULE##_ACT_ID_THEN            = 4,  \
  MODULE##_ACT_ID_ELSE            = 5,  \
  MODULE##_ACT_ID_BOOLEAN_DEFINE  = 6,  \
  MODULE##_ACT_ID_BOOLEAN_SET     = 7,  \
  MODULE##_ACT_ID_CONTINUE        = 8,  \
  MODULE##_ACT_ID_STOP            = 9,  \
  MODULE##_ACT_ID_SELECT          = 10, \
  MODULE##_ACT_ID_CASE            = 11, \
  MODULE##_ACT_ID_PLMN_LIST       = 12, \
  MODULE##_ACT_ID_MCC_LIST        = 13, \
  MODULE##_ACT_ID_TRISTATE_DEFINE = 14, \
  MODULE##_ACT_ID_TRISTATE_SET    = 15

typedef enum {
  MRE_ACT_ID_CUST_BASE(MRE)
} mre_act_id_enum_t;

/*=============================================================================
  Policy definitions
=============================================================================*/

/*-------- pfnNewPolicy_t --------*/
/**
@brief  Callback used by the parser to request a new instance of a policy
        for the given name.
*/
typedef mre_policy_t  *(*pfnNewPolicy_t)(
  char const              *pPolicyName,
  mre_xml_element_t const *pPolicyElem
);


/*-------- MRE_POLICY_BASE --------*/
/**
@brief  Base structure for a policy.

This macro must be the first element of any policy structure; private elements
specific to a given policy should be added after this.

@param  _REF_CNT_OBJ      Policies are reference-counted objects; this enables that.
@param  pName             NUL-terminated ASCII name of this policy.
@param  pInfo             Pointer to private-to-MRE information about the policy.
@param  last_exec_time    Time (low 32 bits from time_get_ms) at which policy last executed.
@param  elapsed_timeticks Number of timeticks taken by last policy execution.
@param  client            Client Id of the policy reqestor
@param  hplmn             HPLMN info obtained from the client
@param  pEhPLMNSet        Pointer to a set of EHPLMNs obtained from the client
*/
#define MRE_POLICY_BASE                       \
  _REF_CNT_OBJ;                               \
  char          const     *pName;             \
  mre_policy_info_t       *pInfo;             \
  pfnNewPolicy_t           pfnNewPolicy;      \
  uint32                   last_exec_time;    \
  uint32                   elapsed_timeticks; \
  sys_modem_as_id_e_type   subs;              \
  mre_client_t             client;            \
  sys_plmn_id_s_type       hplmn;             \
  mre_set_t               *pEhPlmnSet


/*-------- MRE_POLICY_EXECUTE_CTX_BASE --------*/
/**
@brief  MRE Base Execution context.

This macro must be the first element of any policy structure; private elements
specific to a given policy should be added after this.

@param[in]  pPolicy   Pointer to the policy being executed.
*/
#define MRE_POLICY_EXECUTE_CTX_BASE  \
 mre_policy_t *pPolicy

/*-------- mre_policy_execute_ctx_t --------*/
typedef struct
{
  MRE_POLICY_EXECUTE_CTX_BASE;

} mre_policy_execute_ctx_t;

/*-------- mre_policy_t --------*/
/**
@brief  Base (underived) policy.
*/
struct mre_policy_t
{
  MRE_POLICY_BASE;
};


/*=============================================================================
  Policy Accessors
=============================================================================*/
#define EXECUTE_CTX_POLICY(pExecuteCtx)   (((mre_policy_execute_ctx_t *)(pExecuteCtx))->pPolicy)

/*=============================================================================
  Subscription (SUBS) Accessors
=============================================================================*/

#define POLICY_SUBS(pPolicy)       (((mre_policy_t *)(pPolicy))->subs)
#define POLICY_SUBS_SAFE(pPolicy)  ((NULL == (void *)(pPolicy)) ? SYS_MODEM_AS_ID_NONE : ((mre_policy_t *)(pPolicy))->subs)


/*=============================================================================
  Policy Client Accessors
=============================================================================*/
#define POLICY_CLIENT(pPolicy)             (((mre_policy_t *)(pPolicy))->client)
#define POLICY_PNAME(pPolicy)              (((mre_policy_t *)(pPolicy))->pName)
#define POLICY_LAST_EXEC_TIME(pPolicy)     (((mre_policy_t *)(pPolicy))->last_exec_time)
#define POLICY_ELAPSED_TIMETICKS(pPolicy)  (((mre_policy_t *)(pPolicy))->elapsed_timeticks)
#define POLICY_PFN_NEW_POLICY(pPolicy)     (((mre_policy_t *)(pPolicy))->pfnNewPolicy)


/*=============================================================================
  Public APIs to parse and execute policies.
=============================================================================*/

/*-------- mre_policy_create --------*/
/**
@brief  Parse an XML file and return a policy that corresponds to it.

@param[in]  pPolicyPath     EFS path to the policy file.
@param[in]  subsId          The context to read the policy file (AS_ID)
@param[in]  pCondTable      Pointer to a table of condition elements that map
                            condition tags to the functions to create them.
                            The table is terminated with an element with a NULL tag.
@param[in]  pActionTable    Pointer to a table of action elements that map
                            action tags to the functions to create them.
                            The table is terminated with an element with a NULL tag.
@param[in]  pfnNewPolicy    Pointer to a callback function that creates a new
                            instance of the policy whose name is passed to it.
                            This must be initialized as a reference-counted object.
                            The name comes from the "name" attribute of the
                            policy in the XML.
@param[in]  mreClient         Client identifier used for logging purposes to
                            identify while client has a policy running.
@param[out] ppPolicy        Pointer to storage for a pointer to the resulting
                            policy.

@return
  MRE_STATUS_SUCCESS            if policy creation is successful
  MRE_STATUS_ERR_NOT_PRESENT    if pPolicyPath is not present in EFS
  MRE_STATUS_ERR_MALFORMED_XML  if there was an error either parsing the XML
                                or the XML does not match the schema for
                                the policy
*/
mre_status_t mre_policy_create(
  char                          const  *pPolicyPath,
  sys_modem_as_id_e_type                subsId,
  mre_condition_map_element_t   const  *pCondTable,
  mre_action_map_element_t      const  *pActionTable,
  pfnNewPolicy_t                        pfnNewPolicy,
  mre_client_t                          mreClient,
  mre_policy_t                        **ppPolicy
);


/*-------- mre_policy_create_legacy --------*/
/**
@brief  This is for the private use of policyman, and will be phased out in
        favor of mre_policy_create there as well.

@param[in]  pPolicyPath     EFS path to the policy file.
@param[in]  subsId          The context to read the policy file (AS_ID)
@param[in]  pCondTable      Pointer to a table of condition elements that map
                            condition tags to the functions to create them.
                            The table is terminated with an element with a NULL tag.
@param[in]  pActionTable    Pointer to a table of action elements that map
                            action tags to the functions to create them.
                            The table is terminated with an element with a NULL tag.
@param[in]  pfnNewPolicy    Pointer to a callback function that creates a new
                            instance of the policy whose name is passed to it.
                            This must be initialized as a reference-counted object.
                            The name comes from the "name" attribute of the
                            policy in the XML.
@param[out] ppPolicy        Pointer to storage for a pointer to the resulting
                            policy.

@return
  MRE_STATUS_SUCCESS            if policy creation is successful
  MRE_STATUS_ERR_NOT_PRESENT    if pPolicyPath is not present in EFS
  MRE_STATUS_ERR_MALFORMED_XML  if there was an error either parsing the XML
                                or the XML does not match the schema for
                                the policy
*/
mre_status_t mre_policy_create_legacy(
  char                          const  *pPolicyPath,
  sys_modem_as_id_e_type                subsId,
  mre_condition_map_element_t   const  *pCondTable,
  mre_action_map_element_t      const  *pActionTable,
  pfnNewPolicy_t                        pfnNewPolicy,
  mre_policy_t                        **ppPolicy
);


/*-------- mre_policy_init --------*/
/**
@brief  Function to initialize a policy using the context passed.  This executes
        the initial actions specified in the <initial> block of the XML.

@param  pPolicy         Pointer to the policy to initialize.
@param  pNamespaceInfo  Pointer to a mre_namespace_info_t that should be used
                        by *all* policies of the client.  It is the client's
                        responsibility to release any non-NULL named object
                        sets contained in this structure upon termination.
@param  pCtx            Pointer to an execution context for your rules.  This
                        will be passed to all condition evaluation and action
                        execution functions called when evaluating the policy.

@return
  None
*/
void mre_policy_init(
  mre_policy_t          *pPolicy,
  mre_namespace_info_t  *pNamespaceInfo,
  mre_policy_execute_ctx_t  *pCtx
);


/*-------- mre_policy_execute --------*/
/**
@brief  Function to execute the rules for a policy using the context passed.

@param  pPolicy       Pointer to the policy to execute.
@param  precondsMet   The bitmask of preconditions (as defined by your module)
                      that have currently been met.  The MRE will determine
                      which rules to run based on whether all preconditions
                      of the policy have been met.
@param  pCtx          Pointer to an execution context for your rules.  This
                      will be passed to all condition evaluation and action
                      execution functions called when evaluating the policy.

@return
  None
*/
void mre_policy_execute(
  mre_policy_t  *pPolicy,
  mre_policy_execute_ctx_t  *pCtx
);


/*-------- mre_policy_is_enabled --------*/
/**
@brief  Return whether the policy is enabled (i.e. was successfully initialized).

Note that it is NOT necessary to call this in order to decide whether you can
call mre_policy_execute() - MRE will handle that situation for you.  This API
is present in case you have code that executes off your task thread and needs
to know if the policy is enabled.

@param  pPolicy   Pointer to the policy to check.

@return
  TRUE  if the policy was successfully initialized
  FALSE otherwise
*/
boolean mre_policy_is_enabled(
  mre_policy_t  *pPolicy
);


/*-------- mre_policy_are_preconds_met --------*/
/**
@brief  Return whether preconditions for the policy have been met.

Note that it is NOT necessary to call this in order to decide whether you can
call mre_policy_execute() - MRE will handle that situation for you.  This API
is present in case you have code that executes off your task thread and needs
to know if the policy's preconditions have been met.

@param  pPolicy     Pointer to the policy whose preconditions should be checked.
@param  precondsMet The bitmask of preconditions that have actually been met.

@return
  TRUE  if all preconditions required for the policy have been met.
  FALSE otherwise
*/
boolean mre_policy_are_preconds_met(
  mre_policy_t  *pPolicy,
  uint32         precondsMet
);


/*-------- mre_policy_dtor --------*/
/**
@brief  Destructor for the base portion of the MRE policy.  Call this from
        the destructor for your specific policy.

@param  pObj  Pointer to the policy.

@return
  None
*/
void mre_policy_dtor(
  void  *pObj
);




/*=============================================================================
  Declare some atomic operations not available otherwise
=============================================================================*/

/*-------- mre_atomic_swap --------*/
/**
@brief  Do an atomic swap, putting a value into a memory location and returning
        the value that was there.

@param[in]  pTarget   Pointer to the memory location to be swapped with.
@param[in]  value     Value to put in that location.

@return
  The value that was previously in the location pointed to be pTarget.
*/
uint32  mre_atomic_swap(
  uint32 volatile *pTarget,
  uint32           value
);


#define MRE_RELEASE_IF(p)                                           \
  do {                                                              \
    void *pObj;                                                     \
                                                                    \
    pObj = (void *) mre_atomic_swap((uint32 *) &(p), (uint32) NULL);\
    if (NULL != pObj)                                               \
    {                                                               \
      ref_cnt_obj_release(pObj);                                    \
    }                                                               \
  } while (0)


/*=============================================================================
  Utility macros to do string -> uint32 lookups
=============================================================================*/

#define MRE_STR_VALUE_TABLE(name)       mre_str_value_entry_t name[]
#define MRE_STR_VALUE_ENTRY(s, v)       {s, (uint32) (v)}
#define MRE_LOOKUP_STR_VALUE(t, s, type, pv)                      \
  {                                                               \
    uint32  ___value;                                             \
    status = mre_lookup_str_value(t, ARR_SIZE(t), s, &___value);  \
    if (MRE_SUCCEEDED(status))                                    \
    {                                                             \
      *pv = (type) ___value;                                      \
    }                                                             \
  }
#define MRE_LOOKUP_STR_VALUE_NO_STATUS(t, s, type, pv)                      \
  {                                                                         \
    uint32  ___value;                                                       \
    if (MRE_SUCCEEDED(mre_lookup_str_value(t, ARR_SIZE(t), s, &___value))); \
    {                                                                       \
      *pv = (type) ___value;                                                \
    }                                                                       \
  }

/*-----------------------------------------------------------------------------
  Use the macros above as below (don't use mre_lookup_str_value directly):

  typedef enum
  {
    ENUM_1,
    ENUM_2,
    ENUM_3
  } my_enum_t;

  mre_status_t convert_str_to_enum(
    char const  *pStr,
    my_enum_t   *pEnum
  )
  {
    mre_status_t  status;

    static MRE_STR_VALUE_TABLE(table) =
    {
      MRE_STR_VALUE_ENTRY("enum1",  ENUM_1),
      MRE_STR_VALUE_ENTRY("enum2",  ENUM_2),
      MRE_STR_VALUE_ENTRY("enum3",  ENUM_3),
    };

    MRE_LOOKUP_STR_VALUE(table, pStr, my_enum_t, pEnum);

    return status;
  }

  The macros take care of all casting, plus getting the size of the array.
  The MRE_LOOKUP_STR_VALUE and MRE_LOOKUP_STR_VALUE_NO_STATUS macro takes the
  type of the enum being returned in  the 'type' parameter.

  !!!! NOTE: !!!!
  The MRE_LOOKUP_STR_VALUE macro assumes that an mre_status_t named 'status'
  is defined in the calling function.
  The MRE_LOOKUP_STR_VALUE_NO_STATUS macro does not make that assumption and
  returns no status
-----------------------------------------------------------------------------*/


/*-------- mre_str_value_entry_t --------*/
typedef struct
{
  char const *pStr;
  uint32      value;
} mre_str_value_entry_t;



/*=============================================================================
  Utility macros to do string -> uint64 lookups
=============================================================================*/

#define MRE_STR_VALUE64_TABLE(name)     mre_str_value64_entry_t name[]
#define MRE_STR_VALUE64_ENTRY(s, v)     {s, (uint64) (v)}
#define MRE_LOOKUP_STR_VALUE64(t, s, type, pv)                      \
  {                                                                 \
    uint64  ___value;                                               \
    status = mre_lookup_str_value64(t, ARR_SIZE(t), s, &___value);  \
    if (MRE_SUCCEEDED(status))                                      \
    {                                                               \
      *pv = (type) ___value;                                        \
    }                                                               \
  }
#define MRE_LOOKUP_STR_VALUE64_NO_STATUS(t, s, type, pv)                      \
  {                                                                           \
    uint64  ___value;                                                         \
    if (MRE_SUCCEEDED(mre_lookup_str_value64(t, ARR_SIZE(t), s, &___value))); \
    {                                                                         \
      *pv = (type) ___value;                                                  \
    }                                                                         \
  }


/*-------- mre_str_value64_entry_t --------*/
typedef struct
{
  char const *pStr;
  uint64      value;
} mre_str_value64_entry_t;


/*-------- mre_lookup_str_value --------*/
/**
@brief  Translates a string to an integer value based on the table passed in.
        All string comparisons are done in a case-insensitive manner.

@param[in]  pTable    Pointer to an array of mre_str_value_entry_t entries.
@param[in]  nEntries  Number of entries in pTable.
@param[in]  pStr      The string for which the value is desired.
@param[out] pValue    Pointer to memory in which to store the value, if found.

@return
  MRE_STATUS_SUCCESS if the string was found in the table.
  MRE_STATUS_ERR_NOT_PRESENT if the string was not found in the table.
*/
mre_status_t mre_lookup_str_value(
  mre_str_value_entry_t *pTable,
  size_t                 nEntries,
  char const            *pStr,
  uint32                *pValue
);


/*-------- mre_lookup_str_value64 --------*/
/**
@brief  Translates a string to a 64-bit integer value based on the table passed in.
        All string comparisons are done in a case-insensitive manner.

@param[in]  pTable    Pointer to an array of mre_str_value_entry_t entries.
@param[in]  nEntries  Number of entries in pTable.
@param[in]  pStr      The string for which the value is desired.
@param[out] pValue    Pointer to memory in which to store the value, if found.

@return
  MRE_STATUS_SUCCESS if the string was found in the table.
  MRE_STATUS_ERR_NOT_PRESENT if the string was not found in the table.
*/
mre_status_t mre_lookup_str_value64(
  mre_str_value64_entry_t *pTable,
  size_t                   nEntries,
  char const              *pStr,
  uint64                  *pValue
);


/*-------- mre_read_mmgsdi_file --------*/
/**
@brief  Function to read MMGSDI data associated to a file.

@param  sessionId       session for which file read is required.
@param  fileName        enum indicating which SIM file data is from
@param  pRead_buffer  Pointer to  read buffer for MMGSDI data

@return
  MRE_STATUS_SUCCESS if the IMSI-HPLMN is added successfully to policy.
  MRE_STATUS_FAILED otherwise.
*/
mre_status_t mre_read_mmgsdi_file(
  mmgsdi_session_id_type   sessionId,
  mmgsdi_file_enum_type    fileName,
  mmgsdi_data_type        *pRead_buffer
);


/*-------- mre_policy_set_hplmn --------*/
/**
@brief  Function to update IMSI-HPLMN  in a policy.  This function
          should be called after policy is initialized.

@param  pPolicy              Pointer to the policy to update.
@param  pImsiData          Pointer to SIM EF-IMSI data
@param  pAdData            Pointer to SIM EF-AD data

@return
  MRE_STATUS_SUCCESS if the IMSI-HPLMN is added successfully to policy.
  MRE_STATUS_FAILED otherwise.
*/
mre_status_t mre_policy_set_hplmn(
  mre_policy_t       *pPolicy,
  mmgsdi_data_type   *pImsiData,
  mmgsdi_data_type   *pAdData
);


/*-------- mre_policy_delete_hplmn --------*/
/**
@brief  Function to undefine IMSI-HPLMN  in a policy.  This function
          should be called after policy is initialized.

@param  pPolicy              Pointer to the policy to update.

@return
  None.
*/
void mre_policy_delete_hplmn(
  mre_policy_t *pPolicy
);


/*-------- mre_policy_create_ehplmn_set --------*/
/**
@brief  Function to create a EHPLMN Set from MMGSDI data

@param  pEhPlmnList  Pointer to the MMGSDI EHPLMN data

@return
  A MRE set of EHPLMNS if operation is successful.
  NULL otherwise.
*/
mre_set_t *mre_policy_create_ehplmn_set(
  mmgsdi_data_type   *pEhPLMNList
);


/*-------- mre_policy_add_ehplmn_set --------*/
/**
@brief  Function to update EHPLMN Set in a policy.  This function
          should be called after policy is initialized.

@param  pPolicy         Pointer to the policy to update.
@param  pEhPlmnSet  Pointer to a set of EHPLMNs.

@return
  MRE_STATUS_SUCCESS if the EHPLMN list is added successfully to policy.
  MRE_STATUS_FAILED otherwise.
*/
mre_status_t mre_policy_add_ehplmn_set(
  mre_policy_t   *pPolicy,
  mre_set_t      *pEhPlmnSet
);

/*-------- mre_policy_release_ehplmn_set --------*/
/**
@brief  Function to release EHPLMN set in a policy.  This function
          should be called after policy is initialized.

@param  pPolicy         Pointer to the policy to update.

@return
  None.
*/
void mre_policy_release_ehplmn_set(
  mre_policy_t       *pPolicy
);


#ifdef __cplusplus
}
#endif

#endif /* _MRE_H_ */
