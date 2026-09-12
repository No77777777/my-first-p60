/*=========================================================================*/
/*!
  @file
  ds_Sock_CreateInstance.cpp

  @brief
  This file provides implementation of DSSockCreateInstance().

            Copyright (c) 2008-2014 Qualcomm Technologies Incorporated.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_CreateInstance.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-07-14 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Errors_Def.h"
#include "ds_Utils_DebugMsg.h"

#include "ds_Sock_ClassIDInstantiator.h"

#include "ds_Sock_CreateInstance.h"
#include "ds_Sock_SocketFactoryClient.h"
#include "ds_Sock_CSocketFactory.h"
#include "ds_Sock_CSocketFactoryService.h"

/*===========================================================================

                        PUBLIC FUNCTION DEFINITIONS

===========================================================================*/

int DSSockCreateInstance
(
   IEnv* env,
   AEECLSID clsid,
   void** newObj
 )
{
  int ret = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  // Note: in the cases below, the same method is being called
  // both for AEECLSID_CX and for AEECLSID_CXService.
  // AEECLSID_CX is used when the DS requests to create the object.
  // AEECLSID_CXService is used when the CS requests to create the object.
  switch (clsid)
  {
    case ds::Sock::AEECLSID_CSocketFactory:
    case ds::Sock::AEECLSID_CSocketFactoryService:
      ret = DSSockSocketFactoryCreateInstance (env, clsid, newObj);
      break;

    default:
      ret = AEE_ECLASSNOTSUPPORT;
      break;

  } /* switch (clsid) */

  return ret;

} /* DSSockCreateInstance() */


