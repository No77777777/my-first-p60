#ifndef UIM_COMMON_EFS_H
#define UIM_COMMON_EFS_H
/*===========================================================================


                U I M   C O M M O N   E F S   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2018, 2020, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/common/inc/uim_common_efs.h#5 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
07/29/21   shp     Support serving MCC based ECC number for china
02/21/20   cj      Corrected NV path for onchip SIM
01/02/20   sch     Added support for log masking
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration
07/06/17   ku      Poll card for twice when detecting flaky removal interupt
05/19/17   nr      Creating a new efs item for override_opl_pnn_lookup_for_non_rplmn
05/31/17   gm      Boot-up APDU logging
04/19/17   bcho    Moving QCRIL ECC DB to PBM
04/14/17   kv      Added pass-through mode support
04/03/17   ks      UIM automatic slot mapping support
03/29/17   nr      Add support to cache PIN1 of the SIM card when PSM mode enabled
01/27/17   sk      Deprecate direct read of GCF NV 947
01/10/17   bcho    Support for slot specific CDMA LESS NV feature
12/28/16   ll      Add test mode logging enabled APIs for sensitive info control
12/07/16   av      LPA phase2 initial changes
11/24/16   ks      Adding support for spcom interface
10/24/16   vdc     Added support for auto reg SMS when ESN is changed
10/10/16   av      Add support for configuring HTTP stack via LPA EFS item
09/20/16   me      Added support for non uim EDRX EFS and UIM MTC EFS
08/14/16   tkl     Added support for simlock single lock
07/21/16   sp      ECC optimizations for IoE devices
06/29/16   shr     Support for NV based Toolkit disablement
06/24/16   ar      EONS selective logging
06/16/16   av      Support for setting terminal capability from AP
06/09/16   shr     Deprecate GSTK TAL
06/03/16   sp      Revert Change EFS Path for OnChip SIM configuration
05/30/16   sp      Change EFS Path for OnChip SIM configuration
04/20/16   sam     Addition of new NV for subscription manager
04/11/16   av      Introduce LPA support
03/28/16   sam     Support for SAM card
01/22/16   bcho    Send Card and session events based on SFS data in PSM mode
12/16/15   ssr     Delay local card power up in Modem SSR in BTSAP mode
12/10/15   ks      Rel12 polling enhancement
09/10/15   kv      Features_status_list is split into device and slot items
09/02/15   hyo     Support for offtarget
08/28/15   hh      Removed JCDMA related code
08/25/15   nr      Operator specifc NV ECC numbers configuration
08/25/15   bcho    Support for CDMALess Device
08/21/15   at      Support for slot type in MMGSDI APIs
07/30/15   yt      Pause deactivation to allow pending commands to run
07/03/15   tkl     Added support for custom fields
06/17/15   bd      Changes to support concatenated SMSs
06/11/15   ks      New NV to protect workarounds for non-spec compliant cards
05/21/15   kv      Use MCFG Context enum instead of typecast
05/05/15   ks      Enhancing the voltage supply indication to QMI feature
04/30/15   vdc     Added support for APN for bootstrapping
04/30/15   nr      Enhancements in handling of GCF anite flag update
04/15/15   ks      De-Featurizing the FEATURE_UIM_REMOTE/BTSAP_UIM
04/06/15   ll      UIMDRV migrates to UIM COMMON EFS APIs for all EFS items
04/03/15   kk      Migrate to mcfg EFS wrappers
04/02/15   vr      Non UI and non NW proactive command parallel processing
03/16/15   lxu     Use gba_url and gba_port in nv if they are valid
03/11/15   av      Clean up unused function
03/04/15   vdc     Added support for GBA
01/14/15   shr     Disable CC env during SEND/RECEIVE DATA based on NV config
01/07/15   vdc     Added support for handling multiple profiles enabled cards
12/28/14   sam     Add support for UIM powerup logging
12/01/14   gm      Added support to read envelope retry data from NV
11/19/14   av      Fix logic of setting ANITE_NV when card contains CSIM app
11/10/14   akv     Support for user to customize LDO settings
10/31/14   gm      Update qmi_cat_raw_unsupported_features in conf file
10/13/14   tl      Fix compiler warnings
10/12/14   hh      Add support to get Voice Domain Preference
09/09/14   kk      Enhancements to NV handling
07/25/14   av      Auto onchip activation for multi slots
07/15/14   yt      Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "comdef.h"
#include "mcfg_fs.h"

/*=============================================================================

                          UIM COMMON DEFINITIONS
                     Definitions used throughout UIM

=============================================================================*/

/* Number of bytes used for storing length of terminal capability EFS file */
#define UIM_COMMON_TERMINAL_CAPABILITY_EFS_LEN_BYTES                   2
/* Length of LPA support NV/EFS item */
#define UIM_COMMON_LPA_EFS_ITEM_LEN                                    0x10
/* Indices in LPA support NV/EFS item */
#define UIM_COMMON_LPA_EFS_ITEM_LPDD_SUPPORT_INDEX                     0x00
#define UIM_COMMON_LPA_EFS_ITEM_LUID_SUPPORT_INDEX                     0x01
#define UIM_COMMON_LPA_EFS_ITEM_LPA_ON_AP_OR_MODEM_INDEX               0x02
#define UIM_COMMON_LPA_EFS_ITEM_HTTP_STACK_INDEX                       0x03
#define UIM_COMMON_LPA_EFS_ITEM_LDSD_SUPPORT_INDEX                     0x04
#define UIM_COMMON_LPA_EFS_ITEM_LUIE_SUPPORT_INDEX                     0x05

/* Paths for MMGSDI EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_MMGSDI_REFRESH_RETRY                         "/nv/item_files/modem/uim/mmgsdi/refresh_retry"
#define UIM_COMMON_EFS_PATH_MMGSDI_REFRESH_VOTE_OK                       "/nv/item_files/modem/uim/mmgsdi/refresh_vote_ok"
#define UIM_COMMON_EFS_PATH_MMGSDI_FEATURE_STATUS_LIST                   "/nv/item_files/modem/uim/mmgsdi/features_status_list"
#define UIM_COMMON_EFS_PATH_MMGSDI_DEVICE_SPECIFIC_FEATURE_STATUS_LIST   "/nv/item_files/modem/uim/mmgsdi/device_features_status_list"
#define UIM_COMMON_EFS_PATH_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST     "/nv/item_files/modem/uim/mmgsdi/slot_features_status_list"
#define UIM_COMMON_EFS_PATH_MMGSDI_ENS_SLOT_ENABLED                      "/nv/item_files/modem/uim/mmgsdi/ens_slot_enabled"
#define UIM_COMMON_EFS_PATH_MMGSDI_ONCHIP_SUPPORT                        "/nv/item_files/modem/uim/mmgsdi/auto_activate_onchip"
#define UIM_COMMON_EFS_PATH_MMGSDI_ENS_SUB_FEATURES_STATUS_LIST_FILE     "/nv/item_files/modem/uim/mmgsdi/ens_sub_features_status_list"
#define UIM_COMMON_EFS_PATH_MMGSDI_DIAG_SUPPORT                          "/nv/item_files/modem/uim/mmgsdi/mmgsdi_diag_support"
#define UIM_COMMON_EFS_PATH_MMGSDI_HALT_SUB                              "/nv/item_files/modem/uim/mmgsdi/halt_subscription"
#define UIM_COMMON_EFS_PATH_MMGSDI_ENCRYPT_SUB                           "/nv/item_files/modem/uim/mmgsdi/encrypt_sub_ok"
#define UIM_COMMON_EFS_PATH_MMGSDI_CSIM_DEVICE_MODEL                     "/nv/item_files/modem/uim/mmgsdi/csim_device_model"
#define UIM_COMMON_EFS_PATH_MMGSDI_ONCHIP_CONFIG_EFS_FILE_SLOT1          "/nv/item_files/modem/uim/mmgsdi/onchip/onchip_slot1"
#define UIM_COMMON_EFS_PATH_MMGSDI_ONCHIP_CONFIG_EFS_FILE_SLOT2          "/nv/item_files/modem/uim/mmgsdi/onchip/onchip_slot2"
#define UIM_COMMON_EFS_PATH_MMGSDI_ONCHIP_CONFIG_EFS_FILE_SLOT3          "/nv/item_files/modem/uim/mmgsdi/onchip/onchip_slot3"
#define UIM_COMMON_EFS_PATH_MMGSDI_BUILTIN_PLMN_LIST_FILE                "/nv/item_files/modem/uim/mmgsdi/builtin_plmn_list"
#define UIM_COMMON_EFS_PATH_MMGSDI_AUTO_ACTIVATE_MULTI_SLOTS_FILE        "/nv/item_files/modem/uim/mmgsdi/auto_activate_multi_slots"
#define UIM_COMMON_EFS_PATH_MMGSDI_MULTI_PROFILE_FILE                    "/nv/item_files/modem/uim/mmgsdi/multi_profile"
#define UIM_COMMON_EFS_PATH_MMGSDI_GPRS_GCF_ANITE_CONTROL                "/nv/item_files/modem/uim/mmgsdi/gprs_gcf_anite_control"
#define UIM_COMMON_EFS_PATH_MMGSDI_DEACTIVATION_TIMER                    "/nv/item_files/modem/uim/mmgsdi/deact_timer_value"
#define UIM_COMMON_EFS_PATH_MMGSDI_RMT_SIM_TEST_FILE_SLOT_1              "/nv/item_files/modem/uim/mmgsdi/rmt_sim_test_params_1"
#define UIM_COMMON_EFS_PATH_MMGSDI_RMT_SIM_TEST_FILE_SLOT_2              "/nv/item_files/modem/uim/mmgsdi/rmt_sim_test_params_2"
#define UIM_COMMON_EFS_PATH_MMGSDI_RMT_SIM_TEST_FILE_SLOT_3              "/nv/item_files/modem/uim/mmgsdi/rmt_sim_test_params_3"
#define UIM_COMMON_EFS_PATH_MMGSDI_PSM                                   "/nv/item_files/modem/uim/mmgsdi/psm_mode"
#define UIM_COMMON_EFS_PATH_MMGSDI_PSM_PIN1_CACHE_SUPPORT                "/nv/item_files/modem/uim/mmgsdi/psm_pin1_cache_support"
#define UIM_COMMON_EFS_PATH_MMGSDI_SELECTIVE_LOGGING                     "/nv/item_files/modem/uim/mmgsdi/enable_selective_logging"
#define UIM_COMMON_EFS_PATH_MMGSDI_MTC                                   "/nv/item_files/modem/uim/mmgsdi/mtc_mode"
#define UIM_COMMON_EFS_PATH_MMGSDI_SECURE_LOGGING_ENABLED                "/nv/item_files/modem/uim/mmgsdi/secure/test_mode_logging_enabled"
#define UIM_COMMON_EFS_PATH_MMGSDI_PRIVATE_LOGGING_ENABLED                "/nv/item_files/modem/uim/mmgsdi/private_logging_enabled"
#define UIM_COMMON_EFS_PATH_MMGSDI_CSIM_PRESENT                          "/nv/item_files/modem/uim/mmgsdi/csim_present"
#define UIM_COMMON_EFS_PATH_MMGSDI_OVERRIDE_OPL_PNN_LOOKUP_FOR_NON_RPLMN "/nv/item_files/modem/uim/mmgsdi/override_opl_pnn_lookup_for_non_rplmn"

/* Paths for GSTK EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_GSTK_ME_TP_CLIENTS                   "/nv/item_files/modem/uim/gstk/tp_clients"
#define UIM_COMMON_EFS_PATH_GSTK_ME_PROACTIVE_FEATURE_ENABLE_CFG "/nv/item_files/modem/uim/gstk/proactive_feature_enable_cfg"
#define UIM_COMMON_EFS_PATH_GSTK_ME_REDIAL_CFG                   "/nv/item_files/modem/uim/gstk/redial_cfg"
#define UIM_COMMON_EFS_PATH_GSTK_ME_QMI_CAT_SVC_INIT             "/nv/item_files/modem/uim/gstk/qmi_cat_svc_init"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_CAT_VERSION                "/nv/item_files/modem/uim/gstk/cat_version"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_RAT_PRIORITY_CONFIG        "/nv/item_files/modem/uim/gstk/rat_priority_config"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_BIP_APN_MAPPING_TABLE      "/nv/item_files/modem/uim/gstk/bip_apn_mapping_tbl"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_DEFAULT_APN                "/nv/item_files/modem/uim/gstk/default_apn"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_FEATURE_BMASK              "/nv/item_files/modem/uim/gstk/feature_bmsk"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_SMS_LINK_CTRL_TIMER        "/nv/item_files/modem/uim/gstk/sms_link_ctrl_timer"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_TOOLKIT_ENV_RETRY_EXT_DATA "/nv/item_files/modem/uim/gstk/envelope_retry"
#define UIM_COMMON_EFS_PATH_GSTK_SUB_BIP_OTASP_CONCURRENCY       "/nv/item_files/modem/uim/gstk/bip_otasp_concurrency"
#define UIM_COMMON_EFS_PATH_GSTK_SUB_ATTACH_CTRL_TIMER           "/nv/item_files/modem/uim/gstk/attach_ctrl_timer"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_FEATURE_BMASK_2            "/nv/item_files/modem/uim/gstk/feature_bmsk_2"
#define UIM_COMMON_EFS_PATH_GSTK_ME_FEATURE_BMASK                "/nv/item_files/modem/uim/gstk/me_feature_bmsk"
#define UIM_COMMON_EFS_PATH_GSTK_SLOT_TOOLKIT_DISABLED           "/nv/item_files/modem/uim/gstk/toolkit_disabled"

/* Paths for UIMDRV carrier dependent EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_UIMDRV_PDOWN_NV                      "/nv/item_files/modem/uim/uimdrv/nv_pdown_uim_consecutive_techproblems"
#define UIM_COMMON_EFS_PATH_HW_LDO_CONFIG                        "/nv/item_files/modem/uim/uimdrv/uim_hw_ldo_config_efs"
#define UIM_COMMON_EFS_PATH_UIM_PUP_LOGGING_CONFIG               "/nv/item_files/modem/uim/uimdrv/uim_powerup_logging_config"

/* Paths for UIMDRV non-carrier dependent EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_UIMDRV_CONF                                "/nv/item_files/conf/uimdrv.conf"
#define UIM_COMMON_EFS_PATH_UIMDRV_FEATURES_STATUS_LIST                "/nv/item_files/modem/uim/uimdrv/uim_features_status_list"
#define UIM_COMMON_EFS_PATH_UIMDRV_HOTSWAP_DEBOUNCE_CONFIG             "/nv/item_files/modem/uim/uimdrv/me_hotswap_configuration"
#define UIM_COMMON_EFS_PATH_UIMDRV_ACTIVE_SLOT                         "/nv/item_files/modem/uim/uimdrv/nv_active_slot_configuration"
#define UIM_COMMON_EFS_PATH_UIMDRV_DISABLE_POLLING                     "/nv/item_files/modem/uim/uimdrv/uim_disable_polling"
#define UIM_COMMON_EFS_PATH_UIMDRV_BUSY_RESPONSE                       "/nv/item_files/modem/uim/uimdrv/sim_response_timers_config"
#define UIM_COMMON_EFS_PATH_UIMDRV_FEATURE_BUSY_RESPONSE_SIMULATE_NULL "/nv/item_files/modem/uim/uimdrv/uim_busy_response_simulate_null_config"
#define UIM_COMMON_EFS_PATH_UIMDRV_REMOTE_COMMAND_RESP_TIMER           "/nv/item_files/modem/uim/uimdrv/nv_remote_command_resp_timer"
#define UIM_COMMON_EFS_PATH_UIMDRV_SUPPLY_VOLTAGE_IND_ACK_DELAY_TIMER  "/nv/item_files/modem/uim/uimdrv/nv_supply_voltage_ind_ack_delay_timer"
#define UIM_COMMON_EFS_PATH_UIMDRV_WORKAROUNDS_FOR_NON_SPEC_COMPLIANT_CARDS "/nv/item_files/modem/uim/uimdrv/uim_workarounds_for_non_spec_compliant_cards"

#define UIM_COMMON_EFS_PATH_UIMDRV_FEATURE_SUPPORT_HOTSWAP             "/nv/item_files/modem/uim/uimdrv/feature_support_hotswap"
#define UIM_COMMON_EFS_PATH_UIMDRV_HW_CONFIG                           "/nv/item_files/modem/uim/uimdrv/uim_hw_config"
#define UIM_COMMON_EFS_PATH_UIMDRV_EXTENDED_RECOVERY_FILE              "/nv/item_files/modem/uim/uimdrv/uim_extended_recovery_config"
#define UIM_COMMON_EFS_PATH_UIMDRV_GPIO_BASED_HOTSWAP                  "/nv/item_files/modem/uim/uimdrv/uim_gpio_based_hotswap"
#define UIM_COMMON_EFS_PATH_UIMDRV_REL12_POLLING_OPTIMIZATION_FILE     "/nv/item_files/modem/uim/uimdrv/uim_rel12_polling_optimization"
#define UIM_COMMON_EFS_PATH_UIMDRV_POLLING_INTERVAL_FLAKY              "/nv/item_files/modem/uim/uimdrv/uim_polling_interval_flaky"

#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
#error code not present
#endif

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
#define UIM_COMMON_EFS_PATH_LAST_MODE_AND_CONFIG_DELAY_FOR_REMOTE_CONNECT  "/nv/item_files/modem/uim/uimdrv/uim_last_mode_and_config_delay_for_remote_connect"
#define UIM_COMMON_EFS_PATH_UIMDRV_SLOT_MAPPING_FILE                   "/nv/item_files/modem/uim/uimdrv/uim_slot_mapping_config"
#define UIM_COMMON_EFS_PATH_UIMDRV_EXTENDED_SLOT_MAPPING_FILE          "/nv/item_files/modem/uim/uimdrv/uim_extended_slot_mapping_config"
#define UIM_COMMON_EFS_PATH_SLOT_MAPPING_PREFERENCES_TABLE             "/nv/item_files/modem/uim/uimdrv/slot_mapping_table"
#define UIM_COMMON_EFS_PATH_AUTOMATIC_SLOT_MAPPING_NEEDED              "/nv/item_files/modem/uim/uimdrv/auto_mapping_needed"
#define UIM_COMMON_EFS_PATH_UIMDRV_PASSTHROUGH_MODE                    "/nv/item_files/modem/uim/uimdrv/uim_passthrough_mode"
#define UIM_COMMON_EFS_PATH_UIMDRV_BOOT_UP_APDU_LOG                    "/nv/item_files/modem/uim/uimdrv/boot_up_apdu_log"

/* Paths for QMI UIM/CAT EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_CAT_MODE                     "/nv/item_files/modem/qmi/cat/qmi_cat_mode"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_CAT_DEFAULT_LANG             "/nv/item_files/modem/qmi/cat/qmi_cat_default_lang"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_CAT_CONFIG_TP                "/nv/item_files/modem/qmi/cat/qmi_cat_custom_tp"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_CAT_DISPLAY_ALPHA_FILE       "/nv/item_files/modem/qmi/cat/qmi_cat_display_alpha"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_CAT_BLOCK_SMS_PP_ENV         "/nv/item_files/modem/qmi/cat/qmi_cat_block_sms_pp_env"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_CAT_RAW_UNSUPPORTED_FEATURES "/nv/item_files/modem/qmi/cat/qmi_cat_raw_unsupported_features"

#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_SAP_SEC_RESTR      "/nv/item_files/modem/qmi/uim/sap_security_restrictions"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_AUTH_SEC_RESTR     "/nv/item_files/modem/qmi/uim/auth_security_restrictions"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_RESTR     "/nv/item_files/modem/qmi/uim/apdu_security_restrictions"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_SILENT_PIN1_FILE   "/nv/item_files/modem/qmi/uim/qmi_uim_silent_pin1"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_AID_LIST  "/nv/item_files/modem/qmi/uim/apdu_security_aid_list"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_CLOSE_CHANNEL_SYNC "/nv/item_files/modem/qmi/uim/close_channel_sync"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_SILENT_RECOVERY    "/nv/item_files/modem/qmi/uim/silent_recovery"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_REMOTE_SERVICE     "/nv/item_files/modem/uim/remote/uim_remote_service_enable"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_TERMINAL_CAPABILITY       "/nv/item_files/modem/qmi/uim/terminal_capability"
#define UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD "/nv/item_files/modem/qmi/uim/simlock_status_for_valid_card"

/* Paths for GBA EFS items access via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_GBA_BOOTSTRAP_BSF_ADDRESS                "/nv/item_files/modem/uim/gba/bsf_address"
#define UIM_COMMON_EFS_PATH_GBA_BOOTSTRAP_APN                        "/nv/item_files/modem/uim/gba/access_point_name"
#define UIM_COMMON_EFS_PATH_GBA_BOOTSTRAP_CUSTOM_REALM               "/nv/item_files/modem/uim/gba/custom_realm"
#define UIM_COMMON_EFS_PATH_GBA_BOOTSTRAP_CUSTOM_USER_AGENT_TMPI     "/nv/item_files/modem/uim/gba/custom_user_agent_tmpi"
#define UIM_COMMON_EFS_PATH_GBA_BOOTSTRAP_HTTP_HEADER_IMEI           "/nv/item_files/modem/uim/gba/http_header_imei"

/* Paths for non-UIM EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_NON_UIM_CSG_SUPPORT_CONFIG           "/nv/item_files/modem/nas/csg_support_configuration"
#define UIM_COMMON_EFS_PATH_NON_UIM_IMS_ENABLE                   "/nv/item_files/ims/IMS_enable"
#define UIM_COMMON_EFS_PATH_NON_UIM_RPM_INFO                     "/nv/item_files/modem/mmode/rpm_info"
#define UIM_COMMON_EFS_PATH_NON_UIM_VOICE_DOMAIN_PREF            "/nv/item_files/modem/mmode/voice_domain_pref"
#define UIM_COMMON_EFS_PATH_NON_UIM_LTE_3GPP_RELEASE_VER         "/nv/item_files/modem/lte/lte_3gpp_release_ver"
#define UIM_COMMON_EFS_PATH_NON_UIM_PSM_MODE                     "/nv/item_files/modem/mmode/psm_config"
#define UIM_COMMON_EFS_PATH_NON_UIM_EDRX_SUPPORT                 "/nv/item_files/modem/nas/nas_edrx_config"
#define UIM_COMMON_EFS_PATH_NON_UIM_OPERATOR_NAME                "/nv/item_files/modem/mmode/operator_name"

/* Paths for PBM EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_PBM_NV_ECC_LIST                      "/nv/item_files/pbm/pbm_ecc_nums"
#define UIM_COMMON_EFS_PATH_PBM_NV_HARDCODED_ECC_CONFIG          "/nv/item_files/pbm/pbm_hardcoded_ecc_config"
#define UIM_COMMON_EFS_PATH_PBM_NV_HARDCODED_ECC_LIST            "/nv/item_files/pbm/pbm_hardcoded_ecc_list"
/* Paths for PBM EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_PBM_NV_ECC_LIST_PER_SUB              "/nv/item_files/pbm/pbm_nv_ecc_list_per_sub"
#define UIM_COMMON_EFS_PATH_PBM_SIM_ECC_AIRPLANE                 "/nv/item_files/pbm/pbm_sim_ecc_airplane"
#define UIM_COMMON_EFS_PATH_PBM_EFS_SUPPORT_DISABLE              "/nv/item_files/pbm/pbm_efs_support_disable"
#define UIM_COMMON_EFS_PATH_PBM_FEATURES_LIST                    "/nv/item_files/pbm/features_status_list"
#define UIM_COMMON_EFS_PATH_PBM_LATEST_MCC                       "/nv/item_files/pbm/latest_mcc"
#define UIM_COMMON_EFS_PATH_PBM_ADD_SPECIAL_SRV_FLAG             "/nv/item_files/pbm/add_spl_srv_flag"

/* Paths for LPA EF Sitems accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_LPA_SUPPORT                          "/nv/item_files/modem/uim/lpa/lpa_support"

/* Paths for SimLock EFS items accessed via UIM Common APIs */
#define UIM_COMMON_EFS_PATH_RSU_DEVICE_ID                        "/nv/item_files/modem/uim/simlock/rsu_device_id"
#define UIM_COMMON_EFS_PATH_RSU_DELAY_TIMER_VALUE                "/nv/item_files/modem/uim/simlock/initial_delay_timer_value"

/* Config file for multi-context items */
#define UIM_COMMON_EFS_PATH_MMGSDI_MULTI_CONTEXT_CONF            "/nv/item_files/conf/mmgsdi.conf"

#define UIM_COMMON_EFS_PATH_CONF_DIR1                            "/nv"
#define UIM_COMMON_EFS_PATH_CONF_DIR2                            "/nv/item_files"
#define UIM_COMMON_EFS_PATH_CONF_DIR3                            "/nv/item_files/conf"

/* Paths for QMI CAT and QMI UIM config dir */
#define UIM_COMMON_EFS_PATH_MODEM_CONFIG_DIR1                    "/nv/item_files/modem"
#define UIM_COMMON_EFS_PATH_MODEM_QMI_CONFIG_DIR2                "/nv/item_files/modem/qmi"
#define UIM_COMMON_EFS_PATH_MODEM_QMI_CAT_CONFIG_DIR3            "/nv/item_files/modem/qmi/cat"
#define UIM_COMMON_EFS_PATH_MODEM_QMI_UIM_CONFIG_DIR4            "/nv/item_files/modem/qmi/uim"
#define UIM_COMMON_EFS_PATH_MODEM_UIM_MMGSDI_CONFIG_DIR          "/nv/item_files/modem/uim/mmgsdi"
#define UIM_COMMON_EFS_PATH_MODEM_UIM_REMOTE_CONFIG_DIR          "/nv/item_files/modem/uim/remote"
#define UIM_COMMON_EFS_PATH_MODE_UIM_MMGSDI_SECURE_DIR           "/nv/item_files/modem/uim/mmgsdi/secure"

/*=============================================================================
   ENUM:      UIM_COMMON_EFS_ITEM_TYPE

   DESCRIPTION:
     Common enums for EFS items accessed by various UIM modules.
=============================================================================*/
typedef enum
{
  /* Items owned by MMGSDI */
  UIM_COMMON_EFS_MMGSDI_START = 0x0,
  UIM_COMMON_EFS_MMGSDI_REFRESH_RETRY,
  UIM_COMMON_EFS_MMGSDI_REFRESH_VOTE_OK,
  UIM_COMMON_EFS_MMGSDI_FEATURE_STATUS_LIST,
  UIM_COMMON_EFS_MMGSDI_ENS_SLOT_ENABLED,
  UIM_COMMON_EFS_MMGSDI_ONCHIP_SUPPORT,
  UIM_COMMON_EFS_MMGSDI_DIAG_SUPPORT,
  UIM_COMMON_EFS_MMGSDI_HALT_SUBSCRIPTION,
  UIM_COMMON_EFS_MMGSDI_ENCRYPT_SUB_OK,
  UIM_COMMON_EFS_MMGSDI_CSIM_DEVICE_MODEL,

  /* 0xA*/
  UIM_COMMON_EFS_MMGSDI_ONCHIP_CONFIG_SLOT1,
  UIM_COMMON_EFS_MMGSDI_ONCHIP_CONFIG_SLOT2,
  UIM_COMMON_EFS_MMGSDI_ONCHIP_CONFIG_SLOT3,
  UIM_COMMON_EFS_MMGSDI_ENS_SUB_FEATURES_STATUS_LIST,
  UIM_COMMON_EFS_MMGSDI_BUILT_IN_PLMN_LIST,
  UIM_COMMON_EFS_MMGSDI_AUTO_ACTIVATE_MULTI_SLOTS_FILE,
  UIM_COMMON_EFS_MMGSDI_CSIM_PRESENT,
  UIM_COMMON_EFS_MMGSDI_MULTI_PROFILE_FILE,
  UIM_COMMON_EFS_MMGSDI_GPRS_GCF_ANITE_CONTROL,
  UIM_COMMON_EFS_MMGSDI_DEACTIVATION_TIMER,

  /* 0x14 */
  UIM_COMMON_EFS_MMGSDI_PSM,
  UIM_COMMON_EFS_MMGSDI_PSM_PIN1_CACHE_SUPPORT,
  UIM_COMMON_EFS_MMGSDI_DEVICE_SPECIFIC_FEATURE_STATUS_LIST,
  UIM_COMMON_EFS_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST,
  UIM_COMMON_EFS_MMGSDI_SELECTIVE_LOGGING,
  UIM_COMMON_EFS_MMGSDI_MTC,
  UIM_COMMON_EFS_MMGSDI_SECURE_LOGGING_ENABLED,
  UIM_COMMON_EFS_MMGSDI_OVERRIDE_OPL_PNN_LOOKUP_FOR_NON_RPLMN,
  UIM_COMMON_EFS_MMGSDI_PRIVATE_LOGGING_ENABLED,

  /* Items owned by GSTK */
  UIM_COMMON_EFS_GSTK_START = 0x100,
  UIM_COMMON_EFS_GSTK_ME_TP_CLIENTS,
  UIM_COMMON_EFS_GSTK_ME_PROACTIVE_FEATURE_ENABLE_CFG,
  /* Deprecated: UIM_COMMON_EFS_GSTK_ME_TAL_CONF, */
  UIM_COMMON_EFS_GSTK_ME_REDIAL_CFG = 0x104,
  UIM_COMMON_EFS_GSTK_ME_QMI_CAT_SVC_INIT,
  UIM_COMMON_EFS_GSTK_SLOT_CAT_VERSION,
  UIM_COMMON_EFS_GSTK_SLOT_RAT_PRIORITY_CONFIG,
  UIM_COMMON_EFS_GSTK_SLOT_BIP_APN_MAPPING_TABLE,
  UIM_COMMON_EFS_GSTK_SLOT_DEFAULT_APN,

  /* 0x10A */
  UIM_COMMON_EFS_GSTK_SLOT_FEATURE_BMASK,
  UIM_COMMON_EFS_GSTK_SLOT_TOOLKIT_ENV_RETRY_EXT_DATA,
  UIM_COMMON_EFS_GSTK_SUB_BIP_OTASP_CONCURRENCY,
  UIM_COMMON_EFS_GSTK_SUB_ATTACH_CTRL_TIMER,
  UIM_COMMON_EFS_GSTK_SLOT_SMS_LINK_CTRL_TIMER,
  UIM_COMMON_EFS_GSTK_SLOT_FEATURE_BMASK_2,
  UIM_COMMON_EFS_GSTK_ME_FEATURE_BMASK,
  UIM_COMMON_EFS_GSTK_SLOT_TOOLKIT_DISABLED,

  /* Items owned by UIMDRV */
  UIM_COMMON_EFS_UIMDRV_START = 0x200,
  UIM_COMMON_EFS_UIMDRV_CONF,
  UIM_COMMON_EFS_UIMDRV_PDOWN_NV_EF,
  UIM_COMMON_EFS_UIMDRV_LDO_SETTINGS,
  UIM_COMMON_EFS_UIM_PUP_LOGGING_CONFIG,
  UIM_COMMON_EFS_UIMDRV_FEATURES_STATUS_LIST,
  UIM_COMMON_EFS_UIMDRV_HOTSWAP_DEBOUNCE_CONFIG,
  UIM_COMMON_EFS_UIMDRV_ACTIVE_SLOT,
  UIM_COMMON_EFS_UIMDRV_BUSY_RESPONSE,
  UIM_COMMON_EFS_UIMDRV_FEATURE_BUSY_RESPONSE_SIMULATE_NULL,

  /* 0x20A */
  UIM_COMMON_EFS_UIMDRV_REMOTE_COMMAND_RESP_TIMER,
  UIM_COMMON_EFS_UIMDRV_FEATURE_SUPPORT_HOTSWAP,
  UIM_COMMON_EFS_UIMDRV_HW_CONFIG,
  UIM_COMMON_EFS_UIMDRV_DISABLE_POLLING,
  UIM_COMMON_EFS_UIMDRV_EXTENDED_RECOVERY_FILE,
  UIM_COMMON_EFS_UIMDRV_GPIO_BASED_HOTSWAP,
  UIM_COMMON_EFS_UIMDRV_SUPPLY_VOLTAGE_IND_ACK_DELAY_TIMER,
  UIM_COMMON_EFS_UIMDRV_WORKAROUNDS_FOR_NON_SPEC_COMPLIANT_CARDS,
  UIM_COMMON_EFS_UIMDRV_REL12_POLLING_OPTIMIZATION_FILE,
  UIM_COMMON_EFS_UIMDRV_POLLING_INTERVAL_FLAKY,
  /* 0x213 */
  UIM_COMMON_EFS_UIMDRV_BOOT_UP_APDU_LOG,  

#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
  #error code not present
#endif
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
  UIM_COMMON_EFS_LAST_MODE_AND_CONFIG_DELAY_FOR_REMOTE_CONNECT,

  /* 0x216 */
  UIM_COMMON_EFS_UIMDRV_SLOT_MAPPING,
  UIM_COMMON_EFS_UIMDRV_EXTENDED_SLOT_MAPPING,
  UIM_COMMON_EFS_SLOT_MAPPING_PREFERENCES_TABLE,
  UIM_COMMON_EFS_AUTOMATIC_SLOT_MAPPING_NEEDED,
  UIM_COMMON_EFS_UIMDRV_PASSTHROUGH_MODE,

  /* Items owned by QMI UIM and CAT */
  UIM_COMMON_EFS_UIMQMI_START = 0x300,
  UIM_COMMON_EFS_UIMQMI_QMI_CAT_MODE,
  UIM_COMMON_EFS_UIMQMI_QMI_CAT_DEFAULT_LANG,
  UIM_COMMON_EFS_UIMQMI_QMI_CAT_CONFIG_TP,
  UIM_COMMON_EFS_UIMQMI_QMI_CAT_DISPLAY_ALPHA_FILE,
  UIM_COMMON_EFS_UIMQMI_QMI_CAT_BLOCK_SMS_PP_ENV,
  UIM_COMMON_EFS_UIMQMI_QMI_CAT_RAW_UNSUPPORTED_FEATURES,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_SAP_SEC_RESTR,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_AUTH_SEC_RESTR,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_RESTR,

  /* 0x30A */
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_SILENT_PIN1_FILE,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_AID_LIST,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_CLOSE_CHANNEL_SYNC,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_SILENT_RECOVERY,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_GBA_FQDN_WHITE_LIST,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_GBA_FQDN_BLACK_LIST,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_REMOTE_SERVICE,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_TERMINAL_CAPABILITY,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD,

  /* Items accessed in UIM but not owned by UIM */
  UIM_COMMON_EFS_NON_UIM_START = 0x400,
  UIM_COMMON_EFS_NON_UIM_CSG_SUPPORT_CONFIG,
  UIM_COMMON_EFS_NON_UIM_IMS_ENABLE,
  UIM_COMMON_EFS_NON_UIM_RPM_INFO,
  UIM_COMMON_EFS_NON_UIM_VOICE_DOMAIN_PREF,
  UIM_COMMON_EFS_NON_UIM_LTE_3GPP_RELEASE_VER,
  UIM_COMMON_EFS_NON_UIM_PSM_MODE,
  UIM_COMMON_EFS_NON_UIM_EDRX_SUPPORT,
  UIM_COMMON_EFS_NON_UIM_OPERATOR_NAME,

  /* Config file for multi-context items */
  UIM_COMMON_EFS_MMGSDI_MULTI_CONTEXT_CONF = 0x500,

  /* Items owned by GBA */
  UIM_COMMON_EFS_GBA_BOOTSTRAP_BSF_ADDRESS = 0x600,
  UIM_COMMON_EFS_GBA_BOOTSTRAP_APN,
  UIM_COMMON_EFS_GBA_BOOTSTRAP_CUSTOM_REALM,
  UIM_COMMON_EFS_GBA_BOOTSTRAP_CUSTOM_USER_AGENT_TMPI,
  UIM_COMMON_EFS_GBA_BOOTSTRAP_HTTP_HEADER_IMEI,

  /* Items owned by PBM */
  UIM_COMMON_EFS_PBM_NV_ECC_LIST = 0x700,
  UIM_COMMON_EFS_PBM_NV_HARDCODED_ECC_CONFIG,
  UIM_COMMON_EFS_PBM_NV_HARDCODED_ECC_LIST,
  UIM_COMMON_EFS_PBM_SIM_ECC_AIRPLANE,
  UIM_COMMON_EFS_PBM_EFS_SUPPORT_DISABLE,
  UIM_COMMON_EFS_PBM_FEATURES_LIST,
  UIM_COMMON_EFS_PBM_NV_ECC_LIST_PER_SUB,
  UIM_COMMON_EFS_PBM_LATEST_MCC,
  UIM_COMMON_EFS_PBM_ADD_SPECIAL_SRV_FLAG,
  
  /* Items owned by LPA */
  UIM_COMMON_EFS_LPA_SUPPORT = 0x800,

  /* Items owned by SimLock */
  UIM_COMMON_EFS_RSU_DEVICE_ID = 0x900,
  UIM_COMMON_EFS_RSU_DELAY_TIMER_VALUE,

  UIM_COMMON_EFS_ITEM_MAX = 0x7FFF
}uim_common_efs_item_type;


/*=============================================================================
   ENUM:      UIM_COMMON_DIR_ITEM_TYPE

   DESCRIPTION:
     Common enums for DIR items accessed by various UIM modules.
=============================================================================*/
typedef enum
{
  /* Items for the directory */
  UIM_COMMON_EFS_MODEM_CONFIG_DIR1,
  UIM_COMMON_EFS_UIMQMI_QMI_CONFIG_DIR2,
  UIM_COMMON_EFS_UIMQMI_QMI_CAT_CONFIG_DIR3,
  UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_DIR4,
  UIM_COMMON_EFS_CONF_DIR1,
  UIM_COMMON_EFS_CONF_DIR2,
  UIM_COMMON_EFS_CONF_DIR3,

  UIM_COMMON_DIR_ITEM_MAX = 0x7F
}uim_common_dir_item_type;

/*=============================================================================
   ENUM:      UIM_COMMON_EFS_CONTEXT_TYPE

   DESCRIPTION:
     Context for the EFS items which maps to the index corresponding to a
     subscription or slot.
=============================================================================*/
typedef enum
{
  UIM_COMMON_EFS_DEVICE      = -1, /* used for device specific EFS items */
  UIM_COMMON_EFS_CONTEXT_0   =  0,
  UIM_COMMON_EFS_CONTEXT_1,
  UIM_COMMON_EFS_CONTEXT_2,
  UIM_COMMON_EFS_CONTEXT_MAX = 0x7F
} uim_common_efs_context_type;


/*=============================================================================
   ENUM:      UIM_COMMON_EFS_STATUS_ENUM_TYPE

   DESCRIPTION:
     Return types for EFS utilities.
=============================================================================*/
typedef enum
{
  UIM_COMMON_EFS_SUCCESS,
  UIM_COMMON_EFS_ERROR
} uim_common_efs_status_enum_type;


/*=============================================================================
   ENUM:      UIM_COMMON_EFS_FILE_TYPE

   DESCRIPTION:
     Type of EFS file.
=============================================================================*/
typedef enum
{
  UIM_COMMON_EFS_REGULAR_FILE_TYPE, /* Files of size greater than FS_ITEM_MAX */
  UIM_COMMON_EFS_ITEM_FILE_TYPE     /* Files of size <= FS_ITEM_MAX that are stored
                                       on a special partition. */
} uim_common_efs_file_type;

/*=============================================================================
   ENUM:      UIM_COMMON_CONF_ITEM_LIST_TYPE

   DESCRIPTION:
     Type of conf file list
=============================================================================*/
typedef struct
{
  uim_common_efs_item_type file;
  boolean                  is_multi_context;
}uim_common_conf_item_list_type;

/*=============================================================================
FUNCTION:      UIM_COMMON_EFS_CREATE_DIR

DESCRIPTION:
  This function will create dir.

PARAMETERS
  dir_item: The enum corresponding to the EFS item/file to be read
  mode:      Permissions for the directory

DEPENDENCIES:
  None

LIMITATIONS:
  None.

RETURN VALUE:
  uim_common_efs_status_enum_type

  UIM_COMMON_EFS_SUCCESS:          The EFS operation was successful.
  UIM_COMMON_EFS_ERROR:            The EFS operation was not successful.

SIDE EFFECTS:
  None
=============================================================================*/
uim_common_efs_status_enum_type uim_common_efs_create_dir(
  uim_common_dir_item_type  dir_item,
  uint16                    mode
);

/*=============================================================================
FUNCTION:      UIM_COMMON_EFS_GET_FILE_SIZE

DESCRIPTION:
  This function will retrieve the size of the EFS file using efs_stat().

PARAMETERS
  efs_item:        The enum corresponding to the EFS item/file to be read
  efs_context:     Context for the EFS item
  file_size_ptr:   Output variable to return file size

DEPENDENCIES:
  None

LIMITATIONS:
  The EFS item should a regular file.

RETURN VALUE:
  uim_efs_status_enum_type

  UIM_EFS_SUCCESS:          The EFS operation was successful.
  UIM_EFS_ERROR:            The EFS operation was not successful.

SIDE EFFECTS:
  None
=============================================================================*/
uim_common_efs_status_enum_type uim_common_efs_get_file_size(
  uim_common_efs_item_type       efs_item,
  uim_common_efs_context_type    efs_context,
  uint32                       * file_size_ptr
);


/*=============================================================================
FUNCTION:      UIM_COMMON_EFS_GET_DIR_STAT

DESCRIPTION:
  This function will retrieve the stats of the Dir using efs_stat().

PARAMETERS
  dir_item:        The enum corresponding to the EFS dir to be accessed
  file_size_ptr:   Output variable to return file size

DEPENDENCIES:
  None

LIMITATIONS:
  None.

RETURN VALUE:
  uim_common_efs_status_enum_type

  UIM_COMMON_EFS_SUCCESS:          The EFS operation was successful.
  UIM_COMMON_EFS_ERROR:            The EFS operation was not successful.

SIDE EFFECTS:
  None
=============================================================================*/
uim_common_efs_status_enum_type uim_common_efs_get_dir_stat(
  uim_common_dir_item_type       dir_item,
  uint32                       * file_size_ptr
);


/*=============================================================================
FUNCTION:      UIM_COMMON_EFS_READ

DESCRIPTION:
  This function will read the EFS file using efs_read() or efs_get()
  depending on the type of file.
  If file type is UIM_EFS_REGULAR_FILE_TYPE, the file is read using efs_read().
  This file type is used to read files which may need to be read partially.
  For UIM_EFS_ITEM_FILE_TYPE file type, the entire file is read at once
  using efs_get(). This file type is used to read the 'special item' files
  of small size (<= FS_ITEM_MAX).

PARAMETERS
  efs_item:        The enum corresponding to the EFS item/file to be read
  file_type:       Type of EFS file to be accessed
  context:         Index of the EFS item for multi-slot settings
  data_ptr:        Buffer to hold the data to be read (to be allocated
                   by the caller)
  data_len:        Size of the buffer

DEPENDENCIES:
  The caller of this API should know the file size and pass the data_len
  parameter accordingly.

LIMITATIONS:
  None

RETURN VALUE:
  uim_efs_status_enum_type

  UIM_COMMON_EFS_SUCCESS:          The EFS operation was successful.
  UIM_COMMON_EFS_ERROR:            The EFS operation was not successful.

SIDE EFFECTS:
  None
=============================================================================*/
uim_common_efs_status_enum_type uim_common_efs_read(
  uim_common_efs_item_type       efs_item,
  uim_common_efs_file_type       file_type,
  uim_common_efs_context_type    context,
  uint8                        * data_ptr,
  uint32                         data_len
);


/*=============================================================================
FUNCTION:      UIM_COMMON_EFS_WRITE

DESCRIPTION:
  This function will write the data to the EFS using efs_write() or efs_put()
  depending on the type of file.
  If file type is UIM_EFS_REGULAR_FILE_TYPE, the data is written to a regular
  EFS file using efs_write(). These type of files can be updated partially.
  For UIM_EFS_ITEM_FILE_TYPE file type, the entire file is overwritten using
  efs_put(). This file type is used to write to 'special item' files
  of small size (<= FS_ITEM_MAX).

PARAMETERS
  efs_item:        The enum corresponding to the EFS item/file
  file_type:       Type of EFS file to be accessed
  context:         Index of the EFS item for multi-slot settings
  data_ptr:        Buffer with the data to be written
  data_len:        Size of the buffer

DEPENDENCIES:
  The caller of this API should know the file size and pass the data_len
  parameter accordingly.

LIMITATIONS:
  None

RETURN VALUE:
  uim_efs_status_enum_type

  UIM_COMMON_EFS_SUCCESS:          The EFS operation was successful.
  UIM_COMMON_EFS_ERROR:            The EFS operation was not successful.

SIDE EFFECTS:
  None
=============================================================================*/
uim_common_efs_status_enum_type uim_common_efs_write(
  uim_common_efs_item_type       efs_item,
  uim_common_efs_file_type       file_type,
  uim_common_efs_context_type    context,
  const char                   * data_ptr,
  uint32                         data_len
);

/*=============================================================================
FUNCTION:      UIM_COMMON_EFS_CREATE_CONF_FILE

DESCRIPTION:
  This function will create config file for EFS items that are accessed in
  multiple contexts.

PARAMETERS
  None

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
=============================================================================*/
void uim_common_efs_create_conf_file
(
  void
);

/*=============================================================================
FUNCTION:      UIM_COMMON_CONVERT_TO_MCFG_CONTEXT

DESCRIPTION:
  This function will convert uim common context type to mcfg context type.

PARAMETERS
  uim_common_context: UIM COMMON context type, passed in by the callers to API

DEPENDENCIES:
  None

LIMITATIONS:
  None.

RETURN VALUE:
  mcfg_fs_sub_id_e_type - MCFG CONTEXT Enum

SIDE EFFECTS:
  None
=============================================================================*/
mcfg_fs_sub_id_e_type uim_common_convert_to_mcfg_context(
  uim_common_efs_context_type  uim_common_context
);

#endif /* UIM_COMMON_EFS_H */
