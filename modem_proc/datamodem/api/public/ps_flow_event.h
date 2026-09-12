#ifndef PS_FLOW_EVENT_H
#define PS_FLOW_EVENT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        P S _ F L O W _ E V E N T S . H

DESCRIPTION
  This is the header file for the PS flow event functions.

EXTERNAL FUNCTIONS
  PS_FLOW_ALLOC_EVENT_CBACK_BUF()
    Allocates memory used to register for ps_flow event callbacks

  PS_FLOW_FREE_EVENT_CBACK_BUF()
    Free the memory used to register for ps_flow event callbacks

  PS_FLOW_EVENT_CBACK_REG()
    Registers callbacks for events on a flow

  PS_FLOW_EVENT_CBACK_DEREG()
    Deregisters callbacks for a specific event on an flow

Copyright (c) 2008-2009 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ps_flow_event.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

  when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/14/08    pp     Created module as part of Common Modem Interface:
                   Public/Private API split.
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ps_iface_defs.h"



/*===========================================================================

                         EXTERNAL DATA DECLARATIONS

===========================================================================*/
/*===========================================================================
TYPEDEF PS_FLOW_EVENT_CBACK_TYPE

DESCRIPTION
  Definition of event callback function that can be registered with PS.
  There can be any number of events per flow and the memory is owned by the
  clients.

PARAMS
  flow_ptr      : ptr to flow
  event         : the event being registered for
  event_info    : information specific to the event: for details look at
                  typedef
  user_data_ptr : data to pass back to the user, which is passed during event
                  registration

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
typedef void (*ps_flow_event_cback_type)
(
  ps_flow_type                * flow_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_data_ptr
);

/*===========================================================================
TYPEDEF PS_FLOW_ASSOC_FLOW_EVENT_CBACK_TYPE

DESCRIPTION
  Definition of event callback function that can be registered for
  handling events on the associated flow.

PARAMS
  flow_ptr      : ptr to flow
  event         : the event being registered for
  event_info    : information specific to the event: for details look at
                  typedef
  user_data_ptr : data to pass back to the user, which is passed during event
                  registration

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
typedef void (*ps_flow_assoc_flow_event_cback_type)
(
  ps_flow_type                * assoc_flow_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_data_ptr
);


#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* PS_FLOW_EVEN_H */
