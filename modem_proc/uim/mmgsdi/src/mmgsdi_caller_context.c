/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            M M G S D I  P R O C E S S  I N  C L I E N T  C O N T E X T

                 D E F I N I T I O N S  A N D   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains processing of command request in caller's context.
  It optimizes MMGSDI APIs to build confirmation if required info is available
  in MMGSDI cache first in the caller's context instead of queuing it to MMGSDI
  task context and then checking for it in the cache. The reason in that, it is
  possible that MMGSDI is stuck is a state where card is busy for a long time.
  Any new request that is queued to MMGSDI task context, will get onto the MMGSDI
  queue that is blocked. So, any command request will also have to wait in this
  queue until the card is functional again. All this can be avoided to a certain
  extent by doing some minimal processing in the caller's context. If the API
  is called from MMGSDI context (e.g., from QMI), we skip the optimizations
  because of complications related to critical sections.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_caller_context.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/24/19   sg      Fill complete data if requested size is more than file size 
05/07/18   rps     Retrieving attribute cache from caller context
05/09/17   me      Added a check for file type in read and write operation  
04/19/17   dt      adding a check if length of atr > 0
03/14/17   nc      Better checks for return values  
12/29/16   dt      Support for QXDM sim data collection
10/05/16   me      Calling mmgsdi_present_in_preinit_file_access_list when needed 
09/06/16   bcho    Read OPL PNN cache in client context
08/30/16   bcho    Optimization of MMGSDI attribute cache access functions
08/08/16   me      Added check for service table in caller context for access APIs. 
06/14/16   sp      Remove command type parameter from validate_client_id function 
05/03/16   tkl     F3 log prints cleanup
04/26/16   ar      Removing the mmgsdi_cnf_map_req_to_cnf_enum_type()
04/01/16   vdc     Send act resp and session changed evt at same time
02/10/16   na      Optimize read record req by reading from cache directly
01/07/16   sp      Handle prl read in caller context
08/31/15   yt      Remove length information from requests and confirmations
08/31/15   ar      Don't allow read in client_context if PIN not verified
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
08/10/15   tl      Fix cache read for card sessions in caller's context
05/05/15   kv      Populate rec num back into the cnf data after read handling
04/29/15   bcho    Support for App specific cache
02/23/15   yt      Allow reading of record based EFs from cache in caller context
01/08/15   ar      Cache file if req_len is more than file size
11/05/14   ar      Remove mmgsdi_get_card_reader_status () API
10/20/14   stv     Populate status word in cnf data even if read is in caller context
10/14/14   av      When processing in client context,set status in cnf header
10/14/14   am      Compile warnings elimination
08/16/14   lj      Optimize app capabilities with cached data in client context
08/11/14   bcho    Initial version

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "comdef.h"
#include "intconv.h"
#include "uim_msg.h"
#include "mmgsdi_caller_context.h"
#include "mmgsdiutil.h"
#include "mmgsdi_uicc.h"
#include "mmgsdi_icc.h"
#include "mmgsdicache.h"
#include "mmgsdilib_p.h"
#include "mmgsdi_eons.h"
#include "mmgsdi_file.h"

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_BUILD_AND_PROCESS_SRV_AVAILABLE_CNF

   DESCRIPTION:
     This function populates the confirmation information and processes
     the response/confirmation directly without queuing it to mmgsdi task

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

       MMGSDI_SUCCESS:          The response and cnf are built successfully
                                and client has been notified with the cnf
       MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_caller_context_build_and_process_srv_available_cnf(
  mmgsdi_client_id_type               client_id,
  mmgsdi_session_id_type              session_id,
  mmgsdi_slot_id_enum_type            slot_id,
  mmgsdi_return_enum_type             cmd_status,
  boolean                             srv_available,
  mmgsdi_callback_type                response_cb,
  mmgsdi_client_data_type             client_data
)
{
  mmgsdi_cnf_type                    *cnf_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(response_cb);

  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr,
                                     sizeof(mmgsdi_cnf_type));
  if(cnf_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate the mmgsdi srv available cnf data */
  cnf_ptr->srv_available_cnf.srv_available = srv_available;

  /* Construct the mmgsdi cnf response header in order to notify the client */
  cnf_ptr->response_header.response_type       = MMGSDI_SRV_AVAILABLE_REQ;
  cnf_ptr->response_header.slot_id             = slot_id;
  cnf_ptr->response_header.client_data         = client_data;
  cnf_ptr->response_header.session_id          = session_id;
  cnf_ptr->response_header.client_id           = client_id;
  cnf_ptr->response_header.status_word.present = FALSE;
  cnf_ptr->response_header.mmgsdi_status       = cmd_status;

  /* Lets call the client callback with cnf information */
  (response_cb)(cmd_status,
                MMGSDI_SRV_AVAILABLE_REQ,
                cnf_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  return MMGSDI_SUCCESS;
}/* mmgsdi_caller_context_build_and_process_srv_available_cnf */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_CHECK_SERVICE_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to retrieve service status from cached service table.
     If the function is invoked from MMGSDI tasks context we simply return
     without any processing.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The retrieval of service status from cached
                               service table succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_check_service_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
)
{
  mmgsdi_client_id_type              client_id        = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_int_app_info_type         * app_info_ptr     = NULL;
  mmgsdi_slot_id_enum_type           slot_id          = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_service_enum_type           service          = MMGSDI_NONE;
  mmgsdi_session_id_type             session_id       = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_client_data_type            client_data      = 0;
  mmgsdi_callback_type               response_cb      = NULL;
  mmgsdi_protocol_enum_type          protocol         = MMGSDI_NO_PROTOCOL;
  boolean                            srv_available    = FALSE;
  mmgsdi_task_enum_type              mmgsdi_task      = MMGSDI_TASK_MAX;

  if((task_cmd_ptr == NULL) ||
     (task_cmd_ptr->cmd.cmd_enum != MMGSDI_SRV_AVAILABLE_REQ))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* In order to achieve srv available requests optimization,
     try getting the service status from cached service table first
     in caller's context. If that fails, continue with queuing
     service available request to MMGSDI. */
  mmgsdi_task = mmgsdi_util_get_current_task();
  if(MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task))
  {
    return MMGSDI_ERROR;
  }

  session_id  = task_cmd_ptr->cmd.cmd.srv_available_req.request_header.session_id;
  response_cb = task_cmd_ptr->cmd.cmd.srv_available_req.request_header.response_cb;
  client_data = task_cmd_ptr->cmd.cmd.srv_available_req.request_header.client_data;
  service     = task_cmd_ptr->cmd.cmd.srv_available_req.service;

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      session_id,
                      &slot_id,
                      NULL,
                      &app_info_ptr,
                      NULL,
                      NULL,
                      NULL);

    if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* Get the client id */
    mmgsdi_status = mmgsdi_util_get_client_id(session_id, &client_id);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    protocol = mmgsdi_util_determine_protocol(slot_id);
    if(protocol == MMGSDI_NO_PROTOCOL)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* Check service status */
    if(protocol == MMGSDI_UICC)
    {
      mmgsdi_status = mmgsdi_uicc_chk_srv_available(session_id,
                                                    MMGSDI_CHK_SRV_FROM_CACHE_ONLY,
                                                    service,
                                                    &srv_available);
    }
    else
    {
      mmgsdi_status = mmgsdi_icc_chk_srv_available(session_id,
                                                   MMGSDI_CHK_SRV_FROM_CACHE_ONLY,
                                                   service,
                                                   &srv_available);
    }
  }while (0);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    /* service status is successfully retrieved from service table. */
    mmgsdi_status = mmgsdi_caller_context_build_and_process_srv_available_cnf(
                      client_id,
                      session_id,
                      slot_id,
                      mmgsdi_status,
                      srv_available,
                      response_cb,
                      client_data);
  }

  return mmgsdi_status;
}/* mmgsdi_caller_context_check_service_and_notify_client */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_BUILD_AND_PROCESS_READ_CNF

   DESCRIPTION:
     This function populates the confirmation information and processes
     the response/confirmation directly without queuing it to mmgsdi task

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

       MMGSDI_SUCCESS:          The response and cnf are built successfully
                                and client has been notified with the cnf
       MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_caller_context_build_and_process_read_cnf(
  mmgsdi_client_id_type               client_id,
  mmgsdi_session_id_type              session_id,
  mmgsdi_slot_id_enum_type            slot_id,
  mmgsdi_return_enum_type             cmd_status,
  mmgsdi_offset_type                  offset,
  mmgsdi_rec_num_type                 rec_num,
  mmgsdi_access_type                  file_access,
  mmgsdi_data_type                    cnf_data,
  mmgsdi_callback_type                response_cb,
  mmgsdi_client_data_type             client_data
)
{
  mmgsdi_cnf_type                    *cnf_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(response_cb);

  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr,
                                     sizeof(mmgsdi_cnf_type));
  if(cnf_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate the mmgsdi read cnf data */
  cnf_ptr->read_cnf.accessed_rec_num   = rec_num;
  cnf_ptr->read_cnf.accessed_offset    = offset;
  cnf_ptr->read_cnf.read_data          = cnf_data;

  (void) mmgsdi_util_copy_access_type(&cnf_ptr->read_cnf.access,
                                      &file_access);

  /* Construct the mmgsdi cnf response header in order to notify the client */
  cnf_ptr->response_header.response_type       = MMGSDI_READ_REQ;
  cnf_ptr->response_header.slot_id             = slot_id;
  cnf_ptr->response_header.client_data         = client_data;
  cnf_ptr->response_header.session_id          = session_id;
  cnf_ptr->response_header.client_id           = client_id;
  cnf_ptr->response_header.status_word.present = FALSE;
  cnf_ptr->response_header.mmgsdi_status       = cmd_status;

  if( MMGSDI_SUCCESS == cmd_status )
  {
    cnf_ptr->response_header.status_word.present = TRUE;
    cnf_ptr->response_header.status_word.sw1     = GSDI_SIM_SW1_NORM_END;
    cnf_ptr->response_header.status_word.sw2     = GSDI_SIM_SW2_NORM_END;
  }

  /* Lets call the client callback with cnf information */
  (response_cb)(cmd_status,
                MMGSDI_READ_REQ,
                cnf_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  return MMGSDI_SUCCESS;
} /* mmgsdi_caller_context_build_and_process_read_cnf */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_READ_CACHE_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to read the item from MMGSDI cache. If the read
     succeeds or the item is not found on the card, a response is built
     immediately and cnf is sent to the client. App state must be ready;
     otherwise we simply return without doing anything.
     If the function is invoked from MMGSDI tasks context or if the access
     method is not BY_ENUM, we simply return without any processing.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The read from cache succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_read_cache_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
)
{
  mmgsdi_client_id_type              client_id        = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_session_info_type         * session_info_ptr = NULL;
  mmgsdi_slot_id_enum_type           slot_id          = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_len_type                    cache_item_len   = 0;
  mmgsdi_cache_init_enum_type        cache_item_state = MMGSDI_CACHE_MAX_ENUM;
  mmgsdi_session_id_type             session_id       = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_offset_type                 offset           = 0;
  mmgsdi_len_type                    req_len          = 0;
  mmgsdi_client_data_type            client_data      = 0;
  mmgsdi_callback_type               response_cb      = NULL;
  mmgsdi_access_type                 file_access;
  mmgsdi_data_type                   read_data;
  mmgsdi_int_app_info_type         * app_info_ptr     = NULL;
  mmgsdi_task_enum_type              mmgsdi_task      = MMGSDI_TASK_MAX;
  mmgsdi_decoded_attr_cache_type   * attr_cache_ptr   = NULL;

  memset(&file_access, 0x00, sizeof(mmgsdi_access_type));
  memset(&read_data, 0x00, sizeof(mmgsdi_data_type));

  /* We only support reading from cache (if avaialble) for read requests.
     For other requests, we return error.*/
  if((task_cmd_ptr == NULL) ||
     (task_cmd_ptr->cmd.cmd_enum               != MMGSDI_READ_REQ) ||
     (task_cmd_ptr->cmd.cmd.read_req.file_type != MMGSDI_TRANSPARENT_FILE &&
      task_cmd_ptr->cmd.cmd.read_req.file_type != MMGSDI_LINEAR_FIXED_FILE))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  session_id  = task_cmd_ptr->cmd.cmd.read_req.request_header.session_id;
  offset      = task_cmd_ptr->cmd.cmd.read_req.offset;
  file_access = task_cmd_ptr->cmd.cmd.read_req.access;
  req_len     = task_cmd_ptr->cmd.cmd.read_req.data_len;
  response_cb = task_cmd_ptr->cmd.cmd.read_req.request_header.response_cb;
  client_data = task_cmd_ptr->cmd.cmd.read_req.request_header.client_data;

  /* In order to achieve read requests optimization,
     only when the state of the app corresponding to client's session id
     is READY, try getting the item from cache first (in caller's context).
     If that fails and for all other app states, continue with queuing
     read request to MMGSDI. As part of accessing the cache here, do not
     worry about checking for session type as that is checked anyways
     inside the cache function below.
     The reason why we bring the optimization into picture only for app
     state READY is that we want to keep the checks as light weight as
     possible in caller's context. For all other app states, we might
     need to do extra processing to figure out if the read should be
     allowed and we should avoid all that in caller's context.
     The reason for not continuing when access menthod is NOT BY_ENUM
     is that cache access is anyways going to be rejected if the access
     method is BY_PATH. So, we do not even attempt calling tha cache READ
     API and simple continue with queuing the READ request to MMGSDI tasks
     so that the request can be forwarded to the card */

  mmgsdi_task = mmgsdi_util_get_current_task();
  if(MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task) ||
     file_access.access_method != MMGSDI_EF_ENUM_ACCESS)
  {
    return MMGSDI_ERROR;
  }

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  do
  {
    /* Get the client id */
    mmgsdi_status = mmgsdi_util_get_client_id(session_id, &client_id);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      session_id,
                      &slot_id,
                      NULL,
                      NULL,
                      NULL,
                      &session_info_ptr,
                      NULL);

    if(mmgsdi_status    != MMGSDI_SUCCESS ||
       session_info_ptr == NULL)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* The read request are allowed to read from the cache only for prov & card sessions.
       We return error for non-prov sessions.*/
    if(mmgsdi_util_is_non_prov_session(session_info_ptr->session_type))
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* session type is card session */
    if(mmgsdi_util_is_card_session(session_info_ptr->session_type))
    {
      if(mmgsdi_present_in_preinit_file_access_list(&task_cmd_ptr->cmd) == FALSE)
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }
    }
    else
    {
      mmgsdi_status = mmgsdi_util_get_prov_session_info(session_id,
                                                        NULL,
                                                        NULL,
                                                        &app_info_ptr);
      if((mmgsdi_status != MMGSDI_SUCCESS) ||
         (app_info_ptr == NULL) ||
         ((app_info_ptr->app_state != MMGSDI_APP_STATE_READY) &&
          (mmgsdi_present_in_preinit_file_access_list(&task_cmd_ptr->cmd) == FALSE)))
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }

      /* If file is present in preinit_access list and app_state is not READY
         then check for security permissions whether file is allowed to read
         at this stage */
      if (app_info_ptr->app_state != MMGSDI_APP_STATE_READY)
      {
        mmgsdi_status = mmgsdi_util_check_file_attr(session_id,
                                                    app_info_ptr,
                                                    &file_access,
                                                    MMGSDI_READ_REQ,
                                                    MMGSDI_DECODE_ACTUAL_ATTRIBUTES);

        if(mmgsdi_status != MMGSDI_SUCCESS)
        {
          UIM_MSG_MED_1("Read not allowed at this stage, status = 0x%x",
                        mmgsdi_status);
          mmgsdi_status = MMGSDI_ERROR;
          break;
        }
      }
     
      /* If file type in the request does not match the file type in the card */
      mmgsdi_status = mmgsdi_cache_attr_read(session_id,
                                             &file_access,
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
        if (attr_cache_ptr->file_type != task_cmd_ptr->cmd.cmd.read_req.file_type &&
          !(task_cmd_ptr->cmd.cmd.read_req.file_type == MMGSDI_LINEAR_FIXED_FILE && 
            attr_cache_ptr->file_type == MMGSDI_CYCLIC_FILE))
        {
          mmgsdi_status = MMGSDI_EF_INCONSISTENT;
        }
      }

      MMGSDIUTIL_TMC_MEM_FREE(attr_cache_ptr);

      if (mmgsdi_status == MMGSDI_EF_INCONSISTENT)
      {
        break;
      }
    }

    /* Files related to EONS functionality are stored in a special cache, so it
       requires a different mechanism to read cache */
    if(file_access.file.file_enum == MMGSDI_USIM_OPL ||
       file_access.file.file_enum == MMGSDI_USIM_PNN ||
       file_access.file.file_enum == MMGSDI_GSM_OPL ||
       file_access.file.file_enum == MMGSDI_GSM_PNN)
    {
      mmgsdi_status = mmgsdi_eons_opl_pnn_read_record(&task_cmd_ptr->cmd.cmd.read_req,
                                                      &read_data,
                                                      &cache_item_state);
      break;
    }

    /* Read item cache length */
    mmgsdi_status = mmgsdi_cache_read_len(session_id,
                                          &file_access,
                                          task_cmd_ptr->cmd.cmd.read_req.rec_num,
                                          &cache_item_len,
                                          &cache_item_state);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }
     /* If req data_len is 0 or more than the file size, it means the client is
        requesting for the full contents of the file beginning from the offset.
        If req data_len is more than file_size then from MMGSDI task also we
        will return the entire file size */
    if(req_len > 0 && req_len + offset <= cache_item_len)
    {
      read_data.data_len = req_len;
    }
    else if((req_len == 0 || 
            req_len + offset > cache_item_len) &&
            cache_item_len > offset)
    {
      read_data.data_len = cache_item_len - offset;
    }
    else
    {
      mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      break;
    }

    mmgsdi_status = MMGSDI_ERROR;

    if(read_data.data_len > 0)
    {
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(read_data.data_ptr,
                                         read_data.data_len);
      if(read_data.data_ptr == NULL)
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }

      mmgsdi_status = mmgsdi_session_read_cache_ext(session_id,
                                                    file_access.file.file_enum,
                                                    &read_data,
                                                    offset,
                                                    task_cmd_ptr->cmd.cmd.read_req.rec_num);
    }
  }while (0);

  /* Item is either not cached and service is not set or item is not chached
     and service is set. */
  if (mmgsdi_status == MMGSDI_NOT_INIT && app_info_ptr != NULL)
  {
    mmgsdi_status = mmgsdi_util_is_file_ok_in_svc_table(session_id,
                                                        app_info_ptr->app_data.app_type,
                                                        &file_access);
    if (mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* service is set and cache not available we have to queue the
         command to mmgsdi. */
      mmgsdi_status = MMGSDI_NOT_INIT;
    }
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  if((mmgsdi_status == MMGSDI_SUCCESS) ||
     (mmgsdi_status == MMGSDI_NOT_FOUND && cache_item_state == MMGSDI_CACHE_NOT_FOUND) ||
     (mmgsdi_status == MMGSDI_NOT_SUPPORTED) ||
     (mmgsdi_status == MMGSDI_ACCESS_DENIED) || 
     (mmgsdi_status == MMGSDI_EF_INCONSISTENT) ||
     (mmgsdi_status == MMGSDI_INCORRECT_PARAMS))
  {
    /* Item is either found in the cache or we figure that the item
       was not cached because it was not found on the card, or the service is
       not set or read of service table EF fails in service table when
       cache is not init, In all cases, we should return to the caller. */
    mmgsdi_status = mmgsdi_caller_context_build_and_process_read_cnf(client_id,
                                                                     session_id,
                                                                     slot_id,
                                                                     mmgsdi_status,
                                                                     offset,
                                                                     task_cmd_ptr->cmd.cmd.read_req.rec_num,
                                                                     file_access,
                                                                     read_data,
                                                                     response_cb,
                                                                     client_data);
  }
  MMGSDIUTIL_TMC_MEM_FREE(read_data.data_ptr);

  return mmgsdi_status;
} /* mmgsdi_caller_context_read_cache_and_notify_client */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_BUILD_AND_PROCESS_APP_CAPABILITIES_CNF

   DESCRIPTION:
     This function populates the confirmation information and processes
     the response/confirmation directly without queuing it to mmgsdi task

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

       MMGSDI_SUCCESS:          The response and cnf are built successfully
                                and client has been notified with the cnf
       MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_caller_context_build_and_process_app_capabilities_cnf(
  mmgsdi_client_id_type               client_id,
  mmgsdi_session_id_type              session_id,
  mmgsdi_slot_id_enum_type            slot_id,
  boolean                             fdn_enabled,
  boolean                             bdn_enabled,
  boolean                             acl_enabled,
  boolean                             imsi_invalidated,
  mmgsdi_callback_type                response_cb,
  mmgsdi_client_data_type             client_data
)
{
  mmgsdi_cnf_type                    *cnf_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(response_cb);

  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr,
                                     sizeof(mmgsdi_cnf_type));
  if(cnf_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate the mmgsdi app capabilities cnf data */
  cnf_ptr->session_app_capabilities_cnf.fdn_enabled = fdn_enabled;
  cnf_ptr->session_app_capabilities_cnf.bdn_enabled = bdn_enabled;
  cnf_ptr->session_app_capabilities_cnf.acl_enabled = acl_enabled;
  cnf_ptr->session_app_capabilities_cnf.imsi_invalidated = imsi_invalidated;

  /* Construct the mmgsdi cnf response header in order to notify the client */
  cnf_ptr->response_header.response_type       = MMGSDI_SESSION_GET_APP_CAPABILITIES_REQ;
  cnf_ptr->response_header.slot_id             = slot_id;
  cnf_ptr->response_header.client_data         = client_data;
  cnf_ptr->response_header.session_id          = session_id;
  cnf_ptr->response_header.client_id           = client_id;
  cnf_ptr->response_header.status_word.present = FALSE;
  cnf_ptr->response_header.mmgsdi_status       = MMGSDI_SUCCESS;

  /* Lets call the client callback with cnf information */
  (response_cb)(MMGSDI_SUCCESS,
                MMGSDI_SESSION_GET_APP_CAPABILITIES_REQ,
                cnf_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  return MMGSDI_SUCCESS;
}/* mmgsdi_caller_context_build_and_process_app_capabilities_cnf */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_CHECK_APP_CAPABILITIES_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to retrieve app capabilites from cached data.
     If the function is invoked from MMGSDI tasks context we simply return
     without any processing.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The retrieval of service status from cached
                               service table succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_check_app_capabilities_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
)
{
  mmgsdi_client_id_type              client_id        = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_return_enum_type            mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_int_app_info_type         * app_info_ptr     = NULL;
  mmgsdi_slot_id_enum_type           slot_id          = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_session_id_type             session_id       = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_client_data_type            client_data      = 0;
  mmgsdi_callback_type               response_cb      = NULL;
  boolean                            fdn_enabled      = FALSE;
  boolean                            bdn_enabled      = FALSE;
  boolean                            acl_enabled      = FALSE;
  boolean                            imsi_invalidated = FALSE;
  mmgsdi_task_enum_type              mmgsdi_task      = MMGSDI_TASK_MAX;

  if((task_cmd_ptr == NULL) ||
     (task_cmd_ptr->cmd.cmd_enum != MMGSDI_SESSION_GET_APP_CAPABILITIES_REQ))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  mmgsdi_task = mmgsdi_util_get_current_task();
  if(MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task))
  {
    return MMGSDI_ERROR;
  }

  session_id  = task_cmd_ptr->cmd.cmd.session_get_app_cap_req.request_header.session_id;
  response_cb = task_cmd_ptr->cmd.cmd.session_get_app_cap_req.request_header.response_cb;
  client_data = task_cmd_ptr->cmd.cmd.session_get_app_cap_req.request_header.client_data;

  /* In order to achieve app capabilities requests optimization,
     try getting the app capabilities from cached data first
     in caller's context. If that fails, continue with queuing
     app capabilities request to MMGSDI. */
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      session_id,
                      &slot_id,
                      NULL,
                      &app_info_ptr,
                      NULL,
                      NULL,
                      NULL);

    if((mmgsdi_status != MMGSDI_SUCCESS) || (app_info_ptr == NULL))
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* Get the client id */
    mmgsdi_status = mmgsdi_util_get_client_id(session_id, &client_id);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Check app capabilities */
    if(!app_info_ptr->app_capabilities.all_srv_init)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    if(app_info_ptr->app_capabilities.fdn_state == MMGSDI_SRV_ENABLE)
    {
      fdn_enabled = TRUE;
    }

    if(app_info_ptr->app_capabilities.bdn_state == MMGSDI_SRV_ENABLE)
    {
      bdn_enabled = TRUE;
    }

    if(app_info_ptr->app_capabilities.acl_state == MMGSDI_SRV_ENABLE)
    {
      acl_enabled = TRUE;
    }

    imsi_invalidated = app_info_ptr->app_capabilities.imsi_invalidated;

  }while (0);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    /* app capabilities is successfully retrieved from app info. */
    mmgsdi_status = mmgsdi_caller_context_build_and_process_app_capabilities_cnf(
                      client_id,
                      session_id,
                      slot_id,
                      fdn_enabled,
                      bdn_enabled,
                      acl_enabled,
                      imsi_invalidated,
                      response_cb,
                      client_data);
  }

  return mmgsdi_status;
}/* mmgsdi_caller_context_check_app_capabilities_and_notify_client */

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_BUILD_AND_PROCESS_SAP_CARD_READER_STATUS_CNF

   DESCRIPTION:
     This function populates the confirmation information and processes
     the response/confirmation directly without queuing it to mmgsdi task for
     SAP_CARD_READER_STATUS

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

       MMGSDI_SUCCESS:          The response and cnf are built successfully
                                and client has been notified with the cnf
       MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_caller_context_build_and_process_sap_card_reader_status_cnf(
  mmgsdi_client_id_type          client_id,
  mmgsdi_slot_id_enum_type       slot_id,
  mmgsdi_callback_type           response_cb,
  mmgsdi_client_data_type        client_data,
  uint8                          card_reader_status_data
)
{
  mmgsdi_cnf_type               *cnf_ptr      = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(response_cb);

  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr,
                                     sizeof(mmgsdi_cnf_type));
  if(cnf_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  cnf_ptr->response_header.response_type       = MMGSDI_SAP_CARD_READER_STATUS_REQ;
  cnf_ptr->response_header.slot_id             = slot_id;
  cnf_ptr->response_header.client_data         = client_data;
  cnf_ptr->response_header.client_id           = client_id;
  cnf_ptr->response_header.status_word.present = FALSE;
  cnf_ptr->response_header.mmgsdi_status       = MMGSDI_SUCCESS;
  cnf_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_ptr = &card_reader_status_data;
  cnf_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_len = sizeof(uint8);

  /* Lets call the client callback with cnf information */
  (response_cb)(MMGSDI_SUCCESS,
                MMGSDI_SAP_CARD_READER_STATUS_REQ,
                cnf_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  return MMGSDI_SUCCESS;
}/* mmgsdi_caller_context_build_and_process_sap_card_reader_status_cnf */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_SAP_CARD_READER_STATUS_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to retrieve card reader status if the slot_data_ptr is available for
     that particular Slot. This function is only used for SAP_CARD_READER_STATUS.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The retrieval of service status from cached
                               service table succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/

mmgsdi_return_enum_type mmgsdi_caller_context_sap_card_reader_status_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
)
{
  mmgsdi_client_id_type      client_id               = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_client_data_type    client_data             = 0;
  mmgsdi_callback_type       response_cb             = NULL;
  mmgsdi_return_enum_type    mmgsdi_status           = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type   slot_id                 = MMGSDI_MAX_SLOT_ID_ENUM;
  uint8                      slot_index              = 0;
  uint8                      card_reader_status_data = 0x00;
  mmgsdi_slot_data_type     *slot_data_ptr           = NULL;
  mmgsdi_task_enum_type      mmgsdi_task             = MMGSDI_TASK_MAX;

  if((task_cmd_ptr == NULL) ||
     (task_cmd_ptr->cmd.cmd_enum != MMGSDI_SAP_CARD_READER_STATUS_REQ))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  response_cb = task_cmd_ptr->cmd.cmd.sap_card_reader_status_req.request_header.response_cb;
  client_data = task_cmd_ptr->cmd.cmd.sap_card_reader_status_req.request_header.client_data;
  client_id   = task_cmd_ptr->cmd.cmd.sap_card_reader_status_req.request_header.client_id;
  slot_id     = task_cmd_ptr->cmd.cmd.sap_card_reader_status_req.request_header.slot_id;

  mmgsdi_status = mmgsdi_util_is_client_id_valid_in_cmd(client_id);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  mmgsdi_task = mmgsdi_util_get_current_task();
  if(MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task))
  {
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_util_get_slot_index(slot_id, &slot_index);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  if (mmgsdi_sap_connected[slot_index])
  {
    /*bits 1-3              = identity of card reader x.
      bit 4               0 = Card reader is not removable
                          1 = Card reader is removable
      bit 5               0 = Card reader is not present
                          1 = Card reader is present
      bit 6               0 = Card reader present is not ID-1 size
                          1 = Card reader present is ID-1 size
      bit 7               0 = No card present
                          1 = Card is present in reader
      bit 8               0 = No card powered
                          1 = Card in reader is powered
    */

    /* Card reader is present      = 1 <b5> */
    card_reader_status_data |= 0x10;

    /*Card Reader is ID-1 size     = 1 <b6>*/
    card_reader_status_data |= 0x20;

    /*Card is Present in the reader - Check gsdi_data*/
    /*Card in Reader is powered if it is present*/
    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);

    if(slot_data_ptr == NULL)
    {
      mmgsdi_status = MMGSDI_ERROR;
    }
    else
    {
      if(slot_data_ptr->mmgsdi_sap_state != MMGSDI_STATE_NO_CARD)
      {
        card_reader_status_data |= 0xC0;
      }
    }
  }
  else
  {
    UIM_MSG_ERR_0("SAP card reader status Req error: BT is not connected");
    mmgsdi_status = MMGSDI_ERROR;
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_1("Card Reader Status 0x%x", card_reader_status_data);

    mmgsdi_status = mmgsdi_caller_context_build_and_process_sap_card_reader_status_cnf(
                      client_id,
                      slot_id,
                      response_cb,
                      client_data,
                      card_reader_status_data);
  }

  return mmgsdi_status;
}/* mmgsdi_caller_context_sap_card_reader_status_and_notify_client */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_BUILD_AND_PROCESS_READ_PRL_CNF

   DESCRIPTION:
     This function populates the confirmation information and processes
     the response/confirmation directly for read prl request 
     without queuing it to mmgsdi task

   DEPENDENCIES:
     None

   LIMITATIONS:
     Have to protected by critical section while using this

   RETURN VALUE:
     mmgsdi_return_enum_type:

       MMGSDI_SUCCESS:          The response and cnf are built successfully
                                and client has been notified with the cnf
       MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_caller_context_build_and_process_read_prl_cnf(
  mmgsdi_client_id_type               client_id,
  mmgsdi_session_id_type              session_id,
  mmgsdi_slot_id_enum_type            slot_id,
  mmgsdi_callback_type                response_cb,
  mmgsdi_client_data_type             client_data,
  mmgsdi_read_prl_data_type          *prl_data_ptr
)
{
  mmgsdi_cnf_type                    *cnf_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL_2(response_cb, prl_data_ptr);

  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr,
                                     sizeof(mmgsdi_cnf_type));
  if(cnf_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate the mmgsdi read cnf data */
  cnf_ptr->session_read_prl_cnf.prl_version      = prl_data_ptr->prl_version;
  cnf_ptr->session_read_prl_cnf.size             = prl_data_ptr->size;
  cnf_ptr->session_read_prl_cnf.valid            = prl_data_ptr->valid;
  cnf_ptr->session_read_prl_cnf.roaming_list_ptr = prl_data_ptr->roaming_list_ptr;
  cnf_ptr->session_read_prl_cnf.sspr_p_rev       = prl_data_ptr->sspr_p_rev;

  /* Construct the mmgsdi cnf response header in order to notify the client */
  cnf_ptr->response_header.response_type       = MMGSDI_SESSION_READ_PRL_REQ;
  cnf_ptr->response_header.slot_id             = slot_id;
  cnf_ptr->response_header.client_data         = client_data;
  cnf_ptr->response_header.session_id          = session_id;
  cnf_ptr->response_header.client_id           = client_id;
  cnf_ptr->response_header.status_word.present = FALSE;
  cnf_ptr->response_header.mmgsdi_status       = MMGSDI_SUCCESS;

  /* Lets call the client callback with cnf information */
  (response_cb)(MMGSDI_SUCCESS,
                MMGSDI_SESSION_READ_PRL_REQ,
                cnf_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  return MMGSDI_SUCCESS;
}/* mmgsdi_caller_context_build_and_process_read_prl_cnf */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_READ_PRL_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to read PRL data from MMGSDI cache.for 1x prov sessions
     If the PRL data is not validated then we simply return from the caller this function
     and queue the command to MMGSDI.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The read from cache succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:
     None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_read_prl_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
)
{
  uint8                         prl_index                 = 0;
  mmgsdi_technology_enum_type   dummy_tech                = MMGSDI_TECH_UNKNOWN;
  mmgsdi_session_info_type     *session_info_ptr          = NULL;
  mmgsdi_int_app_info_type     *int_app_info_ptr          = NULL;
  mmgsdi_read_prl_data_type    *prl_data_ptr              = NULL;
  mmgsdi_client_id_type         client_id                 = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_client_data_type       client_data               = 0;
  mmgsdi_slot_id_enum_type      slot_id                   = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_callback_type          response_cb               = NULL;
  mmgsdi_return_enum_type       mmgsdi_status             = MMGSDI_SUCCESS;
  mmgsdi_attribute_enum_type    attribute                 = MMGSDI_ATTR_UNKNOWN;
  mmgsdi_task_enum_type         mmgsdi_task               = MMGSDI_TASK_MAX;

  if((task_cmd_ptr == NULL) ||
     (task_cmd_ptr->cmd.cmd_enum != MMGSDI_SESSION_READ_PRL_REQ))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  response_cb = task_cmd_ptr->cmd.cmd.session_read_prl_req.request_header.response_cb;
  client_data = task_cmd_ptr->cmd.cmd.session_read_prl_req.request_header.client_data;
  client_id   = task_cmd_ptr->cmd.cmd.session_read_prl_req.request_header.client_id;

  mmgsdi_task = mmgsdi_util_get_current_task();
  if(MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task))
  {
    return MMGSDI_ERROR;
  }

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                          task_cmd_ptr->cmd.cmd.request_header.session_id,
                          &slot_id,
                          NULL,
                          &int_app_info_ptr,
                          NULL,
                          &session_info_ptr,
                          NULL);

    if(mmgsdi_status != MMGSDI_SUCCESS ||
       session_info_ptr == NULL ||
       int_app_info_ptr == NULL)
    {
      mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      break;
    }

    if(mmgsdi_util_is_1x_prov_session(session_info_ptr->session_type) == FALSE ||
       (int_app_info_ptr->app_state != MMGSDI_APP_STATE_READY &&
        int_app_info_ptr->app_state != MMGSDI_APP_STATE_ILLEGAL))
    {
      mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      break;
    }

    (void)mmgsdi_util_get_tech_and_attr_from_session_type(
                                           session_info_ptr->session_type,
                                           &dummy_tech,
                                           &attribute);
    prl_index = (uint8)attribute;
    if (prl_index > MMGSDI_TER_PRL_INDEX)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    if(mmgsdi_1x_prl[prl_index].prl_validated == FALSE)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    prl_data_ptr = mmgsdi_1x_prl[prl_index].prl_data_ptr;

    mmgsdi_status = mmgsdi_caller_context_build_and_process_read_prl_cnf(
                        client_id,
                        task_cmd_ptr->cmd.cmd.request_header.session_id,
                        slot_id,
                        response_cb,
                        client_data,
                        prl_data_ptr);
  }while(0);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;
  return mmgsdi_status;
}/*mmgsdi_caller_context_read_prl_and_notify_client*/

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_BUILD_AND_PROCESS_GET_ATR_STATUS_CNF

   DESCRIPTION:
     This function populates the confirmation information and processes
     the response/confirmation directly without queuing it to mmgsdi task 

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

       MMGSDI_SUCCESS:          The response and cnf are built successfully
                                and client has been notified with the cnf
       MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:
     None

==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_caller_context_build_and_process_get_atr_status_cnf(
  mmgsdi_client_id_type          client_id,
  mmgsdi_slot_id_enum_type       slot_id,
  mmgsdi_callback_type           response_cb,
  mmgsdi_client_data_type        client_data,
  const mmgsdi_atr_info_type     *atr_ptr
)
{
  mmgsdi_cnf_type               *cnf_ptr      = NULL;

  MMGSDIUTIL_RETURN_IF_NULL_2(response_cb, atr_ptr);

  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr,
                                     sizeof(mmgsdi_cnf_type));
  
  if(cnf_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr->get_atr_cnf.atr_data.data_ptr,
                                     atr_ptr->atr_length);

  if(cnf_ptr->get_atr_cnf.atr_data.data_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  
  /* Construct the mmgsdi cnf response header in order to notify the client */
  cnf_ptr->response_header.response_type       = MMGSDI_GET_ATR_REQ;
  cnf_ptr->response_header.slot_id             = slot_id;
  cnf_ptr->response_header.client_id           = client_id;
  cnf_ptr->response_header.mmgsdi_status       = MMGSDI_SUCCESS;
  cnf_ptr->response_header.client_data         = client_data;
  cnf_ptr->get_atr_cnf.atr_data.data_len       = atr_ptr->atr_length;

  mmgsdi_memscpy(cnf_ptr->get_atr_cnf.atr_data.data_ptr,
                 atr_ptr->atr_length,
                 atr_ptr->atr_data,
                 atr_ptr->atr_length);

  /* Lets call the client callback with cnf information */
  (response_cb)(MMGSDI_SUCCESS,
                MMGSDI_GET_ATR_REQ,
                cnf_ptr);
  
  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr->get_atr_cnf.atr_data.data_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  return MMGSDI_SUCCESS;
}/* mmgsdi_caller_context_build_and_process_get_atr_status_cnf */


/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_GET_ATR_STATUS_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to retrieve atr.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The retrieval of service status from cached
                               service table succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:
     None
==========================================================================*/

mmgsdi_return_enum_type mmgsdi_caller_context_get_atr_status_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
)
{
  mmgsdi_client_id_type      client_id               = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_client_data_type    client_data             = 0;
  mmgsdi_callback_type       response_cb             = NULL;
  mmgsdi_return_enum_type    mmgsdi_status           = MMGSDI_SUCCESS;
  mmgsdi_slot_id_enum_type   slot_id                 = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_slot_data_type     *slot_data_ptr           = NULL;
  mmgsdi_atr_info_type      *atr_ptr                 = NULL;
  mmgsdi_task_enum_type      mmgsdi_task             = MMGSDI_TASK_MAX;

  if((task_cmd_ptr == NULL) ||
     (task_cmd_ptr->cmd.cmd_enum != MMGSDI_GET_ATR_REQ))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  response_cb = task_cmd_ptr->cmd.cmd.get_atr_req.request_header.response_cb;
  client_data = task_cmd_ptr->cmd.cmd.get_atr_req.request_header.client_data;
  client_id   = task_cmd_ptr->cmd.cmd.get_atr_req.request_header.client_id;
  slot_id     = task_cmd_ptr->cmd.cmd.get_atr_req.request_header.slot_id;

  mmgsdi_task = mmgsdi_util_get_current_task();
  if(MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task))
  {
    return MMGSDI_ERROR;        
  }
  
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(atr_ptr, sizeof(mmgsdi_atr_info_type));

  if (atr_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
    
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  do
  {
    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);

    if (slot_data_ptr == NULL)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }
    if ((client_id != MMGSDI_CLIENT_ID_ZERO) && (mmgsdi_util_is_valid_client_id(client_id)!= MMGSDI_SUCCESS))
    {
      mmgsdi_status = MMGSDI_ERROR; 
      break;
    }
    if (slot_data_ptr->card_atr.atr_length == 0)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    atr_ptr->atr_length = slot_data_ptr->card_atr.atr_length;
    
    mmgsdi_memscpy(atr_ptr->atr_data,
                   sizeof(atr_ptr->atr_data),
                   slot_data_ptr->card_atr.atr_data,
                   slot_data_ptr->card_atr.atr_length);
 
  } while(0);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    mmgsdi_status = mmgsdi_caller_context_build_and_process_get_atr_status_cnf(
                      client_id,
                      slot_id,
                      response_cb,
                      client_data,
                      atr_ptr);
  }

  MMGSDIUTIL_TMC_MEM_FREE(atr_ptr)

  return mmgsdi_status;
}/* mmgsdi_caller_context_get_atr_status_and_notify_client */


/* ==========================================================================
   FUNCTION: MMGSDI_CALLER_CONTEXT_BUILD_AND_PROCESS_GET_FILE_ATTR_CACHE_CNF

   DESCRIPTION: This function populates the confirmation information and processes
                the response/confirmation directly without queuing it to mmgsdi task

   DEPENDENCIES:  None

   LIMITATIONS:   None

   RETURN VALUE:
     mmgsdi_return_enum_type:

     MMGSDI_SUCCESS:     The response and cnf are built successfully
                         and client has been notified with the cnf
     MMGSDI_ERROR:       Otherwise

   SIDE EFFECTS:
     None
==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_caller_context_build_and_process_get_file_attr_cache_cnf(
  mmgsdi_client_id_type                     client_id,
  mmgsdi_session_id_type                    session_id,
  mmgsdi_slot_id_enum_type                  slot_id,
  const mmgsdi_decoded_attr_cache_type     *cache_attr_ptr,
  mmgsdi_return_enum_type                   cmd_status,
  mmgsdi_callback_type                      response_cb,
  mmgsdi_client_data_type                   client_data,
  mmgsdi_file_security_access_type          security_access,
  mmgsdi_len_type                           raw_data_len,
  uint8                                    *raw_data_ptr
)
{
  mmgsdi_cnf_type          *cnf_ptr        = NULL;

  MMGSDIUTIL_RETURN_IF_NULL_3(response_cb, cache_attr_ptr, raw_data_ptr);

  /* Obtain memory for confirmation */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cnf_ptr, sizeof(mmgsdi_cnf_type));
  if(cnf_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Populate the mmgsdi get file attr cnf data */
  (void) mmgsdi_util_copy_access_type(&cnf_ptr->get_file_attr_cnf.access, &cache_attr_ptr->access);
  cnf_ptr->get_file_attr_cnf.file_attrib.file_size   = cache_attr_ptr->file_size;

  mmgsdi_memscpy(cnf_ptr->get_file_attr_cnf.file_attrib.file_id,
                 sizeof(cnf_ptr->get_file_attr_cnf.file_attrib.file_id),
                 cache_attr_ptr->file_id,
                 sizeof(cache_attr_ptr->file_id));

  cnf_ptr->get_file_attr_cnf.file_attrib.file_type   = cache_attr_ptr->file_type;
  cnf_ptr->get_file_attr_cnf.file_attrib.sfi         = cache_attr_ptr->sfi;

  if (cache_attr_ptr->file_type == MMGSDI_LINEAR_FIXED_FILE)
  {
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security = security_access;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.num_of_rec    = cache_attr_ptr->num_of_rec;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.rec_len       = cache_attr_ptr->rec_len;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_status   = cache_attr_ptr->file_status;
  }
  else if (cache_attr_ptr->file_type == MMGSDI_TRANSPARENT_FILE)
  {
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security = security_access;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_status   = cache_attr_ptr->file_status;
  }
  else if (cache_attr_ptr->file_type == MMGSDI_CYCLIC_FILE)
  {
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security    = security_access;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.increase_allowed = cache_attr_ptr->cyclic_increase_allowed;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.num_of_rec       = cache_attr_ptr->num_of_rec;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.rec_len          = cache_attr_ptr->rec_len;
    cnf_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_status      = cache_attr_ptr->file_status;
  }
  else
  {
    MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);
    return MMGSDI_ERROR;
  }

  cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len = raw_data_len;
  cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr = raw_data_ptr;

  cnf_ptr->response_header.response_type       = MMGSDI_GET_FILE_ATTR_CNF;
  cnf_ptr->response_header.slot_id             = slot_id;
  cnf_ptr->response_header.client_data         = client_data;
  cnf_ptr->response_header.session_id          = session_id;
  cnf_ptr->response_header.client_id           = client_id;
  cnf_ptr->response_header.status_word.present = FALSE;
  cnf_ptr->response_header.mmgsdi_status       = cmd_status;

  if(cmd_status == MMGSDI_SUCCESS)
  {
    cnf_ptr->response_header.status_word.present = TRUE;
    cnf_ptr->response_header.status_word.sw1     = GSDI_SIM_SW1_NORM_END; 
    cnf_ptr->response_header.status_word.sw2     = GSDI_SIM_SW2_NORM_END; 
  }

  /* Lets call the client callback with cnf information */
  (response_cb)(cmd_status, MMGSDI_GET_FILE_ATTR_CNF, cnf_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  return MMGSDI_SUCCESS;
} /* mmgsdi_caller_context_build_and_process_get_file_attr_cache_cnf */


/* ==========================================================================
  FUNCTION:    MMGSDI_CALLER_CONTEXT_GET_FILE_ATTR_CACHE_AND_NOTIFY_CLIENT

  DESCRIPTION:
    This function tries to get cached file attributes and a response is built
    immediately and cnf is sent to the client.This function uses critical sections.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type:

    MMGSDI_SUCCESS:      The get file attributes from cache succeeded
                         and client has notified with the cnf
    MMGSDI_ERROR:        Otherwise

  SIDE EFFECTS:
    None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_get_file_attr_cache_and_notify_client(
  const mmgsdi_task_cmd_type     *task_cmd_ptr
)
{
  mmgsdi_client_id_type              client_id           = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_slot_id_enum_type           slot_id             = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_session_id_type             session_id          = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_client_data_type            client_data         = 0;
  mmgsdi_callback_type               response_cb         = NULL;
  mmgsdi_decoded_attr_cache_type    *cache_attr_ptr      = NULL;
  mmgsdi_len_type                    raw_data_len        = 0;
  uint8                             *raw_data_ptr        = NULL;
  mmgsdi_file_security_access_type   security_access;
  mmgsdi_access_type                 file_access;

  memset(&file_access, 0x00, sizeof(mmgsdi_access_type));
  memset(&security_access, 0x00, sizeof(mmgsdi_file_security_access_type));

  /* We only support attribute reading from cache (if available) for get file attribute requests.
     For other requests, we return error.*/
  if((task_cmd_ptr               == NULL) ||
     (task_cmd_ptr->cmd.cmd_enum != MMGSDI_GET_FILE_ATTR_REQ))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  session_id  = task_cmd_ptr->cmd.cmd.get_file_attr_req.request_header.session_id;
  file_access = task_cmd_ptr->cmd.cmd.get_file_attr_req.access;
  response_cb = task_cmd_ptr->cmd.cmd.get_file_attr_req.request_header.response_cb;
  client_data = task_cmd_ptr->cmd.cmd.get_file_attr_req.request_header.client_data;

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  do
  {
    /* Get the client id */
    mmgsdi_status = mmgsdi_util_get_client_id(session_id, &client_id);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      session_id,
                      &slot_id,
                      NULL,
                      NULL,
                      NULL,
                      NULL,
                      NULL);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Check if the attributes are already cached */
    mmgsdi_status = mmgsdi_cache_attr_read(session_id,
                                           &file_access,
                                           &cache_attr_ptr,
                                           task_cmd_ptr->cmd.cmd.get_file_attr_req.skip_uicc_arr ?
                                               MMGSDI_DECODE_DEFAULT_ATTRIBUTES : MMGSDI_DECODE_ACTUAL_ATTRIBUTES,
                                           &security_access,
                                           &raw_data_len,
                                           &raw_data_ptr);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }
  }while (0);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    mmgsdi_status = mmgsdi_caller_context_build_and_process_get_file_attr_cache_cnf(client_id,
                                                                                    session_id,
                                                                                    slot_id,
                                                                                    cache_attr_ptr,
                                                                                    mmgsdi_status,
                                                                                    response_cb,
                                                                                    client_data,
                                                                                    security_access,
                                                                                    raw_data_len,
                                                                                    raw_data_ptr);
  }
  MMGSDIUTIL_TMC_MEM_FREE(cache_attr_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(raw_data_ptr);
  mmgsdi_file_free_security_info(&security_access);

  return mmgsdi_status;
} /* mmgsdi_caller_context_get_file_attr_cache_and_notify_client */

