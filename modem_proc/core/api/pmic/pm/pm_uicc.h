#ifndef __PM_UICC_H__
#define __PM_UICC_H__

/*! \file pm_uicc.h
 *  \n
 *  \brief This file contains functions prototypes and variable/type/constant
 *  declarations to support the HOTSWAP (UICC) feature inside the Qualcomm
 *  PMIC chips
 *  \n
 *  \n &copy; Copyright 2011-2013 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_uicc.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/24/11   dy      Add API to set hotswap polarity
09/14/11   dy      Created.
===========================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/
#include "pm_err_flags.h"
#include "com_dtypes.h"
/*===========================================================================

                        DEFINITIONS 

===========================================================================*/
typedef enum
{
    PM_BUA_BATT_ALARM
}pm_bua_alarm_type;
/*===========================================================================

                        API PROTOTYPE

===========================================================================*/
/** @addtogroup pm_uicc
@{ */
/** 
 * @name pm_uicc_cntrl_enable 
 * API NOT SUPPORTED
 */
static inline pm_err_flag_type pm_uicc_cntrl_enable(pm_bua_alarm_type alarm, boolean enable)
{
  return PM_ERR_FLAG__FEATURE_NOT_SUPPORTED;
}

/** @} */ /* end_addtogroup pm_uicc */

#endif /* __PM_UICC_H__ */
