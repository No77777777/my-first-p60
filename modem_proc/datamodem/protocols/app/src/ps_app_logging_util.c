
/*===========================================================================

                       P S _ A P P _ L O G G I N G . C
                                
DESCRIPTION
  This software unit holds the PS_EAP external APIs to pass down to the EAP
  layer or the Peer layer in the EAP system.


EXTERNALIZED FUNCTIONS

Copyright (c) 2015 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE


  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  $Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_app_logging_util.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $


when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/12/15    fn     Created module

===========================================================================*/

/*===========================================================================

                                INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"         /* Customer Specific Features                */

#ifdef FEATURE_DATA_PS_EAP
#include "ps_app_logging_util.h"
#include "msg.h"
#include "ds_Utils_DebugMsg.h"

/*===========================================================================

                    LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION PS_APP_ERROR_LOG

DESCRIPTION
  Report input parameter errors
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ps_app_error_log
(
  const char*  str,
  unsigned int line_num
#ifdef FEATURE_DATA_APP_LOG_DEBUG
  ,uint32 arg1,
  uint32 arg2,
  uint32 arg3
#endif
)
{
  if (NULL == str)
  {
    return;
  }

#ifdef FEATURE_DATA_APP_LOG_DEBUG
  MSG_SPRINTF_5(MSG_SSID_DS_PS, MSG_LEGACY_ERROR,
        "%s line:%d: Error. arg1 %x arg2 %x arg3 %x", 
        str, line_num, arg1, arg2, arg3);
#else
  MSG_SPRINTF_2(MSG_SSID_DS_PS, MSG_LEGACY_ERROR,
        "%s line:%d: Error", 
        str, line_num);
#endif
}


#endif /* FEATURE_DATA_PS_EAP */
