/*===========================================================================
  FILE: NetworkUMTS.cpp

  OVERVIEW: This file provides implementation of the NetworkUMTS class.

  DEPENDENCIES: None

  Copyright (c) 2007 - 2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_NetworkUMTS.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-04-06 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_NetworkUMTS.h"
#include "ds_Net_Platform.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CCritSect.h"

using namespace ds::Net;
using namespace ds::Error;
using namespace NetPlatform;

#define DS_NET_UMTS_ASSERT(expression) \
         dsnet_umts_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_init_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_INIT_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_umts_assert_wrapper
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
}/* dsnet_init_assert_wrapper */


/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
NetworkUMTS::NetworkUMTS
(
  Network* pParent
)
: refCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Store the parent, perform AddRefWeak() */
  DS_NET_UMTS_ASSERT (NULL != pParent);

  mpParent = pParent;

  if(NULL != mpParent)
  {
    (void) mpParent->AddRefWeak();
  }

  if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL, 
                                              AEECLSID_CCritSect,
                                             (void **) &mpICritSect))
  {
    DS_UTILS_ERROR_FATAL("NetworkUMTS::NetworkUMTS(): Cannot create critical section");
  }

} /* NetworkUMTS::NetworkUMTS() */

NetworkUMTS::~NetworkUMTS
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

  /*lint -save -e1550, -e1551 */
  DS_UTILS_RELEASEIF(mpICritSect);
  DS_UTILS_RELEASE_WEAKREF_IF(mpParent);
  /*lint -restore */

} /* ~NetworkUMTS() */


/*---------------------------------------------------------------------------
  Functions inherited from INetworkUMTS
---------------------------------------------------------------------------*/
ds::ErrorType NetworkUMTS::GetIMCNFlag
(
  UMTSIMCNFlagType* imcnFlag
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (mpParent->GetHandle(),
                               IFACE_IOCTL_UMTS_GET_IM_CN_FLAG,
                                 static_cast <void *> (imcnFlag));

  mpICritSect->Leave();

  return result;

} /* NetworkUMTS::GetIMCNFlag() */

ds::ErrorType NetworkUMTS::QueryInterface
(
  AEEIID iid,
  void **ppo
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (iid)
  {
  case AEEIID_INetwork:
    case AEEIID_IQI:
    {
    *ppo = static_cast <INetwork *> (mpParent);
      result = AEE_SUCCESS;

    (void) mpParent->AddRef();
    break;
    }

  case AEEIID_INetworkUMTS:
    {
    *ppo = static_cast <INetworkUMTS *>(this);
      result = AEE_SUCCESS;

    (void) AddRef();
    break;
    }

  default:
    {
      result = AEE_ECLASSNOTSUPPORT;
      break;
  }
  }

  LOG_MSG_FUNCTION_EXIT_4 ("NetworkUMTS::QueryInterface(): "
                           "Result 0x%x, ppo 0x%p, obj 0x%p, iid 0x%x",
                           result, *ppo, this, iid);

  mpICritSect->Leave();

  return result;
} /* NetworkUMTS::QueryInterface() */
