#ifndef __DSS_QOSNETINITIATEDREQHANDLER_H__
#define __DSS_QOSNETINITIATEDREQHANDLER_H__

/*====================================================

FILE:  DSS_QoSNetInitiatedReqHandler.h

SERVICES:
   Handle QoS Net Initiated Request events.

=====================================================

Copyright (c) 2011 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_QoSNetInitiatedReqHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-07-08 sz  Created.

===========================================================================*/

#include "DSS_EventHandlerQoSNetInitiatedReq.h"


class DSSQoSNetInitiatedReqHandler : public DSSEventHandlerQoSNetInitiatedReq
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSQoSNetInitiatedReqHandler* CreateInstance();
   DSSQoSNetInitiatedReqHandler();
};

#endif // __DSS_QOSNETINITIATEDREQHANDLER_H__
