/*===========================================================================
  FILE: ds_Sock_SocketFactoryClient.cpp

  OVERVIEW: This file provides implementation of the SocketFactoryClient class.

  DEPENDENCIES: None

                    Copyright (c) 2010 - 2014 Qualcomm Technologies Incorporated.
                    All Rights Reserved.
                    Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_SocketFactoryClient.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-02-14 ts Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "ds_Sock_SocketFactoryClient.h"
#include "ds_Sock_CSocketFactory.h"
#include "ds_Sock_CSocketFactoryService.h"
#include "ds_Sock_SocketFactory.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ps_mem.h"

using namespace ds::Sock;
using namespace ds::Error;
using namespace ds::Utils;
using namespace ds::Net;


#define DS_SOCK_FACTORYCLIENT_ASSERT(expression) \
         ds_sock_factoryclient_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION ds_sock_eventmgr_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_SOCK_EVENTMGR_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_sock_factoryclient_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DSNET_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_sock_eventmgr_assert_wrapper */



/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                         PUBLIC MEMBER FUNCTIONS

===========================================================================*/
ds::ErrorType CDECL SocketFactoryClient::CreateSocket
(
  ds::AddrFamilyType  family,
  SocketType          socketType,
  ProtocolType        protocol,
  IPolicy *           netPolicyPtr,
  ISocket **          newSockPtrPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds::ErrorType retval = AEE_SUCCESS;

  if ( 0 == socketFactoryPtr)
  {
    LOG_MSG_ERROR_0("SocketFactoryClient::CreateSocket(): "
                    "NULL Socket Factory");
    return AEE_EFAILED;
  }

  retval = socketFactoryPtr->CreateSocketByPolicy(family,
                                                  socketType,
                                                  protocol,
                                                  netPolicyPtr,
                                                  newSockPtrPtr);

  LOG_MSG_FUNCTION_EXIT_2("SocketFactoryClient::CreateSocket(): "
                          "SockFactory 0x%x, retval = 0x%x", this, retval);
  return retval;

} /* SocketFactoryClient::CreateSocket */


ds::ErrorType CDECL SocketFactoryClient::CreateSocketByNetwork
(
  ds::AddrFamilyType  family,
  SocketType          socketType,
  ProtocolType        protocol,
  INetwork *          dsNetObjPtr,
  ISocket **          newSockPtrPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds::ErrorType retval = AEE_SUCCESS;

  if ( 0 == socketFactoryPtr)
  {
    LOG_MSG_ERROR_0("SocketFactoryClient::CreateSocketByNetwork(): "
                    "NULL Socket Factory");
    return AEE_EFAILED;
  }

  retval = socketFactoryPtr->CreateSocketByNetwork(family,
                                                   socketType,
                                                   protocol,
                                                   dsNetObjPtr,
                                                   newSockPtrPtr);

  LOG_MSG_FUNCTION_EXIT_2("SocketFactoryClient::CreateSocketByNetwork(): "
                          "SockFactory 0x%x, retval 0x%x", this, retval);
  return retval;

} /* SocketFactoryClient::CreateSocketByNetwork() */


void * SocketFactoryClient::CreateInstance
(
  AEECLSID    clsID
)
{
  SocketFactory       * SocketFactoryPtr = 0;
  SocketFactoryClient * inst = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  switch (clsID)
  {
   // Note: this code can be called both from DS (using AEECLSID_CSocketFactory
   // or AEECLSID_CSocketPrivFactory),
   // or from CS (using AEECLSID_CSocketFactoryService or AEECLSID_CSocketPrivFactoryService).
    case ds::Sock::AEECLSID_CSocketFactory:
    case ds::Sock::AEECLSID_CSocketFactoryService:
      SocketFactoryPtr = SocketFactory::CreateInstance();
      if (0 != SocketFactoryPtr)
      {
        inst = new SocketFactoryClient(SocketFactoryPtr);
        DS_UTILS_RELEASEIF(SocketFactoryPtr);
      }
      break;

    default:
      goto bail;
  }

  if (0 == inst)
  {
    goto bail;
  }

  (void)inst->AddRef();

bail:
  LOG_MSG_FUNCTION_EXIT_2("SocketFactoryClient::CreateInstance(): "
                          "Class ID %0x inst 0x%x", clsID, inst);
  return inst;
}

SocketFactoryClient::SocketFactoryClient
(
  SocketFactory     * _socketFactory
)throw():refCnt(0)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == _socketFactory)
  {
    DS_SOCK_FACTORYCLIENT_ASSERT(0);
  }

  socketFactoryPtr = _socketFactory;
  (void)socketFactoryPtr->AddRef();

  LOG_MSG_FUNCTION_EXIT_2("SocketFactoryClient::CreateInstance(): "
                          "Created factory client 0x%p with factory 0x%p",
                          this, socketFactoryPtr);

} /* SocketFactoryClient::SocketFactoryClient() */


SocketFactoryClient::~SocketFactoryClient
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 != socketFactoryPtr)
  {
    (void)socketFactoryPtr->Release();
  }

  LOG_MSG_FUNCTION_EXIT_1("SocketFactoryClient::CreateInstance(): "
                          "Deleted factory client 0x%p", this);
} /* SocketFactoryClient::~SocketFactoryClient() */


void * SocketFactoryClient::operator new
(
  unsigned int numBytes
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ps_mem_get_buf( PS_MEM_SOCKET_FACTORY_CLIENT_TYPE);

} /* SocketFactoryClient::operator new() */


void SocketFactoryClient::operator delete
(
  void *  bufPtr
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == bufPtr)
  {
    DS_SOCK_FACTORYCLIENT_ASSERT(0);
    return;
  }

  PS_MEM_FREE(bufPtr);
  return;

} /* SocketFactoryClient::operator delete() */

/*===========================================================================

                          PROTECTED MEMBER FUNCTIONS

===========================================================================*/



/*===========================================================================

                         PRIVATE MEMBER FUNCTIONS

===========================================================================*/



