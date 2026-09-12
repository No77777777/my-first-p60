#ifndef ESTK_BIP_3GPP2_H
#define ESTK_BIP_3GPP2_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              ESTK_BIP_3GPP2
                            H E A D E R   F I L E

GENERAL DESCRIPTION : This file contains the ESTK BIP APIs needed by the BIP connection on 3GPP2.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/estk/src/estk_bip_3gpp2.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/28/16   gm      Initial Version

===========================================================================*/


/*===========================================================================

                         I N C L U D E   F I L E S

===========================================================================*/
#include "estk_bip.h"
#include "ds_profile.h"

/*===========================================================================

FUNCTION: estk_bip_get_3gpp2_profile

DESCRIPTION :
  This function gets the 3GPP2 profile to use

PARAMETERS:
  ch_idx :         [Input]  Index of the active channel
  profile_num_ptr: [Output] The profile num created/selected

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS
    ESTK_ERROR
    ESTK_BAD_INPUT_PARAM

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_bip_get_3gpp2_profile(
  uint32                           ch_idx,
  estk_bip_apn_select_method_type  apn_select_method,
  ds_profile_num_type             *profile_num_ptr
);
#endif /* ESTK_BIP_3GPP2_H */
