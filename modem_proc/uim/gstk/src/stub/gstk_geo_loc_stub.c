/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                     G S T K  G E O G R A P H I C A L  L O C A T I O N  R E Q U E S T


GENERAL DESCRIPTION
  This source file contains dummy functions to process geographical location
  proactive command


FUNCTIONS
  gstk_geo_loc_req
    Dummy function for parses the Geo Loc Command from the card
    and sends it to the client.

  gstk_geo_loc_cnf
    Dummy function for packs the Geo Loc terminal response and sends
    the command to UIM server.

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016-17 QUALCOMM Technologies, Inc (QTI) and its licensors.
All  Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/gstk/src/stub/gstk_geo_loc_stub.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/23/17   gs      Move Geo Loc Envelope to respective file
03/28/16   gm      Initial Version

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* GSTK Header Files   */
#include "gstk_proactive_cmd.h"
#include "uim_msg.h"

/*===========================================================================
FUNCTION gstk_geo_loc_req

DESCRIPTION
  Dummy function for parses the Geographical location request Command from the card
  and sends it to the client.

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
gstk_status_enum_type gstk_geo_loc_req (
  gstk_proactive_cmd_type *command_ptr
)
{
  (void) command_ptr;
  return GSTK_UNSUPPORTED_COMMAND;
}/* gstk_geo_loc_req */

/*===========================================================================
FUNCTION gstk_geo_loc_cnf

DESCRIPTION
  Dummy function for packs the geographical location request terminal response and sends
  the command to UIM server.

PARAMETERS
  geo_loc_cnf_ptr: [Input] Pointer to message that is required to
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
gstk_status_enum_type gstk_geo_loc_cnf (
  gstk_geo_loc_cnf_type* geo_loc_cnf_ptr
)
{
  (void) geo_loc_cnf_ptr;
  return GSTK_UNSUPPORTED_COMMAND;
}/* gstk_geo_loc_cnf */


/*===========================================================================
FUNCTION   gstk_geo_loc_report_ind

DESCRIPTION
  Dummy function of the envelope of Geographical location reporting

PARAMETERS
  geo_loc_report_cmd: [Input] The struct of Geographical location reporting

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
gstk_status_enum_type gstk_geo_loc_report_ind(
  gstk_envelope_geo_loc_report_type *geo_loc_report_cmd)
{
   (void) geo_loc_report_cmd;
   return GSTK_UNSUPPORTED_COMMAND;
} /* gstk_geo_loc_report_ind */

