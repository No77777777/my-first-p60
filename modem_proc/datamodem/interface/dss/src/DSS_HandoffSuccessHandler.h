#ifndef __DSS_HANDOFFSUCCESSHANDLER_H__
#define __DSS_HANDOFFSUCCESSHANDLER_H__

/*====================================================

FILE:  DSS_HandoffSuccessHandler.h

SERVICES:
   Handle handoff events.

=====================================================

Copyright (c) 2013 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_HandoffSuccessHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2013-02-13 da  Created module.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"

class DSSHandoffSuccessHandler : public DSSEventHandlerNetApp
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSHandoffSuccessHandler* CreateInstance();
   DSSHandoffSuccessHandler();
};

#endif // __DSS_HANDOFFSUCCESSHANDLER_H__
