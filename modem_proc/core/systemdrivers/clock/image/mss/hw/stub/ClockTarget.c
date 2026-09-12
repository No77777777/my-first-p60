/*
===========================================================================
*/
/**
  @file ClockTarget.c

*/
/*
  ====================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/stub/ClockTarget.c#2 $
  $DateTime: 2019/07/29 04:10:44 $
  $Author: pwbldsvc $
  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockImage.h"
#include "npa_resource.h"

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
  return ClockStub_InitTarget(pDrvCtxt);

} /* END Clock_InitTarget */


/* =========================================================================
**  Function : ClockStub_InitTarget
** =========================================================================*/
/*
  See ClockDriver.h.
*/
static const char *ClockNPANodeName[] =
{
  "/clk/cpu",
  "/clk/cpu1",
  "/clk/cpu.test",
  "/clk/mss/config_bus",
  "/xo/cxo",
  "/clk/qdss",
  "/clk/dcvs.ena",
  "/ipa/pc",
  "/clk/snoc_periph",
  "/clk/cnoc",
  "/clk/snoc",
  "/clk/snoc_gc",
  "/clk/snoc_monaq",
  "/clk/bimc",
  "/clk/shub",
  "/clk/ce1",
  "/clk/mmnrt",
  "/clk/mmrt",
  "/clk/qup",
  "/clk/ipa",
};
DALResult ClockStub_InitTarget (ClockDrvCtxt *pDrvCtxt)
{
  uint32 i;
  /*-----------------------------------------------------------------------*/
  /* Create stubs for each NPA node.                                       */
  /*-----------------------------------------------------------------------*/

  for (i = 0; i < ARR_SIZE(ClockNPANodeName); i++)
  {
    npa_stub_resource(ClockNPANodeName[i]);
  }
  
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

