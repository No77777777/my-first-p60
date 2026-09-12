/*
===========================================================================
*/
/**
  @file uClockQ6CoreMd.c
  @brief Bumps up the Q6 frequency to minimum perf level

*/
/*===========================================================================
NOTE: The @brief description and any detailed descriptions above do not appear
      in the PDF.

      This file is source file for bumping up the Q6 to minimum supported perf
      level above 19.2 MHz for the MPSS minidump mode.

===========================================================================*/
/*
  ===========================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sm7150/uClockQ6CoreMd.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  07/19/18   shm     First version for SM7150

  ===========================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "com_dtypes.h"
#include "uClockHWIO.h"
#include "HALhwio.h"
#include "busywait.h"
#include "ss_minidump_timer.h"


/*=========================================================================
      Defines
==========================================================================*/

/*
 * TODO: com_dtypes.h needs to be updated to include this.
 */
#ifndef _UINTNT_DEFINED
typedef unsigned long uintnt;
#endif

/*
 * RCGR update timeout. In practice this should be very short (less than 1us),
 * but it depends on the clock frequency being fed into the RCG. Choosing
 * a very conservative value.
 */
#define CLOCK_MD_UPDATE_TIMEOUT_US 1000

/*
 * MPLL3 min frequency is 15 * 19.2 MHz = 288.00 MHz
 * MPLL3 max frequency is 62 * 19.2 MHz = 1190.40 MHz
 * MPLL3 calibration frequency is (15+62)/2 = 38 * 19.2 MHz = 729.6MHz
 */
#define PLL3_CALIB_L_VAL 38

/*
 * MPLL0 max frequency at MinSVS is 15 * 19.2 MHz = 288.00 MHz
 */
#define PLL3_DEFAULT_L_VAL 15

/*
 * PLL lock timeout (2x max locking time).
 */
#define CLOCK_MD_PLL_LOCK_TIMEOUT_US 200

/*
 * Warm-up time in microseconds after turning on the PLL.
 */
#define CLOCK_MD_PLL_WARMUP_TIME_US 25

/*
 * PLL register settings from HW team.
 */
#define HAL_CLK_UPDATED_CONFIG_CTL_VAL    0x20485699
#define HAL_CLK_UPDATED_CONFIG_CTL_U_VAL  0x00002067
#define HAL_CLK_UPDATED_TEST_CTL_VAL      0x40000000
#define HAL_CLK_UPDATED_TEST_CTL_U_VAL    0x0
#define HAL_CLK_UPDATED_USER_CTL_U_VAL    0x4804


/*=========================================================================
      External and forward references
==========================================================================*/

boolean WaitForClear(uintnt nAddr, uint32 nMask, uint32 nWait);
boolean WaitForSet  (uintnt nAddr, uint32 nMask, uint32 nWait);


/*=========================================================================
      Functions
==========================================================================*/

boolean ClockMD_EnablePLLDynProg
(
  void
)
{
  /*
   * 1. Switch the Q6 temporarily to XO
   */
  HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_SEL, 0);
  HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_DIV, 1);
  HWIO_OUTF(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE, 1);
  if (!WaitForClear(HWIO_ADDR(MSS_QDSP6SS_CORE_CMD_RCGR),
                    HWIO_FMSK(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE),
                    CLOCK_MD_UPDATE_TIMEOUT_US))
  {
    return FALSE;
  }

  /*
   * 2. Shut down the PLL
   */
  HWIO_OUTF(MSS_QDSP6SS_PLL_MODE, PLL_OUTCTRL, 0);
  md_ss_modem_wait_microseconds(1);
  HWIO_OUTF(MSS_QDSP6SS_PLL_MODE, PLL_RESET_N, 0);
  HWIO_OUT(MSS_QDSP6SS_PLL_OPMODE, 0);

  /*
   * 2.5 Update the Q6 PLL register settings. Other PLLs' settings
   *     are not needed as this is in mini-dump mode
   */
  HWIO_OUT(MSS_QDSP6SS_PLL_CONFIG_CTL,    HAL_CLK_UPDATED_CONFIG_CTL_VAL);
  HWIO_OUT(MSS_QDSP6SS_PLL_CONFIG_CTL_U,  HAL_CLK_UPDATED_CONFIG_CTL_U_VAL);
  HWIO_OUT(MSS_QDSP6SS_PLL_TEST_CTL,      HAL_CLK_UPDATED_TEST_CTL_VAL);
  HWIO_OUT(MSS_QDSP6SS_PLL_TEST_CTL_U,    HAL_CLK_UPDATED_TEST_CTL_U_VAL);

  /* 
   * 3. Write the calibration CAL_L and L values
   */
  HWIO_OUT(MSS_QDSP6SS_PLL_CAL_L_VAL, PLL3_CALIB_L_VAL);
  HWIO_OUT(MSS_QDSP6SS_PLL_L_VAL, PLL3_DEFAULT_L_VAL);
  HWIO_OUT(MSS_QDSP6SS_PLL_FRAC_VAL, 0);
  HWIO_OUTF(MSS_QDSP6SS_PLL_USER_CTL, PRE_DIV_RATIO, 0x0);
  HWIO_OUTF(MSS_QDSP6SS_PLL_USER_CTL, POST_DIV_RATIO_ODD, 0x0);
  HWIO_OUTF(MSS_QDSP6SS_PLL_USER_CTL, POST_DIV_RATIO_EVEN, 0x0);

  /*
   * 4. Put the PLL in the run mode
   */
  HWIO_OUTF(MSS_QDSP6SS_PLL_MODE, PLL_RESET_N, 1);
  HWIO_OUT(MSS_QDSP6SS_PLL_OPMODE, 1);
  md_ss_modem_wait_microseconds(CLOCK_MD_PLL_WARMUP_TIME_US);
  /* Wait for the PLL to lock */
  if (!WaitForSet(HWIO_ADDR(MSS_QDSP6SS_PLL_MODE),
                  HWIO_FMSK(MSS_QDSP6SS_PLL_MODE, PLL_LOCK_DET_FINE),
                  CLOCK_MD_PLL_LOCK_TIMEOUT_US))
  {
    return FALSE;
  }

  /*
   * Enable the PLLOUT_MAIN, EVEN and OUTCTRL
   */
  HWIO_OUTF(MSS_QDSP6SS_PLL_USER_CTL, PLLOUT_MAIN, 1);
  HWIO_OUTF(MSS_QDSP6SS_PLL_USER_CTL, PLLOUT_EVEN, 1);
  HWIO_OUTF(MSS_QDSP6SS_PLL_MODE, PLL_OUTCTRL, 1);

  /*
   * 5. Switch the Q6 back from XO to the PLL source
   */
  /* Select PLL3 source */
  HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_SEL, 1);
  /* Set the Q6 divider */
  HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_DIV, 1);
  /* Initiate MSS_QDSP6SS_CORE_CMD_RCGR update */
  HWIO_OUTF(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE, 1);
  /* Wait with timeout for register update */
  if (!WaitForClear(HWIO_ADDR(MSS_QDSP6SS_CORE_CMD_RCGR),
                    HWIO_FMSK(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE),
                    CLOCK_MD_UPDATE_TIMEOUT_US))
  {
    return FALSE;
  }

  return TRUE;

} /* END ClockMBA_EnablePLLDynProg */


/* =========================================================================
**  Function : WaitForClear
** =========================================================================*/
/**
  Checks for a clock update.

  @param nAddr [in] -- Address of the register to check for the update.
  @param nMask [in] -- Mask for the update bit to check.
  @param nWait [in] -- Wait delay counter.
*/

boolean WaitForClear(uintnt nAddr, uint32 nMask, uint32 nWait)
{
  uint32 nTimeout = 0;

  /*
   * Wait until bit is cleared
   */
  while (inpdw(nAddr) & nMask)
  {
    if (nTimeout++ >= nWait)
    {
      return FALSE;
    }
    md_ss_modem_wait_microseconds(1);
  }

  return TRUE;
} /* END WaitForClear */


/* =========================================================================
**  Function : WaitForSet
** =========================================================================*/
/**
  Checks for a clock update.

  @param nAddr [in] -- Address of the register to check for the update.
  @param nMask [in] -- Mask for the update bit to check.
  @param nWait [in] -- Wait delay counter.
*/

boolean WaitForSet(uintnt nAddr, uint32 nMask, uint32 nWait)
{
  uint32 nTimeout = 0;

  /*
   * Wait until bit is set.
   */
  while ((inpdw(nAddr) & nMask) == 0)
  {
    if (nTimeout++ >= nWait)
    {
      return FALSE;
    }
    md_ss_modem_wait_microseconds(1);
  }

  return TRUE;
} /* END WaitForSet */
