/*
==============================================================================

FILE:         ClockTargetBSP.c

DESCRIPTION:
  This file contains the custom, target-specific clock driver BSP data.

==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdm845/ClockTargetBSP.c#1 $

==============================================================================
            Copyright (c) 2016 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================

*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockImage.h"


/*=========================================================================
      Data
==========================================================================*/

/*
 * Image-specific BSP data.
 */
ClockImageBSPType ClockImageBSP =
{
  .szCPUClockName = "clk_q6",
  .nInitCPUState  = 0, /* Keep value from MBA. */
};

