#ifndef UIM_REMOTE_H
#define UIM_REMOTE_H

/*===========================================================================

                         U I M _ R E M O T E . H

DESCRIPTION

 This header file defines data types and functions necessary to access a SIM
 attached to a remote device such as an applications processor or another modem
 via the QMI interface.

Copyright (c) 2014 - 2017 by QUALCOMM Technologies, Inc (QTI) and its licensors.All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_remote.h#2 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/13/17   ku      Common path for local and remote card
04/20/16   sam     Support for enhanced subscription manager
12/16/15   ssr     Delay local card power up in Modem SSR in BTSAP mode
09/30/15   lm      Fix compiler warning
08/04/15   ks      Removing the un-necessary extern declarations
07/03/15   ks      Adding report call bcak for remote/local cards power-up
04/15/15   ks      De-Featurizing the FEATURE_UIM_REMOTE/BTSAP_UIM
04/06/15    ll     UIMDRV migrates to UIM COMMON EFS APIs for all EFS items
02/03/14   sam     Usage of newer style diag macros
12/12/13   ks      BTSAP in client mode, intial implementation
===========================================================================*/

/*=============================================================================

                   I N C L U D E S

=============================================================================*/

#include "uimi.h"
#include "uimgen.h"
#include "uimglobals.h"
#include "qmi_uim_remote.h"
#include "uimdrv_msg.h"
#include "uim_p.h"


#define UIM_REMOTE_APDU_HDR_SIZE                    5
#define UIM_REMOTE_SW1_SW2_LENGTH                   2
#define UIM_REMOTE_MAX_RAPDU_LENGTH                 256

/* Masks in BTSAP handle for various indications */
/* Indicates if UIM received connection request from QMI REMOTE client */
#define UIM_BTSAP_ACTIVE_ENABLE_MASK   0x01
#define UIM_BTSAP_ACTIVE_DISABLE_MASK  0xFE

/* Indicates if connection with remote card is established */
#define UIM_BTSAP_CARD_CONNCECTTION_ENABLE_MASK   0x02
#define UIM_BTSAP_CARD_CONNCECTTION_DISABLE_MASK  0xFD

/* Indicates if recovery needs to be triggered */
#define UIM_BTSAP_TRIGGER_RECOVERY_MASK  0x04
#define UIM_BTSAP_DISABLE_RECOVERY_MASK  0xFB


/* Indicates if connection unavailable is requested with remote card */
#define UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK  0x08
#define UIM_BTSAP_CARD_CONN_UNAVAIL_DISABLE_MASK  0xF7

/* BTSAP Transaction timer default value - 7000ms */
#define UIM_BTSAP_TRANSACTION_TIME_DURATION 7000

/* Macro to check if UIM is in BTSAP client mode */
#define IS_BTSAP_HANDLE_ACTIVE(uim_ptr) (uim_ptr->remote.btsap.handle & UIM_BTSAP_ACTIVE_ENABLE_MASK)

/* Macro to return for remote card */
#define UIM_RETURN_IF_REMOTE_CARD(uim_ptr)                                \
  if(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))                                     \
  {                                                                       \
    return;                                                               \
  }                                                                       \


/* Macro for number of entries in uim_sw_data_table */
/* Note : Macro needs to be updated if an entry is added or removed from uim_sw_data_table */


/*===========================================================================

FUNCTION UIM_REMOTE_COMMAND_RESPONSE_CALLBACK

DESCRIPTION
  This procedure is the call back function provided in the command to the
  driver and upon calling this it initiates the processing of the response
  that has been received from the UIM.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets the UIM_CMD_RSP_SIG.

===========================================================================*/

void uim_remote_command_response_callback
(
  uim_instance_global_type *uim_ptr
);


extern void uim_remote_card_connect
(
  uim_instance_global_type *uim_ptr
);

extern void uim_remote_card_power_up
(
  uim_instance_global_type *uim_ptr
);

extern void uim_remote_card_power_down
(
  uim_instance_global_type *uim_ptr
);

extern void uim_remote_card_reset
(
  uim_instance_global_type *uim_ptr
);

extern void uim_remote_send_command
(
  uim_req_buf_type *uim_req_ptr , uim_instance_global_type *uim_ptr
);

extern void uim_remote_send_apdu
(
  uim_req_buf_type *uim_req_ptr , uim_instance_global_type *uim_ptr
);

extern void uim_remote_cmd_rsp_timer_expiry_cb
(
  timer_cb_data_type param
);

extern void uim_remote_send_atr
(
  uim_instance_global_type *uim_ptr,
  const qmi_uim_remote_card_atr_data_type *atr_data_ptr
);


extern void uim_remote_get_command_status
(
   uim_instance_global_type *uim_ptr
);

extern void uim_btsap_init
(
  uim_instance_global_type *uim_ptr
);

void  uim_remote_get_entire_response
(
  uim_instance_global_type *uim_ptr
);


extern void uim_btsap_release_connection_req 
(
  uim_instance_global_type *uim_ptr
);

extern void uim_btsap_save_current_mode_to_efs 
(
  uim_instance_global_type *uim_ptr ,  
  uim_slot_connection_enum_type uim_mode
);

#endif /* UIM_REMOTE_H */
