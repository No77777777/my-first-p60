
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


               M M G S D I   U I M   C O M M O N   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the protocol processing support that is common
  to both UICC and ICC protocols before the command is sent to UIM Module.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2017, 2019 - 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_uim_common.c#5 $$ $DateTime: 2021/09/09 04:03:08 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/21   rps     Add support for GSM FPLMN in fs fallback
03/03/20   cj      Input validation in IP Authentication Data
01/02/20   sch     Added support for log masking
03/26/19   tq      Do not expect app info ptr for card session while writing
06/08/17   nr      Do not apply FPLMN optimization when onchip is enabled
05/08/17   tq      SIM profile Query should return error if SIM is not present
04/14/17   nr      Support to store more FPLMNs in cache for MMGSDI_USIM_FPLMN
03/29/17   nr      Add support to cache PIN1 of the SIM card when PSM mode enabled
03/14/17   nc      Better checks for return values 
01/19/17   nc      F3 log masking 
10/24/16   vdc     Added support for auto reg SMS when ESN is changed
05/30/16   ar      Removing string prints from APDU and AUTH
05/13/16   tkl     Review of macros used by MMGSDI
05/16/16   ar      F3 reduction
04/14/16   ar      Snip out 3GPP2 code in new file
10/16/15   sp      Move client_req_table_info index variables to unsigned
10/05/15   sp      Reduce signed/unsigned conversions in UIM
08/27/15   yt      Check retry type before sending UNBLCOK PIN command to UIM
08/26/15   yt      Reduce duplicate code for PIN operations
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
07/29/15   sp      Remove UIM command in client request table
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
04/22/14   at      Critical KW fixes
01/22/14   yt      Initial version

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "intconv.h"
#include "mmgsdi_uim_common.h"
#include "uim.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "mmgsdi_common_rsp.h"
#include "mmgsdi_logging_mask.h"
#include "mmgsdi_nv.h"
#include "mmgsdi_refresh.h"
#include "mmgsdi_file.h"


/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*============================================================================
   FUNCTION:      MMGSDI_UIM_COMMON_COMPUTE_IP_AUTH

   DESCRIPTION
     This function is called to send Compute IP Auth request to UIM.

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
mmgsdi_return_enum_type mmgsdi_uim_common_compute_ip_auth(
  const mmgsdi_compute_ip_req_type        *  req_ptr,
  mmgsdi_protocol_enum_type                  protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_SUCCESS;
  uim_cmd_type *             uim_cmd_ptr      = NULL;
  uim_slot_type              uim_slot         = UIM_SLOT_NONE;
  uint32                     index            = 0;
  mmgsdi_slot_id_enum_type   mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_int_app_info_type  *app_info_ptr     = NULL;
  mmgsdi_channel_id_type     channel_id       = MMGSDI_DEFAULT_CHANNEL;
  uim_channel_type           uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr          - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }
  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot,&uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    /* Convert to UIM Channel ID */
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /* Get a UIM buffer for the request */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  uim_cmd_ptr->hdr.command               = UIM_COMPUTE_IP_AUTH_F;
  uim_cmd_ptr->hdr.slot                  = uim_slot;
  uim_cmd_ptr->hdr.channel               = uim_channel;
  uim_cmd_ptr->hdr.protocol              = (protocol == MMGSDI_UICC) ?
                                           UIM_UICC : UIM_CDMA;

  /*---------------------------------------------------------------------------
  CLIENT REQUEST TABLE :- Store information
  1) Get the index
  2) If not able to get the index, free the uim_cmd_q, but keep the request
  ptr and the uim_rsp_data buffer so that it can be freed when processing the
  response
  3) Populate client request table with req_ptr, data len and uim_rsp_data buffer
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }
  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                &req_ptr->request_header,
                                                NULL);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* ------------------------------------------------------------------------
      Set COMPUTE IP Authentication data
    ----------------------------------------------------------------------*/
  switch(req_ptr->compute_ip_data.operation_type)
  {
    case MMGSDI_CMPT_IP_SIP_CHAP:
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_operation = UIM_CMPT_IP_CHAP;
      if (int32touint8(req_ptr->compute_ip_data.data.sip_chap_data.chap_challenge_length) <= UIM_MAX_3GPD_CHAP_CHALLENGE_LENGTH)
      {
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.chap_data.chap_challenge_length =
        uint32touint8(req_ptr->compute_ip_data.data.sip_chap_data.chap_challenge_length);
      }
      else
      {
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.chap_data.chap_challenge_length = UIM_MAX_3GPD_CHAP_CHALLENGE_LENGTH;
      }
      mmgsdi_memscpy(
        (void*)uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.chap_data.chap_challenge,
        sizeof(uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.chap_data.chap_challenge),
        (void*)req_ptr->compute_ip_data.data.sip_chap_data.chap_challenge,
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.chap_data.chap_challenge_length);
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.chap_data.chap_id =
        req_ptr->compute_ip_data.data.sip_chap_data.chap_id;
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.chap_data.nai_entry_index =
        req_ptr->compute_ip_data.data.sip_chap_data.nai_entry_index;
      break;

    case MMGSDI_CMPT_IP_MIP_MN_HA:
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_operation = UIM_CMPT_IP_MN_HA_AUTH;
      if (int32touint16(req_ptr->compute_ip_data.data.mn_ha_data.registration_data_length) <= UIM_MAX_3GPD_MN_HA_REG_DATA_LENGTH)
      {
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_ha_data.registration_data_length =
        uint32touint16(req_ptr->compute_ip_data.data.mn_ha_data.registration_data_length);
      }
      else
      {
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_ha_data.registration_data_length = UIM_MAX_3GPD_MN_HA_REG_DATA_LENGTH;
      }
      mmgsdi_memscpy(
        (void*)uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_ha_data.registration_data,
        sizeof(uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_ha_data.registration_data),
        (void*)req_ptr->compute_ip_data.data.mn_ha_data.registration_data_ptr,
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_ha_data.registration_data_length);
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_ha_data.nai_entry_index =
        req_ptr->compute_ip_data.data.mn_ha_data.nai_entry_index;
      break;

    case MMGSDI_CMPT_IP_MIP_RRQ:
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_operation = UIM_CMPT_IP_MIP_RRQ;
      if (int32touint16(req_ptr->compute_ip_data.data.rrq_data.rrq_data_length) <= UIM_MAX_3GPD_HASH_RRQ_DATA_LENGTH)
      {
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mip_rrq_data.rrq_data_length =
        uint32touint16(req_ptr->compute_ip_data.data.rrq_data.rrq_data_length);
      }
      else
      {
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mip_rrq_data.rrq_data_length = UIM_MAX_3GPD_HASH_RRQ_DATA_LENGTH;
      }
      mmgsdi_memscpy(
        (void*)uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mip_rrq_data.rrq_data,
        sizeof(uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mip_rrq_data.rrq_data),
        (void*)req_ptr->compute_ip_data.data.rrq_data.rrq_data_ptr,
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mip_rrq_data.rrq_data_length);
      break;

    case MMGSDI_CMPT_IP_MIP_MN_AAA:
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_operation = UIM_CMPT_IP_MN_AAA_AUTH;
      if (int32touint8(req_ptr->compute_ip_data.data.mn_aaa_data.challenge_length) <= UIM_3GPD_MIP_CHALLENGE_LENGTH)
      {
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_aaa_data.challenge_length =
        uint32touint8(req_ptr->compute_ip_data.data.mn_aaa_data.challenge_length);
      }
      else
      {
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_aaa_data.challenge_length = UIM_3GPD_MIP_CHALLENGE_LENGTH;
      }
      mmgsdi_memscpy(
        (void*)uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_aaa_data.challenge,
        sizeof(uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_aaa_data.challenge),
        (void*)req_ptr->compute_ip_data.data.mn_aaa_data.challenge_ptr,
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_aaa_data.challenge_length);
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.mn_aaa_data.nai_entry_index =
        req_ptr->compute_ip_data.data.mn_aaa_data.nai_entry_index;
      break;
    case MMGSDI_CMPT_IP_HRPD_CHAP:
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_operation = UIM_CMPT_IP_HRPD_AN_CHAP;
      if (int32touint8(req_ptr->compute_ip_data.data.hrpd_chap_data.chap_challenge_length) <= UIM_MAX_HRPD_CHAP_CHALLENGE_LENGTH)
      {
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.hrpd_data.chap_challenge_length =
        uint32touint8(req_ptr->compute_ip_data.data.hrpd_chap_data.chap_challenge_length);
      }
      else
      {
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.hrpd_data.chap_challenge_length = UIM_MAX_HRPD_CHAP_CHALLENGE_LENGTH;
      }
      mmgsdi_memscpy(
        (void*)uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.hrpd_data.chap_challenge,
        sizeof(uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.hrpd_data.chap_challenge),
        (void*)req_ptr->compute_ip_data.data.hrpd_chap_data.chap_challenge_ptr,
        uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.hrpd_data.chap_challenge_length);
      uim_cmd_ptr->compute_ip_auth.cmpt_ip_data.hrpd_data.chap_id =
        req_ptr->compute_ip_data.data.hrpd_chap_data.chap_id;
      break;

    default:
      UIM_MSG_ERR_0("Invalid operation type");
      mmgsdi_util_free_client_request_table_index(index);
      return MMGSDI_ERROR;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_compute_ip_auth */


/*============================================================================
   FUNCTION:      MMGSDI_UIM_COMMON_RUN_CAVE

   DESCRIPTION
     This function is called to send RUN CAVE ALGORITHM request to UIM.

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
mmgsdi_return_enum_type mmgsdi_uim_common_run_cave(
  const mmgsdi_run_cave_req_type        *  req_ptr,
  mmgsdi_protocol_enum_type                protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_SUCCESS;
  uim_cmd_type *             uim_cmd_ptr      = NULL;
  uim_slot_type              uim_slot         = UIM_SLOT_NONE;
  uint32                     index            = 0;
  mmgsdi_cave_esn_type*      temp_esn_ptr     = NULL;
  mmgsdi_slot_id_enum_type   mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_int_app_info_type  *app_info_ptr     = NULL;
  mmgsdi_channel_id_type     channel_id       = MMGSDI_DEFAULT_CHANNEL;
  uim_channel_type           uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr          - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }
  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot,&uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    /* Convert to UIM Channel ID */
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /* Get a UIM buffer for the request */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  uim_cmd_ptr->hdr.command               = UIM_RUN_CAVE_F;
  uim_cmd_ptr->hdr.slot                  = uim_slot;
  uim_cmd_ptr->hdr.channel               = uim_channel;
  uim_cmd_ptr->hdr.protocol              = (protocol == MMGSDI_UICC) ?
                                           UIM_UICC : UIM_CDMA;

  /* ------------------------------------------------------------------------
      Set RUN CAVE Algo data
    ----------------------------------------------------------------------*/

  uim_cmd_ptr->run_cave.dig_len             = req_ptr->dig_len;
  /* req_ptr->digits is a an array of uint8s */
  mmgsdi_memscpy(uim_cmd_ptr->run_cave.digits,
                 sizeof(uim_cmd_ptr->run_cave.digits),
                 req_ptr->digits,
                 sizeof(mmgsdi_cave_digits_type));
  /* Need to move data to temp variable since qw_esn can't take const variable. We
     are not using & of esn type in assignment since it is originally defined as static
     array type in qw.h  */
  temp_esn_ptr = (mmgsdi_cave_esn_type*)(req_ptr->esn);
  qw_equ(uim_cmd_ptr->run_cave.esn, (*temp_esn_ptr));
  uim_cmd_ptr->run_cave.process_control     = req_ptr->process_control;
  uim_cmd_ptr->run_cave.rand_chal           = req_ptr->rand_chal;
  uim_cmd_ptr->run_cave.rand_type           = req_ptr->rand_type;

  /*---------------------------------------------------------------------------
  CLIENT REQUEST TABLE :- Store information
  1) Get the index
  2) If not able to get the index, free the uim_cmd_q, but keep the request
  ptr and the uim_rsp_data buffer so that it can be freed when processing the
  response
  3) Populate client request table with req_ptr, data len and uim_rsp_data buffer
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }
  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                &req_ptr->request_header,
                                                NULL);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  /* Populate masking function pointer */
  if (app_info_ptr != NULL) 
  {
    mmgsdi_logging_populate_masking_fn_ptr(uim_cmd_ptr,
                                           app_info_ptr->app_data.app_type);  
  }

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_run_cave */


/*============================================================================
   FUNCTION:      MMGSDI_UIM_COMMON_CDMA_AKA_AUTH

   DESCRIPTION
     This function is called to send CDMA AKA Authentication request to UIM.

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
mmgsdi_return_enum_type mmgsdi_uim_common_cdma_aka_auth(
  const mmgsdi_cdma_aka_req_type        *  req_ptr,
  mmgsdi_protocol_enum_type                protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_SUCCESS;
  uim_cmd_type *             uim_cmd_ptr      = NULL;
  uim_slot_type              uim_slot         = UIM_SLOT_NONE;
  uint32                     index            = 0;
  mmgsdi_slot_id_enum_type   mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_int_app_info_type  *app_info_ptr     = NULL;
  mmgsdi_channel_id_type     channel_id       = MMGSDI_DEFAULT_CHANNEL;
  uim_channel_type           uim_channel      = UIM_CHANNEL0;

  UIM_MSG_HIGH_0("MMGSDI UIM COMMON CDMA AKA AUTH");

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr            - Cannot be NULL
    2. auth token pointer - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr,
                              req_ptr->autn.data_ptr);

  if (protocol != MMGSDI_ICC && protocol != MMGSDI_UICC)
  {
    return MMGSDI_NOT_SUPPORTED;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }
  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot,&uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    /* Convert to UIM Channel ID */
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  if (req_ptr->autn.data_len > UIM_MAX_AKA_AUTH_BYTES)
  {
    UIM_MSG_ERR_1("Invalid authentication token length: 0x%x",req_ptr->autn.data_len);
    return MMGSDI_ERROR;
  }

  /* Get a UIM buffer for the request */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  uim_cmd_ptr->hdr.command               = UIM_AKA_AUTH_F;
  uim_cmd_ptr->hdr.slot                  = uim_slot;
  uim_cmd_ptr->hdr.channel               = uim_channel;
  uim_cmd_ptr->hdr.protocol              = (protocol == MMGSDI_UICC) ?
                                           UIM_UICC : UIM_ICC;

  /* ------------------------------------------------------------------------
      Set CDMA AKA Authentication data
    ----------------------------------------------------------------------*/
  switch (req_ptr->aka_cmd)
  {
    case MMGSDI_CDMA_AKA_AUTH_3G:
      uim_cmd_ptr->aka_auth.op_type = UIM_AKA_AUTH_3G;
      break;
    case MMGSDI_CDMA_AKA_AUTH_WLAN:
      uim_cmd_ptr->aka_auth.op_type = UIM_AKA_AUTH_WLAN;
      break;
    default:
      UIM_MSG_ERR_1("Invalid CDMA AKA authentication op type: 0x%x", req_ptr->aka_cmd);
      MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
      return MMGSDI_ERROR;
  }
  memscpy(uim_cmd_ptr->aka_auth.randa,
          sizeof(uim_cmd_ptr->aka_auth.randa),
          req_ptr->randa,
          MMGSDI_CDMA_AKA_RANDA_LEN);
  uim_cmd_ptr->aka_auth.autn_len                    = (byte)req_ptr->autn.data_len;
  memscpy(uim_cmd_ptr->aka_auth.autn,
          sizeof(uim_cmd_ptr->aka_auth.autn),
          req_ptr->autn.data_ptr,
          req_ptr->autn.data_len);

  /*---------------------------------------------------------------------------
  CLIENT REQUEST TABLE :- Store information
  1) Get the index
  2) If not able to get the index, free the uim_cmd_q, but keep the request
  ptr and the uim_rsp_data buffer so that it can be freed when processing the
  response
  3) Populate client request table with req_ptr, data len and uim_rsp_data buffer
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }
  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                &req_ptr->request_header,
                                                NULL);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_cdma_aka_auth */


/*============================================================================
   FUNCTION:      MMGSDI_UIM_COMMON_GENERATE_KEY_VPM

   DESCRIPTION
     This function is called to send Generate Key VPM request to UIM.

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
mmgsdi_return_enum_type mmgsdi_uim_common_generate_key_vpm(
  const mmgsdi_generate_key_req_type        *  req_ptr,
  mmgsdi_protocol_enum_type                    protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  mmgsdi_return_enum_type             mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type  *extra_param_ptr  = NULL;
  uim_cmd_type *                      uim_cmd_ptr      = NULL;
  uim_slot_type                       uim_slot         = UIM_SLOT_NONE;
  uint32                              index            = 0;
  mmgsdi_slot_id_enum_type            mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_int_app_info_type           *app_info_ptr     = NULL;
  mmgsdi_channel_id_type              channel_id       = MMGSDI_DEFAULT_CHANNEL;
  uim_channel_type                    uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr          - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);

  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }
  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot,&uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /* Get a UIM buffer for the request */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  uim_cmd_ptr->hdr.command               = UIM_GENERATE_KEYS_F;
  uim_cmd_ptr->hdr.slot                  = uim_slot;
  uim_cmd_ptr->hdr.channel               = uim_channel;
  uim_cmd_ptr->hdr.protocol              = (protocol == MMGSDI_UICC) ?
                                            UIM_UICC : UIM_ICC;

  /* ------------------------------------------------------------------------
      Set Generate Key / VPM data
    ----------------------------------------------------------------------*/
  uim_cmd_ptr->gen_keys.vpm_first_octet    = req_ptr->first_octet;
  uim_cmd_ptr->gen_keys.vpm_last_octet     = req_ptr->last_octet;

  /*---------------------------------------------------------------------------
  CLIENT REQUEST TABLE :- Store information
  1) Get the index
  2) If not able to get the index, free the uim_cmd_q, but keep the request
  ptr and the uim_rsp_data buffer so that it can be freed when processing the
  response
  3) Populate client request table with req_ptr, data len and uim_rsp_data buffer
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Allocate and populate extra parameter */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
                                               MMGSDI_GENERATE_KEY_VPM_REQ,
                                               (void*)req_ptr,
                                               &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                &req_ptr->request_header,
                                                extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_generate_key_vpm */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_OTASP_BLOCK

  DESCRIPTION:
    This function will format the OTASP block request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending OTASP Block
                             CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_otasp_block(
  const mmgsdi_session_otasp_block_req_type *req_ptr,
  mmgsdi_protocol_enum_type                  protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;
  mmgsdi_client_req_extra_info_type *extra_param_ptr= NULL;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot,&uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /* Allocate and populate extra parameter */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
                                               MMGSDI_SESSION_OTASP_BLOCK_REQ,
                                               (void*)req_ptr,
                                               &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;

  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  switch(req_ptr->block_op_type)
  {
    case MMGSDI_OTASP_CONFIG:  /* Perform Config */
      /* It does not require block data, ignore if we have any block length */
      uim_cmd_ptr->hdr.command              = UIM_CONFIG_REQ_F;
      uim_cmd_ptr->configuration.block_id   = req_ptr->block_id;
      break;

    case MMGSDI_OTASP_DOWNLOAD: /* Perform Download */
      uim_cmd_ptr->hdr.command        = UIM_DOWNLOAD_REQ_F;
      uim_cmd_ptr->download.block_id  = req_ptr->block_id;
      uim_cmd_ptr->download.block_len = uint32touint8(req_ptr->block_data.data_len);
      if (req_ptr->block_data.data_ptr)
      {
        mmgsdi_memscpy((void*)uim_cmd_ptr->download.data,
                       sizeof(uim_cmd_ptr->download.data),
                       (void*)req_ptr->block_data.data_ptr,
                       uim_cmd_ptr->download.block_len);
      }
      break;

    case MMGSDI_OTASP_SSPR_DOWNLOAD: /* Perform SSPR Download */
      uim_cmd_ptr->hdr.command              = UIM_SSPR_DOWNLOAD_REQ_F;
      uim_cmd_ptr->sspr_download.block_id   = req_ptr->block_id;
      uim_cmd_ptr->sspr_download.block_len  = uint32touint8(req_ptr->block_data.data_len);
      if (req_ptr->block_data.data_ptr)
      {
        mmgsdi_memscpy((void*)uim_cmd_ptr->sspr_download.data,
                       sizeof(uim_cmd_ptr->sspr_download.data),
                       (void*)req_ptr->block_data.data_ptr,
                       uim_cmd_ptr->sspr_download.block_len);
      }
      break;

    case MMGSDI_OTASP_VALIDATE: /* Perform Validate */
      uim_cmd_ptr->hdr.command           = UIM_VALIDATE_F;
      uim_cmd_ptr->validate.block_id     = req_ptr->block_id;
      uim_cmd_ptr->validate.block_length = uint32touint8(req_ptr->block_data.data_len);
      if (req_ptr->block_data.data_ptr)
      {
        mmgsdi_memscpy((void*)uim_cmd_ptr->validate.data,
                       sizeof(uim_cmd_ptr->validate.data),
                       (void*)req_ptr->block_data.data_ptr,
                       uim_cmd_ptr->validate.block_length);
      }
      break;

    default:
      UIM_MSG_ERR_1("Invalid value of otasp block op type 0x%x",
                    req_ptr->block_op_type);
      MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
      return MMGSDI_INCORRECT_PARAMS;
  }

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  else
  {
    UIM_MSG_HIGH_1("Send MMGSDI UIM COMMON OTASP Block 0x%x Request to UIM",
                   req_ptr->block_op_type);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_otasp_block */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_UICC_OTASP_OTAPA

  DESCRIPTION:
    This function will format the OTAPA request in the form required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending the store
                             ESN command to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_otasp_otapa (
  const mmgsdi_session_otasp_otapa_req_type *req_ptr,
  mmgsdi_protocol_enum_type                  protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr        - Cannot be NULL
    2. Random seed and ESN shall not be 0x00
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);
  /* No need to check randseed and esn,
     already checked when preparing mmgsdi command */

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                         req_ptr->request_header.session_id,
                                         &mmgsdi_slot,
                                         &channel_id,
                                         &app_info_ptr,
                                         NULL,
                                         NULL,
                                         NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  if (protocol == MMGSDI_UICC)
  {
    uim_cmd_ptr->hdr.protocol             = UIM_UICC;
    uim_cmd_ptr->hdr.channel              = uim_channel;
    uim_cmd_ptr->otapa_req.meid_available = TRUE;
  }
  else
  {
    uim_cmd_ptr->hdr.protocol             = UIM_CDMA;
    uim_cmd_ptr->hdr.channel              = UIM_CHANNEL0;
    /* check for meid avaliablity */
#ifndef FEATURE_UIM_TEST_FRAMEWORK
    if (req_ptr->esn)
    {
      uim_cmd_ptr->otapa_req.meid_available =
        mmgsdi_util_get_meid_availability(req_ptr->request_header.session_id);
    }
#else
    #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
  }
  uim_cmd_ptr->hdr.slot                 = uim_slot;
  uim_cmd_ptr->hdr.command              = UIM_OTAPA_REQ_F;
  uim_cmd_ptr->otapa_req.start_stop     = req_ptr->start_stop;
  uim_cmd_ptr->otapa_req.randseed       = req_ptr->randseed;
  uim_cmd_ptr->otapa_req.esn            = req_ptr->esn;
  uim_cmd_ptr->hdr.options              = UIM_OPTION_NONE;

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
    index, &req_ptr->request_header, NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_otasp_otapa */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_OTASP_COMMIT

  DESCRIPTION:
    This function will format the OTASP Commit request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending OTASP
                             CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_otasp_commit (
  const mmgsdi_session_otasp_commit_req_type *req_ptr,
  mmgsdi_protocol_enum_type                   protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command                  = UIM_COMMIT_F;

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_otasp_commit */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_OTASP_SSPR_CONFIG

  DESCRIPTION:
    This function will format the OTASP SSPR Config request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending OTASP SSPR
                             Config CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_otasp_sspr_config (
  const mmgsdi_session_otasp_sspr_config_req_type *req_ptr,
  mmgsdi_protocol_enum_type                        protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command                  = UIM_SSPR_CONFIG_REQ_F;
  uim_cmd_ptr->sspr_config.block_id         = req_ptr->block_id;
  uim_cmd_ptr->sspr_config.req_offset       = req_ptr->req_offset;
  uim_cmd_ptr->sspr_config.req_max_size     = req_ptr->req_max_size;

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_otasp_sspr_config */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_OTASP_KEY_GEN

  DESCRIPTION:
    This function will format the OTASP KEY GEN request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending OTASP KEY
                             GEN CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_otasp_key_gen (
  const mmgsdi_session_otasp_key_gen_req_type *req_ptr,
  mmgsdi_protocol_enum_type                    protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command                  = UIM_KEY_GEN_REQ_F;
  uim_cmd_ptr->key_gen.bs_result_len        = uint32touint8(req_ptr->bs_result.data_len);
  mmgsdi_memscpy((void*)uim_cmd_ptr->key_gen.bs_result,
                 sizeof(uim_cmd_ptr->key_gen.bs_result),
                 (void*)req_ptr->bs_result.data_ptr,
                 uim_cmd_ptr->key_gen.bs_result_len);

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_otasp_key_gen */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_OTASP_MS_KEY

  DESCRIPTION:
    This function will format the OTASP MS KEY request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending OTASP MS KEY
                             CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_otasp_ms_key (
  const mmgsdi_session_otasp_ms_key_req_type *req_ptr,
  mmgsdi_protocol_enum_type                   protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;

  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command                  = UIM_MS_KEY_REQ_F;
  uim_cmd_ptr->ms_key_req.a_key_p_rev       = req_ptr->a_key_p_rev;
  uim_cmd_ptr->ms_key_req.param_p_len       = uint32touint8(req_ptr->param_p.data_len);
  uim_cmd_ptr->ms_key_req.param_g_len       = uint32touint8(req_ptr->param_g.data_len);

  mmgsdi_memscpy((void*)uim_cmd_ptr->ms_key_req.randseed,
                 sizeof(uim_cmd_ptr->ms_key_req.randseed),
                 (void*)req_ptr->randseed.data_ptr,
                 req_ptr->randseed.data_len);
  mmgsdi_memscpy((void*)uim_cmd_ptr->ms_key_req.param_p,
                 sizeof(uim_cmd_ptr->ms_key_req.param_p),
                 (void*)req_ptr->param_p.data_ptr,
                 uim_cmd_ptr->ms_key_req.param_p_len);
  mmgsdi_memscpy((void*)uim_cmd_ptr->ms_key_req.param_g,
                 sizeof(uim_cmd_ptr->ms_key_req.param_g),
                 (void*)req_ptr->param_g.data_ptr,
                 uim_cmd_ptr->ms_key_req.param_g_len);

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_otasp_ms_key */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_BS_CHAL

  DESCRIPTION:
    This function will format the BS CHAL request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending OTASP
                             CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_bs_chal (
  const mmgsdi_session_bs_chal_req_type *req_ptr,
  mmgsdi_protocol_enum_type              protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command                  = UIM_BS_CHAL_F;
  uim_cmd_ptr->bs_chal.randseed             = req_ptr->randseed;

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_bs_chal */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_SSD_CONFIRM

  DESCRIPTION:
    This function will format the OTASP SSD Confrim request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending OTASP
                             CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_ssd_confirm (
  const mmgsdi_session_ssd_confirm_req_type *req_ptr,
  mmgsdi_protocol_enum_type                  protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command                  = UIM_CONFIRM_SSD_F;
  uim_cmd_ptr->confirm_ssd.authbs           = req_ptr->authbs;

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_ssd_confirm */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_SSD_UPDATE

  DESCRIPTION:
    This function will format the SSD Update request in the form
    required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending SSD Update
                             CMD to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_ssd_update (
  const mmgsdi_session_ssd_update_req_type *req_ptr,
  mmgsdi_protocol_enum_type                 protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type         mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;

  /*---------------------------------------------------------------------------
   FUNCTION PARAMETER CHECKS
   1. req_ptr        - Cannot be NULL
   ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                                           req_ptr->request_header.session_id,
                                           &mmgsdi_slot,
                                           &channel_id,
                                           &app_info_ptr,
                                           NULL,
                                           NULL,
                                           NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(mmgsdi_slot, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  /*---------------------------------------------------------------------------
  Populate UIM Block Cmd data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command                  = UIM_SSD_UPDATE_F;
  uim_cmd_ptr->ssd_update.randssd[0]        = req_ptr->randssd[0];
  uim_cmd_ptr->ssd_update.randssd[1]        = req_ptr->randssd[1];
  uim_cmd_ptr->ssd_update.process_control   = req_ptr->process_control;
  uim_cmd_ptr->ssd_update.esn               = req_ptr->esn;

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                                       index,
                                                       &req_ptr->request_header,
                                                       NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data                = index;

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_ssd_update */


/*============================================================================
  FUNCTION:      MMGSDI_UIM_COMMON_STORE_ESN

  DESCRIPTION:
    This function will format the store ESN request in the form required by UIM.

  DEPENDENCIES:
    The technology type has to be determined prior to using this function

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The funtion was successful in sending the store
                             ESN command to UIM.
    MMGSDI_ERROR:
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_store_esn (
  mmgsdi_session_store_esn_req_type    *req_ptr,
  boolean                               sync_process,
  mmgsdi_session_store_esn_cnf_type   **cnf_pptr,
  mmgsdi_protocol_enum_type             protocol
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  uim_cmd_type                   * uim_cmd_ptr      = NULL;
  uim_slot_type                    uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_SUCCESS;
  uint32                           index            = 0;
  mmgsdi_int_app_info_type       * app_info_ptr     = NULL;
  mmgsdi_channel_id_type           channel_id       = 0;
  uim_channel_type                 uim_channel      = UIM_CHANNEL0;
  mmgsdi_task_enum_type            mmgsdi_task      = MMGSDI_TASK_MAX;

  UIM_MSG_HIGH_1("MMGSDI UIM COMMON STORE ESN, sync process is 0x%x",sync_process);

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr        - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(
    req_ptr->request_header.session_id,
    &req_ptr->request_header.slot_id,
    &channel_id,
    &app_info_ptr,
    NULL,
    NULL,
    NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
  {
    return MMGSDI_ERROR;
  }

  if((protocol == MMGSDI_UICC && app_info_ptr->app_data.app_type != MMGSDI_APP_CSIM) ||
     (protocol == MMGSDI_ICC && app_info_ptr->app_data.app_type != MMGSDI_APP_RUIM))
  {
    UIM_MSG_ERR_1("Invalid app_type 0x%x",app_info_ptr->app_data.app_type);
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id,
                                               &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Convert to UIM Channel ID */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
  Populate UIM Cmd pointer data
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.slot                     = uim_slot;
  uim_cmd_ptr->hdr.options                  = UIM_OPTION_NONE;
  uim_cmd_ptr->hdr.channel                  = uim_channel;
  uim_cmd_ptr->hdr.protocol                 = (protocol == MMGSDI_UICC) ?
                                               UIM_UICC : UIM_ICC;
  uim_cmd_ptr->store_meid.len_and_usage =
      req_ptr->esn_meid_info.esn_length | req_ptr->esn_meid_info.esn_usage;

  if (req_ptr->esn_meid_info.meid_available)
  {
    uim_cmd_ptr->hdr.command = UIM_STORE_ESN_MEID_ME_F;
    qw_equ(uim_cmd_ptr->store_meid.meid, req_ptr->esn_meid_info.meid);
    uim_cmd_ptr->store_meid.meid_available = TRUE;
  }
  else
  {
    uim_cmd_ptr->hdr.command = UIM_STORE_ESN_ME_F;
    uim_cmd_ptr->store_esn.esn = req_ptr->esn_meid_info.esn;
  }

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
    index, &req_ptr->request_header, NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  if (sync_process)
  {
    mmgsdi_status = mmgsdi_send_cmd_to_uim_server_synch(uim_cmd_ptr);
  }
  else
  {
    mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  }

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  else
  {
    /* Synchronous internal processing, retrieve the confirmation data */
    if (sync_process)
    {
      mmgsdi_task = mmgsdi_util_get_current_task();
      if (MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task) &&
          mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task] != NULL)
      {
        mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]->rsp_data_index = index;
        mmgsdi_status = mmgsdi_common_rsp_store_esn(
               mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task], cnf_pptr, TRUE, protocol);
        if (mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task] != NULL)
        {
          MMGSDIUTIL_TMC_MEM_FREE(
            mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]->rsp_data.data_ptr);
          MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]);
        }
      }
      else
      {
        UIM_MSG_ERR_1("Sync process for request at index 0x%x has null mmgsdi_internal_synch_uim_rpt_rsp",
                      index);
        mmgsdi_status = MMGSDI_ERROR;
      }
      mmgsdi_util_free_client_request_table_index(index);
    }
  }
  /* Deallocate the uim cmd from heap will be done when the release client info
     has been called*/
  return mmgsdi_status;
#else
  (void)req_ptr;
  (void)sync_process;
  (void)cnf_pptr;
  (void)protocol;
  return MMGSDI_NOT_SUPPORTED;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* mmgsdi_uim_common_store_esn */


/*============================================================================
   FUNCTION:      MMGSDI_UIM_COMMON_CHANGE_PIN
   DESCRIPTION:
     This function packages the CHANGE PIN  command and sends it to UIM
     for further processing

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_change_pin (
  const mmgsdi_pin_operation_req_type             * req_ptr,
  mmgsdi_protocol_enum_type                         protocol
)
{
  uim_cmd_type *                     uim_cmd_ptr      = NULL;
  uim_slot_type                      uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  uint32                             index            = 0;
  mmgsdi_channel_id_type             channel_id       = 0;
  mmgsdi_client_req_extra_info_type *extra_param_ptr  = NULL;
  uim_channel_type                   uim_channel_id   = UIM_CHANNEL0;
  mmgsdi_static_data_type           *aid_ptr          = NULL;
  mmgsdi_int_app_info_type          *app_info_ptr     = NULL;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr        - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id,
                                               &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                   req_ptr->request_header.session_id,
                   NULL,
                   &channel_id,
                   &app_info_ptr,
                   NULL,
                   NULL,
                   NULL);

    if (mmgsdi_status != MMGSDI_SUCCESS || app_info_ptr == NULL)
    {
      return mmgsdi_status;
    }

    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel_id);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }

    aid_ptr = &app_info_ptr->app_data.aid;
  }

  if (req_ptr->pin_puk.data_len < MMGSDI_PIN_MIN_LEN ||
      req_ptr->pin_puk.data_len > MMGSDI_PIN_MAX_LEN ||
      req_ptr->new_pin.data_len < MMGSDI_PIN_MIN_LEN ||
      req_ptr->new_pin.data_len > MMGSDI_PIN_MAX_LEN)
  {
     UIM_MSG_ERR_0("CHANGE_PIN_REQ - PIN invalid");
     return MMGSDI_ERROR;
  }

  /* Allocate and populate extra parameter */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
          MMGSDI_PIN_OPERATION_REQ, (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
    Populate UIM information
    1) command = UIM_CHANGE_CHV_F
    2) protocol = UIM_WCDMA/UIM_GSM  [ GSM and CDMA share the same protocol ]
    3) slot
    4) pin data len and data
    5) user data: the req_ptr
    6) aid
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command         = UIM_CHANGE_CHV_F;
  uim_cmd_ptr->hdr.slot            = uim_slot;
  uim_cmd_ptr->hdr.channel         = uim_channel_id;
  uim_cmd_ptr->hdr.protocol        = (protocol == MMGSDI_UICC) ?
                                      UIM_UICC : UIM_ICC;

  mmgsdi_status = mmgsdi_util_get_key_ref(req_ptr->request_header.session_id,
                                          req_ptr->pin_id,
                                          &uim_cmd_ptr->change_chv.chv);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_memscpy(uim_cmd_ptr->change_chv.chv_digits,
                 sizeof(uim_cmd_ptr->change_chv.chv_digits),
                 req_ptr->pin_puk.data_ptr,
                 req_ptr->pin_puk.data_len);

  mmgsdi_memscpy(uim_cmd_ptr->change_chv.new_chv_digits,
                 sizeof(uim_cmd_ptr->change_chv.new_chv_digits),
                 req_ptr->new_pin.data_ptr,
                 req_ptr->new_pin.data_len);

  /* Populate masking function pointer */
  if (app_info_ptr != NULL)
  {
    mmgsdi_logging_populate_masking_fn_ptr(uim_cmd_ptr,
                                           app_info_ptr->app_data.app_type);
  }

   /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release extra param pointer (Since client table has not been populated
                                    yet)
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                 &req_ptr->request_header,
                                                 extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release extra param pointer pin data (Since client table has not been
                                    populated yet)
    3) Release extra param pointer (Since client table has not been populated
                                    yet)
    4) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* aid */
  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_memscpy(uim_cmd_ptr->change_chv.aid.data,
                   sizeof(uim_cmd_ptr->change_chv.aid.data),
                   aid_ptr->data_ptr,
                   aid_ptr->data_len);
    uim_cmd_ptr->change_chv.aid.aid_length = aid_ptr->data_len;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  UIM_MSG_HIGH_0("Sending down CHANGE CHV/PIN command to UIM");

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;
} /* mmgsdi_uim_common_change_pin */


/*============================================================================
   FUNCTION:      MMGSDI_UIM_COMMON_ENABLE_DISABLE_PIN
   DESCRIPTION:
     This function packages the ENABLE PIN or DISABLE PIN command and sends
     it to UIM for further processing

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_enable_disable_pin (
  const mmgsdi_pin_operation_req_type             * req_ptr,
  mmgsdi_protocol_enum_type                         protocol
)
{
  uim_cmd_type *                     uim_cmd_ptr      = NULL;
  uim_slot_type                      uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  uint32                             index            = 0;
  mmgsdi_channel_id_type             channel_id       = 0;
  mmgsdi_client_req_extra_info_type *extra_param_ptr  = NULL;
  uim_channel_type                   uim_channel_id   = UIM_CHANNEL0;
  mmgsdi_static_data_type           *aid_ptr          = NULL;
  mmgsdi_int_app_info_type          *app_info_ptr     = NULL;
  uim_chv_type                       chv_type         = UIM_CHV_MAX;
  uim_chv_type                       replace_chv;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr        - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if (req_ptr->pin_op != MMGSDI_PIN_OP_ENABLE &&
      req_ptr->pin_op != MMGSDI_PIN_OP_DISABLE &&
      req_ptr->pin_op != MMGSDI_PIN_OP_DISABLE_AND_REPLACE)
  {
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id,
                                               &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      req_ptr->request_header.session_id,
                      NULL,
                      &channel_id,
                      &app_info_ptr,
                      NULL,
                      NULL,
                      NULL);
    if ((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
    {
      return mmgsdi_status;
    }

    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel_id);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
    aid_ptr = &app_info_ptr->app_data.aid;
  }

  if (req_ptr->pin_puk.data_len < MMGSDI_PIN_MIN_LEN ||
      req_ptr->pin_puk.data_len > MMGSDI_PIN_MAX_LEN)
  {
     UIM_MSG_ERR_0("CHV/PIN invalid");
     return MMGSDI_ERROR;
  }

  /* Allocate and populate extra parameter */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
          MMGSDI_PIN_OPERATION_REQ, (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  mmgsdi_status = mmgsdi_util_get_key_ref(req_ptr->request_header.session_id,
                                          req_ptr->pin_id,
                                          &chv_type);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    Populate UIM information
    1) command = UIM_DISABLE_CHV_F/UIM_ENABLE_CHV_F
    2) protocol = UIM_WCDMA/UIM_GSM  [ GSM and CDMA share the same protocol ]
    3) slot
    4) pin data len and data
    5) user data: the req_ptr
    6) aid
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.slot     = uim_slot;
  uim_cmd_ptr->hdr.channel  = uim_channel_id;
  uim_cmd_ptr->hdr.protocol = (protocol == MMGSDI_UICC) ? UIM_UICC : UIM_ICC;

  if (req_ptr->pin_op == MMGSDI_PIN_OP_ENABLE)
  {
    uim_cmd_ptr->hdr.command = UIM_ENABLE_CHV_F;
    uim_cmd_ptr->enable_chv.chv = chv_type;

    mmgsdi_memscpy(uim_cmd_ptr->enable_chv.chv_digits,
                   sizeof(uim_cmd_ptr->enable_chv.chv_digits),
                   req_ptr->pin_puk.data_ptr,
                   req_ptr->pin_puk.data_len);

    if (protocol == MMGSDI_UICC)
    {
      /* aid */
      mmgsdi_memscpy(uim_cmd_ptr->enable_chv.aid.data,
                     sizeof(uim_cmd_ptr->enable_chv.aid.data),
                     aid_ptr->data_ptr,
                     aid_ptr->data_len);
      uim_cmd_ptr->enable_chv.aid.aid_length = aid_ptr->data_len;
    }
  }
  else
  {
    uim_cmd_ptr->hdr.command = UIM_DISABLE_CHV_F;
    uim_cmd_ptr->disable_chv.chv = chv_type;

    mmgsdi_memscpy(uim_cmd_ptr->disable_chv.chv_digits,
                   sizeof(uim_cmd_ptr->disable_chv.chv_digits),
                   req_ptr->pin_puk.data_ptr,
                   req_ptr->pin_puk.data_len);

    if (protocol == MMGSDI_UICC)
    {
      /* aid */
      mmgsdi_memscpy(uim_cmd_ptr->disable_chv.aid.data,
                     sizeof(uim_cmd_ptr->disable_chv.aid.data),
                     aid_ptr->data_ptr,
                     aid_ptr->data_len);
      uim_cmd_ptr->disable_chv.aid.aid_length = aid_ptr->data_len;
    }
  }

  /*Set the P2 Variable if Required*/
  if (protocol == MMGSDI_UICC && req_ptr->pin_op == MMGSDI_PIN_OP_DISABLE_AND_REPLACE)
  {
    mmgsdi_status = mmgsdi_util_get_key_ref(req_ptr->request_header.session_id,
                                            MMGSDI_UNIVERSAL_PIN,
                                            &replace_chv);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
      return mmgsdi_status;
    }
    uim_cmd_ptr->disable_chv.param_p2 = (byte)replace_chv;
  }

  /* Populate masking function pointer */
  if (app_info_ptr != NULL)
  {
    mmgsdi_logging_populate_masking_fn_ptr(uim_cmd_ptr,
                                           app_info_ptr->app_data.app_type);
  }

   /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release extra param pointer (Since client table has not been populated
                                    yet)
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                 &req_ptr->request_header,
                                                 extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release extra param pointer pin data (Since client table has not been
                                    populated yet)
    3) Release extra param pointer (Since client table has not been populated
                                    yet)
    4) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  UIM_MSG_HIGH_1("Sending down PIN op 0x%x command to UIM", req_ptr->pin_op);

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;
} /* mmgsdi_uim_common_enable_disable_pin */


/*============================================================================
   FUNCTION:      MMGSDI_UIM_COMMON_UNBLOCK_VERIFY_PIN
   DESCRIPTION:
     This function packages the UNBLOCK/VERIFY PIN command and sends it to UIM
     for further processing

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

============================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_unblock_verify_pin (
  const mmgsdi_pin_operation_req_type        * req_ptr,
  mmgsdi_num_retry_enum_type                   retry_type,
  mmgsdi_protocol_enum_type                    protocol,
  boolean                                      sync_process
)
{
  uim_cmd_type *                       uim_cmd_ptr      = NULL;
  uim_slot_type                        uim_slot         = UIM_SLOT_NONE;
  mmgsdi_return_enum_type              mmgsdi_status    = MMGSDI_SUCCESS;
  uint32                               index            = 0;
  mmgsdi_channel_id_type               channel_id       = 0;
  mmgsdi_client_req_extra_info_type   *extra_param_ptr  = NULL;
  uim_channel_type                     uim_channel_id   = UIM_CHANNEL0;
  mmgsdi_static_data_type             *aid_ptr          = NULL;
  mmgsdi_int_app_info_type            *app_info_ptr     = NULL;
  uim_chv_type                         chv_type         = UIM_CHV_MAX;
  mmgsdi_pin_operation_cnf_type       *cnf_ptr          = NULL;

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr        - Cannot be NULL
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  if (protocol == MMGSDI_ICC &&
      ((req_ptr->pin_puk.data_len < MMGSDI_PIN_MIN_LEN ||
        req_ptr->pin_puk.data_len > MMGSDI_PIN_MAX_LEN) ||
       (req_ptr->pin_op == MMGSDI_PIN_OP_UNBLOCK &&
        (req_ptr->new_pin.data_len < MMGSDI_PIN_MIN_LEN ||
         req_ptr->new_pin.data_len > MMGSDI_PIN_MAX_LEN))))
  {
     UIM_MSG_ERR_0("CHV/PIN invalid");
     return MMGSDI_INCORRECT_PARAMS;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id,
                                               &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                   req_ptr->request_header.session_id,
                   NULL,
                   &channel_id,
                   &app_info_ptr,
                   NULL,
                   NULL,
                   NULL);
    if (mmgsdi_status != MMGSDI_SUCCESS || app_info_ptr == NULL)
    {
      return mmgsdi_status;
    }

    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uim_channel_id);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }

    aid_ptr = &app_info_ptr->app_data.aid;
  }

  /* Allocate and populate extra parameter */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
          MMGSDI_PIN_OPERATION_REQ, (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  extra_param_ptr->pin_data.retry_type = retry_type;

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  mmgsdi_status = mmgsdi_util_get_key_ref(req_ptr->request_header.session_id,
                                          req_ptr->pin_id,
                                          &chv_type);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    Populate UIM information
    1) command = UIM_UNBLOCK_CHV_F/UIM_VERIFY_CHV_F
    2) protocol = UIM_WCDMA/UIM_GSM  [ GSM and CDMA share the same protocol ]
    3) slot
    4) pin data len and data
    5) user data: the req_ptr
    6) aid
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.slot            = uim_slot;
  uim_cmd_ptr->hdr.channel         = uim_channel_id;
  uim_cmd_ptr->hdr.protocol        = (protocol == MMGSDI_UICC) ?
                                      UIM_UICC : UIM_ICC;

  if ((req_ptr->pin_op == MMGSDI_PIN_OP_UNBLOCK &&
       retry_type == MMGSDI_RETRY_NONE) ||
      retry_type == MMGSDI_GET_NUM_UNBLOCK_RETRIES)
  {
    uim_cmd_ptr->hdr.command   = UIM_UNBLOCK_CHV_F;
    uim_cmd_ptr->unblk_chv.chv = chv_type;

    if (req_ptr->pin_puk.data_len >= MMGSDI_PIN_MIN_LEN &&
        req_ptr->pin_puk.data_len <= MMGSDI_PIN_MAX_LEN)
    {
      mmgsdi_memscpy(uim_cmd_ptr->unblk_chv.chv_digits,
                     sizeof(uim_cmd_ptr->unblk_chv.chv_digits),
                     req_ptr->pin_puk.data_ptr,
                     req_ptr->pin_puk.data_len);
    }
    else
    {
      uim_cmd_ptr->unblk_chv.chv_digits[0] = '\0';
    }

    if (req_ptr->new_pin.data_len >= MMGSDI_PIN_MIN_LEN &&
        req_ptr->new_pin.data_len <= MMGSDI_PIN_MAX_LEN)
    {
      mmgsdi_memscpy(uim_cmd_ptr->unblk_chv.new_chv_digits,
                     sizeof(uim_cmd_ptr->unblk_chv.new_chv_digits),
                     req_ptr->new_pin.data_ptr,
                     req_ptr->new_pin.data_len);
    }
    else
    {
      uim_cmd_ptr->unblk_chv.new_chv_digits[0] = '\0';
    }

    /* aid*/
    if (protocol == MMGSDI_UICC)
    {
      mmgsdi_memscpy(uim_cmd_ptr->unblk_chv.aid.data,
                     sizeof(uim_cmd_ptr->unblk_chv.aid.data),
                     aid_ptr->data_ptr,
                     aid_ptr->data_len);
      uim_cmd_ptr->unblk_chv.aid.aid_length = aid_ptr->data_len;
    }
  }
  else
  {
    uim_cmd_ptr->hdr.command    = UIM_VERIFY_CHV_F;
    uim_cmd_ptr->verify_chv.chv = chv_type;

    if (req_ptr->pin_puk.data_len >= MMGSDI_PIN_MIN_LEN &&
        req_ptr->pin_puk.data_len <= MMGSDI_PIN_MAX_LEN)
    {
      mmgsdi_memscpy(uim_cmd_ptr->verify_chv.chv_digits,
                     sizeof(uim_cmd_ptr->verify_chv.chv_digits),
                     req_ptr->pin_puk.data_ptr,
                     req_ptr->pin_puk.data_len);
    }
    else
    {
      uim_cmd_ptr->verify_chv.chv_digits[0] = '\0';
    }

    /* aid*/
    if (protocol == MMGSDI_UICC)
    {
      mmgsdi_memscpy(uim_cmd_ptr->verify_chv.aid.data,
                     sizeof(uim_cmd_ptr->verify_chv.aid.data),
                     aid_ptr->data_ptr,
                     aid_ptr->data_len);
      uim_cmd_ptr->verify_chv.aid.aid_length = aid_ptr->data_len;
    }
  }
  /* Populate masking function pointer */
  if (app_info_ptr != NULL)
  {
    mmgsdi_logging_populate_masking_fn_ptr(uim_cmd_ptr,
                                           app_info_ptr->app_data.app_type);
  }

   /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release extra param pointer (Since client table has not been populated
                                    yet)
    2) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                 &req_ptr->request_header,
                                                 extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release extra param pointer pin data (Since client table has not been
                                    populated yet)
    3) Release extra param pointer (Since client table has not been populated
                                    yet)
    4) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  UIM_MSG_HIGH_2("Sending down PIN command to UIM for pin_op 0x%x and retry type 0x%x",
                 req_ptr->pin_op, retry_type);

  if (sync_process)
  {
    mmgsdi_status = mmgsdi_send_cmd_to_uim_server_synch(uim_cmd_ptr);
  }
  else
  {
    mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  }

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  else
  {
    if (sync_process)
    {
      mmgsdi_task_enum_type              mmgsdi_task      = MMGSDI_TASK_MAX;

      mmgsdi_task = mmgsdi_util_get_current_task();
      if (mmgsdi_task != MMGSDI_TASK_MAX &&
          mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task] != NULL)
      {
        mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]->rsp_data_index = index;
        mmgsdi_status = mmgsdi_common_rsp_pin_op(mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task],
                                                 protocol,
                                                 &cnf_ptr,
                                                 TRUE);
        if (mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task] != NULL)
        {
          MMGSDIUTIL_TMC_MEM_FREE(
            mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]->rsp_data.data_ptr);
          MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]);
        }
        mmgsdi_util_free_response_ptr((mmgsdi_cnf_type*)cnf_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);
      }
      else
      {
        UIM_MSG_ERR_1("Sync process for request at index 0x%x has null mmgsdi_internal_synch_uim_rpt_rsp",
                      index);
        mmgsdi_status = MMGSDI_ERROR;
      }
      mmgsdi_util_free_client_request_table_index(index);
    }
  }

  return mmgsdi_status;
} /* mmgsdi_uim_common_unblock_verify_pin */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_UIM_COMMON_WRITE_TRANSPARENT

   DESCRIPTION:
     This function package the Write Transparent command and send it to UIM
     for further processing

   DEPENDENCIES:


   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.

   SIDE EFFECTS:

-------------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_uim_common_write_transparent(
  const mmgsdi_write_req_type             * req_ptr,
  boolean                                   sync_process,
  mmgsdi_write_cnf_type                   **cnf_pptr,
  mmgsdi_protocol_enum_type                 protocol
)
{
  uim_cmd_type *                     uim_cmd_ptr        = NULL;
  uim_slot_type                      uim_slot           = UIM_SLOT_NONE;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_SUCCESS;
  uint32                             index              = 0;
  mmgsdi_client_req_extra_info_type *extra_param_ptr    = NULL;
  mmgsdi_session_info_type          *session_info_ptr   = NULL;
  mmgsdi_task_enum_type              mmgsdi_task        = MMGSDI_TASK_MAX;
  mmgsdi_len_type                    mmgsdi_write_len   = 0;
  mmgsdi_len_type                    valid_fplmn_len    = 0;
  boolean                            is_extended_fplmn  = FALSE;
  mmgsdi_int_app_info_type          *app_info_ptr       = NULL;
  mmgsdi_channel_id_type             channel_id         = 0; 
  boolean                            card_session       = FALSE;
  uim_channel_type                   uicc_channel_id    = UIM_MAX_CHANNELS;
  mmgsdi_static_data_type           *aid_ptr            = NULL;
  mmgsdi_slot_data_type             *slot_data_ptr      = NULL;

  /* Check for protocol */
  if ((protocol != MMGSDI_UICC) && (protocol != MMGSDI_ICC))
  {
    return MMGSDI_ERROR;
  }

  /*---------------------------------------------------------------------------
    FUNCTION PARAMETER CHECKS
    1. req_ptr        - Cannot be NULL
    2. access method  - Should be EF Enum access
  ---------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);
  if(sync_process)
  {
    MMGSDIUTIL_RETURN_IF_NULL(cnf_pptr);
  }

  if (req_ptr->data.data_len == 0)
  {
    return MMGSDI_ERROR;
  }

  /* Before sending the write request to uim, check if this file is currently
     being refreshed */
  if(mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_BLOCK_WRITES_TO_REFRESH_FILES) == MMGSDI_FEATURE_ENABLED &&
     mmgsdi_is_file_being_refreshed(req_ptr->request_header.session_id,
                                    req_ptr->access))
  {
    UIM_MSG_ERR_0("Request temporarily not allowed as requested file is being refreshed");
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id,
                                               &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                    req_ptr->request_header.session_id,
                    NULL,
                    &channel_id,
                    &app_info_ptr,
                    NULL,
                    &session_info_ptr,
                    NULL);
  if(session_info_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* Allow write request if the write request is on a card session and files outside ADF 
     Irrespective of app selection else block the write request */
  if (mmgsdi_util_is_card_session(session_info_ptr->session_type))
  {
    if (!mmgsdi_file_is_access_outside_adf(&req_ptr->access))
    {
      return MMGSDI_ERROR;
    }

    card_session = TRUE;
  }
  else if (mmgsdi_status != MMGSDI_SUCCESS || app_info_ptr == NULL)
  {
    if(protocol == MMGSDI_UICC)
    {
      /* For non-prov. Sessions opened through Select with MF, no app shall be
         tied to the Session. In such cases, allow requests for EFs outside
         of ADFs on the channel corresponding to the Session */
      if(mmgsdi_util_is_non_prov_session(session_info_ptr->session_type))
      {
        if((session_info_ptr->channel_info_index >= MMGSDI_MAX_CHANNEL_INFO) ||
           (mmgsdi_channel_info_ptr_table[session_info_ptr->channel_info_index] == NULL))
        {
          UIM_MSG_ERR_0("Channel info needs to be valid for non-prov. session");
          return MMGSDI_ERROR;
        }

        /* Block reads for files under ADF */
        if (!mmgsdi_file_is_access_outside_adf(
               &req_ptr->access))
        {
          return MMGSDI_ERROR;
        }
      }
      else
      {
        return MMGSDI_ERROR;
      }
    }
    else
    {
      /* No application is activated by this time, return error */
      return MMGSDI_ERROR;
    }
  }

  /* For UICC cards, we need to derive the channel_id and the AID. */
  if(protocol == MMGSDI_UICC)
  {
    mmgsdi_status = mmgsdi_util_convert_uim_channel(channel_id, &uicc_channel_id);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }

    /* AID data not required for file access on a Card Session or
     for an EF under MF */
    if((!card_session) &&
       (!mmgsdi_file_is_ef_under_mf(&req_ptr->access)) &&
       app_info_ptr != NULL)
    {
      aid_ptr = &app_info_ptr->app_data.aid;

      if (aid_ptr->data_len > UIM_AID_MAX_NAME_LENGTH)
      {
        UIM_MSG_ERR_1("Invalid UIM AID size 0x%x", aid_ptr->data_len);
        return MMGSDI_ERROR;
      }
    }
  }

  /* Before sending write, make sure we are not trying to
     write more than the max file size */
  mmgsdi_write_len = req_ptr->data.data_len;
  mmgsdi_status = mmgsdi_file_check_file_info_and_update_len(
                    req_ptr->request_header.session_id,
                    req_ptr->request_header.slot_id,
                    req_ptr->access,
                    req_ptr->file_type,
                    req_ptr->offset,
                    FALSE,
                    &mmgsdi_write_len,
                    NULL);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if(!mmgsdi_file_transparent_is_access_range_valid(req_ptr->offset, mmgsdi_write_len))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* If file is FPLMN and extended FPLMN feature is supported and also
     onchip is not enabled then data to be written can be more than file
     len as per attributes */
  if(slot_data_ptr->onchip_sim_data.state != MMGSDI_ONCHIP_SIM_INIT &&
     req_ptr->access.access_method == MMGSDI_EF_ENUM_ACCESS &&
     (req_ptr->access.file.file_enum == MMGSDI_GSM_FPLMN ||
      req_ptr->access.file.file_enum == MMGSDI_USIM_FPLMN) &&
     (app_info_ptr != NULL && app_info_ptr->fplmn_actual_size_on_card < MMGSDI_EXTENDED_FPLMN_CACHE_LEN) &&
     mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SUPPORT_EXTENDED_FPLMN)
        == MMGSDI_FEATURE_ENABLED)
  {
    is_extended_fplmn = TRUE;
  }
  else if( mmgsdi_write_len < req_ptr->data.data_len)
  {
    /* Data to be written is larger than the file size. Instead of truncating
       the data, return error here. Otherwise the client might think that
       entire data in the request was written to the card. */
    return MMGSDI_INCORRECT_LENGTH;
  }

  /* Allocate and populate extra parameter */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
          MMGSDI_WRITE_REQ, (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    return mmgsdi_status;
  }

  /*---------------------------------------------------------------------------
    1) Get a UIM buffer for the request
    2) Check for if the buffer is NULL or not
  ---------------------------------------------------------------------------*/

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr,
                                     sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr->write_data.data.data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /*---------------------------------------------------------------------------
    Populate UIM information
    1) command = UIM_WRITE_BIN_CMD_F
    2) protocol   = UIM_UICC/UIM_ICC [ GSM and CDMA share the same protocol ]
    3) slot
    4) item
    5) path
    6) offset
    7) data len and data
    8) user data: the req_ptr
  ---------------------------------------------------------------------------*/
  uim_cmd_ptr->hdr.command         = UIM_WRITE_BIN_CMD_F;
  uim_cmd_ptr->hdr.protocol        = (protocol == MMGSDI_UICC) ? UIM_UICC : UIM_ICC;
  uim_cmd_ptr->hdr.slot            = uim_slot;
  uim_cmd_ptr->hdr.channel         = (protocol == MMGSDI_UICC) ? uicc_channel_id : UIM_CHANNEL0;
  /* Assign uim_cmd_ptr's data len and data buffer info */
  uim_cmd_ptr->write_bin.offset    = uint32touint16(req_ptr->offset);
  uim_cmd_ptr->write_bin.len       = uint32touint16(req_ptr->data.data_len);
  uim_cmd_ptr->write_bin.data      = extra_param_ptr->write_data.data.data_ptr;

  mmgsdi_status = mmgsdi_file_set_uim_path(&(req_ptr->access),
                                           &uim_cmd_ptr->write_bin.path);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr->write_data.data.data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Populate masking function pointer */
  if (app_info_ptr != NULL) 
  {
    mmgsdi_logging_populate_masking_fn_ptr(uim_cmd_ptr,
                                           app_info_ptr->app_data.app_type);  
  }

  /*---------------------------------------------------------------------------
  Store Information into the client req info table
  1) Get the index
  2) Populate client request table
  ---------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release extra param pointer write data (Since client table has not been
                                    populated yet)
    2) Release extra param pointer (Since client table has not been populated
                                    yet)
    3) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr->write_data.data.data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                 &req_ptr->request_header,
                                                 extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table
    2) Release extra param pointer write data (Since client table has not been
                                    populated yet)
    3) Release extra param pointer (Since client table has not been populated
                                    yet)
    4) Release uim cmd pointer  (Since client table has not been populated yet)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr->write_data.data.data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* If the file is FPLMN then write only latest valid FPLMNs (latest ones
     according to NAS) in the card and in our cache maintain the complete list
     of MMGSDI_EXTENDED_ICC_FPLMN_CACHE_LEN Bytes. In case if FPLMN data_len
     is upto card data size, write directly to card for backward compatibility.
     NAS will always give MMGSDI_EXTENDED_ICC_FPLMN_CACHE_LEN bytes data without
     any holes, with all invalid bytes set to 0xFF */
  if(is_extended_fplmn &&
     app_info_ptr != NULL &&
     req_ptr->data.data_len > app_info_ptr->fplmn_actual_size_on_card)
  {
    if (req_ptr->data.data_len == MMGSDI_EXTENDED_FPLMN_CACHE_LEN)
    {
      /* Searching for number of valid FPLMNs in complete list which needs to be written to card */
      for (valid_fplmn_len = 0; valid_fplmn_len < req_ptr->data.data_len; valid_fplmn_len++)
      {
        if (*(req_ptr->data.data_ptr + valid_fplmn_len) == 0xFF)
        {
          break;
        }
      }
      if ((valid_fplmn_len % 3) != 0)
      {
        UIM_MSG_ERR_1("Incorrect FPLMN valid data: 0x%x", valid_fplmn_len);
        mmgsdi_util_free_client_request_table_index(index);
        MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
        return MMGSDI_ERROR;
      }
    }
    else
    {
      UIM_MSG_ERR_1("FPLMN data length recieved is incorrect 0x%x",
                    req_ptr->data.data_len);
      mmgsdi_util_free_client_request_table_index(index);
      MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
      return MMGSDI_ERROR;
    }

    UIM_MSG_HIGH_1("Support Extended FPLMN. Valid FPLMNs recieved: 0x%x",
                   (valid_fplmn_len/MMGSDI_PLMN_ID_SIZE));
  
    /* If valid list is more than card size write the lastest FPLMNs accroding to file size */
    if (valid_fplmn_len > app_info_ptr->fplmn_actual_size_on_card)
    {
      uim_cmd_ptr->write_bin.data += (valid_fplmn_len - app_info_ptr->fplmn_actual_size_on_card);
    }
    uim_cmd_ptr->write_bin.len = app_info_ptr->fplmn_actual_size_on_card;

    /* Maintain the actual data being written to card, because if file is not present on card
       and FS is active, then this data needs to be written in EFS */
    extra_param_ptr->write_data.actual_data.data_len = uim_cmd_ptr->write_bin.len;
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(extra_param_ptr->write_data.actual_data.data_ptr,
                                       uim_cmd_ptr->write_bin.len);

    if(extra_param_ptr->write_data.actual_data.data_ptr == NULL)
    {
      MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr->write_data.data.data_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    mmgsdi_memscpy(extra_param_ptr->write_data.actual_data.data_ptr,
                   extra_param_ptr->write_data.actual_data.data_len,
                   uim_cmd_ptr->write_bin.data,
                   uim_cmd_ptr->write_bin.len);
  }

  /* For UICC, we need to populate also the SFI and the AID */
  if(protocol == MMGSDI_UICC)
  {
    (void)mmgsdi_file_set_uim_sfi(req_ptr->request_header.session_id,
                                 &(req_ptr->access),
                                 &uim_cmd_ptr->write_bin.sfi);

    if(aid_ptr != NULL && aid_ptr->data_len > 0)
    {
      mmgsdi_memscpy(uim_cmd_ptr->write_bin.aid.data,
                     sizeof(uim_cmd_ptr->write_bin.aid.data),
                     aid_ptr->data_ptr,
                     aid_ptr->data_len);
    
      uim_cmd_ptr->write_bin.aid.aid_length = aid_ptr->data_len;
    }
  }

  /* Assign uim_cmd_ptr's user data */
  uim_cmd_ptr->hdr.user_data = index;

  UIM_MSG_HIGH_1("Sending down WRITE command to UIM 0x%x",
                 req_ptr->access.file.file_enum);

  if(sync_process)
  {
    mmgsdi_status = mmgsdi_send_cmd_to_uim_server_synch(uim_cmd_ptr);
  }
  else
  {
    mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  }

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /*---------------------------------------------------------------------------
    1) Release the client table (Others pointer will be released in the
       function)
    ---------------------------------------------------------------------------*/
    mmgsdi_util_free_client_request_table_index(index);
  }
  else
  {
    /* Synchronous internal processing, retrieve the confirmation data */
    if (sync_process)
    {
      mmgsdi_task = mmgsdi_util_get_current_task();
      if (MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task) &&
          mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task] != NULL)
      {
        mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]->rsp_data_index = index;
        mmgsdi_status = mmgsdi_common_rsp_write(
          mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task], cnf_pptr, TRUE, protocol);
        MMGSDIUTIL_TMC_MEM_FREE(
          mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]->rsp_data.data_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_internal_synch_uim_rpt_rsp[mmgsdi_task]);
      }
      else
      {
        UIM_MSG_ERR_2("Sync process for request at index 0x%x for file access 0x%x, has null mmgsdi_internal_synch_uim_rpt_rsp",
                      index, req_ptr->access.file.file_enum);
        mmgsdi_status = MMGSDI_ERROR;
      }
      mmgsdi_util_free_client_request_table_index(index);
    }
  }

  return mmgsdi_status;
} /* mmgsdi_uim_common_write_transparent */


/*===========================================================================
   FUNCTION:  MMGSDI_UIM_COMMON_SEND_APDU

   DESCRIPTION
     This function packages the send apdu  command and sends it to UIM
     for further processing
   DEPENDENCIES
     None

   RETURN VALUE
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_uim_common_send_apdu(
  mmgsdi_send_apdu_req_type* req_ptr)
{
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_SUCCESS;
  uim_slot_type                      uim_slot        = UIM_SLOT_NONE;
  uim_cmd_type*                      uim_cmd_ptr     = NULL;
  mmgsdi_client_req_extra_info_type* extra_param_ptr = NULL;
  uint32                             index           = 0;

  /*
   * Parameter checks.
   */
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr,
                              req_ptr->request_header.response_cb);
  /*
   * Convert to UIM slot.
   */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id,
                                               &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (req_ptr->data.data_ptr == NULL ||
      mmgsdi_util_is_slot_valid(req_ptr->request_header.slot_id) == FALSE)
  {
    return MMGSDI_ERROR;
  }

  if (req_ptr->data.data_len <= 0 ||
      (req_ptr->data.data_len >= MMGSDI_MAX_CHANNEL_DATA_RSP_LEN &&
       uim_is_extended_apdu_supported(uim_slot) == FALSE))
  {
    return MMGSDI_ERROR;
  }

  /*
   * 1) Get a UIM buffer for the request.
   * 2) Check for if the buffer is NULL or not.
   */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr,
                                     sizeof(uim_cmd_type));
  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  /*
   * 1) Populate UIM information.
   * 2) Fill the protocol to UIM_UICC in all cases, also for 2G cards. 
   *    The reason is that UIMDRV does not really use this information 
   *    and anyway APDU streaming is done accessing applications on the
   *    card as if it's a UICC, even when a 2G SIM is used. 
   */
  uim_cmd_ptr->hdr.command  = UIM_STREAM_ISO7816_APDU_F;
  uim_cmd_ptr->hdr.slot     = uim_slot;
  uim_cmd_ptr->hdr.protocol = UIM_UICC;
  uim_cmd_ptr->hdr.channel  =
    (uim_channel_type)mmgsdi_util_get_channel_id_from_cla(req_ptr->data.data_ptr[0]);

  /* Populate masking function pointers */
  mmgsdi_logging_populate_masking_fn_ptr(uim_cmd_ptr, MMGSDI_APP_NONE);

  /*
   * Get a new index from client req info table for response data.
   * Set the uim data pointer to the client request table data buffer.
   */
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS) 
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /*
   * Allocate and populate extra parameter.
   */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
                    req_ptr->request_header.request_type, (void*)req_ptr, &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
        index,
        &req_ptr->request_header,
        extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  uim_cmd_ptr->stream_iso7816_apdu.num_cmd_bytes = uint32touint16(req_ptr->data.data_len);
  uim_cmd_ptr->stream_iso7816_apdu.cmd_data      = extra_param_ptr->send_apdu_data.data.data_ptr;

  /*
   * Assign uim_cmd_ptr's user data.
   */
  uim_cmd_ptr->hdr.user_data = index;
  UIM_MSG_HIGH_0("Sending down send APDU command to UIM");

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }
  return mmgsdi_status;
} /* mmgsdi_uim_common_send_apdu */
