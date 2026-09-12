/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 U I M   SAM  S T A T E   M A C H I N E

GENERAL DESCRIPTION
  This module contains the state machine for the SAM card of the UIM
  interface.

EXTERNALIZED FUNCTIONS

  uim_process_sam_command
    Processes a isim UIM server command

  uim_process_sam_response
    Processes the responses for a isim UIM server command

  uim_sam_command
    This function contains the state machine that processes APDUs
    for isim commands.

  uim_sam_command_response
    This function processes the UIM responses from APDUs.

Copyright (c) 2016 - 2017, 2020 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimsam.c#2 $
$DateTime: 2020/02/11 10:00:35 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/11/20   ykr     Resetting the extended data offset during extended APDU
04/14/17   kv      Added pass-through mode support
06/23/16   sam     F3 messages optimization
04/20/16   sam     Support for enhanced subscription manager
03/28/16   sam     proper reseting of logical_slot_powered_down flag
03/28/16   sam     Created module.  This module contains the SAM state
                   machine.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "intconv.h"
#include "customer.h"
#include "comdef.h"
#include "uimdrv_msg.h"
#include "err.h"
#include "uimdrv_main.h"
#include "uimdrv.h"
#include "uimi.h"
#include "uimisim.h"
#include "uimutil.h"
#include <memory.h>
#include "uimglobals.h"
#include "mmgsdilib_p.h"
#include "uimgen_hal.h"
#include "uimsam.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* State configuration for the commands when task is in SAM phase. */
/*  UIM_INTERNAL_ME_PUP_F */
static const uim_generic_state_type UIM_SAM_INTERNAL_PUP_STATES[] =
{ UIM_POWER_UP_ST, UIM_RESET_ST,
  UIM_DELAY_AFTER_ATR_ST, UIM_PPS_ST, UIM_UPDATE_OP_PARAMS_ST,
#if defined( FEATURE_UIM_T_1_SUPPORT )
  UIM_IFS_NEG_ST,
#endif /* FEATURE_UIM_T_1_SUPPORT */
  UIM_DONE_ST };


/*===========================================================================

FUNCTION UIM_SAM_STREAM_APDU_SUCCESS

DESCRIPTION
  This procedure handles a successful stream apdu response

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
boolean uim_sam_stream_apdu_success
(
  uim_instance_global_type *uim_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr,
  uim_rsp_buf_type         *rsp_ptr
)
{
  (void)uim_req_buf_static_ptr;
  if(rsp_ptr->cmd_rsp_size > 0)
  {
    /* Try to allocate mem and reset all the variables */
    if( !uim_alloc_set_rd_wr_info(rsp_ptr->cmd_rsp_size, uim_ptr) )
    {
      uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
      return FALSE;
    }

    /* Copy over the data to the buffer provided*/
    uim_memscpy(uim_ptr->command.rd_wr_info.data_ptr ,
           rsp_ptr->cmd_rsp_size,
           rsp_ptr->rsp.data,
           rsp_ptr->cmd_rsp_size);

    uim_ptr->command.rd_wr_info.num_bytes_rsp = rsp_ptr->cmd_rsp_size;
  }
  ++uim_ptr->command.generic_state_ptr;
  return TRUE;
} /* uim_sam_stream_apdu_success */

/*===========================================================================

FUNCTION UIM_SAM_PROCESS_CARD_RESPONSE                

DESCRIPTION
  This procedure processes the response received from the SAM card and changes
  the UIM server control variables based upon the processing status.

DEPENDENCIES
  This function uses global variables defined for the UIM server to determine
  the proper processing of the card response.

RETURN VALUE
  None.

SIDE EFFECTS
  The control variables of the UIM server are adjusted to reflect the change
  in the operation of the UIM server as a result of the processing on the
  response.

===========================================================================*/
void uim_sam_process_card_response
(
  rex_sigs_type            *i_mask,    /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr
)
{
  /* UIM result type */
  uim_cmd_status_type  status   = UIM_CMD_SUCCESS; 
  uim_slot_type        cmd_slot = UIM_SLOT_NONE;

  UIMDRV_MSG_LOW_0(uim_ptr->id,"uim_sam_process_card_response");

  if (i_mask == NULL)
  {
    ERR_FATAL("uim_sam_process_card_response i_mask  is NULL",0,0,0);
  }

  /* This is a case where UIM recvied card removed and a
     succesful response from the UART ISR */
  if ( uim_ptr->flag.static_buffer_used == TRUE &&
       uim_ptr->command.static_cmd_buf.hdr.command == UIM_HOTSWAP_CARD_REM_F &&
      (uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE ))
  {
    if (uim_ptr->command.rsp_buf.sw1 != SW1_NORMAL_END)
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id,"HOTSWAP: Need to ignore this response");
    }
    uim_ptr->command.rsp_buf.sw1 = SW1_NORMAL_END;
    uim_ptr->command.rsp_buf.sw2 = 0x00;
  }

  /*invoke uim_sam_command_response to process the recieved response*/
  if ((uim_ptr->flag.static_buffer_used) || (uim_ptr->command.cmd_ptr != NULL))
  {
    cmd_slot = uim_ptr->command.cmd_ptr->hdr.slot;
    status = uim_sam_command_response(&uim_ptr->command.rsp_buf, uim_ptr);
  }
  else
  {
     UIMDRV_MSG_HIGH_0(uim_ptr->id,"cmd_ptr is NULL");
  }

  /*status returned by the uim_sam_command_response*/

  /*if voltage mismatch recieved. invoke process voltage mismatch*/
  if (status == UIM_VOLTAGE_MISMATCH)
  {
    uim_process_voltage_mismatch(i_mask, uim_ptr, status);
  }
  /*if Clock frequence change required*/
  else if (status == UIM_CLK_FREQ_CHANGE_NEEDED)
  {
    uim_reset_uim (i_mask, uim_ptr->flag.me_powerup, uim_ptr);
  }
  /*if Convention change or voltage requried*/
  else if (status == UIM_CONVENTION_CHANGE || status == UIM_SWITCH_VOLTAGE)
  {
    uim_process_convention_mismatch(i_mask, uim_ptr, status);
  }
  /*command is succesfully completed*/
  else if (status == UIM_CMD_COMPLETED)
  {
    /* Set the flag to false since the command is completed */
    uim_ptr->flag.command_in_progress = FALSE;

    /* Reset the internal command used flag */
    uim_ptr->flag.internal_command_used = FALSE;

    /* include the command Q sig to the mask */
    *i_mask |= UIM_CMD_Q_SIG;

    if (uim_ptr->flag.me_powerup)
    {
      if(uim_ptr->command.cmd_ptr)
      {
      if (uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F)
      {
        uim_ptr->state.status = UIM_INITIALIZED_S;
      }
      }

      /* Do not send LINK established message when status is UIM_ERR_S */
      if(UIM_ERR_S != uim_ptr->state.status)
      {
        /*clearing TP Cache*/
        uim_ptr->cache.int_term_profile_data.num_bytes = 0;
        /* Send a link established messgae to GSDI */
        uim_gsdi_link_est(uim_ptr);
        /*reset the notify_card_indication flag, for next activation*/
        uim_ptr->flag.card_activation_indicated_to_mmgsdi = FALSE;
      }

      uim_ptr->flag.me_powerup = FALSE;
    }
 if (uim_ptr->flag.static_buffer_used)
    {
      /* Set the status as Initialized if the power up command is
         completed successfully */
      if ((uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_ME_PUP_F)||
               (uim_ptr->command.static_cmd_buf.hdr.command == UIM_HOTSWAP_CARD_INS_F) ||
               (uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_WAKE_UP_F)
              )
      {
        /* Reset the counters */
        uim_ptr->debug.convention_change_count = 1;

        if(UIM_ERR_S != uim_ptr->state.status)
        {
          uim_ptr->state.status = UIM_INITIALIZED_S;
        }
        /* Reset the static command count */
        uim_ptr->command.static_cmd_buf.hdr.cmd_count = 1;

        /* reset the static buffer used flag */
        uim_ptr->flag.static_buffer_used = FALSE;
      } /* end if - powerup command */
    }
    /*if it is logical slot 1. then reset the phase to telecom if we have gone into error state. 
      so that on next powerup we start afresh from telecom powerup.*/
    if(UIM_ERR_S == uim_ptr->state.status)
    {
      uim_sam_reset_card_mode(i_mask,uim_ptr);
    }

    /* Clear the global command pointer as we are done with this command. */
    uim_ptr->command.cmd_ptr = NULL;
  }
  /* if the result was not something that we expected */
  else if (status == UIM_CMD_ERR)
  {
    uim_process_cmd_err(i_mask, uim_ptr);
  }  
}/*uim_sam_process_card_response*/

/*===========================================================================

FUNCTION UIM_SAM_PROCESS_COMMAND

DESCRIPTION
  This procedure processes a command sent to the UIM task. It sets the UIM
  state variable and calls the SAM card state machine to process the command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void uim_sam_process_command
(
  /* Pointer to received command */
  rex_sigs_type            *mask_ptr,
  /* rex signals type mask */
  uim_instance_global_type *uim_ptr
)
{
  /* Flag to indicate if the command processing modules have initiated any
     transactions with the UIM */
  boolean  status = TRUE;

  UIM_RETURN_IF_NULL_PTR(uim_ptr);

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);
  
  UIMDRV_MSG_MED_2(uim_ptr->id, "Logical_id-0x%x, Card_mode-0x%x", uim_ptr->logical_id, uim_ptr->card_mode);
  
  if (uim_ptr->command.cmd_ptr == NULL)
  {
    return;
  }

  /*A new command is being attempted, clear the response buffer*/
  (void)memset((void *)&uim_ptr->command.rsp_buf,
               (int)0x00,
               (size_t)sizeof(uim_ptr->command.rsp_buf));

  uim_ptr->command.rpt_buf.slot = uim_ptr->command.cmd_ptr->hdr.slot;

  /* Set the command mode to Generic Command mode as default */
  uim_ptr->state.cmd_mode = UIM_GENERIC_CMD;

  /* Set the command in progress FLAG to TRUE */
  uim_ptr->flag.command_in_progress = TRUE;

#ifdef FEATURE_UIM_T_1_SUPPORT
  /*Reset the T_1 block received since a new commadd is being processed */ 
  uim_ptr->t1.t_1_block_received = FALSE;
#endif

  /*For SAM card no requeue will happen*/
  uim_ptr->flag.command_requeued = FALSE;
  uim_ptr->command.command_requeue_counter = 0;

  /* Let the client know the command is being processed if the client
     provides it's tcb pointer. */
  uim_ptr->command.cmd_ptr->hdr.status = UIM_RECEIVED_S;

  if (uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr != NULL)
  {
    (void) rex_set_sigs( uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr,
                           uim_ptr->command.cmd_ptr->hdr.cmd_hdr.sigs );
  }

  /* Get the mode of the command */
  uim_ptr->state.cmd_mode =
  (uim_command_mode_type) UIM_GET_CMD_MODE((int)uim_ptr->command.cmd_ptr->hdr.command);


  /* Copy user data from received command to report buffer */
  uim_ptr->command.rpt_buf.user_data = uim_ptr->command.cmd_ptr->hdr.user_data;

  /*For all power-up related commands retry attempts are not reduced.
    For other commands retry attempts are reduced by 1
   */
  if(uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_ME_PUP_F ||
     uim_ptr->command.cmd_ptr->hdr.command == UIM_HOTSWAP_CARD_INS_F ||
     uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F ||
     uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_SWITCH_UIM_PASSIVE_F)
  {
    uim_ptr->setting.max_num_cmd_attempts = UIM_MAX_NUM_CMD_ATTEMPTS(uim_ptr);
  }
  else
  {
    uim_ptr->setting.max_num_cmd_attempts = UIM_MAX_NUM_CMD_ATTEMPTS(uim_ptr) - 1;
  }

  /* Process the generic command. */
  switch (uim_ptr->command.cmd_ptr->hdr.command)
  {
    case UIM_INTERNAL_ME_PUP_F:         /* POWER UP UIM due to task start up */
    {
      UIMDRV_MSG_MED_1(uim_ptr->id,  "SAM_MODE: Received internal Powerup command 0x%x ",
                          uim_ptr->command.cmd_ptr->hdr.command );

        uim_ptr->command.generic_state_ptr = UIM_SAM_INTERNAL_PUP_STATES;
    } /* end case - UIM_INTERNAL_PUP_F */
    break;


    case UIM_HOTSWAP_CARD_INS_F:
    case UIM_RESET_F:                                /*Client intialted Reset or Card insertion powerup*/
    case UIM_RESET_SWITCH_UIM_PASSIVE_F:
    case UIM_POWER_UP_UIM_PASSIVE_F:
      {
        UIMDRV_MSG_HIGH_1(uim_ptr->id, "SAM_MODE: Received Reset command for UIM_HOTSWAP_CARD_INS_F/RESET_F/PASSIVE_F - 0x%x", 
		   	                            uim_ptr->command.cmd_ptr->hdr.command);
        /* The status of UIM is set to Uninitialized since reset the UIM */
        uim_ptr->state.status = UIM_UNINITIALIZED_S;

        /*as a caution clear the poll timer if running*/        
        uim_clear_poll_timer(uim_ptr);

        uim_ptr->command.generic_state_ptr = UIM_SAM_INTERNAL_PUP_STATES;

        if(UIM_RESET_SWITCH_UIM_PASSIVE_F != uim_ptr->command.cmd_ptr->hdr.command &&
           UIM_POWER_UP_UIM_PASSIVE_F != uim_ptr->command.cmd_ptr->hdr.command )
        {
          /* Consider an external reset as a initial power-up */
          uim_ptr->flag.me_powerup = TRUE;
        }

      } /* end case - UIM_RESET_F */
      break;

    case UIM_HOTSWAP_CARD_REM_F:
    case UIM_POWER_DOWN_F:
      {
        UIMDRV_MSG_HIGH_0(uim_ptr->id,  "SAM_MODE: Received power down command" );

        if (uim_ptr->command.cmd_ptr->hdr.command == UIM_POWER_DOWN_F &&
            uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
        {
          /* Disable the hot swap interrupt*/
          uim_hotswap_mask_interrupt(uim_ptr);
          uim_ptr->hotswap.hotswap_me_power_down = TRUE;
          UIMDRV_MSG_HIGH_0(uim_ptr->id,"Successfully mask hot swap interrupt for power down command");
        }

        uim_ptr->mode.generic_states[0] = UIM_POWER_DOWN_ST;
        uim_ptr->mode.generic_states[1] = UIM_DONE_ST;
        uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;
      }
    break;


    case UIM_STREAM_ISO7816_APDU_F:     /* Stream an APDU - extended version */
    {
      uim_ptr->card_cmd.stream_apdu_rpt_data_length  = 0xFFFF;

      if(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes < UIM_ATCSIM_APDU_MIN_SIZE)
      {
        /* There are not enough bytes in the command */
        UIMDRV_MSG_HIGH_1(uim_ptr->id, "Insufficient number of bytes in the command 0x%x",
                          uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes);
        status = FALSE;
        break;
      }

      if(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data == NULL)
      {
        UIMDRV_MSG_HIGH_0(uim_ptr->id, "Unable to proceed as data is NULL");
        status = FALSE;
        break;
      }

      UIMDRV_MSG_MED_7(uim_ptr->id, "ISO7816_stream_APDU params class,ins,p1 p2, p3, d1, d2 %x %x %x %x %x %x %x",
                       uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_CLASS_OFFSET],
                       uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_INSTRN_OFFSET],
                       uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P1_OFFSET],
                       uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P2_OFFSET],
                       uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE3_4_7816_LC_OFFSET],
                       uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_APDU_SIZE],
                       uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_APDU_SIZE+1]);

      /* Build the Stream 7816 APDU list of states. */
      uim_ptr->mode.generic_states[0]    = UIM_STREAM_APDU_ST;
      uim_ptr->mode.generic_states[1]    = UIM_DONE_ST;
      uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;
	  
      /* clear APDU stream data */
      uim_ptr->card_cmd.is_ext_cmd_transaction_done = FALSE;
      uim_ptr->card_cmd.ext_cmd_data_offset = 0;	  
    }
    break;

    default:
      UIMDRV_MSG_ERR_1(uim_ptr->id, "***Unknown UIM Generic command: %d*** in SAM phase",
                       uim_ptr->command.cmd_ptr->hdr.command);
	  status = FALSE;
      break;

  }
  if(status)
  {
    /* Call the state machine. */
    uim_sam_command (uim_ptr);
  }
  else
  {
    /* Send an error report to the client and release the buffer */
    uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) SW1_WRONG_PARAMS;
    uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) SW2_BAD_PARAMS_P1_P2;
  
    uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
	
    if (mask_ptr != NULL)
    {
      /* include the command Q sig to the mask */
      *mask_ptr |= UIM_CMD_Q_SIG;
    }
    else
    {
      return;
    }

    /* Clear the flag */
    uim_ptr->flag.command_in_progress = FALSE;
  }

  /* Set the command transacted field in the report buffer */
  uim_ptr->command.rpt_buf.cmd_transacted = status;
}/*uim_sam_process_command*/


/*===========================================================================

FUNCTION UIM_SAM_PROCESS_CMD_ERR                                       

DESCRIPTION
  This procedure processes the error response received from the SAM card

DEPENDENCIES
  This function uses global variables defined for the UIM server to determine
  the proper processing of the card response.

RETURN VALUE
  None.

SIDE EFFECTS
  The control variables of the UIM server are adjusted to reflect the change
  in the operation of the UIM server as a result of the processing on the
  response.

===========================================================================*/
void uim_sam_process_cmd_err
(
  rex_sigs_type            *i_mask,   /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr
)
{
  uim_status_type  notification_reason = UIM_ERR_S;
  
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"uim_sam_process_cmd_err");
  if (i_mask == NULL)
  {
    return;
  }

  /*For an external command (stream APDU) otehr than RESET_F there is no command retry*/
  if(!uim_ptr->flag.static_buffer_used && 
      (UIM_RESET_F != uim_ptr->command.cmd_ptr->hdr.command) && 
      ( UIM_RESET_SWITCH_UIM_PASSIVE_F != uim_ptr->command.cmd_ptr->hdr.command) &&
      ( UIM_POWER_UP_UIM_PASSIVE_F != uim_ptr->command.cmd_ptr->hdr.command))
  {
    uim_ptr->command.cmd_ptr->hdr.cmd_count = uim_ptr->setting.max_num_cmd_attempts;

    /* include the command Q sig to the mask as we are going
     to process the command again */
    *i_mask |= UIM_CMD_Q_SIG;

    /* send an error report */
    uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);

    /* Clearing the bad status word flag */
    uim_ptr->flag.bad_status_words_error = FALSE;
        
    /* reset the static buffer used flag */
    uim_ptr->flag.static_buffer_used = FALSE;
    return;
  }

  /*Power down the sim on RESET command failure. so set it to max attempts*/
  if ((uim_ptr->command.cmd_ptr != NULL) && 
      (UIM_RESET_F == uim_ptr->command.cmd_ptr->hdr.command) && 
      (UIM_RESET_SWITCH_UIM_PASSIVE_F == uim_ptr->command.cmd_ptr->hdr.command) &&
      (UIM_POWER_UP_UIM_PASSIVE_F == uim_ptr->command.cmd_ptr->hdr.command))
  {
    uim_ptr->command.static_cmd_buf.hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
    uim_ptr->command.static_cmd_buf.hdr.slot = uim_ptr->command.cmd_ptr->hdr.slot;
    uim_ptr->command.static_cmd_buf.hdr.command = uim_ptr->command.cmd_ptr->hdr.command;

    uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
  }


  /* Check if the ATR was processed enough */
  if ((!uim_ptr->atr.atr_received) && (uim_ptr->command.static_cmd_buf.hdr.cmd_count == UIM_MAX_NUM_ATTEMPTS))
  {
    notification_reason = UIM_CORRUPT_ATR_RCVD_MAX_TIMES_S;
  }
  
  if (++uim_ptr->command.static_cmd_buf.hdr.cmd_count > UIM_MAX_NUM_ATTEMPTS)
  {
    /* Setting the status to ERROR prior to invoking power down.
       While shutting the LDO down we evaluate the uim status to
       cater to the user customized LDO settings if needed. */
    uim_ptr->state.status = UIM_ERR_S;

    uim_power_down(uim_ptr);

    /* Clear out any votes to keep UIM powered on */
    uim_ptr->state.ctl = UIM_NONE;
    uim_ptr->flag.powerdown = TRUE;
    uim_ptr->flag.command_in_progress = FALSE;

    /* flush all the commands in the command queue */
    uim_flush_command(UIM_FLUSH_ALL, uim_ptr);

    /* Whenever we receive a command, send an error report as the
    SIM is to be powered down */
    *i_mask |= UIM_CMD_Q_SIG;

    /* Notify the higher layers that UIM is declared faulty */
    uim_notify_error(notification_reason, uim_ptr);

    /*Switch mode might have happended and the slots might have switched to SAM mode.
     Reset the card mode for telecom slots. 
     So that for the new session it starts powerup again from telecom mode*/
    uim_sam_reset_card_mode(i_mask,uim_ptr);

    /* Reset the static buffer used flag */
    uim_ptr->flag.static_buffer_used = FALSE;

    /* Reset the static command count */
    uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;
  }
  else
  {
    uim_reset_uim (i_mask, uim_ptr->flag.me_powerup, uim_ptr);
  }
}/*uim_sam_process_cmd_err*/


/*===========================================================================

FUNCTION UIM_SAM_PROCESS_RESPONSE

DESCRIPTION
  This procedure processes the response that has been recd from the SAM card
  at the completion of the command processing and releases the command
  buffer for the current command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Releases the command buffer.

===========================================================================*/
void uim_sam_process_response
(
  uim_rsp_buf_type const   *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
 if (uim_ptr->command.cmd_ptr == NULL || rsp_ptr == NULL)
  {
    return;
  }

  /* We will reset the report options flag in the command to NONE for
     backward compatibility purposes */

  uim_ptr->command.cmd_ptr->hdr.options = UIM_OPTION_NONE;

  /* Set the status to UIM_PASS unless something failed */
  uim_ptr->command.rpt_buf.rpt_status = UIM_PASS;

  UIMDRV_MSG_HIGH_1(uim_ptr->id, "Reporting UIM generic command 0x%x",
                    uim_ptr->command.cmd_ptr->hdr.command );

  switch (uim_ptr->command.cmd_ptr->hdr.command)
  {
    case UIM_INTERNAL_ME_PUP_F:         /* POWER UP UIM due to task start up */
      break;

    case UIM_HOTSWAP_CARD_INS_F:
    case UIM_RESET_F:
    case UIM_RESET_SWITCH_UIM_PASSIVE_F:
    case UIM_POWER_UP_UIM_PASSIVE_F:
      {
        /* Report back the status as success */
        if (uim_ptr->command.cmd_ptr->hdr.rpt_function != NULL)
        {
          if(UIM_RESET_F == uim_ptr->command.cmd_ptr->hdr.command)
          {
            uim_ptr->command.rpt_buf.rpt_type = UIM_RESET_R;
          }
          else if(UIM_RESET_SWITCH_UIM_PASSIVE_F == uim_ptr->command.cmd_ptr->hdr.command)
          {
            uim_ptr->command.rpt_buf.rpt_type = UIM_RESET_SWITCH_UIM_PASSIVE_R;
          }
          else
          {
            uim_ptr->command.rpt_buf.rpt_type = UIM_POWER_UP_UIM_PASSIVE_R;
          }

          /* Fill in necessary fields of command block */
          uim_ptr->command.rpt_buf.rpt_hdr.task_ptr = NULL;

          /* Report to requesting task */
          (*(uim_ptr->command.cmd_ptr->hdr.rpt_function))( &uim_ptr->command.rpt_buf );

           /*set the logical slot power down flag*/
           uim_set_logical_slot_power_down_flag(uim_ptr, FALSE);

         } /* end if - is the report function defined. */

      } /* end case - UIM_RESET_F */
      break;

  case UIM_HOTSWAP_CARD_REM_F:
  case UIM_POWER_DOWN_F:
    {
      uim_status_type  uim_error_notificatoin;
      uim_cmd_name_type current_uim_command = uim_ptr->command.cmd_ptr->hdr.command;
      uim_pdown_option_type pdown_option = uim_ptr->command.cmd_ptr->pdown.pdown_option;
      uim_ptr->command.rpt_buf.rpt_type = UIM_POWER_DOWN_R;
      uim_ptr->command.rpt_buf.rpt_hdr.task_ptr = NULL;

      if (uim_ptr->command.cmd_ptr->hdr.rpt_function != NULL)
      {
        /* Report to requesting task */
        (*(uim_ptr->command.cmd_ptr->hdr.rpt_function))( &uim_ptr->command.rpt_buf );
      }
      /* Flush all the existing commands to empty the queue
      */
      if (current_uim_command == UIM_HOTSWAP_CARD_REM_F)
      {
        uim_flush_command(UIM_FLUSH_ALL,uim_ptr);
        uim_error_notificatoin= UIM_CARD_REMOVED_S;
      }
      else
      {
        uim_flush_command(UIM_FLUSH_ALL, uim_ptr);
        uim_error_notificatoin = UIM_POWER_DOWN_CMD_NOTIFICATION_S;
      }

      /* Notify GSDI about the card removal if the optionm is set */
      if (pdown_option == UIM_OPTION_NOTIFY_PDOWN)
      {
        if(uim_ptr->id < UIM_MAX_INSTANCES)
        {
          uim_notify_error(uim_error_notificatoin, uim_ptr);
        }
      }
      /* set the status to ERROR */
      uim_ptr->state.status = UIM_ERR_S;
    }
    break;

    case UIM_STREAM_ISO7816_APDU_F:            /* Stream APDU to the card */
    {
      uim_process_stream_iso7816_apdu_f_response(rsp_ptr, uim_ptr);
    }
    break;

    default:
    {
       UIMDRV_MSG_ERR_1(uim_ptr->id, "***Unknown UIM Generic command: %d***",
                        uim_ptr->command.cmd_ptr->hdr.command);
    } /* end case - default */
    break;

  }

  if (uim_ptr->command.cmd_ptr->hdr.rpt_function != NULL)
  {
    UIM_FREE(uim_ptr->command.cmd_ptr);
  }

}/* uim_sam_process_response */



/*===========================================================================

FUNCTION UIM_SAM_COMMAND                                      

DESCRIPTION
  This procedure processes the error response received from the SAM card

DEPENDENCIES
  This procedure is the main state machine for command processing of a SAM card.

RETURN VALUE
  None.

SIDE EFFECTS
  Sends a command to the SAM card.

===========================================================================*/

void uim_sam_command
(
  uim_instance_global_type *uim_ptr
)
{
  if (uim_ptr->command.cmd_ptr == NULL)
  {
    return;
  }

  /* Build an APDU based on the UIM generic state */
  if (*uim_ptr->command.generic_state_ptr < UIM_MAX_ST)
  {
    uimgen_hal_handle_generic_cmd_st(uim_ptr);
  }

  UIMDRV_MSG_LOW_1(uim_ptr->id, "UIM state in uim_command %ld",
                   *uim_ptr->command.generic_state_ptr);
}/* uim_sam_command */


/*===========================================================================

FUNCTION UIM_SAM_COMMAND_RESPONSE

DESCRIPTION
  This procedure processes the response to a comamnd that has been
  recd from the SAM card.

DEPENDENCIES
  None

RETURN VALUE
  Status which is of cmd_status_type

SIDE EFFECTS
  Changes the UIM state variable uim_sam_generic_state.

===========================================================================*/
uim_cmd_status_type uim_sam_command_response
(
  uim_rsp_buf_type    *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
  uim_cmd_status_type status = UIM_CMD_SUCCESS;

  /* Normally, the status when a command is completed is UIM_CMD_COMPLETED
  This is changed if there is a proactive command pending. */
  uim_cmd_status_type completed_status = UIM_CMD_COMPLETED;

  /* Flag to indicate if TD character was present */
  uim_generic_state_type curr_uim_sam_generic_state = UIM_DONE_ST;

  if(uim_ptr->command.cmd_ptr == NULL || rsp_ptr == NULL)
  {
    return UIM_CMD_ERR;
  }

  UIMDRV_MSG_HIGH_4(uim_ptr->id,"cmd status 0x%X SW1 0x%x,SW2 0x%x, Response data length 0x%x ",
                    rsp_ptr->cmd_status,
                    rsp_ptr->sw1,
                    rsp_ptr->sw2,
                    rsp_ptr->cmd_rsp_size);

  /* Fill the status words into the report buffer. This could be sent for
     both passed and failed reports. If the command is not sent to the
     card, then the status words are cleared before sending the report */
  uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) rsp_ptr->sw1;
  uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) rsp_ptr->sw2;

  
  /* Necessary so that we do not change the uim state and switch into that
     case also */
  curr_uim_sam_generic_state = *uim_ptr->command.generic_state_ptr;

  UIMDRV_MSG_HIGH_1(uim_ptr->id,"Processsing uim_sam_command_response for state 0x%x",
               curr_uim_sam_generic_state);

  if (curr_uim_sam_generic_state >= UIM_MAX_ST)
  {
    UIMDRV_MSG_ERR_1(uim_ptr->id, "***Unknown UIM Generic state: %d***",
                     curr_uim_sam_generic_state);
  }
  else
  {
    status = uimgen_hal_handle_generic_cmd_st_rsp(rsp_ptr, uim_ptr, &completed_status);
  }

  /* Continue processing the command only if the response indicates success. */
  if (status == UIM_CMD_SUCCESS)
  {
    /* Process the next state of this command. */
    uim_sam_command(uim_ptr);
  } /* end if - command is still in progress */

  /* Determine if the command has completed */
  if (UIM_DONE_ST == *uim_ptr->command.generic_state_ptr)
  {
    /* Indicate the command has completed successfully. */
    return (completed_status);
  } /* end if - uim_ptr->command.generic_state_ptr is UIM_DONE_ST */

  return(status);
}/* uim_sam_command_response */


/**
*  This function handles the UIM_TRANSACTION_SIG/CMD_RSP_TIMEOUT signal for SAM card.
*
*  SIDE EFFECTS
*    This function may change the value of *imask_ptr or *sig_mask_ptr.
*
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs to
*                       handle.  The value may be updated by this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_sam_handle_cmd_rsp_timeout_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  rex_sigs_type orig_sig_mask = *sig_mask_ptr;
  uim_status_type notification_reason = UIM_ERR_S;

  /* Turn on clock if it is stopped and there is a cmd in the Q */
  uim_checkq_and_turn_on_clk(uim_ptr);

  /* clear UIM_TRANSACTION_SIG from the current signal to ensure
     handle_cmd_rsp_timeout_sig() does not get called again */
  (*sig_mask_ptr)  &= ~UIM_TRANSACTION_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );

  if( orig_sig_mask & UIM_CMD_RSP_SIG )
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"WARNING: Command response and Command response timeout happened at the same time");

    /* Go back to the top of the main loop */
    return TRUE;
  }

  /* Check hotswap is enable for current slot */
  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    if(uim_ptr->hotswap.card_status == UIM_CARD_REMOVED)
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id, "HOTSWAP: card_status removed during sam_cmd_rsp_timeout");
      /*UIM will be powered down in card removed signal handling*/
      return TRUE;
    }
  }
  

  if(!(orig_sig_mask & UIM_TRANSACTION_SIG))
  {
    /* Reset and try to recover from these error cases */
    if ( uim_ptr->flag.max_parity_error   ||
         uim_ptr->flag.max_rx_break_error ||
         uim_ptr->flag.max_overrun_error ||
         uim_ptr->flag.bad_status_words_error)
    {
      UIMDRV_MSG_ERR_4(uim_ptr->id,"Reset due to Parity error-0x%x, rx break error-0x%x, overrun error-0x%x, bad status word-0x%x",
                         uim_ptr->flag.max_parity_error,
                         uim_ptr->flag.max_rx_break_error,
                         uim_ptr->flag.max_overrun_error,
                         uim_ptr->flag.bad_status_words_error);

      /* Clear all the flags irrespective of which one
       * got us here
       */
      uim_ptr->flag.max_parity_error   = FALSE;
      uim_ptr->flag.max_rx_break_error = FALSE;
      uim_ptr->flag.max_overrun_error  = FALSE;
      uim_ptr->flag.bad_status_words_error = FALSE;
      uim_ptr->flag.interface_gone_bad = TRUE;
    }
    else if ( uim_can_ignore_timeout(orig_sig_mask, uim_ptr) )
    {
      /* This is a spurious timeout signal.  We have bytes in the FIFO
         Return from this point as though nothing happened */
      UIMDRV_MSG_HIGH_0(uim_ptr->id,"Ignoring the spurious timeout signal for T=0");
      /* Go back to the top of the main loop */
      return TRUE;
    }
  }

  if (!uim_ptr->flag.static_buffer_used)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id, "UIM timeout in external command");
    if(uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F)
    {
       /*For Reset_F. report response and intiate an internal powerup*/
       uim_sam_process_response(&uim_ptr->command.rsp_buf, uim_ptr);
       UIMDRV_MSG_HIGH_0(uim_ptr->id,"Reset_F timeout reported to clients and try a fresh powerup");
       uim_ptr->flag.command_in_progress = FALSE;
       uim_ptr->flag.interface_gone_bad = FALSE;
       uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
       return FALSE;
    }
 
    /*For other commands(Stream APDU) set the notifcation reason to TIMEOUT_ERROR*/   
    notification_reason = UIM_CMD_TIMEOUT_ERR_S;
  }
  else
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id, "UIM timeout in internal powerup command");
    if((++uim_ptr->command.static_cmd_buf.hdr.cmd_count <= UIM_MAX_NUM_ATTEMPTS))
    {
      /*if it s RX_brea/Parity errors retry powerup*/
      if(uim_ptr->flag.interface_gone_bad == TRUE)
      {
        uim_ptr->flag.command_in_progress = FALSE;
        uim_ptr->flag.interface_gone_bad = FALSE;
        uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
        return FALSE;
      }
      /*if it is timeout and ATR is not recieved then check with next Voltage class*/
      else if(!uim_ptr->atr.atr_received)
      {
	if (uim_ptr->state.current_voltage_class < uim_ptr->state.max_possible_voltage_class)
        {
          uim_power_down(uim_ptr);

          UIMDRV_MSG_HIGH_0(uim_ptr->id, "Attempting power up at 3V");

          uim_timed_sleep(UIM_ISO7816_VOLTAGE_SWITCH_PWR_DOWN_DELAY,
                          uim_ptr,
                          UIM_ALL_ZERO_SIG);

          /* Try the next voltage class */
          uim_ptr->state.current_voltage_class++;

          /* Reset the error count */
          uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;
          uim_ptr->flag.command_in_progress = FALSE;
          uim_ptr->flag.interface_gone_bad = FALSE;
          uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
          return FALSE;
        }
        else
        {
          /*All voltages attpemted. card failed to reply with an ATR*/
          notification_reason = UIM_NO_ATR_RECEIVED_WITH_MAX_VOLTAGE_S;
        }
      }
    }
    /*ATR recieved card timedout at PPS or IFS state*/
    else
    {
      notification_reason = UIM_CMD_TIMEOUT_ERR_S;
    }
  }

   /*If transaction signal set. then it is timeout triggered due ot null bytes*/
  if((orig_sig_mask & UIM_TRANSACTION_SIG)!= 0)
  {
    notification_reason = UIM_NULL_BYTE_ERR_S;
  }
  /* Check UIM is in middle of processing of any command */
  if (TRUE == uim_ptr->flag.command_in_progress)
  {
    if (!uim_ptr->flag.static_buffer_used)
    {
      /* Set the status words to default before sending the rpt */
      uim_ptr->command.rpt_buf.cmd_transacted = FALSE;
      uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
      uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;
      /* Sending error report for the current command */
      uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
    }
  }
  /* Set the flag to false since the command is completed */
  uim_ptr->flag.command_in_progress = FALSE;

  UIMDRV_MSG_HIGH_0(uim_ptr->id, "Timeout in SAM mode. Power down the slot");

  /* Setting the status to ERROR prior to invoking power down.
     While shutting the LDO down we evaluate the uim status to
     cater to the user customized LDO settings if needed. */
  uim_ptr->state.status = UIM_ERR_S;

  /* Clear out any votes to keep UIM powered on */
  uim_ptr->state.ctl = UIM_NONE;

  uim_ptr->flag.powerdown = TRUE;

  /* flush all the commands in the command queue */
  uim_flush_command(UIM_FLUSH_ALL, uim_ptr);

  uim_power_down(uim_ptr);

  /* Notify the higher layers that UIM is declared faulty */
  uim_notify_error(notification_reason, uim_ptr);

  /*Switch mode might have happended and the slots might have switched to SAM mode.
   Reset the card mode for telecom slots. 
   So that for the new session it starts powerup again from telecom mode*/
  uim_sam_reset_card_mode(imask_ptr,uim_ptr);
  
  /* Whenver we receive a command, send an error report as the
  SIM is to be powered down */
  (*imask_ptr) |= UIM_CMD_Q_SIG;

  /* Reset the static buffer used flag */
  uim_ptr->flag.static_buffer_used = FALSE;

   /* Reset the static command count */
  uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;

  return FALSE;
}/*uim_sam_handle_cmd_rsp_timeout_sig*/


/**
*  This function handles the UIM_CMD_Q_SIG signal for SAM card.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_sam_handle_cmd_q_sig
(
  rex_sigs_type            *sig_mask_ptr,
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)sig_mask_ptr;
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
  if( q_cnt(&uim_ptr->task.cmd_q) == 0)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"q_cnt is 0 before q_get");
    return FALSE;
  }

  /*fetch the command from the Q*/
  if ((uim_ptr->command.cmd_ptr = (uim_cmd_type *)q_get( &uim_ptr->task.cmd_q )) == NULL)
  {
    return FALSE;
  }

  /* Defensive code to turn on clocks */
  if(uim_ptr->state.status == UIM_CLK_OFF_S)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"CLOCK OFF when processing command");
    uim_stop_power_save(uim_ptr);
    /* Set the state back to Initialized state */
    uim_ptr->state.status = UIM_INITIALIZED_S;
  }
  /* Command has not been attempted yet */
  uim_ptr->command.rpt_buf.cmd_transacted = FALSE;

  /*check if command is allowed or not*/
  if(((uim_ptr->state.status == UIM_ERR_S) &&
     (uim_ptr->command.cmd_ptr->hdr.command  != UIM_POWER_DOWN_F) &&
     (uim_ptr->command.cmd_ptr->hdr.command  != UIM_RESET_F) &&
     (uim_ptr->command.cmd_ptr->hdr.command  != UIM_RESET_SWITCH_UIM_PASSIVE_F) &&
     (uim_ptr->command.cmd_ptr->hdr.command  != UIM_POWER_UP_UIM_PASSIVE_F)) ||
	  (!uim_is_cmd_valid_in_sam_mode(uim_ptr)) ||
     (uim_is_reset_cmd_not_allowed(uim_ptr)))
  {
    /* Set the status words to default*/
    uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
    uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;

    if (uim_ptr->state.status == UIM_ERR_S)
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id, "SIM in power down state");
    }
    else
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id, "Cmd invalid in SAM phase");
    }
    uim_ptr->command.cmd_ptr->hdr.status = UIM_ERR_S;

    if (uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr != NULL)
    {
      (void) rex_set_sigs( uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr,
                             uim_ptr->command.cmd_ptr->hdr.cmd_hdr.sigs );
    }

    /* Get the mode of the command */
    uim_ptr->state.cmd_mode = (uim_command_mode_type)
				UIM_GET_CMD_MODE((int)uim_ptr->command.cmd_ptr->hdr.command);

    /* If always reporting & have a rpt function, report failure
    uim_send_err_rpt internally releases the buffer */
    if (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
    {
      uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
    }
  }
  else
  {
    /* We do not want to look into the command queue until we are
    done processing this command */

    (*imask_ptr) &= ~UIM_CMD_Q_SIG;
    uim_ptr->flag.static_buffer_used = FALSE;

    if (uim_ptr->command.cmd_ptr->hdr.cmd_count == 0)
    {
      /* Set Count to indicate that this is the first attempt.. once
       * set to 1, it would not be reset to 0
       */
      uim_ptr->command.cmd_ptr->hdr.cmd_count = 1;
      /* Intialising here. In uim_process_generic_cmd this takes proper value */
      uim_ptr->setting.max_num_cmd_attempts = UIM_MAX_NUM_CMD_ATTEMPTS(uim_ptr);

    }
    uim_ptr->flag.internal_command_used = FALSE;

    uim_ptr->card_cmd.work_waiting_time =
            uim_ptr->card_cmd.original_work_waiting_time;

    uim_ptr->card_cmd.work_waiting_time_etus =
            uim_ptr->card_cmd.original_work_waiting_time_etus;

    uim_process_command(imask_ptr, uim_ptr);
  }
  return FALSE;
}/*uim_sam_handle_cmd_q_sig*/


/**
*  This function checks if the command recieved is valid or not 
*  when the task is in SAM phase.
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure

*  @return boolean   TRUE: if command is valid in SAM phase
*                    FALSE: if command is invalid in SAM phase
*/
boolean uim_is_cmd_valid_in_sam_mode
(
  uim_instance_global_type *uim_ptr
)
{
  /*invalid slot check*/
  if(((int)(uim_ptr->command.cmd_ptr->hdr.slot) > UIM_NUM_DRV_SLOTS) ||
     ((int)(uim_ptr->command.cmd_ptr->hdr.slot) <= 0))

  {
    return FALSE;
  }

  /*only reset, poer_down and stream commands alloed in SAM phase*/
  if(uim_ptr->card_mode == UIM_CARD_SAM)
  {
    if((uim_ptr->command.cmd_ptr->hdr.command != UIM_RESET_F) &&
       (uim_ptr->command.cmd_ptr->hdr.command != UIM_POWER_DOWN_F) &&
       (uim_ptr->command.cmd_ptr->hdr.command != UIM_STREAM_ISO7816_APDU_F) &&
       (uim_ptr->command.cmd_ptr->hdr.command != UIM_RESET_SWITCH_UIM_PASSIVE_F) &&
       (uim_ptr->command.cmd_ptr->hdr.command != UIM_POWER_UP_UIM_PASSIVE_F))
    {
      UIMDRV_MSG_MED_1(uim_ptr->id,"uim_is_cmd_valid_in_sam_mode INVALID command reject 0x%x", uim_ptr->command.cmd_ptr->hdr.command);
      return FALSE;
    }
  }
  return TRUE;
}/*uim_is_cmd_valid_in_sam_mode*/

/**
*  This function modifies the imask to only check for signals 
*  that are applicabel to SAM card
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_sam_modify_imask(uim_instance_global_type *uim_ptr, rex_sigs_type *imask_ptr)
{
  UIMDRV_MSG_HIGH_0(uim_ptr->id, "uim_sam_modify_imask");

  if(TRUE == m_HWConfig.uim_slot_config[uim_ptr->id].disableUim)
  {
    /* If the UIM slot is disabled, we only care about these sigs */
    *imask_ptr = UIM_DOG_HB_RPT_SIG | UIM_TASK_STOP_SIG;
  }
  else
  {
   *imask_ptr &= ~UIM_POLL_TIMER_SIG |
                 ~UIM_SUSPICIOUS_CARD_REM_SIG|
                 ~UIM_EFSLOG_PURGE_SIG|
                 ~UIM_SIMULATE_NULL_TIMER_EXP_SIG|
                 ~UIM_EXT_RECOVERY_TIMER_EXP_SIG|
                 ~UIM_REMOTE_CONNECTION_UNAVAILABLE_SIG|
                 ~UIM_FETCH_PENDING_SIG;

  }
} /*uim_sam_modify_imask*/


/**
*  This function resets the imask to the default signals 
*  since we are moving fro mSAM phase ot telcom phase
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_telecom_modify_imask(uim_instance_global_type *uim_ptr, rex_sigs_type *imask_ptr)
{
  UIMDRV_MSG_HIGH_0(uim_ptr->id, "uim_telecom_modify_imask");

  if(TRUE == m_HWConfig.uim_slot_config[uim_ptr->id].disableUim)
  {
    /* If the UIM slot is disabled, we only care about these sigs */
    *imask_ptr = UIM_DOG_HB_RPT_SIG | UIM_TASK_STOP_SIG;
  }
  else
  {
   *imask_ptr |= UIM_POLL_TIMER_SIG |
                 UIM_SUSPICIOUS_CARD_REM_SIG|
                 UIM_EFSLOG_PURGE_SIG|
                 UIM_SIMULATE_NULL_TIMER_EXP_SIG|
                 UIM_EXT_RECOVERY_TIMER_EXP_SIG|
                 UIM_REMOTE_CONNECTION_UNAVAILABLE_SIG|
                 UIM_FETCH_PENDING_SIG;

  }
} /*uim_telecom_modify_imask*/


/**
*  This function handles the UIM_CMD_RSP_SIG signal for SAM card.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_sam_handle_cmd_rsp_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG );

  /* UIM BUSY RESPONSE FEATURE: Call mmgsdi API to clear uim busy */
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_CMD_RSP_SIG);
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_CMD_RSP_SIG);

  /* Check to see if we have already flushed out all the commands and
     are not in a state to process commands.
     If a command response is available when UIM is in an erroneous state
     allow the response to be processed only if it is a power down command */
  if (((uim_ptr->state.status == UIM_ERR_S)  &&
       ((uim_ptr->command.cmd_ptr != NULL) && (uim_ptr->command.cmd_ptr->hdr.command  != UIM_POWER_DOWN_F))) ||
      (!uim_ptr->flag.static_buffer_used && uim_ptr->command.cmd_ptr == NULL) )
  {
    UIMDRV_MSG_ERR_3(uim_ptr->id,
                      "CMD_RSP Sig Rcvd uim_status=0x%x, uim_st_bf_us=0x%x, cmd_ptr=0x%x",
                      uim_ptr->state.status,
                      uim_ptr->flag.static_buffer_used,
                      uim_ptr->command.cmd_ptr);
    /* we have received the response */
    uim_ptr->flag.command_in_progress = FALSE;
    if (uim_ptr->flag.powering_down_task)
    {
      /* If always reporting & have a rpt function, report failure
          uim_send_err_rpt internally releases the buffer */
      if ( (uim_ptr->command.cmd_ptr != NULL) && 
         (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0) )
      {
        uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
      }
      /* Need not set the cmd_q_sig in i_mask as task stop is called */
      (void) rex_set_sigs(uim_ptr->tcb_ptr, UIM_STATE_TOGGLE_SIG);
      return TRUE;
    }
    /* Go back to the beginning of the main task loop */
    (*imask_ptr) |= UIM_CMD_Q_SIG;
    return TRUE;
  }

  /* Check to see if we have a timeout signal and clear it since
  * we already got the response and processed it
  * unless the signal is explicitly set.
  */
  if ( uim_ptr->flag.clear_timeout_on_response )
  {
    (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
    (*sig_mask_ptr) &= ~UIM_CMD_RSP_TIMEOUT_SIG;
  }
  else
  {
    /* reset the flag for next use */
    uim_ptr->flag.clear_timeout_on_response = TRUE;
  }

  #if defined( FEATURE_UIM_T_1_SUPPORT )
  if (UIM_T_1_PROTOCOL == uim_ptr->state.current_protocol)
  {
    if(TRUE == uim_ptr->t1.t_1_block_received)
    {
      uim_ptr->t1.t_1_block_received = FALSE;
      /* Process the card response for the T=1 protocol */
      uim_process_t_1_block( &uim_ptr->command.rsp_buf, imask_ptr, uim_ptr);
    }
    else
    {
      uim_process_card_response(imask_ptr, uim_ptr);
    }

  }
  else
  #endif /* FEATURE_UIM_T_1_SUPPORT */
  {
    uim_process_card_response(imask_ptr, uim_ptr);
  }
  return FALSE;
}/*uim_sam_handle_cmd_rsp_sig*/


/**
*  The function is used to read the pass-through mode efs setting for the
*  logical slot
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*
*/
static uint8 uim_sam_passthrough_mode_efs_value
(
  uim_instance_global_type *uim_ptr
)
{
  uim_instance_global_type *uim_logical_ptr = NULL;

  if (uim_ptr == NULL ||
      (UIM_SLOT_NONE == uim_ptr->logical_id) ||
      (UIM_SLOT_AUTOMATIC <= uim_ptr->logical_id))
  {
    UIM_MSG_ERR_0("Invalid Slot Reference or instance global null");
    return 0;
  }

  /* get the instace id for this logical slot so that pass through mode setting can be extracted */
  uim_logical_ptr = uim_get_instance_ptr((uim_instance_enum_type)(uim_ptr->logical_id - 1));
  if(NULL == uim_logical_ptr)
  {
    UIM_MSG_ERR_1("Invalid Slot Reference 0x%x", uim_ptr->logical_id);
    return 0;
  }

  UIMDRV_MSG_HIGH_3(uim_ptr->id,"Physical instance id: 0x%x, Logical slot id: 0x%x, Logical pass through mode setting: 0x%x", 
    uim_ptr->id, uim_ptr->logical_id, uim_logical_ptr->setting.passthrough_mode);
  
  return (uim_logical_ptr->setting.passthrough_mode);
}/* uim_sam_passthrough_mode_efs_value */


/**
*  The function is used to set the card mdoe of the current task based on teh logical ID
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_sam_set_card_mode
(
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  uint8 passthrough_mode_setting = 0;

  passthrough_mode_setting = uim_sam_passthrough_mode_efs_value(uim_ptr);

  /* if pass through mode is set - power up card in sam card mode */
  if(1 == passthrough_mode_setting)
  {
    uim_ptr->card_mode = UIM_CARD_SAM;
    if (imask_ptr != NULL)
    {
      uim_sam_modify_imask(uim_ptr,imask_ptr);
    }
  }
  else
  {
    uim_ptr->card_mode = UIM_CARD_TELECOM;
    if (imask_ptr != NULL)
    {
      uim_telecom_modify_imask(uim_ptr,imask_ptr);
    }
  }

  UIMDRV_MSG_HIGH_1(uim_ptr->id,"Updated card mode:0x%x",uim_ptr->card_mode);
}/*uim_sam_set_card_mode*/


/**
*  The function is used to reset card mdoe of the logical ID 1 to telecom
* if dual sim enabled then logical ID 2 is also set to telecom
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_sam_reset_card_mode
(
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  /*reset card mode based on efs*/
  return uim_sam_set_card_mode (imask_ptr, uim_ptr);
}/*uim_sam_reset_card_mode*/


/**
*  The checks if SAM feature is enabeld or not
*  @return boolean 
           TRUE if Featre SAM card enabled
           FALSE if Featre SAM card disabled
*/
boolean uim_is_feature_sam_enabled(void)
{
#ifdef FEATURE_SAM_CARD
  return TRUE;
#else
  return FALSE;
#endif
}/*uim_is_feature_sam_enabled*/


/**
* Change the mode based on the command, NV setting and the current card mode
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @return void 
*/
void uim_sam_change_mode(
  uim_instance_global_type *uim_ptr
)
{
  uint8 passthrough_mode_setting = 0;

  passthrough_mode_setting = uim_sam_passthrough_mode_efs_value(uim_ptr);

  /* client now has means to indicate to trigger power up in sam card mode or normal mode
  
     If the RESET_F command is received, and NV supports dynamic pass through mode
     based on the current card mode and the reset_f power up option change the card mode */

  if (uim_ptr->command.cmd_ptr->hdr.command   == UIM_RESET_F &&
      passthrough_mode_setting != 1)
  {
    if (uim_ptr->command.cmd_ptr->reset.pup_option == UIM_PUP_OPTION_NON_TELECOM &&
        uim_ptr->card_mode == UIM_CARD_TELECOM)
    {
      uim_ptr->card_mode = UIM_CARD_SAM;
    }
    else if (uim_ptr->command.cmd_ptr->reset.pup_option == UIM_PUP_OPTION_TELECOM &&
             uim_ptr->card_mode == UIM_CARD_SAM)
    {
      uim_ptr->card_mode = UIM_CARD_TELECOM;
    }
  }

  UIMDRV_MSG_HIGH_1(uim_ptr->id,"Updated card mode:0x%x",uim_ptr->card_mode);
}/*uim_sam_change_mode*/
