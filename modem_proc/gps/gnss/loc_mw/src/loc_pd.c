/******************************************************************************
  @file:  loc_pd.c
  @brief: Location Middleware PA module

  DESCRIPTION
  This module configures/retrieves the NV setting of location
  engine on the modem, and processes EFS data.

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2010-2012 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  Copyright (c) 2013-2014 QUALCOMM Atheros Incorporated.
  All Rights Reserved.QUALCOMM Atheros Proprietary and Confidential.
  Copyright (c) 2015-2025 Qualcomm Technologies, Inc. All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc. 
  -----------------------------------------------------------------------------
 ******************************************************************************/

/*=====================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      -------------------------------------------------------
06/18/24   bpk      FR99502:Incorporating latest location fix in emergency INVITE
07/08/20   skm     ShortCodes SpecialRequest support for START_REQ and GET_BEST_POS 
09/09/19   nath     Almanac and ephimeris flag reporting in NAVIC
04/25/19   yh       register PDSM_EXT_STATUS_EVENT_MASK in locpd_Init
03/05/19   nath     Report bang5 PGA gain in ME metrics
01/24/19   nath     Added support for ME metrics
12/21/18   skm      Provide UE WIFI Mac Address in LPPe
11/05/18   sum      Corrected QMI time source mapping for PDSM_GNSS_TIME_SRC_RESERVED.
09/25/18   py       Semi major axis length will be maximum of length along axis and along perpendicular
09/07/18   py       QMI Pos report have hor_unc_ellipse_orient_azimuth with full precision
06/17/18   sj       Added custom SLP for MO AGNSS requests.
05/30/18   py       change data type of velHor & velVert in extended report to float for precision
05/25/18   sum      Translating SV info directly from PDAPI to QMI
04/06/18   sum      Added code to clear in_progress flag in LocMw context
01/24/18   sum      Populated timeUnc field in parsed position report.
02/02/18   sum      Populating vert_unc with raw value.
02/07/18   yh       Do not allow pdsm_get_position when mo_fix_in_progress is true
10/30/17   nath     Updated data types of loc_unc_horizontal,alongAxisUnc and perpAxisUnc to float
06/12/17   kpn      Correction in F3 for DOP info
05/04/17   yh       Debug NMEA
04/27/17   sum      Populating Horz speed in QMI packet with new velHorU
03/31/17   yh       LocMW clean up round 2
03/08/17   muk      free proper encoded memory block in locPd_EncodeGetAvailableGnssPos
03/08/17   yh       set (mo_)fix_in_progress flag for concurrent E911 feature
02/17/17   skm      BLE and Barometer support for LPPE Phase 2 
12/09/16   yh       Fix for over boundary memory reading in memescpy
11/16/16   yh       Lppe DBH feature implementation
11/15/16   yh       Remove big local varialbe in locPd_SendQmiIndicationProxy
10/27/16   yh       wifiApInfo_len = 0 is a valid input
10/25/16   yh       add sv_mask print out
10/25/16   yh       Do not force session end when there is an active NI session
10/21/16   yh       Add GAL to calculate position
10/06/16   yh       QMI_LOC_SECURE_GET_AVAILABLE_POSITION_REQ_V02 clean up
09/14/16   yh       LocMW only end MO sessions
07/28/16   yh       altitude_wrt_ellipsoid converting fix
07/15/16   yh       heading_unc rad to degree converting fix
06/08/16   yh       Lppe
05/19/16   yh       LocMW refactoring
04/26/16   yh       Single shot fix criteria bug fix
04/13/16   yh       Always update GPS engine state
03/23/16   yh       multiple client position report
10/06/15   yh       QZSS
09/03/15   yh       Use the fatest TBF for pdsm_get_position option
08/26/15   pa       Modifying leap second to 17 and removing redundant constants
06/11/15   sjk      LB 2.0 Integration
03/24/12   ss       Supporting operation mode settting to effect Geofencing operation mode
03/10/14   ah       Added support for Get Avail WWAN Pos
03/14/13   ssu      Support for SUPL MT ES and improved SI rules.
04/11/11   rh       Fixed race condition issue when calling pdsm_client_init()
03/29/10   ns       Changed Loc MW to be an OEM type client

  $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_pd.c#7 $
  $DateTime: 2025/06/19 05:09:19 $
  $Author: pwbldsvc $

======================================================================*/

#include "target.h"
#include "queue.h"
#include "time_svc.h"
#include "math.h"
#include "aries_os_api.h"
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
#include "gnss_calendar_util.h"
#include "loc_conv_locEng_qmiLoc.h"
#include "loc_qmi_shim.h"
#include "gnss_common.h"
#include "secapi.h"
#include "location_service_v02.h"

#define LOC_PD_MIN_VELOCTY            0.15 /* 0.15 m/s */

#define PD_DEFAULT_WAIT_TIMEOUT  120000 // 2 minutes

// Number of seconds between January 1, 1970 and January 6, 1980
#define GPS_TO_UTC_DELTA_SECONDS        (3657 * 24 * 60 * 60)
#define SECONDS_IN_ONE_DAY   (24 * 60 * 60)

#define LAT_LON_SCALE_FACTOR     100000000 // (See 80-VF767-1)

#define UNCERTAINTY_TABLE_SIZE   30
#define INV_SQRT_2               0.7071067811865

/* offset (in meters) that should be subtracted from altitude sent
   in the pdsm position report to calculate the altitude w.r.t
   WGS-84 ellipsoid  */
#define LOC_PD_ALTITUDE_WRT_WGS84_ELLIPSOID_OFFSET (500.0F)

#define SESSION_END_THRESHOLD                (10*1000)  /* 10 seconds */

#define LOC_PD_SECURE_DEBUG   0

/* GLONASS frequency number + 8 */
#define LOC_PD_GLO_FREQ(x)  ((x) + 8)

/* The following structure defines the a "cookie"
   whose address can be passed to any loc_pd commands,
   the cookie will be echoed back when the corresponding command callback
   is called from PDAPI. The information may be used to identify the
   client handle, ioctl and any other command specific information for
   the PDAPI command.
   Note: The cookie may not be echoed back in the event callback, it should
   not be relied upon from the event callback */

typedef struct locPd_CmdCbType
{
  loc_client_handle_type  l_ClientHandle; /* LocMW client handle */
  pdsm_pd_cmd_e_type      e_CmdType;      /* PDSM PD Command Type */
  uint32                  q_QmiLocIndId;  /* QMI_LOC Indication MsgId */
  struct locPd_CmdCbType *p_Self;         /* Self referential pointer to check validity */
} locPd_CmdCbData;

/* Table that maps the PDAPI position source to QMI LOC technology mask */
static const struct {
  pdsm_pd_positioning_source_type pdapiPosSource;
  qmiLocPosTechMaskT_v02 qmiLocPostechMask;
}loc_pd_technology_mask_map[] = {

  /* GPS was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_GPS,
    QMI_LOC_POS_TECH_MASK_SATELLITE_V02 },

  /* GLONASS was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_GLO,
    QMI_LOC_POS_TECH_MASK_SATELLITE_V02 },

  /* CELLID was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_CELLID,
    QMI_LOC_POS_TECH_MASK_CELLID_V02 },

  /* WIFI was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_WIFI,
    QMI_LOC_POS_TECH_MASK_WIFI_V02 },

  /* Sensors were used to calculate position */
  { PDSM_PD_POSITION_SOURCE_SENSORS,
    QMI_LOC_POS_TECH_MASK_SENSORS_V02 },

  /* Reference location was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_DEFAULT,
    QMI_LOC_POS_TECH_MASK_REFERENCE_LOCATION_V02 },

  /* Injected position was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_EPI,
    QMI_LOC_POS_TECH_MASK_INJECTED_COARSE_POSITION_V02 },

  /* HYBRID position */
  { PDSM_PD_LOCATION_SOURCE_HYBRID,
    QMI_LOC_POS_TECH_MASK_HYBRID_V02 },

  /* AFLT was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_AFLT,
    QMI_LOC_POS_TECH_MASK_AFLT_V02 },

  /* BDS was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_BDS,
    QMI_LOC_POS_TECH_MASK_SATELLITE_V02 },

  /* GAL was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_GAL,
    QMI_LOC_POS_TECH_MASK_SATELLITE_V02 },

  /* NavIC was used to calculate position */
  { PDSM_PD_POSITION_SOURCE_NAVIC,
    QMI_LOC_POS_TECH_MASK_SATELLITE_V02 },

};


/* struct that carries the client handle and QMI indication to be sent */
typedef struct
{
  loc_client_handle_type client_handle;
  qmiLocInjectWifiApDataIndMsgT_v02 ackWifiApInd;
}loc_pd_ack_wifi_ap_scan_info_inject_ind_struct_t;

extern void sm_GetGpsUtcOffset(uint8 *p_UtcOffset);

// TBD: Skip the intermediate report between intervals when the last fix is good
/* Event callback called from PDAPI for a PD command.
   The implementation will rely on the UserData pointer to identify the client handle etc,
   and will lock the critical section. */
static void locPd_EventCb
(
   void *p_UserData,
   pdsm_pd_event_type         q_PdEvent,
   const pdsm_pd_info_s_type *p_PdInfo
   );

/* Command callback called from PDAPI for a PD command.
   The implementation will rely on the DataBlock pointer to identify the client handle etc,
   and will lock the critical section. */
static void locPd_CmdCb
(
   void *p_DataBlock,
   pdsm_pd_cmd_e_type     e_CmdType,
   pdsm_pd_cmd_err_e_type e_CmdErrStatus
   );

/* Command callback called from PDAPI for a PD command.
   The implementation will rely on the DataBlock pointer to identify the client handle etc,
   and will not lock the critical section. */
static void locPd_CmdCbUnlocked
(
   void *p_DataBlock,
   pdsm_pd_cmd_e_type     e_CmdType,
   pdsm_pd_cmd_err_e_type e_CmdErrStatus
   );

static void locPd_ExtEventCb
(
   void *p_DataBlock,
   pdsm_ext_status_event_type         q_ExtStatusCmd,
   const pdsm_ext_status_info_s_type *pz_ExtStatusInfo
   );

static boolean locPd_ReportPosition(void);
static void locPd_Schedule(void);

static boolean locPd_GetReport
(
   qmiLocEventPositionReportIndMsgT_v02 *pz_PositionReportIndMsg,
   qmiLocEventGnssSvInfoIndMsgT_v02 *pz_GnssSvInfoIndMsg
   );

/* Function to process external coarse position injection request. */
static int locPd_InjectExtPos
(
   loc_client_handle_type                  l_ClientHandle,
   const qmiLocInjectPositionReqMsgT_v02 *pz_PosReq
   );

static qmiLocSessionStatusEnumT_v02 locPd_XlateFixEndReasonCode
(
   pdsm_pd_end_e_type  e_PdSessEndStatus
   );

static qmiLocSessionStatusEnumT_v02 locPd_XlateCmdErrCodeToSessStatus
(
   pdsm_pd_cmd_err_e_type  e_PdCmdErr
   );

/* Function to translate PD cmd error into session status in the LOC API position report. */
static uint32 locPd_ConvertCmdErr2LocStatus
(
   pdsm_pd_cmd_err_e_type e_PdCmdErr
   );

/* Function to process the best available position get request. */
static int locPd_GetBestAvailPos
(
   loc_client_handle_type                           l_ClientHandle,
   const qmiLocGetBestAvailablePositionReqMsgT_v02 *pz_BestAvailPosReq
   );

/* Function to process the available WWAN position get request. */
static int locPd_GetAvailWwanPos
(
   loc_client_handle_type                       l_ClientHandle,
   const qmiLocGetAvailWwanPositionReqMsgT_v02 *pz_AvailWwanPosReq
   );

/* Function to process the available position get request. */
static int locPd_GetAvailPos
(
   loc_client_handle_type                             l_ClientHandle,
   const qmiLocSecureGetAvailablePositionReqMsgT_v02 *pz_AvailPosReq
   );

/* Inject GSM cellular information into the engine */
static int locPd_InjectGsmCellInfo
(
   loc_client_handle_type                    l_ClientHandle,
   const qmiLocInjectGSMCellInfoReqMsgT_v02 *pz_GsmCellInfoReq
   );

/* Inject NI Message into the engine */
static int locPd_InjectNiMsg
(
   loc_client_handle_type                                l_ClientHandle,
   const qmiLocInjectNetworkInitiatedMessageReqMsgT_v02 *pz_NiMsgReq
   );

/* Send WWAN OOS notification to the engine */
static int locPd_WwanOosNotification
(
   loc_client_handle_type l_ClientHandle
   );

/**
 * called by PD task to send QMI Event Ind to LocMW 
 * task 
 *
 *  @return -- TRUE is IPC was sent successfully
 *          -- FALSE otherwise.
 */
static boolean locPd_EventWifiApScanInfoInjectIndProxy
(
   const pdsm_pd_wifi_info_on_demand_injection_req_s_type *pz_WifiApInfoReqInd
   );


/**
 * called by PD task to send indication to LocMW task 
 *
 * @param client_handle
 *               LocAPI client handle
 * @param pz_WifiInjAck
 *               Response/Ack indication sent from PDSM in
 *               locEng format
 *
 * @return 
 * TRUE - if IPC posted to LocMW success 
 * TRUE - if IPC posted to LocMW failed 
 */
static boolean locPd_AckWifiApScanInfoInjectIndProxy(loc_client_handle_type                     l_ClientHandle,
                                                     const locEngInjectWifiApDataIndMsgT_v01 *pz_WifiInjAck);

/**
 * called by PD task to send QMI Event Ind to LocMW 
 * task for Srn AP scan information
 *
 *  @return -- TRUE is IPC was sent successfully
 *          -- FALSE otherwise.
 */
static boolean locPd_EventSrnApScanInfoInjectIndProxy
(
   const locEngEventInjectSrnApDataReqIndMsgT_v01 *pz_SrnApInfoReqInd
   );

/**
 * called by PD task to send indication to LocMW task 
 *
 * @param pz_SrnInjAck
 *               Response/Ack indication sent from PDSM in
 *               locEng format
 *
 * @return 
 * TRUE - if IPC posted to LocMW success 
 * TRUE - if IPC posted to LocMW failed 
 */
static boolean locPd_AckSrnApScanInfoInjectIndProxy
(
   const locEngInjectSrnApDataIndMsgT_v01 *pz_SrnInjAck
   );

/* Initialize the location position determination module */
boolean locPd_Init()
{
  boolean  ret_val = FALSE;
  pdsm_client_status_e_type status = PDSM_CLIENT_OK;
  pdsm_pd_event_type pd_event_mask = PDSM_PD_EVENT_MASK;
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  uint8 i = 0;

  do
  {
    pz_LocPdData->fix_in_progress     = FALSE;
    pz_LocPdData->mo_fix_in_progress  = FALSE;
    pz_LocPdData->supl_mt_fix_state = LOC_SUPL_MT_IDLE;
    pz_LocPdData->z_InjPos.v_InProgress          = FALSE;
    pz_LocPdData->z_InjWifiApScan.v_InProgress   = FALSE;
    pz_LocPdData->z_GetBestAvailPos.v_InProgress = FALSE;
    pz_LocPdData->z_GetAvailWwanPos.v_InProgress = FALSE;
    pz_LocPdData->last_get_position_time = 0;
    pz_LocPdData->last_fix_arrival_time  = 0;
    pz_LocPdData->last_fix_ttf           = 0;

    pz_LocPdData->new_pd_info_received            = FALSE;
    pz_LocPdData->new_interm_pos_info_received    = FALSE;
    pz_LocPdData->new_pd_sess_end_status_received = FALSE;
    pz_LocPdData->new_pd_cmd_err_received         = FALSE;
    pz_LocPdData->session_time_out_ms             = 0;

    // Initialization (cannot be omitted)
    pz_LocPdData->pd_event_cb_hook                = NULL;

    memset(&(pz_LocPdData->pdsm_option), 0, sizeof(pdsm_pd_option_s_type));
    memset(&(pz_LocPdData->pdsm_qos), 0, sizeof(pdsm_pd_qos_type));
    memset(&(pz_LocPdData->pd_info), 0, sizeof(pdsm_pd_info_s_type));
    memset(&(pz_LocPdData->pd_info_copy), 0, sizeof(pdsm_pd_info_s_type));
    memset(&(pz_LocPdData->ext_status_info), 0, sizeof(pdsm_ext_status_info_s_type));
    memset(&(pz_LocPdData->ext_status_info_copy), 0, sizeof(pdsm_ext_status_info_s_type));

    memset(&(pz_LocPdData->cached_gnss_pos), 0, sizeof(pz_LocPdData->cached_gnss_pos));
    memset(&(pz_LocPdData->cached_nw_ref_pos), 0, sizeof(pz_LocPdData->cached_nw_ref_pos));
    pz_LocPdData->cached_gnss_pos_time = 0;
    pz_LocPdData->cached_nw_ref_pos_time = 0;

    pz_LocPdData->l_ActiveClientHandle             = LOC_CLIENT_HANDLE_INVALID;
    pz_LocPdData->z_InjPos.l_ClientHandle          = LOC_CLIENT_HANDLE_INVALID;
    pz_LocPdData->z_InjWifiApScan.l_ClientHandle   = LOC_CLIENT_HANDLE_INVALID;
    pz_LocPdData->z_GetBestAvailPos.l_ClientHandle = LOC_CLIENT_HANDLE_INVALID;
    pz_LocPdData->z_GetAvailWwanPos.l_ClientHandle = LOC_CLIENT_HANDLE_INVALID;
    pz_LocPdData->engine_state = LOC_ENGINE_STATE_OFF;
    pz_LocPdData->v_EndSessionForPowerModeSwitch = FALSE;

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
    memset(&(pz_LocPdData->cached_gnss_test_position), 0, sizeof(pz_LocPdData->cached_gnss_test_position));
    pz_LocPdData->enable_pos_inj_test_mode = FALSE;
    pz_LocPdData->cached_gnss_test_position_valid = FALSE;
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

    /* the fix timer should be non deferrable, i.e it should always wakeup the
       engine from suspended state */

    pz_LocPdData->fix_process_timer =
       os_TimerCreateInCgpsTimerGroup((uint32)LOC_MIDDLEWARE_TIMER_ID_FIX,
                                      (uint32)THREAD_ID_LOC_MIDDLEWARE);

    if (loc_middleware_data.pdapi_client_id == -1)
    {
      loc_middleware_data.pdapi_client_id = pdsm_client_init(PDSM_CLIENT_TYPE_PDA);
    }

    if (loc_middleware_data.pdapi_client_id == -1)
    {
      LOC_MSG_ERROR("locPd_Init: pdsm_client_init failed", 0, 0, 0);
      break;
    }

    pd_event_mask |= PDSM_PD_EVENT_WPS_NEEDED;

    status = pdsm_client_pd_reg(loc_middleware_data.pdapi_client_id,
                                NULL,
                                locPd_EventCb,
                                PDSM_CLIENT_EVENT_REG,
                                pd_event_mask,
                                NULL);
    if (status != PDSM_CLIENT_OK)
    {
      LOC_MSG_ERROR("locPd_Init: pdsm_client_pd_reg failed, error code=%d", status, 0, 0);
    }

    status = pdsm_client_ext_status_reg(loc_middleware_data.pdapi_client_id,
                                        NULL,
                                        locPd_ExtEventCb,
                                        PDSM_CLIENT_EVENT_REG,
                                        PDSM_EXT_STATUS_EVENT_MASK,
                                        NULL);
    if (status != PDSM_CLIENT_OK)
    {
      LOC_MSG_ERROR("locPd_Init: pdsm_client_ext_status_reg failed, error code = %d", status, 0, 0);
    }

    status = pdsm_client_act(loc_middleware_data.pdapi_client_id);
    if (status != PDSM_CLIENT_OK)
    {
      LOC_MSG_ERROR("locPd_Init: pdsm_client_act failed, error code =%d", status, 0, 0);
      break;
    }

    ret_val = TRUE;
  } while (0);

  return ret_val;
}

pdsm_client_status_e_type loc_pd_install_event_cb_hook(pdsm_pd_event_f_type *hook)
{
  pdsm_client_status_e_type e_clientStatus = PDSM_CLIENT_OK;
  if (loc_middleware_data.z_locPd_Data.pd_event_cb_hook == NULL)
  {
    loc_middleware_data.z_locPd_Data.pd_event_cb_hook = hook;
  }
  else
  {
    e_clientStatus = PDSM_CLIENT_ERR_CLIENT_ACTIVE;
  }
  return e_clientStatus;
}

/* Initialize the location NMEA sentence module */
boolean locPd_InitNmea()
{
  pdsm_client_status_e_type status;

  status = pdsm_client_ext_status_reg(loc_middleware_data.pdapi_client_id,
                                      NULL,
                                      locPd_ExtEventCb,
                                      PDSM_CLIENT_EVENT_REG,
                                      PDSM_EXT_STATUS_EVENT_MASK,
                                      NULL);
  if (status != PDSM_CLIENT_OK)
  {
    LOC_MSG_ERROR("locPd_InitNmea: nmea registration failed, error code = %d", status, 0, 0);
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

// This function is used to process cmd request from location task.
int locPd_ProcessCmdRequest(const LocMW_LocCmdStructT *pz_Cmd)
{
  int ret_val = LOC_API_GENERAL_FAILURE;

  if (NULL != pz_Cmd)
  {
    LocMW_LocCmdEnumT cmd_type = pz_Cmd->e_CmdType;

    if (cmd_type == LOC_CMD_TYPE_PD_SCHEDULE)
    {
      loc_pd_process_fix_request(eLOC_PD_SCHEDULE);
      ret_val = LOC_API_SUCCESS;
    }
    else if (cmd_type == LOC_CMD_TYPE_REPORT_POSITION)
    {
      // Notify the requesting client of any new position data
      (void)locPd_ReportPosition();
      ret_val = LOC_API_SUCCESS;
    }
    else
    {
      // internal error
      ret_val = LOC_API_UNSUPPORTED;
    }
  }
  else
  {
    // internal error
    ret_val = LOC_API_GENERAL_FAILURE;
  }

  if (LOC_API_SUCCESS != ret_val)
  {
    LOC_MSG_ERROR("locPd_ProcessCmdRequest internal error", 0, 0, 0);
  }

  return ret_val;
}

// Function used to do next fix request. This function is responsible for multiplexing
// between multiple apps, set up fix config and issue fix request based on the requesting client.
// The scheduling algorithm is the min (client last fix requst time + fix internal)
int loc_pd_process_fix_request(loc_pd_fix_request_e_type fix_request_type)
{
  int32                     time_to_wait_ms = INT32_MAX;
  int32                     time_delta_ms;
  boolean                   status;

  // initialize variable to avoid warning on calling time_get_uptime_ms
  time_type                 time_ms = { 0 };

  uint64                    current_time_ms;
  loc_client_info_s_type *client_ptr = NULL;
  boolean                   is_any_client_requesting_fix = FALSE;
  boolean                   is_stop_client_match_active = FALSE;
  boolean                   non_mo_fix_in_progress = FALSE;
  boolean                   call_end_session = FALSE;

  //end_session_type
  pdsm_pd_end_session_opt_s_type end_session_opt;

  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  LOC_MSG_MED("loc_pd_process_fix_request: request_type = %d, fix_in_progress = %d mo_fix_in_progress = %d\n",
              fix_request_type,
              pz_LocPdData->fix_in_progress,
              pz_LocPdData->mo_fix_in_progress);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  /* Process a stop request from a MO client*/

  if (eLOC_PD_STOP_REQUEST == fix_request_type)
  {
    /* get wait time for next fix to be queued */
    time_to_wait_ms = locClnt_GetMinWaitTime(&client_ptr);

    /* check if any other active state client in queue */
    is_any_client_requesting_fix = locClnt_IsAnyRequestingFix();

    /* check if client calling stop is same as active client that scheduled ongoing fix */
    if (loc_middleware_data.z_locClnt_Data.l_StopClientHandle == locClnt_GetActiveClient())
    {
      is_stop_client_match_active = TRUE;
    }

    /* check if any non-mo fix is in progress */
    non_mo_fix_in_progress = (TRUE == pz_LocPdData->fix_in_progress &&
                              FALSE == pz_LocPdData->mo_fix_in_progress);

    LOC_MSG_MED("loc_pd_process_fix_request(STOP): next fix wait time %d "
                "is_any_client_requesting_fix %d is_stop_client_match_active %d \n",
                (int32)time_to_wait_ms, is_any_client_requesting_fix, is_stop_client_match_active);

    /* Try to End the session
       if - there is no non-mo-fix-in-progress (!fix_in_progress || mo_fix_in_progress)
             if - there is no other ready state client
             if - there is no fix scheduled within 10sec threshold time
                  &&
                  client handle calling stop is same as active client
    */
    if (FALSE == non_mo_fix_in_progress)
    {
      if (FALSE == is_any_client_requesting_fix)
      {
        LOC_MSG_MED("loc_pd_process_fix_request: Processing stop when"
                    " is_any_client_requesting_fix: %d", is_any_client_requesting_fix, 0, 0);

        call_end_session = TRUE;
      }
      else if ((time_to_wait_ms > SESSION_END_THRESHOLD) && (TRUE == is_stop_client_match_active))
      {
        LOC_MSG_MED("loc_pd_process_fix_request: Processing stop when wait time > THRESHOLD and"
                    " is_stop_client_match_active = %d \n", is_stop_client_match_active, 0, 0);

        call_end_session = TRUE;
      }
      else
      {
        call_end_session = FALSE;
        LOC_MSG_MED("Not stopping the fix, is_any_client_requesting_fix %d,"
                    " wait time > THRESHOLD = %d, "
                    "is_stop_client_match_active = %d\n",
                    is_any_client_requesting_fix,
                    time_to_wait_ms > SESSION_END_THRESHOLD,
                    is_stop_client_match_active);
      }

      if (TRUE == call_end_session)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

        end_session_opt.receiver_off = TRUE;
        end_session_opt.v_reason_service_interaction = FALSE;
        locPd_endSession(end_session_opt);

        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
        os_TimerStop(pz_LocPdData->fix_process_timer);

        pz_LocPdData->mo_fix_in_progress = FALSE;
      }
    }
    else
    {
      /* non-mo-session is in progress (NI session, GF, etc) */
      LOC_MSG_MED("loc_process_fix_request: Not stopping the engine as "
                  "an NI session (or non-mo) is active", 0, 0, 0);
    }
  }
  /* check if fix timed out without getting a position fix */
  else if (eLOC_PD_FIX_TIMEOUT == fix_request_type &&
           TRUE == pz_LocPdData->mo_fix_in_progress)
  {
    os_TimerStop(pz_LocPdData->fix_process_timer);
    // this state is hit because the MO positioning session timed out
    time_get_uptime_ms(time_ms);
    current_time_ms = QW_CVT_Q2N(time_ms);

    time_delta_ms = (int)(current_time_ms - pz_LocPdData->last_get_position_time);

    time_to_wait_ms =  pz_LocPdData->session_time_out_ms - time_delta_ms;

    if (time_to_wait_ms < 0)
    {
      LOC_MSG_ERROR("Missing pd cmd err or pd done event\n", 0, 0, 0);

      // Inject timeout error to the app
      pz_LocPdData->pd_sess_end_status = PDSM_PD_END_TIMEOUT;
      pz_LocPdData->new_pd_sess_end_status_received = TRUE;

      // send error report, as an empty location report, to client
      (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                  LOC_CMD_TYPE_REPORT_POSITION,
                                  0,
                                  NULL);

      // Force state reset
      locPd_Schedule();
      LOC_MSG_MED("loc_process_fix_request: Timeout scheduled a position"
                  "report with timeout status, also re-scheduled "
                  "fix processing", 0, 0, 0);
      time_to_wait_ms = INT32_MAX; // Wait until client state changed
    }
  }
  /*   else if (FALSE == pz_LocPdData->fix_in_progress &&
              FALSE == pz_LocPdData->mo_fix_in_progress ) */
  else if (FALSE == pz_LocPdData->mo_fix_in_progress)
  {
    os_TimerStop(pz_LocPdData->fix_process_timer);
    // In case of back to back periodic fixes.
    // When the second request for periodic fix comes in , make sure we dont have an ongoing fix in progress.
    // If we have an ongoing fix in progress wait till the fix is done , in which case we get a session_done and
    // fix_in_progress is reset to FALSE. And then the scheduler decides which fix request to process next, in
    // which case the start fix request would be honoured.
    if ((eLOC_PD_START_REQUEST == fix_request_type) ||
        ((eLOC_PD_SCHEDULE == fix_request_type) &&
         (TRUE == locClnt_IsAnyRequestingFix())) ||
        (eLOC_PD_FIX_TIMEOUT == fix_request_type)
        )
    {
      /* Make a new position request if:
         a) A new start request was received
         b) A schedule request was received and there is atleast
            one MO client active
         c) The fix timer expired and no fix is in progress */

      // Let the GPS client manager picks the gps configuration to be used.
      // It will also figure out when pdsm_get_postion will be called.
      time_to_wait_ms = locClnt_SetupRequestConfig();
      if (time_to_wait_ms <= 0)
      {
        LOC_MSG_HIGH("calling pdsm_get_position server option 1x %d umts %d mode =%d\n",
                     pz_LocPdData->pdsm_option.lsinfo.server_option,
                     pz_LocPdData->pdsm_option.umts_slp_lsinfo.server_option,
                     pz_LocPdData->pdsm_option.operation_mode);

        pz_LocPdData->mo_fix_in_progress = TRUE;

        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
        status = pdsm_get_position(locPd_CmdCb,
                                   NULL, /* not used */
                                   &(pz_LocPdData->pdsm_option),
                                   &(pz_LocPdData->pdsm_qos),
                                   loc_middleware_data.pdapi_client_id);
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
        if (status == TRUE)
        {
          time_to_wait_ms = PD_DEFAULT_WAIT_TIMEOUT; // wait until position report has received
                                                     // or client state changes
          if (time_to_wait_ms < (pz_LocPdData->pdsm_qos.gps_session_timeout * 1000 + 1000))
          {
            time_to_wait_ms = pz_LocPdData->pdsm_qos.gps_session_timeout * 1000 + 1000;
          }
          // The loc mw will wait for the engine to time out, add a padding of 1 second
          pz_LocPdData->session_time_out_ms = time_to_wait_ms;
        }
        else
        {
          LOC_MSG_ERROR("pdsm_get_postion returned %d \n, queuing an empty"
                        "position report and rescheduled fix processing ",
                        status, 0, 0);
          // send error report, as an empty location report, to client
          (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                      LOC_CMD_TYPE_REPORT_POSITION,
                                      0,
                                      NULL);
          pz_LocPdData->mo_fix_in_progress = FALSE;
          locPd_Schedule();
          time_to_wait_ms = INT32_MAX;
        }
      } // end of if (time_to_wait <= 0)
    } // end of !((eLOC_PD_START_REQUEST == fix_request_type)&&(TRUE == pz_LocPdData->fix_in_progress))
    else
    {
      LOC_MSG_MED("Not making a new position request though no fix in progress",
                  0, 0, 0);
    }
  } // end of  FALSE == pz_LocPdData->mo_fix_in_progress)
  /* This state may be hit if NI fix is in progress and timer expires */
  else
  {
    LOC_MSG_HIGH("loc_pd_process_fix_request : unknown state "
                 "fix_request_type = %d, fix in progress = %d,"
                 "mo fix in progress = %d", fix_request_type,
                 pz_LocPdData->fix_in_progress,
                 pz_LocPdData->mo_fix_in_progress);

    time_to_wait_ms = INT32_MAX;
  }

  LOC_MSG_HIGH("loc_module::time_to_wait_ms = %d \n", time_to_wait_ms, 0, 0);

  if ((time_to_wait_ms > 0) && (time_to_wait_ms != INT32_MAX))
  {
    os_TimerStart(pz_LocPdData->fix_process_timer, time_to_wait_ms, 0);
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  return 1;
}

// Notify client regarding position and sv information.
static boolean locPd_ReportPosition(void)
{
  loc_client_handle_type                l_ActiveClientHandle;
  qmiLocEventPositionReportIndMsgT_v02 *pz_PositionReportIndMsg = NULL;
  qmiLocEventGnssSvInfoIndMsgT_v02 *pz_GnssSvInfoIndMsg = NULL;
  locPd_ModuleDataType *pz_LocPdData = NULL;
  boolean is_unpropagated_fix = FALSE;

  pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  pz_PositionReportIndMsg = &(pz_LocPdData->position_payload);
  pz_GnssSvInfoIndMsg = &(pz_LocPdData->gnss_payload);
  is_unpropagated_fix = pz_LocPdData->new_unpropagated_pos_info_received;

  // the payloads will be memset in locPd_GetReport
  locPd_GetReport(pz_PositionReportIndMsg, pz_GnssSvInfoIndMsg);

  l_ActiveClientHandle = locClnt_GetActiveClient();

  // Set the gnss report first (the one being populated from final position report), thent the final position report, so the client can
  if (l_ActiveClientHandle != LOC_CLIENT_HANDLE_INVALID &&
      (TRUE == pz_GnssSvInfoIndMsg->svList_valid || TRUE == pz_GnssSvInfoIndMsg->expandedSvList_valid))
  {
    (void)locClnt_InvokeCallback(LOC_HANDLE_REGISTERED_CLIENTS, LOC_EVENT_SATELLITE_REPORT, &(pz_LocPdData->gnss_payload));
  }

  if ((l_ActiveClientHandle != LOC_CLIENT_HANDLE_INVALID) && (pz_PositionReportIndMsg->sessionStatus >= 0))
  {
    if (is_unpropagated_fix)
    {
      (void)locClnt_PositionCallback(l_ActiveClientHandle,
                                     LOC_EVENT_PARSED_UNPROPAGATED_POSITION_REPORT,
                                     &(pz_LocPdData->position_payload));
    }
    else
    {
      (void)locClnt_PositionCallback(l_ActiveClientHandle,
                                     LOC_EVENT_PARSED_POSITION_REPORT,
                                     &(pz_LocPdData->position_payload));

      // update the state for single-shot client
      if (pz_PositionReportIndMsg->sessionStatus != eQMI_LOC_SESS_STATUS_IN_PROGRESS_V02)
      {
        (void)locClnt_UpdateStateAfterFix();
      }
    }
  }

  loc_pd_process_concurrent_cellid_request();

  LOC_MSG_HIGH("locPd_ReportPosition, position sessionStatus = %d, sv valid mask = 0x%x",
               pz_PositionReportIndMsg->sessionStatus,
               pz_GnssSvInfoIndMsg->svList->validMask, 0);

  return TRUE;
}

// Report cell id reference position to the pending cell id request if:
// (1) from gnss position (final or intermediate) if less than 2 seconds old
// (2) from cell id reference position from MSB request if less than 30 seconds old
void loc_pd_process_concurrent_cellid_request()
{
  time_type                 time_ms = { 0 };
  uint64                    current_time_ms;
  qmiLocEventPositionReportIndMsgT_v02 *pz_PosPayload = NULL;
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  time_get_uptime_ms(time_ms);
  current_time_ms = QW_CVT_Q2N(time_ms);

  // Report cached network reference position to cell id client
  if ((pz_LocPdData->cached_gnss_pos_time + 2000) > current_time_ms)
  {
    pz_PosPayload = &(pz_LocPdData->cached_gnss_pos);
  }
  else if ((pz_LocPdData->cached_nw_ref_pos_time + 30000) > current_time_ms)
  {
    pz_PosPayload = &(pz_LocPdData->cached_nw_ref_pos);
  }

  if (pz_PosPayload != NULL)
  {
    // Mark it as final report for cell id
    pz_PosPayload->sessionStatus =  eQMI_LOC_SESS_STATUS_SUCCESS_V02;
    locClnt_ReportPositionToCellidRequest(pz_PosPayload, current_time_ms);
  }
}

// Fill up leapSeconds and gpsTime as well
static int8 locPd_FillTimestampUtc(qmiLocEventPositionReportIndMsgT_v02 *pz_PositionReportIndMsg,
                                   const pdsm_pd_data_type *pz_PdData)
{
  int8 b_LeapSeconds = 0;

  if ((NULL == pz_PositionReportIndMsg) ||
      (NULL == pz_PdData))
  {
    LOC_MSG_ERROR("locPd_FillTimestampUtc: pz_PositionReportIndMsg = %p, pz_PdData = %p",
                  pz_PositionReportIndMsg, pz_PdData, 0);
    return 0;
  }

  // Retrieve leap seconds
  if ((pz_PdData->opt_field_mask & PDSM_PD_UTCOFFSET_VALID) != 0)
  {
    pz_PositionReportIndMsg->leapSeconds_valid = TRUE;
    pz_PositionReportIndMsg->leapSeconds = b_LeapSeconds = pz_PdData->gpsUtcOffset;

  }
  else
  {
    sm_GetGpsUtcOffset((uint8 *)&b_LeapSeconds); //GPS_UTC_OFFSET;
    pz_PositionReportIndMsg->leapSeconds = b_LeapSeconds;
  }

  if ((pz_PdData->ext_opt_field_mask & PDSM_PD_GPS_TIME_VALID) != 0)
  {
    pz_PositionReportIndMsg->timestampUtc_valid = TRUE;
    pz_PositionReportIndMsg->timestampUtc = pz_PdData->time_stamp + GPS_TO_UTC_DELTA_SECONDS - b_LeapSeconds;
    pz_PositionReportIndMsg->timestampUtc *= 1000;
    if ((pz_PdData->opt_field_mask & PDSM_PD_MSEC_VALID) != 0)
    {
      pz_PositionReportIndMsg->timestampUtc += pz_PdData->time_stamp_msec;
    }
  }
  else if ((pz_PdData->ext_opt_field_mask & PDSM_PD_UTC_TIME_VALID) != 0)
  {
    /* We cannot calculate the calendar GPS time from UTC time as we dont
       have information for the leap seconds */

    (void)gnss_GetUnixEpochFromUTC(&(pz_PdData->ext_pos_data.utc_time),
                                   &(pz_PositionReportIndMsg->timestampUtc));
    /* Scale it to milliseconds, as the TimeStamp now is in seconds. */
    pz_PositionReportIndMsg->timestampUtc *= 1000;

    /* Now lets add more precision.
       utc_time.sec --> is seconds scaled by 100.
                         ( ie. 1sec is reported as 100 &
                            1.99 sec is reported as 199 which is 1990msec)
       GetUnixEpochFromUTC gives us back in seconds. So we can add the tenths
       of a second and hundredth of a second.
 
       Note: Currently the precision of hundredth of a second is lost in
             translation in SM.
    */
    pz_PositionReportIndMsg->timestampUtc +=
       ((pz_PdData->ext_pos_data.utc_time.sec % 100) * 10);
    /* The UTC Time Stamp is valid */
    pz_PositionReportIndMsg->timestampUtc_valid = TRUE;
  }
  else
  {
    // time is not valid, do nothing
    LOC_MSG_MED("locPd_FillTimestampUtc: time is invalid", 0, 0, 0);
  }

  if ((pz_PdData->ext_pos_data.timestamp_gps_week != 0) &&
      (pz_PdData->ext_pos_data.timestamp_gps_week != C_GPS_WEEK_UNKNOWN))
  {
    pz_PositionReportIndMsg->gpsTime_valid = TRUE;
    pz_PositionReportIndMsg->gpsTime.gpsWeek = pz_PdData->ext_pos_data.timestamp_gps_week;
    pz_PositionReportIndMsg->gpsTime.gpsTimeOfWeekMs = pz_PdData->ext_pos_data.timestamp_tow_ms;
  }
  else
  {
    LOC_MSG_MED("locPd_FillTimestampUtc: no gps time, timestampUtc_valid = %d leapSeconds_valid = %d SMGpsWeek=%u",
                pz_PositionReportIndMsg->timestampUtc_valid, pz_PositionReportIndMsg->leapSeconds_valid, pz_PdData->ext_pos_data.timestamp_gps_week);
  }

  // TBD: time uncertainty, not available yet
  return b_LeapSeconds;
}

static void locPd_dumpSvIn
(
   pdsm_pd_ext_meas_s_type *pz_PdExtMeas
   )
{
  if (NULL != pz_PdExtMeas)
  {
    uint32 q_in;
    LOC_MSG_LOW("locPd_dumpSvIn: --------------------------------", 0, 0, 0);
    LOC_MSG_LOW_2("locPd_dumpSvIn: eph_svmask: %d, alm_svmask: %d", pz_PdExtMeas->eph_svmask, pz_PdExtMeas->alm_svmask);

    for (q_in = 0;
         (q_in < pz_PdExtMeas->num_svs) && (q_in < PDSM_EXT_MEAS_NUM_SVS);
         q_in++)
    {
      const pdsm_pd_meas_s_type *pz_PdSvIn = &(pz_PdExtMeas->ext_meas_report_type[q_in]);
      LOC_MSG_LOW("locPd_dumpSvIn: sv_id: %d, elevation: %d, azimuth: %d", pz_PdSvIn->sv_id, pz_PdSvIn->elevation, pz_PdSvIn->azimuth);
      LOC_MSG_LOW("locPd_dumpSvIn: c_no: %d, sv_state:%d ", pz_PdSvIn->c_no, pz_PdSvIn->sv_state, 0);
    }
  }
}

/*---------------------------------------------------------------------
@brief Utility function to convert PDAPI SV State values to QMI
---------------------------------------------------------------------*/
static qmiLocSvStatusEnumT_v02 locPd_ConvPdsmSvStateToQmi(pdsm_LocSvSearchStatusEnumT e_PdSvState)
{
  switch (e_PdSvState)
  {
    case PDSM_SV_STATE_TRACK:
    case PDSM_SV_STATE_VERIFY_TRACK:
    case PDSM_SV_STATE_DPO_TRACK:
      return eQMI_LOC_SV_STATUS_TRACK_V02;
      break;

    case PDSM_SV_STATE_BIT_EDGE:
    case PDSM_SV_STATE_SEARCH:
    case PDSM_SV_STATE_SEARCH_VERIFY:
    case PDSM_SV_STATE_GLO_10MSEC_BIT_EDGE:
    case PDSM_SV_STATE_LPPM_SEARCH:
      return eQMI_LOC_SV_STATUS_SEARCH_V02;
      break;

    case PDSM_SV_STATE_RESTART:
    case PDSM_SV_STATE_IDLE:
    default:
      return eQMI_LOC_SV_STATUS_IDLE_V02;
      break;
  }
}

/*---------------------------------------------------------------------------
@brief
  Utility function that fills out the deprecated TLV 0x10 (svList) 
  under QMI_LOC_EVENT_GNSS_SV_INFO_IND.
  New TLV expandedSvList should be used instead.

@param [in]   pz_PdExtMeas : Pointer to PDAPI Measurement
@param [out]  pz_GnssSvInfoIndMsg : Pointer QMI SV Info Indication

@retval None
---------------------------------------------------------------------*/
static void locPd_fillDeprecatedTlv_svList
(
   const pdsm_pd_ext_meas_s_type *pz_PdExtMeas,
   qmiLocEventGnssSvInfoIndMsgT_v02 *pz_GnssSvInfoIndMsg)
{
  uint8 q_in, q_out;

  if ((NULL == pz_PdExtMeas) || (NULL == pz_GnssSvInfoIndMsg))
  {
    LOC_MSG_ERROR_1("Null Pointer 0x%X", pz_PdExtMeas);
    return;
  }

  for (q_out = q_in = 0;
       (q_in < pz_PdExtMeas->num_svs) && (q_out < QMI_LOC_SV_INFO_LIST_MAX_SIZE_V02) && (q_in < PDSM_EXT_MEAS_NUM_SVS);
       q_in++)
  {
    const pdsm_pd_meas_s_type *pz_PdSvIn = &(pz_PdExtMeas->ext_meas_report_type[q_in]);
    gnss_sv_id_type                  w_SvId = pz_PdSvIn->sv_id;
    qmiLocSvInfoStructT_v02 *pz_QmiSvOut = &(pz_GnssSvInfoIndMsg->svList[q_out]);

    pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SYSTEM_V02;
    pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_GNSS_SVID_V02;

    //Health in SV Info is always invalid
    pz_QmiSvOut->validMask &= ~QMI_LOC_SV_INFO_MASK_VALID_HEALTH_STATUS_V02;
    pz_QmiSvOut->healthStatus = 0;

    if (w_SvId >= PDSM_PD_SV_ID_GPS_MIN && w_SvId <= PDSM_PD_SV_ID_GPS_MAX) // 1~32
    {
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_GPS_V02;
      pz_QmiSvOut->gnssSvId = w_SvId;

      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;
      if ((pz_PdExtMeas->eph_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_GPS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }
      if ((pz_PdExtMeas->alm_svmask  & (1 << (w_SvId - PDSM_PD_SV_ID_GPS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
    }
    else if (w_SvId >= PDSM_PD_SV_ID_SBAS_MIN && w_SvId <= PDSM_PD_SV_ID_SBAS_MAX) // 33~64
    {
      // SBAS: GPS RPN: 120-151,
      // In exteneded measurement report, we follow nmea standard, which is from 33-64.
      pz_QmiSvOut->gnssSvId = w_SvId + (120 - PDSM_PD_SV_ID_SBAS_MIN); // convert from 33~64 to 120~151
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_SBAS_V02;
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;

#if (PDSM_API_MAJ_V > 7) || ((PDSM_API_MAJ_V == 7) && (PDSM_API_MIN_V >= 4))
      if ((pz_PdExtMeas->sbas_eph_mask & (1 << (w_SvId - PDSM_PD_SV_ID_SBAS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }
      else
      {
        pz_QmiSvOut->svInfoMask &= ~QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->sbas_alm_mask & (1 << (w_SvId - PDSM_PD_SV_ID_SBAS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
      else
      {
        pz_QmiSvOut->svInfoMask &= ~QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 7.4
    }
    else if (w_SvId >= PDSM_PD_SV_ID_GLO_MIN && w_SvId <= PDSM_PD_SV_ID_GLO_MAX) // 65~96
    {
      // Gloness: Slot id: 1-32
      // In extended measurement report, we follow nmea standard, which is 65-96
      pz_QmiSvOut->gnssSvId = w_SvId + (1 - PDSM_PD_SV_ID_GLO_MIN);
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_GLONASS_V02;

#if (PDSM_API_MAJ_V > 7) || ((PDSM_API_MAJ_V == 7) && (PDSM_API_MIN_V >= 4))
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      if ((pz_PdExtMeas->glo_eph_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_GLO_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->glo_alm_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_GLO_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }

      pz_GnssSvInfoIndMsg->expandedSvList[q_out].gloFrequency =
         LOC_PD_GLO_FREQ(pz_PdSvIn->sv_freq_num);
#endif // PDAPI >= 7.4
    }
    else if (w_SvId >= PDSM_PD_SV_ID_BDS_MIN && w_SvId <= PDSM_PD_SV_ID_BDS_MAX) // 201~237
    {
      pz_QmiSvOut->gnssSvId = w_SvId;
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_BDS_V02;

#if (PDSM_API_MAJ_V > 7) || ((PDSM_API_MAJ_V == 7) && (PDSM_API_MIN_V >= 30))
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      /* 1ULL becuase BDS svmask is uint64 */
      if ((pz_PdExtMeas->bds_eph_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_BDS_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->bds_alm_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_BDS_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 7.30
    }
    else if (w_SvId >= PDSM_PD_SV_ID_GAL_MIN && w_SvId <= PDSM_PD_SV_ID_GAL_MAX) // 301~336
    {
      pz_QmiSvOut->gnssSvId = w_SvId;
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_GALILEO_V02;

#if (PDSM_API_MAJ_V >= 8)
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      /* 1ULL becuase GAL svmask is uint64 */
      if ((pz_PdExtMeas->gal_eph_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_GAL_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->gal_alm_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_GAL_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 8.x
    }
    else if (w_SvId >= PDSM_PD_SV_ID_QZSS_MIN && w_SvId <= PDSM_PD_SV_ID_QZSS_MAX) // 193-197
    {
      pz_QmiSvOut->gnssSvId = w_SvId;
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_QZSS_V02;

#if (PDSM_API_MAJ_V > 8) || ((PDSM_API_MAJ_V == 8) && (PDSM_API_MIN_V >= 3))
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      if ((pz_PdExtMeas->qzss_eph_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_QZSS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->qzss_alm_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_QZSS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 8.3
    }
    else if (w_SvId >= PDSM_PD_SV_ID_NAVIC_MIN && w_SvId <= PDSM_PD_SV_ID_NAVIC_MAX) // 401~414
    {
      pz_QmiSvOut->gnssSvId = w_SvId;
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_NAVIC_V02;

#if (PDSM_API_MAJ_V >= 8)
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      /* 1ULL becuase GAL svmask is uint64 */
      if ((pz_PdExtMeas->navic_eph_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_NAVIC_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->navic_alm_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_NAVIC_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 8.x
    }
    else
    {
      pz_QmiSvOut->validMask = 0;

      LOC_MSG_LOW("locPd_GetSvReport: skipping SV ID: %d\n", w_SvId, 0, 0);

      // note that q_out won't be increases, so we will overwrite this same out record
      continue;
    }

    // azimuth and elevation in degrees
    if ((pz_PdSvIn->elevation != 0) &&
        (pz_PdSvIn->azimuth != 0))
    {
      pz_QmiSvOut->validMask |= (QMI_LOC_SV_INFO_MASK_VALID_ELEVATION_V02 | QMI_LOC_SV_INFO_MASK_VALID_AZIMUTH_V02);
      pz_QmiSvOut->elevation = (float)(pz_PdSvIn->elevation);
      pz_QmiSvOut->azimuth = (float)(pz_PdSvIn->azimuth);
    }

    // fill in SV cno, since PDAPI already filters out invalid measurement, cno will always be valid
    pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SNR_V02;
    pz_QmiSvOut->snr = ((float)pz_PdSvIn->c_no) / 10;
        pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_PROCESS_STATUS_V02;
    pz_QmiSvOut->svStatus = locPd_ConvPdsmSvStateToQmi(pz_PdSvIn->sv_state);
    ++q_out;
  }

  if (q_out > 0)
  {
    /* Validate SV List with length and validity flag */
    pz_GnssSvInfoIndMsg->svList_len = q_out;
    pz_GnssSvInfoIndMsg->svList_valid = TRUE;
  }
}

/*---------------------------------------------------------------------------
@brief
  Utility function that fills the SV report Ind from measurement report
  (QMI_LOC_EVENT_GNSS_SV_INFO_IND)

@param [out]  pz_GnssSvInfoIndMsg : Pointer QMI SV Info Indication

@retval None
---------------------------------------------------------------------*/
static void locPd_GetSvReport
(
   qmiLocEventGnssSvInfoIndMsgT_v02 *pz_GnssSvInfoIndMsg
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  pdsm_pd_ext_meas_s_type *pz_PdExtMeas = NULL;
  uint32                     q_out, q_in;

  if (NULL == pz_GnssSvInfoIndMsg)
  {
    LOC_MSG_ERROR("locPd_GetSvReport, NULL pz_GnssSvInfoIndMsg", 0, 0, 0);
    return;
  }

  pz_PdExtMeas = &(pz_LocPdData->ext_status_info_copy.ext_status_info.ext_meas_report_type);

  memset(pz_GnssSvInfoIndMsg, 0, sizeof(*pz_GnssSvInfoIndMsg));

  // TLV 0x10 - svList (Deprecated)
  locPd_fillDeprecatedTlv_svList(pz_PdExtMeas, pz_GnssSvInfoIndMsg);

  // TLV 0x11 and 0x12 - expandedSvList and gnssSignalTypeList
  for (q_out = q_in = 0;
       (q_in < pz_PdExtMeas->num_svs) && (q_out < QMI_LOC_EXPANDED_SV_INFO_LIST_MAX_SIZE_V02) && (q_in < PDSM_EXT_MEAS_NUM_SVS);
       q_in++)
  {
    const pdsm_pd_meas_s_type *pz_PdSvIn = &(pz_PdExtMeas->ext_meas_report_type[q_in]);
    gnss_sv_id_type                  w_SvId = pz_PdSvIn->sv_id;
    qmiLocSvInfoStructT_v02 *pz_QmiSvOut = &(pz_GnssSvInfoIndMsg->expandedSvList[q_out].svInfo);
    qmiLocGnssSignalTypeMaskT_v02 *pz_QmiSignalTypeOut = &(pz_GnssSvInfoIndMsg->gnssSignalTypeList[q_out]);

    pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SYSTEM_V02;
    pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_GNSS_SVID_V02;

    //Health in SV Info is always invalid
    pz_QmiSvOut->validMask &= ~QMI_LOC_SV_INFO_MASK_VALID_HEALTH_STATUS_V02;
    pz_QmiSvOut->healthStatus = 0;

    if (w_SvId >= PDSM_PD_SV_ID_GPS_MIN && w_SvId <= PDSM_PD_SV_ID_GPS_MAX) // 1~32
    {
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_GPS_V02;
      pz_QmiSvOut->gnssSvId = w_SvId;

      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;
      if ((pz_PdExtMeas->eph_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_GPS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }
      if ((pz_PdExtMeas->alm_svmask  & (1 << (w_SvId - PDSM_PD_SV_ID_GPS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
    }
    else if (w_SvId >= PDSM_PD_SV_ID_SBAS_MIN && w_SvId <= PDSM_PD_SV_ID_SBAS_MAX) // 33~64
    {
      // SBAS: GPS RPN: 120-151,
      // In exteneded measurement report, we follow nmea standard, which is from 33-64.
      pz_QmiSvOut->gnssSvId = w_SvId + (120 - PDSM_PD_SV_ID_SBAS_MIN); // convert from 33~64 to 120~151
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_SBAS_V02;
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;

#if (PDSM_API_MAJ_V > 7) || ((PDSM_API_MAJ_V == 7) && (PDSM_API_MIN_V >= 4))
      if ((pz_PdExtMeas->sbas_eph_mask & (1 << (w_SvId - PDSM_PD_SV_ID_SBAS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }
      else
      {
        pz_QmiSvOut->svInfoMask &= ~QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->sbas_alm_mask & (1 << (w_SvId - PDSM_PD_SV_ID_SBAS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
      else
      {
        pz_QmiSvOut->svInfoMask &= ~QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 7.4
    }
    else if (w_SvId >= PDSM_PD_SV_ID_GLO_MIN && w_SvId <= PDSM_PD_SV_ID_GLO_MAX) // 65~96
    {
      // Gloness: Slot id: 1-32
      // In extended measurement report, we follow nmea standard, which is 65-96
      pz_QmiSvOut->gnssSvId = w_SvId + (1 - PDSM_PD_SV_ID_GLO_MIN);
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_GLONASS_V02;

#if (PDSM_API_MAJ_V > 7) || ((PDSM_API_MAJ_V == 7) && (PDSM_API_MIN_V >= 4))
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      if ((pz_PdExtMeas->glo_eph_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_GLO_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->glo_alm_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_GLO_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }

      pz_GnssSvInfoIndMsg->expandedSvList[q_out].gloFrequency =
         LOC_PD_GLO_FREQ(pz_PdSvIn->sv_freq_num);
#endif // PDAPI >= 7.4
    }
    else if (w_SvId >= PDSM_PD_SV_ID_BDS_MIN && w_SvId <= PDSM_PD_SV_ID_BDS_MAX) // 201~237
    {
      pz_QmiSvOut->gnssSvId = w_SvId;
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_BDS_V02;

#if (PDSM_API_MAJ_V > 7) || ((PDSM_API_MAJ_V == 7) && (PDSM_API_MIN_V >= 30))
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      /* 1ULL becuase BDS svmask is uint64 */
      if ((pz_PdExtMeas->bds_eph_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_BDS_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->bds_alm_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_BDS_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 7.30
    }
    else if (w_SvId >= PDSM_PD_SV_ID_GAL_MIN && w_SvId <= PDSM_PD_SV_ID_GAL_MAX) // 301~336
    {
      pz_QmiSvOut->gnssSvId = w_SvId;
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_GALILEO_V02;

#if (PDSM_API_MAJ_V >= 8)
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      /* 1ULL becuase GAL svmask is uint64 */
      if ((pz_PdExtMeas->gal_eph_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_GAL_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->gal_alm_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_GAL_MIN))) != 0ULL)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 8.x
    }
    else if (w_SvId >= PDSM_PD_SV_ID_NAVIC_MIN && w_SvId <= PDSM_PD_SV_ID_NAVIC_MAX) // 401~414
    {
       pz_QmiSvOut->gnssSvId = w_SvId;
       pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_NAVIC_V02;

#if (PDSM_API_MAJ_V >= 8) 
       pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
       pz_QmiSvOut->svInfoMask = 0;

         /* 1ULL becuase NAVIC svmask is uint64 */
       if ((pz_PdExtMeas->navic_eph_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_NAVIC_MIN)) ) != 0ULL)
       {
          pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
       }

       if ((pz_PdExtMeas->navic_alm_svmask & (1ULL << (w_SvId - PDSM_PD_SV_ID_NAVIC_MIN)) ) != 0ULL)
       {
          pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
       }
#endif // PDAPI >= 8.x
    }
    else if (w_SvId >= PDSM_PD_SV_ID_QZSS_MIN && w_SvId <= PDSM_PD_SV_ID_QZSS_MAX) // 193-197
    {
      pz_QmiSvOut->gnssSvId = w_SvId;
      pz_QmiSvOut->system = eQMI_LOC_SV_SYSTEM_QZSS_V02;

#if (PDSM_API_MAJ_V > 8) || ((PDSM_API_MAJ_V == 8) && (PDSM_API_MIN_V >= 3))
      pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SVINFO_MASK_V02;
      pz_QmiSvOut->svInfoMask = 0;

      if ((pz_PdExtMeas->qzss_eph_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_QZSS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_EPHEMERIS_V02;
      }

      if ((pz_PdExtMeas->qzss_alm_svmask & (1 << (w_SvId - PDSM_PD_SV_ID_QZSS_MIN))) != 0)
      {
        pz_QmiSvOut->svInfoMask |= QMI_LOC_SVINFO_MASK_HAS_ALMANAC_V02;
      }
#endif // PDAPI >= 8.3
    }
    else
    {
      pz_QmiSvOut->validMask = 0;

      LOC_MSG_LOW("locPd_GetSvReport: skipping SV ID: %d\n", w_SvId, 0, 0);

      // note that q_out won't be increases, so we will overwrite this same out record
      continue;
    }

    // azimuth and elevation in degrees
    if ((pz_PdSvIn->elevation != 0) &&
        (pz_PdSvIn->azimuth != 0))
    {
      pz_QmiSvOut->validMask |= (QMI_LOC_SV_INFO_MASK_VALID_ELEVATION_V02 | QMI_LOC_SV_INFO_MASK_VALID_AZIMUTH_V02);
      pz_QmiSvOut->elevation = (float)(pz_PdSvIn->elevation);
      pz_QmiSvOut->azimuth = (float)(pz_PdSvIn->azimuth);
    }

    // fill in SV cno, since PDAPI already filters out invalid measurement, cno will always be valid
    pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_SNR_V02;
    pz_QmiSvOut->snr = ((float)pz_PdSvIn->c_no) / 10;
        pz_QmiSvOut->validMask |= QMI_LOC_SV_INFO_MASK_VALID_PROCESS_STATUS_V02;
    pz_QmiSvOut->svStatus = locPd_ConvPdsmSvStateToQmi(pz_PdSvIn->sv_state);
    pz_GnssSvInfoIndMsg->rfLoss[q_out] = pz_PdSvIn->rf_loss / 10.f;

    switch (pz_PdSvIn->gnss_signal_type)
    {
      case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L1CA_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1C:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L1C_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L2C_L_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L5_Q_V02;
        break;
      case  PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GLONASS_G1_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GLONASS_G2_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E1_C_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5B_Q:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E5B_Q_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B1_I_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_I:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B2A_I_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L1CA_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1S:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L1S_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L2C_L_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L5_Q_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_SBAS_L1_CA:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_SBAS_L1_CA_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_NAVIC_L5:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_NAVIC_L5_V02;
        break;
      case PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q:
        *pz_QmiSignalTypeOut = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q_V02;
        break;

      default:
        break;
    }
    ++q_out;
  }

  if (q_out > 0)
  {
    /* Validate Expanded SV List with length and validity flag */
    pz_GnssSvInfoIndMsg->expandedSvList_len = q_out;
    pz_GnssSvInfoIndMsg->expandedSvList_valid = TRUE;
    /* Validate Signal Type List with length and validity flag */
    pz_GnssSvInfoIndMsg->gnssSignalTypeList_len = q_out;
    pz_GnssSvInfoIndMsg->gnssSignalTypeList_valid = TRUE;
    /* Validate RF Loss with length and validity flag */
	pz_GnssSvInfoIndMsg->rfLoss_len = q_out;
	pz_GnssSvInfoIndMsg->rfLoss_valid = TRUE;
  }

  locPd_dumpSvIn(pz_PdExtMeas);
}

/**   locPd_ConvertTechnologyMaskFromPdapi
 
@brief : convert PDAPI position source to QMI LOC technology 
       mask.
@param [in]   pdapiPosSource : PDAPI position source 
 
@retval QMI_LOC technology mask
---------------------------------------------------------------------*/

static qmiLocPosTechMaskT_v02 locPd_ConvertTechnologyMaskFromPdapi(
   pdsm_pd_positioning_source_type pdapiPosSource)
{
  int i;
  qmiLocPosTechMaskT_v02 q_PosTechMask = { 0 };

  for (i = 0;
       i < ((sizeof(loc_pd_technology_mask_map)) /
            (sizeof(loc_pd_technology_mask_map[0])));
       i++)
  {
    if (pdapiPosSource & loc_pd_technology_mask_map[i].pdapiPosSource)
    {
      q_PosTechMask |= loc_pd_technology_mask_map[i].qmiLocPostechMask;
    }
  }

  return q_PosTechMask;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to convert from pdsm_pd_reliability_e_type to 
qmiLocReliabilityEnumT_v02

@param[out]  pe_Target      points to target Qmi-Loc data type   
@param[in]   reliability          PDAPI format of reliability indicator

@retval    TRUE if the conversion was successful 
@retval    FALSE if the conversion failed
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_ConvertReliabilityFromPdapi
(
   qmiLocReliabilityEnumT_v02        *pe_Target,
   pdsm_pd_reliability_e_type         e_Reliability
   )
{
  boolean v_result = FALSE;

  if (NULL == pe_Target)
  {
    return v_result;
  }

  v_result = TRUE;
  switch (e_Reliability)
  {
    case PDSM_POSITION_RELIABILITY_NOT_SET:
      *pe_Target = eQMI_LOC_RELIABILITY_NOT_SET_V02;
      break;
    case PDSM_POSITION_RELIABILITY_VERY_LOW:
      *pe_Target = eQMI_LOC_RELIABILITY_VERY_LOW_V02;
      break;
    case PDSM_POSITION_RELIABILITY_LOW:
      *pe_Target = eQMI_LOC_RELIABILITY_LOW_V02;
      break;
    case PDSM_POSITION_RELIABILITY_MEDIUM:
      *pe_Target = eQMI_LOC_RELIABILITY_MEDIUM_V02;
      break;
    case PDSM_POSITION_RELIABILITY_HIGH:
      *pe_Target = eQMI_LOC_RELIABILITY_HIGH_V02;
      break;
    default:
      v_result = FALSE;
      break;
  }

  if (TRUE != v_result)
  {
    LOC_MSG_ERROR("locPd_ConvertReliabilityFromPdapi failed", 0, 0, 0);
  }
  return v_result;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to convert sensor data usage indicator from PDAPI to Qmi-Loc

PDAPI does not define constants, and loc-mw just copies the PDAPI value
over to loc-api structures. 

As Per PDAPI:
    0x0001 ?Accelerometer
    0x0002 ?Gyro

@param[out]  pz_Target      points to target Qmi-Loc data type 
@param[in]   w_Source       PDAPI format of sensor data usage indicator

@retval    TRUE if the conversion was successful 
@retval    FALSE if the conversion failed
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_ConvertSensorUsageFromPdapi
(
   uint16                           w_Source,
   qmiLocSensorUsageMaskT_v02      *pz_Target
   )
{
  boolean v_RetVal = TRUE;
  uint8   u_Cntr = 0;

  const struct {
    uint16                      w_SrcMask;
    qmiLocSensorUsageMaskT_v02  z_TgtMask;
  }z_SensorUsageMaskMap[] =
  {
    { 0x0001,
      QMI_LOC_SENSOR_MASK_USED_ACCEL_V02 },
    { 0x0002,
      QMI_LOC_SENSOR_MASK_USED_GYRO_V02 }
  };
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    if (NULL == pz_Target)
    {
      v_RetVal = FALSE;
      break;
    }

    (*pz_Target) = 0;
    for (u_Cntr = 0;
         u_Cntr < (sizeof(z_SensorUsageMaskMap) / sizeof(z_SensorUsageMaskMap[0]));
         u_Cntr++)
    {
      if (z_SensorUsageMaskMap[u_Cntr].w_SrcMask & w_Source)
      {
        (*pz_Target) |= z_SensorUsageMaskMap[u_Cntr].z_TgtMask;
      }
    }
  } while (0);

  if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locPd_ConvertSensorUsageFromPdapi failed:0x%x",
                  (pz_Target), 0, 0);
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to convert sensor aiding indicator from PDAPI to Qmi-Loc

PDAPI does not define constants, and loc-mw just copies the PDAPI value
over to loc-api structures. 

As Per PDAPI:
   0x0001 ?Heading aided with sensor data
   0x0002 ?Speed aided with sensor data
   0x0004 ?Position aided with sensor data
   0x0008 ?Velocity aided with sensor data

@param[out]  z_Target      Points to target Qmi-Loc data type   
@param[in]   w_Source       PDAPI format of sensor aiding indicator

@retval    TRUE if the conversion was successful 
@retval    FALSE if the conversion failed
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_ConvertSensorAidFromPdapi
(
   uint16                       w_Source,
   qmiLocSensorAidedMaskT_v02  *pz_Target
   )
{
  boolean v_RetVal = TRUE;
  uint8   u_Cntr = 0;

  const struct {
    uint16                      w_SrcMask;
    qmiLocSensorAidedMaskT_v02  z_TgtMask;

  }z_SensorAidingMaskMap[] =
  {
    { 0x0001,
      QMI_LOC_SENSOR_AIDED_MASK_HEADING_V02 },
    { 0x0002,
      QMI_LOC_SENSOR_AIDED_MASK_SPEED_V02 },
    { 0x0004,
      QMI_LOC_SENSOR_AIDED_MASK_POSITION_V02 },
    { 0x0008,
      QMI_LOC_SENSOR_AIDED_MASK_VELOCITY_V02 }
  };
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    if (NULL == pz_Target)
    {
      v_RetVal = FALSE;
      break;
    }

    (*pz_Target) = 0;

    for (u_Cntr = 0;
         u_Cntr < (sizeof(z_SensorAidingMaskMap) / sizeof(z_SensorAidingMaskMap[0]));
         u_Cntr++)
    {
      if (z_SensorAidingMaskMap[u_Cntr].w_SrcMask & w_Source)
      {
        (*pz_Target) |= z_SensorAidingMaskMap[u_Cntr].z_TgtMask;
      }
    }
  } while (0);

  if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locPd_ConvertSensorAidFromPdapi failed: 0x%x",
                  (pz_Target), 0, 0);
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to convert from pdsm_extended_time_src_e_type to
qmiLocTimeSourceEnumT_v02

@param[out]  pe_QmiLocTimeSource   Points to QMI_LOC data type
@param[in]   e_ExtTimeSrc          PDAPI format of time source indicator

@retval    TRUE if the conversion was successful 
@retval    FALSE if the conversion failed
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_ConvertTimeSrcFromPdapi
(
   qmiLocTimeSourceEnumT_v02 *pe_QmiLocTimeSource,
   pdsm_extended_time_src_e_type e_ExtTimeSrc
   )
{
  boolean v_RetVal = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    if (NULL == pe_QmiLocTimeSource)
    {
      v_RetVal = FALSE;
      break;
    }

    switch (e_ExtTimeSrc)
    {
         case PDSM_GNSS_TIME_SRC_RESERVED:
            *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_SYSTEM_TIMETICK_V02;
         break;
      case PDSM_GNSS_TIME_SRC_INVALID:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_INVALID_V02;
        break;
      case PDSM_GNSS_TIME_SRC_UNKNOWN:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_UNKNOWN_V02;
        break;
      case PDSM_GNSS_TIME_SRC_NETWORK_TIME_TRANSFER:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_NETWORK_TIME_TRANSFER_V02;
        break;
      case PDSM_GNSS_TIME_SRC_NETWORK_TIME_TAGGING:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_NETWORK_TIME_TAGGING_V02;
        break;
      case PDSM_GNSS_TIME_SRC_EXTERNAL_INPUT:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_EXTERNAL_INPUT_V02;
        break;
      case PDSM_GNSS_TIME_SRC_GPS_TOW_DECODE:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_TOW_DECODE_V02;
        break;
      case PDSM_GNSS_TIME_SRC_TOW_CONFIRMED:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_TOW_CONFIRMED_V02;
        break;
      case PDSM_GNSS_TIME_SRC_TOW_WEEK_CONFIRMED:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_TOW_AND_WEEK_CONFIRMED_V02;
        break;
      case PDSM_GNSS_TIME_SRC_NAV_SOLUTION:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_NAV_SOLUTION_V02;
        break;
      case PDSM_GNSS_TIME_SRC_SOLVE_FOR_TIME:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_SOLVE_FOR_TIME_V02;
        break;
      case PDSM_GNSS_TIME_SRC_GLO_TOW_DECODE:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_GLO_TOW_DECODE_V02;
        break;
      case PDSM_GNSS_TIME_SRC_TIME_TRANSFORM:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_TIME_TRANSFORM_V02;
        break;
      case PDSM_GNSS_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING_V02;
        break;
      case PDSM_GNSS_TIME_SRC_GSM_SLEEP_TIME_TAGGING:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_GSM_SLEEP_TIME_TAGGING_V02;
        break;
      case PDSM_GNSS_TIME_SRC_QZSS_TOW_DECODE:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_QZSS_TOW_DECODE_V02;
        break;
      case PDSM_GNSS_TIME_SRC_BDS_TOW_DECODE:
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_BDS_TOW_DECODE_V02;
        break;
      case PDSM_GNSS_TIME_SRC_GAL_TOW_DECODE :
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_GAL_TOW_DECODE_V02;
        break;
      case PDSM_GNSS_TIME_SRC_NAVIC_TOW_DECODE :
        *pe_QmiLocTimeSource = eQMI_LOC_TIME_SRC_NAVIC_TOW_DECODE_V02;
        break;
      default:
        v_RetVal = FALSE;
        break;
    }
  } while (0);

  if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locPd_ConvertTimeSrcFromPdapi failed:%d",
                  e_ExtTimeSrc, 0, 0);
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to convert from gnss_SfmtEnumTypeVal to
qmiLocGnssSignalTypeMaskT_v02

@param[in]   u_UsedPrMeasType (of type gnss_SfmtEnumTypeVal)

@retval    qmiLocGnssSignalTypeMaskT_v02 SignalType for SV
*/ /* ------------------------------------------------------------------------*/
static qmiLocGnssSignalTypeMaskT_v02 locPd_ConvertGnssSignaltoQmiType
(
   uint8 u_UsedPrMeasType
   )
{
  qmiLocGnssSignalTypeMaskT_v02 t_SignalType = 0;
  switch (u_UsedPrMeasType)
  {
    case C_GNSS_SFMT_GPS_L1CA:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L1CA_V02;
      break;

    case C_GNSS_SFMT_GLO_G1:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GLONASS_G1_V02;
      break;

    case C_GNSS_SFMT_BDS_B1:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B1_I_V02;
      break;

    case C_GNSS_SFMT_GAL_E1:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E1_C_V02;
      break;

    case C_GNSS_SFMT_QZSS_L1CA:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L1CA_V02;
      break;

    case C_GNSS_SFMT_GPS_L2C:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L2C_L_V02;
      break;

    case C_GNSS_SFMT_GPS_L5Q:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L5_Q_V02;
      break;

    case C_GNSS_SFMT_GLO_G2:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GLONASS_G2_V02;
      break;

    case C_GNSS_SFMT_BDS_B2:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B2_I_V02;
      break;

    case C_GNSS_SFMT_BDS_B2A:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q_V02;
      break;

    case C_GNSS_SFMT_GAL_E5A:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q_V02;
      break;

    case C_GNSS_SFMT_QZSS_L2C:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L2C_L_V02;
      break;

    case C_GNSS_SFMT_QZSS_L5Q:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L5_Q_V02;
      break;
    case C_GNSS_SFMT_NAVIC_L5:
      t_SignalType |= QMI_LOC_MASK_GNSS_SIGNAL_TYPE_NAVIC_L5_V02;
      break;

    default:
      LOC_MSG_ERROR_1("Undefined SignalType %u", u_UsedPrMeasType);
      break;
  }
  return t_SignalType;
}

/*---------------------------------------------------------------------------
@brief
  Utility function that fills out the deprecated TLV 0x2C (gnssSvUsedList)
  under QMI_LOC_EVENT_POSITION_REPORT_IND.
  New TLV expandedGnssSvUsedList should be used instead.

@param[out]  pz_PositionReportIndMsg     Pointer to QMI Indication
@param[in]   pz_NavSvInfo                PDAPI format SV Information

@retval None
---------------------------------------------------------------------*/
static void locPd_fillDeprecatedTlv_svUsedList
(
   qmiLocEventPositionReportIndMsgT_v02  *pz_PositionReportIndMsg,
   const pdsm_GnssNavSvInfo              *pz_NavSvInfo
   )
{
  uint32_t i = 0, q_SvListIdx = 0;

  if ((NULL == pz_PositionReportIndMsg) || (NULL == pz_NavSvInfo))
  {
    LOC_MSG_ERROR_1("Null Pointer 0x%X", pz_PositionReportIndMsg);
    return;
  }

  /** convert the sv used info to gnssSvUsedList */
  /* Populate the GPS satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_GPS_CHAN) && (q_SvListIdx < QMI_LOC_MAX_SV_USED_LIST_LENGTH_V02); i++)
  {
    /* Check whether GPS SVs were used in calculating position */
    if (pz_NavSvInfo->z_GpsSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_GpsSvInfo[i].w_SvId;
    }
  }

  /* Populate the GLO satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_GLO_CHAN) && (q_SvListIdx < QMI_LOC_MAX_SV_USED_LIST_LENGTH_V02); i++)
  {
    /* Check whether GLO SVs were used in calculating position */
    if (pz_NavSvInfo->z_GloSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_GloSvInfo[i].w_SvId;
    }
  }

  /* Populate the QZSS satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_QZSS_CHAN) && (q_SvListIdx < QMI_LOC_MAX_SV_USED_LIST_LENGTH_V02); i++)
  {
    /* Check whether QZSS SVs were used in calculating position */
    if (pz_NavSvInfo->z_QzssSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_QzssSvInfo[i].w_SvId;
    }
  }

  /* Populate the BDS satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_BDS_CHAN) && (q_SvListIdx < QMI_LOC_MAX_SV_USED_LIST_LENGTH_V02); i++)
  {
    /* Check whether GLO SVs were used in calculating position */
    if (pz_NavSvInfo->z_BdsSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_BdsSvInfo[i].w_SvId;
    }
  }

  /* Populate the GAL satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_GAL_CHAN) && (q_SvListIdx < QMI_LOC_MAX_SV_USED_LIST_LENGTH_V02); i++)
  {
    /* Check whether GAL SVs were used in calculating position */
    if (pz_NavSvInfo->z_GalSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_GalSvInfo[i].w_SvId;
    }
  }

  if (q_SvListIdx > 0)
  {
    /* Validate SV List with length and validity flag */
    pz_PositionReportIndMsg->gnssSvUsedList_len = q_SvListIdx;
    pz_PositionReportIndMsg->gnssSvUsedList_valid = TRUE;
  }
}

/* ----------------------------------------------------------------------------
@brief
  Utility function to convert from pdsm_GnssNavSvInfo to
  qmiLocEventPositionReportIndMsgT_v02->expandedGnssSvUsedList
  (TLV 0x36 under QMI_LOC_EVENT_POSITION_REPORT_IND)

@param[out]  pz_PositionReportIndMsg     Pointer to QMI Indication
@param[in]   pz_PdData                   PDAPI format Position Data

@retval    None
-----------------------------------------------------------------------------*/
static void locPd_ConvertGnssNavSvInfoFromPdapi
(
   qmiLocEventPositionReportIndMsgT_v02  *pz_PositionReportIndMsg,
   const pdsm_pd_data_type               *pz_PdData
   )
{
  uint32_t i = 0, q_SvListIdx = 0;
  const pdsm_GnssNavSvInfo *pz_NavSvInfo = NULL;

  if (NULL == pz_PositionReportIndMsg ||
      NULL == pz_PdData)
  {
    return;
  }
  pz_NavSvInfo = &(pz_PdData->z_NavSvInfo);

  // TLV 0x2C - svList (Deprecated)
  locPd_fillDeprecatedTlv_svUsedList(pz_PositionReportIndMsg, pz_NavSvInfo);

  // TLV 0x36 - Convert the sv used info to expandedGnssSvUsedList
  /* Populate the GPS satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_GPS_CHAN) && (q_SvListIdx < QMI_LOC_EXPANDED_SV_INFO_LIST_MAX_SIZE_V02); i++)
  {
    /* Check whether GPS SVs were used in calculating position */
    if (pz_NavSvInfo->z_GpsSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedSignalTypeList[q_SvListIdx] =
         locPd_ConvertGnssSignaltoQmiType(pz_NavSvInfo->z_GpsSvInfo[i].u_UsedPrMeasType);
      pz_PositionReportIndMsg->expandedGnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_GpsSvInfo[i].w_SvId;
    }
  }

  /* Populate the GLO satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_GLO_CHAN) && (q_SvListIdx < QMI_LOC_EXPANDED_SV_INFO_LIST_MAX_SIZE_V02); i++)
  {
    /* Check whether GLO SVs were used in calculating position */
    if (pz_NavSvInfo->z_GloSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedSignalTypeList[q_SvListIdx] =
         locPd_ConvertGnssSignaltoQmiType(pz_NavSvInfo->z_GloSvInfo[i].u_UsedPrMeasType);
      pz_PositionReportIndMsg->expandedGnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_GloSvInfo[i].w_SvId;
    }
  }

  /* Populate the QZSS satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_QZSS_CHAN) && (q_SvListIdx < QMI_LOC_EXPANDED_SV_INFO_LIST_MAX_SIZE_V02); i++)
  {
    /* Check whether QZSS SVs were used in calculating position */
    if (pz_NavSvInfo->z_QzssSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedSignalTypeList[q_SvListIdx] =
         locPd_ConvertGnssSignaltoQmiType(pz_NavSvInfo->z_QzssSvInfo[i].u_UsedPrMeasType);
      pz_PositionReportIndMsg->expandedGnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_QzssSvInfo[i].w_SvId;
    }
  }

  /* Populate the BDS satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_BDS_CHAN) && (q_SvListIdx < QMI_LOC_EXPANDED_SV_INFO_LIST_MAX_SIZE_V02); i++)
  {
    /* Check whether GLO SVs were used in calculating position */
    if (pz_NavSvInfo->z_BdsSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedSignalTypeList[q_SvListIdx] =
         locPd_ConvertGnssSignaltoQmiType(pz_NavSvInfo->z_BdsSvInfo[i].u_UsedPrMeasType);
      pz_PositionReportIndMsg->expandedGnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_BdsSvInfo[i].w_SvId;
    }
  }

  /* Populate the GAL satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_GAL_CHAN) && (q_SvListIdx < QMI_LOC_EXPANDED_SV_INFO_LIST_MAX_SIZE_V02); i++)
  {
    /* Check whether GAL SVs were used in calculating position */
    if (pz_NavSvInfo->z_GalSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedSignalTypeList[q_SvListIdx] =
         locPd_ConvertGnssSignaltoQmiType(pz_NavSvInfo->z_GalSvInfo[i].u_UsedPrMeasType);
      pz_PositionReportIndMsg->expandedGnssSvUsedList[q_SvListIdx++] =
         pz_NavSvInfo->z_GalSvInfo[i].w_SvId;
    }
  }

  /* Populate the Navic satellite info */
  for (i = 0; (i < PDSM_N_ACTIVE_NAVIC_CHAN) && (q_SvListIdx < QMI_LOC_EXPANDED_SV_INFO_LIST_MAX_SIZE_V02); i++)
  {
    /* Check whether NavIC SVs were used in calculating position */
    if (pz_NavSvInfo->z_NavicSvInfo[i].q_SvInfoMask & PDSM_GNSS_SV_INFO_USED_IN_POS_VEL_FIX)
    {
      pz_PositionReportIndMsg->gnssSvUsedSignalTypeList[q_SvListIdx] =
                               locPd_ConvertGnssSignaltoQmiType(pz_NavSvInfo->z_NavicSvInfo[i].u_UsedPrMeasType);
      pz_PositionReportIndMsg->expandedGnssSvUsedList[q_SvListIdx++] =
                               pz_NavSvInfo->z_NavicSvInfo[i].w_SvId;
    }
  }

  /** set the valid flag only if the list is non-empty */
  if (q_SvListIdx > 0)
  {
    /* Validate Expanded SV List with length and validity flag */
    pz_PositionReportIndMsg->expandedGnssSvUsedList_len = q_SvListIdx;
    pz_PositionReportIndMsg->expandedGnssSvUsedList_valid = TRUE;
    /* Validate Signal Type List with length and validity flag */
    pz_PositionReportIndMsg->gnssSvUsedSignalTypeList_len = q_SvListIdx;
    pz_PositionReportIndMsg->gnssSvUsedSignalTypeList_valid = TRUE;
  }

  /* Navigation solution position flags population */
  pz_PositionReportIndMsg->navSolutionMask_valid = FALSE;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to convert the pdapi spoof report to QMI format

@param[out]  pt_spoofReportMask         Points to QMI_LOC data type
@param[in]   pz_PdsmSpoofReport         PDAPI format to be converted

@retval    TRUE if the conversion was successful
@retval    FALSE if the conversion failed
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_ConvertSpoofReportFromPdapi
(
   qmiLocSpoofMaskT_v02 *pt_spoofReportMask,
   const pdsm_spoof_report *pz_PdsmSpoofReport
   )
{
  if ((NULL == pt_spoofReportMask) || (NULL == pz_PdsmSpoofReport))
  {
    LOC_MSG_ERROR_2("locPd_ConvertSpoofReportFromPdapi: NULL input 0x%p 0x%p",
                    pt_spoofReportMask, pz_PdsmSpoofReport);
    return FALSE;
  }

  // Initialize to 0 by default
  *pt_spoofReportMask = 0ULL;

  if (PDSM_SPOOF_EVENT_TIME_DOMAIN & pz_PdsmSpoofReport->spoof_type_mask)
  {
    *pt_spoofReportMask |= QMI_LOC_TIME_SPOOFED_V02;
  }

  if (PDSM_SPOOF_EVENT_POSITION_DOMAIN & pz_PdsmSpoofReport->spoof_type_mask)
  {
    *pt_spoofReportMask |= QMI_LOC_POSITION_SPOOFED_V02;
  }

  if (PDSM_SPOOF_EVENT_NAVIGATION_DATA_DOMAIN & pz_PdsmSpoofReport->spoof_type_mask)
  {
    *pt_spoofReportMask |= QMI_LOC_NAVIGATION_DATA_SPOOFED_V02;
  }

  return TRUE;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to populate JammerList Indication for Intermediate and
Final Fix Reports

@param[out]  pz_PositionReportIndMsg        points to target Qmi-Loc data type
@param[in]   pz_PdsmList                    Source PDAPI format

@retval    None
*/ /* ------------------------------------------------------------------------*/
static void locPd_PopulateJammerInd(qmiLocEventPositionReportIndMsgT_v02 *pz_PositionReportIndMsg,
                                    pdsm_gnss_jammerIndicatorStructType *pz_PdsmList)
{
  uint8 u_I;
  pdsm_LocGnssSignalEnumType e_Signal;

  for (u_I = 0; u_I < QMI_LOC_MAX_GNSS_SIGNAL_TYPE_V02; u_I++)
  {
    switch (u_I)
    {
      case eQMI_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GPS_L1C_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1C;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5B_Q_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5B_Q;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1C_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1C;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2_I_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2_I;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_I_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_I;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_QZSS_L1S_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1S;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_SBAS_L1_CA_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_SBAS_L1_CA;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q;
        break;

      case eQMI_LOC_GNSS_SIGNAL_TYPE_NAVIC_L5_V02:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_NAVIC_L5;
        break;

      default:
        continue;
    }

    pz_PositionReportIndMsg->jammerIndicatorList[u_I].agcMetricDb = pz_PdsmList[e_Signal].l_agcMetricDb;
    pz_PositionReportIndMsg->jammerIndicatorList[u_I].bpMetricDb = pz_PdsmList[e_Signal].l_bpMetricDb;
  }

  pz_PositionReportIndMsg->jammerIndicatorList_len = QMI_LOC_MAX_GNSS_SIGNAL_TYPE_V02;
  pz_PositionReportIndMsg->jammerIndicatorList_valid = TRUE;
}

static boolean locPd_GetReport(qmiLocEventPositionReportIndMsgT_v02 *pz_PositionReportIndMsg,
                               qmiLocEventGnssSvInfoIndMsgT_v02 *pz_GnssSvInfoIndMsg)
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  boolean                      v_ProcessPdInfo           = FALSE,
     v_ProcessIntermPosInfo    = FALSE,
     v_ProcessPdSessEndStatus  = FALSE,
     v_ProcessPdCmdErr         = FALSE,
     v_ProcessUnpropagatedPosInfo = FALSE;
  int                          l_LeapSeconds;
  int                          l_IntermediatePosValid = 1;
  time_type                    z_TimeMs = { 0 };

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
  boolean v_testModeEnabled = FALSE;
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

  if ((NULL == pz_PositionReportIndMsg) ||
      (NULL == pz_GnssSvInfoIndMsg))
  {
    LOC_MSG_ERROR("locPd_GetReport skip all pd reports", 0, 0, 0);
    return FALSE;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  if (TRUE == pz_LocPdData->new_pd_info_received)
  {
    v_ProcessPdInfo      = TRUE;
    memscpy(&(pz_LocPdData->pd_info_copy), sizeof(pz_LocPdData->pd_info_copy),
            &(pz_LocPdData->pd_info), sizeof(pz_LocPdData->pd_info));
    pz_LocPdData->new_pd_info_received = FALSE;
  }

  if (TRUE == pz_LocPdData->new_pd_sess_end_status_received)
  {
    v_ProcessPdSessEndStatus = TRUE;
    pz_LocPdData->pd_sess_end_status_copy = pz_LocPdData->pd_sess_end_status;
    pz_LocPdData->new_pd_sess_end_status_received = FALSE;
  }

  if (TRUE == pz_LocPdData->new_pd_cmd_err_received)
  {
    v_ProcessPdCmdErr                  = TRUE;
    pz_LocPdData->pd_cmd_err_copy = pz_LocPdData->pd_cmd_err;
    pz_LocPdData->new_pd_cmd_err_received = FALSE;
  }

  if (TRUE == pz_LocPdData->new_interm_pos_info_received)
  {
    pz_LocPdData->new_interm_pos_info_received = FALSE;
    v_ProcessIntermPosInfo      = TRUE;
    memscpy(&pz_LocPdData->interm_pos_info_copy, sizeof(pz_LocPdData->interm_pos_info_copy),
            &pz_LocPdData->interm_pos_info, sizeof(pz_LocPdData->interm_pos_info));
  }

  if (TRUE == pz_LocPdData->new_unpropagated_pos_info_received)
  {
    pz_LocPdData->new_unpropagated_pos_info_received = FALSE;
    v_ProcessUnpropagatedPosInfo      = TRUE;
    memscpy(&pz_LocPdData->unpropagated_pos_info_copy, sizeof(pz_LocPdData->unpropagated_pos_info_copy),
            &pz_LocPdData->unpropagated_pos_info, sizeof(pz_LocPdData->unpropagated_pos_info));
  }

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
  if ((TRUE == pz_LocPdData->enable_pos_inj_test_mode) &&
      (TRUE == pz_LocPdData->cached_gnss_test_position_valid))
  {
    v_testModeEnabled = TRUE;
  }
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  // Initialzie the parsed report structure
  memset(pz_PositionReportIndMsg, 0, sizeof(*pz_PositionReportIndMsg));
  memset(pz_GnssSvInfoIndMsg, 0, sizeof(*pz_GnssSvInfoIndMsg));
  // Initialzie sessionStatus to invalid value
  pz_PositionReportIndMsg->sessionStatus = QMILOCSESSIONSTATUSENUMT_MIN_ENUM_VAL_V02;

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
  if (TRUE == v_testModeEnabled)
  {
    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    /* Test position injection mode. Copy over the injected test position and return */
    pz_PositionReportIndMsg->latitude_valid = TRUE;
    pz_PositionReportIndMsg->latitude = pz_LocPdData->cached_gnss_test_position.latitude;

    pz_PositionReportIndMsg->longitude_valid = TRUE;
    pz_PositionReportIndMsg->longitude = pz_LocPdData->cached_gnss_test_position.longitude;

    pz_PositionReportIndMsg->timestampUtc_valid = TRUE;
    pz_PositionReportIndMsg->timestampUtc = pz_LocPdData->cached_gnss_test_position.timestampUtc;

    pz_PositionReportIndMsg->horUncCircular_valid = TRUE;
    pz_PositionReportIndMsg->horUncCircular = pz_LocPdData->cached_gnss_test_position.horUncCircular;

    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

    pz_PositionReportIndMsg->sessionStatus = eQMI_LOC_SESS_STATUS_SUCCESS_V02;
    return TRUE;
  }
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

  // Filling intermediate position or unpropagated fix report here (may be overridden by final positions below)
  if (!v_ProcessPdInfo && (v_ProcessIntermPosInfo || v_ProcessUnpropagatedPosInfo))
  {
    // interm_pos_info_copy
    pdsm_pd_ext_pos_s_type *pz_ExtPosReport = NULL;
    pdsm_pd_data_type *pz_ExtPdData = NULL;

    if (v_ProcessUnpropagatedPosInfo)
    {
      // unpropagated_pos_info_copy
      pz_ExtPosReport = &pz_LocPdData->unpropagated_pos_info_copy.ext_status_info.ext_pos_report_type;
    }
    else
    {
      // interm_pos_info_copy
      pz_ExtPosReport = &pz_LocPdData->interm_pos_info_copy.ext_status_info.ext_pos_report_type;
    }
    pz_ExtPdData = &pz_ExtPosReport->pd_data;

    // Latitude and longitude
    if (pz_ExtPdData->ext_opt_field_mask & PDSM_PD_LAT_LONG_VALID)
    {
      // Do not set valid mask for intermediate positions
      if (l_IntermediatePosValid)
      {
        pz_PositionReportIndMsg->latitude_valid = TRUE;
        pz_PositionReportIndMsg->longitude_valid = TRUE;
      }

      /* Latitude: units in (2^-25 / Pi) rad, representing -90 to 90 degrees */
      /* Longitude: units in (2^-25 / Pi) rad, representing -180 to 180 degrees */
      pz_PositionReportIndMsg->latitude  = ((double)((signed)pz_ExtPdData->lat)) * 180.0 / (1 << 25);
      pz_PositionReportIndMsg->longitude = ((double)((signed)pz_ExtPdData->lon)) * 180.0 / (1 << 25);

      LOC_MSG_HIGH("locPd_GetReport course position lat: 0x%X or %d (/100)",
                   (int)pz_ExtPdData->lat, (int)pz_PositionReportIndMsg->latitude * 100, 0);
      LOC_MSG_HIGH("                course position lon: 0x%X or %d (/100)",
                   (int)pz_ExtPdData->lon, (int)pz_PositionReportIndMsg->longitude * 100, 0);
    }

    // Altitude
    if (pz_ExtPdData->opt_field_mask & PDSM_PD_ALTITUDE_VALID)
    {
      // Populate the altitude field
      if (l_IntermediatePosValid)
      {
        pz_PositionReportIndMsg->altitudeWrtEllipsoid_valid = TRUE;
      }
      pz_PositionReportIndMsg->altitudeWrtEllipsoid =
         pz_ExtPdData->ext_pos_data.f_altitude  - LOC_PD_ALTITUDE_WRT_WGS84_ELLIPSOID_OFFSET;

      if (pz_ExtPdData->ext_opt_field_mask & PDSM_PD_LAT_LONG_VALID)
      {
        if (l_IntermediatePosValid)
        {
          pz_PositionReportIndMsg->altitudeWrtMeanSeaLevel_valid = TRUE;
        }
        pz_PositionReportIndMsg->altitudeWrtMeanSeaLevel = pz_PositionReportIndMsg->altitudeWrtEllipsoid -
          loc_GetGeoidalSeparation(pz_PositionReportIndMsg->latitude, pz_PositionReportIndMsg->longitude);
      }
    }

    LOC_MSG_MED(" Validity Mask in position report from PDAPI = %d, %d ",
                pz_ExtPdData->opt_field_mask,
                pz_ExtPdData->ext_opt_field_mask,
                0);
    // Speed
    if ((pz_ExtPdData->ext_opt_field_mask & PDSM_PD_EXT_VELOCITY_VALID) ||
        (pz_ExtPdData->opt_field_mask & PDSM_PD_VELOCITY_VALID))
    {
      // Do not set valid mask for intermediate positions
      if (l_IntermediatePosValid)
      {
        LOC_MSG_LOW(" Got SPEED UNC = %d and adding it", (int)pz_ExtPdData->opt_field_mask, 0, 0);
        pz_PositionReportIndMsg->speedHorizontal_valid = TRUE;
        pz_PositionReportIndMsg->speedVertical_valid = TRUE;
        pz_PositionReportIndMsg->speedUnc_valid = TRUE;
      }

      if (pz_ExtPdData->ext_opt_field_mask & PDSM_PD_EXT_VELOCITY_VALID)
      {
        pz_PositionReportIndMsg->speedHorizontal  = ((float)pz_ExtPdData->ext_pos_data.f_velHorU) / 100;  /* original in cm/s */
      }
      else
      {
        pz_PositionReportIndMsg->speedHorizontal  = (pz_ExtPdData->ext_pos_data.f_velHor) / 100;  /* original in cm/s */
      }
      pz_PositionReportIndMsg->speedVertical  = (pz_ExtPdData->ext_pos_data.f_velVert) / 100;   /* original in cm/s */

      pz_PositionReportIndMsg->speedUnc = sqrt(
         ((float)pz_ExtPdData->ext_pos_data.velUncHoriz) * pz_ExtPdData->ext_pos_data.velUncHoriz +
         ((float)pz_ExtPdData->ext_pos_data.velUncVert) * pz_ExtPdData->ext_pos_data.velUncVert) / 100; /* original in cm/s */

    }

    // Heading
    if ((l_IntermediatePosValid) &&
        (pz_ExtPdData->ext_opt_field_mask & PDSM_PD_HEADING_VALID))
    {
      LOC_MSG_LOW(" Got heading UNC = %d and try to add it", (int)pz_ExtPdData->ext_opt_field_mask, 0, 0);
            // Heading is only valid if speed is valid and it is greater than 0.15m/sec
            if (pz_PositionReportIndMsg->speedHorizontal >= LOC_PD_MIN_VELOCTY)
      {
        LOC_MSG_LOW(" Got heading UNC = %d and adding it", (int)pz_ExtPdData->ext_opt_field_mask, 0, 0);
        pz_PositionReportIndMsg->heading_valid = TRUE;
        pz_PositionReportIndMsg->heading = ((float)pz_ExtPdData->heading * 360) / (1 << 10);
        pz_PositionReportIndMsg->headingUnc_valid = TRUE;
        pz_PositionReportIndMsg->headingUnc = pz_ExtPdData->ext_pos_data.heading_unc_rad * C_RAD_TO_DEG;
      }
    }

    // PD uncertainty not available, generalize from HEPE
    if (!(pz_ExtPdData->opt_field_mask & PDSM_PD_UNCERTAINTY_VALID))
    {
      if (l_IntermediatePosValid)
      {
        pz_PositionReportIndMsg->horUncEllipseSemiMajor_valid = TRUE;
        pz_PositionReportIndMsg->horUncEllipseSemiMinor_valid = TRUE;
      }
      pz_PositionReportIndMsg->horUncEllipseSemiMajor =
         pz_PositionReportIndMsg->horUncEllipseSemiMinor =
         INV_SQRT_2 * pz_ExtPdData->ext_pos_data.f_loc_unc_horizontal;
    }

    // PD uncertainty is available
    if (pz_ExtPdData->opt_field_mask & PDSM_PD_UNCERTAINTY_VALID)
    {
      if (l_IntermediatePosValid)
      {
        pz_PositionReportIndMsg->horUncCircular_valid = TRUE;
        pz_PositionReportIndMsg->horUncEllipseSemiMajor_valid = TRUE;
        pz_PositionReportIndMsg->horUncEllipseSemiMinor_valid = TRUE;
        pz_PositionReportIndMsg->horUncEllipseOrientAzimuth_valid = TRUE;
        pz_PositionReportIndMsg->vertUnc_valid = TRUE;
        pz_PositionReportIndMsg->horConfidence_valid = TRUE;
      }

      // Ellipse angle
      if (pz_ExtPdData->loc_uncertainty_a > pz_ExtPdData->loc_uncertainty_p)
      {
        if (pz_ExtPdData->ext_opt_field_mask & PDSM_PD_EXT_UNCERTAINTY_VALID)
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_ExtPdData->ext_pos_data.f_loc_uncertainty_ang * 5.625;
        }
        else
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_ExtPdData->loc_uncertainty_ang * 5.625;
        }
      }
      else
      {
        if (pz_ExtPdData->ext_opt_field_mask & PDSM_PD_EXT_UNCERTAINTY_VALID)
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_ExtPdData->ext_pos_data.f_loc_uncertainty_ang * 5.625 + 90;
        }
        else
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_ExtPdData->loc_uncertainty_ang * 5.625 + 90;
        }
      }

      // Elliptical and circular uncertainty
      pz_PositionReportIndMsg->horUncEllipseSemiMajor = pz_ExtPdData->ext_pos_data.f_alongAxisUnc; /* meters */
      pz_PositionReportIndMsg->horUncEllipseSemiMinor = pz_ExtPdData->ext_pos_data.f_perpAxisUnc;  /* meters */
      pz_PositionReportIndMsg->horUncCircular = pz_ExtPdData->ext_pos_data.f_loc_unc_horizontal;   /* meters */

      // Vertical uncertainty
      pz_PositionReportIndMsg->vertUnc = pz_ExtPdData->ext_pos_data.ext_loc_uncertainty_v;

      // Horizontal confidence
      pz_PositionReportIndMsg->horConfidence = pz_ExtPdData->loc_uncertainty_conf;
    }

    // Before LocMW 1.0u4, (Lat==0 && Lon==0) was treated as error case and the session status was not set
    // the position report would be ignored by at least Android driver
    // This behavior has been said to be wrong by marketing and we are going to report Lat/Lon 0/0 as valid, wild guess of the current location
    // Set valid_mask non-zero so the report is sent to clients
    pz_PositionReportIndMsg->sessionStatus = eQMI_LOC_SESS_STATUS_IN_PROGRESS_V02;
    // avoid translation to windows mobile generall error code

    // timestamp_utc is always valid
    locPd_FillTimestampUtc(pz_PositionReportIndMsg, pz_ExtPdData);

    //Populate Time Unc
    pz_PositionReportIndMsg->timeUnc_valid = TRUE;
    pz_PositionReportIndMsg->timeUnc = pz_ExtPdData->ext_pos_data.gps_time_unc_ms;

    // Populate technology mask
    pz_PositionReportIndMsg->technologyMask =
       locPd_ConvertTechnologyMaskFromPdapi(pz_ExtPdData->positioning_source);
    LOC_MSG_LOW(" locPd_GetReport interm_pos_info positioning_source = 0x%x technology_mask = 0x%x",
                pz_ExtPdData->positioning_source,
                pz_PositionReportIndMsg->technologyMask, 0);

    if (0 != pz_PositionReportIndMsg->technologyMask)
    {
      pz_PositionReportIndMsg->technologyMask_valid = TRUE;
    }

    // no validity bit defined in pdsm_pd_data_type
    {
      boolean v_sensorDataUsage = FALSE;
      v_sensorDataUsage = locPd_ConvertSensorUsageFromPdapi(pz_ExtPdData->w_SensorDataUsageMask,
                                                            &(pz_PositionReportIndMsg->sensorDataUsage.usageMask));
      if (TRUE == v_sensorDataUsage)
      {
        v_sensorDataUsage = locPd_ConvertSensorAidFromPdapi(pz_ExtPdData->w_SensorAidingIndicator,
                                                            &(pz_PositionReportIndMsg->sensorDataUsage.aidingIndicatorMask));
      }
      pz_PositionReportIndMsg->sensorDataUsage_valid = v_sensorDataUsage;
    }

    pz_PositionReportIndMsg->horReliability_valid = locPd_ConvertReliabilityFromPdapi(
       &(pz_PositionReportIndMsg->horReliability),
       pz_ExtPdData->e_HoriRelIndicator);
    pz_PositionReportIndMsg->vertReliability_valid = locPd_ConvertReliabilityFromPdapi(
       &(pz_PositionReportIndMsg->vertReliability),
       pz_ExtPdData->e_VertRelIndicator);

    // no validity bit defined in pdsm_pd_data_type
    // PDAPI refers these fields as self-validation as there are value combinations
    // indicating it's invalid
    {
      pz_PositionReportIndMsg->timeSrc_valid =
         locPd_ConvertTimeSrcFromPdapi(&(pz_PositionReportIndMsg->timeSrc), pz_ExtPdData->ext_pos_data.extended_time_source);
    }

    if (pz_ExtPdData->ext_opt_field_mask & PDSM_PD_LAT_LONG_VALID)
    {
      pz_PositionReportIndMsg->DOP_valid = TRUE;
      pz_PositionReportIndMsg->DOP.PDOP = (float)(pz_ExtPdData->ext_pos_data.p_dop) / 10.0f;
      pz_PositionReportIndMsg->DOP.HDOP = (float)(pz_ExtPdData->ext_pos_data.h_dop) / 10.0f;
      pz_PositionReportIndMsg->DOP.VDOP = (float)(pz_ExtPdData->ext_pos_data.v_dop) / 10.0f;

      pz_PositionReportIndMsg->extDOP_valid = TRUE;
      pz_PositionReportIndMsg->extDOP.PDOP = pz_PositionReportIndMsg->DOP.PDOP;
      pz_PositionReportIndMsg->extDOP.HDOP = pz_PositionReportIndMsg->DOP.HDOP;
      pz_PositionReportIndMsg->extDOP.VDOP = pz_PositionReportIndMsg->DOP.VDOP;
      pz_PositionReportIndMsg->extDOP.TDOP = (float)(pz_ExtPdData->ext_pos_data.t_dop) / 10.0f;
      pz_PositionReportIndMsg->extDOP.GDOP = (float)(pz_ExtPdData->ext_pos_data.g_dop) / 10.0f;
    }

    //convert the sv used information
    locPd_ConvertGnssNavSvInfoFromPdapi(pz_PositionReportIndMsg, &(pz_ExtPosReport->pd_data));

    // Cache network reference and GNSS position
    if (pz_ExtPdData->positioning_source == PDSM_PD_POSITION_SOURCE_DEFAULT ||
        pz_ExtPdData->positioning_source == PDSM_PD_POSITION_SOURCE_AFLT)
    {
      time_get_uptime_ms(z_TimeMs);
      pz_LocPdData->cached_nw_ref_pos_time = QW_CVT_Q2N(z_TimeMs);
      memscpy(&(pz_LocPdData->cached_nw_ref_pos),
              sizeof(pz_LocPdData->cached_nw_ref_pos),
              pz_PositionReportIndMsg, sizeof(*pz_PositionReportIndMsg));
    }
    else if (pz_ExtPdData->positioning_source == PDSM_PD_POSITION_SOURCE_GPS)
    {
      time_get_uptime_ms(z_TimeMs);
      pz_LocPdData->cached_gnss_pos_time = QW_CVT_Q2N(z_TimeMs);
      memscpy(&(pz_LocPdData->cached_gnss_pos),
              sizeof(pz_LocPdData->cached_gnss_pos),
              pz_PositionReportIndMsg, sizeof(*pz_PositionReportIndMsg));
    }

    // Populate the Spoof Report for Intermediate Position Report
    pz_PositionReportIndMsg->spoofReportMask_valid =
       locPd_ConvertSpoofReportFromPdapi(&pz_PositionReportIndMsg->spoofReportMask, &pz_ExtPdData->ext_pos_data.spoof_report);

    if (TRUE == pz_ExtPosReport->pd_data.ext_pos_data.v_JammerIndicatorListValid)
    {
      locPd_PopulateJammerInd(pz_PositionReportIndMsg, pz_ExtPosReport->pd_data.ext_pos_data.z_JammerIndicatorList);
    }
  } /* Intermediate positions */

  // Fields are populated according to the order in GPS_POSITION
  if (TRUE == v_ProcessPdInfo)
  {
    pdsm_pd_info_s_type         *pz_PdInfo = &(pz_LocPdData->pd_info_copy);
    pdsm_pd_data_type           *pz_PdData = &(pz_PdInfo->pd_info.pd_data);
    pdsm_pd_data_ext_s_type     *pz_PdDataExt = &(pz_PdInfo->pd_info.pd_ext_data);

    // avoid translation to windows mobile generall error code
    pz_PositionReportIndMsg->sessionStatus = eQMI_LOC_SESS_STATUS_SUCCESS_V02;

    // fill in utc time and get the leap_seconds, this is based on GPS timestamp
    l_LeapSeconds = locPd_FillTimestampUtc(pz_PositionReportIndMsg, pz_PdData);

    //Populate Time Unc
    pz_PositionReportIndMsg->timeUnc_valid = TRUE;
    pz_PositionReportIndMsg->timeUnc = pz_PdData->ext_pos_data.gps_time_unc_ms;

    // lat and lon is not encrypted
    if (PDSM_PD_CRYPT_ALG_NONE == pz_PdInfo->pd_info.encrypt_data.encrypt_alg)
    {
      // populate the latitude and longitude
      pz_PositionReportIndMsg->latitude_valid = TRUE;
      pz_PositionReportIndMsg->longitude_valid = TRUE;
      pz_PositionReportIndMsg->latitude  = ((double)pz_PdDataExt->lat) / LAT_LON_SCALE_FACTOR;
      pz_PositionReportIndMsg->longitude = ((double)pz_PdDataExt->lon) / LAT_LON_SCALE_FACTOR;
    }
    else
    {
      LOC_MSG_HIGH("locPd_GetReport, position encrypted", 0, 0, 0);
    }

    // Populate the altitude field
    pz_PositionReportIndMsg->altitudeWrtEllipsoid_valid = TRUE;
    pz_PositionReportIndMsg->altitudeWrtEllipsoid =
       pz_PdData->ext_pos_data.f_altitude  - LOC_PD_ALTITUDE_WRT_WGS84_ELLIPSOID_OFFSET;

    pz_PositionReportIndMsg->altitudeWrtMeanSeaLevel_valid = TRUE;
    pz_PositionReportIndMsg->altitudeWrtMeanSeaLevel = pz_PositionReportIndMsg->altitudeWrtEllipsoid -
       loc_GetGeoidalSeparation(pz_PositionReportIndMsg->latitude, pz_PositionReportIndMsg->longitude);

    if (pz_PdDataExt->velocity_include == 1)
    {
      pz_PositionReportIndMsg->speedHorizontal_valid = TRUE;
      pz_PositionReportIndMsg->speedVertical_valid = TRUE;
      pz_PositionReportIndMsg->magneticDeviation_valid = TRUE;
      pz_PositionReportIndMsg->speedHorizontal  = ((float)pz_PdData->velocity_hor) / 4;
      pz_PositionReportIndMsg->speedVertical  = ((float)pz_PdData->velocity_ver) / 2;
      pz_PositionReportIndMsg->magneticDeviation = ((float)pz_PdDataExt->magnetic_variation) / 10;
      pz_PositionReportIndMsg->velEnu_valid = TRUE;
      memscpy(pz_PositionReportIndMsg->velEnu, sizeof(pz_PositionReportIndMsg->velEnu),
              pz_PdDataExt->f_VelocityENUMps, sizeof(pz_PdDataExt->f_VelocityENUMps));
      pz_PositionReportIndMsg->velUncEnu_valid = TRUE;
      memscpy(pz_PositionReportIndMsg->velUncEnu, sizeof(pz_PositionReportIndMsg->velUncEnu),
              pz_PdDataExt->f_VelocityUncENUMps, sizeof(pz_PdDataExt->f_VelocityUncENUMps));

      //copy speed unc from ext_pos_data
      if ((pz_PdData->ext_opt_field_mask & PDSM_PD_EXT_VELOCITY_VALID) ||
          (pz_PdData->opt_field_mask & PDSM_PD_VELOCITY_VALID))
      {
        /* overwrite the speed values with EXT-POS if available */
        if (pz_PdData->ext_opt_field_mask & PDSM_PD_EXT_VELOCITY_VALID)
        {
          pz_PositionReportIndMsg->speedHorizontal  = ((float)pz_PdData->ext_pos_data.f_velHorU) / 100;  /* original in cm/s */
        }
        else
        {
          pz_PositionReportIndMsg->speedHorizontal  = (pz_PdData->ext_pos_data.f_velHor) / 100;  /* original in cm/s */
        }
        pz_PositionReportIndMsg->speedVertical  = (pz_PdData->ext_pos_data.f_velVert) / 100;   /* original in cm/s */

        pz_PositionReportIndMsg->speedUnc_valid = TRUE;
        /* convert PDAPI horizontal and vertical velocity unc to speed
           unc */
        pz_PositionReportIndMsg->speedUnc =
           sqrt(((float)pz_PdData->ext_pos_data.velUncHoriz) *
                pz_PdData->ext_pos_data.velUncHoriz +
                ((float)pz_PdData->ext_pos_data.velUncVert) *
                pz_PdData->ext_pos_data.velUncVert) / 100;
      }

         // Heading is only valid if speed is valid and it is greater than 0.15m/sec
        if (pz_PositionReportIndMsg->speedHorizontal>= LOC_PD_MIN_VELOCTY)
      {
        pz_PositionReportIndMsg->heading_valid = TRUE;
        pz_PositionReportIndMsg->heading = ((float)pz_PdDataExt->heading_true) / 10;
        //copy heading unc from ext_pos_data
        pz_PositionReportIndMsg->headingUnc_valid = TRUE;
        pz_PositionReportIndMsg->headingUnc = pz_PdData->ext_pos_data.heading_unc_rad * C_RAD_TO_DEG;
      }
    }

    // populate the uncertainty field
    if ((pz_PdData->opt_field_mask & PDSM_PD_UNCERTAINTY_VALID) != 0)
    {
      pz_PositionReportIndMsg->horUncEllipseSemiMajor_valid = TRUE;
      pz_PositionReportIndMsg->horUncEllipseSemiMinor_valid = TRUE;
      pz_PositionReportIndMsg->horUncEllipseOrientAzimuth_valid = TRUE;
      pz_PositionReportIndMsg->horUncCircular_valid = TRUE;
      pz_PositionReportIndMsg->vertUnc_valid = TRUE;
      pz_PositionReportIndMsg->horConfidence_valid = TRUE;


      // Elliptical and circular uncertainty
      // Note that in PDAPI, the angle is always 0-90 degrees, and length 'along' might be actually shorter
      // than length on the 'perpendicular' axis
      // However, we have decided to ensure the length 'along' to be longer, and change the range of angle
      // to 0-180 degrees
         if(pz_PdData->ext_pos_data.f_alongAxisUnc >= pz_PdData->ext_pos_data.f_perpAxisUnc)
      {
        pz_PositionReportIndMsg->horUncEllipseSemiMajor = pz_PdData->ext_pos_data.f_alongAxisUnc; /* meters */
        pz_PositionReportIndMsg->horUncEllipseSemiMinor = pz_PdData->ext_pos_data.f_perpAxisUnc;  /* meters */
        if (pz_PdData->ext_opt_field_mask & PDSM_PD_EXT_UNCERTAINTY_VALID)
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_PdData->ext_pos_data.f_loc_uncertainty_ang * 5.625;
        }
        else
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_PdData->loc_uncertainty_ang * 5.625;
        }
      }
      else
      {
        pz_PositionReportIndMsg->horUncEllipseSemiMinor = pz_PdData->ext_pos_data.f_alongAxisUnc; /* meters */
        pz_PositionReportIndMsg->horUncEllipseSemiMajor = pz_PdData->ext_pos_data.f_perpAxisUnc;  /* meters */
        if (pz_PdData->ext_opt_field_mask & PDSM_PD_EXT_UNCERTAINTY_VALID)
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_PdData->ext_pos_data.f_loc_uncertainty_ang * 5.625 + 90;
        }
        else
        {
          pz_PositionReportIndMsg->horUncEllipseOrientAzimuth = pz_PdData->loc_uncertainty_ang * 5.625 + 90;
        }
      }
      pz_PositionReportIndMsg->horUncCircular = pz_PdData->ext_pos_data.f_loc_unc_horizontal; /* meters */

      // Vertical uncertainty
      pz_PositionReportIndMsg->vertUnc = pz_PdData->ext_pos_data.ext_loc_uncertainty_v;

      // Horizontal confidence
      pz_PositionReportIndMsg->horConfidence = pz_PdData->loc_uncertainty_conf;
    }

    // Populate technology mask
    pz_PositionReportIndMsg->technologyMask =
       locPd_ConvertTechnologyMaskFromPdapi(pz_PdData->positioning_source);
    LOC_MSG_LOW(" locPd_GetReport final_pos_info positioning_source = 0x%x technology_mask = 0x%x",
                pz_PdData->positioning_source,
                pz_PositionReportIndMsg->technologyMask, 0);

    if (0 != pz_PositionReportIndMsg->technologyMask)
    {
      pz_PositionReportIndMsg->technologyMask_valid = TRUE;
    }

    // Populate the SV info
    locPd_GetSvReport(pz_GnssSvInfoIndMsg);

    // Altitude assumed or calcuated information
    pz_PositionReportIndMsg->altitudeAssumed_valid = TRUE;  /* for position report */


    // Populate fix type, no dwValidFields for FixType
    if (PDSM_PD_FIX_TYPE_2D == pz_PdDataExt->fix_type)
    {
      /* for SV report */
      pz_GnssSvInfoIndMsg->altitudeAssumed = TRUE;

      /* for Position report */
      pz_PositionReportIndMsg->altitudeAssumed_valid = TRUE;
      pz_PositionReportIndMsg->altitudeAssumed = TRUE;

    }
    else if (PDSM_PD_FIX_TYPE_3D == pz_PdDataExt->fix_type)
    {
      /* for SV report */
      pz_GnssSvInfoIndMsg->altitudeAssumed = TRUE;

      /* for Position report */
      pz_PositionReportIndMsg->altitudeAssumed_valid = TRUE;
      pz_PositionReportIndMsg->altitudeAssumed = FALSE;
    }

    if (TRUE == pz_PdDataExt->v_JammerIndicatorListValid)
    {
      locPd_PopulateJammerInd(pz_PositionReportIndMsg, pz_PdDataExt->z_JammerIndicatorList);
    }

    // no validity bit defined in pdsm_pd_data_type
    {
      boolean v_sensorDataUsage = FALSE;
      v_sensorDataUsage = locPd_ConvertSensorUsageFromPdapi(pz_PdData->w_SensorDataUsageMask,
                                                            &(pz_PositionReportIndMsg->sensorDataUsage.usageMask));
      if (TRUE == v_sensorDataUsage)
      {
        v_sensorDataUsage = locPd_ConvertSensorAidFromPdapi(pz_PdData->w_SensorAidingIndicator,
                                                            &(pz_PositionReportIndMsg->sensorDataUsage.aidingIndicatorMask));
      }
      pz_PositionReportIndMsg->sensorDataUsage_valid = v_sensorDataUsage;
    }
    pz_PositionReportIndMsg->horReliability_valid = locPd_ConvertReliabilityFromPdapi(
       &(pz_PositionReportIndMsg->horReliability),
       pz_PdData->e_HoriRelIndicator);
    pz_PositionReportIndMsg->vertReliability_valid = locPd_ConvertReliabilityFromPdapi(
       &(pz_PositionReportIndMsg->vertReliability),
       pz_PdData->e_VertRelIndicator);

    // no validity bit defined in pdsm_pd_data_type
    // PDAPI refers these fields as self-validation as there are value combinations
    // indicating it's invalid
    {
      pz_PositionReportIndMsg->timeSrc_valid =
         locPd_ConvertTimeSrcFromPdapi(&(pz_PositionReportIndMsg->timeSrc), pz_PdData->ext_pos_data.extended_time_source);
    }

    if (pz_PdData->ext_opt_field_mask & PDSM_PD_LAT_LONG_VALID)
    {
      pz_PositionReportIndMsg->DOP_valid = TRUE;
      pz_PositionReportIndMsg->DOP.PDOP = (float)(pz_PdData->ext_pos_data.p_dop) / 10.0f;
      pz_PositionReportIndMsg->DOP.HDOP = (float)(pz_PdData->ext_pos_data.h_dop) / 10.0f;
      pz_PositionReportIndMsg->DOP.VDOP = (float)(pz_PdData->ext_pos_data.v_dop) / 10.0f;

      pz_PositionReportIndMsg->extDOP_valid = TRUE;
      pz_PositionReportIndMsg->extDOP.PDOP = pz_PositionReportIndMsg->DOP.PDOP;
      pz_PositionReportIndMsg->extDOP.HDOP = pz_PositionReportIndMsg->DOP.HDOP;
      pz_PositionReportIndMsg->extDOP.VDOP = pz_PositionReportIndMsg->DOP.VDOP;
      pz_PositionReportIndMsg->extDOP.TDOP = (float)(pz_PdData->ext_pos_data.t_dop) / 10.0f;
      pz_PositionReportIndMsg->extDOP.GDOP = (float)(pz_PdData->ext_pos_data.g_dop) / 10.0f;
    }

    //convert the sv used information
    locPd_ConvertGnssNavSvInfoFromPdapi(pz_PositionReportIndMsg, pz_PdData);

    // Cache network reference and GNSS position
    if (pz_PdData->positioning_source == PDSM_PD_POSITION_SOURCE_DEFAULT ||
        pz_PdData->positioning_source == PDSM_PD_POSITION_SOURCE_AFLT)
    {
      time_get_uptime_ms(z_TimeMs);
      pz_LocPdData->cached_nw_ref_pos_time = QW_CVT_Q2N(z_TimeMs);
      memscpy(&(pz_LocPdData->cached_nw_ref_pos),
              sizeof(pz_LocPdData->cached_nw_ref_pos),
              pz_PositionReportIndMsg, sizeof(*pz_PositionReportIndMsg));
    }
    else if (pz_PdData->positioning_source == PDSM_PD_POSITION_SOURCE_GPS)
    {
      time_get_uptime_ms(z_TimeMs);
      pz_LocPdData->cached_gnss_pos_time = QW_CVT_Q2N(z_TimeMs);
      memscpy(&(pz_LocPdData->cached_gnss_pos),
              sizeof(pz_LocPdData->cached_gnss_pos),
              pz_PositionReportIndMsg, sizeof(*pz_PositionReportIndMsg));
    }

    // Populate the Spoof Report for Final Position Report
    pz_PositionReportIndMsg->spoofReportMask_valid =
       locPd_ConvertSpoofReportFromPdapi(&pz_PositionReportIndMsg->spoofReportMask, &pz_PdData->ext_pos_data.spoof_report);
  }

  // process the error event
  if (TRUE == v_ProcessPdSessEndStatus)
  {
    // avoid translation to windows mobile generall error code
    pz_PositionReportIndMsg->sessionStatus = locPd_XlateFixEndReasonCode(pz_LocPdData->pd_sess_end_status_copy);
  }
  else if (TRUE == v_ProcessPdCmdErr)
  {
    // avoid translation to windows mobile generall error code
    pz_PositionReportIndMsg->sessionStatus = locPd_XlateCmdErrCodeToSessStatus(pz_LocPdData->pd_cmd_err_copy);
  }

  return TRUE;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to Unswap the Float data to match the system Endianess

@param[in]   p_floatData      char data Pointer

@retval    Float   Float data after swapping of bytes
*/ /* ------------------------------------------------------------------------*/

float FloatUnSwap(const unsigned char *p_floatData)
{
  union {
    float swappedFloatData;
    byte b[4];
  } data1, data2;

  if (NULL == p_floatData)
  {
    LOC_MSG_ERROR("FloatUnSwap NULL input pointer", 0, 0, 0);
    return 0;
  }

  data1.b[0] = p_floatData[0];
  data1.b[1] = p_floatData[1];
  data1.b[2] = p_floatData[2];
  data1.b[3] = p_floatData[3];

  data2.b[0] = data1.b[3];
  data2.b[1] = data1.b[2];
  data2.b[2] = data1.b[1];
  data2.b[3] = data1.b[0];

  return data2.swappedFloatData;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to Unswap the Double data to match the system Endianess

@param[in]   p_doubleData      char data Pointer

@retval    Double Double data after swapping of bytes
*/ /* ------------------------------------------------------------------------*/

double DoubleUnSwap(const unsigned char *p_doubleData)
{
  union {
    double swappedDoubleData;
    byte b[8];
  } data1, data2;

  if (NULL == p_doubleData)
  {
    LOC_MSG_ERROR("DoubleUnSwap NULL input pointer", 0, 0, 0);
    return 0;
  }

  data1.b[0] = p_doubleData[0];
  data1.b[1] = p_doubleData[1];
  data1.b[2] = p_doubleData[2];
  data1.b[3] = p_doubleData[3];
  data1.b[4] = p_doubleData[4];
  data1.b[5] = p_doubleData[5];
  data1.b[6] = p_doubleData[6];
  data1.b[7] = p_doubleData[7];

  data2.b[0] = data1.b[7];
  data2.b[1] = data1.b[6];
  data2.b[2] = data1.b[5];
  data2.b[3] = data1.b[4];
  data2.b[4] = data1.b[3];
  data2.b[5] = data1.b[2];
  data2.b[6] = data1.b[1];
  data2.b[7] = data1.b[0];

  return data2.swappedDoubleData;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to return the size of the data type enum used in Opaque data Blob

@param[in]   dataType          Enum data Type

@retval    Size                 Size of Data Type
*/ /* ------------------------------------------------------------------------*/

size_t sizeofDataType(qmiLocSecureMessageDataTypeEnumT_v02 dataType)
{
  size_t size = 0;
  switch (dataType)
  {
    case eQMI_LOC_SECURE_DATA_TYPE_BYTE_8_V02 :
      size = sizeof(byte);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_CHAR_8_V02 :
      size = sizeof(char);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_CHAR_8_V02 :
      size = sizeof(char);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_INT_8_V02 :
      size = sizeof(int8);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_8_V02 :
      size = sizeof(uint8);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_INT_16_V02 :
      size = sizeof(int16);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_16_V02 :
      size = sizeof(uint16);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_INT_32_V02 :
      size = sizeof(int32);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02 :
      size = sizeof(uint32);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_INT_64_V02 :
      size = sizeof(int64);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_64_V02 :
      size = sizeof(uint64);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_BOOL_8_V02 :
      size = sizeof(boolean);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_DOUBLE_64_V02 :
      size = sizeof(double);
      break;
    case eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02:
      size = sizeof(float);
      break;
    default :
      size = 0;
      break;
  }
  return size;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to Extract Value from the given TLV data Blob based on the data type

@param[OUT]  dest                Dest buffer with decoded value
@param[in]   Paramsize           Length of  parameter
@param[in]   type                Type of Start Fix parameter
@param[in]   src                 Data blob 


@retval    TRUE                   Message successfully translated
@retval    FALSE                  Message could not be translated successfully
*/ /* ------------------------------------------------------------------------*/

boolean ExtractValue(void *p_destBuff, size_t Paramsize, qmiLocSecureMessageDataTypeEnumT_v02 type, const uint8 *p_srcBuff)
{
  size_t size = 0;
  size_t DataTypeSize = sizeofDataType(type);
  if (NULL == p_srcBuff || NULL == p_destBuff)
  {
    LOC_MSG_ERROR("ExtractValue p_srcBuff=%x, p_destBuff=%x",\
                     p_srcBuff, p_destBuff, 0);
    return FALSE;
  }
  if (DataTypeSize != Paramsize)
  {
    LOC_MSG_ERROR("ExtractValue: DataTypeSize %d != Paramsize %d",\
                     DataTypeSize, Paramsize, 0);
    return FALSE;
  }
  switch (type)
  {
    case eQMI_LOC_SECURE_DATA_TYPE_BYTE_8_V02:
      {
        byte *temp = (byte *)p_destBuff;
        *temp = Extract1Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_CHAR_8_V02:
      {
        char *temp = (char *)p_destBuff;
        *temp = Extract1Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_CHAR_8_V02:
      {
        char *temp = (char *)p_destBuff;
        *temp = Extract1Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_INT_8_V02:
      {
        int8 *temp = (int8 *)p_destBuff;
        *temp = Extract1Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_8_V02:
      {
        uint8 *temp = (uint8 *)p_destBuff;
        *temp = Extract1Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_INT_16_V02:
      {
        int16 *temp = (int16 *)p_destBuff;
        *temp = Extract2Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_16_V02:
      {
        uint16 *temp = (uint16 *)p_destBuff;
        *temp = Extract2Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_INT_32_V02:
      {
        int32 *temp = (int32 *)p_destBuff;
        *temp = Extract4Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02:
      {
        uint32 *temp = (uint32 *)p_destBuff;
        *temp = Extract4Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_INT_64_V02:
      {
        int64 *temp1 = (int64 *)p_destBuff;
        int64 temp = 0;
        const unsigned char *t1 = NULL;
        t1 = p_srcBuff + 4;
        temp = (((temp & 0x0000000000000000) | (Extract4Bytes(p_srcBuff))) << 32) | (Extract4Bytes(t1));
        *temp1 = temp;
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_64_V02:
      {
        uint64 *temp1 = (uint64 *)p_destBuff;
        uint64 temp = 0;
        const unsigned char *t1 = NULL;
        t1 = p_srcBuff + 4;
        temp = (((temp & 0x0000000000000000) | (Extract4Bytes(p_srcBuff))) << 32) | (Extract4Bytes(t1));
        *temp1 = temp;
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_BOOL_8_V02:
      {
        boolean *temp = (boolean *)p_destBuff;
        *temp = Extract4Bytes(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_DOUBLE_64_V02:
      {
        double *temp = (double *)p_destBuff;
        *temp = DoubleUnSwap(p_srcBuff);
        break;
      }
    case eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02:
      {
        float *temp = (float *)p_destBuff;
        *temp = FloatUnSwap(p_srcBuff);
        break;
      }
    default :
      return FALSE;
  }
  return TRUE;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to decode the data blob of GetAvailablePosition Request in Internal Structure.

@param[in]   src                Input Data Blob
@param[in]   src_length         Length of Data Blob

@param[out]  pz_DecodedGetAvailablePosReq         GetAvailable Position request parameters after decoding

@retval    TRUE                   Message successfully translated
@retval    FALSE                  Message could not be translated successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_PopulateLocGetSecureAvailablePositionParam
(
   const uint8 *p_srcDataBlob,
   uint32 src_length,
   qmiLocGetAvailablePositionReqParamMsgT_v02 *pz_DecodedGetAvailablePosReq
   )
{
  boolean v_RetVal = TRUE;
  qmiLocSecureGetAvailablePosParameterIDEnumT_v02 paramId = QMILOCSECUREGETAVAILABLEPOSPARAMETERIDENUMT_MIN_ENUM_VAL_V02;
  qmiLocSecureMessageDataTypeEnumT_v02 paramType = QMILOCSECUREMESSAGEDATATYPEENUMT_MIN_ENUM_VAL_V02;
  uint8 num_of_data_type = 0;
  uint32 paramBloblength = 0;
  uint32 byte_count = 0;
  LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: data_length passed in GetAvailablePositionReq param %d ", src_length, 0, 0);

  for (byte_count = 0; byte_count < src_length; byte_count += paramBloblength)
  {
    uint32 cur_byteCount = byte_count;
    size_t DataTypeSize = 0;
    paramBloblength = 0;

    // Verify if Parametr , Type and Length data is present to decode the data blob for this parameter
    if ((cur_byteCount + sizeof(qmiLocSecureGetAvailablePosParameterIDEnumT_v02)
         + sizeof(qmiLocSecureMessageDataTypeEnumT_v02)
         + sizeof(uint8)) >= src_length)
    {
      LOC_MSG_MED("\nDecodePositionRptIndicationParam: Data Blob doesnt have Parametr, Type and Length data to decode", 0, 0, 0);
      return FALSE;
    }
    // Decode Parameter Blob
    paramId = (qmiLocSecureGetAvailablePosParameterIDEnumT_v02)(Extract4Bytes(p_srcDataBlob));
    p_srcDataBlob = p_srcDataBlob + sizeof(qmiLocSecureGetAvailablePosParameterIDEnumT_v02);
    LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: ParamID %d ", paramId, 0, 0);
    paramBloblength += sizeof(qmiLocSecureGetAvailablePosParameterIDEnumT_v02);

    // Decode data Type Blob
    paramType = (qmiLocSecureMessageDataTypeEnumT_v02)(Extract4Bytes(p_srcDataBlob));
    p_srcDataBlob = p_srcDataBlob + sizeof(qmiLocSecureMessageDataTypeEnumT_v02);
    LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: Param Data Type %d ", paramType, 0, 0);
    paramBloblength += sizeof(qmiLocSecureMessageDataTypeEnumT_v02);

    // Length of Data Type
    num_of_data_type = Extract1Bytes(p_srcDataBlob);
    p_srcDataBlob = p_srcDataBlob + sizeof(uint8);
    LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam:Param Data Type Number %d ", num_of_data_type, 0, 0);
    paramBloblength += sizeof(uint8);

    DataTypeSize = sizeofDataType(paramType);

    // Verify if data is enough to decode the data blob for this parameter
    if ((cur_byteCount + paramBloblength
         + (DataTypeSize  * num_of_data_type)) > src_length)
    {
      LOC_MSG_ERROR("locPd_PopulateLocGetSecureAvailablePositionParam : Data Blob doesnt have enough data to decode", 0, 0, 0);
      return FALSE;
    }

    if (num_of_data_type == DATA_TYPE_LENGTH_ZERO)
    {
      // This Type Field is not valid.
      // can Ignore it and continue
      // generally it should not happen, because if Field is not valid, Client should not put the param ID in the data buffer
      continue;
    }
    LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam :Processing param ID %d ", paramId, 0, 0);
    switch (paramId)
    {
      case eQMI_LOC_SECURE_GET_AVAILABLE_POS_PARAM_TRANSACTION_ID_V02 :
        if (num_of_data_type == DATA_TYPE_LENGTH_ONE)
        {
          v_RetVal = ExtractValue(&pz_DecodedGetAvailablePosReq->transactionId,
                                  sizeof(pz_DecodedGetAvailablePosReq->transactionId), paramType, p_srcDataBlob);
          LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: transactionId value %d ",
                      pz_DecodedGetAvailablePosReq->transactionId, 0, 0);
        }
        else
        {
          v_RetVal =  FALSE;
        }
        break;
      case eQMI_LOC_SECURE_GET_AVAILABLE_POS_PARAM_NONCE_V02:
        if (num_of_data_type == DATA_TYPE_LENGTH_ONE)
        {
          v_RetVal = ExtractValue(&pz_DecodedGetAvailablePosReq->nonce, sizeof(pz_DecodedGetAvailablePosReq->nonce), paramType, p_srcDataBlob);
          pz_DecodedGetAvailablePosReq->nonce_valid = TRUE;
          LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: nonce value %016llX ", pz_DecodedGetAvailablePosReq->nonce, 0, 0);
        }
        else
        {
          v_RetVal =  FALSE;
        }
        break;
      case eQMI_LOC_SECURE_GET_AVAILABLE_POS_PARAM_SOURCE_V02:
        if (num_of_data_type == DATA_TYPE_LENGTH_ONE)
        {
          uint32 p_tempSrcDataBlob = 0;
          v_RetVal = ExtractValue(&p_tempSrcDataBlob, sizeof(pz_DecodedGetAvailablePosReq->locPositionSource), paramType, p_srcDataBlob);
          pz_DecodedGetAvailablePosReq->locPositionSource_valid = TRUE;
          if (p_tempSrcDataBlob == eQMI_LOC_POS_SRC_GNSS_V02)
          {
            pz_DecodedGetAvailablePosReq->locPositionSource = eQMI_LOC_POS_SRC_GNSS_V02;
          }
          else if (p_tempSrcDataBlob == 0x02)
          {
            pz_DecodedGetAvailablePosReq->locPositionSource = 0x02;
          }
          else
          {
            //Wrong Input Data
            v_RetVal =  FALSE;
          }
          LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: locPositionSource %d ", pz_DecodedGetAvailablePosReq->locPositionSource, 0, 0);
        }
        else
        {
          v_RetVal =  FALSE;
        }
        break;
      case eQMI_LOC_SECURE_GET_AVAILABLE_POS_PARAM_REPORT_DATA_SECURITY_MODE_V02:
        if (num_of_data_type == DATA_TYPE_LENGTH_ONE)
        {
          uint32 p_tempSrcDataBlob = 0;
          v_RetVal = ExtractValue(&p_tempSrcDataBlob, sizeof(pz_DecodedGetAvailablePosReq->locPositionRptDataMode), paramType, p_srcDataBlob);
          pz_DecodedGetAvailablePosReq->locPositionRptDataMode_valid = TRUE;
          if (p_tempSrcDataBlob == eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED_V02)
          {
            pz_DecodedGetAvailablePosReq->locPositionRptDataMode = eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED_V02;
          }
          else if (p_tempSrcDataBlob == eQMI_LOC_SECURE_LOC_DATA_UNENCRYPTED_V02)
          {
            pz_DecodedGetAvailablePosReq->locPositionRptDataMode = eQMI_LOC_SECURE_LOC_DATA_UNENCRYPTED_V02;
          }
          else
          {
            //Wrong Input Data
            v_RetVal =  FALSE;
          }
          LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: locPositionRptDataMode value %d ", pz_DecodedGetAvailablePosReq->locPositionRptDataMode, 0, 0);
        }
        else
        {
          v_RetVal =  FALSE;
        }
        break;
      case eQMI_LOC_SECURE_GET_AVAILABLE_POS_PARAM_REPORT_DATA_TIME_PROPAGATION_V02:
        if (num_of_data_type == DATA_TYPE_LENGTH_ONE)
        {
          uint32 p_tempSrcDataBlob = 0;
          v_RetVal = ExtractValue(&p_tempSrcDataBlob, sizeof(pz_DecodedGetAvailablePosReq->locPositionRptDataTimePropogation), paramType, p_srcDataBlob);

          pz_DecodedGetAvailablePosReq->locPositionRptDataTimePropogation_valid = TRUE;
          if (p_tempSrcDataBlob == eQMI_LOC_POS_REPORT_PROPAGATED_TO_CURRENT_UTC_V02)
          {
            pz_DecodedGetAvailablePosReq->locPositionRptDataTimePropogation = eQMI_LOC_POS_REPORT_PROPAGATED_TO_CURRENT_UTC_V02;
          }
          else
          {
            //Wrong Input Data
            v_RetVal =  FALSE;
          }
          LOC_MSG_MED("locPd_PopulateLocGetSecureAvailablePositionParam: locPositionRptDataTimePropogation value %d ", pz_DecodedGetAvailablePosReq->locPositionRptDataTimePropogation, 0, 0);
        }
        else
        {
          v_RetVal =  FALSE;
        }
        break;
      default:
        {
          v_RetVal =  FALSE;
        }
        break;
    }

    if (v_RetVal == FALSE)
    {
      return v_RetVal;
    }
    else
    {
      paramBloblength += (DataTypeSize * num_of_data_type);
      if (num_of_data_type == DATA_TYPE_LENGTH_ONE)
      {
        p_srcDataBlob = p_srcDataBlob + DataTypeSize;
      }
      else
      {
        // p_srcDataBlob increment is taken care while extracting data from array
      }
    }
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
function to  Parse the Data Blob, verify and  fill in the internal structure for Get Secure Available Position

@param[in]   pz_Msg               Qmi-Loc Format of Message
@param[out]  pz_IoctlInfo         Loc-API IOCTL Format 

@retval    TRUE                   Message successfully translated
@retval    FALSE                  Message could not be translated successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_ConvToLocGetAvailablePosition
(
   const qmiLocSecureGetAvailablePositionReqMsgT_v02 *pz_SecureGetAvailablePosReq,
   qmiLocGetAvailablePositionReqParamMsgT_v02        *pz_GetAvailablePosReq
   )
{
  boolean v_RetVal = TRUE;
  qmiLocGetAvailablePositionReqParamMsgT_v02   *pz_DecodedGetAvailablePosReq =
     (qmiLocGetAvailablePositionReqParamMsgT_v02 *)loc_calloc(
     sizeof(qmiLocGetAvailablePositionReqParamMsgT_v02));
#if LOC_PD_SECURE_DEBUG
  const uint8 *SSdata = pz_SecureGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData;
  uint32 count = 0;
#endif

  if (pz_DecodedGetAvailablePosReq == NULL)
  {
    LOC_MSG_ERROR("locPd_ConvToLocGetAvailablePosition: memory Allocation failed", 0, 0, 0);
    v_RetVal =  FALSE;
    return v_RetVal;
  }
  memset((void *)pz_DecodedGetAvailablePosReq, 0, sizeof(*pz_DecodedGetAvailablePosReq));

  /* Static function only called within loc_pd.c,
     skip input parameter NULL pointer check */

#if LOC_PD_SECURE_DEBUG
  // Enable only for Debugging
  // Message is in TLV format
  //   PARAMETER TYPE LENGTH VALUE  ...........PARAMETER TYPE LENGTH  VALUE
  for (; count < pz_SecureGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len; count++)
  {
    LOC_MSG_MED("locPd_ConvToLocGetAvailablePosition data:  0x%x ", *SSdata, 0, 0);
    SSdata++;
  }
#endif
  v_RetVal = locPd_PopulateLocGetSecureAvailablePositionParam(
     &pz_SecureGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData[0],
     pz_SecureGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len,
     pz_DecodedGetAvailablePosReq);

  if ((TRUE == v_RetVal) &&
      (FALSE == pz_DecodedGetAvailablePosReq->nonce_valid) &&
      (eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED_V02 == pz_SecureGetAvailablePosReq->secureLocDataMode))
  {
    // Start param data blob doesnt have Nonce or Session ID.
    // return a Error Response
    LOC_MSG_MED("locPd_ConvToLocGetAvailablePosition doesnt have Nonce or Session ID.", 0, 0, 0);
    v_RetVal = FALSE;
  }

  if (TRUE == v_RetVal)
  {
    pz_GetAvailablePosReq->transactionId = pz_DecodedGetAvailablePosReq->transactionId;
  }

  if ((TRUE == v_RetVal) && pz_DecodedGetAvailablePosReq->nonce_valid)
  {
    pz_GetAvailablePosReq->nonce = pz_DecodedGetAvailablePosReq->nonce;
    pz_GetAvailablePosReq->nonce_valid = TRUE;
  }

  if ((TRUE == v_RetVal) && (pz_DecodedGetAvailablePosReq->locPositionSource_valid))
  {
    pz_GetAvailablePosReq->locPositionSource = pz_DecodedGetAvailablePosReq->locPositionSource;
    pz_GetAvailablePosReq->locPositionSource_valid = TRUE;
  }
  if ((TRUE == v_RetVal) && (pz_DecodedGetAvailablePosReq->locPositionRptDataMode_valid))
  {
    pz_GetAvailablePosReq->locPositionRptDataMode = pz_DecodedGetAvailablePosReq->locPositionRptDataMode;
    pz_GetAvailablePosReq->locPositionRptDataMode_valid = TRUE;
  }
  if ((TRUE == v_RetVal) && (pz_DecodedGetAvailablePosReq->locPositionRptDataTimePropogation_valid))
  {
    pz_GetAvailablePosReq->locPositionRptDataTimePropogation = pz_DecodedGetAvailablePosReq->locPositionRptDataTimePropogation;
    pz_GetAvailablePosReq->locPositionRptDataTimePropogation_valid = TRUE;
  }
  if (TRUE == v_RetVal)
  {
    LOC_MSG_MED("locPd_ConvToLocGetAvailablePosition : *********Secure Get Available request param. ***********", 0, 0, 0);
    LOC_MSG_MED("locPd_ConvToLocGetAvailablePosition : transactionId %d ",
                pz_GetAvailablePosReq->transactionId, 0, 0);
    LOC_MSG_MED("locPd_ConvToLocGetAvailablePosition : locPositionSource_valid %d  locPositionSource %d ",
                pz_GetAvailablePosReq->locPositionSource_valid,
                pz_GetAvailablePosReq->locPositionSource, 0);
    LOC_MSG_MED("locPd_ConvToLocGetAvailablePosition : locPositionRptDataMode_valid %d  locPositionRptDataMode %d ",
                pz_GetAvailablePosReq->locPositionRptDataMode_valid,
                pz_GetAvailablePosReq->locPositionRptDataMode, 0);
    LOC_MSG_MED("locPd_ConvToLocGetAvailablePosition : locPositionRptDataTimePropogation_valid %d  locPositionRptDataTimePropogation %d ",
                pz_GetAvailablePosReq->locPositionRptDataTimePropogation_valid,
                pz_GetAvailablePosReq->locPositionRptDataTimePropogation, 0);
  }
  if (NULL != pz_DecodedGetAvailablePosReq)
  {
    loc_free(pz_DecodedGetAvailablePosReq);
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
function to Decrypt  the Data Blob, using Secure API

@param[in]   pz_EncryptedMsg         Qmi-Loc Format of Message having Encrypted Packet
@param[out]  pz_DecryptedMsg         Decrypted Data Blob 

@retval    TRUE                   Message successfully translated
@retval    FALSE                  Message could not be translated successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_DecryptSecureAvailablePositionDataBlob(const void *pz_EncryptedMsg, void *pz_DecryptedMsg)
{
  boolean v_RetVal = TRUE;
  secerrno_enum_type status = E_SUCCESS;

  qmiLocSecureGetAvailablePositionReqMsgT_v02    *pz_EncGetAvailablePosReq =
     (qmiLocSecureGetAvailablePositionReqMsgT_v02 *)pz_EncryptedMsg;

  qmiLocSecureGetAvailablePositionReqMsgT_v02    *pz_DecGetAvailablePosReq =
     (qmiLocSecureGetAvailablePositionReqMsgT_v02 *)pz_DecryptedMsg;

#if LOC_PD_SECURE_DEBUG
  uint32 j = 0;
  for (; j < pz_EncGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len; j++)
  {
    LOC_MSG_MED("[DEBUG] Before Encryption Data %d",  pz_EncGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData[j], 0, 0);
  }
  LOC_MSG_MED("[DEBUG] Before Encryption Data Length %d", pz_EncGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len, 0, 0);
#endif

  // Decrypt this
  //pz_GetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData
  LOC_MSG_HIGH("locPd_DecryptSecureAvailablePositionDataBlob Secure Message Decryption  ", 0, 0, 0);
  do
  {
    status = secapi_secure_channel_init();
    if (status != E_SUCCESS && status != E_ALREADY_DONE)
    {
      LOC_MSG_MED("secure channel init failed", 0, 0, 0);
      v_RetVal = FALSE;
      break;
    }
    // Initialize the Output data length
    pz_DecGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len =
       pz_EncGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len;
    status = secapi_authenticate_decrypt_message(SC_SSID_TZ,
                                                 SC_CID_TLOC, (uint8 *)pz_EncGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData,
                                                 pz_EncGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len,
                                                 (uint8 *)pz_DecGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData,
                                                 &pz_DecGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len);

    if (E_SUCCESS != status)
    {
      LOC_MSG_MED("locPd_DecryptSecureAvailablePositionDataBlob failed. Status %d", status, 0, 0);
      v_RetVal = FALSE;
      break;
    }
    LOC_MSG_MED("locPd_DecryptSecureAvailablePositionDataBlob Passed. len=%d",
                pz_DecGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len, 0, 0);
  } while (0);

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function To decrypt and decode the Secure Best Available Position Parameter

@param[in]   pz_SecureAvailablePositionReq    Qmi-Loc Format of Message
@param[out]  pz_AvailablePositionReq          Parsed Get Available Position message


@retval    TRUE       Decrypt QMI-LOC message successfully
@retval    FALSE      Decrypt QMI-LOC message failed
*/ /* ------------------------------------------------------------------------*/
static boolean locPd_ConvToLocSecureAvailablePosition
(
   const qmiLocSecureGetAvailablePositionReqMsgT_v02 *pz_SecureGetAvailablePosReq,
   qmiLocGetAvailablePositionReqParamMsgT_v02        *pz_GetAvailablePosReq
   )
{
  boolean v_RetVal = FALSE;
  qmiLocSecureGetAvailablePositionReqMsgT_v02 *pz_TempSecureGetAvailablePosReq = NULL;
  qmiLocSecureGetAvailablePositionReqMsgT_v02 *pz_DecryptedAvailablePosReq =
     (qmiLocSecureGetAvailablePositionReqMsgT_v02 *)loc_calloc(sizeof(qmiLocSecureGetAvailablePositionReqMsgT_v02));
#if LOC_PD_SECURE_DEBUG
  const uint8 *SSdata = pz_SecureGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData;
  uint32 count;
#endif

  if (NULL == pz_DecryptedAvailablePosReq)
  {
    LOC_MSG_ERROR("locPd_ConvToLocSecureAvailablePosition: calloc failed",
                  0, 0, 0);
    return v_RetVal;
  }
  memset((void *)pz_DecryptedAvailablePosReq, 0,
         sizeof(*pz_DecryptedAvailablePosReq));

  LOC_MSG_MED("locPd_ConvToLocSecureAvailablePosition: Secure Message Mode = %d",
              pz_SecureGetAvailablePosReq->secureLocDataMode, 0, 0);

#if LOC_PD_SECURE_DEBUG
  // Enable only for Debugging
  // Message is in TLV format
  //   PARAMETER TYPE LENGTH VALUE  ...........PARAMETER TYPE LENGTH  VALUE
  for (count = 0; count < pz_SecureGetAvailablePosReq->qmilocSecureGetAvailablePositionRequestData_len; count++)
  {
    LOC_MSG_MED("locPd_ConvToLocSecureAvailablePosition data:  0x%x ", *SSdata, 0, 0);
    SSdata++;
  }
#endif
  //Decode the data, Then  forward, after proper typecasting
  if (pz_SecureGetAvailablePosReq->secureLocDataMode == eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED_V02)
  {
    //Decrypt the payload
    v_RetVal = locPd_DecryptSecureAvailablePositionDataBlob(pz_SecureGetAvailablePosReq, pz_DecryptedAvailablePosReq);
    LOC_MSG_MED("locPd_DecryptSecureAvailablePositionDataBlob : %d", v_RetVal, 0, 0);
    if (TRUE == v_RetVal)
    {
      pz_TempSecureGetAvailablePosReq = pz_DecryptedAvailablePosReq;
    }
  }
  else
  {
    // No need to Decrypt, Data is un-encrypted
    pz_TempSecureGetAvailablePosReq = (qmiLocSecureGetAvailablePositionReqMsgT_v02 *)pz_SecureGetAvailablePosReq;
    v_RetVal = TRUE;
  }
  // Decode the payload and populate the pz_IoctlInfo structure.
  if (v_RetVal)
  {
    v_RetVal = locPd_ConvToLocGetAvailablePosition(pz_TempSecureGetAvailablePosReq, pz_GetAvailablePosReq);
  }
  if (v_RetVal)
  {
    LOC_MSG_MED("locPd_ConvToLocSecureAvailablePosition transactionId=%d",
                pz_GetAvailablePosReq->transactionId, 0, 0);
  }
  if (NULL != pz_DecryptedAvailablePosReq)
  {
    loc_free(pz_DecryptedAvailablePosReq);
  }
  return v_RetVal;
}

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
/* Function to process test position injection request. */
static int locPd_EnablePositionInjectionTestMode
(
   loc_client_handle_type                 l_ClientHandle,
   const qmiLocEnablePositionInjectionTestModeReqMsgT_v02 *pz_Req
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  uint32 e_testModeStatus = LOC_API_GENERAL_FAILURE;

  qmiLocEnablePositionInjectionTestModeIndMsgT_v02 *pz_enableInd = NULL;

  qmiLocEnablePositionInjectionTestModeEnumT_v02 testModeState = eQMI_LOC_TEST_MODE_UNKNOWN_V02;
  /* Function arguments sanity check */
  if (NULL == pz_Req)
  {
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if ((FALSE == pz_LocPdData->enable_pos_inj_test_mode) &&
      (FALSE == pz_Req->enableTestMode))
  {
    LOC_MSG_ERROR("locPd_EnablePositionInjectionTestMode: Test mode already disabled", 0, 0, 0);
    e_testModeStatus = LOC_API_GENERAL_FAILURE;
    testModeState = eQMI_LOC_TEST_MODE_ALREADY_DISABLED_V02;
    pz_LocPdData->cached_gnss_test_position_valid = FALSE;
  }
  else if ((FALSE == pz_LocPdData->enable_pos_inj_test_mode) &&
           (TRUE == pz_Req->enableTestMode))
  {
    pz_LocPdData->enable_pos_inj_test_mode = TRUE;
    e_testModeStatus = LOC_API_SUCCESS;
    testModeState = eQMI_LOC_TEST_MODE_ENABLED_V02;
    LOC_MSG_HIGH("locPd_EnablePositionInjectionTestMode: Test mode succesfully enabled", 0, 0, 0);
  }
  else if ((TRUE == pz_LocPdData->enable_pos_inj_test_mode) &&
           (FALSE == pz_Req->enableTestMode))
  {
    pz_LocPdData->enable_pos_inj_test_mode = FALSE;
    e_testModeStatus = LOC_API_SUCCESS;
    testModeState = eQMI_LOC_TEST_MODE_DISABLED_V02;
    pz_LocPdData->cached_gnss_test_position_valid = FALSE;
    LOC_MSG_HIGH("locPd_EnablePositionInjectionTestMode: Test mode succesfully disabled", 0, 0, 0);
  }
  else if ((TRUE == pz_LocPdData->enable_pos_inj_test_mode) &&
           (TRUE == pz_Req->enableTestMode))
  {
    LOC_MSG_ERROR("locPd_EnablePositionInjectionTestMode: Test mode already enabled", 0, 0, 0);
    e_testModeStatus = LOC_API_GENERAL_FAILURE;
    testModeState = eQMI_LOC_TEST_MODE_ALREADY_ENABLED_V02;
  }
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  pz_enableInd = (qmiLocEnablePositionInjectionTestModeIndMsgT_v02 *)loc_calloc(sizeof(*pz_enableInd));
  if (NULL == pz_enableInd)
  {
    LOC_MSG_ERROR("locPd_ExtEventCb: Failed to allocate memory for QMI_LOC Indication", 0, 0, 0);
  }
  else
  {
    pz_enableInd->testModeState_valid = TRUE;
    pz_enableInd->testModeState = testModeState;
    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    /* Send out the indication to the QMI client */
    locMW_SendQmiIndicationProxy(l_ClientHandle,
                                 QMI_LOC_ENABLE_POSITION_INJECTION_TEST_MODE_IND_V02,
                                 pz_enableInd,
                                 e_testModeStatus);
  }

  return LOC_API_SUCCESS;
}



/* Function to process test position injection request. */
static int locPd_InjectTestPos
(
   loc_client_handle_type                 l_ClientHandle,
   const qmiLocInjectTestPositionReqMsgT_v02 *pz_PosReq
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  qmiLocInjectTestPositionIndMsgT_v02 *pz_Ind = NULL;

  if (NULL == pz_PosReq)
  {
    return LOC_API_INVALID_PARAMETER;
  }
  /* Sanity check the lat and long */
  if ((pz_PosReq->latitude < -90) ||
      (pz_PosReq->latitude > 90) ||
      (pz_PosReq->longitude < -180) ||
      (pz_PosReq->longitude > 180))
  {
    return LOC_API_INVALID_PARAMETER;
  }
  /* Copy the injected position into the cache */
  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  memscpy(&pz_LocPdData->cached_gnss_test_position, sizeof(pz_LocPdData->cached_gnss_test_position),
          pz_PosReq, sizeof(*pz_PosReq));
  pz_LocPdData->cached_gnss_test_position_valid = TRUE;
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  pz_Ind = (qmiLocInjectTestPositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_Ind));
  if (NULL == pz_Ind)
  {
    LOC_MSG_ERROR("locPd_ExtEventCb: Failed to allocate memory for QMI_LOC Indication", 0, 0, 0);
  }
  else
  {
    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    /* Send out the indication to the QMI client */
    locMW_SendQmiIndicationProxy(l_ClientHandle,
                                 QMI_LOC_INJECT_TEST_POSITION_IND_V02,
                                 pz_Ind,
                                 LOC_API_SUCCESS);
  }
  return LOC_API_SUCCESS;
}
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC request from client that belongs to PD module
  (related to PDAPI Position Determination module)

@param[in] l_ClientHandle  : Loc API client handle
@param[in] q_QmiLocMsgId   : QMI_LOC Request Message ID
@param[in] p_QmiLocMsgData : QMI_LOC Request Message Data
@param[in] q_QmiLocMsgLen  : QMI_LOC Request Message Data Length
@param[in/out] pe_QmiErr   : QMI_LOC Response error value

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locPd_ProcessQmiRequest
(
   loc_client_handle_type l_ClientHandle,
   uint32                 q_QmiLocMsgId,
   const void *p_QmiLocMsgData,
   uint32                 q_QmiLocMsgLen,
   qmi_error_type_v01 *pe_QmiErr
   )
{
  uint32                  q_RetVal     = LOC_API_GENERAL_FAILURE;

  if (NULL == pe_QmiErr)
  {
    LOC_MSG_ERROR("locPd_ProcessQmiRequest: NULL pe_QmiErr", 0, 0, 0);
    return FALSE;
  }

  /* Function arguments sanity check */
  if (LOC_CLIENT_HANDLE_INVALID == l_ClientHandle)
  {
    LOC_MSG_ERROR("locPd_ProcessQmiRequest: invalid clientHandle", 0, 0, 0);
    *pe_QmiErr = QMI_ERR_INTERNAL_V01;
    return FALSE;
  }

  /* Check for master client and cmd restriction */
  if (locClnt_IsCommandAllowed(l_ClientHandle, q_QmiLocMsgId) == FALSE)
  {
    LOC_MSG_HIGH("locPd_ProcessQmiRequest: client %d msg %lu not allowed", l_ClientHandle, q_QmiLocMsgId, 0);
    *pe_QmiErr = QMI_ERR_NO_PERMISSION_V01;
    return FALSE;
  }

  switch (q_QmiLocMsgId)
  {
    case QMI_LOC_INJECT_POSITION_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locPd_InjectExtPos(l_ClientHandle, (qmiLocInjectPositionReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locPd_ProcessQmiRequest QMI_LOC_INJECT_POSITION_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_GET_BEST_AVAILABLE_POSITION_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locPd_GetBestAvailPos(l_ClientHandle, (qmiLocGetBestAvailablePositionReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locPd_ProcessQmiRequest QMI_LOC_GET_BEST_AVAILABLE_POSITION_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_GET_AVAILABLE_WWAN_POSITION_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locPd_GetAvailWwanPos(l_ClientHandle, (qmiLocGetAvailWwanPositionReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locPd_ProcessQmiRequest QMI_LOC_GET_AVAILABLE_WWAN_POSITION_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_SECURE_GET_AVAILABLE_POSITION_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locPd_GetAvailPos(l_ClientHandle, (qmiLocSecureGetAvailablePositionReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locPd_ProcessQmiRequest QMI_LOC_SECURE_GET_AVAILABLE_POSITION_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_INJECT_GSM_CELL_INFO_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locPd_InjectGsmCellInfo(l_ClientHandle, (qmiLocInjectGSMCellInfoReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locPd_ProcessQmiRequest QMI_LOC_INJECT_GSM_CELL_INFO_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_INJECT_NETWORK_INITIATED_MESSAGE_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locPd_InjectNiMsg(l_ClientHandle, (qmiLocInjectNetworkInitiatedMessageReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locPd_ProcessQmiRequest QMI_LOC_INJECT_NETWORK_INITIATED_MESSAGE_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_WWAN_OUT_OF_SERVICE_NOTIFICATION_REQ_V02:
      {
        q_RetVal = locPd_WwanOosNotification(l_ClientHandle);
        break;
      }

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
    case QMI_LOC_ENABLE_POSITION_INJECTION_TEST_MODE_REQ_V02:
      {
        q_RetVal = locPd_EnablePositionInjectionTestMode(l_ClientHandle,
                                                         (qmiLocEnablePositionInjectionTestModeReqMsgT_v02 *)p_QmiLocMsgData);
        break;
      }
    case QMI_LOC_INJECT_TEST_POSITION_REQ_V02:
      {
        q_RetVal = locPd_InjectTestPos(l_ClientHandle,
                                       (qmiLocInjectTestPositionReqMsgT_v02 *)p_QmiLocMsgData);
        break;
      }
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

    default:
      q_RetVal = LOC_API_UNSUPPORTED;
      break;
  }

  if (LOC_API_SUCCESS == q_RetVal)
  {
    return TRUE;
  }
  else
  {
    LOC_MSG_ERROR("locPd_ProcessQmiRequest: QMI_LOC 0x%04x returns error=%d", q_QmiLocMsgId, q_RetVal, 0);
    *pe_QmiErr = QMI_ERR_MALFORMED_MSG_V01;
    return FALSE;
  }
}

/* Utility function to convert from QMI_LOC Reliability to PDSM */
static pdsm_pd_reliability_e_type locPd_ConvertToPdsmReliability
(
   qmiLocReliabilityEnumT_v02 e_Reliability
   )
{
  switch (e_Reliability)
  {
    case eQMI_LOC_RELIABILITY_VERY_LOW_V02:
      {
        return PDSM_POSITION_RELIABILITY_VERY_LOW;
      }
    case eQMI_LOC_RELIABILITY_LOW_V02:
      {
        return PDSM_POSITION_RELIABILITY_LOW;
      }
    case eQMI_LOC_RELIABILITY_MEDIUM_V02:
      {
        return PDSM_POSITION_RELIABILITY_MEDIUM;
      }
    case eQMI_LOC_RELIABILITY_HIGH_V02:
      {
        return PDSM_POSITION_RELIABILITY_HIGH;
      }

    case eQMI_LOC_RELIABILITY_NOT_SET_V02:
    default:
      {
        return PDSM_POSITION_RELIABILITY_NOT_SET;
      }
  }
}

/* Utility function to convert from QMI_LOC Altitude Source to PDSM */
static pdsm_pd_altitude_source_e_type locPd_ConvertToPdsmAltSrc
(
   qmiLocAltSrcEnumT_v02 e_AltSrc
   )
{
  switch (e_AltSrc)
  {
    case eQMI_LOC_ALT_SRC_GPS_V02:
      {
        return PDSM_ALTITUDE_SOURCE_GPS;
      }
    case eQMI_LOC_ALT_SRC_CELL_ID_V02:
      {
        return PDSM_ALTITUDE_SOURCE_CID;
      }
    case eQMI_LOC_ALT_SRC_ENHANCED_CELL_ID_V02:
      {
        return PDSM_ALTITUDE_SOURCE_ECID;
      }
    case eQMI_LOC_ALT_SRC_WIFI_V02:
      {
        return PDSM_ALTITUDE_SOURCE_WIFI;
      }
    case eQMI_LOC_ALT_SRC_TERRESTRIAL_V02:
      {
        return PDSM_ALTITUDE_SOURCE_TERRESTRIAL;
      }
    case eQMI_LOC_ALT_SRC_TERRESTRIAL_HYBRID_V02:
      {
        return PDSM_ALTITUDE_SOURCE_TERRESTRIAL_HYBRID;
      }
    case eQMI_LOC_ALT_SRC_ALTITUDE_DATABASE_V02:
      {
        return PDSM_ALTITUDE_SOURCE_ALTITUDE_DATABASE;
      }
    case eQMI_LOC_ALT_SRC_BAROMETRIC_ALTIMETER_V02:
      {
        return PDSM_ALTITUDE_SOURCE_BAROMETRIC_ALTIMETER;
      }
    case eQMI_LOC_ALT_SRC_OTHER_V02:
      {
        return PDSM_ALTITUDE_SOURCE_OTHER;
      }

    case eQMI_LOC_ALT_SRC_UNKNOWN_V02:
    default:
      {
        return PDSM_ALTITUDE_SOURCE_UNKNOWN;
      }
  }
}

/*---------------------------------------------------------------------------
@brief
  Function to forward external DBH coarse position injection to allowed clients.

@param[in] pz_PosReq : Input param containing injected position
---------------------------------------------------------------------------*/
static void locPd_ForwardInjectedDbhPos
(
  const qmiLocInjectPositionReqMsgT_v02 *pz_PosReq
)
{
  qmiLocEventDbhPositionIndMsgT_v02 *pz_DbhPositionInd = NULL;
  int i = 0;
  
  if (NULL == pz_PosReq)
  {
    LOC_MSG_ERROR_0("locPd_ForwardInjectedDbhPos NULL pz_PosReq");
    return;
  }

  pz_DbhPositionInd = (qmiLocEventDbhPositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_DbhPositionInd));
  if (NULL == pz_DbhPositionInd)
  {
    LOC_MSG_ERROR_0("locPd_ForwardInjectedDbhPos NULL pz_DbhPositionInd");
    return;
  }

  if(pz_PosReq->latitude_valid)
  {
    pz_DbhPositionInd->latitude = pz_PosReq->latitude;
    pz_DbhPositionInd->latitude_valid = TRUE;
  }

  if(pz_PosReq->longitude_valid)
  {
    pz_DbhPositionInd->longitude = pz_PosReq->longitude;
    pz_DbhPositionInd->longitude_valid = TRUE;
  }

  if(pz_PosReq->horUncCircular_valid)
  {
    pz_DbhPositionInd->horUncCircular = pz_PosReq->horUncCircular;
    pz_DbhPositionInd->horUncCircular_valid = TRUE;
  }

  if(pz_PosReq->horConfidence_valid)
  {
    pz_DbhPositionInd->horConfidence = pz_PosReq->horConfidence;
    pz_DbhPositionInd->horConfidence_valid = TRUE;
  }

  if(pz_PosReq->horReliability_valid)
  {
    pz_DbhPositionInd->horReliability = pz_PosReq->horReliability;
    pz_DbhPositionInd->horReliability_valid = TRUE;
  }

  if(pz_PosReq->altitudeWrtEllipsoid_valid)
  {
    pz_DbhPositionInd->altitudeWrtEllipsoid = pz_PosReq->altitudeWrtEllipsoid;
    pz_DbhPositionInd->altitudeWrtEllipsoid_valid = TRUE;
  }

  if(pz_PosReq->altitudeWrtMeanSeaLevel_valid)
  {
    pz_DbhPositionInd->altitudeWrtMeanSeaLevel = pz_PosReq->altitudeWrtMeanSeaLevel;
    pz_DbhPositionInd->altitudeWrtMeanSeaLevel_valid = TRUE;
  }

  if(pz_PosReq->vertUnc_valid)
  {
    pz_DbhPositionInd->vertUnc = pz_PosReq->vertUnc;
    pz_DbhPositionInd->vertUnc_valid = TRUE;
  }

  if(pz_PosReq->vertConfidence_valid)
  {
    pz_DbhPositionInd->vertConfidence = pz_PosReq->vertConfidence;
    pz_DbhPositionInd->vertConfidence_valid = TRUE;
  }

  if(pz_PosReq->vertReliability_valid)
  {
    pz_DbhPositionInd->vertReliability = pz_PosReq->vertReliability;
    pz_DbhPositionInd->vertReliability_valid = TRUE;
  }

  if(pz_PosReq->altSourceInfo_valid)
  {
    pz_DbhPositionInd->altSourceInfo.coverage = pz_PosReq->altSourceInfo.coverage;
	pz_DbhPositionInd->altSourceInfo.linkage = pz_PosReq->altSourceInfo.linkage;
	pz_DbhPositionInd->altSourceInfo.source = pz_PosReq->altSourceInfo.source;
    pz_DbhPositionInd->altSourceInfo_valid = TRUE;
  }

  if(pz_PosReq->timestampUtc_valid)
  {
    pz_DbhPositionInd->timestampUtc = pz_PosReq->timestampUtc;
    pz_DbhPositionInd->timestampUtc_valid = TRUE;
  }

  if(pz_PosReq->timestampAge_valid)
  {
    pz_DbhPositionInd->timestampAge = pz_PosReq->timestampAge;
    pz_DbhPositionInd->timestampAge_valid = TRUE;
  }

  if(pz_PosReq->positionSrc_valid)
  {
    pz_DbhPositionInd->positionSrc = pz_PosReq->positionSrc;
    pz_DbhPositionInd->positionSrc_valid = TRUE;
  }

  if(pz_PosReq->rawHorUncCircular_valid)
  {
    pz_DbhPositionInd->rawHorUncCircular = pz_PosReq->rawHorUncCircular;
    pz_DbhPositionInd->rawHorUncCircular_valid = TRUE;
  }

  if(pz_PosReq->rawHorConfidence_valid)
  {
    pz_DbhPositionInd->rawHorConfidence = pz_PosReq->rawHorConfidence;
    pz_DbhPositionInd->rawHorConfidence_valid = TRUE;
  }

  if(pz_PosReq->onDemandCpi_valid)
  {
    pz_DbhPositionInd->onDemandCpi = pz_PosReq->onDemandCpi;
    pz_DbhPositionInd->onDemandCpi_valid = TRUE;
  }

  if(pz_PosReq->positionSrcProvider_valid)
  {
    pz_DbhPositionInd->positionSrcProvider = pz_PosReq->positionSrcProvider;
    pz_DbhPositionInd->positionSrcProvider_valid = TRUE;
  }

  if(pz_PosReq->gpsTime_valid)
  {
    pz_DbhPositionInd->gpsTime.gpsTimeOfWeekMs = pz_PosReq->gpsTime.gpsTimeOfWeekMs;
	pz_DbhPositionInd->gpsTime.gpsWeek = pz_PosReq->gpsTime.gpsWeek;
    pz_DbhPositionInd->gpsTime_valid = TRUE;
  }

  if(pz_PosReq->timeUnc_valid)
  {
    pz_DbhPositionInd->timeUnc = pz_PosReq->timeUnc;
    pz_DbhPositionInd->timeUnc_valid = TRUE;
  }

  if(pz_PosReq->velEnu_valid)
  {
    pz_DbhPositionInd->velEnu[0] = pz_PosReq->velEnu[0];
    pz_DbhPositionInd->velEnu[1] = pz_PosReq->velEnu[1];
    pz_DbhPositionInd->velEnu[2] = pz_PosReq->velEnu[2];
    pz_DbhPositionInd->velEnu_valid = TRUE;
  }

  if(pz_PosReq->velUncEnu_valid)
  {
    pz_DbhPositionInd->velUncEnu[0] = pz_PosReq->velUncEnu[0];
    pz_DbhPositionInd->velUncEnu[1] = pz_PosReq->velUncEnu[1];
    pz_DbhPositionInd->velUncEnu[2] = pz_PosReq->velUncEnu[2];
    pz_DbhPositionInd->velUncEnu_valid = TRUE;
  }

  if(pz_PosReq->expandedGnssSvUsedList_valid)
  {
    pz_DbhPositionInd->expandedGnssSvUsedList_len = pz_PosReq->expandedGnssSvUsedList_len;
    for (i = 0; i < pz_PosReq->expandedGnssSvUsedList_len; i++)
    {
      pz_DbhPositionInd->expandedGnssSvUsedList[i] = pz_PosReq->expandedGnssSvUsedList[i];
    }
    pz_DbhPositionInd->expandedGnssSvUsedList_valid = TRUE;
  }
  

  if(pz_PosReq->numSvInFix_valid)
  {
    pz_DbhPositionInd->numSvInFix = pz_PosReq->numSvInFix;
    pz_DbhPositionInd->numSvInFix_valid = TRUE;
  }


  LOC_MSG_MED_0("locPd_ForwardInjectedDbhPos");

  (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
    LOC_CMD_TYPE_NOTIFY_CLIENT,
    LOC_EVENT_MASK_DBH_POSITION,
    (void *)pz_DbhPositionInd);

  return;
}


/* Function to process external coarse position injection request. */
static int locPd_InjectExtPos
(
   loc_client_handle_type                 l_ClientHandle,
   const qmiLocInjectPositionReqMsgT_v02 *pz_PosReq
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  pdsm_pd_external_position_info_type z_ExtPos = { 0 };
  boolean v_IsPosDBH = FALSE;

  /* Function arguments sanity check */
  if (NULL == pz_PosReq)
  {
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if (pz_LocPdData->z_InjPos.v_InProgress)
  {
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    LOC_MSG_ERROR("locPd_InjectExtPos: Position Injection already in progress", 0, 0, 0);
    return LOC_API_ENGINE_BUSY;
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  /* Translate external coarse position from QMI_LOC to PDAPI format */

  /* Latitude, Longitude, Horizontal Uncertainty Circular & Confidence */
  if ((pz_PosReq->latitude_valid) &&
      (pz_PosReq->longitude_valid) &&
      ((pz_PosReq->horUncCircular_valid) && (pz_PosReq->horUncCircular > 0.0)))
  {
    z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_LAT_LONG_VALID;
    z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_CIRCULAR_UNC_VALID;
    z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_HORIZ_CONF_VALID;

    z_ExtPos.latitude    = pz_PosReq->latitude;
    z_ExtPos.longitude   = pz_PosReq->longitude;
    z_ExtPos.loc_unc_hor = pz_PosReq->horUncCircular;

    /* Parse confidence only when circular unc is set */
    if (pz_PosReq->horConfidence_valid)
    {
      z_ExtPos.confidence_hor = pz_PosReq->horConfidence;
    }
    else
    {
      /* If not set, use default value */
      z_ExtPos.confidence_hor = 50;
    }

    if ((pz_PosReq->rawHorUncCircular_valid) && (pz_PosReq->rawHorUncCircular > 0.0))
    {
      z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_RAW_CIRCULAR_UNC_VALID;
      z_ExtPos.loc_unc_hor_raw = pz_PosReq->rawHorUncCircular;

      /* Parse raw confidence only when raw circular unc is set */
      if (pz_PosReq->rawHorConfidence_valid)
      {
        z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_RAW_HORIZ_CONF_VALID;
        z_ExtPos.confidence_hor_raw = pz_PosReq->rawHorConfidence;
      }
      else
      {
        /* If not set, use default value */
        z_ExtPos.confidence_hor_raw = 50;
      }
    }
  }
  else if ((pz_PosReq->latitude_valid) ||
           (pz_PosReq->longitude_valid) ||
           ((pz_PosReq->horUncCircular_valid) && (pz_PosReq->horUncCircular > 0.0)) ||
           (pz_PosReq->horConfidence_valid))
  {
    LOC_MSG_ERROR("locPd_InjectExtPos: Invalid latitude & longitude parameters", 0, 0, 0);
    LOC_MSG_ERROR("Please provide valid latitude & longitude, with horizontal unc circular & confidence", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  /* Altitude (wrt Ellipsoid or Mean Sea Level), Vertical Uncertainty & Confidence */
  if (((pz_PosReq->altitudeWrtEllipsoid_valid) || (pz_PosReq->altitudeWrtMeanSeaLevel_valid)) &&
      (pz_PosReq->vertUnc_valid))
  {
    z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_ALT_VALID;

    if (pz_PosReq->altitudeWrtEllipsoid_valid)
    {
      z_ExtPos.altitude = pz_PosReq->altitudeWrtEllipsoid;
      z_ExtPos.alt_def  = PDSM_PD_EXTERN_COARSE_POS_ALT_HAE;
    }
    else //if (pz_PosReq->altitudeWrtMeanSeaLevel_valid)
    {
      z_ExtPos.altitude = pz_PosReq->altitudeWrtMeanSeaLevel;
      z_ExtPos.alt_def  = PDSM_PD_EXTERN_COARSE_POS_ALT_MSL;
    }

    if (pz_PosReq->vertUnc_valid)
    {
      z_ExtPos.loc_unc_vert = pz_PosReq->vertUnc;
    }

    if (pz_PosReq->vertConfidence_valid)
    {
      z_ExtPos.confidence_vert = pz_PosReq->vertConfidence;
    }
    else
    {
      /* If not set, use default value */
      z_ExtPos.confidence_vert = 50;
    }
  }
  else if ((pz_PosReq->altitudeWrtEllipsoid_valid) ||
           (pz_PosReq->altitudeWrtMeanSeaLevel_valid) ||
           (pz_PosReq->vertUnc_valid) ||
           (pz_PosReq->vertConfidence_valid))
  {
    LOC_MSG_ERROR("locPd_InjectExtPos: Invalid altitude parameters", 0, 0, 0);
    LOC_MSG_ERROR("Please provide valid altitude, with vertical unc & confidence", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  /* horReliability, vertReliability, altSourceInfo - Loc Api 2.0 Upgrades */
  if (pz_PosReq->horReliability_valid)
  {
    z_ExtPos.e_HoriRelIndicator = locPd_ConvertToPdsmReliability(pz_PosReq->horReliability);
  }
  if (pz_PosReq->vertReliability_valid)
  {
    z_ExtPos.e_VertRelIndicator = locPd_ConvertToPdsmReliability(pz_PosReq->vertReliability);
  }
  if (pz_PosReq->altSourceInfo_valid)
  {
    z_ExtPos.e_AltitudeSource = locPd_ConvertToPdsmAltSrc(pz_PosReq->altSourceInfo.source);

    /* Convert from QMI_LOC Altitude Source Horizontal Linkage to PDSM equivalent */
    switch (pz_PosReq->altSourceInfo.linkage)
    {
      case eQMI_LOC_ALT_SRC_LINKAGE_FULLY_INTERDEPENDENT_V02:
        {
          z_ExtPos.e_AltHorLinkage = PDSM_ALT_HOR_LINK_FULLY_INTERDEPENDENT;
          break;
        }
      case eQMI_LOC_ALT_SRC_LINKAGE_DEPENDS_ON_LAT_LONG_V02:
        {
          z_ExtPos.e_AltHorLinkage = PDSM_ALT_HOR_LINK_ALT_DEP_LAT_LONG;
          break;
        }
      case eQMI_LOC_ALT_SRC_LINKAGE_FULLY_INDEPENDENT_V02:
        {
          z_ExtPos.e_AltHorLinkage = PDSM_ALT_HOR_LINK_FULLY_INDEPENDENT;
          break;
        }

      case eQMI_LOC_ALT_SRC_LINKAGE_NOT_SPECIFIED_V02:
      default:
        {
          z_ExtPos.e_AltHorLinkage = PDSM_ALT_HOR_LINK_NOT_SPECIFIED;
        }
    }

    /* Convert from QMI_LOC Altitude Source Uncertainty Coverage to PDSM equivalent */
    switch (pz_PosReq->altSourceInfo.coverage)
    {
      case eQMI_LOC_ALT_UNCERTAINTY_POINT_V02:
        {
          z_ExtPos.e_AltUncCoverage = PDSM_ALT_COVERAGE_POINT_UNC;
          break;
        }
      case eQMI_LOC_ALT_UNCERTAINTY_FULL_V02:
        {
          z_ExtPos.e_AltUncCoverage = PDSM_ALT_COVERAGE_FULL_UNC;
          break;
        }

      case eQMI_LOC_ALT_UNCERTAINTY_NOT_SPECIFIED_V02:
      default:
        {
          z_ExtPos.e_AltUncCoverage = PDSM_ALT_COVERAGE_NOT_SPECIFIED;
        }
    }
  }

  /* Timestamps */
  if (pz_PosReq->timestampUtc_valid)
  {
    z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_GPS_TIME_VALID;
    z_ExtPos.time_info_type  = PDSM_PD_EXTERN_COARSE_POS_TIME_UTC;
    z_ExtPos.timestamp_sec   = (pz_PosReq->timestampUtc + 500) / 1000 - GPS_TO_UTC_DELTA_SECONDS;
  }
  else if (pz_PosReq->timestampAge_valid)
  {
    z_ExtPos.opt_field_mask |= PDSM_PD_EXTERN_COARSE_POS_GPS_TIME_VALID;
    z_ExtPos.time_info_type  = PDSM_PD_EXTERN_COARSE_POS_TIME_AGE;
    z_ExtPos.timestamp_sec   = pz_PosReq->timestampAge / 1000; /* convert from ms to sec */
  }

  /* Position Source */
  if (pz_PosReq->positionSrc_valid)
  {
    switch (pz_PosReq->positionSrc)
    {
      case eQMI_LOC_POSITION_SRC_GNSS_V02:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_GPS;
          break;
        }
      case eQMI_LOC_POSITION_SRC_CELLID_V02:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_CELLID;
          break;
        }
      case eQMI_LOC_POSITION_SRC_ENH_CELLID_V02:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_ENH_CELLID;
          break;
        }
      case eQMI_LOC_POSITION_SRC_WIFI_V02:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_WIFI;
          break;
        }
      case eQMI_LOC_POSITION_SRC_TERRESTRIAL_V02:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_TERRESTRIAL;
          break;
        }
      case eQMI_LOC_POSITION_SRC_GNSS_TERRESTRIAL_HYBRID_V02:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_GPS_TERRESTRIAL_HYBRID;
          break;
        }
      case eQMI_LOC_POSITION_SRC_OTHER_V02:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_OTHER;
          v_IsPosDBH = TRUE;
          break;
        }
      default:
        {
          z_ExtPos.source = PDSM_PD_EXTERN_COARSE_POS_SOURCE_UNKNOWN;
          break;
        }
    }
  }

  if (pz_PosReq->onDemandCpi_valid)
  {
    z_ExtPos.v_is_on_demand_position_inj = (boolean)(pz_PosReq->onDemandCpi);
  }

  if (pz_PosReq->positionSrcProvider_valid)
  {
    z_ExtPos.m.posSrcProviderValid = 0;
    switch (pz_PosReq->positionSrcProvider)
    {
      case eQMI_LOC_POSITION_SRC_PROVIDER_EXTERNAL_V02:
        z_ExtPos.src_provider = PDSM_PD_EXTERN_CPI_SRC_EXTERNAL;
        z_ExtPos.m.posSrcProviderValid = 1;
        break;

      case eQMI_LOC_POSITION_SRC_PROVIDER_INTERNAL_V02:
        z_ExtPos.src_provider = PDSM_PD_EXTERN_CPI_SRC_INTERNAL;
        z_ExtPos.m.posSrcProviderValid = 1;
        break;

      default:
        LOC_MSG_ERROR("locPd_InjectExtPos: Invalid positionSrcProvider = %d", pz_PosReq->positionSrcProvider, 0, 0);
    }
  }

  /* Set the flag before we call PDAPI. The command callback may come back before
   * loc_mw task resumes execution. In that case the flag check inside command cb
   * will fail. 
   */
  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  pz_LocPdData->z_InjPos.v_InProgress = TRUE;
  pz_LocPdData->z_InjPos.l_ClientHandle = l_ClientHandle; /* for issuing callback later on */
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if (!pdsm_pd_inject_external_position(locPd_CmdCb,
                                        NULL,       // user data pointer
                                        &z_ExtPos,  // external position
                                        loc_middleware_data.pdapi_client_id)) // Client id of user
  {
    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    pz_LocPdData->z_InjPos.v_InProgress = FALSE;
    pz_LocPdData->z_InjPos.l_ClientHandle = LOC_CLIENT_HANDLE_INVALID;
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    return LOC_API_GENERAL_FAILURE;
  }

  if((pz_PosReq->onDemandCpi_valid) && (pz_PosReq->onDemandCpi) && (v_IsPosDBH))
  {
    /* Forward the injected DBH position to Allowed clients (FR99502) */
    LOC_MSG_HIGH_1("Forwarding injected DBH position to allowed clients. Source %d ", pz_PosReq->positionSrc);
    locPd_ForwardInjectedDbhPos(pz_PosReq);
  }

  return LOC_API_SUCCESS;
}

/* Function to translate the client's fix criteria from LOC API to PD API.
   The translated PD API fix criteria will be used in the next position request. */
void locPd_ConvertFixCfg2Pdapi(loc_client_info_s_type *pz_ClientInfo)
{
  locClnt_FixCriteriaType *p_FixCriteria = NULL;
  pdsm_pd_option_s_type *p_PdsmOption  = &(loc_middleware_data.z_locPd_Data.pdsm_option);
  pdsm_pd_qos_type *p_PdsmQos     = &(loc_middleware_data.z_locPd_Data.pdsm_qos);
  uint32                   q_fix_interval = LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT;

  if (NULL == pz_ClientInfo)
  {
    LOC_MSG_ERROR("locPd_ConvertFixCfg2Pdapi, NULL pz_ClientInfo", 0, 0, 0);
    return;
  }
  p_FixCriteria = (locClnt_FixCriteriaType *)(&(pz_ClientInfo->z_FixCriteria));

  /* If it is a willing to share, use the common fix criteria */
  if (p_FixCriteria->v_SharePosition)
  {
    p_FixCriteria = &(loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule);
  }

  p_PdsmOption->session                     = PDSM_PD_SESS_TYPE_NEW;
  if (LOC_CLIENT_FIX_SINGLE == p_FixCriteria->e_RecurrenceType)
  {
    p_PdsmOption->fix_rate.num_fixes          = 1;
  }
  else
  {
    p_PdsmOption->fix_rate.num_fixes          = UINT32_MAX;
  }

  /* Figure out fix interval */
  if (LOC_CLIENT_NOTIFY_ON_DISTANCE == p_FixCriteria->e_NotifyType)
  {
    q_fix_interval = p_FixCriteria->q_MinDistSampleInterval;
  }
  else
  {
    q_fix_interval = p_FixCriteria->q_TbfMs;
  }
  p_PdsmOption->fix_rate.time_between_fixes_ms = q_fix_interval;

  if (p_PdsmOption->fix_rate.time_between_fixes_ms < PDSM_MINIMUM_TBF_MS)
  {
    p_PdsmOption->fix_rate.time_between_fixes_ms = PDSM_MINIMUM_TBF_MS;
  }

  /* Copy the location server info from client */
  p_PdsmOption->lsinfo = pz_ClientInfo->z_ServerInfo;
  p_PdsmOption->umts_slp_lsinfo = pz_ClientInfo->z_SLPServerInfo;

  /* Translate the QOS info */
  p_PdsmQos->accuracy_threshold  = p_FixCriteria->q_PreferredAccuracy;
  if (p_FixCriteria->q_PreferredResponseTime > LOC_CLIENT_FIX_RESPONSE_TIME_MS_DEFAULT)
  {
    p_PdsmQos->gps_session_timeout = (byte)(LOC_CLIENT_FIX_RESPONSE_TIME_MS_DEFAULT / 1000); //255s
  }
  else
  {
    p_PdsmQos->gps_session_timeout = (byte)(p_FixCriteria->q_PreferredResponseTime / 1000);
    if ((p_PdsmQos->gps_session_timeout == 0) && (p_FixCriteria->q_PreferredResponseTime != 0))
    {
      p_PdsmQos->gps_session_timeout = LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT / 1000; //1s
    }

    /* For CP MO, floor qos timeout at 30 seconds */
    if (MO_CP == loc_middleware_data.z_locPa_Data.e_PaMoMethod)
    {
      /* Default response time is defined in ms */
      if (p_PdsmQos->gps_session_timeout < (LOC_CLIENT_MO_RESPONSE_TIME / 1000))
      {
        p_PdsmQos->gps_session_timeout = (LOC_CLIENT_MO_RESPONSE_TIME / 1000);
      }
    }
  }

  p_PdsmOption->operation_mode = locMW_ConvertFixModeToPdapi(p_FixCriteria->e_PreferredOperationMode);

  /* Set QoS timeout to 0 if operation mode is WWAN */
  if (eQMI_LOC_OPER_MODE_WWAN_V02 == p_FixCriteria->e_PreferredOperationMode)
  {
    p_PdsmQos->gps_session_timeout = 0;
  }

  /* If mode is CELL ID (UMTS) or AFLT (MSA with QoS == 0), and if the engine is ON,
   * we change the mode to MSB ref loc, this is done so that the GPS engine can stay on
   * and in the meantime, we also get a ref loc from the network. Ref Location can be used
   * to give a good quality GPS fix with low power in this case.
   */
  if ((p_PdsmOption->operation_mode == PDSM_SESSION_OPERATION_CELL_ID) ||
      (p_PdsmOption->operation_mode == PDSM_SESSION_OPERATION_MSASSISTED &&
       p_PdsmQos->gps_session_timeout == 0))
  {
    if (loc_middleware_data.z_locPd_Data.engine_state == LOC_ENGINE_STATE_ON)
    {
      p_PdsmOption->operation_mode   = PDSM_SESSION_OPERATION_REF_POSITION;
      p_PdsmQos->gps_session_timeout = LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT / 1000; //1s
    }
  }

  LOC_MSG_MED("locPd_ConvertFixCfg2Pdapi, operation_mode=%d, tbf=%d, accuracy_threshold=%d",
              p_PdsmOption->operation_mode, p_PdsmOption->fix_rate.time_between_fixes_ms, p_PdsmQos->accuracy_threshold);
  LOC_MSG_MED("locPd_ConvertFixCfg2Pdapi, timeout=%d, 1x server_option=%d, 1x server_addr=0x%x",
              p_PdsmQos->gps_session_timeout, p_PdsmOption->lsinfo.server_option, p_PdsmOption->lsinfo.server_adrs.server_adrs.server_adrs_v4.server_adrs);

  /* Populate the application ID */
  memset(&p_PdsmOption->supl_application_id, 0, sizeof(p_PdsmOption->supl_application_id));

  if (p_FixCriteria->q_ValidMask & LOC_FIX_CRIT_VALID_APPLICATION_ID)
  {
    LOC_MSG_MED("locPd_ConvertFixCfg2Pdapi, application ID is valid version_valid = %d",
                p_FixCriteria->z_ApplicationId.applicationVersion_valid, 0, 0);

    /* Set the validity bit for mandatory information (name and provider) */
    p_PdsmOption->supl_application_id.u_validity |= PDAPI_SUPL_APPLICATION_ID_VALID;

    /* Copy the application name */
    GNSS_STRLCPY((char *)p_PdsmOption->supl_application_id.u_app_name,
                 p_FixCriteria->z_ApplicationId.applicationName,
                 sizeof(p_PdsmOption->supl_application_id.u_app_name));

    /* Copy the application provider */
    GNSS_STRLCPY((char *)p_PdsmOption->supl_application_id.u_app_provider,
                 p_FixCriteria->z_ApplicationId.applicationProvider,
                 sizeof(p_PdsmOption->supl_application_id.u_app_provider));

    if (p_FixCriteria->z_ApplicationId.applicationVersion_valid)
    {
      /* Set the version validity mask */
      p_PdsmOption->supl_application_id.u_validity |= PDAPI_SUPL_APPLICATION_ID_APP_VERSION_VALID;

      /* Copy the application provider */
      GNSS_STRLCPY((char *)p_PdsmOption->supl_application_id.u_app_version,
                   p_FixCriteria->z_ApplicationId.applicationVersion,
                   sizeof(p_PdsmOption->supl_application_id.u_app_version));
    }
  }

  /* set power mode */
  switch (p_FixCriteria->z_PowerMode.powerMode)
  {
    case  eQMI_LOC_POWER_MODE_IMPROVED_ACCURACY_V02:
      p_PdsmOption->power_mode.powerMode = PDSM_SESSION_POWER_MODE_IMPROVED_ACCURACY;
      break;
    case  eQMI_LOC_POWER_MODE_NORMAL_V02:
      p_PdsmOption->power_mode.powerMode = PDSM_SESSION_POWER_MODE_NORMAL;
      break;
    case  eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_POWER_V02:
      p_PdsmOption->power_mode.powerMode = PDSM_SESSION_POWER_MODE_BACKGROUND_DEFINED_POWER;
      break;
    case  eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_TIME_V02:
      p_PdsmOption->power_mode.powerMode = PDSM_SESSION_POWER_MODE_BACKGROUND_DEFINED_TIME;
      break;
    case  eQMI_LOC_POWER_MODE_BACKGROUND_KEEP_WARM_V02:
      p_PdsmOption->power_mode.powerMode = PDSM_SESSION_POWER_MODE_BACKGROUND_KEEP_WARM;
      break;
    default:
      p_PdsmOption->power_mode.powerMode = PDSM_SESSION_POWER_MODE_NORMAL;
      break;
  }
  p_PdsmOption->power_mode.tbm_msecs = p_FixCriteria->z_PowerMode.timeBetweenMeasurement;

  /*Special request will be acepted as reqular requests during 911. Do not set the SpecialRequestType 
    if LocMW is in 911*/  
  p_PdsmOption->specialReqType.v_specialReqValid = FALSE;
  p_PdsmOption->specialReqType.e_SpecialReq = PDSM_SESSION_DONOTUSE_SPECIAL_REQ_MAX;

  if ( ( FALSE == loc_middleware_data.v_E911Session ) &&
       ( p_FixCriteria->q_ValidMask & LOC_FIX_SPECIAL_REQUEST ) &&
       ( p_FixCriteria->e_SpecialRequestType == eQMI_LOC_SPECIAL_REQUEST_SHORT_CODE_V02 ) )
  {
      p_PdsmOption->specialReqType.v_specialReqValid = TRUE;
      p_PdsmOption->specialReqType.e_SpecialReq = PDSM_SESSION_SPECIAL_REQ_SHORT_CODES;
}

  LOC_MSG_MED("locPd_ConvertFixCfg2Pdapi, powerMode=%d, tbm_msecs=%d, Special Req Valid=%d",
               p_PdsmOption->power_mode.powerMode, 
               p_PdsmOption->power_mode.tbm_msecs, 
               p_PdsmOption->specialReqType.v_specialReqValid);
}


loc_supl_req_e_type loc_pd_continue_processing_supl_request
(
   const pdsm_lcs_supl_notify_verify_req_s_type *pz_PdapiSuplReq
   )
{
  loc_supl_req_e_type result = LOC_SUPL_REQ_DENY;
  int trace = 0;
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  loc_supl_mt_state_e_type supl_mt_fix_state_copy = LOC_SUPL_MT_IDLE;
  boolean                    v_suplEmergency = FALSE;
  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  supl_mt_fix_state_copy = pz_LocPdData->supl_mt_fix_state;
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);


  if (pz_PdapiSuplReq->flags & PDSM_LCS_SUPLINIT_VER2_EXT_EMERGENCY_PRESENT)
  {
    v_suplEmergency = TRUE;
  }

  LOC_MSG_HIGH("loc_pd_continue_processing_supl_request,first MT = %d, 2nd MT ES = %d, PO = %d",
               supl_mt_fix_state_copy,
               v_suplEmergency,
               pz_PdapiSuplReq->notification_priv_type);

  if (pz_PdapiSuplReq->pos_method == POSMETHOD_NO_POSITION)
  {
    trace = 5;
    // NO_POS is allowed no matter what session is going on
    LOC_MSG_HIGH("loc_pd_continue_processing_supl_request, alway accept NO_POSITION", 0, 0, 0);
    result = LOC_SUPL_REQ_ACCEPT;
  }
  else if ((LOC_SUPL_MT_ES_ABOUT_TO_START == supl_mt_fix_state_copy)
           || (LOC_SUPL_MT_ES_IN_PROGRESS == supl_mt_fix_state_copy))
  {
    // There is a SUPL MT ES already running .
    trace = 6;
    // The second incoming session
    if (TRUE == v_suplEmergency)
    {
      trace = 7;
      // ES is allowed
      LOC_MSG_HIGH("loc_pd_continue_processing_supl_request, accept ES while ES is ongoing", 0, 0, 0);
      result = LOC_SUPL_REQ_ACCEPT;
    }
    else
    {
      trace = 8;
      // Non-ES is not allowed, and dropped
      LOC_MSG_HIGH("loc_pd_continue_processing_supl_request, drop non-ES while ES is ongoing", 0, 0, 0);
      result = LOC_SUPL_REQ_DROP;
    }
  }
  else if ((LOC_SUPL_MT_PO_ABOUT_TO_START == supl_mt_fix_state_copy)
           || (LOC_SUPL_MT_PO_IN_PROGRESS == supl_mt_fix_state_copy))
  {
    // There is a SUPL MT PO already running .
    trace = 1;

    // The second incoming session
    if (TRUE == v_suplEmergency ||
        PDSM_LCS_PRIVACY_NEEDED == pz_PdapiSuplReq->notification_priv_type)
    {
      trace = 2;
      // ES is allowed, PO is allowed
      result = LOC_SUPL_REQ_ACCEPT;
      LOC_MSG_HIGH("loc_pd_continue_processing_supl_request, accept ES/PO while PO is ongoing", 0, 0, 0);
    }
    else
    {
      trace = 3;
      // Regular non-PO is not allowed
      result = LOC_SUPL_REQ_DENY;
      LOC_MSG_HIGH("loc_pd_continue_processing_supl_request, deny Regular(non-PO) while PO is ongoing", 0, 0, 0);
    }
  }
  else
  {
    // There is a SUPL MT non-PO already running . The second incoming session is always allowed
    trace = 4;

    result = LOC_SUPL_REQ_ACCEPT;
    LOC_MSG_MED("loc_pd_continue_processing_supl_request, accept all type while non-PO is ongoing", 0, 0, 0);
  }
  LOC_MSG_HIGH("loc_pd_process_supl_request, trace = %d,result=%d", trace, result, 0);
  return result;
}

void loc_pd_handle_service_interaction(pdsm_lcs_notification_resp_s_type *resp_info_ptr)
{
  int trace = 0;
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  //end_session_type
  pdsm_pd_end_session_opt_s_type end_session_opt = { FALSE };
  boolean b_endsession = FALSE;

  loc_supl_mt_state_e_type supl_mt_fix_state_copy = LOC_SUPL_MT_IDLE;
  boolean mo_fix_in_progress_copy = FALSE;

  // Read the state of SUPL ES if it is going on .
  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  supl_mt_fix_state_copy = pz_LocPdData->supl_mt_fix_state;
  mo_fix_in_progress_copy = pz_LocPdData->mo_fix_in_progress;
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if (resp_info_ptr->lcs_info_cmd_type != PDSM_LCS_CMD_TYPE_SUPL_REQ)
  {
    // this is not a SUPL request. do nothing here
    trace = 1;
  }
  else if (resp_info_ptr->lcs_info.supl_req.pos_method == POSMETHOD_NO_POSITION)
  {
    trace = 8;
    // NO POSITION request goes through no matter what session we have now
  }
  else if (resp_info_ptr->lcs_info.supl_req.flags & PDSM_LCS_SUPLINIT_VER2_EXT_EMERGENCY_PRESENT)
  {
    // new incoming is SUPL MT ES - PO does not need to check accept or deny
    if ((resp_info_ptr->lcs_info.supl_req.notification_priv_type == PDSM_LCS_PRIVACY_OVERRIDE)
        || (resp_info_ptr->resp == PDSM_LCS_NOTIFY_VERIFY_ACCEPT)
        || (resp_info_ptr->resp == PDSM_LCS_NOTIFY_VERIFY_NORESP))
    {
      // all -> ES
      // End any ongoing session to honor incoming SUPL MT(ES) if PO or granted by user
      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      pz_LocPdData->supl_mt_fix_state = LOC_SUPL_MT_ES_ABOUT_TO_START;
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      LOC_MSG_HIGH("loc_pd_handle_service_interaction, killing running session to serve SUPL MT ES", 0, 0, 0);
      b_endsession = TRUE;
      trace = 21;
    }
    else
    {
      // User has denied the NI session. So do not kill any sessions. Just continue with the normal flow .
      LOC_MSG_HIGH("loc_pd_handle_service_interaction, not PO, user denied NI request. ", 0, 0, 0);
      trace = 22;
    }
  }
  else if (resp_info_ptr->lcs_info.supl_req.notification_priv_type == PDSM_LCS_PRIVACY_OVERRIDE)
  {
    // new incoming is SUPL MT PO
    if ((LOC_SUPL_MT_ES_ABOUT_TO_START == supl_mt_fix_state_copy)
        || (LOC_SUPL_MT_ES_IN_PROGRESS == supl_mt_fix_state_copy))
    {
      // ES -> PO
      // ES is already running / about to begin. Rejects incoming session locally.
      trace = 41;
      LOC_MSG_HIGH("loc_pd_handle_service_interaction, denying SUPL MT PO , for a SUPL MT ES is in progress", 0, 0, 0);
      resp_info_ptr->resp = PDSM_LCS_NOTIFY_VERIFY_DENY;
    }
    else
    {
      // all others -> PO
      trace = 61;
      // Ends ongoing session to honor incoming session and serve this SUPL MT PO request
      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      pz_LocPdData->supl_mt_fix_state = LOC_SUPL_MT_PO_ABOUT_TO_START;
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      LOC_MSG_HIGH("loc_pd_handle_service_interaction,  killing any running session to serve SUPL MT PO", 0, 0, 0);
      b_endsession = TRUE;
    }
  }
  else
  {
    // new incoming is SUPL MT reqular session
    if ((LOC_SUPL_MT_ES_ABOUT_TO_START == supl_mt_fix_state_copy)
        || (LOC_SUPL_MT_ES_IN_PROGRESS == supl_mt_fix_state_copy)
        || (LOC_SUPL_MT_PO_ABOUT_TO_START == supl_mt_fix_state_copy)
        || (LOC_SUPL_MT_PO_IN_PROGRESS == supl_mt_fix_state_copy))
    {
      // ES/PO -> regular
      trace = 42;
      // ES/PO is already running / about to begin. Rejects incoming session locally.
      LOC_MSG_HIGH("loc_pd_handle_service_interaction, denying SUPL MT regular , for a SUPL MT ES/PO is in progress", 0, 0, 0);
      resp_info_ptr->resp = PDSM_LCS_NOTIFY_VERIFY_DENY;
    }
    else if ((resp_info_ptr->resp == PDSM_LCS_NOTIFY_VERIFY_ACCEPT) ||
             (resp_info_ptr->resp == PDSM_LCS_NOTIFY_VERIFY_NORESP))
    {
      // all others -> regular
      // User has accepted / no response for the NI session . So we allow the SUPL session.
      // We end the ongoing MO sessions to honor the SUPL(MT) regular session.
      // Note that we do not kill an ongoing CP NI-LR ( ES or non-ES mode )
      LOC_MSG_HIGH("loc_pd_handle_service_interaction, user accepted/no-resp for NI request. ", 0, 0, 0);
      // the response is not for SUPL MT PO (Privacy Override)
      if (TRUE == mo_fix_in_progress_copy)
      {
        trace = 91;
        // even for normal SUPL request, we kill all MO requests passing through LocAPI
        LOC_MSG_MED("loc_pd_handle_service_interaction,  killing any MO session to serve SUPL", 0, 0, 0);
        b_endsession = TRUE;
      }
      else
      {
        trace = 92;
        // don't touch anything, let user's intention fall through
      }
    }
    else
    {
      // User has denied the NI session . So do not kill any sessions . Just continue with the normal flow .
      LOC_MSG_HIGH("loc_pd_handle_service_interaction, user denied NI request. ", 0, 0, 0);
      trace = 93;
    }
  }

  if (TRUE == b_endsession)
  {
    LOC_MSG_MED("loc_pd_handle_service_interaction, killing ongoing session", 0, 0, 0);
    end_session_opt.receiver_off = TRUE;
    end_session_opt.v_reason_service_interaction = TRUE;
    locPd_endSession(end_session_opt);
    pz_LocPdData->mo_fix_in_progress = FALSE;
    os_TimerStop(pz_LocPdData->fix_process_timer);
  }
  LOC_MSG_HIGH("loc_pd_handle_service_interaction,  trace %d", trace, 0, 0);
}

/*============================================================================
 *                    U T I L I T Y    F U N C T I O N S FOR SECURE AVAILABLE POSITION
 * =========================================================================*/

static const int16 bsti_endian = 1;  // Byte swap test integer
#define is_bigendian() ( (*(char*)&bsti_endian) == 0 )


/**
In-place swapping of bytes to match endianness of hardware
@param[in/out] *object : memory to swap in-place
@param[in]     _size   : length in bytes
*/
void swapbytes(void *_object, size_t _size)
{
  uint8 * start,*end;
  if (NULL == _object)
  {
    return;
  }
  if (!is_bigendian())
  {
    for (start = (uint8 *)_object, end = start + _size - 1; start < end; ++start, --end)
    {
      unsigned char swap = *start;
      *start = *end;
      *end = swap;
    }
  }
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to Encode  LOC API position report parameters in PTLV data format to populate the data blob of Get Secure Location API

@param [in]   param               Parameter type getting Encoded
@param [in]  dataType            Data Type used for Encoding
@param [in]  length                 length of Data type
@param [in]  value                  Data Value to be encoded
@param [in]  value_size           data Size to be encoded
@param [OUT]   pu_dstEncodedgetAvailablePosRsp         Destination buffer to hold Encoded Data

@retval  size_t  Size of data getting encoded and populated in the Destinantion buffer

*/ /* ------------------------------------------------------------------------*/

size_t populateGetAvailableLocationPosRpt(unsigned char *pu_dstEncodedgetAvailablePosRsp, qmiLocSecureGetAvailablePositionIndParamIDEnumT_v02 param,
                                          qmiLocSecureMessageDataTypeEnumT_v02 dataType, uint8 length, void *value,
                                          size_t value_size)
{

  int k = 0;
  unsigned char *start = pu_dstEncodedgetAvailablePosRsp;
  unsigned char *valstart = NULL;
  uint32 uint32_PlaceHolder;
  uint8 uint8_PlaceHolder;
  uint32 array_count = 0;
  size_t valueSize;

  /* Ensure that NULL pointers are not passed in */
  if (NULL == pu_dstEncodedgetAvailablePosRsp ||
      NULL == value)
  {
    return 0;
  }

  /***Encode in PTLV Format****/

  /* Encode PARAM ID  -- 4 bytes  */
  uint32_PlaceHolder = Swap4Bytes(param);
  memscpy(pu_dstEncodedgetAvailablePosRsp, sizeof(qmiLocSecureGetAvailablePositionIndParamIDEnumT_v02),
          &uint32_PlaceHolder, sizeof(qmiLocSecureGetAvailablePositionIndParamIDEnumT_v02));
  pu_dstEncodedgetAvailablePosRsp += sizeof(qmiLocSecureGetAvailablePositionIndParamIDEnumT_v02);

  /* Encode TYPE  -- 4 bytes  */
  uint32_PlaceHolder = Swap4Bytes(dataType);
  memscpy(pu_dstEncodedgetAvailablePosRsp, sizeof(qmiLocSecureMessageDataTypeEnumT_v02),
          &uint32_PlaceHolder, sizeof(qmiLocSecureMessageDataTypeEnumT_v02));
  pu_dstEncodedgetAvailablePosRsp += sizeof(qmiLocSecureMessageDataTypeEnumT_v02);

  /* Encode Length  -- 1 bytes */
  uint8_PlaceHolder = (length);
  memscpy(pu_dstEncodedgetAvailablePosRsp, sizeof(length),
          &uint8_PlaceHolder, sizeof(length));
  pu_dstEncodedgetAvailablePosRsp += sizeof(length);

  /*Encode the data part */
  valueSize = sizeofDataType(dataType);
  if ((valueSize * length) != value_size)
  {
    LOC_MSG_ERROR("populateGetAvailableLocationPosRpt:  Invalid param value length for ID %d", param, 0, 0);
    //reset the dst pointer to start
    pu_dstEncodedgetAvailablePosRsp  = start;
    return 0;
  }

  valstart = (unsigned char *)value;
  while (array_count < length)
  {
    swapbytes(valstart, valueSize);
    valstart = valstart + valueSize;
    array_count++;
  }
  memscpy(pu_dstEncodedgetAvailablePosRsp, value_size, value, (value_size));
  pu_dstEncodedgetAvailablePosRsp += value_size;
  return  (pu_dstEncodedgetAvailablePosRsp - start);

}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to Encode  IOCTL reports from Loc-Api to Get Available Secure Position data blob

@param [in]    pz_GetAvailablePosInd         Input QMI formatted data with result
@param [out]   pz_SecureGetAvailablePosInd   Encoded Payload of the event
@param [in]    e_IndStatus                   Indication status

@retval  TRUE: if conversion succeeded
       FALSE: if ioctl wasn't found 

*/ /* ------------------------------------------------------------------------*/
static boolean locPd_EncodeGetAvailableGnssPos(
   const qmiLocGetAvailablePositionIndParamMsgT_v02 *pz_GetAvailablePosInd,
   qmiLocSecureGetAvailablePositionIndMsgT_v02 *pz_SecureGetAvailablePosInd,
   qmiLocSessionStatusEnumT_v02                      e_IndStatus
   )
{

  secerrno_enum_type                           e_SecStatus = E_SUCCESS;
  boolean                                      e_IndStatusAvailPos = TRUE;
  uint8 *pu_OutputEncodedMsg = NULL;
  uint32                                       q_OutputEncodedMsgLen = 0;
  uint8 *pu_dstEncodedgetAvailablePosRsp = NULL;
  uint8 *pu_dstEncodedgetAvailablePosRspStart = NULL;

  /* static function, skip NULL pointer check of input parameters */
  pz_SecureGetAvailablePosInd->sessionStatus = e_IndStatus;

  do
  {

    /* Encode the Position in PTLV format
       data is in pz_GetAvailablePosInd
       Output Encoded data needs to be in pz_SecureGetAvailablePosInd */
    size_t size_perMsg = 0;
    uint32 qmilocSecureGetAvailablePositionInd_len = 0;
    pu_dstEncodedgetAvailablePosRsp =
       (uint8 *)loc_calloc(QMI_LOC_SECURE_GET_AVAILABLE_POSITION_IND_ENCRYPTED_MAX_V02);
    if (pu_dstEncodedgetAvailablePosRsp == NULL)
    {
      LOC_MSG_ERROR("locPd_EncodeGetAvailableGnssPos Memory Allocation 2 failed", 0, 0, 0);
      e_IndStatusAvailPos = FALSE;
      break;
    }
    pu_dstEncodedgetAvailablePosRspStart = pu_dstEncodedgetAvailablePosRsp;
    memset(pu_dstEncodedgetAvailablePosRsp, 0x0, sizeof(*pu_dstEncodedgetAvailablePosRsp));

    if (pz_GetAvailablePosInd->transactionId_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_TRANSACTION_ID_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->transactionId, sizeof(pz_GetAvailablePosInd->transactionId));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->nonce_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_NONCE_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_64_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->nonce, sizeof(pz_GetAvailablePosInd->nonce));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->latitude_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_LATITUDE_V02, eQMI_LOC_SECURE_DATA_TYPE_DOUBLE_64_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->latitude, sizeof(pz_GetAvailablePosInd->latitude));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->longitude_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_LONGITUDE_V02, eQMI_LOC_SECURE_DATA_TYPE_DOUBLE_64_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->longitude, sizeof(pz_GetAvailablePosInd->longitude));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horUncCircular_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HORIZ_UNC_CIRCULAR_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horUncCircular, sizeof(pz_GetAvailablePosInd->horUncCircular));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->altitudeWrtEllipsoid_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_ALTITUDE_WRT_ELP_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->altitudeWrtEllipsoid, sizeof(pz_GetAvailablePosInd->altitudeWrtEllipsoid));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->vertUnc_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_VERTICAL_UNC_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->vertUnc, sizeof(pz_GetAvailablePosInd->vertUnc));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->timestampUtc_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_TIME_STAMP_UTC_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_64_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->timestampUtc, sizeof(pz_GetAvailablePosInd->timestampUtc));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->timeUnc_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_TIME_UNC_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->timeUnc, sizeof(pz_GetAvailablePosInd->timeUnc));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horUncEllipseSemiMinor_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HORIZ_UNC_ELP_SEMIMINOR_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horUncEllipseSemiMinor, sizeof(pz_GetAvailablePosInd->horUncEllipseSemiMinor));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horUncEllipseSemiMajor_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HORIZ_UNC_ELP_SEMIMAJOR_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horUncEllipseSemiMajor, sizeof(pz_GetAvailablePosInd->horUncEllipseSemiMajor));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horUncEllipseOrientAzimuth_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HORIZ_UNC_ELP_ORIENT_AZIMUTH_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horUncEllipseOrientAzimuth, sizeof(pz_GetAvailablePosInd->horUncEllipseOrientAzimuth));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horCircularConfidence_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HORIZ_CONFIDENCE_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_8_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horCircularConfidence, sizeof(pz_GetAvailablePosInd->horCircularConfidence));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horEllipticalConfidence_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HORIZ_ELLIP_CONFIDENCE_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_8_V02,
                                                       1, (void *)&pz_GetAvailablePosInd->horEllipticalConfidence, sizeof(pz_GetAvailablePosInd->horEllipticalConfidence));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horReliability_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HORIZ_RELIABILITY_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horReliability, sizeof(pz_GetAvailablePosInd->horReliability));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horSpeed_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_SPEED_HORIZ_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horSpeed, sizeof(pz_GetAvailablePosInd->horSpeed));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->horSpeedUnc_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_SPEED_HORIZ_UNC_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->horSpeedUnc, sizeof(pz_GetAvailablePosInd->horSpeedUnc));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->altitudeWrtMeanSeaLevel_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_ALTITUDE_WRT_MSA_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->altitudeWrtMeanSeaLevel, sizeof(pz_GetAvailablePosInd->altitudeWrtMeanSeaLevel));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->vertConfidence_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_VERTICAL_CONF_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_8_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->vertConfidence, sizeof(pz_GetAvailablePosInd->vertConfidence));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->vertReliability_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_VERT_RELIABILITY_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->vertReliability, sizeof(pz_GetAvailablePosInd->vertReliability));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->vertSpeed_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_SPEED_VERTICAL_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->vertSpeed, sizeof(pz_GetAvailablePosInd->vertSpeed));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->vertSpeedUnc_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_SPEED_VERTICAL_UNC_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->vertSpeedUnc, sizeof(pz_GetAvailablePosInd->vertSpeedUnc));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->heading_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HEADING_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->heading, sizeof(pz_GetAvailablePosInd->heading));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->headingUnc_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HEADING_UNC_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->headingUnc, sizeof(pz_GetAvailablePosInd->headingUnc));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->magneticDeviation_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_MAGNETIC_DEVIATION_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->magneticDeviation, sizeof(pz_GetAvailablePosInd->magneticDeviation));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->technologyMask_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_TECH_MASK_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->technologyMask, sizeof(pz_GetAvailablePosInd->technologyMask));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->DOP_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_PDOP_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->DOP.PDOP, sizeof(pz_GetAvailablePosInd->DOP.PDOP));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_HDOP_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->DOP.HDOP, sizeof(pz_GetAvailablePosInd->DOP.HDOP));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_VDOP_V02, eQMI_LOC_SECURE_DATA_TYPE_FLOAT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->DOP.VDOP, sizeof(pz_GetAvailablePosInd->DOP.VDOP));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->gpsTime_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_GPS_WEEK_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_16_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->gpsTime.gpsWeek, sizeof(pz_GetAvailablePosInd->gpsTime.gpsWeek));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_GPS_TIMEOFWEEKMS_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->gpsTime.gpsTimeOfWeekMs, sizeof(pz_GetAvailablePosInd->gpsTime.gpsTimeOfWeekMs));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->timeSrc_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_TIME_SRC_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->timeSrc, sizeof(pz_GetAvailablePosInd->timeSrc));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->sensorDataUsage_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_SENSOR_USAGE_MASK_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->sensorDataUsage.usageMask, sizeof(pz_GetAvailablePosInd->sensorDataUsage.usageMask));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_AIDING_IND_MASK_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_32_V02, 1,
                                                       (void *)&pz_GetAvailablePosInd->sensorDataUsage.aidingIndicatorMask, sizeof(pz_GetAvailablePosInd->sensorDataUsage.aidingIndicatorMask));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    if (pz_GetAvailablePosInd->gnssSvUsedList_valid)
    {
      size_perMsg = populateGetAvailableLocationPosRpt(pu_dstEncodedgetAvailablePosRsp,
                                                       eQMI_LOC_SECURE_GET_AVAILABLE_POS_REP_PARAM_SV_USED_V02, eQMI_LOC_SECURE_DATA_TYPE_UNSIGNED_INT_16_V02,
                                                       pz_GetAvailablePosInd->gnssSvUsedList_len, (void *)&pz_GetAvailablePosInd->gnssSvUsedList[0],
                                                       (sizeof(pz_GetAvailablePosInd->gnssSvUsedList[0]) * pz_GetAvailablePosInd->gnssSvUsedList_len));
      qmilocSecureGetAvailablePositionInd_len += size_perMsg;
      pu_dstEncodedgetAvailablePosRsp += size_perMsg;
    }
    // Check if data Blob needs to be Encrypted
    if (pz_GetAvailablePosInd->e_PosDataMode == eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED_V02)
    {
      LOC_MSG_MED("locPd_EncodeGetAvailableGnssPos Calling Secure API for Encryption", 0, 0, 0);
      pu_OutputEncodedMsg = (uint8 *)loc_calloc(qmilocSecureGetAvailablePositionInd_len + SECAPI_ENCRYPTION_MSG_HEADER);
      if (pu_OutputEncodedMsg == NULL)
      {
        LOC_MSG_ERROR("pu_OutputEncodedMsg Memory Allocation failed", 0, 0, 0);
        e_IndStatusAvailPos = FALSE;
        break;
      }
      /* Set outPut Message length to be 100 Bytes greater. Set value will have the proper Length */
      q_OutputEncodedMsgLen = qmilocSecureGetAvailablePositionInd_len + SECAPI_ENCRYPTION_MSG_HEADER;
      /* call Encryption Functions */
      e_SecStatus = secapi_secure_message(SC_SSID_TZ,
                                          SC_CID_TLOC, pu_dstEncodedgetAvailablePosRspStart,
                                          qmilocSecureGetAvailablePositionInd_len,
                                          pu_OutputEncodedMsg,
                                          &q_OutputEncodedMsgLen);
      pz_SecureGetAvailablePosInd->secureLocDataMode = eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED_V02;

      if (e_SecStatus == E_SUCCESS)
      {
        pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_valid = TRUE;
        pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_len = q_OutputEncodedMsgLen;
        LOC_MSG_MED("qmilocSecureGetAvailablePositionInd_len before Encryption %d  After %d", qmilocSecureGetAvailablePositionInd_len, q_OutputEncodedMsgLen, 0);
        memscpy(pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd,
                pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_len,
                pu_OutputEncodedMsg,
                pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_len);
      }
      else
      {
        pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_valid = FALSE;
        LOC_MSG_MED("secapi_secure_message Encryption failed e_SecStatus %d", e_SecStatus, 0, 0);
        e_IndStatusAvailPos = FALSE;
      }
    }
    else
    {
      pz_SecureGetAvailablePosInd->secureLocDataMode = eQMI_LOC_SECURE_LOC_DATA_UNENCRYPTED_V02;
      pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_valid = TRUE;
      pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_len = qmilocSecureGetAvailablePositionInd_len;
      LOC_MSG_MED("No Encryption qmilocSecureGetAvailablePositionInd_len %d", qmilocSecureGetAvailablePositionInd_len, 0, 0);
      memscpy(pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd,
              pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_len,
              pu_dstEncodedgetAvailablePosRspStart,
              pz_SecureGetAvailablePosInd->qmilocSecureGetAvailablePositionInd_len);

    }

  }while (0);
  if (NULL != pu_dstEncodedgetAvailablePosRspStart)
  {
    loc_free(pu_dstEncodedgetAvailablePosRspStart);
  }
  if (NULL != pu_OutputEncodedMsg)
  {
    loc_free(pu_OutputEncodedMsg);
  }
  if (FALSE == e_IndStatusAvailPos)
  {
    pz_SecureGetAvailablePosInd->sessionStatus = eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02;
  }
  return e_IndStatusAvailPos;
}

/* Event callback called from PDAPI for a PD command.
   The implementation will rely on the UserData pointer to identify the client handle etc,
   and will lock the critical section. */
static void locPd_EventCb
(
   void *p_UserData,
   pdsm_pd_event_type         q_PdEvent,
   const pdsm_pd_info_s_type *pd_info_ptr
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  time_type             time_ms = { 0 };
  uint64                current_time_ms;
  boolean               result = TRUE;
  qmiLocEventEngineStateIndMsgT_v02 *pz_EngineStateIndMsg = NULL;

  LOC_MSG_HIGH("locPd_EventCb: event=0x%x, client_id=%d", q_PdEvent, pd_info_ptr->client_id, 0);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  /* Location engine is turned on, ignore client id */
  if ((q_PdEvent & PDSM_PD_EVENT_GPS_BEGIN) ||
      (q_PdEvent & PDSM_PD_EVENT_GPS_DONE))
  {
    /* Queue the cmd to start another round of fix processing */
    pz_EngineStateIndMsg = (qmiLocEventEngineStateIndMsgT_v02 *)loc_calloc(sizeof(*pz_EngineStateIndMsg));

    if (NULL != pz_EngineStateIndMsg)
    {
      if (q_PdEvent & PDSM_PD_EVENT_GPS_BEGIN)
      {
        LOC_MSG_MED("locPd_EventCb, PDSM_PD_EVENT_GPS_BEGIN", 0, 0, 0);
        pz_LocPdData->engine_state = LOC_ENGINE_STATE_ON;
        pz_EngineStateIndMsg->engineState = eQMI_LOC_ENGINE_STATE_ON_V02;
      }
      else
      {
        LOC_MSG_MED("locPd_EventCb, PDSM_PD_EVENT_GPS_DONE", 0, 0, 0);
        pz_LocPdData->engine_state = LOC_ENGINE_STATE_OFF;
        pz_EngineStateIndMsg->engineState = eQMI_LOC_ENGINE_STATE_OFF_V02;
      }

      if (TRUE != locMW_SendLocCmdProxy(
            LOC_HANDLE_REGISTERED_CLIENTS,
            LOC_CMD_TYPE_NOTIFY_CLIENT,
            LOC_EVENT_STATUS_REPORT,
            (void *)pz_EngineStateIndMsg))
      {
        result            = FALSE;
      }
    }
    else
    {
      /* Memory allocation error */
      result = FALSE;
    }

    if (TRUE != result)
    {
      // TODO: what else can we do?
      LOC_MSG_ERROR("locPd_EventCb: failed processing engine status event", 0, 0, 0);
    }
  }

  /* GPS session begins */
  if (q_PdEvent & PDSM_PD_EVENT_BEGIN)
  {
    /* Check whether it is a recognized client */
    if ((pd_info_ptr->client_id != loc_middleware_data.pdapi_client_id) &&
        (pd_info_ptr->client_id != loc_middleware_data.ni_client_id))
    {
      LOC_MSG_ERROR("locPd_EventCb: dropped session_begin for unrecognized client", 0, 0, 0);
    }
    else
    {
      pz_LocPdData->fix_in_progress = TRUE;
      time_get_uptime_ms(time_ms);
      current_time_ms = QW_CVT_Q2N(time_ms);
      pz_LocPdData->last_get_position_time = current_time_ms;

      /* will be set to true if sent by pdapi_client_id */
      pz_LocPdData->mo_fix_in_progress = FALSE;

      result = TRUE; // will be set to FALSE for failure case explicitly

      if (pd_info_ptr->client_id == loc_middleware_data.ni_client_id)
      {
        int type = 0;

        /* NI session starts */
        if (LOC_SUPL_MT_ES_ABOUT_TO_START == pz_LocPdData->supl_mt_fix_state)
        {
          // note we don't really know if this is the right session, but we can only guess
          pz_LocPdData->supl_mt_fix_state = LOC_SUPL_MT_ES_IN_PROGRESS;
          type = 2;
        }
        else if (LOC_SUPL_MT_PO_ABOUT_TO_START == pz_LocPdData->supl_mt_fix_state)
        {
          // note we don't really know if this is the right session, but we can only guess
          pz_LocPdData->supl_mt_fix_state = LOC_SUPL_MT_PO_IN_PROGRESS;
          type = 1;
        }
        else
        {
          // NI but probably not our SUPL MT PO session or SUPL MT ES session
          // note we cannot be sure about this
          type = 3;
        }

        LOC_MSG_HIGH("locPd_EventCb: NI client session begin %d", type, 0, 0);
      }
      /* Notify the active client only in case of MO fixes */
      else if (pd_info_ptr->client_id == loc_middleware_data.pdapi_client_id)
      {
        qmiLocEventFixSessionStateIndMsgT_v02 *pz_FixSessionStateInd = NULL;
        pz_LocPdData->mo_fix_in_progress = TRUE;

        /* Send the fix session state event */
        pz_FixSessionStateInd = (qmiLocEventFixSessionStateIndMsgT_v02 *)loc_calloc(sizeof(*pz_FixSessionStateInd));

        if (NULL != pz_FixSessionStateInd)
        {
          pz_FixSessionStateInd->sessionState = eQMI_LOC_FIX_SESSION_STARTED_V02;

          if (TRUE != locMW_SendLocCmdProxy(
                locClnt_GetActiveClient(),
                LOC_CMD_TYPE_NOTIFY_CLIENT,
                LOC_EVENT_FIX_SESSION_STATE_REPORT,
                (void *)pz_FixSessionStateInd))
          {
            result            = FALSE;
          }
        }
        else
        {
          /* Memory allocation error */
          result = FALSE;
        }

        if (TRUE != result)
        {
          // TODO: what else can we do?
          LOC_MSG_ERROR("locPd_EventCb: failed processing session begin event", 0, 0, 0);
        }
      } // end if (pd_info_ptr->client_id == loc_middleware_data.pdapi_client_id)
    } // end of recognised client
  } // end of if (pd_event & PDSM_PD_EVENT_BEGIN)

  // gps session ends
  if (q_PdEvent & PDSM_PD_EVENT_DONE)
  {
    loc_client_handle_type l_ClientHandle = locClnt_GetActiveClient();
    loc_client_info_s_type *pz_ClientInfo = locClnt_FindHandle(l_ClientHandle);
    if (NULL != pz_ClientInfo)
    {
      pz_ClientInfo->v_FirstFix = FALSE;
    }

    result = TRUE; //will be explicity set to false in case of a failure
    pz_LocPdData->fix_in_progress    = FALSE;

    // inform the client only if this was a MO fix
    if (pd_info_ptr->client_id == loc_middleware_data.pdapi_client_id)
    {
      qmiLocEventFixSessionStateIndMsgT_v02 *pz_FixSessionStateInd = NULL;
      pz_LocPdData->mo_fix_in_progress = FALSE;
      pz_FixSessionStateInd = (qmiLocEventFixSessionStateIndMsgT_v02 *)loc_calloc(sizeof(*pz_FixSessionStateInd));
      if (NULL != pz_FixSessionStateInd)
      {
        pz_FixSessionStateInd->sessionState = eQMI_LOC_FIX_SESSION_FINISHED_V02;

        if (TRUE != locMW_SendLocCmdProxy(
              locClnt_GetActiveClient(),
              LOC_CMD_TYPE_NOTIFY_CLIENT,
              LOC_EVENT_FIX_SESSION_STATE_REPORT,
              (void *)pz_FixSessionStateInd))
        {
          result = FALSE;
        }
      }
      else
      {
        // memory allocation failure
        result = FALSE;
      }

      // positioning session is done, reset the state will send a request to update the scheduler
      locPd_Schedule();
    }
    else if (pd_info_ptr->client_id == loc_middleware_data.ni_client_id)
    {
      // NI positioning session is done, reset the state will send a request to update the scheduler
      pz_LocPdData->supl_mt_fix_state  = LOC_SUPL_MT_IDLE;
      locPd_Schedule();
    }
    else
    {
      //got a session done event for an unknown client
      LOC_MSG_HIGH("locPd_EventCb: dropping session done event , unknown client %d",
                   pd_info_ptr->client_id, 0, 0);
      result = FALSE;
    }

    if (TRUE != result)
    {
      // TODO: what else can we do?
      LOC_MSG_ERROR("locPd_EventCb: failed processing session done event", 0, 0, 0);
    }
  }

  // pd end event, record down the error code
  if (q_PdEvent & PDSM_PD_EVENT_END)
  {
    if (pd_info_ptr->client_id == loc_middleware_data.pdapi_client_id)
    {
      LOC_MSG_MED("locPd_EventCb: end status=%d v_EndSessionForPowerModeSwitch = %u\n",
                  pd_info_ptr->pd_info.end_status, pz_LocPdData->v_EndSessionForPowerModeSwitch, 0);
      // update the position received time
      pz_LocPdData->last_fix_ttf = 0;
      if (FALSE == pz_LocPdData->v_EndSessionForPowerModeSwitch)
      {
        pz_LocPdData->pd_sess_end_status = pd_info_ptr->pd_info.end_status;
        pz_LocPdData->new_pd_sess_end_status_received = TRUE;
        (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                    LOC_CMD_TYPE_REPORT_POSITION,
                                    0,
                                    NULL);
      }
      else
      {
        //reset the flag after ending the active session for power mode switch
        pz_LocPdData->v_EndSessionForPowerModeSwitch = FALSE;
        LOC_MSG_MED("locPd_EventCb set v_EndSessionForPowerModeSwitch = FALSE ",
                    0, 0, 0);
      }
    }
  }

  // pd position event
  if ((q_PdEvent & PDSM_PD_EVENT_POSITION) ||
      (q_PdEvent & PDSM_PD_EVENT_VELOCITY) ||
      (q_PdEvent & PDSM_PD_EVENT_HEIGHT))
  {
    if (pd_info_ptr->client_id == loc_middleware_data.pdapi_client_id)
    {
      if (pd_info_ptr->pd_info.pd_data.position_type != PDSM_PD_POSITION_TYPE_INVALID)
      {
        // update the position received time
        time_get_uptime_ms(time_ms);
        pz_LocPdData->last_fix_arrival_time = QW_CVT_Q2N(time_ms);
        pz_LocPdData->last_fix_ttf = (dword)(pz_LocPdData->last_fix_arrival_time  - pz_LocPdData->last_get_position_time);

        memscpy(&pz_LocPdData->pd_info, sizeof(pz_LocPdData->pd_info), pd_info_ptr, sizeof(*pd_info_ptr));
        pz_LocPdData->new_pd_info_received = TRUE;

        (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                    LOC_CMD_TYPE_REPORT_POSITION,
                                    0,
                                    NULL);
      }
    }
    else
    {
      LOC_MSG_ERROR_2("locPd_EventCb: Drop Position Report. PdInfoClientId %d, LocPdapiClientId %d",
                      pd_info_ptr->client_id, loc_middleware_data.pdapi_client_id);
    }
  }

  // wifi ap scan info injection needed. Ignore client id and send to all (NI/MO)
  if (q_PdEvent & PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION)
  {
    /* need to check for in_progress?? */

    LOC_MSG_MED("locPd_EventCb: PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION", 0, 0, 0);

    /* calling proxy to post ipc to LocMW task */
    locPd_EventWifiApScanInfoInjectIndProxy(&(pd_info_ptr->pd_info.z_wifiOnDemandReq));
  }

  // wifi ap scan info injection ack.
  if (q_PdEvent & PDSM_PD_EVENT_ACK_WIFI_INFO_ON_DEMAND_INJECTION)
  {
    LOC_MSG_MED("locPd_EventCb: PDSM_PD_EVENT_ACK_WIFI_INFO_ON_DEMAND_INJECTION", 0, 0, 0);

    /* calling proxy to post ipc to LocMW task */
    locPd_AckWifiApScanInfoInjectIndProxy(pz_LocPdData->z_InjWifiApScan.l_ClientHandle,
                                          &(pd_info_ptr->pd_info.wifi_inj_ack));
    pz_LocPdData->z_InjWifiApScan.v_InProgress = FALSE;
  }

  // SRN (Short Range Node: BTLE) scan info injection request.
  if (q_PdEvent & PDSM_PD_EVENT_REQUEST_SRN_INFO_INJECTION)
  {
    LOC_MSG_MED("locPd_EventCb: PDSM_PD_EVENT_REQUEST_SRN_INFO_INJECTION", 0, 0, 0);
    /* calling proxy to post ipc to LocMW task */
    locPd_EventSrnApScanInfoInjectIndProxy(&(pd_info_ptr->pd_info.z_srnScanReq));
  }

  // SRN (Short Range Node: BTLE) scan info injection request OK.
  if (q_PdEvent & PDSM_PD_EVENT_ACK_SRN_INFO_INJECTION)
  {
    LOC_MSG_MED("locPd_EventCb: PDSM_PD_EVENT_ACK_WIFI_INFO_ON_DEMAND_INJECTION", 0, 0, 0);

    /* calling proxy to post ipc to LocMW task */
    locPd_AckSrnApScanInfoInjectIndProxy(&(pd_info_ptr->pd_info.srn_inj_ack));
  }

  // Forwarding the event to callback hook
  if (pz_LocPdData->pd_event_cb_hook != NULL)
  {
    pz_LocPdData->pd_event_cb_hook(p_UserData, q_PdEvent, pd_info_ptr);
  }

  if (pd_info_ptr->client_id == loc_middleware_data.pdapi_client_id)
  {
    if ((q_PdEvent & PDSM_PD_EVENT_COARSE_POS_INJ_DONE) ||
        (q_PdEvent & PDSM_PD_EVENT_COARSE_POS_INJ_FAILED) ||
        (q_PdEvent & PDSM_PD_EVENT_COARSE_POS_INJ_ERROR))
    {
      uint32             q_RetVal = LOC_API_GENERAL_FAILURE;

      if (q_PdEvent & PDSM_PD_EVENT_COARSE_POS_INJ_DONE)
      {
        q_RetVal = LOC_API_SUCCESS;
      }

      /* Send IPC message to LocMW task to handle QMI_LOC indication */
      locMW_SendQmiIndicationProxy(pz_LocPdData->z_InjPos.l_ClientHandle,
                                   QMI_LOC_INJECT_POSITION_IND_V02,
                                   NULL, q_RetVal);
    }
  }
  else if (loc_middleware_data.ni_client_id == pd_info_ptr->client_id)
  {
    // event for NI client is also processed by us, so it's not counted toward id mismatch
  }
  else if (PDSM_PD_EVENT_WPS_NEEDED == q_PdEvent)
  {
    // PDSM_PD_EVENT_WPS_NEEDED is sent to everyone with client id "-1"
  }
  else
  {
    LOC_MSG_HIGH("locPd_EventCb: client id mismatch for event=0x%x", q_PdEvent, 0, 0);
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
}

/* Command callback called from PDAPI for a PD command.
   The implementation will rely on loc_middleware_data to identify the client handle etc,
   and will lock the critical section. */
static void locPd_CmdCb
(
   void *p_DataBlock, /* not used */
   pdsm_pd_cmd_e_type     e_CmdType,
   pdsm_pd_cmd_err_e_type e_CmdErrStatus
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  LOC_MSG_MED("locPd_CmdCb: cmd=%d, err=%d", e_CmdType, e_CmdErrStatus, 0);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  /* If error is indicated, no further processing of this PD cmd will be performed. */
  switch (e_CmdType)
  {
    case PDSM_PD_CMD_GET_POS:
      {
        if (PDSM_PD_CMD_ERR_NOERR != e_CmdErrStatus)
        {
          pz_LocPdData->new_pd_cmd_err_received = TRUE;
          pz_LocPdData->pd_cmd_err              = e_CmdErrStatus;
          pz_LocPdData->mo_fix_in_progress      = FALSE;

          /* Send error report (empty location report) to client */
          (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                      LOC_CMD_TYPE_REPORT_POSITION,
                                      0,
                                      NULL);

          locPd_Schedule();
        }
        break;
      }

    case PDSM_PD_CMD_END_SESSION:
    case PDSM_PD_CMD_END_SESSION_EX:
      break;

    case PDSM_PD_CMD_INJECT_POS:
      {
        if (PDSM_PD_CMD_ERR_NOERR != e_CmdErrStatus)
        {
          /* Send IPC message to LocMW task to handle QMI_LOC indication */
          locMW_SendQmiIndicationProxy(pz_LocPdData->z_InjPos.l_ClientHandle,
                                       QMI_LOC_INJECT_POSITION_IND_V02,
                                       NULL,
                                       locPd_ConvertCmdErr2LocStatus(e_CmdErrStatus));

          pz_LocPdData->z_InjPos.v_InProgress = FALSE;
        }
        break;
      }

    case PDSM_PD_CMD_GET_BEST_AVAIL_POS:
      {
        if ((PDSM_PD_CMD_ERR_NOERR != e_CmdErrStatus) &&
            (pz_LocPdData->z_GetBestAvailPos.v_InProgress))
        {
          qmiLocGetBestAvailablePositionIndMsgT_v02 *pz_BestAvailPosInd =
             (qmiLocGetBestAvailablePositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_BestAvailPosInd));

          if (NULL == pz_BestAvailPosInd)
          {
            LOC_MSG_ERROR("locPd_CmdCb: Failed to allocate memory for QMI_LOC Indication", 0, 0, 0);
          }
          else
          {
            /* copy the transaction id which was saved in the incoming call */
            pz_BestAvailPosInd->transactionId_valid = 1;
            pz_BestAvailPosInd->transactionId       = pz_LocPdData->z_GetBestAvailPos.q_TxnId;

            /* Send IPC message to LocMW task to handle QMI_LOC indication */
            locMW_SendQmiIndicationProxy(pz_LocPdData->z_GetBestAvailPos.l_ClientHandle,
                                         QMI_LOC_GET_BEST_AVAILABLE_POSITION_IND_V02,
                                         pz_BestAvailPosInd,
                                         locPd_ConvertCmdErr2LocStatus(e_CmdErrStatus));
          }

          LOC_MSG_ERROR("locPd_CmdCb: Failed to get Best Available position. Error=%d", e_CmdErrStatus, 0, 0);

          /* this code is executed inside critical section so no need enter the critical section here. */
          pz_LocPdData->z_GetBestAvailPos.v_InProgress = FALSE;
        }
        break;
      }

    case PDSM_PD_CMD_GET_AVAIL_WWAN_POS:
      {
        if ((PDSM_PD_CMD_ERR_NOERR != e_CmdErrStatus)
            && (pz_LocPdData->z_GetAvailWwanPos.v_InProgress))
        {
          qmiLocGetAvailWwanPositionIndMsgT_v02 *pz_AvailWwanPosInd =
             (qmiLocGetAvailWwanPositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_AvailWwanPosInd));

          if (NULL == pz_AvailWwanPosInd)
          {
            LOC_MSG_ERROR("locPd_CmdCb: Failed to allocate memory for QMI_LOC Indication", 0, 0, 0);
          }
          else
          {
            /* copy the transaction id which was saved in the incoming call */
            pz_AvailWwanPosInd->transactionId_valid = 1;
            pz_AvailWwanPosInd->transactionId       = pz_LocPdData->z_GetAvailWwanPos.q_TxnId;

            /* Send IPC message to LocMW task to handle QMI_LOC indication */
            locMW_SendQmiIndicationProxy(pz_LocPdData->z_GetAvailWwanPos.l_ClientHandle,
                                         QMI_LOC_GET_AVAILABLE_WWAN_POSITION_IND_V02,
                                         pz_AvailWwanPosInd,
                                         locPd_ConvertCmdErr2LocStatus(e_CmdErrStatus));
          }

          LOC_MSG_ERROR("locPd_CmdCb: Failed to get Available WWAN position. Error=%d", e_CmdErrStatus, 0, 0);

          /* this code is executed inside critical section so no need enter the critical section here. */
          pz_LocPdData->z_GetAvailWwanPos.v_InProgress = FALSE;
        }
        break;
      }

      /* wifi on demand - PDSM_PD_CMD_INJECT_WIFI_ON_DEMAND - will never be called as we are passing null func pointer
         in the pdsm call. Failure QMI RESP is sent when pdsm call return failure. */

    case PDSM_PD_CMD_GET_AVAIL_POS_FOR_SRC:
      {
        if ((PDSM_PD_CMD_ERR_NOERR != e_CmdErrStatus)
            && pz_LocPdData->z_GetSecureAvailPos.v_InProgress)
        {
          qmiLocSessionStatusEnumT_v02  e_SessionStatus;
          uint32                        q_RetVal = LOC_API_GENERAL_FAILURE;
          qmiLocGetAvailablePositionIndParamMsgT_v02 *pz_AvailPosInd =
             (qmiLocGetAvailablePositionIndParamMsgT_v02 *)loc_calloc(sizeof(*pz_AvailPosInd));
          qmiLocSecureGetAvailablePositionIndMsgT_v02 *pz_SecureGetAvailPosInd =
             (qmiLocSecureGetAvailablePositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_SecureGetAvailPosInd));

          if (NULL != pz_AvailPosInd &&
              NULL != pz_SecureGetAvailPosInd)
          {
            /* copy the transaction id which was saved in the incoming call */
            pz_AvailPosInd->transactionId_valid = 1;
            pz_AvailPosInd->transactionId = pz_LocPdData->z_GetSecureAvailPos.q_PosTxnID;
            pz_AvailPosInd->nonce_valid   = pz_LocPdData->z_GetSecureAvailPos.v_PosIsNonceValid;

            if (pz_LocPdData->z_GetSecureAvailPos.v_PosIsNonceValid)
            {
              pz_AvailPosInd->nonce = pz_LocPdData->z_GetSecureAvailPos.t_PosNonce;
            }

            pz_AvailPosInd->e_PosDataMode = pz_LocPdData->z_GetSecureAvailPos.e_PosDataMode;

            LOC_MSG_MED("locPd_CmdCb: Transaction ID %d Nonce %x Data Mode %d",
                        pz_AvailPosInd->transactionId, pz_AvailPosInd->nonce, pz_AvailPosInd->e_PosDataMode);
            LOC_MSG_MED("locPd_CmdCb: e_CmdErrStatus %d HorizontalConfidence_valid %d  %d",
                        e_CmdErrStatus, pz_AvailPosInd->horCircularConfidence_valid, pz_AvailPosInd->horCircularConfidence);

            switch (e_CmdErrStatus)
            {
              case PDSM_PD_CMD_ERR_NOERR:
                e_SessionStatus = eQMI_LOC_SESS_STATUS_SUCCESS_V02;
                q_RetVal = LOC_API_SUCCESS;
                break;
              case PDSM_PD_CMD_ERR_BUSY_S:
                e_SessionStatus = eQMI_LOC_SESS_STATUS_IN_PROGRESS_V02;
                q_RetVal = LOC_API_ENGINE_BUSY;
                break;
              case PDSM_PD_CMD_ERR_INVALID_PARAM:
                e_SessionStatus = eQMI_LOC_SESS_STATUS_BAD_PARAMETER_V02;
                q_RetVal = LOC_API_INVALID_PARAMETER;
                break;
              case PDSM_PD_CMD_ERR_OFFLINE_S:
                e_SessionStatus = eQMI_LOC_SESS_STATUS_PHONE_OFFLINE_V02;
                q_RetVal = LOC_API_PHONE_OFFLINE;
                break;
              case PDSM_PD_CMD_ERR_GPS_LOCK_S:
                e_SessionStatus = eQMI_LOC_SESS_STATUS_ENGINE_LOCKED_V02;
                q_RetVal = LOC_API_GNSS_DISABLED;
                break;
              default:
                e_SessionStatus = eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02;
                q_RetVal = LOC_API_GENERAL_FAILURE;
                break;
            }

            if (FALSE == locPd_EncodeGetAvailableGnssPos(
                  pz_AvailPosInd,
                  pz_SecureGetAvailPosInd,
                  e_SessionStatus))
            {
              e_SessionStatus = eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02;
              q_RetVal = LOC_API_GENERAL_FAILURE;
            }

            /* Send IPC message to LocMW task to handle QMI_LOC indication */
            locMW_SendQmiIndicationProxy(pz_LocPdData->z_GetSecureAvailPos.l_ClientHandle,
                                         QMI_LOC_SECURE_GET_AVAILABLE_POSITION_IND_V02,
                                         pz_SecureGetAvailPosInd,
                                         q_RetVal);
            loc_free(pz_AvailPosInd);
          }
          else
          {
            LOC_MSG_ERROR("locPd_CmdCb: Failed to allocate memory for PDSM_PD_CMD_GET_AVAIL_POS_FOR_SRC Indication", 0, 0, 0);
          }

          /* this code is executed inside critical section so no need enter the critical section here. */
          LOC_MSG_ERROR("locPd_CmdCb: set z_GetSecureAvailPos.v_InProgress to FALSE", 0, 0, 0);
          pz_LocPdData->z_GetSecureAvailPos.v_InProgress = FALSE;
        }
        else
        {
          LOC_MSG_MED("locPd_CmdCb: PDSM_PD_CMD_GET_AVAIL_POS_FOR_SRC. Error=%d, z_GetSecureAvailPos.v_InProgress=%d",
                      e_CmdErrStatus, pz_LocPdData->z_GetSecureAvailPos.v_InProgress, 0);
        }
        break;
      }
      // No such command or not supported
    default:
      {
        LOC_MSG_ERROR("locPd_CmdCb: cmd not supported", 0, 0, 0);
        break;
      }
  }
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
}

/** locPd_CmdCbUnlocked
 @details: The command callback called in PDSM context, for a PD
           command that was previously sent to PDAPI. The function
           should generate an IPC to notify Loc MW to send an
           QMI_LOC Indication for the corresponding command.
           Extracts the Loc MW client and PD information from the
           DataBlock ptr without any need for locking the Loc MW
           critical section.

 @warning: Should be invoked only for commands that don't have a
           corresponding event_cb. PDAPI gives out the correct
           DataBlock ptr in command callbacks, but not in the
           event callbacks.

 @param[in]: p_DataBlock   : data pointer passed in during command invocation
 @param[in]: e_CmdType     : PDSM command type for which this callback is called
 @param[in]: e_CmdErrStatus: Command processing result
*/
static void locPd_CmdCbUnlocked
(
   void *p_DataBlock,
   pdsm_pd_cmd_e_type     e_CmdType,
   pdsm_pd_cmd_err_e_type e_CmdErrStatus
   )
{
  locPd_CmdCbData *pz_CmdCbData = (locPd_CmdCbData *)p_DataBlock;
  uint32           q_Status    = locPd_ConvertCmdErr2LocStatus(e_CmdErrStatus);
  uint32           q_MsgId     = 0;

  /* Function arguments sanity check */
  if (NULL == pz_CmdCbData || pz_CmdCbData != pz_CmdCbData->p_Self)
  {
    LOC_MSG_ERROR("locPd_CmdCbUnlocked: invalid callback data %p", pz_CmdCbData, 0, 0);
    return;
  }

  LOC_MSG_MED("locPd_CmdCbUnlocked: e_CmdType=%d=%d, error=%u", pz_CmdCbData->e_CmdType, e_CmdType, q_Status);

  q_MsgId = pz_CmdCbData->q_QmiLocIndId;

  switch (pz_CmdCbData->e_CmdType)
  {
    case PDSM_PD_CMD_INJECT_CELL_INFO:
      {
        if ((QMI_LOC_INJECT_GSM_CELL_INFO_IND_V02             == q_MsgId) ||
            (QMI_LOC_WWAN_OUT_OF_SERVICE_NOTIFICATION_IND_V02 == q_MsgId))
        {
          /* Send IPC message to LocMW task to handle QMI_LOC indication */
          locMW_SendQmiIndicationProxy(pz_CmdCbData->l_ClientHandle,
                                       q_MsgId,
                                       NULL,
                                       q_Status);
        }
        else
        {
          LOC_MSG_ERROR("locPd_CmdCbUnlocked: Invalid QMI_LOC Indication MsgId=%d", q_MsgId, 0, 0);
        }

        break;
      }
    case PDSM_PD_CMD_INJECT_NI_MSG:
      {
        if (QMI_LOC_INJECT_NETWORK_INITIATED_MESSAGE_IND_V02 == q_MsgId)
        {
          /* Send IPC message to LocMW task to handle QMI_LOC indication */
          locMW_SendQmiIndicationProxy(pz_CmdCbData->l_ClientHandle,
                                       q_MsgId,
                                       NULL,
                                       q_Status);
        }
        else
        {
          LOC_MSG_ERROR("locPd_CmdCbUnlocked: Invalid QMI_LOC Indication MsgId=%d", q_MsgId, 0, 0);
        }

        break;
      }

    default:
      {
        LOC_MSG_ERROR("locPd_CmdCbUnlocked: Unknown PD CmdType=%d", pz_CmdCbData->e_CmdType, 0, 0);
        break;
      }
  }
  loc_free(pz_CmdCbData);
}

static void locPd_HandleSvPolyReport
(
   const pdsm_ext_status_info_s_type *pz_ExtStatusInfo
   )
{
  const pdsm_pd_ext_SvPoly_s_type *pz_PdsmSvPoly = NULL;
  qmiLocEventGnssSvPolyIndMsgT_v02 *pz_GnssSvPolyIndMsg =
     (qmiLocEventGnssSvPolyIndMsgT_v02 *)loc_calloc(sizeof(*pz_GnssSvPolyIndMsg));
  int                               i;

  if ((NULL == pz_ExtStatusInfo) || (NULL == pz_GnssSvPolyIndMsg))
  {
    LOC_MSG_ERROR_2("locPd_HandleSvPolyReport NULL ptr ExtStatus %d QMI %d",
      (NULL == pz_ExtStatusInfo), (NULL == pz_GnssSvPolyIndMsg));
    return;
  }
  pz_PdsmSvPoly = &(pz_ExtStatusInfo->ext_status_info.ext_sv_poly_type);
  pz_GnssSvPolyIndMsg->gnssSvId = pz_PdsmSvPoly->w_Sv;
  pz_GnssSvPolyIndMsg->T0 = pz_PdsmSvPoly->d_T0;

  if (pz_PdsmSvPoly->w_SvPolyFlags & PDSM_SV_POLY_FLAGS_COEFF)
  {
      pz_GnssSvPolyIndMsg->svPolyFlagValid = (QMI_LOC_SV_POLY_SRC_ALM_CORR_VALID_V02 | QMI_LOC_SV_POLY_GLO_STR4_VALID_V02);

    if (PDSM_SV_POLY_FLAGS_SRC_ALM_CORR & pz_PdsmSvPoly->w_SvPolyFlags)
      {
        pz_GnssSvPolyIndMsg->svPolyFlags |= QMI_LOC_SV_POLY_SRC_ALM_CORR_V02;
      }
    if (PDSM_SV_POLY_FLAGS_GLO_STR4 & pz_PdsmSvPoly->w_SvPolyFlags)
      {
        pz_GnssSvPolyIndMsg->svPolyFlags |= QMI_LOC_SV_POLY_GLO_STR4_V02;
      }
    if (GAL_ID_OK(pz_PdsmSvPoly->w_Sv))
    {
      pz_GnssSvPolyIndMsg->svPolyFlagValid |= QMI_LOC_SV_POLY_SRC_GAL_FNAV_OR_INAV_VALID_V02;
      if (PDSM_SV_POLY_FLAGS_FNAV & pz_PdsmSvPoly->w_SvPolyFlags)
      {
        pz_GnssSvPolyIndMsg->svPolyFlags |= QMI_LOC_SV_POLY_SRC_GAL_FNAV_OR_INAV_V02;
      }
    }

    if (SLOTID_OK(pz_PdsmSvPoly->w_Sv) ||
        (GLO_SLOTID_UNKNOWN == pz_PdsmSvPoly->w_Sv))
    {
      pz_GnssSvPolyIndMsg->gloFrequency_valid = TRUE;
      pz_GnssSvPolyIndMsg->gloFrequency = pz_PdsmSvPoly->b_FreqNum;
    }
      pz_GnssSvPolyIndMsg->IODE_valid = TRUE;
      pz_GnssSvPolyIndMsg->IODE = pz_PdsmSvPoly->w_Iode;

      pz_GnssSvPolyIndMsg->enhancedIOD_valid = FALSE;
      pz_GnssSvPolyIndMsg->enhancedIOD = 0;

      pz_GnssSvPolyIndMsg->polyCoeffXYZ0_valid = TRUE;
      for (i = 0; i < QMI_LOC_SV_POLY_XYZ_0_TH_ORDER_COEFF_SIZE_V02; i++)
      {
        pz_GnssSvPolyIndMsg->polyCoeffXYZ0[i] = pz_PdsmSvPoly->pd_xyz0[i];
      }

      pz_GnssSvPolyIndMsg->polyCoefXYZN_valid = TRUE;
      for (i = 0; i < QMI_LOC_SV_POLY_XYZ_N_TH_ORDER_COEFF_SIZE_V02; i++)
      {
        pz_GnssSvPolyIndMsg->polyCoefXYZN[i] = pz_PdsmSvPoly->pd_xyzN[i];
      }

      pz_GnssSvPolyIndMsg->polyCoefClockBias_valid = TRUE;
      for (i = 0; i < QMI_LOC_SV_POLY_SV_CLKBIAS_COEFF_SIZE_V02; i++)
      {
        pz_GnssSvPolyIndMsg->polyCoefClockBias[i] = pz_PdsmSvPoly->pf_other[i];
      }

      pz_GnssSvPolyIndMsg->svPosUnc_valid = TRUE;
      pz_GnssSvPolyIndMsg->svPosUnc = pz_PdsmSvPoly->f_PosUnc;

    if (pz_PdsmSvPoly->w_SvPolyFlags & PDSM_SV_POLY_FLAGS_IONO)
      {
        pz_GnssSvPolyIndMsg->ionoDelay_valid = TRUE;
        pz_GnssSvPolyIndMsg->ionoDelay = pz_PdsmSvPoly->f_IonoDelay;

        pz_GnssSvPolyIndMsg->ionoDot_valid = TRUE;
        pz_GnssSvPolyIndMsg->ionoDot = pz_PdsmSvPoly->f_IonoDot;
      }

    if (pz_PdsmSvPoly->w_SvPolyFlags & PDSM_SV_POLY_FLAGS_SBAS_IONO)
      {
        pz_GnssSvPolyIndMsg->sbasIonoDelay_valid = TRUE;
        pz_GnssSvPolyIndMsg->sbasIonoDelay = pz_PdsmSvPoly->f_sbasIonoDelay;

        pz_GnssSvPolyIndMsg->sbasIonoDot_valid = TRUE;
        pz_GnssSvPolyIndMsg->sbasIonoDot = pz_PdsmSvPoly->f_sbasIonoDot;
      }

    if (pz_PdsmSvPoly->w_SvPolyFlags & PDSM_SV_POLY_FLAGS_TROPO)
      {
        pz_GnssSvPolyIndMsg->tropoDelay_valid = TRUE;
        pz_GnssSvPolyIndMsg->tropoDelay = pz_PdsmSvPoly->f_TropoDelay;
      }

    if (pz_PdsmSvPoly->w_SvPolyFlags & PDSM_SV_POLY_FLAGS_ELEV)
      {
        pz_GnssSvPolyIndMsg->elevation_valid = TRUE;
        pz_GnssSvPolyIndMsg->elevation = pz_PdsmSvPoly->f_Elevation;

        pz_GnssSvPolyIndMsg->elevationDot_valid = TRUE;
        pz_GnssSvPolyIndMsg->elevationDot = pz_PdsmSvPoly->f_ElevDot;

        pz_GnssSvPolyIndMsg->elenationUnc_valid = TRUE;
        pz_GnssSvPolyIndMsg->elenationUnc = pz_PdsmSvPoly->f_ElevUnc;
      }

      pz_GnssSvPolyIndMsg->velCoef_valid = TRUE;
      for (i = 0; i < QMI_LOC_SV_POLY_VELOCITY_COEF_SIZE_V02; i++)
      {
        pz_GnssSvPolyIndMsg->velCoef[i] = pz_PdsmSvPoly->d_VelCoef[i];
      }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GPS_ISC_L1CA_VALID)
    {
      pz_GnssSvPolyIndMsg->gpsIscL1ca_valid = TRUE;
      pz_GnssSvPolyIndMsg->gpsIscL1ca = pz_PdsmSvPoly->f_gpsIscL1ca;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GPS_ISC_L2C_VALID)
    {
      pz_GnssSvPolyIndMsg->gpsIscL2c_valid = TRUE;
      pz_GnssSvPolyIndMsg->gpsIscL2c = pz_PdsmSvPoly->f_gpsIscL2c;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GPS_ISC_L5I5_VALID)
    {
      pz_GnssSvPolyIndMsg->gpsIscL5I5_valid = TRUE;
      pz_GnssSvPolyIndMsg->gpsIscL5I5 = pz_PdsmSvPoly->f_gpsIscL5I5;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GPS_ISC_L5Q5_VALID)
    {
      pz_GnssSvPolyIndMsg->gpsIscL5Q5_valid = TRUE;
      pz_GnssSvPolyIndMsg->gpsIscL5Q5 = pz_PdsmSvPoly->f_gpsIscL5Q5;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GPS_TGD_VALID)
    {
      pz_GnssSvPolyIndMsg->gpsTgd_valid = TRUE;
      pz_GnssSvPolyIndMsg->gpsTgd = pz_PdsmSvPoly->f_gpsTgd;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GLO_TGD_G1G2_VALID)
    {
      pz_GnssSvPolyIndMsg->gloTgdG1G2_valid = TRUE;
      pz_GnssSvPolyIndMsg->gloTgdG1G2 = pz_PdsmSvPoly->f_gloTgdG1G2;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_BDS_TGD_B1_VALID)
    {
      pz_GnssSvPolyIndMsg->bdsTgdB1_valid = TRUE;
      pz_GnssSvPolyIndMsg->bdsTgdB1 = pz_PdsmSvPoly->f_bdsTgdB1;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_BDS_TGD_B2_VALID)
    {
      pz_GnssSvPolyIndMsg->bdsTgdB2_valid = TRUE;
      pz_GnssSvPolyIndMsg->bdsTgdB2 = pz_PdsmSvPoly->f_bdsTgdB2;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_BDS_TGD_B2A_VALID)
    {
      pz_GnssSvPolyIndMsg->bdsTgdB2a_valid = TRUE;
      pz_GnssSvPolyIndMsg->bdsTgdB2a = pz_PdsmSvPoly->f_bdsTgdB2a;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_BDS_ISC_B2A_VALID)
    {
      pz_GnssSvPolyIndMsg->bdsIscB2a_valid = TRUE;
      pz_GnssSvPolyIndMsg->bdsIscB2a = pz_PdsmSvPoly->f_bdsIscB2a;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GAL_BGD_E1E5A_VALID)
    {
      pz_GnssSvPolyIndMsg->galBgdE1E5a_valid = TRUE;
      pz_GnssSvPolyIndMsg->galBgdE1E5a = pz_PdsmSvPoly->f_galBgdE1E5a;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_GAL_BGD_E1E5B_VALID)
    {
      pz_GnssSvPolyIndMsg->galBgdE1E5b_valid = TRUE;
      pz_GnssSvPolyIndMsg->galBgdE1E5b = pz_PdsmSvPoly->f_galBgdE1E5b;
    }
    if (pz_PdsmSvPoly->t_SvPolyValidityMask & PDSM_NAVIC_TGD_L5_VALID)
    {
      pz_GnssSvPolyIndMsg->navicTgdL5_valid = TRUE;
      pz_GnssSvPolyIndMsg->navicTgdL5 = pz_PdsmSvPoly->f_navicTgdL5;
    }
  }
  else if (pz_PdsmSvPoly->w_SvPolyFlags & PDSM_SV_POLY_FLAGS_INIT)
  {
    pz_GnssSvPolyIndMsg->svPolyFlagValid = QMI_LOC_SV_POLY_DELETE_VALID_V02;
    pz_GnssSvPolyIndMsg->svPolyFlags |= QMI_LOC_SV_POLY_DELETE_V02;
  }

    (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                LOC_CMD_TYPE_NOTIFY_CLIENT,
                                LOC_EVENT_SV_POLY_REPORT,
                                (void *)pz_GnssSvPolyIndMsg);
  }

/* -----------------------------------------------------------------------*//**
@brief
Mapping of masks from PD SV system enum to QMI-Loc enum.
This is a singleton, there is no need to define another instance of this
*/ /* ------------------------------------------------------------------------*/
static qmiLocSvSystemEnumT_v02 locPd_PdsmGnssSystemToQmi(pdsm_constellation_e_type t_PdEnum)
{
  switch (t_PdEnum)
{
    case PDSM_CONSTELLATION_TYPE_GPS:
      return eQMI_LOC_SV_SYSTEM_GPS_V02;
    case PDSM_CONSTELLATION_TYPE_GALILEO:
      return eQMI_LOC_SV_SYSTEM_GALILEO_V02;
    case PDSM_CONSTELLATION_TYPE_SBAS:
      return eQMI_LOC_SV_SYSTEM_SBAS_V02;
    case PDSM_CONSTELLATION_TYPE_COMPASS:
      return eQMI_LOC_SV_SYSTEM_COMPASS_V02;
    case PDSM_CONSTELLATION_TYPE_GLONASS:
      return eQMI_LOC_SV_SYSTEM_GLONASS_V02;
    case PDSM_CONSTELLATION_TYPE_BDS:
      return eQMI_LOC_SV_SYSTEM_BDS_V02;
    case PDSM_CONSTELLATION_TYPE_QZSS:
      return eQMI_LOC_SV_SYSTEM_QZSS_V02;
    case PDSM_CONSTELLATION_TYPE_NAVIC:
      return eQMI_LOC_SV_SYSTEM_NAVIC_V02;
    default:
      LOC_MSG_ERROR_1("Invalid Gnss System %lu. Default to GPS", (uint32)t_PdEnum);
      return eQMI_LOC_SV_SYSTEM_GPS_V02;
  }
};

/* -----------------------------------------------------------------------*//**
@brief
Utility function to convert from pdsm_extended_time_src_e_type to
qmiLocTimeSourceEnumT_v02

@param     ext_time_source          PDAPI format of time source indicator

@retval    Time Source in QMI Format
*/ /* ------------------------------------------------------------------------*/
static qmiLocTimeSourceEnumT_v02 locPd_ConvToLocTimeSource
(
   pdsm_extended_time_src_e_type       e_ExtTimeSrc
   )
{
  switch (e_ExtTimeSrc)
  {
    case PDSM_GNSS_TIME_SRC_RESERVED: /* Fall Through */
    case PDSM_GNSS_TIME_SRC_INVALID:
      return eQMI_LOC_TIME_SRC_INVALID_V02;

    case PDSM_GNSS_TIME_SRC_UNKNOWN:
      return eQMI_LOC_TIME_SRC_UNKNOWN_V02;

    case PDSM_GNSS_TIME_SRC_NETWORK_TIME_TRANSFER:
      return eQMI_LOC_TIME_SRC_NETWORK_TIME_TRANSFER_V02;

    case PDSM_GNSS_TIME_SRC_NETWORK_TIME_TAGGING:
      return eQMI_LOC_TIME_SRC_NETWORK_TIME_TAGGING_V02;

    case PDSM_GNSS_TIME_SRC_EXTERNAL_INPUT:
      return eQMI_LOC_TIME_SRC_EXTERNAL_INPUT_V02;

    case PDSM_GNSS_TIME_SRC_GPS_TOW_DECODE:
      return eQMI_LOC_TIME_SRC_TOW_DECODE_V02;

    case PDSM_GNSS_TIME_SRC_TOW_CONFIRMED:
      return eQMI_LOC_TIME_SRC_TOW_CONFIRMED_V02;

    case PDSM_GNSS_TIME_SRC_TOW_WEEK_CONFIRMED:
      return eQMI_LOC_TIME_SRC_TOW_AND_WEEK_CONFIRMED_V02;

    case PDSM_GNSS_TIME_SRC_NAV_SOLUTION:
      return eQMI_LOC_TIME_SRC_NAV_SOLUTION_V02;

    case PDSM_GNSS_TIME_SRC_SOLVE_FOR_TIME:
      return eQMI_LOC_TIME_SRC_SOLVE_FOR_TIME_V02;

    case PDSM_GNSS_TIME_SRC_GLO_TOW_DECODE:
      return eQMI_LOC_TIME_SRC_GLO_TOW_DECODE_V02;

    case PDSM_GNSS_TIME_SRC_TIME_TRANSFORM:
      return eQMI_LOC_TIME_SRC_TIME_TRANSFORM_V02;

    case PDSM_GNSS_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING:
      return eQMI_LOC_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING_V02;

    case PDSM_GNSS_TIME_SRC_GSM_SLEEP_TIME_TAGGING:
      return eQMI_LOC_TIME_SRC_GSM_SLEEP_TIME_TAGGING_V02;

    case PDSM_GNSS_TIME_SRC_QZSS_TOW_DECODE:
      return eQMI_LOC_TIME_SRC_QZSS_TOW_DECODE_V02;

    case PDSM_GNSS_TIME_SRC_BDS_TOW_DECODE:
      return eQMI_LOC_TIME_SRC_BDS_TOW_DECODE_V02;

    case PDSM_GNSS_TIME_SRC_GAL_TOW_DECODE :
      return eQMI_LOC_TIME_SRC_GAL_TOW_DECODE_V02;

    case PDSM_GNSS_TIME_SRC_NAVIC_TOW_DECODE :
      return eQMI_LOC_TIME_SRC_NAVIC_TOW_DECODE_V02;

         default:
      LOC_MSG_ERROR_1("locPd_ConvToLocTimeSource failed:%d", e_ExtTimeSrc);
      return eQMI_LOC_TIME_SRC_INVALID_V02;
  }
}

//Runs in SM task context when locPd_ExtEventCb function is called
static void locPd_HandleSvMeasReport
(
   const pdsm_ext_status_info_s_type *pz_ExtStatusInfo
   )
{
  const pdsm_pd_ext_gnss_meas_s_type    *pz_PdsmSvMeas = &(pz_ExtStatusInfo->ext_status_info.ext_gnss_meas_report);
  qmiLocEventGnssSvMeasInfoIndMsgT_v02  *pz_GnssSvMeasIndMsg = NULL;
  int                                    i;
  qmiLocSVMeasurementStructT_v02        *pz_dstSvMeas = NULL;
  const pdsm_LocSVMeasurementStructType *pz_SrcSVMeas = NULL;

  if (NULL == pz_ExtStatusInfo)
  {
    LOC_MSG_ERROR("locPd_HandleSvMeasReport NULL status info ptr", 0, 0, 0);
    return;
  }

  pz_GnssSvMeasIndMsg = (qmiLocEventGnssSvMeasInfoIndMsgT_v02 *)loc_calloc(sizeof(*pz_GnssSvMeasIndMsg));
  if (NULL == pz_GnssSvMeasIndMsg)
  {
    LOC_MSG_ERROR_0("locPd_HandleSvMeasReport, loc_calloc failed");
    return;
  }

  /* TLV 0x01 Current Sequence Number */
  pz_GnssSvMeasIndMsg->seqNum = pz_PdsmSvMeas->u_seqNum;

  /* TLV 0x02 Maximum Number of Sequences */
  pz_GnssSvMeasIndMsg->maxMessageNum = pz_PdsmSvMeas->u_maxMessageNum;

  /* TLV 0x03 Gnss System */
  pz_GnssSvMeasIndMsg->system = locPd_PdsmGnssSystemToQmi(pz_PdsmSvMeas->z_GnssMeas.e_system);

  /* TLV 0x10 Populate clock frequency information */
  pz_GnssSvMeasIndMsg->rcvrClockFrequencyInfo_valid = TRUE;
  pz_GnssSvMeasIndMsg->rcvrClockFrequencyInfo.clockDrift = pz_PdsmSvMeas->z_clockFreq.f_clockDrift;
  pz_GnssSvMeasIndMsg->rcvrClockFrequencyInfo.clockDriftUnc = pz_PdsmSvMeas->z_clockFreq.f_clockDriftUnc;
  pz_GnssSvMeasIndMsg->rcvrClockFrequencyInfo.sourceOfFreq = (qmiLocSourceofFreqEnumT_v02)pz_PdsmSvMeas->z_clockFreq.e_sourceOfFreq;

  /* TLV 0x11 Populate leap second information */
  if (pz_PdsmSvMeas->b_LeapSecValid)
  {
    pz_GnssSvMeasIndMsg->leapSecondInfo_valid = TRUE;
    pz_GnssSvMeasIndMsg->leapSecondInfo.leapSec = pz_PdsmSvMeas->z_LeapSec.u_leapSec;
    pz_GnssSvMeasIndMsg->leapSecondInfo.leapSecUnc = pz_PdsmSvMeas->z_LeapSec.u_leapSecUnc;
  }

  /* TLV 0x12 GPS-GLO Inter-system time biases */
  if (pz_PdsmSvMeas->z_gpsGloInterSystemBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->gpsGloInterSystemBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->gpsGloInterSystemBias.validMask = pz_PdsmSvMeas->z_gpsGloInterSystemBias.u_validMask;
    pz_GnssSvMeasIndMsg->gpsGloInterSystemBias.timeBias = pz_PdsmSvMeas->z_gpsGloInterSystemBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->gpsGloInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_gpsGloInterSystemBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x13 GPS-BDS Inter-system time biases */
  if (pz_PdsmSvMeas->z_gpsBdsInterSystemBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->gpsBdsInterSystemBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->gpsBdsInterSystemBias.validMask = pz_PdsmSvMeas->z_gpsBdsInterSystemBias.u_validMask;
    pz_GnssSvMeasIndMsg->gpsBdsInterSystemBias.timeBias = pz_PdsmSvMeas->z_gpsBdsInterSystemBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->gpsBdsInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_gpsBdsInterSystemBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x14 GPS-GAL Inter-system time biases */
  if (pz_PdsmSvMeas->z_gpsGalInterSystemBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->gpsGalInterSystemBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->gpsGalInterSystemBias.validMask = pz_PdsmSvMeas->z_gpsGalInterSystemBias.u_validMask;
    pz_GnssSvMeasIndMsg->gpsGalInterSystemBias.timeBias = pz_PdsmSvMeas->z_gpsGalInterSystemBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->gpsGalInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_gpsGalInterSystemBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x15 BDS-GLO Inter-system time biases */
  if (pz_PdsmSvMeas->z_bdsGloInterSystemBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->bdsGloInterSystemBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->bdsGloInterSystemBias.validMask = pz_PdsmSvMeas->z_bdsGloInterSystemBias.u_validMask;
    pz_GnssSvMeasIndMsg->bdsGloInterSystemBias.timeBias = pz_PdsmSvMeas->z_bdsGloInterSystemBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->bdsGloInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_bdsGloInterSystemBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x16 GAL-GLO Inter-system time biases */
  if (pz_PdsmSvMeas->z_galGloInterSystemBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->galGloInterSystemBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->galGloInterSystemBias.validMask = pz_PdsmSvMeas->z_galGloInterSystemBias.u_validMask;
    pz_GnssSvMeasIndMsg->galGloInterSystemBias.timeBias = pz_PdsmSvMeas->z_galGloInterSystemBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->galGloInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_galGloInterSystemBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x17 GAL-BDS Inter-system time biases */
  if (pz_PdsmSvMeas->z_galBdsInterSystemBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->galBdsInterSystemBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->galBdsInterSystemBias.validMask = pz_PdsmSvMeas->z_galBdsInterSystemBias.u_validMask;
    pz_GnssSvMeasIndMsg->galBdsInterSystemBias.timeBias = pz_PdsmSvMeas->z_galBdsInterSystemBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->galBdsInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_galBdsInterSystemBias.f_sys1Sys2TBUnc;
  }

  if(pz_PdsmSvMeas->z_gpsNavicInterSystemBias.u_validMask)
      {
     pz_GnssSvMeasIndMsg->gpsNavicInterSystemBias_valid = TRUE;
     pz_GnssSvMeasIndMsg->gpsNavicInterSystemBias.validMask = pz_PdsmSvMeas->z_gpsNavicInterSystemBias.u_validMask;
     pz_GnssSvMeasIndMsg->gpsNavicInterSystemBias.timeBias = pz_PdsmSvMeas->z_gpsNavicInterSystemBias.f_sys1Sys2TB;
     pz_GnssSvMeasIndMsg->gpsNavicInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_gpsNavicInterSystemBias.f_sys1Sys2TBUnc;
  }

  if(pz_PdsmSvMeas->z_galNavicInterSystemBias.u_validMask)
         {
     pz_GnssSvMeasIndMsg->galNavicInterSystemBias_valid = TRUE;
     pz_GnssSvMeasIndMsg->galNavicInterSystemBias.validMask = pz_PdsmSvMeas->z_galNavicInterSystemBias.u_validMask;
     pz_GnssSvMeasIndMsg->galNavicInterSystemBias.timeBias = pz_PdsmSvMeas->z_galNavicInterSystemBias.f_sys1Sys2TB;
     pz_GnssSvMeasIndMsg->galNavicInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_galNavicInterSystemBias.f_sys1Sys2TBUnc;
  }

  if(pz_PdsmSvMeas->z_gloNavicInterSystemBias.u_validMask)
            {
     pz_GnssSvMeasIndMsg->gloNavicInterSystemBias_valid = TRUE;
     pz_GnssSvMeasIndMsg->gloNavicInterSystemBias.validMask = pz_PdsmSvMeas->z_gloNavicInterSystemBias.u_validMask;
     pz_GnssSvMeasIndMsg->gloNavicInterSystemBias.timeBias = pz_PdsmSvMeas->z_gloNavicInterSystemBias.f_sys1Sys2TB;
     pz_GnssSvMeasIndMsg->gloNavicInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_gloNavicInterSystemBias.f_sys1Sys2TBUnc;
            }

  if(pz_PdsmSvMeas->z_bdsNavicInterSystemBias.u_validMask)
  {
     pz_GnssSvMeasIndMsg->bdsNavicInterSystemBias_valid = TRUE;
     pz_GnssSvMeasIndMsg->bdsNavicInterSystemBias.validMask = pz_PdsmSvMeas->z_bdsNavicInterSystemBias.u_validMask;
     pz_GnssSvMeasIndMsg->bdsNavicInterSystemBias.timeBias = pz_PdsmSvMeas->z_bdsNavicInterSystemBias.f_sys1Sys2TB;
     pz_GnssSvMeasIndMsg->bdsNavicInterSystemBias.timeBiasUnc = pz_PdsmSvMeas->z_bdsNavicInterSystemBias.f_sys1Sys2TBUnc;
         }

  /* TLV 0x18 & TLV 0x19 System Clock Information */
  if (pz_PdsmSvMeas->z_GnssMeas.b_isSystemTimeValid)
  {
    const pdsm_LocSystemTimeStructType *p_src = &pz_PdsmSvMeas->z_GnssMeas.z_systemTime;

    if (pz_GnssSvMeasIndMsg->system == eQMI_LOC_SV_SYSTEM_GLONASS_V02)
    {
      qmiLocGloTimeStructT_v02 *p_dst = &pz_GnssSvMeasIndMsg->gloTime;
      p_dst->gloFourYear = p_src->u_gloFourYear;
      p_dst->gloDays = p_src->w_gloDays;
      p_dst->gloMsec = p_src->q_systemMsec;
      p_dst->gloClkTimeBias = p_src->f_systemClkTimeBias;
      p_dst->gloClkTimeUncMs = p_src->f_systemClkTimeUncMs;
      pz_GnssSvMeasIndMsg->gloTime_valid = TRUE;
    }
    else
    {
      qmiLocGnssTimeStructT_v02 *p_dst = &pz_GnssSvMeasIndMsg->systemTime;
      const pdsm_LocSystemTimeStructType *p_src = &pz_PdsmSvMeas->z_GnssMeas.z_systemTime;
      p_dst->system = pz_GnssSvMeasIndMsg->system;
      p_dst->systemWeek = p_src->w_systemWeek;
      p_dst->systemMsec = p_src->q_systemMsec;
      p_dst->systemClkTimeBias = p_src->f_systemClkTimeBias;
      p_dst->systemClkTimeUncMs = p_src->f_systemClkTimeUncMs;
      pz_GnssSvMeasIndMsg->systemTime_valid = TRUE;
    }
  }

  /* TLV 0x1A Extended Time Information */
  pz_GnssSvMeasIndMsg->systemTimeExt_valid = TRUE;
  pz_GnssSvMeasIndMsg->systemTimeExt.refFCount = pz_PdsmSvMeas->z_GnssMeas.z_systemTime.q_refFCount;
  pz_GnssSvMeasIndMsg->systemTimeExt.systemRtc_valid = pz_PdsmSvMeas->z_GnssMeas.z_systemTime.b_SystemRtcValid;
  pz_GnssSvMeasIndMsg->systemTimeExt.systemRtcMs = pz_PdsmSvMeas->z_GnssMeas.z_systemTime.t_systemRtcMs;
  pz_GnssSvMeasIndMsg->systemTimeExt.sourceOfTime = locPd_ConvToLocTimeSource((pdsm_extended_time_src_e_type)(pz_PdsmSvMeas->z_GnssMeas.z_systemTime.e_SourceOfTime));

  /* TLV 0x30 Field is not supported in measurement report. To be added in LT 11.1 */
  pz_GnssSvMeasIndMsg->refCountTicks_valid = FALSE;

  /* TLV 0x1B & 0x1D Populate SV Measurements */
  LOC_MSG_MED("locPd_HandleSvMeasReport:Fcount:%d,NumSv%d", pz_GnssSvMeasIndMsg->systemTimeExt.refFCount, pz_PdsmSvMeas->z_GnssMeas.u_NumSvs, 0);
  if(pz_PdsmSvMeas->z_GnssMeas.u_NumSvs <= QMI_LOC_SV_MEAS_LIST_MAX_SIZE_V02)
               {
    pz_GnssSvMeasIndMsg->svMeasurement_len = pz_PdsmSvMeas->z_GnssMeas.u_NumSvs;
    pz_GnssSvMeasIndMsg->extSvMeasurement_len = 0;
                  }
                  else
                  {
    pz_GnssSvMeasIndMsg->svMeasurement_len = QMI_LOC_SV_MEAS_LIST_MAX_SIZE_V02;
    pz_GnssSvMeasIndMsg->extSvMeasurement_len = pz_PdsmSvMeas->z_GnssMeas.u_NumSvs - QMI_LOC_SV_MEAS_LIST_MAX_SIZE_V02;
            }

  pz_GnssSvMeasIndMsg->svMeasurement_valid = (pz_GnssSvMeasIndMsg->svMeasurement_len != 0);
  pz_GnssSvMeasIndMsg->extSvMeasurement_valid = (pz_GnssSvMeasIndMsg->extSvMeasurement_len != 0);
  pz_GnssSvMeasIndMsg->svCarrierPhaseUncertainty_valid = FALSE;
  pz_GnssSvMeasIndMsg->extSvCarrierPhaseUncertainty_valid = FALSE;

  for (i = 0; i < pz_PdsmSvMeas->z_GnssMeas.u_NumSvs; i++)
         {
    if(i < QMI_LOC_SV_MEAS_LIST_MAX_SIZE_V02)
  {
    pz_dstSvMeas = &pz_GnssSvMeasIndMsg->svMeasurement[i];
    }
    else
    {
      pz_dstSvMeas = &pz_GnssSvMeasIndMsg->extSvMeasurement[i - QMI_LOC_SV_MEAS_LIST_MAX_SIZE_V02];
    }
    pz_SrcSVMeas = &pz_PdsmSvMeas->z_GnssMeas.z_svMeasurement[i];

    pz_dstSvMeas->gnssSvId = pz_SrcSVMeas->w_gnssSvId;
    pz_dstSvMeas->gloFrequency = pz_SrcSVMeas->u_gloFrequency;
    pz_dstSvMeas->svStatus = locPd_ConvPdsmSvStateToQmi(pz_SrcSVMeas->e_svStatus);

    //Health Status
    if (pz_SrcSVMeas->v_healthStatus_valid)
               {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_HEALTH_VALID_V02;
      pz_dstSvMeas->healthStatus = (PD_LOC_MEAS_SV_HEALTH_STATUS_HEALTHY == pz_SrcSVMeas->u_healthStatus);
               }

    pz_dstSvMeas->svInfoMask = pz_SrcSVMeas->e_svInfoMask;
    pz_dstSvMeas->validMeasStatusMask = (uint64_t)pz_SrcSVMeas->t_measurementStatusValid;
    pz_dstSvMeas->measurementStatus = pz_SrcSVMeas->t_measurementStatus;
    pz_dstSvMeas->CNo = pz_SrcSVMeas->w_CNo;
    pz_dstSvMeas->gloRfLoss = pz_SrcSVMeas->w_gloRfLoss;
    pz_dstSvMeas->measLatency = pz_SrcSVMeas->x_measLatency;
    pz_dstSvMeas->svTimeSpeed.svTimeMs = pz_SrcSVMeas->z_svTimeSpeed.q_svMs;
    pz_dstSvMeas->svTimeSpeed.svTimeSubMs = pz_SrcSVMeas->z_svTimeSpeed.f_svSubMs;
    pz_dstSvMeas->svTimeSpeed.svTimeUncMs = pz_SrcSVMeas->z_svTimeSpeed.f_svTimeUncMs;
    pz_dstSvMeas->svTimeSpeed.dopplerShift = pz_SrcSVMeas->z_svTimeSpeed.f_dopplerShift;
    pz_dstSvMeas->svTimeSpeed.dopplerShiftUnc = pz_SrcSVMeas->z_svTimeSpeed.f_dopplerShiftUnc;

    if (pz_SrcSVMeas->v_dopplerAccelValid)
    {
      pz_dstSvMeas->svTimeSpeed.dopplerAccel_valid = TRUE;
      pz_dstSvMeas->svTimeSpeed.dopplerAccel = pz_SrcSVMeas->f_dopplerAccel;
    }

    if (pz_SrcSVMeas->v_lossOfLockValid)
    {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_LOSSOFLOCK_VALID_V02;
      pz_dstSvMeas->lossOfLock = pz_SrcSVMeas->b_lossOfLock;
    }

    if (pz_SrcSVMeas->v_multipathEstValid)
    {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_MULTIPATH_EST_VALID_V02;
      pz_dstSvMeas->multipathEstimate = pz_SrcSVMeas->f_multipathEstimate;
    }

    if (pz_SrcSVMeas->v_fineSpeedValid)
    {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_FINE_SPEED_VALID_V02;
      pz_dstSvMeas->fineSpeed = pz_SrcSVMeas->f_fineSpeed;
    }

    if (pz_SrcSVMeas->v_fineSpeedUncValid)
    {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_FINE_SPEED_UNC_VALID_V02;
      pz_dstSvMeas->fineSpeedUnc = pz_SrcSVMeas->f_fineSpeedUnc;
    }

    if (pz_SrcSVMeas->v_carrierPhaseValid)
    {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_CARRIER_PHASE_VALID_V02;
      pz_dstSvMeas->carrierPhase = pz_SrcSVMeas->d_carrierPhase;
    }

    if (pz_SrcSVMeas->v_cycleSlipCountValid)
    {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_CYCLESLIP_COUNT_VALID_V02;
      pz_dstSvMeas->cycleSlipCount = pz_SrcSVMeas->u_cycleSlipCount;
    }

    if (pz_SrcSVMeas->v_svDirectionValid)
    {
      pz_dstSvMeas->validMask |= QMI_LOC_SV_SV_DIRECTION_VALID_V02;
      pz_dstSvMeas->svAzimuth = pz_SrcSVMeas->f_svAzimuth;
      pz_dstSvMeas->svElevation = pz_SrcSVMeas->f_svElevation;
    }

    /* TLV 0x1D SV Carrier Phase Uncertainty */
    if (pz_SrcSVMeas->v_carrierPhaseUncValid)
               {
      if(i < QMI_LOC_SV_MEAS_LIST_MAX_SIZE_V02)
                  {
        pz_GnssSvMeasIndMsg->svCarrierPhaseUncertainty_valid = TRUE;
        pz_GnssSvMeasIndMsg->svCarrierPhaseUncertainty[i] = pz_SrcSVMeas->f_CarrierPhaseUnc;
                  }
                  else
    {
        pz_GnssSvMeasIndMsg->extSvCarrierPhaseUncertainty_valid = TRUE;
        pz_GnssSvMeasIndMsg->extSvCarrierPhaseUncertainty[i - QMI_LOC_SV_MEAS_LIST_MAX_SIZE_V02] = pz_SrcSVMeas->f_CarrierPhaseUnc;
                  }
    }
  }

  /* if TLV 0x1D - SV Carrier Phase Unc became valid in the loop, populate its length */
  if (pz_GnssSvMeasIndMsg->svCarrierPhaseUncertainty_valid == TRUE)
   {
    pz_GnssSvMeasIndMsg->svCarrierPhaseUncertainty_len = pz_GnssSvMeasIndMsg->svMeasurement_len;
         }

  if (pz_GnssSvMeasIndMsg->extSvCarrierPhaseUncertainty_valid == TRUE)
  {
    pz_GnssSvMeasIndMsg->extSvCarrierPhaseUncertainty_len = pz_GnssSvMeasIndMsg->extSvMeasurement_len;
  }

  /* TLV 0x1C Number of Clock Resets */
  pz_GnssSvMeasIndMsg->numClockResets_valid = TRUE;
  pz_GnssSvMeasIndMsg->numClockResets = pz_PdsmSvMeas->z_GnssMeas.z_systemTime.q_NumClockResets;

  /* TLV 0x1E GNSS Signal Type */
  pz_GnssSvMeasIndMsg->gnssSignalType_valid = TRUE;

  switch (pz_PdsmSvMeas->e_GnssSignalType)
  {
    case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L1CA_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1C:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L1C_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L2C_L_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GPS_L5_Q_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GLONASS_G1_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GLONASS_G2_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E1_C_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5B_Q:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_GALILEO_E5B_Q_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B1_I_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_I:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B2A_I_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L1CA_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1S:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L1S_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L2C_L_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_QZSS_L5_Q_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_SBAS_L1_CA:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_SBAS_L1_CA_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_NAVIC_L5:
      pz_GnssSvMeasIndMsg->gnssSignalType = QMI_LOC_MASK_GNSS_SIGNAL_TYPE_NAVIC_L5_V02;
      break;
    case PD_LOC_GNSS_SIGNAL_TYPE_INVALID:
    default:
      pz_GnssSvMeasIndMsg->gnssSignalType_valid = FALSE;
      LOC_MSG_ERROR_1("locPd_HandleSvMeasReport:MultiBandSignalType %d NOT VALID", pz_PdsmSvMeas->e_GnssSignalType);
      break;
  }
#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
  LOC_MSG_HIGH_2("locPd_HandleSvMeasReport:MultiBandSignalType VALID. PdsmType %d QMIType %d",
                 pz_PdsmSvMeas->e_GnssSignalType,
                 pz_GnssSvMeasIndMsg->gnssSignalType);
#endif

  /* TLV 0x1F Jammer Indicator */
  pz_GnssSvMeasIndMsg->jammerIndicator_valid = TRUE;
  pz_GnssSvMeasIndMsg->jammerIndicator.agcMetricDb = pz_PdsmSvMeas->z_JammerIndicator.l_agcMetricDb;
  pz_GnssSvMeasIndMsg->jammerIndicator.bpMetricDb = pz_PdsmSvMeas->z_JammerIndicator.l_bpMetricDb;

  /* TLV 0x20 GPS L1-L2c Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GpsL1L2cTimeBias.u_validMask)
      {
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias.validMask = pz_PdsmSvMeas->z_GpsL1L2cTimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias.timeBias = pz_PdsmSvMeas->z_GpsL1L2cTimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GpsL1L2cTimeBias.f_sys1Sys2TBUnc;
}

  /* TLV 0x21 GPS L1-L5 Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GpsL1L5TimeBias.u_validMask)
{
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias.validMask = pz_PdsmSvMeas->z_GpsL1L5TimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias.timeBias = pz_PdsmSvMeas->z_GpsL1L5TimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GpsL1L5TimeBias.f_sys1Sys2TBUnc;
      }

  /* TLV 0x22 GLO G1-G2 Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GloG1G2TimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias.validMask = pz_PdsmSvMeas->z_GloG1G2TimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias.timeBias = pz_PdsmSvMeas->z_GloG1G2TimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GloG1G2TimeBias.f_sys1Sys2TBUnc;
      }

  /* TLV 0x20 GPS L1-L2c Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GpsL1L2cTimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias.validMask = pz_PdsmSvMeas->z_GpsL1L2cTimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias.timeBias = pz_PdsmSvMeas->z_GpsL1L2cTimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GpsL1L2cTimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GpsL1L2cTimeBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x21 GPS L1-L5 Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GpsL1L5TimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias.validMask = pz_PdsmSvMeas->z_GpsL1L5TimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias.timeBias = pz_PdsmSvMeas->z_GpsL1L5TimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GpsL1L5TimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GpsL1L5TimeBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x22 GLO G1-G2 Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GloG1G2TimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias.validMask = pz_PdsmSvMeas->z_GloG1G2TimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias.timeBias = pz_PdsmSvMeas->z_GloG1G2TimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GloG1G2TimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GloG1G2TimeBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x23 BDS B1I-B2A Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_BdsB1iB2aTimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->BdsB1iB2aTimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->BdsB1iB2aTimeBias.validMask = pz_PdsmSvMeas->z_BdsB1iB2aTimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->BdsB1iB2aTimeBias.timeBias = pz_PdsmSvMeas->z_BdsB1iB2aTimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->BdsB1iB2aTimeBias.timeBiasUnc = pz_PdsmSvMeas->z_BdsB1iB2aTimeBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x24 BDS B1I-B1C Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_BdsB1iB1cTimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->BdsB1iB1cTimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->BdsB1iB1cTimeBias.validMask = pz_PdsmSvMeas->z_BdsB1iB1cTimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->BdsB1iB1cTimeBias.timeBias = pz_PdsmSvMeas->z_BdsB1iB1cTimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->BdsB1iB1cTimeBias.timeBiasUnc = pz_PdsmSvMeas->z_BdsB1iB1cTimeBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x25 GAL E1-E5A Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GalE1E5aTimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->GalE1E5aTimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GalE1E5aTimeBias.validMask = pz_PdsmSvMeas->z_GalE1E5aTimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GalE1E5aTimeBias.timeBias = pz_PdsmSvMeas->z_GalE1E5aTimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GalE1E5aTimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GalE1E5aTimeBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x26 GAL E1-E5B Intra-System Time Bias */
  if (pz_PdsmSvMeas->z_GalE1E5bTimeBias.u_validMask)
  {
    pz_GnssSvMeasIndMsg->GalE1E5bTimeBias_valid = TRUE;
    pz_GnssSvMeasIndMsg->GalE1E5bTimeBias.validMask = pz_PdsmSvMeas->z_GalE1E5bTimeBias.u_validMask;
    pz_GnssSvMeasIndMsg->GalE1E5bTimeBias.timeBias = pz_PdsmSvMeas->z_GalE1E5bTimeBias.f_sys1Sys2TB;
    pz_GnssSvMeasIndMsg->GalE1E5bTimeBias.timeBiasUnc = pz_PdsmSvMeas->z_GalE1E5bTimeBias.f_sys1Sys2TBUnc;
  }

  /* TLV 0x28 Measurement Code Type */
  pz_GnssSvMeasIndMsg->measurementCodeType_valid = TRUE;
  switch (pz_PdsmSvMeas->e_GnssMeasCodeType)
  {
    case PDSM_MEASUREMENT_CODE_TYPE_A:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_A_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_B:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_B_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_C:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_C_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_I:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_I_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_L:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_L_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_M:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_M_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_P:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_P_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_Q:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_Q_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_S:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_S_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_W:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_W_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_X:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_X_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_Y:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_Y_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_Z:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_Z_V02;
      break;
    case PDSM_MEASUREMENT_CODE_TYPE_CODELESS:
      pz_GnssSvMeasIndMsg->measurementCodeType = eQMI_LOC_GNSS_CODE_TYPE_N_V02;
      break;

      /* 
       * If code type is set to any other code than those above, we need to 
       * set the field to pz_GnssSvMeasIndMsg->measurementCodeType to 
       * eQMI_LOC_GNSS_CODE_TYPE_OTHER 
       * and fill out the TLV otherCodeTypeName.
       *
       */
    default:
      pz_GnssSvMeasIndMsg->measurementCodeType_valid = FALSE;
      break;
  }

  /* TLV 0x27 NHz Measurement Indicator */
  pz_GnssSvMeasIndMsg->nHzMeasurement_valid = TRUE;
  pz_GnssSvMeasIndMsg->nHzMeasurement = pz_PdsmSvMeas->u_NhzMeasurement;


  //Depending on 1hz or Nhz, different event needs to be fired
  if (0 == pz_GnssSvMeasIndMsg->nHzMeasurement)
  {
  (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                              LOC_CMD_TYPE_NOTIFY_CLIENT,
                              LOC_EVENT_GNSS_MEASUREMENT_REPORT,
                              (void *)pz_GnssSvMeasIndMsg);
}
   else
   {
    (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                LOC_CMD_TYPE_NOTIFY_CLIENT,
                                LOC_EVENT_GNSS_NHZ_MEASUREMENT_REPORT,
                                (void *)pz_GnssSvMeasIndMsg);
   }
}

static void locPd_HandleNmeaPosReport
(
   const pdsm_ext_status_info_s_type *pz_ExtStatusInfo
   )
{
  qmiLocEventNmeaIndMsgT_v02   *pz_NmeaIndMsg = NULL;

  if (NULL == pz_ExtStatusInfo)
  {
    LOC_MSG_ERROR("locPd_HandleNmeaPosReport NULL status info ptr", 0, 0, 0);
    return;
  }

  pz_NmeaIndMsg = (qmiLocEventNmeaIndMsgT_v02 *)loc_calloc(sizeof(*pz_NmeaIndMsg));
  if (NULL != pz_NmeaIndMsg)
  {
    if (PDAPI_NMEA_VAR_LEN_DEBUG != pz_ExtStatusInfo->ext_status_info.ext_nmea_var_len_pos_report_type.nmea_type)
    {
      LOC_MSG_MED("locPd_HandleNmeaPosReport %d length nmea, nmea_type = 0x%x",
                  pz_ExtStatusInfo->ext_status_info.ext_nmea_pos_report_type.nmea_length,
                  pz_ExtStatusInfo->ext_status_info.ext_nmea_pos_report_type.nmea_type, 0);
      memscpy(pz_NmeaIndMsg->nmea,
              sizeof(pz_NmeaIndMsg->nmea),
              pz_ExtStatusInfo->ext_status_info.ext_nmea_pos_report_type.nmea_data,
              sizeof(pz_ExtStatusInfo->ext_status_info.ext_nmea_pos_report_type.nmea_data));
    }
    else
    {
      LOC_MSG_MED("locPd_HandleNmeaPosReport %d length vary nmea, nmea_type = 0x%x",
                  pz_ExtStatusInfo->ext_status_info.ext_nmea_var_len_pos_report_type.q_Length,
                  pz_ExtStatusInfo->ext_status_info.ext_nmea_var_len_pos_report_type.nmea_type, 0);
      pz_NmeaIndMsg->expandedNmea_valid = TRUE;
      memscpy(pz_NmeaIndMsg->expandedNmea,
              sizeof(pz_NmeaIndMsg->expandedNmea),
              pz_ExtStatusInfo->ext_status_info.ext_nmea_var_len_pos_report_type.pu_NmeaBuff,
              pz_ExtStatusInfo->ext_status_info.ext_nmea_var_len_pos_report_type.q_Length);
    }

    (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                LOC_CMD_TYPE_NOTIFY_CLIENT,
                                LOC_EVENT_NMEA_POSITION_REPORT,
                                (void *)pz_NmeaIndMsg);
  }
  else
  {
    LOC_MSG_ERROR("locPd_HandleNmeaPosReport, loc_calloc failed ", 0, 0, 0);
  }
}

void locPd_HandleLocationNotification
(
   const pdsm_ext_status_info_s_type *pz_ExtStatusInfo
   )
{
  loc_client_handle_type  l_MasterClientHandle = loc_middleware_data.z_locClnt_Data.l_MasterClientHandle;
  qmiLocLocationRequestNotificationIndMsgT_v02 *pz_IndMsg = NULL;
  const pdsm_pd_location_notification_info_s_type *pz_LocNotificationInfo = NULL;

  if (NULL == pz_ExtStatusInfo)
  {
    LOC_MSG_ERROR("locPd_HandleLocationNotification NULL status info ptr", 0, 0, 0);
    return;
  }
  pz_LocNotificationInfo = &(pz_ExtStatusInfo->ext_status_info.z_ext_location_notification_info);

  if (LOC_CLIENT_HANDLE_INVALID == l_MasterClientHandle)
  {
    LOC_MSG_ERROR("locPd_HandleLocationNotification no master client", 0, 0, 0);
    return;
  }

  pz_IndMsg = (qmiLocLocationRequestNotificationIndMsgT_v02 *)loc_calloc(sizeof(*pz_IndMsg));
  if (NULL != pz_IndMsg)
  {
    switch (pz_LocNotificationInfo->e_protocol_stack)
    {
      case PDSM_PD_LOC_PRTL_CTRL_PLANE:
        pz_IndMsg->protocolStack = eQMI_LOC_CTRL_PLANE_V02;
        break;
      case PDSM_PD_LOC_PRTL_SUPL:
        pz_IndMsg->protocolStack = eQMI_LOC_SUPL_V02;
        break;
      case PDSM_PD_LOC_PRTL_IMS:
        pz_IndMsg->protocolStack = eQMI_LOC_IMS_V02;
        break;
      case PDSM_PD_LOC_PRTL_SIM:
        pz_IndMsg->protocolStack = eQMI_LOC_SIM_V02;
        break;
      case PDSM_PD_LOC_PRTL_MDT:
        pz_IndMsg->protocolStack = eQMI_LOC_MDT_V02;
        break;
      case PDSM_PD_LOC_PRTL_TLOC:
        pz_IndMsg->protocolStack = eQMI_LOC_TLOC_V02;
        break;
      case PDSM_PD_LOC_PRTL_OTHER:
      default:
        pz_IndMsg->protocolStack = eQMI_LOC_OTHER_V02;
        break;
    }

    switch (pz_LocNotificationInfo->e_location_requestor)
    {
      case PDSM_PD_LOC_REQUESTOR_CARRIER:
        pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_CARRIER_V02;
        break;
      case PDSM_PD_LOC_REQUESTOR_OEM:
        pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_OEM_V02;
        break;
      case PDSM_PD_LOC_REQUESTOR_MODEM_CHIPSET_VENDOR:
        pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_MODEM_CHIPSET_VENDOR_V02;
        break;
      case PDSM_PD_LOC_REQUESTOR_GNSS_CHIPSET_VENDOR:
        pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_GNSS_CHIPSET_VENDOR_V02;
        break;
      case PDSM_PD_LOC_REQUESTOR_OTHER_CHIPSET_VENDOR:
        pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_OTHER_CHIPSET_VENDOR_V02;
        break;
      case PDSM_PD_LOC_REQUESTOR_AUTO_CLIENT:
        pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_AUTOMOBILE_CLIENT_V02;
        break;
      case PDSM_PD_LOC_REQUESTOR_OTHER:
      default:
        pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_OTHER_V02;
        break;
    }

    GNSS_STRLCPY(pz_IndMsg->requestorId,
                 pz_LocNotificationInfo->loc_requestor_id,
                 QMI_LOC_MAX_REQUESTOR_ID_STRING_LENGTH_V02 + 1);

    switch (pz_LocNotificationInfo->e_location_response)
    {
      case PDSM_PD_LOC_RESPONSE_REJECTED:
        pz_IndMsg->responseType = eQMI_LOC_REJECTED_V02;
        break;
      case PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED:
        pz_IndMsg->responseType = eQMI_LOC_ACCEPTED_NO_LOCATION_PROVIDED_V02;
        break;
      case PDSM_PD_LOC_RESPONSE_ACCEPTED_LOC_PROVIDED:
      default:
        pz_IndMsg->responseType = eQMI_LOC_ACCEPTED_LOCATION_PROVIDED_V02;
        break;
    }

    switch (pz_LocNotificationInfo->e_location_info)
    {
      case PDSM_PD_LOC_INFO_GNSS_MEAS:
        pz_IndMsg->responseProtocol = QMI_LOC_GNSS_MEASUREMENT_V02;
        break;
      case PDSM_PD_LOC_INFO_OTDOA_MEAS:
        pz_IndMsg->responseProtocol = QMI_LOC_OTDOA_MEASUREMENT_V02;
        break;
      case PDSM_PD_LOC_INFO_DBH_DATA:
        pz_IndMsg->responseProtocol = QMI_LOC_DBH_DATA_V02;
        break;
      case PDSM_PD_LOC_INFO_SRN:
        pz_IndMsg->responseProtocol = QMI_LOC_SRN_V02;
        break;
      case PDSM_PD_LOC_INFO_ECID:
        pz_IndMsg->responseProtocol = QMI_LOC_ECID_V02;
        break;
      case PDSM_PD_LOC_INFO_WLAN:
        pz_IndMsg->responseProtocol = QMI_LOC_WLAN_V02;
        break;
      case PDSM_PD_LOC_INFO_GNSS_POSITION:
      default:
        pz_IndMsg->responseProtocol = QMI_LOC_GNSS_POSITION_V02;
        break;
    }

    pz_IndMsg->inEmergencyMode = pz_LocNotificationInfo->v_e911_state;
    pz_IndMsg->isCachedLocation = pz_LocNotificationInfo->v_cached_location;

    if (pz_LocNotificationInfo->v_client_id_valid)
    {
      pz_IndMsg->clientStrId_valid = TRUE;
      GNSS_STRLCPY(pz_IndMsg->clientStrId,
                   pz_LocNotificationInfo->client_id,
                   QMI_LOC_MAX_CLIENT_ID_STRING_LENGTH_V02 + 1);
    }

    locMW_SendQmiIndicationProxy(l_MasterClientHandle,
                                 QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND_V02,
                                 pz_IndMsg,
                                 LOC_API_SUCCESS);
  }
  else
  {
    LOC_MSG_ERROR("locPd_HandleLocationNotification loc_calloc failed ", 0, 0, 0);
  }
  return;
}

// Process extended measurement report with SV info
// TBD: process intermediate position report
// TBD: skip intermediate report after a good report has been received for this client
//Runs in SM Task Context
static void locPd_ExtEventCb
(
   void *p_DataBlock,
   pdsm_ext_status_event_type         q_ExtStatusCmd,
   const pdsm_ext_status_info_s_type *pz_ExtStatusInfo
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  if (NULL == pz_ExtStatusInfo)
  {
    LOC_MSG_ERROR("locPd_ExtEventCb NULL status info ptr", 0, 0, 0);
    return;
  }

  LOC_MSG_HIGH("locPd_ExtEventCb cmd=0x%x client_id=%d",
    q_ExtStatusCmd, pz_ExtStatusInfo->client_id, 0);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  if ((PDSM_EXT_STATUS_EVENT_NMEA_POS_REPORT == q_ExtStatusCmd) &&
      ((PDAPI_NMEA_DEBUG == pz_ExtStatusInfo->ext_status_info.ext_nmea_pos_report_type.nmea_type) ||
       (PDAPI_NMEA_VAR_LEN_DEBUG == pz_ExtStatusInfo->ext_status_info.ext_nmea_var_len_pos_report_type.nmea_type)))
  {
    LOC_MSG_HIGH("locPd_ExtEventCb: Receive Debug NMEA", 0, 0, 0);
    locPd_HandleNmeaPosReport(pz_ExtStatusInfo);
  }
  // Check that the client id matches the client id registered for the service,
  // Otherwise we may be getting events associated with another client
  else if (pz_ExtStatusInfo->client_id == loc_middleware_data.pdapi_client_id)
  {
    if (PDSM_EXT_STATUS_EVENT_MEASUREMENT == q_ExtStatusCmd)
    {
      const pdsm_pd_ext_meas_s_type *pMeasurementReport = &pz_ExtStatusInfo->ext_status_info.ext_meas_report_type;
      if (PDSM_MEAS_PPM_RPT_VALID & (pMeasurementReport->flags))
      {
        // only PPM information is valid in this report, everything else should be ignored
        // since we do not support PPM info now, there is no need to handle this event at all
        LOC_MSG_HIGH("locPd_ExtEventCb ignoring empty measurement report (%d)", pMeasurementReport->flags, 0, 0);
      }
      else
      {
        qmiLocEventGnssSvInfoIndMsgT_v02 *pz_GnssSvInfoIndMsg = NULL;
        /* This is only Sv info not full measurement*/
        memscpy(&(pz_LocPdData->ext_status_info_copy), sizeof(pz_LocPdData->ext_status_info_copy),
                pz_ExtStatusInfo, sizeof(*pz_ExtStatusInfo));

        pz_GnssSvInfoIndMsg = (qmiLocEventGnssSvInfoIndMsgT_v02 *)loc_calloc(sizeof(*pz_GnssSvInfoIndMsg));

        if (NULL != pz_GnssSvInfoIndMsg)
        {
          locPd_GetSvReport(pz_GnssSvInfoIndMsg);
          (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                      LOC_CMD_TYPE_NOTIFY_CLIENT,
                                      LOC_EVENT_SATELLITE_REPORT,
                                      (void *)pz_GnssSvInfoIndMsg);
        }
        else
        {
          LOC_MSG_ERROR("locPd_ExtEventCb: SV info not sent, loc_calloc failed ", 0, 0, 0);
        }
      }
    }

    if (PDSM_EXT_STATUS_EVENT_POS_REPORT == q_ExtStatusCmd)
    {
      memscpy(&pz_LocPdData->interm_pos_info, sizeof(pz_LocPdData->interm_pos_info),
              pz_ExtStatusInfo,
              sizeof(*pz_ExtStatusInfo));
      pz_LocPdData->new_interm_pos_info_received = TRUE;
      (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                  LOC_CMD_TYPE_REPORT_POSITION,
                                  0,
                                  NULL);
    }

    if (PDSM_EXT_STATUS_EVENT_NMEA_POS_REPORT == q_ExtStatusCmd)
    {
      LOC_MSG_MED("locPd_ExtEventCb: Receive Normal NMEA", 0, 0, 0);
      locPd_HandleNmeaPosReport(pz_ExtStatusInfo);
    }

    if ((PDSM_EXT_STATUS_EVENT_BEST_AVAIL_POS_REPORT == q_ExtStatusCmd) && pz_LocPdData->z_GetBestAvailPos.v_InProgress)
    {
      uint32 q_Status = LOC_API_GENERAL_FAILURE;
      qmiLocGetBestAvailablePositionIndMsgT_v02 *pz_BestAvailPosInd =
         (qmiLocGetBestAvailablePositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_BestAvailPosInd));

      LOC_MSG_MED("locPd_ExtEventCb: Received best available position", 0, 0, 0);

      if (NULL != pz_BestAvailPosInd)
      {
        if (convertLocEngPosToBapPos(&(pz_ExtStatusInfo->ext_status_info.ext_pos_report_qmi_type),
                                     pz_BestAvailPosInd))
        {
          q_Status = LOC_API_SUCCESS;
          
        }

        if(pz_BestAvailPosInd->status != eQMI_LOC_SUCCESS_V02 )
        {
          q_Status = LOC_API_GENERAL_FAILURE;
        }
        
        /* copy the transaction id which was saved in the incoming call */
        pz_BestAvailPosInd->transactionId_valid = 1;
        pz_BestAvailPosInd->transactionId       = pz_LocPdData->z_GetBestAvailPos.q_TxnId;

        /* Send IPC message to LocMW task to handle QMI_LOC indication */
        locMW_SendQmiIndicationProxy(pz_LocPdData->z_GetBestAvailPos.l_ClientHandle,
                                     QMI_LOC_GET_BEST_AVAILABLE_POSITION_IND_V02,
                                     pz_BestAvailPosInd,
                                     q_Status);
      }
      else
      {
        LOC_MSG_ERROR("locPd_ExtEventCb: Failed to allocate memory for QMI_LOC Indication", 0, 0, 0);
      }

      pz_LocPdData->z_GetBestAvailPos.v_InProgress = FALSE;
    }

    if ((PDSM_EXT_STATUS_EVENT_AVAIL_WWAN_POS_REPORT == q_ExtStatusCmd) && pz_LocPdData->z_GetAvailWwanPos.v_InProgress)
    {
      uint32 q_Status = LOC_API_GENERAL_FAILURE;
      qmiLocGetAvailWwanPositionIndMsgT_v02 *pz_AvailWwanPosInd =
         (qmiLocGetAvailWwanPositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_AvailWwanPosInd));

      LOC_MSG_MED("locPd_ExtEventCb: Received available WWAN position", 0, 0, 0);

      if (NULL != pz_AvailWwanPosInd)
      {
        if (convertLocEngPosToAvailWwanPos(&(pz_ExtStatusInfo->ext_status_info.ext_pos_report_qmi_type),
                                           pz_AvailWwanPosInd))
        {
          q_Status = LOC_API_SUCCESS;
        }

        if(pz_AvailWwanPosInd->status != eQMI_LOC_SUCCESS_V02 )
        {
          q_Status = LOC_API_GENERAL_FAILURE;
        }

        /* copy the transaction id which was saved in the incoming call */
        pz_AvailWwanPosInd->transactionId_valid = 1;
        pz_AvailWwanPosInd->transactionId       = pz_LocPdData->z_GetAvailWwanPos.q_TxnId;

        /* Send IPC message to LocMW task to handle QMI_LOC indication */
        locMW_SendQmiIndicationProxy(pz_LocPdData->z_GetAvailWwanPos.l_ClientHandle,
                                     QMI_LOC_GET_AVAILABLE_WWAN_POSITION_IND_V02,
                                     pz_AvailWwanPosInd,
                                     q_Status);
      }
      else
      {
        LOC_MSG_ERROR("locPd_ExtEventCb: Failed to allocate memory for QMI_LOC Indication", 0, 0, 0);
      }

      pz_LocPdData->z_GetAvailWwanPos.v_InProgress = FALSE;
    }

    if ((PDSM_EXT_STATUS_EVENT_AVAIL_POS_REPORT == q_ExtStatusCmd) &&
        pz_LocPdData->z_GetSecureAvailPos.v_InProgress)
    {
      qmiLocSessionStatusEnumT_v02  e_SessionStatus = eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02;
      uint32                        q_RetVal = LOC_API_GENERAL_FAILURE;
      qmiLocGetAvailablePositionIndParamMsgT_v02 *pz_AvailPosInd =
         (qmiLocGetAvailablePositionIndParamMsgT_v02 *)loc_calloc(sizeof(*pz_AvailPosInd));
      qmiLocSecureGetAvailablePositionIndMsgT_v02 *pz_SecureGetAvailPosInd =
         (qmiLocSecureGetAvailablePositionIndMsgT_v02 *)loc_calloc(sizeof(*pz_SecureGetAvailPosInd));

      LOC_MSG_MED("locPd_ExtEventCb: Received available position estimate", 0, 0, 0);

      if (NULL != pz_AvailPosInd &&
          NULL != pz_SecureGetAvailPosInd)
      {
        if (convertLocEngPosToAvailablePosition(&(pz_ExtStatusInfo->ext_status_info.ext_pos_report_qmi_type), pz_AvailPosInd))
        {
          e_SessionStatus = eQMI_LOC_SESS_STATUS_SUCCESS_V02;
        }

        /* copy the transaction id which was saved in the incoming call */
        pz_AvailPosInd->transactionId_valid = 1;
        pz_AvailPosInd->transactionId       = pz_LocPdData->z_GetSecureAvailPos.q_PosTxnID;

        pz_AvailPosInd->nonce_valid = pz_LocPdData->z_GetSecureAvailPos.v_PosIsNonceValid;

        if (pz_LocPdData->z_GetSecureAvailPos.v_PosIsNonceValid)
        {
          pz_AvailPosInd->nonce = pz_LocPdData->z_GetSecureAvailPos.t_PosNonce;
        }

        pz_AvailPosInd->e_PosDataMode = pz_LocPdData->z_GetSecureAvailPos.e_PosDataMode;
        LOC_MSG_MED("locPd_ExtEventCb: Transaction ID %d Nonce %x Data Mode %d",
                    pz_AvailPosInd->transactionId, pz_AvailPosInd->nonce, pz_AvailPosInd->e_PosDataMode);
        LOC_MSG_MED("locPd_ExtEventCb: e_SessionStatus %d HorizontalConfidence_valid %d  %d",
                    e_SessionStatus, pz_AvailPosInd->horCircularConfidence_valid, pz_AvailPosInd->horCircularConfidence);

        if (TRUE == locPd_EncodeGetAvailableGnssPos(
              pz_AvailPosInd,
              pz_SecureGetAvailPosInd,
              e_SessionStatus))
        {
          q_RetVal = LOC_API_SUCCESS;
        }

        /* Send IPC message to LocMW task to handle QMI_LOC indication */
        locMW_SendQmiIndicationProxy(pz_LocPdData->z_GetSecureAvailPos.l_ClientHandle,
                                     QMI_LOC_SECURE_GET_AVAILABLE_POSITION_IND_V02,
                                     pz_SecureGetAvailPosInd,
                                     q_RetVal);
        loc_free(pz_AvailPosInd);
        /* pz_SecureGetAvailPosInd will be freed by the receiver */
      }
      else
      {
        LOC_MSG_ERROR("locPd_ExtEventCb: Failed to allocate memory for QMI_LOC Indication", 0, 0, 0);
      }

      pz_LocPdData->z_GetSecureAvailPos.v_InProgress = FALSE;
    }

    if (PDSM_EXT_STATUS_EVENT_GNSS_MEASUREMENT == q_ExtStatusCmd)
    {
        LOC_MSG_HIGH_4("locPd_ExtEventCb PDSM_EXT_STATUS_EVENT_GNSS_MEASUREMENT PdSig %d Seq %d of %d. NHz %d",
                   pz_ExtStatusInfo->ext_status_info.ext_gnss_meas_report.e_GnssSignalType,
                   pz_ExtStatusInfo->ext_status_info.ext_gnss_meas_report.u_seqNum,
                       pz_ExtStatusInfo->ext_status_info.ext_gnss_meas_report.u_maxMessageNum,
                       pz_ExtStatusInfo->ext_status_info.ext_gnss_meas_report.u_NhzMeasurement);
      locPd_HandleSvMeasReport(pz_ExtStatusInfo);
    }

    if (PDSM_EXT_STATUS_EVENT_SV_POLY == q_ExtStatusCmd)
    {
      LOC_MSG_HIGH("locPd_ExtEventCb PDSM_EXT_STATUS_EVENT_SV_POLY", 0, 0, 0);
      locPd_HandleSvPolyReport(pz_ExtStatusInfo);
    }

    if (PDSM_EXT_STATUS_EVENT_UNPROPAGATED_POS_REPORT == q_ExtStatusCmd)
    {
      LOC_MSG_HIGH("loc_pd_ext_event_cb PDSM_EXT_STATUS_EVENT_UNPROPAGATED_POS_REPORT", 0, 0, 0);
      memscpy(&pz_LocPdData->unpropagated_pos_info, sizeof(pz_LocPdData->unpropagated_pos_info),
              pz_ExtStatusInfo, sizeof(*pz_ExtStatusInfo));
      pz_LocPdData->new_unpropagated_pos_info_received = TRUE;
      // send error report, as an empty location report, to client
      (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                  LOC_CMD_TYPE_REPORT_POSITION,
                                  0,
                                  NULL);
    }
    }

  /* No need to filter this Position Injection Request, As this can be an unsolicited request sent to 
    any client asking it to inject coarse Position */
  if (PDSM_EXT_STATUS_EVENT_POS_INJECT_REQ == q_ExtStatusCmd)
  {
    qmiLocEventInjectPositionReqIndMsgT_v02 *pz_InjectPositionReqInd = NULL;
    const pdsm_pd_ext_pos_inj_req_s_type  *pPosInjReq =
       &pz_ExtStatusInfo->ext_status_info.ext_pos_inj_req_type;
    LOC_MSG_MED("locPd_ExtEventCb POS_INJECT_REQ: flag 0x%X, lat: %d, lon: %d", pPosInjReq->flags, pPosInjReq->lat, pPosInjReq->lon);
    LOC_MSG_MED("locPd_ExtEventCb POS_INJECT_REQ: Punc %u, week: %u, ms: %u", pPosInjReq->pos_unc, pPosInjReq->timestamp_gps_week, pPosInjReq->timestamp_tow_ms);

    pz_InjectPositionReqInd = (qmiLocEventInjectPositionReqIndMsgT_v02 *)loc_calloc(sizeof(*pz_InjectPositionReqInd));
    if (NULL != pz_InjectPositionReqInd)
    {
      if (PDSM_PD_EXT_POS_INJ_LAT_LONG_VALID & pPosInjReq->flags)
      {
        pz_InjectPositionReqInd->latitude =
           pPosInjReq->lat * C_RAD_TO_DEG;
        pz_InjectPositionReqInd->longitude =
           pPosInjReq->lon * C_RAD_TO_DEG;
      }
      if (PDSM_PD_EXT_POS_INJ_POS_UNC_VALID & pPosInjReq->flags)
      {
        pz_InjectPositionReqInd->horUncCircular = pPosInjReq->pos_unc;
      }
      if (PDSM_PD_EXT_POS_INJ_TIME_STAMP_VALID & pPosInjReq->flags)
      {
        //TODO, find the convert function
        pz_InjectPositionReqInd->timestampUtc = pPosInjReq->timestamp_tow_ms;
      }

      (void)locMW_SendLocCmdProxy(
         LOC_HANDLE_REGISTERED_CLIENTS,
         LOC_CMD_TYPE_NOTIFY_CLIENT,
         LOC_EVENT_ASSISTANCE_DATA_REQUEST,
         (void *)pz_InjectPositionReqInd);
    }
    else
    {
      LOC_MSG_ERROR("locPd_ExtEventCb, PDSM_EXT_STATUS_EVENT_POS_INJECT_REQ loc_calloc returns NULL", 0, 0, 0);
    }
  }

  if (PDSM_EXT_STATUS_EVENT_REPORT_ME_METRICS == q_ExtStatusCmd)
  {
    qmiLocGetBandMeasurementMetricsIndMsgT_v02 *pz_ReportMEMetricsInd = NULL;
    pz_ReportMEMetricsInd = (qmiLocGetBandMeasurementMetricsIndMsgT_v02 *)loc_calloc(sizeof(*pz_ReportMEMetricsInd));

    if (NULL != pz_ReportMEMetricsInd)
    {
      pz_ReportMEMetricsInd->systemTick = pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.t_SystemTick;
      pz_ReportMEMetricsInd->band1PgaGainDb = pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.l_Band1PgaGainDb;
      LOC_MSG_MED("locPd_ExtEventCb, PDSM_EXT_STATUS_EVENT_REPORT_ME_METRICS systemTick = %llu band1PgaGainDb = %d",
                  pz_ReportMEMetricsInd->systemTick, pz_ReportMEMetricsInd->band1PgaGainDb, 0);

      if (pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.u_IsBand5PgaGainDbValid)
      {
        pz_ReportMEMetricsInd->band5PgaGainDb = pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.l_Band5PgaGainDb;
        pz_ReportMEMetricsInd->band5PgaGainDb_valid = TRUE;
      }
      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_GPS_L1CA))
      {
        pz_ReportMEMetricsInd->gpsL1caRfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L1CA].w_BpAmpI;
        pz_ReportMEMetricsInd->gpsL1caRfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L1CA].w_BpAmpQ;
        pz_ReportMEMetricsInd->gpsL1caRfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L1CA].l_JammerPwrDb;
        pz_ReportMEMetricsInd->gpsL1caRfStats_valid = TRUE;
        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_GPS_L1CA metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->gpsL1caRfStats.bpAmpI,
                    pz_ReportMEMetricsInd->gpsL1caRfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->gpsL1caRfStats.jammerPwrDb);
      }
      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_GPS_L2C))
      {
        pz_ReportMEMetricsInd->gpsL2cRfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L2C].w_BpAmpI;
        pz_ReportMEMetricsInd->gpsL2cRfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L2C].w_BpAmpQ;
        pz_ReportMEMetricsInd->gpsL2cRfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L2C].l_JammerPwrDb;
        pz_ReportMEMetricsInd->gpsL2cRfStats_valid = TRUE;
        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_GPS_L2C metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->gpsL2cRfStats.bpAmpI,
                    pz_ReportMEMetricsInd->gpsL2cRfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->gpsL2cRfStats.jammerPwrDb);

      }
      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_GPS_L5))
      {
        pz_ReportMEMetricsInd->gpsL5RfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L5].w_BpAmpI;
        pz_ReportMEMetricsInd->gpsL5RfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L5].w_BpAmpQ;
        pz_ReportMEMetricsInd->gpsL5RfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GPS_L5].l_JammerPwrDb;
        pz_ReportMEMetricsInd->gpsL5RfStats_valid = TRUE;

        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_GPS_L5 metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->gpsL5RfStats.bpAmpI,
                    pz_ReportMEMetricsInd->gpsL5RfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->gpsL5RfStats.jammerPwrDb);

      }
      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_GLO_G1))
      {
        pz_ReportMEMetricsInd->gloG1RfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GLO_G1].w_BpAmpI;
        pz_ReportMEMetricsInd->gloG1RfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GLO_G1].w_BpAmpQ;
        pz_ReportMEMetricsInd->gloG1RfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GLO_G1].l_JammerPwrDb;
        pz_ReportMEMetricsInd->gloG1RfStats_valid = TRUE;
        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_GLO_G1 metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->gloG1RfStats.bpAmpI,
                    pz_ReportMEMetricsInd->gloG1RfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->gloG1RfStats.jammerPwrDb);

      }

      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_GLO_G2))
      {
        pz_ReportMEMetricsInd->gloG2RfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GLO_G2].w_BpAmpI;
        pz_ReportMEMetricsInd->gloG2RfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GLO_G2].w_BpAmpQ;
        pz_ReportMEMetricsInd->gloG2RfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GLO_G2].l_JammerPwrDb;
        pz_ReportMEMetricsInd->gloG2RfStats_valid = TRUE;
        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_GLO_G2 metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->gloG2RfStats.bpAmpI,
                    pz_ReportMEMetricsInd->gloG2RfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->gloG2RfStats.jammerPwrDb);

      }

      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_BDS_B1))
      {
        pz_ReportMEMetricsInd->bdsB1RfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_BDS_B1].w_BpAmpI;
        pz_ReportMEMetricsInd->bdsB1RfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_BDS_B1].w_BpAmpQ;
        pz_ReportMEMetricsInd->bdsB1RfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_BDS_B1].l_JammerPwrDb;
        pz_ReportMEMetricsInd->bdsB1RfStats_valid = TRUE;
        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_BDS_B1 metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->bdsB1RfStats.bpAmpI,
                    pz_ReportMEMetricsInd->bdsB1RfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->bdsB1RfStats.jammerPwrDb);
      }
      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_BDS_B2))
      {
        pz_ReportMEMetricsInd->bdsB2RfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_BDS_B2].w_BpAmpI;
        pz_ReportMEMetricsInd->bdsB2RfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_BDS_B2].w_BpAmpQ;
        pz_ReportMEMetricsInd->bdsB2RfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_BDS_B2].l_JammerPwrDb;
        pz_ReportMEMetricsInd->bdsB2RfStats_valid = TRUE;

        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_BDS_B2 metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->bdsB2RfStats.bpAmpI,
                    pz_ReportMEMetricsInd->bdsB2RfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->bdsB2RfStats.jammerPwrDb);
      }

      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_GAL_E1))
      {
        pz_ReportMEMetricsInd->galE1RfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GAL_E1].w_BpAmpI;
        pz_ReportMEMetricsInd->galE1RfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GAL_E1].w_BpAmpQ;
        pz_ReportMEMetricsInd->galE1RfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GAL_E1].l_JammerPwrDb;
        pz_ReportMEMetricsInd->galE1RfStats_valid = TRUE;
        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_GAL_E1 metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->galE1RfStats.bpAmpI,
                    pz_ReportMEMetricsInd->galE1RfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->galE1RfStats.jammerPwrDb);

      }
      if ((pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.w_GNSSenabledBitmask) & (1UL << PDSM_GNSS_SIG_TYPE_GAL_E5A))
      {
        pz_ReportMEMetricsInd->galE5ARfStats.bpAmpI =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GAL_E5A].w_BpAmpI;
        pz_ReportMEMetricsInd->galE5ARfStats.bpAmpQ =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GAL_E5A].w_BpAmpQ;
        pz_ReportMEMetricsInd->galE5ARfStats.jammerPwrDb =
           pz_ExtStatusInfo->ext_status_info.ext_report_ME_metrics.z_MERfStats.z_PerGnssRfStats[PDSM_GNSS_SIG_TYPE_GAL_E5A].l_JammerPwrDb;
        pz_ReportMEMetricsInd->galE5ARfStats_valid = TRUE;
        LOC_MSG_MED("locPd_ExtEventCb, PDSM_GNSS_SIG_TYPE_GAL_E5A metrices bpAmpI = %d, bpAmpQ = %d, jammerPwrDb = %d",
                    pz_ReportMEMetricsInd->galE5ARfStats.bpAmpI,
                    pz_ReportMEMetricsInd->galE5ARfStats.bpAmpQ,
                    pz_ReportMEMetricsInd->galE5ARfStats.jammerPwrDb);

      }

      (void)locMW_SendLocCmdProxy(
         LOC_HANDLE_REGISTERED_CLIENTS,
         LOC_CMD_TYPE_NOTIFY_CLIENT,
         LOC_EVENT_MASK_BAND_MEASUREMENT_METRICS,
         (void *)pz_ReportMEMetricsInd);
    }
    else
    {
      LOC_MSG_ERROR("locPd_ExtEventCb, PDSM_EXT_STATUS_EVENT_REPORT_ME_METRICS loc_calloc returns NULL", 0, 0, 0);
    }
  }

  if (PDSM_EXT_STATUS_EVENT_LOC_NOTIFICATION_REPORT == q_ExtStatusCmd)
  {
    LOC_MSG_MED("locPd_ExtEventCb PDSM_EXT_LOCATION_NOTIFICATION", 0, 0, 0);
    locPd_HandleLocationNotification(pz_ExtStatusInfo);
  }

  if (PDSM_EXT_STATUS_EVENT_E911_NOTIFICATION_REPORT == q_ExtStatusCmd)
  {
    boolean v_e911State = pz_ExtStatusInfo->ext_status_info.z_ext_e911_state_notification.v_e911_state;
    LOC_MSG_MED("locPd_ExtEventCb PDSM_EXT_STATUS_EVENT_E911_NOTIFICATION_REPORT %d", v_e911State, 0, 0);
    if (FALSE == locMW_SendIpcMsg(
        LM_MIDDLEWARE_MSG_ID_E911_STATE_IND,
        sizeof(v_e911State),
        (void *)&(v_e911State)))
    {
      LOC_MSG_ERROR("locPd_ExtEventCb: could not send E911 state IPC to loc MW\n",
                    0, 0, 0);

    }
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
}

/* This funciton will be called when:
   (1) pdsm_get_position returns false
   (2) pd cmd callback reports an error
   (3) PDSM_PD_EVENT_DONE is received
   (4) timer expires waiting for PDSM_PD_EVENT_DONE */
static void locPd_Schedule(void)
{
  LOC_MSG_HIGH("locPd_Schedule", 0, 0, 0);

  /* Not much we can do if it fails. Also no memory is allocated, so no resource to reclaim */
  (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                              LOC_CMD_TYPE_PD_SCHEDULE, 0, NULL);

}

// Translate fix end reason into session status in the loc api position report
static qmiLocSessionStatusEnumT_v02 locPd_XlateFixEndReasonCode(pdsm_pd_end_e_type e_PdSessEndStatus)
{
  qmiLocSessionStatusEnumT_v02 e_SessionStatus;

  switch (e_PdSessEndStatus)
  {
    case PDSM_PD_END_SESS_NONE:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_SUCCESS_V02;
      break;

    case PDSM_PD_END_TIMEOUT:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_TIMEOUT_V02;
      break;

    case PDSM_PD_END_CLIENT_END:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_USER_END_V02;
      break;

    default:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02;
      break;
  }

  LOC_MSG_MED("locPd_XlateFixEndReasonCode, PdSessEndStatus = %d, qmiLocSessionStatus = %d",
              e_PdSessEndStatus, e_SessionStatus, 0);

  return e_SessionStatus;
}

// Translate PD cmd error into session status in the loc api position report
static qmiLocSessionStatusEnumT_v02 locPd_XlateCmdErrCodeToSessStatus(pdsm_pd_cmd_err_e_type e_PdCmdErr)
{
  qmiLocSessionStatusEnumT_v02 e_SessionStatus;

  switch (e_PdCmdErr)
  {
    case PDSM_PD_CMD_ERR_SESS_TYPE_P:
    case PDSM_PD_CMD_ERR_OPERATION_P:
    case PDSM_PD_CMD_ERR_NUM_FIXES_P:
    case PDSM_PD_CMD_ERR_LSINFO_P:
    case PDSM_PD_CMD_ERR_TIMEOUT_P:
    case PDSM_PD_CMD_ERR_QOS_P:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_BAD_PARAMETER_V02;
      break;

    case PDSM_PD_CMD_ERR_OFFLINE_S:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_PHONE_OFFLINE_V02;
      break;

    case PDSM_PD_CMD_ERR_GPS_LOCK_S:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_ENGINE_LOCKED_V02;
      break;

    default:
      e_SessionStatus = eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02;
      break;
  }

  LOC_MSG_MED("locPd_XlateCmdErrCodeToSessStatus, cmd err = %d, session status = %d",
              e_PdCmdErr, e_SessionStatus, 0);
  return e_SessionStatus;
}

/* Function to translate PD cmd error into session status in the LOC API position report. */
static uint32 locPd_ConvertCmdErr2LocStatus(pdsm_pd_cmd_err_e_type e_PdCmdErr)
{
  switch (e_PdCmdErr)
  {
    case PDSM_PD_CMD_ERR_NOERR:
      return LOC_API_SUCCESS;

    case PDSM_PD_CMD_ERR_SESS_TYPE_P:
    case PDSM_PD_CMD_ERR_OPERATION_P:
    case PDSM_PD_CMD_ERR_NUM_FIXES_P:
    case PDSM_PD_CMD_ERR_LSINFO_P:
    case PDSM_PD_CMD_ERR_TIMEOUT_P:
    case PDSM_PD_CMD_ERR_QOS_P:
      return LOC_API_INVALID_PARAMETER;

    case PDSM_PD_CMD_ERR_OFFLINE_S:
      return LOC_API_PHONE_OFFLINE;

    case PDSM_PD_CMD_ERR_GPS_LOCK_S:
      return LOC_API_GNSS_DISABLED;

    default:
      return LOC_API_GENERAL_FAILURE;
  }
}

/* Function to process the best available position get request. */
static int locPd_GetBestAvailPos
(
   loc_client_handle_type                           l_ClientHandle,
   const qmiLocGetBestAvailablePositionReqMsgT_v02 *pz_BestAvailPosReq
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  /* Function arguments sanity check */
  if (NULL == pz_BestAvailPosReq)
  {
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if (pz_LocPdData->z_GetBestAvailPos.v_InProgress)
  {
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    return LOC_API_ENGINE_BUSY;
  }

  /* Set the parameters since PDAPI can call the callback before this function returns */
  pz_LocPdData->z_GetBestAvailPos.v_InProgress = TRUE;
  pz_LocPdData->z_GetBestAvailPos.l_ClientHandle = l_ClientHandle;
  pz_LocPdData->z_GetBestAvailPos.q_TxnId        = pz_BestAvailPosReq->transactionId;

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  /* NFW client postion request permission check */
  if ((FALSE == locClnt_IsPosRequestAllowed2(l_ClientHandle))&&
      (FALSE == locClnt_IsSpecialPosRequest(pz_BestAvailPosReq->specialReqType)))

  {
    /* Trigger locPd_CmdCb to send error in QMI ind message*/
    locPd_CmdCb(NULL, PDSM_PD_CMD_GET_BEST_AVAIL_POS, PDSM_PD_CMD_ERR_GPS_LOCK_S);
    return LOC_API_SUCCESS;
  }

  if (!pdsm_pd_get_best_avail_pos(locPd_CmdCb, loc_middleware_data.pdapi_client_id, NULL))
  {
    /* Clear the in progress flag */
    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    pz_LocPdData->z_GetBestAvailPos.v_InProgress = FALSE;
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

    return LOC_API_GENERAL_FAILURE;
  }

  return LOC_API_SUCCESS;
}

/* Function to process the available WWAN position get request. */
static int locPd_GetAvailWwanPos
(
   loc_client_handle_type                       l_ClientHandle,
   const qmiLocGetAvailWwanPositionReqMsgT_v02 *pz_AvailWwanPosReq
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

  /* Function arguments sanity check */
  if (NULL == pz_AvailWwanPosReq)
  {
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if (pz_LocPdData->z_GetAvailWwanPos.v_InProgress)
  {
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    return LOC_API_ENGINE_BUSY;
  }

  /* Set the in progress flag and client handle since PDAPI can call the callback before this function returns */
  pz_LocPdData->z_GetAvailWwanPos.v_InProgress = TRUE;
  pz_LocPdData->z_GetAvailWwanPos.l_ClientHandle = l_ClientHandle;
  pz_LocPdData->z_GetAvailWwanPos.q_TxnId        = pz_AvailWwanPosReq->transactionId;

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if (!pdsm_pd_get_avail_wwan_pos(locPd_CmdCb, loc_middleware_data.pdapi_client_id, NULL))
  {
    /* Clear the in progress flag */
    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    pz_LocPdData->z_GetAvailWwanPos.v_InProgress = FALSE;
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

    return LOC_API_GENERAL_FAILURE;
  }

  return LOC_API_SUCCESS;
}

/* Function to process the available position get request. */
static int locPd_GetAvailPos
(
   loc_client_handle_type                             l_ClientHandle,
   const qmiLocSecureGetAvailablePositionReqMsgT_v02 *pz_SecureGetAvailablePosReq
   )
{
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  qmiLocGetAvailablePositionReqParamMsgT_v02    z_GetAvailablePosReq = { 0 }; //Small struct, define local variable directly

  /* Function arguments sanity check */
  if (NULL == pz_SecureGetAvailablePosReq)
  {
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  if (pz_LocPdData->z_GetSecureAvailPos.v_InProgress)
  {
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    return LOC_API_ENGINE_BUSY;
  }

  if (FALSE == locPd_ConvToLocSecureAvailablePosition(
        pz_SecureGetAvailablePosReq, &z_GetAvailablePosReq))
  {
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    return LOC_API_GENERAL_FAILURE;
  }

  /* Set the in progress flag and client handle since PDAPI can call the callback before this function returns */
  pz_LocPdData->z_GetSecureAvailPos.v_InProgress    = TRUE;
  pz_LocPdData->z_GetSecureAvailPos.l_ClientHandle  = l_ClientHandle;
  pz_LocPdData->z_GetSecureAvailPos.q_PosTxnID         = z_GetAvailablePosReq.transactionId;
  pz_LocPdData->z_GetSecureAvailPos.v_PosIsNonceValid = z_GetAvailablePosReq.nonce_valid;

  if (z_GetAvailablePosReq.nonce_valid)
  {
    pz_LocPdData->z_GetSecureAvailPos.t_PosNonce = z_GetAvailablePosReq.nonce;
  }
  if (z_GetAvailablePosReq.locPositionRptDataMode_valid)
  {
    pz_LocPdData->z_GetSecureAvailPos.e_PosDataMode = z_GetAvailablePosReq.locPositionRptDataMode;
  }
  else
  {
    pz_LocPdData->z_GetSecureAvailPos.e_PosDataMode = eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED_V02;
  }

  if (z_GetAvailablePosReq.locPositionSource_valid)
  {
    pz_LocPdData->z_GetSecureAvailPos.e_PosDataSource = z_GetAvailablePosReq.locPositionSource;
  }
  else
  {
    pz_LocPdData->z_GetSecureAvailPos.e_PosDataSource = eQMI_LOC_POS_SRC_GNSS_V02;
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

  /* NFW client postion request permission check */
  if (FALSE == locClnt_IsPosRequestAllowed2(l_ClientHandle))
  {
    /* Trigger locPd_CmdCb to send error in QMI ind message*/
    locPd_CmdCb(NULL, PDSM_PD_CMD_GET_AVAIL_POS_FOR_SRC, PDSM_PD_CMD_ERR_GPS_LOCK_S);
    return LOC_API_SUCCESS;
  }

  if (!pdsm_pd_get_available_pos(locPd_CmdCb,
                                 loc_middleware_data.pdapi_client_id,
                                 &(pz_LocPdData->z_GetSecureAvailPos.e_PosDataSource)))
  {
    /* Clear the in progress flag */
    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    pz_LocPdData->z_GetSecureAvailPos.v_InProgress = FALSE;
    LOC_MSG_ERROR("locPd_GetAvailPos: pdsm_pd_get_available_pos return FALSE", 0, 0, 0);
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

    return LOC_API_GENERAL_FAILURE;
  }

  return LOC_API_SUCCESS;
}

/*===========================================================================
FUNCTION locPd_InjectGsmCellInfo

DESCRIPTION
  Inject cellular information into the engine. 

DEPENDENCIES

INPUT PARAMS
  l_ClientHandle   : Identifies the loc API client injecting
  the GSM Cell information
  pz_GsmCellInfoReq : pointer to the GSM Cell Information 

RETURN VALUE
  LOC_API_SUCCESS           : processed command successfully
  LOC_API_GENERAL_FAILURE   : unspecified error command returned error.
  LOC_API_INVALID_PARAMETER : invalid parameter passed in.

SIDE EFFECTS

===========================================================================*/
static int locPd_InjectGsmCellInfo
(
   loc_client_handle_type                    l_ClientHandle,
   const qmiLocInjectGSMCellInfoReqMsgT_v02 *pz_GsmCellInfoReq
   )
{
  int                   q_Status       = LOC_API_SUCCESS;
  locPd_CmdCbData *pz_CmdCbData    = NULL;
  pdsm_cell_info_s_type z_PdsmCellInfo = { 0 };

  /* Function arguments sanity check */
  if (NULL == pz_GsmCellInfoReq)
  {
    LOC_MSG_ERROR("locPd_InjectGsmCellInfo: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    /* Allocate the callback data on heap to be freed in cmd_cb_unlocked or if there is an error. */
    pz_CmdCbData = (locPd_CmdCbData *)loc_calloc(sizeof(*pz_CmdCbData));
    if (NULL == pz_CmdCbData)
    {
      LOC_MSG_ERROR("locPd_InjectGsmCellInfo: calloc failed", 0, 0, 0);
      q_Status = LOC_API_GENERAL_FAILURE;
      break;
    }

    pz_CmdCbData->l_ClientHandle = l_ClientHandle;
    pz_CmdCbData->e_CmdType      = PDSM_PD_CMD_INJECT_CELL_INFO;
    pz_CmdCbData->q_QmiLocIndId  = QMI_LOC_INJECT_GSM_CELL_INFO_IND_V02;
    pz_CmdCbData->p_Self         = pz_CmdCbData;

    /* Set the RAT to GSM */
    z_PdsmCellInfo.activeRAT = PDSM_ACTIVERAT_GSM;

    /* Set the service status */
    z_PdsmCellInfo.serviceStatus =
       (TRUE == pz_GsmCellInfoReq->roamingStatus) ? STATUS_CAMPED_ROAMING : STATUS_CAMPED_NOT_ROAMING;

    /* Set the GSM cell information */
    z_PdsmCellInfo.cellInfo.gsmCell.CID = pz_GsmCellInfoReq->gsmCellId.CID;
    z_PdsmCellInfo.cellInfo.gsmCell.LAC = pz_GsmCellInfoReq->gsmCellId.LAC;
    z_PdsmCellInfo.cellInfo.gsmCell.MCC = pz_GsmCellInfoReq->gsmCellId.MCC;
    z_PdsmCellInfo.cellInfo.gsmCell.MNC = pz_GsmCellInfoReq->gsmCellId.MNC;

    /* Note: timingAdvance TLV is not parsed */

    if (!pdsm_pd_inject_external_cell_info(locPd_CmdCbUnlocked,
                                           pz_CmdCbData,
                                           loc_middleware_data.pdapi_client_id,
                                           &z_PdsmCellInfo))
    {
      q_Status = LOC_API_GENERAL_FAILURE;
      break;
    }
  } while (0);

  if (LOC_API_SUCCESS != q_Status)
  {
    LOC_MSG_ERROR("locPd_InjectGsmCellInfo: Error=%d", q_Status, 0, 0);

    /* Free the callback data on heap in an error case */
    if (NULL != pz_CmdCbData)
    {
      loc_free(pz_CmdCbData);
    }
  }
  else
  {
    LOC_MSG_MED("locPd_InjectGsmCellInfo: Status=%d", q_Status, 0, 0);
  }

  return q_Status;
}

/*===========================================================================
FUNCTION locPd_InjectNiMsg

DESCRIPTION
  Inject external NI message into the engine. 

DEPENDENCIES

INPUT PARAMS
  l_ClientHandle : Identifies the loc API client injecting the NI Msg
  pz_NiMsgReq     : Pointer to the NI msg 

RETURN VALUE
  LOC_API_SUCCESS           : processed command successfully
  LOC_API_GENERAL_FAILURE   : unspecified error command returned error.
  LOC_API_INVALID_PARAMETER : invalid parameter passed in.

SIDE EFFECTS

===========================================================================*/
static int locPd_InjectNiMsg
(
   loc_client_handle_type                                l_ClientHandle,
   const qmiLocInjectNetworkInitiatedMessageReqMsgT_v02 *pz_NiMsgReq
   )
{
  int                    q_Status    = LOC_API_SUCCESS;
  locPd_CmdCbData *pz_CmdCbData = NULL;
  pdsm_ni_message_s_type z_PdsmNiMsg = { 0 };

  /* Function arguments sanity check */
  if (NULL == pz_NiMsgReq)
  {
    LOC_MSG_ERROR("locPd_InjectNiMsg: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    /* Allocate the callback data on heap to be freed in cmd_cb_unlocked or if there is an error. */
    pz_CmdCbData = (locPd_CmdCbData *)loc_calloc(sizeof(*pz_CmdCbData));

    if (NULL == pz_CmdCbData)
    {
      LOC_MSG_ERROR("locPd_InjectNiMsg: calloc failed", 0, 0, 0);
      q_Status = LOC_API_GENERAL_FAILURE;
      break;
    }

    pz_CmdCbData->l_ClientHandle = l_ClientHandle;
    pz_CmdCbData->e_CmdType      = PDSM_PD_CMD_INJECT_NI_MSG;
    pz_CmdCbData->q_QmiLocIndId  = QMI_LOC_INJECT_NETWORK_INITIATED_MESSAGE_IND_V02;
    pz_CmdCbData->p_Self         = pz_CmdCbData;

    switch (pz_NiMsgReq->injectedNIMessageType)
    {
      case eQMI_LOC_INJECTED_NETWORK_INITIATED_MESSAGE_TYPE_SUPL_V02:
        {
          z_PdsmNiMsg.type = NI_MESSAGE_SUPL;
          break;
        }

      default:
        {
          q_Status = LOC_API_INVALID_PARAMETER;
          break;
        }
    }

    if (LOC_API_SUCCESS == q_Status)
    {
      z_PdsmNiMsg.len = pz_NiMsgReq->injectedNIMessage_len;

      /* pass in the pointer to msg payload, PDAPI will make a copy */
      z_PdsmNiMsg.ni_data = (uint8 *)pz_NiMsgReq->injectedNIMessage;

      if (!pdsm_pd_inject_ni_message(locPd_CmdCbUnlocked,
                                     pz_CmdCbData,
                                     loc_middleware_data.pdapi_client_id,
                                     &z_PdsmNiMsg))
      {
        q_Status = LOC_API_GENERAL_FAILURE;
        break;
      }
    }
  } while (0);

  if (LOC_API_SUCCESS != q_Status)
  {
    LOC_MSG_ERROR("locPd_InjectNiMsg: Error=%d", q_Status, 0, 0);

    /* Free the callback data on heap in an error case */
    if (NULL != pz_CmdCbData)
    {
      loc_free(pz_CmdCbData);
    }
  }
  else
  {
    LOC_MSG_MED("locPd_InjectNiMsg: Status=%d", q_Status, 0, 0);
  }

  return q_Status;
}

/*===========================================================================
FUNCTION locPd_WwanOosNotification

DESCRIPTION
  Inject WWAN Out of Service notification into the engine. 

DEPENDENCIES

INPUT PARAMS
  l_ClientHandle : Identifies the loc API client injecting the OOS notification
  

RETURN VALUE
  LOC_API_SUCCESS           : processed command successfully
  LOC_API_GENERAL_FAILURE   : unspecified error command returned error.
  LOC_API_INVALID_PARAMETER : invalid parameter passed in.

SIDE EFFECTS

===========================================================================*/
static int locPd_WwanOosNotification
(
   loc_client_handle_type l_ClientHandle
   )
{
  int                   q_Status       = LOC_API_SUCCESS;
  locPd_CmdCbData *pz_CmdCbData    = NULL;
  pdsm_cell_info_s_type z_PdsmCellInfo = { 0 };

  do
  {
    /* Allocate the callback data on heap to be freed in cmd_cb_unlocked or if there is an error. */
    pz_CmdCbData = (locPd_CmdCbData *)loc_calloc(sizeof(*pz_CmdCbData));

    if (NULL == pz_CmdCbData)
    {
      LOC_MSG_ERROR("locPd_WwanOosNotification: calloc failed", 0, 0, 0);
      q_Status = LOC_API_GENERAL_FAILURE;
      break;
    }

    pz_CmdCbData->l_ClientHandle = l_ClientHandle;
    pz_CmdCbData->e_CmdType      = PDSM_PD_CMD_INJECT_CELL_INFO;
    pz_CmdCbData->q_QmiLocIndId  = QMI_LOC_WWAN_OUT_OF_SERVICE_NOTIFICATION_IND_V02;
    pz_CmdCbData->p_Self         = pz_CmdCbData;

    /* Set the service status */
    z_PdsmCellInfo.serviceStatus = STATUS_OUT_OF_SERVICE;

    if (!pdsm_pd_inject_external_cell_info(locPd_CmdCbUnlocked,
                                           pz_CmdCbData,
                                           loc_middleware_data.pdapi_client_id,
                                           &z_PdsmCellInfo))
    {
      q_Status = LOC_API_GENERAL_FAILURE;
      break;
    }
  } while (0);

  if (LOC_API_SUCCESS != q_Status)
  {
    LOC_MSG_ERROR("locPd_WwanOosNotification: Error=%d", q_Status, 0, 0);

    /* Free the callback data on heap in an error case */
    if (NULL != pz_CmdCbData)
    {
      loc_free(pz_CmdCbData);
    }
  }
  else
  {
    LOC_MSG_MED("locPd_WwanOosNotification: Status=%d", q_Status, 0, 0);
  }

  return q_Status;
}

/**
 * called by PD task to send QMI Event Ind to LocMW 
 * task 
 *
 *  @return -- TRUE is IPC was sent successfully
 *          -- FALSE otherwise.
 */
static boolean locPd_EventWifiApScanInfoInjectIndProxy
(
   const pdsm_pd_wifi_info_on_demand_injection_req_s_type *pz_WifiApInfoReqInd
   )
{
  qmiLocEventInjectWifiApDataReqIndMsgT_v02 z_EventWifiApReqInd = { 0 };

  if (NULL == pz_WifiApInfoReqInd)
  {
    LOC_MSG_ERROR("locPd_EventWifiApScanInfoInjectIndProxy: NULL pointer", 0, 0, 0);
    return FALSE;
  }

  z_EventWifiApReqInd.e911Mode_valid = TRUE;
  z_EventWifiApReqInd.e911Mode = pz_WifiApInfoReqInd->v_emergencyMode;
  /** Generate an IPC to loc MW*/
  if (FALSE == locMW_SendIpcMsg(
        LM_MIDDLEWARE_MSG_ID_PD_EVENT_WIFI_AP_SCAN_INJECT_REQ_IND,
        sizeof(z_EventWifiApReqInd),
        (void *)&z_EventWifiApReqInd))
  {
    LOC_MSG_ERROR("locPd_EventWifiApScanInfoInjectIndProxy: could not send IPC to loc MW\n",
                  0, 0, 0);

    return FALSE;
  }

  return TRUE;
}

/**
 * called by PD task to send indication to LocMW task 
 *
 * @param client_handle
 *               LocAPI client handle
 * @param wifi_inj_ack
 *               Response/Ack indication sent from PDSM in
 *               locEng format
 *
 * @return 
 * TRUE - if IPC posted to LocMW success 
 * TRUE - if IPC posted to LocMW failed 
 */
static boolean locPd_AckWifiApScanInfoInjectIndProxy(loc_client_handle_type                     client_handle,
                                                     const locEngInjectWifiApDataIndMsgT_v01 *pz_WifiInjAck)
{
  loc_pd_ack_wifi_ap_scan_info_inject_ind_struct_t ackWifiAPIndValue;

  LOC_MSG_MED("locPd_AckWifiApScanInfoInjectIndProxy", 0, 0, 0);

  memset(&ackWifiAPIndValue, 0, sizeof(ackWifiAPIndValue));

  ackWifiAPIndValue.client_handle = client_handle;

  ackWifiAPIndValue.ackWifiApInd.status = convertLocEngStatus(pz_WifiInjAck->status);

  /* Generate an IPC to LocMW task */
  if (!locMW_SendIpcMsg(LM_MIDDLEWARE_MSG_ID_PD_INJECT_WIFI_AP_SCAN_IND,
                        sizeof(ackWifiAPIndValue),
                        (void *)&ackWifiAPIndValue))
  {
    LOC_MSG_ERROR("locPd_AckWifiApScanInfoInjectIndProxy: could not send IPC to loc MW", 0, 0, 0);
    return FALSE;
  }

  return TRUE;
}

/** locPd_EventWifiApScanInfoInjectIndHandler
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
void locPd_EventWifiApScanInfoInjectIndHandler(const os_IpcMsgType *pz_IpcMsg)
{
  LOC_MSG_MED("locPd_EventWifiApScanInfoInjectIndHandler", 0, 0, 0);

  if ((NULL != pz_IpcMsg) &&
      (pz_IpcMsg->q_Size == sizeof(qmiLocEventInjectWifiApDataReqIndMsgT_v02)))
  {
    qmiLocEventInjectWifiApDataReqIndMsgT_v02 z_EventWifiApReqInd = { 0 };
    locQmiShimIndInfoStructT                  z_ShimIndInfo = { 0 };

    z_ShimIndInfo.q_MsgLen = memscpy(&z_EventWifiApReqInd, sizeof(z_EventWifiApReqInd), pz_IpcMsg->p_Data, pz_IpcMsg->q_Size);

    z_ShimIndInfo.p_Msg = &z_EventWifiApReqInd;
    z_ShimIndInfo.q_Id = QMI_LOC_EVENT_INJECT_WIFI_AP_DATA_REQ_IND_V02;

    // Broadcast to all registered clients for now
    if (FALSE == locQmiShimBroadcastInd(
          QMI_LOC_EVENT_MASK_INJECT_WIFI_AP_DATA_REQ_V02,
          &z_ShimIndInfo))
    {
      LOC_MSG_ERROR("locPd_EventWifiApScanInfoInjectIndHandler: Could not send indication\n",
                    0, 0, 0);
    }
  }
  else
  {
    LOC_MSG_ERROR("locPd_EventWifiApScanInfoInjectIndHandler: Invalid IPC message\n", 0, 0, 0);
  }

  return;
}

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
boolean locPd_InjectWifiApScanInfoIndHandler(const os_IpcMsgType *pIpcMsg)
{
  if ((NULL != pIpcMsg) &&
      (pIpcMsg->q_Size == sizeof(loc_pd_ack_wifi_ap_scan_info_inject_ind_struct_t)))
  {
    loc_pd_ack_wifi_ap_scan_info_inject_ind_struct_t ackWifiAPIndValue = { 0 };
    locQmiShimIndInfoStructT                         shimIndInfo = { 0 };

    (void)memscpy(&ackWifiAPIndValue, sizeof(ackWifiAPIndValue), pIpcMsg->p_Data, pIpcMsg->q_Size);

    // send a unicast message
    shimIndInfo.p_Msg = &ackWifiAPIndValue.ackWifiApInd;
    shimIndInfo.q_Id = QMI_LOC_INJECT_WIFI_AP_DATA_IND_V02;
    shimIndInfo.q_MsgLen = sizeof(ackWifiAPIndValue.ackWifiApInd);

    LOC_MSG_MED("locPd_InjectWifiApScanInfoIndHandler: calling locQmiShimSendInd client handle %d",
                loc_middleware_data.z_locPd_Data.wifi_ap_scan_client_handle, 0, 0);
    loc_middleware_data.z_locPd_Data.wifi_ap_scan_in_progress = FALSE;

    /* send indication */
    return locQmiShimSendInd(loc_middleware_data.z_locPd_Data.wifi_ap_scan_client_handle,
                             &shimIndInfo);
  }
  else
  {
    LOC_MSG_ERROR("locPd_InjectWifiApScanInfoIndHandler: Invalid IPC message\n", 0, 0, 0);
    return FALSE;
  }
}

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
boolean locPd_ProcessInjectWifiApDataReq(loc_client_info_s_type *pz_ClientInfo,
                                         const qmiLocInjectWifiApDataReqMsgT_v02 *pz_Req,
                                         qmiLocGenRespMsgT_v02 *pz_Resp)
{
  pdsm_wifi_on_demand_info_list_s_type   z_PdsmOnDemandInfo = { 0 };
  pdsm_wifi_ap_scan_info_s_type          z_PdsmApScanInfo = { 0 };
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  locEngWifiApInfoStructT_v01 *p_WifiApArray = NULL;
  locEngWifiApAdditionalDataStructT_v01 *p_WifiAppAddArray = NULL;
  boolean                                v_RetVal = FALSE;

  if (NULL == pz_Resp)
  {
    LOC_MSG_ERROR("locPd_ProcessInjectWifiApDataReq: pResp is NULL \n", 0, 0, 0);
    return v_RetVal;
  }

  if ((NULL == pz_ClientInfo) || (NULL == pz_Req))
  {
    /** invalid parameters */
    pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
    pz_Resp->resp.error = QMI_ERR_INVALID_ARG_V01;
    LOC_MSG_ERROR("locPd_ProcessInjectWifiApDataReq: Invalid params\n", 0, 0, 0);
    return v_RetVal;
  }

  do
  {
    LOC_MSG_MED("locPd_ProcessInjectWifiApDataReq. client: %d, num of APs %d",
                pz_ClientInfo->client_handle, pz_Req->wifiApInfo_len, 0);

    /* check the list length */
    if (pz_Req->wifiApInfo_len > QMI_LOC_WIFI_MAX_REPORTED_APS_PER_MSG_V02)
    {
      pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
      pz_Resp->resp.error = QMI_ERR_ARG_TOO_LONG_V01;

      LOC_MSG_ERROR("locPd_ProcessInjectWifiApDataReq: wifiApInfo_len greater than limit \n", 0, 0, 0);
      break;
    }

    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    /* check progress flag */
    if (pz_LocPdData->wifi_ap_scan_in_progress == TRUE)
    {
      pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
      pz_Resp->resp.error = QMI_ERR_DEVICE_IN_USE_V01;
      LOC_MSG_ERROR("locPd_ProcessInjectWifiApDataReq: wifi ap scan injection already in progress\n", 0, 0, 0);
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      break;
    }

    /* store the client handle for later use */
    pz_LocPdData->wifi_ap_scan_client_handle = pz_ClientInfo->client_handle;
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

    /* convert QMI to locEng/pdsm format */
    z_PdsmApScanInfo.q_num_wifi_aps = pz_Req->wifiApInfo_len;
    if (TRUE == pz_Req->errorCause_valid)
    {
      switch (pz_Req->errorCause)
      {
        case eQMI_LOC_WLAN_AP_ERR_NO_REQ_MEAS_AVAILABLE_V02:
          z_PdsmOnDemandInfo.e_errorCause = eLOC_ENG_WLAN_AP_ERR_NO_REQ_MEAS_AVAILABLE_V01;
          break;
        case eQMI_LOC_WLAN_AP_ERR_WIFI_OFF_V02:
          z_PdsmOnDemandInfo.e_errorCause = eLOC_ENG_WLAN_AP_ERR_WIFI_OFF_V01;
          break;
        case eQMI_LOC_WLAN_AP_ERR_UNKNOWN_V02:
          z_PdsmOnDemandInfo.e_errorCause = eLOC_ENG_WLAN_AP_ERR_NONE_V01;
          break;
        default:
          z_PdsmOnDemandInfo.e_errorCause = eLOC_ENG_WLAN_AP_ERR_UNKNOWN_V01;
          break;
      }
    }

    if (TRUE == pz_Req->requestTimestamp_valid)
    {
      z_PdsmOnDemandInfo.t_request_timestamp_utc = pz_Req->requestTimestamp;
    }

    if (TRUE == pz_Req->receiveTimestamp_valid)
    {
      z_PdsmOnDemandInfo.t_recieved_timestamp_utc = pz_Req->receiveTimestamp;
    }

    if (TRUE == pz_Req->onDemandScan_valid)
    {
      z_PdsmOnDemandInfo.v_on_demand_scan = pz_Req->onDemandScan;
    }

    if (0 != z_PdsmApScanInfo.q_num_wifi_aps)
    {
      p_WifiApArray = (locEngWifiApInfoStructT_v01 *)os_MemAlloc(sizeof(locEngWifiApInfoStructT_v01) * z_PdsmApScanInfo.q_num_wifi_aps, OS_MEM_SCOPE_TASK);
      if (NULL == p_WifiApArray)
      {
        LOC_MSG_ERROR("locPd_ProcessInjectWifiApDataReq os_MemAlloc 1 returns NULL", 0, 0, 0);
        break;
      }
      convertQmiLocInjectWifiApDataReq(&pz_Req->wifiApInfo[0], p_WifiApArray, z_PdsmApScanInfo.q_num_wifi_aps);
      z_PdsmApScanInfo.first_wifi_ap_ptr = p_WifiApArray;

      if (TRUE == pz_Req->wifiApInfoA_valid)
      {
        if (pz_Req->wifiApInfo_len != pz_Req->wifiApInfoA_len)
        {
          LOC_MSG_ERROR("locPd_ProcessInjectWifiApDataReq: wifiApInfoA_len %d != pz_Req->wifiApInfo_len %d",
                        pz_Req->wifiApInfoA_len, pz_Req->wifiApInfo_len, 0);
        }
        else
        {
          p_WifiAppAddArray = (locEngWifiApAdditionalDataStructT_v01 *)os_MemAlloc(sizeof(locEngWifiApAdditionalDataStructT_v01) * z_PdsmApScanInfo.q_num_wifi_aps, OS_MEM_SCOPE_TASK);
          if (NULL == p_WifiAppAddArray)
          {
            LOC_MSG_ERROR("locPd_ProcessInjectWifiApDataReq os_MemAlloc 2 returns NULL", 0, 0, 0);
            break;
          }
          convertQmiLocInjectWifiApAdditionalDataReq((qmiLocWifiApAdditionalDataStructT_v02 *const)&pz_Req->wifiApInfoA[0],
                                                     p_WifiAppAddArray, z_PdsmApScanInfo.q_num_wifi_aps);
          z_PdsmApScanInfo.first_wifi_ap_add_info_ptr = p_WifiAppAddArray;
        }
      }
    }
    z_PdsmOnDemandInfo.z_wifi_ap_scan_info = z_PdsmApScanInfo;

    // call pdsm
    /* return value is used to send QMI response. NO need for locPd_CmdCb to handle again */
    v_RetVal = pdsm_pd_inject_wifi_info_on_demand(NULL,      //locPd_CmdCb,
                                                  (void *)pz_ClientInfo,
                                                  loc_middleware_data.pdapi_client_id,
                                                  &z_PdsmOnDemandInfo);
    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    if (TRUE == v_RetVal)
    {
      LOC_MSG_MED("locPd_ProcessInjectWifiApDataReq return success",
                  0, 0, 0);
      /* set progress flag*/
      pz_LocPdData->wifi_ap_scan_in_progress = TRUE;
      pz_Resp->resp.result =  QMI_RESULT_SUCCESS_V01;
      pz_Resp->resp.error = QMI_ERR_NONE_V01;
    }
    else
    {
      LOC_MSG_MED("locPd_ProcessInjectWifiApDataReq return failure",
                  0, 0, 0);

      /* locPd_CmdCb will not be called for failure handling
         wifi on demand - PDSM_PD_CMD_INJECT_WIFI_ON_DEMAND - will never be called as we are passing null func pointer
         in the pdsm call. Failure QMI RESP is sent when pdsm call return failure. NO QMI IND is sent */
      /* reset progress flag*/
      pz_LocPdData->wifi_ap_scan_in_progress = FALSE;
      pz_LocPdData->wifi_ap_scan_client_handle = LOC_CLIENT_HANDLE_INVALID;
      pz_Resp->resp.result =  QMI_RESULT_FAILURE_V01;
      pz_Resp->resp.error = QMI_ERR_INTERNAL_V01;
    }
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  }while (0);

  if (p_WifiApArray)
  {
    os_MemFree((void **)&p_WifiApArray);
  }
  if (p_WifiAppAddArray)
  {
    os_MemFree((void **)&p_WifiAppAddArray);
  }

  return v_RetVal;
}

/* Short Range Node (SRN) BTLE loc_pd Api's*/
/**
 * called by PD task to send QMI Event Ind to LocMW 
 * task 
 *
 *  @return -- TRUE is IPC was sent successfully
 *          -- FALSE otherwise.
 */
static boolean locPd_EventSrnApScanInfoInjectIndProxy
(
   const locEngEventInjectSrnApDataReqIndMsgT_v01 *pz_SrnApInfoReqInd
   )
{
  qmiLocEventInjectSrnApDataReqIndMsgT_v02 z_EventSrnApReqInd = { 0 };

  if (NULL == pz_SrnApInfoReqInd)
  {
    LOC_MSG_ERROR("locPd_EventSrnApScanInfoInjectIndProxy: NULL pointer", 0, 0, 0);
    return FALSE;
  }
  /*Check and copy 911 indication*/
  if (TRUE == pz_SrnApInfoReqInd->e911Mode_valid)
  {
    z_EventSrnApReqInd.e911Mode_valid = TRUE;
    z_EventSrnApReqInd.e911Mode = pz_SrnApInfoReqInd->e911Mode;
  }
  /*Check and copy MAC address type requested*/
  if (TRUE == pz_SrnApInfoReqInd->srnApMacAddrType_valid)
  {
    z_EventSrnApReqInd.srnApMacAddrType_valid = TRUE;
    z_EventSrnApReqInd.srnApMacAddrType = (qmiLocSrnApDataMacAddrTypeEnumT_v02)(pz_SrnApInfoReqInd->srnApMacAddrType);
  }

  z_EventSrnApReqInd.srnRequest = pz_SrnApInfoReqInd->srnRequest;
  z_EventSrnApReqInd.srnTechMask = pz_SrnApInfoReqInd->srnTechMask;

  /** Generate an IPC to loc MW*/
  if (FALSE == locMW_SendIpcMsg(
        LM_MIDDLEWARE_MSG_ID_PD_EVENT_SRN_AP_SCAN_INJECT_REQ_IND,
        sizeof(z_EventSrnApReqInd),
        (void *)&z_EventSrnApReqInd))
  {
    LOC_MSG_ERROR("locPd_EventSrnApScanInfoInjectIndProxy: could not send IPC to loc MW\n",
                  0, 0, 0);

    return FALSE;
  }

  return TRUE;
}

/**
 * called by PD task to send indication to LocMW task 
 *
 * @param client_handle
 *               LocAPI client handle
 * @param wifi_inj_ack
 *               Response/Ack indication sent from PDSM in
 *               locEng format
 *
 * @return 
 * TRUE - if IPC posted to LocMW success 
 * TRUE - if IPC posted to LocMW failed 
 */
static boolean locPd_AckSrnApScanInfoInjectIndProxy(const locEngInjectSrnApDataIndMsgT_v01 *pz_SrnInjAck)
{
  qmiLocInjectSrnApDataIndMsgT_v02 z_ackSrnAPInd = { 0 };

  LOC_MSG_MED("locPd_AckSrnApScanInfoInjectIndProxy", 0, 0, 0);

  if (NULL == pz_SrnInjAck)
  {
    LOC_MSG_MED("locPd_AckSrnApScanInfoInjectIndProxy NULL pointer", 0, 0, 0);
    return FALSE;
  }

  z_ackSrnAPInd.status = convertLocEngStatus(pz_SrnInjAck->status);

  /* Generate an IPC to LocMW task */
  if (!locMW_SendIpcMsg(LM_MIDDLEWARE_MSG_ID_PD_INJECT_SRN_AP_SCAN_IND,
                        sizeof(z_ackSrnAPInd),
                        (void *)&z_ackSrnAPInd))
  {
    LOC_MSG_ERROR("locPd_AckSrnApScanInfoInjectIndProxy: could not send IPC to loc MW", 0, 0, 0);
    return FALSE;
  }

  return TRUE;
}


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
void locPd_EventSrnApScanInfoInjectIndHandler(const os_IpcMsgType *pz_IpcMsg)
{
  LOC_MSG_MED("locPd_EventWifiApScanInfoInjectIndHandler", 0, 0, 0);

  if ((NULL != pz_IpcMsg) &&
      (pz_IpcMsg->q_Size == sizeof(qmiLocEventInjectSrnApDataReqIndMsgT_v02)))
  {
    locQmiShimIndInfoStructT                  z_ShimIndInfo = { 0 };

    z_ShimIndInfo.q_MsgLen = sizeof(qmiLocEventInjectSrnApDataReqIndMsgT_v02);
    z_ShimIndInfo.p_Msg = (void *)pz_IpcMsg->p_Data;
    z_ShimIndInfo.q_Id = QMI_LOC_EVENT_INJECT_SRN_AP_DATA_REQ_IND_V02;

    // Broadcast to all registered clients for now
    if (FALSE == locQmiShimBroadcastInd(
          QMI_LOC_EVENT_MASK_INJECT_SRN_AP_DATA_REQ_V02,
          &z_ShimIndInfo))
    {
      LOC_MSG_ERROR("locPd_EventSrnApScanInfoInjectIndHandler: Could not send indication\n",
                    0, 0, 0);
    }
  }
  else
  {
    LOC_MSG_ERROR("locPd_EventSrnApScanInfoInjectIndHandler: Invalid IPC message\n", 0, 0, 0);
  }

  return;
}

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
boolean locPd_InjectSrnApScanInfoIndHandler(const os_IpcMsgType *pIpcMsg)
{
  if ((NULL != pIpcMsg) &&
      (pIpcMsg->q_Size == sizeof(qmiLocInjectSrnApDataIndMsgT_v02)))
  {
    locQmiShimIndInfoStructT                         shimIndInfo = { 0 };

    // send a unicast message
    shimIndInfo.p_Msg = (void *)pIpcMsg->p_Data;
    shimIndInfo.q_Id = QMI_LOC_INJECT_SRN_AP_DATA_IND_V02;
    shimIndInfo.q_MsgLen = sizeof(qmiLocInjectSrnApDataIndMsgT_v02);

    LOC_MSG_MED("locPd_InjectSrnApScanInfoIndHandler: calling locQmiShimSendInd", 0, 0, 0);
    /* send indication */
    return locQmiShimSendInd(loc_middleware_data.z_locPd_Data.srn_ap_scan_client_handle,
                             &shimIndInfo);
  }
  else
  {
    LOC_MSG_ERROR("locPd_InjectSrnApScanInfoIndHandler: Invalid IPC message\n", 0, 0, 0);
    return FALSE;
  }
}

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
boolean locPd_ProcessInjectSrnApDataReq(loc_client_info_s_type *pz_ClientInfo,
                                        const qmiLocInjectSrnApDataReqMsgT_v02 *pz_Req,
                                        qmiLocGenRespMsgT_v02 *pz_Resp)
{
  pdsm_srn_scan_info_list_s_type         z_PdsmSrnInfo = { 0 };
  pdsm_srn_ap_scan_info_s_type           z_PdsmSrnApScanInfo = { 0 };
  locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);
  locEngSrnBtleApDeviceDataStructT_v01 *p_SrnApArray = NULL;
  boolean                                v_RetVal = FALSE;

  if (NULL == pz_Resp)
  {
    LOC_MSG_ERROR("locPd_ProcessInjectSrnApDataReq: pResp is NULL \n", 0, 0, 0);
    return v_RetVal;
  }

  if ((NULL == pz_ClientInfo) || (NULL == pz_Req))
  {
    /** invalid parameters */
    pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
    pz_Resp->resp.error = QMI_ERR_INVALID_ARG_V01;
    LOC_MSG_ERROR("locPd_ProcessInjectSrnApDataReq: Invalid params\n", 0, 0, 0);
    return v_RetVal;
  }

  do
  {
    if (FALSE == (convertQmiLocToLocEngSrnDataTechTypeEnum(pz_Req->apDeviceTechType, &z_PdsmSrnInfo.e_srnApTechType)))
    {
      pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
      pz_Resp->resp.error = QMI_ERR_INVALID_ARG_V01;

      LOC_MSG_ERROR("locPd_ProcessInjectSrnApDataReq: Tech Type invalid is 0 \n", 0, 0, 0);
      break;
    }

    if (TRUE == pz_Req->srnBtleApInfo_valid)
    {
      /* check the list length */
      if (pz_Req->srnBtleApInfo_len > QMI_LOC_SRN_MAX_REPORTED_APS_PER_MSG_V02)
      {
        pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
        pz_Resp->resp.error = QMI_ERR_ARG_TOO_LONG_V01;

        LOC_MSG_ERROR("locPd_ProcessInjectSrnApDataReq: srnApInfo_len greater than limit \n", 0, 0, 0);
        break;
      }
      else if (0 == pz_Req->srnBtleApInfo_len)
      {
        pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
        pz_Resp->resp.error = QMI_ERR_MISSING_ARG_V01;

        LOC_MSG_ERROR("locPd_ProcessInjectSrnApDataReq: wifiApInfo_len is 0 \n", 0, 0, 0);
        break;
      }
    }

    LOC_MSG_MED("locPd_ProcessInjectSrnApDataReq. client: %d, num of APs %d",
                pz_ClientInfo->client_handle, pz_Req->srnBtleApInfo_len, 0);

    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
    /* store the client handle for later use */
    pz_LocPdData->srn_ap_scan_client_handle = pz_ClientInfo->client_handle;
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

    /* convert QMI to locEng/pdsm format */
    z_PdsmSrnApScanInfo.q_num_srn_aps = pz_Req->srnBtleApInfo_len;

    if (TRUE == pz_Req->errorCause_valid)
    {
      switch (pz_Req->errorCause)
      {
        case eQMI_LOC_SRN_AP_ERR_NO_REQ_MEAS_AVAILABLE_V02:
          z_PdsmSrnInfo.e_errorCause = eLOC_ENG_SRN_AP_ERR_NO_REQ_MEAS_AVAILABLE_V01;
          break;
        case eQMI_LOC_SRN_AP_ERR_SENSOR_OFF_V02:
          z_PdsmSrnInfo.e_errorCause = eLOC_ENG_SRN_AP_ERR_SENSOR_OFF_V01;
          break;
        case eQMI_LOC_SRN_AP_ERR_UNKNOWN_V02:
          z_PdsmSrnInfo.e_errorCause = eLOC_ENG_SRN_AP_ERR_NONE_V01;
          break;
        default:
          z_PdsmSrnInfo.e_errorCause = eLOC_ENG_SRN_AP_ERR_UNKNOWN_V01;
          break;
      }
    }

    /* Copy request and response time stamp*/
    if (TRUE == pz_Req->requestTimestamp_valid)
    {
      z_PdsmSrnInfo.t_request_timestamp_utc  = pz_Req->requestTimestamp;
    }
    if (TRUE == pz_Req->receiveTimestamp_valid)
    {
      z_PdsmSrnInfo.t_recieved_timestamp_utc = pz_Req->receiveTimestamp;
    }

    /* Check for Max AP and No AP's done above,
       sufficient to check for AP info validity only.*/
    if (TRUE == pz_Req->srnBtleApInfo_valid)
    {
      p_SrnApArray = (locEngSrnBtleApDeviceDataStructT_v01 *)os_MemAlloc(sizeof(locEngSrnBtleApDeviceDataStructT_v01) * z_PdsmSrnApScanInfo.q_num_srn_aps, OS_MEM_SCOPE_TASK);

      if (NULL == p_SrnApArray)
      {
        LOC_MSG_ERROR("locPd_ProcessInjectSrnApDataReq os_MemAlloc 1 returns NULL", 0, 0, 0);
        break;
      }

      convertQmiLocInjectSrnApDataReq(&pz_Req->srnBtleApInfo[0], p_SrnApArray, z_PdsmSrnApScanInfo.q_num_srn_aps);

      z_PdsmSrnApScanInfo.srn_ap_ptr = p_SrnApArray;

      z_PdsmSrnInfo.z_srn_ap_scan_info = z_PdsmSrnApScanInfo;
    }

    // call pdsm
    /* return value is used to send QMI response. NO need for loc_pd_cmd_cb to handle again */
    if (TRUE == pdsm_pd_inject_srn_scan_info(NULL,
                                             (void *)pz_ClientInfo,
                                             loc_middleware_data.pdapi_client_id,
                                             &z_PdsmSrnInfo))
    {
      LOC_MSG_MED("locPd_ProcessInjectSrnApDataReq return success",
                  0, 0, 0);
      pz_Resp->resp.result =  QMI_RESULT_SUCCESS_V01;
      pz_Resp->resp.error = QMI_ERR_NONE_V01;
      v_RetVal = TRUE;
    }
    else
    {
      LOC_MSG_MED("locPd_ProcessInjectSrnApDataReq return failure",
                  0, 0, 0);

      /* loc_pd_cmd_cb will not be called for failure handling
            as we are passing null func pointer in the pdsm call.
            Failure QMI RESP is sent when pdsm call return failure. NO QMI IND is sent */

      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      pz_LocPdData->srn_ap_scan_client_handle = LOC_CLIENT_HANDLE_INVALID;
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

      pz_Resp->resp.result =  QMI_RESULT_FAILURE_V01;
      pz_Resp->resp.error = QMI_ERR_INTERNAL_V01;
    }

  }while (0);

  if (p_SrnApArray)
  {
    os_MemFree((void **)&p_SrnApArray);
  }

  return v_RetVal;
}

void locPd_endSession(pdsm_pd_end_session_opt_s_type z_EndSessionOpt)
{
  pdsm_end_session_ex(locPd_CmdCb,
                      PDSM_PD_END_SESSION_FIX_REQ,
                      &z_EndSessionOpt,
                      NULL,
                      loc_middleware_data.pdapi_client_id);
}
