#ifndef __DSS_PHYSLINKSTATEHANDLER_H__
#define __DSS_PHYSLINKSTATEHANDLER_H__

/*====================================================

FILE:  DSS_PhysLinkStateHandler.h

SERVICES:
   Handle physical link state down event.

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PhysLinkStateHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-13 en  History added.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"

class DSSPhysLinkStateHandler : public DSSEventHandlerNetApp
{
protected:
  virtual void EventOccurred();
  virtual AEEResult RegisterIDL();

public:
  static DSSPhysLinkStateHandler* CreateInstance();
  DSSPhysLinkStateHandler();
};

#endif // __DSS_PHYSLINKSTATEHANDLER_H__
