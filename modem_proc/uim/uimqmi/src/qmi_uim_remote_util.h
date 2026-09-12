#ifndef QMI_UIM_REMOTE_UTIL_H
#define QMI_UIM_REMOTE_UTIL_H

/*===========================================================================

                         Q M I _ U I M _ R E M O T E _ U T I L . H

DESCRIPTION

 This header file defines data types and functions used internally 
 by QMI UIM Remote service

                        COPYRIGHT INFORMATION

Copyright (c) 2015 - 2016 QUALCOMM Technologies, Incorporated and its licensors.
All Rights Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_remote_util.h#2 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/16/16    bcho   Deregister qmi uim remote service on task stop
03/20/15    ks     Intial Version

===========================================================================*/

/*=============================================================================

                   I N C L U D E S

=============================================================================*/
#include "comdef.h"
#include "qmi_csi.h"
#include "qmi_sap.h"
#include "queue.h"
#include "uim_common_efs.h"
#include "qmi_uim_remote.h"
/*=============================================================================

                   D E F I N E S

=============================================================================*/
#define QMI_UIM_REMOTE_MAX_NUM_CARD_CLIENT       5
#define QMI_UIM_REMOTE_MAX_NUM_CARD_SERVICE      5

/*=============================================================================

                   E N U M E R A T E D   D A T A

=============================================================================*/

/*===========================================================================
   ENUM:	  QMI_UIM_REMOTE_CMD_ENUM_TYPE
=============================================================================*/
typedef enum
{
  QMI_UIM_REMOTE_CMD_REGISTER		  = 0x0,
  QMI_UIM_REMOTE_CMD_RELEASE,
  QMI_UIM_REMOTE_CMD_CARD_CONNECT,
  QMI_UIM_REMOTE_CMD_CARD_DISCONNECT,
  QMI_UIM_REMOTE_CMD_SEND_APDU,
  QMI_UIM_REMOTE_CMD_CARD_POWER_DOWN,
  QMI_UIM_REMOTE_CMD_CARD_POWER_UP,
  QMI_UIM_REMOTE_CMD_CARD_RESET,
  QMI_UIM_REMOTE_CMD_NOT_APPLICABLE   = 0x7FFFFFFF
} qmi_uim_remote_cmd_enum_type;


/*=============================================================================

      S T R U C T    A N D    C A L L B A C K   D A T A    T Y P E S

=============================================================================*/
/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_CMD_TYPE
=============================================================================*/
typedef struct {
  q_link_type                     link;
  qmi_uim_remote_slot_type        slot;
  qmi_uim_remote_cmd_enum_type    cmd_enum;
  union
  {
    struct
    {
      qmi_uim_remote_evt_callback_type       * event_cb_ptr;
    } reg_req;
    struct
    {
      void                                   * client_ref_ptr;
      uint16                                   cmd_apdu_len;
      uint8                                  * cmd_apdu_ptr;
      qmi_uim_remote_apdu_callback_type      * rsp_apdu_cb_ptr;
    } send_apdu_req;
    struct
    {
      qmi_uim_remote_power_down_mode_enum_type pdown_mode;
    } power_down_req;
    struct
    {
      boolean                                  timeout_isvalid;
      uint32                                   timeout;
      boolean                                  vcc_isvalid;
      qmi_uim_remote_voltage_class_enum_type   vcc;
    } power_up_req;
  }cmd_data;
}qmi_uim_remote_cmd_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_LAST_APDU_INFO_TYPE
=============================================================================*/
typedef struct
{
  uint8                                id;
  qmi_uim_remote_apdu_callback_type  * rsp_apdu_cb_ptr;
  qmi_uim_remote_rsp_apdu_data_type    rsp_apdu_data;
} qmi_uim_remote_curr_apdu_info_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_CARD_CLIENT_INFO_TYPE
=============================================================================*/
typedef struct
{
  boolean                             in_use;
  qmi_client_handle                   client_handle;
  qmi_uim_remote_slot_type            slot;
  qmi_uim_remote_event_data_type      last_evt_data;
  qmi_uim_remote_curr_apdu_info_type  curr_apdu;
} qmi_uim_remote_card_client_info_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_CARD_SERVICE_INFO_TYPE
=============================================================================*/
typedef struct
{
  boolean                               in_use;
  qmi_uim_remote_evt_callback_type    * event_cb_ptr;
  qmi_uim_remote_slot_type              slot;
} qmi_uim_remote_card_service_info_type;

/*===========================================================================
   STRUCTURE:      QMI_UIM_REMOTE_STATE_TYPE
=============================================================================*/
typedef struct {
  qmi_csi_service_handle                   service_handle;
  qmi_sap_client_handle                    sap_handle;
  q_type                                   cmd_q;
  boolean                                  service_enable;
  qmi_uim_remote_card_client_info_type     card_client_registry[QMI_UIM_REMOTE_MAX_NUM_CARD_CLIENT];
  qmi_uim_remote_card_service_info_type    card_service_registry[QMI_UIM_REMOTE_MAX_NUM_CARD_SERVICE];
}qmi_uim_remote_state_type;

/*=============================================================================

                   F U N C T I O N S

=============================================================================*/

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_GLOBALS_INIT

   DESCRIPTION:
     This function initializes the globals associated with QMI UIM REMOTE

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
void qmi_uim_remote_globals_init (
  void
);

/*===========================================================================
  FUNCTION QMI_UIM_REMOTE_SERVICE_INIT

  DESCRIPTION
    Called to start the QMI UIM Remote service when the QMI modem task is up

  PARAMETERS
    TCB and SIG

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_remote_service_init(
  rex_tcb_type    * tcb_ptr,
  rex_sigs_type     sig
);

/*===========================================================================
   FUNCTION:      QMI_UIM_REMOTE_CMD_Q_SIG_HANDLER

  DESCRIPTION:
    QMI UIM REMOTE CMD Q signal handler

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     Returns TRUE if command queue is empty
     FALSE if there are more commands to process

   SIDE EFFECTS:
     None
===========================================================================*/
boolean qmi_uim_remote_cmd_q_sig_handler (
  void
);

/*===========================================================================
  FUNCTION QMI_UIM_REMOTE_HANDLE_QCSI_SIG

  DESCRIPTION
    Function is called when a QCSI signal is received by the QMI UIM REMOTE
    service. It will result in the calling of one of the QMI UIM REMOTE
    request handlers.

  PARAMETERS
    tcb
    sig

  RETURN VALUE
    int

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_remote_handle_qcsi_sig (
   rex_tcb_type *tcb, rex_sigs_type sig
);

/*=========================================================================== 
  FUNCTION QMI_UIM_REMOTE_DEREG 

  DESCRIPTION 
    Called to deregister the QMI UIM Remote as QMI service and as SAP client on 
    reception of task stop signal 

  PARAMETERS 
    None 

  RETURN VALUE 
    None 

  DEPENDENCIES 
    None 

  SIDE EFFECTS 
    None 
===========================================================================*/ 
void qmi_uim_remote_dereg( 
  void 
 ); 

#endif /* QMI_UIM_REMOTE_UTIL_H */