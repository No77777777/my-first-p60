#ifndef _MRE_DIAG_H_
#define _MRE_DIAG_H_

/**
  @file mre_diag.c

  @brief Policy Manager diag macros.
*/

/*
    Copyright (c) 2015,2016 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/mre_diag.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include <sys/types.h>
#include "comdef.h"    /* Definition for basic types and macros */
#include "log.h"
#include "event_defs.h"
#include "log_codes.h"
#include "mre.h"

#if defined(T_WINNT) || defined (FEATURE_WINCE)
#error code not present
#endif


/*---------------------------------------------------------------------------
  LOG_MRE_POLICY_STATS_INFO structure
---------------------------------------------------------------------------*/
#define LOG_MRE_POLICY_STATS_INFO_LOG_PACKET_VERSION 1

// Log definition LOG_MRE_POLICY_STATS_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_MRE_POLICY_STATS_INFO_C   0x7777

#define MRE_DIAG_NUM_COND_VAL    8
#define MRE_DIAG_NUM_RULE_INFO  24

/**
  The structure for each condition
*/
PACK(struct) mre_dump_cond_info_t {
  uint16                line_number; ///< the line number where this condition is used
  uint16                cond_id;     ///< the condition identifier
  byte                  result;      ///< the condition result (true/false)
  byte                  child_cnt;   ///< the number of sub-conditions inside this one
};

/**
  The structure for each rule
*/
PACK(struct) mre_dump_rule_info_t {
  uint32                               last_eval_time;                   ///< last time rule was evaluated
  uint32                               last_action_time;                 ///< last time actions were taken for rule
  uint16                               eval_count;                       ///< number of times rule has been evaluated
  uint16                               num_conditions;                   ///< number of conditions in rule
  uint16                               act_id;
  struct mre_dump_cond_info_t          cond_val[MRE_DIAG_NUM_COND_VAL];  ///< specifics of each condition evaluated
};

// The overall ruleset struct contains rules which contain conditions
PACK(struct) mre_dump_ruleset_info_t {
  uint16                               num_rules;      ///< number of rules in the policy
  mre_dump_rule_info_t                 rule_info[MRE_DIAG_NUM_RULE_INFO];
};

/**
  policy_num: policy number, unique per XML
  policy_type: policy type, subsidized, open-market or test
  policy_ver:  policy version, version information for XML
  last_exec_time: last time policy was executed
  elapsed_timeticks: duration of policy run (in timeticks)
*/
#define MRE_DUMP_POLICY_INFO_BASE             \
  size_t                  policy_num;         \
  size_t                  policy_type;        \
  size_t                  policy_ver;         \
  uint32                  last_exec_time;     \
  uint32                  elapsed_timeticks;  \
  mre_dump_ruleset_info_t ruleset_info

/**
  The base MRE policy info
*/
typedef PACK(struct) {
  MRE_DUMP_POLICY_INFO_BASE;
} mre_dump_policy_info_t;

/**
  The base MRE policy log packet
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;               // Diag Log Header. Required first, do not access directly.
  uint32                          version;           // Version of this structure. Required as first after log header.

  mre_dump_policy_info_t          policy_info;
} LOG_MRE_POLICY_STATS_INFO_type;

/*---------------------------------------------------------------------------
  (END) LOG_MRE_POLICY_STATS_INFO structures
---------------------------------------------------------------------------*/

/*-------- mre_rules_gather_ruleset_statistics --------*/
void mre_rules_gather_ruleset_statistics(
  mre_set_t               *pRuleset,
  void                    *pCtx,
  mre_dump_ruleset_info_t *pRulesetInfo
);


/*-------- mre_rules_gather_policy_statistics --------*/
void mre_rules_gather_policy_statistics(
  mre_policy_t            *pPolicy,
  mre_dump_policy_info_t  *pPolicyInfo
);


#if defined(T_WINNT) || defined (FEATURE_WINCE)
#error code not present
#endif

#endif /* _MRE_DIAG_H_ */
