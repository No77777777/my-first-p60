/*===========================================================================
                      DS_EDU_MGR_NET_IF.H

DESCRIPTION
 net_if header file for EDU_MGR.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
10/09/14    Youjunf                          First version of file
===========================================================================*/

#ifndef DS_EDU_MGR_NET_IF_H
#define DS_EDU_MGR_NET_IF_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_EDU_MGR

#include "ds_edu_mgr_msg.h"


/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/
typedef enum
{
  DS_EDU_MGR_NET_IF_STATE_NONE,
  DS_EDU_MGR_NET_IF_STATE_DOWN = DS_EDU_MGR_NET_IF_STATE_NONE,
  DS_EDU_MGR_NET_IF_STATE_UP,
  DS_EDU_MGR_NET_IF_STATE_MAX
}ds_edu_mgr_net_if_state_e_type;

typedef enum
{
  DS_EDU_MGR_SYS_SRV_NONE    = 0x00,
  DS_EDU_MGR_SYS_SRV_3GPP    = 0x01,
  DS_EDU_MGR_SYS_SRV_WLAN_LB = 0x02
}ds_edu_mgr_sys_srv_e_type;


void ds_edu_mgr_net_if_init(void);
void ds_edu_mgr_process_sys_service_event(ds_edu_mgr_msg_s_type *msg_ptr);
void ds_edu_mgr_process_dss_net_event(ds_edu_mgr_msg_s_type   *msg_ptr);
void ds_edu_mgr_process_dss_sock_event(ds_edu_mgr_msg_s_type  *msg_ptr);

#endif   /*FEATURE_DATA_EDU_MGR*/
#endif /* DS_EDU_MGR_NET_IF_H */
