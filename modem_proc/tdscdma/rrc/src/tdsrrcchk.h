#ifndef TDSRRCCHK_H
#define TDSRRCCHK_H
/*========================================================================================

                                    R R C C H K

DESCRIPTION

  This file contains the declaration of the interface functions and
  data types used by RRC

  Copyright (c) 2010-2012 by Qualcomm Technologies Incorporated.  All Rights Reserved.
========================================================================================*/

/*===========================================================================


                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcchk.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
  
 when       who     what, where, why
--------    ---     ----------------------------------------------------------
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/
                   

/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/
/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdsrrccmd_v.h"


/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/


/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/


/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION tdsrrcchk_procedure_event_handler

DESCRIPTION
  This function is the entry point of rrcchk

  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcchk_procedure_event_handler
( 
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);  

/*===========================================================================

FUNCTION RRCCHK_INIT_PROCEDURE

DESCRIPTION
  This function will register for the necessary state change indication for
  Counter Check procedure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcchk_init_procedure
(    
  void  
);

#endif


