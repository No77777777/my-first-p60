#ifndef PS_CERT_UTILS_H
#define PS_CERT_UTILS_H

/*===========================================================================
                           PS_CERT_UTILS_H
===========================================================================*/

/*!
  @file
  ps_cert_utils.h

  @brief
  This module includes utility function for certificate fetching

  @detail  
*/

/*===========================================================================

  Copyright (c) 2015-2024 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/06/24    ak     Fix to set content-type in http post
02/06/18    fn     FR 29585 Added support for http post
04/10/15    fn     Initiali version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "ds_http_types.h"
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/
/** @addtogroup constants 
  @{
*/

/** Invalid certificate handle value.
*/
#define PS_CERT_HANDLE_INVALID -1

/** Cert utility error codes
*/
#define PS_CERT_NO_ERR            0  /* Indicates no error.                */
#define PS_CERT_HTTP_DNS_ERR     -1  /* Indicates DNS error in HTTP        */
#define PS_CERT_HTTP_TOUT_ERR    -2  /* Indicates timeout error            */
#define PS_CERT_HTTP_STATUS_ERR  -3  /* Indicates status error during HTTP */
#define PS_CERT_HTTP_ACCESS_ERR  -4  /* Indicates access error during HTTP */
#define PS_CERT_HTTP_GEN_ERR     -5  /* Indicates generic HTTP error       */

/** @addtogroup datatypes
  @{
*/
/** Handle to the certificate utility instance.
*/
typedef uint32 ps_cert_handle_type;

/** structure to hold certificates and bundles
 */
typedef struct ps_cert_container_type
{
  const uint8* certdata;
  uint32 certlen;
  struct ps_cert_container_type* next_cert;
}ps_cert_container_type;

/** client callback type for http url get
*/
typedef void (*ps_cert_url_cback_type)
(
  ps_cert_handle_type cert_handle,
  ps_cert_container_type* certs,
  uint16 num_certs,
  const char* url,
  void * userdata,
  sint15 err_code
);

/** certificate cback from certificate utility a
 *  fter task switch
*/
typedef void (*ps_cert_task_swtch_cback_type)
(
  void *userdata
);

/** client task switch function
*/
typedef boolean (*ps_cert_task_swtch_fptr)
(
  ps_cert_task_swtch_cback_type cert_cback,
  void *userdata
);

/*===========================================================================
                         EXTERNAL FUNCTION DEFINTIONS
===========================================================================*/

/*===========================================================================
FUNCTION PS_CERT_FETCH_URI
===========================================================================*/
/** @ingroup ps_cert_fetch_uri
  Fetches certificate URI given an url and returns a callback 

  @datatypes
  #uint8*\n
  #uint16*\n
  #ps_cert_url_cback_type\n
  #void*

  @vertspace
  @param[in] uri     Contain the uri of the certificate
  @param[in] uri_len Indicates len of uri
  @param[in] ps_cert_url_cback Callback for notifying client
  @param[in] userdata pass back to client as part of callback

  @return 
  ps_cert_handle_type. Any value greater than PS_CERT_HANDLE_INVALID means
  the initialization of the fetch is successful
	
  @dependencies
  None
  @newpage
*/
ps_cert_handle_type ps_cert_fetch_uri
( 
  uint8 * uri, 
  uint16 uri_len,
  ps_cert_url_cback_type ps_cert_url_cback,
  ps_cert_task_swtch_fptr task_cback,
  uint32 http_timeout,
  void* userdata,
  ds_http_iface_info_s_type *iface_info
);

/*===========================================================================
FUNCTION PS_CERT_FETCH_POST
===========================================================================*/
/** @ingroup ps_cert_fetch_post
  Fetches HTTP POST given an url with POST rquest contents and 
              returns a callback with the HTTP POST response
  @datatypes
  #uint8*\n
  #uint16*\n
  #uint8*\n
  #uint32\n 
  #ps_cert_url_cback_type\n
  #void*

  @vertspace
  @param[in] uri     Contain the uri to http post server
  @param[in] uri_len Indicates len of uri
  @param[in] post_request Contain data for post request
  @param[in] post_request_len Indicates length of post request data
  @param[in] http_header_request Indicates http header request data
  @param[in] ps_cert_url_cback Callback for notifying client
  @param[in] userdata pass back to client as part of callback

  @return 
  ps_cert_handle_type. Any value greater than PS_CERT_HANDLE_INVALID means
  the initialization of the fetch is successful
	
  @dependencies
  None
  @newpage
*/
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
);

/*===========================================================================
FUNCTION PS_CERT_ABORT_FETCH
===========================================================================*/
/** @ingroup ps_cert_abort_fetch
  Aborts a fetch to avoid callback

  @datatypes
  #ps_cert_handle_type

  @vertspace
  @param[in] cert_handle Contain a handle to cert utility

  @return 
  None
	
  @dependencies
  None

  @newpage
*/
void ps_cert_abort_fetch
(
  ps_cert_handle_type cert_handle
);

#ifdef __cplusplus
}
#endif

#endif /* PS_CERT_UTILS_H */
