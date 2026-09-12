#ifndef TDSRRCMCM_H
#define TDSRRCMCM_H
/*===========================================================================
         R R C   M O D E   C O N T R O L   M O D U L E 

DESCRIPTION

  This module is the header module for the mode control module.
  This contains the external interface functions to be used by
  by other RRC modules. This module is only used for dual-mode.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcmcm.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
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
#include "comdef.h"
#include "sys.h"
#include "tdsrrccmd_v.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* define the max numbers of mode change notifications */
#define TDSRRCMCM_MAX_NOTIFICATIONS  TDSMAX_NUM_OF_PROCEDURES

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Enumerated type of the different substates for this procedure           */
typedef enum
{
  TDSRRCMCM_INITIAL,                   /* Initial Substate                    */
  TDSRRCMCM_WAIT_START_CNF,            /* Waiting for START confirmation from
                                       L1                                  */
  TDSRRCMCM_WAIT_STOP_CNF,             /* Waiting for STOP confirmation from
                                       LLC                                 */
  TDSRRCMCM_WAIT_SUSPEND_CNF,          /* Waiting for SUSPEND confirmation
                                       from L1                             */
  TDSRRCMCM_WAIT_RESUME_CNF,           /* Waiting for RESUME confirmation from
                                       L1                                  */
  TDSRRCMCM_WAIT_STOP_CNF_FOR_MODE_CHANGE,
  TDSRRCMCM_MAX_STATES
} tdsrrcmcm_state_e_type;

/* RRC Mode Change Table type                                              */
typedef struct
{
  tdsrrc_mode_e_type from_mode;
  tdsrrc_mode_e_type to_mode;
  tdsrrc_proc_e_type procedure;
} tdsrrcmcm_mode_change_table_type;

/* Command Received Indication Type */
typedef enum
{
  TDSRRCMCM_CMD_NONE,
  TDSRRCMCM_CMD_SERVICE_REQ,
  RRCMCM_CMD_STOP_TDS_REQ,
  TDSRRCMCM_CMD_PLMN_LIST_REQ,
  TDSRRCMCM_CMD_MODE_CHANGE,
  TDSRRCMCM_CMD_INTERRAT_PLMN_SRCH_REQ,
  TDSRRCMCM_CMD_MODE_CHANGE_REQ,
  TDSRRCMCM_CMD_DEACT_REQ,
#ifdef FEATURE_LTE_TO_TDSCDMA
  TDSRRCMCM_CMD_LTE_IRAT_REQ,
  TDSRRCMCM_CMD_LTE_PLMN_SRCH_REQ,
#endif
  TDSRRCMCM_CMD_MAX
} tdsrrcmcm_cmd_flag_e_type;

/* Cause for Mode change to be requested */
typedef enum
{
  TDSRRCMCM_ACT_CAUSE_NONE,
  TDSRRCMCM_INTERRAT_CELL_RESELECTION,
  TDSRRCMCM_INTERRAT_CELL_REDIRECTION,
  RRCMCM_INTERRAT_HANDOVER_G2TD,
  RRCMCM_INTERRAT_CC_ORDER_G2TD,
  RRCMCM_INTERRAT_CC_ORDER_TD2G_FAILED
} tdsrrcmcm_activation_cause;  

/* New types */
/* Enum to indicate a buffered ABORT message from RR */
typedef enum
{
  TDSRRCMCM_RR_ABORT_NONE, 
  TDSRRCMCM_RR_INTERRAT_PLMN_ABORT_REQ,
  TDSRRCMCM_RR_INTERRAT_RESELECTION_ABORT_REQ,
  TDSRRCMCM_RR_INTERRAT_REDIRECT_ABORT_REQ,
#ifdef FEATURE_LTE_TO_TDSCDMA
  TDSRRCMCM_LTE_ABORT_PLMN_SRCH_REQ,
  TDSRRCMCM_LTE_ABORT_RESEL_REQ,
  TDSRRCMCM_LTE_ABORT_REDIR_REQ,
  TDSRRCMCM_LTE_ABORT_CGI_REQ,
#endif
  TDSRRCMCM_RR_ABORT_INVLD   /* exception cases,not used currently */
} tdsrrcmcm_abort_flag_e_type;

/* Struct to save mode/proc of a change_mode 
   when MCM is already processing a change_mode_req  */
typedef struct
{
  tdsrrc_mode_e_type mode;
  tdsrrc_proc_e_type proc;
}tdsrrcmcm_change_mode_req_type;

typedef struct
{
  tdsrrc_cmd_type        rrcmcm_mm_req_cmd;                      /* MM commands */ 
  tdsrrc_rr_cmd_type     rrcmcm_interrat_reselection_req_cmd;    /* Inter-RAT cell reselection command */
  tdsrrcmcm_cmd_flag_e_type  rrcmcm_cmd_flag;                    /* Internal command flag - for deferred 
                                                                 command processing */
  tdsrrcmcm_abort_flag_e_type  rrcmcm_abort_flag;                /* Flag to indicate a pending 
                                                                 BPLMN_SEARCH_ABORT_REQ */ 
  tdsrrcmcm_change_mode_req_type rrcmcm_change_mode_req;         /* change_mode_req mode in case of race-conditions */
#ifdef FEATURE_LTE_TO_TDSCDMA
  tdsrrc_cmd_type rrcmcm_lte_irat_cmd;
#endif
} tdsrrcmcm_cmd_store_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION  RRCMCM_INIT_PROCEDURE

DESCRIPTION

  This function initializes the Mode Contro module.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_init_procedure( void );



/*===========================================================================

FUNCTION RRCMCM_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Mode Control module. This function is called by the
  dispatcher whenever an event is received for this module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrcmcm_event_handler(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);


/*===========================================================================

FUNCTION   RRCMCM_GET_SUBSTATE

DESCRIPTION

  This function retrieves the RRC MCM SUBSTATE.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  Returns tdsrrcmcm_state in tdsrrcmcm_state_e_type

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcmcm_state_e_type tdsrrcmcm_get_substate( void );
/*===========================================================================

FUNCTION  RRCMCM_GET_RRC_MODE

DESCRIPTION

  This function returns the current RRC mode.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Returns RRC mode in tdsrrc_mode_e_type.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_mode_e_type tdsrrcmcm_get_rrc_mode(void);

/*===========================================================================

FUNCTION  RRCMCM_IS_DUALMODE_ENABLED

DESCRIPTION

  This function returns True if inter-RAT operation is enabled, otherwise,
  it returns False.

DEPENDENCIES

  None.

RETURN VALUE

  Return tdsrrc_dual_mode_enabled status in boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_dualmode_enabled(void);

#ifdef FEATURE_CMCC_SECURE_CALL
/*===========================================================================

FUNCTION  TDSRRCMCM_IS_SECURE_CALL_ENABLED

DESCRIPTION

  This function returns True if tdsrrc_is_secure_call_enabled, otherwise,
  it returns False.

DEPENDENCIES

  None.

RETURN VALUE

  Return  tdsrrc_is_secure_call_enabled in boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_secure_call_enabled(void);
/*===========================================================================

FUNCTION  TDSRRCMCM_UPDATE_SECURE_CALL_STATUS

DESCRIPTION

  This function Update tdsrrc_is_secure_call_enabled.

DEPENDENCIES

  None.

RETURN VALUE

  Update tdsrrc_is_secure_call_enabled.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_update_secure_call_status(boolean is_secure_call_enabled);
#endif
/*===========================================================================

FUNCTION  RRCMCM_IS_TDS_ONLY_MODE

DESCRIPTION

  This function returns True if only TDSCDMA mode is enabled, otherwise,
  it returns False.

DEPENDENCIES

  None.

RETURN VALUE

  Return tdsrrcmcm_is_tds_only_mode status in boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_tds_only_mode(void);


/*===========================================================================

FUNCTION  RRCMCM_IS_PROCESS_REQUIRED

DESCRIPTION

  This function checks if the further process for an incoming command is.
  required based the current RRC mode.n If it is required, True is returned, 
  otherwise, False is returned.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_process_required(
tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION  RRCMCM_REGISTER_FOR_MODE_CHANGE

DESCRIPTION

  This function registers a notification when a RRC mode change occurs for a
  given condition. The from_mode is the mode from which the transition occurs 
  abd the to_mode is the mode to which the tansition occurs.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_register_for_mode_change(
  tdsrrc_proc_e_type procedure,
  tdsrrc_mode_e_type from_mode,
  tdsrrc_mode_e_type to_mode
);

/*===========================================================================

FUNCTION  tdsrrcmcm_update_tds_activation_cause

DESCRIPTION

  This function updates the cause of tds activation.  Activation could be
  due to either cell reselection or due to GtoW Handover
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_tds_activation_cause( tdsrrcmcm_activation_cause act_cause );
/*===========================================================================

FUNCTION  tdsrrcmcm_update_dual_mode_enabled_status

DESCRIPTION

  This function updates the status of the flag whether if dual mode is
  enabled or not.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_dual_mode_enabled_status( boolean status );

/*===========================================================================

FUNCTION  RRCMCM_SET_ESTABLISHMENT_CAUSE

DESCRIPTION

  This function sets the global RRC establishment cause appropriately 
  based on the last INTER-RAT GTOW procedure. No change otherwise.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_set_establishment_cause(void);

/*===========================================================================

FUNCTION   RRCMCM_UPDATE_RRC_MODE_TO_ACTIVE

DESCRIPTION

  This function set the current RRC mode to active.Currently called 
  by CHO procedure after the GTOW PCCO is successful.
  
  Only IRAT_IN_PROGRESS to ACTIVE is allowed
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_rrc_mode_to_active(void);

/*===========================================================================

FUNCTION   RRCMCM_SEND_IRAT_ABORT_IND

DESCRIPTION

  This function sends a TDSRRC_IRAT_ABORT_IND to the corresponsing
  Interrat procedure to abort the ongoing Interrat operation 
  inorder to service a higher priority request from MM
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_send_irat_abort_ind(void);

/*===========================================================================
FUNCTION tdsrrcmcm_get_rrc_est_cause

DESCRIPTION
  Returns establishment cause

DEPENDENCIES
  None

RETURN VALUE
  Returns establishment cause

SIDE EFFECTS
  None
===========================================================================*/
rrc_establish_cause_e_type tdsrrcmcm_get_rrc_est_cause(void);


/*===========================================================================
FUNCTION tdsrrcmcm_send_dual_mode_info_to_meas_ueci

DESCRIPTION
  This function will send dual mode info to measuremnt and UE Capability information module
   in the mode status indication primitive

DEPENDENCIES
  None

RETURN VALUE
  Returns None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcmcm_send_dual_mode_info_to_meas_ueci(tdsrrc_cmd_type *cmd_ptr);



#ifdef FEATURE_RAT_PRIORITY_LIST
/*===========================================================================

FUNCTION   RRCMCM_UPDATE_MODE_CAPABILITY

DESCRIPTION

  This function updates GSM, LTE mode capability from NAS message
DEPENDENCIES

  None.

RETURN VALUE

  Returns a boolean value.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_mode_capability
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr
);
/*===========================================================================

FUNCTION  RRCMCM_IS_MODE_ENABLED_IN_RAT_PRI_LIST

DESCRIPTION

  This function returns True if mode passed as argument is enabled in RAT PRI LIST, otherwise FALSE

DEPENDENCIES

  None.

RETURN VALUE

  Return TRUE is LTE mode is enabled in RAT PRI LIST

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_mode_enabled_in_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr,
  sys_sys_mode_e_type mode
);
/*===========================================================================

FUNCTION   RRCMCM_UPDATE_MODE_CAPABILITY_FROM_SERVICE_REQ

DESCRIPTION

  This function updates GSM, LTE mode capability from TDSRRC_SERVICE_REQ.
DEPENDENCIES

  None.

RETURN VALUE

  Returns a boolean value.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_mode_capability_from_service_req
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr
);
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
/*===========================================================================

FUNCTION  RRCMCM_IS_LTE_MODE_ENABLED

DESCRIPTION

  This function returns True if LTE mode is enabled, otherwise FALSE

DEPENDENCIES

  None.

RETURN VALUE

  Return rrc_lte_mode_enabled status in boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcmcm_is_lte_mode_enabled
(
  void
);

/*===========================================================================

FUNCTION  rrcmcm_update_lte_mode_enabled_status

DESCRIPTION

  This function updates the status of the flag whether if LTE mode is
  enabled or not.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_update_lte_mode_enabled_status( boolean status );
#endif /* #if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA) */

#endif

/*===========================================================================

FUNCTION   RRCMCM_SEND_PLMN_LIST_CNF_WITH_FAILURE

DESCRIPTION

  This function sends a RRC_PLMN_LIST_CNF to MM to with the
  proper failure cause.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcmcm_send_plmn_list_cnf_with_failure
( 
  sys_network_selection_mode_e_type network_select_mode , 
  uint8 trans_id
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  , sys_service_search_e_type svc_srch_bgnd
#endif
);

/*===========================================================================

FUNCTION   RRCMCM_RESET_LAST_ACT_CAUSE

DESCRIPTION

  This function resets the mcm activation cause.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcmcm_reset_last_act_cause( void );

/*===========================================================================
FUNCTION tdsrrcmcm_set_reestablish_timer_out

DESCRIPTION
  Set tdsrrc_reestablish_timer_out

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcmcm_set_reestablish_timer_out(  void);
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA 
/*===========================================================================

FUNCTION  TDSRRC_SEND_RLC_TA_STATUS

DESCRIPTION

  This function sends signal 'TDSRLC_UL_TA_IND' to RLC to indicate the START/STOP status of LTA/QTA 

DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_send_rlc_ta_status(tdsrlc_ta_ind_e_type tune_away_ind, tdsrlc_ta_success_e_type tune_away_succ);
#endif/*#FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA */
#endif /* TDSRRCMCM_H */



