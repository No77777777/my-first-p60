#ifndef PS_CERT_HTTP_H
#define PS_CERT_HTTP_H

/*=========================================================================*/
/*!
@file
ps_cert_http.h

@brief
Application that provides HTTP service to certificate utility 
 
Copyright (c) 2015-2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_cert_http.h#3 $
$DateTime: 2024/11/06 08:53:47 $$Author: pwbldsvc $ 
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/16/19    ak     Fix to set content-type in http post
02/06/18    fn     FR 29585 Added support for http post
04/10/15    fn     Created Module
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"
#include "ps_cert_utils.h"

/*===========================================================================

CLASS DEFINITION

===========================================================================*/
namespace PS_CERT
{
  typedef void (*ps_cert_http_cback_type)
  (
    uint32  session_id,
    uint32 request_id,
    const uint8* httpdata,
    uint32 httpdata_len,
    sint15 err_code
  );
  /**-------------------------------------------------------------------------- 
    *    @class CertHttpUtils
    *    @brief HTTP utility for fetching over WLAN
  ---------------------------------------------------------------------------*/
  class CertHttpUtils
  {
  public:

    /**=======================================================================
      @fn CertHttpUtils()
      @brief class constructor
      @param req_cback
             session id
      @retval instance to CertHTTPUtil
    =======================================================================*/
    CertHttpUtils( ps_cert_http_cback_type req_cback,
                   uint32* session_id,
                   ps_cert_task_swtch_fptr task_cback,
                   ds_http_iface_info_s_type *iface_info
                   );

    /**=======================================================================
      @fn ~CertHttpUtils()
      @brief Default destructor.
      @param none
      @retval none
    =======================================================================*/
    ~CertHttpUtils();

    /**=======================================================================
      @fn HttpGet()
      @brief Retrieves HTTP content from given url
      @param uint8* uri contains url string
             uint16 uri_len contains len of url string 
      @retval request_id
    =======================================================================*/
    uint32 HttpGet(uint8* uri, uint16 uri_len);
    uint32 HttpPost(const char* uri, uint16 uri_len, uint8* http_post_content, 
      uint16 http_post_content_len, ds_http_header_s_type *http_header_request);
  protected:
    /**=======================================================================
      @fn  ProcessHttpGet()
      @brief process HTTP get and makes get request 
      @retval request_id
    =======================================================================*/
    uint32 ProcessHttpGet();
    uint32 ProcessHttpPost(uint8* http_post_content,uint16 http_post_content_len,
      ds_http_header_s_type *http_header_request);

  private:
    /** @var  uint32 ps_cert_http_session_id
    *   @brief  stores HTTP session id
    */
    uint32 ps_cert_http_session_id;

    /** @var  uint32 ps_cert_http_request_id
    *   @brief  stores HTTP request id
    */
    uint32 ps_cert_http_request_id;

    /** @var  char* cert_uri
    *   @brief  pointer to URL string
    */
    char* cert_uri;

    /** @var  uint16 cert_uri_len
    *   @brief  length of URL string
    */
    uint16 cert_uri_len;

    /** @var  ps_cert_task_swtch_fptr task_cback
    *   @brief  task callback function to run cert util in client
    *           task context
    */
    ps_cert_task_swtch_fptr task_cback;
  };// CertHttp

  void ps_cert_http_error_cb_fcn
  (
    uint32                         session_id,
    uint32                         request_id,
    sint15                         error
  );

} //PS_CERT

#endif // PS_CERT_HTTP_H


