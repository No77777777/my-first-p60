#ifndef TM_DATA_H
#define TM_DATA_H

#include "gps_variation.h"
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================

                  TM_DATA Header File

DESCRIPTION
  This header file contains definitions of data structure necessary for both
  TM & TM protocol tasks.

Copyright (c) 2000 - 2012 Qualcomm Technologies Incorporated.
Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2020 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/prot/tm_data.h#13 $ 
  $DateTime: 2025/02/25 04:54:33 $ 
  $Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/11/22   jv      Reset config with original dd sub once supl session is completed
06/10/20   pa      Add support for PDR Mag Cal State
10/18/19   mj      Remove legacy Spoofing feature
10/16/19   mj      Start CPI request when E911 client is enabled
10/17/19   mj      E911 end handling during concurrency
09/30/19   skm     Support for 911 nDDS on DSDS targets  
08/12/19   sk      Added a NV to set second boundary aligned NMEA Fix report
07/05/19   mj      Handle sub ready event to read IMSI for SUPL
06/06/19   sj      Disable AGNSS for 5G NR.
04/29/19   sj      Add GNSS Diag Buffer feature
04/09/19   mj      Deprecate NV1930
02/22/19   mj      Location Privacy Feature
01/24/19   nath    Added support for ME metrics
09/25/18   nath    Added PQDTM change
08/07/18   skm     AGPS feature Support Mask 
07/25/18   sj      Abort Early Start, LPP CP, UP NILR session if serving rat changes from LTE->OOS->W.
07/20/18   skm     Boeing CaaS Feature
07/17/18   nath    added tm_core_check_sub_capability to enhance deugging
04/02/18   mj      E911 Apptracking Concurrency Support Phase 2
03/20/18   skm     Supl E911 over Wifi
03/09/18   py      Remove "FEATURE_MMGSDI_SESSION_LIB" dependency. True always.
12/14/17   mj      Block sessions during SARF 
06/15/17   kpn     Add AS ID to tm_handle_ota_reset()
05/08/17   rn      Modified NV for LPP12 2 timer feature config to support LPP CP.
05/05/17   kpn     Clear cache of constellations whose meas blk is not received
02/17/17   skm     BLE and Barometer support for LPPE Phase 2 
12/07/16   ss     Using OEM NV Bit 15 to configure Cell CPI Fix for Geofencing
11/16/16   mj      E911 and apptracking concurrency support
09/14/16   rn      Add ULog support for SM.
08/30/16   skm     Added NV for default LPP12 2 timer feature config 
08/30/16   kpn     Add slot id into mmsgsdi global session id table to support pbm_session_record_read()
07/21/16   mj      Add num_fixes tracking in TM session info.
06/23/16   mj      Add changes to get leap second from MGP
06/08/16   mj      Add changes to identify if a session was started for E911
06/06/16   mj      Add GERA support for AP-MP
04/15/16   ld      Added NHz configuration parameter and type 
04/02/16   mj      GERA support
02/17/16   mj      Added NV support to enable/disable CPI injections and early exit logic 
02/11/16   gk      Added LPPe support 
10/27/15   mj      Handle OTA Reset for UMTS 
08/05/15   rh      Added carrier-specific NV 1920 (pos mode support) to TM core 
07/08/15   rh      Changed NV refresh to be done on one specifc sub
06/09/15   ssu     Adding NV for GM filtering of WLAN freq. 
06/01/15   jv      Added QZSS Support
04/06/15   mj      Add new function tm_core_is_active_sess_mo 
03/14/15   rh      Added support for MultiSIM NV 
12/14/14   gk      new session events to PDAPI
11/18/14   ah      Added support for PrisonerSV persistent blacklist SV masks
11/16/14   rh      Added API to get subs ID from MMSGDI table for SIM swap 
10/29/14   ssu     GM LOWI Integration
3/12/14    gk     Handle OTA reset when session is active. consider as end of the session
11/25/14   rk      NV item for LPP Rel10 grt 24 AD 
10/27/14   gk      NV item for terminating SUPL after final fix 
10/24/14   jv      Added Galileo Support
09/10/14   gk      Added delete throttle timer to prevent B-b delete commands
04/28/14   ah      Added handling of Set Premium Services indication status
04/17/14   skm     NV Async Read changes 
02/21/14   ssu   Adding NV for configuring motion sensing distance in case of challenging GPS env. 
02/20/14   ssu     Adding a NV for modifying the medium responsiveness backoff value and gps challenging env backoff config.  
03/10/14   ah      Added support for Get Avail WWAN Pos
02/10/14   ssu     Adding a NV for modifying the rate at which GM requests CPI injection. 
01/31/14    ss     Supporting Max acceptable GF PUNC confguration from QMI
01/10/14   ss      Supporting High Responsiveness Geofencing Config
12/23/13   mj	     Remove FEATURE_DUAL_SIM and FEATURE_TRIPLE_SIM
12/02/13   ah      Support GPS State Info requests for multiple clients simultaneously
10/16/13   rk      Added generic Glonass AcqAsst processing functions.
09/27/13   mj      TSTS Support: Update DSDS_MAX_SESS_TYPES for TSTS
09/12/13   mj      Add new parameter to function tm_mmgsdi_get_sim_info_from_asid
06/07/13   rh      Removed diag timer and NO_FIX report handling 
02/04/13   kj      Move to the DOG Heartbeat mechanism on DIME and beyond
01/31/13   mj       Add new timer api_callback_timer
11/30/12   mj      Added tm_core_lpp_cp_evt_handler and tm_core_external_cell_db_proc
07/17/12   mjohn   Added premium_services_config into  tm_core_config_s_type
03/01/12   rh      Added tm_core_handle_engine_error_recovery_report()
02/06/12   gk      LPP configuration support
06/03/11   gk      Clear cached position  when delete param or gps lock is sent
01/03/11   gk      Eph throttling rework
10/05/10   ns      Port CL1333689: Save status of gps state info req by SYSD.
06/21/10   gk      Save the cached gps state
03/08/10   gk      Save the eph need list
03/03/10   ns      DSDS Support
09/30/09   gk      Support ODp 2.0
07/23/09   gk      Support ODp 2.0
07/10/09   atien   Spirent requested feature:  allow XTRA dload after MSB dload comm failure
6/10/09    gk      Added support for gps utc offset NV item
06/10/09   gk      ODP 1.1 support
6/04/09    gk      Added support for gps utc offset NV item, other inter-position changes
04/02/09   ns      support for qwip
03/11/09   ns      Add variable to store whether sysd was triggered for
                   this session
03/04/09   ns      Created new Data str. for get gps state
02/04/09   atien   MOLR vs AD Call Flow Select support
01/13/09   atien   XTRA-T support
01/27/09   ns      qWip Integration support changes.
08/23/08   gk      GPS lock to be honored for on-demand
08/12/08   gk      Added force reciever off
07/02/08   ank     Added LBS support for CDG OMH R-UIM Spec
06/12/08   lt      Support for external coarse position injection.
04/25/08   gk      on-demand changes
01/21/08   cl      Allow overlapping dload and fix (fix and dload) sessions
09/14/07    gk     NMEA CONFIG set parameters support
07/30/07   gk      Add handling for last position
07/17/07   lt      L1 interface.
06/12/07   cl      Only allow PD API access when TM task is active
05/30/07   lt      Mods. to make TM's RRC and MM iface symmetrical to CM and RR.
04/17/07   lt      Added mo_method into tm_core_config_s_type.
04/04/07   gk/mr   Added function for Time injection handling
01/31/07   cl      Added function for LCS handling
12/04/06    cl     Added NV functionality
10/13/06    cl     Change to use SM_TM for dog report
07/12/06    cl     Initial Release
===========================================================================*/


#include "customer.h"  /* Customer configuration file */
#include "comdef.h"    /* Definition for basic types and macros */
#if !defined (WIN32) && !defined (FEATURE_CGPS_PLAYBACK)
#include "task.h"
#endif
#include "aries_os_globals.h"
#include "aries_os_api.h"
#include "aries_ipc_diag_log.h"   /* For ULog */
#include "tm_common.h"
#include "mmgsdilib_v.h"
#include "tm_prtl_iface.h"
#include "pdapi.h"
#include "tm_cm_iface.h"
#include "tm_pdapi_iface.h"
#include "sm_loc_lte_otdoa.h"
#include "tm_api.h"
#include "sm_nv_efs.h"
#include "tm_spoofDetector_api.h"
#ifdef FEATURE_CGPS_LTE_CELLDB
#include "msgr_umid.h"
#include "lte_tlb.h"
#endif
#include "ldm_api.h"
#include "gnss_diag_buf.h"


#ifdef FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH /* this would be defined in custwcdma.h */
#undef FEATURE_WCDMA_GPS_CELL_ID_READING /* this would be defined in custcgps.h */
#endif

#ifdef FEATURE_GPSONE_BTR
   #define GPSONE_SEC_FLAG 1
#else
   #define GPSONE_SEC_FLAG 0
#endif

/* TM-Core Timer Definition Section */
#define TM_CORE_TIMER_DISPATCH_MASK               0xFF000000

/* TM-Core Transaction ID Definition Section */
#define TM_CORE_TRANSACTION_ID_DISPATCH_MASK      0xFF000000


#define TM_CORE_SESS_TIMER_ID_DIAG                  (0x00000002)
#define TM_CORE_TIMER_ID_API_CALLBACK  (0x00000003)
#define TM_CORE_TIMER_ID_DELETE_THROTTLE_ID  (0x00000004)
#define TM_AUXTECH_WIFI_ON_DEMAND_REQ_TIMER_ID      (0x00000005)
#define TM_DIAG_BUFFER_RESET_TO_STREAMING_TIMER_ID  (0x00000006)

#define TM_CORE_MIN_VELOCTY            0.15 /* 0.15 m/s */

/* constant for the MSB of msec elapsed from 6 Jan 1980 00:00:00 to 6th Jan 2000*/
#define MSB_MSEC_ELAPSED_SINCE_DEFAULT 75

/* TM task info structure */
typedef struct
{
  uint8          task_active : 1;
  uint8          v_mc_task_ready : 1;
//Add flags for other tasks if needed.
  uint8 : 6;
  os_TimerMsgType  *api_callback_timer;
} tm_task_info_s_type;

/* TM-Core default allowed Session handle mask */
#define TM_CORE_ALLOWED_MT_SESSION_HANDLE_MASK  ( (TM_SESS_HANDLE_1X_CP_MT)\
                                                | (TM_SESS_HANDLE_1X_UP_NI)\
                                                | (TM_SESS_HANDLE_UMTS_CP_NI)\
                                                | (TM_SESS_HANDLE_UMTS_UP_NI)\
                                                | (TM_SESS_HANDLE_FTEST)\
                                                )

/*TM_Core masks to enable AGPS on RAT's*/
#define TM_CORE_NV_RAT_ENABLE_CDMA_MASK  0x1
#define TM_CORE_NV_RAT_ENABLE_HDR_MASK   0x2
#define TM_CORE_NV_RAT_ENABLE_WCDMA_MASK 0x4
#define TM_CORE_NV_RAT_ENABLE_GSM_MASK   0x8
#define TM_CORE_NV_RAT_ENABLE_LTE_MASK       0x10
#define TM_CORE_NV_RAT_ENABLE_TDSCDMA_MASK   0x20
#define TM_CORE_NV_RAT_ENABLE_ALL_MASK   0xFF

/* TM-Core allow MO-SUPL during concurrency while engine in early RCVR mode. 
   This mask is used for NV GNSS_NV_EFS_SM_ALLOW_MO_SUPL_DURING_CONCURRENCY */
#define TM_CORE_ALLOW_MO_SUPL_DURING_CONCURRENCY 0x1

/* Used in SUPL & LPP */
#define TM_LPP_MAX_OTDOA_RSTD_MEAS_DATA 3

/* NEW SM feature definitions.
   Please ensure for future features in TM
   Assign a the Bit in the mask and use the same with
   TM_CORE_FEATURE_BIT_IS_SET() to check */
#define TM_SUPL_VOWIFI_EMERGENCY_ENABLE  (0x00000001) /* Enables/Disable SUPL over VoWIFI for 911*/
#define TM_SUPL_ON_NDDS_ENABLE           (0x00000002) 
                                       /*(0xFFFFFFFC)    Reserved for future TM features.*/

/* channelNumber INTEGER (-7..13) as per RRLP 8.6.0, LPP 9.6.0, RRC R8 */
#define C_PROT_GLO_FREQ_MIN GLO_LOWEST_FREQ_NUMBER
#define C_PROT_GLO_FREQ_MAX 13

/* GPS, GLONASS, BDS, GAL SV used as position source to compute the fix (by PE) */
#define C_POS_SOURCE_GPS (0x1<<0)
#define C_POS_SOURCE_GLO (0x1<<1)
#define C_POS_SOURCE_BDS (0x1<<2)
#define C_POS_SOURCE_GAL (0x1<<3)
#define C_POS_SOURCE_WLAN_AP  (0x1<<4)
#define C_POS_SOURCE_ECID_LTE (0x1<<5)
/* Glonass Sv/Channel# sub-buffer structure. Source of data Glo AuxiInfo and/or Glo Almanac */
typedef struct
{
  struct
  {
    unsigned channelNumValid : 1; /* Set to 1 when channel_num is valid */
   } m;
  int8 channel_num;                 /* Insert valid frequency number -7..13 */
} prot_glo_cn_buffer_s_type;

/* Glonass Sv/Channel# buffer structure. Source of data Glo AuxiInfo and/or Glo Almanac */
typedef struct
{
  uint8 u_n;                                            /* Number of valid Sv/ChannelNum */
  prot_glo_cn_buffer_s_type z_glo_sat_elem[N_GLO_SV];   /* Array index is Glonass Sv Id of 0..23 */
} prot_glo_svcn_buffer_s_type;

/* Glonass Acquisition Assistance structure */
typedef struct
{
  struct
  {
    unsigned gnssMsecOfWeekValid : 1;    /* Set 1 for z_steering_glo_data.z_SvSteerInfo.q_Ms to be valid */
    unsigned svDirInjected : 1;          /* When this is 1, the SV direction AD is successfully injected into MGP  */
    unsigned svSteeringInjected : 1;     /* When this is 1, the SV steering AD is successfully injected into MGP */
  } m;

  boolean                          u_freq_num_valid;   /* TRUE if Glonass frequency number in below svdir & steering are valid */
  gnss_SvDirStructType             gnss_sv_dir;         /* SV direction info */
  gnss_SvAcqAssistCommonStructType z_steering_glo_data;/* Steering info */
} prot_glo_acqast_buffer_s_type;

/* Subset of Glonas Acquisition Assistance parameters (Satellite ID/Integer code phase pair) */
typedef PACKED struct PACKED_POST
{
  uint8 satellite_id;         /* 0..63 */
  uint32 integer_code_phase;  /* 0..127 */
} prot_glo_acqasst_subset_s_type;

/* Store subset of Glonass Acquisition Assistance */
typedef struct
{
  uint8 u_cnt;        /* Array size of p_svid_icp */
  prot_glo_acqasst_subset_s_type *p_svid_icp;
} prot_glo_acqasst_sat_buffer_s_type;

/* Glonass Navigation Model structure */
typedef struct
{
  boolean             u_freq_num_valid;
  glo_EphStructType   z_eph_glo_data;
} prot_glo_eph_buffer_s_type;

/*===========================================================================
           Definition of TM-Core Data Structure
===========================================================================*/
typedef void (prtl_init_f_type)(void);

typedef enum
{
  TM_CORE_SESS_STATE_INIT = 0,
  TM_CORE_SESS_STATE_WAIT_AUTH,
  TM_CORE_SESS_STATE_ACCESS_GRANTED,
  TM_CORE_SESS_STATE_RESOURCE_CLEANUP,
  TM_CORE_SESS_STATE_COMPLETED
} tm_core_session_state_e_type;



/* Struct for get pos request*/
typedef struct
{
  pdsm_client_type_e_type       client_type;
  pdsm_client_id_type           client_id;
  pdsm_mt_initiator_e_type      mt_initiator;  /* 0 (not applicable), (1) CP, or (2) UP */
  pdsm_pd_option_s_type         option;        /* PD option type */
  pdsm_pd_qos_type              qos;           /* Ptr to user QOS for this request */
  boolean                       internal_session;
} tm_queued_get_pos_param_s_type;


/* union for tm core get gps state payload*/
typedef  union
{
  pdsm_cmd_s_type *p_cmd_type;
}tm_get_gps_state_payload_u_type;

/* Struct for tm core get gps state payload*/
typedef struct
{
  sm_ReqEnumType source;
  tm_get_gps_state_payload_u_type cmd;
} tm_core_get_gps_state_param_s_type;

typedef tm_core_get_gps_state_param_s_type tm_core_get_best_avail_pos_param_s_type;
typedef tm_core_get_gps_state_param_s_type tm_core_get_avail_wwan_pos_param_s_type;
typedef tm_core_get_gps_state_param_s_type tm_core_get_available_pos_param_s_type;


typedef struct
{
  uint32 q_ClientId;
  uint16 w_TmRcvdDataMask;     /* Bitmask to indicate which data type has been received so far */
} tm_GpsStateInfoClientType;

#define TM_CORE_SESS_INFO_CACHED_INFO_STATE_VALID 0x0001


typedef struct
{
  /* TM-Core Active Session Handle */
  tm_sess_handle_type         active_sess_handle;

  /* Current Operation Mode */
  tm_sess_operation_mode_e_type  op_mode;

  /* Operation already Requested */
  tm_op_req_type                 op_requested;

  /* Protocol sub-module the active session handle belongs to */
  tm_prtl_type                from_protocol;

  /* TM-Core State. Must be updated ONLY using tm_core_update_session_state() */
  tm_core_session_state_e_type session_state;

  /* MO session handle seed */
  unsigned short              mo_sess_handle_seed;

  /* Client information */
  prtl_start_sess_req_param_s_type pd_param;

  /* Session start/done Event firing */
  boolean                    session_event_firing;

  /* Cached eph and alm have-list */
  uint32                     eph_svmask;
  uint32                     alm_svmask;
  uint32                     iono;
  uint32                     utc;
  uint32                     health_svmask;
  uint32                     valid_svmask;

  uint32                     sbas_eph_svmask;
  uint32                     sbas_alm_svmask;

  uint32                     glo_eph_svmask;
  uint32                     glo_alm_svmask;
  uint32                     glo_iono;
  uint32                     glo_utc;
  uint32                     glo_health_svmask;

  uint64                     bds_eph_svmask;
  uint64                     bds_alm_svmask;
  uint32                     bds_iono;
  uint32                     bds_utc;
  uint64                     bds_health_svmask;

  uint32                     qzss_eph_svmask;
  uint32                     qzss_alm_svmask;
  uint32                     qzss_iono;
  uint32                     qzss_utc;
  uint32                     qzss_health_svmask;
  uint64                     gal_eph_svmask;
  uint64                     gal_alm_svmask;
  uint32                     gal_iono;
  uint32                     gal_utc;
  uint64                     gal_health_svmask;

  uint64                     navic_eph_svmask;
  uint64                     navic_alm_svmask;
  uint32                     navic_iono;
  uint32                     navic_utc;
  uint64                     navic_health_svmask;

  boolean                         queued_request;
  tm_queued_get_pos_param_s_type  queued_get_pos_param;
  boolean                         force_eph_download;
  boolean                         glo_force_eph_download;
  boolean                         bds_force_eph_download;
  boolean                         gal_force_eph_download;
  uint32 q_FixTimeRemainingMSec;

  /* Allow XTRA Dload after MSB DLOAD COMM Failure */
  boolean                    msb_xtra_allowed;
  boolean                    end_odp_session;
  /* save the GPS eph need request coming from PE */
  uint32                     eph_need_sv_mask;
  /* save the GLONASS eph need request coming from PE */
  uint32                     glo_eph_need_sv_mask;
  /* save the BDS eph need request coming from PE */
  uint64                     bds_eph_need_sv_mask;
  /* save the GAL eph need request coming from PE */
  uint64                     gal_eph_need_sv_mask;
  /* state info flags. use this instead of adding new booleans */
  uint32                     sess_info_valid_flags; /* see the flag definitions above */

  /* Request for MGP receiver off */
  boolean                    receiver_off_needed;

  /* Bitmask to indicate if request for client (1<<sm_ReqEnumType) is on-going */
  uint32                     q_ReqClientMask;

  /* Structure that holds requesting client specific data (multiple client support) */
  tm_GpsStateInfoClientType  z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_MAX+1];

  /* TRUE if Request XTRA download triggered by AGPS session(SUPL/V2 etc) */
  boolean                    req_xtra_trig_by_agps;

  /* TRUE if current session is started for E911, FALSE otherwise.
        This boolean can be used to track if the ongoing session was
        started for E911, even after the emergency call has ended.
        Eg. Early receiver start for E911, and if NI request does not come in and
        emergency call has ended */
  boolean                    v_session_started_for_e911;
  uint32                     q_num_fixes;
  tm_sess_power_mode_s_type  z_lppm_mode;
  cgps_SrvSystemType         e_srv_system_before_oos;
  boolean                    v_e911_ni_supl_on_dv;
} tm_core_sess_info_s_type;

typedef struct
{
  /* Save client_id for returning back to PD API module */
  pdsm_client_id_type   client_id;
} tm_core_pa_client_info_s_type;

typedef struct
{
  prtl_init_f_type                  *init_fp;
  prtl_start_sess_req_f_type        *start_sess_req_fp;
  prtl_stop_sess_req_f_type         *stop_sess_req_fp;
  prtl_session_req_f_type           *sess_req_data_fp;
  prtl_session_info_f_type          *sess_info_fp;
  prtl_data_pipe_handle_f_type      *data_pipe_handle_fp;
  prtl_timer_cb_f_type              *timer_cb_fp;
  prtl_event_cb_f_type              *event_cb_fp;
  prtl_general_query_handle_f_type  *general_query_fp;
  prtl_gm_event_handle_f_type       *gm_event_handle_fp;
} tm_core_prtl_cb_s_type;

typedef struct
{
  /* Timeout in seconds after which GNSS unavail indication is sent out from GM in a bad GNSS environment */
  uint32 sm_gm_gnss_unavail_ind_timeout;

  /*Geofence Motion Detection Sources*/
  uint32 sm_gm_motion_detection_sources;
  /*Geofence GNSS session timeout*/
  uint8 sm_gm_gnss_pos_qos_sess_timeout;
  /*Geofence Position Sources*/
  uint32 sm_gm_position_sources;
  /*Geofence Max pos unc accepted */
  uint32 sm_gm_gnss_max_pos_unc_accepted;

  /* Geofence use network assisted fixes */
  boolean sm_gm_use_nw_asst_fixes;
   
  /*High Responsiveness Config*/
  uint32 sm_gm_high_responsiveness_config;

  /*GM OEM Control Mask*/
  boolean sm_gm_oem_control_mask;
  /* GM CPI request rate */
  uint32 sm_gm_cpi_request_rate;

  /*GM Medium resp. backoff in seconds */
  uint32 sm_gm_med_resp_backoff;

  /* Backoff config when in Challenging GPS Env in seconds */
  gm_chal_gps_env_backoff_config_s_type sm_gm_chal_gps_env_bo_config;

  /* The motion sensing distance config in case of challenging GPS env */
  gm_chal_gps_env_ms_dist_config_s_type sm_gm_chal_gps_env_ms_dist_config;

  /*The motion state speed config used by Geofencing*/
  gm_ms_speed_config_s_type sm_gm_motion_state_speed_config;

  /*Comprehensive Logging Rate*/
  uint32 q_gm_comp_logging_rate;  

  /* Enable GM LOWI registration */
  boolean sm_gm_lowi_reg_enable;

  /* Mask to subscribe to different services from LOWI */
  uint32 sm_gm_lowi_sub_mask;

  /* Configure how GM registers with LOWI */
  gm_lowi_reg_config_s_type sm_gm_lowi_reg_config;

  /*The ALS IOD sensor config mask used by Geofencing*/
  uint32 q_sm_gm_iod_config_switch_mask;

  /*GM filtering of Wlan freq mask*/
  uint32 q_sm_gm_filter_wlan_freq_mask;
  
  /*Accept Cell Fixes from CPI*/
  boolean v_sm_gm_accept_cell_cpi_fixes;

  /*Enable predictor in GM drive to ped context detector */
  boolean sm_gm_drive_to_ped_predictor_enable;
} geofence_engine_config_s_type;


typedef struct
{
  /* Threshold for Fusion CSM upload*/ 
  uint16 w_sm_fusion_csm_data_upload_threshold_mins;

  /* Size for Fusion CSM SB buffer*/ 
  uint16 w_sm_fusion_csm_sb_buffer_size;

  /* Max number of SSID supported for Fusion CSM*/ 
  uint16 w_sm_fusion_csm_max_ssid_supported;

  /* Venue Tracking resp in sec */ 
  uint16 w_sm_fusion_csm_venue_tracking_resp_sec;
  uint32  u_OperationMode;
  uint8  u_BaroFilterWeight;
  uint8  u_BaroSampleRateHz;
  uint32 q_TestControlMask;

  uint8 u_ChipsetVer;
}fusion_csm_config_s_type;


typedef struct
{
  /* Set Premium Services Config ongoing (TRUE) or not (FALSE) */
  boolean                          prem_svc_cfg_waiting;

  /* If Set Premium Services Config is ongoing, below parameters are valid */
  pdsm_client_id_type              client_id;
  pdsm_pa_premium_svc_cfg_s_type   premium_svc_cfg;
} tm_core_prem_svc_cfg_info;


/* Below are carrier-specific EFS NVs */
typedef struct
{
/* Below from legacy/non-EFS NV */
  /* GPS Lock */
  pdsm_gps_lock_e_type       gps_lock;

  /* NMEA String Output Format */
  uint64                      nmea_output_format;

/* Below from EFS NV */
  /*AGPS rat preference*/
  cgps_nv_agps_rat_pref_config  agps_rat_pref;

  /* Selected protocol (RRLP, RRC, LPP etc) for A-Glonass */
  uint32                      asst_glonass_pos_protocol; // Use PDSM_PA_ASST_GLO_PROTOCOL_XXX to know the choices

  /* LPP enable */
  uint8                       gnss_lte_lpp_enable;

  /*1X UP SUPL Enable*/
  uint8                       gnss_1x_up_supl_enable;

  /* SUPL UDP protocol setting to control UDP BIND */
  uint8                       gnss_sm_udp_allow;

  /* Enable/Disable CPI injections and Early Exit logic */
  uint32                      gnss_sm_emergency_support_config;

  /* Positioning Modes Supported  (NV 1920) */
  uint32                      positioning_modes_supported;

  /* LPPe UP config  */
  uint32                       gnss_lte_lppe_up_config;

  /* LPPe CP config  */
  uint32                       gnss_lte_lppe_cp_config;

  /* BDS Protocol Enable Mask */ 
  uint32                       q_sm_assisted_bds_protocol_enable_mask; 

  /* Advertise UE support for >24 OTDOA AD processing & generate measurements */     
  uint8                        u_gnss_lpp_grt_24_otdoa;

  /* Process earlyTimer value in QOS for LPP Rel12 */ 
  uint8                        u_gnss_lpp_rel12_2timer_support;

  /* SM AGPS features Mask */
  uint32                       q_agps_feature_support;  
                                /*copied from NV item gnss_sm_agps_features (NV 74140)
                                  1) 0x00000001 : Enables SUPL over VoWIFI for 911
                                  2) 0xFFFFFFFE : Reserved for future AGPS features.*/

  /*controls datum -> WGS84/PZ90*/
  uint8                        u_control_nmea_sentence;

  /* Specifies time in seconds to extend E911 state */
  uint32 q_emergency_extension_window;

  /* Represents OEM controls to config GNSS session request and notification to HLOS
                                            bit0 = CP_NILR_OEM_CTRL
                                            bit1 = SUPL_NI_OEM_CTRL
                                            bit2 = NFW_OEM_CTRL 
                                            bit3 = CP_NILR_NOTIFY_OEM_CTRL
                                            bit4 = NFW_NOTIFY_OEM_CTRL */
  uint32 q_gnss_lock_ctrl_for_oem;
  
} tm_core_config_carrier_specific_s_type;

/* macro for easier access to carrier-specific configuration */
#define TM_CORE_CONFIG_DD(x)  (tm_core_info.config_info.subs_cfgs[tm_core_info.config_info.active_subs_cfg].x)

#define TM_CORE_CONFIG_AS_ID(as_id, x)  (tm_core_info.config_info.subs_cfgs[as_id].x)

#define TM_CONFIG_POS_MODE_BIT_IS_SET(x) \
    ( ((tm_core_info.config_info.subs_cfgs[tm_core_info.config_info.active_subs_cfg].positioning_modes_supported) & (x)) != 0)

#define TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET(x) \
    ( ((tm_core_info.config_info.subs_cfgs[tm_core_info.config_info.active_subs_cfg].q_sm_assisted_bds_protocol_enable_mask) & (x)) != 0)

#define TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET_ON_AS_ID(as_id, x) \
        ( ((tm_core_info.config_info.subs_cfgs[as_id].q_sm_assisted_bds_protocol_enable_mask) & (x)) != 0)

#define TM_CORE_FEATURE_BIT_IS_SET(x) \
    (((tm_core_info.config_info.subs_cfgs[tm_core_info.config_info.active_subs_cfg].q_agps_feature_support) & (x)) != 0)

typedef struct
{
  /* UMTS' MO type: CP-LocEstimate, CP-AssistData, and UP:
     initialized to NV_Item cgps_mo_method and can be updated by pdsm_setparam() */
  uint8                      u_mo_method;

  pdsm_pa_nmea_config_s_type nmea_config_type;
  /* smart msb enable/disable */
  boolean                    smart_msb;

  /* UMTS CP MOLR vs AD all Flow Select Mask */
  /*  initialized to NV_Item: aagps_2g_mo_lrsupport and aagps_3g_mo_lrsupport */
  /*  can be updated by pdsm_setparam() */
  uint8                      call_flow_select_mask;
  
 /* ODP 2.0 enable QWIP for ODP */
  boolean                    use_qwip_for_odp;
  /* User plane 1xMSA sessions may be turned off using nv if required, to
   * conserve repeated reqs. for assistance data to the network */
  boolean                    up_1x_msa_disable;
  
  /* preferred xtra data validity age */
  uint16                     preferred_xtra_age;
  /* Only bit 2 used for simulating emergency call.  Bits 6-8 used by PE */
  uint32                     aagps_development_test_control5;
  /* if set to 1, TM will send PE's NO_FIX report as IntermediateReport */
  uint8                      send_no_fix_report;

  /* NMEA callback counter */
  uint8                       nmea_cb_count;

  /* TRUE to accept 1X CP MT even if MO session is active
     Controlled by NV NV_GNSS_OEM_FEATURE_MASK_I bit 2(0x4) */
  boolean                    accept_1xcp_mt_over_mo;

  /*QMI Services config*/
  pdsm_QMIServiceConfig         qmi_cfg;

  /* Config cache for Premium Services */
  uint32                      premium_services_config[PDSM_PA_PREMIUM_SVC_MAX];

  /* Set Premium Services (SAP) Config request while waiting for indication status */
  tm_core_prem_svc_cfg_info   prem_svc_cfg_info;

  /* LPP CP Transport transmission timeout in seconds */
  uint8                       gnss_lpp_cp_timeout_secs;

  /* OEM DRE Tunnel Detection Enablement control flag */
  boolean                     oemdre_td_ctrl;

  /* Emergency support on CP or UP for non-lTE RAT */
  uint32   gnss_emergency_protocol_non_lte;

  /* OEM feature bitmask NV: bit 0x08/0x10 to indicate to pdsm for auto-loop SUPL notification-verification
     back to TM directly. See sm_nv.h for complete OEM feature bit definitions  */
  uint32                    q_gnss_oem_feature_mask;

  /* notify SUPLINIT to AP if GPS locked */
  boolean b_supl_notify_to_ap;

  /*Geofence Engine Config*/
  geofence_engine_config_s_type gm_config;

  /* Timeout value for SM to wait for WiFi scan: if 0, the feature is disabled */
  uint8    wifi_wait_timeout_sec; 
  /* auto accept MT-LR during emergency */
  boolean b_auto_accept_mtlr;

  /* end ongoing LPP periodic session after emergency end */
  boolean b_end_lpp_periodic;
  
  /* Allow MO-SUPL during Concurrency while engine in early RCVR mode */
  uint32 q_allow_mo_supl_during_concurrency;

  /* GNSS Measurement report satellite constellation config */
  uint16     gnss_meas_rep_constell_config;

  /* SV Polynomial report satellite constellation config */  
  uint16     sv_poly_rep_constell_config;

  /* GNSS Measurement report QMI output control via NV */
  uint8     gnss_meas_rep_qmi_out_control;

  /* GNSS Transmission Delays for Protocols  from NV */
  tm_config_agnss_transmission_delay_s_type agnss_trans_delay_config;

  /* end SUPL session after a final fix is reported even if SUPL is communication with server */
  boolean                    b_end_supl_after_final_fix; 

  /* Below are for MultiSIM EFS NV support */
  /* index for currently active */
  uint32 active_subs_cfg;

  /* array to store configurations for all Subs */
  tm_core_config_carrier_specific_s_type  subs_cfgs[SM_CONFIG_MAX_SUBS];

  /* last valid Subscription ID (0 for Single SIM) */
  sys_modem_as_id_e_type max_valid_subs_id; 

  /* Enable/Disable CPI injections and Early Exit logic */
  uint32   gnss_sm_emergency_support_config;

  #ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
  #error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */

  /* Leap Second Information */ 
  mgp_LeapSecondInfoStructType z_LeapSecondInfo;

  /*Enable or disable Concurrency feature*/
  uint32 gnss_sm_concurrent_sessions_support_config;

  /* GNSS NHz Config */
  /* For normal operation, this would have value defined in gnss_common.h
     #define GNSS_NHZ_ENABLED 0x0001
     #define GNSS_NHZ_MODE_AUTOMOTIVE 0x0002
     #define GNSS_NHZ_MODE_DRONE 0x0004
  */
  uint32 gnss_nhz_config; 

  /* Fusion CSM Config*/
  fusion_csm_config_s_type fusion_csm_config;

  /* SARF status (1: RF test in progress; 0: No RF test in progress */
  boolean v_sarf_status;
  pdsm_pa_persist_sv_mask_type z_GnssPersistSV;

  /* Bit to disable usage of Dynamic HEPE */
  uint8   u_disable_Dynamic_HEPE_usage;
  /* GNSS reporting config for Meas,SV Info,Meas Usage,Time params*/
  uint64 t_gnss_reporting_config;

  /* Second boundary aligned NMEA (1-Set,0-Unset)*/
  uint8 u_gnss_second_boundary_aligned_NMEA;

} tm_core_config_s_type;

#define TM_CORE_CONFIG_2TIMER_LPP_UP_BITMASK   (0x1)
#define TM_CORE_CONFIG_2TIMER_LPP_CP_BITMASK   (0x2)

typedef enum
{
  TM_CORE_CONFIG_2TIMER_LPP_UP = 0,
  TM_CORE_CONFIG_2TIMER_LPP_CP = 1
} tm_core_config_2timer_support_enum_type;

#define TM_WIFI_WAIT_TIMEOUT_SEC_MAX  10  /* Wait-for-WIFI-scan-timeout caps at 10 seconds */

#define  TM_RCVD_POS_FROM_PE         0x01
#define  TM_RCVD_TUNC_FROM_ME        0x02
#define  TM_RCVD_ASSIST_DATA_FROM_PE 0x04
#define  TM_RCVD_ALL (TM_RCVD_POS_FROM_PE | TM_RCVD_TUNC_FROM_ME | TM_RCVD_ASSIST_DATA_FROM_PE)

typedef struct
{
  uint32 flags;
  int32  lat;                  /* Latitude:  LSB = Pi/2^32, Range -Pi/2 to Pi/2 */
  int32  lon;                  /* Longitude: LSB = Pi/2^31, Range -Pi to Pi */
  int16  altitude;             /* Altitude in meters */
  uint32 pos_unc;              /* Position Unc in meters */
  uint16 loc_unc_vertical;     /* vertical unc in meters */
  dword  timestamp_tow_ms;     /* Timestamp in GPS TOW milliseconds */
  uint16 timestamp_gps_week;   /* GPS week number */
  uint32 time_unc_us;          /* Time uncertainty in micro seconds */
  uint32 frequency_unc;        /* 1 sided max freq bias uncertainty [m/s] */
  uint32 clk_drift;            /* Clock Drift in meters/sec */
  uint32 clk_bias;             /* Clock frequency bias in meters/sec */
  uint8  engine_state;         /* GPS engine state on/off */
  uint32 eph_svmask;           /* SV Mask for the ephemeris */
  uint32 alm_svmask;           /* SV Mask for the Almanac */
  uint32 iono_valid;           /* IONO is valid */
  uint32 health_svmask;        /* SV Mask for Health */
  uint32 health_bad_svmask;    /* SV Mask for Health */
  uint32 health_noex_svmask;   /* SV Mask for Health */
  uint32 health_unk_svmask;    /* SV Mask for Health */
  uint32 visible_svmask;       /* SV mask for visible SVs */
  uint16 start_gps_week;       /* Current XTRA info is good starting this GPS week */
  uint16 start_gps_minutes;    /* Current XTRA information is good starting this many minutes into the week */
  uint16 valid_duration_hours; /* XTRA info is valid for this many hours */

} tm_core_gps_state_info_s_type;

#ifdef FEATURE_GNSS_PERIODIC_PPM
#error code not present
#endif

typedef struct
{
  /* TM task Info */
  tm_task_info_s_type           tm_task_info;

  /* TM Configuration Info (NV settings) */
  tm_core_config_s_type         config_info;

  /* Session Related Info */
  tm_core_sess_info_s_type      session_info;

  /* Set Parameter Client Info */
  tm_core_pa_client_info_s_type pa_client_info;

  /* Concurrent Session Relelated Info */
  tm_core_pa_client_info_s_type pz_concurrent_client_info;

  /* TM-Core Function Table */
  tm_core_prtl_cb_s_type        prtl_func_cb_table[TM_PRTL_NUM];

  /* Extended data */
  pdsm_pd_data_ext_s_type       pd_ext_data;
  pdsm_pd_opt_field_ext         ext_opt_field_mask; /* validity bits for more fields in the struct pd_ext_data*/
  gnss_MeasBlkStructType        gps_meas_blk;
  gnss_MeasBlkStructType        glo_meas_blk;
  gnss_MeasBlkStructType        bds_meas_blk;
  gnss_MeasBlkStructType        qzss_meas_blk;
  gnss_MeasBlkStructType        gal_meas_blk;
  gnss_MeasBlkStructType        navic_meas_blk;
  gnss_MeasBlkStructType        gps_l2c_meas_blk;
  gnss_MeasBlkStructType        gps_l5q_meas_blk;
  gnss_MeasBlkStructType        glo_g2_meas_blk;
  gnss_MeasBlkStructType        bds_b2_meas_blk;
  gnss_MeasBlkStructType        bds_b2aq_meas_blk;
  gnss_MeasBlkStructType        qzss_l2c_meas_blk;
  gnss_MeasBlkStructType        qzss_l5q_meas_blk;
  gnss_MeasBlkStructType        gal_e5a_meas_blk;

  tm_core_gps_state_info_s_type gps_state_info;
  uint32                        w_RecvdMeasBlkMask;
  /*Cached MGP receiver state*/
  gps_RcvrStateEnumType         e_cached_rcvr_state;

#ifdef FEATURE_GNSS_PERIODIC_PPM
  #error code not present
#endif
  /*If this session is internal*/
  boolean                       internal_session;
  boolean b_pending_ni_resp_from_client;
  os_TimerMsgType*               tm_delete_param_throttle_timer;     /*  throttle timer to throttle back to back delete requests from PDAPI */
  boolean                       cs_on_demand_active;
  boolean                       ext_inj_allowed;
  q_type                        sm_set_param_q;
  boolean                       b_sm_set_param_q_initialized;
  gnss_diag_buffer_hdl          diag_buffer_hdl;
} tm_core_param_s_type;

#ifdef FEATURE_UIM_SUPPORT_LBS
typedef struct
{
   boolean dynamic_mode;
   uint16  seed_pos_options;
}tm_core_ruim_read_buffer;
#ifdef FEATURE_CGPS_XTRA 
typedef struct
{
  boolean  xtra_enabled;
  uint16   download_interval;
  uint8    num_download_attempts;
  uint8    time_between_attempts;
  boolean  auto_download_enabled;
  char     xtra_server_primary[128];     // URL of primary XTRA server
  char     xtra_server_secondary[128];   // URL of secondary XTRA server
  char     xtra_server_tertiary[128];    // URL of tertiary XTRA server
}tm_xtra_ruim_nv_buffer;
#endif /* FEATURE_CGPS_XTRA */
#endif /* FEATURE_UIM_SUPPORT_LBS */

typedef struct
{
  boolean             cached_pos_valid;
  gnss_NavSlnFlagsStructType z_pos_flags;
  pdsm_pd_event_type  pd_event_type;
  pdsm_pd_info_s_type pd_info;
} tm_core_cached_pos_s_type;

typedef struct
{
  boolean                  cached_meas_valid ;
  sm_GpsMeasRptStructType  z_meas;
} tm_core_cached_meas_s_type;


typedef struct
{
  boolean           cached_ref_pos_valid ;
  pdsm_ext_status_info_s_type   pd_ext_status_info;
  boolean           ref_pos_reported;
} tm_core_cached_ref_pos_s_type;

    #define DSDS_MAX_SESS_TYPES   6

typedef struct tm_mmgsdi_session_id_table_entry_s
{
  mmgsdi_session_type_enum_type session_type;
    /* MMGSDI session type GW/1X and PRI/SEC */

  mmgsdi_session_id_type        session_id;
    /* Session-ID assigned by MMGSDI */

  sys_modem_as_id_e_type        asubs_id;
    /* Active Subscription ID assigned by to CM
    ** If not available, set to NONE
    */
  boolean session_id_set;
  mmgsdi_app_enum_type         app_type;
  mmgsdi_slot_id_enum_type     slot_id;
} tm_mmgsdi_session_id_table_entry_s_type;

typedef struct
{
  uint8 					 no_of_sims;
  uint8						 no_of_active_subs;
  tm_core_device_mode_e_type tm_cm_device_info;
  sys_overall_feature_t      tm_cm_overall_device_feature;
  sys_subs_feature_t		 tm_cm_subs_feature[TM_CM_MAX_NO_SUBS];
  sys_ue_mode_e_type		 tm_cm_subs_ue_mode[TM_CM_MAX_NO_SUBS];
} tm_core_device_config_s_type;

/* Gobal variable which store device configuration info such as SS/DSDS/DSDA/TSTS */
extern tm_core_device_config_s_type tm_core_device_config;
extern tm_core_cached_pos_s_type z_cached_pos_info;
extern tm_core_cached_ref_pos_s_type z_cached_ref_pos_info;
extern tm_core_cached_meas_s_type z_cached_meas_info;
extern tm_core_cached_meas_s_type *p_meas_cache;

typedef enum
{
  TM_LPPM_REQ_TYPE_EXTERNAL,
  TM_LPPM_REQ_TYPE_INTERNAL
} tm_lppm_requestor_e_type;

#define SM_TOT_GNSS_MEAS_SYSTEM         5     /// Total GNSS system measurement types supported (GPS + GLONASS + BDS +GAL +QZSS)
#define SM_GNSS_MEAS_SYSTEM_GPS         0
#define SM_GNSS_MEAS_SYSTEM_GLONASS     1
#define SM_GNSS_MEAS_SYSTEM_BDS         2
#define SM_GNSS_MEAS_SYSTEM_GAL         3
#define SM_GNSS_MEAS_SYSTEM_QZSS        4
#define SM_GNSS_MEAS_SYSTEM_INVALID     -1

/* Bitmap for measurement report */
#define SM_GNSS_MEAS_REPORTS_BMAP_GPS     (1<<0)
#define SM_GNSS_MEAS_REPORTS_BMAP_GLONASS (1<<1)
#define SM_GNSS_MEAS_REPORTS_BMAP_BDS     (1<<2)
#define SM_GNSS_MEAS_REPORTS_BMAP_GAL     (1<<3)
#define SM_GNSS_MEAS_REPORTS_BMAP_QZSS    (1<<4)

/*QMI reporting Mask*/
#define SM_GNSS_BMAP_QMI_MEAS_REPORTING        (1<<0)
#define SM_GNSS_BMAP_QMI_SV_POLY_REPORTING     (1<<1)

typedef struct
{
  struct {
    uint8 s_StatusCheckNeeded : 1; /* When build the measurement whether status bit field should be checked or not */
  }m;
  uint8                     u_meas_reports_bmap; /* Use SM_GNSS_MEAS_REPORTS_BMAP_XXX to check which measurement
                                                    report is filled */
  sm_GpsMeasRptStructType   z_meas_reports[SM_TOT_GNSS_MEAS_SYSTEM]; /* Use u_meas_reports_bmap to check which measurement
                                                                        report element is populated. */
} SmGnssMeasReportStructType;

/* Location Notification Struct */
typedef struct
{
  tm_prtl_type                  q_prtl_type; 
  pdsm_client_id_type           e_pdapi_client_id;
  pdsm_client_type_e_type       e_pdapi_client_type;
  tm_sess_operation_mode_e_type e_oper_mode;
  sys_modem_as_id_e_type        e_sub_id;
  pdsm_pd_loc_info_type         e_location_info_type;
  pdsm_pd_loc_response_type     e_location_response;
  boolean                       v_location_privacy_override_set;
  boolean                       v_emergency_bit_set;
} tm_location_notification_s_type;


/* TM ULog Circular Buffer Size */
#define TM_ULOG_SIZE  (1024) 

/*--------------------------------------

      Extern Valuables

---------------------------------------*/
extern tm_core_param_s_type     tm_core_info;

/* For DSDS, EFS cert can be updated anytime to
   program SUPL CERT for a new subscription.
   This boolean will force an EFS read of SUPL
   cert if PDAPI wrote new SUPL CERT into EFS.

*/
extern boolean  tm_use_efs_for_supl_cert;
/* Store the current dd subscription id*/
extern sys_modem_as_id_e_type      tm_current_dd_subs_id;

#define TM_LOW_0( str )              MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "TM: " str)
#define TM_LOW_1( str, a )           MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "TM: " str, a)
#define TM_LOW_2( str, a, b )        MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "TM: " str, a, b)
#define TM_LOW_3( str, a, b, c  )    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "TM: " str, a, b, c)
#define TM_LOW( str, a, b, c )       MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "TM: " str, a, b, c )

#define TM_MED_0( str )              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str)
#define TM_MED_1( str, a )           MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a)
#define TM_MED_2( str, a, b )        MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b)
#define TM_MED_3( str, a, b, c  )    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c)
#define TM_MED_4( str, a, b, c, d)  MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c, d)
#define TM_MED_5( str, a, b, c, d, e)  MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c, d, e)
#define TM_MED_6( str, a, b, c, d, e, f)  MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c, d, e, f)
#define TM_MED_7( str, a, b, c, d, e, f, g)  MSG_7(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c, d, e, f, g)
#define TM_MED_8( str, a, b, c, d, e, f, g, h)  MSG_8(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c, d, e, f, g, h)
#define TM_MED_9( str, a, b, c, d, e, f, g, h, i)  MSG_9(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c, d, e, f, g, h, i)
#define TM_MED( str, a, b, c )       MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: " str, a, b, c )

#define TM_HIGH_0( str )             MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str)
#define TM_HIGH_1( str, a )          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a)
#define TM_HIGH_2( str, a, b )       MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b)
#define TM_HIGH_3( str, a, b, c )   MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c)
#define TM_HIGH_4( str, a, b, c, d)  MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c, d)
#define TM_HIGH_5( str, a, b, c, d, e)  MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c, d, e)
#define TM_HIGH_6( str, a, b, c, d, e, f)  MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c, d, e, f)
#define TM_HIGH_7( str, a, b, c, d, e, f, g)  MSG_7(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c, d, e, f, g)
#define TM_HIGH_8( str, a, b, c, d, e, f, g, h)  MSG_8(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c, d, e, f, g, h)
#define TM_HIGH_9( str, a, b, c, d, e, f, g, h, i)  MSG_9(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c, d, e, f, g, h, i)
#define TM_HIGH( str, a, b, c )      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM: " str, a, b, c )

#define TM_ERROR_0( str )            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM: " str)
#define TM_ERROR_1( str, a )         MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM: " str, a)
#define TM_ERROR_2( str, a, b )      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM: " str, a, b)
#define TM_ERROR_3( str, a, b, c )   MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM: " str, a, b, c)
#define TM_ERROR( str, a, b, c )     MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM: " str, a, b, c )

#define TM_FATAL_0( str )            MSG(MSG_SSID_GPSSM, MSG_LEGACY_FATAL, "TM: " str)
#define TM_FATAL_1( str, a )         MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_FATAL, "TM: " str, a)
#define TM_FATAL_2( str, a, b )      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_FATAL, "TM: " str, a, b)
#define TM_FATAL_3( str, a, b, c )   MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_FATAL, "TM: " str, a, b, c)
#define TM_ASSERT( str, a, b, c )    ERR_FATAL( "TM: " str, a, b, c )

#ifdef TM_DATA_DECLARE
  #define extern
  #define _INITIALIZE(x)  = x
#else
  #define _INITIALIZE(x)
#endif

/* TM ULog Handle */
extern OS_ULOG_HANDLE tm_ULogHandle _INITIALIZE(0);

#ifdef TM_DATA_DECLARE
  #undef extern
#endif
/*-----------------------------------------------------------------------------

                          Extern Functions

-----------------------------------------------------------------------------*/
/*===========================================================================

FUNCTION tm_core_check_sub_capability

DESCRIPTION
   This function provides the sub capability of the desired sub

DEPENDENCIES

RETURN VALUE
boolean

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_check_sub_capability
(
sys_modem_as_id_e_type        sub,
sys_sys_mode_e_type      sys_mode
);

/*===========================================================================

FUNCTION tm_core_init

DESCRIPTION
  This function is to initialize TM-Core data structures.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_init(void);

/*===========================================================================

FUNCTION tm_core_session_active

DESCRIPTION
  This function tells if tmCore is in any active session (MI or MT)

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_session_active(void);

/*===========================================================================

FUNCTION tm_core_abort_recover

DESCRIPTION
  This function performs TM-Core software recovery.

DEPENDENCIES

RETURN VALUE
   None

SIDE EFFECTS

===========================================================================*/
extern void tm_core_abort_recover
(
  boolean                       ask_prtl,
  tm_sess_stop_reason_e_type    abort_reason
);

/*===========================================================================

FUNCTION tm_core_delete_inject_aiding_data

DESCRIPTION
  This function is called to delete or inject aiding data.

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
extern void tm_core_delete_inject_aiding_data
(
  pdsm_delete_parms_type *delete_param_ptr,
  boolean                 force_rcvr_off,
  boolean                 delete_all,
  pdsm_pa_e_type delete_type
);

/*===========================================================================

FUNCTION tm_core_inject_ext_time

DESCRIPTION
  This function is called to inject external time into MGP through LM

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
extern void tm_core_inject_ext_time
(
  pdsm_pd_external_time_info_type *ext_time_ptr
);

/*===========================================================================

FUNCTION tm_core_inject_ext_pos

DESCRIPTION
  This function is called to inject external position into MGP through LM

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_inject_ext_pos
(
  pdsm_cmd_s_type *p_cmd_type
);


/*===========================================================================

FUNCTION tm_core_get_gps_status

DESCRIPTION
  This function is called to get the GPS status information

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_get_gps_status
(
  pdsm_cmd_s_type *p_cmd_type
);


/*===========================================================================

FUNCTION tm_core_update_session_state

DESCRIPTION
  Determine if the state transition is valid and update the tm session
  state.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
extern void tm_core_update_session_state
(
  tm_core_session_state_e_type new_state
);

/*===========================================================================

FUNCTION tm_core_new_mo_sess_handle

DESCRIPTION


DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern tm_sess_handle_type tm_core_new_mo_sess_handle(void);

/*===========================================================================

FUNCTION tm_core_timer_dispatcher

DESCRIPTION
  This function is to figure out which TM software sub-module the timer expiry event
  belongs to and call the proper timer handler (provided to TM-Core by each sub-module)

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_timer_dispatcher(void *timer_data);

/*===========================================================================

FUNCTION tm_core_lm_sess_req_handler

DESCRIPTION
  This function is to handle sess request from LM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_lm_sess_req_handler(void *lm_sess_req_data);

/*===========================================================================

FUNCTION tm_core_lm_sess_update_handler

DESCRIPTION
  This function is to handle sess update from LM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_lm_sess_update_handler(void *lm_sess_update_data);

/*===========================================================================

FUNCTION tm_core_lm_sess_info_handler

DESCRIPTION
  This function is to handle sess info from LM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_lm_sess_info_handler(void *lm_sess_info_data);

/*===========================================================================

FUNCTION tm_core_get_prtl_idx_by_type

DESCRIPTION
  This function is to get protocol array index by protcol type

DEPENDENCIES

RETURN VALUE
  -1, if protocol is not supported.
  Otherwise, array index for the protocol type

SIDE EFFECTS

===========================================================================*/
extern int  tm_core_get_prtl_idx_by_type(tm_prtl_type prtl_type);

/*===========================================================================

FUNCTION tm_core_get_prtl_idx_by_lcs_cmd_type

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  protol array index if lcs_cmd_type is supported
  -1, otherwise

SIDE EFFECTS

===========================================================================*/
extern int tm_core_get_prtl_idx_by_lcs_cmd_type(pdsm_lcs_info_cmd_e_type  lcs_info_cmd_type);

/*===========================================================================

FUNCTION tm_core_sess_req_start

DESCRIPTION
  This function is to figure out if TM-Core accepts session start request from
  protocol sub-module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_sess_req_start(tm_prtl_type                    prtl_type,
                                      tm_sess_handle_type             sess_handle,
                                      tm_sess_req_start_param_s_type  sess_start_param);
/*===========================================================================

FUNCTION tm_core_sess_req_continue

DESCRIPTION
  This function is to figure out if TM-Core accepts session continue request from
  protocol sub-module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_sess_req_continue(tm_prtl_type                       prtl_type,
                                         tm_sess_handle_type                sess_handle,
                                         tm_sess_req_continue_param_s_type  sess_continue_param);

/*===========================================================================

FUNCTION tm_core_sess_req_polling_data

DESCRIPTION
  This function is used to poll LM is data is available

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_sess_req_polling_data(tm_prtl_type                           prtl_type,
                                             tm_sess_handle_type                    sess_handle,
                                             tm_sess_req_polling_data_param_s_type  polling_data_param);


/*===========================================================================

FUNCTION tm_core_sess_req_stop

DESCRIPTION
  This function is to figure out if TM-Core accepts stop request from
  protocol sub-module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_sess_req_stop(tm_prtl_type                       prtl_type,
                                     tm_sess_handle_type                sess_handle,
                                     tm_sess_req_stop_param_s_type      sess_stop_param);


/*===========================================================================

FUNCTION tm_core_recv_data

DESCRIPTION
  This function is to handle data sent from protocol sub-modules.

DEPENDENCIES

RETURN VALUE
  TURE: If TM-Core sub-module accepts it
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_recv_data(tm_prtl_type                     prtl_type,
                                 tm_sess_handle_type              sess_handle,
                                 tm_post_data_e_type              data_type,
                                 const tm_post_data_payload_type  *data_payload_ptr
                                );

/*===========================================================================

FUNCTION tm_core_recv_event

DESCRIPTION
  This function is to handle PD API Events sent from protocol sub-modules.

DEPENDENCIES

RETURN VALUE
  TRUE: If TM-Core sub-module accepts it
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_recv_event( tm_prtl_type         prtl_type,
                                   tm_sess_handle_type  sess_handle,
                                   pdsm_pd_event_type   pd_event,
                                   pdsm_pd_info_s_type* pd_info_ptr,
                                   gnss_NavSlnFlagsStructType *p_PosSrcFlags
                                 );

/*===========================================================================

FUNCTION tm_core_get_active_prtl_module

DESCRIPTION


DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern tm_prtl_type  tm_core_get_active_prtl_module (pdsm_mt_initiator_e_type  mt_initiator);

/*===========================================================================

FUNCTION tm_core_pdcomm_evt_handler

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_pdcomm_evt_handler(uint32 msg_id, void *writeAckData);


/*===========================================================================

FUNCTION tm_core_cm_notify_event

DESCRIPTION
  This function is called by TM-CM module to inform TM-Core phone events

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_cm_notify_event
(
  tm_phone_event_e_type          cm_event_type,
  tm_phone_event_payload_u_type  *evt_payload_ptr
);

/*===========================================================================

FUNCTION tm_core_start_emergency_session

DESCRIPTION
  This function starts a TM_CORE session to handle emergency positioning

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_start_emergency_session(void);

#ifdef FEATURE_CGPS_UMTS_CP_GSM
/*===========================================================================

FUNCTION tm_core_rr_notify_event

DESCRIPTION
  This function is called by TM-RR IFACE to inform TM-Core of RR events

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_rr_notify_event
(
  prtl_rr_event_s_type  *   p_event
);
#endif

/*===========================================================================

FUNCTION tm_core_rrc_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming RRC message.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_rrc_notify_event( prtl_rrc_event_s_type *p_event );


/*===========================================================================

FUNCTION tm_core_mm_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming MM message.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_mm_notify_event( prtl_mm_event_s_type  *p_event );


/*===========================================================================

FUNCTION tm_core_ota_reset_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of either
  an incoming OTA GPS reset OR OTDOA reset event message. 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_ota_reset_notify_event( prtl_event_e_type const prtlEvent );

/*===========================================================================

FUNCTION tm_core_cell_db_proc

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming cell database update message.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_cell_db_proc( void *cell_db_info_ptr );

/*===========================================================================

FUNCTION tm_core_l1_msg_proc

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming message from L1.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

extern void tm_core_l1_notify_event ( prtl_l1_event_s_type *p_event );

/*===========================================================================

FUNCTION tm_core_lte_l1_msg_proc

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming message from LTE L1.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

extern void tm_core_lte_l1_notify_event ( prtl_lte_l1_event_s_type *p_event );



/*===========================================================================

FUNCTION tm_core_get_last_position

DESCRIPTION
  This function is called by TmCore to return a cached position

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_last_position (tm_core_cached_pos_s_type *pd_cached_pos );


#ifdef FEATURE_CGPS_XTRA
/*===========================================================================
FUNCTION tm_core_is_xtra_operation_allowed

DESCRIPTION
Check if assisted mode GPS session are in progress.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_xtra_operation_allowed
(
  void
);

/*===========================================================================
FUNCTION tm_core_handle_xtra_download_req

DESCRIPTION
Handle the XTRA download request recieved from PE via LM

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
void tm_core_handle_xtra_download_req(const sm_ReqXtraStructType *pz_xtraAssistReq);

#ifdef FEATURE_UIM_SUPPORT_LBS
/*===========================================================================

FUNCTION tm_xtra_ruim_init_and_read

DESCRIPTION
  This function reads NV items Asynchronusly to set TM-XTRA configuration
  info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_xtra_ruim_init_and_read(void);

/*===========================================================================

FUNCTION tm_xtra_ruim_read_complete

DESCRIPTION
  This function is called when Async-read for TM_XTRA NV items is complete,
  it sets TM-XTRA configuration info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

extern void tm_xtra_ruim_read_complete(tm_xtra_ruim_nv_buffer* ruim_nv_buffer);

#endif /* FEATURE_UIM_SUPPORT_LBS */

#endif /* FEATURE_CGPS_XTRA */

#ifdef FEATURE_UIM_SUPPORT_LBS
/*===========================================================================

FUNCTION tm_core_ruim_read

DESCRIPTION
  This function read NV items to set TM-Core configuration info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_ruim_read(void);
/*===========================================================================

FUNCTION tm_core_ruim_read_complete

DESCRIPTION
  This function is called  to set TM-Core configuration info structure, after
  Async NV read for NV items is complete

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_ruim_read_complete(tm_core_ruim_read_buffer *tm_core_ruim_buff);
#endif /* FEATURE_UIM_SUPPORT_LBS */


/*===========================================================================

FUNCTION tm_core_get_gps_lock_state

DESCRIPTION
  This function determines if gps is locked

RETURN VALUE
  TRUE if gps is locked, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_gps_lock_state(void);

/*===========================================================================

FUNCTION tm_core_get_on_demand_state

DESCRIPTION
  This function determines if on-dmeand positioning is enabled

RETURN VALUE
  TRUE if on-demand is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_on_demand_state(void);

#ifdef FEATURE_GNSS_PERIODIC_PPM
#error code not present
#endif


/*===========================================================================

FUNCTION tm_core_is_internal_session_on

DESCRIPTION
  This function determines if internal session is ongoing

RETURN VALUE
  TRUE if internal session is running, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_internal_session_on(void);


/*===========================================================================
FUNCTION tm_core_generate_ext_pd_data

DESCRIPTION
  This function generates extended pd data

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_generate_ext_pd_data( const sm_FixInfoStructType *fix_ptr);

/*===========================================================================

FUNCTION tm_core_get_gps_state

DESCRIPTION
  This function is called to get the GPS Receiver state information

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_get_gps_state(
  tm_core_get_gps_state_param_s_type *p_get_gps_state_param
);

/*===========================================================================

FUNCTION tm_core_get_gps_state_para_check

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
pdsm_pd_cmd_err_e_type tm_core_get_gps_state_err_check ( void );


/*===========================================================================

FUNCTION tm_core_get_best_avail_pos_err_check

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
pdsm_pd_cmd_err_e_type tm_core_get_best_avail_pos_err_check ( void );


/*===========================================================================

FUNCTION tm_core_report_gps_state_info

DESCRIPTION
  This function is called by MGP to return the gps state info
  for on-demand positioning and user requested state info

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void  tm_core_report_gps_state_info( void *pos_data);

/*===========================================================================

FUNCTION tm_core_get_call_flow_select_mask

DESCRIPTION
  This function returns the UMTS CP MOLR vs AD call flow select mask in TM data

RETURN VALUE
  uint8 call flow select mask

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
uint8 tm_core_get_call_flow_select_mask(void);

/*===========================================================================

FUNCTION tm_core_xlate_pos_to_ext_status

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_xlate_pos_to_ext_status
(
  pdsm_pd_ext_pos_s_type  *dest_buf_ptr,
  sm_FixInfoStructType *source_buf_ptr,
  uint32                  remaing_fix_time
);

/*===========================================================================

FUNCTION tm_core_xlate_best_avail_pos_to_ext_status

DESCRIPTION convert a BestAvailPos fix report to pd_ext_status report

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_xlate_best_avail_pos_to_ext_status
(
  pdsm_pd_ext_pos_s_type    *dest_buf_ptr,
  sm_FixInfoStructType   *source_buf_ptr,
  uint32                    remaing_fix_time
);

/*===========================================================================

FUNCTION tm_core_get_utc_offset

DESCRIPTION
  This function returns the UTC offset obtained from MGP. If UTC offset is invalid, then the UTC offset will contain
  default value from MGP.

RETURN VALUE
  Boolean: TRUE: If UTC offset is valid
                FALSE: If UTC offset is invalid

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean tm_core_get_utc_offset(uint8 *p_utc_offset);

/*===========================================================================

FUNCTION tm_core_save_ref_position

DESCRIPTION
  This function saves the reference position received from the network so
   it can be reported at the next time tick

RETURN VALUE
  none

DEPENDENCIES
  None

SIDE EFFECTS

==========================================================================*/
void tm_core_save_ref_position( pdsm_ext_status_info_s_type   *pd_ext_status_info);
/*===========================================================================

FUNCTION tm_core_get_efs_supl_cert_status

DESCRIPTION
  This function tells the caller whether it must use SUPL Cert from EFS or not.

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_efs_supl_cert_status( void);
/*===========================================================================

FUNCTION tm_core_set_efs_supl_cert_status

DESCRIPTION
 IF tm_use_efs_for_supl_cert == TRUE, supl session must read supl cert from EFS.

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_set_efs_supl_cert_status(boolean new_efs_cert_status );


/*===========================================================================

FUNCTION tm_on_demand_get_punc_tunc

DESCRIPTION
  This function returns the PUNC tunc values

RETURN VALUE

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_on_demand_get_punc_tunc(FLT *p_punc, FLT *p_tunc);


/*===========================================================================

FUNCTION tm_core_get_xtra_validity_age

DESCRIPTION
  This function returns the NV item for XTRA data validity age

RETURN VALUE

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_get_xtra_validity_age(uint16 *p_age);

/*===========================================================================

FUNCTION tm_core_set_xtra_validity_age

DESCRIPTION
  This function sets the NV item for XTRA data validity age

RETURN VALUE

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_set_xtra_validity_age(uint16 w_age);

/*====================================================================
FUNCTION tm_core_set_xtra_uim_subscription

DESCRIPTION

  This function sets the UIM subscription data maintained by XTRA

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  none

=====================================================================*/
void tm_core_set_xtra_uim_subscription(const void* p_info);
/*====================================================================
FUNCTION tm_core_set_xtra_srv_sys_info
DESCRIPTION
  This function sets PLMN info for the provided srv system information
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
  none
=====================================================================*/
void tm_core_set_xtra_srv_sys_info(const tm_cm_mm_ss_info_s_type * const ss_info);

/*===========================================================================

FUNCTION tm_core_check_mt_up_allowed

DESCRIPTION
  Checks whether a mobile terminated User plane
  session can be allowed or not based on the service set as Designated Data
  by the user. E.g. MT V1/V2 will only be allowed if designated data is 1x.
  Similarly MT SUPL will only be allowed if designated data is GSM/W.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_check_mt_up_allowed
(
  tm_prtl_type  prtl_type
);

/*===========================================================================

FUNCTION tm_core_must_use_supl_cert_frm_efs

DESCRIPTION
  This function tells the caller whether it must use SUPL Cert from EFS or not.

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_must_use_supl_cert_from_efs( void);
/*===========================================================================

FUNCTION tm_core_get_current_dd_subs_id

DESCRIPTION
  This function returns the ID for current DD subscription

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
sys_modem_as_id_e_type tm_core_get_current_dd_subs_id( void);
/*===========================================================================

FUNCTION tm_core_set_current_dd_subs_id

DESCRIPTION
  This function updates the stored value for current DD subscription

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_set_current_dd_subs_id(sys_modem_as_id_e_type asid, boolean reConfig);
/*===========================================================================

FUNCTION tm_core_set_efs_supl_cert_status

DESCRIPTION
 IF tm_use_efs_for_supl_cert == TRUE, supl session must read supl cert from EFS.

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_set_efs_supl_cert_status(boolean new_efs_cert_status );

/*====================================================================
FUNCTION tm_mmgsdi_set_session_id

DESCRIPTION

  This function updates the tm mmgsdi array with session id info for
  the given session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_session_id
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type,
   mmgsdi_session_id_type session_id
);

/*====================================================================
FUNCTION tm_mmgsdi_get_session_id_status

DESCRIPTION

  This function informs whether the session id for a given
  mmgsdi session type has been set or not.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : Session ID was programmed earlier
  FALSE : Session ID not programmed.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_session_id_status
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type
);

/*====================================================================
FUNCTION tm_mmgsdi_close_session_id

DESCRIPTION

  This function close a particular session id and reset its status

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : Session ID was closed successfully
  FALSE : Session ID not found

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_close_session_id
(
   mmgsdi_session_id_type session_id
);

/*====================================================================
FUNCTION tm_mmgsdi_get_session_id_ptr

DESCRIPTION

  This function returns a pointer to the session ID for the specified
  session type.  At most one session ID is associated with each session
  type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : If session id was found
  FALSE : Session id for this session type not found.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_session_id_ptr
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type,
   mmgsdi_session_id_type     ** tm_mmgsdi_session_id_ptr
);
/*====================================================================
FUNCTION tm_mmgsdi_set_subscription_id

DESCRIPTION
  This function updates the tm mmgsdi session info table with the
  subscription id assigned to a given session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_subscription_id
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type,
   sys_modem_as_id_e_type        asubs_id
);

/*====================================================================
FUNCTION tm_mmgsdi_get_subscription_id

DESCRIPTION
  This function gets the subscription id assigned to a given session ID
  from tm mmgsdi session info table.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : subscription id had been found and returned
  FALSE : failed to find subscription id for a particular session id.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_subscription_id
(
   mmgsdi_session_id_type     session_id,
   sys_modem_as_id_e_type     *asubs_id
);

/*====================================================================
FUNCTION tm_mmgsdi_set_app_type

DESCRIPTION
  This function updates the tm mmgsdi session info table with the
  app type for a given session id.


DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_app_type
(
   mmgsdi_session_id_type session_id,
   mmgsdi_app_enum_type         app_type
);

/*====================================================================
FUNCTION tm_mmgsdi_get_app_type_from_sub_id

DESCRIPTION

  This function returns the app type for the active session corresponding to
  the sub id. If the sub id matches but the session is not active, it returns
  MMGSDI_APP_NONE.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
mmgsdi_app_enum_type tm_mmgsdi_get_app_type_from_asid
(
  sys_modem_as_id_e_type asubs_id
);

/*====================================================================
FUNCTION tm_mmgsdi_set_slot_id

DESCRIPTION

  This function updates the tm mmgsdi session info table with the
  slot id for a given session id.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_slot_id
(
  mmgsdi_session_id_type session_id,
  mmgsdi_slot_id_enum_type slot_id
);

/*====================================================================
FUNCTION tm_mmgsdi_get_slot_id_from_sub_id

DESCRIPTION

  This function returns the slot id for the session corresponding to the
  sub_id. If the sub id matches but the session is not active, it returns
  MMGSDI_SLOT_NONE.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
mmgsdi_slot_id_enum_type tm_mmgsdi_get_slot_id_from_asid
(
  sys_modem_as_id_e_type     asubs_id
);

/*====================================================================
FUNCTION tm_mmgsdi_get_sim_info_from_asid

DESCRIPTION

  This function returns a pointer to the session ID and a pointer to
  the application type. for the specified
  subscription ID.  At most one session ID is associated with each session
  type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : session id had been set
  FALSE : Session id had not been set.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_sim_info_from_asid
(
   sys_modem_as_id_e_type      asubs_id,
   mmgsdi_session_id_type     *tm_mmgsdi_session_id_ptr,
  mmgsdi_app_enum_type *app_type_ptr,
  mmgsdi_app_enum_type app_type
);

/*===========================================================================

FUNCTION tm_core_get_xtra_validity_age

DESCRIPTION
  This function returns the NV item for XTRA data validity age

RETURN VALUE

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_get_xtra_validity_age(uint16 *p_age);

/*====================================================================
FUNCTION tm_core_time_validity

DESCRIPTION
   Reports whether the time from time_get_uptime_ms is valid or not

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tm_core_time_validity(void);

/*===========================================================================

FUNCTION tm_agps_emergency_call_simulate

DESCRIPTION
   Reports whether emergency call simulation is enabled or not

DEPENDENCIES

RETURN VALUE
  boolean: TRUE - emergency call simulation enabled
           FALSE - emergency call simulation disabled

SIDE EFFECTS
  None

===========================================================================*/
boolean tm_agps_emergency_call_simulate(void);

/*===========================================================================
FUNCTION tm_core_clear_cached_meas

DESCRIPTION
Function to clean up cached measurement report. Measurement is cached to
generate NMEA sentences.

DEPENDENCIES
NONE
INPUT PARAMS
NONE
RETURN VALUE
NONE

SIDE EFFECTS

===========================================================================*/
void tm_core_clear_cached_meas(void);

/*===========================================================================
FUNCTION tm_core_clear_cached_pos

DESCRIPTION Clears the RAM copy of the position

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_clear_cached_pos(void);

/*===========================================================================
FUNCTION tm_core_clear_cached_ref_pos

DESCRIPTION Clears the RAM copy of the ref position

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tm_core_clear_cached_ref_pos(void);

/*===========================================================================
FUNCTION tm_core_is_standalone_active

DESCRIPTION
Check if Standalone session is in progress.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_standalone_active(void);

/*===========================================================================

FUNCTION tm_core_translate_end_status

DESCRIPTION
  This function translate internal abort reason to external end status.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
pdsm_pd_end_e_type tm_core_translate_end_status
(
  tm_sess_stop_reason_e_type  abort_reason
);

/*===========================================================================

FUNCTION tm_core_gm_evt_handler

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_gm_evt_handler(uint32 msg_id, void *p_Data);

/*===========================================================================
FUNCTION tm_core_get_lte_lpp_up_enable

DESCRIPTION
  This function determines if LPP protocol is enabled in NV



RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_lte_lpp_up_enable(void);

/*===========================================================================
FUNCTION tm_core_get_nr_lpp_up_enable

DESCRIPTION
  This function determines if LPP protocol is enabled in NV over NR



RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_nr_lpp_up_enable(void);

/*===========================================================================

FUNCTION tm_core_get_nr_lpp_cp_enable

DESCRIPTION
  This function determines if LPP protocol on NR is enabled in NV 

 

RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_nr_lpp_cp_enable(void);

/*===========================================================================

FUNCTION tm_core_lppe_up_precondition

DESCRIPTION
  This function decides if E911 LTEWLAN call is on and LPPe is enabled 

RETURN VALUE
  boolean  True - E911 LTE/WLAN call on , otherwise FALSE.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_lppe_up_precondition( void );

/*===========================================================================

FUNCTION tm_core_lppe_up_dbh_enabled

DESCRIPTION
  This function checks if LPPE NV for DBH is enabled for UP

RETURN VALUE
  boolean - TRUE if DBH is allowed, FALSE otherwise

DEPENDENCIES
  tm_core_lppe_up_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_up_dbh_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_up_config);

  /* check if we are capable of DBH Positioning */
  return ((q_ConfigInfo & PDSM_PA_LPPE_DBH_ENABLE) != 0);
}

/*===========================================================================

FUNCTION tm_core_lppe_up_wlanap_enabled

DESCRIPTION
  This function returns the WLANAP NV setting of LPPe protocol on LTE  in UP

RETURN VALUE
  boolean  True - if WLANAP is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_up_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_up_wlanap_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_up_config);
   return ((q_ConfigInfo & PDSM_PA_LPPE_WLAN_ENABLE)!= 0);
}

/*===========================================================================

FUNCTION tm_core_lppe_up_srn_btle_enabled

DESCRIPTION
  This function returns the SRN BTLE NV setting of LPPe protocol on LTE  in UP

RETURN VALUE
  boolean  True - if SRN BTLE is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_up_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_up_srn_btle_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_up_config);
   return ( ( q_ConfigInfo & PDSM_PA_LPPE_SRN_BTLE_ENABLE) != 0 );
}

/*===========================================================================

FUNCTION tm_core_lppe_up_sensor_baro_enabled

DESCRIPTION
  This function returns the Sensor Barometer NV setting of LPPe protocol on LTE
  in CP

RETURN VALUE
  boolean  True - if Sensor Barometer is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_up_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_up_sensor_baro_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_up_config);
   return ( ( q_ConfigInfo & PDSM_PA_LPPE_UBP_ENABLE ) != 0 );
}

/*===========================================================================

FUNCTION tm_core_lppe_up_enabled

DESCRIPTION

RETURN VALUE
  boolean  True - if WLANAP is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_up_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_up_enabled( void )
{
  uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_up_config);

  /* LPPE is only enabled during E911 & in addition to main LPPE bit set any other technology
  *  (like DBH, WLAN-AP) should be enabled.
  */
  return( ( ( q_ConfigInfo & PDSM_PA_LPPE_ENABLE) != 0 ) && 
            ( tm_core_lppe_up_wlanap_enabled() || tm_core_lppe_up_dbh_enabled() ||
              tm_core_lppe_up_srn_btle_enabled() || tm_core_lppe_up_sensor_baro_enabled() ) );
}

/*===========================================================================

FUNCTION tm_core_lppe_cp_precondition

DESCRIPTION
  This function decides if E911 LTE call is on and LPPe is enabled

RETURN VALUE
  boolean  True - E911 LTE call on , otherwise FALSE.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_lppe_cp_precondition( void );

/*===========================================================================

FUNCTION tm_core_lppe_cp_dbh_enabled

DESCRIPTION
  This function decides if LPPe on LPP CP is enabled correctly to advertise
  LPPe DBH support to server.

RETURN VALUE
  boolean  True - LPPe DBH on LPP CP is enabled, otherwise FALSE.

DEPENDENCIES
  tm_core_lppe_cp_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_cp_dbh_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_cp_config);

  /* check if we are capable of DBH. */
  return ((q_ConfigInfo & PDSM_PA_LPPE_DBH_ENABLE) != 0);
}

/*===========================================================================

FUNCTION tm_core_lppe_cp_wlanap_enabled

DESCRIPTION
  This function returns the WLANAP NV setting of LPPe protocol on LTE  in UP

RETURN VALUE
  boolean  True - if WLANAP is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_cp_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_cp_wlanap_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_cp_config);
   return ((q_ConfigInfo & PDSM_PA_LPPE_WLAN_ENABLE)!= 0);
}

/*===========================================================================

FUNCTION tm_core_lppe_cp_srn_btle_enabled

DESCRIPTION
  This function returns the SRN BTLE NV setting of LPPe protocol on LTE  in CP

RETURN VALUE
  boolean  True - if SRN BTLE is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_cp_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_cp_srn_btle_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_cp_config);
   return ( ( q_ConfigInfo & PDSM_PA_LPPE_SRN_BTLE_ENABLE) != 0 );
}

/*===========================================================================

FUNCTION tm_core_lppe_cp_sensor_baro_enabled

DESCRIPTION
  This function returns the Sensor Barometer NV setting of LPPe protocol on LTE
  in CP

RETURN VALUE
  boolean  True - if Sensor Barometer is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_cp_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_cp_sensor_baro_enabled( void )
{
   uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_cp_config);
   return ( ( q_ConfigInfo & PDSM_PA_LPPE_UBP_ENABLE ) != 0 );
}

/*===========================================================================

FUNCTION tm_core_lppe_cp_enabled

DESCRIPTION
  This function decides if LPPe on LPP CP is enabled correctly to advertise LPPe support to the
  outside world.

RETURN VALUE
  boolean  True - LPPe on LPP CP is enabled, otherwise FALSE.

DEPENDENCIES
 tm_core_lppe_cp_precondition() should be called before calling this function

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_cp_enabled( void )
{
  uint32 const q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_cp_config);

  /* LPPE is only enabled during E911 & in addition to main LPPE bit set any other technology
  *  (like DBH, WLAN-AP) should be enabled.
  */
  return( ( ( q_ConfigInfo & PDSM_PA_LPPE_ENABLE) != 0) &&
            ( tm_core_lppe_cp_wlanap_enabled() || tm_core_lppe_cp_dbh_enabled() ||
              tm_core_lppe_cp_srn_btle_enabled () || tm_core_lppe_cp_sensor_baro_enabled() ) );
}

/*===========================================================================

FUNCTION tm_core_lppe_dbh_enabled

DESCRIPTION
  This function checks if GTP WWAN or GTP WIFI, LPPE and LPPE DBH is
  enabled for LPP UP or CP.

RETURN VALUE
  boolean - TRUE if DBH is allowed on LPP CP or UP, FALSE otherwise

DEPENDENCIES
  None.

SIDE EFFECTS

===========================================================================*/
GNSS_INLINE boolean tm_core_lppe_dbh_enabled( void )
{
  return ( (tm_core_lppe_up_precondition() && tm_core_lppe_up_dbh_enabled()) ||
           (tm_core_lppe_cp_precondition() && tm_core_lppe_cp_dbh_enabled()) );
}
/*===========================================================================
FUNCTION tm_core_get_asst_glo_lpp_up_enable

DESCRIPTION
  This function determines the positioning protocol for Assisted GLONASS U-plane
  is LPP.

RETURN VALUE
  TRUE if LPP is selected, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_asst_glo_lpp_up_enable(void);

/*===========================================================================
FUNCTION tm_core_get_asst_glo_lpp_cp_enable

DESCRIPTION
  This function determines if Assisted GLONASS is enabled for U-plane LPP 
  protocol.

RETURN VALUE
  TRUE if LPP is selected, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_asst_glo_lpp_cp_enable(void);

/*===========================================================================

FUNCTION tm_core_otdoa_evt_handler

DESCRIPTION
  This function receives OTDOA measurements from the OTDOA interface and sends
  them to the appropriate protocol


RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_otdoa_evt_handler(sm_loc_lte_otdoa_meas_info_type *p_otdoa_measurement_info);


#ifdef FEATURE_WCDMA_GPS_CELL_ID_READING
/*===========================================================================

FUNCTION tm_core_rrc_cell_info_dch_report_handler

DESCRIPTION
  This function handles the rrc cell info DCH report.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_rrc_cell_info_dch_report_handler(tm_rrc_cell_info_dch_report_type* rrc_cell_info_p);
#endif /*FEATURE_WCDMA_GPS_CELL_ID_READING */

/*===========================================================================

FUNCTION tm_core_reset_location_service_done_handler

DESCRIPTION
  This function handles the reset locaiton service done indication

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_reset_location_service_done_handler(pdsm_pd_ext_reset_location_service_status_e_type* status);

/*===========================================================================
FUNCTION tm_core_is_agps_allowed_on_serv_sys

DESCRIPTION
Check if AGPS is allowed on a serving system

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_agps_allowed_on_serv_sys(cgps_SrvSystemType serv_sys);

/*===========================================================================
FUNCTION tm_core_is_umts_prefered

DESCRIPTION
Check if UMTS/LTE is prefered over other RAT's

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_umts_prefered(void);


/*===========================================================================
FUNCTION tm_core_is_ni_standalone

DESCRIPTION
  This function checks if this is an 1x UP NI triggered session which has fallen
  back to standalone

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_is_ni_standalone(void);

/*===========================================================================
FUNCTION tm_core_reset_location_service_in_progress

DESCRIPTION
  This function checks if reset location service is in progress

DEPENDENCIES

RETURN VALUE
  TRUE/FALSE: Whether reset location service is in progress.

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_is_reset_location_service_in_progress(void);

/*===========================================================================
FUNCTION tm_core_reset_location_service_in_progress

DESCRIPTION
  This function sets whether reset location service is in progress

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_set_reset_location_service_in_progress(boolean reset_location_service_in_progress);

/*===========================================================================
FUNCTION tm_core_handle_engine_error_recovery_report

DESCRIPTION
  This function handles engine error recovery report (usually it'll be sent to
  QMI)

DEPENDENCIES

RETURN VALUE
  TRUE if success

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_engine_error_recovery_report(gnss_engine_err_recovery_rpt *pErrReport);

/*===========================================================================
FUNCTION tm_core_handle_engine_dpo_status_report

DESCRIPTION
  This function handles engine DPO status report (usually it'll be sent to
  QMI)

DEPENDENCIES

SIDE EFFECTS

===========================================================================*/
extern void tm_core_handle_engine_dpo_status_report(sm_DpoStatusReportStructType *p_DpoStatusReport);

/*===========================================================================
FUNCTION tm_core_handle_best_avail_pos

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_best_avail_pos
(
  tm_core_get_best_avail_pos_param_s_type *p_get_best_avail_pos_param
);

/*===========================================================================
FUNCTION tm_core_handle_avail_wwan_pos

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_avail_wwan_pos
(
  tm_core_get_avail_wwan_pos_param_s_type *p_GetAvailWwanPosParam
);

/*===========================================================================
FUNCTION tm_core_handle_available_pos

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_available_pos
(
  tm_core_get_available_pos_param_s_type *p_GetAvailablePosParam
);

/*===========================================================================
FUNCTION tm_core_get_lpp_cp_enable

DESCRIPTION
  This function determines if LPP Control Plane protocol is enabled in NV



RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_get_lpp_cp_enable(void);

/*===========================================================================
FUNCTION tm_core_enable_DpoStatusReport

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE is DPO Status Report was enabled

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_enable_DpoStatusReport(boolean enable);

/*===========================================================================

FUNCTION tm_core_api_callback_handler

DESCRIPTION
  This function handles calling the client callbacks.
  It also starts a non-deferred timer that will fire even when the processor is sleeping.
  The timer runs for 10 sec and firing off every 1 sec.

DEPENDENCIES

RETURN VALUE TRUE/FALSE

SIDE EFFECTS

===========================================================================*/

boolean tm_core_api_callback_handler(const tm_api_notifier_type *p_api_notifier);

/*===========================================================================
FUNCTION tm_task_init_notifier_cb

DESCRIPTION
Callback function for task init notification

RETURN VALUE

DEPENDENCIES
=============================================================================*/
void tm_task_init_notifier_cb(uint32 thread_id);

/*===========================================================================
FUNCTION tm_core_is_supl_ni_proc

DESCRIPTION
  This function checks if this is an SUPL NI trigger should be reported to AP always
  
DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_is_supl_ni_proc(void);

/*===========================================================================
FUNCTION tm_core_mtlr_auto_accept

DESCRIPTION
  This function checks if this is an MTLR should be accepted without AP notification
  during emergency

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_mtlr_auto_accept( void );

/*===========================================================================
FUNCTION tm_core_lpp_cp_evt_handler

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming LPP Control Plane Protocol events.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_lpp_cp_evt_handler( uint32 msg_id, void *lpp_cp_evt_info_ptr );

/*===========================================================================

FUNCTION tm_core_external_cell_db_proc

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming external cell database update message.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

boolean tm_core_external_cell_db_proc( pdsm_cell_info_s_type *external_cell_db_info_ptr );

/*===========================================================================

FUNCTION tm_core_ecid_evt_handler

DESCRIPTION
  This function receives ECID measurements from the ECID interface and sends
  them to the appropriate protocol


RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_ecid_evt_handler(gnss_lpp_ecid_meas_info_type *p_ecid_measurement_info);

/*===========================================================================
FUNCTION tm_core_set_ni_resp_pending

DESCRIPTION
  This function checks if this is an SUPL NI trigger should be reported to AP always

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern void tm_core_set_ni_resp_pending( boolean b_pending );

/*===========================================================================
FUNCTION tm_core_is_ni_resp_pending

DESCRIPTION
  This function checks if this is an SUPL NI trigger should be reported to AP always

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_is_ni_resp_pending( void );


/*===========================================================================
FUNCTION tm_core_retrieve_imsi

DESCRIPTION
  This function is called by TLE to request for IMSI retrieval

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_retrieve_imsi( tm_RetrieveIMSIType * tm_retrieve_imsi_info_ptr );

/*===========================================================================

FUNCTION tm_copy_prm_to_sm_meas_struct

DESCRIPTION
  This function copies the ME provided PRM data to SM GNSS Measurement report struct.

DEPENDENCIES

RETURN VALUE
  Return GPS or GLONASS measurement index upon success, else return invalid if
  PRM data isn't of GPS or GLONASS type.
SIDE EFFECTS

===========================================================================*/
int32 tm_copy_prm_to_sm_meas_struct(const sm_GpsMeasRptStructType *const p_prm_data,
                                    SmGnssMeasReportStructType *p_gnss_meas_report,
                                    boolean const cv_StatusCheckNeeded);

/*===========================================================================

FUNCTION tm_core_get_gm_engine_config

DESCRIPTION
  This function returns the GM Engine configuration

RETURN VALUE
  The timeout value in secs.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_get_gm_engine_config( geofence_engine_config_s_type **gm_config );

/*===========================================================================

FUNCTION tm_core_is_wiper_running

DESCRIPTION
 Function to get whether Wiper is running

DEPENDENCIES

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean tm_core_is_wiper_running( void );

/*===========================================================================

FUNCTION tm_util_map_and_inject_glo_steering_to_tmcore

DESCRIPTION
  Inject Glonass Acquisition Assistance data - Steering - into GNSS engine,
  when deltaT/leap second (from GPS UTCModel or Glonass TimeModel AD), Raw AcqAssist
  (a.k.a Reference Measurement Info) & Glo AuxiInfo is available.

DEPENDENCIES
  Call tm_util_map_glo_acqast_svid_to_freq_num() to associate SvId to FreqNum
  before invoking this function.

RETURN VALUE
  Return when AA Steering isn't injected into GNSS engine.

SIDE EFFECTS
  None

===========================================================================*/
boolean tm_util_map_and_inject_glo_steering_to_tmcore( prot_glo_acqast_buffer_s_type *const p_glo_acqasst,
                                                       prot_glo_acqasst_sat_buffer_s_type const *const p_glo_acqasst_subset,
                                                       prot_glo_svcn_buffer_s_type const *const p_glo_auxi_info,
                                                       gnss_time_info const *const p_gnss_time,
                                                       tm_prtl_type prtl_type,
                                                       tm_sess_handle_type sess_handle,
                                                       tm_post_data_payload_type *data_payload_ptr );

/*===========================================================================

FUNCTION tm_util_map_glo_acqast_svid_to_freq_num

DESCRIPTION
 Map Glonass Acquisition Assistance SvId to frequency number using the information
 in stored SV/ChannelNumber buffer structure.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tm_util_map_glo_acqast_svid_to_freq_num( prot_glo_acqast_buffer_s_type *const p_GloAcqAsstBuf,
                                              prot_glo_svcn_buffer_s_type const *const p_GloSvCnMapBuf );

/*===========================================================================

FUNCTION tm_util_translate_glo_intcodephase

DESCRIPTION
 Compute the Glonass milli-second of the week from interger code phase &
 Glonass Time Of Day in milli-seconds.

DEPENDENCIES

RETURN VALUE
  GNSS milli-second of the week

SIDE EFFECTS
  None

===========================================================================*/
uint32 tm_util_translate_glo_intcodephase( uint32 const q_integer_code_phase, uint32 const q_GloMsecInDay );

/*===========================================================================

FUNCTION tm_util_cp_msa_gnss_mandatory_ad_needed

DESCRIPTION
 Return the mandatory GNSS assistance data needed from network/LS. The returned
 list is used by protocol module to request missing AD from network/LS.
 Use this function only in A-GNSS call flow.

DEPENDENCIES

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
void tm_util_cp_msa_gnss_mandatory_ad_needed( uint32 *p_GpsAdNeeded, uint32 *p_GloAdNeeded, uint32 const q_GpsAdDelivered, uint32 const q_GloAdDelivered );

/*===========================================================================

FUNCTION tm_util_cp_msa_gps_mandatory_ad_needed

DESCRIPTION
 Return the mandatory GPS assistance data needed from network/LS. The returned
 list is used by protocol module to request missing GPS AD from network/LS.
 Use this function only in A-GPS only call flow.

DEPENDENCIES

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
void tm_util_cp_msa_gps_mandatory_ad_needed( uint32 *p_GpsAdNeeded, uint32 const q_GpsAdDelivered );

/*===========================================================================
FUNCTION tm_core_get_lpp_periodic_end_flag

DESCRIPTION
  This function checks if LPP periodic session should end after emergency call endss
  
DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_get_lpp_periodic_end_flag (void);

/********************************************************************
*
* tm_get_imei
*
* Function description:
*  Gets the IMEI value from the GHSDI
*
* Parameters: 
*  p_imei_ptr - The retrieved IMEI to return to the caller (output)
*
* Return:
*  TRUE/FALSE
*  
*********************************************************************
*/  
boolean tm_get_imei(byte *p_imei_ptr);

/********************************************************************
*
* tm_handle_ota_reset
*
* Function description:
*  Handles the OTA GPS reset and OTDOA reset messages
*
* Parameters: 
*   prtlEvent: Protocol Event type
*   e_as_id  : Corresponding Subscription ID.
*
* Return:
*  none
*  
*********************************************************************
*/  
void tm_handle_ota_reset(prtl_event_e_type const prtlEvent, sys_modem_as_id_e_type const e_as_id);

/********************************************************************
* tm_core_handle_wifi_attachment_status_ind
*
* Function description:
* Handles Wifi attachment status indication info
*
* Parameters: 
*  p_wifi_attachment_status_ind_info - Pointer to Wifi attachment status inidcation info stuct
*
* Return:
* void
*  
*********************************************************************
*/  
void tm_core_handle_wifi_attachment_status_ind(pdsm_wifi_attachment_status_ind_info *p_wifi_attachment_status_ind_info);

/*===========================================================================
FUNCTION tm_core_get_end_supl_after_final_fix_nv

DESCRIPTION
  This function returns cached NV value if SUPL can be terminated after final fix is sent

DEPENDENCIES

RETURN VALUE
  TRUE if NV is enabled, FALSE otherwise.

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_get_end_supl_after_final_fix_nv (void);

/*===========================================================================
FUNCTION tm_core_lpp_grt_24_admeas_enabled

DESCRIPTION
  Return if the LPP greater than 24 AD & RSTD measurement processing is
  supported by UE.

RETURN VALUE
  TRUE if LPP > 24 AD feature is supported LPP, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_lpp_grt_24_admeas_enabled(void);

/********************************************************************
*
* tm_core_handle_wifi_enabled_status_ind
*
* Function description:
* Handles Wifi enabled status indication info
*
* Parameters: 
*  e_wifi_enabled_status - Status of wifi enabled.
*
* Return:
* void
*  
*********************************************************************
*/  
void tm_core_handle_wifi_enabled_status_ind(pdsm_wifi_enabled_status_ind_type e_wifi_enabled_status);

/*===========================================================================
FUNCTION tm_core_handle_wifi_fix_response

DESCRIPTION
  This function handles Wifi fix Response

DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
extern void tm_core_handle_wifi_fix_response(tm_wifi_fix_response_type *pz_wifi_fix_resp);

/*===========================================================================


DESCRIPTION
  This function sends session done idnication to PE for PE


RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_send_sess_done_ind_for_lppm(void);

/*===========================================================================

FUNCTION tm_core_refresh_carrier_specific_nv

DESCRIPTION
  This function refresh carrier-specific NVs for a specific subscription


RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_refresh_carrier_specific_nv(void* p_data);

/*===========================================================================
  tm_core_query_for_lppm_status

  Description:

     This function is used to query for Lppm status

 Parameters:
    Session Power Mode
    
  Return value: 
    void

=============================================================================*/
boolean tm_core_query_for_lppm_status(tm_sess_power_mode_s_type z_PowerMode);

/*===========================================================================
  tm_core_modify_lppm

  Description:

     This function is used to modify LPPM (Power Mode and/or TBM)

 Parameters:
    LPPM Power Mode Config
    
  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_modify_lppm(tm_sess_power_mode_s_type z_PowerMode);

/*===========================================================================
  tm_core_lppm_start

  Description:

     This function is put engine in LPPM mode.
     Should be called only when engine is not in LPPM mode.

 Parameters:
    LPPM Power Mode Config

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_lppm_start(tm_sess_power_mode_s_type z_PowerMode);

/*===========================================================================
  tm_core_lppm_stop

  Description:

     This function is put engine out of LPPM mode

 Parameters:
    void

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_lppm_stop(void);

/*===========================================================================
  tm_core_handle_start_cpi

  Description:

     This function handles start CPI request.
     The only called of this function shall be the Tech Selector.

 Parameters:
    v_isEmergency: Is Emergency CPI

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_handle_start_cpi(boolean v_isEmergency);


/*===========================================================================
  tm_core_handle_stop_cpi

  Description:
  
     This function handles stop CPI request.
     The only called of this function shall be the Tech Selector.

 Parameters:
    void

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_handle_stop_cpi(boolean v_isEmergency);

#ifdef FEATURE_GNSS_GERA_ENABLE
#error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

/*===========================================================================
FUNCTION  tm_E911LTEOn

DESCRIPTION
  Checks whether it is an E911 call and LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE if it is both E911 and LTE.
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tm_E911LTEOn();

/*===========================================================================
FUNCTION  tm_E911WLANOn

DESCRIPTION
  Checks whether it is an E911 call and WLAN.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE if it is both E911 and LTE.
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tm_E911WLANOn();

/*===========================================================================
  tm_core_report_sess_start_to_techsel

  Description:

     This function reports session start to TechSel

 Parameters:
    tm_sess_req_start_param_s_type* : Pointer to session start request param
    tm_sess_handle_type: Active Session Handle
    
  Return value: None

=============================================================================*/
void tm_core_report_sess_start_to_techsel(const tm_sess_req_start_param_s_type *pz_sess_start_param,
                                          const tm_sess_handle_type q_sess_handle);

/*===========================================================================
  tm_core_report_fix_start_to_techsel

  Description:

     This function reports fix start to TechSel

  Parameters:

  Return value: None

=============================================================================*/
void tm_core_report_fix_start_to_techsel(void);

/*===========================================================================
tm_core_report_911_state_to_techsel

Description:

This function reports start of E911 to TechSel so that it can terminate LPPM

Parameters:

Return value: None

=============================================================================*/
void tm_core_report_911_state_to_techsel(void);

/*===========================================================================

FUNCTION tm_core_cache_leap_second_info

DESCRIPTION
  This function caches the Leap Second Info into TM, sent from ME

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_cache_leap_second_info(mgp_LeapSecondInfoStructType* pz_LeapSecInfo);

/*===========================================================================
FUNCTION tm_core_lpp_earlytimer_enabled

DESCRIPTION
  Return if the LPP Rel12 2Timer feature is enabled. If enabled the Early
  Response timer in Qos is processed, else it is dropped. By default the
  feature is disabled.

PARAMETER
  tm_core_config_2timer_support_enum_type e_2timer_support : used to 
    distinguish which bit to read in the u_gnss_lpp_rel12_2timer_support field.

RETURN VALUE
  TRUE  :If LPP Rel12 2Timer feature is enabled.
  FALSE :If disabled

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_lpp_earlytimer_enabled(
          tm_core_config_2timer_support_enum_type e_2timer_support);

/*===========================================================================
FUNCTION tm_core_inform_prtl_nv_op

DESCRIPTION
  Update protocol about NV READ operation.

RETURN VALUE
  None
DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_inform_prtl_nv_op(prtl_ruim_nv_read_group* u_NvGroup);
   
/*===========================================================================

FUNCTION tm_core_handle_sensor_info

DESCRIPTION
  This function forwards the SLIM Sensor message to TM AUX Tech.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_handle_sensor_info(void* p_SlimMsg);

/*===========================================================================

FUNCTION tm_core_nmea_debug_info

DESCRIPTION
  This function processes NMEA debug string in TM And forwards
  it to HLOS for debugging purpose. Should not be mixed with NMEA strings
  generated with Measurement reports.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_nmea_debug_info(sm_DebugNMEA* pz_DebugNMEACustom);

/*===========================================================================

FUNCTION tm_core_is_e911_concurrency_allowed

DESCRIPTION
  This function calls MGP API to decide if an E911 session can be started in 
  concurrent mode.

PARAMETERS
  e_incoming_client: New incoming client for which concurrency check is done to get into concurrent mode
  v_is_msa_session: TRUE if concurrency check is done on an ongoing MSA session. 
                              FALSE if ongoing is a non-MSA session

RETURN VALUE
  boolean

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_e911_concurrency_allowed(ldm_client_id_e_type e_incoming_client, boolean v_is_msa_session);


/*===========================================================================

FUNCTION tm_core_update_e911_concurrency_to_lm

DESCRIPTION
  This function updates LM on the current state of E911 concurrent session

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_update_e911_concurrency_to_lm(boolean v_e911_concurrency_ongoing);

/*===========================================================================

FUNCTION tm_core_xlate_sv_state

DESCRIPTION
Translates the SV state from the measurement report to PDAPI state 

DEPENDENCIES 
  None

RETURN VALUE 
PDSM state 

SIDE EFFECTS

===========================================================================*/
pdsm_LocSvSearchStatusEnumT tm_core_xlate_sv_state(gnss_SvStateEnumType in_sv_state);

/*===========================================================================

FUNCTION tm_core_convert_op_mode

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
lm_gps_mode_e_type tm_core_convert_op_mode(tm_sess_operation_mode_e_type op_mode,
                                           tm_prtl_type                  prtl_type);

/*===========================================================================

FUNCTION tm_core_get_fusion_csm_engine_config

DESCRIPTION
  This function returns the Fusion CSM configuration

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_get_fusion_csm_config(fusion_csm_config_s_type *fusion_csm_config);

/*===========================================================================

FUNCTION tm_core_update_fusion_cs_mode

DESCRIPTION
  This function updates the fusion cs mode

INPUT PARAMS
  b_OnDemandActive: Indicates whether on demand is active

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_update_fusion_cs_mode(boolean b_OnDemandActive);

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/*===========================================================================
FUNCTION  tm_core_is_active_sess_mo 

DESCRIPTION
  Checks if the current on-going session is MO AppTrack/Standalone

DEPENDENCIES

RETURN VALUE
  TRUE: If the on-going session is MO AppTrack/Standalone
  FALSE: If the on-going session is not MO AppTrack/Standalone

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_active_sess_mo(void);
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

/*===========================================================================

FUNCTION tm_core_active_session_mode

DESCRIPTION
  Function to get the Session mode for the session in TM Core.

RETURN VALUE
  Return : TM OP_Mode if a session is active.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
tm_sess_operation_mode_e_type tm_core_active_session_mode();

/*===========================================================================

FUNCTION tm_core_get_sarf_status

DESCRIPTION
  This function returns the current SARF status from TM config 

DEPENDENCIES

RETURN VALUE boolean: TRUE: RF test in progress; FALSE: No RF test in progress

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_sarf_status(void);

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/*===========================================================================

FUNCTION tm_core_disable_APMP

DESCRIPTION
  This function disables APMP in GERA as notified by MC

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_disable_APMP(void);
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

/*===========================================================================
FUNCTION tm_core_lm_set_param_req

DESCRIPTION
This function is called to send SET param requests to LM

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_lm_set_param_req
(
   pdsm_pa_info_type *pz_PaInfo,
   pdsm_pa_e_type e_PaSetType
   );

/*===========================================================================

FUNCTION tm_core_persistent_sv_mask_report_handler

DESCRIPTION
  This function handles the GNSS Persistent SV Mask reporting from PE

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_persistent_sv_mask_report_handler(gnss_SvIdMaskStructType *p_SvMask);

/*===========================================================================

FUNCTION tm_core_persistent_sv_mask_filter_handler

DESCRIPTION
This function handles the GNSS Persistent SV Mask reporting from PE

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_persistent_sv_mask_filter_handler(gnss_SvIdMaskStructType *p_SvMask);

/*===========================================================================

FUNCTION tm_core_get_engine_monitor_config

DESCRIPTION
  This function sends the GET request for engine monitor config value to  LM

DEPENDENCIES

RETURN VALUE
 void

SIDE EFFECTS

===========================================================================*/
void tm_core_get_engine_monitor_config(void);

/*===========================================================================

FUNCTION tm_core_engine_monitor_report_get_handler

DESCRIPTION
  This function handles the GET response for engine monitor report from LM.

PARAMETER: Pointer to engine monitor report (uint64)

DEPENDENCIES: NONE

RETURN VALUE: NONE

SIDE EFFECTS: NONE

===========================================================================*/
void tm_core_engine_monitor_report_get_handler(const boolean *pv_engine_monitor_report_enabled);

/*===========================================================================

FUNCTION tm_core_set_engine_monitor_config

DESCRIPTION
  This function sends the SET request for engine monitor config value to  LM

DEPENDENCIES

RETURN VALUE
 void

SIDE EFFECTS

===========================================================================*/
void tm_core_set_engine_monitor_config(const boolean v_engine_monitor_config);

/*===========================================================================
FUNCTION tm_core_handle_engine_monitor_status_report

DESCRIPTION
This function handles engine monitor status report from MGP

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_handle_engine_monitor_status_report(uint64 const *const cpt_EngMonitorStatusRpt);

/*===========================================================================

FUNCTION tm_core_HandleConstellCtrlCfgInd

DESCRIPTION
Interface to report the GNSS Constellation Control Configuration.
This function reports the GNSS Constellation Control Configuration
provided by MGP to PDAPI client.

PARAMS
pz_ConstellCfgFromMgp : Constellation Control Configuration

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
None
===========================================================================*/
void tm_core_HandleConstellCtrlCfgInd(gnss_ConstellationConfigT *pz_ConstellCfgFromMgp);

/*
******************************************************************************
* tm_ConvertToLmLppmMode
*
* Function description:
* Converts TM Session Power Mode to LM LPPM Mode
*
* Return value: TRUE/FALSE
******************************************************************************
*/
boolean tm_ConvertToLmLppmMode(const tm_sess_power_mode_s_type *pz_TmConfig,
                               lm_session_request_lppm_s_type *pz_LmConfig);

/*===========================================================================

FUNCTION tm_core_switch_secondary_to_main_client

DESCRIPTION
   This function starts LM-TM session with E911 parameters when apptrack ends 
   first in concurrent mode.

PARAMS

DEPENDENCIES

RETURN VALUE 
   Boolean: TRUE: Successful, FALSE: Failure

SIDE EFFECTS

===========================================================================*/
void tm_core_switch_secondary_to_main_client(lm_gera_rcvr_mode_change_reason_e_type e_rcvr_change_reason,
                                             boolean                        u_forceMgpOff);
/*===========================================================================

FUNCTION
tm_core_report_ME_metrics_ind

DESCRIPTION
This function is called to send the band measurement metrics

DEPENDENCIES
None.

PARAMETERS
pz_MEMetrics - Band Measurement Metrics

RETURN VALUE
None.

SIDE EFFECTS
None.

===========================================================================*/

void tm_core_report_ME_metrics_ind(gnss_MEMetricsStructType *pz_MEMetrics);

/*===========================================================================
FUNCTION tm_core_lfm_config_update

DESCRIPTION
This function handles Feature status update receoved from Location Feature 
Manager 

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_lfm_config_update(sm_LFMConfigUpdateStructType *pz_LFMfeatureStatus);

/*
******************************************************************************
* tm_construct_location_notification_ind
*
* Function description:
*  Constructs location notification message that indicate the
*  protocol type, location info that was shiped outside modem, location response
*  that indicate if the session was rejected, accepted location shiped and accepted
*  with no location shiped, etc.
*
* PARAMS:
*  tm_location_notification_s_type tm_location_notification
*
* Return value: None
******************************************************************************
*/
void tm_construct_location_notification_ind(tm_location_notification_s_type const tm_location_notification);

/*===========================================================================
    
FUNCTION tm_core_read_new_subs_imsi
    
DESCRIPTION
  This function notifies SUPL of new ready sub. So that it can read the new
  subs IMSI
      
RETURN VALUE
  None
    
DEPENDENCIES
  None
    
SIDE EFFECTS
    
===========================================================================*/
void tm_core_read_new_subs_imsi(void);

/*===========================================================================
FUNCTION tm_send_sess_type_pdapi_event
DESCRIPTION
  This function sends a session type event to PDAPI

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern void tm_send_sess_type_pdapi_event(uint32 sess_req_type,
                                          pdsm_pd_comm_protocol_e_type protocol_type);

/*===========================================================================
FUNCTION  tm_core_send_otdoa_ext_status_event 

DESCRIPTION
  Sends a PDAPI_EXT_EVENT to PDAPI

Parameters:

  event type: the event to be sent to PDAPI
  event_structure: data to be sent for the event
===========================================================================*/
extern void tm_core_send_otdoa_ext_status_event(sm_loc_lte_otdoa_pdapi_event_type sess_event_type); 

/**
* @fn tm_HandleGnssTimeConversionInfo
* @brief Interface to report the GNSS Time Conversion params
*
* This function reports the GNSS Time conversion parameters
* used/generated by ME to PDAPI client. ME generates 1 report
* which is delivered as is to PDAPI.
*
* @param [in] pz_GnssTimeConvParamReport : GNSS time conversion
*        param report
*
* @retval None
*/
void tm_HandleGnssTimeConversionInfo(const gnss_TimeConversionParamType* pz_GnssTimeConvParamReport);
/*===========================================================================
FUNCTION  tm_core_abort_apptrack_in_concurr_state 
    
DESCRIPTION
  Aborts apptrack session in concurrent state
    
Parameters:
  None
===========================================================================*/
void tm_core_abort_apptrack_in_concurr_state(void);

/*===========================================================================
    
FUNCTION tm_core_start_CPI
                                            
DESCRIPTION
  This function starts CPI request when E911 client is added as main or 
  secondary.
    
RETURN VALUE
  None
    
DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/ 
void tm_core_start_CPI(tm_prtl_type prtl_type, 
                       tm_sess_handle_type sess_handle,
                       pdsm_pd_position_special_req_e_type e_specialReq);

/*
 ******************************************************************************
* tm_GetMgpReadiness
 *
 * Function description:
* Gets MGP Readiness status based on  boot-up calibration status
 *
* Return value: TRUE/FALSE
 ******************************************************************************
*/
boolean tm_GetMgpReadiness(mgp_BootupCalStateEnumType e_BootUpCalStatus);

/*===========================================================================
FUNCTION tm_core_set_ni_supl_on_dv

DESCRIPTION
Updates TRUE if NI CP session is in progress on DV outside e911 state

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_set_e911_ni_supl_on_dv(boolean v_e911_ni_supl_on_dv);
/*===========================================================================
FUNCTION tm_core_is_ni_e911_supl_on_dv_in_progress

DESCRIPTION
returns TRUE if Emergency nDD NI SUPL session is in progress 

DEPENDENCIES

RETURN VALUE
  TRUE if NI SUPL on DV is in progress
  FALSE in other cases

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_e911_ni_supl_on_dv_in_progress(void);

/*=============================================================================
  
FUNCTION
  tm_core_report_pdr_mag_cal_state
  
DESCRIPTION
  This function handles the calibration state of Magnetometer received from PE.
  
DEPENDENCIES
  None.
  
PARAMETERS
  sm_MagCalStateStructType - Pointer to the structure,
  pz_MagCalState containing Magnetometer's Calibrated State
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
=============================================================================*/
void tm_core_report_pdr_mag_cal_state(sm_MagCalStateStructType *pz_MagCalState);

#ifdef __cplusplus
}
#endif

#endif /* TM_DATA_H */

