#ifndef __DSS_EXTENDEDIPCONFIGEXHANDLER_H__
#define __DSS_EXTENDEDIPCONFIGEXHANDLER_H__

/*====================================================

FILE:  DSS_ExtendedIPConfigExHandler.h

SERVICES:
   Handle mtu propagated event.

=====================================================

Copyright (c) 2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_ExtendedIPConfigExHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2014-07-20 sb  Created module.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"

class DSSExtendedIPConfigExHandler : public DSSEventHandlerNetApp
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSExtendedIPConfigExHandler* CreateInstance();
   DSSExtendedIPConfigExHandler();
};

#endif // __DSS_EXTENDEDIPCONFIGEXHANDLER_H__
