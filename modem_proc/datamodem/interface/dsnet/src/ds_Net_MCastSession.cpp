/*===========================================================================
  FILE: MCastSession.cpp

  OVERVIEW: This file provides implementation of the MCastSession class.

  DEPENDENCIES: None

  Copyright (c) 2007 - 2017 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_MCastSession.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2017-07-18 rk  Removed BCMCS feature.
  2008-03-25 hm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATUTE_DATA_PS_MCAST
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_MCastSession.h"
#include "ds_Net_Platform.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_Conversion.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSignalBus.h"

using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace ds::Error;

#define DS_NET_MCAST_ASSERT(expression) \
         dsnet_init_mcast_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_mcast_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_MCAST_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_mcast_assert_wrapper
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
MCastSession::MCastSession
(
  int32          ifaceHandle,
  int32          mcastHandle,
  MCastSessionCreationType creationType,
  MCastJoinFlagsType regFlag
)
: Handle (ifaceHandle),
  refCnt (1),
  weakRefCnt (1),
  mMCastHandle (mcastHandle),
  mStatusInfoCode(0),
  mSessionCreationType(creationType),
  mJoinFlag(regFlag)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("MCastSession::MCastSession(): "
                            "Obj 0x%p, iface handle 0x%x, mcast handle 0x%x",
                            this, ifaceHandle, mcastHandle);

  memset((void *) &mRegState, 0, sizeof(MCastStateChangedType));

  //TODO Move out to Init function. Go through constructors of all dsnet/dss
  //objects and fix this

  (void) DS_Utils_CreateInstance (NULL, AEECLSID_CSignalBus,
                                  (void **)(&mpSigBusRegStatus));
  (void) DS_Utils_CreateInstance (NULL, AEECLSID_CSignalBus,
                                  (void **)(&mpSigBusTechStatus));

  if (AEE_SUCCESS != Handle::Init(EventManager::mcastObjList))
  {
    LOG_MSG_ERROR_3 ("MCastSession::MCastSession: "
                     "Failed to init the Handle   "
                     "obj 0x%p, if handle 0x%x, mcastHandle 0x%x",
                     this, ifaceHandle, mcastHandle);
    return;
  }

} /* MCastSession::MCastSession() */

void MCastSession::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  DS_UTILS_RELEASEIF (mpSigBusRegStatus);
  DS_UTILS_RELEASEIF (mpSigBusTechStatus);

  /*-------------------------------------------------------------------------
    Should be the last statement. Call destructor of the base class.
  -------------------------------------------------------------------------*/
  Handle::Destructor();

  mpICritSect->Leave();
} /* MCastSession::Destructor() */

MCastSession::~MCastSession
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

} /* MCastSession::~MCastSession() */


/*---------------------------------------------------------------------------
  Inherited functions from IMCastSession.
---------------------------------------------------------------------------*/
/**
  @brief      This function issues an IOCTL request to leave the multicast
              group it is associated with.

              This function issues an IOCTL request to leave the multicast
              group it is associated with.

  @param      None

  @see        No external dependencies

  @return     AEE_SUCCESS - if the request is issued to leave the MCast group
              successfully. This does not mean that Mcast group is left
              already.
              Error code - on error.
*/
ds::ErrorType MCastSession::Leave
(
  void
)
{
  NetPlatform::MCastLeaveType       leaveInfo;
  ds::ErrorType                     result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Create arguments for MCAST_LEAVE ioctl.
  -------------------------------------------------------------------------*/
  memset (&leaveInfo, 0, sizeof (NetPlatform::MCastLeaveType));

  mpICritSect->Enter();

  leaveInfo.handle = mMCastHandle;

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to leave the MCast session.
  -------------------------------------------------------------------------*/
  result = NetPlatform::IfaceIoctl (GetHandle(),
                                    NetPlatform::IFACE_IOCTL_MCAST_LEAVE,
                                    static_cast <void *> (&leaveInfo));

  LOG_MSG_FUNCTION_EXIT_2 ("MCastSession::Leave(): Result 0x%x, obj 0x%p",
                           result, this);
  mpICritSect->Leave();

  return result;
} /* MCastSession::Leave() */


ds::ErrorType MCastSession::GetTechStatusInfoCode
(
  MCastInfoCodeType* pInfoCode
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pInfoCode)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  *pInfoCode = mStatusInfoCode;

  LOG_MSG_FUNCTION_EXIT_2 ("MCastSession::GetTechStatusInfoCode(): "
                           "Tech status %d, obj 0x%p", *pInfoCode, this);
  mpICritSect->Leave();

  return AEE_SUCCESS;
} /* MCastSession::GetTechStatusInfoCode() */


ds::ErrorType MCastSession::GetRegStateAndInfoCode
(
  MCastStateChangedType* RegStateAndInfoCode
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == RegStateAndInfoCode)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  *RegStateAndInfoCode = mRegState;

  LOG_MSG_FUNCTION_EXIT_3 ("MCastSession::GetRegStateAndInfoCode(): "
                           "Reg state %d, info code %d, obj 0x%p",
                           RegStateAndInfoCode->regState,
                           RegStateAndInfoCode->infoCode,
                           this);
  mpICritSect->Leave();

  return AEE_SUCCESS;
} /* MCastSession::GetRegStateAndInfoCode() */


/*---------------------------------------------------------------------------
  Inherited function from ds::Utils::INode interface.
---------------------------------------------------------------------------*/
/**
  @brief      This method implements the INode interface's Process() function.

              This function is used in case of event handling. Whenever an
              event occurs the event manager singleton traverses the
              network factory. On each network object this Process() function
              is called. If it is a MCast event, the network object calls
              Traverse on the MCastManager. MCastManager calls this Process()
              function on each of the MCast nodes. This Process() function is
              handled thus:

              Switch (EventGroup)
                Case EVENT_GROUP_MCAST:
                  If this MCast object registered for the event
                    If this->handle == eventinfo->handle
                    Notify()
                    Return TRUE.
                  End.
                  Return TRUE.

                Case Default:
                  ASSERT (0)
                  Return FALSE
              End

  @param[in]  userDataPtr: Event info for the event.

  @see        No external dependencies

  @return     TRUE - If the Process() succeeds.
              FALSE - If the Process() fails.
*/
boolean MCastSession::Process
(
  void * userDataPtr
)
{
  EventInfoType *                      pEventInfo = 0;
  ps_iface_mcast_event_info_type *     pPSMCastEventInfo = 0;
  ds::ErrorType                        result = AEE_SUCCESS;
  ps_iface_mcast_info_code_enum_type   mappedInfoCode;
  boolean                              retVal = FALSE;
  int8 infoCodeVersion = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == userDataPtr)
  {
    LOG_MSG_ERROR_1 ("MCastSession::Process(): NULL event info, obj 0x%p",
                     this);
    return FALSE;
  }

  pEventInfo = static_cast <EventInfoType *>(userDataPtr);

  mpICritSect->Enter();

  LOG_MSG_INFO1_5 ("MCastSession::Process(): Handle 0x%x, event 0x%x, "
                   "group %d, event handle 0x%x, obj 0x%p",
                   mMCastHandle,
                   pEventInfo->eventName,
                   pEventInfo->eventGroup,
                   pEventInfo->handle,
                   this);

  do
  {
    if (mMCastHandle == pEventInfo->userHandle)
    {
      /*---------------------------------------------------------------------
        Set the MCast Registration status depending upon event.
      ---------------------------------------------------------------------*/
      switch (pEventInfo->psEventName)
      {
        case IFACE_MCAST_REGISTER_SUCCESS_EV:
        {
          mRegState.regState = MCastRegState::MCAST_REGISTER_SUCCESS;
          break;
        }

        case IFACE_MCAST_REGISTER_FAILURE_EV:
        {
          mRegState.regState = MCastRegState::MCAST_REGISTER_FAILURE;
          break;
        }

        case IFACE_MCAST_DEREGISTERED_EV:
        {
          mRegState.regState = MCastRegState::MCAST_DEREGISTERED;
          break;
        }
      } /* switch */

      /*---------------------------------------------------------------------
        Set the MCast technology status depending upon event info.
      ---------------------------------------------------------------------*/
      pPSMCastEventInfo =
        (ps_iface_mcast_event_info_type *) pEventInfo->psEventInfo;

      /*---------------------------------------------------------------------
        If this MCast session was created by JoinBundle call - only BCMCS 2.0,
        DVBH and MFLO events are fired, otherwise (created by Join call) -
        BCMCS 1.0 are translated to BCMCS 2.0 events, DVBH and MFLO events
        fired as is.
      ---------------------------------------------------------------------*/
      if (MCastSessionCreation::MCAST_SESSION_CREATED_BY_JOIN ==
            mSessionCreationType)
      {
        /* BCMCS code removed and not supported */
        if (MCastInfoCodeIsDVBHOrMFLO(pPSMCastEventInfo->info_code))
        {
          if (IFACE_MCAST_STATUS_EV == pEventInfo->psEventName)
          {
            mStatusInfoCode = (MCastInfoCodeType) pPSMCastEventInfo->info_code;
          }
          else
          {
            mRegState.infoCode =
              (MCastInfoCodeType) pPSMCastEventInfo->info_code;
          }
        }
        else /* BCMCS 2.0 events are filtered out */
        {
          infoCodeVersion = 2;
          retVal = TRUE;
          break;
        }
      }
      else /* created by call to JoinBundle()*/
      {
        if (MCastInfoCodeIsDVBHOrMFLO(pPSMCastEventInfo->info_code) ||
                 PS_IFACE_MCAST_MBMS_SYSTEM_UNAVAILABLE ==
                   pPSMCastEventInfo->info_code)
        {
          DS_NET_MCAST_ASSERT(0);
          retVal = TRUE;
          break;
        }
        else /* BCMCS 2.0 */
        {
          if (IFACE_MCAST_STATUS_EV == pEventInfo->psEventName)
          {
            mStatusInfoCode = (MCastInfoCodeType) pPSMCastEventInfo->info_code;
          }
          else
          {
            mRegState.infoCode =
              (MCastInfoCodeType) pPSMCastEventInfo->info_code;
          }
        }
      }

      /*-----------------------------------------------------------------------
        Default back to Handle::Process()
      -----------------------------------------------------------------------*/
      retVal = Handle::Process(userDataPtr);
    }
  } while (0);

  mpICritSect->Leave();

  if(infoCodeVersion != 0)
  {
    LOG_MSG_INFO1_2 ("MCastSession::Process(): "
                           "Info code %d is ver %d.0",
                           pPSMCastEventInfo->info_code, infoCodeVersion);
  }
  return retVal;

} /* MCastSession::Process() */

ds::ErrorType MCastSession::GetSignalBus
(
  ds::Net::EventType  eventID,
  ISignalBus **       ppISigBus
)
{
  ds::ErrorType  result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("MCastSession::GetSignalBus(): "
                            "Obj 0x%p, event 0x%x, ppISigBus 0x%x",
                             this, eventID, ppISigBus);

  if (0 == ppISigBus)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (eventID)
  {
    case  MCastSessionEvent::QDS_EV_REGISTRATION_STATE:
    {
      *ppISigBus = mpSigBusRegStatus;
      break;
    }

    case  MCastSessionEvent::QDS_EV_TECHNOLOGY_STATUS:
    {
      *ppISigBus = mpSigBusTechStatus;
      break;
    }

    default:
    {
      LOG_MSG_INVALID_INPUT_2 ("MCastSession::GetSignalBus(): "
                               "Unknown event 0x%x, obj 0x%p", eventID, this);
      result = QDS_EINVAL;
      break;
    }
  } /* switch (event ID) */

  /*-------------------------------------------------------------------------
    Handle the case where event is supported by this object.
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS == result)
  {
    if (0 == *ppISigBus)
    {
      result = QDS_EINVAL;
    }
    else
    {
      result = AEE_SUCCESS;
      (void) (*ppISigBus)->AddRef();
    }
  }

  LOG_MSG_FUNCTION_EXIT_3 ("MCastSession::GetSignalBus(): "
                           "Result 0x%x, signal bus 0x%p, eventID 0x%x, obj 0x%p",
                           result, *ppISigBus, eventID, this);
  mpICritSect->Leave();

  return result;
} /* MCastSession::GetSignalBus() */
#endif // FEATUTE_DATA_PS_MCAST
