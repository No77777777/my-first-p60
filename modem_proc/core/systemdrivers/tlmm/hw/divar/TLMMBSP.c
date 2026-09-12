/*
==============================================================================

FILE:         TLMMBSP.c

DESCRIPTION:
  This file contains the TLMM driver hardware BSP data.


==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/hw/divar/TLMMBSP.c#1 $

==============================================================================
            Copyright (c) 2017 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================

*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "comdef.h"
#include "msmhwiobase.h"
#include "GPIOTypes.h"
#include "HALtlmm.h"
#include "ChipInfoDefs.h"

/*=========================================================================
      Externs
==========================================================================*/

extern HAL_tlmm_GpioCoreControlType HAL_gpio_CoreControl;
extern HAL_tlmm_GpioExtControlType HAL_gpio_ExtControl;

/*=========================================================================
      Data Declarations
==========================================================================*/

HALgpioBlockDescType GPIOBlockDescriptors[] =
{
  {
    GPIO_DEVICE_TLMM,
    TLMM_BASE,
    0x1000,
    3,
    {
      0x00100000,
      0x00500000,
      0x00900000,
      0,0
    },
    0,
    112,
    CHIPINFO_FAMILY_DIVAR,
    GPIO_HW_VERSION(1, 0, 0xFF, 0xFF),
    &HAL_gpio_CoreControl,
    &HAL_gpio_ExtControl
  }
};

uint32 gnNumBlockDescs = ARR_SIZE(GPIOBlockDescriptors);;

