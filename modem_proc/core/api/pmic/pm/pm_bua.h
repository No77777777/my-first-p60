#ifndef __PM_BUA_H__
#define __PM_BUA_H__

/** @file pm_bua.h
*
*  This header file contains API and type definitions for BUA driver.
 */
/*
*   Copyright (c) 2013 Qualcomm Technologies, Inc.
*   All Rights Reserved.
*   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
 
/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_bua.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/16/13   kt      Created.
=============================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/
#include "pm_err_flags.h"
#include "com_dtypes.h"
#include "pm_irq.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/** @addtogroup pm_bua
@{ */

/** 
  Types of BUA alarms supported across targets. 
 */
typedef enum 
{
  PM_BUA_BATT_ALARM,  /**< Battery alarm. */
  PM_BUA_UICC1_ALARM, /**< UICC 1 alarm. */
  PM_BUA_UICC2_ALARM, /**< UICC 2 alarm. */ 
  PM_BUA_UICC3_ALARM, /**< UICC 3 alarm. */
  PM_BUA_UICC4_ALARM, /**< UICC 4 alarm. */
  PM_BUA_INVALID_ALARM
}pm_bua_alarm_type;

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/
/* pm_bua_enable */
/**
 *  Enables or disables the BUA.
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; secondary PMIC -- 1.
 * @param[in] enable TRUE -- Enable the BUA. \n
 *                   FALSE -- Disable the BUA.
 * 
 * @return
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_enable(uint8 pmic_chip, boolean enable);

/* pm_bua_enable_status */
/**
 *  Returns the BUA enable/disable status (BUA_OK status).
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.  
 * @param[out] status TRUE -- BUA is enabled. \n
 *                    FALSE -- BUA is disabled.
 * 
 * @return
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_enable_status(uint8 pmic_chip, boolean *status);

/* pm_bua_alarm_detect_status */

/**
 *  Returns the BUA alarm detected status, e.g., 
 *        BATT_GONE status for BATT_ALARM type and
 *        UICCx_ALARM_DETECTED status for UICCx_ALARM type.
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] alarm BUA alarm type. See #pm_bua_alarm_type.
 *  
 * @param[out] status TRUE -- BUA alarm was detected. \n
 *                    FALSE -- BUA alarm was not detected.
 *
 * @return
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_alarm_detect_status(uint8 pmic_chip, pm_bua_alarm_type alarm, boolean *status);

/* pm_bua_irq_enable */

/**
 *  Enables or disables the BUA alarm IRQ.
 *  
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] alarm BUA alarm type. See #pm_bua_alarm_type.
 * @param[in] enable TRUE -- Enable the BUA alarm interrupt. \n
 *                           FALSE -- Disable BUA alarm interrupt.
 *
 * @return
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_irq_enable(uint8 pmic_chip, pm_bua_alarm_type alarm, boolean enable);

/* pm_bua_irq_clear */

/**
 *  Clears the BUA alarm IRQ.
 *  
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] alarm BUA alarm type. See #pm_bua_alarm_type.
 *
 * @return
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_irq_clear(uint8 pmic_chip, pm_bua_alarm_type alarm);

/* pm_bua_irq_set_trigger */

/**
 * Configures the BUA alarm IRQ trigger type.
 *  
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] alarm BUA alarm type. See #pm_bua_alarm_type.  
 * @param[in] trigger IRQ trigger type. See #pm_irq_trigger_type.
 *
 * @return 
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_irq_set_trigger(uint8 pmic_chip, pm_bua_alarm_type alarm, pm_irq_trigger_type trigger);

/* pm_bua_irq_get_trigger */

/**
 *  Returns the BUA alarm IRQ trigger type. 
 *  
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] alarm BUA alarm type. See #pm_bua_alarm_type.
 * @param[out] trigger IRQ trigger type set for the BUA alarm IRQ.
 *                     See #pm_irq_trigger_type.
 *
 * @return
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_irq_get_trigger(uint8 pmic_chip, pm_bua_alarm_type alarm, pm_irq_trigger_type *trigger);

/* pm_bua_irq_status */

/**
 *  Returns the BUA alarm IRQ status.
 *  
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] alarm BUA alarm type. See #pm_bua_alarm_type.
 * @param[in] type Type of IRQ status to read. See #pm_irq_status_type.
 * @param[out] status IRQ status.
 *
 * @return
 *   SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_bua_irq_status(uint8 pmic_chip, pm_bua_alarm_type alarm, pm_irq_status_type type, boolean *status);

/** @} */ /* end_addtogroup pm_bua */

#endif /* __PM_BUA_H__ */
