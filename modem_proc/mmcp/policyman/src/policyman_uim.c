/**
  @file policyman_uim.c

  @brief
*/

/*
    Copyright (c) 2013-2019 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_uim.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_namedobj.h"
#include "mre_util_i.h"
#include "policyman_cfgitem.h"
#include "policyman_cm.h"
#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_plmn.h"
#include "policyman_policies.h"
#include "policyman_rules.h"
#include "policyman_state.h"
#include "policyman_subs.h"
#include "policyman_svc_mode.h"
#include "policyman_task.h"
#include "policyman_util.h"
#include "policyman_uim.h"
#include "policyman_xml.h"
#include "policyman_fullrat_config.h"
#include "pbmlib.h"

#include "mmgsdilib.h"
#include "mmgsdisessionlib.h"
#include "simlock_modem_lib.h" /* Included for SIMLOCK prototypes. */
#include <stringl/stringl.h>

#define IMSI_M_DATA_LEN (MMGSDI_IMSI_LEN + 1)

typedef enum
{
  POLICYMAN_FDN_STATUS_NONE,
  POLICYMAN_FDN_STATUS_ENABLED,
  POLICYMAN_FDN_STATUS_DISABLED
} policyman_fdn_status_t;


/*=============================================================================
   Structures for MMGSDI
=============================================================================*/
typedef struct
{
  mmgsdi_session_type_enum_type session_type;  /* the key */
  sys_modem_as_id_e_type        asubs_id;      /* we assign this */
  mmgsdi_session_id_type        session_id;    /* comes from mmgsdi */
  mmgsdi_app_enum_type          app_type;      /* comes from mmgsdi */
  boolean                       in_use;        /* we manage. true if session open */
  boolean                       active;        /* we manage. true if session active */
  mmgsdi_slot_id_enum_type      slot_id;
  policyman_fdn_status_t        fdn_status;
} pm_mmgsdi_session_id_table_entry_t;

static boolean is_mmgsdi_subscription_ready_received = FALSE;

static pm_mmgsdi_session_id_table_entry_t pm_mmgsdi_session_id_table[] = {
  {MMGSDI_GW_PROV_PRI_SESSION, SYS_MODEM_AS_ID_1, 0, MMGSDI_APP_NONE, FALSE, FALSE, MMGSDI_SLOT_NONE, POLICYMAN_FDN_STATUS_NONE},
  {MMGSDI_1X_PROV_PRI_SESSION, SYS_MODEM_AS_ID_1, 0, MMGSDI_APP_NONE, FALSE, FALSE, MMGSDI_SLOT_NONE, POLICYMAN_FDN_STATUS_NONE},
  {MMGSDI_GW_PROV_SEC_SESSION, SYS_MODEM_AS_ID_2, 0, MMGSDI_APP_NONE, FALSE, FALSE, MMGSDI_SLOT_NONE, POLICYMAN_FDN_STATUS_NONE},
  {MMGSDI_1X_PROV_SEC_SESSION, SYS_MODEM_AS_ID_2, 0, MMGSDI_APP_NONE, FALSE, FALSE, MMGSDI_SLOT_NONE, POLICYMAN_FDN_STATUS_NONE},
};

/*=============================================================================
  Function Prototype
=============================================================================*/

/*-------- pm_mmgsdi_reg_status_cb --------*/
static void pm_mmgsdi_reg_status_cb(
  mmgsdi_return_enum_type  status,
  mmgsdi_cnf_enum_type     data_type,
  const mmgsdi_cnf_type   *data_ptr
);


typedef void (*policyman_mmgsdi_callback_type) (
  mmgsdi_return_enum_type  status,
  mmgsdi_cnf_enum_type     cnf,
  const mmgsdi_cnf_type   *cnf_ptr
);

/*-------- sd_decode_3gpp2_mcc --------*/
EXTERN void sd_decode_3gpp2_mcc(
  word       *decoded_mcc,
  const word  encoded_mcc
);

/*=============================================================================
  Event callback definitions
=============================================================================*/

typedef struct{
  POLICYMAN_CMD_HDR;

  pbm_notify_data_s_type   notify_data;
} policyman_pbm_evt_cb_s;

typedef struct
{
  POLICYMAN_CMD_HDR;

  simlock_event_data_type simlock_event;
  mmgsdi_event_data_type  mmgsdi_event;
} policyman_uim_evt_cb_t;

typedef struct
{
   POLICYMAN_CMD_HDR;

   mmgsdi_return_enum_type status;
   mmgsdi_cnf_enum_type    data_type;
   mmgsdi_cnf_type         data;
} pm_mmgsdi_resp_t;

/*=============================================================================
  APIs for SIMLOCK information
=============================================================================*/

/*-------- policyman_simlock_set_imsi --------*/
void policyman_simlock_set_imsi(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  simlock_imsi_type       *pImsi
)
{
  policyman_uim_info_t *pUimInfo = policyman_state_get_uim_info(pState, subsId);

  pUimInfo->simlock_imsi = *pImsi;
}

/*-------- policyman_simlock_is_enabled --------*/
boolean policyman_simlock_is_enabled(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_uim_info_t     *pUimInfo;
  simlock_result_enum_type  simlockImsiVerifyStatus;

  pUimInfo                = policyman_state_get_uim_info(pState, subsId);
  simlockImsiVerifyStatus = simlock_verify_imsi_pre_policy(pUimInfo->simlock_imsi);

  return (   simlockImsiVerifyStatus == SIMLOCK_SUCCESS
          || simlockImsiVerifyStatus == SIMLOCK_UNSUPPORTED);

}

/*-------- policyman_evaluate_simlock_is_enabled_for_per_subs --------*/
boolean policyman_evaluate_simlock_is_enabled_for_per_subs(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type *subsId
)
{
  size_t  subsIdx;
  size_t  nSim;
  boolean updateRequired = FALSE;

  policyman_get_current_num_sim(&nSim);

  for (subsIdx = 0; subsIdx < nSim; subsIdx++)
  {
    boolean simlockIsEnabled;
    boolean simlockStateHasChanged;

    simlockIsEnabled = policyman_simlock_is_enabled(pState, (sys_modem_as_id_e_type)subsIdx);

    simlockStateHasChanged = policyman_state_set_simlock_enabled( pState,
                                                                  (sys_modem_as_id_e_type)subsIdx,
                                                                  simlockIsEnabled );

    updateRequired = updateRequired || simlockStateHasChanged;

    if (updateRequired)
    {
        *subsId = (sys_modem_as_id_e_type)subsIdx;
    }
  }

  return updateRequired;
}

/*-------- pm_process_simlock_evt --------*/
void pm_process_simlock_evt(
  policyman_cmd_t *pCmd
)
{
  simlock_event_data_type *pEvent = &((policyman_uim_evt_cb_t *)pCmd)->simlock_event;
  policyman_sandbox_t     *pSandbox  = pCmd->pSandbox;
  policyman_state_t       *pState    = pSandbox->pState;


  switch (pEvent->evt_type)
  {
    case SIMLOCK_UNLOCK_EVENT:
    case SIMLOCK_LOCK_EVENT:
    case SIMLOCK_TEMPORARY_UNLOCK_EVENT:
    {
      boolean                updateRequired;
      sys_modem_as_id_e_type subsId;

      updateRequired = policyman_evaluate_simlock_is_enabled_for_per_subs(pState, &subsId);

      if (updateRequired && policyman_uim_get_subs_active(pState, subsId))
      {
        policyman_state_handle_update(pSandbox);
      }
      break;
    }
    default:
      break;
  }
}

/*-------- policyman_uim_simlock_evt_cb --------*/
void policyman_uim_simlock_evt_cb(
  simlock_result_enum_type status,
  simlock_event_data_type  simlock_event
)
{
  policyman_uim_evt_cb_t *pCmd = NULL;

  POLICYMAN_MSG_HIGH_1("PM received SIMLOCK evt type %d", simlock_event.evt_type);

  pCmd = POLICYMAN_CMD_NEW( policyman_uim_evt_cb_t,
                            pm_process_simlock_evt,
                            NULL,
                            NULL );

  pCmd->simlock_event = simlock_event;

  policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
  ref_cnt_obj_release(pCmd);
}

/*-------- policyman_uim_simlock_verified --------*/
boolean policyman_uim_simlock_verified(
  simlock_imsi_type   imsi
)
{
  simlock_result_enum_type simlock_verify_status;

  simlock_verify_status = simlock_verify_imsi(imsi);

  POLICYMAN_MSG_HIGH_1("simlock_verified status: %d", simlock_verify_status);

  return (   simlock_verify_status == SIMLOCK_SUCCESS
          || simlock_verify_status == SIMLOCK_UNSUPPORTED );

}

/*-----------------------------------------------------------------------------
  Condition simlock_enabled
-----------------------------------------------------------------------------*/

/*-------- policyman_condition_simlock_enabled_evaluate --------*/
STATIC boolean policyman_condition_simlock_enabled_evaluate(
  mre_condition_t const  *pCondition,
  void                   *pCtx
)
{
  policyman_state_t      *pState                = POLICY_EXECUTE_STATE(pCtx);
  sys_modem_as_id_e_type  subsId                = ((policy_execute_ctx_t *)pCtx)->subsId;
  boolean                 simlock_is_enabled;

  simlock_is_enabled = policyman_state_simlock_is_enabled(pState, subsId);

  POLICYMAN_UT_MSG_2( SUBS_PREFIX "condition <simlock_enabled> returns %d",
                      subsId,
                      simlock_is_enabled );

  return simlock_is_enabled;
}

/*-------- policyman_condition_simlock_enabled_new --------*/
mre_status_t policyman_condition_simlock_enabled_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mre_status_t      status;
  mre_condition_t  *pCondition;
  policyman_subs_t  pm_subs     = SUBS_THIS;

  pCondition = POLICYMAN_CONDITION_NEW( mre_condition_t,
                                        pPolicy,
                                        policyman_condition_simlock_enabled_evaluate,
                                        policyman_condition_dtor,
                                        pm_subs,
                                        pElem );

  *ppCondition = pCondition;
  status       = MRE_STATUS_SUCCESS;

  POLICYMAN_MSG_HIGH_1("policyman_condition_simlock_enabled_new returned status %d", status);

  return status;
}

/*=============================================================================
  APIs for UIM information
=============================================================================*/

/*-------- policyman_uim_init --------*/
boolean policyman_uim_init(
  policyman_uim_info_t  **pUimInfo
)
{
  *pUimInfo = (policyman_uim_info_t *) policyman_mem_alloc(sizeof(policyman_uim_info_t));

  return TRUE;
}

/*-------- policyman_uim_deinit --------*/
void policyman_uim_deinit(
  policyman_uim_info_t  *pUimInfo
)
{
  POLICYMAN_MEM_FREE_IF(pUimInfo);
}

/*-------- policyman_uim_info_deep_copy --------*/
policyman_uim_info_t * policyman_uim_info_deep_copy(
  policyman_uim_info_t  *pUimState
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo = (policyman_uim_info_t *) policyman_mem_alloc(sizeof(policyman_uim_info_t));

  *pUimInfo = *pUimState;

  return pUimInfo;
}

/*-------- policyman_register_for_uim_events --------*/
void policyman_register_for_uim_events(
  void
)
{
  mmgsdi_return_enum_type  mmgsdi_status;
  simlock_result_enum_type simlock_status;

  is_mmgsdi_subscription_ready_received = FALSE;

  mmgsdi_status = mmgsdi_client_id_and_evt_reg( NULL,  // Not registering for card status events
                                                pm_mmgsdi_reg_status_cb,
                                                0 );
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    POLICYMAN_MSG_ERROR_1("mmgsdi_client_id_and_evt_reg failed %d", mmgsdi_status);
  }

  /* Register for events from SIMLOCK task */
  simlock_status = simlock_register_event(SIMLOCK_REGISTER, policyman_uim_simlock_evt_cb);

  if (simlock_status != SIMLOCK_SUCCESS)
  {
    POLICYMAN_MSG_ERROR_1("simlock_register_event failed %d", simlock_status);
  }
}


/*===========================================================================
  FUNCTION pm_mmgsdi_ssn_tbl_get_entry_by_type()

  DESCRIPTION
 ===========================================================================*/
static pm_mmgsdi_session_id_table_entry_t * pm_mmgsdi_ssn_tbl_get_entry_by_type(
  mmgsdi_session_type_enum_type session_type
)
{
  int                                  index;
  pm_mmgsdi_session_id_table_entry_t  *ssn_entry = NULL;


  for (index = 0; index < ARR_SIZE(pm_mmgsdi_session_id_table); index++)
  {
    if (   pm_mmgsdi_session_id_table[index].session_type == session_type
        && pm_mmgsdi_session_id_table[index].in_use == TRUE )
    {
      ssn_entry = &pm_mmgsdi_session_id_table[index];
      break;
    }
  }

  return ssn_entry;
}


/*===========================================================================
  FUNCTION policyman_map_pbm_to_mmgsdi_session_type()

  DESCRIPTION
 ===========================================================================*/
STATIC mmgsdi_session_type_enum_type policyman_map_pbm_to_mmgsdi_session_type(
  pbm_provision_enum_type  prov_type
)
{
   mmgsdi_session_type_enum_type session_type = MMGSDI_GW_PROV_PRI_SESSION;
  

  switch (prov_type)
  {
    case PBM_PROVISION_GW_PRIMARY:
      session_type = MMGSDI_GW_PROV_PRI_SESSION;
      break;
    case PBM_PROVISION_GW_SECONDARY:
      session_type = MMGSDI_GW_PROV_SEC_SESSION;
      break;
    case PBM_PROVISION_1X_PRIMARY:
      session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;
    case PBM_PROVISION_1X_SECONDARY:
      session_type = MMGSDI_1X_PROV_SEC_SESSION;
      break;
    default:
      session_type = MMGSDI_GW_PROV_PRI_SESSION;
      break;
  }

  return session_type;
}


/*-------- pm_process_fdn_evt --------*/
void pm_process_fdn_evt(
  policyman_cmd_t *pCmd
)
{
  pbm_notify_data_s_type              *pNotifyData    = &((policyman_pbm_evt_cb_s*)pCmd)->notify_data;

  if (pNotifyData->event == PBM_EVENT_PB_READY)
  {
    pm_mmgsdi_session_id_table_entry_t  *pSsnEntry;
    mmgsdi_session_type_enum_type        session_type;

    session_type = policyman_map_pbm_to_mmgsdi_session_type(pNotifyData->session_data.pb_id.prov_type);
    pSsnEntry    = pm_mmgsdi_ssn_tbl_get_entry_by_type(session_type);

    if ( NULL != pSsnEntry)
    {
      if ( pNotifyData->session_data.pb_id.fdn_status == TRUE )
      {
        pSsnEntry->fdn_status = POLICYMAN_FDN_STATUS_ENABLED;
      }
      else
      {
        pSsnEntry->fdn_status = POLICYMAN_FDN_STATUS_DISABLED;
      }
      policyman_state_handle_update(pCmd->pSandbox);
    }
  }
  else
  {
    policyman_state_handle_update(pCmd->pSandbox);
  }
}


/*-------- policyman_pbm_notify_events_cb --------*/
void policyman_pbm_notify_events_cb(
  void                   *user_data,
  pbm_notify_data_s_type *notify_data
)
{
  pbm_device_type             device_type = PBM_DEFAULT;
  policyman_pbm_evt_cb_s     *pCmd = NULL;
  boolean                     fdnUpdate = FALSE;

  POLICYMAN_MSG_HIGH_1( "PBM event %d",
                         notify_data->event );

  switch(notify_data->event)
  {
    case PBM_EVENT_REC_DELETE:
    case PBM_EVENT_REC_ADD:
    case PBM_EVENT_REC_UPDATE:
    {
      device_type = pbm_record_id_to_device_type(notify_data->session_data.rec_id);
    }
    break;

    case PBM_EVENT_PB_READY:
    {
      device_type = notify_data->session_data.pb_id.device_type;
    }
    break;

    default:
    break;
  }

  /* we are interested in FDN entries */
  if(device_type == PBM_FDN)
  {

    pCmd = POLICYMAN_CMD_NEW( policyman_pbm_evt_cb_s,
                              pm_process_fdn_evt,
                              NULL,
                              NULL );

    pCmd->notify_data = *notify_data;
    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    ref_cnt_obj_release(pCmd);
  }
}


/*-------- policyman_register_for_pbm_events --------*/
void policyman_register_for_pbm_events(
  void
)
{
  if(PBM_SUCCESS != pbm_notify_register(policyman_pbm_notify_events_cb, NULL))
  {
    POLICYMAN_MSG_ERROR_0( "registering for PBM notification failed" );
  }
}

/*-------- policyman_uim_handle_imsi_update --------*/
void policyman_uim_handle_imsi_update(
  policyman_state_t      *pState,
  mmgsdi_data_type        imsiData,
  sys_modem_as_id_e_type  subsId,
  mre_set_t              *pItemSet
)
{
  simlock_imsi_type            imsi;
  boolean                      imsi_verified;
  boolean                      imsi_verified_old;
  policyman_svc_mode_item_t   *pSvcItem;
  boolean                      simlockIsEnabled;

  /* Check if this IMSI is successfully SIMLOCK verified by SIMLOCK engine */
  imsi.imsi_len = memscpy( imsi.imsi,
                           sizeof(imsi.imsi),
                           imsiData.data_ptr,
                           imsiData.data_len );

  // Update SIMLOCK IMSI.
  policyman_simlock_set_imsi(pState, subsId, &imsi);

  // Check if IMSI is locked on the sub.
  simlockIsEnabled = policyman_simlock_is_enabled(pState, subsId);

  // Update SIMLOCK enable status for this sub.
  policyman_state_set_simlock_enabled(pState, subsId, simlockIsEnabled);

  // Get existing simlock status
  imsi_verified_old = policyman_state_get_imsi_verified(subsId);

  imsi_verified = policyman_uim_simlock_verified(imsi);
  policyman_state_update_imsi_verified(subsId, imsi_verified);

  imsi_verified = policyman_state_get_imsi_verified(subsId);

  if (imsi_verified != imsi_verified_old)
  {
    // Try to create FULL svc_mode item -- will be created as LIMITED if not verified
    pSvcItem = policyman_svc_mode_item_new(POLICYMAN_SVC_MODE_FULL, subsId);

    mre_set_add(pItemSet, &pSvcItem);
    ref_cnt_obj_release(pSvcItem);
  }
}




/*===========================================================================
  FUNCTION pm_mmgsdi_ssn_tbl_get_entry_by_id()

  DESCRIPTION
 ===========================================================================*/
STATIC pm_mmgsdi_session_id_table_entry_t * pm_mmgsdi_ssn_tbl_get_entry_by_id(
  mmgsdi_session_id_type        session_id
)
{
  int                                  index;
  pm_mmgsdi_session_id_table_entry_t  *ssn_entry = NULL;


  for (index = 0; index < ARR_SIZE(pm_mmgsdi_session_id_table); index++)
  {
    if (   (pm_mmgsdi_session_id_table[index].session_id == session_id)
        && (pm_mmgsdi_session_id_table[index].in_use == TRUE) )
    {
      ssn_entry = &pm_mmgsdi_session_id_table[index];
      break;
    }
  }

  return ssn_entry;
}


/*===========================================================================
  Policymanger's client ID for MMGSDI notifications
 ===========================================================================*/
static mmgsdi_client_id_type pm_mmgsdi_client_id = 0;

/*===========================================================================
  FUNCTION pm_open_session_cb()

  DESCRIPTION
 ===========================================================================*/
void pm_open_session_cb(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    data_type,
  const mmgsdi_cnf_type  *data_ptr
)
{
  uint16 sessionInfoIdx  = 0;
  uint16 sessionTableIdx = 0;

  /* data_ptr is NULL for ICC card where open_session returns success */
  if (   (NULL == data_ptr)
      || (NULL == data_ptr->session_open_ext_cnf.session_info_ptr) )
  {
    return;
  }

  if ( status == MMGSDI_SUCCESS && data_type == MMGSDI_SESSION_OPEN_EXT_CNF)
  {
    uint8 numSessions;

    numSessions = data_ptr->session_open_ext_cnf.num_sessions;

    for (sessionInfoIdx  = 0; sessionInfoIdx < data_ptr->session_open_ext_cnf.num_sessions; sessionInfoIdx++)
    {
      mmgsdi_session_open_info_type *sessionInfo;

      sessionInfo = &data_ptr->session_open_ext_cnf.session_info_ptr[sessionInfoIdx];

      for (sessionTableIdx = 0; sessionTableIdx < ARR_SIZE(pm_mmgsdi_session_id_table); sessionTableIdx++)
      {
        pm_mmgsdi_session_id_table_entry_t *sessionTableEntry;

        sessionTableEntry = &pm_mmgsdi_session_id_table[sessionTableIdx];

        if (sessionTableEntry->session_type == sessionInfo->session_type)
        {
          sessionTableEntry->session_id = sessionInfo->session_id;
          sessionTableEntry->app_type   = sessionInfo->app_info.app_type;
          sessionTableEntry->slot_id    = sessionInfo->slot_id;
          sessionTableEntry->in_use     = TRUE;

          POLICYMAN_MSG_HIGH_3( "num_sessions %d session_id %d session_type %d",
                                numSessions,
                                sessionTableEntry->session_id,
                                sessionTableEntry->session_type );
        }
      }
    }
  }
}

/*===========================================================================
  FUNCTION pm_open_sessions()

  DESCRIPTION
 ===========================================================================*/
static void pm_open_sessions(
  void
)
{
  mmgsdi_return_enum_type result;
  uint32                  session_type_mask = ( MMGSDI_GW_PROV_PRI_SESSION_MASK
                                              | MMGSDI_1X_PROV_PRI_SESSION_MASK
                                              | MMGSDI_GW_PROV_SEC_SESSION_MASK
                                              | MMGSDI_1X_PROV_SEC_SESSION_MASK );

  result = mmgsdi_session_open_ext( pm_mmgsdi_client_id,
                                    session_type_mask,
                                    NULL, // Not registering for session events
                                    FALSE,
                                    pm_open_session_cb,
                                    (mmgsdi_client_data_type)session_type_mask );

  if ( MMGSDI_SUCCESS != result )
  {
    /* log that error and return */
    POLICYMAN_MSG_ERROR_1("PM session open failed with error %d", result);
  }
}

/*===========================================================================
  FUNCTION PM_PROCESS_MMGSDI_REG_STATUS()

  DESCRIPTION
    MMGSDI process function
 ===========================================================================*/
void pm_process_mmgsdi_reg_status(
  policyman_cmd_t *pCmd
)
{
  pm_mmgsdi_resp_t *pMmgsdi_resp = (pm_mmgsdi_resp_t *)pCmd;

  if (pMmgsdi_resp->status == MMGSDI_SUCCESS)
  {
    if (pMmgsdi_resp->data_type== MMGSDI_CLIENT_ID_AND_EVT_REG_CNF)
    {
      pm_mmgsdi_client_id = pMmgsdi_resp->data.client_id_and_evt_reg_cnf.response_header.client_id;

      pm_open_sessions();
    }
  }
  else
  {
    switch (pMmgsdi_resp->data_type)
    {
      case MMGSDI_CLIENT_ID_AND_EVT_REG_CNF:
        POLICYMAN_MSG_ERROR_2( "MMGSDI failed registering PM client ID & EVT registration.  Status %d, Confirmation %d",
                               pMmgsdi_resp->status,
                               pMmgsdi_resp->data_type );
        break;

      default:
        POLICYMAN_MSG_ERROR_2( "MMGSDI returned Failure async. Status %d, Confirmation %d",
                               pMmgsdi_resp->status,
                               pMmgsdi_resp->data_type );
        break;
    }
  }
}


/*===========================================================================
  FUNCTION policyman_map_mmgsdi_to_pbm_session_type()

  DESCRIPTION
 ===========================================================================*/
STATIC pbm_provision_enum_type policyman_map_mmgsdi_to_pbm_session_type(
  mmgsdi_session_type_enum_type  session_type
)
{
  pbm_provision_enum_type prov_type = PBM_PROVISION_UNKNOWN;

  switch (session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      prov_type = PBM_PROVISION_GW_PRIMARY;
      break;
    case MMGSDI_GW_PROV_SEC_SESSION:
      prov_type = PBM_PROVISION_GW_SECONDARY;
      break;
    case MMGSDI_1X_PROV_PRI_SESSION:
      prov_type = PBM_PROVISION_1X_PRIMARY;
      break;
    case MMGSDI_1X_PROV_SEC_SESSION:
      prov_type = PBM_PROVISION_1X_SECONDARY;
      break;
    default:
      prov_type = PBM_PROVISION_UNKNOWN;
      break;
  }

  return prov_type;
}


/*===========================================================================
  FUNCTION pm_mmgsdi_ssn_tbl_get_gw_session_by_subId()

  DESCRIPTION
 ===========================================================================*/
STATIC pm_mmgsdi_session_id_table_entry_t * pm_mmgsdi_ssn_tbl_get_gw_session_by_subId(
  sys_modem_as_id_e_type subs_id
)
{
  int                                  index;
  pm_mmgsdi_session_id_table_entry_t  *ssn_entry = NULL;


  for (index = 0; index < ARR_SIZE(pm_mmgsdi_session_id_table); index++)
  {
    if ( pm_mmgsdi_session_id_table[index].asubs_id == subs_id 
         && pm_mmgsdi_session_id_table[index].in_use
         && ( pm_mmgsdi_session_id_table[index].session_type == MMGSDI_GW_PROV_PRI_SESSION
              || pm_mmgsdi_session_id_table[index].session_type == MMGSDI_GW_PROV_SEC_SESSION )
       )
    {
      ssn_entry = &pm_mmgsdi_session_id_table[index];
      break;
    }
  }

  return ssn_entry;
}

/*===========================================================================
  FUNCTION print_session_info()
 ===========================================================================*/
static void print_session_info(
  pm_mmgsdi_session_id_table_entry_t *ssn_entry
)
{
  POLICYMAN_MSG_MED_5( "session identity: session_type %d, subs %d, app_type %d, active %d, in_use %d",
                       ssn_entry->session_type,
                       ssn_entry->asubs_id,
                       ssn_entry->app_type,
                       ssn_entry->active,
                       ssn_entry->in_use );
}

/*===========================================================================
  Data Structure for READ CNF from MMGSDI
 ===========================================================================*/

#define PM_MMGSDI_RPM_AD_SIZE           4 /* EF-AD */


/*-------- sim_session_is_gw --------*/
STATIC boolean sim_session_is_gw(
  mmgsdi_session_type_enum_type session
)
{
  return    session == MMGSDI_GW_PROV_PRI_SESSION
         || session == MMGSDI_GW_PROV_SEC_SESSION;
}

/*-------- sim_session_is_1x --------*/
STATIC boolean sim_session_is_1x(
  mmgsdi_session_type_enum_type session
)
{
  return    session == MMGSDI_1X_PROV_PRI_SESSION
         || session == MMGSDI_1X_PROV_SEC_SESSION;
}

/*-------- policyman_get_imsi_file_id --------*/
STATIC mmgsdi_file_enum_type policyman_get_imsi_file_id(
  mmgsdi_app_enum_type app_type
)
{
  mmgsdi_file_enum_type fileId = MMGSDI_GSM_IMSI;

  switch (app_type)
  {
    case MMGSDI_APP_USIM:
      fileId = MMGSDI_USIM_IMSI;
      break;

    case MMGSDI_APP_CSIM:
      fileId = MMGSDI_CSIM_IMSI_M;
      break;

    case MMGSDI_APP_RUIM:
      fileId = MMGSDI_CDMA_IMSI_M;
      break;

    default:
      break;
  }

  return fileId;
}

/*-------- policyman_get_ad_file_id --------*/
STATIC mmgsdi_file_enum_type policyman_get_ad_file_id(
  mmgsdi_app_enum_type app_type
)
{
  mmgsdi_file_enum_type fileId = MMGSDI_GSM_AD;

  switch (app_type)
  {
    case MMGSDI_APP_USIM:
      fileId = MMGSDI_USIM_AD;
      break;

    default:
      break;
  }

  return fileId;
}

/*===========================================================================
 FUNCTION policyman_uim_set_imsi_plmn()

 DESCRIPTION
   Set IMSI PLMN into UIM state
===========================================================================*/
STATIC boolean policyman_uim_set_imsi_plmn(
  policyman_state_t  *pState,
  size_t              subsId,
  sys_plmn_id_s_type *pPlmn
)
{
  boolean               changed  = FALSE;
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);

  if (!policyman_plmns_are_equal(&pUimInfo->plmn, pPlmn))
  {
    POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "updating IMSI PLMN", subsId);
    pUimInfo->plmn = *pPlmn;
    changed = TRUE;
  }

  return changed;
}

/*-------- policyman_uim_set_imsi_m_mcc --------*/
STATIC boolean policyman_uim_set_imsi_m_mcc(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  sys_mcc_type            mcc
)
{
  boolean               changed  = FALSE;
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);

  if (pUimInfo->csim_mcc != mcc)
  {
    pUimInfo->csim_mcc = mcc;
    changed            = TRUE;
    POLICYMAN_MSG_HIGH_2("updating IMSI_M MCC %d for Sub %d", mcc, subsId);
  }

  return changed;
}

/*-------- policyman_uim_get_imsi_m_mcc --------*/
uint32 policyman_uim_get_imsi_m_mcc(
  policyman_state_t * pState,
  size_t              subsId
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);
  return pUimInfo->csim_mcc;
}

/*-------- policyman_process_imsi_m_bytes --------*/
STATIC sys_mcc_type policyman_process_imsi_m_bytes(
  sys_modem_as_id_e_type   subsId,
  mmgsdi_data_type         imsiData
)
{
  uint8          *imsi_m           = (uint8 *)imsiData.data_ptr;
  sys_mcc_type    rawMcc           = 0;
  sys_mcc_type    decodedMcc       = 0;

   /* C.S0005 chapter 2.3.1 : 2 bits of 10th byte + 8 bits of 9th byte
     1. Represent the 3 digit MCC as D1D2D3 with a digit equal to 0 given a value of 10.
     2. Compute 100*D1 + 10*D2 + D3 - 111.
     3. Convert to binary and accordingly write in byte 9 & 10.
     e.x MCC 460, D1 = 4, D2 = 6, D3 = 0
          100*4+10*6+0 +10 - 111 = 359 which is 0x0167 in Hex
          hence byte 9 = 0x67, 10 = 0x01.
   */
  rawMcc =  (imsi_m[9] & 0x02) * 512
          + (imsi_m[9] & 0x01) * 256
          + (imsi_m[8] & 0xFF);

  sd_decode_3gpp2_mcc((word *)&decodedMcc, (word)rawMcc);

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "processing IMSI_M data: rawMcc %d, decodedMCC %d",
                        subsId,
                        rawMcc,
                        decodedMcc );

  return decodedMcc;
}

/*-------- imsi_is_present --------*/
boolean imsi_is_present(
  policyman_state_t           *pState,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_uim_info_t      *pUimInfo;
  boolean                    haveImsi = FALSE;
  mmgsdi_app_enum_type       sim_app_type;
  mmgsdi_app_enum_type       cdma_app_type;

  pUimInfo      =  policyman_state_get_uim_info(pState, subsId);
  sim_app_type  =  policyman_uim_get_sim_type(pState, subsId);
  cdma_app_type =  policyman_uim_get_cdma_sim_type(pState, subsId);

  if ((sim_app_type == MMGSDI_APP_SIM) || (sim_app_type == MMGSDI_APP_USIM))
  {
    boolean  plmn_id_is_undefined;
    boolean  mnc_includes_pcs_digit;
    uint32   mcc;
    uint32   mnc;

    sys_plmn_get_mcc_mnc(pUimInfo->plmn, &plmn_id_is_undefined, &mnc_includes_pcs_digit, &mcc, &mnc);
    haveImsi = ((mcc > 0 && mcc < 1000)) && (sys_plmn_id_is_valid(pUimInfo->plmn));
  }
  else if ((cdma_app_type == MMGSDI_APP_CSIM) || (cdma_app_type == MMGSDI_APP_RUIM))
  {
    haveImsi = (pUimInfo->csim_mcc > 0 && pUimInfo->csim_mcc < 1000);
  }

  return haveImsi;
}


/*-------- uim_update_imsi --------*/
STATIC void uim_update_imsi(
  policyman_state_t                  *pState,
  pm_mmgsdi_session_id_table_entry_t *pSsnEntry,
  mmgsdi_data_type                    imsiData,
  mmgsdi_data_type                    adData,
  mre_set_t                          *pItemSet
)
{
  sys_plmn_id_s_type     plmn;
  uint8                  num_mnc_digits;
  boolean                imsi_has_changed = FALSE;

  if (sys_get_num_of_mnc_digits(&imsiData, &adData, &num_mnc_digits))
  {
    mre_extract_plmn_from_imsi(imsiData, num_mnc_digits, &plmn);
    POLICYMAN_MSG_PLMN_0("IMSI PLMN: ", plmn);

    // Set SIM refresh only after first time IMSI is set
    policyman_state_enter_crit_section();
    if (imsi_is_present(pState, pSsnEntry->asubs_id))
    {
       policyman_uim_set_sim_refresh(pState, pSsnEntry->asubs_id, TRUE);
    }

    imsi_has_changed = policyman_uim_set_imsi_plmn(pState, pSsnEntry->asubs_id, &plmn);

    policyman_state_leave_crit_section();

    if (imsi_has_changed)
    {
      policyman_policy_update_hplmn(pSsnEntry->asubs_id, &imsiData, &adData);
      policyman_uim_handle_imsi_update(pState, imsiData, pSsnEntry->asubs_id, pItemSet);
    }
  }
}


/*-------- pm_read_imsi_hplmn --------*/
STATIC mre_status_t pm_read_imsi_hplmn(
  policyman_state_t                  *pState,
  pm_mmgsdi_session_id_table_entry_t *pSsnEntry,
  mre_set_t                          *pItemSet
)
{
  mre_status_t       status    = MRE_STATUS_FAILED;
  mmgsdi_data_type   imsiData  = {0, NULL};
  mmgsdi_data_type   adData    = {0, NULL};
  
  /* Read EF-IMSI */
  status = mre_read_mmgsdi_file( pSsnEntry->session_id,
                                 policyman_get_imsi_file_id(pSsnEntry->app_type),
                                 &imsiData );
  
  if (   MRE_FAILED(status)
      || (imsiData.data_len != MMGSDI_IMSI_LEN) )
  {
    goto Done;
  }
  
  /* Read EF-AD */
  status = mre_read_mmgsdi_file( pSsnEntry->session_id,
                                 policyman_get_ad_file_id(pSsnEntry->app_type),
                                 &adData );
  
  if (   MRE_FAILED(status)
      || (adData.data_len != PM_MMGSDI_RPM_AD_SIZE) )
  {
    goto Done;
  }
  
  uim_update_imsi( pState,
                   pSsnEntry,
                   imsiData,
                   adData,
                   pItemSet );
  
Done:
  POLICYMAN_MEM_FREE_IF(imsiData.data_ptr);
  POLICYMAN_MEM_FREE_IF(adData.data_ptr);
  
  if (MRE_FAILED(status))
  {
    POLICYMAN_MSG_ERROR_1("EFS-IMSI file read returned MRE status %d", status);
  }

  return  status;
} /* pm_read_imsi_hplmn() */


/*-------- uim_update_imsi_m --------*/
STATIC void uim_update_imsi_m(
  policyman_state_t                  *pState,
  pm_mmgsdi_session_id_table_entry_t *pSsnEntry,
  mmgsdi_data_type                    imsiData,
  mre_set_t                          *pItemSet
)
{
  sys_mcc_type    imsi_m_mcc;
  boolean         imsi_has_changed;
  
  imsi_m_mcc = policyman_process_imsi_m_bytes( pSsnEntry->asubs_id,
                                               imsiData );
  
  policyman_state_enter_crit_section();
  
  imsi_has_changed = policyman_uim_set_imsi_m_mcc(pState, pSsnEntry->asubs_id, imsi_m_mcc);
  
  policyman_state_leave_crit_section();
  
  if (imsi_has_changed)
  {
     policyman_uim_handle_imsi_update(pState, imsiData, pSsnEntry->asubs_id, pItemSet);
  }
}


/*-------- pm_read_imsi_m --------*/
STATIC mre_status_t pm_read_imsi_m(
  policyman_state_t                  *pState,
  pm_mmgsdi_session_id_table_entry_t *pSsnEntry,
  mre_set_t                          *pItemSet
)
{
  mre_status_t  status = MRE_STATUS_FAILED;

  if (   pSsnEntry->app_type == MMGSDI_APP_CSIM
      || pSsnEntry->app_type == MMGSDI_APP_RUIM )
  {
    mmgsdi_data_type     imsiData = {0, NULL};
    uint8               *imsi_m;

    status = mre_read_mmgsdi_file( pSsnEntry->session_id,
                                   policyman_get_imsi_file_id(pSsnEntry->app_type),
                                   &imsiData );

    if (   MRE_SUCCEEDED(status)
        && (IMSI_M_DATA_LEN == imsiData.data_len) )
    {
      uim_update_imsi_m( pState,
                         pSsnEntry,
                         imsiData,
                         pItemSet );
    }
    else
    {
      POLICYMAN_MSG_ERROR_1("EFS-IMSI_M file read returned MRE status %d", status);
    }

    POLICYMAN_MEM_FREE_IF(imsiData.data_ptr);
  }

  return  status;
} /* pm_read_imsi_m() */


/*-------- pm_read_imsi_ehplmn_list --------*/
STATIC mre_status_t pm_read_imsi_ehplmn_list(
  policyman_state_t                  *pState,
  pm_mmgsdi_session_id_table_entry_t *pSsnEntry
)
{
  mre_status_t         status    = MRE_STATUS_FAILED;
  mmgsdi_data_type     read_data = {0, NULL};

  if (MMGSDI_APP_USIM == pSsnEntry->app_type)
  {
    status = mre_read_mmgsdi_file( pSsnEntry->session_id,
                                   MMGSDI_USIM_EHPLMN,
                                   &read_data );
  
    if (MRE_SUCCEEDED(status))
    {
      mre_set_t *pEhPLMNSet;
  
      pEhPLMNSet = mre_policy_create_ehplmn_set(&read_data);
      policyman_policy_update_ehplmn_set(pSsnEntry->asubs_id, pEhPLMNSet);
      MRE_RELEASE_IF(pEhPLMNSet);
    }

    POLICYMAN_MEM_FREE_IF(read_data.data_ptr);
  }

  return  status;
} /* pm_read_imsi_ehplmn_list() */


/*-------- policyman_uim_get_subs_active --------*/
boolean policyman_uim_get_subs_active(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo  = policyman_state_get_uim_info(pState, subsId);
  return pUimInfo->subs_active;
}

/*-------- policyman_uim_set_subs_active --------*/
STATIC void policyman_uim_set_subs_active(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  boolean                 newState
)
{
  policyman_uim_info_t *pUimInfo;
  boolean               prevState;

  pUimInfo  = policyman_state_get_uim_info(pState, subsId);
  prevState = pUimInfo->subs_active;

  if (prevState != newState)
  {
    pUimInfo->subs_active = newState;
    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "active state updated from %d to %d",
                          subsId,
                          prevState,
                          newState );

  }
}

/*-------- policyman_process_gw_subs --------*/
STATIC void policyman_process_gw_subs(
  policyman_sandbox_t                *pSandbox,
  pm_mmgsdi_session_id_table_entry_t *pSsnEntry,
  client_msg_t                       *pClientMsg
)
{
  mre_status_t      status;
  mre_set_t        *pItemSet;

  pItemSet = policyman_itemset_new();

  /* read IMSI PLMN for GW subs */
  status = pm_read_imsi_hplmn(pSandbox->pState, pSsnEntry, pItemSet);

  if (MRE_SUCCEEDED(status))
  {
    policyman_uim_set_subs_active(pSandbox->pState, pSsnEntry->asubs_id, TRUE);
    policyman_uim_set_sim_type(pSandbox->pState, pSsnEntry->asubs_id, pSsnEntry->app_type);
    (void)pm_read_imsi_ehplmn_list(pSandbox->pState, pSsnEntry);
  }
  else
  {
    POLICYMAN_MSG_ERROR_1("Internal error in reading HPLMN: %d", status);
  }

  policyman_policies_run_policy_check(pSandbox, pItemSet);
  policyman_cfgitem_update_items(pSandbox->pState, pItemSet, pClientMsg);
  ref_cnt_obj_release(pItemSet);
}

/*-------- policyman_process_1x_subs --------*/
STATIC void policyman_process_1x_subs(
  policyman_sandbox_t                *pSandbox,
  pm_mmgsdi_session_id_table_entry_t *ssnEntry,
  client_msg_t                       *pClientMsg
)
{
  mre_status_t   status;
  mre_set_t     *pItemSet;

  pItemSet = policyman_itemset_new();

  /* read IMSI_M PLMN for 1x subs */
  status = pm_read_imsi_m(pSandbox->pState, ssnEntry, pItemSet);

  if (MRE_SUCCEEDED(status))
  {
    policyman_uim_set_subs_active(pSandbox->pState, ssnEntry->asubs_id, TRUE);
    policyman_uim_set_cdma_sim_type(pSandbox->pState, ssnEntry->asubs_id, ssnEntry->app_type);
  }
  else
  {
    POLICYMAN_MSG_ERROR_1("Internal error %d in reading IMSI_M", status);
  }

  policyman_policies_run_policy_check(pSandbox, pItemSet);
  policyman_cfgitem_update_items(pSandbox->pState, pItemSet, pClientMsg);
  ref_cnt_obj_release(pItemSet);  
}

/*-------- policyman_process_subs_ready_event --------*/
STATIC void policyman_process_subs_ready_event(
  policyman_sandbox_t                *pSandbox,
  pm_mmgsdi_session_id_table_entry_t *ssnEntry,
  mmgsdi_event_data_type             *pEvent,  
  client_msg_t                       *pClientMsg
)
{
  if (is_mmgsdi_subscription_ready_received == FALSE)
  {
    is_mmgsdi_subscription_ready_received = TRUE;

    /* Print out all configuration database when receiving the first MMGSDI_SUBSCRIPTION_READY_EVT
        */
    POLICYMAN_MSG_MED_0("Received the first MMGSDI_SUB_READY_EVT, print all configuration database");
    policyman_cfgitem_display_all();
  }

  ssnEntry->app_type =  pEvent->data.subscription_ready.app_info.app_data.app_type;
  ssnEntry->active   = TRUE;

  if (sim_session_is_gw(ssnEntry->session_type))
  {
    policyman_process_gw_subs(pSandbox, ssnEntry, pClientMsg);
  }
  else if (sim_session_is_1x(ssnEntry->session_type))
  {
    policyman_process_1x_subs(pSandbox, ssnEntry, pClientMsg);
  }
}

/*-------- policyman_process_refresh_event --------*/
STATIC void policyman_process_refresh_event(
  policyman_sandbox_t                *pSandbox,
  pm_mmgsdi_session_id_table_entry_t *pSsnEntry,
  mmgsdi_event_data_type             *pEvent,
  client_msg_t                       *pClientMsg

)
{
  mre_status_t  status;
  mre_set_t    *pItemSet;

  pItemSet = policyman_itemset_new();

  POLICYMAN_MSG_HIGH_1( "Received trigger for UIM refresh for session type %d", pSsnEntry->session_type );

  status = pm_read_imsi_hplmn(pSandbox->pState, pSsnEntry, pItemSet);
  if (MRE_SUCCEEDED(status))
  {
   (void)pm_read_imsi_ehplmn_list(pSandbox->pState, pSsnEntry);
    policyman_policies_run_policy_check(pSandbox, pItemSet);
    policyman_cfgitem_update_items(pSandbox->pState, pItemSet, pClientMsg);
  }
  else
  {
    POLICYMAN_MSG_ERROR_1("Internal error in reading HPLMN: %d", status);
  }

  ref_cnt_obj_release(pItemSet);
}

/*-------- uim_delete_gw_info --------*/
STATIC boolean uim_delete_gw_info(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_uim_info_t  *pUimInfo;
  boolean                info_deleted = FALSE;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);
  if (NON_NULL(pUimInfo))
  {
    pUimInfo->sim_app_type = MMGSDI_APP_NONE;
    info_deleted           = TRUE;
    memset(&pUimInfo->plmn, 0, sizeof(sys_plmn_id_s_type));
    policyman_policy_delete_hplmn(subsId);
    policyman_policy_release_ehplmn_set(subsId);

  }

  return info_deleted;
}


/*-------- uim_delete_1x_info --------*/
STATIC boolean uim_delete_1x_info(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_uim_info_t  *pUimInfo;
  boolean                info_deleted = FALSE;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);
  if (NON_NULL(pUimInfo))
  {
    pUimInfo->csim_mcc      = 0;
    pUimInfo->cdma_app_type = MMGSDI_APP_NONE;
    info_deleted            = TRUE;
  }

  return info_deleted;
}


/*-------- policyman_uim_del_sub --------*/
STATIC boolean policyman_uim_del_sub(
  policyman_state_t             *pState,
  sys_modem_as_id_e_type         subsId,
  mmgsdi_session_type_enum_type  session_type
)
{
  boolean  info_deleted = FALSE;

  if (session_type == MMGSDI_GW_PROV_PRI_SESSION || session_type == MMGSDI_GW_PROV_SEC_SESSION)
  {
    info_deleted = uim_delete_gw_info(pState, subsId);
  }
  else if (session_type == MMGSDI_1X_PROV_PRI_SESSION || session_type == MMGSDI_1X_PROV_SEC_SESSION)
  {
    info_deleted = uim_delete_1x_info(pState, subsId);
  }

  policyman_state_set_imsi_verified(subsId, TRUE);
  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "deleted sub info status %d", subsId, info_deleted );

  return info_deleted;
}

/*-------- policyman_uim_fill_diag_single_session_info --------*/
STATIC void policyman_uim_fill_diag_single_session_info(
  pm_mmgsdi_session_id_table_entry_t *ssn_entry,
  pm_dump_uim_session_info_t         *dump_uim_info
)
{
  dump_uim_info->session_type = ssn_entry->session_type;
  dump_uim_info->asubs_id     = ssn_entry->asubs_id;
  dump_uim_info->session_id   = ssn_entry->session_id;
  dump_uim_info->app_type     = ssn_entry->app_type;
  dump_uim_info->in_use       = ssn_entry->in_use;
  dump_uim_info->active       = ssn_entry->active;
}

/*-------- policyman_cmph_create_and_send_diag_ph_log --------*/
STATIC void policyman_uim_create_and_send_diag_uim_log(
  pm_mmgsdi_session_id_table_entry_t *ssn_entry,
  policyman_state_t                  *pState
)
{
  LOG_PM_UIM_HISTORY_INFO_type *log_ptr;

  log_ptr = LOG_PACKET_NEW( LOG_PM_UIM_HISTORY_INFO_C,
                            LOG_PM_UIM_HISTORY_INFO_type);

  if (NULL != log_ptr)
  {
    policyman_uim_info_t *pUimInfo;

    pUimInfo         = policyman_state_get_uim_info(pState, ssn_entry->asubs_id);
    log_ptr->version = LOG_PM_UIM_HISTORY_INFO_LOG_PACKET_VERSION;

    policyman_uim_fill_diag_single_session_info(ssn_entry, &log_ptr->session_info);
    policyman_uim_fill_diag_uim_info(pUimInfo, &log_ptr->uim_info);

    log_commit( (void*)log_ptr );
  }
}

/*-------- pm_process_uim_evt --------*/
void pm_process_uim_evt(
  policyman_cmd_t *pCmd
)
{
  policyman_uim_evt_cb_t             *pUimCmd   = (policyman_uim_evt_cb_t  *)pCmd;
  mmgsdi_event_data_type             *pEvent    = &((policyman_uim_evt_cb_t *)pCmd)->mmgsdi_event;
  pm_mmgsdi_session_id_table_entry_t *ssnEntry;

  ssnEntry = pm_mmgsdi_ssn_tbl_get_entry_by_id(pEvent->session_id);

  POLICYMAN_MSG_HIGH_3( "PM got MMGSDI evt %d, session_id %d, ssn_entry 0x%0x",
                        pEvent->evt,
                        pEvent->session_id,
                        ssnEntry );

  if (NULL != ssnEntry)
  {
    print_session_info(ssnEntry);

    switch (pEvent->evt)
    {
      case MMGSDI_SUBSCRIPTION_READY_EVT:
        policyman_state_enter_crit_section();

        policyman_process_subs_ready_event( pCmd->pSandbox,
                                            ssnEntry,
                                            pEvent,
                                            &pCmd->clientMsg );
        
        policyman_state_leave_crit_section();
        break;

      case MMGSDI_SESSION_CHANGED_EVT:
        if (!pEvent->data.session_changed.activated
           && ssnEntry->active == TRUE
           )
        {
          mre_set_t *pItemSet;
          
          ssnEntry->active     = FALSE;
          ssnEntry->fdn_status = POLICYMAN_FDN_STATUS_NONE;

          policyman_state_enter_crit_section();
          policyman_uim_set_subs_active(pCmd->pSandbox->pState, ssnEntry->asubs_id, FALSE);
          policyman_uim_del_sub(pCmd->pSandbox->pState, ssnEntry->asubs_id, ssnEntry->session_type);
          policyman_state_leave_crit_section();

          policyman_fullrat_config_reset_timer_expired_per_subs(ssnEntry->asubs_id);
          policyman_fullrat_config_reset_timer_expired_post_per_subs(ssnEntry->asubs_id);
          policyman_fullrat_config_timer_stop_per_subs(ssnEntry->asubs_id);
          policyman_fullrat_config_timer_stop_post_per_subs(ssnEntry->asubs_id);

          pItemSet = policyman_itemset_new();
          policyman_policies_run_policy_check(pCmd->pSandbox, pItemSet);
          policyman_cfgitem_update_items(pCmd->pSandbox->pState, pItemSet, &pCmd->clientMsg);
          ref_cnt_obj_release(pItemSet);
        }
        break;

      case MMGSDI_REFRESH_EVT:
        policyman_state_enter_crit_section();
        policyman_process_refresh_event(pCmd->pSandbox, ssnEntry, pEvent, &pCmd->clientMsg);
        policyman_state_leave_crit_section();
        break;

      default:
       break;
    }

    policyman_uim_create_and_send_diag_uim_log(ssnEntry, pCmd->pSandbox->pState);
  }
}

/*===========================================================================
  FUNCTION pm_mmgsdi_reg_status_cb()

  DESCRIPTION
 ===========================================================================*/
static void pm_mmgsdi_reg_status_cb(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    data_type,
  const mmgsdi_cnf_type  *data_ptr
)
{
   pm_mmgsdi_resp_t *pCmd = NULL;

   if (NULL == data_ptr)
   {
     return;
   }

   pCmd = POLICYMAN_CMD_NEW( pm_mmgsdi_resp_t,
                             pm_process_mmgsdi_reg_status,
                             NULL,
                             NULL );

   pCmd->data_type = data_type;
   pCmd->status = status;
   pCmd->data = *data_ptr;

   policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
   ref_cnt_obj_release(pCmd);
}

/*-------- policyman_uim_get_subs_plmn --------*/
static sys_plmn_id_s_type *policyman_uim_get_subs_plmn(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);

  return &pUimInfo->plmn;
}

/*===========================================================================
 FUNCTION policyman_uim_get_imsi_mcc()

 DESCRIPTION
===========================================================================*/
uint32 policyman_uim_get_imsi_mcc(
  policyman_state_t *pState,
  size_t             subsId
)
{
  return policyman_plmn_get_mcc(policyman_uim_get_subs_plmn(pState, subsId));
}

/*===========================================================================
 FUNCTION policyman_uim_get_imsi_plmn()

 DESCRIPTION
===========================================================================*/
boolean policyman_uim_get_imsi_plmn(
  policyman_state_t  *pInfo,
  size_t              subsId,
  sys_plmn_id_s_type *pPlmn
)
{
  if (subsId >= MAX_SIM_CARDS)
  {
    return FALSE;
  }

  *pPlmn = *policyman_uim_get_subs_plmn(pInfo, subsId);

  return TRUE;
}

/*===========================================================================
 FUNCTION policyman_uim_set_sim_type()

 DESCRIPTION
   Set SIM app type into UIM state
===========================================================================*/
boolean policyman_uim_set_sim_type(
  policyman_state_t    *pInfo,
  size_t                subsId,
  mmgsdi_app_enum_type  sim_app_type
)
{
  boolean               changed  = FALSE;
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pInfo, subsId);

  if (sim_app_type != pUimInfo->sim_app_type)
  {
    pUimInfo->sim_app_type = sim_app_type;
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "updated sim_app_type %d",
                          subsId,
                          sim_app_type );
    changed = TRUE;
  }

  return changed;
}

/*===========================================================================
 FUNCTION policyman_uim_get_sim_type()

 DESCRIPTION
   Get SIM app type from UIM state
===========================================================================*/
mmgsdi_app_enum_type policyman_uim_get_sim_type(
  policyman_state_t    *pInfo,
  size_t                subsId
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pInfo, subsId);

  return pUimInfo->sim_app_type;
}

/*===========================================================================
 FUNCTION policyman_uim_set_cdma_sim_type()

 DESCRIPTION
   Set CDMA app type into UIM state
===========================================================================*/
boolean policyman_uim_set_cdma_sim_type(
  policyman_state_t    *pInfo,
  size_t                subsId,
  mmgsdi_app_enum_type  sim_app_type
)
{
  boolean               changed  = FALSE;
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pInfo, subsId);

  if (sim_app_type != pUimInfo->cdma_app_type)
  {
    pUimInfo->cdma_app_type = sim_app_type;
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "updated cdma_sim_app type %d",
                          subsId,
                          sim_app_type );
    changed = TRUE;
  }

  return changed;
}

/*-------- policyman_uim_get_cdma_sim_type --------*/
mmgsdi_app_enum_type policyman_uim_get_cdma_sim_type(
  policyman_state_t     *pInfo,
  size_t                 subsId
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pInfo, subsId);

  return pUimInfo->cdma_app_type;
}

/*-------- policyman_uim_set_sim_refresh --------*/
void policyman_uim_set_sim_refresh(
  policyman_state_t *pState,
  size_t             subsId,
  boolean            simWasRefreshed
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);

  pUimInfo->sim_refreshed = simWasRefreshed;

}

/*-------- policyman_uim_get_sim_refresh --------*/
boolean policyman_uim_get_sim_refresh(
  policyman_state_t *pState,
  size_t             subsId
)
{
  policyman_uim_info_t *pUimInfo;

  pUimInfo = policyman_state_get_uim_info(pState, subsId);

  return pUimInfo->sim_refreshed;
}

/*-------- policyman_uim_sim_is_3gpp2 --------*/
boolean policyman_uim_sim_is_3gpp2(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  boolean                 *pCdmaIsFromSim,
  boolean                 *pCdmaIsFromNv
)
{
  mmgsdi_app_enum_type simType;

  *pCdmaIsFromNv = (policyman_phone_get_rtre_control(pState, subsId) == CM_RTRE_CONTROL_NV);

  simType = policyman_uim_get_cdma_sim_type(pState, subsId);

  *pCdmaIsFromSim = (simType == MMGSDI_APP_RUIM || simType == MMGSDI_APP_CSIM);

  return *pCdmaIsFromNv || *pCdmaIsFromSim;
}

/*=============================================================================
  Conditions based on serving system.
=============================================================================*/

#define MAX_UIM_CONDITION_SET_SIZE   50


/*-----------------------------------------------------------------------------
  condition_imsi_mcc
-----------------------------------------------------------------------------*/


typedef struct
{
  POLICYMAN_CONDITION_BASE;

  mre_plmn_mcc_set_t *pSet;
  char const         *pSetName;
  boolean             checkIMSI_M;
} policyman_imsi_mcc_condition_t;

/*-------- policyman_map_str_to_session --------*/
STATIC mmgsdi_session_type_enum_type policyman_map_str_to_session(
  char const             *pStr,
  sys_modem_as_id_e_type  subsId
)
{
  static MRE_STR_VALUE_TABLE(map_primary) =
  {
    MRE_STR_VALUE_ENTRY("GW",  MMGSDI_GW_PROV_PRI_SESSION),
    MRE_STR_VALUE_ENTRY("1x",      MMGSDI_1X_PROV_PRI_SESSION),
  };

  static MRE_STR_VALUE_TABLE(map_secondary) =
  {
    MRE_STR_VALUE_ENTRY("GW",  MMGSDI_GW_PROV_SEC_SESSION),
    MRE_STR_VALUE_ENTRY("1x",      MMGSDI_1X_PROV_SEC_SESSION),
  };

  char                          token[32];
  mmgsdi_session_type_enum_type session = MMGSDI_MAX_SESSION_TYPE_ENUM;

  if (policyman_get_token(&pStr, token, sizeof(token)))
  {
    mre_status_t  status;
    if ( subsId == SYS_MODEM_AS_ID_1)
    {
      MRE_LOOKUP_STR_VALUE(map_primary, token, mmgsdi_session_type_enum_type, &session);
    }
    else if ( subsId == SYS_MODEM_AS_ID_2)
    {
      MRE_LOOKUP_STR_VALUE(map_secondary, token, mmgsdi_session_type_enum_type, &session);
    }
  }

  return session;
}


/*-------- evaluate_imsi_mcc_in --------*/
static boolean evaluate_imsi_mcc_in(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
  )
{
  policyman_imsi_mcc_condition_t  *pImsiCond  = (policyman_imsi_mcc_condition_t *) pCondition;
  mre_set_t                       *pItemSet   = POLICY_EXECUTE_ITEMSET(pCtx);
  mre_policy_t                    *pPolicy    = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t               *pState     = POLICY_EXECUTE_STATE(pCtx);
  mre_plmn_mcc_set_t              *pMccSet    = NULL;  
  uint32                           uimMcc     = NO_MCC;
  boolean                          result     = FALSE;

  pMccSet = policyman_get_set_from_condition((policyman_set_condition_t *)pImsiCond, pPolicy, MRE_NAMED_MCC_SET);
  if (pMccSet == NULL)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "named PLMN set not found", POLICY_SUBS(pPolicy));
    goto Done;
  }

  if (pImsiCond->checkIMSI_M)
  {
    uimMcc = policyman_uim_get_imsi_m_mcc(pState, subsId);
  }
  else
  {
    uimMcc = policyman_uim_get_imsi_mcc(pState, subsId);
  }

  result =  mre_mcc_list_contains(pPolicy, pMccSet, &uimMcc);

Done:
  POLICYMAN_UT_MSG_5( SUBS_PREFIX SRC_LINE "condition <imsi_mcc_in pmsubs=%d> with mcc %d returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pImsiCond),
                      subsId,
                      uimMcc,
                      result );
  return result;
}

/*-------- policyman_condition_imsi_mcc_evaluate --------*/
static boolean policyman_condition_imsi_mcc_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const *pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_imsi_mcc_in);
}

/*-------- policyman_condition_imsi_mcc_dtor --------*/
static void policyman_condition_imsi_mcc_dtor(
  void  *pObj
)
{
  policyman_imsi_mcc_condition_t  *pCondition = (policyman_imsi_mcc_condition_t *)pObj;

  MRE_RELEASE_IF(pCondition->pSet);
  POLICYMAN_MEM_FREE_IF(pCondition->pSetName);
  policyman_condition_dtor(pCondition);
}


/*-------- policyman_str_to_session --------*/
STATIC mmgsdi_session_type_enum_type policyman_str_to_session(
  char const  *pStr
)
{
  static MRE_STR_VALUE_TABLE(map_primary) =
  {
    MRE_STR_VALUE_ENTRY("GW",  MMGSDI_GW_PROV_PRI_SESSION),
    MRE_STR_VALUE_ENTRY("1x",      MMGSDI_1X_PROV_PRI_SESSION),
  };

  char                          token[32];
  mmgsdi_session_type_enum_type session = MMGSDI_MAX_SESSION_TYPE_ENUM;

  if (policyman_get_token(&pStr, token, sizeof(token)))
  {
    mre_status_t  status;

    MRE_LOOKUP_STR_VALUE(map_primary, token, mmgsdi_session_type_enum_type, &session);
    if (MRE_FAILED(status))
    {
      POLICYMAN_UT_MSG_1("Invalid value for 'subs' attribute of <service_status_in>: %s", pStr);
  }
  }

  return session;
}


/*-------- session_is_1x --------*/
STATIC boolean session_is_1x(
  mre_xml_element_t const  *pElem
)
{
  char const                     *pStr;
  mmgsdi_session_type_enum_type   session = MMGSDI_GW_PROV_PRI_SESSION;

  pStr = policyman_xml_get_attribute(pElem, "session");
  if (NON_NULL(pStr))
  {
    session = policyman_str_to_session(pStr);
  }

  return (session == MMGSDI_1X_PROV_PRI_SESSION);
}


/*-------- policyman_condition_imsi_mcc_new --------*/
mre_status_t policyman_condition_imsi_mcc_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_condition_t          **ppCondition
)
{
  mre_status_t                     status;
  policyman_imsi_mcc_condition_t  *pCondition  = NULL;
  policyman_subs_t                 pm_subs;

  status = policyman_util_get_subs(pElem, POLICY_SUBS(pPolicy), &pm_subs);

  pCondition = POLICYMAN_CONDITION_NEW( policyman_imsi_mcc_condition_t,
                                        pPolicy,
                                        policyman_condition_imsi_mcc_evaluate,
                                        policyman_condition_imsi_mcc_dtor,
                                        pm_subs,
                                        pElem );

  policyman_plmn_mcc_get_list( pElem,
                               pPolicy,
                               MRE_NAMED_MCC_SET,
                               mre_mcc_list_read,
                               (policyman_set_condition_t *)pCondition );

  //  If there is neither an inline or named list, bail.
  if (IS_NULL(pCondition->pSet) && IS_NULL(pCondition->pSetName))
  {
    MRE_RELEASE_IF(pCondition);
    status = MRE_STATUS_ERR_INVALID_CONDITION;
  }
  else
  {
    pCondition->checkIMSI_M = session_is_1x(pElem);
    *ppCondition = (mre_condition_t *)pCondition;
    status = MRE_STATUS_SUCCESS;
  }

  return status;
}

/*-----------------------------------------------------------------------------
  condition_imsi_plmn
-----------------------------------------------------------------------------*/


/*-------- evaluate_imsi_plmn_in --------*/
static boolean evaluate_imsi_plmn_in(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_set_condition_t *pCond    = (policyman_set_condition_t *) pCondition;
  mre_set_t                 *pItemSet = POLICY_EXECUTE_ITEMSET(pCtx);
  mre_policy_t              *pPolicy  = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t         *pState   = POLICY_EXECUTE_STATE(pCtx);
  mre_plmn_mcc_set_t        *pPlmnSet = NULL;
  sys_plmn_id_s_type         uimPlmn;
  boolean                    result   = FALSE;

  pPlmnSet = policyman_get_set_from_condition(pCond, pPolicy, MRE_NAMED_PLMN_SET);
  if (pPlmnSet == NULL)
  {
    POLICYMAN_MSG_ERROR_1(SUBS_PREFIX "failed to get PLMN set", POLICY_SUBS(pPolicy));
    goto Done;
  }

  policyman_uim_get_imsi_plmn(pState, subsId, &uimPlmn);

  result = mre_plmn_list_contains(pPolicy, pPlmnSet, &uimPlmn);
  POLICYMAN_UT_MSG_PLMN_0("IMSI PLMN: ", uimPlmn);

Done:

  POLICYMAN_UT_MSG_4( SUBS_PREFIX SRC_LINE "condition <imsi_plmn_in pmsubs=%d> returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      subsId,
                      result );
  return result;
}

/*-------- policyman_condition_imsi_plmn_evaluate --------*/
STATIC boolean policyman_condition_imsi_plmn_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_imsi_plmn_in);
}

/*-------- policyman_condition_imsi_plmn_new --------*/
mre_status_t policyman_condition_imsi_plmn_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  return policyman_plmn_mcc_new( pElem, 
                                 pPolicy,
                                 MRE_NAMED_PLMN_SET,
                                 mre_plmn_list_read,
                                 ppCondition, 
                                 policyman_condition_imsi_plmn_evaluate );
}


typedef struct
{
  POLICYMAN_CONDITION_BASE;

  mmgsdi_app_enum_type sim_app_type;
} policyman_sim_type_condition_t;

/*-------- sim_type_is_present --------*/
boolean sim_type_is_present(
  policyman_state_t           *pState,
  sys_modem_as_id_e_type       subsId,
  mmgsdi_app_enum_type         desired_sim_app_type
)
{
  mmgsdi_app_enum_type         sim_app_type;
  mmgsdi_app_enum_type         cdma_app_type;
  boolean                      result = FALSE;

  sim_app_type  = policyman_uim_get_sim_type(pState, subsId);
  cdma_app_type = policyman_uim_get_cdma_sim_type(pState, subsId);

  switch(desired_sim_app_type)
  {
    case MMGSDI_APP_SIM:
    case MMGSDI_APP_USIM:
      result =  desired_sim_app_type == sim_app_type;
      break;

    case MMGSDI_APP_CSIM:
    case MMGSDI_APP_RUIM:
      result =  desired_sim_app_type == cdma_app_type;
      break;

    default:
      break;
  }

  return result;
}

/*-------- evaluate_sim_type --------*/
static boolean evaluate_sim_type(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_sim_type_condition_t *pCond    = (policyman_sim_type_condition_t*)pCondition;
  mre_set_t                      *pItemSet      = POLICY_EXECUTE_ITEMSET(pCtx);
  mre_policy_t                   *pPolicy       = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t              *pState        = POLICY_EXECUTE_STATE(pCtx);
  boolean                         result   = FALSE;
  mmgsdi_app_enum_type            sim_app_type;
  mmgsdi_app_enum_type            cdma_app_type;

  sim_app_type  = policyman_uim_get_sim_type(pState, POLICY_SUBS(pPolicy));
  cdma_app_type = policyman_uim_get_cdma_sim_type(pState, POLICY_SUBS(pPolicy));

  result = sim_type_is_present(pState, POLICY_SUBS(pPolicy), pCond->sim_app_type);

  POLICYMAN_UT_MSG_5( SUBS_PREFIX SRC_LINE "condition <sim_type> with SIM type %d, CSIM type %d, returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      sim_app_type,
                      cdma_app_type,
                      result );

  return result;
}

static boolean policyman_condition_sim_type_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_sim_type);
}

static mmgsdi_app_enum_type policyman_str_to_sim_type(
  const char  *pSim_type
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("3G",   MMGSDI_APP_USIM),
    MRE_STR_VALUE_ENTRY("2G",   MMGSDI_APP_SIM),
    MRE_STR_VALUE_ENTRY("CSIM", MMGSDI_APP_CSIM),
    MRE_STR_VALUE_ENTRY("RUIM", MMGSDI_APP_RUIM),
    MRE_STR_VALUE_ENTRY("NONE", MMGSDI_APP_NONE)
  };

  char                  token[32];
  mmgsdi_app_enum_type  sim_type = MMGSDI_APP_UNKNOWN;

  if (policyman_get_token(&pSim_type, token, sizeof(token)))
  {
    MRE_LOOKUP_STR_VALUE_NO_STATUS(map, token, mmgsdi_app_enum_type, &sim_type);
  }

  return sim_type;
}

/*-------- policyman_condition_sim_type_new --------*/
mre_status_t policyman_condition_sim_type_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                     status = MRE_STATUS_ERR_INVALID_CONDITION;
  policyman_sim_type_condition_t  *pCondition = NULL;
  mmgsdi_app_enum_type             simType;
  char const                      *pStr;
  policyman_subs_t                 pm_subs;

  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pStr = policyman_xml_get_text(pElem);

  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<sim_type> must specify SIM type", POLICY_SUBS(pPolicy));
    goto Done;
  }

  simType = policyman_str_to_sim_type(pStr);

  if (simType == MMGSDI_APP_UNKNOWN)
  {
    POLICYMAN_UT_MSG_2( SUBS_PREFIX "<sim_type> invalid SIM type: %s",
                        POLICY_SUBS(pPolicy),
                        pStr );
    goto Done;
  }

  /*  Allocate the condition
   */
  pCondition = POLICYMAN_CONDITION_NEW( policyman_sim_type_condition_t,
                                        pPolicy,
                                        policyman_condition_sim_type_evaluate,
                                        policyman_condition_dtor,
                                        pm_subs,
                                        pElem );

  pCondition->sim_app_type = simType;

  *ppCondition = (mre_condition_t *) pCondition;
  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}

/*-----------------------------------------------------------------------------
  condition_sim_refresh
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;
} policyman_sim_refresh_condition_t;

/*-------- evaluate_sim_refresh --------*/
static boolean evaluate_sim_refresh(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  mre_policy_t       *pPolicy        = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t  *pState         = POLICY_EXECUTE_STATE(pCtx);
  boolean             sim_refreshed;

  sim_refreshed = policyman_uim_get_sim_refresh(pState, subsId);

  POLICYMAN_UT_MSG_4( SUBS_PREFIX SRC_LINE "condition <sim_refresh pmsubs=%d> returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCondition),
                      subsId,
                      sim_refreshed );
  return sim_refreshed;
}

/*-------- policyman_condition_sim_refresh_evaluate --------*/
static boolean policyman_condition_sim_refresh_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_sim_refresh);
}

/*-------- policyman_condition_sim_refresh_new --------*/
mre_status_t policyman_condition_sim_refresh_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  mre_status_t                        status = MRE_STATUS_ERR_INVALID_CONDITION;
  policyman_sim_refresh_condition_t  *pCondition = NULL;
  policyman_subs_t                    pm_subs;

  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  /*  Allocate the condition
   */
  pCondition = POLICYMAN_CONDITION_NEW( policyman_sim_refresh_condition_t,
                                        pPolicy,
                                        policyman_condition_sim_refresh_evaluate,
                                        policyman_condition_dtor,
                                        pm_subs,
                                        pElem );

  status = MRE_STATUS_SUCCESS;

  *ppCondition = (mre_condition_t *) pCondition;

 Done:

  return status;
}



/*-------- session_availability_info_has_changed --------*/
STATIC boolean session_availability_info_has_changed(
  pm_mmgsdi_session_id_table_entry_t  *ssn_entry,
  policyman_session_avail_enum_t       notifyType,
  mmgsdi_app_enum_type                 appType
)
{
  return (   ((notifyType == SESSION_AVAILABLE)     && !ssn_entry->active)
          || ((notifyType == SESSION_AVAILABLE)     && (ssn_entry->app_type != appType))
          || ((notifyType == SESSION_NOT_AVAILABLE) &&  ssn_entry->active)
          ||  (notifyType == SESSION_REFRESHED) );
}

/*-------- policyman_report_session_availability --------*/
msgr_hdr_s * policyman_report_session_availability(
  policyman_session_avail_enum_t notifyType,
  mmgsdi_session_type_enum_type  sessionType,
  mmgsdi_app_enum_type           appType
)
{
  msgr_hdr_s                          *pMsg           = NULL;
  pm_mmgsdi_session_id_table_entry_t  *ssn_entry;
  boolean                              infoHasChanged = FALSE;

  ssn_entry = pm_mmgsdi_ssn_tbl_get_entry_by_type(sessionType);

  if (NULL == ssn_entry)
  {
    goto Done;
  }

  infoHasChanged = session_availability_info_has_changed(ssn_entry, notifyType, appType);

  if (infoHasChanged)
  {
    policyman_uim_evt_cb_t  *pCmd;

    pCmd = POLICYMAN_CMD_WITH_HOOK_NEW( policyman_uim_evt_cb_t,
                                        pm_process_uim_evt,
                                        NULL,
                                        NULL,
                                        policyman_cfgitem_update_notification_hook );

    pCmd->mmgsdi_event.session_id = ssn_entry->session_id;

    // Create MMGSDI Command based on CM info
    switch (notifyType)
    {
      case SESSION_AVAILABLE:
        pCmd->mmgsdi_event.evt = MMGSDI_SUBSCRIPTION_READY_EVT;
        pCmd->mmgsdi_event.data.subscription_ready.app_info.app_data.app_type = appType;
        break;

      case SESSION_NOT_AVAILABLE:
        pCmd->mmgsdi_event.evt = MMGSDI_SESSION_CHANGED_EVT;
        pCmd->mmgsdi_event.data.session_changed.activated = FALSE;
        break;

      case SESSION_REFRESHED:
        pCmd->mmgsdi_event.evt = MMGSDI_REFRESH_EVT;
        break;

      default:
        break;
    }

    POLICYMAN_MSG_HIGH_0("blocking caller of policyman_report_session_availability()");
    pCmd->signal = policyman_client_block_sig_get();
    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    policyman_client_block_signal_wait(pCmd->signal);

    pMsg = pCmd->clientMsg.pMsg;
    ref_cnt_obj_release(pCmd);
  }

Done:
  POLICYMAN_MSG_HIGH_6( "return from report_session_availability(notifyT %d, sessT %d, appT %d), ssn_entry 0x%0x, infoHasChanged %d, pMsg 0x%x",
                        notifyType,
                        sessionType,
                        appType,
                        ssn_entry,
                        infoHasChanged,
                        pMsg );
  return pMsg;
}

/*-------- evaluate_have_imsi --------*/
STATIC boolean evaluate_have_imsi(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_condition_t const *pCond  = (policyman_condition_t const *)pCondition;
  mre_policy_t                *pPolicy  = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t           *pState   = POLICY_EXECUTE_STATE(pCtx);
  boolean result;

  result = imsi_is_present(pState, POLICY_SUBS(pPolicy));

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX SRC_LINE "<have_imsi> returns %d",
                        POLICY_SUBS(pPolicy),
                        LINE_NUM(pCond),
                        result );

  return result;
}

/*-------- policyman_condition_have_imsi_evaluate --------*/
STATIC boolean policyman_condition_have_imsi_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_have_imsi);
}

/*-------- policyman_condition_have_imsi_new --------*/
mre_status_t policyman_condition_have_imsi_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  policyman_condition_t *pCondition;

  pCondition = POLICYMAN_CONDITION_NEW( policyman_condition_t,
                                        pPolicy,
                                        policyman_condition_have_imsi_evaluate,
                                        policyman_condition_dtor,
                                        SUBS_THIS,
                                        pElem );

  *ppCondition = (mre_condition_t *)pCondition;

  return MRE_STATUS_SUCCESS;
}

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  mmgsdi_session_type_enum_type session_type;
  policyman_fdn_status_t        fdnStatus;
} policyman_fdn_status_condition_t;



static policyman_fdn_status_t policyman_str_to_fdn_status(
  const char  *pFdn_status
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("NONE",      POLICYMAN_FDN_STATUS_NONE),
    MRE_STR_VALUE_ENTRY("ENABLED",   POLICYMAN_FDN_STATUS_ENABLED),
    MRE_STR_VALUE_ENTRY("DISABLED",  POLICYMAN_FDN_STATUS_DISABLED)
  };

  char                    token[32];
  policyman_fdn_status_t  fdn_status = POLICYMAN_FDN_STATUS_NONE;

  if (policyman_get_token(&pFdn_status, token, sizeof(token)))
  {
    MRE_LOOKUP_STR_VALUE_NO_STATUS(map, token, policyman_fdn_status_t, &fdn_status);
  }

  return fdn_status;
}


/*-------- evaluate_fdn_status --------*/
static boolean evaluate_fdn_status(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_fdn_status_condition_t const    *pCond    = (policyman_fdn_status_condition_t *)pCondition;
  mre_policy_t                              *pPolicy  = EXECUTE_CTX_POLICY(pCtx);
  policyman_state_t                         *pState   = POLICY_EXECUTE_STATE(pCtx); 
  boolean                                    result   = FALSE;
  pm_mmgsdi_session_id_table_entry_t        *ssn_entry;

  ssn_entry = pm_mmgsdi_ssn_tbl_get_entry_by_type(pCond->session_type);
  
  if (NULL == ssn_entry)
  {
    goto Done;
  }

  result = ssn_entry->fdn_status == pCond->fdnStatus;

  Done:
  POLICYMAN_MSG_HIGH_4( SUBS_PREFIX SRC_LINE "condition <fdn_status pmsubs=%d> returns %d",
                        POLICY_SUBS(pPolicy),
                        LINE_NUM(pCond),
                        subsId,
                        result );

  return result;
}


/*-------- policyman_condition_fdn_status_evaluate --------*/
STATIC boolean policyman_condition_fdn_status_evaluate(
  mre_condition_t const  *pCondition,
  void                   *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_fdn_status);
}


/*-------- policyman_condition_fdn_status_new --------*/
mre_status_t policyman_condition_fdn_status_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mre_status_t                          status     = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_fdn_status_condition_t     *pCondition;
  policyman_subs_t                      pm_subs = SUBS_THIS;
  char const                           *pStr;
  char const                           *pStrFdn;
  mmgsdi_session_type_enum_type         session = MMGSDI_GW_PROV_PRI_SESSION;
  policyman_fdn_status_t                fdnStatus = POLICYMAN_FDN_STATUS_NONE;

  pStr = policyman_xml_get_attribute(pElem, "session");
  if (NON_NULL(pStr))
  {
    session = policyman_map_str_to_session(pStr, POLICY_SUBS(pPolicy));
  }

  pStrFdn = policyman_xml_get_text(pElem);

  if (pStrFdn == NULL)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<fdn_status> must specify fdn type", POLICY_SUBS(pPolicy));
    goto Done;
  }

  fdnStatus = policyman_str_to_fdn_status(pStrFdn);

  pCondition = POLICYMAN_CONDITION_NEW( policyman_fdn_status_condition_t,
                                        pPolicy,
                                        policyman_condition_fdn_status_evaluate,
                                        policyman_condition_dtor,
                                        pm_subs,
                                        pElem );

  pCondition->session_type = session;
  pCondition->fdnStatus    = fdnStatus;

  *ppCondition = (mre_condition_t *)pCondition;

  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}


typedef struct
{
  POLICYMAN_CONDITION_BASE;

  char const   *pFdnNumber;
} policyman_fdn_has_condition_t;



/*-------- policyman_condition_fdn_has_dtor --------*/
STATIC void policyman_condition_fdn_has_dtor(
  void  *pObj
)
{
  policyman_fdn_has_condition_t *pCondition = (policyman_fdn_has_condition_t *) pObj;

  POLICYMAN_MEM_FREE_IF(pCondition->pFdnNumber);
  policyman_condition_dtor(pCondition);
}


/*-------- policyman_get_fdn_number_set --------*/
STATIC mre_set_t * policyman_get_fdn_number_set(
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
    mre_set_add(pSet, &token);
  }

Done:
  return pSet;
}


/*-------- evaluate_fdn_has --------*/
STATIC boolean evaluate_fdn_has(
  policyman_condition_t const  *pCondition,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  policyman_fdn_has_condition_t const        *pCond = (policyman_fdn_has_condition_t *) pCondition;
  mre_policy_t                               *pPolicy     = EXECUTE_CTX_POLICY(pCtx);
  mre_set_t                                  *pItemSet    = POLICY_EXECUTE_ITEMSET(pCtx); 
  policyman_state_t                          *pState      = POLICY_EXECUTE_STATE(pCtx); 
  sys_modem_as_id_e_type                      policySubs  = POLICY_SUBS(pPolicy);
  boolean                                     result      = FALSE;
  pbm_phonebook_type                          pb_info;
  pm_mmgsdi_session_id_table_entry_t         *ssn_entry;

  ssn_entry     = pm_mmgsdi_ssn_tbl_get_gw_session_by_subId(subsId);

  if (NULL == ssn_entry)
  {
    goto Done;
  }

  pb_info.device_type = PBM_FDN;
  pb_info.pb_category = PBM_LPB;
  pb_info.slot_id     = ssn_entry->slot_id;
  pb_info.prov_type   = policyman_map_mmgsdi_to_pbm_session_type(ssn_entry->session_type);
  
  if ( PBM_SUCCESS == pbm_session_find_number( pb_info,
                                               (const byte *)pCond->pFdnNumber,
                                               (int)strlen(pCond->pFdnNumber),
                                               NULL,
                                               NULL) )
  {
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }

  Done:
  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX SRC_LINE "condition <fdn_has> returns %d",
                        policySubs,
                        LINE_NUM(pCondition),
                        result );

  return result;
}


/*-------- policyman_condition_fdn_has_evaluate --------*/
STATIC boolean policyman_condition_fdn_has_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_fdn_has);
}


/*-------- policyman_condition_fdn_has_new --------*/
mre_status_t policyman_condition_fdn_has_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mre_status_t                          status     = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_fdn_has_condition_t        *pCondition = NULL;
  char const                           *pStr;
  policyman_subs_t                      pm_subs = SUBS_THIS;
  mre_set_t                            *pFdnNumberSet = NULL;


  /* Does this action indicate the subscription
   *  Default is subscription from policy
   */
  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pStr = policyman_xml_get_text(pElem);

  pCondition = POLICYMAN_CONDITION_NEW( policyman_fdn_has_condition_t,
                                        pPolicy,
                                        policyman_condition_fdn_has_evaluate,
                                        policyman_condition_fdn_has_dtor,
                                        pm_subs,
                                        pElem );

  pCondition->pFdnNumber = mre_str_dup(pStr);

  *ppCondition = (mre_condition_t *)pCondition;

  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}


/*-------- policyman_uim_fill_diag_session_info --------*/
void policyman_uim_fill_diag_session_info(
  size_t                          count,
  pm_dump_uim_session_info_t     *dump_uim_info
)
{
  size_t     idx;

  if (count > ARR_SIZE(pm_mmgsdi_session_id_table))
  {
    count = ARR_SIZE(pm_mmgsdi_session_id_table);
  }

  for (idx = 0; idx < count; idx++)
  {
    dump_uim_info[idx].session_type = pm_mmgsdi_session_id_table[idx].session_type;
    dump_uim_info[idx].asubs_id     = pm_mmgsdi_session_id_table[idx].asubs_id;
    dump_uim_info[idx].session_id   = pm_mmgsdi_session_id_table[idx].session_id;
    dump_uim_info[idx].app_type     = pm_mmgsdi_session_id_table[idx].app_type;
    dump_uim_info[idx].in_use       = pm_mmgsdi_session_id_table[idx].in_use;
    dump_uim_info[idx].active       = pm_mmgsdi_session_id_table[idx].active;
  }
}

/*-------- policyman_uim_fill_diag_uim_info --------*/
void policyman_uim_fill_diag_uim_info(
  policyman_uim_info_t  *src_uim_info,
  pm_dump_uim_info_t    *dump_uim_info
)
{
  dump_uim_info->plmn.identity[0]      = src_uim_info->plmn.identity[0];
  dump_uim_info->plmn.identity[1]      = src_uim_info->plmn.identity[1];
  dump_uim_info->plmn.identity[2]      = src_uim_info->plmn.identity[2];
  dump_uim_info->sim_app_type          = src_uim_info->sim_app_type;
  dump_uim_info->cdma_app_type         = src_uim_info->cdma_app_type;
  dump_uim_info->sim_refreshed         = src_uim_info->sim_refreshed;
  dump_uim_info->subs_active           = src_uim_info->subs_active;
  dump_uim_info->csim_mcc              = src_uim_info->csim_mcc;
  dump_uim_info->simlock_imsi.imsi_len = src_uim_info->simlock_imsi.imsi_len;
}
