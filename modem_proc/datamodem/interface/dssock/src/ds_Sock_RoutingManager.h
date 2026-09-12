#ifndef DS_SOCK_ROUTINGMANAGER_H
#define DS_SOCK_ROUTINGMANAGER_H
/*===========================================================================
  @file ds_Sock_RoutingManager.h

  This file declares a class which provides a set of helper methods useful
  in routing and filtering.

  Copyright (c) 2008 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_RoutingManager.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "ds_Utils_CSSupport.h"
#include "ds_Net_IPolicy.h"
#include "ds_Sock_Socket.h"
#include "ps_rt_meta_info.h"
#include "ps_iface_defs.h"


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
namespace ds
{
  namespace Sock
  {
    class RoutingManager
    {                                                         /*lint !e578 */
      public:
        static ds::ErrorType RoutePacket
        (
          ds::Sock::Socket *      sockPtr,
          bool                    isSystemSocket,
          ds::Net::IPolicy *      policyPtr,
          ps_rt_meta_info_type *  newRtMetaInfoPtr
        );

        static void FltrClient
        (
          ds::Sock::Socket *                   sockPtr,
          ps_iface_ipfltr_client_id_enum_type  fltrClient,
          ps_rt_meta_info_type *               newRtMetaInfoPtr
        );
    };

  } /* namespace Sock */
} /* namespace ds */

#endif /* DS_SOCK_ROUTINGMANAGER_H */
