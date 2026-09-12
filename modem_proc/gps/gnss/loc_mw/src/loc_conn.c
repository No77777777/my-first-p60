/******************************************************************************
  @file:  loc_conn.c
  @brief: Location Middleware User Plane Data Connection Manager

  DESCRIPTION
  This module contains routines used to manage User Plane Data Connections
 
 
  -----------------------------------------------------------------------------
  Copyright (c) 2010-2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Copyright (c) 2013-2014 QUALCOMM Atheros, Inc.
  All Rights Reserved. 
  QUALCOMM Proprietary and Confidential.
  Copyright (c) 2015-2019 Qualcomm Technologies, Inc. All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  -----------------------------------------------------------------------------
 ******************************************************************************/


/*=============================================================================
$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_conn.c#2 $
$DateTime: 2020/01/29 08:50:36 $
$Author: pwbldsvc $
=============================================================================*/

#include "comdef.h"     /* Definition for basic types and macros */
#include "customer.h"   /* Customer configuration file */
#include "msg.h"

#include "msg.h"
#include "queue.h"

#include "aries_os_api.h"
#include "pdapi.h"
#include "pdsm_atl.h"

#include "loc_api_2.h"
#include "loc_client.h"
#include "loc_pd.h"
#include "loc_conn.h"
#include "loc_xtra.h"
#include "loc_pa.h"
#include "loc_ni.h"
#include "loc_wifi.h"
#include "loc_api_internal.h"
#include "loc_qmi_shim.h"
#include "loc_utils.h"

/*===================================================================
This current module supports only one connection at any time. If there
are two simulatenous connection need to be supported at the same time,
this module will need to be reimplemented.
====================================================================*/
static boolean gv_PdsmAtlRegistered = FALSE;

// callback function when GPS engine needs to open a data session
pdsm_atl_operation_return_value_e_type locConn_OpenCb
(
  pdsm_atl_session_handle_type  l_SessionHandle,
  pdsm_atl_open_params_type     z_OpenParam
);

// callback function when GPS engine is done with the data session
pdsm_atl_operation_return_value_e_type locConn_CloseCb
(
  pdsm_atl_session_handle_type  l_SessionHandle,
  boolean                       v_e911Close
);

// Routine to initialize the location middleware conn module.
boolean locConn_Init (void)
{
   boolean v_Status=TRUE;

   // Registers with PDSM ATL module to only process connection open and
   // close request
#ifndef FEATURE_GNSS_SA
   if(FALSE == gv_PdsmAtlRegistered)
   {
      v_Status = pdsm_atl_l2_proxy_reg (PDSM_ATL_TYPE_IP,
                                      locConn_OpenCb,
                                      locConn_CloseCb);
      gv_PdsmAtlRegistered = TRUE;
   }
#endif
   memset (&(loc_middleware_data.z_locConn_Data),
           0,
           sizeof (locConn_ModuleDataType));

   return v_Status;
}

// Routine to Deinitialize the location middleware conn module.
boolean locConn_DeInit (void)
{
   boolean v_Status=TRUE;

   // Registers with PDSM ATL module to only process connection open and
   // close request
#ifndef FEATURE_GNSS_SA
   if(TRUE == gv_PdsmAtlRegistered)
   {
      v_Status = pdsm_atl_l2_proxy_dereg (PDSM_ATL_TYPE_IP);
      gv_PdsmAtlRegistered = FALSE;
   }
#endif
   memset (&(loc_middleware_data.z_locConn_Data),
           0,
           sizeof (locConn_ModuleDataType));

   return v_Status;
}

// Callback function when GPS engine needs a data data session to be brought up on A-proc
pdsm_atl_operation_return_value_e_type locConn_OpenCb
(
   pdsm_atl_session_handle_type  l_SessionHandle,
   pdsm_atl_open_params_type     z_OpenParam
)
{
   boolean                       v_Result = FALSE;

   LOC_MSG_HIGH ("locConn_OpenCb session handle = %d, pdsm_atl_iface_name_e_type = %d \n", l_SessionHandle, z_OpenParam.iface_name , 0);
#ifndef FEATURE_GNSS_SA
   // Inform PDSM that ATL event has been received
   pdsm_atl_post_event (PDSM_ATL_TYPE_IP,
                        l_SessionHandle,
                        PDSM_ATL_EVENT_L2_PROXY_OPEN_ACK,
                        z_OpenParam);
#endif
   // see if there is anyone to handle this event : LOC_EVENT_LOCATION_SERVER_REQUEST
   if(locClnt_IsServerRequestHandlerRegistered())
   {
      qmiLocEventLocationServerConnectionReqIndMsgT_v02* pz_SrvrReq = NULL;
      locConn_ModuleDataType*          pz_locConn_Data = &(loc_middleware_data.z_locConn_Data);
      LocMW_LocCmdStructT              z_LocCmd = {0};

      pz_locConn_Data->l_SessionHandle = l_SessionHandle;
      memscpy (&(pz_locConn_Data->z_OpenParam), sizeof(pz_locConn_Data->z_OpenParam), &z_OpenParam, sizeof (z_OpenParam));

      pz_SrvrReq = (qmiLocEventLocationServerConnectionReqIndMsgT_v02 *)loc_calloc(sizeof(*pz_SrvrReq));
      if(NULL != pz_SrvrReq)
      {
         z_LocCmd.p_QmiEventIndMsg = pz_SrvrReq;
         z_LocCmd.t_EventType = LOC_EVENT_LOCATION_SERVER_REQUEST;

         pz_SrvrReq->requestType = eQMI_LOC_SERVER_REQUEST_OPEN_V02;
           // In Aries 4.0 (LOC API 1.0), open connection request only instructs A-proc to open up the connection on available air interface.
           // In Aries 5.0 (LOC API 1.1), open connection request will include the protocol type so the A-proc will know which server to connect to.

         LOC_MSG_MED ("locConn_OpenCb Apn_type = %ll, bearer_type = %d \n", 
                 pz_SrvrReq->apnTypeMask, z_OpenParam.bearer_type , 0);

         v_Result = TRUE;
           /*Set the connection type*/
         switch(z_OpenParam.connection_type) 
         {
            case PDSM_ATL_CONNECTION_LBS:
               pz_SrvrReq->wwanType = eQMI_LOC_WWAN_TYPE_AGNSS_V02;
               break;
            case PDSM_ATL_CONNECTION_WWAN_INTERNET:
               pz_SrvrReq->wwanType = eQMI_LOC_WWAN_TYPE_INTERNET_V02;
               break;
            case PDSM_ATL_CONNECTION_LBS_EMERGENCY:
               pz_SrvrReq->wwanType = eQMI_LOC_WWAN_TYPE_AGNSS_EMERGENCY_V02;
               break;
            default:
               // Unrecognised connection type . Flag an error.  
               LOC_MSG_ERROR ("locConn_OpenCb, unrecognised connection type %d", z_OpenParam.connection_type, 0, 0);
               v_Result = FALSE;
               break;
         }

         /*copy the APN type mask*/
         pz_SrvrReq->apnTypeMask_valid = TRUE;
         pz_SrvrReq->apnTypeMask = z_OpenParam.apn_type;

         /*Copy the Subscription ID*/
         pz_SrvrReq->subId_valid = TRUE;
         switch(z_OpenParam.as_id) 
         {
            case SYS_MODEM_AS_ID_1:
               pz_SrvrReq->subId = eQMI_LOC_SYS_MODEM_AS_ID_1_V02;
               break;
            case SYS_MODEM_AS_ID_2:
               pz_SrvrReq->subId = eQMI_LOC_SYS_MODEM_AS_ID_2_V02;
               break;
            case SYS_MODEM_AS_ID_3:
               pz_SrvrReq->subId = eQMI_LOC_SYS_MODEM_AS_ID_3_V02;
               break;
            default:
               // Unrecognised connection type . Flag an error.  
               LOC_MSG_ERROR ("locConn_OpenCb, unrecognised AS ID type %d, use default :%d",
                               z_OpenParam.as_id, eQMI_LOC_SYS_MODEM_AS_ID_1_V02, 0);
               pz_SrvrReq->subId = eQMI_LOC_SYS_MODEM_AS_ID_1_V02;
               break;
         }

         /*Set the correct bearer type*/
         pz_SrvrReq->bearerType_valid = TRUE;
         switch(z_OpenParam.bearer_type)
         {
           case PDSM_ATL_BEARER_TYPE_WWAN:
                pz_SrvrReq->bearerType = eQMI_LOC_BEARER_TYPE_WWAN_V02;
                break;
           case PDSM_ATL_BEARER_TYPE_WLAN:
                pz_SrvrReq->bearerType = eQMI_LOC_BEARER_TYPE_WLAN_V02;
                break;
           case PDSM_ATL_BEARER_TYPE_ANY:
                pz_SrvrReq->bearerType = eQMI_LOC_BEARER_TYPE_ANY_V02;
                break;
           default:
                pz_SrvrReq->bearerType = eQMI_LOC_BEARER_TYPE_ANY_V02;
                break;                
         }

         if(TRUE == v_Result) 
         {
            // Session handle will be used in future when there is a need to talk to MPC and PDE simultaneously
            pz_SrvrReq->connHandle = l_SessionHandle;

             // Deliver the connection open request event to the registered location client
            if(TRUE != locMW_SendLocCmdProxy(
                        LOC_HANDLE_REGISTERED_CLIENTS,
                        LOC_CMD_TYPE_NOTIFY_CLIENT,
                        LOC_EVENT_LOCATION_SERVER_REQUEST,
                        (void *)pz_SrvrReq))
            {
               v_Result = FALSE;
            }
            else
            {
               // IPC sent, we're done.
               LOC_MSG_LOW("locConn_OpenCb, IPC sent to loc MW task", 0, 0, 0);
            }
         }
         else
         {
            loc_free(pz_SrvrReq);
         }
      }
      else
      {
         // memory allocation error
         v_Result = FALSE;
      }
   }
   else
   {
       // no one can handle server request, let's fail it now
      v_Result = FALSE;

      LOC_MSG_HIGH ("locConn_OpenCb: no client to handle open request. reject", 0,0,0);
   }

   if(TRUE != v_Result)
   {
      // TODO: what else can we do?
      LOC_MSG_ERROR ("locConn_OpenCb, failed to inform client", 0, 0, 0);
      return PDSM_ATL_OPERATION_FAIL;
   }
   else
   {
      return PDSM_ATL_OPERATION_WAIT;
   }
}

// callback function when GPS engine is done with the data session
pdsm_atl_operation_return_value_e_type locConn_CloseCb
(
  pdsm_atl_session_handle_type  l_SessionHandle,
  boolean                       v_e911Close
)
{
   boolean                       v_Result = FALSE;
   locConn_ModuleDataType*       pz_locConn_Data = &(loc_middleware_data.z_locConn_Data);

   pz_locConn_Data->l_SessionHandle = l_SessionHandle;
   memset (&(pz_locConn_Data->z_OpenParam), 0, sizeof (pdsm_atl_open_params_type));

   LOC_MSG_HIGH ("locConn_CloseCb session handle = %d", l_SessionHandle, 0 , 0);
#ifndef FEATURE_GNSS_SA
   pdsm_atl_post_event (PDSM_ATL_TYPE_IP,
                        l_SessionHandle,
                        PDSM_ATL_EVENT_L2_PROXY_CLOSE_ACK,
                        pz_locConn_Data->z_OpenParam);
#endif
   // see if there is anyone to handle this event : LOC_EVENT_LOCATION_SERVER_REQUEST
   if(locClnt_IsServerRequestHandlerRegistered())
   {
      qmiLocEventLocationServerConnectionReqIndMsgT_v02*    pz_SrvrReq =
         (qmiLocEventLocationServerConnectionReqIndMsgT_v02 *)loc_calloc(sizeof(*pz_SrvrReq));

      if(NULL != pz_SrvrReq)
      {
         pz_SrvrReq->requestType = eQMI_LOC_SERVER_REQUEST_CLOSE_V02;
         pz_SrvrReq->connHandle = l_SessionHandle;

            // Deliver the connection open request event to the registered location client
         if ( TRUE == locMW_SendLocCmdProxy(
                        LOC_HANDLE_REGISTERED_CLIENTS,
                        LOC_CMD_TYPE_NOTIFY_CLIENT,
                        LOC_EVENT_LOCATION_SERVER_REQUEST,
                        (void *)pz_SrvrReq))
         {
            // IPC sent, we're done.
            v_Result = TRUE;
         }
      }
      else
      {
         // memory allocation error
         v_Result = FALSE;
      }
   }
   else
   {
       // no one can handle server request, let's fail it now
       v_Result = FALSE;

       LOC_MSG_HIGH ("locConn_CloseCb: no client to handle close request. reject", 0,0,0);
   }

   if(TRUE != v_Result)
   {
      // TODO: what else can we do?
      LOC_MSG_ERROR ("locConn_CloseCb, failed to inform client", 0, 0, 0);
      return PDSM_ATL_OPERATION_FAIL;
   }
   else
   {
      return PDSM_ATL_OPERATION_WAIT;
   }
}

static uint32 locConn_InformLocServerConnStatus
(
   loc_client_handle_type l_ClientHandle,
   const qmiLocInformLocationServerConnStatusReqMsgT_v02* pz_InformLocServerConnStatusReq
)
{
   uint32                     q_Ret = LOC_API_GENERAL_FAILURE;
   locConn_ModuleDataType*    pz_locConn_Data = &(loc_middleware_data.z_locConn_Data);
   pdsm_atl_event_type        q_event = PDSM_ATL_EVENT_CLEAR;
#ifndef FEATURE_GNSS_SA
   boolean                    v_Status= FALSE;
#endif

   if (l_ClientHandle < 0 || NULL == pz_InformLocServerConnStatusReq) 
   {
      LOC_MSG_ERROR( "locConn_InformLocServerConnStatus, l_ClientHandle = %d, pz_InformLocServerConnStatusReq = 0x%p",
                     l_ClientHandle,pz_InformLocServerConnStatusReq,0);
      return LOC_API_INVALID_PARAMETER;
   }

   pz_locConn_Data->l_SessionHandle = pz_InformLocServerConnStatusReq->connHandle;
   if(eQMI_LOC_SERVER_REQUEST_OPEN_V02 == pz_InformLocServerConnStatusReq->requestType)
   {
      if(eQMI_LOC_SERVER_REQ_STATUS_SUCCESS_V02 == pz_InformLocServerConnStatusReq->statusType)
      {
         q_event = PDSM_ATL_EVENT_OPEN_SUCCESS;
         if(pz_InformLocServerConnStatusReq->apnProfile_valid)
         {
            switch(pz_InformLocServerConnStatusReq->apnProfile.pdnType)
            {
               case eQMI_LOC_APN_PROFILE_PDN_TYPE_IPV4_V02:
                  pz_locConn_Data->z_OpenParam.pdp_type = PDSM_ATL_PDP_IP;
                  break;
               case eQMI_LOC_APN_PROFILE_PDN_TYPE_IPV6_V02:
                  pz_locConn_Data->z_OpenParam.pdp_type = PDSM_ATL_PDP_IPV6;
                  break;
               case eQMI_LOC_APN_PROFILE_PDN_TYPE_IPV4V6_V02:
                  pz_locConn_Data->z_OpenParam.pdp_type = PDSM_ATL_PDP_IPV4V6;
                  break;
               case eQMI_LOC_APN_PROFILE_PDN_TYPE_PPP_V02:
                  pz_locConn_Data->z_OpenParam.pdp_type = PDSM_ATL_PDP_PPP;
                  break;
               default:
                  LOC_MSG_ERROR( "locConn_InformLocServerConnStatus, invalid qmiLocServerPDNEnumT=%d",
                                 pz_InformLocServerConnStatusReq->apnProfile.pdnType,0,0);
                  return LOC_API_INVALID_PARAMETER;
            }
            memscpy (&(pz_locConn_Data->z_OpenParam.apn_name),
                     sizeof(pz_locConn_Data->z_OpenParam.apn_name),
                     &(pz_InformLocServerConnStatusReq->apnProfile.apnName),
                     sizeof(pz_InformLocServerConnStatusReq->apnProfile.apnName));
         }
         /*Copy the APN type mask returned in the ATL open params*/
         if (pz_InformLocServerConnStatusReq->apnTypeMask_valid)
         {
            pz_locConn_Data->z_OpenParam.apn_type = pz_InformLocServerConnStatusReq->apnTypeMask;
            LOC_MSG_MED("locConn_InformLocServerConnStatus Apn_type requested = %d,Apn_type provided = %d\n", 
                          pz_locConn_Data->z_OpenParam.apn_type,pz_InformLocServerConnStatusReq->apnTypeMask, 0);
         }
         else
         {
            pz_locConn_Data->z_OpenParam.apn_type = PDSM_ATL_APN_TYPE_MASK_UNSPECIFIED;
            LOC_MSG_MED("locConn_InformLocServerConnStatus setting APN type invalid",0,0, 0);                          
         }
      }
      else
      {
         q_event = PDSM_ATL_EVENT_OPEN_FAIL;
      }
      LOC_MSG_MED ("locConn_InformLocServerConnStatus, pdsm_atl_event_type = 0x%x, l_SessionHandle = %d \n",
                     q_event, pz_locConn_Data->l_SessionHandle, 0);
      MSG_SPRINTF_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED,"apn_name = %s", pz_locConn_Data->z_OpenParam.apn_name);
#ifndef FEATURE_GNSS_SA
      // Inform lcoation engine on the modem about the atl open status
      v_Status = pdsm_atl_post_event (PDSM_ATL_TYPE_IP,
                                    pz_locConn_Data->l_SessionHandle,
                                    q_event,
                                    pz_locConn_Data->z_OpenParam);
      if (v_Status == TRUE)
      {
         q_Ret = LOC_API_SUCCESS;
      }
#else
      q_Ret = LOC_API_SUCCESS;
#endif
   }
   else if(eQMI_LOC_SERVER_REQUEST_CLOSE_V02 == pz_InformLocServerConnStatusReq->requestType)
   {
      if(eQMI_LOC_SERVER_REQ_STATUS_SUCCESS_V02 == pz_InformLocServerConnStatusReq->statusType)
      {
         q_event = PDSM_ATL_EVENT_CLOSE_SUCCESS;
      }
      else
      {
         q_event = PDSM_ATL_EVENT_CLOSE_FAIL;
      }
#ifndef FEATURE_GNSS_SA
      // Inform lcoation engine on the modem about the atl close status
      v_Status = pdsm_atl_post_event (PDSM_ATL_TYPE_IP,
                                    pz_locConn_Data->l_SessionHandle,
                                    q_event,
                                    pz_locConn_Data->z_OpenParam);
      if (v_Status == TRUE)
      {
         q_Ret = LOC_API_SUCCESS;
      }
#else
      q_Ret = LOC_API_SUCCESS;
#endif
   }
   else
   {
      LOC_MSG_ERROR( "locConn_InformLocServerConnStatus, invalid requestType",
                     pz_InformLocServerConnStatusReq->requestType,0,0);
      return LOC_API_INVALID_PARAMETER;
   }

   locMW_SendQmiIndicationProxy(l_ClientHandle,
                              QMI_LOC_INFORM_LOCATION_SERVER_CONN_STATUS_IND_V02,
                              NULL, q_Ret);

   return q_Ret;
}

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC request from client that belongs to connection module

@param[in] l_ClientHandle  : Loc API client handle
@param[in] q_QmiLocMsgId   : QMI_LOC Request Message ID
@param[in] p_QmiLocMsgData : QMI_LOC Request Message Data
@param[in] q_QmiLocMsgLen  : QMI_LOC Request Message Data Length

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locConn_ProcessQmiRequest
(
  loc_client_handle_type l_ClientHandle,
  uint32                 q_QmiLocMsgId,
  const void*            p_QmiLocMsgData,
  uint32                 q_QmiLocMsgLen
)
{
   locPa_ModuleDataType*  pz_LocPaData = &(loc_middleware_data.z_locPa_Data);
   uint8*                 pu_DestBuffer = NULL;
   uint32                 q_RetVal = LOC_API_GENERAL_FAILURE;

   /* Function arguments sanity check */
   if((NULL == p_QmiLocMsgData) || (LOC_CLIENT_HANDLE_INVALID == l_ClientHandle))
   {
      LOC_MSG_ERROR("locConn_ProcessQmiRequest: NULL pointer or invalid l_ClientHandle %d",
                     l_ClientHandle,0,0);
      return FALSE;
   }

   switch (q_QmiLocMsgId)
   {
      case QMI_LOC_INFORM_LOCATION_SERVER_CONN_STATUS_REQ_V02:
      {
         q_RetVal = locConn_InformLocServerConnStatus(l_ClientHandle,
                     (qmiLocInformLocationServerConnStatusReqMsgT_v02*)p_QmiLocMsgData);
         break;
      }

      default:
      {
         LOC_MSG_ERROR("locConn_ProcessQmiRequest: unsupported QMI_LOC 0x00%x", q_QmiLocMsgId, 0, 0);
         break;
      }
   }

   if( LOC_API_SUCCESS == q_RetVal)
   {
      return TRUE;
   }
   else
   {
      LOC_MSG_ERROR("locConn_ProcessQmiRequest: QMI_LOC 0x%04x returns error=%d", q_QmiLocMsgId, q_RetVal, 0);
      return FALSE;
   }
}

