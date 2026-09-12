/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            G S T K   C O N T A C T L E S S  S T A T E  C H A N G E D


GENERAL DESCRIPTION
  This source file contains functions to handle Contactless State Changed
  proactive command when not supported


FUNCTIONS
  gstk_contactless_state_changed_req
    This function parses the Contactless State Changed Proactive Command
    from the card and sends it to the client.

  gstk_contactless_state_changed_cnf
    This function packs the Contactless State Changed terminal response
    and send the command to UIM server.

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2003-2015 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/gstk/src/stub/gstk_contactless_state_changed_stub.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/10/15   vr      Initial Version

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "gstk.h"
#include "gstk_proactive_cmd.h"
#include "gstk_terminal_rsp.h"
#include "uim_msg.h"

#define GSTK_DISABLE_CLASS_R_COMMAND_BYTE_TWENTY_SIXTH 0xFD
#define GSTK_DISABLE_CLASS_R_COMMAND_BYTE_THIRTY_FIRST 0xFE

/*===========================================================================
FUNCTION gstk_contactless_state_changed_req

DESCRIPTION
  This function parses the Contactless State Changed Proactive Command
  from the card and sends it to the client.

  In case of any parsing errors or when GSTK can't send the command to the
  client, it will call the gstk_send_terminal_response function so that
  gstk can send the terminal response back to the card.

PARAMETERS
  command: [Input] Pointer to message parsed and sent to clients

DEPENDENCIES
  None

RETURN VALUE
  gstk_status_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_status_enum_type gstk_contactless_state_changed_req  (
  gstk_proactive_cmd_type* command_ptr)
{
  (void) command_ptr;

  return GSTK_UNSUPPORTED_COMMAND;
} /* gstk_contactless_state_changed_req*/

/*===========================================================================
FUNCTION gstk_contactless_state_changed_cnf

DESCRIPTION
  This function packs the Contactless State Changed terminal response
  and send the command to UIM server.

PARAMETERS
  contactless_state_changed_cnf_ptr: [Input] Pointer to message that is required to
                                      be processed and sent to UIM

DEPENDENCIES
  None

RETURN VALUE
  gstk_status_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_status_enum_type gstk_contactless_state_changed_cnf (
  gstk_contactless_state_changed_cnf_type *contactless_state_changed_cnf_ptr )
{
  (void) contactless_state_changed_cnf_ptr;

  return GSTK_UNSUPPORTED_COMMAND;
} /* gstk_contactless_state_changed_cnf */

/*===========================================================================
FUNCTION gstk_update_class_r_tp

DESCRIPTION
  This function will return false in stub code to disable TP and TRUE otherwise
  for class R TP bits

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  boolean

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
boolean gstk_update_class_r_tp ( void )
{
  return FALSE;
} /* gstk_update_class_r_tp */
