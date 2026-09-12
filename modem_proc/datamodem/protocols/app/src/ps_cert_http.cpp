/*=========================================================================*/
/*!
@file
ps_cert_http.cpp

@brief
Provides helper routines to do HTTP fetching

Copyright (c) 2015-2024 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/dev/data.mpss/3.3/fning.data.mpss.3.3.fning_1_12_check_in/

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/06/24   ak      Fix to set content-type in http post
06/29/18   fn      Fix unitialize next_ptr for session list
02/06/18   fn      FR 29585 Added support for http post
01/14/16   kc      Fixed KW error
10/08/15   fn      Updated to HTTP API DSDS changes
04/10/15   fn      Created module
===========================================================================*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/

#include <stringl/stringl.h>
#include "comdef.h"

#include "ps_cert_http.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ds_http_api.h"
#include "ps_svc.h"
#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_app_logging_util.h"

#ifdef FEATURE_DATA_APP_LOG_DEBUG
  #include "data_msg.h"
#endif

#ifdef __cplusplus
}
#endif  // extern "C"

#if defined(T_WINNT)
#error code not present
#endif /* WINNT */

using namespace PS_CERT;

/*===========================================================================

  Internal Structure

===========================================================================*/
/*---------------------------------------------------------------------------
  struct to store relevant information for task switch
---------------------------------------------------------------------------*/
struct ps_cert_http_evt_info
{

  uint32 session_id;
  uint32 request_id;
  sint15 error;
  const uint8* content;
  uint32 content_size;

   ps_cert_http_evt_info(uint32 sid, uint32 rid, sint15 err, 
                         const uint8* buf, uint32 buf_size)
  : session_id(sid), request_id(rid), error(err), 
    content(buf),content_size(buf_size) { }
};

typedef struct ps_cert_http_inst_type
{
  uint32 session_id;
  ps_cert_task_swtch_fptr task_cback;
  ps_cert_http_inst_type* next_inst;
}ps_cert_http_inst_type;


static PS_CERT::ps_cert_http_cback_type cert_req_cback;
static ps_cert_http_inst_type *cert_http_insts = NULL;
/*===========================================================================

  PRIVATE METHODS

===========================================================================*/

/**-------------------------------------------------------------------------- 
*   @fn PS_CERT_CONVERT_HTTP_ERROR
*   @brief Converts HTTP error to PS CERT erros
*   @return PS cert error definied ps_cert_utils.h
---------------------------------------------------------------------------*/
sint15 ps_cert_convert_http_error
(
  sint15 http_err
)
{
  sint15 err = PS_CERT_HTTP_GEN_ERR;

  switch ( http_err )
  {
     case DS_HTTP_STATUS_OK:
       err = PS_CERT_NO_ERR;
       break;
     case PS_CERT_HTTP_TOUT_ERR: 
       /*cert timeout special case*/
       err = PS_CERT_HTTP_TOUT_ERR;
       break;
     case DS_HTTP_ERROR_SSL:
       err = PS_CERT_HTTP_ACCESS_ERR;
       break;
     case DS_HTTP_ERROR_HTTP_STATUS:
       err = PS_CERT_HTTP_STATUS_ERR;
       break;
     case DS_HTTP_ERROR_DNS:
       err = PS_CERT_HTTP_DNS_ERR;
       break;
     case DS_HTTP_ERROR_DSNET:
     case DS_HTTP_ERROR_SOCKET:
     case DS_HTTP_ERROR_HTTP_HEADER:
     case DS_HTTP_ERROR_NETWORK_DOWN:
     default:
       err = PS_CERT_HTTP_GEN_ERR;
       break;
  }

  PS_APP_DEBUG_2("Converted HTTP errno %i to PS errno %i",http_err, err);

  return err;
}

/**-------------------------------------------------------------------------- 
*   @fn ps_cert_http_add_inst()
*   @brief adds session id to inst block
---------------------------------------------------------------------------*/
void ps_cert_http_add_inst
(
  uint32 session_id, 
  ps_cert_task_swtch_fptr task_cback
)
{
  ps_cert_http_inst_type** http_inst = &cert_http_insts;
  
  while ( NULL != http_inst && NULL != (*http_inst))
  {
    if (session_id == (*http_inst)->session_id )
    {
      return;
    }

    http_inst = &(cert_http_insts->next_inst); 
  }

  /*--------------------------------------------------------------------- 
    Allocate new instance to list if it is not found in list 
  ---------------------------------------------------------------------*/
  PS_APP_DEBUG_1("Adding HTTP session id inst %d", session_id );
  *http_inst = (ps_cert_http_inst_type*) ps_system_heap_mem_alloc (
     sizeof(ps_cert_http_inst_type));

  if ( NULL == *http_inst)
  {
    PS_APP_ERR_LOG_1(sizeof(ps_cert_http_inst_type));
    return;
  }

  (*http_inst)->next_inst = NULL;
  (*http_inst)->session_id = session_id;
  (*http_inst)->task_cback = task_cback;
  
} /* ps_cert_http_add_inst */

/**-------------------------------------------------------------------------- 
*   @fn ps_cert_http_lookup_inst()
*   @brief returns inst block for session_id, returns NULL if can't find
---------------------------------------------------------------------------*/
ps_cert_http_inst_type* ps_cert_http_lookup_inst
(
   uint32 session_id
)
{
  ps_cert_http_inst_type* http_inst = cert_http_insts;
  
  while ( NULL != http_inst)
  {

    PS_APP_DEBUG_2("Session id inst %d lookup %d", 
                    http_inst->session_id , session_id );

    if (session_id == http_inst->session_id )
    {
      return http_inst;
    }
    http_inst = http_inst->next_inst;
  }

  return NULL;
}/* ps_cert_http_lookup_inst */

/**-------------------------------------------------------------------------- 
*   @fn ps_cert_util_evt_handler()
*   @brief Handles PS cmd for HTTP request callbacks
---------------------------------------------------------------------------*/
static void ps_cert_util_evt_handler
(
   ps_cmd_enum_type cmd,
   void* httpdata
)
{
  ps_cert_http_evt_info* http_evt = (ps_cert_http_evt_info*) httpdata;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*--------------------------------------------------------------------- 
    Verify inputs
  ---------------------------------------------------------------------*/
  if (NULL == http_evt ||cmd != PS_CERT_UTIL_CMD || NULL == cert_req_cback)
  {
    PS_APP_ERR_LOG_3(http_evt,cmd,cert_req_cback);
    return;
  }

  /*--------------------------------------------------------------------- 
    Process HTTP result and send information to client
  ---------------------------------------------------------------------*/
  cert_req_cback(
     http_evt->session_id, 
     http_evt->request_id, 
     http_evt->content,
     http_evt->content_size,
     http_evt->error );


  if ( NULL != http_evt->content )
  {
    PS_SYSTEM_HEAP_MEM_FREE(http_evt->content);
  }
  delete http_evt;
} /* ps_cert_util_evt_handler */

/**-------------------------------------------------------------------------- 
*   @fn ps_cert_user_task_switch()
*   @brief Handles user task switches
---------------------------------------------------------------------------*/
static void ps_cert_user_task_switch
(
  void* httpdata
)
{
   PS_APP_DEBUG_1("User task switch recieved 0x%p", httpdata);

  /*--------------------------------------------------------------------- 
    Use same callback function as PS task
  ---------------------------------------------------------------------*/
  ps_cert_util_evt_handler(PS_CERT_UTIL_CMD, httpdata);
}

/**-------------------------------------------------------------------------- 
*   @fn ps_cert_http_cb_fcn()
*   @brief Callback function to handle HTTP request callbacks
---------------------------------------------------------------------------*/
void ps_cert_http_cb_fcn
(
  uint32                         session_id,
  uint32                         request_id,
  sint15                         error,
  uint16                         http_status,
  const uint8*                   content,
  uint32                         content_size,
  const ds_http_response_header* header_info,
  uint32                         num_headers
)
{
  ps_cert_http_evt_info* http_evt = new ps_cert_http_evt_info(session_id, request_id, 
                                                             error, content, content_size);
  uint8* save_content = NULL;
  ps_cert_http_inst_type *inst = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if (error == DS_HTTP_READY || NULL == http_evt)
  {
    PS_APP_ERR_LOG_1(error);
    if ( NULL != http_evt )
    {
      delete http_evt;
    }
    return;
  }
  /*--------------------------------------------------------------------- 
    Convert error to PS error
  ---------------------------------------------------------------------*/
  http_evt->error = ps_cert_convert_http_error(error);

  /*--------------------------------------------------------------------- 
    Copy contents if the HTTP request is successful
  ---------------------------------------------------------------------*/
  switch(http_evt->error)
  {
    case PS_CERT_NO_ERR:
      save_content = (uint8*)ps_system_heap_mem_alloc(content_size);
      if ( NULL == save_content )
      {
        PS_APP_ERR_LOG_1(content_size);
        delete http_evt;
        return; 
      }
      memscpy(save_content, content_size, content, content_size);
      http_evt->content = save_content;
      break;
    default:
      PS_APP_ERR_LOG_2(error, http_evt->error);
      http_evt->content = NULL; 
      http_evt->content_size = 0;
      break;
  }

  inst = ps_cert_http_lookup_inst( session_id );

  /*--------------------------------------------------------------------- 
    Default to at least do a task switch using PS command
      -used for timer errors
      -dstask cback from HTTP utilities
    If user provied task swtitch function then use it to do a task
    switch to user context
  ---------------------------------------------------------------------*/
  if ( NULL == inst || NULL == inst->task_cback )
  {
    (void)ps_set_cmd_handler(PS_CERT_UTIL_CMD, ps_cert_util_evt_handler);
    ps_send_cmd(PS_CERT_UTIL_CMD,(void*) http_evt );
  }
  else
  {
    (void)inst->task_cback(ps_cert_user_task_switch, (void*) http_evt);
  }

}/*ps_cert_http_cb_fcn*/

/**-------------------------------------------------------------------------- 
*   @fn ps_cert_http_error_cb_fcn()
*   @brief Callback function to handle HTTP error request callbacks
---------------------------------------------------------------------------*/
void PS_CERT::ps_cert_http_error_cb_fcn
(
  uint32                         session_id,
  uint32                         request_id,
  sint15                         error
)
{
  ps_cert_http_cb_fcn( session_id,
                       request_id, 
                       PS_CERT_HTTP_TOUT_ERR,
                       0, NULL , 0, NULL, 0 );

} /* ps_cert_http_error_cb_fcn */

/**-------------------------------------------------------------------------- 
*   @fn ps_cert_block_cb_fcn()
*   @brief Handles credentials for HTTPS / SSL 
---------------------------------------------------------------------------*/
void ps_cert_block_cb_fcn
(
  uint32                         session_id,
  uint32                         request_id,
  ds_http_block_event_type       event,
  ds_http_block_event_info_type* event_info
)
{

  /*--------------------------------------------------------------------- 
    Currently no support for http credentails for HTTPS/SSL
  ---------------------------------------------------------------------*/
  PS_APP_ERR_LOG_3(session_id, request_id, event);

  PS_CERT::ps_cert_http_error_cb_fcn(session_id, 
                                     request_id, 
                                     DS_HTTP_ERROR_SSL);

  return;
}/*ps_cert_block_cb_fcn*/

/*===========================================================================

PUBLIC METHODS

===========================================================================*/
/**-------------------------------------------------------------------------- 
*   @fn CertHTTPUtils()
*   @brief Standard constructor
---------------------------------------------------------------------------*/
CertHttpUtils::CertHttpUtils
(
  ps_cert_http_cback_type req_cback,
  uint32 * session_id,
  ps_cert_task_swtch_fptr cback,
  ds_http_iface_info_s_type *iface_info
):
ps_cert_http_session_id(DS_HTTP_ERROR),
ps_cert_http_request_id(DS_HTTP_ERROR),
cert_uri(NULL),
task_cback(cback)
{
  ds_http_iface_info_s_type http_iface_info;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*--------------------------------------------------------------------- 
    Verify input parameters
  ---------------------------------------------------------------------*/
  if ( NULL == req_cback || NULL == session_id || NULL == iface_info )
  {
    delete this;
    return;
  }

  cert_req_cback = req_cback;
  memset(&http_iface_info,0,sizeof(http_iface_info));
  /*--------------------------------------------------------------------- 
    Currently only supports fetching on WLAN
  ---------------------------------------------------------------------*/
  http_iface_info.profile_id = iface_info->profile_id;
  http_iface_info.iface_type = iface_info->iface_type;
  http_iface_info.subs_id = iface_info->subs_id;

  ps_cert_http_session_id = ds_http_open_session(&ps_cert_http_cb_fcn, 
                                                 &ps_cert_block_cb_fcn, 
                                                 &http_iface_info);

  PS_APP_DEBUG_1("CertHttpUtils session_id 0x%x",ps_cert_http_session_id);

  *session_id = ps_cert_http_session_id;

  if(DS_HTTP_ERROR == ps_cert_http_session_id)
  {
    PS_APP_ERR_LOG_1(ps_cert_http_session_id);
    delete this;
    return;
  }

  ps_cert_http_add_inst(ps_cert_http_session_id, 
                       task_cback);
}/*CertHttpUtils::CertHttpUtils*/

/**-------------------------------------------------------------------------- 
*   @fn  ~CertUtils()
*   @brief Default destructor.
---------------------------------------------------------------------------*/
CertHttpUtils::~CertHttpUtils ()
{
  ps_cert_http_inst_type *inst;
  PS_APP_DEBUG_2("CertHttpUtils closing session_id 0x%x request id 0x%x",
                 ps_cert_http_session_id, ps_cert_http_request_id);


 inst = ps_cert_http_lookup_inst(ps_cert_http_session_id);

 if ( NULL != inst)
 {
   PS_SYSTEM_HEAP_MEM_FREE(inst);
 }

}/*CertHttpUtils::~CertHttpUtils*/

/**-------------------------------------------------------------------------- 
*   @fn  HttpGet()
*   @brief HTTP get for certificate
---------------------------------------------------------------------------*/
uint32 CertHttpUtils::HttpGet
(
  uint8* uri,
  uint16 uri_len
)
{
  if ( NULL == uri || DS_HTTP_ERROR == ps_cert_http_session_id ||
       cert_uri != NULL  )
  {
    PS_APP_ERR_LOG_3(uri,cert_uri,ps_cert_http_session_id);
    return DS_HTTP_ERROR;
  }

  /*--------------------------------------------------------------------- 
    Verify URI length and point to URI
  ---------------------------------------------------------------------*/
  cert_uri = (char*)uri;
  cert_uri_len = uri_len;

  return ProcessHttpGet();
} /* CertHttpUtils::GetCert */


/**-------------------------------------------------------------------------- 
*   @fn  HttpGet()
*   @brief HTTP get for certificate
---------------------------------------------------------------------------*/
uint32 CertHttpUtils::HttpPost
(
  const char* uri,
  uint16 uri_len,
  uint8* http_post_content,
  uint16 http_post_len,
  ds_http_header_s_type *http_header_request
)
{
  if ( NULL == uri || DS_HTTP_ERROR == ps_cert_http_session_id ||
       cert_uri != NULL  )
  {
    PS_APP_ERR_LOG_3(uri,cert_uri,ps_cert_http_session_id);
    return DS_HTTP_ERROR;
  }

  /*--------------------------------------------------------------------- 
    Verify URI length and point to URI
  ---------------------------------------------------------------------*/
  cert_uri = (char*)uri;
  cert_uri_len = uri_len;

  return ProcessHttpPost(http_post_content, http_post_len ,http_header_request);
} /* CertHttpUtils:: */

/**-------------------------------------------------------------------------- 
*   @fn  ProcessHttpGet()
*   @brief process HTTP get and makes get request
---------------------------------------------------------------------------*/
uint32 CertHttpUtils::ProcessHttpGet
(
  void 
)
{
  sint15 request_errno = 0;
  ds_http_header_s_type *http_header_ptr = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  PS_APP_SPRINTF_DEBUG_1("ProcessCertGet for URI: %s",cert_uri );

  /*--------------------------------------------------------------------- 
    Make http header for request
  ---------------------------------------------------------------------*/
  http_header_ptr = (ds_http_header_s_type*)
    ps_system_heap_mem_alloc(sizeof(ds_http_header_s_type));

  if (NULL == http_header_ptr)
  {
    PS_APP_ERR_LOG_0();
    return DS_HTTP_ERROR;
  }

  memset(http_header_ptr, 0, sizeof(ds_http_header_s_type));

  /*--------------------------------------------------------------------- 
    Set the value of user_agent inside the http header
  ---------------------------------------------------------------------*/
  strlcpy(http_header_ptr->user_agent,  
          "IPsec client ID /01.00", sizeof(http_header_ptr->user_agent));    

  /*--------------------------------------------------------------------- 
    Send the HTTP Get request
  ---------------------------------------------------------------------*/
  ps_cert_http_request_id = 
   ds_http_create_get_request(ps_cert_http_session_id, 
                              cert_uri, 
                              http_header_ptr, 
                              0, 
                              &request_errno);

  PS_APP_DEBUG_1("CertHttpUtils request_id 0x%x",ps_cert_http_request_id);

  /*--------------------------------------------------------------------- 
    Process error if HTTP create request fails
  ---------------------------------------------------------------------*/
  if( DS_HTTP_ERROR ==  ps_cert_http_request_id )
  {
    PS_APP_ERR_LOG_0();
  }

  /*--------------------------------------------------------------------- 
    Free memory, utility can now make additional HTTP request
  ---------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE( http_header_ptr );
  cert_uri = NULL;
  cert_uri_len = 0;
  return ps_cert_http_request_id;
} /* CertHttpUtils::ProcessHttpGet */

/**-------------------------------------------------------------------------- 
*   @fn  ProcessHttpGet()
*   @brief process HTTP get and makes get request
---------------------------------------------------------------------------*/
uint32 CertHttpUtils::ProcessHttpPost
(
  uint8* http_post_content,
  uint16 http_post_content_len,
  ds_http_header_s_type *http_header_request
)
{
  sint15 request_errno = 0;
  ds_http_content_info_s_type post_content;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  PS_APP_SPRINTF_DEBUG_1("ProcessCertPost for URI: %s",cert_uri );

  /*--------------------------------------------------------------------- 
    Send the HTTP Get request
  ---------------------------------------------------------------------*/
  post_content.content = (const uint8*)http_post_content;
  post_content.content_size = http_post_content_len; 

  ps_cert_http_request_id = 
   ds_http_create_post_request(ps_cert_http_session_id, 
                              cert_uri, 
                              http_header_request,
                              &post_content, 
                              0, 
                              &request_errno);

  PS_APP_DEBUG_2("CertHttpUtils request_id 0x%x errorno %d",ps_cert_http_request_id, request_errno);

  /*--------------------------------------------------------------------- 
    Process error if HTTP create request fails
  ---------------------------------------------------------------------*/
  if( DS_HTTP_ERROR ==  ps_cert_http_request_id )
  {
    PS_APP_ERR_LOG_0();
  }

  cert_uri = NULL;
  cert_uri_len = 0;
  return ps_cert_http_request_id;
} /* CertHttpUtils::ProcessHttpGet */

