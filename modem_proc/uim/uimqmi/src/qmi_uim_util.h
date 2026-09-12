#ifndef QMI_UIM_UTIL_H
#define QMI_UIM_UTIL_H
/*===========================================================================

                         Q M I _ U I M _ U T I L. C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016 - 2017, 2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_util.h#2 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
12/27/17    ra     Disable GSM Security restrictions for Authentication on testcard
01/19/17    me     Populating path internally and parsing IMSI, FPLMN, EHPLMN 
12/07/16    av     LPA phase2 initial changes
12/05/16    vdc    Added support for simlock single lock for full srv policy
08/14/16    tkl    Added support for simlock single lock
07/15/16    sp     Seperate ERA-GLONASS and SAP related QMI UIM functionality
07/08/16    ar     Move qmi_uimi_recovery and unify fdn/acl get_status func.
06/30/16    av     Fix slot id to efs context conversion for uim common APIs
05/19/16    vdc    Move internal enums, structs to new internal header file
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_internal.h"
#include "uim_common_efs.h"
#include "mmgsdilib_common.h"

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Macro used to free userdata in case of error
---------------------------------------------------------------------------*/
#define QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr)  \
            if (mmgsdi_status != MMGSDI_SUCCESS)                        \
            {                                                           \
              qmi_uim_util_free_cb_userdata(cb_userdata_ptr);                \
              cb_userdata_ptr = NULL;                                   \
            }

/*---------------------------------------------------------------------------
  Macro used to free userdata in case of error
---------------------------------------------------------------------------*/
#define QMI_UIM_UTIL_LPA_FREE_USERDATA_IF_ERROR(lpa_status, cb_userdata_ptr)  \
            if (lpa_status != LPA_SUCCESS)                               \
            {                                                            \
              qmi_uim_util_free_cb_userdata(cb_userdata_ptr);                 \
              cb_userdata_ptr = NULL;                                    \
            }

/*---------------------------------------------------------------------------
  Macro used to validate if cmd_buf_p is valid for a response. Free userdata
  pointer & returns if NULL
---------------------------------------------------------------------------*/
#define QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr)             \
            if ((!cb_userdata_ptr->ind_token.is_valid) &&                 \
                (cb_userdata_ptr->cmd_buf_ptr == NULL))                   \
            {                                                             \
              UIM_MSG_HIGH_0("Null cmd_buf ptr for response");            \
              qmi_uim_util_free_cb_userdata(cb_userdata_ptr);                  \
              return;                                                     \
            }

/*---------------------------------------------------------------------------
  Macros used to validate the SP in command buffer or return if NULL.
  The second macro also frees the QMI response before returning.
---------------------------------------------------------------------------*/
#define QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p)                         \
            if (cmd_buf_p->x_p == NULL || cmd_buf_p->x_p->cl_sp == NULL)  \
            {                                                             \
              UIM_MSG_HIGH_0("Transaction ptr or client state pointer is NULL"); \
              return;                                                     \
            }

#define QMI_UIM_VALIDATE_SP_IN_CMD_BUF_FREE_RESPONSE(cmd_buf_p, response) \
            if (cmd_buf_p->x_p == NULL || cmd_buf_p->x_p->cl_sp == NULL)  \
            {                                                             \
              UIM_MSG_HIGH_0("Transaction ptr or client state pointer is NULL"); \
              dsm_free_packet(&response);                                 \
              return;                                                     \
            }

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
);

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
);

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
);

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
);

/*===========================================================================
  FUNCTION QMI_UIM_UTIL_POST_MESSAGE

  DESCRIPTION
    Adds a message to the QMI UIM queue and posts an event to DCC task,
    so that it can be processed later in the right context.If signal is 
    not set and if the command is still present in the queue, delete the
    command from queue, make "present_in_queue" to TRUE, so that
    qmi_uim_msg_ptr will be freed in caller function

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_MMGSDI_SLOT_TO_LPA_SLOT

  DESCRIPTION
    To obtain LPA slot type from MMGSDI slot type

  PARAMETERS
    lpa_status   : LPA return value

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
  mmgsdi_session_type_enum_type   session_type,
  qmi_uim_tlv_file_id_type      * file_id_ptr
);

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
  mmgsdi_session_id_type   session_id,
  uint8                  * num_mnc_digits_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_UTIL_CONVERT_IMSI_TO_DIGITS()

  DESCRIPTION
    Convert IMSI from qmi_uim_imsi_type to qmi_uim_imsi_digits_data_type.

  PARAMETERS
    None

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
);

/*===========================================================================
FUNCTION QMI_UIM_UTIL_IS_SESSION_PROVISIONING

DESCRIPTION
  This function indicates whether the session is provisioning or not

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
boolean qmi_uim_util_is_session_provisioning
(
  mmgsdi_session_type_enum_type  session_type
);

#endif /* QMI_UIM_UTIL_H */
