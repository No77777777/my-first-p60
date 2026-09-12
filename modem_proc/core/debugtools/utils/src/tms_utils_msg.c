/**
@file tms_utils_msg.c
@brief This file contains logs related stuff.
*/
/*=============================================================================
Copyright (c) 2017-18 by Qualcomm Technologies, Inc.
All rights reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/utils/src/tms_utils_msg.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
===============================================================================*/
#include "err.h"
#include "tms_utils_msg.h"
#include "tms_dll_api.h"

utils_log_t utils_log;

/**
 * @brief tms_utils_msg_init
 *
 * Initialize ULOG handles.
*/
void tms_utils_msg_init( void )
{

  /* Create TLS key for tasks which call TMS_MSG_SPRINTF macro */ 
  if(QURT_EOK != qurt_tls_create_key(&utils_log.tls_key, &free))
  {
    ERR_FATAL("TMS ULOG TLS key create failed",0,0,0);
  }
  
#ifdef TMS_MSG_DIAG_F3
  utils_log.diag_f3_flag = 1;
#endif  // TMS_MSG_DIAG_F3

#ifdef TMS_ULOG_LOW_ENABLED
  utils_log.ulog_low_enabled = 1;
#endif  // TMS_ULOG_LOW_ENABLED

#ifdef TMS_ULOG_MESSAGE
    /* Initialize tms_low_ulog_handle */
  ULogFront_RealTimeInit( &utils_log.ulog_handle,
                          "TMS RUNTIME", // renamed to avoid confusion with "TM_ULOG"
                            TMS_ULOG_BUFFER_SIZE,
                            ULOG_MEMORY_LOCAL,
                            ULOG_LOCK_OS );
#endif  // TMS_ULOG_MESSAGE

#ifdef TMS_STARTUP_ULOG_MESSAGE
  /* Initialize tms_low_ulog_handle */
  ULogFront_RealTimeInit( &utils_log.startup_ulog_handle,
                          "TMS STARTUP",
                          TMS_STARTUP_ULOG_BUFFER_SIZE,
                          ULOG_MEMORY_LOCAL,
                          ULOG_LOCK_OS );
#endif  // TMS_STARTUP_ULOG_MESSAGE

#ifdef TMS_SHUTDOWN_ULOG_MESSAGE
  /* Initialize tms_low_ulog_handle */
  ULogFront_RealTimeInit( &utils_log.shutdown_ulog_handle,
                          "TMS SHUTDOWN", 
                          TMS_SHUTDOWN_ULOG_BUFFER_SIZE,
                          ULOG_MEMORY_LOCAL,
                          ULOG_LOCK_OS );
#endif  // TMS_SHUTDOWN_ULOG_MESSAGE

}
