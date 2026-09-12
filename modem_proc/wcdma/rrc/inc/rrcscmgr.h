#ifndef RRCSCMGR_H
#define RRCSCMGR_H
/*===========================================================================
             R R C     S T A T E    C H A N G E    M A N A G E R  

DESCRIPTION

  This is the external header file for the State Change Manager module.
  This module is to be used by other RRC modules only and not by
  tasks other than RRC.
  
Copyright (c) 2001,2003-2005,2007-2008 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcscmgr.h_v   1.1   23 Apr 2001 18:57:32   krishnan  $   
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/inc/rrcscmgr.h#2 $    $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/17/15   vi      Fixed Compilation Errors
03/03/15   sn      Changes for FR 24681
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
05/08/09   ss      Updated Copyright Information
08/02/05   da      Added prototype for rrc_get_prev_state
06/06/05   da      Added prototypes for funcs rrc_register_home_zone_ind_cb(),
                   rrc_deregister_home_zone_ind_cb(), and rrc_trigger_home_zone_ind() 
12/02/04   da      Added prototypes for rrcscmrg_register_rrc_state_change_ind_cb,
                   rrcscmgr_deregister_rrc_state_change_ind_cb
04/23/01   ram     Moved rrc_init_state() and rrc_get_state() function prototypes
                   from rrctask.h. Externalized function rrc_initiate_state_change().
01/08/01   ram     Created file.


===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "rrccmd_v.h"
#include "rrcdata_v.h"
#include "stringl.h"


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
void  rrc_init_state(sys_modem_as_id_e_type wrrc_as_id);




/*===========================================================================

FUNCTION RRC_INITIATE_STATE_CHANGE

DESCRIPTION

  This function initiates a state change in RRC by posting a 
  RRC_NEW_STATE_IND command with the given RRC state. When this
  command is processed, the RRC state will be changed to the
  new state and state change notifications will be sent if needed.
  
  The reason the new state is not set righ away is that we
  want to avoid any potential race conditions with commands
  that are already queued. When commands that are already
  queued are processed before the RRC_NEW_STATE_IND command they
  may be calling rrc_get_state to get the current state which
  may lead to some race conditions.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void  rrc_initiate_state_change
( 
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_state_e_type  state  /* New state */ 
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
#ifdef FEATURE_WCDMA_QSH_DUMP
__attribute__((section(".uncompressible.text")))
#endif
extern  rrc_state_e_type  rrc_get_state(sys_modem_as_id_e_type wrrc_as_id);


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
extern  rrc_state_e_type  rrc_get_prev_state(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION RRCSCMGR_REGISTER_FOR_SCN

DESCRIPTION

  This function registers State change notification requests from 
  other procedures. When a procedure needs to be notified when a 
  particular RRC state transition occurs, that procedure must register
  using this function. The from_state is the state from which the
  transition occurs and the to_state is the state to which the transition
  occurs. Wildcard may be used for both the from_state and to_state.
  The wildcard to be used is RRC_STATE_WILDCARD. When a wildcard is
  used for both from and to states, transitions from any state to
  any other state will be notified.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrcscmgr_register_for_scn
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_proc_e_type  procedure,   /* Procedure that's doing this registration */
  rrc_state_e_type from_state,  /* State from which the transition occurs. This
                                can be "RRC_STATE_WILDCARD" to indicate that the
                                from state can be any RRC state */
  rrc_state_e_type to_state     /* State to which the transition occurs. This
                                can be "RRC_STATE_WILDCARD" to indicate that the
                                to state can be any RRC state */
);




/*===========================================================================

FUNCTION RRCSCMGR_EVENT_HANDLER

DESCRIPTION

  This is the event handler for the state change manager module.
  Only one event is expected here and that is the internal command
  RRC_NEW_STATE_IND indicating that RRC has moved to a new state.
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
void rrcscmgr_event_handler
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr    /* Pointer to received command */
);

#if defined(FEATURE_THOR_MODEM) || defined(FEATURE_WCDMA_ATLAS_MODEM) || defined(FEATURE_TABASCO_MODEM)
/*===========================================================================

FUNCTION RRC_UPDATE_CXM_WITH_STATE

DESCRIPTION

  This function indicates to CXM whenever there is a state change or whenever 
  RRC goes in and out of inactive state

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_update_cxm_with_state
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_state_e_type rrc_state
);
#endif
#endif /* RRCSCMGR_H */

