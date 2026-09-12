#ifndef MMGSDI_FILE_FS_H
#define MMGSDI_FILE_FS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            M M G S D I   F I L E   F S   U P D A T E   H E A D E R S


GENERAL DESCRIPTION

  This header file contains the processing of all functions related to
  SIM file access fallback to File System

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2017, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_file_fs.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/16/19   sg      Fallback to file system/cache for missing files
01/16/17   shr     Initial version
===========================================================================*/

/*=============================================================================

                         INCLUDE FILES FOR MODULE

=============================================================================*/
#include "mmgsdilib_common.h"
#include "uim_v.h"

/* ==========================================================================
FUNCTION:      MMGSDI_UTIL_IS_FILE_OK_IN_ORIG_SVC_TABLE

DESCRIPTION:
  This function checks if the file access is allowed by the original
  service table value from the SIM card or not

DEPENDENCIES:
  None

LIMITATIONS:
  The function is limited to:
  1. Checking access via MMGSDI_EF_ENUM_ACCESS
  2. Checking access on files for which fallback to FS support is present
  For files not meeting above criteria, function returns success
  indicating access allowed.

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS:          OK to access the file.
  MMGSDI_ACCESS_DENIED:    Access conditions not satisfied for this EF.
  MMGSDI_NOT_SUPPORTED:    It is not OK to access the file (for reasons
                           other than access violation).

SIDE EFFECTS:
  None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fs_is_file_ok_in_orig_svc_table (
  mmgsdi_session_id_type    session_id,
  mmgsdi_app_enum_type      app_type,
  const mmgsdi_access_type *mmgsdi_access_ptr
);

/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_READ_RSP

DESCRIPTION:
  This function will parse the UIM response to a Read request
  and provide content from File System if file system fallback
  is enabled for the file

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS: Read from card was successful or data was read
                  from FS
  report_status:  Incorrect input params/Fallback to FS disabled

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fs_parse_uim_read_rsp (
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
);

/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_WRITE_RSP

DESCRIPTION:
  This function will parse the UIM response to a Write request
  and provide content from File System if file system fallback
  is enabled for the file

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type
 
  MMGSDI_SUCCESS: Write to card was successful or data was written
                  into FS
  report_status:  Incorrect input params/Fallback to FS disabled

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fs_parse_uim_write_rsp (
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
);

/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_GET_FILE_ATTR_RSP

DESCRIPTION:
  This function will parse the UIM response to Select request
  and provide content from File System if file system fallback
  is enabled for the file

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS: File attributes were already present in UIM
                  response or File attributes returned from FS
  report_status:  Incorrect input params/Fallback to FS disabled

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fs_parse_uim_get_file_attr_rsp(
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
);

/*==========================================================================
FUNCTION  MMGSDI_FILE_FS_CLEAR_MEMORY

DESCRIPTION
  Off-target function to clear heap allocations w.r.t fallback to File system.

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  NONE
==========================================================================*/
void mmgsdi_file_fs_clear_memory(void);

#endif /* MMGSDI_FILE_FS_H */
