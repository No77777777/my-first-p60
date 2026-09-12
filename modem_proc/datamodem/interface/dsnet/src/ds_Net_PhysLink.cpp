/*=========================================================================*/
/*!
  @file
  ds_Net_PhysLink.cpp

  @brief
  This file provides implementation of the PhysLink class.

  Copyright (c) 2008-2021 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_PhysLink.cpp#3 $
  $DateTime: 2021/05/25 01:37:25 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2008-03-25 hm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include <stringl/stringl.h>
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_PhysLink.h"
#include "ds_Net_Platform.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_Conversion.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSignalBus.h"

using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace ds::Error;
using namespace NetPlatform;

/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
PhysLink::PhysLink
(
  int32            physLinkHandle,
  NetworkModeType  networkMode
)
: Handle(physLinkHandle),
  mpSigBusStateChange(0),
  mpSigBusRanAsstMediaAdapt(0),
  mpSigBusPacketDiscard(0),
  mNetworkMode(networkMode),
  mCachedPreviousState(PhysLinkState::QDS_DOWN),
  refCnt (1),
  weakRefCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("PhysLink::PhysLink(): "
                            "Obj 0x%p, handle 0x%x, mode %d",
                            this, physLinkHandle, mNetworkMode);

  //TODO Need Init method
  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,
                                 (void **) &mpSigBusStateChange);
  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,
                                 (void **) &mpSigBusRanAsstMediaAdapt);

  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,
                                 (void **) &mpSigBusPacketDiscard);

  if (AEE_SUCCESS != Handle::Init(EventManager::physLinkObjList))
  {
    LOG_MSG_ERROR_2 ("PhysLink::PhysLink():"
                     "Failed to init the Handle   "
                     "obj 0x%p, physLinkHandle 0x%x", this, physLinkHandle);
    return;
  }

} /* PhysLink::PhysLink() */

void PhysLink::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  DS_UTILS_RELEASEIF (mpSigBusStateChange);
  DS_UTILS_RELEASEIF (mpSigBusRanAsstMediaAdapt);
  DS_UTILS_RELEASEIF (mpSigBusPacketDiscard);

  /*-------------------------------------------------------------------------
    Should be the last statement. Call destructor of the base class.
  -------------------------------------------------------------------------*/
  Handle::Destructor();

  mpICritSect->Leave();

} /* PhysLink::Destructor() */

PhysLink::~PhysLink
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

} /* PhysLink::~PhysLink() */


/*---------------------------------------------------------------------------
  Inherited functions from IPhysLink.
---------------------------------------------------------------------------*/
ds::ErrorType PhysLink::GoActive
(
  void
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  // Go Active is allowed in case of monitored network to support backward compatibility
  // Hence, allowing this method to continue here without an error

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call GO ACTIVE IOCTL
  -------------------------------------------------------------------------*/
  result = PhysLinkIoctl (GetHandle(), PHYS_LINK_IOCTL_GO_ACTIVE, NULL);

  mpICritSect->Leave();
  return result;

} /* PhysLink::GoActive() */


ds::ErrorType PhysLink::GoDormant
(
  DormantReasonType  dormantReason
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    dormantReason is defined for the future use
  -------------------------------------------------------------------------*/
  (void) dormantReason;

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call GO DORMANT IOCTL
  -------------------------------------------------------------------------*/
  result = PhysLinkIoctl (GetHandle(), PHYS_LINK_IOCTL_GO_DORMANT, NULL);

  LOG_MSG_FUNCTION_EXIT_3 ("PhysLink::GoActive(): Result 0x%x, obj 0x%p, dormany reason %d",
                           result, this, dormantReason);
  mpICritSect->Leave();
  return result;

} /* PhysLink::GoDormant() */


ds::ErrorType PhysLink::GetState
(
 ds::Net::PhysLinkStateType *  state
)
{
  ds::ErrorType         result = AEE_SUCCESS;
  int32                 physLinkHandle = 0;
  PSPhysLinkStateType   physLinkState = PHYS_LINK_STATE_INVALID;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == state)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    physLinkHandle = GetHandle();
    if (0 == physLinkHandle)
    {
      LOG_MSG_INFO2_1 ("PhysLink::GetState(): NULL handle, obj 0x%p", this);

      *state = PhysLinkState::QDS_DOWN;
      result = AEE_SUCCESS;
      break;
    }

    /*-----------------------------------------------------------------------
      Perform phys link IOCTL to get phys link state.
    -----------------------------------------------------------------------*/
    result = PhysLinkIoctl (physLinkHandle,
                            PHYS_LINK_IOCTL_GET_STATE,
                            &physLinkState);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    *state = PS2DSPhysLinkState (physLinkState);
  } while (0);

  LOG_MSG_FUNCTION_EXIT_3 ("PhysLink::GetState(): Result 0x%x, "
                           "state %d, obj 0x%p", result, *state, this);
  mpICritSect->Leave();

  return result;
} /* PhysLink::GetState() */


ds::ErrorType PhysLink::GetPreviousState
(
  ds::Net::PhysLinkStateType *  state
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Validation
  -------------------------------------------------------------------------*/
  if (NULL == state)
  {
    return QDS_EFAULT;
  }

  /*-------------------------------------------------------------------------
    Fetch the previous state from ds_Net cache.
  -------------------------------------------------------------------------*/
  mpICritSect->Enter();

  *state = mCachedPreviousState;

  mpICritSect->Leave();

  LOG_MSG_INFO1_2  ("PhysLink::GetPreviousState(): "
                    "Obj 0x%x, state %d", this, *state);

  return AEE_SUCCESS;

} /* PhysLink::GetPreviousState() */

/*---------------------------------------------------------------------------
  Event processing.
---------------------------------------------------------------------------*/
boolean PhysLink::Process
(
  void *  pUserData
)
{
  EventInfoType *            pEventInfo;
  ds::ErrorType              result = QDS_ERRORS_BASE;
  int32                      physLinkHandle = 0;
  ps_iface_event_enum_type   expectedPhysLinkEvent = PHYS_LINK_DOWN_EV;
  PSPhysLinkStateType        physLinkState = PHYS_LINK_STATE_INVALID;
  PhysLinkEventInfo          physLinkEventInfo;
  boolean                    usingStale = FALSE;
  bool                       getPhysLinkStateFailed = false;
  PhysLinkRanAsstInfoType    *pRanAsstEventInfo = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    User data should never be NULL for event processing.
  -------------------------------------------------------------------------*/
  if (0 == pUserData)
  {
    return FALSE;
  }

  pEventInfo = static_cast <EventInfoType *> (pUserData);

  if(0 == pEventInfo->handle)
  {
    return FALSE;
  }
  
  switch(pEventInfo->psEventName)
  {
    case PHYS_LINK_RAN_ASST_MEDIA_ADAPT_INFO_EV:
    {
      pRanAsstEventInfo =  static_cast <PhysLinkRanAsstInfoType *> (pEventInfo->psEventInfo);
      
      mRanAsstInfo.handle         = static_cast <int32>(pEventInfo->handle);
      mRanAsstInfo.bitRate        = static_cast <uint8>(pRanAsstEventInfo->bit_rate_info.bit_rate);
      mRanAsstInfo.direction      = static_cast <int32>(pRanAsstEventInfo->bit_rate_info.direction);
      mRanAsstInfo.isRohcEnabled  = static_cast <boolean>(pRanAsstEventInfo->is_rohc_enabled);
      break;
    }
    case PHYS_LINK_PACKET_DISCARD_INFO_EV:
    {
      mPacketDiscardInfo.physLinkHandle = GetHandle();
      break;
    }
    default:
    {
      memscpy((void *)&physLinkEventInfo,
               sizeof(PhysLinkEventInfo),
               pEventInfo->psEventInfo,
               sizeof(PhysLinkEventInfo));
      break;
    }
  }

  mpICritSect->Enter();

  do
  {
    physLinkHandle = GetHandle();
    if (0 == physLinkHandle && 0 != mStaleIfaceHandle)
    {
      physLinkHandle = mStaleIfaceHandle;
      usingStale = TRUE;
    }

    LOG_MSG_INFO1_5 ("PhysLink::Process(): "
                     "Handle 0x%x, event 0x%x, event handle 0x%x, obj 0x%p usingStale %d",
                     physLinkHandle,
                     pEventInfo->eventName,
                     pEventInfo->handle,
                     this, usingStale);

    if((int32) PHYS_LINK_RAN_ASST_MEDIA_ADAPT_INFO_EV == pEventInfo->psEventName ||
       (int32) PHYS_LINK_PACKET_DISCARD_INFO_EV == pEventInfo->psEventName)
    {
      /*---------------------------------------------------------------------
        Event belongs to this handle. Call Notify()
      ---------------------------------------------------------------------*/
      Notify (pEventInfo->eventName);
      result = AEE_SUCCESS;
      break;
    }
    if (physLinkHandle == pEventInfo->handle)
    {
      /*-----------------------------------------------------------------------
        cache previous state info in ds_Net
      -----------------------------------------------------------------------*/      
      mCachedPreviousState = PS2DSPhysLinkState(physLinkEventInfo.state);

      /*---------------------------------------------------------------------
        For PHYS_LINK_GONE_EV event, reset the handle before notifying.

        Since Process() is called in a different task context, physlink's
        state could have chaned by the time, Process() is executed. So, skip
        processing the event so that STATE_CHANGED_EV is not posted multiple
        times. Otherwise, the second time the event is posted, current state
        and previous state point to the same state implying that there is no
        state change.
      ---------------------------------------------------------------------*/
      if ((int32) PHYS_LINK_GONE_EV == pEventInfo->psEventName)
      {
        SetHandle (0);
      }
      else
      {
        if (TRUE == usingStale)
        {
          physLinkState = PHYS_LINK_NULL;
        }
        else
        {
        result = NetPlatform::PhysLinkIoctl (physLinkHandle,
                                             PHYS_LINK_IOCTL_GET_STATE,
                                             &physLinkState);
        if (AEE_SUCCESS != result)
        {
          getPhysLinkStateFailed = true;
          break;
        }
          }

        expectedPhysLinkEvent = PS2PSPhysLinkStateToEvent (physLinkState);

        /*-------------------------------------------------------------------
          Do not post the event if the event is different from the current
          state
        -------------------------------------------------------------------*/
        if (expectedPhysLinkEvent != pEventInfo->psEventName)
        {
          LOG_MSG_INFO1_3 ("PhysLink::Process(): "
                           "Expected event %d, got event %d, obj 0x%p",
                           expectedPhysLinkEvent,
                           pEventInfo->psEventName,
                           this);
          break;
        }
      }

      /*---------------------------------------------------------------------
        Event belongs to this handle. Call Notify()
      ---------------------------------------------------------------------*/
      Notify (pEventInfo->eventName);
    }
  } while (0);

  if(result != AEE_SUCCESS)
  {
    LOG_MSG_INFO3_3 ("PhysLink::Process(): "
                     "Failed to get phys link state, obj 0x%p, using stale %d, "
                     "set phys like state failed %d, ",
                     this, usingStale, getPhysLinkStateFailed);
  }

  mpICritSect->Leave();
  return TRUE;

} /* PhysLink::Process() */

ds::ErrorType PhysLink::OnStateChange
(
  ::ISignal *           signalObj,
  ds::Net::EventType    eventID,
  IQI **                regObj
)
{
  ds::ErrorType  result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      if (PhysLinkEvent::QDS_EV_STATE_CHANGED != eventID &&
          PhysLinkEvent::QDS_EV_RAN_ASST_MEDIA_ADAPT_EV != eventID &&
          PhysLinkEvent::QDS_EV_PACKET_DISCARD_EV != eventID
        )
      {
        LOG_MSG_INVALID_INPUT_3 ("PhysLink::OnStateChange(): "
                                 "event 0x%x not supported in mode %d, obj 0x%p",
                                 eventID, mNetworkMode, this);
        result = AEE_EUNSUPPORTED;
        break;
      }
    }

    /*-----------------------------------------------------------------------
      Post event immediately for STATE_CHANGED events so that application
      can query for the current state
    -----------------------------------------------------------------------*/
    if (PhysLinkEvent::QDS_EV_STATE_CHANGED == eventID)
    {
      (void) signalObj->AddRef();
      (void) signalObj->Set();
      (void) signalObj->Release();
    }

    result = Handle::OnStateChange(signalObj, eventID, regObj);
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("PhysLink::OnStateChange(): "
                           "Result 0x%x, obj 0x%p event 0x%x signal 0x%",
                           result, this , eventID, signalObj);
  mpICritSect->Leave();

  return result;
} /* PhysLink::OnStateChange() */

ds::ErrorType PhysLink::GetSignalBus
(
  ds::Net::EventType  eventID,
  ISignalBus **       ppISigBus
)
{
  ds::ErrorType  result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppISigBus)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (eventID)
  {
    case PhysLinkEvent::QDS_EV_STATE_CHANGED:
    {
      *ppISigBus = mpSigBusStateChange;
      break;
    }
    case PhysLinkEvent::QDS_EV_RAN_ASST_MEDIA_ADAPT_EV:
    {
      *ppISigBus = mpSigBusRanAsstMediaAdapt;
      break;
    }
    case PhysLinkEvent::QDS_EV_PACKET_DISCARD_EV:
    {
      *ppISigBus = mpSigBusPacketDiscard;
      break;
    }

    default:
    {
      result = QDS_EINVAL;
      break;
    }
  }

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

  LOG_MSG_FUNCTION_EXIT_4 ("PhysLink::GetSignalBus(): "
                           "Result 0x%x, signal bus 0x%p, obj 0x%p event 0x%x",
                           result, *ppISigBus, this, eventID);
  mpICritSect->Leave();

  return result;
} /* PhysLink::GetSignalBus() */

ds::ErrorType CDECL PhysLink::GetDormancyInfoCode
(
  DormancyInfoCodeType *  pDormancyInfoCode
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Perform phys link IOCTL to fetch dormancy info-code.
  -------------------------------------------------------------------------*/
  result = PhysLinkIoctl (GetHandle(),
                          PHYS_LINK_IOCTL_GET_DORMANCY_INFO_CODE,
                          (void *) pDormancyInfoCode);

  mpICritSect->Leave();

  return result;
} /* PhysLink::GetDormancyInfoCode() */


ds::ErrorType CDECL PhysLink::GetRAMAEventInfo
(
  RanAsstMediaAdaptEventinfoType* ramaEventInfo
)
{

  /*-------------------------------------------------------------------------
    Get Cached Ran Assist media adapt event info 
  -------------------------------------------------------------------------*/
  memscpy(ramaEventInfo, sizeof(RanAsstMediaAdaptEventinfoType),
    &mRanAsstInfo, sizeof(RanAsstMediaAdaptEventinfoType));

  return AEE_SUCCESS;
}/* GetRAMAEventInfo */

ds::ErrorType CDECL PhysLink::GetPacketDiscardInfo
(
  PacketDiscardInfoType *packetDiscardInfo
)
{
  /*-------------------------------------------------------------------------
    Get Cached Packet Discard info 
  -------------------------------------------------------------------------*/
  memscpy(packetDiscardInfo, sizeof(PacketDiscardInfoType),
    &mPacketDiscardInfo, sizeof(PacketDiscardInfoType));
  
  return AEE_SUCCESS;
}


