/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            Q M I    U I M    R E M O T E    ST U B S


GENERAL DESCRIPTION

  This source file contains the stubs functions for the QMI UIM 
  remote service functions

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2015 QUALCOMM Technologies, Incorporated and its licensors.
All Rights Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_remote_stubs.c#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/27/15   ks      Addition of stub functions in UIMQMI remote functionality

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "qmi_uim_remote.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

============================================================================*/

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_REGISTER

   DESCRIPTION:
     This function registers the calling task with the UIM remote service.
     The function returns immediately and the service sends events with status
     of the connection or slot

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_register (
  qmi_uim_remote_evt_callback_type    * event_cb_ptr,
  qmi_uim_remote_slot_type              slot
)
{
  (void)event_cb_ptr;
  (void)slot;  
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_register */


/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_SEND_APDU

   DESCRIPTION:
     This function allows the calling task to send an APDU via the QMI UIM
     REMOTE service to a card with which it has connected

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_send_apdu (
  qmi_uim_remote_slot_type               slot,
  void *                                 client_ref_ptr,
  uint16                                 cmd_apdu_len,
  const uint8 *                          cmd_apdu_ptr,
  qmi_uim_remote_apdu_callback_type    * rsp_apdu_cb_ptr
)
{
  (void)slot;
  (void)client_ref_ptr;
  (void)cmd_apdu_len;
  (void)cmd_apdu_ptr;
  (void)rsp_apdu_cb_ptr;
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_send_apdu */


/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_RELEASE

   DESCRIPTION:
     This function un-registers the calling task from the QMI UIM REMOTE
     service.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_release (
  qmi_uim_remote_slot_type            slot
)
{
  (void)slot;
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_release */


/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_CONNECT

   DESCRIPTION:
     This function connects the calling task with a card on a particular slot
     via the QMI UIM REMOTE service. As part of connect the card may be
     powered up and the calling task may receive a remote card event

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_connect (
  qmi_uim_remote_slot_type            slot
)
{
  (void)slot;
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_card_connect */


/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_DISCONNECT

   DESCRIPTION:
     This function disconnects the calling task from a card with which it was
     previously connected via the QMI UIM REMOTE service.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_disconnect (
  qmi_uim_remote_slot_type            slot
)
{
  (void)slot;
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_card_disconnect */


/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_POWER_DOWN

   DESCRIPTION:
     This function allows the calling task to power down (i.e. drive VCC low)
     a card with which it has previously connected via the QMI UIM REMOTE
     service.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_power_down (
  qmi_uim_remote_slot_type                  slot,
  qmi_uim_remote_power_down_mode_enum_type  pdown_mode
)
{
  (void)slot;
  (void)pdown_mode;
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_card_power_down */


/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_POWER_UP

   DESCRIPTION:
     This function allows the calling task to power up (i.e. drive VCC high)
     a card with which it has previously connected via the QMI UIM REMOTE
     service.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_power_up (
  qmi_uim_remote_slot_type                slot,
  boolean                                 timeout_isvalid,
  uint32                                  timeout,
  boolean                                 vcc_isvalid,
  qmi_uim_remote_voltage_class_enum_type  vcc
)
{
  (void)slot;
  (void)timeout_isvalid;
  (void)timeout;
  (void)vcc_isvalid;
  (void)vcc;
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_card_power_up */


/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CARD_RESET

   DESCRIPTION:
     This function allows the calling task to reset (i.e. toggle RST line)
     a card with which it has previously connected via the QMI UIM REMOTE
     service.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_remote_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_remote_result_type qmi_uim_remote_card_reset (
  qmi_uim_remote_slot_type            slot
)
{
  (void)slot;
  return QMI_UIM_REMOTE_ERROR;
} /* qmi_uim_remote_card_reset */

