/*============================================================================
  FILE:         loc_utils.c

  OVERVIEW:     Utility functions for loc_slim and loc_sdp modules.

  DEPENDENCIES: None.
 
                Copyright (c) 2014 QUALCOMM Atheros, Inc.
                All Rights Reserved.
                Qualcomm Atheros Confidential and Proprietary
                Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc. 
                All Rights Reserved.
                Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_utils.c#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  04/05/19    yh   protocolStack is always 0 in QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND
  06/22/16    ap   Adding defensive F3 messages to debug Non-Duplicable crashe
  01/11/16    yh   LocMW refactoring
  2014-04-17  lv   Initial revision.

============================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "loc_api_internal.h"
#include "loc_utils.h"
#include "loc_qmi_shim.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
/**
 * Create an ipc message and send to LocMW task
 *
 * @param msg_id    ipc message id
 * @param loc_client_handle
 *                  LocAPI client handle
 * @param transaction_id
 *                  will be duplicated into the resulting indication
 * @param p_payload QMI request, usually data structure created by the IDL compiler
 * @param payload_size
 *                  size of the QMI request
 *
 * @return 0 if ipc has been sent successfully
 */
uint32 loc_send_ipc_to_locmw
(
   uint32 msg_id,
   int32 loc_client_handle,
   uint32 transaction_id,
   const void *p_payload,
   uint32 payload_size
)
{
  uint32 error_code = 1;
  uint32 msg_size = sizeof(loc_client_handle) + sizeof(transaction_id) + payload_size;

  if (NULL != p_payload)
  {
    os_IpcMsgType *ipc_msg_ptr =
      os_IpcCreate(msg_size, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_LOC_MIDDLEWARE);
    if (NULL != ipc_msg_ptr)
    {
      // casting to void * first to avoid lint error of memory size
      byte *p_insertion_point = (byte *)(void *)ipc_msg_ptr->p_Data;
      int32 *p_client_handle = NULL;
      uint32 *p_transaction_id = NULL;

      // calculate offset
      p_client_handle = (int32 *)p_insertion_point;
      p_insertion_point += sizeof(int32);
      p_transaction_id = (uint32 *)p_insertion_point;
      p_insertion_point += sizeof(uint32);

      // first is client handle
      *p_client_handle = (uint32)loc_client_handle;
      // copy the transaction id
      *p_transaction_id = transaction_id;
      // make a shallow copy of the request
      (void)memscpy((void *)p_insertion_point, payload_size, p_payload, payload_size);

      ipc_msg_ptr->q_MsgId = msg_id;
      if (TRUE == os_IpcSend(ipc_msg_ptr, THREAD_ID_LOC_MIDDLEWARE))
      {
        // ipc sent
        error_code = 0;
      }
      else
      {
        // ipc not sent
        (void)os_IpcDelete(ipc_msg_ptr);
        error_code = 2;
      }
    }
    else
    {
      // ipc memory allocation failure, ipc not sent
      error_code = 3;
    }
  }
  else
  {
    // null pointer for payload. this shall never happen
    error_code = 4;
  }

  if (0 != error_code)
  {
    LOC_MSG_ERROR("[LOC] loc_send_ipc_to_locmw: "
      "error code: %u", error_code, 0, 0);
  }
  return error_code;
}


/**
 * Unpack an ipc message sent to LocMW.
 *
 * @param p_ipc_msg_in
 *                  [in] ipc message
 * @param p_loc_client_handle
 *                  [out] LocAPI client handle
 * @param p_transaction_id
 *                  [out] transaction id in the request
 * @param p_payload [out] QMI request. structure created by IDL compiler.
 *                  size must be enough to hold payload_size
 * @param payload_size
 *                  [in] size of the QMI request
 *
 * @return 0 if ipc has been unpacked successfully
 */
uint32 loc_unpack_ipc_to_locmw
(
  const os_IpcMsgType *p_ipc_msg_in,
  int32 *p_loc_client_handle,
  uint32 *p_transaction_id,
  void *p_payload,
  uint32 payload_size
)
{
  uint32 error_code = 1;
  uint32 msg_size = sizeof(int32) + sizeof(uint32) + payload_size;

  if (NULL != p_ipc_msg_in &&
      NULL != p_loc_client_handle &&
      NULL != p_transaction_id &&
      NULL != p_payload &&
      msg_size <= p_ipc_msg_in->q_Size)
  {
    byte *p_extraction_point = (byte *)(void *)p_ipc_msg_in->p_Data;
    int32 *p_client_handle = NULL;
    uint32 *p_transaction_id_store = NULL;
    void *p_payload_store = NULL;

    // calculate offset
    p_client_handle = (int32 *)p_extraction_point;
    p_extraction_point += sizeof(int32);
    p_transaction_id_store = (uint32 *)p_extraction_point;
    p_extraction_point += sizeof(uint32);
    p_payload_store = (void *)p_extraction_point;

    // retrieve client id
    *p_loc_client_handle = *p_client_handle;
    // retrieve transaction id
    *p_transaction_id = *p_transaction_id_store;
    // retrieve payload
    (void)memscpy(p_payload, payload_size, p_payload_store, payload_size);

    error_code = 0;
  }
  else
  {
    // null pointer for something. this shall never happen
    error_code = 2;
  }

  if (0 != error_code)
  {
    LOC_MSG_ERROR("[LOC] loc_unpack_ipc_to_locmw: "
      "error code: %u", error_code, 0, 0);
  }
  return error_code;
}

/**
 * get a somewhat unique transaction id for debugging and message matching purposes
 *
 * @return qmiloc transaction id
 */
uint32 loc_qmiloc_get_transaction_id
(
  void
)
{
  // it might be better if transaction can be globally unique and time varying so we can survive
  // state machine resets and the like
  // however, it's a bit difficult to acquire true time varying source from higher level code
  static uint32 transaction_id = 0;
  ++transaction_id;

  return transaction_id;
}

/*---------------------------------------------------------------------------
@brief
  Function to generate an IPC to locMW task

@param[in] e_MsgId       : LocMW IPC message ID
@param[in] q_PayloadSize : Payload size
@param[in] p_PayloadData : Pointer to the payload data

@retval    TRUE    if IPC posted to LocMW successfully
@retval    FALSE   if IPC posted to LocMW unsuccessfully
---------------------------------------------------------------------------*/
boolean locMW_SendIpcMsg
(
  loc_middleware_msg_id_e_type e_MsgId,
  uint32                       q_PayloadSize,
  const void *                 p_PayloadData
)
{
  os_IpcMsgType* p_IpcMsg = os_IpcCreate(q_PayloadSize, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_LOC_MIDDLEWARE);

  LOC_MSG_LOW("locMW_SendIpcMsg: Sending IPC MsgId=%d to LocMW", e_MsgId, 0, 0);

  if (NULL != p_IpcMsg)
  {
    /* Copy the payload */
    (void)memscpy(p_IpcMsg->p_Data, q_PayloadSize, p_PayloadData, q_PayloadSize);

    p_IpcMsg->q_MsgId = e_MsgId;
    if(FALSE == os_IpcSend(p_IpcMsg, THREAD_ID_LOC_MIDDLEWARE))
    {
      LOC_MSG_ERROR("locMW_SendIpcMsg: Sending IPC MsgId=%d to LocMW failed", e_MsgId, 0, 0);
      os_IpcDelete(p_IpcMsg);
      return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------
@brief
  Function to convert server address from QMI LOC to PDAPI format

  Used by both loc_pa and loc_client (to translate the custom location server
  address).

@param[in]  pz_SetServerReq : Set Server Address Request received from client
@param[out] pz_ServerAddr   : PDSM Server Address format

@retval    LOC_API_SUCCESS  Called successfully
@retval    <Anything Else>  Called unsuccessfully
---------------------------------------------------------------------------*/
uint32 locMW_ConvertAddrToPdapi
(
  const qmiLocSetServerReqMsgT_v02* pz_SetServerReq,
  pdsm_server_address_s_type*       pz_ServerAddr
)
{
  if(NULL == pz_SetServerReq || NULL == pz_ServerAddr)
  {
    LOC_MSG_ERROR("locMW_ConvertAddrToPdapi: NULL pointer pz_SetServerReq = %p, pz_ServerAddr = %p",
                   pz_SetServerReq, pz_ServerAddr, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  /* Priority given to IPV4 Address if both are given */
  if (pz_SetServerReq->ipv4Addr_valid)
  {
    pz_ServerAddr->server_addr_type                       = PDSM_SERVER_TYPE_IPV4_ADDRESS;
    pz_ServerAddr->server_adrs.server_adrs_v4.server_adrs = pz_SetServerReq->ipv4Addr.addr;
    pz_ServerAddr->server_adrs.server_adrs_v4.port_id     = pz_SetServerReq->ipv4Addr.port;
    return LOC_API_SUCCESS;
  }
  else if (pz_SetServerReq->urlAddr_valid)
  {
    pz_ServerAddr->server_addr_type = PDSM_SERVER_TYPE_URL_ADDRESS;

    /* Bounded Buffer Copy of incoming string */
	
    pz_ServerAddr->server_adrs.server_adrs_url.url_length = 
    (uint16)GNSS_STRLCPY(pz_ServerAddr->server_adrs.server_adrs_url.url,
                 pz_SetServerReq->urlAddr,
                 sizeof(pz_ServerAddr->server_adrs.server_adrs_url.url));
                 
    if(PDSM_MAX_URL_ADDR_LEN >= pz_ServerAddr->server_adrs.server_adrs_url.url_length)
   {
     LOC_MSG_LOW("locMW_ConvertAddrToPdapi: URL copied safely max_len = %d , URL len = %d",
                   PDSM_MAX_URL_ADDR_LEN, 
                   pz_ServerAddr->server_adrs.server_adrs_url.url_length, 0);
    return LOC_API_SUCCESS;
   }
    else
    {
      ERR_FATAL("locMW_ConvertAddrToPdapi: Copying URL address corrupted other memories max_buf_len =%d, copied URL len =%d",
                PDSM_MAX_URL_ADDR_LEN, 
                pz_ServerAddr->server_adrs.server_adrs_url.url_length, 0);
      return LOC_API_GENERAL_FAILURE;
    }
  }
  else
  {
    LOC_MSG_ERROR("locMW_ConvertAddrToPdapi: no valid Address", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

}

/*---------------------------------------------------------------------------
@brief
  Function to convert fix mode from QMI LOC to PDAPI format

@param[in]  e_OperationMode : Operation mode received from client

@retval    pdsm_pd_session_operation_e_type
---------------------------------------------------------------------------*/
pdsm_pd_session_operation_e_type locMW_ConvertFixModeToPdapi
(
  qmiLocOperationModeEnumT_v02 e_OperationMode
)
{
  switch (e_OperationMode)
  {
    case eQMI_LOC_OPER_MODE_STANDALONE_V02:
    {
      return PDSM_SESSION_OPERATION_STANDALONE_ONLY;
    }

    /* Set WWAN mode to MSA, QoS=0 will be set in the calling function */
    case eQMI_LOC_OPER_MODE_WWAN_V02:
    case eQMI_LOC_OPER_MODE_MSA_V02:
    {
      return PDSM_SESSION_OPERATION_MSASSISTED;
    }

    case eQMI_LOC_OPER_MODE_CELL_ID_V02:
    {
      return PDSM_SESSION_OPERATION_CELL_ID;
    }

    /* Set default to MSB */
    case eQMI_LOC_OPER_MODE_DEFAULT_V02:
    case eQMI_LOC_OPER_MODE_MSB_V02:
    default:
    {
      return PDSM_SESSION_OPERATION_MSBASED;
    }
  }
}

/*---------------------------------------------------------------------------
@brief
  Function to send IPC to LocMW task for servicing Indication to previous
  QMI_LOC request

@param[in] l_ClientHandle : Loc client handle
@param[in] q_QmiMsgId     : QMI_LOC Msg Id
@param[in] p_QmiIndMsg    : QMI_LOC Indication Msg Data
@param[in] q_LocApiStatus : Parameter Status from Loc API, set to
                            LOC_API_CUSTOM_ERROR_STATUS to use custom
                            error status which has been set within the
                            Indication message pointed by p_QmiIndMsg

@retval    TRUE    if IPC posted to LocMW successfully
@retval    FALSE   if IPC posted to LocMW unsuccessfully
---------------------------------------------------------------------------*/
boolean locMW_SendQmiIndicationProxy
(
   loc_client_handle_type     l_ClientHandle,
   uint32                     q_QmiMsgId,
   void*                      p_QmiIndMsg,
   uint32                     q_LocApiStatus
)
{
   os_IpcMsgType*                pz_IpcMsg = NULL;
   locMW_QmiLocIndMsgStructT*    pz_LocIndMsg = NULL;
   boolean                       v_RetVal = FALSE;

   do
   {
      /* Generate IPC message to LocMW */
      pz_IpcMsg = os_IpcCreate(sizeof(*pz_LocIndMsg), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_LOC_MIDDLEWARE);
      if(NULL == pz_IpcMsg)
      {
         LOC_MSG_ERROR("locMW_SendQmiIndicationProxy: os_IpcCreate failed", 0, 0, 0);
         break;
      }

      LOC_MSG_MED("locMW_SendQmiIndicationProxy: ClientHandle=%d, q_QmiMsgId=0x%04x, ParamStatus=%d",
                  l_ClientHandle, q_QmiMsgId, q_LocApiStatus);

      pz_IpcMsg->q_MsgId = LM_MIDDLEWARE_MSG_ID_QMI_LOC_MW_IND;
      pz_LocIndMsg = (locMW_QmiLocIndMsgStructT*)(pz_IpcMsg->p_Data);
      pz_LocIndMsg->l_ClientHandle = l_ClientHandle;
      pz_LocIndMsg->q_QmiIndMsgId  = q_QmiMsgId;
      pz_LocIndMsg->p_QmiIndMsg    = p_QmiIndMsg;
      pz_LocIndMsg->q_LocApiStatus = q_LocApiStatus;

      /* Generate IPC message to LocMW */
      if(FALSE == os_IpcSend(pz_IpcMsg, THREAD_ID_LOC_MIDDLEWARE))
      {
         LOC_MSG_ERROR("locMW_SendQmiIndicationProxy: Sending IPC to LocMW failed", 0, 0, 0);
         os_IpcDelete(pz_IpcMsg);
      }
      else
      {
         v_RetVal = TRUE;
      }
   }while(0);

   /* The caller of locMW_SendQmiIndicationProxy allocated pz_QmiIndMsg,
   free the heap memory for IpcSend failure case */
   if((FALSE == v_RetVal) && (NULL != p_QmiIndMsg))
   {
      loc_free(p_QmiIndMsg);
   }

   return v_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to send QMI_LOC indication to client for previous QMI_LOC request.

@param[in] p_IpcMsg : IPC Message containing the QMI_LOC indication data

@retval    TRUE    if IPC message is handled successfully
@retval    FALSE   if IPC message is handled unsuccessfully
---------------------------------------------------------------------------*/
boolean locMW_HandleQmiIndication
(
   const os_IpcMsgType* p_IpcMsg
)
{
   qmiLocStatusEnumT_v02 e_IndStatus;
   boolean               v_RetVal = FALSE;
   qmiLocStatusEnumT_v02 *p_Temp = NULL;

   if (NULL == p_IpcMsg)
   {
      LOC_MSG_ERROR("locMW_HandleQmiIndication: Invalid IPC message", 0, 0, 0);
      return v_RetVal;
   }
   else
   {
      locMW_QmiLocIndMsgStructT* pz_LocIndMsg = (locMW_QmiLocIndMsgStructT *)(p_IpcMsg->p_Data);

      if(NULL == pz_LocIndMsg)
      {
         LOC_MSG_ERROR("locMW_HandleQmiIndication: NULL pz_LocIndMsg",0,0,0);
         return v_RetVal;
      }

      /* Translate Indication Status from LocMW to QMI_LOC type */
      switch(pz_LocIndMsg->q_LocApiStatus)
      {
         case LOC_API_SUCCESS:
            e_IndStatus = eQMI_LOC_SUCCESS_V02;
            break;

         case LOC_API_INVALID_PARAMETER:
            e_IndStatus = eQMI_LOC_INVALID_PARAMETER_V02;
            break;

         case LOC_API_UNSUPPORTED:
            e_IndStatus = eQMI_LOC_UNSUPPORTED_V02;
            break;

         case LOC_API_ENGINE_BUSY:
            e_IndStatus = eQMI_LOC_ENGINE_BUSY_V02;
            break;

         case LOC_API_PHONE_OFFLINE:
            e_IndStatus = eQMI_LOC_PHONE_OFFLINE_V02;
            break;

         case LOC_API_TIMEOUT:
            e_IndStatus = eQMI_LOC_TIMEOUT_V02;
            break;

         case LOC_API_XTRA_VERSION_CHECK_FAILURE:
            e_IndStatus = eQMI_LOC_XTRA_VERSION_CHECK_FAILURE_V02;
            break;

         case LOC_API_GNSS_DISABLED:
            e_IndStatus = eQMI_LOC_GNSS_DISABLED_V02;
            break;

         case LOC_API_CUSTOM_ERROR_STATUS:
            // for custom error status, really does not matter what to set
            e_IndStatus = eQMI_LOC_SUCCESS_V02;
            break;

         default:
            e_IndStatus = eQMI_LOC_GENERAL_FAILURE_V02;
            break;
      }

      /* Set the status for the indication messages,
         note: QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND doesn't have status TLV */
      if((NULL != pz_LocIndMsg->p_QmiIndMsg)
          && (LOC_API_CUSTOM_ERROR_STATUS != pz_LocIndMsg->q_LocApiStatus)
          && (QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND_V02 != pz_LocIndMsg->q_QmiIndMsgId))
      {
         p_Temp = (qmiLocStatusEnumT_v02 *)pz_LocIndMsg->p_QmiIndMsg;
         *p_Temp = e_IndStatus;
      }

      v_RetVal = locQmi_ProcessInd(pz_LocIndMsg->l_ClientHandle,
                                 pz_LocIndMsg->q_QmiIndMsgId,
                                 pz_LocIndMsg->p_QmiIndMsg,
                                 e_IndStatus);
      /* p_QmiIndMsg is allocated by the caller of locMW_SendQmiIndicationProxy
         free here */
      if(NULL != pz_LocIndMsg->p_QmiIndMsg)
      {
         loc_free(pz_LocIndMsg->p_QmiIndMsg);
         pz_LocIndMsg->p_QmiIndMsg = NULL;
      }
   }

   return v_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to send IPC to LocMW task for LocMW command

@param[in] l_ClientHandle : Loc client handle
@param[in] e_LocCmdType   : LocMW command ID
@param[in] t_EventType    : LocMW event type
@param[in] p_LocCmdData   : LocMW command data

@retval    TRUE    if IPC posted to LocMW successfully
@retval    FALSE   if IPC posted to LocMW unsuccessfully
---------------------------------------------------------------------------*/
boolean locMW_SendLocCmdProxy
(
   loc_client_handle_type     l_ClientHandle,
   LocMW_LocCmdEnumT          e_LocCmdType,
   locClnt_EventMaskType      t_EventType,
   void*                      p_LocCmdData
)
{
   os_IpcMsgType*             pz_IpcMsg = NULL;
   LocMW_LocCmdStructT*       pz_CmdData = NULL;
   boolean                    v_RetVal = FALSE;

   do
   {
      /* Generate IPC message to LocMW */
      pz_IpcMsg = os_IpcCreate(sizeof(*pz_CmdData), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_LOC_MIDDLEWARE);
      if(NULL == pz_IpcMsg)
      {
         LOC_MSG_ERROR("locMW_SendQmiIndicationProxy: os_IpcCreate failed", 0, 0, 0);
         break;
      }

      LOC_MSG_MED_1("locMW_SendLocCmdProxy %d", e_LocCmdType);

      pz_IpcMsg->q_MsgId = LM_MIDDLEWARE_MSG_ID_PDAPI;
      pz_CmdData = (LocMW_LocCmdStructT*)(pz_IpcMsg->p_Data);
      pz_CmdData->l_ClientHandle   = l_ClientHandle;
      pz_CmdData->e_CmdType        = e_LocCmdType;
      pz_CmdData->t_EventType      = t_EventType;
      pz_CmdData->p_QmiEventIndMsg = p_LocCmdData;

      /* Generate IPC message to LocMW */
      if(FALSE == os_IpcSend(pz_IpcMsg, THREAD_ID_LOC_MIDDLEWARE))
      {
         LOC_MSG_ERROR("locMW_SendLocCmdProxy: Sending IPC to LocMW failed", 0, 0, 0);
         os_IpcDelete(pz_IpcMsg);
      }
      else
      {
#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
        if ((LOC_EVENT_GNSS_MEASUREMENT_REPORT == t_EventType) ||
            (LOC_EVENT_GNSS_NHZ_MEASUREMENT_REPORT == t_EventType))
        {
          qmiLocEventGnssSvMeasInfoIndMsgT_v02  *pz_GnssSvMeasIndMsg = (qmiLocEventGnssSvMeasInfoIndMsgT_v02*)p_LocCmdData;
          LOC_MSG_HIGH_3("EVENT 0x%X: Sent IPC 10 to LocMW Task. Sequence %d of %d",
                         t_EventType, pz_GnssSvMeasIndMsg->seqNum, pz_GnssSvMeasIndMsg->maxMessageNum);

        }
#endif
          v_RetVal = TRUE;
      }
   }while(0);

   /* The caller of locMW_SendLocCmdProxy allocated p_LocData,
   free the heap memory for IpcSend failure case */
   if((FALSE == v_RetVal) && (NULL != p_LocCmdData))
   {
      loc_free(p_LocCmdData);
   }

   return v_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to handle LocMW command

@param[in] p_IpcMsg : IPC Message containing the LocMW command data

@retval    TRUE    if IPC message is handled successfully
@retval    FALSE   if IPC message is handled unsuccessfully
---------------------------------------------------------------------------*/
boolean locMW_HandleLocCmd
(
  const os_IpcMsgType* p_IpcMsg
)
{
   if (NULL == p_IpcMsg)
   {
      LOC_MSG_ERROR("locMW_HandleLocCmd: Invalid IPC message", 0, 0, 0);
      return FALSE;
   }
   else
   {
      LocMW_LocCmdStructT*   pz_CmdData = (LocMW_LocCmdStructT*)(p_IpcMsg->p_Data);

      LOC_MSG_MED_1("locMW_HandleLocCmd %d", pz_CmdData->e_CmdType);
      switch (pz_CmdData->e_CmdType)
      {
      case LOC_CMD_TYPE_PD_SCHEDULE:
      case LOC_CMD_TYPE_REPORT_POSITION:
         locPd_ProcessCmdRequest (pz_CmdData);
         break;

      case LOC_CMD_TYPE_NOTIFY_CLIENT:
         locClnt_ProcessCmdRequest (pz_CmdData);
         break;

      case LOC_CMD_TYPE_INJECT_XTRA_DATA:
         locXtra_ProcessCmdRequest (pz_CmdData);
         break;

      case LOC_CMD_TYPE_SILENTLY_DENY_NI_REQUEST:
         locNi_ProcessCmdRequest (pz_CmdData);
         break;

      case LOC_CMD_TYPE_INIT_QMI_LOC:
         locQmi_ProcessCmdRequest(pz_CmdData);
         break;

      default:
         break;
      }

      if(NULL != pz_CmdData->p_QmiEventIndMsg)
      {
         loc_free(pz_CmdData->p_QmiEventIndMsg);
         pz_CmdData->p_QmiEventIndMsg = NULL;
      }
   }
   return TRUE;
}

/*---------------------------------------------------------------------------
@brief
This function converts internal loc errors to QMI Errors.

@param[in] q_LocErr : LOC API Error code

@retval    QMI Error code
---------------------------------------------------------------------------*/
qmi_error_type_v01 locMW_ConvertToQmiErr(uint32 q_LocErr)
{
  switch (q_LocErr)
  {
    case LOC_API_SUCCESS:
      return QMI_ERR_NONE_V01;

    case LOC_API_GENERAL_FAILURE:
      return QMI_ERR_GENERAL_V01;

    case LOC_API_XTRA_VERSION_CHECK_FAILURE:
    case LOC_API_UNSUPPORTED:
      return QMI_ERR_NOT_SUPPORTED_V01;

    case LOC_API_INVALID_HANDLE:
      return QMI_ERR_INVALID_HANDLE_V01;

    case LOC_API_INVALID_PARAMETER:
      return QMI_ERR_INVALID_ARG_V01;

    case  LOC_API_ENGINE_BUSY:
      return QMI_ERR_OP_IN_PROGRESS_V01;

    case LOC_API_PHONE_OFFLINE:
      return QMI_ERR_DEVICE_NOT_READY_V01;

    case LOC_API_TIMEOUT:
      return QMI_ERR_INJECT_TIMEOUT_V01;

    case LOC_API_GNSS_DISABLED:
      return QMI_ERR_DISABLED_V01;

    default:
      return QMI_ERR_GENERAL_V01;
  }
}