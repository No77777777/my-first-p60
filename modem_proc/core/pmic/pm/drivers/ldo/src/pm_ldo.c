/*! \file pm_ldo.c
*  
*  \brief Implementation file for LDO resource type.  
*  
*  \details Each of the APIs checks for access and then if necessary directs
*  the call to Driver implementation.
*  
*  &copy; Copyright 2012-2015 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/ldo/src/pm_ldo.c#1 $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/06/12   hw      Rearchitecturing module driver to peripheral driver
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_ldo_driver.h"
#include "pm_resource_manager.h"
#include "pm_resources_and_types.h"
#include "CoreVerify.h"

/*===========================================================================

                     LOCAL FUNCTION PROTOTYPE

===========================================================================*/

//static pm_err_flag_type pm_ldo_sw_mode_exit_bypass(pm_comm_info_type *comm_ptr, pm_register_address_type reg);

/*===========================================================================

                     API IMPLEMENTATION 

===========================================================================*/

pm_err_flag_type pm_ldo_sw_mode(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_sw_mode_type mode) 
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);
    pm_register_data_type mode_data = 0x00;
    pm_register_address_type    reg = 0;

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(ldo_peripheral_index >= ldo_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (mode >= PM_SW_MODE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_pwr_data_type *pwr_data = &(ldo_ptr->pm_pwr_data);
        boolean pmic4_flag  = pwr_data->pwr_specific_info[ldo_peripheral_index].pmic4_flag;
        boolean is_ret_present = pwr_data->pwr_specific_info[ldo_peripheral_index].is_ret_present;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[ldo_peripheral_index].periph_base_address + pwr_data->pwr_reg_table->MODE_CTL);

        switch(mode)
        {
        case PM_SW_MODE_LPM: /* Low power mode */
          {
              /* if LDO is already in Bypass mode, exit bypass properly */
              if (ldo_ptr->in_bypass[ldo_peripheral_index])
              {
                 /* LDO is already in Bypass mode and will be changed to NPM before programming LPM mode */
                      err_flag = pm_ldo_sw_mode(pmic_chip, ldo_peripheral_index, PM_SW_MODE_NPM);

                 /* wait 10uS for loop to settle */
                 DALSYS_BusyWait(10);
              }

              if (pmic4_flag == FALSE)
              {
                  mode_data = 0x04;
              }
              else
              {
                  mode_data = 0x05;
              }
          }
          break;
        case PM_SW_MODE_NPM: /* normal power mode */
          {
              /* the LDO is already in Bypass mode and will be changed to NPM */
              if (ldo_ptr->in_bypass[ldo_peripheral_index])
              { /* exit bypass properly */

                /* Configure ACTIVE_BYPASS_NPM */
                 if (pmic4_flag == FALSE)
              {
                    mode_data = 0x01;  
                  }
                  else
                  {
                    mode_data = 0x02;
                  }
                 err_flag = pm_comm_write_byte(ldo_ptr->comm_ptr->slave_id, reg, mode_data, 0);
                 
                 /* step2: wait 100uS */
                 DALSYS_BusyWait(100);

                 if (err_flag == PM_ERR_FLAG_SUCCESS)
                  {
                      /* update the bypass LUT */
                      ldo_ptr->in_bypass[ldo_peripheral_index] = 0;
                  }
              }
              mode_data = 0x07; //for NPM
          }
          break;
        case PM_SW_MODE_BYPASS: /* bypass mode */
          {
              if (!ldo_ptr->in_bypass[ldo_peripheral_index])
              {
                 if (pmic4_flag == FALSE)
                  {
                    mode_data = 0x02;
                  }
                  else
                  {
                    mode_data = 0x03;
                  }

                  if (err_flag == PM_ERR_FLAG_SUCCESS)
                  {
                      /* update the bypass LUT */
                      ldo_ptr->in_bypass[ldo_peripheral_index] = 1;
                  }
              }
          }
          break;
        case PM_SW_MODE_RETENTION: /* Retention mode */
          {
             /* LDO is already in Bypass mode and will be changed to NPM before programming retention mode */
                  if (ldo_ptr->in_bypass[ldo_peripheral_index])
                  {
                      err_flag = pm_ldo_sw_mode(pmic_chip, ldo_peripheral_index, PM_SW_MODE_NPM);

                 /* wait 10uS for loop to settle */
                 DALSYS_BusyWait(10);
                  }

             if ((pmic4_flag == FALSE) && (is_ret_present))
             {
                mode_data = 0x03;
              }
              else
              {
                mode_data = 0x04;
              }
          }
          break;
        default:
          err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
          break;
        }
    }

    if (err_flag == PM_ERR_FLAG_SUCCESS)
    {
       err_flag |= pm_comm_write_byte(ldo_ptr->comm_ptr->slave_id, reg, mode_data, 0);
    }

    return err_flag;
}


pm_err_flag_type pm_ldo_sw_mode_status(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_sw_mode_type* sw_mode) 
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_sw_mode_status_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, sw_mode);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_pin_ctrled_status(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_on_off_type *on_off)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);
    uint8 select_pin = 0;

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_pin_ctrl_status_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, on_off, &select_pin);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_pin_ctrl_status(uint8 pmic_chip, uint8 ldo_peripheral_index, uint8 *select_pin)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);
    pm_on_off_type on_off = PM_INVALID;

    if (ldo_ptr == NULL) 
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_pin_ctrl_status_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, &on_off, select_pin);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_volt_level_status(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_volt_level_type *volt_level)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_volt_level_status_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, volt_level);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_sw_enable_status(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_on_off_type *on_off) 
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_sw_enable_status_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, on_off);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_vreg_ok_status(uint8 pmic_chip, uint8 ldo_peripheral_index, boolean *on_off) 
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_is_vreg_ready_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, on_off);
    }
    return err_flag;
}

#if 0
pm_err_flag_type pm_ldo_soft_reset_status(uint8 pmic_chip, uint8 ldo_peripheral_index, boolean *status)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL) 
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_soft_reset_status_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, status);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_soft_reset_exit(uint8 pmic_chip, uint8 ldo_peripheral_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_soft_reset_exit_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index);
    }
    return err_flag;
}

#endif

pm_err_flag_type pm_ldo_volt_calculate_vset(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_volt_level_type  volt_level, uint32* vset)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL) 
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_volt_calculate_vset_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, volt_level, vset);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_volt_level_stepper_done_status(uint8 pmic_chip, uint8 ldo_peripheral_index, boolean *stepper_done)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_volt_level_stepper_done_status_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, stepper_done);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_volt_level(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_volt_level_type volt_level, boolean wait_for_settle)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_volt_level_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, volt_level, wait_for_settle);
    }
    return err_flag;
}

pm_err_flag_type pm_ldo_sw_enable(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_on_off_type on_off, boolean wait_for_settle) 
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_ldo_data_type *ldo_ptr = pm_ldo_get_data(pmic_chip);

    if (ldo_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    { 
        err_flag = pm_pwr_sw_enable_alg(&(ldo_ptr->pm_pwr_data), ldo_ptr->comm_ptr, ldo_peripheral_index, on_off, wait_for_settle);
    }
    return err_flag;
}
/*===========================================================================

                     INTERNAL DRIVER FUNCTIONS 

===========================================================================*/
#if 0
pm_err_flag_type pm_ldo_sw_mode_exit_bypass(pm_comm_info_type *comm_ptr, pm_register_address_type reg)
{
     pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
      
    CORE_VERIFY_PTR(comm_ptr);

    /* step1: set BYPASS_ACT (bit 6) */
    err_flag = pm_comm_write_byte_mask(comm_ptr->slave_id, reg, 0x40, 0x40, 0);

    /* step2: wait 100uS */
    DALSYS_BusyWait(100);

    /* step3: clear BYPASS_ACT and BYPASS_EN (bit 6 and bit 5) */
    err_flag |= pm_comm_write_byte_mask(comm_ptr->slave_id, reg, 0x60, 0x00, 0);

    return err_flag;
}
#endif