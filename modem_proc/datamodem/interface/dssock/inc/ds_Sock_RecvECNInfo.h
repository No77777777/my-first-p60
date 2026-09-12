#ifndef DS_SOCK_RECVECNINFO_H
#define DS_SOCK_RECVECNINFO_H
/*===========================================================================
  @file IPlatformSocketFactory.h

  This file defines the interface for Sockets Platform. This interface
  abstracts various platforms such as WinMobile, Linux, and BMP etc. from
  the sockets library and provides methods to create a PlatformSocket.

  Copyright (c) 2015 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/inc/ds_Sock_RecvECNInfo.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ds_Sock_IRecvECNInfoPriv.h"
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
    class RecvECNInfo : public IRecvECNInfoPriv
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

        RecvECNInfo
        (
          unsigned char  recvIFHandle
        );

        virtual ds::ErrorType CDECL GetAncID
        (
          ds::Sock::AncDataIDType *  ancIDPtr
        );

        virtual ds::ErrorType CDECL SetAncID
        (
          ds::Sock::AncDataIDType  ancID
        );

        virtual ds::ErrorType CDECL GetRecvECNHandle
        (
          unsigned char *  recvECNHandlePtr
        );

      virtual ~RecvECNInfo() {}

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
        unsigned char  recvECNHandle;
    };
  } /* namespace Sock */
} /* namespace ds */

#endif /* DS_SOCK_RECVECNINFO_H */

