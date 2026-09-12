#ifndef DS_SOCK_IRECVSUBIDINFOPRIV_H
#define DS_SOCK_IRECVSUBIDINFOPRIV_H

/*============================================================================
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  Copyright (c) 2019 Qualcomm Technologies, Inc. All rights reserved.

  This software may be subject to U.S. and international export, re-export, or transfer
  ("export") laws. Diversion contrary to U.S. and international law is strictly prohibited.

  Qualcomm Snapdragon is a product of Qualcomm Technologies, Inc. 

  Qualcomm and Snapdragon are trademarks of Qualcomm Technologies Incorporated, registered in the United States and other countries.
============================================================================*/

#include "ds_Utils_Interface.h"
#include "ds_Utils_StdDef.h"
#include "ds_Utils_IQI.h"
#include "ds_Sock_IAncDataPriv.h"

namespace ds
{
   namespace Sock
   {
      const ::AEEIID AEEIID_IRecvSUBIDInfoPriv = 0x106e711;
      
      /** @interface IRecvSUBIDInfoPriv
        * 
        * ds Socket Recv subsscription ID Info interface.
        * This interface is used to provide application with the network interface handle
        * on which a datagram was received via a call to IDSSock RecvMsg API.
        * This information is provided if the IP_RECVSUBID socket option is enabled.
        */
      struct IRecvSUBIDInfoPriv : public ::ds::Sock::IAncDataPriv
      {
         virtual ::AEEResult AEEINTERFACE_CDECL GetRecvSubID(unsigned int* value) = 0;
      };
   };
};
#endif //DS_SOCK_IRECVSUBIDINFOPRIV_H

