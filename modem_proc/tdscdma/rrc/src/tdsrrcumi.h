#ifndef TDSRRCUMI_H
#define TDSRRCUMI_H
/*===========================================================================
                  R R C  UTRAN MOBILITY INFORMATION

DESCRIPTION
  This header file contains function prototypes for UTRAN Mobility Info
  procedure

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcumi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdsrrcasn1.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

/*==========================================================================
  Type for defining the substate of RRC UMI Procedure.
===========================================================================*/

typedef enum
{
  TDSRRCUMI_FAILURE,                /* Failure status */
  TDSRRCUMI_SUCCESS                /* Successful Status */
}tdsrrcumi_status_e_type;

typedef enum 
{
  TDSRRCUMI_INITIAL,                 /* UMI initial substate */
  TDSRRCUMI_WAIT_FOR_RNTI_UPDATE_CNF,       /* wait for rnti update cnf from llc
                                           files which update c_rnti and u_rnti*/
  TDSRRCUMI_WAIT_FOR_CHAN_CFG_CNF,     /* Waiting for confirmation of channel  
                                    config from LLC                        */                      
  TDSRRCUMI_WAIT_FOR_L2_ACK,           /* Waiting for Layer 2 ack for uplink
                                    message transmission                   */
  TDSRRCUMI_WAIT_FOR_REESTABLISH_SRB2, /*waiting for re-establishment of srb2 */
  TDSRRCUMI_MAX_SUBSTATES

} tdsrrcumi_substate_e_type;

/* Indicates whether we received new values for Conn mode UE timers and consts */
typedef enum 
{
  TDSRRCUMI_UE_TIMER_CONST_NOT_RCVD,
  TDSRRCUMI_UE_TIMER_CONST_RCVD
} tdsrrcumi_ue_timer_const_rcvd_status_e_type; 

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

FUNCTION tdsrrcumi_init_procedure

DESCRIPTION
  This function initializes required parameters for UTRAN Mobility Info
  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcumi_init_procedure(void);

/*===========================================================================

FUNCTION tdsrrcumi_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for UTRAN Mobility Info procedure
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC) containing UTRAN
    Mobility Info message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void tdsrrcumi_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION tdsrrcumi_process_utran_mobility_info_command

DESCRIPTION
  This function reads the contents of the UTRAN Mobility Info and
  sends the SYNC_IND to MM layer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcumi_process_utran_mobility_info_command
(
  tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
);

/*===========================================================================

FUNCTION tdsrrcumi_build_utran_mobility_info_confirm_msg

DESCRIPTION
  This function builds the utran mobility info confirm message.
  This function is called by cell update and UMI procedure.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcumi_build_utran_mobility_info_confirm_msg
(
  tdsrrc_utran_mob_info_cnf_type* utran_mob_inf_cnf,
  tdsrrc_UL_DCCH_Message *msg_ptr
);
/*===========================================================================

FUNCTION tdsrrcumi_check_ue_timer_constants_update_status

DESCRIPTION
  Update global tdsrrc_ue_conn_timers_and_constants with timers and constants
  from SIB1.  Function provided for SIB procedure to query if we should
  indeed update connected mode timers and constants database.
  
DEPENDENCIES
  None

RETURN VALUE
  TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_REQ
  :  If SIB Procedure needs to update tdsrrc_ue_conn_timers_and_constants
  TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_NOT_REQ
  : If SIB Procedure need not update tdsrrc_ue_conn_timers_and_constants  
  
SIDE EFFECTS
  None
===========================================================================*/
tdsrrc_conn_timer_constants_update_status_e_type 
tdsrrcumi_check_ue_timer_constants_update_status
(
  boolean new_cell_ind
);

/*===========================================================================

FUNCTION   tdsrrcumi_is_srns_in_progress

DESCRIPTION
  This function returns if SRNS is in progress with UMI procedure
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcumi_is_srns_in_progress
(
void
);
#endif /* TDSRRCUMI_H */
