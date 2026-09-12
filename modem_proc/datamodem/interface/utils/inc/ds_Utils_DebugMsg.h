#ifndef DS_UTILS_DEBUG_MSG_H
#define DS_UTILS_DEBUG_MSG_H
/*===========================================================================
  @file ds_Utils_DebugMsg.h

  This header file defines macros which are used to log debug messages to the
  console. These macros translate to DIAG macros.

  DIAG team's MSG 2.0 API is used to send messages to DIAG.

  Copyright (c) 2008 - 2017 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/inc/ds_Utils_DebugMsg.h#4 $
  $DateTime: 2022/12/12 22:30:14 $ $Author: pwbldsvc $

===========================================================================*/
/*===========================================================================
 
                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#include "msg.h"
#include "err.h"
#include "amssassert.h"
#include "ps_utils_qtrace.h"
#include "ps_sys.h"

extern ULogHandle                   ps_fwk_ulog_hable;

/*===========================================================================

                          MACRO DECLARATIONS

===========================================================================*/
/*--------------------------------------------------------------------------
ERR_FATAL messages
---------------------------------------------------------------------------*/
#define DATA_ERR_FATAL(xx_exp) ERR_FATAL("Assertion " #xx_exp " failed", 0, 0, 0 );


      /*---------------------------------------------------------------------
        Message macros are mapped to DIAG msg levels
      ---------------------------------------------------------------------*/
      #define LOG_MSG_FUNCTION_ENTRY_LEVEL  MSG_LEGACY_MED
      #define LOG_MSG_FUNCTION_EXIT_LEVEL   MSG_LEGACY_MED
      #define LOG_MSG_INFO1_LEVEL           MSG_LEGACY_MED
      #define LOG_MSG_INFO2_LEVEL           MSG_LEGACY_MED
      #define LOG_MSG_INFO3_LEVEL           MSG_LEGACY_LOW
      #define LOG_MSG_INFO4_LEVEL           MSG_LEGACY_LOW
      #define LOG_MSG_STATE_CHANGE_LEVEL    MSG_LEGACY_HIGH
      #define LOG_MSG_INVALID_INPUT_LEVEL   MSG_LEGACY_HIGH
      #define LOG_MSG_ERROR_LEVEL           MSG_LEGACY_ERROR
      #define LOG_MSG_FATAL_ERROR_LEVEL     MSG_LEGACY_FATAL

      /*---------------------------------------------------------------------
        Use different SSID for APPS vs MODEM DS messages.
      ---------------------------------------------------------------------*/
      #define PRINT_MSG( level, fmtString, x, y, z)                         \
        MSG_3( MSG_SSID_DS, level, fmtString, x, y, z);

      #define PRINT_MSG_6( level, fmtString, a, b, c, d, e, f)              \
        MSG_6( MSG_SSID_DS, level, fmtString, a, b, c, d, e, f);

      #define PRINT_MSG_0( level, fmtString)                                \
        MSG( MSG_SSID_DS, level, fmtString);

      #define PRINT_MSG_1( level, fmtString, x)                             \
        MSG_1( MSG_SSID_DS, level, fmtString, x);

      #define PRINT_MSG_2( level, fmtString, x, y)                          \
        MSG_2( MSG_SSID_DS, level, fmtString, x, y);

      #define PRINT_MSG_3( level, fmtString, x, y, z)                       \
        MSG_3( MSG_SSID_DS, level, fmtString, x, y, z);

      #define PRINT_MSG_4( level, fmtString, a, b, c, d)                    \
        MSG_4( MSG_SSID_DS, level, fmtString, a, b, c, d);

      #define PRINT_MSG_5( level, fmtString, a, b, c, d, e)                 \
        MSG_5( MSG_SSID_DS, level, fmtString, a, b, c, d, e);

     #define PRINT_MSG_7( level, fmtString, a, b, c, d, e, f, g)             \
        MSG_7( MSG_SSID_DS, level, fmtString, a, b, c, d, e, f, g);

      #define PRINT_MSG_8( level, fmtString, a, b, c, d, e, f, g, h)          \
        MSG_8( MSG_SSID_DS, level, fmtString, a, b, c, d, e, f, g, h);

      #define PRINT_MSG_9( level, fmtString, a, b, c, d, e, f, g, h, i)       \
        MSG_9( MSG_SSID_DS, level, fmtString, a, b, c, d, e, f, g, h, i);

      #define PRINT_MSG_STR_1( level, fmtString, x)                           \
        MSG_SPRINTF_1( MSG_SSID_DS, level, fmtString, x);


      /**
        This macro is used to log a message, when ever a function is invoked.
        Important input parameters are logged as well.
      */
      #define LOG_MSG_FUNCTION_ENTRY( fmtString, x, y, z)                   \
      {                                                                     \
        PRINT_MSG( LOG_MSG_FUNCTION_ENTRY_LEVEL, fmtString, x, y, z);       \
      }
      #define LOG_MSG_FUNCTION_ENTRY_3( fmtString, x, y, z)                   \
      {                                                                     \
        PRINT_MSG( LOG_MSG_FUNCTION_ENTRY_LEVEL, fmtString, x, y, z);       \
      }

      #define LOG_MSG_FUNCTION_ENTRY_0( fmtString)                          \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_FUNCTION_ENTRY_LEVEL, fmtString);              \
      }

      #define LOG_MSG_FUNCTION_ENTRY_1( fmtString, x)                       \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_FUNCTION_ENTRY_LEVEL, fmtString, x);           \
      }

      #define LOG_MSG_FUNCTION_ENTRY_2( fmtString, x, y)                    \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_FUNCTION_ENTRY_LEVEL, fmtString, x, y);        \
      }

      #define LOG_MSG_FUNCTION_ENTRY_4( fmtString, a, b, c, d)              \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_FUNCTION_ENTRY_LEVEL, fmtString, a, b, c, d);  \
      }

      #define LOG_MSG_FUNCTION_ENTRY_5( fmtString, a, b, c, d, e)           \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_FUNCTION_ENTRY_LEVEL,                          \
                     fmtString, a, b, c, d, e);                             \
      }

      #define LOG_MSG_FUNCTION_ENTRY_6( fmtString, a, b, c, d, e, f)        \
      {                                                                     \
        PRINT_MSG_6( LOG_MSG_FUNCTION_ENTRY_LEVEL, fmtString,               \
                     a, b, c, d, e, f);                                     \
      }

      /**
        This macro is used to log a message, when ever a function returns.
        Return value and output parameters are logged as well.
      */
      #define LOG_MSG_FUNCTION_EXIT( fmtString, x, y, z)                    \
      {                                                                     \
        PRINT_MSG( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString, x, y, z);        \
      }

      #define LOG_MSG_FUNCTION_EXIT_3( fmtString, x, y, z)                    \
      {                                                                     \
        PRINT_MSG( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString, x, y, z);        \
      }
      #define LOG_MSG_FUNCTION_EXIT_0( fmtString)                           \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString);               \
      }

      #define LOG_MSG_FUNCTION_EXIT_1( fmtString, x)                        \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString, x);            \
      }

      #define LOG_MSG_FUNCTION_EXIT_2( fmtString, x, y)                     \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString, x, y);         \
      }

      #define LOG_MSG_FUNCTION_EXIT_4( fmtString, a, b, c, d)               \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString, a, b, c, d);   \
      }

      #define LOG_MSG_FUNCTION_EXIT_5( fmtString, a, b, c, d, e)            \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString, a, b, c, d, e);\
      }

      #define LOG_MSG_FUNCTION_EXIT_6( fmtString, a, b, c, d, e, f)         \
      {                                                                     \
        PRINT_MSG_6( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString,                \
                     a, b, c, d, e, f);                                     \
      }

      #define LOG_MSG_FUNCTION_EXIT_8( fmtString, a, b, c, d, e, f, g, h)   \
      {                                                                     \
        PRINT_MSG_8( LOG_MSG_FUNCTION_EXIT_LEVEL, fmtString,                \
                     a, b, c, d, e, f, g, h);                               \
      }

      /**
        This macro is used to log an informational message.
        For example "Routing look up returned iface" etc.
      */
      #define LOG_MSG_INFO1( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO1_LEVEL, fmtString, x, y, z);                \
      }

      #define LOG_MSG_INFO1_3( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO1_LEVEL, fmtString, x, y, z);                \
      }
	  #define LOG_MSG_INFO1_0( fmtString)                                   \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_INFO1_LEVEL, fmtString);                       \
      }
	  #define LOG_MSG_INFO1_1( fmtString, x)                                \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_INFO1_LEVEL, fmtString, x);                    \
      }
      #define LOG_MSG_INFO1_2( fmtString, x, y)                             \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_INFO1_LEVEL, fmtString, x, y);                 \
      }
      #define LOG_MSG_INFO1_4( fmtString, a, b, c, d)                       \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d);           \
      }
      #define LOG_MSG_INFO1_5( fmtString, a, b, c, d, e)                    \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e);        \
      }
      #define LOG_MSG_INFO1_6( fmtString, a, b, c, d, e, f)                \
      {                                                                     \
        PRINT_MSG_6 ( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e, f);    \
      }
      #define LOG_MSG_INFO1_9( fmtString, a, b, c, d, e, f, g, h, i)                \
      {                                                                             \
        PRINT_MSG_9 ( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e, f, g, h, i);   \
      }
      /**
        This macro is used to log an informational message. This macro logs
        the message with a lower priority than LOG_MSG_INFO1_3. Typically,
        RTT calculation messages are logged using this macro.
      */
      #define LOG_MSG_INFO2( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO2_LEVEL, fmtString, x, y, z);                \
      }
      #define LOG_MSG_INFO2_3( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO2_LEVEL, fmtString, x, y, z);                \
      }
	  #define LOG_MSG_INFO2_0( fmtString)                                   \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_INFO2_LEVEL, fmtString);                       \
      }
	  #define LOG_MSG_INFO2_1( fmtString, x)                                \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_INFO2_LEVEL, fmtString, x);                    \
      }
      #define LOG_MSG_INFO2_2( fmtString, x, y)                             \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_INFO2_LEVEL, fmtString, x, y);                 \
      }
      #define LOG_MSG_INFO2_4( fmtString, a, b, c, d)                       \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_INFO2_LEVEL, fmtString, a, b, c, d);           \
      }
      #define LOG_MSG_INFO2_5( fmtString, a, b, c, d, e)                    \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_INFO2_LEVEL, fmtString, a, b, c, d, e);        \
      }
      #define LOG_MSG_INFO2_6( fmtString, a, b, c, d, e, f)                \
      {                                                                     \
        PRINT_MSG_6 ( LOG_MSG_INFO2_LEVEL, fmtString, a, b, c, d, e, f);    \
      }

      /**
        This macro is used to log an informational message. This macro logs
        the message with a lower priority than LOG_MSG_INFO2_3. Typically,
        stack unwindings are logged using this macro. Normally a function
        logs a message whenever that function fails and it is not typically
        necessary to log another message as stack unwinds. So a low priority
        message is logged as stack unwinds and this message level is turned
        on only in debug builds.
      */
      #define LOG_MSG_INFO3( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO3_LEVEL, fmtString, x, y, z);                \
      }
      #define LOG_MSG_INFO3_3( fmtString, x, y, z)                          \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO3_LEVEL, fmtString, x, y, z);                \
      }
      #define LOG_MSG_INFO3_0( fmtString)                                   \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_INFO3_LEVEL, fmtString);                       \
      }
	  #define LOG_MSG_INFO3_1( fmtString, x)                            \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_INFO3_LEVEL, fmtString, x);                    \
      }
      #define LOG_MSG_INFO3_2( fmtString, x, y)                             \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_INFO3_LEVEL, fmtString, x, y);                 \
      }
      #define LOG_MSG_INFO3_4( fmtString, a, b, c, d)                       \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_INFO3_LEVEL, fmtString, a, b, c, d);           \
      }
      #define LOG_MSG_INFO3_5( fmtString, a, b, c, d, e)                    \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_INFO3_LEVEL, fmtString, a, b, c, d, e);        \
      }
      #define LOG_MSG_INFO3_6( fmtString, a, b, c, d, e, f)                \
      {                                                                     \
        PRINT_MSG_6 ( LOG_MSG_INFO3_LEVEL, fmtString, a, b, c, d, e, f);    \
      }

      /**
        This macro is used to log an informational message. This macro logs
        the message with a lower priority than LOG_MSG_INFO3. Typically,
        this macro is used to log in data path. Logging a message in data path
        results in too much noise in the F3 log and hence So a low priority
        message is logged to log data path. This message level is turned
        on only in debug builds.
      */
      #define LOG_MSG_INFO4( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO4_LEVEL, fmtString, x, y, z);                \
      }
      #define LOG_MSG_INFO4_3( fmtString, x, y, z)                          \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INFO4_LEVEL, fmtString, x, y, z);                \
      }
	  #define LOG_MSG_INFO4_0( fmtString)                               \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_INFO4_LEVEL, fmtString);                       \
      }
	  #define LOG_MSG_INFO4_1( fmtString, x)                            \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_INFO4_LEVEL, fmtString, x);                    \
      }
      #define LOG_MSG_INFO4_2( fmtString, x, y)                             \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_INFO4_LEVEL, fmtString, x, y);                 \
      }
      #define LOG_MSG_INFO4_4( fmtString, a, b, c, d)                       \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_INFO4_LEVEL, fmtString, a, b, c, d);           \
      }
      #define LOG_MSG_INFO4_5( fmtString, a, b, c, d, e)                    \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_INFO4_LEVEL, fmtString, a, b, c, d, e);        \
      }
      #define LOG_MSG_INFO4_6( fmtString, a, b, c, d, e, f)                 \
      {                                                                     \
        PRINT_MSG_6 ( LOG_MSG_INFO4_LEVEL, fmtString, a, b, c, d, e, f);    \
      }

      /**
        This macro is used to log an informational message in Data 
        Path. This message level is dependent on FEATURE_DATA_PATH_INFO 
      */
      #ifdef FEATURE_DATA_PATH_INFO
        #define LOG_MSG_DATA_PATH_INFO( fmtString, x, y, z)                 \
        {                                                                   \
          PRINT_MSG( LOG_MSG_INFO1_LEVEL, fmtString, x, y, z);              \
        }                                 
        #define LOG_MSG_DATA_PATH_INFO_3( fmtString, x, y, z)                \
        {                                                                    \
          PRINT_MSG( LOG_MSG_INFO1_LEVEL, fmtString, x, y, z);               \
        }
        #define LOG_MSG_DATA_PATH_INFO_0( fmtString)                         \
        {                                                                    \
          PRINT_MSG_0( LOG_MSG_INFO1_LEVEL, fmtString);                      \
        }
        #define LOG_MSG_DATA_PATH_INFO_1( fmtString, x)                      \
        {                                                                    \
          PRINT_MSG_1( LOG_MSG_INFO1_LEVEL, fmtString, x);                   \
        }
        #define LOG_MSG_DATA_PATH_INFO_2( fmtString, x, y)                   \
        {                                                                    \
          PRINT_MSG_2( LOG_MSG_INFO1_LEVEL, fmtString, x, y);                \
        }
        #define LOG_MSG_DATA_PATH_INFO_4( fmtString, a, b, c, d)             \
        {                                                                    \
          PRINT_MSG_4( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d);          \
        }
        #define LOG_MSG_DATA_PATH_INFO_5( fmtString, a, b, c, d, e)          \
        {                                                                    \
          PRINT_MSG_5( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e);       \
        }
        #define LOG_MSG_DATA_PATH_INFO_6( fmtString, a, b, c, d, e, f)       \
        {                                                                    \
          PRINT_MSG_6( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e, f);    \
        }
      #else
        #define LOG_MSG_DATA_PATH_INFO( fmtString, x, y, z)       
        #define LOG_MSG_DATA_PATH_INFO_3( fmtString, x, y, z)
        #define LOG_MSG_DATA_PATH_INFO_0( fmtString)
        #define LOG_MSG_DATA_PATH_INFO_1( fmtString, x)                      
        #define LOG_MSG_DATA_PATH_INFO_2( fmtString, x, y)                   
        #define LOG_MSG_DATA_PATH_INFO_4( fmtString, a, b, c, d)  
        #define LOG_MSG_DATA_PATH_INFO_5( fmtString, a, b, c, d, e) 
        #define LOG_MSG_DATA_PATH_INFO_6( fmtString, a, b, c, d, e, f)       
      #endif

      
      /**
        This macro is used to log an informational message in Data 
	    Protocols. This message level is dependent on 
	    FEATURE_DATA_PROTO_INFO 
      */
      #ifdef FEATURE_DATA_PROTO_INFO
        #define LOG_MSG_DATA_PROTO_INFO( fmtString, x, y, z)                 \
        {                                                                   \
          PRINT_MSG( LOG_MSG_INFO1_LEVEL, fmtString, x, y, z);              \
        }                                 
        #define LOG_MSG_DATA_PROTO_INFO_3( fmtString, x, y, z)                \
        {                                                                    \
          PRINT_MSG( LOG_MSG_INFO1_LEVEL, fmtString, x, y, z);               \
        }
        #define LOG_MSG_DATA_PROTO_INFO_0( fmtString)                         \
        {                                                                    \
          PRINT_MSG_0( LOG_MSG_INFO1_LEVEL, fmtString);                      \
        }
        #define LOG_MSG_DATA_PROTO_INFO_1( fmtString, x)                      \
        {                                                                    \
          PRINT_MSG_1( LOG_MSG_INFO1_LEVEL, fmtString, x);                   \
        }
        #define LOG_MSG_DATA_PROTO_INFO_2( fmtString, x, y)                   \
        {                                                                    \
          PRINT_MSG_2( LOG_MSG_INFO1_LEVEL, fmtString, x, y);                \
        }
        #define LOG_MSG_DATA_PROTO_INFO_4( fmtString, a, b, c, d)             \
        {                                                                    \
          PRINT_MSG_4( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d);          \
        }
        #define LOG_MSG_DATA_PROTO_INFO_5( fmtString, a, b, c, d, e)          \
        {                                                                    \
          PRINT_MSG_5( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e);       \
        }
        #define LOG_MSG_DATA_PROTO_INFO_6( fmtString, a, b, c, d, e, f)       \
        {                                                                    \
          PRINT_MSG_6( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e, f);    \
        }
      #else
        #define LOG_MSG_DATA_PROTO_INFO( fmtString, x, y, z)       
        #define LOG_MSG_DATA_PROTO_INFO_3( fmtString, x, y, z)
        #define LOG_MSG_DATA_PROTO_INFO_0( fmtString)
        #define LOG_MSG_DATA_PROTO_INFO_1( fmtString, x)                      
        #define LOG_MSG_DATA_PROTO_INFO_2( fmtString, x, y)                   
        #define LOG_MSG_DATA_PROTO_INFO_4( fmtString, a, b, c, d)  
        #define LOG_MSG_DATA_PROTO_INFO_5( fmtString, a, b, c, d, e) 
        #define LOG_MSG_DATA_PROTO_INFO_6( fmtString, a, b, c, d, e, f)       
      #endif
      
      
      typedef enum
      {
        PS_BAD_ARG_OR_NULL_PTR = 0,
        PS_MEM_ALLOC_ERR       = 1
      }log_msg_error_type;

/*===========================================================================

FUNCTION DATA_PROTOCOLS_ERROR_LOG()

DESCRIPTION
Function for mapping and printing Error Reason.
 
DEPENDENCIES 
None

RETURN VALUE
None
 
SIDE EFFECTS

=============================================================================*/
void data_protocols_error_log
(
  const char*          msg,
  unsigned int         line_num,
  log_msg_error_type   err_code,
  uint32               arg1,
  uint32               arg2,
  uint32               arg3
);

        #define DATA_PROTO_ERROR_LOG_3(err_code, arg1, arg2, arg3)            \
                data_protocols_error_log(__FILENAME__, __LINE__, err_code, (uint32)arg1, (uint32)arg2, (uint32)arg3)

        #define DATA_PROTO_ERROR_LOG_2(err_code, arg1, arg2)                  \
                data_protocols_error_log(__FILENAME__, __LINE__, err_code, (uint32)arg1, (uint32)arg2, 0)

        #define DATA_PROTO_ERROR_LOG_1(err_code, arg1)                        \
                data_protocols_error_log(__FILENAME__, __LINE__, err_code, (uint32)arg1, 0, 0)

        #define DATA_PROTO_ERROR_LOG_0(err_code)                              \
                data_protocols_error_log(__FILENAME__, __LINE__, err_code, 0, 0, 0)

   
      /*---------------------------------------------------------------------------
        PS timer log prints are disabled by default.  Enable FEATURE_DATA_PSTIMER_LOGGING
        in custdatamodem.h file for PSTIMER logging
      ---------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_PSTIMER_LOGGING
      
        #define LOG_PSTIMER_MSG_INFO1_0( fmtString)                           \
        {                                                                     \
          PRINT_MSG_0( LOG_MSG_INFO1_LEVEL, fmtString);                       \
        }
#define LOG_PSTIMER_MSG_INFO1_1( fmtString, x)                                \
        {                                                                     \
          PRINT_MSG_1( LOG_MSG_INFO1_LEVEL, fmtString, x);                    \
        }
#define LOG_PSTIMER_MSG_INFO1_2( fmtString, x, y)                             \
        {                                                                     \
          PRINT_MSG_2( LOG_MSG_INFO1_LEVEL, fmtString, x, y);                 \
        }
#define LOG_PSTIMER_MSG_INFO1_3( fmtString, a, b, c)                          \
        {                                                                     \
          PRINT_MSG_3( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c);              \
        }
#define LOG_PSTIMER_MSG_INFO1_4( fmtString, a, b, c, d)                       \
        {                                                                     \
          PRINT_MSG_4( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d);           \
        }
#define LOG_PSTIMER_MSG_INFO1_5( fmtString, a, b, c, d, e)                    \
        {                                                                     \
          PRINT_MSG_5( LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e);        \
        }
#define LOG_PSTIMER_MSG_INFO1_6( fmtString, a, b, c, d, e, f)                 \
        {                                                                     \
          PRINT_MSG_6(LOG_MSG_INFO1_LEVEL, fmtString, a, b, c, d, e, f);      \
        }
#else
        #define LOG_PSTIMER_MSG_INFO1_0( fmtString)
        #define LOG_PSTIMER_MSG_INFO1_1( fmtString, x)
        #define LOG_PSTIMER_MSG_INFO1_2( fmtString, x, y)
        #define LOG_PSTIMER_MSG_INFO1_3( fmtString, x, y, z)
        #define LOG_PSTIMER_MSG_INFO1_4( fmtString, a, b, c, d)  
        #define LOG_PSTIMER_MSG_INFO1_5( fmtString, a, b, c, d, e) 
        #define LOG_PSTIMER_MSG_INFO1_6( fmtString, a, b, c, d, e, f)
#endif /*FEATURE_DATA_PSTIMER_LOGGING*/

      /**
        This macro is used to log state changes.
      */
      #define LOG_MSG_STATE_CHANGE( fmtString, x, y, z)                     \
      {                                                                     \
        PRINT_MSG( LOG_MSG_STATE_CHANGE_LEVEL, fmtString, x, y, z);         \
      }
      #define LOG_MSG_STATE_CHANGE_3( fmtString, x, y, z)                   \
      {                                                                     \
        PRINT_MSG( LOG_MSG_STATE_CHANGE_LEVEL, fmtString, x, y, z);         \
      }
      #define LOG_MSG_STATE_CHANGE_0( fmtString)                            \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_STATE_CHANGE_LEVEL, fmtString);                \
      }
      #define LOG_MSG_STATE_CHANGE_1( fmtString, x)                         \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_STATE_CHANGE_LEVEL, fmtString, x);             \
      }
      #define LOG_MSG_STATE_CHANGE_2( fmtString, x, y)                      \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_STATE_CHANGE_LEVEL, fmtString, x, y);          \
      }
      #define LOG_MSG_STATE_CHANGE_4( fmtString, a, b, c, d)                \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_STATE_CHANGE_LEVEL, fmtString, a, b, c, d);    \
      }
      #define LOG_MSG_STATE_CHANGE_6( fmtString, a, b, c, d, e, f)          \
      {                                                                     \
        PRINT_MSG_6( LOG_MSG_STATE_CHANGE_LEVEL, fmtString,                 \
                     a, b, c, d, e, f);                                     \
      }

      /**
        This macro is used to log invalid user input. This macro is used
        only in the external API to applications when the input validation
        fails.
      */
      #define LOG_MSG_INVALID_INPUT( fmtString, x, y, z)                    \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INVALID_INPUT_LEVEL, fmtString, x, y, z);        \
      }
      #define LOG_MSG_INVALID_INPUT_3( fmtString, x, y, z)                  \
      {                                                                     \
        PRINT_MSG( LOG_MSG_INVALID_INPUT_LEVEL, fmtString, x, y, z);        \
      }
      #define LOG_MSG_INVALID_INPUT_0( fmtString)                           \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_INVALID_INPUT_LEVEL, fmtString);               \
      }
      #define LOG_MSG_INVALID_INPUT_1( fmtString, x)                        \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_INVALID_INPUT_LEVEL, fmtString, x);            \
      }
      #define LOG_MSG_INVALID_INPUT_2( fmtString, x, y)                     \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_INVALID_INPUT_LEVEL, fmtString, x, y);         \
      }
      #define LOG_MSG_INVALID_INPUT_4( fmtString, a, b, c, d)               \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_INVALID_INPUT_LEVEL, fmtString, a, b, c, d);   \
      }
      #define LOG_MSG_INVALID_INPUT_5( fmtString, a, b, c, d, e)            \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_INVALID_INPUT_LEVEL, fmtString, a, b, c, d, e);\
      }
      #define LOG_MSG_INVALID_INPUT_6( fmtString, a, b, c, d, e, f)         \
      {                                                                     \
        PRINT_MSG_6( LOG_MSG_INVALID_INPUT_LEVEL, fmtString,                \
                     a, b, c, d, e, f);                                     \
      }

      /**
        This macro is used to log recoverable errors with in Common Data code.
      */
      #define LOG_MSG_ERROR( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_ERROR_LEVEL, fmtString, x, y, z);                \
      }
      #define LOG_MSG_ERROR_3( fmtString, x, y, z)                            \
      {                                                                     \
        PRINT_MSG( LOG_MSG_ERROR_LEVEL, fmtString, x, y, z);                \
      }
      #define LOG_MSG_ERROR_0( fmtString)                                   \
      {                                                                     \
        PRINT_MSG_0( LOG_MSG_ERROR_LEVEL, fmtString);                       \
      }
	  #define LOG_MSG_ERROR_1( fmtString, x)                                \
      {                                                                     \
        PRINT_MSG_1( LOG_MSG_ERROR_LEVEL, fmtString, x);                    \
      }
      #define LOG_MSG_ERROR_2( fmtString, x, y)                             \
      {                                                                     \
        PRINT_MSG_2( LOG_MSG_ERROR_LEVEL, fmtString, x, y);                 \
      }
      #define LOG_MSG_ERROR_4( fmtString, a, b, c, d)                       \
      {                                                                     \
        PRINT_MSG_4( LOG_MSG_ERROR_LEVEL, fmtString, a, b, c, d);           \
      }
      #define LOG_MSG_ERROR_5( fmtString, a, b, c, d, e)                    \
      {                                                                     \
        PRINT_MSG_5( LOG_MSG_ERROR_LEVEL, fmtString, a, b, c, d, e);        \
      }
      #define LOG_MSG_ERROR_6( fmtString, a, b, c, d, e, f)                 \
      {                                                                     \
        PRINT_MSG_6( LOG_MSG_ERROR_LEVEL, fmtString, a, b, c, d, e, f);     \
      }
      #define LOG_MSG_ERROR_7( fmtString, a, b, c, d, e, f, g)              \
      {                                                                     \
        PRINT_MSG_7( LOG_MSG_ERROR_LEVEL, fmtString, a, b, c, d, e, f, g);  \
      }

      #define LOG_MSG_FATAL_ERROR_6( fmtString, a, b, c, d, e, f)           \
      {                                                                     \
        PRINT_MSG_6( LOG_MSG_FATAL_ERROR_LEVEL, fmtString,                  \
                     a, b, c, d, e, f);                                     \
      }

      /**
        This macro is used to log IPv4 addresses. The level can be specified
        using level parameter. Choose the level appropriately (INFO1,
        INFO2 etc.)
      */
      #define LOG_MSG_IPV4_ADDR(level, ip_addr)                             \
      {                                                                     \
        PRINT_MSG_6(level,                                                  \
                    "IPV4 Address is %d.%d.%d.%d",                          \
                    (unsigned char)(ip_addr),                               \
                    (unsigned char)(ip_addr >> 8),                          \
                    (unsigned char)(ip_addr >> 16) ,                        \
                    (unsigned char)(ip_addr >> 24),                         \
                    0,                                                      \
                    0);                                                     \
      }

      /**
	This macro is used to log IPV4 address with any formatted text
      */

      #define LOG_IPV4_ADDR_MSG_EX(fmtString, ip_addr)                      \
      {                                                                     \
        MSG_4( MSG_SSID_DS,                                                 \
               MSG_LEGACY_HIGH,                                             \
               fmtString,                                                   \
               (unsigned char)(ip_addr),                                    \
               (unsigned char)(ip_addr >> 8),                               \
               (unsigned char)(ip_addr >> 16) ,                             \
               (unsigned char)(ip_addr >> 24));                             \
      }

      /**
        This macro is used to log IPv6 addresses. The level can be specified
        using level parameter. Choose the level appropriately (INFO1,
        INFO2 etc.)
      */
      #define LOG_MSG_IPV6_ADDR(level, ip_addr)                             \
      {                                                                     \
        MSG_8( MSG_SSID_DS,                                                 \
               level,                                                       \
               "IPV6 Address %x:%x:%x:%x:%x:%x:%x:%x",                      \
               (uint16)(ps_ntohs(ip_addr[0])),                              \
               (uint16)(ps_ntohs(ip_addr[0] >> 16)),                        \
               (uint16)(ps_ntohs(ip_addr[0] >> 32)) ,                       \
               (uint16)(ps_ntohs(ip_addr[0] >> 48)),                        \
               (uint16)(ps_ntohs(ip_addr[1])),                              \
               (uint16)(ps_ntohs(ip_addr[1] >> 16)),                        \
               (uint16)(ps_ntohs(ip_addr[1] >> 32)) ,                       \
               (uint16)(ps_ntohs(ip_addr[1] >> 48)));                       \
      }

      /**
	This macro is used to print IPV6 address with any formatted text
      */
      #define LOG_IPV6_ADDR_MSG_EX(fmtString,ip_addr)                       \
      {                                                                     \
        MSG_8( MSG_SSID_DS,                                                 \
               MSG_LEGACY_HIGH,                                             \
               fmtString,                                                   \
               (uint16)(ps_ntohs(ip_addr[0])),                              \
               (uint16)(ps_ntohs(ip_addr[0] >> 16)),                        \
               (uint16)(ps_ntohs(ip_addr[0] >> 32)) ,                       \
               (uint16)(ps_ntohs(ip_addr[0] >> 48)),                        \
               (uint16)(ps_ntohs(ip_addr[1])),                              \
               (uint16)(ps_ntohs(ip_addr[1] >> 16)),                        \
               (uint16)(ps_ntohs(ip_addr[1] >> 32)) ,                       \
               (uint16)(ps_ntohs(ip_addr[1] >> 48)));                       \
      }

      /**
	This macro prints an IPV6 IID 
      */
      #define LOG_IPV6_IID_MSG_EX(fmtString, iid)                           \
      {                                                                     \
        MSG_4( MSG_SSID_DS,                                                 \
               MSG_LEGACY_HIGH,                                             \
               fmtString,                                                   \
               (uint16)(ps_ntohs(iid)),                                     \
               (uint16)(ps_ntohs(iid >> 16)),                               \
               (uint16)(ps_ntohs(iid >> 32)) ,                              \
               (uint16)(ps_ntohs(iid >> 48)));                              \
      }


/**
  This macro is used to log an informational message with string arguments(%s specifier).
  For example "Routing look up returned iface" etc.
*/
#define LOG_MSG_STR_INFO1_01( fmtString, x)                                  \
{                                                                            \
  MSG_SPRINTF_1(MSG_SSID_DS, LOG_MSG_INFO1_LEVEL, fmtString, x );            \
}

#define LOG_MSG_STR_INFO1_02( fmtString, x, y)                               \
{                                                                            \
  MSG_SPRINTF_2(MSG_SSID_DS, LOG_MSG_INFO1_LEVEL, fmtString, x, y );         \
}

#define LOG_MSG_STR_INFO1_03( fmtString, x, y, z)                            \
{                                                                            \
  MSG_SPRINTF_3(MSG_SSID_DS, LOG_MSG_INFO1_LEVEL, fmtString, x, y, z );      \
}

/*---------------------------------------------------------------------- 
  Wrappers for ASSERT and ERROR_FATAL's
----------------------------------------------------------------------*/
#define DS_UTILS_ASSERT(val)\
  ps_logging_assert( __FILENAME__ , __FUNCTION__ , __LINE__, val)

#define DS_UTILS_ERROR_FATAL(xx_exp) \
  ps_logging_err_fatal( __FILENAME__ , __FUNCTION__ , __LINE__, xx_exp)

/*---------------------------------------------------------------------- 
  Macros for ERROR_MSGs
----------------------------------------------------------------------*/
#define LOG_MSG_ERROR_INT_0(str) \
	  ps_logging_error_log_0(__FILENAME__ ,__LINE__,str)
		
#define LOG_MSG_ERROR_INT_1(str, arg1) \
	  ps_logging_error_log_1(__FILENAME__ ,__LINE__,str, (uint32)arg1)
	
#define LOG_MSG_ERROR_INT_2(str, arg1, arg2) \
	  ps_logging_error_log_2(__FILENAME__ ,__LINE__,str, (uint32)arg1, \
	  (uint32)arg2)
	  
#define LOG_MSG_ERROR_INT_3(str, arg1, arg2, arg3) \
	  ps_logging_error_log_3(__FILENAME__ ,__LINE__,str, (uint32)arg1, \
	  (uint32)arg2, (uint32)arg3)
	
#define LOG_MSG_ERROR_INT_4(str, arg1, arg2, arg3, arg4) \
	  ps_logging_error_log_4(__FILENAME__ ,__LINE__,str, (uint32)arg1, \
	  (uint32)arg2, (uint32)arg3, (uint32)arg4)


    /**
      Ulog macros for PS wk logging
    */ 
#define   LOG_ULOG_MSG_INFO_0( fmtString)                                   \
      {                                                                         \
        ULOG_RT_PRINTF_0( ps_fwk_ulog_hable, fmtString);                  \
      }
    
#define   LOG_ULOG_MSG_INFO_1( fmtString, a)                                   \
      {                                                                         \
        ULOG_RT_PRINTF_1( ps_fwk_ulog_hable, fmtString, a);                  \
      }
    
#define   LOG_ULOG_MSG_INFO_2( fmtString, a, b)                                   \
      {                                                                         \
        ULOG_RT_PRINTF_2( ps_fwk_ulog_hable, fmtString, a, b);                  \
      }
    
#define   LOG_ULOG_MSG_INFO_3( fmtString, a, b, c)                                   \
      {                                                                         \
        ULOG_RT_PRINTF_3( ps_fwk_ulog_hable, fmtString, a, b, c);                  \
      }
    
#define   LOG_ULOG_MSG_INFO_4( fmtString, a, b, c, d)                                   \
      {                                                                         \
        ULOG_RT_PRINTF_4( ps_fwk_ulog_hable, fmtString, a, b, c, d);                  \
      }
    
#define   LOG_ULOG_MSG_INFO_5( fmtString, a, b, c, d, e)                                   \
      {                                                                         \
        ULOG_RT_PRINTF_5( ps_fwk_ulog_hable, fmtString, a, b, c, d, e);                  \
      }

/*===========================================================================

FUNCTION PS_LOGGING_ERR_FATAL

DESCRIPTION
  Wrapper function for ERROR_FATALS
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_err_fatal
(  
  const char    *file_name,
  const char    *func_name,
  unsigned int  line_num,
  const char    *str
);
  
/*===========================================================================

FUNCTION PS_LOGGING_ASSERT

DESCRIPTION
  Wrapper function for ASSERTION
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_assert
(  
  const char      *file_name,
  const char      *func_name,
  unsigned int     line_num,
  int              expression
);  

/*===========================================================================

FUNCTION PS_LOGGING_ERROR_LOG_0

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_error_log_0
(
  const char*   file_name,
  unsigned int  line_num,  
  const char*   format
);

/*===========================================================================

FUNCTION PS_LOGGING_ERROR_LOG_1

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_error_log_1
(
  const char*   file_name,
  unsigned int  line_num,
  const char*   format,
  uint32        arg1
);

/*===========================================================================

FUNCTION PS_LOGGING_ERROR_LOG_2

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_error_log_2
(
  const char*   file_name,
  unsigned int  line_num,
  const char*   format,
  uint32        arg1,
  uint32        arg2
);

/*===========================================================================

FUNCTION PS_LOGGING_ERROR_LOG_3

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_error_log_3
(
  const char*   file_name,
  unsigned int  line_num,  
  const char*   format,
  uint32        arg1,
  uint32        arg2,
  uint32        arg3
);

/*===========================================================================

FUNCTION PS_LOGGING_ERROR_LOG_4

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_error_log_4
(
  const char*   file_name,
  unsigned int  line_num,
  const char*   format,
  uint32        arg1,
  uint32        arg2,
  uint32        arg3,
  uint32        arg4
);


/*==============================================================================

                              LOG MACROS

==============================================================================*/

/*!
  Log macros per buffer/level, with/without tags.

  @param _tag       PS_TAG_*
  @param _inst_id   sys_modem_as_id_e_type
  @param _summary   string for format
  @param ...        variadic args
*/

/* -----------------------------------------------------------------------------
  Buffer: HF
----------------------------------------------------------------------------- */

#define LOG_MSG_QTRACE_HF_DEBUG(                                              \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_DEBUG,                                                       \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    QTRACE_TAG_NONE,                                                          \
    summary,                                                                  \
    ##__VA_ARGS__)

#define LOG_MSG_QTRACE_HF_MED(                                                \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_MED,                                                         \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    QTRACE_TAG_NONE,                                                          \
    summary,                                                                  \
    ##__VA_ARGS__)

#define LOG_MSG_QTRACE_HF_HIGH(                                               \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_HIGH,                                                        \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    QTRACE_TAG_NONE,                                                          \
    summary,                                                                  \
    ##__VA_ARGS__)

#define LOG_MSG_QTRACE_HF_ERROR(                                              \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_ERROR,                                                       \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    QTRACE_TAG_NONE,                                                          \
    summary,                                                                  \
    ##__VA_ARGS__)

/* -----------------------------------------------------------------------------
  QTRACE Macros With TAGS
----------------------------------------------------------------------------- */

#define LOG_MSG_QTRACE_HFT_DEBUG(                                             \
  tag,                                                                        \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_DEBUG,                                                       \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    tag,                                                                      \
    summary,                                                                  \
    ##__VA_ARGS__)

#define LOG_MSG_QTRACE_HFT_MED(                                               \
  tag,                                                                        \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_MED,                                                         \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    tag,                                                                      \
    summary,                                                                  \
    ##__VA_ARGS__)

#define LOG_MSG_QTRACE_HFT_HIGH(                                              \
  tag,                                                                        \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_HIGH,                                                        \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    tag,                                                                      \
    summary,                                                                  \
    ##__VA_ARGS__)

#define LOG_MSG_QTRACE_HFT_ERROR(                                             \
  tag,                                                                        \
  subs_id,                                                                    \
  summary,                                                                    \
  ...                                                                         \
)                                                                             \
  QTRACE(                                                                     \
    ps_utils_qtrace_get_context_id_tbl(PS_UTILS_QTRACE_HF),                   \
    QSH_CLT_DS,                                                               \
    QTRACE_LEVEL_ERROR,                                                       \
    ps_utils_convert_subs_ps_to_sys(subs_id),                                 \
    tag,                                                                      \
    summary,                                                                  \
    ##__VA_ARGS__)

#endif /* DS_UTILS_DEBUG_MSG_H */
