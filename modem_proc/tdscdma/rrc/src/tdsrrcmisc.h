#ifndef TDSRRCMISC_H
#define TDSRRCMISC_H
/*===========================================================================
                  R R C  M I S C  P R O C E D U R E 

DESCRIPTION
  This header file contains function prototypes for the RRC Misc Procedure.
  The RRC Misc Procedure is created for implementation purpose to handle
  processing of unknown or unsupported message type.It also process the Paging
  Control Request command from MM and updates suitable RRC Data.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcmisc.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
11/10/10   bj       Deleted IE "RNC support for change of UE capability".
                          (3GPP: 8.6.2.3)
11/10/10   bj       Delete IE "Acceptance of requested change of capability".
                          (3GPP: 8.1.6.4)

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "tdsrrccmd_v.h"

#include "tdsrrcueci.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


extern boolean tdsrrc_high_mobility_ind_in_rcr;

/* FR19720 2min timer for band F avoidance */
#define TDSRRCISC_AVOID_BAND_F_TIME_MS  120000

#ifdef FEATURE_TDSCDMA_REL7_SPECRS 

#define TDSRRCTMR_COUEC_TIMER_IN_MS 300000UL

typedef enum
{
  TDSCHANGE_OF_CAP_REFUSED,
  TDSCHANGE_OF_CAP_ACCEPTED,
  TDSCHANGE_OF_CAP_WITH_RECONFIG
} tdsrrccouec_cnf_e_type;

/*Following enum declares whether RNC supports Change in
    UE capability or not.*/
typedef enum
{
  /*We need to have this enum instead of boolean because we need to differentiate
      between the case when "cap change support' is not given and when "cap chnage support"
      is given as FALSE. */
  TDSRNC_CAP_CHANGE_SUPPORTED,
  TDSRNC_CAP_CHANGE_UNSUPPORTED,
  TDSRNC_CAP_CHANGE_NONE
} tdsrrc_rnc_cap_change_support_e_type;

typedef enum 
{
  TDSRRCCOUEC_INITIAL,               /* COUEC initial substate      */

  TDSRRCCOUEC_WAIT_FOR_UECI_CNF, /*COUEC waits for UECI CNF*/

  TDSRRCCOUEC_WAIT_FOR_COUEC_TIMER_TO_EXPIRE 
                                                 /* COUEC waits for Timer COUEC to expire */
} tdsrrccouec_substate_e_type;

typedef struct
{
  tdsrrccouec_substate_e_type substate;
  uint8 num_of_retries;
  tdsrrc_proc_e_type proc_id;
  tdsrrc_ue_cap_update_during_rrc_conn_type ue_cap_change_requested;
} tdsrrccouec_int_data_type;


extern tdsrrccouec_int_data_type tdsrrccouec;
extern boolean tdsrrc_rnc_cap_change_support;
extern tdsrrc_ue_cap_update_during_rrc_conn_type tdsrrc_ue_cap_during_rrc_conn;
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/



typedef enum 
{
  TDSRRCMISC_MULTIPLE_DRX_INITIAL,               /* MISC Multiple DRX initial substate      */
  TDSRRCMISC_WAIT_FOR_BPLMN_SUSPEND_CNF, /*MISC waits for BPLMN SUSPEND CNF*/
} tdsrrcmisc_substate_e_type;



/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcmisc_rrc_rcvd_sim_insert_req  

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
boolean tdsrrcmisc_rrc_rcvd_sim_insert_req(void);

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

  SUCCESS:   command successfully sent
  ERR-FATAL: command could not be sent

SIDE EFFECTS

  None

===========================================================================*/
uecomdef_status_e_type tdsrrc_transition_to_disconnected_state
(    
  tdsrrc_proc_e_type rrc_proc,
  tdsrrc_tx_to_discon_reason_e_type tx_to_discon_reason
);
/*===========================================================================

FUNCTION tdsrrcmisc_init_procedure

DESCRIPTION
  This function initializes everything that the RRC Misc procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcmisc_init_procedure(void);

/*===========================================================================

FUNCTION tdsrrcmisc_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the RRC Misc procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcmisc_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);


/*===========================================================================

FUNCTION tdsrrcmisc_initiate_cell_update

DESCRIPTION
  This procedure initiates the Cell Update procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcmisc_initiate_cell_update(void);

/*===========================================================================

FUNCTION tdsrrcmisc_process_T320_expiry

DESCRIPTION
  This procedure handles the Expiry of timer T320.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcmisc_process_T320_expiry(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION rrc_cell_pch_ura_pch_handler

DESCRIPTION
  Used to handle state change to/from Cell PCH (or URA PCH)
  
DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/

void tdsrrcmisc_state_change_handler
(
  tdsrrc_cmd_type *cmd_ptr
);

#ifdef FEATURE_TDSCDMA_REL7_SPECRS 


/*===========================================================================

FUNCTION tdsrrccouec_clear_procedure

DESCRIPTION
  This function clears the COUEC  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_clear_procedure
(
  void
);

/*===========================================================================

FUNCTION tdsrrccouec_process_initial_substate

DESCRIPTION
  This function is the substate event handler of COUEC procedure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_process_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION tdsrrccouec_process_wait_for_timer_couec_to_expire

DESCRIPTION
  This function is the substate event handler of COUEC procedure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_process_wait_for_timer_couec_to_expire
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION tdsrrccouec_process_wait_for_ueci_cnf

DESCRIPTION
  This function is the substate event handler of COUEC procedure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_process_wait_for_ueci_cnf
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);

/*===========================================================================

FUNCTION tdsrrccouec_init_procedure

DESCRIPTION
  This function initializes everything that the RRC COUEC procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_init_procedure
(
  void
);

/*===========================================================================

FUNCTION tdsrrccouec_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the RRC COUEC procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION tdsrrccouec_save_ue_cap

DESCRIPTION
  This function saves the requested UE  capabilities. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_save_ue_cap
(
  void
);

/*===========================================================================

FUNCTION tdsrrccouec_send_couec_cnf

DESCRIPTION
  This function sends  TDSRRC_COUEC_CNF comamnd to the requested procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_send_couec_cnf
(
  tdsrrc_couec_cnf_e_type couec_cnf
);

/*===========================================================================

FUNCTION tdsrrc_save_rnc_cap_change_support

DESCRIPTION
 This function updates global "tdsrrc_rnc_cap_change_support" based on the OTA IE value.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrc_save_rnc_cap_change_support
(
  void
);
#endif/*FEATURE_TDSCDMA_REL7_SPECRS*/

/*====================================================================
FUNCTION: tdsrrc_init_dynamic_ptrs()

DESCRIPTION:
  This function is called to allocate OC, CC and TOC pointers, sib heap

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrc_init_dynamic_ptrs(
    void
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
void tdsrrc_free_dynamic_ptrs(
  void
  );

/*===========================================================================

FUNCTION tdsrrcmisc_check_da_backoff

DESCRIPTION
  FR19720. This function checks dual active RF backoff condition.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: back-off conditions are met.
  FALSE: not back-off condition.

===========================================================================*/
boolean tdsrrcmisc_check_da_backoff(void);

/*====================================================================
FUNCTION: tdsrrcmisc_da_backoff_timer_expired()

DESCRIPTION:
  This function process dual active RF backoff expiration.

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmisc_da_backoff_timer_expired(void);

/*====================================================================
FUNCTION: tdsrrcmisc_is_da_backoff_on()

DESCRIPTION:
  This function checks dual active RF backoff flag.

DEPENDENCIES:
  None.

RETURN VALUE:
  Boolean.
  
SIDE EFFECTS:

====================================================================*/
boolean tdsrrcmisc_is_da_backoff_on(void);

/*===========================================================================

FUNCTION tdsrrcmisc_is_in_band_F

DESCRIPTION
  This function checks if the input frequency is in band F (B39).

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: in band F.
  FALSE: not in band F.

===========================================================================*/
boolean tdsrrcmisc_is_in_band_F(uint16 freq);
#ifdef FEATURE_TDSCDMA_FATAL_RECOVERY 
/*====================================================================
FUNCTION: tdsrrcmisc_procss_hw_error_ind()

DESCRIPTION:
  This function process HW error from L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmisc_procss_hw_error_ind(void);
#endif

/*====================================================================
FUNCTION: tdsrrcmisc_process_qmi_drx_level_req()

DESCRIPTION:
  This function process drx level req from qmi.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmisc_process_qmi_drx_level_req
(
  sys_modem_as_id_e_type as_id,
  uecomdef_status_e_type simid_validation
);
#endif /* TDSRRCMISC_H */

