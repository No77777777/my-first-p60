#ifndef __GNSS_DIAG_BUF_H__
#define __GNSS_DIAG_BUF_H__

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                GNSS DiagBuf

GENERAL DESCRIPTION
  This header file presents the API to use the GNSS diagBuf functionality. 
  A rich set of GNSS logs are specified at compile time to be stored in circular 
  RAM buffers. These logs are flushed out through system diag when the GNSS 
  software detects an error. After an error is detected, the rich set of logs
  continues to be streamed out of system diag for a desired numnber of 
  milliseconds. 

  So we get an island of rich debug information when we detect an error. 

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2019 Qualcomm Technologies, Inc.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/os_api/inc/gnss_diag_buf.h#2 $
  $DateTime: 2019/12/23 22:17:45 $
  $Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/29/19   sj      Add GNSS Diag Buffer feature

============================================================================*/

/**************************************************************************************
 *                                                                                    *
 *          _____    ______              _____     __  __   ______                    *
 *         |  __ \  |  ____|     /\     |  __ \   |  \/  | |  ____|                   *
 *         | |__) | | |__       /  \    | |  | |  | \  / | | |__                      *
 *         |  _  /  |  __|     / /\ \   | |  | |  | |\/| | |  __|                     *
 *         | | \ \  | |____   / ____ \  | |__| |  | |  | | | |____                    *
 *         | |  \ \ |______| / /    \ \ |_____/   | |  | | |______|                   *
 *                                                                                    *
 * Refer to the README in gnss_diag_buf.c                                             *
 **************************************************************************************/
#include "gps_variation.h"
#include "comdef.h"
#include "custcgps.h"

#include "diagbuffer.h"
#include "msg.h"
#include "log.h"

#include "rex.h"
#include "task.h"

#include "aries_os_api.h"
#include "gps_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * gnss_diag_buf_id_e_type type is an enum that lists IDs for all diag buffers
 * currently supported by the GNSS Diag Buffer module. A new enum value is needed
 * for a new GNSS diag buffer. 
 *
 * The enums need to be binary backwards compatible, and therefore new values 
 * should be added only at the end of the enum type. Enum values should always
 * be incremented by one. 
 */
typedef enum
{
  C_GNSS_DIAG_BUF_ID_FIRST   = 0, /* Do not use */
  C_GNSS_DIAG_BUF_ID_MC_FAST,     /* = 1, MC Fast diag buffer ID */
  C_GNSS_DIAG_BUF_ID_SM,          /* = 2, SM diag buffer ID */
  C_GNSS_DIAG_BUF_ID_CC_FAST,     /* = 3, CC Fast diag buffer ID */
  C_GNSS_DIAG_BUF_ID_CC_SLOW,     /* = 4, CC Slow diag buffer ID */
  C_GNSS_DIAG_BUF_ID_PE_NF,       /* = 5, PE Fast diag buffer ID */
  C_GNSS_DIAG_BUF_ID_PE_CD,       /* = 6, PE Slow diag buffer ID */
  C_GNSS_DIAG_BUF_ID_PE_SENSOR,   /* = 7, PE Slow diag buffer ID */
  C_GNSS_DIAG_BUF_ID_GTS,         /* = 8, GTS diag buffer ID */
  C_GNSS_DIAG_BUF_ID_WWAN_ME,     /* = 9, WWAN diag buffer ID */

  /* Add new diag buffer IDs above this line */

  C_GNSS_DIAG_BUF_ID_LAST         /* Do not use */
} gnss_diag_buf_id_e_type;

/*
 * gnss_diag_buffer_hdl type is an opaque handle provided to gnss diag buffer
 * clients, that the clients use in accessing their diag buffer for all other
 * APIs. There is a 1-to-1 correspondance between gnss_diag_buf_id_e_type
 * and the diag_buffer_hdl provided to clients. 
 */
typedef void* gnss_diag_buffer_hdl;


/* 
 * gnss_diag_buffer: structure to hold the state related to each GNSS diag 
 * buffer. Clients should never modify or access members of this structure.
 */
typedef struct
{  
#define GNSS_DIAG_BUF_NAME_MAX_LEN  24
  /* Diag Buffer handle, provided by DiagBuffer module.
   * This will always be the first member of this structure */
  diagbuffer_handle       hdl;

  /* GNSS Diag Buffer buffer name, 0 terminated */
  int8                    gnss_diag_buf_name[GNSS_DIAG_BUF_NAME_MAX_LEN];

  /* GNSS Diag Buffer ID type used by GNSS diag buffer clients */
  gnss_diag_buf_id_e_type id;
  
  /* Only the DM logs in the list below are put into the circular buffer */
  log_code_type           *p_allowed_logs_list;
  uint16                  allowed_logs_list_sz;

  /* sizeof circular buffer desired for this gnss diag buffer */
  uint32                  circ_buffer_sz;

  /* Pointer to circular buffer to store */
  byte                    *p_circ_buffer;

  os_MutexBlockType       buf_mutex;  
} gnss_diag_buffer;

/*
 *******************************************************************************
 * GNSS_DIAG_BUF_MODULE_BOOTUP                                                 *
 *                                                                             *
 * Function description: This function is called at bootup time to initialize  *
 * the diag buffer module. It allocates all requested buffers, and advertizes  *
 * them with the Diag Buffer module. After calling this function, all buffers  *
 * can be seen in QXDM or any other diag client.                               *
 *                                                                             *
 * Parameters:                                                                 *
 *   None                                                                      *
 * Return value:                                                               *
 *   None                                                                      *
 *                                                                             *
 *******************************************************************************
*/
void gnss_diag_buf_module_bootup(void);


/*
 *******************************************************************************
 * GNSS_DIAG_BUFFER_INIT                                                       *
 *                                                                             *
 * Function description: This function is called at bootup time by different   *
 * gnss diag buffer clients such as ME, PE etc. to initialize their individual *
 * diag buffers.                                                               *
 *                                                                             *
 * It allocates all requested buffers, and initializes them with the Diag Buff *
 * module. If the initialization is successful, a gnss_diag_buffer_hdl is      *
 * returned else NULL is returned.                                             *
 *                                                                             *
 * Parameters:                                                                 *
 *   e_buf_id: Buffer ID being initialized.                                    *
 *                                                                             *
 * Return value:                                                               *
 *   If the initialization is successful, a gnss_diag_buffer_hdl is returned,  *
 *   else NULL is returned                                                     *
 *                                                                             *
 *******************************************************************************
*/                                                                            
gnss_diag_buffer_hdl gnss_diag_buffer_init
(
  gnss_diag_buf_id_e_type e_buf_id
);
                                      
/*                                   
 *******************************************************************************
 * GNSS_DM_LOG_STATUS                                                          *
 *                                                                             *
 * Function description: This function is called to check if a particular log  *
 * needs to be filled out or not, and returns TRUE/FALSE.                      *
 *                                                                             *
 * If the log is turned ON in the DMC, the return value is TRUE.               *
 *                                                                             *
 * If the log needs to be filled and put in a diag buffer, the return value is *
 * TRUE                                                                        *
 * In all other cases, the return value is FALSE.                              *
 *                                                                             *
 * Parameters:                                                                 *
 *   log_code : DM log to check.                                               *
 *   hdl      : Handle to the GNSS diag buffer.                                *
 *                                                                             *
 * Return value:                                                               *
 *   TRUE:  Log needs to be filled out.                                        *
 *   FALSE: Log does not need to be filled out.                                *
 *                                                                             *
 *******************************************************************************
*/
#ifdef FEATURE_USE_GNSS_DIAG_BUF
boolean gnss_dm_log_status
(
  log_code_type        log_code, 
  gnss_diag_buffer_hdl hdl
);
#else

#define gnss_dm_log_status( code, handle ) log_status(code)

#endif

/*
 *******************************************************************************
 * GNSS_DIAG_BUFFER_LOG_ALLOC_EXT                                              *
 *                                                                             *
 * Function description: This function is called to allocate memory for a log  *
 * packet, to replace log_alloc_ext()                                          *
 *                                                                             *
 * This function devolves to log_alloc_ext API call in the following cases:    *
 *  1. If the feature is not enabled                                           *
 *  2. If the passed gnss_diag_buffer_hdl is NULL.                             *
 *  3. If the log_code is turned ON in the DMC.                                *
 *  4. If the diag_buffer module is not initialized                            *
 *  5. If the log_code is not in the allowed logs list for the passed gnss     *
 *     diag buffer handle.                                                     *
 *                                                                             *
 * Parameters:                                                                 *
 *   hdl         : Handle to the GNSS diag buffer.                             *
 *   log_code    : DM log to check.                                            *
 *   q_length    : length of the log in bytes                                  *
 *   q_version   : Version passed to log_alloc_ext() API.                      *
 *                                                                             *
 *                 Specifies the version - currently, there are two versions   *
 *                 0 - same as log_alloc() (for backwards compatibility)       *
 *                 1 - The outgoing log packet header will have support to     *
 *                     include ID field to differentiate the log pkts          *
 *                     coming from different instances of the processor.       *
 *                                                                             *
 *   p_parameter : Pointer is passed to log_alloc_ext() API.                   *
 *                                                                             *
 *                 Based on the version passed in, this pointer is interpreted *
 *                 as follows-                                                 *
 *                                                                             *
 *                 Version   pointer                                           *
 *                 0         NULL                                              *
 *                 1         uint32 *  The uint32 value this pointer points    *
 *                                     to is the ID of the instance generating *
 *                                     the log packet                          *
 *                                                                             *
 * Return value:                                                               *
 *   pointer to allocated log. Null of log could not be allocated.             *
 *******************************************************************************
*/
#ifdef FEATURE_USE_GNSS_DIAG_BUF
void* gnss_diag_buffer_log_alloc_ext
(
  gnss_diag_buffer_hdl hdl, 
  log_code_type        log_code,
  uint32               q_length,
  uint32               q_version, 
  void*                p_parameter
);
#else

#define gnss_diag_buffer_log_alloc_ext(                             \
                       hdl,                                         \
                       log_code,                                    \
                       q_length,                                    \
                       q_version,                                   \
                       p_parameter)                                 \
        log_alloc_ext(log_code, q_length, q_version, p_parameter)

#endif


/*
 *******************************************************************************
 * GNSS_DIAG_BUFFER_LOG_ALLOC                                                  *
 *                                                                             *
 * Function description: This function is called to allocate memory for a log  *
 * packet, to replace log_alloc()                                              *
 *                                                                             *
 * This function devolves to log_alloc API call in the following cases:        *
 *  1. If the feature is not enabled                                           *
 *  2. If the passed gnss_diag_buffer_hdl is NULL.                             *
 *  3. If the log_code is turned ON in the DMC.                                *
 *  4. If the diag_buffer module is not initialized                            *
 *  5. If the log_code is not in the allowed logs list for the passed gnss     *
 *     diag buffer handle.                                                     *
 *                                                                             *
 * Parameters:                                                                 *
 *   hdl         : Handle to the GNSS diag buffer.                             *
 *   log_code    : DM log to check.                                            *
 *   q_length    : length of the log in bytes                                  *
 *                                                                             *
 * Return value:                                                               *
 *   pointer to allocated log. Null of log could not be allocated.             *
 *******************************************************************************
*/
#ifdef FEATURE_USE_GNSS_DIAG_BUF
void* gnss_diag_buffer_log_alloc
(
  gnss_diag_buffer_hdl hdl,
  log_code_type        log_code,
  uint32               q_length
);
#else

#define gnss_diag_buffer_log_alloc(                              \
                       hdl,                                      \
                       log_code,                                 \
                       q_length)                                 \
        log_alloc(log_code, q_length)

#endif

/*
 *******************************************************************************
 * GNSS_DM_LOG_COMMIT                                                          *
 *                                                                             *
 * Function description: This function is called to commit any allocated log   * 
 * to the diag buffer desired by the client. The log is committed to the       *
 * buffer only if                                                              *
 *                                                                             *
 *  1. If the passed gnss_diag_buffer_hdl is not NULL.                         *
 *  3. If the log_code is not turned ON in the DMC.                            *
 *  4. If the diag_buffer module is initialized                                *
 *  5. If the log_code is in the allowed logs list for the passed gnss         *
 *     diag buffer handle.                                                     *
 *                                                                             *
 * If any of these conditions is not true, the log is streamed out system diag *
 * and not buffered in the diag buffer.                                        *
 *                                                                             *
 * Parameters:                                                                 *
 *   hdl         : Handle to the GNSS diag buffer.                             *
 *   log_code    : DM log to check.                                            *
 *   p_log       : Pointer to the log                                          *
 *                                                                             *
 * Return value:                                                               *
 *******************************************************************************
*/
#ifdef FEATURE_USE_GNSS_DIAG_BUF
void gnss_dm_log_commit
(
  gnss_diag_buffer_hdl hdl, 
  log_code_type        log_code,
  void                *p_log
);

#else
#define gnss_dm_log_commit(hdl, log_code, p_log) log_commit(p_log)
#endif

/*
 *******************************************************************************
 * GNSS_DM_LOG_FLUSH                                                           *
 *                                                                             *
 * Function description: This function is called to flush all buffered logs to * 
 * the system diag buffer, and set the buffering mode to streaming. I.e. logs  *
 * are flushed out as they are generated.                                      *
 *                                                                             *
 * The client should call this function when it detects an error in its        *
 * operation. The buffering policy continues to be streaming for reset_time_ms *
 * milliseconds after which it resets to circular buffering.                   *
 *                                                                             *
 * The flush applies to all diagBuffers for GNSS.                              *
 *                                                                             *
 * Parameters:                                                                 *
 *   hdl           : Handle to the GNSS diag buffer.                           *
 *   reset_time_ms : Time in milli seconds for which logs should be streamed   *
 *                   out of the diag buffer. DM log to check.                  *
 *                                                                             *
 * Return value:                                                               *
 *   None                                                                      *
 *******************************************************************************
*/
void gnss_dm_log_flush
( 
  gnss_diag_buffer_hdl hdl, 
  uint16               reset_time_ms
);

/*
 *******************************************************************************
 * GNSS_DIAG_BUFFER_HANDLE_DIAG_COMMAND                                        *
 *                                                                             *
 * Function description: This function is called to by the GNSS diag task to   *
 * handle any diag commands related to DiagBuffer functionality.               * 
 *                                                                             *
 * Parameters:                                                                 *
 *                                                                             *
 * Return value:                                                               *
 *******************************************************************************
*/
#define GNSS_DIAG_DIAG_BUFFER_FLUSH_CMD           1

boolean gnss_diag_buffer_handle_diag_command
(
  uint32             q_GNSSDiagBufferTestCmd, /* GNSS Diag buffer test command */
  uint32             q_GNSSDiagBufferId       /* GNSS Diag buffer ID for the test command */
);

/*
 *******************************************************************************
 * TM_DIAG_BUF_TIMER_DISPATCH                                                  *
 *                                                                             *
 * Function description: This function is called to dispatch any timers        *
 * related to the correct functioning of GNSS diag buffers.                    *
 *                                                                             *
 * Parameters:                                                                 *
 *   timer_id    : Unique timer ID.                                            *
 *                                                                             *
 * Return value:                                                               *
 *******************************************************************************
*/
void tm_diag_buf_timer_dispatch(uint32 timer_id);

/*
 *******************************************************************************
 * GNSS_DIAG_BUF_FEATURE_ENABLED                                               *
 *                                                                             *
 * Function description: This function is called to find if the feature is     *
 * enabled and initialized.                                                    *
 * related to the correct functioning of GNSS diag buffers.                    *
 *                                                                             *
 * Parameters:                                                                 *
 *                                                                             *
 * Return value:                                                               *
 *******************************************************************************
*/
boolean gnss_diag_buf_feature_enabled(void);

/*
 *******************************************************************************
 * GNSS_DIAG_BUF_CHECK_STATUS                                                  *
 *                                                                             *
 * Function description: This function is a debug only function, that can be   *
 * called to check the status of a diag buffer, if that buffer has been        *
 * enabled to drain in the diag client, qxdm or odl diag client.               *
 *                                                                             *
 * This is a debug only API and clients are not to exercise it in mission mode *
 *                                                                             *
 * Parameters:                                                                 *
 *   hdl: handle to the diag buffer to be checked.                             *
 * Return value:                                                               *
 *   0  : If buffer is not allocated.                                          *
 *      : size of buffer if allocated.                                         *
 *                                                                             *
 *******************************************************************************
*/
uint32 gnss_diag_buf_check_status(gnss_diag_buffer_hdl hdl);

/*
 *******************************************************************************
 * The macro GNSS_DIAGBUFFER_MSG is a generic macro that can replace all       *
 * MSG_1/2/3 etc. macros to insert the log in the selected Diag Buffer         *
 *                                                                             *
 * xx_gnss_diag_buf_hdl : handle returned by gnss_diag_buffer_init             *
 * xx_ss_id             : use SSID in enum below gnss_diag_buf_ssid            * 
 * xx_ss_mask           : Low / Medium / High etc.                             *
 * xx_fmt               : format string                                        *
 *                                                                             *
 * The msg goes to the system diag buffer if the provided handle is NULL, or   *
 * if the specific diag buffer is not intialized yet or if the F3 is enabled   *
 * and being logged by system diag                                             *
 *                                                                             *
 *******************************************************************************
 */

typedef enum
{
  SSID_GPSSM,
  SSID_GNSS_LOCMW,
  SSID_MGPPE,
  SSID_MGPME
} gnss_diag_buf_ssid;

#if defined(T_WINNT)
#error code not present
#else

#define GNSS_DIAGBUFFER_MSG(xx_gnss_diag_buf_hdl, xx_ss_id, xx_ss_mask, xx_fmt, ...)        \
do                                                                                          \
{                                                                                           \
    if((xx_gnss_diag_buf_hdl == SYSDIAG_HANDLE) ||                                          \
       (msg_status(MSG_ ## xx_ss_id, xx_ss_mask)))                                          \
    {                                                                                       \
      DIAGBUFFER_MSG(SYSDIAG_HANDLE, MSG_ ## xx_ss_id, xx_ss_mask, xx_fmt, ##__VA_ARGS__);  \
      break;                                                                                \
    }                                                                                       \
                                                                                            \
    os_MutexLock(&(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->buf_mutex));                 \
                                                                                            \
    if(((NULL == ((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->p_circ_buffer) ||              \
       (TRUE != gnss_diag_buf_feature_enabled())))                                          \
    {                                                                                       \
      os_MutexUnlock(&(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->buf_mutex));             \
                                                                                            \
      DIAGBUFFER_MSG(SYSDIAG_HANDLE, MSG_ ## xx_ss_id, xx_ss_mask, xx_fmt, ##__VA_ARGS__);  \
      break;                                                                                \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
      os_MutexUnlock(&(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->buf_mutex));             \
                                                                                            \
      DIAGBUFFER_MSG(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->hdl,                       \
                    MSG_ ## xx_ss_id, xx_ss_mask, xx_fmt, ##__VA_ARGS__);                   \
      break;                                                                                \
    }                                                                                       \
} while(0);


/*
 *******************************************************************************
 * The macro GNSS_DIAGBUFFER_MSG_SPRINTF is a macro that can add 1 or more     *
 * strings to selected diag buffer.                                            *
 *                                                                             *
 * xx_gnss_diag_buf_hdl : handle returned by gnss_diag_buffer_init             *
 * xx_ss_id             : use SSID in enum below gnss_diag_buf_ssid            * 
 * xx_ss_mask           : Low / Medium / High etc.                             *
 * xx_fmt               : format string                                        *
 *                                                                             *
 * The msg goes to the system diag buffer if the provided handle is NULL, or   *
 * if the specific diag buffer is not intialized yet or if the F3 is enabled   *
 * and being logged by system diag                                             *
 *                                                                             *
 *******************************************************************************
 */
#define GNSS_DIAGBUFFER_MSG_SPRINTF(xx_gnss_diag_buf_hdl, xx_ss_id, xx_ss_mask, xx_fmt, ...)          \
do                                                                                                    \
{                                                                                                     \
    if((xx_gnss_diag_buf_hdl == SYSDIAG_HANDLE) ||                                                    \
       (msg_status(MSG_ ## xx_ss_id, xx_ss_mask)))                                                    \
    {                                                                                                 \
      DIAGBUFFER_MSG_SPRINTF_N(SYSDIAG_HANDLE, MSG_ ## xx_ss_id, xx_ss_mask, xx_fmt, ##__VA_ARGS__);  \
      break;                                                                                          \
    }                                                                                                 \
                                                                                                      \
    os_MutexLock(&(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->buf_mutex));                           \
                                                                                                      \
    if(((NULL == ((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->p_circ_buffer) ||                        \
       (TRUE != gnss_diag_buf_feature_enabled())))                                                    \
    {                                                                                                 \
      os_MutexUnlock(&(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->buf_mutex));                       \
                                                                                                      \
      DIAGBUFFER_MSG_SPRINTF_N(SYSDIAG_HANDLE, MSG_ ## xx_ss_id, xx_ss_mask, xx_fmt, ##__VA_ARGS__);  \
      break;                                                                                          \
    }                                                                                                 \
    else                                                                                              \
    {                                                                                                 \
      os_MutexUnlock(&(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->buf_mutex));                       \
                                                                                                      \
      DIAGBUFFER_MSG_SPRINTF_N(((gnss_diag_buffer*) xx_gnss_diag_buf_hdl)->hdl,                       \
                    MSG_ ## xx_ss_id, xx_ss_mask, xx_fmt, ##__VA_ARGS__);                             \
      break;                                                                                          \
    }                                                                                                 \
} while(0);
	

#endif

#ifdef __cplusplus
}
#endif

#endif /* __GNSS_DIAG_BUF_H__ */

