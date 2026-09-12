/*===========================================================================

                         Q M I _ U I M _ U T I L. C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016 - 2021 QUALCOMM Technologies, Inc(QTI).
Copyright (c) 2016 - 2017, 2019 - 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_util.c#6 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/09/21    pvb    Clear profile info when delete response is recieved 
07/14/20    vgd    Retrieve pending notifications from euicc after MemoryReset
12/04/19    vgd    LPA phase2.1 changes
12/23/19    vdc    Copy label info for other than USIM/CSIM apps also
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
12/27/17    ra     Disable GSM Security restrictions for Authentication on test card
07/17/17    rps    Return failure if FCP has invalid FCP template tag
05/23/17    sk     QMI error handling for repeated map sessions api
04/12/17    ar     Add support for retrieve/configure default server address 
02/21/17    bcho   Set perso state to IN PROGRESS if any app gets stuck at PIN1
01/19/17    me     Populating path internally and parsing IMSI, FPLMN, EHPLMN
12/07/16    av     LPA phase2 initial changes
12/05/16    vdc    Added support for simlock single lock for full srv policy
11/04/16    me     Combined multi-line ASSERT in one line
08/14/16    tkl    Added support for simlock single lock
07/28/16    ar     Query internally for profile info in case of add profile
07/15/16    sp     Seperate ERA-GLONASS and SAP related QMI UIM functionality
07/08/16    ar     Move qmi_uimi_recovery and unify fdn/acl get_status func.
06/30/16    av     Fix slot id to efs context conversion for uim common APIs
06/28/16    vdc    Remove callback user data from qmi uim global
06/16/16    av     Support for setting terminal capability from AP
05/26/16    vdc    Move internal enums, structs to new internal header file
05/26/16    sp     F3 logging improvements
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "mmgsdilib_p.h"
#include "qmi_uim_util.h"
#include "uim_msg.h"
#include "mmgsdisessionlib_v.h"
#include "amssassert.h"
#include <stringl/stringl.h>
#include "qmi_uim_multi_profile.h"
#include "qmi_uim_internal.h"

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CHECK_CLID()

  DESCRIPTION
    To validate if the passed client id is stil valid within the passed UIM
    service state ptr

  PARAMETERS
    clid   : Client ID

  RETURN VALUE
    TRUE if successful, else FALSE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_check_clid
(
  uint8                         clid,
  uint32                        client_reg_count
)
{
  int    client_index = 0;

  ASSERT(qmi_uim_state_ptr);

  for(client_index = 0; client_index < (int)UIMQMI_SVC_MAX_CLIDS; client_index++)
  {
    if ((qmi_uim_state_ptr->client[client_index] != NULL) &&
        (qmi_uim_state_ptr->client[client_index]->common.clid == clid))
    {
      if(qmi_uim_state_ptr->client_reg_count[client_index] == client_reg_count)
      {
        return TRUE;
      }
      else
      {
        UIM_MSG_HIGH_3("Client reg. count mismatch: 0x%x and 0x%x on clid",
                       client_reg_count,
                       qmi_uim_state_ptr->client_reg_count[client_index],
                       clid);

        return FALSE;
      }
    }
  }

  UIM_MSG_HIGH_1("Invalid clid: 0x%x", clid);

  return FALSE;
} /* qmi_uim_util_check_clid */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_RESPONSE_IND_TOKEN()

  DESCRIPTION
    Composes the TLV content of indication token

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    token              : token info to be sent

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_response_ind_token
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint32                          token
)
{
  PACKED struct PACKED_POST
  {
    uint32 token;
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.token  = token;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_util_response_ind_token */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_GET_CB_USERDATA

  DESCRIPTION
    To obtain the structure used for MMGSDI callbacks

  PARAMETERS
    request_id             : Type of the request
    cmd_buf_ptr            : QMI command buffer required for response
    token_ptr              : pointer to token info sent by client
    clid                   : Client id of the control point

  RETURN VALUE
    qmi_uimi_userdata_type : Poiner to the structure used for userdata

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_uimi_userdata_type* qmi_uim_util_get_cb_userdata
(
  qmi_uimi_cmd_val_e_type    request_id,
  qmi_cmd_buf_type         * cmd_buf_ptr,
  const uint32             * token_ptr,
  uint8                      clid
)
{
  qmi_uimi_userdata_type  * userdata_ptr = NULL;

  ASSERT(qmi_uim_global_ptr && qmi_uim_state_ptr);

  if (clid == QMI_SVC_CLID_UNUSED || clid > UIMQMI_SVC_MAX_CLIDS)
  {
    UIM_MSG_ERR_1("Invalid Client ID: 0x%x", clid);
    return NULL;
  }

  userdata_ptr = uimqmi_malloc(sizeof(qmi_uimi_userdata_type));
  if (userdata_ptr == NULL)
  {
    return NULL;
  }

  memset(userdata_ptr, 0, sizeof(qmi_uimi_userdata_type));

  /* Update parameters */
  userdata_ptr->request_id       = request_id;
  userdata_ptr->cmd_buf_ptr      = cmd_buf_ptr;
  userdata_ptr->clid             = clid;
  userdata_ptr->client_reg_count = qmi_uim_state_ptr->client_reg_count[userdata_ptr->clid - 1];

  if (token_ptr)
  {
    userdata_ptr->ind_token.token    = *token_ptr;
    userdata_ptr->ind_token.is_valid = TRUE;
  }

  return userdata_ptr;
} /* qmi_uim_util_get_cb_userdata */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CREATE_MESSAGE

  DESCRIPTION
    Allocates the memory for a QMI UIM message and initializes it.

  PARAMETERS
    message_type: QMI UIM message type

  RETURN VALUE
    Pointer to the allocated message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_uim_message_type * qmi_uim_util_create_message
(
  qmi_uim_message_enum_type message_type
)
{
  qmi_uim_message_type * msg_ptr = NULL;

  msg_ptr = uimqmi_malloc(sizeof(qmi_uim_message_type));
  if (msg_ptr != NULL)
  {
    memset(msg_ptr, 0, sizeof(qmi_uim_message_type));
    msg_ptr->message_type = message_type;
  }

  return msg_ptr;
} /* qmi_uim_util_create_message */


/*===========================================================================
FUNCTION QMI_UIM_CMP_CMD_PTR

DESCRIPTION
  This function is called by q_linear_delete_new. This function will return TRUE only when the
  message_ptr is present in the queue.

PARAMETER
  item_ptr  [Input]:  ptr to queue item
  param_ptr [Input]:  ptr to param

DEPENDENCIES
  None

RETURN VALUE
  1 - if there is a match
  0 - if there is no match

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
static int qmi_uim_cmp_cmd_ptr
(
  void *item_ptr,
  void *param_ptr
)
{
  if(item_ptr == NULL || param_ptr == NULL)
  {
    return 0;
  }
  
  if(param_ptr == item_ptr)
  {
    UIM_MSG_LOW_0("Passed message_ptr is present in the queue");
    return 1;
  }

  return 0;
} /* qmi_uim_cmp_cmd_ptr */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_POST_MESSAGE

  DESCRIPTION
    Adds a message to the QMI UIM queue and posts an event to DCC task,
    so that it can be processed later in the right context.If signal is 
    not set and if the command is still present in the queue, delete the
    command from queue, make "present_in_queue" to TRUE, so that
    qmi_uim_msg_ptr will be freed in caller function.

  PARAMETERS
    uim_message_ptr     : QMI UIM message
    present_in_queue_ptr: Tells whether command is still present in queue

  RETURN VALUE
    boolean

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_post_message
(
  qmi_uim_message_type * uim_message_ptr,
  boolean              * present_in_queue_ptr
)
{
  ASSERT(uim_message_ptr != NULL && qmi_uim_global_ptr != NULL &&
         present_in_queue_ptr != NULL);

  /* Add the message to the MMGSDI queue */
  q_put(&qmi_uim_global_ptr->qmi_uim_q, &uim_message_ptr->link_next);

  /* Set "present_in_queue" to FALSE by default, so that what ever may the
     value of "present_in_queue" in caller function it will be made to FALSE
     and will be made TRUE only when we are unable to set the signal and
     message_ptr is present in the queue */
  (*present_in_queue_ptr) = FALSE;

  /* Enable the MMGSDI task sig to process in MMGSDI context*/
  if(mmgsdi_set_qmi_uim_sig() == FALSE)
  {
    qmi_uim_message_type *deleted_uim_message_ptr = NULL;

    deleted_uim_message_ptr = q_linear_delete_new(&qmi_uim_global_ptr->qmi_uim_q,
                                                  qmi_uim_cmp_cmd_ptr,
                                                  uim_message_ptr,
                                                  NULL,
                                                  NULL);

    if(deleted_uim_message_ptr != NULL)
    {
      (*present_in_queue_ptr) = TRUE;
    }
   
    UIM_MSG_ERR_1("Could not set signal, del_msg_ptr = 0x%x", deleted_uim_message_ptr);
    return FALSE;
  }
  return TRUE;
} /* qmi_uim_util_post_message */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_FREE_CB_USERDATA

  DESCRIPTION
    To free the structure passed as user data while invoking APIs.
    It is safe to call it from different task context when API callback is hit.

  PARAMETERS
    cb_userdata_ptr        : Callback user data pointer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_util_free_cb_userdata
(
  qmi_uimi_userdata_type*  cb_userdata_ptr
)
{
  if (cb_userdata_ptr != NULL)
  {
    /* Free memory specific for each request id */
    switch(cb_userdata_ptr->request_id)
    {
      case UIMI_CMD_VAL_READ_RECORD:
        if (cb_userdata_ptr->data.read_record.records_cnf != NULL)
        {
          uint16   i = 0;
          for (i = 0; i < cb_userdata_ptr->data.read_record.read_records; i++)
          {
            /* Need to loop only thru the records that were read (successfully or not) */
            if (cb_userdata_ptr->data.read_record.records_cnf[i].read_data.data_ptr)
            {
              uimqmi_free(cb_userdata_ptr->data.read_record.records_cnf[i].read_data.data_ptr);
              cb_userdata_ptr->data.read_record.records_cnf[i].read_data.data_ptr = NULL;
            }
          }
              uimqmi_free(cb_userdata_ptr->data.read_record.records_cnf);
          cb_userdata_ptr->data.read_record.records_cnf = NULL;
        }
        break;
      case UIMI_CMD_VAL_WRITE_RECORD:
        if (cb_userdata_ptr->data.write_record.data.data_ptr != NULL)
        {
              uimqmi_free(cb_userdata_ptr->data.write_record.data.data_ptr);
          cb_userdata_ptr->data.write_record.data.data_ptr = NULL;
        }
        break;
      case UIMI_CMD_VAL_PERSONALIZATION:
      case UIMI_CMD_VAL_PERSONALIZATION_SECURE:
        if(cb_userdata_ptr->data.simlock_lock.code_ptr != NULL)
        {
          uimqmi_free(cb_userdata_ptr->data.simlock_lock.code_ptr);
          cb_userdata_ptr->data.simlock_lock.code_ptr = NULL;
        }
        break;
       default:
        /* Nothing to free */
        break;
    }

    memset(cb_userdata_ptr, 0, sizeof(qmi_uimi_userdata_type));
    uimqmi_free(cb_userdata_ptr);
    cb_userdata_ptr = NULL;

  }
} /* qmi_uim_util_free_cb_userdata */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SAME_FILE

  DESCRIPTION
    Compares to files passed by path.

  PARAMETERS
    file1         : first file
    file2         : second file

  RETURN VALUE
    boolean       : Indicates if the two files are the same

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_same_file
(
  mmgsdi_path_type         file1,
  mmgsdi_path_type         file2
)
{
  if (file1.path_len > 0 &&
      file1.path_len <= MMGSDI_MAX_PATH_LEN &&
      file1.path_len == file2.path_len)
  {
    if (memcmp(file1.path_buf, file2.path_buf, file1.path_len * sizeof(uint16)) == 0)
    {
      return TRUE;
    }
  }
  return FALSE;
} /* qmi_uim_util_same_file */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_MMGSDI_STATUS_TO_ERRVAL

  DESCRIPTION
    To obtain QMI client-specific error message from MMGSDI error message

  PARAMETERS
    mmgsdi_status   : MMGSDI return value

  RETURN VALUE
    errval          : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_convert_mmgsdi_status_to_errval
(
  mmgsdi_return_enum_type mmgsdi_status
)
{
  qmi_error_e_type errval  = QMI_ERR_INTERNAL;

  switch(mmgsdi_status)
  {
    case MMGSDI_SUCCESS:
      errval = QMI_ERR_NONE;
      break;

    case MMGSDI_WARNING_NO_INFO_GIVEN:
    case MMGSDI_INCOMPAT_PIN_STATUS:
      errval = QMI_ERR_NO_EFFECT;
      break;

    case MMGSDI_INCORRECT_CODE:
    case MMGSDI_PERSO_INVALID_CK:
      errval = QMI_ERR_INCORRECT_PIN;
      break;

    case MMGSDI_CODE_BLOCKED:
    case MMGSDI_PERSO_CK_BLOCKED:
      errval = QMI_ERR_PIN_BLOCKED;
      break;

    case MMGSDI_CODE_PERM_BLOCKED:
      errval = QMI_ERR_PIN_PERM_BLOCKED;
      break;

    case MMGSDI_PIN_NOT_INITIALIZED:
      errval = QMI_ERR_SIM_NOT_INITIALIZED;
      break;

    case MMGSDI_ACCESS_DENIED:
      errval = QMI_ERR_ACCESS_DENIED;
      break;

    case MMGSDI_NOT_FOUND:
    case MMGSDI_WRONG_FCP_TEMPLATE_TAG:
      errval = QMI_ERR_SIM_FILE_NOT_FOUND;
      break;

    case MMGSDI_INCORRECT_PARAMS:
      errval = QMI_ERR_INTERNAL;
      break;

    case MMGSDI_AUTH_ERROR_GSM_CNTXT_NOT_SUP:
    case MMGSDI_AUTH_ERROR_INCORRECT_MAC:
    case MMGSDI_AUTH_FAIL:
    case MMGSDI_AUTS_FAIL:
      errval = QMI_ERR_AUTHENTICATION_FAILED;
      break;

    case MMGSDI_MULTI_PROFILE_NO_EFFECT:
      errval = QMI_ERR_NO_EFFECT;
      break;

    case MMGSDI_MULTI_PROFILE_NOT_SUPPORTED:
      errval = QMI_ERR_NOT_SUPPORTED;
      break;

    case MMGSDI_MAPPING_IN_PROGRESS:
      errval = QMI_ERR_DEVICE_IN_USE;
      break;

    default:
      errval = QMI_ERR_INTERNAL;
      break;
  }

  if(mmgsdi_status != MMGSDI_SUCCESS )
  {
    UIM_MSG_HIGH_1("Received mmgsdi_status 0x%x", mmgsdi_status);
  }
  return errval;
} /* qmi_uim_util_convert_mmgsdi_status_to_errval() */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_UIMDRV_STATUS_TO_ERRVAL

  DESCRIPTION
    To obtain QMI client specific error message from UIMDRV error message

  PARAMETERS
    uimdrv_status : UIMDRV return value

  RETURN VALUE
    qmi_error_e_type : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_convert_uimdrv_status_to_errval
(
  uim_return_type uimdrv_status
)
{
  qmi_error_e_type errval  = QMI_ERR_INTERNAL;

  switch(uimdrv_status)
  {
    case UIM_SUCCESS:
      errval = QMI_ERR_NONE;
      break;

    case UIM_INCOMPATIBLE_STATE_ERR_S:
      errval = QMI_ERR_INCOMPATIBLE_STATE;
      break;

    default:
      errval = QMI_ERR_INTERNAL;
      break;
  }

  return errval;
} /* qmi_uim_util_convert_uimdrv_status_to_errval */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SESSION_TYPE_TO_SESSION_ID

  DESCRIPTION
    Convert a session type and AID in the corresponding session id.

  PARAMETERS
    session_id_ptr : Output parameter with session id
    session_info   : session type
    aid            : AID value

  RETURN VALUE
    errval         : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_session_type_to_session_id
(
  mmgsdi_session_id_type *        session_id_ptr,
  mmgsdi_session_type_enum_type   session_type,
  mmgsdi_static_data_type         aid
)
{
  uint16  i  = 0;

  ASSERT(session_id_ptr && qmi_uim_global_ptr);

  /* Initialize output value */
  *session_id_ptr = UIM_INVALID_SESSION_ID;

  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_gw_session_id[0];
      break;

    case MMGSDI_1X_PROV_PRI_SESSION:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_1x_session_id[0];
      break;

    case MMGSDI_GW_PROV_SEC_SESSION:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_gw_session_id[1];
      break;

    case MMGSDI_1X_PROV_SEC_SESSION:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_1x_session_id[1];
      break;

    case MMGSDI_GW_PROV_TER_SESSION:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_gw_session_id[2];
      break;

    case MMGSDI_1X_PROV_TER_SESSION:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_1x_session_id[2];
      break;

    case MMGSDI_CARD_SESSION_SLOT_1:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_card_slot_session_id[0];
      break;

    case MMGSDI_CARD_SESSION_SLOT_2:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_card_slot_session_id[1];
      break;

    case MMGSDI_CARD_SESSION_SLOT_3:
      *session_id_ptr = qmi_uim_global_ptr->mmgsdi_card_slot_session_id[2];
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_1:
    case MMGSDI_NON_PROV_SESSION_SLOT_2:
    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      if (aid.data_len == QMI_UIM_TLV_CHANNEL_ID_LEN)
      {
        mmgsdi_slot_id_enum_type slot = MMGSDI_SLOT_1;
        if(session_type == MMGSDI_NON_PROV_SESSION_SLOT_1)
        {
          slot = MMGSDI_SLOT_1;
        }
        else if(session_type == MMGSDI_NON_PROV_SESSION_SLOT_2)
        {
          slot = MMGSDI_SLOT_2;
        }
        else
        {
          slot = MMGSDI_SLOT_3;
        }

        /* AID length is 1 for a session TLV that is overloaded with channel id, so check
           apdu_channel info for session_id */
        for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
        {
          if (qmi_uim_global_ptr->apdu_channel[i] &&
              qmi_uim_global_ptr->apdu_channel[i]->slot == slot &&
              qmi_uim_global_ptr->apdu_channel[i]->logical_channel == aid.data_ptr[0])
          {
            *session_id_ptr = qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id;
            break;
          }
        }
      }
      else if (aid.data_len <= MMGSDI_MAX_AID_LEN)
      {
        /* For all other AID length, check non_prov_session info */
        for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
        {
          if (qmi_uim_global_ptr->non_prov_session[i] != NULL &&
              qmi_uim_global_ptr->non_prov_session[i]->session_type == session_type)
          {
            if ((qmi_uim_global_ptr->non_prov_session[i]->aid.data_len == 0 &&
                 aid.data_len == 0)||
                (qmi_uim_global_ptr->non_prov_session[i]->aid.data_len > 0 &&
                 qmi_uim_global_ptr->non_prov_session[i]->aid.data_len == aid.data_len &&
                 memcmp((void*)qmi_uim_global_ptr->non_prov_session[i]->aid.data_ptr,
                        (void*)aid.data_ptr,
                         aid.data_len ) == 0))
            {
              *session_id_ptr = qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id;
              break;
            }
          }
        }
      }
      else
      {
        UIM_MSG_ERR_1("Unexpected aid length: 0x%x", aid.data_len);
        return QMI_ERR_INTERNAL;
      }
      break;

    default:
      UIM_MSG_ERR_1("Invalid session type: 0x%x", session_type);
      return QMI_ERR_INTERNAL;
  }

  /* Even if the session was found, it might be not initialized yet.
     Check the value before continuing */
  if (*session_id_ptr == UIM_INVALID_SESSION_ID)
  {
    UIM_MSG_ERR_1("Session not ready: 0x%x", session_type);
    if(session_type != MMGSDI_NON_PROV_SESSION_SLOT_1 &&
       session_type != MMGSDI_NON_PROV_SESSION_SLOT_2 &&
       session_type != MMGSDI_NON_PROV_SESSION_SLOT_3)
    {
      return QMI_ERR_DEVICE_NOT_READY;
    }
    return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_util_session_type_to_session_id */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SESSION_ID_TO_SESSION_TYPE

  DESCRIPTION
    Convert a session id in the corresponding session type and AID.

  PARAMETERS
    session_type_ptr : Output parameter with session type
    aid_ptr          : Output parameter with AID
    session_id       : session id

  RETURN VALUE
    errval           : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_session_id_to_session_type
(
  mmgsdi_session_type_enum_type * session_type_ptr,
  mmgsdi_static_data_type *       aid_ptr,
  mmgsdi_session_id_type          session_id
)
{
  uint16            i       = 0;
  qmi_error_e_type  result  = QMI_ERR_NONE;

  ASSERT(session_type_ptr && aid_ptr && qmi_uim_global_ptr);

  /* Initialize AID */
  memset(aid_ptr, 0, sizeof(mmgsdi_static_data_type));

  if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[0])
  {
    *session_type_ptr = MMGSDI_GW_PROV_PRI_SESSION;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[0])
  {
    *session_type_ptr = MMGSDI_1X_PROV_PRI_SESSION;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[1])
  {
    *session_type_ptr = MMGSDI_GW_PROV_SEC_SESSION;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[1])
  {
    *session_type_ptr = MMGSDI_1X_PROV_SEC_SESSION;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[2])
  {
    *session_type_ptr = MMGSDI_GW_PROV_TER_SESSION;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[2])
  {
    *session_type_ptr = MMGSDI_1X_PROV_TER_SESSION;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_card_slot_session_id[0])
  {
    *session_type_ptr = MMGSDI_CARD_SESSION_SLOT_1;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_card_slot_session_id[1])
  {
    *session_type_ptr = MMGSDI_CARD_SESSION_SLOT_2;
  }
  else if (session_id == qmi_uim_global_ptr->mmgsdi_card_slot_session_id[2])
  {
    *session_type_ptr = MMGSDI_CARD_SESSION_SLOT_3;
  }
  else
  {
    /* Default to error case */
    result = QMI_ERR_INTERNAL;

    /* Check if it's a non provisioning session id */
    for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
    {
      if (qmi_uim_global_ptr->non_prov_session[i] != NULL  &&
          qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id == session_id)
      {
        *session_type_ptr = qmi_uim_global_ptr->non_prov_session[i]->session_type;
        (void)memscpy(aid_ptr,
                      sizeof(mmgsdi_static_data_type),
                      &qmi_uim_global_ptr->non_prov_session[i]->aid,
                      sizeof(mmgsdi_static_data_type));

        result = QMI_ERR_NONE;
        break;
      }
    }
  }

  if (result != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Session id to session type: invalid session id: 0x%x",
                  session_id);
  }

  return result;
} /* qmi_uim_util_session_id_to_session_type */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SESSION_TYPE_TO_CARD_TYPE

  DESCRIPTION
    Convert a session type and AID in the corresponding card type.

  PARAMETERS
    card_type_ptr : Output parameter with card type
    session_info  : session type
    aid           : AID value

  RETURN VALUE
    errval         : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_session_type_to_card_type
(
  mmgsdi_app_enum_type *          card_type_ptr,
  mmgsdi_session_type_enum_type   session_type,
  mmgsdi_static_data_type         aid
)
{
  uint16                   i     = 0;
  uint16                   index = UIM_INVALID_APP_INDEX;
  mmgsdi_slot_id_enum_type slot  = MMGSDI_SLOT_1;

  ASSERT(card_type_ptr && qmi_uim_global_ptr);

  /* Initialize output value */
  *card_type_ptr = MMGSDI_APP_NONE;

  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      index = qmi_uim_global_ptr->card_state.index_gw[0];
      break;

    case MMGSDI_1X_PROV_PRI_SESSION:
      index = qmi_uim_global_ptr->card_state.index_1x[0];
      break;

    case MMGSDI_GW_PROV_SEC_SESSION:
      index = qmi_uim_global_ptr->card_state.index_gw[1];
      break;

    case MMGSDI_1X_PROV_SEC_SESSION:
      index = qmi_uim_global_ptr->card_state.index_1x[1];
      break;

    case MMGSDI_GW_PROV_TER_SESSION:
      index = qmi_uim_global_ptr->card_state.index_gw[2];
      break;

    case MMGSDI_1X_PROV_TER_SESSION:
      index = qmi_uim_global_ptr->card_state.index_1x[2];
      break;

    case MMGSDI_CARD_SESSION_SLOT_1:
    case MMGSDI_CARD_SESSION_SLOT_2:
    case MMGSDI_CARD_SESSION_SLOT_3:
      if(session_type == MMGSDI_CARD_SESSION_SLOT_1)
      {
        slot = MMGSDI_SLOT_1;
      }
      else if(session_type == MMGSDI_CARD_SESSION_SLOT_2)
      {
        slot = MMGSDI_SLOT_2;
      }
      else
      {
        slot = MMGSDI_SLOT_3;
      }

      for (i = 0; i < qmi_uim_global_ptr->card_state.num_apps &&
                  i < QMI_UIM_MAX_APP_COUNT; i++)
      {
        if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
            qmi_uim_global_ptr->card_state.application[i]->slot == slot)
        {
          index = i;
          break;
        }
      }
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_1:
    case MMGSDI_NON_PROV_SESSION_SLOT_2:
    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      if(session_type == MMGSDI_NON_PROV_SESSION_SLOT_1)
      {
        slot = MMGSDI_SLOT_1;
      }
      else if(session_type == MMGSDI_NON_PROV_SESSION_SLOT_2)
      {
        slot = MMGSDI_SLOT_2;
      }
      else
      {
        slot = MMGSDI_SLOT_3;
      }

      for (i = 0; i < qmi_uim_global_ptr->card_state.num_apps &&
                  i < QMI_UIM_MAX_APP_COUNT; i++)
      {
        if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
            qmi_uim_global_ptr->card_state.application[i]->slot == slot &&
            qmi_uim_global_ptr->card_state.application[i]->aid.data_len == aid.data_len)
        {
          if (memcmp(qmi_uim_global_ptr->card_state.application[i]->aid.data_ptr,
                     aid.data_ptr,
                     (uint32)aid.data_len) == 0)
          {
            index = i;
            break;
          }
        }
      }
      break;

    default:
      UIM_MSG_ERR_1("Invalid session type: 0x%x", session_type);
      return QMI_ERR_INTERNAL;
  }

  /* Check if the index is valid */
  if (index >= qmi_uim_global_ptr->card_state.num_apps ||
      index >= QMI_UIM_MAX_APP_COUNT ||
      qmi_uim_global_ptr->card_state.application[index] == NULL)
  {
    UIM_MSG_ERR_1("Invalid index: 0x%x", index);
    return QMI_ERR_INTERNAL;
  }

  /* Check if type is valid */
  if (qmi_uim_global_ptr->card_state.application[index]->app_type != MMGSDI_APP_SIM &&
      qmi_uim_global_ptr->card_state.application[index]->app_type != MMGSDI_APP_RUIM &&
      qmi_uim_global_ptr->card_state.application[index]->app_type != MMGSDI_APP_USIM &&
      qmi_uim_global_ptr->card_state.application[index]->app_type != MMGSDI_APP_CSIM)
  {
    UIM_MSG_ERR_1("Invalid card type: 0x%x",
                  qmi_uim_global_ptr->card_state.application[index]->app_type);
    return QMI_ERR_INTERNAL;
  }

  *card_type_ptr = qmi_uim_global_ptr->card_state.application[index]->app_type;
  return QMI_ERR_NONE;
} /* qmi_uim_util_session_type_to_card_type */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SESSION_ID_TO_SLOT

  DESCRIPTION
    Convert a session id in the corresponding slot.

  PARAMETERS
    slot_ptr      : Output parameter with slot value
    session_id    : session id

  RETURN VALUE
    errval        : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_session_id_to_slot
(
  mmgsdi_slot_id_enum_type * slot_ptr,
  mmgsdi_session_id_type     session_id
)
{
  uint16 i     = 0;
  uint16 index = UIM_INVALID_APP_INDEX;

  ASSERT(slot_ptr && qmi_uim_global_ptr);

  for (i = 0; i < QMI_UIM_MAX_PROV_SESSIONS; i++)
  {
    if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[i])
    {
      index = qmi_uim_global_ptr->card_state.index_gw[i];
      break;
    }
    else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[i])
    {
      index = qmi_uim_global_ptr->card_state.index_1x[i];
      break;
    }
  }

  if (index == UIM_INVALID_APP_INDEX)
  {
    for (i = 0; i < QMI_UIM_MAX_CARD_SESSIONS; i++)
    {
      if (session_id == qmi_uim_global_ptr->mmgsdi_card_slot_session_id[i])
      {
        switch (i)
        {
          case 0:
            *slot_ptr = MMGSDI_SLOT_1;
            return QMI_ERR_NONE;
          case 1:
            *slot_ptr = MMGSDI_SLOT_2;
            return QMI_ERR_NONE;
          case 2:
            *slot_ptr = MMGSDI_SLOT_3;
            return QMI_ERR_NONE;
          default:
            return QMI_ERR_INTERNAL;
        }
      }
    }

    /* Check if it's a non provisioning session id */
    for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
    {
      if (qmi_uim_global_ptr->non_prov_session[i] != NULL  &&
          qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id == session_id)
      {
        switch (qmi_uim_global_ptr->non_prov_session[i]->session_type)
        {
          case MMGSDI_NON_PROV_SESSION_SLOT_1:
            *slot_ptr = MMGSDI_SLOT_1;
            return QMI_ERR_NONE;

          case MMGSDI_NON_PROV_SESSION_SLOT_2:
            *slot_ptr = MMGSDI_SLOT_2;
            return QMI_ERR_NONE;

          case MMGSDI_NON_PROV_SESSION_SLOT_3:
            *slot_ptr = MMGSDI_SLOT_3;
            return QMI_ERR_NONE;

          default:
            /* Nothing to do */
            break;
        }
      }
    }

    /* Check if the session id belongs to an APDU session */
    for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
    {
      if (qmi_uim_global_ptr->apdu_channel[i] != NULL &&
          qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id == session_id)
      {
        *slot_ptr = qmi_uim_global_ptr->apdu_channel[i]->slot;
        return QMI_ERR_NONE;
      }
    }

    UIM_MSG_HIGH_1("Session id to slot: invalid session id: 0x%x", session_id);
    return QMI_ERR_INTERNAL;
  }

  /* We arrive here in case of provisioning application.
     Check index value. */
  if (index == UIM_INVALID_APP_INDEX ||
      index >= qmi_uim_global_ptr->card_state.num_apps ||
      index >= QMI_UIM_MAX_APP_COUNT ||
      qmi_uim_global_ptr->card_state.application[index] == NULL)
  {
    UIM_MSG_ERR_1("Invalid index for slot conversion: 0x%x", index);
    return QMI_ERR_INTERNAL;
  }

  *slot_ptr = qmi_uim_global_ptr->card_state.application[index]->slot;
  return QMI_ERR_NONE;
} /* qmi_uim_util_session_id_to_slot */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CARD_PROTOCOL()

  DESCRIPTION
    Utility function to find out if the card is ICC or UICC.

  PARAMETERS
    slot                       : slot

  RETURN VALUE
    mmgsdi_protocol_enum_type  : Whether the card is ICC or UICC

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
mmgsdi_protocol_enum_type qmi_uim_util_card_protocol
(
  mmgsdi_slot_id_enum_type  slot
)
{
  uint16   index            = 0;
  boolean  unknown_uicc_app = FALSE;

  ASSERT(qmi_uim_global_ptr);

  if (slot != MMGSDI_SLOT_1 &&
      slot != MMGSDI_SLOT_2 &&
      slot != MMGSDI_SLOT_3)
  {
    return MMGSDI_NO_PROTOCOL;
  }

  for (index = 0; index < qmi_uim_global_ptr->card_state.num_apps &&
                  index < QMI_UIM_MAX_APP_COUNT; index++)
  {
    if (qmi_uim_global_ptr->card_state.application[index] != NULL &&
        qmi_uim_global_ptr->card_state.application[index]->slot == slot)
    {
      if (qmi_uim_global_ptr->card_state.application[index]->app_type == MMGSDI_APP_SIM ||
          qmi_uim_global_ptr->card_state.application[index]->app_type == MMGSDI_APP_RUIM)
      {
        return MMGSDI_ICC;
      }
      else if (qmi_uim_global_ptr->card_state.application[index]->app_type == MMGSDI_APP_USIM ||
               qmi_uim_global_ptr->card_state.application[index]->app_type == MMGSDI_APP_CSIM)
      {
        return MMGSDI_UICC;
      }
      else if (qmi_uim_global_ptr->card_state.application[index]->app_type == MMGSDI_APP_UNKNOWN)
      {
        unknown_uicc_app = TRUE;
      }
    }
  }

  /* no known GW or 1X apps but there is unknown uicc app, must be UICC card */
  if (unknown_uicc_app == TRUE)
  {
    return MMGSDI_UICC;
  }

  /* no known GW or 1X application or unknown app, return error for now */
  UIM_MSG_HIGH_0("Found neither any known GW/1X application nor unknown UICC app");
  return MMGSDI_NO_PROTOCOL;
} /* qmi_uim_util_card_protocol */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SESSION_ID_TO_INDEX

  DESCRIPTION
    Convert a session id into an index.

  PARAMETERS
    index_ptr     : output parameter with index
    session_id    : session id

  RETURN VALUE
    errval        : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_session_id_to_index
(
  uint16                 * index_ptr,
  mmgsdi_session_id_type   session_id
)
{
  uint16                         i            = 0;
  qmi_error_e_type               result       = QMI_ERR_NONE;
  mmgsdi_session_type_enum_type  session_type = MMGSDI_GW_PROV_PRI_SESSION;
  mmgsdi_static_data_type        aid;

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));

  ASSERT(index_ptr != NULL);
  ASSERT(qmi_uim_global_ptr != NULL);

  /* Convert session id in session type */
  result = qmi_uim_util_session_id_to_session_type(&session_type,
                                                   &aid,
                                                   session_id);
  if (result != QMI_ERR_NONE)
  {
    return result;
  }

  /* Convert session type into refresh index */
  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      *index_ptr = 0;
      return QMI_ERR_NONE;
    case MMGSDI_1X_PROV_PRI_SESSION:
      *index_ptr = 1;
      return QMI_ERR_NONE;
    case MMGSDI_GW_PROV_SEC_SESSION:
      *index_ptr = 2;
      return QMI_ERR_NONE;
    case MMGSDI_1X_PROV_SEC_SESSION:
      *index_ptr = 3;
      return QMI_ERR_NONE;
    case MMGSDI_GW_PROV_TER_SESSION:
      *index_ptr = 4;
      return QMI_ERR_NONE;
    case MMGSDI_1X_PROV_TER_SESSION:
      *index_ptr = 5;
      return QMI_ERR_NONE;
    case MMGSDI_CARD_SESSION_SLOT_1:
      *index_ptr = 6;
      return QMI_ERR_NONE;
    case MMGSDI_CARD_SESSION_SLOT_2:
      *index_ptr = 7;
      return QMI_ERR_NONE;
    case MMGSDI_CARD_SESSION_SLOT_3:
      *index_ptr = 8;
      return QMI_ERR_NONE;
    case MMGSDI_NON_PROV_SESSION_SLOT_1:
    case MMGSDI_NON_PROV_SESSION_SLOT_2:
    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      /* Search if this session id was already used */
      for (i = 0; i < UIM_REFRESH_NON_PROV_COUNT; i++)
      {
        if (qmi_uim_global_ptr->non_prov_refresh_session_id[i] == session_id)
        {
          *index_ptr = UIM_REFRESH_PROV_COUNT + i;
          return QMI_ERR_NONE;
        }
      }
      /* Find a slot for this new session id */
      for (i = 0; i < UIM_REFRESH_NON_PROV_COUNT; i++)
      {
        if (qmi_uim_global_ptr->non_prov_refresh_session_id[i] == UIM_INVALID_SESSION_ID)
        {
          qmi_uim_global_ptr->non_prov_refresh_session_id[i] = session_id;
          *index_ptr = UIM_REFRESH_PROV_COUNT + i;
          return QMI_ERR_NONE;
        }
      }
      break;
    default:
      break;
  }

  return QMI_ERR_INTERNAL;
} /* qmi_uim_util_session_id_to_index */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_MMGDSI_SLOT_ID_TO_UIM_SLOT_ID()

  DESCRIPTION
    Converts MMGSDI slot id to UIM slot id.

  PARAMETERS
    mmgsdi_slot_id : MMGSDI slot id
    uim_slot_id    : output parameter containing UIM slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id
(
  mmgsdi_slot_id_enum_type  mmgsdi_slot_id,
  uim_slot_type            *uim_slot_id
)
{
  ASSERT(uim_slot_id);

  switch(mmgsdi_slot_id)
  {
    case MMGSDI_SLOT_1:
      *uim_slot_id = UIM_SLOT_1;
      break;

    case MMGSDI_SLOT_2:
      *uim_slot_id = UIM_SLOT_2;
      break;

    case MMGSDI_SLOT_3:
      *uim_slot_id = UIM_SLOT_3;
      break;

    default:
      *uim_slot_id = UIM_SLOT_NONE;
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_UIM_SLOT_ID_TO_MMGSDI_SLOT_ID()

  DESCRIPTION
    Converts UIM slot id to MMGSDI slot id.

  PARAMETERS
    uim_slot_id         : UIM slot id
    mmgsdi_slot_id_ptr  : output parameter containing MMGSDI slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_uim_slot_id_to_mmgsdi_slot_id
(
  uim_slot_type             uim_slot_id,
  mmgsdi_slot_id_enum_type *mmgsdi_slot_id_ptr
)
{
  ASSERT(mmgsdi_slot_id_ptr);

  switch(uim_slot_id)
  {
    case UIM_SLOT_1:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_1;
      break;

    case UIM_SLOT_2:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_2;
      break;

    case UIM_SLOT_3:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_3;
      break;

    default:
      *mmgsdi_slot_id_ptr = MMGSDI_MAX_SLOT_ID_ENUM;
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_util_uim_slot_id_to_mmgsdi_slot_id */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SLOT_INDEX_TO_MMGSDI_SLOT()

  DESCRIPTION
    Determines the MMGSDI slot id from the slot index

  PARAMETERS
    slot_index          : slot index
    mmgsdi_slot_ptr     : output parameter containing MMGSDI slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_slot_index_to_mmgsdi_slot
(
  uint8                              slot_index,
  mmgsdi_slot_id_enum_type          *mmgsdi_slot_ptr
)
{
  ASSERT(mmgsdi_slot_ptr);

  switch(slot_index)
  {
    case 0:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_1;
      break;

    case 1:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_2;
      break;

    case 2:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_3;
      break;

    default:
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
}/* qmi_uim_util_slot_index_to_mmgsdi_slot */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_LPA_STATUS_TO_ERRVAL

  DESCRIPTION
    To obtain QMI client-specific error message from LPA error message

  PARAMETERS
    lpa_status   : LPA return value

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_convert_lpa_status_to_errval
(
  lpa_result_enum_type lpa_status
)
{
  qmi_error_e_type errval  = QMI_ERR_INTERNAL;

  UIM_MSG_HIGH_1("Received lpa_status 0x%x", lpa_status);

  switch(lpa_status)
  {
    case LPA_SUCCESS:
      errval = QMI_ERR_NONE;
      break;

    case LPA_ERROR_PROFILE_NOT_IN_DISABLED_STATE:
    case LPA_ERROR_PROFILE_NOT_IN_ENABLED_STATE:
      errval = QMI_ERR_NO_EFFECT;
      break;

    case LPA_ERROR_HEAP_EXHAUSTED:
      errval = QMI_ERR_NO_MEMORY;
      break;

    case LPA_ERROR_PROFILE_NOT_FOUND:
      errval = QMI_ERR_INVALID_PROFILE;
      break;

    case LPA_ERROR_NETWORK:
      errval = QMI_ERR_NETWORK_NOT_READY;
      break;

    case LPA_ERROR_SIM_NOT_READY:
      errval = QMI_ERR_SIM_NOT_INITIALIZED;
      break;

    case LPA_ERROR_CONFIRMATION_CODE_REQUIRED:
      errval = QMI_ERR_INFO_UNAVAILABLE;
      break;

    case LPA_ERROR_CAT_BUSY:
      errval = QMI_ERR_INCOMPATIBLE_STATE;
      break;

    default:
      errval = QMI_ERR_INTERNAL;
      break;
  }

  return errval;
} /* qmi_uim_util_convert_lpa_status_to_errval */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_MMGSDI_SLOT_TO_LPA_SLOT

  DESCRIPTION
    To obtain LPA slot type from MMGSDI slot type

  PARAMETERS
    mmgsdi_slot        : MMGSDI slot id
    lpa_slot_ptr       : output parameter containing LPA slot id

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot
(
  mmgsdi_slot_id_enum_type          mmgsdi_slot,
  lpa_slot_id_enum_type            *lpa_slot_ptr
)
{
  if(lpa_slot_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  switch(mmgsdi_slot)
  {
    case MMGSDI_SLOT_1:
      *lpa_slot_ptr = LPA_SLOT_1;
      break;

    case MMGSDI_SLOT_2:
      *lpa_slot_ptr = LPA_SLOT_2;
      break;

    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_util_convert_mmgsdi_slot_to_lpa_slot */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_LPA_SLOT_TO_MMGSDI_SLOT

  DESCRIPTION
    To obtain MMGSDI slot type from LPA slot type

  PARAMETERS
    lpa_slot              : LPA slot id
    mmgsdi_slot_ptr       : output parameter containing MMGSDI slot id

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_convert_lpa_slot_to_mmgsdi_slot
(
  lpa_slot_id_enum_type              lpa_slot,
  mmgsdi_slot_id_enum_type          *mmgsdi_slot_ptr
)
{
  if(mmgsdi_slot_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  switch(lpa_slot)
  {
    case LPA_SLOT_1:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_1;
      break;

    case LPA_SLOT_2:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_2;
      break;

    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_util_convert_lpa_slot_to_mmgsdi_slot */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_GSTK_SLOT_ID_TO_MMGSDI_SLOT_ID()

  DESCRIPTION
    Converts GSTK slot id to MMGSDI slot id.

  PARAMETERS
    gstk_slot_id        : GSTK slot id
    mmgsdi_slot_id_ptr  : output parameter containing MMGSDI slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_gstk_slot_id_to_mmgsdi_slot_id
(
  gstk_slot_id_enum_type     gstk_slot_id,
  mmgsdi_slot_id_enum_type  *mmgsdi_slot_id_ptr
)
{
  switch(gstk_slot_id)
  {
    case GSTK_SLOT_1:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_1;
      break;

    case GSTK_SLOT_2:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_2;
      break;

    case GSTK_SLOT_3:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_3;
      break;

    default:
      *mmgsdi_slot_id_ptr = MMGSDI_MAX_SLOT_ID_ENUM;
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_util_gstk_slot_id_to_mmgsdi_slot_id */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_MMGSDI_SLOT_TO_EFS_CONTEXT()

  DESCRIPTION
    Determines the uim common EFS context from mmgsdi slot id

  PARAMETERS
    mmgsdi_slot_id      : MMGSDI slot id

  RETURN VALUE
    uim_common_efs_context_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
uim_common_efs_context_type qmi_uim_util_convert_mmgsdi_slot_to_efs_context
(
  mmgsdi_slot_id_enum_type                   mmgsdi_slot_id
)
{
  uim_common_efs_context_type      efs_context = UIM_COMMON_EFS_CONTEXT_MAX;

  switch(mmgsdi_slot_id)
  {
    case MMGSDI_SLOT_1:
      efs_context = UIM_COMMON_EFS_CONTEXT_0;
      break;

    case MMGSDI_SLOT_2:
      efs_context = UIM_COMMON_EFS_CONTEXT_1;
      break;

    case MMGSDI_SLOT_3:
      efs_context = UIM_COMMON_EFS_CONTEXT_2;
      break;

    default:
      break;
  }

  return efs_context;
}/* qmi_uim_util_convert_mmgsdi_slot_to_efs_context */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_QMI_SLOT_TO_MMGSDI_SLOT()

  DESCRIPTION
    Converts QMI slot information to MMGSDI slot information.

  PARAMETERS
    qmi_slot_id     : QMI UIM slot information
    mmgsdi_slot_ptr : MMGSDI slot information

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_convert_qmi_slot_to_mmgsdi_slot
(
  uim_slot_enum_v01          qmi_slot_id,
  mmgsdi_slot_id_enum_type  *mmgsdi_slot_ptr
)
{
  if(mmgsdi_slot_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  switch (qmi_slot_id)
  {
    case UIM_SLOT_1_V01:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_1;
      break;
    case UIM_SLOT_2_V01:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_2;
      break;
    case UIM_SLOT_3_V01:
      *mmgsdi_slot_ptr = MMGSDI_SLOT_3;
      break;
    case UIM_SLOT_4_V01:
    case UIM_SLOT_5_V01:
      return QMI_ERR_NOT_SUPPORTED;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /*  qmi_uim_convert_qmi_slot_to_mmgsdi_slot */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_IS_APDU_SESSION

  DESCRIPTION
    Check if the given session id belongs to an APDU session

  PARAMETERS
    session_id    : session id

  RETURN VALUE
    boolean       : indicates if the session id belongs to an APDU session

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_is_apdu_session
(
  mmgsdi_session_id_type   session_id
)
{
  uint8 i = 0;

  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] != NULL &&
        qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id == session_id)
    {
      return TRUE;
    }
  }

  return FALSE;
} /* qmi_uim_util_is_apdu_session */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_READ_IMSI_FOR_SESSION_ID()

  DESCRIPTION
    Reads the IMSI from the card for the specified Session_ID

  PARAMETERS
    session_id       : Session ID
    imsi_ptr         : Pointer for IMSI data

  RETURN VALUE
    errval           : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_read_imsi_for_session_id
(
  mmgsdi_session_id_type session_id,
  qmi_uim_imsi_type*     imsi_ptr
)
{
  mmgsdi_file_enum_type         file_name        = MMGSDI_MAX_FILE_ENUM;
  mmgsdi_app_enum_type          app_type         = MMGSDI_APP_NONE;
  mmgsdi_session_type_enum_type session_type     = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_return_enum_type       mmgsdi_status    = MMGSDI_ERROR;
  qmi_error_e_type              result           = QMI_ERR_INTERNAL;
  mmgsdi_static_data_type       aid              = { 0, };
  mmgsdi_data_type              mmgsdi_imsi_data = { 0, NULL};

  ASSERT(imsi_ptr);

  /* Get the session type in order to get the app_type for the IMSI file read */
  result = qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id);
  if(result != QMI_ERR_NONE)
  {
    return result;
  }

  /* Use the session type to get the app_type for the IMSI file read */
  result = qmi_uim_util_session_type_to_card_type(&app_type, session_type, aid);
  if(result != QMI_ERR_NONE)
  {
    return result;
  }

  switch(app_type)
  {
    case MMGSDI_APP_SIM:
      file_name = MMGSDI_GSM_IMSI;
      break;
    case MMGSDI_APP_RUIM:
      file_name = MMGSDI_CDMA_IMSI_M;
      break;
    case MMGSDI_APP_USIM:
      file_name = MMGSDI_USIM_IMSI;
      break;
    case MMGSDI_APP_CSIM:
      file_name = MMGSDI_CSIM_IMSI_M;
      break;
    default:
      return QMI_ERR_INTERNAL;
  }

  mmgsdi_status = mmgsdi_session_read_cache_file_size(session_id, file_name,
                                                      &mmgsdi_imsi_data.data_len);

  imsi_ptr->len = (uint8)mmgsdi_imsi_data.data_len;

  if(mmgsdi_status != MMGSDI_SUCCESS || imsi_ptr->len > sizeof(imsi_ptr->imsi))
  {
    UIM_MSG_ERR_2("Error in IMSI length. mmgsdi_status:0x%x len:0x%x",
                  mmgsdi_status, imsi_ptr->len);
    return QMI_ERR_INTERNAL;
  }

  mmgsdi_imsi_data.data_ptr = imsi_ptr->imsi;

  mmgsdi_status = mmgsdi_session_read_cache(session_id, file_name, mmgsdi_imsi_data);

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    return QMI_ERR_NONE;
  }

  return QMI_ERR_INTERNAL;
} /* qmi_uim_util_read_imsi_for_session_id */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_ADD_CL_SP_TO_NON_PROV_SESSION_INDEX()

  DESCRIPTION
    Adds the passed in client state pointer to the non-prov. session
    index requested (in the global array).

    The cl_sp is not added if already present or if there are no
    slots available in the non-prov. session index.

  PARAMETERS
    index      : non-prov. session index in the global array
    client_sp  : client state pointer

  RETURN VALUE
    errval         : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_util_add_cl_sp_to_non_prov_session_index
(
  uint16                       index,
  qmi_uimi_client_state_type * client_sp
)
{
  uint16                       cl_sp_index = 0;
  uint16                       empty_index = UIM_MAX_NON_PROV_CLIENTS;

  ASSERT(index < UIM_MAX_NON_PROV_SESSIONS && qmi_uim_global_ptr);

  if((client_sp == NULL) ||
     (qmi_uim_global_ptr->non_prov_session[index] == NULL))
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Go through the cl_sp array, check if passed in cl_sp is already present,
     and also find any empty slot */
  for(cl_sp_index = 0;
      cl_sp_index < (uint16)UIM_MAX_NON_PROV_CLIENTS;
      cl_sp_index++)
  {
    if ((qmi_uim_global_ptr->non_prov_session[index]->client_info.opening_cl_sp[cl_sp_index] == NULL) &&
        (empty_index == UIM_MAX_NON_PROV_CLIENTS))
    {
      /* Empty slot found */
      empty_index = cl_sp_index;
    }

    /* If cl_sp already present, return */
    if (qmi_uim_global_ptr->non_prov_session[index]->client_info.opening_cl_sp[cl_sp_index] ==
         client_sp)
    {
      return QMI_ERR_NONE;
    }
  }

  /* Index should be valid... otherwise it means that we didn't find
     the corresponding cl_sp and the array is also full */
  if (empty_index == UIM_MAX_NON_PROV_CLIENTS)
  {
    UIM_MSG_ERR_3("Cannot add cl_sp: 0x%x to non-prov. session index: 0x%x, session ID: 0x%x",
                  client_sp,
                  index,
                  qmi_uim_global_ptr->non_prov_session[index]->mmgsdi_session_id);
    return QMI_ERR_NO_MEMORY;
  }

  /* Store the cl_sp */
  qmi_uim_global_ptr->non_prov_session[index]->client_info.opening_cl_sp[empty_index] =
    client_sp;
  /* Increment the Client count */
  qmi_uim_global_ptr->non_prov_session[index]->client_info.client_count++;

  return QMI_ERR_NONE;
} /* qmi_uim_util_add_cl_sp_to_non_prov_session_index */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_UPDATE_NON_PROV_SESSION_CL_SP()

  DESCRIPTION
    Extracts the Session Info from the passed in TLV list, and if it is a
    non-prov. session already present in the global array, then the
    passed in cl_sp is associated with the session.

  PARAMETERS
    tlv_data_ptr   : pointer to list of TLVs
    tlv_data_count : number of TLVs
    client_sp      : client state pointer

  RETURN VALUE
    errval         : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_update_non_prov_session_cl_sp
(
  qmi_uim_tlv_item_type      * tlv_data_ptr,
  uint8                        tlv_data_count,
  qmi_uimi_client_state_type * client_sp
)
{
  uint16                                 i               = 0;
  qmi_uim_tlv_session_information_type * tlv_session_ptr = NULL;

  ASSERT(tlv_data_ptr && tlv_data_count > 0 && qmi_uim_global_ptr);

  if(client_sp == NULL)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Find session information in the TLVs */
  for (i = 0; i < tlv_data_count; i++)
  {
    if (tlv_data_ptr[i].type == UIMI_TLV_TYPE_SESSION_INFORMATION &&
        tlv_data_ptr[i].present)
    {
      tlv_session_ptr = &tlv_data_ptr[i].data.session_information;
      break;
    }
  }
  if (tlv_session_ptr == NULL)
  {
    UIM_MSG_ERR_0("No session information in TLV");
    return QMI_ERR_INTERNAL;
  }

  /* Verify session type is non provisioning */
  if (tlv_session_ptr->session_type != MMGSDI_NON_PROV_SESSION_SLOT_1 &&
      tlv_session_ptr->session_type != MMGSDI_NON_PROV_SESSION_SLOT_2 &&
      tlv_session_ptr->session_type != MMGSDI_NON_PROV_SESSION_SLOT_3)
  {
    return QMI_ERR_NONE;
  }

  /* Check if the requested non provisioning session is already available
     in the global array */
  for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
  {
    if (qmi_uim_global_ptr->non_prov_session[i] != NULL  &&
        qmi_uim_global_ptr->non_prov_session[i]->session_type == tlv_session_ptr->session_type &&
        qmi_uim_global_ptr->non_prov_session[i]->aid.data_len > 0 &&
        qmi_uim_global_ptr->non_prov_session[i]->aid.data_len == tlv_session_ptr->aid.data_len)
    {
      /* Add cl_sp to the non-prov. session */
      if (memcmp((void*)qmi_uim_global_ptr->non_prov_session[i]->aid.data_ptr,
                 (void*)tlv_session_ptr->aid.data_ptr,
                 tlv_session_ptr->aid.data_len) == 0)
      {
        return qmi_uim_util_add_cl_sp_to_non_prov_session_index(i, client_sp);
      }
    }
  }
  return QMI_ERR_NONE;
} /* qmi_uim_util_update_non_prov_session_cl_sp */


/*===========================================================================
  FUNCTION QMI_UIMI_UTIL_POPULATE_RESPONSE()

  DESCRIPTION
    Populates the response TLV

  PARAMETERS
    qmi_err      :  status of QMI operation
    token_valid  :  if valid token is passed
    token        :  value of token
    token_tlv_id :  ID of response TLV

  RETURN VALUE
    Pointer to the response data

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_util_populate_response
(
  qmi_error_e_type  errval,
  boolean           token_valid,
  uint32            token,
  byte              token_tlv_id
)
{
  dsm_item_type      * response = NULL;
  boolean              retval   = FALSE;

  /* In case of error, send immediate ERROR response. In case of success,
     send immediate SUCCESS response with token for clients requesting indication and
     send PENDING response for all other client requests */
  if (errval == QMI_ERR_NONE)
  {
    if (!token_valid)
    {
      return QMI_SVC_RESPONSE_PENDING;
    }
    else
    {
      UIM_MSG_LOW_0("Sending token TLV");

      /* Insert indication token TLV in response */
      if (FALSE == qmi_uim_util_response_ind_token(
                       token_tlv_id,
                       &response,
                       token))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
  }

  retval = qmi_svc_put_result_tlv(
             &response,
             (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
             errval);

  CHECK_RETVAL();

  return response;
} /* qmi_uimi_util_populate_response */


/*===========================================================================
  FUNCTION QMI_UIMI_UTIL_POPULATE_ENCRYPTED_RESPONSE()

  DESCRIPTION
    Populates the encrypted response TLV

  PARAMETERS
    errval   : status of QMI operation
    cmd_id   : command
    req_id   : request id from client

  RETURN VALUE
    Pointer to the response data

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_util_populate_encrypted_response
(
  qmi_error_e_type                   errval,
  qmi_uimi_cmd_val_e_type            cmd_id,
  uint16                             req_id
)
{
  qmi_error_e_type     error               = QMI_ERR_NONE;
  dsm_item_type      * response            = NULL;
  boolean              retval              = FALSE;

  /* Build the result and encrypt the data and populatate the output sdu */
  error = qmi_uim_response_encrypted_status(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                            cmd_id,
                                            req_id,
                                            errval,
                                            &response);
  if(error != QMI_ERR_NONE)
  {
    /* Even if encrypted status fails, continue with sending
       response back to client so they are at least notified. */
    UIM_MSG_HIGH_0("failed to build and encrypt response");
  }

  retval = qmi_svc_put_result_tlv(
             &response,
             (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
             errval);

  CHECK_RETVAL();

  return response;
} /* qmi_uimi_util_populate_encrypted_response */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_DUPLICATE_TLV_DATA()

  DESCRIPTION
    Duplicate an array of TLV data making sure that internal pointers
    are correctly re-mapped and ownership of external pointers is
    transfered to the destination, zeroing the source.

  PARAMETERS
    dest_ptr       : array of destination TLV data
    src_ptr        : array of source TLV data
    tlv_data_count : size of array of TLV data

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_util_duplicate_tlv_data
(
  qmi_uim_tlv_item_type * dest_ptr,
  uint32                  dest_size,
  qmi_uim_tlv_item_type * src_ptr,
  uint8                   count
)
{
  uint8  i  = 0;

  ASSERT(dest_ptr != NULL && src_ptr != NULL && count > 0 &&
         (count * sizeof(qmi_uim_tlv_item_type)) <= dest_size);

  /* Memcpy the entire TLV array */
  (void)memscpy(dest_ptr,
                dest_size,
                src_ptr,
                count * sizeof(qmi_uim_tlv_item_type));

  /* Zero the source TLV. Ownership of pointers was passed to the
     destination and need to avoid invalid pointers around */
  memset(src_ptr, 0, count * sizeof(qmi_uim_tlv_item_type));

  /* Take care of internal pointers, where memory was not allocated separately.
     They need to be updated to point to the new location */
  for (i = 0; i < count; i++)
  {
    switch(dest_ptr[i].type)
    {
      case UIMI_TLV_TYPE_SET_PIN_PROTECTION:
        dest_ptr[i].data.set_pin_protection.pin_data.data_ptr =
          dest_ptr[i].data.set_pin_protection.pin_value;
        break;
      case UIMI_TLV_TYPE_VERIFY_PIN:
        dest_ptr[i].data.verify_pin.pin_data.data_ptr =
          dest_ptr[i].data.verify_pin.pin_value;
        break;
      case UIMI_TLV_TYPE_UNBLOCK_PIN:
        dest_ptr[i].data.unblock_pin.puk_data.data_ptr =
          dest_ptr[i].data.unblock_pin.puk_value;
        dest_ptr[i].data.unblock_pin.pin_data.data_ptr =
          dest_ptr[i].data.unblock_pin.pin_value;
        break;
      case UIMI_TLV_TYPE_CHANGE_PIN:
        dest_ptr[i].data.change_pin.old_pin_data.data_ptr =
          dest_ptr[i].data.change_pin.old_pin_value;
        dest_ptr[i].data.change_pin.new_pin_data.data_ptr =
          dest_ptr[i].data.change_pin.new_pin_value;
        break;
      case UIMI_TLV_TYPE_DECRYPTED_PIN:
        dest_ptr[i].data.decrypted_pin.decrypted_pin_data.data_ptr =
          dest_ptr[i].data.decrypted_pin.decrypted_pin_value;
        break;
      default:
        /* Nothing to do for this TLV */
        break;
    }
  }
} /* qmi_uim_util_duplicate_tlv_data */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_ADD_AID()

  DESCRIPTION
    Utility function to add an application

  PARAMETERS
    aid_ptr  : pointer to AID + application type
    slot     : slot

  RETURN VALUE
    uint16: indicates the index of the application. The function
            return UIM_INVALID_APP_INDEX if the application could not
            be added

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
uint16 qmi_uim_util_add_aid
(
  const mmgsdi_aid_type*   aid_ptr,
  mmgsdi_slot_id_enum_type slot
)
{
  uint16 index = 0;

  ASSERT(aid_ptr && qmi_uim_global_ptr);

  /* Check slot */
  if (slot != MMGSDI_SLOT_1 && slot != MMGSDI_SLOT_2 && slot != MMGSDI_SLOT_3)
  {
    return UIM_INVALID_APP_INDEX;
  }

  /* Check if there is space */
  if (qmi_uim_global_ptr->card_state.num_apps >= QMI_UIM_MAX_APP_COUNT)
  {
    UIM_MSG_ERR_0("No space to add application");
    return UIM_INVALID_APP_INDEX;
  }

  index = qmi_uim_global_ptr->card_state.num_apps;

  /* Make sure the index is not already used */
  if (qmi_uim_global_ptr->card_state.application[index] != NULL)
  {
    UIM_MSG_ERR_1("Index 0x%x already in use", index);
    return UIM_INVALID_APP_INDEX;
  }

  qmi_uim_global_ptr->card_state.application[index] = (qmi_uim_app_info_type *)uimqmi_malloc(
                                                        sizeof(qmi_uim_app_info_type));
  if (qmi_uim_global_ptr->card_state.application[index] == NULL)
  {
    return UIM_INVALID_APP_INDEX;
  }

  memset(qmi_uim_global_ptr->card_state.application[index],
         0x00,
         sizeof(qmi_uim_app_info_type));

  /* Copy AID value */
  (void)memscpy(&qmi_uim_global_ptr->card_state.application[index]->aid,
                sizeof(mmgsdi_static_data_type),
                &aid_ptr->aid,
                sizeof(mmgsdi_static_data_type));

  /* Copy Label value */
  (void)memscpy(&qmi_uim_global_ptr->card_state.application[index]->label,
                sizeof(mmgsdi_static_data_type),
                &aid_ptr->label,
                sizeof(mmgsdi_static_data_type));

  /* Copy app type and slot */
  qmi_uim_global_ptr->card_state.application[index]->app_type      = aid_ptr->app_type;
  qmi_uim_global_ptr->card_state.application[index]->slot          = slot;

  /* Initialize other states with generic values */
  qmi_uim_global_ptr->card_state.application[index]->app_state     = QMI_UIM_APP_STATE_DETECTED;
  qmi_uim_global_ptr->card_state.application[index]->perso_state   = QMI_UIM_PERSO_STATE_UNKNOWN;
  qmi_uim_global_ptr->card_state.application[index]->perso_feature = MMGSDI_MAX_PERSO_FEATURE_ENUM;

  qmi_uim_global_ptr->card_state.num_apps++;

  return index;
} /* qmi_uim_util_add_aid */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_FIND_AID()

  DESCRIPTION
    Utility function to locate the application with the specified AID

  PARAMETERS
    aid_ptr  : pointer to AID + application type
    slot     : slot

  RETURN VALUE
    uint16: indicates the index of the application. The function
            return UIM_INVALID_APP_INDEX if the application is not
            found

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
uint16 qmi_uim_util_find_aid
(
  const mmgsdi_aid_type *  aid_ptr,
  mmgsdi_slot_id_enum_type slot
)
{
  uint16 index;

  ASSERT(aid_ptr && qmi_uim_global_ptr);

  if (aid_ptr->aid.data_len > MMGSDI_MAX_AID_LEN)
  {
    return UIM_INVALID_APP_INDEX;
  }

  for (index = 0; index < qmi_uim_global_ptr->card_state.num_apps &&
                  index < QMI_UIM_MAX_APP_COUNT; index++)
  {
    if (qmi_uim_global_ptr->card_state.application[index] != NULL &&
        qmi_uim_global_ptr->card_state.application[index]->app_type == aid_ptr->app_type &&
        qmi_uim_global_ptr->card_state.application[index]->slot == slot &&
        qmi_uim_global_ptr->card_state.application[index]->aid.data_len == aid_ptr->aid.data_len)
    {
      if (aid_ptr->aid.data_len == 0)
      {
        return index;
      }
      if (memcmp( (void*)qmi_uim_global_ptr->card_state.application[index]->aid.data_ptr,
                  (void*)aid_ptr->aid.data_ptr,
                  aid_ptr->aid.data_len ) == 0)
      {
        return index;
      }
    }
  }

  return UIM_INVALID_APP_INDEX;
} /* qmi_uim_util_find_aid */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_REMOVE_AID()

  DESCRIPTION
    Utility function to remove an application

  PARAMETERS
    aid_ptr  : pointer to AID + application type
    slot     : slot

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_util_remove_aid
(
  const mmgsdi_aid_type*   aid_ptr,
  mmgsdi_slot_id_enum_type slot
)
{
  uint16 j     = 0;
  uint16 index = 0;

  ASSERT(aid_ptr && qmi_uim_global_ptr);

  index = qmi_uim_util_find_aid(aid_ptr, slot);

  if (index == UIM_INVALID_APP_INDEX ||
      index >= qmi_uim_global_ptr->card_state.num_apps ||
      qmi_uim_global_ptr->card_state.application[index] == NULL)
  {
    /* Application does not exist: immediately return */
    return;
  }

  /* Free info of the app which needs to be removed */
  uimqmi_free(qmi_uim_global_ptr->card_state.application[index]);
  qmi_uim_global_ptr->card_state.application[index] = NULL;

  /* Shift all applications in the table */
  for (j = index; j < qmi_uim_global_ptr->card_state.num_apps - 1 &&
                  j < QMI_UIM_MAX_APP_COUNT - 1; j++)
  {
    qmi_uim_global_ptr->card_state.application[j] =
      qmi_uim_global_ptr->card_state.application[j + 1];
    qmi_uim_global_ptr->card_state.application[j + 1] = NULL;
  }

  /* Decrease number of apps */
  qmi_uim_global_ptr->card_state.num_apps--;

  for (j = 0; j < QMI_UIM_MAX_PROV_SESSIONS; j++)
  {
    /* Update index of GW provisioning app */
    if (qmi_uim_global_ptr->card_state.index_gw[j] == index)
    {
      qmi_uim_global_ptr->card_state.index_gw[j] = UIM_INVALID_APP_INDEX;
    }
    else if (qmi_uim_global_ptr->card_state.index_gw[j] > index &&
             qmi_uim_global_ptr->card_state.index_gw[j] < QMI_UIM_MAX_APP_COUNT)
    {
      qmi_uim_global_ptr->card_state.index_gw[j]--;
    }
    else
    {
      /* Nothing to do */
    }

    /* Update index of 1X provisioning app */
    if (qmi_uim_global_ptr->card_state.index_1x[j] == index)
    {
      qmi_uim_global_ptr->card_state.index_1x[j] = UIM_INVALID_APP_INDEX;
    }
    else if (qmi_uim_global_ptr->card_state.index_1x[j] > index &&
             qmi_uim_global_ptr->card_state.index_1x[j] < QMI_UIM_MAX_APP_COUNT)
    {
      qmi_uim_global_ptr->card_state.index_1x[j]--;
    }
    else
    {
      /* Nothing to do */
    }
  }
} /* qmi_uim_util_remove_aid */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_REMOVE_ALL_AID_IN_SLOT()

  DESCRIPTION
    Utility function to remove all applications for a specific slot

  PARAMETERS
    slot     : slot

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_util_remove_all_aid_in_slot
(
  mmgsdi_slot_id_enum_type slot
)
{
  uint16 i  =  0;

  while(i < qmi_uim_global_ptr->card_state.num_apps && i < QMI_UIM_MAX_APP_COUNT)
  {
    if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
        qmi_uim_global_ptr->card_state.application[i]->slot == slot)
    {
      mmgsdi_aid_type   aid_value = { MMGSDI_APP_NONE };

      aid_value.app_type = qmi_uim_global_ptr->card_state.application[i]->app_type;
      (void)memscpy(&aid_value.aid,
                    sizeof(mmgsdi_static_data_type),
                    &qmi_uim_global_ptr->card_state.application[i]->aid,
                    sizeof(mmgsdi_static_data_type));

      qmi_uim_util_remove_aid(&aid_value, slot);
    }
    else
    {
      i++;
    }
  }
} /* qmi_uim_util_remove_all_aid_in_slot */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_IS_PROVISIONING_APP_INDEX()

  DESCRIPTION
    Checks if the given index is associated with a provisioning application

  PARAMETERS
    index : index of application

  RETURN VALUE
    TRUE  : If the index is associated with prov app
    FALSE : If the index is not associated with prov app

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_is_provisioning_app_index
(
  uint16 index
)
{
  uint8   i      = 0;
  boolean result = FALSE;

  ASSERT(qmi_uim_global_ptr);

  for (i = 0; i < QMI_UIM_MAX_PROV_SESSIONS; i++)
  {
    if (index == qmi_uim_global_ptr->card_state.index_gw[i] ||
        index == qmi_uim_global_ptr->card_state.index_1x[i])
    {
      result = TRUE;
      break;
    }
  }

  return result;
} /* qmi_uim_util_is_provisioning_app_index */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_IS_REQUEST_REJECTED()

  DESCRIPTION
    If APDU security restrictions are enabled on the device, this function
    checks the incoming request to determine if it should be rejected. It
    will be rejected if the logical channel in request doesnt belongs to the
    requesting client.

  PARAMETERS
    session_type   : session type
    aid            : AID value
    cl_sp          : Client state pointer

  RETURN VALUE
    Boolean indicating if the request is to be rejected or not.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_is_request_rejected
(
  mmgsdi_session_type_enum_type   session_type,
  mmgsdi_static_data_type         aid,
  qmi_uimi_client_state_type    * cl_sp
)
{
  uint16                    i               = 0;
  uint16                    channel_index   = UIM_INVALID_APP_INDEX;
  mmgsdi_slot_id_enum_type  slot            = MMGSDI_SLOT_1;

  ASSERT(qmi_uim_global_ptr);

  if (qmi_uim_global_ptr->apdu_sec_restr_value == QMI_UIM_APDU_SEC_RESTRICTIONS_NONE)
  {
    return FALSE;
  }

  /* Validate session type & channel id length */
  if ((session_type != MMGSDI_NON_PROV_SESSION_SLOT_1) &&
      (session_type != MMGSDI_NON_PROV_SESSION_SLOT_2) &&
      (session_type != MMGSDI_NON_PROV_SESSION_SLOT_3))
  {
    return FALSE;
  }

  if (aid.data_len != QMI_UIM_TLV_CHANNEL_ID_LEN)
  {
    return FALSE;
  }

  UIM_MSG_HIGH_1("logical_channel: 0x%x", aid.data_ptr[0]);

  /* Check if the logical channel belongs to requesting client */
  switch(session_type)
  {
    case MMGSDI_NON_PROV_SESSION_SLOT_1:
      slot = MMGSDI_SLOT_1;
      break;
    case MMGSDI_NON_PROV_SESSION_SLOT_2:
      slot = MMGSDI_SLOT_2;
      break;
    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      slot = MMGSDI_SLOT_3;
      break;
    default:
      return FALSE;
  }

  /* If any match is found, the request is OK to proceed */
  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->slot == slot &&
        qmi_uim_global_ptr->apdu_channel[i]->logical_channel == aid.data_ptr[0] &&
        qmi_uim_global_ptr->apdu_channel[i]->opening_cl_sp == cl_sp)
    {
      channel_index = i;
      break;
    }
  }
  if (channel_index == UIM_INVALID_APP_INDEX)
  {
    UIM_MSG_ERR_0("Request rejected due to security restrictions: logical channel doesnt belong to client");
    return TRUE;
  }

  return FALSE;
} /* qmi_uim_util_is_request_rejected */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_FREE_TLV_CONTENT()

  DESCRIPTION
    Free the data inside a TLV

  PARAMETERS
    tlv_data_ptr : pointer to TLV data
    count        : number of TLV items

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_util_free_tlv_content
(
  qmi_uim_tlv_item_type * tlv_data_ptr,
  uint16                  count
)
{
  uint16  i = 0;

  ASSERT(tlv_data_ptr);

  /* Free data in the TLV: only for write transparent and write
     record, we need to make a free */
  for (i = 0; i < count; i++)
  {
    if (!tlv_data_ptr[i].present)
    {
      /* TLV is not present... nothing to free */
      continue;
    }

    switch(tlv_data_ptr[i].type)
    {
      case UIMI_TLV_TYPE_WRITE_TRANSPARENT:
        if (tlv_data_ptr[i].data.write_transparent.data.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.write_transparent.data.data_ptr);
          tlv_data_ptr[i].data.write_transparent.data.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_WRITE_RECORD:
        if (tlv_data_ptr[i].data.write_record.data.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.write_record.data.data_ptr);
          tlv_data_ptr[i].data.write_record.data.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_REGISTER_REFRESH:
        while (tlv_data_ptr[i].data.register_refresh.file_list_ptr != NULL)
        {
          qmi_uim_files_list_type * current_ptr = tlv_data_ptr[i].data.register_refresh.file_list_ptr;
          tlv_data_ptr[i].data.register_refresh.file_list_ptr = current_ptr->next_ptr;
          uimqmi_free(current_ptr);
        }
        break;
      case UIMI_TLV_TYPE_AUTHENTICATE:
        if (tlv_data_ptr[i].data.authenticate.auth_data.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.authenticate.auth_data.data_ptr);
          tlv_data_ptr[i].data.authenticate.auth_data.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_APDU_REQUEST:
        if (tlv_data_ptr[i].data.send_apdu.apdu_data.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.send_apdu.apdu_data.data_ptr);
          tlv_data_ptr[i].data.send_apdu.apdu_data.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_INCREASE:
        if (tlv_data_ptr[i].data.increase_data.data.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.increase_data.data.data_ptr);
          tlv_data_ptr[i].data.increase_data.data.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_GW_NW_PERSO_CODE:
      case UIMI_TLV_TYPE_GW_NS_PERSO_CODE:
      case UIMI_TLV_TYPE_GW_SP_PERSO_CODE:
      case UIMI_TLV_TYPE_GW_CP_PERSO_CODE:
      case UIMI_TLV_TYPE_GW_SIM_PERSO_CODE:
      case UIMI_TLV_TYPE_1X_NW_TYPE1_PERSO_CODE:
      case UIMI_TLV_TYPE_1X_NW_TYPE2_PERSO_CODE:
      case UIMI_TLV_TYPE_1X_RUIM_PERSO_CODE:
        if(tlv_data_ptr[i].data.personalization_code.code_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.personalization_code.code_ptr);
          tlv_data_ptr[i].data.personalization_code.code_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_DECRYPTED_PERSO_SECURE:
        if (tlv_data_ptr[i].data.decrypted_perso_secure.code_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.decrypted_perso_secure.code_ptr);
          tlv_data_ptr[i].data.decrypted_perso_secure.code_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_DECRYPTED_SIMLOCK_CONFIG:
        if (tlv_data_ptr[i].data.decrypted_simlock_config.operation_type.sfs_read_operation.data.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.decrypted_simlock_config.operation_type.sfs_read_operation.data.data_ptr);
          tlv_data_ptr[i].data.decrypted_simlock_config.operation_type.sfs_read_operation.data.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_SEARCH_PATTERN:
        if (tlv_data_ptr[i].data.search_pattern.pattern.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.search_pattern.pattern.data_ptr);
          tlv_data_ptr[i].data.search_pattern.pattern.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_NAF_ID:
        if(tlv_data_ptr[i].data.gba_naf_id.naf_id_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.gba_naf_id.naf_id_ptr);
          tlv_data_ptr[i].data.gba_naf_id.naf_id_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_REMOTE_UNLOCK_CONFIG:
        if(tlv_data_ptr[i].data.remote_unlock_config.config_data.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.remote_unlock_config.config_data.data_ptr);
          tlv_data_ptr[i].data.remote_unlock_config.config_data.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_BSF_INFO:
        if(tlv_data_ptr[i].data.gba_bsf_info.bsf_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.gba_bsf_info.bsf_ptr);
          tlv_data_ptr[i].data.gba_bsf_info.bsf_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_EUICC_DOWNLOAD_CODE:
      case UIMI_TLV_TYPE_EUICC_CONFIRMATION_CODE:
        if (tlv_data_ptr[i].data.euicc_download_code.code.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.euicc_download_code.code.data_ptr);
          tlv_data_ptr[i].data.euicc_download_code.code.data_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_TERMINAL_CAPABILITY:
        if (tlv_data_ptr[i].data.terminal_capability.tlv_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.terminal_capability.tlv_ptr);
          tlv_data_ptr[i].data.terminal_capability.tlv_ptr = NULL;
        }
        break;
      case UIMI_TLV_TYPE_EUICC_DEFAULT_ADDRESS:
        if (tlv_data_ptr[i].data.euicc_default_address.smdp_address.data_ptr)
        {
          uimqmi_free(tlv_data_ptr[i].data.euicc_default_address.smdp_address.data_ptr);
          tlv_data_ptr[i].data.euicc_default_address.smdp_address.data_ptr = NULL;
        }
        break;
      default:
        /* Nothing to free for all other TLVs */
        break;
    }
  }

  /* Zero the memory for security reasons: the TLV might contain
     PIN codes or other sensitive information */
  if (count > 0)
  {
    memset(tlv_data_ptr, 0, count * sizeof(qmi_uim_tlv_item_type));
  }
} /* qmi_uim_util_free_tlv_content */


/*===========================================================================
  FUNCTION QMI_UIM_GET_PROFILE_ID_FROM_INDEX()

  DESCRIPTION
    Converts index into QMI UIM Profile ID

  PARAMETERS
    profile_index     : Index of the profile

  RETURN VALUE
    profile_id

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
mmgsdi_profile_id_enum_type qmi_uim_get_profile_id_from_index
(
  uint8       profile_index
)
{
  switch (profile_index)
  {
    case 0:
      return MMGSDI_PROFILE_1;
    case 1:
      return MMGSDI_PROFILE_2;
    case 2:
      return MMGSDI_PROFILE_3;
    case 3:
      return MMGSDI_PROFILE_4;
    case 4:
      return MMGSDI_PROFILE_5;
    case 5:
      return MMGSDI_PROFILE_6;
    case 6:
      return MMGSDI_PROFILE_7;
    case 7:
      return MMGSDI_PROFILE_8;
    default:
      break;
  }

  return MMGSDI_PROFILE_NOT_APPLICABLE;
} /*  qmi_uim_get_profile_id_from_index */


/*===========================================================================

  FUNCTION QMI_UIMI_GENERIC_RESULT_RESP()

  DESCRIPTION
    Composes the response for delete profile request

  PARAMETERS
    status                      : status of LPA operation
    cb_userdata_ptr             : user_data to retrieve the cmd

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_generic_result_resp
(
  qmi_error_e_type                                  errval,
  qmi_uimi_userdata_type                          * cb_userdata_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  dsm_item_type *          response         = NULL;
  boolean                  free_userdata    = TRUE;

  if (cb_userdata_ptr == NULL)
  {
    return;
  }

  do
  {
    /* Retrieve parameters */
    cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

    /* First check if the requested client is still valid */
    if (FALSE == qmi_uim_util_check_clid(cb_userdata_ptr->clid,
                                         cb_userdata_ptr->client_reg_count))
    {
      break;
    }

    if (cmd_buf_p == NULL)
    {
      break;
    }

    QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

    /* ENABLE/DISBALE: REFRESH will be triggered. If client request
         any operation before REFRESH, already available cache can be used.
       ADD_PROFILE: get_profile_info will be done when installation_complete
         received.
       DELETE/UPDATE: Query get_profile_info as REFRESH is not mandatory.
       eUICC RESET: Query get_profile_info so that LPA retrievs pending
         notifications from card and send to server.
         The cached information will be cleared in get_profile_info response
         handling */
    if (errval == QMI_ERR_NONE)
    {
      /* Use the userdata of original request if needs to query profile info
         again from LPA internally after update nickname or delete profile or
         or eUICC MemoryReset */
      if (cb_userdata_ptr->request_id == UIMI_CMD_VAL_DELETE_PROFILE ||
          cb_userdata_ptr->request_id == UIMI_CMD_VAL_UPDATE_NICKNAME ||
          cb_userdata_ptr->request_id == UIMI_CMD_VAL_EUICC_MEMORY_RESET)
      {
        /* Need to clear the cached profile info as it is stale now.
           If any get sim profile is recieved then it will forced to 
           goto card and fetch fresh data */
        qmi_uim_clear_profile_info(cb_userdata_ptr->data.lpa.slot);
        if (qmi_uim_query_euicc_profile_info(cb_userdata_ptr) == QMI_ERR_NONE)
        {
          free_userdata = FALSE;
        }
      }
    }

    /* Update result TLV and Send response */
    uimqmi_put_result_send_response_ind(cmd_buf_p,
                                        response,
                                        errval,
                                        QMUX_SERVICE_UIM,
                                        FALSE,
                                        QMI_SVC_CLID_UNUSED,
                                        (uint16)UIMI_CMD_VAL_WIDTH);
  } while(0);

  if (free_userdata)
  {
    /* Free callback user data */
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    cb_userdata_ptr = NULL;
  }
}/* qmi_uimi_generic_result_resp */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_IS_SIMLOCK_DISPLAY_FOR_VALID_CARD_ENABLED()

  DESCRIPTION
    This function checks the simlock NV and slot policy to determine if
    the criteria is satisifed for simlock status update for valid card.

  PARAMETERS
    None

  RETURN VALUE
    qmi_uim_simlock_display_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_uim_simlock_display_type qmi_uim_util_is_simlock_display_for_valid_card_enabled
(
  void
)
{
  if(qmi_uim_global_ptr == NULL)
  {
    return QMI_UIM_SIMLOCK_DISPLAY_DEFAULT;
  }

  /* Criteria is statisfied if
     1) Simlock NV set as valid card on any slot for multisim target and slot policy set as valid card on any slot.
     2) Simlock NV set as full service on any slot for multisim target and slot policy set as full service on any slot. 
     3) Simlock NV set as valid card on all slot for multisim target and slot policy set as valid card on all slot. */
  if((qmi_uim_global_ptr->simlock_display_for_valid_card == QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ANY_SLOT_POLICY &&
      qmi_uim_global_ptr->simlock_slot_policy == SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_ANY_SLOT)
       ||
     (qmi_uim_global_ptr->simlock_display_for_valid_card ==QMI_UIM_SIMLOCK_DISPLAY_FOR_FULL_SERVICE_ON_ANY_SLOT_POLICY &&
      qmi_uim_global_ptr->simlock_slot_policy == SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_ANY_VALID_SLOT)
       ||
     (qmi_uim_global_ptr->simlock_display_for_valid_card == QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ALL_SLOT_POLICY &&
      qmi_uim_global_ptr->simlock_slot_policy == SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_ALL_SLOTS))
  {
    return qmi_uim_global_ptr->simlock_display_for_valid_card;
  }

  return QMI_UIM_SIMLOCK_DISPLAY_DEFAULT;
} /* qmi_uim_util_is_simlock_display_for_valid_card_enabled */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_SET_FILE_PATH_FROM_FILE_ID()

  DESCRIPTION
    This function sets the path for ICC and UICC files
    when path length provided is 0.

  PARAMETERS
    None

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_util_set_file_path_from_file_id
(
  mmgsdi_session_type_enum_type    session_type,
  qmi_uim_tlv_file_id_type       * file_id_ptr
)
{
  mmgsdi_session_id_type          session_id       = UIM_INVALID_SESSION_ID;
  mmgsdi_protocol_enum_type       card_protocol    = MMGSDI_MAX_PROTOCOL_ENUM;
  mmgsdi_slot_id_enum_type        slot             = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_static_data_type         aid              = { 0, };

  ASSERT(file_id_ptr);
  
  /* If path is already present, nothing to do */
  if (file_id_ptr->file_access.access_method == MMGSDI_BY_PATH_ACCESS && 
      file_id_ptr->file_access.file.path_type.path_len > 0)
  {
    return QMI_ERR_NONE;
  }

  /* Return error if the file id is not 0x6Fxx and 0x2Fxx */
  if((uint8) (file_id_ptr->file_id >> 8) != 0x6F && 
     (uint8) (file_id_ptr->file_id >> 8) != 0x2F)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* If the session is NON PROV session regardless if it UICC or ICC then
     return error. */
  if ((session_type == MMGSDI_NON_PROV_SESSION_SLOT_1 ||
       session_type == MMGSDI_NON_PROV_SESSION_SLOT_2 ||
       session_type == MMGSDI_NON_PROV_SESSION_SLOT_3))
  {
    return QMI_ERR_INVALID_ARG;
  }
  
  /* Retrieve session id to use it for getting the slot */
  if (qmi_uim_util_session_type_to_session_id(&session_id, session_type, aid) != QMI_ERR_NONE)
  {
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve the slot */
  if (qmi_uim_util_session_id_to_slot(&slot, session_id) != QMI_ERR_NONE)
  {
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve protocol */
  card_protocol = qmi_uim_util_card_protocol(slot);

  /* If the session type is CARD session regardless if it UICC or ICC card
     then set the path to 0x3F00. 
     If the protocol is UICC and GW, 1x sessions are enabled then
     set the path to 0x3F00->0x7FFF. 
     If the protocol is ICC and session type is GW, then set the path to 0x3F00->0x7F20.
     if the protocol is ICC and session type is 1x, then set the path to 0x3F00->0x7F25. 
     If the protocol is ICC and file id is matching one of the files in
     TS 51.011 clause 10.5, then set the path to 0x3F00->0x7F10. */

  if ((uint8)(file_id_ptr->file_id >> 8) == 0x2F)
  {
    if(session_type == MMGSDI_CARD_SESSION_SLOT_1 || 
       session_type == MMGSDI_CARD_SESSION_SLOT_2 ||
       session_type == MMGSDI_CARD_SESSION_SLOT_3)
    {
      file_id_ptr->file_access.file.path_type.path_len    = 1;
      file_id_ptr->file_access.file.path_type.path_buf[0] = 0x3F00;
    }
    else
    {
      return QMI_ERR_INVALID_ARG;
    }
  }
  else if ((session_type == MMGSDI_GW_PROV_PRI_SESSION ||
            session_type == MMGSDI_GW_PROV_SEC_SESSION ||
            session_type == MMGSDI_GW_PROV_TER_SESSION ||
            session_type == MMGSDI_1X_PROV_PRI_SESSION ||
            session_type == MMGSDI_1X_PROV_SEC_SESSION ||
            session_type == MMGSDI_1X_PROV_TER_SESSION) && 
            card_protocol == MMGSDI_UICC)
  {
    file_id_ptr->file_access.file.path_type.path_len    = 2;
    file_id_ptr->file_access.file.path_type.path_buf[0] = 0x3F00;
    file_id_ptr->file_access.file.path_type.path_buf[1] = 0x7FFF;
  }
  else if (file_id_ptr->file_id == 0x6F3A || file_id_ptr->file_id == 0x6F3B ||
           file_id_ptr->file_id == 0x6F3C || file_id_ptr->file_id == 0x6F3D ||
           file_id_ptr->file_id == 0x6F40 || file_id_ptr->file_id == 0x6F42 ||
           file_id_ptr->file_id == 0x6F43 || file_id_ptr->file_id == 0x6F44 ||
           file_id_ptr->file_id == 0x6F49 || file_id_ptr->file_id == 0x6F4A ||
           file_id_ptr->file_id == 0x6F4B || file_id_ptr->file_id == 0x6F4C ||
           file_id_ptr->file_id == 0x6F4D || file_id_ptr->file_id == 0x6F4E ||
           file_id_ptr->file_id == 0x6F47 || file_id_ptr->file_id == 0x6F58 ||
           file_id_ptr->file_id == 0x6F4F || file_id_ptr->file_id == 0x6F54)
  {
    file_id_ptr->file_access.file.path_type.path_len    = 2;
    file_id_ptr->file_access.file.path_type.path_buf[0] = 0x3F00;
    file_id_ptr->file_access.file.path_type.path_buf[1] = 0x7F10;
  }
  else if ((session_type == MMGSDI_GW_PROV_PRI_SESSION ||
            session_type == MMGSDI_GW_PROV_SEC_SESSION ||
            session_type == MMGSDI_GW_PROV_TER_SESSION) && 
            card_protocol == MMGSDI_ICC)
  {
    file_id_ptr->file_access.file.path_type.path_len    = 2;
    file_id_ptr->file_access.file.path_type.path_buf[0] = 0x3F00;
    file_id_ptr->file_access.file.path_type.path_buf[1] = 0x7F20;
  }
  else if ((session_type == MMGSDI_1X_PROV_PRI_SESSION ||
            session_type == MMGSDI_1X_PROV_SEC_SESSION ||
            session_type == MMGSDI_1X_PROV_TER_SESSION) && 
            card_protocol == MMGSDI_ICC)
  {
    file_id_ptr->file_access.file.path_type.path_len    = 2;
    file_id_ptr->file_access.file.path_type.path_buf[0] = 0x3F00;
    file_id_ptr->file_access.file.path_type.path_buf[1] = 0x7F25;
  }
  else
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Set the access method */
  file_id_ptr->file_access.access_method = MMGSDI_BY_PATH_ACCESS;
  
  /* Add file id to the path */
  file_id_ptr->file_access.file.path_type.path_buf[file_id_ptr->file_access.file.path_type.path_len] = file_id_ptr->file_id;
  file_id_ptr->file_access.file.path_type.path_len += 1;

  return QMI_ERR_NONE;
} /*qmi_uim_util_set_file_path_from_file_id */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_GET_NUM_OF_MNC_DIGITS()

  DESCRIPTION
    This function reads the ad data from the cache and gets the number
    of the MNC digits.

  PARAMETERS
    None

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_get_num_of_mnc_digits
(
  mmgsdi_session_id_type            session_id,
  uint8                           * num_mnc_digits_ptr
)
{
  mmgsdi_read_data_type           read_data        = {0, NULL};
  uint8                           ad_data[4]       = {0};
  mmgsdi_file_enum_type           file_name        = MMGSDI_MAX_FILE_ENUM;
  mmgsdi_session_type_enum_type   session_type     = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_app_enum_type            app_type         = MMGSDI_APP_NONE;
  mmgsdi_return_enum_type         mmgsdi_status    = MMGSDI_ERROR;
  qmi_error_e_type                result           = QMI_ERR_INTERNAL;
  mmgsdi_static_data_type         aid              = { 0, };
  
  if (num_mnc_digits_ptr == NULL)
  {
    return FALSE;
  }
 
  /* Get the session type in order to get the app_type for the AD file read */
  result = qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id);
  if(result != QMI_ERR_NONE)
  {
    return FALSE;
  }

  /* If session type is not GW then return error */
  if (session_type != MMGSDI_GW_PROV_PRI_SESSION &&
      session_type != MMGSDI_GW_PROV_SEC_SESSION &&
      session_type != MMGSDI_GW_PROV_TER_SESSION)
  {
    return FALSE;
  }

  /* Use the session type to get the app_type for the AD file read */
  result = qmi_uim_util_session_type_to_card_type(&app_type, session_type, aid);
  if(result != QMI_ERR_NONE)
  {
    return FALSE;
  }

  switch(app_type)
  {
    case MMGSDI_APP_SIM:
      file_name = MMGSDI_GSM_AD;
      break;
    case MMGSDI_APP_USIM:
      file_name = MMGSDI_USIM_AD;
      break;
    default:
      return FALSE;
  }

  /* Read USIM AD to get the number of digits in MNC */
  read_data.data_ptr = ad_data;
  read_data.data_len = sizeof(ad_data);

  mmgsdi_status = mmgsdi_session_read_cache(session_id,
                                            file_name,
                                            read_data);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return FALSE;
  }

  /* Old SIM cards had only 2 digits MNC. */
  *num_mnc_digits_ptr = QMI_UIM_MNC_2_DIGITS;
 
  /* Check the MNC length in 4th byte of EF AD */
  if (read_data.data_len > QMI_UIM_MNC_LEN_OFFSET_IN_AD)
  {
    if (((ad_data[QMI_UIM_MNC_LEN_OFFSET_IN_AD] & 0x0F) == QMI_UIM_MNC_2_DIGITS ) ||
        ((ad_data[QMI_UIM_MNC_LEN_OFFSET_IN_AD] & 0x0F) == QMI_UIM_MNC_3_DIGITS ))
    {
      *num_mnc_digits_ptr = ad_data[QMI_UIM_MNC_LEN_OFFSET_IN_AD] & 0x0F;
      return TRUE;
    }
  }
  return FALSE;
} /* qmi_uim_util_get_num_of_mnc_digits */


/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_IMSI_TO_DIGITS()

  DESCRIPTION
    Convert IMSI from qmi_uim_imsi_type to qmi_uim_imsi_digits_data_type

  PARAMETERS
    imsi_data_ptr : value of IMSI in qmi_uim_imsi_type format
    imsi_digit_ptr: value of IMSI in qmi_uim_imsi_digits_data_type format

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_util_convert_imsi_to_digits
(
  const qmi_uim_imsi_type        * imsi_data_ptr,
  qmi_uim_imsi_digits_data_type  * imsi_digit_ptr
)
{
  uint8    i = 0;

  if(imsi_data_ptr == NULL ||
     imsi_digit_ptr == NULL ||
     ((imsi_data_ptr->imsi[0] + 1) != imsi_data_ptr->len) ||
     ((imsi_data_ptr->imsi[0] * 2) - 1 > sizeof(imsi_digit_ptr->imsi_digits)))
  {
    return FALSE;
  }

  imsi_digit_ptr->imsi_digits_len = 0;
  memset(&imsi_digit_ptr->imsi_digits[0], 0xF, sizeof(imsi_digit_ptr->imsi_digits));

  /* -----------------------------------------------------------
    Parse MCC from the IMSI:
    B0     B1    B2   B3   B4    B5    B6     B7      B8
    LEN  D1PB   D3D2 D5D4 D7D6  D9D8 D11D10 D13D12  D15D14
  -------------------------------------------------------------*/
  for(i = 0; i < imsi_data_ptr->imsi[0] - 1; i++)
  {
    imsi_digit_ptr->imsi_digits[i * 2] = imsi_data_ptr->imsi[i + 1] & QMI_UIM_IMSI_HIGH_BYTE_MASK;
    imsi_digit_ptr->imsi_digits[i * 2] >>= 4;
    imsi_digit_ptr->imsi_digits_len++;
    imsi_digit_ptr->imsi_digits[i * 2 + 1] = imsi_data_ptr->imsi[i + 2] & QMI_UIM_IMSI_LOW_BYTE_MASK;
    imsi_digit_ptr->imsi_digits_len++;
  }

  /* Check if last digit of IMSI is valid */
  if((imsi_data_ptr->imsi[imsi_data_ptr->imsi[0]] & QMI_UIM_IMSI_HIGH_BYTE_MASK) !=
       QMI_UIM_IMSI_HIGH_BYTE_MASK)
  {
    imsi_digit_ptr->imsi_digits[i * 2] =
      imsi_data_ptr->imsi[imsi_data_ptr->imsi[0]] & QMI_UIM_IMSI_HIGH_BYTE_MASK;
    imsi_digit_ptr->imsi_digits[i * 2] >>= 4;
    imsi_digit_ptr->imsi_digits_len++;
  }

  /* Convert the IMSI into digits format */
  for(i = 0; i < imsi_digit_ptr->imsi_digits_len; i++)
  {
    imsi_digit_ptr->imsi_digits[i] += '0';
  }

  return TRUE;
} /* qmi_uim_util_convert_imsi_to_digits */


/*===========================================================================
FUNCTION QMI_UIM_UTIL_IS_SESSION_PROVISIONING

DESCRIPTION
  This function indictaes whether the session is provisioning or not

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
boolean qmi_uim_util_is_session_provisioning
(
  mmgsdi_session_type_enum_type  session_type
)
{
  switch ( session_type )
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
    case MMGSDI_1X_PROV_PRI_SESSION:
    case MMGSDI_GW_PROV_SEC_SESSION:
    case MMGSDI_1X_PROV_SEC_SESSION:
    case MMGSDI_GW_PROV_TER_SESSION:
    case MMGSDI_1X_PROV_TER_SESSION:
      return TRUE;
    default:
      break;
  }

  return FALSE;
}/* qmi_uim_util_is_session_provisioning */