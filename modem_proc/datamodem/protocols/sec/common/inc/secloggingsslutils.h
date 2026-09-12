#ifndef SECLOGGINGSSLUTILS_H
#define SECLOGGINGSSLUTILS_H

/**
   @file secloggingsslutils.h

   @brief 
   Diagnostic Message Header File.

   This module consists of diagnostic message macros.
   The messages are output on qxdm utility.

   Externalized Functions:
   None.

   Initialization and sequencing requirements:
   None.
*/

/*===========================================================================
                                S E C  D I A G
                              H E A D E R  F I L E

Copyright (c) 2006-2021 by Qualcomm Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/


/*===========================================================================
                           EDIT HISTORY FOR FILE

when       who      what, where, why
--------   ---      ------------------------------------
02/12/21   rk       Compiler Version Upgrade to 8.5.01 fixes
07/12/18   shrvn    Added Wrappers for MSG0 and MSG1 F3s, and also some more macros.
05/01/18   ak       Created

===========================================================================*/
#include "msg.h"
#include "secdiag.h"

/*===========================================================================
  DS_SSL related macros
===========================================================================*/

/*---------------------------------------------------------------------------
  Macro for messages with no parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG0_MED(xx_fmt) \
  MSG(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt)

#define DS_SSL_MSG0_HIGH(xx_fmt) \
  ds_ssl_log_msg0(MSG_LEGACY_HIGH, __FILENAME__, __LINE__, xx_fmt)

#define DS_SSL_MSG0_ERROR(xx_fmt) \
  ds_ssl_log_msg0(MSG_LEGACY_ERROR, __FILENAME__, __LINE__, xx_fmt)

/*---------------------------------------------------------------------------
  Macro for messages with 1 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG1_MED(xx_fmt, xx_arg1) \
  MSG_1(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1)

#define DS_SSL_MSG1_HIGH(xx_fmt, xx_arg1) \
  ds_ssl_log_msg1(MSG_LEGACY_HIGH, __FILENAME__, __LINE__, xx_fmt, (uint32)xx_arg1)

#define DS_SSL_MSG1_ERROR(xx_fmt, xx_arg1) \
  ds_ssl_log_msg1(MSG_LEGACY_ERROR, __FILENAME__, __LINE__, xx_fmt, (uint32)xx_arg1)

/*---------------------------------------------------------------------------
  Macro for messages with 2 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG2_MED(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2)

#define DS_SSL_MSG2_HIGH(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2)

#define DS_SSL_MSG2_ERROR(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  Macro for messages with 3 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG3_MED(xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_3(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, xx_arg3)

#define DS_SSL_MSG3_HIGH(xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_3(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3)

#define DS_SSL_MSG3_ERROR(xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_3(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, xx_arg3)

/*---------------------------------------------------------------------------
  Macro for messages with 4 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG4_MED(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4 )\
  MSG_4(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4)

#define DS_SSL_MSG4_HIGH(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4 )\
  MSG_4(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4)

#define DS_SSL_MSG4_ERROR(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  MSG_4(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4)

/*---------------------------------------------------------------------------
  Macro for messages with 5 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG5_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5 )\
  MSG_5(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5)

#define DS_SSL_MSG5_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5 )\
  MSG_5(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,  xx_arg5)

#define DS_SSL_MSG5_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5) \
  MSG_5(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5)

/*---------------------------------------------------------------------------
  Macro for messages with 6 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG6_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6 )\
  MSG_6(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6)

#define DS_SSL_MSG6_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6 )\
  MSG_6(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6)

#define DS_SSL_MSG6_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6) \
  MSG_6(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,xx_arg5, xx_arg6)

/*---------------------------------------------------------------------------
  Macro for messages with 7 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG7_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7 )\
  MSG_7(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7)

#define DS_SSL_MSG7_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7 )\
  MSG_7(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7)

#define DS_SSL_MSG7_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7) \
  MSG_7(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,xx_arg5, xx_arg6, xx_arg7)

/*---------------------------------------------------------------------------
  Macro for messages with 8 parameters.
---------------------------------------------------------------------------*/
#define DS_SSL_MSG8_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8 )\
  MSG_8(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8)

#define DS_SSL_MSG8_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8 )\
  MSG_8(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8)

#define DS_SSL_MSG8_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8) \
  MSG_8(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,xx_arg5, xx_arg6, xx_arg7, xx_arg8)


/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 1 parameters
---------------------------------------------------------------------------*/
#define DS_SSL_MSG_SPRINTF_1(xx_ss_mask, xx_fmt, xx_arg1) \
  MSG_SPRINTF_1(MSG_SSID_SECDIAG_SSL, xx_ss_mask, xx_fmt, xx_arg1)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 2 parameters
---------------------------------------------------------------------------*/
#define DS_SSL_MSG_SPRINTF_2(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2) \
  MSG_SPRINTF_2(MSG_SSID_SECDIAG_SSL, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 3 parameters
---------------------------------------------------------------------------*/
#define DS_SSL_MSG_SPRINTF_3(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_SPRINTF_3(MSG_SSID_SECDIAG_SSL, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 4 parameters
---------------------------------------------------------------------------*/
#define DS_SSL_MSG_SPRINTF_4(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  MSG_SPRINTF_4(MSG_SSID_SECDIAG_SSL, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 5 parameters
---------------------------------------------------------------------------*/
#define DS_SSL_MSG_SPRINTF_5(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5) \
  MSG_SPRINTF_5(MSG_SSID_SECDIAG_SSL, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 6 parameters
---------------------------------------------------------------------------*/
#define DS_SSL_MSG_SPRINTF_6(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6) \
  MSG_SPRINTF_6(MSG_SSID_SECDIAG_SSL, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 7 parameters
---------------------------------------------------------------------------*/
#define DS_SSL_MSG_SPRINTF_7(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6, xx_arg7) \
  MSG_SPRINTF_7(MSG_SSID_SECDIAG_SSL, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
   xx_arg4, xx_arg5, xx_arg6, xx_arg7)

/*---------------------------------------------------------------------------
  Input Error F3 log messages for SSL
---------------------------------------------------------------------------*/

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with 3 parameters
 ---------------------------------------------------------------------------*/
#define DS_SSL_INPUT_ERROR_LOG_3(arg1, arg2, arg3) \
        ds_ssl_input_3_error_log(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, (uint32)arg3)

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with 2 parameters
 ---------------------------------------------------------------------------*/
#define DS_SSL_INPUT_ERROR_LOG_2(arg1, arg2) \
        ds_ssl_input_3_error_log(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, 0)

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with 1 parameters
 ---------------------------------------------------------------------------*/
#define DS_SSL_INPUT_ERROR_LOG_1(arg1) \
        ds_ssl_input_3_error_log(__FILENAME__, __LINE__, (uint32)arg1, 0, 0)

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with no parameters
 ---------------------------------------------------------------------------*/
#define DS_SSL_INPUT_ERROR_LOG_0() \
        ds_ssl_input_3_error_log(__FILENAME__, __LINE__, 0, 0, 0)

/*===========================================================================

FUNCTION DS_SSL_INPUT_ERROR_LOG

DESCRIPTION
  Report input validation failures on parameters
     msg - a const char msg. Typical inputs uses __FILE__ or __FUNCTION__
     line_num - line in code of where error occured
     arg1 - argument
     arg2 - argument
     arg3 - argument
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ds_ssl_input_3_error_log
(
  const char*  msg,
  unsigned int line_num,
  uint32 arg1,
  uint32 arg2,
  uint32 arg3
);


/*===========================================================================

FUNCTION DS_SSL_MEM_ERROR_LOG

DESCRIPTION
  Report heap allocation failures
     msg - a const char msg. Typical inputs uses __FILE__ or __FUNCTION__
     line_num - line in code of where error occured
     arg1 - argument
     arg2 - argument
     arg3 - argument
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
#define DS_SSL_MEM_ERROR_LOG() \
        ds_ssl_mem_error_log(__FILENAME__, __LINE__)

void ds_ssl_mem_error_log
(
  const char*  msg,
  unsigned int line_num
);

/*===========================================================================

FUNCTION DS_SSL_LOG_MSG0

DESCRIPTION
  Prints Messages with 0 parameters i.e., prints a string.

DEPENDENCIES 
  None.

PARAMETERS
  msg_mask - level to indicate MED, HIGH and ERROR messages.
  func     - function name.
  line_num - line number.
  msg      - a const char msg.
  
RETURN VALUE
 None.
 
SIDE EFFECTS
  None.
===========================================================================*/
void ds_ssl_log_msg0
(
  uint16       msg_mask,
  const char*  func,
  unsigned int line_num,
  const char*  msg
);

/*===========================================================================

FUNCTION DS_SSL_LOG_MSG1

DESCRIPTION
  Prints Messages with 1 parameter i.e., prints a string and an argument.

DEPENDENCIES 
  None.

PARAMETERS
  msg_mask - level to indicate MED, HIGH and ERROR messages.
  func     - function name.
  line_num - line number.
  msg      - a const char msg.
  xx_arg1  - an argument.
  
RETURN VALUE
 None.
 
SIDE EFFECTS
  None.
===========================================================================*/
void ds_ssl_log_msg1
(
  uint16       msg_mask,
  const char*  func,
  unsigned int line_num,
  const char*  msg,
  int16        xx_arg1
);

/*===========================================================================
                               DEBUG MSG
==========================================================================*/
#ifdef FEATURE_SEC_SSL_DEBUG
      /*SSL debug Macros*/
      #define DS_SSL_MSG4_DEBUG( fmtString, x, y, z, a)\
      {                                                                    \
        MSG_4( MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, fmtString, x, y, z, a);\
      }     
      #define DS_SSL_MSG3_DEBUG( fmtString, x, y, z)\
      {                                                                    \
        MSG_3( MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, fmtString, x, y, z);          \
      }                                 
      #define DS_SSL_MSG2_DEBUG( fmtString, x, y)                      \
      {                                                                    \
        MSG_2( MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, fmtString, x, y);             \
      }
      #define DS_SSL_MSG1_DEBUG( fmtString, x)                         \
      {                                                                    \
        MSG_1( MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, fmtString, x);                \
      }
      #define DS_SSL_MSG0_DEBUG( fmtString)                            \
      {                                                                    \
        MSG(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_HIGH, fmtString);                      \
      }
      #define DS_SSL_MSG3_ERROR_DEBUG( fmtString, x, y, z)                 \
      {                                                                    \
        MSG_3( MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, fmtString, x, y, z);         \
      }                                 
      #define DS_SSL_MSG2_ERROR_DEBUG( fmtString, x, y)                    \
      {                                                                    \
        MSG_2( MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, fmtString, x, y);            \
      }
      #define DS_SSL_MSG1_ERROR_DEBUG( fmtString, x)                       \
      {                                                                    \
        MSG_1( MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, fmtString, x);               \
      }
      #define DS_SSL_MSG0_ERROR_DEBUG( fmtString)                          \
      {                                                                    \
        MSG(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, fmtString);                     \
      }
      #define DS_SSL_MSG_SPRINTF1_ERROR_DEBUG(fmtString, xx_arg1)                 \
      {                                                                           \
        MSG_SPRINTF_1(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, fmtString,            \
                      xx_arg1);                                                     \
      }
      #define DS_SSL_MSG_SPRINTF2_ERROR_DEBUG(fmtString, xx_arg1, xx_arg2)          \
      {                                                                             \
        MSG_SPRINTF_2(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, fmtString,            \
                      xx_arg1, xx_arg2);                                            \
      }
      #define DS_SSL_MSG_SPRINTF3_ERROR_DEBUG(fmtString, xx_arg1, xx_arg2, xx_arg3) \
      {                                                                             \
        MSG_SPRINTF_3(MSG_SSID_SECDIAG_SSL, MSG_LEGACY_ERROR, fmtString,            \
                      xx_arg1, xx_arg2, xx_arg3);                                   \
      }
#else 
        /*SSL Debug Macro*/ 
        #define DS_SSL_MSG4_DEBUG( fmtString, x, y, z, a)       
        #define DS_SSL_MSG3_DEBUG( fmtString, x, y, z)       
        #define DS_SSL_MSG2_DEBUG( fmtString, x, y)
        #define DS_SSL_MSG1_DEBUG( fmtString, x)
        #define DS_SSL_MSG0_DEBUG( fmtString)
        #define DS_SSL_MSG3_ERROR_DEBUG( fmtString, x, y, z)       
        #define DS_SSL_MSG2_ERROR_DEBUG( fmtString, x, y)
        #define DS_SSL_MSG1_ERROR_DEBUG( fmtString, x)
        #define DS_SSL_MSG0_ERROR_DEBUG( fmtString)
        #define DS_SSL_MSG_SPRINTF1_ERROR_DEBUG(fmtString, xx_arg1)
        #define DS_SSL_MSG_SPRINTF2_ERROR_DEBUG(fmtString, xx_arg1, xx_arg2)
        #define DS_SSL_MSG_SPRINTF3_ERROR_DEBUG(fmtString, xx_arg1, xx_arg2, xx_arg3)
#endif /* FEATURE_SEC_SSL_DEBUG */

#endif /* SECLOGGINGSSLUTILS_H */


