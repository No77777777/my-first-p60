#ifndef MMGSDI_MULTI_PROFILE_H
#define MMGSDI_MULTI_PROFILE_H
/*===========================================================================


           M M G S D I   M U L T I   P R O F I L E   H E A D E R S


===========================================================================*/

/*===========================================================================
                      COPYRIGHT INFORMATION

Copyright (c) 2014 - 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.


                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_multi_profile.h#2 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
12/01/16   vdc     Added support for new version of multi profile card
05/03/16   tkl     F3 log prints cleanup
04/26/16   tkl     Reduce F3 log prints
04/24/16   vdc     Added support to detect multi profile card based on EFs
11/26/15   sp      Updated copyright
10/27/15   vdc     Send profile switching evt when profile has switched in card
01/07/14   vdc     Closed pending comments for handling multi profile cards
12/10/14   vdc     Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

#ifdef FEATURE_UIM_MULTI_PROFILE

/* Maximum number of APDUs supported for switching the profiles */
#define MMGSDI_MULTI_PROFILE_CARD_MAX_APDU_TRANSACTIONS           0x01

/* Byte number which indicates the current active profile in the card */
#define MMGSDI_MULTI_PROFILE_ACTIVE_PROFILE_BYTE                  0x03

/* SIN data length */
#define MMGSDI_MULTI_PROFILE_SIN_DATA_LENGTH                      0x04

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/


/* --------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_MUTLI_PROFILE_CARD_SYSTEM_DATA_TYPE

   DESCRIPTION:    This structure is used to hold multiple profile information,
                   APDUs, Switch back APDUs, corresponding SWs etc

   MEMBERS:
    num_profile:                 Number of profiles supported.
    profile_data:                Holds the profile info (profile id + profile type).
    num_apdu_switch:             Number of APDUs supported for switching to
                                 emergency profile.
    apdu_switch:                 Holds the APDU length & data for switching the
                                 profile to emergency.
    num_apdu_switch_back:        Number of APDUs supported for switching to 
                                 regular profile.
    apdu_switch_back:            Holds the APDU length & data for switch the
                                 profile to regular.
    sw_switch_diff_profile:      Holds the successful status word for switching
                                 to emergency profile.
    sw_switch_same_profile:      Holds the successful status word for switching
                                 to regular (same) profile.
    sw_switch_back_diff_profile: Holds the successful status word for switching
                                 to regular profile.
    sw_switch_back_same_profile: Holds the successful status word for switching
                                 to emergency (same) profile.
    switch_action:               Holds the action required for switching the
                                 profile.
    dedicated_channel_needed:    Indicates whether a dedicated channel is
                                 required for swithcing the profile
    aid:                         Applet identifier
    get_active_profile:          Indicates whether the active profile is
                                 determined
    get_active_profile_apdu:     Get active profile APDU
    check_multi_profile_support: Check whenter determination of multi profile
                                 card is required
    get_sin_apdu:                Get SIN APDU (applicable only when check_multi_profile_support
                                 is TRUE)
    sin_data:                    SIN data (applicable only when check_multi_profile_support
                                 is TRUE)

    dedicated_channel_id:        Indicates the channel id of dedicated channel
    active_profile:              Holds the current active profile information
    reg_profile_detected:        Determines whether the profile is detected at
                                 boot up
    is_ecall_active:             Determines whether any emergency call is active
    is_emergency_refresh:        Determines whether the REFRESH source is
                                 EMERGENCY
    ecasd_session_id:            Non-prov session ID for eCASD
---------------------------------------------------------------------------*/
typedef struct
{
  /* Config data */
  uint8                                       num_profile;
  mmgsdi_profile_info_type                    profile_data[MMGSDI_MAX_PROFILE];
  uint8                                       num_apdu_switch;
  mmgsdi_data_type                            apdu_switch[MMGSDI_MULTI_PROFILE_CARD_MAX_APDU_TRANSACTIONS];
  uint8                                       num_apdu_switch_back;
  mmgsdi_data_type                            apdu_switch_back[MMGSDI_MULTI_PROFILE_CARD_MAX_APDU_TRANSACTIONS];
  mmgsdi_sw_status_type                       sw_switch_diff_profile[MMGSDI_MULTI_PROFILE_CARD_MAX_APDU_TRANSACTIONS];
  mmgsdi_sw_status_type                       sw_switch_same_profile[MMGSDI_MULTI_PROFILE_CARD_MAX_APDU_TRANSACTIONS];
  mmgsdi_sw_status_type                       sw_switch_back_diff_profile[MMGSDI_MULTI_PROFILE_CARD_MAX_APDU_TRANSACTIONS];
  mmgsdi_sw_status_type                       sw_switch_back_same_profile[MMGSDI_MULTI_PROFILE_CARD_MAX_APDU_TRANSACTIONS];
  mmgsdi_profile_card_switch_action_enum_type switch_action;
  boolean                                     dedicated_channel_needed;
  mmgsdi_static_data_type                     aid;
  boolean                                     get_active_profile;
  mmgsdi_data_type                            get_active_profile_apdu;
  boolean                                     check_multi_profile_support;
  mmgsdi_data_type                            get_sin_apdu;
  uint8                                       sin_data[MMGSDI_MULTI_PROFILE_SIN_DATA_LENGTH];

  /* Derived data */
  uint8                                       dedicated_channel_id;
  mmgsdi_profile_info_type                    active_profile;
  boolean                                     reg_profile_detected;
  boolean                                     is_ecall_active;
  boolean                                     is_emergency_refresh;
  mmgsdi_session_id_type                      ecasd_session_id;
}mmgsdi_multi_profile_card_system_data_type;

extern mmgsdi_multi_profile_card_system_data_type *mmgsdi_profile_info_ptr;

/*=============================================================================

                                   MMGSDI MACROS
                        MACRO Definitions used through MMGSDI

=============================================================================*/


/*=============================================================================

                          FUNCTIONS

=============================================================================*/

/*===========================================================================
FUNCTION MMGSDI_MULTI_PROFILE_SET_SIM_PROFILE

DESCRIPTION
  This function is called in mmgsdi_process_command.
  It will switch the profile to the given profile id.

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  mmgsdi_return_enum_type
  MMGSDI_SUCCESS          - Command processed,
  MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
  MMGSDI_ERROR            - Other unknown failure

SIDE EFFECTS
  NONE
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_multi_profile_set_sim_profile (
  const mmgsdi_set_sim_profile_req_type *set_sim_profile_req_ptr,
  mmgsdi_protocol_enum_type              protocol
);

/*===========================================================================
FUNCTION MMGSDI_MULTI_PROFILE_GET_SIM_PROFILE

DESCRIPTION
  This function is called in mmgsdi_process_command.
  It will retrieve the profiles present in the EFS.

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  mmgsdi_return_enum_type
  MMGSDI_SUCCESS          - Command processed,
  MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
  MMGSDI_ERROR            - Other unknown failure

SIDE EFFECTS
  NONE
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_multi_profile_get_sim_profile (
  const mmgsdi_get_sim_profile_req_type *get_profile_req_ptr,
  mmgsdi_protocol_enum_type              protocol
);

/*===========================================================================
FUNCTION MMGSDI_CLEAR_MULTI_PROFILE_HEAP_ALLOC

DESCRIPTION
  This function deletes multi profile related data.

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_multi_profile_clear_heap_alloc(
  void
);

/*===========================================================================
FUNCTION MMGSDI_MULTI_PROFILE_INIT

DESCRIPTION
  Function to initialize the multi profile related globals. This function reads
  & parse the profile information from EFS.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void mmgsdi_multi_profile_init(
  void
);

/*===========================================================================
FUNCTION MMGSDI_MULTI_PROFILE_UPDATE_TO_SINGLE_PROFILE_INFO

DESCRIPTION
  Function to update number of profile supported to only 1.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/

void mmgsdi_multi_profile_update_to_single_profile_info(
  void
);

/*===========================================================================
FUNCTION MMGSDI_MULTI_PROFILE_RESET_DATA

DESCRIPTION
  Function to reset derived multi profile information.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/

void mmgsdi_multi_profile_reset_data(
  void
);

/*===========================================================================
FUNCTION MMGSDI_MULTI_PROFILE_SET_REGULAR_PROFILE

DESCRIPTION
  Determines the profile in the card and switches back to regular profile if required

DEPENDENCIES
  None

RETURN VALUE
  Boolean
    TRUE: Send card inserted event
    FALSE: Do not send card inserted event

SIDE EFFECTS
  None.
===========================================================================*/
boolean mmgsdi_multi_profile_set_regular_profile(
  mmgsdi_slot_id_enum_type  slot_id
);

#endif /* FEATURE_UIM_MULTI_PROFILE */
#endif /* MMGSDI_MULTI_PROFILE_H */
