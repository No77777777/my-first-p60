/*
===========================================================================
*/
/**
  @file ClockMBA.c

  Implementation file for the MBA's MSS clock API.
*/
/*
  ====================================================================

  Copyright (c) 2012 Qualcomm Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mba/hw/sdm845/ClockMBA.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/


#include <HALhwio.h>
#include "ClockMBAHWIO.h"
#include "HALclkFabiaPLL.h"


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
#define CLOCK_MBA_UPDATE_TIMEOUT_US 1000


/*
 * Common clock branch fields/masks (*_CBCR register)
 */
#define HAL_CLK_BRANCH_CTRL_REG_CLK_OFF_FMSK                           0x80000000
#define HAL_CLK_BRANCH_CTRL_REG_CLK_OFF_SHFT                           31
#define HAL_CLK_BRANCH_CTRL_REG_CLK_DIV_FMSK                           0x01FF0000
#define HAL_CLK_BRANCH_CTRL_REG_CLK_DIV_SHFT                           16
#define HAL_CLK_BRANCH_CTRL_REG_CLK_FORCE_MEM_CORE_ON_FMSK             0x00004000
#define HAL_CLK_BRANCH_CTRL_REG_CLK_FORCE_MEM_PERIPH_ON_FMSK           0x00002000
#define HAL_CLK_BRANCH_CTRL_REG_CLK_FORCE_MEM_PERIPH_OFF_FMSK          0x00001000
#define HAL_CLK_BRANCH_CTRL_REG_CLK_ENABLE_FMSK                        0x00000001
#define HAL_CLK_BRANCH_CTRL_REG_CLK_ENABLE_SHFT                        0


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof(x[0]))
#endif

/*
 * MPLL3 min frequency is 15 * 19.2 MHz = 288.00 MHz
 * MPLL3 max frequency is 62 * 19.2 MHz = 1190.40 MHz
 * MPLL3 calibration frequency is (15+62)/2 = 38 * 19.2 MHz = 729.6MHz
 */
#define PLL3_CALIB_L_VAL 38

/*
 * MPLL0 max frequency at NOMINAL is 49 * 19.2 MHz = 940.8MHz
 */
#define PLL3_DEFAULT_L_VAL 49

/*
 * PLL lock timeout (2x max locking time).
 */
#define CLOCK_MBA_PLL_LOCK_TIMEOUT_US 200

/*
 * Warm-up time in microseconds after turning on the PLL.
 */
#define CLOCK_MBA_PLL_WARMUP_TIME_US 25

/*
 * Update time in microseconds setting the PLL UPDATE bit (2x HPG specs)
 */
#define CLOCK_MBA_PLL_UPDATE_TIME_US 2

/*
 * Standby time in microseconds after setting PLL BYPASSNL=1
 */
#define CLOCK_MBA_PLL_STANDBY_TIME_US 5


/*=========================================================================
      External and forward references
==========================================================================*/


extern void mba_modem_wait_microseconds(uint32 microseconds);
boolean WaitForClear(uintnt nAddr, uint32 nMask, uint32 nWait);
boolean WaitForSet(uintnt nAddr, uint32 nMask, uint32 nWait);
static boolean ClockMBA_EnablePLLDynProg(void);



/*=========================================================================
      Data
==========================================================================*/


static const HAL_clk_ClockRegistersType aClockRegisters[] =
{
  /*
   * Clock Name: gcc_prng_ahb_clk
   */
  {
    .nBranchAddr = HWIO_ADDR(GCC_PRNG_AHB_CBCR),
    .mHWVotable  = HAL_CLK_FMSK(PROC_CLK_BRANCH_ENA_VOTE, PRNG_AHB_CLK_ENA)
  },

  /*
   * Clock Name: clk_axi_dma
   */
  {
    .nBranchAddr = HWIO_ADDR(MSS_AXI_DMA_CBCR),
    .mHWVotable  = {0, 0}
  },

  /*
   * Clock Name: clk_axi_crypto
   */
  {
    .nBranchAddr = HWIO_ADDR(MSS_AXI_CRYPTO_CBCR),
    .mHWVotable  = {0, 0}
  },

  /*
   * Clock Name: clk_bus_crypto
   */
  {
    .nBranchAddr = HWIO_ADDR(MSS_BUS_CRYPTO_CBCR),
    .mHWVotable  = {0, 0}
  },
};


/*=========================================================================
      Functions
==========================================================================*/


/* ===========================================================================
**  ClockMBA_EnableClock
**
** ======================================================================== */
/**
  Enable a clock using the specified register and mask.

  @param
  None

  @return
  TRUE  -- Successfully enabled clock.
  FALSE -- Failed to enable clock.
  None

  @dependencies
  None
*/

static boolean ClockMBA_EnableClock
(
  const HAL_clk_ClockRegistersType *pRegisters
)
{
  uintnt nAddr;
  uint32 nVal, nMask;

  /*
   * Sanity check - All clocks must have a CBCR
   */
  if (pRegisters->nBranchAddr == 0)
  {
    return FALSE;
  }

  /*
   * Check for HW votable registers
   */
  nAddr = pRegisters->mHWVotable.nAddr;
  if(nAddr)
  {
    nMask = pRegisters->mHWVotable.nMask;
  }
  else
  {
    nAddr = pRegisters->nBranchAddr;
    nMask = HAL_CLK_BRANCH_CTRL_REG_CLK_ENABLE_FMSK;
  }

  /*
   * Sanity check
   */
  if(nAddr == 0)
  {
    return FALSE;
  }

  /*
   * Enable clock
   */
  nVal  = inpdw(nAddr) | nMask;
  outpdw(nAddr, nVal);

  /*
   * Wait for the clock to turn on
   */
  nAddr = pRegisters->nBranchAddr;
  if (!WaitForClear(nAddr, HAL_CLK_BRANCH_CTRL_REG_CLK_OFF_FMSK, CLOCK_MBA_UPDATE_TIMEOUT_US))
  {
      return FALSE;
  }

  return TRUE;

} /* ClockMBA_EnableClock */


/* ===========================================================================
**  ClockMBA_DisableClock
**
** ======================================================================== */
/**
  Disable a clock using the specified register and mask.

  @param
  None

  @return
  None

  @dependencies
  None
*/

static void ClockMBA_DisableClock
(
  const HAL_clk_ClockRegistersType *pRegisters
)
{
  uintnt nAddr;
  uint32 nVal, nMask;

  /*
   * Check for HW votable registers
   */
  nAddr = pRegisters->mHWVotable.nAddr;
  if(nAddr)
  {
    nMask = pRegisters->mHWVotable.nMask;
  }
  else
  {
    nAddr = pRegisters->nBranchAddr;
    nMask = HAL_CLK_BRANCH_CTRL_REG_CLK_ENABLE_FMSK;
  }

  /*
   * Sanity check
   */
  if(nAddr == 0)
  {
    return;
  }

  /*
   * Disable clock
   */
  nVal  = inpdw(nAddr);
  nVal &= ~nMask;
  outpdw(nAddr, nVal);

} /* ClockMBA_DisableClock */


/* =========================================================================
**  Function : ClockMBA_Init
** =========================================================================*/
/**
  Initialize the MBA clock driver.

  @param
  None

  @return
  TRUE -- Init was successful.
  FALSE -- Init failed.

  @dependencies
  None
*/

boolean ClockMBA_Init(void)
{
  uint32 i;

  /*-----------------------------------------------------------------------*/
  /*  Enable PLL dynamic programming.                                      */
  /*-----------------------------------------------------------------------*/

  if (ClockMBA_EnablePLLDynProg() != TRUE)
  {
      return FALSE;
  }

  /*-----------------------------------------------------------------------*/
  /*  Enable clocks.                                                       */
  /*-----------------------------------------------------------------------*/

  for (i = 0; i < ARRAY_SIZE(aClockRegisters); i++)
  {
    if (ClockMBA_EnableClock(&aClockRegisters[i]) != TRUE)
    {
      return FALSE;
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Hand off GPLL0 vote from mPBL to MPSS.                                */
  /*-----------------------------------------------------------------------*/

  if (HWIO_INF(MSS_Q6_PBL_GPLL_ENA_VOTE, GPLL0))
  {
    /*
     * Set GPLL0 vote using MPSS voting register in GCC.
     */
    HWIO_OUTF(GCC_MSS_Q6_GPLL_ENA_VOTE, GPLL0, 0x1);

    /*
     * Clear GPLL0 vote from mPBL register.
     */
    HWIO_OUTF(MSS_Q6_PBL_GPLL_ENA_VOTE, GPLL0, 0x0);
  }

  /*-----------------------------------------------------------------------*/
  /* Init complete.                                                        */
  /*-----------------------------------------------------------------------*/

  return TRUE;

} /* END ClockMBA_Init */


/* =========================================================================
**  Function : ClockMBA_DeInit
** =========================================================================*/
/**
  Deinitialize the MBA clock driver.

  @param
  None

  @return
  TRUE -- Deinit was successful.
  FALSE -- Deinit failed.

  @dependencies
  None
  NOTE: the loop counter i MUST be a signed integer.
*/

boolean ClockMBA_DeInit(void)
{
  int32 i;

  /*-----------------------------------------------------------------------*/
  /*  Disable clocks (reverse order of init).                              */
  /*-----------------------------------------------------------------------*/

  for (i = ARRAY_SIZE(aClockRegisters)-1; i >= 0; i--)
  {
    ClockMBA_DisableClock(&aClockRegisters[i]);
  }

  /*-----------------------------------------------------------------------*/
  /* Deinit complete.                                                      */
  /*-----------------------------------------------------------------------*/

  return TRUE;

} /* END ClockMBA_DeInit */


/* ===========================================================================
**  ClockMBA_EnablePLLDynProg
**
** ======================================================================== */
/**
  Enable PLL dynamic programming.

  @param
  None

  @return
  TRUE  -- Successfully enabled PLL dynamic programming mode.
  FALSE -- Failed to enable.
  None

  @dependencies
  None
*/

static boolean ClockMBA_EnablePLLDynProg
(
  void
)
{

    /* 
     *  1. Switch the Q6 temporarily to XO
     */
    HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_SEL, 0); 
    HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_DIV, 1); 
    HWIO_OUTF(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE, 1);   
    if( !WaitForClear(HWIO_ADDR(MSS_QDSP6SS_CORE_CMD_RCGR),  
                                 HWIO_FMSK(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE), 
                                 CLOCK_MBA_UPDATE_TIMEOUT_US))  
    {
      return FALSE;
    }

    /* 
     * 2. Shut down the PLL
     */
    HWIO_OUTF(MSS_QDSP6SS_PLL_MODE, PLL_OUTCTRL, 0);
    mba_modem_wait_microseconds(1);
    HWIO_OUTF(MSS_QDSP6SS_PLL_MODE, PLL_RESET_N, 0);
    HWIO_OUT(MSS_QDSP6SS_PLL_OPMODE, 0);

    /* 
     * 2.5 Update the Q6 PLL register settings. The offline PLL settings
     * will be updated automatically in the main image.
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
    mba_modem_wait_microseconds(CLOCK_MBA_PLL_WARMUP_TIME_US);
    /* wait for the PLL to lock */
    if( !WaitForSet(HWIO_ADDR(MSS_QDSP6SS_PLL_MODE),
                                 HWIO_FMSK(MSS_QDSP6SS_PLL_MODE, PLL_LOCK_DET_FINE),
                                 CLOCK_MBA_PLL_LOCK_TIMEOUT_US))
    {
      return FALSE;
    }

    /*
     * Enable the PLL MAIN and OUTCTRL
     */
    HWIO_OUTF(MSS_QDSP6SS_PLL_USER_CTL, PLLOUT_MAIN, 1);
    HWIO_OUTF(MSS_QDSP6SS_PLL_MODE, PLL_OUTCTRL, 1);  

    /*
     * 5. Set the STRAP_ACC value for all parts. These values address HW CRs:
     * QCTDD04276599, QCTDD04285296.
     */
    HWIO_OUTF(MSS_QDSP6SS_STRAP_QMC_ACC_SEL, STRAP_QMC_ACC_SEL, 0x1);
    HWIO_OUT(MSS_QDSP6SS_STRAP_QMC_ACC_U, 0xC0000000);

    /* 
     * 6. Switch the Q6 back from XO to the PLL source
     */
    /* select PLL3 source */
    HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_SEL, 1);  
    /* set the Q6 divider */
    HWIO_OUTF(MSS_QDSP6SS_CORE_CFG_RCGR, SRC_DIV, 1); 
    /* Initiate MSS_QDSP6SS_CORE_CMD_RCGR update */
    HWIO_OUTF(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE, 1);    
    /* wait with timeout for register update */
    if( !WaitForClear(HWIO_ADDR(MSS_QDSP6SS_CORE_CMD_RCGR),   
                                 HWIO_FMSK(MSS_QDSP6SS_CORE_CMD_RCGR, UPDATE),
                                 CLOCK_MBA_UPDATE_TIMEOUT_US))  
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
  while(inpdw(nAddr) & nMask)
  {
    if (nTimeout++ >= nWait)
    {
      return FALSE;
    }

    mba_modem_wait_microseconds(1);
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
  while((inpdw(nAddr) & nMask) == 0)
  {
    if (nTimeout++ >= nWait)
    {
      return FALSE;
    }

    mba_modem_wait_microseconds(1);
  }

  return TRUE;
} /* END WaitForSet */

