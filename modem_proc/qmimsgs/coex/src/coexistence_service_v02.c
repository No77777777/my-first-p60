/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        C O E X I S T E N C E _ S E R V I C E _ V 0 2  . C

GENERAL DESCRIPTION
  This is the file which defines the coex service Data structures.

  Copyright (c) 2013-2019 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.


  $Header: //components/rel/qmimsgs.mpss/4.6.9/coex/src/coexistence_service_v02.c#2 $
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.14.7 
   It was generated on: Thu Jul 25 2019 (Spin 0)
   From IDL File: coexistence_service_v02.idl */

#include "stdint.h"
#include "qmi_idl_lib_internal.h"
#include "coexistence_service_v02.h"
#include "common_v01.h"


/*Type Definitions*/
static const uint8_t coex_tech_band_type_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_band_type_v02, frequency),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_band_type_v02, bandwidth_and_direction),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_antenna_state_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_antenna_state_v02, adv_notice),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_antenna_state_v02, duration),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_tech_sync_state_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sync_state_v02, id),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sync_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_tech_state_update_req_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_tech_state_update_req_v02, id),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_lte_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_lte_state_v02, band_info),
  COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_lte_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_lte_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_lte_state_v02, sync_state_token),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_lte_state_v02, data),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_lte_state_v02, tdd_info),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_tdscdma_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_tdscdma_state_v02, band_info),
  COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_tdscdma_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_tdscdma_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_tdscdma_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_gsm_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_gsm_state_v02, band_info),
  COEX_WWAN_GSM_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_gsm_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_gsm_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_gsm_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_onex_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_onex_state_v02, band_info),
  COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_onex_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_onex_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_onex_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_hdr_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_hdr_state_v02, band_info),
  COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_hdr_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_hdr_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_hdr_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_wcdma_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_wcdma_state_v02, band_info),
  COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_wcdma_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_wcdma_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_wcdma_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wlan_wifi_connection_info_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wlan_wifi_connection_info_v02, band_info),
  COEX_WLAN_WIFI_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wlan_wifi_connection_info_v02, band_info) - QMI_IDL_OFFSET8(coex_wlan_wifi_connection_info_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wlan_wifi_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wlan_wifi_state_v02, high_priority_events_list),
  COEX_WLAN_WIFI_MAX_EVENTS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wlan_wifi_state_v02, high_priority_events_list) - QMI_IDL_OFFSET8(coex_wlan_wifi_state_v02, high_priority_events_list_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wlan_wifi_state_v02, connections_list),
  COEX_WLAN_WIFI_MAX_CONNS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wlan_wifi_state_v02, connections_list) - QMI_IDL_OFFSET8(coex_wlan_wifi_state_v02, connections_list_len),
  QMI_IDL_TYPE88(0, 10),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_wifi_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_diversity_antenna_state_data_v02[] = {
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_diversity_antenna_state_v02, data),
  QMI_IDL_TYPE88(0, 1),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_diversity_antenna_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_metrics_lte_sinr_params_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_params_v02, filter_alpha),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_metrics_lte_sinr_stats_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_stats_v02, sinr_system),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_stats_v02, sinr_bt_only),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_stats_v02, sinr_wifi_only),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_stats_v02, sinr_bt_and_wifi),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_stats_v02, sinr_lte_only),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_metrics_lte_sinr_report_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_report_v02, estatus),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_report_v02, collection_duration),

  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_metrics_lte_sinr_report_v02, stats),
  QMI_IDL_TYPE88(0, 14),
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_conflict_band_range_type_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_conflict_band_range_type_v02, tech_type),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_conflict_band_range_type_v02, frequency_range_start),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_conflict_band_range_type_v02, frequency_range_stop),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wlan_conflict_band_range_type_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_conflict_band_range_type_v02, tech_type),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_conflict_band_range_type_v02, frequency_range_start),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_conflict_band_range_type_v02, frequency_range_stop),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_conflict_definition_type_data_v02[] = {
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_conflict_definition_type_v02, wwan),
  QMI_IDL_TYPE88(0, 16),
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_conflict_definition_type_v02, wlan),
  QMI_IDL_TYPE88(0, 17),
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_conflict_definition_type_v02, wlan_coex_policy),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_conflict_definition_type_v02, wlan_policy_coex_wwan_rx_imd_protection_params),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_conflict_params_report_type_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_conflict_params_report_type_v02, request_estatus),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_conflict_params_report_type_v02, victim_tbl_offset),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_conflict_params_report_type_v02, victim_tbl_complete_size),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_conflict_params_report_type_v02, partial_victim_table),
  COEX_NV_MSG_VICTIM_TABLE_MAX_ENTRIES_V02,
  QMI_IDL_OFFSET8(coex_conflict_params_report_type_v02, partial_victim_table) - QMI_IDL_OFFSET8(coex_conflict_params_report_type_v02, partial_victim_table_len),
  QMI_IDL_TYPE88(0, 18),
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_lte_carrier_type_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_lte_carrier_type_v02, bands),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_lte_carrier_type_v02, data),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_lte_carrier_type_v02, tdd_info),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_lte_ca_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_lte_ca_state_v02, band_info),
  COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_lte_ca_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_lte_ca_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_lte_ca_state_v02, sync_state_token),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_lte_ca_state_v02, cc_info),
  COEX_WWAN_LTE_STATE_MAX_CARRIERS_V02,
  QMI_IDL_OFFSET8(coex_wwan_lte_ca_state_v02, cc_info) - QMI_IDL_OFFSET8(coex_wwan_lte_ca_state_v02, cc_info_len),
  QMI_IDL_TYPE88(0, 20),
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_active_conflict_type_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_active_conflict_type_v02, index),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_active_conflict_type_v02, wan_carrier),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_rt_wlan_active_conflict_type_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_active_conflict_type_v02, wlan_coex_policy),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_active_conflict_type_v02, wwan_tech_and_carrier),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_rt_wlan_channel_avoidance_type_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_channel_avoidance_type_v02, wan_frequency_range),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_channel_avoidance_type_v02, wlan_frequency_range_start),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_channel_avoidance_type_v02, wlan_frequency_range_stop),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_rat_id_mapping_data_v02[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_rat_id_mapping_v02, tech_and_carrier),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_rat_id_mapping_v02, rat_id),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_rt_wlan_active_conflict_type_ex_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_active_conflict_type_ex_v02, wlan_coex_policy),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_active_conflict_type_ex_v02, wwan_tech_and_carrier),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_rt_wlan_active_conflict_type_ex_v02, wlan_frequency),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wwan_nr5g_state_data_v02[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(coex_wwan_nr5g_state_v02, band_info),
  COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(coex_wwan_nr5g_state_v02, band_info) - QMI_IDL_OFFSET8(coex_wwan_nr5g_state_v02, band_info_len),
  QMI_IDL_TYPE88(0, 0),
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_wwan_nr5g_state_v02, sync_state_token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_tech_sleep_wakeup_msg_data_v02[] = {
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sleep_wakeup_msg_v02, tech),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sleep_wakeup_msg_v02, going_to_sleep),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sleep_wakeup_msg_v02, duration),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sleep_wakeup_msg_v02, tech_critical),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sleep_wakeup_msg_v02, rssi),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(coex_tech_sleep_wakeup_msg_v02, wlan_band),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wlan_rtsar_pwr_report_type_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_rtsar_pwr_report_type_v02, tx_pwr_2_4_ghz_dbm10),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_rtsar_pwr_report_type_v02, tx_pwr_5_0_ghz_dbm10),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_wlan_rtsar_pwr_limit_type_data_v02[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_rtsar_pwr_limit_type_v02, sar_plimit_2_4_ghz_dbm10),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_wlan_rtsar_pwr_limit_type_v02, sar_plimit_5_0_ghz_dbm10),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t coex_tech_cdrx_state_info_data_v02[] = {
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(coex_tech_cdrx_state_info_v02, cdrx_state),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_cdrx_state_info_v02, on_duration_timer),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_cdrx_state_info_v02, short_drx_timer),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_cdrx_state_info_v02, long_drx_cycle),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_cdrx_state_info_v02, drx_inactivity_timer),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_cdrx_state_info_v02, drx_short_cycle_timer),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(coex_tech_cdrx_state_info_v02, drx_start_offset),

  QMI_IDL_FLAG_END_VALUE
};

/*Message Definitions*/
static const uint8_t qmi_coex_tech_sync_req_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, tech) - QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, tech_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, tech),
  COEX_MAX_TECHS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, tech) - QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, tech_len),
  QMI_IDL_TYPE88(0, 2),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, volte_cdrx_sync_token) - QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, volte_cdrx_sync_token_valid)),
  0x11,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_tech_sync_req_msg_v02, volte_cdrx_sync_token)
};

static const uint8_t qmi_coex_tech_sync_resp_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_sync_resp_msg_v02, tech) - QMI_IDL_OFFSET8(qmi_coex_tech_sync_resp_msg_v02, tech_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_tech_sync_resp_msg_v02, tech),
  COEX_MAX_TECHS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(qmi_coex_tech_sync_resp_msg_v02, tech) - QMI_IDL_OFFSET8(qmi_coex_tech_sync_resp_msg_v02, tech_len),
  QMI_IDL_TYPE88(0, 2)
};

static const uint8_t qmi_coex_tech_sync_update_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_sync_update_ind_msg_v02, tech) - QMI_IDL_OFFSET8(qmi_coex_tech_sync_update_ind_msg_v02, tech_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_tech_sync_update_ind_msg_v02, tech),
  QMI_IDL_TYPE88(0, 2)
};

static const uint8_t qmi_coex_tech_state_update_req_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_state_update_req_msg_v02, tech_ids) - QMI_IDL_OFFSET8(qmi_coex_tech_state_update_req_msg_v02, tech_ids_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_tech_state_update_req_msg_v02, tech_ids),
  COEX_MAX_TECHS_SUPPORTED_V02,
  QMI_IDL_OFFSET8(qmi_coex_tech_state_update_req_msg_v02, tech_ids) - QMI_IDL_OFFSET8(qmi_coex_tech_state_update_req_msg_v02, tech_ids_len),
  QMI_IDL_TYPE88(0, 3)
};

static const uint8_t qmi_coex_wwan_lte_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_lte_state_ind_msg_v02, lte_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_lte_state_ind_msg_v02, lte_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_lte_state_ind_msg_v02, lte_band_info),
  QMI_IDL_TYPE88(0, 4),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_lte_state_ind_msg_v02, lte_ca_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_lte_state_ind_msg_v02, lte_ca_band_info_valid)),
  0x11,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_lte_state_ind_msg_v02, lte_ca_band_info),
  QMI_IDL_TYPE88(0, 21)
};

static const uint8_t qmi_coex_wwan_tdscdma_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_tdscdma_state_ind_msg_v02, tdscdma_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_tdscdma_state_ind_msg_v02, tdscdma_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_tdscdma_state_ind_msg_v02, tdscdma_band_info),
  QMI_IDL_TYPE88(0, 5)
};

static const uint8_t qmi_coex_wwan_gsm_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_gsm_state_ind_msg_v02, gsm_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_gsm_state_ind_msg_v02, gsm_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_gsm_state_ind_msg_v02, gsm_band_info),
  QMI_IDL_TYPE88(0, 6)
};

static const uint8_t qmi_coex_wwan_onex_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_onex_state_ind_msg_v02, onex_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_onex_state_ind_msg_v02, onex_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_onex_state_ind_msg_v02, onex_band_info),
  QMI_IDL_TYPE88(0, 7)
};

static const uint8_t qmi_coex_wwan_hdr_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_hdr_state_ind_msg_v02, hdr_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_hdr_state_ind_msg_v02, hdr_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_hdr_state_ind_msg_v02, hdr_band_info),
  QMI_IDL_TYPE88(0, 8)
};

static const uint8_t qmi_coex_wwan_wcdma_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_wcdma_state_ind_msg_v02, wcdma_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_wcdma_state_ind_msg_v02, wcdma_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_wcdma_state_ind_msg_v02, wcdma_band_info),
  QMI_IDL_TYPE88(0, 9)
};

static const uint8_t qmi_coex_wlan_wifi_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wlan_wifi_state_ind_msg_v02, wifi_state_info) - QMI_IDL_OFFSET8(qmi_coex_wlan_wifi_state_ind_msg_v02, wifi_state_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wlan_wifi_state_ind_msg_v02, wifi_state_info),
  QMI_IDL_TYPE88(0, 11)
};

/*
 * qmi_coex_wlan_bt_state_ind_msg is empty
 * static const uint8_t qmi_coex_wlan_bt_state_ind_msg_data_v02[] = {
 * };
 */

static const uint8_t qmi_coex_diversity_antenna_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_diversity_antenna_state_ind_msg_v02, state) - QMI_IDL_OFFSET8(qmi_coex_diversity_antenna_state_ind_msg_v02, state_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_diversity_antenna_state_ind_msg_v02, state),
  QMI_IDL_TYPE88(0, 12)
};

static const uint8_t qmi_coex_metrics_lte_sinr_start_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_start_ind_msg_v02, data) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_start_ind_msg_v02, data_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_start_ind_msg_v02, data),
  QMI_IDL_TYPE88(0, 13),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_start_ind_msg_v02, cc_mask) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_start_ind_msg_v02, cc_mask_valid)),
  0x11,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_start_ind_msg_v02, cc_mask)
};

static const uint8_t qmi_coex_metrics_lte_sinr_read_req_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_req_msg_v02, cc_mask) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_req_msg_v02, cc_mask_valid)),
  0x10,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_req_msg_v02, cc_mask)
};

static const uint8_t qmi_coex_metrics_lte_sinr_read_resp_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_resp_msg_v02, report) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_resp_msg_v02, report_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_resp_msg_v02, report),
  QMI_IDL_TYPE88(0, 15),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_resp_msg_v02, cc_id) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_resp_msg_v02, cc_id_valid)),
  0x11,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_read_resp_msg_v02, cc_id)
};

static const uint8_t qmi_coex_metrics_lte_sinr_stop_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_stop_ind_msg_v02, cc_mask) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_stop_ind_msg_v02, cc_mask_valid)),
  0x10,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_sinr_stop_ind_msg_v02, cc_mask)
};

static const uint8_t qmi_coex_wwan_gsm2_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_gsm2_state_ind_msg_v02, gsm2_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_gsm2_state_ind_msg_v02, gsm2_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_gsm2_state_ind_msg_v02, gsm2_band_info),
  QMI_IDL_TYPE88(0, 6)
};

static const uint8_t qmi_coex_wwan_gsm3_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_gsm3_state_ind_msg_v02, gsm3_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_gsm3_state_ind_msg_v02, gsm3_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_gsm3_state_ind_msg_v02, gsm3_band_info),
  QMI_IDL_TYPE88(0, 6)
};

static const uint8_t qmi_coex_conflict_params_req_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_conflict_params_req_msg_v02, victim_tbl_offset) - QMI_IDL_OFFSET8(qmi_coex_conflict_params_req_msg_v02, victim_tbl_offset_valid)),
  0x10,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_conflict_params_req_msg_v02, victim_tbl_offset)
};

static const uint8_t qmi_coex_conflict_params_resp_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_conflict_params_resp_msg_v02, report) - QMI_IDL_OFFSET8(qmi_coex_conflict_params_resp_msg_v02, report_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_conflict_params_resp_msg_v02, report),
  QMI_IDL_TYPE88(0, 19)
};

static const uint8_t qmi_coex_wwan_wcdma2_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_wcdma2_state_ind_msg_v02, wcdma2_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_wcdma2_state_ind_msg_v02, wcdma2_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_wcdma2_state_ind_msg_v02, wcdma2_band_info),
  QMI_IDL_TYPE88(0, 9)
};

static const uint8_t qmi_coex_active_conflict_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, active_conflicts) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, active_conflicts_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, active_conflicts),
  COEX_MAX_ACTIVE_CONFLICTS_V02,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, active_conflicts) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, active_conflicts_len),
  QMI_IDL_TYPE88(0, 22),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts_valid)),
  0x11,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts),
  COEX_MAX_ACTIVE_CONFLICTS_V02,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts_len),
  QMI_IDL_TYPE88(0, 23),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts_ex) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts_ex_valid)),
  0x12,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts_ex),
  COEX_MAX_ACTIVE_CONFLICTS_V02,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts_ex) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_ind_msg_v02, wlan_active_conflicts_ex_len),
  QMI_IDL_TYPE88(0, 26)
};

static const uint8_t qmi_coex_active_conflict_req_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_active_conflict_req_msg_v02, resend_active_conflict_ind) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_req_msg_v02, resend_active_conflict_ind_valid)),
  0x10,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_req_msg_v02, resend_active_conflict_ind),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_active_conflict_req_msg_v02, resend_potential_conflict_ind) - QMI_IDL_OFFSET8(qmi_coex_active_conflict_req_msg_v02, resend_potential_conflict_ind_valid)),
  0x11,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_active_conflict_req_msg_v02, resend_potential_conflict_ind)
};

static const uint8_t qmi_coex_potential_conflict_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_potential_conflict_ind_msg_v02, wlan_potential_conflicts) - QMI_IDL_OFFSET8(qmi_coex_potential_conflict_ind_msg_v02, wlan_potential_conflicts_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_potential_conflict_ind_msg_v02, wlan_potential_conflicts),
  COEX_MAX_CHANNEL_AVOID_CONFLICTS_V02,
  QMI_IDL_OFFSET8(qmi_coex_potential_conflict_ind_msg_v02, wlan_potential_conflicts) - QMI_IDL_OFFSET8(qmi_coex_potential_conflict_ind_msg_v02, wlan_potential_conflicts_len),
  QMI_IDL_TYPE88(0, 24)
};

static const uint8_t qmi_coex_rat_id_update_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, behavior) - QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, behavior_valid)),
  0x10,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, behavior),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, rat_ids) - QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, rat_ids_valid)),
  0x11,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, rat_ids),
  COEX_MAX_RAT_ID_MAPPINGS_V02,
  QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, rat_ids) - QMI_IDL_OFFSET8(qmi_coex_rat_id_update_ind_msg_v02, rat_ids_len),
  QMI_IDL_TYPE88(0, 25)
};

/*
 * qmi_coex_rat_id_update_req_msg is empty
 * static const uint8_t qmi_coex_rat_id_update_req_msg_data_v02[] = {
 * };
 */

static const uint8_t qmi_coex_wwan_nr5g_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_wwan_nr5g_state_ind_msg_v02, nr5g_band_info) - QMI_IDL_OFFSET8(qmi_coex_wwan_nr5g_state_ind_msg_v02, nr5g_band_info_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_wwan_nr5g_state_ind_msg_v02, nr5g_band_info),
  QMI_IDL_TYPE88(0, 27)
};

static const uint8_t qmi_coex_metrics_lte_bler_start_req_msg_data_v02[] = {
  0x01,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_start_req_msg_v02, tb_cnt),

  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_start_req_msg_v02, threshold_err_tb_cnt)
};

static const uint8_t qmi_coex_metrics_lte_bler_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_valid)),
  0x10,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_valid)),
  0x11,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_bt_only) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_bt_only_valid)),
  0x12,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_bt_only),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_bt_only) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_bt_only_valid)),
  0x13,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_bt_only),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_wifi_only) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_wifi_only_valid)),
  0x14,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_wifi_only),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_wifi_only) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_wifi_only_valid)),
  0x15,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_wifi_only),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_bt_wifi) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_bt_wifi_valid)),
  0x16,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_bt_wifi),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_bt_wifi) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_bt_wifi_valid)),
  0x17,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_bt_wifi),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_lte_only) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_lte_only_valid)),
  0x18,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, tb_cnt_lte_only),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_lte_only) - QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_lte_only_valid)),
  0x19,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_metrics_lte_bler_ind_msg_v02, errored_tb_cnt_lte_only)
};

/*
 * qmi_coex_metrics_lte_bler_read_req_msg is empty
 * static const uint8_t qmi_coex_metrics_lte_bler_read_req_msg_data_v02[] = {
 * };
 */

/*
 * qmi_coex_metrics_lte_bler_stop_req_msg is empty
 * static const uint8_t qmi_coex_metrics_lte_bler_stop_req_msg_data_v02[] = {
 * };
 */

static const uint8_t qmi_coex_bt_conn_info_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_bt_conn_info_ind_msg_v02, bt_channel_map) - QMI_IDL_OFFSET8(qmi_coex_bt_conn_info_ind_msg_v02, bt_channel_map_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_bt_conn_info_ind_msg_v02, bt_channel_map),
  COEX_MAX_BT_CHANNEL_MAP_SIZE_V02,
  QMI_IDL_OFFSET8(qmi_coex_bt_conn_info_ind_msg_v02, bt_channel_map) - QMI_IDL_OFFSET8(qmi_coex_bt_conn_info_ind_msg_v02, bt_channel_map_len)
};

static const uint8_t qmi_coex_gnss_crit_demod_state_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_gnss_crit_demod_state_ind_msg_v02, gnss_demod_state)
};

static const uint8_t qmi_coex_tech_sleep_wakeup_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_sleep_wakeup_ind_msg_v02, sleep_wakeup) - QMI_IDL_OFFSET8(qmi_coex_tech_sleep_wakeup_ind_msg_v02, sleep_wakeup_valid)),
  0x10,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_tech_sleep_wakeup_ind_msg_v02, sleep_wakeup),
  QMI_IDL_TYPE88(0, 28)
};

static const uint8_t qmi_coex_2g_wlan_mode_chnl_map_ind_msg_data_v02[] = {
  0x01,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02, wlan_mode_map),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02, channel_map) - QMI_IDL_OFFSET8(qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02, channel_map_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02, channel_map),
  COEX_MAX_WLAN_CONCURRENT_MODES_V02,
  QMI_IDL_OFFSET8(qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02, channel_map) - QMI_IDL_OFFSET8(qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02, channel_map_len)
};

static const uint8_t qmi_coex_rtsar_wlan_cfg_ind_msg_data_v02[] = {
  0x01,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, msg_type),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, reporting_period_msec) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, reporting_period_msec_valid)),
  0x10,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, reporting_period_msec),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_period_msec) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_period_msec_valid)),
  0x11,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_period_msec),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_non_mimo_tx_limits) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_non_mimo_tx_limits_valid)),
  0x12,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_non_mimo_tx_limits),
  COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_non_mimo_tx_limits) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_non_mimo_tx_limits_len),
  QMI_IDL_TYPE88(0, 30),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_mimo_tx_limits) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_mimo_tx_limits_valid)),
  0x13,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_mimo_tx_limits),
  COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_mimo_tx_limits) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_cfg_ind_msg_v02, fallback_mimo_tx_limits_len),
  QMI_IDL_TYPE88(0, 30)
};

static const uint8_t qmi_coex_rtsar_wlan_info_ind_msg_data_v02[] = {
  0x01,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, msg_type),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, mimo_mode_2g) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, mimo_mode_2g_valid)),
  0x10,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, mimo_mode_2g),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, mimo_mode_5g) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, mimo_mode_5g_valid)),
  0x11,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, mimo_mode_5g),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, num_tx_chains) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, num_tx_chains_valid)),
  0x12,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_info_ind_msg_v02, num_tx_chains)
};

static const uint8_t qmi_coex_rtsar_wlan_pwr_report_ind_msg_data_v02[] = {
  0x01,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, tx_chain),
  COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, tx_chain) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, tx_chain_len),
  QMI_IDL_TYPE88(0, 29),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, seq_num) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, seq_num_valid)),
  0x10,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, seq_num),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, duration) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, duration_valid)),
  0x11,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02, duration)
};

static const uint8_t qmi_coex_rtsar_wlan_pwr_limit_ind_msg_data_v02[] = {
  0x01,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, non_mimo_tx_limits),
  COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, non_mimo_tx_limits) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, non_mimo_tx_limits_len),
  QMI_IDL_TYPE88(0, 30),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, mimo_tx_limits) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, mimo_tx_limits_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, mimo_tx_limits),
  COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, mimo_tx_limits) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02, mimo_tx_limits_len),
  QMI_IDL_TYPE88(0, 30)
};

static const uint8_t qmi_coex_tech_volte_cdrx_state_ind_msg_data_v02[] = {
  0x01,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, tech),

  0x02,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, volte_cdrx_sync_token),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, volte_state) - QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, volte_state_valid)),
  0x10,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, volte_state),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, cdrx_info) - QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, cdrx_info_valid)),
  0x11,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(qmi_coex_tech_volte_cdrx_state_ind_msg_v02, cdrx_info),
  QMI_IDL_TYPE88(0, 31)
};

static const uint8_t qmi_coex_rtsar_wlan_pwr_report_req_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_req_msg_v02, seq_num) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_req_msg_v02, seq_num_valid)),
  0x10,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_req_msg_v02, seq_num)
};

static const uint8_t qmi_coex_rtsar_wlan_pwr_report_ack_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ack_ind_msg_v02, seq_num) - QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ack_ind_msg_v02, seq_num_valid)),
  0x10,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_wlan_pwr_report_ack_ind_msg_v02, seq_num)
};

static const uint8_t qmi_coex_rtsar_bt_cfg_ind_msg_data_v02[] = {
  0x01,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, msg_type),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, reporting_period_msec) - QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, reporting_period_msec_valid)),
  0x10,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, reporting_period_msec),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, fallback_period_msec) - QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, fallback_period_msec_valid)),
  0x11,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, fallback_period_msec),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, fallback_limit_idx) - QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, fallback_limit_idx_valid)),
  0x12,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_cfg_ind_msg_v02, fallback_limit_idx)
};

static const uint8_t qmi_coex_rtsar_bt_pwr_limit_ind_msg_data_v02[] = {
  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02, power_limit_idx) - QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02, power_limit_idx_valid)),
  0x10,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02, power_limit_idx),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02, sar_plimit_dbm10) - QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02, sar_plimit_dbm10_valid)),
  0x11,
   QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02, sar_plimit_dbm10)
};

/* Type Table */
static const qmi_idl_type_table_entry  coex_type_table_v02[] = {
  {sizeof(coex_tech_band_type_v02), coex_tech_band_type_data_v02},
  {sizeof(coex_antenna_state_v02), coex_antenna_state_data_v02},
  {sizeof(coex_tech_sync_state_v02), coex_tech_sync_state_data_v02},
  {sizeof(coex_tech_state_update_req_v02), coex_tech_state_update_req_data_v02},
  {sizeof(coex_wwan_lte_state_v02), coex_wwan_lte_state_data_v02},
  {sizeof(coex_wwan_tdscdma_state_v02), coex_wwan_tdscdma_state_data_v02},
  {sizeof(coex_wwan_gsm_state_v02), coex_wwan_gsm_state_data_v02},
  {sizeof(coex_wwan_onex_state_v02), coex_wwan_onex_state_data_v02},
  {sizeof(coex_wwan_hdr_state_v02), coex_wwan_hdr_state_data_v02},
  {sizeof(coex_wwan_wcdma_state_v02), coex_wwan_wcdma_state_data_v02},
  {sizeof(coex_wlan_wifi_connection_info_v02), coex_wlan_wifi_connection_info_data_v02},
  {sizeof(coex_wlan_wifi_state_v02), coex_wlan_wifi_state_data_v02},
  {sizeof(coex_diversity_antenna_state_v02), coex_diversity_antenna_state_data_v02},
  {sizeof(coex_metrics_lte_sinr_params_v02), coex_metrics_lte_sinr_params_data_v02},
  {sizeof(coex_metrics_lte_sinr_stats_v02), coex_metrics_lte_sinr_stats_data_v02},
  {sizeof(coex_metrics_lte_sinr_report_v02), coex_metrics_lte_sinr_report_data_v02},
  {sizeof(coex_wwan_conflict_band_range_type_v02), coex_wwan_conflict_band_range_type_data_v02},
  {sizeof(coex_wlan_conflict_band_range_type_v02), coex_wlan_conflict_band_range_type_data_v02},
  {sizeof(coex_conflict_definition_type_v02), coex_conflict_definition_type_data_v02},
  {sizeof(coex_conflict_params_report_type_v02), coex_conflict_params_report_type_data_v02},
  {sizeof(coex_wwan_lte_carrier_type_v02), coex_wwan_lte_carrier_type_data_v02},
  {sizeof(coex_wwan_lte_ca_state_v02), coex_wwan_lte_ca_state_data_v02},
  {sizeof(coex_active_conflict_type_v02), coex_active_conflict_type_data_v02},
  {sizeof(coex_rt_wlan_active_conflict_type_v02), coex_rt_wlan_active_conflict_type_data_v02},
  {sizeof(coex_rt_wlan_channel_avoidance_type_v02), coex_rt_wlan_channel_avoidance_type_data_v02},
  {sizeof(coex_rat_id_mapping_v02), coex_rat_id_mapping_data_v02},
  {sizeof(coex_rt_wlan_active_conflict_type_ex_v02), coex_rt_wlan_active_conflict_type_ex_data_v02},
  {sizeof(coex_wwan_nr5g_state_v02), coex_wwan_nr5g_state_data_v02},
  {sizeof(coex_tech_sleep_wakeup_msg_v02), coex_tech_sleep_wakeup_msg_data_v02},
  {sizeof(coex_wlan_rtsar_pwr_report_type_v02), coex_wlan_rtsar_pwr_report_type_data_v02},
  {sizeof(coex_wlan_rtsar_pwr_limit_type_v02), coex_wlan_rtsar_pwr_limit_type_data_v02},
  {sizeof(coex_tech_cdrx_state_info_v02), coex_tech_cdrx_state_info_data_v02}
};

/* Message Table */
static const qmi_idl_message_table_entry coex_message_table_v02[] = {
  {sizeof(qmi_coex_tech_sync_req_msg_v02), qmi_coex_tech_sync_req_msg_data_v02},
  {sizeof(qmi_coex_tech_sync_resp_msg_v02), qmi_coex_tech_sync_resp_msg_data_v02},
  {sizeof(qmi_coex_tech_sync_update_ind_msg_v02), qmi_coex_tech_sync_update_ind_msg_data_v02},
  {sizeof(qmi_coex_tech_state_update_req_msg_v02), qmi_coex_tech_state_update_req_msg_data_v02},
  {sizeof(qmi_coex_wwan_lte_state_ind_msg_v02), qmi_coex_wwan_lte_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wwan_tdscdma_state_ind_msg_v02), qmi_coex_wwan_tdscdma_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wwan_gsm_state_ind_msg_v02), qmi_coex_wwan_gsm_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wwan_onex_state_ind_msg_v02), qmi_coex_wwan_onex_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wwan_hdr_state_ind_msg_v02), qmi_coex_wwan_hdr_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wwan_wcdma_state_ind_msg_v02), qmi_coex_wwan_wcdma_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wlan_wifi_state_ind_msg_v02), qmi_coex_wlan_wifi_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wlan_bt_state_ind_msg_v02), 0},
  {sizeof(qmi_coex_diversity_antenna_state_ind_msg_v02), qmi_coex_diversity_antenna_state_ind_msg_data_v02},
  {sizeof(qmi_coex_metrics_lte_sinr_start_ind_msg_v02), qmi_coex_metrics_lte_sinr_start_ind_msg_data_v02},
  {sizeof(qmi_coex_metrics_lte_sinr_read_req_msg_v02), qmi_coex_metrics_lte_sinr_read_req_msg_data_v02},
  {sizeof(qmi_coex_metrics_lte_sinr_read_resp_msg_v02), qmi_coex_metrics_lte_sinr_read_resp_msg_data_v02},
  {sizeof(qmi_coex_metrics_lte_sinr_stop_ind_msg_v02), qmi_coex_metrics_lte_sinr_stop_ind_msg_data_v02},
  {sizeof(qmi_coex_wwan_gsm2_state_ind_msg_v02), qmi_coex_wwan_gsm2_state_ind_msg_data_v02},
  {sizeof(qmi_coex_wwan_gsm3_state_ind_msg_v02), qmi_coex_wwan_gsm3_state_ind_msg_data_v02},
  {sizeof(qmi_coex_conflict_params_req_msg_v02), qmi_coex_conflict_params_req_msg_data_v02},
  {sizeof(qmi_coex_conflict_params_resp_msg_v02), qmi_coex_conflict_params_resp_msg_data_v02},
  {sizeof(qmi_coex_wwan_wcdma2_state_ind_msg_v02), qmi_coex_wwan_wcdma2_state_ind_msg_data_v02},
  {sizeof(qmi_coex_active_conflict_ind_msg_v02), qmi_coex_active_conflict_ind_msg_data_v02},
  {sizeof(qmi_coex_active_conflict_req_msg_v02), qmi_coex_active_conflict_req_msg_data_v02},
  {sizeof(qmi_coex_potential_conflict_ind_msg_v02), qmi_coex_potential_conflict_ind_msg_data_v02},
  {sizeof(qmi_coex_rat_id_update_ind_msg_v02), qmi_coex_rat_id_update_ind_msg_data_v02},
  {sizeof(qmi_coex_rat_id_update_req_msg_v02), 0},
  {sizeof(qmi_coex_wwan_nr5g_state_ind_msg_v02), qmi_coex_wwan_nr5g_state_ind_msg_data_v02},
  {sizeof(qmi_coex_metrics_lte_bler_start_req_msg_v02), qmi_coex_metrics_lte_bler_start_req_msg_data_v02},
  {sizeof(qmi_coex_metrics_lte_bler_ind_msg_v02), qmi_coex_metrics_lte_bler_ind_msg_data_v02},
  {sizeof(qmi_coex_metrics_lte_bler_read_req_msg_v02), 0},
  {sizeof(qmi_coex_metrics_lte_bler_stop_req_msg_v02), 0},
  {sizeof(qmi_coex_bt_conn_info_ind_msg_v02), qmi_coex_bt_conn_info_ind_msg_data_v02},
  {sizeof(qmi_coex_gnss_crit_demod_state_ind_msg_v02), qmi_coex_gnss_crit_demod_state_ind_msg_data_v02},
  {sizeof(qmi_coex_tech_sleep_wakeup_ind_msg_v02), qmi_coex_tech_sleep_wakeup_ind_msg_data_v02},
  {sizeof(qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02), qmi_coex_2g_wlan_mode_chnl_map_ind_msg_data_v02},
  {sizeof(qmi_coex_rtsar_wlan_cfg_ind_msg_v02), qmi_coex_rtsar_wlan_cfg_ind_msg_data_v02},
  {sizeof(qmi_coex_rtsar_wlan_info_ind_msg_v02), qmi_coex_rtsar_wlan_info_ind_msg_data_v02},
  {sizeof(qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02), qmi_coex_rtsar_wlan_pwr_report_ind_msg_data_v02},
  {sizeof(qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02), qmi_coex_rtsar_wlan_pwr_limit_ind_msg_data_v02},
  {sizeof(qmi_coex_tech_volte_cdrx_state_ind_msg_v02), qmi_coex_tech_volte_cdrx_state_ind_msg_data_v02},
  {sizeof(qmi_coex_rtsar_wlan_pwr_report_req_msg_v02), qmi_coex_rtsar_wlan_pwr_report_req_msg_data_v02},
  {sizeof(qmi_coex_rtsar_wlan_pwr_report_ack_ind_msg_v02), qmi_coex_rtsar_wlan_pwr_report_ack_ind_msg_data_v02},
  {sizeof(qmi_coex_rtsar_bt_cfg_ind_msg_v02), qmi_coex_rtsar_bt_cfg_ind_msg_data_v02},
  {sizeof(qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02), qmi_coex_rtsar_bt_pwr_limit_ind_msg_data_v02}
};

/* Range Table */
/* No Ranges Defined in IDL */

/* Predefine the Type Table Object */
static const qmi_idl_type_table_object coex_qmi_idl_type_table_object_v02;

/*Referenced Tables Array*/
static const qmi_idl_type_table_object *coex_qmi_idl_type_table_object_referenced_tables_v02[] =
{&coex_qmi_idl_type_table_object_v02, &common_qmi_idl_type_table_object_v01};

/*Type Table Object*/
static const qmi_idl_type_table_object coex_qmi_idl_type_table_object_v02 = {
  sizeof(coex_type_table_v02)/sizeof(qmi_idl_type_table_entry ),
  sizeof(coex_message_table_v02)/sizeof(qmi_idl_message_table_entry),
  1,
  coex_type_table_v02,
  coex_message_table_v02,
  coex_qmi_idl_type_table_object_referenced_tables_v02,
  NULL
};

/*Arrays of service_message_table_entries for commands, responses and indications*/
static const qmi_idl_service_message_table_entry coex_service_indication_messages_v02[] = {
  {QMI_COEX_TECH_SYNC_REQ_V02, QMI_IDL_TYPE16(0, 0), 56},
  {QMI_COEX_TECH_SYNC_RESP_V02, QMI_IDL_TYPE16(0, 1), 52},
  {QMI_COEX_TECH_SYNC_UPDATE_IND_V02, QMI_IDL_TYPE16(0, 2), 5},
  {QMI_COEX_TECH_STATE_UPDATE_REQ_V02, QMI_IDL_TYPE16(0, 3), 28},
  {QMI_COEX_WWAN_LTE_STATE_IND_V02, QMI_IDL_TYPE16(0, 4), 139},
  {QMI_COEX_WWAN_TDSCDMA_STATE_IND_V02, QMI_IDL_TYPE16(0, 5), 37},
  {QMI_COEX_WWAN_GSM_STATE_IND_V02, QMI_IDL_TYPE16(0, 6), 69},
  {QMI_COEX_WWAN_ONEX_STATE_IND_V02, QMI_IDL_TYPE16(0, 7), 37},
  {QMI_COEX_WWAN_HDR_STATE_IND_V02, QMI_IDL_TYPE16(0, 8), 37},
  {QMI_COEX_WWAN_WCDMA_STATE_IND_V02, QMI_IDL_TYPE16(0, 9), 37},
  {QMI_COEX_WLAN_WIFI_STATE_IND_V02, QMI_IDL_TYPE16(0, 10), 106},
  {QMI_COEX_WLAN_BT_STATE_IND_V02, QMI_IDL_TYPE16(0, 11), 0},
  {QMI_COEX_DIVERSITY_ANTENNA_STATE_IND_V02, QMI_IDL_TYPE16(0, 12), 8},
  {QMI_COEX_METRICS_LTE_SINR_START_IND_V02, QMI_IDL_TYPE16(0, 13), 8},
  {QMI_COEX_METRICS_LTE_SINR_READ_REQ_V02, QMI_IDL_TYPE16(0, 14), 4},
  {QMI_COEX_METRICS_LTE_SINR_READ_RESP_V02, QMI_IDL_TYPE16(0, 15), 14},
  {QMI_COEX_METRICS_LTE_SINR_STOP_IND_V02, QMI_IDL_TYPE16(0, 16), 4},
  {QMI_COEX_WWAN_GSM2_STATE_IND_V02, QMI_IDL_TYPE16(0, 17), 69},
  {QMI_COEX_WWAN_GSM3_STATE_IND_V02, QMI_IDL_TYPE16(0, 18), 69},
  {QMI_COEX_CONFLICT_PARAMS_REQ_V02, QMI_IDL_TYPE16(0, 19), 4},
  {QMI_COEX_CONFLICT_PARAMS_RESP_V02, QMI_IDL_TYPE16(0, 20), 37},
  {QMI_COEX_WWAN_WCDMA2_STATE_IND_V02, QMI_IDL_TYPE16(0, 21), 37},
  {QMI_COEX_ACTIVE_CONFLICT_IND_V02, QMI_IDL_TYPE16(0, 22), 92},
  {QMI_COEX_ACTIVE_CONFLICT_REQ_V02, QMI_IDL_TYPE16(0, 23), 8},
  {QMI_COEX_POTENTIAL_CONFLICT_IND_V02, QMI_IDL_TYPE16(0, 24), 64},
  {QMI_COEX_RAT_ID_UPDATE_IND_V02, QMI_IDL_TYPE16(0, 25), 40},
  {QMI_COEX_RAT_ID_UPDATE_REQ_V02, QMI_IDL_TYPE16(0, 26), 0},
  {QMI_COEX_WWAN_LTE2_STATE_IND_V02, QMI_IDL_TYPE16(0, 4), 139},
  {QMI_COEX_METRICS_LTE2_SINR_START_IND_V02, QMI_IDL_TYPE16(0, 13), 8},
  {QMI_COEX_METRICS_LTE2_SINR_READ_REQ_V02, QMI_IDL_TYPE16(0, 14), 4},
  {QMI_COEX_METRICS_LTE2_SINR_READ_RESP_V02, QMI_IDL_TYPE16(0, 15), 14},
  {QMI_COEX_METRICS_LTE2_SINR_STOP_IND_V02, QMI_IDL_TYPE16(0, 16), 4},
  {QMI_COEX_METRICS_LTE_BLER_START_REQ_V02, QMI_IDL_TYPE16(0, 28), 14},
  {QMI_COEX_METRICS_LTE_BLER_IND_V02, QMI_IDL_TYPE16(0, 29), 70},
  {QMI_COEX_METRICS_LTE_BLER_READ_REQ_V02, QMI_IDL_TYPE16(0, 30), 0},
  {QMI_COEX_METRICS_LTE_BLER_STOP_REQ_V02, QMI_IDL_TYPE16(0, 31), 0},
  {QMI_COEX_METRICS_LTE2_BLER_START_REQ_V02, QMI_IDL_TYPE16(0, 28), 14},
  {QMI_COEX_METRICS_LTE2_BLER_IND_V02, QMI_IDL_TYPE16(0, 29), 70},
  {QMI_COEX_METRICS_LTE2_BLER_READ_REQ_V02, QMI_IDL_TYPE16(0, 30), 0},
  {QMI_COEX_METRICS_LTE2_BLER_STOP_REQ_V02, QMI_IDL_TYPE16(0, 31), 0},
  {QMI_COEX_WWAN_NR5G_STATE_IND_V02, QMI_IDL_TYPE16(0, 27), 37},
  {QMI_COEX_BT_CONN_INFO_IND_V02, QMI_IDL_TYPE16(0, 32), 14},
  {QMI_COEX_WWAN_NR5G2_STATE_IND_V02, QMI_IDL_TYPE16(0, 27), 37},
  {QMI_COEX_GNSS_CRIT_DEMOD_STATE_IND_V02, QMI_IDL_TYPE16(0, 33), 4},
  {QMI_COEX_TECH_SLEEP_WAKEUP_IND_V02, QMI_IDL_TYPE16(0, 34), 24},
  {QMI_COEX_2G_WLAN_MODE_CHNL_MAP_IND_V02, QMI_IDL_TYPE16(0, 35), 27},
  {QMI_COEX_RTSAR_WLAN_CFG_IND_V02, QMI_IDL_TYPE16(0, 36), 41},
  {QMI_COEX_RTSAR_WLAN_INFO_IND_V02, QMI_IDL_TYPE16(0, 37), 25},
  {QMI_COEX_RTSAR_WLAN_PWR_REPORT_IND_V02, QMI_IDL_TYPE16(0, 38), 24},
  {QMI_COEX_RTSAR_WLAN_PWR_LIMIT_IND_V02, QMI_IDL_TYPE16(0, 39), 24},
  {QMI_COEX_TECH_VOLTE_CDRX_STATE_IND_V02, QMI_IDL_TYPE16(0, 40), 37},
  {QMI_COEX_RTSAR_WLAN_PWR_REPORT_REQ_V02, QMI_IDL_TYPE16(0, 41), 5},
  {QMI_COEX_RTSAR_WLAN_PWR_REPORT_ACK_IND_V02, QMI_IDL_TYPE16(0, 42), 5},
  {QMI_COEX_RTSAR_BT_CFG_IND_V02, QMI_IDL_TYPE16(0, 43), 21},
  {QMI_COEX_RTSAR_BT_PWR_LIMIT_IND_V02, QMI_IDL_TYPE16(0, 44), 9}
};

/*Service Object*/
struct qmi_idl_service_object coex_qmi_idl_service_object_v02 = {
  0x06,
  0x02,
  0x22,
  139,
  { 0,
    0,
    sizeof(coex_service_indication_messages_v02)/sizeof(qmi_idl_service_message_table_entry) },
  { NULL, NULL, coex_service_indication_messages_v02},
  &coex_qmi_idl_type_table_object_v02,
  0x10,
  NULL
};

/* Service Object Accessor */
qmi_idl_service_object_type coex_get_service_object_internal_v02
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version ){
  if ( COEX_V02_IDL_MAJOR_VERS != idl_maj_version || COEX_V02_IDL_MINOR_VERS != idl_min_version
       || COEX_V02_IDL_TOOL_VERS != library_version)
  {
    return NULL;
  }
  return (qmi_idl_service_object_type)&coex_qmi_idl_service_object_v02;
}

