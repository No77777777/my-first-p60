/*===========================================================================
  FILE: PS_Sock_IO_SocketIOFactory.cpp

  OVERVIEW: This class acts as a factory for IReader and IWriter interfaces.

  DEPENDENCIES: None

  Copyright (c) 2008 - 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_IO_SocketIOFactory.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-14 msr Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "ds_Utils_StdErr.h"

#include "PS_Sock_IO_SocketIOFactory.h"
#include "PS_Sock_IO_DgramQueue.h"
#include "PS_Sock_IO_ByteStream.h"


using namespace PS::Sock::IO;
using namespace ds::Error;
using namespace ds::Sock;


/*===========================================================================

                     PUBLIC MEMBER FUNCTIONS

===========================================================================*/
ds::ErrorType SocketIOFactory::GetReaderAndWriter
(
  ProtocolType  protocol,
  IReader **    readerPtrPtr,
  IWriter **    writerPtrPtr,
  ICritSect *   critSectPtr
)
{
  ByteStream *  byteStreamPtr = NULL;
  DgramQueue *  dgramQueuePtr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == readerPtrPtr || 0 == writerPtrPtr || 0 == critSectPtr)
  {
    LOG_MSG_INVALID_INPUT_3("SocketIOFactory::GetReaderAndWriter(): "
                            "NULL args 0x%x 0x%x 0x%x",
                            readerPtrPtr, writerPtrPtr, critSectPtr);
    return QDS_EFAULT;
  }

  /*-------------------------------------------------------------------------
    Allocate byte stream or dgram queu and update reader/writer
  -------------------------------------------------------------------------*/
  if (Protocol::QDS_TCP == protocol)
  {
    byteStreamPtr = new ByteStream( critSectPtr);
    if (NULL == byteStreamPtr)
    {
      goto bail;
    }

    *readerPtrPtr = static_cast <IReader *> ( byteStreamPtr);
    *writerPtrPtr = static_cast <IWriter *> ( byteStreamPtr);
  }
  else
  {
    dgramQueuePtr = new DgramQueue( critSectPtr);
    if (NULL == dgramQueuePtr)
    {
      goto bail;
    }

    *readerPtrPtr = static_cast <IReader *> ( dgramQueuePtr);
    *writerPtrPtr = static_cast <IWriter *> ( dgramQueuePtr);
  }

  LOG_MSG_FUNCTION_EXIT_3("SocketIOFactory::GetReaderAndWriter(): "
                          " reader 0x%x writer 0x%x Proto %d",
                          *readerPtrPtr, *writerPtrPtr, protocol);
  return AEE_SUCCESS;

bail:
  if (NULL != byteStreamPtr)
  {
    delete byteStreamPtr;
  }

  if (NULL != dgramQueuePtr)
  {
    delete dgramQueuePtr;
  }

  return QDS_EMFILE;

} /* SocketIOFactory::GetReaderAndWriter() */

