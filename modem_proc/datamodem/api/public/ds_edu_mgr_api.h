#ifndef DS_EDU_MGR_API_H
#define DS_EDU_MGR_API_H

/*===========================================================================

   D A T A   S E R V I C E S   EDU MANAGER   A P I   H E A D E R   F I L E

DESCRIPTION

 The Data Services EDU Manager Header File. Contains shared variables and enums,
 as well as declarations for functions.

Copyright (c) 1998-2014 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.

===========================================================================*/


/*=============================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
 
when       who                    what, where, why
--------   ---       ----------------------------------------------------------
9/10/14   youjunf                      Created
===========================================================================*/



/*===========================================================================
                      INCLUDE FILES
===========================================================================*/
#include "ds_http_types.h"

/*===========================================================================
                      PUBLIC DATA DECLARATIONS
===========================================================================*/
typedef enum 
{ 
  DS_EDU_MGR_NW_STATUS_CHANGE_IND,
  DS_EDU_MGR_PUSH_STARTED_IND,       /*UPD socket succ created, start listen*/
  DS_EDU_MGR_PUSH_IND,               /*PUSH is received from remote server*/
  DS_EDU_MGR_HTTP_RESULT_IND,        /*FETCH or POST result*/
  DS_EDU_MGR_USER_AUTH_REQUIRE_IND,  /*user side authentication needed during HTTP GET/POST*/
  DS_EDU_MGR_HTTPS_CERT_ALERT_IND,   /*HTTPS suspend due to error of authenticating side certificate*/
  DS_EDU_MGR_MAX                     /*Invalid event for boundry check*/
}ds_edu_mgr_event_e_type;


typedef enum 
{ 
  DS_EDU_MGR_NW_3GPP,
  DS_EDU_MGR_NW_WLAN               /*not support wlan now*/
}ds_edu_mgr_nw_e_type;

typedef enum 
{ 
  DS_EDU_MGR_PUSH_UPDATE,         /*PUSH message for UPDATE*/
  DS_EDU_MGR_PUSH_LOG_REQ,        /*PUSH message for LOG REQ*/
  DS_EDU_MGR_PUSH_MAX             /*Invalid PUSH type for boundry check*/
}ds_edu_mgr_push_e_type;


typedef enum 
{
  DS_EDU_MGR_ERR_SUCC,                        /*Operation succeeds*/
  DS_EDU_MGR_ERR_INVALID_PARAM,               /*Invalid paramter*/
  DS_EDU_MGR_ERR_HTTP_ERR,                    /*Local HTTP error*/
  DS_EDU_MGR_ERR_SIZE_EXCEEDS                 /*Size exceeds*/       
}ds_edu_mgr_err_e_type;


typedef enum
{ 
  DS_EDU_MGR_SSL_ABORT,                      /*To abort*/
  DS_EDU_MGR_SSL_CONTINUE                    /*To continue*/
}ds_edu_mgr_ssl_control_e_type;


typedef struct
{ 
  ds_edu_mgr_nw_e_type    nw_type;
  boolean                 is_up;
}ds_edu_mgr_nw_status_info_s_type;


typedef struct
{ 
  boolean is_succ; 
  uint16  port_num; 
}ds_edu_mgr_push_start_result_s_type;


typedef struct
{ 
  ds_edu_mgr_push_e_type push_type; 
  const uint8            *payload_ptr;      // pointer to push packet payload
  uint16                 payload_length;    // length of the payload
}ds_edu_mgr_push_info_s_type;


typedef struct 
{ 
  uint32                         request_id; 
  uint8                          result; 
  uint32                         num_headers; 
  const ds_http_response_header  *header_info_ptr; 
  const uint8                    *content_ptr; 
  uint32                         content_length; 
}ds_edu_mgr_http_result_s_type;


typedef struct 
{ 
  uint32        request_id; 
  const char    *realm_ptr; 
}ds_edu_mgr_user_auth_req_s_type;

typedef struct
{ 
  uint32                        request_id; 
  ds_http_ssl_cert_alert_e_type cert_alert;
}ds_edu_mgr_https_cert_alert_s_type;


typedef union
{ 
  ds_edu_mgr_nw_status_info_s_type    nw_status_info;      
  ds_edu_mgr_push_start_result_s_type push_listen_start_result;
  ds_edu_mgr_push_info_s_type         push_info; 
  ds_edu_mgr_http_result_s_type       http_result; 
  ds_edu_mgr_user_auth_req_s_type     user_auth_req_info;
  ds_edu_mgr_https_cert_alert_s_type  https_cert_alert_info; 
}ds_edu_mgr_event_u_type;



/*----------------------------------------------------------------------------------------------------------------------------------------
This is the callback function registered when calling ds_edu_mgr_register_event_cback. It is used to handle the events sent by EDU manager. 
 
PARAMETERS
  [IN]  event_name           Name of event
  [IN]  event_info_ptr       Event related information
  [IN]  user_data_ptr        Client's ser Data  
 
-----------------------------------------------------------------------------------------------------------------------------------------*/
typedef void (*ds_edu_mgr_event_cb_f_ptr_type)
(
  ds_edu_mgr_event_e_type event_name,
  ds_edu_mgr_event_u_type *event_info_ptr, 
  void                    *user_data_ptr
);



#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================
                      PUBLIC FUNCTION DECLARATIONS
===========================================================================*/


/*===========================================================================
FUNCTION ds_edu_mgr_register_event_cback

DESCRIPTION
  This function registers client’s callback function for EDU Manager events.

PARAMETERS 
 [IN]  edu_mgr_event_cb_ptr  -  Callback function when event is triggered
 [IN]  user_data_ptr         -  Pointer provided by the client to uniquely identify this action.
                                                                The same pointer is passed to the client's callback function. 
                                    A NULL pointer is acceptable.  

DEPENDENCIES   None
RETURN VALUE   None
SIDE EFFECTS   None.
===========================================================================*/
void ds_edu_mgr_register_event_cback 
(
  ds_edu_mgr_event_cb_f_ptr_type event_cb_f_ptr, 
  void                           *user_data_ptr
);






/*===========================================================================
FUNCTION ds_edu_mgr_start_push_listen

DESCRIPTION
  This function triggers EDU manager to create a UDP socket, and start listening on the port binded to this socket.
  Port number will be returned asynchronizedly in DS_EDU_MGR_PUSH_STARTED_IND, refer to ds_edu_mgr_push_start_result_s_type for detail

PARAMETERS     None
DEPENDENCIES   None
RETURN VALUE   None
SIDE EFFECTS   None.
===========================================================================*/
void ds_edu_mgr_start_push_listen(void);






/*===========================================================================
FUNCTION ds_edu_mgr_stop_push_listen

DESCRIPTION
  This function triggers EDU manager to stop listening on the UDP port, and destroy the associated socket.

PARAMETERS     None
DEPENDENCIES   None
RETURN VALUE   None
SIDE EFFECTS   None.
===========================================================================*/
void ds_edu_mgr_stop_push_listen(void);






/*===========================================================================
FUNCTION ds_edu_mgr_fetch_request

DESCRIPTION
  This function sends an HTTP GET to the specified remote server.

PARAMETERS   
  [IN]  url_ptr        - Remote server URL address ended with ‘\0’. url_ptr MUST start with “http://” or “https://”.
  [IN]  request_header - Used for the client to specify HTTP headers. Refer to ds_http_header_s_type
  [OUT] err_value_ptr  -- Pointer to the error value passed synchronizedly. Refer to ds_edu_mgr_err_e_type.
                                                                
DEPENDENCIES   None

RETURN VALUE   
  Returns a request ID that is accupied by EDU Manager corresponding to one client’s fetch/post request.
  

  SIDE EFFECTS   None.
===========================================================================*/
uint32 ds_edu_mgr_fetch_request 
(
  const char            *url_ptr, 
  ds_http_header_s_type *request_header,
  ds_edu_mgr_err_e_type *err_value_ptr 
);


/*===========================================================================
FUNCTION ds_edu_mgr_post_request

DESCRIPTION
  This function sends an HTTP POST to the specified remote server.
 
PARAMETERS   
  [IN]  url_ptr        - Remote server URL address ended with ‘\0’. url_ptr MUST start with “http://” or “https://
  [IN]  content_ptr    - Pointer to the content the client wants to send in HTTP POST.
  [IN]  content_size   - the size fo content the client wants to send
  [IN]  request_header - Used for the client to specify HTTP headers. Refer to ds_http_header_s_type
  [OUT] err_value_ptr  - Pointer to the error value passed synchronizedly. Refer to ds_edu_mgr_err_e_type.
                                                                
DEPENDENCIES   None

RETURN VALUE   
  Returns a request ID that is accupied by EDU Manager corresponding to one client’s fetch/post request.
  

  SIDE EFFECTS   None.
===========================================================================*/
uint32 ds_edu_mgr_post_request 
(
  const char            *url_ptr, 
  const uint8           *content_ptr, 
  uint32                content_size, 
  ds_http_header_s_type *request_header, 
  ds_edu_mgr_err_e_type *err_value_ptr
);


/*===========================================================================
FUNCTION ds_edu_mgr_set_http_auth_info

DESCRIPTION
  This function sets authentication information to EDU Manager.

PARAMETERS   
  [IN]  request_id   - Request ID corresponding to this request.
  [IN]  username_ptr - Pointer to the username
  [IN]  password_ptr - Pointer to the password 
                                                                
DEPENDENCIES   
  Call thins function when callback function is triggered with DS_EDU_MGR_USER_AUTH_REQUIRE_IND event

RETURN VALUE   
  This function returns the error value passed synchronizedly. Refer to 18 ds_edu_mgr_err_e_type.
  

  SIDE EFFECTS   None.
===========================================================================*/
ds_edu_mgr_err_e_type ds_edu_mgr_set_http_auth_info 
(
  uint32  request_id,
  char    *username_ptr,
  char    *password_ptr 
);



/*===========================================================================
FUNCTION ds_edu_mgr_set_http_auth_info

DESCRIPTION
  This function controls the EDU Manager for the action when an SSL certificate alert happens.

PARAMETERS   
  [IN]  request_id   - Request ID corresponding to this request.
  [IN]  user_control - User control option, abort or continue, Refer to ds_edu_mgr_ssl_control_e_type
                       

DEPENDENCIES   
  Call thins function when callback function is triggered with DS_EDU_MGR_HTTPS_CERT_ALERT_IND event

RETURN VALUE   
  This function returns the error value passed synchronizedly. Refer to ds_edu_mgr_err_e_type.
  

  SIDE EFFECTS   None.
===========================================================================*/
ds_edu_mgr_err_e_type ds_edu_mgr_https_user_control 
( 
  uint32                        request_id,
  ds_edu_mgr_ssl_control_e_type user_control
);


/*===========================================================================
FUNCTION DS_EDU_MGR_INIT

DESCRIPTION
  ds edu_mgr initialization, called in dstask initialization

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_edu_mgr_init(void); 


#ifdef __cplusplus
}
#endif
#endif /* DS_EDU_MGR_API_H */

