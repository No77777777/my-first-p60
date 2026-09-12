#ifndef __PMAPP_PWR_P_H__
#define __PMAPP_PWR_P_H__
/*===========================================================================


               P M A P P_ P W R_ P  H E A D E R    F I L E

\details
    This file contains functions prototypes and variable/type/constant 
  declarations to support conversions between voltage values and PMIC
  register values
  
Copyright (c) 2013            by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/inc/pmapp_pwr_p.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/07/13   rh      File created
===========================================================================*/

#include "pm_err_flags.h"
#include "pm_resources_and_types.h"


/*=========================================================================== */
/*                     pmapp_pwr_vdd_mss_set_npm                              */
/*=========================================================================== */
/**
  Changes the mode of MSS Rail from default to NPM and vice-versa

  PARAMETERS
  @param[in]  set_npm: TRUE sets NPM mode on MSS Rail
                       FALSE sets default mode on MSS Rail  
  @return      pm_err_flag_type - PM_ERR_FLAG__SUCCESS if successful, otherwise
                                  appropriate PMIC error flag is returned

*/
/*=========================================================================== */
static inline pm_err_flag_type pmapp_pwr_vdd_mss_set_npm(boolean set_npm)
{
  return PM_ERR_FLAG__FEATURE_NOT_SUPPORTED;
}

/** @} */ /* end_addtogroup pmapp_pwr */

#endif /* __PMAPP_PWR_P_H__ */
