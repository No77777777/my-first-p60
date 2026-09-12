/*============================================================================
  FILE:         Product_Info.c

  OVERVIEW:     Platform detection for reference board design using ADC channels

  DEPENDENCIES: None

                Copyright (c) 2016 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/products/src/product_info.c#1 $


  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2016-03-09  ak  Initial revision for platform detection.

============================================================================*/
#include "err.h"

#ifdef FEATURE_MDM_V1_1
#include "DDIChipInfo.h"
#include "amssassert.h"

#define PRODUCT_MDM_VERSION_1_0 0x00010000
#define PRODUCT_INFO_ASSERT(condition) { if (!(condition)) break; }
#endif /* FEATURE_MDM_V1_1 */

uint32 g_products_smem_base_addr = SCL_SHARED_RAM_BASE;

// ------------------------------------------------------------------------------------------------
// function definitions
// ------------------------------------------------------------------------------------------------
void product_info_init (void)
{
#ifdef FEATURE_MDM_V1_1
  DalChipInfoVersionType chipVersion;
  DalChipInfoFamilyType chipFamily;
  DalDeviceHandle *hChipInfo;
  DALResult result;
  do
  {
    result = DAL_DeviceAttach (DALDEVICEID_CHIPINFO, &hChipInfo);
    PRODUCT_INFO_ASSERT (DAL_SUCCESS == result);
    result = DalDevice_Open (hChipInfo, DAL_OPEN_SHARED);
    PRODUCT_INFO_ASSERT (DAL_SUCCESS == result);
    result = DalChipInfo_GetChipFamily (hChipInfo, &chipFamily);
    PRODUCT_INFO_ASSERT ((DAL_SUCCESS == result) &&
                         (DALCHIPINFO_FAMILY_MDM9x55 == chipFamily));
    result = DalChipInfo_GetChipVersion(hChipInfo, &chipVersion);
    ASSERT ((NULL != chipVersion) && 
            (PRODUCT_MDM_VERSION_1_0 < chipVersion));
  } while (0);
  if (hChipInfo)
  {
    DalDevice_Close(hChipInfo);
  }
#endif /* FEATURE_MDM_V1_1 */
  if (g_products_smem_base_addr)
    return;
}


