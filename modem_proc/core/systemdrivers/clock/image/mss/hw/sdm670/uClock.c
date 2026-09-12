/*
===========================================================================
*/
/**
  @file uClockRoot.c
  @brief Implementation of the micro lightweight clock driver

*/
/*===========================================================================
NOTE: The @brief description and any detailed descriptions above do not appear
      in the PDF.

      This file is source file for the MPSS minidump mode. It is meant to be
      used by a small subset of drivers, which cannot call into the DAL clock
      driver APIs.

===========================================================================*/
/*
  ===========================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdm670/uClock.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  11/15/17   shm     Ported to MPSS from LPASS for minidump mode support

  ===========================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "com_dtypes.h"
#include "uClock.h"
#include "uClockHWIO.h"
#include "HALhwio.h"
#include "busywait.h"
#include "ss_minidump.h"


/*=========================================================================
      Type Definitions
==========================================================================*/


/*=========================================================================
      Local Variables
==========================================================================*/


/*=========================================================================
      Function Prototypes.
==========================================================================*/


/*=============================================================================
 ==============================================================================

  A P P L I C A T I O N    P R O G R A M M E R   I N T E R F A C E

 ==============================================================================
 ============================================================================*/

/* ============================================================================
**  Function : uClock_IsClockOn
** ============================================================================
*/
/**
  Internal API which returns whether a clock is on or not.

  @param[in]  eClockId  - The ID of the clock to enable.

  @return
  TRUE  - The clock is on.
  FALSE - Otherwise.

  @dependencies
  None.

*/
boolean uClock_IsClockOn(uClockIdType eClockId)
{
  uint32 nVal = 0;

  if (!md_ss_check_if_in_minidump_mode())
  {
    return(FALSE);
  }

  switch (eClockId)
  {
    case CLOCK_MSS_BUS_CRYPTO_CLK:
      nVal = HWIO_INF(MSS_BUS_CRYPTO_CBCR, CLKOFF);
      break;
    case CLOCK_MSS_AXI_CRYPTO_CLK:
      nVal = HWIO_INF(MSS_AXI_CRYPTO_CBCR, CLKOFF);
      break;
    case CLOCK_GCC_PRNG_AHB_CLK:
      nVal = HWIO_INF(GCC_PRNG_AHB_CBCR, CLK_OFF);
      break;
    default:
      return(FALSE);
  }

  if(nVal != 0)
  {
    return(FALSE);
  }
  else
  {
    return(TRUE);
  }

} /* uClock_IsClockOn */


/* ============================================================================
**  Function : uClock_EnableClock
** ============================================================================
*/
/**
  Enables a clock.

  @param[in]  eClockId  - The ID of the clock to enable.

  @return
  TRUE  - The clock was enabled.
  FALSE - Otherwise.

  @dependencies
  None.

*/

boolean uClock_EnableClock(uClockIdType eClockId)
{
  uint32 nTimeout = 150;

  if (!md_ss_check_if_in_minidump_mode())
  {
    return(FALSE);
  }
  
  switch (eClockId)
  {
    case CLOCK_MSS_BUS_CRYPTO_CLK:
    {
      HWIO_OUTF(MSS_BUS_CRYPTO_CBCR, CLKEN, 0x1);
      break;
    }
    case CLOCK_MSS_AXI_CRYPTO_CLK:
    {
      HWIO_OUTF(MSS_AXI_CRYPTO_CBCR, CLKEN, 0x1);
      break;
    }
    case CLOCK_GCC_PRNG_AHB_CLK:
    {
      HWIO_OUTF(GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE, PRNG_AHB_CLK_ENA, 0x1);
      break;
    }
    default:
      return(FALSE);
  }

  /*
   * Make sure the clock is on.
   */
   while (uClock_IsClockOn(eClockId) != TRUE)
   {
     if (nTimeout == 0)
     {
       break;
     }
     busywait(2);
     nTimeout--;
   }
   
  if (nTimeout != 0)
  {
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }

} /* uClock_EnableClock */


/* ============================================================================
**  Function : uClock_DisableClock
** ============================================================================
*/
/**
  Disables a clock.

  @param[in]  eClockId  - The ID of the clock to disable.

  @return
  TRUE  - The clock was disabled.
  FALSE - Otherwise.

  @dependencies
  None.

*/

boolean uClock_DisableClock(uClockIdType eClockId)
{
  if (!md_ss_check_if_in_minidump_mode())
  {
    return(FALSE);
  }

  switch (eClockId)
  {
    case CLOCK_MSS_BUS_CRYPTO_CLK:
    {
      HWIO_OUTF(MSS_BUS_CRYPTO_CBCR, CLKEN, 0x0);
      break;
    }
    case CLOCK_MSS_AXI_CRYPTO_CLK:
    {
      HWIO_OUTF(MSS_AXI_CRYPTO_CBCR, CLKEN, 0x0);
      break;
    }
    case CLOCK_GCC_PRNG_AHB_CLK:
    {
      HWIO_OUTF(GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE, PRNG_AHB_CLK_ENA, 0x0);
      break;
    }
    default:
      return(FALSE);
  }
  return(TRUE);

} /* uClock_DisableClock */
