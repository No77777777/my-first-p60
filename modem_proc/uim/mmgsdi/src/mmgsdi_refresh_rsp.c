/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                    M M G S D I   R E F R E S H   R S P   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the refresh response processing.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2007-2016, 2019 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_refresh_rsp.c#3 $$ $DateTime: 2020/10/18 15:27:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/11/20   sg       Consider call ID and previous call type during call switch
06/03/19   kb       Support for card reset on refresh failure 
05/16/16   ar       F3 reduction
04/22/16   tkl      Reduce F3 log prints
04/19/16   ar       Correcting the index to MMGSDI_MAX_CLIENT_REQ_INFO
10/16/15   sp       Move client_req_table_info index variables to unsigned
10/05/15   sp       Reduce signed/unsigned conversions in UIM
09/14/15   bcho     Update Refresh TR for additional files read only if success
07/29/15   sp       Remove UIM command in client request table
06/11/15   lm       Remove redundant code in slot index validation
05/07/15   lxu      Fix crash of refresh on EF GBABP
04/28/15   vdc      Added support for GBA APN
03/30/15   lxu      Support refresh on EF GBABP
09/11/14   hh       Call rex_undef_timer() when timer is no longer needed
05/08/14   hh       Correctly handle act_or_switch that results app deactivation
01/28/14   df       Use q_cnt() instead directly accessing cnt
01/11/14   df       Fix off-target compilation error
01/10/14   df       Use v2.0 diag macros
12/11/13   df       Remove unused memory paging feature
11/27/13   tl       Introduce refresh command queue
11/18/13   vv       Added support for parallel REFRESH
10/04/13   bcho     F3 message reduction
05/14/13   vdc      Memory optimizations for slot, app, pin, upin tables
10/11/12   spo      Removed unnecessary F3 messages
09/28/12   abg      Updated ERR to MSG_ERROR
10/13/11   nb       Added support for new refresh stage
05/17/11   shr      Updates for Thread safety
01/21/11   nmb      Added refresh retry support
12/22/10   yt       Removed FEATURE_UIM_ZI_MEMORY_REDUCTION
05/30/10   adp      Refresh confirmation not being sent if refresh for a client
                    or all clients is complete
04/06/10   shr      Clean up REFRESH RESET and REFRESH RESET_AUTO
12/04/09   kp       Replacing mmgsdi_data_slot2 with mmgsdi_data_slot2_ptr
10/28/09   nb       Release 7 Enhanced Network Selection
09/26/09   kp       ZI memory reduction changes
04/29/07   js       Included header mmgsdi_refresh_rsp.h
03/30/09   xz       1) Fix issue of not sending specific error code to client
                       when REFRESH fails
                    2) Change mmgsdi_refresh_rsp_send_cnf_to_gstk() return type
03/19/09   js       Support for Enhanced NAA Refresh by file path
01/28/09   nb       Fixed Klockwork Error
12/02/08   ssr      Removed Compilation Error
12/01/08   ssr      Handled error condition for some MMGSDI Functions
09/22/08   js       Fixed Lint Errors
06/04/08   tml      Added sw to cnf header
03/17/08   sun      Allow cleanup to be done during a refresh
11/12/07   sun      Initial version

===========================================================================*/


#include "uim_variation.h"
#include "comdef.h"
#include "intconv.h"
#include "mmgsdilib.h"
#include "mmgsdi.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "mmgsdi_refresh.h"
#include "mmgsdi_cnf.h"
#include "mmgsdi_refresh_rsp.h"
#include "gba_lib.h"
#include "mmgsdi_nv.h"

/* ============================================================================
  FUNCTION:      MMGSDI_REFRESH_RSP_CLEAN_UP

  DESCRIPTION:
    This function will clean up the refresh linked list and the global
    refresh pointer

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    None

  SIDE EFFECTS:
    None
=============================================================================*/
void mmgsdi_refresh_rsp_clean_up(uint8 slot_index)
{
  if(!MMGSDIUTIL_SLOT_INDEX_VALID(slot_index))
  {
    return;
  }

  if(mmgsdi_refresh_info_ptr[slot_index] == NULL)
  {
    UIM_MSG_HIGH_0("Nothing to cleanup, refresh ptr is NULL");
    return;
  }

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;
  if(mmgsdi_data_slot_ptr[slot_index])
  {
    mmgsdi_data_slot_ptr[slot_index]->refresh_state = MMGSDI_REFRESH_READY_STATE;
  }

  (void)rex_clr_timer(&mmgsdi_refresh_timer[slot_index]);

  /* Clear the timer if the mode is not AUTO.  If it is Auto
     only keep the timer if the command will be retried later */
  if(mmgsdi_refresh_info_ptr[slot_index]->refresh_status != MMGSDI_REFRESH_LATER ||
      (mmgsdi_refresh_info_ptr[slot_index]->refresh_status == MMGSDI_REFRESH_LATER &&
       mmgsdi_refresh_info_ptr[slot_index]->mode != MMGSDI_REFRESH_RESET_AUTO))
  {
    (void)rex_undef_timer(&mmgsdi_refresh_auto_trigger_timer[slot_index]);
  }

  /* Free mmgsdi_refresh_info_ptr[slot_index] and memory allocated for its
     pointer type members */
  mmgsdi_refresh_free_refresh_info_type(&mmgsdi_refresh_info_ptr[slot_index]);

  if(q_cnt(&mmgsdi_refresh_cmd_q) != 0)
  {
    /* Set the refresh command queue signal for main task */
    (void) rex_set_sigs(UIM_GSDI_TCB, MMGSDI_REFRESH_CMD_Q_SIG);
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

}/*mmgsdi_refresh_rsp_clean_up*/



/* ============================================================================
  FUNCTION:      MMGSDI_REFRESH_IS_REFRESH_EF_GBABP

  DESCRIPTION:
    This function will determine if refresh on EF GBABP happened

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    TRUE:  Refresh on EF GBABP happened
    FALSE: NO refresh on EF GBABP

  SIDE EFFECTS:
    None
=============================================================================*/
static boolean mmgsdi_refresh_is_refresh_ef_gbabp (
  uint8 slot_index
)
{
  uint32 i = 0;
  const mmgsdi_refresh_reg_data_type * refresh_data_ptr = NULL;

  if (slot_index >= MMGSDI_MAX_NUM_SLOTS ||
      mmgsdi_refresh_info_ptr[slot_index] == NULL ||
      !(MMGSDI_APP_USIM == mmgsdi_refresh_info_ptr[slot_index]->aid.app_type ||
         mmgsdi_util_is_isim_aid(mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_ptr,
                                 mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_len)))
  {
    return FALSE;
  }

  if (MMGSDI_REFRESH_NAA_INIT_FCN != mmgsdi_refresh_info_ptr[slot_index]->orig_mode &&
      MMGSDI_REFRESH_NAA_FCN != mmgsdi_refresh_info_ptr[slot_index]->orig_mode &&
      MMGSDI_REFRESH_3G_SESSION_RESET != mmgsdi_refresh_info_ptr[slot_index]->orig_mode)
  {
    /* GBA refresh is applicable only for refresh modes that have a list of files */
    return FALSE;
  }

  refresh_data_ptr = &mmgsdi_refresh_info_ptr[slot_index]->refresh_data;
  if (refresh_data_ptr->refresh_files.num_files > 0 &&
      refresh_data_ptr->refresh_files.file_list_ptr)
  {
    if (MMGSDI_APP_USIM == mmgsdi_refresh_info_ptr[slot_index]->aid.app_type)
    {
      for ( i = 0; i < refresh_data_ptr->refresh_files.num_files; i++)
      {
        if (MMGSDI_USIM_GBABP == refresh_data_ptr->refresh_files.file_list_ptr[i])
        {
          return TRUE;
        }
      }
    }
    else if (mmgsdi_util_is_isim_aid(mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_ptr,
                                     mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_len))
    {
      for ( i = 0; i < refresh_data_ptr->refresh_files.num_files; i++)
      {
        if (MMGSDI_ISIM_GBABP == refresh_data_ptr->refresh_files.file_list_ptr[i])
        {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}/* mmgsdi_refresh_is_refresh_ef_gbabp */


/* ============================================================================
  FUNCTION:      MMGSDI_REFRESH_GET_GBA_SESSION_TYPE

  DESCRIPTION:
    This function will get gba session type using session id

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    gba_session_type

  SIDE EFFECTS:
    None
=============================================================================*/
static gba_session_type mmgsdi_refresh_get_gba_session_type(
  uint8 slot_index
)
{
  gba_session_type                   gba_session             = GBA_SESSION_MAX;
  uint8                              client_index            = 0;
  uint8                              session_index           = 0;
  mmgsdi_len_type                    num_sessions            = 0;
  mmgsdi_session_info_type         * session_ptr             = NULL;

  if(!MMGSDIUTIL_SLOT_INDEX_VALID(slot_index))
  {
    return GBA_SESSION_MAX;
  }

  /* Find the session for which refresh was issued, based on the slot and AID of the REFRESH */
  for(client_index = 0; client_index < MMGSDI_MAX_CLIENT_INFO; client_index++)
  {
    if (mmgsdi_client_id_reg_table[client_index] == NULL)
    {
      continue;
    }

    num_sessions = mmgsdi_client_id_reg_table[client_index]->num_session;

    for (session_index = 0; (num_sessions > 0) &&
         (session_index < MMGSDI_MAX_SESSION_INFO); session_index++)
    {
      session_ptr =
        mmgsdi_client_id_reg_table[client_index]->session_info_ptr[session_index];

      if (session_ptr == NULL)
      {
        /*Invalid Session, Continue to next*/
        continue;
      }
      num_sessions--;

      session_ptr = mmgsdi_util_cmp_aid_get_session_ptr(
                      client_index,
                      session_index,
                      &mmgsdi_refresh_info_ptr[slot_index]->aid,
                      mmgsdi_refresh_info_ptr[slot_index]->request_header.slot_id,
                      TRUE);

      if(session_ptr == NULL)
      {
        /*Invalid Session, Continue to next*/
        continue;
      }

      switch (session_ptr->session_type)
      {
        case MMGSDI_GW_PROV_PRI_SESSION:
          gba_session = GBA_3GPP_PROV_SESSION_PRI;
          break;

        case MMGSDI_GW_PROV_SEC_SESSION:
          gba_session = GBA_3GPP_PROV_SESSION_SEC;
          break;

        case MMGSDI_NON_PROV_SESSION_SLOT_1:
          gba_session = GBA_NON_PROV_SESSION_SLOT_1;
          break;

        case MMGSDI_NON_PROV_SESSION_SLOT_2:
          gba_session = GBA_NON_PROV_SESSION_SLOT_2;
          break;

        default :
          break;
      }

      if (gba_session != GBA_SESSION_MAX)
      {
        return gba_session;
      }
    }
  }

  return GBA_SESSION_MAX;
} /* mmgsdi_refresh_get_gba_session_type */


/* ============================================================================
  FUNCTION:      MMGSDI_CARD_RESET_ON_CARD_REFRESH_FAILURE

  DESCRIPTION:
    This function shall keep a count of Card based REFRESH failures, 
    and trigger a Card Reset if the number of failures crosses the max. threshold 

  DEPENDENCIES:
    Functionality is executed only when MMGSDI_FEATURE_UICC_RESET_ON_REFRESH_FAILURE is enabled

  LIMITATIONS:
    None

  RETURN VALUE:
    void

  SIDE EFFECTS:
    None
=============================================================================*/
static void mmgsdi_card_reset_on_card_refresh_failure(
  mmgsdi_return_enum_type             refresh_status,
  mmgsdi_refresh_source_enum_type     source,
  mmgsdi_slot_id_enum_type            slot_id
)
{
  mmgsdi_slot_data_type              *slot_data_ptr          = NULL;
  mmgsdi_return_enum_type             mmgsdi_status          = MMGSDI_SUCCESS;
  boolean                             call_active            = FALSE;
  uint8                               mmgsdi_prov_index      = MMGSDI_MAX_PROV_APPS;
  mmgsdi_nv_context_type              nv_context             = MMGSDI_NV_CONTEXT_INVALID;
  mmgsdi_session_type_enum_type       session_type           = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_session_id_type              session_id             = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_technology_enum_type         tech                   = MMGSDI_TECH_UNKNOWN;

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);

  if (slot_data_ptr == NULL)
  {
    return;
  }

  /* If REFRESH status is one of the below success status, 
     then reset refresh failure count and return */
  if(refresh_status == MMGSDI_SUCCESS ||
     refresh_status == MMGSDI_REFRESH_IN_PROGRESS ||
     refresh_status == MMGSDI_REFRESH_SUCCESS ||
     refresh_status == MMGSDI_REFRESH_SUCCESS_NO_AID_ACTIVE ||
     refresh_status == MMGSDI_REFRESH_FAIL_INVALID_AID ||
     refresh_status == MMGSDI_REFRESH_ADDITIONAL_FILES_READ)
  {
    slot_data_ptr->refresh_failure_count = 0;
    return;
  }

  /* If source of the refresh not from SIM then return */
  if (source != MMGSDI_REFRESH_SOURCE_SIM)
  {
    return;
  }

  /* Increment the refresh failure count */
  slot_data_ptr->refresh_failure_count++;
  UIM_MSG_MED_1("Refresh failure counter: 0x%x", slot_data_ptr->refresh_failure_count);

  /* Check if card reset on refresh failure is supported */
  nv_context = mmgsdi_util_get_efs_item_index_for_slot(slot_id);
 
  if (MMGSDI_FEATURE_ENABLED != mmgsdi_nv_get_slot_feature_status( 
        MMGSDI_FEATURE_CARD_RESET_ON_REFRESH_FAILURE, nv_context))
  {
    return;
  }

  /* If card trigerred REFRESH failure count hits the max. allowed,
     then reset card if no voice/emergency calls are ongoing */
  if (slot_data_ptr->refresh_failure_count < MMGSDI_MAX_CARD_REFRESH_FAILURE_COUNT)
  {
    return;
  }

  for(tech = MMGSDI_TECH_3GPP; tech <= MMGSDI_TECH_3GPP2; tech++)
  {
    mmgsdi_app_enum_type  app_type   = MMGSDI_APP_NONE;

    /* Retrieve the GW and 1X prov index corresponding to the current slot */
    if ((mmgsdi_util_find_prov_session_for_slot(slot_id,
                                                &session_id,
                                                &app_type,
                                                tech) == MMGSDI_SUCCESS))
    {
      mmgsdi_status = mmgsdi_util_get_session_type(session_id, &session_type, NULL);
      if (mmgsdi_status == MMGSDI_SUCCESS)
      {
        mmgsdi_prov_index = mmgsdi_util_get_prov_index_from_session(session_type);
        if(mmgsdi_prov_index < MMGSDI_MAX_PROV_APPS)
        {
          /* Check the call status: if the call is active, we can immediately break */
          if (mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_prov_index].voice_call_active ||
              mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_prov_index].emergency_call_active)
          {
            call_active = TRUE;
            break;
          }
        }
      }
    }
  }/* end of for loop */

  UIM_MSG_HIGH_1("Ready to reset card after refresh failure: call status = 0x%x", call_active);

  if (!call_active)
  {
    /* Reset card if the call is not active */
    mmgsdi_status =  mmgsdi_card_pup(mmgsdi_generic_data_ptr->client_id,
                                     slot_id,
                                     mmgsdi_util_internal_cb,
                                     MMGSDI_CARD_POWER_UP_INITIAL_PUP,
                                     NULL);
  }
} /* mmgsdi_card_reset_on_card_refresh_failure */


/* ============================================================================
  FUNCTION:      MMGSDI_REFRESH_RSP_SEND_CNF

  DESCRIPTION:
    This function will send confirmation to GSTK

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type
    MMGSDI_SUCCESS:          Succeed in send confirmation to GSTK
    MMGSDI_ERROR:            Fail to send confirmation to GSTK
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_refresh_rsp_send_cnf(
  mmgsdi_request_header_type            *request_hdr_ptr)
{
  mmgsdi_return_enum_type             refresh_status = MMGSDI_CAN_NOT_REFRESH;
  mmgsdi_request_header_type         *req_hdr_ptr    = request_hdr_ptr;
  mmgsdi_return_enum_type             mmgsdi_status  = MMGSDI_ERROR;
  uint8                               slot_index     = MMGSDI_SLOT_1_INDEX;
  mmgsdi_client_req_extra_info_type   extra_param;
  mmgsdi_sw_status_type               status_word;

  if ( req_hdr_ptr == NULL  )
  {
    MMGSDIUTIL_DEBUG_MSG_ERROR_0("Attempt to use NULL POINTER");
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_get_slot_index(request_hdr_ptr->slot_id, &slot_index);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* status word not applicable in refresh to gstk */
  status_word.sw1 = 0x00;
  status_word.sw2 = 0x00;

  if(mmgsdi_refresh_info_ptr[slot_index] != NULL)
  {
    refresh_status = mmgsdi_refresh_info_ptr[slot_index]->refresh_status;
    if(mmgsdi_refresh_info_ptr[slot_index]->additional_files_read &&
       mmgsdi_refresh_info_ptr[slot_index]->refresh_status == MMGSDI_REFRESH_SUCCESS)
    {
      refresh_status = MMGSDI_REFRESH_ADDITIONAL_FILES_READ;
    }
    req_hdr_ptr    = &mmgsdi_refresh_info_ptr[slot_index]->request_header;

    /* Per TS31.111, chapter 6.4.7.2, If there is refresh on EF GBABP in USIM or ISIM,
       ME shall perform a initiate GBA Bootstrap procedure */
    if (mmgsdi_refresh_is_refresh_ef_gbabp(slot_index))
    {
      gba_aid_type                   app_id_data  = {0};
      gba_session_type               session_type = GBA_SESSION_MAX;

      /* AID is valid even if the actual REFRESH did not contain it, because we copy it
         in function mmgsdi_copy_to_refresh_ptr */
      if (mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_len > 0 &&
          mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_len <= sizeof(app_id_data.data))
      {
        app_id_data.data_len = mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_len;
        (void)mmgsdi_memscpy((void *)app_id_data.data,
                             sizeof(app_id_data.data),
                             (void *)mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_ptr,
                             mmgsdi_refresh_info_ptr[slot_index]->aid.aid.data_len);

        session_type = mmgsdi_refresh_get_gba_session_type(slot_index);
        if (session_type != GBA_SESSION_MAX)
        {
          /* Initiate GBA Bootstrap procedure */
          gba_bootstrap(NULL,
                        session_type,
                        &app_id_data,
                        TRUE,
                        NULL,
                        NULL,
                        NULL,
                        NULL);
        }
      }
    }
  }
  extra_param.refresh_data.refresh_req_type = MMGSDI_REFRESH_ORIG_REFRESH_REQ;

  /*Find the index of the original request*/
  mmgsdi_status = mmgsdi_cnf_build_and_queue(refresh_status,
                                             req_hdr_ptr,
                                             &extra_param,
                                             NULL,
                                             FALSE, /* sw not required
                                                       for refresh */
                                             status_word);

  /* Check if Card needs to be reset due to failure of card triggered Refresh. */
  if (mmgsdi_refresh_info_ptr[slot_index] != NULL)
  {
    mmgsdi_card_reset_on_card_refresh_failure(refresh_status,
                                              mmgsdi_refresh_info_ptr[slot_index]->source,
                                              request_hdr_ptr->slot_id);
  }

  mmgsdi_refresh_rsp_clean_up(slot_index);
  return mmgsdi_status;
}/*mmgsdi_refresh_rsp_send_cnf */


/* ============================================================================
  FUNCTION:      MMGSDI_REFRESH_MAIN_RSP

  DESCRIPTION:
    This function will process any refresh response

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type
    MMGSDI_SUCCESS:          The function was successful is sending the response
    MMGSDI_ERROR:            Could not get client index/
                             Could not build confirmation/
                             Other error
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_refresh_main_rsp(
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
)
{
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_request_header_type         req_header       = {0};
  uint32                             index            = 0;
  mmgsdi_client_req_extra_info_type *extra_param_ptr  = NULL;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;
  mmgsdi_refresh_orig_enum_type      refresh_req      = MMGSDI_REFRESH_ORIG_REFRESH_REQ;
  uint8                              slot_index       = MMGSDI_SLOT_1_INDEX;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  report_status = (uim_rsp_ptr->is_uim_success)?
                  MMGSDI_SUCCESS :
                  uim_rsp_ptr->mmgsdi_error_status;

  if(req_header.request_type == MMGSDI_REFRESH_REQ &&
     extra_param_ptr         != NULL)
  {
    refresh_req = extra_param_ptr->refresh_data.refresh_req_type;

    if(refresh_req == MMGSDI_REFRESH_ORIG_REFRESH_REQ &&
      (extra_param_ptr->refresh_data.stage != MMGSDI_REFRESH_STAGE_END_SUCCESS &&
       extra_param_ptr->refresh_data.stage != MMGSDI_REFRESH_STAGE_END_FAILED))
    {
      /*Confirmation will be built later*/
      return MMGSDI_SUCCESS;
    }

    /* Build the REFRESH Confirmation only when we get to one of these stages
     *
     * If REFRESH fails before start, we should get status from uim_rsp_ptr
     * instead of mmgsdi_refresh_info_ptr, which is null in this case.
     */
    if((refresh_req == MMGSDI_REFRESH_ORIG_REFRESH_REQ      ||
       refresh_req == MMGSDI_REFRESH_ORIG_OK_TO_REFRESH_REQ ||
       refresh_req == MMGSDI_REFRESH_ORIG_COMPLETE_REQ      ||
       refresh_req == MMGSDI_REFRESH_ORIG_TIMER_EXPIRY_REQ  ||
       refresh_req == MMGSDI_REFRESH_ORIG_PROCEED_WITH_REFRESH_REQ)            &&
      (extra_param_ptr->refresh_data.stage == MMGSDI_REFRESH_STAGE_END_SUCCESS ||
       extra_param_ptr->refresh_data.stage == MMGSDI_REFRESH_STAGE_END_FAILED))
    {
      mmgsdi_status = mmgsdi_util_get_slot_index(req_header.slot_id, &slot_index);
      if (mmgsdi_status != MMGSDI_SUCCESS)
      {
        return mmgsdi_status;
      }
      if(mmgsdi_refresh_info_ptr[slot_index] != NULL)
      {
        mmgsdi_status = mmgsdi_refresh_rsp_send_cnf(&req_header);

        /* above func will also call mmgsdi_cnf_build_and_queue(),
         * return here to avoid dup mesg to GSTK
         */

        /* Only if the refresh request is MMGSDI_REFRESH_ORIG_REFRESH_REQ,
           should we return, for the remaining to cases we still need to
           build the cnf */
        if(refresh_req == MMGSDI_REFRESH_ORIG_REFRESH_REQ)
        {
          return mmgsdi_status;
        }
      }
    }
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status, &req_header,
                                             extra_param_ptr, NULL,
                                             FALSE, /* sw not required for refresh */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
}/*mmgsdi_refresh_main_rsp*/
