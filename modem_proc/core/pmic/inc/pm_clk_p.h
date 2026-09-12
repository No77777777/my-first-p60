#ifndef __PM_CLK_P_H__
#define __PM_CLK_P_H__

/** @file pm_clk_p.h 
*
*  This header file contains functions and variable declarations 
*  to support Qualcomm PMIC Clock module. 
*/
/*
*   Copyright (c) 2017-2018 Qualcomm Technologies, Inc.
*   All Rights Reserved.
*   Confidential and Proprietary - Qualcomm Technologies, Inc.
*/

/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/inc/pm_clk_p.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/18   rl      Initial version. 
========================================================================== */
#include "pm_clk.h"

/*===========================================================================

Function Definitions 

===========================================================================*/

/** @addtogroup pm_clk
@{ */
/**
  Sets the output buffer drive strength.

 @param[in] pmic_chip   Selects the device in which the buffers being
                        controlled are located. Device index starts with
                        zero.
  @param[in] periph Clock type. See #pm_clk_type
 @param[in] drive_strength Output buffer drive strength setting. See
                          #pm_clk_drv_strength_type.

 @return
  SUCCESS or Error -- See #pm_err_flag_type.

 @dependencies
  None

 @sideeffects
 Interrupts are disabled while communicating with the PMIC.
*/
/*===========================================================================*/

pm_err_flag_type 
pm_clk_buff_set_output_drive_strength(uint8 pmic_chip, 
                                      pm_clk_type periph, 
                                      pm_clk_drv_strength_type drive_strength );

/*===========================================================================

FUNCTION pm_clk_buff_get_output_drive_strength                EXTERNAL FUNCTION

===========================================================================*/
/**

Gets the output buffer drive strength. 
 @param[in] pmic_chip   Selects the device in which the buffers being
                        controlled are located. Device index starts with
                        zero.
 @param[in] periph Clock type. See #pm_clk_type
 
 @param[out] drive_strength   uint8 Pointer to get Output buffer drive strength setting. To be mapped with #pm_clk_drv_strength_type.
 @return 
 SUCCESS or Error -- See #pm_err_flag_type
 
*/
/*===========================================================================*/

pm_err_flag_type pm_clk_buff_get_output_drive_strength(uint8 pmic_chip, 
                                                       pm_clk_type periph, 
                                                       uint8 *drive_strength);

/**
  Enable/disable holding of clockbuffer feature.

 @param[in] pmic_chip   Selects the device in which the buffers being
                        controlled are located. Device index starts with
                        zero.
  @param[in] periph Clock type. See #pm_clk_type
 @param[in] on_off  See #pm_on_off_type 

 @return
  SUCCESS or Error -- See #pm_err_flag_type.

 @dependencies
  None

 @sideeffects
 Interrupts are disabled while communicating with the PMIC.
*/
pm_err_flag_type
pm_clk_hold_enable(uint32 pmic_chip,
                 pm_clk_type periph,
                 pm_on_off_type on_off);

/** @} */ /* end_addtogroup pm_clk */

#endif /* __PM_CLK_P_H__ */
