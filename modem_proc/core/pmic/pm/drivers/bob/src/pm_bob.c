/*! \file pm_bob.c
*
*  \brief Implementation file for BOOST BYPASS public APIs.
*  \n
*  &copy; Copyright 2014-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/bob/src/pm_bob.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/08/17   aab     Updated for pm_sw_mode_type change
10/20/17   czq     Add pm_bob_ocp_latched_status_clear()
06/02/17   al      Add bob volt get status
06/09/14   kt      Created
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_bob_p.h"
#include "pm_bob_driver.h"

/*===========================================================================

                     API IMPLEMENTATION

===========================================================================*/

#define PM_MAX_BOB_PIN_UV_VOLT    (0x1FFF * 1000)


pm_err_flag_type pm_bob_sw_mode_status(uint8 pmic_chip, uint8 peripheral_index, pm_bob_mode_type* mode)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);
    //uint8 periph_type = 0;

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (mode == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type          reg = 0;
        pm_register_data_type             data = (pm_register_data_type)(*mode);
        pm_pwr_data_type                  *pwr_data = &(bob_ptr->pm_pwr_data);

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->MODE_CTL);
        err_flag = pm_comm_read_byte(bob_ptr->comm_ptr->slave_id, reg, &data, 0);
        *mode = (pm_bob_mode_type)data;
    }

    return err_flag;
}

pm_err_flag_type pm_bob_pin_volt_level_status(uint8 pmic_chip, uint8 peripheral_index, uint8 pin_number, pm_volt_level_type* volt_level)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type vout_set[2] = { 0 };
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);
    //uint8 periph_type = 0;
    uint32 vmin = 0;  // microvolt
    uint32 vStep = 0; // microvolt

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (volt_level == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if (peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (pin_number >= PM_BOB_EXT_PIN_MAX || pin_number == PM_BOB_EXT_PIN_RESERVED)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else
    {
        pm_register_address_type          reg_lb = 0;

        pm_pwr_data_type                  *pwr_data = &(bob_ptr->pm_pwr_data);

        reg_lb = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VOUT_EXT_CTRL1_LB); //0x04A

        reg_lb = reg_lb + (2 * (pin_number - 1));

        err_flag = pm_comm_read_byte_array(bob_ptr->comm_ptr->slave_id, reg_lb, 2, vout_set, 0);

        *volt_level = (vout_set[1] << 8)|vout_set[0];

        vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin; // microvolt
        vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep; //microvolt

        *volt_level = vmin + (*volt_level) * vStep;
    }

    return err_flag;
}


pm_err_flag_type pm_bob_volt_level_status(uint8 pmic_chip, uint8 peripheral_index, pm_volt_level_type *volt_level)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        err_flag = pm_pwr_volt_level_status_alg(&(bob_ptr->pm_pwr_data), bob_ptr->comm_ptr, peripheral_index, volt_level);
    }

    return err_flag;
}


pm_err_flag_type
pm_bob_sw_enable_status(uint8 pmic_chip, uint8 peripheral_index,  pm_on_off_type *on_off)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);

   if (bob_ptr == NULL)
   {
       err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else if(peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
   {
      err_flag |= PM_ERR_FLAG_INVALID_PERIPHERAL;
   }
   else if (on_off == NULL)
   {
      err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
   }
   else
   {
      err_flag |= pm_pwr_sw_enable_status_alg(&(bob_ptr->pm_pwr_data), bob_ptr->comm_ptr, peripheral_index, on_off);
   }

   return err_flag;
}
