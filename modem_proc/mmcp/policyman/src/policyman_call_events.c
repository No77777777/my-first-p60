/**
  @file policyman_call_events.c

  @brief
*/

/*
    Copyright (c) 2013-2017,2020,2022 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_call_events.c#3 $
  $DateTime: 2022/02/16 00:48:54 $
  $Author: pwbldsvc $
*/


#include "policyman_call_events.h"
#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_diag.h"
#include "policyman_policies.h"
#include "policyman_state.h"
#include "policyman_subs.h"
#include "policyman_task.h"
#include "policyman_xml.h"

#include "sys.h"
#include "modem_mem.h"
#include <stringl/stringl.h>


/*=============================================================================
 Data Structure for CM CALL Info
==============================================================================*/
struct policyman_cmcall_info_s
{
  cm_call_event_e_type      call_event;       // Indicates the current call event.
  cm_call_type_e_type       call_type;        // Indicates the call type
};

/*=============================================================================
  Call Event callback definition.
=============================================================================*/
typedef struct
{
  POLICYMAN_CMD_HDR;
  cm_call_event_e_type    evt;
  cm_mm_call_info_s_type  info;
  uint32                  timestamp;
} policyman_cmcall_evt_cb_t;

/*=============================================================================
  Call Event History definitions
=============================================================================*/
struct policyman_cmcall_history_entry_s {
    size_t                  next;
    uint32                  timestamp;
    sys_modem_as_id_e_type  subs_id;
    cm_call_event_e_type    call_event;
    cm_call_type_e_type     call_type;
};

struct policyman_cmcall_history_s {
  size_t                            index;
  policyman_cmcall_history_entry_t  h[PM_DUMP_HISTORY_DEPTH];
};

/*=============================================================================
  APIs for CM Phone Event information
=============================================================================*/
/*-------- policyman_cmcall_init --------*/
boolean policyman_cmcall_init(
  policyman_cmcall_info_t **ppCallInfo,
  sys_modem_as_id_e_type    asubs_id
)
{
  policyman_cmcall_info_t *pInfo;

  pInfo = (policyman_cmcall_info_t *)policyman_mem_alloc(sizeof(policyman_cmcall_info_t));

  pInfo->call_event = CM_CALL_EVENT_NONE;

  *ppCallInfo = pInfo;
  return (*ppCallInfo != NULL);  // NOTE: ALWAYS true
}

/*-------- policyman_cmcall_deinit --------*/
void policyman_cmcall_deinit(
  policyman_cmcall_info_t *pInfo
)
{
  POLICYMAN_MEM_FREE_IF(pInfo);
}

/*-------- policyman_cmcall_info_deep_copy --------*/
policyman_cmcall_info_t * policyman_cmcall_info_deep_copy(
  policyman_cmcall_info_t  *pCmCallState
)
{
  policyman_cmcall_info_t *pInfo;

  pInfo = (policyman_cmcall_info_t *)policyman_mem_alloc(sizeof(policyman_cmcall_info_t));

  *pInfo = *pCmCallState;

  return pInfo;
}

/*-------- policyman_cmcall_init_history --------*/
void policyman_cmcall_init_history(
  policyman_state_t  *pState
)
{
  if (NULL == pState->pCmcallHistory)
  {
    int   idx;

    pState->pCmcallHistory = (policyman_cmcall_history_t *)policyman_mem_alloc(sizeof(policyman_cmcall_history_t));

    for (idx = 0; idx < (PM_DUMP_HISTORY_DEPTH); ++idx)
    {
      pState->pCmcallHistory->h[idx].next       = idx + 1;
      pState->pCmcallHistory->h[idx].timestamp  = 0xC0FFEE;
      pState->pCmcallHistory->h[idx].call_event = CM_CALL_EVENT_NONE;
      pState->pCmcallHistory->h[idx].call_type  = CM_CALL_TYPE_NONE;
    }

    pState->pCmcallHistory->h[PM_DUMP_HISTORY_DEPTH - 1].next = 0;
  }
}

/*-------- policyman_cmcall_deinit_history --------*/
void policyman_cmcall_deinit_history(
  policyman_state_t  *pState
)
{
  POLICYMAN_MEM_FREE_IF(pState->pCmcallHistory);
}

/*-------- policyman_cmcall_update_history --------*/
void policyman_cmcall_update_history(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subs_id,
  cm_call_event_e_type    call_event,
  cm_call_type_e_type     call_type,
  uint32                  timestamp
)
{
  if (NULL != pState->pCmcallHistory)
  {
    pState->pCmcallHistory->h[pState->pCmcallHistory->index].timestamp  = timestamp;
    pState->pCmcallHistory->h[pState->pCmcallHistory->index].subs_id    = subs_id;
    pState->pCmcallHistory->h[pState->pCmcallHistory->index].call_event = call_event;
    pState->pCmcallHistory->h[pState->pCmcallHistory->index].call_type  = call_type;

    pState->pCmcallHistory->index = pState->pCmcallHistory->h[pState->pCmcallHistory->index].next;
  }
}

/*-------- policyman_cmcall_fill_diag_call_info --------*/
void policyman_cmcall_fill_diag_call_info(
  sys_modem_as_id_e_type     asubs_id,
  policyman_cmcall_info_t   *subs_cmcall_info,
  pm_dump_cm_call_info_t    *dump_cmcall_info
)
{
  dump_cmcall_info->asubs_id    = asubs_id;
  dump_cmcall_info->call_event  = subs_cmcall_info->call_event;
  dump_cmcall_info->call_type   = subs_cmcall_info->call_type;
}

/*-------- policyman_cmcall_history_entry_to_dump --------*/
STATIC void policyman_cmcall_history_entry_to_dump(
  policyman_cmcall_history_entry_t *hist_entry,
  pm_dump_cm_call_info_history_t   *dump_entry
)
{
  dump_entry->timestamp                = hist_entry->timestamp;

  dump_entry->cm_call_info.asubs_id    = hist_entry->subs_id;
  dump_entry->cm_call_info.call_event  = hist_entry->call_event;
  dump_entry->cm_call_info.call_type   = hist_entry->call_type;
}

/*-------- policyman_cmcall_get_history --------*/
size_t policyman_cmcall_get_history(
  pm_dump_cm_call_info_history_t *h_buffer,
  size_t                          h_len
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

  oldest_hist_idx = pState->pCmcallHistory->index;  // points to oldest history record

  // Calculate the index to start with for the most recent h_len history entries
  start_idx = (oldest_hist_idx + (PM_DUMP_HISTORY_DEPTH - h_len)) % PM_DUMP_HISTORY_DEPTH;

  hist_idx = start_idx;

  for (offset = 0; offset < h_len; offset++ )
  {
    policyman_cmcall_history_entry_to_dump( &pState->pCmcallHistory->h[hist_idx],
                                            &h_buffer[offset] );

    hist_idx = pState->pCmcallHistory->h[hist_idx].next;
  }

  return h_len;
}


/*-------- policyman_cmcall_create_and_send_diag_call_log --------*/
void policyman_cmcall_create_and_send_diag_call_log(
  sys_modem_as_id_e_type     asubs_id,
  policyman_cmcall_info_t   *subs_cmcall_info
)
{
  LOG_PM_CALL_HISTORY_INFO_type *log_ptr;

  log_ptr = LOG_PACKET_NEW( LOG_PM_CALL_HISTORY_INFO_C,
                            LOG_PM_CALL_HISTORY_INFO_type );

  if (NULL != log_ptr)
  {
    log_ptr->version      = LOG_PM_CALL_HISTORY_INFO_LOG_PACKET_VERSION;

    policyman_cmcall_fill_diag_call_info(asubs_id, subs_cmcall_info, &log_ptr->cm_call_info);

    log_commit( (void*)log_ptr );
  }
}


/*-------- policyman_cmcall_get_call_event --------*/
cm_call_event_e_type policyman_cmcall_get_call_event(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_cmcall_info_t *pCallInfo;

  pCallInfo = policyman_state_get_call_info(pState, subsId);

  return pCallInfo->call_event;
}


/*-------- policyman_cmcall_get_call_type --------*/
cm_call_type_e_type policyman_cmcall_get_call_type(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  policyman_cmcall_info_t *pCallInfo;

  pCallInfo = policyman_state_get_call_info(pState, subsId);

  return pCallInfo->call_type;
}


/*-------- policyman_cmcall_update_state --------*/
boolean policyman_cmcall_update_state(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId,
  cm_call_event_e_type    call_event,
  cm_call_type_e_type     type,
  uint32                  timestamp
)
{
  boolean                  changed   = FALSE;
  sys_mcc_type             mcc;
  policyman_cmcall_info_t *pCallInfo;

  pCallInfo = policyman_state_get_call_info(pState, subsId);

  /* For MO call, handle call start/end events. For MT calls, handle call incoming/end events
        All other states are intermediate to start-end and they should be treated as in Call.
   */
  if (
       (   call_event == CM_CALL_EVENT_ORIG
        || call_event == CM_CALL_EVENT_CONNECT
        || call_event == CM_CALL_EVENT_INCOM
        || call_event == CM_CALL_EVENT_END
       )
       &&
       call_event != pCallInfo->call_event)
  {
    pCallInfo->call_event = call_event;
    pCallInfo->call_type  = type;

    if(call_event == CM_CALL_EVENT_END )
    {
      mcc = policyman_state_get_subs_mcc(pState, subsId);
      if (NO_MCC == mcc)
      {
        changed = TRUE;
      }
    }

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "updated state; call_event %d, call_type %d",
                          subsId,
                          call_event,
                          type );
  }

  policyman_cmcall_update_history( pState,
                                   subsId,
                                   call_event,
                                   type,
                                   timestamp );
  {
    policyman_cmcall_info_t evt_info;

    evt_info.call_event = call_event;
    evt_info.call_type  = type;
    policyman_cmcall_create_and_send_diag_call_log(subsId, &evt_info);
  }

  return changed;
}

/*===========================================================================
  FUNCTION POLICYMAN_EXECUTE_PROCESS_CM_CALL_EVT()

  DESCRIPTION
    Execute CM Call Set update update and policy check

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void policyman_execute_process_cmcall_evt(
  policyman_cmd_t *pCmd
)
{
  policyman_cmcall_evt_cb_t *pCallEvt     = (policyman_cmcall_evt_cb_t *)pCmd;

  if (NULL != pCallEvt)
  {
    boolean call_state_changed = FALSE;

    POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "process cmcall event; call_event %d, call_type %d",
                          pCallEvt->info.asubs_id,
                          pCallEvt->evt,
                          pCallEvt->info.call_type );

    policyman_state_enter_crit_section();
    call_state_changed = policyman_cmcall_update_state( pCallEvt->pSandbox->pState,
                                                        pCallEvt->info.asubs_id,
                                                        pCallEvt->evt,
                                                        pCallEvt->info.call_type,
                                                        pCallEvt->timestamp );
    policyman_state_leave_crit_section();

    if (call_state_changed)
    {
      policyman_state_handle_update(pCallEvt->pSandbox);
    }
  }
}

/*===========================================================================
  FUNCTION POLICYMAN_CMCALL_EVENT_CB()

  DESCRIPTION
    Handle CM Call event callback

  PARAMETERS
    evt     : CM Call EVENT type
    p_info : actual payload of CM Call EVENT

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void policyman_cmcall_event_cb(
  cm_call_event_e_type           evt,
  cm_mm_call_info_s_type const  *p_info
)
{
  policyman_cmcall_evt_cb_t *pCmd = NULL;

  POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "received cmcall evt %d",
                        p_info->asubs_id,
                        evt );

  pCmd = POLICYMAN_CMD_NEW( policyman_cmcall_evt_cb_t,
                            policyman_execute_process_cmcall_evt,
                            NULL,
                            NULL ); // No policy associated with CM Call Event

  pCmd->evt       = evt;
  pCmd->info      = *p_info;
  pCmd->timestamp = mre_util_get_time();

  policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
  ref_cnt_obj_release(pCmd);
}




/*=============================================================================
  Call-related conditions
=============================================================================*/

/*-----------------------------------------------------------------------------
  Call Event
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;
} policyman_call_event_condition_t;

/*-------- policyman_cmcall_subs_is_in_call --------*/
boolean policyman_cmcall_subs_is_in_call(
  policyman_state_t        *pState,
  sys_modem_as_id_e_type    subsId
)
{
  boolean               in_call;
  cm_call_event_e_type  call_event;
  cm_call_type_e_type   call_type;

  call_event = policyman_cmcall_get_call_event(pState, subsId);
  call_type  = policyman_cmcall_get_call_type(pState, subsId);
  
  // Return TRUE only when Voice or E911 call is going on
  in_call = ( call_type == CM_CALL_TYPE_EMERGENCY
             || call_type == CM_CALL_TYPE_VOICE
            )
            &&
            (   call_event == CM_CALL_EVENT_ORIG
             || call_event == CM_CALL_EVENT_INCOM
             || call_event == CM_CALL_EVENT_CONNECT
            );

  POLICYMAN_UT_MSG_4( SUBS_PREFIX "ue_is_in_call (call_type %d, call_event %d) returns %d",
                      subsId,
                      call_type,
                      call_event,
                      in_call );

  return in_call;
}



QSH_MDUMP_FN_ATTR_PM void policyman_callinfo_mdump(
  policyman_cmcall_info_t *pCmCallInfo
)
{
  policyman_qsh_dump_collect((void *)pCmCallInfo, sizeof(policyman_cmcall_info_t));
}
