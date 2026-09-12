/*
==============================================================================

FILE:         TLMMBSP.c

DESCRIPTION:
  This file contains the TLMM driver hardware BSP data.


==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/hw/stub/TLMMBSP.c#1 $

==============================================================================
            Copyright (c) 2017 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================

*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "GPIOTypes.h"
#include "HALgpio.h"

/*=========================================================================
      Externs
==========================================================================*/


/*=========================================================================
      Data Declarations
==========================================================================*/

HALgpioBlockDescType GPIOBlockDescriptors[] =
{
  {
    GPIO_DEVICE_TLMM,
    0x0,
    0x1000,
    2,
    {
      0x00500000,
      0x00900000,
      0,0,0
    },
    0,
    149,
    GPIO_HW_VERSION(1, 0, 0xFF, 0xFF)
  },
  {
    GPIO_DEVICE_SSC_LPI,
    0x0,
    0x1000,
    1,
    {
      0x00B40000,
      0,0,0,0
    },
    0,
    17,
    GPIO_HW_VERSION(1, 0, 0xFF, 0xFF),
  },  
};

uint32 gnNumBlockDescs = 2;

