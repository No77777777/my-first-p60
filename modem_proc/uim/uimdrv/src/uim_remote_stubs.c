/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


       U I M   R E M O T E   S T U B S . C

GENERAL DESCRIPTION
  This module contains the stub functions for the uim remote client.

Copyright (c) 2016 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_remote_stubs.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/10/17   kv      Enable subs manager and sam card mode
03/13/17   ku      Common path for local and remote card
07/24/15   ks      UIM remote stub functions
===========================================================================*/

#include "uim_remote.h"

/*===========================================================================
 FUNCTION:      UIM_REMOTE_CARD_POWER_DOWN

 DESCRIPTION:
     This function powers down the remote SIM card.

 DEPENDENCIES
  None

 RETURN VALUE
  None

 SIDE EFFECTS
  None
=============================================================================*/
void uim_remote_card_power_down(uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;
  return;
}/* uim_remote_card_power_down */

/*===========================================================================
 FUNCTION:      UIM_REMOTE_CARD_CONNECT

 DESCRIPTION:
    This function initiates the establishment of a connection with the remote
    SIM and powers it up.

 DEPENDENCIES
  None

 RETURN VALUE
  None

 SIDE EFFECTS
  None
=============================================================================*/
void uim_remote_card_connect(uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;
  return;
}/* uim_remote_card_connect */

/*===========================================================================
 FUNCTION:      UIM_REMOTE_CARD_POWER_UP

 DESCRIPTION:
  This function powers up the remote SIM card.

 DEPENDENCIES
  None

 RETURN VALUE
  None

 SIDE EFFECTS
  None
=============================================================================*/
void uim_remote_card_power_up(uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;
  return;
}/* uim_remote_card_power_up */

/*===========================================================================
 FUNCTION:      UIM_REMOTE_CARD_RESET

 DESCRIPTION:
     This function can be used by the calling task to perform a warm reset
     of the remote SIM card.

 DEPENDENCIES
  None

 RETURN VALUE
  None

 SIDE EFFECTS
  None
=============================================================================*/
void uim_remote_card_reset(uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;  
  return;
}/* uim_remote_card_reset */

/*===========================================================================
FUNCTION UIM_REMOTE_SEND_COMMAND

DESCRIPTION:
 This function transmits an APDU from the calling task to the remote SIM
 card. For each APDU, the calling task can indicate a response callback,
 used to receive the response from the SIM card.

LIMITATIONS:
 None

RETURN VALUE:
 void

SIDE EFFECTS:
 None.
===========================================================================*/
void uim_remote_send_command(uim_req_buf_type *uim_req_ptr , uim_instance_global_type *uim_ptr )
{
  (void)uim_ptr;
  return;
}/* uim_remote_send_command */



/*===========================================================================
FUNCTION UIM_REMOTE_SEND_APDU

DESCRIPTION:
 This function transmits an APDU from the calling task to the remote SIM
 card. For each APDU, the calling task can indicate a response callback,
 used to receive the response from the SIM card.

LIMITATIONS:
 None

RETURN VALUE:
 void

SIDE EFFECTS:
 None.
===========================================================================*/
void uim_remote_send_apdu(uim_req_buf_type *uim_req_ptr , uim_instance_global_type *uim_ptr )
{
  (void)uim_ptr;
  return;
} /* uim_remote_send_apdu */

/*===========================================================================
FUNCTION UIM_REMOTE_GET_ENTIRE_RESPONSE

DESCRIPTION:
 This function is to process the intermediate status words
 and send GET_RESPONSE command to remote card if needed

LIMITATIONS:
 None

RETURN VALUE:
 boolean.
 TRUE - UIM TASK contiues from the main 'for loop' in UIM_TASK_COMMON
 FALSE - Process the response.

SIDE EFFECTS:
 None.
===========================================================================*/
void uim_remote_get_entire_response(uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;
  return;
}/* uim_remote_get_entire_response */

/*===========================================================================
FUNCTION UIM_BTSAP_CMD_RSP_TIMER_EXPIRY_CB

DESCRIPTION
  This procedure is registered as a call-back associated with the expiry of
  the btsap cmd rsp timer. This call-back is called in the context of the high
  priority timer task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Power down and power up the remote SIM interface if the command response timeout expired.
===========================================================================*/
void uim_remote_cmd_rsp_timer_expiry_cb(timer_cb_data_type param)
{
  (void)param;
  return;
} /* uim_remote_cmd_rsp_timer_expiry_cb */


/*===========================================================================
FUNCTION UIM_REMOTE_GET_COMMAND_STATUS

DESCRIPTION
  This procedure updates the command status based on sw1 sw2 from remote card.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_remote_get_command_status(uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;
  return;
}/* uim_remote_get_command_status */


/*===========================================================================
FUNCTION UIM_BTSAP_INIT

DESCRIPTION
  This procedure registers uim with the QMI REMOTE client
  After this registration, events from QMI REMOTE client are received

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void uim_btsap_init (uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;
  return;
}

/*===========================================================================
FUNCTION UIM_BTSAP_SAVE_CURRENT_MODE_TO_EFS

DESCRIPTION
  This procedure write the connection mode (ISO\Remote) and Delay time for
  remote connection in EFs file.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_btsap_save_current_mode_to_efs (uim_instance_global_type *uim_ptr ,  
                                          uim_slot_connection_enum_type uim_mode)
{
  (void)uim_ptr;
  (void)uim_mode;
  return;
}


/*===========================================================================
FUNCTION UIM_BTSAP_RELEASE_CONNECTION_REQ

DESCRIPTION
  This procedure sets the handle and state of BTSAP appropriately before releasing connection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_btsap_release_connection_req(uim_instance_global_type *uim_ptr)
{
  (void)uim_ptr;
  return;
}/* uim_btsap_release_connection_req */