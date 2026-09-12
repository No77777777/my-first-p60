#ifndef _POLICYMAN_CALL_EVENTS_H_
#define _POLICYMAN_CALL_EVENTS_H_

/**
  @file policyman_call_events.h

  @brief
*/

/*
    Copyright (c) 2013-2017,2022 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_call_events.h#2 $
  $DateTime: 2022/02/15 22:29:00 $
  $Author: pwbldsvc $
*/

#include "cm.h"
#include "policyman_i.h"
#include "policyman_diag.h"

typedef struct policyman_cmcall_info_s  policyman_cmcall_info_t;

typedef struct policyman_cmcall_history_entry_s    policyman_cmcall_history_entry_t;
typedef struct policyman_cmcall_history_s          policyman_cmcall_history_t;

/*-------- policyman_cmcall_get_history --------*/
size_t policyman_cmcall_get_history(
  pm_dump_cm_call_info_history_t *h_buffer,
  size_t                          h_len
);

/*-------- policyman_cmcall_fill_diag_call_info --------*/
void policyman_cmcall_fill_diag_call_info(
  sys_modem_as_id_e_type     asubs_id,
  policyman_cmcall_info_t   *subs_ph_info,
  pm_dump_cm_call_info_t    *dump_ph_info
);

/*-------- policyman_cmcall_init_history --------*/
void policyman_cmcall_init_history(
  policyman_state_t  *pState
);

/*-------- policyman_cmcall_deinit_history --------*/
void policyman_cmcall_deinit_history(
  policyman_state_t  *pState
);

/*-------- policyman_cmcall_init --------*/
boolean policyman_cmcall_init(
  policyman_cmcall_info_t **ppInfo,
  sys_modem_as_id_e_type    asubs_id
);

/*-------- policyman_cmcall_deinit --------*/
void policyman_cmcall_deinit(
  policyman_cmcall_info_t *pInfo
);

/*-------- policyman_cmcall_info_deep_copy --------*/
policyman_cmcall_info_t * policyman_cmcall_info_deep_copy(
  policyman_cmcall_info_t  *pCmCallState
);

/*-------- policyman_cmcall_update_state --------*/
boolean policyman_cmcall_update_state(
  policyman_state_t      *pInfo,
  sys_modem_as_id_e_type  subsId,
  cm_call_event_e_type    call_event,
  cm_call_type_e_type     type,
  uint32                  timestamp
);


/*-------- policyman_cmcall_event_cb --------*/
/**
@brief

@param

@return

*/
void policyman_cmcall_event_cb(
  cm_call_event_e_type           evt,
  cm_mm_call_info_s_type const  *p_info
);


/*-------- policyman_cmcall_subs_is_in_call --------*/
boolean policyman_cmcall_subs_is_in_call(
  policyman_state_t        *pState,
  sys_modem_as_id_e_type    subsId
);


/*=============================================================================
  Phone-related rule conditions
=============================================================================*/


/*-------- policyman_mdump_callinfo --------*/
void policyman_callinfo_mdump(
    policyman_cmcall_info_t *pCmCallInfo
);
#endif /* _POLICYMAN_CALL_EVENTS_H_ */
