#ifndef PS_IFACE_LOGICALI_H
#define PS_IFACE_LOGICALI_H
/*===========================================================================
  @file ps_iface_logicali.h

  TODO

  Copyright (c) 2009 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_iface_logicali.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "ps_iface_defs.h"
#include "dcc_task_defs.h"

/*===========================================================================

                         EXTERNAL DATA DECLARATIONS

===========================================================================*/

/*===========================================================================
TYPEDEF PS_LOGICAL_IFACE_EV_CB_CMD_TYPE

DESCRIPTION
  Structure to be used while posting DCC commands. 
===========================================================================*/
typedef struct
{
  ps_iface_event_handle_type    ps_iface_event_handle;
  ps_iface_event_enum_type      ps_iface_event;
  ps_iface_event_info_u_type    ps_iface_event_info;
  ps_iface_event_handle_type    ps_logical_iface_event_handle;
}ps_logical_iface_ev_cb_cmd_type;


/*===========================================================================

                           FORWARD DECLARATIONS

===========================================================================*/

/*===========================================================================

                              EXTERNAL MACROS

===========================================================================*/
#define PS_IFACE_LOGICALI_DEREG_EV_BUF( ps_iface_events,                 \
                                        num_of_events,                   \
                                        ps_iface_ev_buf_ptr)             \
  ps_iface_unsubscribe_event_list( ps_iface_ev_buf_ptr,                  \
                              	   ps_iface_events,                      \
                                   num_of_events);                       \
                                                                         \
  ps_iface_free_event_handle( ps_iface_ev_buf_ptr);                      \
  ps_iface_ev_buf_ptr = NULL;


/*===========================================================================

                              EXTERNAL FUNCTIONS

===========================================================================*/
int32 ps_iface_logicali_reg_iface_all_ev
(
  ps_iface_type  * ps_iface_ptr,
  int16          * ps_errno_ptr
);

int32 ps_iface_logicali_dereg_iface_ev
(
  ps_iface_type  * ps_iface_ptr,
  int16          * ps_errno_ptr
);

void ps_iface_logicali_assoc_iface_ev_cback
(
  ps_iface_type               * ps_iface_ptr,
  ps_iface_event_enum_type      ps_iface_event,
  ps_iface_event_info_u_type    ps_iface_event_info,
  void                        * user_data_ptr
);

int32 ps_iface_logicali_handle_nw_init_qos_flow_enable
(
  ps_iface_type * ps_iface_logical_ptr,
  int16         * ps_errno
);

void ps_iface_logicali_process_assoc_iface_ev
(
  dcc_cmd_enum_type   cmd,
  void              * user_data_ptr
);
#endif /* PS_IFACE_LOGICALI_H */
