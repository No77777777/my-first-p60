/*! \file pm_uicc.c 
*  \n 
*  \brief  File contains the implementation of the public APIs for UICC Application.
*  \n
*  \details Implementation file for UICC Application. Each of the APIs checks for
*           access and then if necessary directs the call to Application implementation.
*  \n
*  &copy; Copyright 2012-2013 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/application/uicc/src/pm_uicc.c#1 $ 

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/10/12   hs       Changed the type for internalResourceIndex from int to uint8.
02/24/12   dy      Add API to set UICC Hotswap Polarity

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_uicc_app.h"
#include "pm_pbs_client.h"
#include "pm_gpio.h"
#include "pm_ldo.h"
/*===========================================================================

                        FUNCTION DECLARTIONS

===========================================================================*/
/* Internal UICC helper functions */
static pm_err_flag_type pm_uicc_pwr_rails_reset_ctrl(pm_bua_alarm_type alarm);

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/

pm_err_flag_type pm_uicc_cntrl_enable(pm_bua_alarm_type alarm, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
	pm_uicc_app_data_type *uicc_ptr = pm_uicc_app_get_data();
    boolean status = FALSE;

	if ((uicc_ptr == NULL) || (uicc_ptr->periph_exists == FALSE))
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if (alarm >= PM_BUA_INVALID_ALARM)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED ;
    }

    /* This is mainly to check if the alarm is supported by BUA module */
    err_flag = pm_bua_alarm_detect_status(0, alarm, &status);

    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (enable == TRUE) 
    {
        /* Configure BUA interrupt polarity */
        err_flag |= pm_bua_irq_set_trigger(0, alarm, PM_IRQ_TRIGGER_ACTIVE_HIGH);

        /* Disable the BUA interrupt */
        err_flag |= pm_bua_irq_enable(0, alarm, FALSE);

        /* Clear the BUA interrupt */
        err_flag |= pm_bua_irq_clear(0, alarm);

        /* Bring UICCx pwr rail out of reset for a fresh start */
        err_flag |= pm_uicc_pwr_rails_reset_ctrl(alarm);

        /* Enable the BUA interrupt */
        err_flag |= pm_bua_irq_enable(0, alarm, TRUE);

        /* Disable BUA module */
        err_flag |= pm_bua_enable(0, FALSE);

        /* Enable BUA module */
        err_flag |= pm_bua_enable(0, TRUE);
    }
    else
    {
        /* Disable the BUA interrupt */
        err_flag |= pm_bua_irq_enable(0, alarm, FALSE);

        /* Clear the BUA interrupt */
        err_flag |= pm_bua_irq_clear(0, alarm);

        /* Bring UICCx pwr rail out of reset for a fresh start */
        err_flag |= pm_uicc_pwr_rails_reset_ctrl(alarm);
    }

    /* Let the user know if we were successful or not */
    return err_flag;
}

pm_err_flag_type pm_uicc_pwr_rails_reset_ctrl(pm_bua_alarm_type alarm)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
	pm_uicc_app_data_type *uicc_ptr = pm_uicc_app_get_data();
    uint8 uicc_ldo = 0;

	if ((uicc_ptr == NULL) || (uicc_ptr->periph_exists == FALSE))
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
	
    /* No power rail needed to be brought out of soft reset for BATT_ALARM */
    if (alarm == PM_BUA_BATT_ALARM)
    {
        return PM_ERR_FLAG_SUCCESS;
    }

    switch (alarm)
    {
    case PM_BUA_UICC1_ALARM:
        uicc_ldo = uicc_ptr->uicc_specific->uicc1_ldo;
        break;

    case PM_BUA_UICC2_ALARM:
        uicc_ldo = uicc_ptr->uicc_specific->uicc2_ldo;
        break;

    case PM_BUA_UICC3_ALARM:
        uicc_ldo = uicc_ptr->uicc_specific->uicc3_ldo;
        break;

    case PM_BUA_UICC4_ALARM:
        uicc_ldo = uicc_ptr->uicc_specific->uicc4_ldo;
        break;

    default:
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED; 

    }

    /* Some PMICs have GPIO controlled UIM power rails which need not be brought out
       of soft reset so returning SUCCESS here */
    if (uicc_ldo == PM_LDO_INVALID)
    {
        return PM_ERR_FLAG_SUCCESS; 
    }
    
    /* Get power rail out of SoftReset state that PBS RAM sequence would have placed */
    //debug : vk needed ?
    //err_flag = pm_ldo_soft_reset_exit(0, uicc_ldo);

    return err_flag;
}
