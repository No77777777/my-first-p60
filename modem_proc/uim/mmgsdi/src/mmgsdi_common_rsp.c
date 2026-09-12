/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                M M G S D I   I C C / U I C C  F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the response processing support for MMGSDI.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2013 - 2017, 2019, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_common_rsp.c#3 $$ $DateTime: 2021/09/08 12:21:44 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/09/21   pvb     Delete attributes from card for File fallback supported file
09/24/19   sg      Fallback to system/cache for EF INCONSISTENT error scenario
09/20/19   sg      Incorrect QMI Error when reading EF-ACC with PIN1 enabled 
09/16/19   sg      Fallback to file system/cache for missing files
07/07/17   me      Support to disable GBA_U service in ISIM 
05/24/17   bcho    Even if EF-OPL caching fails, continue EF-PNN caching
04/04/17   me      Don�t update cache state to NOT FOUND when fallback to cache is supported
03/29/17   nr      Add support to cache PIN1 of the SIM card when PSM mode enabled
01/16/17   shr     Add support for SIM file fallback to File System
06/22/16   ar      Don't update the offset if read_len is less than file size
06/19/16   bcho    F3 frequency reduction by conditional logic
06/13/16   ar      Moving compute_ip_auth handler to 3GPP2 specific file
05/30/16   ar      Removing string prints from APDU and AUTH
05/20/16   vdc     Remove F3 messages for memory allocation failure
05/13/16   nr      Skip Search Req for PB files
05/03/16   tkl     F3 log prints cleanup
04/26/16   ar      Removing the mmgsdi_cnf_map_req_to_cnf_enum_type()
04/19/16   ar      Correcting the index to MMGSDI_MAX_CLIENT_REQ_INFO
04/13/16   ar      CDMA/HDR support on 2nd SUB
03/11/16   tkl     Fix read/write of cache files when cache is not_init
12/28/15   ks      Retry read request for non spec compliant cards
12/07/15   sp      Remove unused variables
10/26/15   nr      Disable MMGSDI PB cache for PBM module
10/16/15   sp      Move client_req_table_info index variables to unsigned
10/05/15   sp      Reduce signed/unsigned conversions in UIM
08/31/15   yt      Remove length information from requests and confirmations
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
07/29/15   sp      Remove UIM command in client request table
07/02/15   ar      Correct the caching logic of EF-PRL and EF-EPRL
06/30/15   vdc     Cache complete data for req's with partial read/write data
04/29/15   bcho    Support for App specific cache
04/06/15   ar      Remove duplicate code
02/25/15   tl      Remove ICC seek support
12/09/14   ar      Replace mmgsdi_u/icc_rsp_srv_available with common func
10/16/14   tl      Set response type before building cnf data for sync resp
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
05/12/14   bcho    NULL check added before dereferencing extra_param_ptr
12/05/13   tkl     SFI Support and decouple uim file enums
01/21/14   yt      Remove duplicate code
01/11/14   df      Fix off-target compilation error
01/10/14   df      Use v2.0 diag macros
01/09/14   ar      Replace old Macros with MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE
12/16/13   tl      Prevent initalizing the cache with partial files
10/02/13   yt      Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "intconv.h"
#include "mmgsdi_icc.h"
#include "mmgsdi_uim_icc.h"
#include "mmgsdi.h"
#include "uim.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "mmgsdi_card_init.h"
#include "mmgsdi_icc_rsp.h"
#include "mmgsdi_uicc_rsp.h"
#include "mmgsdi_evt.h"
#include "mmgsdi_nv.h"
#include "mmgsdicache.h"
#include "mmgsdi_eons.h"
#include "mmgsdi_cnf.h"
#include "mmgsdi_file.h"
#include "mmgsdi_pb_cache.h"
#include "mmgsdi_refresh.h"
#include "mmgsdi_uim_uicc.h"
#include "mmgsdi_file_fs.h"
#include "mmgsdi_common_rsp.h"
#include "mmgsdi_hidden_key.h"
#include "mmgsdi_file_fallback.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* GBA service for ISIM */
#define MMGSDI_ISIM_SRV_GBA   (0x02)


/*===========================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_INCREASE

   DESCRIPTION:
     This function will build the response to INCREASE request

   DEPENDENCIES:


   LIMITATIONS:
     For UICC protocol access, mmgsdi_icc_rsp_increase should be used

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_increase (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_increase_cnf_type          ** mmgsdi_cnf_pptr,
  boolean                              synch,
  mmgsdi_protocol_enum_type            protocol
)
{
  mmgsdi_request_header_type            req_header;
  uint32                                index                 = 0;
  mmgsdi_return_enum_type               mmgsdi_status         = MMGSDI_SUCCESS;
  mmgsdi_increase_cnf_type            * cnf_ptr               = NULL;
  mmgsdi_client_req_extra_info_type   * extra_param_ptr       = NULL;
  mmgsdi_return_enum_type               report_status         = MMGSDI_SUCCESS;
  boolean                               sw_present            = FALSE;
  mmgsdi_data_type                      inc_rsp_data;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);
  if (synch)
  {
    MMGSDIUTIL_RETURN_IF_NULL(mmgsdi_cnf_pptr);
  }

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  memset(&inc_rsp_data, 0x00, sizeof(mmgsdi_data_type));

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    1) Parse the response status returned from UIM
    2) If success:
         update record number accessed in confirmation type
  ---------------------------------------------------------------------------*/
  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  MMGSDIUTIL_RETURN_IF_NULL(extra_param_ptr);
  if (report_status == MMGSDI_SUCCESS)
  {
    /* Increased file needs to be cached as the increased data is placed
       in record 1 of the cyclic file. Trim the response data to cache only
       the record contents.*/
    if(extra_param_ptr->increase_data.data.data_len < uim_rsp_ptr->rsp_data.data_len)
    {
      inc_rsp_data.data_len = extra_param_ptr->increase_data.data.data_len;
    }
    else
    {
      inc_rsp_data.data_len = uim_rsp_ptr->rsp_data.data_len;
    }

    if(uim_rsp_ptr->rsp_data.data_ptr &&
       inc_rsp_data.data_len > 0)
    {
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(inc_rsp_data.data_ptr,
                                         inc_rsp_data.data_len);

      /* In case of failed memory allocation, clear cache to avoid stale data */
      if(inc_rsp_data.data_ptr == NULL)
      {
        if(extra_param_ptr->increase_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)
        {
          (void)mmgsdi_cache_delete_item(req_header.session_id,
                                         1,
                                         &extra_param_ptr->increase_data.access.file.file_enum,
                                         0,
                                         NULL);
        }
        else
        {
          UIM_MSG_HIGH_1("failed to delete cache item with access method: %x",
                         extra_param_ptr->increase_data.access.access_method);
        }
      }
      else
      {
        mmgsdi_data_from_enum_type data_from = MMGSDI_DATA_FROM_CARD_INCOMPLETE;

        if(extra_param_ptr->increase_data.data.data_len == 0)
        {
          data_from = MMGSDI_DATA_FROM_CARD_COMPLETE;
        }

        mmgsdi_memscpy(inc_rsp_data.data_ptr,
                       inc_rsp_data.data_len,
                       uim_rsp_ptr->rsp_data.data_ptr,
                       inc_rsp_data.data_len);

        mmgsdi_status = mmgsdi_cache_write(req_header.session_id,
                                           &extra_param_ptr->increase_data.access,
                                           inc_rsp_data,
                                           0,
                                           data_from,
                                           0);

        MMGSDIUTIL_TMC_MEM_FREE(inc_rsp_data.data_ptr);
      }
    }
  }
  else
  {
    /* Unsuccessful processing of command */
    if (extra_param_ptr->increase_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)
    {
      if(report_status == MMGSDI_NOT_FOUND)
      {
        /* Update cache to indicate that file was not found on card */
        (void)mmgsdi_cache_write(req_header.session_id,
                                 &extra_param_ptr->increase_data.access,
                                 uim_rsp_ptr->rsp_data,
                                 0,
                                 MMGSDI_DATA_NOT_FOUND,
                                 0);
      }
    }
  } /* Parsing UIM Status */

  if (report_status == MMGSDI_SUCCESS)
  {
    /* if read from cache, and the operation is successful,
       the cmd_transacted will be FALSE.  if the operation is not successful,
       the command will go to uim */
    sw_present = TRUE;
  }
  else
  {
    sw_present = uim_rsp_ptr->cmd_transacted;
  }

  if (!synch)
  {
    mmgsdi_status = mmgsdi_cnf_build_and_queue(
      report_status, &req_header, extra_param_ptr,
      (void*)&uim_rsp_ptr->rsp_data,
      sw_present,
      uim_rsp_ptr->status_word);
  }
  else
  {
    /* Obtain memory for confirmation data */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE((*mmgsdi_cnf_pptr),
      sizeof(mmgsdi_increase_cnf_type));
    if ((*mmgsdi_cnf_pptr) == NULL)
    {
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    cnf_ptr = *mmgsdi_cnf_pptr;

    mmgsdi_status = mmgsdi_cnf_build_increase_data(
      cnf_ptr,
      report_status,
      extra_param_ptr,
      (mmgsdi_data_type*)&uim_rsp_ptr->rsp_data);

    if (mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* Populate Header */
      /* Header population should be done AFTER data populate
        because any pointer length will be assigned in the
        individual confirmation population and used in the
        populate_cnf_rsp_header for assigning the response
        length */
      mmgsdi_util_populate_cnf_rsp_header(&cnf_ptr->response_header,
                                          &req_header,
                                          MMGSDI_INCREASE_REQ,
                                          sw_present,
                                          uim_rsp_ptr->status_word);

      cnf_ptr->response_header.mmgsdi_status = report_status;
    }
  }

  return mmgsdi_status;
} /* mmgsdi_common_rsp_increase */


/*=============================================================================
FUNCTION:      MMGSDI_COMMON_RSP_UPDATE_EF_DATA

DESCRIPTION
  This function supports fallback to cache (with default values)  
  when the file is missing on the SIM card. This is supported for the following EFs:
  - USIM EF ACC
  - USIM EF AD
  - USIM EF UST
  - USIM EF FPLMN
  - USIM EF KEYS
  - USIM EF KEYSPS
  - GSM EF AD
  - GSM EF ACC
  - GSM EF SST
  - GSM EF KC

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_common_rsp_update_ef_data (
  mmgsdi_return_enum_type             report_status,
  mmgsdi_session_id_type              session_id,
  mmgsdi_access_type                  file_access,
  mmgsdi_uim_report_rsp_type         *modified_uim_rsp_ptr
)
{
  mmgsdi_int_app_info_type                      *app_info_ptr        = NULL;
  mmgsdi_data_type                               write_data;
  mmgsdi_return_enum_type                        mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_file_attributes_type                    file_attr;
  mmgsdi_rec_num_type                            num_of_rec          = 0;
  mmgsdi_len_type                                rec_len             = 0;
  uint8                                          file_length         = 0;
  const uint8                                   *default_values_ptr  = NULL;
  mmgsdi_file_structure_enum_type                file_type           = MMGSDI_MAX_FILE_STRUCTURE_ENUM;
  mmgsdi_file_info_table_entry_categ_enum_type   table_entry_categ   = MMGSDI_ENTRY_CATEG_CACHE_FALLBACK;
  mmgsdi_pin_status_enum_type                    pin1_status         = MMGSDI_PIN_STATUS_NOT_INITIALIZED;
  mmgsdi_get_file_attr_data_type                 raw_attr_data;

  if (modified_uim_rsp_ptr == NULL)
  {
    return report_status;
  }

  memset(&file_attr, 0x00, sizeof(file_attr));
  memset(&write_data, 0x00, sizeof(write_data));
  memset(&raw_attr_data, 0x00, sizeof(raw_attr_data));

  if (file_access.access_method != MMGSDI_EF_ENUM_ACCESS)
  {
    return report_status;
  }

  /* Get app. info to obtain pin status */
  if (MMGSDI_SUCCESS != mmgsdi_util_get_session_app_info(session_id,
                                                         NULL,
                                                         NULL,
                                                         &app_info_ptr,
                                                         NULL,
                                                         NULL,
                                                         NULL) || app_info_ptr == NULL)
  {
    return report_status;
  }

  if (app_info_ptr->pin1_ptr != NULL)
  {
    pin1_status = app_info_ptr->pin1_ptr->status;
  }

  if (report_status != MMGSDI_NOT_FOUND && report_status != MMGSDI_ACCESS_DENIED &&
      report_status != MMGSDI_EF_INCONSISTENT)
  {
    return report_status;
  }

  /* Do not allow fallback to cache for buggy cards with incorrect access denial */

  if(report_status == MMGSDI_ACCESS_DENIED && 
     (pin1_status != MMGSDI_PIN_DISABLED && pin1_status != MMGSDI_PIN_ENABLED_VERIFIED))
  {
    return report_status;
  }

  /* Get file default length and values from mmgsdi_file_info_table */
  mmgsdi_status = mmgsdi_file_fallback_get_file_info(file_access.file.file_enum,
                                                     &file_length,
                                                     &default_values_ptr, 
                                                     &table_entry_categ, 
                                                     &file_type,
                                                     &raw_attr_data.data_len,
                                                     &raw_attr_data.data_ptr);

  if (mmgsdi_status !=  MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(raw_attr_data.data_ptr);
    return report_status;
  }

  /* When fallback to file system is supported for the file and the
     corresponding feature is enabled, return. */
  if ((table_entry_categ == MMGSDI_ENTRY_CATEG_FS_FALLBACK || 
       table_entry_categ == MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK) && 
       mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS) == 
       MMGSDI_FEATURE_ENABLED)
  {
    MMGSDIUTIL_TMC_MEM_FREE(raw_attr_data.data_ptr);
    return report_status;
  }

  /* If file is not supported w.r.t. fallback to cache, return */
  if (table_entry_categ != MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK &&
      table_entry_categ != MMGSDI_ENTRY_CATEG_CACHE_FALLBACK)
  {
    MMGSDIUTIL_TMC_MEM_FREE(raw_attr_data.data_ptr);
    return report_status;
  }

  write_data.data_len = file_length;

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(write_data.data_ptr,
                                     write_data.data_len);
  if (write_data.data_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(raw_attr_data.data_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  mmgsdi_memscpy(write_data.data_ptr,
                 write_data.data_len,
                 default_values_ptr,
                 file_length);

  file_attr.file_type = file_type;

  /* Fallback support only 1 record for the record based files */
  if (file_attr.file_type == MMGSDI_LINEAR_FIXED_FILE || 
      file_attr.file_type == MMGSDI_CYCLIC_FILE)
  {
    rec_len = file_length;
    num_of_rec = 1;
  }

  /* The current function mmgsdi_common_rsp_update_ef_data function will be called,
     When CARD returned NOT_FOUND or ACCESS_DENIED or EF_INCONSISTENT for a read operation.
     As MMGSDI is supporting File fallback mechanism for the file,
     MMGSDI should delete atributes of the file which MMGSDI got from card previously */
  (void)mmgsdi_cache_delete_attr_selective_items(&(app_info_ptr->attr_cache_tbl),
                                                   NULL,
                                                   0,
                                                   &file_access.file.file_enum,
                                                   1);

  /* Read the attributes from the cache, if it not success then 
     Write the attributes fetched from the static tables to the file attributes cache */
  mmgsdi_status = mmgsdi_cache_check_and_write_default_attr(session_id,
                                                            file_access,
                                                            file_access.file.file_enum,
                                                            file_type,
                                                            raw_attr_data);

  MMGSDIUTIL_TMC_MEM_FREE(raw_attr_data.data_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS) 
  {
    MMGSDIUTIL_TMC_MEM_FREE(write_data.data_ptr);
    return mmgsdi_status;
  }

  /* Check to see if the security attributes allow to read/write file */
  mmgsdi_status = mmgsdi_util_check_file_attr(session_id,
                                               app_info_ptr,
                                               &file_access,
                                               MMGSDI_READ_REQ,
                                               MMGSDI_DECODE_ACTUAL_ATTRIBUTES);
  /* When fallback support is present for the file then return ACCESS_DENIED,
  so that cache status is NOT updated to NOT_FOUND. */
  if (MMGSDI_SUCCESS != mmgsdi_status)
  {
    MMGSDIUTIL_TMC_MEM_FREE(write_data.data_ptr);
    if(mmgsdi_status == MMGSDI_ACCESS_DENIED)
    {
      return mmgsdi_status;
    }
    else
    {
      return report_status;
    }
  }

  modified_uim_rsp_ptr->cmd_transacted = TRUE;
  modified_uim_rsp_ptr->is_uim_success = TRUE;
  modified_uim_rsp_ptr->mmgsdi_error_status = MMGSDI_SUCCESS;
  modified_uim_rsp_ptr->status_word.sw1 = GSDI_SIM_SW1_NORM_END;
  modified_uim_rsp_ptr->status_word.sw2 = GSDI_SIM_SW2_NORM_END;

  MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr); 
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(modified_uim_rsp_ptr->rsp_data.data_ptr, write_data.data_len);
  modified_uim_rsp_ptr->rsp_data.data_len = write_data.data_len;

  /* Update the EF data in the response */
  if (modified_uim_rsp_ptr->rsp_data.data_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(write_data.data_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  mmgsdi_memscpy(modified_uim_rsp_ptr->rsp_data.data_ptr,
                 write_data.data_len,
                 write_data.data_ptr,
                 write_data.data_len);

  MMGSDIUTIL_TMC_MEM_FREE(write_data.data_ptr);
  return MMGSDI_SUCCESS;
} /* mmgsdi_common_rsp_update_ef_data */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_COMMON_RSP_READ

   DESCRIPTION:
     This function will build the response to Read request
     for both ICC/UICC protocol

   DEPENDENCIES:


   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

-------------------------------------------------------------------------------*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_read (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_read_cnf_type              ** mmgsdi_cnf_pptr,
  boolean                              synch,
  mmgsdi_protocol_enum_type            protocol
)
{
  mmgsdi_request_header_type            req_header;
  uint32                                index                 = 0;
  mmgsdi_return_enum_type               mmgsdi_status         = MMGSDI_SUCCESS;
  mmgsdi_read_cnf_type                * cnf_ptr               = NULL;
  mmgsdi_client_req_extra_info_type   * extra_param_ptr       = NULL;
  mmgsdi_return_enum_type               report_status         = MMGSDI_SUCCESS;
  boolean                               sw_present            = FALSE;
  mmgsdi_data_type                      rsp_data_buf;
  mmgsdi_data_type                      cnf_data_buf;
  mmgsdi_nv_context_type                efs_index             = MMGSDI_NV_CONTEXT_INVALID;
  mmgsdi_file_sfi_table_type            file_info;
  mmgsdi_offset_type                    mmgsdi_offset         = 0;
  boolean                               is_cache_init_read    = FALSE;
  mmgsdi_uim_report_rsp_type          * modified_uim_rsp_ptr  = NULL;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);
  if (synch)
  {
    MMGSDIUTIL_RETURN_IF_NULL(mmgsdi_cnf_pptr);
  }

  memset(&file_info, 0x00, sizeof(file_info));
  memset(&rsp_data_buf, 0x00, sizeof(mmgsdi_data_type));
  memset(&cnf_data_buf, 0x00, sizeof(mmgsdi_data_type));

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol != MMGSDI_UICC && protocol != MMGSDI_ICC)
  {
    return MMGSDI_ERROR;
  }

  MMGSDIUTIL_RETURN_IF_NULL(extra_param_ptr);

  /* Parse the response status returned from UIM */
  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  /* Copy the UIM Response data */
  mmgsdi_status = mmgsdi_util_alloc_copy_uim_rsp(&modified_uim_rsp_ptr,
                                                 uim_rsp_ptr);
  
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

 /* if file is missing from the card or access is denied even if PIN status is valid for access or scenario is EF-INCONSISTENT error then attempt to read the default */
  if (report_status == MMGSDI_NOT_FOUND ||
      report_status == MMGSDI_ACCESS_DENIED ||
      report_status == MMGSDI_EF_INCONSISTENT)
  {
    report_status = mmgsdi_common_rsp_update_ef_data(report_status,
                                                     req_header.session_id,
                                                     extra_param_ptr->read_data.access, 
                                                     modified_uim_rsp_ptr);
  }

  /* If fallback to cache fails, then fallback to FS is attempted. */
  if (report_status == MMGSDI_NOT_FOUND ||
      report_status == MMGSDI_ACCESS_DENIED ||
      report_status == MMGSDI_EF_INCONSISTENT)
  {
    report_status = mmgsdi_file_fs_parse_uim_read_rsp(
                    modified_uim_rsp_ptr,
                    &req_header,
                    extra_param_ptr,
                    report_status);
  }

  mmgsdi_offset   = extra_param_ptr->read_data.offset;

  if (modified_uim_rsp_ptr->rsp_data.data_len > 0 &&
      modified_uim_rsp_ptr->rsp_data.data_ptr)
  {
    /* copy rsp_data from uim_rsp_ptr so it can be modified by
       TMSI PLMN check during util write cache operation*/
    rsp_data_buf.data_len = modified_uim_rsp_ptr->rsp_data.data_len;

    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(rsp_data_buf.data_ptr,
                                       rsp_data_buf.data_len);

    if (rsp_data_buf.data_ptr != NULL)
    {
      mmgsdi_memscpy(rsp_data_buf.data_ptr,
                     rsp_data_buf.data_len,
                     modified_uim_rsp_ptr->rsp_data.data_ptr,
                     rsp_data_buf.data_len);
    }
    else
    {
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr); 
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(rsp_data_buf.data_ptr);
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
  }

  if (report_status == MMGSDI_SUCCESS)
  {
    if(extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)
    {
      if ( extra_param_ptr->read_data.access.file.file_enum == MMGSDI_NO_FILE_ENUM ||
           extra_param_ptr->read_data.access.file.file_enum == MMGSDI_MAX_FILE_ENUM ||
           extra_param_ptr->read_data.access.file.file_enum == MMGSDI_EF_PATH_START)
      {
        UIM_MSG_LOW_1("Can not convert to uim enum 0x%x",
                      extra_param_ptr->read_data.access.file.file_enum);
      }
      else
      {
        /*Check the FCN List in case additional files are being read during refresh*/
        mmgsdi_util_check_client_fcn_list(req_header.session_id,
                                          extra_param_ptr->read_data.access.file.file_enum);
      }
    }

    /* write to cache. If it is record based file,
       update only if it is 1st rec */
    if((extra_param_ptr->read_data.file_type == MMGSDI_LINEAR_FIXED_FILE)&&
       (extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)&&
       ((protocol == MMGSDI_UICC &&
         ((extra_param_ptr->read_data.access.file.file_enum == MMGSDI_USIM_OPL)||
          (extra_param_ptr->read_data.access.file.file_enum == MMGSDI_USIM_PNN)))
        ||
        (protocol == MMGSDI_ICC &&
         ((extra_param_ptr->read_data.access.file.file_enum == MMGSDI_GSM_OPL)||
          (extra_param_ptr->read_data.access.file.file_enum == MMGSDI_GSM_PNN)))))
    {
      mmgsdi_status = mmgsdi_eons_opl_pnn_update_record(
                        req_header.session_id,
                        extra_param_ptr->read_data.access.file.file_enum,
                        rsp_data_buf,
                        (mmgsdi_rec_num_type)extra_param_ptr->read_data.rec_num,
                        MMGSDI_CACHE_MAX_ENUM);
    }
    /* Update the cache content if the EF is under Phonebook directory.
       Add record to the file cache list */
    else if(mmgsdi_util_is_ef_under_phonebook(&extra_param_ptr->read_data.access) &&
            !mmgsdi_util_is_ef_pbr(&extra_param_ptr->read_data.access))
    {
      uint32     client_index = 0;

      mmgsdi_status = mmgsdi_util_get_client_id_index(req_header.client_id,
                                                      &client_index);

      /* Disable MMGSDI PB cache from client PBM to avoid unnecessary additional SEARCH commands
         performed by MMGSDI completely. Also restricting usage of MMGSDI PB cache only to external
         clients (Android phonebook).*/
      if(mmgsdi_status == MMGSDI_SUCCESS && !MMGSDIUTIL_IS_PBM_CLIENT(client_index))
      {
        /* Proceed with cache of other PB records.*/
        mmgsdi_status = mmgsdi_pb_cache_records(&extra_param_ptr->read_data.access,
                                                req_header.slot_id,
                                                extra_param_ptr->read_data.rec_num,
                                                req_header.session_id);
      }
    }
    else if (((mmgsdi_util_get_efs_item_index(
                 req_header.session_id, NULL, &efs_index) == MMGSDI_SUCCESS) &&
               (mmgsdi_nv_get_external_feature_status(
                  MMGSDI_EXT_FEAT_NAS_CFG_FEATURE_CSG, efs_index) == MMGSDI_FEATURE_ENABLED)) ||
               (extra_param_ptr->read_data.access.file.file_enum != MMGSDI_USIM_ACSGL &&
                extra_param_ptr->read_data.access.file.file_enum != MMGSDI_USIM_OCSGL &&
                extra_param_ptr->read_data.access.file.file_enum != MMGSDI_USIM_CSGT &&
                extra_param_ptr->read_data.access.file.file_enum != MMGSDI_USIM_OCSGT &&
                extra_param_ptr->read_data.access.file.file_enum != MMGSDI_USIM_HNBN &&
                extra_param_ptr->read_data.access.file.file_enum != MMGSDI_USIM_OHNBN))
    {
      mmgsdi_data_from_enum_type data_from = MMGSDI_DATA_FROM_CARD_INCOMPLETE;

      if(extra_param_ptr->read_data.data_len == 0 &&
         extra_param_ptr->read_data.offset == 0)
      {
        data_from = MMGSDI_DATA_FROM_CARD_COMPLETE;
      }
      /* In case of a PRL file, if the length is greater than 2, it means the
         response contains the PRL size bytes + data. Hence, mark it as
         DATA_FROM_CARD_COMPLETE so that it gets written to the cache. */
      else if ((protocol == MMGSDI_UICC &&
                (extra_param_ptr->read_data.access.file.file_enum == MMGSDI_CSIM_PRL ||
                 extra_param_ptr->read_data.access.file.file_enum == MMGSDI_CSIM_EPRL))
               ||
               (protocol == MMGSDI_ICC &&
                (extra_param_ptr->read_data.access.file.file_enum == MMGSDI_CDMA_PRL ||
                 extra_param_ptr->read_data.access.file.file_enum == MMGSDI_CDMA_EPRL)))
      {
        if (extra_param_ptr->read_data.data_len > MMGSDI_SIZE_OF_PRL_SIZE)
        {
          data_from = MMGSDI_DATA_FROM_CARD_COMPLETE;
        }
      }
      /* MMGSDI sends complete read request for partial read request for files
         present in cache and if the cache is not initialized then the entry is
         present in the cache list. In this case, mark the data from card as complete.*/
      else if (extra_param_ptr->read_data.file_type == MMGSDI_TRANSPARENT_FILE &&
               extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)
      {
        mmgsdi_len_type             cache_data_len = 0;
        mmgsdi_cache_init_enum_type cache_state    = MMGSDI_CACHE_MAX_ENUM;

        if(MMGSDI_NOT_INIT == mmgsdi_cache_read_len(req_header.session_id,
                                                    &extra_param_ptr->read_data.access,
                                                    extra_param_ptr->read_data.rec_num,
                                                    &cache_data_len,
                                                    &cache_state))
        {
          data_from = MMGSDI_DATA_FROM_CARD_COMPLETE;

          /* When read request was processed in case of cache not_inited, 
		     data_len is set to full file size and offset = 0 and extra_params
			 has the partial read req data_len and offset. 
			 therefore, update cache with proper offset if read request is for
			 partial data and use this offset to write to cache instead of the
			 offset data from extra_param */
          mmgsdi_offset = 0;
          is_cache_init_read = TRUE;
        }
      }

      /* Check if the file is ISIM IST, and check if the GBA service (n2)is enabled in the card,
         if GBA service is enabled and NV 73764 disable_gba_u_service is enabled then
         disable the GBA service in the read response. */
      if (extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS &&
          extra_param_ptr->read_data.access.file.file_enum == MMGSDI_ISIM_IST && 
         (rsp_data_buf.data_len >= 1)  && 
         (rsp_data_buf.data_ptr != NULL) &&
         (rsp_data_buf.data_ptr[0] & MMGSDI_ISIM_SRV_GBA) &&
         (mmgsdi_util_get_efs_item_index(req_header.session_id, NULL, &efs_index) == MMGSDI_SUCCESS) &&
         (MMGSDI_FEATURE_ENABLED == mmgsdi_nv_get_slot_feature_status(MMGSDI_FEATURE_DISABLE_GBA_U, efs_index)))
      {
        rsp_data_buf.data_ptr[0] &= (~MMGSDI_ISIM_SRV_GBA);
      }

      (void)mmgsdi_cache_write(req_header.session_id,
                               &extra_param_ptr->read_data.access,
                               rsp_data_buf,
                               mmgsdi_offset,
                               data_from,
                               extra_param_ptr->read_data.rec_num);
    }
  }
  else
  {
    /* unsuccessful processing of command */
    if (extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)
    {
      if(report_status == MMGSDI_NOT_FOUND ||
         report_status == MMGSDI_ACCESS_DENIED)
      {
        if((extra_param_ptr->read_data.file_type == MMGSDI_LINEAR_FIXED_FILE)&&
           (extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)&&
           ((protocol == MMGSDI_UICC &&
             (extra_param_ptr->read_data.access.file.file_enum == MMGSDI_USIM_OPL||
              extra_param_ptr->read_data.access.file.file_enum == MMGSDI_USIM_PNN))
            ||
            (protocol == MMGSDI_ICC &&
             (extra_param_ptr->read_data.access.file.file_enum == MMGSDI_GSM_OPL||
              extra_param_ptr->read_data.access.file.file_enum == MMGSDI_GSM_PNN))))
        {
          mmgsdi_status = mmgsdi_eons_opl_pnn_update_record(
                            req_header.session_id,
                            extra_param_ptr->read_data.access.file.file_enum,
                            rsp_data_buf,
                            (mmgsdi_rec_num_type)extra_param_ptr->read_data.rec_num,
                            MMGSDI_CACHE_NOT_FOUND);
        }
        else if(report_status == MMGSDI_NOT_FOUND)
        {
          /* Update cache to indicate that file was not found on card */
          (void)mmgsdi_cache_write(req_header.session_id,
                                   &extra_param_ptr->read_data.access,
                                   rsp_data_buf,
                                   extra_param_ptr->read_data.offset,
                                   MMGSDI_DATA_NOT_FOUND,
                                   extra_param_ptr->read_data.rec_num);
        }
      }
    }
  } /* Parsing UIM Status */

  if (report_status == MMGSDI_SUCCESS)
  {
    /* if read from cache, and the operation is successful,
       the cmd_transacted will be FALSE.  if the operation is not successful,
       the command will go to uim */
    sw_present = TRUE;
  }
  else if (protocol == MMGSDI_UICC &&
           (report_status == MMGSDI_INCORRECT_LENGTH) &&
           !extra_param_ptr->read_data.select_before_read &&
           (extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS) &&
           (extra_param_ptr->read_data.file_type == MMGSDI_TRANSPARENT_FILE) &&
           (MMGSDI_SUCCESS == mmgsdi_file_get_file_info_from_table(
                                extra_param_ptr->read_data.access.file.file_enum,
                                &file_info)) &&
           (file_info.size != MMGSDI_INVALID_FILE_SIZE) &&
           mmgsdi_file_is_sfi_supported(req_header.slot_id))
  {
    /* MMGSDI has static tables that contain the spec-defined SFIs and
       spec-defined file lengths. If a file has an entry in that table,
       MMGSDI uses the file legnth (is client passed in zero as read
       data len) and SFI from those static tables to send a read request
       to the uimdrv. There are some non-spec compliant cards that have
       file length different than what is mentioned in the spec. For those
       files, card may return 67 00 (incorrect length). If that is the case,
       perform an explicit SELECT on the file and gets its length and retry
       the read with that length. In order to make sure the reattempt is
       performed only once (to avoid an infinite loop) we need to check
       extra_param_ptr->read_data.select_before_read before reattempting
       the read request. */
    mmgsdi_read_req_type               *read_req_ptr                = NULL;

    /* Retry the READ request with explicit SELECT. This READ will be sync
       or async depending on whether the original READ request was sync or
       async */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(read_req_ptr, sizeof(mmgsdi_read_req_type));
    if (read_req_ptr == NULL)
    {
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(rsp_data_buf.data_ptr);
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
    /* Since command is being retried with SELECT, mark the following
       as TRUE so that MMGSDI can ensure that upon receiving
       MMGSDI_INCORRECT_LENGTH, MMGSDI does not reattempt more than
       once.. we dont want to get into a situation where if someone
       fakes the card response as 0x67 0x00 everytime for the read
       request, resulting in infinite queueing and processing of
       READ requests */
    read_req_ptr->select_before_read               = TRUE;
    read_req_ptr->request_header.client_id         = req_header.client_id;
    read_req_ptr->request_header.session_id        = req_header.session_id;
    read_req_ptr->request_header.request_type      = MMGSDI_READ_REQ;
    read_req_ptr->request_header.orig_request_type = MMGSDI_READ_REQ;
    read_req_ptr->request_header.slot_id           = req_header.slot_id;
    read_req_ptr->request_header.client_data       = req_header.client_data;
    read_req_ptr->request_header.response_cb       = req_header.response_cb;
    mmgsdi_memscpy(&read_req_ptr->access, sizeof(mmgsdi_access_type),
                   &extra_param_ptr->read_data.access, sizeof(mmgsdi_access_type));
    read_req_ptr->rec_num                          = extra_param_ptr->read_data.rec_num;
    read_req_ptr->offset                           = extra_param_ptr->read_data.offset;
    read_req_ptr->file_type                        = extra_param_ptr->read_data.file_type;
    read_req_ptr->data_len                         = extra_param_ptr->read_data.data_len;

    mmgsdi_status = mmgsdi_uim_uicc_read_transparent(read_req_ptr, synch, mmgsdi_cnf_pptr);

    MMGSDIUTIL_TMC_MEM_FREE(read_req_ptr);

    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* A read request has been attempted again, which will take care of
         populating and sending the cnf to the client. This current failed
         read request can return now without any futher action as it is
         no longer needed. The entry for this current failed read request
         will be removed from client req table ptr as well. */
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(rsp_data_buf.data_ptr);
      return MMGSDI_SUCCESS;
    }
  }
  else
  {
    sw_present = modified_uim_rsp_ptr->cmd_transacted;
  }

  cnf_data_buf.data_len = rsp_data_buf.data_len;
  cnf_data_buf.data_ptr = rsp_data_buf.data_ptr;

  /* update cnf_data_buf if MMGSDI sends complete read request
     for partial read request for files present in cache but cache is
     not initialized*/
  if (is_cache_init_read &&
      extra_param_ptr->read_data.data_len != 0 &&
      extra_param_ptr->read_data.data_len < rsp_data_buf.data_len)
  {
    cnf_data_buf.data_len = extra_param_ptr->read_data.data_len;

    if (extra_param_ptr->read_data.offset != 0 &&
        extra_param_ptr->read_data.offset < rsp_data_buf.data_len)
    {
      cnf_data_buf.data_ptr =
           &rsp_data_buf.data_ptr[extra_param_ptr->read_data.offset];
    }
  }

  if (!synch)
  {
    mmgsdi_status = mmgsdi_cnf_build_and_queue(
      report_status, &req_header, extra_param_ptr,
      (void*)&cnf_data_buf,
      sw_present,  /* status word required for read */
      modified_uim_rsp_ptr->status_word);
  }
  else
  {
    if (mmgsdi_cnf_pptr == NULL)
    {
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr);    
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(rsp_data_buf.data_ptr);
      return MMGSDI_ERROR;
    }

    /* Obtain memory for confirmation data */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE((*mmgsdi_cnf_pptr),
      sizeof(mmgsdi_read_cnf_type));
    if ((*mmgsdi_cnf_pptr) == NULL)
    {
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr);    
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(rsp_data_buf.data_ptr);
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
    cnf_ptr = *mmgsdi_cnf_pptr;

    mmgsdi_status = mmgsdi_cnf_build_read_data(
      cnf_ptr,
      report_status,
      extra_param_ptr,
      (mmgsdi_data_type*)&cnf_data_buf);

    if (mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* Populate Header */
      /* Header population should be done AFTER data populate
        because any pointer length will be assigned in the
        individual confirmation population and used in the
        populate_cnf_rsp_header for assigning the response
        length */
      mmgsdi_util_populate_cnf_rsp_header(&cnf_ptr->response_header,
                                          &req_header,
                                          MMGSDI_READ_REQ,
                                          sw_present,
                                          /* Status word required for read */
                                          modified_uim_rsp_ptr->status_word);

      cnf_ptr->response_header.mmgsdi_status = report_status;
    }
  }

  MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(rsp_data_buf.data_ptr);
  return mmgsdi_status;
} /* mmgsdi_common_rsp_read */


/* ============================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_WRITE

   DESCRIPTION:
     This function will build the response to Write request

   DEPENDENCIES:


   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_write (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr,
  mmgsdi_write_cnf_type                ** mmgsdi_cnf_pptr,
  boolean                                 synch,
  mmgsdi_protocol_enum_type               protocol
)
{
  mmgsdi_request_header_type             req_header;
  uint32                                 index                = 0;
  mmgsdi_return_enum_type                mmgsdi_status        = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    * extra_param_ptr      = NULL;
  mmgsdi_return_enum_type                report_status        = MMGSDI_SUCCESS;
  mmgsdi_write_cnf_type                * cnf_ptr              = NULL;
  mmgsdi_data_type                       write_data_buf;
  mmgsdi_uim_report_rsp_type           * modified_uim_rsp_ptr = NULL;
  mmgsdi_file_info_table_entry_categ_enum_type  table_entry_categ    = MMGSDI_ENTRY_CATEG_CACHE_FALLBACK;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);
  if (synch)
  {
    MMGSDIUTIL_RETURN_IF_NULL(mmgsdi_cnf_pptr);
  }

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  memset(&write_data_buf, 0x00, sizeof(mmgsdi_data_type));

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (extra_param_ptr == NULL ||
      extra_param_ptr->write_data.data.data_ptr == NULL ||
      extra_param_ptr->write_data.data.data_len <= 0)
  {
    UIM_MSG_ERR_0("No extra write response data");
    return MMGSDI_ERROR;
  }

  /* Parse the response status returned from UIM */
  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  /* Copy the UIM Response data */
  mmgsdi_status = mmgsdi_util_alloc_copy_uim_rsp(&modified_uim_rsp_ptr,
                                                 uim_rsp_ptr);
  
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Parse the UIM response to a Write request and provide content
     from File System if file system fallback is enabled for the file */
  report_status = mmgsdi_file_fs_parse_uim_write_rsp(
                    modified_uim_rsp_ptr,
                    &req_header,
                    extra_param_ptr,
                    report_status);

  /* copy rsp_data from uim_rsp_ptr so it can be modified by
     TMSI PLMN check during util write cache operation*/
  write_data_buf.data_len = extra_param_ptr->write_data.data.data_len;

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(write_data_buf.data_ptr,
                                     write_data_buf.data_len);

  /* In case of failed memory allocation, clear cache to avoid stale data */
  if (write_data_buf.data_ptr == NULL)
  {
    if (extra_param_ptr->increase_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)
    {
      (void)mmgsdi_cache_delete_item(req_header.session_id,
                                     1,
                                     &extra_param_ptr->increase_data.access.file.file_enum,
                                     0,
                                     NULL);
    }
  }
  else
  {
    mmgsdi_memscpy(write_data_buf.data_ptr,
                   write_data_buf.data_len,
                   extra_param_ptr->write_data.data.data_ptr,
                   write_data_buf.data_len);

    if (report_status == MMGSDI_SUCCESS)
    {
      mmgsdi_data_from_enum_type data_from = MMGSDI_DATA_FROM_CARD_INCOMPLETE;

      if(extra_param_ptr->write_data.data.data_len == 0 && 
         extra_param_ptr->write_data.offset == 0)
      {
        data_from = MMGSDI_DATA_FROM_CARD_COMPLETE;
      }

      (void)mmgsdi_cache_write(req_header.session_id,
                               &extra_param_ptr->write_data.access,
                               write_data_buf,
                               extra_param_ptr->write_data.offset,
                               data_from,
                               extra_param_ptr->write_data.rec_num);

      mmgsdi_status = MMGSDI_SUCCESS;
    }
    else
    {
      /* unsuccessful processing of command */
      if (extra_param_ptr->write_data.access.access_method == MMGSDI_EF_ENUM_ACCESS)
      {
        /* If file is not found on SIM card, but fallback to cache is supported, 
           do not delete cache entry (which is needed to allow subsequent reads from cache) */

        if(report_status == MMGSDI_NOT_FOUND && 
           (mmgsdi_file_fallback_check_cache_delete_allowed(extra_param_ptr->write_data.access.file.file_enum) == 
            TRUE))
        {
          (void)mmgsdi_cache_write(req_header.session_id,
                                   &extra_param_ptr->write_data.access,
                                   write_data_buf,
                                   extra_param_ptr->write_data.offset,
                                   MMGSDI_DATA_NOT_FOUND,
                                   extra_param_ptr->write_data.rec_num);
        }
      }
    } /* Parsing UIM Status */

    MMGSDIUTIL_TMC_MEM_FREE(write_data_buf.data_ptr);
  }

  if(!synch)
  {
    mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                               &req_header,
                                               extra_param_ptr,
                                               NULL,
                                               modified_uim_rsp_ptr->cmd_transacted,
                                               /* status word required for write */
                                               modified_uim_rsp_ptr->status_word);
  }
  else
  {
    /* Obtain memory for confirmation data */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE((*mmgsdi_cnf_pptr),
      sizeof(mmgsdi_write_cnf_type));
    if ((*mmgsdi_cnf_pptr) == NULL)
    {
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    cnf_ptr = *mmgsdi_cnf_pptr;

    mmgsdi_status = mmgsdi_cnf_build_write_data(cnf_ptr,
                                                report_status,
                                                extra_param_ptr);

    /* Populate Header */
    /* Header population should be done AFTER data populate because any pointer
       length will be assigned in the individual confirmation population and used in the
       populate_cnf_rsp_header for assigning the response   length
    */
    mmgsdi_util_populate_cnf_rsp_header(&cnf_ptr->response_header,
                                        &req_header,
                                        MMGSDI_WRITE_REQ,
                                        modified_uim_rsp_ptr->cmd_transacted,
                                        /* Status word required for read */
                                        modified_uim_rsp_ptr->status_word);

    if (mmgsdi_status == MMGSDI_SUCCESS)
    {
      cnf_ptr->response_header.mmgsdi_status = report_status;
    }
    else
    {
      UIM_MSG_ERR_1("Failed to build write data: status 0x%x", mmgsdi_status);
      cnf_ptr->response_header.mmgsdi_status = mmgsdi_status;
    }
  }

  MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr->rsp_data.data_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(modified_uim_rsp_ptr);
  return mmgsdi_status;
} /* mmgsdi_common_rsp_write */


/* ============================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_ACTIVATE_ONCHIP

   DESCRIPTION:
     This function will build the response to ACTIVATE ONCHIP REQ

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

============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_activate_onchip(
  mmgsdi_return_enum_type                     mmgsdi_status,
  const mmgsdi_activate_onchip_sim_req_type * onchip_sim_config_ptr
)
{
  mmgsdi_sw_status_type   status_word;

  UIM_MSG_HIGH_1("MMGSDI_PROCESS_ACTIVATE_ONCHIP_RSP status 0x%x", mmgsdi_status);

  /* Status word not applicable to Onchip command */
  status_word.sw1 = 0x00;
  status_word.sw2 = 0x00;

  MMGSDIUTIL_RETURN_IF_NULL(onchip_sim_config_ptr);

  return mmgsdi_cnf_build_and_queue(mmgsdi_status,
                                    &onchip_sim_config_ptr->request_header,
                                    NULL,
                                    NULL,
                                    FALSE,/* sw not required for onchip */
                                    status_word);
} /* mmgsdi_common_rsp_activate_onchip */


/* ============================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_COMPUTE_IP_AUTH

   DESCRIPTION:
     This function will build the response to a COMPUTE IP AUTH command

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_compute_ip_auth (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr,
  mmgsdi_protocol_enum_type               protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type          req_header;
  uint32                              index                 = 0;
  mmgsdi_return_enum_type             mmgsdi_status         = MMGSDI_SUCCESS;
  mmgsdi_return_enum_type             report_status         = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type * extra_param_ptr       = NULL;

  /* --------------------------------------------------------------------------
     Validate the Input pointers.  Must not be NULL
     ------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  /*---------------------------------------------------------------------------
    1) Parse the response status returned from UIM
    2) If success:
         update record number accessed in confirmation type,
  ---------------------------------------------------------------------------*/
  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_3("UIM_FAIL sw1 %x sw2 %x status %d",
                   uim_rsp_ptr->status_word.sw1, uim_rsp_ptr->status_word.sw2,
                   report_status);
  } /* Parsing UIM Status */

  /* Print the data being sent from UIM*/
  mmgsdi_util_print_data_to_diag("From Card : ",
                                  uim_rsp_ptr->rsp_data.data_ptr,
                                  uim_rsp_ptr->rsp_data.data_len);

  mmgsdi_status = mmgsdi_cnf_build_and_queue(
      report_status,
      &req_header,
      extra_param_ptr,
      (void*)&uim_rsp_ptr->rsp_data,
      uim_rsp_ptr->cmd_transacted,
      uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_rsp_compute_ip_auth */


/* ============================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_RUN_CAVE

   DESCRIPTION:
     This function will build the response to a RUN CAVE command

   DEPENDENCIES:

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_run_cave (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr,
  mmgsdi_protocol_enum_type               protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type          req_header;
  uint32                              index                 = 0;
  mmgsdi_return_enum_type             mmgsdi_status         = MMGSDI_SUCCESS;
  mmgsdi_return_enum_type             report_status         = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type * extra_param_ptr       = NULL;

  /* --------------------------------------------------------------------------
     Validate the Input pointers.  Must not be NULL
     ------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }
  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  /*---------------------------------------------------------------------------
    1) Parse the response status returned from UIM
    2) If success:
         update record number accessed in confirmation type,
  ---------------------------------------------------------------------------*/
  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_3("UIM_FAIL sw1 %x sw2 %x status %d",
                   uim_rsp_ptr->status_word.sw1, uim_rsp_ptr->status_word.sw2,
                   report_status);
  } /* Parsing UIM Status */

  mmgsdi_status = mmgsdi_cnf_build_and_queue(
      report_status,
      &req_header,
      extra_param_ptr,
      (void*)&uim_rsp_ptr->rsp_data,
      uim_rsp_ptr->cmd_transacted,
      uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_rsp_run_cave */


/* ============================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_CDMA_AKA_AUTH

   DESCRIPTION:
     This function will build the response to a CDMA AKA AUTH command

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_cdma_aka_auth (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr,
  mmgsdi_protocol_enum_type               protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type          req_header;
  uint32                              index                 = 0;
  mmgsdi_return_enum_type             mmgsdi_status         = MMGSDI_SUCCESS;
  mmgsdi_return_enum_type             report_status         = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type * extra_param_ptr       = NULL;

  /* --------------------------------------------------------------------------
     Validate the Input pointers.  Must not be NULL
     ------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }
  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  /*---------------------------------------------------------------------------
    1) Parse the response status returned from UIM
    2) If success:
         update record number accessed in confirmation type,
  ---------------------------------------------------------------------------*/
  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_3("UIM_FAIL sw1 %x sw2 %x status %d",
                   uim_rsp_ptr->status_word.sw1, uim_rsp_ptr->status_word.sw2,
                   report_status);
  } /* Parsing UIM Status */

  if (uim_rsp_ptr->rsp_data.data_len > 0 && uim_rsp_ptr->rsp_data.data_ptr != NULL)
  {
    UIM_MSG_HIGH_1("CDMA-AKA auth response first byte: 0x%x",
                   uim_rsp_ptr->rsp_data.data_ptr[0]);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(
      report_status,
      &req_header,
      extra_param_ptr,
      (void*)&uim_rsp_ptr->rsp_data,
      uim_rsp_ptr->cmd_transacted,
      uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_rsp_cdma_aka_auth */


/* ============================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_GENERATE_KEY_VPM

   DESCRIPTION:
     This function will build the response to a GENERATE KEY command

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_generate_key_vpm (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr,
  mmgsdi_protocol_enum_type               protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type          req_header;
  uint32                              index                 = 0;
  mmgsdi_return_enum_type             mmgsdi_status         = MMGSDI_SUCCESS;
  mmgsdi_return_enum_type             report_status         = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type * extra_param_ptr       = NULL;
  mmgsdi_data_type                    response_data;

  memset(&response_data, 0x00, sizeof(mmgsdi_data_type));

  /* --------------------------------------------------------------------------
     Validate the Input pointers.  Must not be NULL
     ------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }
  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  /*---------------------------------------------------------------------------
    1) Parse the response status returned from UIM
    2) If success:
         update record number accessed in confirmation type,
  ---------------------------------------------------------------------------*/
  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_3("UIM_FAIL sw1 %x sw2 %x status %d",
                   uim_rsp_ptr->status_word.sw1, uim_rsp_ptr->status_word.sw2,
                   report_status);
  } /* Parsing UIM Status */

  /* Look back in the original command to see what the expected size of the
    response should be. The response should an 8-byte key, plus n VPM Octets
    where n is last - first + 1 according to IS-820-C */

  if((extra_param_ptr != NULL) &&
     (extra_param_ptr->generate_key.vpm_first_octet != 0xFF) &&
     (extra_param_ptr->generate_key.vpm_last_octet != 0xFF) &&
     (extra_param_ptr->generate_key.vpm_last_octet >=
      extra_param_ptr->generate_key.vpm_first_octet))
  {
    response_data.data_len = (extra_param_ptr->generate_key.vpm_last_octet) -
      (extra_param_ptr->generate_key.vpm_first_octet) + 1 + MMGSDI_KEY_SIZE;
    if(uim_rsp_ptr->rsp_data.data_len > UIM_MAX_CHARS)
    {
       UIM_MSG_HIGH_1("Invalid response length from GENERATE KEY command. Length: %d",
                      uim_rsp_ptr->rsp_data.data_len);
       return MMGSDI_ERROR;
    } /* end if Response is too long, something wrong. Limit to UIM_MAX_CHARS */
  } /* end if - the UIM Command data has valid first / last octet data */
  else
  {
    UIM_MSG_HIGH_0("Invalid input data for Generate Key. Response unpopulated");
    return MMGSDI_ERROR;
  } /* end if  - the UIM command Octet Data has invalid first / last info */

  /* Obtain memory for confirmation data */
  if (uim_rsp_ptr->rsp_data.data_ptr &&
      response_data.data_len > 0)
  {
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(response_data.data_ptr,
                                       response_data.data_len);
    if (response_data.data_ptr == NULL)
    {
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    mmgsdi_memscpy(response_data.data_ptr,
                   response_data.data_len,
                   uim_rsp_ptr->rsp_data.data_ptr,
                   response_data.data_len);
  }
  else
  {
    UIM_MSG_HIGH_0("No response data allocated");
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(
      report_status,
      &req_header,
      extra_param_ptr,
      (void*)&response_data,
      FALSE, /* status word not required for generate key */
      uim_rsp_ptr->status_word);

  MMGSDIUTIL_TMC_MEM_FREE(response_data.data_ptr);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_rsp_generate_key_vpm */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_OTASP_BLOCK

  DESCRIPTION:
    This function will build the response to OTASP Block Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_otasp_block (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* unsuccessful processing of command */
    UIM_MSG_ERR_2("UIM Response parsing failed for OTASP 0x%x Block Request 0x%x",
                  extra_param_ptr->session_otasp_block_data.otasp_block_op_type,
                  report_status);
    /* -------------------------------------------------------------------
    Command processing returned Status words other than 0x90 0x00
    Log Error Message
    ----------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM_FAIL sw1:0x%x sw2:0x%x status:0x%0x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for OTASP block operation */
                                             uim_rsp_ptr->status_word);

  UIM_MSG_HIGH_2("RSP - OTASP block 0x%x Request status is 0x%x",
                 extra_param_ptr->session_otasp_block_data.otasp_block_op_type,
                 mmgsdi_status);
  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_otasp_block */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_OTASP_OTAPA

  DESCRIPTION:
    This function will build the response to OTASP OTAPA Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_otasp_otapa (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for OTAPA Request sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for OTA operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_otasp_otapa */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_OTASP_COMMIT

  DESCRIPTION:
    This function will build the response to OTASP COMMIT Request

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_otasp_commit (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);
  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for OTASP Commit sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for OTASP Commit operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_otasp_commit */

/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_OTASP_SSPR_CONFIG

  DESCRIPTION:
    This function will build the response to OTASP SSPR Config Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_otasp_sspr_config (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for OTASP SSPR Config Request sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for OTASP SSPR Config operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_otasp_sspr_config */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_OTASP_KEY_GEN

  DESCRIPTION:
    This function will build the response to OTASP Key Gen Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_otasp_key_gen (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for OTASP Key Gen Request sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for OTASP KEY GEN operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_otasp_key_gen */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_OTASP_MS_KEY

  DESCRIPTION:
    This function will build the response to OTASP MS Key Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_otasp_ms_key (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for OTASP MS Key Request sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for OTASP MS Key operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_otasp_ms_key */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_BS_CHAL

  DESCRIPTION:
    This function will build the response to BS CHAL Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_bs_chal (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }


  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for BS CHAL Request sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for BS CHAL operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_bs_chal */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_SSD_CONFIRM

  DESCRIPTION:
    This function will build the response to SSD Confirm Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_ssd_confirm (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for SSD Confirm Request sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for SSD Confirm operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_ssd_confirm */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_SSD_UPDATE

  DESCRIPTION:
    This function will build the response to SSD Update Request

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS          : The command process is successful.
    MMGSDI_INCORRECT_PARAMS : The parameters supplied to the API are not
                              within appropriate ranges.

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_ssd_update (
  const mmgsdi_uim_report_rsp_type   * uim_rsp_ptr,
  mmgsdi_protocol_enum_type            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type           req_header;
  uint32                               index           = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type    *extra_param_ptr= NULL;
  mmgsdi_return_enum_type              report_status   = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                                     index,
                                                     &req_header,
                                                     &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  if (report_status != MMGSDI_SUCCESS)
  {
    /* -----------------------------------------------------------------------
    Unsuccessful processing of command, command processing returned
    Status words other than 0x90 0x00. Log Error Message
    --------------------------------------------------------------------------*/
    UIM_MSG_ERR_3("UIM Response parsing failed for SSD Update Request sw1:0x%x, sw2:0x%x report_status:0x%x",
                  uim_rsp_ptr->status_word.sw1,
                  uim_rsp_ptr->status_word.sw2,
                  report_status);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for SSD Update operation */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
}/* mmgsdi_common_rsp_ssd_update */


/* ============================================================================
  FUNCTION:      MMGSDI_COMMON_RSP_STORE_ESN

  DESCRIPTION:
    This function will build the response to Store ESN request

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:

=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_store_esn(
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr,
  mmgsdi_session_store_esn_cnf_type    ** store_esn_cnf_pptr,
  boolean                                 synch,
  mmgsdi_protocol_enum_type               protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_request_header_type         req_header;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  uint32                             index            = 0;
  mmgsdi_client_req_extra_info_type *extra_param_ptr  = NULL;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;
  mmgsdi_session_store_esn_cnf_type *cnf_ptr          = NULL;
  mmgsdi_slot_data_type             *slot_data_ptr    = NULL;

  UIM_MSG_HIGH_1("MMGSDI_COMMON_RSP_STORE_ESN, synch: 0x%x", synch);

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);
  if (synch)
  {
    MMGSDIUTIL_RETURN_IF_NULL(store_esn_cnf_pptr);
  }

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_header.slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_status = mmgsdi_uicc_rsp_parse_uim_response(uim_rsp_ptr);
  }
  else
  {
    report_status = mmgsdi_icc_rsp_parse_uim_response(uim_rsp_ptr);
  }

  UIM_MSG_HIGH_1("Store ESN status 0x%x",report_status);

  /* Store the ESN ME Change Flag, LSB indicates if ESN has changed*/
  if ((report_status == MMGSDI_SUCCESS ) &&
      (uim_rsp_ptr->rsp_data.data_ptr != NULL) &&
      (uim_rsp_ptr->rsp_data.data_len == 1))
  {
    if(uim_rsp_ptr->rsp_data.data_ptr[0] & MMGSDI_JCDMA_ESN_ME_BIT)
    {
      slot_data_ptr->esn_me_change_flag = MMGSDI_ESN_ME_CHANGE;
    }
    else
    {
      slot_data_ptr->esn_me_change_flag = MMGSDI_ESN_ME_NO_CHANGE;
    }
    UIM_MSG_HIGH_1("ESN ME Change flag set to : 0x%x",
                   slot_data_ptr->esn_me_change_flag);
  }

  if (!synch)
  {
    mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                               &req_header,
                                               NULL,
                                               NULL,
                                               FALSE, /* status word not required
                                                        for store esn */
                                               uim_rsp_ptr->status_word);
  }
  else
  {
    /* Obtain memory for confirmation data */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
      (*store_esn_cnf_pptr),
      sizeof(mmgsdi_session_store_esn_cnf_type));
    if (*store_esn_cnf_pptr == NULL)
    {
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    cnf_ptr = *store_esn_cnf_pptr;

    /* Populate Header */
    /* Header population should be done AFTER data populate
      because any pointer length will be assigned in the
      individual confirmation population and used in the
      populate_cnf_rsp_header for assigning the response
      length */
    mmgsdi_util_populate_cnf_rsp_header(&cnf_ptr->response_header,
                                        &req_header,
                                        MMGSDI_SESSION_STORE_ESN_REQ,
                                        FALSE, /* status word not
                                               required for gfa */
                                        uim_rsp_ptr->status_word);

    cnf_ptr->response_header.mmgsdi_status = report_status;
  }

  UIM_MSG_HIGH_1("RSP - STORE_ESN status is 0x%x", report_status);
  return mmgsdi_status;
#else
  (void)uim_rsp_ptr;
  (void)store_esn_cnf_pptr;
  (void)synch;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_rsp_store_esn */


/* ============================================================================
   FUNCTION:      MMGSDI_COMMON_RSP_SRV_AVAILABLE

   DESCRIPTION:
     This function will build the response for the service available command

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_rsp_srv_available(
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
)
{
  mmgsdi_request_header_type          req_header;
  uint32                              index            = 0;
  mmgsdi_return_enum_type             mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type  *extra_param_ptr  = NULL;

  /* --------------------------------------------------------------------------
     Validate the Input pointers.  Must not be NULL
     ------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if(!uim_rsp_ptr->is_uim_success)
  {
    mmgsdi_status = MMGSDI_NOT_SUPPORTED;
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(mmgsdi_status,
                                             &req_header,
                                             extra_param_ptr,
                                            (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* status word not required
                                                       for srv avail */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
} /* mmgsdi_common_rsp_srv_available*/


/*----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_COMMON_RSP_SEND_APDU

   DESCRIPTION:
     This function will build the response to SEND_APDU request

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

------------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_common_rsp_send_apdu(
  const mmgsdi_uim_report_rsp_type* uim_rsp_ptr)
{
  mmgsdi_request_header_type         req_header;
  uint32                             index            = 0;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type* extra_param_ptr  = NULL;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;

  UIM_MSG_HIGH_0("MMGSDI_COMMON_RSP_SEND_APDU");

  /* Check input pointers.*/
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);
  /*---------------------------------------------------------------------------
    Obtain index
    Index should be [0, MMGSDI_MAX_CLIENT_REQ_INFO-1],
    Index is set when
    a) a success queuing of command to UIM
    b) a failure in queuing command to UIM but client_request pointer should
      be set in a valid index location in mmgsdi process command
  ---------------------------------------------------------------------------*/
  index = uim_rsp_ptr->rsp_data_index;

  /* Get the request header.*/
  mmgsdi_status =
    mmgsdi_util_get_client_request_table_info(
      uim_rsp_ptr->rsp_data_index,
      &req_header,
      &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             (void*)&uim_rsp_ptr->rsp_data,
                                             FALSE, /* sw not required
                                                       for send apdu */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
} /* mmgsdi_common_rsp_send_apdu*/


/*===========================================================================
FUNCTION MMGSDI_COMMON_RSP_PIN_OP

DESCRIPTION
  This function, called in mmgsdi_process_response.
  It will determine which card technology the command requires (UICC or ICC),
  determine the command type, and call the appropriate pin operation
  functions.

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  mmgsdi_return_enum_type
  MMGSDI_SUCCESS          - Command processed,
  MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
  MMGSDI_ERROR            - Other unknown failure

SIDE EFFECTS
  NONE
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_common_rsp_pin_op (
  const mmgsdi_uim_report_rsp_type *uim_rsp_ptr,
  mmgsdi_protocol_enum_type         protocol,
  mmgsdi_pin_operation_cnf_type   **cnf_pptr,
  boolean                           sync_process
)
{
  mmgsdi_return_enum_type             mmgsdi_status   = MMGSDI_SUCCESS;
  uint32                              index           = 0;
  mmgsdi_request_header_type          req_header;
  mmgsdi_client_req_extra_info_type * extra_param_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  index = uim_rsp_ptr->rsp_data_index;

  if (index >= MMGSDI_MAX_CLIENT_REQ_INFO || protocol == MMGSDI_NO_PROTOCOL)
  {
    UIM_MSG_ERR_2("Invalid index 0x%x or protocol 0x%x", index, protocol);
    return MMGSDI_ERROR;
  }

  if(mmgsdi_client_req_table_info_ptr[index] == NULL)
  {
    UIM_MSG_ERR_1("Index 0x%x not populated in MMGSDI client req table", index);
    return MMGSDI_ERROR;
  }

  /* Retrieve the PIN ID from the Client Table */
  mmgsdi_status = mmgsdi_util_get_client_request_table_info(
                                index,
                                &req_header,
                                &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* extra_param should never be NULL here */
  if(extra_param_ptr == NULL)
  {
    UIM_MSG_ERR_0("extra_param_ptr NULL in PIN operation response");
    return MMGSDI_ERROR;
  }

  UIM_MSG_HIGH_3("Processing Response for MMGSDI_PIN_OPERATION_REQ, Protocol: 0x%x, Slot: 0x%x, PIN ID: 0x%x",
                 protocol,
                 mmgsdi_client_req_table_info_ptr[index]->slot_id,
                 extra_param_ptr->pin_data.pin_info.pin_id);

  /* If operation is for Hiddenkey */
  if(extra_param_ptr->pin_data.pin_info.pin_id == MMGSDI_HIDDENKEY)
  {
    return mmgsdi_hidden_key_rsp_operation(uim_rsp_ptr);
  }

  if(protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_uicc_rsp_pin_operation(uim_rsp_ptr, cnf_pptr, sync_process);
  } /* protocol is MMGSDI_UICC */
  else
  {
      mmgsdi_status = mmgsdi_icc_rsp_pin_operation(uim_rsp_ptr, cnf_pptr, sync_process);
  } /* protocol is MMGSDI_ICC */
  return mmgsdi_status;
}/* mmgsdi_common_rsp_pin_op */
