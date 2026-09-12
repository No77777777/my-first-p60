#ifndef DS_SOCK_RECVSUBIDINFO_H
#define DS_SOCK_RECVSUBIDINFO_H
/*===========================================================================
  @file ds_Sock_RecvSUBIDInfo.h

  This file defines the interface for receiving subcscription ID info from
  socket.

  Confidential and Proprietary - Qualcomm Technologies, Inc.

  Copyright (c) 2019 Qualcomm Technologies, Inc. All rights reserved.

  This software may be subject to U.S. and international export, re-export, or transfer
  ("export") laws. Diversion contrary to U.S. and international law is strictly prohibited.

  Qualcomm Snapdragon is a product of Qualcomm Technologies, Inc. 

  Qualcomm and Snapdragon are trademarks of Qualcomm Technologies Incorporated, registered in the United States and other countries.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/inc/ds_Sock_RecvSUBIDInfo.h#1 $
  $DateTime: 2020/01/21 03:29:52 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ds_Sock_IRecvSUBIDInfoPriv.h"
#include "ds_Utils_CSSupport.h"
#include "ds_Utils_StdErr.h"
#include "ds_Errors_Def.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
namespace ds
{
  namespace Sock
  {
    class RecvSUBIDInfo : public IRecvSUBIDInfoPriv
    {
      public:
        static void * operator new
        (
          unsigned int numBytes
        ) throw();

        static void operator delete
        (
          void *  bufPtr
        ) throw();

        RecvSUBIDInfo
        (
          unsigned int  recvSubID
        );

        virtual ds::ErrorType CDECL GetAncID
        (
          ds::Sock::AncDataIDType *  ancIDPtr
        );

        virtual ds::ErrorType CDECL SetAncID
        (
          ds::Sock::AncDataIDType  ancID
        );

        virtual ds::ErrorType CDECL GetRecvSubID
        (
          unsigned int *  recvSubIDPtr
        );

        virtual ~RecvSUBIDInfo() {}

        /*-------------------------------------------------------------------
          IQI interface Methods
        -------------------------------------------------------------------*/
        DSIQI_DECL_LOCALS()
        DSIQI_ADDREF()
        DSIQI_RELEASE()
        virtual ds::ErrorType CDECL QueryInterface
        (
          AEEIID   iid,
          void **  objPtrPtr
        );

      private:
        unsigned int  recvSubID;
    };
  } /* namespace Sock */
} /* namespace ds */

#endif /* DS_SOCK_RECVSUBIDINFO_H */


