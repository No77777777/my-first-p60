/*===========================================================================
  FILE: ds_Sock_ClassIDInstantiator.cpp

  OVERVIEW: This file provides implementation for the methods defined in
            ds_Sock_ClassIDInstantiator.h

  DEPENDENCIES: None

            Copyright (c) 2008-2014 Qualcomm Technologies Incorporated.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_ClassIDInstantiator.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-14 msr Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "ds_Utils_StdErr.h"
#include "ds_Utils_CSSupport.h"
#include "ds_Sock_ClassIDInstantiator.h"
#include "ds_Sock_SocketFactoryClient.h"

using namespace ds::Sock;
using namespace ds::Error;


/*===========================================================================

                        PUBLIC MEMBER FUNCTIONS

===========================================================================*/
extern "C"
int DSSockSocketFactoryCreateInstance
(
  void *    envPtr,
  AEECLSID  clsID,
  void **   newObjPtrPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == newObjPtrPtr)
  {
    return QDS_EFAULT;
  }

  *newObjPtrPtr = SocketFactoryClient::CreateInstance(clsID);
  if (0 == *newObjPtrPtr)
  {
    return AEE_ENOMEMORY;
  }

  return AEE_SUCCESS;

} /* DSSockSocketFactoryCreateInstance() */

