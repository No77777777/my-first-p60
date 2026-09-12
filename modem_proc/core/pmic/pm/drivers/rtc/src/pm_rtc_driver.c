/*! \file
 *  \n
 *  \brief  pm_rtc_driver.c
 *  \details  
 *  \n &copy; Copyright 2018 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/rtc/src/pm_rtc_driver.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/18/11   wra     mega clocks module V6 Initial Creation
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_rtc_driver.h"
#include "CoreVerify.h"
#include "device_info.h"
#include "pm_utils.h"
#include "pm_malloc.h"
#include "pm_ulog.h"

/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the RTC data */
static pm_rtc_data_type *pm_rtc_data_arr[PM_MAX_NUM_PMICS];

static pm_rtc_register_info_type rtc_reg = 
{
  .base_address      = 0x6000,
  .peripheral_offset = 0x100,
  .time_adj          = 0x044,
  .en_ctl            = 0x046,
  .wdata0            = 0x040,
  .wdata1            = 0x041,
  .wdata2            = 0x042,
  .wdata3            = 0x043,
  .rdata0            = 0x048,
  .rdata1            = 0x049,
  .rdata2            = 0x04A,
  .rdata3            = 0x04B,
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/

void pm_rtc_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_rtc_data_type *rtc_ptr = NULL;
    pm_target_data_type *target_ptr = pm_target_info_target_data();

    if (target_ptr == NULL)
    {
      PM_LOG_MSG_ERR_FATAL("Unable to find target_data");
    }
    
   rtc_ptr =  pm_rtc_data_arr[pmic_index];
   if(rtc_ptr == NULL)
   {
     pm_malloc(sizeof(pm_rtc_data_type),(void**)&rtc_ptr);
     pm_rtc_data_arr[pmic_index] = rtc_ptr;
   }
        
    if (rtc_ptr != NULL)
    {
                                                    
        /* Assign Comm ptr */
        rtc_ptr->comm_ptr = comm_ptr;
        
        /* RTC Register Info - Obtaining Data through dal config */
        rtc_ptr->rtc_reg = &rtc_reg;
        
        CORE_VERIFY_PTR(rtc_ptr->rtc_reg);
        
        rtc_ptr->num_of_peripherals = target_ptr->num_of_rtc[pmic_index];

        /* Num of peripherals cannot be 0 if this driver init gets called */
        CORE_VERIFY(rtc_ptr->num_of_peripherals != 0);
    }
}

pm_rtc_data_type* pm_rtc_get_data(uint8 pmic_index)
{
    if(pmic_index < PM_MAX_NUM_PMICS)
    {
        return pm_rtc_data_arr[pmic_index];
    }

    return NULL;
}


uint8 pm_rtc_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) && 
      (pm_rtc_data_arr[pmic_index] != NULL))
  {
      return pm_rtc_data_arr[pmic_index]->num_of_peripherals;
  }

  return NULL;
}

