/*! \file
 *  \n
 *  \brief  pm_xo_driver.c
 *  \details  
 *  \n &copy; Copyright 2018 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/xo/src/pm_xo_driver.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/10/13   rh      File created
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_xo_driver.h"
#include "CoreVerify.h"
#include "pm_version_p.h"
#include "device_info.h"
#include "pm_malloc.h"
#include "pm_ulog.h"

/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

#define PM_XO_HW_MAX_VER_SUPPORTED 0x60 // 1st bit are dig_major, 0 bit dig_minor = 6.0


/* Static global variable to store the xo data */
static pm_xo_core_data_type *pm_xo_core_data_arr[PM_MAX_NUM_PMICS];

static pm_xo_core_register_info_type xocore_reg = 
{
  .base_address       = 0x5000,
  .peripheral_offset  = 0x100,
  .voltage_ctl1       = 0x040,
  .mode_ctl1          = 0x044,
  .en_ctl             = 0x046,
  .xo_dcc_adj         = 0x056,
  .xo_comp            = 0x058,
  .xo_stepper         = 0x05A,
  .xo_adj             = 0x05C,
  .xo_ctl             = 0x05E,
};

static pm_xo_core_register_info_type xocore_reg_v2 =
{
  .base_address       = 0x5000,
  .peripheral_offset  = 0x100,
  .voltage_ctl1       = 0x040,
  .mode_ctl1          = 0x063,
  .en_ctl             = 0x046,
  .xo_dcc_adj         = 0x056,
  .xo_comp            = 0x058,
  .xo_stepper         = 0x05A,
  .xo_adj             = 0x05B,
  .xo_ctl             = 0x05E,
};

static pm_xo_core_limits xocore_limits =
{0x3F, 0x1F, 0x7F};

static pm_xo_core_limits xocore_limits_v2 =
{0x3F, 0x1F, 0xFF};

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_xo_core_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_xo_core_data_type *xo_ptr = NULL;
    pm_target_data_type *target_ptr = pm_target_info_target_data();

    if (target_ptr == NULL)
    {
      PM_LOG_MSG_ERR_FATAL("Unable to find target_data");
    }

    xo_ptr = pm_xo_core_data_arr[pmic_index];
    
    if (xo_ptr == NULL)
    {
        pm_malloc( sizeof(pm_xo_core_data_type), (void**)&xo_ptr);
                                                    
        /* Assign Comm ptr */
        xo_ptr->comm_ptr = comm_ptr;

        if ((peripheral_info->digital_major_version | peripheral_info->digital_minor_version)  > PM_XO_HW_MAX_VER_SUPPORTED )
        {
            CORE_VERIFY(0);
        }

        if(peripheral_info->digital_major_version >= 0x5)
        {
            xo_ptr->xo_core_register = &xocore_reg_v2;
            xo_ptr->xocore_limits    = &xocore_limits_v2;
            xo_ptr->version_update   = TRUE;
        }
        else
        {
        xo_ptr->xo_core_register = &xocore_reg;
            xo_ptr->xocore_limits    = &xocore_limits;
            xo_ptr->version_update   = FALSE;
        }

        if(xo_ptr->xo_core_register == NULL)
        {
           PM_LOG_MSG_ERR_FATAL("Failed to retrieve register addresses");
        }

        if(xo_ptr->xocore_limits == NULL)
        {
           PM_LOG_MSG_ERR_FATAL("Failed to retrieve limits information");
        }
        
        xo_ptr->num_of_peripherals = target_ptr->num_of_xo[pmic_index];

        /* Num of peripherals cannot be 0 if this driver init gets called */
        CORE_VERIFY(xo_ptr->num_of_peripherals != 0);
        pm_xo_core_data_arr[pmic_index] = xo_ptr;  
    }
}

pm_xo_core_data_type* pm_xo_core_get_data(uint8 pmic_index)
{
    uint8 pmk_index = 0;
    if (pm_target_info_pmk_present(&pmk_index) == TRUE)
        pmic_index = pmk_index;

    if(pmic_index < PM_MAX_NUM_PMICS)
    {
        return pm_xo_core_data_arr[pmic_index];
    }

    return NULL;
}

uint8 pm_xo_core_get_num_peripherals(uint8 pmic_index)
{
  if( (pmic_index < PM_MAX_NUM_PMICS) && 
      (pm_xo_core_data_arr[pmic_index] != NULL))
  {
      return pm_xo_core_data_arr[pmic_index]->num_of_peripherals;
  }

  return NULL;
}


