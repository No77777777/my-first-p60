#ifndef DS707_TIMER_H
#define DS707_TIMER_H
/*===========================================================================

                        D S 7 0 7 _ T I M E R
GENERAL DESCRIPTION
  This file contains the functionality for the IS-707 specific timers.  These
  are:
    The holddown timer
    The inactivitiy timer (which lets phone go dormant in a packet data call).

EXTERNALIZED FUNCTIONS
  ds707_timer_cb()
    Registered as the callback for the holddown and idle timers.  Based on 
    which timer expired, sends msg to DS task, letting it know of timer 
    expiration.
    
 ds707_timer_init()
   Registers the holddown and idle timer with DS task.  This creates the
   timer in REX and associates the callback with it.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  Must call ds707_timer_init() on startup.

 Copyright (c) 2002 by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds707_timer.h_v   1.1   19 Nov 2002 18:41:40   akhare  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/src/ds707_timer.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $
  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/17/02   ak      Updated file header comments.
02/20/02   ak      First version of file.
===========================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_IS707

/*-------------------------------------------------------------------------- 
  Timer to delay the teadown of otasn PDN upon receipt of Refresh cmd by CM
---------------------------------------------------------------------------*/
rex_timer_type        ds707_delay_otasn_pdn_teardown_timer; 

/*===========================================================================
                           EXTERNAL FUNCTIONS
===========================================================================*/
/*===========================================================================
FUNCTION      DS707_TIMER_INIT

DESCRIPTION   Register 707 timers with 3G DSMGR Timer services.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds707_timer_init(void);

#endif /* FEATURE_DATA_IS707 */
#endif /* DS707_TIMER_H      */

