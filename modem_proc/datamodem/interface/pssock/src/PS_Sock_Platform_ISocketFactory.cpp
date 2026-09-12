/*===========================================================================
  FILE: PS_Sock_Platform_ISocketFactory.cpp

  OVERVIEW: This file implements the CreateInstance() method of
  PS::Sock::Platform::ISocketFactory class.

  DEPENDENCIES: None

  Copyright (c) 2008 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_Platform_ISocketFactory.cpp#2 $
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

#include "PS_Sock_Platform_SocketFactory.h"

using namespace PS::Sock::Platform;


/*===========================================================================

                         PUBLIC MEMBER FUNCTIONS

===========================================================================*/
ISocketFactory * ISocketFactory::CreateInstance
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return SocketFactory::CreateInstance();

} /* ISocketFactory::CreateInstance() */

