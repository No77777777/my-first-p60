/*===========================================================================

                            U I M D R V  H A L  R E M O T E

DESCRIPTION
  This file contains driver functions that are specific to UIM REMOTE

Copyright (c) 2015-2017 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimdrv_hal_remote.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/13/17   ku      Common path for local and remote card
06/23/16   sam     F3 messages optimization
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "uim_remote.h"
#include "uimdrv_hal_remote.h"
#include "uimdrv_main.h"
#include "uimdrv_msg.h"

/*============================================================================= 
  FUNCTION UIMDRV_REMOTE_POWER_UP
=============================================================================*/
void uimdrv_remote_power_up
(
  uim_instance_global_type *uim_ptr
)
{
  UIMDRV_MSG_LOW_0(uim_ptr->id,"UIM REMOTE enabled. Doing nothing in power up.");
} /* uimdrv_remote_power_up */


/*============================================================================= 
  FUNCTION UIMDRV_REMOTE_POWER_DOWN
=============================================================================*/
void uimdrv_remote_power_down
(
  uim_instance_global_type *uim_ptr
)
{
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Calling QMI remote API to power down the remote SIM");
  
  /*cmd_rsp_sig is set in the error event which is generated due to power down */
  uim_remote_card_power_down(uim_ptr);
} /* uimdrv_remote_power_down */


/*============================================================================= 
  FUNCTION UIMDRV_REMOTE_RESET
=============================================================================*/
void uimdrv_remote_reset
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"UIM in BTSAP client mode.Call BTSAP API's to power-up remote card");
  if(!(uim_ptr->remote.btsap.handle & UIM_BTSAP_CARD_CONNCECTTION_ENABLE_MASK))
  {
    /* Call card_connect API for the first time */
    uim_remote_card_connect(uim_ptr);
  }
  else if(uim_ptr->command.cmd_ptr->hdr.command == UIM_HOTSWAP_CARD_INS_F)
  {
    /* Call card_reset API during HOTSWAP INSERTED command */
    uim_remote_card_reset(uim_ptr);
  }
  else
  {
    /* Call card_power_up API from second time onwards */
    uim_remote_card_power_up(uim_ptr);
  }
} /* uimdrv_remote_reset */


/*============================================================================= 
  FUNCTION UIMDRV_REMOTE_UPDATE_OP_PARAM
=============================================================================*/
boolean uimdrv_remote_update_op_param
(
  uim_op_params_type const *op_params,
  uim_instance_global_type *uim_ptr
)
{
  return FALSE;
} /* uimdrv_remote_update_op_param */


/*============================================================================= 
  FUNCTION UIMDRV_REMOTE_SEND_COMMAND
=============================================================================*/
void uimdrv_remote_send_command
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
)
{
  UIMDRV_MSG_HIGH_0(uim_ptr->id,
                    "UIM in BTSAP client mode. Calling BTSAP API's to send cmd to remote card");

  uim_remote_send_command(uim_req, uim_ptr);
} /* uimdrv_remote_send_command */


/*============================================================================= 
  FUNCTION UIMDRV_REMOTE_SEND_APDU
=============================================================================*/
void uimdrv_remote_send_apdu
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
)
{
  UIMDRV_MSG_HIGH_0(uim_ptr->id,
                    "UIM in BTSAP client mode. Calling BTSAP API's to send cmd to remote card");

  uim_remote_send_apdu(uim_req, uim_ptr);
} /* uimdrv_remote_send_apdu */

