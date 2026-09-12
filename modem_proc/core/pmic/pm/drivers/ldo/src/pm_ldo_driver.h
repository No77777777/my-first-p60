#ifndef PM_LDO_DRIVER__H
#define PM_LDO_DRIVER__H

/*! \file pm_ldo_driver.h
 *  \n
 *  \brief This file contains LDO peripheral driver related function prototypes,
 *         enums and driver data structure type.   
 *  \n  
 *  \n &copy; Copyright 2012-2014 QUALCOMM Technologies Incorporated, All Rights Reserved
 */
/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/ldo/src/pm_ldo_driver.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/16/14   aks     Adding support for multiple PMICS > 2  
12/06/12   hw      Rearchitecturing module driver to peripheral driver
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_pwr_alg.h"
#include "pm_comm.h"
#include "pm_ldo_p.h"

/*===========================================================================

                     STRUCTURE TYPE AND ENUM

===========================================================================*/



/*===========================================================================

                     LDO TYPES AND STRUCTURES 

===========================================================================*/
typedef struct
{
    pm_comm_info_type  *comm_ptr;
    pm_pwr_data_type  pm_pwr_data;
    uint8              *in_bypass;
    uint8              *periph_subtype;
} pm_ldo_data_type;

/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_ldo_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_ldo_data_type* pm_ldo_get_data(uint8 pmic_index);

uint8 pm_ldo_get_num_peripherals(uint8 pmic_index);

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/


/** Switches between NPM, LPM, and other modes of a regulator.

    @param[in] pmic_chip            Primary PMIC: 0, Secondary PMIC: 1.
    
    @param[in] ldo_peripheral_index LDO peripheral index. Starts from 0
                                    (for the first LDO peripheral).
                                    See @xnameref{hdr:ldoPeripheralIndex}.

    @param[in] sw_mode              Selects the mode of a regulator, e.g.,
                                    HPM, LPM, BYPASS. See #pm_sw_mode_type.
 
    @return 
    SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_ldo_sw_mode
(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_sw_mode_type sw_mode);


/** 
 *  Returns the pin controlled status or the hardware 
 *        enable status (On/Off) of the selected power rail.
 * 
 * @param[in]  pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in]  ldo_peripheral_index LDO peripheral index.
 *                Starts from 0 (for the first LDO peripheral).
 * @param[out] on_off Variable returned to the caller with pin control
 *                status. Refer to pm_resources_and_types.h for
 *                the enum info.
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_ldo_pin_ctrled_status
(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_on_off_type* on_off);

/** 
 * @name pm_ldo_pin_ctrl_status 
 *  
 * @brief This function returns the pin control enable 
 *  status and also the selected pins being followed by the
 *  selected power rail enable.
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first LDO peripheral)
 * @param[out] select_pin Variable returned to the caller with 
 *                        selected pins being followed by the
 *                        power rail.
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG__SUCCESS = SUCCESS else ERROR.
 *
 */
pm_err_flag_type pm_ldo_pin_ctrl_status
(uint8 pmic_chip, uint8 perph_index, uint8 *select_pin);
 
#if 0
/** 
 *  Returns the local soft reset status of 
 *        the selected power rail.
 * 
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1
 * @param[in] ldo_peripheral_index LDO peripheral index.
 *                Starts from 0 (for the first LDO peripheral).
 * @param[out] status Variable returned to the caller with the soft reset
 *                status. Returns TRUE if the LDO is in soft reset
 *                and FALSE otherwise.
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_ldo_soft_reset_status
(uint8 pmic_chip, uint8 ldo_peripheral_index, boolean* status);

/** 
 *  Brings the selected power rail out of 
 *        local soft reset.
 * 
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in] ldo_peripheral_index LDO peripheral index.
 *                Starts from 0 (for the first LDO peripheral).
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_ldo_soft_reset_exit
(uint8 pmic_chip, uint8 ldo_peripheral_index);

#endif

/** 
 * Calculates the PMIC register value for
 *        a supplied voltage
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  ldo_peripheral_index:
 *                Starts from 0 (for first LDO peripheral)
 * @param[in]  volt_level:
 *                Voltage to convert to PMIC register value
 * @param[out] vset:
 *                PMIC register value for supplied voltage
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 *
 */
pm_err_flag_type pm_ldo_volt_calculate_vset
(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_volt_level_type  volt_level, uint32* vset);

/** 
 * Returns the stepper done status for
 *        the selected power rail
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  ldo_peripheral_index:
 *                Starts from 0 (for first LDO peripheral)
 * @param[out]  stepper_done:
 *                Value of stepper done register
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 *
 */
pm_err_flag_type pm_ldo_volt_level_stepper_done_status
(uint8 pmic_chip, uint8 ldo_peripheral_index, boolean *stepper_done);

pm_err_flag_type 
pm_ldo_volt_level(uint8 pmic_chip, 
                  uint8 ldo_peripheral_index,
                  pm_volt_level_type volt_level,
                  boolean wait_for_settle);

pm_err_flag_type 
pm_ldo_sw_enable(uint8 pmic_chip, 
                 uint8 ldo_peripheral_index,
                 pm_on_off_type on_off,
                 boolean wait_for_settle);


#endif /* PM_LDO_DRIVER__H */
