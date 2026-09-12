/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                        S I M L O C K  S T U B  L I B R A R Y


GENERAL DESCRIPTION

  This file contains stubbed library function that can be used to externally
  access .

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

when       who     what, where, why
--------   ---     -----------------------------------------------------------
07/23/20   cj      Added new UIMDIAG command UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD 
10/23/19   sch     Add stub to get category status when SIMLOCK is disabled
09/10/19   vdc     SimLock solution for activation at Point Of Sale
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
07/13/17   nr      Remote SIM Unlock supported in common build 
07/13/17   av      Enhanced simlock RSU solution
12/02/16   vdc     Added support to check RSA public key for remote lock 
11/28/16   vdc     Initial Version

=============================================================================*/

/*=============================================================================

                         INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "customer.h"
#include "simlock_modem_lib.h"
#include "simlock_modem_p.h"

#ifndef FEATURE_SIMLOCK

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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
} /* simlock_set_rsu_mode */


/*=============================================================================
FUNCTION        SIMLOCK_GET_RSU_MODE

DESCRIPTION
  This function retrieves the RSU mode.

PARAMETERS
  simlock_cb_ptr :   Command response call back
  user_data_ptr  :    Client's user data

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
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
} /* simlock_get_rsu_mode */


/*=============================================================================
FUNCTION        SIMLOCK_SET_RSU_DEVICE_KEY

DESCRIPTION
  This function sets simlock rsu device key and stores the information in the
  SFS file.

PARAMETERS
  req_data :         Data for setting a SIMLock rsu device key config
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
  const simlock_set_rsu_device_key_msg_req_type * req_data_ptr,
  simlock_callback_type                           simlock_cb_ptr,
  void                                          * user_data_ptr
)
{
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
} /* simlock_get_status */


/*=============================================================================
FUNCTION        SIMLOCK_GET_STATUS_EXT

DESCRIPTION
  This function retrieves all relevent SIMLOCK information and configurations
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
  (void)inactive_config;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)operation;
  (void)simlock_evt_cb;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data_ptr;
  (void)slot;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)registration;
  (void)client_cb;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data_ptr;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)req_data;
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
} /* simlock_rsu_set_configuration */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_GENERATE_BOLB_REQUEST

DESCRIPTION
  This function generates simlock blob request requested by unlock server.

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
simlock_result_enum_type simlock_rsu_generate_blob_request(
  simlock_rsu_callback_type               simlock_rsu_cb_ptr,
  void                                  * user_data_ptr
)
{
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)remote_lock_public_key_ptr;
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
}


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
  (void)req_data;
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
} /* simlock_rsu_generate_hash */


/*=============================================================================
FUNCTION        SIMLOCK_RSU_RETRIEVE_VERSION

DESCRIPTION
  This function retrieves in first two bytes of the response, the highest
  major and minor versions respectively of blob supported by RSU engine.

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
  (void)simlock_rsu_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
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
  (void)imsi;

  return SIMLOCK_UNSUPPORTED;
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
  (void)imsi;

  return SIMLOCK_UNSUPPORTED;
} /* simlock_verify_imsi_pre_policy */


/*=============================================================================
FUNCTION        SIMLOCK_ACTIVATE_CONFIGURATION

DESCRIPTION
  This function activates SIMLOCK categories associated with config id .

PARAMETERS
  config_id :        config id
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
  (void)config_id;
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
}/* simlock_activate_configuration */


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
  (void)simlock_cb_ptr;
  (void)user_data_ptr;

  return SIMLOCK_UNSUPPORTED;
}/* simlock_get_token */
#endif /* !FEATURE_SIMLOCK */

