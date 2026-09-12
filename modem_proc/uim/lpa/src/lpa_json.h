/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  U I M   JSON encoder and decoder HEADER

GENERAL DESCRIPTION
  This header defines the external interface for the uim_rxstates module.

EXTERNALIZED FUNCTIONS

  uimdrv_rx_event_cb
    Called from uim_rxtx when an rx interrupt happens to execute the RX
    state machine.

Copyright (c) 2016, 2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_json.h#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/29/18    bcho   LPA phase2.1 changes
12/07/16    av     LPA phase2 initial changes
04/11/16    ll     Initial version

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ds_json_ext_i.h"

#include <stringl/stringl.h>
#include "string.h"
#include <ctype.h>

#include "lpa_lpd.h"
#include "lpa_http.h"
#include "lpalib.h"

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_JSON_INITIATE_AUTH_DECODER

DESCRIPTION
   This function parse the ES9+.InitiateAuthentication response JSON string.
   Then populate the json_initiate_auth_ptr passed in from LPA caller

DEPENDENCIES
  Caller need to free all inter pointers in json_initiate_auth_ptr

LIMITATIONS
  None

RETURN VALUE
  Parsing status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_initiate_auth_decoder (
  lpa_data_type*                   json_data_ptr,
  lpa_lpd_initiate_auth_resp_type* json_initiate_auth_ptr,
  lpa_slot_id_enum_type            slot_id
);

/*===========================================================================
FUNCTION LPA_JSON_GET_BOUND_PROFILE_PACKAGE_DECODER

DESCRIPTION
  This function parse the ES9+.GetBoundProfilePackage response JSON string.
  Then populate the json_initiate_auth_ptr passed in from LPA caller

DEPENDENCIES
  Caller need to free all inter pointers in json_initiate_auth_ptr

LIMITATIONS
  Parsing status

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_get_bound_profile_package_decoder (
  lpa_data_type*             json_data_ptr,
  lpa_lpd_get_bpp_resp_type* json_get_bound_profile_package_ptr
);

/*===========================================================================
FUNCTION LPA_JSON_AUTHENTICATE_CLIENT_DECODER

DESCRIPTION
  This function parse the ES9+.AuthenticateClient response JSON string.
  Then populate the lpa_lpd_authenticate_client_resp_type passed in from
  LPA caller

DEPENDENCIES
  Caller need to free all inter pointers in json_authenticate_client_resp_ptr

LIMITATIONS
  Parsing status

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_authenticate_client_decoder (
  lpa_data_type*                         json_data_ptr,
  lpa_lpd_authenticate_client_resp_type* json_authenticate_client_resp_ptr,
  lpa_slot_id_enum_type                  slot_id
);

/*===========================================================================
FUNCTION LPA_JSON_INITIATE_AUTH_ENCODER

DESCRIPTION
  This function encode the ES9+.InitiateAuthentication object to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_initiate_auth_encoder (
  lpa_data_type*                  json_data_ptr,
  lpa_http_initiate_auth_req_type* json_initiate_auth_ptr
);

/*===========================================================================
FUNCTION LPA_JSON_GET_BOUND_PROFILE_PACKAGE_ENCODER

DESCRIPTION
  This function encode the ES9+.GetBoundProfilePackage object to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_get_bound_profile_package_encoder (
  lpa_data_type*            json_data_ptr,
  lpa_http_get_bpp_req_type* json_get_bpp_ptr
);

/*===========================================================================
FUNCTION LPA_JSON_HANDLE_PROFILE_INSTALL_RESULT_ENCODER

DESCRIPTION
  This function encode the ES9+.HandleProfileInstallationResult object to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_handle_profile_install_result_encoder (
  lpa_data_type*                               json_data_ptr,
  lpa_http_handle_installation_result_req_type* json_handle_profile_install_result_ptr
);

/*===========================================================================
FUNCTION LPA_JSON_AUTHENTICATE_CLIENT_ENCODER

DESCRIPTION
  This function encode the ES9+.AuthenticateClient object to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_authenticate_client_encoder (
  lpa_data_type*                               json_data_ptr,
  lpa_http_authenticate_client_req_type*        json_authenticate_client_req_ptr
);

/*===========================================================================
FUNCTION LPA_JSON_HANDLE_NOTIFICATION_ENCODER

DESCRIPTION
  This function encode the ES9+.HandleNotification object to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_handle_notification_encoder (
  lpa_data_type*                               json_data_ptr,
  lpa_http_handle_notification_req_type*       json_handle_notification_req_ptr
);

/*===========================================================================
FUNCTION LPA_JSON_CANCEL_SESSION_ENCODER

DESCRIPTION
  This function encode the ES9+.CancelSession object to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_cancel_session_encoder (
  lpa_data_type*                               json_data_ptr,
  lpa_http_cancel_session_server_req_type*     json_cancel_session_server_req_ptr
);



