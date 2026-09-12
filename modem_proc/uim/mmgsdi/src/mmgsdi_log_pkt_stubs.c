/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


     M M G S D I   B O O T U P  M E S S A G E   L O G G I N G   S T U B S


GENERAL DESCRIPTION

  This source file contains the stubs of functions/method for the bootup message logging.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_log_pkt_stubs.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/13/16   sp      initial version


===========================================================================*/


/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "mmgsdi.h"
#include "mmgsdi_common.h"

/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_INIT

   DESCRIPTION:
     This function initializes the timer and allocates memory to the log packet
     pointer.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_init
(
  void
)
{
}/* mmgsdi_log_pkt_init */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_NV_ITEMS

   DESCRIPTION:
     This is the logging function for the NV values,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_nv_items
(
  void
)
{
}/* mmgsdi_log_pkt_nv_items */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_RTRE_UPDATE

   DESCRIPTION:
     This is the logging function to update rtre NV value,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_rtre_update
(
  mmgsdi_attribute_enum_type  attr_type
)
{
  (void)attr_type;
}/* mmgsdi_log_pkt_rtre_update */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_LINK_ESTABLISHED

   DESCRIPTION:
     This is the logging function for the link established ,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_link_established
(
  const mmgsdi_notify_link_est_req_type* req_ptr
)
{
  (void)req_ptr;
}/* mmgsdi_log_pkt_link_established */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_MMGSDI_EVENT

   DESCRIPTION:
     This is the logging function for the mmgsdi events ,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_mmgsdi_event
(
  const mmgsdi_event_data_type * event_ptr
)
{
  (void)event_ptr;
}/* mmgsdi_log_pkt_mmgsdi_event */

