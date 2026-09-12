#ifndef QMI_UIM_REMOTE_H
#define QMI_UIM_REMOTE_H

/*===========================================================================

                         Q M I _ U I M _ R E M O T E . H

DESCRIPTION

 This header file defines data types and functions necessary to access a SIM
 attached to a remote device such as an applications processor or another modem
 via the QMI interface.

                        COPYRIGHT INFORMATION

Copyright (c) 2013 - 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/inc/qmi_uim_remote.h#2 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/26/15    sp     Updated copyright
04/15/15    ks     De-Featurizing the FEATURE_UIM_REMOTE_UIM
05/07/14    tkl    Add Vcc and response_timeout for power up request
03/16/14    tkl    Add power down mode for power down request
12/11/13    vs     Addition of UIMQMI task and QMI UIM REMOTE

===========================================================================*/

/*=============================================================================

                   I N C L U D E S

=============================================================================*/
#include "comdef.h"
#include "qmi_csi.h"
#include "qmi_sap.h"
#include "queue.h"

/*=============================================================================

                   D E F I N E S

=============================================================================*/
#define QMI_UIM_REMOTE_MAX_ATR_LEN               32

/*=============================================================================

                   E N U M E R A T E D   D A T A

=============================================================================*/

/*===========================================================================
   ENUM:      QMI_UIM_REMOTE_RESULT_TYPE
=============================================================================*/
typedef enum
{
  QMI_UIM_REMOTE_SUCCESS    = 0x0,
  QMI_UIM_REMOTE_ERROR
} qmi_uim_remote_result_type;

/*===========================================================================
   ENUM:      QMI_UIM_REMOTE_SLOT_TYPE
=============================================================================*/
typedef enum
{
  QMI_UIM_REMOTE_SLOT_1              = 0x0,
  QMI_UIM_REMOTE_SLOT_2,
  QMI_UIM_REMOTE_SLOT_3,
  QMI_UIM_REMOTE_SLOT_NOT_APPLICABLE = 0x7FFFFFFF
} qmi_uim_remote_slot_type;

/*===========================================================================
   ENUM:      QMI_UIM_REMOTE_EVT_ENUM_TYPE
=============================================================================*/
typedef enum
{
  QMI_UIM_REMOTE_EVT_NOT_RECEIVED            = 0x0,
  QMI_UIM_REMOTE_EVT_CONNECTION_UNAVAILABLE,
  QMI_UIM_REMOTE_EVT_CONNECTION_AVAILABLE,
  QMI_UIM_REMOTE_EVT_CARD_INSERTED,
  QMI_UIM_REMOTE_EVT_CARD_REMOVED,
  QMI_UIM_REMOTE_EVT_CARD_ERROR,
  QMI_UIM_REMOTE_EVT_CARD_RESET,
  QMI_UIM_REMOTE_EVT_CARD_WAKEUP,
  QMI_UIM_REMOTE_EVT_NOT_APPLICABLE          = 0x7FFFFFFF
} qmi_uim_remote_evt_enum_type;

/*===========================================================================
   ENUM:      QMI_UIM_REMOTE_CARD_ERROR_ENUM_TYPE
=============================================================================*/
typedef enum
{
  QMI_UIM_REMOTE_CARD_ERROR_UNKNOWN_ERROR              = 0x0,
  QMI_UIM_REMOTE_CARD_ERROR_NO_LINK_ESTABLISHED        = 0x1,
  QMI_UIM_REMOTE_CARD_ERROR_COMMAND_TIMEOUT            = 0x2,
  QMI_UIM_REMOTE_CARD_ERROR_DUE_TO_POWER_DOWN          = 0x3,
  QMI_UIM_REMOTE_CARD_ERROR_DUE_TO_POWER_DOWN_TELECOM  = 0x04,
  QMI_UIM_REMOTE_CARD_ERROR_NOT_APPLICABLE             = 0x7FFFFFFF
} qmi_uim_remote_card_error_enum_type;

/*===========================================================================
   ENUM:      QMI_UIM_REMOTE_POWER_DOWN_MODE_ENUM_TYPE
=============================================================================*/
typedef enum
{
  QMI_UIM_REMOTE_POWER_DOWN_TELECOM_INTERFACE   = 0x0,
  QMI_UIM_REMOTE_POWER_DOWN_CARD,
  QMI_UIM_REMOTE_POWER_DOWN_NOT_APPLICABLE      = 0x7FFFFFFF
} qmi_uim_remote_power_down_mode_enum_type;

/*===========================================================================
   ENUM:      QMI_UIM_REMOTE_VOLTAGE_CLASS_ENUM_TYPE
=============================================================================*/
typedef enum
{
  QMI_UIM_REMOTE_VOLTAGE_CLASS_C_LOW           = 0x0,
  QMI_UIM_REMOTE_VOLTAGE_CLASS_C,
  QMI_UIM_REMOTE_VOLTAGE_CLASS_C_HIGH,
  QMI_UIM_REMOTE_VOLTAGE_CLASS_B_LOW,
  QMI_UIM_REMOTE_VOLTAGE_CLASS_B,
  QMI_UIM_REMOTE_VOLTAGE_CLASS_B_HIGH,
  QMI_UIM_REMOTE_VOLTAGE_CLASS_NOT_APPLICABLE   = 0x7FFFFFFF
} qmi_uim_remote_voltage_class_enum_type;

/*=============================================================================

      S T R U C T    A N D    C A L L B A C K   D A T A    T Y P E S

=============================================================================*/

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_CARD_ATR_DATA_TYPE
=============================================================================*/
typedef struct
{
  uint8              data_len;
  uint8 *            data_ptr;
}qmi_uim_remote_card_atr_data_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_CARD_INSERTED_EVT_TYPE
=============================================================================*/
typedef struct
{
  qmi_uim_remote_card_atr_data_type    atr;
}qmi_uim_remote_card_inserted_evt_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_CARD_RESET_EVT_TYPE
=============================================================================*/
typedef struct
{
  qmi_uim_remote_card_atr_data_type    atr;
}qmi_uim_remote_card_reset_evt_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_CARD_ERROR_EVT_TYPE
=============================================================================*/
typedef struct
{
  qmi_uim_remote_card_error_enum_type    err_cause;
}qmi_uim_remote_card_error_evt_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_EVENT_DATA_TYPE
=============================================================================*/
typedef struct
{
  qmi_uim_remote_evt_enum_type    evt;
  qmi_uim_remote_slot_type        slot;
  boolean                         wakeup_support;
  union {
    qmi_uim_remote_card_inserted_evt_type    card_inserted;
    qmi_uim_remote_card_reset_evt_type       card_reset;
    qmi_uim_remote_card_error_evt_type       card_error;
  } evt_data;
} qmi_uim_remote_event_data_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_RSP_APDU_DATA_TYPE
=============================================================================*/
typedef struct
{
  qmi_uim_remote_result_type    result;
  qmi_uim_remote_slot_type      slot;
  void *                        client_ref_ptr;
  uint16                        rsp_apdu_len;
  uint8 *                       rsp_apdu_ptr;
} qmi_uim_remote_rsp_apdu_data_type;

/*===========================================================================
   FUNCTION PROTOTYPE:      QMI_UIM_REMOTE_EVT_CALLBACK_TYPE
=============================================================================*/
typedef void qmi_uim_remote_evt_callback_type
(
  const qmi_uim_remote_event_data_type    * evt_ptr
);

/*===========================================================================
   FUNCTION PROTOTYPE:      QMI_UIM_REMOTE_APDU_CALLBACK_TYPE
=============================================================================*/
typedef void qmi_uim_remote_apdu_callback_type
(
  const qmi_uim_remote_rsp_apdu_data_type    * rsp_apdu_ptr
);

/*=============================================================================

                   F U N C T I O N S

=============================================================================*/

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_REGISTER

   DESCRIPTION:
     This function registers the calling task with the UIM remote service.
     It also registers the callback for events. The function returns
     immediately and the service sends events with status of the connection
     or slot

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_register (
  qmi_uim_remote_evt_callback_type    * event_cb_ptr,
  qmi_uim_remote_slot_type            slot
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_RELEASE

   DESCRIPTION:
     This function releases the calling task from the UIM remote service

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_release (
  qmi_uim_remote_slot_type    slot
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_CONNECT

   DESCRIPTION:
     This function initiates the establishment of a connection with the remote
     SIM and powers it up

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_connect (
  qmi_uim_remote_slot_type    slot
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_DISCONNCET

   DESCRIPTION:
     This function releases the calling task from the UIM remote service

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_disconnect (
  qmi_uim_remote_slot_type    slot
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_SEND_APDU

   DESCRIPTION:
     This function transmits an APDU from the calling task to the remote SIM
     card. For each APDU, the calling task can indicate a response callback,
     used to receive the response from the SIM card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_send_apdu (
  qmi_uim_remote_slot_type               slot,
  void *                                 client_ref_ptr,
  uint16                                 cmd_apdu_len,
  const uint8 *                          cmd_apdu_ptr,
  qmi_uim_remote_apdu_callback_type    * rsp_apdu_cb_ptr
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_POWER_DOWN

   DESCRIPTION:
     This function powers down the remote SIM card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_power_down (
  qmi_uim_remote_slot_type                  slot,
  qmi_uim_remote_power_down_mode_enum_type  pdown_mode
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_POWER_UP

   DESCRIPTION:
     This function powers up the remote SIM card.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_power_up (
  qmi_uim_remote_slot_type                  slot,
  boolean                                   timeout_isvalid,
  uint32                                    timeout,
  boolean                                   vcc_isvalid,
  qmi_uim_remote_voltage_class_enum_type    vcc
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_RESET

   DESCRIPTION:
     This function can be used by the calling task to perform a warm reset
     of the remote SIM card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_reset (
  qmi_uim_remote_slot_type    slot
);
#endif /* QMI_UIM_REMOTE_H */
