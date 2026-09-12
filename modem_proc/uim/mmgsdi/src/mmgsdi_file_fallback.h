/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            M M G S D I   F I L E  F A L L B A C K   H E A D E R S


GENERAL DESCRIPTION

  This header file contains the processing of all MMGSDI functions 
  related to file fallback to cache or File System when file is missing in the card
 
EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS

                        COPYRIGHT INFORMATION

Copyright (c) 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_file_fallback.h#1 $$ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/16/19   sg      Initial Revision

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/*===========================================================================
   ENUM:      MMGSDI_FILE_INFO_TABLE_ENTRY_CATEG_ENUM_TYPE
=============================================================================*/

typedef enum {
  MMGSDI_ENTRY_CATEG_CACHE_FALLBACK        = 0x00, /**< File fallback to cache 
                                                        when missing on card. */
  MMGSDI_ENTRY_CATEG_FS_FALLBACK,                  /**< File fallback to file 
                                                        system when missing on card. */
  MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK,         /**< File fallback to cache 
                                                        or File system supported 
                                                        when file is missing on card. 
                                                        Priority is for fallback to 
                                                        File System and if not supported, 
                                                        then fallback is done to cache. */
}mmgsdi_file_info_table_entry_categ_enum_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*=============================================================================
FUNCTION     MMGSDI_FILE_FALLBACK_SET_DEFAULT_SEC_ATTR_FROM_FILE_INFO_TABLE

DESCRIPTION
  This function retrieves the default security attributes from the File Info table
  corresponding to the file requested

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_file_fallback_set_default_sec_attr_from_file_info_table(
  mmgsdi_file_enum_type              mmgsdi_file,
  mmgsdi_file_security_access_type * operation_ptr
);

/*===========================================================================
FUNCTION        MMGSDI_FILE_FALLBACK_PARSE_UIM_GET_FILE_ATTR_RSP

DESCRIPTION:
  This function will parse the UIM response to Get file atr. request
  and provide content from hard-coded cache if the file being requested
  is supported in cache or file system fallback

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_file_fallback_parse_uim_get_file_attr_rsp (
  mmgsdi_uim_report_rsp_type              *mod_uim_rsp_ptr,
  const mmgsdi_request_header_type        *req_header_ptr,
  const mmgsdi_client_req_extra_info_type *extra_param_ptr,
  mmgsdi_return_enum_type                  report_status
);

/* ==========================================================================
FUNCTION:      MMGSDI_FILE_FALLBACK_GET_FILE_INFO

DESCRIPTION:
  This function gets the file length and file values from File Info table

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type:
    MMGSDI_SUCCESS:  If file is found in the table 
    MMGSDI_NOT_FOUND: If file is not found in the table 

SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fallback_get_file_info(
  mmgsdi_file_enum_type                             file,
  uint8                                            *file_length_ptr,
  const uint8                                     **default_value_pptr,
  mmgsdi_file_info_table_entry_categ_enum_type     *table_entry_categ_ptr,
  mmgsdi_file_structure_enum_type                  *file_type,
  mmgsdi_len_type                                  *file_attr_length_ptr,
  uint8                                           **file_attr_pptr
);

/* ==========================================================================
FUNCTION:      MMGSDI_FILE_FALLBACK_CHECK_CACHE_DELETE_ALLOWED

DESCRIPTION:
  This function checks if file cache can be deleted for the file.
  It is not allowed to be deleted for files for whom fallback to cache is enabled.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  boolean
 
SIDE EFFECTS:

==========================================================================*/
boolean mmgsdi_file_fallback_check_cache_delete_allowed(
  mmgsdi_file_enum_type           file
);

/* ============================================================================
FUNCTION MMGSDI_FILE_SET_DEFAULT_SEC_ATTR_PER_SPEC

DESCRIPTION
  This function sets the default security attributes for various file
  access operations DERIVED FROM the access conditions mentioned in the spec
  for the file. For the files for which the access condition is ADM or NEVER
  (per the spec), the access condition is defaulted to MMGSDI_NEVER_ALLOWED.
  For all other access conditions mentioned in the spec for a file, the access
  condition is defaulted to MMGSDI_ALWAYS_ALLOWED.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_file_set_default_sec_attr_per_spec(
  mmgsdi_file_security_attributes_type *attr_ptr,
  mmgsdi_file_structure_enum_type       mmgsdi_file_type,
  mmgsdi_file_enum_type                 mmgsdi_file
);

