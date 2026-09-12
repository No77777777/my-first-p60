/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            M M G S D I   G S T K  F U N C T I O N S


GENERAL DESCRIPTION

  This file contains the functions required for interaction with GSTK.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2024 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_gstk.c#2 $$ $DateTime: 2024/04/24 21:49:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/23/24   pg     Dont switch protocol to ICC if USIM app is present in EF-DIR
03/30/17   tq     Corrected logging 
12/01/16   vdc    Added support for new version of multi profile card
10/27/16   me     initial versoin

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmgsdiutil.h"
#include "mmgsdi.h"
#include "mmgsdi_uicc.h"
#include "mmgsdi_evt.h"
#include "mmgsdilib_p.h"
#include "mmgsdi_session.h"
#include "mmgsdi_gen.h"
#include "mmgsdi_nv.h"
#include "mmgsdi_euicc.h"
#include "mmgsdi_card_init.h"
#include "mmgsdi_gstk.h"
#include "mmgsdi_psm.h"
#include "mmgsdi_sap.h"
#include "mmgsdi_multi_profile.h"

#include "uim_msg.h"

#include "gstk_p.h"

#include "nvruim_p.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
#define MMGSDI_SST_PROACTIVE_SIM_OFFSET  ((MMGSDI_GSM_SRV_PROACTIVE - MMGSDI_GSM_SRV) / 4)
#define MMGSDI_SST_PROACTIVE_MASK        (0x03 << ((MMGSDI_GSM_SRV_PROACTIVE - MMGSDI_GSM_SRV) % 4))

#define MMGSDI_CDMA_SST_TOOLKIT_OFFSET   ((MMGSDI_CDMA_SRV_PROACTIVE - MMGSDI_CDMA_SRV) / 4)
#define MMGSDI_CDMA_SST_TOOLKIT_MASK     (0x03 << ((MMGSDI_CDMA_SRV_PROACTIVE - MMGSDI_CDMA_SRV) % 4))

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/


/*===========================================================================
FUNCTION MMGSDI_PROCESS_TP_DOWNLOAD_COMPLETE

DESCRIPTION
  This function performs the actions required after TP download completes

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_process_tp_download_complete(
  mmgsdi_tp_dl_complete_req_type * req_ptr
)
{
  mmgsdi_return_enum_type             mmgsdi_status   = MMGSDI_ERROR;
  uint32                              index           = 0;

  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  /*----------------------------------------------------------------------------------
    Get a new index from client req info table for response data.
    Set the uim data pointer to the client request table data buffer.
   -----------------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                    index,
                    &req_ptr->request_header,
                    NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_queue_mmgsdi_uim_report_rsp(
                    index,
                    MMGSDI_TP_DOWNLOAD_COMPLETE_REQ,
                    mmgsdi_status);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;
} /* mmgsdi_process_tp_download_complete */


/*===========================================================================
FUNCTION MMGSDI_PROCESS_TP_DL_COMPLETE_RESPONSE

DESCRIPTION
  This function performs the actions required after TP download completes

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_process_tp_dl_complete_response(
  const mmgsdi_uim_report_rsp_type  * rsp_ptr
)
{
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_slot_data_type              * slot_data_ptr   = NULL;
  gstk_slot_id_enum_type               gstk_slot       = GSTK_SLOT_ID_MAX;
  mmgsdi_slot_id_enum_type             sess_slot       = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                               index           = 0x00;
  uint8                                app_index       = 0x00;
  mmgsdi_request_header_type           req_header;
  mmgsdi_client_req_extra_info_type  * extra_param_ptr = NULL;
  mmgsdi_session_id_type               session_id_1x   = 0x00;
  mmgsdi_session_id_type               session_id_gw   = 0x00;
  mmgsdi_int_app_info_type           * app_info_ptr_gw = NULL;
  mmgsdi_int_app_info_type           * app_info_ptr_1x = NULL;
  uim_slot_type                        uim_slot        = UIM_SLOT_NONE;

  MMGSDIUTIL_RETURN_IF_NULL_2(mmgsdi_generic_data_ptr, rsp_ptr);

  if(rsp_ptr->mmgsdi_error_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_1("Unsuccessful TP 0x%x", rsp_ptr->mmgsdi_error_status);
    return MMGSDI_ERROR;
  }

  index = rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(index,
                                                            &req_header,
                                                            &extra_param_ptr);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  UIM_MSG_HIGH_1("TP Download Complete Response for slot 0x%x",
                 req_header.slot_id);

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_header.slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  MMGSDIUTIL_RETURN_IF_OUT_OF_RANGE(req_header.slot_id, MMGSDI_SLOT_1, MMGSDI_SLOT_3);

  (void)mmgsdi_util_convert_uim_slot(req_header.slot_id, &uim_slot);

  UIM_MSG_HIGH_1("TP RSP: mmgsdi_state = 0x%x", slot_data_ptr->mmgsdi_tp_state);

  if(((slot_data_ptr->mmgsdi_tp_state != MMGSDI_TP_STATE_NOT_DONE) ||
       (slot_data_ptr->mmgsdi_tp_state != MMGSDI_TP_STATE_DONE))   &&
      (slot_data_ptr->tp_ref_count    == req_header.client_data))
  {
    if(slot_data_ptr->protocol == MMGSDI_UICC)
    {
      /* If TP was not sent to card, indicate TP state as not needed */
      if(slot_data_ptr->mmgsdi_tp_state == MMGSDI_TP_STATE_IN_PROGRESS_NOT_OK)
      {
        slot_data_ptr->mmgsdi_tp_state = MMGSDI_TP_STATE_NOT_NEEDED;
      }
      else
      {     
        slot_data_ptr->mmgsdi_tp_state = MMGSDI_TP_STATE_DONE;
      }

      /* For UICC we are still in NOT_INIT and will move to CARD_INSRETED when
      we recieve CARD_INSTERED  */

      /* Invoke PSM to perform special processing if early init was done on slot1*/
      if (req_header.slot_id == MMGSDI_SLOT_1 &&
          mmgsdi_psm_get_status_early_init())
      {
        UIM_MSG_HIGH_0("TP DL Done, trigger in PSM init for real card");
        (void)mmgsdi_psm_handle_card_init_post_tp_dl();

        /* As part of PSM handling, USIM and/or CSIM init is done or
           if card mismatch, reset is triggered which further triggers a new link est
           Nothing to process beyond this */
        return MMGSDI_SUCCESS;
      }

      if(slot_data_ptr->mmgsdi_state == MMGSDI_STATE_NOT_INIT)
      {
        boolean    fail_reason_card_error = FALSE;
        boolean    ef_dir_not_found       = FALSE;

        /* Check if there are any valid UICC applications on the card */
        mmgsdi_status = mmgsdi_uicc_get_available_aids(req_header.slot_id,
                                                       &app_index,
                                                       &fail_reason_card_error,
                                                       &ef_dir_not_found);

        /* If there are valid UICC apps found... */
        if ((mmgsdi_status == MMGSDI_SUCCESS) && (app_index > 0))
        {
#ifdef FEATURE_MMGSDI_3GPP
          if (mmgsdi_util_get_usim_apps(req_header.slot_id))
          {
            slot_data_ptr->card_apps_available |= MMGSDI_USIM_APP_MASK;
          }
#endif /* FEATURE_MMGSDI_3GPP */
#ifdef FEATURE_MMGSDI_3GPP2
          if (mmgsdi_util_get_csim_apps(req_header.slot_id))
          {
            slot_data_ptr->card_apps_available |= MMGSDI_CSIM_APP_MASK;
            nvruim_set_uim_dir_present(NVRUIM_CDMA_SUBSCRIPTION_PRESENT, req_header.slot_id);
          }
#endif /* FEATURE_MMGSDI_3GPP2 */
          UIM_MSG_HIGH_3("No. of Apps present in UICC is 0x%x, USIM present: %x, CSIM present: %x",
                         app_index,
                         (slot_data_ptr->card_apps_available & MMGSDI_USIM_APP_MASK) ? TRUE : FALSE,
                         (slot_data_ptr->card_apps_available & MMGSDI_CSIM_APP_MASK) ? TRUE : FALSE);
        }

        /* Some eUICCs do not have a correct information in the ATR
           regarding eUICC support. In such cases, slot_data_ptr->is_euicc
           could be set to FALSE. We need to find out if it is really an
           eUICC or not by sending a SELECT on ISDR. If SELECT on ISDR
           succeeds, it is an eUICC, otherwise not */
        if(!slot_data_ptr->is_euicc && ef_dir_not_found) 
        {
          boolean is_isdr_found     = FALSE;

          if(MMGSDI_SUCCESS == mmgsdi_euicc_is_isdr_found(req_header.slot_id, 
                                                          &is_isdr_found) &&
             is_isdr_found)
          {
            slot_data_ptr->is_euicc = TRUE;
          }
        }

        /* If valid USIM/CSIM apps are available on the card then check for
           profile info global. If the profile info global is NULL then it is
           expected that the card does not support multiple profiles. In this
           case, send the card inserted event. Otherwise, if the card
           supports to provide the active profile then we first determine the
           current active profile present in the card and set it to REGULAR
           if it is in EMERGENCY. If the card does not support to provide the
           current active profile then we blindly send the set sim profile
           request with requested profile type as REGULAR. Once the switching
           is done, we make the reg_profile_detected flag in profile info global
           to TRUE so that in the subsequent card resets we dont need
           to determine the profile.
           If the card is eUICC, we need to send card_inserted irrespective of
           any default profile present or not which means even if EF-DIR read
           fails (num_app == 0) we have to publish card_inserted */
        if (slot_data_ptr->card_apps_available != 0x00 || slot_data_ptr->is_euicc)
        {
          boolean send_card_inserted = TRUE;
#ifdef FEATURE_UIM_MULTI_PROFILE
          if(req_header.slot_id == MMGSDI_SLOT_1)
          {
            send_card_inserted = mmgsdi_multi_profile_set_regular_profile(req_header.slot_id);
          }
#endif /* FEATURE_UIM_MULTI_PROFILE */

          if(send_card_inserted)
          {
            uint8                         slot_index     = MMGSDI_SLOT_1_INDEX;

            /* When SAP was already connected and MMGSDI receives card power up request
               then it should send SAP disconnected event to the client before
               sending card inserted event */
            if(mmgsdi_util_get_slot_index(req_header.slot_id, &slot_index) == MMGSDI_SUCCESS &&
               mmgsdi_sap_connected[slot_index])
            {
              mmgsdi_evt_build_and_send_sap_disconnected(req_header.slot_id);
              mmgsdi_sap_reset_sap_data(req_header.slot_id);
            }

            UIM_MSG_HIGH_1("MMGSDI_CARD_INSERTED for SLOT 0x%x", req_header.slot_id);
            mmgsdi_evt_build_and_send_card_inserted(TRUE,
                                                    (mmgsdi_client_id_type)0,
                                                    req_header.slot_id);
          }
        }
        /* If fail_reason_card_error is TRUE, it means UIMDRV has not processed
           this cmd because of some internal error or card_error. In that case,
           there is no need to switch the protocol */
        else if (fail_reason_card_error == FALSE)
        {
          UIM_MSG_HIGH_1("No valid UICC app. found on slot 0x%x, switching protocol to ICC",
                         req_header.slot_id);

          mmgsdi_util_preinit_mmgsdi((mmgsdi_session_id_type)0,
                                     MMGSDI_APP_NONE,
                                     MMGSDI_REFRESH_RESET,
                                     req_header.slot_id,
                                     FALSE);

          /* Switch card protocol to ICC */
          mmgsdi_status = mmgsdi_gen_card_reset_sync(req_header.slot_id, UIM_ICC);
        }
      }
    }
    else if((slot_data_ptr->protocol == MMGSDI_ICC) &&
            (slot_data_ptr->mmgsdi_state == MMGSDI_STATE_CARD_INSERTED))
    {
      gstk_slot = mmgsdi_util_convert_gstk_slot(req_header.slot_id);
      if(gstk_slot != GSTK_SLOT_ID_MAX)
      {
        gstk_gsdi_set_fetch_status(gstk_slot, TRUE);
      }

      /* Find all the apps which needs activation from this slot
         Can be both GSM and RUIM or GSM alone or RUIM alone*/
      session_id_gw = mmgsdi_generic_data_ptr->pri_gw_session_id;
      mmgsdi_status = mmgsdi_util_get_session_app_info(
                        session_id_gw,
                        &sess_slot, NULL, &app_info_ptr_gw, NULL, NULL, NULL);
      if(((app_info_ptr_gw != NULL) && (mmgsdi_status == MMGSDI_SUCCESS)) ||
         ((app_info_ptr_gw == NULL) && (mmgsdi_status != MMGSDI_SUCCESS)))
      {
        /* sess_slot will be MMGSDI_MAX_SLOT_ID_ENUM when there is no associated App */
        if(sess_slot != req_header.slot_id)
        {
          session_id_gw = mmgsdi_generic_data_ptr->sec_gw_session_id;
          mmgsdi_status = mmgsdi_util_get_session_app_info(
                            session_id_gw,
                            &sess_slot, NULL, &app_info_ptr_gw, NULL, NULL, NULL);
          if(((app_info_ptr_gw != NULL) && (mmgsdi_status == MMGSDI_SUCCESS)) ||
             ((app_info_ptr_gw == NULL) && (mmgsdi_status != MMGSDI_SUCCESS)))
          {
            /* sess_slot will be MMGSDI_MAX_SLOT_ID_ENUM when there is no associated App */
            if(sess_slot != req_header.slot_id)
            {
              session_id_gw = mmgsdi_generic_data_ptr->ter_gw_session_id;
              mmgsdi_status = mmgsdi_util_get_session_app_info(
                                session_id_gw,
                                &sess_slot, NULL, &app_info_ptr_gw, NULL, NULL, NULL);

              if((app_info_ptr_gw == NULL) && (mmgsdi_status != MMGSDI_SUCCESS))
              {
                UIM_MSG_ERR_1("Could not retrieve GW app info for the slot 0x%x",
                              req_header.slot_id);
              }
            }
          }
        }
      }

      session_id_1x = mmgsdi_generic_data_ptr->pri_1x_session_id;
      mmgsdi_status = mmgsdi_util_get_session_app_info(
                        session_id_1x,
                        &sess_slot, NULL, &app_info_ptr_1x, NULL, NULL, NULL);
      if(((app_info_ptr_1x != NULL) && (mmgsdi_status == MMGSDI_SUCCESS)) ||
         ((app_info_ptr_1x == NULL) && (mmgsdi_status != MMGSDI_SUCCESS)))
      {
        /* sess_slot will be MMGSDI_MAX_SLOT_ID_ENUM when there is no associated App */
        if(sess_slot != req_header.slot_id)
        {
          session_id_1x = mmgsdi_generic_data_ptr->sec_1x_session_id;
          mmgsdi_status = mmgsdi_util_get_session_app_info(
                            session_id_1x,
                            &sess_slot, NULL, &app_info_ptr_1x, NULL, NULL, NULL);
          if(((app_info_ptr_1x != NULL) && (mmgsdi_status == MMGSDI_SUCCESS)) ||
             ((app_info_ptr_1x == NULL) && (mmgsdi_status != MMGSDI_SUCCESS)))
          {
            /* sess_slot will be MMGSDI_MAX_SLOT_ID_ENUM when there is no associated App */
            if(sess_slot != req_header.slot_id)
            {
              session_id_1x = mmgsdi_generic_data_ptr->ter_1x_session_id;
              mmgsdi_status = mmgsdi_util_get_session_app_info(
                                session_id_1x,
                                &sess_slot, NULL, &app_info_ptr_1x, NULL, NULL, NULL);

              if((app_info_ptr_1x == NULL) && (mmgsdi_status != MMGSDI_SUCCESS))
              {
                UIM_MSG_ERR_1("Could not retrieve 1X app info for the slot 0x%x",
                              req_header.slot_id);
              }
            }
          }
        }
      }

      if(app_info_ptr_gw != NULL)
      {
        UIM_MSG_HIGH_1("GSM App TP state 0x%x", app_info_ptr_gw->app_tp_state);
        if(((app_info_ptr_gw->app_tp_state == MMGSDI_TP_STATE_IN_PROGRESS_OK)      ||
            (app_info_ptr_gw->app_tp_state == MMGSDI_TP_STATE_IN_PROGRESS_NOT_OK)) &&
           (app_info_ptr_gw->app_state     == MMGSDI_APP_STATE_READY_FOR_PERSO))
        {
          (void)mmgsdi_set_ens_support_flag(session_id_gw,
                                            req_header.slot_id,
                                            MMGSDI_APP_SIM);

          (void)mmgsdi_card_init_build_internal_pup_req(
                  session_id_gw,
                  req_header.slot_id,
                  MMGSDI_PIN1_EVT,
                  MMGSDI_STATE_CARD_INSERTED,
                  MMGSDI_APP_STATE_READY_FOR_PERSO);
        }
      }

      if(app_info_ptr_1x != NULL)
      {
        UIM_MSG_HIGH_1("CDMA App TP state 0x%x", app_info_ptr_1x->app_tp_state);
        if(((app_info_ptr_1x->app_tp_state == MMGSDI_TP_STATE_IN_PROGRESS_OK)      ||
            (app_info_ptr_1x->app_tp_state == MMGSDI_TP_STATE_IN_PROGRESS_NOT_OK)) &&
           (app_info_ptr_1x->app_state    == MMGSDI_APP_STATE_READY_FOR_PERSO))
        {
          (void)mmgsdi_card_init_build_internal_pup_req(
                  session_id_1x,
                  req_header.slot_id,
                  MMGSDI_PIN1_EVT,
                  MMGSDI_STATE_CARD_INSERTED,
                  MMGSDI_APP_STATE_READY_FOR_PERSO);
        }
      }

      mmgsdi_status = mmgsdi_util_set_tp_states(slot_data_ptr, app_info_ptr_gw, app_info_ptr_1x);
      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        UIM_MSG_ERR_1("mmgsdi_util_set_tp_states returned 0x%x", mmgsdi_status);
      }
    }
    else
    {
      UIM_MSG_ERR_1("Unknown Slot Protocol: 0x%x", slot_data_ptr->protocol);
    }
  }
  else
  {
    UIM_MSG_ERR_3("unexpected TP response: expected 0x%x, received 0x%x, state 0x%x",
                  slot_data_ptr->tp_ref_count,
                  req_header.client_data,
                  slot_data_ptr->mmgsdi_tp_state);
  }
  return MMGSDI_SUCCESS;
} /* mmgsdi_process_tp_dl_complete_response */


/* ============================================================================
FUNCTION MMGSDI_NV_GET_TOOLKIT_ENABLED_STATUS

DESCRIPTION
  This function returns the status Toolkit functionality
  enablement (Terminal Profile, FETCH, Envelopes)
  
PARAMETERS
  slot_id

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  mmgsdi_feature_status_enum_type: MMGSDI_FEATURE_ENABLED : Toolkit is enabled
                                   MMGSDI_FEATURE_DISABLED: Toolkit is disabled

SIDE EFFECTS
  None
============================================================================*/
static mmgsdi_feature_status_enum_type mmgsdi_nv_get_toolkit_enabled_status(
  mmgsdi_slot_id_enum_type    slot_id
)
{
  /*---------------------------------------------------------------------------
    To store status of Toolkit Enablement for each slot (NV XXXXX).
       is_cached:  TRUE if the NV read and value is cached
       is_enabled: TRUE if Toolkit is enabled
  -------------------------------------------------------------------------------*/
  static struct {
    boolean                         is_cached;
    mmgsdi_feature_status_enum_type is_enabled;
  } mmgsdi_toolkit_enabled_status[MMGSDI_MAX_NUM_SLOTS];
  
  uint8                       slot_index       = 0;
  uim_common_efs_context_type efs_context      = UIM_COMMON_EFS_CONTEXT_MAX;
  uint8                       toolkit_disabled = 0;

  if (mmgsdi_util_get_slot_index(slot_id, &slot_index) != MMGSDI_SUCCESS)
  {
    return MMGSDI_FEATURE_DISABLED;
  }

  /* If Toolkit enablement status is cached... */
  if(!mmgsdi_toolkit_enabled_status[slot_index].is_cached)
  {  
    efs_context = mmgsdi_util_get_common_efs_item_context(slot_index);
    if (efs_context == UIM_COMMON_EFS_CONTEXT_MAX)
    {
      return MMGSDI_FEATURE_DISABLED;
    }

    /* Set the default value of Toolkit Enablement as TRUE */
    mmgsdi_toolkit_enabled_status[slot_index].is_enabled = MMGSDI_FEATURE_ENABLED;
    mmgsdi_toolkit_enabled_status[slot_index].is_cached = TRUE;

    /* Read the Toolkit Disabled file from EFS */
    if (uim_common_efs_read(UIM_COMMON_EFS_GSTK_SLOT_TOOLKIT_DISABLED,
                            UIM_COMMON_EFS_ITEM_FILE_TYPE,
                            efs_context,
                            (uint8 *)&toolkit_disabled,
                            sizeof(toolkit_disabled)) ==
        UIM_COMMON_EFS_SUCCESS)
    {
      if(toolkit_disabled)
      {
        mmgsdi_toolkit_enabled_status[slot_index].is_enabled = MMGSDI_FEATURE_DISABLED;
      }
    }
    
    UIM_MSG_LOW_2("Toolkit enabled status for slot_index 0x%x is 0x%x",
                  slot_index, mmgsdi_toolkit_enabled_status[slot_index].is_enabled);
  }
  return mmgsdi_toolkit_enabled_status[slot_index].is_enabled;
} /* mmgsdi_nv_get_toolkit_enabled_status */


/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_IS_TOOLKIT_ALLOWED

DESCRIPTION
  This function is used to determine whether or not toolkit is allowed.

DEPENDENCIES
  None

RETURNS
    TRUE     Toolkit is allowed
    FALSE    Toolkit is not allowed

SIDE EFFECTS
  None
===========================================================================*/
boolean mmgsdi_card_init_is_toolkit_allowed(
  mmgsdi_session_id_type   session_id,
  mmgsdi_slot_id_enum_type slot,
  mmgsdi_app_enum_type     app_type
)
{
#if defined(FEATURE_MMGSDI_GSM_CARD) || defined(FEATURE_MMGSDI_3GPP2)
  byte                     svc_table_ind    = 0x00;
  mmgsdi_file_enum_type    service_tbl_file = MMGSDI_GSM_SST;
  uint8                    pro_cmd_offset   = 0;
  uint8                    pro_cmd_mask     = 0;
  mmgsdi_return_enum_type  mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_data_type         mmgsdi_data_buf;

  memset(&mmgsdi_data_buf, 0x00, sizeof(mmgsdi_data_type));
#endif /* defined(FEATURE_MMGSDI_GSM_CARD) || defined(FEATURE_MMGSDI_3GPP2) */

  /* Check if Toolkit functionality (Terminal Profile, FETCH,
     Envelopes) is disabled for the slot */
  if(mmgsdi_nv_get_toolkit_enabled_status(slot) == MMGSDI_FEATURE_DISABLED)
  {
    return FALSE;
  }

  switch (app_type)
  {
    case MMGSDI_APP_USIM:
    case MMGSDI_APP_CSIM:
      return TRUE;

    case MMGSDI_APP_SIM:
#ifdef FEATURE_MMGSDI_GSM_CARD
    {
      mmgsdi_slot_data_type *slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot);

      if(slot_data_ptr == NULL)
      {
        return FALSE;
      }

      if(slot_data_ptr->sim_phase == MMGSDI_SIM_PHASE_2_PLUS)
      {
        return TRUE;
      }
      service_tbl_file = MMGSDI_GSM_SST;
      pro_cmd_offset = MMGSDI_SST_PROACTIVE_SIM_OFFSET;
      pro_cmd_mask = MMGSDI_SST_PROACTIVE_MASK;
      break;
    }
#else
      return FALSE;
#endif /* FEATURE_MMGSDI_GSM_CARD */

    case MMGSDI_APP_RUIM:
#ifdef FEATURE_MMGSDI_3GPP2
      service_tbl_file = MMGSDI_CDMA_SVC_TABLE;
      pro_cmd_offset = MMGSDI_CDMA_SST_TOOLKIT_OFFSET;
      pro_cmd_mask = MMGSDI_CDMA_SST_TOOLKIT_MASK;
      break;
#else
      return FALSE;
#endif /* FEATURE_MMGSDI_3GPP2 */

    default:
      return FALSE;
  }

#if defined(FEATURE_MMGSDI_GSM_CARD) || defined(FEATURE_MMGSDI_3GPP2)
  /* Read the service table */
  mmgsdi_status = mmgsdi_card_init_cache_binary(session_id,
                                                app_type,
                                                slot,
                                                service_tbl_file,
                                                0,
                                                &mmgsdi_data_buf);
  if (mmgsdi_status != MMGSDI_SUCCESS || mmgsdi_data_buf.data_ptr == NULL)
  {
    UIM_MSG_ERR_0("mmgsdi_card_init_is_toolkit_allowed, data is NULL");
    return FALSE;
  }

  if (mmgsdi_data_buf.data_len <= pro_cmd_offset)
  {
    UIM_MSG_ERR_0("Service Table Read does not contain byte for Proactive Cmd info");

    MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_data_buf.data_ptr);
    return FALSE;
  }

  svc_table_ind = mmgsdi_data_buf.data_ptr[pro_cmd_offset] & pro_cmd_mask;

  /* Free the memory allocated for the Service Table Read */
  MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_data_buf.data_ptr);

  /* Determine if Toolkit is allowed */
  return (svc_table_ind ? TRUE : FALSE);
#else
  (void)session_id;
  (void)slot;
#endif /* defined(FEATURE_MMGSDI_GSM_CARD) || defined(FEATURE_MMGSDI_3GPP2) */
} /* mmgsdi_card_init_is_toolkit_allowed */


/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_GSTK_TERMINAL_PROFILE_DOWNLOAD

DESCRIPTION
  This function is used to notfiy GSTK to perform a Terminal Profile
  download

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  Triggers the GSTK task to send down a Terminal Profile Download
===========================================================================*/
void mmgsdi_card_init_gstk_terminal_profile_download (
  mmgsdi_session_id_type   session_id,
  mmgsdi_slot_id_enum_type slot,
  mmgsdi_app_enum_type     app_type
)
{
  gstk_slot_id_enum_type      gstk_slot       = GSTK_SLOT_ID_MAX;
  mmgsdi_slot_data_type     * slot_data_ptr   = NULL;
  boolean                     ok_to_tp_dl     = FALSE;
  gstk_status_enum_type       gstk_status     = GSTK_ERROR;
  mmgsdi_int_app_info_type  * app_info_ptr    = NULL;
  mmgsdi_return_enum_type     mmgsdi_status   = MMGSDI_ERROR;
  boolean                     fetch_status    = FALSE;

  UIM_MSG_HIGH_2("In mmgsdi_card_init_gstk_terminal_profile_download, slot: 0x%x, app_type: 0x%x",
                 slot, app_type);

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot);

  if(slot_data_ptr == NULL)
  {
    return;
  }

  gstk_slot = mmgsdi_util_convert_gstk_slot(slot);
  if(GSTK_SLOT_ID_MAX == gstk_slot)
  {
    return;
  }

  if((app_type == MMGSDI_APP_SIM) || (app_type == MMGSDI_APP_RUIM))
  {
    /* For UICC we need not be concerned about  App info */
    mmgsdi_status = mmgsdi_util_get_session_app_info(session_id, NULL,
                               NULL, &app_info_ptr, NULL, NULL, NULL);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
       UIM_MSG_ERR_1("Unable to get app info while profile download for 0x%x",
                   app_type);
       return;
    }
  }

  /* Current limitation/implementation is 1 TP per slot, so
     if we have a session that has already performed the TP, we
     do not want to instruct GSTK to send another TP for the
     same slot */
  if ((slot_data_ptr->mmgsdi_tp_state == MMGSDI_TP_STATE_DONE) ||
      (slot_data_ptr->mmgsdi_tp_state == MMGSDI_TP_STATE_IN_PROGRESS_OK))
  {
    UIM_MSG_HIGH_2("Cannot perform more that one TP per slot, TP state: 0x%x on Slot: 0x%x",
                   slot_data_ptr->mmgsdi_tp_state, slot);
    return;
  }

  /* --------------------------------------------------------------------------
  Determine if the card supports Terminal profile download or not.
  ----------------------------------------------------------------------------*/
  ok_to_tp_dl = mmgsdi_card_init_is_toolkit_allowed(session_id, slot, app_type);
  if(ok_to_tp_dl)
  {
    slot_data_ptr->mmgsdi_tp_state = MMGSDI_TP_STATE_IN_PROGRESS_OK;
    if(app_info_ptr != NULL)
    {
      app_info_ptr->app_tp_state = MMGSDI_TP_STATE_IN_PROGRESS_OK;
    }
  }
  else
  {
    slot_data_ptr->mmgsdi_tp_state = MMGSDI_TP_STATE_IN_PROGRESS_NOT_OK;
    if(app_info_ptr != NULL)
    {
      app_info_ptr->app_tp_state = MMGSDI_TP_STATE_IN_PROGRESS_NOT_OK;
    }
  }

#ifdef FEATURE_UIM_MULTI_PROFILE
  /* Allow GSTK to fetch the proactive command before sending the TP. We do this
     to fetch the proactive commands (if any) after sending the TP. In case of
     multi profile card, if the profile is in emergency then we have logic to
     send error TR response to the fetched proactive commands*/
  if(mmgsdi_profile_info_ptr != NULL)
  {
    fetch_status = TRUE;
  }
#endif /* FEATURE_UIM_MULTI_PROFILE */

  gstk_gsdi_set_fetch_status(gstk_slot, fetch_status);

  /* Update TP state and Reference count */
  slot_data_ptr->tp_ref_count++;

  /* Do not send TP request to GSTK if Virtual SIM has been enabled,
     but Virtual SAT has not been enabled */
  UIM_MSG_HIGH_2("Sending TP notifcation for App 0x%x, TP state is 0x%x",
                 app_type, slot_data_ptr->mmgsdi_tp_state);
  gstk_status = gstk_slot_send_terminal_profile_dl_command(
                  gstk_slot,
                  ok_to_tp_dl,
                  slot_data_ptr->tp_ref_count,
                  mmgsdi_gstk_terminal_profile_cb,
                  mmgsdi_psm_get_status_early_init());

  if(gstk_status != GSTK_SUCCESS)
  {
    gstk_cmd_from_card_type response;

    memset((void*)&response, 0x00, sizeof(gstk_cmd_from_card_type));

    /* Fake a response */
    UIM_MSG_ERR_1("Unable to send TP indication to GSTK for slot 0x%x", slot);
    response.hdr_cmd.command_id = GSTK_PROFILE_DL_IND_RSP;
    /* command_number, cmd_ref_id are ignored */
    response.hdr_cmd.cmd_detail_reference = 0;
    response.hdr_cmd.command_number = 0;
    response.hdr_cmd.sim_slot_id = gstk_slot;
    response.hdr_cmd.user_data = slot_data_ptr->tp_ref_count;
    response.cmd.terminal_profile_rsp = GSTK_PROFILE_RSP_FAIL;
    mmgsdi_gstk_terminal_profile_cb(&response);
  }
}  /* mmgsdi_card_init_gstk_terminal_profile_download */


/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_PERFORM_POST_PIN1_TP_IF_NEEDED

DESCRIPTION
  Send Terminal Profile Download if required as part of post-pin1 if required.

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_card_init_perform_post_pin1_tp_if_needed(
  mmgsdi_session_id_type     session_id,
  mmgsdi_slot_id_enum_type   slot_id,
  mmgsdi_app_enum_type       app_type
)
{
  mmgsdi_slot_data_type *slot_data_ptr = NULL;

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);

  if(slot_data_ptr == NULL)
  {
    return;
  }

  /* If TP is in progress/already done, return since only
     one TP is required to be done per slot */
  if((slot_data_ptr->mmgsdi_tp_state == MMGSDI_TP_STATE_DONE)          ||
     (slot_data_ptr->mmgsdi_tp_state == MMGSDI_TP_STATE_IN_PROGRESS_OK))
  {
    UIM_MSG_HIGH_2("TP state for MMGSDI slot 0x%x is 0x%x",
                   slot_id, slot_data_ptr->mmgsdi_tp_state);
    return;
  }
  if ((app_type == MMGSDI_APP_USIM) || (app_type == MMGSDI_APP_CSIM))
  {
    UIM_MSG_HIGH_2("Slot 0x%x App type: 0x%x, Sending Terminal Profile to UICC",
                   slot_id, app_type);
    mmgsdi_card_init_gstk_terminal_profile_download(session_id, slot_id, app_type);
    return;
  }

  /* Attempt to read and cache the SIM Phase
     Ignore return value since nothing can be done anyway*/
  (void)mmgsdi_card_init_check_ef_phase(session_id,
                                         slot_id,
                                         app_type);

  mmgsdi_card_init_gstk_terminal_profile_download(session_id,
                                                  slot_id,
                                                  app_type);
}/* mmgsdi_card_init_perform_post_pin1_tp_if_needed */


/* ============================================================================
FUNCTION MMGSDI_CARD_INIT_TP_STATE_INIT

DESCRIPTION
  This function set the TP state 
  
PARAMETERS
  *slot_data_ptr

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None
                                   
SIDE EFFECTS
  None
============================================================================*/
void mmgsdi_card_init_tp_state_init(
  mmgsdi_slot_data_type   * slot_data_ptr  
)
{
  if (slot_data_ptr != NULL)
  {
    slot_data_ptr->mmgsdi_tp_state = MMGSDI_TP_STATE_NOT_DONE;
  }
}/* mmgsdi_card_init_tp_state_init */


/* ============================================================================
FUNCTION MMGSDI_CARD_INIT_APP_TP_STATE_INIT

DESCRIPTION
  This function set the app TP state 
  
PARAMETERS
  *app_info_ptr

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None
                                   
SIDE EFFECTS
  None
============================================================================*/
void mmgsdi_card_init_app_tp_state_init(
  mmgsdi_int_app_info_type   * app_info_ptr  
)
{
  if (app_info_ptr != NULL)
  {
    app_info_ptr->app_tp_state = MMGSDI_TP_STATE_NOT_DONE;
  }
}/* mmgsdi_card_init_app_tp_state_init */
