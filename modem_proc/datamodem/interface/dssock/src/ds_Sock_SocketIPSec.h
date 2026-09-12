#ifndef DS_SOCK_SOCKETIPSEC_H
#define DS_SOCK_SOCKETIPSEC_H
/*===========================================================================
  @file ds_Sock_SocketIPSec.h

  This file defines a set of helper routines for IPSec related functionality.

  Copyright (c) 2009 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_SocketIPSec.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "ds_Sock_Socket.h"
#include "ps_rt_meta_info.h"


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
namespace ds
{
  namespace Sock
  {
    class SocketIPSec
    {                                                         /*lint !e578 */
      public:
        static bool IsHandleInIfaceList
        (
          int32                   ifaceHandle,
          ps_rt_meta_info_type *  rtMetaInfoPtr
        );

        static bool IsIfaceListInReqState
        (
          ps_rt_meta_info_type *  rtMetaInfoPtr,
          uint32                  reqIfaceState
        );

        static void ClearIfaceList
        (
          ps_rt_meta_info_type *  rtMetaInfoPtr
        );

        static void TearDownIfaceList
        (
          ps_rt_meta_info_type *  rtMetaInfoPtr
        );

        static bool IsIfaceListFlowEnabled
        (
          ps_rt_meta_info_type *  rtMetaInfoPtr
        );

        static void FltrIPSecClient
        (
          ds::Sock::Socket *      sockPtr,
          ps_rt_meta_info_type *  newRtMetaInfoPtr,
          ps_rt_meta_info_type *  oldRtMetaInfoPtr
        );
    }; /* class SocketIPSec */

  } /* namespace Sock */
} /* namespace ds */

#endif /* DS_SOCK_SOCKETIPSEC_H */
