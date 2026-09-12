#ifndef GSTK_CSG_H
#define GSTK_CSG_H

/*===========================================================================


                         G S T K    C S G    H E A D E R

GENERAL DESCRIPTION
  This is the GSTK internal header file to be used by UIM modules.  It
  contains data types and functions that other UIM modules can use to
  access GSTK NV items.

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved. QUALCOMM Technologies Proprietary.
Export of this technology or software is regulated by the
U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/gstk/src/gstk_csg.h#1 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------------------
10/25/18   ks     Update gstk_check_csg_capability
10/24/16   gs     Changes in TP based on RAT and App Type
04/28/16   gm     Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*===========================================================================

                        FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION gstk_csg_process_cm_ph_event

DESCRIPTION
  This function process the cm_ph event

PARAMETERS
  local_info_ptr      [INPUT] Data pointer which has CSG information

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
void gstk_csg_process_cm_ph_event(gstk_local_info_rsp_type *local_info_ptr);

/*===========================================================================
FUNCTION gstk_csg_process_cm_csg_change_ind

DESCRIPTION
  This function process the CSG change indication from CM

PARAMETERS
  cmd_ptr:      [INPUT] Data pointer which csg cell information

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_status_enum_type gstk_csg_process_cm_csg_change_ind(gstk_cmd_type *cmd_ptr);

/*===========================================================================
FUNCTION gstk_csg_cm_ss_info_change

DESCRIPTION
  This function process the CSG change indication from CM

PARAMETERS
  ss_info_ptr:      [INPUT] Data pointer which csg which has CSG cell information

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
void gstk_csg_cm_ss_info_change(const cm_mm_msim_ss_info_s_type*  ss_info_ptr);

/*===========================================================================
FUNCTION gstk_check_csg_capability

DESCRIPTION
  This function is used to check whether CSG enabled or disabled

PARAMETERS
  slot_id:    [Input] Slot ID

DEPENDENCIES
  None

RETURN VALUE
  boolean

===========================================================================*/
boolean gstk_check_csg_capability(gstk_slot_id_enum_type  slot_id);

#endif /* GSTK_CSG_H */

