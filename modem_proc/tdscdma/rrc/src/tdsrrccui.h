#ifndef TDSRRCCUI_H
#define TDSRRCCUI_H
/*===========================================================================
              R R C   C E L L   U P D A T E   H E A D E R   F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  Cell Update Procedure.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccui.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcrbcommon.h"
/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

#define TDSRRCCU_INVALID_TRANSACTION_ID 0xFF   /* This used to initialize RRCCU
                                              Transaction Id */

#define TDSRRCTMR_T_OOS_TIMER_IN_MS TDSRRCTMR_INFINITY_VAL   /* set a very large value for OOS timer value */  

#define TDSRRCTMR_CPHYCHANFAIL_TIMER_IN_MS 8000 /* Timer for phy chann failure */

#define TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER_IN_MS 4000 /* Timer for ignore non-exist MCM after cell update confirm */
/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/



typedef enum
{
  TDSRRCCU_SUCCESS,                            /* Successful Status */      
  TDSRRCCU_FAILURE_MINOR,                      /* Minor Failure Status */
  TDSRRCCU_FAILURE_MAJOR,                       /* Major Failure status */
  TDSRRCCU_PENDING_CONFIG                      /* this cause is set when there 
                                               is an ongoing configuration 
											   and CU is initiated with rl
											   failure or rlc unrecoverable 
											   error */
} tdsrrccu_status_e_type;

typedef enum
{
  TDSRRCCU_T314_T315_ZERO,                    /* Indicates both T314 and T315
                                              are zero */
  TDSRRCCU_T314_ZERO,                         /* Indicates T314 is zero and 
                                              T315 is non-zero */
  TDSRRCCU_T315_ZERO,                         /* Indicates T315 is zero and 
                                              T314 is non-zero */
  TDSRRCCU_T314_T315_NON_ZERO                 /* Indicates both T314 and T315
                                              are non-zero */
}tdsrrccu_re_est_timers_status_e_type;



typedef struct
{
  uint32 no_of_reg_procs;          /* Total number procedures to informed
                                      up on completion of CU */
  tdsrrc_proc_e_type rrc_proc[TDSMAX_NUM_OF_PROCEDURES];
                                   /* Details of procedures that 
                                      are registerd */
}tdsrrccu_reg_proc_type;

typedef enum
{
  TDSRRCCU_INCLUDE_FAILURE_CAUSE,         /* Indicates to include Failure cause */ 
                                     
  TDSRRCCU_NOT_INCLUDE_FAILURE_CAUSE      /* Indicates not to include Failure 
                                          cause */
}tdsrrccu_failure_status_e_type;

typedef enum
{
  TDSRRCCU_NO_RESP_MSG,                  /* No Response Msg is required */

  TDSRRCCU_UMIC_COMPLETE_RESP_MSG,       /* Xmit Utran Mobility Information
                                         Confirm Msg */
  TDSRRCCU_RBR_COMPLETE_RESP_MSG,        /* Xmit Radio Bearer Release Complete
                                         Msg */
  TDSRRCCU_RBRC_COMPLETE_RESP_MSG,       /* Xmit Radio Bearer Reconfiguration
                                         Complete Msg */ 
  TDSRRCCU_TCR_COMPLETE_RESP_MSG,        /* Xmit Transport Channel Reconfiguration
                                         Complete Msg */ 
  TDSRRCCU_PCR_COMPLETE_RESP_MSG         /* Xmit Physical Channel Reconfiguration
                                         Complete Msg */ 
}tdsrrccu_resp_msg_e_type;
  
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION RRCCU_CONVERT_T307_TO_MS

DESCRIPTION
  This function converts the tdsrrc_T_307 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrccu_convert_t307_to_ms
(
  tdsrrc_T_307 rrccu_t_307                    /* enum for Timer T307 */
);
/*===========================================================================

FUNCTION RRCCU_CONVERT_T302_TO_MS

DESCRIPTION
  This function converts the tdsrrc_T_302 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrccu_convert_t302_to_ms
(
  tdsrrc_T_302 rrccu_t_302                    /* enum for Timer T302 */
);
/*===========================================================================

FUNCTION RRCCU_CONVERT_T316_TO_MS

DESCRIPTION
  This function converts the tdsrrc_T_316 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrccu_convert_t316_to_ms
(
  tdsrrc_T_316 rrccu_t_316                    /* enum for Timer T316 */
);

/*===========================================================================

FUNCTION RRCCU_CONVERT_T314_TO_MS

DESCRIPTION
  This function converts the tdsrrc_T_314 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrccu_convert_t314_to_ms
(
  tdsrrc_T_314 rrccu_t_314                    /* enum for Timer T314 */
);

/*===========================================================================

FUNCTION RRCCU_CONVERT_T315_TO_MS

DESCRIPTION
  This function converts the tdsrrc_T_315 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrccu_convert_t315_to_ms
(
  tdsrrc_T_315 rrccu_t_315                    /* enum for Timer T315 */
);
/*===========================================================================

FUNCTION RRCCU_VALIDATE_CELL_UPDATE_INITIATE_REQ

DESCRIPTION

  This function validates TDSRRC_CELL_UPDATE_INITIATE_REQ command and stores
  initiates procedure if it needs confirmation after Cell Update procedure
  successfully compelted.
  
DEPENDENCIES


RETURN VALUE

  tdsrrccu_status_e_type: returns TDSRRCCU_SUCCESS if it successfully validates
  TDSRRC_CELL_UPDATE_INITIATE_REQ. Otherwise it returns
  TDSRRCCU_FAILURE_MINOR.

SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_status_e_type tdsrrccu_validate_cell_update_initiate_req
(    
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION RRCCU_CHECK_RE_EST_TIMERS

DESCRIPTION

  This function reads the values of Timers T314 and T315 from the SIB DB and
  returns details of the counters.
  
DEPENDENCIES


RETURN VALUE

  tdsrrccu_re_est_timers_status_e_type: returns TDSRRCCU_T314_T315_ZERO if both T314 and 
  T315 are zero. Returns TDSRRCCU_T314_ZERO if T314 is zero and T315 is a non-zero.
  Returns TDSRRCCU_T315_ZERO if T315 is non-zero and T315 is a zero and returns
  TDSRRCCU_T314_T315_NON_ZERO if both T314 and T315 are non-zero.

SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_re_est_timers_status_e_type tdsrrccu_check_re_est_timers
(    
  void
);
/*===========================================================================

FUNCTION RRCCU_ESTABLISH_RB0

DESCRIPTION
  This function sends TDSRRC_CHANNEL_CONFIG_REQ to RRC LLC to establish RB0
  
DEPENDENCIES


RETURN VALUE
  tdsrrccu_status_e_type: returns TDSRRCCU_SUCCESS if it successfully sends
  TDSRRC_CHANNEL_CONFIG_REQ to the RRC LLC. Otherwise it returns
  TDSRRCCU_FAILURE_MAJOR.


SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_establish_rb0
(    
  tdsrrc_rb_cfg_e_type action,                   /* Type of action RB 0 */
  tdsrrc_state_e_type dest_state,                /* Next state to transition is
                                                 expected to take place */
  boolean confirm_required                    /* This indicates whether
                                                 TDSRRC_CHANNEL_CONFIG_CNF is 
                                                 required or not */  
);
/*===========================================================================

FUNCTION RRCCU_BUILD_CELL_UPDATE_MSG

DESCRIPTION
  This function builds the CELL UPDATE message with all required
  parameters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
tdsrrccu_status_e_type tdsrrccu_build_cell_update_msg
(
  tdsrrc_UL_CCCH_Message *msg_ptr,            /* Pointer to uplink CCCH message*/
  tdsrrccu_failure_status_e_type status       /* Indicates whether to include
                                              failure cause or not */
);

/*===========================================================================

FUNCTION tdsrrccu_send_cell_update_msg

DESCRIPTION

  This function builds the CELL UPDATE message and enqueues the same in the 
  Uplink queue. Also starts the timer T302.
  
DEPENDENCIES


RETURN VALUE
  tdsrrccu_status_e_type: returns TDSRRCCU_SUCCESS if it successfully enqueues
  CELL UPDATE message in the Uplink queue. Otherwise it returns
  TDSRRCCU_FAILURE_MAJOR

SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_status_e_type tdsrrccu_send_cell_update_msg
(    
  rlc_lc_id_type ul_lc_id,                 /* Logical Channel Id */
  tdsrrccu_failure_status_e_type status       /* Indicates whether to include
                                              failure cause or not */
);
/*===========================================================================

FUNCTION tdsrrccu_send_rrc_initiate_cell_selection_ind

DESCRIPTION
  This function builds TDSRRC_INITIATE_CELL_SELECTION_IND command to post to
  CSP procedure with the cause specified.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrccu_status_e_type: It returns TDSRRCCU_SUCCESS if it successfully sends 
  TDSRRC_INITIATE_CELL_SELECTION_IND command to CSP. Otherwise it returns 
  TDSRRCCU_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_send_rrc_initiate_cell_selection_ind
(
  tdsrrc_cell_sel_cause_e_type cause,           /* Cause to trigger Cell
                                                selection */
  tdsrrc_state_e_type destination_state         /* Destination RRC state */
);

/*===========================================================================

FUNCTION RRCCU_PROCESS_CU_INITIAL

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_INITIAL substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_initial
(    
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION tdsrrccu_process_timer_t315_expired

DESCRIPTION
  This function processes TDSRRC_T315_EXPIRED_IND event. It checks whether T314
  is already expired. If T314 expired, then it will bring down RRC Connection
  and resets the CU state machine. Otherwise it releases the RBs associated 
  with T315 and remain in the same CU substate.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_timer_t315_expired
(    
  void
);
/*===========================================================================

FUNCTION tdsrrccu_process_timer_t314_expired

DESCRIPTION
  This function processes TDSRRC_T314_EXPIRED_IND event. It checks whether T315
  is already expired. If T315 expired, then it will bring down RRC Connection
  and resets the CU state machine. Otherwise it releases the RBs associated 
  with T314 and remain in the same CU substate.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_timer_t314_expired
(    
  void
);
/*===========================================================================

FUNCTION RRCU_PROCESS_CU_WAIT_FOR_UE_TO_CAMP_ON

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_UE_TO_CAMP_ON substate.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void rrcu_process_cu_wait_for_ue_to_camp_on
(    
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION RRCCU_PROCESS_CU_WAIT_FOR_CELL_UPDATE_CONFIRM

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_CELL_UPDATE_CONFIRM substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_wait_for_cell_update_confirm
(    
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION RRCCU_PROCESS_CU_WAIT_FOR_RB_EST_CNF

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_RB_EST_CNF substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_wait_for_rb_est_cnf
(    
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION RRCCU_PROCESS_CU_WAIT_FOR_L2_ACK

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_L2_ACK substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_wait_for_l2_ack
(    
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION tdsrrccu_process_cu_wait_for_rb0_est_cnf

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_RB0_EST_CNF substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_wait_for_rb0_est_cnf
(    
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION RRCCU_PROCESS_CU_WAIT_FOR_RB_REL_CNF

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_RB_REL_CNF substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_wait_for_rb_rel_cnf
(    
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION RRCCU_PROCESS_CU_WAIT_FOR_BPLMN_SUSPEND_CNF

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_BPLMN_SUSPEND_CNF substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_wait_for_bplmn_suspend_cnf
(    
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION RRCCU_PROCESS_CU_WAIT_FOR_RB_PROC_COMPL_IND

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND substate.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cu_wait_for_rb_proc_compl_ind
(    
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION RRCCU_PROCESS_CELL_DCH_STATE

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in CELL_DCH state of Connected  mode.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cell_dch_state
(    
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION RRCCU_PROCESS_CELL_FACH_STATE

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in CELL_FACH state of Connected  mode.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cell_fach_state
(  
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION RRCCU_PROCESS_DISCONNECTED_STATE

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in DISCONNECTED state of Idle  mode.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_disconnected_state
(  
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION RRCCU_PROCESS_CONNECTING_STATE

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in CONNECTING state of Idle  mode.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_connecting_state
(  
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION RRCCU_PROCESS_CELL_PCH_URA_PCH_STATE

DESCRIPTION
  This function processes all events that are dispatched to Cell Update 
  procedure in CELL_PCH and URA_PCH states of Connected  mode.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cell_pch_ura_pch_state
(  
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION tdsrrccu_resend_cell_update_msg

DESCRIPTION
  This function compares V302 with N302 and trasmits CELL UPDATE if 
  V302 <= N302. Otherwise it brings down RRC Connection.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_resend_cell_update_msg
(    
  tdsrrccu_failure_status_e_type status       /* Indicates whether to include
                                              failure cause or not */
);

/*===========================================================================

FUNCTION RRCCU_SEND_RESPONSE_MSG

DESCRIPTION
  This function transmits response message to CELL UPDATE CONFIRM message if
  required.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_send_response_msg
(    
  void
);
/*===========================================================================

FUNCTION RRCCU_DELETE_CRNTI

DESCRIPTION
  This function sends TDSRRC_RNTI_UPDATE_REQ to RRC LLC to delete the C-RNTI
  
DEPENDENCIES


RETURN VALUE
  tdsrrccu_status_e_type: returns TDSRRCCU_SUCCESS if it successfully sends
  TDSRRC_CHANNEL_CONFIG_REQ to the RRC LLC. Otherwise it returns
  TDSRRCCU_FAILURE_MAJOR.


SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_delete_crnti
(  
  void
);
/*===========================================================================

FUNCTION RRCCU_STOP_CU_TIMERS

DESCRIPTION
  This function stops all Cell Update procedure related timers. This
  function will be used when CU procedure non-gracefully terminates and
  RRC transitions to Disconnected State.


DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_stop_cu_timers
(
  void              
);
/*===========================================================================

FUNCTION RRCCU_INFORM_CELL_UPDATE_COMPLETED

DESCRIPTION

  This function checks the database and sends TDSRRC_CELL_UPDATE_COMPLETE_CNF
  command to all RRC procedures that waiting for Cell Update completetion.
  
DEPENDENCIES


RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_inform_cell_update_completed
(    
  void
);
/*===========================================================================

FUNCTION RRCCU_PROCESS_RLC_STATUS_IND

DESCRIPTION
  This function processes TDSRRC_CRLC_STATUS_IND command received from RLC
  when RLC reset is occured.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_rlc_status_ind
(    
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION tdsrrccu_process_re_est_timers

DESCRIPTION
  This function checks the timers T314 and T315 and releases the RBs
  accordingly and starts Cell Selection to camp-on a suitable cell and 
  transition to CELL_FACH state.
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_process_re_est_timers
(
  void
);
/*===========================================================================

FUNCTION tdsrrccu_process_oos_area_handling

DESCRIPTION

  This function is called when Cell update is initiated with cause 
  TDSRRC_REENTERED_SERVICE_AREA by CSP.  It triggers out of service area handling.
  
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_process_oos_area_handling
(
  void 
);
/*===========================================================================

FUNCTION RRCCU_RESET_REGISTRATION_INFO

DESCRIPTION
  This function will reset the registration information.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrccu_reset_registration_info
(    
  void  
);

/*===========================================================================
      
FUNCTION RRCCU_IS_URA_UPDATE_REQD

DESCRIPTION
  
This function will return TRUE if the URA update is required 
else it will return FALSE.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/


boolean tdsrrccu_is_ura_update_reqd
(
 void
);


/*===========================================================================

FUNCTION tdsrrccu_process_rlc_status_ind_in_non_initial_sub_states

DESCRIPTION
  This function processes TDSRRC_CRLC_STATUS_IND command received from RLC
  when RLC reset is occured and CU is active

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_rlc_status_ind_in_non_initial_sub_states
(    
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION RRCCU_GET_CELL_UPDATE_STARTED_STATUS_WITH_CAUSE

DESCRIPTION

  This function checks whether Cell Update procedure is started or not.
  
DEPENDENCIES


RETURN VALUE

  A value of rrccu_cell_update_status_e_type indicating the status of the
  function.

SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_cell_update_started_status_e_type tdsrrccu_get_cell_update_started_status_with_cause
(    
  tdsrrc_CellUpdateCause       *cu_cause_ptr
);

/*===========================================================================
FUNCTION tdsrrccu_get_rb_id_unrec_error()

DESCRIPTION
  This function return the RB id on which RLC unrecoverable error has occured.
  
DEPENDENCIES
  None

RETURN VALUE
  RB Id.
  
SIDE EFFECTS
  None
===========================================================================*/
tdsrrc_RB_Identity tdsrrccu_get_rb_id_unrec_error
(
  void
);

/*===========================================================================

FUNCTION tdsrrccu_set_tvm_ind_in_cu

DESCRIPTION
  This function will set the traffic volume indicator in cell update message. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_set_tvm_ind_in_cu
(
  tdsrrc_UL_CCCH_Message *msg_ptr
);


                         
/*===========================================================================
FUNCTION tdsrrccu_process_dl_sdu_ind_on_ccch

DESCRIPTION
  This function processes DL SDU IND command if it has come over CCCH.

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_process_dl_sdu_ind_on_ccch
(
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================
FUNCTION tdsrrccu_process_dl_sdu_ind_on_dcch

DESCRIPTION
  This function processes DL SDU IND command if it has come over DCCH.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_process_dl_sdu_ind_on_dcch
(
  tdsrrc_cmd_type * cmd_ptr
);

/*===========================================================================
FUNCTION tdsrrccu_process_after_oc_config_set_r8

DESCRIPTION
  This function processes the REl 8 CELL UPDATE CONFIRM after OC is set by CU CNF.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_process_after_oc_config_set_r8
(
  tdsrrc_CellUpdateConfirm_r8_IEs *cu_confirm_ptr
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  ,tdsrrcrb_directed_cell_info_type          directed_cell_info
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
);

/*===========================================================================
FUNCTION tdsrrccu_process_cu_cnf_after_validation_r8

DESCRIPTION
  This function processes the REl 8 CELL UPDATE CONFIRM message after doing the initial validation
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_process_cu_cnf_after_validation_r8
(
  void *msg_ptr,                           /* Pointer to Downlink Msg */
  tdsrrc_CellUpdateConfirm_r8_IEs *cu_confirm_ptr,
  uint32 dl_sdu_type                       /* Downlink SDU type */  
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  ,tdsrrcrb_directed_cell_info_type          directed_cell_info
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
);

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================
FUNCTION tdsrrccu_process_freq_redirection_info_r8

DESCRIPTION
  This function processes the REl 8 CELL UPDATE CONFIRM message for Freq Redirection.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrccu_process_freq_redirection_info_r8
(
  tdsrrc_CellUpdateConfirm_r8_IEs * cu_confirm_ptr,
  tdsrrcrb_directed_cell_info_type * directed_cell_info_ptr
);
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/


#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION RRCCU_PROCESS_CELL_UPDATE_CONFIRM_R8

DESCRIPTION
  This function processes the REl 8 CELL UPDATE CONFIRM message received in response to
  Cell Update message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

===========================================================================*/
void tdsrrccu_process_cell_update_confirm_r8
(    
  void *msg_ptr,                           /* Pointer to Downlink Msg */
  tdsrrc_RRC_TransactionIdentifier rcvd_transaction_id,
  tdsrrc_CellUpdateConfirm_r8_IEs *cu_confirm_ptr,
  uint32 dl_sdu_type                       /* Downlink SDU type */
);

/*===========================================================================

FUNCTION RRCCU_RESPONSE_TO_CELL_UPDATE_CONFIRM_R8

DESCRIPTION
  This function evaluates CELL UPDATE CONFIRM message and decides what
  response message has to be sent
  Cell Update message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_resp_msg_e_type tdsrrccu_find_response_to_cell_update_confirm_r8
(
  tdsrrc_CellUpdateConfirm_r8_IEs *cucnf_ptr
);

/*===========================================================================

FUNCTION tdsrrccu_validate_rb_info_r8

DESCRIPTION
  This function validates the Rb related IEs in Cu Cnf
  Cell Update message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrccu_validate_rb_info_r8
(
   tdsrrc_CellUpdateConfirm_r8_IEs * cu_confirm_ptr
);

/*===========================================================================

FUNCTION tdsrrccu_update_chan_config_rb_r8

DESCRIPTION
 
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/

uint32 
tdsrrccu_update_chan_config_rb_r8
(
 tdsrrc_CellUpdateConfirm_r8_IEs * cu_confirm_ptr,
 tdsrrc_rb_config_type * rb_info
 );

/*===========================================================================
FUNCTION tdsrrccu_validate_rel_rb_exist_in_reconfig_list_r8

DESCRIPTION
  This function checks if the Cu Cnf has the same RB ID in the RB Release list
   and also in RB Reconfig/RB Affected List
  
DEPENDENCIES
  None

RETURN VALUE
  True, if same RB ID is present in RB Release list and also in RB Reconfig/RB Affected List
  else
  FALSE
  
SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrccu_validate_rel_rb_exist_in_reconfig_list_r8
(
  tdsrrc_CellUpdateConfirm_r8_IEs * cu_confirm_ptr
);
#endif /*FEATURE_TDSCDMA_REL8*/

/*===========================================================================

FUNCTION TDSRRCCU_RESPONSE_TO_CELL_UPDATE_CONFIRM_R4

DESCRIPTION
  This function evaluates CELL UPDATE CONFIRM message and decides what
  response message has to be sent
  Cell Update message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_resp_msg_e_type tdsrrccu_find_response_to_cell_update_confirm_r4
(    
  void *msg_ptr,                              /* Pointer to Downlink Msg */
  uint32 dl_sdu_type                          /* Downlink SDU type */
);

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/*===========================================================================

FUNCTION tdsrrccu_establish_tm_um_srb

DESCRIPTION
  This function sends TDSRRC_CHANNEL_CONFIG_REQ to RRC LLC to establish RB0
  and RB1 to RB4 if the RB mapping info of these signalling RBs on FACH
  is available. rrcllc will decide whether to setup RB1 to RB4 along with RB0
  
DEPENDENCIES


RETURN VALUE
  tdsrrccu_status_e_type: returns TDSRRCCU_SUCCESS if it successfully sends
  TDSRRC_CHANNEL_CONFIG_REQ to the RRC LLC. Otherwise it returns
  TDSRRCCU_FAILURE_MAJOR.


SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_establish_tm_um_srb
(    
  boolean pch_to_fach_transition,
  tdsrrc_state_e_type dest_state,                /* Next state to transition is
                                                 expected to take place */
  boolean confirm_required                    /* This indicates whether
                                                 TDSRRC_CHANNEL_CONFIG_CNF is 
                                                 required or not */  
);
/*===========================================================================

FUNCTION tdsrrccu_fill_rb_id_for_fach_state

DESCRIPTION
  This function fill another RBs which is not in the affect and reconfiguration list
  in Cell_FACH state.
  
DEPENDENCIES


RETURN VALUE
  tdsrrccu_status_e_type: returns TDSRRCCU_SUCCESS if it successfully sends
  TDSRRC_CHANNEL_CONFIG_REQ to the RRC LLC. Otherwise it returns
  TDSRRCCU_FAILURE_MAJOR.


SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_fill_rb_id_for_fach_state
(uint32 *rb_count,
  tdsrrc_channel_config_req_type *chan_config_req,
  boolean srb_reest_ind,
  boolean urb_reest_ind
 );
#endif
                         
#endif /* TDSRRCCUI_H */ 
