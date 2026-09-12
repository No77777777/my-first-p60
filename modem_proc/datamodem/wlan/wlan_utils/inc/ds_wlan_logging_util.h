#ifndef DS_WLAN_LOGGING_UTIL_H
#define DS_WLAN_LOGGING_UTIL_H
/*===========================================================================
                           DS_WLAN_LOGGING_UTIL
===========================================================================*/

/*!
  @file
  ds_wlan_logging_util.h

  @brief
  This module implements wlan logging util functions

  @detail
*/

/*===========================================================================

  Copyright (c) 2016-2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/wlan/wlan_utils/inc/ds_wlan_logging_util.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/02/15   fn      Created module
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_WLAN_MAPCON
#include "msg.h"

/*===========================================================================
                             PUBLIC DATA DECLARATIONS
===========================================================================*/
//#define FEATURE_DATA_WLAN_MAPCON_DEBUG (1)

/*===========================================================================
  DS WLAN SS related macros
===========================================================================*/

/*---------------------------------------------------------------------------
  Macro for messages with no parameters.
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG0_MED( xx_fmt) \
  MSG(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, xx_fmt)

#define DATA_WLAN_MSG0_HIGH( xx_fmt) \
  MSG(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, xx_fmt)

#define DATA_WLAN_MSG0_ERROR( xx_fmt) \
  MSG(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, xx_fmt)

/*---------------------------------------------------------------------------
  Macro for messages with 1 parameters.
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG1_MED(xx_fmt, xx_arg1) \
  MSG_1(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, xx_fmt, xx_arg1)

#define DATA_WLAN_MSG1_HIGH(xx_fmt, xx_arg1) \
  MSG_1(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, xx_fmt, xx_arg1)

#define DATA_WLAN_MSG1_ERROR(xx_fmt, xx_arg1) \
  MSG_1(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, xx_fmt, xx_arg1)

/*---------------------------------------------------------------------------
  Macro for messages with 2 parameters.
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG2_MED(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2)

#define DATA_WLAN_MSG2_HIGH(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2)

#define DATA_WLAN_MSG2_ERROR(xx_fmt, xx_arg1, xx_arg2) \
  MSG_2(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  Macro for messages with 3 parameters.
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG3_MED(xx_fmt, xx_arg1, xx_arg2, xx_arg) \
  MSG_3(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, xx_arg)

#define DATA_WLAN_MSG3_HIGH(xx_fmt, xx_arg1, xx_arg2, xx_arg) \
  MSG_3(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg)

#define DATA_WLAN_MSG3_ERROR(xx_fmt, xx_arg1, xx_arg2, xx_arg) \
  MSG_3(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, xx_arg)

/*---------------------------------------------------------------------------
  Macro for messages with 4 parameters.
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG4_MED(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg )\
  MSG_4(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg)

#define DATA_WLAN_MSG4_HIGH(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg )\
  MSG_4(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg)

#define DATA_WLAN_MSG4_ERROR(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg) \
  MSG_4(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg)

/*---------------------------------------------------------------------------
  Macro for messages with 5 parameters.
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG5_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg )\
  MSG_5(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg)

#define DATA_WLAN_MSG5_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg )\
  MSG_5(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,  xx_arg)

#define DATA_WLAN_MSG5_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg) \
  MSG_5(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg)

/*---------------------------------------------------------------------------
  Macro for messages with 6 parameters.
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG6_MED(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg )\
  MSG_6(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg)

#define DATA_WLAN_MSG6_HIGH(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg )\
  MSG_6(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg)

#define DATA_WLAN_MSG6_ERROR(xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4, xx_arg5, xx_arg) \
  MSG_6(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, xx_fmt, xx_arg1, xx_arg2, \
        xx_arg3, xx_arg4,xx_arg5, xx_arg)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 1 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG_SPRINTF_1(xx_ss_mask, xx_fmt, xx_arg1) \
  MSG_SPRINTF_1(MSG_SSID_DS_WLAN, xx_ss_mask, xx_fmt, xx_arg1)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 2 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG_SPRINTF_2(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2) \
  MSG_SPRINTF_2(MSG_SSID_DS_WLAN, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 3 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG_SPRINTF_3(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_SPRINTF_3(MSG_SSID_DS_WLAN, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 4 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG_SPRINTF_4(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  MSG_SPRINTF_4(MSG_SSID_DS_WLAN, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 5 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG_SPRINTF_5(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5) \
  MSG_SPRINTF_5(MSG_SSID_DS_WLAN, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 6 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG_SPRINTF_6(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6) \
  MSG_SPRINTF_6(MSG_SSID_DS_WLAN, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 7 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_MSG_SPRINTF_7(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, \
   xx_arg4, xx_arg5, xx_arg6, xx_arg7) \
  MSG_SPRINTF_7(MSG_SSID_DS_WLAN, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
   xx_arg4, xx_arg5, xx_arg6, xx_arg7)

/*---------------------------------------------------------------------------
  Macro for messages with subs_id parameter
---------------------------------------------------------------------------*/
#define DATA_WLAN_LOG_EXT_VERSION 1

#define WLAN_DATA_SYS_TO_LOG_SUBS(sys_subs_id) ((uint32)(sys_subs_id+1))

#define DATA_WLAN_SUB0_MED(fmtString)    \
  MSG_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString)

#define DATA_WLAN_SUB0_HIGH(fmtString)    \
  MSG_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString)

#define DATA_WLAN_SUB0_ERROR(fmtString)    \
  MSG_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString)

#define DATA_WLAN_SUB1_MED(fmtString, x)    \
  MSG_1_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x)

#define DATA_WLAN_SUB1_HIGH(fmtString, x)    \
  MSG_1_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x)

#define DATA_WLAN_SUB1_ERROR(fmtString, x)    \
  MSG_1_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x)

#define DATA_WLAN_SUB2_MED(fmtString, x, y)    \
  MSG_2_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y)

#define DATA_WLAN_SUB2_HIGH(fmtString, x, y)    \
  MSG_2_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y)

#define DATA_WLAN_SUB2_ERROR(fmtString, x, y)    \
  MSG_2_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y)

#define DATA_WLAN_SUB3_MED(fmtString, x, y, z)    \
  MSG_3_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z)

#define DATA_WLAN_SUB3_HIGH(fmtString, x, y, z)    \
  MSG_3_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z)

#define DATA_WLAN_SUB3_ERROR(fmtString, x, y, z)    \
  MSG_3_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z)

#define DATA_WLAN_SUB4_MED(fmtString, x, y, z, a)    \
  MSG_4_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z, a)

#define DATA_WLAN_SUB4_HIGH(fmtString, x, y, z, a)    \
  MSG_4_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z, a)

#define DATA_WLAN_SUB4_ERROR(fmtString, x, y, z, a)    \
  MSG_4_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z, a)

#define DATA_WLAN_SUB5_MED(fmtString, x, y, z, a, b)    \
  MSG_5_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_MED, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z, a, b)

#define DATA_WLAN_SUB5_HIGH(fmtString, x, y, z, a, b)    \
  MSG_5_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z, a, b)

#define DATA_WLAN_SUB5_ERROR(fmtString, x, y, z, a, b)    \
  MSG_5_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z, a, b)

#ifndef TEST_FRAMEWORK 
/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 0 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF(xx_ss_mask, xx_fmt)        \
  MSG_SPRINTF_1_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, xx_fmt, 0)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 1 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF_1(xx_ss_mask, xx_fmt, xx_arg1)        \
  MSG_SPRINTF_1_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID, xx_fmt, xx_arg1)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 2 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF_2(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2) \
  MSG_SPRINTF_2_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 3 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF_3(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  MSG_SPRINTF_3_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 4 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF_4(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  MSG_SPRINTF_4_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 5 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF_5(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5) \
  MSG_SPRINTF_5_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5)


/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 6 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF_6(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6) \
  MSG_SPRINTF_6_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6)

/*---------------------------------------------------------------------------
  This is the macro for sprintf messages with 7 parameters
---------------------------------------------------------------------------*/
#define DATA_WLAN_SUB_SPRINTF_7(xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7) \
  MSG_SPRINTF_7_EXT(MSG_SSID_DS_WLAN, (xx_ss_mask), DATA_WLAN_LOG_EXT_VERSION,    \
    &WLAN_DATA_LOG_SUB_ID,xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7)
#else
#error code not present
#endif /* TEST_FRAMEWORK */


/*---------------------------------------------------------------------------
  Debug F3 log messages 
---------------------------------------------------------------------------*/

/*===========================================================================

FUNCTION WLAN_INPUT_ERROR_LOG

DESCRIPTION
  Report input validation failures on parameters
     msg - a const char msg. Typical inputs uses __FILE__ or __FUNCTION__
     line_num - line in code of where error occured
     arg1 - arguements
     arg2 - arguements
     arg3 - arguements
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
#define WLAN_INPUT_ERROR_LOG(arg1, arg2, arg3) \
        wlan_input_error_log(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, (uint32)arg3)

#define WLAN_INPUT_ERROR_LOG_2(arg1, arg2) \
        wlan_input_error_log(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, 0)

#define WLAN_INPUT_ERROR_LOG_1(arg1) \
        wlan_input_error_log(__FILENAME__, __LINE__, (uint32)arg1, 0, 0)

#define WLAN_INPUT_ERROR_LOG_0() \
        wlan_input_error_log(__FILENAME__, __LINE__, 0, 0, 0)

#define WLAN_INPUT_ERROR_LOG_SUB(arg1, arg2, arg3) \
        wlan_input_error_log_sub(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, (uint32)arg3, WLAN_DATA_LOG_SUB_ID)

#define WLAN_INPUT_ERROR_LOG_2_SUB(arg1, arg2) \
        wlan_input_error_log_sub(__FILENAME__, __LINE__, (uint32)arg1, (uint32)arg2, 0, WLAN_DATA_LOG_SUB_ID)

#define WLAN_INPUT_ERROR_LOG_1_SUB(arg1) \
        wlan_input_error_log_sub(__FILENAME__, __LINE__, (uint32)arg1, 0, 0, WLAN_DATA_LOG_SUB_ID)

#define WLAN_INPUT_ERROR_LOG_0_SUB() \
        wlan_input_error_log_sub(__FILENAME__, __LINE__, 0, 0, 0, WLAN_DATA_LOG_SUB_ID)

void wlan_input_error_log
(
  const char*  msg,
  unsigned int line_num,
  uint32 arg1,
  uint32 arg2,
  uint32 arg3
);

void wlan_input_error_log_sub
(
  const char*  msg,
  unsigned int line_num,
  uint32 arg1,
  uint32 arg2,
  uint32 arg3,
  uint32 WLAN_DATA_LOG_SUB_ID
);

/*===========================================================================

FUNCTION WLAN_MEM_ERROR_LOG

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
#define WLAN_MEM_ERROR_LOG() \
        wlan_mem_error_log(__FILENAME__, __LINE__)

#define WLAN_MEM_ERROR_LOG_SUB() \
        wlan_mem_error_log_sub(__FILENAME__, __LINE__, WLAN_DATA_LOG_SUB_ID)

void wlan_mem_error_log
(
  const char*  msg,
  unsigned int line_num
);
void wlan_mem_error_log_sub
(
  const char*  msg,
  unsigned int line_num,
  uint32       WLAN_DATA_LOG_SUB_ID
);

#define DATA_WLAN_LOG_EXT_VERSION 1

/*===========================================================================
                               DEBUG MSG
==========================================================================*/
#ifdef FEATURE_DATA_WLAN_MAPCON_DEBUG
        #define WLAN_DATA_MSG_DEBUG_4( fmtString, x, y, z, a)                \
        {                                                                    \
          MSG_4( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, fmtString, x, y, z, a);       \
        }     
        #define WLAN_DATA_MSG_DEBUG_3( fmtString, x, y, z)                   \
        {                                                                    \
          MSG_3( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, fmtString, x, y, z);          \
        }                                 
        #define WLAN_DATA_MSG_DEBUG_2( fmtString, x, y)                      \
        {                                                                    \
          MSG_2( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, fmtString, x, y);             \
        }
        #define WLAN_DATA_MSG_DEBUG_1( fmtString, x)                         \
        {                                                                    \
          MSG_1( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, fmtString, x);                \
        }
        #define WLAN_DATA_MSG_DEBUG_0( fmtString)                            \
        {                                                                    \
          MSG(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, fmtString);                      \
        }
        #define WLAN_DATA_ERROR_DEBUG_3( fmtString, x, y, z)                 \
        {                                                                    \
          MSG_3( MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, fmtString, x, y, z);         \
        }                                 
        #define WLAN_DATA_ERROR_DEBUG_2( fmtString, x, y)                    \
        {                                                                    \
          MSG_2( MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, fmtString, x, y);            \
        }
        #define WLAN_DATA_ERROR_DEBUG_1( fmtString, x)                       \
        {                                                                    \
          MSG_1( MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, fmtString, x);               \
        }
        #define WLAN_DATA_ERROR_DEBUG_0( fmtString)                          \
        {                                                                    \
          MSG(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, fmtString);                     \
        }

        /* New macros to pass in subs id as well */
        #define WLAN_DATA_SUB_DEBUG_4( fmtString, x, y, z, a)                     \
        {                                                                            \
          MSG_4_EXT( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,   \
		  &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z, a);       \
        }     
        #define WLAN_DATA_SUB_DEBUG_3( fmtString, x, y, z)                   \
        {                                                                    \
          MSG_3_EXT( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID, fmtString, x, y, z);          \
        }                                 
        #define WLAN_DATA_SUB_DEBUG_2( fmtString, x, y)                      \
        {                                                                    \
          MSG_2_EXT( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID, fmtString, x, y);             \
        }
        #define WLAN_DATA_SUB_DEBUG_1( fmtString, x)                         \
        {                                                                    \
          MSG_1_EXT( MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID,  fmtString, x);                \
        }
        #define WLAN_DATA_SUB_DEBUG_0( fmtString)                            \
        {                                                                    \
          MSG_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_HIGH, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID, fmtString);                      \
        }
        #define WLAN_DATA_ERROR_DEBUG_3_SUB( fmtString, x, y, z)                 \
        {                                                                    \
          MSG_3_EXT( MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID,  fmtString, x, y, z);         \
        }                                 
        #define WLAN_DATA_ERROR_DEBUG_2_SUB( fmtString, x, y)                    \
        {                                                                    \
          MSG_2_EXT( MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID,  fmtString, x, y);            \
        }
        #define WLAN_DATA_ERROR_DEBUG_1_SUB( fmtString, x)                       \
        {                                                                    \
          MSG_1_EXT( MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID,  fmtString, x);               \
        }
        #define WLAN_DATA_ERROR_DEBUG_0_SUB( fmtString)                          \
        {                                                                    \
          MSG_EXT(MSG_SSID_DS_WLAN, MSG_LEGACY_ERROR, DATA_WLAN_LOG_EXT_VERSION,                         \
		  &WLAN_DATA_LOG_SUB_ID, fmtString);                         \
        }
#else 
        #define WLAN_DATA_MSG_DEBUG_4( fmtString, x, y, z, a)       
        #define WLAN_DATA_MSG_DEBUG_3( fmtString, x, y, z)       
        #define WLAN_DATA_MSG_DEBUG_2( fmtString, x, y)
        #define WLAN_DATA_MSG_DEBUG_1( fmtString, x)
        #define WLAN_DATA_MSG_DEBUG_0( fmtString)
        #define WLAN_DATA_ERROR_DEBUG_3( fmtString, x, y, z)       
        #define WLAN_DATA_ERROR_DEBUG_2( fmtString, x, y)
        #define WLAN_DATA_ERROR_DEBUG_1( fmtString, x)
        #define WLAN_DATA_ERROR_DEBUG_0( fmtString)  

        /* New macros to pass in subs id as well */
        #define WLAN_DATA_SUB_DEBUG_4( fmtString, x, y, z, a)       
        #define WLAN_DATA_SUB_DEBUG_3( fmtString, x, y, z)       
        #define WLAN_DATA_SUB_DEBUG_2( fmtString, x, y)
        #define WLAN_DATA_SUB_DEBUG_1( fmtString, x)
        #define WLAN_DATA_SUB_DEBUG_0( fmtString)
        #define WLAN_DATA_ERROR_DEBUG_3_SUB( fmtString, x, y, z)       
        #define WLAN_DATA_ERROR_DEBUG_2_SUB( fmtString, x, y)
        #define WLAN_DATA_ERROR_DEBUG_1_SUB( fmtString, x)
        #define WLAN_DATA_ERROR_DEBUG_0_SUB( fmtString)              
#endif /* FEATURE_DATA_WLAN_MAPCON_DEBUG */

#endif /* FEATURE_DATA_WLAN_MAPCON */
#endif /* DS_WLAN_LOGGING_UTIL_H */
