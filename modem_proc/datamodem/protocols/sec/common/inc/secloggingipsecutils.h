#ifndef SECLOGGINGIPSECUTILS_H
#define SECLOGGINGIPSECUTILS_H

/**
   @file secloggingipsecutils.h

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

Copyright (c) 2006-2018 by Qualcomm Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/


/*===========================================================================
                           EDIT HISTORY FOR FILE

when       who      what, where, why
--------   ---      ------------------------------------
05/01/18   ak       Created

===========================================================================*/
#include "msg.h"
#include "secdiag.h"

#define SEC_LOG_EXT_VERSION 1

/*===========================================================================
  DS_SEC related macros
===========================================================================*/

/*---------------------------------------------------------------------------
  Macro for messages with no parameters.
---------------------------------------------------------------------------*/
#define DS_SEC_MSG0_MED( xx_fmt) \
  MSG(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, xx_fmt)

#define DS_SEC_MSG0_HIGH( xx_fmt) \
  MSG(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, xx_fmt)

#define DS_SEC_MSG0_ERROR( xx_fmt) \
  MSG(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, xx_fmt)

/*---------------------------------------------------------------------------
  Macro for messages with 1 parameters.
---------------------------------------------------------------------------*/
#define DS_SEC_MSG1_MED(xx_fmt, xx_arg1) \
  MSG_1(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, xx_fmt, xx_arg1)

#define DS_SEC_MSG1_HIGH(xx_fmt, xx_arg1) \
  MSG_1(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, xx_fmt, xx_arg1)

#define DS_SEC_MSG1_ERROR(xx_fmt, xx_arg1) \
  MSG_1(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, xx_fmt, xx_arg1)

/*---------------------------------------------------------------------------
  Macro for messages with 2 parameters.
---------------------------------------------------------------------------*/
#define DS_SEC_MSG2_MED(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2)

#define DS_SEC_MSG2_HIGH(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2)

#define DS_SEC_MSG2_ERROR(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  Macro for messages with 3 parameters.
---------------------------------------------------------------------------*/
#define DS_SEC_MSG3_MED(xx_fmt, xx_arg1, xx_arg2, xx_arg) \
  MSG_3(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, xx_arg)

#define DS_SEC_MSG3_HIGH(xx_fmt, xx_arg1, xx_arg2, xx_arg) \
  MSG_3(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg)

#define DS_SEC_MSG3_ERROR(xx_fmt, xx_arg1, xx_arg2, xx_arg) \
  MSG_3(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, xx_arg)

/*---------------------------------------------------------------------------
  Macro for messages with 4 parameters.
---------------------------------------------------------------------------*/
#define DS_SEC_MSG4_MED(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg )\
  MSG_4(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg)

#define DS_SEC_MSG4_HIGH(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg )\
  MSG_4(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg)

#define DS_SEC_MSG4_ERROR(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg) \
  MSG_4(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg)

/*---------------------------------------------------------------------------
  Macro for messages with 5 parameters.
---------------------------------------------------------------------------*/
#define DS_SEC_MSG5_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg )\
  MSG_5(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg)

#define DS_SEC_MSG5_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg )\
  MSG_5(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,  xx_arg)

#define DS_SEC_MSG5_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg) \
  MSG_5(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg)

/*---------------------------------------------------------------------------
  Macro for messages with 6 parameters.
---------------------------------------------------------------------------*/
#define DS_SEC_MSG6_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg )\
  MSG_6(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg)

#define DS_SEC_MSG6_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg )\
  MSG_6(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg)

#define DS_SEC_MSG6_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg) \
  MSG_6(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,xx_arg5, xx_arg)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 1 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_1(xx_ss_mask, xx_fmt, xx_arg1) \
  MSG_SPRINTF_1(MSG_SSID_SECDIAG_IPSEC, xx_ss_mask, xx_fmt, xx_arg1)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 2 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_2(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2) \
  MSG_SPRINTF_2(MSG_SSID_SECDIAG_IPSEC, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 3 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_3(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_SPRINTF_3(MSG_SSID_SECDIAG_IPSEC, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 4 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_4(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  MSG_SPRINTF_4(MSG_SSID_SECDIAG_IPSEC, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 5 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_5(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5) \
  MSG_SPRINTF_5(MSG_SSID_SECDIAG_IPSEC, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 6 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_6(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6) \
  MSG_SPRINTF_6(MSG_SSID_SECDIAG_IPSEC, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 7 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_7(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6, xx_arg7) \
  MSG_SPRINTF_7(MSG_SSID_SECDIAG_IPSEC, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
   xx_arg4, xx_arg5, xx_arg6, xx_arg7)


/*---------------------------------------------------------------------------
  Macro for messages with subs_id parameter
---------------------------------------------------------------------------*/
#define DS_SEC_MSG0_MED_SUB(fmtString)    \
   MSG_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString)
 
#define DS_SEC_MSG0_HIGH_SUB(fmtString)    \
   MSG_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString)
 
#define DS_SEC_MSG0_ERROR_SUB(fmtString)    \
   MSG_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString)
 
#define DS_SEC_MSG1_MED_SUB(fmtString, x)    \
   MSG_1_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x)
 
#define DS_SEC_MSG1_HIGH_SUB(fmtString, x)    \
   MSG_1_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x)
 
#define DS_SEC_MSG1_ERROR_SUB(fmtString, x)    \
   MSG_1_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x)
 
#define DS_SEC_MSG2_MED_SUB(fmtString, x, y)    \
   MSG_2_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y)
 
#define DS_SEC_MSG2_HIGH_SUB(fmtString, x, y)    \
   MSG_2_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y)
 
#define DS_SEC_MSG2_ERROR_SUB(fmtString, x, y)    \
   MSG_2_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y)
 
#define DS_SEC_MSG3_MED_SUB(fmtString, x, y, z)    \
   MSG_3_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z)
 
#define DS_SEC_MSG3_HIGH_SUB(fmtString, x, y, z)    \
   MSG_3_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z)
 
#define DS_SEC_MSG3_ERROR_SUB(fmtString, x, y, z)    \
   MSG_3_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z)
 
#define DS_SEC_MSG4_MED_SUB(fmtString, x, y, z, a)    \
   MSG_4_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a)
 
#define DS_SEC_MSG4_HIGH_SUB(fmtString, x, y, z, a)    \
   MSG_4_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a)
 
#define DS_SEC_MSG4_ERROR_SUB(fmtString, x, y, z, a)    \
   MSG_4_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a)
 
#define DS_SEC_MSG5_MED_SUB(fmtString, x, y, z, a, b)    \
   MSG_5_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a, b)
 
#define DS_SEC_MSG5_HIGH_SUB(fmtString, x, y, z, a, b)    \
   MSG_5_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a, b)
 
#define DS_SEC_MSG5_ERROR_SUB(fmtString, x, y, z, a, b)    \
   MSG_5_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a, b)

#define DS_SEC_MSG6_MED_SUB(fmtString, x, y, z, a, b,c)    \
   MSG_6_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_MED, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a, b,c)
 
#define DS_SEC_MSG6_HIGH_SUB(fmtString, x, y, z, a, b,c)    \
   MSG_6_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a, b,c)
 
#define DS_SEC_MSG6_ERROR_SUB(fmtString, x, y, z, a, b,c)    \
   MSG_6_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,    \
     &SEC_LOG_SUB_ID, fmtString, x, y, z, a, b,c)


/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 0 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_SUB(xx_ss_mask, xx_fmt)        \
  MSG_SPRINTF_1_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID, xx_fmt, 0)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 1 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_1_SUB(xx_ss_mask, xx_fmt, xx_arg1)        \
  MSG_SPRINTF_1_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID, xx_fmt, xx_arg1)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 2 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_2_SUB(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2) \
  MSG_SPRINTF_2_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 3 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_3_SUB(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_SPRINTF_3_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 4 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_4_SUB(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  MSG_SPRINTF_4_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 5 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_5_SUB(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5) \
  MSG_SPRINTF_5_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5)


/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 6 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_6_SUB(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6) \
  MSG_SPRINTF_6_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 7 parameters
---------------------------------------------------------------------------*/
#define DS_SEC_MSG_SPRINTF_7_SUB(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7) \
  MSG_SPRINTF_7_EXT(MSG_SSID_SECDIAG_IPSEC, (xx_ss_mask), SEC_LOG_EXT_VERSION,    \
    &SEC_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7)


/*---------------------------------------------------------------------------
  Input Error F3 log messages for IPSEC
---------------------------------------------------------------------------*/

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with 3 parameters
 ---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_3(arg1, arg2, arg3) \
        ds_sec_input_3_error_log(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, (uint32)arg3)

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with 2 parameters
 ---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_2(arg1, arg2) \
        ds_sec_input_3_error_log(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, 0)

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with 1 parameters
 ---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_1(arg1) \
        ds_sec_input_3_error_log(__FILENAME__, __LINE__, (uint32)arg1, 0, 0)

 /*---------------------------------------------------------------------------
   This is the macro for input error messages with no parameters
 ---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_0() \
        ds_sec_input_3_error_log(__FILENAME__, __LINE__, 0, 0, 0)

/*---------------------------------------------------------------------------
  This is the macro for input error messages with 3 parameters along with Sub
---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_SUB_3(arg1, arg2, arg3) \
        ds_sec_input_3_error_log_sub(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, (uint32)arg3, SEC_LOG_SUB_ID)

/*---------------------------------------------------------------------------
  This is the macro for input error messages with 2 parameters along with Sub
---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_2_SUB(arg1, arg2) \
        ds_sec_input_3_error_log_sub(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, 0, SEC_LOG_SUB_ID)

/*---------------------------------------------------------------------------
  This is the macro for input error messages with 1 parameters along with Sub
---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_1_SUB(arg1) \
        ds_sec_input_3_error_log_sub(__FILENAME__, __LINE__, (uint32)arg1, 0, 0, SEC_LOG_SUB_ID)

/*---------------------------------------------------------------------------
  This is the macro for input error messages with 0 parameters along with Sub
---------------------------------------------------------------------------*/
#define DS_SEC_INPUT_ERROR_LOG_0_SUB() \
        ds_sec_input_3_error_log_sub(__FILENAME__, __LINE__, 0, 0, 0, SEC_LOG_SUB_ID)

/*===========================================================================

FUNCTION DS_SEC_INPUT_ERROR_LOG

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
void ds_sec_input_3_error_log
(
  const char*  msg,
  unsigned int line_num,
  uint32 arg1,
  uint32 arg2,
  uint32 arg3
);

void ds_sec_input_3_error_log_sub
(
  const char*  msg,
  unsigned int line_num,
  uint32 arg1,
  uint32 arg2,
  uint32 arg3,
  uint32 SEC_LOG_SUB_ID
);

/*===========================================================================

FUNCTION DS_SEC_MEM_ERROR_LOG

DESCRIPTION
  Report heap allocation failures
     msg - a const char msg. Typical inputs uses __FILE__ or __FUNCTION__
     line_num - line in code of where error occured
     arg1 - arguements
     arg2 - arguements
     arg3 - arguements
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
#define DS_SEC_MEM_ERROR_LOG() \
        ds_sec_mem_error_log(__FILENAME__, __LINE__)

#define DS_SEC_MEM_ERROR_LOG_SUB() \
        ds_sec_mem_error_log_sub(__FILENAME__, __LINE__, SEC_LOG_SUB_ID)

void ds_sec_mem_error_log
(
  const char*  msg,
  unsigned int line_num
);
void ds_sec_mem_error_log_sub
(
  const char*  msg,
  unsigned int line_num,
  uint32       SEC_LOG_SUB_ID
);

/*===========================================================================
                               DEBUG MSG
==========================================================================*/
#ifdef FEATURE_SEC_IPSEC_DEBUG
        #define DS_SEC_MSG4_DEBUG( fmtString, x, y, z, a)\
        {                                                                    \
          MSG_4( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, fmtString, x, y, z, a);\
        }     
        #define DS_SEC_MSG3_DEBUG( fmtString, x, y, z)\
        {                                                                    \
          MSG_3( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, fmtString, x, y, z);          \
        }                                 
        #define DS_SEC_MSG2_DEBUG( fmtString, x, y)                      \
        {                                                                    \
          MSG_2( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, fmtString, x, y);             \
        }
        #define DS_SEC_MSG1_DEBUG( fmtString, x)                         \
        {                                                                    \
          MSG_1( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, fmtString, x);                \
        }
        #define DS_SEC_MSG0_DEBUG( fmtString)                            \
        {                                                                    \
          MSG(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, fmtString);                      \
        }
        #define DS_SEC_MSG3_ERROR_DEBUG( fmtString, x, y, z)                 \
        {                                                                    \
          MSG_3( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, fmtString, x, y, z);         \
        }                                 
        #define DS_SEC_MSG2_ERROR_DEBUG( fmtString, x, y)                    \
        {                                                                    \
          MSG_2( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, fmtString, x, y);            \
        }
        #define DS_SEC_MSG1_ERROR_DEBUG( fmtString, x)                       \
        {                                                                    \
          MSG_1( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, fmtString, x);               \
        }
        #define DS_SEC_MSG0_ERROR_DEBUG( fmtString)                          \
        {                                                                    \
          MSG(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, fmtString);                     \
        }

        /* New macros to pass in subs id as well */
        #define DS_SEC_MSG4_DEBUG_SUB( fmtString, x, y, z, a)                     \
        {                                                                            \
          MSG_4_EXT( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,   \
		  &SEC_LOG_SUB_ID, fmtString, x, y, z, a);       \
        }     
        #define DS_SEC_MSG3_DEBUG_SUB( fmtString, x, y, z)                   \
        {                                                                    \
          MSG_3_EXT( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID, fmtString, x, y, z);          \
        }                                 
        #define DS_SEC_MSG2_DEBUG_SUB( fmtString, x, y)                      \
        {                                                                    \
          MSG_2_EXT( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID, fmtString, x, y);             \
        }
        #define DS_SEC_MSG1_DEBUG_SUB( fmtString, x)                         \
        {                                                                    \
          MSG_1_EXT( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID,  fmtString, x);                \
        }
        #define DS_SEC_MSG0_DEBUG_SUB( fmtString)                            \
        {                                                                    \
          MSG_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_HIGH, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID, fmtString);                      \
        }
        #define DS_SEC_MSG3_ERROR_DEBUG_SUB( fmtString, x, y, z)                 \
        {                                                                    \
          MSG_3_EXT( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID,  fmtString, x, y, z);         \
        }                                 
        #define DS_SEC_MSG2_ERROR_DEBUG_SUB( fmtString, x, y)                    \
        {                                                                    \
          MSG_2_EXT( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID,  fmtString, x, y);            \
        }
        #define DS_SEC_MSG1_ERROR_DEBUG_SUB( fmtString, x)                       \
        {                                                                    \
          MSG_1_EXT( MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID,  fmtString, x);               \
        }
        #define DS_SEC_MSG0_ERROR_DEBUG_SUB( fmtString)                          \
        {                                                                    \
          MSG_EXT(MSG_SSID_SECDIAG_IPSEC, MSG_LEGACY_ERROR, SEC_LOG_EXT_VERSION,                         \
		  &SEC_LOG_SUB_ID, fmtString);                         \
        }

#else 
        #define DS_SEC_MSG4_DEBUG( fmtString, x, y, z, a)       
        #define DS_SEC_MSG3_DEBUG( fmtString, x, y, z)       
        #define DS_SEC_MSG2_DEBUG( fmtString, x, y)
        #define DS_SEC_MSG1_DEBUG( fmtString, x)
        #define DS_SEC_MSG0_DEBUG( fmtString)
        #define DS_SEC_MSG3_ERROR_DEBUG( fmtString, x, y, z)       
        #define DS_SEC_MSG2_ERROR_DEBUG( fmtString, x, y)
        #define DS_SEC_MSG1_ERROR_DEBUG( fmtString, x)
        #define DS_SEC_MSG0_ERROR_DEBUG( fmtString)  

        /* New macros to pass in subs id as well */
        #define DS_SEC_MSG4_DEBUG_SUB( fmtString, x, y, z, a)  
        #define DS_SEC_MSG3_DEBUG_SUB( fmtString, x, y, z)       
        #define DS_SEC_MSG2_DEBUG_SUB( fmtString, x, y)
        #define DS_SEC_MSG1_DEBUG_SUB( fmtString, x)
        #define DS_SEC_MSG0_DEBUG_SUB( fmtString)    
        #define DS_SEC_MSG3_ERROR_DEBUG_SUB( fmtString, x, y, z)       
        #define DS_SEC_MSG2_ERROR_DEBUG_SUB( fmtString, x, y)
        #define DS_SEC_MSG1_ERROR_DEBUG_SUB( fmtString, x)
        #define DS_SEC_MSG0_ERROR_DEBUG_SUB( fmtString)   
  
#endif /* FEATURE_SEC_IPSEC_DEBUG */

#endif /* SECLOGGINGIPSECUTILS_H */


