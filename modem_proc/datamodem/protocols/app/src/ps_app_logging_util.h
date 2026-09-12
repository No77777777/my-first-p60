#ifndef PS_APP_LOGGING_UTIL_H
#define PS_APP_LOGGING_UTIL_H
/*===========================================================================
                           PS_APP_LOGGING_UTIL
===========================================================================*/

/*!
  @file
  ps_app_logging_util.h

  @brief
  This module implements wlan logging util functions

  @detail
*/

/*===========================================================================

  Copyright (c) 2016-2018 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_app_logging_util.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/02/15   fn      Created module
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#include "msg.h"



/*===========================================================================
                             PUBLIC DATA DECLARATIONS
===========================================================================*/
#ifdef FEATURE_DATA_APP_LOG_DEBUG
  #define PS_APP_ERR_LOG_3(arg1, arg2, arg3) \
    ps_app_error_log(__FILENAME__ ,__LINE__, (uint32)arg1, (uint32)arg2, (uint32)arg3)

  #define PS_APP_ERR_LOG_2(arg1, arg2) \
    ps_app_error_log(__FILENAME__ ,__LINE__, (uint32)arg1, (uint32)arg2, 0)

  #define PS_APP_ERR_LOG_1(arg1) \
    ps_app_error_log(__FILENAME__ ,__LINE__, (uint32)arg1, 0, 0)

  #define PS_APP_ERR_LOG_0() \
    ps_app_error_log(__FILENAME__ ,__LINE__,0 ,0 ,0)
#else
  #define PS_APP_ERR_LOG_3(arg1, arg2, arg3) \
    ps_app_error_log(__FILENAME__ ,__LINE__)

  #define PS_APP_ERR_LOG_2(arg1, arg2) \
    ps_app_error_log(__FILENAME__ ,__LINE__)

  #define PS_APP_ERR_LOG_1(arg1) \
    ps_app_error_log(__FILENAME__ ,__LINE__)

  #define PS_APP_ERR_LOG_0() \
    ps_app_error_log(__FILENAME__ ,__LINE__)
#endif 

void ps_app_error_log
(
  const char*  str,
  unsigned int line_num
#ifdef FEATURE_DATA_APP_LOG_DEBUG
  ,uint32 arg1,
  uint32 arg2,
  uint32 arg3
#endif
);

/*===========================================================================
                               DEBUG MSG
==========================================================================*/
#ifdef FEATURE_DATA_APP_LOG_DEBUG
        #define PS_APP_DEBUG_3( fmtString, x, y, z)                   \
        {                                                                    \
          MSG_3( MSG_SSID_DS, MSG_LEGACY_HIGH, fmtString, x, y, z);          \
        }                                 
        #define PS_APP_DEBUG_2( fmtString, x, y)                      \
        {                                                                    \
          MSG_2( MSG_SSID_DS, MSG_LEGACY_HIGH, fmtString, x, y);             \
        }
        #define PS_APP_DEBUG_1( fmtString, x)                         \
        {                                                                    \
          MSG_1( MSG_SSID_DS, MSG_LEGACY_HIGH, fmtString, x);                \
        }
        #define PS_APP_DEBUG_0( fmtString)                            \
        {                                                                    \
          MSG(MSG_SSID_DS, MSG_LEGACY_HIGH, fmtString);                    \
        }
        #define PS_APP_ERR_DEBUG_3( fmtString, x, y, z)                 \
        {                                                                    \
          MSG_3( MSG_SSID_DS, MSG_LEGACY_ERROR, fmtString, x, y, z);         \
        }                                 
        #define PS_APP_ERR_DEBUG_2( fmtString, x, y)                    \
        {                                                                    \
          MSG_2( MSG_SSID_DS, MSG_LEGACY_ERROR, fmtString, x, y);            \
        }
        #define PS_APP_ERR_DEBUG_1( fmtString, x)                       \
        {                                                                    \
          MSG_1( MSG_SSID_DS, MSG_LEGACY_ERROR, fmtString, x);               \
        }
        #define PS_APP_ERR_DEBUG_0( fmtString)                          \
        {                                                                    \
          MSG(MSG_SSID_DS, MSG_LEGACY_ERROR, fmtString);                   \
        }
        #define PS_APP_SPRINTF_DEBUG_1( fmtString, x)                    \
        {                                                                \
          MSG_SPRINTF_1(MSG_SSID_DS, MSG_LEGACY_MED, fmtString, x);    \
        }
          
#else 
        #define PS_APP_DEBUG_3( fmtString, x, y, z)       
        #define PS_APP_DEBUG_2( fmtString, x, y)
        #define PS_APP_DEBUG_1( fmtString, x)
        #define PS_APP_DEBUG_0( fmtString)
        #define PS_APP_ERR_DEBUG_3( fmtString, x, y, z)       
        #define PS_APP_ERR_DEBUG_2( fmtString, x, y)
        #define PS_APP_ERR_DEBUG_1( fmtString, x)
        #define PS_APP_ERR_DEBUG_0( fmtString)
        #define PS_APP_SPRINTF_DEBUG_1( fmtString, x)      
#endif /* FEATURE_DATA_APP_LOG_DEBUG */


#endif /* PS_APP_LOGGING_UTIL_H */
