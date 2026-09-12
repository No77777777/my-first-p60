#ifndef TDSL1CLKIF_H
#define TDSL1CLKIF_H

/*===========================================================================
                WL1 CLK Driver interface hrader file

DESCRIPTION
  This file is CLK Driver interface header file that supports multimode
  external interface while maintaning backward compatibility. This file
  contains all declarations and definitions necessary for WL1 to use the
  clock drivers. WL1 modules call inline function in this and this takes care of
  all CLK API changes featurization. FEATURE_CMI is used to maintain backward
  compatibility with older definitions of clk api's.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None. All are passive inline function calling CLK modules based on feature.
  All initialization requirment of respective CLK modules should be followed

Copyright (c) 2009 - 2010 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsl1clkif.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/05/09   ms      Fixed compiler warnings
09/19/09   rmsd    Made interface CMI related changes
08/25/09   rmsd    Created intial version of the file
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#ifdef FEATURE_CMI
#include "time_svc.h"
#include "time_tod.h"  
#else
#include "ts.h"
#include "clkrtc.h"
#endif

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/* Function prototypes */
__inline uint32 tdsl1clkif_time_get_uptime_secs(void);

__inline void tdsl1_time_get(qword time);

__inline void tdsl1_time_get_uptime_ms(qword uptime_ms);


/*===========================================================================

FUNCTION tdsl1clkif_time_get_uptime_secs

DESCRIPTION
  Return the number of seconds the phone has been on.

DEPENDENCIES
  None

RETURN VALUE
  Number of seconds since the phone powered on. The value is not rounded.

SIDE EFFECTS
  None

===========================================================================*/
__inline uint32 tdsl1clkif_time_get_uptime_secs(void)
{
 #ifdef FEATURE_CMI
   return time_get_uptime_secs();
 #else
   return clk_uptime();
 #endif
}

/*=============================================================================

FUNCTION wl1_time_get

DESCRIPTION
  Returns the current time

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  void.

SIDE EFFECTS
  None

=============================================================================*/
__inline void tdsl1_time_get(qword time)
{
 #ifdef FEATURE_CMI
   (void)time_get(time);
 #else
   (void)ts_get(time);
 #endif
}

/*=============================================================================

FUNCTION wl1_time_get_uptime_ms

DESCRIPTION
  Get time the phone has been powered on for
  Number of milliseconds phone has been powered on for returned to 'uptime_ms'
DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
   VOID 

SIDE EFFECTS
  None

=============================================================================*/
__inline void tdsl1_time_get_uptime_ms(qword uptime_ms)
{
 #ifdef FEATURE_CMI
   (void)time_get_uptime_ms(uptime_ms);
 #else
   (void)clk_uptime_ms(uptime_ms);
 #endif

}

#endif
