/**
  @file policyman_serving_system.c

  @brief
*/

/*
    Copyright (c) 2013-2018,2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_serving_system.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "policyman_cm.h"
#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_diag.h"
#include "policyman_fullrat_config.h"
#include "policyman_nas.h"
#include "policyman_plmn.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_rat_capability.h"
#include "policyman_rules.h"
#include "policyman_serving_system.h"
#include "policyman_state.h"
#include "policyman_subs.h"
#include "policyman_task.h"
#include "policyman_timer.h"
#include "policyman_ue_mode.h"
#include "policyman_uim.h"
#include "policyman_util.h"
#include "policyman_xml.h"

#include <stringl/stringl.h>

#define INVALID_RAT_MASK  0xffffffff


/*=============================================================================
  Data Structure definitions.
=============================================================================*/
typedef struct
{
  /*  stack variables
   */
  boolean                   is_operational;
  sys_modem_as_id_e_type    subsId;
  sys_modem_stack_id_e_type stackId;
  byte                      pending_reject;          /* pending reject cause*/
  byte                      reject_cause;            /* registration reject cause*/
  sys_sys_id_type_e_type    id_type;                 /* PLMN id type */
  boolean                   serving_info_valid;      /* Whether PLMN should be used */
  sys_plmn_id_s_type        plmn;                    /* PLMN of the serving system */
  sys_plmn_mcc_list         mcc_list_3gpp2;          /* List of 3GPP2 MCCs  */
  sys_plmn_mcc_list         mcc_list_hdr;            /* List of HDR MCCs  */
  sys_mnc_type              mnc_3gpp2;               /* 3GPP2 MNC */
  sys_mnc_type              mnc_hdr;                 /* HDR MNC */
  sys_sys_mode_e_type       sys_mode;                /* System's mode */
  sys_srv_status_e_type     srv_status;              /* Service Status */
  sys_band_class_e_type     active_band;             /* serving RF band */
  sys_srv_domain_e_type     srv_domain;              /* Serving domain */
  int                       acq_fail_cnt;            /* tracks number of acq fails*/
  boolean                   radio_is_operational;    /* tracks stack operational status for Acquisition failures */
} policyman_stack_info_t;

struct policyman_ss_info_t
{
  sys_modem_as_id_e_type  asubs_id;                      /* Subscription for this info */
  sys_plmn_id_s_type      user_plmn;                     /* User Selected PLMN */
  sys_sim_state_e_type    sim_state;
  policyman_stack_info_t  stack[SYS_MODEM_STACK_ID_MAX];
};

/*=============================================================================
  Serving system Event callback definition.
=============================================================================*/
typedef struct
{
  POLICYMAN_CMD_HDR;
  policyman_cm_serving_info_t info;
} policyman_cm_serving_info_cb_t;

typedef struct
{
  POLICYMAN_CMD_HDR;

  sys_mcc_type  mcc;
  size_t        confidenceLevel;
  size_t        status;
} policyman_hlos_mcc_cmd_t;

typedef struct
{
  POLICYMAN_CMD_HDR;
  sys_modem_as_id_e_type     subsId;
  sys_modem_stack_id_e_type  stackId;
} policyman_cm_report_acq_fail_t;

typedef struct
{
  POLICYMAN_CMD_HDR;
  sys_modem_as_id_e_type   subsId;
  uint32                   stackMask;
} policyman_report_operational_stack_cmd_t;

typedef struct
{
  POLICYMAN_CMD_HDR;
  sys_modem_as_id_e_type   subsId;
  sys_sim_state_e_type     simState;
} policyman_report_sim_state_cmd_t;

/*=============================================================================
  Serving system Event callback definition for Multisim
=============================================================================*/
typedef struct
{
  POLICYMAN_CMD_HDR;
  cm_ss_event_e_type         evt;
  cm_mm_msim_ss_info_s_type  info;
  uint32                     timestamp;
} policyman_msim_cmss_evt_cb_t;

/*=============================================================================
  Serving System History definitions
=============================================================================*/
struct policyman_cmss_history_entry_s {
  size_t                  next;
  uint32                  timestamp;
  policyman_ss_info_t     ss_info;
};

struct policyman_cmss_history_s {
  size_t                          index;
  policyman_cmss_history_entry_t  h[PM_DUMP_HISTORY_DEPTH];
};


/*=============================================================================
  APIs for serving system information
=============================================================================*/

/*-------- policyman_ss_info_deep_copy --------*/
policyman_ss_info_t * policyman_ss_info_deep_copy(
  policyman_ss_info_t  *pSsState
)
{
  policyman_ss_info_t *pInfo;

  pInfo = (policyman_ss_info_t *) policyman_mem_alloc(sizeof(policyman_ss_info_t));

  *pInfo = *pSsState;

  return pInfo;
}

/*-------- policyman_ss_get_stack --------*/
STATIC policyman_stack_info_t * policyman_ss_get_stack(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  sys_modem_stack_id_e_type  stackId
)
{
  policyman_ss_info_t *pSsInfo;

  pSsInfo = policyman_state_get_serving_system_info(pState, subsId);
  return &pSsInfo->stack[stackId];
}


/*-------- policyman_ss_set_stack_opr --------*/
void policyman_ss_set_stack_opr(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  sys_modem_stack_id_e_type  stackId,
  boolean                    isOperational
)
{
  policyman_stack_info_t  *pStackInfo;

  pStackInfo = policyman_ss_get_stack(pState, subsId, stackId);

  pStackInfo->is_operational = isOperational;
}

/*-------- policyman_ss_subs_sys_id_is_defined --------*/
boolean policyman_ss_subs_sys_id_is_defined(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  size_t  stackId;
  boolean idValid = FALSE;

  for (stackId = SYS_MODEM_STACK_ID_1; !idValid && stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_stack_info_t *pStackInfo;

    pStackInfo = policyman_ss_get_stack(pState, subsId, stackId);
    idValid    = pStackInfo->is_operational
              && pStackInfo->id_type != SYS_SYS_ID_TYPE_UNDEFINED;

  }

  return idValid;
}

/*-------- policyman_ss_status_is_service --------*/
STATIC boolean policyman_ss_status_is_service(
  sys_srv_status_e_type srv_status
)
{
  boolean retval = FALSE;

  switch(srv_status)
  {
    case SYS_SRV_STATUS_SRV:
    case SYS_SRV_STATUS_LIMITED:
    case SYS_SRV_STATUS_LIMITED_REGIONAL:
      retval = TRUE;
      break;

    case SYS_SRV_STATUS_NONE:
    case SYS_SRV_STATUS_NO_SRV:
    case SYS_SRV_STATUS_PWR_SAVE:
    default:
      break;
  }

  return retval;
}

/*-------- policyman_ss_reset_acq_fail_cnt --------*/
STATIC void policyman_ss_reset_acq_fail_cnt(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  sys_modem_stack_id_e_type  stackId
)
{
  policyman_stack_info_t     *pStack;

  pStack = policyman_ss_get_stack(pState, subsId, stackId);
  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "Acq fail cnt reset on stackId %d",
                        subsId,
                        stackId );
  pStack->acq_fail_cnt = 0;
}

/*-------- policyman_ss_reset_acq_fail_cnt_per_subs --------*/
void policyman_ss_reset_acq_fail_cnt_per_subs(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId
)
{
  size_t stackId;

  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_ss_reset_acq_fail_cnt(pState, subsId, stackId);
  }
}

/*-------- policyman_ss_update_serving_info_validity --------*/
STATIC void policyman_ss_update_serving_info_validity(
  policyman_stack_info_t *pStackInfo
)
{
  /* A status of NO_SRV does not itself affect the PLMN validity. In this
     case the only changes will be the PLMN becoming invalid if the stack
     is not operational or the id_type is undefined.
  */
  if (pStackInfo->srv_status == SYS_SRV_STATUS_NO_SRV)
  {
    pStackInfo->serving_info_valid =   pStackInfo->serving_info_valid
                                    && pStackInfo->id_type != SYS_SYS_ID_TYPE_UNDEFINED
                                    && (pStackInfo->acq_fail_cnt < 1);
  }
  else
  {
    pStackInfo->serving_info_valid =    pStackInfo->id_type    != SYS_SYS_ID_TYPE_UNDEFINED
                                     && pStackInfo->srv_status != SYS_SRV_STATUS_PWR_SAVE;
  }
}

/*-------- policyman_ss_determine_plmn_validity --------*/
STATIC void policyman_ss_determine_plmn_validity(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  size_t               stackId;

  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_stack_info_t *pStackInfo;

    pStackInfo = policyman_ss_get_stack(pState, subsId, stackId);

    if (pStackInfo->is_operational)
    {
      // Reset acq_fail_cnt if service is available.
      if (policyman_ss_status_is_service(pStackInfo->srv_status))
      {
        policyman_ss_reset_acq_fail_cnt(pState, subsId, stackId);
      }
      // Increment acq_fail_cnt if we are in power save
      else if (pStackInfo->srv_status == SYS_SRV_STATUS_PWR_SAVE)
      {
        pStackInfo->acq_fail_cnt++;
      }

      policyman_ss_update_serving_info_validity(pStackInfo);
    }
  }
}

/*-------- policyman_ss_stack_plmn_is_valid --------*/
STATIC boolean policyman_ss_stack_plmn_is_valid(
  sys_modem_stack_id_e_type  stackId,
  policyman_ss_info_t       *pInfo
)
{
  policyman_stack_info_t *pStackInfo;

  pStackInfo = &pInfo->stack[stackId];
  return pStackInfo->serving_info_valid;
}

/*-------- policyman_ss_set_subs_serving_info_validity --------*/
void policyman_ss_set_subs_serving_info_validity(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  boolean                 servingInfoIsValid
)
{
  size_t  stackId;

  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_stack_info_t *pStackInfo;

    pStackInfo = policyman_ss_get_stack(pState, subsId, stackId);

    pStackInfo->serving_info_valid = servingInfoIsValid;
  }
}

/*-------- policyman_ss_update_user_plmn --------*/
void policyman_ss_update_user_plmn(
 policyman_state_t        *pState,
 sys_modem_as_id_e_type    subsId,
 sys_plmn_id_s_type        user_plmn
)
{
  policyman_ss_info_t *pInfo;

  pInfo = policyman_state_get_serving_system_info(pState, subsId);

  if (  sys_plmn_id_is_valid(user_plmn)
     && !mre_plmns_are_equal(&pInfo->user_plmn, &user_plmn)
     )
  {
    POLICYMAN_MSG_PLMN_1(SUBS_PREFIX "Updating user PLMN: ", subsId, user_plmn);
    pInfo->user_plmn     = user_plmn;
  }
}

/*-------- policyman_ss_reset_user_plmn --------*/
void policyman_ss_reset_user_plmn(
  policyman_state_t     *pState,
  sys_modem_as_id_e_type subsId
)
{
  policyman_ss_info_t *pInfo;

  pInfo = policyman_state_get_serving_system_info(pState, subsId);
  policyman_plmn_clear(&pInfo->user_plmn);
  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "User PLMN is reset", pInfo->asubs_id);
}


/*-------- policyman_ss_find_timer --------*/
STATIC policyman_timer_t *policyman_ss_find_timer(
  char const                  *pName,
  policyman_timer_expirefn_t   pfnExpire,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_timer_t *pTimer = NULL;

  pTimer = policyman_timer_get_timer_per_subs(pName, subsId);
  policyman_timer_set_subs(pTimer, subsId);
  policyman_timer_set_expirefn(pTimer, pfnExpire);

  return pTimer;
}

/*-------- policyman_ss_some_stack_is_operational --------*/
STATIC boolean policyman_ss_some_stack_is_operational(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  size_t  stackId;
  boolean isOperational = FALSE;

  for (stackId = SYS_MODEM_STACK_ID_1; !isOperational && stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_stack_info_t *pStackInfo;

    pStackInfo    = policyman_ss_get_stack(pState, subsId, stackId);
    isOperational = pStackInfo->is_operational;
  }

  return isOperational;
}

/*-------- policyman_subs_valid_for_location --------*/
STATIC boolean policyman_subs_valid_for_location(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  return    policyman_ss_some_stack_is_operational(pState, subsId)
         && (policyman_ss_get_min_acq_fail_count(pState, subsId) == 0);
}

/*-------- policyman_subs_has_location --------*/
boolean policyman_subs_has_location(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  size_t  stackId;
  boolean subs_has_location = FALSE;

  for (stackId = SYS_MODEM_STACK_ID_1; !subs_has_location && stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_stack_info_t *pStackInfo;

    pStackInfo        = policyman_ss_get_stack(pState, subsId, stackId);
    subs_has_location =    pStackInfo->is_operational
                        && pStackInfo->serving_info_valid;
  }

  return subs_has_location;
}

/*-------- policyman_find_subs_with_location_info --------*/
sys_modem_as_id_e_type policyman_find_subs_with_location_info(
  policyman_state_t       *pState
)
{
  size_t                 index;
  size_t                 nSim;
  sys_modem_as_id_e_type subsWithLocation = SYS_MODEM_AS_ID_NONE;

  (void)policyman_get_current_num_sim(&nSim);

  /* loop through all SUBS and determine which one has location info */
  for (index = SYS_MODEM_AS_ID_1; index < nSim; index++)
  {
    if (policyman_subs_has_location(pState, index))
    {
      subsWithLocation = (sys_modem_as_id_e_type)index;
      break;
    }
  }

  return subsWithLocation;
}

/*-------- policyman_ss_handle_timer_expired --------*/
STATIC void policyman_ss_handle_timer_expired(
  policyman_timer_t         *pTimer,
  sys_modem_stack_id_e_type  stackId
)
{
  policyman_sandbox_t      *pSandbox;
  policyman_stack_info_t   *pCopiedStackInfo;
  policyman_ss_info_t       copiedSsInfo;
  sys_modem_as_id_e_type    subsId;

  subsId = policyman_timer_get_subs(pTimer);

  pSandbox = policyman_get_global_sandbox();

  copiedSsInfo = *policyman_state_get_serving_system_info(pSandbox->pState, subsId);

  pCopiedStackInfo = &copiedSsInfo.stack[stackId];

  // Timer has expired so mark stack PLMN as invalid for decision making
  pCopiedStackInfo->serving_info_valid = FALSE;

  policyman_ss_update_state(pSandbox, &copiedSsInfo, subsId);
}


/*-------- policyman_ss_handle_1x_oos_expired --------*/
STATIC void policyman_ss_handle_1x_oos_expired(
  policyman_timer_t *pTimer
)
{
  policyman_ss_handle_timer_expired(pTimer, SYS_MODEM_STACK_ID_1);
}

/*-------- policyman_ss_handle_hybrid_oos_expired --------*/
STATIC void policyman_ss_handle_hybrid_oos_expired(
  policyman_timer_t *pTimer
)
{
  policyman_ss_handle_timer_expired(pTimer, SYS_MODEM_STACK_ID_2);
}

/*-------- policyman_ss_get_timer_for_stack --------*/
STATIC policyman_timer_t * policyman_ss_get_timer_for_stack(
  policyman_stack_info_t    *pStack,
  sys_modem_stack_id_e_type  stackId,
  sys_modem_as_id_e_type     subsId
)
{
  policyman_timer_t       *pTimer = NULL;

  if (stackId == SYS_MODEM_STACK_ID_1)
  {
    pTimer = policyman_ss_find_timer("1x_oos", policyman_ss_handle_1x_oos_expired, subsId);
  }
  /* For secondary stack look up for hybr_oos timer
     */
  else if (stackId == SYS_MODEM_STACK_ID_2)
  {
    pTimer = policyman_ss_find_timer("hybr_oos", policyman_ss_handle_hybrid_oos_expired, subsId);
  }

  return pTimer;
}

/*-------- policyman_ss_start_stack_timer --------*/
void policyman_ss_start_stack_timer(
  policyman_stack_info_t     *pStack,
  sys_modem_stack_id_e_type   stackId
)
{
  policyman_timer_t *pTimer;

  pTimer = policyman_ss_get_timer_for_stack( pStack,
                                             stackId,
                                             pStack->subsId );
  if (NULL != pTimer)
  {
    policyman_timer_start(pTimer);
  }
}


/*-------- policyman_ss_stop_stack_timer --------*/
void policyman_ss_stop_stack_timer(
  policyman_stack_info_t     *pStack,
  sys_modem_stack_id_e_type   stackId
)
{
  policyman_timer_t *pTimer;

  pTimer = policyman_ss_get_timer_for_stack( pStack,
                                             stackId,
                                             pStack->subsId );
  if (NULL != pTimer)
  {
    policyman_timer_stop(pTimer);
  }
}


/*-------- policyman_ss_set_default_value --------*/
void policyman_ss_set_default_value(
  policyman_ss_info_t        *pSsInfo,
  sys_modem_stack_id_e_type   stackId
)
{
  policyman_stack_info_t *pStack;

  if (pSsInfo != NULL)
  {
    pStack = &pSsInfo->stack[stackId];

    pStack->subsId               = pSsInfo->asubs_id;
    pStack->stackId              = stackId;
    pStack->sys_mode             = SYS_SYS_MODE_NONE;
    pStack->srv_status           = SYS_SRV_STATUS_NO_SRV;
    pStack->active_band          = SYS_BAND_CLASS_NONE;
    pStack->id_type              = SYS_SYS_ID_TYPE_UNDEFINED;
    pStack->serving_info_valid   = FALSE;
    pStack->acq_fail_cnt         = 0;
    pStack->radio_is_operational = FALSE;

    policyman_ss_stop_stack_timer(pStack, stackId);

    policyman_plmn_clear(&pStack->plmn);
    memset(&pStack->mcc_list_3gpp2, 0x00, sizeof(pStack->mcc_list_3gpp2));
    memset(&pStack->mcc_list_hdr, 0x00, sizeof(pStack->mcc_list_hdr));

    POLICYMAN_MSG_MED_2( SUBS_PREFIX "stack %d does not have valid data",
                         pSsInfo->asubs_id,
                         stackId );
  }
}


/*-------- policyman_ss_init --------*/
boolean policyman_ss_init(
  sys_modem_as_id_e_type   subs,
  policyman_ss_info_t    **ppInfo
)
{
  size_t              i;
  policyman_ss_info_t *pInfo;

  pInfo = (policyman_ss_info_t *) policyman_mem_alloc(sizeof(policyman_ss_info_t));

  for (i = SYS_MODEM_STACK_ID_1; i < SYS_MODEM_STACK_ID_MAX; i++)
  {
    pInfo->asubs_id = subs;
    pInfo->sim_state = SYS_SIM_STATE_NOT_AVAILABLE;
    policyman_ss_set_default_value(pInfo, i);
  }

  policyman_plmn_clear(&pInfo->user_plmn);

  *ppInfo = pInfo;
  return TRUE;
}


/*-------- policyman_ss_deinit --------*/
void policyman_ss_deinit(
  policyman_ss_info_t *pInfo
)
{
  POLICYMAN_MEM_FREE_IF(pInfo);
}


/*-------- policyman_ss_handle_stack_timers --------*/
STATIC void policyman_ss_handle_stack_timers(
  policyman_stack_info_t    *pStack,
  sys_modem_stack_id_e_type  stackId,
  sys_modem_as_id_e_type     subsId
)
{
  sys_ue_mode_e_type  ueMode = SYS_UE_MODE_CSFB;

  (void)policyman_get_current_ue_mode(subsId, &ueMode);

  if (policyman_ss_status_is_service(pStack->srv_status))
  {
    policyman_ss_stop_stack_timer(pStack, stackId);
  }
  else if ((ueMode != SYS_UE_MODE_CSFB) && pStack->is_operational)
  {
    policyman_ss_start_stack_timer(pStack, stackId);
  }
}


/*-------- policyman_ss_handle_subs_timers --------*/
void policyman_ss_handle_subs_timers(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  sys_modem_stack_id_e_type    stackId;

  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_stack_info_t *pStack;

    pStack = policyman_ss_get_stack(pState, subsId, stackId);
    policyman_ss_handle_stack_timers(pStack, (sys_modem_stack_id_e_type)stackId, subsId);
  }
}


/*===========================================================================
  FUNCTION POLICYMAN_SS_GET_MCC()

  DESCRIPTION
    GET MCC for stack from PM STATE
 ===========================================================================*/
uint32 policyman_ss_get_mcc(
  policyman_ss_info_t         *pInfo,
  sys_modem_stack_id_e_type    stackId
)
{
  return (NULL != pInfo) ? policyman_plmn_get_mcc(&pInfo->stack[stackId].plmn) : 0;
}

/*-------- policyman_ss_get_plmn --------*/
void policyman_ss_get_plmn(
  policyman_ss_info_t         *pInfo,
  sys_modem_stack_id_e_type    stackId,
  sys_plmn_id_s_type          *pPlmn
)
{
  policyman_stack_info_t *pStack;

  if (pInfo != NULL && pPlmn != NULL)
  {
    pStack = &pInfo->stack[stackId];

    if (policyman_ss_stack_plmn_is_valid(stackId, pInfo))
    {
      *pPlmn = pStack->plmn;
    }
    else
    {
      POLICYMAN_MSG_ERROR_3( SUBS_PREFIX "stack %d not operational or PLMN invalid %d, returning PLMN 0",
                             pInfo->asubs_id,
                             stackId,
                             pStack->serving_info_valid );

      sys_plmn_set_mcc_mnc(FALSE, 0, 0, pPlmn);
    }
  }
}

/*-------- policyman_ss_get_plmn_id_type --------*/
sys_sys_id_type_e_type policyman_ss_get_plmn_id_type(
  policyman_ss_info_t         *pSsInfo,
  sys_modem_stack_id_e_type    stackId
)
{
  return (NULL != pSsInfo) ? pSsInfo->stack[stackId].id_type : SYS_SYS_ID_TYPE_UNDEFINED;
}

/*-------- policyman_ss_some_stack_has_service --------*/
STATIC boolean policyman_ss_some_stack_has_service(
  policyman_ss_info_t *pCurrentInfo
)
{
  size_t   stack;
  boolean  hasService = FALSE;

  for (stack = SYS_MODEM_STACK_ID_1; !hasService && stack < SYS_MODEM_STACK_ID_MAX; stack++)
  {
    policyman_stack_info_t *pStack;

    pStack = &pCurrentInfo->stack[stack];
    hasService =  pStack->is_operational
               && policyman_ss_status_is_service(pStack->srv_status);
  }

  return hasService;
}

/*-------- policyman_is_new_srv_status_changed --------*/
STATIC boolean policyman_is_new_srv_status_changed(
  sys_srv_status_e_type current_srv_status,
  sys_srv_status_e_type new_srv_status
)
{
  /* The valid changes are:
    * In Service -> OOS, PWR_SAVE, LIMITED REGIONAL
    * OOS -> PWR-SAVE, SRV, LIMITED SRV, LIMITED REGIONAL
    * LIMITED <-> FULL SERVICE
    */

  return new_srv_status != current_srv_status;

}

/*-------- policyman_3gpp2_mccs_are_equal --------*/
STATIC boolean policyman_3gpp2_mccs_are_equal(
  sys_plmn_mcc_list *pCurrentList,
  sys_plmn_mcc_list *pNewList
)
{
  return memcmp(pCurrentList, pNewList, sizeof(sys_plmn_mcc_list)) == 0;
}

/*-------- policyman_ss_sys_mode_is_3gpp --------*/
STATIC boolean policyman_ss_sys_mode_is_3gpp(
  sys_sys_mode_e_type sys_mode
)
{
  return    sys_mode == SYS_SYS_MODE_GSM
         || sys_mode == SYS_SYS_MODE_WCDMA
         || sys_mode == SYS_SYS_MODE_LTE
         || sys_mode == SYS_SYS_MODE_TDS;
}

/*-------- policyman_has_pcs_digit --------*/
STATIC boolean has_pcs_digit(
  sys_mnc_type mnc
)
{
  return (mnc > 99);  
}

/*-------- policyman_ss_get_plmn_from_stack --------*/
STATIC void policyman_ss_get_plmn_from_stack(
  policyman_stack_info_t  *pStack,
  sys_plmn_id_s_type      *pPlmn
)
{
  /* For 3GPP2 system use MCC from MCC list, otherwise use 3GPP PLMN */
  if (policyman_ss_sys_mode_is_3gpp(pStack->sys_mode))
  {
    *pPlmn = pStack->plmn;
  }
  else if (pStack->mcc_list_hdr.count > 0 && pStack->sys_mode == SYS_SYS_MODE_HDR)
  {
    sys_plmn_set_mcc_mnc( has_pcs_digit(pStack->mnc_hdr), 
                          pStack->mcc_list_hdr.mcc_list[0],
                          pStack->mnc_hdr,
                          pPlmn );
  }
  else if (pStack->mcc_list_3gpp2.count > 0 && pStack->sys_mode == SYS_SYS_MODE_CDMA)
  {
    sys_plmn_set_mcc_mnc( has_pcs_digit(pStack->mnc_3gpp2),
                          pStack->mcc_list_3gpp2.mcc_list[0],
                          pStack->mnc_3gpp2,
                          pPlmn );
  }
}

/*-------- policyman_ss_get_plmn_from_subs --------*/
STATIC boolean policyman_ss_get_plmn_from_subs(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  sys_plmn_id_s_type      *pPlmn
)
{
  size_t                  stackId;
  boolean                 plmnValid = FALSE;
  policyman_ss_info_t    *pSsInfo;
  boolean                 stackIs3gpp = FALSE;
  policyman_stack_info_t *pStack      = NULL;

  // Loop through all stacks and get the PLMN if valid. Prefer 3GPP system over 3GPP2.
  pSsInfo = policyman_state_get_serving_system_info(pState, subsId);
  for (stackId = SYS_MODEM_STACK_ID_1; !stackIs3gpp && stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    plmnValid = policyman_ss_stack_plmn_is_valid(stackId, pSsInfo);
    if (plmnValid)
    {
      pStack = policyman_ss_get_stack(pState, subsId, stackId);
      stackIs3gpp = policyman_ss_sys_mode_is_3gpp(pStack->sys_mode);
    }
  }

  if (NULL != pStack)
  {
    policyman_ss_get_plmn_from_stack(pStack, pPlmn);
  }

  return (NULL != pStack);
}


/*-------- policyman_ss_get_3GPP_plmn_from_subs --------*/
boolean policyman_ss_get_3GPP_plmn_from_subs(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  sys_plmn_id_s_type      *pPlmn
)
{
  policyman_ss_info_t    *pSsInfo;
  size_t                  stackId;
  boolean                 plmnIsValid = FALSE;
  boolean                 stackIs3gpp = FALSE;
  policyman_stack_info_t *pStack      = NULL;

  // Loop through all stacks and get the PLMN if valid.
  pSsInfo = policyman_state_get_serving_system_info(pState, subsId);
  for (stackId = SYS_MODEM_STACK_ID_1; !stackIs3gpp && stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    plmnIsValid = policyman_ss_stack_plmn_is_valid(stackId, pSsInfo);
    if (plmnIsValid)
    {
      pStack      = policyman_ss_get_stack(pState, subsId, stackId);
      stackIs3gpp = policyman_ss_sys_mode_is_3gpp(pStack->sys_mode);
    }
  }

  if (stackIs3gpp)
  {
    policyman_ss_get_plmn_from_stack(pStack, pPlmn);
  }

  return stackIs3gpp;
}


/*-------- policyman_ss_plmn_matches_3GPP_plmn_from_subs --------*/
boolean policyman_ss_plmn_matches_3GPP_plmn_from_subs(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  sys_plmn_id_s_type      *pPlmn
)
{
  boolean            plmn_match = FALSE;
  sys_plmn_id_s_type ss_plmn;

  if (policyman_ss_get_3GPP_plmn_from_subs(pState, subsId, &ss_plmn))
  {
    plmn_match = policyman_plmns_are_equal(pPlmn, &ss_plmn);
  }

  return plmn_match;
}


/*-------- policyman_ss_get_mcc_from_subs --------*/
sys_mcc_type policyman_ss_get_mcc_from_subs(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  sys_plmn_id_s_type  plmn;
  boolean             haveMcc;
  sys_mcc_type        mcc     = NO_MCC;

  haveMcc = policyman_ss_get_plmn_from_subs(pState, subsId, &plmn);

  if (haveMcc)
  {
    mcc = policyman_plmn_get_mcc(&plmn);
  }

  return mcc;
}


/*===========================================================================
  FUNCTION POLICYMAN_SS_GET_SRV_STATUS()

  DESCRIPTION
    GET srv_status for stack
 ===========================================================================*/
sys_srv_status_e_type policyman_ss_get_srv_status(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  sys_modem_stack_id_e_type  stackId
)
{
  sys_srv_status_e_type   srv_status = SYS_SRV_STATUS_NONE;
  policyman_stack_info_t *pStack;

  pStack = policyman_ss_get_stack(pState, subsId, stackId);

  if (pStack->is_operational)
  {
    srv_status = pStack->srv_status;
  }

  return srv_status;
}

/*===========================================================================
  FUNCTION POLICYMAN_SS_SET_SRV_STATUS()

  DESCRIPTION
    SET srv_status for stack into PolicyMan state

 RETURN VALUE
   TRUE is policyman state is updated correctly, FALSE otherwise
 ===========================================================================*/
boolean policyman_ss_set_srv_status(
  policyman_ss_info_t       *pInfo,
  sys_modem_stack_id_e_type  stackId,
  sys_srv_status_e_type      srv_status
)
{
  boolean changed = FALSE;

  if ( pInfo != NULL &&
       (pInfo->stack[stackId].srv_status != srv_status)
     )
  {
    pInfo->stack[stackId].srv_status = srv_status;
    changed =  pInfo->stack[stackId].is_operational = TRUE;
  }

  return changed;
}

/*-------- policyman_ss_subs_is_in_service --------*/
boolean policyman_ss_subs_is_in_service(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_stack_info_t  *pStack1 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_1);
  policyman_stack_info_t  *pStack2 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_2);

  /* don't check stack operational as it won't have service if inactive */
  return    policyman_ss_status_is_service(pStack1->srv_status)
         || policyman_ss_status_is_service(pStack2->srv_status);
}

/*-------- policyman_ss_subs_is_in_pwr_save --------*/
boolean policyman_ss_subs_is_in_pwr_save(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  boolean                  isPwrSave   = TRUE;
  boolean                  subsHasOprStack  = FALSE;
  size_t                   stackID;
  policyman_stack_info_t  *pStack;

  for (stackID = SYS_MODEM_STACK_ID_1; isPwrSave && stackID < SYS_MODEM_STACK_ID_MAX; stackID++)
  {
    pStack = policyman_ss_get_stack(pState, subsId, stackID);
    subsHasOprStack |=  pStack->is_operational;

    if (pStack->is_operational)
    {
      isPwrSave = (pStack->srv_status == SYS_SRV_STATUS_PWR_SAVE);
    }
  }

  return isPwrSave && subsHasOprStack;
}

/*===========================================================================
  FUNCTION POLICYMAN_SS_GET_ACTIVE_BAND()

  DESCRIPTION
    GET active band for stack
 ===========================================================================*/
sys_band_class_e_type policyman_ss_get_active_band(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  sys_modem_stack_id_e_type  stackId
)
{
  sys_band_class_e_type   active_band = SYS_BAND_CLASS_NONE;
  policyman_stack_info_t *pStack;

  pStack = policyman_ss_get_stack(pState, subsId, stackId);

  if (pStack->is_operational)
  {
    active_band = pStack->active_band;
  }

  return active_band;
}

/*===========================================================================
  FUNCTION POLICYMAN_SS_GET_REJECT_CAUSE()

  DESCRIPTION
    GET registration reject cause for MAIN stack
 ===========================================================================*/
byte policyman_ss_get_reject_cause(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  sys_modem_stack_id_e_type  stackId
)
{
  byte                    reject_cause = 0;
  policyman_stack_info_t *pStack;

  pStack = policyman_ss_get_stack(pState, subsId, stackId);

  if (pStack->is_operational)
  {
    reject_cause = pStack->reject_cause;
  }

  return reject_cause;
}

/*===========================================================================
  FUNCTION POLICYMAN_SERVING_DOMAIN_CAMPED()

  DESCRIPTION
    Check CM serving domain is CAMPED ONLY
===========================================================================*/
boolean policyman_serving_domain_camped(
  sys_srv_domain_e_type srv_domain
)
{
  return srv_domain == SYS_SRV_DOMAIN_CAMPED;
}

/*===========================================================================
  FUNCTION POLICYMAN_SRV_STATUS_CHECK_NO_SRV()

  DESCRIPTION
    Check if Service Status is No service
===========================================================================*/
boolean policyman_srv_status_check_no_srv(
  sys_srv_status_e_type srv_status
)
{
  return    srv_status == SYS_SRV_STATUS_NO_SRV
         || srv_status == SYS_SRV_STATUS_NONE;
}

/*===========================================================================
  FUNCTION policyman_copy_mcc_list()

  DESCRIPTION
    Copy MCCs to a list
===========================================================================*/
STATIC void policyman_copy_mcc_list(
  uint32 *pDst,
  uint32 *pSrc,
  uint8   count
)
{
  while (count-- > 0)
  {
    *pDst++ = *pSrc++;
  }
}


#define MCC_LIST_ELEM(list_count, mcc_list, idx) ((idx) < (list_count) ? (mcc_list)[(idx)] : 0)

/*-------- policyman_ss_3gpp2_plmn_print --------*/
STATIC void policyman_ss_3gpp2_plmn_print(
  policyman_stack_info_t const *pStack,
  sys_plmn_mcc_list      const *pMccList,
  sys_mnc_type                  mnc,
  sys_sys_mode_e_type           sysMode
)
{
  POLICYMAN_MSG_HIGH_9( SUBS_PREFIX "MCC list (type %d 2=CDMA 4=HDR) for stack %d, mnc %d (4 of %d entries): %d %d %d %d",
                        pStack->subsId,
                        sysMode,
                        pStack->stackId,
                        mnc,
                        pMccList->count,
                        MCC_LIST_ELEM(pMccList->count, pMccList->mcc_list, 0),
                        MCC_LIST_ELEM(pMccList->count, pMccList->mcc_list, 1),
                        MCC_LIST_ELEM(pMccList->count, pMccList->mcc_list, 2),
                        MCC_LIST_ELEM(pMccList->count, pMccList->mcc_list, 3) );
}

/*===========================================================================
  FUNCTION policyman_read_hdr_mcc_list()

  DESCRIPTION
    Get the list of MCCs for HDR
===========================================================================*/
STATIC void policyman_read_hdr_mcc_list(
  policyman_msim_cmss_evt_cb_t        *cm_ss_evt,
  policyman_stack_info_t              *pStack,
  cm_mm_msim_ss_stack_info_s_type     *pcmStack
)
{
  uint8  cmHdrCount = cm_ss_evt->info.prl_hdr_mcc.count;

  if (cmHdrCount < 1)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "CM sent empty HDR MCC list, don't wipe the previous info", cm_ss_evt->info.asubs_id);
    return;
  }

  if (cmHdrCount > SD_PLMN_MCC_LIST_MAX)
  {
    POLICYMAN_ERR_FATAL("CM sent incorrect HDR MCC list with count %d", cmHdrCount, 0, 0);
  }

  policyman_plmn_clear(&pStack->plmn);

  pStack->id_type            = pcmStack->sys_id.id_type;
  pStack->mcc_list_hdr.count = cmHdrCount;
  pStack->mnc_hdr            = (sys_mnc_type)pcmStack->sys_id.id.is95.imsi_11_12;

  policyman_copy_mcc_list( pStack->mcc_list_hdr.mcc_list,
                           cm_ss_evt->info.prl_hdr_mcc.mcc_list,
                           pStack->mcc_list_hdr.count );


  policyman_ss_3gpp2_plmn_print( pStack,
                                 &pStack->mcc_list_hdr,
                                 pStack->mnc_hdr,
                                 SYS_SYS_MODE_HDR );
}


/*===========================================================================
  FUNCTION policyman_read_1x_mcc_list()

  DESCRIPTION
    Get list of MCCs for 1x
===========================================================================*/
STATIC void policyman_read_1x_mcc_list(
  policyman_msim_cmss_evt_cb_t        *cm_ss_evt,
  policyman_stack_info_t              *pStack,
  cm_mm_msim_ss_stack_info_s_type     *pcmStack
)
{
  uint8 cm3GPP2Count = cm_ss_evt->info.prl_3gpp2_mcc.count;

  if (cm3GPP2Count < 1)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "CM sent empty 3GPP2 MCC list, don't wipe the previous info", cm_ss_evt->info.asubs_id);
    return;
  }

  if (cm3GPP2Count > SD_PLMN_MCC_LIST_MAX)
  {
    POLICYMAN_ERR_FATAL("CM sent incorrect 3GPP2 MCC list with count %d", cm3GPP2Count, 0, 0);
  }

  policyman_plmn_clear(&pStack->plmn);

  pStack->id_type              = pcmStack->sys_id.id_type;
  pStack->mcc_list_3gpp2.count = cm3GPP2Count;
  pStack->mnc_3gpp2            = (sys_mnc_type)pcmStack->sys_id.id.is95.imsi_11_12;

  policyman_copy_mcc_list( pStack->mcc_list_3gpp2.mcc_list,
                           cm_ss_evt->info.prl_3gpp2_mcc.mcc_list,
                           pStack->mcc_list_3gpp2.count );

  policyman_ss_3gpp2_plmn_print( pStack,
                                 &pStack->mcc_list_3gpp2,
                                 pStack->mnc_3gpp2,
                                 SYS_SYS_MODE_CDMA );
}

/*===========================================================================
  FUNCTION POLICYMAN_GET_3GPP2_PLMN_FROM_LIST()

  DESCRIPTION
    Get list of MCCs for 3GPP2
===========================================================================*/
STATIC void policyman_get_3gpp2_plmn_from_list(
  policyman_msim_cmss_evt_cb_t        *cm_ss_evt,
  policyman_stack_info_t              *pStack,
  cm_mm_msim_ss_stack_info_s_type     *pcmStack
)
{
  if (pcmStack->sys_mode == SYS_SYS_MODE_HDR)
  {
    policyman_read_hdr_mcc_list(cm_ss_evt, pStack, pcmStack);
  }
  else if(pcmStack->sys_mode == SYS_SYS_MODE_CDMA)
  {
    policyman_read_1x_mcc_list(cm_ss_evt, pStack, pcmStack);
  }
}

/*-------- policyman_extract_sys_id --------*/
STATIC void policyman_extract_sys_id(
  policyman_msim_cmss_evt_cb_t        *cm_ss_evt,
  policyman_stack_info_t              *pStack,
  cm_mm_msim_ss_stack_info_s_type     *pcmStack
)
{

  switch (pcmStack->sys_id.id_type)
  {
    case SYS_SYS_ID_TYPE_UMTS:

      /* Ignore Main stack 3GPP PLMN if srv_domain is CAMPED_ONLY
               */
      if (!policyman_serving_domain_camped(pcmStack->srv_domain))
      {
        pStack->id_type  = pcmStack->sys_id.id_type;
        pStack->plmn     = pcmStack->sys_id.id.plmn;

        POLICYMAN_MSG_PLMN_2( SUBS_PREFIX "stack %d PLMN:",
                              cm_ss_evt->info.asubs_id,
                              pStack->stackId,
                              pStack->plmn );
      }
      break;

    case SYS_SYS_ID_TYPE_IS856:
    case SYS_SYS_ID_TYPE_IS95:
      policyman_get_3gpp2_plmn_from_list(cm_ss_evt, pStack, pcmStack);
      break;

    default:
      POLICYMAN_MSG_ERROR_2( SUBS_PREFIX "stack %d PLMN ignored",
                             cm_ss_evt->info.asubs_id,
                             pStack->stackId );
      break;
  }
}

/*-------- policyman_determine_current_mcc_for_subs --------*/
STATIC sys_mcc_type policyman_determine_current_mcc_for_subs(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId
)
{
  boolean                 haveLocationFromService;
  boolean                 haveLocationCurrentSub;
  boolean                 currentSubIsValidForLocation;
  sys_mcc_type            mcc            = NO_MCC;
  sys_modem_as_id_e_type  locationSubsId = subsId;

  /*  See if we have a location based on service.  Give preference to the
   *  current subscription's MCC if it has one; otherwise find a subscription
   *  that *does* have an MCC.
   */
  haveLocationCurrentSub       = policyman_subs_has_location(pState, locationSubsId);
  currentSubIsValidForLocation = policyman_subs_valid_for_location(pState, locationSubsId);

  if (!haveLocationCurrentSub && !currentSubIsValidForLocation)
  {
    locationSubsId = policyman_find_subs_with_location_info(pState);
  }

  haveLocationFromService = (locationSubsId != SYS_MODEM_AS_ID_NONE);

  if (haveLocationFromService)
  {
    mcc = policyman_ss_get_mcc_from_subs(pState, locationSubsId);

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "Obtained MCC %d based on network service on subs %d",
                          subsId,
                          mcc,
                          locationSubsId);

    // clear HLOS MCC since location is available from service.
    policyman_state_clear_hlos_mcc(pState);
  }
  else
  {
    // We don't have service, so use the HLOS MCC if there is one.
    mcc = policyman_state_get_hlos_mcc(pState);
    if (mcc != NO_MCC)
    {
      POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "NO MCC from service, obtained MCC %d based on HLOS value",
                            subsId,
                            mcc);
    }
  }

  return mcc;
}


/*-------- policyman_ss_update_current_mcc --------*/
STATIC void policyman_ss_update_current_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  sys_mcc_type  mcc;

  mcc = policyman_determine_current_mcc_for_subs(pState, subsId);
  policyman_state_update_subs_mcc(pState, subsId, mcc);
}

/*-------- policyman_ss_update_current_mcc_on_all_subs --------*/
STATIC void policyman_ss_update_current_mcc_on_all_subs(
  policyman_state_t *pState
)
{
  sys_modem_as_id_e_type  subsId;
  size_t                  nSim;

  (void)policyman_get_current_num_sim(&nSim);

  for (subsId = SYS_MODEM_AS_ID_1; subsId < nSim; ++subsId)
  {
    policyman_ss_update_current_mcc(pState, subsId);
  }
}

/*-------- policyman_ss_print_stack_info --------*/
STATIC void policyman_ss_print_stack_info(
  policyman_state_t const     *pState,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_stack_info_t const  *pStack;
  size_t                         stackId;

  /*  Print out stack information
   */
  for (stackId = 0 ; stackId < SYS_MODEM_STACK_ID_MAX; ++stackId)
  {
    pStack = policyman_ss_get_stack((policyman_state_t *)pState, subsId, stackId);
    POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "Stack info: stack %d, srv_status %d, sys_mode %d, plmn_valid %d",
                          subsId,
                          stackId,
                          pStack->srv_status,
                          pStack->sys_mode,
                          pStack->serving_info_valid );

    /* For Non-CDMA, PLMN print is fine
           For CDMA it should come from MCC list
      */
    if (pStack->is_operational && pStack->serving_info_valid)
    {
      if (pStack->sys_mode == SYS_SYS_MODE_CDMA)
      {
        policyman_ss_3gpp2_plmn_print( pStack,
                                       &pStack->mcc_list_3gpp2,
                                       pStack->mnc_3gpp2,
                                       SYS_SYS_MODE_CDMA );
      }
      else if (pStack->sys_mode == SYS_SYS_MODE_HDR)
      {
        policyman_ss_3gpp2_plmn_print( pStack,
                                       &pStack->mcc_list_hdr,
                                       pStack->mnc_hdr,
                                       SYS_SYS_MODE_HDR );
      }
      else
      {
        POLICYMAN_MSG_PLMN_0("", pStack->plmn);
      }
    }
  }
}

/*-------- policyman_ss_stack_has_changed --------*/
STATIC boolean policyman_ss_stack_has_changed(
  policyman_stack_info_t *pCurStack,
  policyman_stack_info_t *pNewStack
)
{
  return    pCurStack->is_operational       != pNewStack->is_operational
         || pCurStack->id_type              != pNewStack->id_type
         || pCurStack->serving_info_valid   != pNewStack->serving_info_valid
         || pCurStack->srv_domain           != pNewStack->srv_domain
         || pCurStack->acq_fail_cnt         != pNewStack->acq_fail_cnt
         || pCurStack->radio_is_operational != pNewStack->radio_is_operational
         || !policyman_plmns_are_equal(&pCurStack->plmn, &pNewStack->plmn)
         || !policyman_3gpp2_mccs_are_equal(&pCurStack->mcc_list_3gpp2, &pNewStack->mcc_list_3gpp2)
         || !policyman_3gpp2_mccs_are_equal(&pCurStack->mcc_list_hdr, &pNewStack->mcc_list_hdr)
         || policyman_is_new_srv_status_changed(pCurStack->srv_status, pNewStack->srv_status)
         || pNewStack->pending_reject > 0
         || pNewStack->reject_cause   > 0;
}


/*-------- policyman_ss_subs_state_has_changed --------*/
boolean policyman_ss_subs_state_has_changed(
  policyman_state_t       *pState,
  policyman_ss_info_t     *pNewInfo,
  sys_modem_as_id_e_type   asubs_id,
  boolean                 *stackHasChanged,
  size_t                   stackHasChangedLen
)
{
  boolean  subsStateHasChanged = FALSE;
  size_t   stackId;

  for (stackId = SYS_MODEM_STACK_ID_1; stackId < stackHasChangedLen; stackId++)
  {
    policyman_stack_info_t *pCurStack;
    policyman_stack_info_t *pNewStack;

    pCurStack = policyman_ss_get_stack(pState, asubs_id, stackId);
    pNewStack = &pNewInfo->stack[stackId];

    stackHasChanged[stackId] = policyman_ss_stack_has_changed(pCurStack, pNewStack);
    subsStateHasChanged |= stackHasChanged[stackId];
  }

  subsStateHasChanged |= policyman_uim_get_sim_refresh(pState, asubs_id);

  return subsStateHasChanged;
}

/*-------- policyman_ss_update_state --------*/
void policyman_ss_update_state(
  policyman_sandbox_t     *pSandbox,
  policyman_ss_info_t     *pNewInfo,
  sys_modem_as_id_e_type   asubs_id
)
{
  size_t                stackId;
  boolean               stackHasChanged[SYS_MODEM_STACK_ID_MAX];
  boolean               subsStateHasChanged;
  boolean               haveService;
  policyman_ss_info_t  *pCurrentInfo;
  policyman_state_t    *pState;

  pState = pSandbox->pState;

  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Updating SS state", asubs_id);

  subsStateHasChanged = policyman_ss_subs_state_has_changed( pState,
                                                             pNewInfo,
                                                             asubs_id,
                                                             stackHasChanged,
                                                             ARR_SIZE(stackHasChanged) );

  policyman_state_enter_crit_section();

  // Update the state
  pCurrentInfo  = policyman_state_get_serving_system_info(pState, asubs_id);
  *pCurrentInfo = *pNewInfo;

  /* Validating PLMN and updating current MCC on all SUBs */
  policyman_ss_determine_plmn_validity(pState, asubs_id);
  policyman_ss_update_current_mcc_on_all_subs(pState);

  policyman_state_leave_crit_section();

  /*  Let the full-RAT object know about whether we have service so it can
   *  stop/start timers.
   */
  policyman_fullrat_config_notify_service(pState, asubs_id);

  if (subsStateHasChanged)
  {
    policyman_ss_handle_subs_timers(pState, asubs_id);

    policyman_ss_print_stack_info(pState, asubs_id);

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "Information changed on stacks: main %d, gw_hybr %d, handle update",
                          asubs_id,
                          stackHasChanged[SYS_MODEM_STACK_ID_1],
                          stackHasChanged[SYS_MODEM_STACK_ID_2] );

    policyman_state_handle_update(pSandbox);
  }
  else
  {
    POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "information not changed on any stack", asubs_id);
  }
}

STATIC boolean policyman_has_cmSSfield_changed(
  policyman_msim_cmss_evt_cb_t        *cm_ss_evt,
  size_t                               stackId
)
{
  cm_mm_msim_ss_info_s_type          *pcmSSInfo;
  cm_mm_msim_ss_stack_info_s_type    *pcmStack;

  pcmSSInfo = &cm_ss_evt->info;
  pcmStack = &pcmSSInfo->stack_info[stackId];

  return ((pcmStack->changed_fields & CM_SS_EVT_SYS_ID_MASK) ||
          (pcmSSInfo->changed_fields_subs & CM_SS_EVT_PRL_3GPP2_MCC_MASK) ||
          (pcmSSInfo->changed_fields_subs & CM_SS_EVT_PRL_HDR_MCC_MASK) ||
          (pcmStack->changed_fields & CM_SS_EVT_SRV_STATUS_MASK) ||
          (pcmStack->changed_fields & CM_SS_EVT_SYS_MODE_MASK) ||
          (pcmStack->changed_fields & CM_SS_EVT_SRV_DOMAIN_MASK) ||
          (pcmStack->changed_fields & CM_SS_EVT_ACTIVE_BAND_MASK) ||
          (pcmStack->changed_fields & CM_SS_EVT_LAC_TAC_RAC_CHGD_MASK));

}
/*===========================================================================
  FUNCTION POLICYMAN_MSIM_GET_STACK_INFO()

  DESCRIPTION
    Get Main stack info
  ===========================================================================*/
void policyman_msim_get_stack_info(
  policyman_ss_info_t                 *cm_ss_info,
  policyman_msim_cmss_evt_cb_t        *cm_ss_evt
)
{
  size_t                       stackId;
  sys_modem_as_id_e_type       subsId;
  cm_mm_msim_ss_info_s_type   *pcmSSInfo;

  subsId = cm_ss_info->asubs_id;
  pcmSSInfo = &cm_ss_evt->info;

  POLICYMAN_MSG_HIGH_4( SUBS_PREFIX "CM stack update: number_of_stacks %d, oper_status(stack0 %d, stack1 %d)",
                       subsId,
                       pcmSSInfo->number_of_stacks,
                       pcmSSInfo->stack_info[0].is_operational,
                       pcmSSInfo->stack_info[1].is_operational );

  for ( stackId = 0; stackId < CM_NO_STACKS; stackId++ )
  {
    policyman_stack_info_t          *pStack;
    cm_mm_msim_ss_stack_info_s_type *pcmStack;

    /* get current state info locally
      */
    pStack   = &cm_ss_info->stack[stackId];
    pcmStack = &pcmSSInfo->stack_info[stackId];

    if (policyman_has_cmSSfield_changed(cm_ss_evt, stackId))
    {
      POLICYMAN_MSG_HIGH_8( SUBS_PREFIX "stack %d  srv_status %d sys_mode %d srv_domain %d active_band %d system id type %d and service domain %d",
                              subsId,
                              stackId,
                              pcmStack->srv_status,
                              pStack->sys_mode,
                              pcmStack->srv_domain,
                              pcmStack->active_band,
                              pcmStack->sys_id.id_type,
                              pcmStack->srv_domain);
    }

    pStack->is_operational = pcmStack->is_operational;

    /* mark stack as valid
      */
    if (!pcmStack->is_operational)
    {
      policyman_ss_set_default_value(cm_ss_info, stackId);
      continue;
    }

    pStack->radio_is_operational = TRUE;

    // get stack info based on corresponding mask
    if (   (pcmStack->changed_fields & CM_SS_EVT_SYS_ID_MASK)
         ||(pcmSSInfo->changed_fields_subs & CM_SS_EVT_PRL_3GPP2_MCC_MASK)
         ||(pcmSSInfo->changed_fields_subs & CM_SS_EVT_PRL_HDR_MCC_MASK)
       )
    {
      policyman_extract_sys_id( cm_ss_evt, 
                                pStack, 
                                pcmStack );
    }

    if (pcmStack->changed_fields & CM_SS_EVT_SRV_STATUS_MASK)
    {
      pStack->srv_status = pcmStack->srv_status;
    }

    /* Ignore sys mode change if associated service status is NO_SRV/PWR_SAVE
       */
    if (  (pcmStack->changed_fields & CM_SS_EVT_SYS_MODE_MASK)
        && policyman_ss_status_is_service(pcmStack->srv_status)
       )
    {
      pStack->sys_mode = pcmStack->sys_mode;
    }

    if (pcmStack->changed_fields & CM_SS_EVT_SRV_DOMAIN_MASK)
    {
      pStack->srv_domain = pcmStack->srv_domain;
    }

    if (pcmStack->changed_fields & CM_SS_EVT_ACTIVE_BAND_MASK)
    {
      pStack->active_band = pcmStack->active_band;
    }

    /* update reject cause if we have pending reject, reset pending reject
     */
    pStack->reject_cause = pStack->pending_reject;
    pStack->pending_reject = 0;

  }
}

/*===========================================================================
  FUNCTION POLICYMAN_MSIM_GET_SS_INFO()

  DESCRIPTION
    GET changed CM SS Info from event into local policyman_ss_info_t structure

  PARAMETERS
    ss_info : Policyman local structure to retain changed SS info
    ss_evt : Incoming CM SS EVENT for Multisim
  ===========================================================================*/
void policyman_msim_get_ss_info(
  policyman_ss_info_t                 *pSsInfo,
  policyman_msim_cmss_evt_cb_t        *pSsEvt
)
{
  if (pSsInfo != NULL && pSsEvt != NULL)
  {
    sys_modem_as_id_e_type subsId = pSsEvt->info.asubs_id;

    /* restore the old PM state info */
    *pSsInfo = *policyman_state_get_serving_system_info(pSsEvt->pSandbox->pState, subsId);

    /* get stack info */
    policyman_msim_get_stack_info(pSsInfo, pSsEvt);
  }
  else
  {
    POLICYMAN_MSG_ERROR_2( "msim_get_ss_info NULL pointer, data not copied - cm ss info %x, cm ss evt %x",
                           pSsInfo,
                           pSsEvt );
  }
}

/*===========================================================================
  FUNCTION POLICYMAN_MSIM_SS_UPDATE_REJECT_INFO()

  DESCRIPTION
    Update for reject info into PM serving system state for Multisim
===========================================================================*/
void policyman_msim_ss_update_reject_info(
  policyman_msim_cmss_evt_cb_t        *ss_evt
)
{
  sys_modem_stack_id_e_type   stackId;
  policyman_ss_info_t        *cm_ss_info;

  cm_ss_info = policyman_state_get_serving_system_info(ss_evt->pSandbox->pState, ss_evt->info.asubs_id);

  if (NULL != cm_ss_info)
  {
    /* update reject cause in state
      */
    for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; ++stackId)
    {
      if (ss_evt->info.stack_info[stackId].is_operational)
      {
        cm_ss_info->stack[stackId].pending_reject = ss_evt->info.stack_info[stackId].reg_reject_info.reject_cause;

        POLICYMAN_MSG_HIGH_4(SUBS_PREFIX "stack %d - reject_cause %d, reject_domain %d",
                              cm_ss_info->asubs_id,
                              stackId,
                              ss_evt->info.stack_info[stackId].reg_reject_info.reject_cause,
                              ss_evt->info.stack_info[stackId].reg_reject_info.reject_srv_domain );
      }
    }
  }
}


/*===========================================================================
  FUNCTION POLICYMAN_EXECUTE_MSIM_PROCESS_CM_REJECT_EVT()

  DESCRIPTION
    Execute CM SS Set update for reject info for Multisim
 ===========================================================================*/
void policyman_execute_msim_process_cm_reject_evt(
  policyman_cmd_t *pCmd
)
{
  policyman_msim_cmss_evt_cb_t  *ss_evt = (policyman_msim_cmss_evt_cb_t *) pCmd;

  if (ss_evt)
  {
    /* only update the state, don't run the rules now
       next SS event will be LIMITED service so run rules there
    */
    policyman_msim_ss_update_reject_info(ss_evt);
  }
}

/*-------- policyman_cmss_init_history --------*/
void policyman_cmss_init_history(
  policyman_state_t  *pState
)
{
  if (NULL == pState->pCmssHistory)
  {
    int   idx;

    pState->pCmssHistory = (policyman_cmss_history_t *)policyman_mem_alloc(sizeof(policyman_cmss_history_t));

    for (idx = 0; idx < (PM_DUMP_HISTORY_DEPTH); ++idx)
    {
      pState->pCmssHistory->h[idx].next      = idx + 1;
      pState->pCmssHistory->h[idx].timestamp = 0xC0FFEE;
    }

    pState->pCmssHistory->h[PM_DUMP_HISTORY_DEPTH - 1].next = 0;
  }
}

/*-------- policyman_cmss_deinit_history --------*/
void policyman_cmss_deinit_history(
  policyman_state_t  *pState
)
{
  POLICYMAN_MEM_FREE_IF(pState->pCmssHistory);
}

/*-------- policyman_cmss_update_history --------*/
void policyman_cmss_update_history(
  policyman_state_t      *pState,
  policyman_ss_info_t    *ss_info,
  sys_modem_as_id_e_type  subs_id,
  uint32                  timestamp
)
{
  if (NULL != pState->pCmssHistory)
  {
    pState->pCmssHistory->h[pState->pCmssHistory->index].timestamp  = timestamp;
    pState->pCmssHistory->h[pState->pCmssHistory->index].ss_info    = *ss_info;

    pState->pCmssHistory->index = pState->pCmssHistory->h[pState->pCmssHistory->index].next;
  }
}

/*-------- policyman_cmss_info_to_dump_stack --------*/
STATIC void policyman_cmss_info_to_dump_stack(
  policyman_stack_info_t    *src_cm_stack,
  pm_dump_cm_stack_info_t   *dump_cm_stack
)
{
  int  idx;

  dump_cm_stack->is_operational       = src_cm_stack->is_operational;
  dump_cm_stack->stackId              = src_cm_stack->stackId;
  dump_cm_stack->pending_reject       = src_cm_stack->pending_reject;
  dump_cm_stack->reject_cause         = src_cm_stack->reject_cause;
  dump_cm_stack->id_type              = src_cm_stack->id_type;
  dump_cm_stack->serving_info_valid   = src_cm_stack->serving_info_valid;
  dump_cm_stack->plmn.identity[0]     = src_cm_stack->plmn.identity[0];
  dump_cm_stack->plmn.identity[1]     = src_cm_stack->plmn.identity[1];
  dump_cm_stack->plmn.identity[2]     = src_cm_stack->plmn.identity[2];
  dump_cm_stack->mcc_list_3gpp2.count = src_cm_stack->mcc_list_3gpp2.count;

  for (idx = 0; idx < ARR_SIZE(src_cm_stack->mcc_list_3gpp2.mcc_list); idx++)
  {
    dump_cm_stack->mcc_list_3gpp2.mcc_list[idx] = src_cm_stack->mcc_list_3gpp2.mcc_list[idx];
  }

  dump_cm_stack->mcc_list_hdr.count = src_cm_stack->mcc_list_hdr.count;

  for (idx = 0; idx < ARR_SIZE(src_cm_stack->mcc_list_hdr.mcc_list); idx++)
  {
    dump_cm_stack->mcc_list_hdr.mcc_list[idx] = src_cm_stack->mcc_list_hdr.mcc_list[idx];
  }

  dump_cm_stack->mnc_3gpp2            = src_cm_stack->mnc_3gpp2;
  dump_cm_stack->mnc_hdr              = src_cm_stack->mnc_hdr;
  dump_cm_stack->sys_mode             = src_cm_stack->sys_mode;
  dump_cm_stack->srv_status           = src_cm_stack->srv_status;
  dump_cm_stack->active_band          = src_cm_stack->active_band;
  dump_cm_stack->srv_domain           = src_cm_stack->srv_domain;
  dump_cm_stack->acq_fail_cnt         = src_cm_stack->acq_fail_cnt;
  dump_cm_stack->radio_is_operational = src_cm_stack->radio_is_operational;

}

/*-------- policyman_cmss_fill_diag_ss_info --------*/
void policyman_cmss_fill_diag_ss_info(
  policyman_ss_info_t     *subs_ss_info,
  pm_dump_cm_ss_info_t    *dump_ss_info
)
{
  dump_ss_info->asubs_id = subs_ss_info->asubs_id;

  policyman_cmss_info_to_dump_stack( &subs_ss_info->stack[0],
                                     &dump_ss_info->stack[0] );

  policyman_cmss_info_to_dump_stack( &subs_ss_info->stack[1],
                                     &dump_ss_info->stack[1] );
}

/*-------- policyman_cmss_history_entry_to_dump --------*/
STATIC void policyman_cmss_history_entry_to_dump(
  policyman_cmss_history_entry_t *hist_entry,
  pm_dump_cm_ss_info_history_t   *dump_entry
)
{
  dump_entry->timestamp = hist_entry->timestamp;

  policyman_cmss_fill_diag_ss_info( &hist_entry->ss_info,
                                    &dump_entry->cm_ss_info );
}

/*-------- policyman_cmss_get_history --------*/
size_t policyman_cmss_get_history(
  pm_dump_cm_ss_info_history_t *h_buffer,
  size_t                        h_len
)
{
  policyman_state_t      *pState;
  int                     oldest_hist_idx;
  int                     start_idx;
  int                     hist_idx;
  int                     offset;

  pState = policyman_state_get_state();

  // If given length larger than history, get only max history available
  if (h_len > PM_DUMP_HISTORY_DEPTH)
  {
    h_len = PM_DUMP_HISTORY_DEPTH;
  }

  oldest_hist_idx = pState->pCmssHistory->index;  // points to oldest history record

  // Calculate the index to start with for the most recent h_len history entries
  start_idx = (oldest_hist_idx + (PM_DUMP_HISTORY_DEPTH - h_len)) % PM_DUMP_HISTORY_DEPTH;

  hist_idx = start_idx;

  for (offset = 0; offset < h_len; offset++ )
  {
    policyman_cmss_history_entry_to_dump( &pState->pCmssHistory->h[hist_idx],
                                          &h_buffer[offset] );

    hist_idx = pState->pCmssHistory->h[hist_idx].next;
  }

  return h_len;
}


/*-------- policyman_cmss_create_and_send_diag_ss_log --------*/
void policyman_cmss_create_and_send_diag_ss_log(
  policyman_ss_info_t *subs_ss_info
)
{
  LOG_PM_SS_HISTORY_INFO_type *log_ptr;

  log_ptr = LOG_PACKET_NEW( LOG_PM_SS_HISTORY_INFO_C,
                            LOG_PM_SS_HISTORY_INFO_type);

  if (NULL != log_ptr)
  {
    log_ptr->version      = LOG_PM_SS_HISTORY_INFO_LOG_PACKET_VERSION;

    policyman_cmss_fill_diag_ss_info(subs_ss_info, &log_ptr->cm_ss_info);

    log_commit( (void*)log_ptr );
  }
}


/*===========================================================================
  FUNCTION POLICYMAN_EXECUTE_MSIM_PROCESS_CMSS_EVT()

  DESCRIPTION
    Execute CM SS Set update and policy check for Multisim
 ===========================================================================*/
void policyman_execute_msim_process_cmss_evt(
  policyman_cmd_t *pCmd
)
{
  policyman_msim_cmss_evt_cb_t *pSsEvt   = (policyman_msim_cmss_evt_cb_t *) pCmd;
  policyman_sandbox_t          *pSandbox = pCmd->pSandbox;
  policyman_state_t            *pState   = pSandbox->pState;
  policyman_ss_info_t ss_info;

  if (NULL != pSsEvt)
  {
    policyman_msim_get_ss_info(&ss_info, pSsEvt);

    policyman_ss_update_state(pSandbox, &ss_info, pSsEvt->info.asubs_id);

    policyman_cmss_update_history( pState,
                                   &ss_info,
                                   pSsEvt->info.asubs_id,
                                   pSsEvt->timestamp );

    policyman_cmss_create_and_send_diag_ss_log(&ss_info);
  }
}

/*===========================================================================
  FUNCTION POLICYMAN_MSIM_CMSS_EVENT_CB()

  DESCRIPTION
    Handle CM SS event callback function for Multisim

  PARAMETERS
    evt     : CM SS EVENT type
    p_info : actual payload of CM SS EVENT
===========================================================================*/
void policyman_msim_cmss_event_cb(
  cm_ss_event_e_type               evt,
  const cm_mm_msim_ss_info_s_type *p_info
)
{
  execute_fn_t                          policyman_execute_msim_cm_event = NULL;

  switch(evt)
  {
    case CM_SS_EVENT_REG_REJECT:
      POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "received msim CM_SS_EVENT_REG_REJECT", p_info->asubs_id);
      policyman_execute_msim_cm_event = policyman_execute_msim_process_cm_reject_evt;
      break;

    case CM_SS_EVENT_SRV_CHANGED:
      POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "received msim CM_SS_EVENT_SRV_CHANGED", p_info->asubs_id);
      policyman_execute_msim_cm_event = policyman_execute_msim_process_cmss_evt;
      break;

    default:
      break;
  }

  if (NULL != policyman_execute_msim_cm_event)
  {
    policyman_msim_cmss_evt_cb_t  *pCmd = NULL;

    pCmd = POLICYMAN_CMD_NEW( policyman_msim_cmss_evt_cb_t,
                              policyman_execute_msim_cm_event,
                              NULL,
                              NULL ); // No policy associated with CM SS Event

    pCmd->evt       = evt;
    pCmd->info      = *p_info;
    pCmd->timestamp = mre_util_get_time();

    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    ref_cnt_obj_release(pCmd);
  }
}

/*=============================================================================
  Conditions based on serving system.
=============================================================================*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  sys_sys_mode_mask_e_type rat_mask;
} policyman_serving_rat_condition_t;

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  mre_set_t *pRfBandSet;
} policyman_serving_rf_band_condition_t;

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  mre_set_t *pRejectCauseSet;
} policyman_reg_reject_condition_t;


/*-------- policyman_serving_plmn_is_in_stack --------*/
STATIC boolean policyman_serving_plmn_is_in_stack(
  mre_policy_t                 *pPolicy,
  policyman_stack_info_t const *pStack,
  mre_plmn_mcc_set_t           *pPlmnSet
)
{
  boolean  plmn_is_in_stack = FALSE;

  if (pStack->serving_info_valid)
  {
    switch (pStack->sys_mode)
    {
      case SYS_SYS_MODE_CDMA:
      case SYS_SYS_MODE_HDR:
       // don't handle 3GPP2 systems since MNC isn't reliable to get a PLMN.
       break;

      default:
        plmn_is_in_stack = mre_plmn_list_contains(pPolicy, pPlmnSet, &pStack->plmn);
        break;
    }
  }

  return plmn_is_in_stack;
}


/*-------- serving_plmn_is_in_stack --------*/
STATIC boolean serving_plmn_is_in_stack(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_set_condition_t     *pCond  = (policyman_set_condition_t *) pCondition;
  mre_policy_t                  *pPolicy  = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t             *pState   = POLICY_EXECUTE_STATE(pCtx); 
  policyman_stack_info_t const  *pStack;
  size_t                         stackId;
  boolean                        result = FALSE;
  mre_plmn_mcc_set_t            *pPlmnSet;

  POLICYMAN_UT_MSG_2( SUBS_PREFIX "evaluating <serving_plmn_in pmsubs=%d>",
                      POLICY_SUBS(pPolicy),
                      subsId );

  //  Find the PLMN set to use; if we don't have one we'll return FALSE.
  pPlmnSet = policyman_get_set_from_condition(pCond, pPolicy, POLICYMAN_NAMED_PLMN_SET);
  if (pPlmnSet == NULL)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "PLMN set not found", POLICY_SUBS(pPolicy));
    goto Done;
  }

  //  Check to see if some stack's PLMN is in the list.
  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; ++stackId)
  {
    pStack = policyman_ss_get_stack(pState, subsId, stackId);

    if (policyman_serving_plmn_is_in_stack(pPolicy, pStack, pPlmnSet))
    {
      POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "PLMN of subs %d stack %d is in the PLMN set",
                            POLICY_SUBS(pPolicy),
                            subsId,
                            stackId );
      result = TRUE;
      goto Done;
    }
    else
    {
      POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "<serving_plmn_in> PLMN not found in stack %d, is_operational %d, srv_status %d, plmn valid %d",
                            subsId,
                            stackId,
                            pStack->is_operational,
                            pStack->srv_status,
                            pStack->serving_info_valid );
    }
  }

Done:
  POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "condition <serving_plmn_in> returns %d",
                      subsId,
                      LINE_NUM(pCond),
                      result );
  return result;
}


/*-------- policyman_condition_serving_plmn_evaluate --------*/
STATIC boolean policyman_condition_serving_plmn_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, serving_plmn_is_in_stack);
}

/*-------- policyman_condition_serving_plmn_new --------*/
mre_status_t policyman_condition_serving_plmn_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  return policyman_plmn_mcc_new( pElem, 
                                 pPolicy,
                                 MRE_NAMED_PLMN_SET,
                                 mre_plmn_list_read,
                                 ppCondition, 
                                 policyman_condition_serving_plmn_evaluate );
}

/*-------- policyman_3gpp_mcc_is_in_stack --------*/
STATIC boolean policyman_3gpp_mcc_is_in_stack(
  mre_policy_t                  *pPolicy,
  policyman_stack_info_t const  *pStack,
  mre_plmn_mcc_set_t const      *pMccSet
)
{
  sys_mcc_type  servingMcc;

  servingMcc = policyman_plmn_get_mcc((sys_plmn_id_s_type *)&pStack->plmn);

  return pStack->serving_info_valid
         && mre_mcc_list_contains(pPolicy, pMccSet, &servingMcc);
}


/*-------- policyman_check_mcc_in_list --------*/
STATIC boolean policyman_does_mcc_list_intersect_mcc_set(
  policyman_stack_info_t const   *pStack,
  mre_plmn_mcc_set_t     const   *pMccSet,
  sys_plmn_mcc_list      const   *pMccList
)
{
  size_t   index;
  boolean  result = FALSE;

  if (pStack->serving_info_valid)
  {
    for (index = 0; !result && index < pMccList->count; index++)
    {
      result = mre_named_set_contains( (mre_named_set_t const *)pMccSet, 
                                       &pMccList->mcc_list[index]);
    }
  }

  return result;
}

/*-------- subs_mcc_is_in_set --------*/
STATIC boolean subs_mcc_is_in_set(
  mre_policy_t             *pPolicy,
  policyman_state_t        *pState,
  mre_plmn_mcc_set_t const *pMccSet,
  sys_modem_as_id_e_type    subsId
)
{
  policyman_stack_info_t const *pStack;
  size_t                        stackId;
  boolean                       mccIsOnSubs = FALSE;

  /*  Check to see if some stack's MCC is in the list.
   */
  for (stackId = SYS_MODEM_STACK_ID_1; !mccIsOnSubs && stackId < SYS_MODEM_STACK_ID_MAX; ++stackId)
  {
    pStack = policyman_ss_get_stack(pState, subsId, stackId);

    if (pStack->sys_mode == SYS_SYS_MODE_CDMA)
    {
      mccIsOnSubs = policyman_does_mcc_list_intersect_mcc_set(pStack, pMccSet, &pStack->mcc_list_3gpp2);
    }
    else if (pStack->sys_mode == SYS_SYS_MODE_HDR)
    {
      mccIsOnSubs = policyman_does_mcc_list_intersect_mcc_set(pStack, pMccSet, &pStack->mcc_list_hdr);
    }
    else
    {
      mccIsOnSubs = policyman_3gpp_mcc_is_in_stack(pPolicy, pStack, pMccSet);
    }
  }

  return mccIsOnSubs;
}

/*-------- policyman_ss_user_mcc_is_in_set --------*/
STATIC boolean policyman_ss_user_mcc_is_in_set(
  mre_policy_t             *pPolicy,
  policyman_state_t        *pState,
  mre_plmn_mcc_set_t const *pMccSet,
  sys_modem_as_id_e_type    subsId
)
{
  policyman_ss_info_t *pInfo;
  sys_mcc_type         userMcc = NO_MCC;
 
  pInfo    = policyman_state_get_serving_system_info(pState, subsId);
  userMcc  = policyman_plmn_get_mcc(&pInfo->user_plmn);

  return mre_mcc_list_contains(pPolicy, pMccSet, &userMcc);
}


/*-------- policyman_serving_mcc_is_in_other_subs --------*/
STATIC boolean policyman_serving_mcc_is_in_other_subs(
  mre_policy_t            *pPolicy,
  policyman_state_t       *pState,
  mre_plmn_mcc_set_t      *pMccSet,
  sys_modem_as_id_e_type   policySubs
)
{
  sys_modem_as_id_e_type  subs;
  size_t                  nSim = 0;

  (void)policyman_get_current_num_sim(&nSim);

  for (subs = SYS_MODEM_AS_ID_1; subs < nSim; ++subs)
  {
    if ( subs != policySubs
       && subs_mcc_is_in_set(pPolicy, pState, pMccSet, subs)
       )
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*-------- serving_mcc_is_in_set --------*/
STATIC boolean serving_mcc_is_in_set(
  policyman_condition_t const  *pCondition,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  policyman_set_condition_t *pCond         = (policyman_set_condition_t *) pCondition;
  mre_policy_t              *pPolicy       = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t         *pState        = POLICY_EXECUTE_STATE(pCtx); 
  mre_plmn_mcc_set_t        *pMccSet;
  sys_modem_as_id_e_type     thisSubs      = POLICY_SUBS(pPolicy);
  boolean                    mcc_is_in_set = FALSE;

  pMccSet = policyman_get_set_from_condition(pCond, pPolicy, MRE_NAMED_MCC_SET);
  if (pMccSet == NULL)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "named PLMN set not found", thisSubs);
    goto Done;
  }

  switch (pCond->pmSubsId)
  {
    case SUBS_THIS:
      mcc_is_in_set = subs_mcc_is_in_set(pPolicy, pState, pMccSet, thisSubs);
      break;

    case SUBS_OTHER:
      mcc_is_in_set = policyman_serving_mcc_is_in_other_subs(pPolicy, pState, pMccSet, thisSubs);
      break;

    case SUBS_ALL:
      mcc_is_in_set =     subs_mcc_is_in_set(pPolicy, pState, pMccSet, thisSubs)
                      &&  policyman_serving_mcc_is_in_other_subs(pPolicy, pState, pMccSet, thisSubs);
      break;

    case SUBS_ANY:
      mcc_is_in_set =     subs_mcc_is_in_set(pPolicy, pState, pMccSet, thisSubs)
                      ||  policyman_serving_mcc_is_in_other_subs(pPolicy, pState, pMccSet, thisSubs);
      break;

    default:
      POLICYMAN_MSG_ERROR_2( SUBS_PREFIX "unsupported subs type in serving_mcc_is_in_set: %d",
                             POLICY_SUBS(pPolicy),
                             pCond->pmSubsId );
      break;
  }

Done:
  POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "condition <serving_mcc_in> returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      mcc_is_in_set );

  return mcc_is_in_set;
}

/*-------- policyman_condition_serving_mcc_evaluate --------*/
STATIC boolean policyman_condition_serving_mcc_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, serving_mcc_is_in_set);
}

/*-------- policyman_condition_serving_mcc_new --------*/
mre_status_t policyman_condition_serving_mcc_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  return policyman_plmn_mcc_new( pElem, 
                                 pPolicy,
                                 MRE_NAMED_MCC_SET,
                                 mre_mcc_list_read,
                                 ppCondition, 
                                 policyman_condition_serving_mcc_evaluate );
}


/*-------- have_location_from_user_plmn --------*/
STATIC boolean have_location_from_user_plmn(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_ss_info_t *pInfo;

  pInfo = policyman_state_get_serving_system_info(pState, subsId);
  
  return    policyman_ph_get_net_sel_mode_pref(pState, subsId) == CM_NETWORK_SEL_MODE_PREF_MANUAL
         && !sys_plmn_id_is_undefined(pInfo->user_plmn);
}


/*-------- location_is_in_mcc_set --------*/
boolean location_is_in_mcc_set(
  mre_policy_t              *pPolicy,
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  mre_plmn_mcc_set_t const  *pMccSet
)
{
  boolean                haveLocationFromUser;
  boolean                haveLocationFromService;
  boolean                result           = FALSE;
  sys_mcc_type           hlos_mcc;
  sys_modem_as_id_e_type locationSubs     = subsId;

  haveLocationFromUser    = have_location_from_user_plmn(pState, subsId);
  haveLocationFromService = policyman_subs_has_location(pState, subsId);
  hlos_mcc                = policyman_state_get_hlos_mcc(pState);

  if(!haveLocationFromService)
  {
    sys_modem_as_id_e_type  subsWithLocation = policyman_find_subs_with_location_info(pState);
    haveLocationFromService = (SYS_MODEM_AS_ID_NONE != subsWithLocation);

    if(haveLocationFromService)
    {
      locationSubs = subsWithLocation;
    }
  }

  // Try evaluation by looking into User, Modem and HLOS MCC
  if (haveLocationFromUser)
  {
    result = policyman_ss_user_mcc_is_in_set(pPolicy, pState, pMccSet, subsId);
  }
  else if (haveLocationFromService)
  {
    result = subs_mcc_is_in_set(pPolicy, pState, pMccSet, locationSubs);
  }
  else if (NO_MCC != hlos_mcc)
  {
    result = mre_named_set_contains((mre_named_set_t const *)pMccSet, &hlos_mcc);
  }

  POLICYMAN_MSG_HIGH_6( SUBS_PREFIX "<location_is_in_mcc_set> returns %d (haveLocationFromUser %d, haveLocationFromService %d, hlos_mcc %d, locationSubs %d)",
                        subsId,
                        result,
                        haveLocationFromUser,
                        haveLocationFromService,
                        hlos_mcc,
                        locationSubs );

  return result;
}

/*-------- location_mcc_is_in_set  --------*/
STATIC boolean location_mcc_is_in_set(
  policyman_condition_t const  *pCondition,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  mre_plmn_mcc_set_t        *pMccSet;
  policyman_set_condition_t *pCond         = (policyman_set_condition_t *) pCondition;
  mre_policy_t              *pPolicy       = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t         *pState        = POLICY_EXECUTE_STATE(pCtx); 
  boolean                    mcc_is_in_set = FALSE;

  pMccSet = policyman_get_set_from_condition(pCond, pPolicy, MRE_NAMED_MCC_SET);
  if (pMccSet != NULL)
  {
    mcc_is_in_set = location_is_in_mcc_set(pPolicy, pState, subsId, pMccSet);
  }
  else
  {
    POLICYMAN_MSG_ERROR_2(SUBS_PREFIX SRC_LINE "named MCC set not found",
                          POLICY_SUBS(pPolicy),
                          LINE_NUM(pCond) );
  }

 return mcc_is_in_set;
}

/*-------- policyman_condition_location_mcc_evaluate --------*/
STATIC boolean policyman_condition_location_mcc_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, location_mcc_is_in_set);
}

/*-------- policyman_condition_location_mcc_new --------*/
mre_status_t policyman_condition_location_mcc_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  return policyman_plmn_mcc_new( pElem, 
                                 pPolicy,
                                 MRE_NAMED_MCC_SET,
                                 mre_mcc_list_read,
                                 ppCondition, 
                                 policyman_condition_location_mcc_evaluate );
}


/*-------- current_mcc_in_set  --------*/
STATIC boolean current_mcc_in_set(
  policyman_condition_t const  *pCondition,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  mre_plmn_mcc_set_t        *pMccSet;
  policyman_set_condition_t *pCond         = (policyman_set_condition_t *) pCondition;
  mre_policy_t              *pPolicy       = EXECUTE_CTX_POLICY(pCtx);
  boolean                    mcc_is_in_set = FALSE;
  sys_mcc_type               mcc           = NO_MCC;

  pMccSet = policyman_get_set_from_condition( pCond,
                                              pPolicy,
                                              MRE_NAMED_MCC_SET );

  if (NON_NULL(pMccSet))
  {
    policyman_get_current_mcc(subsId, &mcc);
    mcc_is_in_set = mre_named_set_contains((mre_named_set_t const *)pMccSet, &mcc);
  }

  return mcc_is_in_set;
}


/*-------- policyman_condition_current_mcc_in_evaluate --------*/
STATIC boolean policyman_condition_current_mcc_in_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, current_mcc_in_set);
}


/*-------- policyman_condition_current_mcc_in_new --------*/
mre_status_t policyman_condition_current_mcc_in_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  return policyman_plmn_mcc_new( pElem, 
                                 pPolicy,
                                 MRE_NAMED_MCC_SET,
                                 mre_mcc_list_read,
                                 ppCondition, 
                                 policyman_condition_current_mcc_in_evaluate );
}

/*-----------------------------------------------------------------------------
  <service_status_in>
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  mre_set_t   *pSrvStatusSet;
} policyman_service_status_condition_t;

/*-------- policyman_str_to_service_status --------*/
sys_srv_status_e_type policyman_str_to_service_status(
  char const  *pStr
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("FULL",              SYS_SRV_STATUS_SRV),
    MRE_STR_VALUE_ENTRY("LIMITED",           SYS_SRV_STATUS_LIMITED),
    MRE_STR_VALUE_ENTRY("LIMITED-REGIONAL",  SYS_SRV_STATUS_LIMITED_REGIONAL),
    MRE_STR_VALUE_ENTRY("OOS",               SYS_SRV_STATUS_NO_SRV),
    MRE_STR_VALUE_ENTRY("POWER_SAVE",        SYS_SRV_STATUS_PWR_SAVE),
  };

  char                  token[32];
  sys_srv_status_e_type srv_status = SYS_SRV_STATUS_NONE;

  if (policyman_get_token(&pStr, token, sizeof(token)))
  {
    mre_status_t  status;

    MRE_LOOKUP_STR_VALUE(map, token, sys_srv_status_e_type, &srv_status);
    if (MRE_FAILED(status))
    {
      POLICYMAN_UT_MSG_1("Invalid value for <service_status_in>: %s", token);
    }
  }

  return srv_status;
}


/*-------- policyman_service_status_in_subs --------*/
boolean policyman_service_status_in_subs(
  sys_modem_as_id_e_type   subsId,
  mre_set_t               *pSrvStatusSet,
  policyman_state_t       *pState
)
{
  boolean retval = FALSE;
  policyman_stack_info_t  *pStack0 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_1);
  policyman_stack_info_t  *pStack1 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_2);

  /* If both stacks are operational then check for both
   */
  if (   pStack0->is_operational
     && pStack1->is_operational
    )
  {
    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "<service_status_in> service is stack[0] %d stack[1] %d",
                          subsId,
                          pStack0->srv_status,
                          pStack1->srv_status );

    retval =    mre_set_contains(pSrvStatusSet, &pStack0->srv_status)
             || mre_set_contains(pSrvStatusSet, &pStack1->srv_status);
  }

  /* Check stack 0 has srv in set
   */
  else if (pStack0->is_operational)
  {
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "<service_status_in> stack[0] service is %d",
                          subsId,
                          pStack0->srv_status );

    retval = mre_set_contains(pSrvStatusSet, &pStack0->srv_status);
  }

  /* Check stack 1 has srv in set
   */
  else if (pStack1->is_operational)
  {
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "<service_status_in> stack[1] service is %d",
                          subsId,
                          pStack1->srv_status );

    retval = mre_set_contains(pSrvStatusSet, &pStack1->srv_status);
  }

  return retval;
}


/*-------- policyman_service_status_in_other_subs --------*/
STATIC boolean policyman_service_status_in_other_subs(
  sys_modem_as_id_e_type  policySubs,
  mre_set_t              *pSrvStatusSet,
  policyman_state_t      *pState
)
{
  sys_modem_as_id_e_type  subs;
  size_t                  nSim = 0;

  (void)policyman_get_current_num_sim(&nSim);

  for (subs = SYS_MODEM_AS_ID_1 ; subs < nSim ; ++subs)
  {
    if (
          subs != policySubs
       && policyman_service_status_in_subs(subs, pSrvStatusSet, pState)
       )
    {
      return TRUE;
    }
  }

  return FALSE;
}


/*-------- evaluate_service_status --------*/
STATIC boolean evaluate_service_status(
  policyman_condition_t const  *pCondition,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  policyman_service_status_condition_t const *pCond = (policyman_service_status_condition_t *) pCondition;
  mre_policy_t                               *pPolicy     = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t                                  *pItemSet    = POLICY_EXECUTE_ITEMSET(pCtx); 
  policyman_state_t                          *pState      = POLICY_EXECUTE_STATE(pCtx); 
  sys_modem_as_id_e_type                      policySubs  = POLICY_SUBS(pPolicy);
  boolean                                     result;

  switch (pCond->pmSubsId)
  {
    case SUBS_THIS:
      result = policyman_service_status_in_subs(policySubs, pCond->pSrvStatusSet, pState);
      break;

    case SUBS_OTHER:
      result = policyman_service_status_in_other_subs(policySubs, pCond->pSrvStatusSet, pState);
      break;

    case SUBS_ALL:
      result =    policyman_service_status_in_subs(policySubs, pCond->pSrvStatusSet, pState)
               && policyman_service_status_in_other_subs(policySubs, pCond->pSrvStatusSet, pState);
      break;

    default:
      result = FALSE;
      break;
  }

  POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "condition <service_status_in> returns %d",
                      policySubs,
                      LINE_NUM(pCondition),
                      result );

  return result;
}

/*-------- policyman_condition_service_status_evaluate --------*/
STATIC boolean policyman_condition_service_status_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_service_status);
}

/*-------- policyman_condition_service_status_dtor --------*/
STATIC void policyman_condition_service_status_dtor(
  void  *pObj
)
{
  policyman_service_status_condition_t *pCondition = (policyman_service_status_condition_t *) pObj;

  MRE_RELEASE_IF(pCondition->pSrvStatusSet);
  policyman_condition_dtor(pCondition);
}


/*-------- policyman_get_srv_status_set --------*/
STATIC mre_set_t * policyman_get_srv_status_set(
  mre_xml_element_t const *pElem
)
{
  mre_set_t             *pSet = NULL;
  char const            *pStr;
  char                   token[32];
  sys_srv_status_e_type  srv_status;

  pStr = policyman_xml_get_text(pElem);
  if (pStr == NULL)
  {
    goto Done;
  }

  pSet = mre_set_new( sizeof(sys_srv_status_e_type),
                      5,
                      MRE_SET_NO_MAX_SIZE,
                      NULL,
                      NULL );

  while (policyman_get_token(&pStr, token, sizeof(token)))
  {
    srv_status = policyman_str_to_service_status(token);
    if (srv_status == SYS_SRV_STATUS_NONE)
    {
      MRE_RELEASE_IF(pSet);
      goto Done;
    }
    mre_set_add(pSet, &srv_status);
  }

Done:
  return pSet;
}


/*-------- policyman_condition_service_status_new --------*/
mre_status_t policyman_condition_service_status_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                           status = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_subs_t                       pm_subs = SUBS_THIS;
  mre_set_t                             *pSrvStatusSet = NULL;
  policyman_service_status_condition_t  *pCondition = NULL;
  char const                            *pStr;

  /*---------------------------------------------------------------------------
    Get which stack(s) to check
  ---------------------------------------------------------------------------*/
  pStr = policyman_xml_get_attribute(pElem, "subs");
  if (pStr != NULL)
  {
    pm_subs = policyman_str_to_subs(pStr);
    if (pm_subs == SUBS_NONE)
    {
      goto Done;
    }
  }

  /*---------------------------------------------------------------------------
    Get the list of statuses.
  ---------------------------------------------------------------------------*/
  pSrvStatusSet = policyman_get_srv_status_set(pElem);
  if (pSrvStatusSet == NULL)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "<service_status_in>: list of service status required", POLICY_SUBS(pPolicy));
    goto Done;
  }

  /*---------------------------------------------------------------------------
    Create the condition object.
  ---------------------------------------------------------------------------*/
  pCondition = POLICYMAN_CONDITION_NEW( policyman_service_status_condition_t,
                                        pPolicy,
                                        policyman_condition_service_status_evaluate,
                                        policyman_condition_service_status_dtor,
                                        pm_subs,
                                        pElem );

  pCondition->pSrvStatusSet = pSrvStatusSet;
  ref_cnt_obj_add_ref(pSrvStatusSet);

  status = MRE_STATUS_SUCCESS;

Done:
  MRE_RELEASE_IF(pSrvStatusSet);
  if (MRE_FAILED(status))
  {
    MRE_RELEASE_IF(pCondition);
  }

  *ppCondition = (mre_condition_t *) pCondition;
  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman_condition_service_status_new status %d",
                        POLICY_SUBS(pPolicy),
                        status );

  return status;
}


/*-------- evaluate_serving_rat_in --------*/
STATIC boolean evaluate_serving_rat_in(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_serving_rat_condition_t const *pCond  = (policyman_serving_rat_condition_t *) pCondition;
  mre_policy_t                            *pPolicy  = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t                       *pState   = POLICY_EXECUTE_STATE(pCtx); 
  policyman_ss_info_t const               *pSSInfo;
  boolean                                  result;

  pSSInfo = policyman_state_get_serving_system_info(pState, subsId);
  /* return result for MAIN stack only
    */
  result = ( ((pCond->rat_mask & BM(pSSInfo->stack[SYS_MODEM_STACK_ID_1].sys_mode)) != 0)
             ||
             ((pCond->rat_mask & BM(pSSInfo->stack[SYS_MODEM_STACK_ID_2].sys_mode)) != 0));

  POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "condition <serving_rat_in> returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      result );
  return result;
}


/*-------- policyman_condition_serving_rat_evaluate --------*/
STATIC boolean policyman_condition_serving_rat_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_serving_rat_in);
}


/*-------- policyman_condition_serving_rat_new --------*/
mre_status_t policyman_condition_serving_rat_new(
   mre_xml_element_t const       *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                       status = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_serving_rat_condition_t *pCondition;
  char const                        *pStr;
  char                               token[16];
  uint32                             mask;
  policyman_subs_t                   pm_subs;

  pStr = policyman_xml_get_text(pElem);

  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_0("<serving_rat_in> no list of RATs specified");
    goto Done;
  }

  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pCondition = POLICYMAN_CONDITION_NEW( policyman_serving_rat_condition_t,
                                        pPolicy,
                                        policyman_condition_serving_rat_evaluate,
                                        policyman_condition_dtor,
                                        pm_subs,
                                        pElem );

  pCondition->rat_mask = 0;

  while (policyman_get_token(&pStr, token, sizeof(token)))
  {
    mask = policyman_rat_capability_str_to_rat(token);
    if (mask == INVALID_RAT_MASK)
    {
      POLICYMAN_UT_MSG_1("<serving_rat_in>: invalid RAT specified: %s", token);
      break;
    }
    pCondition->rat_mask |= mask;
  }

  status = (pCondition->rat_mask != SYS_SYS_MODE_MASK_NONE) ? MRE_STATUS_SUCCESS : MRE_STATUS_ERR_MALFORMED_XML;

  if (MRE_SUCCEEDED(status))
  {
    *ppCondition = (mre_condition_t *) pCondition;
  }
  else
  {
    MRE_RELEASE_IF(pCondition);
  }

Done:
    return status;
}


/*-------- evaluate_serving_band_in --------*/
STATIC boolean evaluate_serving_band_in(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_serving_rf_band_condition_t *pCond = (policyman_serving_rf_band_condition_t *) pCondition;
  mre_policy_t                          *pPolicy      = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t                             *pItemSet     = POLICY_EXECUTE_ITEMSET(pCtx); 
  policyman_state_t                     *pState       = POLICY_EXECUTE_STATE(pCtx); 
  sys_band_class_e_type                  active_band;
  boolean                                result;

  /* Check only for stack 1
   */
  active_band = policyman_ss_get_active_band(pState, subsId, SYS_MODEM_STACK_ID_1);

  result = mre_set_contains(pCond->pRfBandSet, &active_band);

  POLICYMAN_UT_MSG_4( SUBS_PREFIX SRC_LINE "condition <serving_band_in subs=%d> returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      subsId,
                      result );
  return result;
}


/*-------- policyman_condition_serving_rf_band_evaluate --------*/
STATIC boolean policyman_condition_serving_rf_band_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_serving_band_in);
}

/*-------- policyman_condition_serving_rf_band_dtor --------*/
STATIC void policyman_condition_serving_rf_band_dtor(
  void  *pObj
)
{
  policyman_serving_rf_band_condition_t *pCondition;

  pCondition = (policyman_serving_rf_band_condition_t *) pObj;

  MRE_RELEASE_IF(pCondition->pRfBandSet);
  policyman_condition_dtor(pCondition);
}



/*-------- policyman_condition_serving_rf_band_new --------*/
mre_status_t policyman_condition_serving_rf_band_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                           status = MRE_STATUS_ERR_INVALID_CONDITION;
  policyman_serving_rf_band_condition_t *pCondition = NULL;
  char const                            *pStr;
  char                                   activeBandStr[4];
  sys_band_class_e_type                  active_band;
  mre_set_t                             *pRfBandSet = NULL;
  policyman_subs_t                       pm_subs;

  pStr = policyman_xml_get_text(pElem);

  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<serving_band_in>: no band list specified", POLICY_SUBS(pPolicy));
    goto Done;
  }

  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pCondition = POLICYMAN_CONDITION_NEW( policyman_serving_rf_band_condition_t,
                                        pPolicy,
                                        policyman_condition_serving_rf_band_evaluate,
                                        policyman_condition_serving_rf_band_dtor,
                                        pm_subs,
                                        pElem );

  pRfBandSet = mre_set_new( sizeof(sys_band_class_e_type),
                            5,
                            MRE_SET_NO_MAX_SIZE,
                            NULL,
                            NULL );
  
  pCondition->pRfBandSet = pRfBandSet;

  while (policyman_get_token(&pStr, activeBandStr, sizeof(activeBandStr)))
  {
    active_band = atoi(activeBandStr);
    if (active_band < 0)
    {
      POLICYMAN_UT_MSG_2( SUBS_PREFIX "<serving_band_in>: invalid band specified: %s",
                          POLICY_SUBS(pPolicy),
                          activeBandStr );
      goto Done;
    }

    mre_set_add(pRfBandSet, &active_band);
  }

  status = MRE_STATUS_SUCCESS;

Done:
  if (MRE_FAILED(status))
  {
    MRE_RELEASE_IF(pCondition);
  }

  *ppCondition = (mre_condition_t *) pCondition;

  return status;
}


/*-------- evaluate_reg_reject_cause_in --------*/
STATIC boolean evaluate_reg_reject_cause_in(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_reg_reject_condition_t *pCond = (policyman_reg_reject_condition_t *) pCondition;
  mre_policy_t                     *pPolicy         = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t                        *pItemSet        = POLICY_EXECUTE_ITEMSET(pCtx); 
  policyman_state_t                *pState          = POLICY_EXECUTE_STATE(pCtx); 
  byte                              reject_cause;
  boolean                           result;

  reject_cause = policyman_ss_get_reject_cause(pState, subsId, SYS_MODEM_STACK_ID_1);

  result = mre_set_contains(pCond->pRejectCauseSet, &reject_cause);

  POLICYMAN_UT_MSG_4( SUBS_PREFIX SRC_LINE "condition <reg_reject_cause_in subs=%d> returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      subsId,
                      result );
  return result;
}


/*-------- policyman_condition_reg_reject_evaluate --------*/
STATIC boolean policyman_condition_reg_reject_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_reg_reject_cause_in);
}


/*-------- policyman_condition_reg_reject_dtor --------*/
STATIC void policyman_condition_reg_reject_dtor(
  void  *pObj
)
{
  policyman_reg_reject_condition_t *pCondition = (policyman_reg_reject_condition_t *)pObj;

  MRE_RELEASE_IF(pCondition->pRejectCauseSet);
  policyman_condition_dtor(pCondition);
}


/*-------- policyman_condition_reg_reject_new --------*/
mre_status_t policyman_condition_reg_reject_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                      status = MRE_STATUS_ERR_INVALID_CONDITION;
  policyman_reg_reject_condition_t *pCondition = NULL;
  char const                       *pStr;
  char                              RejCauseStr[4];
  byte                              reject_cause;
  mre_set_t                        *pRejectCauseSet = NULL;
  policyman_subs_t                  pm_subs;

  pStr = policyman_xml_get_text(pElem);

  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_0("<reg_reject_cause_in>: list of rejection codes required");
    goto Done;
  }

  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pCondition = POLICYMAN_CONDITION_NEW( policyman_reg_reject_condition_t,
                                        pPolicy,
                                        policyman_condition_reg_reject_evaluate,
                                        policyman_condition_reg_reject_dtor,
                                        pm_subs,
                                        pElem );

  pRejectCauseSet = mre_set_new( sizeof(byte),
                                 5,
                                 MRE_SET_NO_MAX_SIZE,
                                 NULL,
                                 NULL );
  
  pCondition->pRejectCauseSet = pRejectCauseSet;

  {
    int temp_rej;

    while (policyman_get_token(&pStr, RejCauseStr, sizeof(RejCauseStr)))
    {
      temp_rej = atoi(RejCauseStr);
      if (temp_rej < 0)
      {
        POLICYMAN_UT_MSG_1("<reg_reject_cause_in>: invalid rejection cause: %s", RejCauseStr);
        goto Done;
      }

      reject_cause = (byte) temp_rej;
      mre_set_add(pRejectCauseSet, &reject_cause);
    }
  }

  status = MRE_STATUS_SUCCESS;

Done:
  if (MRE_FAILED(status))
  {
    MRE_RELEASE_IF(pCondition);
  }

  *ppCondition = (mre_condition_t *) pCondition;

  return status;
}

/*-----------------------------------------------------------------------------
  OOS SCAN CNT
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  policyman_comparator_t  test;
  int                     limit;
} policyman_oos_scan_cnt_condition_t;

/*-------- policyman_str_to_service_status --------*/
policyman_comparator_t policyman_str_to_comparator(
  char const  *pStr
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("=",  POLICYMAN_CMP_EQUALS),
    MRE_STR_VALUE_ENTRY(">",  POLICYMAN_CMP_EXCEEDS),
    MRE_STR_VALUE_ENTRY(">=", POLICYMAN_CMP_EQUALS_OR_EXCEEDS),
  };

  char                    token[32];
  policyman_comparator_t  test = POLICYMAN_CMP_INVALID;

  if (policyman_get_token(&pStr, token, sizeof(token)))
  {
    mre_status_t  status;
    
    MRE_LOOKUP_STR_VALUE(map, token, policyman_comparator_t, &test);
    if (MRE_FAILED(status))
    {
      POLICYMAN_UT_MSG_S1("Invalid value for <oos_scan_cnt>: %s", token);
    }
  }

  return test;
}

/*-------- policyman_ss_get_min_acq_fail_count --------*/
int policyman_ss_get_min_acq_fail_count(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_stack_info_t   *pStack1Info;
  policyman_stack_info_t   *pStack2Info;
  int                       minAcqFail = 0;

  pStack1Info = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_1);
  pStack2Info = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_2);

  // check all operational radio stacks and get minimum Acq fail count
  if (pStack1Info->radio_is_operational && pStack2Info->radio_is_operational)
  {
    minAcqFail = MIN(pStack1Info->acq_fail_cnt, pStack2Info->acq_fail_cnt);
  }
  else if (pStack1Info->radio_is_operational)
  {
    minAcqFail = pStack1Info->acq_fail_cnt;
  }
  else if (pStack2Info->radio_is_operational)
  {
    minAcqFail = pStack2Info->acq_fail_cnt;
  }

  POLICYMAN_MSG_MED_6( SUBS_PREFIX "min_acq_fail_count %d: stack1(radio on %d, acq_fail_cnt %d), stack2(radio on %d, acq_fail_cnt %d)",
                       subsId,
                       minAcqFail,
                       pStack1Info->radio_is_operational,
                       pStack1Info->acq_fail_cnt,
                       pStack2Info->radio_is_operational,
                       pStack2Info->acq_fail_cnt );

  return minAcqFail;
}

/*-------- evaluate_oos_scan_cnt --------*/
STATIC boolean evaluate_oos_scan_cnt(
  policyman_condition_t const  *pCondition,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  policyman_oos_scan_cnt_condition_t  *pCond = (policyman_oos_scan_cnt_condition_t *)pCondition;
  mre_policy_t                        *pPolicy         = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t                           *pItemSet        = POLICY_EXECUTE_ITEMSET(pCtx); 
  policyman_state_t                   *pState          = POLICY_EXECUTE_STATE(pCtx); 
  boolean                              limitWasReached = FALSE;
  int                                  scanCnt;
  sys_modem_as_id_e_type               policySubs;

  policySubs = POLICY_SUBS(pPolicy);
  scanCnt    = policyman_ss_get_min_acq_fail_count(pState, policySubs);

  if (pCond->limit > 0)
  {
  switch(pCond->test)
  {
    case POLICYMAN_CMP_EQUALS:
        limitWasReached = (scanCnt == pCond->limit);
      break;

    case POLICYMAN_CMP_EXCEEDS:
        limitWasReached = (scanCnt > pCond->limit);
      break;

    case POLICYMAN_CMP_EQUALS_OR_EXCEEDS:
        limitWasReached = (scanCnt >= pCond->limit);
      break;

    default:
      break;
  }
  }

  POLICYMAN_MSG_HIGH_7( SUBS_PREFIX SRC_LINE "condition <oos_scan_cnt pmsubs=%d> with limit %d, scan count %d, test %d, limit was reached %d",
                        policySubs,
                        LINE_NUM(pCond),
                        pCond->pmSubsId,
                        pCond->limit,
                        scanCnt,
                        pCond->test,
                        limitWasReached );

  return limitWasReached;
}

/*-------- policyman_condition_oos_scan_cnt_evaluate --------*/
STATIC boolean policyman_condition_oos_scan_cnt_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_oos_scan_cnt);
}

/*-------- policyman_condition_oos_scan_cnt_dtor --------*/
STATIC void policyman_condition_oos_scan_cnt_dtor(
  void  *pObj
)
{
  policyman_oos_scan_cnt_condition_t *pCondition = (policyman_oos_scan_cnt_condition_t *)pObj;

  policyman_condition_dtor(pCondition);
}

/*-------- policyman_condition_oos_scan_cnt_new --------*/
mre_status_t policyman_condition_oos_scan_cnt_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                        status = MRE_STATUS_ERR_INVALID_CONDITION;
  policyman_oos_scan_cnt_condition_t *pCondition = NULL;
  char const                         *pStr;
  policyman_comparator_t              test;
  int                                 limit;
  policyman_subs_t                    pm_subs;

  pStr = mre_xml_get_attribute(pElem, "test");

  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<oos_scan_cnt> 'test' attribute required", POLICY_SUBS(pPolicy));
    goto Done;
  }

  test = policyman_str_to_comparator(pStr);

  if (test == POLICYMAN_CMP_INVALID)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<oos_scan_cnt> invalid 'test' attribute", POLICY_SUBS(pPolicy));
    goto Done;

  }

  pStr = mre_xml_get_attribute(pElem, "value");

  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<oos_scan_cnt> 'value' attribute required", POLICY_SUBS(pPolicy));
    goto Done;
  }

  limit = atoi(pStr);

  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pCondition = POLICYMAN_CONDITION_NEW( policyman_oos_scan_cnt_condition_t,
                                        pPolicy,
                                        policyman_condition_oos_scan_cnt_evaluate,
                                        policyman_condition_oos_scan_cnt_dtor,
                                        pm_subs,
                                        pElem );
  pCondition->test     = test;
  pCondition->limit    = limit;

  status = MRE_STATUS_SUCCESS;

Done:
  if (MRE_FAILED(status))
  {
    MRE_RELEASE_IF(pCondition);
  }

  *ppCondition = (mre_condition_t *) pCondition;
  return status;
}


/*-----------------------------------------------------------------------------
  Enforce Full RAT mode
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;
} policyman_enforce_fullrat_condition_t;

/*-------- evaluate_enforce_full_rat --------*/
STATIC boolean evaluate_enforce_full_rat(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  mre_policy_t           *pPolicy   = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t              *pItemSet  = POLICY_EXECUTE_ITEMSET(pCtx); 
  policyman_state_t      *pState    = POLICY_EXECUTE_STATE(pCtx); 
  sys_modem_as_id_e_type  subs      = POLICY_EXECUTE_SUBS(pCtx); 
  boolean                 enforceFullRat;

  /* PLMN id type stays undefined at "first" PWR_SAVE event after coming
        ONLINE, but not on a regular PWR_SAVE event sent after service loss.
   */
  enforceFullRat =    policyman_ss_subs_is_in_pwr_save(pState, subs)
                  && !policyman_ss_subs_sys_id_is_defined(pState, subs);

  POLICYMAN_UT_MSG_5( LOG_SUBS_PREFIX SRC_LINE "condition <enforce_full_rat> on subs %d returns %d",
                      LOG_SUBS(subs),
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCondition),
                      subs,
                      enforceFullRat );

  return enforceFullRat;
}

/*-------- policyman_condition_enforce_full_rat_evaluate --------*/
STATIC boolean policyman_condition_enforce_full_rat_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_enforce_full_rat);
}

/*-------- policyman_condition_enforce_full_rat_new --------*/
mre_status_t policyman_condition_enforce_full_rat_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                           status     = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_enforce_fullrat_condition_t *pCondition = NULL;

  pCondition = POLICYMAN_CONDITION_NEW( policyman_enforce_fullrat_condition_t,
                                        pPolicy,
                                        policyman_condition_enforce_full_rat_evaluate,
                                        policyman_condition_dtor,
                                        SUBS_THIS,
                                        pElem );

  *ppCondition = (mre_condition_t *) pCondition;

  status = MRE_STATUS_SUCCESS;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman_condition_enforce_full_rat_new status %d",
                        POLICY_SUBS(pPolicy),
                        status );
  return status;
}

/*-----------------------------------------------------------------------------
  Have Service
-----------------------------------------------------------------------------*/
boolean policyman_any_subs_has_service(
  policyman_state_t *pState
)
{
  boolean   haveService = FALSE;
  size_t    subs;

  for (subs = SYS_MODEM_AS_ID_1; !haveService && subs < POLICYMAN_NUM_SUBS; subs++)
  {
    haveService = policyman_ss_subs_is_in_service(pState, subs);
  }

  return haveService;

}

/*-------- evaluate_have_service --------*/
STATIC boolean evaluate_have_service(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  mre_policy_t       *pPolicy       = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t  *pState        = POLICY_EXECUTE_STATE(pCtx); 
  boolean               have_service;

  have_service = policyman_any_subs_has_service(pState);

  POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "condition <have_service> returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCondition),
                      have_service );
  return have_service;
}

/*-------- policyman_condition_have_service_evaluate --------*/
STATIC boolean policyman_condition_have_service_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_have_service);
}

/*-------- policyman_condition_have_service_new --------*/
mre_status_t policyman_condition_have_service_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t           status;
  policyman_condition_t *pCondition;

  pCondition = POLICYMAN_CONDITION_NEW( policyman_condition_t,
                                        pPolicy,
                                        policyman_condition_have_service_evaluate,
                                        mre_condition_dtor,
                                        SUBS_THIS,
                                        pElem );

  *ppCondition = (mre_condition_t *)pCondition;

  status = MRE_STATUS_SUCCESS;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman_condition_have_service_new returned status %d",
                        POLICY_SUBS(pPolicy),
                        status );
  return status;
}

/*-----------------------------------------------------------------------------
  Have Serving System
-----------------------------------------------------------------------------*/

/*-------- evaluate_have_serving_system --------*/
STATIC boolean evaluate_have_serving_system(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_condition_t const *pCond  = (policyman_condition_t const *)pCondition;
  mre_policy_t                *pPolicy      = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t           *pState       = POLICY_EXECUTE_STATE(pCtx); 
  boolean                      haveService;

  haveService = policyman_subs_has_location(pState, subsId);

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX SRC_LINE "<have_serving_system> returns %d",
                        POLICY_SUBS(pPolicy),
                        LINE_NUM(pCondition),
                        haveService );

  return haveService;
}

/*-------- policyman_condition_have_serving_system_evaluate --------*/
STATIC boolean policyman_condition_have_serving_system_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_have_serving_system);
}

/*-------- policyman_condition_have_serving_system_new --------*/
mre_status_t policyman_condition_have_serving_system_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  policyman_condition_t *pCondition;

  pCondition = POLICYMAN_CONDITION_NEW( policyman_condition_t,
                                        pPolicy,
                                        policyman_condition_have_serving_system_evaluate,
                                        policyman_condition_dtor,
                                        SUBS_THIS,
                                        pElem );

  *ppCondition = (mre_condition_t *)pCondition;

  return MRE_STATUS_SUCCESS;
}

/*-----------------------------------------------------------------------------
  Have Location
-----------------------------------------------------------------------------*/


/*-------- policyman_ss_have_location_from_user --------*/
boolean policyman_ss_have_location_from_user(
  location_info_t         *plocationInfo,
  sys_modem_as_id_e_type   subsId
)
{
  return plocationInfo->subs[subsId].haveLocationFromUser;
}


/*-------- policyman_ss_have_location_from_subs --------*/
boolean policyman_ss_have_location_from_subs(
  location_info_t         *plocationInfo,
  sys_modem_as_id_e_type   subsId
)
{
  return plocationInfo->subs[subsId].haveLocationOnSub;
}


/*-------- policyman_ss_have_valid_location_from_subs --------*/
boolean policyman_ss_have_valid_location_from_subs(
  location_info_t         *plocationInfo,
  sys_modem_as_id_e_type   subsId
)
{
  return plocationInfo->subs[subsId].subIsValidForLocation;
}


/*-------- policyman_ss_have_location_from_hlos --------*/
boolean policyman_ss_have_location_from_hlos(
  location_info_t          *plocationInfo
)
{
  return plocationInfo->haveHLOSLocation;
}


/*-------- policyman_ss_get_location_info --------*/
void policyman_ss_get_location_info(
  policyman_state_t       *pState,
  location_info_t         *pLocationInfo
)
{
  size_t                 numSim = 0;
  sys_modem_as_id_e_type subsId;

  (void)policyman_get_current_num_sim(&numSim);
  memset(pLocationInfo, 0, sizeof(location_info_t));

  for (subsId = SYS_MODEM_AS_ID_1; subsId < numSim; subsId++)
  {
    pLocationInfo->subs[subsId].haveLocationFromUser    = have_location_from_user_plmn(pState, subsId);
    pLocationInfo->subs[subsId].haveLocationOnSub       = policyman_subs_has_location(pState, subsId);
    pLocationInfo->subs[subsId].subIsValidForLocation   = policyman_subs_valid_for_location(pState, subsId);
  }

  pLocationInfo->haveHLOSLocation = (policyman_state_get_hlos_mcc(pState) != NO_MCC);
}


/*-------- policyman_ss_have_location_from_user_or_subs --------*/
boolean policyman_ss_have_location_from_user_or_subs(
  location_info_t         *plocationInfo,
  sys_modem_as_id_e_type   subsId
)
{
  return ( policyman_ss_have_location_from_user(plocationInfo, subsId) 
        || policyman_ss_have_location_from_subs(plocationInfo, subsId) );
}

/*-------- policyman_ss_have_location --------*/
STATIC boolean policyman_ss_have_location(
  location_info_t         *plocationInfo,
  mre_set_t               *pItemSet,
  sys_modem_as_id_e_type   subsId
)
{
  boolean                 haveLocation;

  // wait for acq fails before looking into other location sources
  haveLocation =   policyman_ss_have_location_from_user_or_subs(plocationInfo, subsId)
                || policyman_ss_have_location_from_hlos(plocationInfo)
                || ( !policyman_ss_have_valid_location_from_subs(plocationInfo, subsId)
                   && policyman_ss_have_location_from_subs(plocationInfo, OTHER_SUB(subsId)) );

  return haveLocation;
}


/*-------- evaluate_have_location --------*/
STATIC boolean evaluate_have_location(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  mre_policy_t              *pPolicy        = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t         *pState         = POLICY_EXECUTE_STATE(pCtx); 
  sys_modem_as_id_e_type     currentSubsId  = POLICY_EXECUTE_SUBS(pCtx);
  boolean                    haveLocation;
  location_info_t            locationInfo;

  policyman_ss_get_location_info(pState, &locationInfo);
  PRINT_LOCATION_INFO(locationInfo);

  haveLocation = policyman_ss_have_location( &locationInfo, 
                                             POLICY_EXECUTE_ITEMSET(pCtx), 
                                             currentSubsId );

  POLICYMAN_MSG_HIGH_4( SUBS_PREFIX SRC_LINE "<have_location> returns %d on subId %d )",
                        POLICY_SUBS(pPolicy),
                        LINE_NUM(pCondition),
                        haveLocation,
                        currentSubsId );

  return haveLocation;
}

/*-------- policyman_condition_have_location_evaluate --------*/
STATIC boolean policyman_condition_have_location_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_have_location);
}

/*-------- policyman_condition_have_location_new --------*/
mre_status_t policyman_condition_have_location_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  policyman_condition_t *pCondition;

  pCondition = POLICYMAN_CONDITION_NEW( policyman_condition_t,
                                        pPolicy,
                                        policyman_condition_have_location_evaluate,
                                        policyman_condition_dtor,
                                        SUBS_THIS,
                                        pElem );

  *ppCondition = (mre_condition_t *)pCondition;

  return MRE_STATUS_SUCCESS;
}


/*===========================================================================
  FUNCTION POLICYMAN_GET_CM_SERVING_INFO()

  DESCRIPTION
    GET changed CM SS Info from event into local policyman_ss_info_t structure

  PARAMETERS
    ss_info : Policyman local structure to retain changed SS info
    ss_evt : Incoming CM Serving Info
  ===========================================================================*/
void policyman_get_cm_serving_info(
  policyman_ss_info_t          *pm_ss_info,
  policyman_cm_serving_info_t  *cm_serving_info
)
{
  sys_modem_stack_id_e_type  pm_stack;
  policyman_stack_info_t    *pStack;

  if (pm_ss_info != NULL && cm_serving_info != NULL)
  {
    // get stack info from serving system info
    if (policyman_check_stack_is_valid(cm_serving_info->stack))
    {
      // Mark stack operational since CM is sending data for that stack
      pm_stack = cm_serving_info->stack;
      pStack = &pm_ss_info->stack[pm_stack];

      pStack->is_operational = TRUE;
      pStack->srv_status = cm_serving_info->srv_status;

      // get sys_mode
      pStack->sys_mode = cm_serving_info->sys_mode;

      POLICYMAN_MSG_HIGH_6( SUBS_PREFIX "cm_serving_info: stack %d, sys_mode %d, srv_status %d, id_type %d, srv_domain %d",
                            cm_serving_info->asubs_id,
                            pm_stack,
                            cm_serving_info->sys_mode,
                            cm_serving_info->srv_status,
                            cm_serving_info->id_type,
                            cm_serving_info->srv_domain );

      // update the PLMN id - CM guarantees to give only 3GPP PLMN id
      pStack->id_type = cm_serving_info->id_type;
      if (cm_serving_info->id_type == SYS_SYS_ID_TYPE_UMTS)
      {
        pStack->plmn = cm_serving_info->plmn;
        POLICYMAN_MSG_PLMN_2( SUBS_PREFIX "cm_serving_info: stack %d PLMN:",
                              cm_serving_info->asubs_id,
                              pm_stack,
                              pStack->plmn );
      }
      else
      {
        POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "PLMN id is not 3GPP, PLMN not updated", cm_serving_info->asubs_id);
      }
    }
    else
    {
      POLICYMAN_MSG_ERROR_2( SUBS_PREFIX "cm_serving_info: stack %d not valid",
                             cm_serving_info->asubs_id,
                             cm_serving_info->stack );
    }
  }
  else
  {
    POLICYMAN_MSG_ERROR_2( "get_cm_serving_info NULL pointer, data not copied - pm_ss_info %x, cm_serving_info %x",
                           pm_ss_info,
                           cm_serving_info );
  }
}


/*===========================================================================
  FUNCTION POLICYMAN_EXECUTE_PROCESS_CM_SERVING_INFO()

  DESCRIPTION
    Process serving system information received from Call Manager
 ===========================================================================*/
void policyman_execute_process_cm_serving_info(
  policyman_cmd_t *pCmd
)
{
  policyman_cm_serving_info_cb_t *pServingInfo = (policyman_cm_serving_info_cb_t *)pCmd;
  policyman_sandbox_t            *pSandbox     = pCmd->pSandbox;

  if (pServingInfo != NULL)
  {
    policyman_cm_serving_info_t *pInfo;

    pInfo = &pServingInfo->info;

    /* Allow API to update for all service domains but for subs 0, subs 1 & subs2
      */
    if ( pInfo && policyman_subs_id_is_valid(pInfo->asubs_id) )
    {
      struct policyman_ss_info_t ss_info;

      ss_info = *policyman_state_get_serving_system_info( pSandbox->pState,
                                                          pInfo->asubs_id );

      policyman_get_cm_serving_info(&ss_info, pInfo);

      policyman_ss_update_state(pSandbox, &ss_info, pInfo->asubs_id);
    }
  }
}

/*===========================================================================
  FUNCTION POLICYMAN_CM_SERVING_INFO_CB()

  DESCRIPTION
    Get Serving system information from CM

  PARAMETERS
    pCmServingInfo    Pointer to CM serving system information.
===========================================================================*/
void policyman_cm_serving_info_cb(
  policyman_cm_serving_info_t *pCmServingInfo
)
{
  policyman_cm_serving_info_cb_t *pCmd;

  if (pCmServingInfo != NULL)
  {
    pCmd = POLICYMAN_CMD_NEW( policyman_cm_serving_info_cb_t,
                              policyman_execute_process_cm_serving_info,
                              NULL,
                              NULL ); // No policy associated with CM SS Event

    pCmd->info = *pCmServingInfo;

    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    ref_cnt_obj_release(pCmd);
  }
}


/*-------- policyman_execute_process_hlos_mcc --------*/
void policyman_execute_process_hlos_mcc(
  policyman_cmd_t *pCmd
)
{
  policyman_hlos_mcc_cmd_t *pHlosInfo = (policyman_hlos_mcc_cmd_t *) pCmd;
  policyman_sandbox_t      *pSandbox  = pCmd->pSandbox;

  /*  Sanitize the input parameter. MCC is 3 digits
   */
  if (  pHlosInfo->status != 0
     || pHlosInfo->mcc < 1
     || pHlosInfo->mcc > 999
     )
  {
    POLICYMAN_MSG_ERROR_0("ignoring HLOS MCC, status is not SUCCESS or MCC out of range");
    return;
  }

  policyman_state_enter_crit_section();

  policyman_state_update_hlos_mcc( pSandbox->pState,
                                   pHlosInfo->mcc,
                                   pHlosInfo->confidenceLevel,
                                   pHlosInfo->status );

  policyman_ss_update_current_mcc_on_all_subs(pSandbox->pState);

  policyman_state_leave_crit_section();

  policyman_state_handle_update(pSandbox);
}

/*-------- policyman_set_hlos_mcc --------*/
policyman_status_t policyman_set_hlos_mcc(
  sys_mcc_type  mcc,
  size_t        confidenceLevel,
  size_t        status
)
{
  policyman_hlos_mcc_cmd_t *pCmd;

  POLICYMAN_MSG_HIGH_3( "received HLOS MCC %d, confidence level %d, HLOS status %d",
                        mcc,
                        confidenceLevel,
                        status );

  pCmd = POLICYMAN_CMD_NEW( policyman_hlos_mcc_cmd_t,
                            policyman_execute_process_hlos_mcc,
                            NULL,
                            NULL );

  pCmd->mcc             = mcc;
  pCmd->confidenceLevel = confidenceLevel;
  pCmd->status          = status;

  policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
  ref_cnt_obj_release(pCmd);

  return POLICYMAN_STATUS_SUCCESS;
}

/*-------- policyman_get_current_mcc --------*/
policyman_status_t policyman_get_current_mcc(
  sys_modem_as_id_e_type  subsId,
  sys_mcc_type           *pMcc
)
{
  policyman_state_t *pState;

  if (!policyman_subs_id_is_valid(subsId) || pMcc == NULL)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "Invalid argument: subsId", subsId);
    return POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }

  pState = policyman_state_get_state();
  *pMcc = policyman_state_get_subs_mcc(pState, subsId);

  return POLICYMAN_STATUS_SUCCESS;
}

/*-------- policyman_execute_process_acq_fail_ind --------*/
void policyman_execute_process_acq_fail_ind(
  policyman_cmd_t *pCmd
)
{
  policyman_cm_report_acq_fail_t *pAcqFailCmd = (policyman_cm_report_acq_fail_t *) pCmd;
  policyman_stack_info_t         *pStackInfo;

  pStackInfo = policyman_ss_get_stack( pAcqFailCmd->pSandbox->pState,
                                       pAcqFailCmd->subsId,
                                       pAcqFailCmd->stackId );

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "policyman_execute_process_acq_fail_ind: stack %d, acq_fail_cnt %d",
                        pStackInfo->subsId,
                        pAcqFailCmd->stackId,
                        pStackInfo->acq_fail_cnt + 1 );

  // ignore the Acq fail if radio stack is not operational
  if (pStackInfo->radio_is_operational)
  {
    policyman_ss_info_t ssInfo;

    ssInfo = *policyman_state_get_serving_system_info(pAcqFailCmd->pSandbox->pState, pAcqFailCmd->subsId);

    ssInfo.stack[pAcqFailCmd->stackId].acq_fail_cnt += 1;

    policyman_ss_update_state(pAcqFailCmd->pSandbox, &ssInfo, pAcqFailCmd->subsId);
  }
}

/*-------- policyman_report_acq_fail --------*/
void policyman_report_acq_fail(
  sys_modem_as_id_e_type         subsId,
  sys_modem_stack_id_e_type      stackId
)
{
  policyman_cm_report_acq_fail_t *pCmd;

  pCmd = POLICYMAN_CMD_NEW( policyman_cm_report_acq_fail_t,
                            policyman_execute_process_acq_fail_ind,
                            NULL,
                            NULL );

  pCmd->subsId = subsId;
  pCmd->stackId = stackId;

  policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
  ref_cnt_obj_release(pCmd);
}


/*-------- policyman_report_plmn_is_invalid --------*/
STATIC boolean policyman_report_plmn_is_invalid(
  sys_plmn_id_s_type *pPlmn
)
{
  sys_mcc_type  requestedMcc;

  requestedMcc = policyman_plmn_get_mcc(pPlmn);

  // invalid MCCs should not trigger a Policy change
  return (requestedMcc == NO_MCC);
}

/*-------- policyman_ss_set_serving_plmn_rat --------*/
void policyman_ss_set_serving_plmn_rat(
  policyman_sandbox_t    *pSandbox,
  sys_modem_as_id_e_type  subsId,
  sys_plmn_id_s_type     *pPlmn,
  sys_sys_mode_e_type     sys_mode
)
{
  policyman_state_t      *pState = pSandbox->pState;
  policyman_stack_info_t *pStack1;
  policyman_stack_info_t *pStack2;

  pStack1 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_1);
  pStack2 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_2);

  // Process candidate as limited GSM service on stack 1, no SS info on stack 2
  pStack1->plmn                 = *pPlmn;
  pStack1->sys_mode             = sys_mode;
  pStack1->is_operational       = TRUE;
  pStack1->radio_is_operational = TRUE;
  pStack1->serving_info_valid   = TRUE;
  pStack1->srv_status           = SYS_SRV_STATUS_LIMITED;

  pStack2->is_operational       = FALSE;
  pStack2->radio_is_operational = FALSE;
  pStack2->serving_info_valid   = FALSE;
}


STATIC boolean policyman_camping_info_is_valid(
  policyman_cm_serving_info_t  *cmServingInfo
)
{
  boolean     cmServingInfoValid = TRUE;

  if ( cmServingInfo->asubs_id == SYS_MODEM_AS_ID_NONE
       || cmServingInfo->asubs_id >= (sys_modem_as_id_e_type) POLICYMAN_NUM_SUBS
       || (FALSE == policyman_check_stack_is_valid(cmServingInfo->stack)))
  {
    POLICYMAN_MSG_ERROR_0("policyman_camping_info_is_valid: Invalid subsId or Stack");
    cmServingInfoValid = FALSE;
    goto Done;
  }

  if (policyman_is_initializing() || policyman_report_plmn_is_invalid(&(cmServingInfo->plmn)))
  {
    POLICYMAN_MSG_HIGH_0("Policyman is initializing, or invalid PLMN, no change");
    cmServingInfoValid = FALSE;
    goto Done;
  }

  Done:
  return   cmServingInfoValid;
}

/*-------- policyman_report_camping_candidate --------*/
msgr_hdr_s * policyman_report_camping_candidate(
  policyman_cm_serving_info_t  *cmServingInfo,
  policyman_sandbox_t          *pSandbox
)
{
  msgr_hdr_s             *pMsg          = NULL;
  policyman_state_t      *pState;
  mre_set_t              *pClonedDbSet;
  policyman_stack_info_t *pStack;


  if (FALSE == policyman_camping_info_is_valid(cmServingInfo))
  {
    goto Done;
  }

  pState   = pSandbox->pState;
  pClonedDbSet = policyman_db_clone(pSandbox->pCurrentDB);

  pStack = policyman_ss_get_stack(pState, cmServingInfo->asubs_id, cmServingInfo->stack);


  pStack->plmn                 = cmServingInfo->plmn;
  pStack->sys_mode             = cmServingInfo->sys_mode;
  pStack->stackId              = cmServingInfo->stack;
  pStack->is_operational       = TRUE;
  pStack->radio_is_operational = TRUE;
  pStack->serving_info_valid   = TRUE;
  pStack->srv_status           = cmServingInfo->srv_status;


  POLICYMAN_MSG_HIGH_2( "executing report camping candidate with subs %d stack %d", cmServingInfo->asubs_id, cmServingInfo->stack );

  /* Run a policy check if needed. We should return a non-NULL message
        in case preference is accepted and no policy change is done. In such
        scenarios MSG num_attach should be set to 0.
   */
  policyman_policies_run_policy_check(pSandbox, pClonedDbSet);

  pMsg = policyman_cfgitem_get_change_msg_hdr_with_allocate( pState,
                                                             pClonedDbSet,
                                                             pSandbox->pCurrentDB,
                                                             FALSE );

  MRE_RELEASE_IF(pClonedDbSet);

Done:
  POLICYMAN_MSG_HIGH_1("returning to caller of policyman_report_camping_candidate(), policy changed (T/F) %d", (pMsg != NULL));
  return pMsg;
}


/*-------- policyman_execute_process_stack_report --------*/
void policyman_execute_process_stack_report(
  policyman_cmd_t *pCmd
)
{
  policyman_report_operational_stack_cmd_t *pOperStacksCmd = (policyman_report_operational_stack_cmd_t *) pCmd;
  policyman_sandbox_t                      *pSandbox       = pCmd->pSandbox;
  policyman_ss_info_t                       copiedSsInfo;
  policyman_stack_info_t                   *pCopiedStackInfo;
  size_t                                    stackId;

  copiedSsInfo = *policyman_state_get_serving_system_info( pSandbox->pState,
                                                           pOperStacksCmd->subsId );

  // update radio stack operational state
  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    uint32  stackMask;

    pCopiedStackInfo = &copiedSsInfo.stack[stackId];

    stackMask = (1 << stackId);

    pCopiedStackInfo->radio_is_operational = ((pOperStacksCmd->stackMask & stackMask) != 0);
  }

  policyman_fullrat_config_reset_timer_expired_per_subs(pOperStacksCmd->subsId);
  policyman_fullrat_config_reset_timer_expired_post_per_subs(pOperStacksCmd->subsId);

  policyman_ss_update_state( pSandbox,
                             &copiedSsInfo,
                             pOperStacksCmd->subsId );
}


/*-------- policyman_report_operational_radio_stacks --------*/
void policyman_report_operational_radio_stacks(
  sys_modem_as_id_e_type subsId,
  uint32                 stackMask
)
{
  policyman_report_operational_stack_cmd_t *pCmd;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman_report_operational_radio_stacks: stackMask 0x%04x",
                        subsId,
                        stackMask );

  pCmd = POLICYMAN_CMD_NEW( policyman_report_operational_stack_cmd_t,
                            policyman_execute_process_stack_report,
                            NULL,
                            NULL );

  pCmd->subsId    = subsId;
  pCmd->stackMask = stackMask;

  policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
  ref_cnt_obj_release(pCmd);
}

/*-------- policyman_serving_system_service_domain_has --------*/
boolean policyman_serving_system_service_domain_has(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  sys_srv_domain_e_type   service_domain_pref
)
{
  boolean                       srvDomainMatched = FALSE;
  policyman_stack_info_t const *pStack;
  size_t                        stackId;

  //  Check if the service domain matches in any of the sub's stacks.
  for (stackId = 0; !srvDomainMatched && stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    pStack = policyman_ss_get_stack(pState, subsId, stackId);

    srvDomainMatched = ( (pStack->radio_is_operational) && (service_domain_pref == pStack->srv_domain) );
  }

  return srvDomainMatched;
}

/*-------- policyman_execute_process_acq_fail_reset --------*/
void policyman_execute_process_acq_fail_reset(
  policyman_cmd_t *pCmd
)
{

  policyman_report_operational_stack_cmd_t *pAcqFailCmd = (policyman_report_operational_stack_cmd_t*) pCmd;
  sys_modem_stack_id_e_type                 stackId;

  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    if( (pAcqFailCmd->stackMask & BM(stackId)) != 0 )
    {
      policyman_ss_reset_acq_fail_cnt(pAcqFailCmd->pSandbox->pState, pAcqFailCmd->subsId, stackId);
    }
  }
}

/*-------- policyman_reset_acq_fail --------*/
void policyman_reset_acq_fail(
  sys_modem_as_id_e_type  subs_id,
  uint8                   stack_mask
)
{
  policyman_report_operational_stack_cmd_t *pCmd;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman_reset_acq_fail: stack mask 0x%x",
                        subs_id,
                        stack_mask );

  pCmd = POLICYMAN_CMD_NEW( policyman_report_operational_stack_cmd_t,
                            policyman_execute_process_acq_fail_reset,
                            NULL,
                            NULL );

  pCmd->subsId    = subs_id;
  pCmd->stackMask = stack_mask;

  policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
  ref_cnt_obj_release(pCmd);
}

/*-------- policyman_report_acq_fail_on_stack --------*/
STATIC void policyman_report_acq_fail_on_stack(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId,
  sys_modem_stack_id_e_type  stackId
)
{
  policyman_stack_info_t *pStackInfo;

  pStackInfo = policyman_ss_get_stack(pState, subsId, stackId);
  pStackInfo->acq_fail_cnt++;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman_report_acq_fail_on_stack: stackId %d",
                        subsId,
                        stackId );
}


/*-------- policyman_report_acq_fail_on_subs --------*/
void policyman_report_acq_fail_on_subs(
  policyman_state_t         *pState,
  sys_modem_as_id_e_type     subsId
)
{
  size_t                  stackId;

  for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
  {
    policyman_report_acq_fail_on_stack(pState, subsId, (sys_modem_stack_id_e_type)stackId);
  }
}

/*-------- policyman_validate_1x_rat_in_msg --------*/
STATIC boolean policyman_validate_1x_rat_in_msg(
  msgr_hdr_s             *pMsg,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_item_t            *pRc           = NULL;
  policyman_item_collection_t *pCollection;
  uint32                       ratCap;
  policyman_status_t           status        = POLICYMAN_STATUS_ERR;
  boolean                      cdmaIsPresent = FALSE;

  if (NULL != pMsg)
  {
    pCollection = policyman_get_collection_from_msgr(pMsg);
    if (NULL != pCollection)
    {
      pRc = policyman_get_collection_item( (policyman_item_collection_t const *)pCollection, 
                                           POLICYMAN_ITEM_RAT_CAPABILITY, 
                                           subsId );

      // release the reference on collection after usage
      ref_cnt_obj_release(pCollection);
      status = policyman_get_rat_capability(pRc, &ratCap);
    }
  }

  // get RAT cap from DB if not found in collection
  if (POLICYMAN_FAILED(status))
  {
    status = policyman_get_rat_capability_for_subs(subsId, &ratCap);
  }

  if (POLICYMAN_SUCCEEDED(status))
  {
    cdmaIsPresent = ((ratCap & SYS_SYS_MODE_MASK_CDMA) != 0);
  }

  return cdmaIsPresent;
}

/*-------- policyman_1x_mcc_is_allowed_to_camp --------*/
boolean policyman_1x_mcc_is_allowed_to_camp(
  sys_mcc_type             mcc,
  sys_modem_as_id_e_type   subsId,
  policyman_sandbox_t     *pSandbox
)
{
  boolean                 allowedToCamp = TRUE;
  msgr_hdr_s             *pMsg          = NULL;
  mre_set_t              *pClonedDbSet;
  policyman_state_t      *pState;
  policyman_stack_info_t *pStack1;
  policyman_stack_info_t *pStack2;

  if (policyman_is_initializing())
  {
    POLICYMAN_MSG_HIGH_0("Policyman is initializing");
    goto Done;
  }

  pState   = pSandbox->pState;
  pClonedDbSet = policyman_db_clone(pSandbox->pCurrentDB);

  pStack1 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_1);
  pStack2 = policyman_ss_get_stack(pState, subsId, SYS_MODEM_STACK_ID_2);

  // Process candidate MCC as limited GSM service on stack 1, no SS info on stack 2
  pStack1->mcc_list_3gpp2.count       = 1;
  pStack1->mcc_list_3gpp2.mcc_list[0] = mcc;
  pStack1->mnc_3gpp2                  = 0;

  pStack1->sys_mode             = SYS_SYS_MODE_CDMA;
  pStack1->is_operational       = TRUE;
  pStack1->radio_is_operational = TRUE;
  pStack1->serving_info_valid   = TRUE;
  pStack1->srv_status           = SYS_SRV_STATUS_LIMITED;

  pStack2->is_operational       = FALSE;
  pStack2->radio_is_operational = FALSE;
  pStack2->serving_info_valid   = FALSE;

  POLICYMAN_MSG_HIGH_1( "executing policyman_1x_mcc_is_allowed_to_camp with subs %d", subsId );
  /* Run a policy check if needed. We should return a non-NULL message
        in case preference is accepted and no policy change is done. In such
        scenarios MSG num_attach should be set to 0.
   */
  policyman_policies_run_policy_check(pSandbox, pClonedDbSet);

  pMsg = policyman_cfgitem_get_change_msg_hdr_with_allocate( pState,
                                                             pClonedDbSet,
                                                             pSandbox->pCurrentDB,
                                                             FALSE );

  MRE_RELEASE_IF(pClonedDbSet);

  // detect if new policy has 1x in its RAT capability
  allowedToCamp = policyman_validate_1x_rat_in_msg(pMsg, subsId);
  policyman_report_msg_free(pMsg);

  Done:
  POLICYMAN_MSG_HIGH_1("returning to caller of policyman_1x_mcc_is_allowed_to_camp(), allowedToCamp(T/F) %d", allowedToCamp);
  return allowedToCamp;
}

/*-------- policyman_ss_get_sim_state --------*/
sys_sim_state_e_type policyman_ss_get_sim_state(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_ss_info_t   *pSSInfo;
  
  pSSInfo = policyman_state_get_serving_system_info(pState, subsId);

  return pSSInfo->sim_state;
}

/*-------- policyman_ss_update_sim_state --------*/
STATIC void policyman_ss_update_sim_state(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  sys_sim_state_e_type    simState
)
{
  policyman_ss_info_t   *pSSInfo;
  
  pSSInfo = policyman_state_get_serving_system_info(pState, subsId);

  if (pSSInfo->sim_state != simState)
  {
    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "updating sim_state from %d to %d",
                          subsId,
                          pSSInfo->sim_state,
                          simState );

    pSSInfo->sim_state = simState;
  }
}

/*-------- policyman_ss_sim_state_changed --------*/
STATIC boolean policyman_ss_sim_state_changed(
  sys_modem_as_id_e_type   subsId,
  sys_sim_state_e_type     simState
)
{
  policyman_ss_info_t   *pSSInfo;
  policyman_state_t     *pState = policyman_state_get_state();

  pSSInfo = policyman_state_get_serving_system_info(pState, subsId);
  
  return pSSInfo->sim_state != simState;
}

/*-------- policyman_execute_process_sim_state_rpt --------*/
STATIC void policyman_execute_process_sim_state_rpt(
  policyman_cmd_t *pCmd
)
{
  mre_set_t                         *pItemSet;
  policyman_report_sim_state_cmd_t  *pSimStateCmd = (policyman_report_sim_state_cmd_t *) pCmd;
  policyman_sandbox_t               *pSandbox     = pSimStateCmd->pSandbox;

  pItemSet = policyman_itemset_new();

  policyman_state_enter_crit_section();

  policyman_ss_update_sim_state( pSandbox->pState, 
                                 pSimStateCmd->subsId, 
                                 pSimStateCmd->simState);

  policyman_policies_run_policy_check(pSandbox, pItemSet);
  policyman_cfgitem_update_items( pSandbox->pState, 
                                  pItemSet, 
                                  &pCmd->clientMsg);
  ref_cnt_obj_release(pItemSet);
        
  policyman_state_leave_crit_section();
}

/*-------- policyman_report_sim_state --------*/
msgr_hdr_s * policyman_report_sim_state(
  sys_modem_as_id_e_type subsId,
  sys_sim_state_e_type   simState
)
{
  policyman_report_sim_state_cmd_t  *pCmd;
  boolean                            simStateChanged = FALSE;  
  msgr_hdr_s                        *pMsg   = NULL;


  if (policyman_is_initializing())
  {
    POLICYMAN_MSG_HIGH_0("Policyman is initializing...ignoring sim_state report");
    goto Done;
  }

  simStateChanged = policyman_ss_sim_state_changed(subsId, simState);
  if (simStateChanged)
  {
    pCmd = POLICYMAN_CMD_WITH_HOOK_NEW( policyman_report_sim_state_cmd_t,
                                        policyman_execute_process_sim_state_rpt,
                                        NULL,
                                        NULL,
                                        policyman_cfgitem_update_notification_hook );
    pCmd->subsId   = subsId;
    pCmd->simState = simState;

    POLICYMAN_MSG_HIGH_0("blocking caller of policyman_report_sim_state()");
    pCmd->signal = policyman_client_block_sig_get();
    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    policyman_client_block_signal_wait(pCmd->signal);

    pMsg = pCmd->clientMsg.pMsg;
    ref_cnt_obj_release(pCmd);
  }

Done:
  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "return from policyman_report_sim_state, simStateChanged %d, simState %d",
                        subsId,
                        simStateChanged,
                        simState);
  return pMsg;
}

QSH_MDUMP_FN_ATTR_PM void policyman_ssinfo_mdump(
    policyman_ss_info_t *pSsInfo
)
{
  policyman_qsh_dump_collect((void *)pSsInfo, sizeof(policyman_ss_info_t));
}
