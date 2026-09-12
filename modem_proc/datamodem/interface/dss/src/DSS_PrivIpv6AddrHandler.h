#ifndef __DSS_PRIVIPV6ADDRHANDLER_H__
#define __DSS_PRIVIPV6ADDRHANDLER_H__

/*====================================================

FILE:  DSS_PrivIpv6AddrHandler.h

SERVICES:
   Handle private Ipv6 Adress events

=====================================================

Copyright (c) 2008 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PrivIpv6AddrHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerPrivIPV6Addr.h"

class DSSPrivIpv6AddrHandler : public DSSEventHandlerPrivIpv6Addr
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSPrivIpv6AddrHandler* CreateInstance();
   DSSPrivIpv6AddrHandler();
};

#endif // __DSS_PRIVIPV6ADDRHANDLER_H__
