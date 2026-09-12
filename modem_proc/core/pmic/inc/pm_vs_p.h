#ifndef __PM_VS_P_H__
#define __PM_VS_P_H__

/** @file pm_vs_p.h 
 *
 *  This header file contains enums and API definitions for the
 *  voltage switch power rail driver.
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

  $Header: //components/rel/core.mpss/10.0/pmic/inc/pm_vs_p.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/18   rl      Initial Creation
========================================================================== */
/*===========================================================================

                        HEADER FILES

===========================================================================*/
#include "pm_vs.h"

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/

/**
 *  Returns the software enable status (On/Off) 
 *        of the selected power rail.
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] perph_index VS peripheral index.
 *                           Starts from 0 (for the first VS peripheral).
 * @param[out] on_off Variable returned to the caller with the software
 *                status. Refer to pm_resources_and_types.h for
 *                the enum info.
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type
pm_vs_sw_enable_status ( uint8 pmic_chip,
                         uint8 perph_index,
                         pm_on_off_type* on_off );

#endif /* __PM_VS_P_H__ */
