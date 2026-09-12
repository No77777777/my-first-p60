#ifndef SRCH_MSGR_H
#define SRCH_MSGR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
          S E A R C H E R  M E S S A G E  R O U T E R  M O D U L E

                         H E A D E R   F I L E

GENERAL DESCRIPTION
      This file contains external interfaces and declarations for the
      searcher message router module.

EXPORTED FUNCTIONS:
      srch_msgr_init
      srch_msgr_register_msg
      srch_msgr_deregister_msg
      srch_msgr_register_1xdemod_msgs
      srch_msgr_deregister_1xdemod_msgs
      srch_msgr_register_srch4_msgs
      srch_msgr_deregister_srch4_msgs
      srch_msgr_send_msg
      srch_msgr_process_rcv_msg
      srch_msgr_stop

INITIALIZATION AND SEQUENCING REQUIREMENTS
      Must call srch_msgr_init() before any other functions in this module
      are invoked.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Copyright (c) 2009 - 2016 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/inc/srch_msgr.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/02/17   pk/srk  FR 37336: Migration to MSGR VARIANT from Inst ID 
02/25/16   srk     Wait for Slew Time response from FW.
02/05/16   pk      1x2GTA Feature changes
10/02/15   srk     DRX SRCH response deadlock redesign and cleanup.
09/09/15   bb      Changes to clean up syncronous abort changes
09/04/15   bb      Changes to wait synchronously for SLAM RSP response from FW
02/05/15   srk     Added support for ASDIV priority change response from FW.
11/18/14   bb      Changes to wait synchronously for SLEW roll response from FW
                   during L21x scenarios
09/22/14   bb      Changes to wait syncronously for FW state config response
                   while disabling MDSP FW
01/08/14   ab      DDR power management Support / Feature Cleanup
05/15/13   ab      1X coex manager changes
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
08/18/11   adw     Mainline 1x message router support.
11/03/10   vks     Add support to wait on a abort response message.
07/13/10   vks     Use MSGR_ONEX_SRCH definition from onex_msgr.h
11/11/09   vks     Clean up on the previous changes
11/09/09   vks     Added Message Router and Q6 FW support under feature
                   FEATURE_1X_SUPPORTS_MSGR.
09/09/09   vks     Initial implementation

==========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Message Router Includes */
#include "msgr.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/



/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

FUNCTION       SRCH_MSGR_INIT

DESCRIPTION    This function initializes the receive queue and registers the
               1x client with the message router.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_init( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_REGISTER_MSG

DESCRIPTION    This function registers a message that 1x is interested in
               collecting from the message router.

DEPENDENCIES   1X client setup with the MSGR must have been completed.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_register_msg
(
  msgr_umid_type             umid              /* UMID of the message      */
);

/*===========================================================================

FUNCTION       SRCH_MSGR_REGISTER_MSG_VARIANT

DESCRIPTION    This function registers a message that 1x is interested in
               collecting from the message router.

DEPENDENCIES   1X client setup with the MSGR must have been completed.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_register_msg_variant
(
  msgr_umid_type             umid              /* UMID of the message      */
);

/*===========================================================================

FUNCTION       SRCH_MSGR_DEREGISTER_MSG

DESCRIPTION    This function deregisters a message that 1x is no longer
               interested in receving from the message router.

DEPENDENCIES   1X client setup with the MSGR must have been completed.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_deregister_msg
(
  msgr_umid_type             umid              /* UMID of the message      */
);

/*===========================================================================

FUNCTION       SRCH_MSGR_DEREGISTER_MSG_VARIANT

DESCRIPTION    This function deregisters a message that 1x is no longer
               interested in receving from the message router.

DEPENDENCIES   1X client setup with the MSGR must have been completed.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_deregister_msg_variant
(
  msgr_umid_type             umid              /* UMID of the message      */
);

/*===========================================================================

FUNCTION       SRCH_MSGR_REGISTER_1XDEMOD_MSGS

DESCRIPTION    This function registers the demod messages that 1x is
               interested in receiving from 1X DEMOD FW (cdmafw_msg.h).

DEPENDENCIES   Must be called after 1X DEMOD APP is successfully enabled.

RETURN VALUE   None.

SIDE EFFECTS   1X starts receiving the registered messages from 1X DEMOD FW.

===========================================================================*/
extern void srch_msgr_register_1xdemod_msgs( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_DEREGISTER_1XDEMOD_MSGS

DESCRIPTION    This function de-registers the demod messages that 1x is no
               longer interested in receiving from 1X DEMOD FW.

DEPENDENCIES   Preferable to call after the 1X DEMOD APP has been disabled.

RETURN VALUE   None.

SIDE EFFECTS   1X stops receiving the registered messages from 1X DEMOD FW.

===========================================================================*/
extern void srch_msgr_deregister_1xdemod_msgs( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_REGISTER_SRCH4_MSGS

DESCRIPTION    This function registers the srch4 messages that 1x is
               interested in receiving from SRCH4 (srchfw_msg.h).

DEPENDENCIES   Must be called after SRCH4 APP is successfully enabled.

RETURN VALUE   None.

SIDE EFFECTS   1X starts receiving the registered messages from SRCH4 FW.

===========================================================================*/
extern void srch_msgr_register_srch4_msgs( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_DEREGISTER_SRCH4_MSGS

DESCRIPTION    This function registers the srch4 messages that 1x is
               interested in receiving from SRCH4 (srchfw_msg.h).

DEPENDENCIES   Preferable to call after the SRCH4 APP has been disabled.

RETURN VALUE   None.

SIDE EFFECTS   1X stops receiving the registered messages from SRCH4 FW.

===========================================================================*/
extern void srch_msgr_deregister_srch4_msgs( void );

#ifdef FEATURE_MODEM_1X_IRAT_LTO1X
/*===========================================================================

FUNCTION       SRCH_MSGR_REGISTER_LTE_SRCH_MSGS

DESCRIPTION    This function registers the LTE ML1 messages that 1x is
               interested in receiving from LTE Searcher module (cdmalte_msg.h).

DEPENDENCIES   Must be called whenever mobile enters to NA state machine.

RETURN VALUE   None.

SIDE EFFECTS   1X starts receiving the registered messages from LTE ML1.

===========================================================================*/
extern void srch_msgr_register_lte_srch_msgs( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_DEREGISTER_LTE_SRCH_MSGS

DESCRIPTION    This function de registers from LTE ML1 messages that 1x is
               no longer interested in receiving from LTE (cdmalte_msg.h).

DEPENDENCIES   Preferable to call after mobile enters to acquisation state.

RETURN VALUE   None.

SIDE EFFECTS   1X stops receiving the registered messages from LTE Searcher

===========================================================================*/
extern void srch_msgr_deregister_lte_srch_msgs( void );
#endif /* FEATURE_MODEM_1X_IRAT_LTO1X */

#ifdef FEATURE_MODEM_1X_COEXISTENCE
/*===========================================================================

FUNCTION       SRCH_MSGR_REGISTER_MCS_COEX_MSGS

DESCRIPTION    This function registers the coex messages that 1x is
                         interested in receiving from mcs_coex_mgr

DEPENDENCIES  None

RETURN VALUE   None.

SIDE EFFECTS   1X starts receiving the registered messages from MCS COEX MGR.

===========================================================================*/
extern void srch_msgr_register_mcs_coex_msgs( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_DEREGISTER_MCS_COEX_MSGS

DESCRIPTION    This function deregisters the coex messages that 1x is
                         interested in receiving from mcs_coex_mgr

DEPENDENCIES   Call at the time of stack deactivation

RETURN VALUE   None.

SIDE EFFECTS   1X stops receiving the registered messages from MCS COEX MGR.

===========================================================================*/
extern void srch_msgr_deregister_mcs_coex_msgs( void );
#endif  /* FEATURE_MODEM_1X_COEXISTENCE */

/*===========================================================================

FUNCTION       SRCH_MSGR_SEND_MSG

DESCRIPTION    This function initializes the header and sends the message to
               the message router.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_send_msg
(
  msgr_hdr_struct_type*      msg_hdr_ptr,        /* Pointer to MSG Header  */
  msgr_umid_type             umid,               /* UMID of the message    */
  int32                      size                /* Size of the message    */
);

/*===========================================================================

FUNCTION       SRCH_MSGR_SEND_MSG_VARIANT

DESCRIPTION    This function initializes the header and sends the message to
               the message router.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_send_msg_variant
(
  msgr_hdr_struct_type*      msg_hdr_ptr,        /* Pointer to MSG Header  */
  msgr_umid_type             umid,               /* UMID of the message    */
  int32                      size                /* Size of the message    */
);

/*===========================================================================

FUNCTION       SRCH_MSGR_PROCESS_RCV_MSG

DESCRIPTION    This function processes the received message.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_process_rcv_msg( void );

#ifdef FEATURE_MODEM_1X_SRCH_ASD
/*===========================================================================

FUNCTION       SRCH_MSGR_SET_ASDIV_RSP_FLAG

DESCRIPTION    This function sets the flag to indicate that SRCH is
               waiting for ASDIV priority change response from FW.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_set_asdiv_rsp_flag( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_WAITING_FOR_ASDIV_RESPONSE_FROM_FW

DESCRIPTION    This function checks if SRCH is stuck waiting for
               ASDIV priority change response from FW.

DEPENDENCIES   None.

RETURN VALUE   TRUE  - If waiting for FW response.
               FALSE - Otherwise

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_msgr_waiting_for_asdiv_response_from_fw( void );
#endif /* FEATURE_MODEM_1X_SRCH_ASD */

/*===========================================================================

FUNCTION       SRCH_MSGR_WAIT_ON_ABORT_RSP

DESCRIPTION    This function waits until the abort response message is
               processed. While waiting, it processes any other messages
               that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_wait_on_abort_rsp( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_WAIT_ON_STATE_CFG_RSP

DESCRIPTION    This function waits until the abort response message is
               processed. While waiting, it processes any other messages
               that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_wait_on_state_cfg_rsp( void );

/*===========================================================================
FUNCTION       SRCH_MSGR_WAIT_ON_QTA_RSP

DESCRIPTION    This function waits until the QTA response message is
               processed. While waiting, it processes any other messages
               that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
extern void srch_msgr_wait_on_qta_rsp( void );

/*===========================================================================
FUNCTION       SRCH_MSGR_WAIT_ON_QTA_STATUS_QUERY_RSP

DESCRIPTION    This function waits until the QTA status query response message
               is processed. While waiting, it processes any other messages
               that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
==========================================================================*/
extern void srch_msgr_wait_on_qta_status_query_rsp( void );

/*===========================================================================
FUNCTION       SRCH_MSGR_WAIT_ON_RX_STOP_RSP

DESCRIPTION    This function waits until the RX STOP response message is
               processed. While waiting, it processes any other messages
               that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
==========================================================================*/
extern void srch_msgr_wait_on_rx_stop_rsp( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_WAIT_ON_SLAM_RSP

DESCRIPTION    This function waits until the slam rsp is received. While
               waiting, it processes any other messages that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_wait_on_slam_rsp( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_WAIT_ON_SLEW_ROLL_RSP

DESCRIPTION    This function waits until the slew roll rsp is received. While
               waiting, it processes any other messages that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_wait_on_slew_roll_rsp( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_WAIT_ON_SLEW_TIME_RSP

DESCRIPTION    This function waits until the slew time rsp is received. While
               waiting, it processes any other messages that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_wait_on_slew_time_rsp( void );

/*===========================================================================

FUNCTION       SRCH_MSGR_STOP

DESCRIPTION    This function de-registers the 1X client from the message
               router. We would no longer be able to send/receive messages.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_msgr_stop( void );
#endif /* SRCH_MSGR_H */
