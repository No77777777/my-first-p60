/*===========================================================================
  FILE: ds_Sock_RecvSUBIDInfo.cpp

  OVERVIEW: This file provides implementation of the RecvSUBIDInfo class.

  DEPENDENCIES: None

  Copyright (c) 2015 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_RecvSUBIDInfo.cpp#1 $
  $DateTime: 2020/01/21 03:29:52 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2015-03-02 ad Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "ds_Utils_StdErr.h"

#include "ds_Sock_RecvSUBIDInfo.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_system_heap.h"


using namespace ds::Sock;
using namespace ds::Error;


/*===========================================================================

                     PUBLIC MEMBER FUNCTIONS

===========================================================================*/
void * RecvSUBIDInfo::operator new
(
  unsigned int numBytes
)
throw()
{

  void * buf = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_SYSTEM_HEAP_MEM_ALLOC(buf, numBytes, void *);
  return buf; 
   
} /* RecvSUBIDInfo::operator new() */

void RecvSUBIDInfo::operator delete
(
  void *  bufPtr
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if (0 == bufPtr)
  {
    DS_UTILS_ERROR_FATAL("delete(): NULL ptr");
    return;
  }
  
  PS_SYSTEM_HEAP_MEM_FREE(bufPtr);
  return;
} /* RecvSUBIDInfo::operator delete() */


RecvSUBIDInfo::RecvSUBIDInfo
(
  unsigned int  _recvSubID
) :refCnt(1),recvSubID( _recvSubID)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return;

} /* RecvSUBIDInfo::RecvSUBIDInfo() */


ds::ErrorType CDECL RecvSUBIDInfo::GetAncID
(
  ds::Sock::AncDataIDType *  ancIDPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ancIDPtr)
  {
    return QDS_EFAULT;
  }

  *ancIDPtr = AncData::RECV_SUBID_INFO;
  return AEE_SUCCESS;

} /* RecvSUBIDInfo::GetAncID() */


ds::ErrorType CDECL RecvSUBIDInfo::SetAncID
(
  ds::Sock::AncDataIDType  ancID
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return AEE_SUCCESS;

} /* RecvSUBIDInfo::SetAncID() */


ds::ErrorType CDECL RecvSUBIDInfo::GetRecvSubID
(
  unsigned int *  recvSubIDPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2("RecvSUBIDInfo::GetRecvSubID(): "
                           "Obj 0x%x SubID obj 0x%x ", this, recvSubIDPtr);

  if (0 == recvSubIDPtr)
  {
    return QDS_EFAULT;
  }

  *recvSubIDPtr = recvSubID;

  LOG_MSG_INFO1_1("RecvSUBIDInfo::GetRecvSubID(): SubID %d", recvSubID);

  return AEE_SUCCESS;

} /* RecvSUBIDInfo::GetRecvSubID() */


ds::ErrorType CDECL RecvSUBIDInfo::QueryInterface
(
  AEEIID   iid,
  void **  objPtrPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("RecvSUBIDInfo::QueryInterface(): "
                           "Obj 0x%x iid %d obj 0x%x", this, iid, objPtrPtr);

  if (0 == objPtrPtr)
  {
    return QDS_EFAULT;
  }

  switch (iid)
  {
    case AEEIID_IAncDataPriv:
    {
      *objPtrPtr = static_cast <IAncDataPriv *> ( this);
      (void) AddRef();
      break;
    }

    case AEEIID_IRecvSUBIDInfoPriv:
    {
      *objPtrPtr = this;
      (void) AddRef();
      break;
    }

    case AEEIID_IQI:
    {
      *objPtrPtr = static_cast <IQI *> ( this);
      (void) AddRef();
      break;
    }

    default:
    {
      *objPtrPtr = 0;
      return AEE_ECLASSNOTSUPPORT;
    }
  } /* switch */

  return AEE_SUCCESS;

} /* RecvSUBIDInfo::QueryInterface() */


