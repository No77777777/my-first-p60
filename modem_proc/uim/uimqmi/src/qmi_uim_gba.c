/*===========================================================================

                         Q M I _ U I M _ G B A. C

DESCRIPTION
  This file contains functions for QMI UIM service's GBA functionality.

Copyright (c) 2016 - 2017, 2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_gba.c#1 $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
03/25/19    vgd    Free uim_msg_ptr only if it is not present in the queue
01/23/17    me     Moved qmi_uim_gba_convert_imsi_to_digits to qmi_uim_util 
11/04/16    me     Combined multi-line ASSERT in one line
09/15/16    tkl    Update GBA / QMI error code mapping
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
05/26/16    vdc    Move internal enums, structs to new internal header file
05/11/16    bcho   qmi_uim_gba_read_impi_resp() moved to gba specific file
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "qmi_uim_gba.h"
#include "mmgsdilib_common.h"
#include "uim_msg.h"
#include "qmi_uim_util.h"
#include "amssassert.h"
#include "qmi_uim_encryption.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim.h"
#include "mmgsdisessionlib_v.h"
#include <stringl/stringl.h>
#include "fs_public.h"

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

  /*---------------------------------------------------------------------------
    IMPI NAI tag
  ---------------------------------------------------------------------------*/
#define QMI_UIM_IMPI_NAI_TAG                   (0x80)

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_GBA_CONVERT_MMGSDI_SESSION_TO_GBA_SESSION

  DESCRIPTION
    Convert MMGSDI session type to GBA session type.

  PARAMETERS
    mmgsdi_session_type  : MMGSDI session type
    gba_session_type_ptr : GBA session type

  RETURN VALUE
    errval               : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_gba_convert_mmgsdi_session_to_gba_session
(
  mmgsdi_session_type_enum_type   mmgsdi_session_type,
  gba_session_type               *gba_session_type_ptr
)
{
  if(gba_session_type_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  switch(mmgsdi_session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      *gba_session_type_ptr = GBA_3GPP_PROV_SESSION_PRI;
      break;

    case MMGSDI_GW_PROV_SEC_SESSION:
      *gba_session_type_ptr = GBA_3GPP_PROV_SESSION_SEC;
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_1:
      *gba_session_type_ptr = GBA_NON_PROV_SESSION_SLOT_1;
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_2:
      *gba_session_type_ptr = GBA_NON_PROV_SESSION_SLOT_2;
      break;

    default:
      UIM_MSG_ERR_1("session type not supported: 0x%x", mmgsdi_session_type);
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_gba_convert_mmgsdi_session_to_gba_session */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_CONVERT_GBA_SESSION_TO_MMGSDI_SESSION

  DESCRIPTION
    Convert GBA session type to MMGSDI session type.

  PARAMETERS
    gba_session_type         : GBA session type
    mmgsdi_session_type_ptr  : MMGSDI session type

  RETURN VALUE
    errval               : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_gba_convert_gba_session_to_mmgsdi_session
(
  gba_session_type                 gba_session_type,
  mmgsdi_session_type_enum_type   *mmgsdi_session_type_ptr
)
{
  if(mmgsdi_session_type_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  switch(gba_session_type)
  {
    case GBA_3GPP_PROV_SESSION_PRI:
      *mmgsdi_session_type_ptr = MMGSDI_GW_PROV_PRI_SESSION;
      break;

    case GBA_3GPP_PROV_SESSION_SEC:
      *mmgsdi_session_type_ptr = MMGSDI_GW_PROV_SEC_SESSION;
      break;

    case GBA_NON_PROV_SESSION_SLOT_1:
      *mmgsdi_session_type_ptr = MMGSDI_NON_PROV_SESSION_SLOT_1;
      break;

    case GBA_NON_PROV_SESSION_SLOT_2:
      *mmgsdi_session_type_ptr = MMGSDI_NON_PROV_SESSION_SLOT_2;
      break;

    default:
      UIM_MSG_ERR_1("session type not supported: 0x%x", gba_session_type);
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_gba_convert_gba_session_to_mmgsdi_session */



/*===========================================================================
  FUNCTION QMI_UIM_GBA_CONVERT_GBA_STATUS_TO_ERRVAL

  DESCRIPTION
    To obtain QMI client-specific error message from GBA error message

  PARAMETERS
    gba_status   : GBA return value

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_gba_convert_gba_status_to_errval
(
  gba_result_enum_type gba_status
)
{
  qmi_error_e_type errval  = QMI_ERR_INTERNAL;

  UIM_MSG_HIGH_1("Received gba_status 0x%x", gba_status);

  switch(gba_status)
  {
    case GBA_SUCCESS:
      errval = QMI_ERR_NONE;
      break;

    case GBA_GENERIC_ERROR:
    case GBA_SIM_ERROR:
      errval = QMI_ERR_INTERNAL;
      break;

    case GBA_INCORRECT_PARAMS:
      errval = QMI_ERR_INVALID_ARG;
      break;

    case GBA_TIMEOUT:
      errval = QMI_ERR_ABORTED;
      break;

    case GBA_AUTH_FAILURE:
    case GBA_SERVER_ERROR:
    case GBA_NETWORK_ERROR:
    case GBA_NETWORK_NOT_READY:
      errval = QMI_ERR_AUTHENTICATION_FAILED;
      break;

    case GBA_MEMORY_ERROR_HEAP_EXHAUSTED:
      errval = QMI_ERR_NO_MEMORY;
      break;

    case GBA_SIM_NOT_READY:
      errval = QMI_ERR_DEVICE_NOT_READY;
      break;

    case GBA_UNSUPPORTED:
      errval = QMI_ERR_NOT_SUPPORTED;
      break;

    default:
      errval = QMI_ERR_INTERNAL;
      break;
  }

  return errval;
} /* qmi_uim_gba_convert_gba_status_to_errval() */


/*===========================================================================
  FUNCTION QMI_UIMI_GBA_CALLBACK()

  DESCRIPTION
    This function is executed as callback of GBA API. This function makes a
    deep copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_gba_callback
(
  gba_result_enum_type          status,
  uint32                        request_id,
  const gba_response_data_type *cnf_ptr,
  const void                   *user_data_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  (void)request_id;

  /* Do not remove log print */
  UIM_MSG_MED_1("Got response from GBA with status: 0x%x", status);

  if(status == GBA_SUCCESS && cnf_ptr == NULL)
  {
    UIM_MSG_ERR_0("NULL response from GBA module");
    return;
  }

  /* Copy content into a buffer and post a message to QMI task */
  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_GBA_CB);
  if (msg_ptr != NULL)
  {
    msg_ptr->data.gba_cnf.status    = status;

    if(cnf_ptr != NULL)
    {
      (void)memscpy(&msg_ptr->data.gba_cnf.cnf_value,
                    sizeof(msg_ptr->data.gba_cnf.cnf_value),
                    cnf_ptr,
                    sizeof(gba_response_data_type));
    }
    msg_ptr->data.gba_cnf.user_data_ptr = user_data_ptr;

    /* Send command */
    if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
       (present_in_queue))
    {
      uimqmi_free(msg_ptr);
    }
    msg_ptr = NULL;
  }
} /* qmi_uimi_gba_callback */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_EVENT_CALLBACK()

  DESCRIPTION
    This function is executed as event callback from GBA. This function makes
    a deep copy of the event data and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_gba_event_callback
(
  const gba_event_data_type         * event_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(event_ptr);

  UIM_MSG_HIGH_2("Received event callback from GBA for event 0x%x and session 0x%x",
                 event_ptr->evt, event_ptr->session_type);

  switch(event_ptr->evt)
  {
    /* Post events to the QMI context. */
    case GBA_KEY_EXPIRED_EVT:
    case GBA_KEY_REPLACED_EVT:
    case GBA_KEY_DISCARDED_EVT:
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_GBA_EVENT);
      if (msg_ptr == NULL)
      {
        return;
      }

      msg_ptr->data.gba_evt.evt.evt = event_ptr->evt;
      msg_ptr->data.gba_evt.evt.session_type = event_ptr->session_type;
      msg_ptr->data.gba_evt.evt.user_data = event_ptr->user_data;

      uimqmi_deep_copy((void **)&msg_ptr->data.gba_evt.evt.aid_ptr,
                       (void *)event_ptr->aid_ptr,
                       sizeof(gba_aid_type));

      uimqmi_deep_copy((void **)&msg_ptr->data.gba_evt.evt.btid_ptr,
                       (void *)event_ptr->btid_ptr,
                       sizeof(gba_btid_type));

      /* Send command */
      if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
         (present_in_queue))
      {
        if(msg_ptr->data.gba_evt.evt.aid_ptr != NULL)
        {
          uimqmi_free(msg_ptr->data.gba_evt.evt.aid_ptr);
        }
        if(msg_ptr->data.gba_evt.evt.btid_ptr != NULL)
        {
          uimqmi_free(msg_ptr->data.gba_evt.evt.btid_ptr);
        }
        uimqmi_free(msg_ptr);
      }
      msg_ptr = NULL;
      break;

    default:
      UIM_MSG_MED_1("Unhandled event: 0x%x", event_ptr->evt);
      break;
  }
} /* qmi_uimi_gba_event_callback */


/*===========================================================================
  FUNCTION QMI_UIM_EFS_READ_FILE

  DESCRIPTION
    Generic function called by QMI UIM to read a file from EFS. This function
    will validate the input parameters and return the EFS data requested.

  PARAMETERS
    file_name_ptr: Name of the file that needs to be read.
    data_len_ptr : Data length of the read file.

  DEPENDENCIES
    None

  RETURN VALUE
    pointer of the buffer where the EFS data is read. Otherwise, return  NULL
    if any failure in opening or reading the file from EFS.

  SIDE EFFECTS
    None
===========================================================================*/
static char * qmi_uim_efs_read_file
(
  const char  * file_name_ptr,
  uint32      * data_len_ptr
)
{
  struct fs_stat   file_stat       = {0};
  int              file_descriptor = 0;
  char           * file_buffer_ptr = NULL;

  if(file_name_ptr == NULL || data_len_ptr == NULL)
  {
    return NULL;
  }

  if(efs_stat(file_name_ptr, &file_stat) != 0 || file_stat.st_size == 0)
  {
    UIM_MSG_LOW_0("efs_stat procedure failed");
    return NULL;
  }

  /* Allocate memory for file buffer */
  file_buffer_ptr = (char *)uimqmi_malloc(file_stat.st_size);
  if(file_buffer_ptr == NULL)
  {
    return NULL;
  }

  /* Open the file in EFS for Read Only */
  file_descriptor = efs_open(file_name_ptr, O_RDONLY);

  if(file_descriptor < 0)
  {
    UIM_MSG_LOW_0("efs_open procedure failed ");
    uimqmi_free(file_buffer_ptr);
    return NULL;
  }

  /* Read the file into buffer provided */
  if (efs_read(file_descriptor, file_buffer_ptr, file_stat.st_size) !=
       file_stat.st_size)
  {
    UIM_MSG_LOW_0("efs_read procedure failed ");
    uimqmi_free(file_buffer_ptr);
    (void)efs_close(file_descriptor);
    return NULL;
  }

  *data_len_ptr = (uint32)file_stat.st_size;

  /* Close the file */
  (void)efs_close(file_descriptor);

  return file_buffer_ptr;
}  /* qmi_uim_efs_read_file */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_ENCRYPT_KS_NAF_REQUIRED()

  DESCRIPTION
    Reads the FQDN entries from the white / black list in the EFS. If white
    list is not available then we fallback to read FQDN entries in the black
    list. This list (white / black) of FQDN entries are checked against the
    input FQDN data and if it matches with any of the entry then encryption of
    ks_naf is done in the response.

  PARAMETERS
    req_fqdn_data_ptr: FQDN data pointer

  RETURN VALUE
    Boolean that indicates whether encryption is required for ks_naf
      TRUE: Encryption is required for ks_naf.
      FALSE: No encryption is done.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_gba_encrypt_ks_naf_required
(
  gba_naf_fqdn_type  * req_fqdn_data_ptr
)
{
  uint16   fqdn_buf_index                  = 0;
  char   * fqdn_list_file_data_ptr         = NULL;
  uint32   efs_data_len                    = 0;
  boolean  result                          = TRUE;
  boolean  is_white_list                   = TRUE;

  if(req_fqdn_data_ptr == NULL)
  {
    return FALSE;
  }

  /* Read the FQDN list file as follows:
     1) Check if white list FQDN file is present in the EFS.
     2) If the white list FQDN file is not present then try to read the black
        list FQDN from the EFS.
     3) If the black list FQDN read also fails then return FALSE as no FQDN
        list is available & encryption of ks_naf is not required */
  fqdn_list_file_data_ptr = qmi_uim_efs_read_file(QMI_UIM_CONFIG_GBA_FQDN_WHITE_LIST,
                                                  &efs_data_len);
  if(fqdn_list_file_data_ptr == NULL)
  {
    UIM_MSG_MED_0("White list FQDN is not available. Falling back to Black list FQDN");

    fqdn_list_file_data_ptr = qmi_uim_efs_read_file(QMI_UIM_CONFIG_GBA_FQDN_BLACK_LIST,
                                                    &efs_data_len);
    if(fqdn_list_file_data_ptr == NULL)
    {
      UIM_MSG_HIGH_0("No FQDN list is available");
      return FALSE;
    }

    /* As white list is not available and we are successful in reading black
       list we make is_white_list to FALSE  & result re-initialized to FALSE.
       If FQDN entry does not match with any FQDN in black list then we return
       this result (FALSE) and we don't encrypt ks_naf */
    is_white_list = FALSE;
    result = FALSE;
  }

  /* Convert the requested FQDN to lower case */
  for (fqdn_buf_index = 0; fqdn_buf_index < req_fqdn_data_ptr->data_len; fqdn_buf_index++)
  {
    req_fqdn_data_ptr->data[fqdn_buf_index] =
      QMI_UIM_LOWER_CASE(req_fqdn_data_ptr->data[fqdn_buf_index]);
  }

  /* Convert the FQDN buffer to lower case */
  for(fqdn_buf_index = 0; fqdn_buf_index < efs_data_len; fqdn_buf_index++)
  {
    fqdn_list_file_data_ptr[fqdn_buf_index] =
      QMI_UIM_LOWER_CASE(fqdn_list_file_data_ptr[fqdn_buf_index]);
  }

  /* Parse the FQDN data */
  fqdn_buf_index = 0;
  while(fqdn_buf_index < efs_data_len)
  {
    uint8   fqdn_start_index = fqdn_buf_index;
    uint8   fqdn_end_index   = fqdn_buf_index;
    uint16  fqdn_len         = 0;

    while(fqdn_end_index < efs_data_len &&
          fqdn_list_file_data_ptr[fqdn_end_index] != ';')
    {
      fqdn_end_index++;
    }

    /* If there is no FQDN entry between two delimiters(;;) then we continue
       to next FQDN entry */
    if(fqdn_end_index == fqdn_buf_index)
    {
      fqdn_buf_index++;
      continue;
    }

    /* update the fqdn_buf_index to point to next FQDN entry in the EFS buffer */
    fqdn_buf_index = fqdn_end_index + 1;

    /* Update the fqdn_end_index to last character of the FQDN entry */
    fqdn_end_index--;

    /* Skip leading spaces if any */
    while(fqdn_list_file_data_ptr[fqdn_start_index] == ' ' &&
          fqdn_start_index < fqdn_end_index)
    {
      fqdn_start_index++;
    }

    /* Skip trailing spaces if any */
    while(fqdn_list_file_data_ptr[fqdn_end_index] == ' ' &&
          fqdn_end_index > fqdn_start_index)
    {
      fqdn_end_index--;
    }

    /* When there are only spaces in FQDN entry then goto next FQDN entry*/
    if(fqdn_start_index == fqdn_end_index &&
       fqdn_list_file_data_ptr[fqdn_start_index] == ' ')
    {
      continue;
    }

    fqdn_len = fqdn_end_index - fqdn_start_index + 1;

    if(req_fqdn_data_ptr->data_len == fqdn_len &&
       memcmp(req_fqdn_data_ptr->data, &fqdn_list_file_data_ptr[fqdn_start_index], fqdn_len) == 0)
    {
      /* FQDN has matched. If the list is white list then return FALSE and
         if it is black list then send TRUE to send encrypted ks_naf */
      if(is_white_list)
      {
        result = FALSE;
      }
      else
      {
        result = TRUE;
      }
      break;
    }
  }

  uimqmi_free(fqdn_list_file_data_ptr);

  return result;
} /* qmi_uim_gba_encrypt_ks_naf_required */


/*===========================================================================
  FUNCTION  QMI_UIM_GBA_PROCESS_GBA_CB()

  DESCRIPTION
    Generic QMI processing for external commands

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
void qmi_uim_gba_process_gba_cb
(
  qmi_uim_message_type * uim_message_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  qmi_uimi_cmd_val_e_type  request_id       = UIMI_CMD_VAL_WIDTH;
  dsm_item_type *          response         = NULL;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint32                   token            = 0;
  gba_naf_fqdn_type        fqdn_data        = {0};
  uint32                   evt_reg_mask     = 0;

  UIM_MSG_HIGH_0("Sending response for GBA");

  ASSERT( uim_message_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)uim_message_ptr->data.gba_cnf.user_data_ptr;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  request_id       = cb_userdata_ptr->request_id;
  ind_required     = cb_userdata_ptr->ind_token.is_valid;
  token            = cb_userdata_ptr->ind_token.token;
  fqdn_data        = cb_userdata_ptr->data.gba.fqdn_data;
  evt_reg_mask     = cb_userdata_ptr->data.gba.evt_reg_mask;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  errval = qmi_uim_gba_convert_gba_status_to_errval(uim_message_ptr->data.gba_cnf.status);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      dsm_free_packet(&response);
      return;
    }
  }

  if(uim_message_ptr->data.gba_cnf.status == GBA_SUCCESS)
  {
    mmgsdi_data_type             gba_resp_data = {0, NULL};
    qmi_uimi_client_state_type * cl_sp         = NULL;

    /* Update the event reg mask for this client as the GBA req succeeded */
    cl_sp = qmi_uim_state_ptr->client[clid - 1];
    if (cl_sp != NULL)
    {
      cl_sp->uim_info.gba_evt_reg_mask |= evt_reg_mask;
    }

    /* If NAF id is empty then do not send Ks_NAF TLV in the response */
    if(fqdn_data.data_len > 0)
    {
      /* Check whether ks_naf should be encrypted */
      if(qmi_uim_gba_encrypt_ks_naf_required(&fqdn_data))
      {
        mmgsdi_data_type     naf_enc_data        = {0, NULL};
        mmgsdi_data_type     ks_naf_data         = {0, NULL};

        ks_naf_data.data_ptr = uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.ks_naf;
        ks_naf_data.data_len = sizeof(uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.ks_naf);

        if(qmi_uim_encrypt_payload(ks_naf_data, &naf_enc_data))
        {
          if(FALSE == qmi_uim_response_generic_payload_16bit(
                       UIMI_TLV_RESP_TAG_OPTIONAL_2,
                       &response,
                       naf_enc_data,
                       TRUE))
          {
            dsm_free_packet(&response);
            errval = QMI_ERR_NO_MEMORY;
          }
        }
        else
        {
          errval = QMI_ERR_INTERNAL;
        }

        /* Free the naf encrypted data if it is allocated */
        if(naf_enc_data.data_ptr != NULL)
        {
          uimqmi_free(naf_enc_data.data_ptr);
          naf_enc_data.data_ptr = NULL;
        }
      }
      else
      {
        if(FALSE == qmi_svc_put_param_tlv(
                     &response,
                     UIMI_TLV_RESP_TAG_OPTIONAL_1,
                     sizeof(uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.ks_naf),
                     (void *)uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.ks_naf))
        {
          errval = QMI_ERR_NO_MEMORY;
          dsm_free_packet(&response);
        }
      }
    }

    if(errval == QMI_ERR_NONE)
    {
      gba_resp_data.data_len = uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.btid.data_len;
      gba_resp_data.data_ptr = (uint8 *)uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.btid.data;

      if (FALSE == qmi_uim_response_generic_payload_8bit(
                              UIMI_TLV_RESP_TAG_OPTIONAL_3,
                              &response,
                              gba_resp_data,
                              FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }

    if(errval == QMI_ERR_NONE)
    {
      gba_resp_data.data_len =  uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.lifetime.data_len;
      gba_resp_data.data_ptr = (uint8 *)uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.lifetime.data;

      if(FALSE == qmi_uim_response_generic_payload_8bit(
                              UIMI_TLV_RESP_TAG_OPTIONAL_4,
                              &response,
                              gba_resp_data,
                              FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }

    if(errval == QMI_ERR_NONE)
    {
      gba_resp_data.data_len = uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.impi.data_len;
      gba_resp_data.data_ptr = (uint8 *)uim_message_ptr->data.gba_cnf.cnf_value.resp_data.success.impi.data;

      if(FALSE == qmi_uim_response_generic_payload_8bit(
                               UIMI_TLV_RESP_TAG_OPTIONAL_5,
                               &response,
                               gba_resp_data,
                               FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
  }

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)request_id);
} /* qmi_uim_gba_process_gba_cb */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_RESPONSE_GBA_EVENT()

  DESCRIPTION
    Composes the TLV content for GBA event type

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    evt_type         : GBA event type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_gba_response_gba_event
(
  byte                            tlv_id,
  dsm_item_type                ** response_pptr,
  gba_key_event_enum_type         evt_type
)
{
  PACKED struct PACKED_POST
  {
    uint32 evt;
  } tlv_data;

  ASSERT(response_pptr);

  switch(evt_type)
  {
    case GBA_KEY_EXPIRED_EVT:
      tlv_data.evt = UIM_GBA_KEY_EXPIRED_V01;
      break;
    case GBA_KEY_REPLACED_EVT:
      tlv_data.evt = UIM_GBA_KEY_REPLACED_V01;
      break;
    case GBA_KEY_DISCARDED_EVT:
      tlv_data.evt = UIM_GBA_KEY_DISCARDED_V01;
      break;
    default:
      UIM_MSG_ERR_1("Invalid evt type: 0x%x", evt_type);
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_gba_response_gba_event */


/*===========================================================================
  FUNCTION  QMI_UIM_GBA_SEND_GBA_KEY_STATUS_IND()

  DESCRIPTION
    This function sends the GBA Key Status indication

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
static void qmi_uim_gba_send_gba_key_status_ind
(
  uint8                       clid,
  const gba_event_data_type * gba_event_ptr
)
{
  dsm_item_type                 * indication     = NULL;
  mmgsdi_session_type_enum_type   mmgsdi_session = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_data_type                gba_btid_data  = {0};

  ASSERT(gba_event_ptr);

  if (qmi_uim_gba_convert_gba_session_to_mmgsdi_session(gba_event_ptr->session_type,
                                                        &mmgsdi_session) != QMI_ERR_NONE)
  {
    return;
  }

  if (FALSE == qmi_uim_response_session_information(
                   UIMI_TLV_IND_TAG_MANDATORY_1,
                   &indication,
                   mmgsdi_session,
                   gba_event_ptr->aid_ptr->data_len,
                   gba_event_ptr->aid_ptr->data))
  {
    dsm_free_packet(&indication);
    return;
  }

  if (FALSE == qmi_uim_gba_response_gba_event(
                   UIMI_TLV_IND_TAG_MANDATORY_2,
                   &indication,
                   gba_event_ptr->evt))
  {
    dsm_free_packet(&indication);
    return;
  }

  gba_btid_data.data_len = gba_event_ptr->btid_ptr->data_len;
  gba_btid_data.data_ptr = (uint8 *)gba_event_ptr->btid_ptr->data;
  if (FALSE == qmi_uim_response_generic_payload_8bit(
                  UIMI_TLV_IND_TAG_MANDATORY_3,
                  &indication,
                  gba_btid_data,
                  TRUE))
  {
    dsm_free_packet(&indication);
    return;
  }

  (void)uimqmi_send_indication(clid,
                               UIMI_CMD_VAL_GBA_KEY_STATUS_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_gba_key_status_ind */


/*===========================================================================
  FUNCTION  QMI_UIM_GBA_PROCESS_GBA_EVENT_CB()

  DESCRIPTION
    This function processes the GBA event

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
void qmi_uim_gba_process_gba_event_cb
(
  qmi_uim_message_type * uim_message_ptr
)
{
  uint8                           i              = 0;
  qmi_uimi_client_state_type    * cl_sp          = NULL;
  gba_event_data_type           * gba_event_ptr  = NULL;
  uint32                          evt_reg_mask   = 0;

  ASSERT( uim_message_ptr );

  gba_event_ptr = &uim_message_ptr->data.gba_evt.evt;

  /* Event reg mask for each request is sent to GBA as event user data. */
  evt_reg_mask = gba_event_ptr->user_data;

  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];
    if(cl_sp == NULL)
    {
      continue;
    }

    if ((cl_sp->uim_info.gba_evt_reg_mask & evt_reg_mask) == 0)
    {
      continue;
    }

    qmi_uim_gba_send_gba_key_status_ind(cl_sp->common.clid,
                                        gba_event_ptr);

    /* Reset the event mask bit after sending the indication as the GBA module
       will not maintain the key anymore and the client is expected to register
       for events in the next GBA request. */
    cl_sp->uim_info.gba_evt_reg_mask &= ~evt_reg_mask;
  }

  if (gba_event_ptr->aid_ptr != NULL)
  {
    uimqmi_free(gba_event_ptr->aid_ptr);
  }
  if (gba_event_ptr->btid_ptr != NULL)
  {
    uimqmi_free(gba_event_ptr->btid_ptr);
  }
} /* qmi_uim_gba_process_gba_event_cb */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_PREPARE_IMPI_FROM_IMSI

  DESCRIPTION
    Prepare IMPI From USIM IMSI

  PARAMETERS
    imsi_digit_ptr : value of imsi in a gba_imsi_data_type digit format
    impi_data_ptr : IMPI after conversion from IMSI
    mnc_length: MNC length

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_gba_prepare_impi_from_imsi(
  const qmi_uim_imsi_digits_data_type  *imsi_digit_ptr,
  qmi_uim_impi_type                    *impi_data_ptr,
  uint8                                 mnc_length
)
{
  const char  impi_suffix[]    = "@ims.mnc000.mcc000.3gppnetwork.org";

  if(imsi_digit_ptr == NULL ||
     impi_data_ptr == NULL ||
     imsi_digit_ptr->imsi_digits_len > QMI_UIM_IMSI_DIGIT_LEN)
  {
    return QMI_ERR_INTERNAL;
  }

  /* Process IMSI */
  /* For example if the IMSI is 234150999999999 (MCC = 234, MNC = 15), the
     private user identity then takes the form
     234150999999999@ims.mnc015.mcc234.3gppnetwork.org as specified in TS.23.003
     section 13. */
  (void)memscpy(impi_data_ptr->data,
                sizeof(impi_data_ptr->data),
                imsi_digit_ptr->imsi_digits,
                imsi_digit_ptr->imsi_digits_len);

  impi_data_ptr->data_len = imsi_digit_ptr->imsi_digits_len;

  (void)memscpy(impi_data_ptr->data + impi_data_ptr->data_len,
                QMI_UIM_IMPI_MAX_V01 - impi_data_ptr->data_len,
                impi_suffix,
                strlen(impi_suffix));

  /*Update MNC */
  if(mnc_length == 3)
  {
    impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MNC_START_POS]     =
      impi_data_ptr->data[3];
    impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MNC_START_POS + 1] =
      impi_data_ptr->data[4];
    impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MNC_START_POS + 2] =
      impi_data_ptr->data[5];
  }
  else
  {
    impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MNC_START_POS]     = '0';
    impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MNC_START_POS + 1] =
      impi_data_ptr->data[3];
    impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MNC_START_POS + 2] =
      impi_data_ptr->data[4];
  }

  /* Update MCC */
  impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MCC_START_POS]     =
    impi_data_ptr->data[0];
  impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MCC_START_POS + 1] =
    impi_data_ptr->data[1];
  impi_data_ptr->data[impi_data_ptr->data_len + QMI_UIM_MCC_START_POS + 2] =
    impi_data_ptr->data[2];

  impi_data_ptr->data_len += strlen(impi_suffix);

  return QMI_ERR_NONE;
}/* qmi_uim_gba_prepare_impi_from_imsi */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_GET_IMPI_FROM_USIM()

  DESCRIPTION
    Provides the IMPI data from the USIM application

  PARAMETERS
    tlv_session_info_ptr: Session Information TLV pointer
    impi_data_pptr      : Pointer to a pointer of IMPI data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_gba_get_impi_from_usim
(
  qmi_uim_tlv_session_information_type   *tlv_session_info_ptr,
  qmi_uim_impi_type                     **impi_data_pptr
)
{
  qmi_error_e_type                errval           = QMI_ERR_NONE;
  mmgsdi_slot_id_enum_type        slot_id          = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_protocol_enum_type       card_protocol    = MMGSDI_NO_PROTOCOL;
  qmi_uim_imsi_type               imsi_data        = {0, };
  qmi_uim_imsi_digits_data_type   imsi_digits_data = {0, };
  mmgsdi_session_id_type          session_id       = UIM_INVALID_SESSION_ID;
  uint8                           num_mnc_digits   = 0;

  if(impi_data_pptr == NULL || tlv_session_info_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_session_info_ptr->session_type,
                                                   tlv_session_info_ptr->aid);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  errval = qmi_uim_util_session_id_to_slot(&slot_id,
                                           session_id);
  if(errval != QMI_ERR_NONE)
  {
    return errval;
  }

  card_protocol = qmi_uim_util_card_protocol(slot_id);
  if(card_protocol != MMGSDI_UICC)
  {
    UIM_MSG_ERR_1("QMI_UIM_GET_GBA_IMPI is not supported for protocol 0x%x",
                  card_protocol);
    return QMI_ERR_NOT_SUPPORTED;
  }

  /* Read the IMSI from MMGSDI */
  errval = qmi_uim_util_read_imsi_for_session_id(session_id, &imsi_data);
  if(errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_0("Could not read IMSI");
    return errval;
  }

  /* Convert IMSI data in to IMSI digits format */
  if(qmi_uim_util_convert_imsi_to_digits(&imsi_data, &imsi_digits_data) == FALSE)
  {
    return QMI_ERR_INTERNAL;
  }

  /* Get the MNC digits by reading the EF AD from cache */
  if (qmi_uim_util_get_num_of_mnc_digits(session_id, &num_mnc_digits) == FALSE)
  {
    return QMI_ERR_INTERNAL;
  }

  *impi_data_pptr = (qmi_uim_impi_type *)uimqmi_malloc(sizeof(qmi_uim_impi_type));
  if(*impi_data_pptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Prepare the IMPI from IMSI according to section 13.3 in 3GPP TS.23.003 */
  errval = qmi_uim_gba_prepare_impi_from_imsi(&imsi_digits_data, *impi_data_pptr, num_mnc_digits);
  if(errval != QMI_ERR_NONE)
  {
    uimqmi_free(*impi_data_pptr);
    *impi_data_pptr = NULL;
  }

  return errval;
} /* qmi_uim_gba_get_impi_from_usim */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_GET_IMPI_FROM_ISIM()

  DESCRIPTION
    Provides the IMPI data from the ISIM application

  PARAMETERS
    tlv_data_ptr   : Pointer to TLV data
    tlv_data_count : Count of TLV data
    cmd_buf_ptr    : Pointer to command buffer
    cl_sp          : Client data pointer

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_gba_get_impi_from_isim
(
  qmi_uim_tlv_item_type       * tlv_data_ptr,
  uint8                         tlv_data_count,
  void                        * cmd_buf_ptr,
  qmi_uimi_client_state_type  * cl_sp
)
{
  qmi_error_e_type         errval     = QMI_ERR_NONE;
  mmgsdi_session_id_type   session_id = UIM_INVALID_SESSION_ID;

  ASSERT(tlv_data_count >= 2);

  if(tlv_data_ptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data_ptr[0].data.session_information.session_type,
                                                   tlv_data_ptr[0].data.session_information.aid);
  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      return errval;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_GET_GBA_IMPI,
                                  tlv_data_ptr,
                                  tlv_data_count,
                                  cmd_buf_ptr,
                                  cl_sp);
  }
  else
  {
    qmi_uim_requested_encrytion_type encryption_status = QMI_UIM_NO_ENCRYPTION;
    mmgsdi_access_type               mmgsdi_access;

    memset(&mmgsdi_access, 0x00, sizeof(mmgsdi_access));

    if(tlv_data_ptr[1].data.encrypted.value)
    {
      encryption_status = QMI_UIM_DATA_ONLY_ENCRYPTION;
    }

    /* Read EF IMPI */
    mmgsdi_access.access_method = MMGSDI_EF_ENUM_ACCESS;
    mmgsdi_access.file.file_enum = MMGSDI_ISIM_IMPI;

    errval = qmi_uim_execute_read_transparent(
                            UIMI_CMD_VAL_GET_GBA_IMPI,
                            session_id,
                            mmgsdi_access,
                            0,
                            0,
                            encryption_status,
                            NULL,
                            cmd_buf_ptr,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data_ptr,
                                                       tlv_data_count,
                                                       cl_sp);
    }
  }

  return errval;
} /* qmi_uim_gba_get_impi_from_isim */


/*===========================================================================
  FUNCTION QMI_UIM_GBA_READ_IMPI_RESP()

  DESCRIPTION
    Handle the response for read transparent of IMPI file

  PARAMETERS
    status       : status of MMGSDI operation
    read_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_gba_read_impi_resp
(
  mmgsdi_return_enum_type      status,
  const mmgsdi_read_cnf_type * read_cnf_ptr
)
{
  qmi_uimi_userdata_type           * cb_userdata_ptr    = NULL;
  uint8                              clid               = QMI_SVC_CLID_UNUSED;
  uint32                             client_reg_count   = 0;
  qmi_cmd_buf_type                 * cmd_buf_p          = NULL;
  qmi_uim_requested_encrytion_type   encryption_status  = FALSE;
  qmi_error_e_type                   errval             = QMI_ERR_NONE;
  dsm_item_type *                    response           = NULL;

  UIM_MSG_HIGH_0("Sending response for read of IMPI");

  ASSERT(read_cnf_ptr);

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)read_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid                = cb_userdata_ptr->clid;
  client_reg_count    = cb_userdata_ptr->client_reg_count;
  cmd_buf_p           = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  encryption_status   = cb_userdata_ptr->data.read_transparent.encryption_status;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if(status == MMGSDI_SUCCESS)
  {
    mmgsdi_data_type impi_data = {0, NULL};

    if(read_cnf_ptr->read_data.data_len > QMI_UIM_IMPI_MAX_V01 ||
       read_cnf_ptr->read_data.data_len < 3 ||
       read_cnf_ptr->read_data.data_ptr[0] != QMI_UIM_IMPI_NAI_TAG ||
       read_cnf_ptr->read_data.data_ptr[1] == 0x00)
    {
      UIM_MSG_ERR_3("Invalid data received for EF IMPI len: 0x%x, data[0]: 0x%x, data[1]: 0x%x",
                    read_cnf_ptr->read_data.data_len,
                    read_cnf_ptr->read_data.data_ptr[0],
                    read_cnf_ptr->read_data.data_ptr[1]);
      errval = QMI_ERR_INTERNAL;
      goto send_result;
    }

    /* process IMPI data and parse the TLV */
    if(read_cnf_ptr->read_data.data_ptr[1] > (read_cnf_ptr->read_data.data_len - 2))
    {
      errval = QMI_ERR_INTERNAL;
      goto send_result;
    }

    impi_data.data_ptr = read_cnf_ptr->read_data.data_ptr + 2;
    impi_data.data_len = read_cnf_ptr->read_data.data_ptr[1];

    /* Check whether IMPI data should be encrypted */
    if(encryption_status == QMI_UIM_DATA_ONLY_ENCRYPTION)
    {
      mmgsdi_data_type   impi_enc_data   = {0, NULL};

      if(qmi_uim_encrypt_payload(impi_data, &impi_enc_data))
      {
        if(FALSE == qmi_uim_response_generic_payload_16bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_2,
                     &response,
                     impi_enc_data,
                     TRUE))
        {
          dsm_free_packet(&response);
          errval = QMI_ERR_NO_MEMORY;
        }
      }
      else
      {
        errval = QMI_ERR_INTERNAL;
      }
    }
    else if(encryption_status == QMI_UIM_NO_ENCRYPTION)
    {
      if(FALSE == qmi_uim_response_generic_payload_8bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_1,
                     &response,
                     impi_data,
                     TRUE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      UIM_MSG_HIGH_1("Other encryption status is not expected: 0x%x", encryption_status);
      errval = QMI_ERR_INTERNAL;
    }
  }
  else
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

send_result:
  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_gba_read_impi_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GBA()

  DESCRIPTION
    Performs GBA authentication

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
dsm_item_type * qmi_uimi_gba
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval              = QMI_ERR_NONE;
  qmi_uimi_userdata_type*        cb_userdata_ptr     = NULL;
  gba_result_enum_type           gba_result          = GBA_SUCCESS;
  boolean                        gba_force_bootstrap = FALSE;
  gba_session_type               gba_session_type    = GBA_SESSION_MAX;
  qmi_uimi_ind_token_info_type   token_info          = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[7];
  gba_aid_type                   app_id_data;
  gba_bootstrap_ext_info_type    gba_ext_info;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  memset(&app_id_data, 0, sizeof(gba_aid_type));
  memset(&gba_ext_info, 0, sizeof(gba_bootstrap_ext_info_type));

  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_NAF_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_FORCE_BOOTSTRAPPING;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[4].type      = UIMI_TLV_TYPE_ACCESS_POINT_NAME;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_3;
  tlv_data[5].type      = UIMI_TLV_TYPE_BSF_INFO;
  tlv_data[5].tag       = UIMI_TLV_TAG_OPTIONAL_4;
  tlv_data[6].type      = UIMI_TLV_TYPE_GBA_KEY_STATUS_IND_REG;
  tlv_data[6].tag       = UIMI_TLV_TAG_OPTIONAL_5;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 7);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = qmi_uim_gba_convert_mmgsdi_session_to_gba_session(
             tlv_data[0].data.session_information.session_type,
             &gba_session_type);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if(tlv_data[0].data.session_information.aid.data_len > 0 &&
     tlv_data[0].data.session_information.aid.data_len <= sizeof(app_id_data.data))
  {
    app_id_data.data_len = tlv_data[0].data.session_information.aid.data_len;
    (void)memscpy(app_id_data.data,
                  sizeof(app_id_data.data),
                  tlv_data[0].data.session_information.aid.data_ptr,
                  tlv_data[0].data.session_information.aid.data_len);
  }

  if(tlv_data[2].present)
  {
    gba_force_bootstrap = tlv_data[2].data.gba_force_bootstrapping.value;
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* Update GBA ext info */
  if (tlv_data[4].present)
  {
    gba_ext_info.apn_ptr = tlv_data[4].data.apn.apn_data_ptr;
  }

  if(tlv_data[5].present)
  {
    gba_ext_info.bsf_info_ptr  = tlv_data[5].data.gba_bsf_info.bsf_ptr;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GBA,
                                                 token_info.is_valid ? NULL : cmd_buf_p,
                                                 token_info.is_valid ? &token_info.token : NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Store FQDN to check against the list of white list / black list FQDNs to
     encrypt ks_naf */
  cb_userdata_ptr->data.gba.fqdn_data = tlv_data[1].data.gba_naf_id.naf_id_ptr->fqdn;

  /* Derive the event mask from session type and BSF and store it in the
     callback data which will be used to update the event mask in client state
     in case of successful response. */
  if (tlv_data[6].present &&
      tlv_data[6].data.gba_key_status_ind_reg.value)
  {
    if (!tlv_data[5].present)
    {
      cb_userdata_ptr->data.gba.evt_reg_mask = 1 << gba_session_type;
    }
    else
    {
      /* In case of non-default BSF, set the mask after adding an offest. */
      cb_userdata_ptr->data.gba.evt_reg_mask = 1 << (gba_session_type + GBA_SESSION_MAX);
    }

    gba_ext_info.evt_callback = qmi_uim_gba_event_callback;
    gba_ext_info.evt_user_data = cb_userdata_ptr->data.gba.evt_reg_mask;
  }

  gba_result = gba_bootstrap(tlv_data[1].data.gba_naf_id.naf_id_ptr,
                             gba_session_type,
                             &app_id_data,
                             gba_force_bootstrap,
                             NULL,
                             &gba_ext_info,
                             qmi_uimi_gba_callback,
                             cb_userdata_ptr);

  errval = qmi_uim_gba_convert_gba_status_to_errval(gba_result);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 7);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_6);
} /* qmi_uimi_gba */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_GBA_IMPI()

  DESCRIPTION
    Retrieves the IMPI used for GBA corresponding to the session

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
dsm_item_type * qmi_uimi_get_gba_impi
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                errval           = QMI_ERR_NONE;
  dsm_item_type                 * response         = NULL;
  boolean                         retval           = TRUE;
  qmi_result_e_type               result           = QMI_RESULT_SUCCESS;
  qmi_uim_impi_type             * impi_data_ptr    = NULL;
  qmi_uim_tlv_item_type           tlv_data[2];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_ENCRYPTED;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  switch(tlv_data[0].data.session_information.session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
    case MMGSDI_GW_PROV_SEC_SESSION:
      errval = qmi_uim_gba_get_impi_from_usim(&tlv_data[0].data.session_information,
                                              &impi_data_ptr);
      if(errval != QMI_ERR_NONE || impi_data_ptr == NULL)
      {
        goto send_result;
      }
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_1:
    case MMGSDI_NON_PROV_SESSION_SLOT_2:
      if(qmi_uim_is_isim_aid(&tlv_data[0].data.session_information.aid))
      {
        errval = qmi_uim_gba_get_impi_from_isim(tlv_data,
                                                2,
                                                cmd_buf_p,
                                                cl_sp);
        if(errval != QMI_ERR_NONE)
        {
          goto send_result;
        }
      }
      else
      {
        errval = QMI_ERR_INTERNAL;
        goto send_result;
      }

      /* Free the buffer used for data and wait for asynchronous response for
         IMPI read, so returning */
      qmi_uim_util_free_tlv_content(tlv_data, 2);
      return QMI_SVC_RESPONSE_PENDING;

    default:
      UIM_MSG_ERR_1("QMI_UIM_GET_GBA_IMPI is not supported, session: 0x%x",
                     tlv_data[0].data.session_information.session_type);
      errval = QMI_ERR_NOT_SUPPORTED;
      goto send_result;
  }

  if (impi_data_ptr == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check whether IMPI data should be encrypted */
  if(tlv_data[1].data.encrypted.value)
  {
    mmgsdi_data_type    impi_enc_data        = {0, NULL};
    mmgsdi_data_type    impi_data            = {0, NULL};

    impi_data.data_ptr = impi_data_ptr->data;
    impi_data.data_len = impi_data_ptr->data_len;

    if(qmi_uim_encrypt_payload(impi_data, &impi_enc_data))
    {
      if(FALSE == qmi_uim_response_generic_payload_16bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_2,
                   &response,
                   impi_enc_data,
                   TRUE))
      {
        dsm_free_packet(&response);
        errval = QMI_ERR_NO_MEMORY;
      }
    }
    else
    {
      errval = QMI_ERR_INTERNAL;
    }
  }
  else
  {
    mmgsdi_data_type impi_data = {0, NULL};

    impi_data.data_ptr = (uint8 *)impi_data_ptr->data;
    impi_data.data_len = impi_data_ptr->data_len;

    if(FALSE == qmi_uim_response_generic_payload_8bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &response,
                   impi_data,
                   TRUE))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Free IMPI data pointer */
  uimqmi_free(impi_data_ptr);
  impi_data_ptr = NULL;

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  /* Send the response */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_gba_impi */

