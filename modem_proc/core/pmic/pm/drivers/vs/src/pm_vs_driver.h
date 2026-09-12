#ifndef __PM_VS_DRIVER_H__
#define __PM_VS_DRIVER_H__

/*! \file pm_vs_driver.h
 *  \n
 *  \brief   
 *  \details  
 *  \n &copy; Copyright 2012-2013 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/vs/src/pm_vs_driver.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/06/12   hw      Rearchitecturing module driver to peripheral driver
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_pwr_alg.h"
#include "pm_comm.h"
#include "pm_vs_p.h"
/*===========================================================================

                     VS TYPES AND STRUCTURES 

===========================================================================*/

typedef struct
{
    pm_comm_info_type    *comm_ptr;
    pm_pwr_data_type  pm_pwr_data;
} pm_vs_data_type;


/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_vs_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_vs_data_type* pm_vs_get_data(uint8 pmic_index);

uint8 pm_vs_get_num_peripherals(uint8 pmic_index);

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/* pm_vs_sw_mode_status */
/**
 *  Returns the mode status (LPM, NPM, AUTO, BYPASS)
 *  of the selected power rail.
 * @note1hang The mode of a regulator changes dynamically.
 *
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] perph_index VS peripheral index.
 *                           Starts from 0 (for the first VS peripheral).
 * @param[out] sw_mode Variable returned to the caller with the mode status.
 * 
 * 
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_vs_sw_mode_status
(uint8 pmic_chip, uint8 perph_index, pm_sw_mode_type* sw_mode);


/* pm_vs_pin_ctrled_status */

/**
 *  Returns the pin controlled status or hardware 
 *        enable status (On/Off) of the selected power rail.
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] perph_index VS peripheral index.
 *                           Starts from 0 (for the first VS peripheral).
 * @param[out] on_off Variable returned to the caller with pin control
 *                status. Refer to pm_resources_and_types.h for
 *                the enum info.
 * 
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_vs_pin_ctrled_status
(uint8 pmic_chip, uint8 perph_index, pm_on_off_type* on_off);

/** 
 * @name pm_vs_pin_ctrl_status 
 *  
 * @brief This function returns the pin control enable 
 *  status and also the selected pins being followed by the
 *  selected power rail enable.
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first VS peripheral)
 * @param[out] select_pin Variable returned to the caller with 
 *                        selected pins being followed by the
 *                        power rail.
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG__SUCCESS = SUCCESS else ERROR.
 *
 */
pm_err_flag_type pm_vs_pin_ctrl_status
(uint8 pmic_chip, uint8 perph_index, uint8 *select_pin);

/**
 * @name pm_vs_vreg_ok_status 
 *  
 * @brief Returns the VREG_OK/VREG_READY status of the rail.
 * 
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in] vs_peripheral_index VS peripheral index. Starts 
 *       from 0 (for the first VS peripheral).
 * @param[out] on_off On/Off status.
 * 
 * @return 
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_vs_vreg_ok_status
(uint8 pmic_chip, uint8 vs_peripheral_index, boolean* on_off);

#endif // __PM_VS_DRIVER_H__
