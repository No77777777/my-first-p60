/******************************************************************************
  @file:  loc_pd.h
  @brief: Location Middleware PD module header

  DESCRIPTION
      This module handles the interaction with the PD-engine of GPS.

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2018 Qualcomm Technologies, Inc.
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
05/25/18   sum      Translating SV info directly from PDAPI to QMI
05/04/17   yh       Debug NMEA
05/19/16   yh       LocMW refactoring
09/03/15   yh       Add loc_fix_criteria_rule to hold the best fix criteria for multiple client
03/24/12   ss       Supporting operation mode settting to effect Geofencing operation mode
03/10/14   ah       Added support for Get Avail WWAN Pos
03/14/13   ssu      Adding support for SUPL MT ES requests.
04/27/11   spn      Updated for Loc-Api 2.0
$Id: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_pd.h#1 $
$DateTime: 2019/10/24 01:30:36 $
======================================================================*/


#ifndef LOC_PD_H
#define LOC_PD_H

#include "loc_api_2.h"
#include "pdapi.h"
#include "loc_client.h"
#include "aries_os_api.h"

// sometimes the engine would burst at least two measurement reports within very short time
// as every measurement report is converted into a set of NMEA sentences, we have to be able
// to buffer at least 2 sets
#define NMEA_BUFFER_MAX_SIZE 20

typedef enum  {
  LOC_SUPL_MT_IDLE,
  LOC_SUPL_MT_PO_ABOUT_TO_START,
  LOC_SUPL_MT_PO_IN_PROGRESS,
  LOC_SUPL_MT_ES_ABOUT_TO_START,
  LOC_SUPL_MT_ES_IN_PROGRESS
}loc_supl_mt_state_e_type;

/* Defines the payload of the LOC_CMD_TYPE_REQUEST_FIX IPC
   It enumerates the different conditions when the locMW
   process_fix_request can be called.
*/
typedef enum
{
  eLOC_PD_START_REQUEST,
  eLOC_PD_STOP_REQUEST,
  eLOC_PD_SCHEDULE,
  eLOC_PD_FIX_TIMEOUT
}loc_pd_fix_request_e_type;

typedef enum
{
  LOC_SUPL_REQ_ACCEPT,
  LOC_SUPL_REQ_DENY,
  LOC_SUPL_REQ_DROP
}loc_supl_req_e_type;

typedef enum
{
  LOC_ENGINE_STATE_ON  = 1,
  LOC_ENGINE_STATE_OFF = 2,
  LOC_ENGINE_STATE_MAX = 0x10000000 /* used to force enum to 32-bit */
} loc_engine_state_e_type;

typedef struct
{
  loc_client_handle_type l_ClientHandle; /* Client which requested the Inject Position */
  boolean                v_InProgress; /* Variable to serialize the Inject Pos request
                                            (current implementation does not support concurrent requests) */
} locPd_InjectPosType;

typedef struct
{
  loc_client_handle_type l_ClientHandle; /* Client which requested the Inject Wifi AP Scan Info */
  boolean                v_InProgress; /* Variable to serialize the Wifi AP Scan injection request
                                            (current implementation does not support concurrent requests) */
} locPd_InjectWifiApScanType;

typedef struct
{
  loc_client_handle_type l_ClientHandle; /* Client which requested the Best Available Position */
  boolean                v_InProgress; /* Variable to serialize the Best Avail Pos request
                                            (current implementation does not support concurrent requests) */
  uint32                 q_TxnId;        /* Transaction Id */
} locPd_GetBestAvailPosType;

typedef struct
{
  loc_client_handle_type l_ClientHandle; /* Client which requested the Available WWAN Position */
  boolean                v_InProgress; /* Variable to serialize the Avail WWAN Pos request
                                            (current implementation does not support concurrent requests) */
  uint32                 q_TxnId;        /* Transaction Id */
} locPd_GetAvailWwanPosType;

typedef struct
{
  boolean                 fix_in_progress;
  boolean                 mo_fix_in_progress;
  uint64                  last_get_position_time;
  uint64                  last_fix_arrival_time;
  uint32                  last_fix_ttf;
  loc_supl_mt_state_e_type    supl_mt_fix_state;
  pdsm_pd_option_s_type       pdsm_option;
  pdsm_pd_qos_type            pdsm_qos;
  boolean                     new_pd_info_received;
  boolean                     new_interm_pos_info_received;
  int32                       new_nmea_messages_counter;
  boolean                     new_nmea_info_received;
  boolean                     new_pd_sess_end_status_received;
  boolean                     new_pd_cmd_err_received;
  pdsm_pd_info_s_type         pd_info, pd_info_copy;
  pdsm_ext_status_info_s_type ext_status_info, ext_status_info_copy;
  pdsm_ext_status_info_s_type interm_pos_info, interm_pos_info_copy;
  pdsm_pd_end_e_type          pd_sess_end_status, pd_sess_end_status_copy;
  pdsm_pd_cmd_err_e_type      pd_cmd_err, pd_cmd_err_copy;
  pdsm_pd_event_f_type       *pd_event_cb_hook;

  os_TimerMsgType *fix_process_timer;

  loc_client_handle_type      l_ActiveClientHandle;
  loc_client_handle_type      motion_data_inject_client_handle;
  loc_engine_state_e_type     engine_state;
  qmiLocEventPositionReportIndMsgT_v02    position_payload;
  qmiLocEventGnssSvInfoIndMsgT_v02        gnss_payload;
  // Cached cell id position or GPS fix
  qmiLocEventPositionReportIndMsgT_v02   cached_gnss_pos;
  uint64                                 cached_gnss_pos_time;

  qmiLocEventPositionReportIndMsgT_v02   cached_nw_ref_pos;
  uint64                                 cached_nw_ref_pos_time;

  // timeout value used by loc mw layer, if a position report is not received
  // within the timeout value, loc mw will reset its state and allow subsequent
  // pdsm_get_position calls
  uint32                     session_time_out_ms;

  locPd_InjectPosType        z_InjPos;
  locPd_InjectWifiApScanType z_InjWifiApScan;
  locPd_GetBestAvailPosType  z_GetBestAvailPos;
  locPd_GetAvailWwanPosType  z_GetAvailWwanPos;

  /* client which requested the secure available  position */
  locPd_GetSecureAvailPosType   z_GetSecureAvailPos;

  /* client which is currently handling the wifi ap scan info injection */
  loc_client_handle_type     wifi_ap_scan_client_handle;

  /* variable to serialize the wifi ap scan injection one client at a time */
  boolean                    wifi_ap_scan_in_progress;

  /* hold the best fix criteria for multiple clients */
  locClnt_FixCriteriaType    loc_fix_criteria_rule;

  /* client which is currently handling the wifi ap scan info injection */
  loc_client_handle_type     srn_ap_scan_client_handle;

  /* End session because of a power mode upgrade switch */
  boolean                 v_EndSessionForPowerModeSwitch;

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
  /* Flag to track enabling of test mode position injection */
  boolean enable_pos_inj_test_mode;

  /* Injected test position cache */
  qmiLocInjectTestPositionReqMsgT_v02 cached_gnss_test_position;

  /* Validity of injected test position */
  boolean cached_gnss_test_position_valid;
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

  boolean                     new_unpropagated_pos_info_received;
  pdsm_ext_status_info_s_type unpropagated_pos_info, unpropagated_pos_info_copy;

} locPd_ModuleDataType;

/* Initialize the location position determination module */
boolean locPd_Init(void);

/* Initialize the location NMEA sentence module */
boolean locPd_InitNmea(void);

// Translate fix configuration from PD api to location API
void locPd_ConvertFixCfg2Pdapi(loc_client_info_s_type *pz_ClientInfo);

// Process cmd request coming from the location middleware task
int locPd_ProcessCmdRequest(const LocMW_LocCmdStructT *pz_Cmd);

// Function to be called from loc client module to process cell id fix request from cached position
void loc_pd_process_concurrent_cellid_request(void);

// Installs an event callback hook for PD events (including QWIP events)
pdsm_client_status_e_type loc_pd_install_event_cb_hook(pdsm_pd_event_f_type *hook);

// Handles the service interaction for CP/UP, NI/MO.
void loc_pd_handle_service_interaction(pdsm_lcs_notification_resp_s_type *resp_info_ptr);

loc_supl_req_e_type loc_pd_continue_processing_supl_request
(const pdsm_lcs_supl_notify_verify_req_s_type *pz_PdapiSuplReq);

//processes a pd_fix_request
int loc_pd_process_fix_request(loc_pd_fix_request_e_type fix_request_type);

//end a session
void locPd_endSession(pdsm_pd_end_session_opt_s_type z_EndSessionOpt);


/** locPd_EventWifiApScanInfoInjectIndHandler
 *  
 *  Handle to send QMI Event IND to the client from LocMW.
 *  
 *  @param [in] pz_IpcMsg  Pointer to ipc message containing the
 *         indication
 *
 * @return 
 * TRUE - if the QMI IND has been sent successfully TRUE 
 * FALSE - if the QMI IND send failed 
 * */
void locPd_EventWifiApScanInfoInjectIndHandler(const os_IpcMsgType *pz_IpcMsg);


/** locPd_InjectWifiApScanInfoIndHandler
 *  
 *  Handle to send QMI IND to the client from LocMW.
 *  
 *  @param [in] pIpcMsg  Pointer to ipc message containing the
 *         indication
 *
 * @return 
 * TRUE - if the QMI IND has been sent successfully TRUE 
 * FALSE - if the QMI IND send failed 
 * */
boolean locPd_InjectWifiApScanInfoIndHandler(const os_IpcMsgType *pIpcMsg);


/**
 * Function to handle QMI command from control point to inject 
 * wifi ap data 
 *
 * @param pz_ClientInfo LocAPI client handle
 * @param pz_Req        QMI request
 * @param pz_Resp       QMI response
 *
 * @return LOC_API_SUCCESS if the command has been handled successfully
 */
boolean locPd_ProcessInjectWifiApDataReq(loc_client_info_s_type                   *pz_ClientInfo,
                                         const qmiLocInjectWifiApDataReqMsgT_v02  *pz_Req,
                                         qmiLocGenRespMsgT_v02                    *pz_Resp);

/* Short Range Node (SRN) BTLE loc_pd Api's*/

/** locPd_EventSrnApScanInfoInjectIndHandler
 *  
 *  Handle to send QMI Event IND to the client from LocMW.
 *  
 *  @param [in] pIpcMsg  Pointer to ipc message containing the
 *         indication
 *
 * @return 
 * TRUE - if the QMI IND has been sent successfully TRUE 
 * FALSE - if the QMI IND send failed 
 * */
void locPd_EventSrnApScanInfoInjectIndHandler(const os_IpcMsgType *pz_IpcMsg);

/** locPd_InjectSrnApScanInfoIndHandler
 *  
 *  Handle to send QMI IND to the client from LocMW.
 *  
 *  @param [in] pIpcMsg  Pointer to ipc message containing the
 *         indication
 *
 * @return 
 * TRUE - if the QMI IND has been sent successfully TRUE 
 * FALSE - if the QMI IND send failed 
 * */
boolean locPd_InjectSrnApScanInfoIndHandler(const os_IpcMsgType *pIpcMsg);

/**
 * Function to handle QMI command from control point to inject  
 * Srn ap data 
 *
 * @param pz_ClientInfo LocAPI client handle
 * @param pz_Req        QMI request
 * @param pz_Resp       QMI response
 *
 * @return LOC_API_SUCCESS if the command has been handled successfully
 */
boolean locPd_ProcessInjectSrnApDataReq(loc_client_info_s_type                   *pz_ClientInfo,
                                        const qmiLocInjectSrnApDataReqMsgT_v02   *pz_Req,
                                        qmiLocGenRespMsgT_v02                    *pz_Resp);


/**
 * Function to to translate fix mode from Loc API to PD API.
 *
 * @param loc_operation_mode operation mode in loc api format
 *
 * @return operation mode in pdapi format
 */
pdsm_pd_session_operation_e_type  loc_pd_xlate_fix_mode_from_locapi_to_pdapi
(
   qmiLocOperationModeEnumT_v02    loc_operation_mode
   );


/**
 * Function to process QMI_LOC request from client that belongs to PD module
 * (related to PDAPI Position Determination module)
 *
 * @param[in] l_ClientHandle  : Loc API client handle
 * @param[in] q_QmiLocMsgId   : QMI_LOC Request Message ID
 * @param[in] p_QmiLocMsgData : QMI_LOC Request Message Data
 * @param[in] q_QmiLocMsgLen  : QMI_LOC Request Message Data Length
 * @param[in/out] pe_QmiErr   : QMI_LOC Response error value
 *
 * @retval    TRUE   Called successfully
 * @retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locPd_ProcessQmiRequest
(
   loc_client_handle_type l_ClientHandle,
   uint32                 q_QmiLocMsgId,
   const void *p_QmiLocMsgData,
   uint32                 q_QmiLocMsgLen,
   qmi_error_type_v01 *pe_QmiErr
   );

#endif // LOC_PD_H
