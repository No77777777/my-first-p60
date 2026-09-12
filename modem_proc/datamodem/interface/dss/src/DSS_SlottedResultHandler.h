#ifndef __DSS_SLOTTEDRESULTHANDLER_H__
#define __DSS_SLOTTEDRESULTHANDLER_H__

/*====================================================

FILE:  DSS_SlottedResultHandler.h

SERVICES:
   Handle Slotted Result events.

=====================================================

Copyright (c) 2008 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_SlottedResultHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"

class DSSSlottedResultHandler : public DSSEventHandlerNetApp
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSSlottedResultHandler* CreateInstance();
   DSSSlottedResultHandler();
};

#endif // __DSS_SLOTTEDRESULTHANDLER_H__
