/*===========================================================================
  FILE: NetworkFactory.cpp

  OVERVIEW: This file provides implementation of the NetworkFactory class.

  DEPENDENCIES: None

  Copyright (c) 2008-2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/


/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_NetworkFactory.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2013-11-01 rk  Removed feature mobileap.
  2010-05-13 ts  Removed CreateDefaultNetworkPriv. CreateNetworkPriv can
                 be used with NULL policy to achive the same functionality.
  2008-03-10 hm  Created module.

===========================================================================*/

/*===========================================================================

                        INCLUDE FILES FOR THE MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_StdErr.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Net_NetworkFactory.h"
#include "ds_Net_Network.h"
#include "ds_Net_NetworkActive.h"
#include "ds_Net_NetworkMonitored.h"
#include "ds_Net_CNetworkFactory.h"
#include "ds_Net_TechUMTSFactory.h"
#include "ds_Utils_CCritSect.h"

using namespace ds::Net;
using namespace ds::Error;

#define DS_NET_NETWORKFAC_ASSERT(expression) \
         dsnet_networkfactory_assert_wrapper(__LINE__, expression)

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
static void dsnet_networkfactory_assert_wrapper
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


/*---------------------------------------------------------------------------
  Static Data Member Definitions
---------------------------------------------------------------------------*/

/*===========================================================================

                        PRIVATE METHODS DEFINITION

===========================================================================*/

NetworkFactory::NetworkFactory
(
  void
)
throw()
: refCnt (0)
{
  if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL,
                                              AEECLSID_CCritSect,
                                             (void **) &mpICritSect))
  {
    DS_NET_NETWORKFAC_ASSERT(0);
  }

} /* NetworkFactory() */

NetworkFactory::~NetworkFactory
(
  void
)
throw()
{

  /*lint -save -e1550, -e1551 */
  DS_UTILS_RELEASEIF(mpICritSect);
  /*lint -restore */

} /* ~NetworkFactory() */

/*===========================================================================

                        PUBLIC METHODS DEFINITION

===========================================================================*/
int NetworkFactory::QueryInterface
(
  AEEIID iid,
  void **ppo
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_3 ("NetworkFactory::QueryInterface(): "
                   "Obj 0x%p, iid 0x%x, ppo 0x%p",
                   this, iid, ppo);

  if (NULL == ppo)
  {
    return QDS_EFAULT;
  }

  *ppo = NULL;

  switch (iid)
  {
  case AEEIID_INetworkFactory:
    *ppo = static_cast <INetworkFactory *> (this);
    break;

  case AEEIID_IQI:
    *ppo = reinterpret_cast <IQI *> (this);
    break;

  default:
    return AEE_ECLASSNOTSUPPORT;
  }

  (void) AddRef();

  return AEE_SUCCESS;
}/* QueryInterface() */

void * NetworkFactory::CreateInstance
(
  AEECLSID    clsID
)
{
  void               * retVal = 0;
  INetworkFactory    * piNetworkFactory = 0;
  bool                 createNetworkFactoryClientFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  switch (clsID)
  {
    /*-------------------------------------------------------------------------
      Note: this code can be called only from DS - AEECLSID_CNetworkFactory
    -------------------------------------------------------------------------*/
    case AEECLSID_CNetworkFactory:
      piNetworkFactory = (INetworkFactory*)GetInstance();
      if (0 == piNetworkFactory)
      {
        createNetworkFactoryClientFailed = true;
        goto bail;
      }

      (void)piNetworkFactory->AddRef();

      retVal = piNetworkFactory;

      break;
    default:
      goto bail;
  }

  return retVal;

bail:

  LOG_MSG_ERROR_2 ("NetworkFactory::CreateInstance(): "
                   "Class ID %0x, create NetworkFactoryClient failed %d",
                   clsID, createNetworkFactoryClientFailed);

  return retVal;
} /* CreateInstance() */

/*---------------------------------------------------------------------------
  Inherited function from INetworkFactory
---------------------------------------------------------------------------*/
int NetworkFactory::CreateNetwork
(
  NetworkModeType     networkMode,
  IPolicy*            pIPolicy,
  INetwork**          ppINetwork,
  int                 *netdownreason
)
{
  Network*            pINetwork = NULL;
  int32               result;
  int                 bIsDefaultPolicy = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("NetworkFactory::CreateNetwork(): "
                            "mode %d, network %d, policy 0x%x",
                            networkMode, ppINetwork,pIPolicy);

  /* Validate arguments */
  if (NULL == ppINetwork)
  {
    return QDS_EFAULT;
  }

  // if pIPolicy is NULL, default policy shall be used
  if (NULL == pIPolicy)
  {
    bIsDefaultPolicy = TRUE;
    pIPolicy = new Policy();
    if (NULL == pIPolicy)
    {
      LOG_MSG_ERROR_0 ("NetworkFactory::CreateNetwork(): "
                       "Cant create NetPolicy object");
      return AEE_ENOMEMORY;
    }
  }

  /* Initialize the rout arg to NULL */
  *ppINetwork = NULL;

  if(ds::Net::NetworkMode::QDS_ACTIVE == networkMode)
  {
    pINetwork = new NetworkActive(reinterpret_cast <Policy *> (pIPolicy));
  }
  else if(ds::Net::NetworkMode::QDS_MONITORED == networkMode)
  {
    pINetwork = new NetworkMonitored(reinterpret_cast <Policy *> (pIPolicy));
  }
  else
  {
    return QDS_EINVAL;
  }

  if (NULL == pINetwork)
  {
    result = AEE_ENOMEMORY;
    goto bail;
  }

  if (bIsDefaultPolicy)
  {
    DS_UTILS_RELEASEIF(pIPolicy);
  }
  
  result = pINetwork->Init();
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  if (ds::Net::NetworkMode::QDS_ACTIVE == networkMode)
  {
    result = pINetwork->BringUpInterface(netdownreason);
    if (AEE_SUCCESS != result && AEE_EWOULDBLOCK != result)
    {
      goto bail;
    }
  }

  *ppINetwork = static_cast <INetwork *> (pINetwork);

  return result;

bail:
  /* Error handling */
  LOG_MSG_ERROR_1 ("NetworkFactory::CreateNetwork(): "
                   "Err %d", result);
  DS_UTILS_RELEASEIF (pINetwork);
  
  if (bIsDefaultPolicy)
  {
    DS_UTILS_RELEASEIF(pIPolicy);
  }
  
  *ppINetwork = NULL;
  return result;

} /* CreateNetwork() */

ds::ErrorType NetworkFactory::CreateIPFilterSpec
(
 IIPFilterPriv** ppIIPFilterSpec
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == ppIIPFilterSpec)
  {
    return QDS_EFAULT;
  }

  *ppIIPFilterSpec = static_cast <IIPFilterPriv *>(new IPFilterSpec());
  if (NULL == *ppIIPFilterSpec)
  {
    return AEE_ENOMEMORY;
  }

  LOG_MSG_INFO1_1 ("NetworkFactory::CreateIPFilterSpec(): "
                   "Created 0x%p", *ppIIPFilterSpec);
  return AEE_SUCCESS;
}

ds::ErrorType NetworkFactory::CreateQoSFlowSpec
(
 IQoSFlowPriv** ppIQoSFlowSpec
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == ppIQoSFlowSpec)
  {
    return QDS_EFAULT;
  }

  *ppIQoSFlowSpec = static_cast <IQoSFlowPriv *> (new QoSFlowSpec());
  if (NULL == *ppIQoSFlowSpec)
  {
    return AEE_ENOMEMORY;
  }

  LOG_MSG_INFO1_1 ("NetworkFactory::CreateQoSFlowSpec(): "
                   "Created 0x%p", *ppIQoSFlowSpec);
  return AEE_SUCCESS;
}

ds::ErrorType NetworkFactory::CreatePolicy
(
  IPolicy**           ppIPolicy
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == ppIPolicy)
  {
    return QDS_EFAULT;
  }

  *ppIPolicy = static_cast <IPolicy *> (new Policy());
  if (NULL == *ppIPolicy)
  {
   return AEE_ENOMEMORY;
  }

  LOG_MSG_INFO1_2 ("NetworkFactory::CreatePolicy(): "
                   "Created Policy ptr 0x%p, object 0x%p",
                   *ppIPolicy, this);
  return AEE_SUCCESS;
}

ds::ErrorType NetworkFactory::CreateTechUMTS
(
  ITechUMTS **         newTechUMTS
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == newTechUMTS)
  {
    return QDS_EFAULT;
  }

  *newTechUMTS = static_cast <ITechUMTS *> (TechUMTSFactory::GetInstance());
  if (NULL == *newTechUMTS)
  {
    return AEE_ENOMEMORY;
  }
  (void) (*newTechUMTS)->AddRef();

  LOG_MSG_INFO1_1 ("NetworkFactory::CreateTechUMTS(): "
                   "Created 0x%p", *newTechUMTS);
  return AEE_SUCCESS;
}

ds::ErrorType NetworkFactory::CreatePolicyPriv
(
  IPolicyPriv** ppIPolicyPriv
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   if (NULL == ppIPolicyPriv)
   {
     return QDS_EFAULT;
   }

   *ppIPolicyPriv = static_cast <IPolicyPriv *> (new Policy());
   if (NULL == *ppIPolicyPriv)
   {
     return AEE_ENOMEMORY;
   }

   return AEE_SUCCESS;
}
