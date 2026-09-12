/*===========================================================================
  FILE: QoSManager.cpp

  OVERVIEW: This file provides implementation of the QoSManager class.

  DEPENDENCIES: None

  Copyright (c) 2008-2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

 $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_QoSManager.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2015-06-24 pg  Added to support qos handle when registration to net initiated qos.
  2008-12-22 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_QoSManager.h"
#include "ds_Net_Platform.h"
#include "ds_Net_QoSFlowSpec.h"
#include "ds_Net_IPFilterSpec.h"
#include "ds_Net_QoSSecondary.h"
#include "ds_Net_QoSDefault.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_Conversion.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSignalBus.h"
#include "ds_Net_QoSSecondariesOutput.h"
#include "ds_Net_QoSSecondariesInput.h"
#include "ds_Net_QoSNetInitiatedReq.h"
#include "ds_Net_QoSNetInitiated.h"
#include "ps_system_heap.h"
#include "dss_errors_def.h"

using namespace ds::Error;
using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace NetPlatform;
using namespace dss::Error;

QoSManager::QoSManager
(
  int32            ifaceHandle,
  NetworkModeType  networkMode

)
: Handle (ifaceHandle),
  mNetworkMode(networkMode),
  refCnt (1),
  weakRefCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSManager::QoSManager(): "
                            "Obj 0x%p, iface handle 0x%x, mode %d",
                            this, ifaceHandle, networkMode);

  //TODO Move to init function
  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,
                                 (void **) &mpSigBusProfilesChanged);

  mpQoSDefault = NULL;

  if (AEE_SUCCESS != Handle::Init(EventManager::qosObjList))
  {
    LOG_MSG_ERROR_1 ("QoSManager::QoSManager(): "
                     "Handle::Init failed, Obj 0x%p", this);
    return;
  }

} /* QoSManager::QoSManager() */

void QoSManager::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  DS_UTILS_RELEASEIF (mpQoSDefault);
  DS_UTILS_RELEASEIF (mpSigBusProfilesChanged);

  /*-------------------------------------------------------------------------
    Should be the last statement. Call destructor of the base class.
  -------------------------------------------------------------------------*/
  Handle::Destructor();

  mpICritSect->Leave();

} /* QoSManager::Destructor() */

QoSManager::~QoSManager
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

} /* QoSManager::~QoSManager() */


/*---------------------------------------------------------------------------
  Inherited functions from IQoSManager.
---------------------------------------------------------------------------*/
ds::ErrorType QoSManager::GetQosDefault
(
  ::IQoS **  ppIDSNetQoSDefault
)
{
  ds::ErrorType  result;
  int32          ifaceHandle;
  int32          flowHandle;
  bool           defaultFlow = true;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppIDSNetQoSDefault)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    if (0 == mpQoSDefault)
    {
      /*---------------------------------------------------------------------
        Get the default flow handle for the iface
      ---------------------------------------------------------------------*/
      ifaceHandle = GetHandle();
      result = NetPlatform::PSGetDefaultFlow(ifaceHandle, &flowHandle);
      if (AEE_SUCCESS != result)
      {
        defaultFlow = false;
        break;
      }

      mpQoSDefault =
        new QoSDefault(ifaceHandle, flowHandle, mNetworkMode);
      if (0 == mpQoSDefault)
      {
        result = AEE_ENOMEMORY;
        break;
      }
    }

    (void) mpQoSDefault->AddRef();

    *ppIDSNetQoSDefault = static_cast <IQoS *> (mpQoSDefault);
    result              = AEE_SUCCESS;
  } while (0);

  if (result != AEE_SUCCESS)
  {
    LOG_MSG_FUNCTION_EXIT_5 ("QoSManager::GetQosDefault(): "
                             "Result 0x%x,default flow on handle failed %d, "
                             "iface handle %d, qos default 0x%p, obj 0x%p",
                             result, defaultFlow, ifaceHandle,
                             mpQoSDefault, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::GetQosDefault() */


ds::ErrorType QoSManager::RequestSecondary
(
  const QoSSpecType *  pQoSSpec,
  ::IQoSSecondary **   ppIDSNetQoSSecondary
)
{
  QoSSecondary *     pSecQoS = NULL;
  QoSRequestType     qosRequestInfo;
  PSQoSSpecType      localQoSSpec;
  ds::ErrorType      result;
  int32              ifaceHandle;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSManager::RequestSecondary(): "
                            "Obj 0x%p, net QoS secondary 0x%p, Qos spec 0x%p",
                            this, ppIDSNetQoSSecondary, pQoSSpec);

  if (0 == ppIDSNetQoSSecondary || 0 == pQoSSpec)
  {
    return QDS_EFAULT;
  }

  memset (&qosRequestInfo, 0, sizeof(qosRequestInfo));

  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      ds_net_error_code = DSS_ERROR_NETWORK_MODE_NOT_SUPPORTED;
      result = AEE_EUNSUPPORTED;
      break;
    }

    /*-----------------------------------------------------------------------
      Validate QoS spec and construct QOS_REQUEST IOCTL arguments.
    -----------------------------------------------------------------------*/

    qosRequestInfo.flow_ptr  = NULL;
    qosRequestInfo.qos_ptr   = &localQoSSpec;

    result = DS2PSQoSRequestSpec (pQoSSpec, qosRequestInfo.qos_ptr);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_COVERT_FROM_DS_TO_PS_FAILED;
      break;
    }

    /*-----------------------------------------------------------------------
      Perform QoS Request IOCTL on the iface handle to get the secondary flow
    -----------------------------------------------------------------------*/
    ifaceHandle = GetHandle();
    result = IfaceIoctl (ifaceHandle,
                         IFACE_IOCTL_QOS_REQUEST,
                         static_cast <void *> (&qosRequestInfo));

    if (AEE_SUCCESS != result && AEE_EWOULDBLOCK != result)
    {
      /*---------------------------------------------------------------------
        Get the err mask set in the QoS Request info argument
      ---------------------------------------------------------------------*/
      PS2DSQoSSpec (qosRequestInfo.qos_ptr,
                    const_cast <QoSSpecType *> (pQoSSpec));

      ds_net_error_code = DSS_ERROR_REQUEST_QOS_FAILED;
      break;
    }

    /*-----------------------------------------------------------------------
      Create a SecondaryQoS object using the this flow handle
    -----------------------------------------------------------------------*/
    pSecQoS = new QoSSecondary (ifaceHandle,
                                (int32) qosRequestInfo.flow_ptr,
                                mNetworkMode);
    if (0 == pSecQoS)
    {
      ds_net_error_code = DSS_ERROR_QOS_ALLOC_FAILED;
      result = AEE_ENOMEMORY;
      break;
    }

    *ppIDSNetQoSSecondary = static_cast <IQoSSecondary *> (pSecQoS);
     result               = AEE_SUCCESS;
  } while (0);

  /*-------------------------------------------------------------------------
    Free the memory allocated in DS2PSQoSRequestSpec and for qos_ptr.
  -------------------------------------------------------------------------*/
  (void) CleanupPSQoSRequestSpec (qosRequestInfo.qos_ptr);

  /*-------------------------------------------------------------------------
    Free memory in case of failure
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_ERROR_3 ("QoSManager::RequestSecondary(): "
                     "Result 0x%x, error code %d, obj 0x%p",
                     result, ds_net_error_code, this);
    DS_UTILS_RELEASEIF (pSecQoS);
  }

  mpICritSect->Leave();

  return result;
} /* QoSManager::RequestSecondary() */

ds::ErrorType QoSManager::RequestBundle
(
  const QoSSpecType *       specs,
  int                       specsLen,
  QoSRequestOpCodeType      opCode,
  IQoSSecondariesOutput **  sessions
)
{
  QoSSecondary **         ppQoSSecondary = 0;
  QoSSecondariesOutput *  pQoSOutput = 0;
  QoSRequestExType        qosRequestExInfo;
  ds::ErrorType           result;
  int32                   index;
  int32                   ifaceHandle;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_5 ("QoSManager::RequestBundle(): "
                            "Obj 0x%p, opcode %d, specs 0x%p, sessions 0x%p"
                            "specsLen %d",
                            this, opCode, specs, sessions, specsLen);

  if (0 == specs || 0 == sessions)
  {
    return QDS_EFAULT;
  }

  if (0 >= specsLen)
  {
    return QDS_EINVAL;
  }

  memset (&qosRequestExInfo, 0, sizeof (QoSRequestExType));

  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      ds_net_error_code = DSS_ERROR_NETWORK_MODE_NOT_SUPPORTED;
      result = AEE_EUNSUPPORTED;
      break;
    }

    /*-----------------------------------------------------------------------
      Construct QOS_REQUEST_EX_IOCTL arguments.
    -----------------------------------------------------------------------*/
    qosRequestExInfo.num_qos_specs     = (uint8) specsLen;
    qosRequestExInfo.qos_control_flags = 0;

    if (opCode == QoSRequestOpCode::QDS_REQUEST)
    {
      qosRequestExInfo.opcode = PS_IFACE_IOCTL_QOS_REQUEST_OP;
    }
    else
    {
      qosRequestExInfo.opcode = PS_IFACE_IOCTL_QOS_CONFIGURE_OP;
    }

    /*-----------------------------------------------------------------------
      Allocate memory for flow_ptrs array. Here we are allocating memory
      for holding only the pointers to ps flows and not the entire
      flow structures.
    -----------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(qosRequestExInfo.flows_ptr,
                              sizeof (ps_flow_type *) * specsLen,
                              ps_flow_type**);
    if (0 == qosRequestExInfo.flows_ptr)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    /*-----------------------------------------------------------------------
      Allocate memory for specs_ptr array.
    -----------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(qosRequestExInfo.qos_specs_ptr,
                             sizeof (qos_spec_type) * specsLen,
                             qos_spec_type*);
    if (0 == qosRequestExInfo.qos_specs_ptr)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    /*-----------------------------------------------------------------------
      Fill the filter and flow specs into the bundled IOCTL request arg.
    -----------------------------------------------------------------------*/
    for (index = 0; index < specsLen; index++)
    {
      qosRequestExInfo.flows_ptr[index] = NULL;
      (void) DS2PSQoSRequestSpec (&specs[index],
                                  &qosRequestExInfo.qos_specs_ptr[index]);
    }

    /*-----------------------------------------------------------------------
      Allocate memory for secondary QoS objects array. Here we are allocating
      memory for only the array of pointers and not the QoSSecondary objects
      themselves.
    -----------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC( ppQoSSecondary,
                              sizeof (QoSSecondary *) * specsLen,
                              QoSSecondary **);
    if (0 == ppQoSSecondary)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    memset (ppQoSSecondary, 0, specsLen * sizeof(QoSSecondary *));

    /*-----------------------------------------------------------------------
      Issue a QOS_REQUEST_EX_IOCTL to request for a bundle of flows.
    -----------------------------------------------------------------------*/
    ifaceHandle = GetHandle();
    result = IfaceIoctl (ifaceHandle,
                         IFACE_IOCTL_QOS_REQUEST_EX,
                         static_cast <void *> (&qosRequestExInfo));

    if (AEE_SUCCESS != result && AEE_EWOULDBLOCK != result)
    {
      for (index = 0; index < specsLen; index++)
      {
        /*-------------------------------------------------------------------
          Get the err mask set in the QoS Request info argument.
        -------------------------------------------------------------------*/
        PS2DSQoSSpec (&qosRequestExInfo.qos_specs_ptr[index],
                      const_cast <QoSSpecType*> (&specs[index]));
      }

      break;
    }

    /*-----------------------------------------------------------------------
      Create corresponding secondary flow objects (QoSSecondary objects)
      associated with these flows.
    -----------------------------------------------------------------------*/
    pQoSOutput = new QoSSecondariesOutput();
    if (0 == pQoSOutput)
    {
      ds_net_error_code = DSS_ERROR_QOS_ALLOC_OUTPUT_FAILED;
      result = AEE_ENOMEMORY;
      break;
    }

    for (index = 0;
         index < qosRequestExInfo.num_qos_specs && index < specsLen;
         index++)
    {
      ppQoSSecondary[index] =
        new QoSSecondary (ifaceHandle,
                          (int32) qosRequestExInfo.flows_ptr[index],
                          mNetworkMode);
      if (0 == ppQoSSecondary[index])
      {
        ds_net_error_code = DSS_ERROR_QOS_ALLOC_FAILED;
        result = AEE_ENOMEMORY;
        break;
      }

      pQoSOutput->AddQoSSecondary(ppQoSSecondary[index]);
    }

    /*-----------------------------------------------------------------------
      Populate the out parameter with the IQoSSecondary interface.
    -----------------------------------------------------------------------*/
    *sessions = static_cast<IQoSSecondariesOutput *> (pQoSOutput);
    result    = AEE_SUCCESS;
  } while (0);

  /*-------------------------------------------------------------------------
    Clean up array of QoSSecondary objects in case of failure
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS != result)
  {
    if (0 != ppQoSSecondary)
    {
      for (index = 0;
           index < qosRequestExInfo.num_qos_specs && index < specsLen;
           index++)
      {
        DS_UTILS_RELEASEIF (ppQoSSecondary[index]);
      }
    }
  }

  /*-------------------------------------------------------------------------
    Do the common clean up
  -------------------------------------------------------------------------*/
  for (index = 0;
       index < qosRequestExInfo.num_qos_specs && index < specsLen;
       index++)
  {
    (void) CleanupPSQoSRequestSpec (&qosRequestExInfo.qos_specs_ptr[index]);
  }

  PS_SYSTEM_HEAP_MEM_FREE (qosRequestExInfo.flows_ptr);
  PS_SYSTEM_HEAP_MEM_FREE (qosRequestExInfo.qos_specs_ptr);
  PS_SYSTEM_HEAP_MEM_FREE (ppQoSSecondary);

  if (AEE_SUCCESS != result)
  {
    LOG_MSG_ERROR_3 ("QoSManager::RequestBundle(): "
                     "Result 0x%x, error code %d, obj 0x%p",
                     result, ds_net_error_code, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::RequestBundle() */

ds::ErrorType QoSManager::RequestNetInitiatedReq
(
  const QoSSpecNetInitiatedType* pQoSNetInitiatedSpec,
  ::IQoSNetInitiatedReq** ppIDSNetQoSInitiatedReq,
  QoSFltrMatchDirType direction
)
{
  int32               result;
  QoSNetInitiatedReq* pQoSNetInitiatedReq = NULL;

  /* Specification for PS IOCTL */
  QoSNetInitiatedRequestType  qosNetInitiatedRequestInfo;

  /* PS Network Initiated QoS specification */
  int32                       ifaceHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_4  ("QoSManager::RequestNetInitiatedReq(): "
                             "Obj 0x%p, network mode %d, initiated req 0x%p, "
                             "initiated spec 0x%p", 
                             this, mNetworkMode, ppIDSNetQoSInitiatedReq,
                             pQoSNetInitiatedSpec);

  if (NetworkMode::QDS_MONITORED == mNetworkMode)
  {
    return AEE_EUNSUPPORTED;
  }

  /*-------------------------------------------------------------------------
    Validate arguments.
  -------------------------------------------------------------------------*/
  if (NULL == ppIDSNetQoSInitiatedReq || NULL == pQoSNetInitiatedSpec)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Validate QoS spec and construct QOS_REQUEST IOCTL arguments.
  -------------------------------------------------------------------------*/
  memset (&qosNetInitiatedRequestInfo, 0, sizeof(qosNetInitiatedRequestInfo));

  result = DS2PSQoSNetInitiatedRequestSpec(pQoSNetInitiatedSpec,
           (PSQoSNetInitiatedSpecType*) &qosNetInitiatedRequestInfo.net_initiated_qos_spec);
  if (AEE_SUCCESS != result)
  {
    mpICritSect->Leave();
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Perform Network Initiated monitoring QoS Request IOCTL on the iface handle.
  -------------------------------------------------------------------------*/
  ifaceHandle = GetHandle();

  qosNetInitiatedRequestInfo.fltr_match_dir = (ps_iface_qos_fltr_match_dir_enum_type) direction;

  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_QOS_NET_INITIATED_REQUEST,
                       static_cast <void *> (&qosNetInitiatedRequestInfo));

  /*-------------------------------------------------------------------------
    Get the err mask set in the QoS Request info argument.
  -------------------------------------------------------------------------*/
  PS2DSQoSNetInitiatedSpec (
    (PSQoSNetInitiatedSpecType*) &qosNetInitiatedRequestInfo.net_initiated_qos_spec,
    const_cast <QoSSpecNetInitiatedType*> (pQoSNetInitiatedSpec)
  );

  if (AEE_SUCCESS != result && AEE_EWOULDBLOCK != result)
  {
    mpICritSect->Leave();
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Create a Net Initiated QoS object using the this flow handle.
  -------------------------------------------------------------------------*/
  pQoSNetInitiatedReq = new QoSNetInitiatedReq (
     ifaceHandle,
     (int32)qosNetInitiatedRequestInfo.handle /* Shall be used as handle to
                                                 identify events */
     );
  if (NULL == pQoSNetInitiatedReq)
  {
    result = AEE_ENOMEMORY;
    mpICritSect->Leave();
    goto bail;
  }

  result = pQoSNetInitiatedReq->RealTimeCheck(pQoSNetInitiatedSpec, direction);
  if ((AEE_SUCCESS != result) && (AEE_EWOULDBLOCK != result))
  {
    mpICritSect->Leave();
    goto bail;
  }

  *ppIDSNetQoSInitiatedReq =
    static_cast <IQoSNetInitiatedReq *> (pQoSNetInitiatedReq);

  /*-------------------------------------------------------------------------
    Free the memory allocated in DS2PSQoSRequestSpec and for qos_ptr.
  -------------------------------------------------------------------------*/
  (void) CleanupPSQoSNetInitiatedSpec ((PSQoSNetInitiatedSpecType*) &qosNetInitiatedRequestInfo.net_initiated_qos_spec);
  mpICritSect->Leave();

  return result;

bail:
  LOG_MSG_ERROR_1 ("QoSManager::RequestNetInitiatedReq(): "
                   "Err %d", result);
  (void) CleanupPSQoSNetInitiatedSpec ((PSQoSNetInitiatedSpecType*) &qosNetInitiatedRequestInfo.net_initiated_qos_spec);
  DS_UTILS_RELEASEIF (pQoSNetInitiatedReq);
  *ppIDSNetQoSInitiatedReq = NULL;
  return result;

} /* RequestNetInitiatedReq() */

ds::ErrorType QoSManager::CreateQoSNetInitiated
(
  ::IQoSNetInitiatedReq* pQoSNetInitiatedReq,
  ::IQoSNetInitiated** ppiQoSNetInitiated,
  QoSFltrMatchDirType* pFltrMatchDir
)
{
  int32            result;

  /* ps flow handle */
  int32            flowHandle;

  QoSNetInitiated* pQoSNetInitiated = NULL;
  bool             lastMatchingFlowHandleIsNotAvailable = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSManager::CreateQoSNetInitiated(): "
                            "Obj 0x%p, initiated req 0x%p, net initiated 0x%p",
                            this, pQoSNetInitiatedReq, ppiQoSNetInitiated);

  /*-------------------------------------------------------------------------
    Validate arguments.
  -------------------------------------------------------------------------*/
  if (NULL == pQoSNetInitiatedReq || NULL == ppiQoSNetInitiated)
  {
    return QDS_EFAULT;
  }

  /*-------------------------------------------------------------------------
    Fetch the PS flow handle corresponding to the new Net Initiated QoS object.
  -------------------------------------------------------------------------*/
  mpICritSect->Enter();

  result = pQoSNetInitiatedReq->GetLastMatchingFlowHandle(&flowHandle, pFltrMatchDir);
  if (AEE_SUCCESS != result)
  {
    mpICritSect->Leave();
    lastMatchingFlowHandleIsNotAvailable = true;
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Create a Net Initiated QoS object using the this flow handle.
  -------------------------------------------------------------------------*/
  pQoSNetInitiated = new QoSNetInitiated (
     GetHandle(),
     flowHandle,
     mNetworkMode);

  if (NULL == pQoSNetInitiated)
  {
    result = AEE_ENOMEMORY;
    mpICritSect->Leave();
    goto bail;
  }

  *ppiQoSNetInitiated = static_cast <IQoSNetInitiated *> (pQoSNetInitiated);
  mpICritSect->Leave();

  return AEE_SUCCESS;

bail:
  LOG_MSG_ERROR_3 ("QoSManager::CreateQoSNetInitiated(): "
                   "obj 0x%p, last matching flow handle is available,"
                   "QoS net initiated 0x %p",
                   this, lastMatchingFlowHandleIsNotAvailable,
                   pQoSNetInitiated);

  DS_UTILS_RELEASEIF (pQoSNetInitiated);
  *ppiQoSNetInitiated = NULL;
  return result;
} /* CreateQoSNetInitiated() */


ds::ErrorType QoSManager::Close
(
  IQoSSecondariesInput *  qosSessions
)
{
  IQoSSecondary *        pIQoSSecondary = 0;
  QoSSecondary *         pQoSSecondary = 0;
  QoSSecondariesInput *  qosSecondariesInput;
  QoSReleaseExType       releaseExInfo;
  ds::ErrorType          result;
  int                    index;
  int32                  flowHandle;
  int                    sessionsLen;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == qosSessions)
  {
    return QDS_EFAULT;
  }

  memset (&releaseExInfo, 0, sizeof(releaseExInfo));

  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      ds_net_error_code = DSS_ERROR_NETWORK_MODE_NOT_SUPPORTED;
      result = AEE_EUNSUPPORTED;
      break;
    }

    /*-----------------------------------------------------------------------
      Construct QOS_RELEASE_EX_IOCTL arguments.
    -----------------------------------------------------------------------*/
    qosSecondariesInput = static_cast <QoSSecondariesInput *> (qosSessions);

    result = qosSecondariesInput->GetNumElements(&sessionsLen);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_GET_NUM_ELEMENTS_FAILED;
      break;
    }


    releaseExInfo.num_flows = (uint8) sessionsLen;
    PS_SYSTEM_HEAP_MEM_ALLOC( releaseExInfo.flows_ptr,
                              sizeof (ps_flow_type*) * sessionsLen,
                              ps_flow_type**);
    if (0 == releaseExInfo.flows_ptr)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    for (index = 0; index < sessionsLen; index++)
    {
      result = qosSecondariesInput->GetNth(index, &pIQoSSecondary);
      if (AEE_SUCCESS != result)
      {
        ds_net_error_code = DSS_ERROR_GET_NTH_FAILED;
        result = AEE_ENOMEMORY;
        break;
      }

      pQoSSecondary = static_cast <QoSSecondary *> (pIQoSSecondary);
      releaseExInfo.flows_ptr[index] =
        (ps_flow_type *)(pQoSSecondary->GetHandle());
      DS_UTILS_RELEASEIF(pIQoSSecondary);
    }

    /*-----------------------------------------------------------------------
      This is a bundled IOCTL operation to be performed on a set of flows.
      However, since this is a flow IOCTL, we need to specify a single flow
      handle to drive the FlowIoctl function. The flow handle in this case
      would be the first flow the bundle.
    -----------------------------------------------------------------------*/
    flowHandle = (int32) releaseExInfo.flows_ptr[0];

    /*-----------------------------------------------------------------------
      Call bundled close for the secondary QoS flows.
    -----------------------------------------------------------------------*/
    result = FlowIoctl (flowHandle,
                        FLOW_IOCTL_QOS_RELEASE_EX,
                        static_cast <void *> (&releaseExInfo));
    if (AEE_SUCCESS != result && AEE_EWOULDBLOCK != result)
    {
      break;
    }
  } while (0);

  PS_SYSTEM_HEAP_MEM_FREE (releaseExInfo.flows_ptr);

  if (result != AEE_SUCCESS)
  {
    LOG_MSG_FUNCTION_EXIT_3 ("QoSManager::Close(): "
                             "Result 0x%x, error code %d, obj 0x%p",
                             result, ds_net_error_code, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::Close() */

ds::ErrorType QoSManager::Resume
(
  IQoSSecondariesInput *  qosSessions
)
{
  IQoSSecondary *        pIQoSSecondary = 0;
  QoSSecondary *         pQoSSecondary = 0;
  QoSSecondariesInput *  qosSecondariesInput;
  QoSResumeExType        resumeExInfo;
  ds::ErrorType          result;
  int32                  flowHandle;
  int                    index;
  int                    sessionsLen;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == qosSessions)
  {
    return QDS_EFAULT;
  }

  memset (&resumeExInfo, 0, sizeof(resumeExInfo));

  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      ds_net_error_code = DSS_ERROR_NETWORK_MODE_NOT_SUPPORTED;
      result = AEE_EUNSUPPORTED;
      break;
    }

    /*-----------------------------------------------------------------------
      Construct QOS_RESUME_EX_IOCTL arguments.
    -----------------------------------------------------------------------*/
    qosSecondariesInput = static_cast <QoSSecondariesInput *> (qosSessions);

    result = qosSecondariesInput->GetNumElements(&sessionsLen);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_GET_NUM_ELEMENTS_FAILED;
      break;
    }

    resumeExInfo.num_flows = (uint8) sessionsLen;
    PS_SYSTEM_HEAP_MEM_ALLOC( resumeExInfo.flows_ptr,
                              sizeof (ps_flow_type*) * sessionsLen, 
                              ps_flow_type**);
    if (0 == resumeExInfo.flows_ptr)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    for (index = 0; index < sessionsLen; index++)
    {
      result = qosSecondariesInput->GetNth(index, &pIQoSSecondary);
      if (AEE_SUCCESS != result)
      {
        ds_net_error_code = DSS_ERROR_GET_NTH_FAILED;
        result = AEE_ENOMEMORY;
        break;
      }

      pQoSSecondary = static_cast <QoSSecondary *> (pIQoSSecondary);
      resumeExInfo.flows_ptr[index] =
        (ps_flow_type *) (pQoSSecondary->GetHandle());
      DS_UTILS_RELEASEIF(pIQoSSecondary);
    }

    /*-----------------------------------------------------------------------
      This is a bundled IOCTL operation to be performed on a set of flows.
      However, since this is a flow IOCTL, we need to specify a single flow
      handle to drive the FlowIoctl function. The flow handle in this case
      would be the first flow the bundle.
    -----------------------------------------------------------------------*/
    flowHandle = (int32) resumeExInfo.flows_ptr[0];

    /*-----------------------------------------------------------------------
      Call bundled resume for the secondary QoS flows.
    -----------------------------------------------------------------------*/
    result = FlowIoctl (flowHandle,
                        FLOW_IOCTL_QOS_RESUME_EX,
                        static_cast <void *> (&resumeExInfo));
    if (AEE_SUCCESS != result && AEE_EWOULDBLOCK != result)
    {
      break;
    }
  } while (0);

  PS_SYSTEM_HEAP_MEM_FREE (resumeExInfo.flows_ptr);

  if (result != AEE_SUCCESS)
  {
    LOG_MSG_ERROR_3 ("QoSManager::Resume(): "
                     "Result 0x%x, error code %d, obj 0x%p",
                     result, ds_net_error_code, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::Resume() */


ds::ErrorType QoSManager::Suspend
(
  IQoSSecondariesInput *  qosSessions
)
{
  IQoSSecondary *        pIQoSSecondary = 0;
  QoSSecondary *         pQoSSecondary = 0;
  QoSSecondariesInput *  qosSecondariesInput;
  QoSSuspendExType       suspendExInfo;
  ds::ErrorType          result;
  int32                  flowHandle;
  int                    index;
  int                    sessionsLen;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == qosSessions)
  {
    return QDS_EFAULT;
  }

  memset (&suspendExInfo, 0, sizeof(suspendExInfo));

  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      ds_net_error_code = DSS_ERROR_NETWORK_MODE_NOT_SUPPORTED;
      result = AEE_EUNSUPPORTED;
      break;
    }

    /*-----------------------------------------------------------------------
      Construct QOS_SUSPEND_EX_IOCTL arguments.
    -----------------------------------------------------------------------*/
    qosSecondariesInput = static_cast<QoSSecondariesInput *> (qosSessions);

    result = qosSecondariesInput->GetNumElements(&sessionsLen);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_GET_NUM_ELEMENTS_FAILED;
      break;
    }

    suspendExInfo.num_flows = (uint8) sessionsLen;
    PS_SYSTEM_HEAP_MEM_ALLOC(suspendExInfo.flows_ptr,
                             sizeof (ps_flow_type*) * sessionsLen,
                             ps_flow_type** );
    if (0 == suspendExInfo.flows_ptr)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    for (index = 0; index < sessionsLen; index++)
    {
      result = qosSecondariesInput->GetNth(index, &pIQoSSecondary);
      if (AEE_SUCCESS != result)
      {
        ds_net_error_code = DSS_ERROR_GET_NTH_FAILED;
        result = AEE_ENOMEMORY;
        break;
      }

      pQoSSecondary = static_cast <QoSSecondary *> (pIQoSSecondary);
      suspendExInfo.flows_ptr[index] =
        (ps_flow_type *) (pQoSSecondary->GetHandle());
      if (0 == suspendExInfo.flows_ptr[index])
      {
        ds_net_error_code = DSS_ERROR_FLOW_HANDLE_IS_INVALID;
        result = AEE_EBADHANDLE;
        DS_UTILS_RELEASEIF(pIQoSSecondary); // GetNth used addref
        break;
      }
      DS_UTILS_RELEASEIF(pIQoSSecondary); // GetNth used addref
    }

    /*-----------------------------------------------------------------------
      This is a bundled IOCTL operation to be performed on a set of flows.
      However, since this is a flow IOCTL, we need to specify a single flow
      handle to drive the FlowIoctl function. The flow handle in this case
      would be the first flow the bundle.
    -----------------------------------------------------------------------*/
    flowHandle = (int32) suspendExInfo.flows_ptr[0];

    /*-----------------------------------------------------------------------
      Call bundled suspend for the secondary QoS flows.
    -----------------------------------------------------------------------*/
    result = FlowIoctl (flowHandle,
                        FLOW_IOCTL_QOS_SUSPEND_EX,
                        static_cast <void *> (&suspendExInfo));
    if (AEE_SUCCESS != result && AEE_EWOULDBLOCK != result)
    {
      break;
    }
  } while (0);

  PS_SYSTEM_HEAP_MEM_FREE (suspendExInfo.flows_ptr);

  if (result != AEE_SUCCESS)
  {
    LOG_MSG_ERROR_3 ("QoSManager::Suspend(): "
                     "Result 0x%x, error code %d, obj 0x%p",
                     result, ds_net_error_code, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::Suspend() */


ds::ErrorType QoSManager::GetSupportedProfiles
(
  QoSProfileIdType *  profiles,
  int                 profilesLen,
  int *               profilesLenReq
)
{
  ds::ErrorType      result;
  int32              nNumOfItemsToCopy;

  ps_iface_ioctl_get_network_supported_qos_profiles_type  stTempVal;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSManager::GetSupportedProfiles(): "
                            "Obj 0x%p,profile 0x%p, profile len %d",
                            this, profiles, profilesLen);

  if (0 == profiles && 0 != profilesLen)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Perform iface IOCTL for GET_NETWORK_SUPPORTED_QOS_PROFILES on the iface
      handle associated with this QoS Manager object.
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_NETWORK_SUPPORTED_QOS_PROFILES,
                         static_cast <void *> (&stTempVal));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    if (0 != profilesLenReq)
    {
      *profilesLenReq = stTempVal.profile_count;
    }

    /*-----------------------------------------------------------------------
      Copy profiles in to OUT param
    -----------------------------------------------------------------------*/
    nNumOfItemsToCopy = MIN(stTempVal.profile_count, profilesLen);
    for (int i = 0; i < nNumOfItemsToCopy; i++)
    {
      profiles[i] = (QoSProfileIdType) stTempVal.profile_value[i];
    }
  } while (0);

  mpICritSect->Leave();

  return result;
} /* QoSManager::GetSupportedProfiles() */

ds::ErrorType QoSManager::CreateQoSSecondariesInput
(
  IQoSSecondariesInput **  newQoSSecondariesInput
)
{
  QoSSecondariesInput *  qosSecondariesInput;
  ds::ErrorType          result;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == newQoSSecondariesInput)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      ds_net_error_code = DSS_ERROR_NETWORK_MODE_NOT_SUPPORTED;
      result = AEE_EUNSUPPORTED;
      break;
    }

    /*-----------------------------------------------------------------------
      Construct QoSSecondariesInput object.
    -----------------------------------------------------------------------*/
    qosSecondariesInput = new QoSSecondariesInput();
    if (0 == qosSecondariesInput)
    {
      ds_net_error_code = DSS_ERROR_QOS_ALLOC_FAILED;
      result = AEE_ENOMEMORY;
      break;
    }

    *newQoSSecondariesInput =
      static_cast <IQoSSecondariesInput *> (qosSecondariesInput);

    result = AEE_SUCCESS;
  } while (0);

  if (result != AEE_SUCCESS)
  {
    LOG_MSG_ERROR_3 ("QoSManager::CreateQoSSecondariesInput(): "
                     "Result 0x%x, error code %d, obj 0x%p",
                      result, ds_net_error_code, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::CreateQoSSecondariesInput() */


boolean QoSManager::Process
(
  void *  userDataPtr
)
{
  EventInfoType *  eventInfoPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == userDataPtr)
  {
    return FALSE;
  }

  eventInfoPtr = static_cast <EventInfoType *> (userDataPtr);

  mpICritSect->Enter();

  do
  {
    LOG_MSG_INFO1_4 ("Network::Process(): "
                     "Handle 0x%x, event 0x%x, event handle 0x%x, obj 0x%p",
                     GetHandle(),
                     eventInfoPtr->eventName,
                     eventInfoPtr->handle,
                     this);

    /*-----------------------------------------------------------------------
      Ignore QoSNetInitiatedReqEvent::QDS_EV_STATE_CHANGED as it is related to
      Network Initiated QoS
    -----------------------------------------------------------------------*/
    if (QoSNetInitiatedReqEvent::QDS_EV_STATE_CHANGED ==
          eventInfoPtr->eventName)
    {
      break;
    }

    if (EVENT_GROUP_QOS == eventInfoPtr->eventGroup)
    {
      if (GetHandle() == eventInfoPtr->handle)
      {
        Notify(eventInfoPtr->eventName);
      }
    }
  } while (0);

  mpICritSect->Leave();
  return TRUE;

} /* QoSManager::Process() */


ds::ErrorType QoSManager::OnStateChange
(
  ISignal *           signalObj,
  ds::Net::EventType  eventName,
  IQI **              regObj
)
{
  ds::ErrorType  result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSManager::OnStateChange(): "
                            "Obj 0x%p, event 0x%x, signal 0x%p",
                            this, eventName, signalObj);

  mpICritSect->Enter();

  do
  {
    if (NetworkMode::QDS_MONITORED == mNetworkMode)
    {
      if (QoSMgrEvent::QDS_EV_PROFILES_CHANGED != eventName)
      {
        result = AEE_EUNSUPPORTED;
        break;
      }
    }

    result = Handle::OnStateChange(signalObj, eventName, regObj);
  } while (0);

  mpICritSect->Leave();

  return result;
} /* QoSManager::OnStateChange() */


ds::ErrorType QoSManager::GetSignalBus
(
  ds::Net::EventType  eventID,
  ISignalBus **       ppISigBus
)
{
  ds::ErrorType  result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSManager::GetSignalBus(): "
                            "Obj 0x%p, event 0x%x, signal bus 0x%p",
                            this, eventID, ppISigBus);

  if (0 == ppISigBus)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (eventID)
  {
    case QoSMgrEvent::QDS_EV_PROFILES_CHANGED:
    {
      *ppISigBus = mpSigBusProfilesChanged;
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

  if (result != AEE_SUCCESS)
  {
    LOG_MSG_ERROR_4 ("QoSManager::GetSignalBus(): "
                     "Result 0x%x, signal bus 0x%p, event 0x%x, obj 0x%p",
                     result, *ppISigBus, eventID, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::GetSignalBus() */


ds::ErrorType QoSManager::QueryInterface
(
  AEEIID   iid,
  void **  ppo
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSManager::QueryInterface(): "
                            "Obj 0x%p, iid 0x%x, ppo 0x%p",
                            this, iid, ppo);

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (iid)
  {
    case AEEIID_IQoSManager:
    case AEEIID_IQI:
    {
      *ppo   = static_cast <IQoSManager *> (this);
      result = AEE_SUCCESS;

      (void) AddRef();
      break;
    }

    case AEEIID_IQoSManagerPriv:
    {
      *ppo   = static_cast <IQoSManagerPriv *> (this);
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

  if(result != AEE_SUCCESS)
  {
    LOG_MSG_ERROR_4 ("QoSManager::QueryInterface(): "
                     "Result 0x%x, ppo 0x%p, iid 0x%x, obj 0x%p",
                     result, *ppo, iid, this);
  }
  mpICritSect->Leave();

  return result;
} /* QoSManager::QueryInterface() */

ds::ErrorType QoSManager::RequestFiltrMatch
(
  void * fltr_info_ptr
)
{
   ds::ErrorType  result = AEE_SUCCESS;
   int32   ifaceHandle;
   
   /*-------------------------------------------------------------------------
    Perform Network Initiated monitoring QoS Request IOCTL on the iface handle.
  -------------------------------------------------------------------------*/
  ifaceHandle = GetHandle();

  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_FILTER_MATCH_REQUEST,
                       static_cast <void *> (fltr_info_ptr));
  return result;
} /* QoSManager::RequestFiltrMatch() */

