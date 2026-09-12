#ifndef TMS_UTILS_MSG_H
#define TMS_UTILS_MSG_H
/**
@file tms_utils_msg.h
@brief This file contains the Macro definitions for TMS utils
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2016-2020 QUALCOMM Technologies Incorporated.
All rights reserved.
QUALCOMM Confidential and Proprietary.
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/api/tms_utils_msg.h#4 $
$DateTime: 2020/04/28 05:22:08 $
$Change: 23608255 $
$Author: pwbldsvc $
===============================================================================*/

#include "qurt.h"
#include "tms_utils.h"
#include "msg.h"
#include "ULog.h"
#include "ULogFront.h"

/**
 * Log enable/disable macro
 */
#define TMS_MSG_DIAG_F3     // Enable this for Diag F3
#define TMS_ULOG_MESSAGE // Enable this for Common ULOGs
#define TMS_STARTUP_ULOG_MESSAGE // Enable this for Start-up ULOGs
#define TMS_SHUTDOWN_ULOG_MESSAGE // Enable this for Shutdown ULOGs
//#define TMS_ULOG_LOW_ENABLED // Enable this for Low level ULOGs

#ifdef TMS_PERIPHERAL_MPSS
#define TMS_PERIPHERAL "MPSS:"
#elif TMS_PERIPHERAL_ADSP
#define TMS_PERIPHERAL "ADSP:"
#elif TMS_PERIPHERAL_SLPI
#define TMS_PERIPHERAL "SLPI:"
#elif TMS_PERIPHERAL_CDSP
#define TMS_PERIPHERAL "CDSP:"
#else
#define TMS_PERIPHERAL "Q6:"
#endif 

#define TMS_ERR_PREFIX "ERR:"

typedef struct {
  /* F3 Flag */
  unsigned int diag_f3_flag;
  
  /* ULOG level Flag */
  unsigned int ulog_low_enabled; 
  
  int tls_key;
  
  /* ULOG handles */
  ULogHandle ulog_handle; 
  ULogHandle startup_ulog_handle;
  ULogHandle shutdown_ulog_handle;
} utils_log_t;

extern utils_log_t utils_log;

/**
 * @brief tms_utils_msg_init
 *
 * Initialize ULOG handles.
*/
void tms_utils_msg_init( void );

/*
Not using: MSG_LEGACY_MED, MSG_LEGACY_FATAL messages
*/
  
/*------ LOW ------*/
#define TMS_MSG_LOW( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 2, "%s:%d:" str, __FILENAME__, __LINE__);  \
    }  \
  } while(0)
      
#define TMS_MSG_LOW_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 3, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)

#define TMS_MSG_LOW_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)

#define TMS_MSG_LOW_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 5, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)

#define TMS_MSG_LOW_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a), ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)
	  
#define TMS_MSG_LOW_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 6, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a), ULOG64_HIGHWORD(a),ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)

#define TMS_MSG_LOW_3_64( str, a, b, c)  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c);  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 8, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a), ULOG64_HIGHWORD(a),ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c),ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)


/*------ HIGH ------*/
#define TMS_MSG_HIGH( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str);  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 2, "%s:%d:" str, __FILENAME__, __LINE__);  \
    }  \
  } while(0)

#define TMS_MSG_HIGH_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 3, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)
  
#define TMS_MSG_HIGH_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)
  
#define TMS_MSG_HIGH_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 5, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)    
  

#define TMS_MSG_HIGH_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)

#define TMS_MSG_HIGH_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 6, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)
	  
#define TMS_MSG_HIGH_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 8, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b),ULOG64_LOWWORD(c), ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)

/*------ ERROR ------*/
#define TMS_MSG_ERROR( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 2, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__);  \
    } \
  } while(0)

#define TMS_MSG_ERROR_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 3, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)  

#define TMS_MSG_ERROR_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 4, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)  
  
#define TMS_MSG_ERROR_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 5, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)

#define TMS_MSG_ERROR_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 4, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)
	  
#define TMS_MSG_ERROR_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 6, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)
	  
#define TMS_MSG_ERROR_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.ulog_handle, 8, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a),ULOG64_LOWWORD(b), ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c),ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)


/*------ SPRINTF_LOW ------*/
#define TMS_MSG_SPRINTF_LOW_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_LOW_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_MSG_SPRINTF_LOW_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_MSG_SPRINTF_LOW_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_LOW_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

/*------ SPRINTF_HIGH ------*/
#define TMS_MSG_SPRINTF_HIGH_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_HIGH_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_HIGH_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_HIGH_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_HIGH_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    } \
  } while(0)

/*------ SPRINTF_ERROR ------*/
#define TMS_MSG_SPRINTF_ERROR_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_ERROR_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_MSG_SPRINTF_ERROR_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_MSG_SPRINTF_ERROR_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_MSG_SPRINTF_ERROR_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

/*------ STARTUP ------*/

/*------ STARTUP LOW ------*/
#define TMS_STARTUP_MSG_LOW( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 2, "%s:%d:" str, __FILENAME__, __LINE__);  \
    }  \
  } while(0)
      
#define TMS_STARTUP_MSG_LOW_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 3, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_LOW_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_LOW_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 5, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_LOW_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)
	  
#define TMS_STARTUP_MSG_LOW_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 6, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_LOW_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 8, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a), ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b), ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c), ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)


/*------ STARTUP HIGH ------*/
#define TMS_STARTUP_MSG_HIGH( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str);  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 2, "%s:%d:" str, __FILENAME__, __LINE__);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_HIGH_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 3, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)
  
#define TMS_STARTUP_MSG_HIGH_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)
  
#define TMS_STARTUP_MSG_HIGH_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 5, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_HIGH_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)
  
#define TMS_STARTUP_MSG_HIGH_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 6, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b), ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_HIGH_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 8, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c),ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)



/*------ STARTUP ERROR ------*/
#define TMS_STARTUP_MSG_ERROR( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 2, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__);  \
    } \
  } while(0)

#define TMS_STARTUP_MSG_ERROR_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 3, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)  

#define TMS_STARTUP_MSG_ERROR_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 4, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)  
  
#define TMS_STARTUP_MSG_ERROR_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 5, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_ERROR_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 4, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0) 

#define TMS_STARTUP_MSG_ERROR_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 6, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0) 
	  
#define TMS_STARTUP_MSG_ERROR_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.startup_ulog_handle, 8, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c),ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)



/*------ STARTUP SPRINTF_LOW ------*/
#define TMS_STARTUP_MSG_SPRINTF_LOW_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_LOW_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_STARTUP_MSG_SPRINTF_LOW_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_STARTUP_MSG_SPRINTF_LOW_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_LOW_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.startup_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

/*------ STARTUP SPRINTF_HIGH ------*/
#define TMS_STARTUP_MSG_SPRINTF_HIGH_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_HIGH_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_HIGH_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_HIGH_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_HIGH_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    } \
  } while(0)

/*------ STARTUP SPRINTF_ERROR ------*/
#define TMS_STARTUP_MSG_SPRINTF_ERROR_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_ERROR_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_STARTUP_MSG_SPRINTF_ERROR_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_STARTUP_MSG_SPRINTF_ERROR_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_STARTUP_MSG_SPRINTF_ERROR_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.startup_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.startup_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
    
/*------ SHUTDOWN ------*/

/*------ SHUTDOWN LOW ------*/
#define TMS_SHUTDOWN_MSG_LOW( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 2, "%s:%d:" str, __FILENAME__, __LINE__);  \
    }  \
  } while(0)
      
#define TMS_SHUTDOWN_MSG_LOW_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 3, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_LOW_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_LOW_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 5, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_LOW_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)
	  
#define TMS_SHUTDOWN_MSG_LOW_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 6, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)
	  
#define TMS_SHUTDOWN_MSG_LOW_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 8, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c),ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)

  
/*------ SHUTDOWN HIGH ------*/
#define TMS_SHUTDOWN_MSG_HIGH( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str);  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 2, "%s:%d:" str, __FILENAME__, __LINE__);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_HIGH_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 3, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)
  
#define TMS_SHUTDOWN_MSG_HIGH_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)
  
#define TMS_SHUTDOWN_MSG_HIGH_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 5, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)    
  
#define TMS_SHUTDOWN_MSG_HIGH_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 4, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)
  
#define TMS_SHUTDOWN_MSG_HIGH_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 6, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)
	  
#define TMS_SHUTDOWN_MSG_HIGH_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 8, "%s:%d:" str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c),ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)

/*------ SHUTDOWN ERROR ------*/
#define TMS_SHUTDOWN_MSG_ERROR( str )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 2, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__);  \
    } \
  } while(0)

#define TMS_SHUTDOWN_MSG_ERROR_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 3, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a);  \
    }  \
  } while(0)  

#define TMS_SHUTDOWN_MSG_ERROR_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 4, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b);  \
    }  \
  } while(0)  
  
#define TMS_SHUTDOWN_MSG_ERROR_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 5, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_ERROR_1_64( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 4, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a));  \
    }  \
  } while(0)  

#define TMS_SHUTDOWN_MSG_ERROR_2_64( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 6, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b));  \
    }  \
  } while(0)
	  
#define TMS_SHUTDOWN_MSG_ERROR_3_64( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      ULogFront_RealTimePrintf(utils_log.shutdown_ulog_handle, 8, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, ULOG64_LOWWORD(a),ULOG64_HIGHWORD(a), ULOG64_LOWWORD(b),ULOG64_HIGHWORD(b), ULOG64_LOWWORD(c),ULOG64_HIGHWORD(c));  \
    }  \
  } while(0)

/*------ SHUTDOWN SPRINTF_LOW ------*/
#define TMS_SHUTDOWN_MSG_SPRINTF_LOW_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_LOW_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_SHUTDOWN_MSG_SPRINTF_LOW_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_SHUTDOWN_MSG_SPRINTF_LOW_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_LOW_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_LOW, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.shutdown_ulog_handle && utils_log.ulog_low_enabled )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

/*------ SHUTDOWN SPRINTF_HIGH ------*/
#define TMS_SHUTDOWN_MSG_SPRINTF_HIGH_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_HIGH_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_HIGH_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_HIGH_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_HIGH_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_HIGH, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    } \
  } while(0)

/*------ SHUTDOWN SPRINTF_ERROR ------*/
#define TMS_SHUTDOWN_MSG_SPRINTF_ERROR_1( str, a )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_1( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_ERROR_2( str, a, b )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_2( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)

#define TMS_SHUTDOWN_MSG_SPRINTF_ERROR_3( str, a, b, c )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_3( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_SHUTDOWN_MSG_SPRINTF_ERROR_4( str, a, b, c, d )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_4( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c, d );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c, d);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#define TMS_SHUTDOWN_MSG_SPRINTF_ERROR_5( str, a, b, c, d, e )  \
  do {  \
    if( utils_log.diag_f3_flag )  \
    {  \
      MSG_SPRINTF_5( MSG_SSID_TMS, MSG_LEGACY_ERROR, TMS_PERIPHERAL str, a, b, c, d, e );  \
    }  \
    if( utils_log.shutdown_ulog_handle )  \
    {  \
      void *tms_bufptr = qurt_tls_get_specific(utils_log.tls_key); \
      if (NULL == tms_bufptr) \
      { \
         tms_bufptr = malloc(TMS_UTILS_BUFFER_SIZE_EXT); \
         if (NULL != tms_bufptr) \
         {  \
           if(QURT_EOK != qurt_tls_set_specific(utils_log.tls_key, tms_bufptr)) {\
              free(tms_bufptr); \
              break; }\
         }  \
         else \
           break; \
      } \
      memset(tms_bufptr, 0, TMS_UTILS_BUFFER_SIZE_EXT); \
      (void)tms_utils_fmt(tms_bufptr, TMS_UTILS_BUFFER_SIZE_EXT, "%s:%d:" TMS_ERR_PREFIX str, __FILENAME__, __LINE__, a, b, c, d, e);  \
      ULogFront_RealTimeString(utils_log.shutdown_ulog_handle, tms_bufptr);  \
    }  \
  } while(0)
  
#endif /* TMS_UTILS_MSG_H */