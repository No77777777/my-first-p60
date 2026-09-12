#ifndef TDSRRCTFCC_H
#define TDSRRCTFCC_H
/*===========================================================================
         R R C  TRANSPORT FORMAT COMBINATION  CONTROL MODEL

DESCRIPTION

  This module is the header module for the Transport Format Combination Control.
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrctfcc.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
12/02/10   su      Provided fix to process TFCC message while another 
                   Reconfiguration is on going.

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
#include "tdsrrccmd_l2.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
typedef enum
{
  TDSRRCTFCC_INITIAL,                   /* Initial Substate                    */
  TDSRRCTFCC_WAIT_FOR_MAC_CFG_CNF,
  TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF,
  TDSRRCTFCC_MAX_SUBSTATES
}tdsrrctfcc_substates_e_type;
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

FUNCTION   RRCTFC_PROCEDURE_EVENT_HANDLER

DESCRIPTION
  
  This procedure processes the Transport Format Combination Control 
  message to config the TFC Subset in MAC.

        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
void tdsrrctfcc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION  RRCTFCC_INIT_PROCEDURE

DESCRIPTION

  This function initializes the TFCC Procedure
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctfcc_init_procedure
(
  void
);



/*===========================================================================

FUNCTION  RRCTFCC_CLEAR_PROCEDURE

DESCRIPTION

  This function clears the TFCC Procedure
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctfcc_clear_procedure
(
void
);


/*===========================================================================

FUNCTION   RRCTFCC_PROCESS_MAC_CONFIG_CNF

DESCRIPTION
  This function process the received MAC CFG CNF
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
void tdsrrctfcc_process_mac_config_cnf
(
tdsrrc_mac_cmd_type * cmd_ptr
);


/*===========================================================================

FUNCTION  tdsrrctfcc_is_reconfig_in_progress

DESCRIPTION

  This function will check if TFCC Procedure substate is reconfig in progress
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE: if TFCC procedure is wating for reconfig in progress
  FALSE: othewise.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrctfcc_is_reconfig_in_progress
(
  void
);
#endif /* TDSRRCTCRC_H */

