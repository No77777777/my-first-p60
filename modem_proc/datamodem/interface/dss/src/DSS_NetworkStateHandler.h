#ifndef __DSS_NETWORKSTATEHANDLER_H__
#define __DSS_NETWORKSTATEHANDLER_H__

/*====================================================

FILE:  DSS_NetworkStateHandler.h

SERVICES:
   Handle network down event.

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_NetworkStateHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-13 en  History added.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"

class DSSNetworkStateHandler : public DSSEventHandlerNetApp
{
protected:
  virtual void EventOccurred();
  virtual AEEResult RegisterIDL();

public:
  static DSSNetworkStateHandler* CreateInstance();
  DSSNetworkStateHandler();
};

#endif // __DSS_NETWORKSTATEHANDLER_H__
