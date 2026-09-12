/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     MBMS SERVICE MANAGER MAIN MODULE (MSMMAIN.C)

GENERAL DESCRIPTION

This module contains the implementation of the MBMS Service Manager module 

EXTERNALIZED FUNCTIONS
 
INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001,2002,2003 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mmcp.mpss/7.9.0/nas/mbmsm/src/msmmain.c#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/17/06   vr      Initial version
03/18/08   rk      Added Support to merge MBMS SM in CB task

===========================================================================*/


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#include "mmcp_variation.h"
#include "sys.h"
#include "sys_v.h"
#include "sys_gw_v.h"
#include "cmwll.h"
#include "rrccmd.h"

#ifdef FEATURE_MBMS
#error code not present
#endif

