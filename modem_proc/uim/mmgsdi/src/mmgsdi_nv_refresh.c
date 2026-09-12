/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 M M G S D I   N V   R E F R E S H  F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains routines used to handle the NV Refresh
  commands.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS



                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2017, 2019, 2024 QUALCOMM Technologies, Inc (QTI) and
its licensors. All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_nv_refresh.c#3 $$ $DateTime: 2024/02/26 16:03:48 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/04/24   cj      Hide CSIM from indication when CDMA_LESS NV is enabled
02/28/19   vm      Send card inserted event for Non-Telecom card
10/26/17   vdc     Do deact and act for clients after NV refresh
08/13/17   bcho    Use subscription based ref id for MCFG API call
10/25/16   me      Removed feature flag FEATURE_GSTK 
07/06/16   av      Send SELECT ISDR to determine if card is eUICC or not
05/20/16   vdc     Remove F3 messages for memory allocation failure
04/26/16   ar      Removing the mmgsdi_cnf_map_req_to_cnf_enum_type()
04/01/16   vdc     Send act resp and session changed evt at same time
01/20/16   ar      Don't RESET card if EF-DIR req is not processed by UIMDRV
09/10/15   kv      Features_status_list is split into device and slot items
09/01/15   bcho    Added mmgsdi bootup log feature
08/31/15   yt      Remove length information from requests and confirmations
05/29/15   kv      Migrated to new MCFG Refresh Done API
04/20/15   kk      Fix MCFG UIM data sent for nv refresh
09/09/14   kk      Enhancements in NV handling
08/29/14   hh      Fix compliation warnings
08/07/14   yt      Avoid NULL pointer dereference
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
07/15/14   kk      NV Refresh: MCFG - MMGSDI interactions
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "uim_msg.h"

#include "nv.h"
#include "err.h"
#include "queue.h"
#include "fs_public.h"

#include "mmgsdi_nv.h"
#include "mmgsdi_nv_refresh.h"
#include "mmgsdiutil.h"
#include "mmgsdisessionlib_v.h"
#include "mmgsdi.h"
#include "mmgsdilib_v.h"
#include "mmgsdi_evt.h"
#include "mmgsdi_card_init.h"
#include "mmgsdi_icc.h"
#include "mmgsdi_log_pkt.h"
#include "mmgsdi_euicc.h"
#include "mmgsdi_gstk.h"
#include "mmgsdi_gen.h"
#include "mmgsdicache.h"
#include "mmgsdi_uicc.h"
#include "mmgsdi_session.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/


/*===========================================================================

  ENUM: MMGSDI_NV_STATE_ENUM_TYPE

  DESCRIPTION:
    This enum describes all the states that a NV/EFS item can have
==========================================================================*/
typedef enum {
  MMGSDI_NV_STATE_NOT_INIT = 0x0,
    /* NV /EFS item is not yet read */
  MMGSDI_NV_STATE_DISABLED,
    /* NV/EFS item is successfully read and value is deactivated */
  MMGSDI_NV_STATE_ENABLED,
    /* NV/EFS item is successfully read and value is activated */
  MMGSDI_NV_STATE_UNCHANGED
    /* NV/EFS value is changed */
}mmgsdi_nv_state_enum_type;


/*===========================================================================
FUNCTION MMGSDI_PROCESS_CONTINUE_AFTER_NV_REFRESH_SLOT

DESCRIPTION
  Processing of continue after nv refresh req for slot from MCFG which posts
  CARD INSERTED evt based on the state of the card.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_process_continue_after_nv_refresh_slot
(
  const mmgsdi_continue_after_nv_refresh_req_type *req_ptr
)
{
  mmgsdi_return_enum_type mmgsdi_status          = MMGSDI_ERROR;
  boolean                 fail_reason_card_error = FALSE;
  boolean                 ef_dir_not_found       = FALSE;
  mmgsdi_session_id_type  card_session_id        = MMGSDI_SESSION_ID_ZERO;
  mmgsdi_nv_context_type  nv_context             = MMGSDI_NV_CONTEXT_INVALID;
  uint8                   slot_index             = MMGSDI_SLOT_1_INDEX;
  mmgsdi_slot_data_type  *slot_data_ptr          = NULL;

  /* Link established processing pending for the slot */
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_slot_index(req_ptr->request_header.slot_id,
                    &slot_index);
  if(mmgsdi_status != MMGSDI_SUCCESS ||
     slot_index >= MMGSDI_MAX_NUM_SLOTS)
  {
    return MMGSDI_ERROR;
  }

  /* Get the Card Slot Session ID corresponding to the slot */
  card_session_id = mmgsdi_generic_data_ptr->slot_session_id[slot_index];

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);

  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  slot_data_ptr->nv_refresh_in_progress = FALSE;

  /* Determine the Protocol used at the SIM - ME Interface */
  switch (slot_data_ptr->protocol)
  {
    case MMGSDI_ICC:
      /* GSM DCS Checking pending */
      /* As per spec 10.7 of GSM 11.11, the DF-DCS should be selected if and only if
         DF-GSM (7F20) is absent.

         NOTE 1: The selection of the GSM application using the identifier '7F21',
         if selection by means of the identifier '7F20' fails, is to ensure backwards
         compatibility with those Phase 1 SIMs which only support the DCS 1800 application
         using the Phase 1 directory DFDCS1800 coded '7F21'.

         NOTE 2: To ensure backwards compatibility with those Phase 1 DCS 1800 MEs,
         which have no means to selectDFGSM two options have been specified.
         These options are given in GSM 09.91 [17].
      */
      nv_context = mmgsdi_util_get_efs_item_index_for_slot(req_ptr->request_header.slot_id);

      /* Look for DCS 1800 DF only for 1X/GSM Targets */
      if (mmgsdi_nv_get_slot_feature_status(MMGSDI_FEATURE_UIM_GSM_DCS_1800,
            nv_context) == MMGSDI_FEATURE_ENABLED)
      {
        if(!slot_data_ptr->gsm_df_present)
        {
          mmgsdi_status = mmgsdi_icc_is_dcs1800_df_present(card_session_id,
            req_ptr->request_header.slot_id);
          if (mmgsdi_status == MMGSDI_SUCCESS)
          {
            UIM_MSG_HIGH_0("DCS 1800 DF Present");

            slot_data_ptr->dcs1800_present = TRUE;
            slot_data_ptr->card_apps_available |= MMGSDI_DCS1800_APP_MASK;
          }
        }
      }
      break;

    case MMGSDI_UICC:
      if(slot_data_ptr->card_type == MMGSDI_CARD_TELECOM)
      {
        mmgsdi_status = mmgsdi_card_init_trigger_tp_dl_for_uicc(req_ptr->request_header.slot_id,
                                                                &fail_reason_card_error,
                                                                &ef_dir_not_found);
      }
      else
      {
        slot_data_ptr->mmgsdi_tp_state = MMGSDI_TP_STATE_NOT_NEEDED;
      }
      break;

    default:
      /* A Protocol was not established at the UIM Interface */
      UIM_MSG_HIGH_2("Invalid Protocol 0x%x for slot 0x%x",
                     slot_data_ptr->protocol, req_ptr->request_header.slot_id);
      break;
  }

  /* Some eUICCs do not have a correct information in the ATR
     regarding eUICC support. In such cases, slot_data_ptr->is_euicc
     could be set to FALSE. We need to find out if it is really an
     eUICC or not by sending a SELECT on ISDR. If SELECT on ISDR
     succeeds, it is an eUICC, otherwise not */
  if(!slot_data_ptr->is_euicc && ef_dir_not_found) 
  {
    boolean is_isdr_found     = FALSE;

    if(MMGSDI_SUCCESS == mmgsdi_euicc_is_isdr_found(req_ptr->request_header.slot_id, 
                                                    &is_isdr_found) &&
       is_isdr_found)
    {
      slot_data_ptr->is_euicc = TRUE;
    }
  }

  if(slot_data_ptr->card_apps_available != 0x00 ||
     slot_data_ptr->is_euicc                    ||
     slot_data_ptr->card_type == MMGSDI_CARD_NON_TELECOM)
  {
    /* Valid applications were found */
    mmgsdi_status = MMGSDI_SUCCESS;
  }
  else if (mmgsdi_card_init_is_toolkit_allowed(MMGSDI_SESSION_ID_ZERO, 
           req_ptr->request_header.slot_id, 
           MMGSDI_APP_USIM) == FALSE &&
           slot_data_ptr->protocol == MMGSDI_UICC &&
           fail_reason_card_error == FALSE)
  {
    /* If fail_reason_card_error is TRUE, it means UIMDRV has not processed
       this cmd because of some internal error or card_error. In that case,
       there is no need to switch the protocol */
    UIM_MSG_HIGH_1("No valid UICC app. found on slot 0x%x, switching protocol to ICC",
                    req_ptr->request_header.slot_id);

    mmgsdi_util_preinit_mmgsdi(card_session_id,MMGSDI_APP_NONE,MMGSDI_REFRESH_RESET,
                               req_ptr->request_header.slot_id,FALSE);

    /* Switch card protocol to ICC */
    mmgsdi_status = mmgsdi_gen_card_reset_sync(req_ptr->request_header.slot_id, UIM_ICC);

    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* a new link establised req will be received */
      return mmgsdi_status;
    }
  }

  /* Need send out card inserted based on mmgsdi_status and slot TP status */
  if (((slot_data_ptr->mmgsdi_tp_state != MMGSDI_TP_STATE_IN_PROGRESS_OK)      &&
       (slot_data_ptr->mmgsdi_tp_state != MMGSDI_TP_STATE_IN_PROGRESS_NOT_OK)) &&
      (slot_data_ptr->mmgsdi_state != MMGSDI_STATE_CARD_INSERTED))
  {
    UIM_MSG_HIGH_1("MMGSDI_CARD_INSERTED for SLOT 0x%x",
                   req_ptr->request_header.slot_id);
    if(slot_data_ptr->card_type == MMGSDI_CARD_TELECOM)
    {
      mmgsdi_evt_build_and_send_card_inserted(
        TRUE, (mmgsdi_client_id_type)0, req_ptr->request_header.slot_id);
    }
    else
    {
      mmgsdi_evt_build_and_send_non_telecom_card_inserted(TRUE,
                                                          MMGSDI_CLIENT_ID_ZERO,
                                                          req_ptr->request_header.slot_id);
    } 
  }
  return MMGSDI_SUCCESS;
}/* mmgsdi_process_continue_after_nv_refresh_slot */


/*===========================================================================
FUNCTION MMGSDI_PROCESS_CONTINUE_AFTER_NV_REFRESH_SESSION

DESCRIPTION
  Processing of continue after nv refresh req for sessions  from MCFG which
  posts SESSION CHANGED evt based on the state of application.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_process_continue_after_nv_refresh_session
(
  const mmgsdi_continue_after_nv_refresh_req_type *req_ptr
)
{
  mmgsdi_return_enum_type        mmgsdi_status     = MMGSDI_ERROR;
  mmgsdi_session_id_type         prov_sessions[2]  = {MMGSDI_SESSION_ID_ZERO};
  uint8                          index             = 0;
  mmgsdi_channel_info_type      *channel_info_ptr  = NULL;
  mmgsdi_session_info_type      *session_info_ptr  = NULL;
  mmgsdi_int_app_info_type      *sel_app_ptr       = NULL;
  mmgsdi_evt_session_notify_type notify_type;

  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  memset(&notify_type, 0x00, sizeof(mmgsdi_evt_session_notify_type));

  switch(req_ptr->nv_context)
  {
    case MMGSDI_NV_CONTEXT_PRIMARY:
      prov_sessions[0] = mmgsdi_generic_data_ptr->pri_gw_session_id;
      prov_sessions[1] = mmgsdi_generic_data_ptr->pri_1x_session_id;
      break;

    case MMGSDI_NV_CONTEXT_SECONDARY:
      prov_sessions[0] = mmgsdi_generic_data_ptr->sec_gw_session_id;
      prov_sessions[1] = mmgsdi_generic_data_ptr->sec_1x_session_id;
      break;

    case MMGSDI_NV_CONTEXT_TERTIARY:
      prov_sessions[0] = mmgsdi_generic_data_ptr->ter_gw_session_id;
      prov_sessions[1] = mmgsdi_generic_data_ptr->ter_1x_session_id;
      break;

    default:
      return MMGSDI_ERROR;
  }

  /* Compare reference_id with ref id from app associated with provisioning sessions on this slot 
     no of types of RAT tech that mmgsdi supports in prov sessions are 2 - GW and 1x */
  for(index = 0; index < sizeof(prov_sessions) / sizeof(prov_sessions[0]); index++)
  {
    sel_app_ptr = NULL;
    channel_info_ptr = NULL;

    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      prov_sessions[index],
                      NULL,
                      NULL,
                      &sel_app_ptr,
                      &channel_info_ptr,
                      NULL,
                      NULL);

    if(mmgsdi_status == MMGSDI_SUCCESS &&
       channel_info_ptr != NULL &&
       channel_info_ptr->slot_id == req_ptr->request_header.slot_id &&
       sel_app_ptr != NULL &&
       sel_app_ptr->nv_refresh_ref_count == req_ptr->reference_id)
    {
      UIM_MSG_HIGH_1("Received ref id matches with subscription based ref id for session id: 0x%x",
                     prov_sessions[index]);
      break;
    }
  }

  if(index == sizeof(prov_sessions) / sizeof(prov_sessions[0]))
  {
    return MMGSDI_ERROR;
  }

  /* NV refresh after prov session activation  -
     send out session changed evt for all applicable sessions.
     req_ptr->nv_context indicates the session type on which pending activations.
     Check if gw and 1x sessions are waiting for NV refresh -pri_gw_session_id
     if so, trigger session changed for each session types */
  for(index = 0; index < sizeof(prov_sessions) / sizeof(prov_sessions[0]); index++)
  {
    sel_app_ptr = NULL;
    channel_info_ptr = NULL;
    session_info_ptr = NULL;

    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      prov_sessions[index],
                      NULL,
                      NULL,
                      &sel_app_ptr,
                      &channel_info_ptr,
                      &session_info_ptr,
                      NULL);
    if((mmgsdi_status != MMGSDI_SUCCESS) || (sel_app_ptr == NULL) ||
       (session_info_ptr == NULL) || (channel_info_ptr == NULL) ||
       (channel_info_ptr->slot_id != req_ptr->request_header.slot_id))
    {
      UIM_MSG_ERR_3("index 0x%x session inactive,chnnl_info_p=0x%x,app_info_p=0x%x",
                    index,channel_info_ptr, sel_app_ptr);
      continue;
    }

    if(sel_app_ptr != NULL &&
       sel_app_ptr->nv_refresh_in_progress)
    {
      uint8 prov_index = MMGSDI_MAX_PROV_APPS;

      /* trigger sending out Session changed event
         only if nv refresh was in progress*/
      UIM_MSG_HIGH_1("After nv refresh index: 0x%x session active sending out session changed evt",
                      index);

      prov_index = mmgsdi_util_get_prov_index_from_session(session_info_ptr->session_type);

      /* Send the confirmation for activation before sending session changed event */
      if(prov_index < MMGSDI_MAX_PROV_APPS &&
         mmgsdi_generic_data_ptr->mmgsdi_switch_act_cnf_ptr[prov_index] != NULL &&
         mmgsdi_generic_data_ptr->mmgsdi_switch_act_cnf_ptr[prov_index]->client_req_cb != NULL)
      {
        (mmgsdi_generic_data_ptr->mmgsdi_switch_act_cnf_ptr[prov_index]->client_req_cb)
          (mmgsdi_status,
          MMGSDI_SESSION_ACT_OR_SWITCH_PROV_REQ,
          &mmgsdi_generic_data_ptr->mmgsdi_switch_act_cnf_ptr[prov_index]->mmgsdi_cnf);

        MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_generic_data_ptr->mmgsdi_switch_act_cnf_ptr[prov_index]);
      }

      sel_app_ptr->nv_refresh_in_progress = FALSE;
      notify_type.notify_type = MMGSDI_EVT_NOTIFY_ALL_SESSIONS;
      notify_type.slot_id     = channel_info_ptr->slot_id;
      mmgsdi_evt_build_and_send_session_changed(
        notify_type, TRUE, (const mmgsdi_int_app_info_type*)sel_app_ptr);
    }
  }

  return MMGSDI_SUCCESS;
}/* mmgsdi_process_continue_after_nv_refresh_session */


/*===========================================================================
FUNCTION MMGSDI_PROCESS_CONTINUE_AFTER_NV_REFRESH

DESCRIPTION
  Processing of continue after nv refresh req from MCFG which posts
  CARD INSERTED evt or SESSION CHANGED evt based on the state
  of the card or state of application.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_process_continue_after_nv_refresh(
  const mmgsdi_continue_after_nv_refresh_req_type *req_ptr)
{
  mmgsdi_return_enum_type        mmgsdi_status     = MMGSDI_ERROR;
  mmgsdi_slot_data_type         *slot_data_ptr     = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  UIM_MSG_HIGH_3("Continue after NV Refresh for slot: 0x%x and context 0x%x, received for ref id: 0x%x",
                 req_ptr->request_header.slot_id,
                 req_ptr->nv_context,
                 req_ptr->reference_id);

  /* Before processing further - check if the reference id matches
     Reference id is used when back to back MCFG UPDATE UIM DATA
     APIs are called during back to back hotswaps etc. MMGSDI has to
     ignore if the reference id do not match. If the reference id do
     not match, continue after nv refresh trigger from MCFG might be
     related to old MCFG NV Refresh triggers  */
  if(slot_data_ptr->nv_refresh_in_progress &&
     slot_data_ptr->nv_refresh_reference_count == req_ptr->reference_id)
  {
    UIM_MSG_HIGH_0("Received ref id matches with slot based ref id");

    mmgsdi_status = mmgsdi_process_continue_after_nv_refresh_slot(req_ptr);
  }
  else
  {
    mmgsdi_status = mmgsdi_process_continue_after_nv_refresh_session(req_ptr);
  }
  return mmgsdi_status;
}/*mmgsdi_process_continue_after_nv_refresh*/


/* ==========================================================================
FUNCTION MMGSDI_NV_REFRESH_INTERNAL_CB

DESCRIPTION
  This function, defined as mmgsdi_callback_type, will handle the internal
  NV refresh commands

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
----------------------------------------------------------------------------*/
static void mmgsdi_nv_refresh_internal_cb(
  mmgsdi_return_enum_type  status,
  mmgsdi_cnf_enum_type     cnf,
  const mmgsdi_cnf_type   *cnf_ptr)
{
  mmgsdi_data_type app_data                   = {0, NULL};
  uint8            app_id[MMGSDI_MAX_AID_LEN] = {0};

  UIM_MSG_HIGH_2("MMGSDI_NV_REFRESH_INTERNAL_CB: Rev'd Cnf=0x%x, Status=0x%x", cnf, status);

  MMGSDIUTIL_CHECK_NON_GSDI_TASK_CLIENT_APP_DATA_CRIT_SECT_ACCESS;

  if (cnf_ptr == NULL ||
      (cnf != MMGSDI_SESSION_DEACTIVATE_REQ &&
       cnf != MMGSDI_SESSION_ACTIVATE_OR_SWITCH_PROVISIONING_CNF) ||
      status  != MMGSDI_SUCCESS)
  {
    return;
  }

  if (cnf == MMGSDI_SESSION_ACTIVATE_OR_SWITCH_PROVISIONING_CNF)
  {
    mmgsdi_slot_data_type* slot_data_ptr = NULL;

    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(cnf_ptr->session_activate_or_switch_provisioning_cnf.response_header.slot_id);

    if(slot_data_ptr != NULL)
    {
      slot_data_ptr->nv_ref_app_deact_count--;
#ifdef FEATURE_MODEM_CONFIG_REFRESH
      /* Reference id is in client data */
      mcfg_refresh_done_w_status(cnf_ptr->session_activate_or_switch_provisioning_cnf.response_header.client_data,
                                 MCFG_REFRESH_SUCCESS);
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
    }
    return;
  }
} /* mmgsdi_nv_refresh_internal_cb */


/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_DEACT_APP_IF_NEEDED

DESCRIPTION
  This function will determine if app deactivation and activation needed or not.
  We are doing this because It may be possible IMSI_M is needed by MCFG
  to load correct MBN. but if CSIM activation started late and till then
  USIM already cached files and published subready with wrong MBN then
  we should reactivate the USIM app when CSIM update the IMSI_M to mcfg and correct 
  MBN is loaded by mcfg. while doing deactivation and activation,
  MMGSDI will cache and validate the files again depending upon the correct MBN
  This will help to remove the issue like GBU bit issue in UST table.

  If app state is greater than MMGSDI_APP_STATE_READY_FOR_NV_REFRESH then re-activate 
  the app.

PARAMETER
  uint8                                : slot_index
  mcfg_refresh_index_type sub_index[In]: sub index
  uint32                               : reference_id
  mmgsdi_nv_state_enum_type            : cdma_less_nv_after_nv_refresh
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 number of apps to be deactivated

SIDE EFFECTS
  None
===========================================================================*/
static boolean mmgsdi_nv_refresh_deact_app_if_needed(
  uint8                      slot_index,
  mcfg_refresh_index_type    sub_index,
  uint32                     reference_id,
  mmgsdi_nv_state_enum_type  cdma_less_nv_after_nv_refresh)
{
  mmgsdi_return_enum_type       mmgsdi_status                       = MMGSDI_ERROR;
  mmgsdi_session_id_type        prov_sess_id[2]                     = {MMGSDI_INVALID_SESSION_ID};
  mmgsdi_int_app_info_type     *prov_app_info_ptr[2]                = {NULL};
  uint8                         i                                   = 0;
  mmgsdi_slot_id_enum_type      slot_id                             = MMGSDI_SLOT_NONE;
  mmgsdi_slot_id_enum_type      deactivated_slot_id                 = MMGSDI_SLOT_NONE;
  uint8                         num_of_app_deact                    = 0;
  mmgsdi_slot_data_type*        slot_data_ptr                       = NULL;
  boolean                       hold_mcfg_refresh_till_sess_act     = FALSE;
  mmgsdi_data_type              app_data                            = {0, NULL};
  mmgsdi_data_type              app_1x_data;
  boolean                       app_1x_found                        = FALSE;

  memset(&app_1x_data, 0x00, sizeof(mmgsdi_data_type));

  if(mmgsdi_generic_data_ptr == NULL)
  {
    return hold_mcfg_refresh_till_sess_act;
  }
  
  switch (sub_index)
  {
    case MCFG_REFRESH_INDEX_0:
       prov_sess_id[0] = mmgsdi_generic_data_ptr->pri_gw_session_id;
       prov_sess_id[1] = mmgsdi_generic_data_ptr->pri_1x_session_id;
      break;
    case MCFG_REFRESH_INDEX_1:
       prov_sess_id[0] = mmgsdi_generic_data_ptr->sec_gw_session_id;
       prov_sess_id[1] = mmgsdi_generic_data_ptr->sec_1x_session_id;
      break;
    case MCFG_REFRESH_INDEX_2:
       prov_sess_id[0] = mmgsdi_generic_data_ptr->ter_gw_session_id;
       prov_sess_id[1] = mmgsdi_generic_data_ptr->ter_1x_session_id;
      break;
    default:
      return hold_mcfg_refresh_till_sess_act;
  }

  for(i = 0; i < (sizeof(prov_sess_id) / sizeof(mmgsdi_session_id_type)); i++)
  {
    mmgsdi_status = mmgsdi_util_get_slot_id(slot_index, &slot_id);

    if(MMGSDI_SUCCESS != mmgsdi_status ||
       !mmgsdi_util_is_slot_valid(slot_id))
    {
      mmgsdi_status = mmgsdi_util_get_session_app_info(
                             prov_sess_id[i], &slot_id,
                             NULL, NULL, NULL, NULL, NULL);
    }

    if(MMGSDI_SUCCESS != mmgsdi_status ||
       !mmgsdi_util_is_slot_valid(slot_id))
    {
      UIM_MSG_ERR_0("No valid slot found for deactivating app");
      continue;
    }

    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      prov_sess_id[i],
                      NULL,
                      NULL,
                      &prov_app_info_ptr[i],
                      NULL,
                      NULL,
                      NULL);

    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
    /* If app state is greater than MMGSDI_APP_STATE_READY_FOR_NV_REFRESH then deactivate 
       and activate it again it may possible app published sub ready with wrong MBN. 
    */
    if(slot_data_ptr != NULL              &&
       mmgsdi_status == MMGSDI_SUCCESS     &&
       prov_app_info_ptr[i] != NULL        &&
         (prov_app_info_ptr[i]->app_data.app_type == MMGSDI_APP_CSIM && 
          prov_app_info_ptr[i]->app_state >= MMGSDI_APP_STATE_ACTIVATED && 
         cdma_less_nv_after_nv_refresh ==  MMGSDI_NV_STATE_ENABLED))
    {
      (void)mmgsdi_cache_delete(prov_app_info_ptr[i], 0, NULL);
      if(mmgsdi_session_deactivate_provisioning(prov_sess_id[i],
                                                mmgsdi_nv_refresh_internal_cb,
                                                0)!= MMGSDI_SUCCESS)
      {
        UIM_MSG_ERR_1("Not able to deactivate app 0x%x", prov_app_info_ptr[i]->app_data.app_type);
      }
      else
      {
        slot_data_ptr->nv_ref_app_deact_count++;
        num_of_app_deact++;
        deactivated_slot_id = slot_id;
      }
    }
    else if (cdma_less_nv_after_nv_refresh ==  MMGSDI_NV_STATE_DISABLED)
    {
      mmgsdi_util_find_first_ruim_csim_app(slot_id, &app_1x_found, &app_1x_data);
      if (app_1x_found)
      {
        app_data.data_len = app_1x_data.data_len;
        app_data.data_ptr = app_1x_data.data_ptr;

        /* Activate CSIM app if CDMA less NV has changed and it is disabled now after
           refresh */
        if(mmgsdi_session_activate_or_switch_provisioning(prov_sess_id[i],
                                                          slot_id,
                                                          app_data,
                                                          FALSE,
                                                          mmgsdi_nv_refresh_internal_cb,
                                                          reference_id) != MMGSDI_SUCCESS)
        {
          UIM_MSG_ERR_0("Not able to activate CSIM app");
        }
      }
    }
  }

  if(num_of_app_deact > 0)
  {
    UIM_MSG_MED_2("num_of_app_deact 0x%x in slot 0x%x", 
                  num_of_app_deact, deactivated_slot_id);
    hold_mcfg_refresh_till_sess_act = TRUE;
  }
  
  return hold_mcfg_refresh_till_sess_act;
}/* mmgsdi_nv_refresh_deact_app_if_needed */


/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_CHANGE_GBA_U_SETTINGS

DESCRIPTION
  This function deactivates and activates GW session to change GBA_U settings

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_refresh_change_gba_u_settings(
  mmgsdi_session_id_type  session_id
)
{
  mmgsdi_slot_id_enum_type  slot_id        = MMGSDI_SLOT_NONE;
  mmgsdi_int_app_info_type *app_info_ptr   = NULL;
  mmgsdi_return_enum_type   mmgsdi_status  = MMGSDI_SUCCESS;

  mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                   &slot_id,
                                                   NULL,
                                                   &app_info_ptr,
                                                   NULL,
                                                   NULL,
                                                   NULL);

  /* Check the following before sending deactivation and subsequent activation:
     1) Only for the cases when the app is USIM. We are limiting this to only
        GW subscription because clients are reading the GBA service bit in
        EF-UST before non-standard NV refresh.
     2) USIM app state is atleast ready for perso (i.e., PIN event is sent out).*/
  if(mmgsdi_status == MMGSDI_SUCCESS &&
     app_info_ptr != NULL &&
     app_info_ptr->app_data.app_type == MMGSDI_APP_USIM &&
     app_info_ptr->app_state > MMGSDI_APP_STATE_PIN_EVT_SENT)
  {
    mmgsdi_static_data_type app_info = {0, {0}};
    mmgsdi_file_enum_type   ust_file = MMGSDI_USIM_UST;

    /* Delete UST cache so that the updated GBA bit is written into UST cache
       after app activation */
    (void)mmgsdi_cache_delete_item(session_id,
                                   1,
                                   &ust_file,
                                   0,
                                   NULL);

    mmgsdi_memscpy(&app_info,
                   sizeof(app_info),
                   &app_info_ptr->app_data.aid,
                   sizeof(app_info_ptr->app_data.aid));

    UIM_MSG_HIGH_0("Resetting the GW session after NV Refresh");

    /* The deactivation is done to ensure that the clients of MMGSDI re-read
       the EF-UST for updated GBA service bit based on refreshed DISABLE_GBA_U
       NV value after activation is done in the call back. */
    (void)mmgsdi_session_deactivate_provisioning(session_id,
                                                 mmgsdi_nv_refresh_internal_cb,
                                                 0);
  }
} /* mmgsdi_nv_refresh_change_gba_u_settings */


/*===========================================================================
FUNCTION MMGSDI_PROCESS_NV_REFRESH_REQ

DESCRIPTION
  This function processes nv refresh req - slot and session nv/efs items
  are refreshed.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_process_nv_refresh_req(
  const mmgsdi_handle_nv_refresh_req_type *req_ptr)
{
  mmgsdi_return_enum_type         mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_feature_status_enum_type gbau_feature_status = MMGSDI_FEATURE_DISABLED;
  mmgsdi_nv_context_type          nv_context          = MMGSDI_NV_CONTEXT_INVALID;
  mmgsdi_session_id_type          session_id          = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_session_id_type          session_id_1x       = MMGSDI_INVALID_SESSION_ID;
  boolean                         gba_srv_avail       = FALSE;
  mmgsdi_nv_state_enum_type       cdma_less_nv_before_nv_refresh = MMGSDI_NV_STATE_NOT_INIT;
  mmgsdi_nv_state_enum_type       cdma_less_nv_after_nv_refresh  = MMGSDI_NV_STATE_NOT_INIT;
  boolean                         hold_refresh_till_sess_act     = FALSE;

  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, mmgsdi_generic_data_ptr);

  UIM_MSG_HIGH_3("Received NV Refresh of type 0x%x - on slot: 0x%x, session: 0x%x",
                req_ptr->type, req_ptr->slot_index, req_ptr->sub_index);

  /*invoke NV refresh procedures here */
  if (req_ptr->type == MCFG_REFRESH_TYPE_SLOT || req_ptr->type == MCFG_REFRESH_TYPE_SLOT_N_SUBS)
  {
    switch(req_ptr->slot_index)
    {
      case MCFG_REFRESH_INDEX_0:
        nv_context = MMGSDI_NV_CONTEXT_PRIMARY;
        session_id = mmgsdi_generic_data_ptr->pri_gw_session_id;
        session_id_1x = mmgsdi_generic_data_ptr->pri_1x_session_id;
        break;

      case MCFG_REFRESH_INDEX_1:
        nv_context = MMGSDI_NV_CONTEXT_SECONDARY;
        session_id = mmgsdi_generic_data_ptr->sec_gw_session_id;
        session_id_1x = mmgsdi_generic_data_ptr->sec_1x_session_id;
        break;

      case MCFG_REFRESH_INDEX_2:
        nv_context = MMGSDI_NV_CONTEXT_TERTIARY;
        session_id = mmgsdi_generic_data_ptr->ter_gw_session_id;
        session_id_1x = mmgsdi_generic_data_ptr->ter_1x_session_id;
        break;

      default:
        return MMGSDI_INCORRECT_PARAMS;
    }

    /* Store old GBA_U NV feature status before refreshing the NV values.
       We do this to check this value against the GBA_U NV after NV refresh.
       If the NV value has changed then send deactivation and activation for GW session */
    gbau_feature_status = mmgsdi_nv_get_slot_feature_status(MMGSDI_FEATURE_DISABLE_GBA_U, nv_context);

#ifdef FEATURE_MMGSDI_3GPP2
    /* Store nv value before refresh */
    cdma_less_nv_before_nv_refresh = 
    (mmgsdi_nv_get_slot_feature_status(MMGSDI_FEATURE_CDMA_LESS, nv_context) == MMGSDI_FEATURE_ENABLED)?
       MMGSDI_NV_STATE_ENABLED : MMGSDI_NV_STATE_DISABLED;
#endif /* FEATURE_MMGSDI_3GPP2 */

    mmgsdi_status = mmgsdi_nv_refresh_slot_items(req_ptr->slot_index);
    UIM_MSG_HIGH_1("Refresh of slot-based items status: 0x%x", mmgsdi_status);

#ifdef FEATURE_MMGSDI_3GPP2
    /* Read the current nv value and compare it with value before nv refresh */
    cdma_less_nv_after_nv_refresh = 
    (mmgsdi_nv_get_slot_feature_status(MMGSDI_FEATURE_CDMA_LESS, nv_context) == MMGSDI_FEATURE_ENABLED)?
       MMGSDI_NV_STATE_ENABLED : MMGSDI_NV_STATE_DISABLED;

    if(cdma_less_nv_before_nv_refresh == cdma_less_nv_after_nv_refresh)
    {
      cdma_less_nv_after_nv_refresh = MMGSDI_NV_STATE_UNCHANGED;
    }
#endif /* FEATURE_MMGSDI_3GPP2 */
  }

  if (req_ptr->type == MCFG_REFRESH_TYPE_SUBS || req_ptr->type == MCFG_REFRESH_TYPE_SLOT_N_SUBS)
  {
    mmgsdi_status = mmgsdi_nv_refresh_session_items(req_ptr->sub_index);
    UIM_MSG_HIGH_1("Refresh of session-based items status: 0x%x", mmgsdi_status);

    /* If app state is greater than MMGSDI_APP_STATE_READY_FOR_NV_REFRESH then deactivate 
       and activate it again. 
     
       This function is not called while handling Slot based MCFG refresh because
       no apps are activated when we receive slot based MCFG refresh.
       Slot based MCFG refresh takes place before card inserted event.
    */
    hold_refresh_till_sess_act = mmgsdi_nv_refresh_deact_app_if_needed(req_ptr->slot_index,
                                                                       req_ptr->sub_index,
                                                                       req_ptr->reference_id,
                                                                       cdma_less_nv_after_nv_refresh);

    UIM_MSG_HIGH_2("Refresh of session-based items status: 0x%x, hold_refresh_till_sess_act: 0x%x", mmgsdi_status, hold_refresh_till_sess_act);
  }

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  if(!hold_refresh_till_sess_act)
  {
    /* Trigger MCFG that the procedure is done */
    (void)mcfg_refresh_done_w_status(req_ptr->reference_id, MCFG_REFRESH_SUCCESS);
  }
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

  /* Change GBA_U setting when below conditions are satisified:
     1) NV refresh type is "slot and subs".
     2) Old GBA_U NV feature status is not equal to refreshed GBA_U NV feature status.
     3) GBA service bit is enabled in USIM service table.*/
  if(MCFG_REFRESH_TYPE_SLOT_N_SUBS == req_ptr->type &&
     gbau_feature_status != mmgsdi_nv_get_slot_feature_status(MMGSDI_FEATURE_DISABLE_GBA_U, nv_context) &&
     MMGSDI_SUCCESS == mmgsdi_uicc_chk_srv_available(session_id,
                                                     MMGSDI_CHK_SRV_FROM_CARD_OR_CACHE,
                                                     MMGSDI_USIM_SRV_GBA,
                                                     &gba_srv_avail) &&
     gba_srv_avail)
  {
    mmgsdi_nv_refresh_change_gba_u_settings(session_id);
  }

  /* Building Bootup Logs */
  mmgsdi_log_pkt_nv_items();

  return MMGSDI_SUCCESS;
}/*mmgsdi_process_nv_refresh_req*/


/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_MCFG_CB

DESCRIPTION
  Call back registered with MCFG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean mmgsdi_nv_refresh_mcfg_cb(
  mcfg_refresh_info_s_type *mcfg_refresh_info_ptr)
{
  mmgsdi_task_cmd_type              *task_cmd_ptr   = NULL;
  mmgsdi_handle_nv_refresh_req_type *msg_ptr        = NULL;
  uint32                             task_cmd_len   = 0;
  mmgsdi_return_enum_type            mmgsdi_status  = MMGSDI_ERROR;

  if(mcfg_refresh_info_ptr == NULL)
  {
    UIM_MSG_ERR_0("MCFG REFRESH INFO PTR NULL");
    return FALSE;
  }

  /* Memory allocation for msg_ptr and check for allocation */
  task_cmd_len = sizeof(mmgsdi_task_cmd_type);

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(task_cmd_ptr,
                                     task_cmd_len);
  if(task_cmd_ptr == NULL)
  {
   return FALSE;
  }

  msg_ptr = &task_cmd_ptr->cmd.cmd.mcfg_refresh_data;

  /* Populating mmgsdi_continue_after_nv_refresh header
     1) Client ID
     2) Request TYPE
     3) Slot ID
     4) Client Data Pointer
     5) Response CallBack
     6) Event Callback
     7) Payload len and request len will be populated after the content
        has been populated */

  task_cmd_ptr->cmd.cmd_enum                = MMGSDI_HANDLE_NV_REFRESH_REQ;
  msg_ptr->request_header.request_type      = MMGSDI_HANDLE_NV_REFRESH_REQ;
  msg_ptr->request_header.orig_request_type = MMGSDI_HANDLE_NV_REFRESH_REQ;
  msg_ptr->request_header.client_id         = mmgsdi_generic_data_ptr->client_id;
  msg_ptr->request_header.session_id        = 0;
  msg_ptr->request_header.client_data       = 0;
  msg_ptr->request_header.response_cb       = NULL;

  /*store mcfg refresh data */
  msg_ptr->type         = mcfg_refresh_info_ptr->type;
  msg_ptr->slot_index   = mcfg_refresh_info_ptr->slot_index;
  msg_ptr->sub_index    = mcfg_refresh_info_ptr->sub_index;
  msg_ptr->reference_id = mcfg_refresh_info_ptr->reference_id;

  /* Put on mmgsdi command queue */
  mmgsdi_status = mmgsdi_cmd(task_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS )
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    MMGSDIUTIL_TMC_MEM_FREE(task_cmd_ptr);
    return FALSE;
  }

  UIM_MSG_HIGH_4("MCFG CB: MMGSDI_HANDLE_NV_REFRESH_REQ status 0x%x, type 0x%x, slot 0x%x, sub 0x%x",
                mmgsdi_status,mcfg_refresh_info_ptr->type,
                mcfg_refresh_info_ptr->slot_index,mcfg_refresh_info_ptr->sub_index);
  return TRUE;
}/*mmgsdi_nv_refresh_mcfg_cb*/

