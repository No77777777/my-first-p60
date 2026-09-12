/*========================================================================
@file ar_osal_log.c
This file contains logging utilities.

Copyright (c) 2020-2021 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

====================================================================== */

/*========================================================================
Edit History

$Header: //components/dev/gpr.common/1.0/sranumul.gpr.common.1.0.mar_3_20_build/platform/qurt/osal/src/ar_osal_log.c#1
$

when       who     what, where, why
--------   ---     -------------------------------------------------------
02/04/10   mwc      Created file.
========================================================================== */
#include "ar_osal_log_i.h"

#define AR_OSAL_MAX_BUF_LENGTH  256

static uint32_t convert_ar_log_lvl_to_diag_lvl(uint32_t ar_lvl)
{
   switch(ar_lvl)
   {
      case AR_LOW_PRIO:
      {
         return MSG_LEGACY_LOW;
      }
      case AR_MED_PRIO:
      {
         return MSG_LEGACY_MED;
      }
      case AR_HIGH_PRIO:
      {
         return MSG_LEGACY_HIGH;
      }      
      case AR_ERROR_PRIO:
      {
         return MSG_LEGACY_ERROR;
      }      
      case AR_FATAL_PRIO:
      {
         return MSG_LEGACY_FATAL;
      }
      default:
      {
         return MSG_LEGACY_LOW;
      }
   }

   return 0;
}

/**
Function to print logging messages in non hexagon architectures (sim & targets)
*/
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

   // print
   uint32_t  xx_ss_mask = convert_ar_log_lvl_to_diag_lvl(level);

#ifdef __qdsp6__
   const msg_const_type xx_mm_log_msg  = { {ln, (MSG_SSID_QDSP6), (xx_ss_mask)}, message, fn};

   msg_send(&xx_mm_log_msg);

#elif __XTENSA__
   const msg_v2_const_type xx_mm_log_msg  = { {ln, (MSG_SSID_QDSP6), (xx_ss_mask)}, message};

   msg_v2_send(&xx_mm_log_msg);
#endif

#ifdef DEBUG_AR_OSAL_LOG_UTIL
   static uint32_t counter = 0;
   printf("printed !sim msg %ld, fn: %s, ln: %ld string: %s \n ", counter++, fn, ln, message);
#endif 

   va_end(_ArgList);
}