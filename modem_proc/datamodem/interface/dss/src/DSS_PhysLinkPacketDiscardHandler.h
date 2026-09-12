#ifndef DSS_PHYSLINKPACKETDISCARDHANDLER_H
#define DSS_PHYSLINKPACKETDISCARDHANDLER_H

/*====================================================

FILE:  DSS_PhysLinkPacketDiscardHandler.h

SERVICES:
   Handle physical link Packet discard event.

=====================================================

Copyright (c) 2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PhysLinkPacketDiscardHandler.h#1 $
  $DateTime: 2021/05/25 01:37:25 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2020-12-11 en  History added.

===========================================================================*/


#include "DSS_EventHandlerNetApp.h"

class DSSPhysLinkPacketDiscardHandler : public DSSEventHandlerNetApp
{
protected:
  virtual void EventOccurred();
  virtual AEEResult RegisterIDL();

public:
  static DSSPhysLinkPacketDiscardHandler* CreateInstance();
  DSSPhysLinkPacketDiscardHandler();
};

#endif // DSS_PHYSLINKPACKETDISCARDHANDLER_H
