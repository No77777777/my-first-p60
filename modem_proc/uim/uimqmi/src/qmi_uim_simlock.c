/*===========================================================================

                         Q M I _ U I M _ S I M L O C K. C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016 - 2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_simlock.c#3 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/28/19    sg     Added support for NS+SP category
09/10/19    vdc    SimLock solution for activation at Point Of Sale
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
04/25/19    mm     Added support for extended simlock data
12/03/18    vgd    Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
03/14/18    nr     QMI should return Success,after successful lock operation
09/28/17    av     Fix response data len of generate_hash functionality
07/13/17    av     Enhanced simlock RSU solution
04/06/17    gm     Added support for extended APDU
03/28/17    ks     Fixing issue in passing parameter to uimqmi_free
02/20/17    vdc    Added support to probe CK for correctness
12/02/16    me     Combined multi-line ASSERT in one line
11/03/16    bcho   Added support for ICCID simlock
10/28/16    nr     Support remote and local configurations together
10/06/16    ar     Added support for SP + EHPLMN simlock
08/19/16    tkl    Added support for simlock single lock
08/18/16    ar     Add support for simlock storage and emergency only mode
08/03/16    vdc    Added support for SPN based SIMLOCK feature
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
06/16/16    vdc    Split QMI UIM parsing files for supporting IOE
05/26/16    vdc    Move internal enums, structs to new internal header file
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "simlock_common.h"
#include "simlock_modem_lib.h"
#include "simlock_modem_p.h"
#include "user_identity_module_v01.h"
#include "user_identity_module_impl_v01.h"
#include "qmi_uim_simlock.h"
#include "qmi_uim_cat_common.h"
#include "mmgsdilib_common.h"
#include "uim_msg.h"
#include "qmi_uim_util.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_simlock_parsing.h"
#include "qmi_uim_encryption.h"
#include "amssassert.h"
#include <stringl/stringl.h>

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

#ifdef FEATURE_SIMLOCK
/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_RSU_RESP_DEEP_COPY()

  DESCRIPTION
    This function makes a deep copy of the simlock RSU response received from
    simlock engine

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_rsu_resp_deep_copy
(
  simlock_rsu_message_response_data_type        * dst_ptr,
  const simlock_rsu_message_response_data_type  * src_ptr
)
{
  if(dst_ptr == NULL ||
     src_ptr == NULL)
  {
    return;
  }

  switch(src_ptr->msg_type)
  {
    case SIMLOCK_RSU_GET_KEY_MSG:
      if(src_ptr->message.get_key_resp_data.data_len > 0)
      {
        dst_ptr->message.get_key_resp_data.data_len =
          src_ptr->message.get_key_resp_data.data_len;

        uimqmi_deep_copy(
          (void**)&dst_ptr->message.get_key_resp_data.data_ptr,
          (void*)src_ptr->message.get_key_resp_data.data_ptr,
          (uint32)src_ptr->message.get_key_resp_data.data_len);
      }
      break;
    case SIMLOCK_RSU_SET_CONFIGURATION_MSG:
      if(src_ptr->message.set_config_resp_data.data_len > 0)
      {
        dst_ptr->message.set_config_resp_data.data_len =
          src_ptr->message.set_config_resp_data.data_len;

        uimqmi_deep_copy(
          (void**)&dst_ptr->message.set_config_resp_data.data_ptr,
          (void*)src_ptr->message.set_config_resp_data.data_ptr,
          (uint32)src_ptr->message.set_config_resp_data.data_len);
      }
      break;
    case SIMLOCK_RSU_GENERATE_HASH_MSG:
      if(src_ptr->message.generate_hash_resp_data.data_len > 0)
      {
        dst_ptr->message.generate_hash_resp_data.data_len =
          src_ptr->message.generate_hash_resp_data.data_len;

        uimqmi_deep_copy(
          (void**)&dst_ptr->message.generate_hash_resp_data.data_ptr,
          (void*)src_ptr->message.generate_hash_resp_data.data_ptr,
          (uint32)src_ptr->message.generate_hash_resp_data.data_len);
      }
      break;
    case SIMLOCK_RSU_BLOB_REQUEST_MSG:
      if(src_ptr->message.generate_simlock_blob_resp_data.data_len > 0)
      {
        dst_ptr->message.generate_simlock_blob_resp_data.data_len =
          src_ptr->message.generate_simlock_blob_resp_data.data_len;
      
        uimqmi_deep_copy(
          (void**)&dst_ptr->message.generate_simlock_blob_resp_data.data_ptr,
          (void*)src_ptr->message.generate_simlock_blob_resp_data.data_ptr,
          (uint32)src_ptr->message.generate_simlock_blob_resp_data.data_len);
      }
      break;
    case SIMLOCK_RSU_RETRIEVE_VERSION_MSG:
      dst_ptr->message.version.major = src_ptr->message.version.major;
      dst_ptr->message.version.minor = src_ptr->message.version.minor;
      break;
    case SIMLOCK_RSU_DELAY_TIMER_START_REQUEST_MSG:
      dst_ptr->message.delay_timer_value = src_ptr->message.delay_timer_value;
      break;
    default:
      break;
  }
} /* qmi_uim_simlock_rsu_resp_deep_copy */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_RSU_RESP_FREE

  DESCRIPTION
    This function frees the memory that was previously allocated when a deep
    copy of data inside the response was performed in simlock RSU callback.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_rsu_resp_free
(
  simlock_rsu_message_response_data_type        * rsu_resp_ptr
)
{
  if(rsu_resp_ptr == NULL)
  {
    return;
  }

  switch(rsu_resp_ptr->msg_type)
  {
    case SIMLOCK_RSU_GET_KEY_MSG:
      if(rsu_resp_ptr->message.get_key_resp_data.data_ptr != NULL)
      {
        uimqmi_free(rsu_resp_ptr->message.get_key_resp_data.data_ptr);
        rsu_resp_ptr->message.get_key_resp_data.data_ptr = NULL;
      }
      break;
    case SIMLOCK_RSU_SET_CONFIGURATION_MSG:
      if(rsu_resp_ptr->message.set_config_resp_data.data_ptr != NULL)
      {
        uimqmi_free(rsu_resp_ptr->message.set_config_resp_data.data_ptr);
        rsu_resp_ptr->message.set_config_resp_data.data_ptr = NULL;
      }
      break;
    case SIMLOCK_RSU_GENERATE_HASH_MSG:
      if(rsu_resp_ptr->message.generate_hash_resp_data.data_ptr != NULL)
      {
        uimqmi_free(rsu_resp_ptr->message.generate_hash_resp_data.data_ptr);
        rsu_resp_ptr->message.generate_hash_resp_data.data_ptr = NULL;
      }
      break;
    case SIMLOCK_RSU_BLOB_REQUEST_MSG:
      if(rsu_resp_ptr->message.generate_simlock_blob_resp_data.data_ptr != NULL)
      {
        uimqmi_free(rsu_resp_ptr->message.generate_simlock_blob_resp_data.data_ptr);
        rsu_resp_ptr->message.generate_simlock_blob_resp_data.data_ptr = NULL;
      }
      break;
    default:
      break;
  }
} /* qmi_uim_simlock_rsu_resp_free */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_MMGDSI_SLOT_ID_TO_SIMLOCK_SLOT_ID()

  DESCRIPTION
    Converts MMGSDI slot id to SIMLOCK slot id.

  PARAMETERS
    mmgsdi_slot_id       : MMGSDI slot id
    simlock_slot_id_ptr  : output parameter containing SIMLOCK slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_simlock_mmgsdi_slot_id_to_simlock_slot_id
(
  mmgsdi_slot_id_enum_type   mmgsdi_slot_id,
  simlock_slot_enum_type   * simlock_slot_id_ptr
)
{
  ASSERT(simlock_slot_id_ptr);

  switch(mmgsdi_slot_id)
  {
    case MMGSDI_SLOT_1:
      *simlock_slot_id_ptr = SIMLOCK_SLOT_1;
      break;

    case MMGSDI_SLOT_2:
      *simlock_slot_id_ptr = SIMLOCK_SLOT_2;
      break;

    case MMGSDI_SLOT_3:
      *simlock_slot_id_ptr = SIMLOCK_SLOT_3;
      break;

    default:
      *simlock_slot_id_ptr = SIMLOCK_SLOT_NUM;
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_simlock_mmgsdi_slot_id_to_simlock_slot_id */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_SIMLOCK_SLOT_ID_TO_MMGSDI_SLOT_ID()

  DESCRIPTION
    Converts SIMLOCK slot id to MMGSDI slot id.

  PARAMETERS
    simock_slot_id     : SIMLOCK slot id
    mmgsdi_slot_id_ptr : output parameter containing MMGSDI slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_simlock_simlock_slot_id_to_mmgsdi_slot_id
(
  simlock_slot_enum_type      simock_slot_id,
  mmgsdi_slot_id_enum_type  * mmgsdi_slot_id_ptr
)
{
  ASSERT(mmgsdi_slot_id_ptr);

  switch(simock_slot_id)
  {
    case SIMLOCK_SLOT_1:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_1;
      break;

    case SIMLOCK_SLOT_2:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_2;
      break;

    case SIMLOCK_SLOT_3:
      *mmgsdi_slot_id_ptr = MMGSDI_SLOT_3;
      break;

    default:
      *mmgsdi_slot_id_ptr = MMGSDI_MAX_SLOT_ID_ENUM;
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_simlock_simlock_slot_id_to_mmgsdi_slot_id */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_REMOTE_SFS_CB()

  DESCRIPTION
    This function is executed in the context of SIM Lock task, when the
    simlock requires the data to be read from or written to a remote
    SFS

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static simlock_result_enum_type qmi_uim_simlock_remote_sfs_cb (
  simlock_file_operation_enum_type            operation,
  simlock_file_type                           file,
  simlock_token_id_type                       token,
  const simlock_data_type                   * simlock_data_ptr,
  simlock_remote_sfs_response_cb_type         simlock_resp_cb
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(simlock_data_ptr != NULL);

  UIM_MSG_HIGH_1("qmi_uim_simlock_remote_sfs_cb, operation: 0x%x", operation);

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_SIMLOCK_REMOTE_SFS_CB);

  if(msg_ptr != NULL)
  {
    msg_ptr->data.simlock_remote_sfs_req.operation           = operation;
    msg_ptr->data.simlock_remote_sfs_req.request_token_id    = token;
    msg_ptr->data.simlock_remote_sfs_req.file                = file;
    msg_ptr->data.simlock_remote_sfs_req.simlock_resp_cb     = simlock_resp_cb;

    if (simlock_data_ptr->data_len > 0)
    {
      msg_ptr->data.simlock_remote_sfs_req.simlock_data.data_len =
                                               simlock_data_ptr->data_len;

      uimqmi_deep_copy(
        (void**)&msg_ptr->data.simlock_remote_sfs_req.simlock_data.data_ptr,
        (void*)simlock_data_ptr->data_ptr,
        (uint32)simlock_data_ptr->data_len);
    }
    else
    {
      msg_ptr->data.simlock_remote_sfs_req.simlock_data.data_len = 0;
      msg_ptr->data.simlock_remote_sfs_req.simlock_data.data_ptr = NULL;
    }

    /* Send command */
    if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
       (present_in_queue))
    {
      uimqmi_free(msg_ptr->data.simlock_remote_sfs_req.simlock_data.data_ptr);
      uimqmi_free(msg_ptr);
    }
    msg_ptr = NULL;

    return SIMLOCK_SUCCESS;
  }

  return SIMLOCK_GENERIC_ERROR;
} /* qmi_uim_simlock_remote_sfs_cb */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_REGISTER_FOR_REMOTE_SFS_OPERATION()

  DESCRIPTION
    This function registers the QMI UIM callback with the simlock, so the
    remote SFS operations can be performed.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_simlock_register_for_remote_sfs_operation
(
  void
)
{
  /* Register for events from SIM Lock task */
  (void)simlock_register_remote_sfs_operation(SIMLOCK_REGISTER,
                                              qmi_uim_simlock_remote_sfs_cb);
} /* qmi_uim_simlock_register_for_remote_sfs_operation */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_CONVERT_SIMLOCK_CATEGORY()

  DESCRIPTION
    Converts the SIM Lock category to MMGSDI feature type

  PARAMETERS
    category            : simlock category
    feature_ptr         : response mmgsdi category
    category_index_ptr  : response index value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_simlock_convert_simlock_category
(
  simlock_category_enum_type        simlock_category,
  mmgsdi_perso_feature_enum_type  * mmgsdi_category_ptr,
  uint8                           * category_index_ptr
)
{
  ASSERT(mmgsdi_category_ptr && category_index_ptr);

  switch (simlock_category)
  {
    case SIMLOCK_CATEGORY_3GPP_NW:
      *mmgsdi_category_ptr = MMGSDI_PERSO_NW;
      *category_index_ptr  = 0;
      break;
    case SIMLOCK_CATEGORY_3GPP_NS:
      *mmgsdi_category_ptr = MMGSDI_PERSO_NS;
      *category_index_ptr  = 1;
      break;
    case SIMLOCK_CATEGORY_3GPP_SP:
      *mmgsdi_category_ptr = MMGSDI_PERSO_SP;
      *category_index_ptr  = 2;
      break;
    case SIMLOCK_CATEGORY_3GPP_CP:
      *mmgsdi_category_ptr = MMGSDI_PERSO_CP;
      *category_index_ptr  = 3;
      break;
    case SIMLOCK_CATEGORY_3GPP_SIM:
      *mmgsdi_category_ptr = MMGSDI_PERSO_SIM;
      *category_index_ptr  = 4;
      break;
    case SIMLOCK_CATEGORY_3GPP_SPN:
      *mmgsdi_category_ptr = MMGSDI_PERSO_SPN;
      *category_index_ptr  = 5;
      break;
    case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
      *mmgsdi_category_ptr = MMGSDI_PERSO_SP_EHPLMN;
      *category_index_ptr  = 6;
      break;
    case SIMLOCK_CATEGORY_3GPP_ICCID:
      *mmgsdi_category_ptr = MMGSDI_PERSO_ICCID;
      *category_index_ptr  = 7;
      break;
    case SIMLOCK_CATEGORY_3GPP_NS_SP:
      *mmgsdi_category_ptr = MMGSDI_PERSO_NS_SP;
      *category_index_ptr  = 8;
      break;
    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE1:
      *mmgsdi_category_ptr = MMGSDI_PERSO_RUIM_NW1;
      *category_index_ptr  = 9;
      break;
    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE2:
      *mmgsdi_category_ptr = MMGSDI_PERSO_RUIM_NW2;
      *category_index_ptr  = 10;
      break;
    case SIMLOCK_CATEGORY_3GPP2_HRPD:
      *mmgsdi_category_ptr = MMGSDI_PERSO_RUIM_HRPD;
      *category_index_ptr  = 11;
      break;
    case SIMLOCK_CATEGORY_3GPP2_SP:
      *mmgsdi_category_ptr = MMGSDI_PERSO_RUIM_SP;
      *category_index_ptr  = 12;
      break;
    case SIMLOCK_CATEGORY_3GPP2_CP:
      *mmgsdi_category_ptr = MMGSDI_PERSO_RUIM_CP;
      *category_index_ptr  = 13;
      break;
    case SIMLOCK_CATEGORY_3GPP2_RUIM:
      *mmgsdi_category_ptr = MMGSDI_PERSO_RUIM_RUIM;
      *category_index_ptr  = 14;
      break;
    default:
      UIM_MSG_ERR_1("Invalid SIM Lock category: 0x%x", simlock_category);
      *mmgsdi_category_ptr = MMGSDI_MAX_PERSO_FEATURE_ENUM;
      return FALSE;
  }

  return TRUE;
} /* qmi_uim_simlock_convert_simlock_category */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_UPDATE_SIMLOCK_RETRIES()

  DESCRIPTION
    Updates the number of SIM Lock retries in the global card status after
    a failed perso operation

  PARAMETERS
    category    : category to update retries
    slot        : slot id corresponding to unlock request
    slot_policy : the current slot policy of the simlock
    retries     : new number of retries

  RETURN VALUE
    Boolean indicating if there was an update

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_simlock_update_simlock_retries
(
  simlock_category_enum_type      category,
  simlock_slot_enum_type          slot,
  simlock_slot_policy_enum_type   slot_policy,
  uint32                          retries
)
{
  uint16                           i                 = 0;
  boolean                          result            = FALSE;
  mmgsdi_slot_id_enum_type         mmgsdi_slot_id    = MMGSDI_SLOT_1;
  mmgsdi_perso_feature_enum_type   feature           = MMGSDI_MAX_PERSO_FEATURE_ENUM;
  uint8                            index             = 0;

  ASSERT(qmi_uim_global_ptr != NULL);

  UIM_MSG_MED_2("qmi_uim_update_simlock_retries, slot: 0x%x, slot_policy: 0x%x",
                slot, slot_policy);

  result = qmi_uim_simlock_convert_simlock_category(category, &feature, &index);
  if (result == FALSE)
  {
    return result;
  }

  if(qmi_uim_simlock_simlock_slot_id_to_mmgsdi_slot_id(slot, &mmgsdi_slot_id) != QMI_ERR_NONE)
  {
    return FALSE;
  }

  for (i = 0; i < qmi_uim_global_ptr->card_state.num_apps &&
              i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
        (qmi_uim_global_ptr->card_state.application[i]->app_state ==
          QMI_UIM_APP_STATE_PERSO ||
         qmi_uim_global_ptr->card_state.application[i]->app_state ==
          QMI_UIM_APP_STATE_READY) &&
        qmi_uim_global_ptr->card_state.application[i]->perso_feature == feature)
    {
      /* In this case, it is possible that each slot has a different
         lock configuration. So only apps belonging to a slot on which
         the unlock operation was performed are updated */
      if(slot_policy == SIMLOCK_SLOT_POLICY_UNIQUE_FOR_EACH_SLOT &&
         qmi_uim_global_ptr->card_state.application[i]->slot != mmgsdi_slot_id)
      {
        continue;
      }

      /* Update global status */
      if (qmi_uim_global_ptr->card_state.application[i]->perso_retries != retries)
      {
        qmi_uim_global_ptr->card_state.application[i]->perso_retries = retries;
        result = TRUE;
      }
    }
  }

  return result;
} /* qmi_uim_simlock_update_simlock_retries */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_SEND_REMOTE_SFS_OPERATION_INDICATION()

  DESCRIPTION
    This function sends a simlock operation indication to a specific client.

  PARAMETERS
    clid           : Client ID
    uim_message_ptr: QMI UIM message

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_simlock_send_remote_sfs_operation_indication
(
  uint8                           clid,
  const qmi_uim_message_type    * uim_message_ptr
)
{
  dsm_item_type               * indication   = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && uim_message_ptr);

  /* Insert the indication payload in TLV */
  if (FALSE == qmi_uim_simlock_remote_sfs_encrypted_ind(
                   UIMI_CMD_VAL_SIMLOCK_CONFIGURATION,
                   uim_message_ptr->data.simlock_remote_sfs_req.operation,
                   uim_message_ptr->data.simlock_remote_sfs_req.request_token_id,
                   uim_message_ptr->data.simlock_remote_sfs_req.file,
                   uim_message_ptr->data.simlock_remote_sfs_req.simlock_data,
                   &indication))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return QMI_ERR_INTERNAL;
  }

  /* Send indication to the client */
  if(FALSE == uimqmi_send_indication(clid,
                                     (uint16)UIMI_CMD_VAL_SIMLOCK_CONFIGURATION,
                                     indication,
                   QMUX_SERVICE_UIM))
  {
    return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_simlock_send_remote_sfs_operation_indication */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_SEND_REMOTE_SFS_OPERATION_INDICATION_TO_ALL()

  DESCRIPTION
    This function will send the simlock data operation indication to all
    registered clients.

  PARAMETERS
    uim_message_ptr: QMI UIM message

  RETURN VALUE
    TRUE  : If indication is sent to at least one client
    FALSE : If otherwise

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_simlock_send_remote_sfs_operation_indication_to_all
(
  const qmi_uim_message_type * uim_message_ptr
)
{
  uint8                              i               = 0;
  qmi_uimi_client_state_type       * cl_sp           = NULL;
  qmi_error_e_type                   errval          = QMI_ERR_NONE;
  boolean                            indication_sent = FALSE;

  if((qmi_uim_state_ptr == NULL) ||
     (uim_message_ptr == NULL))
  {
    return FALSE;
  }

  UIM_MSG_HIGH_0("qmi_uim_simlock_send_remote_sfs_operation_indication_to_all");

  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];
    if(cl_sp == NULL)
    {
      continue;
    }

    /* Send simlock remote SFS indication if client requested for them during client event
       registration (saved in reg_event_type.simlock_remote_sfs_events flag) */
    if (cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
        cl_sp->uim_info.reg_event_type.simlock_remote_sfs_events)
    {
      errval = qmi_uim_simlock_send_remote_sfs_operation_indication(
                   cl_sp->common.clid,
                   uim_message_ptr);
      if ((errval == QMI_ERR_NONE) &&
          (!indication_sent))
      {
        indication_sent = TRUE;
      }
    }
  }

  return indication_sent;
} /* qmi_uim_simlock_send_remote_sfs_operation_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_CHECK_AND_UPDATE_IF_SEND_TEMPORARY_UNLOCK_STATUS_CHANGED()

  DESCRIPTION
    This function will check if the temporary unlock status is changed.
        It compares the temporary unlock status received from SIMLOCK against the one
        maintained at QMI UIM.If atleast a single feature's temporary unlock status is
    changed then this function would return TRUE.Also, it would update the QMI UIM
    global maintaining the temp unlock status.

  PARAMETERS
    get_status_resp_ptr  : Pointer to the get status response received from SIMLOCK

  RETURN VALUE
    TRUE  : If status is changed
    FALSE : If otherwise

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_simlock_check_and_update_if_simlock_temporary_unlock_status_changed(
  const simlock_get_status_msg_resp_type  *get_status_ptr
)
{
  boolean          send_ind     = FALSE;
  uint8            i            = 0;
  uint8            j            = 0;
  uint8            slot_cnt     = 0;

  ASSERT(get_status_ptr);

  slot_cnt = MIN(qmi_uim_global_ptr->card_state.num_slots, QMI_UIM_MAX_CARD_COUNT);

  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < slot_cnt;
       i++)
  {
    /* Check the features that are active and insert the
       corresponding feature id and time left in the TLV data */
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if ((get_status_ptr->lock_info[i].category_info[j].category_status) &&
          (get_status_ptr->remaining_time.duration[i][j] > 0))
      {
        if(qmi_uim_global_ptr->temp_unlock_info[i][j] == FALSE)
        {
          /* A new feature is found to be temporarily unlocked.
             Need to send indication to client */
            send_ind = TRUE;
            qmi_uim_global_ptr->temp_unlock_info[i][j] = TRUE;
          }
        }
      else
      {
        if(qmi_uim_global_ptr->temp_unlock_info[i][j])
        {
          /* Temporary unlock for a feature seem to be expired.
             Need to send indication to client */
            send_ind = TRUE;
            qmi_uim_global_ptr->temp_unlock_info[i][j] = FALSE;
        }
      }
    }
  }

  UIM_MSG_MED_1("qmi_uim_check_and_update_if_simlock_temporary_unlock_status_changed "
                "send_ind is 0x%x", send_ind);

  return TRUE;
}/* qmi_uim_simlock_check_and_update_if_simlock_temporary_unlock_status_changed */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_SEND_TEMPORARY_UNLOCK_INDICATION()

  DESCRIPTION
    This function sends a temporary unlock  indication to a specific client.

  PARAMETERS
    clid           : Client ID
    uim_message_ptr: QMI UIM message

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_simlock_send_temporary_unlock_indication
(
  uint8                                    clid,
  const simlock_get_status_msg_resp_type  *get_status_ptr
)
{
  dsm_item_type               * indication = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && get_status_ptr);

  /* Insert the indication payload in TLV */
  if (FALSE == qmi_uim_response_temporary_unlock_status(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                        &indication,
                                                        get_status_ptr,
                                                        qmi_uim_global_ptr->card_state.num_slots))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    UIM_MSG_MED_1("qmi_uim_response_temporary_unlock_status failed. No ind sent to client 0x%x", clid);
    dsm_free_packet(&indication);
    return QMI_ERR_INTERNAL;
  }

  /* Send indication to the client */
  if(FALSE == uimqmi_send_indication(clid,
                                     (uint16)UIMI_CMD_VAL_TEMPORARY_UNLOCK_STATUS_IND,
                                     indication,
                   QMUX_SERVICE_UIM))
  {
    return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_simlock_send_temporary_unlock_indication */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_SEND_TEMPORARY_UNLOCK_INDICATION_TO_ALL()

  DESCRIPTION
    This function will send the temporary unlock indication to all
    registered clients.

  PARAMETERS
    get_status_ptr: Pointer to get status response received from SIMLOCK

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_send_temporary_unlock_indication_to_all
(
  const simlock_get_status_msg_resp_type *get_status_ptr
)
{
  uint8                              i               = 0;
  qmi_uimi_client_state_type       * cl_sp           = NULL;
  qmi_error_e_type                   errval          = QMI_ERR_NONE;

  if(qmi_uim_state_ptr == NULL ||
     get_status_ptr == NULL)
  {
    return;
  }

  UIM_MSG_HIGH_0("qmi_uim_send_temporary_unlock_indication_to_all");

  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];
    if(cl_sp == NULL)
    {
      continue;
    }

    /* Send temporary unlock indication if client requested for them during client event
       registration (saved in reg_event_type.simlock_temp_unlock_status flag) */
    if (cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
        cl_sp->uim_info.reg_event_type.simlock_temp_unlock_status)
    {
      errval = qmi_uim_simlock_send_temporary_unlock_indication(
                   cl_sp->common.clid,
                   get_status_ptr);
      }
    }
} /* qmi_uim_simlock_send_temporary_unlock_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIMI_SIMLOCK_SET_REMOTE_SFS_CALLBACK()

  DESCRIPTION
    This function is executed as callback of simlock_set_remote_sfs API.
    This is executed in the context of SIM Lock task.

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_simlock_set_remote_sfs_callback
(
  simlock_result_enum_type                    status,
  const void                                * user_data_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(user_data_ptr != NULL);

  UIM_MSG_HIGH_0("Got response from set remote SFS req");

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_SIMLOCK_SET_REMOTE_SFS_CB);

  if(msg_ptr != NULL)
  {
    msg_ptr->data.simlock_set_remote_sfs_resp.status        = status;
    msg_ptr->data.simlock_set_remote_sfs_resp.user_data_ptr = user_data_ptr;

    /* Send command */
    if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
       (present_in_queue))
    {
      uimqmi_free(msg_ptr);
    }
    msg_ptr = NULL;
  }
} /* qmi_uimi_simlock_set_remote_sfs_callback */


/*===========================================================================
  FUNCTION QMI_UIMI_SIMLOCK_REMOTE_UNLOCK_CALLBACK()

  DESCRIPTION
    This function is executed as callback of SIM Lock remote unlock API.
    This is executed in the context of SIM Lock task and needs to make a deep
    copy of the results and post a command to the QMI task to be processed.

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_simlock_remote_unlock_callback
(
  simlock_result_enum_type                        status,
  const simlock_rsu_message_response_data_type  * resp_ptr,
  const void                                    * user_data_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  if(resp_ptr == NULL)
  {
    return;
  }

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_REMOTE_UNLOCK_CB);
  if(msg_ptr != NULL)
  {
    msg_ptr->data.remote_unlock_resp.status        = status;
    msg_ptr->data.remote_unlock_resp.user_data_ptr = user_data_ptr;
    msg_ptr->data.remote_unlock_resp.data.msg_type = resp_ptr->msg_type;

    qmi_uim_simlock_rsu_resp_deep_copy(&(msg_ptr->data.remote_unlock_resp.data),
                                       resp_ptr);

    /* Send command */
    if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
       (present_in_queue))
    {
      qmi_uim_simlock_rsu_resp_free(&(msg_ptr->data.remote_unlock_resp.data));
      uimqmi_free(msg_ptr);
    }
    msg_ptr = NULL;
  }
} /* qmi_uimi_simlock_remote_unlock_callback */


/*===========================================================================
  FUNCTION QMI_UIMI_SIMLOCK_TEMPORARY_UNLOCK_CALLBACK()

  DESCRIPTION
    This function is executed as callback of SIM Lock API. This is
    executed in the context of SIM Lock task and needs to make a deep
    copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_simlock_temporary_unlock_callback
(
  simlock_result_enum_type                    status,
  const simlock_message_response_data_type  * resp_ptr,
  const void                                * user_data_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  /* resp_ptr can be NULL if SIMLOCK cleans up commands as part of TASK STOP
     signal handling */
  if(resp_ptr == NULL)
  {
    return;
  }

  UIM_MSG_HIGH_0("qmi_uimi_simlock_get_status_callback");

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_SIMLOCK_TEMPORARY_UNLOCK_CB);
  if (msg_ptr != NULL)
  {
    msg_ptr->data.simlock.status  = status;
    msg_ptr->data.simlock.user_data_ptr = user_data_ptr;

    memscpy(&msg_ptr->data.simlock.data,
            sizeof(msg_ptr->data.simlock.data),
            resp_ptr,
            sizeof(simlock_message_response_data_type));

    /* Send command */
    if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
       (present_in_queue))
    {
      uimqmi_free(msg_ptr);
    }
    msg_ptr = NULL;
  }
} /* qmi_uimi_simlock_temporary_unlock_callback */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_UNLOCK_RESP

  DESCRIPTION
    Processes the response to the SIM Lock unlock command

  PARAMETERS
    status          : Status of the unlock request
    unlock_ptr      : Pointer to the unlock response data
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_unlock_resp
(
  simlock_result_enum_type                     status,
  const simlock_unlock_device_msg_resp_type  * unlock_ptr,
  qmi_uimi_userdata_type                     * user_data_ptr
)
{
  qmi_cmd_buf_type              * cmd_buf_p           = NULL;
  dsm_item_type                 * response            = NULL;
  qmi_error_e_type                errval              = QMI_ERR_NONE;
  uint8                           clid                = QMI_SVC_CLID_UNUSED;
  simlock_category_enum_type      category            = SIMLOCK_CATEGORY_3GPP_NW;
  simlock_slot_enum_type          slot                = SIMLOCK_SLOT_1;
  uint32                          client_reg_count    = 0;
  uint32                          num_retries         = 0;
  uint16                          request_id          = 0;
  boolean                         ck_present          = TRUE;
  qmi_uimi_cmd_val_e_type         cmd_id              = UIMI_CMD_VAL_WIDTH;

  UIM_MSG_HIGH_0("qmi_uim_simlock_process_unlock_resp");

  ASSERT(unlock_ptr && user_data_ptr);

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  cmd_id           = user_data_ptr->request_id;
  client_reg_count = user_data_ptr->client_reg_count;
  category         = user_data_ptr->data.simlock_unlock.category;
  slot             = user_data_ptr->data.simlock_unlock.slot;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;
  request_id       = user_data_ptr->data.simlock_unlock.request_id;
  ck_present       = user_data_ptr->data.simlock_unlock.ck_present;

  if (status != SIMLOCK_SUCCESS && ck_present)
  {
    simlock_result_enum_type    simlock_status  = SIMLOCK_SUCCESS;

    if (unlock_ptr->num_retries_max == 0)
    {
      num_retries = QMI_UIM_MAX_RETRIES;
    }
    else if (unlock_ptr->curr_retries >= unlock_ptr->num_retries_max)
    {
      num_retries = 0;
    }
    else
    {
      num_retries = unlock_ptr->num_retries_max - unlock_ptr->curr_retries;
    }

    user_data_ptr->data.simlock_unlock.num_retries = num_retries;
    user_data_ptr->data.simlock_unlock.result      = status;

    simlock_status = simlock_get_status(qmi_uimi_simlock_callback,
                                        user_data_ptr);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      /* In case of sucess return. The response to client
         is sent in the get_status callback */
      return;
    }

    /* Update SIM Lock retries in the global variable. Since
       get_status failed, use the default slot policy to update retries */
    (void)qmi_uim_simlock_update_simlock_retries(category, slot,
                                                 SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS,
                                                 num_retries);
  }

  /* Convert SIMLOCK status in QMI code */
  errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

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

  if (status != SIMLOCK_SUCCESS && ck_present)
  {
    /* Send the num_retries in the response */
    if (FALSE == qmi_uim_response_retries_left(
                  UIMI_TLV_RESP_TAG_OPTIONAL_1,
                  &response,
                  num_retries,
                  0))
    {
      dsm_free_packet(&response);
    }
  }

  if(cmd_id == UIMI_CMD_VAL_DEPERSONALIZATION_SECURE)
  {
    /* build the result and encrypt the data */
    errval = qmi_uim_response_encrypted_status(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                               UIMI_CMD_VAL_DEPERSONALIZATION_SECURE,
                                               request_id,
                                               errval,
                                               &response);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_unlock_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_GET_STATUS_RESP_AFTER_UNLOCK

  DESCRIPTION
    Processes the response to the SIM Lock get status command which was
    requested due to a failed unlock attempt

  PARAMETERS
    status          : Status of the get_status request
    get_status_ptr  : Pointer to the response data
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_get_status_resp_after_unlock
(
  simlock_result_enum_type                  status,
  const simlock_get_status_msg_resp_type  * get_status_ptr,
  qmi_uimi_userdata_type                  * user_data_ptr
)
{
  qmi_cmd_buf_type             * cmd_buf_p           = NULL;
  dsm_item_type                * response            = NULL;
  qmi_error_e_type               errval              = QMI_ERR_NONE;
  uint8                          clid                = QMI_SVC_CLID_UNUSED;
  uint32                         client_reg_count    = 0;
  simlock_category_enum_type     category            = SIMLOCK_CATEGORY_3GPP_NW;
  simlock_slot_enum_type         slot                = SIMLOCK_SLOT_1;
  simlock_result_enum_type       unlock_status       = SIMLOCK_SUCCESS;
  simlock_slot_policy_enum_type  slot_policy         = SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS;
  uint32                         num_retries         = 0;

  UIM_MSG_MED_0("qmi_uim_simlock_process_get_status_resp_after_unlock");

  ASSERT(get_status_ptr && user_data_ptr);

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  category         = user_data_ptr->data.simlock_unlock.category;
  slot             = user_data_ptr->data.simlock_unlock.slot;
  unlock_status    = user_data_ptr->data.simlock_unlock.result;
  num_retries      = user_data_ptr->data.simlock_unlock.num_retries;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

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

  /* If the request is a success, retrieve the
     slot_policy from the response. In case the request
     is a failure, then set the policy to SAME_ON_ALL_SLOTS
     as the best possible option */
  if (status == SIMLOCK_SUCCESS)
  {
    slot_policy = get_status_ptr->slot_policy;
  }

  /* Update SIM Lock retries in the global variable */
  (void)qmi_uim_simlock_update_simlock_retries(category, slot,
                                               slot_policy,
                                               num_retries);

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(unlock_status);

  if (FALSE == qmi_uim_response_retries_left(
                    UIMI_TLV_RESP_TAG_OPTIONAL_1,
                    &response,
                    num_retries,
                    0))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_get_status_resp_after_unlock */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_GET_STATUS_RESP

  DESCRIPTION
    Processes the response to the SIM Lock get status command

  PARAMETERS
    status          : Status of the get_status request
    get_status_ptr  : Pointer to the response data
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_get_status_resp
(
  simlock_result_enum_type                  status,
  const simlock_get_status_msg_resp_type  * get_status_ptr,
  qmi_uimi_userdata_type                  * user_data_ptr
)
{
  qmi_cmd_buf_type       * cmd_buf_p           = NULL;
  dsm_item_type          * response            = NULL;
  qmi_error_e_type         errval              = QMI_ERR_NONE;
  uint8                    clid                = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count    = 0;

  UIM_MSG_HIGH_0("qmi_uim_process_simlock_get_status_resp");

  ASSERT(get_status_ptr && user_data_ptr && qmi_uim_global_ptr);

  if (status != SIMLOCK_SUCCESS)
  {
    UIM_MSG_ERR_1("Error in SIM Lock result: 0x%x", status);
    errval = QMI_ERR_INTERNAL;
  }

  /* If we are sending result here then personalisation data was definitly requested.*/
  /* Send the automatic selection TLV only if it was requested, even if we failed to get perso
     configuration */
  if(user_data_ptr->data.get_configuration.automatic_selection_request)
  {
    if (FALSE == qmi_uim_response_boolean(
                        UIMI_TLV_RESP_TAG_OPTIONAL_1,
                        &response,
                        user_data_ptr->data.get_configuration.automatic_selection))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV with perso configuration only in case of success */
  if ((user_data_ptr->data.get_configuration.personalization_request) &&
      (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_get_configuration_simlock(
                        UIMI_TLV_RESP_TAG_OPTIONAL_2,
                        &response,
                        get_status_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

    /* Insert TLV with halt subscription configuration */
  if((user_data_ptr->data.get_configuration.halt_subscription_request) &&
     (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_boolean(
                      UIMI_TLV_RESP_TAG_OPTIONAL_3,
                      &response,
                      user_data_ptr->data.get_configuration.halt_subscription))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV with temporary unlock time left */
  if ((user_data_ptr->data.get_configuration.personalization_request ||
       user_data_ptr->data.get_configuration.perso_reactivation_request) &&
      (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_temporary_unlock_time_left(
                        UIMI_TLV_RESP_TAG_OPTIONAL_7,
                        &response,
                        qmi_uim_global_ptr->card_state.num_slots,
                        get_status_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV with perso reactivation only in case of success */
  if ((user_data_ptr->data.get_configuration.perso_reactivation_request) &&
      (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_perso_reactivation_status(
                        UIMI_TLV_RESP_TAG_OPTIONAL_8,
                        &response,
                        qmi_uim_global_ptr->card_state.num_slots,
                        get_status_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV with perso control key only in case of success */
  if ((user_data_ptr->data.get_configuration.personalization_request ||
       user_data_ptr->data.get_configuration.perso_reactivation_request) &&
      (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_perso_control_key_presence(
                        UIMI_TLV_RESP_TAG_OPTIONAL_9,
                        &response,
                        qmi_uim_global_ptr->card_state.num_slots,
                        get_status_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV for remote simlock storage in case of success */
  if((user_data_ptr->data.get_configuration.remote_simlock_storage_request) &&
     (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_boolean(
                        UIMI_TLV_RESP_TAG_OPTIONAL_10,
                        &response,
                        get_status_ptr->remote_simlock_storage))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV for emergency only mode status in case of success */
  if((user_data_ptr->data.get_configuration.emergency_only_mode_request) &&
     (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_boolean(
                        UIMI_TLV_RESP_TAG_OPTIONAL_11,
                        &response,
                        get_status_ptr->emergency_only_mode))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV for extended apdu status in case of success */
  if((user_data_ptr->data.get_configuration.extended_apdu_request) &&
     (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_boolean(
                    UIMI_TLV_RESP_TAG_OPTIONAL_12,
                    &response,
                    user_data_ptr->data.get_configuration.extended_apdu))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Insert TLV with inactive config only in case of success */
  if ((user_data_ptr->data.get_configuration.inactive_simlock_config) &&
      (errval == QMI_ERR_NONE))
  {
    if (FALSE == qmi_uim_response_simlock_inactive_config_status(
                        UIMI_TLV_RESP_TAG_OPTIONAL_13,
                        &response,
                        get_status_ptr))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    dsm_free_packet(&response);
    return;
  }

  if (cmd_buf_p == NULL)
  {
    dsm_free_packet(&response);
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF_FREE_RESPONSE(cmd_buf_p, response);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_get_status_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_TEMPORARY_UNLOCK_CB

  DESCRIPTION
    Processes the response to the SIM Lock get status command which is queued
    in handling of MMGSDI_TEMPORARY_UNLOCK_EVENT.

  PARAMETERS
    uim_message_ptr : Message pointer containing the information received from SIMLOCK
                      get status request

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_simlock_process_temporary_unlock_cb(
  const qmi_uim_message_type *uim_message_ptr
)
{
  const simlock_get_status_msg_resp_type *get_status_ptr = NULL;

  ASSERT(uim_message_ptr);

  UIM_MSG_MED_0("qmi_uim_simlock_process_temporary_unlock_cb");

  /* If the response from SIMLOCK is failure and is not get_status message,
     then no further processing can be done.*/
  if(uim_message_ptr->message_type != QMI_UIM_MESSAGE_SIMLOCK_TEMPORARY_UNLOCK_CB ||
     uim_message_ptr->data.simlock.status != SIMLOCK_SUCCESS ||
     uim_message_ptr->data.simlock.data.msg_type != SIMLOCK_GET_STATUS_MSG)
  {
    return;
  }

  get_status_ptr = &uim_message_ptr->data.simlock.data.message.get_status_resp;

  if(qmi_uim_simlock_check_and_update_if_simlock_temporary_unlock_status_changed(
       get_status_ptr))
  {
    qmi_uim_simlock_send_temporary_unlock_indication_to_all(get_status_ptr);
  }
} /* qmi_uim_simlock_process_temporary_unlock_cb */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_LOCK_RESP

  DESCRIPTION
    Processes the response of the personalization command

  PARAMETERS
    status          : Status of the lock request
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_lock_resp
(
  simlock_result_enum_type       status,
  qmi_uimi_userdata_type       * user_data_ptr
)
{
  qmi_cmd_buf_type          * cmd_buf_p           = NULL;
  dsm_item_type             * response            = NULL;
  qmi_error_e_type            errval              = QMI_ERR_NONE;
  uint8                       clid                = QMI_SVC_CLID_UNUSED;
  uint32                      client_reg_count    = 0;
  uint16                      request_id          = 0;
  boolean                     ck_present          = TRUE;
  qmi_uimi_cmd_val_e_type     cmd_id              = UIMI_CMD_VAL_WIDTH;

  UIM_MSG_HIGH_0("qmi_uim_simlock_process_lock_resp");

  ASSERT( user_data_ptr );

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  cmd_id           = user_data_ptr->request_id;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;
  request_id       = user_data_ptr->data.simlock_lock.request_id;
  ck_present       = user_data_ptr->data.simlock_lock.ck_present;

  if((status == SIMLOCK_SUCCESS) &&
     (user_data_ptr->data.simlock_lock.next_list_to_lock <
               user_data_ptr->data.simlock_lock.total_list_cnt) &&
     (user_data_ptr->data.simlock_lock.code_ptr != NULL))
  {
    simlock_result_enum_type                 simlock_status     = SIMLOCK_SUCCESS;
    uint8                                    list_to_lock       = 0;
    simlock_add_lock_codes_ck_msg_req_type  *add_lock_codes_ptr = NULL;

    add_lock_codes_ptr =
      (simlock_add_lock_codes_ck_msg_req_type *)uimqmi_malloc(sizeof(simlock_add_lock_codes_ck_msg_req_type));

    if(add_lock_codes_ptr != NULL)
    {
      UIM_MSG_HIGH_0("Processing additional lock codes");

      list_to_lock = user_data_ptr->data.simlock_lock.next_list_to_lock;

      add_lock_codes_ptr->slot      = user_data_ptr->data.simlock_lock.slot;
      add_lock_codes_ptr->ck        = user_data_ptr->data.simlock_lock.ck;
      add_lock_codes_ptr->blacklist = FALSE;
      add_lock_codes_ptr->category  = user_data_ptr->data.simlock_lock.code_ptr[list_to_lock];

      user_data_ptr->data.simlock_lock.next_list_to_lock++;

      simlock_status = simlock_add_lock_codes_ck(add_lock_codes_ptr,
                                                 qmi_uimi_simlock_callback,
                                                 user_data_ptr);

      /* Free add lock codes pointer irrespective of queuing status */
      uimqmi_free(add_lock_codes_ptr);

      if(simlock_status == SIMLOCK_SUCCESS)
      {
        return;
      }
    }
  }

  if(user_data_ptr->data.simlock_lock.next_list_to_lock == 0)
  {
    /* Indicates there was an error in the get lock status
       response, requested from the perso secure message
                           or
       Indicates perso message was requested with reuse code
       data TLV set*/
    errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);
  }
  else if(user_data_ptr->data.simlock_lock.next_list_to_lock == 1)
  {
    /* Indicates there was only one list to lock */
    errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);
  }
  else if((user_data_ptr->data.simlock_lock.next_list_to_lock ==
             user_data_ptr->data.simlock_lock.total_list_cnt)  &&
          (status == SIMLOCK_SUCCESS))
  {
    /* Indicates all the lists were successfully locked */
    errval = QMI_ERR_NONE;
  }
  else
  {
    /* Indicates some lists were added successfully,
       but then an error occurred */
    errval = QMI_ERR_OP_PARTIAL_FAILURE;
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

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

  if(cmd_id == UIMI_CMD_VAL_PERSONALIZATION_SECURE)
  {
    /* build the result and encrypt the data */
    errval = qmi_uim_response_encrypted_status(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                               UIMI_CMD_VAL_PERSONALIZATION_SECURE,
                                               request_id,
                                               errval,
                                               &response);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_lock_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_ACTIVATE_CONFIG_RESP

  DESCRIPTION
    Processes the response of the activate config command

  PARAMETERS
    status          : Status of the lock request
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_activate_config_resp
(
  simlock_result_enum_type       status,
  qmi_uimi_userdata_type       * user_data_ptr
)
{
  qmi_cmd_buf_type          * cmd_buf_p           = NULL;
  dsm_item_type             * response            = NULL;
  qmi_error_e_type            errval              = QMI_ERR_NONE;
  uint8                       clid                = QMI_SVC_CLID_UNUSED;
  uint32                      client_reg_count    = 0;
  UIM_MSG_HIGH_0("qmi_uim_simlock_process_activate_config_resp");

  ASSERT( user_data_ptr );

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

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

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_activate_config_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_GET_TOKEN_RESP

  DESCRIPTION
    Processes the response of the activate config command

  PARAMETERS
    status          : Status of the lock request
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_get_token_resp
(
  simlock_result_enum_type          status,
  simlock_get_token_msg_resp_type * get_token_resp_ptr,
  qmi_uimi_userdata_type          * user_data_ptr
)
{
  qmi_cmd_buf_type          * cmd_buf_p           = NULL;
  dsm_item_type             * response            = NULL;
  qmi_error_e_type            errval              = QMI_ERR_NONE;
  uint8                       clid                = QMI_SVC_CLID_UNUSED;
  uint32                      client_reg_count    = 0;
  uint16                      offset              = 0;
  uint8                     * tlv_data_ptr        = NULL;
  uint8                       slot_cnt            = 0;
  uint16                      tlv_len             = 0;
  uint8                       slot_index          = 0;
  uint8                       feature_index       = 0;
  uint8                       number_of_tokens    = 0;

  PACKED struct PACKED_POST
  {
    uint8 token_len;
    uint8 token[QMI_UIM_TOKEN_LEN_V01];
  } feature_token;

  UIM_MSG_HIGH_0("qmi_uim_simlock_process_get_token_resp");

  ASSERT( user_data_ptr );

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

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

  slot_cnt = MIN(qmi_uim_global_ptr->card_state.num_slots, QMI_UIM_MAX_CARD_COUNT);

  for (slot_index = 0;
       slot_index < SIMLOCK_SLOT_COUNT_MAX && slot_index < slot_cnt;
       slot_index++)
  {
    for(feature_index = 0;
        feature_index < SIMLOCK_CATEGORY_COUNT_MAX && feature_index < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        feature_index++)
    {
      if (get_token_resp_ptr->slot_token_data[slot_index].category_token_data[feature_index].token_present)
      {
        number_of_tokens++;
      }
    }
  }

  /* calculate the total length */
  tlv_len = sizeof(uint8) +
            sizeof(uint8) * slot_cnt +
            sizeof(uint8) * slot_cnt * QMI_UIM_MAX_NUMBER_PERSO_FEATURES +
            number_of_tokens * QMI_UIM_TOKEN_LEN_V01;

  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return;
  }

  /* Insert the number of slots in the TLV data */
  tlv_data_ptr[offset++] = slot_cnt;

  for (slot_index = 0;
       slot_index < SIMLOCK_SLOT_COUNT_MAX && slot_index < slot_cnt;
       slot_index++)
  {
    ASSERT(offset + 1 <= tlv_len);

    /* Insert the number of category for slot */
    tlv_data_ptr[offset++] = SIMLOCK_CATEGORY_COUNT_MAX;

    for(feature_index = 0;
        feature_index < SIMLOCK_CATEGORY_COUNT_MAX && feature_index < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        feature_index++)
    {
      if (get_token_resp_ptr->slot_token_data[slot_index].category_token_data[feature_index].token_present)
      {
        ASSERT((offset + sizeof(feature_token)) <= tlv_len);

        tlv_data_ptr[offset++] = SIMLOCK_TOKEN_LEN;

        (void)memscpy((void*)(tlv_data_ptr + offset),
                      tlv_len - offset,
                      get_token_resp_ptr->slot_token_data[slot_index].category_token_data[feature_index].token_data,
                      SIMLOCK_TOKEN_LEN);
        offset += SIMLOCK_TOKEN_LEN;
      }
      else
      {
        ASSERT(offset + 1 <= tlv_len);
        tlv_data_ptr[offset++] = 0;
      }
    }
  }

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);

  /* if number of tokens is zero, send invalid response in error type*/
  if(errval == QMI_ERR_NONE &&
     number_of_tokens == 0)
  {
    errval = QMI_ERR_INVALID_OPERATION;
  }

  if (FALSE == qmi_svc_put_param_tlv(&response,
                                     UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                     tlv_len,
                                     (void *)tlv_data_ptr))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  uimqmi_free(tlv_data_ptr);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_get_token_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_GET_STATUS_RESP_IN_PERSO_SECURE

  DESCRIPTION
    Processes the response to the SIM Lock get status command which was
    requested in the perso secure message

  PARAMETERS
    status          : Status of the get_status request
    get_status_ptr  : Pointer to the response data
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_get_status_resp_in_perso_secure
(
  simlock_result_enum_type                  status,
  const simlock_get_status_msg_resp_type  * get_status_ptr,
  qmi_uimi_userdata_type                  * user_data_ptr
)
{
  simlock_result_enum_type  simlock_status = SIMLOCK_SUCCESS;

  ASSERT(get_status_ptr && user_data_ptr);

  UIM_MSG_MED_1("Get status response, status:0x%x", status);

  if((status == SIMLOCK_SUCCESS) &&
     (user_data_ptr->data.simlock_lock.code_ptr != NULL))
  {
    simlock_category_enum_type      category       = SIMLOCK_CATEGORY_3GPP_NW;
    mmgsdi_perso_feature_enum_type  feature        = MMGSDI_MAX_PERSO_FEATURE_ENUM;
    simlock_slot_enum_type          slot           = SIMLOCK_SLOT_1;
    mmgsdi_slot_id_enum_type        mmgsdi_slot_id = MMGSDI_SLOT_1;
    uint8                           slot_index     = 0;
    uint8                           category_index = 0;

    slot     = user_data_ptr->data.simlock_lock.slot;
    category = user_data_ptr->data.simlock_lock.code_ptr->category_type;

    if((qmi_uim_simlock_simlock_slot_id_to_mmgsdi_slot_id(slot, &mmgsdi_slot_id) == QMI_ERR_NONE) &&
       (uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot_id,
                                         &slot_index,
                                         qmi_uim_global_ptr->card_state.num_slots) == QMI_ERR_NONE) &&
       (qmi_uim_simlock_convert_simlock_category(category, &feature, &category_index)))
    {
      if((slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
         (category_index < SIMLOCK_CATEGORY_COUNT_MAX))
      {
        /* category is already enabled, update the code data */
        if(get_status_ptr->lock_info[slot_index].category_info[category_index].category_status)
        {
          simlock_add_lock_codes_ck_msg_req_type   *add_lock_codes_ptr = NULL;

          add_lock_codes_ptr =
            (simlock_add_lock_codes_ck_msg_req_type *)uimqmi_malloc(sizeof(simlock_add_lock_codes_ck_msg_req_type));

          if(add_lock_codes_ptr != NULL)
          {
            add_lock_codes_ptr->slot      = slot;
            add_lock_codes_ptr->blacklist = FALSE;
            add_lock_codes_ptr->category  = *user_data_ptr->data.simlock_lock.code_ptr;

            /* The lock codes contained in zeroth list are added here. So
               mark the next list to lock as 1 */
            user_data_ptr->data.simlock_lock.next_list_to_lock    = 1;

            simlock_status = simlock_add_lock_codes_ck(add_lock_codes_ptr,
                                                       qmi_uimi_simlock_callback,
                                                       user_data_ptr);

            uimqmi_free(add_lock_codes_ptr);
          }
        }
        else
        {
          /* category is not enabled, enable it now */
          simlock_set_lock_ck_msg_req_type   *lock_msg_ptr = NULL;

          lock_msg_ptr = (simlock_set_lock_ck_msg_req_type *)uimqmi_malloc(sizeof(simlock_set_lock_ck_msg_req_type));
          if(lock_msg_ptr != NULL)
          {
            /* Set default lock values */
            lock_msg_ptr->auto_lock        = FALSE;
            lock_msg_ptr->blacklist        = FALSE;

            /* Set iteration count and number of max retries to 0 since there is
               no chance of unlock failure as there is no CK */
            lock_msg_ptr->iteration_cnt    = 0;
            lock_msg_ptr->num_retries_max  = 0;
            lock_msg_ptr->slot             = slot;
            lock_msg_ptr->sfs_type         = SIMLOCK_SFS_REMOTE;

            if(user_data_ptr->data.simlock_lock.code_ptr != NULL)
            {
              lock_msg_ptr->category         = *user_data_ptr->data.simlock_lock.code_ptr;
            }

            /* The lock codes contained in zeroth list are locked here. So
               mark the next list to lock as 1 */
            user_data_ptr->data.simlock_lock.next_list_to_lock    = 1;

            simlock_status = simlock_ck_lock(lock_msg_ptr,
                                             qmi_uimi_simlock_callback,
                                             user_data_ptr);
            uimqmi_free(lock_msg_ptr);
          }
        }
        if(simlock_status == SIMLOCK_SUCCESS)
        {
          return;
        }
      }
    }
  }

  /* Indicates there was error in response */
  if(status != SIMLOCK_SUCCESS)
  {
    simlock_status = status;
  }
  else if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Nothing to do */
  }
  else
  {
    simlock_status = SIMLOCK_GENERIC_ERROR;
  }

  /* There was an error, so send response to the client to
     simulate as if there was an error while enabling the
     lock */
  qmi_uim_simlock_process_lock_resp(simlock_status, user_data_ptr);
} /* qmi_uim_simlock_process_get_status_resp_in_perso_secure */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_RELOCK_RESP

  DESCRIPTION
    Processes the response of the relock personalization command

  PARAMETERS
    status          : Status of the relock request
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_relock_resp
(
  simlock_result_enum_type                      status,
  const simlock_relock_device_msg_resp_type   * relock_rsp_ptr,
  qmi_uimi_userdata_type                      * user_data_ptr
)
{
  qmi_cmd_buf_type          * cmd_buf_p           = NULL;
  dsm_item_type             * response            = NULL;
  qmi_error_e_type            errval              = QMI_ERR_NONE;
  uint8                       clid                = QMI_SVC_CLID_UNUSED;
  uint32                      client_reg_count    = 0;

  UIM_MSG_HIGH_0("qmi_uim_simlock_process_relock_resp");

  ASSERT(relock_rsp_ptr && user_data_ptr);

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

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

  /* For relock, retries are sent in these cases */
  if ((status == SIMLOCK_SUCCESS)       ||
      (status == SIMLOCK_INCORRECT_KEY) ||
      (status == SIMLOCK_MAX_RETRIES_REACHED))
  {
    uint32   num_retries = 0;

    if (relock_rsp_ptr->num_retries_max == 0)
    {
      num_retries = QMI_UIM_MAX_RETRIES;
    }
    else if (relock_rsp_ptr->curr_retries >= relock_rsp_ptr->num_retries_max)
    {
      num_retries = 0;
    }
    else
    {
      num_retries = relock_rsp_ptr->num_retries_max - relock_rsp_ptr->curr_retries;
    }

    /* Send the num_retries in the response */
    if (FALSE == qmi_uim_response_retries_left(
                  UIMI_TLV_RESP_TAG_OPTIONAL_1,
                  &response,
                  num_retries,
                  0))
    {
      dsm_free_packet(&response);
    }
  }

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_relock_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_SET_DEVICE_MODE_RESP

  DESCRIPTION
    Processes the response of the set device mode command

  PARAMETERS
    status          : Status of the set device mode request
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_simlock_process_set_device_mode_resp
(
  simlock_result_enum_type       status,
  qmi_uimi_userdata_type       * user_data_ptr
)
{
  qmi_cmd_buf_type          * cmd_buf_p           = NULL;
  dsm_item_type             * response            = NULL;
  qmi_error_e_type            errval              = QMI_ERR_NONE;
  uint8                       clid                = QMI_SVC_CLID_UNUSED;
  uint32                      client_reg_count    = 0;
  uint16                      request_id          = 0;

  ASSERT( user_data_ptr );

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;
  request_id       = user_data_ptr->data.simlock_set_device_mode.request_id;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

  UIM_MSG_HIGH_0("qmi_uim_process_set_device_mode_resp");

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

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);

  /* build the result and encrypt the data */
  errval = qmi_uim_response_encrypted_status(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                             UIMI_CMD_VAL_EMERGENCY_ONLY,
                                             request_id,
                                             errval,
                                             &response);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_set_device_mode_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_SET_REMOTE_SFS_RESP

  DESCRIPTION
    Processes the response of the set remote SFS command

  PARAMETERS
    status          : Status of the set remote SFS request
    user_data_ptr   : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_simlock_process_set_remote_sfs_resp
(
  simlock_result_enum_type       status,
  qmi_uimi_userdata_type       * user_data_ptr
)
{
  qmi_cmd_buf_type          * cmd_buf_p           = NULL;
  dsm_item_type             * response            = NULL;
  qmi_error_e_type            errval              = QMI_ERR_NONE;
  uint8                       clid                = QMI_SVC_CLID_UNUSED;
  uint32                      client_reg_count    = 0;
  uint16                      request_id          = 0;

  ASSERT( user_data_ptr );

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;
  request_id       = user_data_ptr->data.simlock_set_remote_sfs.request_id;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

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

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(status);

  /* build the result and encrypt the data */
  errval = qmi_uim_response_encrypted_status(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                             UIMI_CMD_VAL_SIMLOCK_CONFIGURATION,
                                             request_id,
                                             errval,
                                             &response);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_set_remote_sfs_resp */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SIMLOCK_BLOB_VERSION()

  DESCRIPTION
    Composes the TLV for SIMLOCK blob version

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    version             : blob major and minor version

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_response_simlock_blob_version
(
  byte                             tlv_id,
  dsm_item_type                  **response_pptr,
  simlock_rsu_version_type         version
)
{
  PACKED struct PACKED_POST
  {
    uint8 major;
    uint8 minor;
  } tlv_version;

  ASSERT(response_pptr);

  tlv_version.major = version.major;
  tlv_version.minor = version.minor;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_version),
                               (void *)&tlv_version);
} /* qmi_uim_response_simlock_blob_version */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SIMLOCK_DELAY_TIMER_VALUE()

  DESCRIPTION
    Composes the TLV for SIMLOCK delay timer value

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    delay_timer_value   : SimLock delay timer value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_response_simlock_delay_timer_value
(
  byte                             tlv_id,
  dsm_item_type                  **response_pptr,
  uint16                           delay_timer_value
)
{
  ASSERT(response_pptr);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(delay_timer_value),
                               (void *)&delay_timer_value);
} /* qmi_uim_response_simlock_delay_timer_value */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_REMOTE_UNLOCK_RESP

  DESCRIPTION
    Processes the response of the remote unlock command

  PARAMETERS
    status             : Status of the remote unlock request
    encrypted_key_ptr  : Pointer to encrypted key data
    user_data_ptr      : Pointer to the user data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_simlock_process_remote_unlock_resp
(
  qmi_uim_message_type * uim_message_ptr
)
{
  qmi_cmd_buf_type          * cmd_buf_p           = NULL;
  dsm_item_type             * response            = NULL;
  qmi_error_e_type            errval              = QMI_ERR_NONE;
  uint8                       clid                = QMI_SVC_CLID_UNUSED;
  uint32                      client_reg_count    = 0;
  qmi_uimi_userdata_type    * user_data_ptr       = NULL;
  boolean                     result              = TRUE;
  mmgsdi_data_type            rsu_resp_data;

  ASSERT(uim_message_ptr);

  memset(&rsu_resp_data, 0x00, sizeof(rsu_resp_data));

  user_data_ptr = (qmi_uimi_userdata_type*)uim_message_ptr->data.remote_unlock_resp.user_data_ptr;
  if(user_data_ptr == NULL)
  {
    return;
  }

  /* Retrieve parameters */
  clid             = user_data_ptr->clid;
  client_reg_count = user_data_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)user_data_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(user_data_ptr);
  user_data_ptr = NULL;

    /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count) ||
      cmd_buf_p == NULL)
  {
    qmi_uim_simlock_rsu_resp_free(&(uim_message_ptr->data.remote_unlock_resp.data));
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(
             uim_message_ptr->data.remote_unlock_resp.status);

  switch(uim_message_ptr->data.remote_unlock_resp.data.msg_type)
  {
    case SIMLOCK_RSU_GET_KEY_MSG:
      result = FALSE;
      if(uim_message_ptr->data.remote_unlock_resp.data.message.get_key_resp_data.data_len > 0 &&
         uim_message_ptr->data.remote_unlock_resp.data.message.get_key_resp_data.data_ptr != NULL)
      {
        rsu_resp_data.data_ptr = uim_message_ptr->data.remote_unlock_resp.data.message.get_key_resp_data.data_ptr;
        rsu_resp_data.data_len = (mmgsdi_len_type)uim_message_ptr->data.remote_unlock_resp.data.message.get_key_resp_data.data_len;
        result = qmi_uim_response_generic_payload_16bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &response,
                   rsu_resp_data,
                   TRUE);
      }
      break;
    case SIMLOCK_RSU_SET_CONFIGURATION_MSG:
      if(uim_message_ptr->data.remote_unlock_resp.data.message.set_config_resp_data.data_len > 0 &&
         uim_message_ptr->data.remote_unlock_resp.data.message.set_config_resp_data.data_ptr != NULL)
      {
        rsu_resp_data.data_ptr = uim_message_ptr->data.remote_unlock_resp.data.message.set_config_resp_data.data_ptr;
        rsu_resp_data.data_len = (mmgsdi_len_type)uim_message_ptr->data.remote_unlock_resp.data.message.set_config_resp_data.data_len;
        result = qmi_uim_response_generic_payload_16bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_2,
                   &response,
                   rsu_resp_data,
                   FALSE);
      }
      break;
    case SIMLOCK_RSU_GENERATE_HASH_MSG:
      result = FALSE;
      if(uim_message_ptr->data.remote_unlock_resp.data.message.generate_hash_resp_data.data_len > 0 &&
         uim_message_ptr->data.remote_unlock_resp.data.message.generate_hash_resp_data.data_ptr != NULL)
      {
        rsu_resp_data.data_ptr = uim_message_ptr->data.remote_unlock_resp.data.message.generate_hash_resp_data.data_ptr;
        rsu_resp_data.data_len = (mmgsdi_len_type)uim_message_ptr->data.remote_unlock_resp.data.message.generate_hash_resp_data.data_len;
        result = qmi_uim_response_generic_payload_8bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_3,
                   &response,
                   rsu_resp_data,
                   TRUE);
      }
      break;
    case SIMLOCK_RSU_RETRIEVE_VERSION_MSG:
      result = qmi_uim_response_simlock_blob_version(
                 UIMI_TLV_RESP_TAG_OPTIONAL_4,
                 &response,
                 uim_message_ptr->data.remote_unlock_resp.data.message.version);
      break;
    case SIMLOCK_RSU_BLOB_REQUEST_MSG:
      if(uim_message_ptr->data.remote_unlock_resp.data.message.generate_simlock_blob_resp_data.data_len > 0 &&
         uim_message_ptr->data.remote_unlock_resp.data.message.generate_simlock_blob_resp_data.data_ptr != NULL)
      {
        rsu_resp_data.data_ptr = uim_message_ptr->data.remote_unlock_resp.data.message.generate_simlock_blob_resp_data.data_ptr;
        rsu_resp_data.data_len = (mmgsdi_len_type)uim_message_ptr->data.remote_unlock_resp.data.message.generate_simlock_blob_resp_data.data_len;
        result = qmi_uim_response_generic_payload_16bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_5,
                   &response,
                   rsu_resp_data,
                   TRUE);
      }
      break;
    case SIMLOCK_RSU_DELAY_TIMER_START_REQUEST_MSG:
      (void)qmi_uim_response_simlock_delay_timer_value(
                    UIMI_TLV_RESP_TAG_OPTIONAL_6,
                    &response,
                    uim_message_ptr->data.remote_unlock_resp.data.message.delay_timer_value);
      break;
    case SIMLOCK_RSU_DELAY_TIMER_STOP_REQUEST_MSG:
      /* No data to include in the response */
      break;
    default:
      result = FALSE;
      break;
  }

  if(FALSE == result)
  {
    errval = QMI_ERR_INTERNAL;
    dsm_free_packet(&response);
  }

  qmi_uim_simlock_rsu_resp_free(&(uim_message_ptr->data.remote_unlock_resp.data));

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_simlock_process_remote_unlock_resp */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_PROCESS_SIMLOCK_CB

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
void qmi_uim_simlock_process_simlock_cb
(
  qmi_uim_message_type * uim_message_ptr
)
{
  qmi_uimi_userdata_type * cb_userdata_ptr = NULL;

  ASSERT(uim_message_ptr);

  UIM_MSG_HIGH_1("qmi_uim_simlock_process_simlock_cb, msg_type: 0x%x",
                 uim_message_ptr->data.simlock.data.msg_type);

  switch (uim_message_ptr->data.simlock.data.msg_type)
  {
    case SIMLOCK_UNLOCK_DEVICE_MSG:
    case SIMLOCK_TEMPORARY_UNLOCK_MSG:
      qmi_uim_simlock_process_unlock_resp(uim_message_ptr->data.simlock.status,
                                          &uim_message_ptr->data.simlock.data.message.unlock_resp,
                                          (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      break;

    case SIMLOCK_GET_STATUS_MSG:
      cb_userdata_ptr = (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr;
      if (cb_userdata_ptr == NULL)
      {
        return;
      }
      if(cb_userdata_ptr->request_id == UIMI_CMD_VAL_GET_CONFIGURATION)
      {
        qmi_uim_simlock_process_get_status_resp(uim_message_ptr->data.simlock.status,
                                                &uim_message_ptr->data.simlock.data.message.get_status_resp,
                                                (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      }
      else if (cb_userdata_ptr->request_id == UIMI_CMD_VAL_DEPERSONALIZATION)
      {
        qmi_uim_simlock_process_get_status_resp_after_unlock(uim_message_ptr->data.simlock.status,
                                                             &uim_message_ptr->data.simlock.data.message.get_status_resp,
                                                             (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      }
      else if(cb_userdata_ptr->request_id == UIMI_CMD_VAL_PERSONALIZATION_SECURE)
      {
        qmi_uim_simlock_process_get_status_resp_in_perso_secure(uim_message_ptr->data.simlock.status,
                                                                &uim_message_ptr->data.simlock.data.message.get_status_resp,
                                                                (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      }
      break;

    case SIMLOCK_SET_LOCK_CK_MSG:
    case SIMLOCK_ADD_LOCK_CODES_CK_MSG:
      qmi_uim_simlock_process_lock_resp(uim_message_ptr->data.simlock.status,
                                        (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      break;

    case SIMLOCK_SET_DEVICE_MODE_MSG:
      qmi_uim_simlock_process_set_device_mode_resp(uim_message_ptr->data.simlock.status,
                                                   (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      break;

    case SIMLOCK_RELOCK_DEVICE_MSG:
      qmi_uim_simlock_process_relock_resp(uim_message_ptr->data.simlock.status,
                                          &uim_message_ptr->data.simlock.data.message.relock_resp,
                                         (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      break;

    case SIMLOCK_ACTIVATE_CONFIG_MSG:
      qmi_uim_simlock_process_activate_config_resp(uim_message_ptr->data.simlock.status,
                                                   (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      break;

    case SIMLOCK_GET_TOKEN_MSG:
        qmi_uim_simlock_process_get_token_resp(uim_message_ptr->data.simlock.status,
                                               &uim_message_ptr->data.simlock.data.message.get_token_resp,
                                               (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock.user_data_ptr);
      break;

    default:
      break;
  }
} /* qmi_uim_simlock_process_simlock_cb */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_PROCESS_REMOTE_SFS_CB()

  DESCRIPTION
    This function processes the callback from simlock task. It indicates
    either the simlock data needs to be read from or write to a remote
    storage location to all registered QMI clients.

  PARAMETERS
    uim_message_ptr: QMI UIM message

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_simlock_process_remote_sfs_cb
(
  const qmi_uim_message_type * uim_message_ptr
)
{
  boolean is_indication_sent = FALSE;

  ASSERT(uim_message_ptr != NULL && qmi_uim_global_ptr);

  is_indication_sent = qmi_uim_simlock_send_remote_sfs_operation_indication_to_all(uim_message_ptr);

  if (is_indication_sent)
  {
    /* Save the simlock cb. In case the simlock cb is not NULL,
       indicating a response from TZ is pending, we ignore and
       overwrite it. The expectation is the TZ should respond
       to each operation request in requested order.*/
    qmi_uim_global_ptr->simlock_remote_sfs_operation_resp_cb =
      uim_message_ptr->data.simlock_remote_sfs_req.simlock_resp_cb;
  }
  else
  {
    UIM_MSG_ERR_0("simlock_remote_sfs indication was not sent");

    /* No indication was sent, invoke the callback with error status */
    if (uim_message_ptr->data.simlock_remote_sfs_req.simlock_resp_cb)
    {
      uim_message_ptr->data.simlock_remote_sfs_req.simlock_resp_cb(SIMLOCK_GENERIC_ERROR,
                                                                   uim_message_ptr->data.simlock_remote_sfs_req.request_token_id,
                                                                   NULL);
    }
  }

  if (uim_message_ptr->data.simlock_remote_sfs_req.simlock_data.data_ptr != NULL)
  {
    uimqmi_free(uim_message_ptr->data.simlock_remote_sfs_req.simlock_data.data_ptr);
  }
} /* qmi_uim_simlock_process_remote_sfs_cb */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_PROCESS_TEMPORARY_UNLOCK_EVT()

  DESCRIPTION
    This function processes the Temporary unlock event from MMGSDI

  PARAMETERS
    evt_ptr  : Contains the event information sent by MMGSDI

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_simlock_process_temporary_unlock_event(
  const mmgsdi_event_data_type *evt_ptr
)
{
  simlock_result_enum_type   simlock_status = SIMLOCK_GENERIC_ERROR;
  if(evt_ptr == NULL ||
     evt_ptr->evt != MMGSDI_PERSO_TEMPORARY_UNLOCK_EVT)
  {
    return;
  }

  UIM_MSG_MED_2("qmi_uim_simlock_process_temporary_unlock_event slot 0x%x feature 0x%x",
                 evt_ptr->data.perso_temp_unlock.slot_id,
                 evt_ptr->data.perso_temp_unlock.feature);

  /* Get the current lock status from SIMLOCK */
  simlock_status = simlock_get_status(qmi_uimi_simlock_temporary_unlock_callback,
                                      NULL);
  if(SIMLOCK_SUCCESS != simlock_status)
  {
    UIM_MSG_MED_1("Failed to queue simlock get status command 0x%x", simlock_status);
  }
} /* qmi_uim_simlock_process_temporary_unlock_event */


/*===========================================================================
  FUNCTION QMI_UIMI_DEPERSONALIZATION()

  DESCRIPTION
    Deactivates or Unblocks depersonalization

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
dsm_item_type * qmi_uimi_depersonalization
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                     errval          = QMI_ERR_NONE;
  qmi_uimi_userdata_type*              cb_userdata_ptr = NULL;
  qmi_uim_tlv_item_type                tlv_data[2];
  simlock_result_enum_type             simlock_status  = SIMLOCK_SUCCESS;
  simlock_unlock_device_msg_req_type   unlock_msg;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_DEPERSONALIZATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  memset(&unlock_msg, 0x00, sizeof(unlock_msg));

  unlock_msg.category  = tlv_data[0].data.depersonalization.category;
  unlock_msg.ck        = tlv_data[0].data.depersonalization.ck;
  unlock_msg.ignore_ck = FALSE;
  unlock_msg.probe_ck  = FALSE;

  if (tlv_data[1].present)
  {
    if(qmi_uim_simlock_mmgsdi_slot_id_to_simlock_slot_id(tlv_data[1].data.slot.slot,
                                                         &unlock_msg.slot) != QMI_ERR_NONE)
    {
      errval = QMI_ERR_INTERNAL;
      goto send_result;
    }
  }
  else
  {
    /* default to slot-1 */
    unlock_msg.slot = SIMLOCK_SLOT_1;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_DEPERSONALIZATION,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.simlock_unlock.category      = unlock_msg.category;
  cb_userdata_ptr->data.simlock_unlock.slot          = unlock_msg.slot;
  cb_userdata_ptr->data.simlock_unlock.ck_present    = TRUE;
  /* Set request id to dummy value as it will not be used for regular depersonalization */
  cb_userdata_ptr->data.simlock_unlock.request_id    = 0;

  /* Unlock the SIM */
  simlock_status = simlock_unlock(unlock_msg,
                                  qmi_uimi_simlock_callback,
                                  cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_SIMLOCK_FREE_USERDATA_IF_ERROR(simlock_status, cb_userdata_ptr);

  /* Convert MMGSDI status into QMI err code */
  errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);

send_result:
  /* Free the buffer used for data: this also clears the CK
     information for security reason */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_depersonalization() */


/*===========================================================================
  FUNCTION QMI_UIMI_PERSONALIZATION()

  DESCRIPTION
    Activates the personalization of the device to a particular lock feature.
    Only one feature can be activated at a time

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
dsm_item_type * qmi_uimi_personalization
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                     errval             = QMI_ERR_NONE;
  qmi_uimi_userdata_type  *            cb_userdata_ptr    = NULL;
  simlock_result_enum_type             simlock_status     = SIMLOCK_GENERIC_ERROR;
  simlock_slot_enum_type               simlock_slot       = SIMLOCK_SLOT_1;
  uint8                                index              = 0;
  uint8                                code_index         = 0;
  qmi_uim_tlv_item_type                tlv_data[21];
  uint8                                loop_cnt           = 0;
  uint8                                table_indexes[]    = {1, 2, 3, 4, 5, 6, 7, 8, 14, 15, 16, 17, 18, 19, 20};

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_PERSONALIZATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_GW_NW_PERSO_CODE;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[2].type      = UIMI_TLV_TYPE_GW_NS_PERSO_CODE;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[3].type      = UIMI_TLV_TYPE_GW_SP_PERSO_CODE;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_3;
  tlv_data[4].type      = UIMI_TLV_TYPE_GW_CP_PERSO_CODE;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_4;
  tlv_data[5].type      = UIMI_TLV_TYPE_GW_SIM_PERSO_CODE;
  tlv_data[5].tag       = UIMI_TLV_TAG_OPTIONAL_5;
  tlv_data[6].type      = UIMI_TLV_TYPE_1X_NW_TYPE1_PERSO_CODE;
  tlv_data[6].tag       = UIMI_TLV_TAG_OPTIONAL_6;
  tlv_data[7].type      = UIMI_TLV_TYPE_1X_NW_TYPE2_PERSO_CODE;
  tlv_data[7].tag       = UIMI_TLV_TAG_OPTIONAL_7;
  tlv_data[8].type      = UIMI_TLV_TYPE_1X_RUIM_PERSO_CODE;
  tlv_data[8].tag       = UIMI_TLV_TAG_OPTIONAL_8;
  tlv_data[9].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[9].tag       = UIMI_TLV_TAG_OPTIONAL_9;
  tlv_data[10].type     = UIMI_TLV_TYPE_PERSO_ITERATION_CNT;
  tlv_data[10].tag      = UIMI_TLV_TAG_OPTIONAL_10;
  tlv_data[11].type     = UIMI_TLV_TYPE_PERSO_UNLOCK_RETRIES_CNT;
  tlv_data[11].tag      = UIMI_TLV_TAG_OPTIONAL_11;
  tlv_data[12].type     = UIMI_TLV_TYPE_PERSO_REACTIVATE_FEATURE;
  tlv_data[12].tag      = UIMI_TLV_TAG_OPTIONAL_12;
  tlv_data[13].type     = UIMI_TLV_TYPE_PERSO_REUSE_CODE_DATA;
  tlv_data[13].tag      = UIMI_TLV_TAG_OPTIONAL_13;
  tlv_data[14].type     = UIMI_TLV_TYPE_GW_SP_EXT_PERSO_CODE;
  tlv_data[14].tag      = UIMI_TLV_TAG_OPTIONAL_14;
  tlv_data[15].type     = UIMI_TLV_TYPE_GW_CP_EXT_PERSO_CODE;
  tlv_data[15].tag      = UIMI_TLV_TAG_OPTIONAL_15;
  tlv_data[16].type     = UIMI_TLV_TYPE_GW_SPN_PERSO_CODE;
  tlv_data[16].tag      = UIMI_TLV_TAG_OPTIONAL_16;
  tlv_data[17].type     = UIMI_TLV_TYPE_GW_SP_EXT2_PERSO_CODE;
  tlv_data[17].tag      = UIMI_TLV_TAG_OPTIONAL_17;
  tlv_data[18].type     = UIMI_TLV_TYPE_GW_CP_EXT2_PERSO_CODE;
  tlv_data[18].tag      = UIMI_TLV_TAG_OPTIONAL_18;
  tlv_data[19].type     = UIMI_TLV_TYPE_GW_SP_EHPLMN_PERSO_CODE;
  tlv_data[19].tag      = UIMI_TLV_TAG_OPTIONAL_19;
  tlv_data[20].type     = UIMI_TLV_TYPE_GW_ICCID_PERSO_CODE;
  tlv_data[20].tag      = UIMI_TLV_TAG_OPTIONAL_20;

  /* Parse TLVs */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 21);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_PERSONALIZATION,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Extract slot if present */
  if(tlv_data[9].present)
  {
    if(qmi_uim_simlock_mmgsdi_slot_id_to_simlock_slot_id(tlv_data[9].data.slot.slot,
                                                         &simlock_slot) != QMI_ERR_NONE)
    {
      errval = QMI_ERR_INTERNAL;
      goto send_result;
    }
  }

  /* Check what all optional perso data TLVs were received */
  for(loop_cnt = 0; loop_cnt < sizeof(table_indexes); loop_cnt++)
  {
    index = table_indexes[loop_cnt];
    if(!tlv_data[index].present)
    {
      continue;
    }

    if(code_index != 0)
    {
      errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
      goto send_result;
    }

    code_index = index;
  }

#ifdef FEATURE_QMI_UIM_PERSONALIZATION
  /* If personalization is allowed, we can either:
     Activate SIMLock - by receiving only 1 Lock code TLV per request, or
     Re-activate SIMLock - by receiving a reactivate TLV & no Lock code TLVs or
     Re-use code - by receiving a reuse code data TLV & no lock code TLV */
  if ((code_index != 0 && tlv_data[12].present) ||
      (code_index != 0 && tlv_data[13].present) ||
      (code_index == 0 && (!tlv_data[12].present && !tlv_data[13].present)))
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
    goto send_result;
  }
#else
  /* If personalization is not allowed, we can only do a Relock functionality
     by receiving a reactivate TLV & no Lock code TLVs */
  if (code_index != 0 || !tlv_data[12].present)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
    goto send_result;
  }
#endif /* FEATURE_QMI_UIM_PERSONALIZATION */

  /* Unless the perso feature is defined, this request is unsupported */
  errval = QMI_ERR_NOT_SUPPORTED;

  if (tlv_data[12].present)
  {
    simlock_relock_device_msg_req_type   relock_msg;

    memset(&relock_msg, 0x00, sizeof(simlock_relock_device_msg_req_type));

    relock_msg.ck            = tlv_data[0].data.personalization_ck.ck;
    relock_msg.slot          = simlock_slot;
    relock_msg.category_type = tlv_data[12].data.personalization_reactivate.category;
    relock_msg.sfs_type      = SIMLOCK_SFS_LOCAL;

    /* Send the relock API call */
    simlock_status = simlock_relock(relock_msg,
                                    qmi_uimi_simlock_callback,
                                    cb_userdata_ptr);

    /* Convert MMGSDI status into QMI err code */
    errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);
  }
#ifdef FEATURE_QMI_UIM_PERSONALIZATION
  else
  {
    simlock_set_lock_ck_msg_req_type  *lock_ck_msg_ptr = NULL;

    lock_ck_msg_ptr =
      (simlock_set_lock_ck_msg_req_type *)uimqmi_malloc(sizeof(simlock_set_lock_ck_msg_req_type));
    if(lock_ck_msg_ptr == NULL)
    {
      errval = QMI_ERR_NO_MEMORY;
      goto send_result;
    }

    lock_ck_msg_ptr->ck          = tlv_data[0].data.personalization_ck.ck;

    if (tlv_data[13].present)
    {
      lock_ck_msg_ptr->category.category_type = tlv_data[13].data.personalization_reuse_code.category;
      lock_ck_msg_ptr->reuse_previous_code_list = TRUE;
    }
    else
    {
      (void)memscpy(&lock_ck_msg_ptr->category,
                    sizeof(simlock_category_data_type),
                    tlv_data[code_index].data.personalization_code.code_ptr,
                    sizeof(simlock_category_data_type));

      lock_ck_msg_ptr->reuse_previous_code_list = FALSE;
    }

    lock_ck_msg_ptr->auto_lock   = FALSE;
    lock_ck_msg_ptr->blacklist   = FALSE;
    lock_ck_msg_ptr->slot        = simlock_slot;
    lock_ck_msg_ptr->sfs_type    = SIMLOCK_SFS_LOCAL;

    /* If the TLV is missing, the SIMLock engine sets a default
       value to the iteration count */
    if(tlv_data[10].present)
    {
      lock_ck_msg_ptr->iteration_cnt = tlv_data[10].data.personalization_iteration_cnt.value;
    }

    /* If the TLV is missing, unlimited number of unlock retry
       attempts are allowed */
    if(tlv_data[11].present)
    {
      lock_ck_msg_ptr->num_retries_max  = tlv_data[11].data.personalization_retries_cnt.value;
    }

    cb_userdata_ptr->data.simlock_lock.ck_present = TRUE;

    /* Initialize the list counts */
    cb_userdata_ptr->data.simlock_lock.total_list_cnt = 0;
    cb_userdata_ptr->data.simlock_lock.next_list_to_lock = 0;

    if (!tlv_data[13].present)
    {
      /* Set request id to dummy value as it will not be used for regular personalization */
      cb_userdata_ptr->data.simlock_lock.request_id           = 0;
      cb_userdata_ptr->data.simlock_lock.total_list_cnt       = tlv_data[code_index].data.personalization_code.num_of_lists;
      cb_userdata_ptr->data.simlock_lock.slot                 = lock_ck_msg_ptr->slot;
      cb_userdata_ptr->data.simlock_lock.ck                   = tlv_data[0].data.personalization_ck.ck;

      /* The lock codes contained in zeroth list are locked here. So
       mark the next list to lock as 1 */
      cb_userdata_ptr->data.simlock_lock.next_list_to_lock    = 1;

      /* Save the lock code data pointer */
      cb_userdata_ptr->data.simlock_lock.code_ptr             = tlv_data[code_index].data.personalization_code.code_ptr;

      /* set the original pointer to NULL */
      tlv_data[code_index].data.personalization_code.code_ptr = NULL;
    }

    /* lock the SIM */
    simlock_status = simlock_ck_lock(lock_ck_msg_ptr,
                                     qmi_uimi_simlock_callback,
                                     cb_userdata_ptr);

    /* Convert MMGSDI status into QMI err code */
    errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);

    uimqmi_free(lock_ck_msg_ptr);
  }
#endif /* FEATURE_QMI_UIM_PERSONALIZATION */

send_result:
  /* Free the buffer used for data: this also clears the CK
     information for security reason */
  qmi_uim_util_free_tlv_content(tlv_data, 21);

  /* Free userdata in case of error */
  QMI_UIM_SIMLOCK_FREE_USERDATA_IF_ERROR(simlock_status, cb_userdata_ptr);

  return qmi_uimi_util_populate_response(errval, FALSE, 0, 0);
} /* qmi_uimi_personalization() */


/*===========================================================================
  FUNCTION QMI_UIMI_DEPERSONALIZATION_SECURE()

  DESCRIPTION
    Deactivates personalization of the device to a particular lock feature
    without passing in a CK. The input TLV is encrypted and must be decrypted
    before processing to insure a safe connection with client.
    Only deactivation of an activated feature is allowed.

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
dsm_item_type * qmi_uimi_depersonalization_secure
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                       errval             = QMI_ERR_NONE;
  qmi_uimi_userdata_type  *              cb_userdata_ptr    = NULL;
  simlock_result_enum_type               simlock_status     = SIMLOCK_SUCCESS;
  uint16                                 request_id         = 0;
  simlock_unlock_device_msg_req_type     unlock_msg;
  simlock_temporary_unlock_msg_req_type  temp_unlock_msg;
  qmi_uim_tlv_item_type                  tlv_data;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.type      = UIMI_TLV_TYPE_DECRYPTED_DEPERSO_SECURE;
  tlv_data.tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, &tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* This is required to make sure that TLV was originated for this specific request. */
  if(tlv_data.data.decrypted_deperso_secure.header.cmd_id != UIMI_CMD_VAL_DEPERSONALIZATION_SECURE)
  {
    UIM_MSG_ERR_0("QMI-UIM: decrypt request failed");
    errval = QMI_ERR_MALFORMED_MSG;
    goto send_result;
  }

  /* Retrieve the request_id from the request */
  request_id = tlv_data.data.decrypted_deperso_secure.header.request_id;

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_DEPERSONALIZATION_SECURE,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  cb_userdata_ptr->data.simlock_unlock.ck_present    = FALSE;
  cb_userdata_ptr->data.simlock_unlock.request_id    = request_id;
  cb_userdata_ptr->data.simlock_unlock.category      = tlv_data.data.decrypted_deperso_secure.category_type;
  cb_userdata_ptr->data.simlock_unlock.slot          = tlv_data.data.decrypted_deperso_secure.slot;

  switch (tlv_data.data.decrypted_deperso_secure.operation)
  {
    case QMI_UIM_DEPERSO_UNLOCK_WITHOUT_CK:
      memset(&unlock_msg, 0x00, sizeof(unlock_msg));
      memset(unlock_msg.ck.simlock_ck_data, 0x00, sizeof(unlock_msg.ck.simlock_ck_data));

      unlock_msg.ck.simlock_ck_length = 0;
      unlock_msg.ignore_ck            = FALSE;
      unlock_msg.probe_ck             = FALSE;
      unlock_msg.category             = tlv_data.data.decrypted_deperso_secure.category_type;
      unlock_msg.slot                 = tlv_data.data.decrypted_deperso_secure.slot;

      simlock_status = simlock_unlock_trusted(unlock_msg,
                                              qmi_uimi_simlock_callback,
                                              cb_userdata_ptr);
      break;

    case QMI_UIM_DEPERSO_TEMP_UNLOCK_ENABLE:
    case QMI_UIM_DEPERSO_TEMP_UNLOCK_DISABLE:
      memset(&temp_unlock_msg, 0x00, sizeof(temp_unlock_msg));

      if (tlv_data.data.decrypted_deperso_secure.operation == QMI_UIM_DEPERSO_TEMP_UNLOCK_ENABLE)
      {
        temp_unlock_msg.temporary_unlock_type = SIMLOCK_TEMPORARY_UNLOCK_SET;
      }
      else
      {
        temp_unlock_msg.temporary_unlock_type = SIMLOCK_TEMPORARY_UNLOCK_UNSET;
      }

      temp_unlock_msg.category = tlv_data.data.decrypted_deperso_secure.category_type;
      temp_unlock_msg.slot = tlv_data.data.decrypted_deperso_secure.slot;
      temp_unlock_msg.duration = tlv_data.data.decrypted_deperso_secure.duration;

      simlock_status = simlock_temporary_unlock(&temp_unlock_msg,
                                                qmi_uimi_simlock_callback,
                                                cb_userdata_ptr);
      break;

    default:
      UIM_MSG_ERR_1("Invalid deperso secure operation: 0x%x",
                    tlv_data.data.decrypted_deperso_secure.operation);
      simlock_status = SIMLOCK_INCORRECT_PARAMS;
      break;
  }

  /* Free userdata in case of error */
  QMI_UIM_SIMLOCK_FREE_USERDATA_IF_ERROR(simlock_status, cb_userdata_ptr);

  /* Convert MMGSDI status into QMI err code */
  errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);

send_result:
  /* Free the buffer used for data: this also clears the CK
     information for security reason */
  qmi_uim_util_free_tlv_content(&tlv_data, 1);

  if (errval == QMI_ERR_NONE)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  return qmi_uimi_util_populate_encrypted_response(errval,
                                                   UIMI_CMD_VAL_DEPERSONALIZATION_SECURE,
                                                   request_id);
} /* qmi_uimi_depersonalization_secure() */


/*===========================================================================
  FUNCTION QMI_UIMI_PERSONALIZATION_SECURE()

  DESCRIPTION
    Activates the personalization of the device to a particular lock feature
    without passing in a CK. The input TLV is encrypted and must be decrypted
    before processing to insure a safe connection with client.

    Only one feature can be activated at a time.

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
dsm_item_type * qmi_uimi_personalization_secure
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                      errval                     = QMI_ERR_NONE;
  qmi_uimi_userdata_type  *             cb_userdata_ptr            = NULL;
  simlock_result_enum_type              simlock_status             = SIMLOCK_SUCCESS;
  uint16                                request_id                 = 0;
  qmi_uim_tlv_item_type                 tlv_data;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.type      = UIMI_TLV_TYPE_DECRYPTED_PERSO_SECURE;
  tlv_data.tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, &tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* This is required to make sure that TLV was originated for this specific request */
  if(tlv_data.data.decrypted_perso_secure.header.cmd_id != UIMI_CMD_VAL_PERSONALIZATION_SECURE)
  {
    UIM_MSG_ERR_1("Invalid command id: 0x%x", tlv_data.data.decrypted_perso_secure.header.cmd_id);
    errval = QMI_ERR_MALFORMED_MSG;
    goto send_result;
  }

  /* Retrieve the request_id from the request */
  request_id = tlv_data.data.decrypted_perso_secure.header.request_id;

  if (tlv_data.data.decrypted_perso_secure.code_ptr == NULL)
  {
    UIM_MSG_ERR_0("Invalid category data");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_PERSONALIZATION_SECURE,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Save information in the user data */
  cb_userdata_ptr->data.simlock_lock.request_id           = request_id;
  cb_userdata_ptr->data.simlock_lock.ck_present           = FALSE;
  cb_userdata_ptr->data.simlock_lock.total_list_cnt       = tlv_data.data.decrypted_perso_secure.num_of_lists;
  cb_userdata_ptr->data.simlock_lock.slot                 = tlv_data.data.decrypted_perso_secure.slot;

  memset(cb_userdata_ptr->data.simlock_lock.ck.simlock_ck_data, 0x00, SIMLOCK_CK_MAX);
  cb_userdata_ptr->data.simlock_lock.ck.simlock_ck_length = 0;

  /* The lock codes contained in zeroth list are locked in the
     get_status response */
  cb_userdata_ptr->data.simlock_lock.next_list_to_lock    = 0;

  /* Save the lock code data pointer */
  cb_userdata_ptr->data.simlock_lock.code_ptr             = tlv_data.data.decrypted_perso_secure.code_ptr;

  /* Set the original pointer to NULL */
  tlv_data.data.decrypted_perso_secure.code_ptr           = NULL;

  simlock_status = simlock_get_status(qmi_uimi_simlock_callback,
                                      cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_SIMLOCK_FREE_USERDATA_IF_ERROR(simlock_status, cb_userdata_ptr);

  /* Convert simlock status into QMI err code */
  errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);

send_result:
  /* Free the buffer used for data: this also clears the CK
     information for security reason */
  qmi_uim_util_free_tlv_content(&tlv_data, 1);

  if (errval == QMI_ERR_NONE)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  return qmi_uimi_util_populate_encrypted_response(errval,
                                                   UIMI_CMD_VAL_PERSONALIZATION_SECURE,
                                                   request_id);
} /* qmi_uimi_personalization_secure */


/*===========================================================================
  FUNCTION QMI_UIMI_EMERGENCY_ONLY()

  DESCRIPTION
    Sets the device in a emergency only or normal operation mode. The input
    TLV is encrypted and must be decrypted before processing to insure a safe
    connection with client.

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
dsm_item_type * qmi_uimi_emergency_only
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                       errval             = QMI_ERR_NONE;
  qmi_uimi_userdata_type  *              cb_userdata_ptr    = NULL;
  simlock_result_enum_type               simlock_status     = SIMLOCK_SUCCESS;
  uint16                                 request_id         = 0;
  simlock_set_device_mode_msg_req_type   device_mode_msg;
  qmi_uim_tlv_item_type                  tlv_data;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.type      = UIMI_TLV_TYPE_DECRYPTED_EMERGENCY_ONLY;
  tlv_data.tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, &tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* This is required to make sure that TLV was originated for this specific request. */
  if(tlv_data.data.decrypted_emergency_only.header.cmd_id != UIMI_CMD_VAL_EMERGENCY_ONLY)
  {
    UIM_MSG_ERR_0("QMI-UIM: decrypt request failed");
    errval = QMI_ERR_MALFORMED_MSG;
    goto send_result;
  }

  memset(&device_mode_msg, 0x00, sizeof(simlock_set_device_mode_msg_req_type));

  /* Retrieve the mode and request_id from the request */
  device_mode_msg.mode = tlv_data.data.decrypted_emergency_only.device_mode;
  request_id           = tlv_data.data.decrypted_emergency_only.header.request_id;

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_EMERGENCY_ONLY,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* save information in the user data */
  cb_userdata_ptr->data.simlock_set_device_mode.request_id = request_id;

  simlock_status = simlock_set_device_operation_mode(device_mode_msg,
                                                     qmi_uimi_simlock_callback,
                                                     cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_SIMLOCK_FREE_USERDATA_IF_ERROR(simlock_status, cb_userdata_ptr);

  /* Convert MMGSDI status into QMI err code */
  errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);

send_result:
  /* Free the buffer used for data: this also clears the CK
     information for security reason */
  qmi_uim_util_free_tlv_content(&tlv_data, 1);

  if (errval == QMI_ERR_NONE)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  return qmi_uimi_util_populate_encrypted_response(errval,
                                                   UIMI_CMD_VAL_EMERGENCY_ONLY,
                                                   request_id);
} /* qmi_uimi_emergency_only */


/*===========================================================================
  FUNCTION QMI_UIMI_SIMLOCK_CONFIGURATION()

  DESCRIPTION
    Response to a simlock operation request sent via the indication to the
    client. The input TLV is encrypted and must be decrypted before
    processing to insure a safe connection with client.

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
dsm_item_type * qmi_uimi_simlock_configuration
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                       errval             = QMI_ERR_NONE;
  qmi_uimi_userdata_type  *              cb_userdata_ptr    = NULL;
  simlock_result_enum_type               simlock_status     = SIMLOCK_SUCCESS;
  qmi_uim_simlock_config_operation_type  operation_type     = QMI_UIM_SIMLOCK_CONFIG_OPERATION_READ;
  uint16                                 request_id         = 0;
  qmi_uim_tlv_item_type                  tlv_data;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.type      = UIMI_TLV_TYPE_DECRYPTED_SIMLOCK_CONFIG;
  tlv_data.tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, &tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* This is required to make sure that TLV was originated for this specific request */
  if(tlv_data.data.decrypted_simlock_config.header.cmd_id != UIMI_CMD_VAL_SIMLOCK_CONFIGURATION)
  {
    UIM_MSG_ERR_1("Invalid command id: 0x%x", tlv_data.data.decrypted_simlock_config.header.cmd_id);
    errval = QMI_ERR_MALFORMED_MSG;
    goto send_result;
  }

  /* Retrieve the operation and request_id from the request */
  operation_type = tlv_data.data.decrypted_simlock_config.operation;
  request_id     = tlv_data.data.decrypted_simlock_config.header.request_id;

  /* If the operation is enable or disable remote SFS, call the
     simlock API */
  if((operation_type == QMI_UIM_SIMLOCK_CONFIG_OPERATION_ENABLE_REMOTE_SFS) ||
     (operation_type == QMI_UIM_SIMLOCK_CONFIG_OPERATION_DISABLE_REMOTE_SFS))
  {
    /* Create userdata */
    cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SIMLOCK_CONFIGURATION,
                                                   cmd_buf_p,
                                                   NULL,
                                                   ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
    if (cb_userdata_ptr == NULL)
    {
      errval = QMI_ERR_NO_MEMORY;
      goto send_result;
    }

    /* save information in the user data */
    cb_userdata_ptr->data.simlock_set_remote_sfs.request_id = request_id;

    simlock_status = simlock_set_remote_sfs(tlv_data.data.decrypted_simlock_config.operation_type.remote_sfs_operation,
                                            qmi_uimi_simlock_set_remote_sfs_callback,
                                            cb_userdata_ptr);

    /* Free userdata in case of error */
    QMI_UIM_SIMLOCK_FREE_USERDATA_IF_ERROR(simlock_status, cb_userdata_ptr);

    /* Convert MMGSDI status into QMI err code */
    errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);
  }

  /* If the operation is read/write simlock data in remote SFS, call the
     simlock response cb */
  if(operation_type == QMI_UIM_SIMLOCK_CONFIG_OPERATION_READ)
  {
    if(qmi_uim_global_ptr->simlock_remote_sfs_operation_resp_cb != NULL)
    {
      qmi_uim_global_ptr->simlock_remote_sfs_operation_resp_cb(tlv_data.data.decrypted_simlock_config.operation_type.sfs_read_operation.result,
                                                               request_id,
                                                               &tlv_data.data.decrypted_simlock_config.operation_type.sfs_read_operation.data);
    }
    else
    {
      /* A NULL callback indicates that the modem is not
         expecting this message. So return an error */
      errval = QMI_ERR_INVALID_OPERATION;
    }
  }
  else if(operation_type == QMI_UIM_SIMLOCK_CONFIG_OPERATION_WRITE)
  {
    if(qmi_uim_global_ptr->simlock_remote_sfs_operation_resp_cb != NULL)
    {
      qmi_uim_global_ptr->simlock_remote_sfs_operation_resp_cb(tlv_data.data.decrypted_simlock_config.operation_type.sfs_write_operation.result,
                                                               request_id,
                                                               NULL);
    }
    else
    {
      /* A NULL callback indicates that the modem is not
         expecting this message. So return an error */
      errval = QMI_ERR_INVALID_OPERATION;
    }
  }

send_result:
  /* Free the buffer used for data: this also clears the read data
     information for security reason */
  qmi_uim_util_free_tlv_content(&tlv_data, 1);

  if ((errval == QMI_ERR_NONE) &&
      (operation_type == QMI_UIM_SIMLOCK_CONFIG_OPERATION_ENABLE_REMOTE_SFS ||
       operation_type == QMI_UIM_SIMLOCK_CONFIG_OPERATION_DISABLE_REMOTE_SFS))
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  return qmi_uimi_util_populate_encrypted_response(errval,
                                                   UIMI_CMD_VAL_SIMLOCK_CONFIGURATION,
                                                   request_id);
} /* qmi_uimi_simlock_configuration */


/*===========================================================================
  FUNCTION QMI_UIMI_REMOTE_UNLOCK()

  DESCRIPTION
    This function provides following functionality based on the type of
    request:
    Provides the key in the encrypted format OR
    Applies the simlock configuration data provided as the input OR
    Generates HMAC on the input data OR
    Retrieve the version of blob supported by Simlock engine

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
dsm_item_type *  qmi_uimi_remote_unlock
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                           errval          = QMI_ERR_NONE;
  simlock_result_enum_type                   simlock_status  = SIMLOCK_SUCCESS;
  qmi_uimi_userdata_type                   * cb_userdata_ptr = NULL;
  qmi_uim_tlv_item_type                      tlv_data[3];
  uim_remote_unlock_operation_type_enum_v01  operation       = UIM_REMOTE_UNLOCK_OPERATION_TYPE_ENUM_MAX_ENUM_VAL_V01;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_REMOTE_UNLOCK_CONFIG;
  tlv_data[0].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_REMOTE_UNLOCK_OPERATION;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_REMOTE_UNLOCK_CONFIG_EXT;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_3;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_REMOTE_UNLOCK,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* If simlock data TLV and simlock extended data TLV are present then return
     invalid arguments error */
  if(tlv_data[0].present && tlv_data[2].present)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  if(tlv_data[1].present)
  {
    operation = tlv_data[1].data.remote_unlock_operation.operation;
  }
  else if(tlv_data[0].present)
  {
    if((tlv_data[0].data.remote_unlock_config.config_data.data_len > 0) &&
       (tlv_data[0].data.remote_unlock_config.config_data.data_ptr != NULL))
    {
      operation = UIM_REMOTE_UNLOCK_PROCESS_SIMLOCK_DATA_V01;
    }
  }
  else if(tlv_data[2].present)
  {
    if((tlv_data[2].data.remote_unlock_config.config_data.data_len > 0) &&
       (tlv_data[2].data.remote_unlock_config.config_data.data_ptr != NULL))
    {
      operation = UIM_REMOTE_UNLOCK_PROCESS_SIMLOCK_DATA_V01;
    }
  }
  else
  {
    operation = UIM_REMOTE_UNLOCK_RETRIEVE_ENCRYPTED_KEY_V01;
  }

  switch(operation)
  {
    case UIM_REMOTE_UNLOCK_RETRIEVE_ENCRYPTED_KEY_V01:
      simlock_status = simlock_rsu_get_encrypted_key(qmi_uimi_simlock_remote_unlock_callback,
                                                     cb_userdata_ptr);
      break;
    case UIM_REMOTE_UNLOCK_PROCESS_SIMLOCK_DATA_V01:
      simlock_status = simlock_rsu_set_configuration(tlv_data[0].present ?
                                                       tlv_data[0].data.remote_unlock_config.config_data :
                                                       tlv_data[2].data.remote_unlock_config.config_data,
                                                     qmi_uimi_simlock_remote_unlock_callback,
                                                     cb_userdata_ptr);
      break;
    case UIM_REMOTE_UNLOCK_GENERATE_MAC_V01:
      simlock_status = simlock_rsu_generate_hash(tlv_data[0].present ?
                                                   tlv_data[0].data.remote_unlock_config.config_data :
                                                   tlv_data[2].data.remote_unlock_config.config_data,
                                                 qmi_uimi_simlock_remote_unlock_callback,
                                                 cb_userdata_ptr);
      break;
    case UIM_REMOTE_UNLOCK_GET_MAX_SUPPORTED_VERSION_V01:
      simlock_status = simlock_rsu_retrieve_version(qmi_uimi_simlock_remote_unlock_callback,
                                                    cb_userdata_ptr);
      break;
    case UIM_REMOTE_UNLOCK_BLOB_REQUEST_V01:
      simlock_status = simlock_rsu_generate_blob_request(qmi_uimi_simlock_remote_unlock_callback,
                                                         cb_userdata_ptr);
      break;
    case UIM_REMOTE_UNLOCK_START_DELAY_TIMER_V01:
      simlock_status = simlock_rsu_manage_delay_timer_request(TRUE,
                                                              qmi_uimi_simlock_remote_unlock_callback,
                                                              cb_userdata_ptr);
      break;
    case UIM_REMOTE_UNLOCK_STOP_DELAY_TIMER_V01:
      simlock_status = simlock_rsu_manage_delay_timer_request(FALSE,
                                                              qmi_uimi_simlock_remote_unlock_callback,
                                                              cb_userdata_ptr);
      break;
    default:
      simlock_status = SIMLOCK_INCORRECT_PARAMS;
      break;
  }

  /* Convert simlock status into QMI err code */
  errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval, FALSE, 0, 0);
} /* qmi_uimi_remote_unlock */


/*===========================================================================
  FUNCTION QMI_UIMI_VERIFY_IMSI()

  DESCRIPTION
    Verifies that subscription related to IMSI is validated by SIMLOCK or not.
    If optional IMSI TLV is not present, it verifies the IMSI of all active
    provisioning sessions and returns an error if any of them is not verified.

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
dsm_item_type *  qmi_uimi_verify_imsi
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                errval                 = QMI_ERR_NONE;
  dsm_item_type                 * response               = NULL;
  qmi_result_e_type               result                 = QMI_RESULT_SUCCESS;
  boolean                         retval                 = TRUE;
  simlock_result_enum_type        simlock_status         = SIMLOCK_SUCCESS;
  uint8                           loop_index             = 0;
  uint8                           index                  = 0;
  qmi_uim_imsi_type               imsi_data              = {0, };
  simlock_imsi_type               simlock_imsi           = {0, };
  qmi_uim_tlv_item_type           tlv_data;
  uint16                          *session_index_array[] = {qmi_uim_global_ptr->card_state.index_gw,
                                                            qmi_uim_global_ptr->card_state.index_1x};
  mmgsdi_session_id_type          *session_id_array[]    = {qmi_uim_global_ptr->mmgsdi_gw_session_id,
                                                            qmi_uim_global_ptr->mmgsdi_1x_session_id};

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.type      = UIMI_TLV_TYPE_IMSI;
  tlv_data.tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, &tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if(tlv_data.present)
  {
    if(tlv_data.data.imsi.imsi_data.imsi_len > 0)
    {
      simlock_status = simlock_verify_imsi(tlv_data.data.imsi.imsi_data);
    }
    else
    {
      simlock_status = SIMLOCK_INCORRECT_PARAMS;
    }
  }
  else
  {
    /* Read IMSI for all active subscriptions synchronously and verify IMSI
       is validated by SIMLOCK module or not */
    for(loop_index = 0;
        loop_index < sizeof(session_index_array) / sizeof(session_index_array[0]);
        loop_index++)
    {
      for (index = 0; index < QMI_UIM_MAX_PROV_SESSIONS; index++)
      {
        if (session_index_array[loop_index][index] != 0xFFFF)
        {
          errval = qmi_uim_util_read_imsi_for_session_id(session_id_array[loop_index][index],
                                                         &imsi_data);
          /* It is possible that IMSI data is not cached yet for subscription
             i.e. it gets stuck because of enabled PIN1. This also means that
             SIMLOCK algo is not run for that subscription. For such scenario we
             should not assume that IMSI is not valid; just continue to verify
             IMSI of next active subscription */
          if(errval != QMI_ERR_NONE)
          {
            continue;
          }

          simlock_imsi.imsi_len = imsi_data.len;
          (void)memscpy(simlock_imsi.imsi, sizeof(simlock_imsi.imsi), imsi_data.imsi, imsi_data.len);

          simlock_status = simlock_verify_imsi(simlock_imsi);
          if(simlock_status != SIMLOCK_SUCCESS)
          {
            UIM_MSG_ERR_2("Invalid IMSI for session at index 0x%x, 0x%x",
                          loop_index, index);
            break;
          }
        }
      }

      /*If any of the IMSI is not valid, break the loop */
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        break;
      }
    }
  }

  /* If SIMLOCK module returns generic error, map it to INVALID ID */
  if(simlock_status == SIMLOCK_GENERIC_ERROR)
  {
    errval = QMI_ERR_INVALID_ID;
  }
  else
  {
    errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(&tlv_data, 1);

  /* Send the response */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_verify_imsi */


/*===========================================================================
  FUNCTION QMI_UIMI_ACTIVATE_SIMLOCK_CONFIGURATION()

  DESCRIPTION
    Activates SIMLOCK categories associated with config id.

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
dsm_item_type *  qmi_uimi_activate_simlock_configuration
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  boolean                         retval                 = TRUE;
  qmi_error_e_type                errval                 = QMI_ERR_NONE;
  dsm_item_type                 * response               = NULL;
  qmi_result_e_type               result                 = QMI_RESULT_SUCCESS;
  qmi_uimi_userdata_type        * cb_userdata_ptr        = NULL;
  simlock_result_enum_type        simlock_status         = SIMLOCK_GENERIC_ERROR;
  qmi_uim_tlv_item_type           tlv_data;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.type      = UIMI_TLV_TYPE_CONFIG_ID;
  tlv_data.tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, &tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_ACTIVATE_SIMLOCK_CONFIG,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  simlock_status = simlock_activate_configuration(tlv_data.data.config_id.value,
                                                  qmi_uimi_simlock_callback,
                                                  cb_userdata_ptr);

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);
send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(&tlv_data, 1);

  /* Send the response */
  return qmi_uimi_util_populate_response(errval, FALSE, 0, 0);
}/* qmi_uimi_activate_simlock_configuration */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_SIMLOCK_TOKEN()

  DESCRIPTION
    Returns token assocaited with enabled SIMLOCK cateogries.

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
dsm_item_type *  qmi_uimi_get_simlock_token
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  boolean                         retval                 = TRUE;
  qmi_error_e_type                errval                 = QMI_ERR_NONE;
  dsm_item_type                 * response               = NULL;
  qmi_result_e_type               result                 = QMI_RESULT_SUCCESS;
  qmi_uimi_userdata_type        * cb_userdata_ptr        = NULL;
  simlock_result_enum_type        simlock_status         = SIMLOCK_GENERIC_ERROR;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);


  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_SIMLOCK_TOKEN,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  simlock_status = simlock_get_token(qmi_uimi_simlock_callback,
                                     cb_userdata_ptr);

  errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);
send_result:
  /* Send the response */
  return qmi_uimi_util_populate_response(errval, FALSE, 0, 0);
}/* qmi_uimi_get_simlock_token */

#endif /* FEATURE_SIMLOCK */


/*===========================================================================
  FUNCTION QMI_UIMI_SIMLOCK_CALLBACK()

  DESCRIPTION
    This function is executed as callback of SIM Lock API. This is
    executed in the context of SIM Lock task and needs to make a deep
    copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_simlock_callback
(
  simlock_result_enum_type                    status,
  const simlock_message_response_data_type  * resp_ptr,
  const void                                * user_data_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(user_data_ptr != NULL);

  /* resp_ptr can be NULL if SIMLOCK cleans up commands as part of TASK STOP
     signal handling */
  if(resp_ptr == NULL)
  {
    qmi_uim_util_free_cb_userdata((qmi_uimi_userdata_type*)user_data_ptr);
    return;
  }

  UIM_MSG_HIGH_1("Got response from SIM Lock for message: 0x%x", resp_ptr->msg_type);

  switch(resp_ptr->msg_type)
  {
    case SIMLOCK_UNLOCK_DEVICE_MSG:
    case SIMLOCK_TEMPORARY_UNLOCK_MSG:
    case SIMLOCK_GET_STATUS_MSG:
    case SIMLOCK_SET_LOCK_CK_MSG:
    case SIMLOCK_SET_DEVICE_MODE_MSG:
    case SIMLOCK_ADD_LOCK_CODES_CK_MSG:
    case SIMLOCK_RELOCK_DEVICE_MSG:
    case SIMLOCK_ACTIVATE_CONFIG_MSG:
    case SIMLOCK_GET_TOKEN_MSG:
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_SIMLOCK_CB);
      if (msg_ptr != NULL)
      {
        msg_ptr->data.simlock.status  = status;
        msg_ptr->data.simlock.user_data_ptr = user_data_ptr;

        memscpy(&msg_ptr->data.simlock.data,
                sizeof(msg_ptr->data.simlock.data),
                resp_ptr,
                sizeof(simlock_message_response_data_type));

        /* Send command */
        if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
           (present_in_queue))
        {
          uimqmi_free(msg_ptr);
        }
        msg_ptr = NULL;
      }
      break;

    default:
      break;
  }
} /* qmi_uimi_simlock_callback */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_CONVERT_SIMLOCK_STATUS_TO_ERRVAL

  DESCRIPTION
    To obtain QMI client-specific error message from SIM Lock error message

  PARAMETERS
    simlock_status  : SIM Lock return value

  RETURN VALUE
    errval          : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_simlock_convert_simlock_status_to_errval
(
  simlock_result_enum_type simlock_status
)
{
  qmi_error_e_type errval  = QMI_ERR_INTERNAL;

  UIM_MSG_HIGH_1("Received simlock_status 0x%x", simlock_status);

  switch(simlock_status)
  {
    case SIMLOCK_SUCCESS:
      errval = QMI_ERR_NONE;
      break;

    case SIMLOCK_INVALID_STATE:
      errval = QMI_ERR_NO_EFFECT;
      break;

    case SIMLOCK_INCORRECT_KEY:
      errval = QMI_ERR_INCORRECT_PIN;
      break;

    case SIMLOCK_MAX_RETRIES_REACHED:
      errval = QMI_ERR_PIN_PERM_BLOCKED;
      break;

    case SIMLOCK_INCORRECT_PARAMS:
      errval = QMI_ERR_MALFORMED_MSG;
      break;

    case SIMLOCK_UNSUPPORTED:
      errval = QMI_ERR_NOT_SUPPORTED;
      break;

    case SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED:
      errval = QMI_ERR_NO_MEMORY;
      break;

    case SIMLOCK_RSU_INCOMPATIBLE_STATE:
      errval = QMI_ERR_INCOMPATIBLE_STATE;
      break;

    default:
      errval = QMI_ERR_INTERNAL;
      break;
  }

  return errval;
} /* qmi_uim_simlock_convert_simlock_status_to_errval() */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_CHECK_AND_DEREGISTER_REMOTE_SFS_OPERATION()

  DESCRIPTION
    This function deregisters the QMI UIM callback with the simlock.

  PARAMETERS
    client_id  : Client id of the registered client

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_simlock_check_and_deregister_remote_sfs_operation
(
  uint16  client_id
)
{
  uint8                              i               = 0;
  qmi_uimi_client_state_type       * cl_sp           = NULL;

  if((qmi_uim_state_ptr == NULL) ||
     (client_id == QMI_SVC_CLID_UNUSED))
  {
    return;
  }

  for(i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];
    if((cl_sp == NULL) ||
       (cl_sp->common.clid == QMI_SVC_CLID_UNUSED))
    {
      continue;
    }

    if(cl_sp->common.clid == client_id)
    {
      continue;
    }

    /* If there is at least one client who registered for remote SFS
       events, do not de-register with simlock */
    if(cl_sp->uim_info.reg_event_type.simlock_remote_sfs_events)
    {
      return;
    }
  }

  /* Deregister for events from SIM Lock task */
  (void)simlock_register_remote_sfs_operation(SIMLOCK_DEREGISTER,
                                              NULL);
} /* qmi_uim_simlock_check_and_deregister_remote_sfs_operation */

