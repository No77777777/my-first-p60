
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

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/inc/ds_3gpp_phys_link_ev_cb_hdlr.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/25/14   vb     Handle phys_link callback events

===========================================================================*/

#ifndef DS_3GPP_PHYS_LINK_EV_CB_HDLR
#define DS_3GPP_PHYS_LINK_EV_CB_HDLR

#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "rrcdata.h"
#include "ps_iface.h"
#include "ps_iface_defs.h"
#include "ps_phys_link.h"
#include "ps_phys_linki_event.h"

/*--------------------------------------------------------------------------- 
  DEFINITIONS
---------------------------------------------------------------------------*/

#define DS_3GPP_MAX_PHYS_LINK_EV_CNT 3
#define DS_3GPP_PHYS_LINK_MAX_CB_CNT 10

/*-------------------------------------------------------------------------- 
  Enum to hold all the PHYS LINK EVENTS that will be registered with the Framework
  Any new phys_link events will need to be added here.
--------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PHYS_LINK_FLOW_ENABLED_EV,
  DS_3GPP_PHYS_LINK_FLOW_DISABLED_EV,
  DS_3GPP_PHYS_LINK_GONE_EV,
  DS_3GPP_PHYS_LINK_MAX_EV = DS_3GPP_MAX_PHYS_LINK_EV_CNT
}ds_3gpp_phys_link_ev_enum_type;

typedef enum
{
  DS_3GPP_PHYS_LINK_EV_SUBSCRIBE,
  DS_3GPP_PHYS_LINK_EV_UNSUBSCRIBE
}ds_3gpp_phys_link_reg_enum_type;

/*-------------------------------------------------------------------------- 
  Structure to hold the callback function pointers and the corresponding
  phys_links
--------------------------------------------------------------------------*/
typedef struct
{
  ps_phys_link_event_cback_type     cb_fptr;    
  ps_phys_link_type                 *phys_link_ptr;                  
  void                              *user_data_ptr;
}ds_3gpp_phys_link_ev_cb_data_type;

/*---------------------------------------------------------------------- 
  Queues maintained for every PHYS_LINK event
  The function pointers from the queue will be dispatched to notify the
  event.
 ----------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_phys_link_ev_cb_data_type *phys_link_ev_cb_data_ptr
                                  [DS_3GPP_PHYS_LINK_MAX_CB_CNT];
  uint8                           active_cb_cnt;
}ds_3gpp_phys_link_ev_cb_queue_type;

/*-------------------------------------------------------------------------- 
  Structure to hold the command that gets posted when the callback function 
  is invoked from PS
--------------------------------------------------------------------------*/
typedef struct
{
    ps_phys_link_type                *phys_link_ptr;
    ps_iface_event_enum_type          ps_event;
    ps_iface_event_info_u_type        event_info;
}ds_3gpp_phys_link_ev_cb_cmd_type;


/*===========================================================================
FUNCTION DS_3GPP_PHYS_LINK_REG_EV

DESCRIPTION 
    This function is to be called when MH modules need to subscribe for a
    3gpp phys_link event

 
PARAMETERS
     ps_3gpp_phys_link_ev_list  - phys_link event on which registration needs to happen
     phys_link_ev_cnt           - phys_link cnt
    *cb_fptr;                   - callback function
    *phys_link_ptr;             - phys_link_ptr
    *user_data_ptr;             - user_data_ptr
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_phys_link_reg_ev
(
  ps_iface_event_enum_type        *phys_link_ev_p,
  uint8                            phys_link_ev_cnt,
  ps_phys_link_event_cback_type    cb_fptr,
  ps_phys_link_type               *phys_link_ptr,
  void                            *user_data_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_PHYS_LINK_DEREG_EV

DESCRIPTION 
    This function is to be called when MH modules need to unsubscribe for a
    3gpp phys_link event

 
PARAMETERS
    ps_phys_link_event_enum_type   - phys_link event on which registration needs to happen
    phys_link_ev_cnt               - phys_link ev cnt
    *cb_fptr;                      - callback function
    ps_phys_link_type              - *phys_link_ptr
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_phys_link_dereg_ev
(
  ps_iface_event_enum_type        *phys_link_ev_p,
  uint8                            phys_link_ev_cnt,
  ps_phys_link_event_cback_type    cb_fptr,
  ps_phys_link_type               *phys_link_ptr
);

#ifdef TEST_FRAMEWORK 
#error code not present
#endif

void ds_3gpp_phys_link_ev_cback_hdlr
(
  const ds_cmd_type      *cmd_ptr
);

#endif /* DS_3GPP_PHYS_LINK_EV_CB_HDLR */
