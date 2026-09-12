#ifndef SRCH_PWR_H
#define SRCH_PWR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  S R C H   P O W E R   M O D U L E

GENERAL DESCRIPTION
      This module contains external interfaces and declarations for the
      searcher power module.

EXTERNALIZED FUNCTIONS
      srch_pwr_init
      srch_pwr_deinit
      srch_pwr_enable_pwr_strobe
      srch_pwr_strobe

INITIALIZATION AND SEQUENCING REQUIREMENTS
      Must call srch_pwr_init() before any other functions in this module
      are invoked.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Copyright (c) 2009 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/inc/srch_pwr.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/12   srk     Removed obsolete functions.
01/21/11   adw     Added new "all" client.
08/04/10   adw     Transition request refinements, added 1x Advanced client.
06/17/10   adw     Refined implementation and added transition interface.
09/10/09   adw     Added Node Power Architecture (NPA) support.
09/08/09   adw     Initial revision.

===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Srch */

/* Other */



/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Typedefs
-------------------------------------------------------------------------*/
/* Unique identifier for each client of the power module. Additional clients
   can easily be added by updating the client enum type below as well as
   adding the new client in srch_pwr.c. Search for the "ADD_NEW_CLIENT_HERE"
   tag to see all locations requiring updates in order to add a new pwr
   client. */
typedef enum
{
  SRCH_PWR_FIRST_CLIENT,
  SRCH_PWR_TRANSITION = SRCH_PWR_FIRST_CLIENT,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  SRCH_PWR_INACTIVE,
  SRCH_PWR_INIT,
  SRCH_PWR_IDLE,
  SRCH_PWR_ZZ_PRE_SB,
  SRCH_PWR_ZZ_POST_SB,
  SRCH_PWR_TRAFFIC,
  SRCH_PWR_1X_ADV,
  SRCH_PWR_ALL,
  /* ADD_NEW_CLIENT_HERE: Add new srch_pwr client here... */
  SRCH_PWR_MAX_CLIENTS
}
srch_pwr_client_type;



/*=============================================================================

                      FUNCTION DECLARATIONS FOR MODULE

=============================================================================*/

/*=============================================================================

FUNCTION       SRCH_PWR_INIT
DESCRIPTION    Initialize the srch power module. Creates NPA handles for
               all nodes used by power module clients. This function should
               only be called once during srch task startup and must be called
               before invoking any other routines in the srch power module.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void srch_pwr_init( void );

/*=============================================================================

FUNCTION       SRCH_PWR_DEINIT
DESCRIPTION    De-initialize the srch power module. Cancels any active client
               request and releases all NPA handles created during the
               initialization of the srch power module.

DEPENDENCIES   The srch_pwr_int() routine must have been called previously.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void srch_pwr_deinit( void );

/*===========================================================================

FUNCTION       SRCH_PWR_ENABLE_PWR_STROBE
DESCRIPTION    This function is called by the srch_diag interface to
               enable the power strobe.

DEPENDENCIES   In a given wakeup cycle the power strobe should end up low
               at the end to prevent current consumption during during the
               TCXO off period.

               The srch_pwr_int() routine must have been called previously.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void srch_pwr_enable_pwr_strobe
(
  /* Flag to enable power strobes */
  boolean enable,

  /* Gpio to use for power strobes */
  uint8   gpio
);

/*===========================================================================

FUNCTION       SRCH_PWR_STROBE
DESCRIPTION    This function is used to toggle a GPIO for NERO power
               strobe measurements.

DEPENDENCIES   In a given wakeup cycle the power strobe should end up low
               at the end to prevent current consumption during during the
               TCXO off period.

               The srch_pwr_int() routine must have been called previously.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void srch_pwr_strobe( void );


#endif /* SRCH_PWR_H */

