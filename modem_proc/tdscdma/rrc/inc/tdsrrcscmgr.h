#ifndef TDSRRCSCMGR_H
#define TDSRRCSCMGR_H
/*===========================================================================
             R R C     S T A T E    C H A N G E    M A N A G E R  

DESCRIPTION

  This is the external header file for the State Change Manager module.
  This module is to be used by other RRC modules only and not by
  tasks other than RRC.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/inc/tdsrrcscmgr.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

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

#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsrrccmd_v.h"


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






/* =======================================================================
**                          Macro Definitions
** ======================================================================= */


/* =======================================================================
**                        Function Declarations
** ======================================================================= */


/*===========================================================================

FUNCTION RRC_TRIGGER_HOME_ZONE_IND

DESCRIPTION

This function will call the register HZI cb func to notify the rrc state and 
required location information

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

extern void tdsrrc_trigger_home_zone_ind
(
  void
);


/*===========================================================================

FUNCTION RRC_INIT_STATE

DESCRIPTION

  This function initializes the default state of RRC at powerup.
  This function should be called only once, at powerup.
  
DEPENDENCIES
  None

RETURN VALUE
  Current state of RRC.

SIDE EFFECTS
  None

===========================================================================*/
void  tdsrrc_init_state( void );




/*===========================================================================

FUNCTION RRC_INITIATE_STATE_CHANGE

DESCRIPTION

  This function initiates a state change in RRC by posting a 
  TDSRRC_NEW_STATE_IND command with the given RRC state. When this
  command is processed, the RRC state will be changed to the
  new state and state change notifications will be sent if needed.
  
  The reason the new state is not set righ away is that we
  want to avoid any potential race conditions with commands
  that are already queued. When commands that are already
  queued are processed before the TDSRRC_NEW_STATE_IND command they
  may be calling tdsrrc_get_state to get the current state which
  may lead to some race conditions.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void  tdsrrc_initiate_state_change
( 
  tdsrrc_state_e_type  state  /* New state */ 
);



/*===========================================================================

FUNCTION RRC_GET_STATE

DESCRIPTION
  This function returns the current state of RRC.
  
DEPENDENCIES
  None

RETURN VALUE
  Current state of RRC.

SIDE EFFECTS
  None

===========================================================================*/
extern  tdsrrc_state_e_type  tdsrrc_get_state( void );


/*===========================================================================

FUNCTION RRC_GET_PREV_STATE

DESCRIPTION
  This function returns the previous state of RRC.
  
DEPENDENCIES
  None

RETURN VALUE
  Current state of RRC.

SIDE EFFECTS
  None

===========================================================================*/
extern  tdsrrc_state_e_type  tdsrrc_get_prev_state( void );

/*===========================================================================

FUNCTION RRCSCMGR_REGISTER_FOR_SCN

DESCRIPTION

  This function registers State change notification requests from 
  other procedures. When a procedure needs to be notified when a 
  particular RRC state transition occurs, that procedure must register
  using this function. The from_state is the state from which the
  transition occurs and the to_state is the state to which the transition
  occurs. Wildcard may be used for both the from_state and to_state.
  The wildcard to be used is TDSRRC_STATE_WILDCARD. When a wildcard is
  used for both from and to states, transitions from any state to
  any other state will be notified.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcscmgr_register_for_scn
(
  tdsrrc_proc_e_type  procedure,   /* Procedure that's doing this registration */
  tdsrrc_state_e_type from_state,  /* State from which the transition occurs. This
                                can be "TDSRRC_STATE_WILDCARD" to indicate that the
                                from state can be any RRC state */
  tdsrrc_state_e_type to_state     /* State to which the transition occurs. This
                                can be "TDSRRC_STATE_WILDCARD" to indicate that the
                                to state can be any RRC state */
);

#ifdef FEATURE_TDSCDMA_RRC_REPORT_STATE_MODE_TO_MCS
/*===========================================================================

FUNCTION tdsrrc_send_rrc_mode_and_state_to_mcs

DESCRIPTION
  This function sends RRC mode and state to MCS .
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrrc_send_rrc_mode_and_state_to_mcs(void);
#endif

/*===========================================================================

FUNCTION RRCSCMGR_EVENT_HANDLER

DESCRIPTION

  This is the event handler for the state change manager module.
  Only one event is expected here and that is the internal command
  TDSRRC_NEW_STATE_IND indicating that RRC has moved to a new state.
  This function is called by the dispatcher.
  
  This function initiates the process of notifying procedures, if any,
  that need to be notified of the state change.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcscmgr_event_handler
(
  tdsrrc_cmd_type *cmd_ptr    /* Pointer to received command */
);




#endif /* TDSRRCSCMGR_H */

