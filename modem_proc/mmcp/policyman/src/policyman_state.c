/**
  @file policyman_state.c

  @brief
*/

/*
    Copyright (c) 2013-2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_state.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_efs.h"
#include "policyman_cfgitem.h"
#include "policyman_call_events.h"
#include "policyman_device_config.h"
#include "policyman_efs.h"
#include "policyman_phone_events.h"
#include "policyman_plmn.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_serving_system.h"
#include "policyman_state.h"
#include "policyman_subs.h"
#include "policyman_task.h"
#include "policyman_timer.h"
#include "policyman_util.h"
#include "policyman_uim.h"
#include "trm.h"


#include <stringl/stringl.h>



static policyman_device_info_t   policyman_device_info;
static policyman_subs_state_t    policyman_subs_state_info[POLICYMAN_NUM_SUBS];
static policyman_global_info_t   policyman_global_info;

static policyman_state_t         policyman_state;

static char const               *pIMEI;
static byte                      rawIMEI[9];

static policyman_crit_sect_t     policyman_state_crit_sect;

void  policyman_state_imei_init(void);


/*=============================================================================
  General state APIs
=============================================================================*/

/*-------- policyman_state_retrieve_persisted_current_mcc --------*/
STATIC boolean policyman_state_retrieve_persisted_current_mcc(
  sys_mcc_type           *pCurrentMcc,
  sys_modem_as_id_e_type  subsId
)
{
  mre_efs_status_t        status;
  sys_mcc_type           *pMcc       = NULL;
  size_t                  mccSize    = 0;

  /* Initialize Current MCC to 0
   */
  *pCurrentMcc = 0;

  /*  Try and read from the NV item for Current MCC.
   */
  status = mre_efs_get_file( POLICYMAN_CURRENT_MCC_NV_PATH,
                             (void **)&pMcc,
                             &mccSize,
                             subsId);

  /*  If we were able to read the file set the MCC to that value.
         Otherwise just return.
   */
  if (    status == MRE_EFS_STATUS_SUCCESS
      &&  mccSize == sizeof(sys_mcc_type)
      &&  pMcc != NULL
     )
  {
    *pCurrentMcc = *pMcc;
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "current_mcc %d read from EFS",
                          subsId,
                          *pCurrentMcc );
  }

  POLICYMAN_MEM_FREE_IF(pMcc);
  return *pCurrentMcc != 0;
}


/*-------- policyman_state_notify_history_init --------*/
void policyman_state_notify_history_init(
  policyman_state_t *pState
)
{
  policyman_cfgitem_init_notify_history(pState);
}

/*-------- policyman_state_init --------*/
void policyman_state_init(
  void
)
{
  size_t subs;

  policyman_init_crit_sect(&policyman_state_crit_sect);

  /* Init Policyman device state, subs state and global state
  */
  policyman_state.pDeviceInfo = &policyman_device_info;
  memset(policyman_state.pDeviceInfo, 0x00, sizeof(policyman_device_info_t));

  policyman_state.pGlobalInfo = &policyman_global_info;
  memset(policyman_state.pGlobalInfo, 0x00, sizeof(policyman_global_info_t));

  policyman_state_clear_hlos_mcc(&policyman_state);

  for (subs = 0; subs < POLICYMAN_NUM_SUBS; subs++)
  {
    policyman_state.pSubsState[subs] = &policyman_subs_state_info[subs];
    memset(policyman_state.pSubsState[subs], 0x00, sizeof(policyman_subs_state_t));
    policyman_state.pSubsState[subs]->imsiVerified = TRUE;
    policyman_state_set_simlock_enabled(&policyman_state, subs, TRUE);
  }

  (void) policyman_state_ss_init(&policyman_state);
  (void) policyman_state_uim_init(&policyman_state);
  (void) policyman_state_cmcall_init(&policyman_state);
  (void) policyman_state_network_init(&policyman_state);

  /* Do SUBS init before Phone init since we need memory for subscription allocated before we
       evaluate multimode subscription
    */
  (void) policyman_state_subs_init(&policyman_state);
  (void) policyman_state_cmph_init(&policyman_state);

  (void) policyman_state_notify_history_init(&policyman_state);
}


/*-------- policyman_state_static_init --------*/
void policyman_state_static_init(
  void
)
{
  policyman_state_imei_init();
}


/*-------- policyman_state_deinit --------*/
void policyman_state_deinit(
  void
)
{
  size_t i;

  policyman_subs_deinit(policyman_state.pDeviceInfo->pSubsInfo);

  for (i = 0; i < POLICYMAN_NUM_SUBS; i++)
  {
    policyman_cmph_deinit(policyman_state.pSubsState[i]->pCmPhInfo);
    policyman_network_deinit(policyman_state.pSubsState[i]->pNetworkInfo);
    policyman_cmcall_deinit(policyman_state.pSubsState[i]->pCmCallInfo);
    policyman_uim_deinit(policyman_state.pSubsState[i]->pUimInfo);
    policyman_ss_deinit(policyman_state.pSubsState[i]->pSsInfo);
  }

  policyman_cmcall_deinit_history(&policyman_state);
  policyman_cmss_deinit_history(&policyman_state);
  policyman_cmph_deinit_history(&policyman_state);
  policyman_cfgitem_deinit_notify_history(&policyman_state);
  memset(&policyman_state, 0, sizeof(policyman_state));

  policyman_deinit_crit_sect(&policyman_state_crit_sect);
}


/*-------- policyman_state_static_deinit --------*/
void policyman_state_static_deinit(
  void
)
{
  POLICYMAN_MEM_FREE_IF(pIMEI);
}


/*-------- policyman_state_enter_crit_section --------*/
void policyman_state_enter_crit_section(
  void
)
{
  policyman_enter_crit_sect(&policyman_state_crit_sect);
}


/*-------- policyman_state_leave_crit_section --------*/
void policyman_state_leave_crit_section(
  void
)
{
  policyman_leave_crit_sect(&policyman_state_crit_sect);
}

/*-------- policyman_state_cleanup_after_policy_run --------*/
void policyman_state_cleanup_after_policy_run(
  policyman_state_t      *pState
)
{
  sys_modem_as_id_e_type subsId;
  size_t                 numSim = 0;

  (void) policyman_get_current_num_sim(&numSim);

  policyman_state_enter_crit_section();

  for (subsId = SYS_MODEM_AS_ID_1; subsId < numSim; subsId++)
  {
    policyman_uim_set_sim_refresh(pState, subsId, FALSE);
  }

  policyman_state_leave_crit_section();
}

/*-------- policyman_state_deep_copy_global --------*/
STATIC policyman_global_info_t * policyman_state_deep_copy_global(
  void
)
{
  policyman_global_info_t *pGlobalInfo;

  pGlobalInfo  = (policyman_global_info_t *) policyman_mem_alloc(sizeof(policyman_global_info_t));
  *pGlobalInfo = *policyman_state.pGlobalInfo;

  return pGlobalInfo;
}


/*-------- policyman_state_deep_copy_device --------*/
STATIC policyman_device_info_t * policyman_state_deep_copy_device(
  void
)
{
  policyman_device_info_t *pDeviceInfo;

  pDeviceInfo = (policyman_device_info_t *) policyman_mem_alloc(sizeof(policyman_device_info_t));

  pDeviceInfo->pSubsInfo = policyman_subs_info_deep_copy(policyman_state.pDeviceInfo->pSubsInfo);

  return pDeviceInfo;
}

/*-------- policyman_state_deep_copy_subs --------*/
STATIC policyman_subs_state_t * policyman_state_deep_copy_subs(
  size_t subs
)
{
  policyman_subs_state_t *pSubsState;

  pSubsState = (policyman_subs_state_t *) policyman_mem_alloc(sizeof(policyman_subs_state_t));

  *pSubsState = *policyman_state.pSubsState[subs];


  pSubsState->pCmCallInfo = policyman_cmcall_info_deep_copy(policyman_state.pSubsState[subs]->pCmCallInfo);
  pSubsState->pCmPhInfo   = policyman_cmph_info_deep_copy(policyman_state.pSubsState[subs]->pCmPhInfo);
  pSubsState->pNetworkInfo = policyman_network_info_deep_copy(policyman_state.pSubsState[subs]->pNetworkInfo);
  pSubsState->pSsInfo = policyman_ss_info_deep_copy(policyman_state.pSubsState[subs]->pSsInfo);
  pSubsState->pUimInfo = policyman_uim_info_deep_copy(policyman_state.pSubsState[subs]->pUimInfo);

  return pSubsState;
}


/*-------- policyman_state_clone --------*/
policyman_state_t * policyman_state_clone(
  void
)
{
  policyman_state_t *pStateClone;
  size_t             subs;

  pStateClone = (policyman_state_t *) policyman_mem_alloc(sizeof(policyman_state_t));

  policyman_state_enter_crit_section();

  pStateClone->pGlobalInfo = policyman_state_deep_copy_global();
  pStateClone->pDeviceInfo = policyman_state_deep_copy_device();

  for (subs = 0; subs < POLICYMAN_NUM_SUBS; subs++)
  {
    pStateClone->pSubsState[subs] = policyman_state_deep_copy_subs(subs);
  }

  policyman_state_leave_crit_section();

  return pStateClone;
}


/*-------- policyman_state_delete_device --------*/
void policyman_state_delete_device(
  policyman_device_info_t *pDevice
)
{
  policyman_subs_deinit(pDevice->pSubsInfo);
}


/*-------- policyman_state_delete_subs --------*/
void policyman_state_delete_subs(
  policyman_subs_state_t *pSubs
)
{
  policyman_cmph_deinit(pSubs->pCmPhInfo);
  policyman_network_deinit(pSubs->pNetworkInfo);
  policyman_cmcall_deinit(pSubs->pCmCallInfo);
  policyman_uim_deinit(pSubs->pUimInfo);
  policyman_ss_deinit(pSubs->pSsInfo);
}


/*-------- policyman_state_delete_clone --------*/
void policyman_state_delete_clone(
  policyman_state_t *pState
)
{
  int i;

  for (i = POLICYMAN_NUM_SUBS-1; i >= 0; i--)
  {
    policyman_subs_state_t * pSubsState = pState->pSubsState[i];

    policyman_state_delete_subs(pSubsState);
    POLICYMAN_MEM_FREE_IF(pSubsState);
  }

  policyman_state_delete_device(pState->pDeviceInfo);
  POLICYMAN_MEM_FREE_IF(pState->pDeviceInfo);

  POLICYMAN_MEM_FREE_IF(pState->pGlobalInfo);

  POLICYMAN_MEM_FREE_IF(pState);
}


/*-------- policyman_state_get_state --------*/
policyman_state_t * policyman_state_get_state(
  void
)
{
  return &policyman_state;
}


/*-------- policyman_state_get_global_state --------*/
policyman_global_info_t * policyman_state_get_global_state(
  policyman_state_t *pState
)
{
  return pState->pGlobalInfo;
}


/*-------- policyman_state_get_subs_state --------*/
policyman_subs_state_t * policyman_state_get_subs_state(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
)
{
  if (asubs_id >= ARR_SIZE(pState->pSubsState))
  {
    POLICYMAN_ERR_FATAL("subs ID %d exceeds pSubsState array bounds", asubs_id, 0, 0);
  }
  return pState->pSubsState[asubs_id];
}


/*-------- policyman_state_get_device_info --------*/
policyman_device_info_t * policyman_state_get_device_info(
  policyman_state_t *pState
)
{
  return pState->pDeviceInfo;
}


/*-------- policyman_state_get_subs_info --------*/
policyman_subs_info_t * policyman_state_get_subs_info(
  policyman_state_t *pState
)
{
  return pState->pDeviceInfo->pSubsInfo;
}

/*================================================================================
  Handle updates in PolicyMan state updates
==================================================================================*/

/*-------- policyman_state_handle_update --------*/
void policyman_state_handle_update(
  policyman_sandbox_t *pSandbox
)
{
  mre_set_t       *pItemSet;
  boolean          inTestMode;
  boolean          inFtm;
  boolean          inLimitedMode;

  if (policyman_can_policy_execute(pSandbox))
  {
    pItemSet = policyman_itemset_new();
    policyman_policies_run_policy_check(pSandbox, pItemSet);
    policyman_cfgitem_update_items(pSandbox->pState, pItemSet, NULL);
    ref_cnt_obj_release(pItemSet);
  }
}


/*=============================================================================
  Serving-system related APIs
=============================================================================*/
void policyman_state_ss_init(
  policyman_state_t *pState
)
{
  sys_modem_as_id_e_type  subs;

  for (subs = 0; subs < POLICYMAN_NUM_SUBS; subs++)
  {
    sys_mcc_type mcc = NO_MCC;

    policyman_ss_init(subs, &policyman_state_get_subs_state(pState, subs)->pSsInfo);
    policyman_state_retrieve_persisted_current_mcc(&mcc, subs);
    policyman_state_set_subs_mcc(pState, subs, mcc);
  }

  policyman_cmss_init_history(pState);
}


/*-------- policyman_state_get_serving_system_info --------*/
policyman_ss_info_t *policyman_state_get_serving_system_info(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_subs_state_t *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, subsId);

  return pSubsState->pSsInfo;
}


/*=============================================================================
  Phone-Event related APIs
=============================================================================*/
boolean policyman_state_cmph_init(
  policyman_state_t *pState
)
{
  size_t  subs;
  boolean retval;

  for (subs = 0; subs < POLICYMAN_NUM_SUBS; subs++)
  {
    retval = policyman_cmph_init(&policyman_state_get_subs_state(pState, subs)->pCmPhInfo, subs);
  }

  policyman_cmph_init_history(pState);
  return retval;
}


/*-------- policyman_state_cmph_nv_init --------*/
void policyman_state_cmph_nv_init(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
)
{
  policyman_cmph_nv_init(&policyman_state_get_subs_state(pState, asubs_id)->pCmPhInfo, asubs_id);
}

/*-------- policyman_state_set_imsi_verified --------*/
void policyman_state_set_imsi_verified(
  sys_modem_as_id_e_type  subsId,
  boolean                 verified
)
{
  boolean old = policyman_state.pSubsState[subsId]->imsiVerified;
  if (verified != old)
  {
    policyman_state.pSubsState[subsId]->imsiVerified = verified;
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "state imsiVerified set to %d",
              subsId,
        verified );
  }
}

/*-------- policyman_state_get_cm_ph_state --------*/
policyman_cmph_info_t * policyman_state_get_cm_ph_state(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
)
{
  policyman_subs_state_t *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, asubs_id);

  return pSubsState->pCmPhInfo;
}

 /*-------- policyman_state_reset_ss_info --------*/
static void policyman_state_reset_ss_info(
  policyman_state_t     *pState
)
{
  policyman_ss_info_t  *pSsInfo;
  size_t                stackId;
  size_t                subsId;

  for (subsId = 0; subsId < POLICYMAN_NUM_SUBS; subsId++)
  {
    pSsInfo = policyman_state_get_serving_system_info(pState, subsId);

    for (stackId = SYS_MODEM_STACK_ID_1; stackId < SYS_MODEM_STACK_ID_MAX; stackId++)
    {
      policyman_ss_set_default_value(pSsInfo, stackId);
    }
  }
}

/*-------- policyman_state_handle_oprt_mode --------*/
static void policyman_state_handle_oprt_mode(
  policyman_state_t     *pState,
  sys_oprt_mode_e_type  oprt_mode,
  boolean               updated_oprt_mode
)
{

  switch (oprt_mode)
  {
    case SYS_OPRT_MODE_ONLINE:
    {
      if ( updated_oprt_mode )
      {
        POLICYMAN_MSG_MED_0("oprt_mode changed to SYS_OPRT_MODE_ONLINE, print all configuration database");
        policyman_cfgitem_display_all();
      }
    }
    break;

    case SYS_OPRT_MODE_LPM:
    case SYS_OPRT_MODE_OFFLINE:
    policyman_state_reset_ss_info(pState);
    break;

    default:
    break;
  }//switch (oprt_mode)


  policyman_policy_leave_ftm();
}

/*-------- policyman_state_update_ph_oprt_mode --------*/
void policyman_state_update_ph_oprt_mode(
  policyman_sandbox_t  *pSandbox,
  sys_oprt_mode_e_type  oprt_mode
)
{
  size_t i;
  boolean updated_oprt_mode = FALSE;

  for (i = 0; i < POLICYMAN_NUM_SUBS; i++)
  {
    sys_modem_as_id_e_type  asubs_id   = (sys_modem_as_id_e_type) i;
    policyman_subs_state_t *pSubsState = policyman_state_get_subs_state(pSandbox->pState, asubs_id);

    updated_oprt_mode = policyman_ph_set_oprt_mode(pSubsState->pCmPhInfo, oprt_mode);
  }

  switch(oprt_mode)
  {
    case SYS_OPRT_MODE_FTM:
      policyman_policy_enter_ftm();
      break;

    default:
      policyman_state_handle_oprt_mode(pSandbox->pState, oprt_mode, updated_oprt_mode);
      break;
  }

  if(updated_oprt_mode)
  {
    policyman_state_handle_update(pSandbox);
  }
}

/*-------- policyman_state_update_time_in_lpm --------*/
void policyman_state_update_time_in_lpm(
  policyman_sandbox_t  *pSandbox,
  uint32                timeInLPM
)
{
  size_t idx;

  for (idx = 0; idx < POLICYMAN_NUM_SUBS; idx++)
  {
    sys_modem_as_id_e_type  subsId   = (sys_modem_as_id_e_type) idx;
    policyman_subs_state_t *pSubsState = policyman_state_get_subs_state(pSandbox->pState, subsId);

    policyman_ph_set_time_in_lpm(pSubsState->pCmPhInfo, timeInLPM);
  }
}


/*=============================================================================
  UIM related APIs
=============================================================================*/

/*-------- policyman_state_uim_init --------*/
boolean policyman_state_uim_init(
  policyman_state_t *pState
)
{
  simlock_imsi_type simlock_imsi     = {0};
  boolean           uimInitSucceeded = FALSE;
  size_t            subIdx;

  for (subIdx = 0; subIdx < POLICYMAN_NUM_SUBS; subIdx++)
  {
    uimInitSucceeded = policyman_uim_init(&policyman_state_get_subs_state(pState,
                                            (sys_modem_as_id_e_type)subIdx)->pUimInfo);

    policyman_simlock_set_imsi(pState, (sys_modem_as_id_e_type)subIdx, &simlock_imsi);
  }

  return uimInitSucceeded;
}


/*-------- policyman_state_get_uim_info --------*/
policyman_uim_info_t * policyman_state_get_uim_info(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  asubs_id
)
{
  policyman_subs_state_t *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, asubs_id);

  return pSubsState->pUimInfo;
}

/*=============================================================================
  Num Subs related APIs
=============================================================================*/
boolean policyman_state_subs_init(
  policyman_state_t *pState
)
{
  return policyman_subs_init(&pState->pDeviceInfo->pSubsInfo);
}

/*=============================================================================
  Call related APIs
=============================================================================*/
  /*-------- policyman_state_cmcall_init --------*/
boolean policyman_state_cmcall_init(
  policyman_state_t *pState
)
{
  size_t                  subs;
  boolean                 retval;

  for (subs = 0; subs < POLICYMAN_NUM_SUBS; subs++)
  {
    retval = policyman_cmcall_init(&policyman_state_get_subs_state(pState, subs)->pCmCallInfo, subs);
  }

  policyman_cmcall_init_history(pState);

  return retval;
}


/*-------- policyman_state_get_call_info --------*/
policyman_cmcall_info_t * policyman_state_get_call_info(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_subs_state_t *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, subsId);

  return pSubsState->pCmCallInfo;
}


/*-------- policyman_state_imei_init --------*/
void policyman_state_imei_init(
  void
)
{
  byte              *pImei = NULL;
  char               imei[16];
  char              *pDst;
  size_t             i;

  /*  Get the IMEI
 */
  nv_stat_enum_type result;
  nv_item_type      data;

  result = mre_efs_get_nv_item(NV_UE_IMEI_I, &data, SYS_MODEM_AS_ID_1);
  if (result == NV_DONE_S)
  {
    pImei = data.ue_imei.ue_imei;
  }

  /*  The IMEI buffer must start with a length byte of 8 and have 0x0a in
   *  the low nibble of the next byte.  If it doesn't, don't set IMEI.
   */
  if (pImei == NULL)
  {
    POLICYMAN_MSG_HIGH_0("Unable to read IMEI from NV");
    return;
  }

  if (*pImei != 0x08 || (pImei[1] & 0x0f) != 0x0a)
  {
    POLICYMAN_MSG_HIGH_0("IMEI read from NV has invalid format");
    policyman_device_imei_print(pImei);
    return;
  }

  /*  Copy the raw IMEI into the device information.
   */
  memscpy( rawIMEI,
           sizeof(rawIMEI),
           pImei,
           sizeof(rawIMEI) );

  ++pImei;

  /*  Get a pointer to the storage for the string we'll produce.
   */
  pDst = imei;

  /*  Get the first character of the IMEI.
   */
  *pDst++ = ((*pImei++) >> 4) + '0';

  /*  Get the rest of the characters.
   */
  for (i = 0 ; i < 6 ; ++i)
  {
    *pDst++ = (*pImei & 0x0f) + '0';
    *pDst++ = ((*pImei++) >> 4) + '0';
  }
  *pDst++ = (*pImei & 0x0f) + '0';
  *pDst = '\0';

  POLICYMAN_UT_MSG_1("IMEI is set to %s", imei);

  /*  Store a copy of the string in the device information.
   */
  pIMEI = policyman_str_dup(imei);
}


/*-------- policyman_state_get_imei --------*/
char const * policyman_state_get_imei(
  void
)
{
  return pIMEI;
}


/*-------- policyman_state_get_raw_imei --------*/
byte * policyman_state_get_raw_imei(
  void
)
{
  return rawIMEI;
}


/*=============================================================================
  Network Scan related APIs
=============================================================================*/
boolean policyman_state_network_init(
  policyman_state_t *pState
)
{
   size_t   subsIdx;
   boolean  retval;

   for (subsIdx = 0; subsIdx < POLICYMAN_NUM_SUBS; subsIdx++)
   {
     retval = policyman_network_init(&policyman_state_get_subs_state(pState, subsIdx)->pNetworkInfo, subsIdx);
   }

  return retval;
}

/*-------- policyman_state_update_network_scan_result --------*/
void policyman_state_update_network_scan_result(
  policyman_sandbox_t *pSandbox,
  cm_ph_info_s_type *pInfo
)
{
  policyman_network_info_t *pNetworkInfo;

  if (pInfo != NULL)
  {
    pNetworkInfo = policyman_state_get_network_info(pSandbox->pState, pInfo->asubs_id);

    /* Update the result and run rules
      */
    if (policyman_network_scan_update_result( pNetworkInfo,
                                             pInfo->asubs_id,
                                             pInfo->available_networks_list_cnf,
                                             &pInfo->available_networks )
       )
    {
      policyman_state_handle_update(pSandbox);
    }
  }

}

/*-------- policyman_state_get_network_info --------*/
policyman_network_info_t * policyman_state_get_network_info(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_subs_state_t *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, subsId);

  return pSubsState->pNetworkInfo;
}

/*=============================================================================
  HLOS MCC related APIs
=============================================================================*/

/*-------- policyman_state_current_mcc_update_to_efs --------*/
STATIC void policyman_state_current_mcc_update_to_efs(
  sys_mcc_type            mcc,
  sys_modem_as_id_e_type  subsId
)
{
  boolean  writeStatus;

  writeStatus = policyman_cfgitem_persist_item( POLICYMAN_CURRENT_MCC_NV_PATH,
                                                &mcc,
                                                sizeof(mcc),
                                                subsId );

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "current_mcc %d written to EFS, status %d",
                        subsId,
                        mcc,
                        writeStatus );
}

/*-------- policyman_state_update_hlos_mcc --------*/
void policyman_state_update_hlos_mcc(
  policyman_state_t *pState,
  sys_mcc_type       hlos_mcc,
  size_t             confidenceLevel,
  size_t             status
)
{
  policyman_global_info_t *pGlobal = policyman_state_get_global_state(pState);

  pGlobal->mcc             = hlos_mcc;
  pGlobal->confidenceLevel = confidenceLevel;
  pGlobal->status          = status;
}


/*-------- policyman_state_clear_hlos_mcc --------*/
void policyman_state_clear_hlos_mcc(
  policyman_state_t *pState
)
{
  policyman_state_update_hlos_mcc(pState, NO_MCC, 0, 0);
}


/*-------- policyman_state_get_hlos_mcc --------*/
sys_mcc_type policyman_state_get_hlos_mcc(
  policyman_state_t *pState
)
{
  policyman_global_info_t *pGlobal = policyman_state_get_global_state(pState);

  return pGlobal->mcc;
}

/*-------- policyman_state_set_subs_mcc --------*/
void policyman_state_set_subs_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  sys_mcc_type            mcc
)
{
  policyman_subs_state_t *pSubsState;
  sys_mcc_type            current_mcc;

  pSubsState  = policyman_state_get_subs_state(pState, subsId);
  current_mcc = policyman_state_get_subs_mcc(pState, subsId);

  if (mcc != current_mcc)
  {
    pSubsState->current_mcc = mcc;
    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "updating current_mcc from %d to %d",
                          subsId,
                          current_mcc,
                          mcc );

    policyman_state_current_mcc_update_to_efs(mcc, subsId);
  }
}

/*-------- policyman_state_update_subs_mcc --------*/
void policyman_state_update_subs_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  sys_mcc_type            mcc
)
{
  if (NO_MCC != mcc)
  {
    policyman_state_set_subs_mcc(pState, subsId, mcc);
  }
}

/*-------- policyman_state_get_subs_mcc --------*/
sys_mcc_type policyman_state_get_subs_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_subs_state_t *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, subsId);

  return pSubsState->current_mcc;
}

/*=============================================================================
  allow_1x_without_subs related APIs
=============================================================================*/

/*-------- policyman_state_init_1x_allowed_without_subs --------*/
STATIC void policyman_state_init_1x_allowed_without_subs(
  policyman_state_t  *pState
)
{
  boolean                  allow1x;
  policyman_global_info_t *pGlobal;

  pGlobal = policyman_state_get_global_state(pState);

  pGlobal->allow_1x_without_subs = FALSE;

  if (policyman_get_subs_boolean_value("pm:allow_1x_without_subs", SYS_MODEM_AS_ID_NONE, &allow1x))
  {
    pGlobal->allow_1x_without_subs = allow1x;
    POLICYMAN_MSG_HIGH_1("policyman state updated with 1x_allowed_without_subs status = %d", allow1x);
  }
}

/*-------- policyman_state_get_1x_allowed_without_subs --------*/
boolean policyman_state_get_1x_allowed_without_subs(
  policyman_state_t *pState
)
{
  static boolean           isInitialized = FALSE;
  policyman_global_info_t *pGlobal;

  if (!isInitialized)
  {
    (void)policyman_state_init_1x_allowed_without_subs(pState);
    isInitialized = TRUE;
  }

  pGlobal = policyman_state_get_global_state(pState);
  return pGlobal->allow_1x_without_subs;
}

/*-------- policyman_state_fill_diag_subs_info --------*/
void policyman_state_fill_diag_subs_info(
  sys_modem_as_id_e_type  asubs_id,
  pm_dump_subs_info_t    *dump_subs_info
)
{
  policyman_subs_state_t  *pSubsInfo;

  pSubsInfo   = &policyman_subs_state_info[asubs_id];

  dump_subs_info->asubs_id          = asubs_id;
  dump_subs_info->preconditions_met = 0;

  policyman_cmss_fill_diag_ss_info( pSubsInfo->pSsInfo,
                                    &dump_subs_info->cm_ss_info );

  policyman_cmph_fill_diag_ph_info( pSubsInfo->pCmPhInfo,
                                    &dump_subs_info->cm_ph_info );

  policyman_cmcall_fill_diag_call_info( asubs_id,
                                        pSubsInfo->pCmCallInfo,
                                        &dump_subs_info->cm_call_info );

  policyman_uim_fill_diag_uim_info( pSubsInfo->pUimInfo,
                                    &dump_subs_info->uim_info );
}

/*-------- policyman_state_get_imsi_verified --------*/
boolean policyman_state_get_imsi_verified(
  sys_modem_as_id_e_type  subsId
)
{
  return policyman_state.pSubsState[subsId]->imsiVerified;
}

/*-------- policyman_state_update_imsi_verified --------*/
void policyman_state_update_imsi_verified (
  sys_modem_as_id_e_type  subsId,
  boolean                 verified
)
{
  boolean old = policyman_state.pSubsState[subsId]->imsiVerified;
  if (verified != old)
  {
    policyman_state.pSubsState[subsId]->imsiVerified &= verified;
    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "state imsiVerified updated from %d to %d",
                          subsId,
        old,
        policyman_state.pSubsState[subsId]->imsiVerified );
  }
}

/*-------- policyman_state_set_simlock_enabled --------*/
boolean policyman_state_set_simlock_enabled(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  boolean                 enableSimlock
)
{
  boolean simlockIsEnabled = pState->pSubsState[subsId]->simlockIsEnabled;

  if (enableSimlock != simlockIsEnabled)
  {
    pState->pSubsState[subsId]->simlockIsEnabled = enableSimlock;

    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "policyman state updated with simlock status = %d",
                          subsId,
                          enableSimlock );
  }

  return enableSimlock != simlockIsEnabled;
}

/*-------- policyman_state_simlock_is_enabled --------*/
boolean policyman_state_simlock_is_enabled(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  return pState->pSubsState[subsId]->simlockIsEnabled;
}

/*-------- policyman_state_update_embms_status --------*/
void policyman_state_update_embms_status(
  msgr_hdr_s              *pMsg,
  void                    *pContext,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_subs_state_t         *pSubsState;
  policyman_sandbox_t            *pSandbox;
  lte_rrc_embms_active_sts_ind_s *pEmbmsMsg;

  pEmbmsMsg = (lte_rrc_embms_active_sts_ind_s *)pMsg;

  pSandbox = policyman_get_global_sandbox();

  pSubsState  = policyman_state_get_subs_state(pSandbox->pState, subsId);

  if (pEmbmsMsg->embms_is_active != pSubsState->embmsIsActive
      ||
      pEmbmsMsg->embms_plus_xrat_mask != pSubsState->embmsRatMask)
  {
    POLICYMAN_MSG_HIGH_5( SUBS_PREFIX "embms status: active  %d->%d, allowed rat mask %d->%d",
                          subsId,
                          pSubsState->embmsIsActive,
                          pEmbmsMsg->embms_is_active,
                          pSubsState->embmsRatMask,
                          pEmbmsMsg->embms_plus_xrat_mask );

    pSubsState->embmsIsActive = pEmbmsMsg->embms_is_active;
    pSubsState->embmsRatMask  = pEmbmsMsg->embms_plus_xrat_mask;

    policyman_state_handle_update(pSandbox);
  }
}


/*-------- policyman_state_get_embms_rat_mask --------*/
lte_rrc_embms_plus_xrat_mask_t policyman_state_get_embms_rat_mask(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_subs_state_t  *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, subsId);
  return pSubsState->embmsRatMask;
}


/*-------- policyman_state_embms_rat_is_restricted --------*/
boolean policyman_state_embms_rat_is_restricted(
  policyman_state_t     *pState,
  sys_sys_mode_e_type    ratToCheck,
  sys_modem_as_id_e_type ddsSub
)
{
  boolean                        isRestricted = FALSE;
  lte_rrc_embms_plus_xrat_mask_t embmsRatMask;

  embmsRatMask = policyman_state_get_embms_rat_mask(pState, ddsSub);

  if (ratToCheck == SYS_SYS_MODE_WCDMA)
  {
    isRestricted =  ((embmsRatMask & LTE_RRC_EMBMS_PLUS_W_MASK) == 0);
  }

  if (ratToCheck == SYS_SYS_MODE_LTE)
  {
    isRestricted =  ((embmsRatMask & LTE_RRC_EMBMS_PLUS_L_MASK) == 0);
  }

  if (ratToCheck == SYS_SYS_MODE_CDMA)
  {
    isRestricted =  ((embmsRatMask & LTE_RRC_EMBMS_PLUS_1X_MASK) == 0);
  }

  POLICYMAN_MSG_MED_3( SUBS_PREFIX "embms rat check %d, isRestricted %d",
                        ddsSub,
                        ratToCheck,
                        isRestricted );

  return isRestricted;
}


/*-------- policyman_state_embms_is_enabled --------*/
boolean policyman_state_embms_is_enabled(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_subs_state_t  *pSubsState;

  pSubsState = policyman_state_get_subs_state(pState, subsId);
  return pSubsState->embmsIsActive;
}

/*-----------------------------------------------------------------------------
  Embms Active
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;
} policyman_embms_active_condition_t;


/*-------- evaluate_embms_is_active --------*/
STATIC boolean evaluate_embms_is_active(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_state_t  *pState = POLICY_EXECUTE_STATE(pCtx);
  boolean             result = FALSE;

  result =  policyman_state_embms_is_enabled(pState, subsId);

  POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "condition <embms_is_active>  returns %d",
                      subsId,
                      LINE_NUM(pCondition),
                      result );

  return result;
}


/*-------- policyman_condition_embms_is_active_evaluate --------*/
STATIC boolean policyman_condition_embms_is_active_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_embms_is_active);
}


/*-------- policyman_condition_embms_is_active_new --------*/
mre_status_t policyman_condition_embms_is_active_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_condition_t          **ppCondition
)
{
  mre_status_t                           status = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_embms_active_condition_t    *pCondition;
  policyman_subs_t                       pm_subs;

  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_SUCCEEDED(status))
  {
    pCondition = POLICYMAN_CONDITION_NEW( policyman_embms_active_condition_t,
                                          pPolicy,
                                          policyman_condition_embms_is_active_evaluate,
                                          policyman_condition_dtor,
                                          pm_subs,
                                          pElem );

    *ppCondition = (mre_condition_t *) pCondition;

    status = MRE_STATUS_SUCCESS;
  }

  return status;
}

QSH_MDUMP_FN_ATTR_PM void policyman_state_mdump(
  void
)
{
  size_t i=0;
  /* collect subs info*/
  policyman_qsh_dump_collect((void *)policyman_state.pDeviceInfo, sizeof(policyman_device_info_t));
  policyman_subs_info_mdump(policyman_state.pDeviceInfo->pSubsInfo);

  for (; i<POLICYMAN_NUM_SUBS; i++)
  {
    policyman_qsh_dump_collect((void *)policyman_state.pSubsState[i], sizeof(policyman_subs_state_t));	
    policyman_qsh_dump_collect((void *)policyman_state.pSubsState[i]->pUimInfo, sizeof(policyman_uim_info_t));
    policyman_ssinfo_mdump(policyman_state.pSubsState[i]->pSsInfo);
    policyman_phinfo_mdump(policyman_state.pSubsState[i]->pCmPhInfo);
    policyman_callinfo_mdump(policyman_state.pSubsState[i]->pCmCallInfo);
  }  

  policyman_qsh_dump_collect((void *)policyman_state.pGlobalInfo, sizeof(policyman_global_info_t));
}
