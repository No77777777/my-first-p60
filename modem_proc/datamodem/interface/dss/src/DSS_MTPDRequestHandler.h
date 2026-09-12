#ifndef __DSS_MTPDREQUESTHANDLER_H__
#define __DSS_MTPDREQUESTHANDLER_H__

/*====================================================

FILE:  DSS_MTPDRequestHandler.h

SERVICES:
   Handle MTPD events.

=====================================================

Copyright (c) 2008-2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_MTPDRequestHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"
#include "ds_Net_ITechUMTS.h"

class DSSMTPDRequestHandler : public DSSEventHandlerNetApp
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();
   virtual AEEResult SetEventData(dss_iface_ioctl_event_enum_type event,
                                  bool bReg,
                                  dss_iface_ioctl_event_cb userCB,
                                  void* userData,
                                  sint15 appId);

private:
   struct DSSMTPDRequestData : public HandlerData{
      IQI* pMTPDReg;
   };

public:
   static DSSMTPDRequestHandler* CreateInstance();
   DSSMTPDRequestHandler();
   virtual void Destructor() throw();

};

#endif // __DSS_MTPDREQUESTHANDLER_H__
