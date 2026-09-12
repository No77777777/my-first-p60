/*!
  @file
  ds_3gpp_iface_ev_cb_hdlr.h

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

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/inc/ds_3gpp_iface_ev_cb_hdlr.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/25/14   vb     Handle iface callback events

===========================================================================*/



#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "rrcdata.h"
#include "ps_iface.h"
#include "ps_iface_defs.h"

/*--------------------------------------------------------------------------- 
  DEFINITIONS
---------------------------------------------------------------------------*/

#define DS_3GPP_MAX_IFACE_EV_CNT 14
#define DS_3GPP_IFACE_MAX_CB_CNT  16 /* Different modules that can register for cb*/

/*-------------------------------------------------------------------------- 
  Enum to hold all the IFACE EVENTS that will be registered with the Framework
  Any new iface events will need to be added here.
--------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_IFACE_UP_EV,
  DS_3GPP_IFACE_DOWN_EV,
  DS_3GPP_IFACE_ROUTEABLE_EV,
  DS_3GPP_IFACE_FLOW_ENABLED_EV,
  DS_3GPP_IFACE_FLOW_DISABLED_EV,
  DS_3GPP_IFACE_PREFIX_UPDATE_EV,
  DS_3GPP_IFACE_ACTIVE_OUT_OF_USE_EV,
  DS_3GPP_IFACE_BEARER_TECH_CHANGED_EV,
  DS_3GPP_IFACE_EMBMS_TMGI_ACTIVATED_EV,
  DS_3GPP_IFACE_EMBMS_TMGI_DEACTIVATED_EV,
  DS_3GPP_IFACE_EMBMS_TMGI_ACTIVATE_DEACTIVATE_RESPONSE_EV,
  DS_3GPP_IFACE_EMBMS_ACTIVE_TMGI_LIST_EV,
  DS_3GPP_IFACE_EMBMS_AVAILABLE_TMGI_LIST_EV,
  DS_3GPP_IFACE_EMBMS_AVAILABLE_SAI_LIST_EV,
  DS_3GPP_IFACE_MAX_EV = DS_3GPP_MAX_IFACE_EV_CNT
}ds_3gpp_iface_ev_enum_type;

typedef enum
{
  DS_3GPP_IFACE_EV_REGISTER,
  DS_3GPP_IFACE_EV_DEREGISTER
}ds_3gpp_iface_reg_enum_type;

/*-------------------------------------------------------------------------- 
  Structure to hold the callback function pointers and the corresponding
  ifaces
--------------------------------------------------------------------------*/
typedef struct
{
  ps_iface_event_cback_type      cb_fptr;    
  ps_iface_type                 *iface_ptr;
  void                          *user_data_ptr;
}ds_3gpp_iface_ev_cb_data_type;

/*---------------------------------------------------------------------- 
  Queues maintained for every IFACE event
  The function pointers from the queue will be dispatched to notify the
  event.
 ----------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_iface_ev_cb_data_type *iface_ev_cb_data_ptr[DS_3GPP_IFACE_MAX_CB_CNT];
  uint8                          active_cb_cnt;
}ds_3gpp_iface_ev_cb_queue_type;



/*===========================================================================
FUNCTION DS_3GPP_IFACE_REG_EV

DESCRIPTION 
    This function is to be called when MH modules need to subscribe for a
    3gpp iface event

 
PARAMETERS
     ps_iface_event_enum_type  - iface event on which registration needs to happen
    *cb_fptr;                  - callback function
    *iface_ptr;                - iface_ptr
    *user_data_ptr;            - user_data_ptr
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_iface_reg_ev
(
  ps_iface_event_enum_type        *iface_ev,
  uint8                           iface_ev_cnt,
  ps_iface_event_cback_type       cb_fptr,
  ps_iface_type                   *iface_ptr,
  void                            *user_data_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_IFACE_DEREG_EV

DESCRIPTION 
    This function is to be called when MH modules need to unsubscribe for a
    3gpp iface event

 
PARAMETERS
    ps_iface_event_enum_type   - iface event on which registration needs to happen
    iface_ev_cnt               - iface ev cnt
    *cb_fptr;                  - callback function
    ps_iface_type              - *iface_ptr,
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_iface_dereg_ev
(
  ps_iface_event_enum_type        *iface_ev_p,
  uint8                            iface_ev_cnt,
  ps_iface_event_cback_type        cb_fptr,
  ps_iface_type                   *iface_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_IFACE_EV_CBACK

DESCRIPTION
  This function gets called when any iface event is posted from PS framework
  and posts a command to DS task.
  
PARAMETERS 
  this_iface_ptr:   Iface on which up event was posted
  event:            Event that caused the cback to be invoked
  event_info:       Event info posted by PS
  user_data_ptr:    Data to be passed to the cback function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_iface_ev_cback
(
  ps_iface_type                           *this_iface_ptr,
  ps_iface_event_enum_type                 ps_event,
  ps_iface_event_info_u_type               event_info,
  void                                    *user_data_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_IFACE_EV_CBACK_CMD_HDLR

DESCRIPTION
  This function is the handler for any PS iface event posted from PS framework
  
PARAMETERS 
  cmd_ptr:     Pointer to the command buffer


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_iface_ev_cback_cmd_hdlr
(
   const ds_cmd_type 			   *cmd_ptr
);

