#ifndef __DSS_PRIMARYQOSMODIFYSTATUSHANDLER_H__
#define __DSS_PRIMARYQOSMODIFYSTATUSHANDLER_H__

/*====================================================

FILE:  DSS_PrimaryQoSModifyStatusHandler.h

SERVICES:
   Handle primary QoS status events

=====================================================

Copyright (c) 2011 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PrimaryQoSModifyStatusHandler.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerNetActive.h"

class DSSPrimaryQoSModifyStatusHandler : public DSSEventHandlerNetActive
{
protected:
  virtual void EventOccurred();
  virtual AEEResult RegisterIDL();

public:
  static DSSPrimaryQoSModifyStatusHandler* CreateInstance();
  DSSPrimaryQoSModifyStatusHandler();
};

#endif // __DSS_PRIMARYQOSMODIFYSTATUSHANDLER_H__

