#ifndef TDSRRCDT_H
#define TDSRRCDT_H
/*===========================================================================
                  R R C  D I R E C T  T R A N S F E R

DESCRIPTION
  This header file contains function prototypes for five procedures that deal
  with direct transfer:
  Initial Direct Transfer, Uplink Direct Transfer, Downlink Direct Transfer,
  Signalling Connection Release, and Signalling Connection Release Request.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcdt.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/29/11   hx      Remove not needed inclusion of "rrclsmif.h" 
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */


#include "tdscdma_variation.h"
#include "tdsrrcmm_v.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* This is the upper bound on the size of the payload in the Initial
 * Direct Transfer and Uplink Direct Transfer Messages. This value is
 * specified in the ASN.1.
 */
#define TDSRRCDT_MAX_PAYLOAD 4095
#define TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH 10  
                                           /* Length of Intra Domain NAS
                                              Node Selector in bits */
#define TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN (SYS_MAX_PDP_CONTEXTS + 2)
                                              /* Number of NAS messages  
                                              that can be stored for PS*/
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef enum
{
  TDSRRCSCR_SIG_CONN_NOT_EXIST,        /* Indicates that Signalling Connection
                                       does not exist for the particular\
                                       CN Domain */
  TDSRRCSCR_SIG_CONN_RELEASED,         /* Indicates that Signalling Connection
                                       is released successfully */
  TDSRRCSCR_RAB_PRESENT_IN_SIG_DOMAIN, /* Indicates that a RAB is present in
                                       the signalling connection that is 
                                       being requested to be released*/
  TDSRRCSCR_NO_RAB_PRESENT_IN_SIG_DOMAIN/* Indicates that RAB is not present in
                                       the signalling connection that is 
                                       being requested to be released*/
} tdsrrcscr_sig_conn_rel_status_e_type;


typedef enum
{
  TDSRRCDT_SUCCESS,                            /* Successful Status */      
  TDSRRCDT_FAILURE_MINOR,                      /* Minor Failure Status */
  TDSRRCDT_FAILURE_MAJOR                       /* Major Failure status */
} tdsrrcdt_status_e_type;

typedef enum
{
  TDSRRCSCRR_MUI_STATUS_FALSE,                 /* No outstanding MUI's                  */
  TDSRRCSCRR_MUI_SIG_CON_REL_CNF,              /* Outstanding MUI for sig-con-rel-cnf   */
  TDSRRCSCRR_MUI_CLOSE_SESSION_CNF             /* Outstanding MUI for close-session-cnf */
}tdsrrcscrr_close_session_mui_status_type;

/*==========================================================================
  Type for defining the substates of Initial direct Transfer Procedure.
===========================================================================*/

typedef enum 
{
  TDSRRCIDT_INITIAL,               /* IDT initial sub state              */
  TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF,
                                /* IDT waits for confirmation from Cell 
                                   Update Procedure complete */
  TDSRRCIDT_WAIT_FOR_L2ACK         /* IDT waits for l2 ACK for Initial Direct
                                   Transfer message */
} tdsrrcidt_substate_e_type;

/*==========================================================================
  Type for defining the substates of Uplink direct Transfer procedure.
===========================================================================*/

typedef enum 
{
  TDSRRCUDT_INITIAL,               /* UDT initial sub state              */
  TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF,
                                /* UDT waits for confirmation from Cell 
                                   Update Procedure complete */ 
  TDSRRCUDT_WAIT_FOR_L2ACK         /* UDT waits for l2 ACK for Initial Direct
                                   Transfer message */
                                   
} tdsrrcudt_substate_e_type;
/*==========================================================================
  Type for defining the substates of Signalling Connection Release Request 
  procedure.
===========================================================================*/

typedef enum 
{
  TDSRRCSCRR_INITIAL,               /* SCRR initial sub state              */
  TDSRRCSCRR_WAIT_FOR_CELL_UPDATE_COMP_CNF,
                                /* SCRR waits for confirmation from Cell 
                                   Update Procedure complete */ 
  TDSRRCSCRR_WAIT_FOR_L2ACK       /* SCRR waits for L2 ACK for Signalling
                                  Connection Release Request message */
} tdsrrcscrr_substate_e_type;

typedef struct
{
  /* This flags indicate the status of MUIs for Uplink Direct
     Transfer messages that are pending for L2 ACK */
  boolean             mui_status;
 
  /* This variable to stores the corresponding MUIs of Uplink
     Direct Transfer messages that are transmitted */
  uint8               mui;

   /* This flag indicates whether TDSRRC_DATA_REQ is stored or not */
  boolean             data_req_status;

  /* This struct holds the Uplink Direct Transfer Procedure contents 
     which are saved from the DATA_REQ call for  a Domain
   */
  rrc_data_req_type   data_req;

  
}tdsrrcudt_data_req_type;

typedef struct
{
  /* This flag indictes whether RRC_DATA_IND is stored or not */
  boolean           data_ind_status;


  /* This struct holds the Downlink Direct Transfer procedure contents
  that are saved from the TDSRRC_DOWNLINK_SDU_IND for a Domain */
  rrc_data_ind_type data_ind;


}tdsrrcddt_data_ind_type;


typedef struct
{
  /* This flag indicate the status of MUIs for Signalling
     Connection Request */
  tdsrrcscrr_close_session_mui_status_type mui_status;
 
  /* This variable to stores the corresponding MUIs of the
     Signalling Connection Release Request message that is transmitted */
  uint8               mui;

   /* This flag indicates whether TDSRRC_CLOSE_SESSION_REQ is stored or not */
  boolean             close_sess_req_status;

  /* This flag indicates whether TDSRRC_SIG_CONN_RELEASE_REQ is stored or not*/ 
  boolean             rrc_sig_con_rel_status;
  /* Stores the proc_id which sended TDSRRC_SIG_CONN_RELEASE_REQ
   * The same proc_id is used for sending response back to proc */
  tdsrrc_proc_e_type rrc_proc;
}tdsrrcscrr_close_sess_req_type;

typedef struct
{
  /* This flag indicate the status of MUIs for Initial Direct
     Transfer Message */
  boolean             mui_status;
 
  /* This variable to stores the corresponding MUIs of the
    Initial Direct Tranfer message that is transmitted */
  uint8               mui;

   /* This flag indicates whether TDSRRC_OPEN_SESSION_REQ is stored or not */
  boolean             open_sess_req_status;

  /* This struct holds the Initial Direct Transfer Procedure contents 
     which are saved from the OPEN_SESSION_REQ call for  a Domain
   */
  rrc_open_session_req_type             open_session_req;
  
  /* Flag to indicate if L2_ACK for SMC was received before L2_ACK for
     IDT.  In this case, do not send OPEN_SESSION_CNF to MM, as SMC
     would have already sent the OPEN_SESSION_CNF
   */
  boolean             open_sess_cnf_required;
  rrc_establish_cause_e_type  est_cause;
  /*This variable holds the status of whether CU initiated by IDT
   when UE in CELL PCH or URA PCH*/
  boolean cu_init_by_idt;
  /*This variable holds the NAS Selected GSM MAP PLMN Identity which 
    comes as part of OPEN_SESSION_REQ to DT*/
  rrc_plmn_identity_type      selected_plmn_id;

  /*This variable will indicate what type of START value need to be 
   *carried in IDT 
   *TRUE:- IDT should carry START value calculate according to 8.5.9 
   *FASLE:- IDT should carry START value with out calculating according 
   *to 8.5.9. This is set to FALSE for Re-transmitted IDT. For fresh IDT
   *its set to TRUE */
  boolean fresh_start_value_needed;
  
}tdsrrcidt_open_sess_req_type;

typedef struct
{

  boolean   valid; /* indicate the validity */
  rrc_establish_cause_e_type est_cause;
  tdsrrc_proc_e_type   to_be_sent_by;
  boolean               sent_by_cu;
  tdsrrc_proc_e_type  last_proc_sent;
  /*Call Type*/
  rrc_call_type_in_est_req_e_type  call_type;
}tdsrrcidt_est_cause_info_type;

/* This data structure holds all of the regional data for all the direct 
 * transfer procedures.
 */
typedef struct
{
  /* This flag keep track of whether there is a signalling connection open
   * to the CS domain
   */
  boolean cs_domain_open;

  /* This flag keep track of whether there is a signalling connection open
   * to the PS domain
   */
  boolean ps_domain_open;

} tdsrrcdt_int_data_type;


/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcidt_check_if_est_cause_available

DESCRIPTION
  This function is called by CU as well as IDT procedure just before transmitting 
  a over the air message
  to see if establish cause needs to be set.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

boolean tdsrrcidt_check_if_est_cause_available(tdsrrc_proc_e_type proc_id, 
                             rrc_establish_cause_e_type *est_cause
                             ,rrc_call_type_in_est_req_e_type *call_type
                             );
/*===========================================================================

FUNCTION tdsrrcidt_is_cs_call_initiate

DESCRIPTION
  This function is called by CU to confirm if CS call is being initiated.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE : If CS call initiated CU
  FALSE : Else.

===========================================================================*/

boolean tdsrrcidt_is_cs_call_initiate(void );


/*===========================================================================

FUNCTION tdsrrcidt_set_sent_by_cu

DESCRIPTION
  This function is called by CU to say that it has transmitted to the n/w, 
  helpful in case of priority of any cell update
  cause overwrites the cell update cause, uplink data transmission.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcidt_set_sent_by_cu( tdsrrc_proc_e_type proc_id);



/*===========================================================================

FUNCTION tdsrrcdt_init_procedure

DESCRIPTION
  This function initializes everything that the Direct Transfer procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcdt_init_procedure(void);
/*===========================================================================

FUNCTION tdsrrcdt_is_cs_domain_open

DESCRIPTION
  This function returns whether session for CS domain is open or not.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - CS Domain is Open.
  FALSE - CS Domain isn't Open.

===========================================================================*/
boolean tdsrrcdt_is_cs_domain_open(void);
/*===========================================================================

FUNCTION tdsrrcdt_is_ps_domain_open

DESCRIPTION
  This function returns whether session for PS domain is open or not.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - PS Domain is Open.
  FALSE - PS Domain isn't Open.

===========================================================================*/

boolean tdsrrcdt_is_ps_domain_open(void);

/*===========================================================================

FUNCTION tdsrrcidt_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the initial direct transfer procedure.
  The following events are handled by this procedure:

    TDSRRC_OPEN_SESSION_REQ Primitive (received from MM)
    TDSRRC_STATE_CHANGE_IND Primitive( received from State Change Manager )
    TDSRRC_CELL_UPDATE_COMPLETE_CNF Primitive (received from Cell Update Proc)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcidt_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION tdsrrcudt_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the uplink direct transfer procedure.
  The following events are handled by this procedure:

    TDSRRC_DATA_REQ primitive (received from MM)
    TDSRRC_STATE_CHANGE_IND Primitive( received from State Change Manager )
    TDSRRC_CELL_UPDATE_COMPLETE_CNF Primitive (received from Cell Update Proc)
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcudt_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);
/*===========================================================================

FUNCTION tdsrrcscrr_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the signalling connection release
  request procedure. The following events are handled by this procedure:

    TDSRRC_CLOSE_SESSION_REQ primitive (received from MM)
    TDSRRC_STATE_CHANGE_IND Primitive( received from State Change Manager )
    TDSRRC_CELL_UPDATE_COMPLETE_CNF Primitive (received from Cell Update Proc)


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcscrr_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION tdsrrcddt_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the downlink direct transfer
  procedure. The following events are handled by this procedure:

    Downlink Direct Transfer Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcddt_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);
/*===========================================================================

FUNCTION tdsrrcscr_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the signalling connection release
  procedure. The following events are handled by this procedure:

    Signalling Connection Release Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcscr_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION tdsrrcscr_release_signalling_connection

DESCRIPTION
  This function checks whether Signalling connection exist for the particular
  CN Domain and if it exits, then sends RRC_CLOSE_SESSION_IND to the MM. Also
  sets the local variable for signalling connection status to FALSE.


DEPENDENCIES
  None.

RETURN VALUE
  rrcscr_sig_conn_statue_e_type: it returns TDSRRCSCR_SIG_CONN_RELEASED if there
  exits a signalling connection for the CN domain specified in the formal 
  parameter and if RRCSCR procedure successfully sends RRC_CLOSE_SESSION_IND
  to the MM. Otherwise it retuns TDSRRCSCR_SIG_CONN_NOT_EXIST.

===========================================================================*/

tdsrrcscr_sig_conn_rel_status_e_type tdsrrcscr_release_signalling_connection
(
  rrc_cn_domain_identity_e_type cn_domain_id
);

/*===========================================================================

FUNCTION tdsrrcscr_reset_signalling_connection_status

DESCRIPTION
  This function sets both CS and PS signalling Connections to release status .

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcscr_reset_signalling_connection_status
(
  void                    
);
/*===========================================================================

FUNCTION tdsrrcscr_get_signalling_connection_status

DESCRIPTION
  This function gets signalling Connections to release status of Domain.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if there is an active Signalling connection. Otherwise it returns
  FALSE

===========================================================================*/

boolean tdsrrcscr_get_signalling_connection_status
(
  rrc_cn_domain_identity_e_type cn_domain_id                   
);
/*===========================================================================

FUNCTION tdsrrcdt_get_nv_items

DESCRIPTION
  This  function gets NV  item NV_RC_INTEGRITY_ENABLED  and
  NV_RRC_CIPHERING_ENABLED and stores it in local variables
  used to determine if Integrity/Ciphering are supported or
  not

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcdt_get_nv_items(void);
/*===========================================================================

FUNCTION tdsrrcidt_is_saved_open_session_req

DESCRIPTION
  This function returns TRUE if there is saved open_seesion_req for requested CN domain, and FALSE otherwise.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  There is saved open_seesion_req for requested CN domain
  FALSE: Otherwise

===========================================================================*/
boolean tdsrrcidt_is_saved_open_session_req
(
  rrc_cn_domain_identity_e_type cn_domain
);
/*===========================================================================

FUNCTION tdsrrcidt_is_idt_waiting_for_l2ack_per_domain

DESCRIPTION
  This function returns TRUE/FALSE depending if L2_ACK is pending for
  CS or PS domain which is passed as a formal parameter to the fn.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  The CN domain requested is waiting for L2_ACK
  FALSE: The CN domain requested is not waiting for L2_ACK 

===========================================================================*/

boolean tdsrrcidt_is_idt_waiting_for_l2ack_per_domain
(
  rrc_cn_domain_identity_e_type cn_domain
);
/*===========================================================================

FUNCTION tdsrrcidt_send_open_session_cnf  

DESCRIPTION
  This function builds the RRC_OPEN_SESSION_CNF command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the RRC_OPEN_SESSION_CNF command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

void tdsrrcidt_send_open_session_cnf
(
  rrc_open_session_status_e_type      session_status,  /* Success or failure
                                                          status*/
  rrc_cn_domain_identity_e_type       cn_domain_id     /* CN Domain Id */

);

/*===========================================================================


FUNCTION tdsrrcdt_is_idt_waitingfor_l2ack

DESCRIPTION
  This function is used by other procedures like CU to get the state of idt

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
  
boolean tdsrrcdt_is_idt_waitingfor_l2ack(void);

/*===========================================================================

FUNCTION tdsis_ue_oos_and_em_call_attempted

DESCRIPTION
  This function takes UE to idle if UE is OOS (in Connected Mode) and an emergency call 
  is attempted
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: UE is OOS and EM call is attempted. 
  FALSE: otherwise

===========================================================================*/
boolean tdsis_ue_oos_and_em_call_attempted 
(
  tdsrrc_cmd_type    *cmd_ptr, 
  tdsrrc_proc_e_type rrc_proc
);

/*===========================================================================

FUNCTION tdsrrcscrr_is_cn_domain_open

DESCRIPTION
  This function will return the status of CN domain whether its open or closed
  It also checks to see if signalling connection relaese or
  close session request is in progress. This function is used by DDT
  to check whether the domain is open.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE : If the CN Domain is open.
  FALSE: If the CN Domain is closed.

===========================================================================*/
uint8 tdsrrcscrr_is_cn_domain_open
(
rrc_cn_domain_identity_e_type cn_domain
);


/*===========================================================================

FUNCTION tdsrrcdt_check_if_emergency_call_initiated

DESCRIPTION
  This function checks whether emergency call has been initiated for Initial Direct Transfer and Uplink Direct Transfer

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: Emergency call is initiated
  FALSE: Otherwise

===========================================================================*/
boolean tdsrrcdt_check_if_emergency_call_initiated(void);

/*==========================================================================

FUNCTION tdsrrcdt_fill_plmn_id_in_idt

DESCRIPTION
  This function will fill's selected PLMN I.D in
  IDT message.
  .
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcdt_fill_plmn_id_in_idt
(
  /* PLMN I.D to be sent over the air */
  tdsrrc_PLMN_Identity *plmn_id_msg_ie,
  /*CN Domain I.Dfor which PLMN I.D to be filled*/
  rrc_cn_domain_identity_e_type cn_id
);


/*==========================================================================

FUNCTION tdsrrcdt_is_dt_initiated_cu

DESCRIPTION
  .
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcdt_is_dt_initiated_cu
(
  void
);

/*===========================================================================

FUNCTION tdsrrcscr_release_signalling_connection

DESCRIPTION
  This function checks whether Signalling connection exist for the particular
  CN Domain and if it exits, then sends RRC_CLOSE_SESSION_IND/CNF to the MM. Also
  sets the local variable for signalling connection status to FALSE.


DEPENDENCIES
  None.

RETURN VALUE
  rrcscr_sig_conn_statue_e_type: it returns TDSRRCSCR_SIG_CONN_RELEASED if there
  exits a signalling connection for the CN domain specified in the formal 
  parameter and if RRCSCR procedure successfully sends RRC_CLOSE_SESSION_IND
  to the MM. Otherwise it retuns TDSRRCSCR_SIG_CONN_NOT_EXIST.

===========================================================================*/
tdsrrcscr_sig_conn_rel_status_e_type tdsrrcscr_local_release_of_signalling_connection
(
  rrc_cn_domain_identity_e_type cn_domain_id,
  rrc_session_close_cause_e_type cause
);

/*===========================================================================

FUNCTION tdsrrcscrr_delete_close_session_req

DESCRIPTION
  This function deletes the Close Session req data from the static variable
  for the specified CN domain

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcscrr_delete_close_session_req
(
  rrc_cn_domain_identity_e_type cn_domain_id
);

/*===========================================================================

FUNCTION tdsrrcscr_send_close_session_ind  

DESCRIPTION
  This function builds the RRC_CLOSE_SESSION_IND command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the RRC_CLOSE_SESSION_IND command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

tdsrrcdt_status_e_type tdsrrcscr_send_close_session_ind
(
  rrc_cn_domain_identity_e_type       cn_domain_id,     /* CN Domain Id */
  rrc_session_close_cause_e_type      cause

);

/*===========================================================================

FUNCTION tdsrrcscrr_send_sig_conn_release_cnf  

DESCRIPTION
  This function builds the TDSRRC_SIG_CONN_RELEASE_CNF command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the TDSRRC_SIG_CONN_RELEASE_CNF command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

void tdsrrcscrr_send_sig_conn_release_cnf
(
  uecomdef_status_e_type              status  /* Success or failure status*/
#if defined(FEATURE_3GPP_FAST_DORMANCY)
  , tdsrrc_proc_e_type rrc_proc                     /* Dest rrc proc */
#endif
);

/*===========================================================================

FUNCTION tdsrrcscr_indicate_close_session

DESCRIPTION
  This function will check & delete any pending CLOSE_SESSION_REQ for a  domain.
  based on scrr substae it will send either close_session_cnf/ind or sig_conn_rel 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If IDT is buffered for a particular CN domain.
  FALSE:Otherwise.
===========================================================================*/
tdsrrcdt_status_e_type tdsrrcscr_indicate_close_session
(
  rrc_cn_domain_identity_e_type       cn_domain_id,     /* CN Domain Id */
  rrc_session_close_cause_e_type      cause
);
/*===========================================================================

FUNCTION tdsrrcudt_any_saved_data_req_for_cn_domain

DESCRIPTION
  This function will give status of any saved data req for a given CN domain

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:- If any dat req saved of in process for a CN domain
  FALSE:- otherwise

===========================================================================*/
boolean tdsrrcudt_any_saved_data_req_for_cn_domain
(
  rrc_cn_domain_identity_e_type cn_domain_id
);
/*===========================================================================

FUNCTION tdsrrcudt_delete_all_saved_data_req

DESCRIPTION
  This function deletes the Data Req cmd from the static variable
  for the specified CN domain

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcudt_delete_all_saved_data_req
(
  rrc_cn_domain_identity_e_type cn_domain_id
);


/*===========================================================================

FUNCTION tdsrrcidt_update_proc_id

DESCRIPTION
  This function will be called by RCE to update the variable 
  last_proc_sent so that IDT cause can be skipped from filling
  for the first time

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcidt_update_proc_id(void);

/*===========================================================================

FUNCTION   tdsrrcidt_is_idt_active

DESCRIPTION
  This function checks if idt for the given domain is in progress
DEPENDENCIES
  None.

RETURN VALUE
  TRUE indicates that it is active, FALSE otherwise.

===========================================================================*/

boolean tdsrrcidt_is_idt_active
(
   rrc_cn_domain_identity_e_type domain_id
);

#if defined(FEATURE_3GPP_FAST_DORMANCY)
/*===========================================================================

FUNCTION tdsrrcdt_update_scrr_to_handle_dormancy

DESCRIPTION
  This function handles TDSRRC_CLOSE_SESSION_REQ and/or TDSRRC_SIG_CONN_RELEASE_REQ in case 
  TDSRRC_FAST_DORMANCY_REQ is active. 
  It updates the TDSRRC_CLOSE_SESSION_REQ and/or TDSRRC_SIG_CONN_RELEASE_REQ database
  and aborts TDSRRC_FAST_DORMANCY_REQ.

  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: Emergency call is initiated
  FALSE: Otherwise

===========================================================================*/
boolean tdsrrcdt_update_scrr_to_handle_dormancy
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);
#endif
/*===========================================================================

FUNCTION tdsrrcudt_process_l2_ack_cell_udpate_state

DESCRIPTION
   This function is used to handle L2 ack when UDT at cell update state.
   if L2 ACK success, follow non cell udpate state design.
   if L2 ACK failure, reset the status and wait for cell udpate compelete.
  
DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
void tdsrrcudt_process_l2_ack_cell_udpate_state
(
  tdsrrc_cmd_type *cmd_ptr 
);

/*===========================================================================

FUNCTION tdsrrcdt_is_cs_voice_call_domain_open

DESCRIPTION
   This function is used to indicate TDS is in cs voice call or not.
   if TRUE,TDS is in cs domain and voice call
   else return FALSE
  
DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
boolean tdsrrcdt_is_cs_voice_call_domain_open(void);

/*===========================================================================

FUNCTION tdsrrcdt_is_cs_sms_call_domain_open

DESCRIPTION
   This function is used to indicate TDS is in cs sms call or not.
   if TRUE,TDS is in cs domain and sms call
   else return FALSE
  
DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
boolean tdsrrcdt_is_cs_sms_call_domain_open(void);

#endif /* TDSRRCDT_H */

