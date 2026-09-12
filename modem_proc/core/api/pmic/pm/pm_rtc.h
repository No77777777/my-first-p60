#ifndef __PM_RTC_H__
#define __PM_RTC_H__

/** @file pm_rtc.h
*/

/*===========================================================================

           R T C   S E R V I C E S   H E A D E R   F I L E

DESCRIPTION
  This file contains functions prototypes and variable/type/constant 
  declarations for the RTC services developed for the Qualcomm Power
  Management IC.
  
  Copyright (c) 2003-2009, 2013 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_rtc.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/09   jtn     Move init function prototype to pm_device_init_i.h
10/23/08   jtn     Added API pm_hal_rtc_prescaler_reset()
06/18/07   cng     Added meta comments to miscellaneous RTC APIs
05/31/06   Vish    Fixed LINT warnings.
05/03/05   Vish    Modified pm_hal_rtc_get_time() to read RTC time even in
                   the case when RTC was not running.
01/19/05   Vish    Updated function header for pm_hal_rtc_get_time().
01/28/04   rmd     Added initial support for multiple PMIC models/tiers.
11/07/03   Vish    Added the task of clearing the alarm trigger condition
                   within pm_hal_rtc_disable_alarm() so that the master
                   RTC alarm interrupt could be cleared afterward.
10/02/03   Vish    Added pm_hal_rtc_get_alarm_time() and 
                   pm_hal_rtc_get_alarm_status().
09/23/03   Vish    Changed all pm_rtc_xxx variables/fns to pm_hal_rtc_xxx.
09/13/03   Vish    Created.
===========================================================================*/

#include "comdef.h"
#include "pm_err_flags.h"

/* =========================================================================
                         TYPE DEFINITIONS
========================================================================= */

/** @addtogroup pm_rtc
@{ */

/** RTC time structure used for setting and retrieving current time and for
    setting the alarm time. */
typedef struct
{
   uint32  sec;  /**< RTC time in seconds. */
} pm_hal_rtc_time_type;

/* =========================================================================
                         FUNCTION PROTOTYPES
========================================================================= */

/*===========================================================================
FUNCTION  pm_hal_rtc_get_time                              EXTERNAL FUNCTION
===========================================================================*/

 /**  This function returns the current time of the RTC. This will be the actual
   present time if the RTC has been ticking or the time at which the RTC
   was last stopped.

@param[in] pmic_chip Select the device in which the coin cell charger
                             being controlled is located. Device index starts with zero.

@return 
  SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
  pm_init() and pm_rtc_start() must have been called.

@sideeffects
   Interrupts are disabled during this function.
*/
pm_err_flag_type 
pm_dev_hal_rtc_get_time (uint8 pmic_chip,
                         pm_hal_rtc_time_type *time_ptr);
//#pragma message("Do not use pm_hal_rtc_get_time(), which is deprecated. Use pm_dev_hal_rtc_get_time() instead.")
pm_err_flag_type pm_hal_rtc_get_time(pm_hal_rtc_time_type *time_ptr);


/** @} */ /* end_addtogroup pm_rtc */

#endif // __PM_RTC_H__


