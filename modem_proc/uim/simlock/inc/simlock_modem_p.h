#ifndef SIMLOCK_MODEM_P_H
#define SIMLOCK_MODEM_P_H
/*===========================================================================


            S I M L O C K   D E F I N I T I O N S

                      A N D   F U N C T I O N S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2015, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     -----------------------------------------------------------
09/16/21   tq      Update category based on slot policy during initialization
12/17/15   av      Introduced client reg and pre_policy valid IMSI check APIs
03/04/15   vv      Run algorithm after a lock operation
10/21/14   vv      Add support for late simlock config registration
10/10/14   vv      Add support for remote simlock configuration
09/29/14   vv      Added support for the remote SFS
09/29/14   vv      Added support for emergency only mode
09/17/14   tl      Add support for perso and deperso secure
08/29/14   vv      Added support for temporary unlock
06/04/14   vv      Remove FEATURE_SIMLOCK
03/25/14   tl      Secondary revisions
02/25/14   tl      Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "simlock_common.h"
#include "simlock_modem_lib.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/* ----------------------------------------------------------------------------
   DATA TYPE:      simlock_token_id_type
------------------------------------------------------------------------------*/
typedef uint16 simlock_token_id_type;

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_REGISTRATION_REQUEST_ENUM_TYPE

   DESCRIPTION: This enum specifies if the registration is for events or
                remote operations
-------------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_REGISTRATION_EVENT,
  SIMLOCK_REGISTRATION_REMOTE_SFS
} simlock_registration_request_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_FILE_ENUM_TYPE

   DESCRIPTION: This enum specifies the simlock file types
-----------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_FILE_TYPE_CONFIG,
  SIMLOCK_FILE_TYPE_CATEGORY
} simlock_file_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_FILE_OPERATION_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the type of operation that needs to be performed
     on the simlock data
-------------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_FILE_OPERATION_READ,
  SIMLOCK_FILE_OPERATION_WRITE
} simlock_file_operation_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_REMOTE_SFS_REQUEST_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the remote SFS request type
-----------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_REMOTE_SFS_DISABLE,
  SIMLOCK_REMOTE_SFS_ENABLE
} simlock_remote_sfs_request_enum_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_FILE_TYPE

   DESCRIPTION:
     This structure contains simlock data file type
-------------------------------------------------------------------------------*/
typedef struct
{
  simlock_file_enum_type          file_type;
  union
  {
    struct
    {
      simlock_slot_enum_type      slot;
      simlock_category_enum_type  category;
    } category_data;
  } data;
} simlock_file_type;

/* ----------------------------------------------------------------------------
   DATA TYPE:      SIMLOCK_SET_REMOTE_SFS_CALLBACK_TYPE
-----------------------------------------------------------------------------*/
/** @brief Client callback function.

    The parameter to the callback function contains the results of the
    procedures performed at the client's request.
*/
typedef void (*simlock_set_remote_sfs_callback_type) (simlock_result_enum_type      status,
                                                      const void                  * user_data_ptr);

/* ----------------------------------------------------------------------------
   DATA TYPE:      SIMLOCK_REMOTE_SFS_RESPONSE_CALLBACK_TYPE
-------------------------------------------------------------------------------*/
/** response callback function passed in to perform remote storage
    (read/write) operations on simlock configuration data */
typedef void (*simlock_remote_sfs_response_cb_type) (simlock_result_enum_type            status,
                                                     simlock_token_id_type               token,
                                                     const simlock_data_type           * simlock_data_ptr);

/* ----------------------------------------------------------------------------
   DATA TYPE:      SIMLOCK_REMOTE_SFS_REQUEST_CALLBACK_TYPE
-------------------------------------------------------------------------------*/
/** callback function registered by a client to perform remote storage
    (read/write) operations on simlock configuration data */
typedef simlock_result_enum_type (*simlock_remote_sfs_request_cb_type) (simlock_file_operation_enum_type       operation,
                                                                        simlock_file_type                      file,
                                                                        simlock_token_id_type                  token,
                                                                        const simlock_data_type              * simlock_data_ptr,
                                                                        simlock_remote_sfs_response_cb_type    simlock_remote_resp_cb);

/*=============================================================================

                                  FUNCTIONS

=============================================================================*/

/*=============================================================================
FUNCTION        SIMLOCK_REGISTER_REMOTE_SFS_OPERATION

DESCRIPTION
  This internal API will register the client callback with the SIM Lock task
  so that remote SFS storage operations (read/write) on the simlock data
  can be performed.

PARAMETERS
  registration  : Register or deregister from SIM Lock
  client_cb     : Callback to be registered and involked

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:           The command structure was properly
                               generated and queued onto the SIM Lock
                               Command Queue.
    SIMLOCK_INCORRECT_PARAMS:  The parameters supplied to the API are not
                               within appropriate ranges.

===============================================================================*/
simlock_result_enum_type simlock_register_remote_sfs_operation(
  simlock_registration_enum_type                registration,
  simlock_remote_sfs_request_cb_type            client_cb
);

/*=============================================================================
FUNCTION        SIMLOCK_UNLOCK_TRUSTED

DESCRIPTION
  This function allows the user to unlock and disable one SIMLOCK feature on an
  identified slot. The difference between this function and simlock_unlock() is
  that it will be exposed to trusted sources and allow the trusted caller to unlock
  a feature without a CK value.

PARAMETERS
  req_data       :   Data for unlocking a category
  simlock_cb_ptr :   Command response call back
  user_data_ptr  :   Client's user data

RETURN VALUE
  simlock_result_enum_type:

    SIMLOCK_SUCCESS:           The command structure was properly
                               generated and queued onto the SIM Lock
                               Command Queue.
    SIMLOCK_INCORRECT_PARAMS:  The parameters supplied to the API are not
                               within appropriate ranges.

===============================================================================*/
simlock_result_enum_type simlock_unlock_trusted(
  simlock_unlock_device_msg_req_type          req_data,
  simlock_callback_type                       simlock_cb_ptr,
  void                                      * user_data_ptr
);

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

    SIMLOCK_SUCCESS:           The command structure was properly
                               generated and queued onto the SIM Lock
                               Command Queue.
    SIMLOCK_INCORRECT_PARAMS:  The parameters supplied to the API are not
                               within appropriate ranges.

===============================================================================*/
simlock_result_enum_type simlock_set_remote_sfs (
  simlock_remote_sfs_request_enum_type         req_data,
  simlock_set_remote_sfs_callback_type         simlock_cb_ptr,
  void                                       * user_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_POLICY_USES_SLOT1_CONFIG_FOR_ALL_SLOTS

DESCRIPTION
  This function checks whether slot policy uses slot1 configuration also for
  all additional slots.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE   : If policy uses slot1 config
  FALSE  : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_policy_uses_slot1_config_for_all_slots
(
  simlock_slot_policy_enum_type slot_policy
);

#endif /* SIMLOCK_MODEM_P_H */
