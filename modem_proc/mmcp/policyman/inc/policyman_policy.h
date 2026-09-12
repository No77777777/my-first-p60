#ifndef _POLICYMAN_POLICY_H_
#define _POLICYMAN_POLICY_H_

/**
  @file policyman_policy.h

  @brief Interface to a policy.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_policy.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre.h"
#include "policyman_rules.h"
#include "policyman_state.h"

#include "policyman_i.h"

typedef uint32  policy_attributes_t;

#define POLICY_ATTRIBUTE_NONE               0x00000000
#define POLICY_ATTRIBUTE_ENABLED            0x00000001


#define POLICYMAN_POLICY_ERROR_CAUSE(p)  (p->error_cause)

/*-------- POLICYMAN_POLICY_BASE --------*/
/**
@brief  Base structure for a policy.

This macro must be the first element of any policy structure; private elements
specific to a given policy should be added after this.
*/


#define POLICYMAN_POLICY_BASE                       \
  MRE_POLICY_BASE;                                  \
  policyman_cfg_error_cause  error_cause      


struct policyman_policy_t
{
  POLICYMAN_POLICY_BASE;
};


#define POLICY_IS_ENABLED(pPolicy)                                            \
          mre_policy_is_enabled((mre_policy_t *) pPolicy)

/*-------- policyman_policy_dtor --------*/
/**
@brief  Destructor for a base policy, called from destructors of derivied policies.
*/
void policyman_policy_dtor(
  void  *pObj
);

/*-------- policyman_policy_new --------*/
/**
@brief  Returns pointer to a freshly allocated policy object.
*/
mre_policy_t * policyman_policy_new(
  char const              *pPolicyName,
  mre_xml_element_t const *pPolicyElem
);

/*-------- policyman_policy_get_subs --------*/
/**
@brief  returns the subscription to which the policy belongs.
*/
sys_modem_as_id_e_type  policyman_policy_get_subs(
  mre_policy_t *pPolicy
);

/*-------- policyman_policy_run --------*/
/**
@brief  Execute a policy.
*/
boolean policyman_policy_run(
  void  *pElem,
  void  *pCtx
);


/*-------- policyman_policy_run_post --------*/
boolean policyman_policy_run_post(
  void  *pElem,
  void  *pCtx
);


/*-------- policyman_policy_initialize --------*/
/**
@brief  Read the policy_info file for this policy and use it to initialize
        the policy.
*/
boolean policyman_policy_initialize(
  policyman_policy_t  *pPolicy,
  policyman_state_t   *pState,
  mre_set_t           *pItemSet,
  boolean              keepInitSet
);

#endif /* _POLICYMAN_POLICY_H_ */
