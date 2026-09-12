/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  H D R   A C C E S S   C H A N N E L   M A C   
                            
                               P R O T O C O L       

                          S T A T E   M A C H I N E      

GENERAL DESCRIPTION
  This module contains the implementation of the HDR Access Channel Media 
  Access Control (MAC) Protocol state machine.

EXTERNALIZED FUNCTIONS
  hdramac_change_state
    Change the state of the protocol.
    
  hdramac_get_state
    Return current state.
    
INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating 
  the protocol.  The procotol must be activated before an access probe 
  sequence can be sent.

  Copyright (c) 2000-2003 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdramacsm.c_v   1.7   22 May 2001 13:03:38   shuang  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdramacsm.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/10/03   mpa     Converted F3 msg to use new MSG2.0 SSID
12/11/00   kss     Moved code to start an attempt to hdramac.c.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "err.h"
#include "hdrdebug.h"
#include "rex.h"
#include "task.h"

#include "hdrind.h"
#include "hdrhai.h"
#include "hdramac.h"
#include "hdramaci.h"
#include "hdrrmac.h"
#include "hdrerrno.h"
#include "hdrtrace.h"

/* EJECT */
/*===========================================================================

                             LOCAL DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
                           PROTOCOL STATE VARIABLE
                          
 The protocol state variable is encapsulated in this module.                      
---------------------------------------------------------------------------*/

LOCAL hdramac_state_enum_type hdramacsm_state = HDRAMAC_INACTIVE_STATE;

/*---------------------------------------------------------------------------
                           STATE TRANSITION TABLE 
                           
 This table indicates which state transitions are valid for this protocol.                      
---------------------------------------------------------------------------*/

LOCAL const boolean hdramacsm_is_valid_transition[][ HDRAMAC_NUM_STATES ] =
{
  { FALSE, TRUE  },                /* Transitions from the Inactive state. */
  { TRUE,  FALSE },                /* Transitions from the Active state.   */
};

/* EJECT */
/*===========================================================================

FUNCTION HDRAMACSM_ENTER_INACTIVE_STATE

DESCRIPTION
  This function performs the processing required upon entering the Inactive 
  state.
  
DEPENDENCIES 
  None.

PARAMETERS 
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void hdramacsm_enter_inactive_state ( void )
{
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMACSM_EXIT_INACTIVE_STATE

DESCRIPTION
  This function performs the processing required upon exiting the Inactive 
  state.
  
DEPENDENCIES
  None.

PARAMETERS   
  None.

RETURN VALUE  
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void hdramacsm_exit_inactive_state ( void )
{

}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMACSM_ENTER_ACTIVE_STATE

DESCRIPTION 
  This function performs the processing required upon entering the Active 
  state.
  
DEPENDENCIES
  None.

PARAMETERS 
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void hdramacsm_enter_active_state ( void )
{

  HDR_MSG_PROT( MSG_LEGACY_HIGH, "Enter ACTIVE STATE" );

}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMACSM_EXIT_ACTIVE_STATE

DESCRIPTION  
  This function performs the processing required upon exiting the Active 
  state.
  
DEPENDENCIES  
  None.

PARAMETERS  
  None.

RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void hdramacsm_exit_active_state ( void )
{
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMACSM_CHANGE_STATE

DESCRIPTION 
  This function manages the protocol state transitions.  For valid state 
  transitions, the current state is exited by performing any required exit
  processing, then the new state is entered by perfroming any required entry 
  processing.
  
DEPENDENCIES
  None.

PARAMETERS 
  new_state - new state to enter.

RETURN VALUE
  None.
  
SIDE EFFECTS
  Exit processing for current state and entry processing for new state.  
  Protocol state is changed.
  
===========================================================================*/
void hdramacsm_change_state 
( 
  hdramac_state_enum_type new_state 
)
{
  /*-------------------------------------------------------------------------
   Determine if the state transition is valid.
  -------------------------------------------------------------------------*/
  if ( hdramacsm_is_valid_transition[hdramacsm_state][new_state] == FALSE )
  {
    ERR("Invalid state transition!",0,0,0);
  }
  else
  {
    HDRTRACE_STATE( HDRHAI_AC_MAC_PROTOCOL, 
      hdramacsm_state, new_state);

    /*-----------------------------------------------------------------------
     Exit the current state.
    -----------------------------------------------------------------------*/
    switch ( hdramacsm_state )
    {
      case HDRAMAC_INACTIVE_STATE:
      {
        hdramacsm_exit_inactive_state();
        break;
      }

      case HDRAMAC_ACTIVE_STATE:        
      {
        hdramacsm_exit_active_state();
        break;
      }
    }

    /*-----------------------------------------------------------------------
     Enter the new state.
    -----------------------------------------------------------------------*/
    switch ( new_state )
    {
      case HDRAMAC_INACTIVE_STATE:      
      {
        hdramacsm_enter_inactive_state();
        break;
      }

      case HDRAMAC_ACTIVE_STATE: 
      {
        hdramacsm_enter_active_state();
        break;
      }
    }

    /*-----------------------------------------------------------------------
     Update the state variable.
    -----------------------------------------------------------------------*/
    hdramacsm_state = new_state;
    HDRTRACE_STATE_ENTRY( HDRHAI_AC_MAC_PROTOCOL, new_state);
  } 
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMACSM_GET_STATE

DESCRIPTION
  This function returns the current state of the protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  Current protocol state.

SIDE EFFECTS
  None.
  
===========================================================================*/
hdramac_state_enum_type hdramacsm_get_state ( void )
{
  return ( hdramacsm_state );
}

