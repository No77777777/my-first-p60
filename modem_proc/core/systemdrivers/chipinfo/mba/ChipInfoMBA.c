/*============================================================================

 FILE:      ChipInfoMBA.c

 DESCRIPTION:
 This file implements the Chip Info APIs for MBA

 PUBLIC CLASSES:
 ChipInfo_GetChipVersion
 ChipInfo_GetChipFamily

 ============================================================================
 Copyright (c) 2018 Qualcomm Technologies Incorporated.
 All Rights Reserved.
 QUALCOMM Proprietary/GTDR
 ============================================================================

 $Header: //components/rel/core.mpss/10.0/systemdrivers/chipinfo/mba/ChipInfoMBA.c#1 $
 $DateTime: 2019/04/24 00:03:26 $
 $Author: pwbldsvc $

==========================================================================*/

/*=========================================================================
 Include Files
 ==========================================================================*/

#include "ChipInfo.h"
#include "HALhwio.h"
#include "msmhwiobase.h"

/*========================================================================
 Macros
 ========================================================================*/

#define TCSR_TCSR_REGS_REG_BASE                                                           (CORE_TOP_CSR_BASE            + 0x000c0000)
#define HWIO_TCSR_SOC_HW_VERSION_ADDR                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x00008000)
#define HWIO_TCSR_SOC_HW_VERSION_IN          \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, HWIO_TCSR_SOC_HW_VERSION_RMSK)
#define HWIO_TCSR_SOC_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, m)
#define HWIO_TCSR_SOC_HW_VERSION_FAMILY_NUMBER_BMSK                                       0xf0000000
#define HWIO_TCSR_SOC_HW_VERSION_FAMILY_NUMBER_SHFT                                             0x1c
#define HWIO_TCSR_SOC_HW_VERSION_DEVICE_NUMBER_BMSK                                        0xfff0000
#define HWIO_TCSR_SOC_HW_VERSION_DEVICE_NUMBER_SHFT                                             0x10
#define HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_BMSK                                           0xff00
#define HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_SHFT                                              0x8
#define HWIO_TCSR_SOC_HW_VERSION_MINOR_VERSION_BMSK                                             0xff
#define HWIO_TCSR_SOC_HW_VERSION_MINOR_VERSION_SHFT                                              0x0


/*=========================================================================
 Functions
 ==========================================================================*/

/* ===========================================================================
 **  FUNCTION      ChipInfo_GetChipVersion
 **
 **  DESCRIPTION   See ChipInfo.h
 ** ======================================================================== */

ChipInfoVersionType ChipInfo_GetChipVersion(void)
{
  uint32 nMajorVersion  = HWIO_INF(TCSR_SOC_HW_VERSION, MAJOR_VERSION);
  uint32 nMinorVersion  = HWIO_INF(TCSR_SOC_HW_VERSION, MINOR_VERSION);

  return CHIPINFO_VERSION(nMajorVersion, nMinorVersion);
} /* END ChipInfo_GetChipVersion */


/* ===========================================================================
 **  FUNCTION      ChipInfo_GetChipFamily
 **
 **  DESCRIPTION   See ChipInfo.h
 ** ======================================================================== */

ChipInfoFamilyType ChipInfo_GetChipFamily(void)
{
  uint32 nChipFamilyNum = 0;
  uint32 nChipDeviceNum = 0;

  nChipFamilyNum = HWIO_INF(CHIPINFO_TCSR_SOC_HW_VERSION, FAMILY_NUMBER);
  nChipDeviceNum = HWIO_INF(CHIPINFO_TCSR_SOC_HW_VERSION, DEVICE_NUMBER);

  switch (nChipFamilyNum)
  {
    case 6:
    {
      switch (nChipDeviceNum)
      {
        case 0:
          return CHIPINFO_FAMILY_SDM845;
        case 2:
          return CHIPINFO_FAMILY_SDX24;
        case 3:
          return CHIPINFO_FAMILY_SM8150;
        case 4:
          return CHIPINFO_FAMILY_SDM670;
        case 5:
          return CHIPINFO_FAMILY_QCS605;
        case 6:
          return CHIPINFO_FAMILY_SCX8180;
        case 7:
          return CHIPINFO_FAMILY_SM6150;
        case 8:
          return CHIPINFO_FAMILY_SM8250;
        case 11:
          return CHIPINFO_FAMILY_SDX55;
        case 12:
          return CHIPINFO_FAMILY_SM7150;
        default:
          return CHIPINFO_FAMILY_UNKNOWN;
      }
    }
    case 9:
    {
      switch (nChipDeviceNum)
      {
        case 1:
          return CHIPINFO_FAMILY_NICOBAR;
        default:
          return CHIPINFO_FAMILY_UNKNOWN;
      }
    }
    default:
      return CHIPINFO_FAMILY_UNKNOWN;
  }
  return CHIPINFO_FAMILY_UNKNOWN;
} /* END Chipinfo_GetChipFamily */
