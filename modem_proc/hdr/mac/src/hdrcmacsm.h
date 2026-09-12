
#ifndef HDRCMACSM_H
#define HDRCMACSM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                H D R   C O N T R O L   C H A N N E L   M A C 
                         
                 P R O T O C O L   S T A T E   M A C H I N E  
                                                  
                  I N T E R N A L   H E A D E R   F I L E       

GENERAL DESCRIPTION
  This module contains definitions and declarations that are internal to the
  HDR Control Channel Media Access Control (MAC) Protocol state machine.

EXTERNALIZED FUNCTIONS
  hdrcmacsm_change_state
    Change the protocol state.
    
  hdrcmacsm_get_state
    Get the current protocol state.
    
INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating 
  the protocol.  The procotol must be activated before Control Channel 
  packets can be processed.

  Copyright (c) 2000 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================
                           
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdrcmacsm.h_v   1.2   05 May 2001 11:37:12   shuang  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrcmacsm.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/30/00   st      Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                          INCLUDE FILES FOR MODULE
                               
===========================================================================*/

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/

/*---------------------------------------------------------------------------
                              PROTOCOL STATES

 The protocol can operate in the following states.
---------------------------------------------------------------------------*/

/* Protocol state enum type. */

/* 
   Note: The state enum values are fixed and they are used by the QXDM database
   New enum tag should be added after the current enum tag and also
   assign a fixed value
*/


typedef enum 
{
  HDRCMACSM_INACTIVE_STATE=0,                    
  HDRCMACSM_ACTIVE_STATE=1 
} 
hdrcmacsm_state_enum_type;

/* Number of protocol states. */

#define HDRCMACSM_NUM_STATES   2
                 
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
extern void hdrcmacsm_change_state 
( 
  hdrcmacsm_state_enum_type new_state                  
);

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
extern hdrcmacsm_state_enum_type hdrcmacsm_get_state ( void );

#endif /* HDRCMACSM_H */
