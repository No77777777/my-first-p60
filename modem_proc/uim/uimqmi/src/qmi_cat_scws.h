/*================================================================================

DESCRIPTION
QMI CAT common header file


Copyright (c) 2015 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_cat_scws.h#2 $$ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
10/27/15   vr      Multi-profile switch handling
09/08/15   vr      Initial version

=============================================================================*/

#ifndef QMI_CAT_SCWS_H
#define QMI_CAT_SCWS_H


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "qmi_cat_common.h"


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION  QMI_CAT_PROCESS_SCWS_COMMAND()

  DESCRIPTION
    To process the scws command from GSTK in QMI context

  PARAMETERS

   scws_cmd : Pointer to scws command

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cat_process_scws_command
(
  const qmi_cat_message_type * cat_msg_ptr
);

/*===========================================================================
  FUNCTION QMI_CATI_SCWS_OPEN_CHANNEL()

  DESCRIPTION
    Send the confirmation with channel state for scws open channel cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_open_channel
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
);

/*===========================================================================
  FUNCTION QMI_CATI_SCWS_CLOSE_CHANNEL()

  DESCRIPTION
    Send the confirmation with channel state for scws close channel cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_close_channel
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
);

/*===========================================================================
  FUNCTION QMI_CATI_SCWS_CHANGE_STATUS()

  DESCRIPTION
    Send the confirmation with channel state for scws change status cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_change_status
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
);

/*===========================================================================
  FUNCTION QMI_CATI_SCWS_SEND_DATA()

  DESCRIPTION
    Send the confirmation with channel state for scws send data cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_send_data
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
);

/*===========================================================================
  FUNCTION QMI_CATI_SCWS_DATA_AVAILABLE()

  DESCRIPTION
    Send the confirmation with channel state for scws data available cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_data_available
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
);

/*===========================================================================
FUNCTION: qmi_cati_scws_cmd_reg

DESCRIPTION:
  SCWS event registration.

PARAMETERS:
  slot_index : slot index

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void qmi_cati_scws_cmd_reg
(
  uint8                            slot_index,
  gstk_reg_rsp_cmd_cb_func_type    client_cmd_cb,
  gstk_toolkit_evt_cb_func_type    client_evt_cb
);

/*===========================================================================
  FUNCTION QMI_CATI_CLONE_SCWS_COMMAND

  DESCRIPTION
    Makes a deep copy of a scws command

  PARAMETERS
    gstk_cmd_from_card_type: source procative command
    gstk_cmd_from_card_type: destination procative command

  RETURN VALUE
    boolean: result of the copy

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_cati_clone_scws_command
(
  gstk_cmd_from_card_type       * dst,
  const gstk_cmd_from_card_type * src
);

/*===========================================================================
  FUNCTION QMI_CATI_FREE_SCWS_COMMAND

  DESCRIPTION
    Free SCWS command

  PARAMETERS
    gstk_cmd_from_card_type: scws command

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cati_free_scws_command
(
  gstk_cmd_from_card_type       * command
);

/*===========================================================================
FUNCTION: QMI_CAT_SEND_BUFFERED_SCWS_CMD

DESCRIPTION:
  QMI CAT send buffered commands at the given index to the control point that
  registers for the event

PARAMETERS:
  slot_index  : Index to card slot

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:

===========================================================================*/
void qmi_cat_send_buffered_scws_cmd
(
  uint8  slot_index
);

/*===========================================================================
FUNCTION: qmi_cat_cleanup_scws_globals

DESCRIPTION:
  This function is used to free the qmicat globals

PARAMETERS:
  card_index: slot index

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void qmi_cat_cleanup_scws_globals
(
  uint8 card_index
);

#endif /* QMI_CAT_SCWS_H */
