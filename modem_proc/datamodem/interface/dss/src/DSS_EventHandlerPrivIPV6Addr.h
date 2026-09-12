#ifndef __DSS_EVENTHANDLERPRIVIPV6ADDR_H__
#define __DSS_EVENTHANDLERPRIVIPV6ADDR_H__

/*====================================================

FILE:  DSS_EventHandlerPrivIpv6Addr.h

SERVICES:
   Provide a common base class for event handling.

=====================================================

Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_EventHandlerPrivIPV6Addr.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_PrivIpv6Addr.h"
#include "DSS_EventHandler.h"
#include "DSS_WeakRefScope.h"

class DSSEventHandlerPrivIpv6Addr : public DSSEventHandler
{
public:
   // This class uses two-phase construction: the constructor is empty and the
   // actual initialization is done through the Init() function, so it can return
   // an error value.
  AEEResult Init(DSSPrivIpv6Addr* parentNetAppParam);
  virtual void Destructor() throw();

protected:
   void DispatchCB(dss_iface_ioctl_event_enum_type event,
                   EventData* ped,
                   dss_iface_ioctl_event_info_union_type* eventInfo);

   DSSPrivIpv6Addr* mpParent;

};
  
#endif // __DSS_EVENTHANDLERPRIVIPV6ADDR_H__
