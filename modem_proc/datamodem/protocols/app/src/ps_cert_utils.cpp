/*===========================================================================
                           PS_CERT_UTILS_CPP
===========================================================================*/

/*!
  @file
  ps_cert_utils.cpp

  @brief
  This module implements utility functions for certificates

  @detail  
*/

/*===========================================================================

  Copyright (c) 2015-2018 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/dev/data.mpss/3.3/fning.data.mpss.3.3.fning_1_12_check_in/

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/04/20    vls    Clients wil pass the profile, SUBS and iface type information
                   to send HTTP request
05/16/19    ak     Fix to set content-type in http post
06/29/18    fn     Fix session id for ocsp
02/06/18    fn     FR 29585 Added support for http post
04/10/15    fn     Created module
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rex.h"
#include "queue.h"
#include "ps_cert_utils.h"
#include "ps_cert_utilsi.h"
#include "ds_http_api.h"
#include "ps_app_logging_util.h"

#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"

#ifdef FEATURE_DATA_APP_LOG_DEBUG
  #include "data_msg.h"
  #include "fs_public.h"
#endif

#ifdef __cplusplus
}
#endif  /* extern "C" */

#include "ps_cert_http.h"
#include <stringl/stringl.h>

#ifdef FEATURE_DATA_APP_LOG_DEBUG
#include "ps_AppEFS.h"

using namespace PS_APP;
#endif /* FEATURE_DATA_APP_LOG_DEBUG */

using namespace PS_CERT;
/*===========================================================================
                          VARIABLES AND MACRO DEFINITIONS
==========================================================================*/

/*---------------------------------------------------------------------------
  RFC allowed max len of FQDN host
---------------------------------------------------------------------------*/
#define PS_CERT_MAX_FQDN_HOST 255

/*---------------------------------------------------------------------------
  Max allowed URL length with FQDN host
---------------------------------------------------------------------------*/
#define PS_CERT_MAX_URI_LEN PS_CERT_MAX_FQDN_HOST + 255

/*---------------------------------------------------------------------------
  Default directory for data certificates
---------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_APP_LOG_DEBUG
  #define PS_CERT_UTIL_EFS_DIR "/data/cert/"
#endif 

/*---------------------------------------------------------------------------
  Queue for uri fetch requests
---------------------------------------------------------------------------*/
static q_type  ps_cert_http_queue;

/*---------------------------------------------------------------------------
  state variable to safe guard module intialization 
---------------------------------------------------------------------------*/
static boolean ps_cert_http_is_init = FALSE;

/*---------------------------------------------------------------------------
  Internal session id to keep track of HTTP session (HTTP cert)
---------------------------------------------------------------------------*/
static uint32  cert_session_id;

/*---------------------------------------------------------------------------
  Internal session id to keep track of HTTP session (OCSP)
  Todo generate list of session_id 
---------------------------------------------------------------------------*/
static uint32  ocsp_session_id;

/*---------------------------------------------------------------------------
  Instance of cert HTTP utility 
---------------------------------------------------------------------------*/
static CertHttpUtils          *certHttpUtils = NULL;

/*---------------------------------------------------------------------------
  Instance of ocsp HTTP utility 
---------------------------------------------------------------------------*/
static CertHttpUtils          *postHttpUtils = NULL;

/*---------------------------------------------------------------------------
  Queue request entry
---------------------------------------------------------------------------*/
typedef PACKED struct
{
  q_link_type            link;
  ps_cert_handle_type    cert_req_handle;
  ps_cert_url_cback_type client_cback;
  void*                  client_userdata;
  const char*            url;
  rex_timer_type*        http_timer;
}ps_cert_request_q_entry_type;

/*===========================================================================
FUNCTION     ps_cert_handle_clean_entry

DESCRIPTION   Utility function to clean queue entry

DEPENDENCIES  None.

RETURN VALUE  None
SIDE EFFECTS  None.
===========================================================================*/
void ps_cert_handle_clean_entry
(
  ps_cert_request_q_entry_type* entry
)
{
  if ( entry == NULL )
  {
    return;
  }

  PS_APP_DEBUG_1("Cleaning cert handle %d",
                 entry->cert_req_handle);

  /*--------------------------------------------------------------------- 
    Free URL memory, timer memory
  ---------------------------------------------------------------------*/
  if ( entry->url != NULL )
  {
    PS_SYSTEM_HEAP_MEM_FREE(entry->url);
  }

  if ( NULL != entry->http_timer )
  {
    PS_SYSTEM_HEAP_MEM_FREE(entry->http_timer);
  }

  /*--------------------------------------------------------------------- 
    Remove entry from queue
  ---------------------------------------------------------------------*/
  q_delete(&ps_cert_http_queue, &(entry->link));

  /*--------------------------------------------------------------------- 
    Free memory allocated for entry
  ---------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(entry);

} /* ps_cert_handle_clean_entry */


/*===========================================================================
FUNCTION     PS_CERT_HANDLE_Q_COMPARE_FCT

DESCRIPTION   Utility function to compare two items in the cert queue

DEPENDENCIES  None.

RETURN VALUE  1: if the items are equal
              0: if items are not equal

SIDE EFFECTS  None.
===========================================================================*/
int ps_cert_handle_q_compare_fct
(
  void *item_ptr,
  void *target_val
)
{
  ps_cert_request_q_entry_type            *request_entry = NULL;
  ps_cert_handle_type                     *to_compare = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if((NULL == item_ptr) || (NULL == target_val))
  {
    return FALSE;
  }

  /*--------------------------------------------------------------------- 
    Compare cert handle values of target and entry
  ---------------------------------------------------------------------*/
  request_entry = (ps_cert_request_q_entry_type*)item_ptr;
  to_compare = (ps_cert_handle_type *)target_val;

  PS_APP_DEBUG_2("queue search, entry handle %d, current"
                  " request id %d",
                  request_entry->cert_req_handle,
                  *to_compare);

  if( request_entry->cert_req_handle == *to_compare) 
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}/* ps_cert_handle_q_compare_fct*/


/*===========================================================================
FUNCTION     PS_CERT_HTTP_LOAD_CERT

DESCRIPTION   Load HTTP data to certificate container

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  certs is filled with certificate data. 
              num_certs indicates the number of certificates in container 
===========================================================================*/
void ps_cert_http_load_cert
(
  ps_cert_container_type* certs,
  const uint8* content,
  uint32 content_size,
  uint16 * num_certs
)
{
  *num_certs = 0;

  if ( NULL == content )
  {
    return;
  }

  /* FN debug todo add call to PEM converter to detect multiple PEM cert */

  certs->certdata = content;
  certs->certlen = content_size;
  *num_certs = *num_certs + 1;
  certs->next_cert = NULL;

  PS_APP_DEBUG_2("ps_cert_http_load_cert loading certs, content = 0x%p,"
                 "content len %d",content, content_size);
  PS_APP_DEBUG_3("ps_cert_http_load_cert loading certs, data = 0x%p,"
                 "data_len len %d, num certs %d",
                 certs->certdata, certs->certlen, *num_certs);
} /* ps_cert_http_load_cert */

/*===========================================================================
FUNCTION     PS_CERT_HTTP_FREE_CERT_CONTAINER

DESCRIPTION   Gracefully deallocates memory used in the cert container

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ps_cert_free_cert_container
(
  ps_cert_container_type* certs
)
{
  ps_cert_container_type* iter_certs = certs;

  while (iter_certs != NULL )
  {
    PS_APP_DEBUG_1("ps_cert_free_cert_container iter_certs len %d",
                    iter_certs->certlen );
    iter_certs = certs->next_cert;
    PS_SYSTEM_HEAP_MEM_FREE(certs);
    certs = iter_certs;
  }
} /*ps_cert_free_cert_container*/

/*===========================================================================
FUNCTION  ps_cert_http_process_http_result()

DESCRIPTION  Process HTTP results and makes callback to client
  
PARAMETERS  None.

DEPENDENCIES None.
  
RETURN VALUE None. 
  
SIDE EFFECTS None.
===========================================================================*/
void ps_cert_http_process_http_result
(
  uint32 session_id,
  uint32 request_id,
  const uint8* http_data,
  uint32 http_data_len,
  sint15 err_code
)
{
  ps_cert_request_q_entry_type* request_entry = NULL;
#ifdef FEATURE_DATA_APP_LOG_DEBUG
  AppEFS* efs_util;
  const char* filename;
  int file_handle;
  int bytes_written;
  ps_cert_container_type* certs_iter = NULL;
  uint8 i=0;
#endif
  ps_cert_container_type* certs = NULL;
  uint16 num_of_certs = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*--------------------------------------------------------------------- 
    Verify session id
  ---------------------------------------------------------------------*/
  if ( session_id != cert_session_id &&
       session_id != ocsp_session_id )
  {
    PS_APP_ERR_LOG_3(session_id, cert_session_id, ocsp_session_id);
    return;
  }

  /*--------------------------------------------------------------------- 
    Find the entry corresponding to this request in queue
  ---------------------------------------------------------------------*/
  if ( 0 == q_cnt(&ps_cert_http_queue) )
  {
    PS_APP_ERR_LOG_1(&ps_cert_http_queue);
    return;
  }

  request_entry = (ps_cert_request_q_entry_type*) 
                  q_linear_search( &ps_cert_http_queue,
                                   ps_cert_handle_q_compare_fct, 
                                   (void *)&request_id );


  if ( NULL != request_entry )
  {
    /*----------------------------------------------------------------- 
     Clear timer 
    ------------------------------------------------------------------*/
    rex_clr_timer(request_entry->http_timer);

    PS_APP_DEBUG_3("HTTP cert response recieved for handle %d, "
                   "num of certs %d, err_code %i",
                   request_entry->cert_req_handle, 
                   num_of_certs, err_code);

    /*--------------------------------------------------------------------- 
     Load certificate into container structure
    ---------------------------------------------------------------------*/
    if ( NULL != http_data && http_data_len > 0 )
    {
      certs = (ps_cert_container_type*) ps_system_heap_mem_alloc(
         sizeof(ps_cert_container_type));

      if ( NULL == certs )
      {
        PS_APP_ERR_LOG_1(sizeof(ps_cert_container_type));
        ps_cert_handle_clean_entry(request_entry);
        return;
      }

      ps_cert_http_load_cert(certs,
                             http_data,
                             http_data_len, 
                             &num_of_certs);
    }
    else
    {
      certs = NULL;
      num_of_certs = 0;
    }

#ifdef FEATURE_DATA_APP_LOG_DEBUG
    certs_iter = certs;
    while ( i < num_of_certs &&  certs_iter != NULL )
    {
      PS_APP_DEBUG_3("Certs[%i] 0x%p len 0x%i, ",
                       i, certs_iter->certdata, certs_iter->certlen ); 
      efs_util = new AppEFS();
       
      if ( NULL == efs_util )
      {
        PS_APP_ERR_LOG_0();
        break;
      }

      filename = "debug.txt";

      file_handle = efs_util->OpenFile(filename, 
                                       PS_CERT_UTIL_EFS_DIR, NULL, TRUE);

      if ( file_handle == -1 )
      {
        PS_APP_ERR_LOG_0();
        break;
      }

      bytes_written = efs_util->WritetoFile( certs_iter->certdata, 
                                              certs_iter->certlen, FALSE);

      PS_APP_DEBUG_2("file handle %i bytes_written %i",
                      file_handle,bytes_written); 
      i++;
      certs_iter = certs_iter->next_cert;
      delete efs_util;
    }

    PS_APP_DEBUG_3("Sending cback,certs 0x%p url %p userdata %p",
                certs, request_entry->url, request_entry->client_userdata);               
#endif /*FEATURE_DATA_APP_LOG_DEBUG*/

    /*----------------------------------------------------------------- 
      Make callback to client
    ------------------------------------------------------------------*/
    request_entry->client_cback( request_entry->cert_req_handle,
                                  certs,
                                  num_of_certs,
                                  request_entry->url, 
                                  request_entry->client_userdata,
                                  err_code );
    /*-----------------------------------------------------------------
      Free certificate
    ------------------------------------------------------------------*/
    ps_cert_free_cert_container(certs);

    /*----------------------------------------------------------------- 
      Clean entry memory
    ------------------------------------------------------------------*/
    ps_cert_handle_clean_entry(request_entry);
  }
  else
  {
    /*----------------------------------------------------------------- 
      Request not found in queue session id
    ------------------------------------------------------------------*/
    PS_APP_ERR_LOG_2(request_id, err_code);

  }
}

/*===========================================================================
FUNCTION  ps_cert_http_timeout_cb()

DESCRIPTION  Handles a timeout 
  
PARAMETERS  
  None.

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ps_cert_http_timeout_cb
(
  unsigned long user_data_ptr
)
{
  uint32 request_id = (uint32)user_data_ptr;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*----------------------------------------------------------------- 
   Send timeout error 
  ------------------------------------------------------------------*/
  PS_APP_ERR_LOG_1(request_id);
  PS_CERT::ps_cert_http_error_cb_fcn(cert_session_id,
                                     request_id, 
                                     PS_CERT_HTTP_TOUT_ERR );

}

/*===========================================================================
FUNCTION  ps_ocsp_http_timeout_cb()

DESCRIPTION  Handles a timeout 
  
PARAMETERS  
  None.

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ps_cert_ocsp_timeout_cb
(
  unsigned long user_data_ptr
)
{
  uint32 request_id = (uint32)user_data_ptr;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*----------------------------------------------------------------- 
   Send timeout error 
  ------------------------------------------------------------------*/
  PS_APP_ERR_LOG_1(request_id);
  PS_CERT::ps_cert_http_error_cb_fcn(ocsp_session_id,
                                     request_id, 
                                     PS_CERT_HTTP_TOUT_ERR );

}

/*=============================================================================
                       EXTERNAL FUNCTION DEFINITIONS
==============================================================================*/

/*===========================================================================
FUNCTION      PS_CERT_HTTP_INIT

DESCRIPTION   fetches certificate URI given an url and returns a callback

DEPENDENCIES  None.

RETURN VALUE  handle to certificate fetching

SIDE EFFECTS  None 
===========================================================================*/
void ps_cert_http_init(void)
{

  if ( TRUE == ps_cert_http_is_init )
  {
    return;
  }

  /*---------------------------------------------------------------------
    Initialize http request queue and state variables
  ---------------------------------------------------------------------*/
  q_init(&ps_cert_http_queue);

  certHttpUtils = NULL;
  postHttpUtils = NULL;

  cert_session_id = DS_HTTP_ERROR;
}

/*===========================================================================
FUNCTION      PS_CERT_HTTP_DEINIT

DESCRIPTION   fetches certificate URI given an url and returns a callback

DEPENDENCIES  None.

RETURN VALUE  handle to certificate fetching

SIDE EFFECTS  None 
===========================================================================*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */
/*===========================================================================
FUNCTION      PS_CERT_FETCH_URI

DESCRIPTION   fetches certificate URI given an url and returns a callback

DEPENDENCIES  None.

RETURN VALUE  handle to certificate fetching

SIDE EFFECTS  None 
===========================================================================*/
ps_cert_handle_type ps_cert_fetch_uri
( 
  uint8 * uri,
  uint16 uri_len, 
  ps_cert_url_cback_type ps_cert_url_cback,
  ps_cert_task_swtch_fptr task_cback,
  uint32 http_timeout,
  void* userdata,
  ds_http_iface_info_s_type *iface_info
)
{
  uint32 request_id = DS_HTTP_ERROR;
  ps_cert_request_q_entry_type* request_entry;
  char* cert_uri = NULL;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  PS_APP_SPRINTF_DEBUG_1("Fetch certificate for URL: %s", uri);

  /*---------------------------------------------------------------------
    Sanity check and copy URI if passes validity check
  ---------------------------------------------------------------------*/
  if ( uri_len > PS_CERT_MAX_URI_LEN)
  {
    PS_APP_ERR_LOG_1(uri_len);
    return FALSE;
  }
  else
  {
    cert_uri = (char*) ps_system_heap_mem_alloc(uri_len+1);

    if ( NULL == cert_uri )
    {
      PS_APP_ERR_LOG_1(uri_len);
      return FALSE;
    }

    memscpy(cert_uri, (uri_len+1), uri, uri_len);
    cert_uri[uri_len] = '\0';
  }

  /*---------------------------------------------------------------------
    Initialize certHttpUtils on first fetch
  ---------------------------------------------------------------------*/
  if ( NULL == certHttpUtils )
  {
    certHttpUtils = new CertHttpUtils(ps_cert_http_process_http_result, 
                                      &cert_session_id,
                                      task_cback,
                                      iface_info);
    
    if ( NULL == certHttpUtils )
    {
      PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
      return PS_CERT_HANDLE_INVALID;
    }
    PS_APP_DEBUG_1("Created session id %d", cert_session_id);  
  }

  /*---------------------------------------------------------------------
    Call HTTP fetch utility to get certificate
  ---------------------------------------------------------------------*/
  request_id = certHttpUtils->HttpGet((uint8*)cert_uri, uri_len);
  if ( DS_HTTP_ERROR == request_id )
  {
    PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
    return PS_CERT_HANDLE_INVALID;
  }

  /*---------------------------------------------------------------------
    Allocate memory for request 
  ---------------------------------------------------------------------*/
  request_entry = (ps_cert_request_q_entry_type*) 
    ps_system_heap_mem_alloc(sizeof(ps_cert_request_q_entry_type));

  if (  NULL == request_entry )
  {
    PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
    PS_APP_ERR_LOG_1(sizeof(ps_cert_request_q_entry_type));
    return PS_CERT_HANDLE_INVALID;
  }

  /*---------------------------------------------------------------------
    Save client information and save to request_entry
     cert_req_handle = request_id
  ---------------------------------------------------------------------*/
  PS_APP_DEBUG_2("Adding cert handle %d to queue, timeout %d", 
                  request_id, http_timeout);

  request_entry->client_cback = ps_cert_url_cback;
  request_entry->client_userdata = userdata;
  request_entry->cert_req_handle = request_id;
  request_entry->url = (const char*)cert_uri;

  /*---------------------------------------------------------------------
    allocate timer for request entry
  ---------------------------------------------------------------------*/
  request_entry->http_timer = (rex_timer_type *) 
    ps_system_heap_mem_alloc(sizeof(rex_timer_type));

  if ( NULL == request_entry->http_timer  )
  {
    PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
    PS_SYSTEM_HEAP_MEM_FREE(request_entry);
    PS_APP_ERR_LOG_1(sizeof(rex_timer_type));
    return PS_CERT_HANDLE_INVALID;
  }

  rex_def_timer_ex(request_entry->http_timer, ps_cert_http_timeout_cb,
                   (unsigned long) request_id);

  q_link((void*)request_entry, &(request_entry->link));
  q_put(&ps_cert_http_queue,&(request_entry->link));

  /*---------------------------------------------------------------------
    Convert http timeout to msec and start timer
  ---------------------------------------------------------------------*/
  rex_set_timer(request_entry->http_timer, (http_timeout*1000));

  return request_entry->cert_req_handle;
}

/*===========================================================================
FUNCTION  ps_cert_abort_fetch()

DESCRIPTION  Aborts a fetch to avoid callback
  
PARAMETERS  
  None.

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ps_cert_abort_fetch
(
  ps_cert_handle_type cert_handle
)
{
  ps_cert_request_q_entry_type* request_entry = NULL;

  PS_APP_DEBUG_1("Aborting fetch for cert handle 0x%d",cert_handle);

  if ( cert_handle == PS_CERT_HANDLE_INVALID )
  {
    return;
  }

  /*--------------------------------------------------------------------- 
    Clean all sessions associated with this cert handle
  ---------------------------------------------------------------------*/
  do
  {
    /*--------------------------------------------------------------------- 
      Find the entry in the queue and delete it 
    ---------------------------------------------------------------------*/
    request_entry = (ps_cert_request_q_entry_type*) 
                    q_linear_search( &ps_cert_http_queue,
                       ps_cert_handle_q_compare_fct, 
                       (void *)&cert_handle );

    /*--------------------------------------------------------------------- 
      Delete the request , remove entry from queue, and free entry memory
    ---------------------------------------------------------------------*/
    if ( NULL != request_entry )
    {
      rex_clr_timer( request_entry->http_timer);
      ps_cert_handle_clean_entry(request_entry);
    }
  }
  while ( request_entry != NULL ); 

  return;
}

/*===========================================================================
FUNCTION      PS_CERT_FETCH_POST

DESCRIPTION   fetches HTTP POST given an url with POST rquest contents and 
              returns a callback with the HTTP POST response

DEPENDENCIES  None.

RETURN VALUE  handle to ocsp fetching

SIDE EFFECTS  None 
===========================================================================*/
ps_cert_handle_type ps_cert_fetch_post
( 
  uint8 * uri,
  uint16 uri_len, 
  uint8 * post_request,
  uint32  post_request_len,
  ds_http_header_s_type *http_header_request,
  ps_cert_url_cback_type ps_cert_url_cback,
  ps_cert_task_swtch_fptr task_cback,
  uint32 http_timeout,
  void* userdata,
  ds_http_iface_info_s_type *iface_info
)
{
  uint32 request_id = DS_HTTP_ERROR;
  ps_cert_request_q_entry_type* request_entry;
  char* cert_uri = NULL;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  PS_APP_SPRINTF_DEBUG_1("Fetch POST for ocsp: %s", uri);
  PS_APP_DEBUG_1("Fetch certificate ocsp uri len %d", uri_len);  

  /*---------------------------------------------------------------------
    Sanity check and copy URI if passes validity check
  ---------------------------------------------------------------------*/
  if ( uri_len > PS_CERT_MAX_URI_LEN || NULL == post_request || 
       0 >= post_request_len || NULL == http_header_request ||
       0 == strlen(http_header_request->user_agent) ||
       0 == strlen(http_header_request->content_type) )
  {
    PS_APP_ERR_LOG_3(uri_len, post_request_len, http_header_request);
    return FALSE;
  }
  else
  {
    cert_uri = (char*) ps_system_heap_mem_alloc(uri_len+1);
    if ( NULL == cert_uri )
    {
      PS_APP_ERR_LOG_1(uri_len);
      return FALSE;
    }

    memscpy(cert_uri, (uri_len+1), uri, uri_len);
    cert_uri[uri_len] = '\0';
  }

  /*---------------------------------------------------------------------
    Initialize certHttpUtils on first fetch
  ---------------------------------------------------------------------*/
  if ( NULL == postHttpUtils )
  {
    postHttpUtils = new CertHttpUtils(ps_cert_http_process_http_result, 
                                      &ocsp_session_id,
                                      task_cback,
                                      iface_info);
    
    if ( NULL == postHttpUtils )
    {
      PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
      return PS_CERT_HANDLE_INVALID;
    }
    PS_APP_DEBUG_1("Created OCSP HTTP session id %d", ocsp_session_id);  
  }

  /*---------------------------------------------------------------------
    Call HTTP fetch utility to do HTTP POST
  ---------------------------------------------------------------------*/
  request_id = postHttpUtils->HttpPost((const char*)cert_uri, uri_len, 
    post_request, post_request_len,http_header_request);

  if ( DS_HTTP_ERROR == request_id )
  {
    PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
    return PS_CERT_HANDLE_INVALID;
  }

  /*---------------------------------------------------------------------
    Allocate memory for request 
  ---------------------------------------------------------------------*/
  request_entry = (ps_cert_request_q_entry_type*) 
    ps_system_heap_mem_alloc(sizeof(ps_cert_request_q_entry_type));

  if (  NULL == request_entry )
  {
    PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
    PS_APP_ERR_LOG_1(sizeof(ps_cert_request_q_entry_type));
    return PS_CERT_HANDLE_INVALID;
  }

  /*---------------------------------------------------------------------
    Save client information and save to request_entry
     cert_req_handle = request_id
  ---------------------------------------------------------------------*/
  PS_APP_DEBUG_2("Adding cert handle %d to queue, timeout %d", 
                  request_id, http_timeout);

  request_entry->client_cback = ps_cert_url_cback;
  request_entry->client_userdata = userdata;
  request_entry->cert_req_handle = request_id;
  request_entry->url = (const char*)cert_uri;

  /*---------------------------------------------------------------------
    allocate timer for request entry
  ---------------------------------------------------------------------*/
  request_entry->http_timer = (rex_timer_type *) 
    ps_system_heap_mem_alloc(sizeof(rex_timer_type));

  if ( NULL == request_entry->http_timer  )
  {
    PS_SYSTEM_HEAP_MEM_FREE(cert_uri);
    PS_SYSTEM_HEAP_MEM_FREE(request_entry);
    PS_APP_ERR_LOG_1(sizeof(rex_timer_type));
    return PS_CERT_HANDLE_INVALID;
  }

  rex_def_timer_ex(request_entry->http_timer, ps_cert_ocsp_timeout_cb,
                   (unsigned long) request_id);

  q_link((void*)request_entry, &(request_entry->link));
  q_put(&ps_cert_http_queue,&(request_entry->link));

  /*---------------------------------------------------------------------
    Convert http timeout to msec and start timer
  ---------------------------------------------------------------------*/
  rex_set_timer(request_entry->http_timer, (http_timeout*1000));

  return request_entry->cert_req_handle;
}
