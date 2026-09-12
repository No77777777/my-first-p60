#ifndef QMI_UIM_SAP_H
#define QMI_UIM_SAP_H
/*==============================================================================

                         Q M I _ U I M _ S A P _ H

DESCRIPTION
 
 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sap.h#1 $

when        who    what, where, why
--------    ---    ------------------------------------------------------------
06/22/16    sp     Initial Revision.
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "qmi_uim_internal.h"
#include "mmgsdilib_common.h"

/*==============================================================================

                             EXTERNAL FUNCTIONS

==============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIMI_SAP_CONNECTION()

  DESCRIPTION
    Send connection related SAP commands to the card

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
dsm_item_type * qmi_uimi_sap_connection
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_SAP_REQUEST()

  DESCRIPTION
    Send various SAP requests to the card

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
dsm_item_type * qmi_uimi_sap_request
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIM_SEND_SAP_STATUS_INDICATION_TO_ALL()

  DESCRIPTION
    This function sends a SAP status indication to all QMI_UIM clients

  PARAMETERS
    slot_id : Card slot

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_send_sap_status_indication_to_all
(
  mmgsdi_slot_id_enum_type    slot_id
);

/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SAP_EVT()

  DESCRIPTION
    This function processes the SAP events from MMGSDI

  PARAMETERS
    event_ptr: pointer to event

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_process_sap_evt
(
  const mmgsdi_event_data_type * event_ptr
);

#endif /* QMI_UIM_SAP_H */
