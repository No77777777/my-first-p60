#ifndef TDSRRCRCEI_H
#define TDSRRCRCEI_H
/*===========================================================================
              R R C R C E I  H E A D E R  F I L E

DESCRIPTION
 The RRCRCE module consists of RRC CONNECTION ESTABLISHMENT PROCEDURE 
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrcei.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/03/10   rl      Fixed compiler warnings
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
#include "comdef.h"

#include "queue.h"

#include "rex.h"

#include "tdsrrccmd_v.h"

#include "tdsuecomdef.h"

#include "tdsrrcasn1.h"
/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

/*  NAS times out after 15 seconds if it did not receive response for 
    TDSRRC_EST_REQ. The timer for UE to camp on is selected 14 seconds */
#define TDSRRCRCE_TIMER_VALUE_FOR_UE_TO_CAMP_ON  14000 
#define TDSRRCRCE_TIMER_VALUE_FOR_RRC_CON_REL  3000
#define TDSRRCRCE_TIMER_VALUE_FOR_SIG_CON_REL 40000
/* When NAS indicates that it is not a severe scenario, start timer for 2 secs */
#define TDSRRCRCE_TIMER_VALUE_SMALL_FOR_RRC_CON_REL  2000
#define TDSRRCRCE_ZERO_TIMER_VALUE_FOR_RRC_CON_REL 0  
#define TDSRRCRCE_TIMER_VALUE_FOR_SCRI_HOLDOFF 2000

/*Delay time for sending  Signalling connection release*/
#define TDSRRCRCE_DELAY_TIMER_VALUE_FOR_SIG_CON_REL 3000  

/* This sanity timer value is selected to 29 secs since Upper layer (CC) has
   a timer for value 30 secs. */
#define TDSRRCRCE_TIMER_VALUE_FOR_L2_ACK    29000

/* Wait time for receiving a new cell indication when initiating Cell selection a
fter physical channel failure happened. */
#define TDSRRCTMR_PHYCHANFAILURE_TIMER_IN_MS 10000

#define TDSRRCRCE_INVALID_TRANSACTION_ID 0xFF /* This used to initialize RRCRCE
                                              Transaction Id */

/* The maximum number of digits in MCC */
#define TDSRRCRCE_MCC_LENGTH  3
/* The maximum number of digits in MNC */
#define TDSRRCRCE_MNC_LENGTH  3

/* The following Bit lengths  are for December version of 25.331 specification. 
   If ASN1 for future revisions changes, then these values needs to be 
   updated accordingly */

#define TDSNUM_OF_BITS_FOR_CRITICAL_EXT_CHOICE 0x01
                                          /* Number of bits for Critical
                                             externsion choice */
#define TDSNUM_OF_BITS_FOR_NON_CRITIAL_EXT_PRESENT 0x01
                                          /* Number of bits for Non-Critical
                                             externsion choice */
#define TDSNUM_OF_BITS_FOR_RRC_CONN_SETUP_OPTIONAL_FIELDS 0x0A
                                          /* Number of bits for Optional fields
                                             in RRC Connection Setup message */
#define TDSNUM_OF_BITS_FOR_RRC_CONN_REJECT_OPTIONAL_FIELDS 0x01
                                          /* Number of bits for Optional fields
                                             in RRC Connection Reject message */
#define TDSNUM_OF_BITS_FOR_INITIAL_UE_ID_CHOICE 0x03
                                          /* Number of bits for Initial UE Id
                                             choice */
#define TDSNUM_OF_BITS_FOR_VARIABLE_IMSI_LENGTH 0x04
                                          /* Number of bits for varibale IMSI 
                                             length */
#define TDSNUM_OF_BITS_FOR_FIXED_IMSI_LENGTH 0x06
                                          /* Number of bits for fixed IMSI 
                                             length */
#define TDSNUM_OF_BITS_FOR_DIGIT  0x04       /* Number of bits for a digit */ 

#define TDSNUM_OF_BITS_FOR_TMSI  0x20        /* Number of bits for TMSI */

#define TDSNUM_OF_BITS_FOR_PTMSI 0x20        /* Number of bits for PTMSI */

#define TDSNUM_OF_BITS_FOR_OPTIONAL_MNC  0x01/* Number of bits for optional MNC 
                                             digit */

/*The reason why RCE procedure has its own macro for detecting RCE OTA is because it's not Reconfig
OTA. Otherwise, we would have used TDSRRCRB_R8_CRITICAL_EXT_PRESENT_PTR from tdsrrcrbcommon.h*/

// CR 352866 begin
#define TDSRRCRCE_TIMER_VALUE_FOR_WAIT_RESELCT  5000 
// CR 352866 end

#define TDSRRCRCE_SOFT_ABORT_GUARD_TIMER_VAL 7000

#ifdef FEATURE_TDSCDMA_REL8


#define TDSRRCRCE_R8_CRITICAL_EXT_PRESENT \
  ((msg_ptr->message.u.rrcConnectionSetup.u.  \
    later_than_r3.criticalExtensions.t ==  \
    T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_3_criticalExtensions) &&\
     (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_2_criticalExtensions) &&\
     (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_2_r8)) 


#endif /* FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9


#define TDSRRCRCE_R9_CRITICAL_EXT_PRESENT \
  ((msg_ptr->message.u.rrcConnectionSetup.u.  \
    later_than_r3.criticalExtensions.t ==  \
    T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_criticalExtensions) &&\
     (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_3_criticalExtensions) &&\
     (msg_ptr->message.u.rrcConnectionSetup.u. \
     later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
     T_tdsrrc_RRCConnectionSetup_criticalExtensions_2_criticalExtensions) && \
    (msg_ptr->message.u.rrcConnectionSetup.u. \
    later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == \
    T_tdsrrc_RRCConnectionSetup_criticalExtensions_1_r9)) 


#endif /* FEATURE_TDSCDMA_REL9 */

                                             
/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

/*==========================================================================
  Type for defining the substate of RRC Connection Establishment Procedure.
===========================================================================*/

typedef enum 
{
  TDSRRCRCE_INITIAL,               /* RCE initial sub state              */
  TDSRRCRCE_WAIT_FOR_UE_CAMP_ON,   /* RCE waits for the UE to camp on */
  TDSRRCRCE_WAIT_FOR_SIB7_CURRENT, /* RCE waits for SIB & to become current */
  TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF,/* RCE waits for RB0 conf from RRC LLC     */
  TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP,/* RCE waits for RRC Connection Setup Msg  */
  TDSRRCRCE_WAIT_FOR_RESELCT_COMPL, /* RCE waits for reselection to complete */
  TDSRRCRCE_WAIT_FOR_REDIRECT_CNF, /* RCE waits for conf from CSP procedure   */
  TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE, 
                                /* RCE waits for timer "Wait Time" to expire*/
  TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE, 
                                /* RCE waits for timer to expire*/
  TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF,/* RCE waits for Signalling RBs setup conf */
  TDSRRCRCE_WAIT_FOR_CU_COMPLETE_CNF,  
                                /* RCE waits for Cell Update to complete and 
                                   a valid C-RNTI is available to transmit */
  TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL,
                                /* RCE waits for Signalling Connection 
                                   Release  */
  TDSRRCRCE_WAIT_FOR_L2_ACK,       /* RCE waits for L2-ACK for RRC Connection */
                                /* Setup Complete message */
  TDSRRCRCR_WAIT_FOR_SRB_REL_CNF,  /* RCE waits for the confrimation SRB 
                                   release */                                  
  TDSRRCRCE_FINAL,                  /* RCE remains in this sub state until RRC */
                                /* Connection is released */
  TDSRRCRCE_WAIT_FOR_RB0_REL_CNF,
  TDSRRCRCE_WAIT_FOR_RB0_PHYCHAN_RESETUP_CNF /* Physical channel resetup for RBO after
                                           * Inter-RAT redirection failure
                                           */
  // CR 352866 begin
  , TDSRRCRCE_WAIT_NEW_CELL_IND      /* pending TDSRRC_EST_REQ because cell reselecting */
  , TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7      /* pending SIB7 receive because cell reselecting */
  // CR 352866 end
} tdsrrcrce_substate_e_type;

typedef enum
{
  TDSRRCRCE_SUCCESS,               /* Successful Status */
  TDSRRCRCE_FAILURE_MAJOR,         /* Major Failure status */
  TDSRRCRCE_FAILURE_MINOR          /* Minor Failure Status */
}tdsrrcrce_status_e_type;

typedef enum
{
  TDSRRCRCE_CONNECTION_REJECT,     /* Inter-Freq or Inter-RAT redirection received by 
                                 * the UE in RRC CONNECTION REJECT message */
  TDSRRCRCE_CONNECTION_SETUP       /* Inter-Freq redirection received by 
                                 * the UE in RRC CONNECTION SETUP message */
}tdsrrcrce_conn_rsp_e_type;


/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION tdsrrcrce_process_disconnected_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in Disconnected state of Idle mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_disconnected_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_connecting_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in Connecting state of Idle mode. The connecting 
  state is a transient state created to honor Paging Type 1 message during the
  RRC Connection Establishment procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_connecting_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in CELL_FACH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in CELL_DCH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_cell_ura_pch_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure either in CELL_PCH state or URA_PCH state of 
  Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_cell_ura_pch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_initial_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_INITIAL substate of Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_ue_camp_on_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_UE_CAMP_ON  substate of 
  Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_ue_camp_on_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_rb0_setup_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF substate of 
  Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rb0_setup_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_conn_setup_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in RRCRCE_WAIT_FOR_CONN_SETUP substate of Connecting
  State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_conn_setup_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_srb_setup_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_SRB_SETUP_CNF substate of 
  Connecting State
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_srb_setup_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================
FUNCTION tdsrrcrce_process_rce_final_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in RRCRCE__FINAL substate of either CELL_FACH or
  CELL_DCH State.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_final_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION tdsrrcrce_build_rrc_connection_request_msg

DESCRIPTION
  This function builds the RRC CONNECTION REQUEST message with all required
  parameters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_build_rrc_connection_request_msg
(
  tdsrrc_UL_CCCH_Message *msg_ptr               /* Pointer to uplink CCCH message*/
);

/*===========================================================================

FUNCTION tdsrrcrce_compare_arrays

DESCRIPTION
  This function checks whether two arrays of specified length are equal or
  not. This function uses the standard library memcmp function.

DEPENDENCIES
  This function converts type of arrays into charecter arrays before comparing.
  Hence length should be multiplied suitably if integer or any other types are
  compared.

RETURN VALUE
  TRUE if both are same. otherwise FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcrce_compare_arrays
(
  void *str1,                               /* First array */
  void *str2,                               /* Second array */
  uint32 length                             /* Length of the array */
);
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_connection_request_msg

DESCRIPTION
  This function builds the RRC CONNECTION REQUEST message and places the same
  in the watermark queue to transmit OTA. Even if for some reason connection 
  request is not sent, then also T300 will be started so that sending connection 
  request can be retried after its expiry.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_rrc_connection_request_msg
(
  void
);
/*===========================================================================
FUNCTION tdsrrcrce_send_rrc_connection_setup_complete_message

DESCRIPTION
  This function builds the RRC CONNECTION SETUP COMPLETE message and places
  the same in the watermark queue for transmission.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : It returns TDSRRCRCE_SUCCESS if it successfully builds 
  the RRC CONNECTION SETUP COMPLETE message and calls the Send Chain. 
  Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_connection_setup_complete_message
(
  void
);
/*===========================================================================

FUNCTION tdsrrcrce_get_t300_from_serv_cell_db

DESCRIPTION
  This function reads the serving cell DB and returns the T300 value from the
  UE Timers and Constants in idle mode field

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : Returns TDSRRCRCE_SUCCESS If it successful retrieves
  T300 from the SIB database. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_get_t300_and_n300_from_serv_cell_db
(
  tdsrrc_T_300 *rrcrce_t_300                     /* Pointer to T300 */ 
);
/*===========================================================================

FUNCTION tdsrrcrce_get_n300_from_serv_cell_db

DESCRIPTION
  This function reads the serving cell DB and returns the N300 value from the
  UE Timers and Constants in idle mode field

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : Returns TDSRRCRCE_SUCCESS If it successful retrieves
  N300 from the SIB database. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_get_n300_from_serv_cell_db
(
  tdsrrc_N_300 *rrcrce_n_300                     /* Pointer to N300 */  
);

/*===========================================================================

FUNCTION tdsrrcrce_get_t300_from_serv_cell_db

DESCRIPTION
  This function reads the serving cell DB and returns the T300 value from the
  UE Timers and Constants in idle mode field

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : Returns TDSRRCRCE_SUCCESS If it successful retrieves
  T300 from the SIB database. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_get_t300_from_serv_cell_db
(
  tdsrrc_T_300 *rrcrce_t_300                    /* Pointer to T300 */ 
);

/*===========================================================================
FUNCTION tdsrrcrce_append_start_list

DESCRIPTION
  This function appends the START list to the RRC CONNECTION SETUP COMPLETE 
  message.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcrce_append_start_list
(
  struct tdsrrc_STARTList *start_list_ext     /* Pointer to STARTList*/
);

/*===========================================================================
FUNCTION tdsrrcrce_send_redirection_req

DESCRIPTION
  This function builds the TDSRRC_REDIRECT_REQ command with all parameters
  received through RRC CONNECTION REJECT message and sends the command to
  Cell Selection Procedure
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : Returns TDSRRCRCE_SUCCESS if it successfully sends
  TDSRRC_REDIRECT_REQ command. Otherwise returns TDSRRCRCE_FAILURE_MAJOR
SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_redirection_req
(
  tdsrrc_DL_CCCH_Message   *msg_ptr,
  boolean                cpid_info_present
);

#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================
FUNCTION tdsrrcrce_send_lte_redirection_req

DESCRIPTION
  This function builds the RRC_REDIRECT_REQ command with all parameters
  received through RRC CONNECTION REJECT message and sends the command to
  Cell Selection Procedure
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_lte_redirection_req
(
  tdsrrc_DL_CCCH_Message    *msg_ptr
);
#endif

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_est_cnf

DESCRIPTION
  This function builds RRC_EST_CNF command to post to MM with 
  success  status of RRC Connection Establishment procedure.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_EST_CNF command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_est_cnf
(
  void
);

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_abort_ind

DESCRIPTION
  This function builds RRC_ABORT_IND command to post to MM with failure 
  status of RRC Connection Establishment procedure.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_EST_CNF command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_abort_ind
(
  rrc_abort_cause_e_type cause               /* Cause for RRC Connection 
                                                Failure */
);


/*==========================================================================

FUNCTION tdsrrcrce_update_initial_ue_identity

DESCRIPTION
  This function updates contents of Initial UE Identity Choice and Initial
  UE Identity received from NAS TDSRRC_EST_REQ primitive.
DEPENDENCIES
  None

RETURN VALUE
  None: 

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_update_initial_ue_identity
(
  tdsrrc_InitialUE_Identity *rrc_initial_ue_identity_ext
                                             /* Initial UE Indentity to be 
                                                sent over the air */
);
/*==========================================================================

FUNCTION tdsrrcrce_update_establishment_cause

DESCRIPTION
  This function gets equivalent external RRC Connection Establishment cause
  for the internal RRC Connection Established cause received from the NAS 
  through TDSRRC_EST_REQ primitive.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
tdsrrc_EstablishmentCause tdsrrcrce_update_establishment_cause
(
  void
);
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_channel_config_req_for_rb0

DESCRIPTION
  This function builds TDSRRC_CHANNEL_CONFIG_REQ command and  requests RRC LLC
  to either setup or modify or release  Radio Bearer 0.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  TDSRRC_CHANNEL_CONFIG_REQ command to the RRC LLC. Otherwise it returns
  TDSRRCRCE_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_channel_config_req_for_rb0
(
               
  tdsrrc_rb_cfg_e_type action,                   /* Type of action RB 0 */
  tdsrrc_state_e_type next_state,                /* Next state to transition is
                                                 expected to take place */
  boolean confirm_required                    /* This indiactes whether
                                                 TDSRRC_CHANNEL_CONFIG_CNF is 
                                                 required or not */
);
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_channel_config_req_for_srb

DESCRIPTION
  This function builds TDSRRC_CHANNEL_CONFIG_REQ command and  requests RRC LLC
  to either setup or modify or release  Signalling Radio Bearers 1 through 4.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  TDSRRC_CHANNEL_CONFIG_REQ command to the RRC LLC. Otherwise it returns
  TDSRRCRCE_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_channel_config_req_for_srb
(
               
  tdsrrc_rb_cfg_e_type action,                   /* Type of action on SRBs */
  tdsrrc_state_e_type next_state,                /* Next state to transition is
                                                 expected to take place */
  boolean confirm_required                    /* This indiactes whether
                                                 TDSRRC_CHANNEL_CONFIG_CNF is 
                                                 required or not */
);
/*===========================================================================

FUNCTION tdsrrcrce_validate_rrc_connection_setup_msg

DESCRIPTION
  This function validates whether RRC CONNECTION SETUP message contain min.
  RB info to setup RRC Connection.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type : It returns TDSRRCRCE_SUCCESS if it contains RB 1,RB 2,
  RB 3 and optionally RB 4 info. Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_validate_rrc_connection_setup_msg
(
  tdsrrc_DL_CCCH_Message *msg_ptr,              /* Pointer to Downlink CCCH Msf */
  uint32 dl_sdu_type,                         /* Downlink SDU type */
  tdsrrc_msg_ver_enum_type* msg_version_ptr
);
/*===========================================================================

FUNCTION tdsrrcrce_validate_rrc_est_req

DESCRIPTION
  This function checks whether TDSRRC_EST_REQ contains a valid Initial UE
  indentity.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it finds a valid
  Initial UE Identity in TDSRRC_EST_REQ command to the MM.
  Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_validate_rrc_est_req
(
  tdsrrc_cmd_type *cmd_ptr /* Pointer to RRC Command */
);

/*===========================================================================

FUNCTION rrcrce_validate_access_class

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it finds a valid
  Initial UE Identity in TDSRRC_EST_REQ command to the MM.
  Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_validate_cell_access_status
(
  rrc_cn_domain_identity_e_type   rrc_cn_domain_id,
  rrc_establish_cause_e_type   establishment_cause,
  rrc_abort_cause_e_type  *cause              /* cause */
);

/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_redirect_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_REDIRECT_CNF substate of 
  Connecting State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_redirect_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in RRCRCE_WAIT_FOR_WAIT_TIME_EXPIRE substate of 
  Connecting State
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rrc_timer_expire_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_wait_time_expire_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in RRCRCE_WAIT_FOR_WAIT_TIME_EXPIRE substate of 
  Connecting State
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_wait_time_expire_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_rel_ind

DESCRIPTION
  This function builds RRC_REL_IND command to post to MM with the 
  RRC Connection Release cause.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_REL_IND command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_rel_ind
(
  void
);

/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_initiate_cell_selection_ind

DESCRIPTION
  This function builds TDSRRC_INITIATE_CELL_SELECTION_IND command to post to
  CSP procedure with the cause specified.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  TDSRRC_INITIATE_CELL_SELECTION_IND command to CSP. Otherwise it returns 
  TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_initiate_cell_selection_ind
(
  tdsrrc_cell_sel_cause_e_type cause,           /* Cause to trigger Cell selection */
  tdsrrc_state_e_type dest_state                /* Destination State */
);

/*===========================================================================

FUNCTION tdsrrcrce_send_cell_update_initiate_req

DESCRIPTION
  This function send the cell update initiate request if UE does not have
  a valid C-RNTI in CELL_FACH state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrce_send_cell_update_initiate_req
(
  tdsrrc_proc_e_type procedure
);
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_cu_complete_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in RRCRCE_WAIT__FOR_CU_COMPLETE_CNF sub state of 
  CELL_FACH State
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_cu_complete_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCR_WAIT_FOR_SRB_REL_CNF substate of any RRC 
  state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_srb_rel_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_sib7_current

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_SIB7_CURRENT  substate of 
  Disconnected State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_sib7_current
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_reselct_compl_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RESELCT_COMPL substate of 
  Connecting State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_reselct_compl_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
);
/*===========================================================================

FUNCTION tdsrrcrce_convert_t300_to_ms

DESCRIPTION
  This function converts the tdsrrc_T_300 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrcrce_convert_t300_to_ms
(
  tdsrrc_T_300 rrcrce_t_300                   /* enum for Timer T300 */
);
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL sub state either 
  CELL_FACH State or CELL_DCH state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RB0_REL_CNF substate of any RRC 
  state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rb0_rel_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);
/*===========================================================================

FUNCTION tdsrrcrce_send_rrc_abort_cnf

DESCRIPTION
  This function builds RRC_ABORT_CNF command to post to MM in response to 
  TDSRRC_ABORT_REQ.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: It returns TDSRRCRCE_SUCCESS if it successfully sends 
  RRC_ABORT_CNF command to the MM. Otherwise it returns TDSRRCRCE_FAILURE_MAJOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_send_rrc_abort_cnf
(
  void
);

/*===========================================================================

FUNCTION tdsrrcrce_process_rce_wait_for_rb0_phychan_resetup_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Establishment procedure in TDSRRCRCE_WAIT_FOR_RB0_PHYCHAN_RESETUP_CNF  substate of 
  Connecting State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_rce_wait_for_rb0_phychan_resetup_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION tdsrrcrce_process_connection_rej_with_wait_time

DESCRIPTION
  This function processes connection rejection req with wait time:
  1)connection for registration,go to idle and let NAS try
  2)connection for ps signaling,change lock reason from ACCESS to BG_TRAFFIC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_process_connection_rej_with_wait_time
(
  void
);

#endif /* TDSRRCRCEI_H */

