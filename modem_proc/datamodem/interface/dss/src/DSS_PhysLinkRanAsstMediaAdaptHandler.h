#ifndef DSS_PHYSLINKRANASSTHANDLER_H
#define DSS_PHYSLINKRANASSTHANDLER_H

/*====================================================

FILE:  DSS_PhysLinkRanAsstMediaAdaptHandler.h

SERVICES:
   Handle physical link RAN assisted media adaptation event.

=====================================================

Copyright (c) 2008 - 2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PhysLinkRanAsstMediaAdaptHandler.h#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-13 en  History added.

===========================================================================*/


#include "DSS_EventHandlerRanAsstMediaAdaptReq.h"

class DSSPhysLinkRanAsstMediaAdaptHandler : public DSSEventHandlerRanAsstMediaAdaptReq
{
protected:
  virtual void EventOccurred();
  virtual AEEResult RegisterIDL();

public:
  static DSSPhysLinkRanAsstMediaAdaptHandler* CreateInstance();
  DSSPhysLinkRanAsstMediaAdaptHandler();
};

#endif // DSS_PHYSLINKRANASSTHANDLER_H