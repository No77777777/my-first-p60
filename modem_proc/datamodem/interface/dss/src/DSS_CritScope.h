#ifndef __DSS_CRITSCOPE_H__
#define __DSS_CRITSCOPE_H__

/*===================================================

FILE:  DSS_CritScope.h

SERVICES:
   A utility class to automatically lock and unlock
   an entire scope.

=====================================================

Copyright (c) 2008 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_CritScope.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-13 en  History added.

===========================================================================*/

#include "ds_Utils_ICritSect.h"

// This class provides an abstraction over DSS_CritSect.
// Defining an instance of this class in the top of the scope will enter the
// critical section. The critical section is exited automatically when the scope
// ends, due to the stack semantics that call the object's destructor.
class DSSCritScope {
public:
   DSSCritScope(ICritSect& cs);
   ~DSSCritScope();

private:
   ICritSect* pCritSect;
};

inline DSSCritScope::DSSCritScope(ICritSect& cs)
{
   pCritSect = &cs;
   pCritSect->Enter();
}

/*lint -e{1551} */
inline DSSCritScope::~DSSCritScope()
{
   pCritSect->Leave();
}
/*lint –restore */

#endif // __DSS_CRITSCOPE_H__
