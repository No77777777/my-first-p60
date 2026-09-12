/*! \file
*  
*  \brief  mpp_app.cpp ----File contains the implementation of the public APIs for XO resource type.
*  \details Implementation file for XO resource type.
*
*  &copy; Copyright 2013 Qualcomm Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/xo/src/pm_xo.c#1 $ 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/10/13   rh      File created

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_xo_core_p.h"
#include "pm_xo_driver.h"

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
pm_err_flag_type pm_xo_core_set_power_mode(uint8 pmic_chip, int internalResourceIndex, pm_xo_core_power_mode_type  mode)
{
    pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
    pm_register_address_type reg = 0;
    uint8  reg_mask = 0x0;
    uint8  mode_data = 0x0;
	
    if (xo_ptr == NULL) 
    {
        errFlag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if  ( internalResourceIndex >= xo_ptr->num_of_peripherals )
    {   
        errFlag = PM_ERR_FLAG_INVALID_PERIPHERAL ;
    }
    else if( mode > PM_XO_CORE_PWR_MODE__HPM )
    {
        errFlag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {            
       if(xo_ptr->version_update)
       {
          reg_mask = 0x80;
          mode_data = mode << 7;
       }
       else
       {
          reg_mask = 0x1;
          mode_data = mode;
       }
           
        reg = xo_ptr->xo_core_register->base_address + (pm_register_address_type)(xo_ptr->xo_core_register->peripheral_offset*internalResourceIndex) + 
              xo_ptr->xo_core_register->mode_ctl1; 
                  
        errFlag = pm_comm_write_byte_mask(xo_ptr->comm_ptr->slave_id,  reg, reg_mask, mode_data, 0);
    }

    /* Let the user know if we were successful or not */
    return errFlag;
}

pm_err_flag_type pm_dev_mega_xo_set_xo_trim(uint8 pmic_chip, int internalResourceIndex, uint8 trim_value)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG_SUCCESS;
    pm_err_flag_type      err       = PM_ERR_FLAG_SUCCESS;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
    uint8  reg_mask = 0x0;

    if (xo_ptr == NULL) 
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if  ( internalResourceIndex >= xo_ptr->num_of_peripherals )
    {   
        return PM_ERR_FLAG_INVALID_PERIPHERAL ;
    }
	
    pm_register_address_type reg = xo_ptr->xo_core_register->base_address + (pm_register_address_type)(xo_ptr->xo_core_register->peripheral_offset*internalResourceIndex) + xo_ptr->xo_core_register->xo_adj;

    if(trim_value > (xo_ptr->xocore_limits->trim_max))
    {
        /* Round the value to the boundary */
	trim_value = xo_ptr->xocore_limits->trim_max;

        /* Set error flag */
	err = PM_ERR_FLAG_INVALID_PARAMETER;
     }

     reg_mask = xo_ptr->xocore_limits->trim_max;
	
     errFlag = pm_comm_write_byte_mask(xo_ptr->comm_ptr->slave_id,  reg, reg_mask, (pm_register_data_type)trim_value, 0 );

    if(err == PM_ERR_FLAG_SUCCESS)
    {
        return errFlag;
    }
    else
    {
        return err;
    }
}

pm_err_flag_type pm_dev_mega_xo_get_xo_trim(uint8 pmic_chip, int internalResourceIndex, uint8*  trim_value)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
    uint8  reg_mask = 0x0;
	
    if (xo_ptr == NULL) 
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if  ( internalResourceIndex >= xo_ptr->num_of_peripherals )
    {   
        return PM_ERR_FLAG_INVALID_PERIPHERAL ;
    }
    if(NULL == trim_value)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER ;
    }
	
    pm_register_address_type reg = xo_ptr->xo_core_register->base_address + (pm_register_address_type)(xo_ptr->xo_core_register->peripheral_offset*internalResourceIndex) + xo_ptr->xo_core_register->xo_adj;
	
     errFlag = pm_comm_read_byte(xo_ptr->comm_ptr->slave_id, reg, &data, 0); 

     if (errFlag == PM_ERR_FLAG_SUCCESS)
     {
         reg_mask = xo_ptr->xocore_limits->trim_max;
        *trim_value = (uint8)(data & reg_mask);
      }

    return errFlag;
}

pm_err_flag_type pm_xo_core_set_xo_core_force_on(uint8 pmic_chip, int internalResourceIndex,boolean  xo_core_enable)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG_SUCCESS;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
	
    if (xo_ptr == NULL) 
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
	if ( internalResourceIndex >= xo_ptr->num_of_peripherals )
    {   
        return PM_ERR_FLAG_INVALID_PERIPHERAL ;
    }
	
    pm_register_data_type    data = 0;
    pm_register_address_type reg = xo_ptr->xo_core_register->base_address + (pm_register_address_type)(xo_ptr->xo_core_register->peripheral_offset*internalResourceIndex) + xo_ptr->xo_core_register->en_ctl;

    if(TRUE == xo_core_enable)
    {
        data = 0xFF;
    }
    else
    {
        data = 0;
     }

     errFlag = pm_comm_write_byte_mask(xo_ptr->comm_ptr->slave_id,  reg, 0x80, data, 0 ); // 7

    return errFlag;
}

pm_err_flag_type pmapp_xo_core_set_xo_comp(uint8 cap_value)
{
    return pm_xo_core_set_xo_comp(0, cap_value);
}

pm_err_flag_type pm_xo_core_set_xo_comp(uint8 pmic_chip, uint8 cap_value)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG_SUCCESS;
    pm_err_flag_type      err       = PM_ERR_FLAG_SUCCESS;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
    uint8  reg_mask = 0x0;

    if (xo_ptr == NULL) 
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if(cap_value > (xo_ptr->xocore_limits->cap_val_max))
    {
       /* Round the value to the boundary */
       cap_value = xo_ptr->xocore_limits->cap_val_max;
       
       /* Set error flag */
       err = PM_ERR_FLAG_INVALID_PARAMETER;
    }

    reg_mask = xo_ptr->xocore_limits->cap_val_max;

    pm_register_address_type reg = xo_ptr->xo_core_register->base_address + xo_ptr->xo_core_register->xo_comp;
    errFlag = pm_comm_write_byte_mask(xo_ptr->comm_ptr->slave_id,  reg, reg_mask, (pm_register_data_type)cap_value, 0 );

    if(err == PM_ERR_FLAG_SUCCESS)
    {
        return errFlag;
    }
    else
    {
        return err;
    }

}

pm_err_flag_type pmapp_xo_core_get_xo_comp(uint8* cap_value)
{
    return pm_xo_core_get_xo_comp(0, cap_value);
}

pm_err_flag_type pm_xo_core_get_xo_comp(uint8 pmic_chip, uint8* cap_value)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
    uint8  reg_mask = 0x0;
	
    if (xo_ptr == NULL) 
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if(NULL == cap_value)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER ;
    }
	
    pm_register_address_type reg = xo_ptr->xo_core_register->base_address + xo_ptr->xo_core_register->xo_comp;
	
    errFlag = pm_comm_read_byte(xo_ptr->comm_ptr->slave_id, reg, &data, 0); 

    if (errFlag == PM_ERR_FLAG_SUCCESS)
    {
        
        reg_mask = xo_ptr->xocore_limits->cap_val_max;
        *cap_value = (uint8)(data & reg_mask);
    }

    return errFlag;

}

pm_err_flag_type pmapp_xo_core_set_xo_dcc_adj(uint8 dcc_adjust)
{
    return pm_xo_core_set_xo_dcc_adj(0, dcc_adjust);
}

pm_err_flag_type pm_xo_core_set_xo_dcc_adj(uint8 pmic_chip, uint8 dcc_adjust)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG_SUCCESS;
    pm_err_flag_type      err       = PM_ERR_FLAG_SUCCESS;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
    uint8  reg_mask = 0;

    if (xo_ptr == NULL) 
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if(dcc_adjust > (xo_ptr->xocore_limits->dcc_adjust_max))
    {
       /* Round the value to the boundary */
       dcc_adjust = xo_ptr->xocore_limits->dcc_adjust_max;
       
       /* Set error flag */
       err = PM_ERR_FLAG_INVALID_PARAMETER;
    }

    reg_mask = xo_ptr->xocore_limits->dcc_adjust_max;

    pm_register_address_type reg = xo_ptr->xo_core_register->base_address + xo_ptr->xo_core_register->xo_dcc_adj;
    errFlag = pm_comm_write_byte_mask(xo_ptr->comm_ptr->slave_id,  reg, reg_mask, (pm_register_data_type)dcc_adjust, 0 );

    if(err == PM_ERR_FLAG_SUCCESS)
    {
        return errFlag;
    }
    else
    {
        return err;
    }

}

pm_err_flag_type pmapp_xo_core_get_xo_dcc_adj(uint8* dcc_adjust)
{
    return pm_xo_core_get_xo_dcc_adj(0, dcc_adjust);
}

pm_err_flag_type pm_xo_core_get_xo_dcc_adj(uint8 pmic_chip, uint8* dcc_adjust)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_xo_core_data_type *xo_ptr = pm_xo_core_get_data(pmic_chip);
    uint8  reg_mask = 0x0;
	
    if (xo_ptr == NULL) 
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if(NULL == dcc_adjust)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER ;
    }
	
    pm_register_address_type reg = xo_ptr->xo_core_register->base_address + xo_ptr->xo_core_register->xo_dcc_adj;
	
    errFlag = pm_comm_read_byte(xo_ptr->comm_ptr->slave_id, reg, &data, 0); 

    if (errFlag == PM_ERR_FLAG_SUCCESS)
    {
        reg_mask = xo_ptr->xocore_limits->dcc_adjust_max;
        *dcc_adjust = (uint8)(data & reg_mask); 
    }

    return errFlag;

}

/*===========================================================================

                        DEPRECATED FUNCTIONS

===========================================================================*/
pm_err_flag_type pm_mega_xo_set_xo_trim(uint8 pmic_chip, int externalResourceIndex, uint8 trim_value)
{
	return pm_dev_mega_xo_set_xo_trim(0, externalResourceIndex, trim_value);
}
pm_err_flag_type pm_xo_core_get_xo_trim(uint8 pmic_chip, int externalResourceIndex, uint8* trim_value)
{
	return pm_dev_mega_xo_get_xo_trim(0, externalResourceIndex, trim_value);
}

