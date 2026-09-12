#ifndef TM_API_H
#define TM_API_H


/*===========================================================================
  
                            TM_API
                   
DESCRIPTION:
  This header file contains TM API interface related constants, variables
  and function protocols.

  
  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2020 Qualcomm Technologies, Inc.
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved. 

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/inc/tm_api.h#3 $

  when        who     what, where, why
  --------    ---     ----------------------------------------------------------
  01/28/20    anksha  Added support for navic assist
  07/05/19    mj      Handle sub ready event to read IMSI for SUPL
  07/20/18    skm     Boeing CaaS Feature
  02/17/17    skm     BLE and Barometer support for LPPE Phase 2
  08/10/16    mj      Add new IPC for leap second updates from ME
  06/06/16    mj      Add GERA support for AP-MP TTR
  02/17/16    mj      Added NV support to enable/disable CPI injections and early exit logic
  10/26/15    ss      Supporting Geofence Client Config indication
  08/13/15    hs      Unified fix reporting API implementation
  05/01/15    rh      Added get gps lock TM API
  01/08/15    gk      new pdapi events
  11/18/14    ah      Added support for PrisonerSV persistent blacklist SV masks
  04/28/14    ah      Added handling of Set Premium Services indication status
  01/28/13    mj      Modified tm_api_reg_task_init_notifier to add new param Client thread id
  01/10/13    gk      LPP ECID changes
  11/06/12    mj      Updated NV_CGPS_UTC_GPS_TIME_OFFSET_DEFAULT to 16
  08/01/12    mjohn   Added TM_CORE_MSG_ID_LSMP_GET_PARAM and TM_CORE_MSG_ID_LSMP_SET_PARAM msg id
  04/18/12    ssu     Adding sm_PositionReportStructType for GPS state information.
  03/01/12    rh      Added TM API tm_api_send_engine_error_recovery_report()
  10/10/11    rh      Changed XTRA SNTP servers to time.gpsonextra.net
  06/10/09    gk      ODP 2.0 support
  06/10/09    gk      Added Get UTC offset from NV
  01/12/09    atien   XTRA-T support    
  09/15/08    gk      Rename the on-demand struct to sm_ from cgps_
  04/25/08    gk      on-demand changes
  07/10/07    LT      Initial check-in.
===========================================================================*/

#include "gps_variation.h"
#include "comdef.h"
#include "customer.h"
#include "gps_common.h"
#include "gnss_common.h"
#include "gnss_consts.h"
#include "gnss_nv_loader.h"
#include "sm_api.h"
#include "sm_loc_lte_otdoa.h"
#include "gnss_lpp_ecid.h"
#include "wl_common.h"
#include "location_service_v02.h"
#include "fusion_csm_api.h"
#include "gnss_gdt.h"


#ifdef FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH /* this would be defined in custwcdma.h */
#undef FEATURE_WCDMA_GPS_CELL_ID_READING /* this would be defined in custcgps.h */
#endif

#ifdef __cplusplus
extern "C"
{
#endif



/* TM API to receive NV/Cass config from Location Feature Manager*/
#define TM_CORE_MSG_ID_LFM_CONF_INFO                      (SM_GEN8_IPC_ID_START_RANGE + 118)

/*
 IMP: Ensure the new IPC IDs are updated here
 //source/qcom/qct/modem/gps/IPC_Definitions/tm_ipcdefs.h
 1) Create the new IPC ID in the tm_ipcdefs.h
 2) Use the same IPC ID and IPC # in the file here.
 This is done to ensure when changes are merged to Mainline, the IPC ID's are consistent.
*/

/* TM API to read new ready subs IMSI */
#define TM_CORE_READ_NEW_IMSI                (SM_GEN8_IPC_ID_START_RANGE + 120)
/*
 * Default NV Item Value for XTRA
 */
#define NV_AAGPS_DEFAULT_XTRA_DOWNLOAD_INTERVAL         (48)     // hour
#define NV_AAGPS_DEFAULT_XTRA_NUM_DOWNLOAD_ATTEMPTS     (3)
#define NV_AAGPS_DEFAULT_XTRA_TIME_BETWEEN_ATTEMPTS     (10)     // minute

#define NV_AAGPS_DEFAULT_XTRA_SERVER_URL_SIZE           (128)

/* Default NV Item Value for XTRA Time Information */
#define  NV_AAGPS_DEFAULT_XTRA_TIME_INFO_ENABLED                (1)
#define  NV_AAGPS_DEFAULT_XTRA_TIME_INFO_DISABLED               (0)
#define  NV_AAGPS_DEFAULT_XTRA_TIME_INFO_UNC_THRESH             (12500)   // ms
#define  NV_AAGPS_DEFAULT_XTRA_TIME_INFO_DELAY_THRESH           (1000)    // ms

/* Default NV Item Value for Sensors configuration parameters */
#define  NV_CGPS_DEFAULT_SENSORS_SPI_OPINTERVAL                 (5000)    /* msec */
#define  NV_CGPS_DEFAULT_SENSORS_SPI_DETECTTHRESHOLD            (47)      /* milliG */


/* MSBased aiding throttling feature. This feature throttles MS-PDE 
** connections to download Aiding data, AFLT seed, Eph & Almanac.
** MS starts with a min back-off and goes up to max back-off, increasing
** the interval by back off factor. If the requests shows up on time 
** after the safe duration, then the back off time is reset back to min.
*/
#define GPSONE_MSB_THROTTLE_ENABLE TRUE
#define GPSONE_MSB_THROTTLE_DISABLE FALSE
#define BACK_OFF_FACTOR 4             /* Exp back-off by a factor of 4 */
#define BACK_OFF_MIN_DURATION 60      /* Min duration is 1 min */
#define BACK_OFF_MAX_DURATION 900     /* Max is 15 min's */
#define BACK_OFF_RESET_DURATION 900   /* Safe value is 15 min's */

/* Default NV Item Value for XTRA-T */
#define NV_AAGPS_DEFAULT_XTRA_T_ENABLED                   (1)
#define NV_AAGPS_DEFAULT_XTRA_T_DISABLED                  (0)

/* Bit definitions for the NV_AAGPS_DEVELOPMENT_TEST_CONTROL5 word */
#define NV_AAGPS_TEST5_EMERGENCY_CALL_SIM     ( 1 << 2 )  /* simulated e-call */


/* Maximum number of callbacks that can be registered. The callbacks are
   for potential clients to be notified of the fact that engine is ready to
   accept incoming clients

   Current clients (Please increase limit and add clients here):
   [0]                     - QMI LOC
   [1]                     - SDP
   [2]                     - LBS
*/

#define TM_MAX_NOTIFIERS                      3

/* Future new clients that register callbacks with TM will have to add their indexes, and update the TM_MAX_NOTIFIERS */
#define TM_LOC_MW_CLIENT_INDEX  (0)
#define TM_SDP_CLIENT_INDEX   (1)
#define TM_LBS_CLIENT_INDEX   (2)

#define TM_UNSUPPORTED_CLIENT_INDEX (TM_MAX_NOTIFIERS)

#define TM_E911_CPI_INJECTIONS_ENABLE (0x00000001)
#define TM_E911_EARLY_EXIT_ENABLE (0x00000002)

/* Bit mask to enable E911 concurrency feature */
#define TM_E911_CONCURRENCY_ENABLE (0x00000001)

/* Bits to Control the disable of dynamic HEPE logic for better TTFF */
#define TM_DYN_HEPE_NOT_DISABLED_FOR_BETTER_TTFF   (0x00)
#define TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF       (0x01)

/* Enum for XTM set nv items */
typedef enum
{
  TM_XTM_SET_CLIENT_TOKEN = 0x0001,     /* sets the client token NV*/
  TM_XTM_SESSION_CONTROL  = 0x0002,     /* sets the session control NV*/
} tm_xtm_nv_e_options;

typedef union
{
  uint32 client_token;
  uint8 session_control;
} tm_xtm_nv_u_data;

typedef struct
{
  tm_xtm_nv_e_options xtm_nv_item_set;     /*Type of NV item set by XTRAT */
  tm_xtm_nv_u_data xtm_nv_data_set;        /*Contains the data that is being set*/
  int client_id;                           /*Type of client */
} tm_xtm_struct_nv_write_status;

#ifdef FEATURE_CGPS_WCDMA
typedef struct {
  rrcmeas_plmn_cell_info_struct_type cell_id_info;
  rrc_cell_id_status_type            status;
} tm_rrc_cell_info_dch_report_type;
#endif

/* -----------------------------------------------------------------------*//**
 This callback function is called by GPS tasks once they are up and initialized. 
*/ /* ------------------------------------------------------------------------*/
typedef void (*tm_TaskInitNotifierCb)
   (
   uint32 taskId
   );


/* -----------------------------------------------------------------------*//**
 This callback function is called to deliver IMSI information when it has been retrieved. 
*/ /* ------------------------------------------------------------------------*/
typedef void (*tm_RetrieveIMSICb)
   (
   const cgps_IMSIType *const pz_IMSIInfo
   );

typedef struct
{
  uint8 sub;
  tm_RetrieveIMSICb pz_RetrieveIMSICb;
} tm_RetrieveIMSIType;


typedef struct {
  cgps_ActiveRATEnumType e_TriggerRAT;
  cgps_CellInfoCachedStruct cellInfoCached;
} tm_cell_info_update_type;

typedef struct {
  sm_ApiInitNotifierCb pNotifierCb; /* Callback registered with TM */
  uint8 clientIndex;        /* Index into the CB array, unique for each clients */
  boolean broadcast;        /* Go through the CB array and call all CBs if registered */
} tm_api_notifier_type;

typedef struct {
  sm_FixInfoStructType z_PosFixRpt;
  wl_ResultFlagsType z_wifi_result;
  sm_ReqEnumType e_wifi_pos_src;
} tm_wifi_fix_response_type;



/* TM Gera test commands */
typedef enum
{
  TM_GERA_TEST_CMD_FIRST = 0,
  /* TM gera command to register a client  */
  TM_GERA_TEST_CMD_CLIENT_REG =  TM_GERA_TEST_CMD_FIRST,
  /* TM gera command to deregister a client  */
  TM_GERA_TEST_CMD_CLIENT_DEREG = 1,
  /* TM gera command to enable a client  */
  TM_GERA_TEST_CMD_CLIENT_ENABLE_REQ = 2,
  /* TM gera command for extended enable of a client  */
  TM_GERA_TEST_CMD_CLIENT_ENABLE_REQ_EX = 3,
  /* TM gera command for disable of a client  */
  TM_GERA_TEST_CMD_CLIENT_DISABLE_REQ = 4,
  /* TM gera command for disabled info from the client  */
  TM_GERA_TEST_CMD_CLIENT_DISABLED = 5,
  /* TM gera command to reset GERA  */
  TM_GERA_TEST_CMD_RESET = 6,
  /* TM gera command to query client state  */
  TM_GERA_TEST_CMD_QUERY_CLIENT_STATE = 7,
  TM_GERA_TEST_CMD_LAST = TM_GERA_TEST_CMD_QUERY_CLIENT_STATE,
  TM_GERA_TEST_CMD_MAX = 0xFFFFFFFF
} tm_gera_test_cmd_e_type;

/* Data type to support the test interface for GERA core */
typedef struct
{
  tm_gera_test_cmd_e_type e_gera_cmd_type;
  uint32 q_client_id;
  uint32 q_dep_client_mask;
}tm_gera_test_cmd_s_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                              DATA TYPES                                 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Array to hold the callbacks for notification */
extern sm_ApiInitNotifierCb   pz_SmNotifierCb[TM_MAX_NOTIFIERS];


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                              FUNCTION DECLARATIONS                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**
 * @brief Converts CDMA timestamp into milliseconds
 *
 * This function takes a timestamp in the format generated by time_get()
 * and converts it to the implied total number of gps milliseconds.
 *
 * @param[in]  pt_CmdaTimestamp   Quadword with CDMA timestamp.
 * @param[out] pt_MillisecondsOut Quadword with timestamp in milliseconds.
 *
 * @return None.
 */
void gps_total_msec
(
   const qword pt_CdmaTimestamp,
   qword       pt_MillisecondsOut
   );

/**
 * @brief Convert quadword timestamp into GPS week number and milliseconds.
 *
 * This function takes a total number of gps milliseconds and returns the
 * week and the number of milliseconds in the week.
 *
 * @param[in]  pt_Milliseconds Quadword with milliseconds.
 * @param[out] pw_GpsWeekOut   GPS week number.
 * @param[out] pq_GpsMsOut     GPS millisecond since week start.
 *
 * @return None
 */
void gps_week_ms
(
   const qword  pt_Milliseconds,
   uint16      *pw_GpsWeekOut,
   uint32      *pq_GpsMsOut
   );

/**
 * @brief Convert GPS time into CDMA system time
 *
 * This function takes time stored in GPS Time and converts it into
 * CDMA system time.
 *
 * @param[in]  pz_Clock        GPS time structure.
 * @param[out] pt_TimestampOut Quadword pointer for CDMA time.
 *
 * @return None
 */
void gps_to_cdma_time
(
   const gps_TimeStructType *pz_Clock,
   qword                     pt_TimestampOut
   );

/*******************************************************************************
 * gps_MsecWeekLimit
 *
 * Function description:
 *
 *  me_GpsMsecWeekLimit is a helper function used to perform the 
 *  fairly common check to see if a msec is indeed with the allowable
 *  range of 0 thru WEEK_MSECS-1. Only values within a single week
 *  are entertained. (ie- Multiple week adjustments are not performed
 *
 *  Assigning a NULL pointer to p_GpsWeek will disable the week 
 *  adjustment logic ... ie - The p_GpsMsec will only be affected.
 *
 * Parameters: 
 *
 *  p_GpsMsecs - Pointer to the msec value under test
 *  p_GpsWeek - Pointer to week number which may be adjusted
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/
void gps_MsecWeekLimit(int32 *p_GpsMsecs, uint16 *p_GpsWeek);

/*******************************************************************************
 * tm_api_store_ftcal_tt_data
 *
 * Function description:
 *
 * Parameters: 
 *
 *  
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/
void tm_api_store_ftcal_tt_data(cgps_FtcalTTStruct *p_TTData);

/*******************************************************************************
 * tm_api_store_ftcal_ppm_data
 *
 * Function description:
 *
 * Parameters: 
 *
 *  
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/
void tm_api_store_ftcal_ppm_data(cgps_FtcalPpmStruct *p_PpmData);

/*******************************************************************************
 * tm_api_update_cell_info
 *
 * Function description:
 *
 * Parameters: 
 *
 *  p_CellInfoCached - Pointer to Cell ID information
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/
void tm_api_update_cell_info(const cgps_ActiveRATEnumType e_TriggerRAT, const cgps_CellInfoCachedStruct *const p_CellInfoCached);


/*******************************************************************************
 * tm_api_update_pos_punc_tunc
 *
 * Function description:
 *
 * Parameters: 
 *
 *  p_PosInfo - Pointer to the best available position fix info.
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/
void tm_api_update_gps_state_info(const sm_FixReportStructType *const p_PosInfo);

/*******************************************************************************
 * tm_api_report_utc_offset
 *
 * Function description:
 *  Get UTC offset from TM
 *
 * Parameters:
 *
 *  p_UtcOffset - Pointer to UTC offset information
 *
 * Return value:
 *
 *  void
 *
 *******************************************************************************/
void tm_api_report_utc_offset(uint8 *p_UtcOffset);


/*===========================================================================
FUNCTION tm_xtra_copy_data

DESCRIPTION
This function is called by PD API at client's context. Be sure only to perform
simple operations in this function.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
XTRA command status
===========================================================================*/
pdsm_xtra_cmd_err_e_type tm_xtra_copy_data(uint8 part_num, byte *xtra_part_ptr, uint32 xtra_part_length);




/* Enum for Sensor Data Streams */
typedef enum
{
  /* Client 1 data streams */
  TM_SENSOR_DATA_MOTION_LOW_POWER = 0x0001,     /* low power stationary pos indicator*/
  TM_SENSOR_DATA_VEL_1HZ_1SEC     = 0x0002,     /* velocity process noise 1Hz operating interval 1s */
  TM_SENSOR_DATA_JERK_1HZ_1SEC    = 0x0004,     /* jerk variance 1Hz operating interval 1s */

  /* Client 2 data streams */
  TM_SENSOR_DATA_MOTION_1HZ       = 0x0008,     /* stationary pos indicator 1Hz */
  TM_SENSOR_DATA_VEL_1HZ_100MS    = 0x0010,     /* velocity process noise 1Hz operating interval 100ms */
  TM_SENSOR_DATA_JERK_1HZ_100MS   = 0x0020      /* jerk variance 1Hz operating interval 100ms */
} tm_sensor_data_streams_bitmask;


/*******************************************************************************
 * tm_api_XTM_NV_status_report
 *
 * Function description:
 *  Called by XTRAT module to report the success of NV write to TM thread.
 *
 * Parameters:
 * which nv item has been set 
 * 
 * Return value: 
 * none
 *
 *******************************************************************************/

extern boolean tm_api_xtm_nv_status_report(tm_xtm_struct_nv_write_status *xtm_nv_set);

/*===========================================================================

FUNCTION tm_cm_map_sys_mode

DESCRIPTION
  This function maps a sys_sys_mode_e_type variable to a cgps_SrvSystemType
  variable
 
PARAMETERS
  sys_mode - The sys_sys_mode_e_type variable to be mapped
 
DEPENDENCIES
  None.

RETURN VALUE
  cgps_SrvSystemType

SIDE EFFECTS
  None

===========================================================================*/
cgps_SrvSystemType tm_cm_map_sys_mode(sys_sys_mode_e_type sys_mode);

/**
@brief dispatches an asynchronous request for TM to raise the external spi 
       injection event and request SPI injection
@param
@return
*/
void tm_api_xspi_start_injection(void);

/**
@brief dispatches an asynchronous request for TM to raise the external spi 
       injection event and stop SPI injection
@param
@return
*/
void tm_api_xspi_stop_injection(void);

/*=============================================================================

FUNCTION
  tm_api_reg_task_init_notifier

DESCRIPTION
  Called by potential clients to register their callbacks. The callbacks are
  used to notify the clients that the API/Engine is now ready to accept clients

  If the API/Engine is ready to accept clients, the callback will be called
  inline.  The caller should be ready to handle this and queue an IPC to itself.

DEPENDENCIES
  None.

PARAMETERS
  pNotifierCb - The callback of the client to be registered
  clientThreadId - Client Thread ID to identify the index within the array

RETURN VALUE
  TRUE:  registration has succeeded, or if Engine is ready the callback is
         called immediately;
  FALSE:  registration failed

SIDE EFFECTS
  None.

=============================================================================*/
boolean tm_api_reg_task_init_notifier(sm_ApiInitNotifierCb pNotifierCb, os_ThreadIdEnumType clientThreadId);

/**
@brief Calls the tm_xtra function for indicating xtra injection 
       has completed
@param None
@return None
*/
void tm_api_xtra_inj_done(uint32  q_AcceptedMask);

/*******************************************************************************
 * tm_api_report_wbiq_info
 *
 * Function description:
 *  Called by LM to update TM with the latest WBIQ FFT info
 *
 * Parameters: 
 *
 *  p_ReportWBIQInfo - WBIQ Info
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/

void tm_api_report_wbiq_info(const sm_ReportWBIQInfoStructType *p_ReportWBIQInfo);

/*******************************************************************************
 * tm_api_nav_config_change_resp
 *
 * Function description:
 *  Called by LM to update TM with the result of the NAV config change cmd.
 *
 * Parameters: 
 *
 *  q_GnssConfig - Current GNSS configuration
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/

void tm_api_nav_config_change_resp(const uint32 u_GnssConfig);

/*===========================================================================
  tm_loc_lte_otoda_recv_measurements

  Description:

     This function is used by LOC_LTE_OTDOA module to send the OTDOA measurements
 Parameters:
    Reason: stop reason

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_loc_lte_otdoa_recv_measurements(sm_loc_lte_otdoa_meas_info_type *p_otdoa_measurement_info);



/*===========================================================================
  tm_loc_lte_ecid_recv_measurements

  Description:

     This function is used by LOC_LTE_ECID module to send the ECID measurements
 Parameters:
    Reason: stop reason

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_loc_lte_ecid_recv_measurements(gnss_lpp_ecid_meas_info_type *p_ecid_measurement_info);

#ifdef FEATURE_WCDMA_GPS_CELL_ID_READING
/*******************************************************************************
*   tm_rrc_iface_cell_info_dch_report_callback
* 
* Function description:
*   This function is registered as a callback function with RRC module and
*   called whenever RRC responds to cell id request sent by TM.
* 
* Parameters:
*   Cell ID information: 
*      rrcmeas_plmn_cell_info_struct_type cell_id_info
*   Status: (RRC_NEW_CELL_ID_AVAIL/RRC_NO_CELL_ID_AVAIL)  
*      rrc_cell_id_status_type            status 
* 
* RETURN VALUE
*   None 
*
 *******************************************************************************/

void tm_rrc_iface_cell_info_dch_report_callback(
   rrcmeas_plmn_cell_info_struct_type cell_id_info,
   rrc_cell_id_status_type            status);

#endif /*FEATURE_WCDMA_GPS_CELL_ID_READING */

/*******************************************************************************
 * tm_api_ftcal_request
 *
 * Function description:
 *  Called by LM to update TM that FTCal is enabled/disabled 
 *
 * Parameters: 
 *
 *  b_Enable - TRUE to enable FTCal 
 *             FALSE to disable FTCal
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/

void tm_api_ftcal_request(boolean b_Enable);

/*=============================================================================

FUNCTION
 tm_api_reset_location_service_done

DESCRIPTION
  Indication from MGP that location service completed.

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_reset_location_service_done(pdsm_pd_ext_reset_location_service_status_e_type status);

/*=============================================================================

FUNCTION
  tm_api_send_engine_error_recovery_report

DESCRIPTION
  TM API to send engine error recovery report to QMI

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_send_engine_error_recovery_report(gnss_engine_err_recovery_rpt *pErrReport);

/*
******************************************************************************
* tm_read_qmi_services_configuration
*
* Function description:
*
*  This function is called by QMI services to read which QMI services are 
*  enabled. This function can be called only after SM has been initialized. 
*
* Parameters:
*  output_cfg : QMI Configuration read from NV
*
* Return value:
*  NONE
*
* ******************************************************************************
*/
void tm_read_qmi_services_configuration(pdsm_QMIServiceConfig *output_cfg);

/*
******************************************************************************
* tm_retrieve_IMSI
*
* Function description:
*
*  This function is called by XTM to request TM core to retrieve the IMSI
*
* Parameters:
*  NONE
*
* Return value:
*  NONE
*
* ******************************************************************************
*/
void tm_retrieve_IMSI(uint8 sub, tm_RetrieveIMSICb p_RetrieveImsiInfoCb);


/*=============================================================================

FUNCTION
  tm_api_send_dpo_status_report

DESCRIPTION
  TM API to send DPO status report to TM which inturn sends it to QMI.

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_send_dpo_status_report(sm_DpoStatusReportStructType *p_DpoStatusReport);

/*===========================================================================
FUNCTION tm_core_get_param_msg_handler

DESCRIPTION
  This function handles the get param ipc msg, and sends an ipc msg as a response
  with the param value retrieved
  
DEPENDENCIES

RETURN VALUE TRUE/FALSE

SIDE EFFECTS


===========================================================================*/
extern boolean tm_core_get_param_msg_handler(void *p_Data);

/*===========================================================================
FUNCTION tm_core_handle_prem_svc_status

DESCRIPTION
  This function handles the send premium service indication status ipc msg, 
  and calls tm_pdapi_pa_event_callback to send event to registered clients.

DEPENDENCIES

RETURN VALUE TRUE/FALSE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_prem_svc_status(void *p_Data);

/*===========================================================================

FUNCTION tm_api_get_gps_lock_status()

DESCRIPTION
  TM module can call this function to get current GPS lock status
 
  This is a synchronous API, will return immediately
 
DEPENDENCIES

RETURN VALUE
  Current GPS lock status
  
SIDE EFFECTS

===========================================================================*/
extern pdsm_gps_lock_e_type tm_api_get_gps_lock_status(void);

/*===========================================================================
  tm_handle_wifi_fix_response

  Description:

     This function is used by SM to provide Wifi fix response to TM
 Parameters:
   sm_FixReportStructType: Pointer to Position Report
                                            Note that this may be non Wifi position 
                                            if there was a better
                                            position available in ALE.
    wifi_ResultFlagsType: Wifi Results Flag
          The result flag indicates any error during Wifi pos fix calculation
          like no modem cache, no measurements, etc. It also indicates 
          whether a better non Wifi fix was sent instead. 
    sm_ReqEnumType: Wifi Position fix source (originiator)
  Return value: 
   VOID
=============================================================================*/
void tm_handle_wifi_fix_response(const sm_FixInfoStructType *pz_PosFixRpt,
                                 wl_ResultFlagsType z_wifi_result,
                                 sm_ReqEnumType e_wifi_pos_src);

/*===========================================================================
  tm_handle_gnss_le_fix

  Description:

     This function is used by SM to provide GNSS LE fixes to TM
 Parameters:
   sm_FixReportStructType: Pointer to Position Report
  Return value:
   VOID
=============================================================================*/
void tm_handle_gnss_le_fix(const sm_FixInfoStructType *pz_PosFixRpt);


/*===========================================================================
  tm_report_lppm_status

  Description:

     This function handles LPPM status report

 Parameters:

   pz_LppmStatus:  pointer to the LPPM status struct

  Return value: 
    void

=============================================================================*/
void tm_report_lppm_status(const sm_LppmStatusStructType *pz_LppmStatus);

/*===========================================================================
  tm_handle_ped_status

  Description:

     This function is used to handle MGP Ped Status

 Parameters:

   e_PedStatus:  Pedestrian status enum

  Return value: 
    void

=============================================================================*/
void tm_handle_ped_status(gnss_MgpPedStatusEnumType e_PedStatus);

/*===========================================================================
  tm_handle_gnss_qual_ind

  Description:

     This function is used to handle GNSS Qual Indication

 Parameters:

   e_GnssQualityInd:  GNSS Quality indication enum

  Return value: 
    void

=============================================================================*/
void tm_handle_gnss_qual_ind(const gnss_QualIndEnumType e_GnssQualityInd);


/*===========================================================================
  tm_handle_ped_dev_ctx

  Description:

     This function is used  to handle pedestrian device context

 Parameters:

   e_PedDevCtx:   Pedestrian Device Context enum

  Return value: 
    void

=============================================================================*/
void tm_handle_ped_dev_ctx(const gnss_PedDevCxtEnumType e_PedDevCtx);

/*===========================================================================
  tm_report_ped_align_availibility

  Description:

     This function is used to handle pedestrial alignement availability indication

 Parameters:

   v_PedAlignSensorAvailable: whether Pedestrian alignement is available or not

  Return value: 
    void

=============================================================================*/
void tm_handle_ped_align_availibility(boolean v_PedAlignSensorAvailable);

/*===========================================================================
  tm_handle_ped_sensor_assist_availibility

  Description:

     This function is used to handle Pedestrian Sensor Assistance
     availibility indication

 Parameters:

   v_PedSensorAssistAvailable: whether Pedestrian Sensor Assistance is available or not

  Return value:
    void

=============================================================================*/
void tm_handle_ped_sensor_assist_availibility(boolean v_PedSensorAssistAvailable);


/*===========================================================================
  tm_api_report_mgp_state_to_techsel

  Description:

     This function is used to send current MGP state to TechSel

 Parameters:

   p_rcvrState: Reciever State change info pointer

  Return value: 
    void

=============================================================================*/
void tm_api_report_mgp_state_to_techsel(sm_RcvrStateChangeStructType *p_rcvrState);


/*===========================================================================
  tm_api_report_wifi_scan_to_techsel

  Description:

     This function is used to send Wifi Scan to TechSel

 Parameters:

   p_response: Pointer to Lowi Response

  Return value:
    void

=============================================================================*/
void tm_api_report_wifi_scan_to_techsel(t_wiper_ap_set_struct_type *p_wifiScan);

/*===========================================================================
  tm_api_report_lowi_error_to_techsel

  Description:

     This function is used to send Lowi Error to TechSel
 
 Parameters:

   Void

  Return value: 
    void

=============================================================================*/
void tm_api_report_lowi_error_to_techsel(void);


/*===========================================================================
FUNCTION tm_config_external_time_transfer

DESCRIPTION
  This function sends an IPC to enable/disable external time transfer request to tm_core which will eventually get GERA's
  approval before enabling/disabling at MGP

DEPENDENCIES

RETURN VALUE
  boolean: TRUE: Successfully sent the request to TM
               FALSE: Failed to send the request to TM

SIDE EFFECTS

===========================================================================*/
boolean tm_config_external_time_transfer(uint8 *u_EnableFlag);

/*===========================================================================

FUNCTION tm_core_get_emergency_support_config

DESCRIPTION
  This function returns the configured values for emergency support. Eg. CPI/Early Exit are enable/disable 

DEPENDENCIES

RETURN VALUE UINT32, where bit 0 indicates if CPI enabled and bit 1 indicates Early exit logic enabled.

SIDE EFFECTS

===========================================================================*/
extern uint32 tm_core_get_emergency_support_config(void);

/*=============================================================================

FUNCTION
  tm_api_leap_second_report

DESCRIPTION
  Leap Second information from ME to SM

DEPENDENCIES
  None.

PARAMETERS
  mgp_LeapSecondInfoStructType - Pointer to leap second info

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_leap_second_report(const mgp_LeapSecondInfoStructType *pz_LeapSecInfo);

/*******************************************************************************
 * tm_api_report_xooffset_info
 *
 * Function description:
 *  Called by LM to update TM with the latest XO offset info
 *
 * Parameters: 
 *
 *  pz_ReportXOOffsetInfo - XO offset Info
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/

void tm_api_report_xooffset_info
(
   const sm_ReportXOOffsetInfoStructType *pz_ReportXOOffsetInfo
   );

/*===========================================================================
FUNCTION tm_api_notify_prtl_nv_op

DESCRIPTION
  Queue an IPC message to handle the next NV read.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_api_notify_prtl_nv_op(uint32 u_nvGroup);

/*===========================================================================

FUNCTION tm_core_get_concurrency_support_config

DESCRIPTION
  This function returns the configured values for E911 concurrency 

DEPENDENCIES

RETURN VALUE UINT32, where bit 0 indicates if E911 concurrency with apptrack is enabled or not

SIDE EFFECTS

===========================================================================*/

uint32 tm_core_get_concurrency_support_config(void);

/*===========================================================================

FUNCTION tm_core_mem_alloc_for_cache_aiding_data

DESCRIPTION
  This function either allocates memory for frees memory to cache aiding data during UMTS_UP_NI sessions 
  in concurrency mode. During concurrency, aiding data needs to be sent to UMTS_UP_NI sessions specially
  as its not the primary client running in LM.

PARAMS
  v_mem_alloc: TRUE means mem is allocated, FALSE means free the allocated mem
DEPENDENCIES

RETURN VALUE
   None

SIDE EFFECTS

===========================================================================*/
void tm_core_mem_alloc_for_cache_aiding_data(boolean v_mem_alloc);

/*===========================================================================

FUNCTION tm_fusion_csm_handle_control_req

DESCRIPTION
  This function handles Crowd Source Manager Control Request

DEPENDENCIES

RETURN VALUE Boolean

SIDE EFFECTS

===========================================================================*/
boolean tm_fusion_csm_handle_control_req(fusion_csm_ControlReqType *p_csmControlReq);

/*===========================================================================

FUNCTION tm_fusion_csm_handle_read_cs_data_req
 
DESCRIPTION
  This function handles request to read crowd sourced data
 
DEPENDENCIES

RETURN VALUE Boolean
  
SIDE EFFECTS

===========================================================================*/
boolean tm_fusion_csm_handle_read_cs_data_req(qmiLocCrowdSourceManagerReadDataReqMsgT_v02 *const pz_Req);


/*=============================================================================

  FUNCTION      tm_fusion_csm_RcvdSendAck

  DESCRIPTION
    This function is called by GDT as an ACK for a received record.

  PARAMETERS
    z_Ack  - The ack received
    (input)

  DEPENDENCIES  None

  PARAMETERS

  RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_rcvd_send_ack(gdt_SendAckArgumentType z_Ack);


/*=============================================================================

  FUNCTION      tm_fusion_csm_ProcSendEndResponse

DESCRIPTION
    This API is called by GDT to confirm the session end.

   PARAMETERS
    z_EndStatus  - Indicates whether the session succeeded or not
    (input)

  DEPENDENCIES  None

PARAMETERS

RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_end_response(gdt_EndArgumentType z_EndStatus);


/*=============================================================================

  FUNCTION      tm_fusion_csm_ProcSendBeginResponse

DESCRIPTION
    This function is called by GDT to send a upload session request status.

  DEPENDENCIES  None
 
  PARAMETERS
    z_Response - The permission status for the requested upload session
    (input)

RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_begin_response(gdt_StartArgumentType z_Response);

/*=============================================================================
  
  FUNCTION      tm_fusion_csm_ProcSendOpenResponse

  DESCRIPTION
    This function is called by GDT to send an open request status.

  DEPENDENCIES  None

  PARAMETERS
    z_Response - The status of the requested open request
    (input)

  RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_open_response(gdt_OpenArgumentType z_Response);

/*=============================================================================

  FUNCTION      tm_fusion_csm_ProcSendCloseResponse

DESCRIPTION
    This function is called by GDT to send a close request status.

  DEPENDENCIES  None
  
  PARAMETERS
    z_Response - The status for the requested close request
    (input)

RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_close_response(gdt_CloseArgumentType z_Response);


/*===========================================================================

FUNCTION tm_fusion_csm_handle_set_external_power_config_req

DESCRIPTION
  This function handles external power configuration information from HLOS to Fusion CSM

DEPENDENCIES

RETURN VALUE Boolean

SIDE EFFECTS

===========================================================================*/
boolean tm_fusion_csm_handle_set_external_power_config_req(const qmiLocSetExternalPowerConfigReqMsgT_v02 *pz_Req);

/*=============================================================================

FUNCTION
  tm_api_report_var_len_debug_nmea

DESCRIPTION
  Accepts formatted proprietary NMEA string for debug purpose if the NMEA debug
  string is enabled. NMEA string is sent as IPC to the TM task by this API, for
  forwarding to HLOS for debugging purpose.

DEPENDENCIES
  None.

PARAMETERS
  pDebugNMEA

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_report_var_len_debug_nmea(sm_DebugNMEAVarLen const *const cpz_DebugNMEA);

/*=============================================================================

FUNCTION
  tm_api_set_nmea_debug

DESCRIPTION
  Receive the NMEA string type configuration when they are modified through QMI.
  If the NMEA debug string is disabled, the large internal heap memory can be released,
  which is used only to store the NMEA variable length debug string.

DEPENDENCIES
  None.

PARAMETERS
  cq_NmeaType - Current NMEA sentence type config status

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_set_nmea_debug(pdsm_pa_nmea_type const cq_NmeaType);

/*===========================================================================
  tm_api_report_ranging_wifi_scan_to_techsel

  Description:

     This function is used to send ranging Wifi Scan to TechSel

 Parameters:

   p_wifiScan:  Pointer to the ranging scan data from lowi

  Return value:
    void

=============================================================================*/
void tm_api_report_ranging_wifi_scan_to_techsel(pdsm_RangingScanRespType *p_wifiScan);

/*=============================================================================

FUNCTION
  tm_handle_sensor_le_status

DESCRIPTION
  This function is to report Sensor LE State Change

DEPENDENCIES
  None.

PARAMETERS
  p_SensorLEStatus -Pointer to Sensor LE Status struct

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_handle_sensor_le_status(const sm_SensorLEStatusStructType *const p_SensorLEStatus);

/*=============================================================================

FUNCTION
  tm_handle_sensor_le_rel_disp

DESCRIPTION
  This function is to report Sensor Relative Displacement from Sensor LE

DEPENDENCIES
  None.

PARAMETERS
  p_RelDisp -Pointer to Relative Displacement Struct.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_handle_sensor_le_rel_disp(const sm_SensorLERelDispStructType *const p_RelDisp);

#ifdef FEATURE_GNSS_GERA_ENABLE
#error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

/*=============================================================================

FUNCTION
  tm_api_disable_APMP_at_GERA

DESCRIPTION
  APMP disable from MC to TM

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_disable_APMP_at_GERA(void);

/*=============================================================================

FUNCTION
  tm_api_persistent_sv_mask_report

DESCRIPTION
  Response from MGP regarding GNSS Persistent SV Mask request

DEPENDENCIES
  None.

PARAMETERS
  p_SvMask - Pointer to the persistent blacklist SV mask

RETURN VALUE
  TRUE  : IPC message sent successfully
  FALSE : IPC message sent failed

SIDE EFFECTS
  None.

=============================================================================*/
boolean tm_api_persistent_sv_mask_report(const gnss_SvIdMaskStructType *p_SvMask);

/*=============================================================================

FUNCTION
tm_api_persistent_sv_mask_to_filter

DESCRIPTION
Unsolicited GNSS Persistent SV Mask from MGP

DEPENDENCIES
None.

PARAMETERS
p_SvMask - Pointer to the persistent blacklist SV mask

RETURN VALUE
TRUE  : IPC message sent successfully
FALSE : IPC message sent failed

SIDE EFFECTS
None.

=============================================================================*/
boolean tm_api_persistent_sv_mask_to_filter(const gnss_SvIdMaskStructType *p_SvMask);

/*=============================================================================
FUNCTION
tm_ReportConstellationCtrlCfg

DESCRIPTION
This function is used to send GNSS Constellation Control Configuration to SM.

DEPENDENCIES
None.

PARAMETERS
pz_ConstellCfgFromMgp - GNSS Constellation Control Configuration in MGP

RETURN VALUE
None.

SIDE EFFECTS
None.
=============================================================================*/
void tm_ReportConstellationCtrlCfg(const gnss_ConstellationConfigT *pz_ConstellCfgFromMgp);

/*=============================================================================

FUNCTION
  tm_api_report_rc_on_threshold_period

DESCRIPTION
  This function is to report RC ON Threshold period to TM

DEPENDENCIES
  None.

PARAMETERS
  q_rcOnThresholdMsec -RC ON Threshild period in MilliSeconds

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_report_rc_on_threshold_period(uint32 q_rcOnThresholdMsec);

/*===========================================================================

FUNCTION tm_api_PfaTestPrescDwellResults()

DESCRIPTION
  Pass the prescribed dwell results to TM DIAG
 
DEPENDENCIES

RETURN VALUE

  
SIDE EFFECTS

===========================================================================*/
void tm_api_PfaTestPrescDwellResults(const sm_PfaTestPresDwellResultsType
                                     *p_PrescDwellResults);

/*===========================================================================
FUNCTION tm_pdapi_send_otdoa_event

DESCRIPTION
  This function sends a PDAPI event when OTDOA session starts/stops
  emergency positioning

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_pdapi_send_otdoa_event(sm_loc_lte_otdoa_pdapi_event_type sess_event_type);

/*===========================================================================
FUNCTION tm_api_allow_navic_assist_download

DESCRIPTION
  This function checks if the navic assist download request should be allowed to
  be sent to TM task.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_api_allow_navic_assist_download();

/*===========================================================================
FUNCTION tm_api_cancel_navic_assist_download

DESCRIPTION
  This function processes a cancel req for Navic assist download 

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void tm_api_cancel_navic_assist_download();

/*=============================================================================

FUNCTION
  tm_api_req_navic_assist_download

DESCRIPTION
  This function sends the Navic assist download req IPC to TM task.

DEPENDENCIES
  None.

PARAMETERS
  pz_xtraAssistReq -Navic assist download req type

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_req_navic_assist_download(const sm_ReqXtraStructType *pz_xtraAssistReq);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TM_API_H */