
/*!
  @file
  ds_3gpp_flow_ev_cb_hdlr.h

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2014 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_flow_ev_cb_hdlr.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/25/14   vb     Handle flow callback events

===========================================================================*/



#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "rrcdata.h"
#include "ps_iface.h"
#include "ps_iface_defs.h"

/*--------------------------------------------------------------------------- 
  DEFINITIONS
---------------------------------------------------------------------------*/

#define DS_3GPP_MAX_FLOW_EV_CNT 4
#define DS_3GPP_FLOW_MAX_CB_CNT 10 /* Different modules that can register for cb*/

/*-------------------------------------------------------------------------- 
  Enum to hold all the flow EVENTS that will be registered with the Framework
  Any new flow events will need to be added here.
--------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_FLOW_ACTIVATED_EV,
  DS_3GPP_FLOW_MODIFY_ACCEPTED_EV,
  DS_3GPP_FLOW_MODIFY_REJECTED_EV,
  DS_3GPP_FLOW_NULL_EV,
  DS_3GPP_FLOW_MAX_EV = DS_3GPP_MAX_FLOW_EV_CNT
}ds_3gpp_flow_ev_enum_type;

typedef enum
{
  DS_3GPP_FLOW_EV_SUBSCRIBE,
  DS_3GPP_FLOW_EV_UNSUBSCRIBE
}ds_3gpp_flow_ev_func;

/*-------------------------------------------------------------------------- 
  Structure to hold the callback function pointers and the corresponding
  flows
--------------------------------------------------------------------------*/
typedef struct
{
  ps_flow_event_cback_type      cb_fptr;    
  ps_flow_type                 *flow_ptr;
  void                         *user_data_ptr;
}ds_3gpp_flow_ev_cb_data_type;

/*---------------------------------------------------------------------- 
  Queues maintained for every flow event
  The function pointers from the queue will be dispatched to notify the
  event.
 ----------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_flow_ev_cb_data_type *flow_ev_cb_data_ptr[DS_3GPP_FLOW_MAX_CB_CNT];
  uint8                         active_cb_cnt;
}ds_3gpp_flow_ev_cb_queue_type;


/*===========================================================================
FUNCTION DS_3GPP_FLOW_REG_EV

DESCRIPTION 
    This function is to be called when MH modules need to subscribe for a
    3gpp flow event

 
PARAMETERS
     ps_iface_event_enum_type - flow event on which registration needs to happen
     FLow_count
    *cb_fptr;                 - callback function
    *flow_ptr;                - flow_ptr
    *user_data_ptr;           - user_data_ptr
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_flow_reg_ev
(
  ps_iface_event_enum_type       *flow_ev,
  uint8                           flow_ev_cnt,
  ps_flow_event_cback_type        cb_fptr,
  ps_flow_type                   *flow_ptr,
  void                           *user_data_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_DEREG_EV

DESCRIPTION 
    This function is to be called when MH modules need to unsubscribe for a
    3gpp flow event

 
PARAMETERS
    ps_iface_event_enum_type  - flow event on which registration needs to happen
    flow_ev_cnt               - flow ev cnt
    *cb_fptr;                 - callback function
    ps_flow_type              - *flow_ptr,
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_flow_dereg_ev
(
  ps_iface_event_enum_type        *flow_ev_p,
  uint8                            flow_ev_cnt,
  ps_flow_event_cback_type         cb_fptr,
  ps_flow_type                    *flow_ptr
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif
