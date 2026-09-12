/*========================================================================================

                                    R R C C H K

DESCRIPTION
  This file contains the definitions of the functions and data types used in RRCCHK procedure

  Copyright (c) 2010-2012 by Qualcomm Technologies Incorporated.  All Rights Reserved.
========================================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcchk.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
  
 when       who     what, where, why
--------    ---     ----------------------------------------------------------
03/10/11   yzh      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/
#include "tdsrrcasn1.h"
#include "tdsrrcscmgr.h"
#include "tdsrrccommon.h"
#include "tdsrrclcm.h" 
#include "tdsrrcsend.h"
#include "tdsrrcchk.h"
#include "tdsrrc_dependancy.h"


tdsrrc_RRC_TransactionIdentifier tdschk_transaction_id; /* Variable to hold the transaction id */

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/


/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/


/*===========================================================================
**                  TYPE DECLARATIONS
**=========================================================================*/


/*===========================================================================

                    FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION tdsrrcchk_process_counter_check_message

DESCRIPTION
  This function validates the Counter Check message

DEPENDENCIES
  None

RETURN VALUE
  TRUE - If the message is valid
  FALSE - If the message is invalid
  
SIDE EFFECTS
  None

===========================================================================*/
static boolean tdsrrcchk_process_counter_check_message
(
   tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;  /* Pointer to DCCH message */  
  uint8 message_choice;                    /* Local varibale for message choice */
  boolean ret_val = TRUE;                  /* Return Value */

  /* Check if the decoding was successful */
  if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
  {
    /* Check the message type */
    message_choice = tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    if(message_choice == T_tdsrrc_DL_DCCH_MessageType_counterCheck)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Counter Check Message is rcvd");

      /* Extract the transaction id from the message */
      dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;     
      tdschk_transaction_id = 
      dcch_msg_ptr->message.u.counterCheck.u.r3.counterCheck_r3.rrc_TransactionIdentifier;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid message %d sent to CHK procedure",message_choice);
      ret_val = FALSE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ASN1 Decoding error");
    ret_val = FALSE;
  }
  return ret_val;
}

/*===========================================================================

FUNCTION tdsrrcchk_send_counter_check_response

DESCRIPTION
  This function sends the Counter Check Response message to the network

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
static void tdsrrcchk_send_counter_check_response
(
  void
)
{
  rlc_lc_id_type lc_id;                    /* Local variable to store RLC logical channel Id */
  tdsrrc_UL_DCCH_Message *msg = NULL;         /* Pointer to UL message */

  lc_id = tdsrrclcm_check_ul_rlc_lc_id(
             UE_LOGCHAN_DCCH,              /* Uplink Logical Channel Type. */
             DCCH_AM_RADIO_BEARER_ID,      /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel type. */
             UE_MODE_ACKNOWLEDGED_DATA);   /* Mode of transmission for the RLC 
                                              service entity. */
  if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
  {
    ERR_FATAL("Can't find RLC ID for UL RB 2",0,0,0);
  }

  /* Allocate a buffer for an uplink DCCH message. This buffer is released
     by tdsrrcsend_send_ul_sdu().*/
  msg = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(sizeof (struct tdsrrc_UL_DCCH_Message));
  if(msg == NULL)
  {
    /* Error allocating memory for the uplink message. Print a message and 
       return failure */
    ERR_FATAL("Failed to allocate memory for Counter Check Procedure",0,0,0);
  }

  /* Set the message type to Counter Check Response */
  TDSRRCSET_MSG_TYPE(msg->message, 
            T_tdsrrc_UL_DCCH_MessageType_counterCheckResponse);

  /* Set the bit mask not to include non-critical extensions */
  TDSRRC_RESET_MSG_IE_PRESENT(msg->message.u.counterCheckResponse);

  /* Set the transaction id */
  msg->message.u.counterCheckResponse.rrc_TransactionIdentifier = tdschk_transaction_id;

  /* Send the signalling message */
  if(TDSRRCSEND_SUCCESS == tdsrrcsend_send_ul_sdu(
                                            TDSRRC_PROCEDURE_CHK,
                                            msg,
                                            lc_id,
                                            TDSRRCSEND_L2ACK_NOT_REQUIRED
                                           ) )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Counter Check Response is sent");
  } 
  else
  {
    ERR_FATAL("Failed to send Counter Check Response",0,0,0);
  }
}

/*===========================================================================

FUNCTION tdsrrcchk_procedure_event_handler

DESCRIPTION
  This function is the entry point of rrcchk

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcchk_procedure_event_handler
( 
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)  
{ 
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_SDU_IND:
      if (TRUE == tdsrrcchk_process_counter_check_message(cmd_ptr)) 
      {
        tdsrrcchk_send_counter_check_response();
      }
      break;

    case TDSRRC_STATE_CHANGE_IND:
      break;

    default:     
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Event: %d received",cmd_ptr->cmd_hdr.cmd_id);
      break;
  }
}

/*===========================================================================

FUNCTION RRCCHK_INIT_PROCEDURE

DESCRIPTION
  This function will register for the necessary state change indication for
  Counter Check procedure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcchk_init_procedure
(    
  void  
)
{
  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_CHK,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_DISCONNECTED);
}
