#ifndef TDSMACLB_H
#define TDSMACLB_H

/*=========================================================================================

         M A C    D U M M Y    S I G N A L I N G    H A N D L E R

GENERAL DESCRIPTION
  This file is a place holder for the definitions of various data types & 
  function prototypes that are internally used by MAC subsystem software 
  to support dummy signalling on DCCH.  
  
  This file is included only in MAC subsystem modules. 

  Dummy signaling message transmission on DCCH (intended for uplink RF testing 
  for which Reference Measurement Channels are used) is described in section 
  5.3.2.8 of TS34.109.

  The pre-conditions for enabling the dummy signaling messages on DCCH :

     - UE should be in test loop mode 1 or 2
     - DCCH dummy mode is enabled in UE (by SS entity)

  When dummy signaling is enabled and there is no DCCH data to be sent, UE sets
  all bits in the uplink DCH transport associated with a DCCH to 1.

  Please refer to TS34.109 section 5.3.2.8 for more information on dummy signaling 
  on DCCH with RMC.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


===========================================================================================*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.
   
when       who     what, where, why
--------   ---     ----------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

=====================================================================*/


/*=====================================================================
                          INCLUDE FILES
 *====================================================================*/

#include "tdscdma_variation.h"
#include "customer.h"

#ifdef FEATURE_DUMMY_SIGNALING
#error code not present
#else
void tdsfeature_dummy_signaling_dummy(void);

#endif /* ifdef FEATURE_DUMMY_SIGNALING */

#endif /* ifndef TDSMACLB_H */
