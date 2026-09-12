/*! \file
 *  \n
 *  \brief  pm_gpio_driver.c 
 *  \details  
 *  \n &copy; Copyright 2018 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/gpio/src/pm_gpio_driver.c#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
03/01/12    hs      Updated the driver.
01/12/11    wra     Added target specific configuration hookups
10/29/10    wra     Initial Creation
========================================================================== */
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_gpio_driver.h"
#include "CoreVerify.h"
#include "pm_target_information.h"
#include "pm_version_p.h"
#include "device_info.h"
#include "pm_malloc.h"
#include "hw_module_type.h"
#include "pm_ulog.h"
/*===========================================================================

                        STATIC VARIABLES

===========================================================================*/

/* Static global variable to store the GPIO data */
static pm_gpio_data_type *pm_gpio_data_arr[PM_MAX_NUM_PMICS];

static pm_gpio_register_info_type gpio_reg =
{
  .base_address       = 0xC000,
  .peripheral_offset  = 0x100,
  .status             = 0x008,
  .int_rt_sts         = 0x010,
  .int_set_type       = 0x011,
  .int_polarity_high  = 0x012,
  .int_polarity_low   = 0x013,
  .int_latched_clr    = 0x014,
  .int_en_set         = 0x015,
  .int_en_clr         = 0x016,
  .int_latched_sts    = 0x018,
  .int_pending_sts    = 0x019,
  .int_mid_sel        = 0x01A,
  .int_priority       = 0x01B,
  .mode_ctl           = 0x040,
  .dig_pull_ctl       = 0x042,
  .dig_in_ctl         = 0x043,
  .dig_out_ctl        = 0x045,
  .dig_vin_ctl        = 0x041,
  .en_ctl             = 0x046,
  .dig_out_src_ctl    = 0x044,
};

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_gpio_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_gpio_data_type *gpio_ptr = NULL;

    pm_target_data_type *target_ptr = pm_target_info_target_data();

    if (target_ptr == NULL)
    {
      PM_LOG_MSG_ERR_FATAL("Unable to find target_data");
    }

    gpio_ptr = pm_gpio_data_arr[pmic_index];

    if (gpio_ptr == NULL)
    {
        pm_malloc( sizeof(pm_gpio_data_type), (void**)&gpio_ptr);

        /* Assign Comm ptr */
        gpio_ptr->comm_ptr = comm_ptr;

        /* LDO Register Info - Obtaining Data through dal config */
        gpio_ptr->gpio_register = &gpio_reg;

        CORE_VERIFY_PTR(gpio_ptr->gpio_register);

        gpio_ptr->num_of_peripherals = target_ptr->num_of_gpio[pmic_index];

        if((peripheral_info->peripheral_type==PM_HW_MODULE_GPIO) &&
        ((peripheral_info->peripheral_subtype>=PM_HW_MODULE_GPIO_GPIOLV) ||
        (peripheral_info->peripheral_subtype==PM_HW_MODULE_GPIO_GPIOMV)))
            gpio_ptr->is_periph_unified_reg=1;
        else
            gpio_ptr->is_periph_unified_reg=0;

        /* Num of peripherals cannot be 0 if this driver init gets called */
        CORE_VERIFY(gpio_ptr->num_of_peripherals != 0);

        pm_gpio_data_arr[pmic_index] = gpio_ptr;
    }
}

pm_gpio_data_type* pm_gpio_get_data(uint8 pmic_index)
{
    if(pmic_index < PM_MAX_NUM_PMICS)
    {
        return pm_gpio_data_arr[pmic_index];
    }

    return NULL;
}

uint8 pm_gpio_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) &&
      (pm_gpio_data_arr[pmic_index] != NULL))
  {
      return pm_gpio_data_arr[pmic_index]->num_of_peripherals;
  }

  return NULL;
}