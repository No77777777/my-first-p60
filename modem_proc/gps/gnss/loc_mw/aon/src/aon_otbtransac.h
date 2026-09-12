#ifndef AON_OTBTRANSAC_H
#define AON_OTBTRANSAC_H
/*============================================================================
   @file aon_otbtransac.h

   @brief
      This file contains API definitions to manage the Outdoor Trip Batching
      use case of AlwaysOn Service

   Copyright (c) 2017 Qualcomm Technologies, Inc. All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.

============================================================================*/
/*============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/aon/src/aon_otbtransac.h#1 $ 
  $DateTime: 2019/10/24 01:30:36 $ 
  $Author: pwbldsvc $ 



when        who  what, where, why
----------  ---  -----------------------------------------------------------
04/30/17    yh    Initial creation of file.
=============================================================================*/
#include "aon_api.h"
#include "aon_transac.h"
#include "loc_api_internal.h"

class AonOtbTransac :public AonTransac{

public:
   AonOtbTransac(uint32 reqId,
               uint32 maxLatency, uint32 fixTimeout, 
               AonDatabase* aonDbHdl, AonGmProxy *aonGmProxy,
               aonLocationCallback locCb,aonStatusCallback statusCb);

   ~AonOtbTransac(){}
   /* 
   @brief Retrieve the equivalent geofence config for the AON transaction
   @param[out] gfConfig GF Config object that needs to be populated
   @return TRUE or FALSE
   */
   boolean getEqGeofenceConfig(AonGeofenceConfig &gfConfig);
   /* 
   @brief Retrieve the equivalent background engine client config for AON
   @param[in] gmConfig Background engine client level config object
   @return TRUE or FALSE
   */
   boolean getEqGmClientConfig(AonGmClientConfig &gmConfig);
   /* 
   @brief Breach response handler fn invoked by gmproxy 
   @param[in] gfId Geofence Id
   @param[in] breachType Type of breatch
   @param[in] location  Lcoation object instance adssocited witht his breach
   @return TRUE or FALSE
   */
   boolean breachRspHandler(uint32 gfId, aonGMBreachType breachType,
                           locEngPositionReportMsgT_v01 &location);
   /*
   @brief Help function to use GNSS_DELETE
   @param[in] void
   @return size of the object
   */
   size_t GetSize() const { return sizeof(*this); }

private:
   uint32 q_MinDistance;  // in meters
   uint32 q_MaxLatency;   // in ms
   uint32 q_FixTimeout;   // in ms
};

#endif /*AON_OTBTRANSAC_H*/

