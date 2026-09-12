#ifndef SIMLOCK_CONFIG_H
#define SIMLOCK_CONFIG_H
/*===========================================================================


            S I M   L O C K   C O N F I G   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014, 2016-2017, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_config.h#3 $$ $DateTime: 2019/12/27 11:28:17 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/27/19   cj      Make device dual SIM to single SIM via RSU BLOB
08/26/19   vdc     SimLock solution for activation at Point Of Sale
07/24/17   nr      Reduce stack consumption
07/13/17   nr      Remote SIM Unlock supported in common build 
07/13/17   av      Enhanced simlock RSU solution
09/16/16   nr      Support remote and local configurations together
07/16/16   av      Postpone creation of simlock config files 
08/19/15   vv      Added support to retrieve retry attempts left for locking
12/17/14   vv      Relocate the simlock data files
09/29/14   vv      Added support for the remote SFS
09/29/14   vv      Added support for emergency only mode
09/17/14   tl      Add support for perso and deperso secure
06/20/14   vv      Added support for diag protection
04/14/14   vv      Added support for identifying conflicting codes
02/14/14   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATURE_SIMLOCK

#include "simlock.h"

/*=============================================================================

                   DATA DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_STATUS_ENUM_TYPE

   DESCRIPTION:
-------------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_DISABLED_FILE_NOT_PRESENT,
  SIMLOCK_ENABLED,
  SIMLOCK_DISABLED_FILE_PRESENT
} simlock_status_enum_type;

typedef enum {
  SIMLOCK_CK_NOT_PRESENT,
  SIMLOCK_CK_PRESENT
} simlock_ck_present_type;

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_CONFIG_FILE_PRESENCE_ENUM_TYPE

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_CONFIG_FILE_PRESENT,          /* Config file is present */
  SIMLOCK_CONFIG_FILE_ABSENT,           /* Config file is not yet created */
  SIMLOCK_CONFIG_FILE_CORRUPTED         /* Config file was probably created
                                           at some point but is now either not
                                           present or is corrupted */
} simlock_config_file_presence_enum_type;

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_CATEGORY_STATUS_TYPE

   DESCRIPTION:
     This structure contains the category status

     simlock_status_enum_type  : simlock configuration file status
     simlock_ck_present_type   : information regarding ck present or not
     sfs_type                  : Local or remote SFS
-------------------------------------------------------------------------------*/
typedef struct {
  simlock_status_enum_type    status;
  simlock_ck_present_type     ck_present;
  simlock_sfs_enum_type       sfs_type;
} simlock_category_status_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_CONFIG_DATA_TYPE

   DESCRIPTION:
     This structure contains the config data

     sfs_type          : SIMLOCK_SFS_REMOTE indicates that remote SFS needs to
                         be merged on top of local SFS.(or)
                         SIMLOCK_SFS_LOCAL indicates that only local SFS is used.
     device_mode       : device operation mode (normal or emergency only)
     slot_policy       : slot policy
     sub_policy        : subscription policy
     rsu_mode          : flavor of RSU that is configured
     category_status   : information regarding each category on a slot
     imei              : IMEI of the device slot
-------------------------------------------------------------------------------*/
typedef struct {
  simlock_sfs_enum_type                     sfs_type;
  simlock_device_mode_enum_type             device_mode;
  simlock_slot_policy_enum_type             slot_policy;
  simlock_subscription_policy_enum_type     sub_policy;
  simlock_rsu_mode_enum_type                rsu_mode;
  simlock_category_status_type              category_status[SIMLOCK_SLOT_COUNT_MAX][SIMLOCK_CATEGORY_COUNT_MAX];
  simlock_imei_type                         imei[SIMLOCK_SLOT_COUNT_MAX];
  simlock_disabled_slot_data_type           disabled_slots;
  uint8                                     nickname_len;
  uint8                                     nickname[SIMLOCK_CONFIG_NICKNAME_LEN_MAX];
} simlock_config_data_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_CHECK_CONFIG_FILE

DESCRIPTION
  This function checks if the config file is present and is of the expected
  size

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_config_file_presence_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_config_file_presence_enum_type simlock_config_check_config_file
(
  uint8 config_id
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_READ_CONFIG_DATA

DESCRIPTION
  This function allocates and reads the configuration data present in a file. If the file
  doesnt exist, and all the security conditions are verified, it returns
  default configuration

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_config_data_type* simlock_config_read_config_data
(
  uint8 config_id
);

/*===========================================================================
FUNCTION SIMLOCK_DELETE_INACTIVE_CONFIG

DESCRIPTION
  This function deletes all inactive configurations.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_delete_inactive_config(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_ACTIVATE_INACTIVE_CONFIG

DESCRIPTION
  This function activates categories from inactive configuration associated with
  given config id.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_activate_inactive_config(
  uint8 config_id
);

/*===========================================================================
FUNCTION SIMLOCK_GET_INACTIVE_CONFIG

DESCRIPTION
  This function provides config id and config nickname for all available inactive
  configurations.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_get_inactive_config(
  simlock_inactive_config_resp_type * inactive_config_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_MAX_INACTIVE_CONFIG_CREATED

DESCRIPTION
  This function returns TRUE or FALSE based on count of inactve configurations.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  boolean
    TRUE    if max inactive config are already created.
    FALSE  if max inactive configs are not created yet.

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_max_inactive_config_created(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_FREE_CONFIG_DATA

DESCRIPTION
  This function frees the memory allocated for config file data.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
void simlock_config_free_config_data
(
  simlock_config_data_type * config_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_UPDATE_AND_WRITE_POLICIES

DESCRIPTION
  This function writes the slot and subscription policies in the config file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_update_and_write_policies
(
  const simlock_set_config_policy_msg_req_type  * config_policy_ptr,
  simlock_config_data_type                      * config_data_ptr,
  uint8                                           config_id
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_UPDATE_AND_WRITE_SLOT_CONFIG

DESCRIPTION
  This function writes the slot configuration  in the config file.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_update_and_write_slot_config
(
  const simlock_disabled_slot_data_type  * disabled_slots_ptr,
  simlock_config_data_type               * config_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_UPDATE_AND_WRITE_CATEGORY_STATUS

DESCRIPTION
  This function updates and writes the status of a category in the config
  file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_update_and_write_category_status
(
  uint8                          category_index,
  uint8                          slot_index,
  simlock_status_enum_type       status,
  simlock_ck_present_type        ck_present,
  simlock_config_data_type     * config_data_ptr,
  simlock_sfs_enum_type          sfs_type,
  uint8                          config_id
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_CHECK_DIR

DESCRIPTION
  This function checks if the directory type requested exists in the EFS

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_check_dir
(
  simlock_dir_enum_type         dir_type
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_UPDATE_AND_WRITE_DEVICE_MODE

DESCRIPTION
  This function updates and writes the device mode to the config
  file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_update_and_write_device_mode
(
  simlock_device_mode_enum_type    device_mode,
  simlock_config_data_type       * config_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_UPDATE_SFS_TYPE

DESCRIPTION
  This function updates the SFS type in the config file data variable.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_update_sfs_type
(
  simlock_sfs_enum_type       update_sfs_type,
  simlock_config_data_type  * config_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_READ_CATEGORY_SFS_TYPE

DESCRIPTION
  This function retrieves the type of SFS (LOCAL/REMOTE) being used for
  storing the simlock category data.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_sfs_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_sfs_enum_type simlock_config_read_category_sfs_type
(
  const simlock_config_data_type      *config_data_ptr,
  simlock_category_enum_type           category,
  simlock_slot_enum_type               slot
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_CREATE_SIMLOCK_DIR_IF_NEEDED

DESCRIPTION
  This function creates the necessary directories needed for a config
  file to be written/created successfully

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_create_simlock_dir_if_needed
(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_CONFIG_WRITE_DATA

DESCRIPTION
  This function writes the configuration data to the config file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_config_write_data
(
  simlock_config_data_type  * config_data_ptr,
  uint8                       config_id
);

#endif /* FEATURE_SIMLOCK */

#endif /* SIMLOCK_CONFIG_H */

