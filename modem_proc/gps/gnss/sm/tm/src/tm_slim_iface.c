/*------------------------------------------------------------------------------
  Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2017 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved                   .
  Copyright (c) 2017 - 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
                                                                               .
      Export of this technology or software is regulated by the U.S. Government.
      Diversion contrary to U.S. law prohibited.
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
File comment
------------------------------------------------------------------------------*/


/*=============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_slim_iface.c#1 $ 

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/17/17   skm     BLE and Barometer support for LPPE Phase 2 
01/15/17   skm     Initial Version  

=============================================================================*/
#include "customer.h"   /* Customer configuration file */
#include "comdef.h"
#include "msg.h"
#include "aries_os_api.h"
#include "tm_api.h"
#include "tech_sel_api.h"
#include "tm_slim_iface.h"



/*===========================================================================
FUNCTION  tm_slimNotifyDataCallback

DESCRIPTION
  SLIM callback function provided with slim_open.to receive notification from
  SLIM module. TM_AUXTECH should copy over the data and send an IPC to TM thread
  since Sensor data processing should happen in client context.

DEPENDENCIES
  tm_SlimInit () function should be called before calling slim_open()
 
PARAMETERS 
  t_CallbackData:    client provided data.
  pz_MessageHeader:  pointer to message header
  p_Message       :  pointer to data
 
RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void tm_slimNotifyDataCallback( uint64 t_CallbackData,
                                const slimMessageHeaderStructT *pz_MessageHeader,
                                void *p_Message )
{
  boolean v_Error = FALSE;

  if (NULL == pz_MessageHeader)
  {
    return;
  }

  switch (pz_MessageHeader->msgId)
  {
    /* Handle SLIM open response message. */
    case eSLIM_MESSAGE_ID_OPEN_RESP:
    case eSLIM_MESSAGE_ID_SERVICE_STATUS_IND:
    case eSLIM_MESSAGE_ID_SENSOR_DATA_ENABLE_RESP:
    case eSLIM_MESSAGE_ID_SENSOR_DATA_IND:
         tm_slimHandleSensorReport(t_CallbackData,pz_MessageHeader,p_Message);
      break;
    default:
      /* Other messages are unexpected. drop it*/
      v_Error = TRUE;
      break;
  }

  if (v_Error)
  {
    MSG_3(MSG_SSID_GPSSM,MSG_LEGACY_ERROR,"TM Slim message/service id 0x%X/%d error %d",
                   pz_MessageHeader->msgId, pz_MessageHeader->service,
                   pz_MessageHeader->msgError);
  }
}

/*=============================================================================

FUNCTION
  tm_slimFillSensorResp

DESCRIPTION
  Fill response header from SLIM message header to process
  the callback in TM context

DEPENDENCIES
  None.

PARAMETERS
  pz_Header        : Pointer to Response Header 
  pz_MessageHeader : Pointer to Message Header

RETURN VALUE
  None

SIDE EFFECTS
  None.
=============================================================================*/
static void tm_slimFillSensorResp
(
  slimErrorRespMsgT *pz_Resp,
  const slimMessageHeaderStructT *pz_MessageHeader,
  const uint64 t_CallbackData
)
{
  pz_Resp->callbackData_valid = TRUE;
  pz_Resp->callbackData = t_CallbackData;
  pz_Resp->modemTimeTickMsec = cgps_TimeTickGetMsec();
  pz_Resp->respHeader.service = pz_MessageHeader->service;
  pz_Resp->respHeader.msgError = pz_MessageHeader->msgError;
  pz_Resp->respHeader.msgId = pz_MessageHeader->msgId;
  pz_Resp->respHeader.txnId = pz_MessageHeader->txnId;
  return;
}

/*=============================================================================

FUNCTION
  tm_slimFillSensorStatusInd

DESCRIPTION
  Fill Sensor Status inication Message from SLIM Callback to
  process in TM context

DEPENDENCIES
  None.

PARAMETERS
  pz_StatusResp        : Pointer to Service Indication Msg 
  pz_MessageHeader     : Pointer to Message Header
  t_CallbackData       : Callback data if any
  p_Message            : Pointer to Message Payload

RETURN VALUE
  None

SIDE EFFECTS
  None.
=============================================================================*/
static void tm_slimFillSensorStatusInd
(
  slimInjectServiceStatusEventIndMsgT *pz_StatusResp,
  const slimMessageHeaderStructT      *pz_MessageHeader,
  const uint64 t_CallbackData,
  const void* p_Message
)
{
  /*Fill Status Response Indication header*/
  pz_StatusResp->indHeader.msgError = pz_MessageHeader->msgError;
  pz_StatusResp->indHeader.msgId    = pz_MessageHeader->msgId;
  pz_StatusResp->indHeader.service  = pz_MessageHeader->service;

  /*Fill Status Response*/
  if( NULL != p_Message )
  {
    slimServiceStatusEventStructT *pz_Event =
      (slimServiceStatusEventStructT*)p_Message;
    pz_StatusResp->serviceStatusEvent.service               = pz_Event->service;
    pz_StatusResp->serviceStatusEvent.serviceStatus         = pz_Event->serviceStatus;
    pz_StatusResp->serviceStatusEvent.providerFlags         = pz_Event->providerFlags;
    pz_StatusResp->serviceStatusEvent.availableServicesMask = pz_Event->availableServicesMask;
  }
  pz_StatusResp->callbackData_valid = TRUE;
  pz_StatusResp->callbackData = t_CallbackData;
  pz_StatusResp->modemTimeTickMsec = cgps_TimeTickGetMsec();
  return;
}

/*=============================================================================

FUNCTION
  tm_slimFillSensorDataInd

DESCRIPTION
  Fill Sensor Data inication Message from SLIM Callback to
  process in TM context

DEPENDENCIES
  None.

PARAMETERS
  pz_SensorDataInd     : Pointer to Sensor Data Ind 
  pz_MessageHeader     : Pointer to Message Header
  t_CallbackData       : Callback data if any
  p_Message            : Pointer to Message Payload

RETURN VALUE
  None

SIDE EFFECTS
  None.
=============================================================================*/
static void tm_slimFillSensorDataInd
(  
   slimInjectSensorDataIndMsgT         *pz_SensorDataInd,
   const slimMessageHeaderStructT      *pz_MessageHeader,
   const uint64 t_CallbackData,
   const void* p_Message                                         
)
{
   pz_SensorDataInd->indHeader.msgError = pz_MessageHeader->msgError;
   pz_SensorDataInd->indHeader.msgId    = pz_MessageHeader->msgId;
   pz_SensorDataInd->indHeader.service  = pz_MessageHeader->service;

   /* Payload is specified only if there is no error */
   if (eSLIM_SUCCESS == pz_MessageHeader->msgError &&
       NULL != p_Message)
   {
        slimSensorDataStructT *pz_SensorData =
          (slimSensorDataStructT*)p_Message;
        /*Copy Sensor Data*/
        pz_SensorDataInd->sensorData = *pz_SensorData;
   }
   pz_SensorDataInd->callbackData_valid = TRUE;
   pz_SensorDataInd->callbackData       = t_CallbackData;
   pz_SensorDataInd->modemTimeTickMsec = cgps_TimeTickGetMsec();
}

/*=============================================================================

FUNCTION
  tm_slimHandleSensorReport

DESCRIPTION
  Sensor Baro information from SLIM to SM

DEPENDENCIES
  None.

PARAMETERS
  t_CallbackData   : Client specific callback data
  pz_MessageHeader : Message header pointer
  p_Message        : Message pointer

RETURN VALUE
  None

SIDE EFFECTS
  None.
=============================================================================*/
void tm_slimHandleSensorReport(uint64 t_CallbackData,
                               const slimMessageHeaderStructT *pz_MessageHeader,
                               void *p_Message)
{
  os_IpcMsgType* p_IpcMsg = NULL;
  techSel_SensorReportType *pz_SensorMsg = NULL;

  if ( NULL == pz_MessageHeader )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_slimHandleSensorReport: Null Info");
    return;
  }

  p_IpcMsg = os_IpcCreate(sizeof(*pz_SensorMsg), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL != p_IpcMsg)
  {
    p_IpcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_SENSOR_INFO;
    p_IpcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_IpcMsg->q_Size = sizeof(*pz_SensorMsg);

    pz_SensorMsg = (techSel_SensorReportType *)(p_IpcMsg->p_Data);

    memset(pz_SensorMsg,0,sizeof(techSel_SensorReportType));

    /*Copy the header structure*/
    memscpy(&(pz_SensorMsg->z_header), sizeof(pz_SensorMsg->z_header),
            pz_MessageHeader,sizeof(*pz_MessageHeader));

    /* Payload is specified only if there is no error.
       We are interested in SENSOR_DATA_IND and SERVICE_STATUS_IND.
       If there are errors, Sensor Meas are marked NOT_AVAILABLE*/
      switch ( pz_MessageHeader->msgId )
      {
      case eSLIM_MESSAGE_ID_OPEN_RESP:
      case eSLIM_MESSAGE_ID_SERVICE_STATUS_IND: 
         {
           tm_slimFillSensorStatusInd( &(pz_SensorMsg->z_SensorMsg.z_SensorServiceStatus),
                                       pz_MessageHeader, t_CallbackData, p_Message );
         }
         break;

      case eSLIM_MESSAGE_ID_SENSOR_DATA_IND: 
         {
           tm_slimFillSensorDataInd( &(pz_SensorMsg->z_SensorMsg.z_SensorDataInd),
                                     pz_MessageHeader, t_CallbackData,p_Message );
         }
         break;
      case eSLIM_MESSAGE_ID_SENSOR_DATA_ENABLE_RESP:
         {
           tm_slimFillSensorResp( &(pz_SensorMsg->z_SensorMsg.z_SensorErrorResp),
                                  pz_MessageHeader, t_CallbackData );

           MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM Slim Enable RSP: Error: %d, Service : %d, TXN ID : %d ,MSGID : %d",
               pz_SensorMsg->z_SensorMsg.z_SensorErrorResp.respHeader.msgError,
               pz_SensorMsg->z_SensorMsg.z_SensorErrorResp.respHeader.service,
               pz_SensorMsg->z_SensorMsg.z_SensorErrorResp.respHeader.txnId,
               pz_SensorMsg->z_SensorMsg.z_SensorErrorResp.respHeader.msgId); 
         }
         break;
      default:
        MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unhandled Message id: %d, Service : %d",
              pz_MessageHeader->msgId, pz_MessageHeader->service );
        break;
      }
    
    if (!os_IpcSend(p_IpcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage");
      (void)os_IpcDelete(p_IpcMsg);
      return;
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC message");
  }
}


