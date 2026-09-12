/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                        L P A   L I B R A R Y  S T U B S


GENERAL DESCRIPTION

  This file contains stubs for library function that can be used to externally
  access the LPA.

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_stubs.c#2 $ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
06/08/17   vdc     Return Modem LPA support as FALSE from LPA stub file 
04/12/17   av      Add support to configure/retrieve default server address
02/22/17   ar      Call LPA APIs only if LPA is modem centric
12/07/16   av      LPA phase 2 initial changes
04/11/16   av      Initial Version

=============================================================================*/

/*=============================================================================

                         INCLUDE FILES FOR MODULE

=============================================================================*/
#include "rcinit.h"
#include "lpa_platform.h"
#include "lpalib.h"

/*=============================================================================

                                  FUNCTIONS

=============================================================================*/

/*=============================================================================
FUNCTION        LPA_GET_PROFILES_INFO

DESCRIPTION
  This function allows the user to get complete information about all the
  profiles on the card.

PARAMETERS
  slot_id               :   Slot
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:             The command structure was properly generated and
                             queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                             appropriate ranges.
    LPA_GENERIC_ERROR:       The command was not queued to the LPA command
                             queue.
    LPA_ERROR_NOT_SUPPORTED: LPA is not supported.
===============================================================================*/
lpa_result_enum_type  lpa_get_profiles_info (
  lpa_slot_id_enum_type                 slot_id,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_get_profiles_info */


/*=============================================================================
FUNCTION        LPA_ENABLE_PROFILE

DESCRIPTION
  This function allows the user to enable a profile identified by the ICCID
  on the card.
PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the profile on the card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:             The command structure was properly generated and
                             queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                             appropriate ranges.
    LPA_GENERIC_ERROR:       The command was not queued to the LPA command
                             queue.
    LPA_ERROR_NOT_SUPPORTED: LPA is not supported.
===============================================================================*/
lpa_result_enum_type  lpa_enable_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) iccid;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_enable_profile */


/*=============================================================================
FUNCTION        LPA_DISABLE_PROFILE

DESCRIPTION
  This function allows the user to disable a profile identified by the ICCID
  on the card.
PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the profile on the card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:             The command structure was properly generated and
                             queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                             appropriate ranges.
    LPA_GENERIC_ERROR:       The command was not queued to the LPA command
                             queue.
    LPA_ERROR_NOT_SUPPORTED: LPA is not supported.
===============================================================================*/
lpa_result_enum_type  lpa_disable_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) iccid;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_disable_profile */


/*=============================================================================
FUNCTION        LPA_DELETE_PROFILE

DESCRIPTION
  This function allows the user to delete a profile identified by the ICCID
  on the card.

PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the profile on the card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:             The command structure was properly generated and
                             queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                             appropriate ranges.
    LPA_GENERIC_ERROR:       The command was not queued to the LPA command
                             queue.
    LPA_ERROR_NOT_SUPPORTED: LPA is not supported.
===============================================================================*/
lpa_result_enum_type  lpa_delete_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) iccid;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_delete_profile */


/*=============================================================================
FUNCTION        LPA_CARD_MEMORY_RESET

DESCRIPTION
  This function deletes from the card all or part of installed profiles based
  on the option field. If option field indicates that SMDP Address is also to
  be reset, card performs that operation as well.
  Note that if any profile is ENABLED when this command is received by the
  ISDR, the card issues a REFRESH after performing the memory reset for that
  profile.
PARAMETERS
  slot_id               :   Slot
  option                :   Option indicating what all is to be reset. This
                            field is ignored by LPA when the card is identified
                            to be a phase1/1.1 card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:             The command structure was properly generated and
                             queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                             appropriate ranges.
    LPA_GENERIC_ERROR:       The command was not queued to the LPA command
                             queue.
===============================================================================*/
lpa_result_enum_type  lpa_card_memory_reset (
  lpa_slot_id_enum_type                 slot_id,
  lpa_card_memory_reset_option_type     option,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_card_memory_reset */


/*=============================================================================
FUNCTION        LPA_GET_EID

DESCRIPTION
  This function allows the user to retrieve the EID of the card.

PARAMETERS
  slot_id               :   Slot
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:             The command structure was properly generated and
                             queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                             appropriate ranges.
    LPA_GENERIC_ERROR:       The command was not queued to the LPA command
                             queue.
    LPA_ERROR_NOT_SUPPORTED: LPA is not supported.
===============================================================================*/
lpa_result_enum_type  lpa_get_eid (
  lpa_slot_id_enum_type                 slot_id,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_get_eid */


/*=============================================================================
FUNCTION        LPA_ADD_PROFILE

DESCRIPTION
  This function allows the user to download a profile from the SMDP server
  to the card. If the activation code suggests the confirmation code is
  needed, and no confirmation code is passed in to this API, the API call
  returns error.

PARAMETERS
  slot_id                 :   Slot
  activation_code         :   Activation code that is sent to the server to
                              download a profile. Max length is 255 characters
                              per SGP.22.
  confirmation_code       :   Confirmation code, if mandated by the
                              activation_code.
  response_cb_ptr         :   Command response call back
  event_cb_ptr            :   Event callback called every n time, with the
                              profile download progress
  user_data_ptr           :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:                          The command structure was properly
                                          generated and queued onto the LPA
                                          command queue.
    LPA_ERROR_INCORRECT_PARAMS:                 The parameters supplied to the API
                                          are not within appropriate ranges.
    LPA_ERROR_CONFIRMATION_CODE_REQUIRED: Confirmation code is required but
                                          not provided by the caller.
    LPA_GENERIC_ERROR:                    The command was not queued to the LPA
                                          command queue.
    LPA_ERROR_NOT_SUPPORTED:              LPA is not supported.
===============================================================================*/
lpa_result_enum_type  lpa_add_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_data_type                         activation_code,
  lpa_data_type                         confirmation_code,
  lpa_callback_type                     response_cb_ptr,
  lpa_event_callback_type               event_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) activation_code;
  (void) confirmation_code;
  (void) response_cb_ptr;
  (void) event_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_add_profile */


/*=============================================================================
FUNCTION        LPA_ADD_OR_UPDATE_PROFILE_NICKNAME

DESCRIPTION
  This function allows the user to add or update the nickname of a profile.

PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the target profile
  nickname              :   Nickname of the target profile. Max length can be
                            64 characters per SGP.22
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:             The command structure was properly generated and
                             queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                             appropriate ranges.
    LPA_GENERIC_ERROR:       The command was not queued to the LPA command
                             queue.
    LPA_ERROR_NOT_SUPPORTED: LPA is not supported.
===============================================================================*/
lpa_result_enum_type  lpa_add_or_update_profile_nickname (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_name_type                         nickname,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) iccid;
  (void) nickname;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_add_or_update_profile_nickname */



/*=============================================================================
FUNCTION        LPA_SEND_USER_CONSENT

DESCRIPTION
  This function allows the user to send its consent to go ahead with a certain
  operation for which it has been asked asked by LPA (by sending an event/
  indication) to provide a consent.

PARAMETERS
  slot_id               :   Slot
  go_ahead              :   Yes or No
  msg_type              :   Message for which consent is sent by user
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_send_user_consent (
  lpa_slot_id_enum_type                 slot_id,
  boolean                               go_ahead,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) go_ahead;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_send_user_consent */


/*=============================================================================
FUNCTION        LPA_DEFAULT_SERVER_ADDRESS

DESCRIPTION
  This function allows the user to retrieve the default SM-DS address and
  default SM-DP+ (if any) configured on the eUICC.
  This function also allows the user to configure or remove the default default
  SM-DP+ address on the eUICC.

PARAMETERS
  slot_id               :   Slot
  set_address           :   If TRUE, it is a SET request. If FALSE, it is a
                            GET request.
  default_smdp_address  :   If it is a SET request, the default SM-DP+ address
                            to be set
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_default_server_address (
  lpa_slot_id_enum_type                 slot_id,
  boolean                               set_address,
  lpa_data_type                         default_smdp_address,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  (void) slot_id;
  (void) set_address;
  (void) default_smdp_address;
  (void) response_cb_ptr;
  (void) user_data_ptr;
  LPA_MSG_HIGH_0("LPA not supported!");

  return LPA_ERROR_NOT_SUPPORTED;
} /* lpa_default_server_address */


/*=============================================================================
FUNCTION        LPA_IS_LPA_SUPPORTED

DESCRIPTION
  Indicates if LPDd, LUId, LDSd, LUIe and modem LPA are supported or not.
  This info is needed to be sent to the card in the terminal capability.

PARAMETERS
  lpdd_supported_ptr       :   Boolean to indicate if lpdd is supported
  luid_supported_ptr       :   Boolean to indicate if luid is supported
  ldsd_supported_ptr       :   Boolean to indicate if ldsd is supported
  luie_supported_ptr       :   Boolean to indicate if luie is supported
  modem_lpa_supported_ptr  :   Boolean to indicate if modem LPA is supported

RETURN VALUE
  Void
===============================================================================*/
void lpa_is_lpa_supported(
  boolean                            *lpdd_supported_ptr,
  boolean                            *luid_supported_ptr,
  boolean                            *ldsd_supported_ptr,
  boolean                            *luie_supported_ptr,
  boolean                            *modem_lpa_supported_ptr
)
{
  if(lpdd_supported_ptr)
  {
    *lpdd_supported_ptr = FALSE;
  }

  if(luid_supported_ptr)
  {
    *luid_supported_ptr = FALSE;
  }

  if(ldsd_supported_ptr)
  {
    *ldsd_supported_ptr = FALSE;
  }

  if(luie_supported_ptr)
  {
    *luie_supported_ptr = FALSE;
  }

  if(modem_lpa_supported_ptr)
  {
    *modem_lpa_supported_ptr = FALSE;
  }
} /* lpa_is_lpa_supported */


/*===========================================================================
FUNCTION LPA_TASK_INIT

DESCRIPTION
  Function called to initialize the LPA task queue and globals

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None. Function should only be called once during task initialization.
  On targets with RCInit enabled, this function is called in RCInit context.
  Therefore, it should not include references to TCBs, NV
  operations, etc.
===========================================================================*/
void lpa_task_init (
  void
)
{
  return;
} /* lpa_task_init */


/*===========================================================================
FUNCTION LPA_MAIN

DESCRIPTION
  Entry point for the LPA task. This function performs task initialization,
  then sits in an infinite loop, waiting on an input queue, and responding
  to messages received.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_main (
  dword dummy
)
{
  (void) dummy;

  rcinit_handshake_startup();
  (void)rcevt_signal_name("LPA:READY");
} /* lpa_main */
