/*!
  @file
    sar_dsi_ext.c

  @brief
        Initializes the specific_absorption_rate service to clients for
        getting the device state index (DSI).
*/

/*===========================================================================

  Copyright (c) 2019 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/
/*==========================================================================

$Header: //components/rel/mcs.mpss/5.2/limitsmgr/sar/src/sar_dsi_ext.c#1 $

===========================================================================*/
/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
#include <qmi_csi.h>
#include "specific_absorption_rate_v01.h"
/*=============================================================================

                       TYEPEDEFS

=============================================================================*/

/*=============================================================================

                        FORWARD DECLARATIONS

=============================================================================*/
extern qmi_csi_cb_error sar_handle_rf_set_state_req (
  void          *connection_handle,
  qmi_req_handle req_handle,
  unsigned int   msg_id,
  void          *req_c_struct,
  unsigned int   req_c_struct_len,
  void          *service_cookie
);

extern qmi_csi_cb_error sar_handle_rf_get_state_req (
  void          *connection_handle,
  qmi_req_handle req_handle,
  unsigned int   msg_id,
  void          *req_c_struct,
  unsigned int   req_c_struct_len,
  void          *service_cookie
);

extern qmi_csi_cb_error sar_handle_rf_get_comp_key_req (
  void          *connection_handle,
  qmi_req_handle req_handle,
  unsigned int   msg_id,
  void          *req_c_struct,
  unsigned int   req_c_struct_len,
  void          *service_cookie
);

extern qmi_csi_cb_error sar_handle_invalid_msg (
  void          *connection_handle,
  qmi_req_handle req_handle,
  unsigned int   msg_id,
  void          *req_c_struct,
  unsigned int   req_c_struct_len,
  void          *service_cookie
);

/*=============================================================================

                         INTERNAL VARIABLES

=============================================================================*/

/*=============================================================================

                           FUNCTIONS DEFINITIONS

=============================================================================*/
/*=============================================================================

  FUNCTION:  sar_handle_rf_set_state_req_ext

=============================================================================*/
/*!
    @brief
    Function to handle & respond to QMI_SAR_RF_SET_STATE_REQ_MSG_V01 message

    Any customer specific QMI message handler should be implemented with this
    signature and may be invoked as a replacement to the default handler.

    @return
    qmi_csi_cb_error
*/
/*===========================================================================*/
qmi_csi_cb_error sar_handle_rf_set_state_req_ext (
  void          *connection_handle,
  qmi_req_handle req_handle,
  unsigned int   msg_id,
  void          *req_c_struct,
  unsigned int   req_c_struct_len,
  void          *service_cookie
)
{
  /* Placeholder for replacement customer code for sar_handle_rf_set_state_req()*/
  return QMI_CSI_CB_NO_ERR;
}

/*=============================================================================

  FUNCTION:  sar_dsi_process_req

=============================================================================*/
/*!
    @brief
        Callback function called by QCSI infrastructure when a REQ message to
        SAR DSI is received

    @note
      QCSI infrastructure decodes the data before forwarding it to this layer

    @return
     qmi_csi_cb_error
*/
/*===========================================================================*/
qmi_csi_cb_error sar_dsi_process_req (
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  qmi_csi_cb_error                          req_cb_retval    = QMI_CSI_CB_NO_ERR;
  qmi_response_type_v01                     response;

  /*-----------------------------------------------------------------------*/

  /* Process message appropriately based on QMI msg_id */
  switch( msg_id )
  {
    /* This request provides the current DSI */
    case QMI_SAR_RF_SET_STATE_REQ_MSG_V01:
    {
      req_cb_retval = sar_handle_rf_set_state_req(
                        connection_handle,
                        req_handle,
                        msg_id,
                        req_c_struct,
                        req_c_struct_len,
                        service_cookie
                      );
      /*
      req_cb_retval = sar_handle_rf_set_state_req_ext(
                        connection_handle,
                        req_handle,
                        msg_id,
                        req_c_struct,
                        req_c_struct_len,
                        service_cookie
                      );
      */
      break;
    }

    /* This request is used to query sar_dsi for the current DSI */
    case QMI_SAR_RF_GET_STATE_REQ_MSG_V01:
    {
       req_cb_retval = sar_handle_rf_get_state_req(
                        connection_handle,
                        req_handle,
                        msg_id,
                        req_c_struct,
                        req_c_struct_len,
                        service_cookie
                      );
       break;
    }

     /* This request is used by the client to get the current key */
    case QMI_SAR_GET_COMPATIBILITY_KEY_REQ_MSG_V01:
      req_cb_retval = sar_handle_rf_get_comp_key_req(
                        connection_handle,
                        req_handle,
                        msg_id,
                        req_c_struct,
                        req_c_struct_len,
                        service_cookie
                      );
      break;

    /* error case */
    default:
     req_cb_retval = sar_handle_invalid_msg(
                        connection_handle,
                        req_handle,
                        msg_id,
                        req_c_struct,
                        req_c_struct_len,
                        service_cookie
                      );
    break;
  } /* switch ( msg_id ) */

  /*-----------------------------------------------------------------------*/

  return req_cb_retval;
}

