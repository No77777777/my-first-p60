#ifndef UCLOCK_H
#define UCLOCK_H
/*
===========================================================================
*/
/**
  @file uClock.h
  @brief Public definitions include file for accessing the micro lightweight
         clock device driver.
*/
/*===========================================================================
NOTE: The @brief description and any detailed descriptions above do not appear 
      in the PDF. 

      This file is the public header file for the MPSS minidump mode clock
      driver implementation. It is meant to be used by a small subset of
      drivers, which cannot call into the DAL clock driver APIs. These APIs
      should only be invoked in minidump mode. Otherwise, these will return
      a failure.

===========================================================================*/
/*
  ===========================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.  
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================

  $Header: //components/rel/core.mpss/10.0/api/systemdrivers/uClock.h#1 $
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


/*=========================================================================
      Type Definitions
==========================================================================*/

/*
 * Enumeration of available clocks for the clock controller. Add additional
 * clocks to the bottom of this list as they will be used to index clock tables.
 */
typedef enum
{
  CLOCK_MSS_BUS_CRYPTO_CLK,
  CLOCK_MSS_AXI_CRYPTO_CLK,
  CLOCK_GCC_PRNG_AHB_CLK,

  CLOCK_TOTAL_CLOCK_ENUMS,

}uClockIdType;


/*=========================================================================
      Function Definitions.
==========================================================================*/


/* ============================================================================
**  Function : uClock_EnableClock
** ============================================================================
*/
/**
  Enables a clock. Will return FALSE if called outside of minidump mode.
    
  @param[in]  eClockId  - The ID of the clock to enable.

  @return
  TRUE  - The clock was enabled.
  FALSE - Otherwise.
  
  @dependencies
  None.

*/

boolean uClock_EnableClock(uClockIdType eClockId);


/* ============================================================================
**  Function : uClock_DisableClock
** ============================================================================
*/
/**
  Disables a clock. Will return FALSE if called outside of minidump mode.
    
  @param[in]  eClockId  - The ID of the clock to disable.

  @return
  TRUE  - The clock was disabled.
  FALSE - Otherwise.
  
  @dependencies
  None.

*/

boolean uClock_DisableClock(uClockIdType eClockId);


/* ============================================================================
**  Function : uClock_IsClockOn
** ============================================================================
*/
/**
  Returns whether a clock is on or not.
  Will return FALSE if called outside of minidump mode.
    
  @param[in]  eClockId  - The ID of the clock to check status on.

  @return
  TRUE  - The clock is on.
  FALSE - The clock is off.
  
  @dependencies
  None.

*/

boolean uClock_IsClockOn(uClockIdType eClockId);

#endif /* UCLOCK_H */
