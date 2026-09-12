#ifndef TM_AUXTECH_IF_H
#define TM_AUXTECH_IF_H

/*===========================================================================

                            tm_auxtech_iface.h

DESCRIPTION
  This header file provides interface for auxiliary technology related APIs.


  Copyright (c) 2016 - 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_common/inc/tm_auxtech_iface.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

  when        who     what, where, why
  --------    ---     ----------------------------------------------------------
  06/20/19    skm     Z-Axis support for DBH
  12/21/18    skm     Provide UE WIFI Mac Address in LPPe
  03/20/18    skm     Supl E911 over Wifi 
  06/1/17     ak      Add AuxTech request ID for Recovery
  02/17/17    skm     BLE and Barometer support for LPPE Phase 2
===========================================================================*/

#include "tm_slim_iface.h"
#include "tech_sel_api.h"

#ifdef FEATURE_GNSS_LPPE
/* Wifi AP info */
typedef struct
{
  time_type p_timestamp_ms; /* Modem timestamp in ms */

  locEngWifiApInfoStructT_v01 z_wifi_ap_info; /* Wifi info*/
  locEngWifiApAdditionalDataStructT_v01 z_wifi_ap_add_info;/*Additional Wifi info */
} tm_lppe_WifiApInfoType;

/* Wifi scan results*/
typedef struct
{
  locEngWlanApErrEnumT_v01 e_errorCause; /*Error Cause if any*/
  uint32 q_numWifiAps; /* number of wifi access points */
  uint8_t ue_mac_address[6]; /* UE Wifi Mac Address*/
  tm_lppe_WifiApInfoType *pz_wifiApInfo; /* Wifi AP info array. Number of elements is equal to q_numWifiAps field. */
} tm_lppe_WifiInfoListType;

/* Srn AP info*/
typedef struct
{
  /*Below pointer holds the array containing the info of access points in the SRN scan.*/
  locEngSrnBtleApDeviceDataStructT_v01 z_srn_ap_info; /*Wif info array pointer*/
} tm_lppe_SrnApInfoType;

typedef struct
{
  locEngSrnApDataTechTypeEnumT_v01 e_srnApTechType;    /*Srn Tech Mask*/
  locEngSrnApErrEnumT_v01 e_errorCause;                /*Error Cause if any*/
  uint32 q_numSrnAps;                                /* number of wifi access points */
  tm_lppe_SrnApInfoType *pz_srnApinfo;                 /*Srn scan info*/
} tm_lppe_SrnInfoListType;



/* ---------------------------------------------------------------------------
 * Data type definitions for sensor messages
 * ---------------------------------------------------------------------------*/

typedef techSel_SensorBaroInfoType tm_lppe_sensorInfoType;

#endif /* FEATURE_GNSS_LPPE */

/* Auxiliary Tech Requestor Id */
typedef enum
{
  TM_AUXTECH_REQ_ID_NONE,
  TM_AUXTECH_REQ_ID_SUPLMLID,
  TM_AUXTECH_REQ_ID_LPPE_UP,
  TM_AUXTECH_REQ_ID_LPPE_CP,
  TM_AUXTECH_REQ_ID_LPPE,
  TM_AUXTECH_REQ_ID_RECOVERY,
  TM_AUXTECH_REQ_ID_SUPLWLAN,
  TM_AUXTECH_REQ_ID_EMERGENCY_ORIG
} tm_auxTechReqId;



/**************************** WIFI APIS **************************/

/*===========================================================================
FUNCTION  tm_reqOnDemandWifiScan

DESCRIPTION
  Request on-demand wifi scan.
  If v_emergencyMode is set, priority processing (emergency mode) is requested.

LIMITATION
  Concurrent tm_reqOnDemandWifiScan API calls are not supported. So when the client calls
  this API, it should call tm_resetOnDemandWifiScanReq  API for the next client to make use of this API.

PARAMETERS
  v_emergencyMode - if set to TURE, priority processing is requested.
  e_reqId - requestor id.
  l_clientId - client id.

DEPENDENCIES
   None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
void tm_reqOnDemandWifiScan(boolean v_emergencyMode, tm_auxTechReqId e_reqId,pdsm_client_id_type l_clientId);

/*===========================================================================
FUNCTION  tm_resetOnDemandWifiScanReq

DESCRIPTION
  Reset on-demand wifi scan related variables.
  If this call is made, wifi scan re-request is not made on the next wifi ap results.

PARAMETERS
  e_reqId - requestor id.

DEPENDENCIES
   tm_reqOnDemandWifiScan should be called before calling this API.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void tm_resetOnDemandWifiScanReq(tm_auxTechReqId e_reqId);

#ifdef FEATURE_GNSS_LPPE

/*===========================================================================
FUNCTION  tm_cacheWifiScanReportsInit

DESCRIPTION
  Initialize wifi scan report cache. This frees up the memory allocated for the cache.

DEPENDENCIES
  None
RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void tm_cacheWifiScanReportsInit(void);

/*===========================================================================
FUNCTION  tm_cacheWifiScanReports

DESCRIPTION
  Cache wifi scan reports. Both on-demand and free scan reports are cached.
  They are stored in tm_lppe_WifiInfoListType data structure. The cached reports 
  can be retrieved using the tm_cacheWifiScanReportsGet API.


PARAMETERS
  z_wifiScanResult - wifi scan results
  e_reqId - requestor id.

DEPENDENCIES
   tm_lppe_cacheWifiScanReportsInit should be called before calling thi API for the first time.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void tm_cacheWifiScanReports(pdsm_wifi_on_demand_info_list_s_type z_wifiScanResult, tm_auxTechReqId e_reqId);


/*===========================================================================
FUNCTION  tm_lppe_cacheWifiScanReportsGet

DESCRIPTION
  Get the cached wifi reports. 
  If the number of reports are more than 64,
  the wifi ap list is adjusted such that the first q_numWifiAps have the bigger rssi values in descending order.

  The caller of this API should not free the returned memory.
PARAMETERS
  q_numWifiAps - Maximum number of wifi ap reports needed

DEPENDENCIES
   None
RETURN VALUE
  Pointer to cached Wifi scan reports.
  q_num_wifi_aps field represents the number of cached wifi aps.
  first_wifi_ap_ptr is a pointer to the cached wifi aps.
  In case of no reports, q_num_wifi_aps would be zero and first_wifi_ap_ptr is NULL.

SIDE EFFECTS
   None
===========================================================================*/
tm_lppe_WifiInfoListType * tm_cacheWifiScanReportsGet( );


/**************************** SRN APIS **************************/
/*===========================================================================
FUNCTION  tm_auxTechSrnScanReportsInit

DESCRIPTION
  Initialize Srn scan report cache. This frees up the memory allocated for the cache.

DEPENDENCIES
  None
RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void tm_auxTechSrnScanReportsInit(void);

/*===========================================================================
FUNCTION  tm_cacheWifiScanReports

DESCRIPTION
  Cache SRN BTLE scan reports.
  They are stored in tm_lppe_WifiInfoListType data structure. The cached reports 
  can be retrieved using the tm_cacheWifiScanReportsGet API.


PARAMETERS
  z_srnScanResult - SRN scan results
  e_reqId - requestor id.

DEPENDENCIES
   tm_cacheSrnScanReportsInit should be called before calling thi API for the first time.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void tm_cacheSrnScanReports(pdsm_srn_scan_info_list_s_type z_srnScanResult, tm_auxTechReqId e_reqId);

/*===========================================================================
FUNCTION  tm_reqSrnScan

DESCRIPTION
  Request SRN (BTLE) scan.
  If v_emergencyMode is set, priority processing (emergency mode) is requested.

LIMITATION
  Concurrent tm_reqSrnScan API calls are not supported. So when the client calls
  this API, it should call tm_reqSrnScan  API for the next client to make use of this API.

PARAMETERS
  v_emergencyMode - if set to TURE, priority processing is requested.
  e_reqId - requestor id.
  l_clientId - client id.

DEPENDENCIES
   None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
void tm_reqSrnScan(boolean v_emergencyMode, 
                   tm_auxTechReqId e_reqId,
                   pdsm_client_id_type l_clientId, 
                   boolean v_request);

/*===========================================================================
FUNCTION  tm_cacheSrnScanReportsGet

DESCRIPTION
  Get the cached SRN reports. If the number of reports are more than 64,
  the SRN ap list is adjusted such that the first q_numSrnAps have the bigger rssi
  values in descending order.

  The caller of this API should not free the returned memory.
PARAMETERS
   None
DEPENDENCIES
   None
RETURN VALUE
  Pointer to cached Srn scan reports.

SIDE EFFECTS
   None
===========================================================================*/
tm_lppe_SrnInfoListType * tm_cacheSrnScanReportsGet( );

/*===========================================================================
FUNCTION  tm_SrnReqIdGet

DESCRIPTION
  Get Srn scan requestor Id.

DEPENDENCIES
   None
RETURN VALUE
  TM_AUXTECH_REQ_ID_NONE if there is no Srn requestor id.
  Other values for valid Srn requestor id

SIDE EFFECTS
   None
===========================================================================*/
tm_auxTechReqId tm_SrnReqIdGet(void);

/**************************** Sensor APIS **************************/
/*===========================================================================
FUNCTION  tm_cacheSensorBaroMeasReportGet

DESCRIPTION
  Get the cached Sensor report.Currently only Barometer pressure measurment
  is cached. And the Latest pressure measurement is provided when this
  function is called.

  The caller of this API should not free the returned memory.
PARAMETERS
   None
DEPENDENCIES
   None
RETURN VALUE
  Pointer to cached Sensor barometer measurement report.

SIDE EFFECTS
   None
===========================================================================*/
tm_lppe_sensorInfoType * tm_cacheSensorBaroMeasReportGet( );

/*===========================================================================
FUNCTION  tm_reqSensorBaroMeas

DESCRIPTION
  Request Sensor Measurements, currently Uncompromised Barometer Pressure is supported.

PARAMETERS
  e_reqId   - requestor id.
  v_request - TRUE: start, FALSE: stop sensor streaming.

DEPENDENCIES
   None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
void tm_reqSensorBaroMeas( tm_auxTechReqId e_reqId,
                           boolean v_request );

#endif /* FEATURE_GNSS_LPPE */

/*===========================================================================
FUNCTION  tm_wifiReqIdGet

DESCRIPTION
  Get Wifi requestor Id.

DEPENDENCIES
   None
RETURN VALUE
  TM_AUXTECH_REQ_ID_NONE if there is no wifi requestor id.
  Other values for valid wifi requestor id

SIDE EFFECTS
   None
===========================================================================*/
tm_auxTechReqId tm_wifiReqIdGet(void);

/*===========================================================================
FUNCTION  tm_wifiClientIdGet

DESCRIPTION
  Get Client Id used in the last tm_reqOnDemandWifiScan API call. If tm_reqOnDemandWifiScan API is never called, -1 is returned.

DEPENDENCIES
   None
RETURN VALUE
  Client Id used in the last tm_reqOnDemandWifiScan API call .
  -1 is returned if tm_reqOnDemandWifiScan API is never called.

SIDE EFFECTS
   None
===========================================================================*/
pdsm_client_id_type tm_wifiClientIdGet(void);

/************************** CPI APIs **********************/

/*===========================================================================
FUNCTION  tm_startCPIRequest

DESCRIPTION
  Request to start CPI .

LIMITATION
  Concurrent tm_startCPIRequest API calls are not supported. So when the client calls
  this API, it should call tm_stopCPI  API for the next client to make use of this API. 

DEPENDENCIES
  None

PARAMETERS
  v_emergencyMode Is Emergency mode.
  e_reqId Requestor Id

RETURN VALUE
  TRUE if start is successful
  FALSE if start failed

SIDE EFFECTS
   None
===========================================================================*/
boolean tm_startCPIRequest(boolean v_emergencyMode, tm_auxTechReqId e_reqId);

/*===========================================================================
FUNCTION  tm_stopCPI

DESCRIPTION
  Stop CPI.

PARAMETERS
  e_reqId Requestor Id

DEPENDENCIES
   e_reqId should match with the one mentioned in tm_startCPIRequest API call

RETURN VALUE
  TRUE if stop is successful
  FALSE if e_reqId do not match with the one mentioned in tm_startCPIRequest API call or
        if stop call failed.

SIDE EFFECTS
   None
===========================================================================*/
boolean tm_stopCPI(tm_auxTechReqId e_reqId);

/*===========================================================================
FUNCTION  tm_CpiReqIdGet

DESCRIPTION
  Get Cpi requestor Id.

DEPENDENCIES
   None
RETURN VALUE
  TM_AUXTECH_REQ_ID_NONE if there is no Cpi requestor id.
  Other values for valid Cpi requestor id

SIDE EFFECTS
   None
===========================================================================*/
tm_auxTechReqId tm_CpiReqIdGet(void);

/******************* DBH APIS *****************************/

/*===========================================================================
FUNCTION  tm_DBHPositionGet

DESCRIPTION
  DBH position request. Based on the requestor id, the position is returned to the
  corresponding protocol's event callback function handler.

PARAMETERS
  e_reqId Requestor Id

DEPENDENCIES
   None
RETURN VALUE
  FALSE for unsupported requestor id. TRUE otherwise
SIDE EFFECTS
   None
===========================================================================*/
boolean tm_DBHPositionGet( tm_auxTechReqId e_reqId);

/*===========================================================================
FUNCTION  tm_CacheDBHPosition

DESCRIPTION
  Function to cache CPI position for DBH request.

PARAMETERS
  z_pos Fix info sm_FixReportStructType 

DEPENDENCIES
   None
RETURN VALUE
  None
SIDE EFFECTS
   None
===========================================================================*/
void tm_CacheDBHPosition( sm_FixReportStructType z_pos );

/*===========================================================================
FUNCTION  tm_DBHPositionHandleReportGpsStateInfo

DESCRIPTION
  Handle DBH position request. This function should be called by the response (to tm_core_get_gps_state)
  function when when best available position is requested with type SM_GPS_STATE_INFO_REQ_TYPE_GET_DBH.
  This function then calls the protocol's event callback function handler based on the requestor id.

PARAMETERS
  z_pos best available position

DEPENDENCIES
   None
RETURN VALUE
  None
SIDE EFFECTS
   None
===========================================================================*/
void tm_DBHPositionHandleReportGpsStateInfo(sm_FixReportStructType z_pos);

/*===========================================================================
FUNCTION  tm_auxtech_wifi_on_demand_req_timer_expired

DESCRIPTION
  Handle timer expiration of WIFI on-demand request. Upon this timer expiry the
  WIFI on-demand scan will be requested if all the conditions are met.

PARAMETERS
  cq_TimerId : Expired timer id.

DEPENDENCIES
   None
RETURN VALUE
  None
SIDE EFFECTS
   None
===========================================================================*/
void tm_auxtech_wifi_on_demand_req_timer_expired( uint32 const cq_TimerId  );

#endif /* TM_AUXTECH_IF_H */
