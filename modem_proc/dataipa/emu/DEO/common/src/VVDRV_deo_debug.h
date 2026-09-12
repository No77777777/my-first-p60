#ifndef __VVDRV_DEO_DEBUG_H__
#define __VVDRV_DEO_DEBUG_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_debug.h
 *
 * @brief declares the DEO debug defines
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_os.h"     /* deo_os_printf */

/************************************************************************/
/*                      CONSTANTS & MACROS                              */
/************************************************************************/

/* Define the default debug level */
#ifndef DEO_DEBUG_LEVEL
#define DEO_DEBUG
#define DEO_DEBUG_LEVEL DEO_DS_WARNING
#endif /* !DEO_DEBUG_LEVEL */

#ifdef DEO_DEBUG
/**
 * @def DEO_LOG_MSG(severity, type, message) deo_os_printf message
 *
 * @brief           prints the log message
 *
 * @param           severity    the severity of the log message
 * @param           type        the log point of the message
 * @param           message     the message
 *
 * @note            the type of the log message can be used to define
 *                  different verbosity to different modules in the project
 *
 * @see             dea_debug_init()
 */
#ifndef DEO_LOG_MSG
#define DEO_LOG_MSG(severity, type, message) deo_os_printf message
#endif /* !DEO_LOG_MSG */

/**
 * @def DEO_ASSERT(expr,message)
 *
 * @brief           assertion   macro
 *
 * @param           expr        the expression that should be evaluated
 * @param           message     the log message to be printed in case the expression
 *                              is not true
 */
#ifndef DEO_ASSERT
#define DEO_ASSERT(expr,message) \
  if (!(expr)) \
  { \
    /* should never reach here. this is only to resolve compilation warning */ \
    /* about the message argument */ \
    deo_os_printf message; \
    for (;;) {} \
  }
#endif /* !DEO_ASSERT */

#else /* !DEO_DEBUG */
#ifndef DEO_LOG_MSG
#define DEO_LOG_MSG(severity, type, message) do {} while ((void)0,0)
#endif /* !DEO_LOG_MSG */    
#ifndef DEO_ASSERT
#define DEO_ASSERT(expr,message) do {} while ((void)0,0)
#endif /* !DEO_ASSERT */
#endif /* DEO_DEBUG */

/* The following defines declares the severity levels */
#define  DEO_DS_FATAL    1
#define  DEO_DS_ERROR    2
#define  DEO_DS_WARNING  3
#define  DEO_DS_INFO     4
#define  DEO_DS_DEBUG    5

/* For compatibility reasons we define here debug macros */
/* The debug level can change between 0 (no debug messages) to 5 (verbose debug level) */
#if (DEO_DEBUG_LEVEL > 0)
#define DEO_DBG_F(type, message) DEO_LOG_MSG(DEO_DS_FATAL, type, message)
#else
#define DEO_DBG_F(type, message) do { } while ((void)0,0)
#endif /* DEO_DEBUG_LEVEL > 0 */

#if (DEO_DEBUG_LEVEL > 1)
#define DEO_DBG_E(type, message) DEO_LOG_MSG(DEO_DS_ERROR, type, message)
#else
#define DEO_DBG_E(type, message) do { } while ((void)0,0)
#endif /* DEO_DEBUG_LEVEL > 1 */

#if (DEO_DEBUG_LEVEL > 2)
#define DEO_DBG_W(type, message) DEO_LOG_MSG(DEO_DS_WARNING, type, message)
#else
#define DEO_DBG_W(type, message) do { } while ((void)0,0)
#endif /* DEO_DEBUG_LEVEL > 2 */

#if (DEO_DEBUG_LEVEL > 3)
#define DEO_DBG_I(type, message) DEO_LOG_MSG(DEO_DS_INFO, type, message)
#else
#define DEO_DBG_I(type, message) do { } while ((void)0,0)
#endif /* DEO_DEBUG_LEVEL > 3 */

#if (DEO_DEBUG_LEVEL > 4)
#define DEO_DBG_V(type, message) DEO_LOG_MSG(DEO_DS_DEBUG, type, message)
#define DEO_DBG_X(type, message) DEO_LOG_MSG(DEO_DS_DEBUG, type, message)
#else
#define DEO_DBG_V(type, message) do { } while ((void)0,0)
#define DEO_DBG_X(type, message) do { } while ((void)0,0)
#endif /* DEO_DEBUG_LEVEL > 4 */

#endif  /* __VVDRV_DEO_DEBUG_H__ */
