/*
===========================================================================
*/
/**
  @file ClockTarget.c

*/
/*
  ====================================================================

  Copyright (c) 2016 Qualcomm Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdx24/ClockTarget.c#1 $
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
  DALResult eResult;

  eResult = Clock_InitImage(pDrvCtxt);
  if (eResult != DAL_SUCCESS)
  {
    return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END Clock_InitTarget */

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

