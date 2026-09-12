/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                    M M G S D I   U I M   R S P   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the UIM response processing support for MMGSDI.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2004 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_sap_rsp.c#2 $$ $DateTime: 2019/10/30 06:56:05 $


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/08/17   tq      SIM profile Query should return error if SIM is not present
05/04/17   vdc     Send GET RESPONSE to card when there is no data in cache
05/03/17   vdc     Fix to get ATR in back to back SAP connect and disconnect
03/14/17   nc      Better checks for return values  
01/12/17   nc      Better check of return values 
10/03/16   dd      Removed lint comments
05/03/16   tkl     F3 log prints cleanup
04/19/16   ar      Correcting the index to MMGSDI_MAX_CLIENT_REQ_INFO
03/29/16   av      Preserve SAP ATR when cleaning up globals during SAP connect
12/19/15   vdc     Send SAP disconnect evt if card is powered up in SAP mode
12/17/15   vdc     Move event data to heap to reduce stack size
10/16/15   sp      Move client_req_table_info index variables to unsigned
10/13/15   nr      Adding length check while copying APDU response
10/05/15   sp      Reduce signed/unsigned conversions in UIM
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
07/29/15   sp      Remove UIM command in client request table
04/29/15   bcho    Support for App specific cache
04/06/15   ar      Remove duplicate code
11/12/14   vdc     Send card error evt if NO_ATR is received after slot power up
09/27/14   ar      Add support for multiple slots in BT-SAP
09/17/14   vr      Correction in heap variable created in stack reduction
09/09/14   vr      Reduce stack usage
08/14/14   tl      Remove support for silent SAP PIN verficiation in modem
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
07/28/14   hh      Do not return error if getting session id fail
05/12/14   bcho    NULL check added before dereferencing extra_param_ptr
02/25/14   bcho    Added support to store OPLMN List in EFS and cache it
01/11/14   df      Fix off-target compilation error
01/10/14   df      Use v2.0 diag macros
01/09/14   ar      Replace old Macros with MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE
12/11/13   df      Remove unused memory paging feature
11/27/13   tl      Introduce refresh command queue
10/27/13   df      Send error_evt during sap connect rsp
10/16/13   tl      Remove MMGSDI_FEATURE_SAP
10/04/13   vdc     F3 message reduction
09/20/13   yt      Remove duplicate code
06/24/13   vdc     Added support for triple SIM
05/16/13   vdc     Replace memcpy with safer version memscpy
05/14/13   vdc     Memory optimizations for slot, app, pin, upin tables
02/12/13   at      Don't error connection request if fetching session id fails
12/05/12   vv      Added support for parallel processing in MMGSDI
10/11/12   spo     Removed unnecessary F3 messages
10/01/12   abg     Removed Problem determining protocol type F3 message
09/28/12   abg     Updated ERR to MSG_ERROR
03/16/12   yt      Modify references to gsdi_task_tcb
12/21/11   shr     Legacy GSDI removal updates
10/05/11   nb      Update to differentiate SAP Connect rejection due to active call
07/18/11   nb      Do not generate connect event in case connection failed
06/24/11   nb      Populated the correct structure in event
05/04/11   nb      Pass slot information with SAP events
12/22/10   yt      Removed FEATURE_UIM_ZI_MEMORY_REDUCTION
11/30/10   yt      Including private UIMDRV header
10/05/10   nmb     Removed extra bytes from task_cmd buffer
09/09/10   nb      Send SAP notification using client ID
06/15/10   nb      Check mmgsdi global data in place of gsdi global data for
                   sim availability
04/07/10   nb      SAP Enhancements
03/12/10   adp     Fixing KW errors
12/18/09   nb      Moved Dual Slot Changes
09/26/09   kp      ZI memory reduction changes
04/22/09   sun     Fixed includes
11/05/08   ssr     Resolved compiler error for RUIM enablement
06/23/08   tml     Added featurization for BT in WM
06/04/08   tml     Added sw to cnf header and messages cleanup
11/15/07   tml     Use regular rex function for critical sections for
                   mmgsdi_task_rsp_q to prevent gsdi task stuck when synchronous
                   commands were issued
09/23/07   sk      Put a lock around mmgsdi_task_rsp
09/10/07   tml     Used generic get ATR and card reader functions and removed
                   channel atr function from the file.
03/19/07   sun     Fixed Lint Error
07/26/06   sun     Lint Fix
07/07/06   sun     Lint Fixes
06/12/06   sun     Set mmgsdi_sap_state  on Power Off and Power On
12/14/05   tml     MMGSDI Cleanup
12/05/05   sun     Added support for MMGSDI_SAP_CARD_READER_STATUS_REQ and
                   MMGSDI_SAP_RESET_REQ
11/16/05   sun     Propagated mmgsdi_status back to the clients
11/09/05   sun     Fixed Lint Errors
11/03/05   tml     Fixed header
10/28/05   sun     Fixed Sap Disconnect
10/20/05   sun     Added support for BT Deregisteration
08/26/05   sun     Added support for BT SAP
07/28/05   sst     Fixed compile errors when SAP feature is not defined
06/06/05   sst     Initial version

===========================================================================*/


#include "uim_variation.h"
#include "comdef.h"
#include "intconv.h"
#include "mmgsdi.h"
#include "mmgsdi_icc_rsp.h"
#include "mmgsdi_sap_rsp.h"
#include "mmgsdi_uicc_rsp.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "rex.h"
#include "uim.h"
#include "uim_p.h"
#include "mmgsdi_cnf.h"
#include "mmgsdi_evt.h"
#include "mmgsdi_common_rsp.h"
#include "mmgsdi_sap.h"
#include "mmgsdi_gen.h"

static mmgsdi_return_enum_type mmgsdi_sap_rsp_copy_apdu_data(
  mmgsdi_send_apdu_cnf_type               * apdu_cnf_ptr,
  const mmgsdi_client_req_extra_info_type * extra_param_ptr,
  const mmgsdi_data_type                  * uim_rsp_data_ptr);

/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_is_sim_in_slot_available

  DESCRIPTION:
    Function is called to determine whether or not the SIM in the SLOT
    requested is available.

  PARAMETERS:
    slot: The slot to check availabilty for.
-----------------------------------------------------------------------------*/
static boolean mmgsdi_is_sim_in_slot_available (
  mmgsdi_slot_id_enum_type slot
)
{
  boolean                 sim_present   = FALSE;
  mmgsdi_slot_data_type * slot_data_ptr = NULL;

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot);

  if(slot_data_ptr != NULL)
  {
    if((slot_data_ptr->mmgsdi_state == MMGSDI_STATE_NO_CARD)&&
       (slot_data_ptr->card_error_condition == MMGSDI_CARD_ERR_SAP_CONNECTED))
    {
      sim_present = TRUE;
    }
  }

  return sim_present;
} /* mmgsdi_is_sim_in_slot_available */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_sap_connect

  DESCRIPTION:
    Response to connecting the UIM to the SAP.

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.
-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_sap_connect(
  const mmgsdi_uim_report_rsp_type*   uim_rsp_ptr)
{
  mmgsdi_request_header_type         req_header;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type* extra_param_ptr  = NULL;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;
  mmgsdi_session_id_type             session_id       = 0x00;
  mmgsdi_slot_data_type             *slot_data_ptr    = NULL;
  mmgsdi_slot_id_enum_type           slot_id          = MMGSDI_MAX_SLOT_ID_ENUM;
  uint8                              slot_index       = 0;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  mmgsdi_status = mmgsdi_util_get_client_request_table_info( uim_rsp_ptr->rsp_data_index,
                                                             &req_header,
                                                             &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  MMGSDIUTIL_RETURN_IF_NULL(extra_param_ptr);

  slot_id       = extra_param_ptr->sap_connect_data.slot_id;

  mmgsdi_status = mmgsdi_util_get_slot_index(slot_id, &slot_index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (uim_rsp_ptr->is_uim_success)
  {
    report_status = MMGSDI_SUCCESS;
  }
  else
  {
    report_status = uim_rsp_ptr->mmgsdi_error_status;
  }

  if (report_status == MMGSDI_SUCCESS)
  {
    mmgsdi_sap_connected[slot_index]    = TRUE;

    MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
    if(slot_data_ptr == NULL)
    {
      MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;
      return MMGSDI_ERROR;
    }

    slot_data_ptr->card_error_condition = MMGSDI_CARD_ERR_SAP_CONNECTED;

    /* Send out the Card Error event and perform session and app clean-up
       (queue error for pending cmds, end any ongoing refreshes, clear cache) */
    mmgsdi_evt_build_and_send_card_error(TRUE,
                                         0,
                                         slot_id,
                                         MMGSDI_CARD_ERR_UNKNOWN_ERROR);

    /* Update app_info_ptr->session_closed_cause as it will be sent to clients
       when closing sessions inside mmgsdi_util_preinit_mmgsdi() */
    mmgsdi_util_update_session_closed_cause_for_slot(slot_id,
                                     MMGSDI_SESSION_CLOSED_CAUSE_CARD_REMOVED);

    if (MMGSDI_SUCCESS !=  mmgsdi_util_get_mmgsdi_session_on_ch_zero(slot_id,
                                                                     NULL,
                                                                     &session_id))
    {
      /* OK to not get a session_id since we might be re-initializing
         after a disconnect, so proceed with connection */
      session_id = 0;
    }

    mmgsdi_util_preinit_mmgsdi(session_id,
                               MMGSDI_APP_NONE,
                               MMGSDI_REFRESH_RESET,
                               slot_id,
                               FALSE);

    /* SAP commands are not processed unless this condition is equal to
       MMGSDI_CARD_ERR_SAP_CONNECTED. mmgsdi_util_preinit_mmgsdi() will have
       reset it to MMGSDI_CARD_ERR_UNKNOWN_ERROR so correct this now */
    slot_data_ptr->card_error_condition = MMGSDI_CARD_ERR_SAP_CONNECTED;

    MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             NULL,
                                             FALSE, /* sw not required for  sap connect */
                                             uim_rsp_ptr->status_word);

  /* If connection Failed - No Event Needed */
  if(report_status == MMGSDI_SUCCESS)
  {
    mmgsdi_event_data_type            *event_data_ptr = NULL;

    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(event_data_ptr, sizeof(mmgsdi_event_data_type));

    if (event_data_ptr != NULL)
    {
      event_data_ptr->data.sap_connect.connect_state = MMGSDI_SAP_CONNECTED;
      event_data_ptr->evt                            = MMGSDI_SAP_CONNECT_EVT;
      event_data_ptr->data.sap_connect.sap_mode      = MMGSDIBT_SERVER;
      event_data_ptr->data.sap_connect.slot          = slot_id;

      mmgsdi_evt_notify_client(event_data_ptr);

      mmgsdi_evt_free_data(event_data_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(event_data_ptr);
    }
  }

  return mmgsdi_status;
} /* mmgsdi_sap_rsp_sap_connect */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_sap_disconnect

  DESCRIPTION:
    Response to disconnect the UIM to the SAP.

  PARAMETERS:
    uim_rsp_ptr      : UIM response data.
-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_sap_disconnect(
  const mmgsdi_uim_report_rsp_type*   uim_rsp_ptr)
{
  mmgsdi_request_header_type         req_header;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type* extra_param_ptr  = NULL;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;
  uint8                              slot_index       = MMGSDI_SLOT_1_INDEX;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  mmgsdi_status = mmgsdi_util_get_client_request_table_info(uim_rsp_ptr->rsp_data_index,
                                                            &req_header,
                                                            &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  MMGSDIUTIL_RETURN_IF_NULL(extra_param_ptr);

  if(uim_rsp_ptr->is_uim_success)
  {
    /* We reach here when SAP was connected earlier and disconnction is in
       progress. When SAP got connected, card error event was sent out to all
       clients. So, there is no need to ask votes to the clients and we can
       directly do card reset. As this is equivalent to fresh power up, the
       protocol is set to UICC mode */
    (void)mmgsdi_gen_build_card_reset(req_header.client_id,
                                      req_header.slot_id,
                                      MMGSDI_CARD_RESET_REQ,
                                      UIM_UICC,
                                      MMGSDI_RESET_CAUSE_GENERAL,
                                      MMGSDI_CARD_POWER_UP_INITIAL_PUP,
                                      mmgsdi_util_internal_cb,
                                      0);
  }

  if (uim_rsp_ptr->is_uim_success)
  {
    report_status = MMGSDI_SUCCESS;
  }
  else
  {
    report_status = MMGSDI_ERROR;
  }

  if(mmgsdi_util_get_slot_index(extra_param_ptr->sap_disconnect_data.slot_id, &slot_index) == MMGSDI_SUCCESS &&
     mmgsdi_sap_connected[slot_index])
  {
    mmgsdi_evt_build_and_send_sap_disconnected(extra_param_ptr->sap_disconnect_data.slot_id);
    mmgsdi_sap_reset_sap_data(extra_param_ptr->sap_disconnect_data.slot_id);
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             NULL,
                                             FALSE, /* sw not required for sap connect */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
} /* mmgsdi_sap_rsp_sap_disconnect */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_SAP_RSP_BUILD_CNF

   DESCRIPTION:
     This function builds the SAP Confirmation and put the
     it onto the MMGSDI task queue.

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

-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_build_cnf(
  const mmgsdi_generic_rsp_type        *sap_rsp_ptr
)
{
  mmgsdi_return_enum_type            build_status   = MMGSDI_SUCCESS;
  mmgsdi_request_header_type         req_header;
  mmgsdi_client_req_extra_info_type *extra_param_ptr = NULL;
  mmgsdi_sw_status_type              status_word;

  UIM_MSG_HIGH_0("MMGSDI_SAP_RSP_BUILD_CNF");

  /* status word not applicable to sap rsp */
  status_word.sw1 = 0x00;
  status_word.sw2 = 0x00;

  /*
   * Check input pointers.
   */
  MMGSDIUTIL_RETURN_IF_NULL(sap_rsp_ptr);

  build_status = mmgsdi_util_get_client_request_table_info(
                                sap_rsp_ptr->rsp_data_index,
                                &req_header,
                                &extra_param_ptr);

  if (build_status != MMGSDI_SUCCESS)
  {
    return build_status;
  }

  build_status = mmgsdi_cnf_build_and_queue(sap_rsp_ptr->status,
                                            &req_header,
                                            extra_param_ptr,
                                            NULL,
                                            FALSE,
                                            status_word);

  return build_status;
} /* mmgsdi_sap_rsp_build_cnf */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_sap_send_apdu

  DESCRIPTION:
    The response to sending an APDU from the SAP to the UIM.

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.
-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_sap_send_apdu(
  const mmgsdi_uim_report_rsp_type*     uim_rsp_ptr
)
{
  mmgsdi_request_header_type         req_header;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type* extra_param_ptr  = NULL;
  uint8                              slot_index       = 0;

  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  /* Get the request header.
   */
  mmgsdi_status =mmgsdi_util_get_client_request_table_info( uim_rsp_ptr->rsp_data_index,
                                                            &req_header,
                                                            &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_util_get_slot_index(extra_param_ptr->sap_send_apdu_data.slot_id,
                                             &slot_index);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  else if (extra_param_ptr->sap_send_apdu_data.data.data_ptr [UIM_7816_APDU_INSTRN_OFFSET] ==
             GET_RESPONSE)
  {
    if (mmgsdi_is_sim_in_slot_available(req_header.slot_id))
    {
      report_status = MMGSDI_SUCCESS;
    }
    else
    {
      report_status = MMGSDI_ERROR;
    }
    mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                               &req_header,
                                               extra_param_ptr,
                                               (void *)&uim_rsp_ptr->rsp_data,
                                               FALSE,
                                               uim_rsp_ptr->status_word);
    return mmgsdi_status;
  }
  else
  {
    MMGSDIUTIL_RETURN_IF_NULL(mmgsdibt_pipe_apdu_ptr[slot_index]);
    MMGSDIUTIL_TMC_MEM_FREE(mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_ptr);
    mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf.apdu_data.data_len = 0;
    /* Store the APDU for later use */
    mmgsdi_status = mmgsdi_sap_rsp_copy_apdu_data(
                      &mmgsdibt_pipe_apdu_ptr[slot_index]->sap_send_apdu_cnf,
                                          extra_param_ptr,
                                          &uim_rsp_ptr->rsp_data);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }

    mmgsdi_status = mmgsdi_common_rsp_send_apdu( uim_rsp_ptr);
  }
  return mmgsdi_status;
} /* mmgsdi_sap_rsp_sap_send_apdu*/


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_sap_power

  DESCRIPTION:
    The response to sending a SIM power on/off request for the SAP to the UIM.

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.
    mmgsdi_cnf_ptr: Returned MMGSDI confirmation data.
-----------------------------------------------------------------------------*/
static mmgsdi_return_enum_type mmgsdi_sap_rsp_sap_power(
  const mmgsdi_uim_report_rsp_type*    uim_rsp_ptr,
  mmgsdi_card_state_enum_type          sap_state)
{
  mmgsdi_request_header_type         req_header;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type* extra_param_ptr  = NULL;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;
  mmgsdi_slot_data_type             *slot_data_ptr    = NULL;

  /*
   * Check input pointers.
   */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  mmgsdi_status =mmgsdi_util_get_client_request_table_info(uim_rsp_ptr->rsp_data_index,
                                                           &req_header,
                                                           &extra_param_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (uim_rsp_ptr->is_uim_success)
  {
    report_status = MMGSDI_SUCCESS;
    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(extra_param_ptr->sap_power_data.slot_id);

    if (slot_data_ptr != NULL)
    {
      slot_data_ptr->mmgsdi_sap_state = sap_state;
    }
  }
  else
  {
    report_status = MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             extra_param_ptr,
                                             NULL,
                                             FALSE, /* sw not required
                                                       for sap power on/off */
                                             uim_rsp_ptr->status_word);
  return mmgsdi_status;
} /* mmgsdi_sap_rsp_sap_power */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_sap_power_on

  DESCRIPTION:
    The response to sending a SIM power on request for the SAP to the UIM.

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.
    mmgsdi_cnf_ptr: Returned MMGSDI confirmation data.
-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_sap_power_on(
  const mmgsdi_uim_report_rsp_type*    uim_rsp_ptr)
{
  return mmgsdi_sap_rsp_sap_power(uim_rsp_ptr,
                                  MMGSDI_STATE_CARD_INSERTED);
} /* mmgsdi_sap_rsp_power_on */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_sap_power_off

  DESCRIPTION:
    The response to sending a SIM power off request for the SAP to the UIM.

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.
    mmgsdi_cnf_ptr: Returned MMGSDI confirmation data.
-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_sap_power_off(
  const mmgsdi_uim_report_rsp_type*     uim_rsp_ptr)
{
  return mmgsdi_sap_rsp_sap_power(uim_rsp_ptr,
                                  MMGSDI_STATE_NO_CARD);
} /* mmgsdi_sap_rsp_pwer_off */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_sap_reset

  DESCRIPTION:
    The response to sending a SIM RESET request for the SAP to the UIM.

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.
    mmgsdi_cnf_ptr: Returned MMGSDI confirmation data.
-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_sap_reset(
  const mmgsdi_uim_report_rsp_type*     uim_rsp_ptr)
{
  mmgsdi_request_header_type         req_header;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_client_req_extra_info_type* extra_param_ptr  = NULL;
  mmgsdi_return_enum_type            report_status    = MMGSDI_SUCCESS;

  /*
   * Check input pointers.
   */
  MMGSDIUTIL_RETURN_IF_NULL(uim_rsp_ptr);

  mmgsdi_status =mmgsdi_util_get_client_request_table_info(uim_rsp_ptr->rsp_data_index,
                                                           &req_header,
                                                           &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if (uim_rsp_ptr->is_uim_success)
  {
    report_status = MMGSDI_SUCCESS;
  }
  else
  {
    report_status = MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_cnf_build_and_queue(report_status,
                                             &req_header,
                                             NULL,
                                             NULL,
                                             FALSE, /* sw not required
                                                       for sap reset */
                                             uim_rsp_ptr->status_word);

  return mmgsdi_status;
} /* mmgsdi_sap_rsp_reset */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_build_sap_rsp

  DESCRIPTION:
    build and queue a sap response

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.
    mmgsdi_cnf_ptr: Returned MMGSDI confirmation data.
-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_sap_rsp_build_sap_rsp (
  mmgsdi_return_enum_type     mmgsdi_status,
  int32                       index )
{
  mmgsdi_task_cmd_type     *task_cmd_ptr = NULL;
  mmgsdi_task_enum_type     mmgsdi_task  = MMGSDI_TASK_MAX;

  mmgsdi_task = mmgsdi_util_get_current_task();
  if(mmgsdi_task >= MMGSDI_NUM_TASKS)
  {
    /* In case the current task is undetermined,
       queue to main task */
    mmgsdi_task = MMGSDI_TASK_MAIN;
  }

  /* Memory allocation for msg_ptr and check for allocation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(task_cmd_ptr,
    sizeof(mmgsdi_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  task_cmd_ptr->cmd.cmd_enum                                          = MMGSDI_SAP_REPORT_RSP;
  task_cmd_ptr->cmd.cmd.sap_report_rsp.request_header.client_id       = 0;
  task_cmd_ptr->cmd.cmd.sap_report_rsp.request_header.request_type    = MMGSDI_SAP_REPORT_RSP;
  task_cmd_ptr->cmd.cmd.sap_report_rsp.request_header.slot_id         = MMGSDI_SLOT_AUTOMATIC;
  task_cmd_ptr->cmd.cmd.sap_report_rsp.request_header.client_data     = 0;
  task_cmd_ptr->cmd.cmd.sap_report_rsp.request_header.response_cb     = NULL;
  task_cmd_ptr->cmd.cmd.sap_report_rsp.rsp_data_index                 = index;
  task_cmd_ptr->cmd.cmd.sap_report_rsp.status                         = mmgsdi_status;
  (void)q_link(task_cmd_ptr, &task_cmd_ptr->link);
  /* Put the message on the queue */
  q_put(&mmgsdi_task_rsp_q[mmgsdi_task], &task_cmd_ptr->link);

  /* Set the command queue signal for */
  /* receive task                     */
  if(mmgsdi_task == MMGSDI_TASK_MAIN)
  {
    (void) rex_set_sigs(UIM_GSDI_TCB, MMGSDI_TASK_RSP_Q_SIG);
  }
  else
  {
    (void) rex_set_sigs(rex_self(), MMGSDI_TASK_RSP_Q_SIG);
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_sap_rsp_build_sap_rsp */


/*-----------------------------------------------------------------------------
  FUNCTION: mmgsdi_sap_rsp_copy_apdu_data

  DESCRIPTION:
     Store the APDU for later use

  PARAMETERS:
    apdu_cnf_ptr    : Used to store the response data.
    extra_param_ptr : client request extra info
    uim_rsp_data_ptr: UIM response data.
-----------------------------------------------------------------------------*/
static mmgsdi_return_enum_type mmgsdi_sap_rsp_copy_apdu_data(
  mmgsdi_send_apdu_cnf_type               * apdu_cnf_ptr,
  const mmgsdi_client_req_extra_info_type * extra_param_ptr,
  const mmgsdi_data_type                  * uim_rsp_data_ptr)
{
  uint16                   status_bytes     = 2;

  MMGSDIUTIL_RETURN_IF_NULL_3(apdu_cnf_ptr, extra_param_ptr, uim_rsp_data_ptr);

  /* APDU response data might be zero when UIMDRV rejects the APDU
     and not sending to the card */
  if(uim_rsp_data_ptr->data_len == 0)
  {
    return MMGSDI_ERROR;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(apdu_cnf_ptr->apdu_data.data_ptr,
                                     uim_rsp_data_ptr->data_len);
  if (apdu_cnf_ptr->apdu_data.data_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  mmgsdi_memscpy(apdu_cnf_ptr->apdu_data.data_ptr,
                 uim_rsp_data_ptr->data_len,
                 uim_rsp_data_ptr->data_ptr,
                 uim_rsp_data_ptr->data_len);

  apdu_cnf_ptr->implicit_get_rsp_sw1 =
     uim_rsp_data_ptr->data_ptr[uim_rsp_data_ptr->data_len-2];
  apdu_cnf_ptr->implicit_get_rsp_sw2 =
     uim_rsp_data_ptr->data_ptr[uim_rsp_data_ptr->data_len-1];
  apdu_cnf_ptr->channel_id =
     extra_param_ptr->send_apdu_data.channel_id;

  apdu_cnf_ptr->apdu_data.data_len = uim_rsp_data_ptr->data_len - status_bytes;

  return MMGSDI_SUCCESS;
} /* mmgsdi_sap_rsp_copy_apdu_data */
