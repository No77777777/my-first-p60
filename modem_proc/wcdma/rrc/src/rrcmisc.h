#ifndef RRCMISC_H
#define RRCMISC_H
/*===========================================================================
                  R R C  M I S C  P R O C E D U R E 

DESCRIPTION
  This header file contains function prototypes for the RRC Misc Procedure.
  The RRC Misc Procedure is created for implementation purpose to handle
  processing of unknown or unsupported message type.It also process the Paging
  Control Request command from MM and updates suitable RRC Data.

Copyright (c) 2001, 2003, 2005-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcmisc.h_v   1.1   14 Sep 2001 21:19:02   rjeenaga  $
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcmisc.h#7 $ $DateTime: 2020/07/14 01:35:01 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/14/20   ak      Made changes for FR 61612
04/20/20   gcs     FR59710: Made changes to delay FRLTE and WtoL Resel for MAV
06/24/19   gcs     Made changes for FR 57381
12/04/18   ak      Made changes to modify the rxagc algo.
11/21/15   sg      Made changes to add code for sending REL IND to NAS for FR 53313
11/20/18   ak      Made changes for FR 53313
04/03/17   sas     Changes to take care of missed out global duplications for dual wcdma.
02/19/14   sp      Made changes for Mav debug snapshot
04/02/14   vg      Made changes to Systematically log all the internal and external primitive handled in RRC and corresponding top level action.
01/03/13   ad      Made changes to reduce the redundant F3s
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
05/24/12   sks     Added support for QCHAT specific dynamic DRX.
04/25/12   as      Made changes to trigger cell update/drx req when UE recived OTA with 
                   redirection info and cell reselection in progress
04/10/12   gv      Made changes to remove unused heap allocation and related unused code
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
05/13/10   ss      Fixed compiler warnings.
05/07/10   prk     Fixed compiler warnings.
03/29/10   dm      Made changes for allocating tmc SIB heap.
03/15/10   gkg     Made changes to put changes for "Multiple DRX Cycles" and 
                   "CU wait Timer" under feature R7. 
02/01/10   dm      Added code for handling Multiple DRX.
05/11/09   ps      Feature to dynamically allocate cc/oc/toc/sib_event ptr
05/08/09   ss      Updated Copyright Information
01/19/08   ps      Made changes for ASn1 migration 25.331v7.9  
                   i.e.May 2008 version  (2008-05) 
09/05/06   vm      Added tx to disconnected reason in the function call
                   rrc_transition_to_disconnected_state.
11/09/05   kc      Prototype for taking UE to disconnected state
11/04/05  da/vr    Made changes to detect UE inactivity and take appropriate actions.
08/17/05   vm      Added support for not barring a cell if SIM has not been
                   read (if SIM is present).
                   Added prototype for new function rrcmisc_rrc_rcvd_sim_insert_req().
09/13/01   rj      Updated heading to reflect paging control request processing.
05/16/01   rj      Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "rrccmd_v.h"
#ifdef FEATURE_INACTIVITY_HANDLING
#include "rrcdata_v.h"
#include "uecomdef.h"
#endif /* FEATURE_INTACTIVITY_HANDLING */
#include "rrcrbcommon.h"
#include "rrcueci.h"
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
#include "ds3gpp_api.h"
#endif
#include "rrcrce.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
#define LTE_WRRC_ACQ_DB_MAX_ENTRIES 10
#define MAX_EARFCN_STORED LTE_WRRC_ACQ_DB_MAX_ENTRIES
#endif
/* =======================================================================
**                          Macro Definitions
** ======================================================================= */
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS

#define WRRC_PSEUDO_MEAS_DEBUG_0(FMT) \
do{\
  if(rrcmisc_int_data.pseudo_meas_proc.nv_controls.feature_debug_enabled){\
    WRRC_MSG0_HIGH("W_TO_L_PSEUDO_MEAS:"FMT);\
  }\
}while(0)
#define WRRC_PSEUDO_MEAS_DEBUG_1(FMT,A) \
do{\
  if(rrcmisc_int_data.pseudo_meas_proc.nv_controls.feature_debug_enabled){\
    WRRC_MSG1_HIGH("W_TO_L_PSEUDO_MEAS:"FMT,A);\
  }\
}while(0)
#define WRRC_PSEUDO_MEAS_DEBUG_2(FMT,A,B) \
do{\
  if(rrcmisc_int_data.pseudo_meas_proc.nv_controls.feature_debug_enabled){\
    WRRC_MSG2_HIGH("W_TO_L_PSEUDO_MEAS:"FMT,A,B);\
  }\
}while(0)
#define WRRC_PSEUDO_MEAS_DEBUG_3(FMT,A,B,C) \
do{\
  if(rrcmisc_int_data.pseudo_meas_proc.nv_controls.feature_debug_enabled){\
    WRRC_MSG3_HIGH("W_TO_L_PSEUDO_MEAS:"FMT,A,B,C);\
  }\
}while(0)
#endif /*FEATURE_WCDMA_W2L_PSEUDO_MEAS*/
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


#ifdef FEATURE_WCDMA_REL7_SPECRS 

#define RRCTMR_COUEC_TIMER_IN_MS 300000UL

typedef enum
{
  CHANGE_OF_CAP_REFUSED,
  CHANGE_OF_CAP_ACCEPTED,
  CHANGE_OF_CAP_WITH_RECONFIG
} rrccouec_cnf_e_type;

/*Following enum declares whether RNC supports Change in
    UE capability or not.*/
typedef enum
{
  /*We need to have this enum instead of boolean because we need to differentiate
      between the case when "cap change support' is not given and when "cap chnage support"
      is given as FALSE. */
  RNC_CAP_CHANGE_SUPPORTED,
  RNC_CAP_CHANGE_UNSUPPORTED,
  RNC_CAP_CHANGE_NONE
} rrc_rnc_cap_change_support_e_type;

typedef enum 
{
  RRCCOUEC_INITIAL,               /* COUEC initial substate      */

  RRCCOUEC_WAIT_FOR_UECI_CNF, /*COUEC waits for UECI CNF*/

  RRCCOUEC_WAIT_FOR_COUEC_TIMER_TO_EXPIRE 
                                                 /* COUEC waits for Timer COUEC to expire */
} rrccouec_substate_e_type;
/* Below #defines are added for logging purpose only */
#define rrccouec_substate_e_type_value0 RRCCOUEC_INITIAL
#define rrccouec_substate_e_type_value1 RRCCOUEC_WAIT_FOR_UECI_CNF
#define rrccouec_substate_e_type_value2 RRCCOUEC_WAIT_FOR_COUEC_TIMER_TO_EXPIRE 

typedef struct
{
  rrccouec_substate_e_type substate;
  uint8 num_of_retries;
  rrc_proc_e_type proc_id;
  rrc_ue_cap_update_during_rrc_conn_type ue_cap_change_requested;

  
  /*This global indicates whether Serving RNC supports IE "supportForChangeOfUE-Capability" or not*/
  boolean rrc_rnc_cap_change_support;
  
  /*This variable stores the IE "supportForChangeOfUE-Capability" temporarily until procedure is over
	  successfully*/
  rrc_rnc_cap_change_support_e_type rrc_ordered_rnc_cap_change_support;
  
  /*This structure stores the UE capabilities throughout the RRC Conn Setup.*/
  rrc_ue_cap_update_during_rrc_conn_type rrc_ue_cap_during_rrc_conn;
} rrccouec_int_data_type;

#ifdef FEATURE_DUAL_WCDMA
extern rrccouec_int_data_type rrccouec_sim[MAX_RRC_STACKS];
#define rrccouec rrccouec_sim[WAS_ID_M]
#else /* FEATURE_DUAL_WCDMA */
extern rrccouec_int_data_type rrccouec;
#endif

#endif /*FEATURE_WCDMA_REL7_SPECRS*/


typedef enum 
{
  RRCMISC_MULTIPLE_DRX_INITIAL,               /* MISC Multiple DRX initial substate      */
  RRCMISC_WAIT_FOR_BPLMN_SUSPEND_CNF, /*MISC waits for BPLMN SUSPEND CNF*/
} rrcmisc_substate_e_type;

#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
typedef enum
{
  RRC_PSEUDO_MEAS_INITIAL,
  RRC_PSEUDO_MEAS_WAIT_FOR_RESULTS,
  RRC_PSEUDO_MEAS_WAIT_FOR_STATE_CHANGE,
}rrc_pseudo_meas_substate_e_type;

typedef enum
{
  WRRC_PSEUDO_MEAS_HANDLE_NOW,
  WRRC_PSEUDO_MEAS_DEFER,
  WRRC_PSEUDO_MEAS_IGNORE
}rrc_pseudo_meas_ind_act_e_type;

typedef struct
{
  void* encoded_ptr;
  rrc_SysInfoType19* decoded_ptr;
  uint16 length;
}rrc_sib_19_s_type;

typedef struct
{
  
 uint8                     num_earfcn_count;

 lte_rrc_acq_db_list_s earfcn_list[MAX_EARFCN_STORED];
  
}wcdma_saved_lte_acq_db_earfcn_s;

typedef struct
{
  uint32  t_search_tmr; /* Time UE spends in measuring LTE cell */
  uint32  t2_wait_tmr; /* If measured LTE cells are bad, wait time before retrying measurements */
  int32   lte_rsrq; /*RSRQ threshold to consider a LTE cell for redirection  */
  int32   lte_rsrp; /*RSRP threshold to consider a LTE cell for redirection  */
  boolean feature_enabled; /* Feature control */
  boolean feature_debug_enabled; /* Feature Debug control */
  
}rrc_pseudo_meas_nv_control_type;

typedef struct
{
   wcdma_saved_lte_acq_db_earfcn_s stored_lte_acq_db;
   rrc_sib_19_s_type sib_19;
   l1_pseudo_meas_results_type results;
   rrc_pseudo_meas_nv_control_type nv_controls;
   rrc_pseudo_meas_substate_e_type substate;
   uint8 allocated_tgpsi;
   int16   curr_rxagc;
   uint16   counter;
   int16    prev_rxagc;
}rrc_pseudo_meas_proc_s_type;

#endif /*FEATURE_WCDMA_W2L_PSEUDO_MEAS*/
typedef struct{
  uint16 curr_freq;
  uint16 curr_psc;
  uint16 prev_freq;
  uint16 prev_psc;
  uint8  curr_t_value;
  uint8  prev_t_value;
  boolean depri_req_sent;
}rrc_cell_depr_db_s_type;

typedef struct
{

#ifdef FEATURE_INACTIVITY_HANDLING
  rlc_data_activity_type rlc_data_activity_ind;
#endif /* FEATURE_INTACTIVITY_HANDLING */

  boolean rrc_rcvd_sim_ins_req;
  
  /* Stores the current state of MISC for Multiple DRX */
  rrcmisc_substate_e_type  rrcmisc_multiple_drx_substate;
  
  boolean rrc_high_mobility_ind_in_rcr;

  boolean rrc_clear_sibdb_for_lpm;
	
  boolean is_emergency_call;
  boolean is_delay_wtol_resel_timer_expired;
/* QCHAT specific DRX coeff which UE should use after a successful LAU/RAU */
sys_drx_cn_coefficient_s1_e_type  rrc_qchat_drx_coeff;

#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS

  rrc_pseudo_meas_proc_s_type pseudo_meas_proc;
#endif

  rrc_cell_depr_db_s_type rrc_cell_depri_db;
}rrcmisc_int_data_type;

#ifdef FEATURE_DUAL_WCDMA
extern rrcmisc_int_data_type rrcmisc_int_data_sim[MAX_RRC_STACKS];
#define rrcmisc_int_data rrcmisc_int_data_sim[WAS_ID_M]
#else /* FEATURE_DUAL_WCDMA */
extern rrcmisc_int_data_type rrcmisc_int_data;
#endif



/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION rrcmisc_rrc_rcvd_sim_insert_req  

DESCRIPTION
  This function is currently called by CSP before a cell bar request is 
  sent to L1. If the SIM has not been read, then cell bar request is not
  sent to L1. The reason being that it is possible that the cell UE attempts
  to camp on is reserved for Operator use. If UE has high access class(es) 
  present then the cell should not be barred else the cell should be barred
  for 1280s (~21mins). Till SIM is read it is not known whether High ACs are 
  present or not and hence such a cell is not barred and in addition UE
  does not camp on this cell (during cell selection processs only and not
  due to cell access restrictions change via SIB modification). 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: SIM has been read
  FALSE: SIM has not been read

===========================================================================*/
boolean rrcmisc_rrc_rcvd_sim_insert_req(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION RRC_TRANSITION_TO_DISCONNECTED_STATE

DESCRIPTION

  This function is used to send initiate cell selection command to CSP
  with destination state as disconnected. CSP on receiving this command
  releases all channels and takes UE to idle.
  The reason why UE is being taken to Idle is also sent to CSP. E.g., is
  tx to disconnected due to an emergency call when UE is OOS in Connected Mode
  or whether it is due to RL failure with T314/T315 = 0, etc.
  
DEPENDENCIES

  None

RETURN VALUE
none

SIDE EFFECTS

  None

===========================================================================*/
void rrc_transition_to_disconnected_state
(    
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_proc_e_type rrc_proc,
  rrc_tx_to_discon_reason_e_type tx_to_discon_reason
);
/*===========================================================================

FUNCTION rrcmisc_init_procedure

DESCRIPTION
  This function initializes everything that the RRC Misc procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void rrcmisc_init_procedure(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION rrcmisc_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the RRC Misc procedure.
  The following events are handled by this procedure:

    RRC_DOWNLINK_SDU_IND Primitive (received from RLC)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void rrcmisc_procedure_event_handler(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr);


#ifdef FEATURE_INACTIVITY_HANDLING

/*===========================================================================

FUNCTION RRCMISC_HANDLE_PS_RAB_INACTIVITY

DESCRIPTION
  Get/Reset Inactivity flag in RLC
  
DEPENDENCIES
  None.

RETURN VALUE
  Success : If values were successfully set/reset
  Failue  : Future use
  
===========================================================================*/

uecomdef_status_e_type rrcmisc_handle_ps_rab_inactivity(sys_modem_as_id_e_type wrrc_as_id, uint8 instance_id);


/*===========================================================================

FUNCTION RRCMISC_CHECK_FOR_PS_RAB_INACTIVITY

DESCRIPTION
  Check for PS RAB Inactivity based on the status returned by RLC
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE -  If Inactivity is detected
  FALSE - If PS RABs are active

===========================================================================*/
boolean rrcmisc_check_for_ps_rab_inactivity(sys_modem_as_id_e_type wrrc_as_id );


#endif /* FEATURE_INTACTIVITY_HANDLING */

/*===========================================================================

FUNCTION rrcmisc_initiate_cell_update

DESCRIPTION
  This procedure initiates the Cell Update procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrcmisc_initiate_cell_update(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION rrcmisc_is_emergency_call_ongoing

DESCRIPTION
  This function returns if the ongoing call is emergency call

DEPENDENCIES
  None.

RETURN VALUE
  Boolean
===========================================================================*/

boolean rrcmisc_is_emergency_call_ongoing(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION rrcmisc_process_T320_expiry

DESCRIPTION
  This procedure handles the Expiry of timer T320.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrcmisc_process_T320_expiry(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION rrc_cell_pch_ura_pch_handler

DESCRIPTION
  Used to handle state change to/from Cell PCH (or URA PCH)
  
DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/

void rrcmisc_state_change_handler
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr
);


#ifdef FEATURE_WCDMA_REL7_SPECRS 
/*===========================================================================

FUNCTION rrccouec_is_proc_active

DESCRIPTION
  This function tells whether COUEC procedure is active or not.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
boolean rrccouec_is_proc_active
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrccouec_process_completion_of_ueci_req

DESCRIPTION
  This function is the procedure  handler of COUEC procedure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void rrccouec_process_completion_of_ueci_req
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrccouec_cnf_e_type ueci_cnf
);

/*===========================================================================

FUNCTION rrccouec_process_completion_cap_change_with_reconfig

DESCRIPTION
  This function is called from Reconfig procedures to notify that cap change is complete. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_process_completion_cap_change_with_reconfig
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrccouec_clear_procedure

DESCRIPTION
  This function clears the COUEC  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_clear_procedure
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrccouec_process_initial_substate

DESCRIPTION
  This function is the substate event handler of COUEC procedure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_process_initial_substate
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION rrccouec_process_wait_for_timer_couec_to_expire

DESCRIPTION
  This function is the substate event handler of COUEC procedure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_process_wait_for_timer_couec_to_expire
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION rrccouec_process_wait_for_ueci_cnf

DESCRIPTION
  This function is the substate event handler of COUEC procedure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_process_wait_for_ueci_cnf
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION rrccouec_init_procedure

DESCRIPTION
  This function initializes everything that the RRC COUEC procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_init_procedure
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrccouec_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the RRC COUEC procedure.
  The following events are handled by this procedure:

    RRC_DOWNLINK_SDU_IND Primitive (received from RLC)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_procedure_event_handler
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION rrccouec_save_ue_cap

DESCRIPTION
  This function saves the requested UE  capabilities. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_save_ue_cap
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrccouec_send_couec_cnf

DESCRIPTION
  This function sends  RRC_COUEC_CNF comamnd to the requested procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrccouec_send_couec_cnf
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_couec_cnf_e_type couec_cnf
);

/*===========================================================================

FUNCTION rrc_save_rnc_cap_change_support

DESCRIPTION
 This function updates global "rrc_rnc_cap_change_support" based on the OTA IE value.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rrc_save_rnc_cap_change_support
(
sys_modem_as_id_e_type wrrc_as_id
);
#endif/*FEATURE_WCDMA_REL7_SPECRS*/

/*====================================================================
FUNCTION: rrc_init_dynamic_ptrs()

DESCRIPTION:
  This function is called to allocate OC, CC and TOC pointers, sib heap

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void rrc_init_dynamic_ptrs(sys_modem_as_id_e_type wrrc_as_id
    
    );
/*====================================================================
FUNCTION: rrcllc_free_config_ptrs()

DESCRIPTION:
  This function is called to free the allocated OC, CC and TOC pointers. sib

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void rrc_free_dynamic_ptrs(
  sys_modem_as_id_e_type wrrc_as_id
  );

/*===========================================================================

FUNCTION          RRCCSP_CHECK_INITIATE_CELL_SELECTION_HANDLING

DESCRIPTION       This function is called to check if CSP is in proper substate to handle 
                         RRC_INITIATE_CELL_SELECTION_IND to transition to DCH or FACH.

DEPENDENCIES      None

RETURN VALUE      TRUE/FALSE

SIDE EFFECTS      None
===========================================================================*/
boolean rrccsp_check_initiate_cell_selection_handling
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrcrb_directed_cell_info_type *directed_cell_info
);
#endif /* RRCMISC_H */

/*====================================================================
FUNCTION: rrc_get_qchat_specific_drx_cycle_lenght()

DESCRIPTION:
  This function is called to get the QCHAT specific DRX cycle length.

DEPENDENCIES:
  None.

RETURN VALUE:
  DRX cycle length
  
SIDE EFFECTS:

====================================================================*/
uint32 rrc_get_qchat_specific_drx_cycle_length(sys_modem_as_id_e_type wrrc_as_id);

/*====================================================================
FUNCTION: rrccouec_return_substate()

DESCRIPTION:
 returns substate

DEPENDENCIES:
  None.

RETURN VALUE:
  rrccouec_substate_e_type
  
SIDE EFFECTS:

====================================================================*/

//rrccouec_substate_e_type rrccouec_return_substate(void);
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
/*===========================================================================

FUNCTION rrc_pseudo_meas_triggered_conn_abort_req()

DESCRIPTION

  This function will check if fake meas proc has triggered connection abort req
  
DEPENDENCIES

  None

RETURN VALUE

  TRUE if fake meas has triggered connection abort

SIDE EFFECTS

  None

===========================================================================*/

boolean rrc_pseudo_meas_triggered_conn_abort_req
(
  sys_modem_as_id_e_type wrrc_as_id
);
/*===========================================================================

FUNCTION rrc_pseudo_meas_procedure_event_handler()

DESCRIPTION

  This function will process the throughput low indicataion and configure fake L measurements
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void rrc_pseudo_meas_procedure_event_handler(sys_modem_as_id_e_type wrrc_as_id,rrc_cmd_type *cmd_ptr);
/*===========================================================================

FUNCTION rrc_pseudo_meas_init_procedure

DESCRIPTION
  This function initializes required parameters for fake meas procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_pseudo_meas_init_procedure
(
  sys_modem_as_id_e_type wrrc_as_id
);
/*===========================================================================
FUNCTION rrcrce_is_rce_procedure_not_in_intial_or_final_substate

DESCRIPTION
  This function looks into RRC Connection Establishment provcedure's 
  sub-states and returns the status of RRC Connection Establishment
  Procedure.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if RCE is in active. 
  FALSE: Otherwise 
SIDE EFFECTS
none
===========================================================================*/
extern boolean  rrcrce_is_rce_procedure_not_in_intial_or_final_substate(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION rrc_meas_save_allocated_tgpsi()

DESCRIPTION

  This function saves the tgpsi allocated for w to l measurements
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void rrc_pseudo_meas_save_allocated_tgpsi(sys_modem_as_id_e_type wrrc_as_id,uint8 tgpsi_allocated);
/*===========================================================================

FUNCTION rrc_meas_get_allocated_tgpsi()

DESCRIPTION

  This function returns the tgpsi allocated for w to l measurements
  
DEPENDENCIES

  None

RETURN VALUE

  tgpsi used for fake measurements

SIDE EFFECTS

  None

===========================================================================*/

uint8 rrc_pseudo_meas_get_allocated_tgpsi(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION rrc_pseudo_meas_populate_redirection_results()

DESCRIPTION

  This function will populate the redirection results to CSP pointer
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void rrc_pseudo_meas_fetch_redirection_results
(
  sys_modem_as_id_e_type wrrc_as_id,
   rrc_redirect_req_type *redir_ptr
);
/*===========================================================================

FUNCTION rrc_check_and_suspend_pseudo_meas_procedure()

DESCRIPTION

  This function will populate the redirection results to CSP pointer
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void rrc_check_and_suspend_pseudo_meas_procedure
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_proc_e_type procedure
);
/*===========================================================================

FUNCTION rrc_pseudo_meas_send_lte_acq_db_earfcn_req ()

DESCRIPTION

  This function sends LTE ACQ DB EARFCN REQ to LTE RRC 
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void rrc_pseudo_meas_send_lte_acq_db_earfcn_req
(
  sys_modem_as_id_e_type wrrc_as_id
);
/*===========================================================================

FUNCTION rrc_pseudo_meas_populate_lte_acq_db_earfcns()

DESCRIPTION

  This function will populate the acq db entries to meas pointer 
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void rrc_pseudo_meas_populate_lte_acq_db_earfcns
(
  sys_modem_as_id_e_type wrrc_as_id,
  l1_meas_eutra_frequency_list_type* freq_list_ptr
);



/*===========================================================================

FUNCTION rrc_run_pseudo_meas_algorithm()

DESCRIPTION

  This function increments counter for the pseudo meas algo. 
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_run_pseudo_meas_algorithm
(
  sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrc_start_pseudo_meas_tmr()

DESCRIPTION

  This function starts the pseudo meas algo. 
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_start_pseudo_meas_tmr
(
  sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrc_clear_pseudo_meas_results()

DESCRIPTION

  This function clears the results after redirection to LTE. 
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/
void rrc_clear_pseudo_meas_results(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION rrc_pseudo_meas_handle_mode_change

DESCRIPTION

  This function starts or stop the pseudo meas algo on mode change. 
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_pseudo_meas_handle_mode_change(sys_modem_as_id_e_type wrrc_as_id,rrc_cmd_type *cmd_ptr);


/*===========================================================================

FUNCTION rrc_pseudo_meas_start_algo_on_capability_change

DESCRIPTION

  This function starts pseudo meas algo on mode capability change. 
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_pseudo_meas_start_algo_on_capability_change(sys_modem_as_id_e_type wrrc_as_id);


/*===========================================================================

FUNCTION wrrc_post_throughput_low_ind

DESCRIPTION

  This function will post throughput low indication to internal command queue
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void wrrc_post_pseudo_meas_ind(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION rrc_pseudo_meas_populate_sib19_earfcns()

DESCRIPTION

  This function will populate the acq db entries to meas pointer 
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void rrc_pseudo_meas_populate_sib19_earfcns
(
  sys_modem_as_id_e_type wrrc_as_id,
  l1_meas_eutra_frequency_list_type* freq_list_ptr
);


/*===========================================================================

FUNCTION rrc_pseudo_meas_save_lte_acq_db_entries()

DESCRIPTION

  This function will save the LTE acq db earfcns. This will be called whenever LTE sends a EARFCN response. 
  We store only the top 8 entries.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/


 void rrc_pseudo_meas_save_lte_acq_db_entries
(
  sys_modem_as_id_e_type wrrc_as_id,
  lte_rrc_acq_db_earfcn_rsp_s* earfcn_list_ptr
);

/*===========================================================================
  FUNCTION     rrc_pseudo_meas_get_rsrp

  DESCRIPTION
  This function reports rsrp value to L1 from NV configured 

  DEPENDENCIES
  None

  RETURN VALUE
  None
  
  SIDE EFFECTS
  None
  ===========================================================================*/
int16 rrc_pseudo_meas_get_thresh_rsrp(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================
  FUNCTION     rrc_pseudo_meas_get_rsrq

  DESCRIPTION
  This function reports rsrp value to L1 from NV configured 

  DEPENDENCIES
  None

  RETURN VALUE
  None
  
  SIDE EFFECTS
  None
  ===========================================================================*/
int16 rrc_pseudo_meas_get_thresh_rsrq(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION rrc_pseudo_meas_get_substate

DESCRIPTION

  This function will populate the redirection results to CSP pointer
  
DEPENDENCIES

  None

RETURN VALUE

  rrc_pseudo_meas_substate_e_type

SIDE EFFECTS

  None

===========================================================================*/

rrc_pseudo_meas_substate_e_type rrc_pseudo_meas_get_substate(sys_modem_as_id_e_type wrrc_as_id);

#ifdef ELEVATOR_TRIGGER
/*===========================================================================

FUNCTION rrc_pseudo_meas_process_prioritize_lte_req()

DESCRIPTION

  This function handles WCDMA_RRC_QMI_PRIORITIZE_LTE_REQ message.
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/

boolean rrc_pseudo_meas_process_prioritize_lte_req
(
  sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrc_check_rxagc_criteria_and_post_pseudo_meas_ind()

DESCRIPTION

  This function checks if the delta Rxagc is above a certain threshold.
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_check_rxagc_criteria_and_post_pseudo_meas_ind
(
  sys_modem_as_id_e_type wrrc_as_id
);


/*===========================================================================

FUNCTION rrc_start_rxagc_timer_on_mode_change()

DESCRIPTION

 This function starts the Rxagc timer.
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_start_rxagc_timer_on_mode_change
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_cmd_type *cmd_ptr  
);

/*===========================================================================

FUNCTION rrc_pseudo_meas_rxagc_init_procedure

DESCRIPTION
  This function registers the RXAGC procedure to the state change.

DEPENDENCIES
  None.

RETURN VALUE
  None.

========================================================================*/

void rrc_pseudo_meas_rxagc_init_procedure(sys_modem_as_id_e_type wrrc_as_id );

#endif

/*===========================================================================

FUNCTION rrc_pseudo_meas_clear_forbidden_earfcn()

DESCRIPTION

  This function clears the saved forbidden earfcn list. 
  
DEPENDENCIES

  None

RETURN VALUE
 

SIDE EFFECTS

  None

===========================================================================*/
void rrc_pseudo_meas_clear_forbidden_earfcn(sys_modem_as_id_e_type wrrc_as_id);

#endif /*FEATURE_WCDMA_W2L_PSEUDO_MEAS*/

/*===========================================================================

FUNCTION   rrc_send_depri_req

DESCRIPTION

  This function sends CPHY_CELL_DEPRI_REQ to L1
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_send_depri_req(sys_modem_as_id_e_type wrrc_as_id,boolean depri_cell,boolean serv_cell);
/*===========================================================================

FUNCTION          rrcssgccs_cell_depri_req_info

DESCRIPTION       This function is called by SSGCCS to send depriortization info for a particular cell

DEPENDENCIES      NONE

RETURN VALUE      NONE

SIDE EFFECTS      NONE

===========================================================================*/
void rrcssgccs_cell_depri_req_info(sys_modem_as_id_e_type wrrc_as_id, wcdma_ssgccs_rrc_cell_depr_info_s ssgccs_depr_info);
/*===========================================================================

FUNCTION          rrc_init_cell_depr_info

DESCRIPTION       Resets depri db

DEPENDENCIES      NONE

RETURN VALUE      NONE

SIDE EFFECTS      NONE

===========================================================================*/
void rrc_init_cell_depr_info(sys_modem_as_id_e_type wrrc_as_id,rrc_cmd_type *cmd_ptr);
/*===========================================================================

FUNCTION          rrc_init_cell_depr_info_procedure

DESCRIPTION       Initialize CELL DEPRI DB

DEPENDENCIES      NONE

RETURN VALUE      NONE

SIDE EFFECTS      NONE

===========================================================================*/
void rrc_init_cell_depr_info_procedure(sys_modem_as_id_e_type wrrc_as_id);

