#ifndef _POLICYMAN_DIAG_H_
#define _POLICYMAN_DIAG_H_

/**
  @file policyman_diag.h

  @brief Policy Manager diag message definitions.
*/

/*
    Copyright (c) 2014,2016 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_diag.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include <sys/types.h>
#include "comdef.h"    /* Definition for basic types and macros */
#include "log.h"
#include "event_defs.h"
#include "log_codes.h"

#include "mre_diag.h"
#include "policyman_i.h"
#include "sys.h"
#include "cm.h"
#include "mmgsdilib.h"
#include "mmgsdisessionlib.h"
#include "simlock_common.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#if defined(T_WINNT) || defined (FEATURE_WINCE)
#error code not present
#endif

/*-------- LOG_PACKET_NEW --------*/
#define LOG_PACKET_NEW(log_packet_id, log_packet_struct_type) \
  (log_packet_struct_type *)log_alloc_ex( log_packet_id, \
                                          sizeof(log_packet_struct_type) )

/*---------------------------------------------------------------------------
  EVENT_PM_RAT_CHANGE structure
---------------------------------------------------------------------------*/

// Event definition EVENT_PM_RAT_CHANGE  0xA72==2674
// Comes from: core/api/services/event_defs.h
//#define EVENT_PM_RAT_CHANGE  0xA72

/**
@brief  Event for a change in the rat capability.

Details the new rat capability for an associated subscription ID.
*/
typedef PACK(struct) {
  uint32   ratMask;  ///< rat capability
  int8     subs;     ///< subscription id, maps to sys_modem_as_id_e_type
} policyman_diag_event_rat_cap_t;

/*---------------------------------------------------------------------------
  EVENT_PM_SCAN_OPT structure
---------------------------------------------------------------------------*/

/**
@brief  Event for a change in the scan optimization setting.

Details the 3GPP and 3GPP2 scan optimization setting.
*/
typedef PACK(struct) {
  uint8  tgpp_scan_opt; ///< 3GPP scan optimization
  uint8  tgpp2_scan_opt; ///< 3GPP2 scan optimization
} policyman_diag_event_scan_opt_t;

/*---------------------------------------------------------------------------
  EVENT_PM_RAT_ORDER structure
---------------------------------------------------------------------------*/

/**
@brief  Event for a change in rat acquisition order.

Details the rat acquisition order to be used for scanning and camping.
*/
typedef PACK(struct) {
  uint8  rat_order_table_len;   ///< max number of entries in rat_order_entries[]
  uint8  num_rat_order_entries; ///< number of entries in rat_order_entries[]
  uint8  rat_order_entries[10]; ///< ordered list for rat acquisition
} policyman_diag_event_rat_order_t;

/*---------------------------------------------------------------------------
  EVENT_PM_VOICE_DOMAIN structure
---------------------------------------------------------------------------*/

/**
@brief  Event for a change in voice domain preference.

Details the voice domain preference and whether this is a mandatory setting.
*/
typedef PACK(struct) {
  int8    voice_domain;  ///< voice domain preference, maps to sys_voice_domain_pref_e_type
  boolean mandatory;     ///< whether voice domain can be overwritten by client
} policyman_diag_event_voice_domain_pref_t;

/*---------------------------------------------------------------------------
  EVENT_PM_RF_BANDS structure
---------------------------------------------------------------------------*/

/**
@brief  Event for a change in the rf bands.

Details the rf bands to be used across all RATs.
*/
typedef PACK(struct) {
  uint32  lte_band_count;     ///< number of lte band groups filled in below (1 or 4)
  uint64  gw_bands;           ///< mask of the gw bands to use
  uint64  lte_bands_1_64;     ///< mask of first 64 possible LTE bands
  uint64  lte_bands_65_128;   ///< mask of second 64 possible LTE bands
  uint64  lte_bands_129_192;  ///< mask of third 64 possible LTE bands
  uint64  lte_bands_193_256;  ///< mask of fourth 64 possible LTE bands
  uint64  tds_bands;          ///< mask of TDS bands to use
} policyman_diag_event_rf_bands_t;

/*---------------------------------------------------------------------------
  EVENT_PM_FULL_RAT_EVAL structure
---------------------------------------------------------------------------*/

/**
@brief  Event for an evaulation of full rat mode.

Details determination of whether a subscription should enter full rat mode.
*/
typedef PACK(struct) {
  int8     subs;                   ///< subscription id,  maps to sys_modem_as_id_e_type
  boolean  conditions_met;         ///< whether subscription should enter full rat mode
  boolean  have_location;          ///< location available on any subscription
  boolean  timer_expired;          ///< oos timer reached interval
  boolean  oos_scan_cnt_exceeded;  ///< acq fail counts exceeded setting
  uint32   timer_interval;         ///< length of oos timer
} policyman_diag_event_full_rat_mode_eval_t;

/*---------------------------------------------------------------------------
  EVENT_PM_CLIENT_REQUEST structure
---------------------------------------------------------------------------*/

typedef PACK(struct) {
  uint8   itemId;  // maps to policyman_item_id_t
  uint8   subsId;  // maps to sys_modem_as_id_e_type
  size_t  seqNo;   // Sequence number of the item
} policyman_diag_item_info_t;

#define POLICYMAN_DIAG_NUM_CFG_ITEMS  15

typedef enum {
  POLICYMAN_CLIENT_REQUEST_GET_ITEMS,
  POLICYMAN_CLIENT_REQUEST_GET_ITEMS_MSIM,
  POLICYMAN_CLIENT_REQUEST_GET_ITEMS_SINCE_SEQ,
} policyman_diag_client_request_type_t;

/**
@brief  Event to provide details for a client item request

Details of item requests by clients
*/
typedef PACK(struct) {
  uint32                     client_id;                                         ///< client identifier (thread_id)
  uint8                      request_type;                                      ///< maps to policyman_diag_client_request_type_t
  uint8                      num_items_requested;                               ///< item count (GET_ITEMS, GET_ITEMS_MSIM)
  uint8                      items_requested[POLICYMAN_DIAG_NUM_CFG_ITEMS];     ///< byte entries map to policyman_item_id_t
  uint32                     sequence_number;                                   ///< (GET_ITEMS_MSIM, GET_ITEMS_SINCE_SEQ)
  uint8                      num_item_info;                                     ///< number of returned item infos
  policyman_diag_item_info_t item_info[(POLICYMAN_DIAG_NUM_CFG_ITEMS * 2) + 1]; ///< returned item info;
} policyman_diag_event_client_request_t;

/*-------- policyman_log_item_request --------*/
void policyman_log_item_request(
  policyman_diag_client_request_type_t  requestType,
  size_t                                numRequestedIds,
  policyman_item_id_t const            *pRequestedIdList,
  size_t                                sequenceNumber,
  size_t                                numReturnedItems,
  policyman_item_t           const    **ppReturnedItems
);

/*---------------------------------------------------------------------------
  LOG_PM_CONFIG_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_CONFIG_INFO_LOG_PACKET_VERSION   3

// Log definition LOG_PM_CONFIG_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_CONFIG_INFO_C   0x18A9

/**
@brief  subscription specific information for device configuration information.
*/
typedef PACK(struct) {
  uint8  subs_feature;    ///< feature, sys_subs_feature_t
  uint8  reserved_1;
  uint8  reserved_2;
  uint8  reserved_3;
  uint32 rat_info;       ///< mask of rat capability
} policyman_diag_subs_cfg_info_t;

// Define log record: LOG_PM_CONFIG_INFO_type
/**
@brief  Log of device level configuration information.

Details the device configuration to be used by lower layers.
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;             // Required first, do not access directly (192 bits)
  /* 64-bit align */
  uint32                          version;         // Structure version, required first by diag after header
  uint32                          changed_mask;    ///< mask of changed fields from previous log packet
  /* 64-bit align */
  uint8                           num_sim_cfg;     /**< MSB: cfgs:4
                                                        LSB: sims:4
                                                    */
  uint8                           max_active;      /**< MSB: data:4
                                                        LSB: voice:4
                                                    */
  uint8                           modified;        /**< MSB: reserved:4
                                                         |   max_data:1
                                                         |   feature:1
                                                         |   subs:1
                                                        LSB: sims:1
                                                    */
  uint8                           specialization;  ///< specialization to use, maps to sys_specialization_t
  uint8                           overall_feature; ///< device level feature, maps to sys_subs_feature_t
  uint8                           config_index;    ///< configuration index being used for rat capability
  int32                           explicit_index;  ///< explicit configuration index being used, -1 to ignore
  uint32                          lte_band_count;  ///< number of lte band groups filled in below (1 or 4)
  /* 64-bit align */
  uint64                          hw_gw_bands;     ///< hardware gw bands, maps to sys_band_mask_type
  uint64                          hw_lte_bands_0;  ///< hardware lte bands, maps to  sys_band_mask_type
  uint64                          hw_lte_bands_1;  ///< hardware lte bands, maps to  sys_band_mask_type
  uint64                          hw_lte_bands_2;  ///< hardware lte bands, maps to  sys_band_mask_type
  uint64                          hw_lte_bands_3;  ///< hardware lte bands, maps to  sys_band_mask_type
  uint64                          hw_tds_bands;    ///< hardware tds bands, maps to  sys_band_mask_type
  uint32                          hw_rat_mask;     ///< rat capability from hardware
  policyman_diag_subs_cfg_info_t  subs_cfg_info[2];
} LOG_PM_CONFIG_INFO_type;


/*---------------------------------------------------------------------------
  LOG_PM_SUBS_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_SUBS_INFO_LOG_PACKET_VERSION   3

// Log definition LOG_PM_SUBS_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_SUBS_INFO_C     0x18AA

// Mask for LOG_PM_SUBS_INFO_C:changed_mask
#define PM_SUBS_INFO_CHANGED_ITEM_1_MASK      BM(POLICYMAN_ITEM_DEVICE_CONFIGURATION)
#define PM_SUBS_INFO_CHANGED_ITEM_2_MASK      BM(POLICYMAN_ITEM_RAT_CAPABILITY)
#define PM_SUBS_INFO_CHANGED_ITEM_3_MASK      BM(POLICYMAN_ITEM_RF_BANDS)
#define PM_SUBS_INFO_CHANGED_ITEM_4_MASK      BM(POLICYMAN_ITEM_SVC_MODE)
#define PM_SUBS_INFO_CHANGED_ITEM_5_MASK      BM(POLICYMAN_ITEM_UE_MODE)
#define PM_SUBS_INFO_CHANGED_ITEM_6_MASK      BM(POLICYMAN_ITEM_VOICE_DOMAIN_PREF)
#define PM_SUBS_INFO_CHANGED_ITEM_7_MASK      BM(POLICYMAN_ITEM_RAT_ACQ_ORDER)
#define PM_SUBS_INFO_CHANGED_ITEM_8_MASK      BM(POLICYMAN_ITEM_CALL_MODE)
#define PM_SUBS_INFO_CHANGED_ITEM_9_MASK      BM(POLICYMAN_ITEM_SCAN_OPTIMIZATION)
#define PM_SUBS_INFO_CHANGED_ITEM_10_MASK     BM(POLICYMAN_ITEM_FREQ_LIST)
#define PM_SUBS_INFO_CHANGED_ITEM_11_MASK     BM(POLICYMAN_ITEM_CA_BAND_COMBOS)
#define PM_SUBS_INFO_CHANGED_ITEM_12_MASK     BM(POLICYMAN_ITEM_SERVICE_DOMAIN)

// Mask for LOG_PM_SUBS_INFO_C:flags
#define PM_SUBS_INFO_FLAGS_DOMAIN_PREF_MANDATORY_MASK   0x01
#define PM_SUBS_INFO_FLAGS_RF_FORCE_REGISTRATION_MASK   0x02

// Define log record: LOG_PM_SUBS_INFO_type
/**
@brief  Log of subscription level configuration information.

Details the subscription specific configuration to be used by lower layers.
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;             // Required first, do not access directly (192 bits)
  /* 64-bit align */
  uint32                          version;         // Structure version, required first by diag
  uint32                          changed_mask;    ///< Bitmask of changed items: BM(item_id)
  /* 64-bit align */
  int8                            subs;            ///< subscription id, maps to sys_modem_as_id_e_type
  int8                            call_mode;       ///< call mode to use, maps to sys_call_mode_t
  uint8                           svc_mode;        ///< service mode to use, maps to  policyman_svc_mode_t
  int8                            ue_mode;         ///< ue mode to use, maps to sys_ue_mode_e_type
  uint32                          lte_band_count;  ///< number of lte band groups filled in below (1 or 4)
  /* 64-bit align */
  uint64                          gw_bands;        ///< gw bands to use
  uint64                          lte_bands_0;     ///< lte bands to use
  uint64                          lte_bands_1;     ///< lte bands to use
  uint64                          lte_bands_2;     ///< lte bands to use
  uint64                          lte_bands_3;     ///< lte bands to use
  uint64                          tds_bands;       ///< tds bands to use
  uint32                          rat_mask;        ///< rat capability to use
  uint32                          subphone_cap;    ///< subphone capability from device config
  uint32                          exclude_mask;    ///< mask of rats to exclude from use
  uint32                          base_mask;       ///< base hardware rat capability
  uint8                           flags;           /**< MSB: reserved:6
                                                         |   rf_force_registration:1
                                                        LSB: domain_pref_mandatory:1
                                                                                         */
  int8                            voice_dom_pref;  ///< voice domain, maps to sys_voice_domain_pref_e_type
  int8                            feature0;        ///< feature to use
  int8                            rat_order_table_len;   ///< max number of entries in rat_order_entries[] below
  uint16                          num_rat_order_entries; ///< number of entries in rat_order_entries[]
  uint8                           rat_order_entries[10]; ///< ordered list of rats for acquisition
  uint8                           tgpp_scan_opt;   ///< 3GPP scan optimization
  uint8                           tgpp2_scan_opt;  ///< 3GPP2 scan optimization
  uint8                           freq_list;       ///< frequency list type to use
  int8                            service_domain;  ///< service domain to use, maps to sys_srv_domain_e_type
} LOG_PM_SUBS_INFO_type;


/*---------------------------------------------------------------------------
  LOG_PM_SS_HISTORY_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_SS_HISTORY_INFO_LOG_PACKET_VERSION   1

// Log definition LOG_PM_SS_HISTORY_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_SS_HISTORY_INFO_C   0x1997

#define PM_NUM_SUBS_CONFIGS    2
#define PM_SUBS_CONFIG_SIZE    (PM_NUM_SUBS_CONFIGS * sizeof(policyman_diag_subs_cfg_info_t))
#define PM_CONFIG_BUF_SIZE     (sizeof(LOG_PM_CONFIG_INFO_type) + PM_SUBS_CONFIG_SIZE)
#define PM_SUBS_BUF_SIZE       (sizeof(LOG_PM_SUBS_INFO_type) * POLICYMAN_NUM_SUBS)
#define PM_DUMP_HISTORY_DEPTH  5

/**
@brief  PLMN ID.
*/
typedef PACK(struct) pm_dump_plmn_id_s {
  uint8                                identity[3]; ///< packed plmn id
} pm_dump_plmn_id_t;

/**
@brief  MCC list for 3GPP2 systems.
*/
typedef PACK(struct) {
  uint8     count;                          ///< number of items in mcc_list[]
  uint32    mcc_list[SD_PLMN_MCC_LIST_MAX]; ///< list of available mccs
} pm_dump_plmn_mcc_list;

/**
@brief  Stack specific serving system information.
*/
typedef PACK(struct) {
  boolean                              is_operational;        ///< whether the stack is operational
  int8                                 stackId;               ///< sys_modem_stack_id_e_type; stack id
  byte                                 pending_reject;        ///< pending reject cause
  byte                                 reject_cause;          ///< registration reject cause
  uint8                                id_type;               ///< sys_sys_id_type_e_type; PLMN id type
  boolean                              serving_info_valid;    ///< Whether PLMN should be used
  pm_dump_plmn_id_t                    plmn;                  ///< PLMN of the serving system
  pm_dump_plmn_mcc_list                mcc_list_3gpp2;        ///< List of 3GPP2 MCCs
  pm_dump_plmn_mcc_list                mcc_list_hdr;          ///< List of HDR MCCs
  uint32                               mnc_3gpp2;             ///< sys_mnc_type; 3GPP2 MNC
  uint32                               mnc_hdr;               ///< sys_mnc_type; HDR MNC
  int8                                 sys_mode;              ///< sys_sys_mode_e_type; System's mode
  int8                                 srv_status;            ///< sys_srv_status_e_type; Service Status
  int8                                 active_band;           ///< sys_band_class_e_type; serving RF band
  int8                                 srv_domain;            ///< sys_srv_domain_e_type; Serving domain
  int32                                acq_fail_cnt;          ///< acquistion failures reported on the stack
  boolean                              radio_is_operational;  ///< whether stack is scanning for service
} pm_dump_cm_stack_info_t;

/**
@brief  Subscription level serving system information.
*/
typedef PACK(struct) { // policyman_ss_info_t
  int8                                 asubs_id;              ///< sys_modem_as_id_e_type
  pm_dump_cm_stack_info_t              stack[SYS_MODEM_STACK_ID_MAX];
} pm_dump_cm_ss_info_t;

typedef PACK(struct) {
  uint32                               timestamp;
  pm_dump_cm_ss_info_t                 cm_ss_info;
} pm_dump_cm_ss_info_history_t;

/**
@brief  Log of Serving System event history.

Details the serving system event changes sent to PM.
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;             // Required first, do not access directly (192 bits)
  /* 64-bit align */
  uint32                          version;         // Structure version, required first by diag
  pm_dump_cm_ss_info_t            cm_ss_info;
} LOG_PM_SS_HISTORY_INFO_type;

/*---------------------------------------------------------------------------
  LOG_PM_PH_HISTORY_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_PH_HISTORY_INFO_LOG_PACKET_VERSION   1

// Log definition LOG_PM_PH_HISTORY_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_PH_HISTORY_INFO_C  0x1998

/**
@brief  information sent in the phone event.
*/
typedef PACK(struct) { // policyman_cmph_info_s
  uint32                               changed_fields;        ///< mask of changed fields in event
  int8                                 asubs_id;              ///< sys_modem_as_id_e_type; Subs ID to which preferences apply
  int8                                 mode_pref;             ///< cm_mode_pref_e_type; Indicates the current mode preference
  int8                                 srv_domain_pref;       ///< cm_srv_domain_pref_e_type; The service domain preferred by the client. (GSM/WCDMA/TDS only)
  int8                                 oprt_mode;             ///< sys_oprt_mode_e_type; current operating mode
  /* 64-bit align */
  int8                                 network_sel_mode_pref; ///< cm_network_sel_mode_pref_e_type; Current network selection mode preference (AUTOMATIC/MANUAL)
  pm_dump_plmn_id_t                    user_plmn;             ///< user selected PLMN if network_sel_mode_pref = MANUAL
  uint8                                volte_state;           ///< cm_volte_enable_e_type; Current VoLTE state
  uint8                                rtre_control;          ///< cm_rtre_control_e_type; Indicates 1x subscription source
  uint8                                lte_band_count;        ///< number of lte band groups, 1-4, filled in below (lte_bands[0-3])
  uint8                                dummy_fill;
  /* 64-bit align */
  uint64                               gw_bands;              ///< cm_band_pref_e_type; user preference for GW bands
  uint64                               lte_bands0;            ///< cm_band_pref_e_type; user preference for LTE bands
  uint64                               lte_bands1;            ///< cm_band_pref_e_type; user preference for LTE bands
  uint64                               lte_bands2;            ///< cm_band_pref_e_type; user preference for LTE bands
  uint64                               lte_bands3;            ///< cm_band_pref_e_type; user preference for LTE bands
  uint64                               tds_bands;             ///< cm_band_pref_e_type; user preference for TDS bands
} pm_dump_cm_ph_info_t;

typedef PACK(struct) {
  uint32                               timestamp;
  int8                                 cm_ph_evt;         // cm_ph_event_e_type
  pm_dump_cm_ph_info_t                 cm_ph_info;
} pm_dump_cm_ph_info_history_t;

/**
@brief  Log of phone event history.

Details the phone event changes sent to PM.
*/
typedef PACK(struct) {
  log_hdr_type                         hdr;             // Required first, do not access directly (192 bits)

  uint32                               version;         // Structure version, required first by diag
  int8                                 cm_ph_evt;       ///< cm_ph_event_e_type
  pm_dump_cm_ph_info_t                 cm_ph_info;
} LOG_PM_PH_HISTORY_INFO_type;

/*---------------------------------------------------------------------------
  LOG_PM_UIM_HISTORY_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_UIM_HISTORY_INFO_LOG_PACKET_VERSION   1

// Log definition LOG_PM_UIM_HISTORY_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_UIM_HISTORY_INFO_C 0x1999

/**
@brief  Simlock imsi information.
*/
typedef PACK(struct) pm_dump_simlock_imsi_s {
  uint8    imsi_len;    ///< length of imsi[]
  uint8    imsi[10];    ///< imsi, max length SIMLOCK_IMSI_FILE_LEN
} pm_dump_simlock_imsi_t;

/**
@brief  UIM sim card information.
*/
typedef PACK(struct) {
  pm_dump_plmn_id_t                    plmn;
  uint8                                sim_app_type;      ///< 3GPP sim type, mmgsdi_app_enum_type
  uint8                                cdma_app_type;     ///< 3GPP2 sim type, mmgsdi_app_enum_type
  boolean                              sim_refreshed;     ///< Whether  the sim was refreshed
  boolean                              subs_active;       ///< Whether the sim is active
  uint32                               csim_mcc;          ///< 3GPP CSIM specific mcc, sys_mcc_type
  pm_dump_simlock_imsi_t               simlock_imsi;
} pm_dump_uim_info_t;

/**
@brief  UIM session history.
*/
typedef PACK(struct) {
  uint8                                session_type;  ///< mmgsdi_session_type_enum_type; the key
  int8                                 asubs_id;      ///< sys_modem_as_id_e_type;        PM assigns this
  uint64                               session_id;    ///< mmgsdi_session_id_type;        comes from mmgsdi
  int8                                 app_type;      ///< mmgsdi_app_enum_type;          comes from mmgsdi
  boolean                              in_use;        ///< PM manages -- True if session open
  boolean                              active;        ///< PM manages -- True if session active
} pm_dump_uim_session_info_t;

/**
@brief  Log of uim event history.

Details the uim event changes sent to PM.
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;             // Required first, do not access directly (192 bits)
  /* 64-bit align */
  uint32                          version;         // Structure version, required first by diag
  pm_dump_uim_info_t              uim_info;
  pm_dump_uim_session_info_t      session_info;
} LOG_PM_UIM_HISTORY_INFO_type;

/*---------------------------------------------------------------------------
  LOG_PM_CALL_HISTORY_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_CALL_HISTORY_INFO_LOG_PACKET_VERSION   1

// Log definition LOG_PM_CALL_HISTORY_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_CALL_HISTORY_INFO_C 0x199A

/**
@brief  Call history.
*/
typedef PACK(struct) { // policyman_cmcall_info_s
  int8                                 asubs_id;          ///< sys_modem_as_id_e_type
  int8                                 call_event;        ///< cm_call_event_e_type; Indicates the current call event
  int8                                 call_type;         ///< cm_call_type_e_type; Indicates the call type
} pm_dump_cm_call_info_t;

typedef PACK(struct) {
  uint32                               timestamp;
  pm_dump_cm_call_info_t               cm_call_info;
} pm_dump_cm_call_info_history_t;

/**
@brief  Log of call event history.

Details the call event changes sent to PM.
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;             // Required first, do not access directly (192 bits)
  /* 64-bit align */
  uint32                          version;         // Structure version, required first by diag
  pm_dump_cm_call_info_t          cm_call_info;
} LOG_PM_CALL_HISTORY_INFO_type;

/*---------------------------------------------------------------------------
  LOG_PM_POLICY_STATS_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_POLICY_STATS_INFO_LOG_PACKET_VERSION   2

// Log definition LOG_PM_POLICY_STATS_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_POLICY_STATS_INFO_C  0x199B

/**
@brief  Per-policy statistics.
*/
typedef PACK(struct) {
  mre_dump_policy_info_t               policy_stats;     ///< The MRE-based policy statistics
  uint16                               suspend_count;  ///< if policy is suspended
  boolean                              is_policy_init; ///< if policy was successfully initialized
} pm_dump_policy_info_t;

/**
@brief  Log of policy statistics.

Details the statistics behind each policy that is being run on a device.
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;             // Required first, do not access directly (192 bits)
  /* 64-bit align */
  uint32                          version;         // Structure version, required first by diag
  pm_dump_policy_info_t           policy_info;
} LOG_PM_POLICY_STATS_INFO_type;

/*---------------------------------------------------------------------------
  LOG_PM_DUMP_INFO structure
---------------------------------------------------------------------------*/
#define LOG_PM_DUMP_INFO_LOG_PACKET_VERSION   1

// Log definition LOG_PM_DUMP_INFO_C
// Comes from: core/api/services/log_codes.h
//#define LOG_PM_DUMP_INFO     0x19FF

typedef PACK(struct) {
  uint32                               timestamp;
  LOG_PM_SUBS_INFO_type                subsItems[POLICYMAN_NUM_SUBS];
  LOG_PM_CONFIG_INFO_type              deviceConfig;
} pm_dump_notify_history_t;

typedef PACK(struct) {
  boolean                              xml_initialized;
} pm_dump_file_info_t;

/* Device configuration data:
 *   Configuration item 0.
 *   Contains two device configurations specified in the XML.
 */
typedef PACK(union) {
  LOG_PM_CONFIG_INFO_type              config_info;
} pm_dump_config_info_t;

/* Per-subs data:
 *   Internal subs state variables
 *   SUBS log message content
 */
typedef PACK(struct) {
  /* From policyman_subs_state_t
   * This is not using the policyman_subs_state_t type directly because the actual
   * storage for the sub-structures is needed here instead of a pointer to them.
   */
  int8                                 asubs_id;                // sys_modem_as_id_e_type
  uint32                               preconditions_met;
  pm_dump_cm_ss_info_t                 cm_ss_info;
  pm_dump_cm_ph_info_t                 cm_ph_info;
  pm_dump_uim_info_t                   uim_info;
  pm_dump_cm_call_info_t               cm_call_info;

  /* Log message info */
  LOG_PM_SUBS_INFO_type                subs_log_info;
} pm_dump_subs_info_t;

typedef PACK(struct) {
  log_hdr_type                    hdr;               // Diag Log Header. Required first, do not access directly.
  uint32                          version;           // Version of this structure. Required as first after log header.

  time_t                          dump_time;         // The time() when this data was collected.
  boolean                         is_crash;          // Indicates whether collected under crash conditions or not.

  pm_dump_config_info_t           device_config;

  pm_dump_subs_info_t             subs[POLICYMAN_NUM_SUBS];  // Minimum 2 subs (3 if TRIPLE_SIM is defined)

  /* UIM session information
   *   Four entries, in this order:
   *     MMGSDI_1X_PROV_PRI_SESSION
   *     MMGSDI_GW_PROV_PRI_SESSION
   *     MMGSDI_1X_PROV_SEC_SESSION
   *     MMGSDI_GW_PROV_SEC_SESSION
   */
  pm_dump_uim_session_info_t      uim_session_info[4];

  pm_dump_cm_call_info_history_t  history_cm_call[PM_DUMP_HISTORY_DEPTH];

  pm_dump_cm_ss_info_history_t    history_cm_ss[PM_DUMP_HISTORY_DEPTH];

  pm_dump_cm_ph_info_history_t    history_cm_ph[PM_DUMP_HISTORY_DEPTH];

  pm_dump_notify_history_t        history_pm_notify[PM_DUMP_HISTORY_DEPTH];

  pm_dump_file_info_t             file_info;
  pm_dump_policy_info_t           policy_info;

} LOG_PM_DUMP_INFO_type;

/*
  MINI DUMP log format
*/
typedef PACK(struct) {
  log_hdr_type                    hdr;               // Diag Log Header. Required first, do not access directly.
  uint32                          version;           // Version of this structure. Required as first after log header.

  time_t                          dump_time;         // The time() when this data was collected.
  boolean                         is_crash;          // Indicates whether collected under crash conditions or not.

  pm_dump_config_info_t           device_config;

  pm_dump_subs_info_t             subs[POLICYMAN_NUM_SUBS];  // Minimum 2 subs (3 if TRIPLE_SIM is defined)

  pm_dump_uim_session_info_t      uim_session_info[4]; // 1X_PRI, GW_PRI, GW_SEC, GW_TER

} LOG_PM_MINI_DUMP_INFO_type;

#if defined(T_WINNT) || defined (FEATURE_WINCE)
#error code not present
#endif

#endif /* _POLICYMAN_DIAG_H_ */
