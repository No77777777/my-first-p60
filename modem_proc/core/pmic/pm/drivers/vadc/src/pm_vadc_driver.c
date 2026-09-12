/*! \file
 *  \n
 *  \brief  pm_vadc_driver.c 
 *  \details  
 *  \n &copy; Copyright 2010-2013 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/vadc/src/pm_vadc_driver.c#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
02/22/16    rl      Initial Creation
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_vadc_driver.h"
#include "CoreVerify.h"
#include "pm_target_information.h"
#include "pm_version.h"
#include "device_info.h"
#include "pm_malloc.h"
#include "hw_module_type.h"
#include "pm_ulog.h"
/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the BCL data */
static pm_vadc_data_type *pm_vadc_data_arr[PM_MAX_NUM_PMICS];

static pm_vadc_register_info_type vadc_reg = 
{
  0x3800, 0x14, 0x15, 0x16, 0x48, 0x49, 0x4A
};

pm_vadc_btm_limits vadc_btm_limits =
{ // min_mV, max_mV, step_mV
  2250,      5425,   25    
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_vadc_driver_init(pm_comm_info_type *comm_ptr, 
                         peripheral_info_type *peripheral_info, 
                         uint8 pmic_index)
{
    pm_vadc_data_type *vadc_ptr = NULL;
    
    vadc_ptr = pm_vadc_data_arr[pmic_index];    
        
    if (vadc_ptr == NULL)
    {
        pm_malloc( sizeof(pm_vadc_data_type), (void**)&vadc_ptr);
            
        vadc_ptr->comm_ptr = comm_ptr;
        
    vadc_ptr->vadc_register = &vadc_reg;
        if(vadc_ptr->vadc_register==NULL)
    {
            PM_LOG_MSG_ERR_FATAL("Failed to retrieve register addresses");
    }
        
    vadc_ptr->limits = &vadc_btm_limits;
        if(vadc_ptr->limits == NULL)
    {
            PM_LOG_MSG_ERR_FATAL("Failed to retrieve limits information");
    }
       
        pm_vadc_data_arr[pmic_index] = vadc_ptr;
    }
  return;
}

pm_vadc_data_type* pm_vadc_get_data(uint8 pmic_index)
{
    if(pmic_index < PM_MAX_NUM_PMICS) 
    {
        return pm_vadc_data_arr[pmic_index];
    }

    return NULL;
}
