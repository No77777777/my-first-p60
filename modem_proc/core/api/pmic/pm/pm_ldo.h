#ifndef __PM_LDO_H__
#define __PM_LDO_H__

/** @file pm_ldo.h 
 *  
 *  This header file contains enums and API definitions for LDO
 *  power rail driver.
*/
/*
 *  Copyright (c) 2012-2015 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_ldo.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/06/12   hw      Rearchitecturing module driver to peripheral driver
=============================================================================*/

/*===========================================================================

                 HEADER FILE INCLUDE

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"
#include "com_dtypes.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

/** @addtogroup pm_ldo
@{ */

/**
 * LDO peripheral index. This enum type contains all LDO regulators that are
 * required. 
 */
enum
{
  PM_LDO_1,       /**< LDO 1. */
  PM_LDO_2,       /**< LDO 2. */
  PM_LDO_3,       /**< LDO 3. */
  PM_LDO_4,       /**< LDO 4. */
  PM_LDO_5,       /**< LDO 5. */
  PM_LDO_6,       /**< LDO 6. */
  PM_LDO_7,       /**< LDO 7. */
  PM_LDO_8,       /**< LDO 8. */
  PM_LDO_9,       /**< LDO 9. */
  PM_LDO_10,      /**< LDO 10. */
  PM_LDO_11,      /**< LDO 11. */
  PM_LDO_12,      /**< LDO 12. */
  PM_LDO_13,      /**< LDO 13. */
  PM_LDO_14,      /**< LDO 14. */
  PM_LDO_15,      /**< LDO 15. */
  PM_LDO_16,      /**< LDO 16. */
  PM_LDO_17,      /**< LDO 17. */
  PM_LDO_18,      /**< LDO 18. */
  PM_LDO_19,      /**< LDO 19. */
  PM_LDO_20,      /**< LDO 20. */
  PM_LDO_21,      /**< LDO 21. */
  PM_LDO_22,      /**< LDO 22. */
  PM_LDO_23,      /**< LDO 23. */
  PM_LDO_24,      /**< LDO 24. */
  PM_LDO_25,      /**< LDO 25. */
  PM_LDO_26,      /**< LDO 26. */
  PM_LDO_27,      /**< LDO 27. */
  PM_LDO_28,      /**< LDO 28. */
  PM_LDO_29,      /**< LDO 29. */
  PM_LDO_30,      /**< LDO 30. */
  PM_LDO_31,      /**< LDO 31. */
  PM_LDO_32,      /**< LDO 32. */
  PM_LDO_33,      /**< LDO 33. */
  PM_LDO_INVALID  /**< Invalid LDO. */
};

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/** 
 *  Returns the voltage level (in microvolts) of the selected power rail.
 *  
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in] ldo_peripheral_index LDO peripheral index.
 *                Starts from 0 (for the first LDO peripheral).
 * @param[out] volt_level Variable returned to the caller with the volt
 *                level status in micro volts (uint32).
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_ldo_volt_level_status (uint8 pmic_chip, 
                          uint8 ldo_peripheral_index, 
                          pm_volt_level_type* volt_level);

/**
 * @name pm_ldo_vreg_ok_status 
 *  
 * @brief Returns the VREG_OK/VREG_READY status of the rail.
 * 
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in] ldo_peripheral_index LDO peripheral index. Starts 
 *       from 0 (for the first LDO peripheral).
 * @param[out] on_off On/Off status.
 * 
 * @return 
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type
pm_ldo_vreg_ok_status (uint8 pmic_chip, 
                       uint8 ldo_peripheral_index, 
                       boolean* on_off);

/** @} */ /* end_addtogroup pm_ldo */

#endif /* PM_LDO__H */
