/*
===========================================================================
*/
/**
  @file ClockMSSLite.c
  
  This is a lightweight, non-DAL, implementation for clocks that must
  be available prior to the DAL clock driver being initialized.
*/
/*  
  ====================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential. 

  ==================================================================== 
  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/stub/ClockMSSLite.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $
  ====================================================================
*/ 


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockDriver.h"


/*=========================================================================
      Functions
==========================================================================*/


/* =========================================================================
**  Function : Clock_EnablePRNG
** =========================================================================*/

DALResult Clock_EnablePRNG(void)
{
  return DAL_SUCCESS;

} /* END Clock_EnablePRNG */


/* =========================================================================
**  Function : Clock_DisablePRNG
** =========================================================================*/

DALResult Clock_DisablePRNG(void)
{
  return DAL_SUCCESS;

} /* END Clock_TransferReferenceCount */


/* =========================================================================
**  Function : Clock_TransferReferenceCount
** =========================================================================*/

DALResult Clock_TransferReferenceCount(void)
{
  return DAL_SUCCESS;

} /* END Clock_TransferReferenceCount */

