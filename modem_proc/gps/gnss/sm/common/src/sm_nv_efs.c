/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      Session Manager NV Items Module

GENERAL DESCRIPTION
  This module contains the SM interface routines for the Non-Volatile Memory
  module.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015-2019 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/common/src/sm_nv_efs.c#4 $
  $DateTime: 2022/02/01 05:05:38 $
  $Author: pwbldsvc $

=============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/15/21   sai     Added new NV for ODCPI and change in ODCPI back off logic
08/12/19   sk      Added a NV to set second boundary aligned NMEA Fix report
02/22/19   mj      Location Privacy Feature
10/15/18   nath    Added a NV to control dynamic HEPE usage
09/25/18   nath    Added PQDTM change
08/07/18   skm     AGPS feature Support Mask  
05/18/18   skm     SHA (above SHA256 )and TLS (above 1.1) Support
01/29/18   py      NV grt_24_otdoa and rel12_timer moved to tm_core_carrier_specific_config
07/19/17   ak      Added NV for HEPE threshold config for DR/INS fixes
08/30/16   skm     Added NV for default LPP12 2 timer feature config 
04/15/16   ld      Added NHz NV support
02/17/16   mj      Added NV support to enable/disable CPI injections and early exit logic 
02/17/16   mj      Remove extra space in NV name for CPI/Early Exit feature 
02/11/16   gk      Added LPPe support
08/13/15   sjk     Added default configuration of GM from NV for AON
06/26/15   sjk      Adding NV for configuring Location Batching 
06/09/15   ssu      Adding NV for GM filtering of WLAN freq. 
03/14/15   rh      Added support for MultiSIM-EFS NV API 
02/26/15   ssu      Adding new NV items for GM LOWI integration.
01/14/15   rk      Added LPP >24 AD NV support.
11/13/14   rh      Added protocol transmission delay EFS NV  
02/21/14   ssu   Adding NV for configuring motion sensing distance in case of challenging GPS env. 
02/20/14   ssu     Adding a NV for modifying the medium responsiveness backoff value and gps challenging env backoff config. 
02/10/14   ssu     Adding a NV for modifying the rate at which GM requests CPI injection. 
01/10/14   ss       Supporting High Confidence Geofence config
11/08/12   rh      Added FEATURE_GNSS_NO_NV_WRITES to not write settings back to NV
07/17/12   mjohn   Added NV GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES for Premium Services (Enhanced Cell ID)
08/09/12   rk      Rename "gnss_lte_lpp_enable" EFS NV item to "gnss_lpp_enable" for 
                   tools to set/get this NV on all targets.
06/11/12   rk      Added support to get GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT NV item value.
11/24/10   rh      Added NV item for sending NO_FIX as Intermediate Report
06/10/09   gk      Added Get UTC offset from NV
01/15/09    atien  XTRA-T support
01/25/09   ns      Added support for QWiP NV item
08/22/08   jlp     Added support of emergency call simulation for simulating
                   E911 A-GPS
09/14/07    gk     NMEA CONFIG set parameters support
08/01/07    rw     Add support for XTRA feature
07/31/07    ank    Added Dynamic QoS and Override logic.
05/01/07    lt     Restore the default for cgps_mo_method back to MO_CP.
04/17/07    lt     Changed default for cgps_mo_method to MO_CP_LOC_EST.
04/12/07    ah     Added default read values for all SM-related NV items.
12/22/06    ank    Added default values for various NV items.
11/14/06    ah     Initial Release.

=============================================================================

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*
 * Include files
*/
#include "comdef.h"
//#include "customer.h"
#include "msg.h"
#include "sm_nv_efs.h"
#include "sm_nv.h"
#include "stringl.h"

/* ensure the order of the items in this table is same as the item definitions */
/* CGPS_NV_EFS_REG_ST_ITEM_INTERNAL - EFS path is /cgps/nv/item_files/... */
/* CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL - EFS path is /nv/item_files/cgps/nv/item_files/... */
static const cgps_nv_efs_reg_item_struct sm_nv_reg_items[] =
{
  /* data location, item file name, size */

  /*CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE , */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "xtra/cgps_xtra_preferred_max_valid_age", sizeof(uint16), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
  /*CGPS_NV_EFS_SM_SEND_NO_FIX_REPORT , */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_send_no_fix_report", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
  /*GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE , */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_1x_up_supl_enable", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
  /*GNSS_NV_EFS_SM_TM_LPP_ENABLE*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_lpp_enable", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
  /*GNSS_NV_EFS_SM_AGPS_RAT_PREF_CONFIG , */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_agps_rat_pref_config", sizeof(cgps_nv_agps_rat_pref_config), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
  /*GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL, */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "qmi/gnss_qmi_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
  /* GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_assisted_glo_protocol_select", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
  /* GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_config_premium_service", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS, */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_lpp_cp_timeout", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_EMERGENCY_PROTOCOL_SELECT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_emergency_session_protocol_select", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_SELECT*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_supl_udp_enable_select", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_LM_HEPE_THRESHOLD */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_lm_hepe_threshold", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },


  /* GNSS_NV_EFS_SM_GM_GNSS_UNAVAIL_IND_TIMEOUT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_gnss_unavail_ind_timeout", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_MOTION_DETECTION_SOURCES */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_motion_detection_sources", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_gnss_position_qos_session_timeout", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_POSITION_SOURCES */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_position_sources", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_XTRA_LAST_XTRA3_KEY_INDEX */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "xtra/gnss_xtra3_last_key_index", sizeof(uint16), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_WIFI_WAIT_TIMEOUT_SEC */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/wifi_wait_timeout_sec", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_GNSS_MAX_POS_UNC_ACCEPTED */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_gnss_max_pos_unc_accepted", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_USE_NW_ASST_FIXES */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_use_nw_asst_fixes", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_HIGH_RESPONSIVENESS_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_high_responsiveness_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_ALLOW_MO_SUPL_DURING_CONCURRENCY */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_nv_efs_sm_e911_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_CPI_REQUEST_RATE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_cpi_request_rate", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_MED_RESP_BACKOFF */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_med_resp_backoff", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_challenging_gps_env_backoff_config", sizeof(gm_chal_gps_env_backoff_config_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_challenging_gps_env_motion_sensing_dist_config", sizeof(gm_chal_gps_env_ms_dist_config_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_MS_SPEED_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_motion_state_speed_config", sizeof(gm_ms_speed_config_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
  /* GNSS_NV_EFS_SM_LBS_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_INTERNAL, "sm/gnss_lbs_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

  /* GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_CONTROL */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_meas_qmi_reporting_enable_control", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/tm_config_agnss_transmission_delay", sizeof(tm_config_agnss_transmission_delay_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_SUPPORT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_sm_lpp_grt_24_otdoa_ad_support", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /*GNSS_NV_EFS_SM_TM_OTDOA_CONTROL*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_otdoa_control", sizeof(pdsm_pa_otdoa_cfg_e_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_LOWI_REGISTER_ENABLE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_lowi_register_enable", sizeof(boolean), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_LOWI_SUBSCRIPTION_MASK */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_lowi_subscription_mask", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_LOWI_REG_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_lowi_reg_config", sizeof(gm_lowi_reg_config_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_COMP_LOGGING_RATE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_comp_logging_rate_secs", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/iod_configuration_switches", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_FILTER_WLAN_FREQ_MASK */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_filter_wlan_freq_mask", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_BATCHING_GF_RADIUS */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/batching_gf_radius", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_BATCHING_CONFIG_MASK */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/batching_config_mask", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_AON_DEFAULT_GNSS_POS_QOS_SESSION_TIMEOUT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/aon_default_gnss_position_qos_session_timeout", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_AON_DEFAULT_CPI_REQUEST_RATE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/aon_default_cpi_request_rate", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_AON_DEFAULT_CHAL_GPS_ENV_BO_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/aon_default_challenging_gps_env_backoff_config", sizeof(gm_chal_gps_env_backoff_config_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_AON_DEFAULT_CHAL_GPS_ENV_MS_DIST_CONFIG  */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/aon_default_challenge_gps_env_motion_sensing_dist_config", sizeof(aon_chal_gps_env_ms_dist_config_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_LPPE_UP_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_lppe_up_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_emergency_support_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/tech_sel_lppm_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/tech_sel_wifi_assist_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

#ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
  #error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */

  /* GNSS_NV_EFS_SM_TM_LPPE_CP_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_lppe_cp_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_UAX3_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "xtra/gnss_uax3_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TM_LPP_2TIMER_SUPPORT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_sm_lpp_rel12_2timer_support", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_concurrent_sessions_support_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_NHZ_CONFIG  */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_nhz_config", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_FUSION_CSM_DATA_UPLOAD_THRESHOLD */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_fusion_csm_data_upload_threshold", sizeof(uint16), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_FUSION_CSM_SB_BUFFER_SIZE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_fusion_csm_sb_buffer_size", sizeof(uint16), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_FUSION_CSM_MAX_SSID_SUPPORTED */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_fusion_csm_max_ssid_supported", sizeof(uint16), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_FUSION_CSM_VENUE_TRACKING_RESP_SEC */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_fusion_csm_venue_tracking_resp_sec", sizeof(uint16), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_FUSION_CSM_OPERATION_MODE */
  { CGPS_NV_EFS_REG_ST_ITEM_INTERNAL, "sm/gnss_fusion_csm_operation_mode", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_FUSION_BARO_WEIGHT */
  { CGPS_NV_EFS_REG_ST_ITEM_INTERNAL, "sm/gnss_fusion_csm_baro_weight", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_FUSION_BARO_SAMPLE_RATE_HZ */
  { CGPS_NV_EFS_REG_ST_ITEM_INTERNAL, "sm/gnss_fusion_csm_baro_rate_hz", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TEST_CONTROL */
  { CGPS_NV_EFS_REG_ST_ITEM_INTERNAL, "sm/gnss_fusion_csm_test_control", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/assisted_bds_protocol_enable_mask", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GM_DRIVE_TO_PED_PREDICTOR_ENABLE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gm_drive_to_ped_predictor_enable", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_LM_HEPE_THRESHOLD_FOR_DR */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_lm_hepe_threshold_dr_fixes", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TECH_SEL_LPPM_TBM_PROFILE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/tech_sel_lppm_tbm_profile", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /*GNSS_NV_EFS_SM_TM_SHA_TLS_SUPPORT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_sm_sha_tls_support", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /*GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_sm_agps_features", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /*GNSS_NV_EFS_SM_TM_CONTROL_DATUM_NMEA_SENTENCE*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_control_datum_nmea_sentence", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /*GNSS_NV_EFS_DISABLE_DYN_HEPE_FOR_BETTER_TTFF*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_control_custom_dynamic_HEPE_usage", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW (NV 74234)*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_e911_extension_window", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM (NV 74235)*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_lock_ctrl_for_oem", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_EXTENDED_NMEA_SENTENCE_MASK_V02 (NV 74264)*/
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_nmea_extended_sentence_type_v2", sizeof(uint64), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /*GNSS_NV_EFS_INTEGER_SECOND_BOUNDARY_ALIGNED_NMEA_REPORT   NV# 74293  */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/gnss_control_integer_second_boundary_aligned_NMEA_report", sizeof(uint8), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_XTRA_TRACK_XC_CONFIG_INFO_HANDSHAKE */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "xtra/gnss_xtra3_config_info_handshake", sizeof(uint32), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* GNSS_NV_EFS_SM_TECH_SEL_ODCPI_TIMERS (NV 74571) */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "sm/tech_sel_odcpi_timers", sizeof(tech_sel_odcpi_timers_s_type), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  /* add entries above this line and make sure you update the CGPS_NV_EFS_SM_MAX_ITEM */
};


typedef struct
{
  /*CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE , */
  uint16 cgps_xtra_preferred_max_valid_age;
  /*CGPS_NV_EFS_SM_SEND_NO_FIX_REPORT , */
  uint8  gnss_send_no_fix_report;

  /*GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL, */
  uint32 gnss_qmi_config;
  /* GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES */
  uint32 gnss_config_premium_service;
  /* GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS, */
  uint8 gnss_lpp_cp_timeout;
  /* GNSS_NV_EFS_SM_TM_EMERGENCY_PROTOCOL_SELECT */
  uint32 gnss_emergency_session_protocol_select;
  /* GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_CONTROL */
  uint8 gnss_meas_report_control;
  /* GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK */
  uint32 q_gnss_sm_gm_iod_config_switches_mask;
  /* GNSS_NV_EFS_SM_TM_OTDOA_CONTROL*/
  pdsm_pa_otdoa_cfg_e_type gnss_otdoa_control;

  /* Below are carrier-specific EFS NVs (one copy per sub) */
  /*GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE , */
  uint8 gnss_1x_up_supl_enable[SM_CONFIG_MAX_SUBS];
  /*GNSS_NV_EFS_SM_TM_LPP_ENABLE*/
  uint8 gnss_lpp_enable[SM_CONFIG_MAX_SUBS];
  /*GNSS_NV_EFS_SM_AGPS_RAT_PREF_CONFIG , */
  cgps_nv_agps_rat_pref_config gnss_agps_rat_pref_config[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT */
  uint32 gnss_assisted_glo_protocol_select[SM_CONFIG_MAX_SUBS];

  /*GNSS_NV_EFS_SM_TM_CONTROL_DATUM_NMEA_SENTENCE*/
  uint8 u_gnss_sm_control_datum_nmea_sentence[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_LPPE_UP_CONFIG */
  uint32 gnss_lppe_up_config[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_SELECT*/
  uint8 gnss_supl_udp_enable_select[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_E911_CPI_EARLY_EXIT_CONFIG*/
  uint32 gnss_emergency_support_config[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_LPPE_CP_CONFIG */
  uint32 gnss_lppe_cp_config[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_UAX3_CONFIG */
  uint32 gnss_uax3_config;

  /* GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG */
  uint32 gnss_concurrent_sessions_support_config;

  /* GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK */
  uint32 sm_assisted_bds_protocol_enable_mask[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_SUPPORT */
  uint8 u_gnss_lpp_grt_24_otdoa[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_SHA_TLS_SUPPORT */
  uint32 gnss_sm_sha_tls_support[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT */
  uint32 gnss_sm_agps_features[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT */
  uint8 u_gnss_lpp_rel12_2timer_support[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_DISABLE_DYN_HEPE_FOR_BETTER_TTFF */
  uint8   u_gnss_disable_dynamic_HEPE_usage;

  /* GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW */
  uint32  q_gnss_emergency_extension_window[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM */
  uint32  q_gnss_lock_ctrl_for_oem[SM_CONFIG_MAX_SUBS];

  /* GNSS_NV_EFS_INTEGER_SECOND_BOUNDARY_ALIGNED_NMEA_REPORT */
  uint8 u_gnss_second_boundary_aligned_NMEA;

  /* GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02 (NV 74264) */
  uint64 t_gnss_ext_nmea_sentence_mask_v02[SM_CONFIG_MAX_SUBS];
} sm_efs_nv_cached_struct_type;

static sm_efs_nv_cached_struct_type sm_efs_nv_cached_values;


/*===========================================================================

FUNCTION sm_nv_efs_map_sub

DESCRIPTION
  This function is used to map Subscription ID to proper value (if it's
  subscription-indepdendent NV, it'll always map to Subs 0)
 
  
RETURN VALUE
  cgps_nv_sub_id_type 
  
DEPENDENCIES
 
===========================================================================*/
static sys_modem_as_id_e_type sm_nv_efs_map_sub(cgps_nv_efs_reg_item_type const item, sys_modem_as_id_e_type sub)
{
  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > sm_nv_max_sub))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid Subs ID %d (Max=%d)", sub, sm_nv_max_sub);
    return SM_CONFIG_MIN_SUB_ID;
  }

  switch (item)
  {
    case GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE:
    case GNSS_NV_EFS_SM_TM_LPP_ENABLE:
    case GNSS_NV_EFS_SM_TM_LPPE_UP_CONFIG:
    case GNSS_NV_EFS_SM_AGPS_RAT_PREF_CONFIG:
    case GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT:
#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
    case GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_SELECT:
    case GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG:
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */
    case GNSS_NV_EFS_SM_TM_LPPE_CP_CONFIG:
    case GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_SUPPORT:
    case GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT:
    case GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK:
    case GNSS_NV_EFS_SM_TM_SHA_TLS_SUPPORT:
    case GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT:
    case GNSS_NV_EFS_SM_TM_CONTROL_DATUM_NMEA_SENTENCE:
    case GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW:
    case GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM:
    case GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02:
      return sub; /* subscription-dependent EFS NV, return sub */

    default:
      return SM_CONFIG_MIN_SUB_ID;  /* subscription-independent EFS NV, return 0 */
  }
}

/*==============================================================================
  FUNCTION
    sm_nv_efs_reg_get_default

  DESCRIPTION
    Retrieve the default value for specified EFS NV item and store it in 'dest'.
    Note that the size of 'dest' is implicit via the item type.
 
    NB: This function may block if the data source resides in another task.
    Returns True if successful, False on error. This function should be expanded
    to include all default values.
 
  RETURN VALUE
    TRUE if no ERROR
    FALSE if error

  
==============================================================================*/
boolean sm_nv_efs_reg_get_default(cgps_nv_efs_reg_item_type const item, uint8 data_size,
                                  void *data_ptr)
{

  boolean b_ret = FALSE;

  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return b_ret;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "CGPS_NV_EFS_REG: Registry Get %d Default", item);

  switch (item)
  {
    case CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE:
      if (data_size == sizeof(uint16))
      {
        *(uint16 *)data_ptr = CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE_DEFAULT;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_SEND_NO_FIX_REPORT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_SEND_NO_FIX_REPORT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE_DEFAULT;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPP_ENABLE:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_TM_LPP_ENABLE_DEFAULT;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_AGPS_RAT_PREF_CONFIG:
      if (data_size == sizeof(cgps_nv_agps_rat_pref_config))
      {
        cgps_nv_agps_rat_pref_config *rat_config = (cgps_nv_agps_rat_pref_config *)data_ptr;
        rat_config->umts_lte_prefered = GNSS_NV_EFS_SM_AGPS_RAT_PREF_UMTS_LTE_PREFERED;
        rat_config->agps_rat_enable_bit_mask = GNSS_NV_EFS_SM_AGPS_RAT_ENABLE_BIT_MASK;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL_DEFAULT;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TM_ASST_GLO_PROTOCOL_DEFAULT;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_CONFIG_SAP_PREMIUM_DEFAULT;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_EMERGENCY_PROTOCOL_SELECT:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CP_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_SELECT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = (uint8)GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_LM_HEPE_THRESHOLD:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_LM_HEPE_THRESHOLD_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_GNSS_UNAVAIL_IND_TIMEOUT:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_GNSS_UNAVAIL_IND_TIMEOUT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_MOTION_DETECTION_SOURCES:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_MOTION_DETECTION_SOURCES_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_POSITION_SOURCES:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_POSITION_SOURCES_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_XTRA_LAST_XTRA3_KEY_INDEX:
      if (data_size == sizeof(uint16))
      {
        *(uint16 *)data_ptr = GNSS_NV_EFS_SM_XTRA_LAST_XTRA3_KEY_INDEX_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_XTRA_TRACK_XC_CONFIG_INFO_HANDSHAKE:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_XTRA_TRACK_XC_CONFIG_INFO_HANDSHAKE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_WIFI_WAIT_TIMEOUT_SEC:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_TM_WIFI_WAIT_TIMEOUT_SEC_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_GNSS_MAX_POS_UNC_ACCEPTED:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_GNSS_MAX_POS_UNC_ACCEPTED_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_HIGH_RESPONSIVENESS_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_HIGH_RESPONSIVENESS_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;

#ifdef FEATURE_MO_GPS_FOR_ECALL
    case GNSS_NV_EFS_SM_ALLOW_MO_SUPL_DURING_CONCURRENCY:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_ALLOW_MO_SUPL_DURING_CONCURRENCY_DEFAULT;
        b_ret = TRUE;
      }
      break;
#endif

    case GNSS_NV_EFS_SM_GM_CPI_REQUEST_RATE:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_CPI_REQUEST_RATE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_MED_RESP_BACKOFF:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_MED_RESP_BACKOFF_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_CONFIG:
      if (data_size == (sizeof(gm_chal_gps_env_backoff_config_s_type)))
      {
        gm_chal_gps_env_backoff_config_s_type *backoff_config = (gm_chal_gps_env_backoff_config_s_type *)data_ptr;
        backoff_config->q_backoff_min = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_MIN_DEFAULT;
        backoff_config->q_backoff_max = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_MAX_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_CONFIG:
      if (data_size == (sizeof(gm_chal_gps_env_ms_dist_config_s_type)))
      {
        gm_chal_gps_env_ms_dist_config_s_type *dist_config = (gm_chal_gps_env_ms_dist_config_s_type *)data_ptr;
        dist_config->q_dist_used_for_gps = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_GPS_DEFAULT;
        dist_config->q_dist_used_for_cpi = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_CPI_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_MS_SPEED_CONFIG:
      if (data_size == (sizeof(gm_ms_speed_config_s_type)))
      {
        gm_ms_speed_config_s_type *speed_config = (gm_ms_speed_config_s_type *)data_ptr;
        speed_config->gm_ms_walk_speed = GNSS_NV_EFS_SM_GM_MS_WALK_SPEED_DEFAULT;
        speed_config->gm_ms_run_speed = GNSS_NV_EFS_SM_GM_MS_RUN_SPEED_DEFAULT;
        b_ret = TRUE;
      }
      break;

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
    case GNSS_NV_EFS_SM_LBS_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_LBS_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

    case GNSS_NV_EFS_SM_GM_LOWI_REGISTER_ENABLE:
      if (data_size == (sizeof(boolean)))
      {
        *(boolean *)data_ptr = GNSS_NV_EFS_SM_GM_LOWI_REG_ENABLE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_LOWI_SUBSCRIPTION_MASK:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_LOWI_SUB_MASK_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_LOWI_REG_CONFIG:
      if (data_size == (sizeof(gm_lowi_reg_config_s_type)))
      {
        gm_lowi_reg_config_s_type *p_lowi_reg_conf = (gm_lowi_reg_config_s_type *)data_ptr;
        p_lowi_reg_conf->q_enable_reg_timer_mask = GNSS_NV_EFS_SM_GM_ENABLE_TIMER_MASK_DEFAULT;
        p_lowi_reg_conf->q_short_term_timer_max_retry_cnt = GNSS_NV_EFS_SM_GM_SHORT_TERM_TIMER_MAX_RETRY_CNT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_FILTER_WLAN_FREQ_MASK:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_FILTER_WLAN_FREQ_MASK_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_CONTROL:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY:
      {
        uint32 i, j;

        if (data_size == (sizeof(tm_config_agnss_transmission_delay_s_type)))
        {
          tm_config_agnss_transmission_delay_s_type *trans_delay_config = (tm_config_agnss_transmission_delay_s_type *)data_ptr;

          for (i = 0; i < GNSS_NV_TM_TRANS_DELAY_MAX_INDEX; i++)
          {
            for (j = 0; j < GNSS_NV_TM_TRANS_DELAY_MAX_PLANE; j++)
            {
                /* generic default delay is 1000ms */
                trans_delay_config->trans_delay[i][j] = GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY_DEFAULT;
              }
            }
          b_ret = TRUE;
        }
        break;
      }

    case GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_SUPPORT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_BATCHING_GF_RADIUS:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_BATCHING_GF_RADIUS_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_OTDOA_CONTROL:
      if (sizeof(uint32) == data_size)
      {
        *(uint32 *)data_ptr = (uint32)GNSS_NV_EFS_SM_TM_OTDOA_CONTROL_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_BATCHING_CONFIG_MASK:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_BATCHING_CONFIG_MASK_DEFAULT;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_COMP_LOGGING_RATE:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_GM_COMP_LOGGING_RATE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_AON_GNSS_POS_QOS_SESSION_TIMEOUT:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_AON_DEFAULT_GNSS_POS_QOS_SESSION_TIMEOUT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_AON_CPI_REQUEST_RATE:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_AON_DEFAULT_CPI_REQUEST_RATE;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_AON_CHAL_GPS_ENV_BO_CONFIG:
      if (data_size == (sizeof(gm_chal_gps_env_backoff_config_s_type)))
      {
        gm_chal_gps_env_backoff_config_s_type *backoff_config = (gm_chal_gps_env_backoff_config_s_type *)data_ptr;
        backoff_config->q_backoff_min = GNSS_NV_EFS_SM_AON_DEFAULT_CHAL_GPS_ENV_BO_MIN;
        backoff_config->q_backoff_max = GNSS_NV_EFS_SM_AON_DEFAULT_CHAL_GPS_ENV_BO_MAX;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_AON_CHAL_GPS_ENV_MS_DIST_CONFIG:
      if (data_size == (sizeof(aon_chal_gps_env_ms_dist_config_s_type)))
      {
        aon_chal_gps_env_ms_dist_config_s_type *dist_config = (aon_chal_gps_env_ms_dist_config_s_type *)data_ptr;
        dist_config->q_dist_used_for_gps = GNSS_NV_EFS_SM_AON_DEFAULT_CHAL_GPS_ENV_MS_DIST_GPS;
        dist_config->q_dist_used_for_cpi = GNSS_NV_EFS_SM_AON_DEFAULT_CHAL_GPS_ENV_MS_DIST_CPI;
        b_ret = TRUE;
      }
      break;
#ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
    #error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */
    case GNSS_NV_EFS_SM_TM_LPPE_UP_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TM_LPPE_UP_ENABLE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = (uint32)GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_LPPE_CP_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TM_LPPE_CP_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_UAX3_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TM_UAX3_CONFIG_DEFAULT;  /* UAX3 is enabled */
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG_DEFAULT;  /* Concurrency feature is enabled */
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_NHZ_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_NHZ_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_FUSION_CSM_DATA_UPLOAD_THRESHOLD:
      if (data_size == (sizeof(uint16)))
      {
        *(uint16 *)data_ptr = GNSS_NV_EFS_SM_FUSION_CSM_DATA_UPLOAD_THRESHOLD_MINS_DEFAULT;
        b_ret = TRUE;
      }
      break;


    case GNSS_NV_EFS_SM_FUSION_CSM_SB_BUFFER_SIZE:
      if (data_size == (sizeof(uint16)))
      {
        *(uint16 *)data_ptr = GNSS_NV_EFS_SM_FUSION_CSM_SB_BUFFER_SIZE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_FUSION_CSM_MAX_SSID_SUPPORTED:
      if (data_size == (sizeof(uint16)))
      {
        *(uint16 *)data_ptr = GNSS_NV_EFS_SM_FUSION_CSM_MAX_SSID_SUPPORTED_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_FUSION_CSM_VENUE_TRACKING_RESP_SEC:
      if (data_size == (sizeof(uint16)))
      {
        *(uint16 *)data_ptr = GNSS_NV_EFS_SM_FUSION_CSM_VENUE_TRACKING_RESP_SEC_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_FUSION_CSM_OPERATION_MODE:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_FUSION_CSM_OPERATION_MODE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_FUSION_BARO_WEIGHT:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_FUSION_CSM_BARO_FILTER_WEIGHT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_FUSION_BARO_SAMPLE_RATE_HZ:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_FUSION_BARO_SAMPLE_RATE_HZ_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TEST_CONTROL:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TEST_CONTROL_DEFAULT_MASK;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GM_DRIVE_TO_PED_PREDICTOR_ENABLE:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_GM_DRIVE_TO_PED_PREDICTOR_ENABLE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_LM_HEPE_THRESHOLD_FOR_DR:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_LM_HEPE_THRESHOLD_FOR_DR_DEFAULT;  /* Set default HEPE THRESHOLD for DR/INS fixes */
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TECH_SEL_LPPM_TBM_PROFILE:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_TECH_SEL_LPPM_TBM_PROFILE_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_SHA_TLS_SUPPORT:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TM_SHA_TLS_SUPPORT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_CONTROL_DATUM_NMEA_SENTENCE:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_TM_WGS84_DATUM_NMEA_SENTENCE;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_DISABLE_DYN_HEPE_FOR_BETTER_TTFF:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_DYN_HEPE_DISABLED_FOR_BETTER_TTFF_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = (uint32)GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = (uint32)GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_INTEGER_SECOND_BOUNDARY_ALIGNED_NMEA_REPORT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = GNSS_NV_EFS_SM_SECOND_BOUNDARY_ALIGNED_NMEA_DEFAULT;
      }
      break;

    case GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02:
      if (data_size == sizeof(uint64))
      {
        *(uint64 *)data_ptr = (uint64)GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02_DEFAULT;
        b_ret = TRUE;
      }
      break;

      
    default:
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "CGPS_NV_EFS_REG: Invalid item id: %d", item);
      break;
  }

  return b_ret;
}


/*==============================================================================
  FUNCTION
    sm_nv_efs_reg_get_cached_value

  DESCRIPTION
    Retrieve the specified EFS NV item from cache storage and store it in 'dest'.
    Note that the size of 'dest' is implicit via the item type. Returns True if
    successful, False on error. This function should be expanded to include all
    default values.
 
  RETURN VALUE
    TRUE if no ERROR
    FALSE if error
  
==============================================================================*/
static boolean sm_nv_efs_reg_get_cached_value(cgps_nv_efs_reg_item_type const item, uint8 data_size,
                                              void *data_ptr, sys_modem_as_id_e_type sub)
{

  boolean b_ret = FALSE;

  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return b_ret;
  }

  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > sm_nv_max_sub))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "CGPS_NV_EFS_GET_CAHCED: Invalid sub %d (Max=%d)", sub, sm_nv_max_sub);
    return b_ret;
  }

  switch (item)
  {
    case CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE:
      if (data_size == sizeof(uint16))
      {
        *(uint16 *)data_ptr = sm_efs_nv_cached_values.cgps_xtra_preferred_max_valid_age;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_SEND_NO_FIX_REPORT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.gnss_send_no_fix_report;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_qmi_config;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_config_premium_service;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.gnss_lpp_cp_timeout;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_EMERGENCY_PROTOCOL_SELECT:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_emergency_session_protocol_select;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.q_gnss_sm_gm_iod_config_switches_mask;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_CONTROL:
      if (sizeof(uint32) == data_size)
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_meas_report_control;
        b_ret = TRUE;
      }
      break;

      /* Below are for carrier-specific EFS NVs (one copy per sub) */
    case GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.gnss_1x_up_supl_enable[sub];
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPP_ENABLE:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.gnss_lpp_enable[sub];
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_AGPS_RAT_PREF_CONFIG:
      if (data_size == sizeof(cgps_nv_agps_rat_pref_config))
      {
        memscpy((void *)(data_ptr), sizeof(cgps_nv_agps_rat_pref_config),
                (void *)&(sm_efs_nv_cached_values.gnss_agps_rat_pref_config[sub]),
                sizeof(cgps_nv_agps_rat_pref_config));
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_assisted_glo_protocol_select[sub];
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPPE_UP_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_lppe_up_config[sub];
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_emergency_support_config[sub];
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPPE_CP_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_lppe_cp_config[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG_DEFAULT;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_SELECT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.gnss_supl_udp_enable_select[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_OTDOA_CONTROL:
      if (sizeof(uint32) == data_size)
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_otdoa_control;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_UAX3_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_uax3_config;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_concurrent_sessions_support_config;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.sm_assisted_bds_protocol_enable_mask[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_SUPPORT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.u_gnss_lpp_grt_24_otdoa[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT:
      if (data_size == sizeof(uint8))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.u_gnss_lpp_rel12_2timer_support[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_SHA_TLS_SUPPORT:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_sm_sha_tls_support[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT:
      if (data_size == (sizeof(uint32)))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.gnss_sm_agps_features[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_CONTROL_DATUM_NMEA_SENTENCE:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.u_gnss_sm_control_datum_nmea_sentence[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_DISABLE_DYN_HEPE_FOR_BETTER_TTFF:
      if (data_size == (sizeof(uint8)))
      {
        *(uint8 *)data_ptr = sm_efs_nv_cached_values.u_gnss_disable_dynamic_HEPE_usage;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.q_gnss_emergency_extension_window[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM:
      if (data_size == sizeof(uint32))
      {
        *(uint32 *)data_ptr = sm_efs_nv_cached_values.q_gnss_lock_ctrl_for_oem[sub];
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02:
      if (data_size == sizeof(uint64))
      {
        *(uint64 *)data_ptr = sm_efs_nv_cached_values.t_gnss_ext_nmea_sentence_mask_v02[sub];
        b_ret = TRUE;
      }
      break;

    default:
      b_ret = FALSE;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "CGPS_NV_EFS_GET_CAHCED: Invalid item id: %d", item);
      break;
  }

  return b_ret;
}

/*==============================================================================
  FUNCTION
    sm_nv_efs_reg_set_cached_value

  DESCRIPTION
    Write the specified EFS NV into its cache storage.  Note that the size of
    'dest' is implicit via the item type. Returns True if successful, False on
    error. This function should be expanded to include all default values.
 
  RETURN VALUE
    TRUE if no ERROR
    FALSE if error
  
==============================================================================*/
static boolean sm_nv_efs_reg_set_cached_value(cgps_nv_efs_reg_item_type const item, uint8 data_size,
                                              void *data_ptr, sys_modem_as_id_e_type sub)
{
  boolean b_ret = FALSE;

  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return b_ret;
  }

  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > sm_nv_max_sub))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "CGPS_NV_EFS_SET_CACHED: Invalid sub %d", sub);
    return b_ret;
  }

  switch (item)
  {
    case CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE:
      if (data_size == sizeof(uint16))
      {
        sm_efs_nv_cached_values.cgps_xtra_preferred_max_valid_age = *(uint16 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_SEND_NO_FIX_REPORT:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.gnss_send_no_fix_report = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_qmi_config = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_config_premium_service = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.gnss_lpp_cp_timeout = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_EMERGENCY_PROTOCOL_SELECT:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_emergency_session_protocol_select = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.q_gnss_sm_gm_iod_config_switches_mask = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_CONTROL:
      if (sizeof(uint8) == data_size)
      {
        sm_efs_nv_cached_values.gnss_meas_report_control = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;


      /* Below are for carrier-specific EFS NVs (one copy per sub) */
    case GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.gnss_1x_up_supl_enable[sub] = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPP_ENABLE:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.gnss_lpp_enable[sub] = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_AGPS_RAT_PREF_CONFIG:
      if (data_size == sizeof(cgps_nv_agps_rat_pref_config))
      {
        memscpy((void *)&(sm_efs_nv_cached_values.gnss_agps_rat_pref_config[sub]),
                sizeof(cgps_nv_agps_rat_pref_config),
                (void *)(data_ptr), sizeof(cgps_nv_agps_rat_pref_config));
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_assisted_glo_protocol_select[sub] = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_CONTROL_DATUM_NMEA_SENTENCE:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.u_gnss_sm_control_datum_nmea_sentence[sub] = *(uint8 *)data_ptr;
      }
      break;


    case GNSS_NV_EFS_SM_TM_LPPE_UP_CONFIG:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_lppe_up_config[sub] = *(uint32 *)data_ptr;
      }
      break;
    case GNSS_NV_EFS_SM_TM_LPPE_CP_CONFIG:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_lppe_cp_config[sub] = *(uint32 *)data_ptr;
      }
      break;
    case GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_SUPPORT:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.u_gnss_lpp_grt_24_otdoa[sub] = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;
    case GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.u_gnss_lpp_rel12_2timer_support[sub] = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_SELECT:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.gnss_supl_udp_enable_select[sub] = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;


    case GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_emergency_support_config[sub] = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_SHA_TLS_SUPPORT:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_sm_sha_tls_support[sub] = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.gnss_sm_agps_features[sub] = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

      /* Above are for carrier-specific EFS NVs (one copy per sub) */

    case GNSS_NV_EFS_SM_TM_OTDOA_CONTROL:
      if (sizeof(uint32) == data_size)
      {
        sm_efs_nv_cached_values.gnss_otdoa_control = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_UAX3_CONFIG:
      if (sizeof(uint32) == data_size)
      {
        sm_efs_nv_cached_values.gnss_uax3_config = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG:
      if (sizeof(uint32) == data_size)
      {
        sm_efs_nv_cached_values.gnss_concurrent_sessions_support_config = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_DISABLE_DYN_HEPE_FOR_BETTER_TTFF:
      if (data_size == sizeof(uint8))
      {
        sm_efs_nv_cached_values.u_gnss_disable_dynamic_HEPE_usage = *(uint8 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.q_gnss_emergency_extension_window[sub] = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM:
      if (data_size == sizeof(uint32))
      {
        sm_efs_nv_cached_values.q_gnss_lock_ctrl_for_oem[sub] = *(uint32 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    case GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02:
      if (data_size == sizeof(uint64))
      {
        sm_efs_nv_cached_values.t_gnss_ext_nmea_sentence_mask_v02[sub] = *(uint64 *)data_ptr;
        b_ret = TRUE;
      }
      break;

    default:
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "CGPS_NV_EFS_SET_CACHED Invalid item id: %d", item);
      break;
  }

  return b_ret;
}


/*===========================================================================

FUNCTION sm_nv_efs_init

DESCRIPTION
  This function is used to initialize internal SM-NV data efs structure.
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void sm_nv_efs_init(void)
{
#ifndef FEATURE_GNSS_LEAN_BUILD

  boolean ret_val = FALSE;
  ret_val =  cgps_nv_efs_init(CGPS_NV_EFS_SM, CGPS_NV_EFS_SM_MIN_ITEM,
                              CGPS_NV_EFS_SM_MAX_ITEM,
                              sm_nv_reg_items);

  if (ret_val)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS init successful");
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS init failed");
  }

#endif
}


/*===========================================================================

FUNCTION sm_nv_efs_reg_item_read

DESCRIPTION
  This function is used to read EFS NV item. If the read fails, it sets it to
  default value. This function should be called only at Boot-Up, at all other 
  times, the read should be sm_nv_efs_reg_item_read_ex() which reads from the
  cache
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
boolean sm_nv_efs_reg_item_read(cgps_nv_efs_reg_item_type const item,
                                uint8 data_size,
                                void *data_ptr
                                )
{
#ifndef FEATURE_GNSS_LEAN_BUILD
  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return FALSE;
  }

  /* Using current DD-sub for carrier-specific NV if No Sub is specificed */
  return sm_nv_efs_reg_item_read_from_sub(item, data_size, data_ptr,
                                          sm_nv_efs_map_sub(item, sm_nv_current_sub));
#else
  return TRUE;
#endif
}

/*===========================================================================

FUNCTION sm_nv_efs_reg_item_read_ex

DESCRIPTION
  This function is used to read EFS NV item. If the read fails, it sets it to
  default value. If cached value is present, will return cached value instead
  of reading from EFS.
  GPS SHALL CALL THIS API INSTEAD OF sm_nv_read AFTER BOOT UP INITIALIZATION
  
  Using current DD-sub for carrier-specific EFS NVs
 
RETURN VALUE
  Boolean: TRUE for success, FALSE for failure
  
DEPENDENCIES
 
===========================================================================*/
boolean sm_nv_efs_reg_item_read_ex(cgps_nv_efs_reg_item_type const item,
                                   uint8 data_size,
                                   void *data_ptr
                                   )
{


#ifndef FEATURE_GNSS_LEAN_BUILD

  cgps_nv_efs_reg_rw_stat_enum_type status = CGPS_NV_EFS_REG_RW_STAT_INVALID;
  boolean return_value = FALSE;
  sys_modem_as_id_e_type sm_nv_mapped_sub = sm_nv_efs_map_sub(item, sm_nv_current_sub);

  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return FALSE;
  }


  return_value = sm_nv_efs_reg_get_cached_value(item, data_size, data_ptr,
                                                sm_nv_mapped_sub);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SM_NV_EFS: NV EFS file read from cache for item = %d(status=%u Sub=%d)",
        item, (uint8)return_value, sm_nv_mapped_sub);

  if (return_value == TRUE)
  {
    status = CGPS_NV_EFS_REG_RW_STAT_OK; // got cached value, don't use default
  }

  if (status == CGPS_NV_EFS_REG_RW_STAT_OK)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS file read succeeded for item=%d (sub=%d)",
          item, sm_nv_mapped_sub);
    return_value = TRUE;
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS file read failed for item=%d(sub=%d), setting defaults",
          item, sm_nv_mapped_sub);
    return_value = sm_nv_efs_reg_get_default(item, data_size, data_ptr);
  }

  return return_value;


#else
  return TRUE;
#endif

}


/*===========================================================================

FUNCTION sm_nv_efs_reg_item_write

DESCRIPTION
  This function is used to write an NV item. 

  
RETURN VALUE
  TRUE for success, otherwise FALSE
  
DEPENDENCIES
 
===========================================================================*/
boolean sm_nv_efs_reg_item_write(cgps_nv_efs_reg_item_type const item,
                                 uint8 data_size,
                                 void *data_ptr
                                 )
{
  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return FALSE;
  }

  /* Using current DD-sub for carrier-specific NV if No Sub is specificed */
  return sm_nv_efs_reg_item_write_to_sub(item, data_size, data_ptr,
                                         sm_nv_efs_map_sub(item, sm_nv_current_sub));
}


/*===========================================================================

FUNCTION sm_nv_efs_reg_item_read_sub

DESCRIPTION
  This function is used to read EFS NV item for a specific subscription.
  If the read fails, it sets it to default value. It'll also set the
  cache values if available. This function should be called at bootup
  and during SIM swap (always read from EFS not cache).
  
RETURN VALUE
  Boolean: TRUE for success, FALSE for failure
  
DEPENDENCIES
 
===========================================================================*/
boolean sm_nv_efs_reg_item_read_from_sub(cgps_nv_efs_reg_item_type const item,
                                         uint8 data_size,
                                         void *data_ptr,
                                         sys_modem_as_id_e_type sub
                                         )
{
  cgps_nv_efs_reg_rw_stat_enum_type status = CGPS_NV_EFS_REG_RW_STAT_INVALID;
  boolean return_value = FALSE;

  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return FALSE;
  }

  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > sm_nv_max_sub))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid Subs ID %d(Max=%d)", sub, sm_nv_max_sub);
    return FALSE;
  }

  if ((item >= CGPS_NV_EFS_SM_MIN_ITEM) &&
      (item <= CGPS_NV_EFS_SM_MAX_ITEM))
  {
    status =  cgps_nv_efs_reg_item_read_subsid(item, data_size, data_ptr,
                                               CGPS_NV_EFS_SM_MIN_ITEM,
                                               CGPS_NV_EFS_SM_MAX_ITEM, sm_nv_reg_items,
                                               (cgps_nv_efs_sub_id_type)sub);
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NV item index %d, out of range", (int)item);
    return FALSE;
  }

  if (status == CGPS_NV_EFS_REG_RW_STAT_OK)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS file read succeeded for item=%d(sub=%d)", item, sub);
    return_value = TRUE;
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS file read failed for item=%d(sub=%d), setting defaults", item, sub);
    return_value = sm_nv_efs_reg_get_default(item, data_size, data_ptr);
  }

  if (return_value == TRUE)
  {
    /* write EFS NV from hw to cache */
    (void)sm_nv_efs_reg_set_cached_value(item, data_size, data_ptr, sub);
  }

  return return_value;
}


/*===========================================================================

FUNCTION sm_nv_efs_reg_item_write

DESCRIPTION
  This function is used to write an EFS NV item to a specific subscription
 
  
RETURN VALUE
  Boolean: TRUE for success, FALSE for failure
  
DEPENDENCIES
 
===========================================================================*/
boolean sm_nv_efs_reg_item_write_to_sub
(
   cgps_nv_efs_reg_item_type const item,
   uint8 data_size,
   void *data_ptr,
   sys_modem_as_id_e_type sub
   )
{
#ifndef FEATURE_GNSS_LEAN_BUILD
  cgps_nv_efs_reg_rw_stat_enum_type status = CGPS_NV_EFS_REG_RW_STAT_INVALID;
  boolean return_value = FALSE;

  if (data_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer");
    return FALSE;
  }

  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > SM_CONFIG_MAX_SUB_ID))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid Subs ID %d(Max=%d)", sub, SM_CONFIG_MAX_SUB_ID);
    return FALSE;
  }


  /* Update the write through cache before issuing the NV write */
  return_value = sm_nv_efs_reg_set_cached_value(item, data_size, data_ptr, sub);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SM_NV_EFS: NV EFS file write to cache for item = %d(status=%u sub=%d)",
        item, (uint8)return_value, sub);
#ifdef FEATURE_GNSS_NO_NV_WRITES
  #error code not present
#else
  status = cgps_nv_efs_reg_item_async_write_subsid(
     item,
     data_size, data_ptr,
     CGPS_NV_EFS_SM_MIN_ITEM,
     CGPS_NV_EFS_SM_MAX_ITEM,
     sm_nv_reg_items,
     (cgps_nv_efs_sub_id_type)sub);

#endif /* FEATURE_GNSS_NO_NV_WRITES */

  if (status == CGPS_NV_EFS_REG_RW_STAT_OK)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS file write succeeded for item=%d(sub=%d)", item, sub);
    return_value = TRUE;
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS file write failed for item=%d(sub=%d)", item, sub);
  }

  return return_value;


#else
  return TRUE;
#endif


}



