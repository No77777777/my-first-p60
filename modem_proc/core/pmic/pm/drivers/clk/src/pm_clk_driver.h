#ifndef __PM_CLK_DRIVER_H__
#define __PM_CLK_DRIVER_H__

/*! \file pm_clk_driver.h 
*  \n
*  \brief This file contains functions prototypes and variable/type/constant
*  declarations for supporting clk peripheral 
*  
*  \n &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/clk/src/pm_clk_driver.h#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
12/10/13   rh      File created
========================================================================== */
/*===========================================================================

                        INCLUDE FILES

===========================================================================*/
#include "pm_err_flags.h"
#include "hw_module_type.h"
#include "pm_comm.h"
#include "pm_clk_p.h"

/*===========================================================================

                     STRUCTURE TYPE AND ENUM

===========================================================================*/

#define PM_CLK_INVALID_DATA   99

/* Please make sure to change the clk_info and clk_dep info arrays in
   config folder when the below macro is changed */
#define PM_MAX_NUM_CLKS       14

typedef struct 
{ 
    pm_register_address_type    base_address;  
    pm_register_address_type    peripheral_offset;
    pm_register_address_type    DRV_CTL1;   //0x44
    pm_register_address_type    EN_CTL;     //0x46
    pm_register_address_type    PD_CTL;     //0x48
    pm_register_address_type    DIV_CTL1;   //0x43
    pm_register_address_type    STATUS1;    //0x08
    pm_register_address_type    CLK_HOLD;   //0x45
}pm_clk_register_info_type;

typedef struct
{
    pm_register_address_type    SLPSRC_CTRL1;       
}pm_clk_sleep_register_info_type;

typedef struct 
{
    pm_comm_info_type               *comm_ptr;
    uint32                          num_of_peripherals;
    uint8                           periph_subtype[PM_MAX_NUM_CLKS];
    //array of clk peripheral index  to get to right offset from base
    uint8                           clk_perph_index[PM_ALL_CLKS];
    pm_clk_type                     *clk_common;
    pm_clk_register_info_type       *clk_reg_table;
    pm_clk_sleep_register_info_type *clk_sleep_reg_table;

    boolean                         clk_hold_supported[PM_MAX_NUM_CLKS];  
}pm_clk_data_type;

/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_clk_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_clk_data_type* pm_clk_get_data(uint8 pmic_index);

uint8 pm_clk_get_num_peripherals(uint8 pmic_index);



/*===========================================================================

FUNCTION pm_clk_buff_set_output_drive_strength                EXTERNAL FUNCTION

===========================================================================*/

/**
*  Returns the clk on/off (CLK_OK) status.
*
* @param[in] pmic_chip Selects the PMIC chip in which the buffers being 
*                      controlled are located. The device index starts with
*                      zero.
* @param[in] periph Selects the clock peripheral that is to be pin
*                   controlled. See #pm_clk_type. 
* @param[out] on_off Returns the clk on/off status.
* 
* @return
* SUCCESS or Error -- See #pm_err_flag_type.
*/
pm_err_flag_type pm_clk_ok_status(uint8 pmic_chip, pm_clk_type periph, pm_on_off_type *on_off);



pm_err_flag_type
pm_clk_sw_enable(uint32 pmic_chip,
                 pm_clk_type periph,
                 pm_on_off_type on_off);


#endif /* PM_CLK_DRIVER_H */

