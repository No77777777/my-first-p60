 /******************************************************************************
 @file    ds_3gpp_profile_ex.h
 @brief

  DESCRIPTION
  Tech specific implementation of 3GPP Profile Management

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2019 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
****************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/25/19   bl      API to be used by extenal module
=============================================================================*/

#ifndef DS_3GPP_PROFILE_EX_H
#define DS_3GPP_PROFILE_EX_H

/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_DELETE_PROFILE_ALL

DESCRIPTION   This function clear all profiles per Sub

PARAMETERS    as_id : Subscription ID

DEPENDENCIES  None

RETURN VALUE  TRUE, if successful
              FALSE, if failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_profile_delete_profile_all
(
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_DELETE_PROFILE_ALL_EFS

DESCRIPTION
   This function clear all profiles per Sub no matter data task start or not.

PARAMETERS
  as id : subscription id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none
===========================================================================*/
void ds_3gpp_profile_delete_profile_all_efs
(
  sys_modem_as_id_e_type   as_id
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_CLEAR_MODEM_EMBEDDED_AND_TETHERED_PROFILES_EFS

DESCRIPTION
  This function clears the modem embedded and tethered prfoiles from EFS
  no matter data task start or not.

PARAMETERS
  as_id : Subscription ID

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_profile_clear_modem_embedded_and_tethered_profiles_efs
(
  sys_modem_as_id_e_type   as_id
);

/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_RESET_AP_DEFAULT_APN_EFS

DESCRIPTION   This function resets the AP default APN name no matter DS
              task start or not

PARAMETERS    subs_id: subscription id

DEPENDENCIES  None

RETURN VALUE  TRUE: Reset AP default APN successful
              FALSE: Reset AP default APN unsuccessful

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_profile_reset_ap_default_apn_efs
(
  ds_umts_pdp_subs_e_type  subs_id
);

#endif /*DS_3GPP_PROFILE_EX_H*/

