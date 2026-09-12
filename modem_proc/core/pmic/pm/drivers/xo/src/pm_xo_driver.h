#ifndef PM_XO_DRIVER__H
#define PM_XO_DRIVER__H

/*! \file
*  \n
*  \brief  pm_xo_driver.h
*  \details  This file contains functions prototypes and variable/type/constant
*  declarations for supporting XO pin services for the Qualcomm
*  PMIC chip set.
*  \n &copy; Copyright 2010-2013 Qualcomm Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/xo/src/pm_xo_driver.h#1 $

when         who      what, where, why
--------     ---      ----------------------------------------------------------
12/10/13   rh      File created
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_target_information.h"
#include "pm_config_target.h"
/*===========================================================================

                     TYPE DEFINITIONS 

===========================================================================*/
typedef struct
{
    pm_register_address_type    base_address;  
    pm_register_address_type    peripheral_offset;
    pm_register_address_type    voltage_ctl1;                               
    pm_register_address_type    mode_ctl1;                         
    pm_register_address_type    en_ctl;                                    
    pm_register_address_type    xo_dcc_adj;                              
    pm_register_address_type    xo_comp;         
    pm_register_address_type    xo_stepper;     
    pm_register_address_type    xo_adj;         
    pm_register_address_type    xo_ctl;         
}pm_xo_core_register_info_type;

typedef struct
{
  uint8              trim_max; //Maximum load capacitance.
  uint8              cap_val_max;
  uint8              dcc_adjust_max;
}pm_xo_core_limits;

typedef struct
{
    pm_comm_info_type              *comm_ptr;
    pm_xo_core_register_info_type  *xo_core_register;
    pm_xo_core_limits              *xocore_limits;

    uint8 num_of_peripherals;                
    boolean                         version_update;               
}pm_xo_core_data_type;
/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/
void pm_xo_core_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_xo_core_data_type* pm_xo_core_get_data(uint8 pmic_index);
uint8 pm_xo_core_get_num_peripherals(uint8 pmic_index);
#endif //PM_XO_DRIVER__H
