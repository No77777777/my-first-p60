/*======================================================================

              Technology Selector  API header file

 GENERAL DESCRIPTION
  This file contains API declarations for Technology Selector module
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016-2021 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/api/tech_sel_api.h#2 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
12/15/21   sai   Added new NV for ODCPI and change in ODCPI back off logic
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_API_H
#define TECH_SEL_API_H
#include "comdef.h"
#include "customer.h"
#include "gps_common.h"
#include "gnss_common.h"
#include "sm_api.h"
#include "tm_prtl_iface.h"
#include "slim_client_msg_types.h"
#include "mgp_api.h"
#include "sm_nv_efs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TECH_SEL_SENSOR_CLIENT_MASK_LPP    (0x00000001)
#define TECH_SEL_SENSOR_CLIENT_MASK_FUSION (0x00000002)

/*Data type representing client handle*/
typedef uint32 q_ClientHandle;

/*Client Capability Type*/
typedef struct
{
  uint32 b_IsPosFixCapabale : 1; /*Client is capable of producing position fix*/
  uint32 b_IsSerReqCapable  : 1; /*Client is capable of requesting Tsel Services*/
  uint32 : 30; /*For Padding*/
}techSel_ClientCapType;

/*Client Pos Fix Capability Type*/
typedef struct
{
  uint32 b_IsGnssCapable : 1; /*Client is capable of producing GNSS fix*/
  uint32 b_IsCpiCapable : 1; /*Client is capable of producing CPI fix*/
  uint32 b_IsLpGtpWifiCapable : 1;/*Client is capable of producing LpGtpWifi fix*/
  uint32 : 29; /*For Padding*/
}techSel_ClientPosFixCapType;

/*Client Pos Fix Capability Info*/
typedef struct
{
  techSel_ClientPosFixCapType z_ClientPosFixCapType; /*Client Pos Fix Capability Type*/
  /*Accuracy and Power Capabilities shall be added in future versions*/
}techSel_ClientPosFixCapInfo;

/*Client Capability Info*/
typedef struct
{
 techSel_ClientCapType z_ClientCapType; /*Client Capability Type*/
 techSel_ClientPosFixCapInfo z_ClientPosFixCap;/*Client Pos Fix Capability Info*/
}techSel_ClientCapbilityInfo;

/*Client Info*/
typedef struct
{
  techSel_ClientCapbilityInfo z_ClientCapability;/*Client Capability Info*/
}techSel_ClientInfo;

/*CPI Request Struct*/
typedef struct
{
  boolean v_EmergencyMode; /*Whether CPI need to be requested in Emergency Mode*/
}techSel_CpiRequest;

/*Data type representing a requuest ID. It can be used for any functionality rquiring a request ID.*/
typedef uint32 q_TselRequestId;

typedef struct
{
  boolean v_is_mo;  /*Whether Session is Mobile Originated*/
  boolean v_is_emergency; /*Whether Session is emergency Session*/
  boolean v_is_internal; /*Whether Session is an internal Session*/  
  tm_sess_operation_mode_e_type  e_mode; /*Session Operation Mode*/
  tm_sess_power_mode_s_type     z_power_mode; /*Session LPPM Power Mode*/
  uint32                        q_tbf_msec; /*Time between fixes in milliSeconds*/
  uint32                        q_num_fixes; /*Number of fixes*/
} techSel_PosSessInfo;

typedef enum
{  
  TECH_SEL_LOWI_NO_ERROR,
  TECH_SEL_LOWI_DRIVER_ERROR,
  TECH_SEL_LOWI_LP_ERROR,
  TECH_SEL_LOWI_NO_WIFI_ERROR = 0x100000
}techSel_LowiErrorEnumType;

typedef enum
{
  /** 2.4 Ghz only*/
  TECH_SEL_WIFI_BAND_TWO_POINT_FOUR_GHZ,
  /** 5 Ghz only*/
  TECH_SEL_WIFI_BAND_FIVE_GHZ,
  /** Both (2.5 & 5 Ghz)*/
  TECH_SEL_WIFI_BAND_BAND_ALL,
  TECH_SEL_WIFI_BAND_BAND_MAX = 0x100000
}techSel_WifiBandEnumType;



typedef enum
{
  /** Passive Scan*/
  TECH_SEL_WIFI_SCAN_METHOD_PASSIVE,
  /** Active Scan*/
  TECH_SEL_WIFI_SCAN_METHOD_ACTIVE,
  TECH_SEL_WIFI_SCAN_METHOD_MAX = 0x100000
}techSel_WifiScanMethodEnumType;

/*Call Back function for Wifi Scan*/
typedef boolean (techSel_clientWifiScanCbk)(pdsm_wifi_scan_info_ext_s_type *p_wifi_scan, uint32 q_powerConsumed);

/*Call Back function for Ranging Wifi Scan*/
typedef boolean (techSel_clientWifiRangingScanCbk)(pdsm_RangingScanRespType *p_wifi_scan, uint32 q_powerConsumed);

typedef struct
{  
  dword  num_fixes; /*Number of fixes*/
  dword  time_between_fixes; /*Time Between Fixes*/
  uint8  horizontal_accuracy; /*Horizontal Accuracy in meters needed*/
  uint8  session_timeout;  /*Session Timeout*/
  pdsm_pd_session_operation_e_type pos_operation_mode; /*Position operation mode*/  
} techSel_GnssPosReqInfo;

typedef enum
{
  TECH_SEL_POS_REQ_ERROR_NONE = 0,
  TECH_SEL_POS_REQ_ERROR_FAILED_TO_START = 1,
  TECH_SEL_POS_REQ_ERROR_FIX_NOT_ACQUIRED = 2,
  TECH_SEL_POS_REQ_ERROR_MAX = 0x100000
} techSel_GnssPosReqErrorEnumType;

typedef union
{
  slimInjectSensorDataIndMsgT z_SensorDataInd;
  slimErrorRespMsgT           z_SensorErrorResp;
  slimInjectServiceStatusEventIndMsgT z_SensorServiceStatus;
} techSel_SensorMsgTypeU;

typedef struct
{
  slimMessageHeaderStructT      z_header;      /* Slim message header*/
  techSel_SensorMsgTypeU         z_SensorMsg;   /* Union to hold IND/RESP from SLIM*/
} techSel_SensorReportType;

typedef enum
{
  TECH_SEL_SENSOR_MEAS_ERROR_NONE = 0,
  TECH_SEL_SENSOR_MEAS_NOT_AVAILABLE = 1,
  TECH_SEL_SENSOR_MEAS_NOT_SUPPORTED = 2,
} techSel_sensorErrorEnumT;

typedef struct
{
  techSel_sensorErrorEnumT e_errorCause;     /* Error Cause if any */
  FLT f_BaroPressure;                   /* uncompromised barometer pressure */
  FLT f_BaroAltitude;                   /* Estimated altitude based on Baro Data*/
  FLT f_FilteredBaroAltitude;           /* Estimated smoothed altitude */
  FLT f_FilteredBaroPressure;           /* filtered barometer pressure */
  uint16 w_FilterOutlierCount;
} techSel_SensorBaroInfoType;

typedef struct
{
  techSel_sensorErrorEnumT e_errorCause; 
  slimSensorDataStructT    z_SensorData;
  int64                    r_InjectTimeOffsetMsec; /* time offset between sensor data base_time
                                                      and modem timetick msec */ 
} techSel_SensorInfoType;


/*Call Back function for GNSS Position fixes*/
typedef boolean (techSel_clientGnssPosFixCbk)(const sm_FixInfoStructType *p_posFixCbk, uint32 e_errorType, uint32 q_powerConsumed);

/*Call Backs for Sensor LE*/
typedef void (techSel_clientSensorLeStatusCbk)(sm_SensorLEStatusStructType *p_SensorLEStatus);
typedef void (techSel_clientSensorLeRelDispCbk)(const sm_SensorLERelDispStructType * const p_RelDisp);


/********************************************************************
*
* techSel_Init
*
* Function description:
* This function is used to intialize the Technology Selector. It should be called at bootup and 
* before calling any other TechSel Api in this header file.
*
* Parameters: 
* Void
* Return:
* Void
*  
*********************************************************************
*/  
void techSel_Init(void);

/********************************************************************
*
* techSel_ClientRegister
*
* Function description:
* This function is used by Tech Sel clients to register to TechSel.
* Clients provide info like clilent capabilities and receives a client handle upon succesful registeration
*
* Parameters: 
* techSel_ClientInfo* : Pointer to client info
* q_ClientHandle*: Pointer to cllient handle which will be filled by the tech sel.
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_ClientRegister(techSel_ClientInfo *pz_ClientInfo, q_ClientHandle *pq_ClientHandle);

/********************************************************************
*
* techSel_ReportPedStatus
*
* Function description:
* This function is to report Ped status to Tech Sel.
* It is used to decide whether or not to start LPPM
*
* Parameters: 
* gnss_MgpPedStatusEnumType: Ped Status
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_ReportPedStatus(gnss_MgpPedStatusEnumType e_PedStatus);

/********************************************************************
*
* techSel_ReportGnssQualInd
*
* Function description:
* This function is to report GNSS Qual Ind to Tech Sel
* This is used in determining TBM (Time Between Measurements) for LPPM
*
* Parameters: 
* gnss_QualIndEnumType: Gnss Qual Ind
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_ReportGnssQualInd (gnss_QualIndEnumType  e_GnssQualityInd);

/********************************************************************
*
* techSel_ReportPedDevCtx
*
* Function description:
* This function is used  to handle pedestrian device context
* This is used in determining TBM (Time Between Measurements) for LPPM
*
* Parameters: 
* e_PedDevCtx:   Pedestrian Device Context enum
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_ReportPedDevCtx(gnss_PedDevCxtEnumType  e_PedDevCtx);

/********************************************************************
*
* techSel_ReportLppmStatus
*
* Function description:
* This function is to report LPPM Status report
*
* Parameters: 
* sm_LppmStatusStructType: Pointer to LPPM status
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_ReportLppmStatus(const sm_LppmStatusStructType *pz_LppmStatus);


/********************************************************************
*
* techSel_ReportPedAlignAvailInd
*
* Function description:
* This function is to report Ped Alignement to Tech Sel
* This is used in determining whether or not to start LPPM
*
* Parameters: 
* v_PedAlignSensorAvailable: Whether ped alignement is available or not
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_ReportPedAlignAvailInd(boolean v_PedAlignSensorAvailable);


/********************************************************************
*
* techSel_ReportPedSensorAssistAvailInd
*
* Function description:
* This function is to reports Ped Sensor assistance availability to Tech Sel
* This is used in determining TBM (Time Between Measurements) for LPPM
*
* Parameters: 
* v_PedAlignSensorAvailable: Whether ped sensor assistance is available or not
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_ReportPedSensorAssistAvailInd(boolean v_PedSensorAssistAvailable);

/********************************************************************
* techSel_ReportGnssEngineStateChangeInd
*
* Description:
* This function is used to handle MGP state change indicaions
*
* Parameters:
*  pz_rcvrState: pointer to receiver state change info
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_ReportGnssEngineStateChangeInd(sm_RcvrStateChangeStructType *pz_rcvrState);

/********************************************************************
* techSel_ReportPosSessionStart
*
* Description:
* This function is used to report starting of a position session
*
* Parameters:
*  pz_sess_info: pointer to struct containing information about the pos session
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_ReportPosSessionStart(techSel_PosSessInfo *pz_sess_info);

/********************************************************************
* techSel_ReportPosFixStart
*
* Description:
* This function is used to report starting of a position fix
*
* Parameters:
*
*
* Return value:
* boolean: Pass\Fail
*********************************************************************
*/
boolean techSel_ReportPosFixStart(void);

/********************************************************************
* techSel_ReportE911Start
*
* Description:
* This function reports E911 start to TechSel
*
* Parameters:
* None
*
* Return value:
* None
*********************************************************************
*/
void techSel_ReportE911Start(void);

/********************************************************************
* techSel_ReportPosSessionStop
*
* Description:
* This function is used to report stopping of a position session
*
* Parameters:
*  pz_sess_info: pointer to struct containing information about the pos session
* abort_reason: Enum containing the reason of aborting a positioning sessions
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_ReportPosSessionStop(techSel_PosSessInfo *pz_sess_info,
                                     tm_sess_stop_reason_e_type    abort_reason);


/********************************************************************
* techSel_ReportPosFix
*
* Description:
* This function is used to report a position fix to TechSel
*
* Parameters:
*  pz_FixRpt: pointer to struct containing information about the position
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_ReportPosFix(sm_FixInfoStructType *pz_FixRpt);

/********************************************************************
* techSel_StartCpi
*
* Description:
* This function is used to request starting of CPI fixes.
* This function will return a request ID which client shall use when reqesting stop of a CPI fixes
* Please note that it is responsibility of the client to stop the CPI requests and TechSel wont
* stop the CPI on its own
*
* CPI fixes can be started in normal mode or emergency mode.
* If normal CPI is already going on and a request to start emergency CPI is received,
* then, normal CPI is stopped and emergency CPI is started. In such a case, clients
* which stated normal CPI may get emergency CPI position fixes. The assumption here
* is that normal CPI clients are ok to get emergency CPI fixes.
*
* Parameters:
*  pz_CpiRequest: pointer to struct containing request for CPI
*  pq_ReqId: Pointer to a request ID which shall be filled in by this function
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_StartCpi(techSel_CpiRequest *pz_CpiRequest,q_TselRequestId *pq_ReqId);

/********************************************************************
* techSel_StopCpi
*
* Description:
* This function is used to request stopping of CPI fixes.
* This function expects client to pass the request ID which was assigned to it in techSel_StartCpi.
* Please note that it is responsibility of the client to stop the CPI requests and TechSel wont
* stop the CPI on its own
*
* Parameters:
*  pz_CpiRequest: pointer to struct containing request for CPI
*  pq_ReqId: Pointer to a request ID which shall be filled in by this function
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_StopCpi(q_TselRequestId q_RedId);

/********************************************************************
* techSel_RequestOnDemandWifiScan
*
* Description:
* This function is used to request On Demand Wifi scan
*
* Parameters:
*  e_band: Bands over which wifi scan is requested
*  e_scanType: Type of scan
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_RequestOnDemandWifiScan( techSel_WifiBandEnumType e_band, 
                                         techSel_WifiScanMethodEnumType e_scanType,
                                         techSel_clientWifiScanCbk* p_onDemandWifiScanCbk);

/********************************************************************
* techSel_RegisterForUnsolicitedWifiScan
*
* Description:
* This function is used to register for unsolicted Wifi scans
*
* Parameters:
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_RegisterForUnsolicitedWifiScan( techSel_clientWifiScanCbk* p_unsolicitedWifiScanCbk );

/*===========================================================================
  techSel_ReportWifiScan

  Description:

     This function is used to send Wifi Scan to TechSel

 Parameters:

   p_response: Pointer to Lowi Response

  Return value: 
    void

=============================================================================*/
boolean techSel_ReportWifiScan(t_wiper_ap_set_struct_type* p_wifiScan);

/*===========================================================================
  techSel_ReportLowiError

  Description:

     This function reports Lowi Error to TechSel

 Parameters:

   e_status:Lowi Error

  Return value: 
    void

=============================================================================*/
boolean techSel_ReportLowiError(void);


/********************************************************************
*
* techSel_handleLppmNvConfig
*
* Function description:
* This function handles LPPM config read from NV
*
* Parameters: 
* q_LppmConfig: Lppm Config 
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_handleLppmNvConfig(uint32 q_LppmConfig);

/********************************************************************
*
* techSel_handleWifiAssistNvConfig
*
* Function description:
* This function handles Wifi Assistance config read from NV.
* Wifi Assistance config controls the behaviour when Wifi is used to assist in GNSS sessions.

* Parameters: 
* q_WifiAssistConfig: Wifi Assistance Config 
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_handleWifiAssistNvConfig(uint32 q_WifiAssistConfig);

/********************************************************************
*
* techSel_handlePremiumSvcConfig
*
* Function description:
* This function handles Premium Service Configuration
*
* Parameters: 
* q_PremiumSvcCfg: Premium Service Config 
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/  
boolean techSel_handlePremiumSvcConfig(uint32 q_PremiumSvcCfg);


/********************************************************************
*
* techSel_ReportTimerExpiry
*
* Function description:
* This function is used to report Tech Sel timer expiry
*
* Parameters: 
* q_timerId: Timer Id which got expired
*
* Return:
* void
*  
*********************************************************************
*/  
void techSel_ReportTimerExpiry(uint32 q_timerId);


/********************************************************************
*
* techSel_StartGnssPosFix
*
* Function description:
* This function requests GNSS fix to TechSel
*
* Parameters: 
* pz_PosReq: Pointer to Position request struct
* p_posFixCbk: Pointer to Pos fix Cbk Fn
*
* Return:
* void
*  
*********************************************************************
*/  
boolean techSel_StartGnssPosFix(techSel_GnssPosReqInfo *pz_PosReq, techSel_clientGnssPosFixCbk *p_posFixCbk);
/********************************************************************
*
* techsel_StopGnssPosFix
*
* Function description:
* This function requests to stop gnss position fixes
*
* Parameters: 
* p_posFixCbk: Pointer to Pos fix Cbk Fn
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techsel_StopGnssPosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);
/********************************************************************
*
* techSel_RegisterForUnsolicitedGnssPosFix
*
* Function description:
* This function registers for unsolicited GNSS position fixes
*
* Parameters: 
* p_posFixCbk: Pointer to Pos fix Cbk Fn
*
* Return:
* void
*  
*********************************************************************
*/  
boolean techSel_RegisterForUnsolicitedGnssPosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);

/********************************************************************
*
* techSel_handleSensorNvConfig
*
* Function description:
* This function handles Sensor manager config read from NV
*
* Parameters: 
* u_BaroSampleRate: Required barometer sampling rate in Hz (range: 1 - 100)
* (input)
* 
* u_BaroWeight : Weight factor for the current barometer sample (range 1:100 %)
* (input)
* 
* v_EnableFileStorage: If true, the Baromter data will be stored in EFS
* (input)
* 
* Return:
*  
*********************************************************************
*/  
void techSel_handleSensorNvConfig(uint8 u_BaroSampleRate,
                                  uint8 u_BaroWeight,
                                  boolean v_EnableFileStorage);


/*=============================================================================

FUNCTION
  techSel_ProcessSensorReport

DESCRIPTION
  Process Sensor Baro information from SLIM in TM thread context

DEPENDENCIES
  None.

PARAMETERS
  cpz_SensorMsg - Pointer to SLIM Sensor Message

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
void techSel_ProcessSensorReport(const techSel_SensorReportType *cpz_SensorMsg );

/*=============================================================================

FUNCTION
  techSel_GetSensorBaroMeas

DESCRIPTION
  Obtains the current barometer data from SLIM in TM thread context

DEPENDENCIES
  None.

PARAMETERS
  pz_SensorInfo - If successful, points to the current baromter information
  (output)

RETURN VALUE
  TRUE  - If data is obtained
  FALSE - Otherwise

SIDE EFFECTS
  None.

=============================================================================*/
boolean techSel_GetSensorBaroMeas(techSel_SensorBaroInfoType *pz_SensorInfo);

/*=============================================================================

FUNCTION
  techSel_SlimReqSensorMeas

DESCRIPTION
  This function requests for enable/disable sensor measurements in tm thred.
  Sensor data can be shared among multiple modules in tm. The sensor data is requested
  with the highest sampling rate and the data can be pulled from sensor manager.

DEPENDENCIES
  None.

PARAMETERS
  q_SensorClient - A single bit in this mask identifies a client for sensor data
  (input)         in tm task
 
   e_Service    - Identifies the sensor to be used
  (input)
 
  u_SampleRateHz - If enabling a sensor, defines the sampling rate to be used
  (input)
 
  v_Enable      - If TRUE, it enables a snesor data collectin.
  (input)         If FALSE, and there is no other client for the sensor data the
                  sensor data streaming will be disabled.
RETURN VALUE
  TRUE  - If successful
  FALSE - Otherwise

SIDE EFFECTS
  None.

=============================================================================*/
boolean techSel_SlimReqSensorMeas(uint32 q_SensorClient, slimSensorTypeEnumT e_Service,
                                  uint8 u_SampleRateHz, boolean v_Enable);


/********************************************************************
*
* techSel_RegisterForGnssLePosFix
*
* Function description:
* This function requests GNSS LE fix to TechSel
*
* Parameters: 
* pz_PosReq: Pointer to Position request struct
* p_posFixCbk: Pointer to Pos fix Cbk Fn
*
* Return:
* void
*  
*********************************************************************
*/  
boolean techSel_RegisterForGnssLePosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);
/********************************************************************
*
* techSel_DeregisterForGnssLePosFix
*
* Function description:
* This function deregisters from receving GNSS LE position fixes
*
* Parameters: 
* p_posFixCbk: Pointer to Pos fix Cbk Fn
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techSel_DeregisterForGnssLePosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);

/********************************************************************
*
* techSel_StartSensorLe
*
* Function description:
* This function requests to start Sensor LE
*
* Parameters: 
* pz_SensorLEConfig: Pointer to Sensor LE Config
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techSel_StartSensorLe(const mgp_SensorLEConfigStructType  * const pz_SensorLEConfig,
                              techSel_clientSensorLeStatusCbk *p_statusCbk,
                              techSel_clientSensorLeRelDispCbk *p_relDispCbk,
                              q_TselRequestId *pq_ReqId);
/********************************************************************
*
* techSel_StopSensorLe
*
* Function description:
* This function requests to stop Sensor LE
*
* Parameters: 
* void
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techSel_StopSensorLe(q_TselRequestId q_ReqId);
/********************************************************************
*
* techSel_GetSensorLeRelDisp
*
* Function description:
* This function requests Sensor LE Rel Disp
*
* Parameters: 
* void
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techSel_GetSensorLeRelDisp(uint64 t_ModemTsFromMsec, 
                                   uint64 t_ModemTsToMsec,
                                   q_TselRequestId q_ReqId);
/********************************************************************
*
* techSel_HandleSensorLeStatus
*
* Function description:
* This function handles Sensor Le Status
*
* Parameters: 
* void
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techSel_HandleSensorLeStatus(sm_SensorLEStatusStructType *p_SensorLEStatus);
/********************************************************************
*
* techSel_HandleSensorLeRelDisp
*
* Function description:
* This function handles Sensor Le Rel Disp
*
* Parameters: 
* void
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techSel_HandleSensorLeRelDisp(const sm_SensorLERelDispStructType * const p_RelDisp);

/********************************************************************
* techSel_requestWifiRangingScan
*
* Description:
* This function is used to request Ranging Wifi Scan
*
* Parameters:
*  e_band: Bands over which wifi scan is requested
*  pz_ReqInfo: The ranging scan request information
*  p_RangingWifiScanCbk: The callback for the ranging scan results
*
* Return value: 
* boolean: Pass\Fail
*********************************************************************
*/  
boolean techSel_RequestWifiRangingScan( techSel_WifiBandEnumType e_Band, 
                                        pdsm_RangingScanReqType *pz_ReqInfo,
                                        techSel_clientWifiRangingScanCbk* p_RangingWifiScanCbk);

/*===========================================================================
  techSel_ReportWifiRangingScan

  Description:

     This function is used to send Ranging Wifi Scan to TechSel

 Parameters:

   p_response: Pointer to Wifi Scan

  Return value: 
    void

=============================================================================*/
boolean techSel_ReportWifiRangingScan(pdsm_RangingScanRespType* p_wifiScan);

/********************************************************************
*
* techSel_handleLppmNvTbmProfile
*
* Function description:
* This function is to handle lppm tbm profile
*
* Parameters: 
* u_LppmTbmProfile: lppm tbm profile
*
* Return:
* boolean : TRUE: if success else FALSE
*  
*********************************************************************
*/ 
boolean techSel_handleLppmNvTbmProfile(uint8 u_LppmTbmProfile);

/*===========================================================================
  techSel_HandleRcOnThreshold

  Description:

     This function is used to send RC On Threshold to TechSel

 Parameters:

   q_rcOnThresholdMsec: RC On Threshold in MilliSeconds

  Return value: 
    TRUE\FALSE

=============================================================================*/
boolean techSel_HandleRcOnThreshold(uint32 q_rcOnThresholdMsec);

/********************************************************************
*
* techSel_updateOdcpiTimerConfigFromNv
*
* Function description:
* This function handles ODCPI timer config read from NV.
* ODCPI timers controls the behaviour when ODCPI should be request by core tech sel logic.
	
* Parameters: 
* pz_odcpiTimers: ODCPI timers Config 
*
* Return:
* boolean: Pass\Fail
*  
*********************************************************************
*/	
boolean techSel_updateOdcpiTimerConfigFromNv(tech_sel_odcpi_timers_s_type* pz_odcpiTimers);

/********************************************************************
*
* techSel_isCpiBlocked
*
* Function description:
* This function returns if CPI is Blocked  
	
* Parameters: 
* void
*
* Return:
* boolean: TRUE\FALSE
*  
*********************************************************************
*/	
boolean techSel_isCpiBlocked(void);


#ifdef __cplusplus
}
#endif

#endif /*TECH_SEL_API_H*/
