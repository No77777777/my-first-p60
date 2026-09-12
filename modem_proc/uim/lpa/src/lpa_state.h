#ifndef LPA_STATE_H
#define LPA_STATE_H
/*===========================================================================


            L P A   S T A T E   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_state.h#2 $ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/07/16   av      LPA phase2 initial changes
04/11/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "queue.h"

#include "lpa.h"
#include "lpa_cmd.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   ENUM:      LPA_STATE_ENUM_TYPE

   DESCRIPTION:
     This enum indicates the various states of the LPA module
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_STATE_IDLE,                               /* LPA is not processing any
                                                   command */
  LPA_STATE_INIT,                               /* LPA has dequeued a command
                                                   and has started processing
                                                   it */
  LPA_STATE_INITIATE_AUTH_SENT,                 /* LPA(LPD) has sent Initiate
                                                   Auth request to SMDP */
  LPA_STATE_INITIATE_AUTH_RESP_RCVD,            /* LPA(LPD) has received
                                                   response from SMDP for
                                                   InitiateAuth req */
  LPA_STATE_AUTHENTICATE_CLIENT_SENT,           /* LPA(LPD) has sent Authenticate
                                                   Client request to SMDP */
  LPA_STATE_AUTHENTICATE_CLIENT_RESP_RCVD,      /* LPA(LPD) has received
                                                   response from SMDP for
                                                   AuthenticateClient req */
  LPA_STATE_WAITING_FOR_USER_CONSENT,           /* LPA(LPD) has waiting for 
                                                   consent from user regarding
                                                   whether to go ahead with the
                                                   download or not */
  LPA_STATE_USER_CONSENT_RCVD,                  /* LPA(LPD) has received user 
                                                   consent regarding
                                                   whether to go ahead with the
                                                   download or not */
  LPA_STATE_CANCEL_SESSION_SERVER_SENT,         /* LPA(LPD) has sent 
                                                   CancelSession request to the
                                                   SMDP */
  LPA_STATE_CANCEL_SESSION_SERVER_RESP_RCVD,    /* LPA(LPD) has received 
                                                   CancelSession response from
                                                   SMDP */
  LPA_STATE_GET_BPP_SENT,                       /* LPA(LPD) has sent Get BPP
                                                   request to SMDP */
  LPA_STATE_GET_BPP_RESP_RCVD,                  /* LPA(LPD) has received
                                                   response from SMDP for Get
                                                   BPP req */
  LPA_STATE_HANDLE_INSTALLATION_RESULT_SENT,    /* LPA(LPD) has sent Handle
                                                   Inst Result req to SMDP */
  LPA_STATE_HANDLE_INSTALLATION_RESULT_RESP_RCVD/* LPA(LPD) has received
                                                   response from SMDP for
                                                   Handle Inst Result req */
} lpa_state_enum_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_STATE_CHANGE

DESCRIPTION
  Function moves the LPA state

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_state_change (
  lpa_state_enum_type      current_state,
  lpa_state_enum_type      next_state,
  lpa_slot_id_enum_type    slot_id
);

/*===========================================================================
FUNCTION LPA_STATE_GET_CURRENT

DESCRIPTION
  Function returns the current LPA state.

DEPENDENCIES
  None

RETURN VALUE
  lpa_state_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_state_enum_type lpa_state_get_current (
  lpa_slot_id_enum_type             slot_id
);

/*===========================================================================
FUNCTION LPA_STATE_RESET

DESCRIPTION
  Function called to reset the LPA state machine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void lpa_state_reset (
  lpa_slot_id_enum_type             slot_id
);

#endif /* LPA_STATE_H */

