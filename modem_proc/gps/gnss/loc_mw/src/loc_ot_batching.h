#ifndef LOC_OT_BATCHING_H
#define LOC_OT_BATCHING_H
/*============================================================================
 @file loc_ot_batching.h

 loc MW Outdoor Trip Batching module

 GENERAL DESCRIPTION

 This file defines the Outdoor Trip Batching module in loc middleware.

 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2017 Qualcomm Technologies, Inc. All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.

 =============================================================================*/

/*============================================================================

 EDIT HISTORY FOR FILE

 This section contains comments describing changes made to the module.

 $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_ot_batching.h#1 $
 $DateTime: 2019/10/24 01:30:36 $
 $Author: pwbldsvc $

 when       who     what, where, why
 --------   ---     ----------------------------------------------------------
 04/30/17    yh     Initial version

 =============================================================================*/

#include "location_service_v02.h"
#include "loc_client.h"

/* -----------------------------------------------------------------------*//**
@brief
  Function to free the allocated buffer memory and reset the client's
  outdoor trip batching related fields. 

@param[in]   pz_LocClient            Loc Client Info

@retval    TRUE           Buffer memory release success
@retval    FALSE          Buffer memory release failed
                                  
*//* ------------------------------------------------------------------------*/
boolean locOtb_FreeAndReset
(
   loc_client_info_s_type* pz_LocClient
);


/* -----------------------------------------------------------------------*//**
@brief
  Function to allocate the buffer memory based on availability and control
   points request.
  
@param[in]   pz_Client             Loc Client Info
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
);

/* -----------------------------------------------------------------------*//**
@brief
  Function to process QMI_LOC_QUERY_OTB_ACCUMULATED_DISTANCE_REQ

  @param[in] l_ClientHandle  : Loc Client Info

  @retval    TRUE   Called successfully
  @retval    FALSE  Called unsuccessfully
*//* ------------------------------------------------------------------------*/
boolean locOtb_QueryOTBAccumulatedDistance
(
   loc_client_handle_type            l_ClientHandle
);

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
);

/* -----------------------------------------------------------------------*//**
@brief
  Function to handle read operation from the client.
  
@param[in]   pz_LocClient          Loc Client Info
@param[in]   pz_ReadFromBatchReq   QMI_LOC request message
@param[out]  pz_ReadFromBatchInd   Indication with batched position reports


@retval    TRUE           number of reads field is valid
@retval    FALSE          number of reads field is not valid
                                  
*//* ------------------------------------------------------------------------*/
boolean locOtb_BtchingRead
(
   loc_client_info_s_type*                   pz_LocClient,
   const  qmiLocReadFromBatchReqMsgT_v02*    pz_ReadFromBatchReq,
   qmiLocReadFromBatchIndMsgT_v02*           pz_ReadFromBatchInd
);

/* -----------------------------------------------------------------------*//**
@brief
  Function to recalculate the accumulated distance

  @param[in] l_ClientHandle  : Loc Client Handle

  @retval    TRUE   Called successfully
  @retval    FALSE  Called unsuccessfully
*//* ------------------------------------------------------------------------*/
boolean locOtb_RecalAccumulatedDistance
(
   loc_client_info_s_type* pz_LocClient
);

#endif  //LOC_OT_BATCHING_H

