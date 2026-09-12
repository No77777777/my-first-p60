#ifndef __DSS_MCASTMBMSCTRLHANDLER_H__
#define __DSS_MCASTMBMSCTRLHANDLER_H__
#ifdef FEATUTE_DATA_PS_MCAST
/*====================================================

FILE:  DSS_MCastMBMSCtrlHandler.h

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

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_MCastMBMSCtrlHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerMCastMBMSCtrl.h"

class DSSMCastMBMSCtrlHandler : public DSSEventHandlerMCastMBMSCtrl
{
protected:

   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:

   DSSMCastMBMSCtrlHandler();
   static DSSMCastMBMSCtrlHandler* CreateInstance();
};
#endif // FEATUTE_DATA_PS_MCAST
#endif // __DSS_MCASTMBMSCTRLHANDLER_H__
