#ifndef TDSRRCLBT_H
#define TDSRRCLBT_H
/*===========================================================================
         R R C   L O O P B A C K   T E S T   M O D U L E 

DESCRIPTION

  This module is the header module for the loopback test module.
  This contains the external interface functions to be used by
  by other RRC modules. This must not be used by modules outside RRC.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrclbt.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "tdsrrccmd_v.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* Enumerated type of the different substates for this procedure           */
typedef enum
{
  TDSRRCLBT_INITIAL,                   /* Initial Substate                    */
  TDSRRCLBT_WAIT_FOR_CHAN_CFG_CNF,     /* Waiting for confirmation of channel  
                                       config from LLC                     */                      
  TDSRRCLBT_LB_IN_PROGRESS,            /* LB Test is in progress              */
  TDSRRCLBT_MAX_SUBSTATES
}tdsrrclbt_substates_e_type;
  
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

FUNCTION  RRCLBT_INIT_PROCEDURE

DESCRIPTION

  This function initializes the Loopback Test module.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrclbt_init_procedure( void );




/*===========================================================================

FUNCTION RRCLBT_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Loopback test module. This function is called by the
  dispatcher whenever an event is received for this module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrclbt_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);




/*===========================================================================

FUNCTION  RRCLBT_GET_LB_MODE_AND_DUMMY_SIG_ENABLED

DESCRIPTION

  This functions returns TRUE and the LB mode and signalling enabled status.
  If LB is not active, returns false
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrclbt_get_lb_mode_and_dummy_sig_enabled( tdsrrc_lb_test_mode_e_type *lb_mode,
                                                  boolean  *dummy_signaling_enabled);

#endif /* TDSRRCLBT_H */

