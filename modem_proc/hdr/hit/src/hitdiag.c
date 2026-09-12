/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 HIT DIAG Handler
GENERAL DESCRIPTION
  This file contains functions associated with handling hit packets from diag.

EXTERNALIZED FUNCTIONS
  hitdiag_handler
  
REGIONAL FUNCTIONS
  hitdiagcmd_signal

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

  Copyright (c) 2006, 2007 by Qualcomm Technologies Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/hit/src/hitdiag.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
01/15/2013   smd     Featurized hit cmd and hit diag.
05/07/2007   vh      Changed dynamic memory allocation to static one
03/28/2007   vh      Created

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "hdr_variation.h"
#include "customer.h"

#ifdef FEATURE_HIT
#endif /* FEATURE_HIT */
