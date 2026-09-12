#ifndef DS_3GPP_PDN_THROT_RULES_H
#define DS_3GPP_PDN_THROT_RULES_H

/*!
  @file
  ds_3gpp_pdn_throt_rules.h

  @brief
  PLMN related functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009 - 2014 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_pdn_throt_rules.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/14   vm      Initial version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_pdn_throt_spec.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif


/*===========================================================================

                         EXTERNAL TYPE DECLRATIONS

===========================================================================*/

/*-------------------------------------------------------------------------
  This enumeration type defines all possible PDN Throttling Rules.
  Each PDN Throttle Module shall register 2 function pointers corresponding 
  to its  Throttle Rule
  The function pointers shall handle:
  1. Conditions under which the Rule Passes
  2. Action to be taken if the rule passes.
 
  NOTE: Common Rule should always be the first rule in the list.
  Use the common rule to define behavior that is common across all
  throttling modules
------------------------------------------------------------------------*/
typedef enum
{
  DS_PDN_THROT_RULE_MIN = -1,  
  DS_PDN_THROT_RULE_COMMON = 0, 
  DS_PDN_THROT_RULE_SPEC,
  DS_PDN_THROT_RULE_DATA_RETRY,
  DS_PDN_THROT_RULE_GLOBAL_THROT,
  DS_PDN_THROT_RULE_CC_SP,
  DS_PDN_THROT_RULE_DAM,
  DS_PDN_THROT_RULE_RPM,
  DS_PDN_THROT_RULE_CONFIG,
  DS_PDN_THROT_RULE_MAX
} ds_pdn_throt_rule_e_type;

/*---------------------------------------------------------------------------
 
  A struct containing all throttling level info
----------------------------------------------------------------------------*/

typedef struct
{
  ds_pdn_context_s                         *rejected_pdn_cntx_p;
  cm_call_mode_info_s_type                 *mode_info_p;
  sys_sys_mode_e_type                       sys_mode;
  ps_iface_net_down_reason_type             down_reason;
  ds_pdn_throt_spec_parsed_backoff_info_s  *parsed_info_p;
} ds_pdn_throt_rule_info;

/*------------------------------------------------------------------------
  Clients shall register a function which checks the conditions under which
  the PDN Throttle Rule Passes and  the action that needs to be taken for
  each rule. The below function pointer will point to that
  function
-------------------------------------------------------------------------*/
typedef boolean (*ds_pdn_throt_rules_f_type)
(
  ds_pdn_throt_rule_info  *throt_rule_info_p
);

/*---------------------------------------------------------------------------
 
  A table of rules, conditions under which the rules will pass and the action
  that needs to be taken for each rule.
----------------------------------------------------------------------------*/
typedef struct
{
  
  ds_pdn_throt_rules_f_type    condition_f;
  ds_pdn_throt_rules_f_type    action_f;
} ds_pdn_throt_rule_s;

/*===========================================================================

                      EXTERNAL FUNCTION DECLRATIONS

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_pdn_throt_rule_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_pdn_throt_rule_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION DS_PDN_THROT_RULES_REGISTER

DESCRIPTION
  This function is used by various PDN THrottling modules to register
  the conditions under which a rule can pass and the action
  that needs to be taken if the rule passes.

PARAMETERS 
  rule_id     - Rule id
  rule_p      - Pointer to the structure holding the rule condition and
                rule action

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds_pdn_throt_rule_register
(
	ds_pdn_throt_rule_e_type rule_id,
  ds_pdn_throt_rule_s      *rule_p
);

/*===========================================================================
FUNCTION DS_PDN_THROT_RULES_REGISTER

DESCRIPTION
  This function will go through the list of PDN Throttle rules and execute
  the rule for which the rule condition gets satisfied. 

PARAMETERS 
  rejected_pdn_cntx_p - PDN Context Pointer
  mode_info_p         - Mode Info passed by CM
  sys_mode            - Sys Mode for which Throttling needs to be performed
  ps_down_reason      - Down Reason

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds_pdn_throt_rule_notify
(
  ds_pdn_throt_rule_info *throt_rule_info_p
);

/*===========================================================================
FUNCTION DS_PDN_THROT_RULES_VERIFY_CONDITION

DESCRIPTION
  This function verifies whether rule condition gets satisfied for the given rule. 

PARAMETERS 
  rule_id             - Rule Id
  rejected_pdn_cntx_p - PDN Context Pointer
  mode_info_p         - Mode Info passed by CM
  sys_mode            - Sys Mode for which Throttling needs to be performed
  ps_down_reason      - Down Reason

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_pdn_throt_rule_verify_condition
(
  ds_pdn_throt_rule_e_type         rule_id,
  ds_pdn_throt_rule_info          *throt_rule_info_p,
  sys_modem_as_id_e_type           subs_id
);

#endif /* DS_3GPP_PDN_THROT_RULES_H */
