#ifndef __PM_SMPS_H__
#define __PM_SMPS_H__

/*! \file pm_smps.h
 *  \n
 *  \brief This header file contains enums and API definitions for SMPS power rail driver.
 *  \n
 *  \n &copy; Copyright 2012-2015 Qualcomm Technologies Incorporated, All Rights Reserved
 */
/* ======================================================================= */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_smps.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/06/12   hw      Rearchitecturing module driver to peripheral driver
=============================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/

#include "pm_err_flags.h"
#include "pm_resources_and_types.h"
#include "com_dtypes.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/** @addtogroup pm_smps
@{ */

/** SMPS peripheral index. This enum type contains all required LDO
    regulators. */
enum
{
  PM_SMPS_1,   /**< SMPS 1. */
  PM_SMPS_2,   /**< SMPS 2. */
  PM_SMPS_3,   /**< SMPS 3. */
  PM_SMPS_4,   /**< SMPS 4. */
  PM_SMPS_5,   /**< SMPS 5. */    
  PM_SMPS_6,   /**< SMPS 6. */
  PM_SMPS_7,   /**< SMPS 7. */
  PM_SMPS_8,   /**< SMPS 8. */
  PM_SMPS_9,   /**< SMPS 9. */
  PM_SMPS_10,  /**< SMPS 10. */ 
  PM_SMPS_11,  /**< SMPS 11. */ 
  PM_SMPS_12,  /**< SMPS 12. */ 
  PM_SMPS_INVALID
};

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/** 
 * @name pm_smps_volt_level_status 
 *  
 * @brief This function returns the voltage level (in micro 
 *        volts) of the selected power rail.
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first SMPS peripheral)
 * @param[out] volt_level:  
 *                Variable to return to the caller with volt
 *                level status in micro volts (uint32).
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Under error conditions
 *          like invalid param entered.
 *          PM_ERR_FLAG__SUCCESS = SUCCESS.
 *
 */
pm_err_flag_type pm_smps_volt_level_status
(uint8 pmic_chip, uint8 perph_index, pm_volt_level_type* volt_level);

/**
 * @name pm_smps_vreg_ok_status 
 *  
 * @brief Returns the VREG_OK/VREG_READY status of the rail.
 * 
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in] periph_index SMPS peripheral index. 
 *       Starts from 0 (for the first SMPS peripheral).
 * @param[out] on_off On/Off status.
 * 
 * @return 
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_smps_vreg_ok_status
(uint8 pmic_chip, uint8 periph_index, boolean* on_off);

/** @} */ /* end_addtogroup pm_smps */

#endif /* __PM_SMPS_H__ */
