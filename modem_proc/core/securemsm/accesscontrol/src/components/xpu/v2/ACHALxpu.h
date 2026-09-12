#ifndef AC_HAL_XPU_H
#define AC_HAL_XPU_H

/*===========================================================================
Copyright (c) 2010-2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/securemsm/accesscontrol/src/components/xpu/v2/ACHALxpu.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

============================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "HALxpu2.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ACHAL_xpu_GetResourceGroupConfig(a,b,c) HAL_xpu2_GetResourceGroupConfig(a,b,c)
#define ACHAL_xpu_GetDeviceParameters(a,b) HAL_xpu2_GetDeviceParameters(a,b)
#define ACHAL_xpu_GetMSAResourceGroupConfigInfo(a,b,c)    HAL_xpu2_GetMSAResourceGroupConfigInfo(a,b,c)

#define ACHAL_xpu_XPUDeviceParamsType  HAL_xpu2_XPUDeviceParamsType
#define ACHAL_xpu_ResourceGroupConfigType  HAL_xpu2_ResourceGroupConfigType

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

#endif /* AC_HAL_XPU_H */
