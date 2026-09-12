/*
==============================================================================

FILE:         HALclkRimacPLL.c

DESCRIPTION:
  This file contains the clock HAL code for the Rimac PLL control.

==============================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/systemdrivers/clock/hal/hw/v1/HALclkRimacPLL.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
10/27/14   vt     Created

==============================================================================
            Copyright (c) 2014 Qualcomm Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/

/*============================================================================

                     INCLUDE FILES FOR MODULE

============================================================================*/

#include "HALhwio.h"
#include "HALclkRimacPLL.h"
#include "HALclkGenericPLL.h"
#include "DDIChipInfo.h"


/*============================================================================

             DEFINITIONS AND DECLARATIONS FOR MODULE

=============================================================================*/


/* ============================================================================
**    Definitions
** ==========================================================================*/


/*
 * HAL_CLK_PLL_MODE_*
 *
 * Definitions for configuring the PLLn_MODE register.
 */
#define HAL_CLK_PLL_MODE_ACTIVE  \
  (HWIO_FMSK(PLL_MODE, PLL_OUTCTRL)  | \
   HWIO_FMSK(PLL_MODE, PLL_BYPASSNL) | \
   HWIO_FMSK(PLL_MODE, PLL_RESET_N))

/*
 * HAL_CLK_PLL_INIT_TIME_US
 *
 * Init time in microseconds after de-asserting PLL reset (normal mode).
 */
#define HAL_CLK_PLL_INIT_TIME_US                  5

/*
 * HAL_CLK_LOCK_TIMEOUT_US
 *
 * Timeout in US for PLL_LOCK_DET to get set.
 */
#define HAL_CLK_LOCK_TIMEOUT_US                 200



/* ============================================================================
**    Data Declarations
** ==========================================================================*/

/*
 * HAL_clk_RimacPLLControl
 *
 * Control function array for the Rimac PLLs.
 */
HAL_clk_SourceControlType HAL_clk_RimacPLLControl =
{
  .Enable           = HAL_clk_RimacPLLEnable,
  .Disable          = HAL_clk_RimacPLLDisable,
  .IsEnabled        = HAL_clk_RimacPLLIsEnabled,
  .Config           = HAL_clk_RimacPLLConfig,
  .ConfigPLL        = HAL_clk_RimacPLLConfigPLL,
  .DetectPLLConfig  = HAL_clk_RimacPLLDetectPLLConfig,
  .EnableVote       = NULL,
  .DisableVote      = NULL,
  .IsVoteEnabled    = NULL,
  .IsPLLConfigEqual = HAL_clk_RimacPLLIsPLLConfigEqual,
  .IsCalibrated     = HAL_clk_RimacPLLIsCalibrated,
  .SetCalConfig     = HAL_clk_RimacPLLSetCalConfig,
  .SetRegSettings   = HAL_clk_RimacPLLSetRegSettings,
};


/*============================================================================

               FUNCTION DEFINITIONS FOR MODULE

============================================================================*/

/* ===========================================================================
**  HAL_clk_RimacPLLEnable
**
**  Returns:
**    boolean - TRUE if successfully enabled. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_RimacPLLEnable
(
  HAL_clk_SourceDescType        *pSource,
  HAL_clk_SourceDisableModeType  eMode,
  void                          *pData
)
{
  uint32                  nModeVal;
  boolean                 bResult;

  /*
   * Check if PLL is already enabled and return
   */
  if (HAL_clk_RimacPLLIsEnabled(pSource))
  {
    return TRUE;
  }

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * De-assert PLL bypass.
   * This latches L, Alpha, and AlphaU values.
   */
  nModeVal = HAL_CLK_PLL_HWIO_OUTFV(PLL_MODE, pSource, PLL_BYPASSNL, 1, nModeVal);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for PLL init time
   */
  HAL_clk_BusyWait(HAL_CLK_PLL_INIT_TIME_US);

  /*
   * De-assert PLL reset.
   */
  nModeVal = HAL_CLK_PLL_HWIO_OUTFV(PLL_MODE, pSource, PLL_RESET_N, 1, nModeVal);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for the PLL to lock during the 50 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegSetTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_LOCK_DET),
      HAL_CLK_LOCK_TIMEOUT_US);

  /*
   * Return immediately if the lock failed.
   */
  if (!bResult)
  {
    return FALSE;
  }

  /*
   * Put the PLL in active mode
   */
  nModeVal = HAL_CLK_PLL_HWIO_OUTFV(PLL_MODE, pSource, PLL_OUTCTRL, 1, nModeVal);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  return TRUE;

} /* END HAL_clk_RimacPLLEnable */


/* ===========================================================================
**  HAL_clk_RimacPLLDisable
**
**  PLL shutdown.
**
** ======================================================================== */

static void HAL_clk_RimacPLLDisable
(
  HAL_clk_SourceDescType        *pSource,
  HAL_clk_SourceDisableModeType  eMode,
  void                          *pData
)
{
  uint32                  nModeVal;

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * De-assert the OUTCTL field.
   */
  nModeVal = HAL_CLK_PLL_HWIO_OUTFV(PLL_MODE, pSource, PLL_OUTCTRL, 0, nModeVal);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait 2 ticks of the output main clock.
   */
  HAL_clk_BusyWait(1);

  /*
   * Put the PLL in reset mode
   */
  nModeVal = HAL_CLK_PLL_HWIO_OUTFV(PLL_MODE, pSource, PLL_BYPASSNL, 0, nModeVal);
  nModeVal = HAL_CLK_PLL_HWIO_OUTFV(PLL_MODE, pSource, PLL_RESET_N, 0, nModeVal);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

} /* END HAL_clk_RimacPLLDisable */


/* ===========================================================================
**  HAL_clk_RimacPLLIsEnabled
**
** ======================================================================== */

static boolean HAL_clk_RimacPLLIsEnabled
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32                  nModeVal;

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * Check whether the PLL is enabled.
   */
  return ((nModeVal & HAL_CLK_PLL_MODE_ACTIVE) == HAL_CLK_PLL_MODE_ACTIVE);

} /* END HAL_clk_RimacPLLIsEnabled */


/* ===========================================================================
**  HAL_clk_RimacPLLConfig
**
** ======================================================================== */

static void HAL_clk_RimacPLLConfig
(
  HAL_clk_SourceDescType   *pSource,
  HAL_clk_SourceConfigType  eConfig
)
{
  /*
   * Take action.
   */
  switch (eConfig)
  {
    case HAL_CLK_CONFIG_PLL_FSM_MODE_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_MODE, pSource, PLL_VOTE_FSM_ENA, 1);
      break;

    case HAL_CLK_CONFIG_PLL_FSM_MODE_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_MODE, pSource, PLL_VOTE_FSM_ENA, 0);
      break;

    case HAL_CLK_CONFIG_PLL_AUX_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX, 0);
      break;

    case HAL_CLK_CONFIG_PLL_AUX_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX, 1);
      break;

    case HAL_CLK_CONFIG_PLL_AUX2_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX2, 0);
      break;

    case HAL_CLK_CONFIG_PLL_AUX2_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX2, 1);
      break;

    case HAL_CLK_CONFIG_PLL_EARLY_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_EARLY, 1);
      break;

    case HAL_CLK_CONFIG_PLL_EARLY_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_EARLY, 0);
      break;

    case HAL_CLK_CONFIG_PLL_MAIN_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_MAIN, 1);
      break;

    case HAL_CLK_CONFIG_PLL_MAIN_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_MAIN, 0);
      break;
    default:
      break;
  }

} /* END HAL_clk_RimacPLLConfig */


/* ===========================================================================
**  HAL_clk_RimacPLLConfigPLL
**
**  Returns:
**    boolean - TRUE if successfully configured. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_RimacPLLConfigPLL
(
  HAL_clk_SourceDescType      *pSource,
  const HAL_clk_PLLConfigType *pConfig,
  HAL_clk_SourceConfigMode     eMode
)
{
  uint32                  nRegVal, nFieldVal;
  boolean                 bEnabled;

  /*
   * Check PLL mode.
   */
  bEnabled = HAL_clk_RimacPLLIsEnabled(pSource);

  /*
   * Disable the PLL.
   */
  if (bEnabled)
  {
    HAL_clk_RimacPLLDisable(
      pSource,
      HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
      NULL);
  }

  /*
   * Program L value.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_L_VAL, pSource, pConfig->nL);

  /*
   * Program ALPHA value.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_ALPHA_VAL, pSource, pConfig->nFracVal);

  /*
   * Get PLL_USER_CTL register value.
   */
  nRegVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL, pSource);

  /*
   * Compute the pre-div value (div-1,2 are supported).
   */
  if (pConfig->nPreDiv == 2)
  {
    nFieldVal = 1;
  }

  /*
   * Default to div-1.
   */
  else
  {
    nFieldVal = 0;
  }

  /*
   * Populate PDIV_CTL field in local variable.
   */
  nRegVal =
    HAL_CLK_PLL_HWIO_OUTFV(PLL_USER_CTL, pSource, PDIV_CTL, nFieldVal, nRegVal);

  /*
   * Compute the post-div value (div-1,2,4, and 8 are supported)
   */
  if (pConfig->PostDivs.Rimac.nMain == 2)
  {
    nFieldVal = 1;
  }
  else if (pConfig->PostDivs.Rimac.nMain == 4)
  {
    nFieldVal = 2;
  }
  else if (pConfig->PostDivs.Rimac.nMain == 8)
  {
    nFieldVal = 3;
  }

  /*
   * Default to div-1.
   */
  else
  {
    nFieldVal = 0;
  }

  /*
   * Populate POSTDIV_CTL field in local variable.
   */
  nRegVal =
    HAL_CLK_PLL_HWIO_OUTFV(PLL_USER_CTL, pSource, POSTDIV_CTL, nFieldVal, nRegVal);

  /*
   *  Check fractional output or integer output.
   */
  if (pConfig->nFracVal != 0)
  {
    nFieldVal = 1;
  }
  else
  {
    nFieldVal = 0;
  }

  /*
   * Populate ALPHA_EN field in local variable.
   */
  nRegVal =
    HAL_CLK_PLL_HWIO_OUTFV(PLL_USER_CTL, pSource, ALPHA_EN, nFieldVal, nRegVal);

  /*
   * Program the USER_CTL register.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL, pSource, nRegVal);

  /*
   * Enable the PLL if it was on before.
   */
  if (bEnabled)
  {
    HAL_clk_RimacPLLEnable(
      pSource,
      HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
      NULL);
  }

  return TRUE;

} /* END HAL_clk_RimacPLLConfigPLL */


/* ===========================================================================
**  HAL_clk_RimacPLLDetectPLLConfig
**
** ======================================================================== */

static void HAL_clk_RimacPLLDetectPLLConfig
(
  HAL_clk_SourceDescType *pSource,
  HAL_clk_PLLConfigType  *pConfig
)
{
  uint32                  nRegVal, nFieldVal;

  /*
   * Clear all config values not modified in this function.
   */
  pConfig->nFracVal = 0;
  pConfig->eFracMode = HAL_CLK_PLL_FRAC_MODE_ALPHA;

  /*
   * Read L value.
   */
  pConfig->nL = HAL_CLK_PLL_HWIO_IN(PLL_L_VAL, pSource);

  /*
   * Read ALPHA value.
   */
  pConfig->nFracVal = HAL_CLK_PLL_HWIO_IN(PLL_ALPHA_VAL, pSource);

  /*
   * No reference clock mux, so default to 0.
   */
  pConfig->nSource = 0;

  /*
   * Read USER_CTL register.
   */
  nRegVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL, pSource);

  /*
   * Extract the pre divider.
   */
  nFieldVal = HAL_CLK_PLL_HWIO_INFV(PLL_USER_CTL, pSource, PDIV_CTL, nRegVal);
  if (nFieldVal == 0)
  {
    pConfig->nPreDiv = 1;
  }
  else if (nFieldVal == 1)
  {
    pConfig->nPreDiv = 2;
  }

  /*
   * Extract the post divider.
   */
  nFieldVal = HAL_CLK_PLL_HWIO_INFV(PLL_USER_CTL, pSource, POSTDIV_CTL, nRegVal);
  if (nFieldVal == 0)
  {
    pConfig->PostDivs.Rimac.nMain = 1;
  }
  else if (nFieldVal == 1)
  {
    pConfig->PostDivs.Rimac.nMain = 2;
  }
  else if (nFieldVal == 2)
  {
    pConfig->PostDivs.Rimac.nMain = 4;
  }
  else if (nFieldVal == 3)
  {
    pConfig->PostDivs.Rimac.nMain = 8;
  }

} /* END HAL_clk_RimacPLLDetectPLLConfig */


/* ===========================================================================
**  HAL_clk_RimacPLLIsPLLConfigEqual
**
** ======================================================================== */

static boolean HAL_clk_RimacPLLIsPLLConfigEqual
(
  const HAL_clk_PLLConfigType  *pConfigA,
  const HAL_clk_PLLConfigType  *pConfigB
)
{
  uint32 nMainDivA, nMainDivB;

  if (HAL_clk_GenericIsPLLConfigEqual(pConfigA, pConfigB))
  {
    /* Treat div-0 and div-1 as the same thing. */
    nMainDivA = MAX(1, pConfigA->PostDivs.Rimac.nMain);
    nMainDivB = MAX(1, pConfigB->PostDivs.Rimac.nMain);

    if (nMainDivA == nMainDivB)
    {
      return TRUE;
    }
  }

  return FALSE;

} /* END HAL_clk_RimacPLLIsPLLConfigEqual */


/* ===========================================================================
**  HAL_clk_RimacPLLIsCalibrated
**
** ======================================================================== */

static boolean HAL_clk_RimacPLLIsCalibrated
(
  HAL_clk_SourceDescType *pSource
)
{
  /* No calibration required. */
  return TRUE;

} /* HAL_clk_RimacPLLIsCalibrated */


/* ===========================================================================
**  HAL_clk_RimacPLLSetCalConfig
**
** ======================================================================== */

static boolean HAL_clk_RimacPLLSetCalConfig
(
  HAL_clk_SourceDescType      *pSource,
  const HAL_clk_PLLConfigType *pConfig
)
{
  /* No calibration required. */
  return TRUE;

} /* HAL_clk_RimacPLLSetCalConfig */


/* ===========================================================================
**  HAL_clk_RimacPLLSetRegSettings
**
** ======================================================================== */

static void HAL_clk_RimacPLLSetRegSettings
(
  HAL_clk_SourceDescType              *pSource,
  const HAL_clk_SourceRegSettingsType *pmSettings
)
{
  HAL_CLK_PLL_HWIO_OUT(PLL_CONFIG_CTL,    pSource, pmSettings->nConfigCtl);
  HAL_CLK_PLL_HWIO_OUT(PLL_TEST_CTL,      pSource, pmSettings->nTestCtl);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL,      pSource, pmSettings->nUserCtl);

} /* END HAL_clk_RimacPLLSetRegSettings */

