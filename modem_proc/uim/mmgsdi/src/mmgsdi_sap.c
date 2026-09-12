/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                    M M G S D I   U I M   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the UICC protocol processing support for MMGSDI.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2004 - 2006, 2008 - 2018, 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_sap.c#3 $$ $DateTime: 2020/10/18 15:27:05 $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/11/20   sg      Consider call ID and previous call type during call switch
04/09/18   rps     Correct slot_index when GET_RESPONSE in executed in slot 2
02/07/18   rps     Stop sending duplicate APDU for get response enable case
05/08/17   tq      SIM profile Query should return error if SIM is not present
05/08/17   vdc     Send resp from card if GET RESPONSE cache has only SW
05/04/17   vdc     Send GET RESPONSE to card when there is no data in cache
05/03/17   vdc     Fix to get ATR in back to back SAP connect and disconnect
04/07/17   ll      SAP fix for multiple get responses
10/03/16   dd      Removed lint comments
07/14/16   vdc     Support of SAP on slot2 when voice/data call is active
06/22/16   tkl     Add stub API for CNF
05/30/16   ar      Removing string prints from APDU and AUTH
05/13/16   tkl     Review of macros used by MMGSDI
05/03/16   tkl     F3 log prints cleanup
03/29/16   av      Preserve SAP ATR when cleaning up globals during SAP connect
12/19/15   vdc     Send SAP disconnect evt if card is powered up in SAP mode
12/17/15   vdc     Move event data to heap to reduce stack size
10/16/15   sp      Move client_req_table_info index variables to unsigned
10/05/15   sp      Reduce signed/unsigned conversions in UIM
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
07/29/15   sp      Remove UIM command in client request table
05/30/14   tl      Add error condition for SAP connect/disconnect in card error state
09/27/14   ar      Add support for multiple slots in BT-SAP
08/14/14   tl      Remove support for silent SAP PIN verficiation in modem
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
01/11/14   df      Fix off-target compilation error
01/06/14   df      Use v2.0 diag macros
10/16/13   tl      Remove MMGSDI_FEATURE_SAP
10/04/13   vdc     F3 message reduction
05/16/13   vdc     Replace memcpy with safer version memscpy
05/14/13   vdc     Memory optimizations for slot, app, pin, upin tables
04/15/13   tl      Remove SAP init, dereg and pin verify functionality
10/11/12   spo     Removed unnecessary F3 messages
10/01/12   abg     Removed Problem determining protocol type F3 message
08/29/12   at      Support for new API mmgsdi_sap_connect_ext
12/21/11   shr     Legacy GSDI removal updates
10/05/11   nb      Update to differentiate SAP Connect rejection due to active call
07/18/11   nb      Do not generate SAP event when connection failed
05/17/11   kk      Updates for Thread safety
05/04/11   nb      Pass slot information with SAP Event
11/30/10   yt      Including private UIMDRV header
10/14/10   yt      Updated handling of client req table entries in case of error
09/09/10   nb      Send SAP notification using client ID
06/24/10   yt      Klocwork fixes
04/07/10   nb      SAP Update
12/18/09   nb      Moved Dual Slot Changes
04/22/09   sun     Fixed includes
11/05/08   ssr     Resolved compiler error for RUIM enablement
09/22/08   nb      Resolved compiler warning
06/23/08   tml     Added featurization for BT in WM
09/10/07   tml     Used generic get ATR and card reader status
05/07/07   sp      Replaced calls to mem_malloc/free with mmgsdi_malloc/free
07/26/06   sun     Lint Fixes and Clean up
07/10/06   tml     compilation fix
07/07/06   sun     Lint Fixes
06/27/06   sun     Send UIM_RESET_SWITCH_UIM_PASSIVE_F during SAP_RESET
                   Fixed Alignment
02/22/06   tml     lint fix
12/20/05   sun     Populated extra_param for Sap_Disconnect and return proper
                   errors on invalid state
12/14/05   tml     MMGSDI Cleanup
12/05/05   sun     Added support for MMGSDI_SAP_CARD_READER_STATUS_REQ and
                   MMGSDI_SAP_RESET_REQ
11/09/05   sun     Fixed Lint Errors
11/03/05   tml     Fixed header
11/03/05   sun     On Disconnect, do not do a refresh immediately
10/28/05   sun     Fixed Sap Disconnect
10/20/05   sun     Added support for BT Deregisteration
08/29/05   tml     Fixed compilation
08/26/05   tml     Fixed compilation issue
08/26/05   sun     Added support for BT SAP
08/25/05   pv      Moved functions and data types relating to refresh to the
                   mmgsdi_gen module
07/28/05   sst     Fixed compile errors when SAP feature is not defined
07/28/05   sst     Fixed compile errors when SAP feature is not defined
06/06/05   sst     Initial version

===========================================================================*/


#include "uim_variation.h"
#include "comdef.h"
#include "intconv.h"
#include "mmgsdi.h"
#include "mmgsdi_sap.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "uim.h"
#include "uim_p.h"
#include "mmgsdi_sap_rsp.h"
#include "mmgsdi_gen.h"
#include "mmgsdi_evt.h"
#include "mmgsdi_cnf.h"
#include "mmgsdi_uim_common.h"

/*=============================================================================
  FUNCTION: mmgsdi_sap_sap_connect

  DESCRIPTION:
    Connects the UIM to the SAP.

  DEPENDENCIES
    None

  LIMITATIONS
    NONE

  RETURN VALUE
    mmgsdi_return_enum_type

  SIDE EFFECTS
    NONE
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_sap_sap_connect (
  const mmgsdi_sap_connect_req_type* req_ptr
)
{
  uim_cmd_type*                       uim_cmd_ptr            = NULL;
  uim_slot_type                       uim_slot               = UIM_SLOT_NONE;
  mmgsdi_return_enum_type             mmgsdi_status          = MMGSDI_ERROR;
  uint32                              index                  = 0;
  mmgsdi_client_req_extra_info_type * extra_param_ptr        = NULL;
  boolean                             reject_req             = FALSE;
  mmgsdi_slot_data_type             * slot_data_ptr          = NULL;
  mmgsdi_event_data_type            * event_data_ptr         = NULL;
  mmgsdi_session_id_type              mmgsdi_gw_session_id   = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_session_id_type              mmgsdi_1x_session_id   = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_app_enum_type                app_type               = MMGSDI_APP_NONE;
  mmgsdi_session_type_enum_type       mmgsdi_gw_session_type = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_session_type_enum_type       mmgsdi_1x_session_type = MMGSDI_MAX_SESSION_TYPE_ENUM;
  uint8                               mmgsdi_gw_prov_index   = MMGSDI_MAX_PROV_APPS;
  uint8                               mmgsdi_1x_prov_index   = MMGSDI_MAX_PROV_APPS;

  /* Parameter checks. */
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);
  if (slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* SAP connection should not be valid, even in LPM mode, except when card is
     powered down. Similar to NFC. */
  if (slot_data_ptr->mmgsdi_state == MMGSDI_STATE_NO_CARD &&
      slot_data_ptr->card_error_condition == MMGSDI_CARD_ERR_PWR_DN_CMD_NOTIFY)
  {
    return MMGSDI_ERROR;
  }

  /* Get GW prov index */
  if((MMGSDI_SUCCESS == mmgsdi_util_find_prov_session_for_slot(req_ptr->request_header.slot_id,
                                                              &mmgsdi_gw_session_id,
                                                              &app_type,
                                                              MMGSDI_TECH_3GPP)) &&
     (MMGSDI_SUCCESS == mmgsdi_util_get_session_type(mmgsdi_gw_session_id,
                                                    &mmgsdi_gw_session_type,
                                                    NULL)))
  {
    mmgsdi_gw_prov_index = mmgsdi_util_get_prov_index_from_session(mmgsdi_gw_session_type);
  }

  /* Get 1X prov index */
  if((MMGSDI_SUCCESS == mmgsdi_util_find_prov_session_for_slot(req_ptr->request_header.slot_id,
                                               &mmgsdi_1x_session_id,
                                               &app_type,
                                               MMGSDI_TECH_3GPP2)) &&
     (MMGSDI_SUCCESS == mmgsdi_util_get_session_type(mmgsdi_1x_session_id,
                                                    &mmgsdi_1x_session_type,
                                                    NULL)))
  {
    mmgsdi_1x_prov_index = mmgsdi_util_get_prov_index_from_session(mmgsdi_1x_session_type);
  }

  /* If emergency call is in progress then reject the request */
  if((mmgsdi_gw_prov_index < MMGSDI_MAX_PROV_APPS &&
      mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].emergency_call_active) ||
     (mmgsdi_1x_prov_index < MMGSDI_MAX_PROV_APPS &&
      mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].emergency_call_active))
  {
    reject_req = TRUE;
  }
  else
  {
    /* Check respective call status & decide to proceed or not */
    switch (req_ptr->condition)
    {
      case MMGSDI_SAP_CONN_COND_BLOCK_DATA:
        reject_req = (mmgsdi_gw_prov_index < MMGSDI_MAX_PROV_APPS &&
                      mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].data_call_active) ||
                     (mmgsdi_1x_prov_index < MMGSDI_MAX_PROV_APPS &&
                      mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].data_call_active);
        break;

      case MMGSDI_SAP_CONN_COND_BLOCK_VOICE:
        reject_req = (mmgsdi_gw_prov_index < MMGSDI_MAX_PROV_APPS &&
                      mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].voice_call_active) ||
                     (mmgsdi_1x_prov_index < MMGSDI_MAX_PROV_APPS &&
                      mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].voice_call_active);
        break;

      case MMGSDI_SAP_CONN_COND_BLOCK_NONE:
        /* Nothing to do */
        break;

      case MMGSDI_SAP_CONN_COND_BLOCK_VOICE_OR_DATA:
      default:
        reject_req = (mmgsdi_gw_prov_index < MMGSDI_MAX_PROV_APPS &&
                      (mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].voice_call_active ||
                       mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].data_call_active))
                       ||
                     (mmgsdi_1x_prov_index < MMGSDI_MAX_PROV_APPS &&
                      (mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].voice_call_active ||
                       mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].data_call_active));
        break;
    }
  }

  if (reject_req)
  {
    UIM_MSG_HIGH_7("Connection condition: 0x%x, GW ecall: 0x%x, GW voice call: 0x%x, GW data call: 0x%x, 1x ecall: 0x%x, 1x voice call: 0x%x, 1x data call: 0x%x",
                   req_ptr->condition,
                   (mmgsdi_gw_prov_index < MMGSDI_MAX_PROV_APPS) ? mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].emergency_call_active : FALSE,
                   (mmgsdi_gw_prov_index < MMGSDI_MAX_PROV_APPS) ? mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].voice_call_active : FALSE,
                   (mmgsdi_gw_prov_index < MMGSDI_MAX_PROV_APPS) ? mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_gw_prov_index].data_call_active : FALSE,
                   (mmgsdi_1x_prov_index < MMGSDI_MAX_PROV_APPS) ? mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].emergency_call_active : FALSE,
                   (mmgsdi_1x_prov_index < MMGSDI_MAX_PROV_APPS) ? mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].voice_call_active : FALSE,
                   (mmgsdi_1x_prov_index < MMGSDI_MAX_PROV_APPS) ? mmgsdi_generic_data_ptr->cm_call_info[mmgsdi_1x_prov_index].data_call_active : FALSE);

    return MMGSDI_SIM_BUSY;
  }

  /* Convert to UIM slot. */
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id, &uim_slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Allocate and populate extra parameter.*/
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
                                      MMGSDI_SAP_CONNECT_REQ,
                                      (void*)req_ptr,
                                      &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /*----------------------------------------------------------------------------
    1) Get a UIM buffer for the request.
    2) Check for if the buffer is NULL or not.
   -----------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));

  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate UIM information.*/
  uim_cmd_ptr->hdr.command  = UIM_RESET_SWITCH_UIM_PASSIVE_F;
  uim_cmd_ptr->hdr.protocol = UIM_NO_SUCH_MODE;
  uim_cmd_ptr->hdr.slot     = uim_slot;

  /*---------------------------------------------------------------------------
    Get a new index from client req info table for response data.
    Set the uim data pointer to the client request table data buffer.
   ----------------------------------------------------------------------------*/

  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info( index,
                                                                 &req_ptr->request_header,
                                                                  extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(event_data_ptr, sizeof(mmgsdi_event_data_type));

  if (event_data_ptr == NULL)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Assign uim_cmd_ptr's user data.*/
  uim_cmd_ptr->hdr.user_data = index;

  UIM_MSG_HIGH_0("Sending down SAP connect command to UIM");
  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }
  else
  {
    event_data_ptr->data.sap_connect.sap_mode      = MMGSDIBT_SERVER;
    event_data_ptr->evt                            = MMGSDI_SAP_CONNECT_EVT;
    event_data_ptr->data.sap_connect.slot          = req_ptr->request_header.slot_id;
    event_data_ptr->data.sap_connect.connect_state = MMGSDI_SAP_CONNECTING;

    mmgsdi_evt_notify_client(event_data_ptr);
  }

  mmgsdi_evt_free_data(event_data_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(event_data_ptr);

  return mmgsdi_status;
} /* mmgsdi_sap_sap_connect */


/*=============================================================================
  FUNCTION: mmgsdi_sap_sap_disconnect

  DESCRIPTION:
    Disconnects the UIM from the SAP.

  DEPENDENCIES
    None

  LIMITATIONS
    NONE

  RETURN VALUE
    mmgsdi_return_enum_type

  SIDE EFFECTS
    NONE
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_sap_sap_disconnect (
  const mmgsdi_sap_disconnect_req_type* req_ptr
)
{
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_ERROR;
  uint32                             index           = 0;
  mmgsdi_client_req_extra_info_type* extra_param_ptr = NULL;
  uint8                              slot_index      = 0;
  mmgsdi_slot_data_type            * slot_data_ptr   = NULL;

  /* Parameter checks.*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);
  if (slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* SAP disconnection should not be valid, even in LPM mode, except when card is
     powered down. Similar to NFC. */
  if (slot_data_ptr->mmgsdi_state == MMGSDI_STATE_NO_CARD &&
      slot_data_ptr->card_error_condition == MMGSDI_CARD_ERR_PWR_DN_CMD_NOTIFY)
  {
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_util_get_slot_index(req_ptr->request_header.slot_id,
                                             &slot_index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* If SAP is not in connected mode for requested SLOT than no need
     to handle disconnect request */
  if(req_ptr->sap_mode == MMGSDIBT_CLIENT || mmgsdi_sap_connected[slot_index] == FALSE)
  {
    /*Nothing to do. Return from here. */
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
                    MMGSDI_SAP_DISCONNECT_REQ,
                    (void*)req_ptr,
                    &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return mmgsdi_status;
  }

  /*----------------------------------------------------------------------------------
    Get a new index from client req info table for response data.
    Set the uim data pointer to the client request table data buffer.
   -----------------------------------------------------------------------------------*/

  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info( index,
                                                                  &req_ptr->request_header,
                                                                  extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    mmgsdi_util_free_client_request_table_index(index);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_queue_mmgsdi_uim_report_rsp(
                    index,
                    MMGSDI_SAP_DISCONNECT_REQ,
                    mmgsdi_status);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }

  return mmgsdi_status;

} /* mmgsdi_sap_sap_disconnect */


/*=============================================================================
  FUNCTION: mmgsdi_sap_sap_send_apdu

  DESCRIPTION:
    Sends an APDU from the SAP to the UIM.

  DEPENDENCIES
    None

  LIMITATIONS
    NONE

  RETURN VALUE
    mmgsdi_return_enum_type

  SIDE EFFECTS
    NONE
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_sap_sap_send_apdu (
  mmgsdi_sap_send_apdu_req_type* req_ptr
)
{
  mmgsdi_client_req_extra_info_type* extra_param_ptr = NULL;
  uint32                             index           = 0;
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_slot_data_type             *slot_data_ptr   = NULL;
  uint8                              slot_index      = 0;

  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);

  mmgsdi_status = mmgsdi_util_get_slot_index(req_ptr->request_header.slot_id,
                                             &slot_index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (!mmgsdi_sap_connected[slot_index])
  {
    UIM_MSG_ERR_0("BT is not connected");
    return MMGSDI_ERROR;
  }

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  if (mmgsdibt_pipe_apdu_ptr[slot_index] == NULL)
  {
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mmgsdibt_pipe_apdu_ptr[slot_index],
                                       sizeof(mmgsdi_cnf_type));
    if (mmgsdibt_pipe_apdu_ptr[slot_index] == NULL)
    {
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
  }

  /* If the response APDU is buffered, no need to go to the SIM. If
     response contains only status words then send the GET RESPONSE APDU to card */
  if ((req_ptr->data.data_ptr[UIM_7816_APDU_INSTRN_OFFSET] == GET_RESPONSE) &&
      (mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len > 2))
  {
    mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param( MMGSDI_SAP_SEND_APDU_REQ,
                                                                (void*)req_ptr,
                                                                &extra_param_ptr);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
      return mmgsdi_status;
    }

    mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
      return mmgsdi_status;
    }

    mmgsdi_status =mmgsdi_util_populate_client_request_table_info(index,
                                                                 &req_ptr->request_header,
                                                                 extra_param_ptr);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
      mmgsdi_util_free_client_request_table_index(index);
      return mmgsdi_status;
    }

    mmgsdi_status = mmgsdi_util_queue_mmgsdi_uim_report_rsp(index, MMGSDI_SAP_SEND_APDU_REQ, MMGSDI_SUCCESS);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      mmgsdi_util_free_client_request_table_index(index);
    }
  }
  else
  {
    mmgsdi_status = mmgsdi_uim_common_send_apdu((mmgsdi_send_apdu_req_type*)req_ptr);
  }

  return mmgsdi_status;
} /* mmgsdi_sap_sap_send_apdu */


/*=============================================================================
  FUNCTION: mmgsdi_sap_sap_power_on

  DESCRIPTION:
    Sends a SIM power on request for the SAP to the UIM.

  DEPENDENCIES
    None

  LIMITATIONS
    NONE

  RETURN VALUE
    mmgsdi_return_enum_type

  SIDE EFFECTS
    NONE
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_sap_sap_power_on (
  const mmgsdi_sap_power_on_req_type* req_ptr
)
{
  uim_cmd_type*                      uim_cmd_ptr     = NULL;
  uim_slot_type                      uim_slot        = UIM_SLOT_NONE;
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_SUCCESS;
  uint32                             index           = 0;
  mmgsdi_client_req_extra_info_type* extra_param_ptr = NULL;
  uint8                              slot_index      = 0;

  /* Parameter checks.*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr,
                              req_ptr->request_header.response_cb);

  mmgsdi_status = mmgsdi_util_get_slot_index(req_ptr->request_header.slot_id,
                                             &slot_index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (!mmgsdi_sap_connected[slot_index])
  {
    UIM_MSG_ERR_0("SAP power ON req error: BT is Not Connected");
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot.*/
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id, &uim_slot);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Allocate and populate extra parameter.*/
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param( MMGSDI_SAP_POWER_ON_REQ,
                                                             (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  /*------------------------------------------------------------------------------------
    1) Get a UIM buffer for the request.
    2) Check for if the buffer is NULL or not.
   -------------------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));

  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate UIM information.*/
  uim_cmd_ptr->hdr.command  = UIM_POWER_UP_UIM_PASSIVE_F;
  uim_cmd_ptr->hdr.protocol = UIM_NO_SUCH_MODE;
  uim_cmd_ptr->hdr.slot     = uim_slot;

  /*-------------------------------------------------------------------------------------
    Get a new index from client req info table for response data.
    Set the uim data pointer to the client request table data buffer.
   -------------------------------------------------------------------------------------*/

  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(index,
                                                                 &req_ptr->request_header,
                                                                 extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data.*/
  uim_cmd_ptr->hdr.user_data = index;
  UIM_MSG_HIGH_0("Sending down SAP power on command to UIM");

  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }
  return mmgsdi_status;
} /* mmgsdi_sap_sap_power_on */


/*=============================================================================
  FUNCTION: mmgsdi_sap_sap_power_off

  DESCRIPTION:
    Sends a SIM power off request for the SAP to the UIM.

  DEPENDENCIES
    None

  LIMITATIONS
    NONE

  RETURN VALUE
    mmgsdi_return_enum_type

  SIDE EFFECTS
    NONE
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_sap_sap_power_off (
  const mmgsdi_sap_power_off_req_type* req_ptr
)
{
  uim_cmd_type*                      uim_cmd_ptr     = NULL;
  uim_slot_type                      uim_slot        = UIM_SLOT_NONE;
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_SUCCESS;
  uint32                             index           = 0;
  mmgsdi_client_req_extra_info_type* extra_param_ptr = NULL;
  uint8                              slot_index      = 0;

  /* Parameter checks.*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr,
                              req_ptr->request_header.response_cb);

  mmgsdi_status = mmgsdi_util_get_slot_index(req_ptr->request_header.slot_id,
                                             &slot_index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (!mmgsdi_sap_connected[slot_index])
  {
    UIM_MSG_ERR_0("SAP power OFF req error: BT is Not Connected");
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot.*/
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id, &uim_slot);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  /* Allocate and populate extra parameter.*/
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param( MMGSDI_SAP_POWER_OFF_REQ,
                                                              (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /*-------------------------------------------------------------------------------------------
    1) Get a UIM buffer for the request.
    2) Check for if the buffer is NULL or not.
   -------------------------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));

  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate UIM information.*/
  uim_cmd_ptr->hdr.command  = UIM_POWER_DOWN_F;
  uim_cmd_ptr->hdr.protocol  = UIM_NO_SUCH_MODE;
  uim_cmd_ptr->hdr.slot     = uim_slot;
  uim_cmd_ptr->pdown.pdown_option = UIM_OPTION_NO_NOTIFY_PDOWN;

  /*-------------------------------------------------------------------------------------------
    Get a new index from client req info table for response data.
    Set the uim data pointer to the client request table data buffer.
   -------------------------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info( index,
                                                                  &req_ptr->request_header,
                                                                  extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data.*/
  uim_cmd_ptr->hdr.user_data = index;

  UIM_MSG_HIGH_0("Sending down SAP power off command to UIM");
  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }
  return mmgsdi_status;
} /* mmgsdi_sap_sap_power_off */

/*=============================================================================
  FUNCTION: mmgsdi_sap_sap_reset

  DESCRIPTION:
    Sends a SIM RESET request for the SAP to the UIM.

  DEPENDENCIES
    None

  LIMITATIONS
    NONE

  RETURN VALUE
    mmgsdi_return_enum_type

  SIDE EFFECTS
    NONE
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_sap_sap_reset (
  const mmgsdi_sap_reset_req_type* req_ptr
)
{
  uim_cmd_type*                      uim_cmd_ptr     = NULL;
  uim_slot_type                      uim_slot        = UIM_SLOT_NONE;
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_SUCCESS;
  uint32                             index           = 0;
  mmgsdi_client_req_extra_info_type* extra_param_ptr = NULL;
  uint8                              slot_index      = 0;

  /* Parameter checks.*/
  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr,
                              req_ptr->request_header.response_cb);

  mmgsdi_status = mmgsdi_util_get_slot_index(req_ptr->request_header.slot_id,
                                             &slot_index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (!mmgsdi_sap_connected[slot_index])
  {
    UIM_MSG_ERR_0("SAP reset req error: BT is Not Connected");
    return MMGSDI_ERROR;
  }

  /* Convert to UIM slot.*/
  mmgsdi_status = mmgsdi_util_convert_uim_slot(req_ptr->request_header.slot_id, &uim_slot);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /*Allocate and populate extra parameter.*/
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param( MMGSDI_SAP_RESET_REQ,
                                                              (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /*-------------------------------------------------------------------------------------------
    1) Get a UIM buffer for the request.
    2) Check for if the buffer is NULL or not.
   --------------------------------------------------------------------------------------------*/
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(uim_cmd_ptr, sizeof(uim_cmd_type));

  if (uim_cmd_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate UIM information.*/
  uim_cmd_ptr->hdr.command   = UIM_RESET_SWITCH_UIM_PASSIVE_F;
  uim_cmd_ptr->hdr.protocol  = UIM_NO_SUCH_MODE;
  uim_cmd_ptr->hdr.slot      = uim_slot;

  /*--------------------------------------------------------------------------------------------
    Get a new index from client req info table for response data.
    Set the uim data pointer to the client request table data buffer.
   -------------------------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_populate_client_request_table_info( index,
                                                                  &req_ptr->request_header,
                                                                  extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(uim_cmd_ptr);
    return mmgsdi_status;
  }

  /* Assign uim_cmd_ptr's user data. */
  uim_cmd_ptr->hdr.user_data = index;

  UIM_MSG_HIGH_0("Sending down SAP RESET command to UIM");
  mmgsdi_status = mmgsdi_send_cmd_to_uim_server(uim_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
  }
  return mmgsdi_status;
} /* mmgsdi_sap_sap_reset */


/*===========================================================================
FUNCTION MMGSDI_SAP_RESET_SAP_DATA

DESCRIPTION
  Free the SAP buffer when SAP gets disconnected

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_sap_reset_sap_data(
  mmgsdi_slot_id_enum_type slot_id
)
{
  uint8                        slot_index      = MMGSDI_SLOT_1_INDEX;
  mmgsdi_slot_data_type       *slot_data_ptr   = NULL;

  if(mmgsdi_util_get_slot_index(slot_id, &slot_index) != MMGSDI_SUCCESS)
  {
    return;
  }
  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
  if (slot_data_ptr == NULL)
  {
    return;
  }

  /* Free SAP buffer of corresponding slot where SAP disconnect req arrived */
  if (mmgsdibt_pipe_apdu_ptr[slot_index])
  {
    MMGSDIUTIL_TMC_MEM_FREE(mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(mmgsdibt_pipe_apdu_ptr[slot_index]);
  }

  mmgsdi_sap_connected[slot_index] = FALSE;
} /* mmgsdi_sap_reset_sap_data */


/* =============================================================================
   FUNCTION:      MMGSDI_CNF_BUILD_SAP_SEND_APDU_DATA

   DESCRIPTION:
     This function builds the send APDU confirmation data

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.

   SIDE EFFECTS:
     None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_cnf_build_sap_send_apdu_data(
  mmgsdi_sap_send_apdu_cnf_type           * sap_send_apdu_cnf_ptr,
  mmgsdi_return_enum_type                   mmgsdi_status,
  const mmgsdi_client_req_extra_info_type * extra_param_ptr,
  const mmgsdi_data_type                  * apdu_data_ptr)
{
  mmgsdi_return_enum_type    build_cnf_status       = MMGSDI_SUCCESS;
  uint32                     req_data_len           = 0;
  uint8                      slot_index             = 0;

  MMGSDIUTIL_RETURN_IF_NULL(sap_send_apdu_cnf_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return MMGSDI_SUCCESS;
  }

  if (extra_param_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  if(mmgsdi_util_get_slot_index(extra_param_ptr->sap_send_apdu_data.slot_id,
                                &slot_index) != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  /* If the INS is GET RESPONSE and the response data (data + status words) is
     present in cache then populate the confirmation pointer from it.
     If the INS is GET RESPONSE and there is no response data/only has status words
     in cache then populate the confirmation pointer from card's response.
     If the INS is not GET RESPONSE then populate the confirmation pointer
     from card's response */
  if (extra_param_ptr->sap_send_apdu_data.data.data_ptr [UIM_7816_APDU_INSTRN_OFFSET] ==
        GET_RESPONSE &&
      mmgsdibt_pipe_apdu_ptr[slot_index] != NULL &&
      mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len > 2)
  {
    /* Length requested by the client */
    req_data_len = extra_param_ptr->sap_send_apdu_data.data.data_ptr[UIM_CASE2_7816_LE_OFFSET];

    UIM_MSG_HIGH_2("Requested Length 0x%x, Avaiable Length 0x%x",
                   req_data_len, 
                   mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len);
    if(mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len == 0)
    {
      return MMGSDI_ERROR;
    }
    else if ((req_data_len == 0)     ||
             ((req_data_len + 0x02 ) >= 
              mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len))
    {
      /* Second condition is when user asks for the amount of data greater
       * than or equal to SW2 of original command. But since we have status
       * words for GET RESPONSE in buffer as well we need to adjust the buffer
       * length by 2.
       * Buffer Content - Length = data_len + Status Words Length
       *                  Data   = [Data From card] SW1 SW2
       *                           <-- data_len -->
       */
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
        sap_send_apdu_cnf_ptr->apdu_data.data_ptr,
        mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len);

      if (sap_send_apdu_cnf_ptr->apdu_data.data_ptr == NULL)
      {
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      sap_send_apdu_cnf_ptr->apdu_data.data_len = 
        mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len;
      mmgsdi_memscpy(sap_send_apdu_cnf_ptr->apdu_data.data_ptr,
                     mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len,
                     mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_ptr,
                     mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len);

      MMGSDIUTIL_TMC_MEM_FREE(mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_ptr);
      mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len = 0x00;

      /* If we are returning all available data, clear the intermediate SWs*/
      sap_send_apdu_cnf_ptr->implicit_get_rsp_sw1 = 0x00;
      sap_send_apdu_cnf_ptr->implicit_get_rsp_sw2 = 0x00;

      MMGSDIUTIL_TMC_MEM_FREE(mmgsdibt_pipe_apdu_ptr[slot_index]);
    }
    else /* where requested lenght < available length*/
    {
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
        sap_send_apdu_cnf_ptr->apdu_data.data_ptr,
        req_data_len);

      if (sap_send_apdu_cnf_ptr->apdu_data.data_ptr == NULL)
      {
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      mmgsdi_memscpy(sap_send_apdu_cnf_ptr->apdu_data.data_ptr,
                     req_data_len,
                     mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_ptr,
                     req_data_len);

      mmgsdi_memsmove(mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_ptr,
                      mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len,
                      &mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_ptr[req_data_len],
                      mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len - req_data_len);

      sap_send_apdu_cnf_ptr->apdu_data.data_len = req_data_len;
      mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len -= req_data_len;

      sap_send_apdu_cnf_ptr->implicit_get_rsp_sw1 = GSDI_SIM_SW1_PROCEDURE_BYTE_61;
      if (mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len >= 
            MMGSDI_GEN_MAX_GET_RESPONSE_REQ_LEN) 
      {
        sap_send_apdu_cnf_ptr->implicit_get_rsp_sw2 = MMGSDI_GEN_MAX_GET_RESPONSE_REQ_LEN;
      }
      else
      {
        /* Need to subtract the 2 byte of final status words for sw2 */
        sap_send_apdu_cnf_ptr->implicit_get_rsp_sw2 = 
          mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len - 2;
      }
    }
  }
  else
  {
    if (apdu_data_ptr == NULL)
    {
      return MMGSDI_ERROR;
    }
    build_cnf_status = mmgsdi_cnf_build_send_apdu_data(sap_send_apdu_cnf_ptr,
                                                       mmgsdi_status,
                                                       extra_param_ptr,
                                                       apdu_data_ptr,
                                                       extra_param_ptr->sap_send_apdu_data.slot_id);
  }

  return build_cnf_status;
} /* mmgsdi_cnf_build_sap_send_apdu_data */


/* =============================================================================
   FUNCTION:      MMGSDI_CNF_BUILD_SAP_GET_ATR_DATA

   DESCRIPTION:
     This function builds the get ATR confirmation data

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.

   SIDE EFFECTS:
     None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_cnf_build_sap_get_atr_data(
  mmgsdi_sap_get_atr_cnf_type       * get_atr_cnf_ptr,
  mmgsdi_return_enum_type             mmgsdi_status,
  mmgsdi_slot_id_enum_type            slot_id)
{
  uim_atr_buf_type card_atr = {0, {0}};
  uim_slot_type    uim_slot = UIM_SLOT_NONE;

  MMGSDIUTIL_RETURN_IF_NULL(get_atr_cnf_ptr);

  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    /* Convert to UIM slot. */
    mmgsdi_status = mmgsdi_util_convert_uim_slot(slot_id, &uim_slot);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }

    if(UIM_SUCCESS != uim_retrieve_atr(uim_slot, &card_atr))
    {
      return MMGSDI_ERROR;
    }

    if(card_atr.num_bytes > MMGSDI_MAX_ATR_LEN)
    {
      card_atr.num_bytes = MMGSDI_MAX_ATR_LEN;
    }

    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(get_atr_cnf_ptr->atr_data.data_ptr,
                                       card_atr.num_bytes);
    if (get_atr_cnf_ptr->atr_data.data_ptr == NULL)
    {
      return MMGSDI_ERROR;
    }

    get_atr_cnf_ptr->atr_data.data_len = card_atr.num_bytes;

    if (get_atr_cnf_ptr->atr_data.data_len > 0)
    {
      mmgsdi_memscpy(get_atr_cnf_ptr->atr_data.data_ptr,
                     get_atr_cnf_ptr->atr_data.data_len,
                     card_atr.data,
                     card_atr.num_bytes);
    }
  }
  return MMGSDI_SUCCESS;
} /* mmgsdi_cnf_build_sap_get_atr_data */


/* =============================================================================
   FUNCTION:      MMGSDI_CNF_BUILD_SAP_DISCONNECT_DATA

   DESCRIPTION:
     This function builds the SAP Disconnection confirmation data

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.

   SIDE EFFECTS:
     None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_cnf_build_sap_disconnect_data(
  mmgsdi_sap_disconnect_cnf_type    * disconnect_cnf_ptr,
  mmgsdi_return_enum_type             mmgsdi_status,
  mmgsdi_disconnect_mode_enum_type    disconnect_mode)
{
  MMGSDIUTIL_RETURN_IF_NULL(disconnect_cnf_ptr);

  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    disconnect_cnf_ptr->disconnect_mode = disconnect_mode;
  }
  return MMGSDI_SUCCESS;
} /* mmgsdi_cnf_build_sap_disconnect_data */

