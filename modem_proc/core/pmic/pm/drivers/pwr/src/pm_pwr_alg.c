/*! \file pm_pwr_alg.c 
*  \n
*  \brief  
*  \n  
*  \n &copy; Copyright 2012-2015 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/pwr/src/pm_pwr_alg.c#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
12/06/12   hw      Rearchitecturing module driver to peripheral driver
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_pwr_alg.h"
#include "pm_utils.h"

#define STEPPER_OK          0x81
#define VREG_OK             0x80
#define WAIT_25_US          25

/* MAX wait time will be 160*25usec=4milli sec */
#define VREG_OK_CHECK_MAX_ITERATION 160

/*===========================================================================

                     FUNCTION IMPLEMENTATION 

===========================================================================*/
static pm_err_flag_type pm_pwr_sw_vreg_settled_check_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_settled);

static pm_err_flag_type pm_pwr_sw_vreg_settled_check_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_settled)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_on_off_type on_off;
    boolean vreg_ready;

    uint32 vreg_ok_check_iteration = 0;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (NULL == vreg_settled)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        err_flag = pm_pwr_sw_enable_status_alg(pwr_data, comm_ptr, peripheral_index, &on_off);

        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
           return err_flag;
        }
        else if (PM_OFF == on_off)
        {
           *vreg_settled = TRUE;
            return err_flag;
        }

        //PM_ON case
        do
        {
          err_flag |= pm_pwr_is_vreg_ready_alg(pwr_data, comm_ptr, peripheral_index, &vreg_ready);

          if (vreg_ready == TRUE) 
          {
            break;
          }

          pm_busy_wait(WAIT_25_US);

        }while(++vreg_ok_check_iteration < VREG_OK_CHECK_MAX_ITERATION);

        *vreg_settled = vreg_ready;
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_is_vreg_ready_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_ready)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 vreg_mask;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (vreg_ready == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type reg = 0x0;
        pm_register_data_type data = 0;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->STATUS);
        
        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *vreg_ready = FALSE;
            return err_flag;
        }
        
        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg == TRUE)
        {
            // In the new peripherals, VREG_READY bit is used which indicates both VREG_OK and STEPPER_OK
            vreg_mask = VREG_OK; 
        }
        else if (pwr_data->pwr_specific_info[peripheral_index].is_periph_stepper == TRUE)
        {
            vreg_mask = STEPPER_OK; 
        }
        else
        {
            vreg_mask = VREG_OK;
        }

        if ((data & vreg_mask) == vreg_mask)
        {
            *vreg_ready = TRUE;
        }
        else
        {
            *vreg_ready = FALSE;
        }
    }
       
    return err_flag;
} 


pm_err_flag_type pm_pwr_sw_mode_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_sw_mode_type *sw_mode)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 periph_type = 0;
    boolean pmic4_flag = FALSE;
                                                          
    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (sw_mode == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0xFF;

        periph_type = pwr_data->pwr_specific_info[peripheral_index].periph_type;
            
        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->MODE_CTL);

        pmic4_flag = pwr_data->pwr_specific_info[peripheral_index].pmic4_flag;

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        
        if (err_flag == PM_ERR_FLAG_SUCCESS)
        {
            if (data == 0x07)
                {
                    *sw_mode = PM_SW_MODE_NPM;
                }
            else if ((data == 0x06) && (periph_type != PM_HW_MODULE_LDO))
                {
                    *sw_mode = PM_SW_MODE_AUTO;
                }
            else if (data == 0x05)
            {
                if (pmic4_flag)
                {
                    *sw_mode = PM_SW_MODE_LPM;
                }
                else
                {
                    *sw_mode = PM_SW_MODE_RETENTION;
                }
            }
            else if (data == 0x04)
            {
                if (pmic4_flag == FALSE)
                {
                    *sw_mode = PM_SW_MODE_LPM;
                }
                else
                {
                    *sw_mode = PM_SW_MODE_RETENTION;
                }
            }
            else if (data == 0x03)
            {
                if (pmic4_flag == FALSE)
            {
                    *sw_mode = PM_SW_MODE_RETENTION;
                }
                else if (periph_type == PM_HW_MODULE_LDO)
                {
                    *sw_mode = PM_SW_MODE_BYPASS;
                }
                else
                {
                    *sw_mode = PM_SW_MODE_INVALID;
                }
                }
            else if ((data <= 0x02) && (data >= 0x01) && (periph_type == PM_HW_MODULE_LDO))
                {
                *sw_mode = PM_SW_MODE_BYPASS;
                }
            else if ((data == 0x00) && (periph_type == PM_HW_MODULE_LDO) && (pmic4_flag == FALSE))
                {
                *sw_mode = PM_SW_MODE_BYPASS;
        }
        else
        {
            *sw_mode = PM_SW_MODE_INVALID;
        }
    }
    }
    return err_flag;
}

pm_err_flag_type pm_pwr_sw_enable_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off, boolean wait_for_settle)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    boolean vreg_ready = FALSE;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (on_off >= PM_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type reg = 0x0;
        pm_register_data_type data = 0;

        //periph_type = pwr_data->pwr_specific_info[peripheral_index].periph_type;
            
        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);

        if (PM_OFF == on_off)
        {
            data = 0;
            wait_for_settle = FALSE;
        }
        else
        {
            data = 0x80;
        }

        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
        {
            err_flag = pm_comm_write_byte(comm_ptr->slave_id, reg, data, 0);
        }
        else
        {
            err_flag = pm_comm_write_byte_mask(comm_ptr->slave_id, reg, 0x80, data, 0);
        }

        if ((wait_for_settle == TRUE) && (err_flag == PM_ERR_FLAG_SUCCESS)) 
        {
          err_flag = pm_pwr_sw_vreg_settled_check_alg(pwr_data, comm_ptr, peripheral_index, &vreg_ready);

          if (vreg_ready == FALSE)
          {
              err_flag |= PM_ERR_FLAG_FAILURE;
          }
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_sw_enable_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

     if((pwr_data == NULL) || (comm_ptr == NULL) || (on_off == NULL))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *on_off = PM_INVALID;
            return err_flag;
        }

        if (data & 0x80)
        {
            *on_off = PM_ON;
        }
        else
        {
            *on_off = PM_OFF;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_sw_enable_vreg_ok_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_ok)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (vreg_ok == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type reg = 0x0;
        pm_register_data_type data = 0;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->STATUS);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *vreg_ok = FALSE;
            return err_flag;
        }

        if (data & 0x80)
        {
            *vreg_ok = TRUE;
        }
        else
        {
            *vreg_ok = FALSE;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_pin_ctrl_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off, uint8 *select_pin)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    boolean unified_reg = pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg;
    uint8 mask = (unified_reg ==  TRUE) ? 0xFF : 0x0F; 

    if((pwr_data == NULL) || (comm_ptr == NULL) || (on_off == NULL) || (select_pin == NULL))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0;

        if(pwr_data->pwr_reg_table == NULL)
        {
            return PM_ERR_FLAG_INVALID_PARAMETER;
        }
        
        if(unified_reg==TRUE)
        {
            reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->FOLLOW_HWEN);
        }
        else 
        {   
        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);
        }
        
        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *on_off = PM_INVALID;
            return err_flag;
        }

        *select_pin = (uint8)(data & mask);

        if(*select_pin != 0)
        {
            *on_off = PM_ON;
        }
        else
        {
            *on_off = PM_OFF;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_volt_level_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8  peripheral_index, pm_volt_level_type *volt_level)
{
    pm_err_flag_type                 err_flag = PM_ERR_FLAG_SUCCESS;
    uint32                           vmin = 0; // microvolt
    uint32                           vStep = 0; //microvolt
    uint16                           vset_ht = 0;
    pm_register_address_type         reg = 0;
    pm_register_data_type            reg_data[2] = {0};
    boolean                          vreg_ready = FALSE;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (volt_level == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        //Read ON-OFF status if unified register is set to true 
        //Read vset_valid data if rail is ON else read vset data"
        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
        {
            err_flag |= pm_pwr_is_vreg_ready_alg(pwr_data, comm_ptr, peripheral_index, &vreg_ready);
        }
        
        if ((pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE) && (vreg_ready == TRUE))
        {
            reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VSET_VALID_LB);
        }
        else
        {
            // Get the voltage level LUT
            reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VOLTAGE_CTRL1);
        }

        err_flag |= pm_comm_read_byte_array(comm_ptr->slave_id, reg, 2, reg_data, 0);

        if ( err_flag == PM_ERR_FLAG_SUCCESS )
        {
            vmin  = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin; // Micro volt
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;    // Micro volt

            if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg == TRUE)
            {
                vset_ht = (reg_data[1]<<8) | (reg_data[0]);

                *volt_level = vmin + vset_ht * vStep;
            }
            else
            {
               err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
            }
        }
    }

    return err_flag;
}

#if 0

pm_err_flag_type pm_pwr_soft_reset_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean*  status)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if((pwr_data == NULL) || (comm_ptr == NULL) || (status == NULL))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0;

        if(pwr_data->pwr_reg_table == NULL)
        {
            return PM_ERR_FLAG_INVALID_PARAMETER;
        }

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->PERPH_RESET_CTL4);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            return err_flag;
        }

        if(data & 0x01)
        {
            *status = TRUE;
        }
        else
        {
            *status = FALSE;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_soft_reset_exit_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if((pwr_data == NULL) || (comm_ptr == NULL))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type           sec_reg = 0x0;
        pm_register_address_type           perph_reg = 0x0;
        pm_register_data_type              sec_data = 0xA5;
        pm_register_data_type              perph_data = 0;

        if(pwr_data->pwr_reg_table == NULL)
        {
            return PM_ERR_FLAG_INVALID_PARAMETER;
        }

        sec_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->SEC_ACCESS);
        perph_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->PERPH_RESET_CTL4);

        /* Write 0xA5 to sec_access register to get write access to protected registers */
        err_flag = pm_comm_write_byte(comm_ptr->slave_id, sec_reg, sec_data, 0);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            return err_flag;
        }

        /* Write 0 to perph_ctl4 register to get power rail out of soft reset */
        err_flag = pm_comm_write_byte(comm_ptr->slave_id, perph_reg, perph_data, 0);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            return err_flag;
        }
    }

    return err_flag;
}

#endif

pm_err_flag_type pm_pwr_volt_level_stepper_done_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8  peripheral_index, boolean *stepper_done)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    boolean unified_reg = pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg;
    uint8 mask = (unified_reg ==  TRUE) ? 0x04 : 0x01; 
    
    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (stepper_done == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type          reg = 0;
        pm_register_data_type             data = 0;
        
        // get the voltage level LUT
        reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->STATUS);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *stepper_done = FALSE;
        }
        else if (data & mask)
        {
            *stepper_done = TRUE;
        }
        else
        {
            *stepper_done = FALSE;
        }
        
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_volt_calculate_vset_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8  peripheral_index, pm_volt_level_type volt_level, uint32 *vset)
{
    pm_err_flag_type                  err_flag = PM_ERR_FLAG_SUCCESS;
    uint32                            vmin = 0;  // microvolt
    uint32                            vStep = 0; // microvolt
    uint32                            calculated_vset = 0;

    if (peripheral_index >= (pwr_data->num_of_peripherals) || 
            pwr_data->pwr_specific_info[peripheral_index].periph_type == 0)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL ;
    }
    else
    {
        if ((volt_level <= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMax) && 
            (volt_level >= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin) )
        {
            vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin;
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;
        }
       
        if (vStep > 0)
        {
            calculated_vset = (volt_level - vmin)/vStep;

            if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
            {
                vset[0] = ((calculated_vset) & 0xFF);
                vset[1] = ((calculated_vset>>8) & 0xFF);

            }
            else
            {
                //calculated VSET
                *vset = (pm_register_data_type)(calculated_vset);
            }
        }
        else
        {
            err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
        }
    }   
    return err_flag;
}

pm_err_flag_type pm_pwr_volt_level_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type  volt_level, boolean wait_for_settle)
{
    pm_err_flag_type                  err_flag = PM_ERR_FLAG_SUCCESS;
    uint32                            vmin = 0;  // microvolt
    uint32                            vStep = 0; // microvolt
    uint32                            calculated_vset = 0;
    pm_register_data_type             vset_ht[2] = { 0 };
    pm_register_address_type          reg = 0;

    boolean vreg_settled = FALSE;

    if (peripheral_index >= (pwr_data->num_of_peripherals) ||
        pwr_data->pwr_specific_info[peripheral_index].periph_type == 0)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    } 
    else
    {
        if ((volt_level <= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMax) &&
            (volt_level >= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin))
        {
            vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin;
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;
        }

        if (vStep > 0)
        {
            calculated_vset = (volt_level - vmin) / vStep;

            if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
            {
                vset_ht[0] = ((calculated_vset) & 0xFF);
                vset_ht[1] = ((calculated_vset >> 8) & 0xFF);

                reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VOLTAGE_CTRL1);

                // Set vset
                err_flag = pm_comm_write_byte_array(comm_ptr->slave_id, reg, 2, vset_ht, 0);

                if ((wait_for_settle == TRUE) && (err_flag == PM_ERR_FLAG_SUCCESS))
                {
                    err_flag = pm_pwr_sw_vreg_settled_check_alg(pwr_data, comm_ptr, peripheral_index, &vreg_settled);

                    if (vreg_settled == FALSE)
                    {
                        err_flag |= PM_ERR_FLAG_FAILURE;
                    }
                }
            } 
            else
            {
                err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
            }
        } 
        else
        {
            err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
        }
    }

    return err_flag;
}