/*============================================================================
 @file loc_batching.h

 loc MW Batching module

 GENERAL DESCRIPTION

 This file defines the Batching module in loc middleware.

 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
 Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 

 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
 
 Copyright (c) 2015 - 2017 Qualcomm Technologies, Inc. All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.

 =============================================================================*/

/*============================================================================

 EDIT HISTORY FOR FILE

 This section contains comments describing changes made to the module.

 $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_batching.h#1 $
 $DateTime: 2019/10/24 01:30:36 $
 $Author: pwbldsvc $

 when       who     what, where, why
 --------   ---     ----------------------------------------------------------
 06/11/15   sjk     LB 2.0 integration
 07/25/13   bnk     Initial version

 =============================================================================*/

#ifndef _LOC_BATCHING_H_
#define _LOC_BATCHING_H_

#include "aries_os_api.h"
#include "location_service_v02.h"
#include "loc_client.h"


#define LOC_BATCHING_ADDITIONAL_SLOTS     20 /* additional batching slots for internal manipulation */

#define LOC_BATCHING_DEFAULT_INTERVAL_MS (60000) /* default minInterval in ms */

#define LOC_BATCHING_GNSS_QOS_SESSION_TIMEOUT_MS (20000) /* 20 seconds */

#define LOC_BATCHING_DEFAULT_RADIUS (100) /* 100 meters */

/*
 * Batching config sent to the AON layer is cached in the loc layer using this
 * structure. This info is needed to decide which fix needs to be batched and
 * which fix needs to be ignored/overwritten etc.
 */
typedef struct
{
  uint32  batchConfigMask;
  uint32  minInterval;
  uint32  minDistance;
}locAonBatchingConfig;

/* -----------------------------------------------------------------------*//**
@brief
  Function to free the allocated buffer memory and reset the client's batching
   related fields. 
  
@param[in]   client_ptr            Client Info Handle 

@retval    TRUE           Buffer memory release success
@retval    FALSE          Buffer memory release failed
                                  
*//* ------------------------------------------------------------------------*/
boolean loc_batching_free_and_reset(loc_client_info_s_type     *client_ptr);


/* -----------------------------------------------------------------------*//**
@brief
  Function to allocate the buffer memory based on availability and control
   points request.
  
@param[in]   client_ptr            Client Info Handle 
@param[out]  p_GetBatchSizeInd     Get Batch Indication message to control point
@param[in]   client_batch_size_req Batch size requested by control point 
@param[in]   reqTransactionId      Transaction Id in request message by control point 

@retval    TRUE           Buffer memory alloc success
@retval    FALSE          Buffer memory alloc failed
                                  
*//* ------------------------------------------------------------------------*/
boolean locGetBatchSizeHandleAllocate
(
   loc_client_info_s_type         *client_ptr,
   qmiLocGetBatchSizeIndMsgT_v02  *p_GetBatchSizeInd,
   uint32                         client_batch_size_req,
   uint32                         reqTransactionId
);

/* -----------------------------------------------------------------------*//**
@brief
  Function to handle read operation from the lifo buffer. 
  
@param[in]   pz_LocClient          Loc Client Info Handle 
@param[in]   pz_ReadFromBatchReq   QMI_LOC request message
@param[out]  pz_ReadFromBatchInd   Indication with batched position reports


@retval    TRUE           number of reads field is valid
@retval    FALSE          number of reads field is not valid
                                  
*//* ------------------------------------------------------------------------*/
boolean loc_handle_batching_read
(
   loc_client_info_s_type                    *pz_LocClient,
   const  qmiLocReadFromBatchReqMsgT_v02     *pz_ReadFromBatchReq,
   qmiLocReadFromBatchIndMsgT_v02            *pz_ReadFromBatchInd
);

/* -----------------------------------------------------------------------*//**
@brief
  Function to handle position report from loc AON. The function pushes the 
  batched fix into the LIFO buffer and notifies the control point depending
  on the event registration masks.
  
@param[in]   loc_client_handle     Client Handle 
@param[in]   batching_element      Pointer to the Batched position report
@param[in]   batchCfg              Pointer to the Batch config

@retval    TRUE           push to LIFO buffer is success
@retval    FALSE          push to LIFO buffer is fail
                                  
*//* ------------------------------------------------------------------------*/
boolean loc_batching_batch_position
(
  loc_client_handle_type            loc_client_handle,
  qmiLocBatchedReportStructT_v02   *batching_element,
  locAonBatchingConfig             *batchCfg
);

#endif  //_LOC_BATCHING_H_

