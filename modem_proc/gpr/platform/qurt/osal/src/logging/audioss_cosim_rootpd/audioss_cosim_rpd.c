/*========================================================================
@file ar_osal_log.c
This file contains logging utilities.

Copyright (c) 2020, 2021 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

====================================================================== */

/*========================================================================
Edit History

$Header: //components/dev/gpr.common/1.0/sranumul.gpr.common.1.0.mar_3_20_build/platform/qurt/osal/src/ar_osal_log.c#1
$

when       who     what, where, why
--------   ---     -------------------------------------------------------
21/05/21   amitabhi      Created file.
========================================================================== */
#include "ar_osal_log_i.h"
//#include <xtensa/tie/xt_timer.h>

#define DOMAIN "CC_DSP"

extern volatile uint64_t sys_tick_counter;

#define timetick_freq 9600000 //MCLK or RCO Frequency Value

typedef enum
{
    T_TICK,
    T_USEC,
    T_MSEC,
    T_SEC,
    T_MIN,
    T_HOUR,

    /* Identifier to use if no return value from timer_clr/get is needed */
    T_NONE = T_TICK
} timetick_unit_type;
/**
Function to print logging messages in non hexagon architectures (sim & targets)
*/

uint64 gpr_time_conv_from_ticks(uint64 time, timetick_unit_type unit)
{
    /* Factor for converting to hours or minutes */
    uint64 scale = 1;
    /* Timetick is not yet initialized at this point, return 0 */
    if (timetick_freq == 0)
    {
        return 0;
    }

    switch (unit)
    {
    case T_HOUR:
        /*  Scale minutes to hours */
        scale *= 60;
        /* fall-through */

    case T_MIN:
        /* Scale seconds to minutes */
        scale *= 60;
        /* fall-through */

    case T_SEC:
        /* Convert slow clocks to seconds (or minutes, or hours) */
        time = ((time * 1.0) / (timetick_freq * scale));
        break;

    case T_MSEC:
        /* Convert ticks to milliseconds */
        time = ((time * 1000.0) / timetick_freq);
        break;

    case T_USEC:
        /* Convert ticks to microseconds */
        time = ((time * 1000000.0) / timetick_freq);
        break;

    case T_TICK:
    default:
        /* time = time; */
        break;
    }

    return time;
}

_IRQL_requires_max_(DISPATCH_LEVEL) void ar_log(_In_ uint32_t  level,
                                                  _In_opt_ const char_t *log_tag,
                                                  _In_ const char_t *fn,
                                                  _In_ int32_t ln,
                                                  _In_ const char_t *format,
                                                  ...)
{
   // Print the variable arguments
   va_list _ArgList = { 0 };
   va_start(_ArgList, format);

   #define MAX_MESSAGE_SIZE 256

   char message[MAX_MESSAGE_SIZE];
   vsnprintf (message, MAX_MESSAGE_SIZE, format, _ArgList);

   switch(level)
   {
      case AR_LOW_PRIO:
      {
    	  uint64_t cycles = sys_tick_counter;//XT_RSR_CCOUNT();
    	  uint32_t tms = gpr_time_conv_from_ticks(cycles,T_MSEC);
    	  uint32_t tsec = gpr_time_conv_from_ticks(cycles,T_SEC);
    	  uint32_t tmin = gpr_time_conv_from_ticks(cycles,T_MIN);
    	  printf("LOW    %s %02d:%02d:%03d @ %25.25s : %4d : %s\n",DOMAIN, tmin, tsec-60*tmin, tms - 1000*tsec, fn, ln, message);
    	  break;
      }
      case AR_MED_PRIO:
      {
    	  uint64_t cycles = sys_tick_counter;//XT_RSR_CCOUNT();
    	  uint32_t tms = gpr_time_conv_from_ticks(cycles,T_MSEC);
    	  uint32_t tsec = gpr_time_conv_from_ticks(cycles,T_SEC);
    	  uint32_t tmin = gpr_time_conv_from_ticks(cycles,T_MIN);
    	   printf("MEDIUM %s %02d:%02d:%03d @ %25.25s : %4d : %s\n",DOMAIN, tmin, tsec-60*tmin, tms - 1000*tsec, fn, ln, message);
    	   break;
      }
      case AR_HIGH_PRIO:
      {
    	  uint64_t cycles = sys_tick_counter;//XT_RSR_CCOUNT();
    	  uint32_t tms = gpr_time_conv_from_ticks(cycles,T_MSEC);
    	  uint32_t tsec = gpr_time_conv_from_ticks(cycles,T_SEC);
    	  uint32_t tmin = gpr_time_conv_from_ticks(cycles,T_MIN);
    	  printf("HIGH   %s %02d:%02d:%03d @ %25.25s : %4d : %s\n",DOMAIN, tmin, tsec-60*tmin, tms - 1000*tsec, fn, ln, message);
    	  break;
      }
      case AR_ERROR_PRIO:
      {
    	  uint64_t cycles = sys_tick_counter;//XT_RSR_CCOUNT();
    	  uint32_t tms = gpr_time_conv_from_ticks(cycles,T_MSEC);
    	  uint32_t tsec = gpr_time_conv_from_ticks(cycles,T_SEC);
    	  uint32_t tmin = gpr_time_conv_from_ticks(cycles,T_MIN);
    	  printf("ERROR  %s %02d:%02d:%03d @ %25.25s : %4d : %s\n",DOMAIN, tmin, tsec-60*tmin, tms - 1000*tsec, fn, ln, message);
    	  break;
      }
      case AR_FATAL_PRIO:
      {
    	  uint64_t cycles = sys_tick_counter;//XT_RSR_CCOUNT();
    	  uint32_t tms = gpr_time_conv_from_ticks(cycles,T_MSEC);
    	  uint32_t tsec = gpr_time_conv_from_ticks(cycles,T_SEC);
    	  uint32_t tmin = gpr_time_conv_from_ticks(cycles,T_MIN);
    	  printf("FATAL  %s %02d:%02d:%03d @ %25.25s : %4d : %s\n",DOMAIN, tmin, tsec-60*tmin, tms - 1000*tsec, fn, ln, message);
    	  break;
      }
      default:
      {
    	  break;
      }
   }

   va_end(_ArgList);
}
