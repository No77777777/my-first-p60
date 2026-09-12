/*===========================================================================
                      DS_KEEPALIVE_ASSIST.H

DESCRIPTION
  header file for keepalive assist.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/aps/inc/ds_keepalive_assist.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $
  
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/01/17   Youjunf First version of file
===========================================================================*/

#ifndef DS_KEEPALIVE_ASSIST_H
#define DS_KEEPALIVE_ASSIST_H

/*===========================================================================
                   INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"


/*===========================================================================
                   EXTERNAL FUNCTIONS
===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif 
void ds_keepalive_assist_init(void);

void ds_keepalive_req_cb
(
  ps_sys_tech_enum_type          tech_type,
  ps_sys_event_enum_type         event_name,
  ps_sys_subscription_enum_type  subscription_id,
  void                           *event_info_ptr,
  void                           *user_data_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* DS_KEEPALIVE_ASSIST_H */
