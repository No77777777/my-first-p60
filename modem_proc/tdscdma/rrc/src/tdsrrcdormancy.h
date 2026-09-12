/*===========================================================================
                          R R C  FAST  DORMANCY

DESCRIPTION

 This module defines rrc fast dormancy feature in RRC.

 This module has functionality related to 3GPP defined Fast 
 Dormancy implementation & switch back to proprietary Fast 
 Dormancy in case NW doesnt support.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

  Queues must be initialized by calling tdsrrc_init_command_queues()
  at RRC task startup.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcdormancy.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/20/12   hx      Enhance protection for TDS receiving TDSRRC_FAST_DORMANCY_REQ from NAS
08/15/11   hx      Enable protection for TDS receiving TDSRRC_FAST_DORMANCY_REQ from NAS
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
02/15/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.91.50
01/31/11   ad      Fixed compilation warnings
08/21/10   ss      Fixed compilation warnings.
===========================================================================*/

#include "tdscdma_variation.h"
#if   defined FEATURE_3GPP_FAST_DORMANCY



/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

#include "tdsuecomdef.h"

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Provides the duration for RRC Fast Dormancy URA PCH Guard timer in milliseconds 
  * RRC will defer processing of dormancy request if this timer is runnning in URA/PCH States.
*/
#define TDSRRCDORM_URA_PCH_GUARD_TIMER_VALUE     5000
#define TDSRRCDORM_SRNS_GUARD_TIMER_VALUE        5000
#define TDSRRCDORM_STOP_T323_TIMER_VALUE         0


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef enum 
{
  TDSRRCDORMANCY_INITIAL,                         /* RRCDORMANCY initial sub state */
  TDSRRCDORMANCY_WAIT_FOR_CU_COMP_CNF,            /* RRCDORMANCY waits for confirmation from Cell 
                                                                                       Update Procedure complete */ 
  TDSRRCDORMANCY_WAIT_FOR_L2ACK,                  /* RRCDORMANCY waits for l2 ACK for Initial Direct
                                                                                       Transfer message */
  TDSRRCDORMANCY_WAIT_FOR_SIG_CON_REL_COMPL       /* RRCDORMANCY waits for Signalling Connection Release 
                                                                                    * Needed for fall back to legacy dormancy.*/                                                            
} tdsrrcdormancy_substate_e_type;


typedef enum
{
  TDSRRCDORMANCY_VALIDATION_SUCCESS,                  /* Successful Status */      
  TDSRRCDORMANCY_VALIDATION_FAIL_DOMAIN_NOT_OPEN,     /* Failure Status: PS domain not open */
  TDSRRCDORMANCY_VALIDATION_FAIL_CS_DOMAIN_OPEN,      /* Failure Status: CS domain is also open */
  TDSRRCDORMANCY_VALIDATION_FAIL_RRC_CON_NOT_OPEN,    /* Failure Status: RRC connection not open */
  TDSRRCDORMANCY_VALIDATION_FAIL_UE_OOS,              /* Failure Status: UE OOS */
  TDSRRCDORMANCY_VALIDATION_FAIL_T323_TIMER_RUNNING,  /* Failure Status: T323 timer is running */
  TDSRRCDORMANCY_VALIDATION_FAIL_UE_LEAVING_CONN_MODE_SCAN, /* Failure Status: UE is leaving Connected Mode Scan */
  TDSRRCDORMANCY_VALIDATION_FAIL                      /* Failure status:others */
} tdsrrcdormancy_validation_status_e_type;

typedef enum
{
  TDSRRCDORMANCY_MUI_STATUS_INVALID,                 /* No outstanding MUI's */
  TDSRRCDORMANCY_MUI_SIG_CON_REL_CNF,                /* No outstanding MUI's, but waiting for sig-con-rel-cnf */
  TDSRRCDORMANCY_MUI_SCRI_CNF                        /* Outstanding MUI for SCRI-cnf */
}tdsrrcdormancy_fast_dormancy_mui_status_type;

typedef enum
{
  TDSRRCDORMANCY_SUCCESS,                            /* Successful Status */      
  TDSRRCDORMANCY_FAILURE                             /* Minor Failure Status */
} tdsrrcdormancy_status_e_type;

typedef enum
{
  TDSRRCDORMANCY_T323_TIMER_INVALID,         
  TDSRRCDORMANCY_T323_TIMER_IDLE,    
  TDSRRCDORMANCY_T323_TIMER_RUNNING,    
  TDSRRCDORMANCY_SRNS_GUARD_TIMER_RUNNING            
  //RRCDORMANCY_URA_PCH_GUARD_TIMER_RUNNING         
} tdsrrcdormancy_timer_status_e_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/*Structure to save fast dormancy req*/
typedef struct
{
  /* This flag indicate the status of MUIs for Signalling
     Connection Request */
  tdsrrcdormancy_fast_dormancy_mui_status_type mui_status;
 
  /* This variable to stores the corresponding MUIs of the
     Signalling Connection Release Request message that is transmitted */
  uint8               mui;

   /* This flag indicates whether fast_dormancy_req is stored or not */
  boolean             fast_dormancy_req_status;

  /* Count the number of pending dormancy requests */
  uint32              num_pending_dormancy_req;
   
}tdsrrcdormancy_fast_dormancy_req_type;


typedef struct
{
  /* If T323 timer is supported by N/W then it shows the support of Fast Dormancy.
     * Provides value of T323 timer. Assuming default value = 0 */
  boolean                           T323_nw_support;
  tdsrrc_T_323                         T323_duration;    
  tdsrrcdormancy_timer_status_e_type   T323_current_status;
}tdsrrcdormancy_conn_timers_and_constants_type;

/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION tdsrrcdormancy_init

DESCRIPTION

  This function initializes the dormancy  procedure.
  This function needs to be called once only, at powerup.

DEPENDENCIES

  None.

RETURN VALUE

  None.
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcdormancy_init( void );

/*===========================================================================

FUNCTION tdsrrcdormancy_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the fast dormancy  procedure.
  The following events are handled by this procedure:

    TDSRRC_FAST_DORMANCY_REQ Primitive (received from MM)
    TDSRRC_CELL_UPDATE_COMPLETE_CNF primitive (received from CELL UPDATE Procedure)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcdormancy_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
);

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_reset_n_send_fast_dormancy_cnf  
  
  DESCRIPTION
    This function sends RRC_FAST_DORMANCY_CNF command to NAS
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_reset_n_send_fast_dormancy_cnf
(
  rrc_fast_dormancy_cnf_status_e_type cnf_cause
);

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_check_n_send_cnf_for_failure_conditions  
  
  DESCRIPTION
 This function check if dormancy req is in progress. if so it will abort & send 
 RRC_FAST_DORMANCY_CNF command to NAS.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    boolean 
===========================================================================*/
boolean tdsrrcdormancy_check_n_send_cnf_for_failure_conditions
(
  void
);

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_check_and_abort_dormancy_req  
  
  DESCRIPTION
 This function check if dormancy req is in progress. if so it will abort & send 
 RRC_FAST_DORMANCY_CNF command to NAS.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_check_dormancy_initiated_scrr_active
(
  void
);

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_check_and_abort_dormancy_req  
  
  DESCRIPTION
 This function check if dormancy req is in progress. if so it will abort & send 
 RRC_FAST_DORMANCY_CNF command to NAS.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_check_and_abort_dormancy_req
(
  rrc_fast_dormancy_cnf_status_e_type abort_type
);

/*===========================================================================
  
  FUNCTION 
    rrcdormancy_check_n_restart_timer_T323  

  DESCRIPTION
    This function check if T323 is already running then STOP it and restart with the given time.
    IF timer_value == 0, then it STOPS it.
    
  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_stop_or_restart_timer_T323
(
  unsigned long timer_value,
  tdsrrcdormancy_timer_status_e_type timer_status
);

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_check_if_nw_support_fast_dormancy  

  DESCRIPTION
    This function check if N/W supports dormancy or not

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_check_if_nw_support_fast_dormancy
(
  void
);

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_stop_n_update_t323_timer_status  

  DESCRIPTION
    This function stops gurard timers for SRNS or CELL_PCH/URA_PCH states and update status of
    T323 to IDLE/RUNNING

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
tdsrrcdormancy_timer_status_e_type tdsrrcdormancy_stop_n_update_t323_timer_status
(
  void
);


/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_disable_fast_dormancy  

  DESCRIPTION
    This function disables dormancy support of network

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_disable_fast_dormancy
(
void
);

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco  

  DESCRIPTION
    This function updates dormacy procedure if SRNS relocation is in progress. 
    If so it will abort & send RRC_FAST_DORMANCY_CNF command to NAS.

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_update_dormancy_for_srns_hho_cco
(
  tdsrrc_proc_e_type rrc_proc
);

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_get_nw_support_of_fast_dormancy_from_sib  

  DESCRIPTION
    This function check dormancy support of N/W after SIB reading 

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_get_nw_support_of_fast_dormancy_from_sib
(
  void
);


/*===========================================================================
  
  FUNCTION tdsrrcdormancy_check_and_abort_dormancy_req  
  
  DESCRIPTION
 This function check if dormancy req is in progress. if so it will abort & send 
 RRC_FAST_DORMANCY_CNF command to NAS.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_is_dormancy_active
(
 void
);

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_check_n_reset_v316  

  DESCRIPTION
    This function checks the conditions for resetting v316 counter and reset
    if required.

  DEPENDENCIES
    none.

  RETURN VALUE
    none
===========================================================================*/
void tdsrrcdormancy_check_n_reset_v316
(
  tdsrrc_proc_e_type rrc_procedure
);

#endif 
