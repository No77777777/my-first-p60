/*===========================================================================
  FILE: ds_Net_ClassIDInstantiator.cpp

  OVERVIEW: This file provides implementation for the methods defined in
            ds_Net_ClassIDInstantiator.h

  DEPENDENCIES: None

            Copyright (c) 2010 - 2014 Qualcomm Technologies Incorporated.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_ClassIDInstantiator.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-06-13 vm Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "ds_Utils_StdErr.h"
#include "ds_Utils_CSSupport.h"
#include "ds_Net_ClassIDInstantiator.h"
#include "ds_Net_NetworkFactory.h"
#include "ds_Net_CNetworkFactory.h"

using namespace ds::Net;
using namespace ds::Error;


/*===========================================================================

                        PUBLIC MEMBER FUNCTIONS

===========================================================================*/
extern "C"
int dsNetNetworkFactoryCreateInstance
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

  if (AEECLSID_CNetworkFactory == clsID)
  {
    *newObjPtrPtr =
      NetworkFactory::CreateInstance(clsID);

    if (0 == *newObjPtrPtr)
    {
      LOG_MSG_ERROR_0("dsNetNetworkFactoryCreateInstance(): "
                      "Couldn't allocate NetworkFactory");
      return AEE_ENOMEMORY;
    }
  }

  return AEE_SUCCESS;

} /* dsNetNetworkFactoryCreateInstance() */

