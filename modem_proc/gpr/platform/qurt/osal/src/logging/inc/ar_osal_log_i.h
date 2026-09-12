/*========================================================================
  Copyright (c) 2020-2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
======================================================================== */
/**
@file ar_osal_log_i.h

@brief This file declares ar_osal logging utility APIs.
*/

/*========================================================================
Edit History

$Header: 

when       who     what, where, why
--------   ---     -------------------------------------------------------
03/04/2020   rv      Created file.
========================================================================== */

#ifndef _AR_OSAL_LOG_I_H_
#define _AR_OSAL_LOG_I_H_

#include "ar_osal_log.h"
#include <stdint.h>
#include "msg.h"
#include "msgcfg.h"
#ifdef __qdsp6__
#include <hexagon_sim_timer.h>
#endif

uint32_t ar_log_lvl = (AR_ERROR | AR_INFO | AR_DEBUG | AR_CRITICAL | AR_VERBOSE);

// clang-format off
#define PRIO_STR(prio) \
      ( (prio == AR_LOW_PRIO)      ?  " LOW    "  :     \
      ( (prio == AR_MED_PRIO)      ?  " MEDIUM "  :     \
      ( (prio == AR_HIGH_PRIO)     ?  " HIGH   "  :     \
      ( (prio == AR_ERROR_PRIO)    ?  " ERROR  "  :     \
                                       " FATAL  " ))))

//#define DEBUG_AR_OSAL_LOG_UTIL

#endif 
