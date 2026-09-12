#ifndef DS_SOCK_EVENTMANAGER_H
#define DS_SOCK_EVENTMANAGER_H

/*===========================================================================
  @file EventManager.h

  This file provides the EventManager class.

  TODO: Write detailed explanation.

  Copyright (c) 2008 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/inc/ds_Sock_EventManager.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-02 hm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
namespace ds
{
  namespace Sock
  {
    namespace EventManager
    {
      void Init
      (
        void
      );

      void Deinit
      (
        void
      );

    } /* namespace EventManager */
  } /* namespace Sock */
}/* namespace ds */

#endif /* DS_SOCK_EVENTMANAGER_H */
