/*===========================================================================
  FILE: ds_Sock_EventManager.cpp

  OVERVIEW: This file provides implementation EventManager class.

  DEPENDENCIES: None

  Copyright (c) 2008-2009 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/


/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_EventManager.cpp#3 $
  $DateTime: 2021/10/05 03:31:51 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-02 hm  Created module.

===========================================================================*/

/*===========================================================================

                              INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "target.h"

extern "C"
{
#include "ps_svc.h"
#include "ps_logging_helper.h"
}

#include "ds_Utils_CSSupport.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Sock_EventManager.h"
#include "ds_Sock_EventDefs.h"
#include "ds_Sock_SocketFactory.h"
#include "ds_Sock_Socket.h"

#include "ps_iface.h"
#include "ps_flowi_event.h"
#include "ps_phys_link.h"
#include "ps_mem.h"
#include "ps_rt_meta_info.h"
#include "ps_pkt_meta_info.h"
#include "ps_system_heap.h"
#include "ps_utils.h"
#include "ps_ifacei_event.h"
#include "ps_phys_linki_event.h"

using namespace ds::Sock;
using namespace ds::Utils;
using namespace PS::Sock;

#define DS_SOCK_EVENTMGR_ASSERT(expression) \
         ds_sock_eventmgr_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION ds_sock_eventmgr_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_SOCK_EVENTMGR_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_sock_eventmgr_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DSNET_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_sock_eventmgr_assert_wrapper */



/*===========================================================================

                        PRIVATE DATA DECLARATIONS

===========================================================================*/

static struct
{
  void * psIfaceEvHandlePtr;
  void * psIfaceIPFltrEvHandlePtr;
  void * psFlowEvHandlePtr;
  void * physLinkEvHandlePtr;
  void * physLinkDosAckEvHandlePtr;
} EventHandleBuf;

/*===========================================================================

                        PRIVATE FUNCTION DEFINITIONS

===========================================================================*/
void PSIfaceEventCback
(
  ps_iface_type *             psIfacePtr,
  ps_iface_event_enum_type    psIfaceEvent,
  ps_iface_event_info_u_type  psIfaceEventInfo,
  void *                      userDataPtr
)
{
  ds::Sock::Event::EventInfoType *  sockEventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Propagate event only if IPv6 prefix is removed. Socket doesn't care about
    other type of prefix updates
  -------------------------------------------------------------------------*/
  if (IFACE_PREFIX_UPDATE_EV == psIfaceEvent &&
      PREFIX_REMOVED != psIfaceEventInfo.prefix_info.kind)
  {
    LOG_MSG_FUNCTION_EXIT_2("PSIfaceEventCback(): "
                            "Ignoring ev %d kind %d",
                            psIfaceEvent, psIfaceEventInfo.prefix_info.kind);
    return;
  }

  /*-------------------------------------------------------------------------
    Allocate memory and populate socket event info.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC( sockEventInfoPtr, sizeof( ds::Sock::Event::EventInfoType),
                            ds::Sock::Event::EventInfoType*);

  if (NULL == sockEventInfoPtr)
  {
    LOG_MSG_ERROR_1("PSIfaceEventCback(): "
                    "Out of memory, cannot alloc buf to process ev %d",
                    psIfaceEvent);
    return;
  }

  memset (sockEventInfoPtr, 0, sizeof( ds::Sock::Event::EventInfoType));

  sockEventInfoPtr->eventGroup = ds::Sock::Event::EVENT_GROUP_NETWORK;
  sockEventInfoPtr->eventMask  = (int32) psIfaceEvent;
  sockEventInfoPtr->handle     = reinterpret_cast <int32> ( psIfacePtr);

  if (IFACE_ADDR_CHANGED_EV == psIfaceEvent)
  {
    memscpy(&sockEventInfoPtr->eventInfo,
            sizeof(ps_iface_event_info_u_type),
            &psIfaceEventInfo,
            sizeof(ps_iface_event_info_u_type));
  }

  /*-------------------------------------------------------------------------
    Post the command to PS task.
  -------------------------------------------------------------------------*/
  ps_send_cmd( PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD, (void*)sockEventInfoPtr);

  return;

} /* PSIfaceEventCback() */

void PSFlowEventCback
(
  ps_flow_type *              psFlowPtr,
  ps_iface_event_enum_type    psFlowEvent,
  ps_iface_event_info_u_type  psFlowEventInfo,
  void *                      userDataPtr
)
{
  ds::Sock::Event::EventInfoType *  sockEventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  DS_INTERFACE_LOG_MSG_INFO1_2(PS_LOGGING_CAT_SOCKET, "PSFlowEventCback(): Ev %d PSFlow 0x%p", 
                               psFlowEvent, psFlowPtr);

  /*-------------------------------------------------------------------------
    Allocate memory and populate socket event info.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(sockEventInfoPtr, sizeof( ds::Sock::Event::EventInfoType),
                           ds::Sock::Event::EventInfoType*);

  if (NULL == sockEventInfoPtr)
  {
    LOG_MSG_ERROR_1("PSFlowEventCback(): "
                    "Out of memory, cannot alloc buf to process ev %d",
                    psFlowEvent);
    return;
  }

  sockEventInfoPtr->eventGroup = ds::Sock::Event::EVENT_GROUP_NETWORK;
  sockEventInfoPtr->eventMask  = (int32) psFlowEvent;
  sockEventInfoPtr->handle     = reinterpret_cast <int32> ( psFlowPtr);

  /*-------------------------------------------------------------------------
    Post the command to PS task.
  -------------------------------------------------------------------------*/
  ps_send_cmd( PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD, (void*)sockEventInfoPtr);


  return;
} /* PSFlowEventCback() */


void PhysLinkEventCback
(
  ps_phys_link_type *         physLinkPtr,
  ps_iface_event_enum_type    physLinkEvent,
  ps_iface_event_info_u_type  physLinkEventInfo,
  void *                      userDataPtr
)
{
  ds::Sock::Event::EventInfoType *    sockEventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2("PhysLinkEventCback(): "
                           "Ev %d PhysLink 0x%x", physLinkEvent, physLinkPtr);


  /*-------------------------------------------------------------------------
    Allocate memory and populate socket event info.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(sockEventInfoPtr, sizeof( ds::Sock::Event::EventInfoType),
                           ds::Sock::Event::EventInfoType*);

  if (NULL == sockEventInfoPtr)
  {
    LOG_MSG_ERROR_1("PhysLinkEventCback(): "
                    "Out of memory, cannot alloc buf to process ev %d",
                    physLinkEvent);
    return;
  }

  sockEventInfoPtr->eventGroup = ds::Sock::Event::EVENT_GROUP_NETWORK;
  sockEventInfoPtr->eventMask  = (int32) physLinkEvent;
  sockEventInfoPtr->handle     = reinterpret_cast <int32> ( physLinkPtr);

  /*-------------------------------------------------------------------------
    Post the command to PS task.
  -------------------------------------------------------------------------*/
  ps_send_cmd( PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD, (void*)sockEventInfoPtr);

  return;
} /* PhysLinkEventCback() */


void PhysLinkDoSAckEventCback
(
  ps_phys_link_type *         physLinkPtr,
  ps_iface_event_enum_type    physLinkEvent,
  ps_iface_event_info_u_type  physLinkEventInfo,
  void *                      userDataPtr
)
{
  ds::Sock::Event::DoSAckEventInfoType *   dosAckEventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_1("PhysLinkDoSAckEventCback(): "
                           "PhysLink 0x%x", physLinkPtr);

  /*-------------------------------------------------------------------------
    Allocate memory and populate socket event info.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC( dosAckEventInfoPtr,
                           sizeof( ds::Sock::Event::DoSAckEventInfoType),
                           ds::Sock::Event::DoSAckEventInfoType*);

  if (NULL == dosAckEventInfoPtr)
  {
    LOG_MSG_ERROR_1("PhysLinkDoSAckEventCback(): "
                    "Out of memory, cannot alloc buf to process ev %d",
                    physLinkEvent);
    return;
  }

  dosAckEventInfoPtr->eventGroup = ds::Sock::Event::EVENT_GROUP_NETWORK;
  dosAckEventInfoPtr->eventMask  = (int32) physLinkEvent;
  dosAckEventInfoPtr->handle     = physLinkEventInfo.dos_ack_info.handle;

  dosAckEventInfoPtr->dosAckStatus =
    (DoSAckStatusType) physLinkEventInfo.dos_ack_info.status_info.status;

  dosAckEventInfoPtr->overflow =
    physLinkEventInfo.dos_ack_info.status_info.overflow;

  /*-------------------------------------------------------------------------
    Post the command to PS task.
  -------------------------------------------------------------------------*/
  ps_send_cmd( PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD, (void*)dosAckEventInfoPtr);

  return;
} /* PhysLinkDoSAckEventCback() */


void PSIfaceIPFltrEventCback
(
  ps_iface_type *             psIfacePtr,
  ps_iface_event_enum_type    psIfaceEvent,
  ps_iface_event_info_u_type  psIfaceEventInfo,
  void *                      userDataPtr
)
{
  ds::Sock::Event::IPFilterEventInfoType *  ipFltrEventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2("PSIfaceIPFltrEventCback(): "
                           "Ev %d Iface 0x%x", psIfaceEvent, psIfacePtr);

  /*-------------------------------------------------------------------------
    Allocate memory and populate socket event info.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(ipFltrEventInfoPtr, 
                           sizeof(ds::Sock::Event::IPFilterEventInfoType),
                           ds::Sock::Event::IPFilterEventInfoType*);
  if (NULL == ipFltrEventInfoPtr)
  {
    return;
  }

  ipFltrEventInfoPtr->eventGroup = ds::Sock::Event::EVENT_GROUP_NETWORK;
  ipFltrEventInfoPtr->eventMask  = (int32) psIfaceEvent;
  ipFltrEventInfoPtr->handle     = reinterpret_cast <int32> ( psIfacePtr);
  ipFltrEventInfoPtr->clientID   = psIfaceEventInfo.ipfltr_info.client;

  /*-------------------------------------------------------------------------
    Post the command to PS task.
  -------------------------------------------------------------------------*/
  ps_send_cmd( PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD, (void*)ipFltrEventInfoPtr);

  return;

} /* PSIfaceIPFltrEventCback() */


void PSMemEventCback
(
  int16  wmVal
)
{
  ds::Sock::Event::EventInfoType *  sockEventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Allocate memory and populate socket event info.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC( sockEventInfoPtr, sizeof( ds::Sock::Event::EventInfoType),
                            ds::Sock::Event::IPFilterEventInfoType*);

  if (NULL == sockEventInfoPtr)
  {
    LOG_MSG_ERROR_1("PSMemEventCback(): "
                    "Out of memory, cannot process ps mem ev, WM %d", wmVal);
    return;
  }

  sockEventInfoPtr->eventGroup = ds::Sock::Event::EVENT_GROUP_PS_MEM;
  sockEventInfoPtr->eventMask  = (int32) PSMemEvent::PS_MEM_BUF_AVAILABLE;
  sockEventInfoPtr->handle     = 0;

  /*-------------------------------------------------------------------------
    Post the command to PS task.
  -------------------------------------------------------------------------*/
  ps_send_cmd( PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD, (void*)sockEventInfoPtr);

  return;

} /* PSMemEventCback() */

static void ProcessGenericEventCmdHandler
(
  ps_cmd_enum_type    cmd,
  void *              userDataPtr
)
{
  SocketFactory *                     sockFactoryPtr;
  ds::Sock::Event::EventInfoType *    sockEventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == userDataPtr || PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD != cmd)
  {
    LOG_MSG_INVALID_INPUT_2("ProcessGenericEventCmdHandler(): "
                            "Invalid args: cmd %d, event info 0x%p",
                            cmd, userDataPtr);
    DS_SOCK_EVENTMGR_ASSERT( 0);
    return;
  }

  /*-------------------------------------------------------------------------
    Get the event info
  -------------------------------------------------------------------------*/
  sockEventInfoPtr =
    reinterpret_cast <ds::Sock::Event::EventInfoType *> ( userDataPtr);

  DS_INTERFACE_LOG_MSG_INFO1_3(PS_LOGGING_CAT_SOCKET, "ProcessGenericEventCmdHandler(): "
                  "Processing ev %d, group %d, handle 0x%x",
                  (int)sockEventInfoPtr->eventMask,
                  sockEventInfoPtr->eventGroup,
                  (unsigned int)sockEventInfoPtr->handle);

  /*-------------------------------------------------------------------------
    Relay events to all the sockets in socket factory
  -------------------------------------------------------------------------*/
  sockFactoryPtr = SocketFactory::CreateInstance();
  DS_SOCK_EVENTMGR_ASSERT( 0 != sockFactoryPtr);

  (void) sockFactoryPtr->Traverse( sockEventInfoPtr);
  DS_UTILS_RELEASEIF( sockFactoryPtr);

  /*-------------------------------------------------------------------------
    Free the memory allocated for socket event info.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE( sockEventInfoPtr);

} /* ProcessGenericEventCmdHandler() */


void ds::Sock::EventManager::Init
(
  void
)
{
  int32  retVal;
  /*-------------------------------------------------------------------------
    Note - To subscribe/register for a event, just add it in appropriate
    array 
  -------------------------------------------------------------------------*/

  ps_iface_event_enum_type	ifaceEvents[] = 
  {
    IFACE_ADDR_CHANGED_EV,
    IFACE_ADDR_FAMILY_CHANGED_EV,
    IFACE_PREFIX_UPDATE_EV,
    IFACE_ROUTEABLE_EV,
    IFACE_UP_EV,
    IFACE_DOWN_EV,
    IFACE_CONFIGURING_EV,
    IFACE_LINGERING_EV,
    IFACE_GOING_DOWN_EV,
    IFACE_FLOW_ENABLED_EV,
    IFACE_FLOW_DISABLED_EV
  };

  ps_iface_event_enum_type	ifaceIpFltrEvents [] = 
  {
    IFACE_IPFLTR_UPDATED_EV
  };


  ps_iface_event_enum_type	flowEvents[] = 
  {
    FLOW_ACTIVATED_EV,
    FLOW_SUSPENDED_EV,
    FLOW_TX_ENABLED_EV,
    FLOW_TX_DISABLED_EV
  };

  ps_iface_event_enum_type	physLinkEvents [] = 
  {
    PHYS_LINK_UP_EV,
    PHYS_LINK_DOWN_EV,
    PHYS_LINK_FLOW_ENABLED_EV,
    PHYS_LINK_FLOW_DISABLED_EV
  };

  ps_iface_event_enum_type	physLinkDosAckEvents [] = 
  {
    PHYS_LINK_707_DOS_ACK_EV
  };

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Register command handlers. All iface/flow/phys_link/PS_MEM/DOS_ACK
    events are processed in PS task context.
  -------------------------------------------------------------------------*/
  (void) ps_set_cmd_handler( PS_DSSOCK_PROCESS_GENERIC_EVENT_CMD,
                             ProcessGenericEventCmdHandler);


 /*--------------------------------------------------------------------------
   Subscribe to list of events on iface
 --------------------------------------------------------------------------*/

  EventHandleBuf.psIfaceEvHandlePtr = ps_iface_alloc_event_handle
    ( NULL, PSIfaceEventCback, NULL);

  if (NULL == EventHandleBuf.psIfaceEvHandlePtr)
  {
    goto bail;
  }

  retVal = ps_iface_subscribe_event_list( EventHandleBuf.psIfaceEvHandlePtr,
  	                                  ifaceEvents,
  	                                  ARRAY_SIZE(ifaceEvents));

  if (0 != retVal)
  {
    goto bail;
  }

  EventHandleBuf.psIfaceIPFltrEvHandlePtr = ps_iface_alloc_event_handle
    ( NULL, PSIfaceIPFltrEventCback, NULL);

  if (NULL == EventHandleBuf.psIfaceIPFltrEvHandlePtr)
  {
    goto bail;
  }

  retVal = ps_iface_subscribe_event_list
  	   ( 
             EventHandleBuf.psIfaceIPFltrEvHandlePtr,
  	     ifaceIpFltrEvents,
  	     ARRAY_SIZE(ifaceIpFltrEvents)
           );

  if (0 != retVal)
  {
    goto bail;
  }


 /*--------------------------------------------------------------------------
   Subscribe to list of events on flow
 --------------------------------------------------------------------------*/

  EventHandleBuf.psFlowEvHandlePtr = ps_flow_alloc_event_handle
    ( NULL, PSFlowEventCback, NULL );

  if (NULL == EventHandleBuf.psFlowEvHandlePtr)
  {
    goto bail;
  }

  retVal = ps_flow_subscribe_event_list( EventHandleBuf.psFlowEvHandlePtr,
 	                                 flowEvents,
 	                                 ARRAY_SIZE(flowEvents));

  if (0 != retVal)
  {
    goto bail;
  }


  /*--------------------------------------------------------------------------
	Subscribe to list of events on phys link
   --------------------------------------------------------------------------*/

  EventHandleBuf.physLinkEvHandlePtr = ps_phys_link_alloc_event_handle
    ( NULL, PhysLinkEventCback, NULL );

  if (NULL == EventHandleBuf.physLinkEvHandlePtr )
  {
    goto bail;
  }

  retVal = ps_phys_link_subscribe_event_list
  	   ( 
             EventHandleBuf.physLinkEvHandlePtr,
 	     physLinkEvents,
 	     ARRAY_SIZE(physLinkEvents)
           );

  if (0 != retVal)
  {
    goto bail;
  }

  EventHandleBuf.physLinkDosAckEvHandlePtr = ps_phys_link_alloc_event_handle
    ( NULL, PhysLinkDoSAckEventCback, NULL);

  if (NULL == EventHandleBuf.physLinkDosAckEvHandlePtr)
  {
    goto bail;
  }

  retVal = ps_phys_link_subscribe_event_list
  	   ( 
             EventHandleBuf.physLinkDosAckEvHandlePtr,
  	     physLinkDosAckEvents,
  	     ARRAY_SIZE(physLinkDosAckEvents)
           );
  
  if (0 != retVal)
  {
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Register for ps_mem callbacks
  -------------------------------------------------------------------------*/
  ps_mem_reg_mem_avail_cb( PS_MEM_RT_META_INFO_TYPE, PSMemEventCback);
  ps_mem_reg_mem_avail_cb( PS_MEM_PKT_META_INFO_TYPE, PSMemEventCback);

  return;

bail:
  DS_SOCK_EVENTMGR_ASSERT(0);
  return;

} /* ds::Sock::EventManager::Init() */


void ds::Sock::EventManager::Deinit
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Unsubscribe from a list of events on an interface
    ------------------------------------------------------------------------*/

  ps_iface_free_event_handle (EventHandleBuf.psIfaceEvHandlePtr);
  ps_iface_free_event_handle(EventHandleBuf.psIfaceIPFltrEvHandlePtr);

  /*---------------------------------------------------------------------------
     Unsubscribe from a list of events on a flow
     --------------------------------------------------------------------------*/

  ps_flow_free_event_handle (EventHandleBuf.psFlowEvHandlePtr);

  /*---------------------------------------------------------------------------
     Unsubscribe from a list of events on a phys link
     --------------------------------------------------------------------------*/

  ps_phys_link_free_event_handle (EventHandleBuf.physLinkEvHandlePtr);
  ps_phys_link_free_event_handle ( EventHandleBuf.physLinkDosAckEvHandlePtr); 

} /* ds::Sock::EventManager::Deinit() */
