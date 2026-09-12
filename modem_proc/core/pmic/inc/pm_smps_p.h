#ifndef __PM_SMPS_P_H__
#define __PM_SMPS_P_H__

/*! \file pm_smps_p.h
 *  \n
 *  \brief This header file contains enums and API definitions for SMPS power rail driver.
 *  \n
 *  \n &copy; Copyright 2017-2018 Qualcomm Technologies Incorporated, All Rights Reserved
 */
/* ======================================================================= */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/inc/pm_smps_p.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/18   rl      Initial File
=============================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/

#include "pm_smps.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/** @addtogroup pm_smps
@{ */

/** Quiet mode. */
typedef enum
{
    PM_QUIET_MODE_FOLLOW__DISABLE,      /**< Quiet mode disabled (default). */
    PM_QUIET_MODE_FOLLOW__QUIET,        /**< Quiet mode enabled. */
    PM_QUIET_MODE_FOLLOW__INVALID
}pm_quiet_mode_follow_type;



/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/** 
 * @name pm_smps_pin_ctrled_status 
 *  
 * @brief This function returns the pin ctrled status or h/w 
 *        enable status (On/Off) of the selected power rail.
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first SMPS peripheral)
 * @param[out] on_off:  
 *                Variable to return to the caller with pin ctrl
 *                status. Refer to pm_resources_and_types.h for
 *                the enum info.
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Under error conditions
 *          like invalid param entered.
 *          PM_ERR_FLAG__SUCCESS = SUCCESS.
 *
 */
pm_err_flag_type 
pm_smps_pin_ctrled_status ( uint8 pmic_chip,
                            uint8 perph_index,
                            pm_on_off_type* on_off );

/** 
 * @name pm_smps_sw_mode_status 
 *  
 * @brief This function returns the mode status (LPM, NPM, AUTO, BYPASS)
 *        of the selected power rail. Note, the mode of a regulator
 *        changes dynamically.
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first SMPS peripheral)
 * @param[out] sw_mode:  
 *                Variable to return to the caller with mode status.
 *                Refer to pm_resources_and_types.h for the enum info.
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Under error conditions
 *          like invalid param entered.
 *          PM_ERR_FLAG__SUCCESS = SUCCESS.
 *
 */
pm_err_flag_type 
pm_smps_sw_mode_status ( uint8 pmic_chip, 
                         uint8 perph_index, 
                         pm_sw_mode_type* sw_mode );

/** 
 * @name pm_smps_sw_enable_status 
 *  
 * @brief This function returns the s/w enable status (On/Off) 
 *        of the selected power rail.
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first SMPS peripheral)
 * @param[out] on_off:  
 *                Variable to return to the caller with s/w
 *                status. Refer to pm_resources_and_types.h for
 *                the enum info.
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Under error conditions
 *          like invalid param entered.
 *          PM_ERR_FLAG__SUCCESS = SUCCESS.
 *
 */
pm_err_flag_type
pm_smps_sw_enable_status ( uint8 pmic_chip,
                           uint8 perph_index,
                           pm_on_off_type* on_off );

/**
 * @name pm_smps_npm_status 
 *  
 * @brief Returns the NPM_VREG_OK status of the rail.
 * 
 * @param[in] pmic_chip Primary -- 0; Secondary -- 1.
 * @param[in] periph_index SMPS peripheral index. 
 *       Starts from 0 (for the first SMPS peripheral).
 * @param[out] npm_status true/false status.
 * 
 * @return 
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_smps_npm_status ( uint8 pmic_chip,
                     uint8 periph_index,
                     boolean* npm_status );

/**
  Configures and sets quiet mode for SMPS.
 
    @param[in] pmic_chip             Primary: 0, Secondary: 1.

    @param[in] periph_index SMPS peripheral index. Starts at 
                                     0 (for the first SMPS peripheral).
                                     See @xnameref{hdr:smpsPeripheralIndex}.

    @param[in] quiet_mode            Enable/disable Quiet mode and
                                     select the Quiet mode type. See
                                     #pm_quiet_mode_type.

	@return 
    Error flag type -- See #pm_err_flag_type.
 */
pm_err_flag_type
pm_smps_quiet_mode_mask ( uint8 pmic_chip,
                          uint8 periph_index,
                          pm_quiet_mode_follow_type quiet_mode );

/**
  Sets global quiet mode enable for SMPS.
 
    @param[in] pmic_chip             Primary: 0, Secondary: 1.

    @param[in] on_off                Enable/disable global Quiet
                                     mode enable.

	@return 
    Error flag type -- See #pm_err_flag_type.
 */
pm_err_flag_type
pm_smps_global_quiet_mode_enable ( uint8 pmic_chip,
                                   pm_on_off_type on_off);

/** @} */ /* end_addtogroup pm_smps */

#endif /* __PM_SMPS_P_H__ */
