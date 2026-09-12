#ifndef LPA_HTTP_H
#define LPA_HTTP_H
/*===========================================================================


            L P A   H T T P   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_http.h#2 $ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/07/16   av      LPA phase2 initial changes
10/10/16   av      Add support for configuring HTTP stack via LPA EFS item
10/07/16   ks      Fix to support bigger profile (> 65535) download
10/06/16   av      Introduce DS http post retry timer logic
05/16/16   av      Route HTTP request to QMI HTTP
05/15/16   ll      HTTP Chunking
05/15/16   av      LPA should wait for DS session open cb before sending post
04/11/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "queue.h"
#include "time_types.h"
#include "ds_http_types.h"

#include "lpalib.h"
#include "lpa.h"
#include "lpa_util.h"
#include "lpa_notification.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define LPA_HTTP_HEADER_NAME_MAX_LEN     32
#define LPA_HTTP_HEADER_VALUE_MAX_LEN    256
#define LPA_SESSION_ID_NULL              0xFFFFFFFF
#define LPA_MAX_PROFILE_SIZE             0x100000  /*1 MB*/

typedef enum {
  LPA_HTTP_NONE_CMD,
  LPA_HTTP_INITIATE_AUTH_CMD,
  LPA_HTTP_GET_BPP_CMD,
  LPA_HTTP_HANDLE_INSTALLATION_RESULT_CMD,
  LPA_HTTP_AUTHENTICATE_CLIENT_CMD,
  LPA_HTTP_CANCEL_SESSION_SERVER_CMD,
  LPA_HTTP_HANDLE_NOTIFICATION_CMD,
} lpa_http_cmd_enum_type;

/* Enum used for capturing which stack to be used for HTTP traffic:
   modem or AP */
typedef enum {
  LPA_HTTP_USE_STACK_BASED_ON_CHIP_ID, /* Use CHIP_ID to determine which
                                          HTTP stack to use */
  LPA_HTTP_USE_MODEM_STACK,            /* Use HTTP stack on modem */
  LPA_HTTP_USE_AP_STACK                /* Use HTTP stack on AP */
} lpa_http_stack_usage_enum_type;

typedef enum {
  LPA_HTTP_RSP,
  LPA_HTTP_QMI_HTTP_RSP
} lpa_http_rsp_enum_type;


/* Custom headers that can be specified by the client */
typedef struct
{
  char name [LPA_HTTP_HEADER_NAME_MAX_LEN+1];
  char value[LPA_HTTP_HEADER_VALUE_MAX_LEN+1];
} lpa_http_response_header_type;

typedef struct {
   uint32                         session_id;
   uint32                         request_id;
   sint15                         error;
   uint16                         http_status;
   uint32                         content_size;
   uint8                        * content_ptr;
   uint32                         num_headers;
   lpa_http_response_header_type* header_info_ptr;
} lpa_http_response_data_type;

typedef struct {
   qmi_uim_http_service_registry_id_type service_id;
   qmi_uim_http_transaction_result_type  http_status;
   uint32                                content_size;
   uint8                               * content_ptr;
   uint16                                num_headers;
   lpa_http_response_header_type       * header_info_ptr;
} lpa_http_qmi_http_response_data_type;

typedef struct {
  uint16                                http_status;
  uint32                                data_len;
  uint8                                *data_ptr;
  lpa_slot_id_enum_type                 slot_id;
} lpa_http_resp_data_type;

/* ----------------------------------------------------------------------------
   DATA TYPE:      LPA_HTTP_CALLBACK_TYPE
-----------------------------------------------------------------------------*/
typedef void (*lpa_http_callback_type) (lpa_result_enum_type            status,
                                        const lpa_http_resp_data_type  *resp_ptr,
                                        const void                     *user_data_ptr);

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_HTTP_RSP_TYPE

   DESCRIPTION:
     Contains information for handling response of LPA HTTP commands
-------------------------------------------------------------------------------*/
typedef struct {
  q_link_type                            link;
  lpa_http_rsp_enum_type                 rsp_type;
  union {
    lpa_http_response_data_type          rsp_data;
    lpa_http_qmi_http_response_data_type qmi_http_rsp_data;
  } data;
} lpa_http_rsp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_HTTP_INITIATE_AUTH_REQ_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_slot_id_enum_type                  slot_id;
  uint8                                  euicc_challenge[LPA_EUICC_CHALLENGE_LEN];
  uint8                                  svn[LPA_SVN_LEN];
  lpa_data_type                          cert_info;
  lpa_data_type                          euicc_info1;
  lpa_smdp_address_type                  smdp_address;
  lpa_http_callback_type                 lpd_callback;
  void                                  *user_data_ptr;
} lpa_http_initiate_auth_req_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_HTTP_GET_BPP_REQ_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_slot_id_enum_type           slot_id;
  lpa_data_type                   transaction_id_data;
  lpa_data_type                   prepare_download_data;
  lpa_smdp_address_type           smdp_address;
  lpa_http_callback_type          lpd_callback;
  void                           *user_data_ptr;
} lpa_http_get_bpp_req_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_HTTP_HANDLE_INSTALLATION_RESULT_REQ_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_slot_id_enum_type           slot_id;
  lpa_data_type                   profile_installation_result;
  lpa_smdp_address_type           smdp_address;
  lpa_http_callback_type          lpd_callback;
  void                           *user_data_ptr;
} lpa_http_handle_installation_result_req_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_HTTP_AUTHENTICATE_CLIENT_REQ_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_slot_id_enum_type           slot_id;
  lpa_data_type                   transaction_id_data;
  lpa_data_type                   authenticate_server_response;
  lpa_smdp_address_type           smdp_address;
  lpa_http_callback_type          lpd_callback;
  void                           *user_data_ptr;
} lpa_http_authenticate_client_req_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_HTTP_HANDLE_NOTIFICATION_REQ_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_slot_id_enum_type           slot_id;
  lpa_notification_type           notification;
  lpa_http_callback_type          callback;
  void                           *user_data_ptr;
} lpa_http_handle_notification_req_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_HTTP_CANCEL_SESSION_SERVER_REQ_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_slot_id_enum_type           slot_id;
  lpa_data_type                   transaction_id;
  lpa_data_type                   cancel_session_euicc_resp;
  lpa_smdp_address_type           smdp_address;
  lpa_http_callback_type          lpd_callback;
  void                           *user_data_ptr;
} lpa_http_cancel_session_server_req_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_HTTP_HANDLE_CMD_SIG

DESCRIPTION
  This function, called in lpa_main, is called to process HTTP event

DEPENDENCIES
  LPA task must be finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_http_handle_cmd_sig (
  void
);

/*===========================================================================
   FUNCTION:      LPA_HTTP_INIT

   DESCRIPTION:
     LPA HTTP init function called during task init

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
void lpa_http_init (
  void
);

/*===========================================================================
   FUNCTION:      LPA_HTTP_CLEANUP

   DESCRIPTION:
     LPA HTTP cleanup function called to clean up refrerence data

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_http_cleanup(
  lpa_slot_id_enum_type             slot_id
);

/*===========================================================================
FUNCTION LPA_HTTP_BUILD_AND_SEND_INITIATE_AUTH

DESCRIPTION
  This function is used to send Initiate Auth request to SMDP

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_http_build_and_send_initiate_auth (
  lpa_http_initiate_auth_req_type                *initiate_auth_req_ptr
);

/*===========================================================================
FUNCTION LPA_HTTP_BUILP_AND_SEND_GET_BPP

DESCRIPTION
  This function is used to send Get Bound Profile Package request to SMDP

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_http_build_and_send_get_bpp (
  lpa_http_get_bpp_req_type                *get_bpp_req_ptr
);

/*===========================================================================
FUNCTION LPA_HTTP_BUILP_AND_SEND_HANDLE_INSTALLATION_RESULT

DESCRIPTION
  This function is used to send the Handle Installation Result to SMDP

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_http_build_and_send_handle_installation_result (
  lpa_http_handle_installation_result_req_type         *profile_installation_result_req_ptr
);

/*===========================================================================
FUNCTION LPA_HTTP_BUILP_AND_SEND_AUTHENTICATE_CLIENT

DESCRIPTION
  This function is used to send the Authenticate Client request to SMDP

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_http_build_and_send_authenticate_client (
  lpa_http_authenticate_client_req_type        *authenticate_client_req_ptr
);

/*===========================================================================
FUNCTION LPA_HTTP_BUILP_AND_SEND_CANCEL_SESSION

DESCRIPTION
  This function is used to send the Cancel Session request to SMDP

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_http_build_and_send_cancel_session (
  lpa_http_cancel_session_server_req_type        *cancel_session_server_req_ptr
);

/*===========================================================================
FUNCTION LPA_HTTP_BUILP_AND_SEND_HANDLE_NOTIFICATION

DESCRIPTION
  This function is used to send the Handle Notification request to SMDP

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_http_build_and_send_handle_notification (
  lpa_http_handle_notification_req_type        *send_notification_req_ptr
);

/*===========================================================================
FUNCTION LPA_TIMER_HANDLE_HTTP_RETRY_TIMER_EXPIRE_SIG

DESCRIPTION
  This function, called in lpa_task, is called to process http retry timer
  expiration signal for a given slot.

DEPENDENCIES
  lpa_task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_http_handle_http_retry_timer_expire_sig (
  lpa_slot_id_enum_type               slot_id
);
#endif /* LPA_HTTP_H */

