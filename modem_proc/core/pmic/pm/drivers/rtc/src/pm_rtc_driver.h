#ifndef PM_RTC_DRIVER__H
#define PM_RTC_DRIVER__H

/*! \file
*  \n
*  \brief  pm_rtc_driver.h
*  \details  This file contains functions prototypes and variable/type/constant
*  declarations for supporting RTC pin services for the Qualcomm
*  PMIC chip set.
*  \n &copy; Copyright 2010-2013 Qualcomm Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/rtc/src/pm_rtc_driver.h#1 $

when         who      what, where, why
--------     ---      ----------------------------------------------------------
04/10/12     hs       Changed the type for internalResourceIndex from int to uint8.
08/18/11     wra      mega clocks module V6 Initial Creation
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_target_information.h"
#include "pm_comm.h"
#include "pm_rtc.h"

/*===========================================================================

                     TYPE DEFINITIONS 

===========================================================================*/
typedef struct pm_rtc_register_info_type
{
    pm_register_address_type    base_address;  
    pm_register_address_type    peripheral_offset;
    pm_register_address_type    time_adj;           //0x44    //rtc_time_adjust;    
    pm_register_address_type    en_ctl;             //0x46    //sec_rtc_ctrl; 
    pm_register_address_type    wdata0;             //0x40    //sec_rtc_clk_wdata0;         
    pm_register_address_type    wdata1;             //0x41    //sec_rtc_clk_wdata1;         
    pm_register_address_type    wdata2;             //0x42    //sec_rtc_clk_wdata2;         
    pm_register_address_type    wdata3;             //0x43    //sec_rtc_clk_wdata3;         
    pm_register_address_type    rdata0;             //0x48    //sec_rtc_clk_rdata0;         
    pm_register_address_type    rdata1;             //0x49    //sec_rtc_clk_rdata1;         
    pm_register_address_type    rdata2;             //0x4A    //sec_rtc_clk_rdata2;         
    pm_register_address_type    rdata3;             //0x4B    //sec_rtc_clk_rdata3;         
}pm_rtc_register_info_type;

typedef enum pm_rtc_val_shift_type
{
    PM_RTC_VAL0_SHIFT = 0,
    PM_RTC_VAL1_SHIFT = 8,
    PM_RTC_VAL2_SHIFT = 16,
    PM_RTC_VAL3_SHIFT = 24,
    PM_HAL_RTC_INTERNAL_VAL_FROM_TIME__INVALID
} pm_rtc_val_shift_type;

typedef struct pm_rtc_data_type
{
    pm_comm_info_type *comm_ptr;
    pm_rtc_register_info_type* rtc_reg;
    uint8 num_of_peripherals;
}pm_rtc_data_type;

/** Type for selecting the physical alarms available in the RTC. */
typedef enum
{
   PM_HAL_RTC_ALARM_1,  /**< RTC alarm 1. */
   PM_HAL_RTC_ALARM_INVALID
} pm_hal_rtc_alarm;


/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/
void pm_rtc_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_rtc_data_type* pm_rtc_get_data(uint8 pmic_index);

void pm_rtc_time_32_bits_to_rtc_counter_val(uint32  sec_32_bits, pm_register_data_type *arr);

uint32 pm_rtc_counter_val_to_time_32_bits(pm_register_data_type *t);

/* =========================================================================
                         FUNCTION PROTOTYPES
========================================================================= */

/*===========================================================================
FUNCTION  pm_rtc_start                                 EXTERNAL FUNCTION
===========================================================================*/

/**
   Starts the RTC ticking with the indicated time as its current
   (start-up) time.

@param[in] pmic_chip Select the device in which the coin cell charger
                             being controlled is located. Device index starts
                             with zero.
@param[in] start_time_ptr Pointer to the start-up time for the real-time clock.
                          See #pm_hal_rtc_time_type. Valid input is a valid
                          non-NULL RTC time structure containing any 32-bit
                          number indicating the number of seconds elapsed from
                          a known point in time in history.
   
@return 
  SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
  pm_init() must have been called.

@sideeffects
   Interrupts are disabled during this function.
*/
pm_err_flag_type pm_rtc_start(
   uint8 pmic_chip, 
   const pm_hal_rtc_time_type *start_time_ptr
);



/*===========================================================================
FUNCTION  pm_rtc_stop                                  EXTERNAL FUNCTION

DESCRIPTION
   This function stops the RTC from ticking.

@param[in] pmic_chip Selects the device in which the coin cell charger being controlled is located. 
Device index starts with zero.

@return
   SUCCESS or Error -- See #pm_err_flag_type.          

@dependencies
pm_init() must have been called.

@sideeffects
   Interrupts are disabled during this function.
*/

pm_err_flag_type pm_rtc_stop(uint8 pmic_chip);

/*===========================================================================
FUNCTION  pm_rtc_set_time_adjust                       EXTERNAL FUNCTION
===========================================================================*/

/**
   Sets the time adjustment correction factor for a RTC crystal
   oscillator that is slightly off the 32768 Hz frequency. Every 10th second,
   the frequency divider is switched from the nominal 32768 to
   (32768 - 64 + time_adjust).

@param[in] pmic_chip Select the device in which the coin cell charger
                             being controlled is located. Device index starts
                             with zero.
@param[in] time_adjust Adjustment factor (uint8). Valid inputs: \n
           0 to 63    -->  Compensates for a slower crystal oscillator. \n
           64         -->  Provides no compensation. \n
           65 to 127  -->  Compensates for a faster crystal oscillator.

@return 
  SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
  pm_init() must have been called.

@sideeffects
   Interrupts are disabled during this function.
*/
pm_err_flag_type pm_rtc_set_time_adjust(
    uint8 pmic_chip,
    uint8 time_adjust
);


/*===========================================================================
FUNCTION  pm_rtc_get_time_adjust                       EXTERNAL FUNCTION
===========================================================================*/

/**
   Returns the current time adjustment correction factor in use, as
   set by the previous pm_rtc_set_time_adjust().

@param[in] pmic_chip Select the device in which the coin cell charger
                             being controlled is located. Device index starts
                             with zero.
@param[out] time_adjust_ptr Pointer to the adjustment factor (uint8).
                           Valid inputs: \n
           Non-NULL pointer to a valid uint8 containing: \n
           0 to 63    -->  Compensation for a slower crystal oscillator. \n
           64         -->  No compensation. \n
           65 to 127  -->  Compensation for a faster crystal oscillator.

@return 
  SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
  pm_init() and pm_rtc_set_time_adjust() must have been called.

@sideeffects
   Interrupts are disabled during this function.
*/
pm_err_flag_type pm_rtc_get_time_adjust(
    uint8 pmic_chip, 
    uint8 *time_adjust_ptr
);


#endif // __PM_RTC_DRIVER_H__
