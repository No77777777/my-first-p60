/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                        L P A  L I B R A R Y


GENERAL DESCRIPTION

  This file contains Library function that can be used to externally
  access the LPA.

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2017, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpalib.c#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
09/23/19   kb      Fixed String Conversion Issues
09/15/17   me      Fixing compiler errors   
04/12/17   av      Add support to configure/retrieve default server address
02/22/17   ar      Call LPA APIs only if LPA is modem centric
12/07/16   av      LPA phase 2 initial changes
10/10/16   av      Add support for configuring HTTP stack via LPA EFS item
04/14/16   av      Initial Version

=============================================================================*/

/*=============================================================================

                         INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "customer.h"
#include "uim_msg.h"
#include <stringl/stringl.h>
#include "uim_common_efs.h"

#include "lpa_cmd.h"
#include "lpa_lpd.h"
#include "lpalib.h"
#include "lpa.h"
#include "lpa_platform.h"
#include "lpa_util.h"

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
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_get_profiles_info (
  lpa_slot_id_enum_type                 slot_id,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  lpa_result_enum_type         lpa_status     = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr    = NULL;

  if(!lpa_util_is_slot_valid(slot_id) )
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                             = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                          = user_data_ptr;
  lpa_cmd_ptr->slot_id                                = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                           = LPA_GET_PROFILES_INFO_MSG;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA get profiles info: status 0x%x", lpa_status);

  return lpa_status;
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
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_enable_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  lpa_result_enum_type         lpa_status     = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr    = NULL;

  if(!lpa_util_is_slot_valid(slot_id) ||
     !lpa_util_is_iccid_valid(iccid))
  {
    LPA_MSG_ERR_2(
      "lpa_enable_profile: invalid parameter(s) - slot_id=0x%x, iccid len=0x%x",
      slot_id, iccid.data_len);
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  if (!lpa_util_is_slot_valid(slot_id) ||
      !lpa_util_is_iccid_valid(iccid))
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                             = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                          = user_data_ptr;
  lpa_cmd_ptr->slot_id                                = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                           = LPA_ENABLE_PROFILE_MSG;
  lpa_cmd_ptr->cmd.cmd.enable_profile_req.iccid       = iccid;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA enable profile: status 0x%x", lpa_status);

  return lpa_status;
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
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_disable_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  lpa_result_enum_type         lpa_status     = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr    = NULL;

  if(!lpa_util_is_slot_valid(slot_id) ||
     !lpa_util_is_iccid_valid(iccid))
  {
    LPA_MSG_ERR_2(
      "lpa_disable_profile: invalid parameter(s) - slot_id=0x%x, iccid len=0x%x",
      slot_id, iccid.data_len);
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                             = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                          = user_data_ptr;
  lpa_cmd_ptr->slot_id                                = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                           = LPA_DISABLE_PROFILE_MSG;
  lpa_cmd_ptr->cmd.cmd.disable_profile_req.iccid      = iccid;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA disable profile: status 0x%x", lpa_status);

  return lpa_status;
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
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_delete_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  lpa_result_enum_type         lpa_status     = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr    = NULL;

  if(!lpa_util_is_slot_valid(slot_id) ||
     !lpa_util_is_iccid_valid(iccid))
  {
    LPA_MSG_ERR_2(
      "lpa_delete_profile: invalid parameter(s) - slot_id=0x%x, iccid len=0x%x",
      slot_id, iccid.data_len);
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                             = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                          = user_data_ptr;
  lpa_cmd_ptr->slot_id                                = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                           = LPA_DELETE_PROFILE_MSG;
  lpa_cmd_ptr->cmd.cmd.delete_profile_req.iccid       = iccid;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA delete profile: status 0x%x", lpa_status);

  return lpa_status;
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
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_card_memory_reset (
  lpa_slot_id_enum_type                 slot_id,
  lpa_card_memory_reset_option_type     option,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  lpa_result_enum_type         lpa_status     = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr    = NULL;

  if(!lpa_util_is_slot_valid(slot_id) )
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                             = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                          = user_data_ptr;
  lpa_cmd_ptr->slot_id                                = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                           = LPA_CARD_MEMORY_RESET_MSG;
  lpa_cmd_ptr->cmd.cmd.card_memory_reset_req.option   = option;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA card memory reset: status 0x%x", lpa_status);

  return lpa_status;
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
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_get_eid (
  lpa_slot_id_enum_type                 slot_id,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  lpa_result_enum_type         lpa_status     = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr    = NULL;

  if(!lpa_util_is_slot_valid(slot_id))
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                           = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                        = user_data_ptr;
  lpa_cmd_ptr->slot_id                              = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                         = LPA_GET_EID_MSG;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA get EID: status 0x%x", lpa_status);

  return lpa_status;
} /* lpa_get_eid */


/*=============================================================================
FUNCTION        LPA_ADD_PROFILE

DESCRIPTION
  This function allows the user to download a profile from the SMDP server
  to the card. If the activation code suggests the confirmation code is
  needed, and no confirmation code is passed in to this API, the API call
  returns error. There are following ways/methods to trigger the download:
  1. By using activation code
  2. By asking ISDR to use SMDS discovery method
  3. By asking ISDR to use default SMDP+ address on the eUICC.
  If the method is not the first one above, the 'activation_code' string is
  used to indicate which of the two remaining methods are to be used.
  If activation code starts with 'UseSMDS', use method #2.
  If activation code starts with 'UseSMDP', use method #3.

PARAMETERS
  slot_id                 :   Slot
  activation_code         :   Activation code that is sent to the server to
                              download a profile. Max length is 255 characters
                              per SGP.22.
                              If the profile is to be downloaded using 'Default
                              SMDS' or 'Default SMDP+' methods, this string
                              is used to indicate that... in that case, this
                              is not actually an activation code but just a way
                              to indicate the 'profile download method'
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
  lpa_result_enum_type         lpa_status                 = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr                = NULL;

  if(!lpa_util_is_slot_valid(slot_id) ||
     activation_code.data_len == 0||
     activation_code.data_ptr == NULL)
  {
    LPA_MSG_ERR_2(
      "lpa_add_profile: invalid parameter(s) - slot_id=0x%x, activation code len=0x%x",
      slot_id, activation_code.data_len);
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  /* Copy over the activation code */
  lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_ptr =
    LPA_MALLOC(activation_code.data_len);
  if(lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_ptr == NULL)
  {
    LPA_FREE(lpa_cmd_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_len =
    activation_code.data_len;

  memscpy(lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_ptr,
          lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_len,
          activation_code.data_ptr,
          activation_code.data_len);

  /* Copy over the convirmation code, if any */
  if(confirmation_code.data_len > 0 &&
     confirmation_code.data_ptr)
  {
    /* It is possible that confirmation code is zero and the add profile
       request is still a valid one. */
    lpa_cmd_ptr->cmd.cmd.add_profile_req.confirmation_code.data_ptr =
    LPA_MALLOC(confirmation_code.data_len);

    if(lpa_cmd_ptr->cmd.cmd.add_profile_req.confirmation_code.data_ptr == NULL)
    {
      LPA_FREE(lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_ptr);
      LPA_FREE(lpa_cmd_ptr);
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    lpa_cmd_ptr->cmd.cmd.add_profile_req.confirmation_code.data_len =
      confirmation_code.data_len;
    memscpy(lpa_cmd_ptr->cmd.cmd.add_profile_req.confirmation_code.data_ptr,
          lpa_cmd_ptr->cmd.cmd.add_profile_req.confirmation_code.data_len,
          confirmation_code.data_ptr,
          confirmation_code.data_len);
  }
  /* Whether activation code is a valid activation code and whether the
     confirmation code is needed and provided with this API or not, all
     of that is determined in LPA task context. Error is returned via the
     client callback if there is any issue with activation code or the
     confirmation code */

  lpa_cmd_ptr->lpa_cb_ptr                           = response_cb_ptr;
  lpa_cmd_ptr->event_cb_ptr                         = event_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                        = user_data_ptr;
  lpa_cmd_ptr->slot_id                              = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                         = LPA_ADD_PROFILE_MSG;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_ptr);
    LPA_FREE(lpa_cmd_ptr->cmd.cmd.add_profile_req.confirmation_code.data_ptr);
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA add profile: status 0x%x", lpa_status);

  return lpa_status;
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
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_add_or_update_profile_nickname (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_name_type                         nickname,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
)
{
  lpa_result_enum_type         lpa_status                 = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr                = NULL;

  if(!lpa_util_is_slot_valid(slot_id) ||
     !lpa_util_is_iccid_valid(iccid))
  {
    LPA_MSG_ERR_2(
      "lpa_add_or_update_profile_nickname: invalid parameter(s) - slot_id=0x%x, iccid len=0x%x",
      slot_id, iccid.data_len);
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                                          = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                                       = user_data_ptr;
  lpa_cmd_ptr->slot_id                                             = slot_id;
  lpa_cmd_ptr->cmd.cmd_enum                                        = LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG;
  lpa_cmd_ptr->cmd.cmd.add_or_update_profile_nickname_req.iccid    = iccid;
  lpa_cmd_ptr->cmd.cmd.add_or_update_profile_nickname_req.nickname = nickname;

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_1("Queue of command: LPA add or update profile nickname: status 0x%x", lpa_status);

  return lpa_status;
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
  lpa_result_enum_type         lpa_status                 = LPA_GENERIC_ERROR;

  (void) user_data_ptr;

  if(!lpa_util_is_slot_valid(slot_id))
  {
    LPA_MSG_ERR_1("lpa_send_user_consent: invalid parameter - slot_id=0x%x", slot_id);
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  /* Directly queue this command to the LPD module */
  lpa_status = lpa_lpd_process_user_consent(go_ahead, slot_id);

  LPA_MSG_HIGH_2("Queue of command: LPA send user consent go_ahead 0x%x: status 0x%x",
                 go_ahead, lpa_status);

  return lpa_status;
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
  lpa_result_enum_type         lpa_status                 = LPA_GENERIC_ERROR;
  lpa_cmd_type                *lpa_cmd_ptr                = NULL;

  if(!lpa_util_is_slot_valid(slot_id) ||
     (set_address &&
      default_smdp_address.data_len > LPA_SERVER_ADDRESS_LEN_MAX))
  {
    LPA_MSG_ERR_2(
      "lpa_default_server_address: invalid parameter(s) - slot_id=0x%x or SMDP length=0x%x",
      slot_id, default_smdp_address.data_len);
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_cmd_ptr = (lpa_cmd_type*)LPA_MALLOC(sizeof(lpa_cmd_type));
  if (lpa_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memset(lpa_cmd_ptr, 0x00, sizeof(lpa_cmd_type));

  lpa_cmd_ptr->lpa_cb_ptr                                          = response_cb_ptr;
  lpa_cmd_ptr->user_data_ptr                                       = user_data_ptr;
  lpa_cmd_ptr->slot_id                                             = slot_id;

  if(set_address)
  {
    lpa_cmd_ptr->cmd.cmd_enum                                      = LPA_SET_DEFAULT_SERVER_ADDRESS_MSG;
    if(default_smdp_address.data_len &&
       default_smdp_address.data_ptr)
    {
      lpa_cmd_ptr->cmd.cmd.set_default_server_address_req.default_smdp_address.data_ptr =
        LPA_MALLOC(default_smdp_address.data_len);
      if(lpa_cmd_ptr->cmd.cmd.set_default_server_address_req.default_smdp_address.data_ptr)
      {
        lpa_cmd_ptr->cmd.cmd.set_default_server_address_req.default_smdp_address.data_len
                                                                   = default_smdp_address.data_len;
        memscpy(lpa_cmd_ptr->cmd.cmd.set_default_server_address_req.default_smdp_address.data_ptr,
                lpa_cmd_ptr->cmd.cmd.set_default_server_address_req.default_smdp_address.data_len,
                default_smdp_address.data_ptr,
                default_smdp_address.data_len);
      }
    }
  }
  else
  {
    lpa_cmd_ptr->cmd.cmd_enum                                      = LPA_GET_DEFAULT_SERVER_ADDRESS_MSG;
  }

  lpa_status = lpa_cmd_queue_cmd(lpa_cmd_ptr, slot_id);
  if (lpa_status != LPA_SUCCESS)
  {
    /* Free the task pointer and data since the lpa_cmd_ptr has not
       been put onto the command queue */
    LPA_FREE(lpa_cmd_ptr);
  }

  LPA_MSG_HIGH_2("Queue of command: LPA default server address, set_address 0x%x: status 0x%x",
                 set_address, lpa_status);

  return lpa_status;
} /* lpa_default_server_address */


/*=============================================================================
FUNCTION        LPA_IS_LPA_SUPPORTED

DESCRIPTION
  Indicates if LPDd, LUId, LDSd, LUIe and modem LPA are supported or not.
  This info is needed to be sent to the card in the terminal capability.

PARAMETERS
  lpdd_supported_ptr       :   Boolean to indicate if lpd is supported
  luid_supported_ptr       :   Boolean to indicate if lui is supported
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
  uint8                        lpa_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LEN];
  uim_common_efs_context_type  efs_context               = UIM_COMMON_EFS_CONTEXT_0;
  boolean                      lpdd_support              = TRUE;
  boolean                      luid_support              = TRUE;
  boolean                      ldsd_support              = TRUE;
  boolean                      luie_support              = FALSE;
  boolean                      modem_lpa_support         = TRUE;

  memset(&lpa_support_nv_content, 0x00, sizeof(lpa_support_nv_content));

  /* Read new EFS item here which determines if LPD and LUI are supported
     or not */
  if ((uim_common_efs_read(UIM_COMMON_EFS_LPA_SUPPORT,
                           UIM_COMMON_EFS_ITEM_FILE_TYPE,
                           efs_context,
                           (uint8 *)lpa_support_nv_content,
                           sizeof(lpa_support_nv_content)) == UIM_COMMON_EFS_SUCCESS))
  {
    if(lpa_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LPA_ON_AP_OR_MODEM_INDEX] != 0x00)
    {
      lpdd_support      = FALSE;
      luid_support      = FALSE;
      ldsd_support      = FALSE;
      luie_support      = FALSE;
      modem_lpa_support = FALSE;
    }
    else
    {
      lpdd_support  = lpa_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LPDD_SUPPORT_INDEX];
      luid_support  = lpa_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LUID_SUPPORT_INDEX];
      ldsd_support  = lpa_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LDSD_SUPPORT_INDEX];
      luie_support  = lpa_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LUIE_SUPPORT_INDEX];
    }
  }

  if(lpdd_supported_ptr != NULL)
  {
    *lpdd_supported_ptr = lpdd_support;
  }

  if(luid_supported_ptr != NULL)
  {
    *luid_supported_ptr = luid_support;
  }

  if(ldsd_supported_ptr != NULL)
  {
    *ldsd_supported_ptr = ldsd_support;
  }

  if(luie_supported_ptr != NULL)
  {
    *luie_supported_ptr = luie_support;
  }

  if(modem_lpa_supported_ptr != NULL)
  {
    *modem_lpa_supported_ptr = modem_lpa_support;
  }
} /* lpa_is_lpa_supported */


/*=============================================================================
FUNCTION        LPA_SET_QMI_UIM_HTTP_SIG

DESCRIPTION
  This function sets the passed signal to the LPA task

PARAMETERS
  None

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:                The request was processed successfully
    LPA_ERROR_INCORRECT_PARAMS: The parameters supplied to the API are not within
                                appropriate ranges.
===============================================================================*/
lpa_result_enum_type lpa_set_qmi_uim_http_cmd_q_sig(void)
{
  (void) rex_set_sigs(UIM_LPA_TCB,QMI_UIM_HTTP_CMD_Q_SIG);
  return LPA_SUCCESS;
} /* lpa_set_qmi_uim_http_cmd_q_sig */
