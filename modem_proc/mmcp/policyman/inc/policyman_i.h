#ifndef _POLICYMAN_I_H_
#define _POLICYMAN_I_H_

/**
  @file policyman_i.h

  @brief Internal interface to Policy Manager
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_i.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre.h"
#include "mre_namedobj.h"
#include "policyman.h"

#define SXLTE_MCCS_STR  "sxlte_mccs"
#define SXLTE_PLMNS_STR "sxlte_plmns"


typedef enum
{
  POLICYMAN_BASE_NONE,
  POLICYMAN_BASE_CONFIG,
  POLICYMAN_BASE_HARDWARE,
  POLICYMAN_BASE_CURRENT
} policyman_base_t;

typedef enum
{
  SUBS_NONE,
  SUBS_THIS,
  SUBS_OTHER,
  SUBS_ALL,
  SUBS_ANY,
} policyman_subs_t;

typedef enum
{
  POLICYMAN_CMP_INVALID,
  POLICYMAN_CMP_EQUALS,
  POLICYMAN_CMP_EXCEEDS,
  POLICYMAN_CMP_EQUALS_OR_EXCEEDS,
} policyman_comparator_t;

typedef enum
{
  USE_DB_ONLY       = 0,
  USE_ITEMSET_ONLY,
  USE_DB_ITEMSET
} lookup_method_t;


#define POLICYMAN_UNNAMED_OBJ         MRE_UNNAMED_OBJ
#define POLICYMAN_NAMED_PLMN_SET      MRE_NAMED_PLMN_SET
#define POLICYMAN_NAMED_RF_BANDS      MRE_NAMED_RF_BANDS
#define POLICYMAN_NAMED_TIMER         MRE_NAMED_TIMER
#define POLICYMAN_NAMED_MCC_SET       MRE_NAMED_MCC_SET
#define POLICYMAN_NAMED_BOOLEAN       MRE_NAMED_BOOLEAN
#define POLICYMAN_NAMED_CONFIG        MRE_NAMED_CONFIG
#define POLICYMAN_NAMED_CA_BAND       MRE_NAMED_CA_BAND
#define policyman_named_object_type_t mre_named_object_type_t


#define POLICYMAN_NUM_SUBS            MAX_AS_IDS

#define POLICYMAN_MAX_SUB       ((sys_modem_as_id_e_type) (POLICYMAN_NUM_SUBS + 1))
#define SUBS_ID_TO_NSIM(subsId) ((size_t) (subsId))
#define NSIM_TO_SUBS_ID(nSim)   ((sys_modem_as_id_e_type) (nSim))
#define  OTHER_SUB(subsId)      ((SYS_MODEM_AS_ID_1 == (subsId)) ? SYS_MODEM_AS_ID_2 : SYS_MODEM_AS_ID_1)



/*  Opaque type definitions
 */
typedef struct policyman_config_info_t  policyman_config_info_t;
typedef struct policyman_cmd_t          policyman_cmd_t;
typedef struct policyman_policy_t       policyman_policy_t;
typedef struct policyman_state_t        policyman_state_t;
typedef struct policyman_timer_t        policyman_timer_t;
typedef struct policyman_condition_t    policyman_condition_t;
typedef struct policyman_action_t       policyman_action_t;
typedef struct rf_bands_item_t          rf_bands_item_t;

#define BASE_EXECUTE_CTX                  \
  MRE_POLICY_EXECUTE_CTX_BASE;         \
  policyman_state_t       *pState;        \
  mre_set_t               *pItemSet;   \
  sys_modem_as_id_e_type   subsId

typedef struct
{
  BASE_EXECUTE_CTX;

} policy_execute_ctx_t;

typedef struct
{
  mre_set_t               *pItemSet;
  policyman_item_t *pItem;
  sys_modem_as_id_e_type   subsId;
} cfgitem_execute_ctx_t;


/*=============================================================================
  Base Execute Accessors
=============================================================================*/
#define POLICY_EXECUTE_STATE(pPolicyExecuteCtx)    (((policy_execute_ctx_t *)(pPolicyExecuteCtx))->pState)
#define POLICY_EXECUTE_ITEMSET(pPolicyExecuteCtx)  (((policy_execute_ctx_t *)(pPolicyExecuteCtx))->pItemSet)
#define POLICY_EXECUTE_SUBS(pPolicyExecuteCtx)     (((policy_execute_ctx_t *)(pPolicyExecuteCtx))->subsId)

/*-------- policyman_init --------*/
void policyman_init(
  void
);


#if defined(TEST_FRAMEWORK)
#error code not present
#endif /* +TEST_FRAMEWORK. */

/*-------- policyman_per_subs_deinit --------*/
void policyman_per_subs_deinit(
  size_t subs
);


/*-------- policyman_is_initializing --------*/
boolean policyman_is_initializing(
  void
);


/*-------- policyman_test_mode_is_enabled --------*/
boolean policyman_test_mode_is_enabled(
  void
);


/*-------- policyman_any_test_mode_is_enabled --------*/
boolean policyman_any_test_mode_is_enabled(
  void
);


/*-------- policyman_FTM_is_enabled --------*/
boolean policyman_FTM_is_enabled(
  void
);

/*-------- policyman_set_test_mode --------*/
void policyman_set_test_mode(
  boolean value
);

/*-------- policyman_set_FTM --------*/
void policyman_set_FTM(
  boolean value
);

/*-------- policyman_get_global_sandbox --------*/
policyman_sandbox_t * policyman_get_global_sandbox(
  void
);

/*-------- policyman_is_global_sandbox_suspended --------*/
boolean policyman_is_global_sandbox_suspended(
  void
);

/*-------- policyman_global_sandbox_refresh_policies --------*/
void policyman_global_sandbox_refresh_policies(
  void
);

#endif /* _POLICYMAN_I_H_ */
