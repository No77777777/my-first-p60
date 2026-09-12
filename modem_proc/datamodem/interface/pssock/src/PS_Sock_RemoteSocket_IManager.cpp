/*===========================================================================
  FILE: PS_Sock_RemoteSocket_IManager.cpp

  OVERVIEW: This file is implementation for the interface RemoteSocketManager

  DEPENDENCIES: None

  Copyright (c) 2015 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_RemoteSocket_IManager.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"

#include "PS_Sock_RemoteSocket_Manager.h"

using namespace PS::Sock::RemoteSocket;


/*===========================================================================

                         PUBLIC MEMBER FUNCTIONS

===========================================================================*/
IManager * IManager::GetInstance
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return Manager::GetInstance();

} /* IManager::GetInstance() */

IManager::~IManager()
{

}/* IManager::~IManager() */