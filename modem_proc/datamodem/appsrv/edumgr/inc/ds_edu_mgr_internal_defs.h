/*===========================================================================
                      DS_EDU_MGR_INTERNAL_DEFS.H

DESCRIPTION
 Internal Defs header file for EDU_MGR.

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

#ifndef DS_EDU_MGR_INTERNAL_DEFS_H
#define DS_EDU_MGR_INTERNAL_DEFS_H

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_EDU_MGR

#include "ds_edu_mgr_api.h"
#include "ds_edu_mgr_net_if.h"

typedef struct
{
  ds_edu_mgr_event_cb_f_ptr_type  event_cb_f_ptr;
  void                            *user_data_ptr;
} ds_edu_mgr_cb_s_type;

typedef struct
{
  sint15                          nethandle;
  dss_iface_id_type               iface_id;
  sint15                          push_sock_fd;
  uint16                          sock_port;
  ds_edu_mgr_sys_srv_e_type       pref_sys;
  ds_edu_mgr_net_if_state_e_type  net_state;
}ds_edu_mgr_instance_s_type;

extern ds_edu_mgr_instance_s_type  ds_edu_mgr_instance;    // global variable used by mutiple module
extern ds_edu_mgr_cb_s_type        ds_edu_mgr_cb;

#endif   /*FEATURE_DATA_EDU_MGR*/
#endif  /*DS_EDU_MGR_INTERNAL_DEFS_H*/


