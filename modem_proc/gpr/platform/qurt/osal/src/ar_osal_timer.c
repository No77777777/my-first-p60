/**
 *=============================================================================
 * \file ar_osal_timer.cpp
  This file contains utilities for using timer functionalities.

  Copyright (c) 2020-2022 Qualcomm Technologies, Inc.
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
 * \brief ar_timer_get_time_in_us
 *        Gets the wall clock time in microseconds
 * \return
 *        Wall clock time in microseconds.
 */
uint64_t ar_timer_get_time_in_us(void)
{
   uint64_t ullCurrTime =(uint64_t)qurt_sysclock_timetick_to_us(qurt_sysclock_get_hw_ticks());
   return ullCurrTime;
}