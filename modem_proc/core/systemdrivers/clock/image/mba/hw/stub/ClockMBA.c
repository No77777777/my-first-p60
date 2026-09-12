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

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mba/hw/stub/ClockMBA.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "com_dtypes.h"


/*=========================================================================
      Functions
==========================================================================*/

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
  return TRUE;

} /* END ClockMBA_DeInit */

