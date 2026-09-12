/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                H D R   C O N T R O L   C H A N N E L   M A C   
       
                 P R O T O C O L   S T A T E   M A C H I N E      

GENERAL DESCRIPTION
  This file contains the implementation of the state machine for the HDR 
  Control Channel Media Access Control (MAC) protocol.

EXTERNALIZED FUNCTIONS
  hdrcmacsm_change_state
    Change the state of the protocol.
    
  hdrcmacsm_get_state
    Return current protocol state.
   
INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating 
  the protocol.  The procotol must be activated before Control Channel 
  packets can be processed.

  Copyright (c) 2000-2006 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================

                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdrcmacsm.c_v   1.8   08 Jun 2001 17:55:42   kevins  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrcmacsm.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/06/06   mpa     Use hdrcmac_start_timer() and hdrcmac_stop_timer() APIs.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                          INCLUDE FILES FOR MODULE
                     
===========================================================================*/
#include "comdef.h"
#include "rex.h"
#include "hdrcmac.h"
#include "hdrcmaci.h"
#include "hdrcmacsm.h"
#include "hdrtrace.h"

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/

/* Table of valid state transitions. */

LOCAL const boolean hdrcmacsm_is_valid_transition[][ HDRCMACSM_NUM_STATES ] =
{
  { FALSE, TRUE  },                     /* Transitions from Inactive       */
  { TRUE,  FALSE },                     /* Transitions from Active         */
};

/* Protocol state variable. */

LOCAL hdrcmacsm_state_enum_type hdrcmacsm_state = HDRCMACSM_INACTIVE_STATE;
                 
/* EJECT */
/*===========================================================================

FUNCTION HDRCMACSM_CHANGE_STATE

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
  Protocol state is changed, unless the state transition is not permitted.
  
===========================================================================*/
void hdrcmacsm_change_state 
( 
  hdrcmacsm_state_enum_type  new_state                  
)
{
  /*-------------------------------------------------------------------------
   Determine if the state transition is valid.
  -------------------------------------------------------------------------*/
  if ( hdrcmacsm_is_valid_transition[ hdrcmacsm_state ][ new_state ] == FALSE )
  {
    ERR("Invalid state transition!",0,0,0);
  }
  else
  {
    HDRTRACE_STATE( HDRHAI_CC_MAC_PROTOCOL, 
      hdrcmacsm_state, new_state);
    /*-----------------------------------------------------------------------
     Exit current state.
    -----------------------------------------------------------------------*/
    switch ( hdrcmacsm_state )
    {
      case HDRCMACSM_INACTIVE_STATE:
      {
        break;
      }

      case HDRCMACSM_ACTIVE_STATE: 
      {
        hdrcmac_stop_timer();
        break;
      }
       
      default:
      {
        ERR_FATAL("Bad Current State!",0,0,0);
      }
    }

    /*-----------------------------------------------------------------------
     Enter new state.
    -----------------------------------------------------------------------*/
    switch ( new_state )
    {
      case HDRCMACSM_INACTIVE_STATE:
      {
        hdrcmac_timestamp.status = HDRCMAC_TIMESTAMP_INVALID;
        break;
      }

      case HDRCMACSM_ACTIVE_STATE: 
      {
        hdrcmac_start_timer();
        break;
      }

      default:
      {
        ERR_FATAL("Bad New State!",0,0,0);
      }
    }

    /*-----------------------------------------------------------------------
     Update state variable.
    -----------------------------------------------------------------------*/
    hdrcmacsm_state = new_state;
    HDRTRACE_STATE_ENTRY( HDRHAI_CC_MAC_PROTOCOL, new_state);

  } /* if valid state transition */

} /* hdrcmacsm_change_state() */

/* EJECT */
/*===========================================================================

FUNCTION HDRCMACSM_GET_STATE

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
hdrcmacsm_state_enum_type hdrcmacsm_get_state ( void )
{
  return( hdrcmacsm_state );
}

