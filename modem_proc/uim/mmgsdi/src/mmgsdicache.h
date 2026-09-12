#ifndef MMGSDICACHE_H
#define MMGSDICACHE_H
/*===========================================================================


           M M G S D I   C A C H E   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2011-2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdicache.h#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
09/16/19   sg      Fallback to file system/cache for missing files
02/25/19   vm      Clear common telecom cache during FCN Refresh
02/22/19   vm      Clear common telecom cache during FULL FCN Refresh
05/07/18   rps     Retrieving attribute cache from caller context
02/10/17   nr      Registering all files for FCN refresh
08/30/16   bcho    Optimization of MMGSDI attribute cache access functions
02/29/16   bcho    Delete attr cache while deleting cache of EPSLOCI and EPSNSC
02/12/16   tkl     Skipped write operation if card data remains unchanged
01/22/16   kv      Send Card and session events based on SFS data in PSM mode
01/20/16   ar      Don't RESET card if EF-DIR req is not processed by UIMDRV
12/28/15   ks      Retry read request for non spec compliant cards
09/09/15   hh      Add support for potential silent file change notification
09/02/15   lxu     Fake the fourth byte of EF AD for buggy card
04/29/15   bcho    Support for App specific cache
04/15/15   tl      Cache dummy UST in case UST is not present on the card
07/28/14   vv      Remove unused function
07/14/14   tl      Remove always ON feature FEATURE_MODEM_RCINIT
06/16/14   av      Delete common attributes cache at card error
04/17/14   av      Enhance MMGSDI attributes cache
04/06/14   am      Added mmgsdi heap alloc clean up logic
04/03/14   am      Fix compile error on SBM due to assert_on_crit_sect_owner
12/16/13   tkl     decouple caching with uim item type
12/16/13   tl      Prevent initalizing the cache with partial files
10/28/13   kb      Add offtarget support to check mutex is owned or not
10/16/13   df      Removing unused function
06/24/13   vdc     Added support for triple SIM
10/21/12   av      Added support for new ECC event
10/13/12   abg     Add utility to delete the contents of the common file cache
09/25/12   av      Skip reading EF-ARR for internal selects
02/18/12   av      Cleaned up remaining ALWAYS ON features
12/21/11   kk      Legacy GSDI removal updates
12/21/11   kk      Added USIM ECC caching support
10/28/11   yt      Initial revision

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/


#include "uim.h"
#include "mmgsdi.h"

/*--------------------------------------------------------------------------
                             Global Variables
--------------------------------------------------------------------------*/
#define MMGSDI_MAX_ECC_RECORDS            255

/*===========================================================================
   ENUM:      MMGSDI_CACHE_SEC_ATTR_OPERATION_ENUM_TYPE
=============================================================================*/
/** Indicates what operation needs to be done for the security attributes
*/
typedef enum {
  MMGSDI_SKIP_SECURITY_ATTRIBUTES,             /**< Indicates no security attributes needed in
                                                    output */
  MMGSDI_DECODE_DEFAULT_ATTRIBUTES,            /**< Indicates default attributes is needed in output */
  MMGSDI_DECODE_ACTUAL_ATTRIBUTES,             /**< Indicates actual decoded attributes is needed 
                                                    in output */
}mmgsdi_cache_sec_attr_operation_enum_type;

/*===========================================================================
   ENUM:      MMGSDI_CACHE_UPDATE_ATTR_FIELD_ENUM_TYPE
=============================================================================*/
/** Indicates which attribute needs to be updated in the file attributes
*/
typedef enum {
  MMGSDI_CACHE_NO_UPDATE,           /**< Indicates no attributes update is needed */
  MMGSDI_CACHE_UPDATE_FILE_SIZE,    /**< Indicates file size attributes update is needed
                                         in output */
  MMGSDI_CACHE_UPDATE_MAX_FIELD,    /**< Max number of fields that can be updated */
}mmgsdi_cache_update_attr_field_enum_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION MMGSDI_CACHE_INIT

DESCRIPTION
  Function called to initialize global variables for MMGSDI cache.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called once during task initialization.
===========================================================================*/
void mmgsdi_cache_init(
  void
);

/* ===========================================================================
   FUNCTION:      MMGSDI_CACHE_READ_LEN

   DESCRIPTION:
     This function return the total len - offset of the cache item

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          Copy Success
     MMGSDI_ERROR:            Unable to Copy
     MMGSDI_INCORRECT_PARAMS: Null input pointer

   SIDE EFFECTS:
     If the same EF is presented in more than 1 cache, it will return the data
     from the first cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read_len(
  mmgsdi_session_id_type         session_id,
  const mmgsdi_access_type      *file_access_ptr,
  mmgsdi_rec_num_type            rec_num,
  mmgsdi_len_type               *data_len_ptr,
  mmgsdi_cache_init_enum_type   *cache_state_ptr
);

/* ===========================================================================
   FUNCTION:      MMGSDI_CACHE_READ_ITEM_LEN

   DESCRIPTION:
     This function return the cache item len

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect_ptr to protect accesses to
     the File cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read_item_len(
  mmgsdi_file_enum_type         mmgsdi_file,
  mmgsdi_session_type_enum_type session_type,
  mmgsdi_rec_num_type           rec_num,
  mmgsdi_len_type              *total_len_ptr,
  mmgsdi_cache_init_enum_type  *cache_state_ptr
);

/* ===========================================================================
   FUNCTION:      MMGSDI_CACHE_WRITE

   DESCRIPTION:
     This function converts the mmgsdi file enum type to uim enum type.
     And write data to different caches based on the applications available
     on the card by calling the corresding util write cache for each
     technology

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          Copy Success
     MMGSDI_ERROR:            Unable to Copy
     MMGSDI_INCORRECT_PARAMS: Null input pointer

   SIDE EFFECTS:
     If the same EF is presented in more than 1 cache, all the caches will be
     updated
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_write(
  mmgsdi_session_id_type      session_id,
  const mmgsdi_access_type   *file_access_ptr,
  mmgsdi_data_type            data,
  mmgsdi_len_type             offset,
  mmgsdi_data_from_enum_type  data_fr_card,
  mmgsdi_rec_num_type         rec_num
);

/* ===========================================================================
   FUNCTION:      MMGSDI_CACHE_WRITE_ITEM

   DESCRIPTION:
     This function writes to the cache

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect_ptr to protect accesses to
     the File cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_write_item(
  mmgsdi_file_enum_type         mmgsdi_file,
  mmgsdi_session_type_enum_type session_type,
  mmgsdi_len_type               data_len,
  mmgsdi_len_type               data_offset,
  const uint8 *                 data_ptr,
  mmgsdi_data_from_enum_type    data_fr_card,
  mmgsdi_rec_num_type           rec_num
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE_ITEM

   DESCRIPTION:
     This function delete specific file from cache. It has
     optional param to provide specific file list to be deleted from app cache
     and file list not to be deleted from app cache. If both lists are present,
     precedence is given to "to be deleted" list.

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect_ptr to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_item(
  mmgsdi_session_id_type        session_id,
  uint16                        num_files,
  const mmgsdi_file_enum_type  *mmgsdi_file_ptr,
  uint16                        num_files_not_to_delete,
  const mmgsdi_file_enum_type  *files_not_to_delete_ptr
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE

   DESCRIPTION:
     This function deletes all the files in app specific file cache. It has
     optional param to provide specific file list to be deleted from app cache

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect_ptr to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete(
  mmgsdi_int_app_info_type    * int_app_info_ptr_ptr,
  int32                         num_files_to_delete,
  const mmgsdi_file_enum_type * files_to_delete_ptr

);

/* ===========================================================================
   FUNCTION:      MMGSDI_CACHE_READ

   DESCRIPTION:
     This function converts the mmgsdi file enum type to uim enum type.
     And read data to different caches based on the applications available
     on the card by calling the corresding util write cache for each
     technology

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          Copy Success
     MMGSDI_ERROR:            Unable to Copy
     MMGSDI_INCORRECT_PARAMS: Null input pointer

   SIDE EFFECTS:
     If the same EF is presented in more than 1 cache, it will return the data
     from the first cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read(
  mmgsdi_session_id_type        session_id,
  const mmgsdi_access_type     *file_access_ptr,
  mmgsdi_data_type             *data_ptr,
  mmgsdi_len_type               offset,
  mmgsdi_rec_num_type           rec_num,
  mmgsdi_cache_init_enum_type  *cache_state_ptr
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_CREATE_PROV_FILE_CACHE_CLEAR_CACHES

   DESCRIPTION:
     If the function is invoked due to a Session activation, this function
     creates the provisioning file cache by allocating the
     exact number of memory space required based on the corresponding
     session type and app type.
     If the provisioning file cache already exists, the file cache is
     cleared and allocated memory freed.
     This function also clears the File attributes cache and the record cache.

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect_ptr to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_create_prov_file_cache_clear_caches(
  mmgsdi_session_id_type        session_id,
  boolean                       session_activate
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE_COMMON_FILE_CACHE

   DESCRIPTION:
     This function deletes all the files in the common file cache for the slot

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_common_file_cache(
  mmgsdi_slot_id_enum_type slot_id
);

/* ===========================================================================
   FUNCTION:      MMGSDI_CACHE_READ_ITEM

   DESCRIPTION:
     This function reads to the cache

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect_ptr to protect accesses to
     the File cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read_item (
  mmgsdi_file_enum_type          mmgsdi_file,
  mmgsdi_session_type_enum_type  session_type,
  mmgsdi_len_type                requested_data_len,
  mmgsdi_len_type                data_offset,
  mmgsdi_rec_num_type            rec_num,
  mmgsdi_len_type *              actual_data_len_ptr,
  uint8  *                       data_ptr,
  mmgsdi_cache_init_enum_type   *cache_state_ptr
);

/* ===========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE_RECORD

   DESCRIPTION:
     This function delete the record cache requested by the caller

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     mmgsdi_return_enum_type

   SIDE EFFECTS:

===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_record(
  mmgsdi_session_id_type session_id,
  mmgsdi_access_type     access,
  mmgsdi_len_type        rec_num
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_PROV_FILE_ATTR_CACHE

DESCRIPTION:
  This function clears the provisioning session specific file attributes cache
  except listed in the input parameters' "Not to delete" list

DEPENDENCIES:
  None

LIMITATIONS:
   None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_prov_file_attr_cache(
  mmgsdi_session_type_enum_type session_type,
  const mmgsdi_file_enum_type  *files_not_to_delete_ptr,
  uint16                        num_files_not_to_delete,
  const mmgsdi_file_enum_type  *files_to_delete_ptr,
  uint16                        num_files_to_delete
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_POPULATE_ATTR_CACHE_IF_NEEDED

DESCRIPTION
  Checks for file attributes cache, if found cache ptr is returned. If not
  found and if it is a file in the cache list and get File attributes is perfomed
  on that EF. Get File attributes is performed only if it is a internal call

  Security parameters will be populated only if the respective pointer is valid.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  If the calling function expects the attributes data, then *cache_attr_pptr
  will hold the attributes and the calling function should take care of freeing
  this ptr along with its member ptrs.

CRITICAL SECTIONS:
  The function uses mmgsdi_cache_crit_sect to protect accesses to
  the File attributes cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_populate_attr_cache_if_needed(
  mmgsdi_session_id_type                     session_id,
  mmgsdi_cache_sec_attr_operation_enum_type  skip_uicc_arr,
  boolean                                    is_select_mandatory,
  const mmgsdi_access_type                  *access_ptr,
  mmgsdi_decoded_attr_cache_type           **cache_attr_pptr,
  mmgsdi_sw_status_enum_type                *sw_present_ptr,
  mmgsdi_file_security_access_type          *security_access_out_ptr
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_FILE_ATTR_CACHE_ITEM

DESCRIPTION:
  This function clears specific file attributes cache item

DEPENDENCIES:
  None

LIMITATIONS:
   None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void mmgsdi_cache_delete_file_attr_cache_item(
  mmgsdi_attr_cache_element_type ** attr_cache_element_pptr
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_INIT_USIM_ECC

DESCRIPTION
  This function determines the number of ECC records and caches all the records
  upto a maximum of MMGSDI_MAX_ECC_RECORDS (255) records.
  An ECC event is also sent out.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_init_usim_ecc(
  mmgsdi_session_id_type       session_id
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_GET_ECC_FILE_DATA

DESCRIPTION
 This function copies whole of the ECC data from cache if available.
 The function allocates memory for the ecc data. It is caller's
 responsibility to free the data ptr.
 For RUIM/CSIM/GSM ECC files, the record length returned in ecc_rec_len_ptr
 is zero.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_get_ecc_file_data(
  mmgsdi_session_id_type         session_id,
  mmgsdi_file_enum_type          file_enum,
  mmgsdi_data_type               *ecc_data_ptr,
  mmgsdi_len_type                *ecc_rec_len_ptr
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_ATTR_READ

DESCRIPTION
  Checks for file attributes cache, if found cache ptr is returned in the
  function argument. In the SUCCESS case, *cache_attr_pptr will hold the
  attributes and the calling function should take care of freeing this ptr
  along with its member ptrs.

  Raw data or security parameters or both will be populated only if the
  respective pointers are valid.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  None

CRITICAL SECTIONS:
  The function uses mmgsdi_cache_crit_sect to protect accesses to
  the File attributes cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_attr_read(
  mmgsdi_session_id_type                       session_id,
  const mmgsdi_access_type                    *access_ptr,
  mmgsdi_decoded_attr_cache_type             **cache_attr_pptr,
  mmgsdi_cache_sec_attr_operation_enum_type    sec_attr_oper,
  mmgsdi_file_security_access_type            *security_access_out_ptr,
  mmgsdi_len_type                             *raw_data_len_ptr,
  uint8                                      **raw_data_pptr
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_ATTR_WRITE

DESCRIPTION
  Checks for file attributes cache for a file. If the attributes are not
  cached already, it writes the attribtues to the attribtues cache

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  None

CRITICAL SECTIONS:
  The function uses mmgsdi_cache_crit_sect to protect accesses to
  the File attributes cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_attr_write(
  mmgsdi_session_id_type                   session_id,
  const mmgsdi_access_type               * access_ptr,
  const mmgsdi_file_security_access_type * security_access_ptr,
  boolean                                  is_arr_skipped,
  uint16                                   arr_file_id,
  uint8                                    arr_record_no,
  mmgsdi_get_file_attr_data_type           raw_file_attr_data
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_PKCS15_ATTR_CACHE

DESCRIPTION
 This function deletes all file attributes cached under the PKCS15 DF 7F50.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_cache_delete_pkcs15_attr_cache (
  mmgsdi_slot_id_enum_type  slot_id
);

/*================================================================
FUNCTION  MMGSDI_CACHE_CREATE_APP_CACHE_TABLE

DESCRIPTION:
  Populate app cache table pointer along with CPHS cache pointer

INPUT PARAMETERS:
  None

DEPENDENCIES:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None
===============================================================*/
mmgsdi_return_enum_type mmgsdi_cache_create_app_cache_table(
  mmgsdi_int_app_info_type *app_info_ptr
);

/*===========================================================================
FUNCTION:    MMGSDI_CACHE_DELETE_ATTR_SELECTIVE_ITEMS

DESCRIPTION:
  This function clears file attributes cache.
  It has optional param to provide specific file list to be deleted from attr
  cache and file list not to be deleted from attr cache. If both lists are
  present, only "to be deleted" list will be used. If these optional lists are
  not present, MMGSDI will delete all cached attr from cache.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  This function acquires task state critical section while accessing cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_attr_selective_items(
  mmgsdi_attr_cache_type       *attr_cache_table_ptr,
  const mmgsdi_file_enum_type  *files_not_to_delete_ptr,
  uint16                        num_files_not_to_delete,
  const mmgsdi_file_enum_type  *files_to_delete_ptr,
  uint16                        num_files_to_delete
);

/*===========================================================================
FUNCTION:    MMGSDI_CACHE_DELETE_ATTR_CACHE_BY_PATH

DESCRIPTION:
  This function clears file attributes cache for particular file by path.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  This function acquires task state critical section while accessing cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_attr_cache_by_path
(
  mmgsdi_attr_cache_type       *attr_cache_table_ptr,
  mmgsdi_path_type              file_path
);

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================
FUNCTION MMGSDI_CACHE_ALLOC_AND_POPULATE_FILE_ATTR_CACHE

INPUT
  mmgsdi_file_security_type  security_status_input
  mmgsdi_file_security_type *security_status_output_ptr

OUTPUT
  void

DESCRIPTION

RETURN VALUE
  void

CRITICAL SECTIONS:
   The calling function must enter mmgsdi_cache_crit_sect crit
   sect when security_status_output_ptr is from attributes cache
   before invoking this util function. Protection for attributes cache.
===========================================================================*/
void mmgsdi_cache_alloc_and_populate_file_attr_cache(
  mmgsdi_file_security_type  security_status_input,
  mmgsdi_file_security_type *security_status_output_ptr
);

/*===========================================================================
  FUNCTION MMGSDI_CACHE_IS_WRITE_DATA_SAME_AS_CACHE

DESCRIPTION
  This utility function check if input write request data is the same
  as data stored in mmgsdi cache is in synch with card data

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean mmgsdi_cache_is_write_data_same_as_cache(
  mmgsdi_session_id_type         session_id,
  const mmgsdi_access_type      *file_access_ptr,
  mmgsdi_data_type               write_data,
  mmgsdi_len_type                offset,
  mmgsdi_rec_num_type            rec_num
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_CHECK_FILE_IS_IN_CACHE_LIST_OR_NOT

   DESCRIPTION:
     This function checks the passed file enum is present in MMGSDI cache
     list or not.

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     boolean
     TRUE    File is present in cache list
     FALSE   File is not present in cache list

   SIDE EFFECTS:
     None
==========================================================================*/
boolean mmgsdi_cache_check_file_is_in_cache_list_or_not(
  mmgsdi_file_enum_type          file_enum,
  mmgsdi_session_type_enum_type  session_type
);


/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_CHECK_AND_WRITE_DEFAULT_ATTR

DESCRIPTION:
  This function checks if the EF attributes are cached if not
  then write default attributes to cache

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type:
    MMGSDI_SUCCESS:  If the element is created successfully
    MMGSDI_ERROR:    If the memory allocation failed

SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_check_and_write_default_attr(
  mmgsdi_session_id_type                   session_id,
  const mmgsdi_access_type                 access,
  mmgsdi_file_enum_type                    file_enum,
  mmgsdi_file_structure_enum_type          file_type,
  mmgsdi_get_file_attr_data_type           raw_attr_data
);


/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_UPDATE_FILE_ATTRIBUTES

DESCRIPTION:
  This function updates the passed attrbute's field with the value passed in
  argument.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_update_file_attributes(
  mmgsdi_session_id_type                    session_id,
  mmgsdi_cache_update_attr_field_enum_type  update_field,
  uint16                                    updated_length,
  mmgsdi_get_file_attr_data_type           *raw_attr_ptr
);

/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_COMMON_FILE_ATTR_CACHE

DESCRIPTION:
  This function clears the common files attributes cache

DEPENDENCIES:
  None

LIMITATIONS:
   None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_common_file_attr_cache(
  mmgsdi_slot_id_enum_type slot_id,
  boolean                  telecom_only);

/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_GET_COMMON_ATTR_CACHE_PTR

DESCRIPTION:
  This function returns common attr cache table ptr.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_attr_cache_type*

SIDE EFFECTS:
  None
==========================================================================*/
mmgsdi_attr_cache_type* mmgsdi_cache_common_attr_cache_ptr(
  mmgsdi_slot_id_enum_type slot_id
);

#endif /* MMGSDICACHE_H */
