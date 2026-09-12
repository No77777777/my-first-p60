/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                      ESTK  BEARER INDEPENDENT PROTOCOL


GENERAL DESCRIPTION : Stub function for 3gpp2 ENHANCED STK layer BIP support

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016-17 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/estk/src/stub/estk_bip_3gpp2_stub.c#1 $

when       who     what, where, why
--------   ---     --------------------------------------------------------------------
06/01/17   gs      Don't depend on DS Sys Status for establishing BIP on non-DDS
03/28/16   gm      Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "estk_bip_3gpp2.h"
#include "uim_msg.h"

#ifdef FEATURE_ESTK

static estk_result_enum_type estk_set_net_policy_3gpp2_profile(
  uint32                       ch_idx,
  ds_profile_num_type         *profile_num_ptr
);

static estk_result_enum_type estk_get_3gpp2_profile_to_use(
  uint32                        ch_idx,
  ds_profile_tech_etype         tech_type,
  ds_profile_num_type          *profile_num_ptr,
  ds_profile_subs_etype         sub_id
);

/*===========================================================================

FUNCTION: estk_set_net_policy_3gpp2_profile

DESCRIPTION :
  This function creates profiles with APN and 3GPP2 bearer parameters

PARAMETERS:
  ch_idx :         [Input]  Index of the active channel
  profile_num_ptr: [Output] The profile num created

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS
    ESTK_ERROR

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static estk_result_enum_type estk_set_net_policy_3gpp2_profile(
  uint32                       ch_idx,
  ds_profile_num_type         *profile_num_ptr
)
{
  (void) ch_idx;
  (void) profile_num_ptr;
  return ESTK_UNSUPPORTED;
}

/*===========================================================================

FUNCTION: estk_get_3gpp2_profile_to_use

DESCRIPTION :
  This function iterates throught the list of profiles and tries to reuse a
  profile number if possible for 3gpp2

PARAMETERS:
  ch_idx :         [Input]  Index of the active channel
  tech_type:       [Input]  Tech type 3GPP2
  profile_num_ptr: [Output] The profile num to be reused
  sub_id :         [Input]  DS Sub Id

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS
    ESTK_ERROR

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static estk_result_enum_type estk_get_3gpp2_profile_to_use(
  uint32                        ch_idx,
  ds_profile_tech_etype         tech_type,
  ds_profile_num_type          *profile_num_ptr,
  ds_profile_subs_etype         sub_id
)
{
  (void) ch_idx;
  (void) tech_type;
  (void) profile_num_ptr;
  return ESTK_UNSUPPORTED;
}/* estk_get_3gpp2_profile_to_use */

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
)
{
  (void) ch_idx;
  (void) apn_select_method;
  (void) profile_num_ptr;
  return ESTK_UNSUPPORTED;
}/* estk_bip_get_3gpp2_profile */
#endif /* FEATURE_ESTK */

