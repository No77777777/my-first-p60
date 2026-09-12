/*! \file
*  
*  \brief   ----File contains the implementation of the public APIs for GPIO resource type.
*  \details Implementation file for GPIO resourece type.
*  Each of the APIs checks for access and then if necessary directs
*  the call to Driver implementation or to RPC function for the master processor.
*  
*    PMIC code generation Version: 2.0.0.19
*    This file contains code for Target specific settings and modes.
*  
*  &copy; Copyright 2010-2012 Qualcomm Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/vadc/src/pm_vadc.c#1 $ 


when           who          what, where, why
--------       ---          ----------------------------------------------------------     
01/26/16        rl          Created
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_vadc_driver.h"
#include "pm_comm.h"
#include "pm_ulog.h"
/*===========================================================================

                        Definitions

===========================================================================*/


#define BASE_ADDR_VADC (vadc_ptr->vadc_register->base_address)

#define REG_VADC(reg) ((vadc_ptr->vadc_register->base_address) \
                      + (vadc_ptr->vadc_register->reg))

#define SLAVE_ID_VADC (vadc_ptr->comm_ptr->slave_id)

#define GET_VADC_ERR_CODE(ptr) ptr==NULL ?                           \
                               PM_ERR_FLAG_FEATURE_NOT_SUPPORTED :  \
                               PM_ERR_FLAG_INVALID_PARAMETER

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/


static uint8 pm_vadc_get_irq_mask(pm_vadc_btm_thr_type);


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/


pm_err_flag_type pm_vadc_set_thr_voltage(uint8 pmic_index, 
                                        pm_vadc_btm_thr_type thr_type, 
                                        uint32 volt_mV)
{
    pm_vadc_data_type *vadc_ptr = pm_vadc_get_data(pmic_index);
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type voltage_thr_reg=0; 
    uint8 volt_val;
    
    if((thr_type>=PM_VADC_BTM_THR_INVALID) || 
       (vadc_ptr == NULL) ||
       (volt_mV < vadc_ptr->limits->min_mv) || 
       (volt_mV > vadc_ptr->limits->max_mv))
    {
        err_flag = GET_VADC_ERR_CODE(vadc_ptr);             
        PM_LOG_MSG_ERROR(err_flag, "error = %d, pmic=%d, thr_type=%d, mV=%d", 
                         err_flag, pmic_index, thr_type, volt_mV);       
        return err_flag;
    }
    
    switch (thr_type)
    {
        case PM_VADC_BTM_LOW1_THR:
            voltage_thr_reg = REG_VADC(vcomp_low1_thr);
            break;
        case PM_VADC_BTM_LOW2_THR:
            voltage_thr_reg = REG_VADC(vcomp_low2_thr);
            break;
        case PM_VADC_BTM_HI1_THR:
            voltage_thr_reg = REG_VADC(vcomp_hi_thr);
            break;          
        default:
      CORE_VERIFY(0);
            break;
    }
    
    volt_val = (uint8)((volt_mV - vadc_ptr->limits->min_mv)/
                        vadc_ptr->limits->step_mv);
    
    err_flag|=pm_comm_write_byte(SLAVE_ID_VADC, voltage_thr_reg, volt_val, 0);
    
    PM_LOG_MSG_ERROR(err_flag,"error = %d", err_flag);
    
    return err_flag;    
}

pm_err_flag_type pm_vadc_irq_enable(uint8 pmic_index, 
                                    pm_vadc_btm_thr_type thr_type, 
                                    boolean enabled)
{
    pm_vadc_data_type *vadc_ptr = pm_vadc_get_data(pmic_index);
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS; 
    uint8 irq_mask;
    
    if((thr_type>=PM_VADC_BTM_THR_INVALID) || (vadc_ptr == NULL))
    {
        err_flag = GET_VADC_ERR_CODE(vadc_ptr);
        PM_LOG_MSG_ERROR(err_flag,"error = %d, pmic=%d, thr_type=%d, enable=%d", 
                         err_flag, pmic_index, thr_type, enabled);
        return err_flag;     
    }
    
    irq_mask = pm_vadc_get_irq_mask(thr_type);
    
    err_flag |= pm_comm_write_byte(SLAVE_ID_VADC, 
                (enabled==TRUE ? REG_VADC(int_en_set) : REG_VADC(int_en_clr)), 
                irq_mask, 0);

    PM_LOG_MSG_ERROR(err_flag,"error = %d", err_flag);
    
    return err_flag; 
}


pm_err_flag_type pm_vadc_irq_clear(uint8 pmic_index, 
                                  pm_vadc_btm_thr_type thr_type)
{
    pm_vadc_data_type *vadc_ptr = pm_vadc_get_data(pmic_index);
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS; 
    uint8 irq_mask;
    
    if((thr_type>=PM_VADC_BTM_THR_INVALID) || (vadc_ptr == NULL))
    {
        err_flag = GET_VADC_ERR_CODE(vadc_ptr);        
        PM_LOG_MSG_ERROR(err_flag,"error = %d, pmic=%d, thr_type=%d", 
                         err_flag, pmic_index, thr_type);
        return err_flag;
    }

    irq_mask = pm_vadc_get_irq_mask(thr_type);
    
    err_flag |= pm_comm_write_byte(SLAVE_ID_VADC, REG_VADC(int_latched_clr), irq_mask, 0);

    PM_LOG_MSG_ERROR(err_flag,"error = %d", err_flag);
    
    return err_flag; 
}



pm_err_flag_type pm_vadc_register_isr(uint8 pmic_index, 
                                      pm_vadc_btm_thr_type thr_type, 
                                      void *isr_ptr)
{
    pm_vadc_data_type *vadc_ptr = pm_vadc_get_data(pmic_index);
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS; 
    uint8 irq_mask; 
    uint32 isr_ctx;
    
    if((thr_type>=PM_VADC_BTM_THR_INVALID) || 
       (vadc_ptr == NULL) || 
       (isr_ptr==NULL))
    {
        err_flag = GET_VADC_ERR_CODE(vadc_ptr);
        PM_LOG_MSG_ERROR(err_flag,"error = %d, pmic=%d, thr_type=%d, isr=0x%x", 
                         err_flag, pmic_index, thr_type, isr_ptr);
        return err_flag; 
    }
    
    irq_mask = pm_vadc_get_irq_mask(thr_type);
    
    //Get CTX save slave id and peripheral id in ctx
    //MSB 16 bits are slave id, LSB 16 bits are peripheral index
    isr_ctx = (((uint32)(SLAVE_ID_VADC) & 0xFFFF)<<16) | ((uint32)(BASE_ADDR_VADC) >> 8);
    
    err_flag=pm_comm_register_isr(SLAVE_ID_VADC, BASE_ADDR_VADC, irq_mask, isr_ptr, isr_ctx);

    PM_LOG_MSG_ERROR(err_flag,"error = %d", err_flag);
    
    return err_flag;
}

static uint8 pm_vadc_get_irq_mask(pm_vadc_btm_thr_type thr_type)
{
    uint8 irq_mask=0;
    switch(thr_type)
    {
        case PM_VADC_BTM_LOW1_THR:
            irq_mask = 0x02;
            break;
        case PM_VADC_BTM_LOW2_THR:
            irq_mask = 0x08;
            break;
        case PM_VADC_BTM_HI1_THR:
            irq_mask = 0x20;
        default:
            break;
    }
    return irq_mask;
}


