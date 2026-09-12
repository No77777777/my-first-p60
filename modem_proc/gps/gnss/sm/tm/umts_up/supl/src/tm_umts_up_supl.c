/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  TM-UMTS-UP-SUPL sub-module

GENERAL DESCRIPTION
  This file implements TM UMTS-UP SUPL sub-module.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
Qualcomm Confidential and Proprietary. All Rights Reserved.
Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
Copyright (c) 2013 - 2024 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_up/supl/src/tm_umts_up_supl.c#10 $
  $DateTime: 2024/02/12 21:52:07 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  01/04/24   jv      Abort session on receiving SUPL END even if fix is not reported to pdapi
  11/16/22   jv      Reset dd config with original dd sub once 911 nDD session is completed.
  06/04/20   skm     Get WCDMA Cell info for nDDS sub during AGPS NI 911.
  11/06/19   skm     Support SUPL MO in 911 for DSDS use case
  12/12/19   jvn     Fix memory leaks
  11/12/19   jv      Fixing SMART2.2_GNSS11.0 build errors
  12/12/19   py      Clearing UMTS SUPL cached position on UMTS OTA RESET
  12/10/19   sj      Dont run to timeout for MO E911.
  11/12/19   jv      Fixing SMART2.2_GNSS11.0 build errors
  09/30/19   skm     Support for 911 nDDS on DSDS targets 
  09/27/19   mj      No-Ship support
  09/13/19   mj      Remove support for SUPL area trigger events
  07/31/19   mj      Send Stop tm_core session only for valid active handles
  07/25/19   py      SUPL to process fix if position mode is MODE_AUTO 
  06/25/19   mj      Location Privacy: Send notifications for SUPL POS INIT
  06/06/19   sj      Disable AGNSS for 5G NR.
  04/09/19   mj      Deprecate NV1930
  02/19/19   skm     5G NR-NSA, LPP Rel-15 and SUPL 2.0.4 support 
  02/22/19   mj      Location Privacy Feature
  11/30/18   skm     911 VoWIFI IoT fix for NO_SRV on WIFI
  11/20/18   py      When SLP header is not correct, clen up session after SUPL END
  09/19/18   skm     Drop SUPL INIT, during 911 call when DD and DV SUB are different
  09/07/18   py      Added f_loc_uncertainty_angle in ext_pos_data for better precision
  08/07/18   skm     AGPS feature Support Mask
  06/20/18   py      Corrected case so SUPL_INIT to be ignored even when ABDS is disabled
  05/30/18   py      change data type of velHor & velVert in extended report to float for precision
  04/25/18   mj      Return TM_STOP_REASON_SUPL_SESS_PREEMT during SUPL preemt
  03/20/18   skm     Supl E911 over Wifi
  10/30/17   nath    Updated loc_unc_horizontal,alongAxisUnc and perpAxisUnc to float  
  08/31/17   ak      Add changes to retry LTE cell ID retrival if it fails for first time.
  04/25/17   rk      Tag the GPSTime when injecting GLO Reference Location to MGP.
  03/30/17   sum     Added support for horizontal velocity greater than 0x7fff in supl_convert_supl_end_pos_for_tm_core()
  03/29/16   gk      remove q_init at session cleanup
  03/01/17   kpn     Bring rr_ds_get_current_bs_info() out of the flags
  01/24/17   kpn     L+L changes and support for E911 CP session on non-DD sub
  12/01/16   RK      Fix issue in not caching non-done measurement in e911 scenario.
  11/16/16   mj      E911 and apptracking concurrency support
  11/16/16   RK      Collect all the measurements before processing.
  11/15/16   kpn     Send TUNC based on explicit knowledge of msec-known measurement state
  11/09/16   rk      Report the non-done measurement at X-2 seconds, where X is QoS reponse time.
  10/28/16   rk      Allow sending non-done measurement when close to timeout.
  08/30/16   kpn     Replace pbm_find_location() with pbm_session_record_read()
  08/04/16   mj      Add support to subscribe to MRL updates from LTE L1
  07/28/16   skm     2Timer re-arc for LPPe includes CR's(985835,1034212,1034213,1034216,1034219) 
  07/19/16   ak      Don't run-to_timeout  if NV_AAGPS_EMERGENCY_SERVICES_SPPRT_I is Low_Priority & session type is SI MO E911
  07/06/16   rn      Fix SM from sending out empty SUPL POS before OTDOA timer expiry when
                     server initially requests UEB agps mode and later changes to OTDOA only. 
  07/05/16   rk      Updated GAL/QZSS related traces.
  04/27/16   muk     Disable Storing SUPL Area Id Geofencing on EFS/SFS
  03/29/16   gk      remove q_init at session cleanup
  02/12/16   skm     Add queuing support to send >24AD SUPL POS
  04/18/16   skm     Fix for crash seein > 24 AD test case SUPL message queuing
  02/17/16   gk      During E911 force request of refL
  02/17/16   skm     CPI support for 911 
  02/11/16   gk      LPPe support
  01/05/16   mc      Dynamically allocate variables for huge structures to reduce stack usage
  10/26/15   rk      Support emergency simulation for missing SI MSA case
  09/10/15   rk      Use the pre-defined GLONASS & 4G position method bits in NV 1920 to enabled or
                     advertise GLONASS & 4G position method support respectively
  08/26/15   gk      90% conf reporting changes
  08/06/15   rk      Stop the OTDOA engine only if its running.
  07/25/15   rk      AD request Holdoff timer aren't needed for NI session.
  06/29/15   rk      Cleanup PDSM_PA_ASST_GLONASS_POS_PROTOCOL PA event handling.
  06/24/15   rh      Updated code to use CM-based Cell ID update
  06/22/15   gk      check for waiting for subsequent meas blocks modified.
  06/09/15   mj     Add support for W+W feature
  06/05/15   rk      Fix overwriting the RRLP protocol version in some cases.
  04/23/15   rk      In SI do not request GLO AD if the SLP told you in SUPL RESPONSE it wants to do GPS only.
  04/16/15   rk      When phone camps after 911 is dialed, set_ID by IMEI at E911 dialing shouldn't overwrite.
  04/06/15   skm     SUPL Service interaction broken for incoming MT-ES during ongoing MT session
  03/14/15   rh      Added support for MultiSIM-EFS NV
  03/10/15   lt      Proper handling of TLS version in SUPL messages for areaEventTriggered sessions.
  02/01/15   gk      Clean up tx mgr only if LPP is enabled.
  01/27/15   gk      reset session during E911 handler even if not camped on any RAT.
  01/24/15   gk      Vertical Velocity incorrectly encoded
  01/16/15   rk      Don't ignore good GPS measurements when subsequent GLO has zero SVs & vice-versa.
  01/14/15   rk      Added LPP RSTD >24 measurement processing support
  11/04/14   ah      Support SUPL TD-SCDMA LocID Config through OEM Feature Mask NV
  10/27/14   gk      NV item for terminating SUPL after final fix 
  10/21/14   mj      Add TDSCDMA handling for function tm_umts_up_supl_select_pos_prot
  09/24/14   rk      Added missing steering source type while injecting AcqAssist.
  07/17/14   gk      SUPL MO should not work when NO-SIM. 
  07/02/14   ss      Fixing issues with MSB and MSA request handling when MSA or MSB 
                     is disabled by NV and when LPP is disabled.
  06/30/14   rk      Fix horizontal velocity error computation.
  06/26/14   rk      Fixed Glonass measurement weren't propagated due to incorrect indexing in the SUPL module.
  06/12/14   skm     Fix WCDMA SUPL2.0 RRLP MSA Broken due to GLO meas request to ME in RRLP GPS Assistance Proc
  05/22/14  skm      Update SUPL session Status when session fails
  05/14/14   rk      Fixed the cached measurement report delete issue. Also added support
                     to receive & process the measurements (GPS, GLO) in any order.
  05/01/14   gk      Get the primary PLMN info to send the lte cell id
  04/03/14   ssu     Additional Fix details parameters for Geofence Breach reports
  12/23/13   mj      Remove FEATURE_DUAL_SIM and FEATURE_TRIPLE_SIM
  02/27/13  ssh      OTA Delete to clear away existing NI Geofences
  10/29/13   mj      Update WCDMA and TDSCDMA API calls for segment loading
  08/06/13   mj      Handle returned value from mcc_pd_get_bs_info
  07/12/13   rk      Don't overwrite the LPP error type when set by ReqLocInfo function.
  06/17/13   mj      Call api rr_ds_get_current_bs_info for DSDS target
  05/13/13  ssh      Using timestamp in SUPL_END while pushing the positon to TM.
  05/14/13  ssh      Handle TCP connections in back to back SUPL sessions
  02/22/13   rk      Use correct position method bitmask (against NV 1920) to set SUPL AGNSS posMethod (when LPP as position protocol).
                     Also allow OTDOA to continue in AGNSS settings. [CR 466845]
  02/22/13   rk      Bring back cached measurement report usage for LPP E911 scenario
  02/14/13   mj      Initialize rr_cell_info before accessing it
  11/04/12   mj      Added new function supl_chk_if_external_modem_OOS
  09/07/12   mj      Added change for external cell updates for SGLTE
  08/01/12   ssu     Changes in SUPL to make use of cached cell id while doing
                     a SUPL session till a valid cell-id is available.
  01/04/12   ssu     Obtaining th CELL id information in CELL_DCH from CM APIs
  12/13/11   gk      LPP related fixes
  04/06/11   LT      ASN1 migration.
  03/03/11   gk      Timer race condition fix
  01/03/11   gk      Eph throttling rework
  04/01/10   atien   Add NMR feature
  01/14/10   rb      Set PDSM_PD_UTC_TIME_VALID flag when UTC time is available
  12/14/09    gk     send comm failure event
  10/23/09   rb      Added SUPL Cell ID feature
  07/13/09   gk      Check the supl server address correctly
  05/29/09   lt      Controls for some SUPL optional field build time control.
  05/21/09   gk      Treat PDSM_SESS_TYPE_TRACK_IND as new
  04/21/09   gk      Check if we are in UMTS/GSM before starting 911 processing
  01/06/09   LT      Cached position is now included in SUPL_POS_INIT.
  05/19/08   LT      NI request rejection during emergency added.
  08/02/07   LT      Handling SVs non-existence case; Removal of alm. week extension.
  06/20/07   LT      Added handling of NV Item aagps_gps_lock_control.
  05/25/07   LT      Added GpsComplete event; Time-Stamping position info;
  09/14/06   LT      Initial version

============================================================================*/

#include "gps_variation.h"
#include "customer.h"   /* Customer configuration file */

#ifdef FEATURE_CGPS_UMTS_UP_SUPL

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include <stdio.h>

#include "msg.h"
#include "math.h"

#include "mgp_api.h"
#include "sm_nv.h"
#include "tm_data.h"
#include "tm_common.h"

#include "uim.h"
#include "pbmlib.h"

#include "pd_comms_api.h"
#include "tm_asn1util.h"
#include "tm_rrlp_asn1_encode.h"
#include "tm_rrlp_asn1_decode.h"
#include "tm_rrlp_utils.h"
#include "tm_rrlp_up.h"

#include "tm_umts_common_utils.h" /* this files includes tm_rrlp.h */

#include "tm_sim_utils.h"

#include "sm_api.h" /* included only for the data structures defined there */
#include "sm_log.h"

#ifdef FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH /* this would be defined in custwcdma.h */
#undef FEATURE_WCDMA_GPS_CELL_ID_READING /* this would be defined in custcgps.h */
#endif

#define SUPL_MAIN_VARIABLES_DEFINED
#include "tm_umts_up_supl.h"    /* which includes "tm_supl_asn1.h" */
#undef SUPL_MAIN_VARIABLES_DEFINED

#include "tm_umts_up_supl_comm.h"
#include "pdapibuf.h"

#include "rrclsmif.h"

#ifdef FEATURE_SUPL1_TDS  /* defined in custcgps.h, if .builds defines FEATURE_CGPS_USES_TDS */
#include "tdsrrclsmif.h"
#endif

#include "fs_public.h"
#include "gps_fs_api.h"

#include "tm_prtl_iface.h"
#include "geran_eng_mode_read_api.h"

#if defined (FEATURE_CGPS_USES_CDMA) && !defined (FEATURE_GNSS_LOW_MEMORY)
#include "tm_cm_iface.h"
#include "tm_1x_up_is801_drv_iface.h"
#include "tm_is801.h"
#include "mccdma.h"
#ifdef FEATURE_CMI
#include "mccdma_v.h"
#endif
#endif /* FEATURE_CGPS_USES_CDMA && !FEATURE_GNSS_LOW_MEMORY*/
#include "tm_lpp_tx_mgr.h"
#include "tm_lpp.h"
#include "tm_lpp_asn1util.h"
#include "tm_lpp_up.h"
#include "tm_pdapi_client.h"
#include "gnss_calendar_util.h"
#include "gnss_wwan_iface.h"
#include "tm_l1_iface.h"

#include "tm_auxtech_iface.h"
#include "tm_ds_iface.h"

#ifdef FEATURE_CGPS_LTE_CELLDB
#include "lte_ml1_ext_api.h"
#endif

#include "tm_umts_up_supl_api.h"

/* same def. as in tm_core.c */
#define C_MIN_EPH_HAVE_THRESHOLD 8
#define C_MAX_EPH_NEED_THRESHOLD 1
#define C_MIN_SV_IN_VIEW_THRESHOLD 6

#define NMR_HANDOVER_LIMIT 3  /* we won't send NMR info if # of handovers is greater than this */

#ifdef FEATURE_CGPS_LTE_CELLDB
lte_ml1_gps_mrl_cell_meas_update_t  z_lte_mrl;
#endif

supl_IPAddress                      z_supl_IPAddress; /* for ASN1 encoding */
#define MAX_IMEI_LEN 8    /* Maximum number of bytes required to store the IMSI of the UE */

/* TBD: this and ver_maj_after_supl_init_glob need to go up to oem */
typedef struct
{
  boolean trigger_type_present;
  uint32  q_trigger_type;  /* 0: periodic; 1: areaEvent */
}supl2_specific_supl_init_ies;
supl2_specific_supl_init_ies z_supl_init_trigger_type;


uint32 q_supl_ver_maj_after_supl_init_glob; /* use to remember the agreement before
                                               the session is accepted */

supl_status_s_type        z_supl_session_status;
supl_emergency_struct_type  z_supl_emergency_status;

supl_set_id               z_Set_Id;
supl_area_event_status_s_type  z_supl_area_event_session_status[C_MAX_GEOFENCE_NUM];

/* will save SUPL INIT source (1X/GSM SMS, WAP-PUSH or UDP) */
pdsm_lcs_supl_init_src_e_type z_supl_init_source;

pending_supl_end          z_pending_supl_end[C_MAX_NUM_PENDING_SUPL_END];
pdsm_pd_info_s_type       z_supl_pd_info_buffer;
tm_umts_common_pos_for_net_s_type    z_supl_cached_pos_for_net;
os_TimerMsgType *supl_cf_timer;
os_TimerMsgType *wcdma_l1_mrl_req_timer;
os_TimerMsgType *supl_alm_req_holdoff_timer;
os_TimerMsgType *supl_eph_req_holdoff_timer;
os_TimerMsgType *supl_iono_req_holdoff_timer;
os_TimerMsgType *supl_utc_req_holdoff_timer;
os_TimerMsgType *supl_refloc_req_holdoff_timer;
os_TimerMsgType *supl_tcp_stay_on_only_timer;
os_TimerMsgType *supl_wifi_wait_timer;
os_TimerMsgType *supl_lpp_early_response_timer;
os_TimerMsgType *supl_lte_get_cell_info_retry_timer;

/* number of handover since last valid cell info: 0 means no handover */
uint32 q_handovers_since_last_cell_id;

uint8  supl_gm_handle;
uint32 gm_transaction_id;   /*GM Module Transaction ID*/
uint16 w_SetSessionID;

os_TimerMsgType *supl_glo_alm_req_holdoff_timer;
os_TimerMsgType *supl_glo_eph_req_holdoff_timer;

os_TimerMsgType *supl_bds_alm_req_holdoff_timer;
os_TimerMsgType *supl_bds_eph_req_holdoff_timer;
os_TimerMsgType *supl_bds_reftime_req_holdoff_timer;
os_TimerMsgType *supl_bds_refloc_req_holdoff_timer;
os_TimerMsgType *supl_bds_rti_req_holdoff_timer;
os_TimerMsgType *supl_bds_timemodel_req_holdoff_timer;

os_TimerMsgType *supl_gf_holdoff_timer;

uint8 u_supl_global_num_mlid; /* number of valid entries in z_mlid_buffer */


/*************** functions internal to the SUPL module ******************/

static boolean supl_SuplInit_proc
(const supl_ULP_PDU *p_pdu_decoded,
 uint8 *p_raw_pdu,
 uint16 w_raw_pdu_len,
 uint8 *p_hash
 );

static boolean supl_SuplResponse_proc(const supl_ULP_PDU *p_pdu_decoded);
static boolean supl_SuplPos_proc(const supl_ULP_PDU *p_pdu_decoded);

static boolean supl_SuplPos_is801_proc(uint8 *p_payload, const uint16 payload_length);
static void    supl_SuplEnd_proc(const supl_ULP_PDU *p_pdu_decoded);

boolean supl_cell_info_sanity_chk_ok(const cgps_CellInfoCachedStruct *cell_db_data_ptr);

static boolean supl_cell_information_construct(supl_CellInfo *p_asn1_cell_info,
                                               supl_cell_info_struct_type *p_cell_info_allocated);

static uint8  supl_chk_and_build_all_mlid(supl_LocationIdData *p_supl_LocationIdData,
                                          supl_LocationId *p_supl_LocationID,
                                          supl_cell_info_struct_type *p_cell_info_allocated,
                                          uint8 u_num_allocated_array_elements);

static void    supl_store_slp_sessionId(const supl_ULP_PDU *p_pdu);

static void    supl_util_server_session_id_organizer
(const supl_SessionID *p_source_session_id,
 supl_slp_session_id *p_slp_session_id
 );

static void supl_set_session_id_organizer(const supl_SessionID *p_source_session_id,
                                          supl_set_session_id *p_set_session_id);

static slp_mess_header_check_result_e_type  supl_message_overhead_proc
(const supl_ULP_PDU *p_pdu_decoded,
 uint16 *p_provided_set_session_id
 );

static void supl_send_notification_verifcation(const supl_ULP_PDU *p_pdu_decoded, const uint8 *p_hash);
static pdsm_lcs_supl_format_indicator_e_type supl_convert_format_indicator_pdsm(supl_FormatIndicator formatInd);

static void supl_action_upon_ni_notif_verif_accepted
(pdsm_lcs_supl_notify_verify_req_s_type *p_req_data,
 supl_slp_session_id                    *p_slp_id
 );

static void supl_preempt_ongoing_session(supl_StatusCode e_cancel_reason);

static void supl_gad9_to_pos_est
(const gad_ellip_alt_unc_ellip_type *p_zEllipAltUncEllip,
 supl_PositionEstimate *p_positionInfo
 );

void supl_convert_slp_id_from_notif_verif_resp
(pdsm_lcs_supl_notify_verify_req_s_type   *p_req_data,
 supl_slp_session_id                      *p_slp_id
 );

void supl_convert_velocity
(float f_vel_east,
 float f_vel_north,
 float f_vel_vertical,
 supl_Velocity *p_velocity
 );

static void supl_convert_supl_end_pos_for_tm_core
(const supl_SUPLEND *p_suplEnd, gps_RefLocStructType *p_ref_loc);

static void check_and_handle_handover(boolean *abort_sending_next_msg);

#if !defined (FEATURE_CGPS_USES_CDMA) || defined (FEATURE_GNSS_LOW_MEMORY)
// for strange build that has the feature above defined while
// no 1X AGPS code objs, comment the #ifndef out
static boolean is801_decode_fwd_link_pddm
(tm_prtl_type  prtl_type,
 byte          *data,
 uint16        length,
 byte          *offset
 )
{
  return FALSE;
}
#endif  /* not-FEATURE_CGPS_USES_CDMA  Dummies: for UMTS-only builds */

static supl_init_trigger_type_enum_type supl_init_trigger_event_check(void);

static void  supl_init_trigger_event_proc
(pdsm_lcs_supl_notify_verify_req_s_type *p_req_data,
 supl_slp_session_id                    *p_slp_id,
 tm_cm_phone_state_info_s_type          phone_state_info
 );

static void supl_set_cf_state_area_event(supl_cf_state_enum_type  e_new_supl_cf_state,
                                         uint8                    u_session_num);

static boolean supl_is_in_any_ut0(void);

static boolean supl_SuplTriggerStart_tx(uint8 u_session_num);
static void supl_SuplTriggerResponse_proc(const supl_ULP_PDU *p_pdu_decoded);
static void supl_SuplReport_proc(const supl_ULP_PDU *p_pdu_decoded);



static uint8 supl_area_event_session_match(const supl_SessionID *p_source_session_id);
static void supl_SET_id_get(void);

static boolean supl_SLP_session_id_match_check
(supl_slp_session_id *p_cached_Slp_Sess_Id, supl_slp_session_id *p_incoming_Slp_Sess_Id);


static boolean tm_umts_up_supl_supl_report_prepare_or_tx
(uint8                              u_session_num,
 sm_gm_client_breach_notify_type *p_gm_client_breach_notify,
 uint8 *p_hash,
 supl_slp_session_id *p_provided_slp_session_id
 );

static boolean tm_umts_up_supl_supl_report_tx
(uint8                               u_session_num,
 sm_gm_client_breach_notify_type *p_gm_client_breach_notify,
 uint8 *p_hash,
 supl_slp_session_id *p_provided_slp_session_id
 );

static void  supl_session_query_proc(uint8 *p_hash,
                                     supl_slp_session_id *p_provided_slp_session_id,
                                     uint32 q_supl_version);

static void supl_SuplTrigStop_proc(const supl_ULP_PDU *p_pdu_decoded);

static boolean supl_chk_if_external_modem_OOS(const pdsm_cell_info_s_type external_cell_info);

static boolean supl_is_wifi_scan_needed(void);

static void supl_request_wifi_scan(void);

static pbm_return_type supl_get_msisdn_number(char *const msisdn_number, uint8 *const num_len);

static pbm_session_enum_type supl_pbm_get_session_type(sys_modem_as_id_e_type as_id);


#define SUPL_VELOCITY_UPWARD 0
#define SUPL_VELOCITY_DOWNWARD 1

/*===========================================================================

FUNCTION supl_avoid_run_to_timeout_in_emergency

DESCRIPTION
  This function decides if the emergency session should be run to time out or not.
  This feature is currently supported for carrier using RRLP and LPP over SUPL only.

PARAMETERS:
   cpz_SuplSessionStatus [IN] : Pointer to SUPL session structure.

DEPENDENCIES: none.

RETURN VALUE:
  True  : Dont run to time out.
  False : Run to Time out

SIDE EFFECTS: none.

===========================================================================*/
static boolean supl_avoid_run_to_timeout_in_emergency
(
  supl_status_s_type const *const cpz_SuplSessionStatus
)
{
  /* Get the real emergency status */
  boolean cv_EmerSrvMode  = tm_umts_common_utils_cm_emergency_call_status();
  boolean cv_E911Simulate = tm_agps_emergency_call_simulate();

  if(NULL == cpz_SuplSessionStatus)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null ptr!");
    return TRUE;
  }

  /* Check if e911 simulate test NV is set. This is only enabled in field &
   * other testing to simulate R2TO behavior on live networks. Always run to time out
   * if set */
  if(TRUE == cv_E911Simulate)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "R2TO enabled for E911 simulation!");
    return FALSE;
  }

  /* 
   * If session type is SI MO Real E911 then
   *  don’t run-to-timeout. 
   *
   *  This feature is currently supported for carrier using RRLP & LPP over SUPL.
  */
  if (TRUE == cv_EmerSrvMode)
  {
    if(C_SUPL_SESSION_UI == cpz_SuplSessionStatus->u_callflow_kind)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Dont R2TO for MO E911!");
      return (TRUE);
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "R2TO for NI E911!");
      return (FALSE);
    }
  }
  else
  {
    /* Dont R2TO for all other cases, i.e. non E911, no E911 simulate */
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Dont R2TO for non 911!");
    return (TRUE);
  }
}


/*===========================================================================

FUNCTION supl_fill_ver2_ganss_set_capability

DESCRIPTION
  This function populates the SUPL GANSS SET Capability IEs in the SUPL_START &
  SUPL_POS_INIT message.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/

void supl_fill_ver2_ganss_set_capability
(
   supl_SETCapabilities          *const pz_SetCpblt,
   supl_GANSSPositionMethod        pz_ganssPositionMethod[],
   supl_GANSSPositionMethods     *p_ganssPositionMethodsList,
   supl_callflow_kind_enum_type  const u_callflow_type
   )
{
  boolean u_abds_enabled = FALSE, u_aglo_enabled = FALSE;
  supl_GANSSPositionMethod        *pz_gloPositionMethod = NULL,
     *pz_bdsPositionMethod = NULL;

  if ((NULL == pz_SetCpblt) ||
      (NULL == pz_ganssPositionMethod) ||
      (NULL == p_ganssPositionMethodsList))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SUPL ver2 ganss set capability cannot be populated");
    return;
  }

  /* If both AGLO and ABDS are enabled, send capabilities for both. The first
   * element in the array pz_ganssPositionMethod points to the aglo ganss 
   * element, while the second element points to abds element */
  pz_gloPositionMethod = &pz_ganssPositionMethod[0];
  pz_bdsPositionMethod = &pz_ganssPositionMethod[1];

  memset(pz_ganssPositionMethod, 0, 2 * sizeof(*pz_ganssPositionMethod));

  u_abds_enabled = tm_umts_up_supl_rrlp_assisted_bds_enabled();
  u_aglo_enabled = ((tm_umts_up_supl_rrlp_assisted_glonass_enabled()) ||
                    (tm_core_get_asst_glo_lpp_up_enable() && tm_core_get_lte_lpp_up_enable()));


  /****************************************************************************
   * AGLO Capabilities First                                                  *
   ***************************************************************************/
  // Common for LPP/RRLP8
  if (u_aglo_enabled)
  {
    pz_SetCpblt->posTechnology.m.ver2_PosTechnology_extensionPresent = 1;
    pz_SetCpblt->posTechnology.ver2_PosTechnology_extension.m.gANSSPositionMethodsPresent = 1;

    pz_gloPositionMethod->m.ganssSBASidPresent = 0; /* no GANSS SBA Sid */
    pz_gloPositionMethod->ganssId = C_SUPL2_GANSS_ID_GLONASS;

    /** LPP or RRLP **/
    if (u_callflow_type == C_SUPL_SESSION_UI)
    {
      /* Set-Initiated */
      if ((z_supl_session_status.u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED) &&
          tm_core_pos_mode_allows_aglo_msa_up(z_supl_session_status.u_position_protocol))
      {
        pz_gloPositionMethod->gANSSPositioningMethodTypes.setAssisted = TRUE;
      }
      else if ((z_supl_session_status.u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED) &&
               tm_core_pos_mode_allows_aglo_msb_up(z_supl_session_status.u_position_protocol))
      {
        pz_gloPositionMethod->gANSSPositioningMethodTypes.setBased = TRUE;

        /* in case SLP wants to do auto. */
        pz_gloPositionMethod->gANSSPositioningMethodTypes.autonomous = TRUE;
      }
    }
    else
    {
      /* posTechnology AGPS -- network-initiated: all method regardless
        what SUPL_INIT says, with PreferedMethod set to NoPref, per MSM6275 SUPL 1.0
        Support Reference Guide */
      /* Network-Initiated */
      if (tm_core_pos_mode_allows_aglo_msa_up(z_supl_session_status.u_position_protocol))
      {
        pz_gloPositionMethod->gANSSPositioningMethodTypes.setAssisted = TRUE;
      }
      if (tm_core_pos_mode_allows_aglo_msb_up(z_supl_session_status.u_position_protocol))
      {
        pz_gloPositionMethod->gANSSPositioningMethodTypes.setBased = TRUE;
      }
      if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_STANDALONE))
      {
        pz_gloPositionMethod->gANSSPositioningMethodTypes.autonomous = TRUE;
      }
    }

    pz_gloPositionMethod->gANSSPositioningMethodTypes.extElem1.count = 0;

    pz_gloPositionMethod->gANSSSignals.numbits = 1;
    /* leftmost: BL1 -- only BL1 is supported */
    pz_gloPositionMethod->gANSSSignals.data[0] = LPP_GNSS_SIGNAL_ID_GLONASS_G1_BMASK;

    pz_gloPositionMethod->extElem1.count = 0;

  } /* Common for RRLP8 & LPP */


  if (u_abds_enabled)
  {
    pz_SetCpblt->posTechnology.m.ver2_PosTechnology_extensionPresent = 1;
    pz_SetCpblt->posTechnology.ver2_PosTechnology_extension.m.gANSSPositionMethodsPresent = 1;

    pz_bdsPositionMethod->m.ganssSBASidPresent = 0; /* no GANSS SBA Sid */
    pz_bdsPositionMethod->ganssId = C_SUPL2_GANSS_ID_BDS;

    /** Only MSB is suported for both UI and NI, therefore set MSA to false
     ** unconditionally **/
    pz_bdsPositionMethod->gANSSPositioningMethodTypes.setAssisted = FALSE;

    /* For UI, send the capabilities as reflected in the session params */
    if (u_callflow_type == C_SUPL_SESSION_UI)
    {
      /* Set-Initiated */
      if (z_supl_session_status.u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
      {
        pz_bdsPositionMethod->gANSSPositioningMethodTypes.setBased = TRUE;
        /* in case SLP wants to do auto. */
        pz_bdsPositionMethod->gANSSPositioningMethodTypes.autonomous = TRUE;
      }
    }
    else
    {
      /* posTechnology AGPS -- network-initiated: send all method we are capable of 
       * regardless what SUPL_INIT says, with PreferedMethod set to NoPref. */
      pz_bdsPositionMethod->gANSSPositioningMethodTypes.setBased = TRUE;
      pz_bdsPositionMethod->gANSSPositioningMethodTypes.autonomous = TRUE;
    }

    pz_bdsPositionMethod->gANSSPositioningMethodTypes.extElem1.count = 0;

    pz_bdsPositionMethod->gANSSSignals.numbits = 1;
    pz_bdsPositionMethod->gANSSSignals.data[0] = C_SUPL2_GANSS_SIGNAL_ID_MASK_BDS_BL1; /* leftmost: G1 -- only G1 is supported */

    pz_bdsPositionMethod->extElem1.count = 0;
  }

  /* If both AGLO and ABDS are enabled, send capabilities for both. The first
   * element in the array pz_ganssPositionMethod points to the aglo ganss 
   * element, while the second element points to abds element */
  if (u_abds_enabled && u_aglo_enabled)
  {
    p_ganssPositionMethodsList->elem = &pz_ganssPositionMethod[0];
    p_ganssPositionMethodsList->n = 2;
    pz_SetCpblt->posTechnology.ver2_PosTechnology_extension.gANSSPositionMethods =
       *p_ganssPositionMethodsList;
  }
  /* If AGLO only GANSS enabled, send capabilities only for AGLO */
  else if (u_aglo_enabled)
  {
    p_ganssPositionMethodsList->elem = pz_ganssPositionMethod;
    p_ganssPositionMethodsList->n = 1;
    pz_SetCpblt->posTechnology.ver2_PosTechnology_extension.gANSSPositionMethods =
       *p_ganssPositionMethodsList;
  }
  /* If ABDS only GANSS enabled, send capabilities only for AGLO */
  else if (u_abds_enabled)
  {
    p_ganssPositionMethodsList->elem = &pz_ganssPositionMethod[1];
    p_ganssPositionMethodsList->n = 1;
    pz_SetCpblt->posTechnology.ver2_PosTechnology_extension.gANSSPositionMethods =
       *p_ganssPositionMethodsList;
  }


}


/*===========================================================================

FUNCTION tm_umts_up_supl_session_init

DESCRIPTION
  This function initializes all protocol session related variables.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/

void tm_umts_up_supl_session_init(boolean session_clear_for_prempt)
{
  boolean u_supl_end_waiting = FALSE;
  supl_msg_queue_element *supl_msg = NULL;

  if (session_clear_for_prempt)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL Session status cleared for prempt", 0, 0, 0);

    if (z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND)
    {
      u_supl_end_waiting = TRUE;
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL Session status cleared", 0, 0, 0);
    memset(z_pending_supl_end, 0, sizeof(pending_supl_end) * C_MAX_NUM_PENDING_SUPL_END);
  }

  if (z_supl_session_status.supl_msg_q_status == TRUE)
  {
    /*If SUPL session when pre-empted was in a MULTIPLE MEAS CF state 
    There could be messages in SUPL MSG queue. Free the PDSM_Buffers
    and Queue will be re-initialized in SUPL enqueue function in the next session*/
    while (0 != q_cnt(&(z_supl_session_status.supl_msg_q)))
    {
      supl_msg = (supl_msg_queue_element *)q_get(&(z_supl_session_status.supl_msg_q));
      if (NULL != supl_msg)
      {
        (void)pdsm_freebuf((char *)supl_msg);
      }
    }
    /*Call Queue Destroy to release the mutex associated with it. 
       This call will release the Mutex assiciated with the queue,
       1)initialized at SUPl module INIT
       2)after a successful >24AD session. But not for all other SUPL sessions.
     */
    (void)q_destroy(&(z_supl_session_status.supl_msg_q));
  }

  /*All common SUPL initialization*/
  supl_session_status_reset();
  memset(&z_supl_emergency_status, 0, sizeof(supl_emergency_struct_type));
  z_supl_session_status.z_network_assist.radio_link_prot_params.z_rrlp_params.b_RefNum = -1;
  z_supl_session_status.wait_for_write_ack = FALSE;
  z_supl_session_status.supl_msg_q_status = FALSE;
  z_supl_session_status.v_privacy_override_set = FALSE;
  z_supl_session_status.session_sub_id = tm_core_get_current_dd_subs_id();

  /* Stop L1 MRL updates */
  supl_stop_l1_mrl_updates();

  memset(&z_DBCellMeas, 0, sizeof(wl1_lsm_cell_meas_update_type));

#ifdef FEATURE_CGPS_LTE_CELLDB
  memset(&z_lte_mrl, 0, sizeof(lte_ml1_gps_mrl_cell_meas_update_t));
#endif

  supl_SET_id_get();

  if (u_supl_end_waiting)
  {
    z_supl_session_status.e_cf_state = C_SUPL_CF_STATE_UT0_SUPLEND;
  }
  else
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);
  }
}

/*===========================================================================

FUNCTION tm_umts_up_supl_get_accuracy_threshold

DESCRIPTION
  This function informs the TM core that network communication is over.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
static uint32 tm_umts_up_supl_get_accuracy_threshold
(
   uint32  q_hor_acc_meters,
   boolean *p_multi_report_msa
   )
{

  /* 
   * If we want to run to timeout, we set the multi report msa to true or the
   * hor accuracy to 0 for MSB. 
  */
  if (FALSE == supl_avoid_run_to_timeout_in_emergency(&z_supl_session_status))
  {
    *p_multi_report_msa = TRUE;
    q_hor_acc_meters = 0;
  }

  /* Non E911 callflow. Use the hor_accuracy provided by user */
  return q_hor_acc_meters;
}


/*===========================================================================

FUNCTION tm_umts_up_supl_inform_core_nt_assist_end

DESCRIPTION
  This function informs the TM core that network communication is over.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/

void tm_umts_up_supl_inform_core_nt_assist_end(supl_status_s_type *const cpz_supl_session_status)
{
  tm_sess_req_param_u_type req_param;
  tm_sess_req_param_u_type *p_req_param = &req_param;

  if (NULL == cpz_supl_session_status)
  {
    return;
  }

  /* Query TM-CM module to get current acquired system */
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: inform_core_nt_assist_end", 0, 0, 0);
  if (cpz_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
  {
    (void)supl_stop_is801_session(TM_STOP_REASON_GENERAL_ERROR);
    return;
  }

  /* if the association of GLONASS SV_ID and frequency established from multiple RRLP messages */
  if ((TM_POS_PRTL_RRLP == cpz_supl_session_status->u_position_protocol) &&
      tm_umts_up_supl_rrlp_assisted_glonass_enabled())
  {
    tm_post_data_payload_type    z_tm_post_data_payload;
    uint8 u_i;
    uint32 q_eph_injected_svs = 0;
    uint32 u_num_eph_injected = 0;

    /* No more assistance expected. Inject any buffered assistance data */
    for (u_i = 0; u_i < N_GLO_SV; u_i++)
    {
      if ((cpz_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId > GLO_SLOTID_OFFSET) &&
          (cpz_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId < (N_GLO_SV + GLO_SLOTID_OFFSET + 1)) &&
          (cpz_supl_session_status->z_prot_glo_eph_buffer[u_i].u_freq_num_valid == TRUE)
          )
      {

        /* if this SV already has freq. number, inject it now;
           otherwise keep data in the buffer and inject when all
           assistance have been delivered */
        tm_rrlp_glo_eph_to_TmCore_post_data_payload
           (&cpz_supl_session_status->z_prot_glo_eph_buffer[u_i],
            &z_tm_post_data_payload);

        if (tm_post_data(TM_PRTL_TYPE_UMTS_UP,
                         cpz_supl_session_status->tm_active_session_handle,
                         TM_POST_DATA_TYPE_EPH_GLO,
                         &z_tm_post_data_payload
                         ) == FALSE
            )
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Glonass eph. data post failed: SvID=%u",
                cpz_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId,
                0,
                0);
        }
        else
        {
          u_num_eph_injected++;
          q_eph_injected_svs |=
             1L << (cpz_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId - GLO_SLOTID_OFFSET - 1);
        }

        /* once processed, remove it from the buffer */
        /* 0 would be an invalid number because the api uses 65 and up for Glonass */
        cpz_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId = 0;
        cpz_supl_session_status->z_prot_glo_eph_buffer[u_i].u_freq_num_valid = FALSE;

      } /* if this SV already has freq. number */

    } /* Glonass SV for-loop */

    if (u_num_eph_injected != 0)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL GLONASS eph posted to TmCore (assist end ) 0x%lx, totoal SVs = %u",
            q_eph_injected_svs, u_num_eph_injected, 0);
    }
  }

  if (((cpz_supl_session_status->qos_timer_started) ||
       (cpz_supl_session_status->u_pdapi_fix_reported)) &&
      (cpz_supl_session_status->u_position_protocol != TM_POS_PRTL_LPP)
      )
  {
    /* qos_time has started, or fix has been achieved, no need to continue */
    return;
  }
  cpz_supl_session_status->qos_timer_started = TRUE;

  /* SUPL does not support "multiple MT reporting" */
  p_req_param->continue_param.multi_report_msa = FALSE;

  if (cpz_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
  {
    p_req_param->continue_param.op_mode = TM_SESS_OPERATION_MODE_MSA;
    p_req_param->continue_param.op_req = TM_OP_REQ_PRM;
    p_req_param->continue_param.prm_qos =
       cpz_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec / 1000;

    /* decide if we need to run to timeout or till QoS is met */
    p_req_param->continue_param.accuracy_threshold =
       tm_umts_up_supl_get_accuracy_threshold(
       cpz_supl_session_status->z_session_qos.q_hor_acc_meters,
       &p_req_param->continue_param.multi_report_msa);

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: ntwk over. prm_qos=%lu, acc=%lu",
          p_req_param->continue_param.prm_qos, 
          p_req_param->continue_param.accuracy_threshold,
          0);
  }
  else if (cpz_supl_session_status->u_agps_mode == C_SUPL_OTDOA_MODE)
  {
    p_req_param->continue_param.op_mode = TM_SESS_OPERATION_MODE_OTDOA;
    p_req_param->continue_param.op_req = TM_OP_REQ_OTDOA;
    p_req_param->continue_param.dynamic_qos_enabled = TRUE;

    p_req_param->continue_param.lr_qos =
       cpz_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec / 1000;

    p_req_param->continue_param.accuracy_threshold = cpz_supl_session_status->z_session_qos.q_hor_acc_meters;

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: ntwk over. lr_qos=%lu, acc=%lu",
          p_req_param->continue_param.lr_qos,
          p_req_param->continue_param.accuracy_threshold,
          0);
  }
  else
  {
    p_req_param->continue_param.op_mode = TM_SESS_OPERATION_MODE_MSB;
    p_req_param->continue_param.op_req = TM_OP_REQ_LR;
    p_req_param->continue_param.dynamic_qos_enabled = TRUE;

    p_req_param->continue_param.lr_qos =
       cpz_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec / 1000;

    /* decide if we need to run to timeout or till QoS is met */
    p_req_param->continue_param.accuracy_threshold =
       tm_umts_up_supl_get_accuracy_threshold(
       cpz_supl_session_status->z_session_qos.q_hor_acc_meters,
       &p_req_param->continue_param.multi_report_msa);
    /*if e911 and acuracy > 50mts check here. 
      LR_QOS/response: is already set based on NV/Network provided Qos.
    */
    p_req_param->continue_param.accuracy_threshold =
       tm_util_get_e911_early_exit_qos_config(p_req_param->continue_param.accuracy_threshold);

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: ntwk over. lr_qos=%lu, acc=%lu",
          p_req_param->continue_param.lr_qos,
          p_req_param->continue_param.accuracy_threshold,
          0);
  }



  (void)tm_sess_req(TM_PRTL_TYPE_UMTS_UP,
                    cpz_supl_session_status->tm_active_session_handle,
                    TM_SESS_REQ_CONTINUE,
                    p_req_param
                    );

}



/*===========================================================================

FUNCTION tm_umts_up_supl_start_slp_contact

DESCRIPTION
  Knowing that the SET needs SLP assistance, this function start the contact.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/

void tm_umts_up_supl_start_slp_contact()
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  pd_comms_return_type          pdcomm_connect_ret_value;
  supl_callflow_kind_enum_type  u_callflow_type = p_supl_session_status->u_callflow_kind;
  pdsm_pd_comm_protocol_e_type protocol_type = PDSM_PD_COMM_PROTOCOL_UMTS_UP_SUPL;

  tm_cm_phone_state_info_s_type phone_state_info;

  tm_cm_iface_get_phone_state(&phone_state_info);


  /* Can't run SUPL over LTE if SUPL major version less than 2 */
  if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) &&
      ((p_supl_session_status->q_supl_version & 0xFF) < 2) &&
      (u_callflow_type == C_SUPL_SESSION_UI)
      )
  {
    tm_umts_up_supl_callflow_disrupt_handler();

    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LTE not supported on SUPL 1.0: %u,%lx",
          phone_state_info.srv_system, p_supl_session_status->q_supl_version);
    return;
  }

  if ((z_pdcomm_tcp_info.curr_state == PDCOMM_STATE_CONNECTED) ||
      (z_pdcomm_tcp_info.curr_state == PDCOMM_STATE_CONNECTING) ||
      (z_pdcomm_tcp_info.curr_state == PDCOMM_STATE_DISCONNECTING)
      )
  {
    /*TCP is already connected or connecting*/
    if ((z_pdcomm_tcp_info.emergency_connection == TRUE) ||
        (supl_session_is_emergency_session() == TRUE)
        )
    {
      /*Either previous or this session is an emergency session
            Hence we can not reuse the TCP connection
        */
      /*Mark the session state as pending for TCP connection*/
      p_supl_session_status->u_tcp_connection_attempt_pending = TRUE;

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Marking connection pending in State %x", p_supl_session_status->e_cf_state, 0, 0);
      return;
    }
  }

  /* start SLP contact */
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Start SLP contact", 0, 0, 0);

  /*Reset Emergency connection boolean*/
  z_pdcomm_tcp_info.emergency_connection = FALSE;

  /* LSB in the variable passed contains "version number main" */
  pdcomm_connect_ret_value =
     tm_umts_up_supl_comm_tcp_connect((uint8)(z_supl_session_status.q_supl_version & 0x000000FF));

  if (phone_state_info.srv_system == CGPS_SRV_SYS_LTE)
  {
    protocol_type = PDSM_PD_COMM_PROTOCOL_LTE_UP;
  }
  if (PDCOMM_RESULT_OK == pdcomm_connect_ret_value) /* Already got connected */
  {
    /* tell TmCore to send pdapi event of EventGpsPdConnectionStart and
       EventGpsPdConnectionEstablished */
    tm_umts_common_send_pdapi_comm_event
       (protocol_type,
        C_COMM_BEGIN,
        p_supl_session_status->tm_active_session_handle,
        p_supl_session_status->q_pdapi_client_id
        );

    tm_umts_common_send_pdapi_comm_event
       (protocol_type,
        C_COMM_CONNECTED,
        p_supl_session_status->tm_active_session_handle,
        p_supl_session_status->q_pdapi_client_id
        );

    if (u_callflow_type == C_SUPL_SESSION_UI)
    {
      if (supl_is_wifi_scan_needed())
      {
        /* Send PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION to pdapi client
           Start timer to wait for the scan result */
        supl_request_wifi_scan();
        supl_set_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLSTART, 0);
      }
      else
      {
        /*Check if LTE cell information is available else start a timer of 100 ms 
         to retry cell information get*/
        if ((FALSE == supl_lte_cell_information_construct()))
        {
          /*Move to sub-state  C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART and start timer*/
          supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART, 0);
        }
        else
        {
          if (!supl_SuplStart_tx())
          {
            tm_umts_up_supl_callflow_disrupt_handler();
          }
        }
      }
    }
    else if (u_callflow_type == C_SUPL_SESSION_NI)
    {

      /* Can't run SUPL over LTE if SUPL major version less than 2,
         even though previous negotiation with SUPL_INIT has resulted
         in running SUPL 1.0 */
      if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) &&
          ((p_supl_session_status->q_supl_version & 0xFF) < 2)
          )
      {
        /* send SUPL_END, using the provisioned SUPL version in its
           message header, and abort this session */

        p_supl_session_status->q_supl_version =
           ((z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF) |
              (z_umts_nv_item_ram_copy.gnss_supl_version & 0x0000FF00) |
              ((z_umts_nv_item_ram_copy.gnss_supl_version << 16) & 0x00FF0000);

        (void)tm_umts_up_supl_supl_end_tx(
           &p_supl_session_status->z_session_id.z_Slp_Sess_Id,
           NULL,
           supl_versionNotSupported,
           &p_supl_session_status->supl_init_hash[0],
           p_supl_session_status->q_supl_version);

        tm_umts_up_supl_comm_close_connection();
        tm_umts_up_supl_callflow_disrupt_handler();

        return;
      }

      if (supl_is_wifi_scan_needed())
      {
        /* Send PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION to pdapi client
           Start timer to wait for the scan result */
        supl_request_wifi_scan();
        supl_set_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLPOSINIT, 0);
      }
      else
      {
        /*Check if LTE cell information is available else start a timer of 100 ms 
          to retry cell information get*/
        if (( FALSE == supl_lte_cell_information_construct()) &&
            ( FALSE == tm_E911WLANOn() ))
        {
          /*Move to sub-state C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, and start timer*/
          supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, 0);
        }
        else
        {
          if (!supl_SuplPosInit_tx())
          {
            tm_umts_up_supl_callflow_disrupt_handler();
          }
          else /*SuplPosInit Sent*/
          {
            if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
            {
              /*Start IS801 state machine*/
              if (!supl_start_is801_session(TRUE, NULL))
              {
                /*Unable to start IS801 state machine*/
                tm_umts_up_supl_callflow_disrupt_handler();
              }
            }
          }
        }
      } /* else if no wifi scan needed */
    } /* if NI */
  } /*Already got connected*/

  else if (PDCOMM_RESULT_WAIT == pdcomm_connect_ret_value) /* most likely */
  {
    if (u_callflow_type == C_SUPL_SESSION_UI)
    {
      supl_set_cf_state(C_SUPL_CF_STATE_UT0_SUPLSTART, 0);
      p_supl_session_status->u_network_session_on = TRUE;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: UT0_SuplStart starts", 0, 0, 0);
    }
    else if (u_callflow_type == C_SUPL_SESSION_NI)
    {
      supl_set_cf_state(C_SUPL_CF_STATE_UT0_SUPLPOSINIT, 0);
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: UT0_SuplPosInit starts", 0, 0, 0);
    }

    /* tell TmCore to send pdapi event of EventGpsPdConnectionStart */
    tm_umts_common_send_pdapi_comm_event
       (protocol_type,
        C_COMM_BEGIN,
        p_supl_session_status->tm_active_session_handle,
        p_supl_session_status->q_pdapi_client_id
        );


    if (supl_is_wifi_scan_needed())
    {
      /* Send PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION to pdapi client
         Start timer ( | on top of the existing UT) to wait for the scan result */
      supl_request_wifi_scan();
      supl_set_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP, 0);
    }

  }

  else
  {
    /* not possible to connect at this point */
    tm_umts_up_supl_callflow_disrupt_handler();
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: TCP conn. failed", 0, 0, 0);
  }
}


/********************************************************************
*
* tm_umts_up_supl_send_comm_failure
*
* Function description:
*   this function sends comm failure events.
*
* Parameters:
*   None
*
* Return:
*  None
*
**********************************************************************/
void tm_umts_up_supl_send_comm_failure()
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;

  z_supl_pd_info_buffer.client_id = p_supl_session_status->q_pdapi_client_id;
  (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                         p_supl_session_status->tm_active_session_handle,
                         PDSM_PD_EVENT_COMM_FAILURE,
                         &z_supl_pd_info_buffer,
                         NULL);

  (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                         p_supl_session_status->tm_active_session_handle,
                         PDSM_PD_EVENT_COMM_DONE,
                         &z_supl_pd_info_buffer,
                         NULL);
}

/********************************************************************
*
* supl_lte_cell_information_construct
*
* Function description:
*   this function gets the lte camped cell info.
*
* Parameters:
*   None
*
* Return:
*   TRUE If serving system is LTE and cell info is correctly fetched
*   FALSE otherwise
*
**********************************************************************/
boolean supl_lte_cell_information_construct()
{
  boolean ret_val = FALSE;
  tm_cm_phone_state_info_s_type phone_state_info;

  tm_cm_iface_get_phone_state(&phone_state_info);

  if (phone_state_info.srv_system == CGPS_SRV_SYS_LTE)
  {
    if (tm_umts_lte_get_cell_info(tm_core_get_current_dd_subs_id(), TM_PRTL_TYPE_UMTS_UP, NULL))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Cell info update success");
      ret_val = TRUE;
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Cell info update failed");
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, " Non LTE mode.");
    ret_val = TRUE;
  }
  return ret_val;
}

/********************************************************************
*
* tm_umts_up_supl_callflow_disrupt_handler
*
* Function description:
*   this function properly handles SUPL standard "UT 1-3" expiration.
*
* Parameters:
*   None
*
* Return:
*  None
*
*********************************************************************
*/

void tm_umts_up_supl_callflow_disrupt_handler()
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"tm_umts_up_supl_callflow_disrupt_handler   cf_state protocol callflow op_mode agps_mode fix_reported supl_close_waiting");
  MSG_7(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"tm_umts_up_supl_callflow_disrupt_handler %x       %d       %d      %d        %d           %d       %d", 
        p_supl_session_status->e_cf_state, 
        p_supl_session_status->u_position_protocol, 
        p_supl_session_status->u_callflow_kind, 
        p_supl_session_status->tm_sess_req_param.pd_option.operation_mode, 
        p_supl_session_status->u_agps_mode, 
        p_supl_session_status->u_pdapi_fix_reported, 
        p_supl_session_status->supl_close_waiting);
 
  if ( p_supl_session_status->e_cf_state != C_SUPL_CF_STATE_NULL )
  {
    if (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND))
    {
      tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);

      /* close data link if connected or connecting */
      tm_umts_up_supl_comm_close_connection();
    }

    else
    {
      /* still waiting for sending SUPL_END, but clear all other states */
      p_supl_session_status->e_cf_state = C_SUPL_CF_STATE_UT0_SUPLEND;
    }
  }

  /*If its UI SUPL-IS801 optimal modes session, then let IS801 take care of the error */
  if ((p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801) &&
      (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_UI) &&
      (p_supl_session_status->tm_sess_req_param.pd_option.operation_mode > PDSM_SESSION_OPERATION_MSASSISTED) &&
      (p_supl_session_status->tm_sess_req_param.pd_option.operation_mode < PDSM_SESSION_OPERATION_REF_POSITION)
      )
  {
    (void)supl_stop_is801_session(TM_STOP_REASON_GENERAL_ERROR);
    /* Stop L1 MRL updates */
    supl_stop_l1_mrl_updates();
    return;
  }

  if (((p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED) &&
       (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_UI)
       ) ||
      (tm_core_is_internal_session_on() == TRUE)
      )
  {

    z_supl_pd_info_buffer.client_id = p_supl_session_status->q_pdapi_client_id;

    (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                           p_supl_session_status->tm_active_session_handle,
                           PDSM_PD_EVENT_UPDATE_FAILURE,
                           &z_supl_pd_info_buffer,
                           NULL);

    p_supl_session_status->u_network_session_on = FALSE;

    /* If PDAPI Fix is not reported & we are not waiting for SUPL Close, & we
     * have not reported an AGNSS PLI error to the server, just let tm core
     * know that not more assistance data to be expected */
    if ((!p_supl_session_status->u_pdapi_fix_reported ) &&
        (!p_supl_session_status->supl_close_waiting ) &&
        (p_supl_session_status->u_position_protocol != TM_POS_PRTL_LPP) &&
        (p_supl_session_status->u_agnss_error_reported != TRUE))
    {
      tm_umts_up_supl_inform_core_nt_assist_end(p_supl_session_status);
    }
    else if ( (!p_supl_session_status->u_pdapi_fix_reported ) &&
              (!p_supl_session_status->supl_close_waiting ) &&
              (p_supl_session_status->u_agnss_error_reported != TRUE) &&
         ((p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP) &&
          (tm_lpp_up_get_tm_session_done() != TRUE)))
    {
      tm_umts_up_supl_inform_core_nt_assist_end(p_supl_session_status);
    }
    else
    {
      /* If PDAPI fix reported, or not waiting for supl_close, or we sent an 
       * AGNSS PLI error to the server, tear down TM Core session */
      tm_sess_stop_reason_e_type stop_reason;
      if (!p_supl_session_status->u_pdapi_fix_reported)
      {
        stop_reason = TM_STOP_REASON_GENERAL_ERROR;
      }
      else
      {
        /*MO MSB and fix is already reported. Call it as succesfull session*/
        stop_reason = TM_STOP_REASON_COMPLETED;
      }

      tm_umts_up_supl_inform_core_stop_session(stop_reason);
      tm_umts_up_supl_session_init(FALSE);
    }
  }
  else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
  {
    tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
    tm_umts_up_supl_session_init(FALSE);

  }
  else
  {
    tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
    tm_umts_up_supl_session_init(FALSE);
  }

  /* Stop L1 MRL updates */
  supl_stop_l1_mrl_updates();

}


/********************************************************************
*
* tm_umts_up_supl_terminate_timer_cancel_cf_state
*
* Function description:
*   this function terminates SUPL timer(s) and cancels either one
*   call flow state, or all of them.
*
* Parameters:
*   e_timer_state_to_terminate - the call flow state to cancel; if
*     C_SUPL_CF_STATE_NULL, cancel all.
*
* Return:
*  None
*
*********************************************************************
*/

void tm_umts_up_supl_terminate_timer_cancel_cf_state(supl_cf_state_enum_type e_timer_state_to_terminate)
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  pending_supl_end     *p_pending_supl_end = &z_pending_supl_end[0];

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Cancel 0x%X from 0x%X SUPL state", e_timer_state_to_terminate,
        p_supl_session_status->e_cf_state);

  if (e_timer_state_to_terminate == C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP)
  {
    (void)os_TimerStop(supl_wifi_wait_timer);
  }
  else
  {
    if ((0 != (e_timer_state_to_terminate & C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART)) ||
        (0 != (e_timer_state_to_terminate & C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT)))
    {
      (void)os_TimerStop(supl_lte_get_cell_info_retry_timer);
    }
    else
    {
      (void)os_TimerStop(supl_cf_timer);
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL CF timer cancelled");
    }
  }

  /* if terminating UT0-SuplEnd, clear all the pending SuplEnd in the que */
  if (e_timer_state_to_terminate & C_SUPL_CF_STATE_UT0_SUPLEND)
  {
    memset(p_pending_supl_end, 0, sizeof(pending_supl_end) * C_MAX_NUM_PENDING_SUPL_END);
  }

  if (e_timer_state_to_terminate != C_SUPL_CF_STATE_NULL)
  {
    p_supl_session_status->e_cf_state &= (supl_cf_state_enum_type)~e_timer_state_to_terminate;
  }
  else
  {
    memset(p_pending_supl_end, 0, sizeof(pending_supl_end) * C_MAX_NUM_PENDING_SUPL_END);
    p_supl_session_status->e_cf_state = C_SUPL_CF_STATE_NULL;
  }
}



/********************************************************************
*
* tm_umts_up_supl_pdapi_mapping
*
* Function description:
*   this function interprets PDAPI provided session parameters for
*   the SUPL protocol module.
*
* Parameters:
*   pointer to structure containing PDAPI session parameters.
*
* Return:
*  None
*
*********************************************************************
*/

boolean tm_umts_up_supl_pdapi_mapping(prtl_start_sess_req_param_s_type *start_sess_req_param)
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  boolean u_ret_val = TRUE;
  tm_cm_phone_state_info_s_type phone_state_info;
  boolean is_lpp_enabled = FALSE;
  boolean is_rrlp_enabled = FALSE;
  boolean no_sim_found = FALSE;

  if (NULL == start_sess_req_param)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Null session req param", 0, 0, 0);
    return FALSE;
  }

  tm_cm_iface_get_phone_state(&phone_state_info);


  /* check for conditions that SUPL cannot run */
  if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
  {
    supl_SET_id_get();
    /* Retrieve the IMSI/IMEI value from the USIM*/
    if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
    {
      no_sim_found = TRUE; /* SUPL can't run: no IMSI hence no SIM card */
    }
  }

  if (no_sim_found)
  {
    if (start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_MSASSISTED)
    {

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "No IMSI, can't run MSA", 0, 0, 0);
      return FALSE;
    }
    else
    {
      start_sess_req_param->pd_option.operation_mode =
         PDSM_SESSION_OPERATION_STANDALONE_ONLY;

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "No IMSI, default to standalone", 0, 0, 0);

    }
  }


  /* If provisioned not to support AGPS in GSM or WCDMA for SUPL */
  if (((phone_state_info.srv_system == CGPS_SRV_SYS_GSM) &&
       !(z_umts_nv_item_ram_copy.mo_lr_2g_support &
            NV_AAGPS_2G_MO_LR_HIGH_ACCURACY
         )
       ) ||
      ((phone_state_info.srv_system == CGPS_SRV_SYS_WCDMA) &&
       !(z_umts_nv_item_ram_copy.mo_lr_support &
            NV_AAGPS_3G_MO_LR_HIGH_ACCURACY
         )
       )
      )
  {
    start_sess_req_param->pd_option.operation_mode =
       PDSM_SESSION_OPERATION_STANDALONE_ONLY;

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "G/W AGPS provisioned out, default to standalone", 0, 0, 0);

  }

  if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) && (tm_core_get_lte_lpp_up_enable() == TRUE))
  {
    is_lpp_enabled = TRUE;
  }
  else if ((phone_state_info.srv_system == CGPS_SRV_SYS_NR5G) && (tm_core_get_nr_lpp_up_enable() == TRUE))
  {
    is_lpp_enabled = TRUE;
  }
  /*In 911 on WLAN, there are chances that srv_system and emergency_conn_srv_system 
    are NONE. if VoWIFI 911 is enabled, then select LPP*/
  else if ( ( CGPS_SRV_SYS_WLAN == phone_state_info.emerg_conn_srv_system ) ||
       ( CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys() ) ) 
  {
    is_lpp_enabled = TRUE;
  }  
  else
  {
    is_lpp_enabled = FALSE;
  }




  is_rrlp_enabled = (TM_POS_PRTL_RRLP == tm_umts_up_supl_select_pos_prot());

  /* Set the agps_mode for MO request based on the PDAPI parameters */
  if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UNDEF)
  {

    if (start_sess_req_param->pd_option.session == PDSM_PD_SESS_TYPE_DATA_DLOAD)
    {
        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED | PDSM_POS_MODE_4G_UP_MS_BASED) != 0)
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
        }
        else if (is_rrlp_enabled &&
                 TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED))
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
        }
        else
        {
          u_ret_val = FALSE;
        }
      }

    else if (start_sess_req_param->pd_option.session == PDSM_PD_SESS_TYPE_NEW ||
             start_sess_req_param->pd_option.session == PDSM_PD_SESS_TYPE_TRACK_IND)
    {
      /* if the mode is not undef at this point, this is an MT session, and the mode has been
         set when the request is accepted. Going through the "NT client", from TmCore,
         start_sess_req_param->pd_option.operation_mode would be either MSBASED or MSASSISTED,
         and would not contain AUTONUMOUS. So if this mode has been set already, don't alter it */

      if ((start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_MSBASED) ||
          (start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_OPTIMAL_SPEED)
          )
      {
          if ((is_lpp_enabled == TRUE) && TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_BASED))
          {

            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
          }
          else if ((is_lpp_enabled == FALSE) &&
                   TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED))
          {
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
          }
          else if (is_rrlp_enabled &&
                   TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED))
          {
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
          }
          else if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_STANDALONE))
          {
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_AUTO;
          }
          else
          {
            u_ret_val = FALSE;
          }
        }
      else if ((start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_MSASSISTED) ||
               (start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_OPTIMAL_ACCURACY)
               )
      {

        if ((is_lpp_enabled == TRUE) &&
            TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_ASSISTED))
        {
          if (start_sess_req_param->pd_qos.gps_session_timeout == 0)
          {
            p_supl_session_status->u_agps_mode = C_SUPL_OTDOA_MODE;
          }
          else
          {
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
          }
        }
        else if (is_lpp_enabled == FALSE)
        {
          if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED))
          {
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
          }
        }
        else
        {
          u_ret_val = FALSE;
        }
      }
      else if  (start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_REF_POSITION)
      {
        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED | PDSM_POS_MODE_4G_UP_MS_BASED) != 0)
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
          p_supl_session_status->u_force_request_ref_location = TRUE;
        }
        else if (is_rrlp_enabled &&
                 TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED))
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
        }
        else
        {
          u_ret_val = FALSE;
        }

          }
      else if ((start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_STANDALONE_ONLY) ||
               (start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_OPTIMAL_DATA)
               )
      {
        /* note standalone_only should not reach here as it is handled by TM core and is not passed down to
           protocol modules; but in case it gets here, handle it gracefully */
        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_STANDALONE))
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_AUTO;
        }
        else
        {
          if ((start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_STANDALONE_ONLY) &&
              (no_sim_found == TRUE) &&
              (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_MSB_TO_STANDALONE_ALLOWED_IF_NO_SIM))
              )
          {
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_AUTO;
          }
          else
          {
            u_ret_val = FALSE;
          }
        }
      }
      else if (start_sess_req_param->pd_option.operation_mode == PDSM_SESSION_OPERATION_CELL_ID)
      {
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_CELL_ID;
      }

      else
      {
        u_ret_val = FALSE;
      }

    } /* session == PDSM_PD_SESS_TYPE_NEW  */

    else /* all other session types */
    {
      u_ret_val = FALSE;
    }

  } /* if u_gps_mode undef */

  if (u_ret_val)
  {
    p_supl_session_status->q_pdapi_client_id = start_sess_req_param->client_id;
    p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec = start_sess_req_param->pd_qos.gps_session_timeout * 1000;
    p_supl_session_status->z_session_qos.q_hor_acc_meters = start_sess_req_param->pd_qos.accuracy_threshold;

    if (start_sess_req_param->pd_option.supl_application_id.u_validity &
           PDAPI_SUPL_APPLICATION_ID_VALID)
    {
      memscpy(&p_supl_session_status->z_supl_app_id,
              sizeof(p_supl_session_status->z_supl_app_id),
              &start_sess_req_param->pd_option.supl_application_id,
              sizeof(start_sess_req_param->pd_option.supl_application_id)
              );
    }

    if (!((p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI) &&
          (p_supl_session_status->q_supl_version != 0)
          )
        )
    {
      /* as long as this is not NI and supl version yet to be set, set supl
         version for this session now. Note for UI, call flow kind is not
         yet set at this point */
      p_supl_session_status->q_supl_version =
         ((z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF) |
            (z_umts_nv_item_ram_copy.gnss_supl_version & 0x0000FF00) |
            ((z_umts_nv_item_ram_copy.gnss_supl_version << 16) & 0x00FF0000);
    }

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "MO QoS %lu, %lu msec, %lu acc", p_supl_session_status->q_pdapi_client_id,
          p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec,
          p_supl_session_status->z_session_qos.q_hor_acc_meters);
  }

  return u_ret_val;

}

/*
 ******************************************************************************
 * Function description:
 *  This function handles the negotiation of SUPL version numbers between the
 *  SET and the SLP.
 *
 * Parameters:
 *
 *  p_pdu_decoded: pointer to decoded SLP message.
 *
 *  p_supl_version_for_calling_func: pointer for the function to populate for
 *    the calling function as to what version number to use.
 *
 *  u_session_num: applicable to areaEventTrigger session; use 0xFF otherwise.
 *
 * Return value:
 *
 *    slp_mess_header_check_result_e_type: to indication if the SLP message is
 *      to be rejected due to version not supported.
 *
 *
 ******************************************************************************
*/

slp_mess_header_check_result_e_type supl_version_negotiation
(supl_ULP_PDU *p_pdu_decoded,
 uint32 *p_supl_version_for_calling_func,
 uint8  u_session_num)
{

  uint32 q_negotiated_supl_version;
  uint32 q_provisioned_supl_ver_maj;
  slp_mess_header_check_result_e_type   e_slp_mess_header_check_result;
  uint32 q_existing_session_supl_version;

  boolean v_is_LTE = FALSE;
  tm_cm_phone_state_info_s_type phone_state_info;

  if ((NULL == p_pdu_decoded) ||  (NULL == p_supl_version_for_calling_func))
  {
    return C_SLP_MESS_VERSION_CHECK_FAILED;
  }

  tm_cm_iface_get_phone_state(&phone_state_info);
  if (phone_state_info.srv_system == CGPS_SRV_SYS_LTE)
  {
    v_is_LTE = TRUE;
  }
  else
  {
    v_is_LTE = FALSE;
  }

  e_slp_mess_header_check_result = C_SLP_MESS_HEADER_STATUS_UNKNOWN;

  if (u_session_num >= C_MAX_GEOFENCE_NUM) /* in which case it should be 0xFF */
  {
    q_existing_session_supl_version = z_supl_session_status.q_supl_version;
  }
  else
  {
    q_existing_session_supl_version = z_supl_area_event_session_status[u_session_num].q_supl_version;
  }

  /* incoming message SUPL major version number */
  q_negotiated_supl_version = p_pdu_decoded->version.maj;

  /* for min. and servind, only the SET provisioned number are to be used */
  q_negotiated_supl_version |=
     (z_umts_nv_item_ram_copy.gnss_supl_version & 0x0000FF00) |
        ((z_umts_nv_item_ram_copy.gnss_supl_version << 16) & 0x00FF0000);

  /********  SUPL version negotiation: ************************/

  q_provisioned_supl_ver_maj =
     (z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF;

  /* all non-SUPL_INIT messages */
  if (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT)
  {
    if (p_pdu_decoded->version.maj !=
           (q_existing_session_supl_version & 0x000000FF))
    {
      e_slp_mess_header_check_result |= C_SLP_MESS_VERSION_UNSUPPORTED;
    }
    q_negotiated_supl_version = q_existing_session_supl_version;
  } /* Non-SUPL_INIT */

  else  /* for SUPL_INIT */
  {
    /* note only when the device is provisioned to be SUPL2 or higher, can it
       examine the Ver2Extension of "minimumMajorVersion" */
    if (p_pdu_decoded->version.maj > q_provisioned_supl_ver_maj)
    {

      /* regardless if the SUPL_INIT can be entertained, device will respond
         with provisioned SUPL version number */
      q_negotiated_supl_version =
         ((z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF) |
            (z_umts_nv_item_ram_copy.gnss_supl_version & 0x0000FF00) |
            ((z_umts_nv_item_ram_copy.gnss_supl_version << 16) & 0x00FF0000);


      if (!((q_provisioned_supl_ver_maj >= 2) &&
            p_pdu_decoded->message.u.msSUPLINIT->m.ver2_SUPL_INIT_extensionPresent &&
            p_pdu_decoded->message.u.msSUPLINIT->ver2_SUPL_INIT_extension.m.minimumMajorVersionPresent &&
            (p_pdu_decoded->message.u.msSUPLINIT->ver2_SUPL_INIT_extension.minimumMajorVersion <=
                q_provisioned_supl_ver_maj
             )
            )
          )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL version Major Ver. higher than supported", 0, 0, 0);
        e_slp_mess_header_check_result |= C_SLP_MESS_VERSION_UNSUPPORTED;
      }
      q_supl_ver_maj_after_supl_init_glob = q_negotiated_supl_version; /* NV provisioned version used */

      if ((q_negotiated_supl_version < 2) && v_is_LTE)
      {

        /* SUPL 1 does not support LTE: If after applying existing version negotiation,
           the agreed-on version number is lower than SUPL2, and it is on LTE, mark version
           number as 0 so that the call function will not attempt any SUPL activities. */
        q_negotiated_supl_version = 0;
      }

    } /* SUPL_IINT's version > device provisioned version */

    else /* SUPL_INIT's version <= device provisioned version */
    {

      if ((p_pdu_decoded->version.maj < 2) && v_is_LTE)
      {
        /* even if provisioned version is higher than SUPL_INIT's, if SUPL_INIT's asks for SUPL 1 and the device
           is camped on LTE, treat it as versionNotSupported with a SUPL_END of the provisioned version */
        e_slp_mess_header_check_result |= C_SLP_MESS_VERSION_UNSUPPORTED;

        /* if provisioned major version is less than 2, in LTE, no SUPL is supported, mark the negotiated version to 0 */
        if (q_provisioned_supl_ver_maj < 2)
        {
          q_negotiated_supl_version = 0; /* the incoming SUPL_INIT is to be ignored */
        }
        else /* otherwise, respond with the provisioned version */
        {
          q_negotiated_supl_version =
             ((z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF) |
                (z_umts_nv_item_ram_copy.gnss_supl_version & 0x0000FF00) |
                ((z_umts_nv_item_ram_copy.gnss_supl_version << 16) & 0x00FF0000);

          q_supl_ver_maj_after_supl_init_glob = q_negotiated_supl_version; /* SUPL_INIT's SUPL ver used */
        }

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LTE not supported by SUPL 1: %u, %u, %lx",
              p_pdu_decoded->version.maj,
              q_provisioned_supl_ver_maj,
              q_supl_ver_maj_after_supl_init_glob);

      } /* SUPL 1 and LTE */

      else /* normal case:  Not SUPL_INIT (1.x.x) in LTE */
      {
        q_supl_ver_maj_after_supl_init_glob = q_negotiated_supl_version; /* SUPL_INIT's SUPL ver used */
        MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_INIT verMaj <= NV provisioned: N_Maj:%u, N_Servind:%u, U_Maj:%u, U_Servind:%u, %lu, %lu",
              p_pdu_decoded->version.maj,
              p_pdu_decoded->version.servind,
              (uint8)q_provisioned_supl_ver_maj,
              (uint8)(z_umts_nv_item_ram_copy.gnss_supl_version & 0xFF),
              q_provisioned_supl_ver_maj,
              q_supl_ver_maj_after_supl_init_glob);
      }

    } /* SUPL_INIT's version <= device provisioned version */

  } /* else : SUPL_INIT */

  *p_supl_version_for_calling_func = q_negotiated_supl_version;
  /* for the consumption of the calling func. which was defined by the adding
     global variable q_supl_ver_maj_after_supl_init_glob */

  return e_slp_mess_header_check_result;

}


/*
 ******************************************************************************
 * Function description:
 *  Decode ASN1 encoded SUPL messages coming from the TCP connection.
 *
 * Parameters:
 *
 *  Data_ptr: pointer to the lowest address byte in SUPL data buffer
 *
 *  w_data_len: length of the SUPL message
 *
 *
 * Return value:
 *
 *    TRUE: successfully docoded; FALSE: protocol error has occured.
 *
 *
 ******************************************************************************
*/

boolean  tm_umts_up_supl_message_decode(uint8 *p_data, uint16 w_data_len)
{
  supl_ULP_PDU z_supl_pdu_decoded;
  supl_ULP_PDU *p_pdu_decoded = &z_supl_pdu_decoded;


  int asn1_decode_status = 0xFFFFFFFF;


  slp_mess_header_check_result_e_type   e_slp_mess_header_check_result = C_SLP_MESS_HEADER_STATUS_UNKNOWN;

  supl_slp_session_id  z_p_provided_slp_session_id;
  supl_slp_session_id *p_provided_slp_session_id = &z_p_provided_slp_session_id;

  uint32 q_supl_version = 0;

  tm_cm_phone_state_info_s_type phone_state_info;

  uint8 u_area_event_session_match_num = 0xFF;
  tm_location_notification_s_type location_notification = {0};

  if (NULL == p_data)
  {
    return FALSE;
  }


  memset(&z_supl_pdu_decoded, 0, sizeof(supl_ULP_PDU));
  memset(p_provided_slp_session_id, 0, sizeof(supl_slp_session_id));

  /*SUPL is not Enabled, ignore the message*/
  if (tm_umts_up_is_supl_enabled() == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL message ignored: SUPL not Enabled", 0, 0, 0);
    return TRUE;
  }

  tm_osys_asn1_decode_pdu(p_data, w_data_len, (void *)p_pdu_decoded, PROTO_SUPL, &asn1_decode_status);


  /* log the message: note that the 2nd argument is not used other than for Snapper to
     display a corresponding string to indicate the "connection type", a concept that no longer
     is relevant. Snapper uses 5 for the string "SUPL" */
  sm_log_svr_txrx_rep(C_LSM_LOG_SVR_RX_TYPE, 5, C_LSM_SVR_PROTOCOL_TYPE_SUPL,
                      w_data_len, p_data);

  if (asn1_decode_status == 0)
  {
    /* find if the incoming message has SLP session ID identical to that of an existing area event triggered session */
    u_area_event_session_match_num = supl_area_event_session_match(&p_pdu_decoded->sessionID);

    if (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT)
    {
      if ((z_supl_session_status.u_callflow_kind  == C_SUPL_SESSION_NONE) &&
          (u_area_event_session_match_num == 0xFF) &&
          (z_supl_session_status.e_cf_state == C_SUPL_CF_STATE_NULL)
          )
      {
        /* unexpected message, drop it */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL message arrives unexpected, drop it", 0, 0, 0);
        tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
        return TRUE;
      }
    }

    tm_cm_iface_get_phone_state(&phone_state_info);

    /* When NON emergency SUPL INIT is received and if UE is camped on WLAN, we should drop the SUPL INIT.
       NI SUPL is supported on WLAN only for 911 AGPS sessions*/
    if( ( C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT == p_pdu_decoded->message.t ) &&
        ( ( FALSE == tm_E911WLANOn() ) &&
          ( ( CGPS_SRV_SYS_NONE == phone_state_info.srv_system ) ||
            ( CGPS_SRV_SYS_OTHER == phone_state_info.srv_system ) ||
            ( CGPS_SRV_SYS_WLAN == phone_state_info.srv_system ) ) ) )
    {
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL over WLAN for non 911 AGPS not suppported, SUPL_INIT ignored");
       tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
       return TRUE; /* if SUPL NI over WLAN is blocked for Non 911 AGPS session, drop SUPl_INIT */
    }

    /* If provisioned not to support AGPS in GSM or WCDMA for SUPL */
    if (((phone_state_info.srv_system == CGPS_SRV_SYS_GSM) &&
         (z_umts_nv_item_ram_copy.mt_lr_support &
             NV_AAGPS_MT_LR_UP_GSM_WCDMA_BLOCKED
          )
         ) ||
        ((phone_state_info.srv_system == CGPS_SRV_SYS_WCDMA) &&
         (z_umts_nv_item_ram_copy.mt_lr_support &
             NV_AAGPS_MT_LR_UP_GSM_WCDMA_BLOCKED
          )
         )
        )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "G/W AGPS provisioned out, SUPL_INIT ignored", 0, 0, 0);
      tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
      return TRUE; /* if SUPL NI for GSM/WCDMA is blocked by NV, drop SUPl_INIT */
    }

    if (u_area_event_session_match_num == 0xFF)
    {
      /* Incoming message's SLP SessionID does not match any existing areaEvenTrigger session */
      e_slp_mess_header_check_result =  supl_message_overhead_proc(p_pdu_decoded, NULL);
    }
    else
    {
      /* Incoming message's SLP SessionID matches with an existing areaEvenTrigger session:  */
      if (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT)
      {
        e_slp_mess_header_check_result = C_SLP_MESS_HEADER_OK_NEW;
      }
      else /* Need to send SUPL_END statusCode = sessionID mimatch */
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Incoming SUPL_INIT SLP SessionID same as existing areaEvent: slot %u", u_area_event_session_match_num, 0, 0);
        e_slp_mess_header_check_result = C_SLP_MESS_SLP_SESS_ID_PROBLEM_SUPLINIT_EXISTING_AREA_EVENT;
      }
    }
    e_slp_mess_header_check_result |= supl_version_negotiation(p_pdu_decoded,
                                                               &q_supl_version,
                                                               u_area_event_session_match_num);

    /* if negotiated version is SUPL 1.0 and camped on LTE, the negotiation function would
       indicate q_supl_version being 0, drop the message */
    if ((q_supl_version == 0) ||
        (e_slp_mess_header_check_result & C_SLP_MESS_VERSION_CHECK_FAILED)
        )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL 1.0 attempt over LTE, or version check failed: SUPL_INIT ignored - %u, %lx",
            q_supl_version, e_slp_mess_header_check_result, 0);
      tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
      return TRUE;
    }

    if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLTRIGGEREDSTOP)
    {
      /* do not take action of any sessionID consistency because such ID may be
         for an existing areaEvent session whose sessionIDs may not neccessarily be
         the same as the one in the SUPL_INIT(sessionInforQuery) */
      e_slp_mess_header_check_result &= ~C_SLP_MESS_SLP_SESS_ID_PROBLEM;
      e_slp_mess_header_check_result &= ~C_SLP_MESS_SET_SESS_ID_PROBLEM;
    }

    if (!(e_slp_mess_header_check_result & C_SLP_MESS_SLP_SESS_ID_MISSING))
    {
      /* for the consumption of later SUPL_END */
      supl_util_server_session_id_organizer(&p_pdu_decoded->sessionID, p_provided_slp_session_id);
    }

    if ((e_slp_mess_header_check_result == C_SLP_MESS_HEADER_OK_NEW) ||
        (e_slp_mess_header_check_result == C_SLP_MESS_HEADER_OK_CONSISTENT)
        )
    {

      if (e_slp_mess_header_check_result == C_SLP_MESS_HEADER_OK_NEW)
      {
        if (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLTRIGGEREDSTOP)
        {
          supl_store_slp_sessionId(p_pdu_decoded); /* stores SLP session ID */
        }
      }

      if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT)
      {
        /* it should only enter here if the call to hash generation function
           returned FALSE; otherwise, SUPL_INIT should be decoded in tm_umts_up_supl_hash_proc() */
        (void)supl_SuplInit_proc(p_pdu_decoded, p_data, w_data_len, NULL);
      }
      else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLRESPONSE)
      {
        (void)supl_SuplResponse_proc(p_pdu_decoded);
      }
      else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLPOS)
      {
        (void)supl_SuplPos_proc(p_pdu_decoded);
      }
      else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLEND)
      {
        supl_SuplEnd_proc(p_pdu_decoded);
      }
      else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_AUTHRESP)
      {
        //process_supl_authresp( p_pdu_decoded );
      }
      else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLTRIGRESPONSE)
      {
        supl_SuplTriggerResponse_proc(p_pdu_decoded);
      }
      else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLREPORT)
      {
        supl_SuplReport_proc(p_pdu_decoded);
      }
      else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLTRIGGEREDSTOP)
      {
        supl_SuplTrigStop_proc(p_pdu_decoded);
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LSM: Invalid SUPL message received from PDCOMM", 0, 0, 0);
      }

    } /* close if-passed-overhead-check */

    else
    {
      if (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLEND)
      {
        if ((((z_umts_nv_item_ram_copy.q_gnss_lock_ctrl_for_oem)& PDSM_PA_SUPL_NI_OEM_CTRL)!= 0)&&
            (C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT == p_pdu_decoded->message.t) 
           )
        {
          /* NvItem gnss_lock_ctrl_for_oem has locked GPS NI for SUPL */
          /* Decoding failed, send Privacy notification if SUPL is locked through OEM NV */
          /* Construct Reject location notification */
          location_notification.q_prtl_type = TM_PRTL_TYPE_UMTS_UP;
          location_notification.e_pdapi_client_id = pdsm_ni_client_id_get();
          location_notification.e_pdapi_client_type = PDSM_CLIENT_TYPE_NI;
          location_notification.e_oper_mode = TM_SESS_OPERATION_MODE_MSB;
          location_notification.e_sub_id = tm_core_get_current_dd_subs_id();
          location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_POSITION;
          location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_REJECTED;
          location_notification.v_location_privacy_override_set = FALSE;
          location_notification.v_emergency_bit_set = FALSE;
      
          tm_construct_location_notification_ind(location_notification); 
        }
        
        if (e_slp_mess_header_check_result & C_SLP_MESS_SLP_SESS_ID_MISSING)
        {
          /* p_provided_slp_session_id->present is FALSE in this case */
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       NULL,
                                                       supl_StatusCode_invalidSessionId,
                                                       NULL,
                                                       q_supl_version
                                                       );
        }

        else if (e_slp_mess_header_check_result & C_SLP_MESS_SLP_SESS_ID_PROBLEM)
        {
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       NULL,
                                                       supl_StatusCode_invalidSessionId,
                                                       NULL,
                                                       q_supl_version);
        }

        else if (e_slp_mess_header_check_result & C_SLP_MESS_SLP_SESS_ID_PROBLEM_SUPLINIT_EXISTING_AREA_EVENT)
        {
          supl_set_session_id z_set_session_id_immediate_use;

          memset(&z_set_session_id_immediate_use, 0, sizeof(supl_set_session_id));
          if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
          {
            /* if SET session ID (ismi/msisdn... is still unknow at this point,
               this function will retried it */
            supl_SET_id_get();
          }
          if (z_Set_Id.set_id_choice != C_SETID_UNKNOWN)
          {
            z_set_session_id_immediate_use.w_Session_Id = 0xFFFF; /* since only up to 16 existing sessions supported, use 0xFFFF for this purpose */
            memscpy(&z_set_session_id_immediate_use.z_Set_Id, sizeof(z_set_session_id_immediate_use.z_Set_Id),
                    &z_Set_Id, sizeof(z_Set_Id));

            (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                         &z_set_session_id_immediate_use,
                                                         supl_StatusCode_invalidSessionId,
                                                         NULL,
                                                         q_supl_version);
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SET_ID unavailable: %d, ignore SUPL_INT to be rejected",
                  z_Set_Id.set_id_choice);

            return TRUE;
          }

        }

        else if (e_slp_mess_header_check_result & C_SLP_MESS_SET_SESS_ID_MISSING)
        {
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       NULL,
                                                       supl_StatusCode_invalidSessionId,
                                                       NULL,
                                                       q_supl_version);
        }


        else if (e_slp_mess_header_check_result & C_SLP_MESS_SET_SESS_ID_PROBLEM)
        {
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       &z_supl_session_status.z_session_id.z_Set_Sess_Id,
                                                       supl_StatusCode_invalidSessionId,
                                                       NULL,
                                                       q_supl_version);
        }

        else if (e_slp_mess_header_check_result & C_SLP_MESS_VERSION_UNSUPPORTED)
        {
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       NULL,
                                                       supl_StatusCode_versionNotSupported,
                                                       NULL,
                                                       q_supl_version);
        }
        else
        {
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       NULL,
                                                       supl_StatusCode_unspecified,
                                                       NULL,
                                                       q_supl_version);
        }

        tm_umts_up_supl_callflow_disrupt_handler();

      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Invalid Session ID in SUPL_END: No need to resend a SUPL END", 0, 0, 0);
      }
    } /* failed overhead check */

    tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);

  } /* end-if-decoded-ok */

  else /* decode status showing error */
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: ASN1 decoding failed", 0, 0, 0);
    if (asn1_decode_status != C_ASN1_DECODE_BUFFER_NOT_ALLOCATED)
    {
      tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
    }
  }

  return TRUE;
}


/*===========================================================================

FUNCTION tm_umts_up_supl_tcp_connect_notif

DESCRIPTION SUPL module's actions upon knowing TCP connection is up

DEPENDENCIES
  None.

RETURN VALUE
  none.

SIDE EFFECTS
  None.

===========================================================================*/

void  tm_umts_up_supl_tcp_connect_notif(void)
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  pending_supl_end     *p_pending_supl_end = &z_pending_supl_end[0];
  uint8                u_i;

  tm_cm_phone_state_info_s_type phone_state_info;
  pdsm_pd_comm_protocol_e_type protocol_type = PDSM_PD_COMM_PROTOCOL_UMTS_UP_SUPL;

  if (!supl_is_in_any_ut0() &&
      (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NONE)
      )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Nothing to do with the tcp link, tear down", 0, 0, 0);
    tm_umts_up_supl_comm_close_connection();
    return;
  }

  tm_cm_iface_get_phone_state(&phone_state_info);

  if (phone_state_info.srv_system == CGPS_SRV_SYS_LTE)
  {
    protocol_type = PDSM_PD_COMM_PROTOCOL_LTE_UP;
  }

  /* tell TmCore to send pdapi event of EventGpsPdConnectionEstablished */
  tm_umts_common_send_pdapi_comm_event
     (protocol_type,
      C_COMM_CONNECTED,
      p_supl_session_status->tm_active_session_handle,
      p_supl_session_status->q_pdapi_client_id
      );


  /* TCP connected, send SUPL_END if it has been in UT0 */
  if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND)
  {

    for (u_i = C_MAX_NUM_PENDING_SUPL_END; u_i; u_i--)
    {
      if (p_pending_supl_end->u_validity == TRUE)
      {
        (void)tm_umts_up_supl_supl_end_tx(&p_pending_supl_end->slp_session_id,
                                          &p_pending_supl_end->set_session_id,
                                          p_pending_supl_end->e_supl_status_code,
                                          p_pending_supl_end->hash,
                                          p_supl_session_status->q_supl_version);

        p_pending_supl_end->u_validity = FALSE; /* this pending SUPL_END has been sent */
      }
      p_pending_supl_end++;
    }

    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT0_SUPLEND);

    /* if the connection was brought up only to send SUPL_END, terminate it now */
    if (p_supl_session_status->e_cf_state == C_SUPL_CF_STATE_NULL)
    {
      /* close connection */
      tm_umts_up_supl_comm_close_connection();

      /* cancel all SUPL timer(s) and call flow states */
      tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);

      /* clear all protocol session related variables */
      if (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NONE)
      {
        tm_umts_up_supl_session_init(FALSE);
      }

    }

    /*** Check if this SUPL_END is triggered because of a geofence being
         deleted *****/
    p_pending_supl_end = &z_pending_supl_end[0];
    for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
    {
      if ((z_supl_area_event_session_status[u_i].u_delete_pending) &&
          (p_pending_supl_end->e_supl_status_code ==
              supl_StatusCode_ver2_sessionStopped
           )
          )
      {
        memset(&z_supl_area_event_session_status[u_i], 0,
               sizeof(supl_area_event_status_s_type));
      }
      p_pending_supl_end++;
    }

  }

  /**** the following call flow states are mutually exclusive ***************/
  /*If connection attemp is pending, then close this connection and open a new connection*/
  if (p_supl_session_status->u_tcp_connection_attempt_pending)
  {

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Different TCP Connection required", 0, 0, 0);
    /* close connection */
    tm_umts_up_supl_comm_close_connection();

    p_supl_session_status->u_tcp_connection_attempt_pending = FALSE;

    /* Initializing the flags so that we can attempt a fresh
       connection*/
    z_pdcomm_tcp_info.tls_session_authentication_failure = FALSE;
    z_pdcomm_tcp_info.used_auto_configured_hslp_address = FALSE;

    /*Start SLP contact*/
    tm_umts_up_supl_start_slp_contact();

    return;

  }


  for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
  {
    if (z_supl_area_event_session_status[u_i].u_session_active)
    {

      if (z_supl_area_event_session_status[u_i].e_cf_state &
             C_SUPL_CF_STATE_UT0_SUPLTRIGGERSTART)
      {
        tm_umts_up_supl_terminate_timer_cancel_cf_state_area_event(C_SUPL_CF_STATE_UT0_SUPLTRIGGERSTART, u_i);
        if (!supl_SuplTriggerStart_tx(u_i))
        {
          /* if failing to send SUPL_TRIGGER_START */
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: failed to send SUPL_TRIG_START: slot=%u", u_i, 0, 0);
        }
      }


      if (z_supl_area_event_session_status[u_i].e_cf_state &
             C_SUPL_CF_STATE_UT0_SUPLREPORT_GF_BREACH)
      {
        tm_umts_up_supl_terminate_timer_cancel_cf_state_area_event(C_SUPL_CF_STATE_UT0_SUPLREPORT_GF_BREACH, u_i);
      }
      if (z_supl_area_event_session_status[u_i].z_pending_supl_report_info.u_valid)
      {
        if (tm_umts_up_supl_supl_report_tx
            (
              u_i,
              &z_supl_area_event_session_status[u_i].z_pending_supl_report_info.z_gm_client_breach_notify,
              NULL, NULL
              )
            )
        {
          z_supl_area_event_session_status[u_i].z_pending_supl_report_info.u_valid = FALSE;
        }

      } /* u_i areaEvent session has pending SUPL_REPORT to send */

    } /* u_i areaEvent session active */
  } /* geofene for-loop */

  /* TCP connected, send SUPL_START if it has been in UT0 */
  if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLSTART)
  {

    if (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sending SUPL_START - in UT0_SUPLSTART: %x", p_supl_session_status->e_cf_state, 0, 0);

      /*Check if LTE cell information is available else start a timer of 100 ms 
        to retry cell information get*/
      if ((FALSE == supl_lte_cell_information_construct()))
      {
        /*Move to sub-state C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART, and start timer*/
        supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART, 0);
      }
      else
      {
        if (!supl_SuplStart_tx())
        {
          tm_umts_up_supl_callflow_disrupt_handler();
        }

        /*If SUPL START is succesful, then cftimer is now set to UT1,
          If SUPL START is unsuccesful, then cftimer is stopped.
          So need not take care of cftimer, just cancel the UT0 state
        */
        z_supl_session_status.e_cf_state &= (supl_cf_state_enum_type)~C_SUPL_CF_STATE_UT0_SUPLSTART;
      }
    }
    else /* still wait for WiFi scan result */
    {
      tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT0_SUPLSTART);

      z_supl_session_status.e_cf_state = C_SUPL_CF_STATE_WIFI_WAIT_SUPLSTART; /* note state change but no timer change */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TCP connected, still waiting for WIFI scan result", 0, 0, 0);
    }

  }

  /* TCP connected, send SUPL_POSINIT if it has been in UT0 */
  else if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLPOSINIT)
  {
    /* Can't run SUPL over LTE if SUPL major version less than 2,
       even though previous negotiation with SUPL_INIT has resulted
       in running SUPL 1.0 */
    if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) &&
        ((p_supl_session_status->q_supl_version & 0xFF) < 2)
        )
    {

      p_supl_session_status->q_supl_version =
         ((z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF) |
            (z_umts_nv_item_ram_copy.gnss_supl_version & 0x0000FF00) |
            ((z_umts_nv_item_ram_copy.gnss_supl_version << 16) & 0x00FF0000);

      /* send SUPL_END, using the provisioned SUPL version in its
         message header, and abort this session */
      (void)tm_umts_up_supl_supl_end_tx(
         &p_supl_session_status->z_session_id.z_Slp_Sess_Id,
         NULL,
         supl_versionNotSupported,
         &p_supl_session_status->supl_init_hash[0],
         p_supl_session_status->q_supl_version);

      tm_umts_up_supl_comm_close_connection();
      tm_umts_up_supl_callflow_disrupt_handler();

      return;
    }


    if (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sending SUPL_POSINIT - in UT0_SUPLPOSINIT: %x", p_supl_session_status->e_cf_state, 0, 0);

      /*Check if LTE cell information is available else start a timer of 100 ms 
           to retry cell information get*/
      if ((FALSE == supl_lte_cell_information_construct()))
      {
        /*Move to sub-state C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT and start timer*/
        supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, 0);
      }
      else
      {
        if (!supl_SuplPosInit_tx())
        {
          tm_umts_up_supl_callflow_disrupt_handler();
        }
        else /*SuplPosInit Sent*/
        {
          if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801 &&
              z_supl_session_status.u_callflow_kind == C_SUPL_SESSION_NI)
          {
            /*Start IS801 state machine*/
            if (!supl_start_is801_session(TRUE, NULL))
            {
              /*Unable to start IS801 state machine*/
              tm_umts_up_supl_callflow_disrupt_handler();
            }
          }
        }

        /*If SUPL POS INIT is succesful, then cftimer is now set to UT2,
          If SUPL POS INIT is unsuccesful, then cftimer is stopped.
          So need not take care of cftimer, just cancel the UT0 state
        */
        z_supl_session_status.e_cf_state &= (supl_cf_state_enum_type)~C_SUPL_CF_STATE_UT0_SUPLPOSINIT;
      }
    } /* not waiting for wifi scan result */

    else /* still wait for WiFi scan result */
    {
      tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT0_SUPLPOSINIT);

      z_supl_session_status.e_cf_state = C_SUPL_CF_STATE_WIFI_WAIT_SUPLPOSINIT; /* note state change but no timer change */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TCP connected, still waiting for WIFI scan result", 0, 0, 0);
    }

  }

  else if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLREPORT_QUERY)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT0_SUPLREPORT_QUERY);

    tm_umts_up_supl_supl_report_tx(NULL,
                                   NULL,
                                   &p_supl_session_status->supl_init_hash[0],
                                   NULL
                                   );
  }

}



/*===========================================================================

FUNCTION tm_umts_up_supl_handle_lr_data

DESCRIPTION handles TmCore provided LR data.

DEPENDENCIES
  None.

RETURN VALUE
  none.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_umts_up_supl_handle_lr_data(sm_FixInfoStructType *lr_data_ptr, boolean u_final)
{
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;
  pdsm_pd_info_s_type *p_pd_info_buffer = &z_supl_pd_info_buffer;
  pdsm_pd_event_type       pd_event = PDSM_PD_EVENT_POSITION;
  gad_ellip_alt_unc_ellip_type   z_ellipse;
  tm_lr_resp_type                *lr_resp_ptr = NULL;
  tm_pd_position_info_s_type     z_tm_pd_position_info;
  tm_ext_pos_data_s_type         z_tm_ext_pos_data;
  utc_time                       z_utc_time;
  uint32 q_LongWord;
  int32 l_LongWord;
  double d_Double;
  uint16 w_Word;
  uint32 q_pos_sources;
  uint32 q_glonass_tod_modulo_1hour_ms; /* position time stamp GLONASS TimeOfDay */
  boolean v_sess_type_ni = FALSE;

  if (p_supl_session_status->u_pdapi_fix_reported)
  {
    /* if position has been passed on to pdapi via TmCore already in this session,
       don't repeat that */
    return;
  }

  if (NULL == lr_data_ptr)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null data buffer");
    return;
  }

  if ((p_supl_session_status->u_agps_mode == C_SUPL_AGPS_NO_POSITION) ||
      ((p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_UE_BASED) && 
       (p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_AUTO))       
      )
  {	
    return; /* not supposed to receive any position from TmCore, ignore it */
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: Handle LR data %d", u_final);

  if (C_SUPL_SESSION_NI == p_supl_session_status->u_callflow_kind)
  {
    v_sess_type_ni =  TRUE;
  }
  q_pos_sources = 0;
  if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsGpsUsed)
  {
    q_pos_sources |= C_POS_SOURCE_GPS;
  }
  if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsGloUsed)
  {
    q_pos_sources |= C_POS_SOURCE_GLO;

    /* Store glonass time to report to server */
    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_GloTimeMs = lr_data_ptr->z_NavPos.q_GloTimeMs;
  }
  if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsBdsUsed)
  {
    q_pos_sources |= C_POS_SOURCE_BDS;
  }
  if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsWifiGenerated)
  {
    q_pos_sources |= C_POS_SOURCE_WLAN_AP;
  }
  if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsEcid)
  {
    q_pos_sources |= C_POS_SOURCE_ECID_LTE;
  }

  p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_pos_sources =
     z_supl_cached_pos_for_net.z_pos_info.q_pos_sources = q_pos_sources;

  /* Save the valid Glonass TOD: sanity check first */
  if ((lr_data_ptr->z_NavPos.u_GloNumFourYear == 255) ||
      (lr_data_ptr->z_NavPos.w_GloNumDaysInFourYear == 65535) ||
      (lr_data_ptr->z_NavPos.q_GloTimeMs  > DAY_MSECS))
  {
    q_glonass_tod_modulo_1hour_ms = 0xFFFFFFFF;

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
          "GlonassTOD invalid: Num4Year=%u, NumDaysIn4Year=%u, ToD_ms=%lu",
          lr_data_ptr->z_NavPos.u_GloNumFourYear,
          lr_data_ptr->z_NavPos.w_GloNumDaysInFourYear,
          lr_data_ptr->z_NavPos.q_GloTimeMs);
  }
  else
  {
    q_glonass_tod_modulo_1hour_ms = lr_data_ptr->z_NavPos.q_GloTimeMs % HOUR_MSECS;
  }

  /* construct GAD shape data from LR for multiple usage below */
  (void)tm_umts_common_utils_construct_ellip_alt_unc_ellip_from_lr(&z_ellipse, lr_data_ptr, TM_CONF_REPORTED_FROM_PE, v_sess_type_ni);

  /* Save the position for this session in case it will have to be reported to
     SLP at a later time */
  if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
  {
    /* the values in GAD shape strucutre cannot be used for LPP as LPP expects them in a different
      format. save lpp lat/long/alt seperatelty */
    d_Double = lr_data_ptr->z_NavPos.d_PosLla[0] * C_LAT_RAD_TO_GAD;

    if (lr_data_ptr->z_NavPos.d_PosLla[0] < 0) /* if latitude in the southern hemesphere */
    {
      /* then code the rest with the absolute value of the latitude */
      d_Double = -d_Double;
    }

    q_LongWord = (uint32)tm_util_nearest_long(d_Double);
    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_Lat = q_LongWord;
    /********** longitude ************/

    d_Double = lr_data_ptr->z_NavPos.d_PosLla[1] * C_LON_RAD_TO_GAD;

    l_LongWord = tm_util_nearest_long(d_Double);

    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.l_Long = l_LongWord;

    w_Word = (uint16)tm_util_nearest_long(fabs(lr_data_ptr->z_NavPos.d_PosLla[2]));
    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.w_Alt = w_Word;
  }

  p_supl_session_status->z_pending_new_pos_to_network.u_valid = TRUE;

  p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_fix_time_gps_msec =
     lr_data_ptr->z_NavPos.q_GpsTimeMs;

  p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_glonass_tod_modulo_1hour_ms =
     q_glonass_tod_modulo_1hour_ms;

  p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.w_GpsWeek =
     lr_data_ptr->z_NavPos.w_GpsWeek;

  p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_fix_time_utc_msec =
     lr_data_ptr->z_NavPos.q_UtcTimeMs;

  p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_ClockBiasUncMs =
     lr_data_ptr->z_NavPos.f_ClockBiasUncMs;

  p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_ClockBias =
     lr_data_ptr->z_NavPos.f_ClockBias;

  memscpy(&p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.z_pos,
          sizeof(p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.z_pos),
          &z_ellipse, sizeof(z_ellipse));


  if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsVelValid)
  {
    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.vel_valid = TRUE;

    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_VelEnu[0] =
       lr_data_ptr->z_NavPos.f_VelEnu[0];

    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_VelEnu[1] =
       lr_data_ptr->z_NavPos.f_VelEnu[1];

    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_VelEnu[2] =
       lr_data_ptr->z_NavPos.f_VelEnu[2];

  }

  if (u_final == FALSE)
  {
      p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.u_final = FALSE;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Intermediate pos. save", 0, 0, 0);
      return;
  }


  lr_resp_ptr = (tm_lr_resp_type *)pdsm_getbuf(sizeof(tm_lr_resp_type)); /*lint !e826 */

  if (lr_resp_ptr != NULL)
  {

    /* initialize buffer for pdapi */
    memset(p_pd_info_buffer, 0, sizeof(pdsm_pd_info_s_type));

    /* Convert from LM to IS801 format: simply to take advantage of the readily available
       function to convert position and timestamp to the format the TmCore interface uses */
    if (tm_util_convert_lr(lr_data_ptr, lr_resp_ptr))
    {
      tm_util_xlate_nav_soln_to_tm_pos_info(NULL,
                                            &z_tm_pd_position_info,
                                            &lr_data_ptr->z_NavPos,
                                            PDSM_PD_DLOAD_LOCATION,         /* dont care */
                                            PDSM_PD_DLOAD_SOURCE_STANDALONE /* dont care */);

      /* set up proper position mode in the pdapi report */
      if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
      {
        z_tm_pd_position_info.position_mode = PDSM_PD_POSITION_MODE_MSBASED;
      }
      else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
      {
        z_tm_pd_position_info.position_mode = PDSM_PD_POSITION_MODE_MSASSISTED;
      }
      else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_AUTO)
      {
        z_tm_pd_position_info.position_mode = PDSM_PD_POSITION_MODE_STANDALONE;
      }
      else
      {
        z_tm_pd_position_info.position_mode = PDSM_PD_POSITION_MODE_UNKNOWN;
      }

      tm_util_xlate_nav_soln_to_tm_ext_pos(&z_tm_ext_pos_data,
                                           &lr_data_ptr->z_NavPos);

      z_tm_ext_pos_data.h_dop = lr_data_ptr->z_NavPos.f_HDOP;
      z_tm_ext_pos_data.v_dop = lr_data_ptr->z_NavPos.f_VDOP;
      z_tm_ext_pos_data.p_dop = lr_data_ptr->z_NavPos.f_PDOP;
      z_tm_ext_pos_data.t_dop = lr_data_ptr->z_NavPos.f_TDOP;
      z_tm_ext_pos_data.g_dop = lr_data_ptr->z_NavPos.f_GDOP;
      z_tm_ext_pos_data.pos_hepe =
         sqrt((lr_data_ptr->z_NavPos.f_ErrorEllipse[1] *
               lr_data_ptr->z_NavPos.f_ErrorEllipse[1]) +
              (lr_data_ptr->z_NavPos.f_ErrorEllipse[2] *
               lr_data_ptr->z_NavPos.f_ErrorEllipse[2]));


      z_utc_time = tm_util_construct_utc_time(lr_data_ptr->z_NavPos.w_GpsWeek,
                                              lr_data_ptr->z_NavPos.q_GpsTimeMs,
                                              lr_data_ptr->z_NavPos.q_UtcTimeMs);
      z_tm_ext_pos_data.utc_time.hour = z_utc_time.q_hours;
      z_tm_ext_pos_data.utc_time.minute = z_utc_time.q_mins;
      z_tm_ext_pos_data.utc_time.sec = (uint16)((z_utc_time.q_secs * 100) + (z_utc_time.q_tenths * 10));
      z_tm_ext_pos_data.utc_time.date = (uint32)((z_utc_time.q_utc_days * 1000000) + (z_utc_time.q_month * 10000) + z_utc_time.q_year);

      pd_event = tm_util_pd_info_data_init(p_pd_info_buffer,
                                           lr_resp_ptr,
                                           p_supl_session_status->q_pdapi_client_id,
                                           NULL,
                                           &z_tm_pd_position_info,
                                           &z_tm_ext_pos_data);

      /* flag if this position is to be also sent to the network */
      if ((p_supl_session_status->u_network_session_on) &&
          (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI)
          )
      {
        z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.pos_reported_to_network = TRUE;
      }

      (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                             p_supl_session_status->tm_active_session_handle,
                             pd_event,
                             &z_supl_pd_info_buffer,
                             &lr_data_ptr->z_NavPos.z_PosFlags);
	  
      /* GPS Complete event */
      sm_log_event_report(SM_LOG_EVENT_ID_GPS_COMPLETE, 0);
      p_supl_session_status->u_pdapi_fix_reported = TRUE;

    }

    (void)pdsm_freebuf((char *)lr_resp_ptr);

  }

  /* populate the cached position structure for possible future use for an NI
     session, in SUPL_POS_INIT */

  memset(&z_supl_cached_pos_for_net, 0, sizeof(z_supl_cached_pos_for_net));
  z_supl_cached_pos_for_net.u_valid = TRUE;
  z_supl_cached_pos_for_net.z_pos_info.q_fix_time_gps_msec =
     lr_data_ptr->z_NavPos.q_GpsTimeMs;
  z_supl_cached_pos_for_net.z_pos_info.w_GpsWeek =
     lr_data_ptr->z_NavPos.w_GpsWeek;
  z_supl_cached_pos_for_net.z_pos_info.q_fix_time_utc_msec =
     lr_data_ptr->z_NavPos.q_UtcTimeMs;

  if (q_pos_sources & C_POS_SOURCE_GPS) /* if GPS SV used */
  {
    z_supl_cached_pos_for_net.z_pos_info.q_pos_sources = C_POS_SOURCE_GPS;
  }
  if (q_pos_sources & C_POS_SOURCE_GLO) /* if GLONASS */
  {
    z_supl_cached_pos_for_net.z_pos_info.q_pos_sources |= C_POS_SOURCE_GLO;
  }
  if (q_pos_sources & C_POS_SOURCE_BDS) /* if BeiDou */
  {
    z_supl_cached_pos_for_net.z_pos_info.q_pos_sources |= C_POS_SOURCE_BDS;
  }

  memscpy(&z_supl_cached_pos_for_net.z_pos_info.z_pos,
          sizeof(z_supl_cached_pos_for_net.z_pos_info.z_pos),
          &z_ellipse, sizeof(z_ellipse));


  if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsVelValid)
  {
    z_supl_cached_pos_for_net.z_pos_info.vel_valid = TRUE;

    z_supl_cached_pos_for_net.z_pos_info.f_VelEnu[0] =
       lr_data_ptr->z_NavPos.f_VelEnu[0];

    z_supl_cached_pos_for_net.z_pos_info.f_VelEnu[1] =
       lr_data_ptr->z_NavPos.f_VelEnu[1];

    z_supl_cached_pos_for_net.z_pos_info.f_VelEnu[2] =
       lr_data_ptr->z_NavPos.f_VelEnu[2];
    z_supl_cached_pos_for_net.z_pos_info.f_Vunc3dMps = lr_data_ptr->z_NavPos.f_Vunc3dMps;
    z_supl_cached_pos_for_net.z_pos_info.f_VelVertUnc = lr_data_ptr->z_NavPos.f_VuncVertMps;
  }

  /*******  check to see if reporting to the network is needed *************/

  if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_SUPLPOS_XCHG)
  {

    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
    {
      /* clear timer and call flow state */
      tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_SUPLPOS_XCHG);

      if (lr_data_ptr->z_NavPos.z_PosFlags.b_IsVelValid)
      {

        supl_Velocity z_velocity;
        supl_Horandvervel z_horandvervel;

        memset(&z_velocity, 0, sizeof(supl_Velocity));
        memset(&z_horandvervel, 0, sizeof(supl_Horandvervel));

        z_velocity.u.horandvervel = &z_horandvervel;

        supl_convert_velocity(lr_data_ptr->z_NavPos.f_VelEnu[0],
                              lr_data_ptr->z_NavPos.f_VelEnu[1],
                              lr_data_ptr->z_NavPos.f_VelEnu[2],
                              &z_velocity);

        supl_send_pos_rrlp_to_slp(p_supl_session_status, &z_ellipse, &z_velocity,
                                  lr_data_ptr->z_NavPos.q_GpsTimeMs,
                                  q_pos_sources,
                                  q_glonass_tod_modulo_1hour_ms);

      }
      else
      {
        supl_send_pos_rrlp_to_slp(p_supl_session_status, &z_ellipse, NULL,
                                  lr_data_ptr->z_NavPos.q_GpsTimeMs,
                                  q_pos_sources,
                                  q_glonass_tod_modulo_1hour_ms);
      }
    }

#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
    else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      tm_lpp_up_handle_final_lr_data(p_supl_session_status, NULL);
    }
#endif /* FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY */

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sent SUPL_POS to SLP", 0, 0, 0);
  } /* if rrpl-msrPosReq has been received */

  else if (p_supl_session_status->u_network_session_on)
  {
    /*If internal session is ON and LR is already received of NV is set to terminate SUPL 
      after final fix, kill the connection attempt and close the session*/
    if ((tm_core_is_internal_session_on() ||
         tm_core_get_end_supl_after_final_fix_nv()) &&
        ((p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLSTART) ||
         (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLPOSINIT) ||
         (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND)
         )
        )
    {
      /* send DONE event to pdapi */
      tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
      tm_umts_up_supl_comm_close_connection();
      /* cancel all SUPL timer(s) and call flow states */
      tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);
      /* clear all protocol session related variables */
      tm_umts_up_supl_session_init(FALSE);
    }
    else
    {
      /* no rrlp-msrPosReq ever received, but already engaged in a network session,
         save the position as pending for network, in case this network session requires position going back
         to the network */

      p_supl_session_status->z_pending_new_pos_to_network.u_valid = TRUE;
      p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.u_final = TRUE;
    }
  }

  else  /* there is no network session */
  {
    /* send DONE event to pdapi */
    tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);

    if (!(z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT) &&
        !(z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND)
        )
    {
      tm_umts_up_supl_session_init(FALSE);
    }
  }
}


/*===========================================================================

FUNCTION tm_supl_need_to_store_meas

DESCRIPTION Decide if the measurement should be stored/cached internally.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tm_supl_need_to_store_meas(sm_GpsMeasRptStructType const *const p_gnss_prm_data,
                                   boolean const cv_CacheMeas)
{
  if (p_gnss_prm_data != NULL)
  {
    if (((GNSS_MEAS_BLK_SRC_GPS == p_gnss_prm_data->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc) ||
         (GNSS_MEAS_BLK_SRC_GLO == p_gnss_prm_data->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc)) &&
        cv_CacheMeas)
    {
      return (TRUE);
    }
  }
  return (FALSE);
}

/*===========================================================================

FUNCTION tm_umts_up_supl_handle_prm_data

DESCRIPTION handles TmCore provided PRM data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tm_umts_up_supl_handle_prm_data(sm_GpsMeasRptStructType *p_prm_data)
{
  supl_status_s_type            *p_supl_session_status        = &z_supl_session_status;
  boolean                       v_E911                        = tm_umts_common_utils_emergency_on();
  boolean                       v_CacheMeas                   = FALSE;
  boolean                       v_StatusCheckNeeded           = FALSE;
  uint32                        q_session_remaining_time_msec = 0;
  SmGnssMeasReportStructType    *pz_SuplGnssMeasRpt           = &(p_supl_session_status->z_gnss_meas_report);

  if (p_prm_data == NULL)
  {
    return;
  }
  else
  {
    /* If some measurements are available, start caching meas & status check needed */
    v_CacheMeas = v_StatusCheckNeeded = TRUE;
  }

  /*
  * Cache all the latest measurements, with an exception only for e911 case, don't overwrite done
  * measurement with non-done measurement. Upon GNSS/RLI timer expiry the cached measurement is
  * copied to LPP Tx item & shipped out.
  */
  if (p_prm_data->z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason != GNSS_MEAS_RPT_MEAS_DONE_SM)
  {
    /* No status check needed for non-done measurement for both e911 & non-e911 cases */
    v_StatusCheckNeeded = FALSE;

    if (v_E911 == TRUE)
    {
      /* Don't overwrite the cached done meas with non-done for e911 */
      if ((GNSS_MEAS_BLK_SRC_GPS == p_prm_data->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc) &&
          ((pz_SuplGnssMeasRpt->u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GPS) != 0) &&
          (GNSS_MEAS_RPT_MEAS_DONE_SM == pz_SuplGnssMeasRpt->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS].z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason))
      {
        v_CacheMeas = FALSE;
      }
      else if ((GNSS_MEAS_BLK_SRC_GLO == p_prm_data->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc) &&
               ((pz_SuplGnssMeasRpt->u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GLONASS) != 0) &&
               (GNSS_MEAS_RPT_MEAS_DONE_SM == pz_SuplGnssMeasRpt->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS].z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason))
      {
        v_CacheMeas = FALSE;
      }
    }
  }


  MSG_9(MSG_SSID_GPSSM, MSG_LEGACY_MED, "MeasSrc (%d of %d) %d (GPS/GLO/BDS/GAL/QZSS), MeasDone %d, Filled MeasStruct 0x%x,"
        "StatusCheck %d, GloStatus %d, Cache %d, Emer %d",
        p_prm_data->z_MeasBlk.z_GnssHeader.u_SeqNum, p_prm_data->z_MeasBlk.z_GnssHeader.u_MaxMessageNum,
        p_prm_data->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc,
        (GNSS_MEAS_RPT_MEAS_DONE_SM == p_prm_data->z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason),
        p_supl_session_status->z_gnss_meas_report.u_meas_reports_bmap,
        v_StatusCheckNeeded, p_supl_session_status->u_glo_meas_status, v_CacheMeas, v_E911);

  if (p_prm_data->z_MeasBlk.z_GnssHeader.u_NumSvs == 0)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: 0 SV meas report is ignored later");
  }

  /* If we do not have a SUPL callflow going on or in MSB mode, ignore the PRM!!! */
  if ((!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_SUPLPOS_XCHG)) ||
      (p_supl_session_status->u_agps_mode & C_SUPL_AGPS_MODE_UE_BASED))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: GNSS meas. ignored - not expecting");
    return;
  }

  /* Decide if received GPS or Glonass measurements should be copied/stored internally in SM measurement report struct */
  if (tm_supl_need_to_store_meas(p_prm_data, v_CacheMeas))
  {
    tm_copy_prm_to_sm_meas_struct(p_prm_data, &p_supl_session_status->z_gnss_meas_report, v_StatusCheckNeeded);
  }

  /* SM doesn't depend of the order of measurement report from ME. Below condition checks if all the
  *  measurements expected in the epoch is received.
  */
  if (p_prm_data->z_MeasBlk.z_GnssHeader.u_SeqNum < p_prm_data->z_MeasBlk.z_GnssHeader.u_MaxMessageNum)
  {
    return; /* wait further measurements in an epoch */
  }

  /* In non-E911 case or when R2TO in emergency is not desired:-
   *
   * Ship the first DONE measurment.
   *   or
   *
   * If there are no DONE measurements, then wait for LM to report PRM timeout. RRLP depends on LM
   * PRM timeout indication to process the cached measurement in the SUPL session structure & use
   * that to report to RRLP server. LPP uses RLI/GNSS timer expiry to report to server.
  */
  pz_SuplGnssMeasRpt = &(p_supl_session_status->z_gnss_meas_report);
  if (TRUE == supl_avoid_run_to_timeout_in_emergency(p_supl_session_status))
  {
    /* Done measurement */
    if (GNSS_MEAS_RPT_MEAS_DONE_SM == pz_SuplGnssMeasRpt->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS].z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason)
    {
      if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
      {
        /* LPP meas done before time out */
        tm_lpp_up_handle_prm_data_before_timeout(p_supl_session_status);
      }
      else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
      {
        /* RRLP meas done before time out */
        tm_umts_up_rrlp_handle_prm_data(p_supl_session_status);
      }
    }
  }

  return;
}

/*===========================================================================

FUNCTION tm_umts_up_supl_hash_proc

DESCRIPTION : Decode SUPL_INIT returned in the HASH message, and take the next
              action in a Network Initiated call flow.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tm_umts_up_supl_hash_proc(uint8 *p_hash_data, uint16 w_supl_init_len)
{
  uint8 *p_hash;
  supl_ULP_PDU        z_pdu_decoded;
  supl_ULP_PDU *p_pdu_decoded = &z_pdu_decoded;

  slp_mess_header_check_result_e_type   e_slp_mess_header_check_result;

  supl_slp_session_id  z_p_provided_slp_session_id;
  supl_slp_session_id *p_provided_slp_session_id = &z_p_provided_slp_session_id;

  supl_set_session_id  z_provided_set_session_id;

  supl_status_s_type *p_supl_session_status = &z_supl_session_status;

  uint32 q_supl_version;
  int asn1_decode_status = 0xFFFFFFFF;
  tm_cm_phone_state_info_s_type phone_state_info;
  uint8 u_area_event_session_match_num = 0xFF;

  if (p_hash_data == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "HASH resp data pointer NULL", 0, 0, 0);
    return;
  }

  memset(&z_pdu_decoded, 0, sizeof(supl_ULP_PDU));
  memset(p_provided_slp_session_id, 0, sizeof(supl_slp_session_id));

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL - recv'ed HASH resp", 0, 0, 0);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Auth Fail Flag: %d Auto Config Hslp Addr Used :%d", z_pdcomm_tcp_info.tls_session_authentication_failure,
        z_pdcomm_tcp_info.used_auto_configured_hslp_address, 0);

  p_hash =  p_hash_data + w_supl_init_len;


  /*SUPL is not Enabled, ignore the message*/
  if (tm_umts_up_is_supl_enabled() == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_INIT ignored: SUPL not Enabled", 0, 0, 0);
    return;
  }
  /* If an authentication failure had occured simply try to bring up the data link.
   There is no need to proceed with Supl Init processing since already done*/
  if (z_pdcomm_tcp_info.tls_session_authentication_failure == TRUE)
  {
    memscpy(p_supl_session_status->supl_init_hash, sizeof(p_supl_session_status->supl_init_hash),
            p_hash, sizeof(p_supl_session_status->supl_init_hash));
    p_supl_session_status->u_hash_valid = TRUE;
    tm_umts_up_supl_start_slp_contact();
    return;
  }

  tm_osys_asn1_decode_pdu(p_hash_data, w_supl_init_len, (void *)p_pdu_decoded, PROTO_SUPL, &asn1_decode_status);

  /* log the message: note that the 2nd argument is not used other than for Snapper to
     display a corresponding string to indicate the "connection type", a concept that no longer
     is relevant. Snapper uses 5 for the string "SUPL" */
  sm_log_svr_txrx_rep(C_LSM_LOG_SVR_RX_TYPE, 5, C_LSM_SVR_PROTOCOL_TYPE_SUPL,
                      w_supl_init_len, p_hash_data);

  if (asn1_decode_status == 0)
  {

    tm_cm_iface_get_phone_state(&phone_state_info);

    /* When NON emergency SUPL INIT is received and if UE is camped on WLAN, we should drop the SUPL INIT.
       NI SUPL is supported on WLAN only for 911 AGPS sessions*/
    if( ( C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT == p_pdu_decoded->message.t ) &&
        ( ( FALSE == tm_E911WLANOn() ) &&
          ( ( CGPS_SRV_SYS_NONE == phone_state_info.srv_system ) ||
            ( CGPS_SRV_SYS_OTHER == phone_state_info.srv_system ) ||
            ( CGPS_SRV_SYS_WLAN == phone_state_info.srv_system ) ) ) )
    {
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL over WLAN for non 911 AGPS not suppported, SUPL_INIT ignored");
       tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
       return; /* if SUPL NI over WLAN is blocked for Non 911 AGPS session, drop SUPl_INIT */
    }


    /* If provisioned not to support AGPS in GSM or WCDMA for SUPL */
    if (((phone_state_info.srv_system == CGPS_SRV_SYS_GSM) &&
         (z_umts_nv_item_ram_copy.mt_lr_support &
             NV_AAGPS_MT_LR_UP_GSM_WCDMA_BLOCKED
          )
         ) ||
        ((phone_state_info.srv_system == CGPS_SRV_SYS_WCDMA) &&
         (z_umts_nv_item_ram_copy.mt_lr_support &
             NV_AAGPS_MT_LR_UP_GSM_WCDMA_BLOCKED
          )
         )
        )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "G/W AGPS provisioned out, SUPL_INIT ignored", 0, 0, 0);
      tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
      return; /* if SUPL NI for GSM/WCDMA is blocked by NV, drop SUPl_INIT */
    }

    /* determine if the incoming SUPL_INIT has SLP session ID identical to that of an existing area event triggered session */
    u_area_event_session_match_num = supl_area_event_session_match(&p_pdu_decoded->sessionID);
    if (u_area_event_session_match_num == 0xFF) /* no match */
    {
      e_slp_mess_header_check_result =  supl_message_overhead_proc(p_pdu_decoded, NULL);
    }
    else /* a match found - need to reject this SUPL_INIT with SUPL_END  */
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Incoming SUPL_INIT SLP SessionID same as existing areaEvent: slot %u", u_area_event_session_match_num, 0, 0);
      e_slp_mess_header_check_result = C_SLP_MESS_SLP_SESS_ID_PROBLEM_SUPLINIT_EXISTING_AREA_EVENT;
    }

    e_slp_mess_header_check_result |= supl_version_negotiation(p_pdu_decoded, &q_supl_version, 0xFF);

    /* if negotiated version is SUPL 1.0 and camped on LTE, the negotiation function would
       indicate q_supl_version being 0, drop the message */
    if ((q_supl_version == 0) ||
        (e_slp_mess_header_check_result & C_SLP_MESS_VERSION_CHECK_FAILED)
        )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL 1.0 attempt over LTE, or version check failed: SUPL_INIT ignored - %u, %lx",
            q_supl_version, e_slp_mess_header_check_result, 0);
      tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
      return;
    }


    if (!(e_slp_mess_header_check_result & C_SLP_MESS_SLP_SESS_ID_MISSING))
    {
      /* for the consumption of later SUPL_END */
      supl_util_server_session_id_organizer(&p_pdu_decoded->sessionID, p_provided_slp_session_id);
    }


    if (e_slp_mess_header_check_result == C_SLP_MESS_HEADER_OK_NEW)
    {
      /* if this function returns FALSE, either version or sessionID check
      has failed, simply ignore this message */

      /* received SUPL message */

      if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT)
      {
        (void)supl_SuplInit_proc(p_pdu_decoded, p_hash_data, w_supl_init_len, p_hash);
      }
      else
      {
        /* should not happen */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "pdcomm hash return not SUPLINIT", 0, 0, 0);
      }
    }

    else  /* something wrong with SUPL_INIT in its header */
    {

      if (e_slp_mess_header_check_result & C_SLP_MESS_SLP_SESS_ID_MISSING)
      {
        /* p_provided_slp_session_id->present is FALSE in this case */
        (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                     NULL,
                                                     supl_StatusCode_invalidSessionId,
                                                     NULL,
                                                     q_supl_version
                                                     );
      }

      else if (e_slp_mess_header_check_result & C_SLP_MESS_SLP_SESS_ID_PROBLEM_SUPLINIT_EXISTING_AREA_EVENT)
      {
        supl_set_session_id z_set_session_id_immediate_use;

        memset(&z_set_session_id_immediate_use, 0, sizeof(supl_set_session_id));
        if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
        {
          /* if SET session ID (ismi/msisdn... is still unknow at this point,
             this function will retried it */
          supl_SET_id_get();
        }
        if (z_Set_Id.set_id_choice != C_SETID_UNKNOWN)
        {
          z_set_session_id_immediate_use.w_Session_Id = 0xFFFF; /* since only up to 16 existing sessions supported, use 0xFFFF for this purpose */
          memscpy(&z_set_session_id_immediate_use.z_Set_Id, sizeof(z_set_session_id_immediate_use.z_Set_Id),
                  &z_Set_Id, sizeof(z_Set_Id));

          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       &z_set_session_id_immediate_use,
                                                       supl_StatusCode_invalidSessionId,
                                                       p_hash,
                                                       q_supl_version);
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SET_ID unavailable: %d, ignore SUPL_INT to be rejected",
                z_Set_Id.set_id_choice);

          return;
        }

      }



      else if (e_slp_mess_header_check_result & C_SLP_MESS_VERSION_UNSUPPORTED)
      {
        (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                     NULL,
                                                     supl_StatusCode_versionNotSupported,
                                                     p_hash,
                                                     q_supl_version);
      }
      else if (e_slp_mess_header_check_result & C_SLP_MESS_SUPLINIT_SET_SESS_ID_PROBLEM)
      {

        /* use the SLP-sent illegal SetSessionId in SUPL_END,
           so that SLP can match the session. */
        if (p_pdu_decoded->sessionID.m.setSessionIDPresent)
        {
          supl_set_session_id_organizer(&p_pdu_decoded->sessionID, &z_provided_set_session_id);
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       &z_provided_set_session_id,
                                                       supl_StatusCode_invalidSessionId,
                                                       p_hash,
                                                       q_supl_version);
        }
        else
        {
          (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                       NULL,
                                                       supl_StatusCode_invalidSessionId,
                                                       p_hash,
                                                       q_supl_version);
        }

      }
      else
      {
        (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                     NULL,
                                                     supl_StatusCode_unspecified,
                                                     p_hash,
                                                     q_supl_version);
      }

      if (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT)
      {
        tm_umts_up_supl_callflow_disrupt_handler();
      }

    } /* if something wrong in SUPL_INIT header */

    tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);

  } /* SUPL_INIT decoded ok */

  else /* decode status showing error */
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: ASN1 decoding failed", 0, 0, 0);
    if (asn1_decode_status != C_ASN1_DECODE_BUFFER_NOT_ALLOCATED)
    {
      tm_osys_asn1_free_pdu(p_pdu_decoded, PROTO_SUPL);
    }
  }

}

/********************************************************************
 *
 * tm_umts_up_supl_end_pdsm_ind
 *
 * Function description:
 *  This function sends SUPL session end indiation through PDAPI
 *
 * Parameters:
 *  pz_slp_id : SUPL session id
 *  p_req_data : Notify/verify request information
 *
 * Return: NONE.
 *
 *
 *********************************************************************
 */
static void tm_umts_up_supl_end_pdsm_ind(supl_slp_session_id *pz_slp_id,
                                         pdsm_lcs_supl_notify_verify_req_s_type *p_req_data)
{
  pdsm_ext_status_info_s_type ext_status_info_type = { 0 };
  
  if ((pz_slp_id != NULL) && (p_req_data != NULL))
  {
    (void)tm_umts_up_supl_supl_end_prepare_or_tx(pz_slp_id,
                                                 NULL,
                                                 supl_StatusCode_consentDeniedByUser,
                                                 p_req_data->supl_hash,
                                                 q_supl_ver_maj_after_supl_init_glob);

    /* TBD: may need to pass ver. number to pdpai; for now clear it after use */
    q_supl_ver_maj_after_supl_init_glob = 0;

    memset((void *)&ext_status_info_type, 0, sizeof(ext_status_info_type));
    ext_status_info_type.ext_status_type = PDSM_EXT_STATUS_SUPL_SESSION_END;
    ext_status_info_type.ext_status_info.ext_supl_session_end_ind_type.supl_end_result = PDSM_PD_SUPL_SESSION_END_RESULT_FAIL;

    if (z_supl_session_status.q_pdapi_client_id != 0)
    {
      ext_status_info_type.client_id = z_supl_session_status.q_pdapi_client_id;
    }
    else
    {
      ext_status_info_type.client_id = pdsm_ni_client_id_get();
    }

    /*Send SUPL Session End indication to PDAPI*/
    tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_SUPL_SESSION_END, &ext_status_info_type);
  }
}

/********************************************************************
 *
 * tm_umts_up_supl_notif_verif_resp_proc
 *
 * Function description:
 *  API function for LSM core to ask LCS Agent to process the notification
 *  verification response.
 *
 * Parameters:
 *   pdapi_ver_resp_lcs_ptr: pointer to the lcs structure of the pdapi
 *     command notif_verification_response
 *
 * Return: NONE.
 *
 *
 *********************************************************************
 */
void tm_umts_up_supl_notif_verif_resp_proc(pdsm_lcs_notification_resp_s_type *p_user_resp)
{
  pdsm_lcs_supl_notify_verify_req_s_type  *p_req_data = &p_user_resp->lcs_info.supl_req;
  supl_slp_session_id                     z_slp_id;
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;

  memset(&z_slp_id, 0, sizeof(supl_slp_session_id));
  supl_convert_slp_id_from_notif_verif_resp(p_req_data, &z_slp_id);

  /* check if there modem still needs to process the response or not. If there was a CP session
     that preempted this before teh user responded, we need to reject this NI request */
  if ((tm_core_is_ni_resp_pending() == FALSE) &&
      (p_user_resp->resp == PDSM_LCS_NOTIFY_VERIFY_ACCEPT))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,
        "Notif/Verif accepted by user, but was preempted by another CP NILR session. Reject this session");
    tm_umts_up_supl_end_pdsm_ind(&z_slp_id, p_req_data);
    return;
  }

  if (p_user_resp->resp == PDSM_LCS_NOTIFY_VERIFY_ACCEPT) /* User accepts */
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Notif/Verif accepted by user", 0, 0, 0);
    supl_action_upon_ni_notif_verif_accepted(p_req_data, &z_slp_id);
  }
  else if (p_user_resp->resp ==  PDSM_LCS_NOTIFY_VERIFY_DENY) /* User denies */
  {
    if (p_req_data->notification_priv_type == PDSM_LCS_PRIVACY_OVERRIDE)
    {
#ifndef FEATURE_GNSS_NON_STANDARD_SUPPORT
      if (tm_core_is_ni_resp_pending() == TRUE)
      {
        supl_action_upon_ni_notif_verif_accepted(p_req_data, &z_slp_id);
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "NotifVerif rejection by user overiden due to priv_override", 0, 0, 0);
      }
      else
      {
        /* if the request was already preempted by a CP session, drop the SUPL INIT */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Notif/Verif for PO denied by user, but was preempted by another CP NILR session. Drop SUPL session",
              0, 0, 0);
      }
#else
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "NotifVerif rejection by user not overiden even for priv_override", 0, 0, 0);
#endif

    }

    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Notif/Verif rejected by user", 0, 0, 0);
      (void)tm_umts_up_supl_supl_end_prepare_or_tx(&z_slp_id,
                                                   NULL,
                                                   supl_StatusCode_consentDeniedByUser,
                                                   p_req_data->supl_hash,
                                                   q_supl_ver_maj_after_supl_init_glob);

      /* TBD: may need to pass ver. number to pdpai; for now clear it after use */
      q_supl_ver_maj_after_supl_init_glob = 0;

    }
  }

  else if (p_user_resp->resp == PDSM_LCS_NOTIFY_VERIFY_NORESP) /* no user response */
  {
    if (p_req_data->notification_priv_type == PDSM_LCS_USER_NOTIFY_VERIFY_NOT_ALLOW_NO_RESP)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Notif/Verif no resp, disallow per SUPL_INIT", 0, 0, 0);
      (void)tm_umts_up_supl_supl_end_prepare_or_tx(&z_slp_id,
                                                   NULL,
                                                   supl_StatusCode_consentDeniedByUser,
                                                   p_req_data->supl_hash,
                                                   q_supl_ver_maj_after_supl_init_glob);

      /* TBD: may need to pass ver. number to pdpai; for now clear it after use */
      q_supl_ver_maj_after_supl_init_glob = 0;
    }
    else
    {
      /* all other types should result in accepting upon no response */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Notif/Verif no resp, allow per SUPL_INIT", 0, 0, 0);
      supl_action_upon_ni_notif_verif_accepted(p_req_data, &z_slp_id);
    }
  }

  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: Notif/Verif response invalid, ignored", 0, 0, 0);
  }

}


/*
 ******************************************************************************
 *
 * tm_umts_up_supl_E911_handler
 *
 * Function description:
 *  This function decides the behavior of the SUPL submodule when an E911 call
 *  is originated.
 *
 *
 * Parameters:
 *   None.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

void tm_umts_up_supl_E911_handler(void)
{
  tm_cm_phone_state_info_s_type phone_state_info;

  memset(&phone_state_info, 0x00, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 and Apptrack concurrency in progress. Not aborting ongoing session on E911 orig");
    return;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 handler called", 0, 0, 0);
  
  if (phone_state_info.srv_system == CGPS_SRV_SYS_GSM ||
      phone_state_info.srv_system == CGPS_SRV_SYS_WCDMA ||
      phone_state_info.srv_system == CGPS_SRV_SYS_LTE ||
      phone_state_info.emerg_conn_srv_system == CGPS_SRV_SYS_WLAN ||
      tm_ds_get_emergency_srv_sys() == CGPS_SRV_SYS_WLAN
      /*SUPL will be active protocol in case of 1x-LTE interworking,
        so handle case of 1x also*/
      || ((TM_CORE_CONFIG_DD(gnss_1x_up_supl_enable) != 0) &&
          (phone_state_info.srv_system == CGPS_SRV_SYS_CDMA ||
           phone_state_info.srv_system == CGPS_SRV_SYS_HDR))
      )
  {
    if (z_pdcomm_tcp_info.curr_state == PDCOMM_STATE_CONNECTED)
    {
      (void)tm_umts_up_supl_supl_end_tx(&z_supl_session_status.z_session_id.z_Slp_Sess_Id,
                                        NULL,
                                        supl_StatusCode_unspecified,
                                        NULL,
                                        z_supl_session_status.q_supl_version);
    }
    if (z_supl_session_status.e_cf_state != C_SUPL_CF_STATE_NULL)
    {
      tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_E911);
    }
  }
  /* call session init to force re-reading IMSI if needed */
  tm_umts_up_supl_session_init(FALSE);
  
  return;
}

/********************************************************************
*
* tm_umts_up_supl_stop_nDDS_session
*
* Function description:
*   Reset NV configuration of SUPL and PDCOMMs and clear any nDDS related parameters.
*
* Parameters:
*   None
*
* Return:
*  None
*
*********************************************************************
*/
void tm_umts_up_supl_stop_nDDS_session(void)
{
  pd_comms_sys_info_change_params_type      z_sys_info_change_params;
  supl_status_s_type                        *p_supl_session_status = &z_supl_session_status;
  sys_modem_as_id_e_type                    data_sub = tm_cm_dsds_get_dd_subs();
  

  if(FALSE == tm_core_is_e911_ni_supl_on_dv_in_progress())
  {
    return;
  }
  
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Stopping supl session on nDDS. Inform PDCOMM of dd sub=%d as_id=%d",
        data_sub, p_supl_session_status->session_sub_id);

  /* Reset PDCOMMS conifg with original DD sub before SUPL session */
  z_sys_info_change_params.dd_sub = data_sub;
  z_sys_info_change_params.as_id = data_sub;
  z_sys_info_change_params.srv_system = tm_cm_get_sys_mode_for_as_id(data_sub);
  pd_comms_app_srv_sys_change_notification(z_sys_info_change_params);

  /* Reset UMTS config with original DD sub before SUPL session */
  tm_umts_common_utils_update_config_for_sub(data_sub);

  /* Manually set SETID to UNKNOWN. So that the correct IMSI is used for SUPL sessions*/
  z_Set_Id.set_id_choice = C_SETID_UNKNOWN;
      
   /*Retrieve imsi for supl on original DD*/
  (void)tm_umts_up_supl_comm_retrieve_imsi();
   
  tm_core_set_e911_ni_supl_on_dv(FALSE);

}



/*===========================================================================

FUNCTION tm_umts_up_supl_inform_core_stop_session

DESCRIPTION
  This function informs the TM core that the current session should
  terminate.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/

void  tm_umts_up_supl_inform_core_stop_session(tm_sess_stop_reason_e_type stop_reason)
{
  supl_status_s_type          *p_supl_session_status = &z_supl_session_status;
  tm_sess_req_param_u_type    req_param;
  tm_sess_req_param_u_type    *p_req_param           = &req_param;
  pdsm_ext_status_info_s_type *pz_ext_status_info_type;
  tm_location_notification_s_type location_notification = {0};

  /*Tell the IS801 if SUPL Session is terminating abnormally and if IS801 session is running*/
  if ((stop_reason != TM_STOP_REASON_COMPLETED) &&
      (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
      )
  {
    (void)supl_stop_is801_session(stop_reason);
  }

  if ((stop_reason != TM_STOP_REASON_COMPLETED) && (FALSE == p_supl_session_status->v_send_no_more_location_notifications))
  {
    if (C_SUPL_SESSION_NI == p_supl_session_status->u_callflow_kind)
	{	  
	  /* Construct and send location notification */
      location_notification.q_prtl_type = TM_PRTL_TYPE_UMTS_UP;
      location_notification.e_pdapi_client_id = p_supl_session_status->q_pdapi_client_id;
      location_notification.e_pdapi_client_type = PDSM_CLIENT_TYPE_NI;
      location_notification.e_oper_mode = lpp_up_map_agps_mode_to_tm_oper_mode(p_supl_session_status->u_agps_mode);
      location_notification.e_sub_id = p_supl_session_status->session_sub_id;
      location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_POSITION;
      location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
      location_notification.v_location_privacy_override_set = p_supl_session_status->v_privacy_override_set;
      location_notification.v_emergency_bit_set = p_supl_session_status->z_emergency_data.b_emergency_ni;

      tm_construct_location_notification_ind(location_notification);
    }    
  }
      
  /*memset req_param*/
  memset((void *)p_req_param, 0, sizeof(tm_sess_req_param_u_type));

  /* Send request to stop OTDOA engine before the TM session is stopped */
#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
  if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
  {
    tm_lpp_proc_otdoa_stop_req(p_supl_session_status->tm_active_session_handle,
                               TM_STOP_REASON_COMPLETED,
                               TM_PRTL_TYPE_UMTS_UP,
                               tm_cm_dsds_get_dd_subs());
  }
#endif /* FEATURE_CGPS_LTE_CELLDB && ! FEATURE_GNSS_LOW_MEMORY */

  if (TM_SESS_HANDLE_NONE != p_supl_session_status->tm_active_session_handle)
  {
    p_req_param->stop_param.stop_type = TM_STOP_TYPE_SESSION;
    p_req_param->stop_param.stop_reason = stop_reason;
    if (p_supl_session_status->u_agps_mode == C_SUPL_OTDOA_MODE ||
        p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
    {
      p_req_param->stop_param.op_to_stop = TM_OP_REQ_OTDOA;
    }
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: Stop TmCore session", 0, 0, 0);

    (void)tm_sess_req(TM_PRTL_TYPE_UMTS_UP,
                      p_supl_session_status->tm_active_session_handle,
                      TM_SESS_REQ_STOP,
                      p_req_param
                      );
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Not requesting Stop TmCore session since no SUPL session is in progress");
  }
  
  pz_ext_status_info_type = (pdsm_ext_status_info_s_type *)os_MemAlloc(sizeof(pdsm_ext_status_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_ext_status_info_type)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot allocate memory for pdsm_ext_status_info_s_type in tm_umts_up_supl_inform_core_stop_session");
  }
  else
  {
    memset(pz_ext_status_info_type, 0, sizeof(*pz_ext_status_info_type));

    pz_ext_status_info_type->ext_status_type = PDSM_EXT_STATUS_SUPL_SESSION_END;

    if (stop_reason == TM_STOP_REASON_COMPLETED)
    {
      pz_ext_status_info_type->ext_status_info.ext_supl_session_end_ind_type.supl_end_result = PDSM_PD_SUPL_SESSION_END_RESULT_SUCCESS;
    }
    else
    {
      pz_ext_status_info_type->ext_status_info.ext_supl_session_end_ind_type.supl_end_result = PDSM_PD_SUPL_SESSION_END_RESULT_FAIL;
    }


    if (p_supl_session_status->q_pdapi_client_id != 0)
    {
      pz_ext_status_info_type->client_id = p_supl_session_status->q_pdapi_client_id;
    }
    else
    {
      pz_ext_status_info_type->client_id = pdsm_ni_client_id_get();
    }

    /*Send SUPL Session End indication to PDAPI*/
    tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_SUPL_SESSION_END, pz_ext_status_info_type);
    (void)os_MemFree((void **)&pz_ext_status_info_type);
  }
  /* also clear protocol submodule session related variables */
  tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NONE);
  p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UNDEF;
  p_supl_session_status->u_pdapi_fix_reported =  FALSE;
  p_supl_session_status->u_network_session_on = FALSE;
  p_supl_session_status->v_send_no_more_location_notifications = FALSE;
  
  /* clear all SUPL related timers / call flow states, if not done yet, except UT0_SUPLEND or
     UT3_EXPECT_SUPLREPORT */
  if (p_supl_session_status->e_cf_state != C_SUPL_CF_STATE_NULL)
  {
    if (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND) &&
        !(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT)
        )
    {
      tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);
    }
    else if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND)
    {
      /* if still in UT0_SUPLEND, clear all but C_SUPL_CF_STATE_UT0_SUPLEND */
      p_supl_session_status->e_cf_state = C_SUPL_CF_STATE_UT0_SUPLEND;
    }
    else if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT)
    {
      /* if in C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT, clear all but C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT */
      p_supl_session_status->e_cf_state = C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT;
    }

  }
  /*Reset nDDS session related parameters*/
  tm_umts_up_supl_stop_nDDS_session();
}

/********************************************************************
*
* tm_umts_up_supl_callflow_kind_set
*
* Function description:
*   Sets the nature of the call flow for the current session.
*
* Parameters:
*   enum of None, NetworkInitiated, SetInitiated, or KeepWarm.
*
* Return:
*  None
*
*********************************************************************
*/

void tm_umts_up_supl_callflow_kind_set(supl_callflow_kind_enum_type u_callflow_kind)
{
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;

  p_supl_session_status->u_callflow_kind = u_callflow_kind;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Call flow type set to: %u",
        u_callflow_kind);

  if (u_callflow_kind == C_SUPL_SESSION_NI)
  {
    p_supl_session_status->u_network_session_on = TRUE;
  }

}

/********************************************************************
*
* tm_umts_up_supl_get_pdcomm_tcp_handle
*
* Function description:
*   Upon getting "camped on WCDMA", if no pdcomm TCP handle has been
*   acquired, get it now.
*
* Parameters:
*
* Return:
*  None
*
*********************************************************************
*/

boolean tm_umts_up_supl_get_pdcomm_tcp_handle(void)
{
  pd_comms_get_handle_params_type     z_pdcomm_get_handle_params;
  boolean ret_val = TRUE;

  /**** init TCP *****************************************/
  z_pdcomm_tcp_info.curr_state = PDCOMM_STATE_DISCONNECTED;
  z_pdcomm_tcp_info.recv_state = TM_SUPL_COMM_RECV_HEADER;
  z_pdcomm_tcp_info.num_bytes_to_read = 0;
  memset((void *)&z_pdcomm_tcp_info.tx_buffer, 0, sizeof(tm_supl_comm_output_buffer));


  z_pdcomm_get_handle_params.q_base_msg_id = TM_CORE_PDCOMM_MSG_ID_BASE;
  z_pdcomm_get_handle_params.e_link_type = LINK_TCP;
  if (pd_comms_app_get_handle(&z_pdcomm_get_handle_params, &z_pdcomm_handle_tcp) != PDCOMM_RESULT_OK)
  {
    /* shouldn't happen, fatal */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "fails to get pdcomm handle for TCP at WCDMA camp.", 0, 0, 0);
    ret_val = FALSE;
  }

  return ret_val;
}

/********************************************************************
*
* supl_cell_info_sanity_chk_ok
*
* Function description:
*   Sanity checking cell db update from MGP: WCDMA or LTE.
*
* Parameters:
*
* Return:
*  Boolean: TRUE - sanity check passed; FALSE - sanity check failed.
*
*********************************************************************
*/
boolean supl_cell_info_sanity_chk_ok(const cgps_CellInfoCachedStruct *cell_db_data_ptr)
{
  boolean u_ret = TRUE;
  uint32 q_mcc, q_mnc;
  static char s_Buf[128];

  if (cell_db_data_ptr->u_CachedCellIDValid == TRUE)
  {

#ifdef FEATURE_CGPS_UMTS_CELLDB
    if (cell_db_data_ptr->e_ActiveRAT == CELLDB_ACTIVERAT_WCDMA)
    {
      (void)tm_umts_supl_wcdma_plmn_id_organizer
         ((rrc_plmn_identity_type *)&cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.plmn_id,
          &q_mcc,
          &q_mnc
          );

      if ((q_mcc > C_MCC_MNC_UPLIMIT) || (q_mnc > C_MCC_MNC_UPLIMIT) ||
          ((cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.plmn_id.num_mnc_digits != 2) &&
           (cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.plmn_id.num_mnc_digits != 3)
           ) ||
          (cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.cell_id > C_WCDMA_CELL_ID_UPLIMIT) ||
          (cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.psc > C_WCDMA_CELL_PSC_UPLIMIT) ||
          (cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.freq > C_WCDMA_CELL_FREQ_UPLIMIT)
          )
      {

        MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_MED,
              "CM updating WCDMA CellDB with bogus data: MCC=%lu, MNC=%lu, mnc_digits=%u, CI=%lu, FREQ=%u, PSC=%u\n",
              q_mcc,
              q_mnc,
              cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.plmn_id.num_mnc_digits,
              cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.cell_id,
              cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.freq,
              cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.psc);

        u_ret = FALSE;
      }
    }
#endif

#ifdef FEATURE_CGPS_LTE_CELLDB
    if (cell_db_data_ptr->e_ActiveRAT == CELLDB_ACTIVERAT_LTE)
    {

      (void)tm_umts_supl_lte_plmn_id_organizer
         ((lte_rrc_plmn_s *)&cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.selected_plmn,
          &q_mcc,
          &q_mnc
          );


      if ((q_mcc > C_MCC_MNC_UPLIMIT) || (q_mnc > C_MCC_MNC_UPLIMIT) ||
          ((cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.selected_plmn.num_mnc_digits != 2) &&
           (cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.selected_plmn.num_mnc_digits != 3)
           ) ||
          (cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.cell_identity > C_LTE_CELL_GLOBAL_CELLID_UPLIMIT) ||
          (cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.cell_id > C_LTE_CELL_PHY_CELLID_UPLIMIT)
          )
      {
        uint32 q_num_mnc_digits;

        /* LTE API has either uint32 or uint8 depending on version; for compatibility with both, cast to uint32 here */
        q_num_mnc_digits = (uint32)cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.selected_plmn.num_mnc_digits;

        MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_MED,
              "CM updating LTE CellDB with bogus data: MCC=%lu, MNC=%lu, mnc_digits=%lu, GlbCellID=%lu, PhyCellID=%u, tac=%u\n",
              q_mcc,
              q_mnc,
              q_num_mnc_digits,
              cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.cell_identity,
              cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.cell_id,
              cell_db_data_ptr->z_CellInfoGlobalCached.z_CellInfoLTEGlobal.tac);

        u_ret = FALSE;
      }
    }
#endif

  }

  return u_ret;

}

#ifdef FEATURE_CGPS_WCDMA
/*===========================================================================

FUNCTION
  tm_umts_up_supl_send_wcdma_cell_meas_req

DESCRIPTION
  This function requests cell measurement data from WCDMA L1.

PARAMETERS
   None.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None.

===========================================================================*/

void tm_umts_up_supl_send_wcdma_cell_meas_req(void)
{
  /* Send WCDMA cell measurement request */
#ifdef FEATURE_CGPS_DUAL_WCDMA
  tm_l1_iface_send_wcdma_cell_meas_req(tm_core_get_current_dd_subs_id());
#else
  tm_l1_iface_send_wcdma_cell_meas_req();
#endif
}
#endif /* FEATURE_CGPS_WCDMA */


#ifdef FEATURE_CGPS_LTE_CELLDB
/*===========================================================================

FUNCTION
  tm_umts_up_supl_lte_cell_meas_update

DESCRIPTION
  Update LTE MRL

PARAMETERS

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None.

===========================================================================*/

void tm_umts_up_supl_lte_cell_meas_update(lte_ml1_gps_mrl_cell_meas_update_t *cell_meas_update_ptr)
{

  if (NULL == cell_meas_update_ptr)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LTE MRL report contains NULL ptr, MRL not updated");
    return;
  }

  tm_umts_up_supl_lte_mrl_update(cell_meas_update_ptr, &z_lte_mrl);
}
#endif  /* SUPL2 + LTE_CELL_DB */



/********************************************************************
 *
 * tm_umts_up_supl_assist_req_holdoff_timers_check_start
 *
 * Function description:
 *  Check and Start assistance data request holdoff timers.
 *
 * Parameters:
 *  p_wishlist_mask: pionter to the assistance data wish list.
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */

void  tm_umts_up_supl_assist_req_holdoff_timers_check_start(uint32 *p_wishlist_mask)
{
  int32 l_long_word = 0;

  /* Sanity */
  if (NULL == p_wishlist_mask)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "NULL GPS AD wishlist");
    return;
  }

  /* 
  * Check the GPS hold-off timers to see if any of them needs to hold off. 
  * The hold-off timer is applicable only in non-NI session.
  */
  if (C_SUPL_SESSION_NI == z_supl_session_status.u_callflow_kind)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "AD request hold-off timer is inactive for NI");
    return;
  }

  /* NOTE : GPS / GANSS uses the same bit mask for wishlist mask */
  if (*p_wishlist_mask & C_GPS_ASSIS_ALM)
  {
    l_long_word = os_TimerCheck(supl_alm_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_ALM;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GPS alm holdoff remaining = %ld msec", l_long_word, 0, 0);
    }
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_NAVMODL)
  {
    l_long_word = os_TimerCheck(supl_eph_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_NAVMODL;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GPS eph holdoff remaining = %ld msec", l_long_word, 0, 0);
    }
  }
  if (*p_wishlist_mask & C_GPS_ASSIS_IONO)
  {
    l_long_word = os_TimerCheck(supl_iono_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_IONO;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GPS IONO holdoff remaining = %ld msec", l_long_word, 0, 0);
    }
  }
  if (*p_wishlist_mask & C_GPS_ASSIS_UTC)
  {
    l_long_word = os_TimerCheck(supl_utc_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_UTC;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GPS UTC holdoff remaining = %ld msec", l_long_word, 0, 0);
    }
  }
  if (*p_wishlist_mask & C_GPS_ASSIS_REFLOC)
  {
    l_long_word = os_TimerCheck(supl_refloc_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_REFLOC;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GPS RefL holdoff remaining = %ld msec", l_long_word, 0, 0);
    }
  }

  /*** if any of eph. or alm. is requested, always request refT and refL */
  if ((*p_wishlist_mask & C_GPS_ASSIS_NAVMODL) ||
      (*p_wishlist_mask & C_GPS_ASSIS_ALM)
      )
  {
    *p_wishlist_mask |=  C_GPS_ASSIS_REFTIME |
          C_GPS_ASSIS_REFLOC |
          C_GPS_ASSIS_RTI;
  }

  /******* activate assistance data request holdoff timers *********/

  if (*p_wishlist_mask & C_GPS_ASSIS_ALM)
  {
    (void)os_TimerStart(supl_alm_req_holdoff_timer, C_DEFER_ALM_REQ_MSEC, NULL);
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_NAVMODL)
  {
    (void)os_TimerStart(supl_eph_req_holdoff_timer, C_DEFER_EPH_REQ_MSEC, NULL);
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_IONO)
  {
    (void)os_TimerStart(supl_iono_req_holdoff_timer, C_DEFER_IONO_REQ_MSEC, NULL);
  }
  if (*p_wishlist_mask & C_GPS_ASSIS_UTC)
  {
    (void)os_TimerStart(supl_utc_req_holdoff_timer, C_DEFER_UTC_REQ_MSEC, NULL);
  }
  if (*p_wishlist_mask & C_GPS_ASSIS_REFLOC)
  {
    (void)os_TimerStart(supl_refloc_req_holdoff_timer, C_DEFER_REFLOC_REQ_MSEC, NULL);
  }

}



/********************************************************************
 *
 * tm_umts_up_supl_glo_assist_req_holdoff_timers_check_start
 *
 * Function description:
 *  Check and Start GLONASS assistance data request holdoff timers.
 *
 * Parameters:
 *  p_wishlist_mask: pionter to the assistance data wish list.
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */

void  tm_umts_up_supl_glo_assist_req_holdoff_timers_check_start(uint32 *p_wishlist_mask)
{
  int32 l_long_word = 0;

  /* Sanity */
  if (NULL == p_wishlist_mask)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "NULL GLO AD wishlist");
    return;
  }

  /* 
  * Check the GLONASS hold-off timers to see if any of them needs to hold off. 
  * The hold-off timer is applicable only in non-NI session.
  */
  if (C_SUPL_SESSION_NI == z_supl_session_status.u_callflow_kind)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GLO AD request hold-off timer is inactive for NI");
    return;
  }

  if (tm_umts_common_glonass_enabled())
  {
    /*** check the hold-off timers to see if any of them needs to hold off ***/
    /* NOTE : GPS / GANSS uses the same bit mask for wishlist mask */
    if (*p_wishlist_mask & C_GPS_ASSIS_ALM)
    {
      l_long_word = os_TimerCheck(supl_glo_alm_req_holdoff_timer);
      if (l_long_word > 0)
      {
        *p_wishlist_mask &= ~C_GPS_ASSIS_ALM;
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GLO alm holdoff remaining = %ld msec", l_long_word, 0, 0);
      }
    }

    if (*p_wishlist_mask & C_GPS_ASSIS_NAVMODL)
    {
      l_long_word = os_TimerCheck(supl_glo_eph_req_holdoff_timer);
      if (l_long_word > 0)
      {
        *p_wishlist_mask &= ~C_GPS_ASSIS_NAVMODL;
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GLO eph holdoff remaining = %ld msec", l_long_word, 0, 0);
      }
    }

    /******* activate assistance data request holdoff timers *********/

    if (*p_wishlist_mask & C_GPS_ASSIS_ALM)
    {
      (void)os_TimerStart(supl_glo_alm_req_holdoff_timer, C_DEFER_ALM_REQ_MSEC, NULL);
    }

    if (*p_wishlist_mask & C_GPS_ASSIS_NAVMODL)
    {
      (void)os_TimerStart(supl_glo_eph_req_holdoff_timer, C_DEFER_EPH_REQ_MSEC, NULL);
    }
  }
}


/********************************************************************
 *
 * tm_umts_up_supl_bds_assist_req_holdoff_timers_check_start
 *
 * Function description:
 *  Check and Start BDS assistance data request holdoff timers.
 *
 * Parameters:
 *  p_wishlist_mask: pionter to the assistance data wish list.
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */

void  tm_umts_up_supl_bds_assist_req_holdoff_timers_check_start(uint32 *p_wishlist_mask)
{
  int32 l_long_word = 0;

  /* Sanity */
  if (NULL == p_wishlist_mask)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "NULL BDS AD wishlist");
    return;
  }

  /* 
  * Check the BDS hold-off timers to see if any of them needs to hold off. 
  * The hold-off timer is applicable only in non-NI session.
  */
  if (C_SUPL_SESSION_NI == z_supl_session_status.u_callflow_kind)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS AD request hold-off timer is inactive for NI");
    return;
  }

  if (!tm_umts_up_supl_rrlp_assisted_bds_enabled())
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS AD request zero WL as BDS disabled");
    *p_wishlist_mask = 0;
    return;
  }

  /*** check the hold-off timers to see if any of them needs to hold off ***/
  /* NOTE : GPS / GANSS uses the same bit mask for wishlist mask */
  if (*p_wishlist_mask & C_GPS_ASSIS_ALM)
  {
    l_long_word = os_TimerCheck(supl_bds_alm_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_ALM;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS alm holdoff remaining = %ld msec", l_long_word);
    }
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_NAVMODL)
  {
    l_long_word = os_TimerCheck(supl_bds_eph_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_NAVMODL;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS eph holdoff remaining = %ld msec", l_long_word);
    }
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_REFTIME)
  {
    l_long_word = os_TimerCheck(supl_bds_reftime_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_REFTIME;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS reftime holdoff remaining = %ld msec", l_long_word);
    }
  }
  
  if (*p_wishlist_mask & C_GPS_ASSIS_REFLOC)
  {
    l_long_word = os_TimerCheck(supl_bds_refloc_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_REFLOC;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS refloc holdoff remaining = %ld msec", l_long_word);
    }
  }
  
  if (*p_wishlist_mask & C_GPS_ASSIS_RTI)
  {
    l_long_word = os_TimerCheck(supl_bds_rti_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GPS_ASSIS_RTI;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS rti holdoff remaining = %ld msec", l_long_word);
    }
  }
    
  if (*p_wishlist_mask & C_GNSS_ASSIST_TIME_MODELS)
  {
    l_long_word = os_TimerCheck(supl_bds_timemodel_req_holdoff_timer);
    if (l_long_word > 0)
    {
      *p_wishlist_mask &= ~C_GNSS_ASSIST_TIME_MODELS;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "BDS timemodel holdoff remaining = %ld msec", l_long_word);
    }
  }
    
  
  /******* activate assistance data request holdoff timers *********/

  if (*p_wishlist_mask & C_GPS_ASSIS_ALM)
  {
    (void)os_TimerStart(supl_bds_alm_req_holdoff_timer, C_DEFER_ALM_REQ_MSEC, NULL);
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_NAVMODL)
  {
    (void)os_TimerStart(supl_bds_eph_req_holdoff_timer, C_DEFER_EPH_REQ_MSEC, NULL);
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_REFTIME)
  {
    (void)os_TimerStart(supl_bds_reftime_req_holdoff_timer, C_DEFER_REFLOC_REQ_MSEC, NULL);
  }
  
  if (*p_wishlist_mask & C_GPS_ASSIS_REFLOC)
  {
    (void)os_TimerStart(supl_bds_refloc_req_holdoff_timer, C_DEFER_REFLOC_REQ_MSEC, NULL);
  }

  if (*p_wishlist_mask & C_GPS_ASSIS_RTI)
  {
    (void)os_TimerStart(supl_bds_rti_req_holdoff_timer, C_DEFER_REFLOC_REQ_MSEC, NULL);
  }

  if (*p_wishlist_mask & C_GNSS_ASSIST_TIME_MODELS)
  {
    (void)os_TimerStart(supl_bds_timemodel_req_holdoff_timer, C_DEFER_REFLOC_REQ_MSEC, NULL);
  }
  
}

/********************************************************************
 *
 * tm_umts_up_supl_pa_proc
 *
 * Function description:
 *  Processes PA events from TM Core.
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */

void tm_umts_up_supl_pa_proc(prtl_pa_event_s_type *p_pa_event)
{
  uint8 nv_cgps_umts_pde_server_addr_url[C_MAX_URL_SIZE_BYTE];
  char port_string[6];
  uint16 i = 0;
  uint16 j = 0;

  if (p_pa_event == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return;
  }

  /* NV item limit: 128 bytes */
  /* This setting to 0 in loop is needed to make Klockwork happy */
  for (i = 0; i < C_MAX_URL_SIZE_BYTE; i++)
  {
    nv_cgps_umts_pde_server_addr_url[i] = 0;
  }
  port_string[0] = '\0';

  if ((p_pa_event->pa_event == PDSM_PA_DELETE_PARAMS_EX) ||
      (p_pa_event->pa_event == PDSM_PA_DELETE_PARAMS) || /* PDSM_PA_DELETE_PARAMS is deprecated. */
      (p_pa_event->pa_event == PDSM_PA_DELETE_PARAMS_EX1)) /* PDSM_PA_DELETE_PARAMS is deprecated.
                                                           use PDSM_PA_DELETE_PARAMS_EX instead */
  {
    /* cancel holdoff timer when the corresponding param. is deleted */

    MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Clear SUPL throttling timers. PA delete param 0x%lx, Eph 0x%lx, Alm 0x%lx, GloEph 0x%lx, GloAlm 0x%lx",
          p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags,
          p_pa_event->pa_info_ptr->delete_params.eph_sv_mask,
          p_pa_event->pa_info_ptr->delete_params.alm_sv_mask,
          p_pa_event->pa_info_ptr->delete_params.glo_eph_sv_mask,
          p_pa_event->pa_info_ptr->delete_params.glo_alm_sv_mask
          );

    if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
           PDSM_PA_DELETE_EPH
        )
    {
      if (p_pa_event->pa_info_ptr->delete_params.eph_sv_mask != 0)
      {
        (void)os_TimerStop(supl_eph_req_holdoff_timer);
      }
    }
    if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
           PDSM_PA_DELETE_ALM
        )
    {
      if (p_pa_event->pa_info_ptr->delete_params.alm_sv_mask != 0)
      {
        (void)os_TimerStop(supl_alm_req_holdoff_timer);
      }
    }

    if (tm_umts_common_glonass_enabled())
    {
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_DELETE_ALM_GLO
          )
      {
        if (p_pa_event->pa_info_ptr->delete_params.glo_alm_sv_mask != 0)
        {
          (void)os_TimerStop(supl_glo_alm_req_holdoff_timer);
        }
      }
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_DELETE_EPH_GLO
          )
      {
        if (p_pa_event->pa_info_ptr->delete_params.glo_eph_sv_mask != 0)
        {
          (void)os_TimerStop(supl_glo_eph_req_holdoff_timer);
        }
      }
    }


#ifdef FEATURE_ABDS_RRLP_SUPL
    if (tm_umts_up_supl_rrlp_assisted_bds_enabled())
    {
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_DELETE_ALM_BDS
          )
      {
        if (p_pa_event->pa_info_ptr->delete_params.bds_alm_sv_mask != 0)
        {
          (void)os_TimerStop(supl_bds_alm_req_holdoff_timer);
        }
      }
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_DELETE_EPH_BDS
          )
      {
        if (p_pa_event->pa_info_ptr->delete_params.bds_eph_sv_mask != 0)
        {
          (void)os_TimerStop(supl_bds_eph_req_holdoff_timer);
        }
      }
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_DELETE_TIME_BDS
          )
      {
        (void)os_TimerStop(supl_bds_reftime_req_holdoff_timer);
      }      
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_DELETE_POS
          )
      {
        (void)os_TimerStop(supl_bds_refloc_req_holdoff_timer);
      }      
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_DELETE_HEALTH_BDS
          )
      {
        (void)os_TimerStop(supl_bds_rti_req_holdoff_timer);
      }            
      if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
             PDSM_PA_TIME_OFFSET
          )
      {
        (void)os_TimerStop(supl_bds_timemodel_req_holdoff_timer);
      }                  
    }    
#endif

    if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
           PDSM_PA_DELETE_IONO
        )
    {
      (void)os_TimerStop(supl_iono_req_holdoff_timer);
    }
    if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
           PDSM_PA_DELETE_UTC
        )
    {
      (void)os_TimerStop(supl_utc_req_holdoff_timer);
    }
    if (p_pa_event->pa_info_ptr->delete_params.pdsm_delete_parms_flags &
           PDSM_PA_DELETE_POS
        )
    {
      (void)os_TimerStop(supl_refloc_req_holdoff_timer);
      memset(&z_supl_cached_pos_for_net, 0, sizeof(z_supl_cached_pos_for_net));
    }

  } /* delete param */


  else if (p_pa_event->pa_event == PDSM_PA_POSITION_MODE)
  {
    /* for UMTS CP and UP, it is also updated in the CP event handler
       in case only not both CP and UP are included in the build */
    TM_CORE_CONFIG_DD(positioning_modes_supported) = p_pa_event->pa_info_ptr->position_mode;
  }
  else if (p_pa_event->pa_event == PDSM_PA_UMTS_PDE_NETWORK_ADRS)
  {
    /* for SUPL only */
    if (p_pa_event->pa_info_ptr->server_address.server_addr_type ==
           PDSM_SERVER_TYPE_IPV4_ADDRESS)
    {
      z_umts_nv_item_ram_copy.slp_ip_addr =
         p_pa_event->pa_info_ptr->server_address.server_adrs.server_adrs_v4.server_adrs;
    }
    else if (p_pa_event->pa_info_ptr->server_address.server_addr_type ==
                PDSM_SERVER_TYPE_URL_ADDRESS)
    {
      /* NV item limit: 128 bytes */
      memset(&nv_cgps_umts_pde_server_addr_url[0], 0, C_MAX_URL_SIZE_BYTE);

      if (p_pa_event->pa_info_ptr->server_address.server_adrs.server_adrs_url.url_length >
             C_MAX_URL_SIZE_BYTE)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS_PDE_NETWORK_ADRS too long for NV: %u",
              p_pa_event->pa_info_ptr->server_address.server_adrs.server_adrs_url.url_length,
              0,
              0);
      }

      memscpy(&nv_cgps_umts_pde_server_addr_url[0],
              sizeof(nv_cgps_umts_pde_server_addr_url),
              &p_pa_event->pa_info_ptr->server_address.server_adrs.server_adrs_url.url[0],
              p_pa_event->pa_info_ptr->server_address.server_adrs.server_adrs_url.url_length);

      /* organize UMTS PDE server URL address */
      if (((nv_cgps_umts_pde_server_addr_url[0] == 'h') || (nv_cgps_umts_pde_server_addr_url[0] == 'H')) &&
          ((nv_cgps_umts_pde_server_addr_url[1] == 't') || (nv_cgps_umts_pde_server_addr_url[1] == 'T')) &&
          ((nv_cgps_umts_pde_server_addr_url[2] == 't') || (nv_cgps_umts_pde_server_addr_url[2] == 'T')) &&
          ((nv_cgps_umts_pde_server_addr_url[3] == 'p') || (nv_cgps_umts_pde_server_addr_url[3] == 'P')) &&
          (nv_cgps_umts_pde_server_addr_url[4] == ':') && (nv_cgps_umts_pde_server_addr_url[5] == '/') && (nv_cgps_umts_pde_server_addr_url[6] == '/'))
      {
        j = 7;  /* Start extracting hslp_address after the URL prefix. */
      }
      else
      {
        j = 0;  /* No http[colon][slash][slash] in nv_cgps_umts_pde_server_addr_url. */
      }

      i = j;
      while ((nv_cgps_umts_pde_server_addr_url[i] != ':') && (nv_cgps_umts_pde_server_addr_url[i] != '\0') && (i < (C_MAX_URL_SIZE_BYTE - 1)))
      {
        z_umts_nv_item_ram_copy.slp_address[i - j] = nv_cgps_umts_pde_server_addr_url[i];
        i++;
      }

      /* this IF is only here to satisfy some sanity checks, it is guaranteed
         already by logic above */
      if (i >= j)
      {
        z_umts_nv_item_ram_copy.slp_address[i - j] = '\0';  /* null terminated FQDN */
      }

      i++;
      if (i >= C_MAX_URL_SIZE_BYTE) /* logically, it couldn't be greater than 128 */
      {
        /* no port number in nv_item aagps_defaul_url, use OMA default */
        z_umts_nv_item_ram_copy.slp_port_number = SUPL_PORT_NUM_OFFICIAL_V1_0;
      }
      else
      {
        j = i;

        while ((nv_cgps_umts_pde_server_addr_url[i] != '\0') &&
               (i < (C_MAX_URL_SIZE_BYTE - 1)) &&
               ((i - j) < 6)
               )
        {
          if (((i - j) < 6) && ((i - j) >= 0))
          {
            port_string[i - j] = nv_cgps_umts_pde_server_addr_url[i];
          }
          i++;
        }

        if (((i - j) < 6) && ((i - j) >= 0))
        {
          port_string[i - j] = '\0';  /* null terminated string */
        }

        z_umts_nv_item_ram_copy.slp_port_number = (uint16)atoi(port_string); /* no range check if port_string > "65535" */
        if (z_umts_nv_item_ram_copy.slp_port_number == 0)
        {
          /* no valid port number, use OMA default */
          z_umts_nv_item_ram_copy.slp_port_number = SUPL_PORT_NUM_OFFICIAL_V1_0;
        }
      }

    } /* UMTS PDE server URL */
  }
  else if (p_pa_event->pa_event == PDSM_PA_GPS_LOCK)
  {
    z_umts_nv_item_ram_copy.gps_lock = p_pa_event->pa_info_ptr->gps_lock;
  }
  else if (p_pa_event->pa_event == PDSM_PA_EFS_DATA)
  {
    /* SUPL Root Cert dir updated */
    tm_umts_up_supl_comm_root_certs_updated();
  }
  else if (p_pa_event->pa_event == PDSM_PA_SUPL_VERSION)
  {
    /* SUPL version info.: Bytes from MSB to LSB: rsvd, major, minor, serviceIndicator */
    z_umts_nv_item_ram_copy.gnss_supl_version = p_pa_event->pa_info_ptr->supl_version;
  }
  else if (p_pa_event->pa_event == PDSM_PA_SUPL_SECURITY)
  {
    z_umts_nv_item_ram_copy.aagps_use_transport_security = (uint8)p_pa_event->pa_info_ptr->supl_security;
  }
#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
  else if ((p_pa_event->pa_event == PDSM_PA_LPP_CONFIG_INFO) ||
           (p_pa_event->pa_event == PDSM_PA_LPPE_UP_CONFIG))
  {
    tm_lpp_up_handle_pa_event(p_pa_event);
  }
#endif /* FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY */
  else if (p_pa_event->pa_event == PDSM_PA_EMERGENCY_EXTENSION_WINDOW)
  {
    z_umts_nv_item_ram_copy.q_emergency_extension_window = p_pa_event->pa_info_ptr->q_emergency_extension_window_config;
  }  
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "PDAPI PA event not processed by SUPL: %u",
          p_pa_event->pa_event, 0, 0);
  }


}


/******************************************************************************
 * tm_umts_supl_efs_dir_init
 *
 * Function description:
 *   Ceate the SUPL directory in the EFS
 *
 *   Added EFS directories for chained certificate support.
 *
 * Parameters:   None
 *
 * Return value: TRUE: successful; FALSE: unsucessful
 *
 ******************************************************************************
*/

boolean tm_umts_supl_efs_dir_init(void)
{
  int f_EfsFile;

  f_EfsFile = efs_mkdir("/SUPL", ALLPERMS);
  if(f_EfsFile < 0 && efs_errno != EEXIST)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_umts_supl_efs_dir_init: error in efs_mkdir() /SUPL, f_EfsFile %d errno %d", f_EfsFile, efs_errno);
    return FALSE;
  }

  /* Added EFS directories for chained certificate support. */
  f_EfsFile = efs_mkdir("/client-cert", ALLPERMS);
  if(f_EfsFile < 0 && efs_errno != EEXIST)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_umts_supl_efs_dir_init: error in efs_mkdir() /client-cert, f_EfsFile %d errno %d", f_EfsFile, efs_errno);
    return FALSE;
  }
  
  f_EfsFile = efs_mkdir("/client-key", ALLPERMS);
  if(f_EfsFile < 0 && efs_errno != EEXIST)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_umts_supl_efs_dir_init: error in efs_mkdir() /client-key, f_EfsFile %d errno %d", f_EfsFile, efs_errno);
    return FALSE;
  }
  
  f_EfsFile = efs_mkdir("/cert", ALLPERMS);
  if(f_EfsFile < 0 && efs_errno != EEXIST)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_umts_supl_efs_dir_init: error in efs_mkdir() /cert, f_EfsFile %d errno %d", f_EfsFile, efs_errno);
    return FALSE;
  }

  return TRUE;

}


/******************************************************************************
 * tm_umts_supl_if_need_forced_wcmda_cell_inq
 *
 * Function description:
 *   Check to see if it needs to request WCDMA cell info. directly; Usually
 *   needed for CELL_DCH.
 *
 * Parameters:   None
 *
 * Return value: TRUE: Need direct request of WCDMA cell info.;
 *               FALSE: no need for direct request of WCDMA cell info.
 *
 ******************************************************************************
*/

boolean tm_umts_supl_if_need_forced_wcmda_cell_inq(void)
{
  boolean ret = FALSE;

  static char s_Buf[128];
  uint32 q_mcc, q_mnc;
  tm_cm_phone_state_info_s_type phone_state_info;

  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  if (phone_state_info.srv_system == CGPS_SRV_SYS_WCDMA)
  {

    if (!z_cell_info_cached.u_CachedCellIDValid)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "W-cell inq: no cached cell. %u",
            z_cell_info_cached.u_CachedCellIDValid);
      return TRUE;  /* no cell db while in WCDMA, need directy inq. */
    }


    if (z_cell_info_cached.e_ActiveRAT !=
           CELLDB_ACTIVERAT_WCDMA)
    {

      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "W-cell inq: in W but cell_DB RAT = %u",
            z_cell_info_cached.e_ActiveRAT);

      return TRUE; /* need direct inq. for WCDMA cell info. */
    }


    /*** handle WCDMA RAT matching CELL ID RAT: check for staleness ***/

    (void)tm_umts_supl_wcdma_plmn_id_organizer
       (&z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.plmn_id,
        &q_mcc,
        &q_mnc);

    if (z_cell_info_cached.q_NumCellChangesCachedCellID == 1)
    {

      MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "Valid cell_id avail: MCC=%lu,MNC=%lu,CI=%lu,FREQ=%u,PSC=%u\n",
            q_mcc,
            q_mnc,
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.cell_id,
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.freq,
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.psc);

      /* cached WCDMA cell info. valid, no need to directly request for it */

    }

    else /* stale cell info. */
    {
      MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "Stale cell_id: NChanges=%lu,MCC=%lu,MNC=%lu,CI=%lu,FREQ=%u,PSC=%u\n",
            z_cell_info_cached.q_NumCellChangesCachedCellID,
            q_mcc,
            q_mnc,
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.cell_id,
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.freq,
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoWCDMAGlobal.psc);

      ret = TRUE; /* need direct WCDMA cell info. inq. */
    }

  } /* RAT is WCDMA */

  return ret;
}


/******************************************************************************
 * tm_umts_up_supl_ck_req_tdscmda_cell_info
 *
 * Function description:
 *   Check to see if it is on TDSCDMA and if so request cell information.
 *
 * Parameters:  None.
 *
 * Return: None.
 *
 ******************************************************************************
*/

void tm_umts_up_supl_ck_req_tdscmda_cell_info(void)
{

#ifdef FEATURE_SUPL1_TDS /* defined in custcgps.h, if .builds defines FEATURE_CGPS_USES_TDS */

  tm_cm_phone_state_info_s_type phone_state_info;

  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  if (phone_state_info.srv_system == CGPS_SRV_SYS_TDSCDMA)
  {

    tdsrrcmeas_plmn_cell_info_struct_type     tds_plmn_cell_info;

    memset(&tds_plmn_cell_info, 0, sizeof(tdsrrcmeas_plmn_cell_info_struct_type));

    /* until there is any mechanism to get cell change indication and DCH cell info,
       use this camping event to cache cell info. first */

    if (gnss_wwan_iface_tdsrrcmeas_get_cell_plmn_info(&tds_plmn_cell_info))
    {

      z_cell_info_cached.u_CachedCellIDValid = TRUE;
      z_cell_info_cached.e_ActiveRAT = CELLDB_ACTIVERAT_TDSCDMA;

      z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.cell_id =
         tds_plmn_cell_info.cell_id;


      z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.lac[0] =
         tds_plmn_cell_info.lac[0];

      z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.lac[1] =
         tds_plmn_cell_info.lac[1];

      memscpy(&z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id,
              sizeof(z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id),
              &tds_plmn_cell_info.plmn_id, sizeof(tds_plmn_cell_info.plmn_id)
              );

      MSG_7(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "Camped on TDS - cell info.: MCC[0]=%u [1]=%u [2]=%u, MNC[0]=%u  [1]=%u [2]=%u%, N_MNC=%u",
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id.mcc[0],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id.mcc[1],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id.mcc[2],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id.mnc[0],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id.mnc[1],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id.mnc[2],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.plmn_id.num_mnc_digits
            );
      MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "TDS - cell info.: LAC[0]=%u [1]=%u (LAC=%u), CI=%lu",
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.lac[0],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.lac[1],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.lac[0] * 256 +
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.lac[1],
            z_cell_info_cached.z_CellInfoGlobalCached.z_CellInfoTDSGlobal.cell_id
            );


    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
            "Failed obtaining TDS cell info.: %u", phone_state_info.srv_system);
    }

  } /* camped on TDSCMDA */

#endif /* #if defined feature */

}


/******************************************************************************
 * tm_umts_supl_wcdma_plmn_id_organizer
 *
 * Function description:
 *   Converts MCC and MNC stored in the format of rrcmeas_plmn_cell_info to
 *   uint32.
 *
 * Parameters:
 *  p_cell_id_info: pointer to the structure containing plmn cell
 *                 information;
 *  p_mcc: pointer for the this function to write converted MCC value to.
 *  p_mnc: pointer for the this function to write converted MNC value to.
 *
 * Return:
 *   TRUE/FALSE.
 *
 ******************************************************************************
*/

boolean tm_umts_supl_wcdma_plmn_id_organizer
(rrc_plmn_identity_type *p_plmn_id_info,
 uint32 *p_mcc,
 uint32 *p_mnc
 )
{

  if ((p_mcc == NULL) || (p_mnc == NULL) || (p_plmn_id_info == NULL))
  {
    return FALSE;
  }

  *p_mcc = (p_plmn_id_info->mcc[0] * 100) +
     (p_plmn_id_info->mcc[1] * 10) +
     p_plmn_id_info->mcc[2];


  /* MNC: assume there will be 3 digits, so put the 1st 2 digits in the
     100s and 10s place. */
  *p_mnc = (p_plmn_id_info->mnc[0] * 100) +
     (p_plmn_id_info->mnc[1] * 10);

  /* add in the 3rd digit if there is one */
  if (p_plmn_id_info->num_mnc_digits == 3)
  {
    *p_mnc += p_plmn_id_info->mnc[2];
  }
  else
  {
    /* divide by 10 if there are only 2 digits in the mnc */
    *p_mnc /= 10;
  }

  return TRUE;

}

#ifdef FEATURE_CGPS_LTE_CELLDB

/******************************************************************************
 * tm_umts_supl_lte_plmn_id_organizer
 *
 * Function description:
 *   Converts MCC and MNC stored in the format of lte_rrc_plmn_s to
 *   uint32.
 *
 * Parameters:
 *  p_plmn_id_info: pointer to the structure containing plmn cell
 *                 information;
 *  p_mcc: pointer for the this function to write converted MCC value to.
 *  p_mnc: pointer for the this function to write converted MNC value to.
 *
 * Return:
 *   TRUE/FALSE.
 *
 ******************************************************************************
*/

boolean tm_umts_supl_lte_plmn_id_organizer
(lte_rrc_plmn_s *p_plmn_id_info,
 uint32 *p_mcc,
 uint32 *p_mnc
 )
{

  if ((p_mcc == NULL) || (p_mnc == NULL) || (p_plmn_id_info == NULL))
  {
    return FALSE;
  }

  *p_mcc = ((p_plmn_id_info->mcc[0] & 0x000F) * 100) +
     ((p_plmn_id_info->mcc[1] & 0x000F) * 10) +
     (p_plmn_id_info->mcc[2] & 0x000F);


  /* MNC: assume there will be 3 digits, so put the 1st 2 digits in the
     100s and 10s place. */
  *p_mnc = ((p_plmn_id_info->mnc[0] & 0x000F) * 100) +
     ((p_plmn_id_info->mnc[1] & 0x000F) * 10);

  /* add in the 3rd digit if there is one */
  if (p_plmn_id_info->num_mnc_digits == 3)
  {
    *p_mnc += p_plmn_id_info->mnc[2] & 0x000F;
  }
  else
  {
    /* divide by 10 if there are only 2 digits in the mnc */
    *p_mnc /= 10;
  }

  return TRUE;

}

#endif /* if SUPL2 + LTE_CELL_DB*/


/******************************************************************************
 * tm_umts_up_supl_req_cell_info_from_rrc
 *
 * Function description:
 *   Actively request cell info. from RRC
 *
 * Parameters:   None
 *
 * Return value: TRUE: request to RRC sent successfully;
 *               FALSE: request to RRC failed.
 *
 * Note: reason to featurize this function is the RRC counterpart is
 *       featurized in order to serve different targets some of which
 *       do not have this feature implemented on the GPS side.
 *
 ******************************************************************************
*/

boolean tm_umts_up_supl_req_cell_info_from_rrc(supl_cf_state_enum_type cf_state)
{
  supl_set_cf_state(cf_state, 0);

#ifdef FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH
  /* Request the new cell id information from CM */
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL requested DCH CID (cf_state=%d)", (uint32)cf_state);
  /*Ensure we always call the RRC function to get WCDMA CELL with a SUB ID 
    SUPL uses the TM_CORE's active_sub for its funtionality
    SS   SUPL       : tm_core's sub = tm_cm_dd sub
    DSDS SUPL on DD : tm_core's sub = tm_cm_dd sub
    DSDS SUPL on nDD: tm_core's sub != tm_cm_dd sub
    Always use tm_core's sub since its tracks all 3 scenario's above.
    */
  return tm_cm_request_wcdma_dch_cell_info_by_sub(tm_core_get_current_dd_subs_id());
#else
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't request WCDMA DCH CID.");
  return FALSE;
#endif /* FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH */
}

/*
******************************************************************************
* supl_SuplInit_emergency_bit_process
*
* Function description:
*
* Process incoming SUPL INIT message to see if emergency bit is set
*
* Parameters:  supl_init message
*
* Return value: none
*
******************************************************************************
*/

void supl_SuplInit_emergency_bit_process(const supl_SUPLINIT  *p_suplInit)
{

  if (p_suplInit->m.notificationPresent == 1)
  {
    if (p_suplInit->notification.m.ver2_Notification_extensionPresent == 1)
    {
      /* if emergency call, then ensure the SUPL INIT cotnains the emergency positioning indication */
      if (p_suplInit->notification.ver2_Notification_extension.m.emergencyCallLocationPresent == 1)
      {
        z_supl_emergency_status.b_valid = TRUE;
        z_supl_emergency_status.b_emergency_ni = TRUE;
        if ((p_suplInit->m.ver2_SUPL_INIT_extensionPresent == 1) &&
            (p_suplInit->ver2_SUPL_INIT_extension.m.e_SLPAddressPresent == 1))
        {
          memscpy(&z_supl_emergency_status.z_eslp_address, sizeof(z_supl_emergency_status.z_eslp_address),
                  &p_suplInit->ver2_SUPL_INIT_extension.e_SLPAddress, sizeof(p_suplInit->ver2_SUPL_INIT_extension.e_SLPAddress));
        }
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: Handle NI request with emergency bit set, eslp present %d", p_suplInit->ver2_SUPL_INIT_extension.m.e_SLPAddressPresent, 0, 0);
      }
    }
  }
}
/********************************************************************
*
* tm_umts_up_supl_setup_nDDS_session
*
* Function description:
*   Set new NV configuration for SUPL and PDCOMMs.
*
* Parameters:
*   None
*
* Return:
*  None
*
*********************************************************************
*/
void tm_umts_up_supl_setup_nDDS_session(void)
{
  pd_comms_sys_info_change_params_type z_change_sys_info_params;
    supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  
  sys_modem_as_id_e_type session_sub_id = p_supl_session_status->session_sub_id;

  tm_core_set_e911_ni_supl_on_dv(TRUE);

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_INIT received and handled when DV != DD sub");

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Inform PDCOMM of temporary new dd sub=%d TM_CM dd=%d, TM Core DD=%d",
        session_sub_id,tm_cm_dsds_get_dd_subs(),tm_core_get_current_dd_subs_id());
  
  /* Pass serving system change notification to PDCOMM. 
     To change temporarily DD sub to be same as DV sub. */
  z_change_sys_info_params.dd_sub = session_sub_id; 
  z_change_sys_info_params.as_id = session_sub_id;
  z_change_sys_info_params.srv_system = tm_cm_get_sys_mode_for_as_id(session_sub_id);

  pd_comms_app_srv_sys_change_notification(z_change_sys_info_params);
  
}

/*
******************************************************************************
* supl_SuplInit_proc
*
* Function description:
*
* Process incoming SUPL INIT message
*
* Parameters: p_pdu_decoded: pointer to a structure holding decoded Supl PDU.
*             p_raw_pdu : pointer to raw pdu buffer.
*             w_raw_pdu_len: length of the raw pdu in bytes.
              p_hash: pointer to the first byte of hash - NULL if no HASH
*
* Return value: TRUE/FALSE
*
******************************************************************************
*/

static boolean supl_SuplInit_proc(const supl_ULP_PDU *p_pdu_decoded,
                                  uint8 *p_raw_pdu,
                                  uint16 w_raw_pdu_len,
                                  uint8 *p_hash)

{
  const supl_SUPLINIT  *p_suplInit = p_pdu_decoded->message.u.msSUPLINIT;

  supl_slp_session_id  z_p_provided_slp_session_id;
  supl_slp_session_id *p_provided_slp_session_id = &z_p_provided_slp_session_id;

  tm_cm_phone_state_info_s_type phone_state_info;
  
  uint32  q_supl_status_code = 0xFFFFFFFF;

  uint32  q_supl_version;
  boolean v_privacy_override_set = FALSE;
  boolean v_emergency_bit_set = FALSE;
  tm_location_notification_s_type location_notification = {0};
 
  sys_modem_as_id_e_type e_session_sub_id = z_supl_session_status.session_sub_id;
  
  tm_cm_iface_get_phone_state(&phone_state_info);
  
  if(SM_CONFIG_MIN_SUB_ID > e_session_sub_id || SM_CONFIG_MAX_SUB_ID <= e_session_sub_id)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"session sub id %d out of valid range",e_session_sub_id);
    return FALSE;
  }


  /*SUPL is not Enabled, ignore the message*/
  if (tm_umts_up_is_supl_enabled() == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_INIT ignored: SUPL not Enabled", 0, 0, 0);
    return FALSE;
  }


  /*During Emergency call, process SUPL INIT even if DD and DV subscription are on the different SUB/AS_ID. 
    IF DD sub != DV sub or (emergency call ASID is not same as DD SUB/ASID), inform PDCOMMs about DV sub 
    SUPL INIT.
  */

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "DD SUB [%d] - SUPL SUB: [%d]",tm_cm_dsds_get_dd_subs(), e_session_sub_id);

  if( e_session_sub_id != tm_cm_dsds_get_dd_subs())
    {
    /* emerg_asubs_id is set when e911 orig is handled in tm_cm_handle_emergency_call_orig_event*/
    if(( TM_CM_E911_STATE_NONE == phone_state_info.e911_state ))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL_INIT getting dropped.  DV!=DD and non-emergency session");
      return FALSE;
    }
    else
    {
      if((TM_CORE_CONFIG_AS_ID(e_session_sub_id, q_agps_feature_support) & TM_SUPL_ON_NDDS_ENABLE) != 0)
      {
        tm_umts_up_supl_setup_nDDS_session();

      }
      else
      {
         MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL on nDDS not allowed by NV. value= %d",
              tm_core_info.config_info.subs_cfgs[e_session_sub_id].q_agps_feature_support);
        return FALSE;
      }
    }
  }

  if ( ( TRUE == tm_E911WLANOn() ) && 
       ( PDSM_SUPL_INIT_SOURCE_TYPE_UDP != z_supl_init_source ) )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL INIT SMS not handled for VoWIFI");
    return FALSE;
  }

  if ( ( ( CGPS_SRV_SYS_WLAN == phone_state_info.emerg_conn_srv_system ) ||
         ( CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys() ) ) &&
       ( FALSE == ( TM_CORE_FEATURE_BIT_IS_SET( TM_SUPL_VOWIFI_EMERGENCY_ENABLE ) ) ) &&
       ( TM_CM_E911_STATE_NONE != phone_state_info.e911_state ) 
     )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL_INIT ignored: SUPL 911 over VoWIFI not Enabled");
    return FALSE;
  }


    if( ((TM_CORE_CONFIG_DD(positioning_modes_supported) &
              (PDSM_POS_MODE_UP_MS_ASSISTED    |
               PDSM_POS_MODE_UP_MS_BASED       |
               PDSM_POS_MODE_4G_UP_MS_ASSISTED |
               PDSM_POS_MODE_4G_UP_MS_BASED)) == 0) &&
        ((!tm_umts_common_bds_enabled()) ||
          (TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED)) == FALSE)
       )
    {
      /* SUPL not provisioned by NV, ignore the message */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_INIT ignored: SUPL not provisioned, Supported GPS modes 0x%x bds enbl %d abds modes 0x%x",
            TM_CORE_CONFIG_DD(positioning_modes_supported),
            tm_umts_common_bds_enabled,
            TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED));
      return FALSE;
    }


  if(CGPS_SRV_SYS_NR5G == phone_state_info.srv_system)
  {
    if(FALSE == tm_core_get_nr_lpp_up_enable())
    {
      /* SUPL not provisioned by NV, ignore the message */
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_INIT ignored: SUPL LPP UP not provisioned over NR");
      return FALSE;
    }
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Recvd SUPL_INIT", 0, 0, 0);

  supl_util_server_session_id_organizer(&p_pdu_decoded->sessionID, p_provided_slp_session_id);

  /* incoming message SUPL version number */
  q_supl_version = (uint32)p_pdu_decoded->version.servind << 16;
  q_supl_version |= (uint32)p_pdu_decoded->version.min << 8;
  q_supl_version |= (uint32)p_pdu_decoded->version.maj;


  if (p_hash == NULL)
  {
    /* should not happen: HASH generation failed */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "missing hash", 0, 0, 0);
    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                 NULL,
                                                 supl_StatusCode_unspecified,
                                                 NULL,
                                                 q_supl_version
                                                 );
    return FALSE;
  }
  
  /************* with valid HASH *******************************************/
  if (p_suplInit->posMethod > supl_PosMethod_ver2_sessioninfoquery)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: end NI request due to posMethod out of range", 0, 0, 0);
    q_supl_status_code = supl_unexpectedDataValue;
  }
  else if (p_suplInit->sLPMode != supl_proxy)
  {
    /* Check SLP Mode. If not proxy then respond with an error */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: end NI request due to non-proxy not supported", 0, 0, 0);
    q_supl_status_code = supl_nonProxyModeNotSupported;
  }
  else if (p_suplInit->notification.notificationType > supl_NotificationType_privacyOverride)
  {
    /* this should not happen, as ASN.1 decoder should fail */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: end NI request due to notifType out of range", 0, 0, 0);
    q_supl_status_code = supl_unexpectedDataValue;
  }  
  else if (TM_CM_E911_STATE_NONE == phone_state_info.e911_state)
  {
    if (p_suplInit->notification.notificationType == supl_NotificationType_privacyOverride)
    {
      v_privacy_override_set = TRUE;
    }
    if ((p_suplInit->notification.m.ver2_Notification_extensionPresent == 1) &&
        (p_suplInit->notification.ver2_Notification_extension.m.emergencyCallLocationPresent == 1))
    {
      v_emergency_bit_set = TRUE;
    }
    
    if (((z_umts_nv_item_ram_copy.q_gnss_lock_ctrl_for_oem)& PDSM_PA_SUPL_NI_OEM_CTRL)!= 0)
    {
      /* NvItem gnss_lock_ctrl_for_oem has locked GPS NI for SUPL */
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: Privacy: OEM NV_Item locks non-emergency SUPL NI");            
      q_supl_status_code = supl_positioningNotPermitted;
      
      /* Construct Reject location notification */
      location_notification.q_prtl_type = TM_PRTL_TYPE_UMTS_UP;
      location_notification.e_pdapi_client_id = pdsm_ni_client_id_get();
      location_notification.e_pdapi_client_type = PDSM_CLIENT_TYPE_NI;
      location_notification.e_oper_mode = TM_SESS_OPERATION_MODE_MSB;
      location_notification.e_sub_id = z_supl_session_status.session_sub_id;
      location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_POSITION;
      location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_REJECTED;
      location_notification.v_location_privacy_override_set = v_privacy_override_set;
      location_notification.v_emergency_bit_set = v_emergency_bit_set;
        
      tm_construct_location_notification_ind(location_notification); 
    }    
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: non emergency session", 0, 0, 0);
      supl_SuplInit_emergency_bit_process(p_suplInit);
    }
  }
  else if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
  {
    /* if in emergency mode always accept the SUPL trigger */      
    supl_SuplInit_emergency_bit_process(p_suplInit);
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL:  NI request in emergency call always accept");    
  }
  

  if (p_suplInit->posMethod == supl_ver2_sessioninfoquery)
  {
    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                 NULL,
                                                 supl_StatusCode_ver2_serviceNotSupported,
                                                 p_hash,
                                                 q_supl_version);
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Area events not supported, sending SUPL_END");
    return TRUE;
  }

  if (p_suplInit->posMethod == supl_ver2_historicalDataRetrieval)
  {

    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                 NULL,
                                                 supl_StatusCode_ver2_serviceNotSupported,
                                                 p_hash,
                                                 q_supl_version);
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "HistoricalDataRetrieval not supported, sending SUPL_END", 0, 0, 0);
    return TRUE;
  }



  /*** if no error detected so far, send SUPL_INIT along with hash to pdapi;
       otherwise, send SUPL_END to SLP *************************************/
  if (q_supl_status_code == 0xFFFFFFFF)
  {

    /* TBD: this is better done after pdapi notification comes back, for
       service interaction purpose, but currently pdpai doesn't have this
       IE included  !! */
    /* if SUPL version for the session is not yet set, do it now */
    if (z_supl_session_status.q_supl_version == 0)
    {
      z_supl_session_status.q_supl_version = q_supl_version;
    }

    if (p_pdu_decoded->message.u.msSUPLINIT->ver2_SUPL_INIT_extension.m.triggerTypePresent)
    {

      (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                     NULL,
                                                     supl_StatusCode_ver2_serviceNotSupported,
                                                     p_hash,
                                                     q_supl_version);
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "NI periodic not supported, sending SUPL_END");
      return TRUE;

    }
    else
    {
      z_supl_init_trigger_type.trigger_type_present = FALSE;
    }

    if ((p_pdu_decoded->message.u.msSUPLINIT->ver2_SUPL_INIT_extension.m.historicReportingPresent) ||
        ((p_pdu_decoded->message.u.msSUPLINIT->ver2_SUPL_INIT_extension.m.protectionLevelPresent &&
          p_pdu_decoded->message.u.msSUPLINIT->ver2_SUPL_INIT_extension.protectionLevel.protlevel !=
             supl_ProtLevel_nullProtection)
         )
        )
    {
      (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                   NULL,
                                                   supl_StatusCode_ver2_incompatibleProtectionLevel,
                                                   p_hash,
                                                   q_supl_version);

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Protection level imcompatible, sending SUPL_END", 0, 0, 0);
      return TRUE;
    }


    /* regardless what notification/verification type SUPL_INIT has, send the request
       via pdapi to the OEM, and let OEM decide */
    supl_send_notification_verifcation(p_pdu_decoded, p_hash);
    /* Set the XO Core to be ON in expectation of GPS activity */
    (void)mgp_ForceXoCoreOn();
    return TRUE;
  }
  else
  {
    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_provided_slp_session_id,
                                                 NULL,
                                                 q_supl_status_code,
                                                 p_hash,
                                                 q_supl_version);
    return FALSE;
  }

}


/*
******************************************************************************
* supl_SuplStart_tx
*
* Function description:
*
* Encode and transmit SUPL START
*
* Parameters: NONE.
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
boolean supl_SuplStart_tx(void)
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  boolean               u_ret                 = FALSE;

  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */
  supl_ULP_PDU      pdu;

  supl_SUPLSTART *p_Suplstart;
  supl_SUPLSTART    z_Suplstart;

  supl_cell_info_struct_type  z_cell_info_allocated;

  uint8 *p_length;
  uint8            u_length_msb = 0;
  uint8            u_length_lsb = 0;

  tm_cm_phone_state_info_s_type phone_state_info;

  int encode_status;

  supl_Ver2_SUPL_START_extension z_supl_Ver2_supl_start_extension;

  uint8 u_num_mlid = 0;
  supl_LocationIdData *p_LocationIdData = NULL;
  supl_LocationId     *p_LocationId = NULL;
  supl_cell_info_struct_type  *p_cell_info_allocated_mlid = NULL;


  struct supl_GANSSPositionMethod   z_ganssPositionMethod[2];
  struct supl_GANSSPositionMethods  z_ganssPositionMethodsList;
  struct supl_GANSSPositionMethods  *p_ganssPositionMethodsList =
     &z_ganssPositionMethodsList;


  /* When ready to send SUPL_START, any timer/state waiting for WIFI MLID should be canceled */
  tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP);
  tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_SUPLSTART);
  tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLSTART);

  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  memset(&z_supl_Ver2_supl_start_extension, 0, sizeof(supl_Ver2_SUPL_START_extension));
  memset(&pdu, 0x00, sizeof(supl_ULP_PDU));
  memset(&z_cell_info_allocated, 0, sizeof(supl_cell_info_struct_type));
  memset(&z_Suplstart, 0x00, sizeof(supl_SUPLSTART));
  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));

  memset(&z_supl_Ver2_supl_start_extension, 0, sizeof(supl_Ver2_SUPL_START_extension));
  memset(&z_ganssPositionMethod, 0, sizeof(z_ganssPositionMethod));
  memset(&z_ganssPositionMethodsList, 0, sizeof(z_ganssPositionMethodsList));

  p_Suplstart = &z_Suplstart;
  pdu.message.t = C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLSTART;
  pdu.message.u.msSUPLSTART = p_Suplstart;


  /* overhead build function has to be after we know what message is to be constructed */

  if (!supl_header_build(&pdu, NULL, NULL, NULL, p_supl_session_status->q_supl_version))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplStart attempt aborted: header", 0, 0, 0);
    return FALSE;
  }

  /* Positioning technology */
  p_Suplstart->sETCapabilities.posTechnology.aFLT = FALSE;
  p_Suplstart->sETCapabilities.posTechnology.eOTD = FALSE;
  p_Suplstart->sETCapabilities.posTechnology.oTDOA = FALSE;
  p_Suplstart->sETCapabilities.posTechnology.eCID = FALSE; /* maybe modified below */


  if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
  {
    p_Suplstart->sETCapabilities.posTechnology.agpsSETassisted = TRUE;
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      p_Suplstart->sETCapabilities.posTechnology.oTDOA = TRUE;
    }
  }
  else if (p_supl_session_status->u_agps_mode == C_SUPL_AFLT_MODE)
  {
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
    {
      p_Suplstart->sETCapabilities.posTechnology.aFLT = TRUE;
    }
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      p_Suplstart->sETCapabilities.posTechnology.oTDOA = TRUE;
    }
  }
  else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
  {
    p_Suplstart->sETCapabilities.posTechnology.agpsSETBased = TRUE;

    /*IS801 does not support Autonomous GPS mode*/
    if (p_supl_session_status->u_position_protocol != TM_POS_PRTL_IS801)
    {
#ifndef FEATURE_CGPS_SUPL_VARIANT_SET_CAP_NO_AUTO /* NOT by default defined */
      p_Suplstart->sETCapabilities.posTechnology.autonomousGPS = TRUE;
#endif
    }

  }
  else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
  {
    p_Suplstart->sETCapabilities.posTechnology.agpsSETassisted = FALSE;
    p_Suplstart->sETCapabilities.posTechnology.autonomousGPS = FALSE;
    p_Suplstart->sETCapabilities.posTechnology.agpsSETBased = FALSE;
  }
  else if (p_supl_session_status->u_agps_mode == C_SUPL_OTDOA_MODE)
  {
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      p_Suplstart->sETCapabilities.posTechnology.oTDOA = TRUE;
      // eCID is set later
    }
  }
  else
  {
    /* no other agps mode should generate SUPL_POS_INIT */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplStart attempt aborted", 0, 0, 0);
    u_ret = FALSE;
  }
  /*if SUPL on WLAN, OTDOA is not enabled*/
#ifdef FEATURE_LOC_LTE_OTDOA
  if ( TRUE == tm_E911WLANOn())
  {
     MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: SuplStart OTDOA disabled for WLAN");
     p_Suplstart->sETCapabilities.posTechnology.oTDOA = FALSE;
  }
#endif /*FEATURE_LOC_LTE_OTDOA*/

  p_Suplstart->sETCapabilities.prefMethod = supl_PrefMethod_noPreference;

#ifdef FEATURE_CGPS_SUPL_VARIANT_SET_CAP_PREF_SET_BASED  /* NOT by defaut defined */
  #error code not present
#endif

  /* positioning protocol */
  // Initialize
  p_Suplstart->sETCapabilities.posProtocol.rrc = FALSE;
  p_Suplstart->sETCapabilities.posProtocol.tia801 = FALSE;
  p_Suplstart->sETCapabilities.posProtocol.rrlp = FALSE;

  if (TM_CORE_CONFIG_DD(gnss_1x_up_supl_enable) != 0)
  {
    p_Suplstart->sETCapabilities.posProtocol.tia801 = TRUE;
  }

  if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
  {
    p_Suplstart->sETCapabilities.posProtocol.m.ver2_PosProtocol_extensionPresent = 1;
    p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.lpp = TRUE;
  }
  else
  {
    p_Suplstart->sETCapabilities.posProtocol.rrlp = TRUE;
  }

  /**** SUPL-2 only: Service Capabilities ***/
  if ((p_supl_session_status->q_supl_version & 0x000000FF) == 2)
  {
    /***********  Service Capabilities optional SUPL2 extension ********/

#if 0 /* For future dev. */
    p_Suplstart->sETCapabilities.ver2_SETCapabilities_extension.m.serviceCapabilitiesPresent = 1;
    p_Suplstart->sETCapabilities.ver2_SETCapabilities_extension.m.supportedBearersPresent = 1;

    /* neither reportingCapabilities or eventTriggerCapabilities is present */

    /* neither periodic trigger or area event trigger is supported */

#endif
    p_Suplstart->sETCapabilities.posProtocol.m.ver2_PosProtocol_extensionPresent = 1;

    /***********  PosProtocolVersionRRLP SUPL2 extension in PosProtocol ****/
    if (p_Suplstart->sETCapabilities.posProtocol.rrlp == TRUE)
    {


      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.m.posProtocolVersionRRLPPresent = 1;

      /* default to the backward compatible version first, may be updated below */
      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.majorVersionField =
         SUPL_RRLP_CURRENT_VERSION_MAJOR;
      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.technicalVersionField =
         SUPL_RRLP_CURRENT_VERSION_TECH;
      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.editorialVersionField =
         SUPL_RRLP_CURRENT_VERSION_EDIT;
    }

    if (p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.lpp == TRUE)
    {
      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.m.posProtocolVersionLPPPresent = 1;

      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.majorVersionField = LPP_CURRENT_VERSION_MAJOR;
      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.technicalVersionField = LPP_CURRENT_VERSION_TECH;
      p_Suplstart->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.editorialVersionField = LPP_CURRENT_VERSION_EDIT;
    }

    /* Application ID */
    if (p_supl_session_status->z_supl_app_id.u_validity & PDAPI_SUPL_APPLICATION_ID_VALID)
    {
      p_Suplstart->ver2_SUPL_START_extension = &z_supl_Ver2_supl_start_extension;
      p_Suplstart->m.ver2_SUPL_START_extensionPresent = TRUE;

      /* making sure the app ID elements would never exceed their max. length */
      p_supl_session_status->z_supl_app_id.u_app_provider[PDAPI_SUPL_APPID_APP_PROVIDER_ARR_SIZE - 1] = 0;
      p_supl_session_status->z_supl_app_id.u_app_name[PDAPI_SUPL_APPID_APP_NAME_ARR_SIZE - 1] = 0;
      p_supl_session_status->z_supl_app_id.u_app_version[PDAPI_SUPL_APPID_APP_VERSION_ARR_SIZE - 1] = 0;

      z_supl_Ver2_supl_start_extension.m.applicationIDPresent = TRUE;

      z_supl_Ver2_supl_start_extension.applicationID.appProvider =
         (const char *)&p_supl_session_status->z_supl_app_id.u_app_provider[0];

      z_supl_Ver2_supl_start_extension.applicationID.appName =
         (const char *)&p_supl_session_status->z_supl_app_id.u_app_name[0];

      if (p_supl_session_status->z_supl_app_id.u_validity & PDAPI_SUPL_APPLICATION_ID_APP_VERSION_VALID)
      {
        z_supl_Ver2_supl_start_extension.applicationID.m.appVersionPresent = TRUE;
        z_supl_Ver2_supl_start_extension.applicationID.appVersion =
           (const char *)&p_supl_session_status->z_supl_app_id.u_app_version[0];
      }
    }

    /****** ver2_PosTechnology SUPL2 extension in posTechnology **********/

    // Glonass enabled and Protocol is not IS801
    if ((tm_umts_common_glonass_enabled() || tm_umts_common_bds_enabled())
        && (p_supl_session_status->u_position_protocol != TM_POS_PRTL_IS801))
    {
      supl_fill_ver2_ganss_set_capability(&(p_Suplstart->sETCapabilities), z_ganssPositionMethod,
                                          p_ganssPositionMethodsList, C_SUPL_SESSION_UI);
    }
  }
  /* populate the relevant part of SUPL_START with the cached cell ID info. */
  if (!supl_cell_information_construct(&p_Suplstart->locationId.cellInfo,
                                       &z_cell_info_allocated))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "No Cell ID info.", 0, 0, 0);
    return FALSE;
  }
#ifdef FEATURE_CGPS_LTE_CELLDB
  else if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) &&
           z_cell_info_cached.u_CachedCellIDValid  &&
           (z_cell_info_cached.e_ActiveRAT == CELLDB_ACTIVERAT_LTE) &&
           ((p_supl_session_status->q_supl_version & 0x000000FF) == 2)
           )
  {
    /* if SUPL2, and supl_cell_information_construct() returns TRUE */

    if ((p_Suplstart->locationId.cellInfo.u.ver2_CellInfo_extension->u.lteCell->m.rsrpResultPresent ||
         p_Suplstart->locationId.cellInfo.u.ver2_CellInfo_extension->u.lteCell->m.rsrqResultPresent ||
         p_Suplstart->locationId.cellInfo.u.ver2_CellInfo_extension->u.lteCell->m.tAPresent
         ) ||
        (z_lte_mrl.num_cells > 1)
        )
    {
      p_Suplstart->sETCapabilities.posTechnology.eCID = TRUE;
      p_supl_session_status->u_supl_posMethod_ecid_valid = TRUE;
    }


  }
#endif   /* FEATURE LTE_CELL_DB */


#ifdef FEATURE_USER_PLANE_MRL
  if ((phone_state_info.srv_system == CGPS_SRV_SYS_WCDMA) &&
      z_cell_info_cached.u_CachedCellIDValid &&
      (z_cell_info_cached.e_ActiveRAT == CELLDB_ACTIVERAT_WCDMA)
      )
  {
    p_Suplstart->sETCapabilities.posTechnology.eCID = TRUE;
    p_supl_session_status->u_supl_posMethod_ecid_valid = TRUE;
  }
#endif

  if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_NMR_SUPPORTED) != 0)
  {
    if (phone_state_info.srv_system == CGPS_SRV_SYS_GSM)
    {
      p_Suplstart->sETCapabilities.posTechnology.eCID = TRUE;
      p_supl_session_status->u_supl_posMethod_ecid_valid = TRUE;
    }
  }

#ifdef FEATURE_CGPS_SUPL_VARIANT_NO_ECID /* NOT defined by default */
  #error code not present
#endif


  /* for SET-Initiated, always declare CURRENT */
  p_Suplstart->locationId.status = supl_Status_current;


  /* MLID for SUPL_START */
  if ((p_supl_session_status->q_supl_version & 0x000000FF) >= 2)
  {
    p_Suplstart->ver2_SUPL_START_extension = &z_supl_Ver2_supl_start_extension;

    if (u_supl_global_num_mlid > 1) /* note being 1 means only the current serving cell is in the structure */
    {
      p_LocationIdData = (supl_LocationIdData *)os_MemAlloc((sizeof(supl_LocationIdData) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);
      p_LocationId = (supl_LocationId *)os_MemAlloc((sizeof(supl_LocationId) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);
      p_cell_info_allocated_mlid = (supl_cell_info_struct_type *)os_MemAlloc((sizeof(supl_cell_info_struct_type) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);

      if ((p_LocationIdData == NULL) || (p_LocationId == NULL) || (p_cell_info_allocated_mlid == NULL))
      {
        u_num_mlid = 0;
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "mlid mem. allocation failed, set num_mlid to %u ", u_num_mlid);
      }
      else
      {
        memset(p_LocationIdData, 0, sizeof(supl_LocationIdData) * (u_supl_global_num_mlid - 1));
        memset(p_LocationId, 0, sizeof(supl_LocationId) * (u_supl_global_num_mlid - 1));
        memset(p_cell_info_allocated_mlid, 0, sizeof(supl_cell_info_struct_type) * (u_supl_global_num_mlid - 1));

        p_LocationIdData->locationId = p_LocationId;
        u_num_mlid = supl_chk_and_build_all_mlid(p_LocationIdData, p_LocationId,
                                                 p_cell_info_allocated_mlid, (u_supl_global_num_mlid - 1));
      }

    }
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "num_MLID %u ", u_num_mlid);

    if (u_num_mlid > 0)
    {
      if (p_LocationIdData != NULL)
      {
        p_Suplstart->m.ver2_SUPL_START_extensionPresent = TRUE;
        p_Suplstart->ver2_SUPL_START_extension = &z_supl_Ver2_supl_start_extension;
        z_supl_Ver2_supl_start_extension.m.multipleLocationIdsPresent = TRUE;
        z_supl_Ver2_supl_start_extension.multipleLocationIds.n = u_num_mlid;
        z_supl_Ver2_supl_start_extension.multipleLocationIds.elem = p_LocationIdData;
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "p_LocationIdData not allocated: %lx ", p_LocationIdData);
      }
    }
  } /* running SUPL ver. >= 2 */


  /* QOP */
#ifndef FEATURE_CGPS_SUPL_VARIANT_SUPLSTART_NO_QOP /* NOT by default defined */
  p_Suplstart->m.qoPPresent = 1;
  if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
  {
    p_Suplstart->qoP.horacc = 127; /* max. horacc for cell-ID request */
  }
  else
  {
    p_Suplstart->qoP.horacc = tm_umts_common_utils_meter_to_GADK(TRUE, p_supl_session_status->z_session_qos.q_hor_acc_meters);
  }

  /* delay: so that SLP can set a reasonable response time in RRLP */
  p_Suplstart->qoP.m.delayPresent = 1;
  p_Suplstart->qoP.delay =
     tm_umts_common_next_int_log2_max_128(p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec / 1000);


  /* for OTDOA only or AFLT only, the QoS is 0 from SI session. use the default 16 sec*/
  if (p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec == 0)
  {
    p_Suplstart->qoP.delay = 4;

  }
  /* MaxLocAge: optional - maximum tolerable age of position estimates used for cached position
     fixes; units in seconds from 0 to 65535. --- OMA-TS-UPL-V1_0-20050627-D */
  p_Suplstart->qoP.m.maxLocAgePresent = 1;
  p_Suplstart->qoP.maxLocAge = 0; /* doesn't hurt to always include this and set it to 0 */
#endif


  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, PROTO_SUPL);

  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }

    if (p_cell_info_allocated_mlid != NULL)
    {
      os_MemFree((void **)&p_cell_info_allocated_mlid);
      p_cell_info_allocated_mlid = NULL;
    }
    if (p_LocationId != NULL)
    {
      os_MemFree((void **)&p_LocationId);
      p_LocationId = NULL;
    }
    if (p_LocationIdData != NULL)
    {
      p_LocationIdData->locationId = NULL;
      os_MemFree((void **)&p_LocationIdData);
      p_LocationIdData = NULL;
    }

    return FALSE;
  }

  else
  {
    pd_comms_return_type ret_val;

    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (pdu_buff.length >> 8) & 0x00FF;
    u_length_lsb = pdu_buff.length & 0x00FF;

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, pdu_buff.length);

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplStart attempt aborted", 0, 0, 0);
    }
    else
    {
      supl_set_cf_state(C_SUPL_CF_STATE_UT1, 0);
      u_ret = TRUE;
    }

    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
    pdu_buff.value = NULL;

  }

  if (p_cell_info_allocated_mlid != NULL)
  {
    os_MemFree((void **)&p_cell_info_allocated_mlid);
    p_cell_info_allocated_mlid = NULL;
  }
  if (p_LocationId != NULL)
  {
    os_MemFree((void **)&p_LocationId);
    p_LocationId = NULL;
  }
  if (p_LocationIdData != NULL)
  {
    p_LocationIdData->locationId = NULL;
    os_MemFree((void **)&p_LocationIdData);
    p_LocationIdData = NULL;
  }

  if (u_ret)
  {
    p_supl_session_status->u_network_session_on = TRUE;
    p_supl_session_status->u_send_supl_end_at_fail = TRUE;
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SuplStart sent ", 0, 0, 0);

  }

  return u_ret;
}

/*
******************************************************************************
* supl_are_posmethod_agps_mode_inconsistent
*
* Function description:
*
* Check if the incoming posMethod in SUPL_RESPONSE and SUPL_START is consistent
*
* Parameters: 
*  posMethod: Incoming Pos Method in SUPL_RESPONSE
*  u_agps_mode: u_agps_method sent out in SUPL_START
*  u_supl_posMethod_ecid_valid: ecid valid in incoming SUPL_RESPONSE
*  u_position_protocol: Position protocol being run
*
* Return value: TRUE/FALSE
*
******************************************************************************
*/
static boolean supl_are_posmethod_agps_mode_inconsistent
(
   supl_PosMethod                    posMethod,
   supl_session_agps_mode_enum_type  u_agps_mode,
   boolean                           u_supl_posMethod_ecid_valid,
   tm_positioning_protocols          u_position_protocol
   )
{
  if (((posMethod == supl_agpsSETbased) ||
       (posMethod == supl_PosMethod_ver2_agnssSETbased)) &&
      (u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED))
  {
    return TRUE;
  }

  if (((posMethod == supl_agpsSETassisted) ||
       (posMethod == supl_PosMethod_ver2_agnssSETassisted)) &&
      (u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED))
  {
    return TRUE;
  }

  if (((posMethod == supl_autonomousGPS) ||
       (posMethod == supl_PosMethod_ver2_autonomousGNSS)) &&
      (u_agps_mode != C_SUPL_AGPS_MODE_UE_BASED))
  {
    return TRUE;
  }

  /* Proceed with SuplPosInit only if WCDMA Cell DB information is available*/
  if ((posMethod == supl_eCID) &&
      (u_supl_posMethod_ecid_valid != TRUE))
  {
    return TRUE;
  }

  /*AFLT only supported for IS801*/
  if ((posMethod == supl_aFLT) &&
      ((u_agps_mode != C_SUPL_AFLT_MODE) ||
       (u_position_protocol != TM_POS_PRTL_IS801)))
  {
    return TRUE;
  }

  /* OTDOA only when LPP is selected */
  if ((posMethod == supl_oTDOA) &&
      ((u_agps_mode != C_SUPL_OTDOA_MODE) ||
       (u_position_protocol != TM_POS_PRTL_LPP)))
  {
    return TRUE;
  }

  if (posMethod == supl_eOTD)
  {
    return TRUE;
  }

  return FALSE;
}

/*
******************************************************************************
* supl_SuplResponse_proc
*
* Function description:
*
* Process incoming SUPL RESPONSE message
*
* Parameters: p_pdu_decoded: pointer to a structure holding decoded Supl PDU.
*
* Return value: TRUE/FALSE
*
******************************************************************************
*/
static boolean supl_SuplResponse_proc(const supl_ULP_PDU *p_pdu_decoded)
{

  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;

  boolean u_ret = FALSE;

  const supl_SUPLRESPONSE *p_suplResponse;

  uint8 u_set_abds_enabled = FALSE,
     u_set_aglo_enabled = FALSE,
     u_set_agps_enabled = TRUE;

  uint8 u_pos_method_mismatch = FALSE;
  uint8 u_slp_agnss_desired   = FALSE;

  supl_store_slp_sessionId(p_pdu_decoded);

  p_suplResponse = p_pdu_decoded->message.u.msSUPLRESPONSE;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "process_supl_response()", 0, 0, 0);


  if (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT1))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "bad cf state 0x%x", p_supl_session_status->e_cf_state);
    (void)tm_umts_up_supl_supl_end_tx(&p_supl_session_status->z_session_id.z_Slp_Sess_Id,
                                      NULL,
                                      supl_StatusCode_unexpectedMessage,
                                      NULL,
                                      p_supl_session_status->q_supl_version);
    tm_umts_up_supl_callflow_disrupt_handler();
    return FALSE;
  }

  tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT1);


  /* if SUPL_RESPONSE and SUPL_START do not match in terms of posMethod, SUPL_END it.
     note if SLP asks for autonomous, the application must have requested SET-Based or
     it should be considered a mismatch */
  if (supl_are_posmethod_agps_mode_inconsistent(
        p_suplResponse->posMethod,
        p_supl_session_status->u_agps_mode,
        p_supl_session_status->u_supl_posMethod_ecid_valid,
        p_supl_session_status->u_position_protocol))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "posMethod mismatch supl resp:0x%x agps_mode %d pos_prtl %d",
          p_suplResponse->posMethod, p_supl_session_status->u_agps_mode, p_supl_session_status->u_position_protocol);
    (void)tm_umts_up_supl_supl_end_tx(&p_supl_session_status->z_session_id.z_Slp_Sess_Id,
                                      NULL,
                                      supl_posMethodMismatch,
                                      NULL,
                                      p_supl_session_status->q_supl_version);
    tm_umts_up_supl_callflow_disrupt_handler();
    return FALSE; /* don't proceed */
  }

  if ((p_suplResponse->posMethod == supl_PosMethod_agpsSETassisted) ||
      (p_suplResponse->posMethod == supl_PosMethod_agpsSETbased) ||
      (p_suplResponse->posMethod == supl_PosMethod_agpsSETassistedpref) ||
      (p_suplResponse->posMethod == supl_PosMethod_agpsSETbasedpref) ||
      (p_suplResponse->posMethod == supl_PosMethod_autonomousGPS)
      )
  {
    p_supl_session_status->u_slp_support_gps = TRUE;
  }

  /* Determine if the H-SLP supports any GANNS capability */
  if ((p_suplResponse->posMethod != supl_PosMethod_ver2_agnssSETassisted) &&
      (p_suplResponse->posMethod != supl_PosMethod_ver2_agnssSETbased) &&
      (p_suplResponse->posMethod != supl_PosMethod_ver2_agnssSETassistedpref) &&
      (p_suplResponse->posMethod != supl_PosMethod_ver2_agnssSETbasedpref) &&
      (p_suplResponse->posMethod != supl_PosMethod_ver2_autonomousGNSS)
      )
  {
    p_supl_session_status->u_slp_support_glonass = FALSE;
    p_supl_session_status->u_slp_support_bds = FALSE;
  }
  else
  {
    u_slp_agnss_desired = TRUE;
  }

  if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
  {
    /*Check if LTE cell information is available else start a timer of 100 ms 
      to retry cell information get*/
    if ((FALSE == supl_lte_cell_information_construct()))
    {
      /*Move to sub-state C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, and start timer*/
      supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, 0);
    }
    else
    {
      supl_SuplPosInit_tx();
    }
    return TRUE;
  }

  if (p_suplResponse->posMethod == supl_noPosition) /* this posMethod makes no sense in SuplResponse */
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "posMethod noPos %d", p_suplResponse->posMethod);
    (void)tm_umts_up_supl_supl_end_tx(&p_supl_session_status->z_session_id.z_Slp_Sess_Id,
                                      NULL,
                                      supl_unexpectedDataValue,
                                      NULL,
                                      p_supl_session_status->q_supl_version);
    tm_umts_up_supl_callflow_disrupt_handler();
    return FALSE; /* don't proceed */
  }

  if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
  {
    u_set_abds_enabled = tm_umts_up_supl_rrlp_assisted_bds_enabled();
    u_set_aglo_enabled = tm_umts_up_supl_rrlp_assisted_glonass_enabled();
  }
  else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
  {
    u_set_abds_enabled = FALSE;
    u_set_aglo_enabled = (tm_core_get_asst_glo_lpp_up_enable() &&
                          tm_core_get_lte_lpp_up_enable());
  }

  /****** check AGNSS compatibility if desired by HSLP ************/
  do
  {
    u_pos_method_mismatch = FALSE;
    if (!u_slp_agnss_desired)
    {
      break;
    }

    /* SLP Desires AGNSS!! Check if Ver2 Ext present */
    if (!(p_suplResponse->m.ver2_SUPL_RESPONSE_extensionPresent) ||
        !(p_suplResponse->ver2_SUPL_RESPONSE_extension.m.gnssPosTechnologyPresent))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "AGNSS posmethod but Ver2 SUPL_RESP missing. Default to GPS+GLO!!");
      p_supl_session_status->u_slp_support_gps     = TRUE;
      p_supl_session_status->u_slp_support_glonass = TRUE;
      u_pos_method_mismatch = FALSE;
      break;
    }

    /* SUPL-2 extension and GNSS capability present */
    p_supl_session_status->u_slp_support_gps =
       p_supl_session_status->u_slp_support_gps ||
       p_suplResponse->ver2_SUPL_RESPONSE_extension.gnssPosTechnology.gps;
    p_supl_session_status->u_slp_support_glonass = p_suplResponse->ver2_SUPL_RESPONSE_extension.gnssPosTechnology.glonass;
    p_supl_session_status->u_slp_support_bds = p_suplResponse->ver2_SUPL_RESPONSE_extension.gnssPosTechnology.bds;

    MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SLP Supports GPS:%d glo:%d bds:%d, UE glo:%d bds:%d",
          p_supl_session_status->u_slp_support_gps,
          p_supl_session_status->u_slp_support_glonass,
          p_supl_session_status->u_slp_support_bds,
          u_set_aglo_enabled, u_set_abds_enabled);

    if (p_supl_session_status->u_slp_support_gps != TRUE)
    {
      /* if slp doesnt support GPS, the following combos below arent supported */
      if (((p_supl_session_status->u_slp_support_glonass != TRUE) &&
           (p_supl_session_status->u_slp_support_bds != TRUE))  /* All three not allowed!! */
          || ((p_supl_session_status->u_slp_support_glonass == TRUE) &&
              (p_supl_session_status->u_slp_support_bds != TRUE))  /* Only Glo allowed! */
          || ((p_supl_session_status->u_slp_support_glonass == TRUE) &&
              (p_supl_session_status->u_slp_support_bds == TRUE))) /* Only Glo and BDS! */
      {
        /* need to send SUPL_END posMethodMismatch in the following cases
         * 1. GLO Only
         * 2. Glo + BDS only (no gps) 
         * 3. None of GLO, GPS & BDS */
        u_pos_method_mismatch = TRUE;
        break;
      }
    }

    /* if there is no gnss common in the position method asked by the server
     * and whats enabled in the set, we have a position method mismatch! */
    u_pos_method_mismatch = u_pos_method_mismatch ||
       !((p_supl_session_status->u_slp_support_gps && u_set_agps_enabled) ||
         (p_supl_session_status->u_slp_support_glonass && u_set_aglo_enabled) ||
         (p_supl_session_status->u_slp_support_bds && u_set_abds_enabled));
    break;
  }while (0);

  if (u_pos_method_mismatch == TRUE)
  {
    (void)tm_umts_up_supl_supl_end_tx(
       &p_supl_session_status->z_session_id.z_Slp_Sess_Id,
       NULL,
       supl_StatusCode_posMethodMismatch,
       NULL,
       p_supl_session_status->q_supl_version);
    tm_umts_up_supl_callflow_disrupt_handler();
    return FALSE; /* don't proceed */
  }

  /* agreement on posMethod has been reached */
  /* if assist data is needed, ask for assistance now  */
  if ((p_supl_session_status->z_GpsAssistData.u_Octet3_Whishlist_AH != 0) ||
      (p_supl_session_status->z_GpsAssistData.u_Octet4_Whishlist_IP != 0)
      ||
      (p_supl_session_status->z_GloAssistData.u_Octet3_Whishlist_AH != 0) ||
      (p_supl_session_status->z_GloAssistData.u_Octet4_Whishlist_IP != 0)
      ||
      (p_supl_session_status->z_BdsAssistData.q_WishListMask != 0)

      /*If IS801, then wishlist will be present with IS801, if we are here at this point then IS801
        needs network assistance*/
      || (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
      )
  {
    /*Check if LTE cell information is available else start a timer of 100 ms 
      to retry cell information get*/
    if ((FALSE == supl_lte_cell_information_construct()))
    {
      /*Move to sub-state C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, and start timer*/
      supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, 0);
    }
    else
    {
      if (supl_SuplPosInit_tx())
      {
        u_ret = TRUE;
        if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
        {
          /*Wait for write ack of SUPL_POS_INIT before notifying IS801 module*/
          z_supl_session_status.wait_for_write_ack = TRUE;
        }
      }
    }
  }
  else
  {
    /* should not happen */
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Dont send SUPLPosInit as no AD needed");
    u_ret = FALSE;
  }

  return u_ret;


}

/*
******************************************************************************
* supl_SuplPosInit_RRLP_UEBased_GPSADReq_build
*
* Function description:
*
* Build the AD request for GPS UE Based in the SUPL POS Init to be sent to the 
* server.
*
* Parameters: 
*  p_GpsAssistData  : GPS Wishlist
*  p_requestedAssistData: SUPL AD request updated by this function
*  p_sat_info : list of per SV AD request
*  p_sat_info_element: per SV AD request
* 
*
* Return value: 
*  TRUE  : Success
*  FALSE : Failed to build the request
*
******************************************************************************
*/
static boolean supl_SuplPosInit_RRLP_UEBased_GPSADReq_build
(
   const GpsAssisData_Gsm0931 *const p_GpsAssistData,
   supl_RequestedAssistData   *p_requestedAssistData,
   supl_SatelliteInfo         *p_sat_info,
   supl_SatelliteInfoElement  p_sat_info_element[]
   )
{
  uint8    u_i = 0;
  boolean  asn1_range_error = FALSE;

  if ((p_GpsAssistData == NULL) ||
      (p_requestedAssistData == NULL) ||
      (p_sat_info == NULL) ||
      (p_sat_info_element == NULL))
  {
    MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL ptrs %d %d %d %d",
      NULL==p_GpsAssistData, NULL == p_requestedAssistData, NULL == p_sat_info, NULL == p_sat_info_element);
    return FALSE;
  }

  if ((p_GpsAssistData->u_Octet3_Whishlist_AH == 0) &&
      (p_GpsAssistData->u_Octet4_Whishlist_IP == 0)
      )
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Wishlist empty 0x%x 0x%x",
          p_GpsAssistData->u_Octet3_Whishlist_AH, p_GpsAssistData->u_Octet4_Whishlist_IP);
    return FALSE;
  }

  if (p_GpsAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_ALM)
    p_requestedAssistData->almanacRequested = TRUE;
  if (p_GpsAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_UTC)
    p_requestedAssistData->utcModelRequested = TRUE;
  if (p_GpsAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_IONO)
    p_requestedAssistData->ionosphericModelRequested = TRUE;
  if (p_GpsAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_NAVMODL)
    p_requestedAssistData->navigationModelRequested = TRUE;
  if (p_GpsAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_REFLOC)
    p_requestedAssistData->referenceLocationRequested = TRUE;
  if (p_GpsAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_REFTIME)
    p_requestedAssistData->referenceTimeRequested = TRUE;
  if (p_GpsAssistData->u_Octet4_Whishlist_IP & (C_G0931_GPS_ASSIST_RTI >> 8))
    p_requestedAssistData->realTimeIntegrityRequested = TRUE;


#ifndef FEATURE_CGPS_SUPL_VARIANT_NO_TARGETED_EPH  /* NOT defined by default */
  if (p_requestedAssistData->navigationModelRequested == TRUE)
  {
    p_requestedAssistData->m.navigationModelDataPresent = 1;

#if 0 /* TEST code only: protocol test code only: keep this here for possible quick protocol
              verfication but make sure it is not activated; test is conducted by deleting
              eph. first to clear the saved info. */

    p_GpsAssistData->u_Octet8_NSAT_ToeLimit = 0x10; /* SV num. = 1 */
    p_GpsAssistData->w_SvId_Iode[0] = 0x1207; /* SV ID: 7; IODE: 0x12 */
    p_GpsAssistData->u_Octet6_GpsWeek_LSB8 = 0xFE; /* week: 1022 = 0x3FE */
    p_GpsAssistData->u_Octet5_GpsWeek_MSB2 = 0x0C;  /* MSB of 10-bit moved to MSB of 8-bit */
    p_GpsAssistData->u_Octet7_GpsToe = 150;
    p_GpsAssistData->u_Octet8_NSAT_ToeLimit |= 0x5; /* ToeLimit: 5 */
#endif


    if ((p_GpsAssistData->u_Octet8_NSAT_ToeLimit >> 4) > 0)
    {
      uint8 u_nsat = 0;

      p_requestedAssistData->navigationModelData.m.satInfoPresent = 1;

      p_requestedAssistData->navigationModelData.gpsWeek =
         p_GpsAssistData->u_Octet6_GpsWeek_LSB8;

      p_requestedAssistData->navigationModelData.gpsWeek |=
         ((uint16)p_GpsAssistData->u_Octet5_GpsWeek_MSB2) << 2;

      p_requestedAssistData->navigationModelData.gpsToe =
         p_GpsAssistData->u_Octet7_GpsToe;

      u_nsat = p_GpsAssistData->u_Octet8_NSAT_ToeLimit >> 4;
      p_requestedAssistData->navigationModelData.nSAT =
         (u_nsat <= 15) ? u_nsat : 15;

      p_requestedAssistData->navigationModelData.toeLimit =
         p_GpsAssistData->u_Octet8_NSAT_ToeLimit & 0x0F;

      /* asn1 range check:
         gpsWeek   INTEGER(0..1023);
         gpsToe    INTEGER(0..167);
         nSAT      INTEGER(0..31);
         toeLimit  INTEGER(0..10)  */
      asn1_range_error = FALSE;
      if ((p_requestedAssistData->navigationModelData.gpsWeek > 1023) ||
          (p_requestedAssistData->navigationModelData.gpsToe > 167) ||
          (p_requestedAssistData->navigationModelData.nSAT > 31) ||
          (p_requestedAssistData->navigationModelData.toeLimit > 10)
          )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "navModelData out of asn1 range !", 0, 0, 0);
        asn1_range_error = TRUE;
      }

      p_sat_info->n = p_requestedAssistData->navigationModelData.nSAT;
      p_sat_info->elem = p_sat_info_element;


      for (u_i = 0; u_i < p_requestedAssistData->navigationModelData.nSAT; u_i++)
      {
        p_sat_info_element[u_i].satId =
           (p_GpsAssistData->w_SvId_Iode[u_i] & 0x003F) - 1;

        p_sat_info_element[u_i].iODE =
           p_GpsAssistData->w_SvId_Iode[u_i] >> 8;

        /* asn1 range check:  satId  INTEGER(0..63),  iODE   INTEGER(0..255) */
        if ((p_sat_info_element[u_i].satId > 63) || (p_sat_info_element[u_i].iODE > ((255))))
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "navModelData out of asn1 range !", 0, 0, 0);
          asn1_range_error = TRUE;
        }

      }

#if 0
      memscpy( &p_requestedAssistData->navigationModelData.satInfo,
              sizeof(p_requestedAssistData->navigationModelData.satInfo),
              p_sat_info, sizeof(*p_sat_info));
#endif
      p_requestedAssistData->navigationModelData.satInfo.n =
         p_requestedAssistData->navigationModelData.nSAT;
      p_requestedAssistData->navigationModelData.satInfo.elem =
         p_sat_info_element;

      if (asn1_range_error)
      {
        /* some vaule(s) out of range for ASN.1, don't encode with navModelData */
        p_requestedAssistData->navigationModelData.m.satInfoPresent = 0;
        p_requestedAssistData->navigationModelData.gpsWeek = 0;
        p_requestedAssistData->navigationModelData.gpsToe = 0;
        p_requestedAssistData->navigationModelData.nSAT = 0;
        p_requestedAssistData->navigationModelData.toeLimit = 0;
      }

    } /* closing if-NavData included */

    else /* if no NavData included */
    {
      p_requestedAssistData->navigationModelData.m.satInfoPresent = 0;
      p_requestedAssistData->navigationModelData.gpsWeek = 0;
      p_requestedAssistData->navigationModelData.gpsToe = 0;
      p_requestedAssistData->navigationModelData.nSAT = 0;
      p_requestedAssistData->navigationModelData.toeLimit = 0;

      p_requestedAssistData->navigationModelData.satInfo.n = 0;
      p_requestedAssistData->navigationModelData.satInfo.elem = NULL;
    }

  } /* closing if-NavModel request */
#endif  /* feature_no_target_eph */
  return TRUE;
}

/*
******************************************************************************
* supl_SuplPosInit_RRLP_UEBased_BdsADReq_build
*
* Function description:
*
* Build the AD request for BDS UE Based in the SUPL POS Init to be sent to the 
* server.
*
* Parameters: 
*  p_BdsAssistData  : BDS Wishlist
*  p_ganss_req_generic_data: SUPL GANSS AD requst built by this function
*  p_requestedAssistData: SUPL AD request updated by this function
* 
*
* Return value: 
*  TRUE  : Success
*  FALSE : Failed to build the request
*
******************************************************************************
*/
static boolean supl_SuplPosInit_RRLP_UEBased_BdsADReq_build
(
   const sm_BdsAssistDataStruct        *const p_BdsAssistData,
   supl_GanssReqGenericData            *p_ganss_req_generic_data,
   supl_RequestedAssistData            *p_requestedAssistData
   )
{
  boolean  asn1_range_error = FALSE;
  uint8 u_i = 0;

  if ((p_BdsAssistData == NULL) ||
      (p_ganss_req_generic_data == NULL) ||
      (p_requestedAssistData == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL Pointers! %d %d %d",
          p_BdsAssistData, p_ganss_req_generic_data, p_requestedAssistData);
    return FALSE;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "reqAD WL 0x%x",
        p_BdsAssistData->q_WishListMask);

  if (p_BdsAssistData->q_WishListMask & C_GPS_ASSIS_REFTIME)
  {
    /* Request BDS Ref Time */
    p_requestedAssistData->ver2_RequestedAssistData_extension.m.ganssRequestedCommonAssistanceDataListPresent = TRUE;
    p_requestedAssistData->ver2_RequestedAssistData_extension.ganssRequestedCommonAssistanceDataList.ganssReferenceTime = TRUE;
  }
  if (p_BdsAssistData->q_WishListMask & C_GPS_ASSIS_REFLOC)
  {
    /* note there is no gnss refloc request in the supl-2 extension, since
       ref loc. is generic to GPS or other technologies, the request goes
       out only in the one originally for GPS. */
    p_requestedAssistData->referenceLocationRequested = TRUE;
  }
  if (p_BdsAssistData->q_WishListMask & C_GPS_ASSIS_NAVMODL)
  {
    p_ganss_req_generic_data->m.ganssNavigationModelDataPresent =  1;
    /* dont need any aux info for BDS */
    p_ganss_req_generic_data->ganssAuxiliaryInformation = FALSE;
    p_ganss_req_generic_data->ganssNavigationModelData.extElem1.count = 0;
  }
  if (p_BdsAssistData->q_WishListMask & C_GPS_ASSIS_ALM)
  {
    p_ganss_req_generic_data->ganssAlmanac = TRUE;
  }

  /* Always request RTI if needed, or if anything else is needed */
  if ((p_BdsAssistData->q_WishListMask & C_GPS_ASSIS_RTI) ||
      (p_BdsAssistData->q_WishListMask))
  {
    p_ganss_req_generic_data->ganssRealTimeIntegrity = TRUE;
  }

  /* Always Request Time Model if requesting anything else ******************/
  if ((p_BdsAssistData->q_WishListMask & C_GNSS_ASSIST_TIME_MODELS) ||
      (p_BdsAssistData->q_WishListMask))
  {
    p_ganss_req_generic_data->m.ganssTimeModelsPresent = TRUE;
    p_ganss_req_generic_data->ganssTimeModels.numbits = 16;
    p_ganss_req_generic_data->ganssTimeModels.data[0] = C_GANSS_SYSTEM_TIME_MODEL_REQUESTED_GPS;
    p_ganss_req_generic_data->ganssTimeModels.data[1] = 0x0;
  }

  /* Request Iono ONLY if we are requesting something */
  if ((p_BdsAssistData->q_WishListMask) &&
      (p_BdsAssistData->q_WishListMask & C_GPS_ASSIS_IONO))
  {
    p_requestedAssistData->ver2_RequestedAssistData_extension.m.ganssRequestedCommonAssistanceDataListPresent = TRUE;
    p_requestedAssistData->ver2_RequestedAssistData_extension.ganssRequestedCommonAssistanceDataList.ganssIonosphericModel = TRUE;
    p_requestedAssistData->ver2_RequestedAssistData_extension.ganssRequestedCommonAssistanceDataList.m.ganssAdditionalIonosphericModelForDataID01Present = TRUE;
    p_requestedAssistData->ver2_RequestedAssistData_extension.ganssRequestedCommonAssistanceDataList.ganssAdditionalIonosphericModelForDataID00 = FALSE;
    p_requestedAssistData->ver2_RequestedAssistData_extension.ganssRequestedCommonAssistanceDataList.ganssAdditionalIonosphericModelForDataID11 = FALSE;
    p_requestedAssistData->ver2_RequestedAssistData_extension.ganssRequestedCommonAssistanceDataList.ganssAdditionalIonosphericModelForDataID01 = TRUE;
  }

  p_ganss_req_generic_data->ganssId = C_SUPL2_GANSS_ID_BDS;
  p_ganss_req_generic_data->extElem1.count = 0;
  return TRUE;
}

/*
******************************************************************************
* supl_SuplPosInit_RRLP_UEBased_GloADReq_build
*
* Function description:
*
* Build the AD request for Glonass in the SUPL POS Init to be sent to the 
* server.
*
* Parameters: 
*  p_GloAssistData          : Glonass Wishlist
*  p_ganss_req_generic_data : Generic ganns AD Request sent in SUPL_POS_INIT
*  p_requestedAssistData    : The overall glonass AD req
*  p_ganss_sat_info         : per SV AD Req
*  p_ganss_sat_info_element : 
*  p_orig_wishlist_mask     : Original Wishlist from MGP
* 
*
* Return value: 
*  TRUE  : Success
*  FALSE : Failed to build the request
*
******************************************************************************
*/
static boolean supl_SuplPosInit_RRLP_UEBased_GloADReq_build
(
   const GpsAssisData_ganss_Gsm0931    *const p_GloAssistData,
   supl_GanssReqGenericData            *p_ganss_req_generic_data,
   supl_RequestedAssistData            *p_requestedAssistData,
   supl_SatellitesListRelatedDataList  *p_ganss_sat_info,
   supl_SatellitesListRelatedData      *p_ganss_sat_info_element,
   uint32 *p_orig_wishlist_mask
   )
{
  boolean  asn1_range_error = FALSE;
  uint8 u_i = 0;

  if ((p_GloAssistData == NULL) ||
      (p_ganss_req_generic_data == NULL) ||
      (p_ganss_sat_info == NULL) ||
      (p_ganss_sat_info_element == NULL) ||
      (p_requestedAssistData == NULL))
  {
    MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL Pointers! %d %d %d %d",
      NULL == p_GloAssistData, NULL == p_ganss_req_generic_data, NULL == p_requestedAssistData,
      NULL == p_orig_wishlist_mask);
    return FALSE;
  }

  if ((p_GloAssistData->u_Octet3_Whishlist_AH == 0) &&
      (p_GloAssistData->u_Octet4_Whishlist_IP == 0)
      )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "AD WishList 0!");
    return FALSE;
  }

  /********  Common assistance request ****************************/

  if (p_GloAssistData->u_Octet3_Whishlist_AH &
         C_G0931_GPS_ASSIST_REFLOC)
  {
    /* note there is no gnss refloc request in the supl-2 extension, since
       ref loc. is generic to GPS or other technologies, the request goes
       out only in the one originally for GPS. */
    p_requestedAssistData->referenceLocationRequested = TRUE;
  }

  if (p_GloAssistData->u_Octet3_Whishlist_AH &
         C_G0931_GPS_ASSIST_REFTIME)
  {
    /* instead of requesting GLONASS reference time, request GPS ref. time and UTC */
    p_requestedAssistData->referenceTimeRequested = TRUE;
    p_requestedAssistData->utcModelRequested = TRUE;
  }

  if (p_GloAssistData->u_Octet3_Whishlist_AH &
         C_G0931_GPS_ASSIST_IONO)
  {
    p_requestedAssistData->ver2_RequestedAssistData_extension.m.ganssRequestedCommonAssistanceDataListPresent = TRUE;
    p_requestedAssistData->ver2_RequestedAssistData_extension.ganssRequestedCommonAssistanceDataList.ganssIonosphericModel = TRUE;
  }

  /********** Generic assistance request **************************/
  if (*p_orig_wishlist_mask & C_GNSS_ASSIST_TIME_MODELS)
  {
    *p_orig_wishlist_mask &= ~C_GNSS_ASSIST_TIME_MODELS;
    p_ganss_req_generic_data->m.ganssTimeModelsPresent = TRUE;
    p_ganss_req_generic_data->ganssTimeModels.numbits = 16;
    p_ganss_req_generic_data->ganssTimeModels.data[0] = C_GANSS_SYSTEM_TIME_MODEL_REQUESTED_GPS;
    p_ganss_req_generic_data->ganssTimeModels.data[1] = 0x0;
  }

  if (p_GloAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_NAVMODL)
  {
    p_ganss_req_generic_data->m.ganssNavigationModelDataPresent =  1;

    /* if nav. model is request, also request aux. which has freqency info. */
    p_ganss_req_generic_data->ganssAuxiliaryInformation = TRUE;
    p_ganss_req_generic_data->ganssNavigationModelData.extElem1.count = 0;

#ifndef FEATURE_CGPS_SUPL_VARIANT_NO_TARGETED_EPH  /* NOT defined by default */

#if 0 /* DONT REMOVE: TEST code only: protocol test code only: keep this here for possible
                quick protocol verfication but make sure it is not activated; test is conducted by
                deleting eph. first to clear the saved info. */

    p_GloAssistData->u_Octet8_NSAT_ToeLimit = 0xF0; /* SV num. = 1 */

    p_GloAssistData->w_SvId_Iode[0] = 0x1101; /* SV ID: 1; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[1] = 0x1202; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[2] = 0x1303; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[3] = 0x1404; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[4] = 0x1505; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[5] = 0x1606; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[6] = 0x1707; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[7] = 0x1808; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[8] = 0x1909; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[9] = 0x1A0A; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[10] = 0x1B0B; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[11] = 0x1C0C; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[12] = 0x1D0D; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[13] = 0x1E0E; /* SV ID: 7; IODE: 0x12 */
    p_GloAssistData->w_SvId_Iode[14] = 0x1F0F; /* SV ID: 7; IODE: 0x12 */

    p_GloAssistData->u_Octet6_GanssWeek_LSB8 = 0xFE; /* week: 1022 = 0x3FE */
    p_GloAssistData->u_Octet5_GanssWeek_MSB4 = 0x0C;  /* MSB of 10-bit moved to MSB of 8-bit */
    p_GloAssistData->u_Octet7_GanssToe = 150;
    p_GloAssistData->u_Octet8_NSAT_ToeLimit |= 0x5; /* ToeLimit: 5 */

#endif /* TEST only code */

    if ((p_GloAssistData->u_Octet8_NSAT_ToeLimit >> 4) > 0)
    {
      uint8 u_nsat = 0;

      p_ganss_req_generic_data->ganssNavigationModelData.m.satellitesListRelatedDataListPresent = 1;

      /* Refer to 80-V5430-3 A , Table 3-28
           for the format of GpsAssisData_ganss_Gsm0931 for data processing below.
         Note: ganssWeek is 12 bit field (gpsweek is 10 bit).
               IODE is      10 bit field (gps IODE is 8 bit).
      */
      p_ganss_req_generic_data->ganssNavigationModelData.ganssWeek =
         p_GloAssistData->u_Octet6_GanssWeek_LSB8;
      p_ganss_req_generic_data->ganssNavigationModelData.ganssWeek |=
         ((uint16)p_GloAssistData->u_Octet5_GanssWeek_MSB4) << 2;

      p_ganss_req_generic_data->ganssNavigationModelData.ganssToe =
         p_GloAssistData->u_Octet7_GanssToe;

      u_nsat = p_GloAssistData->u_Octet8_NSAT_ToeLimit >> 4;
      p_ganss_req_generic_data->ganssNavigationModelData.satellitesListRelatedDataList.n =
         (u_nsat <= 15) ? u_nsat : 15;

      p_ganss_req_generic_data->ganssNavigationModelData.t_toeLimit =
         p_GloAssistData->u_Octet8_NSAT_ToeLimit & 0x0F;

      /* asn1 range check:
         ganssWeek   INTEGER(0..4095);
         ganssToe    INTEGER(0..167);
         toeLimit    INTEGER(0..15) )
         NOTE - 80-V5430-3 A says range is 0..10 but tm_supl_asn1.c code allows up to 15
      */
      asn1_range_error = FALSE;
      if ((p_ganss_req_generic_data->ganssNavigationModelData.ganssWeek > 4095) ||
          (p_ganss_req_generic_data->ganssNavigationModelData.ganssToe > 167) ||
          (p_ganss_req_generic_data->ganssNavigationModelData.t_toeLimit > 15)
          )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "GANSS navModelData out of asn1 range !", 0, 0, 0);
        asn1_range_error = TRUE;
      }
      p_ganss_sat_info->n = p_ganss_req_generic_data->ganssNavigationModelData.satellitesListRelatedDataList.n;
      p_ganss_sat_info->elem = p_ganss_sat_info_element;
      for (u_i = 0; u_i < p_ganss_sat_info->n; u_i++)
      {
        p_ganss_sat_info_element[u_i].satId =
           (p_GloAssistData->w_SvId_Iode[u_i] & 0x003F) - 1;

        p_ganss_sat_info_element[u_i].iod =
           p_GloAssistData->w_SvId_Iode[u_i] >> 8; /* 8 LSB of 10 bit IODE */

        p_ganss_sat_info_element[u_i].iod |=
           (p_GloAssistData->w_SvId_Iode[u_i] & 0x00C0) << 2; /* 2 MSB of 10 bit IODE */

        /* asn1 range check:  satId  INTEGER(0..63),  iODE   INTEGER(0..1023) */
        if ((p_ganss_sat_info_element[u_i].satId > 63) || (p_ganss_sat_info_element[u_i].iod > 1023))
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "navModelData out of asn1 range !", 0, 0, 0);
          asn1_range_error = TRUE;
        }
      } /* for */
      p_ganss_req_generic_data->ganssNavigationModelData.satellitesListRelatedDataList.n =
         p_ganss_sat_info->n;
      p_ganss_req_generic_data->ganssNavigationModelData.satellitesListRelatedDataList.elem =
         p_ganss_sat_info->elem;
    }
    else
    {
      p_ganss_req_generic_data->ganssNavigationModelData.m.satellitesListRelatedDataListPresent = 0;
      p_ganss_req_generic_data->ganssNavigationModelData.ganssWeek = 0;
      p_ganss_req_generic_data->ganssNavigationModelData.ganssToe = 0;
      p_ganss_req_generic_data->ganssNavigationModelData.t_toeLimit = 0;
      p_ganss_req_generic_data->ganssNavigationModelData.satellitesListRelatedDataList.n = 0;
      p_ganss_req_generic_data->ganssNavigationModelData.satellitesListRelatedDataList.elem = NULL;
    }
#endif /* FEATURE_CGPS_SUPL_VARIANT_NO_TARGETED_EPH */

    /* 3GPP TS 44031-820.doc Table A.49.2
       Model 4 - Satellite Navigation Model Using GLONASS Earth-Centered, Earth-fixed Parameters
    */
    p_ganss_req_generic_data->m.ganssAdditionalDataChoicesPresent = 1;
    p_ganss_req_generic_data->ganssAdditionalDataChoices.m.orbitModelIDPresent = 1;
    p_ganss_req_generic_data->ganssAdditionalDataChoices.orbitModelID = 3;
  }

  if (p_GloAssistData->u_Octet3_Whishlist_AH &  C_G0931_GPS_ASSIST_ALM)
  {
    p_ganss_req_generic_data->ganssAlmanac = TRUE;

    /* 3GPP TS 44031-820.doc Table A.54
       Model 5 - GANSS Almanac Model Using GLONASS Keplerian Parameters
    */
    p_ganss_req_generic_data->m.ganssAdditionalDataChoicesPresent = 1;
    p_ganss_req_generic_data->ganssAdditionalDataChoices.m.almanacModelIDPresent = 1;
    p_ganss_req_generic_data->ganssAdditionalDataChoices.almanacModelID = 4;
  }

  p_ganss_req_generic_data->ganssAdditionalDataChoices.extElem1.count = 0;

  if (p_GloAssistData->u_Octet3_Whishlist_AH & C_G0931_GPS_ASSIST_UTC)
  {
    p_ganss_req_generic_data->m.ganssAdditionalDataChoicesPresent = 1;
    p_ganss_req_generic_data->ganssAdditionalDataChoices.m.utcModelIDPresent = 1;
    p_ganss_req_generic_data->ganssAdditionalDataChoices.utcModelID = 1; /* from RRLP-: 1 for Model 3: Glonass */
  }

  if (p_GloAssistData->u_Octet4_Whishlist_IP & (C_G0931_GPS_ASSIST_RTI >> 8))
  {
    p_ganss_req_generic_data->ganssRealTimeIntegrity = TRUE;
  }

  /** TBD: MGP/LM/TM interface has not DGPS yet. Implement it when available */

  p_ganss_req_generic_data->ganssId = C_SUPL2_GANSS_ID_GLONASS;
  p_ganss_req_generic_data->extElem1.count = 0;

  return TRUE;
}

/*
******************************************************************************
* supl_SuplPosInit_tx
*
* Function description:
*
* Encode and transmit SUPL_POS_INIT
*
* Parameters: NONE.
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/

boolean supl_SuplPosInit_tx(void)
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;

  boolean u_ret = FALSE;

  tm_pdu_buf_type pdu_buff; /* including length and pointer to data */
  supl_ULP_PDU      pdu;
  supl_SUPLPOSINIT *p_Suplposinit;
  supl_SUPLPOSINIT  z_Suplposinit;
  supl_cell_info_struct_type  z_cell_info_allocated;

  supl_SatelliteInfo         z_sat_info;
  supl_SatelliteInfoElement  z_sat_info_element[15];
  supl_SatellitesListRelatedDataList  ganss_sat_info;
  supl_SatellitesListRelatedData      z_ganss_sat_info_element[15];

  supl_callflow_kind_enum_type  u_callflow_type = p_supl_session_status->u_callflow_kind;

  uint8 *p_length;
  uint8            u_length_msb = 0;
  uint8            u_length_lsb = 0;
  uint8            u_i;

  char          timestamp[14];
  utc_time      z_utc_time;

  tm_umts_common_pos_for_net_s_type *p_pos_suplposinit = NULL;
#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
  boolean b_ret = TRUE;
#endif /* FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY */

  supl_Velocity z_velocity;
  supl_Horandvervel z_Horandvervel;
  int encode_status;

  uint8 u_num_mlid = 0;
  supl_LocationIdData *p_LocationIdData = NULL;
  supl_LocationId     *p_LocationId = NULL;
  supl_cell_info_struct_type  *p_cell_info_allocated_mlid = NULL;

  //uint16 w_word;  //used only for formulating ganss time assistance request
  struct supl_GANSSPositionMethod    z_ganssPositionMethod[2];
  struct supl_GANSSPositionMethods   z_ganssPositionMethodsList;
  struct supl_GANSSPositionMethods   *p_ganssPositionMethodsList =
     &z_ganssPositionMethodsList;
  struct supl_GanssReqGenericData    z_ganss_req_generic_data;
  struct supl_GanssReqGenericData    *p_ganss_req_generic_data = NULL,
  *p_temp = NULL;
  uint8                              u_idx_ganss_req_generic_data = 0;
  struct supl_GanssRequestedGenericAssistanceDataList  z_ganss_requested_generic_assistance_data_list;

  boolean  asn1_range_error = FALSE;
  boolean abort_sending_supl_pos_init = FALSE;

  tm_cm_phone_state_info_s_type phone_state_info;
  supl_Ver2_PosPayLoad_extension z_lpp_payload;
  tm_location_notification_s_type location_notification = {0};
  boolean v_cell_id_present_in_pos_init = FALSE;
  boolean v_position_present_in_pos_init = FALSE;
  /***************************************************************************/

  (void)os_TimerStop(supl_tcp_stay_on_only_timer);
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Stop tcp_stay_on timer if it's on", 0, 0, 0);

  /* When ready to send SUPL_POSINIT, any timer/state waiting for WIFI MLID should be canceled */
  tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP);
  tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_SUPLPOSINIT);
  tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLPOSINIT);

  /* check to see if an active one-shot session exists; if so process with
     this one-shot session; if not, look for an areaEventTrigger session */


  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  if ((p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UNDEF) ||
      (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_NO_POSITION)
      )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SuplPosInit attempt aborted: gps_mode = %u", p_supl_session_status->u_agps_mode, 0, 0);
    return FALSE;
  }

  check_and_handle_handover(&abort_sending_supl_pos_init);

  if (abort_sending_supl_pos_init)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SuplPosInit attempt aborted: due to handover");
    return TRUE;
  }

  memset(&pdu, 0x00, sizeof(supl_ULP_PDU));
  memset(&z_cell_info_allocated, 0, sizeof(supl_cell_info_struct_type));
  memset(&z_Suplposinit, 0x00, sizeof(supl_SUPLPOSINIT));
  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));
  memset(&z_velocity, 0, sizeof(z_velocity));
  memset(&z_Horandvervel, 0, sizeof(supl_Horandvervel));
  memset(&z_lpp_payload, 0, sizeof(supl_Ver2_PosPayLoad_extension));
  memset(&z_ganss_req_generic_data, 0, sizeof(z_ganss_req_generic_data));
  memset(&z_ganssPositionMethod, 0, sizeof(z_ganssPositionMethod));
  memset(&z_ganssPositionMethodsList, 0, sizeof(z_ganssPositionMethodsList));
  memset(&z_sat_info, 0, sizeof(z_sat_info));
  memset(z_sat_info_element, 0, sizeof(z_sat_info_element));
  memset(&ganss_sat_info, 0, sizeof(ganss_sat_info));
  memset(z_ganss_sat_info_element, 0, sizeof(z_ganss_sat_info_element));



  p_Suplposinit = &z_Suplposinit;
  pdu.message.t = C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLPOSINIT;
  pdu.message.u.msSUPLPOSINIT = p_Suplposinit;


  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SuplPosInit Tx. CF type %u, PosMod %d",
        u_callflow_type, p_supl_session_status->u_agps_mode, 0);

  /* overhead build function has to be after we know what message is to be constructed */
  if (!supl_header_build(&pdu, NULL, NULL, NULL, p_supl_session_status->q_supl_version))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosInit attempt aborted: header", 0, 0, 0);
    return FALSE;
  }

  /* Construct location notification */
  location_notification.q_prtl_type = TM_PRTL_TYPE_UMTS_UP;
  location_notification.e_pdapi_client_id = p_supl_session_status->q_pdapi_client_id;
  location_notification.e_pdapi_client_type = PDSM_CLIENT_TYPE_NI;
  location_notification.e_oper_mode = lpp_up_map_agps_mode_to_tm_oper_mode(p_supl_session_status->u_agps_mode);
  location_notification.e_sub_id = p_supl_session_status->session_sub_id;
  location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_POSITION;
  location_notification.v_location_privacy_override_set = p_supl_session_status->v_privacy_override_set;
  location_notification.v_emergency_bit_set = p_supl_session_status->z_emergency_data.b_emergency_ni;
      
  /****************************************************************************
   * Set sETCapabilities.posTechnology based on UI/NI and agps_mode           *
   ****************************************************************************/
  /* Positioning technology */
  p_Suplposinit->sETCapabilities.posTechnology.aFLT = FALSE;
  p_Suplposinit->sETCapabilities.posTechnology.eOTD = FALSE;
  p_Suplposinit->sETCapabilities.posTechnology.oTDOA = FALSE;
  p_Suplposinit->sETCapabilities.posTechnology.eCID = FALSE; /* maybe modified below */

  /* posTechnology AGPS  -- if SET-initiated: capability - pdapi requested method that is
     provisioned; prefered method - NoPreference, per MSM6275 SUPL 1.0 Support Reference Guide */
  if ((u_callflow_type == C_SUPL_SESSION_UI) || (u_callflow_type == C_SUPL_SESSION_KEEP_WARM))
  {

    if (u_callflow_type == C_SUPL_SESSION_KEEP_WARM)
    {
      p_Suplposinit->sETCapabilities.posTechnology.agpsSETBased = TRUE;
    }
    else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
    {
      p_Suplposinit->sETCapabilities.posTechnology.agpsSETassisted = TRUE;
#ifdef FEATURE_LOC_LTE_OTDOA
      if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
      {
        p_Suplposinit->sETCapabilities.posTechnology.oTDOA = TRUE;
      }
#endif /* FEATURE_LOC_LTE_OTDOA */
    }
    else if (p_supl_session_status->u_agps_mode == C_SUPL_AFLT_MODE)
    {
      if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
      {
        p_Suplposinit->sETCapabilities.posTechnology.aFLT = TRUE;
      }

#ifdef FEATURE_LOC_LTE_OTDOA
      if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
      {
        p_Suplposinit->sETCapabilities.posTechnology.oTDOA = TRUE;
      }
#endif /* FEATURE_LOC_LTE_OTDOA */
    }
    else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
    {
      p_Suplposinit->sETCapabilities.posTechnology.agpsSETBased = TRUE;

      /*IS801 does not support Autonomous GPS mode*/
      if (p_supl_session_status->u_position_protocol != TM_POS_PRTL_IS801)
      {
#ifndef FEATURE_CGPS_SUPL_VARIANT_SET_CAP_NO_AUTO /* this is NOT by default defined */
        /* in case SLP wants to do auto. */
        p_Suplposinit->sETCapabilities.posTechnology.autonomousGPS = TRUE;
#endif
      }
    }
    else if (p_supl_session_status->u_agps_mode == C_SUPL_OTDOA_MODE)
    {
#ifdef FEATURE_LOC_LTE_OTDOA
      if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
      {
        p_Suplposinit->sETCapabilities.posTechnology.oTDOA = TRUE;
      }
#endif /* FEATURE_LOC_LTE_OTDOA */
    }
    else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
    {
      p_Suplposinit->sETCapabilities.posTechnology.agpsSETassisted = FALSE;
      p_Suplposinit->sETCapabilities.posTechnology.autonomousGPS = FALSE;
      p_Suplposinit->sETCapabilities.posTechnology.agpsSETBased = FALSE;
    }

    else
    {
      /* no other agps mode should generate SUPL_POS_INIT */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosInit attempt aborted", 0, 0, 0);

      /* Send location notification for failure */
      if (u_callflow_type == C_SUPL_SESSION_NI)
      {
        location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
        tm_construct_location_notification_ind(location_notification); 
      }
      
      return FALSE;
    }

#ifdef FEATURE_CGPS_SUPL_VARIANT_SET_CAP_PREF_SET_BASED /* NOT defined by default */
    #error code not present
#else
    p_Suplposinit->sETCapabilities.prefMethod = supl_PrefMethod_noPreference;
#endif


    /* positioning protocol */
    p_Suplposinit->sETCapabilities.posProtocol.rrc = FALSE;
    if (TM_CORE_CONFIG_DD(gnss_1x_up_supl_enable) != 0)
    {
      p_Suplposinit->sETCapabilities.posProtocol.tia801 = TRUE;
    }
    else
    {
      p_Suplposinit->sETCapabilities.posProtocol.tia801 = FALSE;
    }
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      p_Suplposinit->sETCapabilities.posProtocol.m.ver2_PosProtocol_extensionPresent = 1;
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.lpp = TRUE;
    }
    else
    {
      p_Suplposinit->sETCapabilities.posProtocol.rrlp = TRUE;
    }
    /*Uncheck OTDOA if SRV_SYS is WLAN and PRTL is LPP*/
#ifdef FEATURE_LOC_LTE_OTDOA
    if ( ( TM_POS_PRTL_LPP == p_supl_session_status->u_position_protocol ) &&
         ( ( CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys() ) ||
           ( CGPS_SRV_SYS_WLAN == phone_state_info.emerg_conn_srv_system ) )
       )
    {
      p_Suplposinit->sETCapabilities.posTechnology.oTDOA = FALSE;
    }
#endif /* FEATURE_LOC_LTE_OTDOA */

  }
  else /*posTechnology AGPS -- network-initiated*/
  {
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
    {
        /* posTechnology AGPS -- network-initiated: all method regardless
           what SUPL_INIT says, with PreferedMethod set to NoPref, per MSM6275 SUPL 1.0
           Support Reference Guide */
        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED))
        {
          p_Suplposinit->sETCapabilities.posTechnology.agpsSETassisted = TRUE;
        }

        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED))
        {
          p_Suplposinit->sETCapabilities.posTechnology.agpsSETBased = TRUE;
        }

        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_STANDALONE))
        {
          p_Suplposinit->sETCapabilities.posTechnology.autonomousGPS = TRUE;
        }
      }
    else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
        /* posTechnology AGPS -- network-initiated: all method regardless
           what SUPL_INIT says, with PreferedMethod set to NoPref, per MSM6275 SUPL 1.0
           Support Reference Guide */
        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_ASSISTED))
        {
          p_Suplposinit->sETCapabilities.posTechnology.agpsSETassisted = TRUE;
          #ifdef FEATURE_LOC_LTE_OTDOA
          /*Uncheck OTDOA if SRV_SYS is WLAN and PRTL is LPP*/
          if ( ( CGPS_SRV_SYS_WLAN == phone_state_info.emerg_conn_srv_system ) ||
               ( CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys() )
             ) 
          {
            p_Suplposinit->sETCapabilities.posTechnology.oTDOA = FALSE;
          }
        else if ( phone_state_info.srv_system == CGPS_SRV_SYS_LTE )
          {
            p_Suplposinit->sETCapabilities.posTechnology.oTDOA = TRUE;
          }
#endif /* FEATURE_LOC_LTE_OTDOA */
        }
        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_BASED))
        {
          p_Suplposinit->sETCapabilities.posTechnology.agpsSETBased = TRUE;
        }

        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_STANDALONE))
        {
          p_Suplposinit->sETCapabilities.posTechnology.autonomousGPS = TRUE;
        }
    } /*RRLP or LPP as pos protocol*/
    else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801) /*IS801 as pos protocol*/
    {
      if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
      {
        p_Suplposinit->sETCapabilities.posTechnology.agpsSETassisted = TRUE;
      }
      else if (p_supl_session_status->u_agps_mode == C_SUPL_AFLT_MODE)
      {
        p_Suplposinit->sETCapabilities.posTechnology.aFLT = TRUE;
      }
      else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
      {
        p_Suplposinit->sETCapabilities.posTechnology.agpsSETBased = TRUE;
      }
      else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
      {
        /*For ECID, no capabilities should be shown in SUPL_INIT*/
      }
      else
      {
        /* no other agps mode should generate SUPL_POS_INIT */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosInit attempt aborted", 0, 0, 0);

        /* Send location notification for failure */
        if (u_callflow_type == C_SUPL_SESSION_NI)
        {
          location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
          tm_construct_location_notification_ind(location_notification); 
        }
        
        return FALSE;
      }
    } /*IS801 as pos protocol*/
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Pos Protocol not supported", 0, 0, 0);
    }

    p_Suplposinit->sETCapabilities.prefMethod = supl_PrefMethod_noPreference;

    /* positioning protocol */
    p_Suplposinit->sETCapabilities.posProtocol.rrc = FALSE;
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
    {
      p_Suplposinit->sETCapabilities.posProtocol.tia801 = TRUE;
    }
    else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      uint8 u_maj, u_min, u_servind, q_ConfigInfo;
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.lpp = TRUE;
#ifdef FEATURE_GNSS_LPPE
      if (TRUE == supl_get_supl_version(&u_maj, &u_min, &u_servind))
      {
        if (u_maj == 2 && u_min == 0 && u_servind >= 2)
        {
          if (tm_core_lppe_up_precondition() && tm_core_lppe_up_enabled())
          {
            /* if supl version is 2.0.2 and LPPe is enabled in NV, set lppe = true */
            p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.lppe = TRUE;
          }
        }
      }
#endif
    }
    else
    {
      p_Suplposinit->sETCapabilities.posProtocol.rrlp = TRUE;
    }
  }
  /****************************************************************************
   * sETCapabilities.posTechnology set                                        *
   ****************************************************************************/

  /**** SUPL-2 only: Service Capabilities ***/

  if ((p_supl_session_status->q_supl_version & 0x000000FF) == 2)
  {
    /***********  Service Capabilities optional SUPL2 extension ********/

#if 0 /* For further dev. */

    p_Suplposinit->sETCapabilities.m.ver2_SETCapabilities_extensionPresent = 1;

    p_Suplposinit->sETCapabilities.ver2_SETCapabilities_extension.m.serviceCapabilitiesPresent = 1;

    /* neither reportingCapabilities or eventTriggerCapabilities is present */

    /* neither periodic trigger or area event trigger is supported */

#endif

    /***********  PosProtocolVersionRRLP SUPL2 extension in PosProtocol ****/

    p_Suplposinit->sETCapabilities.posProtocol.m.ver2_PosProtocol_extensionPresent = 1;

    if (p_Suplposinit->sETCapabilities.posProtocol.rrlp == TRUE)
    {
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.m.posProtocolVersionRRLPPresent = 1;

      /* default to the backward compatible version first, may be updated below  
       * based on our GANSS configuration */
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.majorVersionField =
         SUPL_RRLP_CURRENT_VERSION_MAJOR;
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.technicalVersionField =
         SUPL_RRLP_CURRENT_VERSION_TECH;
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.editorialVersionField =
         SUPL_RRLP_CURRENT_VERSION_EDIT;
    }

    if (p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.lpp == TRUE)
    {
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.m.posProtocolVersionLPPPresent = 1;
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.majorVersionField = LPP_CURRENT_VERSION_MAJOR;
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.technicalVersionField = LPP_CURRENT_VERSION_TECH;
      p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.editorialVersionField = LPP_CURRENT_VERSION_EDIT;
#ifdef FEATURE_GNSS_LPPE
      if (p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.lppe == TRUE)
      {

        p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.m.lppePresent = 1;
        p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPPe.majorVersionField = LPPE_CURRENT_VERSION_MAJOR;
        p_Suplposinit->sETCapabilities.posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPPe.minorVersionField = LPPE_CURRENT_VERSION_TECH;
      }
#endif
    }

    /****** ver2_PosTechnology SUPL2 extension in posTechnology **********/

    // Glonass enabled and Protocol is not IS801
    if ((tm_umts_common_bds_enabled() || tm_umts_common_glonass_enabled()) &&
        (p_supl_session_status->u_position_protocol != TM_POS_PRTL_IS801))
    {
      supl_fill_ver2_ganss_set_capability(&(p_Suplposinit->sETCapabilities), z_ganssPositionMethod,
                                          p_ganssPositionMethodsList, u_callflow_type);
    } /* Glonass */
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Glonass not enabled.");
    }
  }

  if (!supl_cell_information_construct(&p_Suplposinit->locationId.cellInfo,
                                       &z_cell_info_allocated))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "No Cell ID info.", 0, 0, 0);

    /* Send location notification for failure */
    if (u_callflow_type == C_SUPL_SESSION_NI)
    {
      location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
      tm_construct_location_notification_ind(location_notification); 
    }
    
    return FALSE;
  }
  else
  {
    /* Cell id populated */
    v_cell_id_present_in_pos_init = TRUE;
  }

  /* MLID for SUPL_POS_INIT */
  if ((p_supl_session_status->q_supl_version & 0x000000FF) >= 2)
  {
    if (u_supl_global_num_mlid > 1) /* note being 1 means only the current serving cell is in the structure */
    {
      p_LocationIdData = (supl_LocationIdData *)os_MemAlloc((sizeof(supl_LocationIdData) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);
      p_LocationId = (supl_LocationId *)os_MemAlloc((sizeof(supl_LocationId) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);
      p_cell_info_allocated_mlid = (supl_cell_info_struct_type *)os_MemAlloc((sizeof(supl_cell_info_struct_type) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);

      if ((p_LocationIdData == NULL) || (p_LocationId == NULL) || (p_cell_info_allocated_mlid == NULL))
      {
        u_num_mlid = 0;
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "mlid mem. allocation failed, set num_mlid to %u ", u_num_mlid);
      }
      else
      {
        memset(p_LocationIdData, 0, sizeof(supl_LocationIdData) * (u_supl_global_num_mlid - 1));
        memset(p_LocationId, 0, sizeof(supl_LocationId) * (u_supl_global_num_mlid - 1));
        memset(p_cell_info_allocated_mlid, 0, sizeof(supl_cell_info_struct_type) * (u_supl_global_num_mlid - 1));

        p_LocationIdData->locationId = p_LocationId;
        u_num_mlid = supl_chk_and_build_all_mlid(p_LocationIdData, p_LocationId,
                                                 p_cell_info_allocated_mlid, (u_supl_global_num_mlid - 1));
      }
    }
    if (u_num_mlid > 0) /* valid mlid exists */
    {
      if (p_LocationIdData != NULL)
      {
        p_Suplposinit->m.ver2_SUPL_POS_INIT_extensionPresent = TRUE;
        p_Suplposinit->ver2_SUPL_POS_INIT_extension.m.multipleLocationIdsPresent = TRUE;
        p_Suplposinit->ver2_SUPL_POS_INIT_extension.multipleLocationIds.n = u_num_mlid;
        p_Suplposinit->ver2_SUPL_POS_INIT_extension.multipleLocationIds.elem = p_LocationIdData;
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "p_LocationIdData not allocated: %lx ", p_LocationIdData);
      }
    }
  } /* running supl ver. >= 2 */

  if ((phone_state_info.srv_system == CGPS_SRV_SYS_WCDMA) &&
      z_cell_info_cached.u_CachedCellIDValid &&
      (z_cell_info_cached.e_ActiveRAT == CELLDB_ACTIVERAT_WCDMA)
      )
  {
#ifdef FEATURE_USER_PLANE_MRL
    p_Suplposinit->sETCapabilities.posTechnology.eCID = TRUE;
#endif

#ifdef FEATURE_CGPS_SUPL_VARIANT_NO_ECID /* NOT defined by default */
    #error code not present
#endif

    if (z_cell_info_cached.q_NumCellChangesCachedCellID > 1)
    {
      /* when there is no cell change, this number is 1 */
      q_handovers_since_last_cell_id =
         z_cell_info_cached.q_NumCellChangesCachedCellID - 1;
    }
    else
    {
      /* current, or no info. available assume current */
      q_handovers_since_last_cell_id = 0;
    }
  } /* in WCDMA with valid cell info. */
  else if ((phone_state_info.srv_system == CGPS_SRV_SYS_GSM) &&
           (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_NMR_SUPPORTED) != 0))
  {
    p_Suplposinit->sETCapabilities.posTechnology.eCID = TRUE;

#ifdef FEATURE_CGPS_SUPL_VARIANT_NO_ECID /* NOT defined by default */
    #error code not present
#endif
  }

#ifdef FEATURE_CGPS_LTE_CELLDB
  else if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) &&
           z_cell_info_cached.u_CachedCellIDValid  &&
           (z_cell_info_cached.e_ActiveRAT == CELLDB_ACTIVERAT_LTE) &&
           ((p_supl_session_status->q_supl_version & 0x000000FF) == 2)
           )
  {
    /* if SUPL2, and supl_cell_information_construct() returns TRUE */

    if ((p_Suplposinit->locationId.cellInfo.u.ver2_CellInfo_extension->u.lteCell->m.rsrpResultPresent ||
         p_Suplposinit->locationId.cellInfo.u.ver2_CellInfo_extension->u.lteCell->m.rsrqResultPresent ||
         p_Suplposinit->locationId.cellInfo.u.ver2_CellInfo_extension->u.lteCell->m.tAPresent
         ) ||
        (z_lte_mrl.num_cells > 1)
        )
    {
      p_Suplposinit->sETCapabilities.posTechnology.eCID = TRUE;
      p_supl_session_status->u_supl_posMethod_ecid_valid = TRUE;
    }
  }

#endif


  /* set this to CURRENT if cell id is of the current cell; otherwise set
     it to STATE to prevent the network from using this cell id to generate
     cell-id based positions for NI */
  if (q_handovers_since_last_cell_id == 0)
  {
    p_Suplposinit->locationId.status = supl_Status_current;
  }
  else
  {
    if (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI)
    {
      p_Suplposinit->locationId.status = supl_Status_stale;
    }
    else
    {
      /* for SI, regardless the actual status, always use "current" and the SET
         will derate the reference position provided by the network */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Stale cell_id, but report current for MO", 0, 0, 0);
      p_Suplposinit->locationId.status = supl_Status_current;
    }
  }

  /****************************************************************************/
  /*Request Assistance Data only if RRLP is selected*/
  /****************************************************************************/
  if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
  {
    /* AD Request. If positioning mode is currently undefined, do not request any
       assistance data at this time */
    if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
    {
      /************************************************************************
       *  Build RRLP Aglo UE-Based AD req                                     *
       ************************************************************************/
      if (FALSE == p_supl_session_status->u_slp_support_gps)
      {
        /* Don't request GPS AD if H-SLP indicated no GPS support */
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "No GPS AD requested. H-SLP not capable of GPS");
      }
      else
      {
        if (supl_SuplPosInit_RRLP_UEBased_GPSADReq_build(
              &p_supl_session_status->z_GpsAssistData,
              &p_Suplposinit->requestedAssistData,
              &z_sat_info, z_sat_info_element))
        {
          p_Suplposinit->m.requestedAssistDataPresent = 1;
        }
      }

      /************************************************************************
       *  Build RRLP Aglo UE-Based AD req                                     *
       *  note that we do not check if the server supports AGLO. This is by   *
       *  design to allow servers to support legacy buggy devices.            *
       *  !!!!!!! Do not add this check !!!!!!                                *
       ************************************************************************/
      if (tm_umts_up_supl_rrlp_assisted_glonass_enabled())
      {
        /* Don't request GLO AD if UE's Glonass capability is disabled or H-SLP 
         * indicated no GLONASS support in UI call flow */
        if (FALSE == tm_up_slp_support_glonass(p_supl_session_status))
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "No GLO AD requested. H-SLP not capable of glonass");
        }
        else
        {
          p_temp = (supl_GanssReqGenericData *)
             os_MemReAlloc(
             (void **)&p_ganss_req_generic_data,
             sizeof(*p_ganss_req_generic_data) * (u_idx_ganss_req_generic_data + 1),
             OS_MEM_SCOPE_TASK);

          if (p_temp != NULL)
          {
            p_ganss_req_generic_data = p_temp;
            memset(p_ganss_req_generic_data, 0, sizeof(*p_ganss_req_generic_data));

            if (supl_SuplPosInit_RRLP_UEBased_GloADReq_build(
                  &p_supl_session_status->z_GloAssistData,
                  p_ganss_req_generic_data,
                  &p_Suplposinit->requestedAssistData,
                  &ganss_sat_info,
                  z_ganss_sat_info_element,
                  &p_supl_session_status->z_OrigWishList.z_GloAssistData.q_WishListMask))
            {
              p_Suplposinit->requestedAssistData.m.ver2_RequestedAssistData_extensionPresent = 1;
              p_Suplposinit->requestedAssistData.ver2_RequestedAssistData_extension.m.ganssRequestedGenericAssistanceDataListPresent = TRUE;
              u_idx_ganss_req_generic_data++;
            }
          }
        }
      }
#ifdef FEATURE_ABDS_RRLP_SUPL
      if (tm_umts_up_supl_rrlp_assisted_bds_enabled())
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
              "BDS Enabled!! slp supports BDS %d", p_supl_session_status->u_slp_support_bds);
        /* Don't request BDS AD if UE's BDS capability is disabled or H-SLP 
         * indicated no BDS support in UI call flow */
        if (FALSE == p_supl_session_status->u_slp_support_bds)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "No BDS AD requested. H-SLP not capable of BDS");
        }
        else
        {
          p_temp = (supl_GanssReqGenericData *)
             os_MemReAlloc(
             (void **)&p_ganss_req_generic_data,
             sizeof(*p_ganss_req_generic_data) * (u_idx_ganss_req_generic_data + 1),
             OS_MEM_SCOPE_TASK);

          if (p_temp != NULL)
          {
            p_ganss_req_generic_data = p_temp;
            memset(&p_ganss_req_generic_data[u_idx_ganss_req_generic_data],
                   0, sizeof(*p_ganss_req_generic_data));
            if (supl_SuplPosInit_RRLP_UEBased_BdsADReq_build(
                  &p_supl_session_status->z_BdsAssistData,
                  &p_ganss_req_generic_data[u_idx_ganss_req_generic_data],
                  &p_Suplposinit->requestedAssistData))
            {
              p_Suplposinit->requestedAssistData.m.ver2_RequestedAssistData_extensionPresent = 1;
              p_Suplposinit->requestedAssistData.ver2_RequestedAssistData_extension.m.ganssRequestedGenericAssistanceDataListPresent = TRUE;
              u_idx_ganss_req_generic_data++;
            }
          }
        }
      }
#endif

      if (p_Suplposinit->requestedAssistData.ver2_RequestedAssistData_extension.m.ganssRequestedGenericAssistanceDataListPresent)
      {
        p_Suplposinit->m.requestedAssistDataPresent = 1;
        z_ganss_requested_generic_assistance_data_list.elem = p_ganss_req_generic_data;
        z_ganss_requested_generic_assistance_data_list.n = u_idx_ganss_req_generic_data;

        p_Suplposinit->requestedAssistData.ver2_RequestedAssistData_extension.ganssRequestedGenericAssistanceDataList =
           z_ganss_requested_generic_assistance_data_list;

      }
    } /* closing if SET-Based */

    else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
    {
      p_Suplposinit->m.requestedAssistDataPresent = 1;
      p_Suplposinit->requestedAssistData.referenceTimeRequested = TRUE;
      p_Suplposinit->requestedAssistData.acquisitionAssistanceRequested = TRUE;

      if (tm_umts_up_supl_rrlp_assisted_glonass_enabled() && (p_supl_session_status->u_position_protocol != TM_POS_PRTL_IS801))
      {
        // Don't request GLO AD if UE's Glonass capability is disabled or H-SLP indicated no GLONASS support in UI call flow
        if (FALSE == tm_up_slp_support_glonass(p_supl_session_status))
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "No GLO AD requested. H-SLP not capable of glonass");
        }
        else
        {
          p_Suplposinit->requestedAssistData.m.ver2_RequestedAssistData_extensionPresent = 1;
          z_ganss_req_generic_data.ganssId = C_SUPL2_GANSS_ID_GLONASS; /* 4: Glonass */
          z_ganss_req_generic_data.extElem1.count = 0;

          /* if AA model is request, also request aux. which has freqency info. */
          z_ganss_req_generic_data.ganssAuxiliaryInformation = TRUE;

          /* UTC model for converting GPS reference time to GLONASS time */
          p_Suplposinit->requestedAssistData.utcModelRequested = TRUE;

          /* GANSS Time Model better quality conversion of GPS reference time
             to GLONASS time; reference system is C_SUPL2_GANSS_ID_GLONASS,
             wanting to convert to GNSS system of GPS */
          z_ganss_req_generic_data.m.ganssTimeModelsPresent = TRUE;
          z_ganss_req_generic_data.ganssTimeModels.data[0] = C_GANSS_SYSTEM_TIME_MODEL_REQUESTED_GPS;  /* GPS */
          z_ganss_req_generic_data.ganssTimeModels.data[1] = 0x0;
          z_ganss_req_generic_data.ganssTimeModels.numbits = 16;

          z_ganss_req_generic_data.ganssReferenceMeasurementInfo = TRUE;

          z_ganss_requested_generic_assistance_data_list.elem = &z_ganss_req_generic_data;
          z_ganss_requested_generic_assistance_data_list.n = 1;

          p_Suplposinit->requestedAssistData.ver2_RequestedAssistData_extension.ganssRequestedGenericAssistanceDataList =
             z_ganss_requested_generic_assistance_data_list;

          p_Suplposinit->requestedAssistData.ver2_RequestedAssistData_extension.m.ganssRequestedGenericAssistanceDataListPresent = TRUE;
        } /* H-SLP support GLO */
      }
    }
    else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_AUTO)
    {
      /* ayt - leave it commented out */
#if 0 /* QQQ: test server would generate error if SuplPosInit contains no request
               for assistance. This server can choose to ignore this request for reference position;

               When this test only code is removed, for now only test with ULTS
            */
      p_Suplposinit->m.requestedAssistDataPresent = 1;
      p_Suplposinit->requestedAssistData.referenceLocationRequested = TRUE;
#endif
    }
    else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
    {
      /* space keeper: no assistance needed for cell ID */
    }
    else
    {
      /* shouldn't get here */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SuplPosInit attempt aborted: gps_mode = %u", p_supl_session_status->u_agps_mode, 0, 0);
      
      if (p_cell_info_allocated_mlid != NULL)
      {
        os_MemFree((void **)&p_cell_info_allocated_mlid);
        p_cell_info_allocated_mlid = NULL;
      }
      if (p_LocationId != NULL)
      {
        os_MemFree((void **)&p_LocationId);
        p_LocationId = NULL;
      }
      if (p_LocationIdData != NULL)
      {
        p_LocationIdData->locationId = NULL;
        os_MemFree((void **)&p_LocationIdData);
        p_LocationIdData = NULL;
      }
      if (p_ganss_req_generic_data != NULL)
      {
        os_MemFree((void **)&p_ganss_req_generic_data);
        p_ganss_req_generic_data = NULL;
      }

      /* Send location notification for failure */
      if (u_callflow_type == C_SUPL_SESSION_NI)
      {
        location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
        tm_construct_location_notification_ind(location_notification); 
      }
      
      return FALSE;
    }
  } /*RRLP is pos protocol*/


  if ((p_supl_session_status->z_pending_new_pos_to_network.u_valid == TRUE) ||
      (z_supl_cached_pos_for_net.u_valid == TRUE)
      )
  {

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "new/cached pos. in SUPL_POS_INIT: %u, %u",
          p_supl_session_status->z_pending_new_pos_to_network.u_valid,
          z_supl_cached_pos_for_net.u_valid, 0);

    if (p_supl_session_status->z_pending_new_pos_to_network.u_valid == TRUE)
    {
      p_pos_suplposinit = &p_supl_session_status->z_pending_new_pos_to_network;
    } /* new GPS position obtained prior to data link is up */

    else
    {
      /* If no new GPS, but there is a cached position, report this position
         in SUPL_POS_INIT */
      p_pos_suplposinit = &z_supl_cached_pos_for_net;
    } /* Cached position available */

    supl_gad9_to_pos_est(&p_pos_suplposinit->z_pos_info.z_pos,
                         &p_Suplposinit->position.positionEstimate);


    /* asn1 range check:

        PositionEstimate ::= SEQUENCE {
          latitudeSign  ENUMERATED {north, south},
          latitude      INTEGER(0..8388607),
          longitude     INTEGER(-8388608..8388607),
          uncertainty
            SEQUENCE {uncertaintySemiMajor  INTEGER(0..127),
                      uncertaintySemiMinor  INTEGER(0..127),
                      orientationMajorAxis  INTEGER(0..180)} OPTIONAL,
          -- angle in degree between major axis and North
          confidence    INTEGER(0..100) OPTIONAL,
          altitudeInfo  AltitudeInfo OPTIONAL,
          ...}

        AltitudeInfo ::= SEQUENCE {
          altitudeDirection  ENUMERATED {height, depth},
          altitude           INTEGER(0..32767),
          altUncertainty     INTEGER(0..127),
          ...} -- based on 3GPP TS 23.032

        Note that supl_gad9_to_pos_est() sets all
          supl_uncertainty_present | supl_confidence_present | supl_altitudeInfo_present

     */

    asn1_range_error = FALSE;

    if ((p_Suplposinit->position.positionEstimate.latitude > 8388607) ||
        (p_Suplposinit->position.positionEstimate.longitude < -8388608) ||
        (p_Suplposinit->position.positionEstimate.longitude > 8388607) ||
        (p_Suplposinit->position.positionEstimate.uncertainty.uncertaintySemiMajor > 127) ||
        (p_Suplposinit->position.positionEstimate.uncertainty.uncertaintySemiMinor > 127) ||
        (p_Suplposinit->position.positionEstimate.uncertainty.orientationMajorAxis > 180) ||
        (p_Suplposinit->position.positionEstimate.confidence > 100) ||
        (p_Suplposinit->position.positionEstimate.altitudeInfo.altitude > 32767) ||
        (p_Suplposinit->position.positionEstimate.altitudeInfo.altUncertainty > 127)
        )
    {
      asn1_range_error = TRUE;
    }

    if (!asn1_range_error)
    {
      p_Suplposinit->m.positionPresent = 1;

      /***** process velocity info. *************/
      if (p_pos_suplposinit->z_pos_info.vel_valid)
      {

        z_velocity.u.horandvervel = &z_Horandvervel;

        p_Suplposinit->position.m.velocityPresent = 1;
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Including velocity in SUPL_POS_INIT", 0, 0, 0);

        p_Suplposinit->position.velocity.t = C_SUPL_ASN1_CHOICE_VELCITY_HORANDVERVEL;
        p_Suplposinit->position.velocity = z_velocity;

        supl_convert_velocity(p_pos_suplposinit->z_pos_info.f_VelEnu[0],
                              p_pos_suplposinit->z_pos_info.f_VelEnu[1],
                              p_pos_suplposinit->z_pos_info.f_VelEnu[2],
                              &z_velocity);
        p_Suplposinit->position.velocity = z_velocity;

      }  /* velocity available */

      memset(timestamp, 0, 14);
      z_utc_time =
         tm_util_construct_utc_time(p_pos_suplposinit->z_pos_info.w_GpsWeek,
                                    p_pos_suplposinit->z_pos_info.q_fix_time_gps_msec,
                                    p_pos_suplposinit->z_pos_info.q_fix_time_utc_msec);


      /* 2 digit year */
      z_utc_time.q_year = z_utc_time.q_year % 100;

      /* 10th digit year converted to ASCII */
      timestamp[0] = z_utc_time.q_year / 10 + 0x30;

      /* 1th digit year to ASCII */
      timestamp[1] = z_utc_time.q_year -
         ((uint32)(z_utc_time.q_year / 10) * 10) + 0x30;

      /* 10th digit month to ASCII */
      timestamp[2] = z_utc_time.q_month / 10 + 0x30;

      /* 1th digit month to ASCII */
      timestamp[3] = z_utc_time.q_month -
         ((uint32)(z_utc_time.q_month / 10) * 10) + 0x30;

      /* 10th digit day to ASCII */
      timestamp[4] = z_utc_time.q_utc_days / 10 + 0x30;

      /* 1th digit day to ASCII */
      timestamp[5] = z_utc_time.q_utc_days -
         ((uint32)(z_utc_time.q_utc_days / 10) * 10) + 0x30;

      /* 10th digit hour to ASCII */
      timestamp[6] = z_utc_time.q_hours / 10 + 0x30;

      /* 1th digit hour to ASCII */
      timestamp[7] = z_utc_time.q_hours -
         ((uint32)(z_utc_time.q_hours / 10) * 10) + 0x30;

      /* 10th digit minute to ASCII */
      timestamp[8] = z_utc_time.q_mins / 10 + 0x30;

      /* 1th digit minute to ASCII */
      timestamp[9] = z_utc_time.q_mins -
         ((uint32)(z_utc_time.q_mins / 10) * 10) + 0x30;

      /* 10th digit second to ASCII */
      timestamp[10] = z_utc_time.q_secs / 10 + 0x30;

      /* 1th digit second to ASCII */
      timestamp[11] = z_utc_time.q_secs -
         ((uint32)(z_utc_time.q_secs / 10) * 10) + 0x30;

      timestamp[12] = 'Z';

      p_Suplposinit->position.timestamp = &timestamp[0];
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "timestamp in SUPL_POS_INIT %s", timestamp, 0, 0);

    } /* if no asn1 range error */
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Saved new pos. has out of ASN1 range elememt, ignored", 0, 0, 0);
    }

    v_position_present_in_pos_init = TRUE;

  } /* if either a new position has been obtained in this session,
       or a cached pos. available */
#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
  if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
  {
    /* encode the SUPL POS within SUPLPOSINIT message with LPP payload*/
    b_ret = tm_lpp_up_enc_lpp_suplpos_suplposinit(p_supl_session_status, p_Suplposinit, &z_lpp_payload);

    if (b_ret == TRUE)
    {
      p_Suplposinit->m.sUPLPOSPresent = 1;
      p_Suplposinit->sUPLPOS.m.velocityPresent = 0;
    }
    else
    {
      /* todo = if encoding fails cannot send supl pos init*/

      /* Send location notification for failure */
      if (u_callflow_type == C_SUPL_SESSION_NI)
      {
        location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
        tm_construct_location_notification_ind(location_notification); 
      }
      
      
      if (p_cell_info_allocated_mlid != NULL)
      {
         os_MemFree((void **)&p_cell_info_allocated_mlid);
         p_cell_info_allocated_mlid = NULL;
      }
      if (p_LocationId != NULL)
      {
        os_MemFree((void **)&p_LocationId);
        p_LocationId = NULL;
      }
      if (p_LocationIdData != NULL)
      {
          p_LocationIdData->locationId = NULL;
        os_MemFree((void **)&p_LocationIdData);
        p_LocationIdData = NULL;
      }
      if (p_ganss_req_generic_data != NULL)
      {
        os_MemFree((void **)&p_ganss_req_generic_data);
        p_ganss_req_generic_data = NULL;
      }
      
      return u_ret;
    }
  }
#endif /* FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY */

  if ((u_callflow_type == C_SUPL_SESSION_NI) &&
      p_supl_session_status->u_hash_valid
      )
  {
    p_Suplposinit->m.verPresent = 1;
    p_Suplposinit->ver.numbits = 64;
    memscpy(&p_Suplposinit->ver.data[0], sizeof(p_Suplposinit->ver.data),
            &p_supl_session_status->supl_init_hash[0], sizeof(p_supl_session_status->supl_init_hash));
  }


  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, PROTO_SUPL);

  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL)
    {
      if ((p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP) &&
          (p_Suplposinit->m.sUPLPOSPresent == 1))
      {
#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
        if (p_Suplposinit->sUPLPOS.posPayLoad.u.ver2_PosPayLoad_extension)
        {
          tm_lpp_asn1_osys_lpppayload_free(&p_Suplposinit->sUPLPOS.posPayLoad.u.ver2_PosPayLoad_extension->lPPPayload);
        }
#endif /* FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY */
      }
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }

    if (p_cell_info_allocated_mlid != NULL)
    {
      os_MemFree((void **)&p_cell_info_allocated_mlid);
      p_cell_info_allocated_mlid = NULL;
    }
    if (p_LocationId != NULL)
    {
      os_MemFree((void **)&p_LocationId);
      p_LocationId = NULL;
    }
    if (p_LocationIdData != NULL)
    {
      p_LocationIdData->locationId = NULL;
      os_MemFree((void **)&p_LocationIdData);
      p_LocationIdData = NULL;
    }
    if (p_ganss_req_generic_data != NULL)
    {
      os_MemFree((void **)&p_ganss_req_generic_data);
    }

    /* Send location notification for failure */
    if (u_callflow_type == C_SUPL_SESSION_NI)
    {
      location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
      tm_construct_location_notification_ind(location_notification); 
    }
      
    return FALSE;
  }

  else
  {
    pd_comms_return_type ret_val;
    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (pdu_buff.length >> 8) & 0x00FF;
    u_length_lsb = pdu_buff.length & 0x00FF;

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, pdu_buff.length);

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosInit attempt aborted", 0, 0, 0);
    }
    else
    {
      supl_set_cf_state(C_SUPL_CF_STATE_UT2, 0);
      u_ret = TRUE;
    }

    if ((p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP) &&
        (p_Suplposinit->m.sUPLPOSPresent == 1))
    {
#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
      if (p_Suplposinit->sUPLPOS.posPayLoad.u.ver2_PosPayLoad_extension)
      {
        tm_lpp_asn1_osys_lpppayload_free(&p_Suplposinit->sUPLPOS.posPayLoad.u.ver2_PosPayLoad_extension->lPPPayload);
      }
#endif /* FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY */
    }
    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
    pdu_buff.value = NULL;

  }


  if (u_ret)
  {
    p_supl_session_status->u_send_supl_end_at_fail = TRUE;
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: SuplPosInit sent ", 0, 0, 0);

    /* Send location notification for SUPL POS INIT */
    if (u_callflow_type == C_SUPL_SESSION_NI)
    {
      if (TRUE == v_cell_id_present_in_pos_init)
      {
        location_notification.e_location_info_type = PDSM_PD_LOC_INFO_ECID;
        location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_LOC_PROVIDED;
        tm_construct_location_notification_ind(location_notification); 
      }
      if (TRUE == v_position_present_in_pos_init)
      {
        location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_POSITION;
        location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_LOC_PROVIDED;
        tm_construct_location_notification_ind(location_notification); 
      }
    }    
  }
  else
  {
    if (u_callflow_type == C_SUPL_SESSION_NI)
    {
      location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
      tm_construct_location_notification_ind(location_notification); 
    }
  }

  /* after SUPL_POS_INIT, no need to acquire cell meas. for MRL anymore */
  supl_stop_l1_mrl_updates();

  if (p_cell_info_allocated_mlid != NULL)
  {
    os_MemFree((void **)&p_cell_info_allocated_mlid);
    p_cell_info_allocated_mlid = NULL;
  }
  if (p_LocationId != NULL)
  {
    os_MemFree((void **)&p_LocationId);
    p_LocationId = NULL;
  }
  if (p_LocationIdData != NULL)
  {
    p_LocationIdData->locationId = NULL;
    os_MemFree((void **)&p_LocationIdData);
    p_LocationIdData = NULL;
  }
  if (p_ganss_req_generic_data != NULL)
  {
    os_MemFree((void **)&p_ganss_req_generic_data);
  }

  return u_ret;

}

/*
******************************************************************************
* supl_SuplPos_proc
*
* Function description:
*
* Process incoming SUPL POS message
*
* Parameters: p_pdu_decoded: pointer to a structure holding decoded Supl PDU.
*
* Return value: None
*
******************************************************************************
*/
static boolean supl_SuplPos_proc(const supl_ULP_PDU *p_pdu_decoded)
{
  uint8 *p_payload = NULL;
  uint16 payload_length = 0;
  const supl_SUPLPOS *p_suplPos;
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;
  boolean u_ret = TRUE;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LSM: process_supl_pos()", 0, 0, 0);

  p_suplPos = p_pdu_decoded->message.u.msSUPLPOS;


  if (  /*If not in UT2 (used only in RRLP case), UT3_RRLP or UT3 Ack*/
      (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT2) &&
       (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
       ) &&
      !(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_RRLP) &&
      !(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_ACK)
      )
  {
    (void)tm_umts_up_supl_supl_end_tx(&p_supl_session_status->z_session_id.z_Slp_Sess_Id,
                                      NULL,
                                      supl_StatusCode_unexpectedMessage,
                                      NULL,
                                      p_supl_session_status->q_supl_version);
    tm_umts_up_supl_callflow_disrupt_handler();
    return FALSE;
  }

  if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT2)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT2);
  }
  else if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_RRLP)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT3_RRLP);
  }
  else if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_ACK)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT3_ACK);
  }


  switch (p_suplPos->posPayLoad.t)
  {
    case C_SUPL_ASN1_CHOICE_RRLP_PAYLOAD:
      if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
      {
        p_payload = &p_suplPos->posPayLoad.u.rrlpPayload->data[0];
        payload_length = p_suplPos->posPayLoad.u.rrlpPayload->numocts;
        u_ret = supl_SuplPos_rrlp_proc(p_supl_session_status,
                                       p_payload, payload_length);
      }
      break;

    case C_SUPL_ASN1_CHOICE_TIA801_PAYLOAD:
      if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
      {
        p_payload = &p_suplPos->posPayLoad.u.tia801payload->data[0];
        payload_length = p_suplPos->posPayLoad.u.tia801payload->numocts;
        u_ret = supl_SuplPos_is801_proc(p_payload, payload_length);
      }
      break;

    case C_SUPL_ASN1_CHOICE_RRC_PAYLOAD:
      (void)tm_umts_up_supl_supl_end_tx(&p_supl_session_status->z_session_id.z_Slp_Sess_Id,
                                        NULL,
                                        supl_posProtocolMismatch,
                                        NULL,
                                        p_supl_session_status->q_supl_version);
      tm_umts_up_supl_callflow_disrupt_handler();
      break;

#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
    case C_SUPL_ASN1_CHOICE_EXT_PAYLOAD:
      if (p_suplPos->posPayLoad.u.ver2_PosPayLoad_extension->m.lPPPayloadPresent)
      {
        if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
        {
          u_ret = tm_lpp_up_suplpos_lpp_proc(&p_suplPos->posPayLoad.u.ver2_PosPayLoad_extension->lPPPayload);

        }
      }
      break;
#endif /* FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY */
  }

  return u_ret;
}


/*
******************************************************************************
* supl_SuplPos_is801_proc
*
* Function description:
*
* Process incoming SUPL POS message containing IS801 payload
*
* Parameters: p_payload: pointer to SUPL POS IS801 payload
*                        payload_length: Length of the payload
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
static boolean supl_SuplPos_is801_proc(uint8 *p_payload, const uint16 payload_length)
{

  byte bytes_consumed_801_1;

  /* Forward it to IS801 for decoding */
  return is801_decode_fwd_link_pddm(TM_PRTL_TYPE_1X_UP,
                                    p_payload,
                                    payload_length,
                                    &bytes_consumed_801_1);
}


/*
******************************************************************************
* static boolean supl_SuplPos_rrlp_tx
*
* Function description:
*
* Encode and transmit SUPL POS with RRLP payload.
* measurements.
*
* Parameters: p_rrlp_data - pointer to rrlp payload;
*             w_byte_cnt - num. of bytes in rrlp payload;
*             p_velocity - pointer the struture holding velocity (null if not available )
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
boolean supl_SuplPos_rrlp_tx(uint8 *p_rrlp_data, uint16 w_byte_cnt, supl_Velocity *p_velocity)
{
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;

  boolean u_ret = FALSE;


  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */
  supl_ULP_PDU      pdu;
  supl_SUPLPOS      z_SuplPos;
  supl_SUPLPOS *p_SuplPos;

  uint8 *p_length;
  uint8            u_length_msb = 0;
  uint8            u_length_lsb = 0;

  supl_PosPayLoad_rrlpPayload z_rrlp_payload;

  int encode_status;


  memset(&pdu, 0x00, sizeof(supl_ULP_PDU));
  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));
  memset(&z_SuplPos, 0x00, sizeof(supl_SUPLPOS));
  memset(&z_rrlp_payload, 0x00, sizeof(supl_PosPayLoad_rrlpPayload));

  pdu.message.t = T_supl_UlpMessage_msSUPLPOS;
  p_SuplPos = &z_SuplPos;
  pdu.message.u.msSUPLPOS = p_SuplPos;

  /* overhead build function has to be after we know what message is to be constructed */
  if (!supl_header_build(&pdu, NULL, NULL, NULL, p_supl_session_status->q_supl_version))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosRrlp attempt aborted: header", 0, 0, 0);
    return FALSE;
  }

#ifndef FEATURE_CGPS_SUPL_VARIANT_SUPL_POS_NO_VEL /* NOT by default defined */
  if (p_velocity == NULL)
  {
    p_SuplPos->m.velocityPresent = 0; /* no optional IE of velocity included */
  }
  else
  {
    p_SuplPos->m.velocityPresent = 1;
    p_SuplPos->velocity = *p_velocity;
  }
#endif

  if (w_byte_cnt > 8192)
    return FALSE;
  z_rrlp_payload.numocts = w_byte_cnt;
  memscpy(z_rrlp_payload.data, sizeof(z_rrlp_payload.data), p_rrlp_data, w_byte_cnt);

  p_SuplPos->posPayLoad.t = C_SUPL_ASN1_CHOICE_RRLP_PAYLOAD;
  p_SuplPos->posPayLoad.u.rrlpPayload = &z_rrlp_payload;

  /* If encoding failed, pdu_buf.value will
  be NULL and pdu_buf.length is meaningless. If encoding succeeded,
  pdu_buf.value will be non-NULL and pdu_buf.length will contain the number
  of bytes in the encoded PDU. */


  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, PROTO_SUPL);

  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }
    return FALSE;
  }

  else
  {

    pd_comms_return_type ret_val;

    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (pdu_buff.length >> 8) & 0x00FF;
    u_length_lsb = pdu_buff.length & 0x00FF;

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, pdu_buff.length);

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosRrlp attempt aborted", 0, 0, 0);
    }
    else if (p_supl_session_status->u_current_is_for_area_event == FALSE)
    {
      if ((p_supl_session_status->z_network_assist.u_MoreToCome == TRUE) ||
          ((p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI) &&
           (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_rrlp_choice
            == C_RRLP_CHOICE_ASSISTDATA)
           )
          )
      {
        supl_set_cf_state(C_SUPL_CF_STATE_UT3_RRLP, 0);
      }
      else if (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_rrlp_choice
               == C_RRLP_CHOICE_ASSISTDATA)
      {
        supl_set_cf_state(C_SUPL_CF_STATE_UT3_ACK, 0);
      }
      else
      {
        supl_set_cf_state(C_SUPL_CF_STATE_UT3, 0);
      }
      u_ret = TRUE;
    }
    else
    {
      u_ret = TRUE;
    }

    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
    pdu_buff.value = NULL;

  }

  if (u_ret)
  {

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: SuplPos sent ", 0, 0, 0);
  }


  return u_ret;
}

/*
******************************************************************************
* static boolean tm_umts_up_supl_SuplPos_is801_tx
*
* Function description:
*
* Encode and transmit SUPL POS with IS801 payload.
* measurements.
*
* Parameters: p_rrlp_data - pointer to rrlp payload;
*             w_byte_cnt - num. of bytes in rrlp payload;
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
boolean tm_umts_up_supl_SuplPos_is801_tx(uint8 *p_is801_data, const uint16 w_byte_cnt)
{

#if defined (FEATURE_CGPS_USES_CDMA) && !defined (FEATURE_GNSS_LOW_MEMORY)

  supl_status_s_type *p_supl_session_status = &z_supl_session_status;

  boolean u_ret = FALSE;

  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */
  supl_ULP_PDU      pdu;

  supl_SUPLPOS      z_SuplPos;
  supl_SUPLPOS *p_SuplPos;

  uint8 *p_length;
  uint8            u_length_msb = 0;
  uint8            u_length_lsb = 0;


  supl_PosPayLoad_tia801payload z_is801_payload;
  int encode_status;

  memset(&pdu, 0x00, sizeof(supl_ULP_PDU));
  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));
  memset(&z_SuplPos, 0x00, sizeof(supl_SUPLPOS));
  memset(&z_is801_payload, 0x00, sizeof(supl_PosPayLoad_tia801payload));

  pdu.message.t = T_supl_UlpMessage_msSUPLPOS;
  p_SuplPos = &z_SuplPos;
  pdu.message.u.msSUPLPOS = p_SuplPos;


  /* overhead build function has to be after we know what message is to be constructed */
  if (!supl_header_build(&pdu, NULL, NULL, NULL, p_supl_session_status->q_supl_version))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosIs801 attempt aborted: header", 0, 0, 0);
    return FALSE;
  }

#ifndef FEATURE_CGPS_SUPL_VARIANT_SUPL_POS_NO_VEL /* NOT by default defined */
  p_SuplPos->m.velocityPresent = 0; /* no optional IE of velocity included */
#endif

  if (w_byte_cnt > 8192)
    return FALSE;
  z_is801_payload.numocts =  w_byte_cnt;
  memscpy(z_is801_payload.data, sizeof(z_is801_payload.data), p_is801_data, w_byte_cnt);
  p_SuplPos->posPayLoad.t = C_SUPL_ASN1_CHOICE_TIA801_PAYLOAD;
  p_SuplPos->posPayLoad.u.tia801payload = &z_is801_payload;


  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, PROTO_SUPL);

  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }
    return FALSE;
  }

  else
  {
    pd_comms_return_type ret_val;

    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (pdu_buff.length >> 8) & 0x00FF;
    u_length_lsb = pdu_buff.length & 0x00FF;

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, pdu_buff.length);

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosIs801 attempt aborted", 0, 0, 0);
    }
    else
    {
      u_ret = TRUE;
    }


    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
    pdu_buff.value = NULL;

  }

  if (u_ret)
  {

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: SuplPos sent ", 0, 0, 0);
  }


  return u_ret;

#else
  return FALSE;  /* dummy for builds without 1X */
#endif /* FEATURE_CGPS_USES_CDMA && ! FEATURE_GNSS_LOW_MEMORY */

}

boolean tm_umts_up_supl_suplpos_lpp_tx(supl_Ver2_PosPayLoad_extension *pz_lpp_payload)
{
  boolean            b_ret                  = FALSE;
  int encode_status = 0;
  uint8              *p_length;
  uint8              u_length_msb           = 0;
  uint8              u_length_lsb           = 0;
  supl_ULP_PDU       pdu;
  supl_SUPLPOS       z_SuplPos;
  supl_SUPLPOS       *p_SuplPos             = NULL;
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;
  tm_pdu_buf_type    pdu_buff; /* including length and pointer to data */

  if (NULL == pz_lpp_payload)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPLPOS LPP param NULL");
    return (FALSE);
  }

  memset(&pdu, 0, sizeof(pdu));
  memset(&z_SuplPos, 0, sizeof(z_SuplPos));
  pdu.message.t = T_supl_UlpMessage_msSUPLPOS;
  p_SuplPos = &z_SuplPos;
  pdu.message.u.msSUPLPOS = p_SuplPos;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SuplPos LPP Tx");

  /* overhead build function has to be after we know what message is to be constructed */
  if (!supl_header_build(&pdu, NULL, NULL, NULL, p_supl_session_status->q_supl_version))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SuplPos LPP tx attempt aborted: header");
    return (FALSE);
  }

#ifndef FEATURE_CGPS_SUPL_VARIANT_SUPL_POS_NO_VEL /* NOT by default defined */
  p_SuplPos->m.velocityPresent = 0; /* no optional IE of velocity included */
#endif

  /* copy the Prov Cap to the Supl Pos init supl pos */
  p_SuplPos->posPayLoad.u.ver2_PosPayLoad_extension = pz_lpp_payload;
  p_SuplPos->posPayLoad.t = C_SUPL_ASN1_CHOICE_EXT_PAYLOAD;


  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, (int)PROTO_SUPL);
  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPLPOS with LPP encoding failed ");
    return (FALSE);
  }
  else
  {
    pd_comms_return_type ret_val;

    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (uint8)((pdu_buff.length >> 8) & 0x00FF);
    u_length_lsb = (uint8)(pdu_buff.length & 0x00FF);

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    if (TRUE == p_supl_session_status->wait_for_write_ack)
    {
      (void)tm_umts_up_supl_enqueue_msg((tm_pdu_buf_type *)&pdu_buff);
      ret_val = PDCOMM_RESULT_WAIT;
    }
    else
    {
      ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, (uint16)pdu_buff.length);
    }

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPLPOS with LPP send failed");
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPLPOS with LPP sent");
      b_ret = TRUE;
    }

    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, (int)PROTO_SUPL);
    pdu_buff.value = NULL;
  }

  return (b_ret);
}

/*
******************************************************************************
* supl_SuplEnd_proc
*
* Function description:
*
* Process incoming SUPL POS message
*
* Parameters: p_pdu_decoded: pointer to a structure holding decoded Supl PDU.
*
* Return value: None
*
******************************************************************************
*/

static void supl_SuplEnd_proc(const supl_ULP_PDU *p_pdu_decoded)
{
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;
  pdsm_pd_info_s_type *p_pd_info_buffer = &z_supl_pd_info_buffer;
  pdsm_pd_event_type       pd_event = PDSM_PD_EVENT_POSITION;
  tm_post_data_payload_type   z_tm_payload;
  const supl_SUPLEND *p_suplEnd;
  uint8 u_area_event_session_match_num = 0xFF;
  gnss_NavSlnFlagsStructType z_pos_flags;
  uint8 u_Confidence = 0;

  tm_cm_phone_state_info_s_type phone_state_info;
  pdsm_pd_comm_protocol_e_type protocol_type = PDSM_PD_COMM_PROTOCOL_UMTS_UP_SUPL;
  
  tm_cm_iface_get_phone_state(&phone_state_info);

  memset(&z_pos_flags, 0x00, sizeof(z_pos_flags));


  if (NULL == p_pdu_decoded)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "NULL SUPL END PDU, clear the session");
    tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
    tm_umts_up_supl_session_init(FALSE);
    return;
  }
  p_suplEnd = p_pdu_decoded->message.u.msSUPLEND;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"process_supl_end()at CF state %x", p_supl_session_status->e_cf_state, 0, 0);
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"supl_SuplEnd_proc   cf_state protocol callflow agps_mode fix_reported supl_close_waiting");
  MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"supl_SuplEnd_proc %x       %d       %d       %d           %d       %d", 
        p_supl_session_status->e_cf_state, 
        p_supl_session_status->u_position_protocol, 
        p_supl_session_status->u_callflow_kind, 
        p_supl_session_status->u_agps_mode, 
        p_supl_session_status->u_pdapi_fix_reported, 
        p_supl_session_status->supl_close_waiting);

  /* no need to maintain the data connection anymore */
  tm_umts_up_supl_comm_close_connection();

  /* the network session is considered terminated */
  p_supl_session_status->u_network_session_on = FALSE;

  p_supl_session_status->u_send_supl_end_at_fail = FALSE;

  if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT10_INFO_QUERY_END)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT10_INFO_QUERY_END);
    supl_session_status_reset();
    return; /* no further action needed */
  }


  if ((u_area_event_session_match_num =
       supl_area_event_session_match(&p_pdu_decoded->sessionID)
       ) < C_MAX_GEOFENCE_NUM
      )
  {
    /* Session ID matches an existing areaEvent session */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_END from SLP ending areaEven session %u",
          u_area_event_session_match_num, 0, 0);

    /* terminate all session timer associated with this area event session */
    tm_umts_up_supl_terminate_timer_cancel_cf_state_area_event
       (C_SUPL_CF_STATE_NULL, u_area_event_session_match_num);

    /* clear session variables associated with this area event session */
    memset(&z_supl_area_event_session_status[u_area_event_session_match_num], 0,
           sizeof(supl_area_event_status_s_type));

  }


  /* if position is included, "seed injection" and "LR post" */
  if (p_suplEnd->m.positionPresent)
  {
    memset(p_pd_info_buffer, 0, sizeof(pdsm_pd_info_s_type));
    p_pd_info_buffer->client_id = p_supl_session_status->q_pdapi_client_id;
    p_supl_session_status->pd_info_buffer_valid = TRUE;

    /* this function populates the local structure of z_tm_payload for seed
       position post, and also populates part of the global structure of
       z_supl_pd_info_buffer pdapi event reporting including the extended pos.
       part of the structure; some other part of z_supl_pd_info_buffer will be
       determined later */
    supl_convert_supl_end_pos_for_tm_core(p_suplEnd, &z_tm_payload.seed_data);

    (void)tm_post_data(TM_PRTL_TYPE_UMTS_UP,
                       p_supl_session_status->tm_active_session_handle,
                       TM_POST_DATA_TYPE_LR,
                       &z_tm_payload);

  }

  /* if network initiated, SUPL protocol module's job is done */
  if (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI)
  {
    if (!p_supl_session_status->u_pdapi_fix_reported)
    {      	  
      /* if pdapi report has not been sent, such as in NI-MSA case, report
         this position to pdapi */
      if (p_suplEnd->m.positionPresent)
      {

        if (C_SUPL_AGPS_MODE_UE_BASED == p_supl_session_status->u_agps_mode)
        {
          p_pd_info_buffer->pd_info.pd_data.position_mode = PDSM_PD_POSITION_MODE_MSBASED;
        }
        else
        {
          p_pd_info_buffer->pd_info.pd_data.position_mode = PDSM_PD_POSITION_MODE_MSASSISTED;
        }

        /* SUPL_END does not provide source info. set the source as default which will be mapped as reference location */
        p_pd_info_buffer->pd_info.pd_data.positioning_source = PDSM_PD_POSITION_SOURCE_DEFAULT;

        /* set up proper position type in the pdapi report */
        p_pd_info_buffer->pd_info.pd_data.position_type = PDSM_PD_POSITION_TYPE_NEW;

        pd_event = PDSM_PD_EVENT_POSITION;

        if (p_suplEnd->position.positionEstimate.m.altitudeInfoPresent)
        {
          pd_event |= PDSM_PD_EVENT_HEIGHT;
          z_pos_flags.b_Is3D = TRUE;
        }
        if (p_suplEnd->position.m.velocityPresent)
        {
          pd_event |= PDSM_PD_EVENT_VELOCITY;
          z_pos_flags.b_IsVelValid = TRUE;
        }

        (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                               p_supl_session_status->tm_active_session_handle,
                               pd_event,
                               &z_supl_pd_info_buffer,
                               &z_pos_flags);
        
      } /* if position present */
    }   /* if pdpai report has not occured */

    if (p_suplEnd->m.statusCodePresent)
    {
      /* network treats SUPL call flow as failure*/
      tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_USER_TERMINATED);
      tm_umts_up_supl_session_init(FALSE);
    }
    else
    {
      /* network treats SUPL call flow as pass*/
      tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
      tm_umts_up_supl_session_init(FALSE);
    }
  }

  else if (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_UI)
  {

    if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
    {

      if (p_supl_session_status->u_pdapi_fix_reported)
      {
        /* For MO MSB, if position has already been reported to pdapi,
           and SUPL_END arrives, end the session */
        tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
        tm_umts_up_supl_session_init(FALSE);
        return;
      }

      /* PDAPI Fix has not been reported yet, if position is present in suplEnd,
       * report that as the PDAPI Position */
      if ( p_suplEnd->m.positionPresent )
      {

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL End Carrying Position", 0, 0, 0);

        p_pd_info_buffer->pd_info.pd_data.position_mode = PDSM_PD_POSITION_MODE_MSBASED;

        /* SUPL_END does not provide source info. */
        p_pd_info_buffer->pd_info.pd_data.positioning_source = PDSM_PD_POSITION_SOURCE_GPS | PDSM_PD_POSITION_SOURCE_GLO;

        /* set up proper position type in the pdapi report */
        p_pd_info_buffer->pd_info.pd_data.position_type = PDSM_PD_POSITION_TYPE_NEW;

        /* If Confidence is provided with Uncertainity Major and Minor values
           use it for scaling before we check the position for Accuracy requirement*/
        if (p_suplEnd->position.positionEstimate.m.confidencePresent)
        {
          u_Confidence = p_suplEnd->position.positionEstimate.confidence;
        }
        else
        {
          u_Confidence = 63; /*If NW does not provide the confidence, use default 63
                               reference tm_lpp_common_xlate_lpp_hor_qos_req*/
        }
        /* If this position, most likely cell-id based, does not satisfy
           qoP, continue as standalone and if in the end there is no better one, use
           this one;  if this one satisfies qoP, end the session */

        if ((p_suplEnd->position.positionEstimate.m.uncertaintyPresent &&
                ( tm_umts_common_utils_GADK_unc_to_meter
                  ( TRUE, p_suplEnd->position.positionEstimate.uncertainty.uncertaintySemiMajor,
                   p_suplEnd->position.positionEstimate.uncertainty.uncertaintySemiMinor, u_Confidence ) <
                  p_supl_session_status->z_session_qos.q_hor_acc_meters
                )
             ) ||
            (tm_umts_common_utils_meter_to_GADK
             (TRUE, p_supl_session_status->z_session_qos.q_hor_acc_meters) == 127
             ) ||
            ((p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP) &&
             tm_lpp_up_rli_received()
             )
            )
        {
          pd_event = PDSM_PD_EVENT_POSITION;

          if (p_suplEnd->position.positionEstimate.m.altitudeInfoPresent)
          {
            z_pos_flags.b_Is3D = TRUE;
            pd_event |= PDSM_PD_EVENT_HEIGHT;
          }
          if (p_suplEnd->position.m.velocityPresent)
          {
            z_pos_flags.b_IsVelValid = TRUE;
            pd_event |= PDSM_PD_EVENT_VELOCITY;
          }

          (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                                 p_supl_session_status->tm_active_session_handle,
                                 pd_event,
                                 &z_supl_pd_info_buffer,
                                 &z_pos_flags);

          p_supl_session_status->u_pdapi_fix_reported = TRUE;

        } /* if the position in the message passes qoP */

      } /* if Position info. is in the message */

      if (((p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801) ||
           (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)) &&
          (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3))
      {
        /* If an AGNSS error has been reported, and SUPL_END has been received, 
         * then tear down this session. */
        if(p_supl_session_status->u_agnss_error_reported == TRUE)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"AGNSS error reported. Clean up Session");
          tm_umts_up_supl_inform_core_stop_session( TM_STOP_REASON_COMPLETED );
          tm_umts_up_supl_session_init(FALSE);
        }
        else /*Fix is already reported*/
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Fix is already reported", 0, 0, 0);
          if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
          {
            (void)os_TimerStop(supl_cf_timer);
            /* clear timer(s) and call flow states if not already done */
            tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);
            /*Stop IS801 session*/
            supl_stop_is801_session(TM_STOP_REASON_COMPLETED);
          }
        }
      }

      if ((p_supl_session_status->u_pdapi_fix_reported == TRUE) ||
          (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3)
          )
      {
        /* Position already reported, or has received SUPL_END after sending
           SUPL_POS(RRLP-measPosResp-Loc/Err ) */

        /* if there is no active network session going, send DONE event to pdapi */

        tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
        tm_umts_up_supl_session_init(FALSE);
      }
      else if ((p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_ACK) ||
               (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT1) ||
               (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT2) ||
               (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_RRLP)
               )
      {
        /* Normal case: receiving SUPL_POS after sending SUPL_POS(RRLP-ACK):
           UT3_ACK; Exception case: UT1, UT2, or UT3_RRLP */

        /* inform TM core that network assistance has completed */
        tm_umts_up_supl_inform_core_nt_assist_end(p_supl_session_status);

        /* clear timer(s) and call flow states if not already done */
        tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);
      }

    } /* close if-Ue-Based */

    else if ((p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED) ||
             (p_supl_session_status->u_agps_mode == C_SUPL_AFLT_MODE) ||
             (p_supl_session_status->u_agps_mode == C_SUPL_OTDOA_MODE)
             )
    {
      /************************ only SET-assisted reaches below ********************/

      /********************** position *********************************************/

      if (p_suplEnd->m.positionPresent)
      {

        p_pd_info_buffer->pd_info.pd_data.position_mode = PDSM_PD_POSITION_MODE_MSASSISTED;

        /* SUPL_END does not provide source info. */
        p_pd_info_buffer->pd_info.pd_data.positioning_source = PDSM_PD_POSITION_SOURCE_GPS | PDSM_PD_POSITION_SOURCE_GLO;

        /* set up proper position type in the pdapi report */
        p_pd_info_buffer->pd_info.pd_data.position_type = PDSM_PD_POSITION_TYPE_NEW;

        pd_event = PDSM_PD_EVENT_POSITION;

        if (p_suplEnd->position.positionEstimate.m.altitudeInfoPresent)
        {
          z_pos_flags.b_Is3D = TRUE;
          pd_event |= PDSM_PD_EVENT_HEIGHT;
        }
        if (p_suplEnd->position.m.velocityPresent)
        {
          z_pos_flags.b_IsVelValid = TRUE;
          pd_event |= PDSM_PD_EVENT_VELOCITY;
        }

        (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                               p_supl_session_status->tm_active_session_handle,
                               pd_event,
                               &z_supl_pd_info_buffer,
                               &z_pos_flags);

        /* if there is no active network session going, send DONE event to pdapi */
        tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
        tm_umts_up_supl_session_init(FALSE);
      } /* close if-Position-available */

      else /* if SUPL_END in a SET-Assisted call flow contains no position */
      {
        if (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3)
        {
          /*For IS801 session, SUPL end may not have the position*/
          if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
          {
            tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
            tm_umts_up_supl_session_init(FALSE);
          }
          else
          {
            z_supl_pd_info_buffer.pd_info.end_status = PDSM_PD_END_NONSPECIFIED_ERROR;
            if (p_suplEnd->m.statusCodePresent)
            {
              if (p_suplEnd->statusCode == supl_StatusCode_posMethodFailure)
              {
                z_supl_pd_info_buffer.pd_info.end_status =
                   PDSM_PD_END_FIX_METHOD_FAILURE;
              }
            }
            tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
          }
        }
        else
        {
          z_supl_pd_info_buffer.pd_info.end_status = PDSM_PD_END_UNKNWN_SYS_ERROR;
          tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
        }

        /* clear timer(s) and call flow states if not already done */
        tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);

        /* clear all protocol session related variables */
        tm_umts_up_supl_session_init(FALSE);

      }

    } /* close-else-if-SET-Assisted */

    else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
    {
      if (phone_state_info.srv_system == CGPS_SRV_SYS_LTE)
      {
        protocol_type = PDSM_PD_COMM_PROTOCOL_LTE_UP;
      }
      tm_umts_common_send_pdapi_comm_event
         (protocol_type,
          C_COMM_DONE,
          p_supl_session_status->tm_active_session_handle,
          p_supl_session_status->q_pdapi_client_id
          );

      if (p_suplEnd->m.positionPresent)
      {

        p_pd_info_buffer->pd_info.pd_data.position_mode = PDSM_PD_POSITION_MODE_UNKNOWN;

        /* SUPL_END does not provide source info. */
        p_pd_info_buffer->pd_info.pd_data.positioning_source = PDSM_PD_POSITION_SOURCE_CELLID;

        /* set up proper position type in the pdapi report */
        p_pd_info_buffer->pd_info.pd_data.position_type = PDSM_PD_POSITION_TYPE_NEW;

        pd_event = PDSM_PD_EVENT_POSITION;

        if (p_suplEnd->position.positionEstimate.m.altitudeInfoPresent)
        {
          pd_event |= PDSM_PD_EVENT_HEIGHT;
          z_pos_flags.b_Is3D = TRUE;
        }
        if (p_suplEnd->position.m.velocityPresent)
        {
          pd_event |= PDSM_PD_EVENT_VELOCITY;
          z_pos_flags.b_IsVelValid = TRUE;
        }

        (void)tm_post_pd_event(TM_PRTL_TYPE_UMTS_UP,
                               p_supl_session_status->tm_active_session_handle,
                               pd_event,
                               &z_supl_pd_info_buffer,
                               &z_pos_flags);

        p_supl_session_status->u_pdapi_fix_reported = TRUE;

        /* if there is no active network session going, send DONE event to pdapi */
        tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
        tm_umts_up_supl_session_init(FALSE);

      } /* if Position info. is in the message */
      else /* if SUPL_END in a CELL ID call flow contains no position */
      {
        z_supl_pd_info_buffer.pd_info.end_status = PDSM_PD_END_NONSPECIFIED_ERROR;
        if (p_suplEnd->m.statusCodePresent)
        {
          z_supl_pd_info_buffer.pd_info.end_status = PDSM_PD_END_PDE_REJECT;
        }
        tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);

        /* clear timer(s) and call flow states if not already done */
        tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);

        /* clear all protocol session related variables */
        tm_umts_up_supl_session_init(FALSE);
      }

    } /* close if-CELL ID */
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: unexpected agps mode: %u", p_supl_session_status->u_agps_mode, 0, 0);
      return;
    }

  } /* close-if-UI */


}


/*
******************************************************************************
* tm_umts_up_supl_supl_end_prepare_or_tx
*
* Function description:
*
* Establish TCP link for sending SuplEnd or call tm_umts_up_supl_supl_end_tx to send
*   SuplEnd.
*
* Parameters:
*
*   p_provided_slp_session_id - pointer to the struture holding
*   slp session ID.
*
*   e_supl_status_code: as enum'ed supl_StatusCode,
*   use 0xFFFFFFFF if status code is to be absent.
*
*   p_hash: pointer to the first byte of HASH, use NULL if hash is to be absent.
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/


boolean  tm_umts_up_supl_supl_end_prepare_or_tx(supl_slp_session_id *p_provided_slp_session_id,
                                                supl_set_session_id *p_provided_set_session_id,
                                                supl_StatusCode e_supl_status_code,
                                                uint8 *p_hash,
                                                uint32 q_supl_version)
{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  pd_comms_return_type pdcomm_connect_ret_value = PDCOMM_RESULT_ERROR_UNKNOWN;
  boolean u_ret = FALSE;
  pending_supl_end   *p_pending_supl_end;

  supl_set_session_id z_set_session_id_immediate_use;

  uint8  u_i;
  boolean u_need_cache = FALSE;

  memset(&z_set_session_id_immediate_use, 0, sizeof(supl_set_session_id));


  /* if no tcp link available yet, set up such link */
  if (z_pdcomm_tcp_info.curr_state != PDCOMM_STATE_CONNECTED)
  {
    /* if not already in UT0 waiting for connection either, initiate
       connection now*/
    if (!supl_is_in_any_ut0())
    {
      /* LSB in the variable passed contains "version number main" */
      pdcomm_connect_ret_value = tm_umts_up_supl_comm_tcp_connect((uint8)(q_supl_version & 0x000000FF));

      if (PDCOMM_RESULT_OK == pdcomm_connect_ret_value) /* quite unlikely */
      {

        if (p_provided_slp_session_id ==  NULL)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                "SLP_sessionID NULL %u, %lx, %lx",
                pdcomm_connect_ret_value,
                p_provided_set_session_id,
                p_provided_slp_session_id);

          return FALSE;

        }

        if (p_provided_set_session_id == NULL)
        {
          if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
          {
            /* if SET session ID (ismi/msisdn... is still unknow at this point,
               this function will retried it */
            supl_SET_id_get();
          }
          if (z_Set_Id.set_id_choice != C_SETID_UNKNOWN)
          {
            z_set_session_id_immediate_use.w_Session_Id = 0;
            memscpy(&z_set_session_id_immediate_use.z_Set_Id,
                    sizeof(z_set_session_id_immediate_use.z_Set_Id),
                    &z_Set_Id, sizeof(z_Set_Id));
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SET_ID unavilable: %d",
                  z_Set_Id.set_id_choice);

            return FALSE;
          }
          if (tm_umts_up_supl_supl_end_tx(p_provided_slp_session_id,
                                          &z_set_session_id_immediate_use,
                                          e_supl_status_code,
                                          p_hash,
                                          q_supl_version)
              )
          {
            u_ret = TRUE;
          }

        }

        else
        {
          if (tm_umts_up_supl_supl_end_tx(p_provided_slp_session_id,
                                          p_provided_set_session_id,
                                          e_supl_status_code,
                                          p_hash,
                                          q_supl_version)
              )
          {
            u_ret = TRUE;
          }

        }
      } /* pdcomm status now shows connected */

      else if (PDCOMM_RESULT_WAIT == pdcomm_connect_ret_value) /* most likely */
      {
        u_need_cache = TRUE;

      } /* pdcomm now shows WAIT */

      else
      {
        /* not possible to connect, abort */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: TCP conn. failed", 0, 0, 0);
      }

    } /* not in any UT0*/

    else /* already in an UT0 statas, don't attempt to bring data link again */
    {
      u_need_cache = TRUE;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Already in UT0, cache SUPL_END", 0, 0, 0);
    } /* has been in UT0 already */

    if (u_need_cache)
    {
      p_pending_supl_end = &z_pending_supl_end[0];
      z_supl_session_status.q_supl_version = q_supl_version;

      /* if the number of pending SUPL_END messages exceeds the maximum, this one
         will not be sent */
      for (u_i = C_MAX_NUM_PENDING_SUPL_END; u_i; u_i--)
      {
        if (p_pending_supl_end->u_validity == FALSE)
        {

          if (p_provided_set_session_id != NULL)
          {
            memscpy(&p_pending_supl_end->set_session_id,
                    sizeof(p_pending_supl_end->set_session_id),
                    p_provided_set_session_id,
                    sizeof(*p_provided_set_session_id));
          }
          else
          {
            if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
            {
              /* if SET session ID (ismi/msisdn... is still unknow at this point,
                 this function will retried it */
              supl_SET_id_get();
            }
            if (z_Set_Id.set_id_choice != C_SETID_UNKNOWN)
            {
              p_pending_supl_end->set_session_id.w_Session_Id = 0;
              memscpy(&p_pending_supl_end->set_session_id.z_Set_Id,
                      sizeof(p_pending_supl_end->set_session_id.z_Set_Id),
                      &z_Set_Id, sizeof(z_Set_Id));
            }
            else
            {
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SET_ID unavilable: %d",
                    z_Set_Id.set_id_choice);

              return FALSE;
            }
          }

          if (p_provided_slp_session_id != NULL)
          {
            memscpy(&p_pending_supl_end->slp_session_id,
                    sizeof(p_pending_supl_end->slp_session_id),
                    p_provided_slp_session_id,
                    sizeof(*p_provided_slp_session_id));
          }
          else
          {
            memscpy(&p_pending_supl_end->slp_session_id,
                    sizeof(p_pending_supl_end->slp_session_id),
                    &z_supl_session_status.z_session_id.z_Slp_Sess_Id,
                    sizeof(z_supl_session_status.z_session_id.z_Slp_Sess_Id));
          }
          p_pending_supl_end->e_supl_status_code = e_supl_status_code;

          if (p_hash != NULL)
          {
            p_pending_supl_end->u_is_hash_present = TRUE;
            memscpy(p_pending_supl_end->hash, sizeof(p_pending_supl_end->hash), p_hash, sizeof(p_pending_supl_end->hash));
          }
          else
          {
            p_pending_supl_end->u_is_hash_present = FALSE;
          }

          p_pending_supl_end->u_validity = TRUE;

          break;
        }
        else
        {
          p_pending_supl_end++;
        }

      }

      /* if not yet set state to include C_SUPL_CF_STATE_UT0_SUPLEND, do so now */
      if (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND))
      {
        supl_set_cf_state(C_SUPL_CF_STATE_UT0_SUPLEND, 0);
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: UT0-SUPLEND starts", 0, 0, 0);
      }

    } /* cache SUPL_END for later delivery */

  } /* close if-tcp-link-not-yet-available */

  else /* already connected */
  {
    if (p_provided_slp_session_id ==  NULL)
    {
      /* SLP session ID being NULL is not used in this scenario */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
            "SLP_sessionID NULL %u, %lx, %lx",
            pdcomm_connect_ret_value,
            p_provided_set_session_id,
            p_provided_slp_session_id);

      return FALSE;

    }

    if (p_provided_set_session_id == NULL)
    {
      if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
      {
        /* if SET session ID (ismi/msisdn... is still unknow at this point,
           this function will retried it */
        supl_SET_id_get();
      }
      if (z_Set_Id.set_id_choice != C_SETID_UNKNOWN)
      {
        z_set_session_id_immediate_use.w_Session_Id = 0;
        memscpy(&z_set_session_id_immediate_use.z_Set_Id,
                sizeof(z_set_session_id_immediate_use.z_Set_Id),
                &z_Set_Id, sizeof(z_Set_Id));
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SET_ID unavilable: %d",
              z_Set_Id.set_id_choice);

        return FALSE;
      }

      if (tm_umts_up_supl_supl_end_tx(p_provided_slp_session_id,
                                      &z_set_session_id_immediate_use,
                                      e_supl_status_code,
                                      p_hash,
                                      q_supl_version)
          )
      {
        u_ret = TRUE;
      }

    }

    else
    {

      if (tm_umts_up_supl_supl_end_tx(p_provided_slp_session_id,
                                      p_provided_set_session_id,
                                      e_supl_status_code,
                                      p_hash,
                                      q_supl_version)
          )
      {
        u_ret = TRUE;
      }
    }

  }

  return u_ret;

}


/*
******************************************************************************
* tm_umts_up_supl_supl_end_tx
*
* Function description:
*
* Encode and transmit SUPL END
*
* Parameters:
*
*   p_provided_slp_session_id - pointer to the struture holding
*   slp session ID.
*
*   e_supl_status_code: as enum'ed supl_StatusCode,
*   use 0xFFFFFFFF if status code is to be absent.
*
*   p_hash: pointer to the first byte of HASH, use NULL if hash is to be absent.
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/

boolean tm_umts_up_supl_supl_end_tx(supl_slp_session_id *p_provided_slp_session_id,
                                    supl_set_session_id *p_provided_set_session_id,
                                    supl_StatusCode e_supl_status_code,
                                    uint8 *p_hash,
                                    uint32 q_supl_version)
{

  boolean u_ret = FALSE;

  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */
  supl_ULP_PDU      pdu;
  supl_SUPLEND *p_Suplend;
  supl_SUPLEND      z_Suplend;

  uint8 *p_length;
  uint8            u_length_msb = 0;
  uint8            u_length_lsb = 0;

  uint16 *p_SET_SessionID_sessionId;
  supl_set_id *p_SET_ID;

  int encode_status;

  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));
  memset(&pdu, 0x00, sizeof(supl_ULP_PDU));
  memset(&z_Suplend, 0, sizeof(supl_SUPLEND));

  pdu.message.t = C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLEND;
  p_Suplend = &z_Suplend;
  pdu.message.u.msSUPLEND = p_Suplend;

  if (p_provided_set_session_id == NULL)
  {
    p_SET_SessionID_sessionId = NULL;
    p_SET_ID = NULL;
  }
  else
  {
    p_SET_SessionID_sessionId = &p_provided_set_session_id->w_Session_Id;
    p_SET_ID = &p_provided_set_session_id->z_Set_Id;
  }

  /* overhead build function has to be after we know what message is to be constructed */
  if (!supl_header_build(&pdu, p_SET_SessionID_sessionId, p_SET_ID, p_provided_slp_session_id, q_supl_version))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplEnd attempt aborted: header", 0, 0, 0);
    return FALSE;
  }

  if (e_supl_status_code != 0xFFFFFFFF)
  {
    p_Suplend->m.statusCodePresent = 1;
    p_Suplend->statusCode = e_supl_status_code;
  }

  if (p_hash != NULL)
  {
    p_Suplend->m.verPresent = 1;
    p_Suplend->ver.numbits = 64;
    memscpy(&p_Suplend->ver.data[0], sizeof(p_Suplend->ver.data), p_hash, sizeof(p_Suplend->ver.data));
  }

  /* If encoding failed, pdu_buf.value will
  be NULL and pdu_buf.length is meaningless. If encoding succeeded,
  pdu_buf.value will be non-NULL and pdu_buf.length will contain the number
  of bytes in the encoded PDU. */

  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, PROTO_SUPL);

  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }
    return FALSE;
  }

  else
  {
    pd_comms_return_type ret_val;

    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (pdu_buff.length >> 8) & 0x00ff;
    u_length_lsb = pdu_buff.length & 0x00ff;

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, pdu_buff.length);

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplEnd attempt aborted", 0, 0, 0);
    }
    else
    {
      u_ret = TRUE;
    }

    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
    pdu_buff.value = NULL;

  }

  if (u_ret)
  {

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SuplEnd sent ", 0, 0, 0);
    z_supl_session_status.u_send_supl_end_at_fail = FALSE;
  }

  return u_ret;
}


/*
 ******************************************************************************
 * supl_header_build
 *
 * Function description:
 *   Provides information for building SUPL overhead: version, SessionID
 *
 * Parameters:
 *
 *  p_pdu: pointer to a structure holding to be encoded SUPL PDU
 *
 *  p_provided_set_session_id: pointer to the provided SET_SESSION_ID.sessionID;
 *
 *  p_provided_Set_Id: pointer to the SET_ID of the one saved provided by the SLP,
 *    mainly for error case handling.
 *
 *  p_provided_slp_session_id: pointer to a structure holding SLP Session ID;
 *    if NULL, this function will use SLP session ID info in z_supl_session_status.
 *
 * Return value:
 *  TRUE: header build successful; FALSE: header build failure.
 *
 *
 ******************************************************************************
*/
boolean supl_header_build
(supl_ULP_PDU *p_pdu,
 uint16 *p_provided_set_session_id,
 supl_set_id *p_provided_Set_Id,
 supl_slp_session_id *p_provided_slp_session_id,
 uint32               q_supl_version)
{
  uint32 cnt;
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  uint16               *p_set_session_id;  /* this is session_id portion of the SET_SESSION_ID */
  supl_slp_session_id  *p_slp_session_id;

  supl_set_id *p_Set_Id;

  if (p_provided_set_session_id == NULL)
  {
    /* this is used by the current session only (no long lasting areaEvent type)
       and is 0 all the time unless carrying a wrong session ID from the SLP and is
       building the header for SUPL_END in this case */
    p_set_session_id = &p_supl_session_status->z_session_id.z_Set_Sess_Id.w_Session_Id;
  }
  else
  {
    p_set_session_id = p_provided_set_session_id;
  }

  if (p_provided_Set_Id == NULL)
  {
    if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
    {
      /* if SET session ID (ismi/msisdn... is still unknow at this point,
         this function will retried it */
      supl_SET_id_get();
    }
    if (z_Set_Id.set_id_choice != C_SETID_UNKNOWN)
    {
      p_Set_Id = &z_Set_Id;
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SET_ID unavilable: %d",
            z_Set_Id.set_id_choice);

      return FALSE; /* unable to get SET_ID */
    }
  }
  else
  {
    p_Set_Id = p_provided_Set_Id;
  }


  if (p_provided_slp_session_id == NULL)
  {
    p_slp_session_id = &z_supl_session_status.z_session_id.z_Slp_Sess_Id;
  }
  else
  {
    p_slp_session_id = p_provided_slp_session_id;
  }


  /* init length to 0.  we will fill in the correct length later after it is encoded */
  p_pdu->length = 0;

  /* version  */
  p_pdu->version.maj = q_supl_version & 0x000000FF;
  p_pdu->version.min = (q_supl_version >> 8) & 0x000000FF;
  p_pdu->version.servind = (q_supl_version >> 16) & 0x000000FF;

  p_pdu->sessionID.m.setSessionIDPresent = 1;

  /* SUPLSTART sets the SET session ID; incremented by 1 each time SUPLSTART is sent,
     using the range of 1 to 65535 */
  if (p_pdu->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLSTART)
  {
    /* SLP Session ID should never be present in SUPL START */
    p_slp_session_id->presence = FALSE;
  }

  if (p_slp_session_id->presence == TRUE)
  {
    p_pdu->sessionID.m.slpSessionIDPresent = 1;
  }

  /******  SET session ID *******/
  p_pdu->sessionID.setSessionID.sessionId = *p_set_session_id;

  if (p_provided_Set_Id == NULL)
  {
    /* if SET session ID (ismi/msisdn... is still unknow at this point,
       this function will retried it */
    supl_SET_id_get();
  }

  if (p_Set_Id->set_id_choice == T_supl_SETId_msisdn)
  {
    p_pdu->sessionID.setSessionID.setId.t = T_supl_SETId_msisdn;
    p_pdu->sessionID.setSessionID.setId.u.msisdn =
       (supl_SETId_msisdn *)&p_Set_Id->u.msisdn;
  }

  else if (p_Set_Id->set_id_choice == T_supl_SETId_imsi)
  {
    p_pdu->sessionID.setSessionID.setId.t = T_supl_SETId_imsi;
    p_pdu->sessionID.setSessionID.setId.u.imsi =
       (supl_SETId_imsi *)&p_Set_Id->u.imsi;

  }

  else if (p_Set_Id->u.other_setID.valid == TRUE)
  {
    /* neither MSISDN nor IMSI found: shouldn't happen unless in the case of
       test cases for error handling. Use the other_setID field
       of the session status structure because in this case the set ID field
       of the incoming message was decoded there and it is needed to return it
       in the SUPL End msg. */
    uint8 *p_src = &p_Set_Id->u.other_setID.raw_contents[0];
    uint8 *p_dest = (uint8 *)&p_pdu->sessionID.setSessionID.setId.u;

    p_pdu->sessionID.setSessionID.setId.t =
       p_Set_Id->set_id_choice;

    for (cnt = OTHER_SETID_SIZE; cnt; cnt--)
    {
      *p_dest++ = *p_src++;
    }
  }


  /*** SLP Session ID  ****/
  if (p_pdu->sessionID.m.slpSessionIDPresent == 1)
  {

    p_pdu->sessionID.slpSessionID.sessionID.numocts = C_SUPL_SLP_SESSION_ID_BYTE_LENGTH;
    memscpy(p_pdu->sessionID.slpSessionID.sessionID.data, sizeof(p_pdu->sessionID.slpSessionID.sessionID.data),
            p_slp_session_id->session_Id, sizeof(p_slp_session_id->session_Id));

    p_pdu->sessionID.slpSessionID.slpId.t = p_slp_session_id->zSlpAddress.choice;

    if (p_pdu->sessionID.slpSessionID.slpId.t == T_supl_SLPAddress_iPAddress)
    {

      p_pdu->sessionID.slpSessionID.slpId.u.iPAddress = &z_supl_IPAddress;
      z_supl_IPAddress.t = p_slp_session_id->zSlpAddress.u.z_Ip_Address.choice;

      /* IPV4 */
      if (p_slp_session_id->zSlpAddress.u.z_Ip_Address.choice == C_SUPL_ASN1_CHOICE_IPV4ADDRESS)
      {
        z_supl_IPAddress.u.ipv4Address = &p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V4;
      }
      /* IPV6 */
      else if (p_slp_session_id->zSlpAddress.u.z_Ip_Address.choice == T_supl_IPAddress_ipv6Address)
      {
        z_supl_IPAddress.u.ipv6Address = &p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V6;
      }
    }

    else if (p_pdu->sessionID.slpSessionID.slpId.t == T_supl_SLPAddress_fQDN)
    {
      p_pdu->sessionID.slpSessionID.slpId.u.fQDN = (supl_FQDN)p_slp_session_id->zSlpAddress.u.z_fqdn.fqdn;

    }

    else
    {
      /* should not happen */
      return FALSE;
    }

  }

  return TRUE;
}

/********************************************************************
 *
 * supl_cell_info_construct_tds_gsm
 *
 * Function description:
 *   Construct cell information specific to active RAT = GSM.
 *
 * Parameters:
 *   p_Asn1CellInfo      : Pointer to a structure that contains
 *                         memory allocated for the ASN1 tool
 *
 *   p_CellInfoAllocated : Pointer to a structure to hold the
 *                         to-be-constructed cell information
 *
 *   q_CellId            : Cell Identity, range: (0...65535)
 *
 *   u_Lac0              : Location Area Code, byte 0
 *
 *   u_Lac1              : Location Area Code, byte 1
 *
 *   z_RrcPlmnId         : Structure that holds MCC/MNC parameters
 *
 * Return: TRUE/FALSE ( FALSE: no valid cell ID )
 *********************************************************************
 */
boolean supl_cell_info_construct_tds_gsm
(
   supl_CellInfo *p_Asn1CellInfo,
   supl_cell_info_struct_type *p_CellInfoAllocated,
   uint32                      q_CellId,
   uint8                       u_Lac0,
   uint8                       u_Lac1,
   rrc_plmn_identity_type      z_RrcPlmnId
   )
{
  uint32                 q_test1, q_test2;
  rrc_plmn_identity_type z_PlmnId = z_RrcPlmnId;

  if ((NULL == p_Asn1CellInfo) || (NULL == p_CellInfoAllocated))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "supl_cell_info_construct_tds_gsm: NULL arguments");
    return FALSE;
  }

  memset(&p_CellInfoAllocated->u1.gsmCell, 0, sizeof(supl_GsmCellInformation));

  p_Asn1CellInfo->t         = C_SUPL_ASN1_CHOICE_GSM_CELL;
  p_Asn1CellInfo->u.gsmCell = &p_CellInfoAllocated->u1.gsmCell;

  /* CI: use low 16 bits of TDS cell identity */
  p_Asn1CellInfo->u.gsmCell->refCI = q_CellId & 0xFFFF;

  /* LAC: tds_plmn_cell_info.lac is of uint8 lac[RRC_MAX_GSMMAP_LAC_LENGTH],
     where RRC_MAX_GSMMAP_LAC_LENGTH is defined as 2, in wcdma\api\rrcmmif.h;
     refLAC here is of uint16 */
  p_Asn1CellInfo->u.gsmCell->refLAC   = u_Lac0;
  p_Asn1CellInfo->u.gsmCell->refLAC <<= 8;
  p_Asn1CellInfo->u.gsmCell->refLAC  |= u_Lac1;

  /* MCC/MNC: convert to integers */
  if (!tm_umts_supl_wcdma_plmn_id_organizer(&z_PlmnId,
                                            &q_test1,
                                            &q_test2))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cached cell db plmn organizer failure RAT=: %u",
          z_cell_info_cached.e_ActiveRAT);
    return FALSE;
  }
  p_Asn1CellInfo->u.gsmCell->refMCC = (uint16)q_test1;
  p_Asn1CellInfo->u.gsmCell->refMNC = (uint16)q_test2;

  return TRUE;
}

/********************************************************************
 *
 * supl_cell_info_construct_tds_wcdma
 *
 * Function description:
 *   Construct cell information specific to active RAT = WCDMA.
 *
 * Parameters:
 *   p_Asn1CellInfo      : Pointer to a structure that contains
 *                         memory allocated for the ASN1 tool
 *
 *   p_CellInfoAllocated : Pointer to a structure to hold the
 *                         to-be-constructed cell information
 *
 *   q_CellId            : Cell Identity, range: (0...)
 *
 *   u_Lac0              : Location Area Code, byte 0
 *
 *   u_Lac1              : Location Area Code, byte 1
 *
 *   z_RrcPlmnId         : Structure that holds MCC/MNC parameters
 *
 * Return: TRUE/FALSE ( FALSE: no valid cell ID )
 *********************************************************************
 */
boolean supl_cell_info_construct_tds_wcdma
(
   supl_CellInfo *p_Asn1CellInfo,
   supl_cell_info_struct_type *p_CellInfoAllocated,
   uint32                      q_CellId,
   rrc_plmn_identity_type      z_RrcPlmnId
   )
{
  uint32                 q_test1, q_test2;
  rrc_plmn_identity_type z_PlmnId = z_RrcPlmnId;

  if ((NULL == p_Asn1CellInfo) || (NULL == p_CellInfoAllocated))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "supl_cell_info_construct_tds_wcdma: NULL arguments");
    return FALSE;
  }

  memset(&p_CellInfoAllocated->u1.wcdmaCell, 0, sizeof(supl_WcdmaCellInformation));
  memset(&p_CellInfoAllocated->u2.z_supl_wcdma_cell_info.z_frequency_info_fdd, 0,
         sizeof(supl_FrequencyInfoFDD));
  memset(&p_CellInfoAllocated->u2.z_supl_wcdma_cell_info.z_measured_results, 0,
         sizeof(supl_MeasuredResults));

  p_Asn1CellInfo->t           = C_SUPL_ASN1_CHOICE_WCDMA_CELL;
  p_Asn1CellInfo->u.wcdmaCell = &p_CellInfoAllocated->u1.wcdmaCell;

  /* Cell ID */
  p_Asn1CellInfo->u.wcdmaCell->refUC = q_CellId;

  /* MCC/MNC: convert to integers */
  if (!tm_umts_supl_wcdma_plmn_id_organizer(&z_PlmnId,
                                            &q_test1,
                                            &q_test2))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cached cell db plmn organizer failure RAT=: %u",
          z_cell_info_cached.e_ActiveRAT);
    return FALSE;
  }
  p_CellInfoAllocated->u1.wcdmaCell.refMCC = (uint16)q_test1;
  p_CellInfoAllocated->u1.wcdmaCell.refMNC = (uint16)q_test2;

  return TRUE;
}

/********************************************************************
 *
 * supl_cell_information_construct
 *
 * Function description:
 *   Construct cell information used by several SUPL messages.
 *
 * Parameters:
 *   p_asn1_cell_info      : pointer to a structure that contains 
 *                           memory allocated for the ASN1 tool
 *
 *   p_cell_info_allocated : pointer to a structure to hold the
 *                           to-be-constructed cell information
 *
 * Return: TRUE/FALSE ( FALSE: no valid cell ID )
 *********************************************************************
 */
static boolean supl_cell_information_construct(supl_CellInfo *p_asn1_cell_info,
                                               supl_cell_info_struct_type *p_cell_info_allocated)
{
  return tm_umts_up_supl_cell_info_construct(p_asn1_cell_info, p_cell_info_allocated);
}  

/********************************************************************
 *
 * supl_chk_and_build_all_mlid
 *
 * Function description:
 *  utility function to construct multiple location ID used by several
 *  SUPL messages.
 *
 * Parameters:
 *   p_supl_LocationIdData - pointer to a dynamically allocated array
 *      of structure of supl_LocationIdData.
 *
 *  p_supl_LocationId -- pointer to a dynamically allocated array
 *      of structure of supl_LocationData.
 *
 *  p_cell_info_allocated - pointer to a dynamically allocated array
 *     of structure supl_cell_info_struct_type.
 *
 *  u_num_allocated_array_elements - number of array elements allocated
 *     for each of the above three arrays.
 *
 * Return: number of Multiple Location ID entries
 *
 *********************************************************************
 */

static uint8  supl_chk_and_build_all_mlid(supl_LocationIdData *p_supl_LocationIdData,
                                          supl_LocationId *p_supl_LocationID,
                                          supl_cell_info_struct_type *p_cell_info_allocated,
                                          uint8 u_num_allocated_array_elements)
{
  return tm_umts_up_supl_chk_and_build_all_mlid(p_supl_LocationIdData, p_supl_LocationID, 
                                                p_cell_info_allocated, u_num_allocated_array_elements);
}

/********************************************************************
*
* supl_set_cf_state
*
* Function description:
*   this function properly sets SUPL call flow state machine states
*
* Parameters:
*  e_supl_cf_state [IN] -- SUPL call flow state to set to.
*  u_supl_cf_lm_timer_ms [IN] -- Set this given time for SUPL call flow timer.
*
* Return:
*  None
*
*********************************************************************
*/
void supl_set_cf_state(supl_cf_state_enum_type e_new_supl_cf_state, uint32 const cq_TimeoutMsec)
{
  supl_status_s_type *p_supl_session_status     = &z_supl_session_status;
  int16              x_temp                     = 0;
  uint8              u_TransmitDelayAdjRespTime = 0;
  uint8              u_GnssEngRespTime          = 0;
  uint8              u_temp2                    = 0;
  uint32             q_RemainTimeMsec = 0;

  if (e_new_supl_cf_state == C_SUPL_CF_STATE_NULL)
  {
    return;
  }

  /* note C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP state flag could be requested while in UT0, so
     don't terminate UT0 here */
  if (e_new_supl_cf_state != C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP)
  {
    /* LPP needs remaining SUPL CF time in C_SUPL_CF_STATE_SUPLPOS_XCHG state before stopping */
    q_RemainTimeMsec = os_TimerCheck(supl_cf_timer);

    /* For LPP don't stop the SUPL CF timer in C_SUPL_CF_STATE_SUPLPOS_XCHG state. Its decided in
    *  the below code.
    */
    if (!(((p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_SUPLPOS_XCHG) != 0) &&
          (TM_POS_PRTL_LPP == p_supl_session_status->u_position_protocol)))
    {
      (void)os_TimerStop(supl_cf_timer);
    }

  }

  if (e_new_supl_cf_state & C_SUPL_CF_STATE_CELL_INFO_REQ)
  {
    (void)os_TimerStart(supl_cf_timer, 3000, NULL); /* hardcode timer to 3 sec. */
  }
  else if ((e_new_supl_cf_state & C_SUPL_CF_STATE_UT0_SUPLSTART) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_UT0_SUPLPOSINIT) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_UT0_SUPLREPORT_QUERY)
           )
  {
    /* "UT0": waiting for TLS link to establish  */
    (void)os_TimerStart(supl_cf_timer, SUPL_TIMER_UT0_VALUE_MSEC, NULL);
  }
  else if (e_new_supl_cf_state & C_SUPL_CF_STATE_UT1)
  {
    /* use the NV value for UT1 */
    (void)os_TimerStart(supl_cf_timer, z_umts_nv_item_ram_copy.aagps_ut1_value * 1000, NULL);
  }
  else if ((e_new_supl_cf_state & C_SUPL_CF_STATE_UT2) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_UT10_INFO_QUERY_END)
           )
  {
    (void)os_TimerStart(supl_cf_timer, z_umts_nv_item_ram_copy.aagps_ut2_value * 1000, NULL);
  }
  else if ((e_new_supl_cf_state & C_SUPL_CF_STATE_UT3) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_UT3_RRLP) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_UT3_ACK)
           )
  {
    /* Reset LPP UP transaction timer id indexes when SUPL state machine enters UT3 */
    if ((TM_POS_PRTL_LPP == p_supl_session_status->u_position_protocol) &&
        ((e_new_supl_cf_state & C_SUPL_CF_STATE_UT3) != 0))
    {
      tm_lpp_up_reset_txtimerid_index();
    }
    (void)os_TimerStart(supl_cf_timer, z_umts_nv_item_ram_copy.aagps_ut3_value * 1000, NULL);
  }
  else if (e_new_supl_cf_state & C_SUPL_CF_STATE_SUPLPOS_XCHG)
  {
    /* make it one second earlier, for better chance of being received by SLP; and
       to avoid race condition with LM timeout */
    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
    {
      x_temp = p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_measure_response_time_sec - 1;

      if (x_temp >= 0)
      {
        u_TransmitDelayAdjRespTime = (uint8)x_temp;
      }
      else
      {
        u_TransmitDelayAdjRespTime = 0;
      }


      /* at this point u_gps_session_timeout_value_secs is set to the min of QoS timeout from UI
         and (u_measure_respone_time_sec - 2)*/
      u_GnssEngRespTime = (p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec / 1000);
      /* use the smaller of the timers for SUPL so we can send a non-DONE mesaurement
         also ensure  supl timer is atleast greater than the LM timer to avoid race condition */
      if (u_TransmitDelayAdjRespTime < u_GnssEngRespTime)
      {
        u_temp2 = u_TransmitDelayAdjRespTime;
      }
      else
      {
        /* make sure SUPL timer is atleast 1 second longer than the GNSSEngine/LM-timer but less
           than the N/W response time */
        if ((u_TransmitDelayAdjRespTime - u_GnssEngRespTime) > 1)
      {
          u_temp2 = u_GnssEngRespTime + 1;
      }
      else
      {
          u_temp2 = u_TransmitDelayAdjRespTime;
        }
      }

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL LM PRM Timer= %d, gps_sess_timer = %d, SUPL CF timer= %d", u_TransmitDelayAdjRespTime, u_GnssEngRespTime, u_temp2);
      (void)os_TimerStart(supl_cf_timer, u_temp2 * 1000, NULL);
    }
    else if (TM_POS_PRTL_LPP == p_supl_session_status->u_position_protocol)
    {
      /* Check if SUPL FSM re-enter the same C_SUPL_CF_STATE_SUPLPOS_XCHG state when SUPL CF timer
      *  is running to determine if the SUPL CF timer should be adjusted for the concurrent
      *  session
      */
      if (((z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_SUPLPOS_XCHG) != 0) &&
          (q_RemainTimeMsec != 0))
      {
        if (q_RemainTimeMsec >= cq_TimeoutMsec)
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Remaining SUPL CF timeout %lu sufficient to cover new response time",
                q_RemainTimeMsec);
          return;
        }
      }

      /* Start the SUPl CF timer */
      if (cq_TimeoutMsec != 0)
      {
        os_TimerStop(supl_cf_timer);
        (void)os_TimerStart(supl_cf_timer, cq_TimeoutMsec, NULL);
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL CF timeout %lu msec", cq_TimeoutMsec, 0, 0);
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No timeout value for SUPL CF timer");
      }
    }
  }
  else if ((e_new_supl_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_SUPLSTART))
  {
    /* note C_SUPL_CF_STATE_WIFI_WAIT_SUPLSTART only sets timer if system never entered C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP */
    (void)os_TimerStart(supl_wifi_wait_timer, tm_core_info.config_info.wifi_wait_timeout_sec * 1000, NULL);
  }
  else if ((e_new_supl_cf_state & C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART) ||
           (e_new_supl_cf_state & C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT))
  {
    /*Start a 100ms timer to retry LTE cell info get*/
    (void)os_TimerStart(supl_lte_get_cell_info_retry_timer, 100, NULL);
  }
  p_supl_session_status->e_cf_state |= e_new_supl_cf_state;
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL CF State: 0x%x", p_supl_session_status->e_cf_state, 0, 0);
}


/*
******************************************************************************
* supl_store_slp_sessionId
*
* Function description:
*   Stores the incoming SLP session ID
*
* Parameters:
*  pointer to supl_ULP_PDU
*
* Return value:
*  None
*
*
******************************************************************************
*/
static void supl_store_slp_sessionId(const supl_ULP_PDU *p_pdu)

{
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  supl_session_id      *p_session_id  = &p_supl_session_status->z_session_id;

  if (p_pdu->sessionID.m.slpSessionIDPresent)
  {
    supl_util_server_session_id_organizer(&p_pdu->sessionID, &p_session_id->z_Slp_Sess_Id);
  } /* close if-SLP-sessionID present */

}


/*
******************************************************************************
* supl_util_server_session_id_organizer
*
* Function description:
*   This function re-packages server session ID
*
* Parameters:
*
* p_source_session_id : pointer to a structure holding decoded SessionID from
*   a source that is an IE in an incoming SUPL message.
*
* p_slp_session_id : pointer for the function to pass repackaged
*   slp session ID to the calling function.
*
* Return value:
*  None
*
*
******************************************************************************
*/

static void supl_util_server_session_id_organizer(const supl_SessionID *p_source_session_id,
                                                  supl_slp_session_id *p_slp_session_id)
{

  memset(p_slp_session_id, 0, sizeof(supl_slp_session_id));

  if (p_source_session_id->m.slpSessionIDPresent == 1)
  {
    p_slp_session_id->presence = TRUE;
  }
  else
  {
    return;
  }

  /* Copy Session ID */
  memscpy(p_slp_session_id->session_Id, sizeof(p_slp_session_id->session_Id),
          p_source_session_id->slpSessionID.sessionID.data,
          sizeof(p_source_session_id->slpSessionID.sessionID.data));

  if (p_source_session_id->slpSessionID.slpId.t == C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS)
  {
    /* IP Address */
    p_slp_session_id->zSlpAddress.choice = C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS;

    if (p_source_session_id->slpSessionID.slpId.u.iPAddress->t == C_SUPL_ASN1_CHOICE_IPV4ADDRESS)
    {
      /* IPV4 */
      p_slp_session_id->zSlpAddress.u.z_Ip_Address.choice = C_SUPL_ASN1_CHOICE_IPV4ADDRESS;
      p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.numocts = 4;
      memscpy(p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.data,
              sizeof(p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.data),
              p_source_session_id->slpSessionID.slpId.u.iPAddress->u.ipv4Address->data,
              sizeof(p_source_session_id->slpSessionID.slpId.u.iPAddress->u.ipv4Address->data));
    }
    else
    {
      /* IPV6 */
      p_slp_session_id->zSlpAddress.u.z_Ip_Address.choice = C_SUPL_ASN1_CHOICE_IPV6ADDRESS;
      p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.numocts = 16;
      memscpy(p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.data,
              sizeof(p_slp_session_id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.data),
              p_source_session_id->slpSessionID.slpId.u.iPAddress->u.ipv6Address->data,
              sizeof(p_source_session_id->slpSessionID.slpId.u.iPAddress->u.ipv6Address->data));
    }
  }
  else
  {

    /* FQDN */
    p_slp_session_id->zSlpAddress.choice = C_SUPL_ASN1_CHOICE_SLPADRESS_FQDN;

    GNSS_STRLCPY(p_slp_session_id->zSlpAddress.u.z_fqdn.fqdn,
                 p_source_session_id->slpSessionID.slpId.u.fQDN,
                 C_SUPL_FQDN_VISIBLE_STRING_BYTE_LENGTH - 1);

    // NULL terminate
    p_slp_session_id->zSlpAddress.u.z_fqdn.fqdn[C_SUPL_FQDN_VISIBLE_STRING_BYTE_LENGTH - 1] = NULL;
    p_slp_session_id->zSlpAddress.u.z_fqdn.u_length = strlen(p_slp_session_id->zSlpAddress.u.z_fqdn.fqdn);
  }
}

/*
******************************************************************************
* supl_set_session_id_organizer
*
* Function description:
*   This function re-packages set session ID
*
* Parameters:
*
* p_pdu_decoded : pointer to a structure holding decoded SUPL PDU
*
* p_set_session_id : pointer for the function to pass repackaged
*   set session ID to the calling function.
*
* Return value:
*  None
*
*
******************************************************************************
*/

static void supl_set_session_id_organizer(const supl_SessionID *p_source_session_id,
                                          supl_set_session_id *p_set_session_id)
{
  uint32 choice;
  uint32 cnt;

  p_set_session_id->w_Session_Id = p_source_session_id->setSessionID.sessionId;

  choice = p_source_session_id->setSessionID.setId.t;
  p_set_session_id->z_Set_Id.set_id_choice = choice;

  /* We only support MSISDN and IMSI */
  if (choice == C_SUPL_ASN1_CHOICE_SETID_MSISDN)
  {
    uint8 *p_src = p_source_session_id->setSessionID.setId.u.msisdn->data;
    uint8 *p_dest = p_set_session_id->z_Set_Id.u.msisdn.data;

    /* MSISDN */
    for (cnt = 8; cnt; cnt--)
    {
      *p_dest++ = *p_src++;
    }
    p_set_session_id->z_Set_Id.u.msisdn.numocts = 8;
  }
  else if (choice == C_SUPL_ASN1_CHOICE_SETID_IMSI)
  {
    uint8 *p_src = p_source_session_id->setSessionID.setId.u.imsi->data;
    uint8 *p_dest = p_set_session_id->z_Set_Id.u.imsi.data;

    /* IMSI */
    for (cnt = 8; cnt; cnt--)
    {
      *p_dest++ = *p_src++;
    }
    p_set_session_id->z_Set_Id.u.imsi.numocts = 8;
  }
  else
  /* For any other setID choice a SUPL End msg will be generated with invalid session ID error.
     Save the raw contents of setID for this case too, so that it can be returned in SUPL End. */
  {
    uint8 *p_src = (uint8 *)&p_source_session_id->setSessionID.setId.u;
    uint8 *p_dest = &p_set_session_id->z_Set_Id.u.other_setID.raw_contents[0];

    for (cnt = OTHER_SETID_SIZE; cnt; cnt--)
    {
      *p_dest++ = *p_src++;
    }
    p_set_session_id->z_Set_Id.u.other_setID.valid = TRUE;
  }
}



/*
******************************************************************************
* supl_message_overhead_proc
*
* Function description:
*   This function checks the overhead of SUPL messages and
*   indicates to the calling function if it has passed the check.
*
* Parameters:
*
* p_pdu_decoded : pointer to a structure holding decoded SUPL PDU
*
* Return value:
*  slp_mess_header_check_result_e_type
*
******************************************************************************
*/

static slp_mess_header_check_result_e_type  supl_message_overhead_proc
(const supl_ULP_PDU *p_pdu_decoded,
 uint16 *p_provided_set_session_id
 )

{
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;
  supl_session_id *p_session_id  = &p_supl_session_status->z_session_id;
  uint8                   u_session_id_consistent = TRUE;

  supl_slp_session_id  z_incoming_slp_session_id;
  supl_set_session_id  z_incoming_set_session_id;

  slp_mess_header_check_result_e_type  e_ret;

  uint16               *p_set_session_id;



  e_ret = C_SLP_MESS_HEADER_STATUS_UNKNOWN;

  if (p_provided_set_session_id == NULL)
  {
    p_set_session_id = &p_supl_session_status->z_session_id.z_Set_Sess_Id.w_Session_Id;
  }
  else
  {
    p_set_session_id = p_provided_set_session_id;
  }


  memset(&z_incoming_slp_session_id, 0, sizeof(supl_slp_session_id));


  /* any message coming from SLP must contain SLP session ID */
  if (!p_pdu_decoded->sessionID.m.slpSessionIDPresent)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Missing SlpSessionID", 0, 0, 0);
    e_ret |= C_SLP_MESS_SLP_SESS_ID_MISSING;
  }

  /******************* Check SLP Session ID *****************************************/

  /* organized SLP session ID and place it in z_incoming_slp_session_id */
  if (p_pdu_decoded->sessionID.m.slpSessionIDPresent)
  {
    supl_util_server_session_id_organizer(&p_pdu_decoded->sessionID, &z_incoming_slp_session_id);
  }


  /* Save SLP session ID, even if the message is to be rejected using SUPL_END, since
     SULP_END needs this SLP_session_ID too. Note for SUPLINIT, no such check should be
     done, as it will be the first message on have SLP session ID. No need to check
     SUPL_TRIGGERED_STOP either.  */

  if ((p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT) &&
      (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLTRIGGEREDSTOP)
      )
  {
    if (supl_SLP_session_id_match_check(&p_session_id->z_Slp_Sess_Id,
                                        &z_incoming_slp_session_id) == FALSE
        )
    {
      /* Callflow termination handled in this routine */
      e_ret |= C_SLP_MESS_SLP_SESS_ID_PROBLEM;
    }
    else
    {
      e_ret = C_SLP_MESS_HEADER_OK_CONSISTENT;
    }


  } /* close if-not-SuplInit or SUPL_TRIGGERED_STOP */

  else /* for SuplInit or SUPL_TRIGGERED_STOP*/
  {
    if (e_ret == C_SLP_MESS_HEADER_STATUS_UNKNOWN)
    {
      e_ret = C_SLP_MESS_HEADER_OK_NEW;
    }
  }


  /******************************* check SET session ID *****************************/
  u_session_id_consistent = TRUE;

  if ((p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT) &&
      (p_pdu_decoded->message.t != C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLTRIGGEREDSTOP)
      )
  {

    supl_set_session_id_organizer(&p_pdu_decoded->sessionID, &z_incoming_set_session_id);


    /* anything other than SUPLINIT, incoming message must contain SetSessionId */
    if (!p_pdu_decoded->sessionID.m.setSessionIDPresent)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
            "SUPL: SLP mess. missing SetSessionID: %u",
            p_pdu_decoded->sessionID.m.setSessionIDPresent);

      e_ret |= C_SLP_MESS_SET_SESS_ID_MISSING;
      return e_ret;

    }

    /* anything other than SUPLINIT, incoming SET Session ID should match what the SET has */
    if (*p_set_session_id !=
           z_incoming_set_session_id.w_Session_Id)
    {
      u_session_id_consistent = FALSE;

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: SLP mess SET SessID mismatch %u  (%u)",
            z_incoming_set_session_id.w_Session_Id,
            *p_set_session_id, 0);

    }

    if (z_incoming_set_session_id.z_Set_Id.set_id_choice == C_SUPL_ASN1_CHOICE_SETID_MSISDN)
    {
      /* Verify MSISDN */
      if (memcmp(z_incoming_set_session_id.z_Set_Id.u.msisdn.data,
                 z_Set_Id.u.msisdn.data,
                 8))
      {
        u_session_id_consistent = FALSE;
      }
    }
    else if (z_incoming_set_session_id.z_Set_Id.set_id_choice == C_SUPL_ASN1_CHOICE_SETID_IMSI)
    {
      /* Verify IMSI */
      if (memcmp(z_incoming_set_session_id.z_Set_Id.u.imsi.data,
                 z_Set_Id.u.imsi.data,
                 8))
      {
        u_session_id_consistent = FALSE;
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "IMSI mismatch: %u, %u", z_incoming_set_session_id.z_Set_Id.u.imsi.data[7],
              z_Set_Id.u.imsi.data[7],
              0);
      }
    }
    else
    {
      /* Unsupported choice */
      u_session_id_consistent = FALSE;
    }

    if (!u_session_id_consistent)
    {
      e_ret |= C_SLP_MESS_SET_SESS_ID_PROBLEM;

      /* use this in SUPL_END */
      memscpy(&p_supl_session_status->z_session_id.z_Set_Sess_Id,
              sizeof(p_supl_session_status->z_session_id.z_Set_Sess_Id),
              &z_incoming_set_session_id, sizeof(z_incoming_set_session_id));

    }
    else
    {
      if (e_ret == C_SLP_MESS_HEADER_STATUS_UNKNOWN)
      {
        e_ret = C_SLP_MESS_HEADER_OK_CONSISTENT;
      }
    }

  } /* close if-not-SuplInit or SUPL_TRIGGERED_STOP*/

  else if (p_pdu_decoded->message.t == C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLINIT) /* for SuplInit: */
  {
    /* SUPLINIT must NOT contain SetSessionId */
    if (p_pdu_decoded->sessionID.m.setSessionIDPresent)
    {
      e_ret |= C_SLP_MESS_SUPLINIT_SET_SESS_ID_PROBLEM;
    }
  }

  return e_ret;

}



/*
******************************************************************************
* supl_send_notification_verifcation
*
* Function description:
*   LCS Agent's sending of notification verification to PDAPI
*
* Parameters:
*
*  : pointer to a structure holding decoded Supl PDU
*
* Return value:
*  None
*
*
******************************************************************************
*/
static void supl_send_notification_verifcation
(
   const supl_ULP_PDU *p_pdu_decoded,
   const uint8 *p_hash
   )
{

  pdsm_lcs_info_s_type z_notification_info;

  pdsm_lcs_supl_notify_verify_req_s_type *p_supl_pdapi_notif_verif =
     &z_notification_info.lcs_info.supl_req;

  const supl_SUPLINIT *p_suplInit;
  uint8 length = 0;

  supl_slp_session_id      z_local_copy_slp_session_id;

  p_suplInit = p_pdu_decoded->message.u.msSUPLINIT;

  /* initialize the notification info. structure */
  memset(&z_notification_info, 0, sizeof(pdsm_lcs_info_s_type));

  z_notification_info.client_id = pdsm_ni_client_id_get();
  z_notification_info.lcs_cb_cmd_type = PDSM_LCS_CMD_TYPE_SUPL_REQ;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "send_notif_verif: clientID=%d", z_notification_info.client_id, 0, 0);

  /* check if optional notification/verification IE is present */
  if (p_suplInit->m.notificationPresent == 0)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_INIT has No Notification IE", 0, 0, 0);

    /* for all practical purposes, treat this the same as a notification type
       of no-notification-no-verification */
    p_supl_pdapi_notif_verif->notification_priv_type = PDSM_LCS_NO_NOTIFY_NO_VERIFY;

  }

  else /* notif/verif IE is present */
  {

    /* notification_priv_type */
    switch (p_suplInit->notification.notificationType)
    {
      case supl_noNotificationNoVerification:
        {
          p_supl_pdapi_notif_verif->notification_priv_type = PDSM_LCS_NO_NOTIFY_NO_VERIFY;
          break;
        }

      case supl_notificationOnly:
        {
          p_supl_pdapi_notif_verif->notification_priv_type = PDSM_LCS_USER_NOTIFY_ONLY;
          break;
        }

      case supl_notificationAndVerficationAllowedNA:
        {
          p_supl_pdapi_notif_verif->notification_priv_type = PDSM_LCS_USER_NOTIFY_VERIFY_ALLOW_NO_RESP;
          break;
        }

      case supl_notificationAndVerficationDeniedNA:
        {
          p_supl_pdapi_notif_verif->notification_priv_type = PDSM_LCS_USER_NOTIFY_VERIFY_NOT_ALLOW_NO_RESP;
          break;
        }

      case supl_privacyOverride:
        {
          p_supl_pdapi_notif_verif->notification_priv_type = PDSM_LCS_PRIVACY_OVERRIDE;
		  z_supl_session_status.v_privacy_override_set = TRUE;
		  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL session with Privacy Override set");
          break;
        }

      default:
        {
          /* should not happen */
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Unknown Notif. type received. Denying request", 0, 0, 0);
          break;
        }

    } /* close notification_priv_type */

  } /* close else ( if notification_priv_type is present ) */

  p_supl_pdapi_notif_verif->flags = 0;

  /* pass up the emergency services bit and SLP address */
  if ((p_suplInit->notification.m.ver2_Notification_extensionPresent == 1) &&
      (p_suplInit->notification.ver2_Notification_extension.m.emergencyCallLocationPresent == 1))
  {
    p_supl_pdapi_notif_verif->flags |= PDSM_LCS_SUPLINIT_VER2_EXT_EMERGENCY_PRESENT;

    if ((p_suplInit->m.ver2_SUPL_INIT_extensionPresent == 1) &&
        (p_suplInit->ver2_SUPL_INIT_extension.m.e_SLPAddressPresent == 1))
    {
      if (p_suplInit->ver2_SUPL_INIT_extension.e_SLPAddress.t == 2)
      {
        p_supl_pdapi_notif_verif->supl_init_ver2_ext_emergency.b_eslp_address_present = TRUE;
        p_supl_pdapi_notif_verif->supl_init_ver2_ext_emergency.e_slp_address.choice = PDSM_SERVER_TYPE_URL_ADDRESS;
        GNSS_STRLCPY(p_supl_pdapi_notif_verif->supl_init_ver2_ext_emergency.e_slp_address.supl_slp_address.server_adrs_url.url,
                     p_suplInit->ver2_SUPL_INIT_extension.e_SLPAddress.u.fQDN,
                     C_SUPL_FQDN_VISIBLE_STRING_BYTE_LENGTH - 1);
        // NULL terminate
        p_supl_pdapi_notif_verif->supl_init_ver2_ext_emergency.e_slp_address.supl_slp_address.server_adrs_url.url[C_SUPL_FQDN_VISIBLE_STRING_BYTE_LENGTH - 1] = NULL;

        p_supl_pdapi_notif_verif->supl_init_ver2_ext_emergency.e_slp_address.supl_slp_address.server_adrs_url.url_length = strlen(p_suplInit->ver2_SUPL_INIT_extension.e_SLPAddress.u.fQDN);
      }
    }
  }
  /* encoding type */
  if (p_suplInit->notification.m.encodingTypePresent)
  {
    p_supl_pdapi_notif_verif->flags |= PDSM_LCS_ENCODING_TYPE_PRESENT;

    switch (p_suplInit->notification.encodingType)
    {
      case supl_ucs2:
        p_supl_pdapi_notif_verif->datacoding_scheme = PDSM_SUPL_UCS2;
        break;
      case supl_gsmDefault:
        p_supl_pdapi_notif_verif->datacoding_scheme = PDSM_SUPL_GSM_DEFAULT;
        break;
      case supl_utf8:
        p_supl_pdapi_notif_verif->datacoding_scheme = PDSM_SUPL_UTF8;
        break;
      default:
        p_supl_pdapi_notif_verif->datacoding_scheme = PDSM_SUPL_NOTIF_DISPLAYENCODINGTYPE_UNKNOWN_ENUMERATOR;
        break;
    }
  } /* close encoding-type */

  /* RequestorID */
  if (p_suplInit->notification.m.requestorIdPresent)
  {
    length = p_suplInit->notification.requestorId.numocts;
    p_supl_pdapi_notif_verif->flags |= PDSM_LCS_REQUESTOR_ID_PRESENT;

    /* make sure we don't exceed the local buffer size */
    if (length > PDSM_LCS_REQUESTOR_ID_LEN)  /* pdapi defines requestID array limit, currently 63 bytes */
    {
      length = PDSM_LCS_REQUESTOR_ID_LEN;
    }
    if (length >= ARR_SIZE(p_suplInit->notification.requestorId.data))
    {
      /* SUPL defines this size of 50 */
      length = ARR_SIZE(p_suplInit->notification.requestorId.data);
    }

    /*  copy the string locally */
    memscpy((void *)&p_supl_pdapi_notif_verif->requestor_id.requestor_id_string[0],
            sizeof(p_supl_pdapi_notif_verif->requestor_id.requestor_id_string),
            p_suplInit->notification.requestorId.data, length);

    p_supl_pdapi_notif_verif->requestor_id.requestor_id_string[PDSM_LCS_REQUESTOR_ID_LEN - 1] = NULL;
    p_supl_pdapi_notif_verif->requestor_id.string_len = length;

  } /* close RequestorID */

  /* RequestorIDType */
  if (p_suplInit->notification.m.requestorIdTypePresent)
  {
    p_supl_pdapi_notif_verif->requestor_id.data_coding_scheme =
       (byte)supl_convert_format_indicator_pdsm(p_suplInit->notification.requestorIdType);
  } /* close RequestorIDType */

  /* ClientName */
  if (p_suplInit->notification.m.clientNamePresent)
  {
    length = p_suplInit->notification.clientName.numocts;
    p_supl_pdapi_notif_verif->flags |= PDSM_LCS_CLIENT_NAME_PRESENT;

    /* make sure we don't exceed the local buffer size */
    if (length > PDSM_LCS_CLIENT_NAME_LEN) /* pdapi defines clientName array to be of 63 bytes */
    {
      length = PDSM_LCS_CLIENT_NAME_LEN;
    }
    if (length >= ARR_SIZE(p_suplInit->notification.clientName.data))
    {
      /* SUPL defines this size of 50 */
      length = ARR_SIZE(p_suplInit->notification.clientName.data);
    }

    /* copy the string locally */
    memscpy((void *)&p_supl_pdapi_notif_verif->client_name.client_name_string[0],
            sizeof(p_supl_pdapi_notif_verif->client_name.client_name_string),
            p_suplInit->notification.clientName.data, length);

    /* null terminate */
    p_supl_pdapi_notif_verif->client_name.client_name_string[PDSM_LCS_CLIENT_NAME_LEN - 1] = NULL;

    /* set length */
    p_supl_pdapi_notif_verif->client_name.string_len = length;

  } /* close ClientName */

  /* ClientName Type */
  if (p_suplInit->notification.m.clientNameTypePresent)
  {
    p_supl_pdapi_notif_verif->client_name.data_coding_scheme =
       (byte)supl_convert_format_indicator_pdsm(p_suplInit->notification.clientNameType);
  } /* close ClientName Type */


  /* convert slp session id into the proper format and populate the structure for pdapi */
  supl_util_server_session_id_organizer(&p_pdu_decoded->sessionID, &z_local_copy_slp_session_id);

  if (z_local_copy_slp_session_id.presence)
  {
    p_supl_pdapi_notif_verif->supl_slp_session_id.presence = TRUE;

    memscpy(&p_supl_pdapi_notif_verif->supl_slp_session_id.session_id[0],
            sizeof(p_supl_pdapi_notif_verif->supl_slp_session_id.session_id),
            &z_local_copy_slp_session_id.session_Id[0],
            sizeof(z_local_copy_slp_session_id.session_Id));

    if (z_local_copy_slp_session_id.zSlpAddress.choice == C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS)
    {
      if (z_local_copy_slp_session_id.zSlpAddress.u.z_Ip_Address.choice == C_SUPL_ASN1_CHOICE_IPV4ADDRESS)
      {
        p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.choice = PDSM_SERVER_TYPE_IPV4_ADDRESS;

        memscpy(&p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v4.server_adrs,
                sizeof(p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v4.server_adrs),
                &z_local_copy_slp_session_id.zSlpAddress.u.z_Ip_Address.u.Ip_V4.data[0],
                sizeof(z_local_copy_slp_session_id.zSlpAddress.u.z_Ip_Address.u.Ip_V4.data)
                );
      }
      else if (z_local_copy_slp_session_id.zSlpAddress.u.z_Ip_Address.choice == C_SUPL_ASN1_CHOICE_IPV6ADDRESS)
      {
        p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.choice = PDSM_SERVER_TYPE_IPV6_ADDRESS;
        memscpy(&p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v6.server_adrs[0],
                sizeof(p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v6.server_adrs),
                &z_local_copy_slp_session_id.zSlpAddress.u.z_Ip_Address.u.Ip_V6.data[0],
                sizeof(z_local_copy_slp_session_id.zSlpAddress.u.z_Ip_Address.u.Ip_V6.data)
                );
      }
      else
      {
        /* should never happen, as it wouldn't pass ASN1 decoder */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: invalid SLP IP_address type", 0, 0, 0);
      }
    } /* if slp address is of ip_address */

    else if (z_local_copy_slp_session_id.zSlpAddress.choice == C_SUPL_ASN1_CHOICE_SLPADRESS_FQDN)
    {
      p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.choice = PDSM_SERVER_TYPE_URL_ADDRESS;

      p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_url.url_length =
         z_local_copy_slp_session_id.zSlpAddress.u.z_fqdn.u_length;

      memscpy(&p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_url.url[0],
              sizeof(p_supl_pdapi_notif_verif->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_url.url),
              &z_local_copy_slp_session_id.zSlpAddress.u.z_fqdn.fqdn[0],
              z_local_copy_slp_session_id.zSlpAddress.u.z_fqdn.u_length
              );
    }
    else
    {
      /* should never happen, as it wouldn't pass ASN1 decoder */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: invalid SLP Address choice", 0, 0, 0);
    }

  } /* if SLP ID present */

  if (p_suplInit->posMethod == supl_PosMethod_agpsSETassisted)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETASSISTED;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_agpsSETbased)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETBASED;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_agpsSETassistedpref)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETASSISTED_PREF;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_agpsSETbasedpref)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETBASED_PREF;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_autonomousGPS)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AUTONOMOUS_GPS;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_aFLT)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AFLT;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_eCID)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_ECID;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_eOTD)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_EOTD;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_oTDOA)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_OTDOA;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_noPosition)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_NO_POSITION;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETbased)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETBASED;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETassisted)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETASSISTED;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETbasedpref)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETBASED_PREF;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETassistedpref)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AGPS_SETASSISTED_PREF;
  }
  else if (p_suplInit->posMethod == supl_PosMethod_ver2_autonomousGNSS)
  {
    p_supl_pdapi_notif_verif->pos_method = POSMETHOD_AUTONOMOUS_GPS;
  }

  else
  {
    /* supl_PosMethod_ver2_sessioninfoquery of supl_PosMethod_Ext which
       is supported on this version does not go to pdapi anyway; */
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "posMethod not for NotifVerif: %d",
          p_suplInit->posMethod);
    return;
  }

  /* Set the ver2 ext present unconditionally, so we can send a GLO gnss tech 
   * mask in the notif-verify even if server did not ask */
  p_supl_pdapi_notif_verif->flags |= PDSM_LCS_SUPLINIT_VER2_EXT_PRESENT;
  /* Set the constellations the HSLP Supports. This will be used when requesting
   * Assistance Data in SUPL_POS_INIT */
  p_supl_pdapi_notif_verif->supl_init_ver2_ext.validity |= PDAPI_SUPLINIT_VER2_EXT_BITMAP_GNSSPOSTECH_VALID;
  p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech = 0x0;

  if ((p_suplInit->posMethod == supl_PosMethod_agpsSETassisted) ||
      (p_suplInit->posMethod == supl_PosMethod_agpsSETbased) ||
      (p_suplInit->posMethod == supl_PosMethod_agpsSETassistedpref) ||
      (p_suplInit->posMethod == supl_PosMethod_agpsSETbasedpref) ||
      (p_suplInit->posMethod == supl_PosMethod_autonomousGPS)
      )
  {
    p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech |= PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_GPS;
  }

  /* Determine if the H-SLP supports any GANNS capability */
  if ((p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETassisted) ||
      (p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETbased) ||
      (p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETassistedpref) ||
      (p_suplInit->posMethod == supl_PosMethod_ver2_agnssSETbasedpref) ||
      (p_suplInit->posMethod == supl_PosMethod_ver2_autonomousGNSS)
      )
  {

    if ((p_suplInit->m.ver2_SUPL_INIT_extensionPresent) &&
        (p_suplInit->ver2_SUPL_INIT_extension.m.gnssPosTechnologyPresent))
    {
      if (p_suplInit->ver2_SUPL_INIT_extension.gnssPosTechnology.gps)
      {
        p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech |= PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_GPS;
      }
      if (p_suplInit->ver2_SUPL_INIT_extension.gnssPosTechnology.glonass)
      {
        p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech |= PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_GLONASS;
      }
      if (p_suplInit->ver2_SUPL_INIT_extension.gnssPosTechnology.bds)
      {
        p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech |= PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_BDS;
      }
    }
    else
    {
      /* For NI callflows, some servers on some carriers dont send us the 
       * ver2_SUPL_INIT_extension because of buggy devices.
       * Therefore, we assume that they support Glo if the ext is not present. 
       * But if they do send us the ver2_SUPL_INIT_extension, we use the capability
       * mentioned within. Therefore set GLO mask to on 
       * To preserve legacy behavior, if server requests AGNSS, but does not
       * provide SUPL INIT Extension, assume GPS and GLONASS are supported */
      p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech |= PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_GPS;
      p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech |= PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_GLONASS;
    }
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SLP Supports gnss post tech mask 0x%x GPS(0x1) GLO (0x20) BDS (0x40)",
        p_supl_pdapi_notif_verif->supl_init_ver2_ext.gnss_pos_tech);


  if (p_suplInit->m.qoPPresent)
  {
    p_supl_pdapi_notif_verif->flags |= PDSM_LCS_SUPL_QOP_VALID;

    p_supl_pdapi_notif_verif->supl_qop.horacc = (uint8)p_suplInit->qoP.horacc;

    if (p_suplInit->qoP.m.veraccPresent)
    {
      p_supl_pdapi_notif_verif->supl_qop.bit_mask = PDSM_LCS_SUPL_QOP_VERACC_VALID;
      p_supl_pdapi_notif_verif->supl_qop.veracc = (uint8)p_suplInit->qoP.veracc;
    }
    if (p_suplInit->qoP.m.maxLocAgePresent)
    {
      p_supl_pdapi_notif_verif->supl_qop.bit_mask |= PDSM_LCS_SUPL_QOP_MAXAGE_VALID;
      p_supl_pdapi_notif_verif->supl_qop.maxLocAge = (uint16)p_suplInit->qoP.maxLocAge;
    }
    if (p_suplInit->qoP.m.delayPresent)
    {
      p_supl_pdapi_notif_verif->supl_qop.bit_mask |= PDSM_LCS_SUPL_QOP_DELAY_VALID;
      p_supl_pdapi_notif_verif->supl_qop.delay = (uint8)p_suplInit->qoP.delay;
    }
  } /* if SuplInit contains the optional QoP */


  /* if trigger type present */
  if (p_suplInit->ver2_SUPL_INIT_extension.m.triggerTypePresent)
  {
    if (p_suplInit->ver2_SUPL_INIT_extension.triggerType ==
           supl_TriggerType_periodic)
    {
      p_supl_pdapi_notif_verif->supl_init_ver2_ext.trigger_type =
         PDAPI_SUPLINIT_TRIGGER_TYPE_PERIODIC;
    }
    else if (p_suplInit->ver2_SUPL_INIT_extension.triggerType ==
                supl_TriggerType_areaEvent)
    {
      p_supl_pdapi_notif_verif->supl_init_ver2_ext.trigger_type =
         PDAPI_SUPLINIT_TRIGGER_TYPE_AREA_EVENT;
    }

    if ((p_suplInit->ver2_SUPL_INIT_extension.triggerType ==
            supl_TriggerType_periodic) ||
        (p_suplInit->ver2_SUPL_INIT_extension.triggerType ==
            supl_TriggerType_areaEvent))
    {

      /* set validity for trigger type to valid */
      p_supl_pdapi_notif_verif->supl_init_ver2_ext.validity |=
         PDAPI_SUPLINIT_VER2_EXT_BITMAP_TRIGTYPE_VALID;

      /* set flag for SUPLINIT_VER2_EXT_PRESENT */
      p_supl_pdapi_notif_verif->flags |= PDSM_LCS_SUPLINIT_VER2_EXT_PRESENT;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Unrecongnized trigger type with value %d, ignored",
            p_suplInit->ver2_SUPL_INIT_extension.triggerType, 0, 0);
    }

  }  /* if trigger type present */


  p_supl_pdapi_notif_verif->user_response_timer = 2; /* QQQ: arbitary for now */

  p_supl_pdapi_notif_verif->supl_init_source = z_supl_init_source; // SUPL Init Source (1x-SMS/GSM-SMS/UDP/WapPush)
  z_supl_init_source = PDSM_SUPL_INIT_SOURCE_TYPE_UNKNOWN;

  memscpy(p_supl_pdapi_notif_verif->supl_hash, sizeof(p_supl_pdapi_notif_verif->supl_hash), p_hash, sizeof(p_supl_pdapi_notif_verif->supl_hash));


  (void)tm_request_app_response(TM_PRTL_TYPE_UMTS_UP,
                                PDSM_LCS_EVENT_SUPL_NOTIFY_VERIFY_REQ,
                                &z_notification_info
                                );

  //lsm_log_notification_verification_request(&z_notification_info);    /* QQQ */

  //lsm_dm_ui_req(z_notification_info.client_id, &z_notification_info);


}


/*
******************************************************************************
* supl_convert_format_indicator_pdsm
*
* Function description:
*   Helper function to convert SUPL's FormatIndicator to the PDSM defined ones.
*
* Parameters:
*
*  : supl_FormatIndicator to be converted to pdsm_lcs_notify_verify_e_type
*
* Return value:
*  pdsm_lcs_supl_format_indicator_e_type
*
*
******************************************************************************
*/
static pdsm_lcs_supl_format_indicator_e_type supl_convert_format_indicator_pdsm(supl_FormatIndicator formatInd)
{
  pdsm_lcs_supl_format_indicator_e_type    lcsFormatInd = PDSM_SUPL_FORMAT_OSS_UNKNOWN;

  switch (formatInd)
  {
    case supl_logicalName:
      lcsFormatInd = PDSM_SUPL_FORMAT_LOGICAL_NAME;
      break;
    case supl_e_mailAddress:
      lcsFormatInd = PDSM_SUPL_FORMAT_EMAIL_ADDRESS;
      break;
    case supl_msisdn:
      lcsFormatInd = PDSM_SUPL_FORMAT_MSISDN;
      break;
    case supl_url:
      lcsFormatInd = PDSM_SUPL_FORMAT_URL;
      break;
    case supl_sipUrl:
      lcsFormatInd = PDSM_SUPL_FORMAT_SIP_URL;
      break;
    case supl_min:
      lcsFormatInd = PDSM_SUPL_FORMAT_MIN;
      break;
    case supl_mdn:
      lcsFormatInd = PDSM_SUPL_FORMAT_MDN;
      break;
    case supl_iMSPublicidentity:
      lcsFormatInd = PDSM_SUPL_FORMAT_IMSPUBLIC_IDENTITY;
      break;
    default:
      lcsFormatInd = PDSM_SUPL_FORMAT_OSS_UNKNOWN;
      break;
  }

  return lcsFormatInd;
}



/********************************************************************
*
* supl_action_upon_ni_notif_verif_accepted
*
* Function description:
*   actions to be taken once an NI request SUPL_INIT is accepted
*   by the OEM layer.
*
* Parameters:
*   p_req_data : pointer to a structure holding information from
*   pdapi that is the part of SUPL_INIT needed in the response to
*   be sent to the SLP.
* 
* Return:
*  None
*
*********************************************************************
*/

static void supl_action_upon_ni_notif_verif_accepted
(
   pdsm_lcs_supl_notify_verify_req_s_type *p_req_data,
   supl_slp_session_id                    *p_slp_id
   )
{

  supl_status_s_type *p_supl_session_status = &z_supl_session_status;
  uint8                 u_hor_acc_threshold = 0;
  tm_cm_phone_state_info_s_type phone_state_info;

  supl_init_trigger_type_enum_type e_trigger_type;

#if 0  /* Use these if direct call to start session if the concept of
            "network-owned" session is to be implemented */
  tm_sess_req_param_u_type z_tm_core_req_param;
  tm_sess_req_param_u_type *p_tm_core_req_param = &z_tm_core_req_param;
#endif

  if (p_req_data ==  NULL || p_slp_id == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: attempt to dereference NULL pointer", 0, 0, 0);
    return;
  }

  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  	  /* Handle the case where the request is for "no position", for which all the network needs
     is an accept or reject and no further action is needed */
  if (p_req_data->pos_method == POSMETHOD_NO_POSITION)
  {
    if ((p_req_data->notification_priv_type == PDSM_LCS_USER_NOTIFY_ONLY) ||
        (p_req_data->notification_priv_type == PDSM_LCS_NO_NOTIFY_NO_VERIFY)
        )
    {
      /* SUPL_END with only VER in it */
      (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_slp_id,
                                                   NULL,
                                                   0xFFFFFFFF,
                                                   p_req_data->supl_hash,
                                                   q_supl_ver_maj_after_supl_init_glob);

      q_supl_ver_maj_after_supl_init_glob = 0;

    }
    else
    {
      (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_slp_id,
                                                   NULL,
                                                   supl_StatusCode_consentGrantedByUser,
                                                   p_req_data->supl_hash,
                                                   q_supl_ver_maj_after_supl_init_glob);
      q_supl_ver_maj_after_supl_init_glob = 0;

    }
    return;
  }

  /*********** preempt any existing SUPL session *********************/
  if (p_supl_session_status->u_callflow_kind != C_SUPL_SESSION_NONE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: preempting existing session", 0, 0, 0);
    supl_preempt_ongoing_session(supl_StatusCode_resourceShortage);
  }

  /*Select the positioning protocol to use*/
  if ((p_supl_session_status->u_position_protocol =
       tm_umts_up_supl_select_pos_prot()) == TM_POS_PRTL_NONE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: Position Protocol couldnt be determined", 0, 0, 0);
    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_slp_id,
                                                 NULL,
                                                 supl_StatusCode_unspecified,
                                                 NULL,
                                                 p_supl_session_status->q_supl_version);

    return;
  }
  /* Checking to see if this is area event trigger */
  e_trigger_type = supl_init_trigger_event_check();

  if (e_trigger_type != C_SUPL_INIT_TRIG_TYPE_NULL)
  {
    if (e_trigger_type == C_SUPL_INIT_TRIG_TYPE_AREA_EVENT)
    {
      supl_init_trigger_event_proc(p_req_data, p_slp_id, phone_state_info);
      q_supl_ver_maj_after_supl_init_glob = 0;

    }
    else
    {
      /* should never happen */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "trigger types not processed here: %u", e_trigger_type, 0, 0);
    }

    return;
  } /* for Triggered sessions, this function ends here */

  /*********** copy SLP supported constellations *********************/
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "p_req_data->flags 0x%x postech 0x%x",
        p_req_data->flags,
        p_req_data->supl_init_ver2_ext.gnss_pos_tech);

  if (p_req_data->flags & PDSM_LCS_SUPLINIT_VER2_EXT_PRESENT)
  {
    p_supl_session_status->u_slp_support_gps     =
       (p_req_data->supl_init_ver2_ext.gnss_pos_tech &
           PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_GPS) ? TRUE : FALSE;
    p_supl_session_status->u_slp_support_glonass =
       (p_req_data->supl_init_ver2_ext.gnss_pos_tech &
           PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_GLONASS) ? TRUE : FALSE;
    p_supl_session_status->u_slp_support_bds     =
       (p_req_data->supl_init_ver2_ext.gnss_pos_tech &
           PDAPI_SUPLINIT_GNSS_POS_TECH_BITMAP_BDS) ? TRUE : FALSE;
  }
  /*********** copy SUPL hash *********************/
  memscpy(p_supl_session_status->supl_init_hash, sizeof(p_supl_session_status->supl_init_hash),
          p_req_data->supl_hash, sizeof(p_req_data->supl_hash));
  p_supl_session_status->u_hash_valid = TRUE;

  /************** store SLP session ID ************************************/
  p_supl_session_status->z_session_id.z_Slp_Sess_Id = *p_slp_id;
  p_supl_session_status->ni_pos_method = p_req_data->pos_method;

  p_supl_session_status->q_supl_version = q_supl_ver_maj_after_supl_init_glob;
  q_supl_ver_maj_after_supl_init_glob = 0;
  MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "SLP supports GPS %d BDS %d Glo %d. supl ver 0x%x",
        p_supl_session_status->u_slp_support_gps,
        p_supl_session_status->u_slp_support_bds,
        p_supl_session_status->u_slp_support_glonass,
        p_supl_session_status->q_supl_version);

  /* Request MRL updates */
  supl_request_l1_mrl_updates(phone_state_info.srv_system);

  /* if this was an emergency session, copy over the SLP address if given by the server */
  if (p_req_data->flags & PDSM_LCS_SUPLINIT_VER2_EXT_EMERGENCY_PRESENT)
  {
    p_supl_session_status->z_emergency_data.b_valid = TRUE;
    p_supl_session_status->z_emergency_data.b_emergency_ni = TRUE;
    if (p_req_data->supl_init_ver2_ext_emergency.b_eslp_address_present == TRUE)
    {

      p_supl_session_status->z_emergency_data.b_eslp_present = TRUE;
      memscpy(&p_supl_session_status->z_emergency_data.z_eslp_address,
              sizeof(p_supl_session_status->z_emergency_data.z_eslp_address),
              &p_req_data->supl_init_ver2_ext_emergency.e_slp_address,
              sizeof(p_req_data->supl_init_ver2_ext_emergency.e_slp_address));
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_UMTS_UP: SUPL ESLP present", 0, 0, 0);
    }
  }
  else
  {
    p_supl_session_status->z_emergency_data.b_valid = FALSE;
    p_supl_session_status->z_emergency_data.b_emergency_ni = FALSE;
    p_supl_session_status->z_emergency_data.b_eslp_present = FALSE;
  }

  if (!(p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND))
  {
    if ((z_pdcomm_tcp_info.emergency_connection == TRUE) ||
        (supl_session_is_emergency_session() == TRUE)
        )
    {
      /*Previous connection can not be reused*/
      tm_umts_up_supl_comm_close_connection();
    }
  }

  /******** set agps_mode ******************************************/
  if ((p_req_data->pos_method == POSMETHOD_AGPS_SETBASED) ||
      (p_req_data->pos_method == POSMETHOD_AGPS_SETBASED_PREF)
      )
  {

    /*NI MS BASED only if MS-BASED GPS (or MSBased BDS) is supported and RRLP is 
     * used as pos protocol*/
    if (((TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED))
         || tm_umts_up_supl_rrlp_assisted_bds_enabled()) &&
        (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
       )
    {
      p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED; /* RRLP may alter */
      p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

      /* horz. unc. 0 to prevent LM turning off MGP */
      u_hor_acc_threshold = 0;

      tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

      /* this will ask pdapi to generate pdsm_get_pos() to TmCore */
      if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSB,
                                                  u_hor_acc_threshold,
                                                  TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
          )
      {
        /* this attempt to start a new SUPL session fails, clear session status */
        memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
        return;
      }

    } /* MSB allowed and RRLP is pos protocol*/
    else if ((TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED)) &&
             (p_supl_session_status->u_position_protocol != TM_POS_PRTL_LPP))
    {
      /* MSB not allowed but MSA is allowed or IS801 is pos protocol, start with MSA and will let SLP
         know that in SUPL_POS_INIT */
      p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED; /* RRLP may alter */
      p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

      tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

      /* this will ask pdapi to generate pdsm_get_pos() to TmCore. Note accuracy_threshold
          does not matter for MSA, arbitary value of 100 used. */
      if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSA, 100,
                                                  TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
          )
      {
        /* this attempt to start a new SUPL session fails, clear session status */
        memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
        return;
      }

    } /* MSB not allowed but MSA is */
    else if  (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_BASED))
      {

        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED; /* RRLP may alter */
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

        /* horz. unc. 0 to prevent LM turning off MGP */
        u_hor_acc_threshold = 0;

        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

        /* this will ask pdapi to generate pdsm_get_pos() to TmCore */
        if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSB, u_hor_acc_threshold,
                                                    TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
            )
        {
          /* this attempt to start a new SUPL session fails, clear session status */
          memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
          return;
        }
      }

      else if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_ASSISTED))
      {
        /* MSB not allowed but MSA is allowed or IS801 is pos protocol, start with MSA and will let SLP
           know that in SUPL_POS_INIT */
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED; /* RRLP may alter */
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

        /* this will ask pdapi to generate pdsm_get_pos() to TmCore. Note accuracy_threshold
            does not matter for MSA, arbitary value of 100 used. */
        if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSA, 100,
                                                    TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
            )
        {
          /* this attempt to start a new SUPL session fails, clear session status */
          memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
          return;
        }

      } /* MSB not allowed but MSA is */
    } /* MSB allowed and LPP is pos protocol*/

  }  /* SUPL_INIT asking for MSB or MSB_pref */

  else if ((p_req_data->pos_method == POSMETHOD_AGPS_SETASSISTED) ||
           (p_req_data->pos_method == POSMETHOD_AGPS_SETASSISTED_PREF) ||
           (p_req_data->pos_method == POSMETHOD_AFLT) ||
           (p_req_data->pos_method == POSMETHOD_ECID) ||
           (p_req_data->pos_method == POSMETHOD_EOTD) ||
           (p_req_data->pos_method == POSMETHOD_OTDOA)
           )
  {

    if (p_supl_session_status->u_position_protocol != TM_POS_PRTL_LPP)
    {
      if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED))
      {

        byte gps_session_timeout = TM_MAX_SESSION_TIMEOUT;
        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;


        if ((p_req_data->pos_method == POSMETHOD_AGPS_SETASSISTED) ||
            (p_req_data->pos_method == POSMETHOD_AGPS_SETASSISTED_PREF)
            )
        {
          if ((p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP) ||
              (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
              )
          {
            /*For both IS801 and RRLP, selected Assisted*/
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
          }
        } /*SETASSISTED or SETASSISTED_PREF*/

        else if (p_req_data->pos_method == POSMETHOD_AFLT ||
                 p_req_data->pos_method == POSMETHOD_ECID ||
                 p_req_data->pos_method == POSMETHOD_OTDOA) /*AFLT/ECID/OTDOA*/
        {
          if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
          {
            /* even for the AFLT, ECID, OTDOA will start as MSA first, the SLP
            has the chance to change to a supported mode once it gets SUPL_POS_INIT */
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED; /* RRLP may alter */
          }
          else if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_IS801)
          {

            if (p_req_data->pos_method == POSMETHOD_AFLT)
            {
              /*If IS801 is pos protocol and pos method is AFLT then set session agps mode to UE_ASSISTED*/
              p_supl_session_status->u_agps_mode = C_SUPL_AFLT_MODE;
              /*For AFLT, gps_session_timeout should be zero*/
              gps_session_timeout = 0;
            }
            else
            {
              /*ECID case*/
              p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_CELL_ID;
            }
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Pos Protocol not supported", 0, 0, 0);
          }
        } /*ECID/AFLT*/
        else /*all other, right now EOTD */
        {
          if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
          {
            /* For all unsupported, will start as MSA first, the SLP
            has the chance to change to a supported mode once it gets SUPL_POS_INIT */
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED; /* RRLP may alter */
          }
        }


        if (p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_UNDEF)
        {
          /* this will ask pdapi to generate pdsm_get_pos() to TmCore. Note accuracy_threshold
             does not matter for MSA, arbitary value of 100 used. */
          if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSA, 100,
                                                      gps_session_timeout, 1, 0, MT_UP, FALSE)
              )
          {
            /* this attempt to start a new SUPL session fails, clear session status */
            memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
            return;
          }
        }
      }
      /*For RRLP if MSA is not allowed and MSB is allowed, MSB is started.*/
      else if ((TM_CONFIG_POS_MODE_BIT_IS_SET(
               PDSM_POS_MODE_UP_MS_BASED | PDSM_POS_MODE_4G_UP_MS_BASED)) ||
               tm_umts_up_supl_rrlp_assisted_bds_enabled()
               )
      {
        if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
        {
          /* MSA not allowed but MSB is. Start with MSB and see if SLP changes its
          mind  */
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
          p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

          tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

          /* this will ask pdapi to generate pdsm_get_pos() to TmCore */
          /* horz. unc. 0 to prevent LM turning off MGP */
          if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSB, 0,
                                                      TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
              )
          {
            /* this attempt to start a new SUPL session fails, clear session status */
            memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
            return;
          }
        }

      }  /* MSA not allowed but MSB allowed */
    }

    if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
    {
      if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_ASSISTED))
      {

        byte gps_session_timeout = TM_MAX_SESSION_TIMEOUT;
        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;


        if ((p_req_data->pos_method == POSMETHOD_AGPS_SETASSISTED) ||
            (p_req_data->pos_method == POSMETHOD_AGPS_SETASSISTED_PREF)
            )
        {
          /*For both IS801 and RRLP, selected Assisted*/
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
        } /*SETASSISTED or SETASSISTED_PREF*/

        else if (p_req_data->pos_method == POSMETHOD_AFLT ||
                 p_req_data->pos_method == POSMETHOD_OTDOA ||
                 p_req_data->pos_method == POSMETHOD_ECID) /*AFLT/ECID*/
        {
          if (p_req_data->pos_method != POSMETHOD_ECID)
          {
            p_supl_session_status->u_agps_mode = C_SUPL_OTDOA_MODE;
            /*For AFLT, gps_session_timeout should be zero*/
            gps_session_timeout = 0;
          }
          else
          {
            /*ECID case*/
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_CELL_ID;
          }
        } /*ECID/AFLT*/
        else /*all other*/
        {
          if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
          {
            /* For all unsupported, will start as MSA first, the SLP
            has the chance to change to a supported mode once it gets SUPL_POS_INIT */
            p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED; /* RRLP may alter */
          }
        }

        if (p_supl_session_status->u_agps_mode == C_SUPL_OTDOA_MODE)
        {
          /* this will ask pdapi to generate pdsm_get_pos() to TmCore. Note accuracy_threshold
             does not matter for MSA, arbitary value of 100 used. */
          if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_OTDOA, 100,
                                                      gps_session_timeout, 1, 0, MT_UP, FALSE))
          {
            /* this attempt to start a new SUPL session fails, clear session status */
            memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
            return;
          }
        }
        else if (p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_UNDEF)
        {
          /* this will ask pdapi to generate pdsm_get_pos() to TmCore. Note accuracy_threshold
             does not matter for MSA, arbitary value of 100 used. */
          if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSA, 100,
                                                      gps_session_timeout, 1, 0, MT_UP, FALSE)
              )
          {
            /* this attempt to start a new SUPL session fails, clear session status */
            memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
            return;
          }
        }
      }
      /*For LPP if MSA is not allowed and MSB is allowed, MSB is started.*/
      else if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_BASED))
      {
        if (p_supl_session_status->u_position_protocol == TM_POS_PRTL_LPP)
        {
          /* MSA not allowed but MSB is. Start with MSB and see if SLP changes its
             mind  */
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
          p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

          tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

          /* this will ask pdapi to generate pdsm_get_pos() to TmCore */
          /* horz. unc. 0 to prevent LM turning off MGP */
          if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSB, 0,
                                                      TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
              )
          {
            /* this attempt to start a new SUPL session fails, clear session status */
            memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
            return;
          }
        }

      } /* MSA not allowed but MSB allowed */
    } /* lpp*/

  } /* close if-assisted */


  else if (p_req_data->pos_method == POSMETHOD_AUTONOMOUS_GPS)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: accepting NI autonomous", 0, 0, 0);

    /* even for autonomous, with NI it will involve RRLP msrPosReq
       so it needs MSB to be supported; or MSA so that SLP will
       change its mind to ask for MSA; SET will inform SLP if stanalone
       is supported in SUPL_POS_INIT capability IE */

    /*For IS801 if pos method is autonomous, then MSA will be started*/

    if (p_supl_session_status->u_position_protocol != TM_POS_PRTL_LPP)
    {
      if (((TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED)) ||
           tm_umts_up_supl_rrlp_assisted_bds_enabled())
          && (p_supl_session_status->u_position_protocol == TM_POS_PRTL_RRLP)
          )
      {
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

        /* horz. unc. 0 to prevent LM turning off MGP */
        u_hor_acc_threshold = 0;

        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

        if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSB, u_hor_acc_threshold,
                                                    TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
            )
        {
          /* this attempt to start a new SUPL session fails, clear session status */
          memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
          return;
        }

      } /* if MSB provisioned */

      else if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED))
      {

        /* MSB not allowed but MSA is. Start with MSA and if if SLP changes to MSA later */
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

        /* this will ask pdapi to generate pdsm_get_pos() to TmCore. Note accuracy_threshold
           does not matter for MSA, arbitary value of 100 used. */
        if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSA, 100,
                                                    TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
            )
        {
          /* this attempt to start a new SUPL session fails, clear session status */
          memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
          return;
        }
      } /* MSB not allowed but MSA is */
    }
    else
    {
      /* LPP */
      if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_BASED))
      {
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

        /* horz. unc. 0 to prevent LM turning off MGP */
        u_hor_acc_threshold = 0;

        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

        if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSB, u_hor_acc_threshold,
                                                    TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
            )
        {
          /* this attempt to start a new SUPL session fails, clear session status */
          memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
          return;
        }


      } /* if MSB provisioned */

      else if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_4G_UP_MS_ASSISTED))
      {

        /* MSB not allowed but MSA is. Start with MSA and if if SLP changes to MSA later */
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
        p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

        tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NI);

        /* this will ask pdapi to generate pdsm_get_pos() to TmCore. Note accuracy_threshold
           does not matter for MSA, arbitary value of 100 used. */
        if (!tm_umts_common_ni_client_start_session(TM_SESS_OPERATION_MODE_MSA, 100,
                                                    TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE)
            )
        {
          /* this attempt to start a new SUPL session fails, clear session status */
          memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
          return;
        }
      } /* MSB not allowed but MSA is */
    }
  }
  else
  {
    /* no other mode is allowed, ignore the message if happens.  */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "OEM returns unsupported pos_method: %u ", p_req_data->pos_method, 0, 0);
  }

  /* this should not happen */
  if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UNDEF)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "agps cannot be established", 0, 0, 0);
    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_slp_id,
                                                 NULL,
                                                 supl_StatusCode_posMethodMismatch,
                                                 p_req_data->supl_hash,
                                                 p_supl_session_status->q_supl_version);
  }

}



/*
 ******************************************************************************
 *
 * supl_preempt_ongoing_session
 *
 * Function description:
 *  This function preemts the ongoing SUPL session, as a new session has been
 *  accepted.
 *
 *
 * Parameters:
 *   None.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

static void supl_preempt_ongoing_session(supl_StatusCode e_cancel_reason)
{
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;

  if (p_supl_session_status->u_network_session_on == TRUE)
  {
    (void)tm_umts_up_supl_supl_end_prepare_or_tx(&p_supl_session_status->z_session_id.z_Slp_Sess_Id,
                                                 NULL,
                                                 e_cancel_reason,
                                                 NULL,
                                                 p_supl_session_status->q_supl_version);
  }

  /* inform TmCore the current session has been preempted */
  tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_SUPL_SESS_PREEMT);

  tm_umts_up_supl_session_init(TRUE);

}

/*
 ******************************************************************************
 *
 * supl_gad9_to_pos_est
 *
 * Function description:
 *  This function converts a GAD Shape of Ellipsoid Point with altitude and
 *  uncertainty ellipsoid into supl_PositionEstimate.
 *  accepted.
 *
 *
 * Parameters:
 *   None.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

static void supl_gad9_to_pos_est
(const gad_ellip_alt_unc_ellip_type *p_zEllipAltUncEllip,
 supl_PositionEstimate *p_positionInfo
 )

{

  p_positionInfo->m.altitudeInfoPresent = 1;
  p_positionInfo->m.confidencePresent = 1;
  p_positionInfo->m.uncertaintyPresent = 1;

  /***************************** latitude *****************************/

  /** handle sign bit */

  if (p_zEllipAltUncEllip->u_Lat0  & 0x80)
    p_positionInfo->latitudeSign = supl_south;
  else
    p_positionInfo->latitudeSign = supl_north;

  p_positionInfo->latitude =
     ((uint32)p_zEllipAltUncEllip->u_Lat0 & 0x0000007F) << 16;
  p_positionInfo->latitude |=
     (uint32)p_zEllipAltUncEllip->u_Lat1  << 8;
  p_positionInfo->latitude |=
     (uint32)p_zEllipAltUncEllip->u_Lat2;

  /***************************** longitude *****************************/

  p_positionInfo->longitude =
     (uint32)p_zEllipAltUncEllip->u_Long0 << 16;

  p_positionInfo->longitude |=
     (uint32)p_zEllipAltUncEllip->u_Long1 << 8;

  p_positionInfo->longitude |=
     (uint32)p_zEllipAltUncEllip->u_Long2;

  /*** sign extend it **/
  if (p_positionInfo->longitude & 0x00800000)
    p_positionInfo->longitude |= 0xFF800000;

  /* unc. major */
  p_positionInfo->uncertainty.uncertaintySemiMajor =
     p_zEllipAltUncEllip->u_UncMajor;

  /* unc. minor */
  p_positionInfo->uncertainty.uncertaintySemiMinor =
     p_zEllipAltUncEllip->u_UncMinor;

  /* unc. major axis orientation */
  p_positionInfo->uncertainty.orientationMajorAxis =
     p_zEllipAltUncEllip->u_OrientMajor;

  /****************** altitude ***********************************************/
  p_positionInfo->m.altitudeInfoPresent = 1;

  if (p_zEllipAltUncEllip->u_Alt0 & 0x80)
    p_positionInfo->altitudeInfo.altitudeDirection = supl_depth;
  else
    p_positionInfo->altitudeInfo.altitudeDirection = supl_height;

  p_positionInfo->altitudeInfo.altitude =
     ((uint32)p_zEllipAltUncEllip->u_Alt0 & 0x0000007F) << 8;

  p_positionInfo->altitudeInfo.altitude |=
     (uint32)p_zEllipAltUncEllip->u_Alt1;

  p_positionInfo->altitudeInfo.altUncertainty =
     (uint32)p_zEllipAltUncEllip->u_UncAlt;

  /********************** confidence ****************************************/
  p_positionInfo->confidence =
     (uint32)p_zEllipAltUncEllip->u_Confidence;

}

/*
 ******************************************************************************
 *
 * supl_convert_slp_id_from_notif_verif_resp
 *
 * Function description:
 *  This function converts SLP ID passed back to TM from Notif-Verif response
 *  into the format that can be used in SUPL messages.
 *
 *
 * Parameters:
 *   None.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

void supl_convert_slp_id_from_notif_verif_resp
(pdsm_lcs_supl_notify_verify_req_s_type   *p_req_data,
 supl_slp_session_id                      *p_slp_id
 )
{

  if (p_req_data->supl_slp_session_id.presence)
  {

    p_slp_id->presence = TRUE;

    memscpy(&p_slp_id->session_Id[0], sizeof(p_slp_id->session_Id),
            &p_req_data->supl_slp_session_id.session_id[0],
            sizeof(p_req_data->supl_slp_session_id.session_id)
            );

    if (p_req_data->supl_slp_session_id.slp_address.choice == PDSM_SERVER_TYPE_IPV4_ADDRESS)
    {
      p_slp_id->zSlpAddress.choice =
         C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS;

      p_slp_id->zSlpAddress.u.z_Ip_Address.choice =
         C_SUPL_ASN1_CHOICE_IPV4ADDRESS;

      p_slp_id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.numocts = C_SUPL_IPADDRESS_IPV4_BYTE_LENGTH;

      memscpy(&p_slp_id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.data[0],
              sizeof(p_slp_id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.data),
              &p_req_data->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v4.server_adrs,
              sizeof(p_req_data->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v4.server_adrs)
              );

    } /* SLP address being IP address, V4 */

    else if (p_req_data->supl_slp_session_id.slp_address.choice == PDSM_SERVER_TYPE_IPV6_ADDRESS)
    {
      p_slp_id->zSlpAddress.choice =
         C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS;

      p_slp_id->zSlpAddress.u.z_Ip_Address.choice =
         C_SUPL_ASN1_CHOICE_IPV6ADDRESS;

      p_slp_id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.numocts = C_SUPL_IPADDRESS_IPV6_BYTE_LENGTH;


      memscpy(&p_slp_id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.data[0],
              sizeof(p_slp_id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.data),
              &p_req_data->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v6.server_adrs[0],
              sizeof(p_req_data->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_v6.server_adrs)
              );

    } /* SLP address being IP address, V6 */

    else if (p_req_data->supl_slp_session_id.slp_address.choice == PDSM_SERVER_TYPE_URL_ADDRESS)
    {
      p_slp_id->zSlpAddress.choice = C_SUPL_ASN1_CHOICE_SLPADRESS_FQDN;

      p_slp_id->zSlpAddress.u.z_fqdn.u_length =
         p_req_data->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_url.url_length;

      memscpy(&p_slp_id->zSlpAddress.u.z_fqdn.fqdn[0],
              sizeof(p_slp_id->zSlpAddress.u.z_fqdn.fqdn),
              &p_req_data->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_url.url[0],
              p_req_data->supl_slp_session_id.slp_address.supl_slp_address.server_adrs_url.url_length
              );
    }

  } /* if SLP ID present */

}

/*
 ******************************************************************************
 *
 * supl_convert_velocity
 *
 * Function description:
 *  This function converts velocity information into the velocity format
 *  of SUPL POS. The function currently only handles velocity type of
 *  horizontal_and_vertical.
 *
 *
 * Parameters:
 *   f_vel_east, f_vel_north, f_vel_vertical : float point velocity info.
 *   *p_velocity : pointer to the structure for SUPL POS velocity; note its
 *                 member u.horandvervel must be allocated by the calling
 *                 function.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/
void supl_convert_velocity
(float f_vel_east,
 float f_vel_north,
 float f_vel_vertical,
 supl_Velocity *p_velocity
 )

{
  float f_heading_ang;
  uint16 w_hor_velocity;
  int16  s_bearing;
  int8   b_vertical_velocity;

  uint8            u_bearing_bit_string[2];
  uint8            u_hor_speed_bit_string[2];
  uint8            u_ver_direct_bit_string;
  uint8            u_ver_direct;
  uint8            u_ver_velocity_bit_string;

  float f_temp = 0;

  if ((p_velocity == NULL) || (p_velocity->u.horandvervel == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Mem not allocated for vel.", 0, 0, 0);
    return;
  }


  tm_umts_common_convert_velocity(&f_heading_ang,
                                  &w_hor_velocity,
                                  &s_bearing,
                                  &b_vertical_velocity,
                                  &u_ver_direct,
                                  &u_ver_velocity_bit_string,
                                  &f_vel_east,
                                  &f_vel_north,
                                  &f_vel_vertical);

  p_velocity->t = C_SUPL_ASN1_CHOICE_VELCITY_HORANDVERVEL;

  /* vertical velocity direction */
  if (f_vel_vertical < 0.0f)
  {
    /* downward direction */
    u_ver_direct = SUPL_VELOCITY_DOWNWARD;
  }
  else
  {
    /* upward direction */
    u_ver_direct = SUPL_VELOCITY_UPWARD;
  }
  /*   hor_speed        BIT STRING(SIZE (16)) */
  p_velocity->u.horandvervel->horspeed.numbits = 16;
  u_hor_speed_bit_string[0] = (w_hor_velocity >> 0x08) & 0xFF;
  /* high byte */
  u_hor_speed_bit_string[1] = w_hor_velocity & 0xFF;
  /* low byte */
  p_velocity->u.horandvervel->horspeed.data[0] = u_hor_speed_bit_string[0];
  p_velocity->u.horandvervel->horspeed.data[1] = u_hor_speed_bit_string[1];


  /*   bearing BIT STRING(SIZE (9)) */
  p_velocity->u.horandvervel->bearing.numbits = 9;
  /* high byte - upper 8 (of 9) bits */
  u_bearing_bit_string[0] = (s_bearing >> 0x01) & 0xFF;
  /* low byte - lowest 1 bit */
  u_bearing_bit_string[1] = s_bearing & 0x01;
  u_bearing_bit_string[1] <<= 0x07;
  p_velocity->u.horandvervel->bearing.data[0] = u_bearing_bit_string[0];
  p_velocity->u.horandvervel->bearing.data[1] = u_bearing_bit_string[1];


  /*   verspeed        BIT STRING(SIZE (8)) */
  p_velocity->u.horandvervel->verspeed.numbits = 8;
  p_velocity->u.horandvervel->verspeed.data[0] = u_ver_velocity_bit_string;

  /*   verdirect       BIT STRING(SIZE (1)) */
  p_velocity->u.horandvervel->verdirect.numbits = 1;
  u_ver_direct_bit_string = u_ver_direct << 7;
  p_velocity->u.horandvervel->verdirect.data[0] = u_ver_direct_bit_string;

}

/*
 ******************************************************************************
 *
 * supl_convert_supl_end_pos_for_tm_core
 *
 * Function description:
 *  This function converts position / velocity info in SUPL_END to that of
 *  pdsm_pd_info_s_type variable of z_supl_pd_info_buffer, and prepares the
 *  position for posting to TmCore as type LR.
 *
 * Parameters:
 *   p_suplEnd : pointer to SUPL_END message buffer.
 *   p_ref_loc : pointer of structre for seed location that
 *               is to be populated by this function.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

static void supl_convert_supl_end_pos_for_tm_core
(const supl_SUPLEND *p_suplEnd, gps_RefLocStructType *p_ref_loc)
{

  pdsm_pd_info_s_type *p_pd_info_buffer = &z_supl_pd_info_buffer;
  uint32                   q_temp = 0;
  double  d_temp;
  float   f_temp;
  char    timestamp[14];
  uint16  day, month, year, hour, minute, second;
  uint16 gps_week = 0;
  uint32 gps_towms = 0;
  uint8 leap_seconds = 0;

  uint8 u_MajorUncK, u_MinorUncK, u_UncAngle, u_IS801Major, u_IS801Minor, u_IS801Angle;
  float f_major_unc_meters, f_minor_unc_meters;

  /*
   *  Initialize the return structure to 0.  This sets all validity
   *  flags to FALSE and all integer and FLT/DBL numbers to 0.
   */
  memset(p_ref_loc, 0, sizeof(*p_ref_loc));

  /*
   *  Initialize uncertainty to MAX values "just in case"
   */
  p_ref_loc->f_LocUncrtntySemiMajor = MAX_UNCERTAINTY;
  p_ref_loc->f_LocUncrtntySemiMinor = MAX_UNCERTAINTY;
  p_ref_loc->f_AltitudeUnc          = MAX_UNCERTAINTY;

  /* includes optional extended position fields */
  p_pd_info_buffer->pd_info.pd_data.opt_field_mask = PDSM_PD_EXT_STATUS_VALID;

  /* Set UTC Time */
  memscpy(timestamp, sizeof(timestamp), p_suplEnd->position.timestamp, sizeof(timestamp));
  year = (timestamp[0] - '0') * 10 + (timestamp[1] - '0') + 2000; /* Convert 2 digit year to 4 digit year */
  month = (timestamp[2] - '0') * 10 + (timestamp[3] - '0');
  day = (timestamp[4] - '0') * 10 + (timestamp[5] - '0');
  z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.utc_time.date =
     (uint32)((day * 1000000) + (month * 10000) + year);
  hour = (timestamp[6] - '0') * 10 + (timestamp[7] - '0');
  minute = (timestamp[8] - '0') * 10 + (timestamp[9] - '0');
  second = (timestamp[10] - '0') * 10 + (timestamp[11] - '0');
  z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.utc_time.hour = hour;
  z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.utc_time.minute = minute;
  z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.utc_time.sec = second * 100;
  z_supl_pd_info_buffer.pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_UTC_TIME_VALID;

  /*Calculate the GPS time from the SUPL END position UTC timestamp*/
  (void)tm_core_get_utc_offset((uint8 *)&leap_seconds);

  if (gnss_ConvertUtcTimeStamptoGpsTime(&z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.utc_time,
                                        leap_seconds,
                                        &gps_week,
                                        &gps_towms))
  {
    z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.timestamp_gps_week = gps_week;
    z_supl_pd_info_buffer.pd_info.pd_data.ext_pos_data.timestamp_tow_ms = gps_towms;

    p_ref_loc->w_GpsWeek = gps_week;

    p_ref_loc->q_GpsMsec = gps_towms;


    p_pd_info_buffer->pd_info.pd_data.time_stamp = (uint32)(tm_umts_common_utils_full_gps_ms(gps_week,
                                                                                             gps_towms) / SEC_MSECS);

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "GPS Week %d GPS Tow MSec %d Full GPS Sec %d",
          p_ref_loc->w_GpsWeek,
          p_ref_loc->q_GpsMsec,
          p_pd_info_buffer->pd_info.pd_data.time_stamp);
    z_supl_pd_info_buffer.pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_GPS_TIME_VALID;
  }

  if (tm_core_get_utc_offset((uint8 *)&z_supl_pd_info_buffer.pd_info.pd_data.gpsUtcOffset))
  {
    z_supl_pd_info_buffer.pd_info.pd_data.opt_field_mask |= PDSM_PD_UTCOFFSET_VALID;
  }

  /* LATITUDE:  construct a 24-bit GAD shape Latitude with Bit 23 being the sign bit */
  q_temp = p_suplEnd->position.positionEstimate.latitude & 0x007FFFFF;
  q_temp |= p_suplEnd->position.positionEstimate.latitudeSign << 23;

  /* PDAPI lat. LSB is ( 180 / 2^25 ) degrees, and GAD shape lat. LSB
     is (90 / 2^23 ), so, GadShapeLat * (90/2^23 ) / (180 / 2^25 ) =
     GadShapeLat * 2
  */
  /* Absolute value of ext_pos_data.lat = (abs(lat rad) * C_2_32_OVER_PI);
     q_temp * LAT_IS801_SCALING ==> abs(lat degrees)
     LAT_IS801_SCALING = 90/2^23
     abs( ext_pos_data.lat ) =
       q_temp * 90 / 2^23 * 2^32 / PI * DEG_TO_RADIANS =
       q_temp * 90 * 2^9 * DEG_TO_RADIANS / PI =
       q_temp * 90 * 2^9 * PI / 180 / PI =
       q_temp * 2^8
  */


  if (q_temp & 0x00800000)
  {
    /* southern hemisphere */
    q_temp &= 0x007FFFFF;
    p_pd_info_buffer->pd_info.pd_data.lat  = (int32)(0 - (int32)(q_temp * 2));
    p_pd_info_buffer->pd_info.pd_data.ext_pos_data.lat = (int32)(0 - (int32)(q_temp * 256));
    d_temp = (double)(0 - (int32)q_temp);
  }
  else
  {
    /* northern hemisphere */
    q_temp &= 0x007FFFFF;
    p_pd_info_buffer->pd_info.pd_data.lat  = (int32)(q_temp * 2);
    p_pd_info_buffer->pd_info.pd_data.ext_pos_data.lat = (int32)(q_temp * 256);
    d_temp = (double)q_temp;
  }
  p_ref_loc->d_Latitude = d_temp * LAT_IS801_SCALING * DEG_TO_RADIANS;


  /* LONGITUDE: construct a 32-bit 2's complement ************************/
  q_temp = p_suplEnd->position.positionEstimate.longitude & 0x00FFFFFF;
  if (q_temp & 0x00800000)
  {
    q_temp |=  0xFF800000; /* sign extend */
  }

  /* PDAPI longitude has LSB of (180/2^25) degrees, and GAD shape lat. LSB
     is (360/2^24) degrees, so GadShapeLong * (360/2^24) / (180/2^25 ) =
     GadShapeLong * 4
  */
  /* ext_pos_data.lon = lon_rad * C_2_31_OVER_PI
     q_long_word * LON_IS801_SCALING ==> longitude degrees
     LON_IS801_SCALING = 180.0/(DBL)(1L<<23)
     ext_pos_data.lon =
       q_long_word * 180 / 2^23 * 2^31 / PI * DEG_TO_RADIANS =
       q_long_word * 180 * 2^8 * DEG_TO_RADIANS / PI =
       q_long_word * 180 * 2^8 * PI / 180 / PI =
       q_long_word * 2^8
  */
  p_pd_info_buffer->pd_info.pd_data.lon = (int32)q_temp * 4;
  p_pd_info_buffer->pd_info.pd_data.ext_pos_data.lon = (int32)(q_temp) * 256;
  d_temp = (DBL)((int32)q_temp);
  p_ref_loc->d_Longitude = d_temp * LON_IS801_SCALING * DEG_TO_RADIANS;
  p_pd_info_buffer->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_LAT_LONG_VALID;

  if (p_suplEnd->position.positionEstimate.m.altitudeInfoPresent)
  {

    /* 3D fix */
    p_pd_info_buffer->pd_info.pd_data.fix_type = 1;
    p_pd_info_buffer->pd_info.pd_data.opt_field_mask |= PDSM_PD_ALTITUDE_VALID;

    p_ref_loc->u_AltitudeIsValid = TRUE;

    q_temp = p_suplEnd->position.positionEstimate.altitudeInfo.altitude & 0x0000FFFF;
    q_temp |= p_suplEnd->position.positionEstimate.altitudeInfo.altitudeDirection << 15;

    /* Check if the altitude is below  (i.e. depth) */
    if (q_temp & 0x00008000)
    {
      p_pd_info_buffer->pd_info.pd_data.altitude = -(int16)(q_temp & 0x00007FFF); /* signed, in meter */
      p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_altitude = -(float)(q_temp & 0x00007FFF); /* signed, in meter */
      p_ref_loc->f_Altitude = -(float)(q_temp & 0x7FFF);

    }
    else
    {
      p_pd_info_buffer->pd_info.pd_data.altitude = (int16)(q_temp & 0x00007FFF); /* signed, in meter */
      p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_altitude = (float)(q_temp & 0x00007FFF); /* signed, in meter */
      p_ref_loc->f_Altitude = (float)(q_temp & 0x7FFF);

    }
    p_pd_info_buffer->pd_info.pd_data.altitude += 500; /* BREW needs 500-meter altitude offset */
    p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_altitude += 500.0; /* BREW needs 500-meter altitude offset */


    if (p_suplEnd->position.positionEstimate.m.uncertaintyPresent)
    {
      uint8 u_temp;
      float f_alt_unc_meter;

      u_temp = p_suplEnd->position.positionEstimate.altitudeInfo.altUncertainty & 0x7F;
      f_alt_unc_meter =  tm_umts_common_utils_GADK_to_meter(FALSE, u_temp);

      p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_v =
         (uint8)tm_util_select_pos_unc(f_alt_unc_meter);

      p_ref_loc->f_AltitudeUnc = f_alt_unc_meter;

    }

  } /* close if-altitude-present */


  if (p_suplEnd->position.positionEstimate.m.uncertaintyPresent)
  {
    p_pd_info_buffer->pd_info.pd_data.opt_field_mask |= PDSM_PD_UNCERTAINTY_VALID;


    /* we get major axis unc, minor axis unc, and angle from North
     * of major axis (0 to 178 degrees). */
    u_MajorUncK = p_suplEnd->position.positionEstimate.uncertainty.uncertaintySemiMajor & 0x7F; /* Major axis unc */
    u_MinorUncK = p_suplEnd->position.positionEstimate.uncertainty.uncertaintySemiMinor & 0x7F; /* Minor axis unc */

    /* Angle from North of Major axis:
     *    2N < angle < 2(N+1)
     * where N = 0..89
     */
    u_UncAngle  = p_suplEnd->position.positionEstimate.uncertainty.orientationMajorAxis << 1;

    f_major_unc_meters = tm_umts_common_utils_GADK_to_meter(TRUE, u_MajorUncK);
    f_minor_unc_meters = tm_umts_common_utils_GADK_to_meter(TRUE, u_MinorUncK);
    p_ref_loc->f_LocUncrtntySemiMajor = f_major_unc_meters;
    p_ref_loc->f_LocUncrtntySemiMinor = f_minor_unc_meters;

    u_IS801Major = (uint8)tm_util_select_pos_unc(f_major_unc_meters);
    u_IS801Minor = (uint8)tm_util_select_pos_unc(f_minor_unc_meters);


    p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_loc_unc_horizontal = sqrt((f_major_unc_meters * f_major_unc_meters) +
                    (f_minor_unc_meters * f_minor_unc_meters));
    if (u_UncAngle > (2 * 89))
      u_UncAngle = 2 * 89;

    if (u_UncAngle >= 90)
    {
      /* Major is perp axis, and minor is along axis.
       * Subtract 90 from angle */
      u_UncAngle -= 90;

      p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_a = u_IS801Minor;
      p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_p = u_IS801Major;

      /* ext_pos_data.alongAxisUnc = (uint16)ext_pos_data_ptr->ext_loc_uncrtnty_a */
      p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_alongAxisUnc = f_minor_unc_meters;
      p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_perpAxisUnc = f_major_unc_meters;
    }
    else
    {
      /* Major is along axis, and minor is perp axis. */
      p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_a = u_IS801Major;
      p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_p = u_IS801Minor;

      /* ext_pos_data.alongAxisUnc = (uint16)ext_pos_data_ptr->ext_loc_uncrtnty_a */
      p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_alongAxisUnc = f_major_unc_meters;
      p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_perpAxisUnc = f_minor_unc_meters;
    }



    /* The angle is defined as follows in IS801.
     * LOC_UNCRTNTY_ANG – Angle of axis with respect to True North for
     * position uncertainty. The mobile station shall set this field to
     * the angle of axis for position uncertainty, in units of 5.625
     * degrees, in the range from 0 to 84.375 degrees, where 0 degrees
     * is True North and the angle increases toward the East.
     *
     * The coded value of the range is hence from 0 to 15.  We'll
     * interpret this as:
     *
     *    N * 5.625 <= angle < (N+1) * 5.625
     *
     * where N is the coded value, since this would be the most logical way
     * of dividing up the coded space EVENLY.  This is therefore just a
     * simple truncation.
     */
    p_ref_loc->f_LocUncrtntyAng = (float)u_UncAngle;
    u_IS801Angle = (uint8)((float)u_UncAngle / (float)5.625);

    /* With the limit checking above, the u_UncAngle is guaranteed to
     * be LESS than 90, so the u_IS801Angle is in the correct range
     * of 0..15.  No further limit checking needed. */

    p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_ang = u_IS801Angle;

    /*Fill ext_pos_data with full precision.*/
    p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_loc_uncertainty_ang = ( (float)u_UncAngle / (float)5.625 );
    p_pd_info_buffer->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_EXT_UNCERTAINTY_VALID;

    if (p_suplEnd->position.positionEstimate.m.confidencePresent)
    {
      p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_conf = p_suplEnd->position.positionEstimate.confidence & 0x7F;
    }
    else
    {
      p_pd_info_buffer->pd_info.pd_data.loc_uncertainty_conf = 0; /* 0 means "no information" */
    }

  } /* close if-uncertainty-present */

  /****************** if velocity is available *************************/

  if (p_suplEnd->position.m.velocityPresent)
  {
    uint16 w_horizontal_speed, w_heading_angle, w_speed_unc;
    uint8  u_vertical_speed;

    if (p_suplEnd->position.velocity.t == C_SUPL_ASN1_CHOICE_VELCITY_HORVEL)
    {
      if (p_suplEnd->position.velocity.u.horvel == NULL)
      {

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "velocity data buffer NULL, discarded", 0, 0, 0);
      }
      else if ((p_suplEnd->position.velocity.u.horvel->horspeed.numbits != 16) ||
               (p_suplEnd->position.velocity.u.horvel->bearing.numbits != 9)
               )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "velocity data length error, discarded", 0, 0, 0);
      }
      else
      {

        p_pd_info_buffer->pd_info.pd_data.opt_field_mask |= PDSM_PD_VELOCITY_VALID;
        p_pd_info_buffer->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_EXT_VELOCITY_VALID;
        p_ref_loc->u_VelocityIsValid = TRUE;

        /* horizontal speed: horspeed is defined to be of 16 bits */
        w_horizontal_speed = (uint16)(p_suplEnd->position.velocity.u.horvel->horspeed.data[0]);
        w_horizontal_speed <<= 8;
        w_horizontal_speed |= (uint16)(p_suplEnd->position.velocity.u.horvel->horspeed.data[1]);

        /* SUPL provides horspeed in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)w_horizontal_speed * 0.277f;

        /* Compute horizontal velocity from components and scale to units of 0.25 m/s, as per PDAPI spec. */
        p_pd_info_buffer->pd_info.pd_data.velocity_hor =
           (uint16)tm_util_nearest_long(4.0f * f_temp);

        /* Compute horizontal velocity in pdsm_pd_ext_pos_data_s_type velHor
           from components and scale to units of 0.01 m/s, as per PDAPI spec.
           f_verHor is defined as float in pdapi while no bearing is included, so     
           only magnitude of velocity should be reported here and should be limited
           to 0x7FFFF 
        */
        p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = 100.0f * f_temp;

        if(p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor > INT16_MAX)
        {
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = INT16_MAX;
        }

        p_ref_loc->f_VelHor = f_temp;


        /*************** bearing ******************************************/
        w_heading_angle = (uint16)(p_suplEnd->position.velocity.u.horvel->bearing.data[0]);
        w_heading_angle <<= 8;
        w_heading_angle |= (uint16)(p_suplEnd->position.velocity.u.horvel->bearing.data[1]);
        w_heading_angle &= 0x01FF;

        p_pd_info_buffer->pd_info.pd_data.heading =
           (uint16)tm_util_nearest_long(w_heading_angle * HEADING_SCALE);

        p_ref_loc->u_HeadingIsValid = TRUE;
        p_ref_loc->f_Heading = (float)w_heading_angle;

      } /* supl_end structure data sanity check ok */

    }  /* close if C_SUPL_ASN1_CHOICE_VELCITY_HORVEL */

    else if (p_suplEnd->position.velocity.t == C_SUPL_ASN1_CHOICE_VELCITY_HORANDVERVEL)
    {

      if (p_suplEnd->position.velocity.u.horandvervel == NULL)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "velocity data buffer NULL, discarded", 0, 0, 0);
      }
      else if ((p_suplEnd->position.velocity.u.horandvervel->horspeed.numbits != 16) ||
               (p_suplEnd->position.velocity.u.horandvervel->verspeed.numbits != 8) ||
               (p_suplEnd->position.velocity.u.horandvervel->verdirect.numbits != 1) ||
               (p_suplEnd->position.velocity.u.horandvervel->bearing.numbits != 9)
               )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_END velocity data length error, discarded", 0, 0, 0);
      }
      else
      {
        p_pd_info_buffer->pd_info.pd_data.opt_field_mask |= PDSM_PD_VELOCITY_VALID;
        p_pd_info_buffer->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_EXT_VELOCITY_VALID;
        p_ref_loc->u_VelocityIsValid = TRUE;

        /* Horizontal speed: horspeed is defined to be of 16 bits in its bitstring */
        w_horizontal_speed = (uint16)(p_suplEnd->position.velocity.u.horandvervel->horspeed.data[0]);
        w_horizontal_speed <<= 8;
        w_horizontal_speed |= (uint16)(p_suplEnd->position.velocity.u.horandvervel->horspeed.data[1]);

        /* SUPL provides horspeed in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)w_horizontal_speed * 0.277f;

        /* Compute horizontal velocity from components and scale to units of 0.25 m/s, as per PDAPI spec. */
        p_pd_info_buffer->pd_info.pd_data.velocity_hor =
           (uint16)tm_util_nearest_long(4.0f * f_temp);

        /* Compute horizontal velocity in pdsm_pd_ext_pos_data_s_type velHor
           from components and scale to units of 0.01 m/s, as per PDAPI spec.
           f_verHor is defined as float in pdapi while no bearing is included, so     
           only magnitude of velocity should be reported here and should be limited
           to 0x7FFFF
        */
        p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = 100.0f * f_temp;
        if (p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor > INT16_MAX)
        {
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = INT16_MAX;
        }


        p_ref_loc->f_VelHor = f_temp;

        /************* vertical speed ************************************************/

        u_vertical_speed = p_suplEnd->position.velocity.u.horandvervel->verspeed.data[0];

        /* SUPL provides verspeed in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)u_vertical_speed * 0.277f;

        if (p_suplEnd->position.velocity.u.horandvervel->verdirect.data[0] == 0) /* upward */
        {

          p_pd_info_buffer->pd_info.pd_data.velocity_ver =
             (int8)tm_util_nearest_long(2.0f * f_temp);

          /* Compute vertical velocity in pdsm_pd_ext_pos_data_s_type velVert
             from components and scale to units of 0.01 m/s, as per PDAPI spec. */
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velVert = 100.0f * f_temp;
        }
        else /* downward */
        {

          p_pd_info_buffer->pd_info.pd_data.velocity_ver =
             0 - (int8)tm_util_nearest_long(2.0f * f_temp);

          /* Compute vertical velocity in pdsm_pd_ext_pos_data_s_type velVert
           from components and scale to units of 0.01 m/s, as per PDAPI spec. */
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velVert = 0.0f - (100.0f * f_temp);
        }

        p_ref_loc->f_VelVert = f_temp;

        /************ bearing *******************************************/

        w_heading_angle = (uint16)(p_suplEnd->position.velocity.u.horandvervel->bearing.data[0]);
        w_heading_angle <<= 8;
        w_heading_angle |= (uint16)(p_suplEnd->position.velocity.u.horandvervel->bearing.data[1]);
        w_heading_angle &= 0x07FF;

        p_pd_info_buffer->pd_info.pd_data.heading =
           (uint16)tm_util_nearest_long(w_heading_angle * HEADING_SCALE);

        p_ref_loc->u_HeadingIsValid = TRUE;
        p_ref_loc->f_Heading = (float)w_heading_angle;

      } /* supl_end structure data sanity check ok */

    }  /* close if C_SUPL_ASN1_CHOICE_VELCITY_HORANDVERVEL */

    else if (p_suplEnd->position.velocity.t == C_SUPL_ASN1_CHOICE_VELCITY_HORVELUNCERT)
    {

      /**** horizontal speed **************/

      if (p_suplEnd->position.velocity.u.horveluncert == NULL)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "velocity data buffer NULL, discarded", 0, 0, 0);
      }
      else if ((p_suplEnd->position.velocity.u.horveluncert->horspeed.numbits != 16) ||
               (p_suplEnd->position.velocity.u.horveluncert->bearing.numbits != 9) ||
               (p_suplEnd->position.velocity.u.horveluncert->uncertspeed.numbits != 8)
               )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "velocity data length error, discarded", 0, 0, 0);
      }
      else
      {

        p_pd_info_buffer->pd_info.pd_data.opt_field_mask |= PDSM_PD_VELOCITY_VALID;
        p_pd_info_buffer->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_EXT_VELOCITY_VALID;
        p_ref_loc->u_VelocityIsValid = TRUE;

        w_horizontal_speed = (uint16)(p_suplEnd->position.velocity.u.horveluncert->horspeed.data[0]);
        w_horizontal_speed <<= 8;
        w_horizontal_speed |= (uint16)(p_suplEnd->position.velocity.u.horveluncert->horspeed.data[0]);

        /* SUPL provides verspeed in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)w_horizontal_speed * 0.277f;

        /* Compute horizontal velocity from components and scale to units of 0.25 m/s, as per PDAPI spec. */
        p_pd_info_buffer->pd_info.pd_data.velocity_hor =
           (uint16)tm_util_nearest_long(4.0f * f_temp);

        /* Compute horizontal velocity in pdsm_pd_ext_pos_data_s_type velHor
           from components and scale to units of 0.01 m/s, as per PDAPI spec.
           f_verHor is defined as float in pdapi while no bearing is included, so     
           only magnitude of velocity should be reported here and should be limited
           to 0x7FFFF
        */

        p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = 100.0f * f_temp;
        if (p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor > INT16_MAX)
        {
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = INT16_MAX;
        }
        p_ref_loc->f_VelHor = f_temp;

        /**** bearing ******/
        w_heading_angle = (uint16)(p_suplEnd->position.velocity.u.horveluncert->bearing.data[0]);
        w_heading_angle <<= 8;
        w_heading_angle |= (uint16)(p_suplEnd->position.velocity.u.horveluncert->bearing.data[1]);

        p_pd_info_buffer->pd_info.pd_data.heading =
           (uint16)tm_util_nearest_long(w_heading_angle * HEADING_SCALE);

        p_ref_loc->u_HeadingIsValid = TRUE;
        p_ref_loc->f_Heading = (float)w_heading_angle;

        /**** hor. velocity. unc ******/

        w_speed_unc = (uint16)(p_suplEnd->position.velocity.u.horveluncert->uncertspeed.data[0]);

        /* SUPL provides speedunc in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)w_speed_unc * 0.277f;

        /* Compute vertical velocity in pdsm_pd_ext_pos_data_s_type velUncHor
           from components and scale to units of 0.01 m/s, as per PDAPI spec. */
        p_pd_info_buffer->pd_info.pd_data.ext_pos_data.velUncHoriz =
           (uint16)tm_util_nearest_long(100.0f * f_temp);

      } /* supl_end structure data sanity check ok */
    }  /* close if supl_horveluncert */

    else if (p_suplEnd->position.velocity.t == C_SUPL_ASN1_CHOICE_VELCITY_HORANDVERUNCERT)
    {

      /***** horizontal speed *************************/

      if (p_suplEnd->position.velocity.u.horandveruncert == NULL)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_END velocity data error, discarded", 0, 0, 0);
      }
      else if ((p_suplEnd->position.velocity.u.horandveruncert->horspeed.numbits != 16) ||
               (p_suplEnd->position.velocity.u.horandveruncert->verspeed.numbits != 8) ||
               (p_suplEnd->position.velocity.u.horandveruncert->verdirect.numbits != 1) ||
               (p_suplEnd->position.velocity.u.horandveruncert->bearing.numbits != 9) ||
               (p_suplEnd->position.velocity.u.horandveruncert->horuncertspeed.numbits != 8) ||
               (p_suplEnd->position.velocity.u.horandveruncert->veruncertspeed.numbits != 8)
               )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_END velocity data error, discarded", 0, 0, 0);
      }

      else
      {
        p_pd_info_buffer->pd_info.pd_data.opt_field_mask |= PDSM_PD_VELOCITY_VALID;
        p_pd_info_buffer->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_EXT_VELOCITY_VALID;
        p_ref_loc->u_VelocityIsValid = TRUE;

        w_horizontal_speed = (uint16)(p_suplEnd->position.velocity.u.horandveruncert->horspeed.data[0]);
        w_horizontal_speed <<= 8;
        w_horizontal_speed |= (uint16)(p_suplEnd->position.velocity.u.horandveruncert->horspeed.data[1]);

        /* SUPL provides horspeed in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)w_horizontal_speed * 0.277f;

        /* Compute horizontal velocity from components and scale to units of 0.25 m/s, as per PDAPI spec. */
        p_pd_info_buffer->pd_info.pd_data.velocity_hor =
           (uint16)tm_util_nearest_long(4.0f * f_temp);

        /* Compute horizontal velocity in pdsm_pd_ext_pos_data_s_type velHor
           from components and scale to units of 0.01 m/s, as per PDAPI spec.
           f_verHor is defined as float in pdapi while no bearing is included, so     
           only magnitude of velocity should be reported here and should be limited
           to 0x7FFFF
        */
        p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = 100.0f * f_temp;
        if (p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor > INT16_MAX)
        {
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velHor = INT16_MAX;
        }

        p_ref_loc->f_VelHor = f_temp;

        /***** vertical speed *************************/
        u_vertical_speed = p_suplEnd->position.velocity.u.horandveruncert->verspeed.data[0];

        /* SUPL provides verspeed in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)u_vertical_speed * 0.277f;

        if (p_suplEnd->position.velocity.u.horandveruncert->verdirect.data[0] == 0) /* upward */
        {
          p_pd_info_buffer->pd_info.pd_data.velocity_ver =
             (int8)tm_util_nearest_long(2.0f * f_temp);

          /* Compute vertical velocity in pdsm_pd_ext_pos_data_s_type velVert
             from components and scale to units of 0.01 m/s, as per PDAPI spec. */
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velVert = 100.0f  * f_temp;
        }
        else /* downward */
        {
          p_pd_info_buffer->pd_info.pd_data.velocity_ver =
             0 - (int8)tm_util_nearest_long(2.0f * f_temp);

          /* Compute vertical velocity in pdsm_pd_ext_pos_data_s_type velVert
             from components and scale to units of 0.01 m/s, as per PDAPI spec. */
          p_pd_info_buffer->pd_info.pd_data.ext_pos_data.f_velVert =
             (0.0f - (100.0f * f_temp));
        }


        p_ref_loc->f_VelVert = f_temp;

        /************ bearing ********************************/

        w_heading_angle = (uint16)(p_suplEnd->position.velocity.u.horandveruncert->bearing.data[0]);
        w_heading_angle <<= 8;
        w_heading_angle |= (uint16)(p_suplEnd->position.velocity.u.horandveruncert->bearing.data[0]);

        p_pd_info_buffer->pd_info.pd_data.heading =
           (uint16)tm_util_nearest_long(w_heading_angle * HEADING_SCALE);

        p_ref_loc->u_HeadingIsValid = TRUE;
        p_ref_loc->f_Heading = (float)w_heading_angle;

        /**** horizontal velocity unc ******/

        w_speed_unc = (uint16)(p_suplEnd->position.velocity.u.horandveruncert->horuncertspeed.data[0]);

        /* SUPL provides speedunc in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)w_speed_unc * 0.277f;

        /* Compute vertical velocity in pdsm_pd_ext_pos_data_s_type velUncHor
           from components and scale to units of 0.01 m/s, as per PDAPI spec. */
        p_pd_info_buffer->pd_info.pd_data.ext_pos_data.velUncHoriz =
           (uint16)tm_util_nearest_long(100.0f * f_temp);

        /**** vertical velocity unc ******/
        w_speed_unc = (uint16)(p_suplEnd->position.velocity.u.horandveruncert->veruncertspeed.data[0]);

        /* SUPL provides speedunc in the increment of 1 km/hour 16 bits: convert to meter/sec. */
        f_temp = (float)w_speed_unc * 0.277f;

        /* Compute vertical velocity unc. in pdsm_pd_ext_pos_data_s_type verUncVert
           from components and scale to units of 0.01 m/s, as per PDAPI spec. */
        p_pd_info_buffer->pd_info.pd_data.ext_pos_data.velUncVert =
           (uint16)tm_util_nearest_long(100.0f * f_temp);

      } /* supl_end structure data sanity check ok */

    }  /* close if  supl_horandveruncert */

  }  /* close if-velocity available */

}


/*
 ******************************************************************************
 *
 * tm_umts_up_supl_select_pos_prot
 *
 * Function description:
 *  Selects positioning protocol depending on the phone serving system.
 *  If LTE, RRLP is chosen as the positioning protocol.
 *  If 1x,HRPD,eHRPD IS801 is chosen as positioning protocol.
 *
 *  !!!!! MUST HAVE NO SIDE EFFECTS !!!!!
 *
 * Parameters:
 *  Void
 *
 * Return value:
 *   Positioning protocol selected.
 *
 ******************************************************************************
*/

uint8 tm_umts_up_supl_select_pos_prot(void)
{
  tm_cm_phone_state_info_s_type phone_state_info;
  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  /*During 911 call on WLAN, the call can be connected on WIFI Rat. 
    CM can still show RAT L/G/W so check for the call connected RAT for WIFI case*/
  if ( ( phone_state_info.emerg_conn_srv_system == CGPS_SRV_SYS_WLAN ) ||
       ( tm_ds_get_emergency_srv_sys() == CGPS_SRV_SYS_WLAN )
     )
  {
    if (tm_core_get_lte_lpp_up_enable() == TRUE)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPP selected as positioning protocol for WLAN");
      return TM_POS_PRTL_LPP;
    }
  }

  #ifdef FEATURE_NR5G_SA
  if (phone_state_info.srv_system == CGPS_SRV_SYS_NR5G)
  {
    if (tm_core_get_nr_lpp_up_enable() == TRUE)
    {
         MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPP selected as positioning protocol for NR");
         return TM_POS_PRTL_LPP;
       }
       else
       {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "No UP possible on NR");
      return TM_POS_PRTL_NONE;
    }
  }
  #endif /* FEATURE_NR5G_SA */
  
  if (TM_CORE_CONFIG_DD(gnss_1x_up_supl_enable) != 0)
  {
    switch (phone_state_info.srv_system)
    {
      case CGPS_SRV_SYS_CDMA:
      case CGPS_SRV_SYS_HDR:
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "IS801 selected as positioning protocol", 0, 0, 0);
        return TM_POS_PRTL_IS801;
      case CGPS_SRV_SYS_WCDMA:
      case CGPS_SRV_SYS_GSM:
      case CGPS_SRV_SYS_GW:
      case CGPS_SRV_SYS_TDSCDMA:
        return TM_POS_PRTL_RRLP;
      case CGPS_SRV_SYS_LTE:
      case CGPS_SRV_SYS_WLAN:
        if (tm_core_get_lte_lpp_up_enable() == TRUE)  // lpp check for NV item
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPP selected as positioning protocol", 0, 0, 0);
          return TM_POS_PRTL_LPP;
        }
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "RRLP selected as positioning protocol", 0, 0, 0);
        return TM_POS_PRTL_RRLP;
      default:
        return TM_POS_PRTL_NONE;
    }
  }
  if (phone_state_info.srv_system == CGPS_SRV_SYS_LTE)
  {
    if (tm_core_get_lte_lpp_up_enable() == TRUE)
    {
      return TM_POS_PRTL_LPP;
    }
  }

  return TM_POS_PRTL_RRLP;
}

/*
 ******************************************************************************
 *
 * supl_start_is801_session
 *
 * Function description:
 *  Start IS801 session
 *
 * Parameters:
 *   None.
 *
 * Return value:
 *    TRUE or FALSE
 *
 ******************************************************************************
*/

boolean  supl_start_is801_session(boolean wake_tm_core, tm_sess_req_start_param_s_type  *actions)
{
#if defined (FEATURE_CGPS_USES_CDMA) && !defined (FEATURE_GNSS_LOW_MEMORY)

  tm_sess_handle_type sess_handle;
  tm_1x_up_is801_drv_req_param_u_type  req_param;

  if (z_supl_session_status.u_agps_mode != C_SUPL_AGPS_MODE_CELL_ID)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Starting IS801 session", 0, 0, 0);

    sess_handle = z_supl_session_status.tm_active_session_handle;
    req_param.start_param.wake_tm_core = wake_tm_core;
    req_param.start_param.actions      = actions;
    req_param.start_param.start_sess_req_param = &(z_supl_session_status.tm_sess_req_param);

    return tm_1x_up_is801_drv_req(
       sess_handle,
       TM_IS801_DRV_REQ_START,
       req_param
       );
  }
  else
  {
    return FALSE;
  }
#else
  return FALSE; /* dummy for builds without 1X */

#endif /* FEATURE_CGPS_USES_CDMA && ! FEATURE_GNSS_LOW_MEMORY */
}


/*
 ******************************************************************************
 *
 * supl_stop_is801_session
 *
 * Function description:
 *  Stop IS801 session
 *
 * Parameters:
 *   None.
 *
 * Return value:
 *    TRUE or FALSE
 *
 ******************************************************************************
*/

boolean  supl_stop_is801_session(const tm_sess_stop_reason_e_type stop_reason)
{

#if defined (FEATURE_CGPS_USES_CDMA) && !defined (FEATURE_GNSS_LOW_MEMORY)

  tm_sess_handle_type sess_handle;
  tm_1x_up_is801_drv_req_param_u_type  req_param;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Stopping IS801 sessionn. stop reason %d xlated to (TM_STOP_REASON_GENERAL_ERROR)",
        stop_reason, 0, 0);

  sess_handle = z_supl_session_status.tm_active_session_handle;
  req_param.stop_param.stop_reason = TM_STOP_REASON_GENERAL_ERROR;
  req_param.stop_param.stop_type   = PRTL_SESS_STOP_SINGLE_FIX;

  return tm_1x_up_is801_drv_req(sess_handle,
                                TM_IS801_DRV_REQ_STOP,
                                req_param);

#else
  return FALSE; /* dummy for builds without 1X */

#endif  /* FEATURE_CGPS_USES_CDMA && ! FEATURE_GNSS_LOW_MEMORY */

}


/*
 ******************************************************************************
 *
 * tm_umts_up_supl_is801_session_stop
 *
 * Function description:
 *  Handling termination of IS801 session
 *
 * Parameters:
 *   stop_reason: Indicating the reason of termination
 *
 * Return value:
 *    void
 *
 ******************************************************************************
*/
void tm_umts_up_supl_is801_session_stop(const tm_sess_stop_reason_e_type   stop_reason)
{
  if (z_supl_session_status.u_position_protocol == TM_POS_PRTL_IS801)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL-IS801 Stopping Reason %d SUPL CF state %d", stop_reason, z_supl_session_status.e_cf_state, 0);

    if (stop_reason == TM_STOP_REASON_COMPLETED)
    {
      /*SUPL is waiting for IS801 session to stop*/
      if (!(z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT3))
      {
        tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_COMPLETED);
        tm_umts_up_supl_session_init(FALSE);
      }
    }
    else
    {
      /*Handle the error conditions here*/
      if (!(z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT3))
      {
        if (z_pdcomm_tcp_info.curr_state == PDCOMM_STATE_CONNECTED &&
            (z_supl_session_status.u_send_supl_end_at_fail))
        {
          (void)tm_umts_up_supl_supl_end_tx(&z_supl_session_status.z_session_id.z_Slp_Sess_Id,
                                            NULL,
                                            supl_StatusCode_unspecified,
                                            NULL,
                                            z_supl_session_status.q_supl_version);
        }

        tm_umts_up_supl_inform_core_stop_session(stop_reason);
        tm_umts_up_supl_session_init(FALSE);
      }
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_FATAL, "IS801 is not the selected protocol. This event is unexpected", 0, 0, 0);
  }
}

/*
 ******************************************************************************
 *
 * tm_umts_up_supl_is801_nt_assist_done
 *
 * Function description:
 *  Handling close connection from IS801
 *
 * Parameters:
 *    sess_end_sent: Whether IS801 sent session end.
 * Return value:
 *    void
 *
 ******************************************************************************
*/
void tm_umts_up_supl_is801_nt_assist_done(boolean sess_end_sent)
{
  if (z_supl_session_status.u_position_protocol == TM_POS_PRTL_IS801)
  {
    if (!sess_end_sent)
    {
      if (z_pdcomm_tcp_info.curr_state == PDCOMM_STATE_CONNECTED &&
          (!(z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT3)) &&
          (z_supl_session_status.u_send_supl_end_at_fail))
      {
        (void)tm_umts_up_supl_supl_end_tx(&z_supl_session_status.z_session_id.z_Slp_Sess_Id,
                                          NULL,
                                          supl_StatusCode_unspecified,
                                          NULL,
                                          z_supl_session_status.q_supl_version);

        tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_NULL);

        tm_umts_up_supl_comm_close_connection();
      }
    }
    else
    {
      /*Session end is sent, set the state to UT3*/
      supl_set_cf_state(C_SUPL_CF_STATE_UT3, 0);
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_FATAL, "IS801 is not the selected protocol. This even is unexpected", 0, 0, 0);
  }
}

/*
 ******************************************************************************
 *
 * tm_umts_up_supl_lpp_pos_err_reported
 *
 * Function description:
 *  If we report an error to hslp for AGNSS measurements/position, set a flag
 *  so we handle SUPL_END properly
 *
 * Parameters:
 *    void
 * Return value:
 *    void
 *
 ******************************************************************************
*/
void tm_umts_up_supl_lpp_pos_err_reported (void)
{
  if((z_supl_session_status.u_position_protocol == TM_POS_PRTL_LPP) ||
     (z_supl_session_status.u_position_protocol == TM_POS_PRTL_LPPE))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"u_agnss_error_reported = TRUE");
    z_supl_session_status.u_agnss_error_reported = TRUE;
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"LPP is not the selected protocol. This event is unexpected");
  }
}

/*
 ******************************************************************************
 *
 * tm_umts_up_supl_is801_pos_reported
 *
 * Function description:
 *  Handling Reporting of LR from IS801
 *
 * Parameters:
 *    void
 * Return value:
 *    void
 *
 ******************************************************************************
*/
void tm_umts_up_supl_is801_pos_reported(void)
{
  if (z_supl_session_status.u_position_protocol == TM_POS_PRTL_IS801)
  {
    z_supl_session_status.u_pdapi_fix_reported = TRUE;
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_FATAL, "IS801 is not the selected protocol. This even is unexpected", 0, 0, 0);
  }
}

/*
 ******************************************************************************
 *
 * tm_umts_up_supl_is801_pos_reported
 *
 * Function description:
 *  Handling Reporting of LR from IS801
 *
 * Parameters:
 *    void
 * Return value:
 *    void
 *
 ******************************************************************************
*/
void tm_umts_up_supl_is801_inform_1x_pos_method(supl_session_agps_mode_enum_type mode)
{
  z_supl_session_status.u_agps_mode = mode;
}


/*
 ******************************************************************************
 *
 * tm_umts_up_supl_is801_pos_reported
 *
 * Function description:
 *  Handling Reporting of LR from IS801
 *
 * Parameters:
 *    void
 * Return value:
 *    void
 *
 ******************************************************************************
*/
static void check_and_handle_handover(boolean *abort_sending_next_msg)
{
  uint8 new_pos_protocol, old_pos_protocol;
  supl_status_s_type   *p_supl_session_status = &z_supl_session_status;
  tm_sess_req_param_u_type req_param;
  tm_sess_req_param_u_type *p_req_param = &req_param;

  new_pos_protocol = tm_umts_up_supl_select_pos_prot();
  old_pos_protocol = p_supl_session_status->u_position_protocol;

  if (new_pos_protocol != old_pos_protocol)
  {
    /*Handover has happened*/
    if (old_pos_protocol == TM_POS_PRTL_RRLP && new_pos_protocol == TM_POS_PRTL_IS801)
    {
      /*RRLP to IS801*/
      if ( /*NI MS_BASED OR AUTO*/
          (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI &&
           p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED
           )/*NI MS_BASED*/ ||
          p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_AUTO /*AUTO*/)
      {
        /*IS801 does not support NI MS-BASED, start MSA instead*/
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
        p_req_param->continue_param.op_req  = TM_OP_REQ_NONE;
        p_req_param->continue_param.op_mode  = TM_SESS_OPERATION_MODE_MSA;
        p_req_param->continue_param.dynamic_qos_enabled = TRUE;

        if (!tm_sess_req(TM_PRTL_TYPE_UMTS_UP,
                         p_supl_session_status->tm_active_session_handle,
                         TM_SESS_REQ_CONTINUE,
                         p_req_param
                         )
            )
        {
          tm_umts_up_supl_callflow_disrupt_handler();
          *abort_sending_next_msg = TRUE;
          return;
        }

      }
      else if ( /*NI AFLT OR NI ECID OR UI AFLT*/
               (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI &&
                p_supl_session_status->ni_pos_method == POSMETHOD_AFLT
                )/*NI AFLT*/ ||
               (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI &&
                p_supl_session_status->ni_pos_method == POSMETHOD_ECID
                )/*NI ECID*/ ||
               (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_UI &&
                p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED &&
                p_supl_session_status->tm_sess_req_param.pd_qos.gps_session_timeout == 0
                )/*UI AFLT*/)
      {
        if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AFLT_MODE;
          p_supl_session_status->tm_sess_req_param.pd_qos.gps_session_timeout = 0;
        }
      }
    } /*RRLP to IS801*/

    else if (old_pos_protocol == TM_POS_PRTL_IS801 && new_pos_protocol == TM_POS_PRTL_RRLP)
    {
      /*If NI MS_BASED OR NI MS_BASED Pref  set mode to MS-BASED mode*/
      if (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI &&
          (p_supl_session_status->ni_pos_method == POSMETHOD_AGPS_SETBASED ||
           p_supl_session_status->ni_pos_method == POSMETHOD_AGPS_SETASSISTED_PREF)
          )
      {
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
      }
      /*Else if NI AFLT OR UI AFLT OR NI ECID or NI EoTD and NI OTDOA set mode to MS-ASSISTED*/
      else if ( /*NI AFLT OR  NI ECID OR NI EOTD OR NI OTDOA OR UI AFLT*/
               (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI &&
                (p_supl_session_status->ni_pos_method == POSMETHOD_AFLT ||
                 p_supl_session_status->ni_pos_method == POSMETHOD_ECID ||
                 p_supl_session_status->ni_pos_method == POSMETHOD_EOTD ||
                 p_supl_session_status->ni_pos_method == POSMETHOD_OTDOA))/*NI AFLT OR  NI ECID OR NI EOTD OR NI OTDOA*/ ||
               (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_UI &&
                p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED &&
                p_supl_session_status->tm_sess_req_param.pd_qos.gps_session_timeout == 0
                )/*UI AFLT*/)
      {
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
      }

      /*if MS-BASED start MS-BASED*/
      if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
      {
        p_req_param->continue_param.op_mode  = TM_SESS_OPERATION_MODE_MSB;
        p_req_param->continue_param.op_req = TM_OP_REQ_AIDING_DATA_STATUS | TM_OP_REQ_LR;
        p_req_param->continue_param.lr_qos = TM_MAX_SESSION_TIMEOUT;
        *abort_sending_next_msg = TRUE;
      }
      /*else if AUTO*/
      else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_AUTO)
      {
        p_req_param->continue_param.op_mode  = TM_SESS_OPERATION_MODE_MSB;
        p_req_param->continue_param.op_req =  TM_OP_REQ_LR;
        if (p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI)
        {
          p_req_param->continue_param.lr_qos = TM_MAX_SESSION_TIMEOUT;
        }
        else
        {
          p_req_param->continue_param.lr_qos = (p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec / 1000);
        }
      }
      /*else if MS-ASSISTED*/
      else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
      {
        p_req_param->continue_param.op_mode  = TM_SESS_OPERATION_MODE_MSA;
        p_req_param->continue_param.op_req = TM_OP_REQ_PPM;
        p_req_param->continue_param.prm_qos = TM_MAX_SESSION_TIMEOUT;

        /* the wishlist setting is not absolutely needed as it will be set like that when forming
           SuplPosInit */
        p_supl_session_status->z_GpsAssistData.u_Octet3_Whishlist_AH |= C_G0931_GPS_ASSIST_REFTIME;
        p_supl_session_status->z_GpsAssistData.u_Octet3_Whishlist_AH |= C_G0931_GPS_ASSIST_ACQ;
      }


      p_req_param->continue_param.accuracy_threshold = p_supl_session_status->z_session_qos.q_hor_acc_meters;
      p_req_param->continue_param.num_fixes = p_supl_session_status->tm_sess_req_param.pd_option.fix_rate.num_fixes;
      p_req_param->continue_param.tbf_ms = p_supl_session_status->tm_sess_req_param.pd_option.fix_rate.time_between_fixes_ms;
      p_req_param->continue_param.dynamic_qos_enabled = TRUE;

      if (!tm_sess_req(TM_PRTL_TYPE_UMTS_UP,
                       p_supl_session_status->tm_active_session_handle,
                       TM_SESS_REQ_CONTINUE,
                       p_req_param
                       )
          )
      {
        tm_umts_up_supl_callflow_disrupt_handler();
        *abort_sending_next_msg = TRUE;
        return;
      }
    } /*IS801 to RRLP*/

    p_supl_session_status->u_position_protocol = new_pos_protocol;
  }
}

/*
 ******************************************************************************
 *
 * supl_comm_getnext_gm_transaction_id
 *
 * Function description:
 *  Get next transaction id to send request to Geofence module
 *
 * Parameters:
 *    void
 * Return value:
 *    void
 *
 ******************************************************************************
*/
uint32 supl_comm_getnext_gm_transaction_id(void)
{
  if ((gm_transaction_id < TM_UMTS_UP_GM_TRANSACTION_ID_BASE) ||
      (gm_transaction_id >=
          (TM_UMTS_UP_GM_TRANSACTION_ID_BASE + TM_UMTS_UP_GM_TRANSACTION_ID_RANGE)
       )
      )
  {
    gm_transaction_id = TM_UMTS_UP_GM_TRANSACTION_ID_BASE;
  }
  else
  {
    gm_transaction_id++;
  }
  return gm_transaction_id;
}



/*
 ******************************************************************************
 *
 * supl_SET_id_get
 *
 * Function description:
 *   Retrieve SET ID information.
 *
 * Parameters:
 *    void
 * Return value:
 *    void
 *
 ******************************************************************************
*/


static void supl_SET_id_get(void)
{
  boolean b_ret = TRUE;
  byte stored_imei[MAX_IMEI_LEN + 1] = { 0 };
  static boolean b_set_id_imei = FALSE;

  tm_cm_phone_state_info_s_type phone_state_info;

  memset((void *)&phone_state_info, 0, sizeof(phone_state_info));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if ((b_set_id_imei == TRUE) &&
      (phone_state_info.e911_state == TM_CM_E911_STATE_NONE))
  {
    /* if we stored IMEI as IMSI, reset it as we are no longer in 911 state */
    memset((void *)&z_Set_Id, 0, sizeof(z_Set_Id));
    b_set_id_imei = FALSE;
  }
#ifndef FEATURE_CGPS_SUPL_MSISDN_FIRST /* default: not defined, will use IMSI first */

  /* decide SET ID if not yet available to SUPL, try to retrieve IMSI */
  if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
  {
    z_Set_Id.set_id_choice = T_supl_SETId_imsi;
    if (z_umts_nv_item_ram_copy.set_imsi[0] ==  '\0')
    {
      /* IMSI value has not yet been read from the SIM*/
      if (tm_umts_up_supl_comm_retrieve_imsi() == FALSE)
      {
        z_Set_Id.set_id_choice = C_SETID_UNKNOWN;
      }
    }

    /* Retrieving the copy of the IMSI stored in the Ram */
    if (z_Set_Id.set_id_choice == T_supl_SETId_imsi)
    {
      memscpy(z_Set_Id.u.imsi.data, sizeof(z_Set_Id.u.imsi.data),
              &z_umts_nv_item_ram_copy.set_imsi[0], sizeof(z_umts_nv_item_ram_copy.set_imsi));
      z_Set_Id.u.imsi.numocts = MAX_IMSI_LEN_BYTES;
    }
  } /* close if SETID unknown */

  /* if still no SET_ID, try MSISDN */
  if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
  {
    uint8 u_i, u_j;
    pbm_return_type pbm_return       = PBM_ERROR;
    uint8 u_num_len                  = PBM_NUM_SIZE;
    char *msisdn_number              = NULL;

    memset(z_Set_Id.u.msisdn.data, 0xFF, C_SUPL_SETID_MSISDN_BYTE_LENGTH);

    if (NULL != (msisdn_number = (char *)pdsm_getbuf(u_num_len * sizeof(char))))
    {
      pbm_return = supl_get_msisdn_number(msisdn_number, &u_num_len);
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "msisdn_number::Not enough memory on heap !!");
    }

    if (pbm_return == PBM_SUCCESS)
    {
      if (msisdn_number[0] == '+')
      {
        u_i = 1;
      }
      else
      {
        u_i = 0;
      }
      u_j = 0;

      while (u_j < C_SUPL_SETID_MSISDN_BYTE_LENGTH)
      {
        if ((msisdn_number[u_i] >= 0x30) && (msisdn_number[u_i] <= 0x39))
        {
          z_Set_Id.u.msisdn.data[u_j] = msisdn_number[u_i] & 0x0F;

          u_i++;

          if ((msisdn_number[u_i] >= 0x30) && (msisdn_number[u_i] <= 0x39))
          {
            z_Set_Id.u.msisdn.data[u_j] |= msisdn_number[u_i] << 4;
            u_i++;
          }
          else
          {
            z_Set_Id.u.msisdn.data[u_j] |= 0xF0; /* fill unused with F */
            break;
          }
          u_j++;
        }
        else /*  terminate if not 0-9 ascii */
        {
          break;
        }
      };

      if (u_i > 4) /* at least there are 4 valid characters */
      {
        z_Set_Id.set_id_choice = T_supl_SETId_msisdn;
        z_Set_Id.u.msisdn.numocts = C_SUPL_SETID_MSISDN_BYTE_LENGTH; /* SUPL dictates 8 octects */
      }
    } /* if pbm return success */

    (void)pdsm_freebuf((char *)msisdn_number);
  } /* if still no SET_ID */


#else  /************* Special feature: use MSISDN first *************/

  /* if no SET_ID, try MSISDN first  */
  if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
  {
    uint8 u_i, u_j;
    pbm_return_type pbm_return       = PBM_ERROR;
    uint8 u_num_len                  = PBM_NUM_SIZE;
    char *msisdn_number              = NULL;

    memset(z_Set_Id.u.msisdn.data, 0xFF, C_SUPL_SETID_MSISDN_BYTE_LENGTH);

    if (NULL != (msisdn_number = (char *)pdsm_getbuf(u_num_len * sizeof(char))))
    {
      pbm_return = supl_get_msisdn_number(msisdn_number, &u_num_len);
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "msisdn_number::Not enough memory on heap !!");
    }

    if (pbm_return == PBM_SUCCESS)
    {
      if (msisdn_number[0] == '+')
      {
        u_i = 1;
      }
      else
      {
        u_i = 0;
      }
      u_j = 0;

      while (u_j < C_SUPL_SETID_MSISDN_BYTE_LENGTH)
      {
        if ((msisdn_number[u_i] >= 0x30) && (msisdn_number[u_i] <= 0x39))
        {
          z_Set_Id.u.msisdn.data[u_j] = msisdn_number[u_i] & 0x0F;

          u_i++;

          if ((msisdn_number[u_i] >= 0x30) && (msisdn_number[u_i] <= 0x39))
          {
            z_Set_Id.u.msisdn.data[u_j] |= msisdn_number[u_i] << 4;
            u_i++;
          }
          else
          {
            z_Set_Id.u.msisdn.data[u_j] |= 0xF0; /* fill unused with F */
            break;
          }
          u_j++;
        }

        else /* terminate if not 0-9 ascii */
        {
          break;
        }
      };

      if (u_i > 4) /* at least there are 4 valid characters */
      {
        z_Set_Id.set_id_choice = C_SUPL_ASN1_CHOICE_SETID_MSISDN;
        z_Set_Id.u.msisdn.numocts = C_SUPL_SETID_MSISDN_BYTE_LENGTH; /* SUPL dictates 8 octects */
      }
    } /* if pbm return success */

    (void)pdsm_freebuf((char *)msisdn_number);
  } /* if no SET_ID */

  /* Still no SET_ID available to SUPL, try to retrieve IMSI */
  if (z_Set_Id.set_id_choice == C_SETID_UNKNOWN)
  {
    z_Set_Id.set_id_choice = T_supl_SETId_imsi;
    if (z_umts_nv_item_ram_copy.set_imsi[0] ==  '\0')
    {
      /* IMSI value has not yet been read from the SIM*/
      if (tm_umts_up_supl_comm_retrieve_imsi() == FALSE)
      {
        z_Set_Id.set_id_choice = C_SETID_UNKNOWN;
      }
    }
    /* Retrieving the copy of the IMSI stored in the Ram */
    if (z_Set_Id.set_id_choice == T_supl_SETId_imsi)
    {
      z_Set_Id.u.imsi.numocts = memscpy(z_Set_Id.u.imsi.data, sizeof(z_Set_Id.u.imsi.data),
                                        &z_umts_nv_item_ram_copy.set_imsi[0], sizeof(z_umts_nv_item_ram_copy.set_imsi));
    }
  } /* close if SETID unknown */

#endif /* use MSIDSN first */


  /* if still no SET_ID, check if IMEI is available and use that in leiu of IMSI for E911 */
  if ((z_Set_Id.set_id_choice == C_SETID_UNKNOWN) &&
      (phone_state_info.e911_state != TM_CM_E911_STATE_NONE))
  {
    b_ret = tm_get_imei(stored_imei);
    if (b_ret == TRUE  && stored_imei[0] == MAX_IMEI_LEN)
    {
      z_Set_Id.set_id_choice = T_supl_SETId_imsi;
      /* imei[0] is the length  */
      memscpy(z_Set_Id.u.imsi.data, MAX_IMEI_LEN,
              &stored_imei[1], MAX_IMEI_LEN);
      z_Set_Id.u.imsi.numocts = MAX_IMEI_LEN;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL SET_id Get IMEI succeeded", 0, 0, 0);
      b_set_id_imei = TRUE;
    }    
  }  

#if (defined(T_WINNT) && defined(SMART_OSYS))
  #error code not present
#endif

}

/*
 ******************************************************************************
 *
 * supl_SLP_session_id_match_check
 *
 * Function description:
 *   Determine if the incoming message has matching SLP session ID with that of
 *   a cached copy.
 *
 * Parameters:
 *   p_cached_Slp_Sess_Id : pointer to the cached copy of the SLP session ID.
 *   p_incoming_Slp_Sess_Id: pointer to the incoming message's SLP session ID.
 *
 * Return value:
 *   TRUE: SLP session IDs match; FALSE: SLP session IDs do not match.
 *
 ******************************************************************************
*/


static boolean supl_SLP_session_id_match_check
(supl_slp_session_id *p_cached_Slp_Sess_Id, supl_slp_session_id *p_incoming_Slp_Sess_Id)
{
  boolean  u_session_id_consistent = TRUE;

  if ((p_cached_Slp_Sess_Id->presence == TRUE) &&
      (p_incoming_Slp_Sess_Id->presence == TRUE)
      )
  {
    /* Session ID */
    if (memcmp((unsigned char *)p_cached_Slp_Sess_Id->session_Id,
               (unsigned char *)p_incoming_Slp_Sess_Id->session_Id,
               C_SUPL_SLP_SESSION_ID_BYTE_LENGTH) != 0)
    {
      u_session_id_consistent = FALSE;
    }

    /* SLP ID */
    if (p_incoming_Slp_Sess_Id->zSlpAddress.choice == C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS)
    {
      /* IP Address */
      if (p_incoming_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.choice == C_SUPL_ASN1_CHOICE_IPV4ADDRESS)
      {
        /* IPV4 */
        p_cached_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.numocts = 4;
        if (memcmp(p_cached_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.data,
                   p_incoming_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.u.Ip_V4.data,
                   4) ||
            (p_cached_Slp_Sess_Id->zSlpAddress.choice != C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS) ||
            (p_cached_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.choice != C_SUPL_ASN1_CHOICE_IPV4ADDRESS)
            )
        {
          u_session_id_consistent = FALSE;
        }
      }
      else
      {
        /* IPV6 */
        p_cached_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.numocts = 16;
        if (memcmp(p_cached_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.data,
                   p_incoming_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.u.Ip_V6.data,
                   16) ||
            (p_cached_Slp_Sess_Id->zSlpAddress.choice != C_SUPL_ASN1_CHOICE_SLPADDRESS_IPADDRESS) ||
            (p_cached_Slp_Sess_Id->zSlpAddress.u.z_Ip_Address.choice != C_SUPL_ASN1_CHOICE_IPV6ADDRESS)
            )
        {
          u_session_id_consistent = FALSE;
        }
      }
    }
    else
    {
      uint8 fqdn_len = p_cached_Slp_Sess_Id->zSlpAddress.u.z_fqdn.u_length;

      /* FQDN */
      if ((fqdn_len != p_incoming_Slp_Sess_Id->zSlpAddress.u.z_fqdn.u_length) ||
          memcmp(p_cached_Slp_Sess_Id->zSlpAddress.u.z_fqdn.fqdn,
                 p_incoming_Slp_Sess_Id->zSlpAddress.u.z_fqdn.fqdn,
                 fqdn_len) ||
          (p_cached_Slp_Sess_Id->zSlpAddress.choice != C_SUPL_ASN1_CHOICE_SLPADRESS_FQDN)
          )
      {
        u_session_id_consistent = FALSE;
      }
    }

  }

  return  u_session_id_consistent;

}


/*
 ******************************************************************************
 *
 * supl_init_trigger_event_check
 *
 * Function description:
 *   Determine if event trigger session is requested.
 *
 * Parameters:
 *    void
 * Return value:
 *    Value of the supl_init_trigger_type_enum_type (NULL, areaEvent, periodic
 *    or wrong type indication ).
 *
 ******************************************************************************
*/


static supl_init_trigger_type_enum_type supl_init_trigger_event_check(void)
{

  if ((q_supl_ver_maj_after_supl_init_glob & 0x000000FF) < 2)
  {
    return C_SUPL_INIT_TRIG_TYPE_NULL;
  }

  if (z_supl_init_trigger_type.trigger_type_present)
  {
    if (z_supl_init_trigger_type.q_trigger_type == 1)
    {
      return C_SUPL_INIT_TRIG_TYPE_AREA_EVENT;
    }
    else if (z_supl_init_trigger_type.q_trigger_type == 0)
    {
      return C_SUPL_INIT_TRIG_TYPE_PERIODIC;
    }
    else
    {
      return C_SUPL_INIT_TRIG_TYPE_WRONG_DEF;
    }
  }
  else
  {
    return C_SUPL_INIT_TRIG_TYPE_NULL;
  }

}


/*
 ******************************************************************************
 *
 * supl_init_trigger_event_proc
 *
 * Function description:
 *   Process trigger event from SUPL_INIT.
 *
 * Parameters:
 *    p_req_data: pointer to pdapi client provided data including info. from
 *                SUPL_INIT.
 *    p_slp_id:   pointer to SLP ID.
 *
 * Return value:
 *    None.
 *
 ******************************************************************************
*/


static void  supl_init_trigger_event_proc
(pdsm_lcs_supl_notify_verify_req_s_type *p_req_data,
 supl_slp_session_id                    *p_slp_id,
 tm_cm_phone_state_info_s_type phone_state_info
 )
{

  uint8 u_i, u_session_num;

  /* for now only MSB supported */
  if (!((p_req_data->pos_method == POSMETHOD_AGPS_SETBASED) ||
        (p_req_data->pos_method == POSMETHOD_AGPS_SETBASED_PREF)
        )
      )
  {

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Unsupported pos_mothod for areaTriggerEvent: %u",
          p_req_data->pos_method, 0, 0);

    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_slp_id,
                                                 NULL,
                                                 supl_StatusCode_posMethodMismatch,
                                                 p_req_data->supl_hash,
                                                 q_supl_ver_maj_after_supl_init_glob);
    return;
  }

  for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
  {
    z_supl_area_event_session_status[u_i].u_position_protocol = tm_umts_up_supl_select_pos_prot();
    /* find an open slot for the session */
    if (z_supl_area_event_session_status[u_i].u_session_active == FALSE)
    {
      /* slot found, take this slot */
      z_supl_area_event_session_status[u_i].u_session_active = TRUE;


      if ((p_req_data->pos_method == POSMETHOD_AGPS_SETBASED) ||
          (p_req_data->pos_method == POSMETHOD_AGPS_SETBASED_PREF)
          )
      {

        /*NI MS BASED only if MS-BASED supported and RRLP is used as pos protocol*/
        if (TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED | PDSM_POS_MODE_4G_UP_MS_BASED) != 0)
        {
          z_supl_area_event_session_status[u_i].u_agps_mode =
             C_SUPL_AGPS_MODE_UE_BASED;
        }
      }

      memscpy(z_supl_session_status.supl_init_hash, sizeof(z_supl_session_status.supl_init_hash),
              p_req_data->supl_hash, sizeof(p_req_data->supl_hash));
      z_supl_session_status.u_hash_valid = TRUE;

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_INIT takes %u slot for areaEvent session", u_i, 0, 0);
      break;
    }
  }

  if (u_i >= C_MAX_GEOFENCE_NUM)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Exceeding number of areaTriggerEvent session : %u",
          u_i, 0, 0);

    (void)tm_umts_up_supl_supl_end_prepare_or_tx(p_slp_id,
                                                 NULL,
                                                 supl_StatusCode_resourceShortage,
                                                 p_req_data->supl_hash,
                                                 q_supl_ver_maj_after_supl_init_glob);
    return;
  }

  /************** store SLP session ID ************************************/

  /** for later use **/
  z_supl_area_event_session_status[u_i].z_session_id.z_Slp_Sess_Id = *p_slp_id;
  z_supl_area_event_session_status[u_i].q_supl_version = q_supl_ver_maj_after_supl_init_glob;

  /* for the immediate use of processing SLP messages */
  z_supl_session_status.z_session_id.z_Slp_Sess_Id = *p_slp_id;
  z_supl_session_status.q_supl_version = q_supl_ver_maj_after_supl_init_glob;

  q_supl_ver_maj_after_supl_init_glob = 0;
  u_session_num = u_i;

  /* Request MRL updates */
  supl_request_l1_mrl_updates(phone_state_info.srv_system);

  /* check if cached cell ID is valid and not stale: if not, request new
     cell ID info. from L1 */
  if (tm_umts_supl_if_need_forced_wcmda_cell_inq() == FALSE)
  {
    /* cell info. valid and current, start SLP contact */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "start SLP contact", 0, 0, 0);
    /* Initializing the flags so that we can attempt a fresh
       connection*/
    z_pdcomm_tcp_info.tls_session_authentication_failure = FALSE;
    z_pdcomm_tcp_info.used_auto_configured_hslp_address = FALSE;
    tm_umts_up_supl_start_slp_contact_area_event(u_session_num);
  }
  else
  {
    if (!tm_umts_up_supl_req_cell_info_from_rrc(C_SUPL_CF_STATE_CELL_INFO_REQ_AREA_EVENT))
    {
      /* Initializing the flags so that we can attempt a fresh
         connection*/
      z_pdcomm_tcp_info.tls_session_authentication_failure = FALSE;
      z_pdcomm_tcp_info.used_auto_configured_hslp_address = FALSE;
      /* effort to update cell info. fails, go ahead anyway hoping new
         update may come before it's ready to send the message */
      tm_umts_up_supl_start_slp_contact_area_event(u_session_num);
    }
    else /* not contacting SLP yet, give RRC chance to provide CELL info */
    {
      z_supl_area_event_session_status[u_session_num].q_slp_contact_pending |=
         C_AREA_EVENT_PENDING_SLP_CONTACT_TRIIGER_START;
    }
  }
}


/*
 ******************************************************************************
 *
 * tm_umts_up_supl_start_slp_contact_area_event
 *
 * Function description:
 *  Actions upon tcp connection is established for the use of area event
 *  sessions.
 *
 * Parameters:
 *   u_session_num: index to the active area event session.
 *                  0xFF: no knowledge of which area event session needing SLP
 *                        at the time of calling this function.
 *
 * Return value:
 *    None.
 *
 ******************************************************************************
*/


void tm_umts_up_supl_start_slp_contact_area_event(uint8 u_session_num)
{
  pd_comms_return_type          pdcomm_connect_ret_value;

  tm_cm_phone_state_info_s_type phone_state_info;
  uint8  u_i;

  if (u_session_num == 0xFF)
  {
    /* need to search for the relevant areaEvent session */
    for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
    {
      if (z_supl_area_event_session_status[u_i].q_slp_contact_pending &
             C_AREA_EVENT_PENDING_SLP_CONTACT_TRIIGER_START)
      {
        u_session_num = u_i;
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
              "Identified AreaEventSession %u for SLP contact start",
              u_session_num);
        break;
      }
    }
  }

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "Invalid areaEvent session num: %u",
          u_session_num);
    return;
  }

  tm_cm_iface_get_phone_state(&phone_state_info);

  pdcomm_connect_ret_value = tm_umts_up_supl_comm_tcp_connect(SUPL_VERSION_MAIN_2);

  if (PDCOMM_RESULT_OK == pdcomm_connect_ret_value) /* Already got connected */
  {
    /* Can't run SUPL over LTE if SUPL major version less than 2,
       even though previous negotiation with SUPL_INIT has resulted
       in running SUPL 1.0 */
    if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) &&
        ((z_supl_area_event_session_status[u_session_num].q_supl_version & 0xFF) < 2)
        )
    {
      /* send SUPL_END, using the provisioned SUPL version in its
         message header, and abort this session */

      z_supl_area_event_session_status[u_session_num].q_supl_version =
         ((z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF) |
            (z_umts_nv_item_ram_copy.gnss_supl_version & 0x0000FF00) |
            ((z_umts_nv_item_ram_copy.gnss_supl_version << 16) & 0x00FF0000);

      (void)tm_umts_up_supl_supl_end_tx(
         &z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id,
         NULL,
         supl_versionNotSupported,
         &z_supl_area_event_session_status[u_session_num].supl_init_hash[0],
         z_supl_area_event_session_status[u_session_num].q_supl_version);

      tm_umts_up_supl_comm_close_connection();
      return;
    }

    /* send SUPL_TRIGGER_START */



  } /*Already got connected*/

  else if (PDCOMM_RESULT_WAIT == pdcomm_connect_ret_value) /* most likely */
  {
    supl_set_cf_state_area_event(C_SUPL_CF_STATE_UT0_SUPLTRIGGERSTART, u_session_num);
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Starting UT0 for SUPL_TriggerStart", 0, 0, 0);
  }

  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: TCP conn. failed", 0, 0, 0);
  }
}


/*
 ******************************************************************************
 *
 * supl_set_cf_state_area_event
 *
 * Function description:
 *   Area event related session state handling.
 *
 * Parameters:
 *   u_session_num: index to the active area event session.
 *
 * Return value:
 *    None.
 *
 ******************************************************************************
*/


static void supl_set_cf_state_area_event
(supl_cf_state_enum_type e_new_supl_cf_state, uint8 u_session_num)
{

  if ((e_new_supl_cf_state == C_SUPL_CF_STATE_NULL) ||
      (u_session_num >= C_MAX_GEOFENCE_NUM)
      )
    return;

  (void)os_TimerStop(supl_cf_timer); /* stop any current running timer */

  if ((e_new_supl_cf_state & C_SUPL_CF_STATE_UT0_SUPLTRIGGERSTART) ||
      (e_new_supl_cf_state & C_SUPL_CF_STATE_UT0_SUPLREPORT_GF_BREACH)
      )
  {
    /* "UT0": waiting for TLS link to establish  */
    (void)os_TimerStart(supl_cf_timer, SUPL_TIMER_UT0_VALUE_MSEC, NULL);
  }
  else if (e_new_supl_cf_state & C_SUPL_CF_STATE_UT1_SUPLTRIGGERRESPONSE)
  {
    (void)os_TimerStart(supl_cf_timer, z_umts_nv_item_ram_copy.aagps_ut1_value * 1000, NULL);
  }
  else if (e_new_supl_cf_state & C_SUPL_CF_STATE_UT3_LAST_SUPLREPORT)
  {
    (void)os_TimerStart(supl_cf_timer, z_umts_nv_item_ram_copy.aagps_ut3_value * 1000, NULL);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Unexpected areaEvent cf_state: %u", e_new_supl_cf_state, 0, 0);
  }

  z_supl_area_event_session_status[u_session_num].e_cf_state |= e_new_supl_cf_state;
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL CF State: 0x%x", z_supl_area_event_session_status[u_session_num].e_cf_state, 0, 0);

}


/*
 ******************************************************************************
 *
 * supl_is_in_any_ut0
 *
 * Function description:
 *   Check if it is waiting for data connection to be established.
 *
 * Parameters:
 *   None.
 *
 * Return value:
 *   TRUE: yes it is waiting for data connection to be established.
 *   FALSE: no it is not waiting.
 *
 ******************************************************************************
*/

static boolean supl_is_in_any_ut0(void)
{
  uint8 u_i;
  supl_status_s_type *p_supl_session_status = &z_supl_session_status;
  boolean u_ret = FALSE;

  if ((p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLSTART) ||
      (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLPOSINIT) ||
      (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLEND) ||
      (p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT0_SUPLREPORT_QUERY)
      )
  {
    return TRUE;
  }

  for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
  {
    if (z_supl_area_event_session_status[u_i].u_session_active)
    {

      if ((z_supl_area_event_session_status[u_i].e_cf_state &
              C_SUPL_CF_STATE_UT0_SUPLTRIGGERSTART) ||
          (z_supl_area_event_session_status[u_i].e_cf_state &
              C_SUPL_CF_STATE_UT0_SUPLTRIGSTOP) ||
          (z_supl_area_event_session_status[u_i].e_cf_state &
              C_SUPL_CF_STATE_UT0_SUPLREPORT_GF_BREACH)
          )
      {
        u_ret = TRUE;
        break;
      }
    }
  }

  return u_ret;

}


/*
 ******************************************************************************
 *
 * tm_umts_up_supl_terminate_timer_cancel_cf_state_area_event
 *
 * Function description:
 *   Area event related session timer termination.
 *
 * Parameters:
 *   u_session_num: index to the active area event session.
 *
 * Return value:
 *    None.
 *
 ******************************************************************************
*/


void tm_umts_up_supl_terminate_timer_cancel_cf_state_area_event
(supl_cf_state_enum_type      e_timer_state_to_terminate,
 uint8                        u_session_num)
{

  (void)os_TimerStop(supl_cf_timer);

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
    return;

  if (e_timer_state_to_terminate != C_SUPL_CF_STATE_NULL)
  {
    z_supl_area_event_session_status[u_session_num].e_cf_state &=
       (supl_cf_state_enum_type)~e_timer_state_to_terminate;
  }
  else
  {
    z_supl_area_event_session_status[u_session_num].e_cf_state =
       C_SUPL_CF_STATE_NULL;
  }
}


/*
 ******************************************************************************
 *
 * supl_SuplTriggerStart_tx
 *
 * Function description:
 *   Formulating and sending of SUPL_TRIGGER_START.
 *
 * Parameters:
 *   u_session_num: index to the active area event session.
 *
 * Return value:
 *    None.
 *
 ******************************************************************************
*/


static boolean supl_SuplTriggerStart_tx(uint8 u_session_num)
{

  tm_pdu_buf_type pdu_buff; /* including length and pointer to data */
  supl_ULP_PDU      pdu;

  supl_Ver2_SUPLTRIGGEREDSTART *p_SuplTriggerStart;
  supl_Ver2_SUPLTRIGGEREDSTART    z_SuplTriggerStart;
  supl_SETCapabilities            z_sETCapabilities;
  supl_SETCapabilities *p_sETCapabilities;
  supl_ServiceCapabilities *p_service_capabilities;

  supl_cell_info_struct_type  z_cell_info_allocated;

  uint8 u_num_mlid = 0;
  supl_LocationIdData *p_LocationIdData = NULL;
  supl_LocationId     *p_LocationId = NULL;
  supl_cell_info_struct_type  *p_cell_info_allocated_mlid = NULL;

  int                         encode_status;

  uint8 *p_length;
  uint8            u_length_msb = 0;
  uint8            u_length_lsb = 0;

  boolean          u_ret  = FALSE;

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
    return FALSE;

  memset(&pdu, 0x00, sizeof(supl_ULP_PDU));
  memset(&z_SuplTriggerStart, 0x00, sizeof(supl_Ver2_SUPLTRIGGEREDSTART));
  memset(&z_sETCapabilities, 0x00, sizeof(supl_SETCapabilities));
  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));

  p_SuplTriggerStart = &z_SuplTriggerStart;
  pdu.message.t = C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLTRIGSTART;
  pdu.message.u.msSUPLTRIGGEREDSTART = p_SuplTriggerStart;

  p_sETCapabilities = &z_sETCapabilities;
  z_SuplTriggerStart.sETCapabilities = p_sETCapabilities;

  /* overhead build function has to be after we know what message is to be constructed */
  z_supl_session_status.z_session_id.z_Set_Sess_Id.w_Session_Id = w_SetSessionID;
  z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id.w_Session_Id = w_SetSessionID;

  supl_SET_id_get();
  memscpy(&z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id.z_Set_Id,
          sizeof(z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id.z_Set_Id),
          &z_Set_Id, sizeof(z_Set_Id)
          );

  w_SetSessionID++;

  if (!supl_header_build(&pdu,
                         &z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id.w_Session_Id,
                         NULL,
                         &z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id,
                         z_supl_area_event_session_status[u_session_num].q_supl_version)
      )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplStart attempt aborted: header", 0, 0, 0);
    return FALSE;
  }


  /* Positioning technology */
  p_sETCapabilities->posTechnology.aFLT = FALSE;
  p_sETCapabilities->posTechnology.eOTD = FALSE;
  p_sETCapabilities->posTechnology.oTDOA = FALSE;
  p_sETCapabilities->posTechnology.eCID = FALSE; /* maybe modified below */


  if (z_supl_area_event_session_status[u_session_num].u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
  {
    p_sETCapabilities->posTechnology.agpsSETassisted = TRUE;

    if (z_supl_area_event_session_status[u_session_num].u_position_protocol == TM_POS_PRTL_LPP)
    {
      p_sETCapabilities->posTechnology.oTDOA = TRUE;
    }
  }

  else if (z_supl_area_event_session_status[u_session_num].u_agps_mode == C_SUPL_AFLT_MODE)
  {
    if (z_supl_area_event_session_status[u_session_num].u_position_protocol == TM_POS_PRTL_IS801)
    {
      p_sETCapabilities->posTechnology.aFLT = TRUE;
    }
  }

  else if (z_supl_area_event_session_status[u_session_num].u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
  {
    p_sETCapabilities->posTechnology.agpsSETBased = TRUE;

    /*IS801 does not support Autonomous GPS mode*/
    if (z_supl_area_event_session_status[u_session_num].u_position_protocol != TM_POS_PRTL_IS801)
    {
#ifndef FEATURE_CGPS_SUPL_VARIANT_SET_CAP_NO_AUTO /* NOT by default defined */
      p_sETCapabilities->posTechnology.autonomousGPS = TRUE;
#endif
    }

  }
  else if (z_supl_area_event_session_status[u_session_num].u_agps_mode == C_SUPL_AGPS_MODE_CELL_ID)
  {
    p_sETCapabilities->posTechnology.agpsSETassisted = FALSE;
    p_sETCapabilities->posTechnology.autonomousGPS = FALSE;
    p_sETCapabilities->posTechnology.agpsSETBased = FALSE;
  }

  else if (z_supl_area_event_session_status[u_session_num].u_agps_mode == C_SUPL_OTDOA_MODE)
  {
    p_sETCapabilities->posTechnology.oTDOA = TRUE;
  }
  else
  {
    /* no other agps mode should generate SUPL_POS_INIT */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SuplTriggerStart_tx attempt aborted", 0, 0, 0);
    return FALSE;
  }

  p_sETCapabilities->prefMethod = supl_PrefMethod_noPreference;

#ifdef FEATURE_CGPS_SUPL_VARIANT_SET_CAP_PREF_SET_BASED  /* NOT by defaut defined */
  #error code not present
#endif


  if (z_supl_area_event_session_status[u_session_num].u_position_protocol == TM_POS_PRTL_LPP)
  {
    p_sETCapabilities->posProtocol.m.ver2_PosProtocol_extensionPresent = 1;
    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.lpp = TRUE;

    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.m.posProtocolVersionLPPPresent = 1;
    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.majorVersionField = LPP_CURRENT_VERSION_MAJOR;
    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.technicalVersionField = LPP_CURRENT_VERSION_TECH;
    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.posProtocolVersionLPP.editorialVersionField = LPP_CURRENT_VERSION_EDIT;
  }
  else
  {
    p_sETCapabilities->posProtocol.rrlp = TRUE;
    /***********  PosProtocolVersionRRLP SUPL2 extension in PosProtocol ****/

    p_sETCapabilities->posProtocol.m.ver2_PosProtocol_extensionPresent = 1;

    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.m.posProtocolVersionRRLPPresent = 1;

    /* default to the backward compatible version first, may be updated below */
    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.majorVersionField =
       SUPL_RRLP_CURRENT_VERSION_MAJOR;
    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.technicalVersionField =
       SUPL_RRLP_CURRENT_VERSION_TECH;
    p_sETCapabilities->posProtocol.ver2_PosProtocol_extension.posProtocolVersionRRLP.editorialVersionField =
       SUPL_RRLP_CURRENT_VERSION_EDIT;
  }
  /* positioning protocol */

  p_sETCapabilities->m.ver2_SETCapabilities_extensionPresent = 1;

  p_sETCapabilities->ver2_SETCapabilities_extension.m.serviceCapabilitiesPresent = 1;

  p_service_capabilities = &p_sETCapabilities->ver2_SETCapabilities_extension.serviceCapabilities;

  p_service_capabilities->servicesSupported.areaEventTrigger = 1;

  p_service_capabilities->m.eventTriggerCapabilitiesPresent = 1;

  p_service_capabilities->eventTriggerCapabilities.m.maxAreaIdListSupportedPresent = 1;
  p_service_capabilities->eventTriggerCapabilities.maxAreaIdListSupported = C_MAX_NUM_AREA_ID_LIST; /* 32 */

  p_service_capabilities->eventTriggerCapabilities.m.maxAreaIdSupportedPerListPresent = 1;
  p_service_capabilities->eventTriggerCapabilities.maxAreaIdSupportedPerList = C_MAX_NUM_AREA_ID_PER_LIST; /* 256 */

  p_service_capabilities->eventTriggerCapabilities.m.maxNumGeoAreaSupportedPresent = 1;
  p_service_capabilities->eventTriggerCapabilities.maxNumGeoAreaSupported = C_MAX_NUM_GEOAREA_PER_SUPL_INIT;  /* spec: 32; impl: 1 */

  p_service_capabilities->eventTriggerCapabilities.geoAreaShapesSupported.ellipticalArea = 0;
  p_service_capabilities->eventTriggerCapabilities.geoAreaShapesSupported.polygonArea = 0;

  p_service_capabilities->sessionCapabilities.maxNumberTotalSessions = C_MAX_GEOFENCE_NUM;
  p_service_capabilities->sessionCapabilities.maxNumberTriggeredSessions = C_MAX_GEOFENCE_NUM;
  p_service_capabilities->sessionCapabilities.maxNumberPeriodicSessions = 1; /* not used but need a INTEGER (1..32) */




  /* populate the relevant part of SUPL_START with the cached cell ID info. */
  if (!supl_cell_information_construct(&p_SuplTriggerStart->locationId.cellInfo,
                                       &z_cell_info_allocated))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "No Cell ID info.", 0, 0, 0);
    return FALSE;
  }

  /* MLID for SUPL_TRIGGER_START */
  if (u_supl_global_num_mlid > 1) /* note being 1 means only the current serving cell is in the structure */
  {
    p_LocationIdData = (supl_LocationIdData *)os_MemAlloc((sizeof(supl_LocationIdData) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);
    p_LocationId = (supl_LocationId *)os_MemAlloc((sizeof(supl_LocationId) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);
    p_cell_info_allocated_mlid = (supl_cell_info_struct_type *)os_MemAlloc((sizeof(supl_cell_info_struct_type) * (u_supl_global_num_mlid - 1)), OS_MEM_SCOPE_TASK);

    if ((p_LocationIdData == NULL) || (p_LocationId == NULL) || (p_cell_info_allocated_mlid == NULL))
    {
      u_num_mlid = 0;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "mlid mem. allocation failed, set num_mlid to %u ", u_num_mlid);
    }
    else
    {
      memset(p_LocationIdData, 0, sizeof(supl_LocationIdData) * (u_supl_global_num_mlid - 1));
      memset(p_LocationId, 0, sizeof(supl_LocationId) * (u_supl_global_num_mlid - 1));
      memset(p_cell_info_allocated_mlid, 0, sizeof(supl_cell_info_struct_type) * (u_supl_global_num_mlid - 1));

      p_LocationIdData->locationId = p_LocationId;
      u_num_mlid = supl_chk_and_build_all_mlid(p_LocationIdData, p_LocationId,
                                               p_cell_info_allocated_mlid, (u_supl_global_num_mlid - 1));
    }
  }

  if (u_num_mlid > 0) /* valid mlid exists */
  {
    if (p_LocationIdData != NULL)
    {
      p_SuplTriggerStart->m.multipleLocationIdsPresent = TRUE;
      p_SuplTriggerStart->multipleLocationIds.n = u_num_mlid;
      p_SuplTriggerStart->multipleLocationIds.elem = p_LocationIdData;
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "p_LocationIdData not allocated: %lx ", p_LocationIdData);
    }
  }


  /* set this to CURRENT if cell id is of the current cell; otherwise set
     it to STATE to prevent the network from using this cell id to generate
     cell-id based positions for NI */
  p_SuplTriggerStart->locationId.status =
     (q_handovers_since_last_cell_id == 0) ?
     supl_Status_current : supl_Status_stale;

  if (z_supl_session_status.u_hash_valid)
  {
    p_SuplTriggerStart->m.verPresent = 1;
    p_SuplTriggerStart->ver.numbits = 64;
    memscpy(&p_SuplTriggerStart->ver.data[0], sizeof(p_SuplTriggerStart->ver.data),
            &z_supl_session_status.supl_init_hash[0], sizeof(z_supl_session_status.supl_init_hash));
    z_supl_session_status.u_hash_valid = FALSE; /* later SUPL_POS_INIT should not include HASH */
  }


  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, PROTO_SUPL);

  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }

    if (p_cell_info_allocated_mlid != NULL)
    {
      os_MemFree((void **)&p_cell_info_allocated_mlid);
      p_cell_info_allocated_mlid = NULL;
    }
    if (p_LocationId != NULL)
    {
      os_MemFree((void **)&p_LocationId);
      p_LocationId = NULL;
    }
    if (p_LocationIdData != NULL)
    {
      p_LocationIdData->locationId = NULL;
      os_MemFree((void **)&p_LocationIdData);
      p_LocationIdData = NULL;
    }

    return FALSE;
  }

  else
  {
    pd_comms_return_type ret_val;

    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (pdu_buff.length >> 8) & 0x00FF;
    u_length_lsb = pdu_buff.length & 0x00FF;

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, pdu_buff.length);

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplStart attempt aborted", 0, 0, 0);
    }
    else
    {
      supl_set_cf_state_area_event(C_SUPL_CF_STATE_UT1_SUPLTRIGGERRESPONSE, u_session_num);
      u_ret = TRUE;
    }

    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
    pdu_buff.value = NULL;

  }

  if (u_ret)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SuplTriggerStart sent ", 0, 0, 0);
  }

  if (p_cell_info_allocated_mlid != NULL)
  {
    os_MemFree((void **)&p_cell_info_allocated_mlid);
    p_cell_info_allocated_mlid = NULL;
  }
  if (p_LocationId != NULL)
  {
    os_MemFree((void **)&p_LocationId);
    p_LocationId = NULL;
  }
  if (p_LocationIdData != NULL)
  {
    p_LocationIdData->locationId = NULL;
    os_MemFree((void **)&p_LocationIdData);
    p_LocationIdData = NULL;
  }

  return u_ret;


}


/*
******************************************************************************
* supl_SuplTriggerResponse_proc
*
* Function description:
*
*   Process incoming SUPL TRIGGER RESPONSE
*
* Parameters: p_pdu_decoded: pointer to a structure holding decoded Supl PDU.
*
* Return value: TRUE/FALSE
*
******************************************************************************
*/

static void supl_SuplTriggerResponse_proc(const supl_ULP_PDU *p_pdu_decoded)
{

  const supl_Ver2_SUPLTRIGGEREDRESPONSE *p_suplTrigResponse;
  uint8      u_session_num = 0xFF;
  boolean    u_supl_trig_response_accept = FALSE;
  supl_AreaEventParams         *p_area_event_params;
  supl_CircularArea            *p_circular_area;
  supl_AreaIdList              *p_ArealIDList;
  supl_AreaId                  *p_AreaID_per_list;
  uint32                       transaction_id;
  sm_gm_add_request_type       gm_request;
  uint32                       q_temp;
  sint31                       l_temp;
  double                       d_temp;
  uint8                        error_code = 0;
  uint8                        u_i;
  uint16                       w_j;
  uint16                       w_num_area_id;

  supl_gm_blob                 z_gm_blob;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_TRIG_RESPONSE rcvd", 0, 0, 0);


  p_suplTrigResponse = p_pdu_decoded->message.u.msSUPLTRIGGEREDRESPONSE;
  p_area_event_params = p_suplTrigResponse->triggerParams.u.areaEventParams;

  /* identify the active session slot */
  u_session_num = supl_area_event_session_match(&p_pdu_decoded->sessionID);

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
  {
    /* no matching areaEvent session found: it should return 0xFF */
    /* TBD: notify SLP */
    return;
  }

  tm_umts_up_supl_terminate_timer_cancel_cf_state_area_event
     (C_SUPL_CF_STATE_UT1_SUPLTRIGGERRESPONSE, u_session_num);


  transaction_id = supl_comm_getnext_gm_transaction_id();
  memset((void *)&gm_request, 0, sizeof(sm_gm_add_request_type));

  if (p_suplTrigResponse->m.triggerParamsPresent)
  {
    if (p_suplTrigResponse->triggerParams.t ==
           T_supl_TriggerParams_areaEventParams)
    {
      if (p_area_event_params->areaEventType <= C_AREA_EVENT_TYPE_LEAVING)
      {

        u_supl_trig_response_accept = TRUE; /* all must-have are here and valid */


        gm_request.client_handle = supl_gm_handle;
        gm_request.q_transaction_id = transaction_id;

        if (p_area_event_params->areaEventType == supl_AreaEventType_enteringArea)
        {
          gm_request.geofence.event_type = GEOFENCE_EVENT_ENTERING_AREA;
        }
        else if (p_area_event_params->areaEventType == supl_AreaEventType_insideArea)
        {
          gm_request.geofence.event_type = GEOFENCE_EVENT_INSIDE_AREA;
        }
        else if (p_area_event_params->areaEventType == supl_AreaEventType_outsideArea)
        {
          gm_request.geofence.event_type = GEOFENCE_EVENT_OUTSIDE_AREA;
        }
        else if (p_area_event_params->areaEventType == supl_AreaEventType_leavingArea)
        {
          gm_request.geofence.event_type = GEOFENCE_EVENT_LEAVING_AREA;
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "AreaEventType %d not recongnized", p_area_event_params->areaEventType, 0, 0);
        }
      }

      gm_request.geofence.location_estimate =
         (p_area_event_params->locationEstimate) ? TRUE : FALSE;


      if (p_area_event_params->m.repeatedReportingParamsPresent)
      {

        gm_request.geofence.min_interval_time =
           (p_area_event_params->repeatedReportingParams.minimumIntervalTime >=
               C_AREA_EVENT_MIN_REPORT_INTERVAL_FLOOR) ?
           p_area_event_params->repeatedReportingParams.minimumIntervalTime :
           C_AREA_EVENT_MIN_REPORT_INTERVAL_FLOOR;

        z_supl_area_event_session_status[u_session_num].q_min_interval_time =
           gm_request.geofence.min_interval_time;

        gm_request.geofence.max_num_reports =
           p_area_event_params->repeatedReportingParams.maximumNumberOfReports;
      }
      else
      {

        gm_request.geofence.max_num_reports = 1;

        /* since it is defined not to include value 0, use 0 to indicate its
           absence */
        z_supl_area_event_session_status[u_session_num].q_min_interval_time = 0;
      }

      gm_request.geofence.start_time =
         (p_area_event_params->m.startTimePresent) ?
         p_area_event_params->startTime : 0;

      gm_request.geofence.stop_time =
         (p_area_event_params->m.stopTimePresent) ?
         p_area_event_params->stopTime :
         C_AREA_EVENT_DEFAULT_STOP_TIME;

      if (gm_request.geofence.stop_time > gm_request.geofence.start_time)
      {
        gm_request.geofence.stop_time =  gm_request.geofence.stop_time - gm_request.geofence.start_time;
      }
      else
      {
        gm_request.geofence.stop_time = 0;
      }

      if (p_area_event_params->m.geographicTargetAreaListPresent)
      {

        /* regardless of how many geographic areas the SLP gives, this
           implementation supports only one, so will only read the first one,
           and hence the follow param. is ignored.
           p_suplTrigResponse->triggerParams.u.areaEventParams->geographicTargetAreaList.n
         */

        if (p_area_event_params->geographicTargetAreaList.elem->t !=
               T_supl_GeographicTargetArea_circularArea
            )
        {
          u_supl_trig_response_accept = FALSE;
        }
        else /* process circular: lat/long def. same as in PosEstimate -- GAD */
        {
          p_circular_area =
             p_area_event_params->geographicTargetAreaList.elem->u.circularArea;

          gm_request.geofence.target_area_provided = TRUE;
          gm_request.geofence.target_area.target_area_type =
             GEOFENCE_TARGET_AREA_CIRCULAR_TYPE;

          /* lat/long for geofence input: 2's complement */

          q_temp = (p_circular_area->coordinate.latitudeSign ==
                       supl_Coordinate_latitudeSign_north) ?
             p_circular_area->coordinate.latitude :
             (p_circular_area->coordinate.latitude |
                 0x80000000
              );

          if (q_temp & 0x80000000)
          {
            d_temp = (double)((int32)(q_temp & 0x7FFFFFFF) * -1);
          }
          else
          {
            d_temp = (double)q_temp;
          }

          /* restore degrees from GAD: * 90/2^23, and GM API unit for latitude
             being 180/2^25 degrees, so * 90/2^23 * 2^25 / 180 = * 2 */
          d_temp *=  2.0;

          gm_request.geofence.target_area.u.circ_target_area.latitude =
             tm_util_nearest_long(d_temp);

          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "to GM - LaT GAD/circ:   %ld, %ld",
                p_circular_area->coordinate.latitude,
                gm_request.geofence.target_area.u.circ_target_area.latitude,
                0);

          l_temp = tm_umts_common_utils_S32Convert(p_circular_area->coordinate.longitude, 24);

          /*  restore degrees from GAD shape: * 180/2^23, and GM API unit for
              longitude being 180/2^25 degrees, so * 180/2^23 * 2^25 / 180 = 4 */

          d_temp = (double)l_temp * 4.0;

          gm_request.geofence.target_area.u.circ_target_area.longitude =
             tm_util_nearest_long(d_temp);

          gm_request.geofence.target_area.u.circ_target_area.radius =
             p_circular_area->radius;

          gm_request.geofence.num_of_area_ids = 0; /* this may change */

        } /* else: it is circular */
      } /* if - geographic target list */


      /*****  areaID List ****************************************************/

      if (p_area_event_params->m.areaIdListsPresent)
      {
        uint8 u_num_areaIDLists = 0;
        uint16 w_num_areaID_per_list = 0;

        sm_gm_area_id *p_gm_area_id_list = NULL;
        /* points to the base of the entire list allocated for GM */

        sm_gm_area_id *p_gm_area_id = NULL;
        /* points to each GM area ID */

        u_num_areaIDLists =
           (p_area_event_params->areaIdLists.n <= C_MAX_NUM_AREA_ID_LIST) ?
           p_area_event_params->areaIdLists.n : C_MAX_NUM_AREA_ID_LIST;



        p_ArealIDList = p_area_event_params->areaIdLists.elem; /* point to 1st list */

        /**** determining total number of area IDs **************/
        w_num_area_id = 0;
        for (u_i = 0; u_i < u_num_areaIDLists; u_i++)
        {
          w_num_areaID_per_list =
             (p_ArealIDList->areaIdSet.n <= C_MAX_NUM_AREA_ID_PER_LIST) ?
             p_ArealIDList->areaIdSet.n : C_MAX_NUM_AREA_ID_PER_LIST;

          p_AreaID_per_list = p_ArealIDList->areaIdSet.elem;

          for (w_j = 0; w_j < w_num_areaID_per_list; w_j++)
          {

            if (w_j < (w_num_areaID_per_list  - 1))
            {
              p_AreaID_per_list++;
            }
            w_num_area_id++;

          } /* areaID within one areaIDList for-loop */

          if (u_i <  (u_num_areaIDLists  - 1))
          {
            p_ArealIDList++; /* point to the next list */

          }
        } /* areaIdList for-loop */


        /**** allocating memory of total area IDs for GM *********/
        p_gm_area_id_list = (sm_gm_area_id *)os_MemAlloc((sizeof(sm_gm_area_id) * w_num_area_id), OS_MEM_SCOPE_TASK);


        if (p_gm_area_id_list == NULL)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "failed to allocate mem. for GM areaID_list", 0, 0, 0);
          /* TBD: inform SLP */
          return;
        }

        memset(p_gm_area_id_list, 0, sizeof(sm_gm_area_id) * w_num_area_id);

        gm_request.geofence.area_id_list = p_gm_area_id_list;
        /*  base of the list */

        gm_request.geofence.num_of_area_ids = w_num_area_id;
        /* total number of area IDs */

        p_gm_area_id = p_gm_area_id_list;
        /* individual GM area ID starts at 1st */

        p_ArealIDList = p_area_event_params->areaIdLists.elem; /* re-point to 1st list */

        for (u_i = 0; u_i < u_num_areaIDLists; u_i++)
        {

          w_num_areaID_per_list =
             (p_ArealIDList->areaIdSet.n <= C_MAX_NUM_AREA_ID_PER_LIST) ?
             p_ArealIDList->areaIdSet.n : C_MAX_NUM_AREA_ID_PER_LIST;

          p_AreaID_per_list = p_ArealIDList->areaIdSet.elem;

          MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                "AreaList #%u containing %u areaIDs",
                u_i, w_num_areaID_per_list);

          for (w_j = 0; w_j < w_num_areaID_per_list; w_j++)
          {

            /* if there is no geographic target area, all areaIDs should
               be considered as within */
            if (p_area_event_params->m.geographicTargetAreaListPresent)
            {
              p_gm_area_id->area_id_type =
                 (p_ArealIDList->areaIdSetType == supl_AreaIdSetType_border) ?
                 GEOFENCE_AREA_ID_BORDER_TYPE : GEOFENCE_AREA_ID_WITHIN_TYPE;
            }
            else
            {
              p_gm_area_id->area_id_type = GEOFENCE_AREA_ID_WITHIN_TYPE;
            }

            switch (p_AreaID_per_list->t)
            {

              case T_supl_AreaId_gSMAreaId:

                p_gm_area_id->area_id_rat_type = GEOFENCE_AREA_ID_RAT_GSM_TYPE;

                if (p_AreaID_per_list->u.gSMAreaId->m.refMCCPresent)
                {
                  p_gm_area_id->u.gSMAreaId.mcc =
                     p_AreaID_per_list->u.gSMAreaId->refMCC;
                }
                else
                {
                  p_gm_area_id->u.gSMAreaId.mcc =
                     GEOFENCE_NO_MCC; /* indicate the absence of mcc */
                }

                if (p_AreaID_per_list->u.gSMAreaId->m.refMNCPresent)
                {
                  p_gm_area_id->u.gSMAreaId.mnc =
                     p_AreaID_per_list->u.gSMAreaId->refMNC;
                }
                else
                {
                  p_gm_area_id->u.gSMAreaId.mnc =
                     GEOFENCE_NO_MNC; /* to indicate the absence of mnc */
                }

                if (p_AreaID_per_list->u.gSMAreaId->m.refLACPresent)
                {
                  p_gm_area_id->u.gSMAreaId.lac =
                     p_AreaID_per_list->u.gSMAreaId->refLAC;
                }
                else
                {
                  p_gm_area_id->u.gSMAreaId.lac =
                     GEOFENCE_NO_LAC; /* use 0xFFFF to indicate the absence of lab: note TBD covering 65535 */
                }

                if (p_AreaID_per_list->u.gSMAreaId->m.refCIPresent)
                {
                  p_gm_area_id->u.gSMAreaId.cell_identity =
                     p_AreaID_per_list->u.gSMAreaId->refCI;
                }
                else
                {
                  p_gm_area_id->u.gSMAreaId.cell_identity =
                     GEOFENCE_NO_GSM_CID; /* to indicate the absence of cell_identity */
                }

                break;

              case  T_supl_AreaId_wCDMAAreaId:

                p_gm_area_id->area_id_rat_type =
                   GEOFENCE_AREA_ID_RAT_WCDMA_TYPE;

                if (p_AreaID_per_list->u.wCDMAAreaId->m.refMCCPresent)
                {
                  p_gm_area_id->u.wCDMAAreaId.mcc =
                     p_AreaID_per_list->u.wCDMAAreaId->refMCC;
                }
                else
                {
                  p_gm_area_id->u.wCDMAAreaId.mcc =
                     GEOFENCE_NO_MCC; /* to indicate the absence of mcc */
                }

                if (p_AreaID_per_list->u.wCDMAAreaId->m.refMNCPresent)
                {
                  p_gm_area_id->u.wCDMAAreaId.mnc =
                     p_AreaID_per_list->u.wCDMAAreaId->refMNC;
                }
                else
                {
                  p_gm_area_id->u.wCDMAAreaId.mnc =
                     GEOFENCE_NO_MNC; /* to indicate the absence of mnc */
                }

                if (p_AreaID_per_list->u.wCDMAAreaId->m.refLACPresent)
                {
                  p_gm_area_id->u.wCDMAAreaId.lac =
                     p_AreaID_per_list->u.wCDMAAreaId->refLAC;
                }
                else
                {
                  p_gm_area_id->u.wCDMAAreaId.lac =
                     GEOFENCE_NO_LAC; /* to indicate the absence of lab */
                }

                if (p_AreaID_per_list->u.wCDMAAreaId->m.refUCPresent)
                {
                  p_gm_area_id->u.wCDMAAreaId.cell_identity =
                     p_AreaID_per_list->u.wCDMAAreaId->refUC;
                }
                else
                {
                  p_gm_area_id->u.wCDMAAreaId.cell_identity =
                     GEOFENCE_NO_WCDMA_CID; /* to indicate the absence of cell_identity */
                }

                break;

              case T_supl_AreaId_lTEAreaId:

                p_gm_area_id->area_id_rat_type =
                   GEOFENCE_AREA_ID_RAT_LTE_TYPE;

                if (p_AreaID_per_list->u.lTEAreaId->m.refMCCPresent)
                {
                  p_gm_area_id->u.lTEAreaId.mcc =
                     p_AreaID_per_list->u.lTEAreaId->refMCC;
                }
                else
                {
                  p_gm_area_id->u.lTEAreaId.mcc =
                     GEOFENCE_NO_MCC; /* to indicate the absence of mcc */
                }

                if (p_AreaID_per_list->u.lTEAreaId->m.refMNCPresent)
                {
                  p_gm_area_id->u.lTEAreaId.mnc =
                     p_AreaID_per_list->u.lTEAreaId->refMNC;
                }
                else
                {
                  p_gm_area_id->u.lTEAreaId.mnc =
                     GEOFENCE_NO_MNC; /* to indicate the absence of mnc */
                }

                if (p_AreaID_per_list->u.lTEAreaId->m.refCIPresent)
                {
                  /* p_AreaID_per_list->u.lTEAreaId->refCI is of bitstring size 29: */

                  /* p_AreaID_per_list->u.lTEAreaId->refCI.data[0] holds the 8 msb
                     of the 29-bit refCI;
                   */
                  q_temp = (uint32)p_AreaID_per_list->u.lTEAreaId->refCI.data[0];
                  q_temp <<= 21;
                  q_temp &= 0x1FE00000;
                  p_gm_area_id->u.lTEAreaId.cell_identity = q_temp;

                  q_temp = (uint32)p_AreaID_per_list->u.lTEAreaId->refCI.data[1];
                  q_temp <<= 13;
                  q_temp &= 0x001FE000;
                  p_gm_area_id->u.lTEAreaId.cell_identity |= q_temp;

                  q_temp = (uint32)p_AreaID_per_list->u.lTEAreaId->refCI.data[2];
                  q_temp <<= 5;
                  q_temp &= 0x00001FE0;
                  p_gm_area_id->u.lTEAreaId.cell_identity |= q_temp;

                  /* p_AreaID_per_list->u.lTEAreaId->refCI.data[3] holds the 5 lsb
                     of the 29-bit refCI on its left 5 bits;
                   */
                  q_temp = (uint32)p_AreaID_per_list->u.lTEAreaId->refCI.data[3];
                  q_temp >>= 3;
                  q_temp &= 0x0000001F;
                  p_gm_area_id->u.lTEAreaId.cell_identity |= q_temp;

                }
                else
                {
                  p_gm_area_id->u.lTEAreaId.cell_identity =
                     GEOFENCE_NO_LTE_CID; /* use max. allowed plus 1 to indicate the absence of lab */
                }

                break;

              default:
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Unsupported RAT type for geofence: %u",
                      p_AreaID_per_list->t, 0, 0);


            } /* end of areaID RAT type switch */

            if (w_j < (w_num_areaID_per_list  - 1))
            {
              p_AreaID_per_list++;
              p_gm_area_id++;
            }

          } /* areaID within one areaIDList for-loop */

          if (u_i <  (u_num_areaIDLists  - 1))
          {
            p_ArealIDList++;
            p_gm_area_id++;
          }

        } /* areaIdList for-loop */


      } /* if areaIDList present */

      /****  Add this geofence ********/
      memscpy(&z_gm_blob.z_session_id, sizeof(z_gm_blob.z_session_id),
              &z_supl_area_event_session_status[u_session_num].z_session_id,
              sizeof(z_supl_area_event_session_status[u_session_num].z_session_id));

      z_gm_blob.q_supl_version =
         z_supl_area_event_session_status[u_session_num].q_supl_version;

      z_gm_blob.w_SetSessionID =
         z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id.w_Session_Id;

      /**** allocating memory of client blob for GM *********/
      gm_request.geofence.client_blob = (byte *)os_MemAlloc(sizeof(supl_gm_blob), OS_MEM_SCOPE_TASK);


      if (gm_request.geofence.client_blob == NULL)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "failed to allocate mem. for GM client blob", 0, 0, 0);
        if ((gm_request.geofence.area_id_list != NULL) &&
            (gm_request.geofence.num_of_area_ids > 0)
            )
        {
          os_MemFree((void **)&(gm_request.geofence.area_id_list));
        }
        /* TBD: inform SLP */
        return;
      }
      memset((void *)gm_request.geofence.client_blob, 0, sizeof(supl_gm_blob));
      gm_request.geofence.client_blob_size = sizeof(supl_gm_blob);

      memscpy(&gm_request.geofence.client_blob[0], sizeof(gm_request.geofence.client_blob),
              &z_gm_blob, sizeof(z_gm_blob));

      gm_request.geofence.persistency_needed = FALSE;
      gm_request.geofence.responsiveness = GEOFENCE_RESPONSIVENESS_HIGH;

      gm_request.geofence.hysteresis_required = TRUE;

      if (gm_add_geofence(&gm_request, &error_code) == FAILED)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Not able to add Geofence, error_code %d", error_code, 0, 0);
        /* TBD: response to SLP needed */
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Succesfully sent request to add Geofence", 0, 0, 0);
        z_supl_area_event_session_status[u_session_num].u_gm_trasaction_id_valid = TRUE;
        z_supl_area_event_session_status[u_session_num].q_gm_transaction_id =
           gm_request.q_transaction_id;
      }



      /* determine if the tcp link is to be kept or disconnected, based on start time */
      if (gm_request.geofence.start_time >=
             C_SUPL_DEFAULT_TCP_ON_ONLY_TIME_SEC)
      {
        /* disconnect tcp for now */
        tm_umts_up_supl_comm_close_connection();
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TCP disconnect: areaEvent start time: %u ",
              p_area_event_params->startTime, 0, 0);

      }
      else
      {
        /* If no single-shot session going on, start a timer for 130 sec.,
           if GM does not request anything by that time, disconnect tcp;
           the time is canceled when needing to send the message to SLP  */
        if (!z_supl_session_status.u_network_session_on)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TCP stayOnOnly timer start: areaEvent start time: %u ",
                p_area_event_params->startTime, 0, 0);
          (void)os_TimerStart(supl_tcp_stay_on_only_timer,
                              (C_SUPL_DEFAULT_TCP_ON_ONLY_TIME_SEC * 1000),
                              NULL);
        }
      }


    } /* if - areaEventTrigger */
    else
    {
      /* periodic trigger not supported */
      (void)tm_umts_up_supl_supl_end_tx(
         &z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id,
         NULL,
         supl_ver2_serviceNotSupported,
         NULL,
         z_supl_area_event_session_status[u_session_num].q_supl_version);

      memset(&z_supl_area_event_session_status[u_session_num],
             0,
             sizeof(supl_area_event_status_s_type));

      tm_umts_up_supl_comm_close_connection();


    }

  } /* if - triggerParam present */

  else  /* triggerParam absent: for NI such IE is mandatory */
  {
    (void)tm_umts_up_supl_supl_end_tx(
       &z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id,
       NULL,
       supl_dataMissing,
       NULL,
       z_supl_area_event_session_status[u_session_num].q_supl_version);

    memset(&z_supl_area_event_session_status[u_session_num],
           0,
           sizeof(supl_area_event_status_s_type));

    tm_umts_up_supl_comm_close_connection();

  }

  if (u_supl_trig_response_accept == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_TRIGGER_RESPONSE not accepted", 0, 0, 0);
  }

  return;
}

/*
******************************************************************************
* supl_SuplReport_proc
*
* Function description:
*
*   Process incoming SUPL TRIGGER RESPONSE
*
* Parameters: p_pdu_decoded: pointer to a structure holding decoded Supl PDU.
*
* Return value: TRUE/FALSE
*
******************************************************************************
*/

static void supl_SuplReport_proc(const supl_ULP_PDU *p_pdu_decoded)
{
  /* when receiving SUPL_REPORT from SLP, the assistance data related
     exchange with the SLP is complete within this current areaEvent
     trigger session */

  /* there is no way of knowing when a breach will happen, terminate
     TCP link now   */
  if (z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT);
    tm_umts_up_supl_callflow_kind_set(C_SUPL_SESSION_NONE);
    tm_umts_up_supl_comm_close_connection();
    z_supl_session_status.u_current_is_for_area_event = FALSE;
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Recvd SUPL_REPORT, disc. TCP", 0, 0, 0);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Recvd unexpected SUPL_REPORT", 0, 0, 0);
  }
}

/*
******************************************************************************
* supl_SuplTrigStop_proc
*
* Function description:
*
*   Process incoming SUPL TRIGGER STOP
*
* Parameters: p_pdu_decoded: pointer to a structure holding decoded Supl PDU.
*
* Return value: none.
*
******************************************************************************
*/

static void supl_SuplTrigStop_proc(const supl_ULP_PDU *p_pdu_decoded)
{
  sm_gm_delete_request_type z_sm_gm_delete_request;
  supl_set_session_id z_provided_set_session_id;
  supl_slp_session_id z_provided_slp_session_id;
  uint8 u_error_code = 0;
  supl_StatusCode e_supl_status_code = 0xFFFFFFFF;
  uint8 u_session_num;

  memset(&z_provided_set_session_id, 0, sizeof(supl_set_session_id));
  memset(&z_provided_slp_session_id, 0, sizeof(supl_slp_session_id));

  z_sm_gm_delete_request.client_handle = supl_gm_handle;

  u_session_num = supl_area_event_session_match(&p_pdu_decoded->sessionID);

  if (u_session_num != 0xFF) /* found a match */
  {
    z_sm_gm_delete_request.q_transaction_id =
       z_supl_area_event_session_status[u_session_num].q_gm_transaction_id;

    z_sm_gm_delete_request.geofence_id =
       z_supl_area_event_session_status[u_session_num].q_geofence_id;


    if (gm_delete_geofence(&z_sm_gm_delete_request, &u_error_code) ==
           FAILED)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failure of geofence delete: ID %lu, error=%u",
            z_sm_gm_delete_request.geofence_id, u_error_code, 0);
      e_supl_status_code = supl_StatusCode_unspecified;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Goefence %u deleted", z_sm_gm_delete_request.geofence_id, 0, 0);
      z_supl_area_event_session_status[u_session_num].u_session_active = FALSE;
    }
  }

  /* Send SUPL_END using the trigged Session ID */

  supl_set_session_id_organizer(&p_pdu_decoded->sessionID, &z_provided_set_session_id);
  supl_util_server_session_id_organizer(&p_pdu_decoded->sessionID, &z_provided_slp_session_id);


  (void)tm_umts_up_supl_supl_end_prepare_or_tx(&z_provided_slp_session_id,
                                               &z_provided_set_session_id,
                                               e_supl_status_code,
                                               NULL,
                                               z_supl_session_status.q_supl_version
                                               );

}

/*
******************************************************************************
* supl_area_event_session_match
*
* Function description:
*
*   Identify the matching area event session when a new area event trigger
*   type of session message arrives.
*
* Parameters: p_source_session_id - pointer to a structure holding decoded
*   sessionID info. in the message from the SLP.
*
* Return value: matching session number; 0xFF - no match
*
******************************************************************************
*/

static uint8 supl_area_event_session_match(const supl_SessionID *p_source_session_id)
{

  supl_slp_session_id  z_incoming_slp_session_id;

  supl_area_event_status_s_type *p_supl_event_status;


  uint8 u_i;
  uint8 u_ret_val = 0xFF;

  memset(&z_incoming_slp_session_id, 0, sizeof(supl_slp_session_id));

  /* organized SLP session ID and place it in z_incoming_slp_session_id */
  if (p_source_session_id->m.slpSessionIDPresent)
  {
    supl_util_server_session_id_organizer(p_source_session_id, &z_incoming_slp_session_id);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SLP session ID missing, skip checking areaEvent match", 0, 0, 0);
    return u_ret_val;
  }

  p_supl_event_status = &z_supl_area_event_session_status[0];


  for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
  {
    if (p_supl_event_status->u_session_active)
    {
      /* check SLP session ID */

      if (supl_SLP_session_id_match_check(&p_supl_event_status->z_session_id.z_Slp_Sess_Id,
                                          &z_incoming_slp_session_id
                                          ) == TRUE
          )
      {

        u_ret_val = u_i; /* matching session found */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Active AreaEvent session #%u matches incoming message",
              u_ret_val, 0, 0);
        break;
      }

    } /* if an active areaEvent session found */

    if (u_i < (C_MAX_GEOFENCE_NUM - 1))
    {
      p_supl_event_status++;
    }

  } /* active areaEvent trigger session check for-loop */

  return u_ret_val;

}


/*
******************************************************************************
* tm_umts_up_supl_geofence_areaEventSession_match
*
* Function description:
*
*   Identify the matching area event session when GM asks SUPL for actions
*
* Parameters:
*
*  q_geofence_id: Geofence ID -- if 0xFFFFFFFF, no such info
*     provided in which case the function will find the first
*     available areaEvent session.
*
* Return value: identified active SUPL areaEvent session.
*               0xFF: no such session identified.
*
*
******************************************************************************
*/

uint8 tm_umts_up_supl_geofence_areaEventSession_match(uint32 q_geofence_id)
{
  uint8 u_i, u_session_num;

  u_session_num = 0xFF;


  for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
  {
    if (q_geofence_id != 0xFFFFFFFF)
    {
      if (z_supl_area_event_session_status[u_i].u_geofence_id_valid &&
          (z_supl_area_event_session_status[u_i].q_geofence_id ==
              q_geofence_id)
          )
      {
        u_session_num = u_i;
        break;
      }
    }
    else
    {
      if (z_supl_area_event_session_status[u_i].u_geofence_id_valid)
      {
        u_session_num = u_i;
        break;
      }
    }
  }

  return u_session_num;

}


/*
******************************************************************************
* tm_umts_up_supl_area_event_to_active
*
* Function description:
*
*   Retrieve areaEvent session status info. for current active session use.
*
* Parameters: index to the areaEvent session to be used.
*
* Return value: None.
*
******************************************************************************
*/

void tm_umts_up_supl_area_event_to_active(uint8 u_session_num)
{

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
    return;

  z_supl_session_status.z_session_id.z_Set_Sess_Id.w_Session_Id =
     z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id.w_Session_Id;

  memscpy(&z_supl_session_status.z_session_id.z_Slp_Sess_Id,
          sizeof(z_supl_session_status.z_session_id.z_Slp_Sess_Id),
          &z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id,
          sizeof(z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id));

  z_supl_session_status.u_position_protocol = TM_POS_PRTL_RRLP;

  z_supl_session_status.u_current_is_for_area_event = TRUE;

}

/*
******************************************************************************
* tm_umts_up_supl_gf_breach_proc
*
* Function description:
*
*   Processes geofence breach info.
*
* Parameters: p_gm_client_notify - pointer to data of from the GM module.
*
* Return value: None.
*
******************************************************************************
*/

void tm_umts_up_supl_gf_breach_proc(sm_gm_client_breach_notify_type *p_gm_client_breach_notify)
{
  uint8 u_session_num = 0xFF;

  u_session_num = tm_umts_up_supl_geofence_areaEventSession_match
     (p_gm_client_breach_notify->geofence_id);

  if (u_session_num == 0xFF)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "no matching SUPL session to proc. the GF breach %d",
          p_gm_client_breach_notify->geofence_id, 0, 0);
  }
  else
  {

    if (tm_umts_up_supl_supl_report_prepare_or_tx(u_session_num, p_gm_client_breach_notify, NULL, NULL))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_REPORT sent: %u", u_session_num, 0, 0);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_REPORT pending: %u", u_session_num, 0, 0);
    }
  }

}

/*
******************************************************************************
* tm_umts_up_supl_gf_delete_proc
*
* Function description:
*
*   Processes geofence delete info..
*
* Parameters: p_gm_client_notify - pointer to data of from the GM module.
*
* Return value: None.
*
******************************************************************************
*/

void tm_umts_up_supl_gf_delete_proc(sm_gm_client_notification_type *p_gm_client_notify)
{
  uint8 u_session_num = 0xFF;

  u_session_num = tm_umts_up_supl_geofence_areaEventSession_match
     (p_gm_client_notify->notification.geofence_id);

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "no matching SUPL session to proc. the GF breach %d",
          p_gm_client_notify->notification.geofence_id, 0, 0);
  }
  else
  {
    if (p_gm_client_notify->notification.return_code == GEOFENCE_DELETE_ON_MAX_REPORTS)
    {
      /* wait for SLP to send SUPL_END */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GF %u id=%u delete max. report reached - UT3 waiting starts",
            u_session_num, p_gm_client_notify->notification.geofence_id, 0);
    }
    else if (p_gm_client_notify->notification.return_code == GEOFENCE_DELETE_ON_STOP_TIME)
    {
      if (tm_umts_up_supl_supl_end_prepare_or_tx(
            &z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id,
            &z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id,
            supl_StatusCode_ver2_sessionStopped,
            NULL,
            z_supl_area_event_session_status[u_session_num].q_supl_version)
          )
      {

        tm_umts_up_supl_comm_close_connection();

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_END AreaEventStopTime sent: %u, %u:",
              u_session_num,
              p_gm_client_notify->notification.geofence_id,
              0);

      }

      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_END AreaEventStopTime pending: %u, %u:",
              u_session_num,
              p_gm_client_notify->notification.geofence_id,
              0);
      }

      /* clear the active session */
      if (z_supl_area_event_session_status[u_session_num].u_session_active)
      {
        memset(&z_supl_area_event_session_status[u_session_num], 0,
               sizeof(supl_area_event_status_s_type));
      }


    } /* if due to StopTime */
  }

}


/*
******************************************************************************
* tm_umts_up_supl_supl_report_prepare_or_tx
*
* Function description:
*
*   Processes prepare and/or send SUPL_REPORT.
*
* Parameters: p_gm_client_notify - pointer to data of from the GM module.
*
* Return value: TRUE - message sent in this function; FALSE - message not sent
*               yet.
*
******************************************************************************
*/


static boolean tm_umts_up_supl_supl_report_prepare_or_tx
(uint8                              u_session_num,
 sm_gm_client_breach_notify_type *p_gm_client_breach_notify,
 uint8 *p_hash,
 supl_slp_session_id *p_provided_slp_session_id
 )
{

  sm_gm_breach_ack_notify_type z_sm_gm_breach_ack_notify;
  uint8  u_sm_gm_error_code;

  pd_comms_return_type pdcomm_connect_ret_value = PDCOMM_RESULT_ERROR_UNKNOWN;
  boolean u_ret = FALSE;

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
    return FALSE;

  /* if no tcp link available yet, set up such link */
  if (z_pdcomm_tcp_info.curr_state != PDCOMM_STATE_CONNECTED)
  {

    pdcomm_connect_ret_value = tm_umts_up_supl_comm_tcp_connect(SUPL_VERSION_MAIN_2);

    if (PDCOMM_RESULT_OK == pdcomm_connect_ret_value) /* quite unlikely */
    {
      if (tm_umts_up_supl_supl_report_tx(u_session_num,
                                         p_gm_client_breach_notify,
                                         p_hash,
                                         p_provided_slp_session_id
                                         )
          )
      {
        u_ret = TRUE;
      }
    }

    else if (PDCOMM_RESULT_WAIT == pdcomm_connect_ret_value) /* most likely */
    {
      if (p_gm_client_breach_notify != NULL)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Cache gm_breach_notify for later SUPL_REPORT", 0, 0, 0);
        z_supl_area_event_session_status[u_session_num].z_pending_supl_report_info.u_valid = TRUE;
        z_supl_area_event_session_status[u_session_num].z_pending_supl_report_info.z_gm_client_breach_notify =
           *p_gm_client_breach_notify;

        supl_set_cf_state_area_event(C_SUPL_CF_STATE_UT0_SUPLREPORT_GF_BREACH, u_session_num);

      }
      else if (p_hash != NULL)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Waiting TCP connection to respond to SessionInfoQuery", 0, 0, 0);
        supl_set_cf_state(C_SUPL_CF_STATE_UT0_SUPLREPORT_QUERY, 0);
      }

    }

    else
    {
      /* not possible to connect, abort */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: TCP conn. failed; NACK GM", 0, 0, 0);

      if (p_gm_client_breach_notify != NULL)
      {
        /* for geofence breach, ack to GM now with NACK */

        z_sm_gm_breach_ack_notify.geofence_id =
           z_supl_area_event_session_status[u_session_num].q_geofence_id;

        /* SUPL_REPORT for breach report has failed to be sent from SM perspective */
        z_sm_gm_breach_ack_notify.accepted = FALSE;
        u_sm_gm_error_code = GEOFENCE_ERROR_UNSPECIFIED;

        gm_breach_ack(&z_sm_gm_breach_ack_notify, &u_sm_gm_error_code);
      }
    }

  } /* close if-tcp-link-not-yet-available */

  else /* already connected */
  {
    if (tm_umts_up_supl_supl_report_tx(u_session_num,
                                       p_gm_client_breach_notify,
                                       p_hash,
                                       p_provided_slp_session_id
                                       )
        )
    {
      u_ret = TRUE;
    }
  }


  return u_ret;

}


/*
******************************************************************************
* tm_umts_up_supl_supl_report_tx
*
* Function description:
*
*   send SUPL_REPORT.
*
* Parameters:
*
* Return value: None.
*
******************************************************************************
*/


static boolean tm_umts_up_supl_supl_report_tx
(uint8                               u_session_num,
 sm_gm_client_breach_notify_type *p_gm_client_breach_notify,
 uint8 *p_hash,
 supl_slp_session_id *p_provided_slp_session_id
 )
{

  tm_pdu_buf_type pdu_buff; /* including length and pointer to data */
  supl_ULP_PDU      pdu;

  supl_Ver2_SUPLREPORT *p_SuplReport;
  supl_Ver2_SUPLREPORT    z_SuplReport;
  supl_ReportData         z_supl_ReportData;
  supl_SessionInformation z_SessionInformation_array[C_MAX_GEOFENCE_NUM];
  supl_SessionInformation *p_SessionInformation_per_geofence;
  supl_IPAddress          z_supl_IPAddress_for_slp;


  uint32 q_temp;
  uint16 w_FixWn;
  uint32 q_fix_time_gps_msec, q_fix_time_utc_msec;


  int              encode_status;
  uint8 *p_length;
  uint8            u_length_msb = 0;
  uint8            u_length_lsb = 0;

  char          timestamp[14];
  utc_time      z_utc_time;
  uint64        t_TS;
  qword         qword_timestamp; /* in the format used by tm_lr_resp_type */
  uint32        q_Gps0P25Msec;

  supl_area_event_status_s_type *p_supl_area_event_status;
  uint8         u_i;

  boolean       u_ret  = FALSE;

  sm_gm_breach_ack_notify_type z_sm_gm_breach_ack_notify;
  uint8  u_sm_gm_error_code;


  /***************************************************************************/

  if (u_session_num >= C_MAX_GEOFENCE_NUM)
    return FALSE;


  memset(&pdu, 0x00, sizeof(supl_ULP_PDU));
  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));
  memset(&z_SuplReport, 0x00, sizeof(supl_Ver2_SUPLREPORT));
  memset(&z_supl_ReportData, 0x00, sizeof(supl_ReportData));
  memset(&z_SessionInformation_array[0], 0x00, (sizeof(supl_SessionInformation) * C_MAX_GEOFENCE_NUM));
  memset(&z_supl_IPAddress_for_slp, 0, sizeof(supl_IPAddress));

  p_SuplReport = &z_SuplReport;
  pdu.message.t = C_SUPL_ASN1_CHOICE_ULP_MESSAGE_SUPLREPORT;
  pdu.message.u.msSUPLREPORT = p_SuplReport;


  if (p_hash != NULL) /* if HASH provided, it must be for SUPL_INFO_QUERY */
  {

    /* w_SetSessionID has incremented after last SUPL_TRIGGER_START, so this is a new number */
    if (!supl_header_build(&pdu, &w_SetSessionID, NULL, p_provided_slp_session_id, z_supl_session_status.q_supl_version))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplReport attempt aborted: header", 0, 0, 0);
      return FALSE;
    }
    z_supl_session_status.z_session_id.z_Set_Sess_Id.w_Session_Id = w_SetSessionID;

    if (p_provided_slp_session_id != NULL)
    {
      memscpy(&z_supl_session_status.z_session_id.z_Slp_Sess_Id,
              sizeof(z_supl_session_status.z_session_id.z_Slp_Sess_Id),
              p_provided_slp_session_id,
              sizeof(*p_provided_slp_session_id));
    }

    /* need SessionList: TBD need to handle mulitple active sessions */
    p_supl_area_event_status = &z_supl_area_event_session_status[0];
    p_SessionInformation_per_geofence = &z_SessionInformation_array[0];

    for (u_i = 0; u_i < C_MAX_GEOFENCE_NUM; u_i++)
    {

      if (p_supl_area_event_status->u_session_active &&
          ((p_supl_area_event_status->z_session_id.z_Set_Sess_Id.z_Set_Id.set_id_choice ==
               T_supl_SETId_imsi
            ) ||
           (p_supl_area_event_status->z_session_id.z_Set_Sess_Id.z_Set_Id.set_id_choice ==
               T_supl_SETId_msisdn
            )
           )
          )
      {

        z_SuplReport.m.sessionListPresent = TRUE;

        /* populate sessionInformation of the active areaEventTrigger sessions */
        p_SessionInformation_per_geofence->sessionID.m.setSessionIDPresent = TRUE;

        p_SessionInformation_per_geofence->sessionID.setSessionID.sessionId =
           p_supl_area_event_status->z_session_id.z_Set_Sess_Id.w_Session_Id;

        p_SessionInformation_per_geofence->sessionID.setSessionID.setId.t =
           p_supl_area_event_status->z_session_id.z_Set_Sess_Id.z_Set_Id.set_id_choice;

        if (p_SessionInformation_per_geofence->sessionID.setSessionID.setId.t == T_supl_SETId_imsi)
        {
          p_SessionInformation_per_geofence->sessionID.setSessionID.setId.u.imsi =
             &p_supl_area_event_status->z_session_id.z_Set_Sess_Id.z_Set_Id.u.imsi;
        }
        else  /* ( z_SessionInformation.sessionID.setSessionID.setId.t == T_supl_SETId_msisdn ) */
        {
          p_SessionInformation_per_geofence->sessionID.setSessionID.setId.u.msisdn =
             &p_supl_area_event_status->z_session_id.z_Set_Sess_Id.z_Set_Id.u.msisdn;
        }

        p_SessionInformation_per_geofence->sessionID.m.slpSessionIDPresent = TRUE;

        p_SessionInformation_per_geofence->sessionID.slpSessionID.slpId.t =
           p_supl_area_event_status->z_session_id.z_Slp_Sess_Id.zSlpAddress.choice;

        p_SessionInformation_per_geofence->sessionID.slpSessionID.sessionID.numocts =
           C_SUPL_SLP_SESSION_ID_BYTE_LENGTH;

        memscpy(p_SessionInformation_per_geofence->sessionID.slpSessionID.sessionID.data,
                sizeof(p_SessionInformation_per_geofence->sessionID.slpSessionID.sessionID.data),
                p_supl_area_event_status->z_session_id.z_Slp_Sess_Id.session_Id,
                sizeof(p_supl_area_event_status->z_session_id.z_Slp_Sess_Id.session_Id));

        if (p_SessionInformation_per_geofence->sessionID.slpSessionID.slpId.t
            == T_supl_SLPAddress_iPAddress)
        {
          p_SessionInformation_per_geofence->sessionID.slpSessionID.slpId.u.iPAddress =
             &z_supl_IPAddress_for_slp;

          z_supl_IPAddress_for_slp.t =
             p_supl_area_event_status->z_session_id.z_Slp_Sess_Id.zSlpAddress.u.z_Ip_Address.choice;

          /* IPV4 */
          if (z_supl_IPAddress_for_slp.t == C_SUPL_ASN1_CHOICE_IPV4ADDRESS)
          {
            z_supl_IPAddress_for_slp.u.ipv4Address =
               &p_supl_area_event_status->z_session_id.z_Slp_Sess_Id.zSlpAddress.u.z_Ip_Address.u.Ip_V4;
          }
          /* IPV6 */
          else if (z_supl_IPAddress_for_slp.t == T_supl_IPAddress_ipv6Address)
          {
            z_supl_IPAddress_for_slp.u.ipv6Address =
               &p_supl_area_event_status->z_session_id.z_Slp_Sess_Id.zSlpAddress.u.z_Ip_Address.u.Ip_V6;
          }
        }
        else if (p_SessionInformation_per_geofence->sessionID.slpSessionID.slpId.t
                 == T_supl_SLPAddress_fQDN)
        {
          p_SessionInformation_per_geofence->sessionID.slpSessionID.slpId.u.fQDN =
             (supl_FQDN)p_supl_area_event_status->z_session_id.z_Slp_Sess_Id.zSlpAddress.u.z_fqdn.fqdn;
        }

      } /* one existing geofence found */

      if ((u_i + 1) < (C_MAX_GEOFENCE_NUM - 1))
      {
        p_supl_area_event_status++;
        p_SessionInformation_per_geofence++;
      }

    } /* max. num. of geofence for-loop */

    if (z_SuplReport.m.sessionListPresent == TRUE)
    {
      z_SuplReport.sessionList.n = 1; /* for current interoperative use, set to 1 */
      z_SuplReport.sessionList.elem = &z_SessionInformation_array[0];
    }

    z_SuplReport.m.verPresent = 1;
    z_SuplReport.ver.numbits = 64;
    memscpy(&z_SuplReport.ver.data[0], sizeof(z_SuplReport.ver.data),
            &z_supl_session_status.supl_init_hash[0],
            sizeof(z_supl_session_status.supl_init_hash));


  }

  else if  (p_gm_client_breach_notify != NULL)
  {

    if (!supl_header_build(&pdu,
                           &z_supl_area_event_session_status[u_session_num].z_session_id.z_Set_Sess_Id.w_Session_Id,
                           NULL,
                           &z_supl_area_event_session_status[u_session_num].z_session_id.z_Slp_Sess_Id,
                           z_supl_area_event_session_status[u_session_num].q_supl_version)
        )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplReport attempt aborted: header", 0, 0, 0);

      /* NACK GM */
      z_sm_gm_breach_ack_notify.geofence_id =
         z_supl_area_event_session_status[u_session_num].q_geofence_id;

      /* SUPL_REPORT for breach report has failed to be sent from SM perspective */
      z_sm_gm_breach_ack_notify.accepted = FALSE;
      u_sm_gm_error_code = GEOFENCE_ERROR_UNSPECIFIED;

      gm_breach_ack(&z_sm_gm_breach_ack_notify, &u_sm_gm_error_code);

      return FALSE;
    }


    if (p_gm_client_breach_notify->fix_included)
    {
      tm_lr_resp_type breach_pos_fix;

      tm_util_convert_lr(&p_gm_client_breach_notify->pos_fix_rpt, &breach_pos_fix);

      p_SuplReport->m.reportDataListPresent = TRUE;

      p_SuplReport->reportDataList.n = 1;
      p_SuplReport->reportDataList.elem = &z_supl_ReportData;

      z_supl_ReportData.m.positionDataPresent = TRUE;


      /* LAT: unit for latitude from GM breach (of the type of tm_lr_resp_type) :
         180/2^25 degrees, 2's compliment; obtain positionEstimate.latitude
         (GAD) from degrees:  * 2^23/90  --- so to convert from GM breach to
         positionEstimate.latitude:  /2
       */
      if (breach_pos_fix.latitude >= 0)
      {
        z_supl_ReportData.positionData.position.positionEstimate.latitude =
           (OSUINT32)(breach_pos_fix.latitude >> 1);

        z_supl_ReportData.positionData.position.positionEstimate.latitudeSign =
           supl_PositionEstimate_latitudeSign_north;
      }
      else
      {
        q_temp = (uint32)(0 - breach_pos_fix.latitude);
        q_temp >>= 1;
        q_temp |= 0x80000000;

        z_supl_ReportData.positionData.position.positionEstimate.latitude
           = (OSUINT32)q_temp;
      }

      /* LONG: unit for longitude from GM breach (of the type of tm_lr_resp_type):
         180/2^25 degrees, 2's compliment; obtain positionEstimate.longitude
         (GAD) from degrees: * 2^23 / 180 -- so to convert from GM breach to
         positionEsimate.longitude: /4
       */
      z_supl_ReportData.positionData.position.positionEstimate.longitude =
         breach_pos_fix.longitude >> 2;


      /* ALTITUDE : meters */
      if (breach_pos_fix.height_incl)
      {
        z_supl_ReportData.positionData.position.positionEstimate.m.altitudeInfoPresent = TRUE;

        if ((breach_pos_fix.height - 500) >= 0)
        {
          z_supl_ReportData.positionData.position.positionEstimate.altitudeInfo.altitude =
             breach_pos_fix.height - 500;

          z_supl_ReportData.positionData.position.positionEstimate.altitudeInfo.altitudeDirection =
             supl_AltitudeInfo_altitudeDirection_height;
        }
        else
        {
          q_temp = (uint32)(0 - (breach_pos_fix.height - 500));
          z_supl_ReportData.positionData.position.positionEstimate.altitudeInfo.altitude =
             q_temp;

          z_supl_ReportData.positionData.position.positionEstimate.altitudeInfo.altitudeDirection =
             supl_AltitudeInfo_altitudeDirection_depth;

        }

      } /* if altitude included */

      /* TBD: unc. velocity, etc. */

      /* mandatory timestamp */
      memset(timestamp, 0, 14);

#if 0  /* test code */
      w_FixWn = 1637;
      q_fix_time_gps_msec = 525981;
      q_fix_time_utc_msec =  q_fix_time_gps_msec;
#endif

      qw_equ(qword_timestamp, breach_pos_fix.timestamp);

      //t_TS = *((uint64 *)((void *)p_gm_client_breach_notify->pos_fix.timestamp));
      t_TS = *((uint64 *)((void *)&qword_timestamp));

      /* Section of code leverated from mc_pgi.c mc_pgi_tt_data_proc() */
      /* Compute the number of GPS weeks elapsed */
      w_FixWn = (uint16)((t_TS >> 16) / WEEK_1P25MSECS);
      /* Compute Time of Week in units of 1.25msec. */
      /*lint -e{647} milliseconds of week in 0.25msec unit can be fit into 32-bit integer */
      q_Gps0P25Msec = (uint32)(((t_TS >> 16) - (w_FixWn * WEEK_1P25MSECS)) * 5);
      /* Convert the TimeofWeek to millisecods of the week. */
      q_fix_time_gps_msec = q_Gps0P25Msec >> 2;

      q_fix_time_utc_msec = q_fix_time_gps_msec - 15000; /* 15 sec. lead time FOR NOW */
      z_utc_time = tm_util_construct_utc_time(w_FixWn,
                                              q_fix_time_gps_msec,
                                              q_fix_time_utc_msec);

      /* 2 digit year */
      z_utc_time.q_year = z_utc_time.q_year % 100;

      /* 10th digit year converted to ASCII */
      timestamp[0] = z_utc_time.q_year / 10 + 0x30;

      /* 1th digit year to ASCII */
      timestamp[1] = z_utc_time.q_year -
         ((uint32)(z_utc_time.q_year / 10) * 10) + 0x30;

      /* 10th digit month to ASCII */
      timestamp[2] = z_utc_time.q_month / 10 + 0x30;

      /* 1th digit month to ASCII */
      timestamp[3] = z_utc_time.q_month -
         ((uint32)(z_utc_time.q_month / 10) * 10) + 0x30;

      /* 10th digit day to ASCII */
      timestamp[4] = z_utc_time.q_utc_days / 10 + 0x30;

      /* 1th digit day to ASCII */
      timestamp[5] = z_utc_time.q_utc_days -
         ((uint32)(z_utc_time.q_utc_days / 10) * 10) + 0x30;

      /* 10th digit hour to ASCII */
      timestamp[6] = z_utc_time.q_hours / 10 + 0x30;

      /* 1th digit hour to ASCII */
      timestamp[7] = z_utc_time.q_hours -
         ((uint32)(z_utc_time.q_hours / 10) * 10) + 0x30;

      /* 10th digit minute to ASCII */
      timestamp[8] = z_utc_time.q_mins / 10 + 0x30;

      /* 1th digit minute to ASCII */
      timestamp[9] = z_utc_time.q_mins -
         ((uint32)(z_utc_time.q_mins / 10) * 10) + 0x30;

      /* 10th digit second to ASCII */
      timestamp[10] = z_utc_time.q_secs / 10 + 0x30;

      /* 1th digit second to ASCII */
      timestamp[11] = z_utc_time.q_secs -
         ((uint32)(z_utc_time.q_secs / 10) * 10) + 0x30;

      timestamp[12] = 'Z';

      z_supl_ReportData.positionData.position.timestamp = &timestamp[0];

    } /* if position available */

  }  /* for geofence breach report */

  encode_status = tm_osys_asn1_encode_pdu((void *)&pdu, &pdu_buff, PROTO_SUPL);

  if ((encode_status != 0) || (pdu_buff.value == NULL) || (pdu_buff.length == 0))
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }
  }

  else
  {
    pd_comms_return_type ret_val;

    p_length = (uint8 *)pdu_buff.value;
    u_length_msb = (pdu_buff.length >> 8) & 0x00FF;
    u_length_lsb = pdu_buff.length & 0x00FF;

    *p_length++ = u_length_msb;
    *p_length = u_length_lsb;

    ret_val = tm_umts_up_supl_comm_write((uint8 *)pdu_buff.value, pdu_buff.length);

    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplReport attempt aborted", 0, 0, 0);
    }
    else
    {
      if (p_gm_client_breach_notify != NULL)
      {

        if (p_gm_client_breach_notify->is_last_report)
        {
          /* if known to be the last breach report, keep tcp on for downlink SUPL_END */
          supl_set_cf_state_area_event(C_SUPL_CF_STATE_UT3_LAST_SUPLREPORT, u_session_num);
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                "Keep tcp link after SUPL_REPORT - last rprt: %u",
                p_gm_client_breach_notify->is_last_report);

        }
        else
        {
          if (!(z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT))
          {
            tm_umts_up_supl_comm_close_connection();
            MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                  "Disc. after SUPL_REPORT sent: %u, %u",
                  p_gm_client_breach_notify->is_last_report,
                  z_supl_session_status.e_cf_state);

          }
          else
          {
            MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                  "Do NOT disconn due to UT3_EXPECT_SUPLREPORT on: %u, %u",
                  p_gm_client_breach_notify->is_last_report,
                  z_supl_session_status.e_cf_state);
          }
        }
      }

      if (p_hash != NULL) /* response to sessionInfoQuery */
      {
        supl_set_cf_state(C_SUPL_CF_STATE_UT10_INFO_QUERY_END, 0);
        /* UT10: use the same NV item for UT2 here */
      }

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL_REPORT sent", 0, 0, 0);
      u_ret = TRUE;
    }

    /*Free the encoded PDU*/
    tm_asn1_free_osys_buf(pdu_buff.value, PROTO_SUPL);
    pdu_buff.value = NULL;

  }

  if (p_gm_client_breach_notify != NULL)
  {

    z_sm_gm_breach_ack_notify.geofence_id =
       z_supl_area_event_session_status[u_session_num].q_geofence_id;

    if (u_ret) /* ACK GM */
    {
      /* SUPL_REPORT for breach report has been sent from SM perspective */
      z_sm_gm_breach_ack_notify.accepted = TRUE;
      u_sm_gm_error_code = GEOFENCE_ERROR_NONE;
    }

    else /* NACK GM*/
    {
      /* SUPL_REPORT for breach report has failed to be sent from SM perspective */
      z_sm_gm_breach_ack_notify.accepted = FALSE;
      u_sm_gm_error_code = GEOFENCE_ERROR_UNSPECIFIED;
    }

    gm_breach_ack(&z_sm_gm_breach_ack_notify, &u_sm_gm_error_code);
  }

  return u_ret;

}

/*
******************************************************************************
* tm_umts_up_supl_gm_init
*
* Function description:
*
*   Regigster SUPL with the Geofence Module
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/

void tm_umts_up_supl_gm_init(void)
{
  uint8 error_code = 0;
  sm_gm_client_type client;
  sm_gm_client_geofences_handles_request_type gm_get_handles_request;
  sm_gm_set_client_config_type gfClientConfig = { 0 };

  client.client_id = GEOFENCE_CLIENT_SUPL;
  client.q_base_msg_id = TM_CORE_GM_MSG_ID_BASE;
  client.q_base_transaction_id = TM_UMTS_UP_GM_TRANSACTION_ID_BASE;
  client.q_client_thread_id = THREAD_ID_SM_TM;

  client.client_cap.client_capabilites =
     GEOFENCE_CLIENT_POS_FIX_CAPABLE | GEOFENCE_CLIENT_GEOFENCE_SERVICE_REQ;
  client.client_cap.pos_fix_capability.pos_fix_method_cap = GEOFENCE_CLIENT_POS_FIX_AGNSS_UP_CAPABLE | GEOFENCE_CLIENT_POS_FIX_CELL_ID_CAPABLE;
  client.client_cap.pos_fix_capability.pos_fix_accuracy_cap = GEOFENCE_CLIENT_POS_FIX_ACCURACY_HIGH;
  client.client_cap.pos_fix_capability.pos_fix_accuracy = 2; /*Minimum accuracy can be as low as 2 meters*/
  client.client_cap.pos_fix_capability.pos_fix_power_cap = GEOFENCE_CLIENT_POS_FIX_POWER_PROFILE_2;
  client.client_cap.pos_fix_capability.high_power_proc_wakeup_cap = GEOFENCE_CLIENT_HIGH_POWER_PROC_WAKEUP_PROFILE_1;
  client.client_cap.pos_fix_capability.nw_usage_cap = GEOFENCE_CLIENT_NW_USAGE_PROFILE_1;

  client.client_cap.pos_fix_capability.is_imp_location_notification_required = FALSE;
  client.client_cap.pos_fix_capability.pos_fix_wait_secs = 60; /*Geofence core shall wait for 60 seconds for the fix*/
  client.client_cap.pos_fix_capability.is_prior_fix_request_indication_required = FALSE; /*Prior fix request indication required*/
  client.client_cap.pos_fix_capability.prior_fix_request_indication_secs = 0; /*Prior fix request indication required 2mins beforehand*/

  gfClientConfig.q_mask = (SM_GM_CLIENT_CONFIG_MAX_GEOFENCES_MASK |
                              SM_GM_CLIENT_CONFIG_GNSS_POS_QOS_SESS_TIMEOUT_MASK |
                              SM_GM_CLIENT_CONFIG_CPI_REQUEST_RATE_MASK |
                              SM_GM_CLIENT_CHAL_GPS_ENV_BO_MIN_MASK |
                              SM_GM_CLIENT_CHAL_GPS_ENV_BO_MAX_MASK |
                              SM_GM_CLIENT_CHAL_GPS_ENV_MS_DIST_GPS_MASK |
                              SM_GM_CLIENT_CHAL_GPS_ENV_MS_DIST_CPI_MASK |
                              SM_GM_CLIENT_CONFIG_GNSS_UNAVAIL_AGGR_EXIT_MASK);

  gfClientConfig.q_max_geofences = 20;
  gfClientConfig.q_gnss_pos_qos_sess_timeout = GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT_DEFAULT;
  gfClientConfig.q_cpi_request_rate = GNSS_NV_EFS_SM_GM_CPI_REQUEST_RATE_DEFAULT;
  gfClientConfig.q_chal_gps_env_bo_min = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_MIN_DEFAULT;
  gfClientConfig.q_chal_gps_env_bo_max = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_MAX_DEFAULT;
  gfClientConfig.q_chal_gps_env_ms_dist_gps = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_GPS_DEFAULT;
  gfClientConfig.q_chal_gps_env_ms_dist_cpi = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_CPI_DEFAULT;
  gfClientConfig.gnss_unavail_aggresive_exit = FALSE;

  if (gm_client_register(&client, &supl_gm_handle, &gfClientConfig, &error_code) == FAILED)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Not able to add SUPL as Geofence client, error_code %d", error_code, 0, 0);
  }
  else
  {
    gm_get_handles_request.client_handle =  supl_gm_handle;
    gm_get_handles_request.q_transaction_id =  supl_comm_getnext_gm_transaction_id();

    if (gm_get_client_geofences_handles(&gm_get_handles_request, &error_code) == FAILED)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "gm_client handle get failed, error_code %d", error_code, 0, 0);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "gm_client handle get successful, error_code %d", error_code, 0, 0);
    }
  }

}

/*
******************************************************************************
* supl_session_query_proc
*
* Function description:
*
*   Process SUPL_INIT - SessionInfoQuery
*
* Parameters: p_hash: pointer to HASH.
*
* Return value: None.
*
******************************************************************************
*/

static void  supl_session_query_proc(uint8 *p_hash,
                                     supl_slp_session_id *p_provided_slp_session_id,
                                     uint32 q_supl_version)
{
  if (tm_umts_up_supl_supl_report_prepare_or_tx(0, NULL, p_hash, p_provided_slp_session_id))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_REPORT for query sent", 0, 0, 0);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL_REPORT for query pending", 0, 0, 0);
    memscpy(&z_supl_session_status.z_session_id.z_Slp_Sess_Id,
            sizeof(z_supl_session_status.z_session_id.z_Slp_Sess_Id),
            p_provided_slp_session_id, sizeof(*p_provided_slp_session_id));
    memscpy(&z_supl_session_status.supl_init_hash[0], sizeof(z_supl_session_status.supl_init_hash), p_hash, sizeof(z_supl_session_status.supl_init_hash));
    z_supl_session_status.u_hash_valid = TRUE;
    z_supl_session_status.q_supl_version = q_supl_version;
  }

}
/*
******************************************************************************
* supl_session_is_emergency_session
*
* Function description:
*
*   Return if the SUPL INIT is for an emergency session and if ESLP was provided
*
* Parameters:
*
* Return value: TRUE/FALSE
*
******************************************************************************
*/

boolean  supl_session_is_emergency_session(void)
{
  if ((z_supl_session_status.z_emergency_data.b_valid == TRUE) &&
      (z_supl_session_status.z_emergency_data.b_emergency_ni == TRUE))
  {
    return TRUE;
  }
  return FALSE;

}


/*
******************************************************************************
* tm_umts_up_is_supl_enabled
*
* Function description:
*
*   Return if SUPL is enabled or not
*
* Parameters:
*
* Return value: TRUE/FALSE
*
******************************************************************************
*/

boolean tm_umts_up_is_supl_enabled(void)
{

  if (((z_umts_nv_item_ram_copy.gnss_supl_version >> 16) & 0x000000FF) == 0)
  {
    return FALSE;
  }

  return TRUE;
}

/*
******************************************************************************
* tm_umts_up_supl_rrlp_assisted_bds_enabled
*
* Function description:
*
* Check if RRLP for assisted BDS is provisioned for SUPL only when UE's
* position protocol is RRLP.
* 
* Parameters: None
* 
* Return value: TRUE - RRLP for assisted BDS is provisioned for SUPL.
*               FALSE - RRLP for assisted BDS is not provisioned for SUPL.
*
******************************************************************************
*/
boolean tm_umts_up_supl_rrlp_assisted_bds_enabled(void)
{
  MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "prtl %d bds_enbl %d suplversion 0x%x abds_pos_mode %d",
        z_supl_session_status.u_position_protocol,
        tm_umts_common_bds_enabled(),
        z_supl_session_status.q_supl_version,
        (TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED)));
#ifdef FEATURE_ABDS_RRLP_SUPL
  if ((TM_POS_PRTL_RRLP == z_supl_session_status.u_position_protocol) &&
      (tm_umts_common_bds_enabled()) &&
      ((z_supl_session_status.q_supl_version & 0x000000FF) >=  2) &&          /* Major Version >= 2 */
      //(((z_supl_session_status.q_supl_version >> 8)  & 0x000000FF ) >=  0 ) &&  /* Minor Version >= 0 */
      (((z_supl_session_status.q_supl_version >> 16) & 0x000000FF) >=  2) &&  /* Servind Indication >= 2 */
      (TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED))
     )
  {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
#else
  return FALSE;
#endif
}

/*
******************************************************************************
* tm_umts_up_supl_rrlp_assisted_glonass_enabled
*
* Function description:
*
* Check if RRLP for assisted GLONASS is provisioned for SUPL only when UE's
* position protocol is RRLP.
* 
* Parameters: None
* 
* Return value: TRUE - RRLP for assisted GLONASS is provisioned for SUPL.
*               FALSE - RRLP for assisted GLONASS is not provisioned for SUPL.
*
******************************************************************************
*/
boolean tm_umts_up_supl_rrlp_assisted_glonass_enabled(void)
{
    if (TM_POS_PRTL_RRLP == z_supl_session_status.u_position_protocol)
    {
      if (tm_umts_common_glonass_enabled() &&
          (TM_CORE_CONFIG_DD(asst_glonass_pos_protocol) & PDSM_PA_ASST_GLO_PROTOCOL_RRLP_UP) &&
          ((z_supl_session_status.q_supl_version & 0x000000FF) ==  2)
          )
      {
        return (TRUE);
      }
    }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Pos protocol %u", z_supl_session_status.u_position_protocol);
  return (FALSE);
}

/********************************************************************
* supl_chk_if_external_modem_OOS
*
* Function description:
*   Processing cell db update from external sources coming in from pdapi.
*
* Parameters: pdsm_cell_info_s_type
*
* Return:
*  TRUE/FALSE
*
*********************************************************************
*/
boolean supl_chk_if_external_modem_OOS(const pdsm_cell_info_s_type external_cell_info)
{
  boolean external_celldb_OOS = FALSE;

  /* Checking two cases to verify if external modem is OOS
        1. When the serviceStatus says OOS or UNK
        2. When LAC and CID is -1. This happens when we receive an inject external cell update
            that indicate that we are camped on a non-gsm RAT. And for SGLTE that means its camped on
            the main modem. If RAT is non-gsm then LAC and CID will be -1
    */
  if (((external_cell_info.serviceStatus == STATUS_OUT_OF_SERVICE) ||
       (external_cell_info.serviceStatus == STATUS_UNKNOWN)) ||
      ((external_cell_info.cellInfo.gsmCell.LAC == HEX_NEGATIVE_ONE) &&
       (external_cell_info.cellInfo.gsmCell.CID == HEX_NEGATIVE_ONE)))
  {
    external_celldb_OOS = TRUE;
  }

  return external_celldb_OOS;
}


/********************************************************************
*
* tm_umts_up_supl_external_cell_db_update
*
*   Processing cell db update from external sources coming in from pdapi.
*
* Parameters:
*
* Return:
*  None
*
*********************************************************************
*/
void tm_umts_up_supl_external_cell_db_update(const pdsm_cell_info_s_type external_cell_info)
{

  static char s_Buf[256];

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
        "Cached external cellDB valid : %u, New external cellDB valid : %u",
        !supl_chk_if_external_modem_OOS(z_external_cell_info_cached),
        !supl_chk_if_external_modem_OOS(external_cell_info));


  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External cell info: MCC = %d, MNC = %d, LAC = %d",
        external_cell_info.cellInfo.gsmCell.MCC,
        external_cell_info.cellInfo.gsmCell.MNC,
        external_cell_info.cellInfo.gsmCell.LAC);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External cell info: CID = %d, serviceStatus = %d, RAT = %d",
        external_cell_info.cellInfo.gsmCell.CID,
        external_cell_info.serviceStatus,
        external_cell_info.activeRAT);

  /* Currently support external cell updates for GSM RAT only */
  if ((external_cell_info.activeRAT != PDSM_ACTIVERAT_GSM) &&
      (external_cell_info.activeRAT != PDSM_ACTIVERAT_NONE))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Currently SGLTE architecture supports only GSM cell updates", 0, 0, 0);
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External celldb not updated", 0, 0, 0);
    return;
  }

  // Check if the cell-id information received from external source is valid
  if (supl_chk_if_external_modem_OOS(external_cell_info))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External Cell update is INVALID/OOS", 0, 0, 0);
  }
  else
  {
    // The cell-id information received from external source is valid
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External Cell update is VALID", 0, 0, 0);
  }

  // Update cached celldb info if valid or not
  memscpy(&z_external_cell_info_cached, sizeof(z_external_cell_info_cached),
          &external_cell_info, sizeof(external_cell_info));


  if (z_external_cell_info_cached.activeRAT == PDSM_ACTIVERAT_GSM)
  {
    MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "Cached External Cell Updates for GSM CellDB: Valid=%u,MCC=%lu,MNC=%lu,LAC=%lu,CID=%lu\n",
          supl_chk_if_external_modem_OOS(z_external_cell_info_cached),
          z_external_cell_info_cached.cellInfo.gsmCell.MCC,
          z_external_cell_info_cached.cellInfo.gsmCell.MNC,
          z_external_cell_info_cached.cellInfo.gsmCell.LAC,
          z_external_cell_info_cached.cellInfo.gsmCell.CID);

  }


  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Cached external cell info: MCC = %d, MNC = %d, LAC = %d",
        z_external_cell_info_cached.cellInfo.gsmCell.MCC,
        z_external_cell_info_cached.cellInfo.gsmCell.MNC,
        z_external_cell_info_cached.cellInfo.gsmCell.LAC);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External cell info: CID = %d, serviceStatus = %d, RAT = %d",
        z_external_cell_info_cached.cellInfo.gsmCell.CID,
        z_external_cell_info_cached.serviceStatus,
        z_external_cell_info_cached.activeRAT);



  /************** for displaying diag. message only *********************/
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "External sources updating CellDB for RAT: %u, validity = %u",
        z_external_cell_info_cached.activeRAT,
        !supl_chk_if_external_modem_OOS(z_external_cell_info_cached));
}


/********************************************************************
*
* tm_umts_up_supl_get_cell_db_type
*
* Function description:
*   Decides which cell db should be considered for SUPL messages from cell_db_type
*
* Parameters: None
*
* Return:
*  One of the cell db types defined in cell_db_type
*
*********************************************************************
*/
cell_db_type tm_umts_up_supl_get_cell_db_type(void)
{

  cell_db_type cell_db = CELL_DB_NONE;

  /* case where there are no external Cell db updates or external cell DB is invalid (OOS/UKN) */
  if ((supl_chk_if_external_modem_OOS(z_external_cell_info_cached)) &&
      (z_cell_info_cached.u_CachedCellIDValid == TRUE))
  {
    cell_db = CELL_DB_MAIN_MODEM;
  }
  /* case where we have external cell db updates and they are valid
        ( camped-roaming/camped-not-roaming). While the main modem's
        cell db could be valid/invalid, we prefer the external cell db */
  else if (!supl_chk_if_external_modem_OOS(z_external_cell_info_cached))
  {
    cell_db = CELL_DB_EXTERNAL_MODEM;
  }
  /* case where both cell DB are invalid */
  else if ((supl_chk_if_external_modem_OOS(z_external_cell_info_cached)) &&
           (z_cell_info_cached.u_CachedCellIDValid == FALSE))
  {
    /* cell_db is already set to CELL_DB_NONE  */
  }

  return cell_db;
}

/********************************************************************
*
* supl_session_status_reset
*
* Function description:
*   Use this common garbage collector function to reset the
*   z_supl_session_status global object & any pointers to avoid memory leaks.
*
*********************************************************************
*/
void supl_session_status_reset(void)
{
  /* Reset LPP UP state/var/timer etc !!! */
#if defined (FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
  if (z_supl_session_status.u_position_protocol == TM_POS_PRTL_LPP)
  {
    tm_lpp_up_init();
  }
#endif /* FEATURE_CGPS_LTE_CELLDB && ! FEATURE_GNSS_LOW_MEMORY */


  /* Zeroing */
  memset(&z_supl_session_status, 0, sizeof(supl_status_s_type));
}

/********************************************************************
*
* tm_umts_up_supl_ota_delete_proc
*
* Function description:
*   Process OTA Delete
*
*
*********************************************************************
*/
void tm_umts_up_supl_ota_delete_proc(void)
{
  sm_gm_purge_request_type z_gm_purge_request;
  uint8 u_error_code = 0;

  (void)os_TimerStop(supl_alm_req_holdoff_timer);
  (void)os_TimerStop(supl_eph_req_holdoff_timer);
  (void)os_TimerStop(supl_iono_req_holdoff_timer);
  (void)os_TimerStop(supl_utc_req_holdoff_timer);
  (void)os_TimerStop(supl_refloc_req_holdoff_timer);
  (void)os_TimerStop(supl_gf_holdoff_timer);
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "OTA-delete resets SUPL holdoff timers", 0, 0, 0);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Clearing away NI Geofences", 0, 0, 0);
  memset(&z_supl_area_event_session_status[0],
         0,
         sizeof(supl_area_event_status_s_type) * C_MAX_GEOFENCE_NUM);

  memset(&z_gm_purge_request, 0, sizeof(sm_gm_purge_request_type));

  memset(&z_supl_cached_pos_for_net, 0, sizeof(tm_umts_common_pos_for_net_s_type));

  z_gm_purge_request.client_handle = supl_gm_handle;
  z_gm_purge_request.q_transaction_id = supl_comm_getnext_gm_transaction_id();
  (void)gm_purge_geofence(&z_gm_purge_request, &u_error_code);
}


/********************************************************************
*
* supl_is_wifi_scan_needed
*
* Function description:
*   Check if WiFi scan is needed
*
*
*********************************************************************
*/
boolean supl_is_wifi_scan_needed(void)
{
  if (tm_core_info.config_info.wifi_wait_timeout_sec == 0)
  {
    return FALSE;
  }
  else
  {
    if (z_supl_session_status.u_wifi_scan_requested == FALSE)
    {
      z_supl_session_status.u_wifi_scan_requested = TRUE;
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
}


/********************************************************************
*
* supl_request_wifi_scan
*
* Function description:
*   Request WiFi scan
*
*
*********************************************************************
*/

void supl_request_wifi_scan(void)
{
  /* Request for On-Demand Wifi Scan */
  tm_reqOnDemandWifiScan(FALSE, TM_AUXTECH_REQ_ID_SUPLMLID, z_supl_session_status.q_pdapi_client_id);
}



/********************************************************************
*
* tm_umts_up_supl_wifi_scan_report_proc
*
* Function description:
*   Process WIFI scan result
*
*
*********************************************************************
*/

void tm_umts_up_supl_wifi_scan_report_proc(prtl_wifi_scan_event_s_type z_wifi_scan_result)
{
  uint8 u_i;
  locEngWifiApInfoStructT_v01 *wifi_ap_ptr = NULL;
  locEngWifiApAdditionalDataStructT_v01 *wifi_ap_add_info_ptr = NULL;

  cgps_CellInfoCachedStruct z_cgps_CellInfoCached;

  wifi_ap_ptr = z_wifi_scan_result.z_wifi_scan_info.z_wifi_ap_scan_info.first_wifi_ap_ptr;
  wifi_ap_add_info_ptr = z_wifi_scan_result.z_wifi_scan_info.z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr;
  for (u_i = 0;
       (u_i < z_wifi_scan_result.z_wifi_scan_info.z_wifi_ap_scan_info.q_num_wifi_aps) && (wifi_ap_ptr != NULL);
       ++u_i)
  {
    MSG_9(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "WIFI_AP %u, Mask %x, MAC: %u %u %u %u %u %u, AP_RSSI %d",
          u_i, wifi_ap_ptr->wifiInfoMask,
          wifi_ap_ptr->mac_address[5], wifi_ap_ptr->mac_address[4], wifi_ap_ptr->mac_address[3],
          wifi_ap_ptr->mac_address[2], wifi_ap_ptr->mac_address[1], wifi_ap_ptr->mac_address[0],
          wifi_ap_ptr->ap_rssi);


    memset(&z_cgps_CellInfoCached, 0, sizeof(z_cgps_CellInfoCached));
    z_cgps_CellInfoCached.u_CachedCellIDValid = TRUE;
    z_cgps_CellInfoCached.e_ActiveRAT = CELLDB_WIFI;
    memscpy(&z_cgps_CellInfoCached.z_CellInfoGlobalCached.wifi.z_wifi_ap_cell_info,
            sizeof(z_cgps_CellInfoCached.z_CellInfoGlobalCached.wifi.z_wifi_ap_cell_info),
            wifi_ap_ptr,
            sizeof(locEngWifiApInfoStructT_v01));
    memscpy((void*)&z_cgps_CellInfoCached.z_CellInfoGlobalCached.wifi.z_wifi_ap_add_cell_info,
            sizeof(z_cgps_CellInfoCached.z_CellInfoGlobalCached.wifi.z_wifi_ap_add_cell_info),
            (void*)wifi_ap_add_info_ptr,
            sizeof(locEngWifiApAdditionalDataStructT_v01));
    tm_umts_up_supl_update_mlid_db(&z_cgps_CellInfoCached);


    wifi_ap_ptr++;
  }

  if ((z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_SUPLSTART) ||
      (z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLSTART)
      )
  {
    /* Got WiFi cell info, need to send SUPL_START now */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sending SUPL_START with WIFI info as WIFI info. arrives at state of %x", z_supl_session_status.e_cf_state, 0, 0);

    /*Check if LTE cell information is available else start a timer of 100 ms 
         to retry cell information get*/
    if ((FALSE == supl_lte_cell_information_construct()))
    {
      /*Move to sub-state C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART, and start timer*/
      supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLSTART, 0);
    }
    else
    {
      if (!supl_SuplStart_tx())
      {
        tm_umts_up_supl_callflow_disrupt_handler();
      }

      /*If SUPL START is succesful, then cftimer is now set to UT1,
        If SUPL START is unsuccesful, then cftimer is stopped.
        So need not take care of cftimer, just cancel the UT0 state if still there
      */
      z_supl_session_status.e_cf_state &= (supl_cf_state_enum_type)~C_SUPL_CF_STATE_UT0_SUPLSTART;
    }
  }

  if ((z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_SUPLPOSINIT) ||
      (z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLPOSINIT)
      )
  {
    /*Check if LTE cell information is available else start a timer of 100 ms 
         to retry cell information get*/
    if ((FALSE == supl_lte_cell_information_construct()))
    {
      /*Move to sub-state C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, and start timer*/
      supl_set_cf_state(C_SUPL_CF_STATE_LTE_CELLID_RETRY_TIMER_SUPLPOSINIT, 0);
    }
    else
    {
      if (!supl_SuplPosInit_tx())
      {
        tm_umts_up_supl_callflow_disrupt_handler();
      }
      else /*SuplPosInit Sent*/
      {
        if (z_supl_session_status.u_position_protocol == TM_POS_PRTL_IS801 &&
            z_supl_session_status.u_callflow_kind == C_SUPL_SESSION_NI)
        {
          /*Start IS801 state machine*/
          if (!supl_start_is801_session(TRUE, NULL))
          {
            /*Unable to start IS801 state machine*/
            tm_umts_up_supl_callflow_disrupt_handler();
          }
        }
      }
      /*If SUPL POS INIT is succesful, then cftimer is now set to UT2,
        If SUPL POS INIT is unsuccesful, then cftimer is stopped.
        So need not take care of cftimer, just cancel the UT0 state
      */
      z_supl_session_status.e_cf_state &= (supl_cf_state_enum_type)~C_SUPL_CF_STATE_UT0_SUPLPOSINIT;
    }
  }

  /* wait for WIFI scan result is over */
  if (z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_NO_TCP);
  }
  if (z_supl_session_status.e_cf_state & C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLSTART)
  {
    tm_umts_up_supl_terminate_timer_cancel_cf_state(C_SUPL_CF_STATE_WIFI_WAIT_TIMER_SUPLSTART);
  }

}

/*
 ******************************************************************************
 *
 * tm_up_slp_support_glonass
 *
 * Function description:
 *   Based on the receive SUPL data decide whether SLP server has GLONASS capability.
 *
 * Parameters:
 *   
 *
 * Return value:
 *  True if SLP is GLONASS capable, so UE can ask for GLONASS AD. Otherwise False.
 *
 ******************************************************************************
*/
boolean tm_up_slp_support_glonass(supl_status_s_type const *const p_supl_session_status)
{
  if (NULL == p_supl_session_status)
  {
    return (FALSE);
  }

  return (p_supl_session_status->u_slp_support_glonass);
}

/*
 ******************************************************************************
 *
 * tm_up_slp_support_bds
 *
 * Function description:
 *   Based on the receive SUPL data decide whether SLP server has BDS capability.
 *
 * Parameters:
 *   
 *
 * Return value:
 *  True if SLP is BDS capable. Otherwise False.
 *
 ******************************************************************************
  */
boolean tm_up_slp_support_bds(supl_status_s_type const *const p_supl_session_status)
{
#ifdef FEATURE_ABDS_RRLP_SUPL
  if (NULL == p_supl_session_status)
  {
    return (FALSE);
  }

  /* Atpresent UE will know if H-SLP supports BDS in Set-Initiated call flow.
  *  For all other call flow types UE assumes H-SLP supports BDS, which is normal.
  */
  if (C_SUPL_SESSION_UI == p_supl_session_status->u_callflow_kind)
  {
    return (p_supl_session_status->u_slp_support_bds);
  }

  return (TRUE); /* Retain the legacy behaviour of asking GLONASS if the SLP capability isn't known */
#else
  return (FALSE);
#endif
}
/*
 ******************************************************************************
 *
 * tm_umts_up_supl_enqueue_msg
 *
 * Function description:
 *   Enqueue the SUPL message in the case where we wait for ACK before sending the
 *   next message
 *
 * Parameters:
 *   tm_pdu_buf_type *pdu_buff
 *   
 * Return value:
 *  
 *
 ******************************************************************************/
void tm_umts_up_supl_enqueue_msg(tm_pdu_buf_type *supl_msg)
{

  supl_msg_queue_element *new_supl_msg;
  pd_comms_return_type ret_val;

  if (NULL == supl_msg)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid Buffer");
    return;
  }

  /*Initialize the Queue here, tm_umts_up_supl_session_init () 
    will check for elements and destroy the queue.
    z_supl_session_status Data strcture is memset to '0'
    in supl_session_status_reset()*/
  if (FALSE == z_supl_session_status.supl_msg_q_status)
  {
    (void)q_init(&(z_supl_session_status.supl_msg_q));
    z_supl_session_status.supl_msg_q_status = TRUE;
  }

  new_supl_msg = (supl_msg_queue_element *)pdsm_getbuf(sizeof(supl_msg_queue_element));

  if (NULL == new_supl_msg)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "PDSM Buffer get failed");
    return;
  }

  memscpy((void *)new_supl_msg->msg, MAX_SUPL_MESSAGE_SIZE, (void *)supl_msg->value, supl_msg->length);

  new_supl_msg->length = supl_msg->length;

  q_put(&(z_supl_session_status.supl_msg_q), &new_supl_msg->link);

  if (q_cnt(&(z_supl_session_status.supl_msg_q)) == 1)
  {
    ret_val = tm_umts_up_supl_comm_write((uint8 *)supl_msg->value, (uint16)supl_msg->length);
    if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
    {
      /* message sending failed */
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosLPP attempt aborted");
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: enqueued SuplPosLPP, packetLen: %d", new_supl_msg->length);
  }
  return;
}

/*
 ******************************************************************************
 *
 * tm_umts_up_supl_process_write_ack
 *
 * Function description:
 *   Dequeue the SUPL message, check if we need to send any more messages and
 *   send it to pd_comms.
 *
 * Parameters:
 * 
 *   
 * Return value:
 *  
 *
 ******************************************************************************
*/
void tm_umts_up_supl_process_write_ack(void *msg_data_ptr)
{
  pd_comms_ipc_write_ack_hdr_type *write_hdr_ptr;
  supl_msg_queue_element *supl_msg = NULL;
  uint16  bytes_written;
  pd_comms_return_type ret_val;

  if (NULL == msg_data_ptr)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: Received Null msg_data_ptr");
    return;
  }

  write_hdr_ptr = (pd_comms_ipc_write_ack_hdr_type *)msg_data_ptr;

  bytes_written = write_hdr_ptr->q_bytes_written;

  if (q_cnt(&(z_supl_session_status.supl_msg_q)) != 0)
  {
    supl_msg = (supl_msg_queue_element *)q_get(&(z_supl_session_status.supl_msg_q));
    if (NULL != supl_msg)
    {
      if (supl_msg->length == bytes_written)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: SSL write ack received for %d bytes", supl_msg->length);
      }
      (void)pdsm_freebuf((char *)supl_msg);
    }

    if (q_cnt(&(z_supl_session_status.supl_msg_q)) != 0)
    {
      supl_msg = (supl_msg_queue_element *)q_check(&(z_supl_session_status.supl_msg_q));
      if (NULL != supl_msg)
      {
        ret_val = tm_umts_up_supl_comm_write((uint8 *)supl_msg->msg, (uint16)supl_msg->length);
        if ((ret_val != PDCOMM_RESULT_WAIT) && (ret_val != PDCOMM_RESULT_OK))
        {
          /* message sending failed */
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SUPL: SuplPosLPP attempt aborted");
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: enqueued next SuplPOSLPP msg");
        }
      }
    }
    else
    {
      if (z_supl_session_status.wait_for_write_ack == TRUE)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL: No more Msg's to ACK, Clear WF-ACK Flag");
        z_supl_session_status.wait_for_write_ack = FALSE;
      }
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unexpected ACK");
  }
}


/*
 ******************************************************************************
 *
 * supl_get_supl_version
 *
 * Function description:
 *   Get the current SUPL version configured on the device.
 *
 * Parameters:
 *  p_maj = Major version
 *  p_min = Minor version
 *  p_servind = Serving Indidation
 *   
 * Return value:
 *  
 *
 ******************************************************************************
*/
boolean supl_get_supl_version(uint8 *p_maj, uint8 *p_min, uint8 *p_servind)
{
  uint8 u_maj, u_min, u_servind;
  u_maj = z_supl_session_status.q_supl_version & 0x000000FF;
  u_min = (z_supl_session_status.q_supl_version >> 8) & 0x000000FF;
  u_servind = (z_supl_session_status.q_supl_version >> 16) & 0x000000FF;

  if (NULL == p_maj || NULL == p_min || NULL == p_servind)
  {
    return FALSE;
  }
  *p_maj = u_maj;
  *p_min = u_min;
  *p_servind = u_servind;
  return TRUE;
}

/*
 ******************************************************************************
 *
 * supl_pbm_get_session_type
 *
 * Function description:
 *   Get the session type from the as_id
 *
 * Parameters:
 *  as_id = Subscription ID
 *
 * Return value:
 *  pbm_session_enum_type = session type
 *
 ******************************************************************************
*/
static pbm_session_enum_type supl_pbm_get_session_type(sys_modem_as_id_e_type as_id)
{
  sys_sys_mode_e_type e_sys_mode           = SYS_SYS_MODE_NONE;
  pbm_session_enum_type e_pbm_session_type = PBM_SESSION_DEFAULT;

  /* Get current sys_mode on the DD sub. */
  e_sys_mode = tm_cm_get_sys_mode_for_as_id(tm_cm_dsds_get_dd_subs());

  /* Find session type based on the app type [SIM or USIM].and the sys_mode. */
  if (MMGSDI_APP_SIM == tm_mmgsdi_get_app_type_from_asid(as_id))
  {
    mmgsdi_slot_id_enum_type e_slot_id = tm_mmgsdi_get_slot_id_from_asid(as_id);

    switch (e_slot_id)
    {
      case MMGSDI_SLOT_1:
        e_pbm_session_type = PBM_SESSION_GPB_1;
        break;
      case MMGSDI_SLOT_2:
        e_pbm_session_type = PBM_SESSION_GPB_2;
        break;
      case MMGSDI_SLOT_3:
        e_pbm_session_type = PBM_SESSION_GPB_3;
        break;
      default:
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_pbm_get_session_type::Invalid Slot ID %d extracted !!", e_slot_id);
        break;
    }
  }
  else
  {
    switch (e_sys_mode)
    {
      case SYS_SYS_MODE_GSM:
      case SYS_SYS_MODE_WCDMA:
      case SYS_SYS_MODE_GW:
      case SYS_SYS_MODE_LTE:
      case SYS_SYS_MODE_GWL:
        {
          switch (as_id)
          {
            case SYS_MODEM_AS_ID_1:
              e_pbm_session_type = PBM_SESSION_LPB_GW;
              break;
            case SYS_MODEM_AS_ID_2:
              e_pbm_session_type = PBM_SESSION_LPB_GW_SECONDARY;
              break;
            case SYS_MODEM_AS_ID_3:
              e_pbm_session_type = PBM_SESSION_LPB_GW_TERTIARY;
              break;
            default:
              MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_pbm_get_session_type::Invalid AS ID %d encountered for sys_mode %d!!", as_id, e_sys_mode);
              break;
          }
        }
        break;
      case SYS_SYS_MODE_CDMA:
        {
          switch (as_id)
          {
            case SYS_MODEM_AS_ID_1:
              e_pbm_session_type = PBM_SESSION_LPB_1X;
              break;
            case SYS_MODEM_AS_ID_2:
              e_pbm_session_type = PBM_SESSION_LPB_1X_SECONDARY;
              break;
            case SYS_MODEM_AS_ID_3:
              e_pbm_session_type = PBM_SESSION_LPB_1X_TERTIARY;
              break;
            default:
              MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_pbm_get_session_type::Invalid AS ID %d encountered for sys_mode %d!!", as_id, e_sys_mode);
              break;
          }
        }
        break;
      default:
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_pbm_get_session_type::Invalid sys_mode %d encountered!!", e_sys_mode);
        break;
    }
  }

  return e_pbm_session_type;
}

/*
 ******************************************************************************
 *
 * supl_get_msisdn_number
 *
 * Function description:
 *   Read the MSISDN number from PBM module
 *
 * Parameters:
 *  msisdn_number = MSISDN number returned from PBM module
 *  num_len = length of the msisdn_number parameter
 *
 * Return value:
 *  pbm_return_type = PBM_SUCCESS/ PBM_ERROR
 *
 ******************************************************************************
*/
static pbm_return_type supl_get_msisdn_number(char *const msisdn_number, uint8 *const num_len)
{
  uint8 *p_data_buf;
  pbm_phonebook_type *p_pb_type;
  uint32 data_buf_size;
  int num_fields;
  pbm_record_id_type rec_id;
  sys_modem_as_id_e_type e_as_id;
  pbm_session_enum_type e_pbm_session_type;

  /* Get PBM session type from sub id. */
  e_pbm_session_type = supl_pbm_get_session_type(tm_core_get_current_dd_subs_id());

  if (NULL == (p_pb_type = (pbm_phonebook_type *)pdsm_getbuf(sizeof(pbm_phonebook_type))))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "pbm_phonebook_type::Not enough memory on heap !!");
    return PBM_ERROR;
  }

  /* Fill pbm_phonebook_type structure with MSISDN as the device type. */
  *p_pb_type = pbm_device_id_to_pb_id(e_pbm_session_type, PBM_MSISDN);

  /* Print pbm_phonebook_type */
  MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "pbm_phonebook_type::pb_category=%d, prov_type=%d, slot_id=%d, device_type=%d",
        p_pb_type->pb_category, p_pb_type->prov_type, p_pb_type->slot_id, p_pb_type->device_type);

  if (0 == pbm_session_get_num_recs(*p_pb_type))
  {
    /* No valid records available. */
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "PBM_MSISDN::No records found !");
    (void)pdsm_freebuf((char *)p_pb_type);
    return PBM_ERROR;
  }

  /* Get record id format form MSISDN record #1. There could possibly be more than one number, but
     only the first will be used here */
  rec_id = pbm_session_location_to_record_id(*p_pb_type, 1);

  data_buf_size = pbm_session_calculate_fields_size_from_id(rec_id);

  if (NULL == (p_data_buf = ((uint8 *)pdsm_getbuf(data_buf_size * sizeof(uint8)))))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Not enough memory on heap !!");
    (void)pdsm_freebuf((char *)p_pb_type);
    return PBM_ERROR;
  }

  /* Read MSISDN record data from PBM. */
  if (PBM_SUCCESS == pbm_session_record_read(rec_id, NULL, &num_fields, p_data_buf, data_buf_size))
  {
    uint8 u_field_index;
    void *p_field_data = NULL;

    /* Parse the received content, from PBM, for the Phone number field. */
    for (u_field_index = 0; u_field_index < num_fields; ++u_field_index)
    {
      p_field_data = pbm_get_field_data_ptr(p_data_buf, u_field_index);

      if (NULL == p_field_data)
      {
        *num_len = 0;
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Field data[0x%x] from PBM is NULL !!", u_field_index);
        break;
      }
      if (PBM_FIELD_PHONE_GENERIC == ((pbm_field_s_type *)p_data_buf)[u_field_index].field_id)
      {
        memscpy(msisdn_number, *num_len, (char *)p_field_data, (uint8)(((pbm_field_s_type *)p_data_buf)[u_field_index].data_len));
        /* Exclude '\0' character at the end while calculating the length. */
        *num_len = ((pbm_field_s_type *)p_data_buf)[u_field_index].data_len - 1;

        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "MSISDN: len=%d, number is ..", *num_len);
        {
          uint8 u_index;
          for (u_index = 0; u_index < *num_len; ++u_index)
          {
            MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "MSISDN: msisdn[%d]=%d", u_index, msisdn_number[u_index]);
          }
        }
        break;
      }
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "msisdn: Record read failed!!");
  }

  (void)pdsm_freebuf((char *)p_pb_type);
  (void)pdsm_freebuf((char *)p_data_buf);

  /* Return PBM_SUCCESS if msisdn data extracted is valid. Otherwise PBM_ERROR. */
  if (*num_len)
  {
    return PBM_SUCCESS;
  }
  return PBM_ERROR;
}

/*===========================================================================
FUNCTION supl_request_l1_mrl_updates

DESCRIPTION
  This function starts a 1second timer to request MRL updates for WCDMA and subscibes to LTE LI MRL updates if in LTE.

PARAMETERS:   
  srv_system
DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void supl_request_l1_mrl_updates(cgps_SrvSystemTypeVal srv_system)
{
#ifdef FEATURE_USER_PLANE_MRL
#ifdef FEATURE_CGPS_WCDMA
  if (srv_system == CGPS_SRV_SYS_WCDMA)
  {
    tm_umts_up_supl_send_wcdma_cell_meas_req();
    (void)os_TimerStart(wcdma_l1_mrl_req_timer, 1000, NULL);
  }
#endif /* FEATURE_CGPS_WCDMA */
#ifdef FEATURE_CGPS_LTE_CELLDB
  if (srv_system == CGPS_SRV_SYS_LTE)
  {
    /* Subscribe MRL from LTE L1. We will receive a call back right away with MRL info through 
         GPS registered callback function l1_lte_cell_meas_update. After subscribing to MRL,
         LTE LI will continue to send MRLs through this call back when new MRL updates are available 
    */
    lte_ml1_subscribe_gps_mrl(tm_core_get_current_dd_subs_id(), TRUE);
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Subscribed to MRL updates with LTE L1");
  }
#endif /* FEATURE_CGPS_LTE_CELLDB */
#endif /* FEATURE_USER_PLANE_MRL */
}

/*===========================================================================
FUNCTION supl_stop_l1_mrl_updates

DESCRIPTION
  This function stops the 1second timer to request MRL updates for WCDMA and un-subscribes to LTE LI MRL updates if in LTE.

PARAMETERS:   none.
  
DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void supl_stop_l1_mrl_updates(void)
{
#ifdef FEATURE_USER_PLANE_MRL
#ifdef FEATURE_CGPS_WCDMA
  (void)os_TimerStop(wcdma_l1_mrl_req_timer);
#endif /* FEATURE_CGPS_WCDMA */

#ifdef FEATURE_CGPS_LTE_CELLDB
  /* Un-subscribe to MRL updates from LTE L1. No more MRL updates sent from LTE L1 */
  lte_ml1_subscribe_gps_mrl(tm_core_get_current_dd_subs_id(), FALSE);
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Un-subscribed to MRL updates with LTE L1");
#endif /* FEATURE_CGPS_LTE_CELLDB */
#endif /* FEATURE_USER_PLANE_MRL */
}

#endif /* #ifdef FEATURE_CGPS_UMTS_UP_SUPL */

