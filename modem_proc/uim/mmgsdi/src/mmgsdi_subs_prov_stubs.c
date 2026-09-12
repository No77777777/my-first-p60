/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


             M M G S D I   S U B S   P R O V   S T U B   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains stubs for subscription provisioning functions.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2015 - 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_subs_prov_stubs.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/13/16   bcho    Reset subscription provisioning info on card error
02/12/16   kv      Fix compilation warnings - void unused variables
08/24/15   av      Rename it to mmgsdi_subs_prov_stubs.c to avoid comp error
08/20/15   ar      Initial version

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "mmgsdi_subs_prov.h"

/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/


/*===========================================================================
FUNCTION MMGSDI_UPDATE_EFS_SUBS_PROV

DESCRIPTION
  This function contains stub for mmgsdi_update_efs_subs_prov

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_update_efs_subs_prov(
  const mmgsdi_session_changed_evt_info_type *event_ptr,
  mmgsdi_session_id_type                      session_id
)
{
  (void)event_ptr;
  (void)session_id;
  return;
} /* mmgsdi_update_efs_subs_prov */


/*===========================================================================
FUNCTION MMGSDI_CLEAR_SUBS_PROV_APP_INFO

DESCRIPTION
  This function contains stub for mmgsdi_clear_subs_prov_app_info

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
============================================================================ */
void mmgsdi_clear_subs_prov_app_info(
  const mmgsdi_session_changed_evt_info_type *event_ptr
)
{
  (void)event_ptr;
  return;
} /* mmgsdi_clear_subs_prov_app_info */


/*===========================================================================
FUNCTION:  MMGSDI_PROVISION_APPLICATIONS_FROM_EFS

DESCRIPTION
  This function contains stub for mmgsdi_provision_applications_from_efs

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_provision_applications_from_efs(
  mmgsdi_slot_id_enum_type slot_id
)
{
  (void)slot_id;
  return;
} /* mmgsdi_provision_applications_from_efs */


/*===========================================================================
FUNCTION:  MMGSDI_DELETE_EFS_SUBS_PROV

DESCRIPTION
  This function contains stub for mmgsdi_delete_efs_subs_prov

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_delete_efs_subs_prov(
  void
)
{
  return;
}/* mmgsdi_delete_efs_subs_prov */


/*===========================================================================
FUNCTION MMGSDI_DELETE_EFS_SLOT_SUBS_PROV

DESCRIPTION
  This function will delete slot specific subscription provisioning data.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_delete_efs_slot_subs_prov(
  mmgsdi_slot_id_enum_type  slot_id
)
{
  (void)slot_id;
}/* mmgsdi_delete_efs_slot_subs_prov */
