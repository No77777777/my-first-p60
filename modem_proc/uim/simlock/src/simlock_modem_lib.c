/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                        S I M L O C K  L I B R A R Y


GENERAL DESCRIPTION

  This file contains Library function that can be used to externally
  access and configure SIM Lock.

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

when       who     what, where, why
--------   ---     -----------------------------------------------------------
07/23/20   cj      Added new UIMDIAG command UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD
09/10/19   vdc     SimLock solution for activation at Point Of Sale
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
07/13/17   nr      Remote SIM Unlock supported in common build 
07/13/17   av      Enhanced simlock RSU solution
04/11/17   bcho    Check slot and sub policy values before queuing the request
02/20/17   vdc     Added support to probe CK for correctness
01/26/17   bcho    Support for slot policy valid card must on all slots
12/02/16   vdc     Added support to check RSA public key for remote lock
12/01/16   vdc     Extend blob to support lock/unlock with HCK
11/28/16   vdc     Added stub file for simlock library functions
11/03/16   bcho    Added support for ICCID simlock
10/06/16   ar      Added support for SP + EHPLMN simlock
08/03/16   vdc     Added support for SPN based SIMLOCK feature
05/20/16   vdc     Remove F3 messages for memory allocation failure
01/22/16   av      Support for autolock whitelist
01/06/16   stv     Remote simlock support
12/17/15   av      Introduced client reg and pre_policy valid IMSI check APIs
11/19/15   vdc     Reduce stack size for SIMLOCK APIs
09/28/15   vv      Support for reuse code data
07/06/15   bcho    Redundant checks for SIMLOCK
05/15/15   vv      Support for RSU set configuration in simlock
05/15/15   vv      Support for RSU in simlock
03/19/15   at      Introducing new relock feature in SIMLock
12/30/14   bcho    Support for more then 1 bytes of GID1/GID2
12/26/14   vv      Support for whitelist and blacklist codes in same category
10/21/14   vv      Add support for late simlock config registration
10/10/14   vv      Add support for remote simlock configuration
09/29/14   vv      Added support for the remote SFS
09/26/14   vv      Added support for emergency only mode
09/17/14   tl      Add support for perso and deperso secure
08/29/14   vv      Added support for temporary unlock
08/27/14   tl      Introduce network related SIM Lock slot policies
06/04/14   vv      Remove FEATURE_SIMLOCK
06/03/14   vv      Added support for OTA unlock
03/25/14   tl      Secondary revisions
03/18/14   tl      Initial Version


=============================================================================*/

/*=============================================================================

                         INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "uim_msg.h"
#include "intconv.h"
#include <stringl/stringl.h>
#include "simlock_platform.h"
#include "simlock_modem_lib.h"
#include "simlock_modem.h"
#include "simlock_util.h"
#include "simlock_validity.h"
#include "simlock_rsu_key.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/* As per the 3GPP spec. 22.022, Annex A.4, the
   length of OTA unlock request if length of every
   key doesn't exceed 8 digits is as below */
#define SIMLOCK_OTA_UNLOCK_DATA_MIN               88

#define IS_OTA_UNLOCK_DATA_LEN_VALID(x)      \
  ((x == SIMLOCK_OTA_UNLOCK_DATA_MIN) || (x == SIMLOCK_OTA_UNLOCK_DATA_MAX) ? TRUE : FALSE)

/*=============================================================================

                                  FUNCTIONS

=============================================================================*/

/*=============================================================================
FUNCTION        SIMLOCK_SET_POLICY

DESCRIPTION
  This function sets the slot and subscription policy in the SIM Lock feature
  and stores the information in the SFS.

PARAMETERS
  req_data :         Data for setting a SIMLock policy
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_set_policy(
  simlock_set_config_policy_msg_req_type      req_data,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                      *task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL )
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* Check if slot and sub policy values are within valid range */
  if(req_data.slot_policy >= SIMLOCK_SLOT_POLICY_MAX ||
     req_data.slot_policy < SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS ||
     req_data.sub_policy  >= SIMLOCK_SUBSCRIPTION_POLICY_MAX ||
     req_data.sub_policy  < SIMLOCK_SUBSCRIPTION_POLICY_INDEPENDENT)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                              = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                  = SIMLOCK_SET_SIMLOCK_POLICY_MSG;
  task_cmd_ptr->simlock_cb_ptr                                        = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                         = user_data_ptr;
  task_cmd_ptr->data.engine.message.config_policy_msg                 = req_data;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_SIMLOCK_POLICY_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_set_policy */


/*=============================================================================
FUNCTION        SIMLOCK_SET_RSU_MODE

DESCRIPTION
  This function sets simlock rsu mode and stores the information in the
  config file SFS.

PARAMETERS
  req_data :         Data for setting a SIMLock mode config
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_set_rsu_mode(
  simlock_set_rsu_mode_msg_req_type       req_data,
  simlock_callback_type                   simlock_cb_ptr,
  void                                  * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type          * task_cmd_ptr    = NULL;
  simlock_result_enum_type         simlock_status  = SIMLOCK_GENERIC_ERROR;

  if(simlock_cb_ptr == NULL ||
     req_data.rsu_mode < SIMLOCK_RSU_MODE_NO_RSU ||
     req_data.rsu_mode >= SIMLOCK_RSU_MODE_MAX)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                         = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type             = SIMLOCK_SET_RSU_MODE_MSG;
  task_cmd_ptr->simlock_cb_ptr                   = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                    = user_data_ptr;
  task_cmd_ptr->data.engine.message.rsu_mode_msg = req_data;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_RSU_MODE_MSG status 0x%x",
                  simlock_status);

  return simlock_status;
#else
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_set_rsu_mode */


/*=============================================================================
FUNCTION        SIMLOCK_SET_RSU_DEVICE_KEY

DESCRIPTION
  This function sets simlock rsu device key and stores information in the
  SFS file.

PARAMETERS
  req_data_ptr :     Data for setting a SIMLock rsu device key config
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_set_rsu_device_key(
  const simlock_set_rsu_device_key_msg_req_type  *req_data_ptr,
  simlock_callback_type                           simlock_cb_ptr,
  void                                           *user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type          * task_cmd_ptr    = NULL;
  simlock_result_enum_type         simlock_status  = SIMLOCK_GENERIC_ERROR;

  if(simlock_cb_ptr == NULL || req_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                               = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                   = SIMLOCK_SET_RSU_DEVICE_KEY_MSG;
  task_cmd_ptr->simlock_cb_ptr                         = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                          = user_data_ptr;
  task_cmd_ptr->data.engine.message.rsu_device_key_msg = *req_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_RSU_DEVICE_KEY_MSG status 0x%x",
                  simlock_status);

  return simlock_status;
#else
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_set_rsu_device_key */


/*=============================================================================
FUNCTION        SIMLOCK_HCK_LOCK

DESCRIPTION
  This function allows the client to provision one SIM Lock feature
  with up to 20 sets of that feature data. The SIM Lock feature
  will then be locked to the device to an HCK which is generated by
  the carrier using a randomly generated SALT key and the original
  control key stored by the carrier.

  When autolock indicator is enabled, the lock data may also contain a valid
  list (whitelist) of lock codes. If that is the case, once the simcard is
  inserted, if the (first) simcard data matches any of the entry in the
  whitelist, the device is autolocked to that simcard.

  In addition, an autolock indicator, blacklist indicator, maximum
  unlock attempt indicator and key generation iteration count indicator
  will be stored along with the security data in the SFS.

PARAMETERS
  req_data_ptr :     Data for locking a category
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_hck_lock(
  const simlock_set_lock_hck_msg_req_type   * req_data_ptr,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                      *task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if(simlock_cb_ptr == NULL || req_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                                  = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                      = SIMLOCK_SET_LOCK_HCK_MSG;
  task_cmd_ptr->simlock_cb_ptr                                            = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                             = user_data_ptr;
  task_cmd_ptr->data.engine.message.lock_hck_msg                          = *req_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_LOCK_HCK_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_hck_lock */


/*=============================================================================
FUNCTION        SIMLOCK_CK_LOCK

DESCRIPTION
  This function allows the client to provision one SIM Lock feature
  with up to 20 sets of that feature data. The SIM Lock feature will then be
  locked to the device with the CK passed to the client by the carrier.

  When autolock indicator is enabled, the lock data may also contain a valid
  list (whitelist) of lock codes. If that is the case, once the simcard is
  inserted, if the (first) simcard data matches any of the entry in the
  whitelist, the device is autolocked to that simcard.

  In addition, an autolock indicator, blacklist indicator, maximum
  unlock attempt indicator and key generation iteration count indicator
  will be stored along with the security data in the SFS.

PARAMETERS
  req_data_ptr :     Data for locking a category
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_ck_lock(
  const simlock_set_lock_ck_msg_req_type    * req_data_ptr,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                      *task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL || req_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                                 = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                     = SIMLOCK_SET_LOCK_CK_MSG;
  task_cmd_ptr->simlock_cb_ptr                                           = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                            = user_data_ptr;
  task_cmd_ptr->data.engine.message.lock_ck_msg                          = *req_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_LOCK_CK_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_ck_lock */


/*=============================================================================
FUNCTION        SIMLOCK_RELOCK

DESCRIPTION
  This function allows the client to relock to the specified SIM Lock feature.
  The SIM Lock feature should have been previously locked successfully with
  valid feature data and then unlocked successfully. The same CK needs to be
  passed here as the original lock API call.

PARAMETERS
  req_data :         Data for relocking a category
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_relock(
  simlock_relock_device_msg_req_type          req_data,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                      *task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL )
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                            = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                = SIMLOCK_RELOCK_DEVICE_MSG;
  task_cmd_ptr->simlock_cb_ptr                                      = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                       = user_data_ptr;
  task_cmd_ptr->data.engine.message.relock_msg                      = req_data;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RELOCK_DEVICE_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_relock */


/*=============================================================================
FUNCTION        SIMLOCK_UNLOCK

DESCRIPTION
  This function allows the user to unlock and disable one SIMLOCK feature on an
  identified slot.

PARAMETERS
  req_data :         Data for unlocking a category
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_unlock(
  simlock_unlock_device_msg_req_type          req_data,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  if (simlock_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if (req_data.ck.simlock_ck_length == 0 || req_data.ignore_ck)
  {
    UIM_MSG_ERR_2("Invalid inputs, simLock CK length: 0x%x, Ignore CK: 0x%x",
                  req_data.ck.simlock_ck_length, req_data.ignore_ck);
    return SIMLOCK_INCORRECT_PARAMS;
  }

  return simlock_unlock_trusted(req_data,
                                simlock_cb_ptr,
                                user_data_ptr);
} /* simlock_unlock */


/*=============================================================================
FUNCTION        SIMLOCK_UNLOCK_TRUSTED

DESCRIPTION
  This function allows the user to unlock and disable one SIMLOCK feature on an
  identified slot. The difference between this function and simlock_unlock() is
  that it will be exposed to trusted sources and allow the trusted caller to unlock
  a feature without a CK value.

PARAMETERS
  req_data :         Data for unlocking a category
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_unlock_trusted(
  simlock_unlock_device_msg_req_type          req_data,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                      *task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                             = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                 = SIMLOCK_UNLOCK_DEVICE_MSG;
  task_cmd_ptr->simlock_cb_ptr                                       = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                        = user_data_ptr;

  simlock_memscpy(&task_cmd_ptr->data.engine.message.unlock_msg,
                  sizeof(task_cmd_ptr->data.engine.message.unlock_msg),
                  &req_data,
                  sizeof(req_data));

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_UNLOCK_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_unlock_trusted */


/*=============================================================================
FUNCTION        SIMLOCK_GET_STATUS

DESCRIPTION
  This function retrieves all relavent SIMLOCK information and configurations
  across all slots and subscription applications including the slot and
  subscription policies.

PARAMETERS
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_get_status(
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_result_enum_type  simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL )
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* Set inactive config as FALSE for legacy function */
  simlock_status = simlock_get_status_ext(FALSE,
                                          simlock_cb_ptr,
                                          user_data_ptr);

  return simlock_status;
} /* simlock_get_status */


/*=============================================================================
FUNCTION        SIMLOCK_GET_RSU_MODE

DESCRIPTION
  This function retrieves the RSU mode.

PARAMETERS
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_get_rsu_mode(
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type                     * task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL )
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                          = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                              = SIMLOCK_GET_RSU_MODE_MSG;
  task_cmd_ptr->simlock_cb_ptr                                    = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                     = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_GET_RSU_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
#else
  (void)simlock_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_get_rsu_mode */


/*=============================================================================
FUNCTION        SIMLOCK_GET_STATUS_EXT

DESCRIPTION
  This function retrieves all relavent SIMLOCK information and configurations
  across all slots and subscription applications including the slot and
  subscription policies.

PARAMETERS
  inactive_config:   Is read of inactive configurations required
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_get_status_ext(
  boolean                                     inactive_config,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                     * task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL )
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                          = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type              = SIMLOCK_GET_STATUS_MSG;
  task_cmd_ptr->simlock_cb_ptr                    = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                     = user_data_ptr;

  task_cmd_ptr->data.engine.message.get_status_msg.inactive_config = inactive_config;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_GET_STATUS_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_get_status_ext */


/*=============================================================================
FUNCTION        SIMLOCK_GET_CATEGORY_DATA

DESCRIPTION
  This function retrieves the SIM Lock category data for one SIM Lock feature.

PARAMETERS
  req_data :         Category and slot info for which data is requested
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_get_category_data(
  simlock_get_category_data_msg_req_type      req_data,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                     * task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL )
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                          = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                              = SIMLOCK_GET_CATEGORY_DATA_MSG;
  task_cmd_ptr->data.engine.message.category_data_msg.slot        = req_data.slot;
  task_cmd_ptr->data.engine.message.category_data_msg.category    = req_data.category;
  task_cmd_ptr->simlock_cb_ptr                                    = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                     = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_GET_CATEGORY_DATA_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_get_category_data */


/*=============================================================================
FUNCTION        SIMLOCK_RUN_ALGO

DESCRIPTION
  This function runs the relevant data from EF-IMSI (3GPP), EF-IMSI_M/IMSI_T
  (3GPP2), EF-AD (3GPP), EF-GID1, and EF-GID2 from all active applications on
  the device against the SIM Lock data and configuration stored in the SIM Lock
  feature and responds with the status of which applications are allowed to
  continue to subscription ready and which applications stored invalid network
  information.

PARAMETERS
  req_data_ptr :     Data from SIM card needed to verify SIMLock
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_run_algo(
  const simlock_run_algorithm_msg_req_type  * req_data_ptr,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_task_cmd_type                      *task_cmd_ptr    = NULL;
  simlock_run_algorithm_msg_req_type         *msg_ptr         = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_SUCCESS;
  uint32                                      i               = 0;
  uint32                                      j               = 0;

  if (simlock_cb_ptr == NULL || req_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  for (i = 0; i < SIMLOCK_SLOT_COUNT_MAX; i++)
  {
    for (j = 0; j < req_data_ptr->card_data[i].num_subscriptions; j++)
    {
      if(req_data_ptr->card_data[i].subscription_data[j].imsi.imsi_len > SIMLOCK_IMSI_FILE_LEN ||
         req_data_ptr->card_data[i].subscription_data[j].ad.ad_len > SIMLOCK_AD_FILE_LEN ||
         req_data_ptr->card_data[i].subscription_data[j].gid1.gid_len > SIMLOCK_MAX_GID_LEN ||
         req_data_ptr->card_data[i].subscription_data[j].gid2.gid_len > SIMLOCK_MAX_GID_LEN ||
         req_data_ptr->card_data[i].subscription_data[j].iccid.iccid_len > SIMLOCK_ICCID_LEN ||
         req_data_ptr->card_data[i].subscription_data[j].ehplmn.ehplmn_len > (SIMLOCK_EHPLMN_NUM_MAX * SIMLOCK_PLMN_ID_LEN) ||
         (req_data_ptr->card_data[i].subscription_data[j].ehplmn.ehplmn_len % 3) != 0)
      {
        simlock_status = SIMLOCK_INCORRECT_PARAMS;
        break;
      }
    }

    if(simlock_status != SIMLOCK_SUCCESS)
    {
      /* Break the loop if we find any issue with length of SIMLOCK card data */
      break;
    }
  }

  if (simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  msg_ptr = &task_cmd_ptr->data.engine.message.algorithm_msg;

  task_cmd_ptr->cmd_type                  = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type      = SIMLOCK_RUN_ALGORITHM_MSG;
  task_cmd_ptr->user_data_ptr             = user_data_ptr;
  task_cmd_ptr->simlock_cb_ptr            = simlock_cb_ptr;

  simlock_memscpy(msg_ptr, sizeof(simlock_run_algorithm_msg_req_type),
                  req_data_ptr, sizeof(simlock_run_algorithm_msg_req_type));

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);

  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RUN_ALGORITHM_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_run_algo */


/*=============================================================================
FUNCTION        SIMLOCK_REGISTER_EVENT

DESCRIPTION
  This internal API will register the client callback with the SIM Lock task
  so that certain commands will trigger indications to the client.

PARAMETERS
  operation       : Register or deregister from SIM Lock
  simlock_evt_cb  : Callback to be registered and involked

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_register_event(
  simlock_registration_enum_type           operation,
  simlock_evt_cb_type                      simlock_evt_cb
)
{
  simlock_task_cmd_type                     * task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_evt_cb == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                   = SIMLOCK_REGISTER_CMD;
  task_cmd_ptr->data.register_msg.operation                = operation;
  task_cmd_ptr->data.register_msg.request_type             = SIMLOCK_REGISTRATION_EVENT;
  task_cmd_ptr->data.register_msg.cb_type.simlock_evt_cb   = simlock_evt_cb;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: Internal SIM Lock event registration status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_register_event */


/*=============================================================================
FUNCTION        SIMLOCK_OTA_UNLOCK

DESCRIPTION
  This function allows to unlock and disable 3GPP SIMLOCK features - NW, NS
  SP and CP on a device identified by the IMEI.

PARAMETERS
  req_data_ptr :     OTA data for unlocking the categories
  slot :             Physical slot
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_ota_unlock(
  const simlock_ota_unlock_data_type    * req_data_ptr,
  simlock_slot_enum_type                  slot,
  simlock_ota_callback_type               simlock_cb_ptr,
  void                                  * user_data_ptr
)
{
  simlock_result_enum_type     simlock_status = SIMLOCK_SUCCESS;
  simlock_task_cmd_type      * task_cmd_ptr   = NULL;

  if ((simlock_cb_ptr == NULL) ||
      (req_data_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if(!IS_OTA_UNLOCK_DATA_LEN_VALID(req_data_ptr->ota_data_len))
  {
    UIM_MSG_ERR_1("invalid input data len, 0x%x",
                  req_data_ptr->ota_data_len);
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type = SIMLOCK_OTA_UNLOCK_CMD;
  simlock_memscpy(&task_cmd_ptr->data.ota_unlock_msg.unlock_req,
                  sizeof(simlock_ota_unlock_data_type),
                  req_data_ptr,
                  sizeof(simlock_ota_unlock_data_type));
  task_cmd_ptr->data.ota_unlock_msg.slot                = slot;
  task_cmd_ptr->data.ota_unlock_msg.simlock_ota_cb_ptr  = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                           = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_OTA_UNLOCK status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_ota_unlock */


/*=============================================================================
FUNCTION        SIMLOCK_TEMPORARY_UNLOCK

DESCRIPTION
  This function allows to set a temporary unlock for a requested duration or
  unset an existing temporary unlock. If the function is called for a category,
  which is currently in a temporary unlocked state, then the new temporary unlock
  duration overrides the existing one.

PARAMETERS
  req_data_ptr :     Pointer to the temporary unlock request
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_temporary_unlock(
  const simlock_temporary_unlock_msg_req_type * req_data_ptr,
  simlock_callback_type                         simlock_cb_ptr,
  void                                        * user_data_ptr
)
{
  simlock_result_enum_type     simlock_status = SIMLOCK_SUCCESS;
  simlock_task_cmd_type      * task_cmd_ptr   = NULL;

  if ((req_data_ptr == NULL) ||
      (simlock_cb_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if((req_data_ptr->temporary_unlock_type != SIMLOCK_TEMPORARY_UNLOCK_SET) &&
     (req_data_ptr->temporary_unlock_type != SIMLOCK_TEMPORARY_UNLOCK_UNSET))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if((req_data_ptr->temporary_unlock_type == SIMLOCK_TEMPORARY_UNLOCK_SET) &&
     (req_data_ptr->duration == 0))
  {
    UIM_MSG_ERR_0("unlock duration is invalid");
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                                       = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                           = SIMLOCK_TEMPORARY_UNLOCK_MSG;
  task_cmd_ptr->data.engine.message.temporary_unlock_msg                       = *req_data_ptr;
  task_cmd_ptr->simlock_cb_ptr                                                 = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                                  = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_TEMPORARY_UNLOCK status 0x%x",
                 simlock_status);

  return simlock_status;
}/* simlock_temporary_unlock */


/*=============================================================================
FUNCTION        SIMLOCK_SET_DEVICE_OPERATION_MODE

DESCRIPTION
  This function allows to set a device in the emergency only mode or set the
  device back in the normal operation mode.

PARAMETERS
  req_data :         device operation mode request
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_set_device_operation_mode(
  simlock_set_device_mode_msg_req_type        req_data,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
)
{
  simlock_result_enum_type     simlock_status = SIMLOCK_SUCCESS;
  simlock_task_cmd_type      * task_cmd_ptr   = NULL;

  if (simlock_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                      = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                          = SIMLOCK_SET_DEVICE_MODE_MSG;
  task_cmd_ptr->data.engine.message.device_mode_msg.mode      = req_data.mode;
  task_cmd_ptr->user_data_ptr                                 = user_data_ptr;
  task_cmd_ptr->simlock_cb_ptr                                = simlock_cb_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_DEVICE_OPERATION_MODE status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_set_device_operation_mode */


/*=============================================================================
FUNCTION        SIMLOCK_REGISTER_REMOTE_SFS_OPERATION

DESCRIPTION
  This internal API will register the client callback with the SIM Lock task
  so that remote SFS storage operations (read/write) on the simlock data
  can be performed.

PARAMETERS
  registration       : Register or deregister from SIM Lock
  simlock_remote_cb  : Callback to be registered and involked

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_register_remote_sfs_operation(
  simlock_registration_enum_type                registration,
  simlock_remote_sfs_request_cb_type            client_cb
)
{
  simlock_task_cmd_type           * task_cmd_ptr    = NULL;
  simlock_result_enum_type          simlock_status  = SIMLOCK_GENERIC_ERROR;

  if ((registration == SIMLOCK_REGISTER) &&
      (client_cb == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                        = SIMLOCK_REGISTER_CMD;
  task_cmd_ptr->data.register_msg.request_type                  = SIMLOCK_REGISTRATION_REMOTE_SFS;
  task_cmd_ptr->data.register_msg.operation                     = registration;
  task_cmd_ptr->data.register_msg.cb_type.simlock_remote_req_cb = client_cb;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: Internal SIM Lock remote registration status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_register_remote_sfs_operation */


/*=============================================================================
FUNCTION        SIMLOCK_SET_REMOTE_SFS

DESCRIPTION
  This function allows to move the simlock data from a local SFS to remote
  SFS or vice-versa.

PARAMETERS
  req_data       : Enable or disable remote SFS
  simlock_cb_ptr : Command response call back
  user_data_ptr  : Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_set_remote_sfs(
  simlock_remote_sfs_request_enum_type         req_data,
  simlock_set_remote_sfs_callback_type         simlock_cb_ptr,
  void                                       * user_data_ptr
)
{
  simlock_result_enum_type     simlock_status = SIMLOCK_SUCCESS;
  simlock_task_cmd_type      * task_cmd_ptr   = NULL;

  if (simlock_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if((req_data != SIMLOCK_REMOTE_SFS_DISABLE) &&
     (req_data != SIMLOCK_REMOTE_SFS_ENABLE))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                       = SIMLOCK_SET_REMOTE_SFS_CMD;
  task_cmd_ptr->data.remote_sfs_msg.request_type               = req_data;
  task_cmd_ptr->data.remote_sfs_msg.remote_sfs_cb_ptr          = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                  = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_SET_REMOTE_SFS_REQUEST status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_set_remote_sfs */


/*=============================================================================
FUNCTION        SIMLOCK_ADD_LOCK_CODES_CK

DESCRIPTION
  This function allows the client to add up to 20 sets of feature data
  for a SIMLock feature that is already locked. The feature data will be
  added to the existing feature data, if the CK passed matches CK with which
  the device was locked.

PARAMETERS
  req_data_ptr :     Additional lock code data
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_add_lock_codes_ck(
  const simlock_add_lock_codes_ck_msg_req_type  * req_data_ptr,
  simlock_callback_type                           simlock_cb_ptr,
  void                                          * user_data_ptr
)
{
  simlock_task_cmd_type                      *task_cmd_ptr    = NULL;
  simlock_result_enum_type                    simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL || req_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                                       = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                           = SIMLOCK_ADD_LOCK_CODES_CK_MSG;
  task_cmd_ptr->simlock_cb_ptr                                                 = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                                  = user_data_ptr;
  task_cmd_ptr->data.engine.message.add_lock_codes_ck_msg                      = *req_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_ADD_LOCK_CODES_CK_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_add_lock_codes_ck */


/*=============================================================================
FUNCTION        SIMLOCK_ADD_LOCK_CODES_HCK

DESCRIPTION
  This function allows the client to add up to 20 sets of feature data
  for a SIMLock feature that is already locked. The feature data will be
  added to the existing feature data, if the HCK passed matches HCK with which
  the device was locked.

PARAMETERS
  req_data_ptr :     Additional lock code data
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_add_lock_codes_hck(
  const simlock_add_lock_codes_hck_msg_req_type  * req_data_ptr,
  simlock_callback_type                            simlock_cb_ptr,
  void                                           * user_data_ptr
)
{
  simlock_task_cmd_type           *task_cmd_ptr    = NULL;
  simlock_result_enum_type         simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL || req_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));

  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                                                       = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type                                           = SIMLOCK_ADD_LOCK_CODES_HCK_MSG;
  task_cmd_ptr->simlock_cb_ptr                                                 = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                                                  = user_data_ptr;
  task_cmd_ptr->data.engine.message.add_lock_codes_hck_msg                     = *req_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_ADD_LOCK_CODES_HCK_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_add_lock_codes_hck */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_GET_ENCRYPTED_KEY

DESCRIPTION
  This function gets the modem key for the RSU. The random key is encrypted with
  a public key that is provisioned in the device.

PARAMETERS
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_rsu_get_encrypted_key(
  simlock_rsu_callback_type                   simlock_rsu_cb_ptr,
  void                                      * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type           * task_cmd_ptr    = NULL;
  simlock_result_enum_type          simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_rsu_cb_ptr == NULL )
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_RSU_CMD;
  task_cmd_ptr->data.rsu_msg.msg_type          = SIMLOCK_RSU_GET_KEY_MSG;
  task_cmd_ptr->data.rsu_msg.rsu_cb_ptr        = simlock_rsu_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RSU_GET_ENCRYPTED_KEY status 0x%x",
                 simlock_status);

  return simlock_status;
#else
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_rsu_get_encrypted_key */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_SET_CONFIGURATION

DESCRIPTION
  This function sets the simlock configuration provided in the request. It
  verifies the integrity of the data in the request, before setting the
  simlock configuration.

PARAMETERS
  req_data :         Simlock configuration data
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_rsu_set_configuration(
  simlock_data_type                       req_data,
  simlock_rsu_callback_type               simlock_rsu_cb_ptr,
  void                                  * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type          * task_cmd_ptr    = NULL;
  simlock_result_enum_type         simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_rsu_cb_ptr == NULL ||
      req_data.data_len  == 0     ||
      req_data.data_ptr  == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_RSU_CMD;
  task_cmd_ptr->data.rsu_msg.msg_type          = SIMLOCK_RSU_SET_CONFIGURATION_MSG;
  task_cmd_ptr->data.rsu_msg.rsu_cb_ptr        = simlock_rsu_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  task_cmd_ptr->data.rsu_msg.message.rsu_data.data_len = req_data.data_len;

  simlock_util_deep_copy((void **)&task_cmd_ptr->data.rsu_msg.message.rsu_data.data_ptr,
                         (void *)req_data.data_ptr,
                          req_data.data_len);

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr->data.rsu_msg.message.rsu_data.data_ptr);
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RSU_SET_CONFIGURATION status 0x%x",
                 simlock_status);

  return simlock_status;
#else
  (void)req_data;
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_rsu_set_configuration */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_GENERATE_BLOB_REQUEST

DESCRIPTION
  This function generates simlock blob request requested by unlock server.

PARAMETERS
  simlock_cb_ptr:    Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_rsu_generate_blob_request(
  simlock_rsu_callback_type               simlock_rsu_cb_ptr,
  void                                  * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type          * task_cmd_ptr    = NULL;
  simlock_result_enum_type         simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_rsu_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_RSU_CMD;
  task_cmd_ptr->data.rsu_msg.msg_type          = SIMLOCK_RSU_BLOB_REQUEST_MSG;
  task_cmd_ptr->data.rsu_msg.rsu_cb_ptr        = simlock_rsu_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RSU_BLOB_REQUEST_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
#else
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_rsu_generate_blob_request */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_MANAGE_DELAY_TIMER_REQUEST

DESCRIPTION
  This function start/stop the delay timer if not already.

PARAMETERS
  simlock_cb_ptr:    Command response call back
  user_data_ptr :    Client's user data
  run_timer       :    Indicates start/stop delay timer

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_rsu_manage_delay_timer_request(
  boolean                                 run_timer,
  simlock_rsu_callback_type               simlock_rsu_cb_ptr,
  void                                  * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type          * task_cmd_ptr    = NULL;
  simlock_result_enum_type         simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_rsu_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_RSU_CMD;
  task_cmd_ptr->data.rsu_msg.msg_type          = (run_timer == FALSE)? SIMLOCK_RSU_DELAY_TIMER_STOP_REQUEST_MSG : SIMLOCK_RSU_DELAY_TIMER_START_REQUEST_MSG;
  task_cmd_ptr->data.rsu_msg.rsu_cb_ptr        = simlock_rsu_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_2("Queue of SIMLOCK command: status 0x%x run_timer 0x%x",
                 simlock_status,
                 run_timer);

  return simlock_status;
#else
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_rsu_manage_delay_timer_request */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_CHECK_PUBLIC_KEY

DESCRIPTION
  This function validates input public key with the public key in the code or in EFS
  (key in code takes priority)

PARAMETERS
  remote_lock_public_key_ptr: Pointer to RSA public key
  simlock_rsu_cb_ptr:         Command response call back
  user_data_ptr :             Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:        Public key comparison is successful
    SIMLOCK_GENERIC_ERROR:  Public key comparison is not successful
===============================================================================*/
simlock_result_enum_type simlock_rsu_check_public_key (
  const simlock_remote_lock_public_key_type  *remote_lock_public_key_ptr,
  simlock_rsu_callback_type                   simlock_rsu_cb_ptr,
  void                                       *user_data_ptr
)
{
  simlock_result_enum_type  simlock_status = SIMLOCK_SUCCESS;
  simlock_task_cmd_type    *task_cmd_ptr   = NULL;

  if(remote_lock_public_key_ptr == NULL ||
     simlock_rsu_cb_ptr         == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_RSU_CMD;
  task_cmd_ptr->data.rsu_msg.msg_type          = SIMLOCK_RSU_CHECK_PUBLIC_KEY_MSG;
  task_cmd_ptr->data.rsu_msg.rsu_cb_ptr        = simlock_rsu_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  simlock_memscpy(task_cmd_ptr->data.rsu_msg.message.public_key.modulus,
                  sizeof(task_cmd_ptr->data.rsu_msg.message.public_key.modulus),
                  remote_lock_public_key_ptr->modulus,
                  sizeof(remote_lock_public_key_ptr->modulus));

  simlock_memscpy(task_cmd_ptr->data.rsu_msg.message.public_key.exponent,
                  sizeof(task_cmd_ptr->data.rsu_msg.message.public_key.exponent),
                  remote_lock_public_key_ptr->exponent,
                  sizeof(remote_lock_public_key_ptr->exponent));

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RSU_CHECK_PUBLIC_KEY_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_rsu_check_public_key */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_GENERATE_HASH

DESCRIPTION
  This function, using the shared symmetric key, generates hmac on the data
  that is passed in.

PARAMETERS 
  req_data       :   Data on which the hmac is to be generated 
  simlock_cb_ptr :   Command response call back
  user_data_ptr  :   Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_rsu_generate_hash(
  simlock_data_type                           req_data,
  simlock_rsu_callback_type                   simlock_rsu_cb_ptr,
  void                                      * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type           * task_cmd_ptr    = NULL;
  simlock_result_enum_type          simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_rsu_cb_ptr == NULL ||
      req_data.data_len == 0 ||
      req_data.data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_RSU_CMD;
  task_cmd_ptr->data.rsu_msg.msg_type          = SIMLOCK_RSU_GENERATE_HASH_MSG;
  task_cmd_ptr->data.rsu_msg.rsu_cb_ptr        = simlock_rsu_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  task_cmd_ptr->data.rsu_msg.message.rsu_data.data_len = req_data.data_len;

  simlock_util_deep_copy((void **)&task_cmd_ptr->data.rsu_msg.message.rsu_data.data_ptr,
                         (void *)req_data.data_ptr,
                          req_data.data_len);

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr->data.rsu_msg.message.rsu_data.data_ptr);
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RSU_GENERATE_HASH status 0x%x",
                 simlock_status);

  return simlock_status;
#else
  (void)req_data;
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_rsu_generate_hash */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_RETRIEVE_VERSION

DESCRIPTION
  This function retrieves the highest major and minor versions of incoming blob
  supported by RSU engine.

PARAMETERS 
  simlock_cb_ptr :   Command response call back
  user_data_ptr  :   Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.

===============================================================================*/
simlock_result_enum_type simlock_rsu_retrieve_version(
  simlock_rsu_callback_type                   simlock_rsu_cb_ptr,
  void                                      * user_data_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_task_cmd_type           * task_cmd_ptr    = NULL;
  simlock_result_enum_type          simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_rsu_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_RSU_CMD;
  task_cmd_ptr->data.rsu_msg.msg_type          = SIMLOCK_RSU_RETRIEVE_VERSION_MSG;
  task_cmd_ptr->data.rsu_msg.rsu_cb_ptr        = simlock_rsu_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_RSU_RETRIEVE_VERSION status 0x%x",
                 simlock_status);

  return simlock_status;
#else
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_rsu_retrieve_version */


/*=============================================================================
FUNCTION        SIMLOCK_VERIFY_IMSI

DESCRIPTION
  This function validates input IMSI. It goes through all IMSIs which SIMLOCK
  engine found as valid in last algo run and check whether input IMSI matches
  with any.

PARAMETERS
  imsi:         EF IMSI Data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:        IMSI's subscription is valid.
    SIMLOCK_GENERIC_ERROR:  IMSI's subscription is not valid.
    SIMLOCK_UNSUPPORTED:    SIMLOCK feature is not defined.
===============================================================================*/
simlock_result_enum_type simlock_verify_imsi (
  simlock_imsi_type                       imsi
)
{
  simlock_result_enum_type simlock_status = SIMLOCK_GENERIC_ERROR;

  if(simlock_validity_check_imsi(imsi))
  {
    simlock_status = SIMLOCK_SUCCESS;
  }

  return simlock_status;
} /* simlock_verify_imsi */


/*=============================================================================
FUNCTION        SIMLOCK_VERIFY_IMSI_PRE_POLICY

DESCRIPTION
  This function validates input IMSI. It goes through all the IMSIs which
  SIMLOCK engine found as valid prior to applying the policies in the last algo
  run and checks whether input IMSI matches with any.

PARAMETERS
  imsi:         EF IMSI Data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:        IMSI's subscription was valid even prior to the
                            policies check performed by simlock.
    SIMLOCK_GENERIC_ERROR:  IMSI's subscription was not valid prior to the
                            policies check performed by simlock.
    SIMLOCK_UNSUPPORTED:    SIMLOCK feature is not defined.
===============================================================================*/
simlock_result_enum_type simlock_verify_imsi_pre_policy (
  simlock_imsi_type                                  imsi
)
{
  simlock_result_enum_type simlock_status = SIMLOCK_GENERIC_ERROR;

  if(simlock_validity_check_imsi_pre_policy(imsi))
  {
    /* IMSI is valid prior to applying the sub/slot policies */
    simlock_status = SIMLOCK_SUCCESS;
  }

  /* We are here because IMSI is determined to be as invalid:
     1. Prior to applying the policies only OR
     2. Prior to and after applying the policies */
  return simlock_status;
} /* simlock_verify_imsi_pre_policy */


/*=============================================================================
FUNCTION        SIMLOCK_ACTIVATE_CONFIGURATION

DESCRIPTION
  This function activates SIMLOCK categories associated with config id .

PARAMETERS
  config id :        config id
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.
===============================================================================*/
simlock_result_enum_type simlock_activate_configuration(
  uint8                                         config_id,
  simlock_callback_type                         simlock_cb_ptr,
  void                                        * user_data_ptr
)
{
  simlock_task_cmd_type           * task_cmd_ptr    = NULL;
  simlock_result_enum_type          simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type           = SIMLOCK_ACTIVATE_CONFIG_MSG;
  task_cmd_ptr->simlock_cb_ptr                 = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;
  task_cmd_ptr->data.engine.message.activate_configuration.config_id =
    config_id;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_ACTIVATE_CONFIG_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
} /* simlock_activate_configuration */


/*=============================================================================
FUNCTION        SIMLOCK_GET_TOKEN

DESCRIPTION
  This function returns token associated with enabled SIMLOCK cateogries.

PARAMETERS
  simlock_cb_ptr :   Command response call back
  user_data_ptr :    Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:                     The command structure was properly
                                         generated and queued onto the SIM Lock
                                         Command Queue.
    SIMLOCK_INCORRECT_PARAMS:            The parameters supplied to the API are not
                                         within appropriate ranges.
    SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED: The sufficient memory is not available to
                                         process the request.
    SIMLOCK_UNSUPPORTED:                 The request is not supported.
    SIMLOCK_GENERIC_ERROR:               An error has occurred while processing
                                         the request.
===============================================================================*/
simlock_result_enum_type simlock_get_token(
  simlock_callback_type                         simlock_cb_ptr,
  void                                        * user_data_ptr
)
{
  simlock_task_cmd_type           * task_cmd_ptr    = NULL;
  simlock_result_enum_type          simlock_status  = SIMLOCK_GENERIC_ERROR;

  if (simlock_cb_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->cmd_type                       = SIMLOCK_ENGINE_CMD;
  task_cmd_ptr->data.engine.msg_type           = SIMLOCK_GET_TOKEN_MSG;
  task_cmd_ptr->simlock_cb_ptr                 = simlock_cb_ptr;
  task_cmd_ptr->user_data_ptr                  = user_data_ptr;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_1("Queue of SIMLOCK command: SIMLOCK_GET_TOKEN_MSG status 0x%x",
                 simlock_status);

  return simlock_status;
}/* simlock_get_token */

#endif /* FEATURE_SIMLOCK */

