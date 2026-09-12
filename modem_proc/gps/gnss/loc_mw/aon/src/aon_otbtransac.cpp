/*============================================================================
  @file aon_otbtransac.cpp

  @brief
    This file contains the implementation for the DBT transaction

   Copyright (c) 2017 Qualcomm Technologies, Inc. All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.

============================================================================*/

/*============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/aon/src/aon_otbtransac.cpp#1 $ 
  $DateTime: 2019/10/24 01:30:36 $ 
  $Author: pwbldsvc $ 



when        who  what, where, why
----------  ---  ----------------------------------------------------------- 
04/30/17    yh    Initial creation of file.


=============================================================================*/
/*****************************************************************************
 * Include Files
 * *************************************************************************/
#include <math.h>
#include "aon_internal.h"
#include "aon_otbtransac.h"
#include "aon_gmproxy.h"
#include "aon_db.h"

#define AON_OTB_DEFAULT_DISTANCE  (1) //meter

#define AON_OTB_MILlISECOND_TO_SECOND  (1000)

AonOtbTransac:: AonOtbTransac(uint32 reqId,
                  uint32 maxLatency,uint32 fixTimeout,
                  AonDatabase* aonDbHdl, AonGmProxy *aonGmProxy,
                  aonLocationCallback locCb, aonStatusCallback statusCb):
                 AonTransac(reqId, aonDbHdl, aonGmProxy, locCb, statusCb),
                 q_MaxLatency(maxLatency),q_FixTimeout(fixTimeout),
                 q_MinDistance(AON_OTB_DEFAULT_DISTANCE)
{ }

/* 
    @brief Retrieve the equivalent background engine client config for AON
    @param[in] gmConfig Background engine client level config object
    @return TRUE or FALSE
    */
boolean AonOtbTransac:: getEqGmClientConfig(AonGmClientConfig &gmConfig)
{
   gmConfig.mGnssUnavailAggresiveExit = TRUE;
   gmConfig.mGmBackOffMax = q_MaxLatency/AON_OTB_MILlISECOND_TO_SECOND;
   gmConfig.mGmBackOffMin = q_MaxLatency/AON_OTB_MILlISECOND_TO_SECOND;
   gmConfig.mSessionTimeout = q_FixTimeout/AON_OTB_MILlISECOND_TO_SECOND;
   return TRUE;
}
/* 
    @brief Retrieve the equivalent geofence config for the AON transaction
    @param[out] gfConfig GF Config object that needs to be populated
    @return TRUE or FALSE
    */
boolean AonOtbTransac::getEqGeofenceConfig(AonGeofenceConfig &gfConfig)
{
   gfConfig.mGfResponsivenessInSec = q_MaxLatency/AON_OTB_MILlISECOND_TO_SECOND;
   gfConfig.mResponsiveness = GEOFENCE_RESPONSIVENESS_CUSTOM;
  
   gfConfig.mGfBreachEventType = (sm_gm_geofence_event_type)( 
      GEOFENCE_EVENT_ENTERING_AREA |
      GEOFENCE_EVENT_LEAVING_AREA  |
      GEOFENCE_EVENT_INSIDE_AREA   |
      GEOFENCE_EVENT_OUTSIDE_AREA  );

   gfConfig.mGfRadius = AON_OTB_DEFAULT_DISTANCE;
   gfConfig.mGfContextUsageFlag = FALSE; //disable context
   gfConfig.mGfForceResponsiveness = TRUE; //disable motion sensor
   return TRUE;
}
/* 
    @brief Breach response handler fn invoked by gmproxy 
    @param[in] gfId Geofence Id
    @param[in] breachType Type of breatch
    @param[in] location  Lcoation object instance adssocited witht his breach
    @return TRUE or FALSE
    */
boolean AonOtbTransac::breachRspHandler(uint32 gfId, aonGMBreachType breachType, locEngPositionReportMsgT_v01 &location)

{
   boolean retVal = FALSE;

   AON_MSG_MED("AonOtbTransac::breachRspHandler. gfId %d breachType %d", 
               gfId, breachType,0);


   mAonLocCb(mReqId, &location, AON_POSITION_TYPE_TRACKING);

   //Repalce the GF with updated params
   retVal = replaceGF(gfId, location.latitude, location.longitude);

   return retVal;
}
