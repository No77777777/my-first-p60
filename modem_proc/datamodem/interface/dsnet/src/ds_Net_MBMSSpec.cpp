/*===========================================================================
  FILE: MBMSSpec.cpp

  OVERVIEW: This file provides implementation of the MBMSSpec class.

  DEPENDENCIES: None

  Copyright (c) 2008-2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_MBMSSpec.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2008-08-10 hm  Created module.

===========================================================================*/

/*===========================================================================

                            INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATUTE_DATA_PS_MCAST
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_CSSupport.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_MBMSSpec.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CCritSect.h"

using namespace ds::Net;
using namespace ds::Error;

#define DS_NET_MBMS_ASSERT(expression) \
         dsnet_mbms_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_mbms_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_MBMS_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_mbms_assert_wrapper
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
                             CONSTRUCTOR/DESTRUCTOR
===========================================================================*/

MBMSSpec::MBMSSpec
(
  void 
):mTmgi (0), 
  mSessionStartTime (0),
  mSessionEndTime (0),
  mPriority (0),
  mServiceType (0),
  mServiceMethodType (0),
  mSelectedService (0),
  mServiceSecurity (0),
  refCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL, 
                                              AEECLSID_CCritSect,
                                              (void **) &mpICritSect))
  {
    DS_UTILS_ERROR_FATAL("MBMSSpec::MBMSSpec(): Cannot create crit sect");
  }
}

MBMSSpec::~MBMSSpec
(
  void 
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_UTILS_RELEASEIF(mpICritSect);

  if (0 != refCnt)
  {
    DS_NET_MBMS_ASSERT (0);
  }
}

/*===========================================================================
                             PUBLIC METHODS
===========================================================================*/
int MBMSSpec::GetTMGI (uint64* tmgi)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == tmgi)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();
  *tmgi = mTmgi;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::SetTMGI (uint64 tmgi)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();
  mTmgi = tmgi;
  mpICritSect->Leave();

  return AEE_SUCCESS;
}

int MBMSSpec::GetSessionStartTime (uint64* sessionStartTime)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == sessionStartTime)
  {
    return QDS_EFAULT;
  }
  mpICritSect->Enter();
  *sessionStartTime = mSessionStartTime;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::SetSessionStartTime (uint64 sessionStartTime)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();
  mSessionStartTime = sessionStartTime;
  mpICritSect->Leave();

  return AEE_SUCCESS;
}

int MBMSSpec::GetSessionEndTime (uint64* sessionEndTime)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == sessionEndTime)
  {
    return QDS_EFAULT;
  }
  mpICritSect->Enter();
  *sessionEndTime = mSessionEndTime;  
  mpICritSect->Leave();

  return AEE_SUCCESS;
}

int MBMSSpec::SetSessionEndTime (uint64 sessionEndTime)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();
  mSessionEndTime = sessionEndTime;
  mpICritSect->Leave();

  return AEE_SUCCESS;
}

int MBMSSpec::GetPriority (unsigned short int* priority)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == priority)
  {
    return QDS_EFAULT;
  }
  mpICritSect->Enter();
  *priority = mPriority;
  mpICritSect->Leave();

  return AEE_SUCCESS;
}

int MBMSSpec::SetPriority (unsigned short int priority)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();
  mPriority = priority;
  mpICritSect->Leave();

  return AEE_SUCCESS;
}

int MBMSSpec::GetService (MBMSServiceType* service)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == service)
  {
    return QDS_EFAULT;
  }
  
  mpICritSect->Enter();
  *service = mServiceType;
  mpICritSect->Leave();

  return AEE_SUCCESS;
}

int MBMSSpec::SetService (MBMSServiceType service)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();
  mServiceType = service;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::GetServiceMethod (MBMSServiceMethodType* serviceMethod)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == serviceMethod)
  {
    return QDS_EFAULT;
  }
  mpICritSect->Enter();
  *serviceMethod = mServiceMethodType;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::SetServiceMethod (MBMSServiceMethodType serviceMethod)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();
  mServiceMethodType = serviceMethod;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::GetSelectedService (boolean* selectedService)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == selectedService)
  {
    return QDS_EFAULT;
  }
  mpICritSect->Enter();
  *selectedService = mSelectedService;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::SetSelectedService (boolean selectedService)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();
  mSelectedService = selectedService;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::GetServiceSecurity (boolean* serviceSecurity)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == serviceSecurity)
  {
    return QDS_EFAULT;
  }
  mpICritSect->Enter();
  *serviceSecurity = mServiceSecurity;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

int MBMSSpec::SetServiceSecurity (boolean serviceSecurity)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();
  mServiceSecurity = serviceSecurity;
  mpICritSect->Leave();
  return AEE_SUCCESS;
}

ds::ErrorType MBMSSpec::QueryInterface 
(
  AEEIID iid, 
  void **ppo
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == ppo)
  {
    LOG_MSG_ERROR_0 ("MBMSSpec::QueryInterface(): "
                     "NULL args");
    return QDS_EFAULT;
  }

  switch (iid)
  {
    case AEEIID_IMBMSSpecPriv:
      *ppo = static_cast <IMBMSSpecPriv *> (this);
      (void) AddRef ();
      break;

    case AEEIID_IQI:
      *ppo = reinterpret_cast <IQI *> (this);
      (void) AddRef ();
      break;
           
    default:
      return AEE_ECLASSNOTSUPPORT;
  }

  return AEE_SUCCESS;
}
#endif //FEATUTE_DATA_PS_MCAST
