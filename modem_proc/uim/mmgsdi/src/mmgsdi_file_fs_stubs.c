/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            M M G S D I   F I L E   F S   S T U B S   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the processing of all stub functions related to
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

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_file_fs_stubs.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/16/19   sg      Fallback to file system/cache for missing files
01/16/17   shr     Initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "intconv.h"
#include "mmgsdiutil.h"
#include "mmgsdi_file_fs.h"


/* ==========================================================================
FUNCTION:      MMGSDI_UTIL_IS_FILE_OK_IN_ORIG_SVC_TABLE

DESCRIPTION:
  Stub function

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
)
{
  return MMGSDI_SUCCESS;
} /* mmgsdi_file_fs_is_file_ok_in_orig_svc_table */


/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_READ_RSP

DESCRIPTION:
  Stub function

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS:          The command structure was properly generated
                           and queued onto the MMGSDI Command Queue.
  MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                           within appropriate ranges.

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_file_fs_parse_uim_read_rsp (
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
)
{
  (void)mod_uim_rsp_ptr;
  (void)req_header_ptr;
  (void)extra_param_ptr;

  return report_status;
} /* mmgsdi_file_fs_parse_uim_read_rsp */


/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_WRITE_RSP

DESCRIPTION:
  Stub function

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type
 
  MMGSDI_SUCCESS:          The command structure was properly generated
                           and queued onto the MMGSDI Command Queue.
  MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                           within appropriate ranges.

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_file_fs_parse_uim_write_rsp (
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
)
{
  (void)mod_uim_rsp_ptr;
  (void)req_header_ptr;
  (void)extra_param_ptr;   

  return report_status;
} /* mmgsdi_file_fs_parse_uim_write_rsp */


/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_GET_FILE_ATTR_RSP

DESCRIPTION:
  Stub function

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS:          The command structure was properly generated
                           and queued onto the MMGSDI Command Queue.
  MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                           within appropriate ranges.

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_file_fs_parse_uim_get_file_attr_rsp(
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
)
{
  (void)mod_uim_rsp_ptr;
  (void)req_header_ptr;
  (void)extra_param_ptr;   

  return report_status;
} /* mmgsdi_file_fs_parse_uim_get_file_attr_rsp */


#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

