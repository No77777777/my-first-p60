/*============================================================================
 @file loc_ot_batching.c

 loc MW Outdoor Trip Batching module implementation

 GENERAL DESCRIPTION

 This file contains the loc middleware Outdoor Trip Batching module implementation.
 
 Copyright (c) 2017 Qualcomm Technologies, Inc. All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.

 =============================================================================*/

/*============================================================================

 EDIT HISTORY FOR FILE

 This section contains comments describing changes made to the module.

 $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_ot_batching.c#1 $
 $DateTime: 2019/10/24 01:30:36 $
 $Author: pwbldsvc $

 when       who     what, where, why
 --------   ---     ----------------------------------------------------------
 04/30/17   yh     Initial version

=============================================================================*/

#include "customer.h"
#include "comdef.h"
#include "loc_api_internal.h"

#include "loc_ot_batching.h"
#include "loc_qmi_shim.h"
#include "location_common.h"
#include "mgp_api.h"
#include "loc_nv_efs.h"

#define LOC_OTB_DEALY_BATCHING_SIZE    (5) /* Hold the position between buffer full and buffer read */

#define LOC_OTB_INTERVAL_OFFSET        (100) /* Batch the position when interval >= TBF-offset */

static char u_string[200];  /* debug string */

/* -----------------------------------------------------------------------*//**
@brief
  Query the GNSS_NV_EFS_LOC_OTB_BUFFER_THRESHOLD from NV item.

@retval            Max OTB position allowed to alloc
*//* ------------------------------------------------------------------------*/
static uint32 locOtb_BufferThreshold(void)
{
   static uint32  q_OtbBufferThreshold = 0;

   if (0 != q_OtbBufferThreshold)
   {
      LOC_MSG_HIGH("locOtb_BufferThreshold 1 q_OtbBufferThreshold %u", 
                    q_OtbBufferThreshold, 0, 0 );
      return q_OtbBufferThreshold;
   }

   q_OtbBufferThreshold = GNSS_NV_EFS_LOC_OTB_BUFFER_THRESHOLD_DEFAULT;
   if (TRUE == locNvEfsInit())
   {
      if ( FALSE == locNvEfsRead(GNSS_NV_EFS_LOC_OTB_BUFFER_THRESHOLD,
                         (void *)&q_OtbBufferThreshold,
                          sizeof(q_OtbBufferThreshold)))
      {
         LOC_MSG_HIGH("locOtb_BufferThreshold could not read nv for q_OtbBufferThreshold, defaulting it to %u", 
                        q_OtbBufferThreshold, 0, 0 );
      }
   }
   else
   {
      LOC_MSG_HIGH("locOtb_BufferThreshold locNvEfsInit failed, defaulting it to %u", 
                     q_OtbBufferThreshold, 0, 0 );
   }

   LOC_MSG_HIGH("locOtb_BufferThreshold 2 q_OtbBufferThreshold %u", 
                 q_OtbBufferThreshold, 0, 0 );
   return q_OtbBufferThreshold;
}


/* -----------------------------------------------------------------------*//**
@brief
  Function to allocate the buffer memory based on availability and control
   points request.
  
@param[in]   pz_Client             Client Info Handle
@param[out]  p_GetBatchSizeInd     Get Batch Indication message to control point
@param[in]   q_RequestBatchSize    Batch size requested by control point 
@param[in]   q_ReqTransactionId    Transaction Id in request message by control point 

@retval    TRUE           Buffer memory alloc success
@retval    FALSE          Buffer memory alloc failed
                                  
*//* ------------------------------------------------------------------------*/
boolean locOtb_HandleGetBatchSize
(
   loc_client_info_s_type*                pz_LocClient,
   qmiLocGetBatchSizeIndMsgT_v02*         pz_GetBatchSizeInd,
   uint32                                 q_RequestBatchSize,
   uint32                                 q_ReqTransactionId
)
{
   boolean v_RetVal = FALSE;
   qmiLocBatchedReportStructT_v02** pp_OtbReportArray = NULL;
   uint32 i;
   uint32  q_OtbBufferThreshold = locOtb_BufferThreshold();

   if (NULL == pz_GetBatchSizeInd) 
   {
      LOC_MSG_ERROR("locOtb_HandleGetBatchSize: NULL pz_GetBatchSizeInd",
                     0, 0, 0);
      return v_RetVal;
   }

   pz_GetBatchSizeInd->transactionId = q_ReqTransactionId;
   pz_GetBatchSizeInd->status = eQMI_LOC_GENERAL_FAILURE_V02;
   if (NULL == pz_LocClient)
   {
      LOC_MSG_ERROR("locOtb_HandleGetBatchSize: NULL pz_Client = %p",
                     pz_LocClient, 0, 0);
      return v_RetVal;
   }

   if (NULL != pz_LocClient->z_OtbInfo.pp_OtbReportArray ||
       TRUE == pz_LocClient->z_OtbInfo.v_OtbClient)
   {
      /* Buffer already allocated case */
      LOC_MSG_ERROR("locOtb_HandleGetBatchSize: pp_OtbReportArray = %p, v_OtbClient = %d",
                     pz_LocClient->z_OtbInfo.pp_OtbReportArray,
                     pz_LocClient->z_OtbInfo.v_OtbClient, 0);
      return v_RetVal;
   }

   if (0 == q_RequestBatchSize)
   {
      LOC_MSG_ERROR("locOtb_HandleGetBatchSize: q_RequestBatchSize = %u",
                     q_RequestBatchSize, 0, 0);
      pz_GetBatchSizeInd->status = eQMI_LOC_INVALID_PARAMETER_V02;
      return v_RetVal;
   }

   if (q_OtbBufferThreshold < q_RequestBatchSize)
   {
      LOC_MSG_ERROR("locOtb_HandleGetBatchSize: q_RequestBatchSize = %u > %d threshold",
                     q_RequestBatchSize, q_OtbBufferThreshold, 0);
      pz_GetBatchSizeInd->status = eQMI_LOC_INVALID_PARAMETER_V02;
      pz_GetBatchSizeInd->batchSize = q_OtbBufferThreshold;
      return v_RetVal;
   }

   pp_OtbReportArray = (qmiLocBatchedReportStructT_v02**)os_MemAlloc(
      (LOC_OTB_DEALY_BATCHING_SIZE + q_RequestBatchSize) * sizeof(void*), OS_MEM_SCOPE_TASK);
   if (NULL == pp_OtbReportArray)
   {
      LOC_MSG_ERROR("locOtb_HandleGetBatchSize: alloc pp_OtbReportArray failed", 0, 0, 0);
      pz_GetBatchSizeInd->status = eQMI_LOC_INSUFFICIENT_MEMORY_V02;
      return v_RetVal;
   }
   memset(pp_OtbReportArray, 0, sizeof((LOC_OTB_DEALY_BATCHING_SIZE + q_RequestBatchSize) * sizeof(void*)));

   for (i=0; i<(LOC_OTB_DEALY_BATCHING_SIZE + q_RequestBatchSize); i++)
   {
      pp_OtbReportArray[i] = (qmiLocBatchedReportStructT_v02*)os_MemAlloc(
                              sizeof(qmiLocBatchedReportStructT_v02), OS_MEM_SCOPE_TASK);
      if( NULL == pp_OtbReportArray[i])
      {
         break;
      }
      memset(pp_OtbReportArray[i],0,sizeof(qmiLocBatchedReportStructT_v02));
   }

   if (i == (LOC_OTB_DEALY_BATCHING_SIZE + q_RequestBatchSize))
   {
      LOC_MSG_MED("locOtb_HandleGetBatchSize: alloc %u reports for client Handle %d",
         i, pz_LocClient->client_handle, 0);
      v_RetVal = TRUE;
      memset((void *)&(pz_LocClient->z_OtbInfo), 0, sizeof(pz_LocClient->z_OtbInfo));
      pz_LocClient->z_OtbInfo.pp_OtbReportArray = pp_OtbReportArray;
      pz_LocClient->z_OtbInfo.q_ReportArraySize = q_RequestBatchSize;
      pz_LocClient->z_OtbInfo.q_AllocatedReportSize = (LOC_OTB_DEALY_BATCHING_SIZE + q_RequestBatchSize);
      pz_LocClient->z_OtbInfo.q_Head = 0;
      pz_LocClient->z_OtbInfo.q_Tail = 1;  /* FIFO algorithm */
      pz_LocClient->z_OtbInfo.v_OtbClient = TRUE;

      pz_GetBatchSizeInd->status = eQMI_LOC_SUCCESS_V02;
      pz_GetBatchSizeInd->batchSize = q_RequestBatchSize;
   }
   else
   {
      qmiLocBatchedReportStructT_v02 *temp;
      LOC_MSG_ERROR("locOtb_HandleGetBatchSize: alloc %u reports less than required %u for client Handle %d",
                     i, LOC_OTB_DEALY_BATCHING_SIZE + q_RequestBatchSize, pz_LocClient->client_handle);
      /* clean up the memory */
      q_RequestBatchSize = i;
      for(i=0; i<(LOC_OTB_DEALY_BATCHING_SIZE + q_RequestBatchSize); i++)
      {
         temp = pp_OtbReportArray[i];
         if(NULL != temp)
            os_MemFree((void **)&temp);
      }
      os_MemFree((void **)&pp_OtbReportArray);
      pz_GetBatchSizeInd->status = eQMI_LOC_INSUFFICIENT_MEMORY_V02;
   }
   return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
  Function to free the allocated buffer memory and reset the client's
  outdoor trip batching related fields. 

@param[in]   pz_LocClient            Client Info Handle 

@retval    TRUE           Buffer memory release success
@retval    FALSE          Buffer memory release failed
                                  
*//* ------------------------------------------------------------------------*/
boolean locOtb_FreeAndReset
(
   loc_client_info_s_type* pz_LocClient
)
{
   uint32 i=0;
   if (NULL == pz_LocClient)
   {
      LOC_MSG_MED("locOtb_FreeAndReset NULL pz_LocClient", 0, 0, 0);
      return FALSE;
   }

   /* free the buffer */
   if (NULL != pz_LocClient->z_OtbInfo.pp_OtbReportArray)
   {
      while (NULL != (pz_LocClient->z_OtbInfo.pp_OtbReportArray[i])
         && (i < pz_LocClient->z_OtbInfo.q_AllocatedReportSize))
      {
         os_MemFree((void **)&(pz_LocClient->z_OtbInfo.pp_OtbReportArray[i]));
         i++;
      }
      os_MemFree((void **)&(pz_LocClient->z_OtbInfo.pp_OtbReportArray));
      LOC_MSG_MED("locOtb_FreeAndReset free otb batching array size %u", i, 0, 0);
   }

   /* reset all fields */
   memset(&(pz_LocClient->z_OtbInfo), 0, sizeof(pz_LocClient->z_OtbInfo));

   return TRUE;
}

/* -----------------------------------------------------------------------*//**
@brief
  Function to process QMI_LOC_QUERY_OTB_ACCUMULATED_DISTANCE_REQ

  @param[in] pz_LocClient  : Loc Client Info

  @retval    TRUE   Called successfully
  @retval    FALSE  Called unsuccessfully
*//* ------------------------------------------------------------------------*/
boolean locOtb_QueryOTBAccumulatedDistance
(
   loc_client_handle_type            l_ClientHandle
)
{
   locQmiShimIndInfoStructT         z_ShimIndInfo = {0};
   qmiLocQueryOTBAccumulatedDistanceIndMsgT_v02   z_QueryOTBAccumulatedDistanceInd = {0};
   loc_client_info_s_type* pz_LocClient = locClnt_FindHandle(l_ClientHandle);

   if (NULL == pz_LocClient)
   {
      LOC_MSG_ERROR(" locOtb_QueryOTBAccumulatedDistance: can't find ClientHandle %d",
                     l_ClientHandle, 0, 0);
      return FALSE;
   }

   if (pz_LocClient->z_OtbInfo.v_OtbClient)
   {
      z_QueryOTBAccumulatedDistanceInd.status = eQMI_LOC_SUCCESS_V02;
      z_QueryOTBAccumulatedDistanceInd.accumulatedDistance = (uint32)(pz_LocClient->z_OtbInfo.d_AccumulatedDistance);
      z_QueryOTBAccumulatedDistanceInd.batchedPosition = pz_LocClient->z_OtbInfo.q_TotalBatchedPosition;
   }
   else
   {
      LOC_MSG_ERROR(" locOtb_QueryOTBAccumulatedDistance: client Handle %d is not an OTB client",
                     pz_LocClient->client_handle, 0, 0);
      z_QueryOTBAccumulatedDistanceInd.status = eQMI_LOC_GENERAL_FAILURE_V02;
   }


   z_ShimIndInfo.p_Msg = &z_QueryOTBAccumulatedDistanceInd;
   z_ShimIndInfo.q_MsgLen = sizeof(z_QueryOTBAccumulatedDistanceInd);
   z_ShimIndInfo.q_Id = QMI_LOC_QUERY_OTB_ACCUMULATED_DISTANCE_IND_V02;

   /* send indication */
   return locQmiShimSendInd(pz_LocClient->client_handle, &z_ShimIndInfo);
}

static boolean locOtb_Push
(
   locClnt_OtbInfoType*                pz_OtbInfo,
   qmiLocBatchedReportStructT_v02*     pz_BatchedPosition
)
{
   if ((NULL == pz_OtbInfo) || (NULL == pz_BatchedPosition))
   {
      LOC_MSG_ERROR(" locOtb_Push: Null pointers", 0, 0, 0);
      return FALSE;
   }

   pz_OtbInfo->q_TotalBatchedPosition++;
   if (pz_OtbInfo->q_Head == pz_OtbInfo->q_Tail)
   {
      /* 1 array[tail++%size] = position */
      *(pz_OtbInfo->pp_OtbReportArray[pz_OtbInfo->q_Tail++ % pz_OtbInfo->q_AllocatedReportSize]) = *pz_BatchedPosition;
      /* 2 tail%=size */
      pz_OtbInfo->q_Tail %= pz_OtbInfo->q_AllocatedReportSize;
      /* 3 head++, head%=size */
      pz_OtbInfo->q_Head++;
      pz_OtbInfo->q_Head %= pz_OtbInfo->q_AllocatedReportSize;
      LOC_MSG_HIGH(" locOtb_Push: buffer full q_Head=%u q_Tail=%u q_TotalOccupied=%u",
         pz_OtbInfo->q_Head, pz_OtbInfo->q_Tail, pz_OtbInfo->q_TotalOccupied);
      return TRUE;
   }
   else
   {
      /* 1 array[tail++%size] = position */
      *(pz_OtbInfo->pp_OtbReportArray[pz_OtbInfo->q_Tail++ % pz_OtbInfo->q_AllocatedReportSize]) = *pz_BatchedPosition;
      /* 2 tail%=size */
      pz_OtbInfo->q_Tail %= pz_OtbInfo->q_AllocatedReportSize;
      /* 3 occupied++ */
      pz_OtbInfo->q_TotalOccupied++;
      LOC_MSG_HIGH(" locOtb_Push: q_Head=%u q_Tail=%u q_TotalOccupied=%u",
         pz_OtbInfo->q_Head, pz_OtbInfo->q_Tail, pz_OtbInfo->q_TotalOccupied);
      return TRUE;
   }
}

static boolean locOtb_Pull
(
   locClnt_OtbInfoType*                pz_OtbInfo,
   qmiLocBatchedReportStructT_v02*     pz_BatchedPosition
)
{
   if ((NULL == pz_OtbInfo) || (NULL == pz_BatchedPosition))
   {
      LOC_MSG_ERROR(" locOtb_Pull: Null pointers", 0, 0, 0);
      return FALSE;
   }

   /* (head - tail + size) % size == size -1 */
   if (((pz_OtbInfo->q_Head - pz_OtbInfo->q_Tail + pz_OtbInfo->q_AllocatedReportSize) % pz_OtbInfo->q_AllocatedReportSize)
      == (pz_OtbInfo->q_AllocatedReportSize-1))
   {
      LOC_MSG_ERROR(" locOtb_Pull: FIFO is empty q_Head=%u q_Tail=%u",
         pz_OtbInfo->q_Head, pz_OtbInfo->q_Tail, 0);
      return FALSE;
   }
   else
   {
      /* 1 position = array[++head % size] */
      *pz_BatchedPosition = *(pz_OtbInfo->pp_OtbReportArray[++pz_OtbInfo->q_Head % pz_OtbInfo->q_AllocatedReportSize]);
      /* 2 head%=size */
      pz_OtbInfo->q_Head %= pz_OtbInfo->q_AllocatedReportSize;
      /* 3 occupied-- */
      pz_OtbInfo->q_TotalOccupied--;
      LOC_MSG_HIGH(" locOtb_Pull: q_Head=%u q_Tail=%u q_TotalOccupied=%u",
         pz_OtbInfo->q_Head, pz_OtbInfo->q_Tail, pz_OtbInfo->q_TotalOccupied);
      return TRUE;
   }
}

/* -----------------------------------------------------------------------*//**
@brief
  Function to send QMI_LOC_EVENT_BATCH_FULL_NOTIFICATION_IND to the client
  
@param[in]   pz_LocClient   Loc Client Info Handle

@retval    TRUE           event reporting success
@retval    FALSE          event reporting fail
*//* ------------------------------------------------------------------------*/
static boolean locOtb_ReportBatchFullInd
(
   loc_client_info_s_type    *pz_LocClient
)
{
   qmiLocEventBatchFullIndMsgT_v02  z_BatchFullInd = {0};
   locQmiShimIndInfoStructT         z_ShimIndInfo = {0};

   if (NULL == pz_LocClient)
   {
      LOC_MSG_ERROR(" locOtb_ReportBatchFullInd: NULL pz_LocClient",
                     0, 0, 0);
      return FALSE;
   }

   /* Send report only if the client registered */
   if (0 == (pz_LocClient->t_EventRegMask & LOC_EVENT_MASK_BATCH_FULL_NOTIFICATION))
   {
      LOC_MSG_MED("Client did not register for BATCH_FULL_NOTIFICATION, registered Loc t_EventRegMask = 0x%8x",
                  pz_LocClient->t_EventRegMask, 0, 0);
      return FALSE; 
   }

   z_BatchFullInd.batchCount = pz_LocClient->z_OtbInfo.q_TotalOccupied; /* size of batch to be notified */
   z_BatchFullInd.batchType_valid = TRUE;
   z_BatchFullInd.batchType = eQMI_LOC_OUTDOOR_TRIP_BATCHING_V02;
   z_BatchFullInd.accumulatedDistance_valid = TRUE;
   z_BatchFullInd.accumulatedDistance = (uint32)(pz_LocClient->z_OtbInfo.d_AccumulatedDistance);
   z_BatchFullInd.batchedPosition_valid = TRUE;
   z_BatchFullInd.batchedPosition = pz_LocClient->z_OtbInfo.q_TotalBatchedPosition;

   z_ShimIndInfo.p_Msg = &z_BatchFullInd;
   z_ShimIndInfo.q_MsgLen = sizeof(z_BatchFullInd);
   z_ShimIndInfo.q_Id = QMI_LOC_EVENT_BATCH_FULL_NOTIFICATION_IND_V02;

   LOC_MSG_LOW("locOtb_ReportBatchFullInd Count:%u accumulatedDistance %u",
                z_BatchFullInd.batchCount,
                z_BatchFullInd.accumulatedDistance, 0);

   return locQmiShimSendInd(pz_LocClient->client_handle,
                           &z_ShimIndInfo) ;
}

/* -----------------------------------------------------------------------*//**
@brief
  Function to handle position report from loc AON. The function pushes the 
  batched fix into the FIFO buffer and notifies the control point if tripDistance 
  is met or buffer is full
  
@param[in]   l_ClientHandle        Loc Client Handle 
@param[in]   pz_BatchedPosition    Pointer to the Batched position report

@retval    TRUE           push to FIFO buffer is success
@retval    FALSE          push to FIFO buffer is fail
                                  
*//* ------------------------------------------------------------------------*/
boolean locOtb_BatchPosition
(
   loc_client_handle_type            l_ClientHandle,
   qmiLocBatchedReportStructT_v02*   pz_BatchedPosition
)
{
   boolean                    v_RetVal = FALSE;
   loc_client_info_s_type*    pz_LocClient = locClnt_FindHandle(l_ClientHandle);
   double                     d_DistanceBetPosition = 0.0;
   boolean                    v_Push = FALSE;
   qmiLocBatchedReportStructT_v02*   pz_LastPosition = NULL;

   if (NULL == pz_LocClient)
   {
      LOC_MSG_ERROR(" locOtb_BatchPosition: can't find locClient %d",l_ClientHandle, 0, 0);
      return v_RetVal;
   }

   if (NULL == pz_BatchedPosition)
   {
      LOC_MSG_ERROR(" locOtb_BatchPosition: NULL pz_BatchedPosition", 0, 0, 0);
      return v_RetVal;
   }

   if (pz_LocClient->z_OtbInfo.t_LastFixTimestampUtc >= pz_BatchedPosition->timestampUtc)
   {
      /* Ignore fix with time stamp same as the last fix received */
      LOC_MSG_HIGH("locOtb_BatchPosition: Ignore fix with same timestamp as last fix reported", 0, 0, 0);
      return v_RetVal;
   }

   if (pz_LocClient->z_OtbInfo.v_BatchAllPos)
   {
      v_Push = TRUE;
   }
   else if ((pz_BatchedPosition->timestampUtc - pz_LocClient->z_OtbInfo.t_LastFixTimestampUtc) >= 
      (pz_LocClient->z_OtbInfo.q_MinTimeInterval - LOC_OTB_INTERVAL_OFFSET))
   {
      v_Push = TRUE;
   }

   if (TRUE == v_Push)
   {
      /* Push to FIFO */
      v_RetVal = locOtb_Push(&(pz_LocClient->z_OtbInfo),pz_BatchedPosition);
      if (TRUE == v_RetVal)
      {
         /* update the fix id in the position report report */
         pz_BatchedPosition->fixId = pz_LocClient->z_OtbInfo.q_FixId;
         /* increment the fix id counter */
         pz_LocClient->z_OtbInfo.q_FixId++;
      }
   }
   else
   {
      LOC_MSG_HIGH("locOtb_BatchPosition: dropping the position timestampUtc %u last timestampUtc",
                     pz_BatchedPosition->timestampUtc,
                     pz_LocClient->z_OtbInfo.t_LastFixTimestampUtc, 0);
      return v_RetVal;
   }

   if (TRUE == v_RetVal)
   {
      uint32   q_Index;
      uint32   q_Size = pz_LocClient->z_OtbInfo.q_AllocatedReportSize;
      pz_LocClient->z_OtbInfo.t_LastFixTimestampUtc = pz_BatchedPosition->timestampUtc;

      /* Update the accumulated distance */
      if (pz_LocClient->z_OtbInfo.q_TotalOccupied >=2)
      {
         q_Index = (pz_LocClient->z_OtbInfo.q_Tail-2 + q_Size) % q_Size;
         pz_LastPosition = pz_LocClient->z_OtbInfo.pp_OtbReportArray[q_Index];
         d_DistanceBetPosition = loc_CalcDistBetLatLongHaversine(
                                   pz_BatchedPosition->latitude  * LAT_LONG_SCALE,
                                   pz_BatchedPosition->longitude * LAT_LONG_SCALE,
                                   pz_LastPosition->latitude  * LAT_LONG_SCALE, 
                                   pz_LastPosition->longitude * LAT_LONG_SCALE);
         pz_LocClient->z_OtbInfo.d_AccumulatedDistance += d_DistanceBetPosition;
         (void) snprintf(u_string, sizeof(u_string),
                     "locOtb_BatchPosition: distance = %f, accumulated distance = %f",
                     d_DistanceBetPosition,
                     pz_LocClient->z_OtbInfo.d_AccumulatedDistance);
         MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", u_string);
         LOC_MSG_MED("locOtb_BatchPosition: total occupied %u",
                     pz_LocClient->z_OtbInfo.q_TotalOccupied, 0, 0);
      }
   }

   /* check for distance met or buffer full */
   if (((pz_LocClient->z_OtbInfo.q_TotalOccupied == pz_LocClient->z_OtbInfo.q_ReportArraySize) &&
      (FALSE == pz_LocClient->z_OtbInfo.v_BufferFullReported)) ||
      ((pz_LocClient->z_OtbInfo.d_AccumulatedDistance >= pz_LocClient->z_OtbInfo.q_BatchDistance)&&
      (FALSE == pz_LocClient->z_OtbInfo.v_BatchDistanceReported)))
   {
      v_RetVal = locOtb_ReportBatchFullInd(pz_LocClient);
      if(TRUE == v_RetVal)
      {
         if(pz_LocClient->z_OtbInfo.q_TotalOccupied == pz_LocClient->z_OtbInfo.q_ReportArraySize)
         {
            pz_LocClient->z_OtbInfo.v_BufferFullReported = TRUE;
         }

         if(pz_LocClient->z_OtbInfo.d_AccumulatedDistance >= pz_LocClient->z_OtbInfo.q_BatchDistance)
         {
            pz_LocClient->z_OtbInfo.v_BatchDistanceReported = TRUE;
         }
      }
   }

   return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
  Function to handle read operation from the fifo buffer. 
  
@param[in]   pz_LocClient          Loc Client Info Handle 
@param[in]   pz_ReadFromBatchReq   QMI_LOC request message
@param[out]  pz_ReadFromBatchInd   Indication with batched position reports


@retval    TRUE           number of reads field is valid
@retval    FALSE          number of reads field is not valid
                                  
*//* ------------------------------------------------------------------------*/
boolean locOtb_BtchingRead
(
   loc_client_info_s_type                    *pz_LocClient,
   const  qmiLocReadFromBatchReqMsgT_v02     *pz_ReadFromBatchReq,
   qmiLocReadFromBatchIndMsgT_v02            *pz_ReadFromBatchInd)
{
   boolean v_RetVal = FALSE;
   uint32 i=0;

   if ((NULL == pz_LocClient) ||
      (NULL == pz_ReadFromBatchReq) ||
      (NULL == pz_ReadFromBatchInd))
   {
      LOC_MSG_MED("locOtb_BtchingRead: NULL pointer", 0, 0, 0);
      return FALSE;
   }

   /* pull the entries from the batching buffer */
   for (i=0; i< pz_ReadFromBatchReq->numberOfEntries; i++)
   {
      v_RetVal = locOtb_Pull(&(pz_LocClient->z_OtbInfo), &(pz_ReadFromBatchInd->batchedReportList[i]));
      if (FALSE == v_RetVal)
         break;
   }

   if (i > 0)
   {
      pz_ReadFromBatchInd->numberOfEntries_valid = TRUE;
      pz_ReadFromBatchInd->numberOfEntries = i;
      pz_ReadFromBatchInd->batchedReportList_valid = TRUE;
      pz_ReadFromBatchInd->batchedReportList_len = i;
      v_RetVal = TRUE;
   }

   LOC_MSG_MED("locOtb_BtchingRead: number of entries to read %u returned %u total occupied %u",
      pz_ReadFromBatchReq->numberOfEntries, i, pz_LocClient->z_OtbInfo.q_TotalOccupied);
   pz_LocClient->z_OtbInfo.v_BufferFullReported = FALSE;

   return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
  Function to recalculate the accumulated distance

  @param[in] l_ClientHandle  : Loc Client Handle

  @retval    TRUE   Called successfully
  @retval    FALSE  Called unsuccessfully
*//* ------------------------------------------------------------------------*/
boolean locOtb_RecalAccumulatedDistance
(
   loc_client_info_s_type *pz_LocClient
)
{
   locClnt_OtbInfoType    *pz_OtbInfo = NULL;
   uint32                  q_Head,q_Tail,q_Size,q_TotalOccupied,q_Index;
   qmiLocBatchedReportStructT_v02   *pz_PositionOne, *pz_PositionTwo;
   double                  d_DistanceBetPosition = 0.0;

   if (NULL == pz_LocClient)
   {
      LOC_MSG_MED("locOtb_RecalAccumulatedDistance: NULL pz_LocClient", 0, 0, 0);
      return FALSE;
   }

   pz_OtbInfo = &(pz_LocClient->z_OtbInfo);
   q_Head = pz_OtbInfo->q_Head;
   q_Tail = pz_OtbInfo->q_Tail;
   q_Size = pz_OtbInfo->q_AllocatedReportSize;
   q_TotalOccupied = pz_OtbInfo->q_TotalOccupied;

   if (q_TotalOccupied <= 1)
   {
      pz_OtbInfo->d_AccumulatedDistance = 0.0;
      LOC_MSG_MED("locOtb_RecalAccumulatedDistance: q_Head %u, q_Tail %u, q_TotalOccupied %u",
                  q_Head, q_Tail, q_TotalOccupied);
      return TRUE;
   }

   pz_OtbInfo->d_AccumulatedDistance = 0.0;
   while (((q_Head - q_Tail + q_Size) % q_Size) != (q_Size-2))
   {
      q_Index = ++q_Head % q_Size;
      pz_PositionOne = pz_OtbInfo->pp_OtbReportArray[q_Index];
      pz_PositionTwo = pz_OtbInfo->pp_OtbReportArray[(q_Index+1)% q_Size];
      q_Head %= q_Size;
      d_DistanceBetPosition = loc_CalcDistBetLatLongHaversine(
                                 pz_PositionTwo->latitude  * LAT_LONG_SCALE,
                                 pz_PositionTwo->longitude * LAT_LONG_SCALE,
                                 pz_PositionOne->latitude  * LAT_LONG_SCALE, 
                                 pz_PositionOne->longitude * LAT_LONG_SCALE);
      pz_OtbInfo->d_AccumulatedDistance += d_DistanceBetPosition;
      LOC_MSG_MED("locOtb_RecalAccumulatedDistance: q_Head = %u, q_Tail = %u q_Index = %u",
                  q_Head, q_Tail, q_Index);
      (void) snprintf(u_string, sizeof(u_string), 
                     "locOtb_RecalAccumulatedDistance: distance = %f, accumulated distance = %f",
                     d_DistanceBetPosition,
                     pz_LocClient->z_OtbInfo.d_AccumulatedDistance);
      MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", u_string);
   }
   return TRUE;
}