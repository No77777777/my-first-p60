#ifndef __DSS_FASTDORMSTATUSHANDLER_H__
#define __DSS_FASTDORMSTATUSHANDLER_H__

/*====================================================

FILE:  DSS_FastDormStatusHandler.h

SERVICES:
   Handle fast dormancy status updates.
=====================================================

Copyright (c) 2008 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_FastDormStatusHandler.h#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

when       who what, where, why
---------- --- ------------------------------------------------------------
2011-05-20 mct History added.

===========================================================================*/

#include "DSS_EventHandlerNetApp.h"

class DSSFastDormStatusHandler : public DSSEventHandlerNetApp
{
protected:
   virtual void EventOccurred();
   virtual AEEResult RegisterIDL();

public:
   static DSSFastDormStatusHandler* CreateInstance();
   DSSFastDormStatusHandler();

};

#endif // __DSS_FASTDORMSTATUSHANDLER_H__
