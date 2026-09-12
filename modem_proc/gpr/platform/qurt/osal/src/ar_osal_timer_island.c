/**
 *=============================================================================
 * \file ar_osal_timer_island.cpp
  This file contains utilities for using timer functionalities.

  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  when       who     what, where, why
  --------   ---     -------------------------------------------------------
  11/08/2020  np    Created the file.
 *============================================================================= */

#include "ar_osal_timer.h"
#include "ar_osal_error.h"
#include "ar_osal_types.h"
#include "qurt.h"


/**
 * \brief ar_timer_get_time_in_ms
 *        Gets the wall clock time in milliseconds
 *
 * Converts ticks into milliseconds,
 *     1 tick = 1/19.2MHz seconds
 *
 * MilliSeconds = Ticks * 10ULL/192000ULL. Compiler uses magic multiply functions
 * to resolve this repeated fractional binary. Performance is 10 cycles.
 *
 * \return
 *        Wall clock time in milliseconds.
 */
uint64_t ar_timer_get_time_in_ms(void)
{
   uint64_t time = (uint64_t)((qurt_sysclock_get_hw_ticks()) * 10ull / 192000ull);
   return time;
}
