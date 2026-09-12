/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

        H D R   F O R W A R D   T R A F F I C   C H A N N E L   M A C   
                            
                                P R O T O C O L       

                          S T A T E   M A C H I N E      

GENERAL DESCRIPTION
  This file contains the implementation of the state machine for the HDR 
  Forward Traffic Channel Media Access Control (MAC) protocol.

EXTERNALIZED FUNCTIONS   
  hdrfmacsm_change_state
    Change the state of the protocol.
    
  hdrfmacsm_get_state
    Return current protocol state.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating 
  the protocol.  The procotol must be activated before Forward Traffic 
  Channel packets can be processed.

  Copyright (c) 2000 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
  
/* EJECT */
/*===========================================================================

                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdrfmacsm.c_v   1.9   22 May 2001 13:03:46   shuang  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrfmacsm.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/26/07   etv     Removed HDRDRC_PREDICTOR_CTRL_DSP & HDRDRC_HANDOFF_CTRL_DSP.
                   These macros are already called from hdrsrchtc_init_drc.
05/02/01   kss     Use new DRC macros.
03/15/01   kss     Re-enabled variable rate DRC.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                          INCLUDE FILES FOR MODULE
                     
===========================================================================*/
#include "comdef.h"
#include "err.h"
#include "rex.h"
#include "hdrfmac.h"
#include "hdrfmaci.h"
#include "hdrhmp.h"
#include "hdrbuf.h"
#include "hdrind.h"
#include "hdrhai.h"
#include "hdrpcp.h"
#include "hdrtrace.h"
#include "hdrmdspmcr.h"

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/

/* Table of valid state transitions. */

LOCAL const boolean hdrfmac_is_valid_transition[][ HDRFMAC_NUM_STATES ] =
{
  { FALSE, TRUE,  FALSE },              /* Transitions from Inactive       */
  { TRUE,  FALSE, TRUE  },              /* Transitions from Variable Rate  */
  { TRUE,  TRUE,  FALSE }               /* Transitions from Fixed Rate     */
};

/* Protocol state variable. */

LOCAL hdrfmac_state_enum_type hdrfmac_state = HDRFMAC_INACTIVE_STATE;

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_ENTER_INACTIVE_STATE

DESCRIPTION
  This function performs the processing required upon entry to the inactive
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
void hdrfmacsm_enter_inactive_state ( void )
{
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_EXIT_INACTIVE_STATE

DESCRIPTION
  This function performs the processing required upon exit from the inactive
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
void hdrfmacsm_exit_inactive_state ( void )
{
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_ENTER_VAR_RATE_STATE

DESCRIPTION
  This function performs the processing required upon entry to the variable
  rate state.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacsm_enter_var_rate_state ( void )
{

}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_EXIT_VAR_RATE_STATE

DESCRIPTION
  This function performs the processing required upon exit from the variabe
  rate state.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacsm_exit_var_rate_state ( void )
{
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_ENTER_FIX_RATE_STATE

DESCRIPTION
  This function performs the processing required upon entry to the fixed
  rate state.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacsm_enter_fix_rate_state ( void )
{
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_EXIT_FIX_RATE_STATE

DESCRIPTION
  This function performs the processing required upon exit from the fixed
  rate state.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacsm_exit_fix_rate_state ( void )
{
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_CHANGE_STATE

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
  None.
  
===========================================================================*/
void hdrfmacsm_change_state 
( 
  hdrfmac_state_enum_type new_state 
)
{
  /*-----------------------------------------------------------------------
   Determine if the state transition is valid.
  -----------------------------------------------------------------------*/
  if ( hdrfmac_is_valid_transition[ hdrfmac_state ][ new_state ] == FALSE )
  {
    ERR("Invalid state transition!",0,0,0);
  }
  else
  {
    HDRTRACE_STATE( HDRHAI_FTC_MAC_PROTOCOL, 
      hdrfmac_state, new_state);
    /*---------------------------------------------------------------------
     Exit current state.
    ---------------------------------------------------------------------*/
    switch ( hdrfmac_state )
    {
      case HDRFMAC_INACTIVE_STATE:
      {
        hdrfmacsm_exit_inactive_state();
        break;
      }

      case HDRFMAC_VAR_RATE_STATE:
      {
        hdrfmacsm_exit_var_rate_state();
        break;
      }

      case HDRFMAC_FIX_RATE_STATE:
      {
        hdrfmacsm_exit_fix_rate_state();
        break;
      }
    }

    /*---------------------------------------------------------------------
     Enter new state.
    ---------------------------------------------------------------------*/
    switch ( new_state )
    {
      case HDRFMAC_INACTIVE_STATE:
      {
        hdrfmacsm_enter_inactive_state();
        break;
      }

      case HDRFMAC_VAR_RATE_STATE:         
      {
        hdrfmacsm_enter_var_rate_state();
        break;
      }

      case HDRFMAC_FIX_RATE_STATE:         
      {
        hdrfmacsm_enter_fix_rate_state();
        break;
      }
    }

    /*---------------------------------------------------------------------
     Update state variable.
    ---------------------------------------------------------------------*/
    hdrfmac_state = new_state;
    HDRTRACE_STATE_ENTRY( HDRHAI_FTC_MAC_PROTOCOL, new_state);

  } /* if valid state transition */

} /* hdrfmac_enter_state() */

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACSM_GET_STATE

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
hdrfmac_state_enum_type hdrfmacsm_get_state ( void )
{
  return( hdrfmac_state );
}

