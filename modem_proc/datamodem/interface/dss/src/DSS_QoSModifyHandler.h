#ifndef __DSS_QOSMODIFYHANDLER_H__
#define __DSS_QOSMODIFYHANDLER_H__

/*====================================================

FILE:  DSS_QoSModifyHandler.h

SERVICES:
   Handle network extended IP config events.

=====================================================

Copyright (c) 2008 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_QoSModifyHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerQoS.h"

class DSSQoSModifyHandler : public DSSEventHandlerQoS
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSQoSModifyHandler* CreateInstance();
   DSSQoSModifyHandler();
};

#endif // __DSS_QOSMODIFYHANDLER_H__
