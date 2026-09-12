/******************************************************************************
  @file:  loc_api_internal.h
  @brief: Location Middleware internal API header

  DESCRIPTION
      Declares the internal data types and macros
      used by loc middleware

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2010 - 2015 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014, 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  -----------------------------------------------------------------------------
 ******************************************************************************/

/*=====================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      ------------------------------------------------------- 
03/31/17   yh    Loc_NI cleanup
02/17/17   skm   BLE support for LPPE Phase 2 
05/19/16   yh    LocMW refactoring
01/11/16   yh    Loc_Wifi cleanup
10/21/15   sudj  Fix Crash caused by bad AON configuration
09/30/15   sj    Added hysterisis for unsolicited status indications 
06/11/15   sj    LB 2.0 integration
04/02/15   ssu   Batch Dwell notification for geofence.
03/02/15   sj    Use replaceGf() API from GM instead of Del/Add seq  
02/08/13   ssu   Move to the DOG Heartbeat mechanism on DIME and beyond   
04/07/10   ns    Added loc_mw_task_started flag to denote that loc mw
                 task has started

$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_api_internal.h#1 $
$Author: pwbldsvc $
$DateTime: 2019/10/24 01:30:36 $
======================================================================*/

#ifndef FEATURE_LOC_API_INTERNAL_H
#define FEATURE_LOC_API_INTERNAL_H

#include "gps_variation.h"
#include "queue.h"
#include "loc_api_2.h"
#include "pdapi.h"
#include "loc_conn.h"
#include "loc_xtra.h"
#include "loc_client.h"
#include "loc_pd.h"
#include "loc_pa.h"
#include "loc_ni.h"
#include "loc_wifi.h"
#include "msg.h"
#include "aries_os_globals.h"
#include "aries_os_api.h"
#include "loc_gdt.h"

/*Include to get the data-type definitions for QMI-LOC */
#include "location_service_v02.h"

#include "slim_task.h"
#include "err.h" // for ERR_FATAL

#ifdef __cplusplus
extern "C"
{
#endif
#define LOC_HANDLE_REGISTERED_CLIENTS (-2)

#define LOC_MSG_LOW_0( str )           MSG(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_LOW, str )
#define LOC_MSG_LOW_1( str, a )        MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_LOW, str, a )
#define LOC_MSG_LOW_2( str, a, b )     MSG_2(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_LOW, str, a, b )
#define LOC_MSG_LOW( str, a, b, c )    MSG_3(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_LOW, str, a, b, c )

#define LOC_MSG_MED_0( str )           MSG(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, str )
#define LOC_MSG_MED_1( str, a )        MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, str, a )
#define LOC_MSG_MED_2( str, a, b )     MSG_2(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, str, a, b )
#define LOC_MSG_MED( str, a, b, c )    MSG_3(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, str, a, b, c )

#define LOC_MSG_HIGH_0( str )                     MSG(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, str )
#define LOC_MSG_HIGH_1( str, a )                  MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, str, a )
#define LOC_MSG_HIGH_2( str, a, b )               MSG_2(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, str, a, b )
#define LOC_MSG_HIGH( str, a, b, c )              MSG_3(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, str, a, b, c )
#define LOC_MSG_HIGH_4( str, a, b, c, d )         MSG_4(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, str, a, b, c, d )
#define LOC_MSG_HIGH_5( str, a, b, c, d, e )      MSG_5(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, str, a, b, c, d, e )
#define LOC_MSG_HIGH_6( str, a, b, c, d, e, f )   MSG_6(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, str, a, b, c, d, e, f )


#define LOC_MSG_ERROR_0( str )         MSG(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_ERROR, str )
#define LOC_MSG_ERROR_1( str, a )      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_ERROR, str, a )
#define LOC_MSG_ERROR_2( str, a, b )   MSG_2(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_ERROR, str, a, b )
#define LOC_MSG_ERROR( str, a, b, c )  MSG_3(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_ERROR, str, a, b, c )

#define LOC_MSG_FATAL_0( str )         MSG(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_FATAL, str )
#define LOC_MSG_FATAL_1( str, a )      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_FATAL, str, a )
#define LOC_MSG_FATAL_2( str, a, b )   MSG_2(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_FATAL, str, a, b )
#define LOC_ERR_FATAL( str, a, b, c )  MSG_3(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_FATAL, str, a, b, c )

// The following macros can be used to print with LOC MW client ID appended at the front
// Since client ID is optional, if it has not been provided, then empty string will be printed
// pz_client must be a pointer to the client structure (loc_client_info_s_type) associated with the client
#define LOC_MW_CLIENT_MSG_LOW( pz_client, str, a, b, c )      \
             MSG_SPRINTF_4( MSG_SSID_GNSS_LOCMW, MSG_LEGACY_LOW, "[%s] " str, ((pz_client) ? (pz_client)->pb_ClientIdStr : ""), a, b, c )
#define LOC_MW_CLIENT_MSG_MED( pz_client, str, a, b, c )      \
             MSG_SPRINTF_4( MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, "[%s] " str, ((pz_client) ? (pz_client)->pb_ClientIdStr : ""), a, b, c )
#define LOC_MW_CLIENT_MSG_HIGH( pz_client, str, a, b, c )     \
             MSG_SPRINTF_4( MSG_SSID_GNSS_LOCMW, MSG_LEGACY_HIGH, "[%s] " str, ((pz_client) ? (pz_client)->pb_ClientIdStr : ""), a, b, c )
#define LOC_MW_CLIENT_MSG_ERROR( pz_client, str, a, b, c )    \
             MSG_SPRINTF_4( MSG_SSID_GNSS_LOCMW, MSG_LEGACY_ERROR, "[%s] " str, ((pz_client) ? (pz_client)->pb_ClientIdStr : ""), a, b, c )

// MACRO Initialize Critical Section
#define LOC_MW_INIT_CRIT_SECTION( mutex_ctrl_block )                     \
  { os_MutexInit( mutex_ctrl_block, MUTEX_DATA_ONLY_CONTEXT ); }

// MACRO Enter Critical Section
#define LOC_MW_ENTER_CRIT_SECTION( mutex_ctrl_block )                    \
  { os_MutexLock( mutex_ctrl_block ); }

// MACRO Leave Critical Section
#define LOC_MW_LEAVE_CRIT_SECTION( mutex_ctrl_block )                    \
  { os_MutexUnlock( mutex_ctrl_block ); }

/* This Marco is only used to check the sanity of uint8_t defined in location_service_v02.h
   TRUE is "1" and FALSE is "0", ie the valid values are less than "2"*/
#define LOC_MW_IS_VALID_BOOLEAN(value)    ((value)<2)

typedef struct
{
  // Module initialization is done when the first client registers for the service
  volatile boolean         loc_mw_task_started;
  boolean                  qmi_loc_enabled;
  boolean                  pd_module_initialized;
  boolean                  nmea_module_initialized;
  boolean                  pa_module_initialized;
  boolean                  conn_module_initialized;
  boolean                  xtra_module_initialized;
  boolean                  ni_module_initialized;
  boolean                  geofence_module_initialized;
  boolean                  aon_module_initialized;
  boolean                  v_nv_module_initialized;
  boolean                  nmea_module_needed;    // Whether NMEA module is needed or not

  pdsm_client_id_type      pdapi_client_id;       // Used for pdapi events
  pdsm_client_id_type      xtra_client_id;  // Used for xtra events
  pdsm_client_id_type      ni_client_id;    // Used for ni events

  locClnt_ModuleDataType          z_locClnt_Data;
  locPd_ModuleDataType            z_locPd_Data;
  locXtra_ModuleDataType          z_locXtra_Data;
  locConn_ModuleDataType          z_locConn_Data;
  locPa_ModuleDataType            z_locPa_Data;
  locNi_ModuleDataType            z_locNi_Data;
  os_MutexBlockType             crit_sec; // critical section for the module
  os_MutexBlockType               pd_crit_sec; // critical section for position module
  boolean                         wifi_module_initialized;
  locWifi_ModuleDataType          z_locWifi_Data;
  pdsm_client_id_type             wifi_client_id;
  os_MutexBlockType               wifi_crit_sec; // critical section for wifi module
  os_MutexBlockType               xtra_crit_sec; // critical section for xtra module
  os_MutexBlockType               client_crit_sec; // critical section for client module

  /* Location privacy related parameters */
  boolean                         v_NvAllowNfwPosReq;
  boolean                         v_NvAllowNfwPosReqNotification;
  boolean                         v_AfwAllowNfwPosReq;
  boolean                         v_AfwAllowAfwPosReq;
  boolean                         v_AfwAllowPriPosReq;
  boolean                         v_E911Session;
} loc_middleware_data_s_type;

extern loc_middleware_data_s_type loc_middleware_data;

typedef enum
{
  LM_MIDDLEWARE_MSG_ID_PDAPI = C_USR_MSG_ID_FIRST,

  LM_MIDDLEWARE_MSG_ID_LFM_REG_CB = 5000,

  LM_MIDDLEWARE_MSG_ID_SDP_SET_SPI_STATUS_IND =             6000,
  LM_MIDDLEWARE_MSG_ID_SDP_INJECT_SENSOR_DATA_IND =         6001,
  LM_MIDDLEWARE_MSG_ID_SDP_INJECT_TIME_SYNC_DATA_IND =      6002,
  LM_MIDDLEWARE_MSG_ID_SDP_GET_CRADLE_MOUNT_CONFIG_IND =    6003,
  LM_MIDDLEWARE_MSG_ID_SDP_GET_EXTERNAL_POWER_CONFIG_IND =  6004,
  LM_MIDDLEWARE_MSG_ID_SDP_SET_CRADLE_MOUNT_CONFIG_IND =    6005,
  LM_MIDDLEWARE_MSG_ID_SDP_SET_EXTERNAL_POWER_CONFIG_IND =  6006,
  LM_MIDDLEWARE_MSG_ID_SDP_EVENT_SENSOR_STREAMING_READINESS_IND =   6007,
  LM_MIDDLEWARE_MSG_ID_SDP_EVENT_TIME_SYNC_NEEDED_IND =     6008,
  LM_MIDDLEWARE_MSG_ID_SDP_EVENT_SWITCH_SPI_STREAMING_REPORT_IND =  6009,
  LM_MIDDLEWARE_MSG_ID_SET_SENSOR_CONTROL_CONFIG_IND                     =  6010,
  LM_MIDDLEWARE_MSG_ID_GET_SENSOR_CONTROL_CONFIG_IND                     =  6011,
  LM_MIDDLEWARE_MSG_ID_SET_SENSOR_PROPERTIES_IND                         =  6012,
  LM_MIDDLEWARE_MSG_ID_GET_SENSOR_PROPERTIES_IND                         =  6013,
  LM_MIDDLEWARE_MSG_ID_SET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_IND  =  6014,
  LM_MIDDLEWARE_MSG_ID_GET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_IND  =  6015,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_NI_IND     =                6016,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_GEN_ALERT_IND  =            6017,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_BREACH_IND =                6018,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_ADD_IND =                   6019,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_DELETE_IND =                6020,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_QUERY_IND  =                6021,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_EDIT_IND   =                6022,
  LM_MIDDLEWARE_MSG_ID_EVENT_PEDOMETER_CONTROL_IND  =       6023,
  LM_MIDDLEWARE_MSG_ID_EVENT_MOTION_DATA_CONTROL_IND  =     6024,
  LM_MIDDLEWARE_MSG_ID_PEDOMTER_REPORT_IND            =     6025,
  LM_MIDDLEWARE_MSG_ID_INJECT_MOTION_DATA_IND         =     6026,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_SET_ENGINE_CONFIG_IND =     6027,
  LM_MIDDLEWARE_MSG_ID_SDP_EVENT_VEHICLE_SENSOR_INJECTION_READINESS_IND = 6028,
  LM_MIDDLEWARE_MSG_ID_SDP_INJECT_VEHICLE_SENSOR_DATA_IND         =     6029,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_BATCH_BREACH_IND      =     6030,
  LM_MIDDLEWARE_MSG_ID_PD_EVENT_WIFI_AP_SCAN_INJECT_REQ_IND              =  6031,
  LM_MIDDLEWARE_MSG_ID_PD_INJECT_WIFI_AP_SCAN_IND     =     6032,
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_PROXIMITY_IND         =     6033,
  LM_MIDDLEWARE_MSG_ID_GTP_CLIENT_DLD_DATA_IND        =     6034,
  LM_MIDDLEWARE_MSG_ID_GDT_BEGIN_IND                  =     6035, /* MP sending indication to start a session */
  LM_MIDDLEWARE_MSG_ID_GDT_END_IND                    =     6036, /* MP sending indication to end a session */
  LM_MIDDLEWARE_MSG_ID_GDT_BEGIN_REQ_IND              =     6037, /* Ind resp. to the request received from AP */
  LM_MIDDLEWARE_MSG_ID_GDT_END_REQ_IND                =     6038, /* Ind resp. to therequest received from AP */
  LM_MIDDLEWARE_MSG_ID_WLE_APCACHE_REQ_IND            =     6039, /* Ind resp. to APCACHE request received from AP */
  LM_MIDDLEWARE_MSG_ID_WLE_NOAPCACHE_REQ_IND          =     6040, /* Ind resp. to NOAPCACHE request received from AP */
  LM_MIDDLEWARE_MSG_ID_GEOFENCE_BATCH_DWELL_IND       =     6041,
  LM_MIDDLEWARE_MSG_ID_TZ_REQ_IND                     =     6042,
  LM_MIDDLEWARE_MSG_ID_PD_EVENT_SRN_AP_SCAN_INJECT_REQ_IND    =  6043,
  LM_MIDDLEWARE_MSG_ID_PD_INJECT_SRN_AP_SCAN_IND      = 6044,
  LM_MIDDLEWARE_MSG_ID_SEND_FDCL_SERVICE_REQ_IND      = 6045,
  LM_MIDDLEWARE_MSG_ID_SEND_GET_FDCL_BS_LIST_IND       = 6046,
  LM_MIDDLEWARE_MSG_ID_SEND_INJECT_FDCL_DATA_IND      = 6047,
  LM_MIDDLEWARE_MSG_ID_SEND_FDCL2_SERVICE_REQ_IND     = 6048, /* Service Request indicating MP has data to upload */
  LM_MIDDLEWARE_MSG_ID_SEND_GET_FDCL2_BS_OBS_DATA_IND = 6049, /* Crowdsourced Data to upload */
  LM_MIDDLEWARE_MSG_ID_E911_STATE_IND                 = 6050, /* E911 state indication from TM */

  /* SLIM message ids reserve range 9000-9999 */
  LM_MIDDLEWARE_MSG_ID_SLIM_RESERVERD_MIN = IPC_SLIM_TO_TASK_MIN,
  LM_MIDDLEWARE_MSG_ID_SLIM_RESERVERD_MAX = IPC_SLIM_TO_TASK_MAX,

  LM_MIDDLEWARE_MSG_ID_QMI_LOC_MW_IND                 =     6100,
  LM_MIDDLEWARE_MSG_ID_UNUSED_IND                     =     6101,
  LM_MIDDLEWARE_MSG_ID_QMI_LOC_PA_IND                 =     6102,

  LM_MIDDLEWARE_MSG_ID_AON_MIN                           =     7000,
  /*  LM_MIDDLEWARE_MSG_ID_GM_ADD_RESPONSE                 =     7000,
    LM_MIDDLEWARE_MSG_ID_GM_EDIT_NOTIFY                    =     7001,
    LM_MIDDLEWARE_MSG_ID_GM_DELETE_RESPONSE                =     7002,
    LM_MIDDLEWARE_MSG_ID_GM_PURGE_RESPONSE                 =     7003,
    LM_MIDDLEWARE_MSG_ID_GM_BREACH_NOTIFY                  =     7004,
    LM_MIDDLEWARE_MSG_ID_GM_POS_REQ                        =     7005,
    LM_MIDDLEWARE_MSG_ID_GM_CLIENT_BLOB_NOTIFY             =     7006,
    LM_MIDDLEWARE_MSG_ID_GM_CLIENT_GEOFENCE_HANDELS_NOTIFY =     7007,
    LM_MIDDLEWARE_MSG_ID_GM_GEOFENCE_HANDELS_NOTIFY        =     7008,
    LM_MIDDLEWARE_MSG_ID_GM_QUERY_NOTIFY                   =     7009,
    LM_MIDDLEWARE_MSG_ID_GM_UNSOL_EVENT_NOTIFY             =     7010,
    LM_MIDDLEWARE_MSG_ID_GM_PRIO_POS_REQ_INDICATION_NOTIFY =     7011,
    LM_MIDDLEWARE_MSG_ID_GM_SET_ENGINE_CONFIG_NOTIFY       =     7012,
    LM_MIDDLEWARE_MSG_ID_GM_ABORT_POS_REQ                  =     7013,
    LM_MIDDLEWARE_MSG_ID_GM_BATCH_BREACH_NOTIFY            =     7014,
    LM_MIDDLEWARE_MSG_ID_GM_PROXIMITY_NOTIFY               =     7015,
    LM_MIDDLEWARE_MSG_ID_GM_REPLACE_NOTIFY                 =     7016 */
  LM_MIDDLEWARE_MSG_ID_AON_MAX                           =     7016,

  LM_MIDDLEWARE_MSG_ID_AON_STATUS_CB                     =     7200,
  LM_MIDDLEWARE_MSG_ID_AON_LOCATION_CB                   =     7201,

  LM_MIDDLEWARE_MSG_ID_GDT2_MIN                          = 8000,
  LM_MIDDLEWARE_MSG_ID_GTP_AP_STATUS_REQ_IND             = LM_MIDDLEWARE_MSG_ID_GDT2_MIN,
  LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_BEGIN_REQ_IND        = 8001,
  LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_READY_STATUS_IND     = 8002,
  LM_MIDDLEWARE_MSG_ID_GDT_RECEIVE_DONE_STATUS_IND       = 8003,
  LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_END_STATUS_IND       = 8004,
  LM_MIDDLEWARE_MSG_ID_GDT_RECEIVE_DONE_REQ_IND          = 8005,
  LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_END_REQ_IND          = 8006,
  LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_BEGIN_STATUS_IND     = 8007,
  LM_MIDDLEWARE_MSG_ID_GDT2_MAX                          = LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_BEGIN_STATUS_IND,

  LM_MIDDLEWARE_MSG_ID_CROWDSOURCE_MANAGER_MIN           = 9000,
  LM_MIDDLEWARE_MSG_ID_CROWDSOURCE_MANAGER_CONTROL_IND   = 9001,
  LM_MIDDLEWARE_MSG_ID_CROWDSOURCE_MANAGER_MAX           = LM_MIDDLEWARE_MSG_ID_CROWDSOURCE_MANAGER_CONTROL_IND,

  LM_MIDDLEWARE_MSG_ID_GNSS_MIN = 20000,
  LM_MIDDLEWARE_MSG_ID_GNSS_EPHEMERIS_REPORT_0 = LM_MIDDLEWARE_MSG_ID_GNSS_MIN, //deprecated
  LM_MIDDLEWARE_MSG_ID_GNSS_EPHEMERIS_REPORT_1, //Addition of GPS System Time to Ephemeris Report
  LM_MIDDLEWARE_MSG_ID_GNSS_EPHEMERIS_REPORT = LM_MIDDLEWARE_MSG_ID_GNSS_EPHEMERIS_REPORT_1,
  LM_MIDDLEWARE_MSG_ID_GNSS_EVENT_REPORT,
} loc_middleware_msg_id_e_type;

#define LOC_MIDDLEWARE_TIMER_ID_FIX  (2)
#define LOC_MIDDLEWARE_TIMER_ID_XTRA (3)
#define LOC_MIDDLEWARE_TIMER_ID_PA   (4)
#define LOC_MIDDLEWARE_TIMER_ID_SLIM_RESERVED_MIN IPC_SLIM_TIMER_ID_MIN
#define LOC_MIDDLEWARE_TIMER_ID_SLIM_RESERVED_MAX IPC_SLIM_TIMER_ID_MAX

#define LOC_MIDDLEWARE_TIMER_ID_RESERVED_AON_MIN (10000)
#define LOC_MIDDLEWARE_TIMER_ID_RESERVED_AON_HYSTERISIS (10999)
#define LOC_MIDDLEWARE_TIMER_ID_RESERVED_AON_MAX (LOC_MIDDLEWARE_TIMER_ID_RESERVED_AON_HYSTERISIS)

/*---------------------------------------------------------------------------
@brief
  Function to send IPC to LocMW task for servicing Indication to previous
  QMI_LOC request

@param[in] l_ClientHandle : Loc client handle
@param[in] q_QmiIndMsgID  : QMI_LOC Msg Id
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
   uint32                     q_QmiIndMsgID,
   void                      *p_QmiIndMsg,
   uint32                     q_LocApiStatus
   );

/*---------------------------------------------------------------------------
@brief
  Function to handle QMI_LOC indication to client for previous QMI_LOC request
  which is handled by LocMW directly.

@param[in] p_IpcMsg : IPC Message containing the QMI_LOC indication data

@retval    TRUE    if IPC message is handled successfully
@retval    FALSE   if IPC message is handled unsuccessfully
---------------------------------------------------------------------------*/
boolean locMW_HandleQmiIndication
(
   const os_IpcMsgType *p_IpcMsg
   );

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
   void                      *p_LocCmdData
   );

/*---------------------------------------------------------------------------
@brief
  Function to handle LocMW command

@param[in] p_IpcMsg : IPC Message containing the LocMW command data

@retval    TRUE    if IPC message is handled successfully
@retval    FALSE   if IPC message is handled unsuccessfully
---------------------------------------------------------------------------*/
boolean locMW_HandleLocCmd
(
   const os_IpcMsgType *p_IpcMsg
   );

#ifdef __cplusplus
}
#endif


#endif // FEATURE_LOC_API_INTERNAL_H
