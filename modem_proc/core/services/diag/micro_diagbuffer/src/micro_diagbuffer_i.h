#ifndef MICRO_DIAGBUFFER_I_H
#define MICRO_DIAGBUFFER_I_H
/*============================================================================
  micro_diagbuffer_i.h

  micro diagbuffer internal header file

  Copyright (c) 2017-2018 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
============================================================================*/

/* $Header: //components/rel/core.mpss/10.0/services/diag/micro_diagbuffer/src/micro_diagbuffer_i.h#1 $ */

/*==========================================================================
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/09/18   sa      Moved Non-Public API to internal files.

===========================================================================*/

#include "micro_ULog.h"
#include "micro_diagbuffer.h"

/**
 * <!-- micro_ULog_Enable -->
 *
 * @brief Enable the log. Logs are enabled on create. You only need to invoke
 * this routine if you've previously disabled the log.
 *
 * @param h : Handle to the microUlog
 *
 * @return MICRO_ULOG_SUCCESS on success, MICRO_ULOG_ERROR on error
 */
micro_ULogResult micro_ULog_Enable( micro_ULogHandle h );

/**
 * <!-- micro_diagbuffer_enable -->
 *
 * @brief Enable the micro diagbuffer. micro diagbuffers are enabled on create. 
 * You only need to invoke this routine if you've previously disabled the 
 * micro diagbuffer.
 *
 * This API replaces micro_ULog_Enable() of microULog.
 *
 * @param h : Handle to the micro diagbuffer
 *
 * @return MICRO_DIAGBUFFER_SUCCESS : if successful
 *         MICRO_DIAGBUFFER_ERROR   : otherwise 
 */
#define micro_diagbuffer_enable(h) \
              micro_ULog_Enable(h)

#endif /* MICRO_DIAGBUFFER_I_H */