#ifndef TDSRRCSEND_H
#define TDSRRCSEND_H
/*===========================================================================
              R R C S E N D  H E A D E R  F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  SEND CHAIN.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsend.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/
#include "comdef.h"
#include "queue.h"
#include "rex.h"
#include "tdsrrcdata.h"
#include "tdsuecomdef.h"

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/


/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

typedef enum
{
  TDSRRCSEND_SUCCESS,                          /* Successfultransmission */
  TDSRRCSEND_LOGICAL_CHANNEL_NOT_SETUP,        /* Logical Channel was not setup*/
  TDSRRCSEND_LOGICAL_CHANNEL_NOT_SUPPORTED,    /* Logical Channel is not supported */
  TDSRRCSEND_INCOMPATIBLE_L2ACK_REQUEST,       /* Indicates L2 ACK is requested for
                                               Transparent and Unacknowledged 
                                               mode transmission */
  TDSRRCSEND_INVALID_RB_ID,                    /* Identifies invalid RB Id. */
  TDSRRCSEND_FAILED_TO_ENCODE,                 /* ASN1 encoding failed */
  TDSRRCSEND_FAILED_CONVERT_SDU_TO_PACKECT,    /* Failed Convert SDU to DSM Item */
  TDSRRCSEND_FAILED_TO_ENQUEUE_SDU,            /* Failed to enqueue the DSM item
                                               in the watermark queue */
  TDSRRCSEND_INVALID_TM_RLC_SIZE_RECEIVED      /* Failed to get a valid RLC SIZE
                                               to enqueue  TM SDU */
}tdsrrcsend_status_e_type; 

typedef enum
{
  TDSRRCSEND_L2ACK_NOT_REQUIRED,               /* L2 Ack not required */
  TDSRRCSEND_L2ACK_REQUIRED                    /* L2 Ack required */
}tdsrrcsend_l2ack_status_e_type;
/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION tdsrrcsend_send_ul_sdu

DESCRIPTION
  This function will encode the uplink OTA messages using ASN1. Then converts
  the encoded message into DSM Item and places the DSM iterm in the watermark
  queue pointed by the Logical Channel ID.

DEPENDENCIES
  The l2ack_status  should not be set TDSRRCSEND_L2ACK_REQUIRED for Transparent
  and Unacknowleded mode transmission.

RETURN VALUE
  tdsrrcsend_status_e_type : If it is successful, then it returns TDSRRCSEND_SUCCESS. 
  Otherwise it returns one  of the causes for failure.


SIDE EFFECTS
  None
===========================================================================*/

tdsrrcsend_status_e_type tdsrrcsend_send_ul_sdu
(    
  tdsrrc_proc_e_type  rrc_procedure,          /* RRC procedure */
  void             *msg_ptr,               /* Pointer to the Uplink OTA 
                                              message */
  rlc_lc_id_type   lc_id,                  /* Logical Channel Id */
  tdsrrcsend_l2ack_status_e_type l2ack_status /* Indicates whether L2 Ack 
                                              required or not */
);
/*===========================================================================

FUNCTION tdsrrcsend_send_ul_sdu_with_mui

DESCRIPTION
  This function will encode the uplink OTA messages using ASN1. Then converts
  the encoded message into DSM Item and places the DSM iterm in the watermark
  queue pointed by the Logical Channel ID.

DEPENDENCIES
  The l2ack_status  should not be set TDSRRCSEND_L2ACK_REQUIRED for Transparent
  and Unacknowleded mode transmission.

RETURN VALUE
  tdsrrcsend_status_e_type : If it is successful, then it returns TDSRRCSEND_SUCCESS. 
  Otherwise it returns one  of the causes for failure.


SIDE EFFECTS
  None
===========================================================================*/

tdsrrcsend_status_e_type tdsrrcsend_send_ul_sdu_with_mui
(    
  tdsrrc_proc_e_type  rrc_procedure,          /* RRC procedure */
  void             *msg_ptr,               /* Pointer to the Uplink OTA 
                                              message */
  rlc_lc_id_type   lc_id,                  /* Logical Channel Id */
  tdsrrcsend_l2ack_status_e_type l2ack_status, 
                                           /* Indicates whether L2 Ack 
                                              required or not */
  uint8            *mui                    /* MUI tagged to message
                                              that is transmitted */

);

/*===========================================================================

FUNCTION RRCSEND_SEND_RRC_STATUS_MESSAGE

DESCRIPTION
  This function sends an RRC Status Message. This function is typically called
  when the ASN.1 decoder fails or when there is a semantic error in a message.

DEPENDENCIES
  An uplink DCCH must be set up.

RETURN VALUE
  SUCCESS if the message was successfully sent, otherwise FAILURE.

SIDE EFFECTS
  None.

===========================================================================*/

uecomdef_status_e_type tdsrrcsend_send_rrc_status_message
(
  uint32 protocol_error_choice,             // The protocol error cause to be placed in
                                            // the message
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,
                                           // Transaction Id received

  tdsrrc_ReceivedMessageType received_msg_id, 
                                           // Identification of the Recvd message.
  tdsrrc_proc_e_type rrc_procedure,           // RRC procedure
  rlc_lc_id_type   lc_id,                  /* Logical Channel Id */
  tdsrrcsend_l2ack_status_e_type l2ack_status /* Indicates whether L2 Ack 
                                              required or not */
);
/*==========================================================================*/

#endif /* TDSRRCSEND_H */

/*==========================================================================*/
