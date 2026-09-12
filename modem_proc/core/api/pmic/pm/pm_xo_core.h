#ifndef __PM_XO_CORE_H__
#define __PM_XO_CORE_H__

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

$Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_xo_core.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/12   hs      Initial version. 
========================================================================== */
#include "comdef.h"
#include "pm_err_flags.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/** @addtogroup pm_xo_core
@{ */

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
pm_err_flag_type pm_dev_mega_xo_set_xo_trim(uint8 pmic_chip, int externalResourceIndex, uint8 trim_value);
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
pm_err_flag_type pm_dev_mega_xo_get_xo_trim(uint8 pmic_chip, int externalResourceIndex, uint8* trim_value);


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
// Below API will detect PMK and apply xo_comp to appropriate pmic
pm_err_flag_type pmapp_xo_core_set_xo_comp(uint8 cap_value);
/*===========================================================================

FUNCTION FUNCTION pmapp_xo_core_get_xo_comp                     EXTERNAL FUNCTION
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
// Below API will detect PMK and return data from appropriate pmic
pm_err_flag_type pmapp_xo_core_get_xo_comp(uint8* cap_value);

/*===========================================================================

FUNCTION FUNCTION pmapp_xo_core_set_xo_dcc_adj                     EXTERNAL FUNCTION
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
// Below API will detect PMK and apply xo_dcc_adj to appropriate pmic
pm_err_flag_type pmapp_xo_core_set_xo_dcc_adj(uint8 dcc_adjust);

/*===========================================================================

FUNCTION FUNCTION pmapp_xo_core_get_xo_dcc_adj                     EXTERNAL FUNCTION
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
// Below API will detect PMK and return data from appropriate pmic
pm_err_flag_type pmapp_xo_core_get_xo_dcc_adj(uint8* dcc_adjust);


/** @} */ /* end_addtogroup pm_xo_core */

#endif /* __PM_XO_CORE_H__ */
