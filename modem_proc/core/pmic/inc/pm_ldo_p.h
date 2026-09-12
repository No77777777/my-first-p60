#ifndef __PM_LDO_P_H__
#define __PM_LDO_P_H__

/** @file pm_ldo_p.h 
 *  
 *  This header file contains enums and API definitions for LDO
 *  power rail driver.
*/
/*
 *  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/inc/pm_ldo_p.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/18   rl      Rearchitecturing module driver to peripheral driver
=============================================================================*/

/*===========================================================================

                 HEADER FILE INCLUDE

===========================================================================*/
#include "pm_ldo.h"

/*===========================================================================

                 FUNCTION DEFINITIONS

===========================================================================*/

/** 
 *  Returns the mode status (LPM, NPM, AUTO, BYPASS)
 *  of the selected power rail.
 *  
 * @note1hang The mode of a regulator changes dynamically.
 * 
 * @param[in]  pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in]  ldo_peripheral_index LDO peripheral index.
 *                Starts from 0 (for the first LDO peripheral).
 * @param[out] sw_mode Variable returned to the caller with the mode status.
 *                Refer to pm_resources_and_types.h for the enum info.
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_ldo_sw_mode_status ( uint8 pmic_chip, 
                        uint8 ldo_peripheral_index, 
                        pm_sw_mode_type* sw_mode );

/**
 *  Returns the software enable status (On/Off) 
 *        of the selected power rail.
 * 
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in] ldo_peripheral_index LDO peripheral index.
 *                Starts from 0 (for the first LDO peripheral).
 * @param[out] on_off Variable returned to the caller with the software
 *                status. Refer to pm_resources_and_types.h for
 *                the enum info.
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type
pm_ldo_sw_enable_status ( uint8 pmic_chip,
                          uint8 ldo_peripheral_index,
                          pm_on_off_type* on_off );


#endif /* __PM_LDO_P_H__ */
