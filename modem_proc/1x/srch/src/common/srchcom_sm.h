#ifndef SRCHCOM_H
#define SRCHCOM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

        S E A R C H C O M  ---  H E A D E R   F I L E

GENERAL DESCRIPTION
  This module contains header information pertaining to the common state
  machine.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2005 - 2013 Qualcomm Technologies, Inc. 
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

$Header: //components/rel/1x.mpss/8.0/srch/src/common/srchcom_sm.h#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
09/18/09   adw      Added support for dynamic memory allocation.
03/25/09   adw      Include modem_1x_defs.h for 1x specific modem defines.
10/13/06   trc      Changes for new STM grammar
06/29/05   kwo      Centralized int cmd queue
05/31/05   rng      Initial Version - Merged from SHDR Sandbox.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "customer.h"
#include "modem_1x_defs.h"


/*===========================================================================

FUNCTION SRCHCOM_INIT

DESCRIPTION
  This function initializes the Searcher common state machine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void srchcom_init( void );

/*===========================================================================

FUNCTION SRCHCOM_DEINIT

DESCRIPTION
  This function de-initializes the Searcher common state machine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void srchcom_deinit( void );

#endif /* SRCHCOM_H */
