/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                M M G S D I   I C C / U I C C  F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the command processing support for MMGSDI.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2013 - 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_common.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/07/18   rps     Retrieving attribute cache from caller context
05/09/17   me      Added a check for file type in read and write operation  
04/14/17   nr      Support to store more FPLMNs in cache for MMGSDI_USIM_FPLMN
03/29/17   nr      Add support to cache PIN1 of the SIM card when PSM mode enabled
01/26/17   shr     Add support for SIM file fallback to File System
01/03/17   sk      Removed USIM spec version
08/30/16   bcho    Optimization of MMGSDI attribute cache access functions
06/22/16   ar      Don't update the offset if read_len is less than file size
06/19/16   bcho    F3 frequency reduction by conditional logic
05/13/16   tkl     Review of macros used by MMGSDI
05/16/16   ar      F3 reduction
05/13/16   nr      Skip Search Req for PB files
05/10/16   tkl     F3 log prints cleanup
05/03/16   nr      Delete PB cache for the file that is being written currently
04/13/16   ar      CDMA/HDR support on 2nd SUB
04/06/16   ar      Remove duplicates from mmgsdi_util_print_file_information()
04/06/16   sp      Add DSDS onchip support
03/11/16   tkl     Free memory in extra_params in write operation
03/04/16   nr      Adding ONCHIP support for KCGPRS files
02/12/16   tkl     Skipped write operation if card data remains unchanged
01/17/16   ks      Send failure status words for read request
01/20/16   gm      Added support to purge temporary identity
11/11/15   av      Don't cache EF if service disabled in UST/SST for onchip
10/26/15   nr      Disable MMGSDI PB cache for PBM module
10/16/15   sp      Move client_req_table_info index variables to unsigned
10/05/15   sp      Reduce signed/unsigned conversions in UIM
09/10/15   kv      Features_status_list is split into device and slot items
08/31/15   bcho    Run SIMLOCK Algo in Onchip SIM
08/21/15   yt      Reduce duplicate code for PIN operations
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
07/29/15   sp      Remove UIM command in client request table
05/29/15   bcho    Support for Milenage and XOR algo in mmgsdi_uicc_onchip_auth
04/29/15   bcho    Support for App specific cache
04/06/15   ar      Remove duplicate code
03/06/15   bcho    Check cache status while handling MMGSDI_GET_FILE_ATTR_REQ
02/25/15   tl      Remove ICC seek support
11/14/14   hh      Fix compilation warnings
10/27/14   hh      Fix KW error
10/15/14   kk      Fixed onchip config for USIM EF KC
09/29/14   tkl     Added support for persistent cache
08/11/14   bcho    Optimize srv req by using cached srv table in client context
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
06/30/14   vv      Remove Perso code
06/18/14   av      Fix out of bound access in mmgsdi_util_find_app_pin_index
06/03/14   tl      Remove additional parameter variables
05/27/14   av      Don't allow access if UST/CST can't be read(pin not verified)
04/02/14   hh      Correct range check in mmgsdi_common_activate_onchip
03/27/14   yt      Ensure usage of correct slot id
03/18/14   tl      Introduce new SIM Lock feature
02/25/14   hh      Purge temporary mobile identities during UICC Reset Refresh
02/20/14   ak      Fix compile errors due to strict compiler on 9x35
12/05/13   tkl     SFI Support and decouple uim file enums
01/24/14   yt      Correctly initialize additional parameters
01/21/14   yt      Remove duplicate code
01/11/14   df      Fix off-target compilation error
01/10/14   df      Use v2.0 diag macros
12/16/13   tl      Prevent initalizing the cache with partial files
11/06/13   yt      Fixes for multi-SIM OnChip activation
10/03/13   yt      Multi-SIM support for OnChip
10/02/13   yt      Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "intconv.h"
#include "mmgsdi_uim_uicc.h"
#include "mmgsdi_uim_icc.h"
#include "mmgsdi.h"
#include "mmgsdi_evt.h"
#include "uim.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "mmgsdicache.h"
#include "mmgsdi_common_rsp.h"
#include "mmgsdi_card_init.h"
#include "mmgsdi_uim_common.h"
#include "mmgsdi_uicc.h"
#include "mmgsdi_icc.h"
#include "mmgsdi_nv.h"
#include "mmgsdicache.h"
#include "mmgsdi_eons.h"
#include "mmgsdi_file.h"
#include "mmgsdi_pb_cache.h"
#include "mmgsdi_cnf.h"
#include "mmgsdi_onchip.h"
#include "mmgsdi_file_fs.h"
#include "mmgsdi_hidden_key.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */


/*===========================================================================
   FUNCTION:      MMGSDI_COMMON_GET_FILE_ATTR

   DESCRIPTION:
     This function process the GET_FILE_ATTR command for ICC or UICC.

     Behavior for the function for the following file types:

     TRANSPARENT FILE:  Will get information on the characteristics of
                        Transparent file
     CYCLIC FILE:       Will get information on the characteristics of
                        Cyclic file.
     LINEAR FIXED FILE: Will get information on the characteristics of
                        Linear Fixed file.
     MASTER FILE:       Will get information on the Master File
     DEDICATED FILE:    Will get information on the Dedicated File

   DEPENDENCIES:
     The technology type (GSM/CDMA/UICC) has to be dtermined prior to using this
     function


   LIMITATIONS:
     None


   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The funtion was successful in getting the file
                              attributes of a particular file.
     MMGSDI_ERROR:
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                              within appropriate ranges.

   SIDE EFFECTS:
     None

=============================================================================*/
mmgsdi_return_enum_type mmgsdi_common_get_file_attr(
  mmgsdi_get_file_attr_req_type * req_ptr,
  mmgsdi_protocol_enum_type       protocol
)
{
  mmgsdi_return_enum_type             mmgsdi_status        = MMGSDI_ERROR;
  mmgsdi_int_app_info_type          * prov_app_info_ptr    = NULL;
  mmgsdi_len_type                     cache_len            = 0;
  mmgsdi_cache_init_enum_type         cache_state          = MMGSDI_CACHE_MAX_ENUM;
  mmgsdi_slot_data_type             * slot_data_ptr        = NULL;
  mmgsdi_decoded_attr_cache_type     *cache_attr_ptr       = NULL;
  mmgsdi_len_type                     raw_data_len         = 0;
  uint8                              *raw_data_ptr         = NULL;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_p                                  - Cannot be NULL
    2. req_p->request_header.response_cb      - Cannot be NULL
    3. req_p->access.access_method            - MMGSDI_EF_ENUM_ACCESS
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  if (protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);
  if (NULL == slot_data_ptr)
  {
    return MMGSDI_ERROR;
  }

  /* For onchip, return static file attributes only */
  if (MMGSDI_ONCHIP_SIM_INIT == slot_data_ptr->onchip_sim_data.state)
  {
    return mmgsdi_onchip_get_file_attr(req_ptr, protocol);
  }

  switch (req_ptr->access.access_method)
  {
    case MMGSDI_EF_ENUM_ACCESS:
      UIM_MSG_HIGH_1("Sending SELECT to UIM for MMGSDI_EF_ENUM_ACCESS 0x%x",
                     req_ptr->access.file.file_enum);
      break;

    case MMGSDI_DF_ENUM_ACCESS:
      UIM_MSG_HIGH_1("Sending SELECT to UIM for MMGSDI_DF_ENUM_ACCESS 0x%x",
                     req_ptr->access.file.df_enum);
      break;

    case MMGSDI_BY_PATH_ACCESS:
      UIM_MSG_HIGH_0("Sending SELECT to UIM for MMGSDI_BY_PATH_ACCESS");
      break;

    default:
      MMGSDIUTIL_DEBUG_MSG_ERROR_1("Invalid Access Type 0x%x",
                                   req_ptr->access.access_method);
      return MMGSDI_INCORRECT_PARAMS;
  }

  if(MMGSDI_SUCCESS != mmgsdi_util_check_file_access_data(
                                           req_ptr->request_header.session_id,
                                           req_ptr->access,
                                           protocol))
  {
    return MMGSDI_ERROR;
  }

  if(mmgsdi_util_get_prov_session_info(req_ptr->request_header.session_id,
                                       NULL,
                                       NULL,
                                       &prov_app_info_ptr) != MMGSDI_SUCCESS)
  {
    prov_app_info_ptr = NULL;
  }

  if (prov_app_info_ptr != NULL)
  {
    /* Check status of EF in cached SST/CST/UST */
    mmgsdi_status = mmgsdi_util_is_file_ok_in_svc_table(
      req_ptr->request_header.session_id,
      prov_app_info_ptr->app_data.app_type,
      &req_ptr->access);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      if(req_ptr->access.file.file_enum == MMGSDI_USIM_7F66_PROP1_ACT_HPLMN)
      {
        mmgsdi_status = MMGSDI_ACCESS_DENIED;
      }
      mmgsdi_util_print_file_information(req_ptr->access);
      return mmgsdi_status;
    }
    /* If Service Table in the card does not have the service enabled,
       return so that File Attr. can be returned from FS cache */
    mmgsdi_status = mmgsdi_file_fs_is_file_ok_in_orig_svc_table(
       req_ptr->request_header.session_id,
       prov_app_info_ptr->app_data.app_type,
       &req_ptr->access);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      return MMGSDI_NOT_FOUND;
    }
  }

  /* Check if MMGSDI has tried to cache this file and status is NOT FOUND. If
     yes, there is no reason to try to access the same file again and we can
     avoid sending the SELECT command */
  mmgsdi_status = mmgsdi_cache_read_len(req_ptr->request_header.session_id,
                                        &req_ptr->access,
                                        1,
                                        &cache_len,
                                        &cache_state);

  if((mmgsdi_status == MMGSDI_NOT_FOUND) &&
     (cache_state == MMGSDI_CACHE_NOT_FOUND))
  {
    return mmgsdi_status;
  }

  /* Check if the attributes are already cached */
  mmgsdi_status = mmgsdi_cache_attr_read(req_ptr->request_header.session_id,
                                         &req_ptr->access,
                                         &cache_attr_ptr,
                                         MMGSDI_SKIP_SECURITY_ATTRIBUTES,
                                         NULL,
                                         &raw_data_len,
                                         &raw_data_ptr);

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    /* We are here means the file attr is cached but the security attributes
       could not be decoded on the fly from caller context. So return the raw
       attributes from here and in response, security attributes will be calculated */
    do
    {
      uim_rpt_type                       report_response;
      mmgsdi_client_req_extra_info_type *extra_param_ptr = NULL;
      uint32                             index           = 0;

      /* Allocate and populate extra parameter */
      mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(MMGSDI_GET_FILE_ATTR_REQ, 
                                                                 (void*)req_ptr,
                                                                 &extra_param_ptr);
      if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
      {
        break;
      }

      mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
      if (mmgsdi_status != MMGSDI_SUCCESS)
      {
        /*---------------------------------------------------------------------------
          1) Release extra param pointer (Since client table has not been
                                          populated yet)
          2) Release uim cmd pointer  (Since client table has not been populated yet)
        ---------------------------------------------------------------------------*/
        MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
        break;
      }

      mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                                     &req_ptr->request_header,
                                                                     extra_param_ptr);

      if (mmgsdi_status != MMGSDI_SUCCESS)
      {
        /*---------------------------------------------------------------------------
          1) Release the client table
          2) Release extra param pointer (Since client table has not been
                                          populated yet)
          3) Release uim cmd pointer  (Since client table has not been populated yet)
        ---------------------------------------------------------------------------*/
        mmgsdi_util_free_client_request_table_index(index);
        MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
        break;
      }

      memset(&report_response, 0x00, sizeof(uim_rpt_type));

       /* populate the necessary field in the uim_report_type */
      report_response.rpt_status                = UIM_PASS;
      report_response.rpt.select.num_data_bytes = raw_data_len;

      if(raw_data_len > UIM_MAX_CHARS)
      {
        mmgsdi_util_free_client_request_table_index(index);
        MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }

      mmgsdi_memscpy(report_response.rpt.select.data,
                     sizeof(report_response.rpt.select.data),
                     raw_data_ptr,
                     raw_data_len);

      report_response.rpt_type = UIM_SELECT_R;

      report_response.sw1                     = SW1_NORMAL_END;
      report_response.sw2                     = SW2_NORMAL_END;
      report_response.cmd_transacted          = FALSE;
      report_response.user_data               = index;

      mmgsdi_uim_report(&report_response);
    }while(0);

    MMGSDIUTIL_TMC_MEM_FREE(raw_data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(cache_attr_ptr);
    return mmgsdi_status;
  }

  UIM_MSG_HIGH_0("Sending down SELECT command to MMGSDI UIM");

  /* Send select request to the UIM server */
  if (protocol == MMGSDI_ICC)
  {
    mmgsdi_status = mmgsdi_uim_icc_select(req_ptr, FALSE, NULL);
  }
  else if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_uim_uicc_select(req_ptr, FALSE, NULL, TRUE);
  }

  return mmgsdi_status;
} /* mmgsdi_common_get_file_attr */


/*===========================================================================
FUNCTION: MMGSDI_COMMON_READ_REPORT_FAILURE_RESPONSE

DESCRIPTION
  This function reports the status along with the status words
  during the intermediate select failure

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_common_read_report_failure_response (
  const mmgsdi_read_req_type        * req_ptr,
  mmgsdi_protocol_enum_type           protocol,
  mmgsdi_return_enum_type             report_status
)
{
  uint32                               index                = 0;
  mmgsdi_client_req_extra_info_type  * extra_param_ptr      = NULL;
  mmgsdi_return_enum_type              mmgsdi_status        = MMGSDI_SUCCESS;
  uim_rpt_type                         report_data;

  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);
  memset(&report_data, 0x00, sizeof(uim_rpt_type));

  if( report_status != MMGSDI_NOT_FOUND )
  {
    return report_status;
  }

  /* For FILE NOT FOUND case, report sw1 sw2 along with the status */
  switch (req_ptr->file_type)
  {
    case MMGSDI_LINEAR_FIXED_FILE:
    case MMGSDI_CYCLIC_FILE:
      report_data.rpt_type        = UIM_READ_REC_CMD_R;
      break;

    case MMGSDI_TRANSPARENT_FILE:
      report_data.rpt_type        = UIM_READ_BIN_CMD_R;
      break;

    default:
      return report_status;
  }

  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return report_status;
  }

  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
                                                  MMGSDI_READ_REQ,
                                                  (void*)req_ptr,
                                                  &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    mmgsdi_util_free_client_request_table_index(index);
    return report_status;
  }

  /*---------------------------------------------------------------------------
    Populate Client request table
    ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                                 &req_ptr->request_header,
                                                                 extra_param_ptr);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    return report_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    report_data.sw1                     = SW1_WRONG_PARAMS;
    report_data.sw2                     = SW2_FILE_NOT_FOUND;
  }
  else
  {
    report_data.sw1                     = SW1_REFERENCE;
    report_data.sw2                     = SW2_NOT_FOUND;
  }
  report_data.user_data                 = index;
  report_data.cmd_transacted            = TRUE;
  mmgsdi_uim_report(&report_data);

  return MMGSDI_SUCCESS;
} /* mmgsdi_common_read_report_failure_response */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_READ

  DESCRIPTION:
    This function will provide read access to any file using the U/ICC
    protocol

  DEPENDENCIES:


  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                            within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                            but the service table indicates the card does not
                            have the support

  SIDE EFFECTS:

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_read (
  mmgsdi_read_req_type        * req_ptr,
  mmgsdi_protocol_enum_type     protocol
)
{
  mmgsdi_return_enum_type              mmgsdi_status        = MMGSDI_SUCCESS;
  mmgsdi_data_type                     temp_data_holder     = {0, NULL};
  uint32                               index                = 0;
  mmgsdi_client_req_extra_info_type  * extra_param_ptr      = NULL;
  uim_rpt_type                         report_ptr;
  mmgsdi_int_app_info_type            *prov_app_info_ptr    = NULL;
  mmgsdi_slot_id_enum_type             slot                 = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_cache_init_enum_type          cache_state          = MMGSDI_CACHE_MAX_ENUM;
  boolean                              is_enum_imsi         = FALSE;
  mmgsdi_slot_data_type              * slot_data_ptr        = NULL;
  mmgsdi_onchip_sim_state_enum_type    onchip_state         = MMGSDI_ONCHIP_SIM_STATE_MAX_ENUM;
  boolean                              is_card_session      = FALSE;
  mmgsdi_session_type_enum_type        session_type         = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_nv_context_type               efs_index            = MMGSDI_NV_CONTEXT_INVALID;
  mmgsdi_decoded_attr_cache_type     * attr_cache_ptr       = NULL;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_p                                  - Cannot be NULL
    2. req_p->request_header.response_cb      - Cannot be NULL
    3. req_p->access.access_method            - MMGSDI_EF_ENUM_ACCESS
                                              - MMGSDI_BY_PATH_ACCESS
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  if (protocol != MMGSDI_UICC && protocol != MMGSDI_ICC)
  {
    return MMGSDI_ERROR;
  }

  /* Check for if it is EF access only */
  switch (req_ptr->access.access_method)
  {
    case MMGSDI_EF_ENUM_ACCESS:
    case MMGSDI_BY_PATH_ACCESS:
      break;

    default:
      MMGSDIUTIL_DEBUG_MSG_ERROR_1("Invalid Access Type 0x%x",
                                   req_ptr->access.access_method);
      return MMGSDI_ERROR;
  }

  if(MMGSDI_SUCCESS != mmgsdi_util_check_file_access_data(
                                           req_ptr->request_header.session_id,
                                           req_ptr->access,
                                           protocol))
  {
    return MMGSDI_ERROR;
  }

  if(mmgsdi_util_get_prov_session_info(req_ptr->request_header.session_id,
                                       NULL,
                                       &slot,
                                       &prov_app_info_ptr) != MMGSDI_SUCCESS)
  {
    /* Check if it is a card session - if so, allow reading of files at
       MF level from cache and from card */
    if(mmgsdi_util_get_session_type(req_ptr->request_header.session_id,
                                    &session_type,
                                    NULL) == MMGSDI_SUCCESS)
    {
      is_card_session = mmgsdi_util_is_card_session(session_type);
    }
    prov_app_info_ptr = NULL;
  }

  /* if it is not default app or if the access method is access by path
     read the data from the card. Check PB cache content first if it is
     phonebook EF */
  if ((prov_app_info_ptr == NULL &&
       is_card_session == FALSE) ||
      (req_ptr->access.access_method == MMGSDI_BY_PATH_ACCESS
      && mmgsdi_util_is_ef_under_phonebook(&req_ptr->access) == FALSE))
  {
    switch (req_ptr->file_type)
    {
      case MMGSDI_LINEAR_FIXED_FILE:
      case MMGSDI_CYCLIC_FILE:
        if (protocol == MMGSDI_UICC)
        {
          mmgsdi_status = mmgsdi_uim_uicc_read_record(req_ptr, FALSE, NULL);
        }
        else
        {
          mmgsdi_status = mmgsdi_uim_icc_read_record(req_ptr, FALSE, NULL);
        }
        break;

      case MMGSDI_TRANSPARENT_FILE:
        if (protocol == MMGSDI_UICC)
        {
          mmgsdi_status = mmgsdi_uim_uicc_read_transparent(req_ptr, FALSE, NULL);
        }
        else
        {
          mmgsdi_status = mmgsdi_uim_icc_read_transparent(req_ptr, FALSE, NULL);
        }
        break;

      default:
        return MMGSDI_INCORRECT_PARAMS;
    }

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      mmgsdi_status = mmgsdi_common_read_report_failure_response(req_ptr,
                                                                 protocol,
                                                                 mmgsdi_status);
    }
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    Check status of EF in cached UST. For files under MF, skip service table
    check
  ---------------------------------------------------------------------------*/
  if(mmgsdi_file_is_ef_under_mf(&req_ptr->access) == FALSE &&
     prov_app_info_ptr != NULL)
  {
    mmgsdi_status = mmgsdi_util_is_file_ok_in_svc_table(
                      req_ptr->request_header.session_id,
                      prov_app_info_ptr->app_data.app_type,
                      &req_ptr->access);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      if(req_ptr->access.file.file_enum == MMGSDI_USIM_7F66_PROP1_ACT_HPLMN &&
         protocol == MMGSDI_UICC)
      {
        mmgsdi_status = MMGSDI_ACCESS_DENIED;
      }
      return mmgsdi_status;
    }
    /* If Service Table in the card does not have the service enabled,
       return so that File Attr. can be returned from FS cache */
    mmgsdi_status = mmgsdi_file_fs_is_file_ok_in_orig_svc_table(
       req_ptr->request_header.session_id,
       prov_app_info_ptr->app_data.app_type,
       &req_ptr->access);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      return MMGSDI_NOT_FOUND;
    }   
  }

  if((req_ptr->file_type == MMGSDI_LINEAR_FIXED_FILE) &&
      ((protocol == MMGSDI_UICC &&
        (req_ptr->access.file.file_enum == MMGSDI_USIM_OPL ||
         req_ptr->access.file.file_enum == MMGSDI_USIM_PNN)) ||
       (protocol == MMGSDI_ICC &&
        (req_ptr->access.file.file_enum == MMGSDI_GSM_OPL ||
         req_ptr->access.file.file_enum == MMGSDI_GSM_PNN))))
  {
    mmgsdi_status = mmgsdi_eons_opl_pnn_read_record(req_ptr,
                                                    &temp_data_holder,
                                                    &cache_state);
  }
  /* Check the phonebook cache content if it EF is under Phonebook directory (5F3A)
     to avoid sending directly to the card */
  else if(req_ptr->file_type == MMGSDI_LINEAR_FIXED_FILE &&
          mmgsdi_util_is_ef_under_phonebook(&req_ptr->access) &&
          !mmgsdi_util_is_ef_pbr(&req_ptr->access))
  {
    uint32 client_index  = 0;

    mmgsdi_status = mmgsdi_util_get_client_id_index(req_ptr->request_header.client_id,
                                                    &client_index);

    /* Disable MMGSDI PB cache from client PBM to avoid unnecessary additional SEARCH commands
       performed by MMGSDI completely.*/
    if(mmgsdi_status == MMGSDI_SUCCESS && !MMGSDIUTIL_IS_PBM_CLIENT(client_index))
    {
      /* Get the phonebook record from cached data */
      mmgsdi_status = mmgsdi_pb_cache_get_record(req_ptr->request_header.slot_id,
                                                 req_ptr->rec_num,
                                                 &req_ptr->access, &temp_data_holder,
                                                 req_ptr->request_header.session_id);
    }
    else
    {
      /* Updating mmgsdi_status to MMGSDI_ERROR explicitly to send to the card
         for the commands coming from PBM.*/
      mmgsdi_status = MMGSDI_ERROR;
    }
  }
  else if (req_ptr->access.access_method == MMGSDI_EF_ENUM_ACCESS &&
           (((mmgsdi_util_get_efs_item_index(
                req_ptr->request_header.session_id, NULL, &efs_index) == MMGSDI_SUCCESS) &&
              (mmgsdi_nv_get_external_feature_status(
                 MMGSDI_EXT_FEAT_NAS_CFG_FEATURE_CSG, efs_index) == MMGSDI_FEATURE_ENABLED)) ||
              (req_ptr->access.file.file_enum != MMGSDI_USIM_ACSGL &&
               req_ptr->access.file.file_enum != MMGSDI_USIM_OCSGL &&
               req_ptr->access.file.file_enum != MMGSDI_USIM_CSGT &&
               req_ptr->access.file.file_enum != MMGSDI_USIM_OCSGT &&
               req_ptr->access.file.file_enum != MMGSDI_USIM_HNBN &&
               req_ptr->access.file.file_enum != MMGSDI_USIM_OHNBN)))
  {
    /* Get length of IMSI_M, IMSI_T and IMSI irrespective to requested len */
    if (req_ptr->access.access_method == MMGSDI_EF_ENUM_ACCESS &&
        (((req_ptr->access.file.file_enum == MMGSDI_CSIM_IMSI_M ||
           req_ptr->access.file.file_enum == MMGSDI_CSIM_IMSI_T ||
           req_ptr->access.file.file_enum == MMGSDI_USIM_IMSI) &&
          protocol == MMGSDI_UICC) ||
         ((req_ptr->access.file.file_enum == MMGSDI_CDMA_IMSI_M ||
           req_ptr->access.file.file_enum == MMGSDI_CDMA_IMSI_T ||
           req_ptr->access.file.file_enum == MMGSDI_GSM_IMSI) &&
          protocol == MMGSDI_ICC)))
    {
      is_enum_imsi = TRUE;
    }

    if(req_ptr->data_len == 0 || is_enum_imsi == TRUE)
    {
      /* Check the length of file if cached */
      mmgsdi_status = mmgsdi_cache_read_len(
                      req_ptr->request_header.session_id,
                      &req_ptr->access,
                      req_ptr->rec_num,
                      &temp_data_holder.data_len,
                      &cache_state);

      /*---------------------------------------------------------------------------
        If successfully retreived file length from cache then
        - Allocate temporary buffer
        - Read from Cache
        ---------------------------------------------------------------------------*/

      if (mmgsdi_status == MMGSDI_SUCCESS)
      {
        if(temp_data_holder.data_len <= req_ptr->offset)
        {
          mmgsdi_status = MMGSDI_ERROR;
        }
        else
        {
          temp_data_holder.data_len =
            temp_data_holder.data_len - req_ptr->offset;
        }
      }
    }
    else
    {
      temp_data_holder.data_len = req_ptr->data_len;
    }

    if (mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* 'temp_data_holder.data_len' could possibly be zero here */
      if(temp_data_holder.data_len == 0)
      {
        UIM_MSG_ERR_0("Can not allocate zero length memory to temp_data_holder.data_ptr");
        return MMGSDI_ERROR;
      }

      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(temp_data_holder.data_ptr,
                                         temp_data_holder.data_len);
      if (temp_data_holder.data_ptr == NULL)
      {
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      mmgsdi_status = mmgsdi_cache_read(req_ptr->request_header.session_id,
                                        &req_ptr->access,
                                        &temp_data_holder,
                                        req_ptr->offset,
                                        req_ptr->rec_num,
                                        &cache_state);

      if (mmgsdi_status == MMGSDI_SUCCESS)
      {
        /* check if the file type in the request matches the actual file type, 
           the check is needed since file cache access does not perform this verification */
        mmgsdi_status = mmgsdi_cache_attr_read(req_ptr->request_header.session_id,
                                               &req_ptr->access,
                                               &attr_cache_ptr,
                                               MMGSDI_SKIP_SECURITY_ATTRIBUTES,
                                               NULL,
                                               NULL,
                                               NULL);

        if(mmgsdi_status == MMGSDI_SUCCESS && attr_cache_ptr != NULL)
        {
          /* Requests to access both Linear fixed and cyclic files are done via 
             Linear Fixed File Type by MMGSDI APIs. Hence, request type of 
             Linear Fixed File and actual file type of cyclic file is not considered 
             a file type mismatch */
          if (attr_cache_ptr->file_type != req_ptr->file_type &&
            !(req_ptr->file_type == MMGSDI_LINEAR_FIXED_FILE && 
              attr_cache_ptr->file_type == MMGSDI_CYCLIC_FILE))
          {
            MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);
            MMGSDIUTIL_TMC_MEM_FREE(attr_cache_ptr);
            return MMGSDI_EF_INCONSISTENT;
          }
        }

        MMGSDIUTIL_TMC_MEM_FREE(attr_cache_ptr);
      }
    }
    /* Cache status indicates that the file is not present on the card */
    else if((mmgsdi_status == MMGSDI_NOT_FOUND) &&
            (cache_state == MMGSDI_CACHE_NOT_FOUND))
    {
      return mmgsdi_status;
    }
  }
  else
  {
    /* for other than EF ENUM acess methods */
    mmgsdi_status = MMGSDI_ERROR;
  }

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);

    /* Cache status indicates that the file is not present on the card */
    if((mmgsdi_status == MMGSDI_NOT_FOUND) &&
       (cache_state == MMGSDI_CACHE_NOT_FOUND))
    {
      return mmgsdi_status;
    }

    if(mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SKIP_UICC_SECURITY_ATTR_CHECK_BEFORE_READ)
         == MMGSDI_FEATURE_DISABLED &&
       prov_app_info_ptr != NULL)
    {
      /*Check to see if the security attributes allow this file to be read*/
      mmgsdi_status = mmgsdi_util_check_file_attr(
                        req_ptr->request_header.session_id,
                        prov_app_info_ptr,
                        &req_ptr->access,
                        MMGSDI_READ_REQ,
                        MMGSDI_DECODE_ACTUAL_ATTRIBUTES);

      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        UIM_MSG_ERR_0("File Read not allowed");
        return mmgsdi_status;
      }
    }

    UIM_MSG_HIGH_1("Cache READ failed. Sending U/ICC READ command to MMGSDI UIM 0x%x",
                   req_ptr->access.file.file_enum);
    /*---------------------------------------------------------------------------
      Read from cache failed
      Send to the card
      ---------------------------------------------------------------------------*/

    switch (req_ptr->file_type)
    {
      case MMGSDI_LINEAR_FIXED_FILE:
      case MMGSDI_CYCLIC_FILE:
        if (protocol == MMGSDI_UICC)
        {
          mmgsdi_status = mmgsdi_uim_uicc_read_record(req_ptr, FALSE, NULL);
        }
        else
        {
          mmgsdi_status = mmgsdi_uim_icc_read_record(req_ptr, FALSE, NULL);
        }
        break;

      case MMGSDI_TRANSPARENT_FILE:
        if (protocol == MMGSDI_UICC)
        {
          mmgsdi_status = mmgsdi_uim_uicc_read_transparent(req_ptr, FALSE, NULL);
        }
        else
        {
          mmgsdi_status = mmgsdi_uim_icc_read_transparent(req_ptr, FALSE, NULL);
        }
        break;

      default:
        return MMGSDI_INCORRECT_PARAMS;
    }

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      mmgsdi_status = mmgsdi_common_read_report_failure_response(req_ptr,
                                                                 protocol,
                                                                 mmgsdi_status);
    }
    return mmgsdi_status;
  } /* end if - data did not come from cache */

  /*---------------------------------------------------------------------------
    Read from cache succeed
      1) Check if security attributes allow file to be read
      2) get client request table index
      3) allocate extra parameter
      4) allocate uim cmd pointer
      5) queue report response
      ---------------------------------------------------------------------------*/

  /*Check to see if the security attributes allow this file to be read*/

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot);

  if(slot_data_ptr != NULL)
  {
    onchip_state = slot_data_ptr->onchip_sim_data.state;
  }

  /* If onchip support is activated, or if the EFS item is set,
     do not check for file attributes */
  if(onchip_state != MMGSDI_ONCHIP_SIM_INIT &&
     prov_app_info_ptr != NULL &&
     mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SKIP_UICC_SECURITY_ATTR_CHECK_BEFORE_READ)
       == MMGSDI_FEATURE_DISABLED)
  {
    mmgsdi_status = mmgsdi_util_check_file_attr(
                      req_ptr->request_header.session_id,
                      prov_app_info_ptr,
                      &req_ptr->access,
                      MMGSDI_READ_REQ,
                      MMGSDI_DECODE_ACTUAL_ATTRIBUTES);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      UIM_MSG_ERR_0("File Read not allowed");
      MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);
      return mmgsdi_status;
    }
  }

  UIM_MSG_HIGH_1("Getting data from cache U/ICC READ command 0x%x",
                 req_ptr->access.file.file_enum);

  /* Data came from cache */
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);
    return mmgsdi_status;
  } /* not successful */

  /*---------------------------------------------------------------------------
    Allocate and Populate Extra Parameter
    - Free Request Table if error in Extra Parameter
    - Free temp data holder if error
  ---------------------------------------------------------------------------*/

  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
    MMGSDI_READ_REQ, (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release temp data  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    Populate Client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                index,
                                                &req_ptr->request_header,
                                                extra_param_ptr );
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release extra param pointer (Since client table has not been
                                    populated yet)
    3) Release temp data  (Since client table has not been populated yet)
    4) Release uim cmd data  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/

    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);
    return mmgsdi_status;
  }

  /* populate the necessary field in the uim_report_type */
  report_ptr.rpt.read.num_bytes_read =
    uint32touint16(temp_data_holder.data_len);
  report_ptr.rpt.read.data           = temp_data_holder.data_ptr;
  report_ptr.rpt_status              = UIM_PASS;

  switch (req_ptr->file_type)
  {
    case MMGSDI_LINEAR_FIXED_FILE:
    case MMGSDI_CYCLIC_FILE:
      report_ptr.rpt_type            = UIM_READ_REC_CMD_R;
      break;

    case MMGSDI_TRANSPARENT_FILE:
      report_ptr.rpt_type            = UIM_READ_BIN_CMD_R;
      break;

    default:
      mmgsdi_util_free_client_request_table_index(index);
      MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);
      return MMGSDI_INCORRECT_PARAMS;
  }

  report_ptr.sw1                     = SW1_NORMAL_END;
  report_ptr.sw2                     = SW2_NORMAL_END;
  report_ptr.cmd_transacted          = FALSE;
  report_ptr.user_data               = index;

  mmgsdi_uim_report(&report_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(temp_data_holder.data_ptr);

  return MMGSDI_SUCCESS;
} /* mmgsdi_common_read */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_WRITE

  DESCRIPTION:
    This function will process the write command to any file

  DEPENDENCIES:


  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                            within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                            but the service table indicates the card does not
                            have the support

  SIDE EFFECTS:

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_write (
  mmgsdi_write_req_type     * req_ptr,
  mmgsdi_protocol_enum_type   protocol
)
{
  mmgsdi_return_enum_type           mmgsdi_status        = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type          slot                 = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_int_app_info_type         *prov_app_info_ptr    = NULL;
  mmgsdi_decoded_attr_cache_type  * attr_cache_ptr       = NULL;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_p                                  - Cannot be NULL
    2. req_p->request_header.response_cb      - Cannot be NULL
    3. req_p->access.access_method            - MMGSDI_EF_ENUM_ACCESS
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  /* Check for protocol */
  if ((protocol != MMGSDI_UICC) && (protocol != MMGSDI_ICC))
  {
    return MMGSDI_ERROR;
  }

  /* Check for access method only */
  switch (req_ptr->access.access_method)
  {
    case MMGSDI_BY_PATH_ACCESS:
    case MMGSDI_EF_ENUM_ACCESS:
      break;

    default:
      MMGSDIUTIL_DEBUG_MSG_ERROR_1("Invalid Access Type 0x%x",
                                 req_ptr->access.access_method);
      return MMGSDI_ERROR;
  }

  if(MMGSDI_SUCCESS != mmgsdi_util_check_file_access_data(
                                            req_ptr->request_header.session_id,
                                            req_ptr->access,
                                            protocol))
  {
    return MMGSDI_ERROR;
  }

  if(mmgsdi_util_get_prov_session_info(req_ptr->request_header.session_id,
                                       NULL,
                                       &slot,
                                       &prov_app_info_ptr) != MMGSDI_SUCCESS)
  {
    prov_app_info_ptr = NULL;
  }

  if (prov_app_info_ptr != NULL)
  {
    /* Check if requested file is being purged */
    if ((MMGSDI_EF_ENUM_ACCESS   == req_ptr->access.access_method)      &&
        ((MMGSDI_GSM_LOCI        == req_ptr->access.file.file_enum) ||
         (MMGSDI_USIM_LOCI       == req_ptr->access.file.file_enum) ||
         (MMGSDI_USIM_PSLOCI     == req_ptr->access.file.file_enum) ||
         (MMGSDI_USIM_EPSLOCI    == req_ptr->access.file.file_enum))    &&
        (mmgsdi_util_is_purge_temp_identities_in_progress(slot))
        )
    {
      UIM_MSG_ERR_1("Request not allowed, file_enum=0x%x is being purged",
                    req_ptr->access.file.file_enum);
      return MMGSDI_ERROR;
    } /* XXLOCI file being purged */

    /*Check to see if the security attributes allow this file to be updated*/
    mmgsdi_status = mmgsdi_util_check_file_attr(
                      req_ptr->request_header.session_id,
                      prov_app_info_ptr,
                      &req_ptr->access,
                      MMGSDI_WRITE_REQ,
                      MMGSDI_DECODE_ACTUAL_ATTRIBUTES);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      UIM_MSG_ERR_0("File Update not allowed");
      return mmgsdi_status;
    }

    /* Check status of EF in cached UST/SST/CST */
    mmgsdi_status = mmgsdi_util_is_file_ok_in_svc_table(
                      req_ptr->request_header.session_id,
                      prov_app_info_ptr->app_data.app_type,
                      &req_ptr->access);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      mmgsdi_util_print_file_information(req_ptr->access);
      UIM_MSG_ERR_1("SERVICE not allowed: error 0x%x", mmgsdi_status);
      return mmgsdi_status;
    }
    /* If Service Table in the card does not have the service enabled,
       return so that File Attr. can be returned from FS cache */
    mmgsdi_status = mmgsdi_file_fs_is_file_ok_in_orig_svc_table(
       req_ptr->request_header.session_id,
       prov_app_info_ptr->app_data.app_type,
       &req_ptr->access);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      return MMGSDI_NOT_FOUND;
    }   
  }

  /* check against cache, directly sends CNF if there's no need to send write to card */
  if (mmgsdi_cache_is_write_data_same_as_cache(req_ptr->request_header.session_id,
                                               &req_ptr->access,
                                               req_ptr->data,
                                               req_ptr->offset,
                                               req_ptr->rec_num))
  {
    mmgsdi_client_req_extra_info_type * extra_param_ptr = NULL;
    mmgsdi_sw_status_type               sw              = {GSDI_SIM_SW1_NORM_END, GSDI_SIM_SW2_NORM_END};

    /* check if the file type in the request matches the actual file type, 
       the check is needed since file cache access does not perform this verification */
    mmgsdi_status = mmgsdi_cache_attr_read(req_ptr->request_header.session_id,
                                           &req_ptr->access,
                                           &attr_cache_ptr,
                                           MMGSDI_SKIP_SECURITY_ATTRIBUTES,
                                           NULL,
                                           NULL,
                                           NULL);

    if(mmgsdi_status == MMGSDI_SUCCESS && attr_cache_ptr != NULL)
    {
      /* Requests to access both Linear fixed and cyclic files are done via 
         Linear Fixed File Type by MMGSDI APIs. Hence, request type of 
         Linear Fixed File and actual file type of cyclic file is not considered 
         a file type mismatch */
      if (attr_cache_ptr->file_type != req_ptr->file_type &&
        !(req_ptr->file_type == MMGSDI_LINEAR_FIXED_FILE && 
          attr_cache_ptr->file_type == MMGSDI_CYCLIC_FILE))
      {
        MMGSDIUTIL_TMC_MEM_FREE(attr_cache_ptr);
        return MMGSDI_EF_INCONSISTENT;
      }
    }

    MMGSDIUTIL_TMC_MEM_FREE(attr_cache_ptr);

    /* Allocate and populate extra parameter */
    mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
            MMGSDI_WRITE_REQ, (void*)req_ptr, &extra_param_ptr);

    if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
    {
      return mmgsdi_status;
    }

    mmgsdi_status = mmgsdi_cnf_build_and_queue(MMGSDI_SUCCESS,
                                               &req_ptr->request_header,
                                               extra_param_ptr,
                                               NULL,
                                               TRUE,
                                               sw);

    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr->write_data.data.data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);

    return mmgsdi_status;
  }

  UIM_MSG_HIGH_1("Sending down WRITE command to MMGSDI UIM 0x%x",
                 req_ptr->access.file.file_enum);
  switch (req_ptr->file_type)
  {
    case MMGSDI_LINEAR_FIXED_FILE:
    case MMGSDI_CYCLIC_FILE:
      /* Delete file attributes and search data also as the content may change as part of Write */
      mmgsdi_pb_free_ef_cache (req_ptr->access.file.path_type,
                               req_ptr->request_header.session_id,
                               req_ptr->request_header.slot_id);

      if (protocol == MMGSDI_ICC)
      {
        mmgsdi_status = mmgsdi_uim_icc_write_record(req_ptr);
      }
      else
      {
        mmgsdi_status = mmgsdi_uim_uicc_write_record(req_ptr, FALSE, NULL);
      }
      break;

    case MMGSDI_TRANSPARENT_FILE:
      mmgsdi_status = mmgsdi_uim_common_write_transparent(req_ptr, FALSE, NULL, protocol);
      break;

    default:
      mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      break;
  }

  return mmgsdi_status;
} /* mmgsdi_common_write */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_INCREASE

  DESCRIPTION:
    This function will provide increase access to any file

  DEPENDENCIES:


  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                            within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                            but the service table indicates the card does not
                            have the support

  SIDE EFFECTS:

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_increase (
  mmgsdi_increase_req_type     * req_ptr,
  mmgsdi_protocol_enum_type      protocol
)
{
  mmgsdi_return_enum_type           mmgsdi_status        = MMGSDI_SUCCESS;
  mmgsdi_int_app_info_type         *prov_app_info_ptr    = NULL;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_p                                  - Cannot be NULL
    2. req_p->request_header.response_cb      - Cannot be NULL
    3. req_p->access.access_method            - MMGSDI_EF_ENUM_ACCESS
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  if (protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /* Check for access method only */
  switch (req_ptr->access.access_method)
  {
    case MMGSDI_BY_PATH_ACCESS:
    case MMGSDI_EF_ENUM_ACCESS:
      break;

    default:
      MMGSDIUTIL_DEBUG_MSG_ERROR_1("Invalid Access Type 0x%x",
                                   req_ptr->access.access_method);
      return MMGSDI_ERROR;
  }

  if(MMGSDI_SUCCESS != mmgsdi_util_check_file_access_data(
                                            req_ptr->request_header.session_id,
                                            req_ptr->access,
                                            protocol))
  {
    return MMGSDI_ERROR;
  }

  if(mmgsdi_util_get_prov_session_info(req_ptr->request_header.session_id,
                                       NULL,
                                       NULL,
                                       &prov_app_info_ptr) != MMGSDI_SUCCESS)
  {
    prov_app_info_ptr = NULL;
  }

  if (prov_app_info_ptr != NULL)
  {
    /*Check to see if the security attributes allow this file to be updated*/
    mmgsdi_status = mmgsdi_util_check_file_attr(
                      req_ptr->request_header.session_id,
                      prov_app_info_ptr,
                      &req_ptr->access,
                      MMGSDI_INCREASE_REQ,
                      MMGSDI_DECODE_ACTUAL_ATTRIBUTES);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      UIM_MSG_ERR_0("File Update not allowed");
      return mmgsdi_status;
    }

    /* Check status of EF in cached UST */
    mmgsdi_status = mmgsdi_util_is_file_ok_in_svc_table(
                      req_ptr->request_header.session_id,
                      prov_app_info_ptr->app_data.app_type,
                      &req_ptr->access );
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      mmgsdi_util_print_file_information(req_ptr->access);
      UIM_MSG_ERR_1("SERVICE not allowed: error 0x%x", mmgsdi_status);
      return mmgsdi_status;
    }
  }

  UIM_MSG_HIGH_1("Sending down INCREASE command to MMGSDI UIM 0x%x",
                 req_ptr->access.file.file_enum);
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_uim_uicc_increase(req_ptr);
  }
  else if (protocol == MMGSDI_ICC)
  {
    mmgsdi_status = mmgsdi_uim_icc_increase(req_ptr);
  }

  return mmgsdi_status;
} /* mmgsdi_common_increase */


/*============================================================================
   FUNCTION:      MMGSDI_COMMON_COMPUTE_IP_AUTH

   DESCRIPTION
     This function is called for a Compute IP Authentication request.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

============================================================================*/
mmgsdi_return_enum_type mmgsdi_common_compute_ip_auth(
  const mmgsdi_compute_ip_req_type        *  req_ptr,
  mmgsdi_protocol_enum_type                  protocol
)
{
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if ((protocol != MMGSDI_ICC) && (protocol != MMGSDI_UICC))
  {
    return MMGSDI_NOT_SUPPORTED;
  }

  return mmgsdi_uim_common_compute_ip_auth(req_ptr, protocol);
}/* mmgsdi_common_compute_ip_auth */


/*============================================================================
   FUNCTION:      MMGSDI_COMMON_RUN_CAVE

   DESCRIPTION
     This function is called for a RUN CAVE ALGORITHM request.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

============================================================================*/
mmgsdi_return_enum_type mmgsdi_common_run_cave(
  const mmgsdi_run_cave_req_type        *  req_ptr,
  mmgsdi_protocol_enum_type                protocol
)
{
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr          - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if (protocol != MMGSDI_ICC && protocol != MMGSDI_UICC)
  {
    return MMGSDI_NOT_SUPPORTED;
  }

  return mmgsdi_uim_common_run_cave(req_ptr, protocol);
} /* mmgsdi_common_run_cave */


/*============================================================================
   FUNCTION:      MMGSDI_COMMON_CDMA_AKA_AUTH

   DESCRIPTION
     This function is called for a CDMA AKA Authenticate request.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

============================================================================*/
mmgsdi_return_enum_type mmgsdi_common_cdma_aka_auth(
  const mmgsdi_cdma_aka_req_type        *  req_ptr,
  mmgsdi_protocol_enum_type                protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  boolean                        cdma_aka_avail = FALSE;
  mmgsdi_return_enum_type        mmgsdi_status  = MMGSDI_ERROR;
  mmgsdi_session_type_enum_type  session_type   = MMGSDI_MAX_SESSION_TYPE_ENUM;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
    2. req_ptr->request_header.response_cb      - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  if (protocol != MMGSDI_ICC && protocol != MMGSDI_UICC)
  {
    return MMGSDI_NOT_SUPPORTED;
  }

  mmgsdi_status = mmgsdi_util_get_session_type(req_ptr->request_header.session_id,
                                               &session_type, NULL);

  if(mmgsdi_status != MMGSDI_SUCCESS ||
     (session_type != MMGSDI_1X_PROV_PRI_SESSION &&
      session_type != MMGSDI_1X_PROV_SEC_SESSION &&
      session_type != MMGSDI_1X_PROV_TER_SESSION ))
  {
    UIM_MSG_ERR_1("Invalid session type: %d",session_type);
    return MMGSDI_ERROR;
  }

  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_uicc_chk_srv_available(req_ptr->request_header.session_id,
                                                  MMGSDI_CHK_SRV_FROM_CARD_OR_CACHE,
                                                  MMGSDI_CSIM_SRV_AKA,
                                                  &cdma_aka_avail);
  }
  else
  {
    mmgsdi_status = mmgsdi_icc_chk_srv_available(req_ptr->request_header.session_id,
                                                 MMGSDI_CHK_SRV_FROM_CARD_OR_CACHE,
                                                 MMGSDI_CDMA_SRV_AKA,
                                                 &cdma_aka_avail);
  }

  if (mmgsdi_status != MMGSDI_SUCCESS || cdma_aka_avail == FALSE)
  {
    UIM_MSG_ERR_2("CDMA AKA AUTH service check failed. status: %d, srv avail: %d",
                  mmgsdi_status,cdma_aka_avail);
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_cdma_aka_auth(req_ptr, protocol);
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_cdma_aka_auth */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_GENERATE_KEY_VPM

  DESCRIPTION:
    This function will allow GENERATE KEY / VPM command to be sent to UIM

  DEPENDENCIES:


  LIMITATIONS:
    None

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type mmgsdi_common_generate_key_vpm (
  const mmgsdi_generate_key_req_type   *req_ptr,
  mmgsdi_protocol_enum_type             protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
    2. req_ptr->request_header.response_cb      - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  if (protocol != MMGSDI_ICC && protocol != MMGSDI_UICC)
  {
    return MMGSDI_NOT_SUPPORTED;
  }

  return mmgsdi_uim_common_generate_key_vpm(req_ptr, protocol);
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_generate_key_vpm */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_OTASP_BLOCK

  DESCRIPTION:
    This function will allow OTASP Block Request command to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application


  LIMITATIONS:
    None

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_otasp_block (
  const mmgsdi_session_otasp_block_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                      protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  if (protocol != MMGSDI_ICC && protocol != MMGSDI_UICC)
  {
    return MMGSDI_NOT_SUPPORTED;
  }

  return mmgsdi_uim_common_otasp_block(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_otasp_block */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_OTASP_OTAPA

  DESCRIPTION:
    This function will allow OTAPA Request command to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_otasp_otapa (
  const mmgsdi_session_otasp_otapa_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                      protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /* No need to check callback,
     already checked when preparing mmgsdi command*/
  /* No need to check randseed,
     already checked when preparing mmgsdi command */
  if (protocol == MMGSDI_UICC && 0 == req_ptr->esn)
  {
    UIM_MSG_ERR_0("ESN value should not be 0x00 for UICC OTAPA Request");
    return MMGSDI_ERROR;
  }

  UIM_MSG_HIGH_2("Processing MMGSDI_SESSION_OTASP_OTAPA_REQ,Protocol: 0x%x, Slot:0x%x",
                 protocol,req_ptr->request_header.slot_id);
  return mmgsdi_uim_common_otasp_otapa(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_otasp_otapa */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_OTASP_COMMIT

  DESCRIPTION:
    This function will allow OTASP Commit Request to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_otasp_commit (
  const mmgsdi_session_otasp_commit_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                       protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_otasp_commit(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_otasp_commit */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_OTASP_SSPR_CONFIG

  DESCRIPTION:
    This function will allow OTASP SSPR Config Request to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_otasp_sspr_config (
  const mmgsdi_session_otasp_sspr_config_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                            protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_otasp_sspr_config(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_otasp_sspr_config */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_OTASP_KEY_GEN

  DESCRIPTION:
    This function will allow OTASP KEY GEN Request to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_otasp_key_gen (
  const mmgsdi_session_otasp_key_gen_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                        protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_otasp_key_gen(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_otasp_key_gen */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_OTASP_MS_KEY

  DESCRIPTION:
    This function will allow OTASP MS Key Request to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_otasp_ms_key (
  const mmgsdi_session_otasp_ms_key_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                       protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_otasp_ms_key(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_otasp_ms_key */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_BS_CHAL

  DESCRIPTION:
    This function will allow BS CHAL Request to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_bs_chal (
  const mmgsdi_session_bs_chal_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                  protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_bs_chal(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_bs_chal */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_SSD_CONFIRM

  DESCRIPTION:
    This function will allow SSD Confirm Request to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_ssd_confirm (
  const mmgsdi_session_ssd_confirm_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                      protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_ssd_confirm(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_ssd_confirm */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_SSD_UPDATE

  DESCRIPTION:
    This function will allow SSD Update Request to be sent to UIM

  DEPENDENCIES:
    If the application is any application other than default, then the session
    should have been opened with that application

  LIMITATIONS:

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_ssd_update (
  const mmgsdi_session_ssd_update_req_type    * req_ptr,
  mmgsdi_protocol_enum_type                     protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_uim_common_ssd_update(req_ptr, protocol);
#else
  (void)req_ptr;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_common_ssd_update */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_PIN_OPERATION

  DESCRIPTION:
    This function will perform pin operations

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                            within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                            but the service table indicates the card does not
                            have the support

  SIDE EFFECTS:

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_pin_operation (
  const mmgsdi_pin_operation_req_type     * req_ptr,
  mmgsdi_protocol_enum_type                 protocol
)
{
  mmgsdi_return_enum_type    mmgsdi_status     = MMGSDI_ERROR;
  mmgsdi_int_app_info_type  *sel_app_ptr       = NULL;
  uint32                     pin_index         = 0;
  mmgsdi_app_pin_info_type **app_pin1_tbl_pptr = NULL;
  uint8                      app_pin1_tbl_size = 0;
  mmgsdi_session_info_type  *session_info_ptr  = NULL;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_p                                  - Cannot be NULL
    2. req_p->request_header.response_cb      - Cannot be NULL
    3. req_p->access.access_method            - MMGSDI_EF_ENUM_ACCESS
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  if (protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /* If request is for a Hiddenkey Operation */
  if(req_ptr->pin_id == MMGSDI_HIDDENKEY)
  {
    /* Hiddenkey is supported only on UICC */
    if(protocol == MMGSDI_UICC)
    {
      mmgsdi_status = mmgsdi_hidden_key_operation(req_ptr);
    } /* protocol is MMGSDI_UICC */
    else
    {
      UIM_MSG_ERR_0("Hiddenkey operation not supported for non-UICC protocols");
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
    } /* protocol is MMGSDI_ICC */
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_ICC &&
      (req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_1 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_2 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_3 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_4 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_5 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_6 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_7 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_8 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_1 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_2 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_3 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_4 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_5 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_6 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_7 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_8))
  {
    UIM_MSG_ERR_1("Key ref based PIN input 0x%x not supported for ICC mode",
                  req_ptr->orig_pin_id);
    return MMGSDI_INCORRECT_PARAMS;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(req_ptr->request_header.session_id,
                                                   NULL,
                                                   NULL,
                                                   &sel_app_ptr,
                                                   NULL,
                                                   &session_info_ptr,
                                                   NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS || sel_app_ptr == NULL ||
      session_info_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* Block PIN operations for non-provisioning apps in ICC mode because
     MMGSDI has valid PIN info only for prov sessions. Streaming APDUs
     should be used to perform such PIN operations. */
  if(protocol == MMGSDI_ICC &&
     mmgsdi_util_is_non_prov_session(session_info_ptr->session_type))
  {
    UIM_MSG_ERR_1("PIN OP not supported for session type 0x%x in non-UICC mode",
                  session_info_ptr->session_type);
    return MMGSDI_NOT_SUPPORTED;
  }

  if(protocol == MMGSDI_UICC &&
     req_ptr->pin_id == MMGSDI_UNIVERSAL_PIN &&
     sel_app_ptr->universal_pin_ptr != NULL &&
     sel_app_ptr->universal_pin_ptr->status == MMGSDI_PIN_DISABLED &&
     req_ptr->pin_op != MMGSDI_PIN_OP_ENABLE)
  {
    UIM_MSG_ERR_0("UPIN is not in use or disabled - PIN operation skipped");
    return MMGSDI_ERROR;
  }

  if (protocol == MMGSDI_UICC &&
      sel_app_ptr->fcp_pin_data_absent &&
      (req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_1 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_2 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_3 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_4 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_5 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_6 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_7 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_PIN_APPL_8))
  {
    mmgsdi_key_ref_values_enum_type   input_key_ref = MMGSDI_PIN_APPL_1;

    switch (req_ptr->orig_pin_id)
    {
      case MMGSDI_KEY_REF_PIN_APPL_1:
        input_key_ref = MMGSDI_PIN_APPL_1;
        break;
      case MMGSDI_KEY_REF_PIN_APPL_2:
        input_key_ref = MMGSDI_PIN_APPL_2;
        break;
      case MMGSDI_KEY_REF_PIN_APPL_3:
        input_key_ref = MMGSDI_PIN_APPL_3;
        break;
      case MMGSDI_KEY_REF_PIN_APPL_4:
        input_key_ref = MMGSDI_PIN_APPL_4;
        break;
      case MMGSDI_KEY_REF_PIN_APPL_5:
        input_key_ref = MMGSDI_PIN_APPL_5;
        break;
      case MMGSDI_KEY_REF_PIN_APPL_6:
        input_key_ref = MMGSDI_PIN_APPL_6;
        break;
      case MMGSDI_KEY_REF_PIN_APPL_7:
        input_key_ref = MMGSDI_PIN_APPL_7;
        break;
      case MMGSDI_KEY_REF_PIN_APPL_8:
        input_key_ref = MMGSDI_PIN_APPL_8;
        break;
      default:
        break;
    }
    if (sel_app_ptr->pin1_ptr->key_ref != input_key_ref)
    {
      UIM_MSG_HIGH_0("PIN1 keyref switch as req keyref mismatch with curr keyref");

      app_pin1_tbl_pptr = mmgsdi_util_get_app_pin1_info_tbl_ptr(&app_pin1_tbl_size,
                                                                req_ptr->request_header.slot_id);
      if(app_pin1_tbl_pptr == NULL ||
         app_pin1_tbl_size == 0)
      {
        UIM_MSG_ERR_0("Could not get PIN1 table");
        return MMGSDI_ERROR;
      }

      /* Ignoring return value because app pin table is fixed and pin index for
         a particular key ref will always be found */
      (void)mmgsdi_util_find_app_pin_index((const mmgsdi_app_pin_info_type **)app_pin1_tbl_pptr,
                                           app_pin1_tbl_size,
                                           input_key_ref,
                                           &pin_index);
      sel_app_ptr->pin1_ptr = app_pin1_tbl_pptr[pin_index];
    }
  }

  if (protocol == MMGSDI_UICC &&
      sel_app_ptr->fcp_pin_data_absent &&
      (req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_1 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_2 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_3 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_4 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_5 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_6 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_7 ||
       req_ptr->orig_pin_id == MMGSDI_KEY_REF_SEC_PIN_APPL_8))
  {
    mmgsdi_key_ref_values_enum_type   input_key_ref = MMGSDI_SEC_PIN_APPL_1;
    switch (req_ptr->orig_pin_id)
    {
      case MMGSDI_KEY_REF_SEC_PIN_APPL_1:
        input_key_ref = MMGSDI_SEC_PIN_APPL_1;
        break;
      case MMGSDI_KEY_REF_SEC_PIN_APPL_2:
        input_key_ref = MMGSDI_SEC_PIN_APPL_2;
        break;
      case MMGSDI_KEY_REF_SEC_PIN_APPL_3:
        input_key_ref = MMGSDI_SEC_PIN_APPL_3;
        break;
      case MMGSDI_KEY_REF_SEC_PIN_APPL_4:
        input_key_ref = MMGSDI_SEC_PIN_APPL_4;
        break;
      case MMGSDI_KEY_REF_SEC_PIN_APPL_5:
        input_key_ref = MMGSDI_SEC_PIN_APPL_5;
        break;
      case MMGSDI_KEY_REF_SEC_PIN_APPL_6:
        input_key_ref = MMGSDI_SEC_PIN_APPL_6;
        break;
      case MMGSDI_KEY_REF_SEC_PIN_APPL_7:
        input_key_ref = MMGSDI_SEC_PIN_APPL_7;
        break;
      case MMGSDI_KEY_REF_SEC_PIN_APPL_8:
        input_key_ref = MMGSDI_SEC_PIN_APPL_8;
        break;
      default:
        break;
    }

    if (sel_app_ptr->pin2.key_ref != input_key_ref)
    {
      UIM_MSG_HIGH_0("PIN2 keyref switch as req keyref mismatch with curr keyref");
      sel_app_ptr->pin2.key_ref = input_key_ref;
    }
  }

  if(protocol == MMGSDI_UICC && req_ptr->pin_id == MMGSDI_PIN2)
  {
    if(MMGSDI_SUCCESS == mmgsdi_util_select_adf_before_pin2_op(
          req_ptr->request_header.session_id))
    {
      UIM_MSG_HIGH_0("Selected ADF successfully prior to PIN2 operation");
    }
  }

  /* Check for access method only */
  switch(req_ptr->pin_op)
  {
    case MMGSDI_PIN_OP_VERIFY:
    case MMGSDI_PIN_OP_UNBLOCK:
      mmgsdi_status = mmgsdi_uim_common_unblock_verify_pin(req_ptr,
                                                           MMGSDI_RETRY_NONE,
                                                           protocol,
                                                           FALSE);
      break;
    case MMGSDI_PIN_OP_ENABLE:
    case MMGSDI_PIN_OP_DISABLE_AND_REPLACE:
    case MMGSDI_PIN_OP_DISABLE:
      if (protocol == MMGSDI_ICC && req_ptr->pin_op != MMGSDI_PIN_OP_ENABLE)
      {
        if (req_ptr->pin_op == MMGSDI_PIN_OP_DISABLE_AND_REPLACE)
        {
          return MMGSDI_INCORRECT_PARAMS;
        }

        if (mmgsdi_icc_is_disable_chv1_allowed(
              req_ptr->request_header.session_id) == FALSE)
        {
          return MMGSDI_NOT_SUPPORTED;
        }
      }
      mmgsdi_status = mmgsdi_uim_common_enable_disable_pin(req_ptr, protocol);
      break;
    case MMGSDI_PIN_OP_CHANGE:
      mmgsdi_status = mmgsdi_uim_common_change_pin(req_ptr, protocol);
      break;
    default:
      return MMGSDI_INCORRECT_PARAMS;
  }
  return mmgsdi_status;
} /* mmgsdi_common_pin_operation */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_PURGE_TEMPORARY_IDENTITY_GUTI

  DESCRIPTION:
    This function will purge the GUTI

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                             but the service table indicates the card does not
                             have the support

  SIDE EFFECTS:
    None

===========================================================================*/
static mmgsdi_return_enum_type  mmgsdi_common_purge_temporary_identity_guti (
  const mmgsdi_purge_temporary_identity_req_type     * req_ptr,
  mmgsdi_protocol_enum_type                            protocol
)
{
  mmgsdi_return_enum_type               mmgsdi_status        = MMGSDI_SUCCESS;
  boolean                               eps_mmi_avail        = FALSE;
  mmgsdi_int_app_info_type            * prov_app_info_ptr    = NULL;
  mmgsdi_slot_id_enum_type              slot_id              = MMGSDI_MAX_SLOT_ID_ENUM;

  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, mmgsdi_generic_data_ptr);

  if(mmgsdi_generic_data_ptr->is_oprt_mode_online ||
     protocol == MMGSDI_NO_PROTOCOL)
  {
    UIM_MSG_HIGH_2("OPRT_MODE is 0x%x and protocol 0x%x",
                   mmgsdi_generic_data_ptr->is_oprt_mode_online,
                   protocol);
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_util_get_prov_session_info(req_ptr->request_header.session_id,
                                                    NULL,
                                                    &slot_id,
                                                    &prov_app_info_ptr);

  if((mmgsdi_status != MMGSDI_SUCCESS) ||
     (prov_app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if(prov_app_info_ptr->app_data.app_type != MMGSDI_APP_USIM ||
     prov_app_info_ptr->pin1_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  if(prov_app_info_ptr->pending_purge_guti)
  {
    return MMGSDI_SUCCESS;
  }

  /* If PIN is not verified store purge data in global and process it again after SUB_READY */
  switch(prov_app_info_ptr->pin1_ptr->status)
  {
    case MMGSDI_PIN_DISABLED:
    case MMGSDI_PIN_ENABLED_VERIFIED:
      break;

    case MMGSDI_PIN_ENABLED_NOT_VERIFIED:
    case MMGSDI_PIN_BLOCKED:
      prov_app_info_ptr->pending_purge_guti = TRUE;
      return MMGSDI_SUCCESS;

    default:
      return MMGSDI_ERROR;
  }

  switch (protocol)
  {
    case MMGSDI_ICC:
      return MMGSDI_INCORRECT_PARAMS;

    case MMGSDI_UICC:
      /* Purge EF_EPSLOCI and EF_EPSNSC if EPS MMI service is available */
      if((MMGSDI_SUCCESS == mmgsdi_uicc_chk_srv_available(req_ptr->request_header.session_id,
                                                          MMGSDI_CHK_SRV_FROM_CARD_OR_CACHE,
                                                          MMGSDI_USIM_SRV_EPS_MMI,
                                                          &eps_mmi_avail))
         && eps_mmi_avail)
      {
        (void) mmgsdi_util_purge_temp_identities(slot_id,
                                                 req_ptr->request_header.session_id,
                                                 MMGSDI_USIM_EPSLOCI,
                                                 protocol);

        (void) mmgsdi_util_purge_temp_identities(slot_id,
                                                 req_ptr->request_header.session_id,
                                                 MMGSDI_USIM_EPSNSC,
                                                 protocol);
      }
      break;

    default:
      return MMGSDI_ERROR;
  }

  /* call NAS API for purging temporary identity */
  mmgsdi_status = mmgsdi_send_purge_req_to_nas(req_ptr);

  return mmgsdi_status;
} /* mmgsdi_common_purge_temporary_identity_guti */


/*===========================================================================
  FUNCTION:      MMGSDI_COMMON_PURGE_TEMPORARY_IDENTITY

  DESCRIPTION:
    This function will purge the temporary identity

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                             but the service table indicates the card does not
                             have the support

  SIDE EFFECTS:
    None

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_common_purge_temporary_identity (
  const mmgsdi_purge_temporary_identity_req_type     * req_ptr,
  mmgsdi_protocol_enum_type                            protocol
)
{
  mmgsdi_return_enum_type           mmgsdi_status        = MMGSDI_SUCCESS;
  mmgsdi_sw_status_type             status_word          = {0};
  boolean                           send_event           = TRUE;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr                                  - Cannot be NULL
    2. req_ptr->request_header.response_cb      - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  switch (req_ptr->temporary_identity_req_state)
  {
    case MMGSDI_PURGE_TEMPORARY_IDENTITY_SOURCE_CLIENT_RSP:
      /* If client_status is TRUE which indicate that client has process the
         purge GUTI successfully */
      if(req_ptr->client_status == FALSE)
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;

    case MMGSDI_PURGE_TEMPORARY_IDENTITY_SOURCE_REQ:
      switch (req_ptr->temporary_identity)
      {
        case MMGSDI_PURGE_TEMPORARY_IDENTITY_GUTI:
          mmgsdi_status = mmgsdi_common_purge_temporary_identity_guti(req_ptr, protocol);
          if(mmgsdi_status == MMGSDI_SUCCESS)
          {
            send_event = FALSE;
          }
          break;

        default:
          mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
          break;
      }
      (void) mmgsdi_cnf_build_and_queue(mmgsdi_status,
                                        &req_ptr->request_header,
                                        NULL,
                                        NULL,
                                        FALSE,
                                        status_word);
      break;
  }

  if(send_event)
  {
    mmgsdi_slot_id_enum_type    slot_id              = MMGSDI_SLOT_AUTOMATIC;
    mmgsdi_int_app_info_type  * prov_app_info_ptr    = NULL;

    mmgsdi_status = mmgsdi_util_get_prov_session_info(req_ptr->request_header.session_id,
                                                      NULL,
                                                      &slot_id,
                                                      &prov_app_info_ptr);

    if(prov_app_info_ptr != NULL)
    {
      prov_app_info_ptr->pending_purge_guti =  FALSE;
    }

    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* send purge event to client */
      mmgsdi_evt_build_and_send_purge_temporary_identity_status(req_ptr->request_header.session_id,
                                                                req_ptr->temporary_identity,
                                                                mmgsdi_status,
                                                                slot_id);
    }
  }

  return mmgsdi_status;
} /* mmgsdi_common_purge_temporary_identity */
