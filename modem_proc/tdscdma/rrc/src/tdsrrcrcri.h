#ifndef TDSRRCRCRI_H
#define TDSRRCRCRI_H
/*===========================================================================
              R R C R C R I  H E A D E R  F I L E

DESCRIPTION
 The RRCRCR module consists of RRC CONNECTION RELEASE PROCEDURE 
 implementation. The 3GPP TS 25.331 RRC Protocol specification is used as
 basis for implementation. 
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrcri.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/21/11   yuh    Merged LTE code
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
12/17/10   rl      Fixed the compiler error
11/03/10   rl      Fixed review comments
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdscdma_variation.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcmm_v.h"
#include "tdsrrcintif.h"

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/
 /* This sanity timer value is selected to 29 secs since Upper layer (CC) has
    a timer for value 30 secs. */

/*Further reduce to 10 sec to minimize the release delay */
#define TDSRRCRCR_TIMER_VALUE_FOR_L2_ACK 10000 /* This sanity check timer 
                                               to receive L2 ack */

/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

/*==========================================================================
  Type for defining the substate of RRC Connection Release Procedure.
===========================================================================*/

typedef enum 
{
  TDSRRCRCR_INITIAL,               /* RCR initial substate  */

  TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE, 
                                /* RCR waits for the Timer T308 to expire */
  TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE,
                                /* RCR waits for L2 ACK for transmission 
                                   of RRC Connection Release Complete Msg  */
  TDSRRCRCR_WAIT_FOR_CELL_UPDATE_COMP_CNF,
                                /* RCR waits for CELL UPDATE to complete so
                                   that a valid C_RNTI is available to 
                                   transmit in CELL_FACH state */
  TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF
                                /* RCR waits for RRC LLC to Release of 
                                   SRBs */
} tdsrrcrcr_substate_e_type;

typedef enum
{
  TDSRRCRCR_SUCCESS,               /* Successful Status */
  TDSRRCRCR_FAILURE_MAJOR,         /* Major Failure status */
  TDSRRCRCR_FAILURE_MINOR          /* Minor Failure Status */
}tdsrrcrcr_status_e_type;

typedef enum
{
  TDSRRCRCR_INCLUDE_ERROR_INDICATION,     /* Indicates to include Error 
                                          Indication */
  TDSRRCRCR_NOT_INCLUDE_ERROR_INDICATION  /* Indicates not to include Error
                                          Indication */
}tdsrrcrcr_error_indication_status_e_type;


/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION tdsrrcrcr_process_disconnected_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in DISCONNECTED state of Idle  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_disconnected_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);
/*===========================================================================

FUNCTION tdsrrcrcr_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in CELL_FACH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);
/*===========================================================================

FUNCTION tdsrrcrcr_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in CELL_DCH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);
/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_initial_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_INITIAL substate of either CELL_FACH State or
  CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);
/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_t308_timer_expire_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE  substate of 
  CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_t308_timer_expire_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_l2ack_for_rrc_conn_rel_complete_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE
  substate of CELL_FACH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_l2ack_for_rrc_conn_rel_complete_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);
/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Release procedure in TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF substate of either
  CELL_DCH State or CELL_FACH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION tdsrrcrcr_convert_t308_to_ms

DESCRIPTION
  This function converts the tdsrrc_T_308 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrcrcr_convert_t308_to_ms
(
  tdsrrc_T_308 rrcrcr_t_308                   /* enum for Timer T308 */
);


/*===========================================================================

FUNCTION tdsrrcrcr_send_connection_rel_complete_msg

DESCRIPTION
  This function builds RRC CONNECTION RELEASE COMPLETE message and send the
  same to the SEND CHAIN to transmit either in Unacknowledged mode or 
  Acknowledged mode.
DEPENDENCIES
  None

RETURN VALUE
  rrcrcr_state_e_type: It returns TDSRRCRCR_SUCCESS if it successfully places 
  RRC CONNECTION RELEASE COMPLETE SDU to the SEND CHAIN. Otherwise it returns
  TDSRRCRCR_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrcr_status_e_type tdsrrcrcr_send_connection_rel_complete_msg
(
  tdsrrc_RB_Identity rb_id,                    /* Radio bearer Id */ 
  uecomdef_logch_mode_e_type mode,          /* Mode of transmit */  
  tdsrrcrcr_error_indication_status_e_type error_ind_status
                                            /* Indicates whether to include
                                               Error Indication field in the 
                                               RRC Connection Rel comlete Msg*/                                         
);

/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_cell_update_comp_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_WAIT_FOR_CELL_UPDATE_COMP_CNF  substate of 
  CELL_FACH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_cell_update_comp_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
);

/*==========================================================================
FUNCTION tdsrrcrcr_save_redirection_info

DESCRIPTION
  This function saves the redirection info received in RRC
  Connection Release message. This is info is then later passed on to
  CSP for further processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_save_redirection_info
(
  tdsrrc_RedirectionInfo_r6 *redirection_info_ptr
);

/*==========================================================================
FUNCTION tdsrrcrcr_send_redirection_req

DESCRIPTION
  This function builds the TDSRRC_REDIRECT_REQ command with all parameters
  received through RRC CONNECTION RELEASE message and sends the command to
  Cell Selection Procedure
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcrcr_send_redirection_req
(
  void
);

#if defined FEATURE_TDSCDMA_TO_LTE
/*========================================================================
FUNCTION tdsrrcrcr_save_lte_redirection_info

DESCRIPTION
  This function saves the LTE redirection info received in RRC
  Connection Release message. This info is then later passed on to
  CSP for further processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void tdsrrcrcr_save_lte_redirection_info
(
  tdsrrc_EUTRA_TargetFreqInfoList *EUTRA_target_freq_info_ptr
);
#endif

#ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
/*===========================================================================
FUNCTION tdsrrcrcr_cancel_reconfig_proc

DESCRIPTION
  This function is used to check whether can cancel the reconfiguration procedure
  and try to cancel the reconfiguration procedure.
  
DEPENDENCIES
  None

RETURN VALUE
 void

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_cancel_reconfig_proc
(
  void
);
#endif

#endif /* TDSRRCRCRI_H */

