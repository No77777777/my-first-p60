#ifndef DS_SOCK_IRECVTTLINFOPRIV_H
#define DS_SOCK_IRECVTTLINFOPRIV_H

/*============================================================================
  Copyright (c) 2015 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
  ============================================================================*/

#include "ds_Utils_Interface.h"
#include "ds_Utils_StdDef.h"
#include "ds_Utils_IQI.h"
#include "ds_Sock_IAncDataPriv.h"

namespace ds
{
   namespace Sock
   {
      const ::AEEIID AEEIID_IRecvTTLInfoPriv = 0x106e70f;
      
      /** @interface IRecvTTLInfoPriv
        * 
        * ds Socket Recv TTL Info interface.
        * This interface is used to provide application with the network interface handle
        * on which a datagram was received via a call to IDSSock RecvMsg API.
        * This information is provided if the IP_RECVTTL socket option is enabled.
        */
      struct IRecvTTLInfoPriv : public ::ds::Sock::IAncDataPriv
      {
         virtual ::AEEResult AEEINTERFACE_CDECL GetRecvTTLHandle(unsigned char* value) = 0;
      };
   };
};
#endif //DS_SOCK_IRECVTTLINFOPRIV_H
