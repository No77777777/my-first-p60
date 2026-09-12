
#ifndef __PMAPP_RTC_H__
#define __PMAPP_RTC_H__

/*===========================================================================
              P M   R T C   A P P   H E A D E R   F I L E

DESCRIPTION
  This file contains functions prototypes and variable/type/constant
  declarations for the real-time clock application developed for the Qualcomm
  Power Manager Chip Set.

Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/api/pmic/pm/pmapp_rtc.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/08/11   hw      Merging changes from the PMIC Distributed Driver Arch branch
02/13/09   vk      Removed feature FEATURE_PMIC_RTC from .h file for RPC. 
06/28/07   cng     Added meta comments to RTC APIs
06/18/07   cng     Added meta comments to pm_rtc_rw_cmd
03/07/07   aab     Added pm_rtc_get_power_reset_status() API
05/31/06   Vish    Fixed LINT warnings.
01/19/05   Vish    Updated function header for pm_rtc_rw_cmd().
11/07/03   Vish    Added the task of clearing the alarm trigger condition
                   within pm_reset_rtc_alarm() so that the master
                   RTC alarm interrupt could be cleared afterward. Also
                   resetting all alarms within pm_rtc_init() to avoid
                   any false alarms.
11/03/03   Vish    Changed signature of pm_rtc_init().
                   Renamed pm_s/get_time_adjust() to pm_rtc_s/get_time_adjust()
                   in order to be consistent with earlier PM chip libraries.
                   Added documentation. Wrapped this file under
                   FEATURE_PMIC_RTC.
10/02/03   Vish    Adding alarm support.
09/24/03   Vish    Added pm_rtc_init().
09/19/03   Vish    Created.
===========================================================================*/


/* =========================================================================
                         INCLUDE FILES
========================================================================= */
#include "comdef.h"
#include "pm_err_flags.h"

/* =========================================================================
                       MACROS / CONSTANTS
========================================================================= */


/* =========================================================================
                         TYPE DEFINITIONS
========================================================================= */
// Real-time clock command type
typedef enum
{
   PM_RTC_SET_CMD,
   PM_RTC_GET_CMD,
   PM_RTC_INVALID_CMD
}pm_rtc_cmd_type;

// Real-time clock display mode type
typedef enum
{
   PM_RTC_12HR_MODE,
   PM_RTC_24HR_MODE,
   PM_RTC_INVALID_MODE
}pm_rtc_display_type;

// PMIC's version of the Julian time structure
typedef struct
{
   word year;            // Year [1980..2100]
   word month;           // Month of year [1..12]
   word day;             // Day of month [1..31]
   word hour;            // Hour of day [0..23]
   word minute;          // Minute of hour [0..59]
   word second;          // Second of minute [0..59]
   word day_of_week;     // Day of the week [0..6] Monday..Sunday
} pm_rtc_julian_type;


/*===========================================================================
                  REAL TIME CLOCK FUNCTION PROTOTYPES
===========================================================================*/

/*===========================================================================
FUNCTION   pm_rtc_get_power_reset_status               EXTERNAL FUNCTION

DESCRIPTION
   This function returns the status of rtc power reset latched status
   A value of 1 means that power reset happened and 0 means otherwise.

PARAMETERS
   None

RETURN VALUE
   None

DEPENDENCIES
   The following function(s) must have been called:
   pm_init()

SIDE EFFECTS
   None
===========================================================================*/
extern boolean pm_rtc_get_power_reset_status(void);
/*~ FUNCTION pm_rtc_get_power_reset_status */

#endif /* __PMAPP_RTC_H__  */
