/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM Auxiliary Technology Interface  module

GENERAL DESCRIPTION
This file implements the TM's non-GNSS technology interface.

Copyright (c) 2017 - 2019 Qualcomm Technologies, Inc.
Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  when        who     what, where, why
  --------    ---     ----------------------------------------------------------
  12/15/21   sai     Added new NV for ODCPI and change in ODCPI back off logic
  06/20/19   skm     Z-Axis support for DBH
  02/12/19    ak     Changes to use a copy of currenttime while calculation 
                     of measage for AP
  12/21/18    skm     Provide UE WIFI Mac Address in LPPe
  09/20/18    skm     Ensure duplicate start CPI requests are not issued
  03/20/18    skm     Supl E911 over Wifi
  02/17/17    skm     BLE and Barometer support for LPPE Phase 2
  ============================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "customer.h"   /* Customer configuration file */
#include "comdef.h"

#include "msg.h"
#include "tm_pdapi_client.h"
#include "tm_umts_up_supl.h"
#include "tm_auxtech_iface.h"
#include "tech_sel_api.h"
#include "slim_client_types.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/* Maximum number of cached wifi aps that are returned to the caller */
#define MAX_CACHED_WLAN_AP_SIZE (64)
/* Request WIFI on-demand scans after 3 sec of HLOS notifies WiFi is turned Off */
#define WIFI_ON_DEMAND_REQ_TIME_INTERVAL_MSEC (3000)

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/
/* Function prototype */
prtl_event_cb_f_type* tm_core_get_umpts_up_event_cb_fp(void);
prtl_event_cb_f_type* tm_core_get_lpp_cp_event_cb_fp(void);

/* Maximum number if cached Srn AP's that can be reported*/
#define MAX_CACHED_SRN_AP_SIZE (64)

/* Data structure that holds the session info */
typedef struct
{
  tm_auxTechReqId e_wifiReqId; /* Wifi requestor id */
  tm_auxTechReqId e_cpiReqId;  /* CPI requestor id */
  tm_auxTechReqId e_dbhReqId;   /* DBH requestor id */
  tm_auxTechReqId e_srnReqId;   /* SRN requestor id */
  tm_auxTechReqId e_ubpReqId;   /* BARO requestor id */
  q_TselRequestId q_cpiTechSelRequestId; /* CPI Tech Sel Request Id returned by tm_startCPIRequest call */
  boolean v_reqWifiScan; /* Flag that holds whether wifi scan request is made */

  pdsm_client_id_type l_wifiClientId; /*Client of previous */
  tm_lppe_WifiInfoListType z_cache_wifi_scan_result; /* Cache that holds the wifi scan results.*/
#ifdef FEATURE_GNSS_LPPE
  tm_lppe_SrnInfoListType  z_cache_srn_scan_result;
  tm_lppe_sensorInfoType   z_cache_baro_meas_result;
#endif /* FEATURE_GNSS_LPPE */
} tm_auxTech_sessioninfo_s_type;

typedef struct
{
  boolean                v_CachedPosValid;  /* Flag that hold if CPI is Valid*/
  sm_FixReportStructType *z_CachedPos;       /* Structure that holds CPI Pos*/
}tm_auxTech_CachedCpi_s_type;

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
static tm_auxTech_sessioninfo_s_type z_sessioninfo;
static os_TimerMsgType *pz_WifiOnDemandReqTimer = NULL;
static tm_auxTech_CachedCpi_s_type z_CacheCPIPos;

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/

/*===========================================================================
FUNCTION  tm_requestOnDemandWifiScan

DESCRIPTION
  Start on-demand wifi scan.
  If v_emergencyMode is set, priority processing (emergency mode) is requested.

PARAMETERS
  v_emergencyMode - if set to TURE, priority processing is requested
  e_reqId - requestor type.

DEPENDENCIES
   None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
static void tm_requestOnDemandWifiScan(boolean v_emergencyMode, tm_auxTechReqId e_reqId, pdsm_client_id_type l_clientId)
{

  tm_pdapi_pd_event_s_type *pz_eventInfo;

  /* Allocate memory */
  pz_eventInfo = (tm_pdapi_pd_event_s_type *)os_MemAlloc(sizeof(tm_pdapi_pd_event_s_type), OS_MEM_SCOPE_TASK);

  if (NULL == pz_eventInfo)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory. So, not sending wifi scan request ", 0, 0, 0);
    return;
  }

  memset(pz_eventInfo, 0, sizeof(*pz_eventInfo));

  pz_eventInfo->e_pd_event = PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION;
  pz_eventInfo->pd_event_data.pd_info_type.client_id = l_clientId;

  pz_eventInfo->pd_event_data.pd_info_type.pd_info.z_wifiOnDemandReq.v_emergencyMode = v_emergencyMode;

  tm_pdapi_pd_event_callback(pz_eventInfo, TM_PDAPI_SESS_TYPE_NONE);

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "WiFi scan request posted with requestor id %d", e_reqId);
  os_MemFree((void **)&pz_eventInfo);
}

/*===========================================================================
FUNCTION  auxtech_start_wifi_on_demand_req_timer

DESCRIPTION
  Create WiFi on-demand request timer & start the timer with the given interval.

PARAMETERS
  cq_TimeInteralMSec - Timer interval in milli seconds.

DEPENDENCIES
   None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
static void auxtech_start_wifi_on_demand_req_timer(uint32 const cq_TimeInteralMSec)
{
  if (NULL == pz_WifiOnDemandReqTimer)
  {
    pz_WifiOnDemandReqTimer = os_TimerCreateInCgpsTimerGroup(TM_AUXTECH_WIFI_ON_DEMAND_REQ_TIMER_ID,
                                                             THREAD_ID_SM_TM);
    if (NULL == pz_WifiOnDemandReqTimer)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Wifi on demand req timer failed");
      return;
    }
  }
  os_TimerStop(pz_WifiOnDemandReqTimer);
  os_TimerStart(pz_WifiOnDemandReqTimer, cq_TimeInteralMSec, 0);
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Wifi on demand req interval %u", cq_TimeInteralMSec);
}

/*===========================================================================
FUNCTION  tm_reqOnDemandWifiScan

DESCRIPTION
  Start on-demand wifi scan.
  If v_emergencyMode is set, priority processing (emergency mode) is requested.

PARAMETERS
  v_emergencyMode - if set to TURE, priority processing is requested
  e_reqId - requestor Id.
  l_clientId - client id.

DEPENDENCIES
   None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
void tm_reqOnDemandWifiScan(boolean v_emergencyMode, tm_auxTechReqId e_reqId, pdsm_client_id_type l_clientId)
{
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Req on demand WiFi scan. ReqId %d Emergency %d", e_reqId, v_emergencyMode);

  tm_requestOnDemandWifiScan(v_emergencyMode, e_reqId, l_clientId);

  z_sessioninfo.v_reqWifiScan = TRUE;
  z_sessioninfo.e_wifiReqId = e_reqId;
  z_sessioninfo.l_wifiClientId = l_clientId;
}

/*===========================================================================
FUNCTION  tm_resetOnDemandWifiScanReq

DESCRIPTION
  Reset on-demand wifi scan related variables.
  If this call is made, wifi scan re-request is not made on the next wifi ap results.

DEPENDENCIES
   None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
void tm_resetOnDemandWifiScanReq(tm_auxTechReqId e_reqId)
{
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Reset on demand WiFi scan states. ReqId %d", e_reqId);

  /* If the requestor id not same as the one mentioned in tm_reqOnDemandWifiScan API */
  if (z_sessioninfo.e_wifiReqId != e_reqId)
  {
    return;
  }
  z_sessioninfo.v_reqWifiScan = FALSE;
  z_sessioninfo.e_wifiReqId   = TM_AUXTECH_REQ_ID_NONE;

  /* Stop & release the wifi on-demand req timer if running */
  os_TimerStop(pz_WifiOnDemandReqTimer);
  os_TimerDestroy(pz_WifiOnDemandReqTimer);
  pz_WifiOnDemandReqTimer = 0;
}



/*===========================================================================
FUNCTION  tm_cacheWifiScanReports

DESCRIPTION
  Cache wifi scan reports. They are stored in tm_lppe_WifiInfoListType data structire.
  The cached reports can be retrieved using the tm_lppe_cacheWifiScanReportsGet API.

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
void tm_cacheWifiScanReports
(
   pdsm_wifi_on_demand_info_list_s_type z_wifiScanResult,
   tm_auxTechReqId e_reqId
   )
{
  /* Structure that holds the new(non-matching) wifi ap indices (to given scan results) */
  typedef struct {
    uint32 q_numWifiAps;     /* Number of non-matching wifi aps */
    uint32 *pq_wifiApsIndex; /* Array that holds the wifi ap indices*/
  } wifi_aps_index_list;

  uint32 q_i, q_j,     /* scratch variables */
     q_total_aps,  /* total number of aps after the new scan is cached */
     q_last_ap,
  /* Old and new AP cache size in bytes */
     q_old_ap_cache_size, q_new_ap_cache_size;
  wifi_aps_index_list z_wifiApsNewList;
  boolean v_matchFound;
  uint8_t *pu_apMacAddress;
  tm_lppe_WifiApInfoType *wifiInfoTempMem;
  time_type p_time_ms  = { 0, 0 };
  time_type p_time_ms_cpy  = { 0, 0 };
  tm_lppe_WifiInfoListType *p_cached_wifi_scan = &z_sessioninfo.z_cache_wifi_scan_result;

  /* If Wifi scan is not running, i.e tm_stopOnDemandWifiScan was requested before, do not cache
     Or LPPE CPI requestes to get free WIFI scans not requested do not cache */
  if ((FALSE == z_sessioninfo.v_reqWifiScan) &&
      (TM_AUXTECH_REQ_ID_LPPE != tm_CpiReqIdGet()))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "WiFi scan or LPPE 911 CPI not requested. So not caching ");
    return;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Cache Wifi scan reports. ReqId %d", e_reqId);

  /* Request for on-demand wifi scans immediately if the report doesn't indicate WiFi status as
     Off. If WiFi status is Off, request after a second */
  if (TRUE == z_wifiScanResult.v_on_demand_scan)
  {
    if ((eLOC_ENG_WLAN_AP_ERR_WIFI_OFF_V01 == z_wifiScanResult.e_errorCause) &&
        (0 == z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps))
    {
      /* Request on-demand wifi scan after a second */
      auxtech_start_wifi_on_demand_req_timer(WIFI_ON_DEMAND_REQ_TIME_INTERVAL_MSEC);
    }
    else if ((TM_AUXTECH_REQ_ID_SUPLWLAN == e_reqId) ||
             (TM_AUXTECH_REQ_ID_SUPLMLID == e_reqId))
    {
      /* IF WIFI scans were requested for SUPLMLID or SUPLWLAN stop the WIFI injection*/
      tm_resetOnDemandWifiScanReq(e_reqId);
    }
    else
    {
      tm_reqOnDemandWifiScan(TRUE, e_reqId, z_sessioninfo.l_wifiClientId);
    }
  }

  /* If no reports are present, just return. */
  if (0 == z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "WiFi scan results empty");

    /* If cache is empty, copy the error cause */
    if ((0 == p_cached_wifi_scan->q_numWifiAps) &&
        (eLOC_ENG_WLAN_AP_ERR_NONE_V01 != z_wifiScanResult.e_errorCause))
    {
      p_cached_wifi_scan->e_errorCause = z_wifiScanResult.e_errorCause;
    }
    return;
  }

  /* If the wifi info pointers are NULL, thrown an error. */
  if ((NULL == z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_ptr) ||
      (NULL == z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Wifi AP info is NULL. Number of wifi aps = ",
          p_cached_wifi_scan->q_numWifiAps, 0, 0);
    return;
  }

  /* Get the Modem time in ms*/
  time_get_uptime_ms(p_time_ms);

  /*Copy UE WIFI Mac address*/
  (void)memscpy(&p_cached_wifi_scan->ue_mac_address, sizeof(p_cached_wifi_scan->ue_mac_address),
                &z_wifiScanResult.z_wifi_ap_scan_info.ue_mac_address, sizeof(z_wifiScanResult.z_wifi_ap_scan_info.ue_mac_address));

  /* Wifi result cache is empty. Just copy the scan results to the cache */
  if (0 == p_cached_wifi_scan->q_numWifiAps)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Empty cache! num new aps = %d ", z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps, 0, 0);
    /* Check for error cause. If so, update it and return */
    if (eLOC_ENG_WLAN_AP_ERR_NONE_V01 != z_wifiScanResult.e_errorCause)
    {
      p_cached_wifi_scan->e_errorCause = z_wifiScanResult.e_errorCause;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "Update error cause %d for empty cache",
            p_cached_wifi_scan->e_errorCause);
      return;
    }

    p_cached_wifi_scan->e_errorCause = eLOC_ENG_WLAN_AP_ERR_NONE_V01;
    q_new_ap_cache_size = sizeof(*p_cached_wifi_scan->pz_wifiApInfo) *
       (z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps);

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "new ap cache size = %d", q_new_ap_cache_size, 0, 0);

    /* Allocate memory */
    p_cached_wifi_scan->pz_wifiApInfo =
       (tm_lppe_WifiApInfoType *)os_MemAlloc(q_new_ap_cache_size,
                                             OS_MEM_SCOPE_TASK);

    if (NULL == p_cached_wifi_scan->pz_wifiApInfo)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory", 0, 0, 0);
      return;
    }

    p_cached_wifi_scan->q_numWifiAps = z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps;

    /* Copy the wifi ap info the the cache. */
    for (q_i = 0; q_i < z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps; ++q_i)
    {
      p_cached_wifi_scan->pz_wifiApInfo[q_i].z_wifi_ap_info     =  z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_ptr[q_i];
      p_cached_wifi_scan->pz_wifiApInfo[q_i].z_wifi_ap_add_info = z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i];
      /*Use a copy of current time to avoid decreasing current time for each iteration.
       if this is not done current time will get decremented in each iteration 
       and so current time will keep changing for each iteration. But current time
       should remain same while processing data of all APs
     */
      qw_equ(p_time_ms_cpy, p_time_ms);

      qw_dec(p_time_ms_cpy, z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i].measAge);
      /* Copy the time stamp as well */
      memscpy(p_cached_wifi_scan->pz_wifiApInfo[q_i].p_timestamp_ms,
              sizeof(p_cached_wifi_scan->pz_wifiApInfo[q_i].p_timestamp_ms),
              p_time_ms_cpy, sizeof(p_time_ms_cpy));

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "All -new APs Curr Time %d %d  meas-age = %d", qw_lo(p_time_ms_cpy), qw_hi(p_time_ms_cpy),
            z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i].measAge
            );
    }
    return;
  }

  /* If error cause is not none, dont update the cache */
  if (eLOC_ENG_WLAN_AP_ERR_NONE_V01 != z_wifiScanResult.e_errorCause)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "WiFi Scan Error %d. Keep existing APs", z_wifiScanResult.e_errorCause);
    return;
  }

  /* Parse all the results to check if it is some are already present.
     If so, overwrite the cached record with the new one.
     If not, add to the new wifi aps list */

  /* A new data strcuture wifi_aps_index_list is used that holds the index of new wifi aps. */
  z_wifiApsNewList.q_numWifiAps = 0;
  /* Allocate memory equal to max of z_wifiScanResult.q_num_wifi_aps (in case all are new)*/
  z_wifiApsNewList.pq_wifiApsIndex =
     (uint32 *)os_MemAlloc(
     z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps * sizeof(uint32),
     OS_MEM_SCOPE_TASK);

  if (NULL == z_wifiApsNewList.pq_wifiApsIndex)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory for the new structure", 0, 0, 0);
    return;
  }

  /* For each element in the new wifi ap list*/
  for (q_i = 0; q_i < z_wifiScanResult.z_wifi_ap_scan_info.q_num_wifi_aps; ++q_i)
  {
    pu_apMacAddress = z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_ptr[q_i].mac_address;
    v_matchFound = FALSE;

    /* Search through the existing cache*/
    for (q_j = 0; q_j < p_cached_wifi_scan->q_numWifiAps; ++q_j)
    {
      /* If the wifi is aleady present in the cache (match) */
      if (0 == (memcmp(pu_apMacAddress, (p_cached_wifi_scan->pz_wifiApInfo[q_j].z_wifi_ap_info.mac_address),
                       sizeof(p_cached_wifi_scan->pz_wifiApInfo[q_j].z_wifi_ap_info.mac_address))))
      {
        /* Overwrite the existing cached ap values with the new ap results */
        p_cached_wifi_scan->pz_wifiApInfo[q_j].z_wifi_ap_info     = z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_ptr[q_i];
        p_cached_wifi_scan->pz_wifiApInfo[q_j].z_wifi_ap_add_info = z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i];
        qw_equ(p_time_ms_cpy, p_time_ms);
        qw_dec(p_time_ms_cpy, z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i].measAge);
        /* Copy the time stamp as well */
        memscpy(p_cached_wifi_scan->pz_wifiApInfo[q_j].p_timestamp_ms,
                sizeof(p_cached_wifi_scan->pz_wifiApInfo[q_j].p_timestamp_ms),
                p_time_ms_cpy, sizeof(p_time_ms_cpy));
        v_matchFound = TRUE;

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Cached APs Curr Time %d %d  meas-age = %d", qw_lo(p_time_ms_cpy), qw_hi(p_time_ms_cpy),
              z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i].measAge);
        break;
      }
    }
    /* If wifi ap is new to the cache.*/
    if (FALSE == v_matchFound)
    {
      /* New ap. Add the z_wifiScanResult index in z_wifiApsNewList.wifi_aps_index_ptr . */
      z_wifiApsNewList.pq_wifiApsIndex[z_wifiApsNewList.q_numWifiAps] = q_i;
      z_wifiApsNewList.q_numWifiAps++;
    }
  }

  /* if no new aps are found, dont do anything*/
  if (0 == z_wifiApsNewList.q_numWifiAps)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "No New APs!!", 0, 0, 0);
    os_MemFree((void **)&(z_wifiApsNewList.pq_wifiApsIndex));
    return;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Found %d new APs", z_wifiApsNewList.q_numWifiAps, 0, 0);

  /* New Aps are found!!! */
  q_old_ap_cache_size = sizeof(p_cached_wifi_scan->pz_wifiApInfo[0]) *
     p_cached_wifi_scan->q_numWifiAps;
  q_total_aps = p_cached_wifi_scan->q_numWifiAps + z_wifiApsNewList.q_numWifiAps;
  q_new_ap_cache_size = sizeof(p_cached_wifi_scan->pz_wifiApInfo[0]) * q_total_aps;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Old Cache size %d New size %d total aps %d", q_old_ap_cache_size, q_new_ap_cache_size, q_total_aps);

  /* Increase the size in cache */
  wifiInfoTempMem = (tm_lppe_WifiApInfoType *)os_MemAlloc(q_new_ap_cache_size,
                                                          OS_MEM_SCOPE_TASK);

  if (NULL == wifiInfoTempMem)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory", 0, 0, 0);
    os_MemFree((void **)&(z_wifiApsNewList.pq_wifiApsIndex));
    return;
  }

  memscpy(wifiInfoTempMem, q_new_ap_cache_size,
          p_cached_wifi_scan->pz_wifiApInfo, q_old_ap_cache_size);
  os_MemFree((void **)&p_cached_wifi_scan->pz_wifiApInfo);
  p_cached_wifi_scan->pz_wifiApInfo = wifiInfoTempMem;

  q_last_ap = p_cached_wifi_scan->q_numWifiAps;

  /* Add the new ap results to the cache wifi.*/
  for (q_i = 0; q_i < z_wifiApsNewList.q_numWifiAps; ++q_i)
  {
    uint32  q_new_ap_index = z_wifiApsNewList.pq_wifiApsIndex[q_i];

    p_cached_wifi_scan->pz_wifiApInfo[q_last_ap + q_i].z_wifi_ap_info =
       z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_ptr[q_new_ap_index];
    p_cached_wifi_scan->pz_wifiApInfo[q_last_ap + q_i].z_wifi_ap_add_info =
       z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_new_ap_index];

    qw_equ(p_time_ms_cpy, p_time_ms);
    qw_dec(p_time_ms_cpy, z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i].measAge);
    /* Copy the time stamp as well */
    memscpy(p_cached_wifi_scan->pz_wifiApInfo[q_last_ap + q_i].p_timestamp_ms,
            sizeof(p_cached_wifi_scan->pz_wifiApInfo[q_last_ap + q_i].p_timestamp_ms),
            p_time_ms_cpy, sizeof(p_time_ms_cpy));
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "New APs Curr Time %d %d  meas-age = %d", qw_lo(p_time_ms_cpy), qw_hi(p_time_ms_cpy),
          z_wifiScanResult.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i].measAge
          );
  }

  /* Update the number of wifis in the cache. */
  p_cached_wifi_scan->q_numWifiAps = p_cached_wifi_scan->q_numWifiAps + z_wifiApsNewList.q_numWifiAps;


  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "New cache size = %d APs", p_cached_wifi_scan->q_numWifiAps, 0, 0);

  /* Deallocate the memory created for the temporary z_wifiApsNewList.wifi_aps_index_ptr data structure. */
  os_MemFree((void **)&(z_wifiApsNewList.pq_wifiApsIndex));
}

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
void tm_cacheWifiScanReportsInit()
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Init wifi scan report cache");
  /* Free the existing cache memory  */
  if (NULL != z_sessioninfo.z_cache_wifi_scan_result.pz_wifiApInfo)
    os_MemFree((void **)&(z_sessioninfo.z_cache_wifi_scan_result.pz_wifiApInfo));

  z_sessioninfo.z_cache_wifi_scan_result.q_numWifiAps = 0;
  z_sessioninfo.z_cache_wifi_scan_result.pz_wifiApInfo = NULL;
  z_sessioninfo.z_cache_wifi_scan_result.e_errorCause = eLOC_ENG_WLAN_AP_ERR_UNKNOWN_V01;

  z_sessioninfo.v_reqWifiScan = FALSE;
  z_sessioninfo.e_wifiReqId = TM_AUXTECH_REQ_ID_NONE;
  z_sessioninfo.l_wifiClientId = -1;
}

/*  */
/*===========================================================================
FUNCTION  tm_cacheWifiScanReportsSort

DESCRIPTION
  Sort the z_sessioninfo.z_cache_wifi_scan_result.pz_wifiApInfo array.
  Here the sort algorithm is done only for the first q_numWifiAps values.

  The algorithm is as follows:
    a. For ith run, indices from i+1 till the end of array are searched element bigger than ith element.
           If so, swap ith element with the biggest value.
       So, after the first run, the biggest element in the array is put at the first place
       For after the second run, second biggest element is put at second place

    b. Do this q_numWifiAps times starting from the first element.

PARAMETERS
  q_numWifiAps - Maximum number of wifi ap reports needed

DEPENDENCIES
   Assumes that q_numWifiAps <= the size of the array.

RETURN VALUE
  none
SIDE EFFECTS
  none
===========================================================================*/

static boolean tm_cacheWifiScanReportsSort(uint32 q_numWifiAps)
{
  uint32 q_i, q_j, q_i_big;
  int32_t l_ith_big_val;
  uint32 q_storedNumWifiAps = z_sessioninfo.z_cache_wifi_scan_result.q_numWifiAps;
  tm_lppe_WifiApInfoType *pz_wifiApInfo = z_sessioninfo.z_cache_wifi_scan_result.pz_wifiApInfo;

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sort %lu WiFi cached scan report. Max %lu",
        q_storedNumWifiAps, q_numWifiAps);

  /* If the number of elements to be sorted is more than the size of the array, exit.*/
  if (q_numWifiAps > q_storedNumWifiAps)
  {
    return FALSE;
  }

  if (pz_wifiApInfo == NULL)
  {
    return FALSE;
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Begin sorting cache WiFi scan reports");

  /* Sort values based on rssi value. Do this for the first requested q_numWifiAps times */

  for (q_i = 0; q_i < q_numWifiAps; q_i++)
  {
    /* Assume that rssi at q_i is the bigest value between indices [ q_i to q_storedNumWifiAps] of the array */
    l_ith_big_val = pz_wifiApInfo[q_i].z_wifi_ap_info.ap_rssi;
    q_i_big = 0;

    /* Find the biggest index between indices [ (q_i+1) to q_storedNumWifiAps] of the array  */
    for (q_j = q_i + 1; q_j < q_storedNumWifiAps; q_j++)
    {
      /* If biggest is found that the existing one, update it.*/
      if (pz_wifiApInfo[q_j].z_wifi_ap_info.ap_rssi > l_ith_big_val)
      {
        q_i_big = q_j;
        l_ith_big_val = pz_wifiApInfo[q_j].z_wifi_ap_info.ap_rssi;
      }
    }
    /* If index with biggest rssi value is found, swap the vals. */
    if (0 != q_i_big)
    {
      tm_lppe_WifiApInfoType z_tempWifiApVal = pz_wifiApInfo[q_i_big];
      pz_wifiApInfo[q_i_big] = pz_wifiApInfo[q_i];
      pz_wifiApInfo[q_i] = z_tempWifiApVal;
    }
  }

  return TRUE;
}

/*===========================================================================
FUNCTION  tm_lppe_cacheWifiScanReportsGet

DESCRIPTION
  Get the cached wifi reports. 
  If the number of reports are more than MAX_CACHED_WLAN_AP_SIZE,
  the wifi ap list is adjusted such that the first q_numWifiAps have the bigger rssi values in descending order.

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

tm_lppe_WifiInfoListType* tm_cacheWifiScanReportsGet(void)
{
  char scans[100];

  int32 num_aps = z_sessioninfo.z_cache_wifi_scan_result.q_numWifiAps;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Get cached Wifi scan reports");

  /* If number of wifi aps are more than MAX_CACHED_WLAN_AP_SIZE and if rssi is valid in wifi aps, sort them */
  if (z_sessioninfo.z_cache_wifi_scan_result.q_numWifiAps > MAX_CACHED_WLAN_AP_SIZE)
  {
    tm_cacheWifiScanReportsSort(MAX_CACHED_WLAN_AP_SIZE);
  }

  (void)snprintf(
     scans, sizeof(scans),
     "ssid InfMask mac_ad    TxPwr AntGain SNR DevType RSSI Chnl RTT RTTUx MobSNR MobRSSI HighResRSSI");

  MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "%s", scans);


  while (num_aps > 0)
  {
    tm_lppe_WifiApInfoType *pz_wifiApInfo =
       &z_sessioninfo.z_cache_wifi_scan_result.pz_wifiApInfo[--num_aps];

    snprintf(scans, sizeof(scans), "%s 0x%lx %x:%x:%x:%x:%x:%x %ld %ld %ld %d %ld %d %ld %d %d %ld %ld %ld",
             pz_wifiApInfo->z_wifi_ap_add_info.ssid,
             pz_wifiApInfo->z_wifi_ap_info.wifiInfoMask,
             pz_wifiApInfo->z_wifi_ap_info.mac_address[5],
             pz_wifiApInfo->z_wifi_ap_info.mac_address[4],
             pz_wifiApInfo->z_wifi_ap_info.mac_address[3],
             pz_wifiApInfo->z_wifi_ap_info.mac_address[2],
             pz_wifiApInfo->z_wifi_ap_info.mac_address[1],
             pz_wifiApInfo->z_wifi_ap_info.mac_address[0],
             pz_wifiApInfo->z_wifi_ap_info.ap_transmit_power,
             pz_wifiApInfo->z_wifi_ap_info.ap_antenna_gain,
             pz_wifiApInfo->z_wifi_ap_info.ap_signal_to_noise,
             pz_wifiApInfo->z_wifi_ap_info.ap_device_type,
             pz_wifiApInfo->z_wifi_ap_info.ap_rssi,
             pz_wifiApInfo->z_wifi_ap_info.ap_channel,
             pz_wifiApInfo->z_wifi_ap_info.ap_round_trip_delay,
             pz_wifiApInfo->z_wifi_ap_info.ap_round_trip_delay_unit,
             pz_wifiApInfo->z_wifi_ap_info.ap_round_trip_delay_accuracy,
             pz_wifiApInfo->z_wifi_ap_info.mobile_signal_to_noise,
             pz_wifiApInfo->z_wifi_ap_info.mobile_rssi,
             pz_wifiApInfo->z_wifi_ap_add_info.apHighResolutionRssi
             );
    MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "%s", scans);
  }
  return &(z_sessioninfo.z_cache_wifi_scan_result);
}

#ifdef FEATURE_GNSS_LPPE
/**************************** SRN APIS **************************/

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
                   boolean v_request)
{
  tm_pdapi_pd_event_s_type *pz_eventInfo;

  if ((TM_AUXTECH_REQ_ID_SUPLMLID >= e_reqId) ||
      (TM_AUXTECH_REQ_ID_LPPE < e_reqId))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "AuxTech Requestpr ID not supported %u", e_reqId);
    return;
  }

  /* Allocate memory */
  pz_eventInfo = (tm_pdapi_pd_event_s_type *)os_MemAlloc(sizeof(tm_pdapi_pd_event_s_type), OS_MEM_SCOPE_TASK);

  if (NULL == pz_eventInfo)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory. So, not sending wifi scan request ", 0, 0, 0);
    return;
  }

  memset(pz_eventInfo, 0, sizeof(*pz_eventInfo));

  pz_eventInfo->e_pd_event = PDSM_PD_EVENT_REQUEST_SRN_INFO_INJECTION;
  pz_eventInfo->pd_event_data.pd_info_type.client_id = l_clientId;

  /*Indicate Emergency Mode*/
  pz_eventInfo->pd_event_data.pd_info_type.pd_info.z_srnScanReq.e911Mode_valid = TRUE;
  pz_eventInfo->pd_event_data.pd_info_type.pd_info.z_srnScanReq.e911Mode = v_emergencyMode;

  pz_eventInfo->pd_event_data.pd_info_type.pd_info.z_srnScanReq.srnRequest = v_request;
  /* Request BlueTooth LE scans*/
  pz_eventInfo->pd_event_data.pd_info_type.pd_info.z_srnScanReq.srnTechMask = LOC_ENG_SRN_AP_DATA_TECH_TYPE_BTLE_V01;

  /*Indicate the MAC address type PUBLIC/PRIVATE*/
  pz_eventInfo->pd_event_data.pd_info_type.pd_info.z_srnScanReq.srnApMacAddrType_valid = TRUE;
  pz_eventInfo->pd_event_data.pd_info_type.pd_info.z_srnScanReq.srnApMacAddrType = eLOC_ENG_SRN_AP_DATA_PUBLIC_MAC_ADDR_V01;

  tm_pdapi_pd_event_callback(pz_eventInfo, TM_PDAPI_SESS_TYPE_NONE);

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SRN scan request posted with requestor id %d, request = %u", e_reqId, v_request);

  if (TRUE == v_request)
  {
    z_sessioninfo.e_srnReqId = e_reqId;
  }
  else
  {
    z_sessioninfo.e_srnReqId = TM_AUXTECH_REQ_ID_NONE;
  }

  os_MemFree((void **)&pz_eventInfo);
}

/*===========================================================================
FUNCTION  tm_cacheSrnScanReports

DESCRIPTION
  Cache SRN BTLE scan reports.
  They are stored in tm_lppe_srnApInfoListType data structure. The cached reports 
  can be retrieved using the tm_cacheSrnScanReportsGet API.


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
void tm_cacheSrnScanReports(pdsm_srn_scan_info_list_s_type z_srnScanResult, tm_auxTechReqId e_reqId)
{

  uint32 q_i = 0,     /* scratch variables */
     q_new_ap_cache_size;

  tm_lppe_SrnInfoListType *p_cached_srn_scan = &z_sessioninfo.z_cache_srn_scan_result;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Cache SRN scan reports. ReqId %d", e_reqId);

  /* If Srn scan is not requested by any LPPE client, do not cache */
  if ((TM_AUXTECH_REQ_ID_SUPLMLID >= e_reqId) ||
      (TM_AUXTECH_REQ_ID_LPPE < e_reqId))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Srn scan not cached, incorrect ReqID");
    return;
    /*TODO: check if this can occur when lppe session has closed and we receive a scan report*/
  }

  /* If no reports are present, just return. */
  if (0 == z_srnScanResult.z_srn_ap_scan_info.q_num_srn_aps)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Srn scan results empty");

    /* If cache is empty, copy the error cause */
    if ((0 == p_cached_srn_scan->q_numSrnAps) &&
        (eLOC_ENG_SRN_AP_ERR_NONE_V01 != z_srnScanResult.e_errorCause))
    {
      p_cached_srn_scan->e_errorCause = z_srnScanResult.e_errorCause;
    }
    return;
  }

  /* If the Srn info pointer is NULL, thrown an error. */
  if (NULL == z_srnScanResult.z_srn_ap_scan_info.srn_ap_ptr)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Srn AP info is NULL. Number of Srn aps = ",
          p_cached_srn_scan->q_numSrnAps);
    return;
  }

  /* Srn result cache is empty. Just copy the scan results to the cache */
  if (0 == p_cached_srn_scan->q_numSrnAps)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Empty cache! num new Srn aps = %d ", z_srnScanResult.z_srn_ap_scan_info.q_num_srn_aps);
    /* Check for error cause. If so, update it and return */
    if (eLOC_ENG_SRN_AP_ERR_NONE_V01 != z_srnScanResult.e_errorCause)
    {
      p_cached_srn_scan->e_errorCause = z_srnScanResult.e_errorCause;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "Update error cause %d for empty cache",
            p_cached_srn_scan->e_errorCause);
      return;
    }

    p_cached_srn_scan->e_errorCause = eLOC_ENG_SRN_AP_ERR_NONE_V01;
    q_new_ap_cache_size = sizeof(*p_cached_srn_scan->pz_srnApinfo) *
       (MAX_CACHED_SRN_AP_SIZE);

    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "new ap cache size = %d", q_new_ap_cache_size);

    /* Allocate memory */
    p_cached_srn_scan->pz_srnApinfo =
       (tm_lppe_SrnApInfoType *)os_MemAlloc(q_new_ap_cache_size,
                                            OS_MEM_SCOPE_TASK);

    if (NULL == p_cached_srn_scan->pz_srnApinfo)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory");
      return;
    }

    p_cached_srn_scan->q_numSrnAps = z_srnScanResult.z_srn_ap_scan_info.q_num_srn_aps;

    /* Copy the wifi ap info the the cache. */
    for (q_i = 0; q_i < z_srnScanResult.z_srn_ap_scan_info.q_num_srn_aps; ++q_i)
    {
      p_cached_srn_scan->pz_srnApinfo[q_i].z_srn_ap_info = z_srnScanResult.z_srn_ap_scan_info.srn_ap_ptr[q_i];
      /*TODO check for Time stamp conversion*/
    }
    return;
  }

  /* If error cause is not none, dont update the cache */
  if (eLOC_ENG_SRN_AP_ERR_NONE_V01 != z_srnScanResult.e_errorCause)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "Srn Scan Error %d. Keep existing APs", z_srnScanResult.e_errorCause);
    return;
  }

  if (0 != p_cached_srn_scan->q_numSrnAps)
  { /*over write the scans we have in the cache with the latest one
      TODO, need to check for existing and update it only*/
    for (q_i = 0; q_i < z_srnScanResult.z_srn_ap_scan_info.q_num_srn_aps; ++q_i)
    {
      p_cached_srn_scan->pz_srnApinfo[q_i].z_srn_ap_info = z_srnScanResult.z_srn_ap_scan_info.srn_ap_ptr[q_i];
      /*TODO check for Time stamp conversion*/
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,
        "Srn Scan Error %d. Keep existing APs");
  }
  return;
}

/*===========================================================================
FUNCTION  tm_auxTechSrnScanReportsInit

DESCRIPTION
  Initialize SRN BTLE scan report cache. This frees up the memory allocated for the cache.

DEPENDENCIES
  None
RETURN VALUE
   None
SIDE EFFECTS
   None
===========================================================================*/
void tm_auxTechSrnScanReportsInit()
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Init Srn scan report cache");
  z_sessioninfo.z_cache_srn_scan_result.q_numSrnAps  = 0;
  z_sessioninfo.z_cache_srn_scan_result.pz_srnApinfo = NULL;
  z_sessioninfo.z_cache_srn_scan_result.e_errorCause = eLOC_ENG_SRN_AP_ERR_UNKNOWN_V01;

  z_sessioninfo.e_srnReqId = TM_AUXTECH_REQ_ID_NONE;
}

/*===========================================================================
FUNCTION  tm_cacheSrnScanReportsSort

DESCRIPTION
  Sort the z_sessioninfo.z_cache_srn_scan_result.pz_srnApInfo array.

  The algorithm is as follows:
    a. For ith run, indices from i+1 till the end of array are searched element bigger than ith element.
           If so, swap ith element with the biggest value.
       So, after the first run, the biggest element in the array is put at the first place
       For after the second run, second biggest element is put at second place

    b. Do this q_numSrnAps times starting from the first element.

PARAMETERS
  q_numSrnAps - Maximum number of Srn ap reports needed

DEPENDENCIES
   Assumes that q_numSrnAps <= the size of the array.

RETURN VALUE
  none
SIDE EFFECTS
  none
===========================================================================*/

static boolean tm_cacheSrnScanReportsSort(uint32 q_numSrnAps)
{
  uint32 q_i, q_j, q_i_big;
  int32_t l_ith_big_val;
  uint32 q_storedNumAps = z_sessioninfo.z_cache_srn_scan_result.q_numSrnAps;
  tm_lppe_SrnApInfoType *pz_srnApInfo = z_sessioninfo.z_cache_srn_scan_result.pz_srnApinfo;

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sort %lu Srn cached scan report. Max %lu",
        q_storedNumAps, q_numSrnAps);

  /* If the number of elements to be sorted is more than the size of the array, exit.*/
  if (q_numSrnAps > q_storedNumAps)
  {
    return FALSE;
  }

  if (pz_srnApInfo == NULL)
  {
    return FALSE;
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Begin sorting cache Srn scan reports");

  /* Sort values based on rssi value. Do this for the first requested q_numWifiAps times */

  for (q_i = 0; q_i < q_numSrnAps; q_i++)
  {
    /* Assume that rssi at q_i is the bigest value between indices [ q_i to q_storedNumWifiAps] of the array */
    l_ith_big_val = pz_srnApInfo[q_i].z_srn_ap_info.apSrnRssi;
    q_i_big = 0;

    /* Find the biggest index between indices [ (q_i+1) to q_storedNumAps] of the array  */
    for (q_j = q_i + 1; q_j < q_storedNumAps; q_j++)
    {
      /* If biggest is found that the existing one, update it.*/
      if (pz_srnApInfo[q_j].z_srn_ap_info.apSrnRssi > l_ith_big_val)
      {
        q_i_big = q_j;
        l_ith_big_val = pz_srnApInfo[q_j].z_srn_ap_info.apSrnRssi;
      }
    }
    /* If index with biggest rssi value is found, swap the vals. */
    if (0 != q_i_big)
    {
      tm_lppe_SrnApInfoType z_tempSrnApVal = pz_srnApInfo[q_i_big];
      pz_srnApInfo[q_i_big] = pz_srnApInfo[q_i];
      pz_srnApInfo[q_i] = z_tempSrnApVal;
    }
  }

  return TRUE;
}
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
tm_lppe_SrnInfoListType* tm_cacheSrnScanReportsGet()
{
  char scans[100];

  int32 num_aps = z_sessioninfo.z_cache_srn_scan_result.q_numSrnAps;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Get cached Srn BTLE scan reports");

  /* If number of Srn aps are more than MAX_CACHED_WLAN_AP_SIZE and if rssi is valid in wifi aps, sort them */
  if (z_sessioninfo.z_cache_srn_scan_result.q_numSrnAps > MAX_CACHED_SRN_AP_SIZE)
  {
    tm_cacheSrnScanReportsSort(MAX_CACHED_SRN_AP_SIZE);
  }

  (void)snprintf(
     scans, sizeof(scans),
     "InfMask mac_ad RSSI");

  MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "%s", scans);

  while (num_aps > 0)
  {
    tm_lppe_SrnApInfoType *pz_srnApInfo =
       &z_sessioninfo.z_cache_srn_scan_result.pz_srnApinfo[--num_aps];

    snprintf(scans, sizeof(scans), "0x%lx %x:%x:%x:%x:%x:%x %ld %lld",
             pz_srnApInfo->z_srn_ap_info.srnApDataMask,
             pz_srnApInfo->z_srn_ap_info.macAddress[5],
             pz_srnApInfo->z_srn_ap_info.macAddress[4],
             pz_srnApInfo->z_srn_ap_info.macAddress[3],
             pz_srnApInfo->z_srn_ap_info.macAddress[2],
             pz_srnApInfo->z_srn_ap_info.macAddress[1],
             pz_srnApInfo->z_srn_ap_info.macAddress[0],
             pz_srnApInfo->z_srn_ap_info.apSrnRssi,
             pz_srnApInfo->z_srn_ap_info.apSrnTimestamp);
    MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "%s", scans);
  }
  return &(z_sessioninfo.z_cache_srn_scan_result);
}

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
tm_auxTechReqId tm_SrnReqIdGet(void)
{
  return z_sessioninfo.e_srnReqId;
}

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
tm_lppe_sensorInfoType* tm_cacheSensorBaroMeasReportGet()
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Get cached Sensor Barometer Pressure Measurement");

  techSel_GetSensorBaroMeas(&(z_sessioninfo.z_cache_baro_meas_result));

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Barometer Pressure : %d, error %d",
        (uint32)z_sessioninfo.z_cache_baro_meas_result.f_BaroPressure, (uint32)z_sessioninfo.z_cache_baro_meas_result.e_errorCause);

  return &(z_sessioninfo.z_cache_baro_meas_result);
}

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
void tm_reqSensorBaroMeas(tm_auxTechReqId e_reqId,
                          boolean v_request)
{
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_reqSensorBaroMeas, e_reqId:%d, v_request:%d",
        e_reqId, v_request);
  if (FALSE == techSel_SlimReqSensorMeas(TECH_SEL_SENSOR_CLIENT_MASK_LPP, eSLIM_SENSOR_TYPE_BAROMETER,
                                         1, v_request))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sensor Baro Enable/Disable failed");
    return;
  }

  if (TRUE == v_request)
  {
    z_sessioninfo.e_ubpReqId = e_reqId;
  }
  else
  {
    z_sessioninfo.e_ubpReqId = TM_AUXTECH_REQ_ID_NONE;
  }

  return;
}

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
tm_auxTechReqId tm_wifiReqIdGet(void)
{
  return z_sessioninfo.e_wifiReqId;
}

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
pdsm_client_id_type tm_wifiClientIdGet(void)
{
  return z_sessioninfo.l_wifiClientId;
}

/*===========================================================================
FUNCTION  tm_startCPIRequest

DESCRIPTION
  Request to start CPI .

PARAMETERS
  v_emergencyMode Is Emergency mode.
  e_reqId Requestor Id

DEPENDENCIES
  None
RETURN VALUE
  TRUE if start is successful
  FALSE if start failed

SIDE EFFECTS
   None
===========================================================================*/
boolean tm_startCPIRequest(boolean v_emergencyMode, tm_auxTechReqId e_reqId)
{
  techSel_CpiRequest z_CpiRequest = {0};
  MSG_2(MSG_SSID_GPSSM,MSG_LEGACY_MED,"Start CPI request. ReqId %d EmergencyMode %d", 
                                       e_reqId, v_emergencyMode );
  
  if( TM_AUXTECH_REQ_ID_NONE == e_reqId)
  {
    return FALSE;
  }

  /* If ODCPI is already started by some client, just update the ownership */
  if (z_sessioninfo.e_cpiReqId != TM_AUXTECH_REQ_ID_NONE)
  {
    z_sessioninfo.e_cpiReqId = e_reqId;
    return (TRUE);
  }
  //Below condition is to Block CPI request due to MGP error recovery in non emergency when ODCPI backoff timer is running.
  if((FALSE == v_emergencyMode) && (TM_AUXTECH_REQ_ID_RECOVERY == e_reqId) && (TRUE == techSel_isCpiBlocked()))
  {
    MSG(MSG_SSID_GPSSM,MSG_LEGACY_MED,"ODCPI Backoff timer in progress, CPI request due to Recovery is blocked");
    return FALSE;
  }

  z_CpiRequest.v_EmergencyMode  = v_emergencyMode;

  if (TRUE == techSel_StartCpi(&z_CpiRequest, &(z_sessioninfo.q_cpiTechSelRequestId)))
  {
    MSG_2(MSG_SSID_GPSSM,MSG_LEGACY_MED,"Start CPI request. ReqId %d EmergencyMode %d", 
                                       e_reqId, v_emergencyMode );

    z_sessioninfo.e_cpiReqId = e_reqId;
    return TRUE;
  }
  return FALSE;
}

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
boolean tm_stopCPI(tm_auxTechReqId e_reqId)
{
#ifndef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
  /* If the protocol requesting stop is
     1) LPPE or EMERGENCY_ORIG, then the protocol LPPE_CP/UP was not triggered
         and hence did not end up using the DBH, clear the cache.
     2) LPPE_CP or LPPE_UP, this means protocol used DBH clear the cache.
 */
  if ((e_reqId == z_sessioninfo.e_cpiReqId) ||
      (e_reqId == TM_AUXTECH_REQ_ID_LPPE_UP) ||
      (e_reqId == TM_AUXTECH_REQ_ID_LPPE_CP))
  {
    /*If CPI injection is used as DBH, ensure
    1)on session completion the cache is invalidated. 
    2)Free the cached CPI SM fix Report*/
    z_CacheCPIPos.v_CachedPosValid = FALSE;
    os_MemFree((void **)&z_CacheCPIPos.z_CachedPos);
  }
#endif

  /* Place ODCPI stop request if ODCPI start operation is successful through AuxTech */
  if (z_sessioninfo.e_cpiReqId != TM_AUXTECH_REQ_ID_NONE)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Stop CPI. ReqId %d", e_reqId);
    z_sessioninfo.e_cpiReqId = TM_AUXTECH_REQ_ID_NONE;
    return (techSel_StopCpi(z_sessioninfo.q_cpiTechSelRequestId));
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "ODCPI is not started through AT");
  }
  return (FALSE);
}

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
tm_auxTechReqId tm_CpiReqIdGet(void)
{
  return z_sessioninfo.e_cpiReqId;
}

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
boolean tm_DBHPositionGet(tm_auxTechReqId e_reqId)
{
  tm_core_get_gps_state_param_s_type z_get_gps_state_param;
  sm_FixReportStructType z_CPIPos = { 0 };

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Get DBH position. ReqId %d", e_reqId);

  if (TM_AUXTECH_REQ_ID_NONE == e_reqId)
  {
    return FALSE;
  }

  z_get_gps_state_param.source = SM_GPS_STATE_INFO_REQ_TYPE_GET_DBH;
  z_get_gps_state_param.cmd.p_cmd_type = NULL;

  /*For MSM targets the CPI injected POS is directly used for DBH 
    We only need the requestor ID to be stored here so that
    tm_DBHPositionHandleReportGpsStateInfo() can send the CPI injected
    position to the correct protocol*/
#ifdef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
  #error code not present
#endif

  z_sessioninfo.e_dbhReqId = e_reqId;

#ifndef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
  /* In the event CPI was injections were cached for DBH.
     Push the cached position to the correct DBH requestor.
     DBH requestor for the current session is note above*/
  if (TRUE == z_CacheCPIPos.v_CachedPosValid)
  {
    memscpy(&z_CPIPos, sizeof(sm_FixReportStructType),
            (sm_FixReportStructType *)z_CacheCPIPos.z_CachedPos, sizeof(sm_FixReportStructType));
    tm_DBHPositionHandleReportGpsStateInfo(z_CPIPos);
  }

#endif
  return TRUE;
}

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
void tm_CacheDBHPosition( sm_FixReportStructType z_pos )
{
   if ( ( z_sessioninfo.e_cpiReqId == TM_AUXTECH_REQ_ID_LPPE_CP ) ||
        ( z_sessioninfo.e_cpiReqId == TM_AUXTECH_REQ_ID_LPPE_UP ) ||
        ( z_sessioninfo.e_cpiReqId == TM_AUXTECH_REQ_ID_EMERGENCY_ORIG) ||
        ( z_sessioninfo.e_cpiReqId == TM_AUXTECH_REQ_ID_LPPE) )
   {
     MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"Cache CPI POS for DBH");
     if ( NULL != z_CacheCPIPos.z_CachedPos ) 
     {
        z_CacheCPIPos.v_CachedPosValid = TRUE;
        memscpy(z_CacheCPIPos.z_CachedPos, sizeof(sm_FixReportStructType),
                (sm_FixReportStructType* )&z_pos, sizeof(sm_FixReportStructType));
        return;
     }
     else
     {
       z_CacheCPIPos.z_CachedPos = (sm_FixReportStructType *)os_MemAlloc(sizeof(sm_FixReportStructType),OS_MEM_SCOPE_TASK);
       if ( NULL == z_CacheCPIPos.z_CachedPos )
       {
         MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Malloc Error for CPI Cache");
         return;
       }
       else
       {
         z_CacheCPIPos.v_CachedPosValid = TRUE;
         memscpy(z_CacheCPIPos.z_CachedPos, sizeof(sm_FixReportStructType),
                (sm_FixReportStructType* )&z_pos, sizeof(sm_FixReportStructType));
  return;
}
  }
  }
}

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
void tm_DBHPositionHandleReportGpsStateInfo(sm_FixReportStructType z_pos)
{
  prtl_event_cb_f_type *event_cb_f = NULL;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Handle best available position. ReqId %d", z_sessioninfo.e_dbhReqId);

  switch (z_sessioninfo.e_dbhReqId)
  {
    case TM_AUXTECH_REQ_ID_SUPLMLID:
    case TM_AUXTECH_REQ_ID_LPPE_UP:
      {
        /* If the requestor id is TM_AUXTECH_REQ_ID_SUPLMLID or TM_AUXTECH_REQ_ID_LPPE_UP,
           forward the DBH position report to UMTS UP */
        prtl_event_u_type z_event_payload;
        event_cb_f = tm_core_get_umpts_up_event_cb_fp();
        if (NULL == event_cb_f)
        {
          return;
        }

        z_event_payload.z_dbh_pos_report = z_pos;
        (*event_cb_f)(PTRL_EVENT_TYPE_DBH_POS_REPORT, &z_event_payload);
        break;
      }
    case TM_AUXTECH_REQ_ID_LPPE_CP:
      {
        /* If the requestor id is TM_AUXTECH_REQ_ID_LPPE_CP,
           forward the DBH position report to UMTS CP */
        prtl_event_u_type z_event_payload;
        event_cb_f = tm_core_get_lpp_cp_event_cb_fp();
        if (NULL == event_cb_f)
        {
          return;
        }

        z_event_payload.z_dbh_pos_report = z_pos;
        (*event_cb_f)(PTRL_EVENT_TYPE_DBH_POS_REPORT, &z_event_payload);
        break;
      }
    default:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Undefined requestor id %d", z_sessioninfo.e_dbhReqId, 0, 0);
      break;
  }

}

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
void tm_auxtech_wifi_on_demand_req_timer_expired(uint32 const cq_TimerId)
{
  if (TRUE == z_sessioninfo.v_reqWifiScan)
  {
    tm_requestOnDemandWifiScan(TRUE, z_sessioninfo.e_wifiReqId, z_sessioninfo.l_wifiClientId);
  }
}
