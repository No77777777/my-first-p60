#ifndef TDSMAC_UL_HS_SUBTASK_H
#define TDSMAC_UL_HS_SUBTASK_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              U P L I N K   M A C   H S U P A   H E A D E R    F I L E

DESCRIPTION
   MAC HSUPA specific type declarations.

Copyright (c) 2011-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/mac/inc/tdsmaculhssubtask.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who      what, where, why
--------   -------- --------------------------------------------------------
02/02/11   ps       Initial release Creation of file Changes for MAC HSUPA code
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_TDSCDMA_HSUPA

#include "tdsmacrrcif.h"
#include "tdsmacinternal.h"
#include "tdsmaculhs.h"

/*===========================================================================

                     DEFINES AND MACROS FOR MODULE

===========================================================================*/
/*===========================================================================

                     KEY MACROS

===========================================================================*/

/*===========================================================================

                     GENERAL PURPOSE MACROS

===========================================================================*/


/*===========================================================================

                     TYPE DEFINITIONS FOR MODULE

===========================================================================*/



/*===========================================================================

                         GLOBAL FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION        tdsmac_ulhs_handle_timer_expiry

DESCRIPTION
  This function handles the expiry of the MAC ULHS timers 
RETURN VALUE
  none

SIDE EFFECTS
none
===========================================================================*/
void tdsmac_ulhs_handle_timer_expiry
(
  uint32 timer_sig
);

/*===========================================================================
FUNCTION    tdsmac_ulhs_proc_erucch_status

DESCRIPTION Process the ERUCCH status in task context
    
DEPENDENCIES
    None
RETURN VALUE
    None
SIDE EFFECTS
    None
===========================================================================*/

void tdsmac_ulhs_proc_erucch_status
(
 void
);


#endif /* FEATURE_TDSCDMA_HSUPA */
#endif /* MAC_HS_UL_H */
