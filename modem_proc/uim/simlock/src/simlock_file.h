#ifndef SIMLOCK_FILE_H
#define SIMLOCK_FILE_H
/*===========================================================================


            S I M   L O C K   F I L E   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2015, 2017 - 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_file.h#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/10/19   vdc     SimLock solution for activation at Point Of Sale
12/03/18   vgd     Move configuring pubkey from EFS to code for encrypting DEVICE_KEY
07/13/17   nr      Remote SIM Unlock supported in common build
06/24/15   stv     Perform efs sync before calling client call back
05/15/15   vv      Support for RSU in simlock
12/17/14   vv      Relocate the simlock data files
09/29/14   vv      Added support for the remote SFS
09/09/14   vv      Perform efs_sync() after writing the data
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
#define SIMLOCK_FILE_PATH_LEN     50

typedef char  simlock_filename_type[SIMLOCK_FILE_PATH_LEN];

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_FILE_READ

DESCRIPTION
  This function reads the data from the requested file by calling the SFS or
  EFS based read functions

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_file_read(
  uint8                       ** data_buf_pptr,
  uint32                       * data_buf_len_ptr,
  simlock_data_file_enum_type    file_enum,
  simlock_sfs_enum_type          sfs_type,
  uint8                          config_id
);

/*===========================================================================
FUNCTION SIMLOCK_FILE_WRITE

DESCRIPTION
  This function writes the data to the requested file by calling the SFS or
  EFS based write functions

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_file_write(
  uint8                        * data_buf_ptr,
  uint32                         data_buf_len,
  simlock_data_file_enum_type    file_enum,
  simlock_sfs_enum_type          sfs_type,
  uint8                          config_id
);

/*===========================================================================
FUNCTION SIMLOCK_DELETE_FILE

DESCRIPTION
  This function deletes the file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_delete_file(
  simlock_data_file_enum_type    file_enum,
  uint8                          config_id
);

/*===========================================================================
FUNCTION SIMLOCK_FILE_CHECK

DESCRIPTION
  This function checks if the requested file exists by calling the SFS or
  EFS based get file size functions

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_file_check(
   simlock_data_file_enum_type  file_enum,
   uint32                     * sfs_file_len_ptr,
   uint8                        config_id
);

/*===========================================================================
FUNCTION SIMLOCK_FILE_MKDIR

DESCRIPTION
  This function creates a directory in the file system

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_file_mkdir(
   const char * sfs_file_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_FILE_COMMIT_TO_FS_IF_NEEDED

DESCRIPTION
  This function checks the status of the variable, which indicates whether
  a write to the file system has happened which requires an efs sync.
  It performs efs sync operation if required.
  It also resets the variable value.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  void

SIDE EFFECTS
  NONE
===========================================================================*/
void simlock_file_commit_to_fs_if_needed(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_FILE_MKDIR_EFS

DESCRIPTION
  This function creates a directory in the EFS

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_file_mkdir_efs(
  const char * efs_file_ptr
);

#ifdef FEATURE_SIMLOCK_RSU
/*===========================================================================
FUNCTION SIMLOCK_FILE_DELETE_ALL_RSU_FILES

DESCRIPTION
  This function delete all RSU related files.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_file_delete_all_rsu_files(
  void
);
#endif /* FEATURE_SIMLOCK_RSU */
#endif /* FEATURE_SIMLOCK */

#endif /* SIMLOCK_FILE_H */