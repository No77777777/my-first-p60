#ifndef __DSS_DPDPINGSTATHANDLER_H__
#define __DSS_DPDPINGSTATHANDLER_H__

/*====================================================

FILE:  DSS_DpdPingStatHandler.h

SERVICES:
   Handle DPD ping statistic

=====================================================

Copyright (c) 2019 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_DpdPingStatHandler.h#1 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2014-11-24 bh Created module.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"

class DSSDpdPingStatHandler : public DSSEventHandlerNetApp
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSDpdPingStatHandler* CreateInstance();
   DSSDpdPingStatHandler();
};

#endif // __DSS_DPDPINGSTATHANDLER_H__
