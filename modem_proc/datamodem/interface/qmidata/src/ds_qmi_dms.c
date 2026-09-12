/*===========================================================================

                         D S _ Q M I _ D M S . C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

EXTERNALIZED FUNCTIONS

  qmi_dms_init()
    Register the DMS service with QMUX for all applicable QMI links
    

Copyright (c) 2004-2019 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/src/ds_qmi_dms.c#8 $ $DateTime: 2023/06/27 06:58:20 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
--------    ---    ----------------------------------------------------------
06/12/15    pg     Fixed LLVM warnings.
05/04/15    sc     Fixed compilation error when feature LTE_BAND_NUM is disabled.
04/11/15    sc     Added support for LTE-U 256 bands.
03/30/15    sah    Added PRL source support in PRL payload.
11/12/14    sk     QMI-DMS Service QCSI migration.
06/10/14    vrk    Updated from deprecated TRM APIs to new APIs
06/10/14    vrk    Added support for SRLTE configuration
05/27/14    wc     Image version reporting
05/19/14    vrk    Added support for current device config through policyman
04/11/14    vrk    Modified get_sw_ver to fetch info from global.
03/19/14    sk     Added new message - DMSI_CMD_GET_ENCRYPTED_DEVICE_SERIAL_NUMBERS.
03/10/14    vrk    Added error code QMI_ERR_DEVICE_NOT_READY to QMI_DMS_SET_OPERATING_MODE_RESP
03/13/14    sah    Added support for tertiary subscription
02/18/14    sah    Modified nv items to report correct phone self ESN and MEID
                   after UIM card is inserted.
02/13/14    vrk    Modified handling of get_ck_state_q
01/16/14    sah    Using Sysmon API for poweroff when phone unlock attempts
                   trials exceeded.
01/16/14    sah    Fix for MSISDN read when additional data is present
                   in extension file.
11/28/13    gk     Added support for get_operator info message
11/27/13    sah    Added support for set_ap_sw_version message.
11/26/13    gk     Added support for get_mac message
09/25/13    gk     In SGLTE mode we should report CS and PS active
08/30/13    gk     Added SGLTE support to simul_voice_and_data_capability TLV
07/16/13    gk     Fixed double prepending of message header
07/09/13    az     Implemented TDSCDMA GET/SET test config support
06/07/13    rk     modified get msisdn message to retrieve proper values 
01/25/13    ua     Poweroff mapping to OFFLINE first is revereted. 
                   Only Poweroff is sent. 
05/21/13    gk     Added support for device_multisim_capability TLV 
05/10/13    ab     Add TTL markers for boot up profiling 
03/18/13    ab     Increased number of dms clients available to 32 
02/05/13    sn     Fixing invalid async command buffer access on client release.
12/11/12    sn     DMS Memory Optimization, moving to new QMI Framework.
10/31/12    svj    Allow hex digits in ICCID numbers 
10/26/12    gk     Reset and Shutdown operating mode changes calls CM API for clients 
                   to handle any mode change processes before actually invoking 
                   Sys Mon API's.
09/10/12    sb     Len checks for QMI TLVs
08/13/12    svj    Add a TLV to indicate device service capability
08/07/12    ab     Add a TLV to indicate simultaneous voice and data capability
08/06/12    gk     Added support for QMI service to log what messages/TLVs  
                   are supported on this branch.
07/18/12    sn     Fix to use Sysmon API for shutdown. For reset, if AT
                   cmd is not forwarded to AP, Sysmon API for reset is used.
06/11/12    sb     Fix to move to correct UIM state in case of CARD ERROR
                   event.
05/31/12    sj     Support SYS_OPRT_MODE_NET_TEST_GW in get 
12/23/11    ua     Route the Operating mode Reset command through 
                   DMS->AT->QMI-AT->Apps Proc. 
04/27/12    sb     Added additional check in qmi_dmsi_set_operating_mode()
04/27/12    sb     Removed feature Wrap FEATURE_DATA_NIKEL
04/20/12    sn     Added TLV for CSFB voice support in device capability msg.
02/28/12    sj     Add support for TDSCDMA 
02/17/12    sb     Fixed Compiler and KW Warnings.
01/25/12    sj     Fix qmi_dmsi_get_hw_supported_radio_if_list and 
                   qmi_dmsi_get_max_channel_rate to use CM mode_capability
                   which is runtime determined
12/13/11    wc     Fix get_msisdn returing NOT_PROVISINOED on multi-mode build
11/02/11    wc     Add qmi_dms_get_current_prl_info
10/10/11    sj     Add support for LTE band capability TLV  
10/04/11    sa     Q6 free floating changes for QMI.
12/09/10    ua     Added new message for Set SPC, and 
                   additional TLV for GET Device Serial Numbers to have the IMEI SVN.
08/03/11    sa     GSDI cleanup changes.
07/14/11    sj     Expose UIM status and PIN1 status to other modules 
06/22/11    wc     Send PRL init indication when PRL is loaded 
05/11/11    sa     Added new message for SW version info.
03/08/11    ua     Retreiving WCDMA/HSDPA/HSUPA default data rates from RRC.
02/21/11    kk     Fix to prevent insignificant bytes in GET_UIM_IMSI.
02/15/11    kk     Added support for PRL_ONLY TLV in PRL_INFO message.
01/10/11    ss     Cleaning up of Globals for thread safety in Q6 Free 
                   Floating environment.
12/17/10    kk     Added log messages before ASSERTs.
12/01/10    ua     Added support for LTE Data rates. 
11/09/10    ua     Extending support of qmi_dmsi_get_msisdn in LTE Mode. 
08/02/10    ack    Added support for setting user time using QMI
06/29/10    kk     Removing bbver.h dependency for BOOT_BLOCK_VERSION.
06/21/10    kk     Fix to align to new MMGSDI APIs for PERSO requests.
06/21/10    kk     Fix to check UIM status before posting commands to MMGSDI.
06/07/10    kk     Fix to update app_type and uim_state from all possible
                   events.
06/07/10    kk     Fix to include HSDPA and HSUPA channel rates in device
                   capabilities.
06/07/10    kk     Fix to prevent accidental memset of get_ck queue.
04/13/10    kk     Fix for GET_UIM_ICCID to report correct error value.
03/18/10    jee    changed FEATURE_MMGSDI to FEATURE_MMGSDI_SESSION_LIB
11/10/09    jee    Changes to support MMGSDI re-architecture
10/16/09    kk     Windows7 compliance - new messages and ATCoP dependencies.
10/09/09    kk     Segregated the internal and externalized functions.
10/08/09    kk     Windows7 compliance - DMS changes.
02/19/09    am     DS Task De-coupling effort and introduction of DCC task.
10/02/06    ks     Fixed PIN1 and PIN2 status TLV types in event_report_ind.
09/26/06    ks     Updated version of DMS to 1.1. Removed pin_id param
                   from pin_status tlvs.
09/06/06    ks     Featurized pin support code, under FEATURE_MMGSDI.
09/05/06    ks     Added support for PIN1/PIN2 operations using QMI.
09/05/06    ks     Added support to handle async responses to DMS.
09/02/06    jd     In production, don't return WLAN in supported radio list
08/23/06    ks     Changes to support multiple qmi/rmnet instances.
07/11/06    ks     Using dsi_get_nv_item() to read from NV. Other clean up.
07/06/06    ks     Removed qmi_dmsi_get_tl(), using qmi_svc_get_tl() instead
06/07/06    ks     Fixed Power state and Battery level reporting
04/05/06    jd     Changed GET_DEVICE_SERIAL_NUMBERS to only return not 
                   provisioned if no serial number is returned.
03/13/06    ks     Changed parsing of requests to return error for fixed 
                   length tlvs when tlv lengh passed is incorrect.
03/13/06    ks     Changed QMI_ERR_INVALID_ARG to QMI_ERR_MALFORMED_MSG.                    
03/06/06    ks     Return QMI_ERR_INVALID_ARG if a malformed TLV is seen.
10/27/05    ks     Removed bat_lvl_lower_limit < BAT_LVL_MIN(0) check as 
                   bat_lvl_lower_limit is unsigned.
09/08/05    ks     Added support for returning imei, MSISDN for UMTS.
08/15/05    ks     Return error when all optional TLVs are absent for messages
                   which don't have any mandatory TLVs.
05/31/05   jd/ks   Code review updates
05/11/05    ks     Code review comments and clean up.
03/14/05    ks     Removed featurization
11/21/04    jd/ks  Created module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#ifndef TEST_FRAMEWORK
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "ttl_map.h"

#include <stdio.h>
#include <string.h>
#include <stringl/stringl.h>

#include "mobile.h"
#include "dsm.h"
#include "nv.h"
#include "msg.h"
#include "err.h"
#include "amssassert.h"

#include "ps_in.h"
#include "ps_byte.h"
#include "dsatctab.h"
#include "dcc_task_defs.h"
#include "dcc_task_svc.h"

#include "ds_qmi_task.h"
#include "ds_qmi_if.h"

#include "ds_qmi_dms.h"
#include "ds_qmi_dms_ext.h"
#include "ds_qmi_defs.h"
#include "ds_qmi_svc.h"
#include "secapi.h"

#include "qmi_charger.h"

#ifdef FEATURE_MMGSDI_SESSION_LIB
  #include "mmgsdilib.h"
  #include "mmgsdisessionlib.h"
#endif /* FEATURE_MMGSDI_SESSION_LIB */

#include "DDIChipInfo.h"

#include "time.h"
#include "time_genoff.h"
#include "smem.h"

#ifdef FEATURE_PREFERRED_ROAMING
   #include "prl.h"
#endif /* FEATURE_PREFERRED_ROAMING */

#include "qw.h"

#include <stringl/stringl.h>
#include "ds_Utils_DebugMsg.h"
#include "ds_qmi_nv_util.h"
#include "ps_system_heap.h"

#include "trm.h"
#include "sys_m_reset.h"

#include "qmi_si.h"
#include "qmi_idl_lib.h"
#include "qmi_csi_target_ext.h"
#include "mqcsi_log.h"
#include "qmi_sap.h"
#include "device_management_service_v01.h"

#include "ps_sys.h"
#include "ps_sys_conf.h"
#include "ps_sys_event.h"
#include "ps_sys_ioctl.h"

#include "policyman.h"
#include "policyman_msg.h"
#include "dsmsgrrecv.h"
#include "ds3gmsgr.h"

#ifdef FEATURE_TDSCDMA
#include "tds_rrc_ext_if.h"
#endif

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
#if (defined FEATURE_OTASP)
#include "cai.h"
#endif /* (defined FEATURE_OTASP) */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
#include "ps_utils.h"

#include "ds3gmgr.h"


#if defined(FEATURE_LTE)
#include "lte_rrc_ext_api.h"
#endif

#if defined(FEATURE_WCDMA)
#ifdef FEATURE_SEGMENT_LOADING
#include "IWCDMA.h"
#else
#include "wcdma_common_api.h"
#endif
#endif

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
#include "mc_v.h"
#endif

#ifdef FEATURE_HDR
#include "hdrcp_api.h"
#endif
#if defined(FEATURE_GSM)
#include "geran_grr_api.h"
#endif

#ifdef FEATURE_TDSCDMA
#ifdef FEATURE_SEGMENT_LOADING
#include "ITDSCDMA.h"
#else
#include "tdsrrcdataext.h"
#endif 
#endif


/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

#define DMSI_MAX_CLIDS    (32)

#define DMSI_QCSI_OS_SIGNAL ((rex_sigs_type)(1 << DCC_DMS_RDR_SIGNAL))

#define DMSI_SERIAL_NO_SIZ  (256)

#define DMSI_BASE_DECIMAL   (10)
#define DMSI_BASE_HEX       (16)
#define DMSI_UIM_PIN1       (1)
#define DMSI_UIM_PIN2       (2)
#define DMSI_MAX_PINS       (2)
#define DMSI_MIN_SIZ        (10)
#define DMS_GSM_RELEASE_VERSION  9
#define WHOLE_FACTORY_MAX_V01 127

/*---------------------------------------------------------------------------
  Policyman Max subs
---------------------------------------------------------------------------*/

#define QMI_DMS_POLICYMAN_MAX_SUBS (3)

/*---------------------------------------------------------------------------
  Constants for IMSI
---------------------------------------------------------------------------*/
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
#define DMSI_IMSI_MCC_0 999        /* 000 */
#define DMSI_IMSI_S1_0  16378855   /* 0000000 */
#define DMSI_IMSI_S2_0  999        /* 000 */
#define DMSI_IMSI_CLASS0_ADDR_NUM 0xFF
#define DMSI_IMSI_11_12_0 99
#define DMSI_FMMIN 0
#define DMSI_CDMAMIN 1
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

#define DMSI_PRM_TYPE_IDLE_MODE     (0x10)
#define DMSI_PRM_TYPE_SLEEP_MODE    (0x11)
#define DMSI_PRM_TYPE_RX_MODE       (0x12)
#define DMSI_PRM_TYPE_TX_MODE       (0x13)

#define DMSI_BAT_LVL_MIN               (0)
#define DMSI_BAT_LVL_MAX               (100)
#define DMSI_BAT_LVL_INACTIVE          (255)
#define DMSI_PWR_STATE_INACTIVE        (0)
#define DMSI_EVENT_MASK_POWER_STATE    (0x01)

/*---------------------------------------------------------------------------
  UIM IMSI length
---------------------------------------------------------------------------*/
#define DMSI_MMGSDI_IMSI_LEN (15)

/*---------------------------------------------------------------------------
  UIM MSISDN length
---------------------------------------------------------------------------*/
#define DMSI_MMGSDI_MSISDN_LEN (15)

/*---------------------------------------------------------------------------
  Invalid PRL version for RUIM
---------------------------------------------------------------------------*/
#define DMSI_PRL_VERSION_INVALID (0xFFFF)

/*---------------------------------------------------------------------------
  Macro used to determine target support
---------------------------------------------------------------------------*/
#define DMSI_TARGET_SUPPORTS_CDMA(mode_capability)  (mode_capability & SYS_SYS_MODE_MASK_CDMA )
#define DMSI_TARGET_SUPPORTS_HDR(mode_capability)  (mode_capability & SYS_SYS_MODE_MASK_HDR )
#define DMSI_TARGET_SUPPORTS_GSM(mode_capability)  (mode_capability & SYS_SYS_MODE_MASK_GSM )
#define DMSI_TARGET_SUPPORTS_WCDMA(mode_capability)  (mode_capability & SYS_SYS_MODE_MASK_WCDMA )
#define DMSI_TARGET_SUPPORTS_LTE(mode_capability)  (mode_capability & SYS_SYS_MODE_MASK_LTE )
#define DMSI_TARGET_SUPPORTS_TDSCDMA(mode_capability)  (mode_capability & SYS_SYS_MODE_MASK_TDS )


/* invalid SPC used to allow certain service provisioning (defined by carrier)
   without knowing the actual SPC*/
#define SPECIAL_INVALID_SPC "999999"

/*---------------------------------------------------------------------------
  mac_address buffer length
---------------------------------------------------------------------------*/
#define DMSI_MAC_ADDR_LEN                        6

//Current threshold is 5 secs
#define MODEM_ACTIVITY_QUERY_THRESHOLD_TIME      5000 

#define DS_QMI_DMS_ASSERT(expression) \
         ds_qmi_dms_assert_wrapper(__LINE__, expression)

/*===========================================================================

                                DATA TYPES

===========================================================================*/

/*---------------------------------------------------------------------------
  Set Event Report event bitmask
---------------------------------------------------------------------------*/
typedef enum
{
  DMSI_REPORT_STATUS_EV_POWER_STATE     = 0x01,
  DMSI_REPORT_STATUS_EV_ACTIVATION_STATE= 0x04,
  DMSI_REPORT_STATUS_EV_OPRT_MODE       = 0x08,
  DMSI_REPORT_STATUS_EV_WD_STATE        = 0x20,
  DMSI_REPORT_STATUS_EV_PRL_INIT        = 0x40,
  DMSI_REPORT_STATUS_EV_CDMA_LOCK_MODE  = 0x80,
  DMSI_REPORT_STATUS_EV_DEV_MSIM_CAP    = 0x100
} qmi_dmsi_report_status_ev_e_type;


#ifdef FEATURE_TDSCDMA
typedef struct
{
  boolean status;
} tdscdma_set_config_res_type;

typedef struct
{
  tds_rrc_config_e_type  active_config;
  tds_rrc_config_e_type  desired_config;
} tdscdma_get_config_res_type;
#endif


/*---------------------------------------------------------------------------
  DMS client sunscription preference enum
---------------------------------------------------------------------------*/
typedef enum
{
  DMSI_BIND_SUBS_PRIMARY   = 0x00,
#ifdef FEATURE_DUAL_SIM
  DMSI_BIND_SUBS_SECONDARY,
#endif
#ifdef FEATURE_TRIPLE_SIM
  DMSI_BIND_SUBS_TERTIARY,
#endif
  DMSI_BIND_SUBS_MAX
} qmi_dms_client_subs_e_type;


/*---------------------------------------------------------------------------
  QMI DMS Client state info
---------------------------------------------------------------------------*/
typedef struct {
  q_link_type              link;  /*< Data Structure required by the queue 
                                      library to add this as part of a 
                                      queue */
  qmi_client_handle        clnt;
  struct 
  {  
    boolean  report_power_state; 
    boolean  report_activation_state;
    boolean  report_operating_mode;
    boolean  report_wd_state;
    boolean  report_prl_init;
    uint8    bat_lvl_lower_limit;
    uint8    bat_lvl_upper_limit;
    uint8    last_bat_lvl_reported;
    uint8    last_power_state_reported;
    boolean  report_cdma_lock_mode; 
    boolean  report_device_mode;
    boolean  report_ims_capability;
    boolean  report_device_state;
  } report_status;
  qmi_dms_client_subs_e_type subscription_id;
}qmi_dmsi_client_info_type;

typedef struct
{
  q_link_type    link;
  qmi_req_handle req_handle;
  qmi_dmsi_client_info_type* cl_sp;
  int            msg_id;
  uint32         user_data;
}ds_qmi_dms_async_cmd_type;

/*---------------------------------------------------------------------------
  QMI DMS Service state info
---------------------------------------------------------------------------*/
typedef struct
{
  q_type                 client_q; /*< Information about all the clients that
                                       have connected to the service. This is queue 
                                       of type qmi_dmsi_client_info_type */
  qmi_csi_service_handle service_handle;
  qmi_sap_client_handle  sap_client_handle;
  uint16                 num_of_clients;
#ifdef FEATURE_TDSCDMA
  q_type                 set_tdscdma_config_pending_q;
  q_type                 get_tdscdma_config_pending_q;
#endif
  q_type                 modem_activity_pend_q;
  uint64                 last_modem_activity_query_time;
} qmi_dmsi_state_info_type;


typedef struct
{
  ps_sys_modem_power_stats_info_type  power_stats_info;
  boolean                             is_cached_value;
} qmi_dmsi_power_stats_info_type;

typedef struct
{
  uint16 cmd_id;
  uint32 subscription_id;
  union
  {
    struct
    {
      qmi_dmsi_report_status_ev_e_type         evt_type;
      void                                    *evt_value;
      qmi_dmsi_client_info_type*               cl_sp;
    }dms_evt_info;

    struct
    {
      qmi_if_cm_ph_info_type  info;
      cm_ph_event_e_type      event;
    } cm_ph;

    struct
    {
      void *                user_data;
      cm_ph_cmd_err_e_type  err_type;
    } cm_ph_err;

    struct
    {
      void *                 user_data;
      cm_call_cmd_e_type     call_cmd;
      cm_call_cmd_err_e_type err_type;
    } cm_call_err;

    qmi_if_mmgsdi_evt_info_type mmgsdi_evt_info;
	
    qmi_dmsi_power_stats_info_type modem_power_stats;
	
#ifdef FEATURE_TDSCDMA
    tdscdma_set_config_res_type tdscdma_set_config_res;
    tdscdma_get_config_res_type tdscdma_get_config_res;
#endif
  } data;
} qmi_dmsi_cmd_buf_type;


#ifdef FEATURE_PREFERRED_ROAMING

extern nv_roaming_list_type nv_pr_list;  /* buffer to hold preferred
                                            roaming list table being
                                            sent to or read from NV
                                          */
#endif /* FEATURE_PREFERRED_ROAMING */

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
#ifdef FEATURE_PREFERRED_ROAMING
extern prl_sspr_p_rev_e_type sspr_p_rev; /* PRL protocol revision */
#endif /* FEATURE_PREFERRED_ROAMING */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

static qmi_csi_os_params             os_params;
static boolean                       qmi_dmsi_inited = FALSE;

typedef struct
{
  q_link_type                   link;  //must be first
  qmi_dmsi_client_info_type *  cl_sp;
}qmi_dmsi_power_stats_pend_q_type;


typedef enum
{
  QMI_DMSI_LOCAL_ERR_NONE = 0,
  QMI_DMSI_LOCAL_ERR_INVALID_ASYNC_BUF,
  QMI_DMSI_LOCAL_ERR_NULL_CLIENT_DATA_PTR,
  QMI_DMSI_LOCAL_ERR_INVAL_MMGSDI_CNF_TYPE,
  QMI_DMSI_LOCAL_ERR_INVAL_MMGSDI_READ_CNF_TYPE,
  QMI_DMSI_LOCAL_ERR_IGNORE_MMGSDI_SESSION_CHNG_EVT,
  QMI_DMSI_LOCAL_ERR_SAP_CSI_REGISTER_FAIL,
  QMI_DMSI_LOCAL_ERR_CLIENT_NOT_FOUND,
  QMI_DMSI_LOCAL_ERR_Q_DELETE_FAIL,
  QMI_DMSI_LOCAL_ERR_NO_MEMORY,
  QMI_DMSI_LOCAL_ERR_OP_DEVICE_UNSUPPORTED,
  QMI_DMSI_LOCAL_ERR_MMGSDI_MSISDN_FAIL,
  QMI_DMSI_LOCAL_ERR_SECAPI_FAIL,
  QMI_DMSI_LOCAL_ERR_INVAL_IMEI,
  QMI_DMSI_LOCAL_ERR_NULL_IMEI,
  QMI_DMSI_LOCAL_ERR_NULL_ESN,
  QMI_DMSI_LOCAL_ERR_NULL_MEID,
  QMI_DMSI_LOCAL_ERR_NULL_IMEISV,
  QMI_DMSI_LOCAL_ERR_NOT_PROVISIONED,
  QMI_DMSI_LOCAL_ERR_ADM_PROTECTED_RUIM,
  QMI_DMSI_LOCAL_ERR_SPECIAL_SPC_MIP_NOT_ALLOWED,
  QMI_DMSI_LOCAL_CHECK_SPC_FAIL,
  QMI_DMSI_LOCAL_ERR_INVAL_HA_LEN,
  QMI_DMSI_LOCAL_ERR_INVAL_AAA_LEN,
  QMI_DMSI_LOCAL_ERR_INVAL_MIN_LEN,
  QMI_DMSI_LOCAL_ERR_INVAL_MIN_CONTENTS,
  QMI_DMSI_LOCAL_ERR_PRL_ALREADY_WRITE,
  QMI_DMSI_LOCAL_ERR_PRL_TOTAL_LEN,
  QMI_DMSI_LOCAL_ERR_INVAL_PRL_LEN,
  QMI_DMSI_LOCAL_ERR_INVAL_PRL_SEGMENT_LEN,
  QMI_DMSI_LOCAL_ERR_PRL_SEGMENT_LEN,
  QMI_DMSI_LOCAL_ERR_SEGMENT_ORDER,
  QMI_DMSI_LOCAL_ERR_INVAL_PRL,
  QMI_DMSI_LOCAL_ERR_PRL_INFO_LEN,
  QMI_DMSI_LOCAL_ERR_INTERNAL,
  QMI_DMSI_LOCAL_ERR_MDN,
  QMI_DMSI_LOCAL_ERR_INVAL_FILE_ENUM,
  QMI_DMSI_LOCAL_ERR_INVAL_MSISDN_LEN,
  QMI_DMSI_LOCAL_ERR_INVAL_MSISDN_BCD_FORMAT,
  QMI_DMSI_LOCAL_ERR_EXT_UIM_ONLY,
  QMI_DMSI_LOCAL_ERR_INVAL_READ_DATA_LEN,
  QMI_DMSI_LOCAL_ERR_MMGSDI_EXT_FAIL,
  QMI_DMSI_LOCAL_ERR_POWER_STATS_NOT_CONFIG,
  QMI_DMSI_LOCAL_ERR_SYS_CONF_FAIL,
  QMI_DMSI_LOCAL_ERR_UNSUPPORTED_MSGR_TYPE,
  QMI_DMSI_LOCAL_ERR_UNKNOWN_CALL_EVENT,
  QMI_DMSI_LOCAL_ERR_NULL_CLIENT,
  QMI_DMSI_LOCAL_ERR_TDSCDMA_DESIRED_CONFIG_FAIL,
  QMI_DMSI_LOCAL_ERR_TDSCDMA_ACTIVE_CONFIG_FAIL,
  QMI_DMSI_LOCAL_ERR_INVAL_CONFIG_STATUS,
  QMI_DMSI_LOCAL_ERR_SIM_UNINIT,
  QMI_DMSI_LOCAL_ERR_MMGSDI_IMSI_FAIL,
  QMI_DMSI_LOCAL_ERR_OTSAP_SPC_RETRIES_MAX_VAL
} qmi_dmsi_local_err_type; 

/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS 

===========================================================================*/

/*---------------------------------------------------------------------------
  QMI service command handlers
  forward declarations & cmd handler dispatch table definition
---------------------------------------------------------------------------*/

static qmi_csi_cb_error  qmi_dmsi_reset(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error qmi_dmsi_bind_subscription(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error qmi_dmsi_get_bind_subscription(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_set_event_report(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_device_cap(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_device_mfr(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_device_model_id(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_device_rev_id(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_msisdn(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_device_serial_numbers(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_power_state(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_device_hardware_rev(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_operating_mode(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_set_operating_mode(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_time(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_set_time(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_current_prl_info(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_activated_state(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_activate_automatic(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_activate_manual(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_read_eri_file(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_restore_factory_defaults(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_validate_spc(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_band_capability(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_factory_sku(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_sw_version(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_set_spc(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_set_ap_sw_version(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_cdma_lock_mode(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error qmi_dmsi_set_test_config_req(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error qmi_dmsi_get_test_config_req(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error qmi_dmsi_get_oem_china_operator(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error qmi_dmsi_get_mac_address(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);
static qmi_csi_cb_error  qmi_dmsi_get_encrypted_device_serial_numbers(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

static qmi_csi_cb_error qmi_dmsi_trigger_modem_activity_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

static qmi_csi_cb_error qmi_dmsi_get_modem_activity_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

static qmi_csi_cb_error qmi_dmsi_get_ui_status_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

static qmi_csi_cb_error qmi_dmsi_set_ui_status_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

static qmi_csi_cb_error qmi_dmsi_set_device_cap_config_req
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

static qmi_csi_cb_error qmi_dmsi_indication_register(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

static qmi_error_type_v01 qmi_dmsi_get_device_serial_numbers_ext(
                           boolean,
                           void*,
                           qmi_dmsi_client_info_type *);

static void qmi_dmsi_initialize_client
(
  qmi_dmsi_client_info_type * cl_sp
);

static boolean qmi_dmsi_msg_hdlr_cb
(
  dcc_sig_enum_type  sig,
  void              *svc_ptr
);

static qmi_csi_cb_error qmi_dmsi_svc_connect_cb
(
  qmi_client_handle          client_handle,
  void                      *service_handle,
  void                     **connection_handle
);

static void qmi_dmsi_svc_disconnect_cb
(
  void                      *connection_handle,
  void                      *service_handle
);

static qmi_csi_cb_error qmi_dmsi_handle_client_req_cb
(
  void                    *connection_handle,
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void                    *req_c_struct,
  unsigned int             req_c_struct_len,
  void                    *service_handle
);

static qmi_csi_cb_error  qmi_dmsi_set_ap_version(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

boolean qmi_dms_check_valid_async_cmd_buf
(
  ds_qmi_dms_async_cmd_type *async_cmd_buf
);

static int dmsi_get_device_ims_capability_from_pm
(
  void  * ims_capability
);

static qmi_csi_cb_error  qmi_dmsi_get_device_state_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
);

//for SN
static qmi_csi_cb_error qmi_dmsi_get_whole_2498
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
	
);
	
	
static qmi_csi_cb_error qmi_dmsi_set_whole_2498
	
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
);
	
	
//for product ID
static qmi_csi_cb_error qmi_dmsi_get_whole_2497
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
);
	
	
static qmi_csi_cb_error qmi_dmsi_set_whole_2497
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
);
	
	
//for module No
static qmi_csi_cb_error qmi_dmsi_get_whole_2499
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
);
	
	
static qmi_csi_cb_error qmi_dmsi_set_whole_2499
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
);
	
static qmi_csi_cb_error qmi_dmsi_get_whole_2500
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
);

static qmi_csi_cb_error qmi_dmsi_set_whole_2500
(
  qmi_dmsi_client_info_type* clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void*                      req_c_struct,
  int                        req_c_struct_len,
  void*                      service_handle
);



/* Define service handle table for DMS messages */
static qmi_csi_cb_error (* const req_handle_table[])
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
) = 
{
 #ifndef REMOVE_QMI_DMS_RESET_V01
   qmi_dmsi_reset                             ,  /* Request handler for message ID 0x00*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_SET_EVENT_REPORT_V01
   qmi_dmsi_set_event_report                  ,  /* Request handler for message ID 0x01*/
 #else
   NULL,
 #endif
  NULL, /*0x02*/
  qmi_dmsi_indication_register, /*0x03*/
  NULL,NULL,NULL,NULL,NULL,NULL,  /* Request handler for message ID 0x04 to 0x09*/
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL    ,  /* Request handler for message ID 0x0A to 0x11*/
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL    ,  /* Request handler for message ID 0x12 to 0x19*/
  NULL,NULL,NULL,NULL,NULL,NULL              ,  /* Request handler for message ID 0x1A to 0x1F*/
 #ifndef REMOVE_QMI_DMS_GET_DEVICE_CAP_V01
   qmi_dmsi_get_device_cap                    ,  /* Request handler for message ID 0x20*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_DEVICE_MFR_V01
   qmi_dmsi_get_device_mfr                    ,  /* Request handler for message ID 0x21*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_DEVICE_MODEL_ID_V01
   qmi_dmsi_get_device_model_id               ,  /* Request handler for message ID 0x22*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_DEVICE_REV_ID_V01
   qmi_dmsi_get_device_rev_id                 ,  /* Request handler for message ID 0x23*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_MSISDN_V01
   qmi_dmsi_get_msisdn                        ,  /* Request handler for message ID 0x24*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_V01
   qmi_dmsi_get_device_serial_numbers         ,  /* Request handler for message ID 0x25*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_POWER_STATE_V01
    qmi_dmsi_get_power_state                   ,  /* Request handler for message ID 0x26*/
 #else
    NULL,
 #endif
  NULL,NULL,NULL,NULL,NULL                   ,  /* Request handler for message ID 0x27 to 0x02B*/
 #ifndef REMOVE_QMI_DMS_GET_DEVICE_HARDWARE_REV_V01
   qmi_dmsi_get_device_hardware_rev           ,  /* Request handler for message ID 0x2C*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_OPERATING_MODE_V01
   qmi_dmsi_get_operating_mode                ,  /* Request handler for message ID 0x2D*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_SET_OPERATING_MODE_V01
   qmi_dmsi_set_operating_mode                ,  /* Request handler for message ID 0x2E*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_TIME_V01
   qmi_dmsi_get_time                          ,  /* Request handler for message ID 0x2F*/
 #else
   NULL,
 #endif
   NULL                                       ,  /* Request handler for message ID 0x30*/
 #ifndef REMOVE_QMI_DMS_GET_ACTIVATION_STATE_V01
   qmi_dmsi_get_activated_state               ,  /* Request handler for message ID 0x31*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_ACTIVATE_AUTOMATIC_V01
   qmi_dmsi_activate_automatic                ,  /* Request handler for message ID 0x32*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_ACTIVATE_MANUAL_V01
   qmi_dmsi_activate_manual                   ,  /* Request handler for message ID 0x33*/
 #else
   NULL,
 #endif
   NULL                                       ,  /* Request handler for message ID 0x34*/
   NULL                                       ,  /* Request handler for message ID 0x35*/
   NULL                                       ,  /* Request handler for message ID 0x36*/
   NULL                                       ,  /* Request handler for message ID 0x37*/
   NULL                                       ,  /* Request handler for message ID 0x38*/
 #ifndef REMOVE_QMI_DMS_READ_ERI_FILE_V01
   qmi_dmsi_read_eri_file                     ,  /* Request handler for message ID 0x39*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_RESTORE_FACTORY_DEFAULTS_V01
   qmi_dmsi_restore_factory_defaults          ,  /* Request handler for message ID 0x3A*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_VALIDATE_SERVICE_PROGRAMMING_CODE_V01
   qmi_dmsi_validate_spc                      ,  /* Request handler for message ID 0x3B*/
 #else
   NULL,
 #endif
  NULL,NULL,NULL,NULL                        ,  /* Request handler for message ID 0x3C to 0X3F*/
  NULL,NULL,NULL,NULL,NULL                   ,  /* Request handler for message ID 0x40 to 0X44*/
 #ifndef REMOVE_QMI_DMS_GET_BAND_CAPABILITY_V01
   qmi_dmsi_get_band_capability               ,  /* Request handler for message ID 0x45*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_FACTORY_SKU_V01
   qmi_dmsi_get_factory_sku                   ,  /* Request handler for message ID 0x46*/
 #else
   NULL,
 #endif
  NULL,NULL,NULL,NULL                        ,  /* Request handler for message ID 0x47 to 4A*/
 #ifndef REMOVE_QMI_DMS_SET_TIME_V01
   qmi_dmsi_set_time                          ,  /* Request handler for message ID 0x4B*/
 #else
   NULL,
 #endif
  NULL                                       ,  /* Request handler for message ID 0x4C*/
  NULL                                       ,  /* Request handler for message ID 0x4D*/
  NULL                                       ,  /* Request handler for message ID 0x4E*/
  NULL,NULL                                  ,  /* Request handler for message ID 0x4F to 0x50*/
 #ifndef REMOVE_QMI_DMS_GET_SW_VERSION_V01
   qmi_dmsi_get_sw_version                    ,  /* Request handler for message ID 0x51*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_SET_SPC_V01
   qmi_dmsi_set_spc                           ,  /* Request handler for message ID 0x52*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_CURRENT_PRL_INFO_V01
   qmi_dmsi_get_current_prl_info              ,  /* Request handler for message ID 0x53*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_BIND_SUBSCRIPTION_V01
   qmi_dmsi_bind_subscription                 ,  /* Request handler for message ID 0x54*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_BIND_SUBSCRIPTION_V01
   qmi_dmsi_get_bind_subscription             ,  /* Request handler for message ID 0x55*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_SET_AP_SW_VERSION_V01
   qmi_dmsi_set_ap_sw_version                 ,  /* Request handler for message ID 0x56*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_CDMA_LOCK_MODE_V01
   qmi_dmsi_get_cdma_lock_mode                ,  /* Request handler for message ID 0x57*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_SET_TEST_CONFIG_V01
   qmi_dmsi_set_test_config_req               ,  /* Request handler for message ID 0x58*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_TEST_CONFIG_V01
   qmi_dmsi_get_test_config_req               ,  /* Request handler for message ID 0x59*/
 #else
   NULL,
 #endif
  NULL                                        ,  /* Request handler for message ID 0x5A*/
 #ifndef REMOVE_QMI_DMS_OEM_CHINA_OPERATOR_V01
   qmi_dmsi_get_oem_china_operator            ,  /* Request handler for message ID 0x5B*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_MAC_ADDRESS_V01
   qmi_dmsi_get_mac_address                   ,  /* Request handler for message ID 0x5C*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_ENCRYPTED_DEVICE_SERIAL_NUMBERS_V01
   qmi_dmsi_get_encrypted_device_serial_numbers  ,/* Request handler for message ID 0x5D*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION_REQ_V01
   qmi_dmsi_trigger_modem_activity_info  ,/* Request handler for message ID 0x5E*/
 #else
   NULL,
 #endif
 #ifndef REMOVE_QMI_DMS_GET_MODEM_ACTIVITY_INFO_REQ_V01
   qmi_dmsi_get_modem_activity_info  ,/* Request handler for message ID 0x5F*/
 #else
   NULL,
 #endif

 NULL,/* Request handler for message ID 0x60*/
 NULL,/* Request handler for message ID 0x61*/
 NULL,/* Request handler for message ID 0x62*/
 
 #ifndef REMOVE_QMI_DMS_GET_UI_STATUS_INFO_V01
   qmi_dmsi_get_ui_status_info  ,/* Request handler for message ID 0x63*/
 #else
   NULL,
 #endif

#ifndef REMOVE_QMI_DMS_SET_UI_STATUS_INFO_V01
   qmi_dmsi_set_ui_status_info  ,/* Request handler for message ID 0x64*/
#else
   NULL,
#endif
#ifndef REMOVE_QMI_DMS_SET_DEVICE_CAP_CONFIG_V01
   qmi_dmsi_set_device_cap_config_req  ,/* Request handler for message ID 0x65*/
#else
   NULL,
#endif

 NULL, /* Request handler for message ID 0x66*/
 NULL, /* Request handler for message ID 0x67*/
 NULL, /* Request handler for message ID 0x68*/

#ifndef REMOVE_QMI_DMS_SET_AP_OS_VERSION_V01
   qmi_dmsi_set_ap_version  ,/* Request handler for message ID 0x69*/
#else
   NULL,
#endif
  NULL,   /* Request handler for message ID 0x6A*/
  NULL,   /* Request handler for message ID 0x6B*/
  NULL,   /* Request handler for message ID 0x6C*/
  NULL,   /* Request handler for message ID 0x6D*/
  NULL,   /* Request handler for message ID 0x6E*/
  NULL,   /* Request handler for message ID 0x6F*/
  NULL,   /* Request handler for message ID 0x70*/
  NULL,   /* Request handler for message ID 0x71*/
  NULL,   /* Request handler for message ID 0x72*/
  NULL,   /* Request handler for message ID 0x73*/
  NULL,   /* Request handler for message ID 0x74*/
  NULL,   /* Request handler for message ID 0x75*/
  NULL,   /* Request handler for message ID 0x76*/
  NULL,   /* Request handler for message ID 0x77*/
  NULL,   /* Request handler for message ID 0x78*/
  NULL,  /* Request handler for message ID 0x79*/
  NULL,  /* Request handler for message ID 0x7A*/
  NULL,  /* Request handler for message ID 0x7B*/
  qmi_dmsi_get_device_state_info,   /* Request handler for message ID 0x7C*/
  NULL,  /* Request handler for message ID 0x7D*/
  NULL,  /* Request handler for message ID 0x7E*/
  NULL,  /* Request handler for message ID 0x7F*/
  qmi_dmsi_get_whole_2498,   /* Request handler for message ID 0x80*/
  qmi_dmsi_set_whole_2498,   /* Request handler for message ID 0x81*/
  qmi_dmsi_get_whole_2497,   /* Request handler for message ID 0x82*/
  qmi_dmsi_set_whole_2497,   /* Request handler for message ID 0x83*/
  qmi_dmsi_get_whole_2499,   /* Request handler for message ID 0x84*/
  qmi_dmsi_set_whole_2499,   /* Request handler for message ID 0x85*/
  qmi_dmsi_get_whole_2500,   /* Request handler for message ID 0x86*/
  qmi_dmsi_set_whole_2500   /* Request handler for message ID 0x87*/
};

static void qmi_dmsi_modem_activity_info_ind
(
  qmi_dmsi_client_info_type             *clnt_info,
  qmi_dmsi_power_stats_info_type        *activity_info
);

static void qmi_dms_generate_set_ui_status_resp
(
  void *               user_data,
  cm_ph_cmd_err_e_type ph_cmd_err
);

static void qmi_dmsi_reg_sys_events
( 
  void
);

static void dmsi_power_statistics_info_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
);

static void dmsi_power_statistics_info_cb_ex
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr,
  boolean                                        is_cached_value
);

static void dsdi_policyman_msg_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
);

extern rex_tcb_type dcc_tcb;

static int dmsi_compare_qmi_client_handle
( 
  void *item_ptr,
  void *compare_val 
)
{
  qmi_dmsi_client_info_type *clnt_info = (qmi_dmsi_client_info_type*)item_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return ( (clnt_info->clnt == 
                          (qmi_client_handle)compare_val) ? 1 : 0 );
}

static int dmsi_compare_connection_info_handle
( 
  void *p_ItemPtr,
  void *p_CompareVal 
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* item_ptr is an element of the Client Q i.e. Connection Handle, 
     and compare val is also a Connection Handle */
  return ( (p_ItemPtr == p_CompareVal) ? 1 : 0 );
}

static int dmsi_cmd_buf_belongs_to_client
(
  ds_qmi_dms_async_cmd_type *async_cmd_buf,
  qmi_dmsi_client_info_type *cl_sp
)
{
  return (async_cmd_buf->cl_sp == cl_sp)?TRUE:FALSE;
}

/*===========================================================================

FUNCTION DS_QMI_DMS_ASSERT_WRAPPER()

DESCRIPTION
  Wrapper function for DS_QMI_DMS ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_qmi_dms_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DS_QMI_DMS_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_qmi_dms_assert_wrapper */

static qmi_dmsi_state_info_type  qmi_dms_state;

/*---------------------------------------------------------------------------
  QMI instance configurations
---------------------------------------------------------------------------*/
static void     qmi_dmsi_reset_client(qmi_dmsi_client_info_type *);



typedef enum
{
  QMI_DMSI_ACTIVATION_TYPE_CALL  = 0,
  QMI_DMSI_ACTIVATION_TYPE_OTASP = 1,
  QMI_DMSI_ACTIVATION_TYPE_MAX,
  QMI_DMSI_ACTIVATION_TYPE_MAX32 = 0x10000000
} qmi_dmsi_activation_e_type;

typedef enum {
  DMS_DEVICE_MAC_WLAN,      /**< WLAN device MAC address \n */
  DMS_DEVICE_MAC_BT,        /**< Bluetooth device MAC address \n */
  DMS_DEVICE_MAX
}qmi_dmsi_device_mac_e_type;

typedef enum {
  DMS_OPERATOR_CT,
  DMS_OPERATOR_CU,
  DMS_OPERATOR_CMCC,
  DMS_OPERATOR_MAX
}qmi_dmsi_oem_china_operator_enum;

typedef struct
{
  qmi_dmsi_activation_e_type    act_type;
  union
  {
    cm_call_event_e_type     call_event;
#if (defined FEATURE_OTASP)
    cm_otasp_status_e_type   otasp_state;
#endif /* (defined FEATURE_OTASP) */
  } event;
} qmi_dmsi_activation_evt_info_type;

typedef struct 
{
  /* control point that is currently writing PRL information */
  qmi_dmsi_client_info_type *cl_sp;

  /* total PRL length */
  uint16 len_total;
  /* PRL length received so far */
  uint16 len_received;
  /* next expected sequence number */
  uint8  seq_num;
  /* PRL data received so far */
  uint8  *data;
} qmi_dmsi_prl_segmented_type;

/*---------------------------------------------------------------------------
  QMI_DMS Global state information - initialized in qmi_dms_init()
  Global to all QMI instances
---------------------------------------------------------------------------*/
static struct
{ 
  boolean inited;
  /*-------------------------------------------------------------------------
    Periodic timer to poll battery level - export to OEM_CHARGER module
  -------------------------------------------------------------------------*/
  struct
  {
    uint8  low_limit;
    uint8  high_limit;
    uint8  report_power_state_client_count;
  } pwr_rpt;
#ifdef FEATURE_MMGSDI_SESSION_LIB
  dms_uim_state_enum_v01 uim_state;
#endif /* FEATURE_MMGSDI_SESSION_LIB */

  cm_call_id_type               voice_call_id;

  qmi_dmsi_activation_evt_info_type act_evt;

  /* Segmented PRL data */
  qmi_dmsi_prl_segmented_type prl_info;
  
  dms_activation_state_enum_v01 activation_state;

  uint32 clients_created;  
  /* used to store NV config values so they only have to be read once 
     at startup */
  boolean nv_allow_invalid_spc;
  boolean nv_restrict_imsi_access;
  boolean nv_use_max_defined_data_rates;
  boolean nv_use_last_otasp_act_state;
  boolean cdma_lock_mode;
  boolean ui_ready_status;
} qmi_dmsi_global = {FALSE,};

/*---------------------------------------------------------------------------
  Image version reporting
---------------------------------------------------------------------------*/
#define DMSI_PRM_TYPE_IMAGE_VER 0x10

#define IMAGE_INDEX_LEN     2
#define IMAGE_QC_VER_LEN   72
#define IMAGE_VARIANT_LEN  20
#define IMAGE_OEM_VER_LEN  32
#define IMAGE_VER_STR_SIZE 128

/* Image version format 128 bytes */
typedef PACKED struct PACKED_POST
{
  char index[IMAGE_INDEX_LEN];      // Image index
  char colon1;                      // ':'
  char qc_ver[IMAGE_QC_VER_LEN];    // QC image version string
  char variant[IMAGE_VARIANT_LEN];  // Variant
  char colon2;                      // ':'
  char oem_ver[IMAGE_OEM_VER_LEN];  // OEM version string
} qmi_dmsi_image_ver_type;


/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS 

===========================================================================*/
static void qmi_dmsi_process_cmd
(
  dcc_cmd_enum_type  cmd,
  void *cmd_ptr
);

static void qmi_dmsi_process_svc_init(void);

static void *qmi_dmsi_get_cmd_buf(void);

static void qmi_dmsi_event_report_ind
(
  qmi_dmsi_client_info_type *  sp,
  uint32   event_mask,
  uint8    power_status,  /* bitmask */
  uint8    battery_lvl,
  uint8    pin_index,
  uint16   activation_state,
  uint8    operating_mode,
  boolean  wd_state,
  boolean  cdma_lock_mode 
);
static uint8 qmi_dmsi_get_hw_supported_radio_if_list(uint32 *);
static qmi_error_type_v01 qmi_dmsi_get_max_channel_rate(uint32 *, uint32 *);
static uint8 qmi_dmsi_get_voice_capability(void);
static uint8 qmi_dmsi_get_sim_capability(void);
static uint64 qmi_dmsi_get_voice_support_capability(void);
static uint64 qmi_dmsi_get_simul_voice_and_data_capability(void);
static void qmi_dmsi_set_battery_level_limits(uint8);
static qmi_error_type_v01 qmi_dmsi_get_multisim_info
                          (dms_multisim_capability_type_v01*,
                          policyman_item_t const *cm_pm_device_config);
static qmi_error_type_v01 qmi_dmsi_get_device_overall_capability
                          (sys_overall_feature_t*);

static qmi_error_type_v01 qmi_dmsi_get_device_config_list
(
  dms_device_config_capability_type_v01* tlv_cfg_list,
  int16_t* tlv_explicit_index,
  policyman_item_t const *cm_pm_device_config
);

static qmi_error_type_v01  qmi_dmsi_get_max_subs_static_device_cap
(
  dms_get_device_cap_resp_msg_v01* response
);

static dms_device_tdscdma_rel_info_v01 dmsi_get_tdscdmsa_rel_version
(
  uint32 subs_index
);

static dms_device_lte_rat_rel_info_v01 dmsi_get_lte_rel_version
(
  uint32 subs_index
);

static dms_device_wcdma_rel_info_v01 dmsi_get_wcdma_rel_version
(
   uint32 subs_index
);

static dms_device_gsm_rel_info_v01 dmsi_get_gsm_rel_version
(
   uint32 subs_index
);

static dms_device_hdr_rat_rel_info_v01 dmsi_get_hdr_rel_version
(
   uint32 subs_index
);

static dms_device_cdma_1x_rel_info_v01 dmsi_get_cdma_rel_version
(
   uint32 subs_index
);

static qmi_error_type_v01  qmi_dmsi_get_rat_release_version_info
(
  dms_get_device_cap_resp_msg_v01* response
);


#ifdef FEATURE_MMGSDI_SESSION_LIB
  static void qmi_dmsi_generate_read_uim_msisdn_resp
  (
    mmgsdi_read_cnf_type * read_cnf,
    ds_qmi_dms_async_cmd_type* cmd_buf_p
  );
#endif /* FEATURE_MMGSDI_SESSION_LIB */
static qmi_error_type_v01 qmi_dmsi_oprt_mode_from_cm
(
  sys_oprt_mode_e_type,
  dms_operating_mode_enum_v01 *
);

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  static qmi_error_type_v01 qmi_dmsi_get_mdn_min(char *,int,char *,int,uint16);
  static boolean qmi_dmsi_minnum_validate(dword, word);
  static dword qmi_dmsi_generate_default_min(dword);
  static int qmi_dmsi_minnum_pcs_to_text(char *, dword, word);
  static void  qmi_dmsi_mobnum_pcs_to_text(char *, nv_mob_dir_data_type *);
  static boolean qmi_dmsi_mobnum_text_to_pcs(char*, uint8, nv_mob_dir_data_type *);
  static boolean qmi_dmsi_min_text_to_min12(char*, uint8, dword*, word*);
  static boolean qmi_dmsi_reset_imsi_components(byte nam);
  static boolean qmi_dmsi_reset_imsi_t_components(byte nam);
  static void qmi_dmsi_generate_activation_state_ind(void);
  static dms_activation_state_enum_v01 qmi_dmsi_calculate_activated_state(
                                                                          void);
#if (defined FEATURE_OTASP)
  static void qmi_dmsi_otasp_forced_call_release(void);
#endif /* (defined FEATURE_OTASP) */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

  static void qmi_dmsi_reset_prl_series_info(void);

void qmi_dms_event_report_req_send_ind
(
   qmi_dmsi_report_status_ev_e_type               event_type,
   void                                         * event_info,
   qmi_dmsi_client_info_type*                     cl_sp
);

#ifdef FEATURE_TDSCDMA
static void qmi_dmsi_tdscdma_config_init(void);
static qmi_error_type_v01 qmi_dmsi_tdscdma_get_config_val
(
  tds_rrc_config_e_type config,
  dms_test_config_tds_enum_v01 *val
);
static void qmi_dmsi_generate_set_test_config_resp(tdscdma_set_config_res_type *config_resp);
static void qmi_dmsi_generate_get_test_config_resp(tdscdma_get_config_res_type *config_resp);
#endif

static qmi_error_type_v01 qmi_dmsi_get_device_multisim_current_cap_info
(
  void* response,
  uint8 resp_type,
  policyman_item_t const *cm_pm_device_config

);

static dms_subs_voice_data_capability_enum_v01 qmi_dmsi_is_simul_voice_data 
(
   sys_ue_mode_e_type ue_mode 
);

void qmi_dmsi_msgr_hdlr 
(
  const msgr_hdr_struct_type *dsmsg,
  void  *user_data
);

static void qmi_dmsi_policyman_config_init (void);

static uint64 qmi_dmsi_convert_rat_mask (uint32 rat);

static void qmi_dmsi_generate_current_device_capability_ind( void );

static qmi_error_type_v01 qmi_dmsi_get_subs_device_feature_mode
(
  dms_get_device_cap_resp_msg_v01* response
);

static qmi_error_type_v01 qmi_dmsi_get_max_active_data_subs
(
  uint8_t * response,
  policyman_item_t const *cm_pm_device_config
);

void qmi_dmsi_send_response
(
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void*                    resp_msg,
  unsigned int             c_struct_len
);

static void qmi_dmsi_generate_power_stats_ind( qmi_dmsi_power_stats_info_type  *activity_info );

int qmi_dms_is_client_power_stats_query_pending
(
  qmi_dmsi_client_info_type *  q_cl_cp,
  qmi_dmsi_client_info_type *  delete_cl_sp
);

static void qmi_dmsi_generate_ims_capability_ind
(
  dms_ims_capability_ind_msg_v01  * ind_msg,
  qmi_dmsi_client_info_type       * cl_sp
);

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_DMS_CHECK_VALID_ASYNC_CMD_BUF()

  DESCRIPTION
    This Function validates the async command buffer passed.
    It chcks whether cl_sp in async command buffer is valid by comparing it with
    clients queue in dms state.
    
  PARAMETERS
    async_cmd_buf : pointer to the async command buffer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_dms_check_valid_async_cmd_buf
(
  ds_qmi_dms_async_cmd_type *async_cmd_buf
)
{  
  qmi_dmsi_client_info_type * cl_sp = NULL;
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);
  if ((async_cmd_buf != NULL) && (async_cmd_buf->req_handle != NULL))
  {
    while(cl_sp != NULL)
    {
      if(cl_sp == async_cmd_buf->cl_sp)
      {
        goto bail;
      }
      cl_sp = q_next(&qmi_dms_state.client_q,
                           &(cl_sp->link));
    }
  }
  local_err = QMI_DMSI_LOCAL_ERR_INVALID_ASYNC_BUF;
 
bail:

  LOG_MSG_INFO1_1("DMS check Valid Async Command Buffer %d", local_err);
  if (QMI_DMSI_LOCAL_ERR_NONE != local_err)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

#ifdef FEATURE_MMGSDI_SESSION_LIB
/*===========================================================================
  FUNCTION qmi_dms_is_inited()

  DESCRIPTION  Provides if DMS Service is Initialized or not

  PARAMETERS
    none

  RETURN VALUE
   boolean - returns TRUE if DMS service is intialized else FALSE

  DEPENDENCIES
    none

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_dms_is_inited
(
  void
)
{
  /*return status of DMS service initialization*/
  return qmi_dmsi_inited;
}

/*===========================================================================
  FUNCTION QMI_DMS_MMGSDI_EVT_CBACK()

  DESCRIPTION
    Callback function called by mmgsdi to report events

  PARAMETERS
    event  : Info about the event generated

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_mmgsdi_evt_cback
(
  const mmgsdi_event_data_type *  event
)
{
  qmi_dmsi_cmd_buf_type *  cmd_ptr;
  mmgsdi_slot_id_enum_type slot = MMGSDI_SLOT_AUTOMATIC;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*Check if task is inited post command else ignore */
  if (!qmi_dmsi_inited)
  {
    return;
  }
  
  /*Received event_cback from MMGSDI , posting cmd to DS */
  
  switch(event->evt)
  {
    case MMGSDI_CARD_INSERTED_EVT:
      slot = event->data.card_inserted.slot;
      break;

    case MMGSDI_CARD_ERROR_EVT:
      slot = event->data.card_error.slot;
      break;

    case MMGSDI_CARD_REMOVED_EVT:
      slot = event->data.card_removed.slot;
      break;

    default:
      break;
  }
  
  switch(event->evt)
  {
    case MMGSDI_CARD_INSERTED_EVT:
    case MMGSDI_CARD_ERROR_EVT:
    case MMGSDI_CARD_REMOVED_EVT:
      cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
      if( cmd_ptr == NULL)
      {
        return;
      }
      cmd_ptr->cmd_id = QMI_CMD_PROCESS_UIM_GET_STATE;
      cmd_ptr->data.mmgsdi_evt_info.event = event->evt;
      cmd_ptr->data.mmgsdi_evt_info.slot = slot;
      cmd_ptr->data.mmgsdi_evt_info.card_err_info = 
                 event->data.card_error.info;
      break;

    default:
      return;
  }

  dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
} /* qmi_dms_mmgsdi_evt_cback */


/*===========================================================================
  FUNCTION QMI_DMS_PROCESS_MMGSDI_OPERATION_COMPLETE()

  DESCRIPTION
    To send a response to the QMI pin operation requested by the client.
    
  PARAMETERS
    cnf_type : Type of MMGSDI PIN confirmation
    cnf_data : Response data

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_process_mmgsdi_operation_complete
(
  mmgsdi_cnf_enum_type       cnf_type,
  mmgsdi_cnf_type*           cnf_data
)
{
  ds_qmi_dms_async_cmd_type* async_cmd_buf = NULL;
  qmi_dmsi_local_err_type    local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  async_cmd_buf = (ds_qmi_dms_async_cmd_type*)cnf_data->response_header.client_data;

  do 
  {
    if (async_cmd_buf == NULL)
    {
      /* QMI should always pass command buffer for MMGSDI read */
      local_err = QMI_DMSI_LOCAL_ERR_NULL_CLIENT_DATA_PTR;
      break;
    }
    else
    {
      if (qmi_dms_check_valid_async_cmd_buf(async_cmd_buf))
      {
        /* Reassigning the client data to the original command buffer */
        switch (cnf_type)
        {
          case MMGSDI_READ_CNF:
            qmi_dmsi_generate_read_uim_msisdn_resp(
              &(cnf_data->read_cnf), async_cmd_buf);
            break;

          default:
            local_err = QMI_DMSI_LOCAL_ERR_INVAL_MMGSDI_CNF_TYPE;
            break;
        }
      } 
      else
      {
        PS_SYSTEM_HEAP_MEM_FREE(async_cmd_buf);
        local_err = QMI_DMSI_LOCAL_ERR_INVAL_MMGSDI_READ_CNF_TYPE;
      }
    }
  } while (0); 

  LOG_MSG_INFO1_2("DMS mmgsdi operation for cnf_type %d exit with local_err %d",
                  cnf_type, local_err);
} /* qmi_dms_process_mmgsdi_operation_complete */

/*===========================================================================
  FUNCTION QMI_DMS_PROCESS_UIM_GET_STATE()

  DESCRIPTION
    To process event received for SIM status.
    
  PARAMETERS
    event    : MMGSDI event that was generated
    card_err_info: Contains card error code
    session_activated : Contains if session has been activated

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_process_uim_get_state
(
  mmgsdi_events_enum_type event,
  mmgsdi_card_err_info_enum_type card_err_info,
  boolean session_activated
)
{
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  switch(event)
  {
    case MMGSDI_CARD_INSERTED_EVT:
      qmi_dmsi_global.uim_state = DMS_UIM_INITIALIZATION_FAILED_V01;
      break;
    /* SessionChanged replaces SelectAid ev, means init in progress */
    case MMGSDI_SESSION_CHANGED_EVT: 
      if ((DMS_UIM_NOT_PRESENT_V01 == qmi_dmsi_global.uim_state) &&
           (FALSE == session_activated))
      {
        local_err = QMI_DMSI_LOCAL_ERR_IGNORE_MMGSDI_SESSION_CHNG_EVT;
        /* Dont generate indication since there is no change in state */
        
      }
      else
      {
        qmi_dmsi_global.uim_state = DMS_UIM_INITIALIZATION_FAILED_V01;
      }
      break;

    /* Card removed ev is seen only on targets with some special hardware */
    case MMGSDI_CARD_REMOVED_EVT: 
      qmi_dmsi_global.uim_state = DMS_UIM_NOT_PRESENT_V01;
      break;

    case MMGSDI_SUBSCRIPTION_READY_EVT:
      qmi_dmsi_global.uim_state = DMS_UIM_INITIALIZATION_COMPLETED_V01;
      break;

    case MMGSDI_CARD_ERROR_EVT:
      /* Checking if card is present in the slot or not */
      /* If the hardware supports the mechanism to detect presence of the card, 
         MMGSDI_CARD_ERR_CARD_REMOVED will be sent when card gets removed. Else 
         one of the other causes will be sent for card removal or power up 
         without card
      */
      if((MMGSDI_CARD_ERR_NO_ATR_RCVD_AT_MAX_VOLT == card_err_info) ||
         (MMGSDI_CARD_ERR_CARD_REMOVED == card_err_info) ||
         (MMGSDI_CARD_ERR_NO_ATR_RCVD_AFTER_RESET == card_err_info) ||
         (MMGSDI_CARD_ERR_CORRUPT_ATR_RCVD_MAX_TIMES == card_err_info))
      {
        qmi_dmsi_global.uim_state = DMS_UIM_NOT_PRESENT_V01 ;
      }
      else
      {
        qmi_dmsi_global.uim_state = DMS_UIM_INITIALIZATION_FAILED_V01;
      }
      break;

    default:
      return;
  }

  LOG_MSG_INFO2_3("DMS received mmgsdi event %d for sim status with card_err_info %d "
                  "exit with local_err %d ", 
                  event,card_err_info, local_err);
} /* qmi_dms_process_uim_get_state */


#endif /* FEATURE_MMGSDI_SESSION_LIB */

/*===========================================================================
  FUNCTION QMI_DMS_INIT()

  DESCRIPTION
    Register the DMS service with QMI Framework 

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_init
(
  void
)
{
  qmi_csi_error         rc = QMI_CSI_NO_ERR;
  qmi_sap_error         src = QMI_SAP_NO_ERR;
  qmi_csi_options       options;
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_1("QMI DMS service init %d", qmi_dmsi_inited);
  do
  {
    if (!qmi_dmsi_inited)
    {
      /*-------------------------------------------------------------------------
        Setting DCC signal handler for handling QMI messages
      -------------------------------------------------------------------------*/
      (void)dcc_set_sig_handler(DCC_DMS_RDR_SIGNAL,
                                qmi_dmsi_msg_hdlr_cb, &qmi_dms_state);
      dcc_enable_sig(DCC_DMS_RDR_SIGNAL);

      /* Set the signal to init again */
      DCC_SET_SIGNAL(DCC_DMS_RDR_SIGNAL);
      break;
    }

    os_params.tcb = &dcc_tcb;
    os_params.sig = DMSI_QCSI_OS_SIGNAL;
    QMI_CSI_OPTIONS_INIT(options);
    (void)mqcsi_register_log_options(&options);
  
    /*-------------------------------------------------------------------------
      Register QMI dms service with QCSI
    -------------------------------------------------------------------------*/
    rc = qmi_csi_register_with_options(dms_get_service_object_v01(),
                                       qmi_dmsi_svc_connect_cb,
                                       qmi_dmsi_svc_disconnect_cb,
                                       qmi_dmsi_handle_client_req_cb,
                                       &qmi_dms_state, &os_params,
                                       &options,
                                       &qmi_dms_state.service_handle);

    //register with SAP
    src = qmi_sap_register(dms_get_service_object_v01(),
                           NULL,
                           &qmi_dms_state.sap_client_handle);

    /*Check whether registration is successful with QCSI and QSAP or not*/
    if ((src != QMI_SAP_NO_ERR) || (rc != QMI_CSI_NO_ERR))
    {
      local_err = QMI_DMSI_LOCAL_ERR_SAP_CSI_REGISTER_FAIL;
      DS_QMI_DMS_ASSERT(0);
      break;
    }

    /* Initialize the queue which will hold all handles of clients */
    (void)q_init(&qmi_dms_state.client_q);
    qmi_dms_state.num_of_clients = 0;
    qmi_dmsi_process_svc_init();
    qmi_dmsi_reg_sys_events();
	
    #ifdef FEATURE_MMGSDI_SESSION_LIB
      qmi_if_mmgsdi_reg();
    #endif /* FEATURE_MMGSDI_SESSION_LIB */

///*-------------------------------------------------------------------------
//  Setting QMI dms service command handler with DCC task process
//-------------------------------------------------------------------------*/
    dcc_set_cmd_handler(DCC_DMS_SVC_CMD, qmi_dmsi_process_cmd);
  } while (0); 

  LOG_MSG_INFO1_3("DMS Init exit with rc %d, src %d, local_err %d",
                  rc, src, local_err);
} /* qmi_dms_init */

static boolean qmi_dmsi_msg_hdlr_cb
(
  dcc_sig_enum_type  sig,
  void              *svc_ptr
)
{
  qmi_dmsi_state_info_type *dms_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT((sig == DCC_DMS_RDR_SIGNAL) && svc_ptr);

  if (!qmi_dmsi_inited)
  {
    /* Call init again to register with QCSI now
     * Wait for all tasks to be inited */
    qmi_dmsi_inited = TRUE;
    qmi_dms_init();
    return TRUE;
  }

  dms_sp = (qmi_dmsi_state_info_type*) svc_ptr;

  qmi_csi_handle_event(dms_sp->service_handle, &os_params);
  return TRUE;
} /* qmi_dmsi_msg_hdlr_cb */

static qmi_csi_cb_error qmi_dmsi_svc_connect_cb
(
  qmi_client_handle          client_handle,
  void                      *service_handle,
  void                     **connection_handle
)
{
  qmi_dmsi_client_info_type    *clnt_info_ptr =  NULL;
  qmi_dmsi_state_info_type     *dms_svc_ptr;
  qmi_csi_cb_error              rc;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_QMI_DMS_ASSERT(connection_handle && service_handle);

  dms_svc_ptr = (qmi_dmsi_state_info_type *) service_handle;
 
  if ( NULL != q_linear_search( &dms_svc_ptr->client_q,
                                dmsi_compare_qmi_client_handle,
                                client_handle ) )
  {
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  if (DMSI_MAX_CLIDS == dms_svc_ptr->num_of_clients)
  {
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC(clnt_info_ptr,
  	                       sizeof(qmi_dmsi_client_info_type),
  	                       qmi_dmsi_client_info_type*);
  if(!clnt_info_ptr)
  {
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  qmi_dmsi_initialize_client(clnt_info_ptr);
  (void) q_link ( clnt_info_ptr, &clnt_info_ptr->link );
  clnt_info_ptr->clnt = client_handle;
  /* Add it to the queue of clients */
  q_put( &(dms_svc_ptr->client_q), &(clnt_info_ptr->link) );
  dms_svc_ptr->num_of_clients++;

  *connection_handle = clnt_info_ptr;
  rc = QMI_CSI_CB_NO_ERR;
bail:
  LOG_MSG_INFO1_4("dmsi_svc_connect_cb:rc (%d) clnt_info_ptr (0x%p),client_handle (0x%p),num_of_clients (%d)",
                  rc,clnt_info_ptr,client_handle,dms_svc_ptr->num_of_clients);
  return rc;
} /* qmi_dmsi_svc_connect_cb() */

static void qmi_dmsi_svc_disconnect_cb
(
  void                      *connection_handle,
  void                      *service_handle
)
{
  qmi_dmsi_state_info_type    *dms_svc_ptr;
  qmi_dmsi_client_info_type   *clnt_info_ptr;
  qmi_dmsi_local_err_type     local_err = QMI_DMSI_LOCAL_ERR_NONE; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(connection_handle && service_handle);
  
  dms_svc_ptr = (qmi_dmsi_state_info_type *)service_handle;
  do
  {
    clnt_info_ptr = q_linear_search(&dms_svc_ptr->client_q,
                                    dmsi_compare_connection_info_handle,
                                    connection_handle);
    if (NULL == clnt_info_ptr)
    {
      local_err = QMI_DMSI_LOCAL_ERR_CLIENT_NOT_FOUND;
      break;
    }
    qmi_dmsi_reset_client(clnt_info_ptr);
    if (TRUE != q_delete_ext(&dms_svc_ptr->client_q,
                             &clnt_info_ptr->link))
    {
      local_err = QMI_DMSI_LOCAL_ERR_Q_DELETE_FAIL;
      break;
    }
    PS_SYSTEM_HEAP_MEM_FREE(clnt_info_ptr);
    dms_svc_ptr->num_of_clients--;
  } while (0); 

  LOG_MSG_INFO1_2("dmsi_svc_disconnect_cb: Releasing client 0x%p with local_err %d", 
                  connection_handle, local_err);
} /* qmi_dmsi_svc_disconnect_cb() */


static qmi_csi_cb_error qmi_dmsi_handle_client_req_cb
(
  void*                    connection_handle,
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void*                    req_c_struct,
  unsigned int             req_c_struct_len,
  void*                    service_handle
)

{
  qmi_csi_cb_error               rc;
  qmi_dmsi_client_info_type     *clnt_info_ptr;
  qmi_dmsi_state_info_type      *dms_svc_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(connection_handle && service_handle);
  
  rc = QMI_CSI_CB_NO_ERR;
  dms_svc_ptr = (qmi_dmsi_state_info_type*) service_handle;

  clnt_info_ptr = q_linear_search( &dms_svc_ptr->client_q,
                                   dmsi_compare_connection_info_handle,
                                   connection_handle );
  while(1)
  {
    if(NULL == clnt_info_ptr)
    {
      rc = QMI_CSI_CB_INTERNAL_ERR;
      break;
    }
    if(msg_id < (sizeof(req_handle_table) / sizeof(*req_handle_table)))
    {
      if(req_handle_table[msg_id])
      {
        rc = req_handle_table[msg_id] (clnt_info_ptr, req_handle, msg_id,
                                       req_c_struct, req_c_struct_len,
                                       service_handle);
        break;
  	  
      }
      else
      {
        rc = QMI_CSI_CB_UNSUPPORTED_ERR;
        break;
      }
    }
    else
    {
      rc = QMI_CSI_CB_UNSUPPORTED_ERR;
      break;
    }
  }
  LOG_MSG_INFO1_2("dmsi_handle_req_cb: message ID: (%d) rc (%d)", msg_id,rc);
  return rc;
} /* qmi_dmsi_handle_client_req_cb() */

/*===========================================================================
  FUNCTION QMI_DMS_CHARGER_EVENT_OCCURED()

  DESCRIPTION
    Call back function called when the battery level ind timer expires.
    Causes report_status_ind to be sent
    
  PARAMETERS
    arg_ptr : user data

  RETURN VALUE
    None
        
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_charger_event_occured
(
  uint8 event_mask,
  uint8 power_state_mask,
  uint8 battery_level
)
{
  qmi_dmsi_client_info_type *  cl_sp = NULL;
  boolean send_event_report_ind = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);

  qmi_dmsi_global.pwr_rpt.low_limit = DMSI_BAT_LVL_MIN;
  qmi_dmsi_global.pwr_rpt.high_limit = DMSI_BAT_LVL_MAX;

    while(cl_sp != NULL)
    {
      if( cl_sp->report_status.report_power_state )
      {
        /*-------------------------------------------------------------------
          If event mask is 0x01 or 0x03 => power state has changed so 
          generate indicatation without checking anything else 

          If event mask is 0x02 check battery level and decide whether to 
          generate ind or not as below 
        
          ->on lower limit side:
          If the current bat lvl is <= the client's lower limit and the 
          client's lower limit is < previous battery level when this
          function was called then generate the ind

            ---|-----|-------|-----------
               L1    L2      L3
               20    25      35
                   (curr)  (prev level)

         ->on upper limit side: similar to above 
        -------------------------------------------------------------------*/

        /*------------------------------------------------------------------- 
          If the event mask indicates power state change, send event report 
          ind if the current power state is not equal to the last reported
        -------------------------------------------------------------------*/   
        if((event_mask & DMSI_EVENT_MASK_POWER_STATE) && 
           (power_state_mask !=
            cl_sp->report_status.last_power_state_reported)) 
        {
          send_event_report_ind = TRUE;         
        }
          
        //or else event mask is battery level, hence check levels   
        //If first time going under lower limit             
        if((battery_level <= cl_sp->report_status.bat_lvl_lower_limit) && 
          ((cl_sp->report_status.bat_lvl_lower_limit < cl_sp->report_status.last_bat_lvl_reported) ||
          (cl_sp->report_status.last_bat_lvl_reported == DMSI_BAT_LVL_INACTIVE)))
        {
          send_event_report_ind = TRUE;
        }
       
        if((battery_level >= cl_sp->report_status.bat_lvl_upper_limit) && 
          ((cl_sp->report_status.bat_lvl_upper_limit > cl_sp->report_status.last_bat_lvl_reported) ||
          (cl_sp->report_status.last_bat_lvl_reported == DMSI_BAT_LVL_INACTIVE))) 
        {
          send_event_report_ind = TRUE;
        }
       
        if(send_event_report_ind == TRUE)
        {
          LOG_MSG_INFO2_3("Sending DMS IND to client: %0x, power state: %d, bat lvl: %d",
                          cl_sp, power_state_mask, battery_level);
          qmi_dmsi_event_report_ind(cl_sp,
                                    DMSI_REPORT_STATUS_EV_POWER_STATE,
                                    power_state_mask,
                                    battery_level,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0);

          cl_sp->report_status.last_bat_lvl_reported     = battery_level;
          cl_sp->report_status.last_power_state_reported = power_state_mask;
          send_event_report_ind = FALSE;
        }
      }
      cl_sp = q_next(&qmi_dms_state.client_q,
                           &(cl_sp->link));
    }

  /*-------------------------------------------------------------------------
    Determine the new high and low battery limits and set them in the 
    charger module
  -------------------------------------------------------------------------*/
  qmi_dmsi_set_battery_level_limits(battery_level);            

} /* qmi_dms_charger_event_occured */

/*===========================================================================
  FUNCTION QMI_DMS_GENERATE_OPRT_MODE_IND()

  DESCRIPTION
    Called when CM notifies us that the operating mode has changed.  Sends
    an indication with the new operating mode to clients registered for
    the event.
        
  PARAMETERS
    new_oprt_mode : new operating mode (if this is LPM, QMI DMS will
                    determine if it is persistent LPM on its own)
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_generate_oprt_mode_ind
(
  sys_oprt_mode_e_type  new_cm_oprt_mode
)
{
  qmi_dmsi_client_info_type *    cl_sp = NULL;
  dms_operating_mode_enum_v01      new_dms_oprt_mode;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  new_dms_oprt_mode = DMS_OP_MODE_ONLINE_V01;
  if(!qmi_dmsi_inited)
  {
    return;
  }
  if (QMI_ERR_NONE_V01 != qmi_dmsi_oprt_mode_from_cm(new_cm_oprt_mode,
                                                 &new_dms_oprt_mode))
  {
    return;
  }
  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);
  while(cl_sp != NULL)
  {
    if( cl_sp->report_status.report_operating_mode)
    {
      qmi_dmsi_event_report_ind(cl_sp,
                                DMSI_REPORT_STATUS_EV_OPRT_MODE,
                                0,
                                0,
                                0,
                                0,
                                (uint8) new_dms_oprt_mode,
                                0,
                                0);
    }
    cl_sp = q_next(&qmi_dms_state.client_q,
                         &(cl_sp->link));
  }

} /* qmi_dms_generate_oprt_mode_ind() */

/*===========================================================================
  FUNCTION QMI_DMS_GENERATE_SET_OPRT_MODE_RESP()

  DESCRIPTION
    Called when cmd_cb() supplied in cm_ph_cmd_set_oprt_mode() is called 
    by CM. Sends response to control point for the corresponding
    Set Operating Mode Reqs
        
  PARAMETERS
    user_data   : user data
    ph_cmd_err  : error type
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_generate_set_oprt_mode_resp
(
  void *               user_data,
  cm_ph_cmd_err_e_type ph_cmd_err
)
{
  qmi_error_type_v01                  errval = QMI_ERR_NONE_V01 ;
  qmi_result_type_v01                 result;
  ds_qmi_dms_async_cmd_type*          qmi_resp_buf_p = NULL;
  dms_set_operating_mode_resp_msg_v01  resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  qmi_resp_buf_p = (ds_qmi_dms_async_cmd_type*)(user_data);
  
  if(qmi_resp_buf_p == NULL )
  {
    errval = QMI_ERR_INVALID_HANDLE_V01;
    goto bail;
  }
  
  if ( qmi_dms_check_valid_async_cmd_buf(qmi_resp_buf_p) == FALSE )
  {
    errval = QMI_ERR_INVALID_HANDLE_V01;
    PS_SYSTEM_HEAP_MEM_FREE(qmi_resp_buf_p);
    goto bail;
  }
  memset(&resp_msg,0,sizeof(dms_set_operating_mode_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Send SUCCESS or FAILURE response to control point based on ph_cmd_err
  -------------------------------------------------------------------------*/
  switch (ph_cmd_err)
  {
    case CM_PH_CMD_ERR_NOERR:
      errval = QMI_ERR_NONE_V01;
      break;

    case CM_PH_CMD_ERR_NO_BUF_L:
      errval = QMI_ERR_NO_MEMORY_V01;
      break;

    case CM_PH_CMD_ERR_IN_USE_S:
      errval = QMI_ERR_DEVICE_IN_USE_V01;
      break;

    case CM_PH_CMD_ERR_OPRT_MODE_S:
      errval = QMI_ERR_INVALID_TRANSITION_V01; //error_code
      break;

    case CM_PH_CMD_ERR_RF_SW:
      errval = QMI_ERR_DEVICE_NOT_READY_V01; //Temporary Error
      resp_msg.fail_reason_valid = TRUE;
      resp_msg.fail_reason = DMS_DEVICE_STATE_FAIL_RF_SW_V01;
      break;
	  
    case CM_PH_CMD_ERR_RF_HW:
      errval = QMI_ERR_DEVICE_NOT_READY_V01; //Temporary Error
      resp_msg.fail_reason_valid = TRUE;
      resp_msg.fail_reason = DMS_DEVICE_STATE_FAIL_RF_HW_V01;
      break;

    case CM_PH_CMD_ERR_GPS:
      errval = QMI_ERR_DEVICE_NOT_READY_V01; //Temporary Error
      resp_msg.fail_reason_valid = TRUE;
      resp_msg.fail_reason = DMS_DEVICE_STATE_FAIL_GPS_V01;
      break;

    default:
      errval = QMI_ERR_INTERNAL_V01;
      break;
  }

  result = (errval == QMI_ERR_NONE_V01? QMI_RESULT_SUCCESS_V01
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(qmi_resp_buf_p->req_handle,
                         qmi_resp_buf_p->msg_id,
                         &resp_msg,
                         sizeof(dms_set_operating_mode_resp_msg_v01));
  PS_SYSTEM_HEAP_MEM_FREE(qmi_resp_buf_p);
bail:
  if (QMI_ERR_INVALID_HANDLE_V01 == errval)
  {
    LOG_MSG_INFO1_0("Invalid resp_buf_ptr");
  }
} /* qmi_dms_generate_set_oprt_mode_resp() */

/*===========================================================================
  FUNCTION QMI_DMS_GENERATE_CDMA_LOCK_MODE_IND()

  DESCRIPTION
    Called when the cdma lock mode status changes.  Sends
    an indication with the new cdma lock mode state to clients registered for
    the event.
        
  PARAMETERS
    new_cdma_lock_mode : new state of the cdma lock mode
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_generate_cdma_lock_mode_ind
(
  boolean  new_cdma_lock_mode
)
{
  qmi_dmsi_client_info_type *    cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!qmi_dmsi_inited)
  {
    return;
  }
  LOG_MSG_INFO2_1("qmi_dms_generate_cdma_lock_mode_ind()function entry"
                  "new_cdma_lock_mode %d", new_cdma_lock_mode);
  /* Update the global variable with the new value */
  qmi_dmsi_global.cdma_lock_mode = new_cdma_lock_mode;
  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);

  /* Send the indication for all the registered clients */
  while(cl_sp != NULL)
  {
    if( cl_sp->report_status.report_cdma_lock_mode)
    {
      qmi_dmsi_event_report_ind(cl_sp,
                                DMSI_REPORT_STATUS_EV_CDMA_LOCK_MODE,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                new_cdma_lock_mode);
    }
    cl_sp = q_next(&qmi_dms_state.client_q,
                           &(cl_sp->link));
  }
} /* qmi_dms_generate_cdma_lock_mode_ind */

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_DMSI_RESET()

  DESCRIPTION
    Reset the issuing DMS client's state

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle


  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_reset
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_reset_resp_msg_v01       resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_reset_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Reset the client's state
  -------------------------------------------------------------------------*/
  qmi_dmsi_reset_client(clnt_info);
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_reset_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_reset() */

/*===========================================================================
FUNCTION  QMI_DMSI_CM_PH_CB

DESCRIPTION
  CM phone command callback function

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  Adds command in DS command buffer

===========================================================================*/
/* ARGSUSED */
static void qmi_dmsi_cm_ph_cb 
(
  void                         *data_ptr,         /* Data block pointer    */
  cm_ph_cmd_e_type              ph_cmd,              /* Command ID            */
  cm_ph_cmd_err_e_type          ph_cmd_err           /* Command error code    */
)
{
  qmi_dmsi_cmd_buf_type *  cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Post a cmd to DCC to handle this in DCC context
  -------------------------------------------------------------------------*/
  cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
  if( cmd_ptr == NULL)
  {
    return;
  }

  switch(ph_cmd)
  {
    case CM_PH_CMD_OPRT_MODE:
	    cmd_ptr->cmd_id = QMI_CMD_SET_OPERATING_MODE_RESP;
      break;

    case CM_PH_CMD_UI_READY_STATUS:
        cmd_ptr->cmd_id = QMI_CMD_SET_UI_STATUS_RESP;
      break;

    default:
      LOG_MSG_INFO2_0("Unexpected ph_cmd received in qmi_dmsi_cm_ph_cb");
      PS_SYSTEM_HEAP_MEM_FREE(cmd_ptr);
      return;
  }
  // Fill in the QMI CMD with relevant DMS related params 
  cmd_ptr->data.cm_ph_err.user_data = data_ptr;
  cmd_ptr->data.cm_ph_err.err_type = ph_cmd_err;
  dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
  return;
} /* qmi_dmsi_cm_ph_cb */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_CAP()

  DESCRIPTION
    Gives the device capabilities

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle


  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_device_cap
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_device_cap_resp_msg_v01 resp_msg;
  qmi_error_type_v01 errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  uint32 max_tx_channel_rate;
  uint32 max_rx_channel_rate;
  uint64 sglte_info;
  policyman_status_t         status = -1;
  policyman_item_t const     *cm_pm_device_config = NULL;
  policyman_item_id_t        ids[] = { POLICYMAN_ITEM_DEVICE_CONFIGURATION };
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_device_cap_resp_msg_v01));

  status = policyman_get_items(
              &ids[0],
              sizeof(ids)/sizeof(policyman_item_id_t),
              &cm_pm_device_config);
  if (POLICYMAN_FAILED(status) || cm_pm_device_config == NULL)
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_0("policyman_get_items failed");
    goto send_result;
  }

  errval = qmi_dmsi_get_device_config_list((void*)&(resp_msg.device_config_list),
                                           (int16_t*)&(resp_msg.explicit_config_index),
                                           cm_pm_device_config);
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  else
  {
    resp_msg.device_config_list_valid = TRUE;
    resp_msg.explicit_config_index_valid = TRUE;
  }

  errval = qmi_dmsi_get_max_subs_static_device_cap((void*)&resp_msg);
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  else
  {
    resp_msg.device_max_subscription_static_capability_valid = TRUE;
	
     errval = qmi_dmsi_get_rat_release_version_info((void*)&resp_msg);
     if(errval != QMI_ERR_NONE_V01)
     {
       goto send_result;
     }
     else
     {
       resp_msg.rat_rel_info_valid = TRUE;
     }
  }
  

  /*-------------------------------------------------------------------------
    Get the device multisim info
  -------------------------------------------------------------------------*/
  errval = qmi_dmsi_get_multisim_info(&(resp_msg.multisim_capability),
  										cm_pm_device_config);
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  else
  {
    resp_msg.multisim_capability_valid = TRUE;
  }

   /*do call and fill the 3 tlvs requied for this*/
   /*Sending FALSE as second arg for get_device message*/
  errval = qmi_dmsi_get_device_multisim_current_cap_info((void*)&resp_msg,FALSE,
  														  cm_pm_device_config);
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  else
  {
    resp_msg.current_multisim_capability_valid = TRUE;
    resp_msg.current_subscription_capability_valid = TRUE;
    resp_msg.subs_voice_data_capability_valid = TRUE;
    resp_msg.ims_capability_valid = TRUE;
    resp_msg.max_ims_instances_valid = TRUE;
  }

  /*-------------------------------------------------------------------------
    Get the device feature mode 
  -------------------------------------------------------------------------*/
  errval = qmi_dmsi_get_subs_device_feature_mode(&resp_msg);
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  else
  {
    resp_msg.subs_device_feature_mode_valid = TRUE;
  }

  /*-------------------------------------------------------------------------
    Get the Max active subs
  -------------------------------------------------------------------------*/
  errval = qmi_dmsi_get_max_active_data_subs(&(resp_msg.max_active_data_subscriptions),
  												cm_pm_device_config);
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  else
  {
    resp_msg.max_active_data_subscriptions_valid = TRUE;
  }

  /*-------------------------------------------------------------------------
    Get the list of supported radio ifs
  -------------------------------------------------------------------------*/
  resp_msg.device_capabilities.radio_if_list_len = 
    qmi_dmsi_get_hw_supported_radio_if_list( (uint32*)&(resp_msg.device_capabilities.radio_if_list));
  
  /*-------------------------------------------------------------------------                                           
    Get the max TX and RX channel rates
  -------------------------------------------------------------------------*/
  errval = qmi_dmsi_get_max_channel_rate(&max_rx_channel_rate,
                                         &max_tx_channel_rate);
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }

  resp_msg.device_capabilities.max_tx_channel_rate = max_tx_channel_rate;
  resp_msg.device_capabilities.max_rx_channel_rate = max_rx_channel_rate;

  /*-------------------------------------------------------------------------
    Get the voice capability of the device
  -------------------------------------------------------------------------*/
  resp_msg.device_capabilities.data_service_capability = 
                                 (uint32)qmi_dmsi_get_voice_capability();

  /*-------------------------------------------------------------------------
    Get the SIM capability of the device
  -------------------------------------------------------------------------*/
  resp_msg.device_capabilities.sim_capability = qmi_dmsi_get_sim_capability();

  /*-------------------------------------------------------------------------
    Get the Simultaneous Voice and Data capability of the device
  -------------------------------------------------------------------------*/
  resp_msg.simul_voice_and_data_capability =
            qmi_dmsi_get_simul_voice_and_data_capability();
  resp_msg.simul_voice_and_data_capability_valid = TRUE;

  /*-------------------------------------------------------------------------
    Get the Voice Support capability of the device
  -------------------------------------------------------------------------*/
  resp_msg.voice_support_capability =
            qmi_dmsi_get_voice_support_capability();
  if(resp_msg.voice_support_capability != 0)
  {
    resp_msg.voice_support_capability_valid = TRUE;
  }
  
  sglte_info = resp_msg.simul_voice_and_data_capability;
  /*-------------------------------------------------------------------------
    Get the Device Service capability of the device
  -------------------------------------------------------------------------*/
  resp_msg.device_service_capability_valid = TRUE;
  resp_msg.device_service_capability  = (dms_device_service_capability_enum_v01)
    ((sglte_info & QMI_DMS_MASK_SGLTE_CAPABLE_V01)?DMS_DEVICE_CAP_SIMUL_VOICE_AND_DATA_V01:
     (uint32)qmi_dmsi_get_voice_capability());
	 
  resp_msg.device_capabilities.data_service_capability = 
    (sglte_info & QMI_DMS_MASK_SGLTE_CAPABLE_V01)?DMS_DATA_CAP_SIMUL_CS_AND_PS_V01:
       resp_msg.device_capabilities.data_service_capability;
    

send_result:
  if(NULL != cm_pm_device_config)
  {
    policyman_item_release(cm_pm_device_config);
  }
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_device_cap_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_get_device_cap() */



/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_MFR()

  DESCRIPTION
    Gives the name of the device manufacturer.
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_device_mfr
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  char *             mfr_info;
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  nv_stat_enum_type    nv_status;
  nv_item_type         *dcc_nv_item_ptr;
  dms_get_device_mfr_resp_msg_v01 resp_msg ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&resp_msg,0,sizeof(dms_get_device_mfr_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  
  /*-------------------------------------------------------------------------
    Query NV for Model Identification
  -------------------------------------------------------------------------*/
  nv_status = dcc_get_nv_item(NV_UNDP_HSU_MFGSTR_I, dcc_nv_item_ptr);
  if ((nv_status == NV_DONE_S) && 
                        strlen((char *)dcc_nv_item_ptr->undp_hsu_mfgstr))
  {
    mfr_info =(char *) dcc_nv_item_ptr->undp_hsu_mfgstr;
  } 
  else
  {
    mfr_info = qmi_device_mfr_str;
  }
  strlcpy((char*)(resp_msg.device_manufacturer),mfr_info,sizeof(resp_msg.device_manufacturer));
  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
send_result:
  result = (errval == QMI_ERR_NONE_V01? QMI_RESULT_SUCCESS_V01
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_device_mfr_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
  
} /* qmi_dmsi_get_device_mfr() */



/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_MODEL_ID()

  DESCRIPTION
    Gives the model ID of the device in use
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_device_model_id
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_device_model_id_resp_msg_v01 resp_msg;
  char               model_id_info[QMI_DMS_DEVICE_MODEL_ID_MAX_V01];
  nv_stat_enum_type  nv_status;
  qmi_error_type_v01   errval =  QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  nv_item_type      *dcc_nv_item_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(model_id_info, 0, sizeof(char)*QMI_DMS_DEVICE_MODEL_ID_MAX_V01);
  memset(&resp_msg,0,sizeof(dms_get_device_model_id_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Query NV for Model Identification
  -------------------------------------------------------------------------*/
  nv_status = dcc_get_nv_item(NV_UNDP_HSU_PRODSTR_I, dcc_nv_item_ptr);
  if( nv_status == NV_DONE_S )
  {
    (void) strlcpy((char*)model_id_info,
                    (const char*)dcc_nv_item_ptr->undp_hsu_prodstr,
                    sizeof(resp_msg.device_model_id));
  }
  if ((nv_status != NV_DONE_S) || (0 == strlen(model_id_info)))
  {
    nv_status = dcc_get_nv_item(NV_MOB_MODEL_I, dcc_nv_item_ptr);
    if(nv_status == NV_DONE_S)
    {
      (void) dsatutil_itoa(dcc_nv_item_ptr->mob_model, 
                           (unsigned char*)model_id_info,
                           DMSI_BASE_DECIMAL);
    }
    else
    {
      /*-----------------------------------------------------------------------
        Model not found in NV, hence using the global Model variable
      -----------------------------------------------------------------------*/  
      (void) dsatutil_itoa(mob_model,
                           (unsigned char*)model_id_info, 
                           DMSI_BASE_DECIMAL);
    }
  }

  /*-----------------------------------------------------------------------
    Make sure that the string is NULL terminated so that strlen will work
  -----------------------------------------------------------------------*/  
  strlcpy((char*)resp_msg.device_model_id,(char*)model_id_info,sizeof(resp_msg.device_model_id));
  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);

send_result:
  result = (errval == QMI_ERR_NONE_V01? QMI_RESULT_SUCCESS_V01
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_device_model_id_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
 
} /* qmi_dmsi_get_device_model_id() */



/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_REV_ID()

  DESCRIPTION
    Gives the revision ID of the device in use
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_device_rev_id
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
/* boot_block_version isn't defined for NAND-based (6k and later) targets.
 * We need to send the default NOR boot block version in our response, for
 * backward compatibility. */
#define QMI_BOOT_BLOCK_VERSION 0x1

  char                 dev_rev_id_info[QMI_DMS_DEVICE_REV_ID_MAX_V01];
  qmi_error_type_v01   errval =  QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_get_device_rev_id_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_device_rev_id_resp_msg_v01));
  memset(dev_rev_id_info, 0, sizeof(dev_rev_id_info));
  /*-------------------------------------------------------------------------
    Populate the revision ID info
  -------------------------------------------------------------------------*/
  (void) snprintf (dev_rev_id_info, QMI_DMS_DEVICE_REV_ID_MAX_V01,
           "%s  %d  [%s %s]",
           mob_sw_rev,
           QMI_BOOT_BLOCK_VERSION,
           rel_date,
           rel_time);

  strlcpy((char*)&(resp_msg.device_rev_id),dev_rev_id_info,sizeof(resp_msg.device_rev_id));
  result = (errval == QMI_ERR_NONE_V01? QMI_RESULT_SUCCESS_V01
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_device_rev_id_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_get_device_rev_id() */


/*===========================================================================
  FUNCTION QMI_DMSI_GET_MSISDN()

  DESCRIPTION
    Gives the msisdn information(MDN) of the device
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_msisdn
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01        errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01       result;
  dms_get_msisdn_resp_msg_v01 resp_msg;
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  char                    mob_num[NV_DIR_NUMB_PCS_SIZ + 1];
  char                    min_num[NV_DIR_NUMB_PCS_SIZ+1];
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

#ifdef FEATURE_MMGSDI_SESSION_LIB
  mmgsdi_return_enum_type mmgsdi_status;
  mmgsdi_access_type      sim_filename;
  qmi_if_info_type       *qmi_if_info_ptr;
  qmi_dmsi_client_info_type * dms_cl_sp;
  ds_qmi_dms_async_cmd_type *async_cmd_buf = NULL;
  mmgsdi_session_type_enum_type session_type;
  mmgsdi_session_type_enum_type session_type_cdma;
  qmi_if_mmgsd_session_e_type session_index;
#endif /* FEATURE_MMGSDI_SESSION_LIB */
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_msisdn_resp_msg_v01));
#ifdef FEATURE_MMGSDI_SESSION_LIB
  dms_cl_sp = (qmi_dmsi_client_info_type*)clnt_info;
  qmi_if_info_ptr = qmi_if_info_get_ptr();
  /* check that returning IMSI is allowed and that the UIM is ready.  if not,
     ignore the IMSI since it is optional. */
  
  session_type = MMGSDI_GW_PROV_PRI_SESSION;
  session_index = QMI_IF_MMGSD_GW_PROV_PRI_SESSION;
  session_type_cdma = MMGSDI_1X_PROV_PRI_SESSION;
  
#ifdef FEATURE_DUAL_SIM  
  if(dms_cl_sp->subscription_id == DMSI_BIND_SUBS_SECONDARY) 
  {
    session_type = MMGSDI_GW_PROV_SEC_SESSION;
    session_index = QMI_IF_MMGSD_GW_PROV_SEC_SESSION;
    session_type_cdma = MMGSDI_1X_PROV_SEC_SESSION;
  }
#endif
#ifdef FEATURE_TRIPLE_SIM
  if(dms_cl_sp->subscription_id == DMSI_BIND_SUBS_TERTIARY)
  {
    session_type = MMGSDI_GW_PROV_TER_SESSION;
    session_index = QMI_IF_MMGSD_GW_PROV_TER_SESSION;
    session_type_cdma = MMGSDI_1X_PROV_TER_SESSION;
  }
#endif

  errval = (qmi_error_type_v01)qmi_dms_get_uim_access_status_ext(session_type);
  if (errval != QMI_ERR_NONE_V01)
  {
    if ((errval = (qmi_error_type_v01)qmi_dms_get_uim_access_status_ext(session_type_cdma)) 
                   != QMI_ERR_NONE_V01)
      {
        local_err = QMI_DMSI_LOCAL_ERR_SIM_UNINIT;
        goto send_result;
      }

    /* Read MDN and MIN */
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
    memset(mob_num, 0, sizeof(mob_num));
    memset(min_num, 0, sizeof(min_num));
    errval = qmi_dmsi_get_mdn_min(mob_num, sizeof(mob_num),
                                  min_num, sizeof(min_num),
                                  (uint16)dms_cl_sp->subscription_id);
    /* add the MIN anytime it is present */
    if (errval == QMI_ERR_NONE_V01)
    {
      if (strlen(min_num))
      {
        resp_msg.mobile_id_number_valid = TRUE;
       	strlcpy((char*)(resp_msg.mobile_id_number),min_num,sizeof(resp_msg.mobile_id_number));
      }
      /* qmi_dmsi_get_mdn_min() above checks for NOT_PROVISIONED case and sets
          errval correctly, no reason to check it again*/
      strlcpy((char*)(resp_msg.voice_number),mob_num,sizeof(resp_msg.voice_number));
    }
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */ 
  }
  else
  {
    sim_filename.access_method = MMGSDI_EF_ENUM_ACCESS;
    sim_filename.file.file_enum = MMGSDI_NO_FILE_ENUM;
    
  /* GET IMSI, Request goes to MMGSDI, then MSISDN is read */
    if (!qmi_dmsi_global.nv_restrict_imsi_access)
    {    
      if (qmi_if_info_ptr->session_info[session_index].app_type == MMGSDI_APP_SIM)
      {
        sim_filename.file.file_enum = MMGSDI_GSM_IMSI;
      }
      else if (qmi_if_info_ptr->session_info[session_index].app_type == MMGSDI_APP_USIM)
      {
        sim_filename.file.file_enum = MMGSDI_USIM_IMSI;
      }
  
      if(sim_filename.file.file_enum != MMGSDI_NO_FILE_ENUM)
      {
        PS_SYSTEM_HEAP_MEM_ALLOC(async_cmd_buf,
                                 sizeof(ds_qmi_dms_async_cmd_type),
                                 ds_qmi_dms_async_cmd_type*);
        if( async_cmd_buf == NULL )
        {
          local_err = QMI_DMSI_LOCAL_ERR_NO_MEMORY;
          errval = QMI_ERR_NO_MEMORY_V01;
          goto send_result;
        }
        /* Fill Async command buffer with necessary info to send the response 
           after processing in SIM Files in call back*/
        memset(async_cmd_buf,0,sizeof(ds_qmi_dms_async_cmd_type));
        async_cmd_buf->req_handle = req_handle;
        async_cmd_buf->msg_id = msg_id;
        async_cmd_buf->cl_sp = clnt_info;

        mmgsdi_status = mmgsdi_session_read_transparent(
                                          qmi_if_info_ptr->session_info[session_index].session_id,
                                          sim_filename,
                                          0,
                                          DMSI_MMGSDI_IMSI_LEN,
                                          qmi_if_mmgsdi_resp_cback,
                                          (uint32) async_cmd_buf);
  
        if(mmgsdi_status == MMGSDI_SUCCESS)
        {
          /* IMSI will be reported back later from MMGSDI, other items will be 
                    processed at that time */
          return QMI_CSI_CB_NO_ERR;
        }
        else
        { 
          /* For any reason IMSI cannot be obtained from MMGSDI, it will be
          skipped and the functon will go forward to process other items */
          local_err = QMI_DMSI_LOCAL_ERR_MMGSDI_IMSI_FAIL;
          PS_SYSTEM_HEAP_MEM_FREE(async_cmd_buf);
        }
      }
    }

    /*going ahead and reading msisdn */
    if (qmi_if_info_ptr->session_info[session_index].app_type == MMGSDI_APP_SIM)
    {
      sim_filename.file.file_enum = MMGSDI_TELECOM_MSISDN;
    }
    else if (qmi_if_info_ptr->session_info[session_index].app_type == MMGSDI_APP_USIM)
    {
      sim_filename.file.file_enum = MMGSDI_USIM_MSISDN;
    }
    else
    {
      local_err = QMI_DMSI_LOCAL_ERR_OP_DEVICE_UNSUPPORTED;
      errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
      goto send_result;
    }
    PS_SYSTEM_HEAP_MEM_ALLOC(async_cmd_buf,
                             sizeof(ds_qmi_dms_async_cmd_type),
                             ds_qmi_dms_async_cmd_type*);
    if( async_cmd_buf == NULL )
    {
      local_err = QMI_DMSI_LOCAL_ERR_NO_MEMORY;
      errval = QMI_ERR_NO_MEMORY_V01;
      goto send_result;
    }
    async_cmd_buf->req_handle = req_handle;
    async_cmd_buf->msg_id = msg_id;
    async_cmd_buf->cl_sp = clnt_info;

    /* Read entire record for EF-MSISDN (x+14), len param = 0 */
    mmgsdi_status = mmgsdi_session_read_record(
                                 qmi_if_info_ptr->session_info[session_index].session_id,
                                     sim_filename,
                                     1,
                                     0,
                                     qmi_if_mmgsdi_resp_cback,
                                     (uint32) async_cmd_buf);

    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* MSISDN will be reported back later from MMGSDI, other items will be
      processed at that time */
      return QMI_CSI_CB_NO_ERR;
    }
    else
    {
      /* For any reason MSISDN cannot be obtained from MMGSDI it will be
      skipped and the functon will go forward to process other items */
      local_err = QMI_DMSI_LOCAL_ERR_MMGSDI_MSISDN_FAIL;
      PS_SYSTEM_HEAP_MEM_FREE(async_cmd_buf);
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
  }

send_result:
#endif /* FEATURE_MMGSDI_SESSION_LIB */
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);

  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_msisdn_resp_msg_v01));

  LOG_MSG_INFO1_1("DMS get msisdn exiting with local_err %d", local_err);
  return QMI_CSI_CB_NO_ERR;


} /* qmi_dmsi_get_msisdn() */


/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_SERIAL_NUMBERS()

  DESCRIPTION
    Gives the serial numbers of the device in use
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_device_serial_numbers
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01 errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01 result = QMI_RESULT_SUCCESS_V01;
  dms_get_device_serial_numbers_resp_msg_v01 resp_msg;
  memset(&resp_msg,0,sizeof(dms_get_device_serial_numbers_resp_msg_v01));
  /*FALSE to indicate this is not for encrypted message*/
  errval = qmi_dmsi_get_device_serial_numbers_ext(FALSE,&resp_msg,clnt_info);
  if(errval != QMI_ERR_NONE_V01)
  {
    result = QMI_RESULT_FAILURE_V01;
  }
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_device_serial_numbers_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_get_device_serial_numbers() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_ENCRYPTED_DEVICE_SERIAL_NUMBERS()

  DESCRIPTION
    Gives the encrypted serial numbers of the device in use
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_encrypted_device_serial_numbers
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01 errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01 result = QMI_RESULT_SUCCESS_V01;
  dms_get_encrypted_device_serial_numbers_resp_msg_v01 resp_msg;

  memset(&resp_msg,0,sizeof(dms_get_encrypted_device_serial_numbers_resp_msg_v01));
  /* TRUE to indicate this is for encrypted message*/
  errval = qmi_dmsi_get_device_serial_numbers_ext(TRUE,(void*)&resp_msg,clnt_info);
  if(errval != QMI_ERR_NONE_V01)
  {
    result = QMI_RESULT_FAILURE_V01;
  }
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_encrypted_device_serial_numbers_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_get_encrypted_device_serial_numbers() */


/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_SERIAL_NUMBERS_EXT()

  DESCRIPTION
    common utility for device serial and encrypted device serial numbers.
    
  PARAMETERS
  is_encrypted - to identify the message
  resp_msg     - response message
  clnt_info    - client information

  RETURN VALUE
    qmi_error_type_v01
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

static qmi_error_type_v01 qmi_dmsi_get_device_serial_numbers_ext
(
  boolean                   is_encrypted,
  void*                     resp_msg,
  qmi_dmsi_client_info_type *clnt_info
  
)
{
  /*Encrypted data would be ~100 bytes more*/
  #define DMSI_ENCRYPTED_SERIAL_NO_SIZ DMSI_SERIAL_NO_SIZ+100
  uint8              dev_serial_num_info[DMSI_SERIAL_NO_SIZ];
  nv_stat_enum_type  nv_status;
  int                i;
  boolean            resp_flag = FALSE;
  dms_get_device_serial_numbers_resp_msg_v01 *dev_ser_msg;
  dms_get_encrypted_device_serial_numbers_resp_msg_v01 *enc_dev_ser_msg;
  // used for 3GPP2 ESN/MEID
  byte *             end_ptr;
  int                padding;

  // used for 3GPP IMEI
  uint8              imei_ascii[(NV_UE_IMEI_SIZE-1)*2];
  uint8              imei_bcd_len = 0;
  uint8              digit = '\0';

  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  nv_item_type      *dcc_nv_item_ptr;
  uint32             encrypted_info_len = 0;
  uint8              dev_serial_num_encrypted_info[DMSI_ENCRYPTED_SERIAL_NO_SIZ];
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   nv_status = NV_DONE_S;
  if(is_encrypted == TRUE)
  {
    enc_dev_ser_msg = resp_msg;
  }
  else
  {
    dev_ser_msg = resp_msg;
  }
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    local_err = QMI_DMSI_LOCAL_ERR_NO_MEMORY;
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }  

  memset(dev_serial_num_info, 0, sizeof(dev_serial_num_info));
  memset(dev_serial_num_encrypted_info, 0, sizeof(dev_serial_num_encrypted_info));
  nv_status = dcc_get_nv_item_ext( NV_UE_IMEI_I, dcc_nv_item_ptr,
  	        (uint16)((qmi_dmsi_client_info_type *)clnt_info)->subscription_id );
  if (nv_status == NV_DONE_S)
  {
    imei_bcd_len = dcc_nv_item_ptr->ue_imei.ue_imei[0];
    if( imei_bcd_len <= (NV_UE_IMEI_SIZE-1) )
    {
      memset(imei_ascii, 0, (NV_UE_IMEI_SIZE-1)*2);
      for( i = 1; i <= imei_bcd_len; i++ )
      {
        digit = QMI_BCD_LOW_DIGIT(dcc_nv_item_ptr->ue_imei.ue_imei[i]);
        if( (digit <= 9) || (i <= 1) )
        {
          imei_ascii[(i-1)*2] = digit + '0';
        }
        else
        {
          imei_ascii[(i-1)*2] = '\0';
          break;
        }
        
        digit = QMI_BCD_HIGH_DIGIT(dcc_nv_item_ptr->ue_imei.ue_imei[i]);
        if( (digit <= 9) || (i <= 1) )
        {
          imei_ascii[((i-1)*2)+1] = digit + '0';
        }
        else
        {
          imei_ascii[((i-1)*2)+1] = '\0';
          break;
        }
      }
      
      /* Skip the first byte because it is just the ID */
      memscpy( (uint8*)dev_serial_num_info, 
               (NV_UE_IMEI_SIZE-1)*2-1,
               imei_ascii + 1, 
               (NV_UE_IMEI_SIZE-1)*2-1 );

      if(is_encrypted == TRUE)
      {
        encrypted_info_len = DMSI_ENCRYPTED_SERIAL_NO_SIZ;
        if (secapi_secure_message(SC_SSID_TZ,     /* TZ as destination end */
                                  SC_CID_DATA_QMI,/* QMI as client ID */
                                  dev_serial_num_info,
                                  (uint32)strlen((const char *)dev_serial_num_info),
                                   dev_serial_num_encrypted_info,
                                  (uint32*)&encrypted_info_len))
       {
         local_err = QMI_DMSI_LOCAL_ERR_SECAPI_FAIL;
         errval = QMI_ERR_ENCODING_V01;
         goto send_result;
       }
      }
      resp_flag = TRUE;
      if(is_encrypted == TRUE)
      {
        enc_dev_ser_msg->imei_valid = TRUE;
        enc_dev_ser_msg->imei_len =  encrypted_info_len;
        memscpy((char*)&enc_dev_ser_msg->imei,sizeof(enc_dev_ser_msg->imei),(char*)&dev_serial_num_encrypted_info,encrypted_info_len);
      }
      else
      {
        dev_ser_msg->imei_valid= TRUE;
        strlcpy((char*)&dev_ser_msg->imei,(char*)&dev_serial_num_info,sizeof(dev_ser_msg->imei));
      }
    }
    else
    {
      local_err = QMI_DMSI_LOCAL_ERR_INVAL_IMEI;
    }
  }
  else
  {
    local_err = QMI_DMSI_LOCAL_ERR_NULL_IMEI;
  }

  /*-------------------------------------------------------------------------
    Report ESN if CDMA is provisioned.
    dsatutil_itoa returns a pointer to the null-terminator in the output
    string.
  -------------------------------------------------------------------------*/
  memset(dev_serial_num_info, 0, sizeof(dev_serial_num_info));
  memset(dev_serial_num_encrypted_info, 0, sizeof(dev_serial_num_encrypted_info));
  
  /*---------------------------------------------------------------------------------
    Here NV_ESN_ME_I is being used instead of NV_ESN_I because value of NV_ESN_I 
    is getting changed based on dfferent UIM cards or when the card is not inserted
    while value of NV_ESN_ME_I remains consistent.
  ----------------------------------------------------------------------------------*/
  nv_status = dcc_get_nv_item_ext (NV_ESN_ME_I, dcc_nv_item_ptr,
               (uint16)((qmi_dmsi_client_info_type *)clnt_info)->subscription_id);
  if(NV_DONE_S == nv_status)
  {
    (void)dsatutil_itoa(dcc_nv_item_ptr->esn.esn,
                        dev_serial_num_info,
                        DMSI_BASE_HEX);
      if(is_encrypted == TRUE)
      {
        
        encrypted_info_len = DMSI_ENCRYPTED_SERIAL_NO_SIZ;
        if (secapi_secure_message(SC_SSID_TZ,                     /* TZ as destination end */
                                  SC_CID_DATA_QMI,                       /* QMI as client ID */
                                  dev_serial_num_info,
                                  (uint32)strlen((const char *)dev_serial_num_info),
                                  dev_serial_num_encrypted_info,
                                  (uint32*)&encrypted_info_len))
        {
          local_err = QMI_DMSI_LOCAL_ERR_SECAPI_FAIL;
          errval = QMI_ERR_ENCODING_V01;
          goto send_result;
        }        
      }
    if (strlen((const char *)dev_serial_num_info)||strlen((const char *)dev_serial_num_encrypted_info))
    {
      resp_flag = TRUE;
      if(is_encrypted == TRUE)
      {
        enc_dev_ser_msg->esn_valid = TRUE;
        enc_dev_ser_msg->esn_len =  encrypted_info_len;
        memscpy((char*)&enc_dev_ser_msg->esn,sizeof(enc_dev_ser_msg->esn),(char*)&dev_serial_num_encrypted_info,encrypted_info_len);
      }
      else
      {
        dev_ser_msg->esn_valid = TRUE;
        strlcpy((char*)&dev_ser_msg->esn,(char*)&dev_serial_num_info,sizeof(dev_ser_msg->esn));
      }
    }
  }
  else
  {
    local_err = QMI_DMSI_LOCAL_ERR_NULL_ESN;
  }

  /*-------------------------------------------------------------------------
    Report MEID if supported
  -------------------------------------------------------------------------*/
  memset(dev_serial_num_info, 0, sizeof(dev_serial_num_info));
  memset(dev_serial_num_encrypted_info, 0, sizeof(dev_serial_num_encrypted_info));

  /*---------------------------------------------------------------------------------
    Here NV_MEID_ME_I is being used instead of NV_MEID_I because value of NV_MEID_I 
    is getting changed based on dfferent UIM cards or when the card is not inserted
    while value of NV_MEID_ME_I remains consistent.
  ----------------------------------------------------------------------------------*/
  nv_status = dcc_get_nv_item_ext (NV_MEID_ME_I, dcc_nv_item_ptr,
            (uint16)((qmi_dmsi_client_info_type *)clnt_info)->subscription_id);
  if( NV_DONE_S == nv_status )
  {
    /* nv_item_type.meid is a qword, read appropriately and convert to 14 hex
       digits.  6 digits come from the upper long of the qw and the remaining
       8 come from the lower long of the qw */
    /* 6 from the upper long of the qw */
    end_ptr = dsatutil_itoa( qw_hi(dcc_nv_item_ptr->meid),
                             dev_serial_num_info,
                             DMSI_BASE_HEX );
    /* if the string is not 6 char's long, pad the beginning with 0s */
    padding = 6 - strlen((const char *) dev_serial_num_info);
    if (padding > 0)
    {
      for (i = 5; i >= padding; i--)
      {
        dev_serial_num_info[i] = dev_serial_num_info[i-padding];
      }
      for (i = 0; i < padding; i++)
      {
        dev_serial_num_info[i] = '0';
      }
      dev_serial_num_info[6] = '\0';
    }
    else if(padding < 0)
    {
       LOG_MSG_ERROR_1("dev_serial_num_info length is > 6, padding = %d",
                       padding);
    }

    /* get the start of the second half of the string which is where the lower
       long char's start */
    end_ptr = dev_serial_num_info + 6;

    (void) dsatutil_itoa( qw_lo(dcc_nv_item_ptr->meid),
                          end_ptr,
                          DMSI_BASE_HEX );

    /* if the string is not 8 char's long, pad the beginning with 0s */
    padding = 8 - strlen((const char *) end_ptr);
    if (padding)
    {
      for (i = 7; i >= padding; i--)
      {
        end_ptr[i] = end_ptr[i-padding];
      }
      for (i = 0; i < padding; i++)
      {
        end_ptr[i] = '0';
      }
      dev_serial_num_info[14] = '\0';
    }
    if(is_encrypted == TRUE)
    {
      encrypted_info_len = DMSI_ENCRYPTED_SERIAL_NO_SIZ;	  
      if (secapi_secure_message(SC_SSID_TZ,					  /* TZ as destination end */
                         	SC_CID_DATA_QMI, 				  /* QMI as client ID */
                           	dev_serial_num_info,
                           	(uint32)strlen((const char *)dev_serial_num_info),
                           	dev_serial_num_encrypted_info,
                           	(uint32*)&encrypted_info_len))
      {
        errval = QMI_ERR_ENCODING_V01;
        goto send_result;
      }
    }
    if (strlen((const char *)dev_serial_num_info) || strlen((const char *)dev_serial_num_encrypted_info))
    {
      resp_flag = TRUE;
      if(is_encrypted == TRUE)
      {
        enc_dev_ser_msg->meid_valid = TRUE;
        enc_dev_ser_msg->meid_len =  encrypted_info_len;
        memscpy((char*)&enc_dev_ser_msg->meid,sizeof(enc_dev_ser_msg->meid),(char*)&dev_serial_num_encrypted_info,encrypted_info_len);
      }
      else
      {
        dev_ser_msg->meid_valid = TRUE;
        strlcpy((char*)&dev_ser_msg->meid,(char*)&dev_serial_num_info,sizeof(dev_ser_msg->meid));
      }
    }
  }
  else
  {
    local_err = QMI_DMSI_LOCAL_ERR_NULL_MEID;
  }
 
  #if defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE) || defined(FEATURE_TDSCDMA)
  /*-------------------------------------------------------------------------
    Report IMEISV SVN if supported
  -------------------------------------------------------------------------*/
  memset((void *)&dcc_nv_item, 0, sizeof(dcc_nv_item));
  memset(dev_serial_num_info, 0, sizeof(dev_serial_num_info));
  memset(dev_serial_num_encrypted_info, 0, sizeof(dev_serial_num_encrypted_info));
  nv_status = dcc_get_nv_item_ext ( NV_UE_IMEISV_SVN_I , &dcc_nv_item,
  	        (uint16)((qmi_dmsi_client_info_type *)clnt_info)->subscription_id);
  if( NV_DONE_S == nv_status )
  {
    (void)dsatutil_itoa(dcc_nv_item.ue_imeisv_svn,
                        dev_serial_num_info,
                        DMSI_BASE_HEX);
     if(is_encrypted == TRUE)
     {
       encrypted_info_len = DMSI_ENCRYPTED_SERIAL_NO_SIZ;
       if (secapi_secure_message(SC_SSID_TZ,					  /* TZ as destination end */
                           	SC_CID_DATA_QMI, 					  /* QMI as client ID */
                           	dev_serial_num_info,
                           	(uint32)strlen((const char *)dev_serial_num_info),
                           	dev_serial_num_encrypted_info,
                           	(uint32*)&encrypted_info_len))
       {
         local_err = QMI_DMSI_LOCAL_ERR_SECAPI_FAIL;
         errval = QMI_ERR_ENCODING_V01;
         goto send_result;
       }
     }
     resp_flag = TRUE;
     if(is_encrypted == TRUE)
     {
       enc_dev_ser_msg->imeisv_svn_valid = TRUE;
       enc_dev_ser_msg->imeisv_svn_len =   encrypted_info_len;
       memscpy((char*)&enc_dev_ser_msg->imeisv_svn ,sizeof(enc_dev_ser_msg->imeisv_svn),(char*)&dev_serial_num_encrypted_info,encrypted_info_len);

    }
    else
    {
      dev_ser_msg->imeisv_svn_valid = TRUE;
      strlcpy((char*)&dev_ser_msg->imeisv_svn,(char*)&dev_serial_num_info,sizeof(dev_ser_msg->imeisv_svn));
    }	 
  }
  else
  {
    local_err = QMI_DMSI_LOCAL_ERR_NULL_IMEISV;
  }
#endif /* defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE) || defined(FEATURE_TDSCDMA) */
  if(resp_flag == FALSE && errval == QMI_ERR_NONE_V01)
  {
    local_err = QMI_DMSI_LOCAL_ERR_NOT_PROVISIONED;
    errval = QMI_ERR_NOT_PROVISIONED_V01;
  }
  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
send_result:
  LOG_MSG_INFO1_1("DMS get device serial numbers exit with local_err %d", local_err);
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
  return errval;

} /* qmi_dmsi_get_device_serial_numbers_ext() */



/*===========================================================================
  FUNCTION QMI_DMSI_GET_POWER_STATE()

  DESCRIPTION
    Gives the status of the device power including battery strength of the
    device and whether the device is externally powered
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_power_state
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_power_state_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /*-------------------------------------------------------------------------
    Build the response from back to front
  -------------------------------------------------------------------------*/
  memset(&resp_msg,0,sizeof(dms_get_power_state_resp_msg_v01));
  qmi_if_get_power_state(&(resp_msg.power_state.power_status), 
                         &(resp_msg.power_state.battery_lvl));

  resp_msg.resp.error = QMI_ERR_NONE_V01;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_power_state_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_get_power_state() */



/*===========================================================================
  FUNCTION QMI_DMSI_SET_EVENT_REPORT()

  DESCRIPTION
    To set state for event reporting 
        
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_set_event_report
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_dmsi_client_info_type *  dms_client_sp;
  qmi_dmsi_cmd_buf_type     *  cmd_ptr;
  dms_set_event_report_resp_msg_v01 resp_msg;
  dms_set_event_report_req_msg_v01* req_msg;
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  uint8    curr_power_state;
  uint8    curr_bat_lvl;
  uint8    type = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(req_c_struct && clnt_info);

  memset(&resp_msg,0,sizeof(dms_set_event_report_resp_msg_v01));
  curr_bat_lvl = 0;
  req_msg = (dms_set_event_report_req_msg_v01*)req_c_struct;
  dms_client_sp = (qmi_dmsi_client_info_type *) clnt_info;

  /*-------------------------------------------------------------------------
    If ALL optional TLVs are absent return error or else carry on and
    process the request.
  -------------------------------------------------------------------------*/
  if(!(req_msg->report_power_state_valid ||
       req_msg->lvl_limits_valid ||
       req_msg->report_pin_state_valid || 
       req_msg->report_activation_state_valid ||
       req_msg->report_oprt_mode_state_valid ||
       req_msg->report_uim_state_valid ||
       req_msg->report_wireless_disable_state_valid ||
       req_msg->report_prl_init_valid ||
       req_msg->report_cdma_lock_mode_valid ||
       req_msg->report_device_multisim_info_valid ))
  {
      errval = QMI_ERR_MISSING_ARG_V01;
      goto send_result;
  }

  /*-------------------------------------------------------------------------
    Perform 'supported' error checking on all provided TLV's before processing
  -------------------------------------------------------------------------*/
#ifndef FEATURE_MMGSDI_SESSION_LIB
  if (req_msg->report_pin_state_valid  || req_msg->report_uim_state_valid)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
    goto send_result;
  }
#endif /* FEATURE_MMGSDI_SESSION_LIB */

#ifndef FEATURE_OTASP
  if (req_msg->report_activation_state_valid)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
    goto send_result;
  }
#endif /* FEATURE_OTASP */

  if (req_msg->report_wireless_disable_state_valid)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    change power state reporting if tlv included in request
  -------------------------------------------------------------------------*/
  if( req_msg->report_power_state_valid )
  {
    if(req_msg->report_power_state > 0)
    { 
      req_msg->report_power_state = TRUE;
    }
    else
    {
      req_msg->report_power_state = FALSE;
    }  
    /*-----------------------------------------------------------------------
      if power state report_mode is same as previous do nothing
    -----------------------------------------------------------------------*/
    if(dms_client_sp->report_status.report_power_state !=
       req_msg->report_power_state)
    {
      dms_client_sp->report_status.report_power_state =
        req_msg->report_power_state;
      if (req_msg->report_power_state == FALSE)
      {
        qmi_dmsi_global.pwr_rpt.report_power_state_client_count--; 

        if(qmi_dmsi_global.pwr_rpt.report_power_state_client_count == 0)
        {
          qmi_charger_set_power_state_reporting(FALSE);
        }
      }

      if(req_msg->report_power_state == TRUE)
      {
        qmi_dmsi_global.pwr_rpt.report_power_state_client_count++;

        if(qmi_dmsi_global.pwr_rpt.report_power_state_client_count == 1)
        {
          qmi_charger_set_power_state_reporting(TRUE);
        }
        else
        { 
          /*-----------------------------------------------------------------
            Since 2nd or more client has registered, reset the previous
            power state mask in Charger, so that it causes a charger event
            and a DMS ind can be sent to this new client
          -----------------------------------------------------------------*/
          qmi_charger_reset_prev_power_state_mask();
        }
      }
    }
  }
  
  /*-------------------------------------------------------------------------
    change power state report thresholds if tlv included in request
  -------------------------------------------------------------------------*/
  if( req_msg->lvl_limits_valid )  
  {
    /*-----------------------------------------------------------------------
      validate limits.
      - if lower is less than upper, swap them
      - if lower is less than min, set to min
      - if upper is more than max, set to max
    -----------------------------------------------------------------------*/ 
    if (req_msg->lvl_limits.battery_lvl_lower_limit >
	         req_msg->lvl_limits.battery_lvl_upper_limit)
    {
      type = req_msg->lvl_limits.battery_lvl_lower_limit;
      req_msg->lvl_limits.battery_lvl_lower_limit = 
                      req_msg->lvl_limits.battery_lvl_upper_limit;
      req_msg->lvl_limits.battery_lvl_upper_limit = type;
    }

    if (req_msg->lvl_limits.battery_lvl_upper_limit > DMSI_BAT_LVL_MAX)
    {
      req_msg->lvl_limits.battery_lvl_upper_limit = DMSI_BAT_LVL_MAX;
    }

    /*-----------------------------------------------------------------------
      commit limits to client state
    -----------------------------------------------------------------------*/ 
    dms_client_sp->report_status.bat_lvl_lower_limit =
      req_msg->lvl_limits.battery_lvl_lower_limit;
    dms_client_sp->report_status.bat_lvl_upper_limit =
      req_msg->lvl_limits.battery_lvl_upper_limit;

    /*-----------------------------------------------------------------------
      Change OEM charger report limits if applicable

       Generate event_report ind if current power level is outside
       registered range
    -----------------------------------------------------------------------*/ 
    qmi_if_get_power_state(&curr_power_state, 
                           &curr_bat_lvl);
  }
  /*-------------------------------------------------------------------------
    Update the battery level limits - determine the new high and low battery 
    limits and set them in the charger module. Note - if the new limits in the
    request are such that there is no change in the high, low limits for the 
    charger, it will result in again setting the existing charger limits, 
    but that is alright
  -------------------------------------------------------------------------*/
  if(req_msg->report_power_state_valid || req_msg->lvl_limits_valid)
  {
    qmi_dmsi_set_battery_level_limits(curr_bat_lvl);
  }
                                                 
  if (req_msg->report_activation_state_valid)
  {
    if (0 < req_msg->report_activation_state)
    {
      dms_client_sp->report_status.report_activation_state = TRUE;
    }
    else
    {
      dms_client_sp->report_status.report_activation_state = FALSE;
    }
  }
  
  /*-------------------------------------------------------------------------
    Update setting for reporting the operating mode if TLV present
  -------------------------------------------------------------------------*/
  if (req_msg->report_oprt_mode_state_valid)
  {
    dms_client_sp->report_status.report_operating_mode =
      (0 < req_msg->report_oprt_mode_state)? TRUE: FALSE;
  }

  /*-------------------------------------------------------------------------
    Update setting for reporting the wireless disable switch state
  -------------------------------------------------------------------------*/
  if (req_msg->report_wireless_disable_state_valid)
  {
    if (0 < req_msg->report_wireless_disable_state)
    {
      dms_client_sp->report_status.report_wd_state = TRUE;
    }
    else
    {
      dms_client_sp->report_status.report_wd_state = FALSE;
    }
  }

  if (req_msg->report_prl_init_valid)
  {
    dms_client_sp->report_status.report_prl_init = 
                     (0 < req_msg->report_prl_init ) ? TRUE : FALSE; 
  }

  /*-------------------------------------------------------------------------
    Update setting for reporting cdma lock mode if TLV present
  -------------------------------------------------------------------------*/
  if (req_msg->report_cdma_lock_mode_valid )
  {
    LOG_MSG_INFO2_0("In cdma lock Event Report indication!");
    dms_client_sp->report_status.report_cdma_lock_mode =
      (0 < req_msg->report_cdma_lock_mode)? TRUE: FALSE;

    /* If a client registers for QMI indication, the indication must be sent right away */
    /* Post a command to dcc task to send the indication */
    if(dms_client_sp->report_status.report_cdma_lock_mode == TRUE)
    {

      cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
      if( cmd_ptr == NULL)
      {
        errval = QMI_ERR_NO_MEMORY_V01;
        goto send_result;
      }
      memset( cmd_ptr, 0x00, sizeof(*cmd_ptr) );

      cmd_ptr->cmd_id= QMI_CMD_DMS_EVT_REPORT_REQ_IND;
      cmd_ptr->data.dms_evt_info.evt_type = DMSI_REPORT_STATUS_EV_CDMA_LOCK_MODE;
      cmd_ptr->data.dms_evt_info.evt_value = &qmi_dmsi_global.cdma_lock_mode;
      cmd_ptr->data.dms_evt_info.cl_sp = dms_client_sp;
      dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
    }
  }

  if (req_msg->report_device_multisim_info_valid)
  {
    dms_client_sp->report_status.report_device_mode =
      (0 < req_msg->report_device_multisim_info)? TRUE: FALSE;
    if(dms_client_sp->report_status.report_device_mode == TRUE)
    {
       /*Send the current info related to client*/
      cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
      if( cmd_ptr == NULL)
      {
        errval = QMI_ERR_NO_MEMORY_V01;
        goto send_result;
      }
      memset( cmd_ptr, 0x00, sizeof(*cmd_ptr) );

      cmd_ptr->cmd_id= QMI_CMD_DMS_EVT_REPORT_REQ_IND;
      cmd_ptr->data.dms_evt_info.evt_type = DMSI_REPORT_STATUS_EV_DEV_MSIM_CAP;
      cmd_ptr->data.dms_evt_info.evt_value = NULL;
      cmd_ptr->data.dms_evt_info.cl_sp = dms_client_sp;
      dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
    }
  }

  /*-------------------------------------------------------------------------
    Build the response from back to front
    No Failure possible upto here so result is success
  -------------------------------------------------------------------------*/
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_event_report_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;


} /* qmi_dmsi_set_event_report() */



/*===========================================================================
  FUNCTION QMI_DMSI_EVENT_REPORT_IND()

  DESCRIPTION
    Is called when any of the conditions set in set_event_report becomes true
    Sends an indication to the client
        
  PARAMETERS
    sp           : service provided state pointer (user data)
    clid         : clientID
    event_mask   : mask to indicate what caused the indication to be called
    sig_strength : current signal strength
    radio_if     : radio technology in use
    power_status : power status of the mobile
    battery_lvl  : battery level
    
  RETURN VALUE
    void
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_event_report_ind
(
  qmi_dmsi_client_info_type *   clnt_info,
  uint32   event_mask,
  uint8    power_status,  /* bitmask */
  uint8    battery_lvl,
  uint8    pin_index,
  uint16   activation_state,
  uint8    operating_mode,
  boolean  wd_state,
  boolean  cdma_lock_mode
)
{
  qmi_error_type_v01            errval  = QMI_ERR_NONE_V01;
  dms_event_report_ind_msg_v01* ind_msg = NULL;
  qmi_csi_error                 rc = 0;
  qmi_if_info_type  *qmi_if_info_ptr;
  policyman_status_t         status = -1;
  policyman_item_t const     *cm_pm_device_config = NULL;
  policyman_item_id_t        ids[] = { POLICYMAN_ITEM_DEVICE_CONFIGURATION };

  DS_QMI_DMS_ASSERT(clnt_info != NULL);
  PS_SYSTEM_HEAP_MEM_ALLOC(ind_msg, sizeof(dms_event_report_ind_msg_v01),
                           dms_event_report_ind_msg_v01*);
  if(ind_msg == NULL)
  {
    return;
  }
  memset(ind_msg,0,sizeof(dms_event_report_ind_msg_v01));
  if(event_mask & DMSI_REPORT_STATUS_EV_DEV_MSIM_CAP) 
  {
    status = policyman_get_items(
              &ids[0],
              sizeof(ids)/sizeof(policyman_item_id_t),
              &cm_pm_device_config);
    if (POLICYMAN_FAILED(status) || cm_pm_device_config == NULL)
    {
      errval = QMI_ERR_INTERNAL_V01;
      LOG_MSG_ERROR_INT_0("policyman_get_items failed");
      goto bail;
    }
    do 
    {
     errval = qmi_dmsi_get_device_config_list((void*)&(ind_msg->device_config_list),
                                              (int16_t*)&(ind_msg->explicit_config_index),
                                              cm_pm_device_config);
     if(errval != QMI_ERR_NONE_V01)
     {
       break;
     }
     else
     {
       ind_msg->device_config_list_valid    = TRUE;
       ind_msg->explicit_config_index_valid = TRUE;
     }

     errval = qmi_dmsi_get_multisim_info(&(ind_msg->multisim_capability), cm_pm_device_config);
     if(errval != QMI_ERR_NONE_V01)
     {
        break;
     }
     else
     {
       ind_msg->multisim_capability_valid = TRUE;
     }

     /*Do call msim info thing to add 3 tlvs and mention it is ind or response*/
     /*Sending TRUE as second arg for indication*/
     errval =  qmi_dmsi_get_device_multisim_current_cap_info((void*)ind_msg,TRUE,cm_pm_device_config);
     if(errval != QMI_ERR_NONE_V01)
     {
        break;
     }
     else
     {
       ind_msg->current_multisim_capability_valid = TRUE;
       ind_msg->current_subscription_capability_valid = TRUE;
       ind_msg->subs_voice_data_capability_valid = TRUE;
     }
      /*get the max_active_data subs for ind or response*/
      errval =  qmi_dmsi_get_max_active_data_subs(&(ind_msg->max_active_data_subscriptions),cm_pm_device_config);
      if(errval != QMI_ERR_NONE_V01)
      {
        break;
      }
      else
      {
        ind_msg->max_active_data_subscriptions_valid = TRUE;
      }
    }while(0);
    if(errval != QMI_ERR_NONE_V01)
    {
      PS_SYSTEM_HEAP_MEM_FREE(ind_msg);
      goto bail;
    }
  }

  /*-------------------------------------------------------------------------
    ind was called for power state and battery lvl
  -------------------------------------------------------------------------*/
  if(event_mask & DMSI_REPORT_STATUS_EV_POWER_STATE) 
  {
    ind_msg->power_state_valid = TRUE;
    ind_msg->power_state.power_status = power_status;
    ind_msg->power_state.battery_lvl = battery_lvl;
  }

  if(event_mask & DMSI_REPORT_STATUS_EV_ACTIVATION_STATE) 
  {
    ind_msg->activation_state_valid = TRUE;
    ind_msg->activation_state = activation_state;
  }

  if (event_mask & DMSI_REPORT_STATUS_EV_OPRT_MODE)
  {
    ind_msg->operating_mode_valid = TRUE;
    ind_msg->operating_mode = operating_mode;
  }

  if (event_mask & DMSI_REPORT_STATUS_EV_WD_STATE)
  {
    ind_msg->wireless_disable_state_valid = TRUE;
    ind_msg->wireless_disable_state = (wd_state == TRUE) ? 1 : 0;
  }

  if (event_mask & DMSI_REPORT_STATUS_EV_PRL_INIT)
  {
    qmi_if_info_ptr = qmi_if_info_get_ptr();
    ind_msg->prl_init_valid = TRUE;
    ind_msg->prl_init = 1;
    ind_msg->prl_info_valid =TRUE;
    ind_msg->prl_info.prl_version = qmi_if_info_ptr->ph_info.prl_info[clnt_info->subscription_id].prl_id;
    ind_msg->prl_info.prl_only = qmi_if_info_ptr->ph_info.prl_info[clnt_info->subscription_id].prl_only;
    ind_msg->prl_info.prl_source = 
       (dms_prl_source_enum_v01)qmi_if_info_ptr->ph_info.prl_info[clnt_info->subscription_id].prl_source;
  }

  if (event_mask & DMSI_REPORT_STATUS_EV_CDMA_LOCK_MODE)
  {
    ind_msg->cdma_lock_mode_state_valid = TRUE;
    ind_msg->cdma_lock_mode_state = (cdma_lock_mode == TRUE) ? 1 : 0;
  }

  rc = qmi_csi_send_ind( clnt_info->clnt,
                    QMI_DMS_EVENT_REPORT_IND_V01,
                    (void*)ind_msg, 
                    sizeof(dms_event_report_ind_msg_v01));

  PS_SYSTEM_HEAP_MEM_FREE(ind_msg);
bail:
  if(NULL != cm_pm_device_config)
  {
    policyman_item_release(cm_pm_device_config);
  }

  if ((errval != QMI_ERR_NONE_V01) || (rc != QMI_CSI_NO_ERR))
  {
    LOG_MSG_ERROR_INT_2("Unable to generate Event Report indication! d; d",errval, rc);
  }
} /* qmi_dmsi_event_report_ind() */


/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_HARDWARE_REV()

  DESCRIPTION
    Gives the device hardware revision
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_device_hardware_rev
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  uint32               ver;
  dms_get_device_hardware_rev_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_device_hardware_rev_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Populate the HW revision
  -------------------------------------------------------------------------*/
  LOG_MSG_INFO1_0("dms query device hardware rev");

  ver = DalChipInfo_ChipVersion();

 (void) dsatutil_itoa(ver,(uint8*)&(resp_msg.hardware_rev), DMSI_BASE_HEX);
  resp_msg.hardware_rev[QMI_DMS_HARDWARE_REV_MAX_V01-1] = '\0';
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_device_hardware_rev_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

  
} /* qmi_dmsi_get_device_hardware_rev() */



/*===========================================================================
  FUNCTION QMI_DMSI_SET_OPERATING_MODE()

  DESCRIPTION
    Sets the operating mode of the device
        
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_set_operating_mode
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  nv_stat_enum_type  nv_status;
  qmi_dmsi_client_info_type           * client_sp;
  dms_set_operating_mode_req_msg_v01  * req_msg = NULL;
  dms_set_operating_mode_resp_msg_v01 resp_msg;
  ds_qmi_dms_async_cmd_type *     cmd_buf_p = NULL;
  sys_oprt_mode_e_type      new_oprt_mode;
  dms_operating_mode_enum_v01 curr_oprt_mode;
  boolean                   got_curr_oprt_mode = FALSE;
  cm_oprt_mode_params_s_type            new_oprt_mode_params;
  qmi_if_info_type                    * qmi_if_info_ptr;
  nv_item_type                        * dcc_nv_item_ptr = NULL;
  qmi_dmsi_cmd_buf_type *  cmd_ptr = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);

  client_sp = (qmi_dmsi_client_info_type *)clnt_info;
  req_msg   = (dms_set_operating_mode_req_msg_v01*)req_c_struct;
  memset(&resp_msg,0,sizeof(dms_set_operating_mode_resp_msg_v01));

  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Populate the local structure with the values coming in the request and
    also validate the values
  -------------------------------------------------------------------------*/
  LOG_MSG_INFO1_0("dms set operating mode");

  new_oprt_mode = SYS_OPRT_MODE_NONE;

  switch(req_msg->operating_mode)
  {
    case DMS_OP_MODE_SHUTTING_DOWN_V01:
      new_oprt_mode = SYS_OPRT_MODE_PWROFF;
      break;

    case DMS_OP_MODE_FACTORY_TEST_MODE_V01 :
      new_oprt_mode = SYS_OPRT_MODE_FTM;
      break;

    case DMS_OP_MODE_OFFLINE_V01 :
      new_oprt_mode = SYS_OPRT_MODE_OFFLINE;
      break;

    case DMS_OP_MODE_ONLINE_V01 :
      new_oprt_mode = SYS_OPRT_MODE_ONLINE;
      TTLBOOTUP(QMI_DMS_ONLINE);

      break;

    case DMS_OP_MODE_LOW_POWER_V01 :
    case DMS_OP_MODE_PERSISTENT_LOW_POWER_V01 :
    case DMS_OP_MODE_MODE_ONLY_LOW_POWER_V01 :
      new_oprt_mode = SYS_OPRT_MODE_LPM;
      break;

    case DMS_OP_MODE_RESETTING_V01 :
      new_oprt_mode = SYS_OPRT_MODE_RESET ;
      break;

    default:
      /* Any other value is considered invalid */
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
      break;
  }
 /*If new mode is of LPM/RESET/SHUTDOWN check if operation is allowed with CM*/
  if( ((new_oprt_mode == SYS_OPRT_MODE_LPM) ||
       (new_oprt_mode == SYS_OPRT_MODE_RESET) ||
       (new_oprt_mode == SYS_OPRT_MODE_PWROFF)) &&
      !cm_rpm_check_reset_allowed() )
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_0("CM denied LPM/Reset/Shutdown cmd");
    goto send_result;
  }

  qmi_if_info_ptr = qmi_if_info_get_ptr();
  /* Check wireless disable switch and prevent mode change to ONLINE or FTM */
  if ( qmi_if_info_ptr->ph_info.wd_switch_on )
  {
    if ((DMS_OP_MODE_ONLINE_V01 == req_msg->operating_mode) ||
        (DMS_OP_MODE_FACTORY_TEST_MODE_V01    == req_msg->operating_mode))
    {
      errval = QMI_ERR_HARDWARE_RESTRICTED_V01;
      LOG_MSG_INFO1_1("Could not change mode to %d with wireless disable switch on",
                      req_msg->operating_mode);
      goto send_result;
    }
  }

  /*-------------------------------------------------------------------------
                  Update NV LPM flag if mode changes to LPM or Persistent LPM 
   -------------------------------------------------------------------------*/
  if ((DMS_OP_MODE_PERSISTENT_LOW_POWER_V01 == req_msg->operating_mode) ||
      (DMS_OP_MODE_LOW_POWER_V01 == req_msg->operating_mode))
  {
    /*Get current operating mode*/
    if( QMI_ERR_NONE_V01 == qmi_dmsi_oprt_mode_from_cm( qmi_if_info_ptr->ph_info.oprt_mode, 
                                                    &curr_oprt_mode ) )
    {
      got_curr_oprt_mode = TRUE;
    }
    else
    {
      LOG_MSG_INFO1_1("get current oprt mode failed sys oprt mode = %d\n",
                      qmi_if_info_ptr->ph_info.oprt_mode);
      got_curr_oprt_mode = FALSE;
    }

    /* Read the nv for LPM persistence flag */
    nv_status = dcc_get_nv_item( NV_LPM_PERSISTENCE_I, dcc_nv_item_ptr );
    switch (nv_status)
    {
      case NV_DONE_S:
        /* Clear nv for LPM or set it for P_LPM only if the nv doesn't match
           the mode request*/
        if (dcc_nv_item_ptr->lpm_persistence != 
              (DMS_OP_MODE_PERSISTENT_LOW_POWER_V01 == req_msg->operating_mode))
        {
          dcc_nv_item_ptr->lpm_persistence = 
                 (DMS_OP_MODE_PERSISTENT_LOW_POWER_V01 == req_msg->operating_mode);
          if (NV_DONE_S != dcc_put_nv_item(NV_LPM_PERSISTENCE_I, dcc_nv_item_ptr))
          {
            errval = QMI_ERR_INTERNAL_V01;
            LOG_MSG_ERROR_INT_1("Could not set NV_LPM_PERSISTENCE_I to [d]",
                            dcc_nv_item_ptr->lpm_persistence);
            goto send_result;
          }
        }
        break;

      case NV_NOTACTIVE_S:
          dcc_nv_item_ptr->lpm_persistence = 
              (DMS_OP_MODE_PERSISTENT_LOW_POWER_V01 == req_msg->operating_mode);
          if (NV_DONE_S != dcc_put_nv_item(NV_LPM_PERSISTENCE_I, dcc_nv_item_ptr))
          {
            errval = QMI_ERR_INTERNAL_V01;
            LOG_MSG_ERROR_INT_1("Could not set NV_LPM_PERSISTENCE_I to [d]",
                            dcc_nv_item_ptr->lpm_persistence);
            goto send_result;
          }
          break;

      default:
        errval = QMI_ERR_INTERNAL_V01;
        goto send_result;
        /* no need for break after goto */
    }

    /* if current operating mode is known, check for LPM<->P-LPM changes */
    if( got_curr_oprt_mode )
    {
      if  ( (DMS_OP_MODE_LOW_POWER_V01 == req_msg->operating_mode  && 
             DMS_OP_MODE_PERSISTENT_LOW_POWER_V01 == curr_oprt_mode )|| 
            (DMS_OP_MODE_PERSISTENT_LOW_POWER_V01 == req_msg->operating_mode &&
             DMS_OP_MODE_LOW_POWER_V01 == curr_oprt_mode) )
      { 
        /* if a change has happened, send an IND */
        qmi_dms_generate_oprt_mode_ind(SYS_OPRT_MODE_LPM);
      }
    }
  }

  /* ----------------------------------------------------------
     Check if e911 call is pending
    -----------------------------------------------------------*/
  new_oprt_mode_params.oprt_mode = new_oprt_mode;
  new_oprt_mode_params.e911_pending_state = CM_E911_PENDING_STATE_NONE;
  new_oprt_mode_params.e911_pending_sub = SYS_MODEM_AS_ID_NONE;

  if( req_msg->e911_pending_info_valid &&
      req_msg->operating_mode == DMS_OP_MODE_ONLINE_V01 )
  {
    new_oprt_mode_params.e911_pending_state =
      req_msg->e911_pending_info.is_pending ?
        CM_E911_PENDING_STATE_ON : CM_E911_PENDING_STATE_OFF;
    if( req_msg->e911_pending_info.is_pending )
    {
      switch(req_msg->e911_pending_info.subscription_id)
      {
        case DMS_PRIMARY_SUBS_V01 :
          new_oprt_mode_params.e911_pending_sub = SYS_MODEM_AS_ID_1;
          break;
        case DMS_SECONDARY_SUBS_V01 :
          new_oprt_mode_params.e911_pending_sub = SYS_MODEM_AS_ID_2;
          break;
        case DMS_TERTIARY_SUBS_V01 :
          new_oprt_mode_params.e911_pending_sub = SYS_MODEM_AS_ID_3;
          break;
        default:
          errval = QMI_ERR_INVALID_ARG_V01;          
          goto send_result;
      }
    }
  }
  
  /* For shutdown/Reset call sys monitor and send immediate response */
  if ( (DMS_OP_MODE_RESETTING_V01 == req_msg->operating_mode) || 
       (DMS_OP_MODE_SHUTTING_DOWN_V01 == req_msg->operating_mode) )
  {
    LOG_MSG_INFO1_1("Triggered DMS OPRT MODE %d by sys mon APIs",req_msg->operating_mode);

    if(DMS_OP_MODE_RESETTING_V01 == req_msg->operating_mode)
    {
      sys_m_initiate_shutdown();
    }
    else
    {
      sys_m_initiate_poweroff();
    }

   /* Post a command to DCC task to send event report indication */

    cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
    if( cmd_ptr != NULL)
    {
      cmd_ptr->cmd_id  = QMI_CMD_DMS_RESET_SHUTDOWN;
      cmd_ptr->data.cm_ph.info.oprt_mode = new_oprt_mode;
      dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
    }
    else
    {
      LOG_MSG_ERROR_INT_0("Out of cmd buf");
    }

    errval = QMI_ERR_NONE_V01;
    goto send_result;
  }

  if (SYS_OPRT_MODE_NONE != new_oprt_mode)
  {
    /* ----------------------------------------------------------
     Allocated temp memory for resp_buf_type 
    -----------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(cmd_buf_p,
                             sizeof(ds_qmi_dms_async_cmd_type),
                             ds_qmi_dms_async_cmd_type*);
    if( cmd_buf_p == NULL )
    {
      errval = QMI_ERR_NO_MEMORY_V01;
      goto send_result;
    }
    memset(cmd_buf_p,0,sizeof(ds_qmi_dms_async_cmd_type));
    cmd_buf_p->req_handle = req_handle;
    cmd_buf_p->msg_id = msg_id;
    cmd_buf_p->cl_sp = clnt_info;
    if ( !cm_ph_cmd_oprt_mode3( qmi_dmsi_cm_ph_cb,
                               cmd_buf_p,
                               qmi_if_info_ptr->cm_client_id,
                                &new_oprt_mode_params ) )
    {
      LOG_MSG_ERROR_1 ("Unable to change operating mode, cm_ph_cmd_oprt_mode"
                       " fails in Set Oper Mode Cmd 0x%x", cmd_buf_p);
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
    LOG_MSG_INFO2_3("Response pending for Set Operating Mode %d,"
                    "e911_pending_state %d e911_pending_sub %d\t sent to cm",
                           new_oprt_mode_params.oprt_mode,
                           new_oprt_mode_params.e911_pending_state,
                           new_oprt_mode_params.e911_pending_sub);
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
    return QMI_CSI_CB_NO_ERR; 
  }

send_result:
  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
  if(cmd_buf_p != NULL)
  {
    PS_SYSTEM_HEAP_MEM_FREE(cmd_buf_p);
  }
  result = (errval == QMI_ERR_NONE_V01)?QMI_RESULT_SUCCESS_V01: 
                                        QMI_RESULT_FAILURE_V01;
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_operating_mode_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_set_operating_mode() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_OPERATING_MODE()

  DESCRIPTION
    Gives the operating mode of the device
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_operating_mode
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_get_operating_mode_resp_msg_v01 resp_msg;
  dms_operating_mode_enum_v01  operating_mode;
  qmi_if_info_type  *qmi_if_info_ptr;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_operating_mode_resp_msg_v01));
  qmi_if_info_ptr = qmi_if_info_get_ptr();
  operating_mode = DMS_OP_MODE_ONLINE_V01;

  /*-------------------------------------------------------------------------
    Perform query action
  -------------------------------------------------------------------------*/
  LOG_MSG_INFO1_0("dms query current operating mode");

  errval = qmi_dmsi_oprt_mode_from_cm( qmi_if_info_ptr->ph_info.oprt_mode, 
                                       &operating_mode );
  if (QMI_ERR_NONE_V01 == errval)
  {
    resp_msg.operating_mode = (dms_operating_mode_enum_v01)operating_mode;
  }

  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_operating_mode_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_get_operating_mode() */


/*===========================================================================
  FUNCTION QMI_DMSI_GET_CURRENT_PRL_INFO()

  DESCRIPTION
    Gives current active PRL (Preferred Roaming List) info of the device
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_current_prl_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_if_info_type  *qmi_if_info_ptr;
  dms_get_current_prl_info_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_current_prl_info_resp_msg_v01));
#if (!defined(FEATURE_CDMA_800) && !defined(FEATURE_CDMA_1900))
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
  goto send_result;
#endif /* (!defined(FEATURE_CDMA_800) && !defined(FEATURE_CDMA_1900)) */

  qmi_if_info_ptr = qmi_if_info_get_ptr();
  LOG_MSG_INFO2_2("qmi_dmsi_get_current_prl_info, prl_info_valid = %d for sub_id %d",
  	              qmi_if_info_ptr->prl_info_valid[clnt_info->subscription_id], clnt_info->subscription_id);
  
  if (qmi_if_info_ptr->prl_info_valid[clnt_info->subscription_id] != TRUE ||
      qmi_if_info_ptr->ph_info.prl_info[clnt_info->subscription_id].is_prl_load_complete != TRUE)
  {
    errval = QMI_ERR_INFO_UNAVAILABLE_V01;
    goto send_result;
  }
  /* Send PRL info on which the client is bound */
  resp_msg.prl_only_valid = TRUE;
  resp_msg.prl_only = qmi_if_info_ptr->ph_info.prl_info[clnt_info->subscription_id].prl_only;
  resp_msg.prl_version_valid = TRUE;
  resp_msg.prl_version = qmi_if_info_ptr->ph_info.prl_info[clnt_info->subscription_id].prl_id;
  resp_msg.prl_source_valid = TRUE;
  resp_msg.prl_source =
   (dms_prl_source_enum_v01)qmi_if_info_ptr->ph_info.prl_info[clnt_info->subscription_id].prl_source;

send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_current_prl_info_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

 
} /* qmi_dmsi_get_current_prl_info() */

/*===========================================================================
  FUNCTION QMI_DMSI_IMAGE_INDEX_TO_TYPE()

  DESCRIPTION
    Convert image index to image type.
    
  PARAMETERS
    image_index: image index

  RETURN VALUE
    uint32: image type
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint32 qmi_dmsi_image_index_to_type
(
  uint8  image_index
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   * Passing through of image index is not allowed so we have to
   * convert the image index to the image type defined in the QMI spec.
   * image_index is defined in core\debugtools\version\src\image_versino.h
   * This conversion is a workaround before image_index definition is exposed
   * throught core API
  -------------------------------------------------------------------------*/
  if (image_index <= 5)
  {
    return image_index + 1;
  }
  else if (image_index >= 9 && image_index <= 14)
  {
    return image_index - 2;
  }

  return 0;

} /* qmi_dmsi_image_index_to_type() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_SW_VERSION()

  DESCRIPTION
    Gives the software version of the device.
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_dmsi_get_sw_version
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  char              *sw_version_info = NULL;
  char               sw_ver2[QMI_DMS_SW_VERSION_MAX_V01 + 1]; /*+1 for NULL char*/
  uint16             offset = 0;
  uint16             len = 0;
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  nv_stat_enum_type     nv_status;
  dms_get_sw_version_resp_msg_v01 resp_msg;

  qmi_dmsi_image_ver_type * image_ver = NULL;
  uint32                    image_ver_table_size = 0;
  char                      index_str[IMAGE_INDEX_LEN + 1];
  char                      qc_ver_str[IMAGE_QC_VER_LEN + 1];
  char                      variant_str[IMAGE_VARIANT_LEN + 1];
  char                      oem_ver_str[IMAGE_OEM_VER_LEN + 1];
  char                      image_ver_str[IMAGE_VER_STR_SIZE];
  uint8                     image_index;
  uint32                    image_type;
  uint8                     count = 0;
  int                       i;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  memset(&resp_msg,0,sizeof(dms_get_sw_version_resp_msg_v01));

  /*-------------------------------------------------------------------------
    Populate image versions 
  -------------------------------------------------------------------------*/
  image_ver = (qmi_dmsi_image_ver_type *)
          smem_get_addr(SMEM_IMAGE_VERSION_TABLE, &image_ver_table_size);

  if (image_ver_table_size / sizeof(qmi_dmsi_image_ver_type) > QMI_DMS_IMAGE_VER_MAX_V01)
  {
    LOG_MSG_ERROR_1("qmi_dmsi_get_sw_version, image_ver_table_size / sizeof(qmi_dmsi_image_ver_type) :%d",
                    image_ver_table_size / sizeof(qmi_dmsi_image_ver_type));

    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  if (image_ver != NULL)
  {
    resp_msg.image_versions_valid = TRUE;
    for (i = image_ver_table_size / sizeof(qmi_dmsi_image_ver_type) - 1; i >= 0; i--)
    {
      if (image_ver[i].colon1 == ':')
      {
        // Copy strings
        memscpy(index_str, IMAGE_INDEX_LEN, image_ver[i].index, IMAGE_INDEX_LEN);
        memscpy(qc_ver_str, IMAGE_QC_VER_LEN, image_ver[i].qc_ver, IMAGE_QC_VER_LEN);
        memscpy(variant_str, IMAGE_VARIANT_LEN, image_ver[i].variant, IMAGE_VARIANT_LEN);
        memscpy(oem_ver_str, IMAGE_OEM_VER_LEN, image_ver[i].oem_ver, IMAGE_OEM_VER_LEN);

        index_str[IMAGE_INDEX_LEN] = '\0';
        qc_ver_str[IMAGE_QC_VER_LEN] = '\0';
        variant_str[IMAGE_VARIANT_LEN] = '\0';
        oem_ver_str[IMAGE_OEM_VER_LEN] = '\0';

        image_index = (uint8)atoi(index_str);
        if (image_index != i)
        {
          LOG_MSG_ERROR_2("Invalid image index %d %d", image_index, i);
          continue;
        }

        // Number of valid image versions
        count++;

        // Convert image index to image type
        image_type = qmi_dmsi_image_index_to_type(image_index);

        // Image version string format is "QC:Variant:OEM"
        snprintf(image_ver_str, IMAGE_VER_STR_SIZE, "%s:%s:%s",
                              qc_ver_str, variant_str, oem_ver_str);
        resp_msg.image_versions[resp_msg.image_versions_len].image_type = image_type;
        strlcpy(&(resp_msg.image_versions[resp_msg.image_versions_len].image_ver[0]),
                 image_ver_str,sizeof(resp_msg.image_versions[resp_msg.image_versions_len].image_ver));
        resp_msg.image_versions_len++;
      }
    }
  }

  /*-------------------------------------------------------------------------
    Mandatory TLV
  -------------------------------------------------------------------------*/
  memset((void *)&dcc_nv_item, 0, sizeof(dcc_nv_item));
  nv_status = dcc_get_nv_item(NV_SW_VERSION_INFO_I, &dcc_nv_item);
  if(nv_status == NV_DONE_S)
  {
    sw_version_info = (char *)dcc_nv_item.sw_version_info;
  } 
  else
  {
   /*mob_sw_rev is a global extern variable*/
    sw_version_info = (char*)mob_sw_rev;
  }

  len = (uint16)strlen(sw_version_info);
  
  if((sw_version_info == NULL) || (len == 0))
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
    goto send_result;
  }

  /*Max length of sw_version in IDL is 32 bytes only. 
    if exceeds truncate the string from start*/
  if(len > QMI_DMS_SW_VERSION_MAX_V01)
  {
   /* sw_version examples, old: "M9615ACETWTAAM5011116.1" (< 32 bytes)
       new : "MPSS.DI.2.0-01022-M8974AAAAANAZM-1" (>  32 bytes)
      truncate first N characters from start i.e., �MPSS.....� 
      output will be DI.2.0-01022-M8974AAAAANAZM-1 */
     memset(sw_ver2, 0, sizeof(sw_ver2));
     offset  = len - QMI_DMS_SW_VERSION_MAX_V01;
     (void) strlcpy (sw_ver2,
                      (char*)(sw_version_info + offset),
                      (QMI_DMS_SW_VERSION_MAX_V01 + 1) );  /*+1 for null char*/

     /*Send the truncated string in response*/
     sw_version_info = (char*)sw_ver2;

  /*-------------------------------------------------------------------------
    As Mandatory TLV was not enough lengh include optional tlv
  -------------------------------------------------------------------------*/
   resp_msg.sw_version_ext_valid = TRUE;
   strlcpy((char*)&(resp_msg.sw_version_ext),(char*)mob_sw_rev, 
            sizeof(resp_msg.sw_version_ext));

  }
  /*-------------------------------------------------------------------------
    Fill the Mandatory TLV
  -------------------------------------------------------------------------*/
  strlcpy((char*)&(resp_msg.sw_version),(char*)sw_version_info,
           sizeof(resp_msg.sw_version));
  send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                        : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_sw_version_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_get_sw_version */
/*===========================================================================
  FUNCTION QMI_DMSI_SET_SPC()

  DESCRIPTION
    Updates (Set) the Service Programming Code. After validating the current 
    SPC code. 
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_dmsi_set_spc
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_if_spc_result_e_type check_spc_result = QMI_SPC_FAIL;
  qmi_if_info_type       *qmi_if_info_ptr;
  dms_set_spc_req_msg_v01*  req_msg = NULL;
  dms_set_spc_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);
  req_msg = (dms_set_spc_req_msg_v01*)req_c_struct;
  memset(&resp_msg,0,sizeof(dms_set_spc_resp_msg_v01));
  
 
   /*-----------------------------------------------------------------------
     Check the supplied SPC is valid
   -----------------------------------------------------------------------*/
   check_spc_result = qmi_if_check_spc((uint8 *)req_msg->curr_spc, FALSE); 
   
   if(check_spc_result == QMI_SPC_FAIL)
   {
     errval = QMI_ERR_AUTHENTICATION_FAILED_V01;
     goto send_result;
   }

   if(check_spc_result == QMI_SPC_LOCKED)
   {
     errval = QMI_ERR_AUTHENTICATION_LOCK_V01;
     goto send_result;
   }

   if(check_spc_result == QMI_SPC_ERR_INTERNAL)
   {
     errval = QMI_ERR_INTERNAL_V01;
     goto send_result;
   }

   /* store the SPC lock code into NV.*/
   
    memscpy((void *)dcc_nv_item.sec_code.digits, 
            NV_SEC_CODE_SIZE,
           (char *)req_msg->new_spc,
           NV_SEC_CODE_SIZE);
   
    if (NV_DONE_S != dcc_put_nv_item(NV_SEC_CODE_I, &dcc_nv_item))
    {
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }

    /* Update cached value in QMI IF*/
    qmi_if_info_ptr = qmi_if_info_get_ptr();
    qmi_if_info_ptr->nv_sec_code = dcc_nv_item.sec_code;
    /*-------------------------------------------------------------------------
    Build the response from back to front
    No Failure possible upto here so result is success
  -------------------------------------------------------------------------*/
send_result:
  LOG_MSG_INFO1_1("Set Service Programming Code exit with errval %d", errval);
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_spc_resp_msg_v01));  
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_set_spc */
/*===========================================================================
  FUNCTION QMI_DMSI_GET_ACTIVATED_STATE()

  DESCRIPTION
    Gives the activation state of the device
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_activated_state
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_activation_state_resp_msg_v01 resp_msg;

  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if !defined(FEATURE_CDMA_800) && !defined(FEATURE_CDMA_1900)
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
#else /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
  memset(&resp_msg,0,sizeof(dms_get_activation_state_resp_msg_v01));
  resp_msg.activation_state = DMS_ACTIVATION_NOT_ACTIVATED_V01;

  /*-------------------------------------------------------------------------
    If an activation is in progress return the intermediate state
  -------------------------------------------------------------------------*/
  if ((DMS_ACTIVATION_NOT_ACTIVATED_V01!=qmi_dmsi_global.activation_state) &&
      (DMS_ACTIVATION_ACTIVATED_V01    !=qmi_dmsi_global.activation_state) &&
      (DMS_ACTIVATION_STATE_ENUM_MAX_ENUM_VAL_V01   !=qmi_dmsi_global.activation_state))
  {
    resp_msg.activation_state = (uint16) qmi_dmsi_global.activation_state;
  }
  /*-------------------------------------------------------------------------
    Else re-calculate the current activated state of the device to return
  -------------------------------------------------------------------------*/
  else
  {
    resp_msg.activation_state = (uint16) qmi_dmsi_calculate_activated_state();
  }
  
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_activation_state_resp_msg_v01));  
  return QMI_CSI_CB_NO_ERR;
  
} /* qmi_dmsi_get_activated_state() */



/*===========================================================================
  FUNCTION QMI_DMSI_ACTIVATE_AUTOMATIC()

  DESCRIPTION
    Performs an automatic service provisioning update with the OTA network.
        
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_activate_automatic
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_if_info_type  *qmi_if_info_ptr;
  dms_activate_automatic_req_msg_v01  *req_msg = NULL;
  dms_activate_automatic_resp_msg_v01  resp_msg;
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
#if defined(FEATURE_OTASP)
  /* used to make the CM call origination request */
  cm_num_s_type                 dial_num;
  cm_cdma_orig_params_s_type    call_orig_params;
  cm_orig_alpha_s_type          alpha_buf;
#endif /* defined(FEATURE_OTASP) */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

  nv_item_type      *dcc_nv_item_ptr;
  qmi_dms_client_subs_e_type  sub_id;
  ds_qmi_dms_async_cmd_type * pending_cmd = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);

  qmi_if_info_ptr = qmi_if_info_get_ptr();
  sub_id  = 0;
  req_msg = (dms_activate_automatic_req_msg_v01*)req_c_struct;
  memset(&resp_msg,0,sizeof(dms_activate_automatic_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  sub_id = clnt_info->subscription_id;
  if(sub_id != (qmi_dms_client_subs_e_type)qmi_if_get_current_cdma_sub_id()) 
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  if( dcc_nv_item_ptr == NULL )
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)

  /*-------------------------------------------------------------------------
    Perform automatic activation
  -------------------------------------------------------------------------*/
  LOG_MSG_INFO1_0("dms activate automatic");

  /* OTASP requires an activation code, make sure SOMETHING was specified */
  if (req_msg->activation_code.act_code[0] == '\0')
  {
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  /* check that the device has minimally been activated */
  if(NV_DONE_S != dcc_get_nv_item_ext(NV_ESN_I, dcc_nv_item_ptr, (uint16)sub_id))
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
    goto send_result;
  } 
  if (0 == dcc_nv_item_ptr->esn.esn)
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
    goto send_result;
  }

  /* If qmi_if_info has not yet been inited, no CM Serving System events have
     been received which means the device has not yet gone online.
     If qmi_if_info has been inited, check the service status (1x-only check
     is fine since the OTASP call will be voice).
     */
  if ((!(qmi_if_info_ptr->inited)) ||
      (qmi_if_info_ptr->inited && 
      (SYS_SRV_STATUS_SRV != qmi_if_info_ptr->ss_info[sub_id].srv_status)))
  {
    errval = QMI_ERR_NO_NETWORK_FOUND_V01;
    goto send_result;
  }

#if defined(FEATURE_OTASP)
  /* default the CM call origination parameters */
  memset(&dial_num, 0, sizeof(cm_num_s_type));
  memset(&alpha_buf, 0, sizeof(cm_orig_alpha_s_type));
  memset(&call_orig_params, CM_CALL_CMD_PARAM_DEFAULT_VALUE, 
                                                    sizeof(call_orig_params));

  cm_num_fill(&dial_num, 
              (uint8 *) &(req_msg->activation_code.act_code), 
              strlen(req_msg->activation_code.act_code), 
              CM_DIGIT_MODE_8BIT_ASCII);

  PS_SYSTEM_HEAP_MEM_ALLOC(pending_cmd,
                           sizeof(ds_qmi_dms_async_cmd_type),
                           ds_qmi_dms_async_cmd_type*);
  if(pending_cmd != NULL)
  {
    memset(pending_cmd,0,sizeof(ds_qmi_dms_async_cmd_type));
    pending_cmd->req_handle = req_handle;
    pending_cmd->msg_id     = msg_id;
    pending_cmd->cl_sp      = clnt_info;
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  /* create the CM call origination request */
  if (cm_mm_call_cmd_orig(qmi_dms_cm_call_cmd_cb,
                  (void *) pending_cmd,
                  qmi_if_info_ptr->cm_client_id,
                  CM_CALL_TYPE_VOICE,
                  CM_SRV_TYPE_AUTOMATIC,
                  NULL,
                  &dial_num,
                  &alpha_buf,
                  &call_orig_params, /* cdma orig params */
                  NULL, /* GW CS parameters */
                  NULL, /* GW PS parameters */
                  &qmi_dmsi_global.voice_call_id
                  ))
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
    return QMI_CSI_CB_NO_ERR; 
  }
  else
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
#else /* defined(FEATURE_OTASP) */
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
#endif /* defined(FEATURE_OTASP) */
#else /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
send_result:	 
  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
  PS_SYSTEM_HEAP_MEM_FREE(pending_cmd);
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);

  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_activate_automatic_resp_msg_v01));  
  return QMI_CSI_CB_NO_ERR;
  
} /* qmi_dmsi_activate_automatic() */



/*===========================================================================
  FUNCTION QMI_DMSI_ACTIVATE_MANUAL()

  DESCRIPTION
    Performs a manual service provisioning with data provided by the user
        
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_activate_manual
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01 result;
  dms_activate_manual_req_msg_v01 *req_msg = NULL;
  dms_activate_manual_resp_msg_v01 resp_msg;
  
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  int32    temp;
  qmi_if_spc_result_e_type      check_spc_result;
  dword                         qmi_dmsi_manual_min1;
  word                          qmi_dmsi_manual_min2;
  nv_items_enum_type            item_id;
  nv_stat_enum_type             nv_status;
  nv_ds_mip_ss_user_prof_type * ss_profile;
  byte *                        data_ptr;
  char *                        min_validate_ptr ;
  byte                          curr_nam;
  byte                          curr_index;
  uint8 mdn_len = 0;
  uint8 min_len = 0;
  uint8 mn_ha_len = 0;
  uint8 mn_aaa_len = 0;
  uint8 spc_len = 0;
  uint16 prl_total_len = 0;
  uint16 sub_id = 0;
  size_t copy_len = 0;

  char default_nai[QMI_NV_ITEM_SIZE_MAX];

#ifdef FEATURE_SD20
  word                     prl_version = PRL_DEFAULT_VER;
#endif

  boolean write_all_parameters;
#endif  /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

  nv_item_type      *dcc_nv_item_ptr;
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);

  req_msg = (dms_activate_manual_req_msg_v01*)req_c_struct;
  memset(&resp_msg,0,sizeof(dms_activate_manual_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    local_err = QMI_DMSI_LOCAL_ERR_NO_MEMORY;
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }

#if (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900))
  memset(default_nai, 0, sizeof(default_nai));
  sub_id = (uint16)qmi_if_get_current_cdma_sub_id();

#if defined(FEATURE_RUIM) && defined(FEATURE_UIM_RUN_TIME_ENABLE)
  /*-------------------------------------------------------------------------
    Changing provisioning cannot be done on a RUIM card. Do not change any
    values, only allow the UQCN processing.
  -------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_MPSS_AT20_DEV
  if (NV_RTRE_CONTROL_USE_RUIM == nv_rtre_control())
#else
  if (NV_RTRE_CONTROL_USE_RUIM == nv_rtre_control_ext((uint16)qmi_if_get_current_cdma_sub_id()))
#endif /* FEATURE_DATA_MPSS_AT20_DEV */
  {
    local_err = QMI_DMSI_LOCAL_ERR_ADM_PROTECTED_RUIM;
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
    goto send_result;
  }
#endif /* defined(FEATURE_RUIM) && defined(FEATURE_UIM_RUN_TIME_ENABLE) */
  
  /*-------------------------------------------------------------------------
    Populate the local structure with the values coming in the request and
    also validate the values
  -------------------------------------------------------------------------*/
 

  write_all_parameters = TRUE;
  /* check if configured to allow writing the PRL only */
  if (TRUE == qmi_dmsi_global.nv_allow_invalid_spc)
  {
    /* if allowed, check the specified SPC against the 'invalid' one to see if
       client is trying to write only the PRL */
    write_all_parameters = strncmp((char *) req_msg->activation_data.spc, 
                                       SPECIAL_INVALID_SPC, 
                                       strlen(SPECIAL_INVALID_SPC));
  }

  /* if the request is to write only the PRL (special SPC given), no other TLVs
     should be specified in the request as they must have the real SPC */
  if (!write_all_parameters)
  {
    if (req_msg->mn_ha_key_valid || req_msg->mn_aaa_key_valid)
    {
      local_err = QMI_DMSI_LOCAL_ERR_SPECIAL_SPC_MIP_NOT_ALLOWED;
      errval = QMI_ERR_AUTHENTICATION_FAILED_V01;
      goto send_result;
    }
  }
  else
  {
    /* Check SPC first , no need to validate values and give any special err for
       SPC, make it obscure */
    check_spc_result = qmi_if_check_spc((uint8 *)req_msg->activation_data.spc, TRUE); 
  
    if(check_spc_result == QMI_SPC_FAIL)
    {
      local_err = QMI_DMSI_LOCAL_CHECK_SPC_FAIL;
      errval = QMI_ERR_AUTHENTICATION_FAILED_V01;
      goto send_result;
    }

    if(check_spc_result == QMI_SPC_LOCKED)
    {
      local_err = QMI_DMSI_LOCAL_CHECK_SPC_FAIL;
      errval = QMI_ERR_AUTHENTICATION_LOCK_V01;
      goto send_result;
    }

    if(check_spc_result == QMI_SPC_ERR_INTERNAL)
    {
      local_err = QMI_DMSI_LOCAL_CHECK_SPC_FAIL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
  }
  spc_len = strlen((char*)&req_msg->activation_data.spc);
  mdn_len = strlen((char*)&req_msg->activation_data.mdn);
  min_len = strlen((char*)&req_msg->activation_data.min);
  
  /* ------------------------------------------------------------------
  Validate PARAMs here first ... PRL, MN-HA and MN-AAA keys, MIN, MDN 
  --------------------------------------------------------------------*/
  if(req_msg->mn_ha_key_valid)/* If MN-HA TLV received validate its length */
  {
    mn_ha_len = strlen((char*)&req_msg->mn_ha_key.mn_ha_key);
    if( mn_ha_len == 0 )
	{
	  errval = QMI_ERR_INVALID_ARG_V01;
	  local_err = QMI_DMSI_LOCAL_ERR_INVAL_HA_LEN;
	  goto send_result;
	}
  }

  if(req_msg->mn_aaa_key_valid) /* If MN-AAA TLV received validate its length */
  {
    mn_aaa_len = strlen((char*)&req_msg->mn_aaa_key.mn_aaa_key);
    if( mn_aaa_len == 0 )
    {
      errval = QMI_ERR_INVALID_ARG_V01;
      local_err = QMI_DMSI_LOCAL_ERR_INVAL_AAA_LEN;
      goto send_result;
    }
  }

  if (write_all_parameters)
  {
    /* Validate MIN length and contents */
    min_validate_ptr = (char *)&req_msg->activation_data.min;
    if(min_len != DMSI_MIN_SIZ)
    {
      errval = QMI_ERR_INVALID_ARG_V01;
      local_err = QMI_DMSI_LOCAL_ERR_INVAL_MIN_LEN;
      goto send_result;
    }
    else
    {
      for (temp=0; temp < min_len; temp++ ) 
      {
        if ( (*(min_validate_ptr+temp) < '0') || 
             (*(min_validate_ptr+temp) > '9') )
        {
          errval = QMI_ERR_INVALID_ARG_V01;
          local_err = QMI_DMSI_LOCAL_ERR_INVAL_MIN_CONTENTS;
          goto send_result;
        }
      }
    }
  }

  curr_nam = (byte) CM_NAM_1;
  nv_status = dcc_get_nv_item(NV_CURR_NAM_I, dcc_nv_item_ptr);
  if (nv_status == NV_DONE_S && dcc_nv_item_ptr->curr_nam < NV_MAX_NAMS)
  {
    curr_nam = (byte) dcc_nv_item_ptr->curr_nam;
  }
  LOG_MSG_INFO3_1("Updating NAM %d", curr_nam);

  curr_index = 0;
  /* read the active MIP profile index and verify it is within the valid range*/
  nv_status = dcc_get_nv_item_ext(NV_DS_MIP_ACTIVE_PROF_I, dcc_nv_item_ptr, sub_id);
  if ((NV_DONE_S == nv_status) && 
      (NV_DS_MIP_MAX_NUM_PROF > dcc_nv_item_ptr->ds_mip_active_prof))
  {
    curr_index = dcc_nv_item_ptr->ds_mip_active_prof;
  }
  LOG_MSG_INFO3_1("Updating MIP profile %d", curr_index);

  if(req_msg->pref_roaming_list_valid) /* If PRL TLV recvd validate it here */
  {
    prl_total_len =  req_msg->pref_roaming_list.prl_total_len;
    /* if a transfer series is not in progress */
    if (NULL == qmi_dmsi_global.prl_info.cl_sp)
    {
      /* reset all variables */
      qmi_dmsi_reset_prl_series_info();

      /* set the current control point calling this function */
      qmi_dmsi_global.prl_info.cl_sp = 
                  (qmi_dmsi_client_info_type *)clnt_info;
      
      /* and the total length to that provided */
      qmi_dmsi_global.prl_info.len_total = prl_total_len;
    }

    /* check that this is the control point that started the series */
    else if (qmi_dmsi_global.prl_info.cl_sp != 
                  ((qmi_dmsi_client_info_type *)clnt_info))
    {
      errval = QMI_ERR_DEVICE_IN_USE_V01;
      local_err = QMI_DMSI_LOCAL_ERR_PRL_ALREADY_WRITE;
      goto send_result;
    }

    /* check that the total prl length is not too large */
    if (NV_ROAMING_LIST_MAX_SIZE <= prl_total_len)
    {
      errval = QMI_ERR_ARG_TOO_LONG_V01;
      local_err = QMI_DMSI_LOCAL_ERR_PRL_TOTAL_LEN;
      goto send_result;
    }
    
    /*  check the total PRL length is not 0 or total PRL size is
        different than previous requests */
    if ((0 == prl_total_len) || 
        (qmi_dmsi_global.prl_info.len_total != prl_total_len))
    {
      errval = QMI_ERR_INVALID_ARG_V01;
      local_err = QMI_DMSI_LOCAL_ERR_INVAL_PRL_LEN;
      goto send_result;
    }
    
    /* check that the segment length is not too large */
    if (QMI_DMS_PRL_DATA_MAX_V01 <= req_msg->pref_roaming_list.prl_len)
    {
      errval = QMI_ERR_SEGMENT_TOO_LONG_V01;
      local_err = QMI_DMSI_LOCAL_ERR_PRL_SEGMENT_LEN;
      goto send_result;
    }
    
    /*  check the Segment length is not 0 or the total received length
        is not large then total prl length  */
    if ((0 == req_msg->pref_roaming_list.prl_len) || 
        (prl_total_len < 
           (qmi_dmsi_global.prl_info.len_received + req_msg->pref_roaming_list.prl_len)))
    {
      errval = QMI_ERR_INVALID_ARG_V01;
      local_err = QMI_DMSI_LOCAL_ERR_INVAL_PRL_SEGMENT_LEN;
      goto send_result;
    }

    /* check that the segment sequence number is correct */
    if (qmi_dmsi_global.prl_info.seq_num != req_msg->pref_roaming_list.prl_seg_num)
    {
      errval = QMI_ERR_SEGMENT_ORDER_V01;
      local_err = QMI_DMSI_LOCAL_ERR_SEGMENT_ORDER;
      goto send_result;
    }

    /* If this is the first segment alloc memory for prl data */
    if(qmi_dmsi_global.prl_info.seq_num == 0)
    {
      PS_SYSTEM_HEAP_MEM_ALLOC(qmi_dmsi_global.prl_info.data, 
                               qmi_dmsi_global.prl_info.len_total,
                               uint8*);
      if(NULL == qmi_dmsi_global.prl_info.data)
      {
        errval = QMI_ERR_NO_MEMORY_V01;
        local_err = QMI_DMSI_LOCAL_ERR_NO_MEMORY;
        goto send_result;
      }
      memset(qmi_dmsi_global.prl_info.data, 0, qmi_dmsi_global.prl_info.len_total);
    }

    /* append the current PRL segment */
    memscpy(&qmi_dmsi_global.prl_info.data[qmi_dmsi_global.prl_info.len_received],
            qmi_dmsi_global.prl_info.len_total - qmi_dmsi_global.prl_info.len_received,
            &req_msg->pref_roaming_list.prl[0], 
            req_msg->pref_roaming_list.prl_len);

    /* update the PRL series variables with the information provided */
    qmi_dmsi_global.prl_info.len_received += req_msg->pref_roaming_list.prl_len;
    qmi_dmsi_global.prl_info.seq_num++;

    /* check that all the PRL segments have been written */
    if (qmi_dmsi_global.prl_info.len_total == 
                                          qmi_dmsi_global.prl_info.len_received)
    {
      LOG_MSG_INFO2_0("writing PRL");
#ifdef FEATURE_SD20
      /* Get PRL protocol revision from NV. Validate PRL revision from nv first
      ** then with all other available protocol revisions next.
      ** Update revision in nv if returned revision does not match that from nv.
      */
  
      nv_status = dcc_get_nv_item(NV_SSPR_P_REV_I, dcc_nv_item_ptr);
      if(nv_status != NV_DONE_S)
      {
        dcc_nv_item_ptr->sspr_p_rev = PRL_SSPR_P_REV_INVALID;
        sspr_p_rev = PRL_SSPR_P_REV_1;
      }
      else
      {
        sspr_p_rev = (prl_sspr_p_rev_e_type)dcc_nv_item_ptr->sspr_p_rev;
      }

      /* Validate PRL now */
      if (PRL_VALID != prl_validate( qmi_dmsi_global.prl_info.data,
                                     &prl_version,
                                     &sspr_p_rev,
                                     TRUE))
      {
        errval = QMI_ERR_INVALID_ARG_V01;
        local_err = QMI_DMSI_LOCAL_ERR_INVAL_PRL;
        goto send_result;
      }
      else
      {
        if(qmi_dmsi_global.prl_info.len_received > NV_ROAMING_LIST_MAX_SIZE)
        {
          errval = QMI_ERR_ARG_TOO_LONG_V01;
          local_err = QMI_DMSI_LOCAL_ERR_PRL_INFO_LEN;
          goto send_result;
        }

        memset(&nv_pr_list, 0, sizeof(nv_pr_list));
        memscpy(nv_pr_list.roaming_list, 
                qmi_dmsi_global.prl_info.len_received,
               qmi_dmsi_global.prl_info.data, 
               qmi_dmsi_global.prl_info.len_received);
        nv_pr_list.nam = curr_nam;
        nv_pr_list.prl_version = prl_version;
        nv_pr_list.size = qmi_dmsi_global.prl_info.len_received * 8;
        nv_pr_list.valid = TRUE;

#ifdef FEATURE_IS683A_PRL
        item_id = NV_ROAMING_LIST_683_I;
#else /* FEATURE_IS683A_PRL */
        item_id = NV_ROAMING_LIST_I;
#endif /* FEATURE_IS683A_PRL */
        if (NV_DONE_S != dcc_put_nv_item_ext(item_id, (nv_item_type*)&nv_pr_list, sub_id))
        {
          errval = QMI_ERR_INTERNAL_V01;
          local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
          goto send_result;
        }
      }
      
      if ( sspr_p_rev != dcc_nv_item_ptr->sspr_p_rev) 
      {
        dcc_nv_item_ptr->sspr_p_rev = sspr_p_rev;
        if (NV_DONE_S != dcc_put_nv_item (NV_SSPR_P_REV_I, dcc_nv_item_ptr))
        {
          errval = QMI_ERR_INTERNAL_V01;
          local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
          goto send_result;
        }
      }

      /* series finished, free variables so it can be restarted again */
      qmi_dmsi_reset_prl_series_info();
#else /* FEATURE_SD20 */
      errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
      local_err = QMI_DMSI_LOCAL_OP_DEVICE_UNSUPPORTED_V01;
      goto send_result;
#endif /* FEATURE_SD20 */
    }
  }/* If PRL TLV recvd validate it here */

  /* ------------------------------------------------------------------
     Commit mandatory items to NV
  --------------------------------------------------------------------*/
  if (write_all_parameters)
  {
   /* MDN, this also validates len and content so not done before */
    memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
    dcc_nv_item_ptr->mob_dir_number.nam = curr_nam;
    if (!qmi_dmsi_mobnum_text_to_pcs((char *)&req_msg->activation_data.mdn, 
                                   mdn_len, 
                                   &dcc_nv_item_ptr->mob_dir_number))
    {
      errval = QMI_ERR_INVALID_ARG_V01;
      local_err = QMI_DMSI_LOCAL_ERR_MDN;
      goto send_result;
    }
    if( NV_DONE_S != dcc_put_nv_item_ext( NV_DIR_NUMBER_PCS_I,
                                    dcc_nv_item_ptr, sub_id ))
    {
      errval = QMI_ERR_INTERNAL_V01;
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      goto send_result;
    }

    /* MIN */
    if(!qmi_dmsi_min_text_to_min12((char *)&req_msg->activation_data.min, 
                                   min_len, 
                                   &qmi_dmsi_manual_min1, 
                                   &qmi_dmsi_manual_min2))
    {
      errval = QMI_ERR_INTERNAL_V01;
      local_err = QMI_DMSI_LOCAL_ERR_INVAL_MIN_CONTENTS;
      goto send_result;
    }

    /* Write MIN1, both CDMA MIN1 and Analog */
    memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
    dcc_nv_item_ptr->min1.nam = curr_nam;
    dcc_nv_item_ptr->min1.min1[NV_ANALOG_MIN_INDEX] = qmi_dmsi_manual_min1;
    dcc_nv_item_ptr->min1.min1[NV_CDMA_MIN_INDEX]   = qmi_dmsi_manual_min1;
    if( NV_DONE_S != dcc_put_nv_item_ext( NV_MIN1_I, dcc_nv_item_ptr, sub_id ) )
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }

    /* Write MIN2, both CDMA and Analog */
    dcc_nv_item_ptr->min2.nam = curr_nam;
    dcc_nv_item_ptr->min2.min2[NV_ANALOG_MIN_INDEX] = qmi_dmsi_manual_min2;
    dcc_nv_item_ptr->min2.min2[NV_CDMA_MIN_INDEX]   = qmi_dmsi_manual_min2;
    if( NV_DONE_S != dcc_put_nv_item_ext( NV_MIN2_I, dcc_nv_item_ptr, sub_id ) )
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }

    /* Update ACCOLC with last digit of MIN (ASCII digit to binary) */
    dcc_nv_item_ptr->accolc.nam = curr_nam;  
    dcc_nv_item_ptr->accolc.ACCOLCpClass[NV_CDMA_MIN_INDEX] = 
                         req_msg->activation_data.min[min_len - 1] - '0';
    if( NV_DONE_S != dcc_put_nv_item_ext( NV_ACCOLC_I,
                                    dcc_nv_item_ptr, sub_id ) )
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }

    /* Home Sid , no validation done for home SID */
#ifdef FEATURE_SSPR_ENHANCEMENTS
    /* NV Item #259 */
    dcc_nv_item_ptr->home_sid_nid.nam = curr_nam;
    /* Read this NV Item to check if it is already active */
    nv_status = dcc_get_nv_item_ext( NV_HOME_SID_NID_I, dcc_nv_item_ptr, sub_id);
    if ((nv_status == NV_DONE_S) || (nv_status == NV_NOTACTIVE_S))                                   
    {
      // Modify only the SID and write it to NV
      dcc_nv_item_ptr->home_sid_nid.pair[curr_nam].sid = 
                                ps_ntohs(req_msg->activation_data.sid); 
      nv_status = dcc_put_nv_item_ext( NV_HOME_SID_NID_I, dcc_nv_item_ptr, sub_id);
      if(nv_status != NV_DONE_S)                                  
      {
        local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
    	  errval = QMI_ERR_INTERNAL_V01;
    	  goto send_result;
      }
    }
    else
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
#else /* FEATURE_SSPR_ENHANCEMENTS */
    /* NV Item #38 */
    dcc_nv_item_ptr->sid_nid.nam = curr_nam;
    /* Read this NV Item to check if it is already active */
    nv_status = dcc_get_nv_item_ext(NV_SID_NID_I, dcc_nv_item_ptr, sub_id);
    if((nv_status == NV_DONE_S) || (nv_status == NV_NOTACTIVE_S))                                 
    {
      dcc_nv_item_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][curr_nam].sid = 
                                   ps_ntohs(req_msg->activation_data.sid);
      nv_status = dcc_put_nv_item_ext(NV_SID_NID_I, dcc_nv_item_ptr, sub_id);
      if(nv_status != NV_DONE_S)                                  
      {
        local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
        errval = QMI_ERR_INTERNAL_V01;
        goto send_result;
      }
    }
    else
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
#endif /* FEATURE_SSPR_ENHANCEMENTS */

    // Only execute this on the last PRL segment or non PRL request
    if (req_msg->pref_roaming_list_valid && qmi_dmsi_global.prl_info.len_received)  {
       goto send_result;
    }

    /*--------------------------------------------------------------------------
       Update default SIP/MIP NAIs to MDN@CONSTANT as per requirements 
    --------------------------------------------------------------------------*/
    /* Once provisioning is completed need to update the other provisioning
       values that are based on ones that may have changed.  NAI includes the
       MDN so anytime the MDN changes it should be updated.*/
    memset(default_nai, 0, sizeof(default_nai));
    if (QMI_NV_STATUS_OK != qmi_nv_read(QMI_NV_ITEM_DEFAULT_NAI, 0, 0,
                                        (void *)default_nai,
                                        sizeof(default_nai)))
    {
      memset(default_nai, 0, sizeof(default_nai));
    }
    memset((void *)&dcc_nv_item_ptr->ppp_user_id.user_id[0], 0, 
           sizeof(dcc_nv_item_ptr->ppp_user_id.user_id));
    copy_len = memscpy((void *)&dcc_nv_item_ptr->ppp_user_id.user_id[0],
                       NV_MAX_PPP_USER_ID_LENGTH,
                       (void *)req_msg->activation_data.mdn, 
                       mdn_len);
    copy_len += memscpy((void *)&dcc_nv_item_ptr->ppp_user_id.user_id[mdn_len],
                        NV_MAX_PPP_USER_ID_LENGTH - mdn_len,
                        (void *)default_nai, 
                        strlen(default_nai) + 1);
    dcc_nv_item_ptr->ppp_user_id.user_id_len = copy_len;

    nv_status = dcc_put_nv_item_ext (NV_PPP_USER_ID_I, dcc_nv_item_ptr, sub_id);
    if (nv_status != NV_DONE_S)
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
  
    nv_status = dcc_put_nv_item_ext (NV_HDR_AN_AUTH_USER_ID_LONG_I, dcc_nv_item_ptr, sub_id);
    if (nv_status != NV_DONE_S)
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
  
    dcc_nv_item_ptr->ds_mip_gen_user_prof.index = curr_index;
    nv_status = dcc_get_nv_item_ext (NV_DS_MIP_GEN_USER_PROF_I, dcc_nv_item_ptr, sub_id);
    if ((nv_status != NV_DONE_S) && (nv_status != NV_NOTACTIVE_S))
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
    else
    {
      copy_len = memscpy((void *)&dcc_nv_item_ptr->ds_mip_gen_user_prof.nai[0],
                         NV_MAX_NAI_LENGTH,
                         (void *)req_msg->activation_data.mdn, 
                         mdn_len);
      copy_len += memscpy((void *)&dcc_nv_item_ptr->ds_mip_gen_user_prof.nai[mdn_len],
                          NV_MAX_NAI_LENGTH - mdn_len,
                          (void *)default_nai, 
                          strlen(default_nai) + 1);
      dcc_nv_item_ptr->ds_mip_gen_user_prof.nai_length = copy_len;
  
      nv_status = dcc_put_nv_item_ext(NV_DS_MIP_GEN_USER_PROF_I, dcc_nv_item_ptr, sub_id);
      if (nv_status != NV_DONE_S)
      {
        local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
        errval = QMI_ERR_INTERNAL_V01;
        goto send_result;
      }
    }
  } /* if (write_all_parameters) */

  /*--------------------------------------------------------------------------
	  Write MN-HA and MN-AAA shared secrets to active MIP profile
  --------------------------------------------------------------------------*/
  if((req_msg->mn_ha_key_valid) || (req_msg->mn_aaa_key_valid))
  {
    dcc_nv_item_ptr->ds_mip_ss_user_prof.index = curr_index;
    nv_status = dcc_get_nv_item_ext(NV_DS_MIP_SS_USER_PROF_I, dcc_nv_item_ptr, sub_id);
    if ((nv_status != NV_DONE_S) && (nv_status != NV_NOTACTIVE_S))
    {
      local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
    else
    {
      ss_profile = &dcc_nv_item_ptr->ds_mip_ss_user_prof;
      if (req_msg->mn_ha_key_valid)
      {
        // mn_ha_len validated before 
        ss_profile->mn_ha_shared_secret_length = mn_ha_len;
        data_ptr = (byte *) ss_profile->mn_ha_shared_secret;
        memscpy (data_ptr, 
                 mn_ha_len, 
                (uint8 *)req_msg->mn_ha_key.mn_ha_key, 
                 mn_ha_len);
      }

      if (req_msg->mn_aaa_key_valid)
      {
	      // mn_aaa_len validated before 
        ss_profile->mn_aaa_shared_secret_length = mn_aaa_len;
        data_ptr = (byte *) ss_profile->mn_aaa_shared_secret;
        memscpy (data_ptr, 
                 mn_aaa_len, 
                 (uint8 *)req_msg->mn_aaa_key.mn_aaa_key, 
                 mn_aaa_len);
      }

      nv_status = dcc_put_nv_item_ext(NV_DS_MIP_SS_USER_PROF_I, dcc_nv_item_ptr, sub_id);
      if (nv_status != NV_DONE_S)
      {
        errval = QMI_ERR_INTERNAL_V01;
        goto send_result;
      }
    }/* else , nv_status != NV_DONE_S for NV_DS_MIP_SS_USER_PROF_I*/

  }/* (got_v_in_opt2) || (got_v_in_opt3) */
#else /* (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)) */
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
#endif /* (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)) */

send_result:
#if (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900))
  if ((QMI_ERR_NONE_V01 != errval) && (req_msg->pref_roaming_list_valid))
  {
    /* reset the PRL transmission series */
    qmi_dmsi_reset_prl_series_info();
  }
#endif /* (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)) */

  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);

  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_activate_manual_resp_msg_v01));  

  LOG_MSG_INFO1_1("DMS Activate Manual exit with local_err %d", local_err);
  return QMI_CSI_CB_NO_ERR;
  
} /* qmi_dmsi_activate_manual() */


/*===========================================================================
  FUNCTION QMI_DMSI_READ_ERI_FILE()

  DESCRIPTION
    Reads the contents ERI file maintained by the device
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_read_eri_file
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_nv_status_e_type nv_status;
  fs_size_t            bytes_read;
  dms_read_eri_file_resp_msg_v01 resp_msg;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  bytes_read = 0;
  memset(&resp_msg, 0, sizeof(dms_read_eri_file_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Perform query action
  -------------------------------------------------------------------------*/
  nv_status = qmi_nv_file_read(QMI_NV_FILE_ERI,
                               (void *)&(resp_msg.eri_file.eri_data),
                               (fs_size_t)QMI_DMS_ERI_DATA_MAX_V01,
                               &bytes_read);
  
  if (QMI_NV_STATUS_OK == nv_status)
  {
    resp_msg.eri_file.eri_data_len = (uint16) bytes_read;
  } 
  else if (QMI_NV_STATUS_NOT_ACTIVE == nv_status) 
  {
    resp_msg.eri_file.eri_data_len = 0;
  } 
  else 
  {
    errval = QMI_ERR_INTERNAL_V01;
  }

  LOG_MSG_INFO2_3("Read ERI file with bytes %d,status %d and errval %d",bytes_read,nv_status,errval);
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_read_eri_file_resp_msg_v01));  
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_read_eri_file() */

/*===========================================================================
  FUNCTION QMI_DMSI_RESTORE_FACTORY_DEFAULTS()

  DESCRIPTION
    Performs the restore factory defaults procedure
        
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_restore_factory_defaults
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_restore_factory_defaults_req_msg_v01* req_msg;
  dms_restore_factory_defaults_resp_msg_v01 resp_msg;
  qmi_if_spc_result_e_type	check_spc_result;
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  uint8 i;
  uint16 sub_id;
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)*/
  nv_item_type      *dcc_nv_item_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(req_c_struct != NULL);

  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  errval = QMI_ERR_NONE_V01;
  req_msg = (dms_restore_factory_defaults_req_msg_v01*)req_c_struct; 
  memset(&resp_msg, 0, sizeof(dms_restore_factory_defaults_resp_msg_v01));
  
  if(!req_c_struct_len)
  {
    errval = QMI_ERR_MISSING_ARG_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Populate the local structure with the values coming in the request and
    also validate the values
  -------------------------------------------------------------------------*/
  LOG_MSG_INFO1_0("dms restore factory defaults");

  /* Check SPC first , no need to validate values and give any special err for
     SPC, make it obscure */
  if ((FALSE == qmi_dmsi_global.nv_allow_invalid_spc) ||
      (0 != strncmp((char *)req_msg->spc, 
                        SPECIAL_INVALID_SPC, 
                        strlen(SPECIAL_INVALID_SPC))))
  {
    check_spc_result = qmi_if_check_spc((uint8 *)req_msg->spc, FALSE); 
  
    if(check_spc_result == QMI_SPC_FAIL)
    {
    	errval = QMI_ERR_AUTHENTICATION_FAILED_V01;
    	goto send_result;
    }
  
    if(check_spc_result == QMI_SPC_LOCKED)
    {
    	errval = QMI_ERR_AUTHENTICATION_LOCK_V01;
    	goto send_result;
    }

    if(check_spc_result == QMI_SPC_ERR_INTERNAL)
    {
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
  }

  /*-------------------------------------------------------------------------
    Initiate the factory defaults procedure

    For C2K and UMTS:
    
    For C2K, only:
    Set the IMSI Components to 0
    Set the IMSI_T Components to 0
    Set MDN = "0000000000"
    
    For UMTS, only:
    
    Reboot! - this is expected to be intiated by the PC Connection Manager !
   -------------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    Reset items for both C2K and UMTS builds.
   -------------------------------------------------------------------------*/

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  /*-------------------------------------------------------------------------
    Reset items for C2K builds.
   -------------------------------------------------------------------------*/
  sub_id = (uint16)qmi_if_get_current_cdma_sub_id();
  
#if defined(FEATURE_RUIM) && defined(FEATURE_UIM_RUN_TIME_ENABLE)
  /*-------------------------------------------------------------------------
    Changing provisioning cannot be done on a RUIM card. Do not change any
    values, only allow the UQCN processing.
  -------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_MPSS_AT20_DEV
  if (NV_RTRE_CONTROL_USE_RUIM == nv_rtre_control())
#else
  if (NV_RTRE_CONTROL_USE_RUIM == nv_rtre_control_ext((uint16)qmi_if_get_current_cdma_sub_id()))
#endif /* FEATURE_DATA_MPSS_AT20_DEV */
  {
    LOG_MSG_INFO1_0("Cannot change ADM-protected values on RUIM");
  }
  else
#endif /* defined(FEATURE_RUIM) && defined(FEATURE_UIM_RUN_TIME_ENABLE) */
  {
    nv_item_type *dcc_nv_mdn_ptr = NULL;

    /*-------------------------------------------------------------------------
      Allocate temporary memory for the NV item
    -------------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_mdn_ptr, sizeof(nv_item_type),
                           nv_item_type*);
    if( dcc_nv_mdn_ptr == NULL )
    {
      errval = QMI_ERR_NO_MEMORY_V01;
      goto send_result;
    }

    /* setup the MDN NV item (fills with 0 char) once here before loop */
    memset(dcc_nv_mdn_ptr, 0, sizeof(nv_item_type));
    memscpy(&dcc_nv_item_ptr->dir_number.dir_number[0],
            sizeof(dcc_nv_item_ptr->dir_number.dir_number),
            "0000000000", 
            sizeof(dcc_nv_item_ptr->dir_number.dir_number));

    /* clear all NAM's of info */
    for( i=0; i < NV_MAX_NAMS; i++ ) 
    {
      /* Set the IMSI Components to 0 */
      if ( qmi_dmsi_reset_imsi_components(i) == FALSE )
      {
        errval = QMI_ERR_INTERNAL_V01;
        LOG_MSG_ERROR_INT_1("Could not reset IMSI components on NAM: d", i);
        PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_mdn_ptr);
        goto send_result;
      }
  
#ifdef FEATURE_IS95B_TRUE_IMSI
      /* Set the IMSI_T Components to 0 */
      if ( qmi_dmsi_reset_imsi_t_components(i) == FALSE )
      {
        errval = QMI_ERR_INTERNAL_V01;
        LOG_MSG_ERROR_INT_1("Could not reset IMSI_T components on NAM d", i);
        PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_mdn_ptr);
        goto send_result;
      }
#endif /* FEATURE_IS95B_TRUE_IMSI */

      /* Set MDN = "0000000000" */
      dcc_nv_mdn_ptr->dir_number.nam = i;
      if ( NV_DONE_S != dcc_put_nv_item_ext( NV_DIR_NUMBER_I, dcc_nv_mdn_ptr, sub_id ) )
      {
        errval = QMI_ERR_INTERNAL_V01;
        PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_mdn_ptr);
        goto send_result;
      }

      /* Set SSD-A to 0 */
      memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
      dcc_nv_item_ptr->ssd_a.nam = i;
      if ( NV_DONE_S != dcc_put_nv_item_ext( NV_SSD_A_I, dcc_nv_item_ptr, sub_id ) )
      {
        errval = QMI_ERR_INTERNAL_V01;
        PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_mdn_ptr);
        goto send_result;
      }

      /* Set SSD-B to 0 */
      memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
      dcc_nv_item_ptr->ssd_b.nam = i;
      if ( NV_DONE_S != dcc_put_nv_item_ext( NV_SSD_B_I, dcc_nv_item_ptr, sub_id ) )
      {
        errval = QMI_ERR_INTERNAL_V01;
        PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_mdn_ptr);
        goto send_result;
      }
    }

    /*-------------------------------------------------------------------------
      Free the temporary memory allocated for NV item.
    -------------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_mdn_ptr);
  }

#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE) || defined(FEATURE_TDSCDMA)
  /*-------------------------------------------------------------------------
    Reset items for UMTS builds.
   -------------------------------------------------------------------------*/

#endif /* defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE) || defined(FEATURE_TDSCDMA) */

send_result: 
  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);

  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_restore_factory_defaults_resp_msg_v01));  
  return QMI_CSI_CB_NO_ERR;

  
} /* qmi_dmsi_restore_factory_defaults() */

/*===========================================================================
  FUNCTION QMI_DMSI_VALIDATE_SPC()

  DESCRIPTION
    Validates a provided security code.
        
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_validate_spc
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_validate_service_programming_code_req_msg_v01* req_msg;
  dms_validate_service_programming_code_resp_msg_v01 resp_msg;
  qmi_if_spc_result_e_type	check_spc_result;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(req_c_struct != NULL);
  errval = QMI_ERR_NONE_V01;
  memset(&resp_msg, 0, sizeof(dms_validate_service_programming_code_resp_msg_v01));
  
  /* Check the SPC */
  req_msg = (dms_validate_service_programming_code_req_msg_v01*)req_c_struct;
  check_spc_result = qmi_if_check_spc((uint8 *)req_msg->spc, TRUE); 

  if(QMI_SPC_FAIL == check_spc_result)
  {
    errval = QMI_ERR_AUTHENTICATION_FAILED_V01;
  }
  else if (QMI_SPC_LOCKED == check_spc_result)
  {
    errval = QMI_ERR_AUTHENTICATION_LOCK_V01;
  }
  else if(QMI_SPC_ERR_INTERNAL == check_spc_result)
  {
    errval = QMI_ERR_INTERNAL_V01;
  }

  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_validate_service_programming_code_resp_msg_v01));  
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_validate_spc() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_TIME()

  DESCRIPTION
    Gives the current time from the device
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_time
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_time_resp_msg_v01 resp_msg;
  uint64 time_count = 0;
  time_genoff_args_struct_type genoff_args;
  time_type                    ts_val;
  time_type                    ts_val_usr;
  time_type                    ts_val_sys;
  time_source_enum_type        ts_source;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&resp_msg, 0, sizeof(dms_get_time_resp_msg_v01));
  memset(&genoff_args, 0, sizeof (genoff_args));
  memset(ts_val, 0, sizeof(ts_val));
  memset(ts_val_usr, 0, sizeof(ts_val_usr));
  memset(ts_val_sys, 0, sizeof(ts_val_sys));
  
  /*-------------------------------------------------------------------------
    Query the current user time
  -------------------------------------------------------------------------*/

  genoff_args.base        = ATS_USER; 
  genoff_args.base_source = TIME_SCLK;
  genoff_args.ts_val      = ts_val_usr;
  genoff_args.unit        = TIME_MSEC;
  genoff_args.operation   = T_GET;
  time_genoff_opr( &genoff_args );

  resp_msg.user_time_in_ms_valid = TRUE;
  memscpy(&resp_msg.user_time_in_ms,sizeof(resp_msg.user_time_in_ms),&ts_val_usr,sizeof(time_type));

  memset(&genoff_args, 0, sizeof (genoff_args));

  /*-------------------------------------------------------------------------
    Query the current sys time
  -------------------------------------------------------------------------*/

  genoff_args.base        = ATS_TOD; 
  genoff_args.base_source = TIME_SCLK;
  genoff_args.ts_val      = ts_val_sys;
  genoff_args.unit        = TIME_MSEC;
  genoff_args.operation   = T_GET;
  time_genoff_opr( &genoff_args );

  resp_msg.sys_time_in_ms_valid = TRUE;
  memscpy(&resp_msg.sys_time_in_ms,sizeof(resp_msg.sys_time_in_ms),&ts_val_sys,sizeof(ts_val_sys));
  /*-------------------------------------------------------------------------
    Query the current timestamp
  -------------------------------------------------------------------------*/
  /*
                             TIMESTAMP FORMAT

       |<------------ 48 bits --------------->|<----- 16 bits ------->|
       +--------------------------------------+-----------------------+
       |      1.25 ms counter                 |   1/32 chip counter   |
       +--------------------------------------+-----------------------+
             (11K years of dynamic range)          (1.25 ms dynamic
                                                    range. Rolls over
                                                    at count 49152)
  */
  ts_source = time_get(ts_val);

  /*-------------------------------------------------------------------------
    Populate the timestamp response
  -------------------------------------------------------------------------*/
  /* copy the timestamp to native C */
  time_count = QW_CVT_Q2N(ts_val);

  /* shift the timestamp to conform with the output alignment */
  time_count = time_count >> 16;

  memscpy(resp_msg.device_time.time_count,sizeof(resp_msg.device_time.time_count),
          &time_count,sizeof(time_count));  

  resp_msg.device_time.time_source = (dms_time_source_enum_v01)ts_source;
  
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_time_resp_msg_v01));  

  LOG_MSG_INFO2_4("Dms query User Time: ts_val_usr[0]=%d : ts_val_usr[1]=%d,"
                  "Dms query Sys Time: ts_val_sys[0]=%d : ts_val_sys[1]=%d",
                  ts_val_usr[0], ts_val_usr[1],ts_val_sys[0], ts_val_sys[1]);

  return QMI_CSI_CB_NO_ERR;

  
} /* qmi_dmsi_get_time() */

/*===========================================================================
  FUNCTION QMI_DMSI_SET_TIME()

  DESCRIPTION
    Sets the current time to the device

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_dmsi_set_time
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  time_genoff_args_struct_type genoff_args;
  dms_set_time_req_msg_v01* req_msg = NULL;
  dms_set_time_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(req_c_struct != NULL);
  req_msg = (dms_set_time_req_msg_v01*)req_c_struct;
  memset(&resp_msg,0,sizeof(dms_set_time_resp_msg_v01));
  memset(&genoff_args,0,sizeof(genoff_args));

  /*-------------------------------------------------------------------------
    We support only User Time Reference Type (0x00000000)
  -------------------------------------------------------------------------*/
  if(req_msg->time_reference_type_valid && 
  	 req_msg->time_reference_type != DMS_TIME_REF_TYPE_USER_V01)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
    goto send_result;
  }
  genoff_args.base        = ATS_USER; 
  genoff_args.base_source = TIME_SCLK;
  genoff_args.ts_val      = (void*)&req_msg->time_in_ms;
  genoff_args.unit        = TIME_MSEC;
  genoff_args.operation   = T_SET;
  time_genoff_opr( &genoff_args );

send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_time_resp_msg_v01)); 
  return QMI_CSI_CB_NO_ERR;


} /* qmi_dmsi_set_time() */

/*===========================================================================
  FUNCTION QMI_DMSI_RESET_CLIENT()

  DESCRIPTION
    Resets the state for the given client
    
  PARAMETERS
    cl_sp_in :  client info handle
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_reset_client
(
  qmi_dmsi_client_info_type * cl_sp_in
)
{
  qmi_dmsi_client_info_type * cl_sp;
  void  * pending_cl_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cl_sp = (qmi_dmsi_client_info_type *)cl_sp_in;
  /*-------------------------------------------------------------------------
    Free resources for client
    Remove any pending commmands for this client from the pending q's
    Clear pending msg transactions and partially bundled responses
  -------------------------------------------------------------------------*/
  /* (void) q_linear_delete_new(
           &(qmi_dms_state.set_oprt_mode_pending_q),
           (q_compare_func_type) qmi_svc_cmd_buf_belongs_to_client,
           cl_sp,
           (q_action_func_type) NULL,
           NULL); */
           
  #ifdef FEATURE_TDSCDMA

  (void) q_linear_delete_new(&qmi_dms_state.get_tdscdma_config_pending_q,
                             (q_compare_func_type) dmsi_cmd_buf_belongs_to_client,
                             cl_sp,
                             (q_action_func_type) NULL,
                             NULL);

  (void) q_linear_delete_new(&qmi_dms_state.set_tdscdma_config_pending_q,
                             (q_compare_func_type) dmsi_cmd_buf_belongs_to_client,
                             cl_sp,
                             (q_action_func_type) NULL,
                             NULL);

  #endif

  do 
  {
    // Delete all pending power stats requests of current client
    pending_cl_sp = q_linear_delete_new(&qmi_dms_state.modem_activity_pend_q,
                                       (q_compare_func_type) qmi_dms_is_client_power_stats_query_pending,
                                       cl_sp,
                                       (q_action_func_type) NULL,
                                       NULL);
  }while(pending_cl_sp != NULL);

  /*-------------------------------------------------------------------------
    If power state reporting is set for this client then
    - reset it
    - decrement the report power state count by 1
    - If this count becomes 0, turn off the timer for power state reporting
  -------------------------------------------------------------------------*/
  if(cl_sp->report_status.report_power_state == TRUE)
  {
    cl_sp->report_status.report_power_state  = FALSE;
    if (qmi_dmsi_global.pwr_rpt.report_power_state_client_count > 0)
    {
      qmi_dmsi_global.pwr_rpt.report_power_state_client_count--;
    }

    if(qmi_dmsi_global.pwr_rpt.report_power_state_client_count == 0)
    {
      qmi_charger_set_power_state_reporting(FALSE);
    }
  }
  /*-------------------------------------------------------------------------
    clear event reporting then set non-zero default values
  -------------------------------------------------------------------------*/
  memset(&(cl_sp->report_status), 0, sizeof(cl_sp->report_status));
  cl_sp->report_status.bat_lvl_lower_limit = DMSI_BAT_LVL_MIN;
  cl_sp->report_status.bat_lvl_upper_limit = DMSI_BAT_LVL_MAX;
  cl_sp->report_status.last_bat_lvl_reported = DMSI_BAT_LVL_INACTIVE;
  cl_sp->report_status.last_power_state_reported = DMSI_PWR_STATE_INACTIVE;

  if (cl_sp_in == qmi_dmsi_global.prl_info.cl_sp)
  {
    /* end the PRL series because the client that was using it is ending */
    qmi_dmsi_reset_prl_series_info();
  }  
} /* qmi_dmsi_reset_client() */



/*===========================================================================
  FUNCTION QMI_DMSI_GET_MAX_CHANNEL_RATE()

  DESCRIPTION
    Get the maximum theoretically supported RX and TX channel rate

  PARAMETERS
    max_rx_channel_rate (output parameter)
    max_tx_channel_rate (output parameter)

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_get_max_channel_rate
(
  uint32 * max_rx_channel_rate,
  uint32 * max_tx_channel_rate
)
{
  qmi_channel_info_s_type supp_channel_info;
  uint16 mode_capability = 0;
  qmi_if_info_type  *qmi_if_info_ptr;
#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE) || defined(FEATURE_TDSCDMA)
  uint32  tx_rate = 0;
  uint32  rx_rate = 0;
#endif /* FEATURE_WCDMA || FEATURE_GSM || FEATURE_LTE || FEATURE_TDSCDMA */
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  uint32  tx_rate_3gpp2 = 0;
  uint32  rx_rate_3gpp2 = 0;
#endif /* FEATURE_CDMA_800 || FEATURE_CDMA_1900 */
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT (max_rx_channel_rate && max_tx_channel_rate);

  *max_tx_channel_rate = 0;
  *max_rx_channel_rate = 0;

  memset(&supp_channel_info, 0, sizeof(supp_channel_info));

  qmi_if_info_ptr = qmi_if_info_get_ptr();

  mode_capability = (uint16)qmi_if_info_ptr->ph_info.mode_capability;


  /*-------------------------------------------------------------------------
    Determine max supported channel rate based on software feature support
  -------------------------------------------------------------------------*/
#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE) || defined(FEATURE_TDSCDMA)
  if (qmi_dmsi_global.nv_use_max_defined_data_rates)
  {
    if(DMSI_TARGET_SUPPORTS_LTE(mode_capability))
    {
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_LTE;
    }
    else if(DMSI_TARGET_SUPPORTS_WCDMA(mode_capability))
    {  
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_WCDMA;
    }
    else if(DMSI_TARGET_SUPPORTS_TDSCDMA(mode_capability))
    {  
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_TDSCDMA;
    }
    else if(DMSI_TARGET_SUPPORTS_GSM(mode_capability))
    {
      #if defined(FEATURE_GSM_EGPRS)
        supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_EDGE;
        supp_channel_info.extend.gprs_edge.multislot_class =
                                            QMI_DEFAULT_GPRS_EDGE_MULTISLOT_CLASS;
      #else
        supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_GPRS;
        supp_channel_info.extend.gprs_edge.multislot_class =
                                            QMI_DEFAULT_GPRS_EDGE_MULTISLOT_CLASS;
      #endif
    }
    /* else supp_channel_info.supp_channel stays at QMI_CHANNEL_SUPP_MIN in
       which case qmi_svc_ext_get_default_channel_rates will return rates as 0 */
  } 
  else 
  {
    if(DMSI_TARGET_SUPPORTS_LTE(mode_capability))
    {   
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_LTE;
    }
    else if(DMSI_TARGET_SUPPORTS_WCDMA(mode_capability))
    {  
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_WCDMA;
    }
    else if(DMSI_TARGET_SUPPORTS_TDSCDMA(mode_capability))
    {  
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_TDSCDMA;
    }
    else if(DMSI_TARGET_SUPPORTS_GSM(mode_capability))
    {
      #if defined(FEATURE_GSM_EGPRS)
        supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_EDGE;
        supp_channel_info.extend.gprs_edge.multislot_class =
                                         qmi_if_info_ptr->gw_info.edge_multislot_class;
      #else
        supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_GPRS;
        supp_channel_info.extend.gprs_edge.multislot_class =
                                         qmi_if_info_ptr->gw_info.gprs_multislot_class;
      #endif
    }
    /* else supp_channel_info.supp_channel stays at QMI_CHANNEL_SUPP_MIN in
       which case qmi_svc_ext_get_default_channel_rates will return rates as 0 */
  }
  qmi_svc_ext_get_default_channel_rates( supp_channel_info,
                                         &tx_rate,
                                         &rx_rate );
#endif /* FEATURE_WCDMA || FEATURE_GSM || FEATURE_LTE || FEATURE_TDSCDMA*/

#if defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)
  if(DMSI_TARGET_SUPPORTS_HDR(mode_capability))
  {  
    #ifdef FEATURE_HDR_REVA_L1
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_HDR_REVA;
    #else /* FEATURE_HDR_REVA_L1 */
      supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_HDR_REV0;
    #endif /* FEATURE_HDR_REVA_L1 */
  }
  else if(DMSI_TARGET_SUPPORTS_CDMA(mode_capability))
  {  
    supp_channel_info.supp_channel = QMI_CHANNEL_SUPP_CDMA;
  }
  /* else supp_channel_info.supp_channel stays at QMI_CHANNEL_SUPP_MIN in
     which case qmi_svc_ext_get_default_channel_rates will return rates as 0 */
  qmi_svc_ext_get_default_channel_rates( supp_channel_info,
                                         &tx_rate_3gpp2,
                                         &rx_rate_3gpp2 );
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

#if (( defined(FEATURE_LTE) || defined(FEATURE_WCDMA) || defined(FEATURE_GSM)) || defined(FEATURE_TDSCDMA)) && \
    (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900))
    *max_tx_channel_rate = MAX(tx_rate, tx_rate_3gpp2);
    *max_rx_channel_rate = MAX(rx_rate, rx_rate_3gpp2);
#elif defined(FEATURE_WCDMA) || defined(FEATURE_GSM)|| defined(FEATURE_LTE) || defined(FEATURE_TDSCDMA)
    *max_tx_channel_rate = tx_rate;
    *max_rx_channel_rate = rx_rate;
#elif defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
    *max_tx_channel_rate = tx_rate_3gpp2;
    *max_rx_channel_rate = rx_rate_3gpp2;
#endif /* defined(FEATURE_LTE) || (defined(FEATURE_WCDMA) || defined(FEATURE_GSM)) || defined(FEATURE_TDSCDMA) &&
          (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)) */

  return errval;
} /* qmi_dmsi_get_max_channel_rate */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_SIM_CAPABILITY()

  DESCRIPTION
    to get the SIM capability of the mobile
    
  PARAMETERS
    None

  RETURN VALUE
    sim_capability
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint8 qmi_dmsi_get_sim_capability
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  #if defined(FEATURE_MMGSDI_GSM) || defined(FEATURE_MMGSDI_UMTS) || defined(FEATURE_MMGSDI_CDMA)
    return(DMS_SIM_SUPPORTED_V01);           
  #else 
    return(DMS_SIM_NOT_SUPPORTED_V01);
  #endif

} /* qmi_dmsi_get_sim_capability */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_HW_SUPPORTED_RADIO_IF_LIST()

  DESCRIPTION
    to get the list of supported radio technologies by the hardware
    
  PARAMETERS
    radio_if_list

  RETURN VALUE
    Number of supported radio_ifs
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint8 qmi_dmsi_get_hw_supported_radio_if_list
(
  uint32 *  radio_if_list
)
{
  uint8 index;
  qmi_if_info_type  *qmi_if_info_ptr;
  uint16 mode_capability = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  index = 0;   

  qmi_if_info_ptr = qmi_if_info_get_ptr();

  mode_capability = (uint16)qmi_if_info_ptr->ph_info.mode_capability;

  if(DMSI_TARGET_SUPPORTS_CDMA(mode_capability))
  {
      radio_if_list[index++] = DMS_RADIO_IF_1X_V01;
  }
   
  if(DMSI_TARGET_SUPPORTS_HDR(mode_capability))
  {  
    radio_if_list[index++] = DMS_RADIO_IF_1X_EVDO_V01;
  }
      
  if(DMSI_TARGET_SUPPORTS_GSM(mode_capability))
  {  
    radio_if_list[index++] = DMS_RADIO_IF_GSM_V01;
  }

  if(DMSI_TARGET_SUPPORTS_WCDMA(mode_capability))
  {  
    radio_if_list[index++] = DMS_RADIO_IF_UMTS_V01;
  }

  if(DMSI_TARGET_SUPPORTS_LTE(mode_capability))
  {  
    radio_if_list[index++] = DMS_RADIO_IF_LTE_V01;
  }

  if(DMSI_TARGET_SUPPORTS_TDSCDMA(mode_capability))
  {
    radio_if_list[index++] = DMS_RADIO_IF_TDS_V01;
  }
  return index;
                
} /* qmi_dmsi_get_hw_supported_radio_if_list */



/*===========================================================================
  FUNCTION QMI_DMSI_GET_VOICE_CAPABILITY()

  DESCRIPTION
    to get the voice capability of the mobile
    
  PARAMETERS
    None
    
  RETURN VALUE
    voice_capability
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint8 qmi_dmsi_get_voice_capability
(
  void
)
{

  uint32 sv_mask = 0;
  uint8 ret_val = DMS_DATA_CAP_NONE_V01;
  trm_get_info_return_type return_info;
  trm_get_info_input_type  input_info;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&return_info, 0x0, sizeof(trm_get_info_return_type));
  memset(&input_info, 0x0, sizeof(trm_get_info_input_type));

#ifndef TEST_FRAMEWORK
  input_info.client_id = TRM_CM; /* For techs which don't have a client; use CM */
#endif
  input_info.info_type = TRM_SIMUL_CAPABILITY;
  
  trm_get_info(&input_info, &return_info);

  DS_QMI_DMS_ASSERT(return_info.info_type == TRM_SIMUL_CAPABILITY);
  sv_mask = (uint32)return_info.info.simul_cap_info;

  if( (sv_mask & TRM_SVDO_IS_ENABLED) ||
      (sv_mask & TRM_SVLTE_IS_ENABLED) )
  {
    ret_val = DMS_DEVICE_CAP_SIMUL_VOICE_AND_DATA_V01;
  }
  else 
  {
    if( (MCS_MODEM_CAP_AVAILABLE == mcs_modem_has_capability(MCS_MODEM_CAPABILITY_FEATURE_GSM ))||
        (MCS_MODEM_CAP_AVAILABLE == mcs_modem_has_capability(MCS_MODEM_CAPABILITY_FEATURE_1X)) )
    {
      ret_val = DMS_DEVICE_CAP_VOICE_ONLY_V01;
    }

    if ((MCS_MODEM_CAP_AVAILABLE == mcs_modem_has_capability(MCS_MODEM_CAPABILITY_FEATURE_HSPA ))||
       (MCS_MODEM_CAP_AVAILABLE == mcs_modem_has_capability( MCS_MODEM_CAPABILITY_FEATURE_WCDMA))||
       (MCS_MODEM_CAP_AVAILABLE == mcs_modem_has_capability( MCS_MODEM_CAPABILITY_FEATURE_TDSCDMA)))
    {
      ret_val = DMS_DEVICE_CAP_NONSIMUL_VOICE_AND_DATA_V01;
    }

    if ( (MCS_MODEM_CAP_AVAILABLE == mcs_modem_has_capability(MCS_MODEM_CAPABILITY_FEATURE_LTE )) ||
         (MCS_MODEM_CAP_AVAILABLE == mcs_modem_has_capability(MCS_MODEM_CAPABILITY_FEATURE_DO )) )
    {
      if((DMS_DEVICE_CAP_VOICE_ONLY_V01 == ret_val ) ||
         (DMS_DEVICE_CAP_NONSIMUL_VOICE_AND_DATA_V01 == ret_val ))
      {
        ret_val = DMS_DEVICE_CAP_NONSIMUL_VOICE_AND_DATA_V01;
      }
      else
      {
        ret_val = DMS_DEVICE_CAP_DATA_ONLY_V01;
      }
    }

  }

  if ( DMS_DATA_CAP_NONE_V01 == ret_val )
  {
#if defined(FEATURE_IS2000_REL_A_SVD) || defined(FEATURE_UMTS_CONC_SRVC)
    ret_val= DMS_DEVICE_CAP_SIMUL_VOICE_AND_DATA_V01;
#elif defined(FEATURE_DATA)
    ret_val= DMS_DEVICE_CAP_NONSIMUL_VOICE_AND_DATA_V01;
#else /* FEATURE_DATA */
    ret_val= DMS_DEVICE_CAP_VOICE_ONLY_V01;
#endif
  }

  return ret_val;
} /* qmi_dmsi_get_voice_capability */


/*===========================================================================
  FUNCTION QMI_DMSI_GET_VOICE_SUPPORT_CAPABILITY()

  DESCRIPTION
    to get the voice support capability of the mobile,
    used to determine the device capability of fallback for voice    
  PARAMETERS
    None
    
  RETURN VALUE
    voice_support_capability mask identifying 3gpp CSFB and 1x CSFB support
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint64 qmi_dmsi_get_voice_support_capability
(
  void
)
{
  uint64 voice_support_capability = 0;

  #if defined(FEATURE_3GPP_CSFB)
    voice_support_capability |= QMI_DMS_MASK_VOICE_SUPPORT_GW_CSFB_CAPABLE_V01;
  #endif

  #if defined(FEATURE_LTE) && defined(FEATURE_LTE_TO_1X)
    voice_support_capability |= QMI_DMS_MASK_VOICE_SUPPORT_1x_CSFB_CAPABLE_V01;
  #endif

  return voice_support_capability;
} /* qmi_dmsi_get_voice_support_capability */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_SIMUL_VOICE_AND_DATA_CAPABILITY()

  DESCRIPTION
    Returns the Simultaneous Voice and Data Capability of the device
 
  PARAMETERS
    None
    
  RETURN VALUE
    simul_voice_and_data_capability mask identifying SVLTE and SVDO support
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint64 qmi_dmsi_get_simul_voice_and_data_capability
(
  void
)
{
  uint32 sv_mask = 0;
  uint64 simul_voice_and_data_capability = 0;
  sys_overall_feature_t dev_overall_cap;
  trm_get_info_return_type return_info;
  trm_get_info_input_type  input_info;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    memset(&return_info, 0x0, sizeof(trm_get_info_return_type));
   memset(&input_info, 0x0, sizeof(trm_get_info_input_type));
#ifndef TEST_FRAMEWORK  
   input_info.client_id = TRM_CM; /* For techs which don't have a client; use CM */
#endif
   input_info.info_type = TRM_SIMUL_CAPABILITY;
    
   trm_get_info(&input_info, &return_info);
  
   DS_QMI_DMS_ASSERT(return_info.info_type == TRM_SIMUL_CAPABILITY);
   sv_mask = (uint32)return_info.info.simul_cap_info;

  if( sv_mask & TRM_SVLTE_IS_ENABLED)
  {  
    simul_voice_and_data_capability |= QMI_DMS_MASK_SVLTE_CAPABLE_V01;
  }
  if( sv_mask & TRM_SVDO_IS_ENABLED)
  {  
    simul_voice_and_data_capability |= QMI_DMS_MASK_SVDO_CAPABLE_V01;
  }
  if(QMI_ERR_NONE_V01 == qmi_dmsi_get_device_overall_capability(&dev_overall_cap))
  {
    if(dev_overall_cap == SYS_OVERALL_FEATURE_MODE_SGLTE)
    {
      simul_voice_and_data_capability |= QMI_DMS_MASK_SGLTE_CAPABLE_V01;
    }
  }
  return simul_voice_and_data_capability;
} /* qmi_dmsi_get_simul_voice_and_data_capability */


/*===========================================================================
  FUNCTION QMI_DMSI_SET_BATTERY_LEVEL_LIMITS()

  DESCRIPTION
    It is used to determine the global high and low battery level limits to
    be set in the charge module.
    
  PARAMETERS
    curr_bat_lvl : The current battery level
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    none

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_set_battery_level_limits
(
  uint8 curr_bat_lvl
)
{
  qmi_dmsi_client_info_type *  cl_sp;
  uint8                        low_limit, high_limit;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);
  low_limit = DMSI_BAT_LVL_MIN;
  high_limit = DMSI_BAT_LVL_MAX;
  
  /*-------------------------------------------------------------------------
    traverse through all the DMS clients to determine the new high and low
    battery limits - the new high and low limts for the charger are the 
    client's upper and lower limts which are closest to the current battery
    level

    Note - It is possible that after looping through the high and low limits 
    to be set in the charger remain unchanged and it will result in again 
    setting the existing charger limits, but that is alright
  -------------------------------------------------------------------------*/
    while( cl_sp != NULL )
    {
      if(  cl_sp->report_status.report_power_state )
      {
        /*-------------------------------------------------------------------
          The new high limit for the charger is the client's upper or lower
          limit closest to the current battery level on the higher side
        -------------------------------------------------------------------*/
        if(cl_sp->report_status.bat_lvl_upper_limit >= curr_bat_lvl)
        {
          high_limit = MIN(cl_sp->report_status.bat_lvl_upper_limit,
                           high_limit);
        }
        if(cl_sp->report_status.bat_lvl_lower_limit >= curr_bat_lvl)
        {
          high_limit = MIN(cl_sp->report_status.bat_lvl_lower_limit,
                           high_limit);
        }

        /*-------------------------------------------------------------------
          The new low limit for the charger is the client's upper or lower
          limit closest to the current battery level on the lower side
        -------------------------------------------------------------------*/
        if(cl_sp->report_status.bat_lvl_lower_limit <= curr_bat_lvl)
        {
          low_limit = MAX(cl_sp->report_status.bat_lvl_lower_limit,
                          low_limit);
        }
        if(cl_sp->report_status.bat_lvl_upper_limit <= curr_bat_lvl)
        {
          low_limit = MAX(cl_sp->report_status.bat_lvl_upper_limit,
                          low_limit);
        }
      }
      cl_sp = q_next(&qmi_dms_state.client_q,
                           &(cl_sp->link));
    }


  /*-------------------------------------------------------------------------
    Set the high and low limits in the charger to the computed global high
    and low limts
  -------------------------------------------------------------------------*/
  qmi_charger_set_battery_level_limits(low_limit, high_limit);

  qmi_dmsi_global.pwr_rpt.low_limit = low_limit;
  qmi_dmsi_global.pwr_rpt.high_limit = high_limit;
} /* dmsi_set_battery_level_limits() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_MULTISIM_INFO()

  DESCRIPTION
    Get the multisim capabilities of the device from policyman and populate the response

  PARAMETERS
    response (input parameter)
    response (output parameter)
    cm_pm_device_config :  policyman device config pointer

  RETURN VALUE
    errval

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_get_multisim_info
(
  dms_multisim_capability_type_v01* response,
  policyman_item_t const *cm_pm_device_config
)
{  
  policyman_status_t         status;
  policyman_item_t const     *cm_policyman_device_config = cm_pm_device_config;
  uint32                     max_subs;
  uint32                     config_list_len;
  uint32                     max_active;
  uint32                     rat; 
  uint64                     rat_mask;
  int32                      cfg_index;
  int32                      subs_index;
  qmi_error_type_v01         errval = QMI_ERR_NONE_V01;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(response,0x00,sizeof(dms_multisim_capability_type_v01));

  if (!cm_policyman_device_config)
  {
    errval = QMI_ERR_INTERNAL_V01;
    return errval;
  }

  status = 
    policyman_device_config_num_sims(cm_policyman_device_config,(size_t*)&max_subs);
  response->max_subscriptions = (uint8)max_subs;

  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    return errval;
  }

  status = policyman_device_config_max_active(cm_policyman_device_config, (size_t*)&max_active);
 
  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    return errval;
  }
      
  status = policyman_device_config_num_cfgs(cm_policyman_device_config, (size_t*)&config_list_len);

  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    return errval;
  }

  if( (max_subs > QMI_DMS_MAX_SUBSCRIPTION_LIST_LEN_V01) || 
      ( config_list_len > QMI_DMS_MAX_CONFIG_LIST_LEN_V01 ) ||
      (max_active > max_subs) )
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  
  LOG_MSG_INFO1_3("num_sims %d max_active %d num_cfg %d",
                  max_subs, max_active, config_list_len);   
  response->subscription_config_list_len = config_list_len;
  cfg_index  = response->subscription_config_list_len;
  subs_index = response->max_subscriptions;
  for (cfg_index -= 1 ; cfg_index >= 0 ; cfg_index--)
  {
    response->subscription_config_list[cfg_index].subscription_list_len = subs_index;
	LOG_MSG_INFO1_2("subs config list len for config_index %d %d",subs_index,cfg_index);
    for (subs_index -= 1 ; subs_index >= 0 ; subs_index--)
    {
      status = policyman_device_config_get_config(cm_policyman_device_config, cfg_index, subs_index, &rat);
      if (POLICYMAN_FAILED(status))
      {
        errval = QMI_ERR_INTERNAL_V01;
        goto send_result;
      }
      rat_mask = qmi_dmsi_convert_rat_mask(rat);
      response->subscription_config_list[cfg_index].subscription_list[subs_index] = rat_mask;
      LOG_MSG_INFO3_2("ratmask for subs_index %d %d",rat_mask,subs_index);  
    }
    response->subscription_config_list[cfg_index].max_active = max_active;
    subs_index = response->max_subscriptions;
  }

  send_result:
  return errval;
}

/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_CONFIG_LIST()

  DESCRIPTION
    Get the Device configuration list from policyman and populate the response

  PARAMETERS
    response (input parameter)
    cm_pm_device_config :  policyman device config pointer

  RETURN VALUE
    errval

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_get_device_config_list
(
  dms_device_config_capability_type_v01* tlv_cfg_list,
  int16_t* tlv_explicit_index,
  policyman_item_t const *cm_pm_device_config
)
{  
  policyman_status_t         status;
  policyman_item_t const     *cm_policyman_device_config = cm_pm_device_config;
  int32                      cfg_index, subs_index;
  int                        explicit_index;
  size_t                     max_subs, config_list_len, max_active, curr_index;
  uint32                     rat;
  uint64                     rat_mask;
  qmi_error_type_v01         errval = QMI_ERR_NONE_V01;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(tlv_cfg_list == NULL || tlv_explicit_index == NULL)
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  memset(tlv_cfg_list,0x00,sizeof(dms_device_config_capability_type_v01));

  if (!cm_policyman_device_config)
  {
    errval = QMI_ERR_INTERNAL_V01;
    return errval;
  }

  status = policyman_device_config_num_sims(cm_policyman_device_config,
                                            &max_subs);
  if (POLICYMAN_FAILED(status) || (max_subs > QMI_DMS_POLICYMAN_MAX_SUBS))
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  /*Fill the TLV info*/
  tlv_cfg_list->max_subscriptions = (uint8)max_subs;

  status = policyman_device_config_max_active_subs(cm_policyman_device_config,
                                                   &max_active);
  if (POLICYMAN_FAILED(status) || (max_active > QMI_DMS_POLICYMAN_MAX_SUBS) ||
      (max_active > max_subs))
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  /*Fill the TLV info*/
  tlv_cfg_list->max_active = (uint8)max_active;
  
  status = policyman_device_config_num_cfgs(cm_policyman_device_config,
                                            &config_list_len);
  if ( POLICYMAN_FAILED(status) || (config_list_len > QMI_DMS_MAX_CONFIG_LIST_LEN_V01) )
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  LOG_MSG_INFO1_3("num_sims %d max_active %d num_cfg %d",
                  max_subs, max_active, config_list_len);
  /*Fill the TLV info*/ 
  tlv_cfg_list->device_cfg_list_len = config_list_len;
  
  cfg_index  = tlv_cfg_list->device_cfg_list_len;
  subs_index = tlv_cfg_list->max_subscriptions;
  for (cfg_index -= 1 ; cfg_index >= 0 ; cfg_index--)
  {
    /*Fill the TLV info : Each entry of the list is configurations of all subs*/
    tlv_cfg_list->device_cfg_list[cfg_index].subs_cfg_list_len = subs_index;
    for (subs_index -= 1 ; subs_index >= 0 ; subs_index--)
    {
      status = policyman_device_config_get_config(cm_policyman_device_config,
                                                  cfg_index, subs_index, &rat);
      if (POLICYMAN_FAILED(status))
      {
        errval = QMI_ERR_INTERNAL_V01;
        goto send_result;
      }
      rat_mask = qmi_dmsi_convert_rat_mask(rat);
      /*Fill the TLV info*/ 
      tlv_cfg_list->device_cfg_list[cfg_index].subs_cfg_list[subs_index] = 
                                (dms_subs_capability_mask_type_v01)rat_mask;
    }
    subs_index = tlv_cfg_list->max_subscriptions;
  }

  status = policyman_device_config_get_current_config_index(cm_policyman_device_config,
                                                            &curr_index);
  /* Index should point within array indices starting from 0*/
  if (POLICYMAN_FAILED(status) || (curr_index >= config_list_len))
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  /*Fill the TLV info*/ 
  tlv_cfg_list->current_config_index = (uint8)curr_index;

  status = policyman_device_config_get_explicit_index(&explicit_index);
  /*explicit_index:-1 is valid if it is modem set config else 
    index value should point within array indices starting from 0.
    So, 0,-1 are valid & Max index should be within cfg_list length */
  if (POLICYMAN_FAILED(status) || 
       ((explicit_index > 0) && (explicit_index >= (int16_t)config_list_len)) )
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  /*Fill the TLV info*/
  *tlv_explicit_index = (int16_t)explicit_index;

send_result:
  return errval;
}/*qmi_dmsi_get_device_config_list()*/

/*===========================================================================
  FUNCTION QMI_DMSI_GET_MAX_SUBS_STATIC_DEVICE_CAP()

  DESCRIPTION
    Get the device static maximum rat capabilities from policyman and 
    populate the response

  PARAMETERS
    dms pointer to the response (input parameter)

  RETURN VALUE
    errval (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01  qmi_dmsi_get_max_subs_static_device_cap
(
  dms_get_device_cap_resp_msg_v01* response
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  uint32   max_subs,subs_index,max_rat_cap;
  policyman_status_t      status;
  policyman_item_t const  *cm_policyman_device_config = NULL;
  policyman_item_id_t ids[] = { POLICYMAN_ITEM_DEVICE_CONFIGURATION };
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  max_rat_cap = 0;
  /*Get the policyman items intially*/
  status = policyman_get_items_block(
                &ids[0],
                sizeof(ids)/sizeof(policyman_item_id_t),
                &cm_policyman_device_config);
  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  status = policyman_device_config_num_sims(cm_policyman_device_config,
                                             (size_t*)&max_subs);
  if (POLICYMAN_FAILED(status) || (max_subs > QMI_DMS_POLICYMAN_MAX_SUBS))
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  response->device_max_subscription_static_capability_len = (uint8)max_subs;

  /*get the max static rat capabiltiy of each subs*/
  for( subs_index = (uint32)SYS_MODEM_AS_ID_1;
       (subs_index < max_subs) && (subs_index <= SYS_MODEM_AS_ID_3);
        subs_index++ )
  {
    status = policyman_device_config_get_max_capability(cm_policyman_device_config,
                                                        subs_index,
                                                        &max_rat_cap);
    if ( POLICYMAN_FAILED(status) )
    {
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
    LOG_MSG_INFO1_2("max_subs_static_cap: ratmask 0x%x for subs_index %d",
                    max_rat_cap,subs_index);
    response->device_max_subscription_static_capability[subs_index] = 
                                            qmi_dmsi_convert_rat_mask(max_rat_cap);
  }

  send_result:
    if(NULL != cm_policyman_device_config)
    {
      policyman_item_release(cm_policyman_device_config);
    }
    return errval;

}/*qmi_dmsi_get_max_subs_static_device_cap()*/

/*===========================================================================
  FUNCTION DMSI_GET_TDSCDMA_REL_VERSION()

  DESCRIPTION
    Get the device static maximum rat release version information from 
    TDSCDMA RAT and populate the response

  PARAMETERS
    subs_index   Subscription Id for which the release is to be found

  RETURN VALUE
    dms_device_tdscdma_rel_info_v01 (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dms_device_tdscdma_rel_info_v01 dmsi_get_tdscdmsa_rel_version
(
	uint32 subs_index
)
{
#ifdef FEATURE_TDSCDMA
	tdsrrc_rel_version_enum  tdscdma_rel_version ;

	tdscdma_rel_version = tdsrrc_get_3gpp_release_version_ext();
	LOG_MSG_INFO3_1("tdsrrc_get_3gpp_release_version_ext  %d ", tdscdma_rel_version);
	switch(tdscdma_rel_version)
	{
	   case TDSRRC_REL_VERSION_4:
		 return DMS_TDSRRC_REL_VERSION_4_V01;
	   case TDSRRC_REL_VERSION_5:
		 return DMS_TDSRRC_REL_VERSION_5_V01;
	   case TDSRRC_REL_VERSION_6:
		 return DMS_TDSRRC_REL_VERSION_6_V01;
	   case TDSRRC_REL_VERSION_7:
		 return DMS_TDSRRC_REL_VERSION_7_V01;
	   case TDSRRC_REL_VERSION_8:
		 return DMS_TDSRRC_REL_VERSION_8_V01;
	   case TDSRRC_REL_VERSION_9:
		 return DMS_TDSRRC_REL_VERSION_9_V01;
	   default:
	         return DMS_TDSCDMA_REV_UNAVAILABLE_V01;
						
	}

#endif
	return DMS_TDSCDMA_REV_UNAVAILABLE_V01;

}
/*===========================================================================
  FUNCTION DMSI_GET_LTE_REL_VERSION()

  DESCRIPTION
    Get the device static maximum rat release version information from 

    LTE RAT and populate the response

  PARAMETERS
    subs_index   Subscription Id for which the release is to be found

  RETURN VALUE
    dms_device_lte_rat_rel_info_v01 (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

static dms_device_lte_rat_rel_info_v01 dmsi_get_lte_rel_version
(
	uint32 subs_index
)
{

#if defined(FEATURE_LTE)
  lte_3gpp_release_ver_e lte_rel_version;
  lte_rel_version = lte_rrc_get_3gpp_release_version((sys_modem_as_id_e_type)subs_index);
  LOG_MSG_INFO3_1("lte_rrc_get_3gpp_release_version %d ", lte_rel_version);

   switch(lte_rel_version)
   {
   	 case LTE_3GPP_REL8:		
	 	return DMS_LTE_3GPP_REL_8_V01;
	 case LTE_3GPP_REL8_DEC08:
	 	return DMS_LTE_3GPP_REL8_DEC_08_V01;
	 case LTE_3GPP_REL8_MAR09:
	 	return DMS_LTE_3GPP_REL8_MAR_09_V01;
	 case LTE_3GPP_REL8_JUN09:
	 	return DMS_LTE_3GPP_REL8_JUN_09_V01;
	 case LTE_3GPP_REL8_SEP09:
	 	return DMS_LTE_3GPP_REL8_SEP_09_V01;
	 case LTE_3GPP_REL8_DEC09:
	 	return DMS_LTE_3GPP_REL8_DEC_09_V01;
         case LTE_3GPP_REL8_MAR10:		
	 	return DMS_LTE_3GPP_REL8_MAR_10_V01;
	 case LTE_3GPP_REL8_JUN10:
	 	return DMS_LTE_3GPP_REL8_JUN_10_V01;
         case LTE_3GPP_REL8_SEP10:
	 	return DMS_LTE_3GPP_REL8_SEP_10_V01;
	 case LTE_3GPP_REL8_DEC10:		
	 	return DMS_LTE_3GPP_REL8_DEC_10_V01;
			
	 case LTE_3GPP_REL9:
	 	return DMS_LTE_3GPP_REL_9_V01;
         case LTE_3GPP_REL9_MAR10:
 	 	return DMS_LTE_3GPP_REL9_MAR_10_V01;
	 case LTE_3GPP_REL9_JUN10:
	 	return DMS_LTE_3GPP_REL9_JUN_10_V01;
         case LTE_3GPP_REL9_SEP10:
	 	return DMS_LTE_3GPP_REL9_SEP_10_V01;
			
	 case LTE_3GPP_REL10:
	 	return DMS_LTE_3GPP_REL_10_V01;
	 case LTE_3GPP_REL10_SEP11:
	 	return DMS_LTE_3GPP_REL10_SEP_11_V01;
	 case LTE_3GPP_REL10_DEC11:
	 	return DMS_LTE_3GPP_REL10_DEC_11_V01;
	 case LTE_3GPP_REL10_MAR12:
	 	return DMS_LTE_3GPP_REL10_MAR_12_V01;
	 case LTE_3GPP_REL10_JUN12:
	 	return DMS_LTE_3GPP_REL10_JUN_12_V01;
         case LTE_3GPP_REL10_SEP12:
	 	return DMS_LTE_3GPP_REL10_SEP_12_V01;	 
	 case LTE_3GPP_REL10_DEC12:
	 	return DMS_LTE_3GPP_REL10_DEC_12_V01;
	 case LTE_3GPP_REL10_MAR13:
	 	return DMS_LTE_3GPP_REL10_MAR_13_V01;
	 case LTE_3GPP_REL10_JUN13:
                return DMS_LTE_3GPP_REL10_JUN_13_V01;

	case LTE_3GPP_REL11:
		return DMS_LTE_3GPP_REL_11_V01;
	case LTE_3GPP_REL11_JUN13:
		return DMS_LTE_3GPP_REL11_JUN_13_V01;
	case LTE_3GPP_REL11_MAR14:
		return DMS_LTE_3GPP_REL11_MAR_14_V01;

	case LTE_3GPP_REL12:
		return DMS_LTE_3GPP_REL_12_V01;
#ifdef FEATURE_QMI_DSD_CR2319539
	case LTE_3GPP_REL13:
		return DMS_LTE_3GPP_REL_13_V01;

	case LTE_3GPP_REL14:
		return DMS_LTE_3GPP_REL_14_V01;
#endif
	default:
		return DMS_LTE_REV_UNAVAILABLE_V01;
 
  }
#endif
  return DMS_LTE_REV_UNAVAILABLE_V01;
}

/*===========================================================================
  FUNCTION DMSI_GET_WCDMA_REL_VERSION()

  DESCRIPTION
    Get the device static maximum rat release version information from 
    WCDMA RAT and populate the response

  PARAMETERS
    subs_index   Subscription Id for which the release is to be found

  RETURN VALUE
    dms_device_wcdma_rel_info_v01 (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

static dms_device_wcdma_rel_info_v01 dmsi_get_wcdma_rel_version
(
	uint32 subs_index
)
{

#if defined(FEATURE_WCDMA)
     uint8 wcdma_rel_version ;
     wcdma_rel_version = wcdma_rrc_get_3gpp_release_version_sub((sys_modem_as_id_e_type) subs_index);
     LOG_MSG_INFO3_1("wcdma_rrc_get_3gpp_release_version_sub %d ", wcdma_rel_version);
     switch (wcdma_rel_version)
     {
        case WCDMA_RELEASE_99:
            return DMS_WCDMA_REL_99_V01;
        case WCDMA_RELEASE_5:			 
            return DMS_WCDMA_REL_5_V01;
        case WCDMA_RELEASE_6:
            return DMS_WCDMA_REL_6_V01;
        case WCDMA_RELEASE_7:
            return DMS_WCDMA_REL_7_V01;
        case WCDMA_RELEASE_8:
            return DMS_WCDMA_REL_8_V01;
        case WCDMA_RELEASE_9:
            return DMS_WCDMA_REL_9_V01;
        case WCDMA_RELEASE_10:
            return DMS_WCDMA_REL_10_V01;
        case WCDMA_RELEASE_11:;
            return DMS_WCDMA_REL_11_V01;
        case WCDMA_RELEASE_12:
            return DMS_WCDMA_REL_12_V01;
	default:
           return DMS_WCDMA_REV_UNAVAILABLE_V01;
				
      }
#endif
  return DMS_WCDMA_REV_UNAVAILABLE_V01;
}

/*===========================================================================
  FUNCTION DMSI_GET_GSM_REL_VERSION()

  DESCRIPTION
    Get the device static maximum rat release version information from 
    GSM RAT and populate the response

  PARAMETERS
    subs_index   Subscription Id for which the release is to be found

  RETURN VALUE
    dms_device_gsm_rel_info_v01 (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

static dms_device_gsm_rel_info_v01 dmsi_get_gsm_rel_version
(
	uint32 subs_index
)
{

#if defined(FEATURE_GSM)
   uint8 gsm_rel_version;
   gsm_rel_version = geran_grr_get_3gpp_release_version();
   LOG_MSG_INFO3_1("geran_grr_get_3gpp_release_version %d ", gsm_rel_version);
   if(gsm_rel_version == DMS_GSM_RELEASE_VERSION)
   {
    return DMS_GSM_RELEASE_VERSION_9_V01;
   }
#endif
    return DMS_GSM_REV_UNAVAILABLE_V01;
}

/*===========================================================================
  FUNCTION DMSI_GET_HDR_REL_VERSION()

  DESCRIPTION
    Get the device static maximum rat release version information from 
    HDR RAT and populate the response

  PARAMETERS
    subs_index   Subscription Id for which the release is to be found

  RETURN VALUE
    dms_device_hdr_rat_rel_info_v01 (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

static dms_device_hdr_rat_rel_info_v01 dmsi_get_hdr_rel_version
(
	uint32 subs_index
)
{

#ifdef FEATURE_HDR
     hdrcp_mc_evdo_release_version_enum_type  rel_version ;
     rel_version = hdrcp_mc_get_evdo_release_version((sys_modem_as_id_e_type)subs_index);
	 LOG_MSG_INFO3_1(" hdrcp_mc_get_evdo_release_version %d ",rel_version);
	 switch(rel_version)
	 {
        case EVDO_REL_0:
           return DMS_HDR_EVDO_REL_0_V01;
        case EVDO_REL_A:
           return DMS_HDR_EVDO_REL_A_V01;
        case EVDO_REL_B:
           return DMS_HDR_EVDO_REL_B_V01;
        case EVDO_REL_C:			
           return DMS_HDR_EVDO_REL_C_V01;
        default:
           return DMS_HDR_REV_UNAVAILABLE_V01;
	 }

#endif
  return DMS_HDR_REV_UNAVAILABLE_V01;
}

/*===========================================================================
  FUNCTION DMSI_GET_CDMA_REL_VERSION()

  DESCRIPTION
    Get the device static maximum rat release version information from 
    CDMA RAT and populate the response

  PARAMETERS
    subs_index   Subscription Id for which the release is to be found

  RETURN VALUE
    dms_device_cdma_1x_rel_info_v01 (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS

   None
===========================================================================*/


static dms_device_cdma_1x_rel_info_v01 dmsi_get_cdma_rel_version
(
  uint32 subs_index

)
{     
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
      uint32 rel_version ;
      rel_version = mc_get_cdma_release_revision((sys_modem_as_id_e_type)subs_index);
      LOG_MSG_INFO3_1(" mc_get_cdma_release_revision %d ", rel_version);
   switch(rel_version)
   {
      case (uint32)P_REV_JSTD008:
         return DMS_REV_JSTD008_V01;
      case (uint32)P_REV_IS95A:  	
        return DMS_REV_IS95A_V01;
      case (uint32)P_REV_IS95B:
        return DMS_REV_IS95B_V01;
      case (uint32)P_REV_IS2000:
        return DMS_REV_IS2000_V01;
      case (uint32)P_REV_IS2000_REL_A:  	
         return DMS_REV_IS2000_REL_A_V01;
      case (uint32)P_REV_IS2000_REL_B:
         return DMS_REV_IS2000_REL_B_V01;
      case (uint32)P_REV_IS2000_REL_C_MI:	  	
         return DMS_REV_IS2000_REL_C_MI_V01;
      case (uint32)P_REV_IS2000_REL_D:	
         return DMS_REV_IS2000_REL_D_V01;
      case (uint32)P_REV_IS2000_REL_C:	  	
	    return DMS_REV_IS2000_REL_C_V01;
      default:
          break;
    }
#endif

  return DMS_REV_UNAVAILABLE_V01;
}

/*===========================================================================
  FUNCTION QMI_DMSI_GET_RAT_RELEASE_VERSION_INFO()

  DESCRIPTION
    Get the device static maximum rat release version information from 
    different RAT modules(GERAN,WCDMA,LTE,IX,HDR) and populate the response

  PARAMETERS
    dms pointer to the response (input parameter)

  RETURN VALUE
    errval (output parameter)

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01  qmi_dmsi_get_rat_release_version_info
(
  dms_get_device_cap_resp_msg_v01* response
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  uint32   max_subs,subs_index,rat_mask;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if(response->device_max_subscription_static_capability_len)
  {
      max_subs = response->rat_rel_info_len = (uint8)response->device_max_subscription_static_capability_len;
  }
  else
  {
  	errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
 
  LOG_MSG_INFO3_1(" qmi_dmsi_get_rat_release_version_info max_subs  %d ", max_subs);
  
  /*get the max static rat capability of each subs*/
  for( subs_index = (uint32)SYS_MODEM_AS_ID_1;
       (subs_index < max_subs) && (subs_index < SYS_MODEM_AS_ID_3);
        subs_index++ )
  {
    rat_mask = response->device_max_subscription_static_capability[subs_index];
	
	response->rat_rel_info[subs_index].subscription = (dms_bind_subscription_enum_v01)(subs_index + 1);
	
    if(rat_mask & DMS_SUBS_CAPABILITY_CDMA_V01)
    {
      response->rat_rel_info[subs_index].cdma1x_rel_info = dmsi_get_cdma_rel_version(
	   	                                                    (sys_modem_as_id_e_type)subs_index);
    }
    if(rat_mask & DMS_SUBS_CAPABILITY_HDR_V01)
    {
      response->rat_rel_info[subs_index].hdrcp_rel_info = dmsi_get_hdr_rel_version(
	  	                                                   (sys_modem_as_id_e_type)subs_index);
    }
    if(rat_mask & DMS_SUBS_CAPABILITY_GSM_V01) 
    {
      response->rat_rel_info[subs_index].geran_rel_info = dmsi_get_gsm_rel_version(
	   	                                                  (sys_modem_as_id_e_type)subs_index);
    }    
    if(rat_mask & DMS_SUBS_CAPABILITY_WCDMA_V01)
    {	   
      response->rat_rel_info[subs_index].wcdma_rel_info = dmsi_get_wcdma_rel_version(
	   	                                                   (sys_modem_as_id_e_type) subs_index);
    }
    if(rat_mask & DMS_SUBS_CAPABILITY_LTE_V01)
    {
      response->rat_rel_info[subs_index].lte_rel_info = dmsi_get_lte_rel_version(
	  	                                                (sys_modem_as_id_e_type)subs_index);
    }    		 
    if(rat_mask & DMS_SUBS_CAPABILITY_TDS_V01)
    {
      response->rat_rel_info[subs_index].tdscdma_rel_info = dmsi_get_tdscdmsa_rel_version(
	 	                                                   (sys_modem_as_id_e_type)subs_index);
    }

  }

  send_result:
  return errval;

}/*qmi_dmsi_get_rat_release_version_info()*/

/*===========================================================================
  FUNCTION QMI_DMSI_SET_AP_SW_VERSION()
  
  
  DESCRIPTION
    Sets the AP Software Version
  	  
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None
  
  SIDE EFFECTS
    None
  ===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_set_ap_sw_version
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_set_ap_sw_version_req_msg_v01* req_msg;
  dms_set_ap_sw_version_resp_msg_v01 resp_msg;
  sw_version_s_type  sw_info;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_QMI_DMS_ASSERT(req_c_struct != NULL);
  req_msg = (dms_set_ap_sw_version_req_msg_v01*)req_c_struct;
  memset((void *)&resp_msg, 0, sizeof(dms_set_ap_sw_version_resp_msg_v01));
  memset(&sw_info,0,sizeof(sw_info));

  strlcpy((char*)sw_info.sw_version,req_msg->ap_sw_version,sizeof(sw_info.sw_version));
  sw_info.length = strlen(req_msg->ap_sw_version);
  //Call CM API to Set APSS Sw Version
  cm_set_apss_sw_version (&sw_info);

  resp_msg.resp.error = QMI_ERR_NONE_V01;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_ap_sw_version_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_set_ap_sw_version() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_OVERALL_CAPABILITY()

  DESCRIPTION
    Get the overall capabilities(h/w + configuration) of the device from policyman and populate the response

  PARAMETERS
    device_overall_cap (input parameter)
    errval (output parameter)

  RETURN VALUE
    errval

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_get_device_overall_capability
(
  sys_overall_feature_t* device_overall_cap
)
{ 
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;

  policyman_status_t  status;   
  policyman_item_t const *cm_policyman_device_config = NULL;
  policyman_item_id_t ids[] = { POLICYMAN_ITEM_DEVICE_CONFIGURATION };
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(NULL == device_overall_cap)
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_0("Invalid input parameter ");
    goto send_result;
  }
  
  status = policyman_get_items(
              &ids[0],
              sizeof(ids)/sizeof(policyman_item_id_t),
              &cm_policyman_device_config
              );
  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_0("get_items fail"); 
    goto send_result;

  }
  
  status = 
    policyman_device_config_overall_feature(cm_policyman_device_config,(sys_overall_feature_t*)device_overall_cap);

  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_0("device_config_overall_feature fail");
    goto send_result;
  }

  if((sys_overall_feature_t)*device_overall_cap > SYS_OVERALL_FEATURE_MODE_SGLTE)
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_1("cap is greater: d", *device_overall_cap);
  }

  send_result:
  if(NULL != cm_policyman_device_config)
  {
    policyman_item_release(cm_policyman_device_config);
  }
  return errval;
}


/*===========================================================================
  FUNCTION QMI_DMSI_GET_SUBS_DEVICE_FEATURE_MODE()

  DESCRIPTION
    Get the device static feature mode of the device from policyman and 
    populate the response

  PARAMETERS
    dms pointer to the response (input parameter)
    errval (output parameter)

  RETURN VALUE
    errval

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_get_subs_device_feature_mode
(
  dms_get_device_cap_resp_msg_v01* response
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  uint32   max_subs;
  uint32   subs_index;
  sys_subs_feature_t      subs_device_feature_mode;
  sys_overall_feature_t   device_overall_cap;
  dms_device_subs_feature_mode_enum_v01  dms_sub_feature_mode;

  policyman_status_t      status;
  policyman_item_t const  *cm_policyman_device_config = NULL;
  policyman_item_id_t ids[] = { POLICYMAN_ITEM_DEVICE_CONFIGURATION };
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO3_0("qmi_dmsi_get_subs_device_feature_mode");

  /*Get the policyman items intially*/
  status = policyman_get_items_block(
              &ids[0],
              sizeof(ids)/sizeof(policyman_item_id_t),
              &cm_policyman_device_config);
  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_0("policyman_get_items_block failed");
    goto send_result;
  }

  /*Get the policyman overall feature first and 
    if it returns the MULTISIM then go for per subs feature mode*/
  status = 
    policyman_device_config_overall_feature(cm_policyman_device_config,&device_overall_cap);
  if (POLICYMAN_FAILED(status))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_INT_0("device_config_overall_feature fail");
    goto send_result;
  }

  if ( device_overall_cap != SYS_OVERALL_FEATURE_MODE_MULTISIM )
  {
    /*Single sim case, treat this as the first subs and send info*/
    max_subs = 1;     
    switch(device_overall_cap)
    { 
      case SYS_OVERALL_FEATURE_MODE_NORMAL:
          dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_NORMAL_V01;
        break;
      case SYS_OVERALL_FEATURE_MODE_SVLTE:
          dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_SVLTE_V01;
        break;
      case SYS_OVERALL_FEATURE_MODE_SGLTE:
          dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_SGLTE_V01;
        break;
      case SYS_OVERALL_FEATURE_MODE_SRLTE:
          dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_SRLTE_V01;
        break;
      default:
          dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_NORMAL_V01;
        break;
    }
    /*Fill the first element of the array*/
    response->subs_device_feature_mode_len = max_subs;
    response->subs_device_feature_mode[max_subs-1] = dms_sub_feature_mode;
  }
  else
  {
    /*Multi sim case, get the per subs info and send it*/
    status = 
      policyman_device_config_num_sims(cm_policyman_device_config,(size_t*)&max_subs);
    if (POLICYMAN_FAILED(status) || (max_subs > SYS_MODEM_AS_ID_3))
    {
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
    response->subs_device_feature_mode_len = max_subs;
    for( subs_index = (uint32)SYS_MODEM_AS_ID_1;
         (subs_index < max_subs) && (subs_index < QMI_DMS_POLICYMAN_MAX_SUBS);
         subs_index++ )
    {
      status = policyman_device_config_get_subs_feature(cm_policyman_device_config,0,
                                                        subs_index,
                                                        &subs_device_feature_mode);
      if (POLICYMAN_FAILED(status))
      {  
       errval = QMI_ERR_INTERNAL_V01;
       goto send_result;
      }

      switch(subs_device_feature_mode)
      { 
        case SYS_SUBS_FEATURE_MODE_NORMAL:
             dms_sub_feature_mode = DMS_DEVICE_SUBS_FEATURE_MODE_NORMAL_V01;
          break;
        case SYS_SUBS_FEATURE_MODE_SVLTE:
          dms_sub_feature_mode    = DMS_DEVICE_SUBS_FEATURE_MODE_SVLTE_V01;
          break;
        case SYS_SUBS_FEATURE_MODE_SGLTE:
            dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_SGLTE_V01;
          break;
        case SYS_SUBS_FEATURE_MODE_SRLTE:
            dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_SRLTE_V01;
          break;
        case SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE:
            dms_sub_feature_mode  = 
                           DMS_DEVICE_SUBS_FEATURE_MODE_DUAL_MULTIMODE_V01;
          break;
        default:
            dms_sub_feature_mode  = DMS_DEVICE_SUBS_FEATURE_MODE_NORMAL_V01;
          break;
      }
      response->subs_device_feature_mode[subs_index] = (uint32)dms_sub_feature_mode;
    }
  }

  send_result:
  if(NULL != cm_policyman_device_config)
  {
    policyman_item_release(cm_policyman_device_config);
  }
  return errval;
}


/*===========================================================================
  FUNCTION QMI_DMSI_GET_MAX_ACTIVE_DATA_SUBS()

  DESCRIPTION
    Get the max active data subscriptions on device from policyman 
    and populate the response

  PARAMETERS
    response - dsm pointer to the response (input parameter)
    cm_pm_device_config -  policyman device config pointer
    errval (output parameter)

  RETURN VALUE
    errval

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_get_max_active_data_subs
(
  uint8_t*  response,
  policyman_item_t const *cm_pm_device_config
)
{
  policyman_status_t  status = -1; 
  policyman_item_t const *cm_policyman_device_config = cm_pm_device_config;
  uint32   max_data_subs = 0;
  qmi_error_type_v01 errval = QMI_ERR_NONE_V01;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  LOG_MSG_INFO3_0("qmi_dmsi_get_max_active_data_subs");

  if (!cm_policyman_device_config)
  {
    errval = QMI_ERR_INTERNAL_V01;
    return errval;
  }

  status = 
    policyman_device_config_max_active_data(cm_policyman_device_config,(size_t*)&max_data_subs);
  if (POLICYMAN_FAILED(status) || (max_data_subs > SYS_MODEM_AS_ID_3))
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  *response = max_data_subs;
  send_result:
  return errval;
}

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
/*===========================================================================
  FUNCTION QMI_DMSI_MOBNUM_PCS_TO_TEXT()

  DESCRIPTION
    convert MDN NV format to ascii-text representation
    
  PARAMETERS
    dest : will contain the mobile number in ascii-text format
    src  : mobile number in NV format

  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_mobnum_pcs_to_text
(
  char *                  dest, 
  nv_mob_dir_data_type *  src
)
{
  int i;
  int len;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(dest && src);

  len = src->n_digits;

  if(len > NV_DIR_NUMB_PCS_SIZ)
  {
    LOG_MSG_INFO1_0("Ignoring Mobile Number digits 16 and higher");
  }

  /*-------------------------------------------------------------------------
    MDN NV format is sort of like BCD, but 8 bits instead of 4, and 
    '0' is represented as 0x0A.  Stop parsing if a place holder digit (0x0F)
    is found.
  -------------------------------------------------------------------------*/
  for (i=0; (i < MIN(len,NV_DIR_NUMB_PCS_SIZ)) && (0x0F != src->digitn[i]); i++)
  {
    dest[i] = (src->digitn[i]==10) ? (char)'0' : (char)src->digitn[i] + '0';
  }
  dest[i] = '\0';
} /* qmi_dmsi_mobnum_pcs_to_text() */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

/*===========================================================================
  FUNCTION QMI_DMSI_RESET_PRL_SERIES_INFO()

  DESCRIPTION
    Resets the stored PRL 'series' information that is used to complet
    QMI_DMS_ACTIVATE_MANUAL requests.    
    
  PARAMETERS
    None

  RETURN VALUE
    ptr to response
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_reset_prl_series_info
(
  void
)
{
  qmi_dmsi_global.prl_info.cl_sp = NULL;
  qmi_dmsi_global.prl_info.seq_num = 0;
  qmi_dmsi_global.prl_info.len_received = 0;
  if(qmi_dmsi_global.prl_info.data != NULL)
  {
    PS_SYSTEM_HEAP_MEM_FREE(qmi_dmsi_global.prl_info.data);
  }
} /* qmi_dmsi_reset_prl_series_info() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_BAND_CAPABILITY()

  DESCRIPTION
    Gives the current band class info
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_band_capability
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
/* When feature flag LTE_BAND_NUM is disabled at MMCP side,
   define it to 64 so that the behavior is as per legacy implementation*/
#ifndef LTE_BAND_NUM
   #define LTE_BAND_NUM 64
#endif 
  qmi_if_info_type  *qmi_if_info_ptr;
  dms_get_band_capability_resp_msg_v01 resp_msg;
  uint16                               i;
  uint64                               bitmask = 1;
  uint64                               lte_band_capability;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&resp_msg,0,sizeof(dms_get_band_capability_resp_msg_v01));
  qmi_if_info_ptr = qmi_if_info_get_ptr();
  resp_msg.band_capability = (uint64)qmi_if_info_ptr->ph_info.band_capability;
  resp_msg.lte_band_capability_valid = TRUE;
  memscpy(&resp_msg.lte_band_capability,
          sizeof(uint64),
          &qmi_if_info_ptr->ph_info.lte_band_capability,
          sizeof(uint64));
  resp_msg.tds_band_capability_valid = TRUE;
  resp_msg.tds_band_capability = (uint64)qmi_if_info_ptr->ph_info.tds_band_capability;

  resp_msg.supported_lte_bands_len = 0;

  /* The following code snippet loops over each band (total bands LTE_BAND_NUM) to get
     the relevant uint64 variable to be used to get the supported band number */
  for (i = 0; i < LTE_BAND_NUM; i++)
  {
    if (i % 64 == 0)
    {
      bitmask = 1;
    }
#if (LTE_BAND_NUM == 256)
    if (i < 64)
    {
      lte_band_capability = qmi_if_info_ptr->ph_info.lte_band_capability.bits_1_64;
    }
    else if (i >= 64 && i < 128)
    {
      lte_band_capability = qmi_if_info_ptr->ph_info.lte_band_capability.bits_65_128;
    }
    else if (i >= 128 && i < 192)
    {
      lte_band_capability = qmi_if_info_ptr->ph_info.lte_band_capability.bits_129_192;
    }
    else
    {
      lte_band_capability = qmi_if_info_ptr->ph_info.lte_band_capability.bits_193_256;
    }
#else
    lte_band_capability = qmi_if_info_ptr->ph_info.lte_band_capability;
#endif /* LTE_BAND_NUM == 256 */
    
    if (bitmask & lte_band_capability)
    {
      resp_msg.supported_lte_bands[resp_msg.supported_lte_bands_len++] = i+1;
    }
    bitmask = bitmask << 1;
  }
  if (resp_msg.supported_lte_bands_len > 0)
  {
    resp_msg.supported_lte_bands_valid = TRUE;
  }

  resp_msg.resp.error = QMI_ERR_NONE_V01;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_band_capability_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;


} /* qmi_dmsi_get_band_capability */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_FACTORY_SKU()

  DESCRIPTION
    Gives the factory SKU for the device.
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_factory_sku
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result = QMI_RESULT_SUCCESS_V01;
  char *             factory_sku;
  nv_stat_enum_type  nv_status;
  nv_item_type      *dcc_nv_item_ptr;
  dms_get_factory_sku_resp_msg_v01 resp_msg;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_factory_sku_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  
  // Query NV For factory SKU
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  nv_status = dcc_get_nv_item(NV_FACTORY_DATA_4_I, dcc_nv_item_ptr);
  if (NV_DONE_S == nv_status)
  {
    factory_sku = (char *) dcc_nv_item_ptr->factory_data_4;
    strlcpy((char*)&(resp_msg.factory_serial_number),factory_sku,sizeof(resp_msg.factory_serial_number));
  } 
  else if (NV_NOTACTIVE_S == nv_status) 
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
  } 
  else 
  {
    errval = QMI_ERR_INTERNAL_V01;
  }

  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
  
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_factory_sku_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_get_factory_sku */

#ifdef FEATURE_MMGSDI_SESSION_LIB
/*===========================================================================
  FUNCTION QMI_DMSI_GENERATE_READ_UIM_MSISDN_RESP()

  DESCRIPTION
    To send a response to the QMI read MSISDN requested by the client.
    
  PARAMETERS
    read_cnf : info for IMSI read from UIM
    cmd_buf_p :Async command buffer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_generate_read_uim_msisdn_resp
(
  mmgsdi_read_cnf_type * read_cnf,
  ds_qmi_dms_async_cmd_type* cmd_buf_p
)
{
  qmi_error_type_v01        errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01       result = QMI_RESULT_SUCCESS_V01;
  boolean                 output_msisdn;
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  char                    mob_num[NV_DIR_NUMB_PCS_SIZ + 1];
  char                    min_num[NV_DIR_NUMB_PCS_SIZ+1];
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

  mmgsdi_return_enum_type mmgsdi_status;
  mmgsdi_access_type      sim_filename;
  qmi_if_mmgsd_session_e_type session_index;

  qmi_if_info_type  *qmi_if_info_ptr;
  qmi_dmsi_client_info_type * dms_cl_sp;
  ds_qmi_dms_async_cmd_type* async_cmd_buf = cmd_buf_p;
  static char uim_imsi[DMSI_MMGSDI_IMSI_LEN * 2 + 1];
  static char uim_msisdn[DMSI_MMGSDI_MSISDN_LEN * 2 + 1];
  dms_get_msisdn_resp_msg_v01 resp_msg;
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mmgsdi_status = read_cnf->response_header.mmgsdi_status;
  qmi_if_info_ptr = qmi_if_info_get_ptr();
    
  /*-------------------------------------------------------------------------
    Build the response from back to front
  -------------------------------------------------------------------------*/
  output_msisdn = FALSE;
  dms_cl_sp = cmd_buf_p->cl_sp;
  if(dms_cl_sp == NULL)
  {
    return;
  }
  session_index = QMI_IF_MMGSD_GW_PROV_PRI_SESSION;

#ifdef FEATURE_DUAL_SIM
  if(dms_cl_sp->subscription_id == DMSI_BIND_SUBS_SECONDARY) 
  {
    session_index = QMI_IF_MMGSD_GW_PROV_SEC_SESSION;
  }
#endif
#ifdef FEATURE_TRIPLE_SIM
  if(dms_cl_sp->subscription_id == DMSI_BIND_SUBS_TERTIARY)
  {
    session_index = QMI_IF_MMGSD_GW_PROV_TER_SESSION;
  }
#endif
  if (read_cnf->access.file.file_enum != MMGSDI_TELECOM_MSISDN &&
      read_cnf->access.file.file_enum != MMGSDI_USIM_MSISDN    &&
      read_cnf->access.file.file_enum != MMGSDI_GSM_IMSI       && 
      read_cnf->access.file.file_enum != MMGSDI_USIM_IMSI      &&
      read_cnf->access.file.file_enum != MMGSDI_USIM_EXT5)
  {
    local_err = QMI_DMSI_LOCAL_ERR_INVAL_FILE_ENUM;
    goto bail;
  }

  if(read_cnf->access.file.file_enum == MMGSDI_GSM_IMSI ||
     read_cnf->access.file.file_enum == MMGSDI_USIM_IMSI)
  {
    memset(uim_imsi, 0, DMSI_MMGSDI_IMSI_LEN * 2 + 1);

    /* Report IMSI only when it is available through MMGSDI */
    if (MMGSDI_SUCCESS == mmgsdi_status) 
    {
      int i;
      uint8 current_byte;
      boolean bcd_valid = FALSE;

      /* Decode MCC1 from Byte 2 (bit 4-7), should always available */
      current_byte = read_cnf->read_data.data_ptr[1];
      if(QMI_BCD_HIGH_DIGIT(current_byte) < 0x0A)
      {
        bcd_valid = TRUE;
        uim_imsi[0] = QMI_BCD_HIGH_DIGIT(current_byte) + '0';

        /* Decode the rest from BCD to ASCII string, stop when out of range */
        for(i = 2; i < DMSI_MMGSDI_IMSI_LEN; i++)
        {
          current_byte = read_cnf->read_data.data_ptr[i];
          if(QMI_BCD_LOW_DIGIT(current_byte) < 0x0A)
            uim_imsi[(i - 2) * 2 + 1] = QMI_BCD_LOW_DIGIT(current_byte) + '0';
          else if(QMI_BCD_LOW_DIGIT(current_byte) == 0x0F)
            break;
          else
          {
            bcd_valid = FALSE;
            break;
          }
        
          if(QMI_BCD_HIGH_DIGIT(current_byte) < 0x0A)
            uim_imsi[(i - 2) * 2 + 2] = QMI_BCD_HIGH_DIGIT(current_byte) + '0';
          else if(QMI_BCD_HIGH_DIGIT(current_byte) == 0x0F)
            break;
          else
          {
            bcd_valid = FALSE;
            break;
          }
        }
      }

      /* Only send IMSI if it is valid*/
      if(bcd_valid != TRUE)
      {
          memset(uim_imsi, 0, DMSI_MMGSDI_IMSI_LEN);
      }
    }

    /* Read MSISDN request to MMGSDI */
    sim_filename.access_method = MMGSDI_EF_ENUM_ACCESS;
    sim_filename.file.file_enum = MMGSDI_NO_FILE_ENUM;
    
    if (qmi_if_info_ptr->session_info[session_index].app_type == MMGSDI_APP_SIM)
    {
      sim_filename.file.file_enum = MMGSDI_TELECOM_MSISDN;
    }
    else if (qmi_if_info_ptr->session_info[session_index].app_type == MMGSDI_APP_USIM)
    {
      sim_filename.file.file_enum = MMGSDI_USIM_MSISDN;
    }
    else
    {
       local_err = QMI_DMSI_LOCAL_ERR_OP_DEVICE_UNSUPPORTED;
       errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
       goto send_result;
    }

    /* Read entire record for EF-MSISDN (x+14), len param = 0 */
    mmgsdi_status = mmgsdi_session_read_record(
                                     qmi_if_info_ptr->session_info[session_index].session_id,
                                     sim_filename,
                                     1,
                                     0,
                                     qmi_if_mmgsdi_resp_cback,
                                     (uint32) async_cmd_buf);

    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* MSISDN will be reported back later from MMGSDI, other items will be
      processed at that time */
      return;
    }
    else
    { 
      /* For any reason MSISDN cannot be obtained from MMGSDI it will be
      skipped and the functon will go forward to process other items */
      local_err = QMI_DMSI_LOCAL_ERR_MMGSDI_MSISDN_FAIL;
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
  
  }
  
  if (read_cnf->access.file.file_enum == MMGSDI_TELECOM_MSISDN ||
      read_cnf->access.file.file_enum == MMGSDI_USIM_MSISDN)
  {
    memset(uim_msisdn, 0, DMSI_MMGSDI_MSISDN_LEN * 2 + 1);
    /* Report MSISDN only when it is available through MMGSDI */
    if (MMGSDI_SUCCESS == mmgsdi_status) 
    {
      uint8 current_byte, i, offset, len;
      boolean bcd_valid = FALSE;
      /* Decode MSISDN, start by finding the start of binary MSISDN data */
      #define DMSI_MSISDN_ENCODED_DATA_OFFSET 14

      /* make sure the overall length is valid (greater than MSISDN start) */
      if (read_cnf->read_data.data_len < DMSI_MSISDN_ENCODED_DATA_OFFSET)
      {
        local_err = QMI_DMSI_LOCAL_ERR_INVAL_MSISDN_LEN;
        errval = QMI_ERR_INTERNAL_V01;
        goto send_result;
      }

      /* calculate the MSISDN start (offset is from the end of the EF) */
      offset = (uint8)(read_cnf->read_data.data_len - DMSI_MSISDN_ENCODED_DATA_OFFSET);

      /* Decode MCC1 from Byte 2 (bit 4-7), should always available */
      len = read_cnf->read_data.data_ptr[offset];
      if( len < DMSI_MSISDN_ENCODED_DATA_OFFSET )
      {
        bcd_valid = TRUE;

        /* Decode the rest from BCD to ASCII string, stop when out of range */
        for(i = 2; i <= len; i++)
        {
          current_byte = read_cnf->read_data.data_ptr[offset + i];
          if (QMI_BCD_LOW_DIGIT(current_byte) < 0x0A)
            uim_msisdn[(i - 2) * 2] = QMI_BCD_LOW_DIGIT(current_byte)+'0';
          else if (QMI_BCD_LOW_DIGIT(current_byte) == 0x0F)
            break;
          else
          {
            bcd_valid = FALSE;
            break;
          }

          if (QMI_BCD_HIGH_DIGIT(current_byte) < 0x0A)
            uim_msisdn[(i - 2) * 2 + 1] = QMI_BCD_HIGH_DIGIT(current_byte)+'0';
          else if (QMI_BCD_HIGH_DIGIT(current_byte) == 0x0F)
            break;
          else
          {
            bcd_valid = FALSE;
            break;
          }
        }
      }

      /* Only send MSISDN if it is valid*/
      if(bcd_valid != TRUE)
      {
        local_err = QMI_DMSI_LOCAL_ERR_INVAL_MSISDN_BCD_FORMAT;
        memset(uim_msisdn, 0, DMSI_MMGSDI_MSISDN_LEN * 2 + 1);
      }
      /* Reading Extension file if additional data is there */
      if(bcd_valid == TRUE && read_cnf->read_data.data_ptr[read_cnf->read_data.data_len-1] != 0xFF)
      {
        mmgsdi_access_type  sim_filename_ext;
        mmgsdi_rec_num_type  ext_rec_num;
        /* extension record number is present in the last byte*/
        ext_rec_num = read_cnf->read_data.data_ptr[read_cnf->read_data.data_len-1];

        sim_filename_ext.access_method = MMGSDI_EF_ENUM_ACCESS;
        sim_filename_ext.file.file_enum = MMGSDI_NO_FILE_ENUM;
    
        if (qmi_if_info_ptr->session_info[session_index].app_type == MMGSDI_APP_USIM)
        {
          sim_filename_ext.file.file_enum = MMGSDI_USIM_EXT5;
        }
        else
        {
          local_err = QMI_DMSI_LOCAL_ERR_EXT_UIM_ONLY;
          goto send_result;
        }

        mmgsdi_status = mmgsdi_session_read_record(
                                   qmi_if_info_ptr->session_info[session_index].session_id,
                           	       sim_filename_ext,
                                   ext_rec_num,
                                   0,
                                   qmi_if_mmgsdi_resp_cback,
                                   (uint32)async_cmd_buf);
       if (MMGSDI_SUCCESS == mmgsdi_status)
       {
         return;
       }
       else
       {
         /* For any reason MSISDN cannot be obtained from MMGSDI it will be
            skipped and the functon will go forward to process other items */
         local_err = QMI_DMSI_LOCAL_ERR_MMGSDI_MSISDN_FAIL;
         errval = QMI_ERR_INTERNAL_V01;        
       }
      }
    }
    else
    {
      errval = (qmi_error_type_v01)qmi_if_map_mmgsdi_status_to_errval(mmgsdi_status,__LINE__);
    }

    /* MSISDN is the last item, continue sending result */
    goto send_result;
  }

  if(read_cnf->access.file.file_enum == MMGSDI_USIM_EXT5)
  {
    /* Report MSISDN only when it is available through MMGSDI */
    if (MMGSDI_SUCCESS == mmgsdi_status) 
    {
      uint8 current_byte, j, len;
      boolean bcd_valid = FALSE;

      /* Length of Ext5 file  */
      #define DMSI_MSISDN_EXTN_LENGTH 5

      /* make sure the overall length is valid (greater than MSISDN start) */
      if (read_cnf->read_data.data_len < read_cnf->read_data.data_ptr[1])
      {
         local_err = QMI_DMSI_LOCAL_ERR_INVAL_READ_DATA_LEN;
         goto send_result;
      }
      /* In byte 2, length of additional data is present */
      len = read_cnf->read_data.data_ptr[1];

      #define MSISDN_EXT_ADDITIONAL_DATA_TYPE 0x02
      /* Data type should be additional data*/
      if(read_cnf->read_data.data_ptr[0] == MSISDN_EXT_ADDITIONAL_DATA_TYPE)
      {
         bcd_valid = TRUE;
         /* 10 bytes are already read from msisdn*/
         #define UIM_MSISDN_OFFSET 10

         /* Decode the rest from BCD to ASCII string, stop when out of range. 
            Only 5 bytes will be read here */
         for(j=0; (j < len && j < DMSI_MSISDN_EXTN_LENGTH); j++)
         {
            current_byte = read_cnf->read_data.data_ptr[j+2];
            if (QMI_BCD_LOW_DIGIT(current_byte) < 0x0A)
              uim_msisdn[(j+UIM_MSISDN_OFFSET) * 2] = QMI_BCD_LOW_DIGIT(current_byte)+'0';
           else if (QMI_BCD_LOW_DIGIT(current_byte) == 0x0F)
              break;
           else
           {
              bcd_valid = FALSE;
              break;
           }

            if (QMI_BCD_HIGH_DIGIT(current_byte) < 0x0A)
              uim_msisdn[(j+UIM_MSISDN_OFFSET) * 2 + 1] = QMI_BCD_HIGH_DIGIT(current_byte)+'0';
            else if (QMI_BCD_HIGH_DIGIT(current_byte) == 0x0F)
               break;
            else
            {
              bcd_valid = FALSE;
              break;
            }
         }
         if(bcd_valid != TRUE)
         {
           local_err = QMI_DMSI_LOCAL_ERR_INVAL_MSISDN_BCD_FORMAT;
           memset(uim_msisdn, 0, DMSI_MMGSDI_MSISDN_LEN * 2 + 1);
         }
       }
     }
     else
     {
       local_err = QMI_DMSI_LOCAL_ERR_MMGSDI_EXT_FAIL;
     }
   }

send_result:
  /* add the IMSI anytime it is present */
  memset(&resp_msg,0,sizeof(dms_get_msisdn_resp_msg_v01));
  if(strlen(uim_imsi))
  {
    strlcpy((char*)resp_msg.imsi,uim_imsi,sizeof(resp_msg.imsi));
    resp_msg.imsi_valid = TRUE;
  }

  /* Read MDN and MIN */
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
  memset(mob_num, 0, sizeof(mob_num));
  memset(min_num, 0, sizeof(min_num));

  errval = qmi_dmsi_get_mdn_min(mob_num, sizeof(mob_num),
                                min_num, sizeof(min_num),
                                (uint16)dms_cl_sp->subscription_id);
  /* add the MIN anytime it is present */
  if (errval == QMI_ERR_NONE_V01)
  {
    if (strlen(min_num))
    {
      resp_msg.mobile_id_number_valid = TRUE;
      strlcpy((char*)resp_msg.mobile_id_number,min_num,
               sizeof(resp_msg.mobile_id_number));
    }
    strlcpy((char*)resp_msg.voice_number,mob_num,sizeof(resp_msg.voice_number));
  }

  //Getting MDN and MIN Failed, include MSISDN in the response.
  if(errval != QMI_ERR_NONE_V01)
  {
    output_msisdn = TRUE;
    errval = QMI_ERR_NONE_V01;
  }
#else
  /* Return MSISDN as CDMA feature is disabled */
  output_msisdn = TRUE;
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

  if(output_msisdn) 
  {
    /* MSISDN */
    if(strlen(uim_msisdn))
    {
      strlcpy((char*)resp_msg.voice_number,uim_msisdn,
               sizeof(resp_msg.voice_number));
    }
    else if (errval == QMI_ERR_NONE_V01)
    {
      /* No records of MSISDN */
      errval = QMI_ERR_NOT_PROVISIONED_V01;
    }
  }
  /*clear the reponse and send only error incase of failure*/
  if(errval != QMI_ERR_NONE_V01)
  {
    result = QMI_RESULT_FAILURE_V01;
    memset(&resp_msg,0,sizeof(resp_msg));
  }
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  if(qmi_dms_check_valid_async_cmd_buf(async_cmd_buf))
  {
    qmi_dmsi_send_response(async_cmd_buf->req_handle, 
                           async_cmd_buf->msg_id,
                           &resp_msg,  	
                           sizeof(dms_get_msisdn_resp_msg_v01));
  }
  PS_SYSTEM_HEAP_MEM_FREE(async_cmd_buf);
bail:
  LOG_MSG_INFO1_1("sending response for MSISDN get with local_err %d", local_err);
} /* qmi_dmsi_generate_read_uim_msisdn_resp */

#endif /* FEATURE_MMGSDI_SESSION_LIB */

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
#if (defined FEATURE_OTASP)
/*===========================================================================
  FUNCTION QMI_DMSI_OTASP_FORCED_CALL_RELEASE()

  DESCRIPTION
    End all calls to allow power down regostration through after OTASP session

  PARAMETERS
   None
    
  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_otasp_forced_call_release()
{
  cm_end_params_s_type end_params;

  if ( qmi_dmsi_global.voice_call_id != CM_CALL_ID_INVALID )
  {
     end_params.call_id = (cm_call_id_type) qmi_dmsi_global.voice_call_id;
     end_params.info_type = CM_CALL_MODE_INFO_CDMA;
     end_params.end_params.cdma_end.end_reason_included = FALSE;
     end_params.end_params.cdma_end.end_reason = CAI_REL_NORMAL;
     (void) cm_mm_call_cmd_end( NULL, NULL, (cm_client_id_type) -1,
                              1, &end_params );
  }
  else
  {
      LOG_MSG_INFO1_0 ("QMI: Could not locate call_id ");
  }
} /* qmi_dmsi_otasp_forced_call_release */
#endif /* (defined FEATURE_OTASP) */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */


/*===========================================================================
  FUNCTION QMI_DMS_PROCESS_CALL_EVENT()

  DESCRIPTION
    Called when a QMI_IF call event happens.
    
  PARAMETERS
    None

  RETURN VALUE
    None
        
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_process_call_event
(
  void
)
{
  qmi_if_info_type  *qmi_if_info_ptr;
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
  
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
#if defined(FEATURE_OTASP)

  qmi_if_asubs_id_type  sub_id = QMI_IF_AS_ID_MIN;

#endif /* defined(FEATURE_OTASP) */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  qmi_if_info_ptr = qmi_if_info_get_ptr();

  switch (qmi_if_info_ptr->call_info.call_event)
  {
    case CM_CALL_EVENT_END:
    case CM_CALL_EVENT_ORIG:
    case CM_CALL_EVENT_CONNECT:
      /* CDMA devices */
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
      /* OTASP activation states */
#if defined(FEATURE_OTASP)
      if ((CM_CALL_TYPE_STD_OTASP     == qmi_if_info_ptr->call_info.call_type) ||
          (CM_CALL_TYPE_NON_STD_OTASP == qmi_if_info_ptr->call_info.call_type))
      {
        qmi_dmsi_global.act_evt.act_type = QMI_DMSI_ACTIVATION_TYPE_CALL;
        qmi_dmsi_global.act_evt.event.call_event = 
                                               qmi_if_info_ptr->call_info.call_event;

        sub_id = qmi_if_get_default_sub_id();

        LOG_MSG_INFO2_3("qmi_dms_process_call_event: call_event %d activation_state %d sys_mode %d",
                        qmi_if_info_ptr->call_info.call_event,
                        qmi_dmsi_global.activation_state,
                        qmi_if_info_ptr->ss_info[sub_id].sys_mode);

        qmi_dmsi_generate_activation_state_ind();
      }
#endif /* defined(FEATURE_OTASP) */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
      break;

    case CM_CALL_EVENT_OTASP_STATUS:
      /* CDMA devices */
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
      /* OTASP activation states */
#if defined(FEATURE_OTASP)
      if ((CM_CALL_TYPE_STD_OTASP     == qmi_if_info_ptr->call_info.call_type) ||
          (CM_CALL_TYPE_NON_STD_OTASP == qmi_if_info_ptr->call_info.call_type))
      {
        if (CM_OTASP_STATUS_SPC_RETRIES_EXCEEDED == 
                                              qmi_if_info_ptr->call_info.otasp_state)
        {
          /* Too many failed attempts to unlock phone. power down */
          (void)cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, 
                                    SYS_OPRT_MODE_OFFLINE_CDMA);
          if(!cm_rpm_check_reset_allowed())
          {
            local_err = QMI_DMSI_LOCAL_ERR_OTSAP_SPC_RETRIES_MAX_VAL;
            break;
          }
          LOG_MSG_INFO1_0("Triggered Sys Initate Poweroff");          
          sys_m_initiate_poweroff();
        } 
        else 
        {
          qmi_dmsi_global.act_evt.act_type = QMI_DMSI_ACTIVATION_TYPE_OTASP;
          qmi_dmsi_global.act_evt.event.otasp_state = 
                                              qmi_if_info_ptr->call_info.otasp_state;
          qmi_dmsi_generate_activation_state_ind();
        }
      }
#endif /* defined(FEATURE_OTASP) */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
      break;

    default:
      local_err = QMI_DMSI_LOCAL_ERR_UNKNOWN_CALL_EVENT;
      break;
  }

  LOG_MSG_INFO1_1("DMS Process call event exit with local err %d", local_err);
} /* qmi_dms_process_call_event */


#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)

/*===========================================================================
  FUNCTION QMI_DMSI_GENERATE_ACTIVATION_STATE_IND()

  DESCRIPTION
    Called when the activation state information changes. Causes the 
    activation_state_ind to be sent to all clients that have requested to be
    notified of such events.
    
  PARAMETERS
    None

  RETURN VALUE
    None
        
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_generate_activation_state_ind
(
  void
)
{
  qmi_dmsi_client_info_type *     cl_sp;
  dms_activation_state_enum_v01 act_state;
#if (defined FEATURE_OTASP)
  boolean                          otasp_state;
  boolean                          last_otasp_state;
  qmi_nv_status_e_type             nv_status;
#endif /* (defined FEATURE_OTASP) */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!qmi_dmsi_inited)
  {
    return;
  }
  act_state = DMS_ACTIVATION_STATE_ENUM_MAX_ENUM_VAL_V01;

#if (defined FEATURE_OTASP)
  otasp_state = FALSE;
  last_otasp_state = FALSE;
#endif /* (defined FEATURE_OTASP) */

  LOG_MSG_INFO2_3("Generating DMS IND for activation state change (event=%d)"
                  "and call_event %d, OTASP state %d",
                  qmi_dmsi_global.act_evt.act_type,
                  qmi_dmsi_global.act_evt.event.call_event,
                  qmi_dmsi_global.act_evt.event.otasp_state);

  /*-----------------------------------------------------------------------
    Determine the new activation state based on the current global values
  -----------------------------------------------------------------------*/
  switch (qmi_dmsi_global.act_evt.act_type)
  {

    /* CDMA devices */
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
    /* OTASP activation states */
#if (defined FEATURE_OTASP)
    case QMI_DMSI_ACTIVATION_TYPE_CALL:
    {
      switch (qmi_dmsi_global.act_evt.event.call_event)
      {
        case CM_CALL_EVENT_ORIG:
        {
          act_state = DMS_ACTIVATION_CONNECTING_V01;
          break;
        }
            
        case CM_CALL_EVENT_CONNECT:
        {
          act_state = DMS_ACTIVATION_CONNECTED_V01;
          break;
        }

        /* To determine 'activated' state based on the last OTASP
           success or failure, do so once the call has ended */
        case CM_CALL_EVENT_END:
        {
          /* If the last state was COMMIT then the call was successful */
          if (DMS_ACTIVATION_OTASP_SETTINGS_COMMITTED_V01 == qmi_dmsi_global.activation_state)
          {
            act_state = DMS_ACTIVATION_ACTIVATED_V01;
            otasp_state = TRUE;
          }
          /* Else it failed */
          else
          {
            act_state = DMS_ACTIVATION_NOT_ACTIVATED_V01;
            otasp_state = FALSE;
          }
          
          /* see if build should store last OTASP state to QMI NV item */
          if (qmi_dmsi_global.nv_use_last_otasp_act_state)
          {
             /* if so, read the 'last' OTASP state */
             nv_status = qmi_nv_read(QMI_NV_ITEM_LAST_OTASP_STATE, 0, 0,
                                    &last_otasp_state, 
                                    sizeof(last_otasp_state));

            /* check if the current val was read or not.  if it was read, make
               sure the new val is different.  if it wasn't read, try to write
               anyways. */
             
              /* if the current OTASP state doesn't match last, write new val */
            if (otasp_state != last_otasp_state)
            {
              qmi_nv_write(QMI_NV_ITEM_LAST_OTASP_STATE, 
                           0, 0, &otasp_state, 
                           sizeof(otasp_state));
            }
          }
          break;
        }

        /* No other CM call events translate into activation states so ignore */
        default:
          break;
      }
      break;
    } /* case QMI_DMSI_ACTIVATION_TYPE_CALL */

    case QMI_DMSI_ACTIVATION_TYPE_OTASP:
    {
      switch (qmi_dmsi_global.act_evt.event.otasp_state)
      {
        case CM_OTASP_STATUS_SPL_UNLOCKED:
        {
          act_state = DMS_ACTIVATION_OTASP_SEC_AUTHENTICATED_V01;
          break;
        }

        case CM_OTASP_STATUS_NAM_DOWNLOADED:
        {
          act_state = DMS_ACTIVATION_OTASP_NAM_DOWNLOADED_V01;
          break;
        }

        case CM_OTASP_STATUS_MDN_DOWNLOADED:
        {
          act_state = DMS_ACTIVATION_OTASP_MDN_DOWNLOADED_V01;
          break;
        }

        case CM_OTASP_STATUS_IMSI_DOWNLOADED:
        {
          act_state = DMS_ACTIVATION_OTASP_IMSI_DOWNLOADED_V01;
          break;
        }

        case CM_OTASP_STATUS_PRL_DOWNLOADED:
        {
          act_state = DMS_ACTIVATION_OTASP_PRL_DOWNLOADED_V01;
          break;
        }

        case CM_OTASP_STATUS_COMMITTED:
        {
          act_state = DMS_ACTIVATION_OTASP_SETTINGS_COMMITTED_V01;
          break;
        }

        /* No other OTASP events translate into activation states so ignore */
        default:
          break;
      }
      break;
    } /* case QMI_DMSI_ACTIVATION_TYPE_OTASP */

#endif /* (defined FEATURE_OTASP) */
#endif /* (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
    default:
      break;
  }

  /* If the activation state was not changed then there is no new activation IND
     to send.  Return here ignore the un-matched events. */
  if (DMS_ACTIVATION_STATE_ENUM_MAX_ENUM_VAL_V01 == act_state)
  {
    return;
  }

  LOG_MSG_INFO2_2("qmi_dmsi_generate_activation_state_ind: old %d, new %d",
                  qmi_dmsi_global.activation_state, act_state);
  if (act_state != qmi_dmsi_global.activation_state)
  {
    /* Update the global state if the 'new' state is valid and has changed */
    qmi_dmsi_global.activation_state = act_state;
    cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);
    /* Generate event report ind for clients registered for authetication state
       changes */
    while(cl_sp != NULL)
    {

      if( cl_sp->report_status.report_activation_state )
      {
          qmi_dmsi_event_report_ind(cl_sp,
                                    DMSI_REPORT_STATUS_EV_ACTIVATION_STATE,
                                    0,
                                    0,
                                    0,
                                    (uint16) qmi_dmsi_global.activation_state,
                                    0,
                                    0,
                                    0);
      }
      cl_sp = q_next(&qmi_dms_state.client_q,
                           &(cl_sp->link));
    }
  }
  //else it is cached

#if (defined FEATURE_OTASP)
  if (DMS_ACTIVATION_OTASP_SETTINGS_COMMITTED_V01 == qmi_dmsi_global.activation_state)
  {
    qmi_dmsi_otasp_forced_call_release();
  }
#endif /* (defined FEATURE_OTASP) */
} /* qmi_dmsi_generate_activation_state_ind */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */

/*===========================================================================
FUNCTION  QMI_DMS_CM_CALL_CMD_CB

DESCRIPTION
  CM call command callback function

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  Adds command in DS command buffer

===========================================================================*/
void qmi_dms_cm_call_cmd_cb 
(
  void                         *data_ptr,         /* Data block pointer    */
  cm_call_cmd_e_type           call_cmd,          /* Command ID            */
  cm_call_cmd_err_e_type       call_cmd_err       /* Command error code    */
)
{
   qmi_dmsi_cmd_buf_type *  cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-------------------------------------------------------------------------
    Post a cmd to DCC to handle this in DCC context
  -------------------------------------------------------------------------*/

  switch(call_cmd)
  {
    case CM_CALL_CMD_ORIG:
      cmd_ptr = (qmi_dmsi_cmd_buf_type *) qmi_dmsi_get_cmd_buf();
      if( cmd_ptr != NULL)
      {
        cmd_ptr->cmd_id = QMI_CMD_DMS_CM_CALL_CMD_STATUS;
        cmd_ptr->data.cm_call_err.user_data = data_ptr;
        cmd_ptr->data.cm_call_err.call_cmd  = call_cmd;
        cmd_ptr->data.cm_call_err.err_type  = call_cmd_err;
        dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
      }
      break;

    default:
      LOG_MSG_INFO2_0("Unexpected ph_cmd received in qmi_dmsi_cm_ph_cb");
  }

  return;
  
} /* qmi_dms_cm_call_cmd_cb */



/*===========================================================================
  FUNCTION QMI_DMS_PROCESS_CM_CALL_ERR()

  DESCRIPTION
    Process the CM call event

  PARAMETERS
    user_data :  user data ptr passed to CM
    call_cmd  :  requested CM call command
    err_type  :  result of request
    
  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_process_cm_call_err
(
  void *                 user_data,
  cm_call_cmd_e_type     call_cmd,
  cm_call_cmd_err_e_type err_type
)
{
  ds_qmi_dms_async_cmd_type *     cmd_buf_p;
  qmi_error_type_v01              errval = QMI_ERR_NONE_V01;
  dms_activate_automatic_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd_buf_p = (ds_qmi_dms_async_cmd_type*) user_data;
  if(cmd_buf_p == NULL)
  {
    return;
  }
  memset(&resp_msg,0,sizeof(dms_activate_automatic_resp_msg_v01));
  /* map the CM request error to the proper QMI error value */
  switch (err_type)
  {
    case CM_CALL_CMD_ERR_NOERR:
      errval = QMI_ERR_NONE_V01;
      break;

    case CM_CALL_CMD_ERR_OFFLINE_S:
      errval = QMI_ERR_NO_NETWORK_FOUND_V01;
      break;

    case CM_CALL_CMD_ERR_IN_USE_S:
      errval = QMI_ERR_DEVICE_IN_USE_V01;
      break;

    default:
      errval = QMI_ERR_INTERNAL_V01;
      break;
  }
  resp_msg.resp.error = errval;
  resp_msg.resp.result = (errval == QMI_ERR_NONE_V01)?QMI_RESULT_SUCCESS_V01:QMI_RESULT_FAILURE_V01;
  if(qmi_dms_check_valid_async_cmd_buf(cmd_buf_p))
  {
    qmi_dmsi_send_response(cmd_buf_p->req_handle,
                           cmd_buf_p->msg_id,
                           &resp_msg, 
                           sizeof(dms_activate_automatic_resp_msg_v01));
  }
  PS_SYSTEM_HEAP_MEM_FREE(cmd_buf_p);
} /* qmi_dms_process_cm_call_err() */

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
/*===========================================================================
  FUNCTION QMI_DMSI_GET_MDN_MIN()

  DESCRIPTION
    a
    
  PARAMETERS
    None

  RETURN VALUE
    None
        
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_type_v01 qmi_dmsi_get_mdn_min
(
  char *mob_num,
  int   mdn_size,
  char *min_num,
  int   min_size,
  uint16 sub_id
)
{
  qmi_error_type_v01      errval = QMI_ERR_NONE_V01;
  
  dword                   min1;
  word                    min2;
  int                     min_len;

  nv_stat_enum_type       nv_status;
  nv_item_type           *dcc_nv_item_ptr;

  cm_nam_e_type           curr_nam;
  mmgsdi_session_type_enum_type   session_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT(mob_num && min_num);

  if(sub_id >= DMSI_BIND_SUBS_MAX)
  {
    return QMI_ERR_INTERNAL_V01;
  }

  min1 = min2 = min_len = 0;

  memset(mob_num, 0, mdn_size);
  memset(min_num, 0, min_size);

#if defined(FEATURE_RUIM) && defined(FEATURE_UIM_RUN_TIME_ENABLE)
#ifdef FEATURE_MMGSDI_SESSION_LIB
  session_type = MMGSDI_1X_PROV_PRI_SESSION;
	
#ifdef FEATURE_DUAL_SIM
  if(sub_id == DMSI_BIND_SUBS_SECONDARY) 
  {
    session_type = MMGSDI_1X_PROV_SEC_SESSION;
  }
#endif
#ifdef FEATURE_TRIPLE_SIM
  if(sub_id == DMSI_BIND_SUBS_TERTIARY)
  {
    session_type = MMGSDI_1X_PROV_TER_SESSION;
  }
#endif
  /*-------------------------------------------------------------------------
    Check the RUIM state before querying MDN/MIN
  -------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_MPSS_AT20_DEV
  if ((NV_RTRE_CONTROL_USE_RUIM == nv_rtre_control()) &&
#else
  if ((NV_RTRE_CONTROL_USE_RUIM == nv_rtre_control_ext(sub_id)) &&
#endif /* FEATURE_DATA_MPSS_AT20_DEV */
      ((errval = (qmi_error_type_v01)
       qmi_dms_get_uim_access_status_ext(session_type)) != 
       QMI_ERR_NONE_V01))
  {
      LOG_MSG_INFO1_1("RUIM not initialized (%d)", errval);
  }
#else /* FEATURE_MMGSDI_SESSION_LIB */
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
#endif /* FEATURE_MMGSDI_SESSION_LIB */
#endif /* defined(FEATURE_RUIM) && defined(FEATURE_UIM_RUN_TIME_ENABLE) */

  LOG_MSG_INFO1_3("cl sub_id %d session_type %d errval (%d)",sub_id, session_type, errval);

  if (QMI_ERR_NONE_V01 == errval)
  {
    /*-------------------------------------------------------------------------
      Allocate temporary memory for the NV item
    -------------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
    if( dcc_nv_item_ptr == NULL )
    {
      return QMI_ERR_NO_MEMORY_V01;
    }
    nv_status = dcc_get_nv_item(NV_CURR_NAM_I, dcc_nv_item_ptr);  
    if(nv_status != NV_DONE_S || dcc_nv_item_ptr->curr_nam >= NV_MAX_NAMS)
    {
      /*-----------------------------------------------------------------------
        Default to the first NAM
      -----------------------------------------------------------------------*/
      dcc_nv_item_ptr->curr_nam = (byte) CM_NAM_1;
    }
    curr_nam = (cm_nam_e_type)dcc_nv_item_ptr->curr_nam;
    dcc_nv_item_ptr->mob_dir_number.nam = dcc_nv_item_ptr->curr_nam;

    /*-------------------------------------------------------------------------
      Get the Mobile Directory Number from NV
    -------------------------------------------------------------------------*/
    nv_status = dcc_get_nv_item_ext(NV_DIR_NUMBER_PCS_I, dcc_nv_item_ptr, sub_id);
    if (NV_DONE_S == nv_status)
    {
      /*-----------------------------------------------------------------------
        Convert the mobile number to text format
      -----------------------------------------------------------------------*/
      qmi_dmsi_mobnum_pcs_to_text(mob_num, &dcc_nv_item_ptr->mob_dir_number);
      dcc_nv_item_ptr->min1.nam = curr_nam;
      if (NV_DONE_S == dcc_get_nv_item_ext(NV_MIN1_I, dcc_nv_item_ptr, sub_id))
      {
        min1 = dcc_nv_item_ptr->min1.min1[NV_CDMA_MIN_INDEX];
        if (NV_DONE_S == dcc_get_nv_item_ext(NV_MIN2_I, dcc_nv_item_ptr, sub_id))
        {
          min2 = dcc_nv_item_ptr->min2.min2[NV_CDMA_MIN_INDEX];
          if (qmi_dmsi_minnum_validate(min1, min2))
          {
            min_len = qmi_dmsi_minnum_pcs_to_text(min_num, min1, min2);
            min_num[min_len] = '\0';
          }
        }
      }
    }
    else
    {
      errval = QMI_ERR_NOT_PROVISIONED_V01;  
    }

    /*-------------------------------------------------------------------------
      Free the temporary memory allocated for NV item.
    -------------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
  }

  return errval;
} /* qmi_dmsi_get_mdn_min() */

/*===========================================================================
  FUNCTION QMI_DMSI_GENERATE_DEFAULT_MIN()

  DESCRIPTION
    Generates the default MIN (IMSI_M_S1 / IMSI_T_S1) for the device, based on 
    the ESN, that is set when the UI defaults the provisioning.
    
  PARAMETERS
    esn : dword containing the ESN in binary

  RETURN VALUE
    dword MIN in encoded format returned
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dword qmi_dmsi_generate_default_min
(
  dword  esn
)
{
  word zero;     /* Encoding of three zero digits */
  word fourth;   /* Fourth from last decimal digit of the ESN */
  word last3;    /* Last three decimal digits of the ESN */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Encode digits as per JSTD-008 section 2.3.1.1 */
  zero = 1110 - 111;
  last3 = (esn % 1000);
  last3 += ((last3 / 100) == 0) ? 1000 : 0;
  last3 += (((last3 % 100) / 10) == 0) ? 100 : 0;
  last3 += ((last3 % 10) == 0) ? 10 : 0;
  last3 -= 111;
  fourth = ((esn % 10000) / 1000); /* In range 0-9 */
  if (fourth == 0)
  {
    fourth = 10;
  }

  /* Concatenate results and return 24 bit value for imsi_s1 */
  /* Example: esn = 120406
  **      imsi_s1 = 000  0  406
  **  encodes to -> 999  10 395
  **       in hex = 3e7  a  18b
  **    in binary = 1111100111 1010 0110001011
  */
  return (((dword) zero << 14) | (fourth << 10) | last3);
} /* qmi_dmsi_generate_default_min() */

/*===========================================================================
  FUNCTION QMI_DMSI_MINNUM_VALIDATE()

  DESCRIPTION
    validate MIN NV format to make sure no reserved areas are set and that the 
    MIN values are within the propper range
    
  PARAMETERS
    min1 : lower 24 bits of the MIN binary representation
    min2 : upper 10 bits of the MIN binary representation

  RETURN VALUE
    Boolean whether the specified MIN is valid according to the binary MIN 
    encoding format.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_dmsi_minnum_validate
(
  dword  min1,
  word   min2
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( (   min1 & 0xFF000000 ) != 0 ) ||
       ( ( ( min1 & 0x00FFC000 ) >> 14 ) > 999 ) ||
       ( ( ( min1 & 0x00003C00 ) >> 10 ) > 10 ) ||
       ( ( ( min1 & 0x00003C00 ) >> 10 ) == 0 ) ||
       ( ( ( min1 & 0x000003FF )         > 999 ) ) 
     ||
       ( ( min2 & 0xFC00 ) != 0) || 
       ( ( min2 & 0x03FF)          > 999 )
     )
  {
    return FALSE;
  }
  
  return TRUE;
} /* qmi_dmsi_minnum_validate() */


/*===========================================================================
  FUNCTION QMI_DMSI_MINNUM_PCS_TO_TEXT()

  DESCRIPTION
    convert MIN NV format to ascii-text representation
    
  PARAMETERS
    dest : will contain the mobile number in ascii-text format
    min1 : lower 24 bits of the MIN binary representation
    min2 : upper 10 bits of the MIN binary representation

  RETURN VALUE
    length of ascii-text string that was created
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static int qmi_dmsi_minnum_pcs_to_text
(
  char *  dest, 
  dword  min1,
  word   min2
)
{
  byte * start;
  byte * end;
  dword   temp;
  boolean digit_one_zero;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(dest != NULL);

  start = end = (byte *) dest;

  /*-------------------------------------------------------------------------
    MIN binary format is as per JSTD-008 section 2.3.1.1, convert to BCD per
    spec
  -------------------------------------------------------------------------*/
  /* mask off and convert first three digits from MIN2 */
  temp = (min2 & 0x03FF);
  digit_one_zero = FALSE;
  /* add 100 and check for overflows from the 4th digit */
  temp += 100;
  if (temp > 999)
  {
    temp -= 1000;
    end[0] = (char) '0';
    end++;
    digit_one_zero = TRUE;
  }
  /* add 10 and check for overflows from the 5th digit */
  temp += 10;
  if ((temp % 10) == (temp % 100))
  {
    temp -= 100;
    if (digit_one_zero)
    {
    end[0] = (char) '0';
    end++;
    }
  }
  /* add 1 and check for overflows from the 6th digit */
  temp += 1;
  if (0 == (temp % 10))
  {
    temp -= 10;
  }
  end = dsatutil_itoa( temp, end, DMSI_BASE_DECIMAL);

  /* mask off and convert next three digits from MIN1 */
  temp = (min1 & 0x00FFC000) >> 14;
  digit_one_zero = FALSE;
  /* add 100 and check for overflows from the 4th digit */
  temp += 100;
  if (temp > 999)
  {
    temp -= 1000;
    end[0] = (char) '0';
    end++;
    digit_one_zero = TRUE;
  }
  /* add 10 and check for overflows from the 5th digit */
  temp += 10;
  if ((temp % 10) == (temp % 100))
  {
    temp -= 100;
    if (digit_one_zero)
    {
    end[0] = (char) '0';
    end++;
    }
  }
  /* add 1 and check for overflows from the 6th digit */
  temp += 1;
  if (0 == (temp % 10))
  {
    temp -= 10;
  }
  end = dsatutil_itoa( temp, end, DMSI_BASE_DECIMAL);

  /* mask off and convert next digit from MIN1 */
  temp = (min1 & 0x00003C00) >> 10;
  /* check for overflows from the 7th digit */
  if (temp == 10)
  {
    temp = 0;
  }
  end = dsatutil_itoa( temp, end, DMSI_BASE_DECIMAL);

  /* mask off and convert last three digits from MIN1 */
  temp = (min1 & 0x000003FF);
  digit_one_zero = FALSE;
  /* add 100 and check for overflows from the 4th digit */
  temp += 100;
  if (temp > 999)
  {
    temp -= 1000;
    end[0] = (char) '0';
    end++;
    digit_one_zero = TRUE;
  }
  /* add 10 and check for overflows from the 5th digit */
  temp += 10;
  if ((temp % 10) == (temp % 100))
  {
    temp -= 100;
    if (digit_one_zero)
    {
    end[0] = (char) '0';
    end++;
    }
  }
  /* add 1 and check for overflows from the 6th digit */
  temp += 1;
  if (0 == (temp % 10))
  {
    temp -= 10;
  }
  end = dsatutil_itoa( temp, end, DMSI_BASE_DECIMAL);

  /* return the length (end - start) given in number of bytes */
  return ((end - start) / sizeof(byte));
} /* qmi_dmsi_minnum_pcs_to_text() */

/*===========================================================================
  FUNCTION QMI_DMSI_MOBNUM_PCS_TO_TEXT()

  DESCRIPTION
    convert MDN NV format to ascii-text representation
    
  PARAMETERS
    dest : will contain the mobile number in ascii-text format
    src  : mobile number in NV format

  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean  qmi_dmsi_mobnum_text_to_pcs
(
  char *                  src,
  uint8                   len,
  nv_mob_dir_data_type *  dest
)
{
  int i;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(dest && src);

  if ((len == 0) || (len > NV_DIR_NUMB_PCS_SIZ))
  {
  	LOG_MSG_INFO1_0("Invalid MDN length");
  	return FALSE;
  }
  else
  {
    dest->n_digits = len;
  }

  /*-------------------------------------------------------------------------
    MDN NV format is sort of like BCD, but 8 bits instead of 4, and 
    '0' is represented as 0x0A.
  -------------------------------------------------------------------------*/
  for (i=0; i < len; i++)
  {
    if ('0' == src[i]) 
    {
      dest->digitn[i] = 0x0A;
    } 
    else if ( (src[i] >= '1') || (src[i]  <= '9'))
    {
      dest->digitn[i] = (byte) src[i] - '0';
    } 
    else 
    {
      return FALSE; /* invalid num */
    }
  }

  return TRUE;
} /* qmi_dmsi_mobnum_text_to_pcs() */

/*===========================================================================
  FUNCTION QMI_DMSI_MIN_TEXT_TO_MIN12()

  DESCRIPTION
    convert MDN NV format to ascii-text representation
    
  PARAMETERS
    min_data : (IN)  poitner to text-format MIN
    len      : (IN)  length of text-format MIN
    min1_ptr : (OUT) pointer to binary-format MIN1
    min2_ptr : (OUT) pointer to binary-format MIN2

  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean  qmi_dmsi_min_text_to_min12
(
  char *          min_data,
  uint8           len,
  dword *				  min1_ptr, 
  word *				  min2_ptr
)
{
  int       i;
  uint16    digit;
  char *    min_text;
  dword		  min1;
  word 		  min2;

  if ((NULL == min_data) || (NULL == min1_ptr) || (NULL == min2_ptr))
  {
    LOG_MSG_ERROR_INT_0("NULL ptr passed");
    return FALSE;
  }

  min_text = min_data;  
  /* Validate MIN length and content */
  if(len != DMSI_MIN_SIZ)
  {
    LOG_MSG_ERROR_INT_0("Invalid MIN length");
    return FALSE;
  }
  else
  {
    for (i=0; i<len; i++ ) 
    {
      if ( (*(min_text+i) <  '0') || (*(min_text+i) > '9') ) 
        return FALSE;
    }
  }

  // Encode the first three digits (IS-95A 6.3.1.1)
  min2 = 0;
  for (i = 0; i < 3; i++) 
  {
    digit = *min_text - '0';
    if (0 == digit) 
      digit = 10;
    min_text++;
    min2 = (uint16) (min2 * 10) + digit;
  }

  min2 -= 111;

  // Encode the last seven digits (IS-95A 6.3.1.1)

  // Encode the second three digits into the ten most
  // significant bits (of the 24-bit number)...
  min1 = 0;
  for (i = 0; i < 3; i++) 
  {
    digit = *min_text - (uint16) '0';
    if (0 == digit) 
      digit = 10;
    min_text++;
    min1 = (uint32) (min1 * 10) + digit;
  }

  min1 -= 111;

  min1 <<= 14;
  min1 &= 0x00FFC000;

  // The fourth last digit is mapping as BCD into four bits
  digit = *min_text - '0';
  if (0 == digit) 
    digit = 10;
  min_text++;

  min1 = min1 | (0x00003C00 & (digit << 10));

  // Encode the last three digits into the ten least significant bits
  {
    uint32 tmp = 0;
    for (i = 0; i < 3; i++) 
    {
      digit = *min_text - '0';
      if (0 == digit) 
        digit = 10;
      min_text++;

      tmp = (uint32) (tmp * 10) + digit;
    }

    tmp -= 111;
    tmp &= 0x000003FF;

    min1 |= tmp;
  }

  // Update the min1_ptr and min2_ptr
  *min1_ptr = min1;
  *min2_ptr = min2;
  return TRUE;
} /* qmi_dmsi_min_text_to_min12() */

/*===========================================================================
  FUNCTION QMI_DMSI_RESET_IMSI_COMPONENTS()

  DESCRIPTION
    Resets the IMSI components for restore factory operation
    
  PARAMETERS
    nam

  RETURN VALUE
    TRUE/FALSE
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean  qmi_dmsi_reset_imsi_components(byte nam)
{
  nv_item_type      *dcc_nv_item_ptr;
  uint16             sub_id;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    return FALSE;
  }
  
  sub_id = (uint16)qmi_if_get_current_cdma_sub_id();
  /* NV_IMSI_MCC_I*/
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  dcc_nv_item_ptr->imsi_mcc.nam = nam;
  dcc_nv_item_ptr->imsi_mcc.imsi_mcc = DMSI_IMSI_MCC_0;      
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_IMSI_MCC_I, dcc_nv_item_ptr, sub_id) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /* NV_IMSI_11_12_I */
  /* Default MNC is 00 in ASCII and 99 in min */
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  dcc_nv_item_ptr->imsi_11_12.nam = nam;
  dcc_nv_item_ptr->imsi_11_12.imsi_11_12 = DMSI_IMSI_11_12_0;  /* default to '00' */
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_IMSI_11_12_I, dcc_nv_item_ptr, sub_id ) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /* NV_MIN2_I */
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  dcc_nv_item_ptr->min2.nam = nam;
  dcc_nv_item_ptr->min2.min2[DMSI_FMMIN] = DMSI_IMSI_S2_0 ;
  dcc_nv_item_ptr->min2.min2[DMSI_CDMAMIN] = DMSI_IMSI_S2_0 ;
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_MIN2_I, dcc_nv_item_ptr, sub_id ) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /* NV_MIN1_I */
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  dcc_nv_item_ptr->min1.nam = nam;
  dcc_nv_item_ptr->min1.min1[DMSI_FMMIN] = DMSI_IMSI_S1_0 ;
  dcc_nv_item_ptr->min1.min1[DMSI_CDMAMIN] = DMSI_IMSI_S1_0;
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_MIN1_I, dcc_nv_item_ptr, sub_id ) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
  
  /* All IMSI components reset as required here, return true */
  return TRUE;

}/* qmi_dmsi_reset_imsi_components() */

#ifdef FEATURE_IS95B_TRUE_IMSI
/*===========================================================================
  FUNCTION QMI_DMSI_RESET_IMSI_T_COMPONENTS()

  DESCRIPTION
    Resets the IMSI_T components for restore factory operation
    
  PARAMETERS
    nam

  RETURN VALUE
    TRUE/FALSE
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean  qmi_dmsi_reset_imsi_t_components(byte nam)
{
  nv_item_type      *dcc_nv_item_ptr;
  uint16				  sub_id;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    return FALSE;
  }
  
  sub_id = (uint16)qmi_if_get_current_cdma_sub_id();
  /* NV_IMSI_T_MCC_I */
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  /* Default MCC is 000 in ASCII and 999 in min */
  dcc_nv_item_ptr->imsi_t_mcc.nam = nam;
  dcc_nv_item_ptr->imsi_t_mcc.imsi_mcc = DMSI_IMSI_MCC_0;      /* default to '000' */
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_IMSI_T_MCC_I, dcc_nv_item_ptr, sub_id ) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /* NV_IMSI_T_11_12_I */
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  /* Default MNC is 00 in ASCII and 99 in min */
  dcc_nv_item_ptr->imsi_t_11_12.nam = nam;
  dcc_nv_item_ptr->imsi_t_11_12.imsi_11_12 = DMSI_IMSI_11_12_0;  /* default to '00' */
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_IMSI_T_11_12_I, dcc_nv_item_ptr, sub_id ) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /* NV_IMSI_T_S2_I */
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  /* Default S2 is 000 in ASCII and 999 in min */
  dcc_nv_item_ptr->imsi_t_s2.nam = nam;
  dcc_nv_item_ptr->imsi_t_s2.min2[DMSI_FMMIN] = DMSI_IMSI_S2_0 ;
  dcc_nv_item_ptr->imsi_t_s2.min2[DMSI_CDMAMIN] = DMSI_IMSI_S2_0 ;
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_IMSI_T_S2_I, dcc_nv_item_ptr, sub_id ) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /* NV_IMSI_T_S1_I */
  memset(dcc_nv_item_ptr, 0, sizeof(nv_item_type));
  dcc_nv_item_ptr->imsi_t_s1.nam = nam;
  dcc_nv_item_ptr->imsi_t_s1.min1[DMSI_FMMIN] = DMSI_IMSI_S1_0 ;
  dcc_nv_item_ptr->imsi_t_s1.min1[DMSI_CDMAMIN] =  DMSI_IMSI_S1_0 ;
  if( NV_DONE_S != dcc_put_nv_item_ext( NV_IMSI_T_S1_I, dcc_nv_item_ptr, sub_id ) )
  {
    PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    Free the temporary memory allocated for NV item.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);;

  /* All IMSI_T components reset as required here, return true */
  return TRUE;

}/* qmi_dmsi_reset_imsi_t_components() */
#endif /* def FEATURE_IS95B_TRUE_IMSI */


/*===========================================================================
  FUNCTION QMI_DMSI_CALCULATE_ACTIVATED_STATE()

  DESCRIPTION
    convert MDN NV format to ascii-text representation
    
  PARAMETERS
    dest : will contain the mobile number in ascii-text format
    src  : mobile number in NV format

  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dms_activation_state_enum_v01 qmi_dmsi_calculate_activated_state
(
  void
)
{
  dms_activation_state_enum_v01 act_state;
  
  dword                            min1;
  dword                            min1_default;
  boolean                          otasp_state;
  nv_item_type                    *dcc_nv_item_ptr;
  nv_stat_enum_type                nv_status;
  cm_nam_e_type                    curr_nam;
  uint16                           sub_id;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  act_state = DMS_ACTIVATION_STATE_ENUM_MAX_ENUM_VAL_V01;
  
  otasp_state = FALSE;
  min1 = min1_default = 0;

  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    return act_state;
  }

  /* see if build stores 'last' OTASP state to QMI NV item */
  if (qmi_dmsi_global.nv_use_last_otasp_act_state)
  {
    /* start with NOT_ACTIVATED in case 'last' NV not set */
    act_state = DMS_ACTIVATION_NOT_ACTIVATED_V01;
    if (QMI_NV_STATUS_OK == qmi_nv_read(QMI_NV_ITEM_LAST_OTASP_STATE, 0, 0, 
                                        &otasp_state, 
                                        sizeof(otasp_state)))
    {
      /* use 'last' NV only if set and non-zero */
      if (otasp_state)
      {
        act_state = DMS_ACTIVATION_ACTIVATED_V01;
      }
    }
  }

   sub_id = (uint16)qmi_if_get_current_cdma_sub_id();

  /* if act_state not set, use the ESN/MIN to determine */
  if (DMS_ACTIVATION_STATE_ENUM_MAX_ENUM_VAL_V01 == act_state)
  {
    act_state = DMS_ACTIVATION_NOT_ACTIVATED_V01;
    if(NV_DONE_S == dcc_get_nv_item_ext (NV_ESN_I, dcc_nv_item_ptr, sub_id))
    {
      min1_default = qmi_dmsi_generate_default_min(dcc_nv_item_ptr->esn.esn);
    }
  
    nv_status = dcc_get_nv_item(NV_CURR_NAM_I, dcc_nv_item_ptr);  
    if(nv_status != NV_DONE_S || dcc_nv_item_ptr->curr_nam >= NV_MAX_NAMS)
    {
      /*-----------------------------------------------------------------------
        Default to the first NAM
      -----------------------------------------------------------------------*/
      dcc_nv_item_ptr->curr_nam = (byte) CM_NAM_1;
    }
    curr_nam = (cm_nam_e_type)dcc_nv_item_ptr->curr_nam;
    dcc_nv_item_ptr->min1.nam = curr_nam;

    if (NV_DONE_S == dcc_get_nv_item_ext(NV_MIN1_I, dcc_nv_item_ptr, sub_id))
    {
      min1 = dcc_nv_item_ptr->min1.min1[NV_CDMA_MIN_INDEX];
      if (qmi_dmsi_minnum_validate(min1, 0) && (min1 != min1_default))
      {
        act_state = DMS_ACTIVATION_ACTIVATED_V01;
      }
      else
      {
#ifdef FEATURE_IS95B_TRUE_IMSI
        dcc_nv_item_ptr->imsi_t_s1.nam = curr_nam;
        if (NV_DONE_S == dcc_get_nv_item_ext(NV_IMSI_T_S1_I, dcc_nv_item_ptr, sub_id))
        {
          min1 = dcc_nv_item_ptr->imsi_t_s1.min1[NV_CDMA_MIN_INDEX];
          if (qmi_dmsi_minnum_validate(min1, 0) && (min1 != min1_default))
          {
            act_state = DMS_ACTIVATION_ACTIVATED_V01;
          }
        }
#endif /* FEATURE_IS95B_TRUE_IMSI */
      }
    }
  }

  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);

  return act_state;
} /* qmi_dmsi_calculate_activated_state */
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
/*===========================================================================
  FUNCTION QMI_DMSI_BIND_SUBSCRIPTION()

  DESCRIPTION
    Binds user given subscription to this client

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.


  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_bind_subscription
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_dmsi_client_info_type *  dms_client_sp;
  dms_bind_subscription_req_msg_v01 *req_msg;
  dms_bind_subscription_resp_msg_v01 resp_msg;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);

  dms_client_sp = (qmi_dmsi_client_info_type *)clnt_info;
  req_msg = (dms_bind_subscription_req_msg_v01*)req_c_struct;
  memset(&resp_msg,0,sizeof(dms_bind_subscription_resp_msg_v01));

#ifndef FEATURE_DUAL_SIM
#define SUBS_ID_SECONDARY 2
  if(req_msg->bind_subs == SUBS_ID_SECONDARY)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
    goto send_result;
  }
#endif


#ifndef FEATURE_TRIPLE_SIM
#define SUBS_ID_TERTIARY 3
  if(req_msg->bind_subs == SUBS_ID_TERTIARY)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
    goto send_result;
  }
#endif

  if(!((req_msg->bind_subs < (DMSI_BIND_SUBS_MAX+1)) && (req_msg->bind_subs != 0)))
  {
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  dms_client_sp->subscription_id = (qmi_dms_client_subs_e_type)(req_msg->bind_subs - 1) ;
    
  send_result:
    result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                     : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_bind_subscription_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;


} /* qmi_dmsi_bind_subscription */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_BIND_SUBSCRIPTION()

  DESCRIPTION
    Returns already bound  subscription to this client

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_bind_subscription
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_dmsi_client_info_type *  dms_client_sp = NULL ;
  uint32 subscription_id;
  dms_get_bind_subscription_resp_msg_v01 resp_msg;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT(clnt_info != NULL);

  memset(&resp_msg,0,sizeof(dms_get_bind_subscription_resp_msg_v01));
  dms_client_sp = (qmi_dmsi_client_info_type *)clnt_info;
  
  if(  dms_client_sp->subscription_id >= DMSI_BIND_SUBS_MAX)
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  subscription_id = (dms_client_sp->subscription_id) + 1;
  resp_msg.bind_subscription_valid = TRUE;
  resp_msg.bind_subscription = subscription_id;
  send_result:
   result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                    : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_bind_subscription_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_get_bind_subscription */


/*===========================================================================
  FUNCTION QMI_DMSI_OPRT_MODE_FROM_CM()

  DESCRIPTION
    Maps a CM operating mode enum to a QMI DMS operating mode enum.
    
  PARAMETERS
    cm_oprt_mode  [in]
    dms_oprt_mode [out]

  RETURN VALUE
    QMI_ERR_NONE_V01 on success, QMI_ERR_INTERNAL_V01 if CM operating mode is
    unrecognized.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_oprt_mode_from_cm
(
  sys_oprt_mode_e_type        cm_oprt_mode,
  dms_operating_mode_enum_v01  *dms_oprt_mode
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  nv_stat_enum_type  nv_status;
  nv_item_type      *dcc_nv_item_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Allocate temporary memory for the NV item
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dcc_nv_item_ptr, sizeof(nv_item_type),
                           nv_item_type*);
  if( dcc_nv_item_ptr == NULL )
  {
    return QMI_ERR_NO_MEMORY_V01;
  }

  switch (cm_oprt_mode)
  {
    case SYS_OPRT_MODE_NONE:
      errval = QMI_ERR_INFO_UNAVAILABLE_V01;
      break;

    case SYS_OPRT_MODE_PWROFF:
      *dms_oprt_mode = DMS_OP_MODE_SHUTTING_DOWN_V01;
      break;
  
    case SYS_OPRT_MODE_FTM:
      *dms_oprt_mode = DMS_OP_MODE_FACTORY_TEST_MODE_V01;
      break;
  
    case SYS_OPRT_MODE_OFFLINE:
    case SYS_OPRT_MODE_OFFLINE_AMPS:
    case SYS_OPRT_MODE_OFFLINE_CDMA:
      *dms_oprt_mode = DMS_OP_MODE_OFFLINE_V01;
      break;

    case SYS_OPRT_MODE_ONLINE:
      *dms_oprt_mode = DMS_OP_MODE_ONLINE_V01;
      break;
  
    case SYS_OPRT_MODE_LPM:
      *dms_oprt_mode = DMS_OP_MODE_LOW_POWER_V01;
      break;

    case SYS_OPRT_MODE_RESET:
      *dms_oprt_mode = DMS_OP_MODE_RESETTING_V01;
      break;

    case SYS_OPRT_MODE_NET_TEST_GW:
      *dms_oprt_mode = DMS_OP_MODE_NET_TEST_GW_V01;
      break;

    default:
      LOG_MSG_INFO2_1("Operating mode unknown (%d), can't convert to QMI DMS oprt mode",
                      cm_oprt_mode);
      errval = QMI_ERR_INTERNAL_V01;
      break;
  }

  /*-------------------------------------------------------------------------
    Check if the device is actually in P-LPM instead of LPM
  -------------------------------------------------------------------------*/
  if ( QMI_ERR_NONE_V01 == errval &&
       DMS_OP_MODE_LOW_POWER_V01 == *dms_oprt_mode )
  {
    /* Read the nv for LPM persistence flag */
    nv_status = dcc_get_nv_item(NV_LPM_PERSISTENCE_I, dcc_nv_item_ptr);
    if(NV_DONE_S == nv_status)
    {
      /* P-LPM flag set, read the value */
      if (dcc_nv_item_ptr->lpm_persistence)
      {
        *dms_oprt_mode = DMS_OP_MODE_PERSISTENT_LOW_POWER_V01;
      }
    }
  } 

  PS_SYSTEM_HEAP_MEM_FREE(dcc_nv_item_ptr);
  return errval;
} /* qmi_dmsi_oprt_mode_from_cm() */

#ifdef FEATURE_MMGSDI_SESSION_LIB
/*===========================================================================
  FUNCTION QMI_DMSI_GET_UIM_STATUS()

  DESCRIPTION
    Returns the status of the UIM based on the values cached from earlier
    MMGSDI events.

  PARAMETERS
    None

  RETURN VALUE
    QMI_ERR_NONE_V01: UIM present and unlocked
    QMI_ERR_SIM_NOT_INITIALIZED: If the SIM is not initialized yet
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_dmsi_get_uim_status
(
  void
)
{
  qmi_if_info_type  *qmi_if_info_ptr;

  qmi_if_info_ptr = qmi_if_info_get_ptr();
  LOG_MSG_INFO2_2("qmi_dmsi_get_uim_status(): app_type=0x%x, uim_state=0x%x",
                  qmi_if_info_ptr->session_info[QMI_IF_MMGSD_GW_PROV_PRI_SESSION].app_type,
                  qmi_dmsi_global.uim_state);

  switch (qmi_dmsi_global.uim_state)
  {
    case DMS_UIM_INITIALIZATION_COMPLETED_V01: // i.e. subscription ready ev occured
      return QMI_ERR_NONE;

    case DMS_UIM_NOT_PRESENT_V01: // i.e. sim not present
    case DMS_UIM_STATE_UNAVAILABLE_V01: // i.e. no mmgsdi event yet
      return QMI_ERR_SIM_NOT_INITIALIZED;

    default:
      return QMI_ERR_NONE;
  }
} /* qmi_dmsi_get_uim_status */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_UIM_STATUS_EXT()

  DESCRIPTION
    Returns the status of the UIM based on the values fetched from mmgsdi module.

  PARAMETERS
    None

  RETURN VALUE
    QMI_ERR_NONE_V01: UIM present and unlocked
    QMI_ERR_SIM_NOT_INITIALIZED: If the SIM is not initialized yet
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_dms_get_uim_access_status_ext
(
   uint8 session_type
)
{
  mmgsdi_session_info_query_type    session_info_query;
  mmgsdi_session_get_info_type      session_get_info_ptr;
  mmgsdi_return_enum_type  mmgsdi_status;
  session_info_query.query_type = MMGSDI_SESSION_INFO_QUERY_BY_TYPE;
  session_info_query.query_key.session_type = (mmgsdi_session_type_enum_type)session_type;
  
  mmgsdi_status = mmgsdi_session_get_info_sync(session_info_query, &session_get_info_ptr);
  
  switch(mmgsdi_status)
  {
    case MMGSDI_SUCCESS:
      switch(session_get_info_ptr.app_state)
      {
        case MMGSDI_APP_STATE_READY:
        case MMGSDI_APP_STATE_ILLEGAL:
          return QMI_ERR_NONE;
        default:
          return QMI_ERR_SIM_NOT_INITIALIZED;
      }
    default:
      return QMI_ERR_SIM_NOT_INITIALIZED;
  }      
}
    

/*===========================================================================
  FUNCTION QMI_DMS_GET_UIM_ACCESS_STATUS()

  DESCRIPTION
    Returns the status of the UIM for read/write of PIN1 protected values based
    on the values cached from earlier MMGSDI events.  

  PARAMETERS
    None

  RETURN VALUE
    QMI_ERR_NONE_V01: UIM present and unlocked
    QMI_ERR_SIM_NOT_INITIALIZED: If the UIM is not initialized yet
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_dms_get_uim_access_status
(
  void
)
{
  qmi_error_e_type errval = QMI_ERR_INTERNAL;


  errval = qmi_dmsi_get_uim_status();
  if (QMI_ERR_NONE == errval)
  {
      if (DMS_UIM_INITIALIZATION_FAILED_V01 == qmi_dmsi_global.uim_state)
      {
        /*UIM not ready to read/write*/
        errval = QMI_ERR_SIM_NOT_INITIALIZED;
      }
  }

  return errval;
} /* qmi_dms_get_uim_access_status */

#endif /* FEATURE_MMGSDI_SESSION_LIB */

/*===========================================================================
  FUNCTION QMI_DMS_GENERATE_PRL_INIT_IND()

  DESCRIPTION
    Called when CM notifies us that the PRL is loaded onto device. Sends
    an indication with the PRL init TLV.
        
  PARAMETERS
    None
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_generate_prl_init_ind
(
  void
)
{
  qmi_dmsi_client_info_type *    cl_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!qmi_dmsi_inited)
  {
    return;
  }
  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);
  while(cl_sp != NULL)
  {
    if( cl_sp->report_status.report_prl_init)
    {
      qmi_dmsi_event_report_ind(cl_sp,
                                  DMSI_REPORT_STATUS_EV_PRL_INIT,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0);
      }
      cl_sp = q_next(&qmi_dms_state.client_q,
                           &(cl_sp->link));
    }
} /* qmi_dms_generate_prl_init_ind() */

/*===========================================================================
  FUNCTION QMI_DMSI_SET_TEST_CONFIG_REQ()

  DESCRIPTION
    Set TDSCDMA test config request
        
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_set_test_config_req
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_set_test_config_req_msg_v01* req_msg;
  dms_set_test_config_resp_msg_v01 resp_msg;
  ds_qmi_dms_async_cmd_type *async_cmd_p = NULL;
#ifdef FEATURE_TDSCDMA
  boolean retval = FALSE;
  tds_rrc_set_config_req_type set_config_req;
#endif
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT(req_c_struct != NULL);
 
  req_msg = (dms_set_test_config_req_msg_v01*)req_c_struct;
  LOG_MSG_INFO1_1("qmi_dmsi_set_test_config_req(): Set TDSCDMA config req %d",
                  req_msg->tds_config_valid);
  if (FALSE == req_msg->tds_config_valid)
  {
    errval = QMI_ERR_MISSING_ARG_V01;
   
    goto send_result;
  }

#ifdef FEATURE_TDSCDMA
  /*-------------------------------------------------------------------------
    Populate the local structure with the values coming in the request and
    also validate the values
  -------------------------------------------------------------------------*/

  switch (req_msg->tds_config)
  {
    case DMS_TEST_CONFIG_TDS_PRODUCTION_V01:
      set_config_req.config = TDS_CONFIG_FIELD;
      break;
    case DMS_TEST_CONFIG_TDS_LAB_V01:
      set_config_req.config = TDS_CONFIG_LAB;
      break;
    case DMS_TEST_CONFIG_TDS_USER_V01:
      set_config_req.config = TDS_CONFIG_USER;
      break;
    default:
    {
      /* Any other value is considered invalid */
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
      break;
    }
  }

  
  /* Now call the MSGRT send routine */
  retval = ds3gmsgr_msg_send(
            TDSCDMA_RRC_SET_CONFIG_REQ, 
            MSGR_DS_MSGRRECV,
            (msgr_hdr_struct_type*)(&set_config_req),
            sizeof(tds_rrc_set_config_req_type));


  if (retval == FALSE)
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  PS_SYSTEM_HEAP_MEM_ALLOC(async_cmd_p,sizeof(ds_qmi_dms_async_cmd_type),
                           ds_qmi_dms_async_cmd_type*);
  if(async_cmd_p == NULL)
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  memset(async_cmd_p,0,sizeof(ds_qmi_dms_async_cmd_type));
  async_cmd_p->cl_sp = clnt_info;
  async_cmd_p->msg_id = msg_id;
  async_cmd_p->req_handle = req_handle;
  q_put(&(qmi_dms_state.set_tdscdma_config_pending_q),
        &(((ds_qmi_dms_async_cmd_type*)async_cmd_p)->link));
#else
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED_V01;
#endif

send_result:
  if (errval != QMI_ERR_NONE_V01)
  {
    if(async_cmd_p != NULL)
    {
      PS_SYSTEM_HEAP_MEM_FREE(async_cmd_p);
    }
    result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error = errval;
    resp_msg.resp.result = result;
    qmi_dmsi_send_response(req_handle,
                           msg_id,
                           &resp_msg,
                           sizeof(dms_set_test_config_resp_msg_v01));
  }
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_set_test_config_req() */


/*===========================================================================
  FUNCTION QMI_DMSI_GET_TEST_CONFIG_REQ()

  DESCRIPTION
    Get TDSCDMA test config request
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_test_config_req
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_test_config_resp_msg_v01 resp_msg;
  qmi_error_type_v01            errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01           result = QMI_RESULT_SUCCESS_V01;
#ifdef FEATURE_TDSCDMA
  ds_qmi_dms_async_cmd_type* async_cmd_p = NULL;
  boolean retval = FALSE;
  tds_rrc_get_config_req_type get_config_req;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&resp_msg,0,sizeof(dms_get_test_config_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Perform query action
  -------------------------------------------------------------------------*/

#ifdef FEATURE_TDSCDMA
  memset(&get_config_req, 0, sizeof(tds_rrc_get_config_req_type));

  retval = ds3gmsgr_msg_send(
            TDSCDMA_RRC_GET_CONFIG_REQ, 
            MSGR_DS_MSGRRECV,
            (msgr_hdr_struct_type*)(&get_config_req),
            sizeof(tds_rrc_get_config_req_type));

  if(retval == FALSE)
  {
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  PS_SYSTEM_HEAP_MEM_ALLOC(async_cmd_p,sizeof(ds_qmi_dms_async_cmd_type),
                           ds_qmi_dms_async_cmd_type*);
  if(async_cmd_p == NULL)
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  memset(async_cmd_p,0,sizeof(ds_qmi_dms_async_cmd_type));
  async_cmd_p->cl_sp = clnt_info;
  async_cmd_p->msg_id = msg_id;
  async_cmd_p->req_handle = req_handle;
  q_put(&(qmi_dms_state.get_tdscdma_config_pending_q),
            &(((ds_qmi_dms_async_cmd_type*)async_cmd_p)->link));
  return QMI_CSI_CB_NO_ERR;
send_result:

  LOG_MSG_INFO1_1("qmi_dmsi_get_test_config_req(): "
                  "dms query tdscdma test get config exit with err %d",
                  errval);

  if (errval != QMI_ERR_NONE_V01)
  {
    if(async_cmd_p != NULL)
    {
      PS_SYSTEM_HEAP_MEM_FREE(async_cmd_p);
    }
    result = QMI_RESULT_FAILURE_V01;
  }
#else
  if (errval != QMI_ERR_NONE_V01)
  {
    errval = QMI_ERR_NONE_V01;
    result = QMI_RESULT_SUCCESS_V01;
  }
#endif
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_test_config_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
 
} /* qmi_dmsi_get_test_config_req() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_OEM_CHINA_OPERATOR()

  DESCRIPTION
    Gives the operator info of the device configured by OEM.
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_dmsi_get_oem_china_operator
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_oem_china_operator_resp_msg_v01 resp_msg;
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;

 #define OPERATOR_F_NAME \
                           (char*)"/nv/item_files/modem/mmode/operator_name"
  int efs_ret;
  struct fs_stat temp_buf;
  uint8 buf ;
  uint32 oem_china_operator;
  mcfg_fs_e_type        file_type = MCFG_FS_TYPE_EFS;
  mcfg_fs_sub_id_e_type mcfg_subs_id = MCFG_FS_SUBID_0;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* The subcription ID to which client is bound. */
  mcfg_subs_id = (mcfg_fs_sub_id_e_type)clnt_info->subscription_id;
  
  /*EFS SUCCESS == 0*/
  if((mcfg_fs_stat(OPERATOR_F_NAME, &temp_buf, file_type, mcfg_subs_id) != MCFG_FS_STATUS_OK))
  {
    errval = QMI_ERR_INFO_UNAVAILABLE_V01;
    goto send_result;
  }
  else  
  {
    efs_ret = mcfg_fs_read(OPERATOR_F_NAME, &buf, sizeof(uint8), file_type, mcfg_subs_id);

    if (efs_ret != MCFG_FS_STATUS_OK)
    {
      errval = QMI_ERR_NOT_PROVISIONED_V01;
      goto send_result;
    }
    
    /* The actual NV values start from 0 (OPERATOR_NONE but since we are only 
       reporting CT/CU/CMCC + 1 is needed here.
    */
    if((buf >=  DMS_OPERATOR_MAX + 1) || (buf < DMS_OPERATOR_CT + 1))
    {
      errval = QMI_ERR_INFO_UNAVAILABLE_V01;
      goto send_result;
    }
  
    oem_china_operator = buf - 1;
  }
  resp_msg.operator_info_valid = TRUE;
  resp_msg.operator_info = oem_china_operator;
  
  send_result:
   result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                    : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_oem_china_operator_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

}/* qmi_dmsi_get_oem_china_operator */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_MAC_ADDRESS()

  DESCRIPTION
    Get the MAC address configured in NV for a device

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_mac_address
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_mac_address_req_msg_v01* req_msg;
  dms_get_mac_address_resp_msg_v01 resp_msg;
  qmi_error_type_v01          errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01         result;
  nv_stat_enum_type  nv_status;
  uint8 index ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT(req_c_struct != NULL);
  memset(&resp_msg, 0, sizeof(dms_get_mac_address_resp_msg_v01));
  req_msg = (dms_get_mac_address_req_msg_v01*)req_c_struct;

  if( DMS_DEVICE_MAX <= req_msg->device )
  {
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  memset((void *)&dcc_nv_item, 0, sizeof(dcc_nv_item));

  if( DMS_DEVICE_MAC_WLAN_V01 == req_msg->device )
  {
    nv_status = dcc_get_nv_item(NV_WLAN_MAC_ADDRESS_I, &dcc_nv_item);
    
    if(nv_status == NV_DONE_S)
    { 
      resp_msg.mac_address_valid = TRUE;
      for(index =0; index < DMSI_MAC_ADDR_LEN; index++)
      {
        resp_msg.mac_address[index] = dcc_nv_item.wlan_mac_address[index];
      }
      resp_msg.mac_address_len = index;
    }
    else
    {
      errval = QMI_ERR_NOT_PROVISIONED_V01;
      goto send_result;
    }
  }
  else
  {
    nv_status = dcc_get_nv_item(NV_BD_ADDR_I, &dcc_nv_item);

    if(nv_status == NV_DONE_S)
    {
      resp_msg.mac_address_valid = TRUE;
      for(index =0; index < DMSI_MAC_ADDR_LEN ; index++)
      {
        resp_msg.mac_address[index] = dcc_nv_item.bd_addr.bd_addr[index];
      }
      resp_msg.mac_address_len = index;
    }
    else
    {
      errval = QMI_ERR_NOT_PROVISIONED_V01;
      goto send_result;
    }
  }
   

send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_mac_address_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_get_mac_address() */

#ifdef FEATURE_TDSCDMA
/*===========================================================================
  FUNCTION QMI_DMSI_GENERATE_GET_TEST_CONFIG_RESP()

  DESCRIPTION
    Called from get config callback routine. Sends response to the caller 
        
  PARAMETERS
    config_resp   : GET response data
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dmsi_generate_get_test_config_resp
(
  tdscdma_get_config_res_type *config_resp
)
{
  ds_qmi_dms_async_cmd_type*  cmd_buf_p = NULL;
  qmi_error_type_v01          errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01         result;
  dms_get_test_config_resp_msg_v01 resp_msg;
  qmi_dmsi_local_err_type     local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(config_resp != NULL);
  memset(&resp_msg,0,sizeof(dms_get_test_config_resp_msg_v01));
  cmd_buf_p = q_get(&(qmi_dms_state.get_tdscdma_config_pending_q));
 
  if(cmd_buf_p == NULL)
  {
    goto bail;
  }
  
  if( NULL == cmd_buf_p->cl_sp|| NULL == cmd_buf_p->req_handle )
  {
    local_err = QMI_DMSI_LOCAL_ERR_NULL_CLIENT;
    goto bail;
  }

  /*-------------------------------------------------------------------------
    GET TDSCDMA config response
  -------------------------------------------------------------------------*/

  errval = qmi_dmsi_tdscdma_get_config_val(config_resp->desired_config,
                                           &(resp_msg.desired_tds_config));
  if (errval == QMI_ERR_INTERNAL_V01)
  {
    local_err = QMI_DMSI_LOCAL_ERR_TDSCDMA_DESIRED_CONFIG_FAIL;
    goto send_result;
  }
  else
  {
    resp_msg.desired_tds_config_valid = TRUE;
  }

  errval = qmi_dmsi_tdscdma_get_config_val(config_resp->active_config,
                                           &(resp_msg.active_tds_config));
  if (errval == QMI_ERR_INTERNAL_V01)
  {
    local_err = QMI_DMSI_LOCAL_ERR_TDSCDMA_ACTIVE_CONFIG_FAIL;
    goto send_result;
  }
  else
  {
    resp_msg.active_tds_config_valid = TRUE;
  }


send_result: 
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
                                   : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  if(qmi_dms_check_valid_async_cmd_buf(cmd_buf_p))
  {
    qmi_dmsi_send_response(cmd_buf_p->req_handle,
                           cmd_buf_p->msg_id,
                           &resp_msg,
                           sizeof(dms_get_test_config_resp_msg_v01));
  }
  PS_SYSTEM_HEAP_MEM_FREE(cmd_buf_p);
bail:
  LOG_MSG_INFO1_2("qmi_dmsi_generate_get_test_config_resp(): "
                  "cmd_buf ptr = %x exit with local_err %d",
                  ((cmd_buf_p != NULL)?cmd_buf_p:0), 
                  local_err);

} /* qmi_dms_generate_get_tdscdma_config_resp() */

/*===========================================================================
  FUNCTION      QMI_DMSI_TDSCDMA_GET_CONFIG_VAL()

  DESCRIPTION
    Mapping function for TDSCDMA GET/SET test config values
        
  PARAMETERS
    config      Config value to be mapped and return as DMSI enum
    val         Pointer to store the return value
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_type_v01 qmi_dmsi_tdscdma_get_config_val
(
   tds_rrc_config_e_type config, dms_test_config_tds_enum_v01 *val
)
{
  switch (config)
  {
    case TDS_CONFIG_FIELD:
      *val = DMS_TEST_CONFIG_TDS_PRODUCTION_V01;
      break;
    case TDS_CONFIG_LAB:
      *val = DMS_TEST_CONFIG_TDS_LAB_V01;
      break;
    case TDS_CONFIG_USER:
      *val = DMS_TEST_CONFIG_TDS_USER_V01;
      break;
    default:
    {
      /* Any other value is considered invalid */
      LOG_MSG_INFO1_1 ("qmi_dmsi_tdscdma_get_config_val(): "
                       "Invalid GET TDSCDMA config value, config value: %d ",
                       config);
      return QMI_ERR_INTERNAL_V01;
      break;
    }
  }

  return QMI_ERR_NONE_V01;
}

/*===========================================================================
  FUNCTION QMI_DMSI_GENERATE_SET_TEST_CONFIG_RESP()

  DESCRIPTION
    Called from SET TDSCDMA config callback routine.
    Sends response to the caller  for the corresponding TDSCDMA Set config
    request
        
  PARAMETERS
    config_resp : TDSCDMA config response
    
  RETURN VALUE  
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dmsi_generate_set_test_config_resp
(
  tdscdma_set_config_res_type * config_resp
)
{
  ds_qmi_dms_async_cmd_type*     cmd_buf_p = NULL;
  qmi_error_type_v01       errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01      result;
  dms_set_test_config_resp_msg_v01 resp_msg;
  qmi_dmsi_local_err_type  local_err = QMI_DMSI_LOCAL_ERR_NONE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT(config_resp != NULL);
  memset(&resp_msg,0,sizeof(dms_set_test_config_resp_msg_v01));
  cmd_buf_p = q_get(&(qmi_dms_state.set_tdscdma_config_pending_q));
 
  

  if(cmd_buf_p == NULL)
  {
    goto bail;
  }
  
  if( NULL == cmd_buf_p->cl_sp || NULL == cmd_buf_p->req_handle )
  {
    local_err = QMI_DMSI_LOCAL_ERR_NULL_CLIENT;
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Set TDSCDMA set config response
  -------------------------------------------------------------------------*/
  if (config_resp->status != TRUE)
  {
    local_err = QMI_DMSI_LOCAL_ERR_INVAL_CONFIG_STATUS;
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  
send_result: 
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 
  								 : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  if(qmi_dms_check_valid_async_cmd_buf(cmd_buf_p))
  {
    qmi_dmsi_send_response(cmd_buf_p->req_handle, 
                           cmd_buf_p->msg_id,
                           &resp_msg,
                           sizeof(dms_set_test_config_resp_msg_v01));
  }
  PS_SYSTEM_HEAP_MEM_FREE(cmd_buf_p);

bail:

  LOG_MSG_INFO1_2("qmi_dmsi_generate_set_test_config_resp(): cmd_buf ptr = %x"
                  "exit with local_err %d",
                   ((cmd_buf_p != NULL)?cmd_buf_p:0), local_err);
} /* qmi_dms_generate_set_tdscdma_config_resp() */


/*===========================================================================
  FUNCTION QMI_DMSI_TDSCDMA_CONFIG_INIT()

  DESCRIPTION
    This function initializes TDSCDMA Get and Set response queues

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    QMI DMS must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_tdscdma_config_init
(
  void
)
{
  qmi_dmsi_state_info_type *         dms_sp;

  dms_sp = (qmi_dmsi_state_info_type*)&qmi_dms_state;
  
  /* Initialize tdscdma Get and Set response queues */
  q_init( &(dms_sp->get_tdscdma_config_pending_q) );
  q_init( &(dms_sp->set_tdscdma_config_pending_q) );

  dsmsgrrecv_reg_cb(TDSCDMA_RRC_GET_CONFIG_RSP,
                     (dsmsgrrecv_cback_f_ptr) qmi_dmsi_msgr_hdlr,
                     FALSE,
                     NULL);

  dsmsgrrecv_reg_cb(TDSCDMA_RRC_SET_CONFIG_RSP,
                     (dsmsgrrecv_cback_f_ptr) qmi_dmsi_msgr_hdlr,
                     FALSE,
                     NULL);
}
#endif

/*===========================================================================
  FUNCTION qmi_dmsi_policyman_config_init()

  DESCRIPTION  Init function to register with PS Sys module
               for recieving policyman indications
   
  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    
  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_policyman_config_init (void)
{
  int                      sub_ind = 0;
  ps_sys_event_enum_type   event_id   = PS_SYS_EVENT_MIN;
  int16                    result = 0;
  int16                    dss_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*Register for policyman indicaiton*/    
  for (sub_ind = PS_SYS_PRIMARY_SUBS; sub_ind < PS_SYS_SUBS_MAX; sub_ind++)
  {    
    result = ps_sys_event_reg_ex (PS_SYS_TECH_3GPP,
                                  PS_SYS_EVENT_3GPP_POLICYMAN_MSG_IND,
                                  (ps_sys_subscription_enum_type) sub_ind,
                                  dsdi_policyman_msg_cb,
                                  NULL,
                                  &dss_errno);
    if(result < 0)
    {
      event_id = PS_SYS_EVENT_SYSTEM_STATUS_EX;
      break;
    }
  }  
  
  if(result < 0)
  {
    LOG_MSG_ERROR_INT_2("Couldn't reg for ps sys event_id = d Err : (d)",
                    event_id, dss_errno);
    DS_QMI_DMS_ASSERT(0);
    return;
  }  
} /* qmi_dmsi_policyman_config_init() */

/*===========================================================================
  FUNCTION DMSI_POLICYMAN_MSG_CB()

  DESCRIPTION
    This callback is called when ds3g module recieves policyman
    message to be sent to DMS module

  PARAMETERS
    tech_type      : technology for which roaming status has changed
    event_name     : Event name
    subscription_id: Subscription id
    event_info_ptr : DDS switch complete event info
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void dsdi_policyman_msg_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
)
{
  qmi_dmsi_cmd_buf_type           * cmd_ptr = NULL;
  ps_sys_policyman_msg_info_type  * ps_policyman_msg_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_4("dsdi_policyman_msg_cb tech_type %d event_name %d"
                  " event_info_ptr 0x%x Subscription id %d",
                  tech_type, event_name, event_info_ptr,subscription_id);
   
  DS_QMI_DMS_ASSERT(event_info_ptr != NULL);
  
  ps_policyman_msg_ptr = (ps_sys_policyman_msg_info_type *)event_info_ptr;                  
  
  switch (ps_policyman_msg_ptr->policyman_msg) 
  {
    case  PS_SYS_POLICYMAN_DEV_CURR_CAPABILITY:
       /*post a command to DCC*/
        cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
        if( cmd_ptr == NULL)
        {
          break;
        }       
        memset(cmd_ptr, 0, sizeof(qmi_dmsi_cmd_buf_type));
        cmd_ptr->cmd_id = QMI_CMD_DMS_DEV_CURRENT_CAP_CB;
        dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
      break;
    
    default:
      LOG_MSG_ERROR_INT_1("Unknown msg: d",ps_policyman_msg_ptr->policyman_msg);
      break;
  }      
} /* dsdi_policyman_msg_cb */

/*===========================================================================
  FUNCTION qmi_dmsi_msgr_hdlr()

  DESCRIPTION  call back registered for message router
   
  PARAMETERS
     msgrtype  - message id requested
     dsmsg     - changed data

  RETURN VALUE
    Boolean    - true, if message is handled correctly
               - false, if not

  DEPENDENCIES
    none

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dmsi_msgr_hdlr 
(
  const msgr_hdr_struct_type *dsmsg,
  void *user_data
)
{
  qmi_dmsi_cmd_buf_type *  cmd_ptr = NULL;
  dsmsgrrcv_msg_u             *rcvd_msg_ptr = NULL;
#ifdef FEATURE_TDSCDMA
  tds_rrc_set_config_rsp_type *set_resp_val = NULL;
  tds_rrc_get_config_rsp_type *get_resp_val = NULL;
#endif /*FEATURE_TDSCDMA*/
  qmi_dmsi_local_err_type      local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(NULL != dsmsg);

  rcvd_msg_ptr = (dsmsgrrcv_msg_u *)dsmsg;

  MSGR_CLEAR_VARIANT(rcvd_msg_ptr->hdr.id);
  
  switch (rcvd_msg_ptr->hdr.id) 
  {
    case  POLICYMAN_CFG_UPDATE_MSIM_IND:
       /*post a command to DCC*/
       /*Ignore payload here as info will be queried*/
       /*release the payload as it has reference in policyman*/
       policyman_msgr_attachments_release((msgr_hdr_s*)dsmsg);
        cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
        if( cmd_ptr == NULL)
        {
          break;
        }
        memset(cmd_ptr, 0, sizeof(qmi_dmsi_cmd_buf_type));
        cmd_ptr->cmd_id = QMI_CMD_DMS_DEV_CURRENT_CAP_CB;
        dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
      break;

#ifdef FEATURE_TDSCDMA
    case TDSCDMA_RRC_SET_CONFIG_RSP:
      set_resp_val = (tds_rrc_set_config_rsp_type *)dsmsg;

      cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
      if( cmd_ptr == NULL)
      {
        break;
      }
      
      memset(cmd_ptr, 0, sizeof(qmi_dmsi_cmd_buf_type));
      cmd_ptr->cmd_id = QMI_CMD_TDSCDMA_SET_CONFIG_RES;
      cmd_ptr->data.tdscdma_set_config_res.status = set_resp_val->status;

      dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
      break;

    case TDSCDMA_RRC_GET_CONFIG_RSP:
      get_resp_val = (tds_rrc_get_config_rsp_type *)dsmsg;

      cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();
      if( cmd_ptr == NULL)
      {
        break;
      }

      memset(cmd_ptr, 0, sizeof(qmi_dmsi_cmd_buf_type));
      cmd_ptr->cmd_id = QMI_CMD_TDSCDMA_GET_CONFIG_RES;
      cmd_ptr->data.tdscdma_get_config_res.active_config = get_resp_val->active_config;
      cmd_ptr->data.tdscdma_get_config_res.desired_config = get_resp_val->desired_config;

      dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);
      break;
#endif /*FEATURE_TDSCDMA*/

    default:
      local_err = QMI_DMSI_LOCAL_ERR_UNSUPPORTED_MSGR_TYPE;
  }
  LOG_MSG_INFO1_2("qmi_dmsi_msgr_hdlr  with msgr type = [%d] exit with local_err %d",
                   rcvd_msg_ptr->hdr.id, local_err);
}

/*===========================================================================
  FUNCTION qmi_dms_device_mode_changed()

  DESCRIPTION  Device mode is changed and checks if an indication needs to be
               sent to clients.
    Note: Indication will be sent only in case of single sim SVLTE config
          Rest of the things will be taken care of Policyman indications
  PARAMETERS
    none
  RETURN VALUE
    none
  DEPENDENCIES
    none
  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_device_mode_changed
(
  void
)
{
  uint32               max_subs = 0;
  qmi_error_e_type errval = QMI_ERR_NONE;
  policyman_status_t   status = -1; 
  policyman_item_t const *cm_pm_device_config = NULL;
  policyman_item_id_t id1[] = { POLICYMAN_ITEM_DEVICE_CONFIGURATION };
  sys_subs_feature_t      subs_device_feature_mode;  

   /*Get the policyman items intially*/
  status = policyman_get_items(
              &id1[0],
              sizeof(id1)/sizeof(policyman_item_id_t),
              &cm_pm_device_config);
  if (POLICYMAN_FAILED(status) || (cm_pm_device_config == NULL) )
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  status = 
    policyman_device_config_num_sims(cm_pm_device_config,
                                     (size_t*)&max_subs);
  if (POLICYMAN_FAILED(status) || (max_subs > SYS_MODEM_AS_ID_3))
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

   /*Indication will be sent only in case of single sim SVLTE config
     Rest of the things will be taken care of Policyman indications*/
  if(max_subs != 1)
  {
    goto send_result;
  }

  status = policyman_device_config_get_subs_feature(cm_pm_device_config,0,
                                                     SYS_MODEM_AS_ID_1,
                                                     &subs_device_feature_mode);
  if (POLICYMAN_FAILED(status))
  { 
    errval = QMI_ERR_INTERNAL;
    goto send_result;    
  }
  
  if(subs_device_feature_mode == SYS_SUBS_FEATURE_MODE_SVLTE)
  {
    qmi_dmsi_generate_current_device_capability_ind();
  }

  send_result:
   policyman_item_release(cm_pm_device_config);
   if(errval != QMI_ERR_NONE)
   {
     LOG_MSG_ERROR_2("qmi_dms_device_mode_changed: failed to send IND errval:%d status: 0x%x",
                      errval, status);
   }
   return;  
}

/*===========================================================================
  FUNCTION qmi_dmsi_generate_current_device_capability_ind()

  DESCRIPTION  Generate the Event report indicaiton for curr dev cap
   
  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    
  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_generate_current_device_capability_ind ( void )
{
  qmi_dmsi_client_info_type *    cl_sp;
  dms_ims_capability_ind_msg_v01 ind_msg;
  int                            result = -1;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!qmi_dmsi_inited)
  {
    return;
  }

  memset(&ind_msg, 0, sizeof(ind_msg));
  result = dmsi_get_device_ims_capability_from_pm((void *)&ind_msg);
  if (result != 0)
  {
    LOG_MSG_ERROR_INT_0("qmi_dmsi_generate_current_device_capability_ind() : "
                    "Unable to generate ims capabilty indication"); 
  }

  /* Send the indication for all the registered clients */
  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);
  while(cl_sp != NULL)
  {
    if( cl_sp->report_status.report_device_mode)
    {
      qmi_dmsi_event_report_ind(cl_sp,
                                DMSI_REPORT_STATUS_EV_DEV_MSIM_CAP,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0);
    }

    if( cl_sp->report_status.report_ims_capability && 
        result == 0)
    {
      LOG_MSG_INFO1_0("Generating ims cap ind");
      qmi_dmsi_generate_ims_capability_ind(&ind_msg, cl_sp);
    }

    cl_sp = q_next(&qmi_dms_state.client_q,
                           &(cl_sp->link));
  }
} /* qmi_dmsi_generate_current_device_capability_ind */

/*===========================================================================
FUNCTION qmi_dmsi_get_device_multisim_current_cap_info()

  DESCRIPTION - fill all the tlvs related to curr dev cap in given resp pointer
    
  PARAMETERS
     response - dsm_item for response
     cm_pm_device_config - policyman device config pointer
      uint8   - resp_type, IND or RESP
 
  RETURN VALUE
   qmi_error_type_v01   - error encountered while filling tlv

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_type_v01 qmi_dmsi_get_device_multisim_current_cap_info
(
  void* response,
  uint8 resp_type,
  policyman_item_t const *cm_pm_device_config
)
{
  policyman_status_t  status = -1; 
  policyman_item_t const *cm_policyman_device_config = cm_pm_device_config;
  policyman_item_t const *cm_policyman_ims_config = NULL;
  uint32             max_subs = 0;
  uint32             max_active = 0;
  boolean            is_ims_enabled = FALSE;
  int                max_ims_instances = 0;
  sys_subs_feature_t      subs_device_feature_mode = 0; 
  qmi_if_info_type       *qmi_if_info_ptr = NULL;
  const policyman_item_collection_t *cm_policyman_dev_curr_cap = NULL;
  const policyman_item_t *pItem = NULL;
  policyman_item_id_t id2[] = { POLICYMAN_ITEM_RAT_CAPABILITY, POLICYMAN_ITEM_UE_MODE, POLICYMAN_ITEM_IMS_CONFIG };
  policyman_item_id_t id3[] = { POLICYMAN_ITEM_IMS_CONFIG };
  uint32             curr_rat_tmp = 0;
  sys_ue_mode_e_type dev_vd_cap_tmp = 0;
  sys_modem_as_id_e_type sub_id;
  int              numitems = 0;
  int i = 0;
  qmi_error_type_v01 errval = QMI_ERR_NONE_V01;
  dms_get_device_cap_resp_msg_v01* dev_cap_res;
  dms_event_report_ind_msg_v01* ind_res;
  /* resp_type FALSE indicates the caller is get_dev_cap request handler
     TRUE indicates the caller is indication*/
  if(resp_type == FALSE)
  {
    dev_cap_res = (dms_get_device_cap_resp_msg_v01*)response;
  }
  else
  {
    ind_res = (dms_event_report_ind_msg_v01*)response;
  }
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!cm_policyman_device_config)
    {
      errval = QMI_ERR_INTERNAL_V01;
    return errval;
    }

    status = 
      policyman_device_config_num_sims(cm_policyman_device_config,(size_t*)&max_subs);
    if (POLICYMAN_FAILED(status))
    {
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
    status = policyman_device_config_max_active(cm_policyman_device_config, (size_t*)&max_active);
    if (POLICYMAN_FAILED(status))
    {
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }
    if(resp_type == FALSE)
    {
      dev_cap_res->current_multisim_capability.max_active = (uint8)max_active;
      dev_cap_res->current_multisim_capability.max_subscriptions = (uint8)max_subs;
    }
    else
    {
      ind_res->current_multisim_capability.max_active = (uint8)max_active;
      ind_res->current_multisim_capability.max_subscriptions = (uint8)max_subs;
    }

    status = policyman_get_items(
                  &id3[0],
                  sizeof(id3)/sizeof(policyman_item_id_t),
                  &cm_policyman_ims_config
                  );
    if (POLICYMAN_FAILED(status))
    {
      LOG_MSG_ERROR_INT_0("Get max SUBS etc failed");
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }

    status = policyman_ims_config_get_max_ims_instances(cm_policyman_ims_config, (size_t*)&max_ims_instances);
    if (POLICYMAN_FAILED(status))
    {
      LOG_MSG_ERROR_INT_0("Get max instances failed");
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    } 

    LOG_MSG_INFO1_1("max_ims_instances %d", max_ims_instances);

    if(resp_type == FALSE)
    {
      dev_cap_res->max_ims_instances = (uint8)max_ims_instances;       
    }

  /*Get current RAT Capability and UE mode per subs*/
  status = policyman_get_items_msim(id2,
                            sizeof(id2)/sizeof(policyman_item_id_t),
                            &cm_policyman_dev_curr_cap);
  if ( POLICYMAN_FAILED(status) || (cm_policyman_dev_curr_cap == NULL) || (cm_policyman_dev_curr_cap->numItems == 0))
   {
     errval = QMI_ERR_INTERNAL_V01;
     goto send_result;
   }

  /*check for the number of items it should be max_subs * items asked*/
  status = -1;
  numitems = cm_policyman_dev_curr_cap->numItems;

  for (i=0; i < numitems; i++)
  {
    pItem = (policyman_item_t *)cm_policyman_dev_curr_cap->pItems[i];

    switch (policyman_item_get_id_msim(pItem,&sub_id))
    {
      case POLICYMAN_ITEM_RAT_CAPABILITY:

        status = policyman_get_rat_capability( pItem,
                                            &curr_rat_tmp);
        if ( POLICYMAN_FAILED(status) || (sub_id < SYS_MODEM_AS_ID_1) || 
            (sub_id > SYS_MODEM_AS_ID_3))
        {
          errval = QMI_ERR_INTERNAL_V01;
          goto send_result;
        }
        if(resp_type == FALSE)
     	{
          dev_cap_res->current_subscription_capability_len++;
          dev_cap_res->current_subscription_capability[sub_id] = qmi_dmsi_convert_rat_mask(curr_rat_tmp);
     	}
     	else
     	{
          ind_res->current_subscription_capability_len++;
          ind_res->current_subscription_capability[sub_id] = qmi_dmsi_convert_rat_mask(curr_rat_tmp);
     	}
        
       break;

     case POLICYMAN_ITEM_UE_MODE:

       status = policyman_get_ue_mode( pItem,
                                       &dev_vd_cap_tmp);
     if ( POLICYMAN_FAILED(status) || (sub_id < SYS_MODEM_AS_ID_1) || 
          (sub_id > SYS_MODEM_AS_ID_3))
       {
         errval = QMI_ERR_INTERNAL_V01;
         goto send_result;
       }
       /*Convert to QMI notations*/
       /*Use new API from policyman to fill this*/
        if(resp_type == FALSE)
        {
     	  dev_cap_res->subs_voice_data_capability_len++;
          dev_cap_res->subs_voice_data_capability[sub_id].simul_voice_data_capable = 
                  policyman_is_svd_operation_allowed(sub_id);
          dev_cap_res->subs_voice_data_capability[sub_id].subs_voice_data_capability = 
                 qmi_dmsi_is_simul_voice_data(dev_vd_cap_tmp);
        }
        else
        {
          ind_res->subs_voice_data_capability_len++;
          ind_res->subs_voice_data_capability[sub_id].simul_voice_data_capable = 
                                policyman_is_svd_operation_allowed(sub_id);
          ind_res->subs_voice_data_capability[sub_id].subs_voice_data_capability = 
                                qmi_dmsi_is_simul_voice_data(dev_vd_cap_tmp);
        }
        break;

     case POLICYMAN_ITEM_IMS_CONFIG:
        /* Filled in only for get request. Ignored for event indication*/
        if(resp_type == FALSE)
        {
          status = policyman_ims_config_get_is_enabled(pItem, 
                                                           &is_ims_enabled);
          if ( POLICYMAN_FAILED(status) || (sub_id < SYS_MODEM_AS_ID_1) || 
              (sub_id > SYS_MODEM_AS_ID_3))
          {
            errval = QMI_ERR_INTERNAL_V01;
            goto send_result;
          }

          LOG_MSG_INFO1_2(" is_ims_enabled %d subs_id %d", is_ims_enabled, sub_id);
          dev_cap_res->ims_capability[dev_cap_res->ims_capability_len].enabled = (uint8)is_ims_enabled;
         /*Policyman uses sys_modem_as_id_e_type which has Primary at 0 and secondary at 1
           It needs to be converted to DMS type by adding 1
         */
          dev_cap_res->ims_capability[dev_cap_res->ims_capability_len].subscription = 
           (dms_bind_subscription_enum_v01)(sub_id + 1);
          dev_cap_res->ims_capability_len++;
        }
        
       break;

     default:
        break;
    }/*switch*/
  }/*for*/
  /*Policyman dont handle single sim SVLTE case. Get info from CM and update ue_mode*/
   if(max_subs == 1)
   {
     status = policyman_device_config_get_subs_feature(cm_policyman_device_config,0,
                                                     SYS_MODEM_AS_ID_1,
                                                     &subs_device_feature_mode);
     if (POLICYMAN_FAILED(status))
     {
       errval = QMI_ERR_INTERNAL_V01;
       goto send_result;
     }
     if(subs_device_feature_mode == SYS_SUBS_FEATURE_MODE_SVLTE)
     {
       qmi_if_info_ptr = qmi_if_info_get_ptr();
       /*if hybrid stack is opertational it is SVLTE else CSFB*/
       if(qmi_if_info_ptr->ss_info[SYS_MODEM_AS_ID_1].is_hybr_stack_operational)
       {
        if(resp_type == FALSE)
     	{
     	  dev_cap_res->subs_voice_data_capability[SYS_MODEM_AS_ID_1].subs_voice_data_capability = 
                 qmi_dmsi_is_simul_voice_data(SYS_UE_MODE_SVLTE);
     	}
     	else
     	{
          ind_res->subs_voice_data_capability[SYS_MODEM_AS_ID_1].subs_voice_data_capability = 
                 qmi_dmsi_is_simul_voice_data(SYS_UE_MODE_SVLTE);
     	}
       }
       else
       {
        if(resp_type == FALSE)
        {
          dev_cap_res->subs_voice_data_capability[SYS_MODEM_AS_ID_1].subs_voice_data_capability = 
                 qmi_dmsi_is_simul_voice_data(SYS_UE_MODE_CSFB);
        }
        else
        {
          ind_res->subs_voice_data_capability[SYS_MODEM_AS_ID_1].subs_voice_data_capability = 
                 qmi_dmsi_is_simul_voice_data(SYS_UE_MODE_CSFB);
     	}
       }
     }
     LOG_MSG_INFO1_2("Single Sim case: Subs feature mode:%d and UE_Mode: %d ",
                   subs_device_feature_mode, 
                    (resp_type == FALSE ? 
                     dev_cap_res->subs_voice_data_capability[SYS_MODEM_AS_ID_1].subs_voice_data_capability:
                     ind_res->subs_voice_data_capability[SYS_MODEM_AS_ID_1].subs_voice_data_capability) );
   }

  send_result:

  /*release the policyman items after use*/
  if(cm_policyman_dev_curr_cap != NULL)
  {
    policyman_item_collection_release(cm_policyman_dev_curr_cap);
  }
  if(cm_policyman_ims_config != NULL)
  {
    /* Releasing single item */
    policyman_item_release(cm_policyman_ims_config);
  }

  LOG_MSG_INFO1_3("qmi_dmsi_get_device_multisim_current_cap_info: "
                    "resp type %d, info.errval:%d Policyman. "
                    "status:0x%x",resp_type, errval, status);
 

  return errval;

}

/*===========================================================================
FUNCTION qmi_dmsi_get_device_ims_capability_info()

  DESCRIPTION - fill all the tlvs related to ims mode in given resp pointer
    
  PARAMETERS
     ims_capability - capability message to fill in
 
  RETURN VALUE
   qmi_error_type_v01   - error encountered while filling tlv

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static int dmsi_get_device_ims_capability_from_pm
(
  void  * ims_capability
)
{
  policyman_status_t                  status = -1; 
  boolean                             is_ims_enabled = FALSE;
  const policyman_item_collection_t * cm_policyman_dev_ims_cap = NULL;
  const policyman_item_t            * pItem = NULL;
  policyman_item_id_t                 id[]   = { POLICYMAN_ITEM_IMS_CONFIG };
  int                                 numitems = 0;
  int                                 item_index = 0;
  dms_ims_capability_ind_msg_v01    * ind_res;
  int                                 result = 0;
  sys_modem_as_id_e_type              sub_id;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  LOG_MSG_INFO1_0("dmsi_get_device_ims_capability_from_pm()");
  ind_res = (dms_ims_capability_ind_msg_v01 *)ims_capability;
 
  /*Get IMS mode information per subs*/
  status = policyman_get_items_msim(id,
                                    sizeof(id)/sizeof(policyman_item_id_t),
                                    &cm_policyman_dev_ims_cap);
  if ( POLICYMAN_FAILED(status) || (cm_policyman_dev_ims_cap == NULL) || 
       (cm_policyman_dev_ims_cap->numItems == 0))
  {
    result = -1;
    goto send_result;
  }

  status = -1;
  numitems = cm_policyman_dev_ims_cap->numItems;

  for (item_index=0; item_index < numitems; item_index++)
  {
    pItem = (policyman_item_t *)cm_policyman_dev_ims_cap->pItems[item_index];
    (void)policyman_item_get_id_msim(pItem,&sub_id);
    status = policyman_ims_config_get_is_enabled(pItem, 
                                                     &is_ims_enabled);
    if ( POLICYMAN_FAILED(status) || (sub_id < SYS_MODEM_AS_ID_1) || 
         (sub_id > SYS_MODEM_AS_ID_3))
    {
      result = -1;
      goto send_result;
    }

    LOG_MSG_INFO1_2(" is_ims_enabled %d subs_id %d", is_ims_enabled, sub_id);
    ind_res->ims_capability[ind_res->ims_capability_len].enabled = (uint8)is_ims_enabled;
    /*Policyman uses sys_modem_as_id_e_type which has Primary at 0 and secondary at 1
      It needs to be converted to DMS type by adding 1
     */
    ind_res->ims_capability[ind_res->ims_capability_len].subscription = 
     (dms_bind_subscription_enum_v01)(sub_id + 1);
    ind_res->ims_capability_len++;        
  }

send_result:
  if(cm_policyman_dev_ims_cap != NULL)
  {
    policyman_item_collection_release(cm_policyman_dev_ims_cap);
  }

  LOG_MSG_INFO1_2("dmsi_get_device_ims_capability_from_pm: "
                    "result :%d Policyman. "
                    "status:0x%x", result, status);
 

  return result;
} /* dmsi_get_device_ims_capability_from_pm */

/*===========================================================================
  FUNCTION qmi_dmsi_is_simul_voice_data()

  DESCRIPTION  - Convert policyman ue_mode to QMI enum
    
  PARAMETERS
     ue_mode - policyman enum for ue_mode

  RETURN VALUE
     dms_subs_voice_data_capability_enum_v01  - qmi enum for ue_mode
   
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dms_subs_voice_data_capability_enum_v01 qmi_dmsi_is_simul_voice_data 
(
   sys_ue_mode_e_type ue_mode
)
{
  dms_subs_voice_data_capability_enum_v01 vd_cap;

  switch(ue_mode)
  {
    case SYS_UE_MODE_NONE:
    case SYS_UE_MODE_NORMAL:
        vd_cap = DMS_SUBS_VOICE_DATA_CAPABILITY_NORMAL_V01;
      break;
  
    case SYS_UE_MODE_SVLTE:
    case SYS_UE_MODE_SVLTE_ONLY:
        vd_cap = DMS_SUBS_VOICE_DATA_CAPABILITY_SVLTE_V01;
      break;

    case SYS_UE_MODE_SGLTE:
    case SYS_UE_MODE_SGLTE_ONLY:
        vd_cap = DMS_SUBS_VOICE_DATA_CAPABILITY_SGLTE_V01;
      break;

    case SYS_UE_MODE_CSFB:
    case SYS_UE_MODE_CSFB_ONLY:
        vd_cap = DMS_SUBS_VOICE_DATA_CAPABILITY_CSFB_V01;
      break;

    case SYS_UE_MODE_1XSRLTE_ONLY:
    case SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED:
    case SYS_UE_MODE_GSMSRLTE_ONLY:
    case SYS_UE_MODE_GSMSRLTE_CSFB_PREFERRED:
        vd_cap =  DMS_SUBS_VOICE_DATA_CAPABILITY_SRLTE_V01;
      break;

    default:
        vd_cap = DMS_SUBS_VOICE_DATA_CAPABILITY_NORMAL_V01;
      break;
  }

  return vd_cap;
}

/*===========================================================================
  FUNCTION qmi_dmsi_convert_rat_mask()

  DESCRIPTION  - convert rat mask from policyman to QMI rat mask
    
  PARAMETERS
    rat  - policyman rat mask

  RETURN VALUE
   uint64 - QMI rat mask

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint64 qmi_dmsi_convert_rat_mask 
(
   uint32 rat
)
{
  uint64 rat_mask = 0;

  if(rat & SYS_SYS_MODE_MASK_CDMA)
  {
    rat_mask |= DMS_SUBS_CAPABILITY_CDMA_V01;
  }
  if(rat & SYS_SYS_MODE_MASK_HDR)
  {
    rat_mask |= DMS_SUBS_CAPABILITY_HDR_V01;
  }
  if(rat & SYS_SYS_MODE_MASK_GSM) 
  {
    rat_mask |= DMS_SUBS_CAPABILITY_GSM_V01;
  }
  if(rat & SYS_SYS_MODE_MASK_WCDMA)
  {
    rat_mask |= DMS_SUBS_CAPABILITY_WCDMA_V01;
  }
  if(rat & SYS_SYS_MODE_MASK_LTE)
  {
    rat_mask |= DMS_SUBS_CAPABILITY_LTE_V01;
  }
  if(rat & SYS_SYS_MODE_MASK_TDS)
  {
    rat_mask |= DMS_SUBS_CAPABILITY_TDS_V01;
  }
  
  return rat_mask;

}

/*===========================================================================
  FUNCTION QMI_DMSI_INITIALIZE_CLIENT()

  DESCRIPTION
    Initialize the new client state for the given client state ptr.
    
  PARAMETERS
    cl_sp :  client state pointer
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_initialize_client
(
  qmi_dmsi_client_info_type * cl_sp
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset( cl_sp , 0, sizeof(qmi_dmsi_client_info_type) );
/*-----------------------------------------------------------------------
	init client bat levels to min/max 
  -----------------------------------------------------------------------*/
  memset(&(cl_sp->report_status), 0, sizeof(cl_sp->report_status));
  cl_sp->report_status.bat_lvl_lower_limit = DMSI_BAT_LVL_MIN;
  cl_sp->report_status.bat_lvl_upper_limit = DMSI_BAT_LVL_MAX;
  cl_sp->report_status.last_bat_lvl_reported = DMSI_BAT_LVL_INACTIVE;
  cl_sp->report_status.last_power_state_reported = DMSI_PWR_STATE_INACTIVE; 
  
} /* qmi_dmsi_initialize_client() */

/*===========================================================================
  FUNCTION QMI_DMSI_PROCESS_CMD()

  DESCRIPTION
    This function processes a QMI DMS command or event.

    It is called by the QMI command handler and will dispatch the
    associated command/event handler function.

  PARAMETERS
    cmd_ptr:  private data buffer containing the QMI DMS command
              information.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI DMS must already have been initialized

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_process_cmd
(
  dcc_cmd_enum_type  cmd,
  void *cmd_ptr
)
{
  qmi_dmsi_cmd_buf_type *cmd_buf_ptr = NULL;
/*-------------------------------------------------------------------------*/

  DS_QMI_DMS_ASSERT(cmd_ptr != NULL);
  cmd_buf_ptr = (qmi_dmsi_cmd_buf_type *)cmd_ptr;

  switch(cmd_buf_ptr->cmd_id)
  {

#if defined(FEATURE_MMGSDI_SESSION_LIB)
    case QMI_CMD_PROCESS_UIM_GET_STATE:
      /* The mmgsdi_evt_info.activated field is populated and used only for
         MMGSDI_SESSION_CHANGED_EVT */
      qmi_dms_process_uim_get_state(cmd_buf_ptr->data.mmgsdi_evt_info.event,
                                       cmd_buf_ptr->data.mmgsdi_evt_info.card_err_info,
                                       cmd_buf_ptr->data.mmgsdi_evt_info.activated);
      break;

#endif /* defined(FEATURE_MMGSDI_SESSION_LIB) */

    case QMI_CMD_DMS_CM_CALL_CMD_STATUS:
      qmi_dms_process_cm_call_err(cmd_buf_ptr->data.cm_call_err.user_data,
                                     cmd_buf_ptr->data.cm_call_err.call_cmd,
                                     cmd_buf_ptr->data.cm_call_err.err_type);
      break;

    case QMI_CMD_SET_OPERATING_MODE_RESP:
      qmi_dms_generate_set_oprt_mode_resp(cmd_buf_ptr->data.cm_ph_err.user_data,
                                             cmd_buf_ptr->data.cm_ph_err.err_type);
      break;

    case QMI_CMD_DMS_EVT_REPORT_REQ_IND:
      qmi_dms_event_report_req_send_ind
                          (cmd_buf_ptr->data.dms_evt_info.evt_type,                           
                           cmd_buf_ptr->data.dms_evt_info.evt_value,
                           cmd_buf_ptr->data.dms_evt_info.cl_sp);
      break;

    case QMI_CMD_DMS_RESET_SHUTDOWN:
      qmi_dms_generate_oprt_mode_ind(cmd_buf_ptr->data.cm_ph.info.oprt_mode);
      break;

#ifdef FEATURE_TDSCDMA	  
  	case QMI_CMD_TDSCDMA_SET_CONFIG_RES:
    {
      qmi_dmsi_generate_set_test_config_resp(&(cmd_buf_ptr->data.tdscdma_set_config_res));
      break;
    }
    case QMI_CMD_TDSCDMA_GET_CONFIG_RES:
    {
      qmi_dmsi_generate_get_test_config_resp(&(cmd_buf_ptr->data.tdscdma_get_config_res));
      break;
    }
#endif
    case QMI_CMD_DMS_DEV_CURRENT_CAP_CB:
    {
      qmi_dmsi_generate_current_device_capability_ind();
      break;
    }
    case QMI_CMD_DMS_POWER_STATISTICS_EVT:
    {
      qmi_dmsi_generate_power_stats_ind(&(cmd_buf_ptr->data.modem_power_stats));
      break;
    }
    case QMI_CMD_SET_UI_STATUS_RESP:
    {
      qmi_dms_generate_set_ui_status_resp(cmd_buf_ptr->data.cm_ph_err.user_data,
                                          cmd_buf_ptr->data.cm_ph_err.err_type);
      break;
    }
    default:
      LOG_MSG_ERROR_INT_1("Unexpected cmd (d) recd in QMI DMS cmd handler",
                      cmd_buf_ptr->cmd_id);
      DS_QMI_DMS_ASSERT(0);
  }
  PS_SYSTEM_HEAP_MEM_FREE(cmd_ptr);
  return;
} /* qmi_dmsi_process_cmd() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_CMD_BUF()

  DESCRIPTION
    Allocate and assign a QMI DMS command buffer from the PS MEM heap
    based on the QMI CMD type
    
  PARAMETERS
    cmd - QMI command type
   
  RETURN VALUE
    cmd_buf_ptr - Pointer to the allocated command buffer

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void *qmi_dmsi_get_cmd_buf()
{
  qmi_dmsi_cmd_buf_type *cmd_buf_ptr = NULL;
/*-------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(cmd_buf_ptr, sizeof(qmi_dmsi_cmd_buf_type),
                               qmi_dmsi_cmd_buf_type*);
  if (NULL == cmd_buf_ptr)
  {
    LOG_MSG_ERROR_INT_0(" Dms get cmd buf out of memory error");
  }

  return ((void *)cmd_buf_ptr);
} /* qmi_dmsi_get_cmd_buf */

/*===========================================================================
  FUNCTION QMI_DMSI_PROCESS_SVC_INIT()

  DESCRIPTION
    This function processes a init callback request and intialize the service
    related infos.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    QMI DMS must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_process_svc_init
(
  void
)
{
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_0( "qmi_dmsi init callback " );

  /*-------------------------------------------------------------------------
    initialize client state non-zero fields
  -------------------------------------------------------------------------*/

  if(qmi_dmsi_global.inited == FALSE)
  {
    /*-----------------------------------------------------------------------
      Initialize the global DMS service parameters
    -----------------------------------------------------------------------*/  
    memset( &qmi_dmsi_global, 0, sizeof(qmi_dmsi_global) );
  
    qmi_dmsi_global.pwr_rpt.low_limit       = DMSI_BAT_LVL_MIN;
    qmi_dmsi_global.pwr_rpt.high_limit      = DMSI_BAT_LVL_MAX;
    /* Default value of AP's UI status is Ready */
    qmi_dmsi_global.ui_ready_status         = TRUE; 
    
  #ifdef FEATURE_MMGSDI_SESSION_LIB  
    qmi_dmsi_global.uim_state = DMS_UIM_STATE_UNAVAILABLE_V01;
  #endif /* FEATURE_MMGSDI_SESSION_LIB */

    /*-----------------------------------------------------------------------
      initialize the charger module
    -----------------------------------------------------------------------*/  
    qmi_charger_init();

    qmi_dmsi_global.voice_call_id = CM_CALL_ID_INVALID;

#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
    qmi_dmsi_global.activation_state = qmi_dmsi_calculate_activated_state();
#endif /* defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900) */
    
    /* init the PRL sequence number to 0 for manual activation PRL's */
    qmi_dmsi_global.prl_info.cl_sp = NULL;

    if (QMI_NV_STATUS_OK != qmi_nv_read(QMI_NV_ITEM_ALLOW_INVALID_SPC,
                                  0,
                                  0,
                                  &qmi_dmsi_global.nv_allow_invalid_spc,
                                  sizeof(qmi_dmsi_global.nv_allow_invalid_spc)))
    {
      qmi_dmsi_global.nv_allow_invalid_spc = FALSE;
    }
    if (QMI_NV_STATUS_OK != qmi_nv_read(QMI_NV_ITEM_RESTRICT_IMSI_ACCESS,
                               0,
                               0,
                               &qmi_dmsi_global.nv_restrict_imsi_access,
                               sizeof(qmi_dmsi_global.nv_restrict_imsi_access)))
    {
      qmi_dmsi_global.nv_restrict_imsi_access = FALSE;
    }
    if (QMI_NV_STATUS_OK != qmi_nv_read(
                         QMI_NV_ITEM_USE_MAX_DEFINED_DATA_RATES,
                         0,
                         0,
                         &qmi_dmsi_global.nv_use_max_defined_data_rates,
                         sizeof(qmi_dmsi_global.nv_use_max_defined_data_rates)))
    {
      qmi_dmsi_global.nv_use_max_defined_data_rates = FALSE;
    }
    if (QMI_NV_STATUS_OK != qmi_nv_read(
                         QMI_NV_ITEM_USE_OTASP_ACT_STATE,
                         0,
                         0,
                         &qmi_dmsi_global.nv_use_last_otasp_act_state,
                         sizeof(qmi_dmsi_global.nv_use_last_otasp_act_state)))
    {
      qmi_dmsi_global.nv_use_last_otasp_act_state = FALSE;
    }

#ifdef FEATURE_TDSCDMA
    /* Initialize tdscdma test config module for GET/SET routines */
    qmi_dmsi_tdscdma_config_init();
#endif
    qmi_dmsi_policyman_config_init();

    qmi_dmsi_global.inited = TRUE;
  }

  (void) q_init( &qmi_dms_state.modem_activity_pend_q );
  qmi_dms_state.last_modem_activity_query_time = 0;
} /* qmi_dmsi_process_svc_init */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_CDMA_LOCK_MODE()

  DESCRIPTION
    Retrieves CDMA lock mode
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_cdma_lock_mode
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_cdma_lock_mode_resp_msg_v01 resp_msg;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&resp_msg, 0, sizeof(dms_get_cdma_lock_mode_resp_msg_v01));

  resp_msg.cdma_lock_mode_status_valid = TRUE;
  resp_msg.cdma_lock_mode_status = (uint8) qmi_dmsi_global.cdma_lock_mode;
	
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_cdma_lock_mode_resp_msg_v01));       
  return QMI_CSI_CB_NO_ERR;
  
} /* qmi_dmsi_get_cdma_lock_mode() */

/*===========================================================================
  FUNCTION QMI_DMS_EVENT_REPORT_REQ_SEND_IND()

  DESCRIPTION
    Sends the indication of a phone to the client that 
    registered for it.
    
  PARAMETERS
    event        : phone event from cm
    value        : status of the phone event
    clid         : Id of the registering client

  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_event_report_req_send_ind
(
  qmi_dmsi_report_status_ev_e_type             event_type,
  void                                       * event_info,
  qmi_dmsi_client_info_type*                   cl_sp
)
{
  boolean *                                    cdma_lock_mode = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch (event_type)
  {
    case DMSI_REPORT_STATUS_EV_CDMA_LOCK_MODE:
      cdma_lock_mode = (boolean *) event_info;	
      qmi_dmsi_event_report_ind(cl_sp,
                                DMSI_REPORT_STATUS_EV_CDMA_LOCK_MODE,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                *cdma_lock_mode);
   break;
   
   case DMSI_REPORT_STATUS_EV_DEV_MSIM_CAP:
          qmi_dmsi_event_report_ind(cl_sp,
                                    DMSI_REPORT_STATUS_EV_DEV_MSIM_CAP,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0);
    break;

    default:
      break;
  }
} /*qmi_dms_event_report_req_send_ind*/

/*===========================================================================
  FUNCTION QMI_DMSI_SEND_RESPONSE()

  DESCRIPTION
  sends response using QCSI API.
    
  PARAMETERS
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    c_struct_len       :       length of response


  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dmsi_send_response
(
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void*                    resp_msg,
  unsigned int             c_struct_len
)
{
  qmi_csi_error err = QMI_CSI_NO_ERR;
  err = qmi_csi_send_resp(req_handle, msg_id, resp_msg, c_struct_len);
  LOG_MSG_INFO1_2("qmi_dmsi_send_response err = %d for msg_id = %d",err,msg_id);
}

/*===========================================================================
  FUNCTION QMI_DMSI_TRIGGER_MODEM_ACTIVITY_INFO()

  DESCRIPTION
    Triggers the modem activity info calculation
    
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_trigger_modem_activity_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_dmsi_client_info_type *  dms_client_sp;
  dms_config_modem_activity_info_req_msg_v01    *req_msg;
  dms_config_modem_activity_info_resp_msg_v01    resp_msg;

  int16   return_val = 0;
  int16   ps_errno = 0;
  ps_sys_modem_power_stats_config_type   stats_configuration;
  qmi_dmsi_power_stats_pend_q_type     * pend_stats_q_node = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);

  dms_client_sp = (qmi_dmsi_client_info_type *)clnt_info;
  req_msg = (dms_config_modem_activity_info_req_msg_v01*)req_c_struct;
  memset(&resp_msg, 0, sizeof(dms_config_modem_activity_info_resp_msg_v01));
  
  if ( req_msg->enable_statistics_valid ) 
  {
    if(req_msg->enable_statistics > 1)
    {
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }
    stats_configuration.enable_power_statistics = req_msg->enable_statistics;
    return_val = ps_sys_conf_set_ex( PS_SYS_TECH_ALL, 
                                     PS_SYS_CONF_MODEM_POWER_STATS_CONFIGURATION,
                                     PS_SYS_PRIMARY_SUBS, 
                                     &stats_configuration,
                                     &ps_errno );
  }
  else
  {
    errval = QMI_ERR_MISSING_ARG_V01;
    goto send_result;

  }

  if(return_val < 0)
  {
    switch (ps_errno)
    {                                   
      case DS_NOMEMORY:
        errval  = QMI_ERR_NO_MEMORY_V01;
        break;
      
      case DS_EFAULT:
        errval = QMI_ERR_INVALID_ARG_V01;
        break;
        
      default:
        errval = QMI_ERR_INTERNAL_V01;
        break;
    } /*  switch (ps_errno) */
    
    goto send_result;
  }

  if ( req_msg->enable_statistics == FALSE )
  {
    //power statistic turned off; Reset last query time
    qmi_dms_state.last_modem_activity_query_time = 0;

    //Clear the pending queue
    while ( (pend_stats_q_node = 
            (qmi_dmsi_power_stats_pend_q_type *) q_get(&qmi_dms_state.modem_activity_pend_q)) != NULL )
    {
      PS_SYSTEM_HEAP_MEM_FREE(pend_stats_q_node);   
    }
  }

send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                     : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_config_modem_activity_info_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_trigger_modem_activity_info() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_MODEM_ACTIVITY_INFO()

  DESCRIPTION
    Queries for the modem activity information. The actual statisctics is
    not returned in the response. Client is expected to listen to
    QMI_DMSI_MODEM_ACTIVITY_INFO_IND for the actual statistics
 
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_modem_activity_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  qmi_dmsi_client_info_type *  dms_client_sp = NULL ;
  dms_get_modem_activity_info_resp_msg_v01 resp_msg;


  int16   return_val = 0;
  int16   ps_errno = 0;
  qmi_dmsi_client_info_type *  pending_cl_sp = NULL; 
  qmi_dmsi_power_stats_pend_q_type *pend_stats_q_node = NULL;
  ps_sys_modem_power_stats_info_type  activity_info;
  ps_sys_modem_power_stats_config_type   stats_configuration;
  uint64  current_time;
  qmi_dmsi_local_err_type local_err = QMI_DMSI_LOCAL_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT(clnt_info != NULL);

  dms_client_sp = (qmi_dmsi_client_info_type *)clnt_info;
  memset(&resp_msg,0,sizeof(dms_get_modem_activity_info_resp_msg_v01));
  memset(&activity_info, 0, sizeof(ps_sys_modem_power_stats_info_type));
  memset(&stats_configuration, 0, sizeof(ps_sys_modem_power_stats_config_type));

  //Check if power stats calculation is enabled 
  return_val = ps_sys_conf_get_ex( PS_SYS_TECH_ALL, 
                                   PS_SYS_CONF_MODEM_POWER_STATS_CONFIGURATION,
                                   PS_SYS_PRIMARY_SUBS,
                                   &stats_configuration,
                                   &ps_errno );

  if(return_val == 0)
  {
    if (stats_configuration.enable_power_statistics == FALSE)
    {
      local_err = QMI_DMSI_LOCAL_ERR_POWER_STATS_NOT_CONFIG;
      errval = QMI_ERR_NOT_PROVISIONED_V01;
      goto send_result;
    }
  }
  else
  {
    local_err = QMI_DMSI_LOCAL_ERR_INTERNAL;
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Q has pending responses? Add the new request to Q. Do not invoke MH for
    stats again since, this will reset lower layers stats to 0, for every new
    request
  -------------------------------------------------------------------------*/
  
  pending_cl_sp = (qmi_dmsi_client_info_type *)q_check(&qmi_dms_state.modem_activity_pend_q);
  if ( pending_cl_sp != NULL )
  {
    LOG_MSG_INFO2_1("Adding cl_sp 0x%p into pending queue ", dms_client_sp);
  }
  else /* Q is empty */
  {
    // Query the current sys time
    current_time = time_get_ms_native();
    if( (qmi_dms_state.last_modem_activity_query_time != 0) && 
        ((current_time - qmi_dms_state.last_modem_activity_query_time) < 
         (MODEM_ACTIVITY_QUERY_THRESHOLD_TIME + 1)) )
    {
      /*-------------------------------------------------------------------------
        New request has been made within the minimum threshold time.Don't trigger
        MH for new values, since this will reset lower layers statistics to 0.
        Return the last reported statistics from our cache.
      -------------------------------------------------------------------------*/
      return_val = ps_sys_conf_get_ex( PS_SYS_TECH_ALL, 
                                       PS_SYS_CONF_MODEM_POWER_STATISTICS,
                                       PS_SYS_PRIMARY_SUBS, 
                                       (void*)&activity_info,
                                       &ps_errno );
    
      if(return_val == 0)
      {
        //Sending cached value
        dmsi_power_statistics_info_cb_ex(PS_SYS_TECH_ALL,
                                         PS_SYS_EVENT_MODEM_POWER_STATISTICS,
                                         PS_SYS_PRIMARY_SUBS,
                                         &activity_info,
                                         NULL,
                                         TRUE);
      }
    }
    else
    {
      //Invoke MH to compute modem power stats
      return_val = ps_sys_conf_set_ex( PS_SYS_TECH_ALL, 
                                       PS_SYS_CONF_QUERY_MODEM_POWER_STATS,
                                       PS_SYS_PRIMARY_SUBS, 
                                       (void*)dms_client_sp,
                                       &ps_errno );

      if(return_val == 0)
      {
        //Update time stamp of last query request
        qmi_dms_state.last_modem_activity_query_time  = current_time;
      }
    }
                              
    if(return_val < 0)
    {
      local_err = QMI_DMSI_LOCAL_ERR_SYS_CONF_FAIL;
      switch (ps_errno)
      {                                   
        case DS_NOMEMORY:
          errval  = QMI_ERR_NO_MEMORY_V01;
          break;
        
        case DS_EFAULT:
          errval = QMI_ERR_INVALID_ARG_V01;
          break;
          
        default:
          errval = QMI_ERR_INTERNAL_V01;
          break;
      } /*  switch (ps_errno) */
      goto send_result;
    }
  }/* Q is empty*/

 /*-------------------------------------------------------------------------
    Queue client state pointer to the pending queue of power stats.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(pend_stats_q_node, 
                                  sizeof(qmi_dmsi_power_stats_pend_q_type),
                                  qmi_dmsi_power_stats_pend_q_type*);
  if( NULL == pend_stats_q_node )
  {
    DS_QMI_DMS_ASSERT(0);
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }

  //Put client into pending queue
  pend_stats_q_node->cl_sp = dms_client_sp;
  q_link(pend_stats_q_node, &(pend_stats_q_node->link));
  q_put( &qmi_dms_state.modem_activity_pend_q, &(pend_stats_q_node->link));

send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                     : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_modem_activity_info_resp_msg_v01));
  LOG_MSG_INFO1_1("DMS get modem activity exit with local err %d ", local_err);
  return QMI_CSI_CB_NO_ERR;
  
} /* qmi_dmsi_get_modem_activity_info() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_UI_STATUS_INFO

  DESCRIPTION
    Queries UI status info at modem.
    This command is applicable per device and is independent of subscription

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_get_ui_status_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_get_ui_status_info_resp_msg_v01 resp_msg;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);

  memset(&resp_msg,0,sizeof(dms_get_ui_status_info_resp_msg_v01));

  /*-------------------------------------------------------------------------
    get the UI status and respond
  -------------------------------------------------------------------------*/
  resp_msg.ui_ready_status_valid = TRUE;
  resp_msg.ui_ready_status = qmi_dmsi_global.ui_ready_status;

  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                       : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_ui_status_info_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_get_ui_status_info() */

/*===========================================================================
  FUNCTION QMI_DMSI_SET_UI_STATUS_INFO

  DESCRIPTION
    Controls/sets UI status info at modem.
    This command is applicable per device and is independent of subscription

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_set_ui_status_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_set_ui_status_info_req_msg_v01  *req_msg;
  dms_set_ui_status_info_resp_msg_v01 resp_msg;
  ds_qmi_dms_async_cmd_type * cmd_buf_p = NULL;
  qmi_if_info_type     *qmi_if_info_ptr = NULL;
  boolean   new_ui_ready_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);
  memset(&resp_msg,0,sizeof(dms_set_ui_status_info_resp_msg_v01));
  req_msg = (dms_set_ui_status_info_req_msg_v01*)req_c_struct;
  
  if (req_msg->ui_ready_status_valid != TRUE)
  {
    errval = QMI_ERR_MISSING_ARG_V01;
    goto send_result;
  }

  new_ui_ready_status = (boolean)req_msg->ui_ready_status;

  if ( new_ui_ready_status == qmi_dmsi_global.ui_ready_status )
  {    
    errval = QMI_ERR_NO_EFFECT_V01;
    goto send_result;   
  }

  LOG_MSG_INFO1_2("qmi_dmsi_set_ui_status_info: New: %d while Curr: %d",
                   new_ui_ready_status, qmi_dmsi_global.ui_ready_status);
  /* ----------------------------------------------------------
     Allocated temp memory for resp_buf_type 
    -----------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(cmd_buf_p,
                             sizeof(ds_qmi_dms_async_cmd_type),
                             ds_qmi_dms_async_cmd_type*);
    if( cmd_buf_p == NULL )
    {
      errval = QMI_ERR_NO_MEMORY_V01;
      goto send_result;
    }
    memset(cmd_buf_p,0,sizeof(ds_qmi_dms_async_cmd_type));
    cmd_buf_p->req_handle = req_handle;
    cmd_buf_p->msg_id = msg_id;
    cmd_buf_p->cl_sp = clnt_info;
    cmd_buf_p->user_data = (uint32)new_ui_ready_status;
	
    qmi_if_info_ptr = qmi_if_info_get_ptr();
    if ( !cm_ph_cmd_ui_ready_status( qmi_dmsi_cm_ph_cb,
                                     cmd_buf_p,
                                     qmi_if_info_ptr->cm_client_id,
                                     new_ui_ready_status) )
    {
      errval = QMI_ERR_INTERNAL_V01;
      goto send_result;
    }

    return QMI_CSI_CB_NO_ERR; 

send_result:
  if(cmd_buf_p != NULL)
  {
    PS_SYSTEM_HEAP_MEM_FREE(cmd_buf_p);
  }
  result = (errval == QMI_ERR_NONE_V01)?QMI_RESULT_SUCCESS_V01: 
                                        QMI_RESULT_FAILURE_V01;
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_ui_status_info_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}

/*===========================================================================
  FUNCTION QMI_DMSI_SET_DEVICE_CAP_CONFIG_REQ

  DESCRIPTION
     This command sets Device configuration from modem lists config list
     This command is applicable per device and is independent of subscription

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_set_device_cap_config_req
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01  result;
  dms_set_device_cap_config_req_msg_v01  *req_msg;
  dms_set_device_cap_config_resp_msg_v01 resp_msg;
  int                        explicit_index;
  policyman_status_t         status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info && req_c_struct);
  memset(&resp_msg,0,sizeof(dms_set_device_cap_config_resp_msg_v01));
  req_msg = (dms_set_device_cap_config_req_msg_v01*)req_c_struct;

  /*Atleast one of the optional TLVs should present in the request*/
  if (req_msg->explicit_config_index_valid != TRUE)
  {
    errval = QMI_ERR_MISSING_ARG_V01;
    goto send_result;
  }

  /*Validation checks are at API called into already*/
  explicit_index = (int)req_msg->explicit_config_index;

  status = policyman_device_config_set_explicit_index(explicit_index);
  /*-1 is valid if it is modem set config else Index should point within array indices starting from 0*/
  if (POLICYMAN_FAILED(status))
  {
    errval = (status == POLICYMAN_STATUS_ERR_INVALID_ARGS)?
		         QMI_ERR_INVALID_ARG_V01:
		         QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

send_result:
  result = (errval == QMI_ERR_NONE_V01)?QMI_RESULT_SUCCESS_V01: 
                                        QMI_RESULT_FAILURE_V01;
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_device_cap_config_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}

/*===========================================================================
  FUNCTION QMI_DMSI_POWER_STATISTICS_CB()

  DESCRIPTION
    This call back is invoked when the power statitics event is sent from
    PS SYS layer
        
  PARAMETERS
    tech_type       : technology for which system status has changed
    event_name      : Event name
    subscription_id : Subscription identifier
    event_info_ptr  : Power statistics payload
    user_data_ptr   : Optional user data to be sent.
 
  RETURN VALUE
    void
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void dmsi_power_statistics_info_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
)
{
   dmsi_power_statistics_info_cb_ex( tech_type,
                                     event_name, 
                                     subscription_id, 
                                     event_info_ptr, 
                                     user_data_ptr,
                                     FALSE);
} /* dmsi_power_statistics_info_cb() */

/*===========================================================================
  FUNCTION QMI_DMSI_POWER_STATISTICS_CB_EX()

  DESCRIPTION
    This call back is invoked when the power statitics event is sent from
    PS SYS layer with cached_value support
        
  PARAMETERS
    tech_type       : technology for which system status has changed
    event_name      : Event name
    subscription_id : Subscription identifier
    event_info_ptr  : Power statistics payload
    user_data_ptr   : Optional user data to be sent.
    is_cached_value : Is Cached Value
 
 
  RETURN VALUE
    void
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void dmsi_power_statistics_info_cb_ex
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr,
  boolean                                        is_cached_value
)
{
  qmi_dmsi_cmd_buf_type                        * cmd_ptr = NULL;
  ps_sys_modem_power_stats_info_type           * power_stats_ptr = NULL;        
  qmi_dmsi_power_stats_info_type               * qmi_power_stats_ptr = NULL;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(event_info_ptr != NULL);
  power_stats_ptr = (ps_sys_modem_power_stats_info_type *)event_info_ptr;

  cmd_ptr = (qmi_dmsi_cmd_buf_type *)qmi_dmsi_get_cmd_buf();

  if( cmd_ptr == NULL)
  {
    return;
  }

  qmi_power_stats_ptr = &cmd_ptr->data.modem_power_stats;
  memscpy( &(qmi_power_stats_ptr->power_stats_info), 
           sizeof(ps_sys_modem_power_stats_info_type),
           power_stats_ptr, 
           sizeof(ps_sys_modem_power_stats_info_type) );

  qmi_power_stats_ptr->is_cached_value = is_cached_value;
  
  cmd_ptr->cmd_id = QMI_CMD_DMS_POWER_STATISTICS_EVT;
  cmd_ptr->subscription_id = subscription_id;
  dcc_send_cmd_ex(DCC_DMS_SVC_CMD, cmd_ptr);

} /* dmsi_power_statistics_info_cb() */

/*===========================================================================
  FUNCTION qmi_dmsi_generate_power_stats_ind()

  DESCRIPTION  Generate power statistics indication to all pending clients
   
  PARAMETERS
    activity_info: Modem activity info payload 

  RETURN VALUE
    None

  DEPENDENCIES
    
  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_generate_power_stats_ind 
( 
  qmi_dmsi_power_stats_info_type  *activity_info
)
{
  qmi_dmsi_client_info_type *    cl_sp;
  qmi_dmsi_power_stats_pend_q_type * pend_stats_q_node = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  while ( (pend_stats_q_node = 
          (qmi_dmsi_power_stats_pend_q_type *) q_get(&qmi_dms_state.modem_activity_pend_q)) != NULL )
  {
    cl_sp = pend_stats_q_node->cl_sp;
    if ( NULL ==  cl_sp )
    {
      PS_SYSTEM_HEAP_MEM_FREE(pend_stats_q_node);
      continue;
    }
    qmi_dmsi_modem_activity_info_ind( cl_sp, 
                                      activity_info );
    PS_SYSTEM_HEAP_MEM_FREE(pend_stats_q_node);
  }
}/* qmi_dmsi_generate_power_stats_ind */

/*===========================================================================
  FUNCTION QMI_DMSI_MODEM_ACTIVITY_INFO_IND()

  DESCRIPTION
    This indication is sent to the client interested in modem activity info
        
  PARAMETERS
    clid             : clientID
    activity_info    : activity information
 
  RETURN VALUE
    void
    
  DEPENDENCIES
 
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_modem_activity_info_ind
(
  qmi_dmsi_client_info_type             *clnt_info,
  qmi_dmsi_power_stats_info_type        *activity_info
)
{
  dms_activity_info_ind_msg_v01*     ind_msg = NULL;
  qmi_csi_error                      rc;
  uint32                             index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DMS_ASSERT(clnt_info != NULL);
  PS_SYSTEM_HEAP_MEM_ALLOC(ind_msg, sizeof(dms_activity_info_ind_msg_v01),
                           dms_activity_info_ind_msg_v01*);
  if(ind_msg == NULL)
  {
    return;
  }
  memset(ind_msg,0,sizeof(dms_activity_info_ind_msg_v01));

  ind_msg->idle_mode_duration_valid = TRUE;
  ind_msg->idle_mode_duration = 
    activity_info->power_stats_info.idle_mode_duration;

  ind_msg->sleep_mode_duration_valid = TRUE;
  ind_msg->sleep_mode_duration = 
    activity_info->power_stats_info.sleep_mode_duration;

  ind_msg->rx_mode_duration_valid = TRUE;
  ind_msg->rx_mode_duration = 
    activity_info->power_stats_info.rx_mode_duration;

  ind_msg->tx_mode_duration_valid = TRUE;
  ind_msg->tx_mode_duration_len = PS_SYS_MODEM_POWER_LEVEL_MAX;

  for (index = 0; index < PS_SYS_MODEM_POWER_LEVEL_MAX; index++) 
  {
    ind_msg->tx_mode_duration[index].tx_mode_level =
      (dms_tx_mode_level_enum_v01)activity_info->power_stats_info.tx_mode_duration[index].tx_mode_level;
    ind_msg->tx_mode_duration[index].tx_mode_duration =
      activity_info->power_stats_info.tx_mode_duration[index].tx_mode_duration;
  }

  ind_msg->modem_tech_active_time_valid = TRUE;
  ind_msg->modem_tech_active_time_len = PS_SYS_MODEM_POWER_MAX_RAT_TECH;

  for(index = 0; index < PS_SYS_MODEM_POWER_MAX_RAT_TECH; index++)
  {
    ind_msg->modem_tech_active_time[index].modem_power_tech = 
      (dms_modem_power_tech_enum_v01)activity_info->power_stats_info.modem_tech_active_time[index].modem_power_tech;

    ind_msg->modem_tech_active_time[index].modem_tech_active_time =
        activity_info->power_stats_info.modem_tech_active_time[index].modem_tech_active_time;
  }

  ind_msg->wlan_active_time_valid = TRUE;
  ind_msg->wlan_active_time = activity_info->power_stats_info.wlan_active_time;

  ind_msg->gps_active_time_valid = TRUE;
  ind_msg->gps_active_time = activity_info->power_stats_info.gps_active_time;

  ind_msg->modem_active_vote_status_valid = TRUE;
  ind_msg->modem_active_vote_status = activity_info->power_stats_info.modem_active_vote_status;

  ind_msg->wlan_active_vote_status_valid = TRUE;
  ind_msg->wlan_active_vote_status = activity_info->power_stats_info.wlan_active_vote_status;
  

  ind_msg->gps_active_vote_status_valid = TRUE;
  ind_msg->gps_active_vote_status = activity_info->power_stats_info.gps_active_vote_status;

  //By default true
  if (TRUE == activity_info->is_cached_value) 
  {
    ind_msg->is_newly_calculated_values_valid = TRUE;
    ind_msg->is_newly_calculated_values = FALSE;
  }
  else
  {
    ind_msg->is_newly_calculated_values_valid = TRUE;
    ind_msg->is_newly_calculated_values = TRUE;
  }

  rc = qmi_csi_send_ind( clnt_info->clnt,
                         QMI_DMS_MODEM_ACTIVITY_INFO_IND_V01,
                         (void*)ind_msg, 
                         sizeof(dms_activity_info_ind_msg_v01) );
  PS_SYSTEM_HEAP_MEM_FREE(ind_msg);

}/* qmi_dmsi_modem_activity_info_ind */

/*===========================================================================
  FUNCTION QMI_DMS_GENERATE_SET_UI_STATUS_RESP()

  DESCRIPTION
    Called when cmd_cb() supplied in cm_ph_cmd_set_ui_status_mode() is called 
    by CM. Sends response to control point for the corresponding
    Set ui status info Req
        
  PARAMETERS
    user_data   : user data
    ph_cmd_err  : error type
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dms_generate_set_ui_status_resp
(
  void *               user_data,
  cm_ph_cmd_err_e_type ph_cmd_err
)
{
  qmi_error_type_v01                  errval = QMI_ERR_NONE_V01 ;
  qmi_result_type_v01                 result;
  ds_qmi_dms_async_cmd_type*          qmi_resp_buf_p = NULL;
  dms_set_ui_status_info_resp_msg_v01  resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  qmi_resp_buf_p = (ds_qmi_dms_async_cmd_type*)(user_data);
  
  if(qmi_resp_buf_p == NULL )
  {
    LOG_MSG_INFO1_0("Invalid resp_buf_ptr");
    return;
  }
  /*-------------------------------------------------------------------------
    Send SUCCESS or FAILURE response to control point based on ph_cmd_err.
  -------------------------------------------------------------------------*/
  switch (ph_cmd_err)
  {
    case CM_PH_CMD_ERR_NOERR:
      errval = QMI_ERR_NONE_V01;
      break;
    case CM_PH_CMD_ERR_NO_BUF_L:
      errval = QMI_ERR_NO_MEMORY_V01;
      break;
    default:
      errval = QMI_ERR_INTERNAL_V01;
      break;
  }

 /* ----------------------------------------------------------
   Update the Gloabl data once the CM processes the UI STATUS INFO sucessfully
   ui_ready_status value is boolean changes TRUE <--> FALSE only
  -----------------------------------------------------------*/
  if(errval == QMI_ERR_NONE_V01)
  {
    qmi_dmsi_global.ui_ready_status = (boolean)qmi_resp_buf_p->user_data;
  }

  if ( qmi_dms_check_valid_async_cmd_buf(qmi_resp_buf_p) == FALSE )
  {
    PS_SYSTEM_HEAP_MEM_FREE(qmi_resp_buf_p);
    return;
  }

  memset(&resp_msg,0,sizeof(dms_set_ui_status_info_resp_msg_v01));

  result = (errval == QMI_ERR_NONE_V01? QMI_RESULT_SUCCESS_V01
                                   : QMI_RESULT_FAILURE_V01);

  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(qmi_resp_buf_p->req_handle,
                         qmi_resp_buf_p->msg_id,
                         &resp_msg,
                         sizeof(dms_set_ui_status_info_resp_msg_v01));
  PS_SYSTEM_HEAP_MEM_FREE(qmi_resp_buf_p);
} /* qmi_dms_generate_set_ui_status_resp() */

/*===========================================================================
FUNCTION QMI_DMS_IS_CLIENT_POWER_STATS_QUERY_PENDING

  DESCRIPTION
  Queue Compare Function
    Used by the queue searching functions to determine if an item is in
       the queue.

  DEPENDENCIES
    None

  RETURN VALUE
  Returns TRUE if the element should be operated upon, FALSE otherwise
    
  SIDE EFFECTS
  None.
===========================================================================*/
int qmi_dms_is_client_power_stats_query_pending
( 
  qmi_dmsi_client_info_type *  q_cl_cp,
  qmi_dmsi_client_info_type *  delete_cl_sp
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return (q_cl_cp == delete_cl_sp) ? TRUE : FALSE;
} /* qmi_dms_is_client_power_stats_query_pending */

/*===========================================================================
  FUNCTION QMI_DMSI_REG_SYS_EVENTS()

  DESCRIPTION
    Registers for PS system level events

  PARAMETERS
    None

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_reg_sys_events
( 
  void
)
{
  ps_sys_event_enum_type   event_id   = PS_SYS_EVENT_MIN;
  int16                    result = 0;
  int16                    dss_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                PS_SYS_EVENT_MODEM_POWER_STATISTICS,
                                PS_SYS_PRIMARY_SUBS,
                                dmsi_power_statistics_info_cb,
                                NULL,
                                &dss_errno);
  if(result < 0)
  {
    event_id = PS_SYS_EVENT_MODEM_POWER_STATISTICS;
    goto send_result;
  }

send_result:
  if(result < 0)
  {
    LOG_MSG_ERROR_INT_2("Couldn't reg for ps sys event_id = d Err : (d)",
                    event_id, dss_errno);
    DS_QMI_DMS_ASSERT(0);
    return;
  }
}/* qmi_dmsi_reg_sys_events */

/*===========================================================================
  FUNCTION QMI_DMSI_INDICATION_REGISTER()

  DESCRIPTION
    Registers the requesting control point for various indications. 

  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_indication_register
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_indication_register_req_msg_v01  * req_msg;
  dms_indication_register_resp_msg_v01   resp_msg;
  boolean                                got_one_tlv = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("qmi_dmsi_indication_register()");
  DS_QMI_DMS_ASSERT(req_c_struct != NULL);
  req_msg = (dms_indication_register_req_msg_v01 *)req_c_struct;
  memset(&resp_msg, 0, sizeof(resp_msg));

  // Process the request
  if (req_msg->report_ims_capability_valid)
  {
    got_one_tlv = TRUE;
    clnt_info->report_status.report_ims_capability =
                        req_msg->report_ims_capability ? TRUE : FALSE;
  }
  if (req_msg->report_device_state_valid)
  {
    got_one_tlv = TRUE;
    clnt_info->report_status.report_device_state =
                        req_msg->report_device_state ? TRUE : FALSE;
  }

  // Check if at least one TLV is present
  if (got_one_tlv)
  {
    resp_msg.resp.error = QMI_ERR_NONE_V01;
    resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  }
  else
  {
    resp_msg.resp.error = QMI_ERR_MISSING_ARG_V01;
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
  }

  // Send response
  qmi_dmsi_send_response(req_handle, msg_id, &resp_msg, sizeof(resp_msg));
  return QMI_CSI_CB_NO_ERR;

} /* qmi_dmsi_indication_register() */

/*===========================================================================
  FUNCTION QMI_DMSI_GENERATE_IMS_CAPABILITY_IND()

  DESCRIPTION
    Generate IMS capability indication.

  PARAMETERS
    ind_msg : indication message
    cl_sp   : Client information

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dmsi_generate_ims_capability_ind
(
  dms_ims_capability_ind_msg_v01  * ind_msg,
  qmi_dmsi_client_info_type       * cl_sp
)
{
  qmi_csi_error                     rc;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rc = qmi_csi_send_ind(cl_sp->clnt,
                        QMI_DMS_IMS_CAPABILITY_IND_V01,
                        ind_msg,
                        sizeof(dms_ims_capability_ind_msg_v01));
  if (rc != QMI_CSI_NO_ERR)
  {
    LOG_MSG_ERROR_INT_1("Failed to report IMS mode change, error %d", rc);
  }

  return;
} /* qmi_dmsi_generate_ims_capability_ind */

/*===========================================================================
  FUNCTION QMI_DMSI_SET_AP_VERSION()
  
  
  DESCRIPTION
    Update the AP operating system Version
  	  
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None
  
  SIDE EFFECTS
    None
  ===========================================================================*/
static qmi_csi_cb_error qmi_dmsi_set_ap_version
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_set_ap_version_req_msg_v01*      req_msg;
  dms_set_ap_version_resp_msg_v01      resp_msg;
  ps_sys_ioctl_3gpp_ap_version_info_type  ap_version_info;
  sw_version_s_type                       sw_info;
  int16                                   ps_errno = 0;
  int16                                   result = 0;
  qmi_dmsi_local_err_type                 local_error = QMI_DMSI_LOCAL_ERR_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_UTILS_ASSERT(req_c_struct != NULL);
  req_msg = (dms_set_ap_version_req_msg_v01*)req_c_struct;
  memset((void *)&resp_msg, 0, sizeof(dms_set_ap_version_resp_msg_v01));
  memset(&ap_version_info,0,sizeof(ap_version_info));
  memset(&sw_info,0,sizeof(sw_info));

  resp_msg.resp.error = QMI_ERR_NONE_V01;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;

  if((!req_msg->ap_os_version_valid) && 
     (!req_msg->ap_sw_version_valid) && 
     (!req_msg->ap_wlan_macid_valid))
  {
    resp_msg.resp.error = QMI_ERR_MISSING_ARG_V01;
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    goto send_result;
  }

  if(req_msg->ap_os_version_valid)
  {
    strlcpy((char*)ap_version_info.version_info, req_msg->ap_os_version, sizeof(ap_version_info.version_info));
    ap_version_info.version_info_length = strlen(req_msg->ap_os_version);
    //inform AP OS info to DS
    result = ps_sys_ioctl_ex(PS_SYS_TECH_3GPP,
                             PS_SYS_IOCTL_3GPP_SET_OS_VERSION_INFO,
                            (ps_sys_subscription_enum_type)(clnt_info->subscription_id + 1),
                             &ap_version_info,
                             &ps_errno);
    if(result != 0)
    {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      switch(ps_errno)
      {
        case DS_ENOMEM:
          resp_msg.resp.error  = QMI_ERR_NO_MEMORY_V01;
          break;
        default:
          resp_msg.resp.error  = QMI_ERR_NOT_SUPPORTED_V01;
          break;
      }
      local_error = QMI_DMSI_LOCAL_ERR_NOT_PROVISIONED;
      goto send_result;
    }
  }

  memset(&ap_version_info,0,sizeof(ap_version_info));
  ps_errno = 0;
  result = 0;
  if(req_msg->ap_wlan_macid_valid)
  {
    //Inform AP MACID information to DS
    strlcpy((char*)ap_version_info.version_info,req_msg->ap_wlan_macid,sizeof(ap_version_info.version_info));
    ap_version_info.version_info_length = strlen(req_msg->ap_wlan_macid);
    result = ps_sys_ioctl_ex(PS_SYS_TECH_3GPP,
                             PS_SYS_IOCTL_3GPP_SET_AP_WLAN_MACID,
                            (ps_sys_subscription_enum_type)(clnt_info->subscription_id + 1),
                             &ap_version_info,
                             &ps_errno);
    if(result != 0)
    {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      switch(ps_errno)
      {
        case DS_ENOMEM:
          resp_msg.resp.error  = QMI_ERR_NO_MEMORY_V01;
          break;
        default:
          resp_msg.resp.error  = QMI_ERR_NOT_SUPPORTED_V01;
          break;
      }
      local_error = QMI_DMSI_LOCAL_ERR_NOT_PROVISIONED;
      goto send_result;
    }
  }

  memset(&ap_version_info,0,sizeof(ap_version_info));
  ps_errno = 0;
  result = 0;
  if(req_msg->ap_sw_version_valid)
  {
    //Call CM API to Set APSS Sw Version
    strlcpy((char*)sw_info.sw_version,req_msg->ap_sw_version,sizeof(sw_info.sw_version));
    sw_info.length = strlen(req_msg->ap_sw_version);
    cm_set_apss_sw_version (&sw_info);

    //Inform AP SW version information to DS, Must be the last field passing to Appsrv
    strlcpy((char*)ap_version_info.version_info,req_msg->ap_sw_version,sizeof(ap_version_info.version_info));
    ap_version_info.version_info_length = strlen(req_msg->ap_sw_version);
    result = ps_sys_ioctl_ex(PS_SYS_TECH_3GPP,
                             PS_SYS_IOCTL_3GPP_SET_SW_VERSION_INFO,
                            (ps_sys_subscription_enum_type)(clnt_info->subscription_id + 1),
                             &ap_version_info,
                             &ps_errno);
    if(result != 0)
    {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      switch(ps_errno)
      {
        case DS_ENOMEM:
          resp_msg.resp.error  = QMI_ERR_NO_MEMORY_V01;
          break;
        default:
          resp_msg.resp.error  = QMI_ERR_NOT_SUPPORTED_V01;
          break;
      }
      local_error = QMI_DMSI_LOCAL_ERR_NOT_PROVISIONED;
      goto send_result;
    }
  }

send_result:
  LOG_MSG_INFO1_3("qmi_dmsi_set_ap_version result %d ps_errno %d local_error %d", result, ps_errno, local_error);
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_ap_version_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
} /* qmi_dmsi_set_ap_version() */

/*===========================================================================
  FUNCTION QMI_DMS_GENERATE_DEVICE_STATE_INFO_IND()

  DESCRIPTION
    Called when CM notifies us that the device state
    has changed.
    Sends an indication with the new operating mode to clients registered for
    the event.
        
  PARAMETERS
    rf_init_status: RF initialization status
    is_gps_init_completed: GPS initialization status
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dms_generate_device_state_info_ind
(
  cm_rf_init_s_type    *rf_init_status,
  boolean              is_gps_init_completed
)
{
  qmi_dmsi_client_info_type *         cl_sp = NULL;
  dms_device_state_info_ind_msg_v01   ind_msg;
  qmi_csi_error                       rc;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!qmi_dmsi_inited)
  {
    return;
  }
  LOG_MSG_INFO1_3("qmi_dms_generate_device_state_info_ind enter error code %d status %d is GPS ready %d", 
  rf_init_status->error_code,
  rf_init_status->status,
  is_gps_init_completed);

  memset(&ind_msg, 0, sizeof(dms_device_state_info_ind_msg_v01));

  if(rf_init_status->status == CM_RF_INIT_UNKNOWN)
  {
    ind_msg.init_state = DMS_INIT_STATE_IN_PROGRESS_V01;
  }
  else if(is_gps_init_completed && rf_init_status->status == CM_RF_INIT_READY)
  {
    ind_msg.init_state = DMS_INIT_STATE_READY_V01;
  }
  else
  {
    ind_msg.init_state = DMS_INIT_STATE_FAILED_V01;
    ind_msg.failure_reason_valid  = TRUE;

    if(rf_init_status->status == CM_RF_INIT_FAILED)
    {
      switch(rf_init_status->error_code)
      {
        case CM_RF_FAILURE_HW:
          ind_msg.failure_reason = DMS_DEVICE_STATE_FAIL_RF_HW_V01;
          break;
        case CM_RF_FAILURE_SW:
          ind_msg.failure_reason = DMS_DEVICE_STATE_FAIL_RF_SW_V01;
          break;
        default:
          LOG_MSG_INFO1_0("qmi_dms_generate_device_state_info_ind invalid error ");
          break;
      }
    }
    else
    {
      if(!is_gps_init_completed)
      {
        ind_msg.failure_reason = DMS_DEVICE_STATE_FAIL_GPS_V01;
      }
      else
      {
        LOG_MSG_INFO1_0("qmi_dms_generate_device_state_info_ind invalid input");
      }
    }
  }
  cl_sp = (qmi_dmsi_client_info_type*)q_check(&qmi_dms_state.client_q);
  while(cl_sp != NULL)
  {
    if(cl_sp->report_status.report_device_state)
    {
      rc = qmi_csi_send_ind( cl_sp->clnt,
                           QMI_DMS_DEVICE_STATE_INFO_IND_V01,
                           (void*)&ind_msg, 
                           sizeof(dms_device_state_info_ind_msg_v01) );

    }
    cl_sp = q_next(&qmi_dms_state.client_q,&(cl_sp->link));
  }

} /* qmi_dms_generate_device_state_info_ind() */

/*===========================================================================
  FUNCTION QMI_DMSI_GET_DEVICE_STATE_INFO()

  DESCRIPTION
    Retrieve the device state informtaion
  	  
  PARAMETERS
    req_c_struct       :       Request Message with all TLV
    req_handle         :       Request Handle
    clnt_info          :       Client Info Handle
    msg_id             :       Message ID
    req_c_struct_len   :       length of request message
    service_handle     :       DMS service handle

  RETURN VALUE
    qmi_csi_cb_error types.
    
  DEPENDENCIES
    None
  
  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error    qmi_dmsi_get_device_state_info
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  dms_get_device_state_info_resp_msg_v01       resp_msg;
  qmi_if_info_type  *qmi_if_info_ptr;
/*-------------------------------------------------------*/
  DS_QMI_DMS_ASSERT(req_c_struct != NULL || clnt_info != NULL);
  memset(&resp_msg ,0,sizeof(dms_get_device_state_info_resp_msg_v01));

  qmi_if_info_ptr = qmi_if_info_get_ptr();
          LOG_MSG_INFO1_3("qmi_dmsi_get_device_state_info rf status %d rf reason %d gps complted? %d",
  qmi_if_info_ptr->ph_info.rf_init_status.status,
  qmi_if_info_ptr->ph_info.rf_init_status.error_code,
  qmi_if_info_ptr->ph_info.is_gps_init_completed);

  resp_msg.init_state_valid = TRUE; 
  if(qmi_if_info_ptr->ph_info.rf_init_status.status == CM_RF_INIT_UNKNOWN)
  {
    resp_msg.init_state = DMS_INIT_STATE_IN_PROGRESS_V01;
  }
  else if(qmi_if_info_ptr->ph_info.rf_init_status.status == CM_RF_INIT_READY &&
     qmi_if_info_ptr->ph_info.is_gps_init_completed)
  {
    resp_msg.init_state = DMS_INIT_STATE_READY_V01;
  }
  else
  {
    resp_msg.init_state = DMS_INIT_STATE_FAILED_V01;
    resp_msg.failure_reason_valid = TRUE;

    if(qmi_if_info_ptr->ph_info.rf_init_status.status == CM_RF_INIT_FAILED)
    {
      switch(qmi_if_info_ptr->ph_info.rf_init_status.error_code)
      {
        case CM_RF_FAILURE_HW:
          resp_msg.failure_reason = DMS_DEVICE_STATE_FAIL_RF_HW_V01;
          break;
        case CM_RF_FAILURE_SW:
          resp_msg.failure_reason = DMS_DEVICE_STATE_FAIL_RF_SW_V01;
          break;
        default:
          LOG_MSG_INFO1_0("qmi_dmsi_get_device_state_info invalid error ");
          break;
      }
    }
    else
    {
      if(!qmi_if_info_ptr->ph_info.is_gps_init_completed)
      {
         resp_msg.failure_reason = DMS_DEVICE_STATE_FAIL_GPS_V01;
      }
      else
      {
        LOG_MSG_INFO1_0("qmi_dms_generate_device_state_info_ind invalid input");
      }
    }
  }

  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                          sizeof(resp_msg));
  return QMI_CSI_CB_NO_ERR;
}/*qmi_dmsi_get_device_state_info*/
static qmi_csi_cb_error qmi_dmsi_get_whole_2498
(
    qmi_dmsi_client_info_type*  clnt_info,
    qmi_req_handle              req_handle,
    int                         msg_id,
    void*                       req_c_struct,
    int                         req_c_struct_len,
    void*                       service_handle
)
{
  LOG_MSG_INFO2_0("modem qmi_dmsi_get_whole_2498() start");
  dms_get_whole_2498_resp_msg_v01       resp_msg;
	qmi_error_type_v01                  errval = QMI_ERR_NONE_V01;
	qmi_result_type_v01                 result;
	nv_item_type                        nv_item;
	nv_stat_enum_type                   nv_status;
	memset(&nv_item, 0, sizeof(nv_item));
	memset(&resp_msg, 0, sizeof(resp_msg));
	nv_status = dcc_get_nv_item(NV_FACTORY_DATA_2_I , &nv_item);
	LOG_MSG_INFO2_1("modem qmi_dmsi_get_whole_2498() read nv_status:%d\n", nv_status);
	if (nv_status != NV_DONE_S)
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
    goto send_result;
	}
	for(int i =0; i < 128; i++)
  {
    if (nv_item.factory_data_2[i] == 0)
    {
      resp_msg.whole_2498[i] = '\0';
      break;
    }
    resp_msg.whole_2498[i]= nv_item.factory_data_2[i];
	}
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_get_whole_2498() whole_2498:%s\n", resp_msg.whole_2498);
	send_result:
    result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
      : QMI_RESULT_FAILURE_V01);
    resp_msg.resp.error = errval;
    resp_msg.resp.result = result;
    qmi_dmsi_send_response(req_handle,
                          msg_id,
                          &resp_msg,
                          sizeof(dms_get_whole_2498_resp_msg_v01));
    return QMI_CSI_CB_NO_ERR;
	
}/*qmi_dmsi_get_whole_2498*/
	
static qmi_csi_cb_error  qmi_dmsi_set_whole_2498
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01               errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01              result;
  dms_set_whole_2498_req_msg_v01*    req_msg = NULL;
  dms_set_whole_2498_resp_msg_v01    resp_msg;
  //nv_stat_enum_type                nv_status;
  char whole_2498_set[WHOLE_FACTORY_MAX_V01 + 1] = {0};
  int i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_0("modem qmi_dmsi_set_whole_2498() start");
  ASSERT(clnt_info && req_c_struct);
  req_msg = (dms_set_whole_2498_req_msg_v01*)req_c_struct;
  memset(&resp_msg,0,sizeof(dms_set_whole_2498_resp_msg_v01));
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2498() req_msg->whole_2498 :%s", req_msg->whole_2498);
  memcpy(whole_2498_set, req_msg->whole_2498, WHOLE_FACTORY_MAX_V01);
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2498() whole_2498 to be set :%s", whole_2498_set);
  for (i = 0; i < 128; i++){
    dcc_nv_item.factory_data_2[i] = whole_2498_set[i];
    DS_AT_MSG_SPRINTF_2_HIGH("modem qmi_dmsi_set_whole_2498() dcc_nv_item.factory_data_2[%d] :%d", i, dcc_nv_item.factory_data_2[i]);
  }
  if (NV_DONE_S != dcc_put_nv_item(NV_FACTORY_DATA_2_I , &dcc_nv_item))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_0("Could not Update new whole 2498");
    goto send_result;
  }
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01: QMI_RESULT_FAILURE_V01);
	resp_msg.resp.error = errval;
	resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_whole_2498_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
	
}/* qmi_dmsi_set_whole_2498 */
	
	
//for product ID
static qmi_csi_cb_error qmi_dmsi_get_whole_2497
(
    qmi_dmsi_client_info_type*  clnt_info,
    qmi_req_handle              req_handle,
    int                         msg_id,
    void*                       req_c_struct,
    int                         req_c_struct_len,
    void*                       service_handle
)
{
  LOG_MSG_INFO2_0("modem qmi_dmsi_get_whole_2497() start");
	dms_get_whole_2497_resp_msg_v01       resp_msg;
	qmi_error_type_v01                  errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01                 result;
	nv_item_type                        nv_item;
	nv_stat_enum_type                   nv_status;
	memset(&nv_item, 0, sizeof(nv_item));
	memset(&resp_msg, 0, sizeof(resp_msg));
	nv_status = dcc_get_nv_item(NV_FACTORY_DATA_1_I , &nv_item);
	LOG_MSG_INFO2_1("modem qmi_dmsi_get_whole_2497() read nv_status:%d\n", nv_status);
	if (nv_status != NV_DONE_S)
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
    goto send_result;
	}
  for(int i =0; i < 128; i++)
  {
    if(nv_item.factory_data_1[i] == 0){
        resp_msg.whole_2497[i] = '\0';
        break;
    }
    resp_msg.whole_2497[i]= nv_item.factory_data_1[i];
  }
	
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_get_whole_2497() whole_2497:%s\n", resp_msg.whole_2497);
  send_result:
    result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
      : QMI_RESULT_FAILURE_V01);
    resp_msg.resp.error = errval;
    resp_msg.resp.result = result;
   qmi_dmsi_send_response(req_handle,
                          msg_id,
                          &resp_msg,
                          sizeof(dms_get_whole_2497_resp_msg_v01));
	
    return QMI_CSI_CB_NO_ERR;
	
}/*qmi_dmsi_get_whole_2497*/
	
	
static qmi_csi_cb_error qmi_dmsi_set_whole_2497
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01               errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01              result;
  dms_set_whole_2497_req_msg_v01*  req_msg = NULL;
  dms_set_whole_2497_resp_msg_v01  resp_msg;
  //nv_stat_enum_type              nv_status;
  char whole_2497_set[WHOLE_FACTORY_MAX_V01 + 1] = {0};
  int i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_0("modem qmi_dmsi_set_whole_2497() start");
  ASSERT(clnt_info && req_c_struct);
  req_msg = (dms_set_whole_2497_req_msg_v01*)req_c_struct;
  memset(&resp_msg, 0, sizeof(dms_set_whole_2497_resp_msg_v01));
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2497() req_msg->whole_2497 :%s", req_msg->whole_2497);
  memcpy(whole_2497_set, req_msg->whole_2497, WHOLE_FACTORY_MAX_V01);
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2497() whole_2497 to be set :%s", whole_2497_set);
  for (i = 0; i < 128; i++){
    dcc_nv_item.factory_data_1[i] = whole_2497_set[i];
    DS_AT_MSG_SPRINTF_2_HIGH("modem qmi_dmsi_set_whole_2497() dcc_nv_item.factory_data_1[%d] :%d", i, dcc_nv_item.factory_data_1[i]);
  }
  if (NV_DONE_S != dcc_put_nv_item(NV_FACTORY_DATA_1_I, &dcc_nv_item))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_0("Could not Update new whole 2497");
    goto send_result;
  }
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                       : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_whole_2497_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_set_whole_2497 */
	
	
//for Module No
	
static qmi_csi_cb_error qmi_dmsi_get_whole_2499
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  LOG_MSG_INFO2_0("modem qmi_dmsi_get_whole_2499() start");
  dms_get_whole_2499_resp_msg_v01  resp_msg;
  qmi_error_type_v01               errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01              result;
  nv_item_type                     nv_item;
  nv_stat_enum_type                nv_status;
  memset(&nv_item, 0, sizeof(nv_item));
  memset(&resp_msg, 0, sizeof(resp_msg));
  nv_status = dcc_get_nv_item(NV_FACTORY_DATA_3_I, &nv_item);
  LOG_MSG_INFO2_1("modem qmi_dmsi_get_whole_2499() read nv_status:%d\n", nv_status);
  if (nv_status != NV_DONE_S)
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
    goto send_result;
  }
  for (int i = 0; i < 128; i++)
  {
    if (nv_item.factory_data_3[i] == 0)
    {
      resp_msg.whole_2499[i] = '\0';
      break;
    }
    resp_msg.whole_2499[i] = nv_item.factory_data_3[i];
  }
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_get_whole_2499() whole_2499:%s\n", resp_msg.whole_2499);
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                       : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_whole_2499_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_get_whole_2499 */
	
	
static qmi_csi_cb_error qmi_dmsi_set_whole_2499
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01               errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01              result;
  dms_set_whole_2499_req_msg_v01*  req_msg = NULL;
  dms_set_whole_2499_resp_msg_v01  resp_msg;
  //nv_stat_enum_type              nv_status;
  char whole_2499_set[WHOLE_FACTORY_MAX_V01 + 1] = {0};
  int i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_0("modem qmi_dmsi_set_whole_2499() start");
  ASSERT(clnt_info && req_c_struct);
  req_msg = (dms_set_whole_2499_req_msg_v01*)req_c_struct;
  memset(&resp_msg, 0, sizeof(dms_set_whole_2499_resp_msg_v01));
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2499() req_msg->whole_2499 :%s", req_msg->whole_2499);
  memcpy(whole_2499_set, req_msg->whole_2499, WHOLE_FACTORY_MAX_V01);
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2499() whole_2499 to be set :%s", whole_2499_set);
  for (i = 0; i < 128; i++){
    dcc_nv_item.factory_data_3[i] = whole_2499_set[i];
    DS_AT_MSG_SPRINTF_2_HIGH("modem qmi_dmsi_set_whole_2499() dcc_nv_item.factory_data_3[%d] :%d", i, dcc_nv_item.factory_data_3[i]);
  }
  if (NV_DONE_S != dcc_put_nv_item(NV_FACTORY_DATA_3_I, &dcc_nv_item))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_0("Could not Update new whole 2499");
    goto send_result;
  }
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                       : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_whole_2499_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_set_whole_2499 */
	
static qmi_csi_cb_error qmi_dmsi_get_whole_2500
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  LOG_MSG_INFO2_0("modem qmi_dmsi_get_whole_2500() start");
  dms_get_whole_2500_resp_msg_v01  resp_msg;
  qmi_error_type_v01               errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01              result;
  nv_item_type                     nv_item;
  nv_stat_enum_type                nv_status;
  memset(&nv_item, 0, sizeof(nv_item));
  memset(&resp_msg, 0, sizeof(resp_msg));
  nv_status = dcc_get_nv_item(NV_FACTORY_DATA_4_I, &nv_item);
  LOG_MSG_INFO2_1("modem qmi_dmsi_get_whole_2500() read nv_status:%d\n", nv_status);
  if (nv_status != NV_DONE_S)
  {
    errval = QMI_ERR_NOT_PROVISIONED_V01;
    goto send_result;
  }
  for (int i = 0; i < 127; i++)
  {
    if (nv_item.factory_data_4[i] == 0)
    {
      resp_msg.whole_2500[i] = '\0';
      break;
    }
    resp_msg.whole_2500[i] = nv_item.factory_data_4[i];
  }
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_get_whole_2500() whole_2500:%s\n", resp_msg.whole_2500);
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                       : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_get_whole_2500_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_get_whole_2500 */
	
	
static qmi_csi_cb_error qmi_dmsi_set_whole_2500
(
  qmi_dmsi_client_info_type*  clnt_info,
  qmi_req_handle              req_handle,
  int                         msg_id,
  void*                       req_c_struct,
  int                         req_c_struct_len,
  void*                       service_handle
)
{
  qmi_error_type_v01               errval = QMI_ERR_NONE_V01;
  qmi_result_type_v01              result;
  dms_set_whole_2500_req_msg_v01*  req_msg = NULL;
  dms_set_whole_2500_resp_msg_v01  resp_msg;
  //nv_stat_enum_type              nv_status;
  char whole_2500_set[WHOLE_FACTORY_MAX_V01 + 1] = {0};
  int i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_0("modem qmi_dmsi_set_whole_2500() start");
  ASSERT(clnt_info && req_c_struct);
  req_msg = (dms_set_whole_2500_req_msg_v01*)req_c_struct;
  memset(&resp_msg, 0, sizeof(dms_set_whole_2500_resp_msg_v01));
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2500() req_msg->whole_2500 :%s", req_msg->whole_2500);
  memcpy(whole_2500_set, req_msg->whole_2500, WHOLE_FACTORY_MAX_V01);
  DS_AT_MSG_SPRINTF_1_HIGH("modem qmi_dmsi_set_whole_2500() whole_2500 to be set :%s", whole_2500_set);
  for (i = 0; i < 128; i++){
    dcc_nv_item.factory_data_4[i] = whole_2500_set[i];
    DS_AT_MSG_SPRINTF_2_HIGH("modem qmi_dmsi_set_whole_2500() dcc_nv_item.factory_data_4[%d] :%d", i, dcc_nv_item.factory_data_4[i]);
  }
  if (NV_DONE_S != dcc_put_nv_item(NV_FACTORY_DATA_4_I, &dcc_nv_item))
  {
    errval = QMI_ERR_INTERNAL_V01;
    LOG_MSG_ERROR_0("Could not Update new whole 2500");
    goto send_result;
  }
send_result:
  result = (errval == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01
                                       : QMI_RESULT_FAILURE_V01);
  resp_msg.resp.error = errval;
  resp_msg.resp.result = result;
  qmi_dmsi_send_response(req_handle,
                         msg_id,
                         &resp_msg,
                         sizeof(dms_set_whole_2500_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}/* qmi_dmsi_set_whole_2500 */



#ifndef TEST_FRAMEWORK
   #pragma GCC diagnostic pop
#endif
