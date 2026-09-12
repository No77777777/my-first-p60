#ifndef __DSS_EVENTHANDLERRANASSTMEDIAADAPT_H__
#define __DSS_EVENTHANDLERRANASSTMEDIAADAPT_H__

/*====================================================

FILE:  DSS_EventHandlerRanAsstMediaAdaptReq.h

SERVICES:
   Provide a common base class for event handling.

=====================================================

Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_EventHandlerRanAsstMediaAdaptReq.h#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-07-05 sz  Created.

===========================================================================*/

#include "DSS_EventHandler.h"

class DSSRanAsstMediaAdaptReq;

class DSSEventHandlerRanAsstMediaAdaptReq : public DSSEventHandler
{
public:
   // This class uses two-phase construction: the constructor is empty and the
   // actual initialization is done through the Init() function, so it can return
   // an error value.
   AEEResult Init(DSSRanAsstMediaAdaptReq* parentNetAppParam);
   virtual void Destructor() throw();

protected:
   void DispatchCB(dss_iface_ioctl_event_enum_type event, 
                   EventData* ped,
                   dss_iface_ioctl_event_info_union_type* eventInfo);

   DSSRanAsstMediaAdaptReq* mpParent;
};

#endif // __DSS_EVENTHANDLERRANASSTMEDIAADAPT_H__
