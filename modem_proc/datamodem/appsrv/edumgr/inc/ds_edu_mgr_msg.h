/*===========================================================================
                      DS_EDU_MGR_MSG.H

DESCRIPTION
 MSG header file for EDU_MGR.

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

#ifndef DS_EDU_MGR_MSG_H
#define DS_EDU_MGR_MSG_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_EDU_MGR

#include "dssocket_defs.h"

typedef enum
{
  DS_EDU_MGR_MSG_ID_MIN,
  DS_EDU_MGR_START_LISTEN_IND = DS_EDU_MGR_MSG_ID_MIN,
  DS_EDU_MGR_STOP_LISTEN_IND,
  DS_EDU_MGR_SYS_SERVICE_IND,
  DS_EDU_MGR_NET_EVT_IND,
  DS_EDU_MGR_SOCK_EVT_IND,
  DS_EDU_MGR_MSG_ID_MAX
}ds_edu_mgr_msg_id_e_type;

typedef struct
{
  sint15   nethandle;
  sint15   sock_fd;
  uint32   event_mask;
}ds_edu_mgr_sock_event_s_type;


typedef struct
{
  sint15            nethandle;
  dss_iface_id_type iface_id;
  sint15            err;
}ds_edu_mgr_net_event_s_type;


typedef struct
{
  boolean is_wlan_available;
  boolean is_3gpp_available;
}ds_edu_mgr_sys_event_s_type;


typedef struct
{
  ds_edu_mgr_msg_id_e_type        edu_mgr_msg_id;
  union
  {
    ds_edu_mgr_sys_event_s_type   sys_evt;
    ds_edu_mgr_net_event_s_type   net_evt;
    ds_edu_mgr_sock_event_s_type  sock_evt;
  }msg_content;
}ds_edu_mgr_msg_s_type;

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/
void ds_edu_mgr_msg_init(void);

void ds_edu_mgr_msg_post
(
  ds_edu_mgr_msg_id_e_type msg_id,
  void *msg_content_ptr
);

#endif   /*FEATURE_DATA_EDU_MGR*/
#endif /* DS_EDU_MGR_MSG_H */
