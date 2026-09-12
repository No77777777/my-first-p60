#ifndef DS_UTILS_CRITSCOPE_H
#define DS_UTILS_CRITSCOPE_H

/*===========================================================================
  @file ds_Utils_CritScope.h

  A utility class to automatically lock and unlock an entire scope.

  Copyright (c) 2008,2010 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/inc/ds_Utils_CritScope.h#2 $
  $DateTime: 2019/10/22 02:53:17 $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-13 mt  Copied from DSS_CritScope.

===========================================================================*/

#include "ds_Utils_ICritSect.h"

namespace ds
{
  namespace Utils
  {

    /*! @brief Lock/unlock a scope.

      This class provides a useful utility for critical section usage.
      Defining an instance of this class in the top of the scope will enter
      the critical section. The critical section is exited automatically when
      the scope ends, due to the stack semantics that call the object's
      destructor.
    */
    class CritScope {
      public:
        CritScope(ICritSect *cs);
        ~CritScope();

      private:
        ICritSect* critSectPtr;
    };

    inline CritScope::CritScope(ICritSect *cs)
    {
      critSectPtr = cs;
      critSectPtr->Enter();
    }

    /*lint -e{1551} */
    inline CritScope::~CritScope()
    {
      critSectPtr->Leave();
    }
    /*lint –restore */

  } /* namespace Utils */
}/* namespace ds */

#endif /* DS_UTILS_CRITSCOPE_H */

