/*===========================================================================
  FILE: ds_Sock_Init.cpp

  OVERVIEW: This file provides functions that are used to perform DSSock module
  initialization

  DEPENDENCIES: None

  Copyright (c) 2008 - 2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_Init.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-14 msr Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ds_Sock_Init.h"
#include "ds_Sock_MemManager.h"
#include "ds_Sock_EventManager.h"
#include "ds_Utils_DebugMsg.h"

#include "ds_Sock_ISocketFactory.h"
#include "ds_Sock_CreateInstance.h"
#include "ds_Sock_CSocketFactory.h"
#include "ds_Utils_CSSupport.h"

using namespace ds::Sock;

static ISocketFactory *ISocketFactoryPtr = 0;
static ISocketFactory *ISocketFactoryPrivPtr = 0;


#define DS_SOCK_INIT_ASSERT(expression) \
         ds_sock_init_assert_wrapper(__LINE__, expression)

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
static void ds_sock_init_assert_wrapper
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

                           EXTERNAL FUNCTIONS

===========================================================================*/

void DSSockPowerup
(
  void
)
{
  int32  ret_val;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MemManager::MemPoolInit();

  /*-------------------------------------------------------------------------
    Create singletons and only release during powerdown
    Allocate a SocketFactory object if it is not already allocated.
  -------------------------------------------------------------------------*/

  ret_val = DSSockCreateInstance(0, 
                                 AEECLSID_CSocketFactory, 
                                 (void**)&ISocketFactoryPtr);
  if (AEE_SUCCESS != ret_val)
  {
    DS_SOCK_INIT_ASSERT(0);
    return;
  }

} /* DSSockPowerup() */

void DSSockPowerdown
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_UTILS_RELEASEIF(ISocketFactoryPrivPtr);
  DS_UTILS_RELEASEIF(ISocketFactoryPtr);
} /* DSPowerdown() */


void DSSockInit
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ds::Sock::EventManager::Init();

} /* DSSockInit() */

void DSSockDeinit
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  EventManager::Deinit();

} /* DSSockDeinit() */
