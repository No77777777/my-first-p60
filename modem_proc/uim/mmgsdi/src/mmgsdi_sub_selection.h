#ifndef MMGSDI_SUB_SELECTION_H
#define MMGSDI_SUB_SELECTION_H
/*===========================================================================


          M M G S D I   S U B S C R I P T I O N   S E L E C T I O N   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2013, 2015 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.  
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_sub_selection.h#2 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
08/20/15   ar      Removal of multisim_auto_provisioning by using stubs
05/14/15   yt      Support for mapping sessions to slots
03/25/15   kk      Subs prov EFS error handling
08/30/13   tl      Convert channel info table from static to dynamic allocation
08/20/13   spo     Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "mmgsdi.h"

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/


/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_PROCESS_MAP_SESSIONS_TO_SLOTS_REQ

DESCRIPTION
  This function processes the MMGSDI_MAP_SESSIONS_TO_SLOTS_REQ.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_sub_sel_process_map_sessions_to_slots_req(
  const mmgsdi_map_sessions_to_slots_req_type * sessions_to_slots_req_ptr
);

/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_GET_SESSION_ID_FOR_SLOT

DESCRIPTION
  This function finds the MMGSDI session ID for a slot based on the stored
  sessions-to-slots mapping.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_sub_sel_get_session_id_for_slot(
  mmgsdi_slot_id_enum_type      slot_id,
  mmgsdi_technology_enum_type   tech_type,
  mmgsdi_session_id_type      * session_id_ptr
);

#endif /* MMGSDI_SUB_SELECTION_H */
