#ifndef __PM_XO_CORE_P_H__
#define __PM_XO_CORE_P_H__

/** @file pm_xo_core.h
*
*   PMIC-MEGA XO CORE RELATED DECLARATION 
*   This header file contains functions and variable declarations 
*  to support Qualcomm PMIC MEGA XO module. 
*/
/*
 *  Copyright (c) 2012-2013 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
*/

/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/inc/pm_xo_core_p.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/12   hs      Initial version. 
========================================================================== */
#include "comdef.h"
#include "pm_err_flags.h"
#include "pm_xo_core.h"
/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/** @addtogroup pm_xo_core
@{ */

/** Crystal oscillator core power modes. */
typedef enum
{
    /**
     * Normal power mode
     */
    PM_XO_CORE_PWR_MODE__NPM, 
    /**
     * High power mode
     */
    PM_XO_CORE_PWR_MODE__HPM
} pm_xo_core_power_mode_type;

/*===========================================================================

                 SMBC DRIVER FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

FUNCTION FUNCTION pm_mega_xo_set_power_mode                EXTERNAL FUNCTION
===========================================================================*/

/**
Sets the XO core power mode.

@param[in] pmic_chip Select the device in which the coin cell charger
                             being controlled is located. Device index starts
                             with zero.
@param[in] externalResourceIndex External resource index.
@param[in] mode XO core power mode. See #pm_xo_core_power_mode_type.

@return 
 SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
 None.

@sideeffects
Interrupts are disabled while communicating with the PMIC.
*/
pm_err_flag_type pm_xo_core_set_power_mode(uint8 pmic_chip, 
                                           int externalResourceIndex, 
                                           pm_xo_core_power_mode_type mode);


/*===========================================================================

FUNCTION FUNCTION pm_xo_set_xo_trim                     EXTERNAL FUNCTION
===========================================================================*/

/**
Trims the 19.2M Hz XO load capacitances.

@param[in] trim_value Raw trim value to be written, in the range of 0 to 63
                      (uint8).

@return 
 SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
pm_init() must have been called.

@sideeffects
Interrupts are disabled while communicating with the PMIC.
*/
//#pragma message("Do not use pm_mega_xo_set_xo_trim(), which is deprecated. Use pm_dev_mega_xo_set_xo_trim() instead.")
pm_err_flag_type pm_mega_xo_set_xo_trim(uint8 pmic_chip, int externalResourceIndex, uint8 trim_value);
/*===========================================================================

FUNCTION FUNCTION pm_xo_get_xo_trim                     EXTERNAL FUNCTION
===========================================================================*/

/**
Gets the raw trim value of the load capacitances for the 19.2 MHz XO.

@param[out] trim_value Pointer to the raw trim value.

@return
 Raw trim value in the range of 0 to 63. \n
 A value of 0xFF is an error code that indicates a read failure.

@dependencies
pm_init() must have been called.

@sideeffects
Interrupts are disabled while communicating with the PMIC.
*/
pm_err_flag_type pm_xo_core_get_xo_trim(uint8 pmic_chip, int externalResourceIndex, uint8* trim_value);
/**
 * Forces the XO core on if pm_xo_enable(TRUE) has been called.
 * 
 *  * @param[in] xo_core_enable TRUE -- Force the XO core on. \n
 *                           FALSE -- Leave the XO core off.
 * 
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type  pm_xo_core_set_xo_core_force_on(uint8 pmic_chip, int externalResourceIndex, boolean xo_core_enable);
/*===========================================================================

FUNCTION FUNCTION pm_xo_core_set_xo_comp                     EXTERNAL FUNCTION
===========================================================================*/

/**
set xo comp data.

@param[in] cap_value Raw cap value to be written

@return 
 SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
pm_init() must have been called.

@sideeffects
Interrupts are disabled while communicating with the PMIC.
*/
// Use below api to set any pmic specific xo_comp data
pm_err_flag_type pm_xo_core_set_xo_comp(uint8 pmic_chip, uint8 cap_value);
/*===========================================================================

FUNCTION FUNCTION pm_xo_core_get_xo_comp                     EXTERNAL FUNCTION
===========================================================================*/

/**
Get xo comp data.

@param[out] trim_value Pointer to the raw trim value.

@return
 Raw cap value 

@dependencies
pm_init() must have been called.

@sideeffects
Interrupts are disabled while communicating with the PMIC.
*/
// Use below api to get any pmic specific xo_comp data
pm_err_flag_type pm_xo_core_get_xo_comp(uint8 pmic_chip, uint8* cap_value);

/*===========================================================================

FUNCTION FUNCTION pm_xo_core_set_xo_dcc_adj                     EXTERNAL FUNCTION
===========================================================================*/

/**
set xo dcc adjustment data.

@param[in] dcc_adjust raw adjustment value to be written

@return 
 SUCCESS or Error -- See #pm_err_flag_type.

@dependencies
pm_init() must have been called.

@sideeffects
Interrupts are disabled while communicating with the PMIC.
*/
// Use below api to set any pmic specific xo_dcc_adj data
pm_err_flag_type pm_xo_core_set_xo_dcc_adj(uint8 pmic_chip, uint8 dcc_adjust);
/*===========================================================================

FUNCTION FUNCTION pm_xo_core_get_xo_dcc_adj                     EXTERNAL FUNCTION
===========================================================================*/

/**
Get xo dcc adjustment data.

@param[out] dcc_adjust Pointer to the raw value.

@return
 Raw dcc_adjust value 

@dependencies
pm_init() must have been called.

@sideeffects
Interrupts are disabled while communicating with the PMIC.
*/
// Use below api to get any pmic specific xo_dcc_adj data
pm_err_flag_type pm_xo_core_get_xo_dcc_adj(uint8 pmic_chip, uint8* dcc_adjust);

/** @} */ /* end_addtogroup pm_xo_core */

#endif /* __PM_XO_CORE_P_H__ */
