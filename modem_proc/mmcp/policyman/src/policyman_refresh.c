/**
  @file policyman_refresh.c

  @brief  Handling for PM refresh on SIM swap
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_refresh.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "policyman_i.h"
#include "policyman_refresh.h"
#include "policyman_task.h"
#include "policyman_policies.h"
#include "policyman_fullrat_config.h"
#include "policyman_serving_system.h"



#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*=============================================================================
  MCFG Refresh Event callback definition.
=============================================================================*/

static boolean in_refresh = FALSE;

typedef struct
{
  POLICYMAN_CMD_HDR;

  mcfg_refresh_info_s_type info;
} policyman_refresh_evt_cb_t;


/*-------- policyman_resume_after_refresh --------*/
void policyman_resume_after_refresh(
  policyman_sandbox_t *pSandbox,
  mre_set_t           *pItemSet
)
{
  size_t           numClients;

  numClients = policyman_resume_internal(POLICYMAN_CLIENT_POLICYMAN);

  if (numClients == 0)
  {
    policyman_policies_run_policy_check(pSandbox, pItemSet);
  }
}

/*===========================================================================
  FUNCTION POLICYMAN_EXECUTE_PROCESS_MCFG_REFRESH_EVT()

  DESCRIPTION
    Execute mcfg sim refresh event

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void policyman_execute_process_mcfg_refresh_evt(
  policyman_cmd_t *pCmd
)
{
  policyman_refresh_evt_cb_t *refresh_evt    = (policyman_refresh_evt_cb_t *) pCmd;
  policyman_sandbox_t        *pSandbox       = pCmd->pSandbox;
  policyman_state_t          *pState         = pSandbox->pState;
  mcfg_refresh_info_s_type    refresh_info   = refresh_evt->info;
  mre_set_t                  *pItemSet;
  policyman_cmph_info_t      *pCmPhInfo;
  policyman_ss_info_t        *pSsInfo;
  size_t                      subs;

  if (policyman_any_test_mode_is_enabled())
  {
    mcfg_refresh_done(refresh_info.reference_id);
    POLICYMAN_MSG_MED_0("Policyman in test mode, no refresh action needed. Refresh done sent to MCFG.");
    return;
  }

  pItemSet  = policyman_itemset_new(); 
  subs      = refresh_info.sub_index;
  in_refresh = TRUE;

  policyman_suspend_by_client(POLICYMAN_CLIENT_POLICYMAN);

  pCmPhInfo = policyman_state_get_cm_ph_state(pState, subs);
  
  policyman_state_enter_crit_section();

  // Clear out cfgitem and per-subs state information
  policyman_per_subs_deinit(subs);
  
  // Restart policyman info -- re-read everything due to a potential new xmls
  
  policyman_cmph_nv_init(&pCmPhInfo, subs);
  policyman_ss_reset_acq_fail_cnt_per_subs(pState, subs);

  policyman_ss_reset_user_plmn(pState, subs);
  policyman_fullrat_config_init_carrier_per_subs(subs);

  // re-read policy for subs
  policyman_policies_per_subs_init(pState, pItemSet, subs);
  policyman_cfgitem_ensure_items_per_subs(pState, pItemSet, subs);
  policyman_set_policy_initialized(TRUE);

  policyman_state_leave_crit_section();

  // let mcfg know were done
  if ( mcfg_refresh_done(refresh_info.reference_id) )
  {
    POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Refresh done notification sent to MCFG", refresh_info.sub_index);
  }
  else
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "Refresh done notification to MCFG failed", refresh_info.sub_index);
  }

  policyman_resume_after_refresh(pSandbox, pItemSet);
  policyman_dbupdate_enter_idle_state();

  in_refresh =  FALSE;

  policyman_cfgitem_update_device_config_for_force_notification(pItemSet);
  policyman_cfgitem_update_items(pState, pItemSet, &pCmd->clientMsg);

  MRE_RELEASE_IF(pItemSet);
}

/*===========================================================================
  FUNCTION POLICYMAN_MCFG_REFRESH_CB()

  DESCRIPTION
    Handle MCFG SIM refresh callback

  PARAMETERS
    evt     : CM Phone EVENT type
    p_info : actual payload of CM Phone EVENT

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean policyman_mcfg_refresh_cb(
  mcfg_refresh_info_s_type *p_info
)
{
  policyman_refresh_evt_cb_t *pCmd = NULL;
  sys_modem_as_id_e_type      subsId;

  subsId = (sys_modem_as_id_e_type)p_info->sub_index;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "received mcfg sim refresh for sim/slot %d",
                        p_info->sub_index,
                        p_info->slot_index );

  if (policyman_subs_id_is_valid(subsId))
  {
    pCmd = POLICYMAN_CMD_NEW( policyman_refresh_evt_cb_t,
                              policyman_execute_process_mcfg_refresh_evt,
                              NULL,
                              NULL ); // No policy associated with CM Phone Event

    pCmd->info = *p_info;

    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    ref_cnt_obj_release(pCmd);

    return TRUE;
  }

  return FALSE;
}


/*------------------------------------------------------------------------
  FUNCTION POLICYMAN_REGISTER_FOR_MCFG_REFRESH()

  DESCRIPTION
    With this function, Policy Manager registers with MCFG refresh events as a client
--------------------------------------------------------------------------*/
void policyman_register_for_mcfg_refresh(
  void
)
{
  mcfg_refresh_registration_s_type  reg;
  boolean                           success = FALSE;

  reg.order     = MCFG_REFRESH_ORDER_30;
  reg.slot_mask = MCFG_REFRESH_INDEX_MASK_NONE;
  reg.sub_mask  = MCFG_REFRESH_INDEX_MASK_ALL;
  reg.cb        = policyman_mcfg_refresh_cb;

  success = mcfg_refresh_register(&reg);

  if (!success)
  {
    POLICYMAN_MSG_ERROR_0("mcfg_refresh_register failed");
  }
}

/*------------------------------------------------------------------------
  FUNCTION policyman_in_refresh_phase()

  DESCRIPTION
    With this function, Policy Manager return refresh information
--------------------------------------------------------------------------*/
boolean policyman_in_refresh_phase(
  void
)
{
  return in_refresh;
}

#endif
