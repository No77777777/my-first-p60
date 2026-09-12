/*===========================================================================
  FILE: MCastManagerPriv.cpp

  OVERVIEW: This file provides implementation of the MCastManagerPriv class.

  DEPENDENCIES: None

  Copyright (c) 2010 -2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_MCastManagerPriv.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2010-07-29 vm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATUTE_DATA_PS_MCAST
#include "comdef.h"
#include "ds_Utils_StdErr.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Net_Utils.h"
#include "ds_Net_MCastManagerPriv.h"
#include "ds_Net_MCastSessionPriv.h"
#include "ds_Net_Platform.h"
#include "ds_Net_Conversion.h"
#include "ds_Utils_CCritSect.h"
#include "ds_Net_MBMSSpec.h"
#include "ds_Net_MCastMBMSCtrl.h"
#include "ps_system_heap.h"
#include "dss_errors_def.h"

using namespace ds::Error;
using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace NetPlatform;
using namespace dss::Error;


MCastManagerPriv::MCastManagerPriv
(
  int32      ifaceHandle
)
: mIfaceHandle (ifaceHandle),
  refCnt(1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2 ("MCastManagerPriv::MCastManagerPriv(): "
                            "Obj 0x%p, iface handle 0x%x", this, ifaceHandle);

  //TODO Move out to Init function. Go through constructors of all dsnet/dss
  //objects and fix this
  if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL,
                                              AEECLSID_CCritSect,
                                              (void **) &mpICritSect))
  {
    LOG_MSG_ERROR_1 ("MCastManagerPriv::MCastManagerPriv(): "
                     "Failed to alloc CritSect, obj 0x%p", this);
    return;
  }
}  /* MCastManagerPriv::MCastManagerPriv() */

MCastManagerPriv::~MCastManagerPriv
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mIfaceHandle = 0;

  /*lint -save -e1550, -e1551 */
  DS_UTILS_RELEASEIF(mpICritSect);
  /*lint -restore */

} /* MCastManagerPriv::~MCastManagerPriv() */


//TODO Need to undo IFACE_IOCTL_MCAST_JOIN in case of error in below function
ds::ErrorType MCastManagerPriv::Join
(
  const ::ds::SockAddrStorageType  addr,
  IQI *                            info,
  IMCastSessionPriv **             session
)
{
  MCastJoinType  mcastJoinInfo;
  ds::ErrorType  result;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == session || 0 == addr)
  {
    LOG_MSG_INVALID_INPUT_3 ("MCastManagerPriv::Join(): NULL arg, obj 0x%p"
                             "session 0x%p, addr 0x%x",
                             this, session, addr);
    return QDS_EFAULT;
  }

  /*-------------------------------------------------------------------------
    info parameter is for the future use
  -------------------------------------------------------------------------*/
  (void) info;

  /*-------------------------------------------------------------------------
    Validate MCast join info and create IOCTL arguments to perform
    iface level IOCTL to join the multicast group.
  -------------------------------------------------------------------------*/
  memset (&mcastJoinInfo, 0, sizeof (MCastJoinType));
  result = DS2PSMCastJoinSpec (addr, &mcastJoinInfo);
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_INVALID_INPUT_1 ("MCastManager::Join(): Failed to convert "
                             "MCast spec from DS to PS format, obj 0x%p",
                             this);
    return result;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Perform Mcast join IOCTL on the iface handle to join the multicast
      group. Create a MCastSessionPriv object for the session after the IOCTL
      is issued.
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (mIfaceHandle,
                         IFACE_IOCTL_MCAST_JOIN,
                         static_cast <void *> (&mcastJoinInfo));
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_IFACE_IOCTL_MCAST_JOIN_FAILED;
      break;
    }

    /*-----------------------------------------------------------------------
      Use the Mcast handle obtained from the IOCTL to create a MCast
      session object.
    -----------------------------------------------------------------------*/
    *session = new MCastSessionPriv (mIfaceHandle,
                                     mcastJoinInfo.handle,
                                     MCastJoinFlags::REG_SETUP_ALLOWED);
    if (0 == *session)
    {
      ds_net_error_code = DSS_ERROR_MCAST_SESSION_ALLOC_FAILED;
      result = AEE_ENOMEMORY;
      break;
    }

  } while (0);

  LOG_MSG_FUNCTION_EXIT_3( "MCastManagerPriv::Join(): "
                           "Result 0x%x, MCast session 0x%p, obj 0x%p,
                           error code %d",
                           result, *session, this, ds_net_error_code);
  mpICritSect->Leave();

  return result;
} /* MCastSessionPriv::Join() */

//TODO Need to undo IFACE_IOCTL_MCAST_JOIN_EX in case of error in below function
ds::ErrorType MCastManagerPriv::JoinBundle
(
  const ::ds::SockAddrStorageType * addrSeq,
  int                               addrSeqLen,
  IQI **                            mcastSpecInfoSeq,
  int                               mcastSpecInfoSeqLen,
  const MCastJoinFlagsType *        mcastJoinFlagsSeq,
  int                               mcastJoinFlagsSeqLen,
  IMCastSessionPriv **              sessions,
  int                               sessionsLen,
  int *                             sessionsLenReq
)
{
  MCastJoinExType      mcastJoinExInfo;
  ds::ErrorType        result;
  uint32               index;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_6("MCastManagerPriv::JoinBundle(): "
                           "Obj 0x%p, addrSeq 0x%p, sessions 0x%p, JoinFlagsSeq"
                           "0x%p, addrSeqLen %d, JoinFlagsSeqLen %d", this,
                           addrSeq, sessions , mcastJoinFlagsSeq, addrSeqLen,
                           mcastJoinFlagsSeqLen);

  if (0 == addrSeq || 0 == sessions || 0 == mcastJoinFlagsSeq)
  {
    return QDS_EFAULT;
  }

  if (0 >= addrSeqLen || addrSeqLen != mcastJoinFlagsSeqLen ||
      addrSeqLen != sessionsLen)
  {
    return QDS_EINVAL;
  }

  /*-------------------------------------------------------------------------
    mcastSpecInfoSeq and mcastSpecInfoSeqLen parameters are for future use
  -------------------------------------------------------------------------*/
  (void) mcastSpecInfoSeq;
  (void) mcastSpecInfoSeqLen;

  /*-------------------------------------------------------------------------
    sessionsLenReq should be populated with sessionsLen.
  -------------------------------------------------------------------------*/
  if (0 != sessionsLenReq)
  {
    *sessionsLenReq = sessionsLen;
  }

  /*-------------------------------------------------------------------------
    Construct JOIN_EX IOCTL args.
  -------------------------------------------------------------------------*/
  memset (&mcastJoinExInfo, 0, sizeof (MCastJoinExType));
  result = DS2PSMCastJoinExSpec (addrSeq,
                                 addrSeqLen,
                                 &mcastJoinExInfo,
                                 mcastJoinFlagsSeq);
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_INVALID_INPUT_1 ("MCastManagerPriv::JoinBundle(): Failed to convert "
                             "MCast spec from DS to PS format, obj 0x%p",
                             this);
    return result;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Issue a MCAST_JOIN_EX iface IOCTL to request for a bundle MCAST join
      IOCTLS. Create corresponding Mcast session objects (MCastSessionPriv
      objects) associated with these MCast Handles.
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (mIfaceHandle,
                         IFACE_IOCTL_MCAST_JOIN_EX,
                         static_cast <void *> (&mcastJoinExInfo));
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_IFACE_IOCTL_MCAST_JOIN_FAILED;
      break;
    }

    /*-----------------------------------------------------------------------
      Create MCastSessionPriv objects for all of these MCast handles.
    -----------------------------------------------------------------------*/
    memset (sessions,
            0,
            sizeof (MCastSessionPriv *) * mcastJoinExInfo.num_flows);

    for (index = 0; index < mcastJoinExInfo.num_flows; index++)
    {
      sessions[index] =
        new MCastSessionPriv (mIfaceHandle,
                              mcastJoinExInfo.handle[index],
                              mcastJoinFlagsSeq[index]);
      if (0 == sessions[index])
      {
        ds_net_error_code = DSS_ERROR_MCAST_SESSION_ALLOC_FAILED;
        result = AEE_ENOMEMORY;
        break;
      }
    }

    /*-----------------------------------------------------------------------
      Update the sessionsLenReq argument.
    -----------------------------------------------------------------------*/
    if (0 != sessionsLenReq)
    {
      *sessionsLenReq = mcastJoinExInfo.num_flows;
    }
  } while (0);

  /*-------------------------------------------------------------------------
    Release array of MCastSession objects in case of failure
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS != result)
  {
    for (index = 0; index < mcastJoinExInfo.num_flows; index++)
    {
      /*---------------------------------------------------------------------
        MCastSessionsOutput object holds the reference to MCastSessions we
        created, so here we can release them.
      ---------------------------------------------------------------------*/
      DS_UTILS_RELEASEIF( sessions[index]);
    }
  }

  LOG_MSG_FUNCTION_EXIT_2( "MCastManagerPriv::JoinBundle(): "
                           "Result 0x%x, obj 0x%p, error code %d",
                           result, this, ds_net_error_code);
  mpICritSect->Leave();

  return result;
} /* MCastManagerPriv::JoinBundle() */


ds::ErrorType MCastManagerPriv::LeaveBundle
(
  IMCastSessionPriv **  sessions,
  int                   sessionsLen
)
{
  MCastSessionPriv *  mcastSessionObj;
  MCastLeaveExType    leaveExInfo;
  ds::ErrorType       result;
  int                 index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3("MCastManagerPriv::LeaveBundle(): "
                           "Obj 0x%p, sessions 0x%p, sessionsLen %d",
                           this, sessions, sessionsLen);

  if (0 == sessions)
  {
    return QDS_EFAULT;
  }

  if (0 >= sessionsLen)
  {
    return QDS_EINVAL;
  }

  /*-------------------------------------------------------------------------
    Validate sessions argument and construct MCAST_LEAVE_EX IOCTL arguments.
  -------------------------------------------------------------------------*/
  memset (&leaveExInfo, 0, sizeof (MCastLeaveExType));
  leaveExInfo.num_flows = (uint8) sessionsLen;

  for (index = 0; index < sessionsLen; index++)
  {
    if (0 == sessions[index])
    {
      LOG_MSG_INVALID_INPUT_2 ("MCastManagerPriv::LeaveBundle(): "
                               "NULL arg at index %d, obj 0x%p", index, this);
      return QDS_EFAULT;
    }

    mcastSessionObj = reinterpret_cast <MCastSessionPriv *> (sessions[index]);
    leaveExInfo.handle[index] = mcastSessionObj->GetMCastHandle();
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call bundled leave IOCTL on underlying iface to leave the mcast group.
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (mIfaceHandle,
                       IFACE_IOCTL_MCAST_LEAVE_EX,
                       static_cast <void *> (&leaveExInfo));

  LOG_MSG_FUNCTION_EXIT_2( "MCastManagerPriv::LeaveBundle(): "
                           "Result 0x%x, obj 0x%p", result, this);
  mpICritSect->Leave();

  return result;
} /* MCastManagerPriv::LeaveBundle() */

ds::ErrorType MCastManagerPriv::RegisterBundle
(
  IMCastSessionPriv **  sessions,
  int                   sessionsLen
)
{
  MCastSessionPriv *        mcastSessionObj;
  MCastRegisterExType       regExInfo;
  ds::ErrorType             result;
  int                       index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("MCastManagerPriv::RegisterBundle(): "
                            "obj 0x%p, sessions 0x%p, sessions len %d",
                            this, sessions, sessionsLen);

  if (0 == sessions)
  {
    return QDS_EFAULT;
  }

  if (0 >= sessionsLen)
  {
    return QDS_EINVAL;
  }

  /*-------------------------------------------------------------------------
    Construct MCAST_REG_EX IOCTL arguments.
    Register can be called only on sessions group created from
    call to JoinBundle with all regFlags == REG_SETUP_NOT_ALLOWED
  -------------------------------------------------------------------------*/
  memset (&regExInfo, 0, sizeof (MCastRegisterExType));
  regExInfo.num_flows = (uint8) sessionsLen;

  for (index = 0; index < sessionsLen; index++)
  {
    if (0 == sessions[index])
    {
      LOG_MSG_INVALID_INPUT_2 ("MCastManagerPriv::RegisterBundle(): "
                               "NULL arg at index %d, obj 0x%p", index, this);
      return QDS_EFAULT;
    }

    mcastSessionObj = reinterpret_cast <MCastSessionPriv *> (sessions[index]);

    if (MCastJoinFlags::REG_SETUP_NOT_ALLOWED != mcastSessionObj->GetRegFlag())
    {
      LOG_MSG_INVALID_INPUT_2 ("MCastManagerPriv::RegisterBundle(): "
                               "MCast session at index %d doesn't allow "
                               "registration, obj 0x%p", index, this);
      return QDS_EINVAL;
    }

    regExInfo.handle[index] = mcastSessionObj->GetMCastHandle();
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call bundled REG_EX IOCTL on underlying iface.
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (mIfaceHandle,
                       IFACE_IOCTL_MCAST_REGISTER_EX,
                       static_cast <void *> (&regExInfo));

  mpICritSect->Leave();

  return result;
} /* MCastManagerPriv::RegisterBundle() */


ds::ErrorType  MCastManagerPriv::CreateMBMSSpecPriv
(
  IMBMSSpecPriv **  ppIMBMSSpec
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppIMBMSSpec)
  {
    return QDS_EFAULT;
  }

  *ppIMBMSSpec = static_cast <IMBMSSpecPriv *> (new MBMSSpec());
  if (0 == *ppIMBMSSpec)
  {
    LOG_MSG_ERROR_1 ("MCastManagerPriv::CreateMBMSSpecPriv(): "
                     "Failed to alloc MBMSSpec, obj 0x%p", this);
    return AEE_ENOMEMORY;
  }

  return AEE_SUCCESS;
} /* MCastManagerPriv::CreateMBMSSpecPriv() */

ds::ErrorType MCastManagerPriv::GetTechObject
(
  AEEIID   iid,
  void **  ppo
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
    case AEEIID_IMCastManagerPriv:
    case AEEIID_IQI:
    {
      result = QueryInterface(iid, ppo);
      break;
    }

    default:
    {
      result = AEE_ECLASSNOTSUPPORT;
      break;
    }
  }

  LOG_MSG_FUNCTION_EXIT_4( "MCastManagerPriv::CreateMCastSessionsInput(): "
                           "Result 0x%x, ppo 0x%x, obj 0x%p, iid  0x%x",
                           result, *ppo, this, iid);
  mpICritSect->Leave();

  return result;
} /* MCastManagerPriv::GetTechObject() */

ds::ErrorType MCastManagerPriv::QueryInterface
(
  AEEIID   iid,
  void **  ppo
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
    case AEEIID_IMCastManagerPriv:
    case AEEIID_IQI:
    {
      *ppo   = static_cast <IMCastManagerPriv *>(this);
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

  LOG_MSG_FUNCTION_EXIT_4 ("MCastManagerPriv::QueryInterface(): "
                           "Result 0x%x, ppo 0x%p, obj 0x%p, iid 0x%x",
                           result, *ppo, this, iid);
  mpICritSect->Leave();

  return result;
} /* MCastManagerPriv::QueryInterface() */
#endif // FEATUTE_DATA_PS_MCAST
