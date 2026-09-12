/*===========================================================================

                         Q M I _ U I M _ G B A._ S T U B S C

DESCRIPTION
  This file contains stub functions for QMI UIM service's GBA functionality.

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_gba_stubs.c#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
06/16/16    vdc    Split QMI UIM parsing files for supporting IOE
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
#include "qmi_uim_util.h"
#include "mmgsdisessionlib_v.h"
#include "qmi_uim_parsing.h"
#include "qmi_framework.h"

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
  (void)mmgsdi_session_type;
  (void)gba_session_type_ptr;

  return QMI_ERR_NOT_SUPPORTED;
} /* qmi_uim_gba_convert_mmgsdi_session_to_gba_session */


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
  (void)gba_status;

  return QMI_ERR_NOT_SUPPORTED;
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
  (void)status;
  (void)request_id;
  (void)cnf_ptr;
  (void)user_data_ptr;
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
  (void)event_ptr;
} /* qmi_uimi_gba_event_callback */


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
  (void)uim_message_ptr;
} /* qmi_uim_gba_process_gba_cb */


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
  (void)uim_message_ptr;
} /* qmi_uim_gba_process_gba_event_cb */


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
  (void)tlv_session_info_ptr;
  (void)impi_data_pptr;

  return QMI_ERR_NOT_SUPPORTED;
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
  (void)tlv_data_ptr;
  (void)tlv_data_count;
  (void)cmd_buf_ptr;
  (void)cl_sp;

  return QMI_ERR_NOT_SUPPORTED;
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
  (void)status;
  (void)read_cnf_ptr;
}/* qmi_uim_gba_read_impi_resp */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_NAF_ID()

  DESCRIPTION
    Parses the TLV item for NAF ID request

  PARAMETERS
    sdu_in                     : input sdu
    len                        : length of the TLV item
    gba_naf_id_ptr             : NAF ID data pointer

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_naf_id
(
  dsm_item_type **            sdu_in,
  uint16                      len,
  qmi_uim_tlv_naf_id_type *   gba_naf_id_ptr
)
{
  (void)sdu_in;
  (void)len;
  (void)gba_naf_id_ptr;

  return QMI_ERR_INTERNAL;
} /* qmi_uim_tlv_naf_id */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_APN()

  DESCRIPTION
    Parses the TLV item for Access Point Name request

  PARAMETERS
    sdu_in                     : input sdu
    len                        : length of the TLV item
    apn_ptr                    : APN data pointer

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_apn
(
  dsm_item_type **        sdu_in,
  uint16                  len,
  qmi_uim_tlv_apn_type  * apn_ptr
)
{
  (void)sdu_in;
  (void)len;
  (void)apn_ptr;

  return QMI_ERR_INTERNAL;
} /* qmi_uim_tlv_apn */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_BSF_INFO()

  DESCRIPTION
    Parses the TLV item of verify IMSI request

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    imsi_data_ptr     : output with imsi data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_bsf_info
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_bsf_info_type                  *  gba_bsf_info_ptr
)
{
  (void)sdu_in;
  (void)len;
  (void)gba_bsf_info_ptr;

  return QMI_ERR_INTERNAL;
} /* qmi_uim_tlv_bsf_info */


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
  boolean        retval   = TRUE;
  dsm_item_type *response = NULL;

  (void)sp;
  (void)cmd_buf_p;
  (void)cl_sp;
  (void)sdu_in;

  retval = qmi_svc_put_result_tlv(&response, QMI_RESULT_FAILURE, QMI_ERR_NOT_SUPPORTED);
  CHECK_RETVAL();

  return response;
}/* qmi_uimi_gba */


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
  boolean        retval   = TRUE;
  dsm_item_type *response = NULL;

  (void)sp;
  (void)cmd_buf_p;
  (void)cl_sp;
  (void)sdu_in;

  retval = qmi_svc_put_result_tlv(&response, QMI_RESULT_FAILURE, QMI_ERR_NOT_SUPPORTED);
  CHECK_RETVAL();

  return response;
}/* qmi_uimi_get_gba_impi */
