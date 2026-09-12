#ifndef DS_WLAN_MEAS_CONN_STATUS_H
#define DS_WLAN_MEAS_CONN_STATUS_H
/*===========================================================================

                 WLAN_MEAS_CONN_STATUS HEADER FILE


DESCRIPTION
  This file contains data declarations and function prototypes 
  
EXTERNALIZED FUNCTIONS 
 
 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
4/30/15   youjunf      created
===========================================================================*/


/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "ps_sys.h"
#include "ps_sys_event.h"
/*=========================================================================== 

                      FUNCTION DECLARATIONS
===========================================================================*/


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_CONN_STATUS_INIT

DESCRIPTION   This function initializes wlan connection status module.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/

void ds_wlan_meas_conn_status_init(void);



/*===========================================================================
FUNCTION      DS_WLAN_MEAS_WLAN_SVC_CHG_CB

DESCRIPTION   This callback is called to notify WLAN status change.
  
PARAMETERS 
  tech_type - States Technology type
  event_name - Name of event
  subscription_id - Subscription id
  event_info_ptr - Event related information
  user_data_ptr -   User Data 
    
DEPENDENCIES  None.

RETURN VALUE  None.
 
SIDE EFFECTS  None.
===========================================================================*/
void ds_wlan_meas_wlan_srv_chg_cb
(
  ps_sys_tech_enum_type     tech_type,
  ps_sys_event_enum_type    event_name,
  void                      *event_info_ptr,
  void                      *user_data_ptr
);

#endif /* DS_WLAN_MEAS_CONN_STATUS_H */

