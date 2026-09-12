/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        HIT COMMAND SERVICE UTILITY FUNCTIONS

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS
    hitcmd_alloc
    hitcmd_free
    hitcmd_param_ptr
    hitcmd_send_status
    hitcmd_send_rsp_pkt
    hitcmd_refresh
    
REGIONAL FUNCTIONS
    None

INITIALIZATION AND SEQUENCING REQUIREMENTS
    None
    
    Copyright (c) 2006 - 2014 by Qualcomm Technologies Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/hit/src/hitcmd.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
01/15/2013   smd     Featurized hit cmd and hit diag.
01/15/2014   vko     Fixed compiler warnings
12/19/2013   sat     Changed memcpy() to assignment or memscpy() (safer version).
05/07/2007   vh      Changed dynamic memory allocation to static one
03/28/2007   vh      Created

===========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "hdr_variation.h"
#include "customer.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#include "amssassert.h"

#ifdef FEATURE_HIT
#endif /* FEATURE_HIT */

