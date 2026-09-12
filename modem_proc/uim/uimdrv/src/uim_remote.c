/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


       U I M  REMOTE . C

GENERAL DESCRIPTION
  This module contains the procedures for the uim remote client.
  The module receives the event from the QMI REMOTE CLIENT. The module handles the
  commands sent to the QMI remote client and also handles the responses from the
  QMI remote client.

Copyright (c) 2013-2017 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_remote.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/23/17   ku      Improved handling of QMI_UIM_REMOTE_EVT_CARD_REMOVED event in BTSAP
03/13/17   ku      Common path for local and remote card
02/17/17   gm      Masking sensitive information
01/05/16   ku      Correction of extended APDU logging for Remote sim TX data
11/30/16   gm      Store response data in buffer and then commit
11/03/16   ku      APDU  Logging for extended log packets
10/24/16   ks      Appropriate status words handling for remote SIM
09/12/16   ku      Removing MFLO related dead code
08/03/16   ks      Handling 60 xx as UIM_PROBLEM for remote SIM.
06/23/16   sam     F3 messages optimization
05/17/16   ssr     Remove msg for malloc failure
04/20/16   sam     Support for enhanced subscription manager
03/28/16   sam     Support for SAM card
06/24/15   ssr     Fixed KW errors
09/30/15   lm      Fix compiler warning
09/02/15   hyo     Drivers and state machine abstraction
08/11/15   hyo     usb uicc removal
06/12/15   sam     Max atr chars length check for Remote sim
04/15/15   ks      De-Featurizing the FEATURE_UIM_REMOTE/BTSAP_UIM
08/04/15   sam     FR24498: Powerup logging new changes
03/20/15   ks      DO not do GET RESPONSE for the 62 F3
01/07/14   sam     Modification of msg macros for FR24498-UIM Powerup Logging
10/01/14   ll      KW errors
09/18/14   akv     UIMDRV UT enhancements
05/07/14   tkl     Added Vcc and Rsp Timeout for power up request
04/02/14   lxu     Support USB UICC APDU logging
04/01/14   xj      set CMD_RSP_TIME_OUT if USB drv timeout or response size 0
03/27/14   lxu     USB UICC modem implement
03/21/14   ll      Remove return_ptr and remove/change uim_free to UIM_FREE
03/06/14   tkl     Add power down mode for power down request
01/29/14   sam     Usage of newer style diag macros
12/12/13   ks      BTSAP in client mode, intial implementation
===========================================================================*/
#include "uim_remote.h"
#include "uimdrv_main.h"

/*===========================================================================

FUNCTION UIM_REMOTE_COMMAND_RESPONSE_CALLBACK

DESCRIPTION
  This procedure is the call back function provided in the command to the
  driver and upon calling this it initiates the processing of the response
  that has been received from the UIM.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets the UIM_CMD_RSP_SIG.

===========================================================================*/

void uim_remote_command_response_callback
(
  uim_instance_global_type *uim_ptr
)
{

  if(TRUE == uim_nv_is_feature_enabled(UIMDRV_FEATURE_RECOVERY_ON_BAD_STATUS_WORD, uim_ptr) &&
     TRUE == uim_ptr->flag.bad_status_words_error)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"recieved bad status word. forcing recovery");
    uim_force_recovery(uim_ptr);
    return;
  }

  /* clear the command response timer */
  uim_clear_cmd_rsp_timer(uim_ptr);

  /* clear the signal as well just in case if it was set */
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
  
  /* Set the command response signal */
  (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG );
} /*  uim_remote_command_response_callback */


/*===========================================================================
FUNCTION UIM_REMOTE_APDU_RESPONSE_CB

DESCRIPTION
  This procedure is the callback that is called after every QMI REMOTE send command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_remote_apdu_response_cb(const qmi_uim_remote_rsp_apdu_data_type *remote_rsp_apdu_ptr)
{
  const uint8           *rsp_apdu_ptr     = NULL;
  uint16_t              rsp_apdu_size     = 0;
  uint8                 sw1               = 0x00;
  uint8                 sw2               = 0x00;
  uim_global_remote_type  *remote_data_ptr  = NULL;
  uim_instance_global_type *uim_ptr       = NULL;
  uim_instance_enum_type    remote_slot      = 0;

  if(NULL == remote_rsp_apdu_ptr)
  {
    return;
  }

  remote_slot = remote_rsp_apdu_ptr->slot;

  /*convert logical to physical slot ID*/
  remote_slot = (uim_instance_enum_type)(uim_convert_logical_to_physical_slot_id((uim_slot_type)(remote_slot + 1)) - 1);

  uim_ptr = uim_get_instance_ptr(remote_rsp_apdu_ptr->slot);
  if(uim_ptr == NULL || uim_ptr->command.cmd_ptr == NULL)
  {
    return;
  }

  /*Ignore any remote response received if current task is in SAM mode*/
  if(UIM_CARD_SAM  == uim_ptr->card_mode || UIM_SLOT_STATE_INACTIVE == uim_ptr->activity_status)
  {
    UIM_MSG_HIGH_2("ignore remote response: task is in sam mode or inactive. mode-0x%x, activity_status-0x%x",
                    uim_ptr->card_mode, uim_ptr->activity_status);
    return;
  }

  remote_data_ptr   = &(uim_ptr->remote);

  if(NULL == remote_data_ptr)
  {
    return;
  }

  remote_data_ptr->rsp_buf_offset = 0x00;

  /* Ignore response as it is not expected*/
  if((WAIT_FOR_RESPONSE != remote_data_ptr->state)||
     ( NULL == remote_data_ptr->cmd_ptr)||
     ( NULL == remote_data_ptr->cmd_ptr->rsp_ptr))
  {
    UIMDRV_MSG_ERR_1(uim_ptr->id,
                  "Either UIM btsap state : 0x%x not expecting a response or cmd_ptr is NULL",
remote_data_ptr->state);
    return;
  }

  remote_data_ptr->state = WAIT_FOR_NOTHING;
  /* Clear the btsap transaction timer */
  (void) rex_clr_timer( &uim_ptr->remote.remote_cmd_rsp_timer);

  if((QMI_UIM_REMOTE_SUCCESS != remote_rsp_apdu_ptr->result)
     ||(remote_rsp_apdu_ptr->rsp_apdu_len < UIM_REMOTE_SW1_SW2_LENGTH) )
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id, "Invalid resp received setting TRANSACTION SIG");
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }

  rsp_apdu_ptr  = remote_rsp_apdu_ptr->rsp_apdu_ptr;
  rsp_apdu_size = remote_rsp_apdu_ptr->rsp_apdu_len;

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_DEBUG_LOG,
                               uim_ptr) == TRUE)
  {
    /* Logging for extended packets */
    uim_log_put_apdu_data(FALSE, TRUE, (uint8 *)rsp_apdu_ptr, rsp_apdu_size, uim_ptr);
  }

  /* Determine the status words in the command */
  sw1 = *(rsp_apdu_ptr + rsp_apdu_size -2);
  sw2 = *(rsp_apdu_ptr + rsp_apdu_size -1);
  rsp_apdu_size = rsp_apdu_size - UIM_REMOTE_SW1_SW2_LENGTH;

  if((UIM_INSTRN_CASE_1 == remote_data_ptr->cmd_ptr->instrn_case)
      || (UIM_INSTRN_CASE_3 == remote_data_ptr->cmd_ptr->instrn_case))
  {
    rsp_apdu_size = 0;
  }

  /* Checking if accumulated response data is more than 256 bytes  */
  if (rsp_apdu_size + remote_data_ptr->rsp_buf_offset > UIM_REMOTE_MAX_RAPDU_LENGTH)
  {
    remote_data_ptr->extended_data.extended_data_ptr = (uint8 *) uim_malloc((rsp_apdu_size + remote_data_ptr->rsp_buf_offset));
    if(remote_data_ptr->extended_data.extended_data_ptr == NULL)
    {
      rsp_apdu_size  = UIM_REMOTE_MAX_RAPDU_LENGTH - remote_data_ptr->rsp_buf_offset;
    }
    else
    {
      remote_data_ptr->extended_data.no_need_send_get_response_to_qmi = TRUE;
      if(remote_data_ptr->rsp_buf_offset > 0)
      {
        uim_memscpy(remote_data_ptr->extended_data.extended_data_ptr,
            (remote_data_ptr->rsp_buf_offset),
               &(remote_data_ptr->cmd_ptr->rsp_ptr->rsp.data),
                (remote_data_ptr->rsp_buf_offset));
      }
      uim_memscpy(remote_data_ptr->extended_data.extended_data_ptr + remote_data_ptr->rsp_buf_offset,
        rsp_apdu_size,
             rsp_apdu_ptr,
             rsp_apdu_size);

      remote_data_ptr->extended_data.length  = (rsp_apdu_size + remote_data_ptr->rsp_buf_offset);
      remote_data_ptr->extended_data.offset  = 0;
      remote_data_ptr->rsp_buf_offset        = 0;
      remote_data_ptr->extended_data.ext_sw1 = sw1;
      remote_data_ptr->extended_data.ext_sw2 = sw2;
      /* sw1 sw2 as 61 00 to indicate response is pending in eztended buffer */
      remote_data_ptr->cmd_ptr->rsp_ptr->sw1 = 0x61;
      remote_data_ptr->cmd_ptr->rsp_ptr->sw2 = 0x00;
      remote_data_ptr->cmd_ptr->rsp_ptr->cmd_status   = UIM_REMOTE_STATUS;
      remote_data_ptr->cmd_ptr->rsp_ptr->cmd_rsp_size = 0;
      uim_remote_command_response_callback(uim_ptr);
      return;
    }
  }

  /* Response data received is less than 256 bytes */
  if(rsp_apdu_size)
  {
    uim_memscpy(&(remote_data_ptr->cmd_ptr->rsp_ptr->rsp) + remote_data_ptr->rsp_buf_offset,
        rsp_apdu_size,
                rsp_apdu_ptr,
                rsp_apdu_size );
  }

  remote_data_ptr->cmd_ptr->rsp_ptr->cmd_status   = UIM_REMOTE_STATUS;
  remote_data_ptr->rsp_buf_offset = remote_data_ptr->rsp_buf_offset + rsp_apdu_size;
  remote_data_ptr->cmd_ptr->rsp_ptr->cmd_rsp_size = remote_data_ptr->rsp_buf_offset;
  remote_data_ptr->cmd_ptr->rsp_ptr->sw1          = sw1;
  remote_data_ptr->cmd_ptr->rsp_ptr->sw2          = sw2;
  uim_remote_command_response_callback(uim_ptr);

  return;
}/* uim_remote_apdu_response_cb */

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
  uim_ptr->card_cmd.num_resp_bytes_rcvd_total = uim_ptr->remote.cmd_ptr->rsp_ptr->cmd_rsp_size;
  uim_ptr->remote.intermediate_done           = FALSE;
  UIMDRV_MSG_HIGH_3(uim_ptr->id,"the length status word sw1 sw2 0x%x 0x%x 0x%x ",uim_ptr->remote.cmd_ptr->rsp_ptr->cmd_rsp_size, uim_ptr->card_cmd.resp_buf_ptr->sw1, uim_ptr->card_cmd.resp_buf_ptr->sw2);
  uim_rx_isr_process_sw2_byte(uim_ptr, uim_ptr->card_cmd.resp_buf_ptr->sw1, uim_ptr->card_cmd.resp_buf_ptr->sw2);
}/* uim_remote_get_command_status */

/*===========================================================================
FUNCTION UIM_REMOTE_SEND_ATR

DESCRIPTION
  This procedure stores the ATR data of the remote sim in the appropriate buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_remote_send_atr(uim_instance_global_type *uim_ptr, const qmi_uim_remote_card_atr_data_type *atr_data_ptr)
{
  uint8   atr_len        = 0;
  uint8   num_hist_chars = 0;

  if(NULL == uim_ptr || NULL == atr_data_ptr)
  {
    return;
  }

  atr_len                        = (uint8)(atr_data_ptr->data_len);

  if(UIM_MAX_ATR_CHARS < atr_len)
  {
    atr_len = UIM_MAX_ATR_CHARS;
  }
  uim_ptr->atr.atr_buf.num_bytes = atr_len;

  uim_memscpy(uim_ptr->atr.atr_buf.data,atr_len,atr_data_ptr->data_ptr, atr_len);
  num_hist_chars = (uim_ptr->atr.atr_buf.data[UIM_ATR_TS_BYTE_INDEX]  & UIM_K_FIELD);

  /*discard the historical characters if atr_len is less that num_hist_chars*/
  if(num_hist_chars < uim_ptr->atr.atr_buf.num_bytes)
  {
    /* Copy the historical characters to the historical char  buffer. */
    uim_ptr->atr.hist_char_buf.length = num_hist_chars;
    uim_memscpy (uim_ptr->atr.hist_char_buf.data,num_hist_chars,
        (&(uim_ptr->atr.atr_buf.data[(atr_len - num_hist_chars)])),
         num_hist_chars);
  }

  uim_ptr->command.rsp_buf.cmd_status         = UIM_DONE;
  uim_ptr->remote.state                       = WAIT_FOR_NOTHING;
  /* Set the flag to TRUE */
  uim_ptr->atr.atr_pps_done                   = TRUE;
  (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG );
  return;
} /* uim_remote_send_atr */

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
  qmi_uim_remote_result_type result     = QMI_UIM_REMOTE_ERROR;
  if(uim_ptr == NULL)
  {
    return;
  }
  if(uim_ptr->remote.btsap.handle & UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK)
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"Connection Unavailable event triggered.No need to call powerdown API");
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }

  result =   uim_interface_qmi_uim_remote_card_power_down((qmi_uim_remote_slot_type)uim_ptr->id,
                                                           QMI_UIM_REMOTE_POWER_DOWN_CARD);

  if(result != QMI_UIM_REMOTE_SUCCESS)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"Could powerdown the remote card");
  }
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
  qmi_uim_remote_result_type result  = QMI_UIM_REMOTE_ERROR;
  if(uim_ptr == NULL)
  {
    return;
  }
  if(uim_ptr->remote.btsap.handle & UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK)
  {
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }

  uim_ptr->remote.btsap.handle = (uim_ptr->remote.btsap.handle | UIM_BTSAP_CARD_CONNCECTTION_ENABLE_MASK) ;
  uim_ptr->remote.btsap.handle = (uim_ptr->remote.btsap.handle & UIM_BTSAP_DISABLE_RECOVERY_MASK);
  uim_ptr->remote.state  = WAIT_FOR_ATR;
  /* Set the transaction timer prior to connecting to remote card.
     Clear it once ATR is received */
  (void)rex_set_timer(&uim_ptr->remote.remote_cmd_rsp_timer,
            uim_ptr->remote.btsap.btsap_cmd_rsp_timer_value);
  UIMDRV_MSG_HIGH_1(uim_ptr->id,"Sending the qmi remote connect API with timer set to 0x%x",
                             uim_ptr->remote.btsap.btsap_cmd_rsp_timer_value);
  result = uim_interface_qmi_uim_remote_card_connect((qmi_uim_remote_slot_type)uim_ptr->id);

  if(result != QMI_UIM_REMOTE_SUCCESS)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"Could not connect to the remote card");
    (void) rex_clr_timer( &uim_ptr->remote.remote_cmd_rsp_timer);
  }
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
  qmi_uim_remote_result_type result  = QMI_UIM_REMOTE_ERROR;
  if(uim_ptr == NULL)
  {
    return;
  }
  if(uim_ptr->remote.btsap.handle & UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK)
  {
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }
  uim_ptr->remote.state  = WAIT_FOR_ATR;
  uim_ptr->remote.btsap.handle = (uim_ptr->remote.btsap.handle & UIM_BTSAP_DISABLE_RECOVERY_MASK);

  /* Set the transaction timer prior to power-up the remote card.
     Clear it once ATR is received */
  (void)rex_set_timer(&uim_ptr->remote.remote_cmd_rsp_timer,
                   uim_ptr->remote.btsap.btsap_cmd_rsp_timer_value);

  result = uim_interface_qmi_uim_remote_card_power_up((qmi_uim_remote_slot_type)uim_ptr->id,
                                        FALSE,
                                        0,
                                        FALSE,
                                        QMI_UIM_REMOTE_VOLTAGE_CLASS_NOT_APPLICABLE);
  if(result != QMI_UIM_REMOTE_SUCCESS)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"Couldn't powerup the remote card");
    /* Clear the transaction timer */
    (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
  }
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
  qmi_uim_remote_result_type result = QMI_UIM_REMOTE_ERROR ;
  if(uim_ptr == NULL)
  {
    return;
  }
  if(uim_ptr->remote.btsap.handle & UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK)
  {
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }
  uim_ptr->remote.state  = WAIT_FOR_ATR;
  uim_ptr->remote.btsap.handle = (uim_ptr->remote.btsap.handle & UIM_BTSAP_DISABLE_RECOVERY_MASK);
  /* Set the transaction timer prior to power-up the remote card.
     Clear it once ATR is received */
  (void)rex_set_timer(&uim_ptr->remote.remote_cmd_rsp_timer,
                   uim_ptr->remote.btsap.btsap_cmd_rsp_timer_value );
  result = uim_interface_qmi_uim_remote_card_reset((qmi_uim_remote_slot_type)uim_ptr->id);
  if(result != QMI_UIM_REMOTE_SUCCESS)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"Could reset the remote card");
    (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
  }
  return;
}/* uim_remote_card_reset */

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
  uint16_t  cmd_apdu_size     = 0;
  uint8     *cmd_apdu_data_ptr    = NULL;
  uim_global_remote_type  *remote_data_ptr = NULL;

  if(uim_ptr == NULL)
  {
    return;
  }
  remote_data_ptr = &(uim_ptr->remote);
  uim_ptr->state.last_instrn_case = uim_req_ptr->instrn_case;
  
  if(remote_data_ptr->btsap.handle & UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK)
  {
    (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }
  if (remote_data_ptr->btsap.handle & UIM_BTSAP_TRIGGER_RECOVERY_MASK)
  {
    remote_data_ptr->btsap.handle = (remote_data_ptr->btsap.handle & UIM_BTSAP_DISABLE_RECOVERY_MASK);
    if(uim_ptr->command.mode != UIM_RECOVERY_MODE)
    {
      (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
      /* Set the command response timeout signal  */
      (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
      return;
    }
  }

  /* Copy the contents of APDU header into last sent APDU header */
  uim_ptr->debug.last_sent_apdu = uim_req_ptr->apdu_hdr;

  /* Based on the instruction case determine the size of the
     command APDU to be sent to the card */
  switch (uim_req_ptr->instrn_case)
  {
    case UIM_INSTRN_CASE_1:  /* Case 1 - No data exchanged */
    case UIM_INSTRN_CASE_2:  /* Case 2 - Response data only */
    {
      cmd_apdu_size = UIM_REMOTE_APDU_HDR_SIZE;
      break;
    }

    case UIM_INSTRN_CASE_3:  /* Case 3 - Command data only */
    case UIM_INSTRN_CASE_4:  /* Case 4 - Response and command data */
    {
      cmd_apdu_size = UIM_REMOTE_APDU_HDR_SIZE + uim_req_ptr->apdu_hdr.p3;
      break;
    }
    default:
    {
      /* The instruction case is not valid. Cannot process command.
         Create a failure through the time out mechanism. */
      UIMDRV_MSG_ERR_1(uim_ptr->id,"Err instrn case=0x%x.",
                                  uim_req_ptr->instrn_case);
      /* Clear the cmd_rsp_timer first */
      (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
      /* Set the command response timeout signal */
      (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
      return;
    }
  }

    /* Allocate memory based on computed size */
  cmd_apdu_data_ptr = (uint8 *)uim_malloc(cmd_apdu_size);
  if (cmd_apdu_data_ptr == NULL)
  {
    /* Clear the cmd_rsp_timer first */
    (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }

  /* Setup the command APDU to be sent to the card */
  *cmd_apdu_data_ptr       = uim_req_ptr->apdu_hdr.uim_class;
  *(cmd_apdu_data_ptr + 1) = uim_req_ptr->apdu_hdr.instrn;
  *(cmd_apdu_data_ptr + 2) = uim_req_ptr->apdu_hdr.p1;
  *(cmd_apdu_data_ptr + 3) = uim_req_ptr->apdu_hdr.p2;
  *(cmd_apdu_data_ptr + 4) = uim_req_ptr->apdu_hdr.p3;

  /* If the command APDU contains command data copy it into
       the command APDU buffer prior to sending it to the card */
  if (cmd_apdu_size > UIM_REMOTE_APDU_HDR_SIZE)
  {
    uim_memscpy((cmd_apdu_data_ptr + UIM_REMOTE_APDU_HDR_SIZE),
                (cmd_apdu_size - UIM_REMOTE_APDU_HDR_SIZE),
                uim_req_ptr->req.data,
                (cmd_apdu_size - UIM_REMOTE_APDU_HDR_SIZE));
  }

  if(uim_req_ptr->apdu_hdr.instrn == GET_RESPONSE)
  {
    if(remote_data_ptr->extended_data.no_need_send_get_response_to_qmi == TRUE)
    {
      uim_remote_get_entire_response(uim_ptr);
      uim_remote_command_response_callback(uim_ptr);
      UIM_FREE(cmd_apdu_data_ptr);      
      return;
    }
  }
  else
  {
    remote_data_ptr->rsp_buf_offset       = 0;
    remote_data_ptr->extended_data.offset = 0;
    remote_data_ptr->extended_data.length = 0;
    remote_data_ptr->cmd_ptr = uim_req_ptr;
  }

  /* Clear the transaction signal prior to setting the timer */
  (void)rex_clr_sigs(uim_ptr->tcb_ptr,UIM_TRANSACTION_SIG);

  /* Set the transaction timer prior to issuing a command */
  if (IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
  {
    (void)rex_set_timer(&uim_ptr->remote.remote_cmd_rsp_timer,
                        uim_ptr->remote.btsap.btsap_cmd_rsp_timer_value);
  }

  remote_data_ptr->state = WAIT_FOR_RESPONSE;

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_DEBUG_LOG,
                               uim_ptr) == TRUE)
  {
    /* Logging for extended log packets */
    uim_log_put_apdu_data(TRUE, TRUE, cmd_apdu_data_ptr, cmd_apdu_size, uim_ptr);
  }
  uim_interface_qmi_uim_remote_send_apdu((qmi_uim_remote_slot_type)uim_ptr->id,
                           NULL,
                           cmd_apdu_size,
                           cmd_apdu_data_ptr,
                           uim_remote_apdu_response_cb);
  UIM_FREE(cmd_apdu_data_ptr);
  return;
}/* uim_remote_send_command */


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
  if(uim_ptr == NULL)
  {
    return;
  }

  if( ( !IS_BTSAP_HANDLE_ACTIVE(uim_ptr)) || (uim_req_ptr == NULL))
  {
    (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
    return;
  }
  uimdrv_set_card_cmd_data(uim_req_ptr, uim_ptr);
  uim_remote_send_apdu(uim_req_ptr, uim_ptr);

}/* uim_remote_send_command */



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
void  uim_remote_get_entire_response(uim_instance_global_type *uim_ptr)
{
  uim_req_buf_type      *get_rsp_apdu_ptr  = NULL;
  uim_global_remote_type *remote_data_ptr    = NULL;
  uint8              sw1;
  uint8          sw2;
  uint16                 pending_size = 0;

  if(uim_ptr == NULL)
  {
    return;
  }
  remote_data_ptr = &(uim_ptr->remote);

  sw1 =   remote_data_ptr->cmd_ptr->rsp_ptr->sw1;
  sw2 =   remote_data_ptr->cmd_ptr->rsp_ptr->sw2;
  /* Since data is already cached in extended buffer,we dont send next GET_RESPONSE cmd to remote card*/
  pending_size  = (remote_data_ptr->extended_data.length - remote_data_ptr->extended_data.offset);
  UIMDRV_MSG_HIGH_2(uim_ptr->id,"extended data lenegth 0x%x, 0x%x",remote_data_ptr->extended_data.length, pending_size);
  if(pending_size > 256)
  {
    uim_ptr->command.rsp_buf.sw1              = 0x61;
    uim_ptr->command.rsp_buf.sw2              = 0x00;
    uim_ptr->command.rsp_buf.cmd_status   = UIM_DONE;
    uim_memscpy((uim_ptr->command.rsp_buf.rsp.data),UIM_REMOTE_MAX_RAPDU_LENGTH,
            ((remote_data_ptr->extended_data.extended_data_ptr) + (remote_data_ptr->extended_data.offset)),
             (UIM_REMOTE_MAX_RAPDU_LENGTH ));
    uim_ptr->command.rsp_buf.cmd_rsp_size = UIM_REMOTE_MAX_RAPDU_LENGTH;
    remote_data_ptr->extended_data.offset = remote_data_ptr->extended_data.offset + UIM_REMOTE_MAX_RAPDU_LENGTH;
  }
  else
  {
    uim_memscpy(&(remote_data_ptr->cmd_ptr->rsp_ptr->rsp.data),pending_size,
            ((remote_data_ptr->extended_data.extended_data_ptr) + (remote_data_ptr->extended_data.offset)),
            (pending_size));
    uim_ptr->command.rsp_buf.sw1 = remote_data_ptr->extended_data.ext_sw1;
    uim_ptr->command.rsp_buf.sw2 = remote_data_ptr->extended_data.ext_sw2;
    uim_ptr->command.rsp_buf.cmd_status  = UIM_DONE;
    uim_ptr->command.rsp_buf.cmd_rsp_size  = pending_size;
    remote_data_ptr->rsp_buf_offset       = 0;
    remote_data_ptr->extended_data.offset = 0;
    remote_data_ptr->extended_data.length = 0;
    remote_data_ptr->extended_data.no_need_send_get_response_to_qmi = FALSE;
    UIM_FREE(remote_data_ptr->extended_data.extended_data_ptr);
    }
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
  uim_instance_global_type *uim_ptr = (uim_instance_global_type*)param;

  if(uim_ptr == NULL)
  {
    return;
  }
  UIMDRV_MSG_ERR_0(uim_ptr->id, "No response from remote sim for the command");
  /* Clear the cmd_rsp_timer_ptr first */
   (void) rex_clr_timer( &uim_ptr->remote.remote_cmd_rsp_timer);
  /* Set the command response timeout signal */

  (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
  return;
} /* uim_remote_cmd_rsp_timer_expiry_cb */
