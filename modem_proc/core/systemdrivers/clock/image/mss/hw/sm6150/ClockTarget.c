/*
===========================================================================
*/
/**
  @file ClockTarget.c

*/
/*
  ====================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sm6150/ClockTarget.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $
  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockImage.h"
#include "HALclkHWIO.h"
#include "HALhwio.h"
#include "uSleep_mode_trans.h"
#include "rcecb.h"
#include "rcinit.h"


/*=========================================================================
      Prototypes
==========================================================================*/

static boolean HAL_clk_GPLLStubEnableVote (HAL_clk_SourceDescType *pSource);
static void  Clock_UImageCallback (uSleep_state_notification state);
static void Clock_TargetPostInit (void);


/*=========================================================================
      Data
==========================================================================*/

/*
 * Stubbed PLL control for GPLLs.
 */
HAL_clk_SourceControlType HAL_clk_GPLLStubPLLControl =
{
  .EnableVote = HAL_clk_GPLLStubEnableVote,
};


/*=========================================================================
      Functions
==========================================================================*/

/* =========================================================================
**  Function : Clock_InitTarget
** =========================================================================*/
/*
  See ClockDriver.h.
*/

DALResult Clock_InitTarget (ClockDrvCtxt *pDrvCtxt)
{
  DALResult     eResult;
  RCECB_HANDLE  hRCECB;

  eResult = Clock_InitImage(pDrvCtxt);
  if (eResult != DAL_SUCCESS)
  {
    return DAL_ERROR;
  }

  /*
   * Remove any votes that may be leftover from earlier boot (MBA). We rely
   * on BCM votes keeping GPLL0 on implicitly.
   */
  HWIO_OUTF(GCC_MSS_Q6_GPLL_ENA_VOTE, GPLL0, 0);

  /*
   * Vote for MSS-specific GPLL0 branch. We can't set the SLEEP_ENA bit
   * because it's apparently required for the RSC to function on the way down.
   */
  HWIO_OUTF(GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE, MSS_GPLL0_DIV_CLK_SRC_ENA, 1);
  
  /*
   * Set divider to div-2 to provide MPSS with a 150MHz source.
   * The input clock to CDIV is 300 MHz (gpll0_out_aux2_div)
   */
  HWIO_OUTF(GCC_MSS_PLL0_MAIN_DIV_CDIVR, CLK_DIV, 1);

  /*
   * Register for the end of RCINIT.
   */
  hRCECB = rcecb_register_name(RCINIT_RCECB_INITGROUPS, Clock_TargetPostInit);
  if (hRCECB == RCECB_NULL)
  {
    return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END Clock_InitTarget */


/* =========================================================================
**  Function : Clock_TargetPostInit
** =========================================================================*/
/*
  Perform any necessary actions once image initialization has completed
  (i.e. end of rcinit).
*/

static void Clock_TargetPostInit (void)
{
  uSleep_notification_cb_handle hUImageCallback;

  /*
   * Register micro image callback used to vote for GPLL0 while in micro
   * image, since all BCM votes go away in that case.
   */
  hUImageCallback =
    uSleep_registerNotificationCallback(100, 100, Clock_UImageCallback);
  if (!hUImageCallback)
  {
    DALSYS_LogEvent(DALDEVICEID_CLOCK, DALSYS_LOGEVENT_WARNING,
      "Unable to register uSleep callback.");
  }

} /* END Clock_TargetPostInit */


/* =========================================================================
**  Function : ClockStub_InitTarget
** =========================================================================*/
/*
  See ClockDriver.h.
*/

DALResult ClockStub_InitTarget (ClockDrvCtxt *pDrvCtxt)
{
  return DAL_SUCCESS;

} /* END ClockStub_InitTarget */


/* =========================================================================
**  Function : Clock_ProcessorSleep
** =========================================================================*/
/*
  See DDIClock.h
*/

DALResult Clock_ProcessorSleep
(
  ClockDrvCtxt *pDrvCtxt,
  ClockSleepModeType eMode,
  uint32 nFlags
)
{
  return DAL_SUCCESS;

} /* END Clock_ProcessorSleep */


/* =========================================================================
**  Function : Clock_ProcessorRestore
** =========================================================================*/
/*
  See DDIClock.h
*/

DALResult Clock_ProcessorRestore
(
  ClockDrvCtxt *pDrvCtxt,
  ClockSleepModeType eMode,
  uint32 nFlags
)
{
  return DAL_SUCCESS;

} /* END Clock_ProcessorRestore */


/* =========================================================================
**  Function : HAL_clk_GPLLStubEnableVote
** =========================================================================*/

static boolean HAL_clk_GPLLStubEnableVote
(
  HAL_clk_SourceDescType *pSource
)
{
  /*
   * Don't actually vote on PLL. We rely on BCM votes keeping GPLL0 on
   * implicitly. We can't enable our vote directly, since there are
   * suppressible domains requiring GPLL0, so our vote would never
   * go away. This would leave GPLL0 while entering XO shutdown. Relying
   * on BCM votes means GPLL0 will be on while active and automatically
   * turn off when modem goes down.
   */
  return TRUE;

} /* END HAL_clk_GPLLStubEnableVote */


/* =========================================================================
**  Function : Clock_UImageCallback
** =========================================================================*/

static void Clock_UImageCallback (uSleep_state_notification state)
{
  if (state == USLEEP_STATE_ENTER)
  {
    /*
     * When entering micro image mode, assert vote for GPLL0 since all BCM votes
     * will go away and we can't rely on GPLL0 being on anymore. XO is still
     * voted, so this is safe.
     */
    HWIO_OUTF(GCC_MSS_Q6_GPLL_ENA_VOTE, GPLL0, 1);
  }
  else if (state == USLEEP_STATE_EXIT)
  {
    /*
     * When exiting micro image, remove vote for GPLL0 since BCM votes are back
     * and we can rely on BCM keeping it on.
     */
    HWIO_OUTF(GCC_MSS_Q6_GPLL_ENA_VOTE, GPLL0, 0);
  }

} /* END Clock_UImageCallback */
