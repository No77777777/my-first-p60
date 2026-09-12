#ifndef SRCHHHO_H
#define SRCHHHO_H
/*===========================================================================
FILE: tdssrchhho.h

GENERAL DESCRIPTION:
      This file contains implementation of external functions and utility
      functions shared by search routines at various states.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright(c) 2000-2010 by Qualcomm Technologies Incorporated. All Rights Reserved.
        
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchhho.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/17/10   sv      Changed some API names.
03/03/09   sv      Externed the "srch_retry_interf_redirection_acq" function.
02/27/09   sv      Cleaned up some code.
10/03/08   hk      Bringing in 7k Mailine fixes
08/02/05   m       Moved a prototype to tdssrchset.h as function also moved
06/02/05   m       Modified few prototypes to match the change in c files
05/13/05   m       Modified prototype for the few functions
05/03/05   m       Fixed bugs reported in integration of srch optimization
                   feature and resume on HHO failure
04/13/05   m       Added more support for srch optimization for HHO
03/28/05   m       Created file.
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "tdsl1rrcif.h"
#include "tdssrchset.h"
#include "tdssrchcmd.h"


#endif
