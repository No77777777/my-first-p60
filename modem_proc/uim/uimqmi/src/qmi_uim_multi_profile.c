/*==============================================================================

                   Q M I _ U I M _ M U L T I _ P R O F I L E _ C

DESCRIPTION
  The Data Services Qualcomm MSM Interface Device Management Services source
  file.

Copyright (c) 2016 - 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_multi_profile.c#3 $

when        who    what, where, why
--------    ---    -------------------------------------------------------------
10/13/20    vgd    Return error for set_server_address if Modem LPA is disabled
08/17/20    vgd    Added logic to get EID using both M2M and LPA methods
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
05/01/18    vdc    Prioritize ERA-GLONASS over LPA
04/19/17    ku     GET EID for M2M cards
04/12/17    ar     Add support for retrieve/configure default server address
02/22/17    ar     Call LPA APIs only if LPA is modem centric
02/16/17    ar     Support for multiple add_profile request from same client
12/07/16    av     LPA phase2 initial changes
12/02/16    me     Combined multi-line ASSERT in one line
11/25/16    ar     Query for profile info only if LPA is modem centric
07/28/16    ar     Query internally for profile info in case of add profile
06/22/16    sp     Initial Revision
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/

#include "uim_variation.h"
#include "mmgsdilib_common.h"
#include "uim_msg.h"
#include "qmi_uim_util.h"
#include "amssassert.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim.h"
#include "mmgsdisessionlib_v.h"
#include "fs_public.h"
#include "modem_mem.h"
#include <stringl/stringl.h>

#include "qmi_uim_multi_profile.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_multi_profile_parsing.h"
#include "qmi_uim_internal.h"
#include "qmi_uim_m2m_euicc.h"

/*==============================================================================

                             EXTERNAL FUNCTIONS

==============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_LPA_EVENT_CALLBACK()

  DESCRIPTION
    This function is executed as event callback from LPA. This function makes
    a deep copy of the event data and post a command to the QMI task to be
    processed.

  PARAMETERS
  status             : LPA status for the request
  event_ptr          : LPA event data

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_lpa_event_callback
(
  lpa_result_enum_type                status,
  const lpa_event_data_type         * event_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(event_ptr);

  UIM_MSG_HIGH_1("Received event callback from LPA for event 0x%x",
                 event_ptr->evt_type);

  switch(event_ptr->evt_type)
  {
    /* Post events to the QMI context. */
    case LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT:
    case LPA_REQUEST_USER_CONSENT_EVENT:
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_LPA_EVENT);
      if (msg_ptr == NULL)
      {
        return;
      }

      msg_ptr->data.lpa_evt.evt.evt_type = event_ptr->evt_type;
      msg_ptr->data.lpa_evt.evt.slot_id  = event_ptr->slot_id;
      msg_ptr->data.lpa_evt.status       = status;
      msg_ptr->data.lpa_evt.evt.evt_data = event_ptr->evt_data;

      /* Send command */
      if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
         (present_in_queue))
      {
        uimqmi_free(msg_ptr);
      }
      msg_ptr = NULL;
      break;

    default:
      UIM_MSG_MED_1("Unhandled event: 0x%x", event_ptr->evt_type);
      break;
  }
} /* qmi_uimi_lpa_event_callback */


/*===========================================================================
  FUNCTION QMI_UIM_IS_EUICC_CARD

  DESCRIPTION
    Determine whether the card is eUICC or not

  PARAMETERS
    mmgsdi slot

  RETURN VALUE
    boolean vaue

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_is_euicc_card
(
  mmgsdi_slot_id_enum_type   mmgsdi_slot
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  uint8                          slot_index      = 0;

  ASSERT(qmi_uim_global_ptr);

  errval = uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if (errval != QMI_ERR_NONE        ||
      qmi_uim_global_ptr->is_euicc_card[slot_index] == FALSE)
  {
    return FALSE;
  }
  return TRUE;
} /* qmi_uim_is_euicc_card */


/*===========================================================================
  FUNCTION QMI_UIM_GET_ICCID_FROM_EUICC_PROFILE_ID()

  DESCRIPTION
    Retrieve ICCID from eUICC profile_id

  PARAMETERS
    profile_id        : QMI UIM profile id
    mmgsdi_slot       : MMGSDI Slot

  RETURN VALUE
    Profile info pointer

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_uim_euicc_profile_info_type* qmi_uim_get_iccid_from_euicc_profile_id
(
  mmgsdi_profile_id_enum_type   profile_id,
  mmgsdi_slot_id_enum_type      mmgsdi_slot
)
{
  uint8             profile_index = 0;
  uint8             slot_index    = 0;
  qmi_error_e_type  errval        = QMI_ERR_NONE;

  ASSERT(qmi_uim_global_ptr);

  errval = uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if (errval != QMI_ERR_NONE)
  {
    return NULL;
  }

  for (profile_index = 0; profile_index < QMI_UIM_PROFILES_MAX_V01; profile_index++)
  {
    if (qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index] != NULL)
    {
      if (qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_id == profile_id &&
          qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_info.iccid.data_len > 0)
      {
        return qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index];
      }
    }
  }

  return NULL;
} /* qmi_uim_get_iccid_from_euicc_profile_id */


/*===========================================================================
  FUNCTION QMI_UIM_CLEAR_PROFILE_INFO()

  DESCRIPTION
    Clear all the profiles data for that slot from QMI UIM cache

  PARAMETERS
    slot_index

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_clear_profile_info
(
  mmgsdi_slot_id_enum_type   mmgsdi_slot
)
{
  uint8            profile_index = 0;
  uint8            slot_index    = 0;
  qmi_error_e_type errval        = QMI_ERR_NONE;

  ASSERT(qmi_uim_global_ptr);

  errval = uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if (errval != QMI_ERR_NONE)
  {
    return;
  }

  for (profile_index = 0; profile_index < QMI_UIM_PROFILES_MAX_V01; profile_index++)
  {
    if (qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index] != NULL)
    {
      if (qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_info.icon.data_ptr != NULL)
      {
        uimqmi_free(qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_info.icon.data_ptr);
      }
      uimqmi_free(qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]);
      qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index] = NULL;
    }
  }
} /* qmi_uim_clear_profile_info */


/*===========================================================================
  FUNCTION QMI_UIM_COPY_EUICC_PROFILE_INFO()

  DESCRIPTION
    Copy all the profiles data received from LPA module

  PARAMETERS
    get_profiles_info_resp_ptr : result of LPA operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_copy_euicc_profile_info
(
  const lpa_get_profiles_info_resp_type    * get_profiles_info_resp_ptr,
  mmgsdi_slot_id_enum_type                   mmgsdi_slot
)
{
  uint8                            num_profiles     = MIN (get_profiles_info_resp_ptr->num_profiles, QMI_UIM_PROFILES_MAX_V01);
  uint8                            slot_index       = 0;
  uint8                            profile_index    = 0;
  mmgsdi_profile_id_enum_type      profile_id       = MMGSDI_PROFILE_NOT_APPLICABLE;

  ASSERT(qmi_uim_global_ptr);

  /* Clear the old data before copying the new data in global */
  qmi_uim_clear_profile_info(mmgsdi_slot);

  if (uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                       &slot_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  for (profile_index = 0; profile_index < num_profiles; profile_index++)
  {
    profile_id = qmi_uim_get_profile_id_from_index(profile_index);
    if (profile_id == MMGSDI_PROFILE_NOT_APPLICABLE)
    {
      return;
    }

    qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index] =
            (qmi_uim_euicc_profile_info_type *)uimqmi_malloc(sizeof(qmi_uim_euicc_profile_info_type));
    if (qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index] == NULL)
    {
      return;
    }

    qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_id =
          profile_id;

    qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_info =
            get_profiles_info_resp_ptr->profile_info[profile_index];

    uimqmi_deep_copy((void **)&qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_info.icon.data_ptr,
                     (const void*)get_profiles_info_resp_ptr->profile_info[profile_index].icon.data_ptr,
                     (uint32)get_profiles_info_resp_ptr->profile_info[profile_index].icon.data_len);
  }
} /* qmi_uim_copy_euicc_profile_info */


/*===========================================================================
  FUNCTION QMI_UIMI_LPA_CALLBACK()

  DESCRIPTION
    This function is executed as callback of LPA API. This function makes a
    deep copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS
    status           : LPA status for the request
    cnf_ptr          : LPA response data
    user_data_ptr    : Client data

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_lpa_callback
(
  lpa_result_enum_type          status,
  const lpa_response_data_type *cnf_ptr,
  void                         *user_data_ptr
)
{
  qmi_uim_message_type   * msg_ptr          = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  boolean                  present_in_queue = FALSE;

  UIM_MSG_HIGH_1("Got response from LPA with status: 0x%x", status);

  if(status == LPA_SUCCESS && cnf_ptr == NULL)
  {
    return;
  }

  cb_userdata_ptr = (qmi_uimi_userdata_type*)user_data_ptr;
  if (cb_userdata_ptr != NULL)
  {
    if(cb_userdata_ptr->data.lpa.is_profile_query)
    {
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_LPA_PROFILE_INFO_CB);
    }
    else
    {
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_LPA_CB);
    }
  }

  /* Copy content into a buffer and post a message to QMI task */
  if (msg_ptr != NULL)
  {
    msg_ptr->data.lpa_cnf.status    = status;

    if(cnf_ptr != NULL)
    {
      (void)memscpy(&msg_ptr->data.lpa_cnf.cnf_value,
                    sizeof(msg_ptr->data.lpa_cnf.cnf_value),
                    cnf_ptr,
                    sizeof(lpa_response_data_type));

      /* Need to deep copy the icon data from response */
      if (cnf_ptr->msg_type == LPA_GET_PROFILES_INFO_MSG)
      {
        uint8     num_profiles = MIN (msg_ptr->data.lpa_cnf.cnf_value.message.get_profiles_info_resp.num_profiles, QMI_UIM_PROFILES_MAX_V01);
        uint8     profile_index = 0;

        for (profile_index = 0; profile_index < num_profiles; profile_index++)
        {
          if (cnf_ptr->message.get_profiles_info_resp.profile_info[profile_index].icon.data_len > 0)
          {
            uimqmi_deep_copy((void **)&cnf_ptr->message.get_profiles_info_resp.profile_info[profile_index].icon.data_ptr,
                             (void *)msg_ptr->data.lpa_cnf.cnf_value.message.get_profiles_info_resp.profile_info[profile_index].icon.data_ptr,
                             (uint32)cnf_ptr->message.get_profiles_info_resp.profile_info[profile_index].icon.data_len);
          }
        }
      }
    }

    msg_ptr->data.lpa_cnf.user_data_ptr = user_data_ptr;

    /* Send command */
    if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
       (present_in_queue))
    {
      uimqmi_free(msg_ptr);
    }
    msg_ptr = NULL;
  }
} /* qmi_uimi_lpa_callback */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_SIM_PROFILE()

  DESCRIPTION
    Retrieves the profile information supported by the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_get_sim_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                errval                        = QMI_ERR_NONE;
  mmgsdi_return_enum_type         mmgsdi_status                 = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*         cb_userdata_ptr               = NULL;
  qmi_uim_tlv_item_type           tlv_data[1];
  lpa_result_enum_type            lpa_status                    = LPA_SUCCESS;
  lpa_slot_id_enum_type           lpa_slot                      = LPA_SLOT_1;
  uint8                           slot_index                    = 0;
  dsm_item_type *                 response                      = NULL;
  qmi_result_e_type               result                        = QMI_RESULT_SUCCESS;
  boolean                         retval                        = TRUE;
  uint8                           profile_index                 = 0;
  boolean                         response_pending              = FALSE;
  uint32                          file_size                     = 0;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));

  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_SIM_PROFILE,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* If multi profile config file is present then proceed with ERA-GLONASS handling.
     Otherwise, fallback to LPA if ATR bit for eUICC is set.
     Since ERA-GLONASS is only supported on slot1, fallback to LPA for request
     on other slot */
  if (tlv_data[0].data.slot.slot == MMGSDI_SLOT_1 &&
      uim_common_efs_get_file_size(UIM_COMMON_EFS_MMGSDI_MULTI_PROFILE_FILE,
                                   UIM_COMMON_EFS_CONTEXT_0,
                                   &file_size) == UIM_COMMON_EFS_SUCCESS)
  {
    /* Execute MMGSDI function to get sim profile */
    mmgsdi_status = mmgsdi_get_sim_profile(
                      qmi_uim_global_ptr->mmgsdi_client_id,
                      tlv_data[0].data.slot.slot,
                      qmi_uimi_mmgsdi_callback,
                      (mmgsdi_client_data_type)cb_userdata_ptr);

    /* Convert MMGSDI status in QMI code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
    if (errval == QMI_ERR_NONE)
    {
      response_pending = TRUE;
    }

    /* Free userdata in case of error */
    QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);
  }
  else if (qmi_uim_is_euicc_card(tlv_data[0].data.slot.slot))
  {
    boolean    lpa_modem_solution     = FALSE;

    lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

    /* If device uses LPA on the AP, disable modem functionality */
    if(!lpa_modem_solution)
    {
      errval = QMI_ERR_NOT_SUPPORTED;
      goto send_result;
    }

    /* If profile data is not available then send the request to LPA to
       retrieve profile info else if the profile info is already available then
       send response from buffer. Check data for profile_index 0, if cache
       is not available, then first entry should be invalid. In case of no
       profile the request will be sent to LPA module to read it again from
       card. */
    if (qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][0] == NULL)
    {
      errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                            &lpa_slot);
      if (errval != QMI_ERR_NONE)
      {
        goto send_result;
      }

      cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

      lpa_status = lpa_get_profiles_info(lpa_slot,
                                         qmi_uimi_lpa_callback,
                                         cb_userdata_ptr);

      /* Convert LPA status in QMI code */
      errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);
      if (errval == QMI_ERR_NONE)
      {
        response_pending = TRUE;
      }
    }
    else
    {
      lpa_get_profiles_info_resp_type *get_profiles_info_resp_ptr = NULL;

      get_profiles_info_resp_ptr = (lpa_get_profiles_info_resp_type *)uimqmi_malloc(sizeof(lpa_get_profiles_info_resp_type));
      if (get_profiles_info_resp_ptr == NULL)
      {
        errval = QMI_ERR_NO_MEMORY;
        goto send_result;
      }

      for (profile_index = 0; profile_index < QMI_UIM_PROFILES_MAX_V01; profile_index++)
      {
        if(qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index] == NULL)
        {
          break;
        }
        get_profiles_info_resp_ptr->profile_info[profile_index] =
          qmi_uim_global_ptr->qmi_uim_euicc_profile_data_ptr[slot_index][profile_index]->profile_info;
        get_profiles_info_resp_ptr->num_profiles++;
      }

      /* Insert profile info TLV*/
      if (FALSE == qmi_uim_response_get_euicc_profile_info(
                              UIMI_TLV_RESP_TAG_OPTIONAL_1,
                              &response,
                              (const lpa_get_profiles_info_resp_type *)get_profiles_info_resp_ptr))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
      uimqmi_free(get_profiles_info_resp_ptr);
    }

    /* Free userdata in case of error */
    QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);
  }
  else
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

send_result:
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  if (response_pending)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }
  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_sim_profile */


/*===========================================================================
  FUNCTION QMI_UIMI_SET_SIM_PROFILE()

  DESCRIPTION
    Switches the profile in the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_set_sim_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                  errval                       = QMI_ERR_NONE;
  mmgsdi_return_enum_type           mmgsdi_status                = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*           cb_userdata_ptr              = NULL;
  qmi_uim_tlv_item_type             tlv_data[3];
  lpa_result_enum_type              lpa_status                   = LPA_SUCCESS;
  lpa_slot_id_enum_type             lpa_slot                     = LPA_SLOT_1;
  qmi_uim_euicc_profile_info_type  *profile_info_ptr             = NULL;
  uint8                             slot_index                   = 0;
  uint32                            file_size                    = 0;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));

  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_PROFILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_SET_UNSET_PROFILE;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SET_SIM_PROFILE,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* If multi profile config file is present then proceed with ERA-GLONASS handling.
     Otherwise, fallback to LPA if ATR bit for eUICC is set.
     Since ERA-GLONASS is only supported on slot1, fallback to LPA for request
     on other slot */
  if (tlv_data[0].data.slot.slot == MMGSDI_SLOT_1 &&
      uim_common_efs_get_file_size(UIM_COMMON_EFS_MMGSDI_MULTI_PROFILE_FILE,
                                   UIM_COMMON_EFS_CONTEXT_0,
                                   &file_size) == UIM_COMMON_EFS_SUCCESS)
  {
    /* Execute MMGSDI function to set SIM profile */
    mmgsdi_status = mmgsdi_set_sim_profile(
                      qmi_uim_global_ptr->mmgsdi_client_id,
                      tlv_data[0].data.slot.slot,
                      tlv_data[1].data.sim_profile.profile_id,
                      qmi_uimi_mmgsdi_callback,
                      (mmgsdi_client_data_type)cb_userdata_ptr);

    /* Convert MMGSDI status in QMI code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

    /* Free userdata in case of error */
    QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);
  }
  else if (qmi_uim_is_euicc_card(tlv_data[0].data.slot.slot))
  {
    boolean    lpa_modem_solution     = FALSE;

    lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

    /* If device uses LPA on the AP, disable modem functionality */
    if(!lpa_modem_solution)
    {
      errval = QMI_ERR_NOT_SUPPORTED;
      goto send_result;
    }

    errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                          &lpa_slot);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }

    profile_info_ptr = qmi_uim_get_iccid_from_euicc_profile_id(tlv_data[1].data.sim_profile.profile_id,
                                                               tlv_data[0].data.slot.slot);
    if (profile_info_ptr == NULL)
    {
      errval = QMI_ERR_INVALID_ARG;
      goto send_result;
    }

    cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

    if (!tlv_data[2].present || tlv_data[2].data.set_unset_profile.value)
    {
      lpa_status = lpa_enable_profile(lpa_slot,
                                      profile_info_ptr->profile_info.iccid,
                                      qmi_uimi_lpa_callback,
                                      cb_userdata_ptr);
    }
    else
    {
      lpa_status = lpa_disable_profile(lpa_slot,
                                       profile_info_ptr->profile_info.iccid,
                                       qmi_uimi_lpa_callback,
                                       cb_userdata_ptr);
    }

    /* Convert LPA status in QMI code */
    errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);

    /* Free userdata in case of error */
    QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);
  }
  else
  {
    errval = QMI_ERR_INTERNAL;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_set_sim_profile */


/*===========================================================================
  FUNCTION QMI_UIMI_DELETE_PROFILE()

  DESCRIPTION
    Delete the requested profile from card. The command will go to LPA in all
    cases even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_delete_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                 errval              = QMI_ERR_NONE;
  lpa_result_enum_type             lpa_status          = LPA_SUCCESS;
  qmi_uimi_userdata_type*          cb_userdata_ptr     = NULL;
  qmi_uim_tlv_item_type            tlv_data[2];
  lpa_slot_id_enum_type            lpa_slot            = LPA_SLOT_1;
  qmi_uim_euicc_profile_info_type *profile_info_ptr    = NULL;
  boolean                          lpa_modem_solution  = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_PROFILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If device uses LPA on the AP, disable modem functionality */
  if(!lpa_modem_solution)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  profile_info_ptr = qmi_uim_get_iccid_from_euicc_profile_id(tlv_data[1].data.sim_profile.profile_id,
                                                             tlv_data[0].data.slot.slot);
  if (profile_info_ptr == NULL)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                        &lpa_slot);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_DELETE_PROFILE,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

  /* Execute LPA function to delete profile */
  lpa_status = lpa_delete_profile(lpa_slot,
                                  profile_info_ptr->profile_info.iccid,
                                  qmi_uimi_lpa_callback,
                                  cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);

  /* Convert LPA status to QMI err code */
  errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
}/* qmi_uimi_delete_profile */


/*===========================================================================
  FUNCTION QMI_UIMI_EUICC_MEMORY_RESET()

  DESCRIPTION
    Reset the eUICC memory for that Slot. The command will go to LPA in all
    scases even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_euicc_memory_reset
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                  errval              = QMI_ERR_NONE;
  lpa_result_enum_type              lpa_status          = LPA_SUCCESS;
  qmi_uimi_userdata_type*           cb_userdata_ptr     = NULL;
  qmi_uim_tlv_item_type             tlv_data[2];
  lpa_slot_id_enum_type             lpa_slot            = LPA_SLOT_1;
  lpa_card_memory_reset_option_type reset_option        = {TRUE, TRUE, TRUE};
  boolean                           lpa_modem_solution  = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_EUICC_RESET_OPTION_MASK;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If device uses LPA on the AP, disable modem functionality */
  if(!lpa_modem_solution)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                        &lpa_slot);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* If TLV present, copy the reset option else default is all options SET */
  if (tlv_data[1].present)
  {
    reset_option = tlv_data[1].data.euicc_reset_option_mask.euicc_reset_option;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_EUICC_MEMORY_RESET,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

  /* Execute LPA function to reset eUICC card */
  lpa_status = lpa_card_memory_reset(lpa_slot,
                                     reset_option,
                                     qmi_uimi_lpa_callback,
                                     cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);

  /* Convert MMGSDI status to QMI err code */
  errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
}/* qmi_uimi_euicc_memory_reset */


/*===========================================================================
  FUNCTION QMI_UIMI_UPDATE_PROFILE_NICKNAME()

  DESCRIPTION
    Updates the nickname of requested profile. The command will go to LPA in
    all cases even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_update_profile_nickname
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                 errval              = QMI_ERR_NONE;
  lpa_result_enum_type             lpa_status          = LPA_SUCCESS;
  qmi_uimi_userdata_type*          cb_userdata_ptr     = NULL;
  qmi_uim_tlv_item_type            tlv_data[3];
  lpa_name_type                    nickname_data       = {0, {0}};
  lpa_slot_id_enum_type            lpa_slot            = LPA_SLOT_1;
  qmi_uim_euicc_profile_info_type *profile_info_ptr    = NULL;
  boolean                          lpa_modem_solution  = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));

  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_PROFILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_EUICC_PROFILE_NAME;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If device uses LPA on the AP, disable modem functionality */
  if(!lpa_modem_solution)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  if (tlv_data[2].present)
  {
    nickname_data = tlv_data[2].data.euicc_name.euicc_name_data;
  }

  profile_info_ptr = qmi_uim_get_iccid_from_euicc_profile_id(tlv_data[1].data.sim_profile.profile_id,
                                                             tlv_data[0].data.slot.slot);
  if (profile_info_ptr == NULL)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                        &lpa_slot);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_UPDATE_NICKNAME,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

  /* Execute LPA function to update profile nickname */
  lpa_status = lpa_add_or_update_profile_nickname(lpa_slot,
                                                  profile_info_ptr->profile_info.iccid,
                                                  nickname_data,
                                                  qmi_uimi_lpa_callback,
                                                  cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);

  /* Convert LPA status to QMI err code */
  errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
}/* qmi_uimi_update_profile_nickname */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_PROFILE_INFO()

  DESCRIPTION
    Provide the complete info of the requested profile. The command
    will go to LPA in all cases even if card is not marked as eUICC
    and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_get_profile_info
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                 errval              = QMI_ERR_NONE;
  qmi_uim_tlv_item_type            tlv_data[2];
  dsm_item_type *                  response            = NULL;
  boolean                          retval              = TRUE;
  qmi_result_e_type                result              = QMI_RESULT_SUCCESS;
  mmgsdi_data_type                 lpa_profile_data    = {0, NULL};
  qmi_uim_euicc_profile_info_type *profile_info_ptr    = NULL;
  boolean                          lpa_modem_solution  = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));

  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_PROFILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If device uses LPA on the AP, disable modem functionality */
  if(!lpa_modem_solution)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  profile_info_ptr = qmi_uim_get_iccid_from_euicc_profile_id( tlv_data[1].data.sim_profile.profile_id,
                                                              tlv_data[0].data.slot.slot);
  if (profile_info_ptr == NULL)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Insert eUICC profile ICCID info in TLV */
  if (profile_info_ptr->profile_info.iccid.data_len > 0)
  {
    lpa_profile_data.data_len = profile_info_ptr->profile_info.iccid.data_len;
    lpa_profile_data.data_ptr = (uint8 *)profile_info_ptr->profile_info.iccid.data;

    if(FALSE == qmi_uim_response_generic_payload_8bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &response,
                   lpa_profile_data,
                   FALSE))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
      goto send_result;
    }
  }

  /* Insert eUICC profile state info in TLV */
  if (FALSE == qmi_uim_response_profile_state(UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                              &response,
                                              profile_info_ptr->profile_info.state))
  {
    UIM_MSG_LOW_0("Error in constructing profile state tlv");
  }

  /* Insert eUICC profile nickname info in TLV */
  if (profile_info_ptr->profile_info.nickname.name_len > 0)
  {
    lpa_profile_data.data_len = profile_info_ptr->profile_info.nickname.name_len;
    lpa_profile_data.data_ptr = (uint8 *)profile_info_ptr->profile_info.nickname.name;

    if(FALSE == qmi_uim_response_generic_payload_8bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_3,
                   &response,
                   lpa_profile_data,
                   FALSE))
    {
      UIM_MSG_LOW_0("Error in constructing profile nickname tlv");
    }
  }

  /* Insert eUICC profile service provider name info in TLV */
  if (profile_info_ptr->profile_info.spn.name_len > 0)
  {
    lpa_profile_data.data_len = profile_info_ptr->profile_info.spn.name_len;
    lpa_profile_data.data_ptr = (uint8 *)profile_info_ptr->profile_info.spn.name;

    if(FALSE == qmi_uim_response_generic_payload_8bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_4,
                   &response,
                   lpa_profile_data,
                   FALSE))
    {
      UIM_MSG_LOW_0("Error in constructing profile spn name tlv");
    }
  }

  /* Insert eUICC profile name info in TLV */
  if (profile_info_ptr->profile_info.name.name_len > 0)
  {
    lpa_profile_data.data_len = profile_info_ptr->profile_info.name.name_len;
    lpa_profile_data.data_ptr = (uint8 *)profile_info_ptr->profile_info.name.name;

    if(FALSE == qmi_uim_response_generic_payload_8bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_5,
                   &response,
                   lpa_profile_data,
                   FALSE))
    {
      UIM_MSG_LOW_0("Error in constructing profile name tlv");
    }
  }

  /* Insert eUICC profile icon info in TLV */
  if (profile_info_ptr->profile_info.icon.data_len > 0)
  {
    /* Insert eUICC profile icon type info in TLV */
    if (qmi_uim_response_profile_icon_type(UIMI_TLV_RESP_TAG_OPTIONAL_6,
                                           &response,
                                           profile_info_ptr->profile_info.icon_type))
    {
      lpa_profile_data.data_len = profile_info_ptr->profile_info.icon.data_len;
      lpa_profile_data.data_ptr = profile_info_ptr->profile_info.icon.data_ptr;

      if(FALSE == qmi_uim_response_generic_payload_16bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_7,
                     &response,
                     lpa_profile_data,
                     FALSE))
      {
        UIM_MSG_LOW_0("Error in constructing Icon tlv");
      }
    }
  }

  /* Insert profile class */
  if (FALSE == qmi_uim_response_profile_class(UIMI_TLV_RESP_TAG_OPTIONAL_8,
                                              &response,
                                              profile_info_ptr->profile_info.profile_class))
  {
    UIM_MSG_LOW_0("Error in constructing profile class tlv");
  }

  /* Insert Policy Rules info and user consent info */
  if (FALSE == qmi_uim_response_ppr_info(UIMI_TLV_RESP_TAG_OPTIONAL_9,
                                         &response,
                                         profile_info_ptr->profile_info.ppr.ppr_ids))
  {
    UIM_MSG_LOW_0("Error in constructing profile policy rules tlv");
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  /* Insert result item in the TLV */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
}/* qmi_uimi_get_profile_info */


/*===========================================================================
  FUNCTION QMI_UIMI_ADD_PROFILE()

  DESCRIPTION
    Add a new profile in card. The command will go to LPA in all cases
    even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_add_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval              = QMI_ERR_NONE;
  lpa_result_enum_type           lpa_status          = LPA_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr     = NULL;
  qmi_uim_tlv_item_type          tlv_data[4];
  lpa_data_type                  activation_code;
  lpa_data_type                  confirmation_code;
  lpa_slot_id_enum_type          lpa_slot            = LPA_SLOT_1;
  uint8                          client_index        = 0;
  uint8                          slot_index          = 0;
  boolean                        lpa_modem_solution  = FALSE;

  ASSERT(sp && sdu_in && cl_sp && sdu_in);

  memset(tlv_data, 0x00, sizeof(tlv_data));
  memset(&activation_code, 0x00, sizeof(lpa_data_type));
  memset(&confirmation_code, 0x00, sizeof(lpa_data_type));

  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_EUICC_DOWNLOAD_CODE;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_EUICC_CONFIRMATION_CODE;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[3].type      = UIMI_TLV_TYPE_USER_CONSENT_SUPPORTED;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE || qmi_uim_global_ptr == NULL)
  {
    goto send_result;
  }

  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If device uses LPA on the AP, disable modem functionality */
  if(!lpa_modem_solution)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                        &lpa_slot);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);

  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if any other client is adding a Profile. We can support multiple
     operations from the same client, but cannot currently support two clients
     adding a profile at the same time */
  for (client_index = 0; client_index < UIMQMI_SVC_MAX_CLIDS; client_index++)
  {
    qmi_uimi_client_state_type   * temp_cl_sp       = NULL;

    temp_cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[client_index];

    if(temp_cl_sp == NULL ||
       temp_cl_sp->common.clid == QMI_SVC_CLID_UNUSED ||
       temp_cl_sp->uim_info.add_profile_client_info[slot_index].add_profile_initiated == 0 ||
       temp_cl_sp == (qmi_uimi_client_state_type *)cl_sp)
    {
      continue;
    }
    errval = QMI_ERR_INCOMPATIBLE_STATE;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_ADD_PROFILE,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

  activation_code.data_len = tlv_data[1].data.euicc_download_code.code.data_len;
  activation_code.data_ptr = tlv_data[1].data.euicc_download_code.code.data_ptr;

  if (tlv_data[2].present)
  {
    confirmation_code.data_len = tlv_data[2].data.euicc_download_code.code.data_len;
    confirmation_code.data_ptr = tlv_data[2].data.euicc_download_code.code.data_ptr;
  }

  /* Execute LPA function to add a new profile */
  lpa_status = lpa_add_profile(lpa_slot,
                               activation_code,
                               confirmation_code,
                               qmi_uimi_lpa_callback,
                               qmi_uim_lpa_event_callback,
                               cb_userdata_ptr);

  if (lpa_status == LPA_SUCCESS)
  {
    /* Set the flag so that download progress indication will be sent
       only to this client */
    ((qmi_uimi_client_state_type *)cl_sp)->uim_info.add_profile_client_info[slot_index].add_profile_initiated++;
    if (tlv_data[3].present && tlv_data[3].data.euicc_user_consent_supported.value)
    {
      ((qmi_uimi_client_state_type *)cl_sp)->uim_info.add_profile_client_info[slot_index].user_consent_supported = TRUE;
    }
    else
    {
      ((qmi_uimi_client_state_type *)cl_sp)->uim_info.add_profile_client_info[slot_index].user_consent_supported = FALSE;
    }
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);

  /* Convert LPA status to QMI err code */
  errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
}/* qmi_uimi_add_profile */


/*===========================================================================
  FUNCTION QMI_UIMI_PROFILE_USER_CONSENT()

  DESCRIPTION
    Provides the user consent to LPA for the add_profile operation

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_profile_user_consent
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type          errval              = QMI_ERR_NONE;
  lpa_result_enum_type      lpa_status          = LPA_SUCCESS;
  qmi_uimi_userdata_type*   cb_userdata_ptr     = NULL;
  qmi_uim_tlv_item_type     tlv_data[2];
  lpa_slot_id_enum_type     lpa_slot            = LPA_SLOT_1;
  boolean                   lpa_modem_solution  = FALSE;

  ASSERT(sp);
  ASSERT(cmd_buf_p);
  ASSERT(cl_sp);
  ASSERT(sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_PROFILE_USER_CONSENT;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If device uses LPA on the AP, disable modem functionality */
  if(!lpa_modem_solution)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                        &lpa_slot);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_TLV_TYPE_PROFILE_USER_CONSENT,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

  lpa_status = lpa_send_user_consent(lpa_slot,
                                     tlv_data[1].data.euicc_user_consent.value,
                                     cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);

  /* Convert LPA status to QMI err code */
  errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
}/* qmi_uimi_profile_user_consent */


/*===========================================================================
  FUNCTION QMI_UIMI_EUICC_DEFAULT_SERVER_ADDRESS()

  DESCRIPTION
    Get the default SMDS and SMDP address (if configured) from card.
    Also, configure the value of defualt SMDP address in card.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_euicc_default_server_address
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval             = QMI_ERR_NONE;
  lpa_result_enum_type           lpa_status         = LPA_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr    = NULL;
  qmi_uim_tlv_item_type          tlv_data[2];
  lpa_slot_id_enum_type          lpa_slot           = LPA_SLOT_1;
  lpa_data_type                  smdp_address;
  boolean                        lpa_modem_solution = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  memset(&smdp_address, 0x00, sizeof(lpa_data_type));

  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_EUICC_DEFAULT_ADDRESS;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If device uses LPA on the AP, disable modem functionality */
  if(!lpa_modem_solution)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(tlv_data[0].data.slot.slot,
                                                        &lpa_slot);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_EUICC_DEFAULT_SERVER_ADDRESS,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.lpa.slot = tlv_data[0].data.slot.slot;

  if(tlv_data[1].present)
  {
    smdp_address.data_len = tlv_data[1].data.euicc_default_address.smdp_address.data_len;
    smdp_address.data_ptr = tlv_data[1].data.euicc_default_address.smdp_address.data_ptr;
  }

  /* Execute LPA function to get/set default address */
  lpa_status = lpa_default_server_address(lpa_slot,
                                          (tlv_data[1].present ? TRUE : FALSE),
                                          smdp_address,
                                          qmi_uimi_lpa_callback,
                                          cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);

  /* Convert LPA status to QMI err code */
  errval = qmi_uim_util_convert_lpa_status_to_errval(lpa_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_euicc_default_server_address */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_EUICC_PROFILE_RESP()

  DESCRIPTION
    Composes the response for get_sim_profile request

  PARAMETERS
    status                     : status of LPA operation
    get_profiles_info_resp_ptr : result of LPA operation
    cb_userdata_ptr            : user_data to retrieve the cmd

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_get_euicc_profile_resp
(
  lpa_result_enum_type                              status,
  const lpa_get_profiles_info_resp_type           * get_profiles_info_resp_ptr,
  qmi_uimi_userdata_type                          * cb_userdata_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  uint32                   client_reg_count = 0;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  mmgsdi_slot_id_enum_type slot             = MMGSDI_MAX_SLOT_ID_ENUM;

  ASSERT(get_profiles_info_resp_ptr);

  if (cb_userdata_ptr == NULL)
  {
    return;
  }

  /* Retrieve parameters */
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  slot             = cb_userdata_ptr->data.lpa.slot;

  /* Free callback user data */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  if (status == LPA_SUCCESS)
  {
    /* Insert profile info TLV*/
    if (FALSE == qmi_uim_response_get_euicc_profile_info(
                            UIMI_TLV_RESP_TAG_OPTIONAL_1,
                            &response,
                            get_profiles_info_resp_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
    if (errval == QMI_ERR_NONE)
    {
      /* Copy all the profile data in global for providing to apps in the
         subsequents QMI UIM API calls */
      qmi_uim_copy_euicc_profile_info(get_profiles_info_resp_ptr, slot);
    }
  }
  else
  {
    /* Convert LPA status in QMI code */
    errval = qmi_uim_util_convert_lpa_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
}/* qmi_uimi_get_euicc_profile_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_SIM_PROFILE_RESP()

  DESCRIPTION
    Composes the response for get SIM profile request

  PARAMETERS
    status                  : status of MMGSDI operation
    get_sim_profile_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_get_sim_profile_resp
(
  mmgsdi_return_enum_type                 status,
  const mmgsdi_get_sim_profile_cnf_type  *get_sim_profile_cnf_ptr
)
{
  uint8                               clid               = QMI_SVC_CLID_UNUSED;
  uint32                              client_reg_count   = 0;
  qmi_cmd_buf_type *                  cmd_buf_p          = NULL;
  qmi_uimi_userdata_type            * cb_userdata_ptr    = NULL;
  dsm_item_type *                     response           = NULL;
  qmi_error_e_type                    errval             = QMI_ERR_NONE;

  UIM_MSG_HIGH_0("Sending response for get sim profile");

  ASSERT(get_sim_profile_cnf_ptr);

  /* Retrieve userdata */
  cb_userdata_ptr =
    (qmi_uimi_userdata_type*)get_sim_profile_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid              = cb_userdata_ptr->clid;
  client_reg_count  = cb_userdata_ptr->client_reg_count;
  cmd_buf_p         = cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Insert APDU response in the TLV, in case of success */
  if (status == MMGSDI_SUCCESS)
  {
    /* Insert profile info TLV*/
    if (FALSE == qmi_uim_response_get_all_profile_info(
                            UIMI_TLV_RESP_TAG_OPTIONAL_1,
                            &response,
                            get_sim_profile_cnf_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }

    /* Insert active profile info TLV*/
    if (FALSE == qmi_uim_response_get_active_profile_info(
                            UIMI_TLV_RESP_TAG_OPTIONAL_2,
                            &response,
                            get_sim_profile_cnf_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }
  else
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_get_sim_profile_resp */


/*===========================================================================
  FUNCTION QMI_UIM_QUERY_EUICC_PROFILE_INFO

  DESCRIPTION
    Query LPA module about the profile data

  PARAMETERS
    mmgsdi_slot
    userdata

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_query_euicc_profile_info
(
  qmi_uimi_userdata_type       * cb_userdata_ptr
)
{
  lpa_result_enum_type       lpa_status      = LPA_SUCCESS;
  lpa_slot_id_enum_type      lpa_slot        = LPA_SLOT_1;
  qmi_error_e_type           errval          = QMI_ERR_NONE;

  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  errval = qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(cb_userdata_ptr->data.lpa.slot,
                                                        &lpa_slot);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  lpa_status = lpa_get_profiles_info(lpa_slot,
                                     qmi_uimi_lpa_callback,
                                     cb_userdata_ptr);

  return qmi_uim_util_convert_lpa_status_to_errval(lpa_status);
} /* qmi_uim_query_euicc_profile_info */


/*===========================================================================
  FUNCTION  QMI_UIM_SEND_ADD_PROFILE_IND()

  DESCRIPTION
    This function sends the Download Progress during add profile

  PARAMETERS
    clid                 : Clint ID to whom ind is send
    lpa_event_ptr        : The event data
    status               : LPA status

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_add_profile_ind
(
  uint8                       clid,
  const lpa_event_data_type * lpa_event_ptr,
  lpa_result_enum_type        status
)
{
  dsm_item_type                 * indication     = NULL;
  mmgsdi_slot_id_enum_type        mmgsdi_slot    = MMGSDI_MAX_SLOT_ID_ENUM;

  ASSERT(lpa_event_ptr);

  if (qmi_uim_util_convert_lpa_slot_to_mmgsdi_slot(lpa_event_ptr->slot_id,
                                                   &mmgsdi_slot) != QMI_ERR_NONE)
  {
    return;
  }

  if (FALSE == qmi_uim_response_slot(
                             UIMI_TLV_IND_TAG_MANDATORY_1,
                             &indication,
                             mmgsdi_slot))
  {
    dsm_free_packet(&indication);
    return;
  }

  if (FALSE == qmi_uim_response_euicc_download_status_data(
                   UIMI_TLV_IND_TAG_MANDATORY_2,
                   &indication,
                   lpa_event_ptr,
                   status))
  {
    dsm_free_packet(&indication);
    return;
  }

  /* If status is not SUCCESS then we send error indication to control point
     irrespective of type of event as control point might be waiting on the indication */
  if (status != LPA_SUCCESS)
  {
    if (FALSE == qmi_uim_response_download_error_cause(
                                      UIMI_TLV_IND_TAG_OPTIONAL_1,
                                      &indication,
                                      status))
    {
      UIM_MSG_LOW_0("Error in constructing download error cause tlv");
    }
  }

  if (lpa_event_ptr->evt_type == LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT &&
      lpa_event_ptr->evt_data.profile_download_progress.installation_complete == FALSE)
  {
    if (FALSE == qmi_uim_response_profile_percentage(
                     UIMI_TLV_IND_TAG_OPTIONAL_2,
                     &indication,
                     lpa_event_ptr->evt_data.profile_download_progress.percent_downloaded))
    {
      UIM_MSG_LOW_0("Error in constructing profile percentage tlv");
    }
  }
  else if (lpa_event_ptr->evt_type == LPA_REQUEST_USER_CONSENT_EVENT)
  {
    if (FALSE == qmi_uim_response_profile_policy_rules(
                          UIMI_TLV_IND_TAG_OPTIONAL_3,
                          &indication,
                          lpa_event_ptr->evt_data.request_user_consent))
    {
      UIM_MSG_LOW_0("Error in constructing user consent tlv");
      dsm_free_packet(&indication);
      return;
    }
  }

  (void)uimqmi_send_indication(clid,
                               UIMI_CMD_VAL_ADD_PROFILE_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_add_profile_ind */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_EID_RESP()

  DESCRIPTION
    Composes the response for GET_EID request

  PARAMETERS
    status                       : status of LPA operation
    get_eid_resp_ptr             : result of LPA operation
    cb_userdata_ptr              : user_data to retrieve the cmd

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_get_eid_resp
(
  lpa_result_enum_type                              status,
  const lpa_get_eid_resp_type                     * get_eid_resp_ptr,
  qmi_uimi_userdata_type                          * cb_userdata_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  uint32                   client_reg_count = 0;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  mmgsdi_slot_id_enum_type mmgsdi_slot      = MMGSDI_SLOT_NONE;

  ASSERT(get_eid_resp_ptr);

  if (cb_userdata_ptr == NULL)
  {
    return;
  }

  /* Retrieve parameters */
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  mmgsdi_slot      = cb_userdata_ptr->data.lpa.slot;

  /* Free callback user data */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  do
  {
    uint8                    slot_index       = 0;

    if (uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                         &slot_index,
                                         qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
    {
      break;
    }

    /* First check if the requested client is still valid */
    if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
    {
      break;
    }

    if (cmd_buf_p == NULL)
    {
      break;
    }

    QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

    if(status == LPA_SUCCESS)
    {
      /* Copy EID data into global */
      memset(qmi_uim_global_ptr->eid_data[slot_index].eid,
             0x00,
             sizeof(qmi_uim_global_ptr->eid_data[slot_index].eid));

      qmi_uim_global_ptr->eid_data[slot_index].eid_valid = TRUE;
      qmi_uim_global_ptr->eid_data[slot_index].eid_len = sizeof(get_eid_resp_ptr->eid);

      (void)memscpy(qmi_uim_global_ptr->eid_data[slot_index].eid,
                    sizeof(qmi_uim_global_ptr->eid_data[slot_index].eid),
                    (void *)get_eid_resp_ptr->eid,
                    sizeof(get_eid_resp_ptr->eid));

      /* Insert EID info TLV if it is a request from client */
      if (FALSE == qmi_uim_response_get_eid_info(
                              UIMI_TLV_RESP_TAG_OPTIONAL_1,
                              &response,
                              get_eid_resp_ptr))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      qmi_uim_global_ptr->eid_data[slot_index].eid_valid = FALSE;

      /* Convert MMGSDI status in QMI code */
      errval = qmi_uim_util_convert_lpa_status_to_errval(status);
    }

    /* Update result TLV and Send response */
    uimqmi_put_result_send_response_ind(cmd_buf_p,
                                        response,
                                        errval,
                                        QMUX_SERVICE_UIM,
                                        FALSE,
                                        QMI_SVC_CLID_UNUSED,
                                        (uint16)UIMI_CMD_VAL_WIDTH);
  }while(0);
}/* qmi_uimi_get_eid_resp */
  

/*===========================================================================
  FUNCTION QMI_UIMI_GET_DEFAULT_SERVER_ADDRESS_RESP()

  DESCRIPTION
    Composes the response for GET_SERVER_DEFAULT_ADDRESS request

  PARAMETERS
    status                       : status of LPA operation
    get_server_address_ptr       : result of LPA operation
    cb_userdata_ptr              : user_data to retrieve the cmd

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_get_default_server_address_resp
(
  lpa_result_enum_type                              status,
  const lpa_default_server_address_resp_type      * get_server_address_ptr,
  qmi_uimi_userdata_type                          * cb_userdata_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  uint32                   client_reg_count = 0;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  mmgsdi_data_type         lpa_server_data  = {0, NULL};

  ASSERT(get_server_address_ptr);

  if (cb_userdata_ptr == NULL)
  {
    return;
  }

  /* Retrieve parameters */
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;

  /* Free callback user data */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  if (status == LPA_SUCCESS)
  {
    UIM_MSG_HIGH_2("smds_address len len=0x%x, smdp_address len=0x%x",
                   get_server_address_ptr->default_smds_address.data_len,
                   get_server_address_ptr->default_smdp_address.data_len);

    /* Insert SMDS address TLV*/
    if(get_server_address_ptr->default_smds_address.data_len > 0)
    {
      lpa_server_data.data_len = get_server_address_ptr->default_smds_address.data_len;
      lpa_server_data.data_ptr = (uint8 *)get_server_address_ptr->default_smds_address.data_ptr;

      if(FALSE == qmi_uim_response_generic_payload_8bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_1,
                     &response,
                     lpa_server_data,
                     FALSE))
      {
        UIM_MSG_HIGH_0("Error in constructing SMDS tlv");
      }
    }

    /* Insert SMDP address TLV*/
    if(get_server_address_ptr->default_smdp_address.data_len > 0)
    {
      lpa_server_data.data_len = get_server_address_ptr->default_smdp_address.data_len;
      lpa_server_data.data_ptr = (uint8 *)get_server_address_ptr->default_smdp_address.data_ptr;

      if(FALSE == qmi_uim_response_generic_payload_8bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_2,
                     &response,
                     lpa_server_data,
                     FALSE))
      {
        UIM_MSG_HIGH_0("Error in constructing SMDP tlv");
      }
    }
  }
  else
  {
    /* Convert MMGSDI status in QMI code */
    errval = qmi_uim_util_convert_lpa_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
}/* qmi_uimi_get_default_server_address_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_LPA_PROFILE_INFO_CB()

  DESCRIPTION
    Processing of LPA response for internally triggered profile fetch

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_process_lpa_profile_info_cb
(
  qmi_uim_message_type * uim_message_ptr
)
{
  lpa_result_enum_type     status           = LPA_SUCCESS;
  lpa_response_data_type * cnf_ptr          = NULL;
  lpa_message_enum_type    msg_type;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;

  ASSERT(uim_message_ptr != NULL);

  status   = uim_message_ptr->data.lpa_cnf.status;
  cnf_ptr  = &uim_message_ptr->data.lpa_cnf.cnf_value;
  msg_type = uim_message_ptr->data.lpa_cnf.cnf_value.msg_type;

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)uim_message_ptr->data.lpa_cnf.user_data_ptr;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }
  if (status == LPA_SUCCESS && msg_type == LPA_GET_PROFILES_INFO_MSG)
  {
    qmi_uim_copy_euicc_profile_info(&cnf_ptr->message.get_profiles_info_resp,
                                    cb_userdata_ptr->data.lpa.slot);
  }
  /* Free callback user data */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;
} /* qmi_uim_process_lpa_profile_info_cb */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_LPA_CB()

  DESCRIPTION
    Processing of LPA response

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_process_lpa_cb
(
  qmi_uim_message_type * uim_message_ptr
)
{
  lpa_result_enum_type     status           = LPA_SUCCESS;
  lpa_response_data_type * cnf_ptr          = NULL;
  lpa_message_enum_type    msg_type;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;

  ASSERT(uim_message_ptr != NULL);

  status   = uim_message_ptr->data.lpa_cnf.status;
  cnf_ptr  = &uim_message_ptr->data.lpa_cnf.cnf_value;
  msg_type = uim_message_ptr->data.lpa_cnf.cnf_value.msg_type;

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)uim_message_ptr->data.lpa_cnf.user_data_ptr;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  switch (msg_type)
  {
    case LPA_GET_PROFILES_INFO_MSG:
      /* If request_id is not GET_SIM_PROFILE, it means this is response of
         internally queried profile info. So, copy the latest profile
         data and return */
      if (cb_userdata_ptr->request_id != UIMI_CMD_VAL_GET_SIM_PROFILE)
      {
        if (status == LPA_SUCCESS)
        {
          qmi_uim_copy_euicc_profile_info(&cnf_ptr->message.get_profiles_info_resp,
                                          cb_userdata_ptr->data.lpa.slot);
        }
        /* Free callback user data */
        qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
        cb_userdata_ptr = NULL;
      }
      else
      {
        qmi_uimi_get_euicc_profile_resp(status, &cnf_ptr->message.get_profiles_info_resp, cb_userdata_ptr);
      }
      break;
    case LPA_ENABLE_PROFILE_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_DISABLE_PROFILE_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_DELETE_PROFILE_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_CARD_MEMORY_RESET_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_GET_EID_MSG:
      qmi_uimi_get_eid_resp(status, &cnf_ptr->message.get_eid_resp, cb_userdata_ptr);
      break;
    case LPA_ADD_PROFILE_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_SEND_USER_CONSENT_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_SET_DEFAULT_SERVER_ADDRESS_MSG:
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_lpa_status_to_errval(status),
                                   cb_userdata_ptr);
      break;
    case LPA_GET_DEFAULT_SERVER_ADDRESS_MSG:
      qmi_uimi_get_default_server_address_resp(status,
                                               &cnf_ptr->message.default_server_address_resp,
                                               cb_userdata_ptr);
      break;
    default:
      break;
  }
} /* qmi_uim_process_lpa_cb */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_LPA_EVENT_CB()

  DESCRIPTION
    This function processes the LPA event

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_process_lpa_event_cb
(
  qmi_uim_message_type * uim_message_ptr
)
{
  uint8                           client_index   = 0;
  qmi_uimi_client_state_type    * cl_sp          = NULL;
  lpa_event_data_type           * lpa_event_ptr  = NULL;
  lpa_result_enum_type            status         = LPA_SUCCESS;
  uint8                           index          = 0;
  mmgsdi_slot_id_enum_type        mmgsdi_slot    = MMGSDI_MAX_SLOT_ID_ENUM;

  ASSERT( uim_message_ptr );

  lpa_event_ptr = &uim_message_ptr->data.lpa_evt.evt;
  status        = uim_message_ptr->data.lpa_evt.status;

  if (qmi_uim_global_ptr == NULL ||
      qmi_uim_util_convert_lpa_slot_to_mmgsdi_slot(lpa_event_ptr->slot_id,
                                                   &mmgsdi_slot) != QMI_ERR_NONE ||
      uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot, &index, qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  /* Send indication to only that client who initiated the add_profile req */
  for (client_index = 0; client_index < UIMQMI_SVC_MAX_CLIDS; client_index++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[client_index];

    if(cl_sp == NULL || cl_sp->common.clid == QMI_SVC_CLID_UNUSED ||
       cl_sp->uim_info.add_profile_client_info[index].add_profile_initiated == 0)
    {
      continue;
    }

    /* If user consent is not supported by client, directly invoke LPA API
       and don't send any indication to control point */
    if (status == LPA_SUCCESS &&
        lpa_event_ptr->evt_type == LPA_REQUEST_USER_CONSENT_EVENT &&
        cl_sp->uim_info.add_profile_client_info[index].user_consent_supported == FALSE)
    {
      (void)lpa_send_user_consent(lpa_event_ptr->slot_id,
                                  TRUE,
                                  NULL);
      break;
    }

    qmi_uim_send_add_profile_ind(cl_sp->common.clid,
                                 lpa_event_ptr,
                                 status);

    /* If profile installation is complete or if there is an error while
       add_profile is in progress, reset the add_profile_requested flag
       so that no further indication will be sent to client. Also, query
       for the new profile information to cache in QMI UIM */
    if (status != LPA_SUCCESS ||
        (lpa_event_ptr->evt_type == LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT &&
         lpa_event_ptr->evt_data.profile_download_progress.installation_complete &&
         lpa_event_ptr->evt_data.profile_download_progress.percent_downloaded >= 100))
    {
      qmi_error_e_type         errval          = QMI_ERR_INTERNAL;
      qmi_uimi_userdata_type  *cb_userdata_ptr = NULL;

      /* Reset the flag as it is error case or the profile
         installation is complete */
      cl_sp->uim_info.add_profile_client_info[index].add_profile_initiated--;
      cl_sp->uim_info.add_profile_client_info[index].user_consent_supported = FALSE;

      cb_userdata_ptr = uimqmi_malloc(sizeof(qmi_uimi_userdata_type));
      if (cb_userdata_ptr != NULL)
      {
        memset(cb_userdata_ptr, 0, sizeof(qmi_uimi_userdata_type));
               cb_userdata_ptr->data.lpa.slot             = mmgsdi_slot;
               cb_userdata_ptr->data.lpa.is_profile_query = TRUE;

        /* We are not considering here that whether LPA is implemented on
           AP side or not (NV 73849, index 2) as we fall here only if LPA
           solution is modem centric */
        errval = qmi_uim_query_euicc_profile_info(cb_userdata_ptr);
          UIM_MSG_MED_2("Internally triggered get_profile_info result: 0x%x for slot: 0x%x",
                        errval, mmgsdi_slot);
      }
    }
    break;
  }
} /* qmi_uim_process_lpa_event_cb */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_M2M_EID_RESP

  DESCRIPTION
    Generic function to composes response of GET_EID command for MMGSDI.
    If response for EID request is not success, tries to get EID using 
    LPA method.

  PARAMETERS
    uim_message_ptr : Pointer to message from QMI UIM message

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_get_m2m_eid_resp(
  qmi_uim_message_type * uim_message_ptr
)
{
  qmi_uimi_userdata_type   *cb_userdata_ptr    = NULL;
  dsm_item_type            *response           = NULL;
  qmi_error_e_type          errval             = QMI_ERR_NONE;
  mmgsdi_cnf_type         * cnf_ptr            = NULL;
  mmgsdi_slot_id_enum_type  mmgsdi_slot        = MMGSDI_SLOT_NONE;
  boolean                   lpa_modem_solution = FALSE;

  ASSERT(uim_message_ptr);

  cnf_ptr   = &uim_message_ptr->data.cnf.cnf_value;

  if(cnf_ptr == NULL)
  {
    return;
  }
    
  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)cnf_ptr->response_header.client_data;
  
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  mmgsdi_slot       = cnf_ptr->response_header.slot_id;
    
  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  /* If getting EID using M2M method is not success and Modem LPA is supported
     try to get EID using LPA */
  if(uim_message_ptr->data.cnf.status != MMGSDI_SUCCESS &&
     lpa_modem_solution)
  {
    lpa_result_enum_type     lpa_status         = LPA_SUCCESS;
    lpa_slot_id_enum_type    lpa_slot           = LPA_SLOT_1;

    if(qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot(mmgsdi_slot,
                                                    &lpa_slot))
    {
      return;
    }

    cb_userdata_ptr->data.lpa.slot = mmgsdi_slot;
    
    /* Execute LPA function to get EID */
    lpa_status = lpa_get_eid(lpa_slot,
                             qmi_uimi_lpa_callback,
                             cb_userdata_ptr);

    /* Free userdata in case of error */
    QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr);
    
    return;
  }
  else
  {
    uint8                     clid              = QMI_SVC_CLID_UNUSED;
    uint32                    client_reg_count  = 0;
    qmi_cmd_buf_type         *cmd_buf_p         = NULL;
    uint8                     slot_index        = 0;

    /* Retrieve parameters */
    cmd_buf_p         = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
    clid              = cb_userdata_ptr->clid;
    client_reg_count  = cb_userdata_ptr->client_reg_count;

    /* Free callback user data */
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr); 
    cb_userdata_ptr = NULL;

    if (uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                         &slot_index,
                                         qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
    {
      return;
    }

    UIM_MSG_HIGH_0("Sending response for GET_EID");

    /* First check if the requested client is still valid */
    if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
    {
      return;
    }

    if (cmd_buf_p == NULL)
    {
      return;
    }

    QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

    if(uim_message_ptr->data.cnf.status == MMGSDI_SUCCESS) 
    {
      memset(qmi_uim_global_ptr->eid_data[slot_index].eid,
             0x00,
             sizeof(qmi_uim_global_ptr->eid_data[slot_index].eid));

      /* Copy EID data in global */
      qmi_uim_global_ptr->eid_data[slot_index].eid_valid = TRUE;
      qmi_uim_global_ptr->eid_data[slot_index].eid_len   = cnf_ptr->get_m2m_eid_cnf.eid_response.data_len;

      (void)memscpy(qmi_uim_global_ptr->eid_data[slot_index].eid,
                    sizeof(qmi_uim_global_ptr->eid_data[slot_index].eid),
                    cnf_ptr->get_m2m_eid_cnf.eid_response.data_ptr,
                    cnf_ptr->get_m2m_eid_cnf.eid_response.data_len);

       /* Insert EID info TLV*/
      if (FALSE == qmi_uim_response_eid(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                        &response,
                                        &cnf_ptr->get_m2m_eid_cnf.eid_response))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
   }
   else
   {
     /* Send Not Supported as get_eid using M2M method is not success and 
        Modem LPA is not supported*/
     errval = QMI_ERR_NOT_SUPPORTED;
   }

    /* Update result TLV and Send response */
    uimqmi_put_result_send_response_ind(cmd_buf_p,
                                        response,
                                        errval,
                                        QMUX_SERVICE_UIM,
                                        FALSE,
                                        QMI_SVC_CLID_UNUSED,
                                        (uint16)UIMI_CMD_VAL_WIDTH);
  }
} /* qmi_uimi_get_m2m_eid_resp */