#ifndef __DSS_QOSINFOCODEUPDATEDHANDLER_H__
#define __DSS_QOSINFOCODEUPDATEDHANDLER_H__

/*====================================================

FILE:  DSS_QoSInfoCodeUpdatedHandler.h

SERVICES:
   Handle network outage events.

=====================================================

Copyright (c) 2008 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_QoSInfoCodeUpdatedHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerQoS.h"

class DSSQoSInfoCodeUpdatedHandler : public DSSEventHandlerQoS
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSQoSInfoCodeUpdatedHandler* CreateInstance();
   DSSQoSInfoCodeUpdatedHandler();
};

#endif // __DSS_QOSINFOCODEUPDATEDHANDLER_H__
